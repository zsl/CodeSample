// ShellToolExt.h : CShellToolExt ������

#pragma once
#include "resource.h"       // ������

#include "ShellTools_i.h"
#include <ShlObj.h>
#include <comdef.h>
#include <vector>
#include <string>
using namespace std;
#include "Menu.h"

#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE ƽ̨(�粻�ṩ��ȫ DCOM ֧�ֵ� Windows Mobile ƽ̨)���޷���ȷ֧�ֵ��߳� COM ���󡣶��� _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA ��ǿ�� ATL ֧�ִ������߳� COM ����ʵ�ֲ�����ʹ���䵥�߳� COM ����ʵ�֡�rgs �ļ��е��߳�ģ���ѱ�����Ϊ��Free����ԭ���Ǹ�ģ���Ƿ� DCOM Windows CE ƽ̨֧�ֵ�Ψһ�߳�ģ�͡�"
#endif


// CShellToolExt

class ATL_NO_VTABLE CShellToolExt :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CShellToolExt, &CLSID_ShellToolExt>,
	public IDispatchImpl<IShellToolExt, &IID_IShellToolExt, &LIBID_ShellToolsLib, /*wMajor =*/ 1, /*wMinor =*/ 1>,
 	public IShellExtInit,
	public IContextMenu3
{
public:
	CShellToolExt();

DECLARE_REGISTRY_RESOURCEID(IDR_SHELLTOOLEXT)


BEGIN_COM_MAP(CShellToolExt)
	COM_INTERFACE_ENTRY(IShellToolExt)
	COM_INTERFACE_ENTRY(IDispatch)
 	COM_INTERFACE_ENTRY(IShellExtInit)
	COM_INTERFACE_ENTRY(IContextMenu3)
	COM_INTERFACE_ENTRY(IContextMenu2)
	COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
	//IShellExtInit
	STDMETHOD(Initialize)(LPCITEMIDLIST, LPDATAOBJECT, HKEY);
	//IContextMenu3
	STDMETHOD(GetCommandString)(UINT, UINT, UINT*, LPSTR, UINT);
	STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO);
	STDMETHOD(QueryContextMenu)(HMENU, UINT, UINT, UINT, UINT);
	STDMETHOD(HandleMenuMsg)(UINT, WPARAM, LPARAM);
	STDMETHOD(HandleMenuMsg2)(UINT, WPARAM, LPARAM, LRESULT*);
protected:
	enum
	{
		FTYPE_FILE = 0x01,
		FTYPE_DIR  = 0x02,
		FTYPE_BACK = 0x04,
	};
	bool ReadPlugFile(int type, const vector<string>& files);
	vector<string> m_FileNames;
	vector<Menu> m_menus;
	HBITMAP m_hBitmap;
};

OBJECT_ENTRY_AUTO(__uuidof(ShellToolExt), CShellToolExt)
