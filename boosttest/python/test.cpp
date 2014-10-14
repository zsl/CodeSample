#include <boost/python.hpp>
#include <Windows.h>
using namespace boost::python;

#include <windows.h>

int get_console_color(void)
{
	CONSOLE_SCREEN_BUFFER_INFO screenBufInfo;
	GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &screenBufInfo);
	return (int)screenBufInfo.wAttributes;
}

void set_console_color(int color)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)color);
}

enum Color
{
	RED   = FOREGROUND_RED | FOREGROUND_INTENSITY,
	BLUE  = FOREGROUND_BLUE | FOREGROUND_INTENSITY,
	GREEN = FOREGROUND_GREEN | FOREGROUND_INTENSITY,
};

BOOST_PYTHON_MODULE(consolecolor)
{
	def("get_color", get_console_color, "ȡ�ÿ���̨��ǰ��ɫ");
	def("set_color", set_console_color, "���ÿ���̨��ǰ��ɫ");
	enum_<Color>("Color").value("RED", RED).value("BLUE", BLUE).value("GREEN", GREEN).export_values();
}
