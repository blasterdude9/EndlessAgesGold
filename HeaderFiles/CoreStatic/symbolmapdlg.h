#if !defined(AFX_SYMBOLMAPDLG_H__9BB573E9_B9E1_4A47_B54D_EFCF166582EB__INCLUDED_)
#define AFX_SYMBOLMAPDLG_H__9BB573E9_B9E1_4A47_B54D_EFCF166582EB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SymbolMapDlg.h : header file
//
#include "CTextRenderClass.h"
/////////////////////////////////////////////////////////////////////////////
// CSymbolMapDlg dialog

class AFX_EXT_CLASS   CSymbolMapDlg : public CDialog
{
// Construction
public:
	CSymbolMapDlg(CWnd* pParent = NULL);   // standard constructor

	CSymbolMapperObjList * m_symbolMapDBREF;
// Dialog Data
	//{{AFX_DATA(CSymbolMapDlg)
	enum { IDD = IDD_DIALOG_SymbolMapper };
	CListBox	m_symbolMapList;
	int		m_gridDensity;
	float	m_horizUVIndex;
	CString	m_idCharForm;
	float	m_charSpacingPercentage;
	float	m_vertUVIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSymbolMapDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListSymbols(int sel);
	// Generated message map functions
	//{{AFX_MSG(CSymbolMapDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONSaveDB();
	afx_msg void OnBUTTONReplace();
	afx_msg void OnBUTTONLoadDB();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonAdd();
	afx_msg void OnSelchangeLISTSymbolMapList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CString m_symbolGroupID;
	CString m_optText;
	afx_msg void OnBnClickedButtonMerge();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYMBOLMAPDLG_H__9BB573E9_B9E1_4A47_B54D_EFCF166582EB__INCLUDED_)
