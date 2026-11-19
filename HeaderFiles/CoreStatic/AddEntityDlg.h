#if !defined(AFX_ADDENTITYDLG_H__C8A05440_D6DE_11D2_9FE7_D0A74CC10000__INCLUDED_)
#define AFX_ADDENTITYDLG_H__C8A05440_D6DE_11D2_9FE7_D0A74CC10000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AddEntityDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CAddEntityDlg dialog

class AFX_EXT_CLASS   CAddEntityDlg : public CDialog
{
// Construction
public:
	CAddEntityDlg(CWnd* pParent = NULL);   // standard constructor
    BOOL saveCollisionInfo;
	CString m_pathGlobal;
	CString m_folderPath;
// Dialog Data
	//{{AFX_DATA(CAddEntityDlg)
	enum { IDD = IDD_DIALOG_ADDENTITY };
	CButton	m_checkSaveCollisionInfo;
	CString	m_entityName;
	CString	m_filename;
	CString	m_secondaryCollisionFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddEntityDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CAddEntityDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonBrowse();
	afx_msg void OnCHECKSaveCollisionInfo();
	afx_msg void OnBUTTONBrowseSecondary();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ADDENTITYDLG_H__C8A05440_D6DE_11D2_9FE7_D0A74CC10000__INCLUDED_)
