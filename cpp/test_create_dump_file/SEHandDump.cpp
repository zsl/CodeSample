#include <Windows.h>
#include <Dbghelp.h>
#include <string>
#include <iostream>

#ifdef _UNICODE || UNICODE
    #define _T(str) L##str
#else
    #define _T(str) str
#endif

#pragma comment(lib, "dbghelp.lib")

using namespace std;
// 创建Dump文件
// 
void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)
{
	// 创建Dump文件
	//
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// Dump信息
	//
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;

	// 写入Dump文件内容
	//
	BOOL success = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
	cout << success <<endl;
	CloseHandle(hDumpFile);
}

// 得到当前时间
//
wstring GetPresentTime()
{
	SYSTEMTIME time;
	GetLocalTime(&time); 

	wchar_t wszTime[128];
	swprintf_s(wszTime, _T("%04d-%02d-%02d %02d-%02d-%02d-%03d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	return wstring(wszTime);
}

// 作为except块中表达式的函数
//
LONG CrashHandler(EXCEPTION_POINTERS *pException)
{	
	// 在这里添加处理程序崩溃情况的代码
	//

	// 这里以弹出一个对话框为例子
	//
	MessageBox(NULL, _T("Message from Catch handler"), L"Test", MB_OK);

	wstring filename = GetPresentTime() + _T(".dmp");
	// 创建Dump文件
	//
	CreateDumpFile(filename.c_str(), pException);

	return EXCEPTION_EXECUTE_HANDLER;
}

int main()
{
	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)CrashHandler);
	__try
	{
		MessageBox(NULL, L"Message from '__try' section", L"Test", MB_OK);

		// 除零，人为的使程序崩溃
		//
		int i = 13;
		int j = 0;
		//int m = i / j;
		int *p = 0;
		*p = 0;
	}
	// 捕捉到让程序崩溃的异常时创建Dump文件
	//
	__except(CrashHandler(GetExceptionInformation()))
	{
		// 这里以弹出一个对话框为例子
		//
		MessageBox(NULL, L"Message from '__except' section", L"Test", MB_OK);
	}

	MessageBox(NULL, L"Funcation completed", L"Test", MB_OK);

	return 0;
}