#if !defined(AFX_AISETTINGSDLG_H__E55ABD61_940A_11D4_B1ED_0000B4BD56DD__INCLUDED_)
#define AFX_AISETTINGSDLG_H__E55ABD61_940A_11D4_B1ED_0000B4BD56DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AiSettingsDlg.h : header file
//
#include "AIClass.h"
#include "CAIScriptClass.h"
#include "CMovementModel.h"
#include "afxwin.h"
#include <afxcmn.h>
/////////////////////////////////////////////////////////////////////////////
// CAiSettingsDlg dialog

class AFX_EXT_CLASS   CAiSettingsDlg : public CDialog
{
// Construction
public:
	CAiSettingsDlg(CWnd* pParent = NULL);   // standard constructor
    CAIScriptObjList	*m_refList;
	int					m_finalAiScriptAssign;
	BOOL				m_enableFeelerSys;
	BOOL				enableExponentTurning;
	CAIObj				*m_aiPtrRef;
	CMovementObjList	*m_edbRef;
	void				*m_gStatusMem;

// Dialog Data
	//{{AFX_DATA(CAiSettingsDlg)
	enum { IDD = IDD_DIALOG_AISettings };
	CButton	m_checkArmed;
	CComboBox	m_dimCfgList;
	CComboBox	m_globalInventory;
	CListBox	m_inventoryDB;
	CComboBox	m_entityDB;
	CButton	m_radiusSysIgnoreElevation;
	CButton	m_radiusSysAttackOnExtents;
	CButton	m_enableRadiusSystem;
	CButton	m_enableLootGen;
	CButton	m_checkAbilityToJump;
	CButton	m_checkEnableVision;
	CSliderCtrl	m_sliderAccuracy;
	CButton	m_enableExponentTurning;
	CButton	m_checkFeelerSysEnable;
	CComboBox	m_initialScriptIndex;
	float	m_floorTolerance;
	float	m_sensedepth;
	float	m_senseHieght;
	float	m_senseRange;
	int		m_cycleTime;
	float	m_senseWidth;
	float	m_wallTolerance;
	CString	m_cfgName;
	float	m_safeDistance;
	int		m_accuracyPercentage;
	int		m_percentageAttack;
	float	m_travelDesiredDist;
	int		m_team;
	float	m_initialCloseEnoughtDist;
	long	m_distanceQuerryDuration;
	float	m_eyeLevel;
	float	m_prefferedDistanceAboveTarget;
	long	m_betweenAttackTimeMax;
	long	m_betweenAttackTimeMin;
	long	m_attackDurationMax;
	long	m_attackDurationMin;
	float	m_attackDistanceMax;
	float	m_attackDistanceMin;
	float	m_followDistanceMax;
	float	m_followDistanceMin;
	long	m_lootGenMaxAmount;
	long	m_lootGenMinAmount;
	int		m_persistenceDuration;
	float	m_sightRange;
	float	m_radiusSysRange;
	CString	m_displayedName;
	int		m_dimCfgIndex;
	int		m_percItemDrop;
	float	m_expLevel;
	int		m_energyOverride;
	int		m_worldCollisionOverride;
	int		m_agressive;
	int		m_agressiveRange;
	int		m_strikeShootRange;
	CString	m_phyOverrideStatus;
	//Script Info
	CString m_scriptName;
	CString m_scriptDescription;
	CString m_scriptEmote;
	int		m_miscInt1;
	int		m_miscInt2;
	int		m_miscInt3;
	int		m_useTimer;
	CComboBox m_scriptList;
	CComboBox m_scriptAction;
	CComboBox m_scriptOccurance;
	CListBox m_scriptObjects;
	CString	m_staticMiscInt;
	CString	m_staticMiscInt2;
	CString	m_staticMiscInt3;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAiSettingsDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
    void InListGlobalInventory(int sel);
	void InListEntityDB(int sel);
	void InListInventoryCfgDB(int sel);
	void InListDimCfgs(int sel);
	void InListScriptObjects(int sel);
	void TextInterperate(int sel);
	CString ConvToString(int number);
	// Generated message map functions
	//{{AFX_MSG(CAiSettingsDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeCOMBOInitialScriptIndex();
	afx_msg void OnCHECKfeelerSysEnable();
	virtual void OnOK();
	afx_msg void OnCHECKEnableExponentTuring();
	afx_msg void OnCHECKEnableVisionSystem();
	afx_msg void OnCHECKAbilityToJump();
	afx_msg void OnCHECKEnableRadisSys();
	afx_msg void OnSelchangeCOMBOEntityDB();
	afx_msg void OnBUTTONAddInventoryObject();
	afx_msg void OnBUTTONDeleteInvenObj();
	afx_msg void OnBUTTONGetCfg();
	afx_msg void OnCHECKcheckArmed();
	afx_msg void OnSelchangeLISTInventory();
	afx_msg void OnDeltaposSPINDimChanger(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCOMBOdimList();
	afx_msg void OnBUTTONPhyOverride();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_steeringModel;
	CButton m_treeLock;
	afx_msg void OnBnClickedButtonAiscriptAddtolist();
	afx_msg void OnBnClickedButtonAiscriptRemovefromlist();
	afx_msg void OnBnClickedButtonAiscriptReplaceinlist();
	afx_msg void OnLbnSelchangeListAiscriptScriptlist();
	afx_msg void OnCbnSelchangeComboAiscriptAction();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AISETTINGSDLG_H__E55ABD61_940A_11D4_B1ED_0000B4BD56DD__INCLUDED_)
