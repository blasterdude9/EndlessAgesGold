#if !defined(AFX_SKILLEDITORDLG_H__38992141_66F4_11D4_B1ED_0000B4BD56DD__INCLUDED_)
#define AFX_SKILLEDITORDLG_H__38992141_66F4_11D4_B1ED_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkillEditorDlg.h : header file
//
#include "CCharacterDefinitionsClass.h"
#include "CResourceClass.h"
/////////////////////////////////////////////////////////////////////////////
// CSkillEditorDlg dialog

class AFX_EXT_CLASS   CSkillEditorDlg : public CDialog
{
// Construction
public:
	CSkillEditorDlg(CWnd* pParent = NULL);   // standard constructor

	CharacterDefObject * charDefinitionRef;
	CResourceObjectList * resourceListRef;

// Dialog Data
	//{{AFX_DATA(CSkillEditorDlg)
	enum { IDD = IDD_DIALOG_SkillEditor };
	CListBox	m_statDependancyList;
	CListBox	m_statisticList;
	CListBox	m_skillList;
	float	m_currentValue;
	CString	m_name;
	float	m_skillCap;
	int		m_type;
	float	m_statCap;
	CString	m_statName;
	int		m_statType;
	float	m_statCurrentAmount;
	float	m_statImprovement;
	float	m_statImprovementCap;
	float	m_ImprovementOnSuccess;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkillEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InList(int sel);
	void InListStatistics(int sel);
	void InListSkillDependancies(int sel);
	// Generated message map functions
	//{{AFX_MSG(CSkillEditorDlg)
	afx_msg void OnBUTTONAbilityEditor();
	afx_msg void OnBUTTONAddSkill();
	afx_msg void OnBUTTONDeleteSkill();
	afx_msg void OnBUTTONReplaceValues();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLISTSkillList();
	afx_msg void OnBUTTONAddStatistic();
	afx_msg void OnBUTTONDeleteStatistic();
	afx_msg void OnBUTTONStatReplaceValues();
	afx_msg void OnSelchangeLISTStatistics();
	afx_msg void OnBUTTONAddStatDependancy();
	afx_msg void OnBUTTONRemoveStatDependancy();
	afx_msg void OnSelchangeLISTSTATDependancyList();
	afx_msg void OnBUTTONReplaceDependancyValues();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKILLEDITORDLG_H__38992141_66F4_11D4_B1ED_0000B4BD56DD__INCLUDED_)
