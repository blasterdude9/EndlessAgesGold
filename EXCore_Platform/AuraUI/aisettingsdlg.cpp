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
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_steeringModel, m_steeringModel);
	DDX_Control(pDX, IDC_CHECK_aiTreeLock, m_treeLock);
    DDX_Text(pDX, IDC_STATIC_phyOverrideStatus, m_phyOverrideStatus);
	
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
	m_agressiveRange			 = m_aiPtrRef->m_agressiveRange;
	m_strikeShootRange           = m_aiPtrRef->m_strikeShootRange;


    m_steeringModel.AddString("AI Standard");
	m_steeringModel.AddString("Platform Standard");

	m_steeringModel.SetCurSel(m_aiPtrRef->m_steeringModel);


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
	m_aiPtrRef->m_agressiveRange			 = m_agressiveRange;

    m_aiPtrRef->m_enableCashApplied			 = m_enableLootGen.GetCheck();
	m_aiPtrRef->m_accuracy					 = m_sliderAccuracy.GetPos();

    m_aiPtrRef->m_radiusSysEnabled           = m_enableRadiusSystem.GetCheck();
	m_aiPtrRef->m_radiusSysAttackOnExtents   = m_radiusSysAttackOnExtents.GetCheck();
	m_aiPtrRef->m_radiusSysIgnoreY           = m_radiusSysIgnoreElevation.GetCheck();
	m_aiPtrRef->m_lockedOnPath               = m_treeLock.GetCheck();
    m_aiPtrRef->m_percentageOfDropping       = m_percItemDrop;
	m_aiPtrRef->m_strikeShootRange			 = m_strikeShootRange;


	m_aiPtrRef->m_steeringModel = m_steeringModel.GetCurSel();

	UpdateData(FALSE);
	CDialog::OnOK();
}

void CAiSettingsDlg::OnCHECKEnableExponentTuring() 
{
	if(enableExponentTurning){
       enableExponentTurning = FALSE;
	}
	else{
       enableExponentTurning = TRUE;
	}
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
		 gstatus->m_itemManager->m_listArray[0].m_axList[cLoop].ManualLock();//list traversals have to be unsafe commands so we lock the list channel
		 for(CAxLinkObj * pos = gstatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
		 {
			 CItemObj * iPtr = (CItemObj *) gstatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
			 m_globalInventory.AddString(iPtr->m_itemName);
		 }
		 gstatus->m_itemManager->m_listArray[0].m_axList[cLoop].ManualUnLock();
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


	m_aiPtrRef->m_inventoryCfg->AddInventoryItem(1,
												 FALSE,
												 itemPtr);//add inven obj

	InListInventoryCfgDB(m_aiPtrRef->m_inventoryCfg->GetCount()-1);
}
//----------------------------------------------------//
//----------------------------------------------------//
//----------------------------------------------------//
void CAiSettingsDlg::InListInventoryCfgDB(int sel)
{
   m_inventoryDB.ResetContent();
   if(m_aiPtrRef->m_inventoryCfg)
   for(POSITION posLoc = m_aiPtrRef->m_inventoryCfg->GetHeadPosition();posLoc != NULL;)
   {
       CInventoryObj * iPtr = (CInventoryObj *) m_aiPtrRef->m_inventoryCfg->GetNext(posLoc);
       
	   m_inventoryDB.AddString(iPtr->m_itemData->m_itemName);
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
   for(int loop=0;loop<m_aiPtrRef->m_spawnCfgCount;loop++)
   {
	   CString build = "DIM:";
	   build = operator+(build,ConvToString(loop));
       m_dimCfgList.AddString(build);
   }
   m_dimCfgList.SetCurSel(sel);
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
	{
	   return;
	}

	

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
             if(!posLoc){
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
	{
	   return;
	}

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
