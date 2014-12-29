#include "updaterForm.h"
#include "mainWindow.h"

using namespace System;
using namespace System::Windows::Forms;

[STAThread]
void Main(array<String^>^ args)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);

	//fsLauncher::mainWindow form;
	//Application::Run();

	fsLauncher::updaterForm^ form = gcnew fsLauncher::updaterForm();
	fsLauncher::mainWindow^ formMain = gcnew fsLauncher::mainWindow();

	form->ShowDialog();

	formMain->ShowDialog();
}