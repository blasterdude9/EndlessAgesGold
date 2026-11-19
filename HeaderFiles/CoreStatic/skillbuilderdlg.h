

#if !defined(AFX_SKILLBUILDERDLG_H__1115CD81_34EE_11D5_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_SKILLBUILDERDLG_H__1115CD81_34EE_11D5_B1EE_0000B4BD56DD__INCLUDED_



#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkillBuilderDlg.h : header file
//
#include "CSkillClass.h"
/////////////////////////////////////////////////////////////////////////////
// CSkillBuilderDlg dialog

class AFX_EXT_CLASS   CSkillBuilderDlg : public CDialog
{
// Construction
public:
	CSkillBuilderDlg(CWnd* pParent = NULL);   // standard constructor

    CSkillObjectList * m_skillDatabaseRef;

// Dialog Data
	//{{AFX_DATA(CSkillBuilderDlg)
	enum { IDD = IDD_DIALOG_SKILLBUILDER };
	CListBox	m_skillsAvailable;
	CString	m_skillName;
	int		m_gainIncrement;
	int		m_type;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkillBuilderDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InListSkills(int sel);
	void LoadSkillDB(CString fileName);
	// Generated message map functions
	//{{AFX_MSG(CSkillBuilderDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONAbilities();
	afx_msg void OnBUTTONAddSkill();
	afx_msg void OnButtonDelete();
	afx_msg void OnBUTTONLevelRanges();
	afx_msg void OnSelchangeLISTSkillsAvailable();
	afx_msg void OnBUTTONSaveDB();
	afx_msg void OnBUTTONLoadDB();
	afx_msg void OnBUTTONReplaceSkill();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKILLBUILDERDLG_H__1115CD81_34EE_11D5_B1EE_0000B4BD56DD__INCLUDED_)
