#if !defined(AFX_ADDTEXTUREDLG_H__2F045AC1_0982_11D4_B1E8_0000B4BD56DD__INCLUDED_)
#define AFX_ADDTEXTUREDLG_H__2F045AC1_0982_11D4_B1E8_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddTextureDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddTextureDlg dialog

class AFX_EXT_CLASS   CAddTextureDlg : public CDialog
{
// Construction
public:
	CAddTextureDlg(CWnd* pParent = NULL);   // standard constructor
    CString textureBaseName;
	CString FileNames[2000];
	CString pathOfFiles;
	int fileCounter;
// Dialog Data
	//{{AFX_DATA(CAddTextureDlg)
	enum { IDD = IDD_DIALOG_AddTexture };
	CListBox	m_fileList;
	CString	m_fileName;
	int		m_mipLevels;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddTextureDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddTextureDlg)
	afx_msg void OnDeltaposSPINMipSpinner(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBUTTONBrowseFileName();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDTEXTUREDLG_H__2F045AC1_0982_11D4_B1E8_0000B4BD56DD__INCLUDED_)
