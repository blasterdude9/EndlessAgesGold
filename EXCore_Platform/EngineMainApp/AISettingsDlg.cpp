// AiSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "AiSettingsDlg.h"
#include "MotionSettingsDlg.h"
#include "globalstatusvariables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAiSettingsDlg dialog


CAiSettingsDlg::CAiSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAiSettingsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAiSettingsDlg)
	m_floorTolerance = 0.0f;
	m_sensedepth = 0.0f;
	m_senseHieght = 0.0f;
	m_senseRange = 0.0f;
	m_cycleTime = 0;
	m_senseWidth = 0.0f;
	m_wallTolerance = 0.0f;
	m_cfgName = _T("");
	m_safeDistance = 0.0f;
	m_accuracyPercentage = 0;
	m_percentageAttack = 0;
	m_travelDesiredDist = 0.0f;
	m_team = 0;
	m_initialCloseEnoughtDist = 0.0f;
	m_distanceQuerryDuration = 0;
	m_eyeLevel = 0.0f;
	m_prefferedDistanceAboveTarget = 0.0f;
	m_betweenAttackTimeMax = 0;
	m_betweenAttackTimeMin = 0;
	m_attackDurationMax = 0;
	m_attackDurationMin = 0;
	m_attackDistanceMax = 0.0f;
	m_attackDistanceMin = 0.0f;
	m_followDistanceMax = 0.0f;
	m_followDistanceMin = 0.0f;
	m_lootGenMaxAmount = 0;
	m_lootGenMinAmount = 0;
	m_persistenceDuration = 0;
	m_sightRange = 0.0f;
	m_radiusSysRange = 0.0f;
	m_displayedName = _T("");
	m_dimCfgIndex = 0;
	m_percItemDrop = 0;
	m_expLevel = 0.0f;
	m_energyOverride = 0;
	m_worldCollisionOverride = 0;
	m_agressive = 0;
	m_agressiveRange = 0;
	m_scriptName = _T("");
	m_scriptDescription = _T("");
	m_scriptEmote = _T("");
	m_miscInt1 = 0;
	m_miscInt2 = 0;
	m_miscInt3 = 0;
	m_useTimer = 0;
	m_staticMiscInt = _T("");
	m_staticMiscInt2 = _T("");
	m_staticMiscInt3 = _T("");
	//}}AFX_DATA_INIT
}


void CAiSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAiSettingsDlg)
	DDX_Control(pDX, IDC_CHECK_checkArmed, m_checkArmed);
	DDX_Control(pDX, IDC_COMBO_dimList, m_dimCfgList);
	DDX_Control(pDX, IDC_COMBO_GlobalInventory, m_globalInventory);
	DDX_Control(pDX, IDC_LIST_Inventory, m_inventoryDB);
	DDX_Control(pDX, IDC_COMBO_EntityDB, m_entityDB);
	DDX_Control(pDX, IDC_CHECK_radiusIgnoreElevation, m_radiusSysIgnoreElevation);
	DDX_Control(pDX, IDC_CHECK_radiusAttackOnExtents, m_radiusSysAttackOnExtents);
	DDX_Control(pDX, IDC_CHECK_EnableRadisSys, m_enableRadiusSystem);
	DDX_Control(pDX, IDC_CHECK_EnableLootGen, m_enableLootGen);
	DDX_Control(pDX, IDC_CHECK_AbilityToJump, m_checkAbilityToJump);
	DDX_Control(pDX, IDC_CHECK_EnableVisionSystem, m_checkEnableVision);
	DDX_Control(pDX, IDC_SLIDER_Accuracy, m_sliderAccuracy);
	DDX_Control(pDX, IDC_CHECK_EnableExponentTuring, m_enableExponentTurning);
	DDX_Control(pDX, IDC_CHECK_feelerSysEnable, m_checkFeelerSysEnable);
	DDX_Control(pDX, IDC_COMBO_InitialScriptIndex, m_initialScriptIndex);
	DDX_Text(pDX, IDC_EDIT_FloorTolerance, m_floorTolerance);
	DDX_Text(pDX, IDC_EDIT_SenseDepth, m_sensedepth);
	DDX_Text(pDX, IDC_EDIT_SenseHieght, m_senseHieght);
	DDX_Text(pDX, IDC_EDIT_SenseRange, m_senseRange);
	DDX_Text(pDX, IDC_EDIT_SenseTimeCycle, m_cycleTime);
	DDX_Text(pDX, IDC_EDIT_SenseWidth, m_senseWidth);
	DDX_Text(pDX, IDC_EDIT_WallTolerance, m_wallTolerance);
	DDX_Text(pDX, IDC_EDIT_cfgName, m_cfgName);
	DDX_Text(pDX, IDC_EDIT_SafeDistance, m_safeDistance);
	DDX_Text(pDX, IDC_EDIT_accuracyPercentage, m_accuracyPercentage);
	DDX_Text(pDX, IDC_EDIT_percentageAttack, m_percentageAttack);
	DDX_Text(pDX, IDC_EDIT_travelDesiredDist, m_travelDesiredDist);
	DDX_Text(pDX, IDC_EDIT_team, m_team);
	DDX_Text(pDX, IDC_EDIT_initialCloseEnoughDist, m_initialCloseEnoughtDist);
	DDX_Text(pDX, IDC_EDIT_DistanceQuerryDuration, m_distanceQuerryDuration);
	DDX_Text(pDX, IDC_EDIT_eyeLevel, m_eyeLevel);
	DDX_Text(pDX, IDC_EDIT_preferedDistanceAboveTarget, m_prefferedDistanceAboveTarget);
	DDX_Text(pDX, IDC_EDIT_BetweenAttackTimeMax, m_betweenAttackTimeMax);
	DDX_Text(pDX, IDC_EDIT_BetweenAttackTimeMin, m_betweenAttackTimeMin);
	DDX_Text(pDX, IDC_EDIT_AttackDurationMax, m_attackDurationMax);
	DDX_Text(pDX, IDC_EDIT_AttackDurationMin, m_attackDurationMin);
	DDX_Text(pDX, IDC_EDIT_AttackDistanceMax, m_attackDistanceMax);
	DDX_Text(pDX, IDC_EDIT_AttackDistanceMin, m_attackDistanceMin);
	DDX_Text(pDX, IDC_EDIT_followDistanceMax, m_followDistanceMax);
	DDX_Text(pDX, IDC_EDIT_followDistanceMin, m_followDistanceMin);
	DDX_Text(pDX, IDC_EDIT_lootGenMaxAmount, m_lootGenMaxAmount);
	DDX_Text(pDX, IDC_EDIT_lootGenMinAmount, m_lootGenMinAmount);
	DDX_Text(pDX, IDC_EDIT_persistenceDuration, m_persistenceDuration);
	DDX_Text(pDX, IDC_EDIT_sightRange, m_sightRange);
	DDX_Text(pDX, IDC_EDIT_radiusRange, m_radiusSysRange);
	DDX_Text(pDX, IDC_EDIT_DisplayedName, m_displayedName);
	DDX_Text(pDX, IDC_EDIT_DimCfgIndex, m_dimCfgIndex);
	DDX_Text(pDX, IDC_EDIT_PercItmDrop, m_percItemDrop);
	DDX_Text(pDX, IDC_EDIT_expLevel, m_expLevel);
	DDX_Text(pDX, IDC_EDIT_EnergyOvverride, m_energyOverride);
	DDX_Text(pDX, IDC_EDIT_worldCollisionOverride, m_worldCollisionOverride);
	DDX_Text(pDX, IDC_EDIT_agressive, m_agressive);
	DDX_Text(pDX, IDC_EDIT_agressiveRange, m_agressiveRange);
    DDX_Text(pDX, IDC_EDIT_strikeLootRange, m_strikeShootRange);
	DDX_Text(pDX, IDC_EDIT_AISCRIPT_NAME, m_scriptName);
	DDX_Text(pDX, IDC_EDIT_AISCRIPT_DESCRIPTION, m_scriptDescription);
	DDX_Text(pDX, IDC_EDIT_AISCRIPT_EMOTEMESSAGE, m_scriptEmote);
	DDX_Text(pDX, IDC_EDIT_AISCRIPT_MISCINT1, m_miscInt1);
	DDX_Text(pDX, IDC_EDIT_AISCRIPT_MISCINT2, m_miscInt2);
	DDX_Text(pDX, IDC_EDIT_AISCRIPT_MISCINT3, m_miscInt3);
	DDX_Text(pDX, IDC_EDIT_AISCRIPT_USETIMER, m_useTimer);
	DDX_Control(pDX, IDC_COMBO_AISCRIPT_LISTTYPE, m_scriptList);
	DDX_Control(pDX, IDC_COMBO_AISCRIPT_ACTION, m_scriptAction);
	DDX_Control(pDX, IDC_COMBO_AISCRIPT_OCCURENCE, m_scriptOccurance);
	DDX_Text(pDX, IDC_STATIC_AIMISC1, m_staticMiscInt);
	DDX_Text(pDX, IDC_STATIC_AIMISC2, m_staticMiscInt2);
	DDX_Text(pDX, IDC_STATIC_AIMISC3, m_staticMiscInt3);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_steeringModel, m_steeringModel);
	DDX_Control(pDX, IDC_CHECK_aiTreeLock, m_treeLock);
    DDX_Text(pDX, IDC_STATIC_phyOverrideStatus, m_phyOverrideStatus);
	DDX_Control(pDX, IDC_LIST_AISCRIPT_SCRIPTLIST, m_scriptObjects);	
}

BEGIN_MESSAGE_MAP(CAiSettingsDlg, CDialog)
	//{{AFX_MSG_MAP(CAiSettingsDlg)
	ON_CBN_SELCHANGE(IDC_COMBO_InitialScriptIndex, OnSelchangeCOMBOInitialScriptIndex)
	ON_BN_CLICKED(IDC_CHECK_feelerSysEnable, OnCHECKfeelerSysEnable)
	ON_BN_CLICKED(IDC_CHECK_EnableExponentTuring, OnCHECKEnableExponentTuring)
	ON_BN_CLICKED(IDC_CHECK_EnableVisionSystem, OnCHECKEnableVisionSystem)
	ON_BN_CLICKED(IDC_CHECK_AbilityToJump, OnCHECKAbilityToJump)
	ON_BN_CLICKED(IDC_CHECK_EnableRadisSys, OnCHECKEnableRadisSys)
	ON_CBN_SELCHANGE(IDC_COMBO_EntityDB, OnSelchangeCOMBOEntityDB)
	ON_BN_CLICKED(IDC_BUTTON_AddInventoryObject, OnBUTTONAddInventoryObject)
	ON_BN_CLICKED(IDC_BUTTON_DeleteInvenObj, OnBUTTONDeleteInvenObj)
	ON_BN_CLICKED(IDC_BUTTON_GetCfg, OnBUTTONGetCfg)
	ON_BN_CLICKED(IDC_CHECK_checkArmed, OnCHECKcheckArmed)
	ON_LBN_SELCHANGE(IDC_LIST_Inventory, OnSelchangeLISTInventory)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DimChanger, OnDeltaposSPINDimChanger)
	ON_CBN_SELCHANGE(IDC_COMBO_dimList, OnSelchangeCOMBOdimList)
	ON_BN_CLICKED(IDC_BUTTON_PhyOverride, OnBUTTONPhyOverride)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_AISCRIPT_ADDTOLIST, &CAiSettingsDlg::OnBnClickedButtonAiscriptAddtolist)
	ON_BN_CLICKED(IDC_BUTTON_AISCRIPT_REMOVEFROMLIST, &CAiSettingsDlg::OnBnClickedButtonAiscriptRemovefromlist)
	ON_BN_CLICKED(IDC_BUTTON_AISCRIPT_REPLACEINLIST, &CAiSettingsDlg::OnBnClickedButtonAiscriptReplaceinlist)
	ON_LBN_SELCHANGE(IDC_LIST_AISCRIPT_SCRIPTLIST, &CAiSettingsDlg::OnLbnSelchangeListAiscriptScriptlist)
	ON_CBN_SELCHANGE(IDC_COMBO_AISCRIPT_ACTION, &CAiSettingsDlg::OnCbnSelchangeComboAiscriptAction)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAiSettingsDlg message handlers

BOOL CAiSettingsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);
	m_initialScriptIndex.AddString("NONE");
	for(POSITION posLoc = m_refList->GetHeadPosition();posLoc != NULL;)
	{
        CAIScriptObj * scrObj = (CAIScriptObj*)m_refList->GetNext(posLoc);
        m_initialScriptIndex.AddString(scrObj->scriptName);
	}
	m_initialScriptIndex.SetCurSel(m_finalAiScriptAssign+1);

	//m_aiPtrRef->
    m_enableFeelerSys = m_aiPtrRef->m_feelerEnable;
	if(m_enableFeelerSys)
	   m_checkFeelerSysEnable.SetCheck(1);

	enableExponentTurning = m_aiPtrRef->m_exponentTurning;
    if(enableExponentTurning)
	   m_enableExponentTurning.SetCheck(1);

	if(m_aiPtrRef->m_physicsModuleOverride)
		m_phyOverrideStatus = "Enabled";
	else
	    m_phyOverrideStatus = "Disabled";
	m_floorTolerance             = m_aiPtrRef->m_floorTolerance;
	m_sensedepth                 = m_aiPtrRef->m_feelerDepth;
	m_senseHieght                = m_aiPtrRef->m_feelerHieght;
	m_senseRange                 = m_aiPtrRef->m_feelerRange;
	m_cycleTime                  = m_aiPtrRef->m_feelerCycleTime;
	m_senseWidth                 = m_aiPtrRef->m_feelerWidth;
	m_wallTolerance              = m_aiPtrRef->m_wallMinimumHieght;
	m_cfgName                    = m_aiPtrRef->m_aiCfgName;
	m_safeDistance               = m_aiPtrRef->m_safeDistance;
	m_accuracyPercentage         = m_aiPtrRef->m_accuracy;
	m_percentageAttack           = m_aiPtrRef->m_percentageAttack;
	m_travelDesiredDist          = m_aiPtrRef->m_travelingDesiredDist;
	m_team                       = m_aiPtrRef->List_team;
    m_initialCloseEnoughtDist    = m_aiPtrRef->List_closeEnoughDist;

	m_distanceQuerryDuration     = m_aiPtrRef->m_distanceQuerryDuration;
	m_betweenAttackTimeMax       = m_aiPtrRef->m_attackTimeBetweenMax;
	m_betweenAttackTimeMin       = m_aiPtrRef->m_attackTimeBetweenMin;
	m_attackDistanceMax          = m_aiPtrRef->m_attackMaxDistance;
	m_attackDistanceMin          = m_aiPtrRef->m_attackMinDistance;
	m_eyeLevel                   = m_aiPtrRef->m_eyeLevel;
	m_followDistanceMax          = m_aiPtrRef->m_followMaxDistance;
	m_followDistanceMin          = m_aiPtrRef->m_followMinDistance;
	m_prefferedDistanceAboveTarget = m_aiPtrRef->m_prefernceAboveTarget;
	m_attackDurationMax          = m_aiPtrRef->m_attackDurationMax;
	m_attackDurationMin          = m_aiPtrRef->m_attackDurationMin;
	m_lootGenMaxAmount           = m_aiPtrRef->m_maxCash;
	m_lootGenMinAmount           = m_aiPtrRef->m_minCash;
	m_persistenceDuration        = m_aiPtrRef->m_durationTillTargetLost;
	m_sightRange                 = m_aiPtrRef->List_maxSightRadius;
	m_radiusSysRange             = m_aiPtrRef->m_radiusSysRange;
	m_displayedName              = m_aiPtrRef->m_nameOvveride;
	m_percItemDrop               = m_aiPtrRef->m_percentageOfDropping;
	m_expLevel                   = m_aiPtrRef->m_curExperienceLevel;
	m_energyOverride             = m_aiPtrRef->m_overriderMaxEngery;
	m_worldCollisionOverride     = m_aiPtrRef->m_collisionDBOverride;
	m_agressive                  = m_aiPtrRef->m_agressive;
	/*m_agressiveRange			 = m_aiPtrRef->m_agressiveRange;
	m_strikeShootRange           = m_aiPtrRef->m_strikeShootRange;*/
	m_agressiveRange			 = (int)m_aiPtrRef->m_agressiveRange;
	m_strikeShootRange           = (int)m_aiPtrRef->m_strikeShootRange;
	//WARNING: C4244 --> conversion from float to int, possible loss of data

    m_steeringModel.AddString("AI Standard");
	m_steeringModel.AddString("Platform Standard");

	m_steeringModel.SetCurSel(m_aiPtrRef->m_steeringModel);
	m_scriptList.SetCurSel(0);
	m_scriptAction.SetCurSel(0);
	m_scriptOccurance.SetCurSel(0);

    if(m_aiPtrRef->m_enableCashApplied)
	   m_enableLootGen.SetCheck(1);	

	m_treeLock.SetCheck(m_aiPtrRef->m_lockedOnPath);

    if(m_aiPtrRef->m_enableVision)
       m_checkEnableVision.SetCheck(1);

	if(m_aiPtrRef->m_enableJumpAbility)
       m_checkAbilityToJump.SetCheck(1);

	if(m_aiPtrRef->m_radiusSysEnabled)
	   m_enableRadiusSystem.SetCheck(1);

	if(m_aiPtrRef->m_radiusSysAttackOnExtents)
	   m_radiusSysAttackOnExtents.SetCheck(1);

	if(m_aiPtrRef->m_radiusSysIgnoreY)
	   m_radiusSysIgnoreElevation.SetCheck(1);

    m_sliderAccuracy.SetRange(0,100);
    m_sliderAccuracy.SetPos(m_aiPtrRef->m_accuracy);

	UpdateData(FALSE);
	
	InListEntityDB(0);
	InListGlobalInventory(0);
	InListInventoryCfgDB(0);
	InListDimCfgs(0);
	InListScriptObjects(-1);
	TextInterperate(0);
	
	m_entityDB.SetCurSel(m_edbRef->GetPhysicalIndexByUniqueID(m_aiPtrRef->m_entityCfgDBindexRef));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAiSettingsDlg::OnSelchangeCOMBOInitialScriptIndex() 
{
	UpdateData(TRUE);
	m_finalAiScriptAssign = m_initialScriptIndex.GetCurSel();
	m_finalAiScriptAssign--;
	UpdateData(FALSE);
}

void CAiSettingsDlg::OnCHECKfeelerSysEnable() 
{
	if(!m_enableFeelerSys)
		m_enableFeelerSys = TRUE;
	else
		m_enableFeelerSys = FALSE;
}

void CAiSettingsDlg::OnOK() 
{
	UpdateData(TRUE);
	m_aiPtrRef->m_feelerEnable               = m_enableFeelerSys;
	m_aiPtrRef->m_floorTolerance             = m_floorTolerance;
	m_aiPtrRef->m_feelerDepth                = m_sensedepth;
	m_aiPtrRef->m_feelerHieght               = m_senseHieght;
	m_aiPtrRef->m_feelerRange                = m_senseRange;
	m_aiPtrRef->m_feelerCycleTime            = m_cycleTime;
	m_aiPtrRef->m_feelerWidth                = m_senseWidth;
	m_aiPtrRef->m_wallMinimumHieght          = m_wallTolerance;
	m_aiPtrRef->m_aiCfgName                  = m_cfgName;
	m_aiPtrRef->m_safeDistance               = m_safeDistance;
	m_aiPtrRef->m_accuracy                   = m_accuracyPercentage;
	m_aiPtrRef->m_percentageAttack           = m_percentageAttack;
	m_aiPtrRef->m_travelingDesiredDist       = m_travelDesiredDist;
	m_aiPtrRef->m_exponentTurning            = enableExponentTurning;
	m_aiPtrRef->List_team                    = m_team;
	m_aiPtrRef->List_closeEnoughDist         = m_initialCloseEnoughtDist;

	m_aiPtrRef->m_distanceQuerryDuration     = m_distanceQuerryDuration;
	m_aiPtrRef->m_attackTimeBetweenMax       = m_betweenAttackTimeMax;
	m_aiPtrRef->m_attackTimeBetweenMin       = m_betweenAttackTimeMin;
	m_aiPtrRef->m_attackMaxDistance          = m_attackDistanceMax;
	m_aiPtrRef->m_attackMinDistance          = m_attackDistanceMin;
	m_aiPtrRef->m_eyeLevel                   = m_eyeLevel;
	m_aiPtrRef->m_followMaxDistance          = m_followDistanceMax;
	m_aiPtrRef->m_followMinDistance          = m_followDistanceMin;
	m_aiPtrRef->m_prefernceAboveTarget       = m_prefferedDistanceAboveTarget;
	m_aiPtrRef->m_attackDurationMax          = m_attackDurationMax;
	m_aiPtrRef->m_attackDurationMin          = m_attackDurationMin;

	m_aiPtrRef->m_maxCash                    = m_lootGenMaxAmount;
	m_aiPtrRef->m_minCash                    = m_lootGenMinAmount;
	m_aiPtrRef->m_durationTillTargetLost     = m_persistenceDuration;
	m_aiPtrRef->List_maxSightRadius          = m_sightRange;
    m_aiPtrRef->m_radiusSysRange             = m_radiusSysRange;
	m_aiPtrRef->m_nameOvveride               = m_displayedName;
	m_aiPtrRef->m_curExperienceLevel         = m_expLevel;
	m_aiPtrRef->m_overriderMaxEngery         = m_energyOverride;
	m_aiPtrRef->m_collisionDBOverride        = m_worldCollisionOverride;

	m_aiPtrRef->m_agressive                  = m_agressive;

	/*m_aiPtrRef->m_agressiveRange			 = m_agressiveRange;*/

	m_aiPtrRef->m_agressiveRange			 = (float)m_agressiveRange;
	//WARNING: C4244 --> conversion from int to float, possible loss of data

    m_aiPtrRef->m_enableCashApplied			 = m_enableLootGen.GetCheck();
	//m_aiPtrRef->m_accuracy					 = m_sliderAccuracy.GetPos();

    m_aiPtrRef->m_radiusSysEnabled           = m_enableRadiusSystem.GetCheck();
	m_aiPtrRef->m_radiusSysAttackOnExtents   = m_radiusSysAttackOnExtents.GetCheck();
	m_aiPtrRef->m_radiusSysIgnoreY           = m_radiusSysIgnoreElevation.GetCheck();
	m_aiPtrRef->m_lockedOnPath               = m_treeLock.GetCheck();
    m_aiPtrRef->m_percentageOfDropping       = m_percItemDrop;

	/*m_aiPtrRef->m_strikeShootRange			 = m_strikeShootRange;*/
	
	m_aiPtrRef->m_strikeShootRange			 = (float)m_strikeShootRange;
	//WARNING: C4244 --> conversion from int to float, possible loss of data


	m_aiPtrRef->m_steeringModel = m_steeringModel.GetCurSel();

	UpdateData(FALSE);
	CDialog::OnOK();
}

void CAiSettingsDlg::OnCHECKEnableExponentTuring() 
{
	if(enableExponentTurning)
		enableExponentTurning = FALSE;
	else
		enableExponentTurning = TRUE;
}

void CAiSettingsDlg::OnCHECKEnableVisionSystem() 
{
	if(m_aiPtrRef->m_enableVision)
       m_aiPtrRef->m_enableVision = FALSE;
	else
	   m_aiPtrRef->m_enableVision = TRUE;
}

void CAiSettingsDlg::OnCHECKAbilityToJump() 
{
	if(m_aiPtrRef->m_enableJumpAbility)
       m_aiPtrRef->m_enableJumpAbility = FALSE;
	else
	   m_aiPtrRef->m_enableJumpAbility = TRUE;
}

//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
void CAiSettingsDlg::InListGlobalInventory(int sel)
{
	GlobalStatusVariables * gstatus = (GlobalStatusVariables *)m_gStatusMem;
	m_globalInventory.ResetContent();
	for(unsigned int cLoop=0;cLoop<gstatus->m_itemManager->m_cellCount;cLoop++)
	{
		for(CAxLinkObj * pos = gstatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
		{
			CItemObj * iPtr = (CItemObj *) gstatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
			m_globalInventory.AddString(iPtr->m_itemName);
		}
	}	
	m_globalInventory.SetCurSel(sel);
}
//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
void CAiSettingsDlg::InListEntityDB(int sel)
{
	m_entityDB.ResetContent();
	for(POSITION posLoc = m_edbRef->GetHeadPosition();posLoc != NULL;)
	{
		CMovementObj * mPtr = (CMovementObj *) m_edbRef->GetNext(posLoc);
		m_entityDB.AddString(mPtr->List_name);
	}
	m_entityDB.SetCurSel(sel);
}
//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
void CAiSettingsDlg::OnCHECKEnableRadisSys() 
{
	// TODO: Add your control notification handler code here	
}
//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
void CAiSettingsDlg::OnSelchangeCOMBOEntityDB() 
{
	// TODO: Add your control notification handler code here	
}
//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
void CAiSettingsDlg::OnBUTTONAddInventoryObject() 
{
    int glsel = m_globalInventory.GetCurSel();
    if(glsel == -1)
	{
       MessageBox("Must select a global inventory obj");
	   return;
	}

	GlobalStatusVariables * gstatus = (GlobalStatusVariables *)m_gStatusMem;

	CString itemName;
	m_globalInventory.GetWindowTextA(itemName);

	CItemObj * itemPtr = gstatus->m_itemManager->GetItemByName(itemName.GetBuffer());
	if(!itemPtr)
		return;
  
	if(!m_aiPtrRef->m_inventoryCfg)
		m_aiPtrRef->m_inventoryCfg = new CInventoryObjList();

	m_aiPtrRef->m_inventoryCfg->AddInventoryItem(1, FALSE, itemPtr);//add inven obj

	InListInventoryCfgDB(m_aiPtrRef->m_inventoryCfg->GetCount()-1);
}
//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
void CAiSettingsDlg::InListInventoryCfgDB(int sel)
{
	m_inventoryDB.ResetContent();
	if(m_aiPtrRef->m_inventoryCfg)
	{
		for(POSITION posLoc = m_aiPtrRef->m_inventoryCfg->GetHeadPosition();posLoc != NULL;)
		{
			CInventoryObj * iPtr = (CInventoryObj *) m_aiPtrRef->m_inventoryCfg->GetNext(posLoc);
			m_inventoryDB.AddString(iPtr->m_itemData->m_itemName);
		}
	}
	m_inventoryDB.SetCurSel(sel);
}
//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
void CAiSettingsDlg::InListDimCfgs(int sel)
{
	m_dimCfgList.ResetContent();

	if(m_aiPtrRef->m_spawnCfg)
	{
		for(int loop=0;loop<m_aiPtrRef->m_spawnCfgCount;loop++)
		{
			CString build = "DIM:";
			build = operator+(build,ConvToString(loop));
			m_dimCfgList.AddString(build);
		}
	}
	m_dimCfgList.SetCurSel(sel);
}
//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
void CAiSettingsDlg::InListScriptObjects(int sel)
{
	//Make sure we have scripts
	if(!m_aiPtrRef->m_beginningScripts && !m_aiPtrRef->m_randomScripts
	   && !m_aiPtrRef->m_setScripts && !m_aiPtrRef->m_endingScripts)
		return;

	//Reset the list
	m_scriptObjects.ResetContent();

	//Add scripts from the beginning if we have any
	if(m_aiPtrRef->m_beginningScripts)
	{
		for(POSITION posLoc = m_aiPtrRef->m_beginningScripts->GetHeadPosition(); posLoc != NULL;)
		{
			CAILogicScriptObj *scriptPtr = (CAILogicScriptObj *) m_aiPtrRef->m_beginningScripts->GetNext(posLoc);
			CString tempName = "Unknown";
			if(scriptPtr)
				tempName = scriptPtr->scriptName;
			//Add the name to the list
			m_scriptObjects.AddString(tempName);
		}
	}
	
	//Add scripts from random if we have any
	if(m_aiPtrRef->m_randomScripts)
	{
		for(POSITION posLoc = m_aiPtrRef->m_randomScripts->GetHeadPosition(); posLoc != NULL;)
		{
			CAILogicScriptObj *scriptPtr = (CAILogicScriptObj *) m_aiPtrRef->m_randomScripts->GetNext(posLoc);
			CString tempName = "Unknown";
			if(scriptPtr)
				tempName = scriptPtr->scriptName;
			//Add the name to the list
			m_scriptObjects.AddString(tempName);
		}
	}
	
	//Add scripts from the set if we have any
	if(m_aiPtrRef->m_setScripts)
	{
		for(POSITION posLoc = m_aiPtrRef->m_setScripts->GetHeadPosition(); posLoc != NULL;)
		{
			CAILogicScriptObj *scriptPtr = (CAILogicScriptObj *) m_aiPtrRef->m_setScripts->GetNext(posLoc);
			CString tempName = "Unknown";
			if(scriptPtr)
				tempName = scriptPtr->scriptName;
			//Add the name to the list
			m_scriptObjects.AddString(tempName);
		}
	}

	//Add scripts from the ending list if we have any
	if(m_aiPtrRef->m_endingScripts)
	{
		for(POSITION posLoc = m_aiPtrRef->m_endingScripts->GetHeadPosition(); posLoc != NULL;)
		{
			CAILogicScriptObj *scriptPtr = (CAILogicScriptObj *) m_aiPtrRef->m_endingScripts->GetNext(posLoc);
			CString tempName = "Unknown";
			if(scriptPtr)
				tempName = scriptPtr->scriptName;
			//Add the name to the list
			m_scriptObjects.AddString(tempName);
		}
	}

	//Set the selection
	if(sel > -1)
		m_scriptObjects.SetCurSel(sel);
}
//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
void CAiSettingsDlg::TextInterperate(int sel)
{
    UpdateData(TRUE);
	m_staticMiscInt      = "MiscInt1 - Not Used";
	m_staticMiscInt2     = "MiscInt2 - Not Used";
	m_staticMiscInt3     = "MiscInt3 - Not Used";

	//Update the static info for what we are using
	switch(sel)
	{
		case 0://Place Holder 1
			m_staticMiscInt      = "MiscInt1 - Place Holder 1";
			m_staticMiscInt2     = "MiscInt2 - Place Holder 1";
			m_staticMiscInt3     = "MiscInt3 - Place Holder 1";
		break;
		case 1://Place Holder 2
			m_staticMiscInt      = "MiscInt1 - Place Holder 2";
			m_staticMiscInt2     = "MiscInt2 - Not Used";
			m_staticMiscInt3     = "MiscInt3 - Not Used";
		break;
		case 2://Place Holder 3
			m_staticMiscInt      = "MiscInt1 - Place Holder 3";
			m_staticMiscInt2     = "MiscInt2 - Place Holder 3";
			m_staticMiscInt3     = "MiscInt3 - Not Used";
		break;
	}

	UpdateData(FALSE);
}
//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
void CAiSettingsDlg::OnBUTTONDeleteInvenObj() 
{
	int index = m_inventoryDB.GetCurSel();
	if(index == -1)
	{
       MessageBox("Must select an inventory object");
       return;
	}

    if(!m_aiPtrRef->m_inventoryCfg)
		return;

	POSITION posLoc = m_aiPtrRef->m_inventoryCfg->FindIndex(index);
	if(!posLoc)
		return;

	if(!m_aiPtrRef->m_inventoryCfg)
		return;
     
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CInventoryObj * iPtr = (CInventoryObj *) m_aiPtrRef->m_inventoryCfg->GetAt(posLoc);
	iPtr->SafeDelete();
	delete iPtr;
	iPtr = 0;
	m_aiPtrRef->m_inventoryCfg->RemoveAt(posLoc);
	InListInventoryCfgDB(index-1);	
}
//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
void CAiSettingsDlg::OnBUTTONGetCfg() 
{
    int entitySel = m_entityDB.GetCurSel();
	if(entitySel == -1)
	{
       MessageBox("Must Select a entity");
	   return;
	}

	POSITION posLoc = m_edbRef->FindIndex(entitySel);
	if(!posLoc)
	return;
	
	CMovementObj * entPtr = (CMovementObj *)m_edbRef->GetAt(posLoc);
  
	m_aiPtrRef->m_entityCfgDBindexRef = entPtr->m_uniqueId;
    if(entPtr->List_skeletalObject->m_spawnCfg)
     if(entPtr->List_skeletalObject->m_deformableMeshArrayCount > 0)
	 {
	   m_aiPtrRef->m_spawnCfgCount       = entPtr->List_skeletalObject->m_deformableMeshArrayCount;
	   
	   
	   if(m_aiPtrRef->m_spawnCfg)
	      delete [] m_aiPtrRef->m_spawnCfg;
	      m_aiPtrRef->m_spawnCfg = 0;
          m_aiPtrRef->m_spawnCfg = new int[m_aiPtrRef->m_spawnCfgCount];
          CopyMemory(m_aiPtrRef->m_spawnCfg,entPtr->List_skeletalObject->m_spawnCfg,sizeof(int)*m_aiPtrRef->m_spawnCfgCount);
	 }
	 else{
       if(m_aiPtrRef->m_spawnCfg)
	      delete [] m_aiPtrRef->m_spawnCfg;
	      m_aiPtrRef->m_spawnCfg = 0;
	 }

	 InListDimCfgs(0);

}
//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
void CAiSettingsDlg::OnCHECKcheckArmed() 
{
    int index = m_inventoryDB.GetCurSel();
	if(index == -1)
	{
       MessageBox("Must select an inventory object");
       return;
	}

	if(!m_aiPtrRef->m_inventoryCfg)
		return;

	POSITION posLoc = m_aiPtrRef->m_inventoryCfg->FindIndex(index);
	if(posLoc)
	{
       CInventoryObj * iPtr = (CInventoryObj *) m_aiPtrRef->m_inventoryCfg->GetAt(posLoc);
       if(iPtr->m_armed)
		  iPtr->m_armed = FALSE;
	   else
		  iPtr->m_armed = TRUE;
	}

}
//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
void CAiSettingsDlg::OnSelchangeLISTInventory() 
{
	int index = m_inventoryDB.GetCurSel();
	if(index == -1)
	{
       MessageBox("Must select an inventory object");
       return;
	}

    if(!m_aiPtrRef->m_inventoryCfg)
		return;

	POSITION posLoc = m_aiPtrRef->m_inventoryCfg->FindIndex(index);
	if(!posLoc)
		return;

	if(!m_aiPtrRef->m_inventoryCfg)
		return;
     
	CInventoryObj * iPtr = (CInventoryObj *) m_aiPtrRef->m_inventoryCfg->GetAt(posLoc);
	UpdateData(TRUE);
	
	m_checkArmed.SetCheck(iPtr->m_armed);


    UpdateData(FALSE);
	
}
//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
void CAiSettingsDlg::OnDeltaposSPINDimChanger(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;

	int cfgSelect = m_dimCfgList.GetCurSel();
	if(cfgSelect == -1)
		return;

	CMovementObj * entPtr = m_edbRef->GetByID(m_aiPtrRef->m_entityCfgDBindexRef);
	if(!entPtr)
		return;
    /*if(!edbPos)
		return;
        CMovementObj * entPtr = (CMovementObj *) m_edbRef->GetNext(edbPos);*/

	if(!m_aiPtrRef->m_spawnCfg)
		return;

	UpdateData(TRUE);
	int old = m_aiPtrRef->m_spawnCfg[cfgSelect];
	if(pNMUpDown->iDelta == 1)
		m_aiPtrRef->m_spawnCfg[cfgSelect]++;
	else
		m_aiPtrRef->m_spawnCfg[cfgSelect]--;

	if(m_aiPtrRef->m_spawnCfg[cfgSelect] < 0)
		m_aiPtrRef->m_spawnCfg[cfgSelect] = 0;
       	
	POSITION alPos = entPtr->List_skeletalObject->m_alternateConfigurations->FindIndex(cfgSelect);
	if(!alPos)
		return;	
	CAlterUnitDBObject * alPtr = (CAlterUnitDBObject *)entPtr->List_skeletalObject->m_alternateConfigurations->GetNext(alPos);

	POSITION posLoc = alPtr->m_configurations->FindIndex(m_aiPtrRef->m_spawnCfg[cfgSelect]);
	if(!posLoc)
	{
		m_aiPtrRef->m_spawnCfg[cfgSelect] = old;
		return;
	}

	m_dimCfgIndex = m_aiPtrRef->m_spawnCfg[cfgSelect];
	UpdateData(FALSE);
	*pResult = 0;
}

CString CAiSettingsDlg::ConvToString(int number)
{    
	if(number==0)
		return "0";

	int decimal,sign;
	char *buffer;
	CString temp;	 
	buffer = _fcvt(number,0,&decimal,&sign);
	temp = buffer;
	if(sign != 0)
		temp = operator+("-",temp);

	return temp;
}

void CAiSettingsDlg::OnSelchangeCOMBOdimList() 
{
	int cfgSelect = m_dimCfgList.GetCurSel();
	if(cfgSelect == -1)
		return;

	CMovementObj * entPtr = m_edbRef->GetByID(m_aiPtrRef->m_entityCfgDBindexRef);
	if(!entPtr)
	    return;	

	/*POSITION edbPos = m_edbRef->FindIndex(m_aiPtrRef->m_entityCfgDBindexRef);
    if(!edbPos)
		return;
        CMovementObj * entPtr = (CMovementObj *) m_edbRef->GetNext(edbPos);*/

	if(!m_aiPtrRef->m_spawnCfg)
		return;

	UpdateData(TRUE);
	m_dimCfgIndex = m_aiPtrRef->m_spawnCfg[cfgSelect];
	UpdateData(FALSE);
}

void CAiSettingsDlg::OnBUTTONPhyOverride() 
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
	CMotionSettingsDlg dlg;
	if(!m_aiPtrRef->m_physicsModuleOverride)
	{//get from base entity to initialize
		POSITION edbPos = m_edbRef->FindIndex(m_aiPtrRef->m_entityCfgDBindexRef);
        if(!edbPos)
		{
			AfxMessageBox("Must Select a entity type first");
		    return;
		}
        CMovementObj * entPtr = (CMovementObj *) m_edbRef->GetNext(edbPos);
        entPtr->List_movementDB->Clone(&m_aiPtrRef->m_physicsModuleOverride);
		UpdateData(TRUE);
		m_phyOverrideStatus = "Enabled";
		UpdateData(FALSE);
	}//end get from base entity to intialize		
	
	dlg.m_aiListRef = gStatus->m_aiList;
	dlg.movementDB = m_aiPtrRef->m_physicsModuleOverride; 
	dlg.DoModal();
	m_aiPtrRef->m_physicsModuleOverride = dlg.movementDB;	
}

void CAiSettingsDlg::OnBnClickedButtonAiscriptAddtolist()
{
	//Create a new script
	CAILogicScriptObj *newScript = new CAILogicScriptObj();
	UpdateData(TRUE);

	//Store all the script information we'll need
	newScript->scriptName			= m_scriptName;
	newScript->scriptDescription	= m_scriptDescription;
	newScript->whenToUse			= m_scriptOccurance.GetCurSel();
	newScript->lastUseTimeStamp		= 0;
	newScript->howOftenTimer		= m_useTimer;
	newScript->scriptAction			= m_scriptAction.GetCurSel();
	newScript->miscInt1				= m_miscInt1;
	newScript->miscInt2				= m_miscInt2;
	newScript->miscInt3				= m_miscInt3;
	newScript->emoteMessage			= m_scriptEmote;
	newScript->listPlacement		= m_scriptList.GetCurSel();

	//Add to the beginning list
	if(newScript->listPlacement == 0)
	{
		//If we are null, make us valid
		if(!m_aiPtrRef->m_beginningScripts)
			m_aiPtrRef->m_beginningScripts = new CAILogicScriptObjList();
		
		//Add to the tail
		m_aiPtrRef->m_beginningScripts->AddTail(newScript);
	}

	//Add to the random list
	else if(newScript->listPlacement == 1)
	{
		//If we are null, make us valid
		if(!m_aiPtrRef->m_randomScripts)
			m_aiPtrRef->m_randomScripts = new CAILogicScriptObjList();
		
		//Add to the tail
		m_aiPtrRef->m_randomScripts->AddTail(newScript);
	}

	//Add to the set list
	else if(newScript->listPlacement == 2)
	{
		//If we are null, make us valid
		if(!m_aiPtrRef->m_setScripts)
			m_aiPtrRef->m_setScripts = new CAILogicScriptObjList();
		
		//Add to the tail
		m_aiPtrRef->m_setScripts->AddTail(newScript);
	}

	//Add to the ending list
	else if(newScript->listPlacement == 3)
	{
		//If we are null, make us valid
		if(!m_aiPtrRef->m_endingScripts)
			m_aiPtrRef->m_endingScripts = new CAILogicScriptObjList();
		
		//Add to the tail
		m_aiPtrRef->m_endingScripts->AddTail(newScript);
	}
	
	//Update the objects in the list and the selection
	UpdateData(FALSE);
	InListScriptObjects(m_scriptObjects.GetCount());
}

void CAiSettingsDlg::OnBnClickedButtonAiscriptRemovefromlist()
{
	//Make sure we have something selected
	int sel = m_scriptObjects.GetCurSel();
	if(sel == -1)
	{
		AfxMessageBox("Must select a Script to remove");
		return;
	}

	//Get which list we are going to be working in
	int whichList = m_scriptList.GetCurSel();

	//Script we are removing must be in the beginning scripts
	if(whichList == 0)
	{
		//Place holder for the script we're removing
		POSITION holder = m_aiPtrRef->m_beginningScripts->GetHeadPosition();
		for(POSITION posLoc = m_aiPtrRef->m_beginningScripts->GetHeadPosition(); posLoc != NULL;)
		{//script loop
			//update the holder
			holder = posLoc;
			
			CAILogicScriptObj *scriptPtr = (CAILogicScriptObj *) m_aiPtrRef->m_beginningScripts->GetNext(posLoc);
			if(strcmp(scriptPtr->scriptName, m_scriptName) == 0)
			{//found it, remove it
				delete scriptPtr;
				scriptPtr = 0;
				m_aiPtrRef->m_beginningScripts->RemoveAt(holder);
			}//end found it, remove it
		}//end script loop
	}
	
	//Script we are removing is a random
	else if(whichList == 1)
	{
		//Place holder for the script we're removing
		POSITION holder = m_aiPtrRef->m_randomScripts->GetHeadPosition();
		for(POSITION posLoc = m_aiPtrRef->m_randomScripts->GetHeadPosition(); posLoc != NULL;)
		{//script loop
			//update the holder
			holder = posLoc;
			
			CAILogicScriptObj *scriptPtr = (CAILogicScriptObj *) m_aiPtrRef->m_randomScripts->GetNext(posLoc);
			if(strcmp(scriptPtr->scriptName, m_scriptName) == 0)
			{//found it, remove it
				delete scriptPtr;
				scriptPtr = 0;
				m_aiPtrRef->m_randomScripts->RemoveAt(posLoc);
			}//end found it, remove it
		}//end script loop
	}
	
	//Script we are removing is a set script
	else if(whichList == 2)
	{
		//Place holder for the script we're removing
		POSITION holder = m_aiPtrRef->m_setScripts->GetHeadPosition();
		for(POSITION posLoc = m_aiPtrRef->m_setScripts->GetHeadPosition(); posLoc != NULL;)
		{//script loop
			//update the holder
			holder = posLoc;
			
			CAILogicScriptObj *scriptPtr = (CAILogicScriptObj *) m_aiPtrRef->m_setScripts->GetNext(posLoc);
			if(strcmp(scriptPtr->scriptName, m_scriptName) == 0)
			{//found it, remove it
				delete scriptPtr;
				scriptPtr = 0;
				m_aiPtrRef->m_setScripts->RemoveAt(posLoc);
			}//end found it, remove it
		}//end script loop
	}
	
	//Script we are removing is an ending script
	else if(whichList == 3)
	{
		//Place holder for the script we're removing
		POSITION holder = m_aiPtrRef->m_endingScripts->GetHeadPosition();
		for(POSITION posLoc = m_aiPtrRef->m_endingScripts->GetHeadPosition(); posLoc != NULL;)
		{//script loop
			//update the holder
			holder = posLoc;
			
			CAILogicScriptObj *scriptPtr = (CAILogicScriptObj *) m_aiPtrRef->m_endingScripts->GetNext(posLoc);
			if(strcmp(scriptPtr->scriptName, m_scriptName) == 0)
			{//found it, remove it
				delete scriptPtr;
				scriptPtr = 0;
				m_aiPtrRef->m_endingScripts->RemoveAt(posLoc);
			}//end found it, remove it
		}//end script loop
	}

	//Update the objects in the list and the selection
	InListScriptObjects(m_scriptObjects.GetCount()-1);
}

void CAiSettingsDlg::OnBnClickedButtonAiscriptReplaceinlist()
{
	//Store old script and what list it was in
	//Value initializing
	CAILogicScriptObj *oldScriptPtr = NULL;
	POSITION holder = NULL;
	int scriptIndex = m_scriptObjects.GetCurSel();
	int listHolder = 0;

	int scriptCount = 0;
	int begStart = 0;
	int ranStart = 0;
	int setStart = 0;
	int endStart = 0;

	//Setup the beginning script variables
	if(m_aiPtrRef->m_beginningScripts)
	{
		scriptCount += m_aiPtrRef->m_beginningScripts->GetCount();
		ranStart = scriptCount;
	}
	//Setup the random script variables
	if(m_aiPtrRef->m_randomScripts)
	{
		ranStart = scriptCount;
		scriptCount += m_aiPtrRef->m_randomScripts->GetCount();
		setStart = scriptCount;
	}
	//Setup the set script variables
	if(m_aiPtrRef->m_setScripts)
	{
		setStart = scriptCount;
		scriptCount += m_aiPtrRef->m_setScripts->GetCount();
		endStart = scriptCount;
	}
	//Setup the ending script variables
	if(m_aiPtrRef->m_endingScripts)
	{
		endStart = scriptCount;
		scriptCount += m_aiPtrRef->m_endingScripts->GetCount();
	}

	//The value selected in the list is part of a BEGINNING script
	if(scriptIndex >= begStart && scriptIndex < ranStart)
	{
		POSITION posLoc = m_aiPtrRef->m_beginningScripts->FindIndex(scriptIndex);
		if(!posLoc)
			return;
		oldScriptPtr = (CAILogicScriptObj*) m_aiPtrRef->m_beginningScripts->GetAt(posLoc);
		holder = posLoc;
		listHolder = 0;
	}
	//The value seleced in the list is part of a RANDOM script
	else if(scriptIndex >= ranStart && scriptIndex < setStart)
	{
		POSITION posLoc = m_aiPtrRef->m_randomScripts->FindIndex(scriptIndex-ranStart);
		if(!posLoc)
			return;
		oldScriptPtr = (CAILogicScriptObj*) m_aiPtrRef->m_randomScripts->GetAt(posLoc);
		holder = posLoc;
		listHolder = 1;
	}
	//The value selected in the list is part of a SET script
	else if(scriptIndex >= setStart && scriptIndex < endStart)
	{
		POSITION posLoc = m_aiPtrRef->m_setScripts->FindIndex(scriptIndex-setStart);
		if(!posLoc)
			return;
		oldScriptPtr = (CAILogicScriptObj*) m_aiPtrRef->m_setScripts->GetAt(posLoc);
		holder = posLoc;
		listHolder = 2;
	}
	//The value selected in the list is part of a ENDING script
	else if(scriptIndex >= endStart && scriptIndex < scriptCount)
	{
		POSITION posLoc = m_aiPtrRef->m_endingScripts->FindIndex(scriptIndex-endStart);
		if(!posLoc)
			return;
		oldScriptPtr = (CAILogicScriptObj*) m_aiPtrRef->m_endingScripts->GetAt(posLoc);
		holder = posLoc;
		listHolder = 3;
	}

	//invalid script
	if(!oldScriptPtr)
		return;

	//if the script changed lists, remove it from its original list and add to the new
	if(oldScriptPtr->listPlacement != m_scriptList.GetCurSel())
	{
		//remove from the old
		switch(listHolder)
		{
			case 0:
				delete oldScriptPtr;
				oldScriptPtr = 0;
				m_aiPtrRef->m_beginningScripts->RemoveAt(holder);
			break;
			case 1:
				delete oldScriptPtr;
				oldScriptPtr = 0;
				m_aiPtrRef->m_randomScripts->RemoveAt(holder);
			break;
			case 2:
				delete oldScriptPtr;
				oldScriptPtr = 0;
				m_aiPtrRef->m_setScripts->RemoveAt(holder);
			break;
			case 3:
				delete oldScriptPtr;
				oldScriptPtr = 0;
				m_aiPtrRef->m_endingScripts->RemoveAt(holder);
			break;
		}
		//add to the new
		//Create a new script
		CAILogicScriptObj *newScript = new CAILogicScriptObj();
		UpdateData(TRUE);

		//Store all the script information we'll need
		newScript->scriptName			= m_scriptName;
		newScript->scriptDescription	= m_scriptDescription;
		newScript->whenToUse			= m_scriptOccurance.GetCurSel();
		newScript->lastUseTimeStamp		= 0;
		newScript->howOftenTimer		= m_useTimer;
		newScript->scriptAction			= m_scriptAction.GetCurSel();
		newScript->miscInt1				= m_miscInt1;
		newScript->miscInt2				= m_miscInt2;
		newScript->miscInt3				= m_miscInt3;
		newScript->emoteMessage			= m_scriptEmote;
		newScript->listPlacement		= m_scriptList.GetCurSel();

		//Add to the beginning list
		if(newScript->listPlacement == 0)
		{
			//If we are null, make us valid
			if(!m_aiPtrRef->m_beginningScripts)
				m_aiPtrRef->m_beginningScripts = new CAILogicScriptObjList();
			
			//Add to the tail
			m_aiPtrRef->m_beginningScripts->AddTail(newScript);
		}

		//Add to the random list
		else if(newScript->listPlacement == 1)
		{
			//If we are null, make us valid
			if(!m_aiPtrRef->m_randomScripts)
				m_aiPtrRef->m_randomScripts = new CAILogicScriptObjList();
			
			//Add to the tail
			m_aiPtrRef->m_randomScripts->AddTail(newScript);
		}

		//Add to the set list
		else if(newScript->listPlacement == 2)
		{
			//If we are null, make us valid
			if(!m_aiPtrRef->m_setScripts)
				m_aiPtrRef->m_setScripts = new CAILogicScriptObjList();
			
			//Add to the tail
			m_aiPtrRef->m_setScripts->AddTail(newScript);
		}

		//Add to the ending list
		else if(newScript->listPlacement == 3)
		{
			//If we are null, make us valid
			if(!m_aiPtrRef->m_endingScripts)
				m_aiPtrRef->m_endingScripts = new CAILogicScriptObjList();
			
			//Add to the tail
			m_aiPtrRef->m_endingScripts->AddTail(newScript);
		}

		//Update the objects in the list and the selection
		UpdateData(FALSE);
	}
	//otherwise modify the list with its new information
	else
	{
		oldScriptPtr->scriptName			= m_scriptName;
		oldScriptPtr->scriptDescription		= m_scriptDescription;
		oldScriptPtr->whenToUse				= m_scriptOccurance.GetCurSel();
		oldScriptPtr->lastUseTimeStamp		= 0;
		oldScriptPtr->howOftenTimer			= m_useTimer;
		oldScriptPtr->scriptAction			= m_scriptAction.GetCurSel();
		oldScriptPtr->miscInt1				= m_miscInt1;
		oldScriptPtr->miscInt2				= m_miscInt2;
		oldScriptPtr->miscInt3				= m_miscInt3;
		oldScriptPtr->emoteMessage			= m_scriptEmote;
		oldScriptPtr->listPlacement			= m_scriptList.GetCurSel();
	}

	//Update the objects in the list
	InListScriptObjects(-1);
}

void CAiSettingsDlg::OnLbnSelchangeListAiscriptScriptlist()
{
	//Value initializing
	CAILogicScriptObj *scriptPtr = NULL;
	int scriptIndex = m_scriptObjects.GetCurSel();

	int scriptCount = 0;
	int begStart = 0;
	int ranStart = 0;
	int setStart = 0;
	int endStart = 0;

	//Setup the beginning script variables
	if(m_aiPtrRef->m_beginningScripts)
	{
		scriptCount += m_aiPtrRef->m_beginningScripts->GetCount();
		ranStart = scriptCount;
	}
	//Setup the random script variables
	if(m_aiPtrRef->m_randomScripts)
	{
		ranStart = scriptCount;
		scriptCount += m_aiPtrRef->m_randomScripts->GetCount();
		setStart = scriptCount;
	}
	//Setup the set script variables
	if(m_aiPtrRef->m_setScripts)
	{
		setStart = scriptCount;
		scriptCount += m_aiPtrRef->m_setScripts->GetCount();
		endStart = scriptCount;
	}
	//Setup the ending script variables
	if(m_aiPtrRef->m_endingScripts)
	{
		endStart = scriptCount;
		scriptCount += m_aiPtrRef->m_endingScripts->GetCount();
	}

	//The value selected in the list is part of a BEGINNING script
	if(scriptIndex >= begStart && scriptIndex < ranStart)
	{
		POSITION posLoc = m_aiPtrRef->m_beginningScripts->FindIndex(scriptIndex);
		if(!posLoc)
			return;
		scriptPtr = (CAILogicScriptObj*) m_aiPtrRef->m_beginningScripts->GetAt(posLoc);
	}
	//The value seleced in the list is part of a RANDOM script
	else if(scriptIndex >= ranStart && scriptIndex < setStart)
	{
		POSITION posLoc = m_aiPtrRef->m_randomScripts->FindIndex(scriptIndex-ranStart);
		if(!posLoc)
			return;
		scriptPtr = (CAILogicScriptObj*) m_aiPtrRef->m_randomScripts->GetAt(posLoc);
	}
	//The value selected in the list is part of a SET script
	else if(scriptIndex >= setStart && scriptIndex < endStart)
	{
		POSITION posLoc = m_aiPtrRef->m_setScripts->FindIndex(scriptIndex-setStart);
		if(!posLoc)
			return;
		scriptPtr = (CAILogicScriptObj*) m_aiPtrRef->m_setScripts->GetAt(posLoc);
	}
	//The value selected in the list is part of a ENDING script
	else if(scriptIndex >= endStart && scriptIndex < scriptCount)
	{
		POSITION posLoc = m_aiPtrRef->m_endingScripts->FindIndex(scriptIndex-endStart);
		if(!posLoc)
			return;
		scriptPtr = (CAILogicScriptObj*) m_aiPtrRef->m_endingScripts->GetAt(posLoc);
	}

	//Set the parameters if valid
	if(scriptPtr)
	{
		m_scriptOccurance.SetCurSel(scriptPtr->whenToUse);
		m_scriptAction.SetCurSel(scriptPtr->scriptAction);
		m_scriptList.SetCurSel(scriptPtr->listPlacement);
		SetDlgItemText(IDC_EDIT_AISCRIPT_NAME, scriptPtr->scriptName);
		SetDlgItemText(IDC_EDIT_AISCRIPT_DESCRIPTION, scriptPtr->scriptDescription);
		SetDlgItemText(IDC_EDIT_AISCRIPT_EMOTEMESSAGE, scriptPtr->emoteMessage);	
		SetDlgItemInt(IDC_EDIT_AISCRIPT_USETIMER, scriptPtr->howOftenTimer, TRUE);
		SetDlgItemInt(IDC_EDIT_AISCRIPT_MISCINT1, scriptPtr->miscInt1, TRUE);
		SetDlgItemInt(IDC_EDIT_AISCRIPT_MISCINT2, scriptPtr->miscInt2, TRUE);
		SetDlgItemInt(IDC_EDIT_AISCRIPT_MISCINT3, scriptPtr->miscInt3, TRUE);
		TextInterperate(scriptPtr->scriptAction);
	}
}

void CAiSettingsDlg::OnCbnSelchangeComboAiscriptAction()
{
	//Get the selection in the drop box
	int sel = m_scriptAction.GetCurSel();
	if(sel < 0)
	   return;

	//Update the misc int info
	TextInterperate(sel);
}
