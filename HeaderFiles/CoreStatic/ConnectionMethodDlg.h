#if !defined(AFX_CONNECTIONMETHODDLG_H__F4BA6B01_4160_11D3_AFDD_E5048660E725__INCLUDED_)
#define AFX_CONNECTIONMETHODDLG_H__F4BA6B01_4160_11D3_AFDD_E5048660E725__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConnectionMethodDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CConnectionMethodDlg dialog

class AFX_EXT_CLASS   CConnectionMethodDlg : public CDialog
{
// Construction
public:
	CConnectionMethodDlg(CWnd* pParent = NULL);   // standard constructor
    CString Sessions[50];
	int SessionCount;
	int finalSelection;
// Dialog Data
	//{{AFX_DATA(CConnectionMethodDlg)
	enum { IDD = IDD_DIALOG_ConnectionMethods };
	CListBox	m_listMethods;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CConnectionMethodDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CConnectionMethodDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkLISTConnectionMethods();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CONNECTIONMETHODDLG_H__F4BA6B01_4160_11D3_AFDD_E5048660E725__INCLUDED_)
