#if !defined(AFX_SCRIPTEVENTADD_H__69E88302_93BB_11D4_B1ED_0000B4BD56DD__INCLUDED_)
#define AFX_SCRIPTEVENTADD_H__69E88302_93BB_11D4_B1ED_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScriptEventAdd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScriptEventAdd dialog

class  AFX_EXT_CLASS CScriptEventAdd : public CDialog
{
// Construction
public:
	CScriptEventAdd(CWnd* pParent = NULL);   // standard constructor
    int m_finalEventInt;
// Dialog Data
	//{{AFX_DATA(CScriptEventAdd)
	enum { IDD = IDD_DIALOG_ScriptEventAdd };
	CComboBox	m_event;
	int		m_miscInt;
	CString	m_changeableText;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptEventAdd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScriptEventAdd)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeComboEvent();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIPTEVENTADD_H__69E88302_93BB_11D4_B1ED_0000B4BD56DD__INCLUDED_)
