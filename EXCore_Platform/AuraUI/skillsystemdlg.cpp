// SkillSystemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "direct.h"
#include "SkillSystemDlg.h"
#include "globalstatusvariables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkillSystemDlg dialog


CSkillSystemDlg::CSkillSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkillSystemDlg::IDD, pParent)
	, m_proficiency(0)
	, m_perfective(0)
{
	//{{AFX_DATA_INIT(CSkillSystemDlg)
	m_abilityMasteredExp = 0.0f;
	m_abilityMinimumLevel = 0;
	m_abilityMiscInt = 0;
	m_abilityName = _T("");
	m_raceAdvFactor = 0.0f;
	m_basicAbilityMastered = 0.0f;
	m_endExpRange = 0.0f;
	m_expStart = 0.0f;
	m_gainIncrement = 0.0f;
	m_levelTitle = _T("");
	m_skillName = _T("");
	m_expStartRange = 0.0f;
	m_startLevel = 0;
	m_statAmount = 0.0f;
	m_typeRef = 0;
	m_menuID = 0;
	m_gainEvenIfFailedValue = 0.0f;
	m_maxMultiplier = 0.0f;
	m_miscInt2 = 0;
	m_miscInt3 = 0;
	m_effectBind = 0;
	m_miscAmount = 0;
	//}}AFX_DATA_INIT
}


void CSkillSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkillSystemDlg)
	DDX_Control(pDX, IDC_COMBO_GlobalInventory, m_globalInventory);
	DDX_Control(pDX, IDC_COMBO_StatDatabase, m_statRefList);
	DDX_Control(pDX, IDC_COMBO_edbList, m_edbList);
	DDX_Control(pDX, IDC_COMBO_BasicAbilityFunction, m_basicAbilityFunction);
	DDX_Control(pDX, IDC_COMBO_AbilityFunction, m_cmboAbilityFunction);
	DDX_Control(pDX, IDC_CHECK_BasicAbilitySkill, m_basicAbilitySkillEnable);
	DDX_Control(pDX, IDC_LIST_AbilityDB, m_abilitiesList);
	DDX_Control(pDX, IDC_LIST_AbilityRequiredItems, m_abilityRequiredItemsList);
	DDX_Control(pDX, IDC_LIST_AbilityResultItems, m_abilityResultItemsList);
	DDX_Control(pDX, IDC_LIST_RacialAdvDB, m_racialAdvantageList);
	DDX_Control(pDX, IDC_LIST_requiredStats, m_abilityRequiredStatsList);
	DDX_Control(pDX, IDC_LIST_SkillLevelsDB, m_skillLevelsList);
	DDX_Control(pDX, IDC_LIST_SkillsDB, m_skillsList);
	DDX_Text(pDX, IDC_EDIT_AbilityMasteredExp, m_abilityMasteredExp);
	DDX_Text(pDX, IDC_EDIT_AbilityminimumLevel, m_abilityMinimumLevel);
	DDX_Text(pDX, IDC_EDIT_abilityMiscInt, m_abilityMiscInt);
	DDX_Text(pDX, IDC_EDIT_abilityName, m_abilityName);
	DDX_Text(pDX, IDC_EDIT_AdvFactor, m_raceAdvFactor);
	DDX_Text(pDX, IDC_EDIT_basicAbilityMastered, m_basicAbilityMastered);
	DDX_Text(pDX, IDC_EDIT_EndExpRange, m_endExpRange);
	DDX_Text(pDX, IDC_EDIT_ExpStart, m_expStart);
	DDX_Text(pDX, IDC_EDIT_gainIncrement, m_gainIncrement);
	DDX_Text(pDX, IDC_EDIT_LevelTitle, m_levelTitle);
	DDX_Text(pDX, IDC_EDIT_SkillName, m_skillName);
	DDX_Text(pDX, IDC_EDIT_StartExpRange, m_expStartRange);
	DDX_Text(pDX, IDC_EDIT_StartLevel, m_startLevel);
	DDX_Text(pDX, IDC_EDIT_statAmount, m_statAmount);
	DDX_Text(pDX, IDC_EDIT_TypeRef, m_typeRef);
	DDX_Text(pDX, IDC_EDIT_MenuID, m_menuID);
	DDX_Text(pDX, IDC_EDIT_GainEvenIfFailedExpLevel, m_gainEvenIfFailedValue);
	DDX_Text(pDX, IDC_EDIT_MaxMultiplier, m_maxMultiplier);
	DDX_Text(pDX, IDC_EDIT_abilityMiscInt2, m_miscInt2);
	DDX_Text(pDX, IDC_EDIT_abilityMiscInt3, m_miscInt3);
	DDX_Text(pDX, IDC_EDIT_effectBind, m_effectBind);
	DDX_Text(pDX, IDC_EDIT_miscAmount, m_miscAmount);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_Proficiency, m_proficiency);
	DDV_MinMaxInt(pDX, m_proficiency, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_Perfective, m_perfective);
	DDV_MinMaxInt(pDX, m_perfective, 0, 1000);
	DDX_Control(pDX, IDC_CHECK_EnableInternalTimer, m_enableInternalTimer);
	DDX_Control(pDX, IDC_CHECK_FilterLevelEval, m_filterLevelEval);
}


BEGIN_MESSAGE_MAP(CSkillSystemDlg, CDialog)
	//{{AFX_MSG_MAP(CSkillSystemDlg)
	ON_LBN_SELCHANGE(IDC_LIST_SkillsDB, OnSelchangeLISTSkillsDB)
	ON_BN_CLICKED(IDC_BUTTON_AddSkill, OnBUTTONAddSkill)
	ON_BN_CLICKED(IDC_BUTTON_Delete_Skill, OnBUTTONDeleteSkill)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceSkill, OnBUTTONReplaceSkill)
	ON_BN_CLICKED(IDC_BUTTON_AddRacialAdv, OnBUTTONAddRacialAdv)
	ON_BN_CLICKED(IDC_BUTTON_DeleteRacialAdv, OnBUTTONDeleteRacialAdv)
	ON_LBN_SELCHANGE(IDC_LIST_RacialAdvDB, OnSelchangeLISTRacialAdvDB)
	ON_BN_CLICKED(IDC_BUTTON_AddLevel, OnBUTTONAddLevel)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceLevel, OnBUTTONReplaceLevel)
	ON_BN_CLICKED(IDC_BUTTON_DeleteLevel, OnBUTTONDeleteLevel)
	ON_LBN_SELCHANGE(IDC_LIST_SkillLevelsDB, OnSelchangeLISTSkillLevelsDB)
	ON_BN_CLICKED(IDC_BUTTON_ADDAbility, OnBUTTONADDAbility)
	ON_BN_CLICKED(IDC_BUTTON_Delete_Ability, OnBUTTONDeleteAbility)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceAbility, OnBUTTONReplaceAbility)
	ON_LBN_SELCHANGE(IDC_LIST_AbilityDB, OnSelchangeLISTAbilityDB)
	ON_BN_CLICKED(IDC_BUTTON_AddStatReq, OnBUTTONAddStatReq)
	ON_BN_CLICKED(IDC_BUTTON_DeleteStatReq, OnBUTTONDeleteStatReq)
	ON_BN_CLICKED(IDC_BUTTON_AddRequiredItem, OnBUTTONAddRequiredItem)
	ON_BN_CLICKED(IDC_BUTTON_AddResultItem, OnBUTTONAddResultItem)
	ON_BN_CLICKED(IDC_BUTTON_DeleteRequiredItem, OnBUTTONDeleteRequiredItem)
	ON_BN_CLICKED(IDC_BUTTON_DeleteResultItem, OnBUTTONDeleteResultItem)
	ON_BN_CLICKED(IDC_BUTTON_LoadSys, OnBUTTONLoadSys)
	ON_BN_CLICKED(IDC_BUTTON_SaveSkillSys, OnBUTTONSaveSkillSys)
	ON_LBN_SELCHANGE(IDC_LIST_requiredStats, OnSelchangeLISTrequiredStats)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceStatReq, OnBUTTONReplaceStatReq)
	ON_BN_CLICKED(IDC_BUTTON_InsertLevel, OnBUTTONInsertLevel)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_ExportToSql, OnBnClickedButtonExporttosql)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkillSystemDlg message handlers

BOOL CSkillSystemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;

	UpdateData(TRUE);
    POSITION posLoc;
	for(posLoc = m_edbDatabaseRef->GetHeadPosition();posLoc != NULL;)
	{
        CMovementObj * mPtr = (CMovementObj*)m_edbDatabaseRef->GetNext(posLoc);
		m_edbList.AddString(mPtr->List_name);
	}
	m_edbList.SetCurSel(0);
	
    for(posLoc = m_statDatabaseRef->GetHeadPosition();posLoc != NULL;)
	{
        CStatObj * sPtr = (CStatObj*)m_statDatabaseRef->GetNext(posLoc);
		m_statRefList.AddString(sPtr->m_statName);
	}
	m_statRefList.SetCurSel(0);

	for(unsigned int cLoop=0;cLoop<gStatus->m_itemManager->m_cellCount;cLoop++)
	{
		 for(CAxLinkObj * pos = gStatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
		 {
			 CItemObj * iPtr = (CItemObj *) gStatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
			 m_globalInventory.AddString(iPtr->m_itemName);
		 }
	}	
	m_globalInventory.SetCurSel(0);

	m_basicAbilityFunction.SetCurSel(0);
	m_cmboAbilityFunction.SetCurSel(0);
	
	UpdateData(FALSE);

	InListSkills(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::InListSkills(int sel)
{
	m_skillsList.ResetContent();

	for(POSITION posLoc = m_skillDatabaseRef->GetHeadPosition();posLoc != NULL;)
	{
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetNext(posLoc);
		m_skillsList.AddString(skPtr->m_skillName);
	}

	if(sel > -1)
	   m_skillsList.SetCurSel(sel);

	
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnSelchangeLISTSkillsDB() 
{
	int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		UpdateData(TRUE);
	    m_skillName               = skPtr->m_skillName;
	    m_gainIncrement           = skPtr->m_gainIncrement;
	    m_expStart                = skPtr->m_currentExperience;
	    m_startLevel              = skPtr->m_currentLevel;
	    m_basicAbilityMastered    = skPtr->m_basicAbilityExpMastered;
		m_typeRef                 = skPtr->m_skillType;
		m_gainEvenIfFailedValue   = skPtr->m_skillGainsWhenFailed;
   	    m_maxMultiplier           = skPtr->m_maxMultiplier;
	    m_enableInternalTimer.SetCheck(skPtr->m_useInternalTimer);
	    m_basicAbilitySkillEnable.SetCheck(skPtr->m_basicAbilitySkill);
	    m_basicAbilityFunction.SetCurSel(skPtr->m_basicAbilityFunction);

		m_filterLevelEval.SetCheck(skPtr->m_filterEvalLevel);
	  
	    UpdateData(FALSE);

		InListRacialAdvantages(0);
		InListLevels(0);
		
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnBUTTONAddSkill() 
{
	CSkillObject * skillPtr = new CSkillObject();
	UpdateData(TRUE);
	skillPtr->m_skillName               = m_skillName;
	skillPtr->m_gainIncrement           = m_gainIncrement;
	skillPtr->m_currentExperience       = m_expStart;
	skillPtr->m_currentLevel            = m_startLevel;
	skillPtr->m_basicAbilityExpMastered = m_basicAbilityMastered;
	skillPtr->m_basicAbilitySkill       = m_basicAbilitySkillEnable.GetCheck();
	skillPtr->m_basicAbilityFunction    = m_basicAbilityFunction.GetCurSel();
	skillPtr->m_skillType               = m_skillDatabaseRef->GetFreeID();
	skillPtr->m_skillGainsWhenFailed    = m_gainEvenIfFailedValue;
	skillPtr->m_maxMultiplier           = m_maxMultiplier;
	skillPtr->m_useInternalTimer        = m_enableInternalTimer.GetCheck();
	skillPtr->m_filterEvalLevel         = m_filterLevelEval.GetCheck();
	if(skillPtr->m_basicAbilityFunction == -1)
	   skillPtr->m_basicAbilityFunction = 0;

	m_skillDatabaseRef->AddTail(skillPtr);
	UpdateData(FALSE);
	InListSkills(m_skillDatabaseRef->GetCount()-1);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnBUTTONDeleteSkill() 
{
	int skillSelection = m_skillsList.GetCurSel();
	if(skillSelection == -1)
	{
		MessageBox("Must select a skill");
		return;
	}

	POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;

	CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);
	skPtr->SafeDelete();
	delete skPtr;
	skPtr = 0;
	m_skillDatabaseRef->RemoveAt(posLoc);

	InListSkills(skillSelection-1);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnBUTTONReplaceSkill() 
{
    int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
       MessageBox("Must select a skill");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		UpdateData(TRUE);
	    skPtr->m_skillName               = m_skillName;
	    skPtr->m_gainIncrement           = m_gainIncrement;
	    skPtr->m_currentExperience       = m_expStart;
	    skPtr->m_currentLevel            = m_startLevel;
	    skPtr->m_basicAbilityExpMastered = m_basicAbilityMastered;
	    skPtr->m_basicAbilitySkill       = m_basicAbilitySkillEnable.GetCheck();
	    skPtr->m_basicAbilityFunction    = m_basicAbilityFunction.GetCurSel();
		skPtr->m_skillGainsWhenFailed    = m_gainEvenIfFailedValue;
   	    skPtr->m_maxMultiplier           = m_maxMultiplier;
		skPtr->m_useInternalTimer        = m_enableInternalTimer.GetCheck();
		skPtr->m_filterEvalLevel         = m_filterLevelEval.GetCheck();
	    if(skPtr->m_basicAbilityFunction == -1)
	       skPtr->m_basicAbilityFunction = 0;

	    UpdateData(FALSE);
	InListSkills(skillSelection);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::InListRacialAdvantages(int sel)
{
    m_racialAdvantageList.ResetContent();
	int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		if(!skPtr->m_raceAdvantageDB)
			return;

		for(POSITION rcPos = skPtr->m_raceAdvantageDB->GetHeadPosition();rcPos != NULL;)
		{
            CRacialAdvantageObject * rPtr = (CRacialAdvantageObject *)skPtr->m_raceAdvantageDB->GetNext(rcPos);
            POSITION entPos = m_edbDatabaseRef->FindIndex(rPtr->m_edbRef);
			if(entPos){
               CMovementObj * mPtr = (CMovementObj*)m_edbDatabaseRef->GetAt(entPos);
		       m_racialAdvantageList.AddString(mPtr->List_name);
			}
		}


		if(sel > -1)
		   m_racialAdvantageList.SetCurSel(sel);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnBUTTONAddRacialAdv() 
{
	int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

	int edbSelection = m_edbList.GetCurSel();
	if(edbSelection == -1)
	{
	   MessageBox("Must select an entity.");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		if(!skPtr->m_raceAdvantageDB)
			skPtr->m_raceAdvantageDB = new CRacialAdvantageList();

        CRacialAdvantageObject * rPtr = new CRacialAdvantageObject();
  
		UpdateData(TRUE);
		rPtr->m_edbRef         = edbSelection;
        rPtr->m_adjustment     = m_raceAdvFactor;
		UpdateData(FALSE);

	    skPtr->m_raceAdvantageDB->AddTail(rPtr);
		InListRacialAdvantages(skPtr->m_raceAdvantageDB->GetCount()-1);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnBUTTONDeleteRacialAdv() 
{
    int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

	int edbSelection = m_edbList.GetCurSel();
	if(edbSelection == -1)
	{
	   MessageBox("Must select an entity.");
       return;
	}

	int racialAdvSelection = m_racialAdvantageList.GetCurSel();
    if(racialAdvSelection == -1)
	{
       MessageBox("Must select an entity.");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
    
	CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);
	if(!skPtr->m_raceAdvantageDB)
		return;

    POSITION racePos = skPtr->m_raceAdvantageDB->FindIndex(racialAdvSelection);
	if(!racePos)
	    return;
    
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CRacialAdvantageObject * rPtr = (CRacialAdvantageObject *)skPtr->m_raceAdvantageDB->GetAt(racePos);  
    delete rPtr;
	rPtr = 0;
	skPtr->m_raceAdvantageDB->RemoveAt(racePos);
	InListRacialAdvantages(racialAdvSelection-1);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnSelchangeLISTRacialAdvDB() 
{
	int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

	int edbSelection = m_edbList.GetCurSel();
	if(edbSelection == -1)
	{
	   MessageBox("Must select an entity.");
       return;
	}

	int racialAdvSelection = m_racialAdvantageList.GetCurSel();
    if(racialAdvSelection == -1)
	{
       MessageBox("Must select an entity.");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);
  
		if(!skPtr->m_raceAdvantageDB)
			return;

        POSITION racePos = skPtr->m_raceAdvantageDB->FindIndex(racialAdvSelection);
		if(!racePos)
		    return;
        
		CRacialAdvantageObject * rPtr = (CRacialAdvantageObject *)skPtr->m_raceAdvantageDB->GetAt(racePos);  
        UpdateData(TRUE);
		m_edbList.SetCurSel(rPtr->m_edbRef);
        m_raceAdvFactor = rPtr->m_adjustment;
		UpdateData(FALSE);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnBUTTONAddLevel() 
{
	int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		if(!skPtr->m_levelRangeDB)
			skPtr->m_levelRangeDB = new CLevelRangeList();


		CLevelRangeObject * levelRange = new CLevelRangeObject();
		UpdateData(TRUE);
		/*levelRange->m_rangeStart = m_expStartRange;
		levelRange->m_rangeEnd   = m_endExpRange;*/

		levelRange->m_rangeStart = (long)m_expStartRange;
		levelRange->m_rangeEnd   = (long)m_endExpRange;
		//WARNING: C4244 --> conversion from float to long, possible loss of data
	    
        levelRange->m_levelTitle = m_levelTitle;
		levelRange->m_menuID     = m_menuID;
		levelRange->m_profficiencyBonus = m_proficiency;
		levelRange->m_perfectiveBonus = m_perfective;
		
		skPtr->m_levelRangeDB->AddTail(levelRange);
		UpdateData(FALSE);
		InListLevels(skPtr->m_levelRangeDB->GetCount()-1);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::InListLevels(int sel)
{
    m_skillLevelsList.ResetContent();
    int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		if(!skPtr->m_levelRangeDB)
			return;

        for(POSITION lPos = skPtr->m_levelRangeDB->GetHeadPosition();lPos != NULL;)
		{
            CLevelRangeObject * levelRange = (CLevelRangeObject *) skPtr->m_levelRangeDB->GetNext(lPos);
            m_skillLevelsList.AddString(levelRange->m_levelTitle);
		}

	if(sel > -1)
       m_skillLevelsList.SetCurSel(sel);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnBUTTONReplaceLevel() 
{
    int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

	int skillLevelSel = m_skillLevelsList.GetCurSel();
    if(skillLevelSel == -1)
	{
	   MessageBox("Must select a skill Level.");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		if(!skPtr->m_levelRangeDB)
			return;


		POSITION lvlPos = skPtr->m_levelRangeDB->FindIndex(skillLevelSel);
		if(!lvlPos)
			return;
		CLevelRangeObject * levelRange = (CLevelRangeObject *)skPtr->m_levelRangeDB->GetAt(lvlPos);

		UpdateData(TRUE);
      
		/*levelRange->m_rangeStart = m_expStartRange;
	    levelRange->m_rangeEnd   = m_endExpRange;*/

		levelRange->m_rangeStart = (long)m_expStartRange;
	    levelRange->m_rangeEnd   = (long)m_endExpRange;
		//WARNING: C4244 --> conversion from float to long, possible loss of data

        levelRange->m_levelTitle = m_levelTitle;
		levelRange->m_menuID     = m_menuID;
		levelRange->m_profficiencyBonus = m_proficiency;
		levelRange->m_perfectiveBonus = m_perfective;

		UpdateData(FALSE);
		InListLevels(skillLevelSel);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnBUTTONDeleteLevel() 
{
	int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

	int skillLevelSel = m_skillLevelsList.GetCurSel();
    if(skillLevelSel == -1)
	{
	   MessageBox("Must select a skill Level.");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
    CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

	if(!skPtr->m_levelRangeDB)
		return;

	POSITION lvlPos = skPtr->m_levelRangeDB->FindIndex(skillLevelSel);
	if(!lvlPos)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;

	CLevelRangeObject * levelRange = (CLevelRangeObject *)skPtr->m_levelRangeDB->GetAt(lvlPos);
	levelRange->SafeDelete();
	delete levelRange;
	levelRange = 0;
	skPtr->m_levelRangeDB->RemoveAt(lvlPos);
	
	InListLevels(skillLevelSel-1);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnSelchangeLISTSkillLevelsDB() 
{
	int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
       return;
	}

	int skillLevelSel = m_skillLevelsList.GetCurSel();
    if(skillLevelSel == -1)
	{
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		if(!skPtr->m_levelRangeDB)
			return;


		POSITION lvlPos = skPtr->m_levelRangeDB->FindIndex(skillLevelSel);
		if(!lvlPos)
			return;
		CLevelRangeObject * levelRange = (CLevelRangeObject *)skPtr->m_levelRangeDB->GetAt(lvlPos);

		UpdateData(TRUE);
      
		/*m_expStartRange   = levelRange->m_rangeStart;
	    m_endExpRange     = levelRange->m_rangeEnd;*/

		m_expStartRange   = (float)levelRange->m_rangeStart;
	    m_endExpRange     = (float)levelRange->m_rangeEnd;
		//WARNING: C4244 --> conversion from long to float, possible loss of data

        m_levelTitle      = levelRange->m_levelTitle;
		m_menuID          = levelRange->m_menuID;
		m_proficiency     = levelRange->m_profficiencyBonus;
		m_perfective      = levelRange->m_perfectiveBonus;

		UpdateData(FALSE);
		InListAbilities(0);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::InListAbilities(int sel)
{
	m_abilitiesList.ResetContent();
    int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
       return;
	}

	int skillLevelSel = m_skillLevelsList.GetCurSel();
    if(skillLevelSel == -1)
	{
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		if(!skPtr->m_levelRangeDB)
			return;
	    
		POSITION lvlPos = skPtr->m_levelRangeDB->FindIndex(skillLevelSel);
		if(!lvlPos)
			return;
		CLevelRangeObject * levelRange = (CLevelRangeObject *)skPtr->m_levelRangeDB->GetAt(lvlPos);

        if(!levelRange->m_abilityDB)
			return;
        
		for(posLoc = levelRange->m_abilityDB->GetHeadPosition();posLoc != NULL;)
		{
            CSkillAbilityObject * aPtr = (CSkillAbilityObject *)levelRange->m_abilityDB->GetNext(posLoc);
            m_abilitiesList.AddString(aPtr->m_abilityName);
		}

		if(sel > -1)
	       m_abilitiesList.SetCurSel(sel);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnBUTTONADDAbility() 
{
	int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

	int skillLevelSel = m_skillLevelsList.GetCurSel();
    if(skillLevelSel == -1)
	{
	   MessageBox("Must select a level");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		if(!skPtr->m_levelRangeDB)
			return;
	    
		POSITION lvlPos = skPtr->m_levelRangeDB->FindIndex(skillLevelSel);
		if(!lvlPos)
			return;
		CLevelRangeObject * levelRange = (CLevelRangeObject *)skPtr->m_levelRangeDB->GetAt(lvlPos);

        if(!levelRange->m_abilityDB)
			levelRange->m_abilityDB = new CSkillAbilityList;
        

		CSkillAbilityObject * ability = new CSkillAbilityObject();
		UpdateData(TRUE);
		ability->m_functionType = m_cmboAbilityFunction.GetCurSel();
        if(ability->m_functionType < 0)
		   ability->m_functionType = 0;

	    ability->m_abilityName       = m_abilityName;
        ability->m_masterLevel       = m_abilityMasteredExp;
		ability->m_minimumLevelToUse = m_abilityMinimumLevel;
		ability->m_miscInt           = m_abilityMiscInt;
		ability->m_miscInt2          = m_miscInt2;
		ability->m_miscInt3          = m_miscInt3;
		//ability->m_miscInt2
		
		levelRange->m_abilityDB->AddTail(ability);
		UpdateData(FALSE);
		InListAbilities(levelRange->m_abilityDB->GetCount()-1);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnBUTTONDeleteAbility() 
{
	int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

	int skillLevelSel = m_skillLevelsList.GetCurSel();
    if(skillLevelSel == -1)
	{
	   MessageBox("Must select a level");
       return;
	}

	int abilitySelection = m_abilitiesList.GetCurSel();
	if(abilitySelection == -1)
	{
	   MessageBox("Must select a ability");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
    CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

	if(!skPtr->m_levelRangeDB)
		return;
    
	POSITION lvlPos = skPtr->m_levelRangeDB->FindIndex(skillLevelSel);
	if(!lvlPos)
		return;
	CLevelRangeObject * levelRange = (CLevelRangeObject *)skPtr->m_levelRangeDB->GetAt(lvlPos);

    if(!levelRange->m_abilityDB)
		return;
    
    POSITION aPos = levelRange->m_abilityDB->FindIndex(abilitySelection);
	if(!aPos)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CSkillAbilityObject * ability = (CSkillAbilityObject *)levelRange->m_abilityDB->GetAt(aPos);
    ability->SafeDelete();
	delete ability;
	ability = 0;
	levelRange->m_abilityDB->RemoveAt(aPos);
	InListAbilities(abilitySelection-1);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnBUTTONReplaceAbility() 
{
	int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

	int skillLevelSel = m_skillLevelsList.GetCurSel();
    if(skillLevelSel == -1)
	{
	   MessageBox("Must select a level");
       return;
	}

	int abilitySelection = m_abilitiesList.GetCurSel();
	if(abilitySelection == -1)
	{
	   MessageBox("Must select a ability");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		if(!skPtr->m_levelRangeDB)
			return;
	    
		POSITION lvlPos = skPtr->m_levelRangeDB->FindIndex(skillLevelSel);
		if(!lvlPos)
			return;
		CLevelRangeObject * levelRange = (CLevelRangeObject *)skPtr->m_levelRangeDB->GetAt(lvlPos);

        if(!levelRange->m_abilityDB)
			return;
        
        POSITION aPos = levelRange->m_abilityDB->FindIndex(abilitySelection);
		if(!aPos)
			return;
		CSkillAbilityObject * ability = (CSkillAbilityObject *)levelRange->m_abilityDB->GetAt(aPos);
         
		UpdateData(TRUE);
		ability->m_functionType = m_cmboAbilityFunction.GetCurSel();
        if(ability->m_functionType < 0)
		   ability->m_functionType = 0;

	    ability->m_abilityName       = m_abilityName;
        ability->m_masterLevel       = m_abilityMasteredExp;
		ability->m_minimumLevelToUse = m_abilityMinimumLevel;
		ability->m_miscInt           = m_abilityMiscInt;
		ability->m_miscInt2          = m_miscInt2;
		ability->m_miscInt3          = m_miscInt3;
		ability->m_effectBind        = m_effectBind;
		ability->m_miscAmount        = m_miscAmount;
		
		UpdateData(FALSE);

		InListAbilities(abilitySelection);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnSelchangeLISTAbilityDB() 
{
	int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
       return;
	}

	int skillLevelSel = m_skillLevelsList.GetCurSel();
    if(skillLevelSel == -1)
	{
       return;
	}

	int abilitySelection = m_abilitiesList.GetCurSel();
	if(abilitySelection == -1)
	{
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		if(!skPtr->m_levelRangeDB)
			return;
	    
		POSITION lvlPos = skPtr->m_levelRangeDB->FindIndex(skillLevelSel);
		if(!lvlPos)
			return;
		CLevelRangeObject * levelRange = (CLevelRangeObject *)skPtr->m_levelRangeDB->GetAt(lvlPos);

        if(!levelRange->m_abilityDB)
			return;
        
        POSITION aPos = levelRange->m_abilityDB->FindIndex(abilitySelection);
		if(!aPos)
			return;
		CSkillAbilityObject * ability = (CSkillAbilityObject *)levelRange->m_abilityDB->GetAt(aPos);
         
		UpdateData(TRUE);
		m_cmboAbilityFunction.SetCurSel(ability->m_functionType);
	    m_abilityName                = ability->m_abilityName;
        m_abilityMasteredExp         = ability->m_masterLevel;
		m_abilityMinimumLevel        = ability->m_minimumLevelToUse;
		m_abilityMiscInt             = ability->m_miscInt;
		m_miscInt2                   = ability->m_miscInt2;
		m_miscInt3                   = ability->m_miscInt3;
		m_effectBind                 = ability->m_effectBind;
		m_miscAmount                 = ability->m_miscAmount;
		
		UpdateData(FALSE);

        InListResultItems(0);
		InListRequiredItems(0);
		InListStatReq(0);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnBUTTONAddStatReq() 
{
    int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

	int skillLevelSel = m_skillLevelsList.GetCurSel();
    if(skillLevelSel == -1)
	{
	   MessageBox("Must select a level");
       return;
	}

	int abilitySelection = m_abilitiesList.GetCurSel();
	if(abilitySelection == -1)
	{
	   MessageBox("Must select a ability");
       return;
	}

	int statRefSelection = m_statRefList.GetCurSel();
	if(statRefSelection == -1)
	{
	   MessageBox("Must select a stat reference");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		if(!skPtr->m_levelRangeDB)
			return;
	    
		POSITION lvlPos = skPtr->m_levelRangeDB->FindIndex(skillLevelSel);
		if(!lvlPos)
			return;
		CLevelRangeObject * levelRange = (CLevelRangeObject *)skPtr->m_levelRangeDB->GetAt(lvlPos);

        if(!levelRange->m_abilityDB)
			return;
        
        POSITION aPos = levelRange->m_abilityDB->FindIndex(abilitySelection);
		if(!aPos)
			return;
		CSkillAbilityObject * ability = (CSkillAbilityObject *)levelRange->m_abilityDB->GetAt(aPos);
		if(!ability->m_requiredStatistics)
			ability->m_requiredStatistics = new CStatObjList;


        POSITION sRPOS = m_statDatabaseRef->FindIndex(statRefSelection);
        if(!sRPOS)
			return;
		CStatObj * statPtr = (CStatObj *)m_statDatabaseRef->GetAt(sRPOS);

        CStatObj * newStatReq = NULL;
		statPtr->Clone(&newStatReq);
        UpdateData(TRUE);
		newStatReq->m_currentValue = m_statAmount;
        UpdateData(FALSE);
        ability->m_requiredStatistics->AddTail(newStatReq);
		InListStatReq(ability->m_requiredStatistics->GetCount()-1);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::InListStatReq(int sel)
{
	m_abilityRequiredStatsList.ResetContent();
     int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

	int skillLevelSel = m_skillLevelsList.GetCurSel();
    if(skillLevelSel == -1)
	{
	   MessageBox("Must select a level");
       return;
	}

	int abilitySelection = m_abilitiesList.GetCurSel();
	if(abilitySelection == -1)
	{
	   MessageBox("Must select a ability");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		if(!skPtr->m_levelRangeDB)
			return;
	    
		POSITION lvlPos = skPtr->m_levelRangeDB->FindIndex(skillLevelSel);
		if(!lvlPos)
			return;
		CLevelRangeObject * levelRange = (CLevelRangeObject *)skPtr->m_levelRangeDB->GetAt(lvlPos);

        if(!levelRange->m_abilityDB)
			return;
        
        POSITION aPos = levelRange->m_abilityDB->FindIndex(abilitySelection);
		if(!aPos)
			return;
		CSkillAbilityObject * ability = (CSkillAbilityObject *)levelRange->m_abilityDB->GetAt(aPos);
		if(!ability->m_requiredStatistics)
			return;

        for(posLoc = ability->m_requiredStatistics->GetHeadPosition();posLoc != NULL;)
		{
            CStatObj * statPtr = (CStatObj *) ability->m_requiredStatistics->GetNext(posLoc);
			m_abilityRequiredStatsList.AddString(statPtr->m_statName);
		}

		if(sel > -1)
		   m_abilityRequiredStatsList.SetCurSel(sel);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnBUTTONDeleteStatReq() 
{
    int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

	int skillLevelSel = m_skillLevelsList.GetCurSel();
    if(skillLevelSel == -1)
	{
	   MessageBox("Must select a level");
       return;
	}

	int abilitySelection = m_abilitiesList.GetCurSel();
	if(abilitySelection == -1)
	{
	   MessageBox("Must select a ability");
       return;
	}

	int requiresStatSel = m_abilityRequiredStatsList.GetCurSel();
	if(requiresStatSel == -1)
	{
	   MessageBox("Must select a required stat");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
    CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

	if(!skPtr->m_levelRangeDB)
		return;
    
	POSITION lvlPos = skPtr->m_levelRangeDB->FindIndex(skillLevelSel);
	if(!lvlPos)
		return;
	CLevelRangeObject * levelRange = (CLevelRangeObject *)skPtr->m_levelRangeDB->GetAt(lvlPos);

    if(!levelRange->m_abilityDB)
		return;
    
    POSITION aPos = levelRange->m_abilityDB->FindIndex(abilitySelection);
	if(!aPos)
		return;
	CSkillAbilityObject * ability = (CSkillAbilityObject *)levelRange->m_abilityDB->GetAt(aPos);
	if(!ability->m_requiredStatistics)
		return;

    posLoc = ability->m_requiredStatistics->FindIndex(requiresStatSel);
	if(!posLoc)
		return;
    
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CStatObj * statPtr = (CStatObj *) ability->m_requiredStatistics->GetAt(posLoc);
	delete statPtr;
	statPtr = 0;
	ability->m_requiredStatistics->RemoveAt(posLoc);

    InListStatReq(requiresStatSel-1);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::InListRequiredItems(int sel)
{
	m_abilityRequiredItemsList.ResetContent();
    int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

	int skillLevelSel = m_skillLevelsList.GetCurSel();
    if(skillLevelSel == -1)
	{
	   MessageBox("Must select a level");
       return;
	}

	int abilitySelection = m_abilitiesList.GetCurSel();
	if(abilitySelection == -1)
	{
	   MessageBox("Must select a ability");
       return;
	}

	int glInventorySel = m_globalInventory.GetCurSel();
	if(glInventorySel == -1)
	{
	   MessageBox("Must select a global Inventory Obj");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		if(!skPtr->m_levelRangeDB)
			return;
	    
		POSITION lvlPos = skPtr->m_levelRangeDB->FindIndex(skillLevelSel);
		if(!lvlPos)
			return;
		CLevelRangeObject * levelRange = (CLevelRangeObject *)skPtr->m_levelRangeDB->GetAt(lvlPos);

        if(!levelRange->m_abilityDB)
			return;
        
        POSITION aPos = levelRange->m_abilityDB->FindIndex(abilitySelection);
		if(!aPos)
			return;
		CSkillAbilityObject * ability = (CSkillAbilityObject *)levelRange->m_abilityDB->GetAt(aPos);


        if(!ability->m_requiredItems)
            return;


		for(posLoc = ability->m_requiredItems->GetHeadPosition();posLoc != NULL;)
		{
            CInventoryObj * invPtr = (CInventoryObj *)ability->m_requiredItems->GetNext(posLoc);

            m_abilityRequiredItemsList.AddString(invPtr->m_itemData->m_itemName);
		}

		if(sel>-1)
		   m_abilityRequiredItemsList.SetCurSel(sel);
		
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::InListResultItems(int sel)
{
	m_abilityResultItemsList.ResetContent();
    int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

	int skillLevelSel = m_skillLevelsList.GetCurSel();
    if(skillLevelSel == -1)
	{
	   MessageBox("Must select a level");
       return;
	}

	int abilitySelection = m_abilitiesList.GetCurSel();
	if(abilitySelection == -1)
	{
	   MessageBox("Must select a ability");
       return;
	}

	int glInventorySel = m_globalInventory.GetCurSel();
	if(glInventorySel == -1)
	{
	   MessageBox("Must select a global Inventory Obj");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		if(!skPtr->m_levelRangeDB)
			return;
	    
		POSITION lvlPos = skPtr->m_levelRangeDB->FindIndex(skillLevelSel);
		if(!lvlPos)
			return;
		CLevelRangeObject * levelRange = (CLevelRangeObject *)skPtr->m_levelRangeDB->GetAt(lvlPos);

        if(!levelRange->m_abilityDB)
			return;
        
        POSITION aPos = levelRange->m_abilityDB->FindIndex(abilitySelection);
		if(!aPos)
			return;
		CSkillAbilityObject * ability = (CSkillAbilityObject *)levelRange->m_abilityDB->GetAt(aPos);


        if(!ability->m_resultItems)
            return;


		for(posLoc = ability->m_resultItems->GetHeadPosition();posLoc != NULL;)
		{
            CInventoryObj * invPtr = (CInventoryObj *)ability->m_resultItems->GetNext(posLoc);

            m_abilityResultItemsList.AddString(invPtr->m_itemData->m_itemName);
		}

		if(sel>-1)
		   m_abilityResultItemsList.SetCurSel(sel);

		
		
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnBUTTONAddRequiredItem() 
{
	int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

	int skillLevelSel = m_skillLevelsList.GetCurSel();
    if(skillLevelSel == -1)
	{
	   MessageBox("Must select a level");
       return;
	}

	int abilitySelection = m_abilitiesList.GetCurSel();
	if(abilitySelection == -1)
	{
	   MessageBox("Must select a ability");
       return;
	}

	int glInventorySel = m_globalInventory.GetCurSel();
	if(glInventorySel == -1)
	{
	   MessageBox("Must select a global Inventory Obj");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		if(!skPtr->m_levelRangeDB)
			return;
	    
		POSITION lvlPos = skPtr->m_levelRangeDB->FindIndex(skillLevelSel);
		if(!lvlPos)
			return;
		CLevelRangeObject * levelRange = (CLevelRangeObject *)skPtr->m_levelRangeDB->GetAt(lvlPos);

        if(!levelRange->m_abilityDB)
			return;
        
        POSITION aPos = levelRange->m_abilityDB->FindIndex(abilitySelection);
		if(!aPos)
			return;
		CSkillAbilityObject * ability = (CSkillAbilityObject *)levelRange->m_abilityDB->GetAt(aPos);


        if(!ability->m_requiredItems)
            ability->m_requiredItems = new CInventoryObjList();

 
		CString itemName;
		m_globalInventory.GetWindowTextA(itemName);
		GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;

		CItemObj * itemPtr = gStatus->m_itemManager->GetItemByName(itemName.GetBuffer());
		if(!itemPtr)
			return;

        CInventoryObj * invObj = new CInventoryObj();
	    itemPtr->Clone(&invObj->m_itemData);
	    invObj->m_quanity = 1;
	    invObj->m_armed   = FALSE;
	    ability->m_requiredItems->AddTail(invObj);

		//ability->m_requiredItems->AddInventoryItem(1,FALSE,glPtr->m_itemData);

	    InListRequiredItems(ability->m_requiredItems->GetCount()-1);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnBUTTONAddResultItem() 
{
	int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

	int skillLevelSel = m_skillLevelsList.GetCurSel();
    if(skillLevelSel == -1)
	{
	   MessageBox("Must select a level");
       return;
	}

	int abilitySelection = m_abilitiesList.GetCurSel();
	if(abilitySelection == -1)
	{
	   MessageBox("Must select a ability");
       return;
	}

	int glInventorySel = m_globalInventory.GetCurSel();
	if(glInventorySel == -1)
	{
	   MessageBox("Must select a global Inventory Obj");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		if(!skPtr->m_levelRangeDB)
			return;
	    
		POSITION lvlPos = skPtr->m_levelRangeDB->FindIndex(skillLevelSel);
		if(!lvlPos)
			return;
		CLevelRangeObject * levelRange = (CLevelRangeObject *)skPtr->m_levelRangeDB->GetAt(lvlPos);

        if(!levelRange->m_abilityDB)
			return;
        
        POSITION aPos = levelRange->m_abilityDB->FindIndex(abilitySelection);
		if(!aPos)
			return;
		CSkillAbilityObject * ability = (CSkillAbilityObject *)levelRange->m_abilityDB->GetAt(aPos);


        if(!ability->m_resultItems)
            ability->m_resultItems = new CInventoryObjList();


		CString itemName;
		m_globalInventory.GetWindowTextA(itemName);
		GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;

		CItemObj * itemPtr = gStatus->m_itemManager->GetItemByName(itemName.GetBuffer());
		if(!itemPtr)
			return;
		

		ability->m_resultItems->AddInventoryItem(1,FALSE,itemPtr);
		InListResultItems(ability->m_resultItems->GetCount()-1);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnBUTTONDeleteRequiredItem() 
{
	int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

	int skillLevelSel = m_skillLevelsList.GetCurSel();
    if(skillLevelSel == -1)
	{
	   MessageBox("Must select a level");
       return;
	}

	int abilitySelection = m_abilitiesList.GetCurSel();
	if(abilitySelection == -1)
	{
	   MessageBox("Must select a ability");
       return;
	}

	int requiredItemSel = m_abilityRequiredItemsList.GetCurSel();
	if(requiredItemSel == -1)
	{
	   MessageBox("Must select a required item");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		if(!skPtr->m_levelRangeDB)
			return;
	    
		POSITION lvlPos = skPtr->m_levelRangeDB->FindIndex(skillLevelSel);
		if(!lvlPos)
			return;
		CLevelRangeObject * levelRange = (CLevelRangeObject *)skPtr->m_levelRangeDB->GetAt(lvlPos);

        if(!levelRange->m_abilityDB)
			return;
        
        POSITION aPos = levelRange->m_abilityDB->FindIndex(abilitySelection);
		if(!aPos)
			return;
		CSkillAbilityObject * ability = (CSkillAbilityObject *)levelRange->m_abilityDB->GetAt(aPos);


        if(!ability->m_requiredItems)
            return;


		POSITION gPos = ability->m_requiredItems->FindIndex(requiredItemSel);
		if(!gPos)
			return;

		if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
			return;
		
		CInventoryObj * invPtr = (CInventoryObj *)ability->m_requiredItems->GetAt(gPos);
		invPtr->SafeDelete();
		delete invPtr;
		invPtr = 0;

		ability->m_requiredItems->RemoveAt(gPos);
		InListRequiredItems(requiredItemSel-1);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnBUTTONDeleteResultItem() 
{
	int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

	int skillLevelSel = m_skillLevelsList.GetCurSel();
    if(skillLevelSel == -1)
	{
	   MessageBox("Must select a level");
       return;
	}

	int abilitySelection = m_abilitiesList.GetCurSel();
	if(abilitySelection == -1)
	{
	   MessageBox("Must select a ability");
       return;
	}

	int resultItemSel = m_abilityResultItemsList.GetCurSel();
	if(resultItemSel == -1)
	{
	   MessageBox("Must select a result item");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		if(!skPtr->m_levelRangeDB)
			return;
	    
		POSITION lvlPos = skPtr->m_levelRangeDB->FindIndex(skillLevelSel);
		if(!lvlPos)
			return;
		CLevelRangeObject * levelRange = (CLevelRangeObject *)skPtr->m_levelRangeDB->GetAt(lvlPos);

        if(!levelRange->m_abilityDB)
			return;
        
        POSITION aPos = levelRange->m_abilityDB->FindIndex(abilitySelection);
		if(!aPos)
			return;
		CSkillAbilityObject * ability = (CSkillAbilityObject *)levelRange->m_abilityDB->GetAt(aPos);


        if(!ability->m_resultItems)
            return;


		POSITION gPos = ability->m_resultItems->FindIndex(resultItemSel);
		if(!gPos)
			return;

		if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
			return;
		
		CInventoryObj * invPtr = (CInventoryObj *)ability->m_resultItems->GetAt(gPos);
		invPtr->SafeDelete();
		delete invPtr;
		invPtr = 0;

		ability->m_resultItems->RemoveAt(gPos);
		InListResultItems(resultItemSel-1);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnBUTTONLoadSys() 
{
	 //use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization

   static char BASED_CODE filter[] = "Skill Files|*.SKL|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
	   SetCurrentDirectory(MainDir);
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	   if(res == FALSE){
		  SetCurrentDirectory(MainDir);
		  return;
	   }
	   CArchive the_in_Archive(&fl,CArchive::load);
	   if(m_skillDatabaseRef)
	   {
        m_skillDatabaseRef->SafeDelete();
		delete m_skillDatabaseRef;
		m_skillDatabaseRef=0;
	   }
	   the_in_Archive >> m_skillDatabaseRef;
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InListSkills(0);
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnBUTTONSaveSkillSys() 
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "SKL Files|*.SKL|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "SKL", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
       if(res == FALSE){
		  SetCurrentDirectory(MainDir);
		  MessageBox(fileName);
	      return;
	   }
       CArchive the_out_Archive(&fl,CArchive::store);
       the_out_Archive << m_skillDatabaseRef;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);  
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnSelchangeLISTrequiredStats() 
{
	int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

	int skillLevelSel = m_skillLevelsList.GetCurSel();
    if(skillLevelSel == -1)
	{
	   MessageBox("Must select a level");
       return;
	}

	int abilitySelection = m_abilitiesList.GetCurSel();
	if(abilitySelection == -1)
	{
	   MessageBox("Must select a ability");
       return;
	}

	int requiresStatSel = m_abilityRequiredStatsList.GetCurSel();
	if(requiresStatSel == -1)
	{
	   MessageBox("Must select a required stat");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		if(!skPtr->m_levelRangeDB)
			return;
	    
		POSITION lvlPos = skPtr->m_levelRangeDB->FindIndex(skillLevelSel);
		if(!lvlPos)
			return;
		CLevelRangeObject * levelRange = (CLevelRangeObject *)skPtr->m_levelRangeDB->GetAt(lvlPos);

        if(!levelRange->m_abilityDB)
			return;
        
        POSITION aPos = levelRange->m_abilityDB->FindIndex(abilitySelection);
		if(!aPos)
			return;
		CSkillAbilityObject * ability = (CSkillAbilityObject *)levelRange->m_abilityDB->GetAt(aPos);
		if(!ability->m_requiredStatistics)
			return;

        posLoc = ability->m_requiredStatistics->FindIndex(requiresStatSel);
		if(!posLoc)
			return;
            CStatObj * statPtr = (CStatObj *) ability->m_requiredStatistics->GetAt(posLoc);
			UpdateData(TRUE);
			m_statAmount = statPtr->m_currentValue;
			UpdateData(FALSE);
	
}
//---------------------------------------//
//---------------------------------------//
//---------------------------------------//
void CSkillSystemDlg::OnBUTTONReplaceStatReq() 
{
	int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

	int skillLevelSel = m_skillLevelsList.GetCurSel();
    if(skillLevelSel == -1)
	{
	   MessageBox("Must select a level");
       return;
	}

	int abilitySelection = m_abilitiesList.GetCurSel();
	if(abilitySelection == -1)
	{
	   MessageBox("Must select a ability");
       return;
	}

	int requiresStatSel = m_abilityRequiredStatsList.GetCurSel();
	if(requiresStatSel == -1)
	{
	   MessageBox("Must select a required stat");
       return;
	}

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		if(!skPtr->m_levelRangeDB)
			return;
	    
		POSITION lvlPos = skPtr->m_levelRangeDB->FindIndex(skillLevelSel);
		if(!lvlPos)
			return;
		CLevelRangeObject * levelRange = (CLevelRangeObject *)skPtr->m_levelRangeDB->GetAt(lvlPos);

        if(!levelRange->m_abilityDB)
			return;
        
        POSITION aPos = levelRange->m_abilityDB->FindIndex(abilitySelection);
		if(!aPos)
			return;
		CSkillAbilityObject * ability = (CSkillAbilityObject *)levelRange->m_abilityDB->GetAt(aPos);
		if(!ability->m_requiredStatistics)
			return;

        posLoc = ability->m_requiredStatistics->FindIndex(requiresStatSel);
		if(!posLoc)
			return;
            CStatObj * statPtr = (CStatObj *) ability->m_requiredStatistics->GetAt(posLoc);
			UpdateData(TRUE);
			statPtr->m_currentValue = m_statAmount;
			UpdateData(FALSE);
}

void CSkillSystemDlg::OnBUTTONInsertLevel() 
{
	int skillSelection = m_skillsList.GetCurSel();
    if(skillSelection == -1)
	{
	   MessageBox("Must select a skill.");
       return;
	}

	int skillLevelSel = m_skillLevelsList.GetCurSel();
    if(skillLevelSel == -1)
	{
	   MessageBox("Must select a level");
       return;
	}

	

    POSITION posLoc = m_skillDatabaseRef->FindIndex(skillSelection);
    if(!posLoc)
		return;
        CSkillObject * skPtr = (CSkillObject*)m_skillDatabaseRef->GetAt(posLoc);

		POSITION posMark = skPtr->m_levelRangeDB->FindIndex(skillLevelSel);
        if(!posMark)
		{
           MessageBox("Invalid selection");
           return;
		}

		if(!skPtr->m_levelRangeDB)
			skPtr->m_levelRangeDB = new CLevelRangeList();


		CLevelRangeObject * levelRange = new CLevelRangeObject();
		UpdateData(TRUE);

		/*levelRange->m_rangeStart = m_expStartRange;
	    levelRange->m_rangeEnd   = m_endExpRange;*/

		levelRange->m_rangeStart = (long)m_expStartRange;
	    levelRange->m_rangeEnd   = (long)m_endExpRange;
		//WARNING: C4244 --> conversion from float to long, possible loss of data

        levelRange->m_levelTitle = m_levelTitle;
		levelRange->m_menuID     = m_menuID;
		
        

		skPtr->m_levelRangeDB->InsertAfter(posMark,levelRange);
		UpdateData(FALSE);
		InListLevels(skPtr->m_levelRangeDB->GetCount()-1);
}

void CSkillSystemDlg::OnBnClickedButtonExporttosql()
{
	CSqlConnectionDlg dlg;
     if(dlg.DoModal() == IDOK)
	 {
        if(!m_skillDatabaseRef->ExportToSQL(dlg.m_ipAddress, dlg.m_database, dlg.m_username, dlg.m_password))
		    AfxMessageBox("Failed",0,0);
		else
			AfxMessageBox("Completed",0,0);
	 }
	//m_skillDatabaseRef->ExportToSQL();
}
