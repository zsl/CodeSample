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

	DWORD		m_dwState;	// 0��û��ѡ�У�1�����棻2��ȡ����3�����
};



//////////////////////////////////////////////////////////////////////////
// CScreenCapture �Ի���
class CScreenCapture : public CDialog
{
	DECLARE_DYNAMIC(CScreenCapture)

public:
	CScreenCapture(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CScreenCapture();

// �Ի�������
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
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK();

	DECLARE_MESSAGE_MAP()

private:
	CDC				m_DCMem;
	CBitmap			m_BmpScreen;	// ��Ļ��ͼ
	BLENDFUNCTION	m_Blend;
	BLENDFUNCTION	m_BlendTip;

	HCURSOR		m_hCursor;
	CFont		m_TipFont;		// ��ʾ��Ϣ����

	CPoint		m_ptMouse;		// ���λ��
	CSize		m_ScreenSize;
	CPoint		m_TipInfoPos;
	CRect		m_TipInfoRect;
	CRect		m_SelectRect;

	bool		m_bSelectOK;	// �Ƿ�ѡ���˷�Χ
	bool		m_bSelectRgn;	// �Ƿ�ʼѡ��Χ

	bool		m_bInRgn;		// �Ƿ���ѡ��ķ�Χ��

	CSCaptureTool	m_Tool;
};



