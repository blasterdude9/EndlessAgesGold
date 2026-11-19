#if !defined(AFX_PROGRESSMETERDLG_H__D55B2921_D43A_11D4_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_PROGRESSMETERDLG_H__D55B2921_D43A_11D4_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ProgressMeterDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CProgressMeterDlg dialog

class AFX_EXT_CLASS   CProgressMeterDlg : public CDialog
{
// Construction
public:
	CProgressMeterDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CProgressMeterDlg)
	enum { IDD = IDD_DIALOG_PROGRESS };
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CProgressMeterDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CProgressMeterDlg)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROGRESSMETERDLG_H__D55B2921_D43A_11D4_B1EE_0000B4BD56DD__INCLUDED_)
