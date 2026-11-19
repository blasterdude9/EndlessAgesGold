#if !defined(AFX_SCRIPTADD_H__69E88301_93BB_11D4_B1ED_0000B4BD56DD__INCLUDED_)
#define AFX_SCRIPTADD_H__69E88301_93BB_11D4_B1ED_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScriptAdd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScriptAdd dialog

class  AFX_EXT_CLASS CScriptAdd : public CDialog
{
// Construction
public:
	CScriptAdd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CScriptAdd)
	enum { IDD = IDD_DIALOG_ScriptNew };
	CString	m_scriptDescription;
	CString	m_scriptName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScriptAdd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScriptAdd)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCRIPTADD_H__69E88301_93BB_11D4_B1ED_0000B4BD56DD__INCLUDED_)
