#include "GetErrorMessage.h"

typedef std::basic_string<TCHAR> tstring;

int main()
{
    // char
    for (int i = 1; i < 10; ++i){
        std::string msg = GetErrorMsg<char>(i);
        std::cout << i << ":" << msg << std::endl;
    }

    // wchar
    std::wcout.imbue(std::locale(""));
    for (int i = 1; i < 10; ++i){
        std::wstring msg = GetErrorMsg<wchar_t>(i);
        std::wcout << i << ":" << msg << std::endl;
    }
}