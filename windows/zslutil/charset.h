/**
* @file charset.h
* @brief Unicode与Ansi之间的转换函数
* @author zsl, sanlo.zhang@gmail.com
*/
#ifndef ZSL_CHARSET_H__
#define ZSL_CHARSET_H__

#include <Windows.h>
#include <string>

// 如果需要支持韩文，codePage应该为GB18030
#define CP_GB18030 54936

/**
* @brief  从Unicode转换到Ansi
* 
* @param str 需要转换的Unicode字符串
* @param codePage 目标ANSi字符串的编码
* @return 目标Ansi字符串
*/
std::string Unicode2Ansi(const wchar_t *str, UINT codePage = CP_ACP);
std::string Unicode2Ansi(const std::wstring &str, UINT codePage = CP_ACP);

/**
* @brief  从Ansi转换到Unicode
* 
* @param str 需要转换的Ansie字符串
* @param codePage ANSi字符串的编码
* @return 转换成的Unicode字符串
*/
std::wstring Ansi2Unicode(const char *str, UINT codePage = CP_ACP);
std::wstring Ansi2Unicode(const std::string &str, UINT codePage = CP_ACP);


#endif