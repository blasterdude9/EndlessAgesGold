#if !defined(AFX_EDITSTARTINVENTORYDLG_H__27E481A1_77C0_11D4_B1ED_0000B4BD56DD__INCLUDED_)
#define AFX_EDITSTARTINVENTORYDLG_H__27E481A1_77C0_11D4_B1ED_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditStartInventoryDlg.h : header file
//
#include "CArmedInventoryClass.h"
#include "CMovementModel.h"
/////////////////////////////////////////////////////////////////////////////
// CEditStartInventoryDlg dialog

class AFX_EXT_CLASS   CEditStartInventoryDlg : public CDialog
{
// Construction
public:
	CEditStartInventoryDlg(CWnd* pParent = NULL);   // standard constructor
    CArmedInventoryList      * m_armedInventoryDatabase;
	//CMovementObj           * m_modelRef;
	CCfgObjList              * m_autoCfgRef;
	CItemObjList             * m_itemDBRef;
	void					 * m_gstatusMem;

// Dialog Data
	//{{AFX_DATA(CEditStartInventoryDlg)
	enum { IDD = IDD_DIALOG_EditStartInventory };
	CButton	m_radioArmed;
	CComboBox	m_globalInventory;
	CListBox	m_listInventory;
	int		m_itemCountLimit;
	int		m_maxWieghtLimit;
	int		m_quanity;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditStartInventoryDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InListInventory(int sel);
	void InListGlobalItems(int sel) ;
	// Generated message map functions
	//{{AFX_MSG(CEditStartInventoryDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDelete();
	afx_msg void OnSelchangeLISTInventory();
	afx_msg void OnChangeEDITItemCountLimit();
	afx_msg void OnChangeEDITMaxWieghtLimit();
	afx_msg void OnCHECKarmed();
	afx_msg void OnButtonAdd();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITSTARTINVENTORYDLG_H__27E481A1_77C0_11D4_B1ED_0000B4BD56DD__INCLUDED_)
