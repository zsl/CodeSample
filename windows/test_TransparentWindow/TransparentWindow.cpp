// TransparentWindow.cpp : ����Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "TransparentWindow.h"
#include <GdiPlus.h>

#pragma  comment(lib, "gdiplus.lib")

class GdiPlusIniter{
public:
    GdiPlusIniter(){
        Gdiplus::GdiplusStartupInput StartupInput;  
        GdiplusStartup(&m_gdiplusToken,&StartupInput,NULL); 
    }

    ~GdiPlusIniter(){
        Gdiplus::GdiplusShutdown(m_gdiplusToken);
    }
private:
    ULONG_PTR m_gdiplusToken;
} g_GdiplusInitier;

#define MAX_LOADSTRING 100

// ȫ�ֱ���:
HINSTANCE hInst;								// ��ǰʵ��
TCHAR szTitle[MAX_LOADSTRING];					// �������ı�
TCHAR szWindowClass[MAX_LOADSTRING];			// ����������

// �˴���ģ���а����ĺ�����ǰ������:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

 	// TODO: �ڴ˷��ô��롣
	MSG msg;
	HACCEL hAccelTable;

	// ��ʼ��ȫ���ַ���
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_TRANSPARENTWINDOW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// ִ��Ӧ�ó����ʼ��:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_TRANSPARENTWINDOW));

	// ����Ϣѭ��:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  ����: MyRegisterClass()
//
//  Ŀ��: ע�ᴰ���ࡣ
//
//  ע��:
//
//    ����ϣ��
//    �˴�������ӵ� Windows 95 �еġ�RegisterClassEx��
//    ����֮ǰ�� Win32 ϵͳ����ʱ������Ҫ�˺��������÷������ô˺���ʮ����Ҫ��
//    ����Ӧ�ó���Ϳ��Ի�ù�����
//    ����ʽ��ȷ�ġ�Сͼ�ꡣ
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_TRANSPARENTWINDOW));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_TRANSPARENTWINDOW);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   ����: InitInstance(HINSTANCE, int)
//
//   Ŀ��: ����ʵ�����������������
//
//   ע��:
//
//        �ڴ˺����У�������ȫ�ֱ����б���ʵ�������
//        ��������ʾ�����򴰿ڡ�
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // ��ʵ������洢��ȫ�ֱ�����

   hWnd = CreateWindowEx(WS_EX_LAYERED, szWindowClass, szTitle, WS_POPUP,
      100, 100, 400, 400, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   // ����ʹ��SetLayeredWindowAttributes�������������ڵ�͸����
   //SetLayeredWindowAttributes(hWnd, RGB(0xff,0xff,0xff), 0xff, LWA_COLORKEY);
   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ����: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  Ŀ��: ���������ڵ���Ϣ��
//
//  WM_COMMAND	- ����Ӧ�ó���˵�
//  WM_PAINT	- ����������
//  WM_DESTROY	- �����˳���Ϣ������
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		// �����˵�ѡ��:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
    case WM_LBUTTONDOWN:
        {
            ::SendMessage(hWnd, WM_NCLBUTTONDOWN, HTCAPTION, lParam);
        }
        break;
    case WM_CREATE:
        {
            
            HDC hdc = ::GetDC(hWnd);
            RECT rcWindow;
            ::GetWindowRect(hWnd, &rcWindow);

            HDC memdc = ::CreateCompatibleDC(hdc);
            HBITMAP membitmap = ::CreateCompatibleBitmap(hdc, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);
            HGDIOBJ oldbmp = ::SelectObject(memdc, membitmap);
            {
                // ���ɰ�͸����ɫ
                Gdiplus::Graphics g(memdc);
                Gdiplus::SolidBrush brush(Gdiplus::Color(0x55, 0x0, 0x0, 0x0));
                g.FillRectangle(&brush, rcWindow.left, rcWindow.top, rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top);

            }

            BLENDFUNCTION bf = {0};
            bf.BlendOp = AC_SRC_OVER;
            bf.BlendFlags = 0;
            bf.SourceConstantAlpha = 0xff;
            bf.AlphaFormat = AC_SRC_ALPHA;

            POINT ptdst = {rcWindow.left, rcWindow.top};

            SIZE szdst = {rcWindow.right - rcWindow.left, rcWindow.bottom - rcWindow.top};
            POINT ptsrc = {0, 0};
            // ʹ��UpdateLayeredWindow�����ô���͸���ı���
            // Ŀ�괰�ڵ�size����ָ����������ʾ������
            BOOL ret = ::UpdateLayeredWindow(hWnd, NULL, NULL, &szdst, memdc, &ptsrc, 0, &bf, ULW_ALPHA);

            ::SelectObject(memdc, oldbmp);
            ::DeleteObject(membitmap);
            ::DeleteDC(memdc);
            ::ReleaseDC(hWnd, hdc);


        }
        break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// �����ڡ������Ϣ�������
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
