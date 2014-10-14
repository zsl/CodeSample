/**
* @file charset.h
* @brief Unicode��Ansi֮���ת������
* @author zsl, sanlo.zhang@gmail.com
*/
#ifndef ZSL_CHARSET_H__
#define ZSL_CHARSET_H__

#include <Windows.h>
#include <string>

// �����Ҫ֧�ֺ��ģ�codePageӦ��ΪGB18030
#define CP_GB18030 54936

/**
* @brief  ��Unicodeת����Ansi
* 
* @param str ��Ҫת����Unicode�ַ���
* @param codePage Ŀ��ANSi�ַ����ı���
* @return Ŀ��Ansi�ַ���
*/
std::string Unicode2Ansi(const wchar_t *str, UINT codePage = CP_ACP);
std::string Unicode2Ansi(const std::wstring &str, UINT codePage = CP_ACP);

/**
* @brief  ��Ansiת����Unicode
* 
* @param str ��Ҫת����Ansie�ַ���
* @param codePage ANSi�ַ����ı���
* @return ת���ɵ�Unicode�ַ���
*/
std::wstring Ansi2Unicode(const char *str, UINT codePage = CP_ACP);
std::wstring Ansi2Unicode(const std::string &str, UINT codePage = CP_ACP);


#endif