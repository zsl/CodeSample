#include "charset.h"
#include <iostream>

int main()
{
    std::string ansi = "你好，中国";
    std::wstring uni = L"你好，中国";

    std::wstring strUniResult = Ansi2Unicode(ansi);
    std::string strAnsiResult = Unicode2Ansi(uni);

    std::cout << strAnsiResult << std::endl;

    std::wcout.imbue(std::locale(""));
    std::wcout << strUniResult << std::endl;

	// Unicode转换为utf8
    std::string strUtf8 = Unicode2Ansi(uni, CP_UTF8);
    
}