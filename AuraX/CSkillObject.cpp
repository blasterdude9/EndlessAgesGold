#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CSkillClass.h"
#include "dbconnectionsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CSkillAbilityObject::CSkillAbilityObject()
{

    m_abilityName          = "N/A";
	m_menuID               = -1;
    m_minimumLevelToUse    = 0;
    m_masterLevel          = 0.0f;
	m_functionType         = 0;
    m_requiredStatistics   = NULL;//minimum statistics to attempt
	m_requiredItems        = NULL;
	m_resultItems          = NULL;
    m_miscInt              = 0;
    m_miscInt2             = 0;
	m_miscInt3             = 0;
	m_effectBind           = -1;
	m_miscAmount           = 0;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CSkillAbilityObject::Clone(CSkillAbilityObject ** clone)
{
     CSkillAbilityObject * copyLocal = new CSkillAbilityObject();

	 copyLocal->m_functionType                = m_functionType;
	 copyLocal->m_abilityName                 = m_abilityName;
	 copyLocal->m_masterLevel                 = m_masterLevel;
	 copyLocal->m_minimumLevelToUse           = m_minimumLevelToUse;
	 copyLocal->m_miscInt                     = m_miscInt;
	 copyLocal->m_miscInt2                    = m_miscInt2;
	 copyLocal->m_miscInt3                    = m_miscInt3;
	 copyLocal->m_effectBind                  = m_effectBind;
	 copyLocal->m_miscAmount                  = m_miscAmount;

	 if(m_requiredStatistics)
	    m_requiredStatistics->Clone(&copyLocal->m_requiredStatistics);

	 if(m_requiredItems)
		m_requiredItems->Clone(&copyLocal->m_requiredItems);

	 if(m_resultItems)
		m_resultItems->Clone(&copyLocal->m_resultItems);

     *clone = copyLocal;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CSkillAbilityObject::Serialize( CArchive& ar )
{

int reserved = 0;
float reservedFloat = 0.0f;

CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar << m_abilityName
          << m_minimumLevelToUse
          << m_miscAmount
		  << m_requiredStatistics
		  << m_functionType
		  << m_miscInt
		  << m_miscInt2
		  << m_miscInt3
		  << m_masterLevel
		  << reservedFloat
		  << reservedFloat
		  << m_requiredItems
		  << m_resultItems
		  << m_effectBind;
	}
	else
	{
       ar >> m_abilityName
          >> m_minimumLevelToUse
          >> m_miscAmount
		  >> m_requiredStatistics
		  >> m_functionType
		  >> m_miscInt
		  >> m_miscInt2
		  >> m_miscInt3
		  >> m_masterLevel
		  >> reservedFloat
		  >> reservedFloat
		  >> m_requiredItems
		  >> m_resultItems
		  >> m_effectBind;


	   //m_effectBind = -1;

	   //m_requiredItems = NULL;
	   //m_resultItems   = NULL;
	}
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CSkillAbilityObject::SafeDelete()
{
   if(m_requiredItems)
   {
      m_requiredItems->SafeDelete();
	  delete m_requiredItems;
	  m_requiredItems = 0;
   }
 
   if(m_resultItems)
   {
      m_resultItems->SafeDelete();
	  delete m_resultItems;
	  m_resultItems = 0;
   }

   if(m_requiredStatistics)
   {
      m_requiredStatistics->SafeDelete();
	  delete m_requiredStatistics;
	  m_requiredStatistics = 0;
   }

}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CSkillAbilityList::Clone(CSkillAbilityList ** clone)
{
     CSkillAbilityList * copyLocal = new CSkillAbilityList();

	 for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CSkillAbilityObject * skPtr = (CSkillAbilityObject *)GetNext(posLoc);
		 CSkillAbilityObject * clonePtr = NULL;
         skPtr->Clone(&clonePtr);
         copyLocal->AddTail(clonePtr);
	 }

	 *clone = copyLocal;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CSkillAbilityObject * CSkillAbilityList::GetObjByIndex(int index)
{
    if(index < 0)
	   return NULL;

    POSITION posLoc = FindIndex(index);
	if(!posLoc)
        return NULL;
    CSkillAbilityObject * ptr = (CSkillAbilityObject *)GetAt(posLoc);
	return ptr;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CSkillAbilityList::SafeDelete()
{
     for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CSkillAbilityObject * skPtr = (CSkillAbilityObject *)GetNext(posLoc);
		 skPtr->SafeDelete();
		 delete skPtr;
		 skPtr=0;
	 }
	 RemoveAll();
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CLevelRangeObject::CLevelRangeObject()
{
   m_rangeStart        = 0;
   m_rangeEnd          = 0;
   m_levelTitle        = "N/A";
   m_abilityDB         = 0;
   m_menuID            = -1;
   m_profficiencyBonus = 0;//
   m_perfectiveBonus   = 0;//
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CLevelRangeObject::SafeDelete()
{
   if(m_abilityDB)
   {
      m_abilityDB->SafeDelete();
      delete m_abilityDB;
	  m_abilityDB = 0;
   }

}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CLevelRangeObject::Clone(CLevelRangeObject ** clone)
{
     CLevelRangeObject * lRangeObj = new CLevelRangeObject();
     lRangeObj->m_levelTitle      = m_levelTitle;
	 lRangeObj->m_rangeEnd        = m_rangeEnd;
	 lRangeObj->m_rangeStart      = m_rangeStart;
	 lRangeObj->m_menuID          = m_menuID;
	 lRangeObj->m_profficiencyBonus = m_profficiencyBonus;//
     lRangeObj->m_perfectiveBonus   = m_perfectiveBonus;//

	 if(m_abilityDB)
		m_abilityDB->Clone(&lRangeObj->m_abilityDB);

   *clone = lRangeObj;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CLevelRangeObject::Serialize( CArchive& ar )
{

CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar << m_rangeStart
          << m_rangeEnd
          << m_levelTitle
		  << m_abilityDB
		  << m_menuID
		  << m_profficiencyBonus
		  << m_perfectiveBonus;
	}
	else
	{
       ar >> m_rangeStart
          >> m_rangeEnd
          >> m_levelTitle
		  >> m_abilityDB
		  >> m_menuID
		  >> m_profficiencyBonus
		  >> m_perfectiveBonus;

	   //m_profficiencyBonus = 0;//
       //m_perfectiveBonus   = 0;//

	   //m_menuID = -1;

	   //m_abilityDB = NULL;
	}
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CLevelRangeObject * CLevelRangeList::GetObjByIndex(int index)
{
	if(index < 0)
	   return NULL;

    POSITION posLoc = FindIndex(index);
	if(!posLoc)
        return NULL;
    CLevelRangeObject * ptr = (CLevelRangeObject *)GetAt(posLoc);
	return ptr;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
BOOL CLevelRangeList::GetExperienceValueByLevel(int level,
												long & experiencePoints)
{
	int trace=1;
    for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
        CLevelRangeObject * rgPtr = (CLevelRangeObject *)GetNext(posLoc);
		if(trace == level)
		{
           experiencePoints = rgPtr->m_rangeStart;
           return TRUE;
		}
		trace++;
	}
	return FALSE;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CLevelRangeList::Clone(CLevelRangeList ** clone)
{
     CLevelRangeList * copyLocal = new CLevelRangeList();

	 for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CLevelRangeObject * skPtr = (CLevelRangeObject *)GetNext(posLoc);
		 CLevelRangeObject * clonePtr = NULL;
         skPtr->Clone(&clonePtr);
         copyLocal->AddTail(clonePtr);
	 }

	 *clone = copyLocal;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CLevelRangeList::SafeDelete()
{
     for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
        CLevelRangeObject * skPtr = (CLevelRangeObject *)GetNext(posLoc);
		skPtr->SafeDelete();
		delete skPtr;
		skPtr=0;
	 }
	 RemoveAll();
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CLevelRangeObject * CLevelRangeList::GetLevelByExp(long exp)
{
	CLevelRangeObject * persist = NULL;
     for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
        CLevelRangeObject * skPtr = (CLevelRangeObject *)GetNext(posLoc);
	    if(exp >= skPtr->m_rangeStart)
           persist = skPtr;

	 }
	 return persist;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CRacialAdvantageObject::CRacialAdvantageObject()
{
     m_edbRef          = 0;
     m_adjustment      = 0.0f;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CRacialAdvantageList::Clone(CRacialAdvantageList ** clone)
{
     CRacialAdvantageList * copyLocal = new CRacialAdvantageList();

	 for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CRacialAdvantageObject * skPtr = (CRacialAdvantageObject *)GetNext(posLoc);
		 CRacialAdvantageObject * clonePtr = NULL;
         skPtr->Clone(&clonePtr);
         copyLocal->AddTail(clonePtr);
	 }

	 *clone = copyLocal;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CRacialAdvantageList::SafeDelete()
{
      for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CRacialAdvantageObject * skPtr = (CRacialAdvantageObject *)GetNext(posLoc);
	     delete skPtr;
		 skPtr = 0;
	  }
	  RemoveAll();
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CRacialAdvantageObject::Clone(CRacialAdvantageObject ** clone)
{
     CRacialAdvantageObject * copyLocal = new CRacialAdvantageObject();
     
	 copyLocal->m_adjustment   = m_adjustment;
     copyLocal->m_edbRef       = m_edbRef;

	 *clone = copyLocal;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
float CRacialAdvantageList::GetAdjustmentByEdbIndex(int edbIndex)
{
     for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CRacialAdvantageObject * skPtr = (CRacialAdvantageObject *)GetNext(posLoc);
	     if(skPtr->m_edbRef == edbIndex)
            return skPtr->m_adjustment;
	 }
	 return 1.0f;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CRacialAdvantageObject::Serialize( CArchive& ar )
{

CObject::Serialize( ar );
	if(ar.IsStoring() ){
	ar << m_adjustment
       << m_edbRef;
	   
	}
	else
	{
    ar >> m_adjustment
       >> m_edbRef;
	}
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CSkillObject::CSkillObject()
{

m_skillName           = "Unknown";
m_skillType           = 0;
m_currentExperience   = 0.0f;
m_currentLevel        = 0;
m_gainIncrement       = 0.1f;
m_levelRangeDB        = NULL;
m_raceAdvantageDB     = NULL;
m_basicAbilitySkill   = FALSE;//means that it only has one function like hiding
m_basicAbilityExpMastered = 0.0f;//component of ability basic skill option
m_basicAbilityFunction= 0;
m_skillGainsWhenFailed=0.0f;
m_maxMultiplier       = 3.0f;
m_minimalSave         = FALSE;
m_lastUseTime         = GetTickCount();
m_useInternalTimer    = FALSE;
m_currentSkillDelay   = 5000;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
int CSkillObject::UseSkill(long expOfTarget,
						   int levelToUse,
						   int ability,
						   int curDBCfg,
						   CInventoryObjList * currentInv,
						   int * specialFunction,
						   CStatObjList  *currentStats,
						   BOOL * updateStatsToClient,
						   CLevelRangeObject ** levelRaisedPtr,
						   int * returnMiscInt,
						   float totalCurrentExp,
						   int maxItemsInInventory,
						   CSkillObject * dbObjectRef,
						   int * effectBind,
						   long & curGlobalSkillDelay,
					       long & curPlayerSkillTimeStamp)
{
   //calculate speed cap
   if(totalCurrentExp == 0.0f)//prevent divide by zero
	  totalCurrentExp = 1.0f;

   float speedCapMultiplier = 10000.0f/totalCurrentExp;
   if(speedCapMultiplier > 1.0f)
	  speedCapMultiplier = 1.0f;
   else if(speedCapMultiplier < .1f)
	  speedCapMultiplier = .1f;

   if(!dbObjectRef->m_levelRangeDB)
	   return 19;

   if(!dbObjectRef->m_useInternalTimer)
   {
      if(((long)GetTickCount() - curPlayerSkillTimeStamp) < curGlobalSkillDelay)
      {//dont let him use skills too fast
         return 300;
      }//end dont let him use skills too fast
   }
   else
   {//skill based timer
      if(((long)GetTickCount() - m_lastUseTime) < m_currentSkillDelay)
	  {
         return 301;
	  }
   }//end skill based timer
   m_lastUseTime = GetTickCount();
   

   *returnMiscInt = 0;

   CLevelRangeObject * beforeUseLevel = dbObjectRef->m_levelRangeDB->GetLevelByExp((long)m_currentExperience);


   *specialFunction = -1;//default none

	  float racialAdvantageFactor = 1.0f;
	  if(dbObjectRef->m_raceAdvantageDB)//see if there is a racial advantage/disadvantage
		 racialAdvantageFactor = dbObjectRef->m_raceAdvantageDB->GetAdjustmentByEdbIndex(curDBCfg);

	  if(ability > -1){//if ability based

         if(levelToUse < 0)
	        return 2;//invalid level error

         POSITION levelPos = dbObjectRef->m_levelRangeDB->FindIndex(levelToUse);
	     if(!levelPos)
		    return 2;//invalid level error
         CLevelRangeObject * lPtr = (CLevelRangeObject *)dbObjectRef->m_levelRangeDB->GetAt(levelPos);
	  
       
         if(m_currentExperience < lPtr->m_rangeStart)
            return 21;//you lask the skill to attempt this

         if(lPtr->m_abilityDB)
		 {//valid ability DB
            POSITION posLoc = lPtr->m_abilityDB->FindIndex(ability);
	        if(posLoc)
			{//valid
               CSkillAbilityObject * aPtr = (CSkillAbilityObject *) lPtr->m_abilityDB->GetAt(posLoc);

			   *effectBind = aPtr->m_effectBind;
			   //check required items and remove them and proceed
			   if(aPtr->m_requiredStatistics)
			   {//verify stat req
                  if(!currentStats)
                      return 14;//stat req not met

			      for(POSITION stPos = aPtr->m_requiredStatistics->GetHeadPosition();stPos != NULL;)
				  {//req loop
				     CStatObj* statPtr = (CStatObj*) aPtr->m_requiredStatistics->GetNext(stPos);
                     float curVal = currentStats->GetStatValueByID(statPtr->m_id,currentInv);
                     if(curVal < statPtr->m_currentValue)
                        return 14;//stat req not met
				  }//end req loop
			   }//end verify stat req
			   
			   if(aPtr->m_miscInt2 != 0 && aPtr->m_miscInt3 == 0)
			   {//required item
                  if(!currentInv->VerifyInventory(aPtr->m_miscInt2))
				      return 38;//item not present
			   }//end required item
			   else if(aPtr->m_miscInt2 != 0 && aPtr->m_miscInt3 != 0)
			   {//required item and required exp obj
			      if(!currentInv->VerifyInventory(aPtr->m_miscInt2))
				      return 38;//item not present
				  if(!currentInv->IsExpItemObjectPresent(aPtr->m_miscInt2,aPtr->m_miscInt3))
				      return 39;//you lack the component
			   }//end required item and required exp obj
			   
			   if(aPtr->m_requiredItems)
			   {//required items valid
				  if(!currentInv)
					  return 8;//does not have required inventory items

				  POSITION curInvPos;
                  for(curInvPos = aPtr->m_requiredItems->GetHeadPosition();curInvPos != NULL;)
				  {//verify required items
                      CInventoryObj * reqInvPtr = (CInventoryObj *) aPtr->m_requiredItems->GetNext(curInvPos);
					  if(!currentInv->VerifyInventory(reqInvPtr->m_itemData->m_globalID))
						  return 8;//does not have required inventory items
				  }//end verify required items
                  for(curInvPos = aPtr->m_requiredItems->GetHeadPosition();curInvPos != NULL;)
				  {//remove required items
                      CInventoryObj * reqInvPtr = (CInventoryObj *) aPtr->m_requiredItems->GetNext(curInvPos);
					  currentInv->DeleteByGLID(reqInvPtr->m_itemData->m_globalID);
				  }//end verify required items
			   }//end remove items valid
              
			   

			      float multiplier = 1.0f;
                  float spread = aPtr->m_masterLevel-lPtr->m_rangeStart;
				  float posInLevel = m_currentExperience - lPtr->m_rangeStart;
                  if(posInLevel != 0)
					 multiplier = spread/posInLevel;

				  if(multiplier > dbObjectRef->m_maxMultiplier)
					 multiplier = dbObjectRef->m_maxMultiplier;
                   
				  double conv = spread;
				  float resultFromThrow = 0.0f;

                  if(posInLevel<spread)//otherwise its matered so leave at zero
				  resultFromThrow = (float)(rand()%((int)(spread+1.0f)));
                  if(resultFromThrow <= (posInLevel+1.0f))
				  {//successful throw
                     
					 if(multiplier <= 0)
						 multiplier = 1.0f;

					 float applicableBenefit = ((dbObjectRef->m_gainIncrement * multiplier)*racialAdvantageFactor)*speedCapMultiplier;
                     m_currentExperience += applicableBenefit;//apply exp gained

					 if(aPtr->m_resultItems)//success you gain items ;D
                        for(POSITION benInvPos = aPtr->m_resultItems->GetHeadPosition();benInvPos != NULL;)
						{//remove required items
                           CInventoryObj * additionalPtr = (CInventoryObj *) aPtr->m_resultItems->GetNext(benInvPos);
						   if(currentInv->GetCount() < maxItemsInInventory)
						      currentInv->AddInventoryItem(1,FALSE,additionalPtr->m_itemData);
						   else
							  return 40;
						}//end verify required items

					 *specialFunction = aPtr->m_functionType;
					 *returnMiscInt   = aPtr->m_miscInt;
					 if(currentStats)
                     if(aPtr->m_requiredStatistics)
					 {//access stats and use the appropriately
					   for(POSITION stPos = aPtr->m_requiredStatistics->GetHeadPosition();stPos != NULL;)
					   {//push required stats
					    CStatObj* statPtr = (CStatObj*) aPtr->m_requiredStatistics->GetNext(stPos);
					     if(currentStats->UseStatByID(statPtr->m_currentValue,statPtr->m_id,currentInv))
						 {//you have increased a stat
			                //inform client to update stats and notify
							*updateStatsToClient = TRUE;
						 }//end you have increased a stat
					   }//end push required stats
					 }//end access stats and use the appropriately

					 CLevelRangeObject * afterSuccessfulUse = dbObjectRef->m_levelRangeDB->GetLevelByExp((long)m_currentExperience);
					 if(afterSuccessfulUse)
                     if(beforeUseLevel->m_rangeStart != afterSuccessfulUse->m_rangeStart)
						*levelRaisedPtr = afterSuccessfulUse;

					 return 1;//successful attempt
				  }//end successful throw
				  else
				  {//unsuccessful throw
                     if(dbObjectRef->m_skillGainsWhenFailed > 0.0f)
					 {//skill will gain some even when failed
                        if(m_currentExperience < dbObjectRef->m_skillGainsWhenFailed)
						{//can still gain on failure
                           float curveMultiplier = 1.0f;
                           if(m_currentExperience != 0)
					          curveMultiplier = dbObjectRef->m_skillGainsWhenFailed/m_currentExperience;

						   if(curveMultiplier > dbObjectRef->m_maxMultiplier)
					          curveMultiplier = dbObjectRef->m_maxMultiplier;

                           float applicableBenefit = ((dbObjectRef->m_gainIncrement * multiplier)*racialAdvantageFactor)*curveMultiplier;
                           m_currentExperience += applicableBenefit;//apply exp gained

						   CLevelRangeObject * afterSuccessfulUse = dbObjectRef->m_levelRangeDB->GetLevelByExp((long)m_currentExperience);
						   if(afterSuccessfulUse)
						   if(beforeUseLevel->m_rangeStart != afterSuccessfulUse->m_rangeStart)
								*levelRaisedPtr = afterSuccessfulUse;
						}//end can still gain on failure
					 }//end skill will gain some even when failed

                     return 10;//unsuccessful attempt
				  }//end unsuccessful throw
			}//end valid
		 }//end valid ability DB
	     else 
	        return 0;//FAILED
	  }//end if ability based
      else
	  {//non ability skill based-linear stuffola
		  if(m_currentExperience == 0)
			 m_currentExperience = 10.0f;
		  
		  
		  if(!dbObjectRef->m_basicAbilitySkill)
		  {//derived by player skill use (contact etc)
		     float multiplier = expOfTarget/m_currentExperience;
		     if(multiplier > dbObjectRef->m_maxMultiplier)
		        multiplier = dbObjectRef->m_maxMultiplier;
             float applicableBenefit = ((dbObjectRef->m_gainIncrement * multiplier)*racialAdvantageFactor);//*speedCapMultiplier;
             m_currentExperience += applicableBenefit;
			 CLevelRangeObject * afterSuccessfulUse = dbObjectRef->m_levelRangeDB->GetLevelByExp((long)m_currentExperience);
                     if(afterSuccessfulUse)
					 if(beforeUseLevel->m_rangeStart != afterSuccessfulUse->m_rangeStart)
						*levelRaisedPtr = afterSuccessfulUse;
		  }//end derived by player skill use (contact etc)
		  else{//basic ability skill
             float multiplier = 1.0f;
             float spread = dbObjectRef->m_basicAbilityExpMastered;
		     float posInLevel = m_currentExperience;
             if(posInLevel != 0)
			    multiplier = spread/posInLevel;

			    if(multiplier > dbObjectRef->m_maxMultiplier)
			       multiplier = dbObjectRef->m_maxMultiplier;
                   
			    double conv = spread;
			    float resultFromThrow = 0.0f;

                  if(posInLevel<spread)//otherwise its matered so leave at zero
				  resultFromThrow = (float)(rand()%((int)spread+1));
                  if(resultFromThrow <= (posInLevel+1.0f))
				  {//successful throw
                     float applicableBenefit = ((dbObjectRef->m_gainIncrement * multiplier)*racialAdvantageFactor)*speedCapMultiplier;
                     m_currentExperience += applicableBenefit;//apply exp gained
					 *specialFunction = dbObjectRef->m_basicAbilityFunction;

                     CLevelRangeObject * afterSuccessfulUse = dbObjectRef->m_levelRangeDB->GetLevelByExp((long)m_currentExperience);
                     if(afterSuccessfulUse)
					 if(beforeUseLevel->m_rangeStart != afterSuccessfulUse->m_rangeStart)
						*levelRaisedPtr = afterSuccessfulUse;

					 return 1;//successful attempt
				  }//end successful throw
				  else{//unsuccessful throw
					 if(dbObjectRef->m_skillGainsWhenFailed > 0.0f)
					 {//skill will gain some even when failed
                        if(m_currentExperience < dbObjectRef->m_skillGainsWhenFailed)
						{//can still gain on failure
                           float curveMultiplier = 1.0f;
                           if(m_currentExperience != 0)
					          curveMultiplier = dbObjectRef->m_skillGainsWhenFailed/m_currentExperience;

						   if(curveMultiplier > dbObjectRef->m_maxMultiplier)
					          curveMultiplier = dbObjectRef->m_maxMultiplier;

                           float applicableBenefit = ((dbObjectRef->m_gainIncrement * multiplier)*racialAdvantageFactor)*curveMultiplier;
                           m_currentExperience += applicableBenefit;//apply exp gained

						    CLevelRangeObject * afterSuccessfulUse = dbObjectRef->m_levelRangeDB->GetLevelByExp((long)m_currentExperience);
							if(afterSuccessfulUse)
							if(beforeUseLevel->m_rangeStart != afterSuccessfulUse->m_rangeStart)
								*levelRaisedPtr = afterSuccessfulUse;
						}//end can still gain on failure
					 }//end skill will gain some even when failed
                     return 10;//unsuccessful attempt
				  }//end unsuccessful throw
		  }//end basic ability skill
	  }//end non ability skill based-and linear stufola

   return 1;//ok
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CSkillObject::SafeDelete()
{
	if(m_levelRangeDB)
	{
       m_levelRangeDB->SafeDelete();
	   delete m_levelRangeDB;
       m_levelRangeDB = 0;
	}

	if(m_raceAdvantageDB)
	{
       m_raceAdvantageDB->SafeDelete();
	   delete m_raceAdvantageDB;
	   m_raceAdvantageDB = 0;
	}

}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CSkillObject::Clone(CSkillObject ** clone)
{
     CSkillObject * sklObj = new CSkillObject();
     sklObj->m_currentExperience    = m_currentExperience;
	 sklObj->m_currentLevel         = m_currentLevel;
	 sklObj->m_gainIncrement        = m_gainIncrement;
	 sklObj->m_skillName            = m_skillName;
	 sklObj->m_skillType            = m_skillType;
	 sklObj->m_basicAbilitySkill    = m_basicAbilitySkill;
     sklObj->m_basicAbilityExpMastered = m_basicAbilityExpMastered;
	 sklObj->m_basicAbilityFunction = m_basicAbilityFunction;
     sklObj->m_skillGainsWhenFailed = m_skillGainsWhenFailed;
     sklObj->m_maxMultiplier        = m_maxMultiplier;
	 sklObj->m_useInternalTimer     = m_useInternalTimer;

	 if(m_levelRangeDB)
		m_levelRangeDB->Clone(&sklObj->m_levelRangeDB);

     if(m_raceAdvantageDB)
		m_raceAdvantageDB->Clone(&sklObj->m_raceAdvantageDB);

	 *clone = sklObj;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CSkillObject::CloneMinimum(CSkillObject ** clone)
{
     CSkillObject * sklObj          = new CSkillObject();
     sklObj->m_currentExperience    = m_currentExperience;
	 sklObj->m_currentLevel         = m_currentLevel;
	 sklObj->m_skillType            = m_skillType;

	 *clone = sklObj;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
BOOL CSkillObject::EvaluateLevelAndTitle(CString & title,int & level,CSkillObject * skillRef)
{

    if(!skillRef->m_levelRangeDB)
		return FALSE;

    int levelTrace = 1;
	for(POSITION posLoc = skillRef->m_levelRangeDB->GetHeadPosition();posLoc != NULL;)
	{
        CLevelRangeObject * lrObject = (CLevelRangeObject *) skillRef->m_levelRangeDB->GetNext(posLoc);
        
		
		if(m_currentExperience >= lrObject->m_rangeStart)
		{//in range
           title = lrObject->m_levelTitle;
		   level = levelTrace;
		}//end in range
		levelTrace++;
	}

	return TRUE;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
BOOL CSkillObject::EvaluateLevel(int & level,CSkillObject * skillRef)
{

    if(!skillRef->m_levelRangeDB)
		return FALSE;

    int levelTrace = 1;
	for(POSITION posLoc = skillRef->m_levelRangeDB->GetHeadPosition();posLoc != NULL;)
	{
        CLevelRangeObject * lrObject = (CLevelRangeObject *) skillRef->m_levelRangeDB->GetNext(posLoc);
        if(m_currentExperience >= lrObject->m_rangeStart)
		{//in range
		   level = levelTrace;
		}//end in range
		levelTrace++;
	}

	return TRUE;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
int CSkillObject::GetPercentageToNextLevel(CSkillObject * skillPtrRef)
{

    if(!skillPtrRef->m_levelRangeDB)
		return 0;

	CLevelRangeObject * finalLevel = NULL;
    int levelTrace = 1;
	for(POSITION posLoc = skillPtrRef->m_levelRangeDB->GetHeadPosition();posLoc != NULL;)
	{
        CLevelRangeObject * lrObject = (CLevelRangeObject *) skillPtrRef->m_levelRangeDB->GetNext(posLoc);
        if(m_currentExperience >= lrObject->m_rangeStart)
		{//in range
		   finalLevel = lrObject;
		}//end in range
	}

	//caclulate percentages
    if(finalLevel)
	{
       float range = (float)(finalLevel->m_rangeEnd-finalLevel->m_rangeStart);
       float levelStart = (float)finalLevel->m_rangeStart;
       float positionInLevel = m_currentExperience - levelStart;
       if(positionInLevel >= range)
		  return 100;

	   if(range == 0)
		  return 0;
   
	      float multiplier = positionInLevel/range;
          return ((int)(100.0f*multiplier));
	}
	return 0;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CSkillObject::Serialize( CArchive& ar )
{
char version = 0;

float floatReserved = 0.0f;
int   intReserved   = 0;
CSkillAbilityList * abilityDB = NULL;

CObject::Serialize( ar );
	if(ar.IsStoring() ){
	
	 if(!m_minimalSave)
	 {
	  version = 2;
      ar << version;

	  ar << m_skillName
         << m_skillGainsWhenFailed
         << m_currentExperience 
         << m_skillType 
         << m_maxMultiplier 
         << m_currentLevel
	     << m_levelRangeDB
	     << abilityDB
	     << m_gainIncrement
	     << m_raceAdvantageDB
	     << m_basicAbilitySkill
	     << m_basicAbilityExpMastered
	     << m_basicAbilityFunction
		 << m_useInternalTimer
		 << m_filterEvalLevel
		 << intReserved
		 << intReserved;
	 }
	 else{//minimal save
		 version = 1;
         ar << version;
		 
		 ar << m_currentExperience 
            << m_skillType 
            << m_currentLevel;
	 }//end minimal save
	 m_minimalSave = FALSE;  
	}
	else
	{
      ar >> version;
	  //version = 0;

	  m_filterEvalLevel = 0;

	  if(version == 1)
	  {
         ar >> m_currentExperience 
            >> m_skillType 
            >> m_currentLevel;
	  }
	  else if(version == 0)
	  {
      ar >> m_skillName
         >> m_skillGainsWhenFailed
         >> m_currentExperience 
         >> m_skillType 
         >> m_maxMultiplier 
         >> m_currentLevel
	     >> m_levelRangeDB
	     >> abilityDB
	     >> m_gainIncrement
	     >> m_raceAdvantageDB
	     >> m_basicAbilitySkill
	     >> m_basicAbilityExpMastered
	     >> m_basicAbilityFunction
		 >> m_useInternalTimer;
	  }
	  else if(version == 2)
	  {
      ar >> m_skillName
         >> m_skillGainsWhenFailed
         >> m_currentExperience 
         >> m_skillType 
         >> m_maxMultiplier 
         >> m_currentLevel
	     >> m_levelRangeDB
	     >> abilityDB
	     >> m_gainIncrement
	     >> m_raceAdvantageDB
	     >> m_basicAbilitySkill
	     >> m_basicAbilityExpMastered
	     >> m_basicAbilityFunction
		 >> m_useInternalTimer
		 >> m_filterEvalLevel
		 >> intReserved
		 >> intReserved;
	  }
	  
	 
	  m_minimalSave = FALSE;  
	  m_lastUseTime = GetTickCount();
	  //m_useInternalTimer = FALSE;
	  m_currentSkillDelay   = 5000;
	}
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CSkillObject::SerializeAlloc( CArchive& ar )
{
	if(ar.IsStoring() ){
		 ar << m_currentExperience 
            << m_skillType 
            << m_currentLevel;
	}
	else
	{
         ar >> m_currentExperience 
            >> m_skillType 
            >> m_currentLevel;
	}
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CSkillObjectList::SerializeAlloc(CArchive& ar,int loadCount)
{
	if(ar.IsStoring() ){	
	   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
           CSkillObject * elementPtr = (CSkillObject *)GetNext(posLoc);
		   elementPtr->SerializeAlloc(ar);
	   }
	}
	else
	{
       for(int i=0;i<loadCount;i++)
	   {
		   CSkillObject * elementPtr = new CSkillObject();
           elementPtr->SerializeAlloc(ar);
		   AddTail(elementPtr);
	   }
	}
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CSkillObjectList::CSkillObjectList()
{
	m_skillToDisplay	= -1;
	m_totalExperience	= 0;
	m_speedGainCap		= 15000.0f;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
int CSkillObjectList::UseSkillByType( int type,
									  CSkillObjectList * dbList,
									  long expOfTarget,
						              int levelToUse,
						              int ability,
						              int curDBCfg,
						              CInventoryObjList * currentInv,
						              int * specialFunction,
									  CSkillObject ** newAddedSkill,
									  CStatObjList  *currentStats,
						              BOOL * updateStatsToClient,
									  CLevelRangeObject ** levelRaised,
									  CSkillObject ** skillPtr,
									  int * returnMiscInt,
									  int maxItemsInInventory,
									  int * effectBind,
									  long & curGlobalSkillDelay,
									  long & curPlayerSkillTimeStamp)
{
    //see if we have used this skill before-if not add it to our database
	CSkillObject * skPtr = this->GetSkillByType(type);
    CSkillObject * dbSkillPtr = dbList->GetSkillByType(type);
	*effectBind = -1;
	
    if(!dbSkillPtr)
        return FALSE;//safty first

	if(!skPtr)
	{//need to add this skill
	   //if(!dbSkillPtr)
		//  return FALSE;
       dbSkillPtr->Clone(&skPtr);
       AddTail(skPtr);
	   *newAddedSkill = skPtr;//reference this
	}//end need to add this skill



    float totalCurrentExp = UpdateTotalExperienceSum();

	*skillPtr = skPtr;
	return skPtr->UseSkill(expOfTarget,
                           levelToUse,
					       ability,
					       curDBCfg,
                           currentInv,
                           specialFunction,
						   currentStats,
						   updateStatsToClient,
						   levelRaised,
						   returnMiscInt,
						   totalCurrentExp,
						   maxItemsInInventory,
						   dbSkillPtr,
						   effectBind,
						   curGlobalSkillDelay,
						   curPlayerSkillTimeStamp);
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CSkillObject * CSkillObjectList::GetSkillByType(int type)
{
	 for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CSkillObject * skPtr = (CSkillObject *)GetNext(posLoc);
		 if(skPtr->m_skillType == type)
            return skPtr;
	 }
	 return NULL;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CSkillObject * CSkillObjectList::GetSkillByName(CString skillName)
{
	 for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CSkillObject * skPtr = (CSkillObject *)GetNext(posLoc);
		 if(skPtr->m_skillName.CompareNoCase(skillName)==0)
            return skPtr;
	 }
	 return NULL;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
int CSkillObjectList::GetEffeciencyByTypeAndLevel(int type,
													CSkillObjectList * charSkillList,
													int * proficiency,
													int * perfective)
{
     if(!charSkillList)
		 return 0;

	 CSkillObject * skillPtr = GetSkillByType(type);
	 if(!skillPtr)
		 return 0;

	 CSkillObject * charSkill = charSkillList->GetSkillByType(type);
	 if(!charSkill)
		 return 0;

     if(!skillPtr->m_levelRangeDB)
		 return 0;

	 
     int levelTrace = 1;
	 for(POSITION posLoc = skillPtr->m_levelRangeDB->GetHeadPosition();posLoc != NULL;)
	 {
        CLevelRangeObject * lrObject = (CLevelRangeObject *) skillPtr->m_levelRangeDB->GetNext(posLoc);
        
		
		if(charSkill->m_currentExperience >= lrObject->m_rangeStart)
		{//in range

           *proficiency = lrObject->m_profficiencyBonus;
           *perfective  = lrObject->m_perfectiveBonus;
		}//end in range
		else
		    return 0;

		levelTrace++;
	 }
	 return 0;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CSkillObject * CSkillObjectList::GetSkillByIndex(int index)
{
	 int trace = 0;
	 for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CSkillObject * skPtr = (CSkillObject *)GetNext(posLoc);
		 if(trace == index)
            return skPtr;
		 trace++;
	 }
	 return NULL;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
int CSkillObjectList::GetSkillIndexByType(int type)
{
	 int trace = 0;
	 for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CSkillObject * skPtr = (CSkillObject *)GetNext(posLoc);
		 if(skPtr->m_skillType == type)
            return trace;
		 trace++;
	 }
	 return -1;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CString CSkillObjectList::GetTitleByLevelAndType(int type,int level)
{
	 CSkillObject * skillPtr = GetSkillByType(type);
     if(skillPtr)
     if(skillPtr->m_levelRangeDB)
	 {
        CLevelRangeObject * levelPtr = skillPtr->m_levelRangeDB->GetObjByIndex(level-1);
		if(levelPtr)
		{
		   return levelPtr->m_levelTitle;
		}
	 }
	 return "";
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
/*CString CSkillObjectList::GetTitleByLevelAndType(int type,int level)
{
	 CSkillObject * skillPtr = GetSkillByType(type);
     if(skillPtr)
     if(skillPtr->m_levelRangeDB)
	 {
        CLevelRangeObject * levelPtr = skillPtr->m_levelRangeDB->GetObjByIndex(level-1);
		if(levelPtr)
		{
		   return levelPtr->m_levelTitle;
		}
	 }
	 return "";
}*/
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
BOOL CSkillObjectList::EvaluateRequiredSkillLevel(int type,int requiredLevel,CSkillObjectList * skillDBRef)
{
	 CSkillObject * skillPtr = GetSkillByType(type);
	 CSkillObject * skillPtrRef = skillDBRef->GetSkillByType(type);
     if(skillPtr)
	 {
		int curLevel = 1;
	    skillPtr->EvaluateLevel(curLevel,skillPtrRef);
		if(curLevel >= requiredLevel)
           return TRUE;
	 }
	 return FALSE;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CSkillObject* CSkillObjectList::GetBestSkill(CSkillObjectList * skillDB)
{
   if(!skillDB)
	   return NULL;

	CSkillObject * highestSkill = NULL;
	int curhighestLevel = 1;
	int testLevel = 1;
    for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
        CSkillObject * skPtr = (CSkillObject *)GetNext(posLoc);

		CSkillObject * skillPtrRef = skillDB->GetSkillByType(skPtr->m_skillType);
		if(!skillPtrRef)
             continue;
	    if(!highestSkill)
		{
			skPtr->EvaluateLevel(curhighestLevel,skillPtrRef);
		    highestSkill = skPtr;
			continue;
		}
		if(skPtr->EvaluateLevel(testLevel,skillPtrRef))
		{
           if(curhighestLevel < testLevel)
		   {
              highestSkill = skPtr;
			  curhighestLevel = testLevel;
		   }
		}
	}
	return highestSkill;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CSkillObject* CSkillObjectList::GetBestSkillWEval(CSkillObjectList * skillDB)
{
   if(!skillDB)
	   return NULL;

	CSkillObject * highestSkill = NULL;
	int curhighestLevel = 1;
	int testLevel = 1;
    for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
        CSkillObject * skPtr = (CSkillObject *)GetNext(posLoc);

		CSkillObject * skillPtrRef = skillDB->GetSkillByType(skPtr->m_skillType);
		if(!skillPtrRef)
           continue;

		if(skillPtrRef->m_filterEvalLevel)
           continue;


	    if(!highestSkill)
		{
			skPtr->EvaluateLevel(curhighestLevel,skillPtrRef);
		    highestSkill = skPtr;
			continue;
		}
		if(skPtr->EvaluateLevel(testLevel,skillPtrRef))
		{
           if(curhighestLevel < testLevel)
		   {
              highestSkill = skPtr;
			  curhighestLevel = testLevel;
		   }
		}
	}
	return highestSkill;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CSkillObjectList::Clone(CSkillObjectList ** clone)
{
     CSkillObjectList * copyLocal = new CSkillObjectList();

	 for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CSkillObject * skPtr = (CSkillObject *)GetNext(posLoc);
		 CSkillObject * clonePtr = NULL;
         skPtr->Clone(&clonePtr);
         copyLocal->AddTail(clonePtr);
	 }

	 *clone = copyLocal;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CSkillObjectList::CloneMinimum(CSkillObjectList ** clone)
{
     CSkillObjectList * copyLocal = new CSkillObjectList();

	 for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CSkillObject * skPtr = (CSkillObject *)GetNext(posLoc);
		 CSkillObject * clonePtr = NULL;
         skPtr->CloneMinimum(&clonePtr);
         copyLocal->AddTail(clonePtr);
	 }

	 *clone = copyLocal;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CSkillObjectList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
        CSkillObject * skPtr = (CSkillObject *)GetNext(posLoc);
		skPtr->SafeDelete();
		delete skPtr;
		skPtr=0;
	}
	RemoveAll();
}
//------------------------------------------------------------//
//------------------------------------------------------------//
//------------------------------------------------------------//
int CSkillObjectList::GetFreeID()
{
    int trace = 1;

	BOOL done = FALSE;
    while(!done)
	{
      done = TRUE;
	  for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
          CSkillObject * spnPtr = (CSkillObject *)GetNext(posLoc);
	      if(spnPtr->m_skillType == trace)
		  {
		      done = FALSE;
			  trace++;
			  break;
		  }
	  }

	}

    return trace;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
float CSkillObjectList::UpdateTotalExperienceSum()
{
	m_totalExperience = 0.0f;
    for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
        CSkillObject * skPtr = (CSkillObject *)GetNext(posLoc);
		m_totalExperience += skPtr->m_currentExperience;
	}
	return m_totalExperience;
}
//-------------------------------------------------------------------//
void CSkillObjectList::ExportToSQL()
{
    ConnectionStructureClass connectionStructTemp;
    connectionStructTemp.AquireFromCfgFile("baseAccount.cfg");
	
    CDBConnectionSystem * dbConnection = new CDBConnectionSystem(connectionStructTemp.m_sqlServerIP,
			                                                     connectionStructTemp.m_sqlServerCatalog,
																 connectionStructTemp.m_sqlServerUsername,
																 connectionStructTemp.m_sqlServerPassword,
																 NULL);


	if(!dbConnection->m_connectionEstablished)
	{
        return;
	}

    CString clearFromGil		= "DELETE FROM GM_SKILLS";
    CString addObject           = "INSERT INTO GM_SKILLS (SKILLNAME, SKILLTYPE) Values('$SKILLNAME','$SKILLTYPE')";

	CABFunctionLib cablib;
	CString temp;
	dbConnection->ExecQuery(clearFromGil);

	int trace = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CSkillObject * objPtr  = (CSkillObject *)GetNext(posLoc);
		
        temp = addObject;
		temp.Replace("$SKILLNAME",objPtr->m_skillName);
		temp.Replace("$SKILLTYPE",cablib.ConvToString(objPtr->m_skillType));
		dbConnection->ExecQuery(temp);
		trace++;
	}

	delete dbConnection;
	dbConnection=0;
}


//-------------------------------------------------------------------//
BOOL CSkillObjectList::ExportToSQL(CString ip, CString database, CString usename, CString password)
{
	
    CDBConnectionSystem * dbConnection = new CDBConnectionSystem(ip,
			                                                     database,
																 usename,
																 password,
																 NULL);


	if(!dbConnection->m_connectionEstablished)
	{
        return FALSE;
	}

    CString clearFromGil		= "DELETE FROM GM_SKILLS";
    CString addObject           = "INSERT INTO GM_SKILLS (SKILLNAME, SKILLTYPE) Values('$SKILLNAME','$SKILLTYPE')";

	CABFunctionLib cablib;
	CString temp;
	dbConnection->ExecQuery(clearFromGil);

	int trace = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CSkillObject * objPtr  = (CSkillObject *)GetNext(posLoc);
		
        temp = addObject;
		temp.Replace("$SKILLNAME",objPtr->m_skillName);
		temp.Replace("$SKILLTYPE",cablib.ConvToString(objPtr->m_skillType));
		dbConnection->ExecQuery(temp);
		trace++;
	}

	delete dbConnection;
	dbConnection=0;
	return TRUE;
}

IMPLEMENT_SERIAL(CSkillAbilityObject, CObject, 0)
IMPLEMENT_SERIAL(CSkillAbilityList, CObList, 0)
IMPLEMENT_SERIAL(CSkillObject, CObject, 0)
IMPLEMENT_SERIAL(CSkillObjectList, CObList, 0)
IMPLEMENT_SERIAL(CLevelRangeObject, CObject, 0)
IMPLEMENT_SERIAL(CLevelRangeList, CObList, 0)
IMPLEMENT_SERIAL(CRacialAdvantageObject, CObject, 0)
IMPLEMENT_SERIAL(CRacialAdvantageList, CObList, 0)


