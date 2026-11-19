#if !defined(AFX_ALPHATEXTUREDLG_H__961F13E1_2C19_11D3_AFDD_C6842D9F0725__INCLUDED_)
#define AFX_ALPHATEXTUREDLG_H__961F13E1_2C19_11D3_AFDD_C6842D9F0725__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AlphaTextureDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAlphaTextureDlg dialog

class AFX_EXT_CLASS   CAlphaTextureDlg : public CDialog
{
// Construction
public:
	CAlphaTextureDlg(CWnd* pParent = NULL);   // standard constructor
    int textureSize;
	BOOL enabled;
// Dialog Data
	//{{AFX_DATA(CAlphaTextureDlg)
	enum { IDD = IDD_DIALOG_ALPHATEXTURE };
	CButton	m_checkEnabled;
	CButton	m_check64X64;
	CButton	m_check256X256;
	CButton	m_check128X128;
	int		m_channel;
	CString	m_fileName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAlphaTextureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAlphaTextureDlg)
	afx_msg void OnBUTTONBrowseFileName();
	virtual BOOL OnInitDialog();
	afx_msg void OnCheck128x128();
	afx_msg void OnCheck256x256();
	afx_msg void OnCheck64x64();
	afx_msg void OnCheckEnabled();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALPHATEXTUREDLG_H__961F13E1_2C19_11D3_AFDD_C6842D9F0725__INCLUDED_)
