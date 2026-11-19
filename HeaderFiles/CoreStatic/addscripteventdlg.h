#if !defined(AFX_ADDSCRIPTEVENTDLG_H__6C56CA61_574A_11D3_AFDD_0000B45FDB0F__INCLUDED_)
#define AFX_ADDSCRIPTEVENTDLG_H__6C56CA61_574A_11D3_AFDD_0000B45FDB0F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddScriptEventDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddScriptEventDlg dialog

class AFX_EXT_CLASS   CAddScriptEventDlg : public CDialog
{
// Construction
public:
	CAddScriptEventDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAddScriptEventDlg)
	enum { IDD = IDD_DIALOG_ADDScriptEvent };
	CString	m_event;
	int		m_miscInt;
	CString	m_miscText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddScriptEventDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddScriptEventDlg)
	afx_msg void OnButtonBrowse();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDSCRIPTEVENTDLG_H__6C56CA61_574A_11D3_AFDD_0000B45FDB0F__INCLUDED_)
