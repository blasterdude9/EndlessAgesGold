#if !defined(AFX_NEWACCOUNTSMODULE_H__1027973C_C65D_46B6_9CB4_83EF53F0108A__INCLUDED_)
#define AFX_NEWACCOUNTSMODULE_H__1027973C_C65D_46B6_9CB4_83EF53F0108A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// NewAccountsModule.h : header file
//
#include "CMultiplayerClass.h"
/////////////////////////////////////////////////////////////////////////////
// CNewAccountsModule dialog

class CNewAccountsModule : public CDialog
{
// Construction
public:
	CNewAccountsModule(CWnd* pParent = NULL);   // standard constructor
    CAccountObjectList * accountDatabaseDBREF;
	CMultiplayerObj *    multiplayerRef;
	CDialogBar       *   m_multiPlayerDlgRef;

// Dialog Data
	//{{AFX_DATA(CNewAccountsModule)
	enum { IDD = IDD_DIALOG_newAccounts };
	CButton	m_registered;
	CString	m_password;
	CString	m_username;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNewAccountsModule)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListAccounts(int sel);
	CString ConvToString(int number);
	// Generated message map functions
	//{{AFX_MSG(CNewAccountsModule)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NEWACCOUNTSMODULE_H__1027973C_C65D_46B6_9CB4_83EF53F0108A__INCLUDED_)
