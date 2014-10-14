#include "stdafx.h"
#include <windows.h>
#include <shlwapi.h>
#include <stdio.h>
#include <string>
using namespace std;
#pragma comment(lib, "shlwapi.lib")

void copypath(const string& path)
{
	HWND hWnd = ::GetDesktopWindow();
	if (::OpenClipboard(hWnd))
	{
		::EmptyClipboard();
		int len = path.length() + 1;
		HGLOBAL hGlobal = ::GlobalAlloc(GMEM_FIXED, len);
		char* temp = (char*)::GlobalLock(hGlobal);
		strcpy(temp, path.c_str());
		::GlobalUnlock(hGlobal);
		::SetClipboardData(CF_TEXT, hGlobal);
		::CloseClipboard();
	}
}

void run_cmmand(const string& path)
{
	STARTUPINFO si = {0};
	si.cb = sizeof(si);
	PROCESS_INFORMATION pi = {0};
	string params = "/K \"CD ";
	params += path + "\"";
	ShellExecute(::GetDesktopWindow(), "open", "cmd.exe", params.c_str(), NULL, SW_SHOW);
}

void OpenConsole(const string& fileName)
{
	if (!PathIsDirectory(fileName.c_str()))
	{
		int index = fileName.find_last_of('\\');
		copypath(fileName.substr(index + 1));
		run_cmmand(fileName.substr(0, index));
	}
	else
	{
		run_cmmand(fileName);
	}
}

void CopyPath(const string& fileName)
{
	copypath(fileName);
}

void CopyPathChange(const string& fileName)
{
	string path;
	for (unsigned int i = 0; i < fileName.length(); i++)
	{
		if (fileName[i] == '\\')
		{
			path.append(1, '/');
		}
		else
		{
			path.append(1, fileName[i]);
		}
	}
	copypath(path);
}

void CopyPathDouble(const string& fileName)
{
	string path;
	for (unsigned int i = 0; i < fileName.length(); i++)
	{
		if (fileName[i] == '\\')
		{
			path.append("\\\\");
		}
		else
		{
			path.append(1, fileName[i]);
		}
	}
	copypath(path);
}
void DllRegister(const string& fileName)
{
	string params = "\"" + fileName + "\"";
	ShellExecute(::GetDesktopWindow(), "open", "regsvr32.exe", params.c_str(), NULL, SW_SHOW);
}

void DllUnRegister(const string& fileName)
{
	string params = "/u \"" + fileName + "\"";
	ShellExecute(::GetDesktopWindow(), "open", "regsvr32.exe", params.c_str(), NULL, SW_SHOW);
}