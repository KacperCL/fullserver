#include "main.h"

using namespace std;

void openBrowser(LPCSTR url)
{
	ShellExecuteA(NULL, "open", url, NULL, NULL, SW_SHOWNORMAL);
}

// i hate looking at this functions but i don't have time to rewrite it. don't copy it anywhere, it's shit!
void runGame(const char* player, char ip[], int port, char pass[])
{
	char szCmdLine[MAX_PATH] = "";
	if (strlen(pass) > 1){
		sprintf_s(szCmdLine, "gta_sa.exe -c -n %.20s -h %.48s -p %d -z %.48s", player, ip, port, pass);
	}
	else{
		sprintf_s(szCmdLine, "gta_sa.exe -c -n %.20s -h %.48s -p %d", player, ip, port);
	}
	
	STARTUPINFOA si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	HWND hwnd = NULL;
	::CreateProcessA(
		NULL,
		szCmdLine,
		NULL,
		NULL,
		FALSE,
		0,
		NULL,
		NULL,
		&si,
		&pi);

	DWORD pid = GetProcId((char*)"gta_sa.exe"); // case sensitive! grr!

	if (pid == NULL) {
		pid = GetProcId((char*)"gta_sa.EXE");
	}

	if (pid == NULL) {
		pid = GetProcId((char*)"GTA_SA.exe");
	}
	
	if (pid == NULL) {
		pid = GetProcId((char*)"GTA_SA.EXE");
	}

	if (pid == NULL) {
		printf("[DEBUG:RUN] FAILED TO OPEN GAME!\n", pid);
		return;
	}

	HANDLE hproc = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_WRITE | PROCESS_VM_READ | PROCESS_VM_OPERATION, FALSE, pid);
	printf("[DEBUG:RUN] PID = %i\n", pid);

	PVOID addr = VirtualAllocEx(
		hproc,
		NULL,
		0x5000,
		MEM_COMMIT | MEM_RESERVE,
		PAGE_EXECUTE_READWRITE);
	printf("[DEBUG:RUN] ADDR = %.8x\n", addr);

	WriteProcessMemory(hproc, addr, "samp.dll", 9, NULL);

	HANDLE hthread = CreateRemoteThread(
		hproc,
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)
		GetProcAddress(GetModuleHandleA("kernel32"), "LoadLibraryA"),
		addr,
		0,
		NULL);

	CloseHandle(hthread);
	CloseHandle(hproc);
}

// another shitty function
DWORD GetProcId(char* ProcName)
{
	PROCESSENTRY32   pe32;
	HANDLE         hSnapshot = NULL;

	pe32.dwSize = sizeof(PROCESSENTRY32);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	if (Process32First(hSnapshot, &pe32))
	{
		do{
			if (strcmp(pe32.szExeFile, ProcName) == 0)
				return pe32.th32ProcessID;
		} while (Process32Next(hSnapshot, &pe32));
	}

	if (hSnapshot != INVALID_HANDLE_VALUE)
		CloseHandle(hSnapshot);

	return 0;
}


void toInt(std::string from, int &to)
{
	int result = 0;
	int count = from.length();
	for (int a = 0; a < count; a++)
	{
		if (from[a] >= '0' && (char)from[a] <= '9')
		{
			result += (from[a] - 48) * pow(count - a - 1);
		}
		else result = result / 10;
	}
	to = result;
}

int pow(int a)
{
	int multiply_by = 1;
	for (int b = 0; b < a; b++)
	{
		multiply_by *= 10;
	}
	return multiply_by;
}