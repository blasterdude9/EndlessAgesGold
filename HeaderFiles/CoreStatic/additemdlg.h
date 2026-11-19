
#pragma once
// AddItemDlg.h : header file
//
#include "CItemClass.h"
#include "CArmedInventoryClass.h"
#include "CMovementModel.h"
#include "CElementsClass.h"


/////////////////////////////////////////////////////////////////////////////
// CAddItemDlg dialog

class AFX_EXT_CLASS   CAddItemDlg : public CDialog
{
// Construction
public:
	CAddItemDlg(CWnd* pParent = NULL);   // standard constructor

	CItemObj * m_itemRef;
	CArmedInventoryList * m_armedInventoryListRef;
	CMovementObjList    * m_enitityDB;
	CElementObjList     * m_currentElements;
	CSkillObjectList    * m_skillDatabaseRef;
// Dialog Data
	//{{AFX_DATA(CAddItemDlg)
	enum { IDD = IDD_DIALOG_ADDITEM };
	CListBox	m_expIconList;
	CButton	m_stackable;
	CButton	m_destroyWhenUsed;
	CButton	m_nonTransferable;
	CListBox	m_elementDefensiveList;
	CComboBox	m_elements;
	CListBox	m_statBonusList;
	CButton	m_nonDrop;
	CComboBox	m_useType;
	CComboBox m_bonusType;
	CComboBox m_abilityType;
	CButton	m_permanent;
	CComboBox	m_skillsAvailable;
	CButton	m_checkDisarmable;
	CButton	m_checkUnarmedAttack;
	CButton m_checkEmblem0;
	CButton m_checkEmblem1;
	CButton m_checkEmblem2;
	CButton m_checkEmblem3;
	CButton m_checkEmblem4;
	CString	m_itemName;
	int		m_marketCost;
	int		m_requiredDex;
	int		m_requiredKnowledge;
	int		m_requiredStr;
	int		m_uniqueLocationID;
	int		m_glID;
	int		m_animapreference;
	int		m_animaAccessDefault;
	int		m_minimumSkillRequired;
	int		m_masteredSkillLevel;
	int		m_useValue;
	int		m_armourRating;
	int		m_bonusStatID;
	int		m_statBonusAmount;
	int		m_currentCharges;
	int		m_chargeType;
	int		m_maxCharges;
	int		m_keyID;
	int		m_elementDefenseValue;
	int		m_requiredSkillLevel;
	int		m_iconDBIndex;
	int		m_iconMiscInt;
	int		m_iconMiscInt2;
	float	m_iconPosXonscreen;
	float	m_iconPosYonscreen;
	int		m_defenseSpecificID;
	//}}AFX_DATA
	int     m_reloadDuration;
	int		m_bonusDuration;
	float	m_damageMultiplier;
	int		m_explosionRef;
	int		m_shotsPerFire;
	int		m_ammoRegenRate;
	int		m_ammoSecTick;
	int     m_itemHP;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAddItemDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListStatBonuses(int sel);
	CString ConvToString(int number);
	void InListElementStatus(int sel);
	void InListIconExpandable(int sel);
	// Generated message map functions
	//{{AFX_MSG(CAddItemDlg)
	afx_msg void OnBUTTONAddAttachmentType();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCHECKdisarmable();
	afx_msg void OnCHECKpermanent();
	afx_msg void OnCHECKunarmedAttack();
	afx_msg void OnBUTTONDeleteStatBonus();
	afx_msg void OnBUTTONAddStatBonus();
	afx_msg void OnSelchangeLISTStatBonusList();
	afx_msg void OnBUTTONAddElementDefense();
	afx_msg void OnBUTTONDeleteElementDefense();
	afx_msg void OnCHECKstackable();
	afx_msg void OnBUTTONAddIconExpObj();
	afx_msg void OnBUTTONDeleteIconExpObj();
	afx_msg void OnSelchangeLISTIconExpObjList();
	afx_msg void OnCHECKemblem0();
	afx_msg void OnCHECKemblem1();
	afx_msg void OnCHECKemblem2();
	afx_msg void OnCHECKemblem3();
	afx_msg void OnCHECKemblem4();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_visualRef;
	afx_msg void OnBnClickedOk();
	int m_dimCfgIndex;
	int m_dimCfgChangeTo;
	int m_dimCfgChangeBack;
	afx_msg void OnBnClickedCheck4();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
