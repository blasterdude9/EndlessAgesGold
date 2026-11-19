#if !defined(AFX_AIRAIDDLG_H__38F4F350_C704_4C35_A174_401A6EF5E7F3__INCLUDED_)
#define AFX_AIRAIDDLG_H__38F4F350_C704_4C35_A174_401A6EF5E7F3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AIRaidDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAIRaidDlg dialog

class CAIRaidDlg : public CDialog
{
// Construction
public:
	CAIRaidDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CAIRaidDlg)
	enum { IDD = IDD_DIALOG_AIRaidDlg };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAIRaidDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAIRaidDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AIRAIDDLG_H__38F4F350_C704_4C35_A174_401A6EF5E7F3__INCLUDED_)
