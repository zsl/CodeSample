#include "charset.h"
#include <iostream>

int main()
{
    std::string ansi = "��ã��й�";
    std::wstring uni = L"��ã��й�";

    std::wstring strUniResult = Ansi2Unicode(ansi);
    std::string strAnsiResult = Unicode2Ansi(uni);

    std::cout << strAnsiResult << std::endl;

    std::wcout.imbue(std::locale(""));
    std::wcout << strUniResult << std::endl;

	// Unicodeת��Ϊutf8
    std::string strUtf8 = Unicode2Ansi(uni, CP_UTF8);
    
}