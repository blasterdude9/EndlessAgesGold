#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>

#include "CAIICollectionQuest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAICollectionQuestObj::CAICollectionQuestObj()
{
	m_index					   = 0;
    m_title                    = "Unknown";
    m_questDesription          = "Quest Description";
    m_questCompletionMessage   = "Quest Completion";
    m_type                     = 0;//0= collection quest
    m_requiredItems            = NULL;
    m_reward                   = NULL;
    m_skillTypeBonus           = -1;
    m_expBonus                 = 0.0f;
	m_maxExpLimit              = 0.0f;

	m_spawnAiCfg               = -1;//-1 =  none/ no spawn
	ZeroMemory(&m_spawnPosition,sizeof(D3DVECTOR));
	m_worldID                 = 0;
	m_spawnedAiLifeline       = 0;
	m_randomRewardType        = FALSE;
	m_spawnToLocationOverride = FALSE;
	m_makeTargetRebirthPoint  = FALSE;
	m_spawnStamp              = GetTickCount();
	m_raceSpecificAccess      = -1;
	m_miscAction              = 0;
	m_completedKeyEnabled     = 0;
	m_progessBasedByQuestJournal = FALSE;
	m_cashReward              = 0;
	m_requiredSkillType       = 0;
    m_requiredSkillLevel      = 0;
    m_mobToHuntID             = 0;
    m_maxDropCount            = 0;
    m_dropGlid                = 0;
    m_dropPercentage          = 100;
	m_autocomplete            = FALSE;
	m_openDialogWindow        = FALSE;
	m_optionalIntruction       = "";
	m_listProgressionOptions  = FALSE;
	m_undeletable             = FALSE;
	m_spawnOnFail             = FALSE;
	m_portalLimitMsg          = "";
	m_portalLimitID           = 0;
	m_closeJournalEntryByName = "";
	m_expansionIDReq          = 0;
	m_flagHostileOnSuccess    = 0;
	m_requireOwnershipOfKingdomIndex = 0;
    m_kingdomID				  = 0;
	m_cashContribution        = 0;
	m_closeJournalOnSuccess   = FALSE;
	m_deleteByLocationID      = FALSE;
	m_deleteAllByRequiredGLID = FALSE;
	m_deteteAllByRewardGLID	  = FALSE;
	m_despawnOnReward		  = FALSE;
	m_flagTimer			      = FALSE;
	m_requiredTimeStamp       = 0;
	m_maxDmgMod               = 0;
	m_minDmgMod               = 0;
	m_spawnArrayCountOption   = 0;//>0 activates
	m_spawnArrayOffset	      = 0.0f;//radius offset
	m_spawnRotationRange	  = 0;
	m_instanceGenerateType    = 0;//0=none 1=group based 2=arena based
	m_instanceGenerateParam   = 0;//m_instanceGenerateType: if arena then its time based
	m_instanceGenerateParam2  = 0;
	m_instanceGenerateParam3  = 0;//*sometimes if arena minimum before start timer begin
	m_dwMem					  = NULL;
	m_dwMemCount		      = 0;
	m_ruleCount			      = 0;
	m_rules					  = NULL;
    m_instanceGenerateParam4  = 0;//0=disabled
	m_arenaStartTimerStamp    = 0;//0=disabled
	m_arenaBetPool			  = 0;
}

void CAICollectionQuestObj::SafeDelete()
{
    if(m_requiredItems){
       m_requiredItems->SafeDelete();
	   delete m_requiredItems;
	   m_requiredItems = 0;
	}

    if(m_reward){
	   m_reward->SafeDelete();
       delete m_reward;
	   m_reward = 0;
	}

	if(m_dwMem)
	{
       delete [] m_dwMem;
       m_dwMem = 0;
	   m_dwMemCount = 0;
	}

	if(m_rules)
	{
	   delete [] m_rules;
	   m_rules = 0;
	}
}

int CAICollectionQuestObj::AddToDWMEM(int maxCount,long dwVar)
{
     if(maxCount < 1)
	    return 0;

	 m_dwMemCount = maxCount;

     if(!m_dwMem)
	 {
		 m_dwMem = new long[maxCount];
		 memset(m_dwMem,-1,maxCount*sizeof(long));
	 }

	 for(int i=0;i<maxCount;i++)
	 {//dont let him signup twice
         if(m_dwMem[i] == dwVar)
			return 9;//already signed up
	 }
     
	 for(int i=0;i<maxCount;i++)
	 {
         if(m_dwMem[i] == -1)
		 {
            m_dwMem[i] = dwVar;//assign val
            if(i == (maxCount-1))
			   return 1;//full
			return 2;//not full yet
		 }
	 }
	 return 1;//full
}

bool
CAICollectionQuestObj::RemoveFromDWMEM(long dwVar)
{
	// ERROR checking: Does the underlying collection exist?
	if (!this->m_dwMem
	|| (this->m_dwMemCount <= 0))
		return false;

	// LOOP through the collection and find the target ID
	for (int i=0; i<this->m_dwMemCount; i++)
	{
		// IF we've found the target ID...
		if(this->m_dwMem[i] == dwVar)
		{
			// REMOVE the target ID from the collection
			this->m_dwMem[i] = -1;
				return true;
		}
	}
	return false;
}

BOOL CAICollectionQuestObj::VerifyInDWMEM(long dwVar)
{
     if(!m_dwMem)
	    return 0;
     
	 for(int i=0;i<m_dwMemCount;i++)
	 {
         if(m_dwMem[i] == dwVar)
		    return TRUE;
	 }
	 return FALSE;//full
}

int CAICollectionQuestObj::DWMEM_getCount()
{
     if(!m_dwMem)
	    return 0;
     
	 int trace = 0;
	 for(int i=0;i<m_dwMemCount;i++)
	 {
         if(m_dwMem[i] != -1)
		    trace++;
	 }
	 return trace;//full
}

void CAICollectionQuestObj::ClearDWMEM()
{
    if(m_dwMem)
	{
       delete [] m_dwMem;
       m_dwMem = 0;
	}
	m_dwMemCount = 0;
}

BOOL CAICollectionQuestObj::EvaluateQuest(CInventoryObjList * playersInventory,
										  CSkillObjectList  * skillDatabase,
										  CSkillObjectList  * skillMainDatabase,
										  float * expGained,
										  CPlayerObject * currentPlayer,
										  int * optErrorCode,
										  CServerCurrencySystem * currencyDatabase,
										  CItemManager	* m_itemManager
										  )
{
    *optErrorCode = 0;

    if(!m_requiredItems)
        return FALSE;

	if(!m_reward)
        return FALSE;

	if(!playersInventory)
        return FALSE;


	if(this->m_requiredTimeStamp > 0)
	{
       if(GetTickCount()-currentPlayer->m_questTimer > this->m_requiredTimeStamp)
	      return FALSE;
	}

	if(this->m_raceSpecificAccess > -1)
		if(this->m_raceSpecificAccess != currentPlayer->m_curEDBIndex)
			return FALSE;

	*optErrorCode = 1;

	CInventoryObjList * tempHoldingList = NULL;
    playersInventory->Clone(&tempHoldingList);
	
	BOOL hasRequiredItems = TRUE;
	BOOL hasRequiredCurrency = TRUE;
    POSITION posLoc;
	BOOL isCurrency = FALSE;

	for(posLoc = m_requiredItems->GetHeadPosition();posLoc != NULL;)
	{//verification loop
        CInventoryObj * invObj = (CInventoryObj *)m_requiredItems->GetNext(posLoc);
		isCurrency = FALSE;
		for(CAxLinkObj *curLoc = currencyDatabase->GetHeadPosition_unsafe(); curLoc != NULL;)
		{
			CServerCurrency *curObj = (CServerCurrency *)currencyDatabase->GetNext_unsafe(&curLoc);
			if(curObj->m_glid == invObj->m_itemData->m_globalID)
			{
				isCurrency = TRUE;
				break;
			}
		}
		if(!isCurrency)
		{
			if(!tempHoldingList->DeleteByGLIDX(invObj->m_itemData->m_globalID,invObj->m_quanity))
			{//no go
				hasRequiredItems = FALSE;
				break;
			}//end no go
		}
	}//end verification loop

	int amountOfMDBNeeded = currencyDatabase->EvalCurrency(m_requiredItems);
	if(amountOfMDBNeeded)
	{
		int amountofMDBinInventory = currencyDatabase->EvalCurrency(tempHoldingList);
		if(amountofMDBinInventory < amountOfMDBNeeded)
			hasRequiredCurrency = FALSE;
	}

	tempHoldingList->SafeDelete();
	delete tempHoldingList;
	tempHoldingList = 0;

	if(!hasRequiredItems || !hasRequiredCurrency)       
        return FALSE;//havent completed it

	*optErrorCode = 2;

    if(m_clearInventory)
	{
        playersInventory->RemoveAllExceptPerminentNONdrop();
	}
	
	if(m_deleteByLocationID)
	{
        for(posLoc = m_reward->GetHeadPosition();posLoc != NULL;)
        {//remove requirement item loop
            CInventoryObj * invObj = (CInventoryObj *)m_reward->GetNext(posLoc);
	        playersInventory->DeleteByLOCIDAll(invObj->m_itemData->m_locationID);
        }//end remove requirement item loop
	}

	if(m_deleteAllByRequiredGLID)
	{
        for(posLoc = m_requiredItems->GetHeadPosition();posLoc != NULL;)
        {//remove requirement item loop
            CInventoryObj * invObj = (CInventoryObj *)m_requiredItems->GetNext(posLoc);
	        playersInventory->DeleteByGLIDAll(invObj->m_itemData->m_globalID);
        }//end remove requirement item loop
	}
	if(m_deteteAllByRewardGLID)
	{
		for(posLoc = m_reward->GetHeadPosition();posLoc != NULL;)
        {//remove requirement item loop
            CInventoryObj * invObj = (CInventoryObj *)m_reward->GetNext(posLoc);
	        playersInventory->DeleteByGLIDAll(invObj->m_itemData->m_globalID);
        }//end remove requirement item loop
	}
	
    //if you made it this far he has done quest correctly
    for(posLoc = m_requiredItems->GetHeadPosition();posLoc != NULL;)
	{//remove requirement item loop
        CInventoryObj * invObj = (CInventoryObj *)m_requiredItems->GetNext(posLoc);
		isCurrency = FALSE;
		for(CAxLinkObj *curLoc = currencyDatabase->GetHeadPosition_unsafe(); curLoc != NULL;)
		{
			CServerCurrency *curObj = (CServerCurrency *)currencyDatabase->GetNext_unsafe(&curLoc);
			if(curObj->m_glid == invObj->m_itemData->m_globalID)
			{
				isCurrency = TRUE;
				break;
			}
		}
		if(!isCurrency)
			playersInventory->DeleteByGLIDX(invObj->m_itemData->m_globalID,invObj->m_quanity);
	}//end remove requirement item loop

	if(amountOfMDBNeeded)
		currencyDatabase->RemoveCurrencyIfEnoughExists(m_itemManager,playersInventory,-1,amountOfMDBNeeded);

	if(!m_randomRewardType)
	{//regular reward type
      for(posLoc = m_reward->GetHeadPosition();posLoc != NULL;)
	  {//remove requirement item loop
        CInventoryObj * invObj = (CInventoryObj *)m_reward->GetNext(posLoc);
		int dmgModFinal = 0;
		if(this->m_maxDmgMod > 0)
		{//bonus add dmg bon us to rewards
           int range = this->m_maxDmgMod - this->m_minDmgMod;
		   if(range > 0)
		   {
              dmgModFinal = this->m_minDmgMod + (rand()%range);
		   }
		   else
		      dmgModFinal = this->m_maxDmgMod;

		   invObj->m_itemData->m_damageModifier = dmgModFinal;
		}//end bonus add dmg bon us to rewards
		
		if(invObj->m_quanity > 0)
	       playersInventory->AddInventoryItem(invObj->m_quanity,FALSE,invObj->m_itemData);
		else
		   playersInventory->AddInventoryItem(1,FALSE,invObj->m_itemData);
	  }//end remove requirement item loop
	}//end regular reward type
	else if((int)m_reward->GetCount()>0){//random reward
            int selectionCur = rand()%((int)m_reward->GetCount());
			if(selectionCur>-1)
			{
			   POSITION rendPos = m_reward->FindIndex(selectionCur);
			   if(rendPos)
			   {
				  CInventoryObj * invObj = (CInventoryObj *)m_reward->GetAt(rendPos);
				  int dmgModFinal = 0;
					if(this->m_maxDmgMod > 0)
					{//bonus add dmg bon us to rewards
					   int range = this->m_maxDmgMod - this->m_minDmgMod;
					   if(range > 0)
					   {
						  dmgModFinal = this->m_minDmgMod + (rand()%range);
					   }
					   else
						  dmgModFinal = this->m_maxDmgMod;

					   invObj->m_itemData->m_damageModifier = dmgModFinal;
					}//end bonus add dmg bon us to rewards
                  playersInventory->AddInventoryItem(1,FALSE,invObj->m_itemData);
			   }
			}
	}//end randome reward

	if(m_expBonus != 0.0f)
	{
	 if(!skillDatabase)
		 skillDatabase = new CSkillObjectList();

       CSkillObject * skillPtr = skillDatabase->GetSkillByType(m_skillTypeBonus);
       
	   //see if we have used this skill before-if not add it to our database
       if(!skillPtr)
	   {//need to add this skill
          CSkillObject * dbSkillPtr = skillMainDatabase->GetSkillByType(m_skillTypeBonus);
	      if(!dbSkillPtr)
		      return FALSE;
              dbSkillPtr->CloneMinimum(&skillPtr);
              skillDatabase->AddTail(skillPtr);
	   }//end need to add this skill
	  
	   if(skillPtr)
	   {
          if(skillPtr->m_currentExperience < m_maxExpLimit)
		  {//you can still gain on this
             skillPtr->m_currentExperience += m_expBonus;
			 *expGained = m_expBonus;
		  }//end you can still gain on this
	   }
	}

	*optErrorCode = 3;

	return TRUE;
}


void CAICollectionQuestObj::Clone(CAICollectionQuestObj ** clone)
{
     CAICollectionQuestObj * copyLocal = new CAICollectionQuestObj();
	 copyLocal->m_index								= m_index;
     copyLocal->m_title								= m_title;
     copyLocal->m_questDesription					= m_questDesription;
     copyLocal->m_questCompletionMessage			= m_questCompletionMessage;
     copyLocal->m_type								= m_type;//0= collection quest
	 copyLocal->m_skillTypeBonus					= m_skillTypeBonus;
     copyLocal->m_expBonus							= m_expBonus;
 	 copyLocal->m_maxExpLimit						= m_maxExpLimit;
	 copyLocal->m_spawnAiCfg						= m_spawnAiCfg;
	 CopyMemory(&copyLocal->m_spawnPosition,&m_spawnPosition,sizeof(D3DVECTOR));
	 copyLocal->m_worldID							= m_worldID;
	 copyLocal->m_spawnedAiLifeline					= m_spawnedAiLifeline;
	 copyLocal->m_randomRewardType					= m_randomRewardType;
	 copyLocal->m_spawnToLocationOverride			= m_spawnToLocationOverride;
	 copyLocal->m_makeTargetRebirthPoint			= m_makeTargetRebirthPoint;
	 copyLocal->m_raceSpecificAccess				= m_raceSpecificAccess;
	 copyLocal->m_optionalCommand					= m_optionalCommand;
	 copyLocal->m_miscAction						= m_miscAction;
	 copyLocal->m_completedKeyEnabled				= m_completedKeyEnabled;
	 copyLocal->m_progessBasedByQuestJournal		= m_progessBasedByQuestJournal;
	 copyLocal->m_cashReward						= m_cashReward;
	 copyLocal->m_requiredSkillType					= m_requiredSkillType;
     copyLocal->m_requiredSkillLevel				= m_requiredSkillLevel;
	 copyLocal->m_mobToHuntID						= m_mobToHuntID;
     copyLocal->m_maxDropCount						= m_maxDropCount;
     copyLocal->m_dropGlid							= m_dropGlid;
     copyLocal->m_dropPercentage					= m_dropPercentage;
	 copyLocal->m_autocomplete						= m_autocomplete;
	 copyLocal->m_openDialogWindow					= m_openDialogWindow;
	 copyLocal->m_optionalIntruction				= m_optionalIntruction;
	 copyLocal->m_listProgressionOptions			= m_listProgressionOptions;
	 copyLocal->m_undeletable						= m_undeletable;
	 copyLocal->m_spawnOnFail						= m_spawnOnFail;
	 copyLocal->m_portalLimitMsg					= m_portalLimitMsg;
	 copyLocal->m_portalLimitID						= m_portalLimitID;
	 copyLocal->m_closeJournalEntryByName			= m_closeJournalEntryByName;
	 copyLocal->m_expansionIDReq					= m_expansionIDReq;
	 copyLocal->m_flagHostileOnSuccess				= m_flagHostileOnSuccess;
	 copyLocal->m_requireOwnershipOfKingdomIndex	= m_requireOwnershipOfKingdomIndex;
	 copyLocal->m_kingdomID							= m_kingdomID;
	 copyLocal->m_cashContribution					= m_cashContribution;
	 copyLocal->m_miscString						= m_miscString;
	 copyLocal->m_closeJournalOnSuccess             = m_closeJournalOnSuccess;
	 copyLocal->m_clientScript						= m_clientScript;
     copyLocal->m_requiredScript					= m_requiredScript;
	 copyLocal->m_rewardScript						= m_rewardScript;
	 copyLocal->m_deleteByLocationID				= m_deleteByLocationID;
	 copyLocal->m_clearInventory					= m_clearInventory;
	 copyLocal->m_deleteAllByRequiredGLID           = m_deleteAllByRequiredGLID;
	 copyLocal->m_deteteAllByRewardGLID				= m_deteteAllByRewardGLID;
	 copyLocal->m_despawnOnReward					= m_despawnOnReward;
	 copyLocal->m_flagTimer							= m_flagTimer;
	 copyLocal->m_requiredTimeStamp					= m_requiredTimeStamp;
	 copyLocal->m_maxDmgMod							= m_maxDmgMod;
	 copyLocal->m_minDmgMod							= m_minDmgMod;
	 copyLocal->m_spawnArrayCountOption				= m_spawnArrayCountOption;//>0 activates
	 copyLocal->m_spawnArrayOffset					= m_spawnArrayOffset;//radius offset
	 copyLocal->m_spawnRotationRange				= m_spawnRotationRange;
	 copyLocal->m_spawnToWorldName					= m_spawnToWorldName;
	 copyLocal->m_instanceGenerateType				= m_instanceGenerateType;//0=none 1=group based 2=arena based
	 copyLocal->m_instanceGenerateParam				= m_instanceGenerateParam;//m_instanceGenerateType: if arena then its time based
	 copyLocal->m_instanceGenerateParam2			= m_instanceGenerateParam2;

	 copyLocal->m_instanceGenerateParam3			= m_instanceGenerateParam3;//*sometimes if arena minimum before start timer begin
     copyLocal->m_instanceGenerateParam4			= m_instanceGenerateParam4;//0=disabled
	 copyLocal->m_arenaBetPool						= m_arenaBetPool;

	 if(m_rules)
	 {
	    copyLocal->m_ruleCount = m_ruleCount;
        copyLocal->m_rules = new DYNRULES[m_ruleCount];
        CopyMemory(copyLocal->m_rules,m_rules,sizeof(DYNRULES)*m_ruleCount);
	 }

     if(m_requiredItems)
		m_requiredItems->Clone(&copyLocal->m_requiredItems);
   
	 if(m_reward)
		m_reward->Clone(&copyLocal->m_reward);

	 *clone = copyLocal;

}

void CAICollectionQuestList::Clone(CAICollectionQuestList ** clone)
{
     CAICollectionQuestList * copyLocal = new CAICollectionQuestList();

     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
         CAICollectionQuestObj * spnPtr = (CAICollectionQuestObj *)GetNext(posLoc);
		 CAICollectionQuestObj * newPtr = NULL;
		 spnPtr->Clone(&newPtr);
         copyLocal->AddTail(newPtr);
	 }

	 *clone = copyLocal;
}

CAICollectionQuestObj * CAICollectionQuestList::GetByAvailIndex(int baseSelection,int raceID,int * retTraceVis)
{
   int trace = 0;
   
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
         CAICollectionQuestObj * spnPtr = (CAICollectionQuestObj *)GetNext(posLoc);
		 if(spnPtr->m_raceSpecificAccess > -1)
		    if(spnPtr->m_raceSpecificAccess != raceID)
			   continue;

		 if(trace == baseSelection)
		 {
		    *retTraceVis = trace;
		    return spnPtr;
		 }

		 trace++;
		
	 }
     //redundancy possible race not found-will cause failure msg
	 trace = 0;
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
         CAICollectionQuestObj * spnPtr = (CAICollectionQuestObj *)GetNext(posLoc);
		
		 if(trace == baseSelection)
		 {
		    *retTraceVis = trace;
		    return spnPtr;
		 }

		 trace++;
		
	 }


	 return NULL;
}

CAICollectionQuestObj * CAICollectionQuestList::GetByIndex(int selection)
{
   if(selection < 0)
      return NULL;

   POSITION posLoc = FindIndex(selection);
   if(!posLoc)
	   return NULL;

   CAICollectionQuestObj * spnPtr = (CAICollectionQuestObj *)GetAt(posLoc);

   return spnPtr;
}

void CAICollectionQuestList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CAICollectionQuestObj * spnPtr = (CAICollectionQuestObj *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}


void CAICollectionQuestObj::Serialize( CArchive& ar )
{
BOOL reserved = 0;
CString reservedString = "Reserved";

CObject::Serialize( ar );
	if(ar.IsStoring() ){	

       ar.SetObjectSchema(9);
       ASSERT(ar.GetObjectSchema() == 9);

	   ar << m_title
	      << m_questDesription
	      << m_questCompletionMessage
	      << m_type
	      << m_requiredItems
		  << m_reward
		  << m_skillTypeBonus
          << m_expBonus
	      << m_maxExpLimit
		  << m_spawnAiCfg
	      << m_spawnPosition.x
		  << m_spawnPosition.y
		  << m_spawnPosition.z
	      << m_worldID
	      << m_spawnedAiLifeline
		  << m_randomRewardType
		  << m_spawnToLocationOverride
		  << m_makeTargetRebirthPoint;
	   
	   ar << m_optionalCommand
		  << m_miscAction
		  << m_raceSpecificAccess
		  << m_completedKeyEnabled
		  << m_progessBasedByQuestJournal;

	   ar << m_cashReward
		  << m_autocomplete
		  << m_openDialogWindow
		  << m_requiredSkillType
		  << m_requiredSkillLevel;

	   ar << m_optionalIntruction
	      << m_portalLimitMsg
		  << m_closeJournalEntryByName
		  << m_miscString;

	   ar << m_mobToHuntID
          << m_maxDropCount
          << m_dropGlid
          << m_dropPercentage
		  << m_listProgressionOptions
		  << m_undeletable
		  << m_spawnOnFail
		  << m_portalLimitID;

	   ar << m_expansionIDReq
	      << m_flagHostileOnSuccess
		  << m_requireOwnershipOfKingdomIndex
		  << m_kingdomID;

	   ar << m_cashContribution
	      << m_closeJournalOnSuccess
		  << reserved
		  << reserved
		  << reserved
	      << reserved;

	   ar << m_clientScript
	      << reservedString
		  << reservedString
		  << reservedString
		  << reservedString
	      << reservedString;

	   ar << m_arenaBetPool;
	}
	else
	{
       int version = ar.GetObjectSchema();

       ar >> m_title
	      >> m_questDesription
	      >> m_questCompletionMessage
	      >> m_type
	      >> m_requiredItems
		  >> m_reward
		  >> m_skillTypeBonus
          >> m_expBonus
	      >> m_maxExpLimit
		  >> m_spawnAiCfg
	      >> m_spawnPosition.x
		  >> m_spawnPosition.y
		  >> m_spawnPosition.z
	      >> m_worldID
	      >> m_spawnedAiLifeline
		  >> m_randomRewardType
		  >> m_spawnToLocationOverride
		  >> m_makeTargetRebirthPoint;

	   m_raceSpecificAccess      = -1;
	   m_requiredSkillType       = 0;
	   m_requiredSkillLevel      = 0;
	   m_mobToHuntID             = 0;
       m_maxDropCount            = 0;
       m_dropGlid                = 0;
       m_dropPercentage          = 0;
	   m_portalLimitID           = 0;
	   m_openDialogWindow        = FALSE;
	   m_expansionIDReq          = 0;
	   m_flagHostileOnSuccess    = 0;
	   m_requireOwnershipOfKingdomIndex = 0;
	   m_kingdomID				 = 0;
	   m_cashContribution        = 0;
	   m_clientScript            = "";
	   m_closeJournalOnSuccess   = FALSE;
	   m_deleteByLocationID      = FALSE;
	   m_deleteAllByRequiredGLID = FALSE;
	   m_deteteAllByRewardGLID	 = FALSE;
	   m_maxDmgMod			     = 0;
	   m_minDmgMod				 = 0;
	   
	   if(version == 2)
	   {
	   ar >> m_optionalCommand
		  >> m_miscAction
		  >> m_raceSpecificAccess
		  >> m_completedKeyEnabled
		  >> m_progessBasedByQuestJournal;

	   m_cashReward = 0;
	   }
	   else if(version == 3)
	   {
         ar >> m_optionalCommand
		    >> m_miscAction
		    >> m_raceSpecificAccess
		    >> m_completedKeyEnabled
		    >> m_progessBasedByQuestJournal;

	     ar >> m_cashReward
		    >> m_autocomplete
		    >> m_openDialogWindow
		    >> m_requiredSkillType
		    >> m_requiredSkillLevel;
	   }
	   else if(version == 4)
	   {
         ar >> m_optionalCommand
		    >> m_miscAction
		    >> m_raceSpecificAccess
		    >> m_completedKeyEnabled
		    >> m_progessBasedByQuestJournal;

	     ar >> m_cashReward
		    >> m_autocomplete
		    >> m_openDialogWindow
		    >> m_requiredSkillType
		    >> m_requiredSkillLevel;

		 ar >> reservedString
	        >> reservedString
		    >> reservedString
		    >> reservedString;

	   }
	   else if(version == 5)
	   {
         ar >> m_optionalCommand
		    >> m_miscAction
		    >> m_raceSpecificAccess
		    >> m_completedKeyEnabled
		    >> m_progessBasedByQuestJournal;

	     ar >> m_cashReward
		    >> m_autocomplete
		    >> m_openDialogWindow
		    >> m_requiredSkillType
		    >> m_requiredSkillLevel;

		 ar >> m_optionalIntruction
	        >> m_portalLimitMsg
		    >> m_closeJournalEntryByName
		    >> reservedString;

		 ar >> m_mobToHuntID
            >> m_maxDropCount
            >> m_dropGlid
            >> m_dropPercentage
		    >> m_listProgressionOptions
		    >> m_undeletable
		    >> m_spawnOnFail
		    >> m_portalLimitID;

	   }
	   else if(version == 6)
	   {
         ar >> m_optionalCommand
		    >> m_miscAction
		    >> m_raceSpecificAccess
		    >> m_completedKeyEnabled
		    >> m_progessBasedByQuestJournal;

	     ar >> m_cashReward
		    >> m_autocomplete
		    >> m_openDialogWindow
		    >> m_requiredSkillType
		    >> m_requiredSkillLevel;

		 ar >> m_optionalIntruction
	        >> m_portalLimitMsg
		    >> m_closeJournalEntryByName
		    >> m_miscString;

		 ar >> m_mobToHuntID
            >> m_maxDropCount
            >> m_dropGlid
            >> m_dropPercentage
		    >> m_listProgressionOptions
		    >> m_undeletable
		    >> m_spawnOnFail
		    >> m_portalLimitID;

		 ar >> m_expansionIDReq
	        >> m_flagHostileOnSuccess
		    >> m_requireOwnershipOfKingdomIndex
		    >> reserved;

	   }
	   else if(version == 7)
	   {
         ar >> m_optionalCommand
		    >> m_miscAction
		    >> m_raceSpecificAccess
		    >> m_completedKeyEnabled
		    >> m_progessBasedByQuestJournal;

	     ar >> m_cashReward
		    >> m_autocomplete
		    >> m_openDialogWindow
		    >> m_requiredSkillType
		    >> m_requiredSkillLevel;

		 ar >> m_optionalIntruction
	        >> m_portalLimitMsg
		    >> m_closeJournalEntryByName
		    >> m_miscString;

		 ar >> m_mobToHuntID
            >> m_maxDropCount
            >> m_dropGlid
            >> m_dropPercentage
		    >> m_listProgressionOptions
		    >> m_undeletable
		    >> m_spawnOnFail
		    >> m_portalLimitID;

		 ar >> m_expansionIDReq
	        >> m_flagHostileOnSuccess
		    >> m_requireOwnershipOfKingdomIndex
		    >> m_kingdomID;

		 ar >> m_cashContribution
	        >> m_closeJournalOnSuccess
		    >> reserved
		    >> reserved
		    >> reserved
	        >> reserved;

	   }
	   else if(version == 8)
	   {
         ar >> m_optionalCommand
		    >> m_miscAction
		    >> m_raceSpecificAccess
		    >> m_completedKeyEnabled
		    >> m_progessBasedByQuestJournal;

	     ar >> m_cashReward
		    >> m_autocomplete
		    >> m_openDialogWindow
		    >> m_requiredSkillType
		    >> m_requiredSkillLevel;

		 ar >> m_optionalIntruction
	        >> m_portalLimitMsg
		    >> m_closeJournalEntryByName
		    >> m_miscString;

		 ar >> m_mobToHuntID
            >> m_maxDropCount
            >> m_dropGlid
            >> m_dropPercentage
		    >> m_listProgressionOptions
		    >> m_undeletable
		    >> m_spawnOnFail
		    >> m_portalLimitID;

		 ar >> m_expansionIDReq
	        >> m_flagHostileOnSuccess
		    >> m_requireOwnershipOfKingdomIndex
		    >> m_kingdomID;

		 ar >> m_cashContribution
	        >> m_closeJournalOnSuccess
		    >> reserved
		    >> reserved
		    >> reserved
	        >> reserved;

		 ar >> m_clientScript
	        >> reservedString
		    >> reservedString
		    >> reservedString
		    >> reservedString
	        >> reservedString;
	   }
	   else if(version == 8)
	   {
         ar >> m_optionalCommand
		    >> m_miscAction
		    >> m_raceSpecificAccess
		    >> m_completedKeyEnabled
		    >> m_progessBasedByQuestJournal;

	     ar >> m_cashReward
		    >> m_autocomplete
		    >> m_openDialogWindow
		    >> m_requiredSkillType
		    >> m_requiredSkillLevel;

		 ar >> m_optionalIntruction
	        >> m_portalLimitMsg
		    >> m_closeJournalEntryByName
		    >> m_miscString;

		 ar >> m_mobToHuntID
            >> m_maxDropCount
            >> m_dropGlid
            >> m_dropPercentage
		    >> m_listProgressionOptions
		    >> m_undeletable
		    >> m_spawnOnFail
		    >> m_portalLimitID;

		 ar >> m_expansionIDReq
	        >> m_flagHostileOnSuccess
		    >> m_requireOwnershipOfKingdomIndex
		    >> m_kingdomID;

		 ar >> m_cashContribution
	        >> m_closeJournalOnSuccess
		    >> reserved
		    >> reserved
		    >> reserved
	        >> reserved;

		 ar >> m_clientScript
	        >> reservedString
		    >> reservedString
		    >> reservedString
		    >> reservedString
	        >> reservedString;

		 ar >> m_arenaBetPool;
	   }

	   if(m_optionalIntruction == "Reserved")
		  m_optionalIntruction = "";

	   m_spawnStamp              = GetTickCount();


	   //m_raceSpecificAccess      = -1;
	   //m_spawnToLocationOverride = 0;
	}
}     


IMPLEMENT_SERIAL(CAICollectionQuestObj, CObject, VERSIONABLE_SCHEMA|9)
IMPLEMENT_SERIAL(CAICollectionQuestList, CObList, 0)

