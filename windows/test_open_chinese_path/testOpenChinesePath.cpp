#include <iostream>
#include <fstream>

std::locale get_chinese_locale()
{
    // 系统默认的locale不一定是中文的
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
    // 需要将区域设置为系统默认的，否则，如果路径中包含中文，打开文件就会失败
    std::locale oldLocale = std::locale::global(get_chinese_locale());
    std::ifstream ifs("E:\\你好啊.txt");
    std::locale::global(oldLocale);

    if (!ifs)
        std::cout << "打开文件失败\n";
    else 
        std::cout << "打开文件成功\n";

    // 将Locale直接设置为chs
    oldLocale = std::locale::global(get_chinese_locale());
    std::ofstream ofs("中文.txt", std::ios::binary | std::ios::out);
    oldLocale = std::locale::global(oldLocale);
    std::cout << oldLocale.name().c_str() << std::endl;
    ofs.close();
}