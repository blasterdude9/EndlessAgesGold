#if !defined(AFX_WORLDGROUPDLG_H__9E604B85_E976_11D3_B1E8_0000B4BD56DD__INCLUDED_)
#define AFX_WORLDGROUPDLG_H__9E604B85_E976_11D3_B1E8_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WorldGroupDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWorldGroupDlg dialog

class AFX_EXT_CLASS   CWorldGroupDlg : public CDialog
{
// Construction
public:
	CWorldGroupDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWorldGroupDlg)
	enum { IDD = IDD_DIALOG_WorldGroupDlg };
	int		m_groupID;
	CString	m_groupName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWorldGroupDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWorldGroupDlg)
	afx_msg void OnDeltaposSPINIDSpinner(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WORLDGROUPDLG_H__9E604B85_E976_11D3_B1E8_0000B4BD56DD__INCLUDED_)
