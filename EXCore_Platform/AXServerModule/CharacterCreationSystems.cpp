#include "StdAfx.h"
#include "charactercreationsystems.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCharacterCreationSystems::CCharacterCreationSystems(CSharedNet * sharedInterface)
{
	m_sharedInterfaceRef = sharedInterface;
}

CCharacterCreationSystems::~CCharacterCreationSystems(void)
{
}

//---used by client-------------------------------------------------------------------//
BOOL CCharacterCreationSystems::HandleCharacterCreationOnAccount(LPVOID lpMessage,int & msgSize,DPID & idFromLoc, int handleOverride)
{
         //security checks     
	     if(msgSize < sizeof(CREATECHARHEADER_DATA))
		 {//verify header allocation
            m_sharedInterfaceRef->m_eventLog->AddEvent("Packet size invalid-disconnect char creation header Struct",idFromLoc); 
		    m_sharedInterfaceRef->m_netComObject->Disconnect(idFromLoc);//disconnect for incomplience
		    return FALSE;
		 }//end verify header allocation
	
	     LPCREATECHARHEADER_DATA header = (LPCREATECHARHEADER_DATA)lpMessage;
		 
         int packetPojectedSize = sizeof(CREATECHARHEADER_DATA) + (sizeof(ITEMDATA_STRUCT)*(header->m_defCfgCount+header->m_attachmentCount)) + header->m_nameLength;
		 
		 if(msgSize != packetPojectedSize)
		 {
            m_sharedInterfaceRef->m_eventLog->AddEvent("Packet size invalid-disconnect char creation body Struct",idFromLoc); 
		    m_sharedInterfaceRef->m_netComObject->Disconnect(idFromLoc);//disconnect for incomplience
		    return FALSE;
		 }

		 int headerSize = sizeof(CREATECHARHEADER_DATA);
		 BYTE * dataBlock = (BYTE*)lpMessage;

		 char name[50];
		 ZeroMemory(name,50);
		 if(header->m_nameLength < 0 || header->m_nameLength >= 50)
		 {//name too long for the buffer - would overwrite the stack
            m_sharedInterfaceRef->m_eventLog->AddEvent("Character name length invalid-disconnect",idFromLoc);
		    m_sharedInterfaceRef->m_netComObject->Disconnect(idFromLoc);
		    return FALSE;
		 }
		 bool allSpaces = true;

         for(int i=0;i<header->m_nameLength;i++)
		 {
             name[i] = dataBlock[(i+headerSize)];

			 // Make sure the player's name has more than just spaces
             if(dataBlock[(i+headerSize)] != ' ')
			    allSpaces = false;
		 }

		 CString cnvName = name;

		 if(allSpaces == true)
            cnvName = "Unnamed";

		 if((header->m_defCfgCount+header->m_attachmentCount) <= 0)
		     return FALSE;

		 ITEMDATA_STRUCT * itemDataStruct = new ITEMDATA_STRUCT[header->m_defCfgCount+header->m_attachmentCount];
         BYTE * routeing = (BYTE*)itemDataStruct;
     
         int itemStruct = sizeof(ITEMDATA_STRUCT);

		 for(int p=0;p<(itemStruct*(header->m_defCfgCount+header->m_attachmentCount));p++)
		 routeing[p] = dataBlock[(p+headerSize+header->m_nameLength)];


 
           CAccountObject * accountPtr = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountByID_LOS(idFromLoc);
           if(!accountPtr){//error locating account
               m_sharedInterfaceRef->m_eventLog->AddEvent("Error locating account 21");
               delete [] itemDataStruct;
               return FALSE;
		   }//end error locating account
           //make sure they dont create too many characters
         
           if(m_sharedInterfaceRef->m_netComObject->m_accountDBMode == 1)
           if(accountPtr->m_charsCountDMMode >= (m_sharedInterfaceRef->m_netComObject->m_clientCharacterMaxCount-1))
		   {
              m_sharedInterfaceRef->m_eventLog->AddEvent("Player has no room to create new characters.");
			  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			  delete [] itemDataStruct;
		      return FALSE;
		   }

           //end make sure they dont create too many characters
           CPlayerObject * newPobj			= new CPlayerObject();
		   newPobj->m_currentID				= accountPtr->m_currentID;
		   newPobj->m_currentInventory		= new CInventoryObjList(); 
		   accountPtr->m_characterOnAccount = newPobj;
		   accountPtr->m_charsCountDMMode++;
		   //newPobj->m_bankInventory      = new CInventoryObjList();
		   newPobj->m_skills             = new CSkillObjectList();
		   if(handleOverride > 0)
		      newPobj->m_handle             = handleOverride;

		   if(!newPobj->m_questJournal)
               newPobj->m_questJournal = new CQuestJournalList();

		   newPobj->m_noteriety = new CNoterietyObj();

		   

           //get database and give the start inventory to player if first time creating character
             CMovementObj * DBPtr = m_sharedInterfaceRef->m_entObjList->GetObjByIndex(header->m_entityType);
		     if(!DBPtr){//invalid spawn index
                 m_sharedInterfaceRef->m_eventLog->AddEvent("Player attempted to create from and unknown databse entity");
				 m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
				 delete [] itemDataStruct;
                 return FALSE;
			 }//end invalid spawn index
		     else{//valid attach start inventory

                CString errorCatch;
                if(m_sharedInterfaceRef->m_serverScripts->ScriptProcessor(newPobj,sizeof(CPlayerObject),0,errorCatch,accountPtr->m_currentlyPlayingGameName,header->m_entityType,m_sharedInterfaceRef)==false)
		        {
                   m_sharedInterfaceRef->m_eventLog->AddEvent(errorCatch);
		        }

                if(DBPtr->m_stats)
				   DBPtr->m_stats->Clone(&newPobj->m_stats);
				else
				   newPobj->m_stats = new CStatObjList();


				newPobj->m_deformableMeshCount = DBPtr->List_skeletalObject->m_deformableMeshArrayCount;
                if(newPobj->m_deformableMeshCount > 0)
				   newPobj->m_deformableCfg       = new int[newPobj->m_deformableMeshCount];

				for(int loop=0;loop<DBPtr->List_skeletalObject->m_deformableMeshArrayCount;loop++)
				{//deformable mesh cfg available
                   newPobj->m_deformableCfg[loop] = DBPtr->List_skeletalObject->m_spawnCfg[loop];
				}//end deformable mesh cfg available
				 
				
				
				if(DBPtr->m_autoCfg)
                for(POSITION posLoc = DBPtr->m_autoCfg->GetHeadPosition();posLoc != NULL;)
				{//inital cfg list
                   CCfgObj * cfgPtr = (CCfgObj *)DBPtr->m_autoCfg->GetNext(posLoc);
                   if(cfgPtr->m_quanity > 0)
				   {//set inventory	
					    
                     CItemObj * itemRef = DBPtr->GetItemByID(cfgPtr->m_globalCFG);
                     if(!itemRef)
					     continue;

				     for(int iLoop=0;iLoop<cfgPtr->m_quanity;iLoop++)
                         newPobj->m_currentInventory->AddInventoryItem(1,
								                                       cfgPtr->m_armed,
								                                       itemRef);
					                                                   
				   }//end set inventory
				}//end inital cfg list
			 }//end valid attach start inventory





	   newPobj->m_dbCfg					= header->m_entityType;
	   newPobj->m_curEDBIndex			= header->m_entityType;
	   newPobj->m_originalEDBCfg		= header->m_entityType;
	   if(m_sharedInterfaceRef->m_netComObject->m_spawnPointCfgs == NULL || m_sharedInterfaceRef->m_netComObject->m_currentWorldsUsed == NULL)
	   {
           m_sharedInterfaceRef->m_eventLog->AddEvent("Error 546..Server has no valid preset spawn/world Cfgs");
		   m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
		   return FALSE;
	   }

       if(header->m_spawnCfg < 0)
	   {
           m_sharedInterfaceRef->m_eventLog->AddEvent("Bounds FindIndex CharacterCreationOnAccount()");
		   m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
		   return FALSE;
	   }


       /*POSITION spwnPos = m_sharedInterfaceRef->m_netComObject->m_spawnPointCfgs->FindIndex(header->m_spawnCfg);
       if(!spwnPos)
	   {
           m_sharedInterfaceRef->m_eventLog->AddEvent("Error 547..Invalid spawn cfg",">",header->m_spawnCfg);
		   m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
		   return FALSE;
	   }
	   CSpawnCfgObj * spwnPtr = (CSpawnCfgObj *) m_sharedInterfaceRef->m_netComObject->m_spawnPointCfgs->GetAt(spwnPos);*/
	   
	   newPobj->m_maxEnergy         = DBPtr->List_maxEnergy;
       if(newPobj->m_stats)
	      newPobj->m_maxEnergy         = newPobj->m_stats->GetBenefitBonus(1,newPobj->m_currentInventory) + DBPtr->List_maxEnergy;
	  
	   
	   newPobj->m_energy            = newPobj->m_maxEnergy;
	   newPobj->m_bornSpawnCfgIndex = header->m_spawnCfg;
	   newPobj->m_charName          = cnvName;
        
	   //assign cfg
	   int atLoop=0;
	   for(atLoop=0;atLoop<header->m_attachmentCount;atLoop++)
	   {
           if(newPobj->m_currentInventory)
              for(POSITION posLoc = newPobj->m_currentInventory->GetHeadPosition();posLoc != NULL;)
			  {
                  CInventoryObj * invObj = (CInventoryObj *)newPobj->m_currentInventory->GetNext(posLoc);
                  if(invObj->m_itemData->m_locationID == itemDataStruct[atLoop].m_quantity)
				  {//found object by location id
					 if(invObj->m_armed)
					 {
                       invObj->m_itemData->m_globalID = itemDataStruct[atLoop].m_GLID;//replace with new item
                       break;
					 }
				  }//end found object by location id
			  }
	   }
	   //assign def cfg
	   for(atLoop=header->m_attachmentCount;atLoop<(header->m_attachmentCount+header->m_defCfgCount);atLoop++)
	   {
           if(newPobj->m_deformableMeshCount > itemDataStruct[atLoop].m_GLID && itemDataStruct[atLoop].m_GLID >= 0)
              newPobj->m_deformableCfg[itemDataStruct[atLoop].m_GLID] = itemDataStruct[atLoop].m_quantity;
		    else
			  m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid deformable index");
	   }
	   
	   

	   
	   accountPtr->m_currentcharInUse = 0;
       m_sharedInterfaceRef->m_netComObject->SendSpawnMsg( DPNSEND_GUARANTEED,
                                                           accountPtr->m_currentID,//idFromLoc,
								                           (short)newPobj->m_curEDBIndex,
								                           newPobj->m_currentWorldMap,
										                   newPobj->m_altCfgIndex,
												           (short)newPobj->m_3dPosition.x,
												           (short)newPobj->m_3dPosition.y,
												           (short)newPobj->m_3dPosition.z);





	   if(itemDataStruct)
	   {
          delete [] itemDataStruct;
		  itemDataStruct = 0;
	   }
	   if(dataBlock)
	   {
          delete [] dataBlock;
		  dataBlock = 0;
	   }
       //successful
       m_sharedInterfaceRef->m_eventLog->AddEvent(accountPtr->m_accountName," Generated a new character:DB->",newPobj->m_curEDBIndex);

	   m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
    return TRUE;
}



