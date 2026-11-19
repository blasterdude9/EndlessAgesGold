#if !defined(AFX_MENUENTITYLINKDLG_H__CFBAC1A2_ABAA_11D4_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_MENUENTITYLINKDLG_H__CFBAC1A2_ABAA_11D4_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MenuEntityLinkDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMenuEntityLinkDlg dialog

class CMenuEntityLinkDlg : public CDialog
{
// Construction
public:
	CMenuEntityLinkDlg(CWnd* pParent = NULL);   // standard constructor
    BOOL runtimeDB;
// Dialog Data
	//{{AFX_DATA(CMenuEntityLinkDlg)
	enum { IDD = IDD_DIALOG_MenuEntityLink };
	CButton	m_runtimeDBCheckBox;
	int		m_databaseIndex;
	int		m_initialAnimationIndex;
	int		m_aiCfgOverride;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuEntityLinkDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMenuEntityLinkDlg)
	afx_msg void OnCHECKruntimeDB();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUENTITYLINKDLG_H__CFBAC1A2_ABAA_11D4_B1EE_0000B4BD56DD__INCLUDED_)
