#include <Windows.h>
#include "detours.h"
#include <iostream>

#pragma comment(lib, "Detour.lib")

HANDLE (WINAPI * TrueCreateFile)(LPCTSTR lpFileName, 
	DWORD dwDesiredAccess, 
	DWORD dwShareMode, LPSECURITY_ATTRIBUTES, DWORD, DWORD, HANDLE) = CreateFile;

HANDLE WINAPI My_CreateFile(LPCTSTR lpFileName, 
	DWORD dwDesiredAccess, 
	DWORD dwShareMode, LPSECURITY_ATTRIBUTES pa, DWORD d1, DWORD d2, HANDLE hTem){

		char *addrOfCreateFile = (char *)CreateFile;
		int jmpOff = *( (int *)( addrOfCreateFile + 1) );

		if ( jmpOff == (char *)My_CreateFile - addrOfCreateFile - 5){

		std::cout << "CreateFileHookMain.dll : " << "CreateFile(" << lpFileName << ")" << std::endl; 
		}
		else
			std::cout << "Other Detoured !!!!!!\n";

		return TrueCreateFile(lpFileName, dwDesiredAccess, dwShareMode, pa ,d1, d2, hTem);
} 

BOOL WINAPI DllMain(HANDLE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved){
	switch (ul_reason_for_call){
	case DLL_PROCESS_ATTACH:
		DetourRestoreAfterWith();

		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread() );
		DetourAttach(&(PVOID&)TrueCreateFile, My_CreateFile);
		DetourTransactionCommit();

		std::cout << "DLL_ATTATCH\n";

		break;
	case DLL_PROCESS_DETACH:
		DetourTransactionBegin();
		DetourUpdateThread(GetCurrentThread() );
		DetourDetach(&(PVOID&)TrueCreateFile, My_CreateFile);
		DetourTransactionCommit();
		break;
	default:
		break;
	}

	return TRUE;
}