#if !defined(AFX_ABILITYEDITORDLG_H__5E0CBC41_6729_11D4_B1ED_0000B4BD56DD__INCLUDED_)
#define AFX_ABILITYEDITORDLG_H__5E0CBC41_6729_11D4_B1ED_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AbilityEditorDlg.h : header file
//
#include "CCharacterDefinitionsClass.h"
#include "CResourceClass.h"
/////////////////////////////////////////////////////////////////////////////
// CAbilityEditorDlg dialog

class AFX_EXT_CLASS   CAbilityEditorDlg : public CDialog
{
// Construction
public:
	CAbilityEditorDlg(CWnd* pParent = NULL);   // standard constructor
    int m_refSkillType;
	CharacterDefObject * charDefinitionRef;
	CResourceObjectList * resourceListRef;
// Dialog Data
	//{{AFX_DATA(CAbilityEditorDlg)
	enum { IDD = IDD_DIALOG_AbilityEditor };
	CListBox	m_listCosts;
	CListBox	m_abilityList;
	float	m_abilityGainBasis;
	float	m_abilityMasterLimit;
	float	m_abilityMinRequiredSkill;
	int		m_abilityMiscInt;
	int		m_abilityType;
	CString	m_abilityName;
	int		m_costQuanity;
	int		m_costResourceType;
	int		m_attributeMiscInt;
	CString	m_successAction;
	CString	m_comment;
	int		m_abilityDuration;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAbilityEditorDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListAbilities(int sel);
	void InListAbilityCost(CAbilityObject * ability,int sel);
	CString ConvertLongToString(int number);
	// Generated message map functions
	//{{AFX_MSG(CAbilityEditorDlg)
	afx_msg void OnBUTTONADDAbility();
	afx_msg void OnBUTTONDELETEability();
	afx_msg void OnBUTTONReplaceValues();
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeLISTAbilities();
	afx_msg void OnBUTTONCostReplaceValues();
	afx_msg void OnBUTTONaddCost();
	afx_msg void OnBUTTONDeleteCost();
	afx_msg void OnSelchangeLISTCostList();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ABILITYEDITORDLG_H__5E0CBC41_6729_11D4_B1ED_0000B4BD56DD__INCLUDED_)
