#if !defined(AFX_CURRENCYDLG_H__62032261_514A_11D5_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_CURRENCYDLG_H__62032261_514A_11D5_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CurrencyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCurrencyDlg dialog

class AFX_EXT_CLASS   CCurrencyDlg : public CDialog
{
// Construction
public:
	CCurrencyDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCurrencyDlg)
	enum { IDD = IDD_DIALOG_CurrencyObject };
	CString	m_currencyName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCurrencyDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCurrencyDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CURRENCYDLG_H__62032261_514A_11D5_B1EE_0000B4BD56DD__INCLUDED_)
