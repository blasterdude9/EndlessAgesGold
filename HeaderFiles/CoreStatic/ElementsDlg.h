#if !defined(AFX_ELEMENTSDLG_H__7739C9CC_69B9_41DC_B86C_F0F156ACB849__INCLUDED_)
#define AFX_ELEMENTSDLG_H__7739C9CC_69B9_41DC_B86C_F0F156ACB849__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ElementsDlg.h : header file
//
#include "CElementsClass.h"
/////////////////////////////////////////////////////////////////////////////
// CElementsDlg dialog

class AFX_EXT_CLASS   CElementsDlg : public CDialog
{
// Construction
public:
	CElementsDlg(CWnd* pParent = NULL);   // standard constructor

	CElementObjList * m_elementDBREF;
// Dialog Data
	//{{AFX_DATA(CElementsDlg)
	enum { IDD = IDD_DIALOG_Elements };
	CListBox	m_elementsList;
	CString	m_elementName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CElementsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListElementals(int sel);
	// Generated message map functions
	//{{AFX_MSG(CElementsDlg)
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonLoad();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonExporttosql();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ELEMENTSDLG_H__7739C9CC_69B9_41DC_B86C_F0F156ACB849__INCLUDED_)
