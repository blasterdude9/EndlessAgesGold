#if !defined(AFX_LASERDLG_H__8A1B4B21_05BD_11D5_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_LASERDLG_H__8A1B4B21_05BD_11D5_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LaserDlg.h : header file
//
#include "CLaserClass.h"
#include "CTextureDBClass.h"
/////////////////////////////////////////////////////////////////////////////
// CLaserDlg dialog

class AFX_EXT_CLASS   CLaserDlg : public CDialog
{
// Construction
public:
	CLaserDlg(CWnd* pParent = NULL);   // standard constructor

    CTextureDBObjList * GL_textureDataBase;
	CLaserObj * m_laserObjectRef;
// Dialog Data
	//{{AFX_DATA(CLaserDlg)
	enum { IDD = IDD_DIALOG_Laser };
	CButton	m_checkFadeOut;
	CButton	m_checkLockDown;
	float	m_length;
	long	m_millisecondsOfFade;
	float	m_width;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLaserDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLaserDlg)
	afx_msg void OnBUTTONMaterialAccess();
	afx_msg void OnCHECKFadeOut();
	afx_msg void OnCHECKlockDown();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LASERDLG_H__8A1B4B21_05BD_11D5_B1EE_0000B4BD56DD__INCLUDED_)
