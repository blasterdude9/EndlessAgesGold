#if !defined(AFX_LODSYSTEMDLG_H__0A057E61_1531_11D4_B1E8_0000B4BD56DD__INCLUDED_)
#define AFX_LODSYSTEMDLG_H__0A057E61_1531_11D4_B1E8_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LodSystemDlg.h : header file
//
#include "CWldObject.h"
/////////////////////////////////////////////////////////////////////////////
// CLodSystemDlg dialog

class AFX_EXT_CLASS   CLodSystemDlg : public CDialog
{
// Construction
public:
	CLodSystemDlg(CWnd* pParent = NULL);   // standard constructor
    BOOL m_lodType;
	LPDIRECT3DDEVICE9 g_pd3dDeviceRef;
// Dialog Data
	//{{AFX_DATA(CLodSystemDlg)
	enum { IDD = IDD_DIALOG_ADD_LOD };
	CButton	m_radioBoundingBoxBased;
	CButton	m_radioDistanceBased;
	CString	m_fileName;
	float	m_activation;
	float	m_maxX;
	float	m_maxY;
	float	m_maxZ;
	float	m_minX;
	float	m_minY;
	float	m_minZ;
	//}}AFX_DATA

	CButton	m_radioAlphaDissolveBased;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLodSystemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLodSystemDlg)
	afx_msg void OnButtonBrowse();
	afx_msg void OnRADIOBoundingBoxBased();
	afx_msg void OnRADIODistanceBased();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLoadfromarw();
	afx_msg void OnBnClickedRadioAlphadissolve();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LODSYSTEMDLG_H__0A057E61_1531_11D4_B1E8_0000B4BD56DD__INCLUDED_)
