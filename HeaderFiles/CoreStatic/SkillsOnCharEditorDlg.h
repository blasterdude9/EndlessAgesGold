#if !defined(AFX_SKILLSONCHAREDITORDLG_H__BBC8F1A1_38A7_11D5_B1EE_0000B4BD56DD__INCLUDED_)
#define AFX_SKILLSONCHAREDITORDLG_H__BBC8F1A1_38A7_11D5_B1EE_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkillsOnCharEditorDlg.h : header file
//
#include "CStatClass.h"
#include "CSkillClass.h"
/////////////////////////////////////////////////////////////////////////////
// CSkillsOnCharEditorDlg dialog

class AFX_EXT_CLASS   CSkillsOnCharEditorDlg : public CDialog
{
// Construction
public:
	CSkillsOnCharEditorDlg(CWnd* pParent = NULL);   // standard constructor


	CStatObjList * m_statisticDBREF;
	CStatObjList * m_statisticsOnChar;
	CSkillObjectList * m_skillDBRef;
	CSkillObjectList * m_onCharSkills;
// Dialog Data
	//{{AFX_DATA(CSkillsOnCharEditorDlg)
	enum { IDD = IDD_DIALOG_StartSkillEditor };
	CListBox	m_statsOnChar;
	CListBox	m_statisticsDB;
	CListBox	m_skillsDB;
	CListBox	m_currentSkills;
	long	m_currentExp;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkillsOnCharEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListCharacterSkills(int sel);
	void InListCharacterStats(int sel);
	// Generated message map functions
	//{{AFX_MSG(CSkillsOnCharEditorDlg)
	afx_msg void OnBUTTONAddSkill();
	afx_msg void OnBUTTONDeleteSkill();
	afx_msg void OnBUTTONReplaceSkillValues();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLISTCurrentSkills();
	afx_msg void OnBUTTONAddStat();
	afx_msg void OnBUTTONDeleteStat();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKILLSONCHAREDITORDLG_H__BBC8F1A1_38A7_11D5_B1EE_0000B4BD56DD__INCLUDED_)
