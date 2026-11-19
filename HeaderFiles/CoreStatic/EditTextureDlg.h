#if !defined(AFX_EDITTEXTUREDLG_H__103BA801_3CD6_11D4_B1E9_0000B4BD56DD__INCLUDED_)
#define AFX_EDITTEXTUREDLG_H__103BA801_3CD6_11D4_B1E9_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EditTextureDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CEditTextureDlg dialog

class AFX_EXT_CLASS   CEditTextureDlg : public CDialog
{
// Construction
public:
	CEditTextureDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CEditTextureDlg)
	enum { IDD = IDD_DIALOG_AddTextureSet };
	CString	m_filename;
	int		m_level;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CEditTextureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CEditTextureDlg)
	afx_msg void OnButtonBrowse();
	afx_msg void OnDeltaposSPINLevel(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EDITTEXTUREDLG_H__103BA801_3CD6_11D4_B1E9_0000B4BD56DD__INCLUDED_)
