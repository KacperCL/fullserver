#pragma once
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "advapi32")
#pragma comment(lib, "ws2_32.lib")

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <vcclr.h>
#include <tlhelp32.h>
#include <string>
#include <exception>

#include "md5wrapper.h"

DWORD GetProcId(char* ProcName);
void openBrowser(LPCSTR url);
void runGame(const char* player, char ip[], int port, char pass[]);

void toInt(std::string from, int &to);
int pow(int a);

// sorry but... fuck unicode!
#undef PROCESSENTRY32
#undef Process32First
#undef Process32Next

#define VERSION 1300
#define PLAYER_NAME "Player"