#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>

#include "CCharacterDefinitionsClass.h"
#include "CTextReadoutClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//------------------------------------//
//------------------------------------//
//------Statistical section-----------//
//------------------------------------//
CStatisticObject::CStatisticObject()
{
    m_type              = 0;
    m_statName          = "NAME";
	m_currentAmount     = 0.0f;
    m_statCap           = 0.0f;
}

void CStatisticObject::Clone(CStatisticObject ** copy)
{
     CStatisticObject * copyLocal = new CStatisticObject();
     copyLocal->m_type = m_type;
     copyLocal->m_statName = m_statName;
	 copyLocal->m_currentAmount = m_currentAmount;
	 copyLocal->m_statCap = m_statCap;
	 *copy = copyLocal;
}


void CStatisticObject::Serialize( CArchive& ar )
{

CObject::Serialize( ar );
	if(ar.IsStoring() ){
	ar << m_type
       << m_statName
       << m_currentAmount
       << m_statCap;
	}
	else
	{
    ar >> m_type
       >> m_statName
       >> m_currentAmount
       >> m_statCap;
	}
}

void CStatisticObjectList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CStatisticObject * statPtr = (CStatisticObject *)GetNext(posLoc);
        delete statPtr;
		statPtr=0;
	}
    RemoveAll();
}


void CStatisticObjectList::Clone(CStatisticObjectList ** copy)
{
    CStatisticObjectList * copyLocal = new CStatisticObjectList;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CStatisticObject * statPtr = (CStatisticObject *)GetNext(posLoc);
        CStatisticObject * clonePtr = NULL;
		statPtr->Clone(&clonePtr);
		copyLocal->AddTail(clonePtr);
	}
    *copy = copyLocal;
}


CStatisticObject * CStatisticObjectList::GetStatObjectByType(int type)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CStatisticObject * cstatObj = (CStatisticObject *)GetNext(posLoc);
		if(cstatObj->m_type == type)
		   return cstatObj;
	}
	return NULL;
}
//------------------------------------//
//------------------------------------//
//------Affected by stat Section------//
//------------------------------------//
CAffectedByStatObject::CAffectedByStatObject()
{
    m_statisticalImprovement                 = 0.0f;
    m_statisticalImprovementCap              = 0.0f;
	m_statType                               = 0;
    m_improvementOfSuccessVariable           = 0.0f;
}

void CAffectedByStatObject::Clone(CAffectedByStatObject ** copy)
{
    CAffectedByStatObject * copyLocal = new CAffectedByStatObject();
    copyLocal->m_improvementOfSuccessVariable   = m_improvementOfSuccessVariable;
    copyLocal->m_statType                       = m_statType;
	copyLocal->m_statisticalImprovement         = m_statisticalImprovement;
	copyLocal->m_statisticalImprovementCap      = m_statisticalImprovementCap;
*copy = copyLocal;
}


void CAffectedByStatObject::Serialize( CArchive& ar )
{

CObject::Serialize( ar );
	if(ar.IsStoring() ){
	ar << m_statisticalImprovement
       << m_statisticalImprovementCap
       << m_statType
       << m_improvementOfSuccessVariable;
	}
	else
	{
    ar >> m_statisticalImprovement
       >> m_statisticalImprovementCap
       >> m_statType
       >> m_improvementOfSuccessVariable;
	}
}

void CAffectedByStatObjectList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CAffectedByStatObject * ptr = (CAffectedByStatObject *)GetNext(posLoc);
        delete ptr;
		ptr=0;
	}
    RemoveAll();
}

void CAffectedByStatObjectList::Clone(CAffectedByStatObjectList ** copy)
{
    CAffectedByStatObjectList * copyLocal = new CAffectedByStatObjectList;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CAffectedByStatObject * ptr = (CAffectedByStatObject *)GetNext(posLoc);
        CAffectedByStatObject * clonePtr = NULL;
		ptr->Clone(&clonePtr);
		copyLocal->AddTail(clonePtr);
	}
    *copy = copyLocal;
}

//------------------------------------//
//------------------------------------//
//---skill cost class section---------//
//------------------------------------//
CSkillCostObject::CSkillCostObject()
{
    m_type                      = 0;
    m_costAmount                = 0;
}

void CSkillCostObject::Clone(CSkillCostObject ** copy)
{

CSkillCostObject * copyLocal = new CSkillCostObject();
copyLocal->m_costAmount = m_costAmount;
copyLocal->m_type       = m_type;
*copy = copyLocal;
}


void CSkillCostObject::Serialize( CArchive& ar )
{

CObject::Serialize( ar );
	if(ar.IsStoring() ){
	ar << m_type
       << m_costAmount;
	}
	else
	{
    ar >> m_type
       >> m_costAmount;
	}
}


void CSkillCostObjectList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CSkillCostObject * ptr = (CSkillCostObject *)GetNext(posLoc);
        delete ptr;
		ptr=0;
	}
    RemoveAll();
}

void CSkillCostObjectList::Clone(CSkillCostObjectList ** copy)
{
    CSkillCostObjectList * copyLocal = new CSkillCostObjectList;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CSkillCostObject * ptr = (CSkillCostObject *)GetNext(posLoc);
        CSkillCostObject * clonePtr = NULL;
		ptr->Clone(&clonePtr);
		copyLocal->AddTail(clonePtr);
	}
    *copy = copyLocal;
}


//------------------------------------//
//------------------------------------//
//------Ability class section---------//
//------------------------------------//
CAbilityObject::CAbilityObject()
{
    m_type                   = 0;
    m_miscInt                = 0;
	m_minimumRequiredSkill   = 0.0f;
    m_masterLimit            = 0.0f;
	m_costOfAbilityList      = NULL;
	m_amountOfGainOnSuccess  = 1.0f;
	m_name                   = "NAME";
	m_actionAttribute        = 0;
    m_actionAttributeInt     = 0;
	m_abilityDuration        = 100;
	m_abilityInUse           = FALSE;
}

void CAbilityObject::Clone(CAbilityObject ** copy)
{
   CAbilityObject * copyLocal = new CAbilityObject();
   if(m_costOfAbilityList)
   m_costOfAbilityList->Clone(&copyLocal->m_costOfAbilityList);
   
   copyLocal->m_name    = m_name;
   copyLocal->m_type    = m_type;
   copyLocal->m_miscInt = m_miscInt;
   copyLocal->m_minimumRequiredSkill = m_minimumRequiredSkill;
   copyLocal->m_masterLimit = m_masterLimit;
   copyLocal->m_amountOfGainOnSuccess = m_amountOfGainOnSuccess;
   copyLocal->m_actionAttribute        = m_actionAttribute;
   copyLocal->m_actionAttributeInt     = m_actionAttributeInt;
   copyLocal->m_comment                = m_comment;
   copyLocal->m_abilityDuration        = m_abilityDuration;
   copyLocal->m_abilityInUse = FALSE;

   *copy = copyLocal;
}

void CAbilityObject::SafeDelete()
{
	if(m_costOfAbilityList){
        m_costOfAbilityList->SafeDelete();
		delete m_costOfAbilityList;
        m_costOfAbilityList=0;
	}

}

void CAbilityObject::Serialize( CArchive& ar )
{

CObject::Serialize( ar );
	if(ar.IsStoring() ){
	ar << m_type
       << m_miscInt
       << m_minimumRequiredSkill
       << m_masterLimit
	   << m_costOfAbilityList
	   << m_amountOfGainOnSuccess
	   << m_name
	   << m_actionAttribute
	   << m_actionAttributeInt
	   << m_comment
	   << m_abilityDuration;
	}
	else
	{
    ar >> m_type
       >> m_miscInt
       >> m_minimumRequiredSkill
       >> m_masterLimit
	   >> m_costOfAbilityList
	   >> m_amountOfGainOnSuccess
	   >> m_name
	   >> m_actionAttribute
	   >> m_actionAttributeInt
	   >> m_comment
	   >> m_abilityDuration;

    m_abilityInUse           = FALSE;
	
	}
}

CAbilityObject* CAbilityObjectList::GetAbilityByType(int type)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CAbilityObject * ptr = (CAbilityObject *)GetNext(posLoc);
        if(ptr->m_type == type)
		   return ptr; 
	}
	return NULL;
}

void CAbilityObjectList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CAbilityObject * ptr = (CAbilityObject *)GetNext(posLoc);
        if(ptr->m_costOfAbilityList){
		   ptr->m_costOfAbilityList->SafeDelete();
           ptr->m_costOfAbilityList = 0;
		}
		delete ptr;
	}
    RemoveAll();
}

void CAbilityObjectList::Clone(CAbilityObjectList ** copy)
{
    CAbilityObjectList * copyLocal = new CAbilityObjectList;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CAbilityObject * ptr = (CAbilityObject *)GetNext(posLoc);
        CAbilityObject * clonePtr = NULL;
		ptr->Clone(&clonePtr);
		copyLocal->AddTail(clonePtr);
	}
    *copy = copyLocal;
}
//------------------------------------//
//------------------------------------//
//---Base skill class section---------//
//------------------------------------//
CBaseSkillObject::CBaseSkillObject()
{
    m_skillName              = "SKILL";
    m_skillType              = 0;
	m_affectedByStatList     = NULL;
    m_skillAbilities         = NULL;
	m_currentSkillValue      = 0.0f;
	m_skillCap               = 0.0f;
}

void CBaseSkillObject::SafeDelete()
{
        if(m_affectedByStatList){
		   m_affectedByStatList->SafeDelete();
		   delete m_affectedByStatList;
		   m_affectedByStatList=0;
		}
		if(m_skillAbilities){
           m_skillAbilities->SafeDelete();
		   delete m_skillAbilities;
		   m_skillAbilities=0;
		}
}


void CBaseSkillObject::Clone(CBaseSkillObject ** clone)
{
   CBaseSkillObject * copyLocal = new CBaseSkillObject();
   if(m_affectedByStatList)
   m_affectedByStatList->Clone(&copyLocal->m_affectedByStatList);

   if(m_skillAbilities)
   m_skillAbilities->Clone(&copyLocal->m_skillAbilities);

   copyLocal->m_currentSkillValue = m_currentSkillValue;
   copyLocal->m_skillCap = m_skillCap;
   copyLocal->m_skillName = m_skillName;
   copyLocal->m_skillType = m_skillType;
 
  *clone = copyLocal;
}


void CBaseSkillObject::Serialize( CArchive& ar )
{

CObject::Serialize( ar );
	if(ar.IsStoring() ){
	ar << m_skillName
       << m_skillType
       << m_affectedByStatList
       << m_skillAbilities
	   << m_currentSkillValue
	   << m_skillCap;

	}
	else
	{
    ar >> m_skillName
       >> m_skillType
       >> m_affectedByStatList
       >> m_skillAbilities
	   >> m_currentSkillValue
	   >> m_skillCap;

	}
}

CBaseSkillObject* CBaseSkillObjectList::GetSkillByType(int type)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CBaseSkillObject * ptr = (CBaseSkillObject *)GetNext(posLoc);
        if(ptr->m_skillType == type)
		   return ptr; 
	}
	return NULL;
}

void CBaseSkillObjectList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CBaseSkillObject * ptr = (CBaseSkillObject *)GetNext(posLoc);
        ptr->SafeDelete();
		delete ptr;
		ptr=0;
	}
	RemoveAll();
}


void CBaseSkillObjectList::Clone(CBaseSkillObjectList ** copy)
{
    CBaseSkillObjectList * copyLocal = new CBaseSkillObjectList;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CBaseSkillObject * ptr = (CBaseSkillObject *)GetNext(posLoc);
        CBaseSkillObject * clonePtr = NULL;
		ptr->Clone(&clonePtr);
		copyLocal->AddTail(clonePtr);
	}
    *copy = copyLocal;
}
//------------------------------------//
//------------------------------------//
//-character definition class section-//
//------------------------------------//
CharacterDefObject::CharacterDefObject()
{
   m_statistics = NULL;
   m_skills = NULL;
   m_skillInUse = -1;
   m_abilityInUse = -1;
   m_inUseTimeStamp = 0;
   m_durationInUse = 0;
   m_misInput1 = 0;
}

void CharacterDefObject::SafeDelete()
{
	if(m_statistics){
       m_statistics->SafeDelete();
       delete m_statistics;
	   m_statistics=0;
	}
    if(m_skills){
       m_skills->SafeDelete();
       delete m_skills;
	   m_skills=0;
	}

}

void CharacterDefObject::Clone(CharacterDefObject ** copy)
{
    CharacterDefObject * copyLocal = new CharacterDefObject();
    if(m_skills)
	   m_skills->Clone(&copyLocal->m_skills);

	if(m_statistics)
	   m_statistics->Clone(&copyLocal->m_statistics);


	copyLocal->m_skillInUse = -1;
	copyLocal->m_abilityInUse = -1;
    copyLocal->m_inUseTimeStamp = 0;
	copyLocal->m_durationInUse = 0;
    copyLocal->m_misInput1 = 0;

	*copy = copyLocal;
}


void CharacterDefObject::Serialize( CArchive& ar )
{

CObject::Serialize( ar );
	if(ar.IsStoring() ){
	ar << m_statistics 
	   << m_skills;

	}
	else
	{
    ar >> m_statistics 
	   >> m_skills;
	
	m_skillInUse     = -1;
	m_abilityInUse   = -1;
    m_inUseTimeStamp = 0;
    m_durationInUse = 0;
    m_misInput1 = 0;
	
	}
}





void CharacterDefObjectList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CharacterDefObject * ptr = (CharacterDefObject *)GetNext(posLoc);
        if(ptr->m_skills){
		   ptr->m_skills->SafeDelete();
		   delete ptr->m_skills;
		   ptr->m_skills=0;
		}
		if(ptr->m_statistics){
		   ptr->m_statistics->SafeDelete();
		   delete ptr->m_statistics;
		   ptr->m_statistics=0;
		}
		delete ptr;
		ptr=0;
	}
    RemoveAll();
}


void CharacterDefObjectList::Clone(CharacterDefObjectList ** copy)
{
    CharacterDefObjectList * copyLocal = new CharacterDefObjectList;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CharacterDefObject * ptr = (CharacterDefObject *)GetNext(posLoc);
        CharacterDefObject * clonePtr = NULL;
		ptr->Clone(&clonePtr);
		copyLocal->AddTail(clonePtr);
	}
    *copy = copyLocal;
}
//functions
//-------------------------//
//USESKILL 0=function failed 1=success 2=failed 3=skill not high enough to attemp
int CharacterDefObject::UseSkill(int skillType,
								 int abilityType,
								 int *actionRet,
								 int *actionIndexMiscRet,
								 CString * commentRet)
{

    if(!m_skills)
        return 0;
        CBaseSkillObject * skillObj = m_skills->GetSkillByType(skillType);
        if(!skillObj)
            return 0;//no skill found
        if(!skillObj->m_skillAbilities)
		    return 0;
            CAbilityObject * abilityPtr = skillObj->m_skillAbilities->GetAbilityByType(abilityType);
			if(!abilityPtr)
                return 0;

			*commentRet = abilityPtr->m_comment;
            //Evaluate
            if(abilityPtr->m_minimumRequiredSkill > skillObj->m_currentSkillValue)
			   return 3;//not enough skill to attemp ability
			
			float m_range = skillObj->m_currentSkillValue - abilityPtr->m_minimumRequiredSkill;
            float m_skillZone = abilityPtr->m_masterLimit - abilityPtr->m_minimumRequiredSkill;

			int modulate = (int)m_skillZone+1;
            int diceRoll = rand()%modulate;
            if(diceRoll <= m_range || skillObj->m_currentSkillValue == 0)
			{
			    //------------------------------------------------//
				if(m_range >= 0){//benefits of succesful throw
                
				*actionRet = abilityPtr->m_actionAttribute;	
				*actionIndexMiscRet = abilityPtr->m_actionAttributeInt;
					
				float percentage = m_range/m_skillZone;
                percentage = 1.0f - percentage;
				    //stat improvements from success
				    if(skillObj->m_affectedByStatList)
                    for(POSITION affectedPos = skillObj->m_affectedByStatList->GetHeadPosition();affectedPos != NULL;)
					{//affected stat loop
                       CAffectedByStatObject * affectedObj = (CAffectedByStatObject *)skillObj->m_affectedByStatList->GetNext(affectedPos);
                       if(m_statistics){//valid stats
                          CStatisticObject * statPtr = (CStatisticObject *)m_statistics->GetStatObjectByType(affectedObj->m_statType);
						  if(affectedObj->m_statisticalImprovementCap > statPtr->m_currentAmount)
						  {//can benefit stat by using this ability
                             float impAmount = affectedObj->m_statisticalImprovement * percentage;
							 statPtr->m_currentAmount = statPtr->m_currentAmount + impAmount;
						  }//can benefit stat by using this ability
					   }//end valid stats
					}//end affected stat loop
					//end stat improvements from success
					//skill improvements
                    if(skillObj->m_currentSkillValue < skillObj->m_skillCap)
					   skillObj->m_currentSkillValue += (abilityPtr->m_amountOfGainOnSuccess * percentage);
					else
					   skillObj->m_currentSkillValue = skillObj->m_skillCap;
					//end skill improvements
				}//end benefits of succesful throw
			    //------------------------------------------------//
				return 1;//SUCCESS
			}
			else
			{
               //failed skill use
			   return 2;
			}
     return 0;
}
//-------------------------//
//ADDSKILL
int CharacterDefObject::AddSkill(CString   skillName,
								 int       skillType,
								 float     initialSkillValue,
								 float     skillCap)
{
    if(!m_skills)
		m_skills = new CBaseSkillObjectList;

        CBaseSkillObject * newSkillPtr    = new CBaseSkillObject();
		newSkillPtr->m_currentSkillValue  = initialSkillValue;
		newSkillPtr->m_skillCap           = skillCap;
		newSkillPtr->m_skillName          = skillName;
		newSkillPtr->m_skillType          = skillType;

        m_skills->AddTail(newSkillPtr);

	return 1;
}
//-------------------------//
//ADDAbility
int CharacterDefObject::AddAbility(int skillType,
								   int type,
								   int miscInt,
								   float minimumRequiredSkill,
								   float masterLimit,
								   float gainBasis,
								   CString name,
								   CString comment,
								   int duration)
{
    if(!m_skills)
        return 0;

    CBaseSkillObject * skillObj = m_skills->GetSkillByType(skillType);

    if(!skillObj->m_skillAbilities)
		skillObj->m_skillAbilities = new CAbilityObjectList;

    CAbilityObject * abilityPtr = new CAbilityObject();
	abilityPtr->m_name = name;
	abilityPtr->m_masterLimit = masterLimit;
	abilityPtr->m_minimumRequiredSkill = minimumRequiredSkill;
	abilityPtr->m_miscInt = miscInt;
	abilityPtr->m_type = type;
	abilityPtr->m_amountOfGainOnSuccess = gainBasis;
	abilityPtr->m_abilityDuration       = duration;
	abilityPtr->m_comment               = comment;

    skillObj->m_skillAbilities->AddTail(abilityPtr);

	return 1;
}
//-------------------------//
//ADDSTAT
int CharacterDefObject::AddStatistic(int type,
									 CString name,
									 float startAmount,
									 float statCap)
{
    if(!m_statistics)
		m_statistics = new CStatisticObjectList;

	CStatisticObject * newStatPtr   = new CStatisticObject();
	newStatPtr->m_currentAmount     = startAmount;
	newStatPtr->m_statCap           = statCap;
    newStatPtr->m_statName          = name;
	newStatPtr->m_type              = type;
 
	m_statistics->AddTail(newStatPtr);

	return 1;
}


//serialize globals
IMPLEMENT_SERIAL(CStatisticObject,          CObject, 0)
IMPLEMENT_SERIAL(CStatisticObjectList,      CObList, 0)
IMPLEMENT_SERIAL(CAbilityObject,            CObject, 0)
IMPLEMENT_SERIAL(CAbilityObjectList,        CObList, 0)
IMPLEMENT_SERIAL(CAffectedByStatObject,     CObject, 0)
IMPLEMENT_SERIAL(CAffectedByStatObjectList, CObList, 0)
IMPLEMENT_SERIAL(CSkillCostObject,          CObject, 0)
IMPLEMENT_SERIAL(CSkillCostObjectList,      CObList, 0)
IMPLEMENT_SERIAL(CBaseSkillObject,          CObject, 0)
IMPLEMENT_SERIAL(CBaseSkillObjectList,      CObList, 0)
IMPLEMENT_SERIAL(CharacterDefObject,        CObject, 0)
IMPLEMENT_SERIAL(CharacterDefObjectList,    CObList, 0)