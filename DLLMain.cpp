#include "Utils/Utils.h"
#include "Utils/Backend/Backend.h"
#include "Utils/Hooks/Hooks.h"
#include <iostream>
#include <intrin.h>
#include "Utils/BNM-IL2CPP/BNM.hpp"
#include <cstdint>
#include <string>

#define QWORD int64_t

// Ain't no party like a diddy party
QWORD WINAPI MainThread(LPVOID param)
{
	IL2CPP::Initialize();
	RunBackend.Load();
	AllocConsole();
	FILE* fp;
	freopen_s(&fp, "CONOUT$", "w", stdout);;
	SetConsoleTitleA("Moonlight | https://discord.gg/Uw48gqKxNF");
	const HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);

	Hooks();

	if (Hooks)
	{

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
		printf("      00000000      \n");
		printf("   000000000        \n");
		printf(" 000000000          \n");
		printf(" 00000000           \n");
		printf("000000000 Moonlight \n");
		printf("000000000           \n");
		printf("0000000000          \n");
		printf("0000000000          \n");
		printf(" 000000000000       \n");
		printf(" 0000000000000000000\n");
		printf("   00000000000000000\n");
		printf("      000000000000  \n\n");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
		printf("Initializing...\n");

		Sleep(100);
		printf("Hooking Initialized!\n\n");

		Sleep(450);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
		printf("[ OPENING ]");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
		printf(" Press Right Control to open the menu.\n");

		Sleep(100);
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
		printf("[ CREDITS ]");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
		printf(" @dsyq For helping me with hacks.\n");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
		printf("[ CREDITS ]");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
		printf(" zzzmate For DX11 Base. (https://github.com/zzzmate/DX11-Hook-ImGui)\n\n");

	}

	else
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
		printf("[ ERROR ]");

		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY);
		printf(" Failed to hook! Closing...\n");
		Sleep(3000);
		exit(0);
	}

	return 0;
}

BOOL __stdcall DllMain(HINSTANCE hModule, DWORD dwReason, LPVOID lpReserved)
{
	switch (dwReason)
	{
	case 1:
		HANDLE hMainThread = CreateThread(nullptr, 0, LPTHREAD_START_ROUTINE(MainThread), hModule, 0, nullptr);
		if (hMainThread)
			CloseHandle(hMainThread);
		break;
	}

	return TRUE;
}

