
#pragma once

// UpgradeWormDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUpgradeWormDlg dialog

class AFX_EXT_CLASS   CUpgradeWormDlg : public CDialog
{
// Construction
public:
	CUpgradeWormDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL m_disarmAll;
	BOOL m_deleteAllUnknownItems;
	BOOL m_useDeathCfg;
	BOOL m_rebuildGroupDatabases;
	BOOL m_clearAll;
// Dialog Data
	//{{AFX_DATA(CUpgradeWormDlg)
	enum { IDD = IDD_DIALOG_UpgradeWormDlg };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUpgradeWormDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUpgradeWormDlg)
	afx_msg void OnCHECKDisarmAll();
	virtual BOOL OnInitDialog();
	afx_msg void OnCHECKDeleteUnknownItems();
	afx_msg void OnCHECKuseDeathCfg();
	afx_msg void OnCHECKrebuildGroupDatabases();
	afx_msg void OnCHECKclear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	CString m_specialCode;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

