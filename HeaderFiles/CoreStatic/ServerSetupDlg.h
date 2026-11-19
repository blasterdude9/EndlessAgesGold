#if !defined(AFX_SERVERSETUPDLG_H__F4BA6B02_4160_11D3_AFDD_E5048660E725__INCLUDED_)
#define AFX_SERVERSETUPDLG_H__F4BA6B02_4160_11D3_AFDD_E5048660E725__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ServerSetupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CServerSetupDlg dialog

class AFX_EXT_CLASS   CServerSetupDlg : public CDialog
{
// Construction
public:
	CServerSetupDlg(CWnd* pParent = NULL);   // standard constructor
    BOOL passwordRequired;
	BOOL encrypted;

// Dialog Data
	//{{AFX_DATA(CServerSetupDlg)
	enum { IDD = IDD_DIALOG_SERVERsetup };
	CButton	m_checkPasswordRequired;
	CButton	m_checkEncyption;
	int		m_connectionLimit;
	CString	m_password;
	CString	m_serverName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CServerSetupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CServerSetupDlg)
	afx_msg void OnCHECKEncyption();
	afx_msg void OnCHECKPasswordRequired();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SERVERSETUPDLG_H__F4BA6B02_4160_11D3_AFDD_E5048660E725__INCLUDED_)
