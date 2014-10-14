#pragma once

#include "resource.h"

//////////////////////////////////////////////////////////////////////////
#define	TIMER_ID_SAVEBMP	10000


//////////////////////////////////////////////////////////////////////////
class CSCaptureTool
{
public:
	void	Init();
	void	SetPos(int i_iX, int i_iY);
	void	SetRender(bool i_bRender);
	void	Draw(CWnd* i_pWnd);

	int		GetHeight();
	int		GetWidth();

	void	OnMouseMove(CPoint i_Pos);
	DWORD	LButtonDownInBtn(CPoint i_Pos);
	bool	MousePtInRect(CPoint i_Pos);

public:
	CSCaptureTool();
	~CSCaptureTool();

private:
	CBitmap		m_BmpBase;
	CBitmap		m_BmpBtnSave;
	CBitmap		m_BmpBtnCancel;
	CBitmap		m_BmpBtnOK;

	bool		m_bRender;

	CPoint		m_Pos;
	//CSize		m_Size;

	CRect		m_rcBase;
	CRect		m_rcSave;
	CRect		m_rcCancel;
	CRect		m_rcOK;

	DWORD		m_dwState;	// 0：没有选中；1：保存；2：取消；3：完成
};



//////////////////////////////////////////////////////////////////////////
// CScreenCapture 对话框
class CScreenCapture : public CDialog
{
	DECLARE_DYNAMIC(CScreenCapture)

public:
	CScreenCapture(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CScreenCapture();

// 对话框数据
	enum { IDD = IDD_SCREENCAPTURE_DIALOG };

public:	
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:
	void	DisplayTipInfo();
	void	SaveSelectBmp();
	void	SaveBmpFile();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

private:
	CDC				m_DCMem;
	CBitmap			m_BmpScreen;	// 屏幕截图
	BLENDFUNCTION	m_Blend;
	BLENDFUNCTION	m_BlendTip;

	HCURSOR		m_hCursor;
	CFont		m_TipFont;		// 提示信息字体

	CPoint		m_ptMouse;		// 鼠标位置
	CSize		m_ScreenSize;
	CPoint		m_TipInfoPos;
	CRect		m_TipInfoRect;
	CRect		m_SelectRect;

	bool		m_bSelectOK;	// 是否选定了范围
	bool		m_bSelectRgn;	// 是否开始选择范围

	bool		m_bInRgn;		// 是否在选择的范围内

	CSCaptureTool	m_Tool;
};



