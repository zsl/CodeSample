// dllmain.cpp : DllMain 的实现。

#include "stdafx.h"
#include "resource.h"
#include "ShellTools_i.h"
#include "dllmain.h"
#include <string>
using namespace std;

CShellToolsModule _AtlModule;
string g_dllPath;
HINSTANCE g_dllInstance;

// DLL 入口点
extern "C" BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	g_dllInstance = hInstance;
	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TCHAR buf[MAX_PATH];
		GetModuleFileName((HMODULE)hInstance, buf, MAX_PATH);
		for (int i=strlen(buf); i>=0; i--)
		{
			if (buf[i] == '\\')
			{
				buf[i+1] = 0;
				break;
			}
		}
		g_dllPath = buf;
		::setlocale(LC_ALL, "chs");
	}
	return _AtlModule.DllMain(dwReason, lpReserved); 
}
