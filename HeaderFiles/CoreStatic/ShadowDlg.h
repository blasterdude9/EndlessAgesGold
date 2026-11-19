#if !defined(AFX_SHADOWDLG_H__65CDD881_3037_11D3_AFDD_A71B33107E25__INCLUDED_)
#define AFX_SHADOWDLG_H__65CDD881_3037_11D3_AFDD_A71B33107E25__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ShadowDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CShadowDlg dialog

class AFX_EXT_CLASS   CShadowDlg : public CDialog
{
// Construction
public:
	CShadowDlg(CWnd* pParent = NULL);   // standard constructor
    BOOL shadowEnabled;
	BOOL useAlternateImage;
	BOOL trueAlpha;
// Dialog Data
	//{{AFX_DATA(CShadowDlg)
	enum { IDD = IDD_DIALOG_Shadow };
	CButton	m_shadowEnabled;
	float	m_above;
	float	m_depth;
	float	m_collisionRadius;
	float	m_radius;
	int		m_textureIndex;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CShadowDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CShadowDlg)
	afx_msg void OnBUTTONBrowseAlternateFile();
	afx_msg void OnCHECKEnable();
	afx_msg void OnCHECKUseAlternateFile();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckTruealpha();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SHADOWDLG_H__65CDD881_3037_11D3_AFDD_A71B33107E25__INCLUDED_)
