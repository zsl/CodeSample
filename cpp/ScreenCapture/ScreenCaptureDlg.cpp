// ScreenCapture.cpp : 实现文件
//
#include "stdafx.h"
#include "ScreenCaptureDlg.h"
#include "atlimage.h"
#include <string>

//////////////////////////////////////////////////////////////////////////
COLORREF  TIP_INFO_BOX_BK_COLOR		= RGB(58, 58, 56);
COLORREF  TIP_INFO_BOX_EDGE_COLOR	= RGB(255, 142, 0);
COLORREF  TIP_INFO_BOX_TEXT_COLOR	= RGB(255, 255, 255);
COLORREF  SELECT_RECT_EDGE_COLOR	= RGB(87, 133, 52);

const int TIP_INFO_BOX_WIDTH		= 150;
const int TIP_INFO_BOX_HEIGHT		= 65;

const int TIP_INFO_TEXT_OFFSET		= 5;
const int TIP_INFO_TEXT_LINE_HEIGHT	= 15;



//////////////////////////////////////////////////////////////////////////
// CScreenCapture 对话框
IMPLEMENT_DYNAMIC(CScreenCapture, CDialog)

BEGIN_MESSAGE_MAP(CScreenCapture, CDialog)
	ON_WM_CREATE()
	ON_WM_PAINT()	
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////////
CScreenCapture::CScreenCapture(CWnd* pParent /*=NULL*/)
	: CDialog(CScreenCapture::IDD, pParent)
{
	m_bSelectOK		= false;
	m_bSelectRgn	= false;
	m_bInRgn		= false;


	m_Blend.AlphaFormat			= 0;
	m_Blend.BlendFlags			= 0;
	m_Blend.BlendOp				= AC_SRC_OVER;
	m_Blend.SourceConstantAlpha	= 210;

	m_BlendTip.AlphaFormat			= 0;
	m_BlendTip.BlendFlags			= 0;
	m_BlendTip.BlendOp				= AC_SRC_OVER;
	m_BlendTip.SourceConstantAlpha	= 180;


	m_TipFont.CreateFont(	12,                        // nHeight
							0,                         // nWidth
							0,                         // nEscapement
							0,                         // nOrientation
							FW_NORMAL,				   // nWeight
							FALSE,                     // bItalic
							FALSE,                     // bUnderline
							0,                         // cStrikeOut
							ANSI_CHARSET,              // nCharSet
							OUT_DEFAULT_PRECIS,        // nOutPrecision
							CLIP_DEFAULT_PRECIS,       // nClipPrecision
							DEFAULT_QUALITY,           // nQuality
							DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
							_T("宋体"));				   // lpszFacename

	m_Tool.Init();
}

CScreenCapture::~CScreenCapture()
{
}

void CScreenCapture::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


//////////////////////////////////////////////////////////////////////////
// CScreenCapture 消息处理程序
int CScreenCapture::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialog::OnCreate(lpCreateStruct) == -1)
	{
		return -1;
	}

	//ShowWindow(SW_HIDE);

    // 获取屏幕dc
	CDC l_DCScreen;
	l_DCScreen.CreateDC(_T("DISPLAY"), NULL, NULL, NULL);
	m_ScreenSize.cx = GetDeviceCaps(l_DCScreen.GetSafeHdc(), HORZRES);
	m_ScreenSize.cy = GetDeviceCaps(l_DCScreen.GetSafeHdc(), VERTRES);

    // 获取屏幕bmp
	m_DCMem.CreateCompatibleDC(&l_DCScreen);
	m_BmpScreen.CreateCompatibleBitmap(&l_DCScreen, m_ScreenSize.cx, m_ScreenSize.cy);

	CBitmap* l_pOldBmp = m_DCMem.SelectObject(&m_BmpScreen);

	m_DCMem.BitBlt(0, 0, m_ScreenSize.cx, m_ScreenSize.cy, &l_DCScreen, 0, 0, SRCCOPY);

    // 设置提示框的位置
	CRect rect(0,0,m_ScreenSize.cx, m_ScreenSize.cy);

	m_TipInfoPos.x = m_TipInfoPos.y = 0;
	m_TipInfoRect = CRect(0, 0, TIP_INFO_BOX_WIDTH, TIP_INFO_BOX_HEIGHT);;

    //
	MoveWindow(rect);
	//ShowWindow(SW_SHOW);

	m_hCursor = LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_SC_SELECT));
	return 0;
}

void CScreenCapture::OnPaint()
{
	CPaintDC l_DCTip(this); // device context for painting

	DisplayTipInfo();
	//l_DCTip.SetBkMode(RGB(0, 0, 0));
	////l_DCTip.StretchBlt(	0, 0, m_ScreenSize.cx, m_ScreenSize.cy,
	////					&m_DCMem,
	////					0, 0, m_ScreenSize.cx, m_ScreenSize.cy,
	////					SRCAND);

	//l_DCTip.AlphaBlend(	0, 0, m_ScreenSize.cx, m_ScreenSize.cy,
	//					&m_DCMem,
	//					0, 0, m_ScreenSize.cx, m_ScreenSize.cy,
	//					m_Blend);
}



void CScreenCapture::OnMouseMove(UINT nFlags, CPoint point)
{
	m_ptMouse = point;
	
	if (!m_bSelectOK)
	{
		m_SelectRect.right = point.x;
		m_SelectRect.bottom = point.y;
	}
	else
	{
		if (m_bInRgn)
		{
			if (FALSE == m_SelectRect.PtInRect(point))
			{
				m_hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
				m_bInRgn = false;
			}
		}
		else
		{
			if (TRUE == m_SelectRect.PtInRect(point))
			{
				m_hCursor = LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEALL));
				m_bInRgn = true;
			}
		}
	}

	m_Tool.OnMouseMove(point);
	
	DisplayTipInfo();
	CDialog::OnMouseMove(nFlags, point);
}


void CScreenCapture::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (!m_bSelectOK)
	{
		m_bSelectRgn = true;
		m_SelectRect.left = m_SelectRect.right = point.x;
		m_SelectRect.top = m_SelectRect.bottom = point.y;
	}
	else
	{
		switch (m_Tool.LButtonDownInBtn(point))
		{
		case 0:
			break;

		case 1:
			{
				SetTimer(TIMER_ID_SAVEBMP, 100, NULL);
			}
			break;

		case 2:
			{
				m_bSelectOK = false;
				OnOK();
				return ;
			}
			break;

		case 3:
			{
				OnOK();
				return ;
			}
			break;
		}
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CScreenCapture::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (!m_bSelectOK && m_bSelectRgn)
	{
		if (m_SelectRect.left > point.x)
		{
			m_SelectRect.right = m_SelectRect.left;
			m_SelectRect.left = point.x;
		}
		else
		{
			m_SelectRect.right = point.x;
		}

		if (m_SelectRect.top > point.y)
		{
			m_SelectRect.bottom = m_SelectRect.top;
			m_SelectRect.top = point.y;
		}
		else
		{
			m_SelectRect.bottom = point.y;
		}

		m_bSelectRgn = false;
		m_bSelectOK = true;

		m_bInRgn = true;
		m_hCursor = ::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEALL));

		if (m_SelectRect.left != m_SelectRect.right)
		{
			CPoint l_Pos;

			if (m_ScreenSize.cy - m_SelectRect.bottom > m_Tool.GetHeight())
			{
				l_Pos.y = m_SelectRect.bottom;
			}
			else
			{
				l_Pos.y = m_ScreenSize.cy - m_Tool.GetHeight();
			}

			if (m_SelectRect.right - m_Tool.GetWidth() > 0)
			{
				l_Pos.x = m_SelectRect.right - m_Tool.GetWidth();
			}
			else
			{
				l_Pos.x = 0;
			}

			m_Tool.SetPos(l_Pos.x, l_Pos.y);
			m_Tool.SetRender(true);
			DisplayTipInfo();
		}
	}

	CDialog::OnLButtonUp(nFlags, point);
}

void CScreenCapture::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	OnOK();
}

void CScreenCapture::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (m_bSelectOK)
	{
		if (!m_Tool.MousePtInRect(point))
		{
			m_TipInfoPos.x = m_TipInfoPos.y = 0;
			m_Tool.SetRender(false);
			m_bSelectOK = false;
			m_hCursor = ::LoadCursor(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDC_CURSOR_SC_SELECT));
			DisplayTipInfo();
		}
	}
	else
	{
		OnOK();
		return ;
	}

	CDialog::OnRButtonUp(nFlags, point);
}


void CScreenCapture::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == TIMER_ID_SAVEBMP)
	{
		KillTimer(nIDEvent);
		SaveBmpFile();
	}

	CDialog::OnTimer(nIDEvent);
}


BOOL CScreenCapture::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (nHitTest == HTCLIENT)
	{
		::SetCursor(m_hCursor);
		return TRUE;
	}
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CScreenCapture::DisplayTipInfo()
{
	if (m_bSelectRgn || m_bSelectOK)
	{		
		m_TipInfoPos.x = min(m_SelectRect.left, m_SelectRect.right);
		m_TipInfoPos.y = min(m_SelectRect.top, m_SelectRect.bottom);

		if (m_TipInfoPos.x + m_TipInfoRect.Width() >  m_ScreenSize.cx)
		{
			m_TipInfoPos.x -= m_TipInfoRect.Width();
		}

		if (m_TipInfoRect.Height() <  m_TipInfoPos.y)
		{
			m_TipInfoPos.y -= m_TipInfoRect.Height();
		}
	}
	

	CClientDC	l_DCTip(this);
	CDC			l_DCMem;
	CBitmap		l_TipBmp;
	CBitmap*	l_pOldBmp;
	COLORREF	l_TipColor = l_DCTip.GetPixel(m_ptMouse);
	
	l_DCMem.CreateCompatibleDC(&l_DCTip);	
	l_TipBmp.CreateCompatibleBitmap(&l_DCTip, m_ScreenSize.cx, m_ScreenSize.cy);
	l_pOldBmp = l_DCMem.SelectObject(&l_TipBmp);

	// 将屏幕作为背景
	l_DCMem.SetBkColor(RGB(0, 0, 0));
	//l_DCMem.BitBlt(	0, 0, m_ScreenSize.cx, m_ScreenSize.cy,
	//				&m_DCMem,
	//				0, 0,
	//				SRCCOPY);
    // 将屏幕放到l_DCMem中
	l_DCMem.AlphaBlend(	0, 0, m_ScreenSize.cx, m_ScreenSize.cy,
						&m_DCMem,
						0, 0, m_ScreenSize.cx, m_ScreenSize.cy,
						m_Blend);


	//////////////////////////////////////////////////////////////////////////
	// 绘制截图信息
	CDC			l_DCInfo;
	CBitmap		l_TipBmpInfo;
	CBitmap*	l_pOldBmpInfo;

	l_DCInfo.CreateCompatibleDC(&l_DCTip);	
	l_TipBmpInfo.CreateCompatibleBitmap(&l_DCTip, m_TipInfoRect.Width(), m_TipInfoRect.Height());
	l_pOldBmpInfo = l_DCInfo.SelectObject(&l_TipBmpInfo);

	//------------------------
	// draw 截图信息
	//------------------------
	CBrush	l_BrushBK(TIP_INFO_BOX_BK_COLOR);
	CPen	l_PenBK(PS_SOLID, 1, TIP_INFO_BOX_EDGE_COLOR);
	CPen*	l_pOldPen	= l_DCInfo.SelectObject(&l_PenBK);
	CBrush*	l_pOldBrush	= l_DCInfo.SelectObject(&l_BrushBK);

	l_DCInfo.SetBrushOrg(0, 0);
	l_DCInfo.Rectangle(m_TipInfoRect);
	l_DCInfo.SelectObject(l_pOldBrush);
	l_DCInfo.SelectObject(l_pOldPen);

	//-----------------------------------
	// draw 文字
	//-----------------------------------
	CString l_strRect, l_strRGB, l_strTip;

	if (m_bSelectRgn || m_bSelectOK)
	{
		l_strRect.Format(_T("区域大小：%d×%d"), abs(m_SelectRect.right - m_SelectRect.left), abs(m_SelectRect.bottom - m_SelectRect.top));
		l_strRGB.Format(_T("当前RGB：%d,%d,%d"), GetRValue(l_TipColor), GetGValue(l_TipColor), GetBValue(l_TipColor));
	}
	else
	{
		l_strRect.Format(_T("当前位置：%d,%d"), m_ptMouse.x, m_ptMouse.y);
		l_strRGB.Format(_T("当前RGB：%d,%d,%d"), GetRValue(l_TipColor), GetGValue(l_TipColor), GetBValue(l_TipColor));
	}

	l_strTip = _T("双击可以快速完成截图");

	CFont* l_pOldFont = l_DCInfo.SelectObject(&m_TipFont);

	l_DCInfo.SetBkMode(TRANSPARENT);
	l_DCInfo.SetTextColor(TIP_INFO_BOX_TEXT_COLOR);
	l_DCInfo.TextOut(TIP_INFO_TEXT_OFFSET, TIP_INFO_TEXT_OFFSET, l_strRect);
	l_DCInfo.TextOut(TIP_INFO_TEXT_OFFSET, TIP_INFO_TEXT_OFFSET + TIP_INFO_TEXT_LINE_HEIGHT, l_strRGB);
	l_DCInfo.TextOut(TIP_INFO_TEXT_OFFSET, TIP_INFO_TEXT_OFFSET * 3 + TIP_INFO_TEXT_LINE_HEIGHT * 2, l_strTip);
	l_DCInfo.SelectObject(l_pOldFont);
	
	l_DCInfo.SelectObject(&l_pOldBmpInfo);

	l_DCMem.AlphaBlend(	m_TipInfoPos.x, m_TipInfoPos.y, m_TipInfoRect.Width(), m_TipInfoRect.Height(),
						&l_DCInfo,
						0, 0, m_TipInfoRect.Width(), m_TipInfoRect.Height(),
						m_BlendTip);

	// end info plane
	//////////////////////////////////////////////////////////////////////////

	//-----------------------------------
	// 画选中的矩形
	//-------------------------------------
	if (m_bSelectRgn || m_bSelectOK)
	{
        // 矩形区域
		l_DCMem.BitBlt(	m_SelectRect.left, m_SelectRect.top, m_SelectRect.Width(), m_SelectRect.Height(),
						&m_DCMem,
						m_SelectRect.left, m_SelectRect.top,
						SRCCOPY);

		CPen l_PenLine(PS_SOLID, 1, SELECT_RECT_EDGE_COLOR);

		l_pOldPen = l_DCMem.SelectObject(&l_PenLine);

		l_DCMem.MoveTo(m_SelectRect.left, m_SelectRect.top);
		l_DCMem.LineTo(m_SelectRect.right, m_SelectRect.top);
		l_DCMem.LineTo(m_SelectRect.right, m_SelectRect.bottom);
		l_DCMem.LineTo(m_SelectRect.left, m_SelectRect.bottom);
		l_DCMem.LineTo(m_SelectRect.left, m_SelectRect.top);

		l_DCMem.SelectObject(l_pOldPen);

	}


	// display all
	l_DCTip.StretchBlt(	0, 0, m_ScreenSize.cx, m_ScreenSize.cy,
						&l_DCMem,
						0, 0, m_ScreenSize.cx, m_ScreenSize.cy,
						SRCCOPY);

	l_DCMem.SelectObject(l_pOldBmp);
	
	m_Tool.Draw(this);
}

void CScreenCapture::SaveSelectBmp()
{
	if (!m_bSelectOK)
	{
		return;
	}

	if (m_SelectRect.IsRectEmpty() || m_SelectRect.IsRectNull())
	{
		return;
	}

	CDC l_DCSave;	
	CBitmap l_BmpSave;
	// BITMAP l_BmpScreen;

	l_DCSave.CreateCompatibleDC(NULL);
	// m_BmpScreen.GetObject(sizeof(l_BmpScreen), &l_BmpScreen);
	// l_BmpScreen.bmWidth = m_SelectRect.Width();
	// l_BmpScreen.bmHeight = m_SelectRect.Height();
	// l_BmpSave.CreateBitmapIndirect(&l_BmpScreen);
    l_BmpSave.CreateCompatibleBitmap(&l_DCSave, m_SelectRect.Width(), m_SelectRect.Height());

	CBitmap* l_pOldBmp = l_DCSave.SelectObject(&l_BmpSave);

	l_DCSave.StretchBlt(0, 0, m_SelectRect.Width(), m_SelectRect.Height(),
						&m_DCMem,
						m_SelectRect.left, m_SelectRect.top, m_SelectRect.Width(), m_SelectRect.Height(),
						SRCCOPY);

	HBITMAP l_hBmp = (HBITMAP)l_BmpSave.Detach();

	l_DCSave.SelectObject(l_pOldBmp);

	// save to the clipboard
	if (::OpenClipboard(m_hWnd))
	{
		::EmptyClipboard();
		::SetClipboardData(CF_BITMAP, l_hBmp);
		::CloseClipboard();
	}
}

void CScreenCapture::SaveBmpFile()
{
	if (!m_bSelectOK)
	{
		return;
	}

	if (m_SelectRect.IsRectEmpty() || m_SelectRect.IsRectNull())
	{
		return;
	}

	std::wstring l_strSaveFile = _T("zsl截图.jgp");

	// 用户选择需要保存的文件名和路径
	std::wstring l_strFilter = 	_T("BMP (*.bmp)|*.bmp|PNG (*.png)|*.png|JEPG (*.jpg)|*.jpg|GIF (*.gif)|*.gif||");
	CFileDialog l_SaveDlg(FALSE, _T("jpg"), _T("zsl截图"), OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT, l_strFilter.c_str());
	
	if (l_SaveDlg.DoModal() == IDOK)
	{
		l_strSaveFile = l_SaveDlg.GetPathName();
	}
	else
	{
		return ;
	}


	// 处理需要保存的文件
	CDC l_DCSave;	
	CBitmap l_BmpSave;
	BITMAP l_BmpScreen;

	l_DCSave.CreateCompatibleDC(NULL);
	m_BmpScreen.GetObject(sizeof(l_BmpScreen), &l_BmpScreen);
	l_BmpScreen.bmWidth = m_SelectRect.Width();
	l_BmpScreen.bmHeight = m_SelectRect.Height();
	l_BmpSave.CreateBitmapIndirect(&l_BmpScreen);

	CBitmap* l_pOldBmp = l_DCSave.SelectObject(&l_BmpSave);

	l_DCSave.StretchBlt(0, 0, m_SelectRect.Width(), m_SelectRect.Height(),
						&m_DCMem,
						m_SelectRect.left, m_SelectRect.top, m_SelectRect.Width(), m_SelectRect.Height(),
						SRCCOPY);

	HBITMAP l_hBmp = (HBITMAP)l_BmpSave.Detach();

	l_DCSave.SelectObject(l_pOldBmp);

	CImage l_Save;

	l_Save.Attach(l_hBmp);
	l_Save.Save(l_strSaveFile.c_str());

	CDialog::OnOK();
}

void CScreenCapture::OnOK()
{
	SaveSelectBmp();

	CDialog::OnOK();
}



//////////////////////////////////////////////////////////////////////////
CSCaptureTool::CSCaptureTool()
{
	m_bRender = false;

	m_Pos.x = m_Pos.y = 0;

	m_rcBase.left = 0;
	m_rcBase.top = 0;
	m_rcBase.right = 183;
	m_rcBase.bottom = 25;


	m_rcSave.left = 11;
	m_rcSave.top = 3;
	m_rcSave.right = 61;
	m_rcSave.bottom = 22;

	m_rcCancel.left = 66;
	m_rcCancel.top = 3;
	m_rcCancel.right = 116;
	m_rcCancel.bottom = 22;

	m_rcOK.left = 121;
	m_rcOK.top = 3;
	m_rcOK.right = 171;
	m_rcOK.bottom = 22;

	m_BmpBase.LoadBitmap(IDB_BITMAP_SCTOOL_BASE);

	m_BmpBtnSave.LoadBitmap(IDB_BITMAP_SCTOOL_BTN_SAVE);
	m_BmpBtnCancel.LoadBitmap(IDB_BITMAP_SCTOOL_BTN_CANEL);
	m_BmpBtnOK.LoadBitmap(IDB_BITMAP_SCTOOL_BTN_OK);

	m_dwState = 0;
}

CSCaptureTool::~CSCaptureTool()
{

}

void CSCaptureTool::Init()
{

}

void CSCaptureTool::SetPos(int i_iX, int i_iY)
{
	m_Pos.x = i_iX;
	m_Pos.y = i_iY;
}

void CSCaptureTool::SetRender(bool i_bRender)
{
	m_bRender = i_bRender;
}

void CSCaptureTool::Draw(CWnd* i_pWnd)
{
	if (!m_bRender)
	{
		return ;
	}

	CClientDC	l_DCTip(i_pWnd);
	CDC			l_DCMem;
	CBitmap*	l_pOldBmp;

	CDC			l_DCBtn;
	CBitmap*	l_pOldBtnBmp;

	l_DCMem.CreateCompatibleDC(&l_DCTip);	
	l_pOldBmp = l_DCMem.SelectObject(&m_BmpBase);

	l_DCBtn.CreateCompatibleDC(&l_DCMem);	
	l_pOldBtnBmp = l_DCBtn.SelectObject(&m_BmpBtnSave);

	if (m_dwState == 1)
	{
		l_DCMem.BitBlt(	m_rcSave.left, m_rcSave.top, m_rcSave.right - m_rcSave.left, m_rcSave.bottom - m_rcSave.top,
						&l_DCBtn,
						m_rcSave.right - m_rcSave.left, 0,
						SRCCOPY);
	}
	else
	{
		l_DCMem.BitBlt(	m_rcSave.left, m_rcSave.top, m_rcSave.right - m_rcSave.left, m_rcSave.bottom - m_rcSave.top,
						&l_DCBtn,
						0, 0,
						SRCCOPY);
	}


	l_DCBtn.SelectObject(&m_BmpBtnCancel);

	if (m_dwState == 2)
	{
		l_DCMem.BitBlt(	m_rcCancel.left, m_rcCancel.top, m_rcCancel.right - m_rcCancel.left, m_rcCancel.bottom - m_rcCancel.top,
						&l_DCBtn,
						m_rcCancel.right - m_rcCancel.left, 0,
						SRCCOPY);
	}
	else
	{
		l_DCMem.BitBlt(	m_rcCancel.left, m_rcCancel.top, m_rcCancel.right - m_rcCancel.left, m_rcCancel.bottom - m_rcCancel.top,
						&l_DCBtn,
						0, 0,
						SRCCOPY);
	}


	l_DCBtn.SelectObject(&m_BmpBtnOK);

	if (m_dwState == 3)
	{
		l_DCMem.BitBlt(	m_rcOK.left, m_rcOK.top, m_rcOK.right - m_rcOK.left, m_rcOK.bottom - m_rcOK.top,
						&l_DCBtn,
						m_rcOK.right - m_rcOK.left, 0,
						SRCCOPY);
	}
	else
	{
		l_DCMem.BitBlt(	m_rcOK.left, m_rcOK.top, m_rcOK.right - m_rcOK.left, m_rcOK.bottom - m_rcOK.top,
						&l_DCBtn,
						0, 0,
						SRCCOPY);
	}


	l_DCTip.BitBlt(	m_Pos.x, m_Pos.y, m_rcBase.right - m_rcBase.left, m_rcBase.bottom - m_rcBase.top,
					&l_DCMem,
					0, 0,
					SRCCOPY);

	l_DCBtn.SelectObject(l_pOldBtnBmp);
	l_DCMem.SelectObject(l_pOldBmp);
}

int CSCaptureTool::GetHeight()
{
	return (m_rcBase.bottom - m_rcBase.top);
}

int CSCaptureTool::GetWidth()
{
	return (m_rcBase.right - m_rcBase.left);
}

void CSCaptureTool::OnMouseMove(CPoint i_Pos)
{
	if (!m_bRender)
	{
		return ;
	}

	m_dwState = 0;

	i_Pos.x -= m_Pos.x;
	i_Pos.y -= m_Pos.y;

	if (PtInRect(m_rcBase, i_Pos))
	{
		if (PtInRect(m_rcSave, i_Pos))
		{
			m_dwState = 1;
		}
		else if (PtInRect(m_rcCancel, i_Pos))
		{
			 m_dwState = 2;
		}
		else if (PtInRect(m_rcOK, i_Pos))
		{
			m_dwState = 3;
		}
	}
}

DWORD CSCaptureTool::LButtonDownInBtn(CPoint i_Pos)
{
	if (!m_bRender)
	{
		return 0;
	}

	i_Pos.x -= m_Pos.x;
	i_Pos.y -= m_Pos.y;

	if (PtInRect(m_rcBase, i_Pos))
	{
		if (PtInRect(m_rcSave, i_Pos))
		{
			return 1;
		}
		else if (PtInRect(m_rcCancel, i_Pos))
		{
			return 2;
		}
		else if (PtInRect(m_rcOK, i_Pos))
		{
			return 3;
		}
	}

	return 0;
}

bool CSCaptureTool::MousePtInRect(CPoint i_Pos)
{
	if (!m_bRender)
	{
		return false;
	}

	i_Pos.x -= m_Pos.x;
	i_Pos.y -= m_Pos.y;

	if (PtInRect(m_rcBase, i_Pos))
	{
		return true;
	}

	return false;
}
