#if !defined(AFX_BLOCKIPDLG_H__9D16F2AE_CE67_4375_9628_8370E6BCB542__INCLUDED_)
#define AFX_BLOCKIPDLG_H__9D16F2AE_CE67_4375_9628_8370E6BCB542__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BlockIpDlg.h : header file
//
#include "CBlockIPListClass.h"
/////////////////////////////////////////////////////////////////////////////
// CBlockIpDlg dialog

class AFX_EXT_CLASS   CBlockIpDlg : public CDialog
{
// Construction
public:
	CBlockIpDlg(CWnd* pParent = NULL);   // standard constructor

	CBlockIPList * m_IPBlockListRef;
// Dialog Data
	//{{AFX_DATA(CBlockIpDlg)
	enum { IDD = IDD_DIALOG_IPBlockDlg };
	CListBox	m_IPList;
	CString	m_ipAdress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBlockIpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    BOOL InListIps(int sel);
	// Generated message map functions
	//{{AFX_MSG(CBlockIpDlg)
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonDelete();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLOCKIPDLG_H__9D16F2AE_CE67_4375_9628_8370E6BCB542__INCLUDED_)
