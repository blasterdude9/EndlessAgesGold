#if !defined(AFX_PATCHSYSTEMDLG_H__E6F6D069_50B6_4D63_ACAD_4FE176E8501C__INCLUDED_)
#define AFX_PATCHSYSTEMDLG_H__E6F6D069_50B6_4D63_ACAD_4FE176E8501C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PatchSystemDlg.h : header file
//
#include "CABFunctionLib.h"
#include "CFilePatchClass.h"
/////////////////////////////////////////////////////////////////////////////
// CPatchSystemDlg dialog

class AFX_EXT_CLASS   CPatchSystemDlg : public CDialog
{
// Construction
public:
	CPatchSystemDlg(CWnd* pParent = NULL);   // standard constructor
    CFilePatchObjectList * m_patchDatabaseRef;
	CABFunctionLib         m_cabLib;
// Dialog Data
	//{{AFX_DATA(CPatchSystemDlg)
	enum { IDD = IDD_DIALOG_PatchSystem };
	CListBox	m_patchHistory;
	CString	m_oldFileName;
	CString	m_browseNewVersion;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatchSystemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListPatchDatabase(int sel);
	// Generated message map functions
	//{{AFX_MSG(CPatchSystemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonAdd();
	afx_msg void OnButtonBrowse();
	afx_msg void OnButtonBrowsenewversion();
	afx_msg void OnButtonCreatepatch();
	afx_msg void OnButtonDelete();
	afx_msg void OnBUTTONSaveDatabase();
	afx_msg void OnButtonTestpatch();
	afx_msg void OnBUTTONLoadDatabase();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATCHSYSTEMDLG_H__E6F6D069_50B6_4D63_ACAD_4FE176E8501C__INCLUDED_)
