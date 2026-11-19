#pragma once

// CommerceDlg.h : header file
//
#include "CCommerceClass.h"
/////////////////////////////////////////////////////////////////////////////
// CCommerceDlg dialog

class AFX_EXT_CLASS   CCommerceDlg : public CDialog
{
// Construction
public:
	CCommerceDlg(CWnd* pParent = NULL);   // standard constructor

    void					* m_gStatusMem;
	CCommerceObjList        * m_commerceObjDBRef;
// Dialog Data
	//{{AFX_DATA(CCommerceDlg)
	enum { IDD = IDD_DIALOG_Commerce };
	CButton	m_willDealWithMurderers;
	CListBox	m_storeInventory;
	CListBox	m_globalInventory;
	CListBox	m_commerceDB;
	int		m_storeID;
	CString	m_storeName;
	float	m_popoutBoxSizeMaxX;
	float	m_popoutBoxSizeMaxY;
	float	m_popoutBoxSizeMaxZ;
	float	m_popoutBoxSizeMinX;
	float	m_popoutBoxSizeMinY;
	float	m_popoutBoxSizeMinZ;
	int		m_worldID;
	int		m_menuBind;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCommerceDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListCommerceDB(int sel);
	void InListStoreInventory(int sel);
	// Generated message map functions
	//{{AFX_MSG(CCommerceDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONAddItem();
	afx_msg void OnBUTTONAddStore();
	afx_msg void OnBUTTONDeleteItem();
	afx_msg void OnBUTTONDeleteStore();
	afx_msg void OnBUTTONReplaceStore();
	afx_msg void OnSelchangeLISTCommerceDB();
	afx_msg void OnButtonLoad();
	afx_msg void OnButtonSave();
	afx_msg void OnCHECKwillDealWithMurderers();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLbnSelchangeListStoreinventory();
	int m_raceFilterID;
	afx_msg void OnBnClickedButtonReplaceitem();
	afx_msg void OnBnClickedButtonLoadbox();
	afx_msg void OnBnClickedButtonInsertitem();
	afx_msg void OnBnClickedButtonBumpbox();
	float m_bumpAmount;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
