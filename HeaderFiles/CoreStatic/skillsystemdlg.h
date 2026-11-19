#if !defined(AFX_SKILLSYSTEMDLG_H__CDD3601C_F24B_41CD_BC0C_269890E30ABB__INCLUDED_)
#define AFX_SKILLSYSTEMDLG_H__CDD3601C_F24B_41CD_BC0C_269890E30ABB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SkillSystemDlg.h : header file
//
#include "CSkillClass.h"
#include "CMovementModel.h"
#include "CStatClass.h"
#include "afxwin.h"
#include "SqlConnectionDlg.h"
/////////////////////////////////////////////////////////////////////////////
// CSkillSystemDlg dialog

class AFX_EXT_CLASS   CSkillSystemDlg : public CDialog
{
// Construction
public:
	CSkillSystemDlg(CWnd* pParent = NULL);   // standard constructor


	CSkillObjectList        * m_skillDatabaseRef;
	CMovementObjList        * m_edbDatabaseRef;
	CStatObjList            * m_statDatabaseRef;
	void                    * m_gStatusMem;
// Dialog Data
	//{{AFX_DATA(CSkillSystemDlg)
	enum { IDD = IDD_DIALOG_SkillSystemDlg };
	CComboBox	m_globalInventory;
	CComboBox	m_statRefList;
	CComboBox	m_edbList;
	CComboBox	m_basicAbilityFunction;
	CComboBox	m_cmboAbilityFunction;
	CButton		m_basicAbilitySkillEnable;
	CListBox	m_abilitiesList;
	CListBox	m_abilityRequiredItemsList;
	CListBox	m_abilityResultItemsList;
	CListBox	m_racialAdvantageList;
	CListBox	m_abilityRequiredStatsList;
	CListBox	m_skillLevelsList;
	CListBox	m_skillsList;
	float	m_abilityMasteredExp;
	int		m_abilityMinimumLevel;
	int		m_abilityMiscInt;
	CString	m_abilityName;
	float	m_raceAdvFactor;
	float	m_basicAbilityMastered;
	float	m_endExpRange;
	float	m_expStart;
	float	m_gainIncrement;
	CString	m_levelTitle;
	CString	m_skillName;
	float	m_expStartRange;
	int		m_startLevel;
	float	m_statAmount;
	int		m_typeRef;
	int		m_menuID;
	float	m_gainEvenIfFailedValue;
	float	m_maxMultiplier;
	int		m_miscInt2;
	int		m_miscInt3;
	int		m_effectBind;
	int		m_miscAmount;
	//}}AFX_DATA

	CButton	m_filterLevelEval;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSkillSystemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListSkills(int sel);
	void InListRacialAdvantages(int sel);
	void InListLevels(int sel);
	void InListAbilities(int sel);
	void InListStatReq(int sel);
	void InListRequiredItems(int sel);
	void InListResultItems(int sel);
	// Generated message map functions
	//{{AFX_MSG(CSkillSystemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLISTSkillsDB();
	afx_msg void OnBUTTONAddSkill();
	afx_msg void OnBUTTONDeleteSkill();
	afx_msg void OnBUTTONReplaceSkill();
	afx_msg void OnBUTTONAddRacialAdv();
	afx_msg void OnBUTTONDeleteRacialAdv();
	afx_msg void OnSelchangeLISTRacialAdvDB();
	afx_msg void OnBUTTONAddLevel();
	afx_msg void OnBUTTONReplaceLevel();
	afx_msg void OnBUTTONDeleteLevel();
	afx_msg void OnSelchangeLISTSkillLevelsDB();
	afx_msg void OnBUTTONADDAbility();
	afx_msg void OnBUTTONDeleteAbility();
	afx_msg void OnBUTTONReplaceAbility();
	afx_msg void OnSelchangeLISTAbilityDB();
	afx_msg void OnBUTTONAddStatReq();
	afx_msg void OnBUTTONDeleteStatReq();
	afx_msg void OnBUTTONAddRequiredItem();
	afx_msg void OnBUTTONAddResultItem();
	afx_msg void OnBUTTONDeleteRequiredItem();
	afx_msg void OnBUTTONDeleteResultItem();
	afx_msg void OnBUTTONLoadSys();
	afx_msg void OnBUTTONSaveSkillSys();
	afx_msg void OnSelchangeLISTrequiredStats();
	afx_msg void OnBUTTONReplaceStatReq();
	afx_msg void OnBUTTONInsertLevel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	// This adds to your effective roll.
	int m_proficiency;
	// This value decreases the starting point of the proficiency range to reduce low rolls
	int m_perfective;
	CButton m_enableInternalTimer;
	afx_msg void OnBnClickedButtonExporttosql();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SKILLSYSTEMDLG_H__CDD3601C_F24B_41CD_BC0C_269890E30ABB__INCLUDED_)
