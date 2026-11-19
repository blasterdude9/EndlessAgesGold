// AbilityEditorDlg.cpp : implementation file
//

#include  "stdafx.h"
#include "Resource.h"
#include "AbilityEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAbilityEditorDlg dialog


CAbilityEditorDlg::CAbilityEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAbilityEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAbilityEditorDlg)
	m_abilityGainBasis = 0.0f;
	m_abilityMasterLimit = 0.0f;
	m_abilityMinRequiredSkill = 0.0f;
	m_abilityMiscInt = 0;
	m_abilityType = 0;
	m_abilityName = _T("");
	m_costQuanity = 0;
	m_costResourceType = 0;
	m_attributeMiscInt = 0;
	m_successAction = _T("");
	m_comment = _T("");
	m_abilityDuration = 0;
	//}}AFX_DATA_INIT
}


void CAbilityEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAbilityEditorDlg)
	DDX_Control(pDX, IDC_LIST_CostList, m_listCosts);
	DDX_Control(pDX, IDC_LIST_Abilities, m_abilityList);
	DDX_Text(pDX, IDC_EDIT_AbilityGainBasis, m_abilityGainBasis);
	DDX_Text(pDX, IDC_EDIT_AbilityMasterLimit, m_abilityMasterLimit);
	DDX_Text(pDX, IDC_EDIT_AbilityMinRequiredSkill, m_abilityMinRequiredSkill);
	DDX_Text(pDX, IDC_EDIT_abilityMiscInt, m_abilityMiscInt);
	DDX_Text(pDX, IDC_EDIT_abilityType, m_abilityType);
	DDX_Text(pDX, IDC_EDIT_abilityName, m_abilityName);
	DDX_Text(pDX, IDC_EDIT_costQuanity, m_costQuanity);
	DDX_Text(pDX, IDC_EDIT_resourceCostType, m_costResourceType);
	DDX_Text(pDX, IDC_EDIT_attributeMiscInt, m_attributeMiscInt);
	DDX_CBString(pDX, IDC_COMBO_AttributeAction, m_successAction);
	DDX_Text(pDX, IDC_EDIT_Comment, m_comment);
	DDX_Text(pDX, IDC_EDIT_AbilityDuration, m_abilityDuration);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAbilityEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CAbilityEditorDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADDAbility, OnBUTTONADDAbility)
	ON_BN_CLICKED(IDC_BUTTON_DELETEability, OnBUTTONDELETEability)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceValues, OnBUTTONReplaceValues)
	ON_LBN_SELCHANGE(IDC_LIST_Abilities, OnSelchangeLISTAbilities)
	ON_BN_CLICKED(IDC_BUTTON_CostReplaceValues, OnBUTTONCostReplaceValues)
	ON_BN_CLICKED(IDC_BUTTON_addCost, OnBUTTONaddCost)
	ON_BN_CLICKED(IDC_BUTTON_DeleteCost, OnBUTTONDeleteCost)
	ON_LBN_SELCHANGE(IDC_LIST_CostList, OnSelchangeLISTCostList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAbilityEditorDlg message handlers

void CAbilityEditorDlg::OnBUTTONADDAbility() 
{
	UpdateData(TRUE);
	charDefinitionRef->AddAbility(m_refSkillType,m_abilityType,
		                          m_abilityMiscInt,m_abilityMinRequiredSkill,
								  m_abilityMasterLimit,m_abilityGainBasis,
								  m_abilityName,
								  m_comment,
								  m_abilityDuration);

	
	UpdateData(FALSE);
	InListAbilities(-1);
}

void CAbilityEditorDlg::OnBUTTONDELETEability() 
{
	CBaseSkillObject * bsKillPtr = charDefinitionRef->m_skills->GetSkillByType(m_refSkillType);
	if(!bsKillPtr)
		return;

	if(!bsKillPtr->m_skillAbilities)
		return;

	int abilitySel = m_abilityList.GetCurSel();
	if(abilitySel == -1)
	{
		AfxMessageBox("Must select a ability");
		return;
	}

	POSITION posLoc = bsKillPtr->m_skillAbilities->FindIndex(abilitySel);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CAbilityObject * ability = (CAbilityObject *)bsKillPtr->m_skillAbilities->GetAt(posLoc);
	ability->SafeDelete();
	bsKillPtr->m_skillAbilities->RemoveAt(posLoc);
	InListAbilities(abilitySel-1);
}

void CAbilityEditorDlg::OnBUTTONReplaceValues() 
{
	CBaseSkillObject * bsKillPtr = charDefinitionRef->m_skills->GetSkillByType(m_refSkillType);
    if(!bsKillPtr)
		return;
	
	if(!bsKillPtr->m_skillAbilities)
	    return;


    int abilitySel = m_abilityList.GetCurSel();
	if(abilitySel == -1){
       AfxMessageBox("Must select a ability");
	   return;
	}

    POSITION posLoc = bsKillPtr->m_skillAbilities->FindIndex(abilitySel);
	if(!posLoc)
		return;
    CAbilityObject * ability = (CAbilityObject *)bsKillPtr->m_skillAbilities->GetAt(posLoc);
    
	
	
	UpdateData(TRUE);

    ability->m_type = m_abilityType;
    ability->m_miscInt = m_abilityMiscInt;
    ability->m_minimumRequiredSkill = m_abilityMinRequiredSkill;
    ability->m_masterLimit = m_abilityMasterLimit;
    ability->m_amountOfGainOnSuccess = m_abilityGainBasis;
    ability->m_name = m_abilityName;
	ability->m_actionAttributeInt = m_attributeMiscInt;
	ability->m_comment            = m_comment;
	ability->m_abilityDuration    = m_abilityDuration;

    if(m_successAction == "NONE")
       ability->m_actionAttribute = 0;
	if(m_successAction == "Generate Resource")
       ability->m_actionAttribute = 1;
	if(m_successAction == "Create Inventory")
       ability->m_actionAttribute = 2;
	if(m_successAction == "Use Missile Database")
       ability->m_actionAttribute = 3;
	if(m_successAction == "Use Blend Mode")
       ability->m_actionAttribute = 4;

	UpdateData(FALSE);
	InListAbilities(abilitySel);
}

void CAbilityEditorDlg::InListAbilities(int sel)
{
   UpdateData(TRUE);
   CBaseSkillObject * bsKillPtr = charDefinitionRef->m_skills->GetSkillByType(m_refSkillType);
   if(!bsKillPtr->m_skillAbilities)
	   return;

   m_abilityList.ResetContent();
   for(POSITION posLoc = bsKillPtr->m_skillAbilities->GetHeadPosition();posLoc != NULL;)
   {
       CAbilityObject * abilityPtr = (CAbilityObject *)bsKillPtr->m_skillAbilities->GetNext(posLoc);
       m_abilityList.AddString(abilityPtr->m_name);
   }

   if(sel >= 0){
      m_abilityList.SetCurSel(sel);
   }
   UpdateData(FALSE);
}


BOOL CAbilityEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InListAbilities(-1);
	





    



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAbilityEditorDlg::OnSelchangeLISTAbilities() 
{
	CBaseSkillObject * bsKillPtr = charDefinitionRef->m_skills->GetSkillByType(m_refSkillType);
    UpdateData(TRUE);
	if(!bsKillPtr)
		return;
	
	if(!bsKillPtr->m_skillAbilities)
	    return;


    int abilitySel = m_abilityList.GetCurSel();
	if(abilitySel == -1){
	   return;
	}

    POSITION posLoc = bsKillPtr->m_skillAbilities->FindIndex(abilitySel);
	if(!posLoc)
		return;
    CAbilityObject * ability = (CAbilityObject *)bsKillPtr->m_skillAbilities->GetAt(posLoc);
    
	
	
	

    m_abilityType             = ability->m_type;
    m_abilityMiscInt          = ability->m_miscInt;
    m_abilityMinRequiredSkill = ability->m_minimumRequiredSkill;
    m_abilityMasterLimit      = ability->m_masterLimit;
    m_abilityGainBasis        = ability->m_amountOfGainOnSuccess;
    m_abilityName             = ability->m_name;
	m_attributeMiscInt        = ability->m_actionAttributeInt;
	m_comment                 = ability->m_comment;
    m_abilityDuration         = ability->m_abilityDuration;

	if(ability->m_actionAttribute == 0)
	   m_successAction = "NONE";
	if(ability->m_actionAttribute == 1)
	   m_successAction = "Generate Resource";
	if(ability->m_actionAttribute == 2)
	   m_successAction = "Create Inventory";
	if(ability->m_actionAttribute == 3)
	   m_successAction = "Use Missile Database";
	if(ability->m_actionAttribute == 3)
	   m_successAction = "Use Blend Mode";
	

	UpdateData(FALSE);
	InListAbilities(abilitySel);
	InListAbilityCost(ability,-1);
}


void CAbilityEditorDlg::InListAbilityCost(CAbilityObject * ability,int sel)
{
  m_listCosts.ResetContent();
  if(!ability->m_costOfAbilityList)
	  return;

  for(POSITION posLoc = ability->m_costOfAbilityList->GetHeadPosition();posLoc != NULL;)
  {
      CSkillCostObject * costPtr = (CSkillCostObject *)ability->m_costOfAbilityList->GetNext(posLoc);
      
	  if(!resourceListRef)
	  m_listCosts.AddString(operator+("Unknown Type :",ConvertLongToString(costPtr->m_type)));
	  else{
          CResourceObject * resPtr = resourceListRef->GetByType(costPtr->m_type);
          if(resPtr)
		    m_listCosts.AddString(operator+("Known Type :",resPtr->m_resourceName));
		  else
		    m_listCosts.AddString(operator+("Unknown Type :",ConvertLongToString(costPtr->m_type)));
	  }
  }

  if(sel >= 0)
	 m_listCosts.SetCurSel(sel);

}


void CAbilityEditorDlg::OnBUTTONCostReplaceValues() 
{
	CBaseSkillObject * bsKillPtr = charDefinitionRef->m_skills->GetSkillByType(m_refSkillType);
    if(!bsKillPtr)
		return;
	
	if(!bsKillPtr->m_skillAbilities)
	    return;


    int abilitySel = m_abilityList.GetCurSel();
	if(abilitySel == -1){
	   MessageBox("Must Select a ability");
	   return;
	}

    POSITION posLoc = bsKillPtr->m_skillAbilities->FindIndex(abilitySel);
	if(!posLoc)
		return;
    CAbilityObject * ability = (CAbilityObject *)bsKillPtr->m_skillAbilities->GetAt(posLoc);
    
	if(!ability->m_costOfAbilityList)
		return;
	
    int cstSel = m_listCosts.GetCurSel();
    if(cstSel == -1){
	   MessageBox("Must select a cost");
       return;
	}

    posLoc = ability->m_costOfAbilityList->FindIndex(cstSel);
    if(!posLoc)
		return;
	CSkillCostObject * cskillPtr = (CSkillCostObject *)ability->m_costOfAbilityList->GetAt(posLoc);
	UpdateData(TRUE);
	cskillPtr->m_costAmount = m_costQuanity;
	cskillPtr->m_type       = m_costResourceType;
	UpdateData(FALSE);
	InListAbilityCost(ability,cstSel);
}

void CAbilityEditorDlg::OnBUTTONaddCost() 
{
	CBaseSkillObject * bsKillPtr = charDefinitionRef->m_skills->GetSkillByType(m_refSkillType);
    if(!bsKillPtr)
		return;
	
	if(!bsKillPtr->m_skillAbilities)
	    return;


    int abilitySel = m_abilityList.GetCurSel();
	if(abilitySel == -1){
       AfxMessageBox("Must select a ability");
	   return;
	}

    POSITION posLoc = bsKillPtr->m_skillAbilities->FindIndex(abilitySel);
	if(!posLoc)
		return;
    CAbilityObject * ability = (CAbilityObject *)bsKillPtr->m_skillAbilities->GetAt(posLoc);

    if(!ability->m_costOfAbilityList)
		ability->m_costOfAbilityList = new CSkillCostObjectList;

    CSkillCostObject * skillCost = new CSkillCostObject();
    UpdateData(TRUE);
	skillCost->m_costAmount = m_costQuanity;
	skillCost->m_type       = m_costResourceType;
	UpdateData(FALSE);
	ability->m_costOfAbilityList->AddTail(skillCost);
	InListAbilityCost(ability,ability->m_costOfAbilityList->GetCount());

}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
void CAbilityEditorDlg::OnBUTTONDeleteCost() 
{	
	CBaseSkillObject * bsKillPtr = charDefinitionRef->m_skills->GetSkillByType(m_refSkillType);
	if(!bsKillPtr)
		return;

	if(!bsKillPtr->m_skillAbilities)
		return;

	int abilitySel = m_abilityList.GetCurSel();
	if(abilitySel == -1)
	{
		MessageBox("Must Select a ability");
		return;
	}

	POSITION posLoc = bsKillPtr->m_skillAbilities->FindIndex(abilitySel);
	if(!posLoc)
		return;
	
	CAbilityObject * ability = (CAbilityObject *)bsKillPtr->m_skillAbilities->GetAt(posLoc);
	if(!ability->m_costOfAbilityList)
		return;

	int cstSel = m_listCosts.GetCurSel();
	if(cstSel == -1)
	{
		MessageBox("Must select a cost");
		return;
	}

	posLoc = ability->m_costOfAbilityList->FindIndex(cstSel);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;

	CSkillCostObject * cskillPtr = (CSkillCostObject *)ability->m_costOfAbilityList->GetAt(posLoc);
	delete cskillPtr;
	cskillPtr=0;
	ability->m_costOfAbilityList->RemoveAt(posLoc);
	InListAbilityCost(ability,cstSel-1);
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//----------this function converts long to cstring-------------//
CString CAbilityEditorDlg::ConvertLongToString(int number)
{    
	          int decimal,sign;
              char *buffer;
              CString temp;	 
	          buffer = _fcvt(number,0,&decimal,&sign);
    	      temp = buffer;
			  if(sign != 0)
                 temp = operator+("-",temp);
			  return temp;
}


void CAbilityEditorDlg::OnSelchangeLISTCostList() 
{
    CBaseSkillObject * bsKillPtr = charDefinitionRef->m_skills->GetSkillByType(m_refSkillType);
    if(!bsKillPtr)
		return;
	
	if(!bsKillPtr->m_skillAbilities)
	    return;


    int abilitySel = m_abilityList.GetCurSel();
	if(abilitySel == -1){
	   return;
	}

    POSITION posLoc = bsKillPtr->m_skillAbilities->FindIndex(abilitySel);
	if(!posLoc)
		return;
    CAbilityObject * ability = (CAbilityObject *)bsKillPtr->m_skillAbilities->GetAt(posLoc);
    
	if(!ability->m_costOfAbilityList)
		return;
	
    int cstSel = m_listCosts.GetCurSel();
    if(cstSel == -1){
       return;
	}

    posLoc = ability->m_costOfAbilityList->FindIndex(cstSel);
    if(!posLoc)
		return;
	CSkillCostObject * cskillPtr = (CSkillCostObject *)ability->m_costOfAbilityList->GetAt(posLoc);

	UpdateData(TRUE);
	m_costQuanity = cskillPtr->m_costAmount;
	m_costResourceType = cskillPtr->m_type;
	UpdateData(FALSE);
}
