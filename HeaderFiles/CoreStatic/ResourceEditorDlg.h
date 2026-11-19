#if !defined(AFX_RESOURCEEDITORDLG_H__527BC961_6A27_11D4_B1ED_0000B4BD56DD__INCLUDED_)
#define AFX_RESOURCEEDITORDLG_H__527BC961_6A27_11D4_B1ED_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ResourceEditorDlg.h : header file
//
#include "CResourceClass.h"
/////////////////////////////////////////////////////////////////////////////
// CResourceEditorDlg dialog

class AFX_EXT_CLASS   CResourceEditorDlg : public CDialog
{
// Construction
public:
	CResourceEditorDlg(CWnd* pParent = NULL);   // standard constructor

	CResourceObjectList * resourceListRef;
	BOOL m_enableMaxVar;
	BOOL m_enableRegenerationVar;
// Dialog Data
	//{{AFX_DATA(CResourceEditorDlg)
	enum { IDD = IDD_DIALOG_ResourceEditor };
	CButton	m_enableRegenerate;
	CButton	m_enableMaxAmount;
	CListBox	m_listResource;
	CString	m_name;
	float	m_regenerationVar;
	int		m_type;
	int		m_amount;
	int		m_maxAmount;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CResourceEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InList(int sel);
	// Generated message map functions
	//{{AFX_MSG(CResourceEditorDlg)
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	afx_msg void OnBUTTONReplaceValues();
	afx_msg void OnCHECKmaxAmountEnable();
	afx_msg void OnCHECKRegenEnable();
	afx_msg void OnSelchangeLISTResources();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESOURCEEDITORDLG_H__527BC961_6A27_11D4_B1ED_0000B4BD56DD__INCLUDED_)
