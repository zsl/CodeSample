#include "charset.h"
std::string Unicode2Ansi( const wchar_t *str, UINT codePage /*= CP_ACP*/ )
{
    if (0 == wcslen(str)){
        return std::string();
    }

    int count = WideCharToMultiByte(codePage, 0, str, -1, NULL, 0, NULL, NULL);

    char *buf = new char[count];
    memset(buf, 0, count);
    WideCharToMultiByte(codePage, 0, str, -1, buf, count, NULL, NULL);

    std::string result = buf;

    delete[] buf;

    return result;
}

std::string Unicode2Ansi( const std::wstring &str, UINT codePage /*= CP_ACP*/ )
{
    return Unicode2Ansi(str.c_str(), codePage);
}

std::wstring Ansi2Unicode( const char *str, UINT codePage /*= CP_ACP*/ )
{
    if (0 == strlen(str)){
        return std::wstring();
    }

    int count = MultiByteToWideChar(codePage, 0, str, -1, NULL, 0);
    
    wchar_t *buf = new wchar_t[count];
    memset(buf, 0, count * sizeof(wchar_t) );

    MultiByteToWideChar(codePage, 0, str, -1, buf, count);

    std::wstring result = buf;

    delete[] buf;

    return result;
}

std::wstring Ansi2Unicode( const std::string &str, UINT codePage /*= CP_ACP*/ )
{
    return Ansi2Unicode(str.c_str(), codePage);
}
