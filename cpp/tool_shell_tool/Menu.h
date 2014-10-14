#pragma once
#include <vector>
#include <sstream>
#include "stringex.h"
using namespace std;
#include <windows.h>

class Menu
{
public:

	Menu(const string& text,
		 const string& path,
		 const string& icon,
		 const string& param, 
		 const string& root)
		 :m_text(text),
		 m_path(path),
		 m_param(param),
		 m_root(root),
		 m_icon(icon),
		 m_hIcon(NULL),
		 m_bmp(false)
	{
	}

	~Menu(void)
	{
		del_icon();
	}

	static bool pathAbsolute(const string& path)
	{
		if (path.length() > 3 && (path[1] == ':' || path[2] == ':'))
		{
			return true;
		}
		return false;
	}

	void run(const vector<string>& files)
	{
		stringstream ss;
		if (!pathAbsolute(m_path))
		{
			ss<<m_root;
		}
		ss<<stringex(m_path).trim('\\');
		string file = ss.str();
		ss.str("");
		if (!m_param.empty())
		{
			ss<<"\""<<m_param<<"\"";
		}
		for (vector<string>::const_iterator itr = files.begin(); itr != files.end(); ++itr)
		{
			ss<<" \""<<*itr<<"\"";
		}
		ShellExecuteA(NULL, "open", file.c_str(), ss.str().c_str(), NULL, SW_SHOW);
	}

	const string& get_text()
	{
		return m_text;
	}

	HANDLE get_icon()
	{
		if (!m_hIcon)
			load_icon();
		return m_hIcon;
	}

	bool is_bmp()
	{
		return m_bmp;
	}

private:
	
	void load_icon()
	{
		m_bmp = false;
		if (!m_hIcon && !m_icon.empty())
		{
			stringex iconpath;
			if (!pathAbsolute(m_icon))
			{
				iconpath = m_root + m_icon;
			}
			else
			{
				iconpath = m_icon;
			}
			iconpath.tolower();
			int x = 16;
			int y = 16;
			m_hIcon = LoadImageA(0, iconpath.c_str(), IMAGE_BITMAP, x, y, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			if (m_hIcon)
			{
				m_bmp = true;
				return;
			}
			m_hIcon = LoadImageA(0, iconpath.c_str(), IMAGE_ICON, x, y, LR_CREATEDIBSECTION | LR_LOADFROMFILE);
			if (!m_hIcon)
			{
				ExtractIconExA(iconpath.c_str(), 0, 0, (HICON*)&m_hIcon, 1);
			}
		}
	}

	void del_icon()
	{
		if (m_hIcon)
		{
			DeleteObject(m_hIcon);
			m_hIcon = NULL;
		}
	}

	string m_text;
	string m_path;
	HANDLE m_hIcon;
	string m_param;
	string m_root;
	string m_icon;
	bool m_bmp;
};
