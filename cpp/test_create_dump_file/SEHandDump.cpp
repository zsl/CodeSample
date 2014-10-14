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
// ����Dump�ļ�
// 
void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)
{
	// ����Dump�ļ�
	//
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// Dump��Ϣ
	//
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;

	// д��Dump�ļ�����
	//
	BOOL success = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);
	cout << success <<endl;
	CloseHandle(hDumpFile);
}

// �õ���ǰʱ��
//
wstring GetPresentTime()
{
	SYSTEMTIME time;
	GetLocalTime(&time); 

	wchar_t wszTime[128];
	swprintf_s(wszTime, _T("%04d-%02d-%02d %02d-%02d-%02d-%03d"), time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	return wstring(wszTime);
}

// ��Ϊexcept���б��ʽ�ĺ���
//
LONG CrashHandler(EXCEPTION_POINTERS *pException)
{	
	// ��������Ӵ�������������Ĵ���
	//

	// �����Ե���һ���Ի���Ϊ����
	//
	MessageBox(NULL, _T("Message from Catch handler"), L"Test", MB_OK);

	wstring filename = GetPresentTime() + _T(".dmp");
	// ����Dump�ļ�
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

		// ���㣬��Ϊ��ʹ�������
		//
		int i = 13;
		int j = 0;
		//int m = i / j;
		int *p = 0;
		*p = 0;
	}
	// ��׽���ó���������쳣ʱ����Dump�ļ�
	//
	__except(CrashHandler(GetExceptionInformation()))
	{
		// �����Ե���һ���Ի���Ϊ����
		//
		MessageBox(NULL, L"Message from '__except' section", L"Test", MB_OK);
	}

	MessageBox(NULL, L"Funcation completed", L"Test", MB_OK);

	return 0;
}