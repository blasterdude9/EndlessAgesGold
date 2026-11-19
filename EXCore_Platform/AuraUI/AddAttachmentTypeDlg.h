#if !defined(AFX_ADDATTACHMENTTYPEDLG_H__D8A50B42_E5C5_11D4_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_ADDATTACHMENTTYPEDLG_H__D8A50B42_E5C5_11D4_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddAttachmentTypeDlg.h : header file
//
#include "CArmedInventoryClass.h"
#include "CItemClass.h"
#include "CABFunctionLib.h"
#include "CMovementModel.h"
/////////////////////////////////////////////////////////////////////////////
// CAddAttachmentTypeDlg dialog

class  CAddAttachmentTypeDlg : public CDialog
{
// Construction
public:
	CAddAttachmentTypeDlg(CWnd* pParent = NULL);   // standard constructor

	CArmedInventoryList * m_armedInventoryListRef;
	CItemObj            * m_itemRef;
	CABFunctionLib        m_cbLib;
	CMovementObjList    * m_enitityDB;
// Dialog Data
	//{{AFX_DATA(CAddAttachmentTypeDlg)
	enum { IDD = IDD_DIALOG_AddAttachment };
	CComboBox	m_attachments;
	CListBox	m_currentItems;
	CComboBox	m_edbList;
	int         m_dimCfgIndex;
	int         m_dimCfgChangeTo;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddAttachmentTypeDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListAttachments(int sel);
	void InListCurrentItems(int sel);
	// Generated message map functions
	//{{AFX_MSG(CAddAttachmentTypeDlg)
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	virtual BOOL OnInitDialog();
	void InListEDB(int sel);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonatupdateitem();
public:
	afx_msg void OnLbnSelchangeListcurrentitems();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDATTACHMENTTYPEDLG_H__D8A50B42_E5C5_11D4_B1EE_0000B4BD56DD__INCLUDED_)
