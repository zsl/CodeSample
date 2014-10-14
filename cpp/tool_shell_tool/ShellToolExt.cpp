// ShellToolExt.cpp : CShellToolExt µÄÊµÏÖ

#include "stdafx.h"
#include "ShellToolExt.h"
#include <atlconv.h>
#include <fstream>

#include <stdio.h>
#include <Shlwapi.h>
#include "resource.h"
#pragma comment(lib, "shlwapi.lib")
#include "Markup.h"
using namespace std;

#ifdef _DEBUG
#pragma comment(lib, "hl_lib_db.lib")
#else // _DEBUG
//#pragma comment(lib, "hl_lib.lib")
#endif

extern string g_dllPath;
extern HINSTANCE g_dllInstance;

#define OPEN_DEBUG_STRING			0
#if (OPEN_DEBUG_STRING)
#ifdef _UNICODE
#define DebugString(x) OutputDebugStringW(x)
#else
#define DebugString(x) OutputDebugStringA(x)
#endif
#else
#define DebugString(x)
#endif

CShellToolExt::CShellToolExt()
{
	m_hBitmap = LoadBitmap(g_dllInstance, MAKEINTRESOURCE(IDB_BITMAP));
}

HRESULT CShellToolExt::Initialize(LPCITEMIDLIST pidlFolder, LPDATAOBJECT pDataObj, HKEY hProgID)
{
	DebugString("Initialize");
	int type = 0;

	m_FileNames.clear();
	if (!pDataObj)
	{
		type = FTYPE_BACK;
		char buf[MAX_PATH];
		if (SHGetPathFromIDList(pidlFolder, buf))
		{
			m_FileNames.push_back(buf);
		}
		else
		{
			return E_INVALIDARG;
		}
	}
	else
	{
		FORMATETC fmt = {CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL};
		STGMEDIUM stg = {TYMED_HGLOBAL};
		HDROP hDrop;
		if (FAILED(pDataObj->GetData(&fmt, &stg)))
		{
			return E_INVALIDARG;
		}

		DebugString("hDrop = (HDROP)GlobalLock(stg.hGlobal);");
		hDrop = (HDROP)GlobalLock(stg.hGlobal);

		if (NULL == hDrop)
		{
			return E_INVALIDARG;
		}
		UINT uNumFiles = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, 0);
		if (0 == uNumFiles)
		{
			GlobalUnlock(stg.hGlobal);
			ReleaseStgMedium(&stg);
			return E_INVALIDARG;
		}

		TCHAR buf[2048];
		for (UINT i = 0; i < uNumFiles; i++)
		{
			if (0 == DragQueryFile(hDrop, i, buf, 2048))
			{
				GlobalUnlock(stg.hGlobal);
				ReleaseStgMedium(&stg);
				return E_INVALIDARG;
			}
			m_FileNames.push_back(buf);
			if (type != (FTYPE_DIR | FTYPE_FILE))
			{
				type |= PathIsDirectory(buf) ? FTYPE_DIR : FTYPE_FILE;
			}
		}
		GlobalUnlock(stg.hGlobal);
		ReleaseStgMedium(&stg);
	}
	if (!ReadPlugFile(type, m_FileNames))
	{
		m_FileNames.clear();
		return E_INVALIDARG;
	}
	return S_OK;
}
// CShellToolExt

HRESULT CShellToolExt::QueryContextMenu(HMENU hMenu, UINT uMenuIndex, 
										UINT uidFirstCmd, UINT uidLastCmd, UINT uFlags)
{
	DebugString("QueryContextMenu");
	if (uFlags & CMF_DEFAULTONLY)
	{
		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);
	}
	HMENU hPopMenu = ::CreatePopupMenu();
	if (hPopMenu == NULL)
	{
		return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, 0);
	}
	
	for (UINT i = 0; i < m_menus.size(); i++)
	{
		DebugString(string("InsertMenu:" + m_menus[i].get_text()).c_str());
		InsertMenu(hPopMenu, i, MF_BYPOSITION | MF_OWNERDRAW, uidFirstCmd + i, (LPCSTR)&m_menus[i]/*.get_text().c_str()*/);
	}
	InsertMenu(hMenu, uMenuIndex, MF_POPUP, (UINT_PTR)hPopMenu, _T("ShellTools"));
	SetMenuItemBitmaps(hMenu, (UINT_PTR)hPopMenu, MF_BYCOMMAND, m_hBitmap, m_hBitmap);
	return MAKE_HRESULT(SEVERITY_SUCCESS, FACILITY_NULL, m_menus.size());
}

HRESULT CShellToolExt::GetCommandString(UINT idCmd, UINT uFlags, UINT *pwReserved, LPSTR pszName, UINT cchMax)
{
	DebugString("GetCommandString");
	USES_CONVERSION;
	if (0 != idCmd)
	{
		return E_INVALIDARG;
	}
	if (uFlags & GCS_HELPTEXT)
	{
		LPCTSTR szText = _T("This is the shell tool.");
		if (uFlags & GCS_UNICODE)
		{
			lstrcpynW((LPWSTR)pszName, T2CW(szText), cchMax);
		}
		else
		{
			lstrcpynA(pszName, T2CA(szText), cchMax);
		}
		return S_OK;
	}
	return E_INVALIDARG;
}


HRESULT CShellToolExt::InvokeCommand(LPCMINVOKECOMMANDINFO pCmdInfo)
{
	DebugString("InvokeCommand");
	if (0 != HIWORD(pCmdInfo->lpVerb))
	{
		return E_INVALIDARG;
	}

	DWORD verb = LOWORD(pCmdInfo->lpVerb);

	if (verb >= 0 && verb < m_menus.size())
	{
		m_menus[verb].run(m_FileNames);
		return S_OK;
	}
	else
	{
		return E_INVALIDARG;
	}
}

HRESULT CShellToolExt::HandleMenuMsg(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	DebugString("HandleMenuMsg");
	LRESULT res = 0;
	return HandleMenuMsg2(uMsg, wParam, lParam, &res);
}

HRESULT CShellToolExt::HandleMenuMsg2(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	DebugString("HandleMenuMsg2");
	switch (uMsg)
	{
	case WM_INITMENUPOPUP:
		DebugString("WM_INITMENUPOPUP");
		break;
	case WM_DRAWITEM:
		{
			DebugString("WM_DRAWITEM");
			if (wParam == 0 && lParam != 0)
			{
				LPDRAWITEMSTRUCT lpDIS = (LPDRAWITEMSTRUCT)lParam;
				Menu* pMenu = (Menu*)lpDIS->itemData;
				const string& text = pMenu->get_text();
				HANDLE hIcon = pMenu->get_icon();
				RECT rect;
				rect.left = lpDIS->rcItem.left + 28;
				rect.top = lpDIS->rcItem.top;
				rect.bottom = lpDIS->rcItem.bottom;
				rect.right = lpDIS->rcItem.right;
				SetBkMode(lpDIS->hDC, TRANSPARENT);
				if (ODS_SELECTED & lpDIS->itemState)
				{
					HBRUSH hBrush = CreateSolidBrush(GetSysColor(COLOR_HIGHLIGHT));
					::FillRect(lpDIS->hDC, &lpDIS->rcItem, hBrush);
					DeleteObject(hBrush);
					SetTextColor(lpDIS->hDC, GetSysColor(COLOR_HIGHLIGHTTEXT));
					DrawText(lpDIS->hDC, text.c_str(), text.length(), &rect, DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
					SetTextColor(lpDIS->hDC, GetSysColor(COLOR_MENUTEXT));
				}
				else
				{
					HBRUSH hBrush = CreateSolidBrush(GetSysColor(COLOR_MENU));
					::FillRect(lpDIS->hDC, &lpDIS->rcItem, hBrush);
					DeleteObject(hBrush);
					DrawText(lpDIS->hDC, text.c_str(), text.length(), &rect, DT_VCENTER | DT_NOPREFIX | DT_SINGLELINE);
				}
				if (hIcon)
				{
					if (pMenu->is_bmp())
					{
						HDC hMemDC = CreateCompatibleDC(lpDIS->hDC);
						SelectObject(hMemDC, (HGDIOBJ)hIcon);
						BitBlt(lpDIS->hDC, lpDIS->rcItem.left + 6, lpDIS->rcItem.top + 2, 16, 16, hMemDC, 0, 0, SRCCOPY);
						DeleteDC(hMemDC);
					}
					else//ico
					{
						::DrawIconEx(lpDIS->hDC, lpDIS->rcItem.left + 6, lpDIS->rcItem.top + 2, (HICON)hIcon, 16, 16, 0, 0, DI_NORMAL);
					}
				}
				if (pResult)
				{
					*pResult = TRUE;
				}
			}
			else
			{
				if (pResult)
				{
					*pResult = FALSE;
				}
			}
		}
		break;
	case WM_MENUCHAR:
		DebugString("WM_MENUCHAR");
		if (pResult)
		{
			*pResult = MNC_IGNORE;
		}
		break;
	case WM_MEASUREITEM:
		{
			DebugString("WM_MEASUREITEM");
			if (wParam == 0 && lParam != 0)
			{
				LPMEASUREITEMSTRUCT lpMIS = (LPMEASUREITEMSTRUCT)lParam;
				const string& text = ((Menu*)(lpMIS->itemData))->get_text();
				HDC hDC = GetDC(NULL);
				DebugString("HDC hDC = GetDC(NULL);");
				int len = 28;
				if (hDC)
				{
					SIZE size;
					GetTextExtentPoint32(hDC, text.c_str(), text.length(), &size);
					DebugString("GetTextExtentPoint32(hDC, text.c_str(), text.length(), &size);");
					len += size.cx;
					ReleaseDC(NULL, hDC);
					DebugString("ReleaseDC(NULL, hDC);");
				}
				else
				{
					len += text.length()*16;
				}
				lpMIS->itemHeight = 20;
				lpMIS->itemWidth = len;

				if (pResult)
				{
					*pResult = TRUE;
				}
			}
			else
			{
				if (pResult)
				{
					*pResult = FALSE;
				}
			}
		}
		break;
	default:
		break;
	}
	DebugString("return S_OK;");
	return S_OK;
}

bool CShellToolExt::ReadPlugFile( int ftype, const vector<string>& files )
{
	DebugString("ReadPlugFile");
	string configfile = g_dllPath + "plugins.xml";
	CMarkup xml;
	string text, path, icon, param;
	stringex min, max, type, extends;
	m_menus.clear();
	if (xml.Load(configfile))
	{
		DebugString("xml.Load(configfile)");
		if (xml.FindElem("ShellTools"))
		{
			DebugString("xml.FindElem(\"ShellTools\")");
			while (xml.FindChildElem())
			{
				DebugString("xml.FindChildElem()");
				stringex name = xml.GetChildTagName();
				if (name.compare_nocase("menu") == 0)
				{
					text = xml.GetChildAttrib("text");
					path = xml.GetChildAttrib("path");
					min = xml.GetChildAttrib("min");
					max = xml.GetChildAttrib("max");
					type = xml.GetChildAttrib("type");
					extends = xml.GetChildAttrib("extends");
					icon = xml.GetChildAttrib("icon");
					param = xml.GetChildAttrib("param");
	
					//check count
					if (ftype != FTYPE_BACK)
					{
						UINT size = files.size();
						UINT iMin, iMax;
						if (min.empty())
						{
							iMin = 0;
						}
						else
						{
							iMin = min.integer();
						}
		
						if (max.empty())
						{
							iMax = 0xffffffff;
						}
						else
						{
							iMax = max.integer();
						}
						if ( !(size >= iMin && size <= iMax) )
						{
							continue;
						}
	
					}
					//check file/dir type
					vector<stringex> ts = type.split('|');
					int menuType = 0;
					for (vector<stringex>::iterator itr = ts.begin(); itr != ts.end(); ++itr)
					{
						if (*itr == "dir")
						{
							menuType |= FTYPE_DIR;
						}
						else if (*itr == "file")
						{
							menuType |= FTYPE_FILE;
						}
						else if (*itr == "back")
						{
							menuType |= FTYPE_BACK;
						}
					}
					if (menuType - ftype < 0 || (menuType & ftype) == 0)
					{
						continue;
					}
	
					//check extends
					bool bOK = false;
					if ((ftype & FTYPE_FILE) && (!extends.empty()))
					{
						vector<stringex> exts = extends.split('|');
						for (vector<string>::const_iterator itrf = files.begin(); itrf != files.end(); ++itrf)
						{
							stringex fileName = *itrf;
							if (!PathIsDirectory(fileName.c_str()))
							{
								bOK = false;
								fileName.tolower();
								for (vector<stringex>::iterator itr = exts.begin(); itr != exts.end(); ++itr)
								{
									if (fileName.end_with(*itr))
									{
										bOK = true;
										break;
									}
								}
								if (!bOK)
								{
									break;
								}
							}
							else
							{
								continue;
							}
						}
						if (!bOK)
						{
							continue;
						}
					}
	
					m_menus.push_back(Menu(text, path, icon, param, g_dllPath));
					DebugString("m_menus.push_back(Menu(text, path, icon, param, g_dllPath));");
				} 
			}
		}

	}
	return (m_menus.size() != 0);
}