#if !defined(AFX_SKILLRANGESDLG_H__568C4061_34FD_11D5_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_SKILLRANGESDLG_H__568C4061_34FD_11D5_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkillRangesDlg.h : header file
//
#include "CSkillClass.h"
/////////////////////////////////////////////////////////////////////////////
// CSkillRangesDlg dialog

class AFX_EXT_CLASS   CSkillRangesDlg : public CDialog
{
// Construction
public:
	CSkillRangesDlg(CWnd* pParent = NULL);   // standard constructor

	CLevelRangeList     * m_levelRangeDBRef;
// Dialog Data
	//{{AFX_DATA(CSkillRangesDlg)
	enum { IDD = IDD_DIALOG_SKILLRANGES };
	CListBox	m_levelRangesDB;
	CString	m_rangeTitle;
	long	m_maxExp;
	long	m_minExp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkillRangesDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListRanges(int sel);
	// Generated message map functions
	//{{AFX_MSG(CSkillRangesDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONAddRange();
	afx_msg void OnBUTTONDeleteRange();
	afx_msg void OnBUTTONReplaceRange();
	afx_msg void OnSelchangeLISTLevelRangesDB();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKILLRANGESDLG_H__568C4061_34FD_11D5_B1EE_0000B4BD56DD__INCLUDED_)
