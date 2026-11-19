#if !defined(AFX_MENUADDUNITDLG_H__17407EA1_ACE4_11D3_B7BC_0000B4BD56DD__INCLUDED_)
#define AFX_MENUADDUNITDLG_H__17407EA1_ACE4_11D3_B7BC_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MenuAddUnitDlg.h : header file
//
#include "CMenuVisual.h"

/////////////////////////////////////////////////////////////////////////////
// CMenuAddUnitDlg dialog

class AFX_EXT_CLASS   CMenuAddUnitDlg : public CDialog
{
// Construction
public:
	CMenuAddUnitDlg(CWnd* pParent = NULL);   // standard constructor
	CString m_name;
	int		m_visualID;
	int		m_freeVisualID;
	int		m_visualType;
	// Dialog Data
	//{{AFX_DATA(CMenuAddUnitDlg)
	enum { IDD = IDD_DIALOG_MenuSysAddVisual };
	//}}AFX_DATA
    
	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenuAddUnitDlg)
	//}}AFX_VIRTUAL
	
// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMenuAddUnitDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonGetFreeVisualID();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENUADDUNITDLG_H__17407EA1_ACE4_11D3_B7BC_0000B4BD56DD__INCLUDED_)
