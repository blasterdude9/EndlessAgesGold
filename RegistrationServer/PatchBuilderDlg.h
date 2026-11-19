#include "CFilePatchClass.h"

#if !defined(AFX_PATCHBUILDERDLG_H__B9448E91_F19E_44AC_8F9F_1B1E4074FE32__INCLUDED_)
#define AFX_PATCHBUILDERDLG_H__B9448E91_F19E_44AC_8F9F_1B1E4074FE32__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PatchBuilderDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPatchBuilderDlg dialog

class CPatchBuilderDlg : public CDialog
{
// Construction
public:
	CPatchBuilderDlg(CWnd* pParent = NULL);   // standard constructor



// Dialog Data
	//{{AFX_DATA(CPatchBuilderDlg)
	enum { IDD = IDD_DIALOG_PatchBuilder };
	CButton	m_checkExistingFile;
	CString	m_newFilePath;
	CString	m_oldFilePath;
	long	m_patchServerSearchLength;
	CString	m_patchLabel;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatchBuilderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CPatchBuilderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONBroewseForOldFile();
	afx_msg void OnBUTTONBrowseForNewFile();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATCHBUILDERDLG_H__B9448E91_F19E_44AC_8F9F_1B1E4074FE32__INCLUDED_)
