#include <iostream>
#include <fstream>

std::locale get_chinese_locale()
{
    // ϵͳĬ�ϵ�locale��һ�������ĵ�
    std::locale chslocale;
    try
    {
        chslocale = std::locale("chs");
    }
    catch (std::exception &e)
    {
        chslocale = std::locale("");
    }

    return chslocale;
}

int main()
{
    // ��Ҫ����������ΪϵͳĬ�ϵģ��������·���а������ģ����ļ��ͻ�ʧ��
    std::locale oldLocale = std::locale::global(get_chinese_locale());
    std::ifstream ifs("E:\\��ð�.txt");
    std::locale::global(oldLocale);

    if (!ifs)
        std::cout << "���ļ�ʧ��\n";
    else 
        std::cout << "���ļ��ɹ�\n";

    // ��Localeֱ������Ϊchs
    oldLocale = std::locale::global(get_chinese_locale());
    std::ofstream ofs("����.txt", std::ios::binary | std::ios::out);
    oldLocale = std::locale::global(oldLocale);
    std::cout << oldLocale.name().c_str() << std::endl;
    ofs.close();
}