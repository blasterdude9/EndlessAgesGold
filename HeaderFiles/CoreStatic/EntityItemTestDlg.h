#if !defined(AFX_ENTITYITEMTESTDLG_H__BE799E61_E684_11D4_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_ENTITYITEMTESTDLG_H__BE799E61_E684_11D4_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EntityItemTestDlg.h : header file
//
#include "CItemClass.h"
/////////////////////////////////////////////////////////////////////////////
// CEntityItemTestDlg dialog

class AFX_EXT_CLASS   CEntityItemTestDlg : public CDialog
{
// Construction
public:
	CEntityItemTestDlg(CWnd* pParent = NULL);   // standard constructor
    int m_selection;
	CItemObjList * m_itemObjectListRef;

// Dialog Data
	//{{AFX_DATA(CEntityItemTestDlg)
	enum { IDD = IDD_DIALOG_ItemTest };
	CListBox	m_itemList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEntityItemTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEntityItemTestDlg)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ENTITYITEMTESTDLG_H__BE799E61_E684_11D4_B1EE_0000B4BD56DD__INCLUDED_)
