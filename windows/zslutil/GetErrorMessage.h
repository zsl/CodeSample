/**
 * @file GetErrorMessage.h
 * @brief 获取Windows错误码对应的信息
 *
 * @author zsl, sanlo.zhang@gmail.com
 */

#ifndef ZSLUTIL_GETERRORMESSAGE_H__
#define ZSLUTIL_GETERRORMESSAGE_H__

#include <Windows.h>
#include <string>
#include <iostream>

#include "charset.h"

template<typename Tchar>
std::basic_string<Tchar> GetErrorMsg(int errcode);

template<>
std::wstring GetErrorMsg<wchar_t>(int errcode)
{
    DWORD systemLocal = MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL);
    
    HLOCAL hLocal = NULL;

    BOOL ret = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                            NULL, errcode, systemLocal, (wchar_t*)&hLocal, 0, NULL);

    if (!ret){
        // 是否是一个网络库的错误
        HMODULE hDll = LoadLibraryExW(L"netmsg.dll", NULL, DONT_RESOLVE_DLL_REFERENCES);
        if (hDll){
            ret = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER,
                                  NULL, errcode, systemLocal, (wchar_t*)&hLocal, 0, NULL);

        }
    }

    std::wstring msg;
    if (ret && hLocal){
        msg = (wchar_t*)LocalLock(hLocal);
        LocalFree(hLocal);
    }

    return msg;
}

template<>
std::string GetErrorMsg<char>(int errcode)
{
    return Unicode2Ansi(GetErrorMsg<wchar_t>(errcode) );
}


#endif
