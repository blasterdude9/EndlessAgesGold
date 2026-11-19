#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d9.h>
#include <afxtempl.h>
#include "CQuestJournal.h"
#include "ItemManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CQJRequiredItem::CQJRequiredItem()
{
    m_glid    = -1;
	m_quanity = 0;
	m_logType = 0;
}

void CQJRequiredItem::Clone(CQJRequiredItem ** clone)
{
	CQJRequiredItem * copyLocal = new CQJRequiredItem();

	copyLocal->m_glid				= m_glid;
	copyLocal->m_quanity			= m_quanity;
	copyLocal->m_logType			= m_logType;

	*clone = copyLocal;
}

void CQJRequiredItem::Serialize( CArchive& ar )
{
BOOL reserved = 0;
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_glid
          << m_quanity
		  << m_itemName;
	  
	}
	else
	{
       ar >> m_glid
          >> m_quanity
		  >> m_itemName;
	}
}   

void CQJRequiredItemList::Clone(CQJRequiredItemList ** clone)
{
     CQJRequiredItemList * copyLocal = new CQJRequiredItemList();

	 for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CQJRequiredItem * skPtr = (CQJRequiredItem *)GetNext(posLoc);
		 CQJRequiredItem * clonePtr = NULL;
         skPtr->Clone(&clonePtr);
         copyLocal->AddTail(clonePtr);
	 }

	 *clone = copyLocal;
}

void CQJRequiredItemList::SafeDelete()
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CQJRequiredItem * spnPtr = (CQJRequiredItem *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

CQJRequiredItem * CQJRequiredItemList::GetByID(int id)
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CQJRequiredItem * spnPtr = (CQJRequiredItem *)GetNext(posLoc);
		if(spnPtr->m_glid == id)
           return spnPtr;
	}
	return NULL;
}


CQuestJournalObj::CQuestJournalObj()
{
    m_requiredItems = NULL;
	m_currentProgression = 0;
	m_status = 0;
	m_mobToHuntID = 0;
	m_maxDropCount = 0;
	m_dropGlid = 0;
	m_dropPercentage = 0;
	m_lastSelectionIndex = 0;
	m_autoComplete = TRUE;//for hunting quests
	m_skillExpType = 0;
    m_expBonus = 0.0f;
    m_thisGainCap = 0.0f;
	m_undeletable = FALSE;
	this->m_questDescription = "";
	this->m_npcName = "";
	this->m_keyStatement = "";

}

CQuestJournalObj::~CQuestJournalObj()
{
	this->SafeDelete();
}

void CQuestJournalObj::SafeDelete()
{
	
    if(m_requiredItems)
	{
	   m_requiredItems->SafeDelete();
       delete m_requiredItems;
	   m_requiredItems = 0;
	}
}

void CQuestJournalObj::Clone(CQuestJournalObj ** clone)
{
	CQuestJournalObj * copyLocal = new CQuestJournalObj();

	copyLocal->m_questDescription   = m_questDescription;
	copyLocal->m_npcName            = m_npcName;
	copyLocal->m_keyStatement       = m_keyStatement;
	copyLocal->m_currentProgression = m_currentProgression;
	copyLocal->m_mobToHuntID        = m_mobToHuntID;
	copyLocal->m_maxDropCount       = m_maxDropCount;
	copyLocal->m_dropGlid           = m_dropGlid;
	copyLocal->m_dropPercentage     = m_dropPercentage;
	copyLocal->m_lastSelectionIndex = m_lastSelectionIndex;
	copyLocal->m_autoComplete       = m_autoComplete;//for hunting quests
	copyLocal->m_skillExpType       = m_skillExpType;
    copyLocal->m_expBonus           = m_expBonus;
    copyLocal->m_thisGainCap        = m_thisGainCap;
	copyLocal->m_undeletable        = m_undeletable;

    if(m_requiredItems)
	   m_requiredItems->Clone(&copyLocal->m_requiredItems);

	*clone = copyLocal;
}


void CQuestJournalObj::Serialize( CArchive& ar )
{
BOOL reserved = 0;
CString reservedString = "";
CObject::Serialize( ar );
	if(ar.IsStoring() ){	

       ar.SetObjectSchema(3);
       ASSERT(ar.GetObjectSchema() == 3);


	   ar << m_requiredItems
          << m_questDescription
	      << m_npcName
	      << m_keyStatement
		  << m_currentProgression
		  << m_status;

	   ar << m_mobToHuntID
	      << m_maxDropCount
	      << m_dropGlid
	      << m_dropPercentage
		  << m_lastSelectionIndex
		  << m_undeletable
		  << reserved
		  << reserved;

	   ar << m_autoComplete
	      << m_skillExpType
          << m_expBonus
          << m_thisGainCap
		  << reserved
		  << reserved
		  << reserved
		  << reserved;
	  
	}
	else
	{
       int version = ar.GetObjectSchema();

       ar >> m_requiredItems
          >> m_questDescription
	      >> m_npcName
	      >> m_keyStatement
		  >> m_currentProgression
		  >> m_status;

	   m_autoComplete = TRUE;//for hunting quests
	   m_skillExpType = 0;
       m_expBonus = 0.0f;
       m_thisGainCap = 0.0f;

	   if(version == 2)
	   {
	   ar >> m_mobToHuntID
	      >> m_maxDropCount
	      >> m_dropGlid
	      >> m_dropPercentage
		  >> m_lastSelectionIndex
		  >> reserved
		  >> reserved
		  >> reserved;
	   }
	   else if(version == 3)
	   {
       ar >> m_mobToHuntID
	      >> m_maxDropCount
	      >> m_dropGlid
	      >> m_dropPercentage
		  >> m_lastSelectionIndex
		  >> m_undeletable
		  >> reserved
		  >> reserved;

	   ar >> m_autoComplete
	      >> m_skillExpType
          >> m_expBonus
          >> m_thisGainCap
		  >> reserved
		  >> reserved
		  >> reserved
		  >> reserved;

	   }
	}
}     

BOOL CQuestJournalList::LogQuest(CString description, 
	                             CInventoryObjList * requiredItems,
				                 CString m_npcName,
				                 CString m_keyStatement,
								 void * itemManagerMem,
								 BOOL succeeded,
								 int curVisibleSelection,
								 int logOption,
								 int mobToHuntID,
	                             int maxDropCount,
	                             int dropGlid,
	                             int dropPercentage,
								 BOOL autoComplete,
	                             int skillExpType,
                                 float expBonus,
                                 float thisGainCap,
								 BOOL isProgressive,
								 BOOL undeletable,
								 CString closeJournalEntryByName,
								 BOOL closeJournalOnSuccess,
								 int & maxProgressions
								 )
{
	//existing quest...
	CQuestJournalObj * existingPtr = GetBySeer(m_npcName);
	
	if(existingPtr)
	{//existing quest
	   if(succeeded)
	   {//progress to next quest
	      if(curVisibleSelection == existingPtr->m_currentProgression && isProgressive == TRUE)
		  {//success will eman somthing because this is a undone quest
          existingPtr->m_currentProgression = existingPtr->m_currentProgression+1;		
		  /*if(existingPtr->m_currentProgression >= maxProgressions)
		     existingPtr->m_currentProgression = maxProgressions;*/
		  existingPtr->m_status = 1;
		  }//end success will eman somthing because this is a undone quest
		  else
		  existingPtr->m_status = 1;
	   }//end progress to next quest
	   else{
	      // A failed check of a part the player has ALREADY finished (e.g. the quest window sent the
	      // old option again right after the turn-in) must not put that part back in the journal -
	      // the player stays on the part they are really on (the "quest restarted" bug).
	      if(isProgressive == TRUE && curVisibleSelection < existingPtr->m_currentProgression)
	         return TRUE;
	      //if(existingPtr->m_currentProgression <= curVisibleSelection)
		  //{//dont overwrite
            existingPtr->m_keyStatement       = m_keyStatement;
	        existingPtr->m_npcName            = m_npcName;
	        existingPtr->m_questDescription   = description;
			existingPtr->m_mobToHuntID        = mobToHuntID;
			existingPtr->m_maxDropCount       = maxDropCount;
			existingPtr->m_dropGlid           = dropGlid;
	        existingPtr->m_dropPercentage     = dropPercentage;
			
			existingPtr->m_autoComplete       = autoComplete;
	        existingPtr->m_skillExpType       = skillExpType;
            existingPtr->m_expBonus           = expBonus;
            existingPtr->m_thisGainCap        = thisGainCap;
			existingPtr->m_undeletable        = undeletable;

			if(curVisibleSelection > existingPtr->m_lastSelectionIndex)
			   existingPtr->m_lastSelectionIndex = curVisibleSelection;

		    if(existingPtr->m_requiredItems)
		    {
		     existingPtr->m_requiredItems->SafeDelete();
             BuildRequiredList(existingPtr,requiredItems,itemManagerMem,logOption);
		    }
            existingPtr->m_status = 0;//revert status to new objective
			MakeMostRecentBySeer(m_npcName);
		  //}//end done over write
	   }
	   /*if(succeeded == closeJournalOnSuccess)
	   if(closeJournalEntryByName.GetLength() > 0 && closeJournalEntryByName != "Reserved")
	      DeleteEntryByNpcName(closeJournalEntryByName);*/
       return TRUE;
	}//end existing quest
	//new quest....
	if(GetCount() > MAXJOURNALITEMS)
	{
	   POSITION posLast = NULL;
       for(POSITION posLoc = GetTailPosition();(posLast = posLoc) != NULL;)
	   {
          CQuestJournalObj * spnPtr = (CQuestJournalObj *)GetPrev(posLoc);
		  if(spnPtr->m_undeletable)
		     continue;
		  /*
          spnPtr->SafeDelete();
		  delete spnPtr;
		  spnPtr = 0;
		  */
		  ::SafeDelete(spnPtr);
		  RemoveAt(posLast);
		  break;
	   }
	}
	
	CQuestJournalObj * newPtr    = new CQuestJournalObj();
	newPtr->m_requiredItems      = new CQJRequiredItemList();
	newPtr->m_keyStatement       = m_keyStatement;
	newPtr->m_npcName            = m_npcName;
	newPtr->m_questDescription   = description;
	newPtr->m_mobToHuntID        = mobToHuntID;
	newPtr->m_maxDropCount       = maxDropCount;
	newPtr->m_dropGlid           = dropGlid;
	newPtr->m_dropPercentage     = dropPercentage;
	newPtr->m_lastSelectionIndex = curVisibleSelection;
	newPtr->m_autoComplete       = autoComplete;
	newPtr->m_skillExpType       = skillExpType;
    newPtr->m_expBonus           = expBonus;
    newPtr->m_thisGainCap        = thisGainCap;
    BuildRequiredList(newPtr,requiredItems,itemManagerMem,logOption);

	if(succeeded)
	{
       newPtr->m_status = 1;
       newPtr->m_currentProgression = newPtr->m_currentProgression+1;
	   /*if(newPtr->m_currentProgression >= maxProgressions)
	      newPtr->m_currentProgression = maxProgressions;*/
	}
    AddHead(newPtr);

    //if(!succeeded)
	   /*if(closeJournalEntryByName.GetLength() > 0 && closeJournalEntryByName != "Reserved")
	      DeleteEntryByNpcName(closeJournalEntryByName);//could be the same at the current be carful*/


	return TRUE;
}

BOOL CQuestJournalList::LogQuest(CString npcName,
								 CString description,
								 CString keyStatement)
{
	DeleteEntryByNpcName(npcName);
	
	CQuestJournalObj * newPtr    = new CQuestJournalObj();
    
    newPtr->m_npcName				= npcName;
	newPtr->m_questDescription		= description;
	newPtr->m_keyStatement			= keyStatement;

    AddHead(newPtr);
	return TRUE;
}


BOOL CQuestJournalList::BuildRequiredList(CQuestJournalObj * newPtr,
										  CInventoryObjList * requiredItems,
										  void * itemManagerMem,
										  int logOption)
{
//build required list

    CItemManager * iManager = (CItemManager *)itemManagerMem;

	if(requiredItems)
	for(POSITION posLoc = requiredItems->GetHeadPosition();posLoc != NULL;)
	{
        CInventoryObj * invPtr = (CInventoryObj *)requiredItems->GetNext(posLoc);
		if(!invPtr->m_itemData)
			continue;

		if(invPtr->m_itemData->m_permanent)
			continue;
           
		CQJRequiredItem * existingItem = newPtr->m_requiredItems->GetByID(invPtr->m_itemData->m_globalID);
		if(existingItem)
		{
		   if(invPtr->m_quanity == 0)
           existingItem->m_quanity += 1;
		   else
		   existingItem->m_quanity += invPtr->m_quanity;
		}
		else{
           
           CItemObj * globalPtr = iManager->GetItemByID_LOS(invPtr->m_itemData->m_globalID);
           if(globalPtr)
		   {
              CQJRequiredItem * newItem = new CQJRequiredItem();
		      newItem->m_glid = invPtr->m_itemData->m_globalID;
		      newItem->m_quanity = invPtr->m_quanity;
			  newItem->m_logType = logOption;


		      if(newItem->m_quanity == 0)
		         newItem->m_quanity = 1;
			  if(logOption == 4)
			  {
                  newItem->m_itemName  = "?????";
			  }
			  else
		          newItem->m_itemName  = globalPtr->m_itemName;
			  
			  iManager->Unlock();
              newPtr->m_requiredItems->AddTail(newItem);
		   }
		}
	}

	if(newPtr->m_requiredItems)
	for(POSITION posLoc = newPtr->m_requiredItems->GetHeadPosition();posLoc != NULL;)
	{
        CQJRequiredItem * existingItem = (CQJRequiredItem *)newPtr->m_requiredItems->GetNext(posLoc);
		
        existingItem->m_itemName = "     (" + m_cabLib.ConvToString(existingItem->m_quanity) + ")" + existingItem->m_itemName;
		
	}
	return TRUE;
}




CQuestJournalObj * CQuestJournalList::GetBySeer(CString seerName)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CQuestJournalObj * spnPtr = (CQuestJournalObj *)GetNext(posLoc);
		if(seerName == spnPtr->m_npcName)
		{  
		   return spnPtr;
		}
	}
    return NULL;
}

CQuestJournalObj * CQuestJournalList::GetMostRecentUncompleted()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CQuestJournalObj * spnPtr = (CQuestJournalObj *)GetNext(posLoc);
		if(spnPtr->m_status == 0)
		{  
		   return spnPtr;
		}
	}
    return NULL;
}


BOOL CQuestJournalList::DeleteEntryByNpcName(CString name)
{
	POSITION posLast = NULL;
	for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CQuestJournalObj * spnPtr = (CQuestJournalObj *)GetNext(posLoc);
		if(spnPtr->m_npcName.CompareNoCase(name)==0)
		{  
			/*
           spnPtr->SafeDelete();
		   delete spnPtr;
		   spnPtr = 0;
		   */
			::SafeDelete(spnPtr);
           RemoveAt(posLast);
		   return TRUE;
		}
	}
    return FALSE;
}

void CQuestJournalList::MakeMostRecentBySeer(CString seerName)
{
    POSITION posLast;
	for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CQuestJournalObj * spnPtr = (CQuestJournalObj *)GetNext(posLoc);
		if(seerName.CompareNoCase(spnPtr->m_npcName)==0)
		{  
		    RemoveAt(posLast);
			AddHead(spnPtr);
			return;
		}
	}
}

void CQuestJournalList::RemoveBySeer(CString seerName)
{
	POSITION posLast = NULL;
	for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CQuestJournalObj * spnPtr = (CQuestJournalObj *)GetNext(posLoc);
		if(seerName.CompareNoCase(spnPtr->m_npcName)==0)
		{  
		   /*
           spnPtr->SafeDelete();
		   delete spnPtr;
		   spnPtr = 0;
		   */
			::SafeDelete(spnPtr);
		   RemoveAt(posLast);
		   return;
		}
	}
}

void CQuestJournalList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CQuestJournalObj * spnPtr = (CQuestJournalObj *)GetNext(posLoc);
		/*
           spnPtr->SafeDelete();
		   delete spnPtr;
		   spnPtr = 0;
		   */
			::SafeDelete(spnPtr);
	}
	RemoveAll();
}



void CQuestJournalList::Clone(CQuestJournalList ** clone)
{
     CQuestJournalList * copyLocal = new CQuestJournalList();

	 for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CQuestJournalObj * skPtr = (CQuestJournalObj *)GetNext(posLoc);
		 CQuestJournalObj * clonePtr = NULL;
         skPtr->Clone(&clonePtr);
         copyLocal->AddTail(clonePtr);
	 }

	 *clone = copyLocal;
}

CQuestJournalObj * CQuestJournalList::GetUncompletedQuestJournalEntryByTarget(int aiCfgId)
{
	 for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CQuestJournalObj * skPtr = (CQuestJournalObj *)GetNext(posLoc);
		 if(skPtr->m_mobToHuntID > 0)
		 if(skPtr->m_status == 0)
         if(aiCfgId == skPtr->m_mobToHuntID)
		 {
            return skPtr;
		 }
	 }
     return NULL;
}

CQuestJournalObj * CQuestJournalList::GetByIndex(int index)
{
	 int trace = 0;
	 for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CQuestJournalObj * skPtr = (CQuestJournalObj *)GetNext(posLoc);
		 if(trace == index)
		 {
            return skPtr;
		 }
		 trace++;
	 }
     return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CQuestJournalList
// Function:	AddQuestJournalEntry(CQuestJournalObj*, QJ_INSERT_LOCATION)
// Description:	Add a journal entry to this quest journal list with duplicate
//	checking and prevention. The boolean parameter indicates if the new entry
//	should be added at the head or tail of the list.
///////////////////////////////////////////////////////////////////////////////

BOOL 
CQuestJournalList::AddQuestJournalEntry(CQuestJournalObj *pJournalEntry, QJ_INSERT_LOCATION eLocation)
{
	// CHECK for duplicates
	bool bIsDuplicate = false;
	for (POSITION p1 = this->GetHeadPosition(); p1 != NULL;)
	{
		CQuestJournalObj *pCompareEntry = dynamic_cast<CQuestJournalObj*>(this->GetNext(p1));
		if (!pCompareEntry) continue;

		// IF these two journal entries have the same:
		//	- NPC Name
		//	- Quest Description
		//	- Key Statement
		//	- Required Items
		// THEN this is a duplicate entry and we don't add it!
		if ((pJournalEntry->m_npcName.Compare(pCompareEntry->m_npcName) == 0)
		&& (pJournalEntry->m_questDescription.Compare(pCompareEntry->m_questDescription) == 0)
		&& (pJournalEntry->m_keyStatement.Compare(pCompareEntry->m_keyStatement) == 0))
		{
			// This is looking like a dupe so far. 
			//	Now we check required items for final confirmation.
			if (pJournalEntry->m_requiredItems 
			&& pCompareEntry->m_requiredItems
			&& (pJournalEntry->m_requiredItems->GetCount() == pCompareEntry->m_requiredItems->GetCount()))
			{
				// CHECK that all the required items are the same
				bIsDuplicate = true;
				POSITION	p2 = pJournalEntry->m_requiredItems->GetHeadPosition(), 
							p3 = pCompareEntry->m_requiredItems->GetHeadPosition();
				while ((p2 != NULL) && (p3 != NULL))
				{
					CQJRequiredItem *pRequiredItem = dynamic_cast<CQJRequiredItem*>(pJournalEntry->m_requiredItems->GetNext(p2));
					CQJRequiredItem *pCompareItem = dynamic_cast<CQJRequiredItem*>(pCompareEntry->m_requiredItems->GetNext(p3));
					if (!pRequiredItem || !pCompareItem) break;
					else if ((pRequiredItem->m_glid != pCompareItem->m_glid)
					|| (pRequiredItem->m_quanity != pCompareItem->m_quanity))
					{
						bIsDuplicate = false;
						continue;
					}
				}
			}
			else if (!pJournalEntry->m_requiredItems && !pCompareEntry->m_requiredItems)
				bIsDuplicate = true;
			
			// We have a duplicate entry, so don't add it to the list!
			if (bIsDuplicate) 
				return FALSE;
		}
	}

	// IF we're here, the passed in entry is not a duplicate so we ADD it!
	switch (eLocation)
	{
		default:
		case QJ_HEAD:
		{
			this->AddHead(pJournalEntry);
			break;
		}
		case QJ_TAIL:
		{
			this->AddTail(pJournalEntry);
			break;
		}
	}
	return TRUE;
}


IMPLEMENT_SERIAL(CQuestJournalObj, CObject, VERSIONABLE_SCHEMA|3)
IMPLEMENT_SERIAL(CQuestJournalList, CObList, 0)
IMPLEMENT_SERIAL(CQJRequiredItem, CObject, 0)
IMPLEMENT_SERIAL(CQJRequiredItemList, CObList, 0)
