#if !defined(AFX_ADDCONTROLEVENTDLG_H__F8940741_0BA0_11D3_9FE7_B0A74CC10000__INCLUDED_)
#define AFX_ADDCONTROLEVENTDLG_H__F8940741_0BA0_11D3_9FE7_B0A74CC10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddControlEventDlg.h : header file
//
#include "ControlsClass.h"
#include "afxwin.h"
/////////////////////////////////////////////////////////////////////////////
// CAddControlEventDlg dialog

class AFX_EXT_CLASS   CAddControlEventDlg : public CDialog
{
// Construction
public:
	CAddControlEventDlg(CWnd* pParent = NULL);   // standard constructor
    int finalKeyNum;
	int finalActionNum;
	int eventBasis;
	ControlInfoList * m_controlListRef;
// Dialog Data
	//{{AFX_DATA(CAddControlEventDlg)
	enum { IDD = IDD_DIALOG_ControlAddEvent };
	CString	m_comboActions;
	CString	m_comboKeyButton;
	float	m_miscFloat1;
	float	m_miscFloat3;
	float	m_miscFloat2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddControlEventDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CAddControlEventDlg)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_actionDropList;
	CComboBox m_keyList;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDCONTROLEVENTDLG_H__F8940741_0BA0_11D3_9FE7_B0A74CC10000__INCLUDED_)
