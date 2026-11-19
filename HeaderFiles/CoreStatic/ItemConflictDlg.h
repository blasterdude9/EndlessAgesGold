#if !defined(AFX_ITEMCONFLICTDLG_H__4B61E35D_127C_4975_BF36_CF9E45DBC640__INCLUDED_)
#define AFX_ITEMCONFLICTDLG_H__4B61E35D_127C_4975_BF36_CF9E45DBC640__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ItemConflictDlg.h : header file
//

#include "CInventoryClass.h"
/////////////////////////////////////////////////////////////////////////////
// CItemConflictDlg dialog

class AFX_EXT_CLASS   CItemConflictDlg : public CDialog
{
// Construction
public:
	CItemConflictDlg(CWnd* pParent = NULL);   // standard constructor

	int m_lastSelected;
    int m_returnCommand;
	int m_currentItemGlid; 
	void					* m_gStatusMem;
	CInventoryObjList		* m_currentInventory;
	CInventoryObj			* m_currentInvItem;
// Dialog Data
	//{{AFX_DATA(CItemConflictDlg)
	enum { IDD = IDD_DIALOG_ItemConflictDlg };
	CComboBox	m_globalInvDB;
	CString	m_currentName;
	CString	m_reference;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CItemConflictDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CItemConflictDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDelete();
	afx_msg void OnBUTTONreplaceAllSimilar();
	afx_msg void OnBUTTONReplace();
	virtual void OnOK();
	afx_msg void OnBUTTONKillSessionLoop();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ITEMCONFLICTDLG_H__4B61E35D_127C_4975_BF36_CF9E45DBC640__INCLUDED_)
