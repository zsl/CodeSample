#include <windows.h>
#include <process.h>

#include <ctime>
#include <cstdlib>
#include <vector>
#include <iostream>

using namespace std;

unsigned int __stdcall threadProc(void *arg)
{
    DWORD tlsIndex = reinterpret_cast<DWORD>(arg);
    clock_t begin = clock();
    TlsSetValue(tlsIndex, PVOID(begin) );  // ����TlsSetValue ���� ֵ

    printf("\nbegin thread: %d, clock_t: %d", GetCurrentThreadId() , begin);

    Sleep(3000);

    clock_t end = clock();
    clock_t diff = end -  reinterpret_cast<clock_t>(TlsGetValue(tlsIndex) );
    double sec = 1.0 * diff / CLOCKS_PER_SEC; // ����TlsGetValueȡ��ֵ

    printf("\nend thread: %d, clock: %d, live time %f", GetCurrentThreadId(), end, sec );

    return 0;
}

int main(int argc, char *argv[])
{
    // ����Tls����
    DWORD tlsIndex = TlsAlloc();

    vector<HANDLE> threads;
    for (int i = 0; i < 50; ++i){
        HANDLE h = (HANDLE)_beginthreadex(NULL, 0, threadProc, (void*)tlsIndex, 0, NULL);
        threads.push_back(h);
    }

    for (size_t i = 0; i < threads.size(); ++i){
        WaitForSingleObject(threads[i], INFINITE);
        CloseHandle(threads[i]);
    }
}
