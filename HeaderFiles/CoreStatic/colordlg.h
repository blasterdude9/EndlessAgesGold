#if !defined(AFX_COLORDLG_H__D79B1580_E2C7_11D2_9FE7_D0A74CC10000__INCLUDED_)
#define AFX_COLORDLG_H__D79B1580_E2C7_11D2_9FE7_D0A74CC10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ColorDlg.h : header file
//
#include <afxcmn.h>

/////////////////////////////////////////////////////////////////////////////
// CColorDlg dialog

class AFX_EXT_CLASS   CColorDlg : public CDialog
{
// Construction
public:
	CColorDlg(CWnd* pParent = NULL);   // standard constructor
     RECT StaticRect;
	int Top,Left,Right,Bottom;

// Dialog Data
	//{{AFX_DATA(CColorDlg)
	enum { IDD = IDD_DIALOG_COLORD3D };
	CSliderCtrl	m_sliderAlpha;
	CStatic	m_visual;
	CSliderCtrl	m_sliderRed;
	CSliderCtrl	m_sliderGreen;
	CSliderCtrl	m_sliderBlue;
	float	m_red;
	float	m_blue;
	float	m_green;
	BOOL color255;
	float	m_alpha;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CColorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CColorDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COLORDLG_H__D79B1580_E2C7_11D2_9FE7_D0A74CC10000__INCLUDED_)
