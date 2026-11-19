#if !defined(AFX_NEWACCOUNTDLG_H__0068A8E1_FF4D_11D3_B1E8_0000B4BD56DD__INCLUDED_)
#define AFX_NEWACCOUNTDLG_H__0068A8E1_FF4D_11D3_B1E8_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewAccountDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CNewAccountDlg dialog

class AFX_EXT_CLASS   CNewAccountDlg : public CDialog
{
// Construction
public:
	CNewAccountDlg(CWnd* pParent = NULL);   // standard constructor
    BOOL registered;
// Dialog Data
	//{{AFX_DATA(CNewAccountDlg)
	enum { IDD = IDD_DIALOG_NEWaccount };
	CButton	m_registeredVersion;
	CString	m_password;
	CString	m_userName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewAccountDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CNewAccountDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckRegisteredver();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWACCOUNTDLG_H__0068A8E1_FF4D_11D3_B1E8_0000B4BD56DD__INCLUDED_)
