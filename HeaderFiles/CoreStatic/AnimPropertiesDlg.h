#if !defined(AFX_ANIMPROPERTIESDLG_H__67D00441_31AF_11D3_AFDD_F002B3087325__INCLUDED_)
#define AFX_ANIMPROPERTIESDLG_H__67D00441_31AF_11D3_AFDD_F002B3087325__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AnimPropertiesDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAnimPropertiesDlg dialog

class AFX_EXT_CLASS   CAnimPropertiesDlg : public CDialog
{
// Construction
public:
	CAnimPropertiesDlg(CWnd* pParent = NULL);   // standard constructor
    BOOL useInterpolation;
	BOOL mustFinishAnim;
// Dialog Data
	//{{AFX_DATA(CAnimPropertiesDlg)
	enum { IDD = IDD_DIALOG_AnimProperties };
	CButton	m_mustFinishAnim;
	CButton	m_useInterpolation;
	int		m_animationAction;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAnimPropertiesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAnimPropertiesDlg)
	afx_msg void OnCHECKUseInterpolation();
	virtual BOOL OnInitDialog();
	afx_msg void OnCHECKMustFinishAnim();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ANIMPROPERTIESDLG_H__67D00441_31AF_11D3_AFDD_F002B3087325__INCLUDED_)
