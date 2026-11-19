#pragma once
// SpawnGenDlg.h : header file
//
#include "CServerItemGen.h"
#include "CWorldAvailableClass.h"
#include "CMovementModel.h"
#include "afxcmn.h"
#include "CSkillClass.h"
#include "AIClass.h"
/////////////////////////////////////////////////////////////////////////////
// CSpawnGenDlg dialog

class AFX_EXT_CLASS   CSpawnGenDlg : public CDialog
{
// Construction
public:
	CSpawnGenDlg(CWnd* pParent = NULL);   // standard constructor

    float Y_override;
	CServerItemGenObjList   * m_serverItemGenRef;
	CWorldAvailableObjList  * m_worldsDB;
	CMovementObjList        * m_edbRef;
    CAIObjList              * m_aiListRef;
	CMovementObj            * m_focusEntity;
	CSkillObjectList        * m_skillDatabaseRef;
	BOOL                      m_focusEntityExists;
	int                       m_currentSelTreeID;
	void                    * m_gstatusRef;

// Dialog Data
	//{{AFX_DATA(CSpawnGenDlg)
	enum { IDD = IDD_DIALOG_SpawnGenDlg };
	CButton	m_checkArmed;
	CComboBox	m_itemTypeCombo;
	CComboBox	m_comboDimCfgList;
	CListBox	m_contentsList;
	CComboBox	m_worldSelection;
	CComboBox	m_dropValueGlids;
	CComboBox	m_dbRef;
	CListBox	m_generators;
	float	m_posX;
	float	m_posY;
	float	m_posZ;
	long	m_respawnDuration;
	int		m_worldID;
	int		m_curAnimation;
	CString	m_generatorName;
	int		m_spawnGenID;
	int		m_keyID;
	int		m_curCfgNum;
	CString	m_displayedName;
	int		m_rotationAdjust;
	CString m_entityMappingOverride;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpawnGenDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void InListTreeGenerators(int sel);
    void InListGenerators(int sel);
	void InListItemContents(int sel);
	void InListConfigurations(CServerItemGenObj * spnGenPtr,int sel);
	CString ConvToString(int number);
	void SetAllItemData(CItemObj * itemData,int glid);
	HTREEITEM InsertRoot(CString name,int idIndex,BOOL selected);
	HTREEITEM AddTreeItem(HTREEITEM root,CString name,int idIndex,BOOL selected);
	// Generated message map functions
	//{{AFX_MSG(CSpawnGenDlg)
	afx_msg void OnBUTTONAddNew();
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnBUTTONEdit();
	afx_msg void OnButtonDelete();
	afx_msg void OnBUTTONGetCurData();
	afx_msg void OnSelchangeLISTGeneratorList();
	afx_msg void OnButtonLoad();
	afx_msg void OnButtonSave();
	afx_msg void OnBUTTONAddItemToContents();
	afx_msg void OnBUTTONDeleteItemFromContents();
	afx_msg void OnSelchangeLISTContentsList();
	afx_msg void OnDeltaposSPINDimAdjust(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeCOMBODimCfgList();
	afx_msg void OnCHECKarmed();
	afx_msg void OnBUTTONTypeCfg();
	afx_msg void OnBUTTONSaveUnit();
	afx_msg void OnBUTTONLoadUnit();
	afx_msg void OnBUTTONRebuildDim();
	afx_msg void OnBUTTONIntegrityCheck();
	afx_msg void OnBUTTONrebuild();
	afx_msg void OnBUTTONsearch();
	afx_msg void OnTvnSelchangedTreegeneratorslist(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnCbnSelchangeComboWorldselection();
	afx_msg void OnBnClickedButtonExporttosql();
	afx_msg void OnBnClickedButtonImportfromsql();
	afx_msg void OnBnClickedButtonLoadfromrne();
	afx_msg void OnBnClickedButtonLoaduploadsqltofolder();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	int m_randomAnimation;
	int m_mountOption;
	int m_randomGlobalPerc;
	int m_randomExpEffect;
	CTreeCtrl m_generatorTree;
	int m_organizedGroupID;
	CString m_noResponsMsg;
	float m_sightRangeOverride;
	float m_interactRadius;
	int m_autoTriggerType;
	int m_worldChannel;
};
