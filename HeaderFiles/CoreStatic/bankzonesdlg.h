#if !defined(AFX_BANKZONESDLG_H__E30DC9E1_578B_11D5_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_BANKZONESDLG_H__E30DC9E1_578B_11D5_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BankZonesDlg.h : header file
//
#include "CBankClass.h"
/////////////////////////////////////////////////////////////////////////////
// CBankZonesDlg dialog

class AFX_EXT_CLASS   CBankZonesDlg : public CDialog
{
// Construction
public:
	CBankZonesDlg(CWnd* pParent = NULL);   // standard constructor

	CBankObjList * m_bankDBRef;

// Dialog Data
	//{{AFX_DATA(CBankZonesDlg)
	enum { IDD = IDD_DIALOG_BANKZONES };
	CButton	m_willDealWithMurderers;
	CListBox	m_bankDBList;
	int		m_menuBind;
	float	m_popoutBoxMaxX;
	float	m_popoutBoxMaxY;
	float	m_popoutBoxMaxZ;
	float	m_popoutBoxMinX;
	float	m_popoutBoxMinY;
	float	m_popoutBoxMinZ;
	int		m_worldID;
	CString	m_bankName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBankZonesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListBankZones(int sel);
	// Generated message map functions
	//{{AFX_MSG(CBankZonesDlg)
	afx_msg void OnBUTTONAddBank();
	afx_msg void OnBUTTONDeleteBank();
	afx_msg void OnBUTTONReplaceBank();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonLoad();
	afx_msg void OnSelchangeLISTBankDB();
	afx_msg void OnCHECKwillDealWithMurderers();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonloadbox();
	afx_msg void OnBnClicked4294();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BANKZONESDLG_H__E30DC9E1_578B_11D5_B1EE_0000B4BD56DD__INCLUDED_)
