#if !defined(AFX_GETSIGNIFICANTOPTIONSDLG_H__D2D91525_43FD_4DEF_818D_B513078D4AEA__INCLUDED_)
#define AFX_GETSIGNIFICANTOPTIONSDLG_H__D2D91525_43FD_4DEF_818D_B513078D4AEA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// GetSignificantOptionsDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// GetSignificantOptionsDlg dialog

class AFX_EXT_CLASS   GetSignificantOptionsDlg : public CDialog
{
// Construction
public:
	GetSignificantOptionsDlg(CWnd* pParent = NULL);   // standard constructor
    BOOL m_getInverseOnly;
// Dialog Data
	//{{AFX_DATA(GetSignificantOptionsDlg)
	enum { IDD = IDD_DIALOG_GetSignificantOptions };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(GetSignificantOptionsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(GetSignificantOptionsDlg)
	afx_msg void OnCHECKGetInverseOnly();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GETSIGNIFICANTOPTIONSDLG_H__D2D91525_43FD_4DEF_818D_B513078D4AEA__INCLUDED_)
