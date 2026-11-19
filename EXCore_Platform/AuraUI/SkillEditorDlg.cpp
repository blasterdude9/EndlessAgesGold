// SkillEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SkillEditorDlg.h"
#include "AbilityEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSkillEditorDlg dialog


CSkillEditorDlg::CSkillEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSkillEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSkillEditorDlg)
	m_currentValue = 0.0f;
	m_name = _T("");
	m_skillCap = 0.0f;
	m_type = 0;
	m_statCap = 0.0f;
	m_statName = _T("");
	m_statType = 0;
	m_statCurrentAmount = 0.0f;
	m_statImprovement = 0.0f;
	m_statImprovementCap = 0.0f;
	m_ImprovementOnSuccess = 0.0f;
	//}}AFX_DATA_INIT
}


void CSkillEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSkillEditorDlg)
	DDX_Control(pDX, IDC_LIST_STAT_DependancyList, m_statDependancyList);
	DDX_Control(pDX, IDC_LIST_Statistics, m_statisticList);
	DDX_Control(pDX, IDC_LIST_SkillList, m_skillList);
	DDX_Text(pDX, IDC_EDIT_currentValue, m_currentValue);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDX_Text(pDX, IDC_EDIT_skillCap, m_skillCap);
	DDX_Text(pDX, IDC_EDIT_type, m_type);
	DDX_Text(pDX, IDC_EDIT_statCap, m_statCap);
	DDX_Text(pDX, IDC_EDIT_statName, m_statName);
	DDX_Text(pDX, IDC_EDIT_statType, m_statType);
	DDX_Text(pDX, IDC_EDIT_currentAmount, m_statCurrentAmount);
	DDX_Text(pDX, IDC_EDIT_StatImprovement, m_statImprovement);
	DDX_Text(pDX, IDC_EDIT_StatImprovementCap, m_statImprovementCap);
	DDX_Text(pDX, IDC_EDIT_ImprovementOnSuccess, m_ImprovementOnSuccess);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSkillEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CSkillEditorDlg)
	ON_BN_CLICKED(IDC_BUTTON_AbilityEditor, OnBUTTONAbilityEditor)
	ON_BN_CLICKED(IDC_BUTTON_AddSkill, OnBUTTONAddSkill)
	ON_BN_CLICKED(IDC_BUTTON_DeleteSkill, OnBUTTONDeleteSkill)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceValues, OnBUTTONReplaceValues)
	ON_LBN_SELCHANGE(IDC_LIST_SkillList, OnSelchangeLISTSkillList)
	ON_BN_CLICKED(IDC_BUTTON_AddStatistic, OnBUTTONAddStatistic)
	ON_BN_CLICKED(IDC_BUTTON_DeleteStatistic, OnBUTTONDeleteStatistic)
	ON_BN_CLICKED(IDC_BUTTON_StatReplaceValues, OnBUTTONStatReplaceValues)
	ON_LBN_SELCHANGE(IDC_LIST_Statistics, OnSelchangeLISTStatistics)
	ON_BN_CLICKED(IDC_BUTTON_AddStatDependancy, OnBUTTONAddStatDependancy)
	ON_BN_CLICKED(IDC_BUTTON_RemoveStatDependancy, OnBUTTONRemoveStatDependancy)
	ON_LBN_SELCHANGE(IDC_LIST_STAT_DependancyList, OnSelchangeLISTSTATDependancyList)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceDependancyValues, OnBUTTONReplaceDependancyValues)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSkillEditorDlg message handlers


//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CSkillEditorDlg::OnBUTTONAddSkill() 
{
if(!charDefinitionRef)
    charDefinitionRef = new CharacterDefObject();

UpdateData(TRUE);	
	charDefinitionRef->AddSkill(m_name,m_type,m_currentValue,m_skillCap);
	
UpdateData(FALSE);
InList(-1);	
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CSkillEditorDlg::OnBUTTONDeleteSkill() 
{
	if(!charDefinitionRef)
		return;

	if(!charDefinitionRef->m_skills)
		return;

	int skillSel = m_skillList.GetCurSel();
	if(skillSel ==-1)
	{
		AfxMessageBox("Must select a skill");
		return;
	}

	POSITION sklPos = charDefinitionRef->m_skills->FindIndex(skillSel);
	if(!sklPos)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CBaseSkillObject * skillPtr = (CBaseSkillObject *)charDefinitionRef->m_skills->GetAt(sklPos);
	skillPtr->SafeDelete();
	delete skillPtr;
	skillPtr=0;
	charDefinitionRef->m_skills->RemoveAt(sklPos);
	InList((skillSel-1));
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CSkillEditorDlg::OnBUTTONReplaceValues() 
{
	if(!charDefinitionRef)
		return;

	if(!charDefinitionRef->m_skills)
		return;

	int skillSel = m_skillList.GetCurSel();
	if(skillSel ==-1){
       AfxMessageBox("Must select a skill");
	   return;
	}

    POSITION sklPos = charDefinitionRef->m_skills->FindIndex(skillSel);
    if(!sklPos)
		return;
	CBaseSkillObject * skillPtr = (CBaseSkillObject *)charDefinitionRef->m_skills->GetAt(sklPos);
	UpdateData(TRUE);


    skillPtr->m_skillName = m_name;
	skillPtr->m_skillType = m_type;
	skillPtr->m_currentSkillValue = m_currentValue;
	skillPtr->m_skillCap = m_skillCap;

    UpdateData(FALSE);
    InList(skillSel);
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CSkillEditorDlg::InList(int sel) 
{
	UpdateData(TRUE);
    if(!charDefinitionRef)
		return;

	if(!charDefinitionRef->m_skills)
		return;
	m_skillList.ResetContent();
    for(POSITION posLoc = charDefinitionRef->m_skills->GetHeadPosition();posLoc != NULL;){
		CBaseSkillObject * skillPtr = (CBaseSkillObject *)charDefinitionRef->m_skills->GetNext(posLoc);
        m_skillList.AddString(skillPtr->m_skillName);
	}

	if(sel >= 0)
	   m_skillList.SetCurSel(sel);

	InListSkillDependancies(-1);
    UpdateData(FALSE);
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
BOOL CSkillEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	
	InList(-1);
	InListStatistics(-1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CSkillEditorDlg::OnSelchangeLISTSkillList() 
{
    if(!charDefinitionRef)
		return;

	if(!charDefinitionRef->m_skills)
		return;

	int skillSel = m_skillList.GetCurSel();
	if(skillSel ==-1){
	   return;
	}

    POSITION sklPos = charDefinitionRef->m_skills->FindIndex(skillSel);
    if(!sklPos)
		return;
	CBaseSkillObject * skillPtr = (CBaseSkillObject *)charDefinitionRef->m_skills->GetAt(sklPos);
	UpdateData(TRUE);


    m_name = skillPtr->m_skillName;
	m_type = skillPtr->m_skillType;
	m_currentValue = skillPtr->m_currentSkillValue;
	m_skillCap = skillPtr->m_skillCap;

    UpdateData(FALSE);
    InList(skillSel);
	
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CSkillEditorDlg::OnBUTTONAbilityEditor() 
{
	if(!charDefinitionRef)
		return;

	if(!charDefinitionRef->m_skills)
		return;

	int skillSel = m_skillList.GetCurSel();
	if(skillSel ==-1){
       AfxMessageBox("Must select a skill");
	   return;
	}

    POSITION sklPos = charDefinitionRef->m_skills->FindIndex(skillSel);
    if(!sklPos)
		return;
	    CBaseSkillObject * skillPtr = (CBaseSkillObject *)charDefinitionRef->m_skills->GetAt(sklPos);
        
        CAbilityEditorDlg dlg;
		dlg.resourceListRef = resourceListRef;
		dlg.m_refSkillType = skillPtr->m_skillType;
		dlg.charDefinitionRef = charDefinitionRef;
        dlg.DoModal();



}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CSkillEditorDlg::InListStatistics(int sel)
{
    
	if(!charDefinitionRef)
		return;

	if(!charDefinitionRef->m_statistics)
		return;

    UpdateData(TRUE);

	m_statisticList.ResetContent();
    for(POSITION posLoc = charDefinitionRef->m_statistics->GetHeadPosition();posLoc != NULL;){
		CStatisticObject * skillPtr = (CStatisticObject *)charDefinitionRef->m_statistics->GetNext(posLoc);
        m_statisticList.AddString(skillPtr->m_statName);
	}

	if(sel >= 0)
	   m_statisticList.SetCurSel(sel);

    UpdateData(FALSE);


}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CSkillEditorDlg::OnBUTTONAddStatistic() 
{
	if(!charDefinitionRef)
    charDefinitionRef = new CharacterDefObject();

    UpdateData(TRUE);	
	charDefinitionRef->AddStatistic(m_statType,m_statName,m_statCurrentAmount,m_statCap);
    UpdateData(FALSE);
    InListStatistics(charDefinitionRef->m_statistics->GetCount()-1);		
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CSkillEditorDlg::OnBUTTONDeleteStatistic() 
{
	if(!charDefinitionRef)
		return;

	if(!charDefinitionRef->m_statistics)
		return;

	int statSel = m_statisticList.GetCurSel();
	if(statSel == -1)
	   return;

    POSITION sklPos = charDefinitionRef->m_statistics->FindIndex(statSel);
    if(!sklPos)
		return;

	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;

	CStatisticObject * statPtr = (CStatisticObject *)charDefinitionRef->m_statistics->GetAt(sklPos);
	delete statPtr;
	statPtr=0;
	charDefinitionRef->m_statistics->RemoveAt(sklPos);

    InListStatistics(statSel-1);
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CSkillEditorDlg::OnBUTTONStatReplaceValues() 
{
	if(!charDefinitionRef)
		return;

	if(!charDefinitionRef->m_statistics)
		return;

	int statSel = m_statisticList.GetCurSel();
	if(statSel ==-1){
	   return;
	}

    POSITION sklPos = charDefinitionRef->m_statistics->FindIndex(statSel);
    if(!sklPos)
		return;
	CStatisticObject * statPtr = (CStatisticObject *)charDefinitionRef->m_statistics->GetAt(sklPos);
	UpdateData(TRUE);


    statPtr->m_statName      = m_statName;
	statPtr->m_type          = m_statType;
	statPtr->m_currentAmount = m_statCurrentAmount;
	statPtr->m_statCap       = m_statCap;

    UpdateData(FALSE);
    InListStatistics(statSel);
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CSkillEditorDlg::OnSelchangeLISTStatistics() 
{
	if(!charDefinitionRef)
		return;

	if(!charDefinitionRef->m_statistics)
		return;

	int statSel = m_statisticList.GetCurSel();
	if(statSel ==-1){
	   return;
	}

    POSITION sklPos = charDefinitionRef->m_statistics->FindIndex(statSel);
    if(!sklPos)
		return;
	CStatisticObject * statPtr = (CStatisticObject *)charDefinitionRef->m_statistics->GetAt(sklPos);
	UpdateData(TRUE);


    m_statName          = statPtr->m_statName;
	m_statType          = statPtr->m_type;
	m_statCurrentAmount = statPtr->m_currentAmount;
	m_statCap           = statPtr->m_statCap;

    UpdateData(FALSE);
    InListStatistics(statSel);
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CSkillEditorDlg::InListSkillDependancies(int sel)
{
    m_statDependancyList.ResetContent();

    if(!charDefinitionRef)
	   return;

    if(!charDefinitionRef->m_skills)
	   return;

	if(!charDefinitionRef->m_statistics)
		return;

    int skillSel = m_skillList.GetCurSel();
	if(skillSel ==-1){
	   return;
	}

    POSITION sklPos = charDefinitionRef->m_skills->FindIndex(skillSel);
    if(!sklPos)
	   return;
	CBaseSkillObject * skillPtr = (CBaseSkillObject *)charDefinitionRef->m_skills->GetAt(sklPos);
	UpdateData(TRUE);
    
	if(!skillPtr->m_affectedByStatList)
       return;

	for(POSITION posLoc = skillPtr->m_affectedByStatList->GetHeadPosition();posLoc != NULL;)
	{
        CAffectedByStatObject * afObj = (CAffectedByStatObject *)skillPtr->m_affectedByStatList->GetNext(posLoc);
		CStatisticObject * statObj = (CStatisticObject *)charDefinitionRef->m_statistics->GetStatObjectByType(afObj->m_statType);
        if(statObj){
           m_statDependancyList.AddString(statObj->m_statName);
		}
		else{
           m_statDependancyList.AddString("Error 2207: No Reference stat Object");
		}
	}
   
    if(sel >= 0)
	   m_statDependancyList.SetCurSel(sel);
    UpdateData(FALSE);

}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CSkillEditorDlg::OnBUTTONAddStatDependancy() 
{
	
    if(!charDefinitionRef)
	   return;

    if(!charDefinitionRef->m_skills){
	   MessageBox("No current statistics- add one");
	   return;
	}

	if(!charDefinitionRef->m_statistics){
	   MessageBox("No current statistics- add one");
	   return;
	}

	UpdateData(TRUE);
    int skillSel = m_skillList.GetCurSel();
	if(skillSel ==-1){
	   MessageBox("Must select a skill");
	   return;
	}


	int statSel = m_statisticList.GetCurSel();
	if(statSel ==-1){
	   MessageBox("Must select a statistic");
	   return;
	}

    POSITION sklPos = charDefinitionRef->m_skills->FindIndex(skillSel);
    if(!sklPos)
	   return;
	CBaseSkillObject * skillPtr = (CBaseSkillObject *)charDefinitionRef->m_skills->GetAt(sklPos);

	POSITION statPos = charDefinitionRef->m_statistics->FindIndex(statSel);
    if(!statPos)
	   return;
	CStatisticObject * statObj = (CStatisticObject *)charDefinitionRef->m_statistics->GetAt(statPos);
	
    
	if(!skillPtr->m_affectedByStatList)
        skillPtr->m_affectedByStatList = new CAffectedByStatObjectList;

	    CAffectedByStatObject * newAffectStatObj = new CAffectedByStatObject();
		newAffectStatObj->m_statType = statObj->m_type;
		newAffectStatObj->m_statisticalImprovement = m_statImprovement;
		newAffectStatObj->m_statisticalImprovementCap = m_statImprovementCap;
		newAffectStatObj->m_improvementOfSuccessVariable = m_ImprovementOnSuccess;

		skillPtr->m_affectedByStatList->AddTail(newAffectStatObj);

    UpdateData(FALSE);
	InListSkillDependancies(skillPtr->m_affectedByStatList->GetCount());
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CSkillEditorDlg::OnBUTTONRemoveStatDependancy() 
{
	
	
	UpdateData(TRUE);
    int skillSel = m_skillList.GetCurSel();
	if(skillSel ==-1){
	   MessageBox("Must select a skill");
	   return;
	}


	int dependancySel = m_statisticList.GetCurSel();
	if(dependancySel ==-1){
	   MessageBox("Must select a Dependancy Stat");
	   return;
	}
	
	
	if(!charDefinitionRef)
	   return;

    if(!charDefinitionRef->m_skills){
	   return;
	}


    POSITION sklPos = charDefinitionRef->m_skills->FindIndex(skillSel);
    if(!sklPos)
	   return;
	CBaseSkillObject * skillPtr = (CBaseSkillObject *)charDefinitionRef->m_skills->GetAt(sklPos);

    if(!skillPtr->m_affectedByStatList)
		return;

	POSITION afPos = skillPtr->m_affectedByStatList->FindIndex(skillSel);
    if(!afPos)
	   return;
	CAffectedByStatObject * affectedStatPtr = (CAffectedByStatObject *)skillPtr->m_affectedByStatList->GetAt(afPos);
    delete affectedStatPtr;
	affectedStatPtr=0;
    skillPtr->m_affectedByStatList->RemoveAt(afPos);

    UpdateData(FALSE);
	InListSkillDependancies(dependancySel-1);
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CSkillEditorDlg::OnSelchangeLISTSTATDependancyList() 
{

	UpdateData(TRUE);

    int skillSel = m_skillList.GetCurSel();
	if(skillSel ==-1){
	   return;
	}


	int dependancySel = m_statDependancyList.GetCurSel();
	if(dependancySel ==-1){
	   return;
	}
		
	if(!charDefinitionRef)
	   return;

    if(!charDefinitionRef->m_skills){
	   return;
	}


    POSITION sklPos = charDefinitionRef->m_skills->FindIndex(skillSel);
    if(!sklPos)
	   return;
	CBaseSkillObject * skillPtr = (CBaseSkillObject *)charDefinitionRef->m_skills->GetAt(sklPos);

    if(!skillPtr->m_affectedByStatList)
		return;

	POSITION afPos = skillPtr->m_affectedByStatList->FindIndex(skillSel);
    if(!afPos)
	   return;
	CAffectedByStatObject * affectedStatPtr = (CAffectedByStatObject *)skillPtr->m_affectedByStatList->GetAt(afPos);
    
    m_statImprovement      = affectedStatPtr->m_statisticalImprovement;
	m_statImprovementCap   = affectedStatPtr->m_statisticalImprovementCap;
	m_ImprovementOnSuccess = affectedStatPtr->m_improvementOfSuccessVariable;

    UpdateData(FALSE);
}
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
//-----------------------------------------------------------------------//
void CSkillEditorDlg::OnBUTTONReplaceDependancyValues() 
{
	UpdateData(TRUE);

    int skillSel = m_skillList.GetCurSel();
	if(skillSel ==-1){
	   MessageBox("Must select a skill");
	   return;
	}


	int dependancySel = m_statDependancyList.GetCurSel();
	if(dependancySel ==-1){
	   MessageBox("Must select a dependancy");
	   return;
	}
		
	if(!charDefinitionRef)
	   return;

    if(!charDefinitionRef->m_skills){
	   return;
	}


    POSITION sklPos = charDefinitionRef->m_skills->FindIndex(skillSel);
    if(!sklPos)
	   return;
	CBaseSkillObject * skillPtr = (CBaseSkillObject *)charDefinitionRef->m_skills->GetAt(sklPos);

    if(!skillPtr->m_affectedByStatList)
		return;

	POSITION afPos = skillPtr->m_affectedByStatList->FindIndex(skillSel);
    if(!afPos)
	   return;
	CAffectedByStatObject * affectedStatPtr = (CAffectedByStatObject *)skillPtr->m_affectedByStatList->GetAt(afPos);
    
    affectedStatPtr->m_statisticalImprovement = m_statImprovement;
	affectedStatPtr->m_statisticalImprovementCap = m_statImprovementCap;
	affectedStatPtr->m_improvementOfSuccessVariable = m_ImprovementOnSuccess;

    UpdateData(FALSE);
	InListSkillDependancies(dependancySel);
}
