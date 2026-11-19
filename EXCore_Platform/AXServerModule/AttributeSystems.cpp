#include "StdAfx.h"
#include "attributesystems.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAttributeSystems::CAttributeSystems(CSharedNet * sharedInterface,CArenaSystems * arenaSys,CQuestSystems * questSys)
{
	m_sharedInterfaceRef		= sharedInterface;
	m_arenaInterfaceRef			= arenaSys;
	m_questSystemsRef			= questSys;
}

CAttributeSystems::~CAttributeSystems(void)
{
}

//--------------Handles system messages-----------------------------------------------//
BOOL CAttributeSystems::HandleAttribMessageServerSide_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
	if(m_sharedInterfaceRef->m_eventLog->m_logPriority >= 3)//non-allocation occulder
	   m_sharedInterfaceRef->m_eventLog->LogByPriority("HandleAttribMessageServerSide_safe() Attrib:",(DWORD)formatedMessage->attribute,3);

    switch(formatedMessage->attribute)
	{
	  case 1:	AtribRqstAttachmentStatesPlayerServerSide_safe(formatedMessage,idFromLoc);	break;
	  case 2:	AtribRemovePlayerByNetworkID_safe(formatedMessage,idFromLoc);				break;
      case 3:	RequestAiPendingInfo_safe(formatedMessage,idFromLoc);						break;
	  case 4:	AtribRequestItemInfo_safe(formatedMessage,idFromLoc);						break;
	  case 9:	AtribClientRequestPlayer_safe(formatedMessage,idFromLoc);					break;
	  case 10:	AtribDeleteCharacterFromAccount_safe(formatedMessage,idFromLoc);			break;
      case 14:	AtribRequestCharList_safe(formatedMessage,idFromLoc);						break;
	  case 15:	AtribSetInvenItemSwap_safe(formatedMessage,idFromLoc);						break;
	  case 18:	AtribSetDeformableItem_safe(formatedMessage,idFromLoc);						break;
      case 19:	AtribRequestCharInventory_safe(formatedMessage,idFromLoc);					break;
      case 21:	AtribRequestBuyItemFromStore_safe(formatedMessage,idFromLoc);			    break;
	  case 22:	AtribRequestSellItemToStore_safe(formatedMessage,idFromLoc);				break;
	  case 23:	AtribRequestToLootItem_safe(formatedMessage,idFromLoc);						break;
	  case 24:	AtribRequestToBankItem_safe(formatedMessage,idFromLoc);						break;
	  case 25:	AtribTakeItemFromBank_safe(formatedMessage,idFromLoc);						break;
	  case 26:	AtribRequestPortal_safe(formatedMessage,idFromLoc);							break;
      case 27:	AtribUseItem_safe(formatedMessage,idFromLoc);								break;
	  case 29:	AtribCancelTransaction_safe(formatedMessage,idFromLoc);						break;
	  case 30:	AtribAcceptTradeTerms_safe(formatedMessage,idFromLoc);						break;
	  case 31:	AtribDestroyItemByGlid_safe(formatedMessage,idFromLoc);						break;
	  case 32:	AtribQuestHandling_safe(formatedMessage,idFromLoc);							break;
	  case 33:	AtribSkillAbilityUse_safe(formatedMessage,idFromLoc);						break;
	  case 37:	AtribExpandItemDataRequest_safe(formatedMessage,idFromLoc);					break;
	  case 38:	AtribRequestHouseCfg_safe(formatedMessage,idFromLoc);						break;
	  case 39:	AtribBuyItemFromHouseStore_safe(formatedMessage,idFromLoc);					break;
	  case 40:	AtribAddItemToHouseStore_safe(formatedMessage,idFromLoc);					break;
	  case 41:	AtribRemoveItemFromHouseStore_safe(formatedMessage,idFromLoc);				break;
	  case 42:	AtribRemoveCashFromHouseStore_safe(formatedMessage,idFromLoc);				break;
	  case 43:	AtribRequestToOrganize2BankItems_safe(formatedMessage,idFromLoc);			break;
	  case 44:	AtribRequestToOrganize2InventoryItems_safe(formatedMessage,idFromLoc);		break;
	  case 45:	HandleQuestJournalRequest_safe(formatedMessage,idFromLoc);					break;
	  case 46:	ReportMurder_safe(formatedMessage->idRefrence,formatedMessage->uniInt,formatedMessage->uniInt2);break;
	  case 47:	AtribAIServerRequestHousingState_safe(formatedMessage,idFromLoc);			break;
	  case 48:	AtribRequestToLootAllItems_safe(formatedMessage,idFromLoc);					break;
	  case 49:	AtribRequestCharArmedInventory_safe(formatedMessage,idFromLoc);				break;
	  case 50:	AtribRequestCharUnarmedInventory_safe(formatedMessage,idFromLoc);			break;
	  case 62:	AtribSetSkillTitle_safe(formatedMessage,idFromLoc);							break;
	  case 63:  AtribRequestBadgeInfo(formatedMessage,idFromLoc);							break;
	  case 64:  RequestDynamicItem(formatedMessage,idFromLoc);								break;

	  default:
           m_sharedInterfaceRef->m_eventLog->LogByPriority("HandleAttribMessageServerSide_safe() unknown Attrib:",(DWORD)formatedMessage->attribute,2);
           return FALSE;
	  break;
	 
	}
	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::RequestDynamicItem(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
	CPlayerObject * currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByDPNID_Alive_LOS(idFromLoc);
	if(!currentPlayer)
		return FALSE;


    byte * buffer = NULL;
	int size = 0;

	
	if(m_sharedInterfaceRef->m_itemManager->GenerateDynItemPacket(&buffer,size,formatedMessage->uniInt,formatedMessage->idRefrence))
	{
		if(currentPlayer->m_miscOutBox->GetCount() < 20)
           currentPlayer->m_miscOutBox->AddPacket(size,(BYTE*)buffer,0,currentPlayer->m_currentWorldID);
	}

	m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();

	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AllowExtremeMovement(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
      CPlayerObject * currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_Alive(formatedMessage->idRefrence);
	  if(currentPlayer)
		 currentPlayer->m_allowLargeMovement = true;

	  return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribRqstAttachmentStatesPlayerServerSide_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{//crash 121705
	   m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Lock();//have to lock before runtime list locks due to lock order strat
	   m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Lock();
	   CAccountObject * accountPtr = (CAccountObject *)m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountByID(idFromLoc);
       
	   if(!accountPtr)
	   {
		   m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		   m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
		   m_sharedInterfaceRef->m_eventLog->LogByPriority("AtribRqstAttachmentStatesPlayerServerSide_safe() cannot find DPNID:",(DWORD)idFromLoc,3);
		   return FALSE;
	   }

	   CPlayerObject * reciepiant = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByDPNID_PrefAlive(idFromLoc);//can be null

	   CPlayerObject * requestedData = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID(formatedMessage->idRefrence);//db already locked
	   if(requestedData)
	   {//found network entity
		if(!requestedData->m_inGame)
		{
		   m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		   m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
           return FALSE;
		}

		BOOL msgToSelf = FALSE;
		BOOL sendRaw = FALSE;

        ENTITYCFG_HEADER cfgHeader;
		cfgHeader.m_clanMember  = 0;
		cfgHeader.clanID        = 0;
		cfgHeader.m_groupMember = 0;

		
		if(requestedData)
		 if(!accountPtr->m_aiServer)
		  if(requestedData->m_currentID == accountPtr->m_currentID)
		  {//requesting self
	        if(requestedData->m_invunerableTemp)
				if(requestedData->m_energy > 0)
				{//make sure you are configuring yourself
				   requestedData->InvunerableTimeStamp();
				}

		    SendForceJetValuesIfAvail(requestedData->m_currentInventory,requestedData);//send force jet

			sendRaw = TRUE;
			if(requestedData->m_stasis != 2)
			   msgToSelf = TRUE;
		  }

        if(!sendRaw)
		if(!accountPtr->m_aiServer)
		if(reciepiant)
		  if(reciepiant->m_playerEntityCfgOutbox->GetCount() > 5)
		  {
		     m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			 m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			 return FALSE;
		  }

		CInventoryObjList * inventoryTempList = requestedData->m_currentInventory;
        if(requestedData->m_stasis == 2)
		{//aquire alternat internal cfg
           CAIObj * aiReferencePtr = m_sharedInterfaceRef->m_aiList->GetByIndex(requestedData->m_altCfgIndex);
           if(aiReferencePtr)
		   {//configure through bot reference
			   inventoryTempList = aiReferencePtr->m_inventoryCfg;
		   }
		   else
		   m_sharedInterfaceRef->m_eventLog->AddEvent("Configure shapeshifted error");
		}//end aquire alternat internal cfg

		if(reciepiant)
		{
			if(requestedData->m_networkAssignedID != reciepiant->m_networkAssignedID)
			{
				if(requestedData->m_teamTemp == reciepiant->m_teamTemp && requestedData->m_teamTemp > -1)
				{
					cfgHeader.m_clanMember = 1;
					m_sharedInterfaceRef->m_eventLog->AddEvent("temp team "+requestedData->m_charName,(DWORD)requestedData->m_teamTemp);
				}
				if(requestedData->m_groupID == reciepiant->m_groupID && requestedData->m_groupID > 0)
				{
					cfgHeader.m_groupMember = 1;
					//m_sharedInterfaceRef->m_eventLog->AddEvent("group "+requestedData->m_charName,(DWORD)requestedData->m_groupID);
				}
			}
		}

		cfgHeader.m_catagorie      = 21;
		cfgHeader.networkDefinedID = requestedData->m_networkAssignedID;
		cfgHeader.dbCfg            = requestedData->m_curEDBIndex;
		cfgHeader.m_isItem         = (signed char)formatedMessage->uniShort3;
		if(requestedData->m_aiControlled)
			cfgHeader.isAI		   = true;
		else
			cfgHeader.isAI		   = false;
		cfgHeader.clanID           = requestedData->m_clanHandle;
		cfgHeader.Xpos             = (short)requestedData->m_3dPosition.x;
        cfgHeader.Ypos             = (short)requestedData->m_3dPosition.y;
		cfgHeader.Zpos             = (short)requestedData->m_3dPosition.z;
		cfgHeader.anim             = requestedData->m_curAnim;
		cfgHeader.mount            = requestedData->m_aiCfgOfMounted;
		
        //name length
        cfgHeader.nameDataSize = requestedData->m_visName.GetLength();
        int textSizeABS = cfgHeader.nameDataSize;
		if(requestedData->m_noteriety)
           if(requestedData->m_noteriety->m_currentMurders >= m_sharedInterfaceRef->m_netComObject->m_murderCountMax)
		      cfgHeader.nameDataSize = -cfgHeader.nameDataSize;
        //inventory size
		int invenItemCount = 0;
        if(inventoryTempList){//valid data
          int trace = 0;
		  for(POSITION invenPos = inventoryTempList->GetHeadPosition();invenPos != NULL;)
		  {
			  CInventoryObj * invenObj = (CInventoryObj *) inventoryTempList->GetNext(invenPos);
			  if(invenObj->m_armed)
                 invenItemCount++;
		  }
		}//end valid data
		if(m_sharedInterfaceRef->m_teamGlidNotifier > 0)
		   if(cfgHeader.m_clanMember == 1)
              invenItemCount++;

		if(m_sharedInterfaceRef->m_groupGlidNotifier > 0)
			if(cfgHeader.m_groupMember == 1)
				invenItemCount++;

        cfgHeader.invenDataSize  = (short)(invenItemCount * sizeof(SHORTTYPE_DATA));
        cfgHeader.defCfgDataSize = (short)(requestedData->m_deformableMeshCount * sizeof(SHORTTYPE_DATA));

        int datasize = (cfgHeader.invenDataSize + cfgHeader.defCfgDataSize + textSizeABS)+sizeof(ENTITYCFG_HEADER);

		BYTE * dataBlock = new BYTE[datasize];
        //copy header
        CopyMemory(dataBlock,&cfgHeader,sizeof(ENTITYCFG_HEADER));

		int headerSz = sizeof(ENTITYCFG_HEADER);
		//load name
		int offset = headerSz;
		if(textSizeABS > 0)
		CopyMemory(&dataBlock[offset],requestedData->m_visName.GetBuffer(),textSizeABS);
        //load inven
		  if(invenItemCount > 0)
		  {//inven cfg
            SHORTTYPE_DATA * invenDataBlock = new SHORTTYPE_DATA[invenItemCount];
            int loopTrace = 0;
			for(POSITION invenPos = inventoryTempList->GetHeadPosition();invenPos != NULL;)
			{
			  CInventoryObj * invenObj = (CInventoryObj *) inventoryTempList->GetNext(invenPos);
			  if(invenObj->m_armed)
			  {
			     invenDataBlock[loopTrace].m_data = invenObj->m_itemData->m_globalID;//optimize
                 loopTrace++;
			  }
			}
			if(m_sharedInterfaceRef->m_teamGlidNotifier > 0)
		      if(cfgHeader.m_clanMember == 1)
			  {//add temp attachment notifier
				  int teamThing = requestedData->m_teamTemp % 4;
				  if(!teamThing)
					  invenDataBlock[loopTrace].m_data = m_sharedInterfaceRef->m_teamGlidNotifier;
				  else if(teamThing == 1)
					  invenDataBlock[loopTrace].m_data = m_sharedInterfaceRef->m_teamGlidNotifier+1;
				  else if(teamThing == 2)
					  invenDataBlock[loopTrace].m_data = m_sharedInterfaceRef->m_teamGlidNotifier+2;
				  else if(teamThing == 3)
					  invenDataBlock[loopTrace].m_data = m_sharedInterfaceRef->m_teamGlidNotifier+3;
				  else
					  invenDataBlock[loopTrace].m_data = m_sharedInterfaceRef->m_teamGlidNotifier;
				 loopTrace++;
			  }
		    if(m_sharedInterfaceRef->m_groupGlidNotifier > 0)
			  if(cfgHeader.m_groupMember == 1)
			  {//add group attachment notifier
				  invenDataBlock[loopTrace].m_data = m_sharedInterfaceRef->m_groupGlidNotifier;
				  loopTrace++;
			  }
			//parse
		    BYTE * invenByteData = (BYTE*)invenDataBlock;
		    offset = textSizeABS + headerSz;//optimize 
			if(cfgHeader.invenDataSize > 0)
			   CopyMemory(&dataBlock[offset],invenByteData,cfgHeader.invenDataSize);//this still neeed tons of optimization           
        

            if(invenDataBlock)
			   delete [] invenDataBlock;
		       invenDataBlock = 0;
		  }//end inven cfg
		  
		  
         //load deformable
		 if(requestedData->m_deformableMeshCount > 0)
		 {//valid def cfg
         SHORTTYPE_DATA * defCfgDataBlock = new SHORTTYPE_DATA[requestedData->m_deformableMeshCount];
         for(int dlp=0;dlp<requestedData->m_deformableMeshCount;dlp++)
		 {//deformable cfg loop
            defCfgDataBlock[dlp].m_data = requestedData->m_deformableCfg[dlp];
		 }//end deformable cfg loop
         //parse
		  BYTE * defCfgByteData = (BYTE*)defCfgDataBlock;
		  offset = textSizeABS + headerSz + cfgHeader.invenDataSize;
		  if(cfgHeader.defCfgDataSize > 0)
		  CopyMemory(&dataBlock[offset],defCfgByteData,cfgHeader.defCfgDataSize);
          if(defCfgDataBlock)
			 delete [] defCfgDataBlock;
		     defCfgDataBlock = 0;
		 }//end valid def cfg

		  if(sendRaw)
		  {//to self player direct send
              m_sharedInterfaceRef->m_netComObject->SendByteBlockMsg(DPNSEND_GUARANTEED,
							                        idFromLoc,
								                    dataBlock,
									                datasize,
												    m_sharedInterfaceRef->m_netComObject->m_sceneStatePackTimeout);

			  if(reciepiant)
			  {
					m_sharedInterfaceRef->UpdatePhysicsBlock(reciepiant);
					m_sharedInterfaceRef->UpdateClientStatistics(reciepiant,idFromLoc);
			  }
		  }//end to self player direct send
		  else if(!accountPtr->m_aiServer){//player output
		   
			if(reciepiant)
				reciepiant->m_playerEntityCfgOutbox->AddPacket(datasize,(BYTE*)dataBlock,0,reciepiant->m_currentWorldID);
			if(dataBlock)
			   delete [] dataBlock;
		       dataBlock = 0;
		  }//end player output
		  else
		  {//AI output
               accountPtr->AddEntityCfgMsg(datasize,(BYTE*)dataBlock,0,requestedData->m_currentWorldID);
			
			if(dataBlock)
			   delete [] dataBlock;
		       dataBlock = 0;
		  }//end AI out put
	   }//end found network entity
	   m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	   m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
	   return TRUE;
}
//--------------Handles system messages-----------------------------------------------//
BOOL CAttributeSystems::AtribRemovePlayerByNetworkID_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
	//only ai should be able to make this call otherwise heavily exploitable-regular player objects cannot be removed like this
	CPlayerObject * playerTemp = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_Alive_LOS(formatedMessage->uniInt);
	if(!playerTemp)
		return FALSE;
	if(playerTemp->m_aiControlled)
	   playerTemp->m_timeLastUpdated -= m_sharedInterfaceRef->m_netComObject->m_timeOutDuration;//force timeout to force removal into one section
   
    m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
   
	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribRequestItemInfo_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
     if(!m_sharedInterfaceRef->m_netComObject->m_runtimeItems)
	     return FALSE;

     m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Lock();

     CServerItemObj * lstObj = m_sharedInterfaceRef->m_netComObject->m_runtimeItems->GetObjectByID_LOS(formatedMessage->uniInt);
	 if(lstObj)
	 {
        m_sharedInterfaceRef->SendItemLocationMsg( DPNSEND_NOCOMPLETE | DPNSEND_PRIORITY_HIGH,
												   idFromLoc,
											       lstObj); 
		m_sharedInterfaceRef->m_netComObject->m_runtimeItems->Unlock();//only is locked if it found the object
		m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	    return TRUE;
	 }
	 m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	    
	 m_sharedInterfaceRef->m_eventLog->AddEvent("Could not find item id","",formatedMessage->uniInt);
     return FALSE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribClientRequestPlayer_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
	       CAccountObject * accountPtr = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountByID_LOS(idFromLoc);
           if(!accountPtr){//error locating account
               m_sharedInterfaceRef->m_eventLog->AddEvent("Error locating account 25");
               return FALSE;
		   }//end error locating account
   
		   if(formatedMessage->uniShort3 < 15)//VERSION EXPLOIT
		   {
               m_sharedInterfaceRef->m_eventLog->AddEvent("Version Exploit " + accountPtr->m_accountName);
			   m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
               return FALSE;
		   }
           
           //set client content version
		   accountPtr->m_localClientContentVersion = formatedMessage->uniInt2;

	       if(!accountPtr->m_characterOnAccount)
		   {
               m_sharedInterfaceRef->m_characterPendingLoader->AddCharacterToPendingList(accountPtr->m_accountName,formatedMessage->uniInt);
               m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			   return TRUE;
		   }
		   else
		   {//already loaded send spawn msg
			   m_sharedInterfaceRef->m_netComObject->SendSpawnMsg(DPNSEND_GUARANTEED,
                                                                         accountPtr->m_currentID,//idFromLoc,
								                                         (short)accountPtr->m_characterOnAccount->m_curEDBIndex,
								                                         accountPtr->m_characterOnAccount->m_currentWorldMap,
																		 accountPtr->m_characterOnAccount->m_altCfgIndex,
																		 (short)accountPtr->m_characterOnAccount->m_3dPosition.x,
																		 (short)accountPtr->m_characterOnAccount->m_3dPosition.y,
																		 (short)accountPtr->m_characterOnAccount->m_3dPosition.z);
               m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			   return TRUE;
		   }
		   m_sharedInterfaceRef->m_eventLog->AddEvent("***player already on account and should not be","->",accountPtr->m_accountName);
		   m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
           return FALSE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribDeleteCharacterFromAccount_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
           CAccountObject * accountPtr = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountByID_LOS(idFromLoc);
           if(!accountPtr){//error locating account
               m_sharedInterfaceRef->m_eventLog->AddEvent("Error locating account 6");
               return FALSE;
		   }//end error locating account

		   if(formatedMessage->uniInt < 0)
		   {
              m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid Range 2004 delete char ",accountPtr->m_accountName);
			  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
              return FALSE;
		   }

		   
		   if(m_sharedInterfaceRef->m_netComObject->m_accountDBMode == 1)
		   {
			      if(accountPtr->m_charsCountDMMode > -1)
				  {
                    m_sharedInterfaceRef->m_threadedDataProcessor->AddToQue(0,accountPtr->m_accountName,formatedMessage->uniInt,idFromLoc,3,NULL,0,0,0,0,accountPtr->m_currentlyPlayingGameName);
                    accountPtr->m_charsCountDMMode--;
				  }
				  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
                  return TRUE;
		   }

		   if(!accountPtr->m_characterOnAccount){
               m_sharedInterfaceRef->m_eventLog->AddEvent("Attempting to delete character but no characters exist ",accountPtr->m_accountName);
               m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			   return FALSE;
		   }
           
		   if(!accountPtr->m_characterOnAccount)
		   {
               m_sharedInterfaceRef->m_eventLog->AddEvent("Character does not exist on account (delete process):",accountPtr->m_accountName);
               m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			   return FALSE;
		   }
               CPlayerObject * plObj = accountPtr->m_characterOnAccount;
               
			   
			   
			   if(accountPtr->m_currentcharInUse != -1)
			   {
                  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
                  return FALSE;
			   }
  
			   delete plObj;
               plObj = 0;
               accountPtr->m_characterOnAccount = NULL;

			   m_sharedInterfaceRef->m_eventLog->AddEvent("Client has deleted player on his account :",accountPtr->m_accountName);
	           //send notification to client that deletetion
	     /*      m_sharedInterfaceRef->SendAttribMsg( DPNSEND_GUARANTEED,
													idFromLoc,
													14,
													formatedMessage->uniInt,
													0,
													0,
													0,
													NULL);
		*/	   m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
	   return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribPlayerIsLooting(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
          m_sharedInterfaceRef->m_eventLog->LogByPriority("AtribPlayerIsLooting exec",2);


          CPlayerObject * targetObj = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID(formatedMessage->uniInt);
          if(!targetObj)
		  {//cannot find target
             m_sharedInterfaceRef->m_eventLog->AddEvent("Error 12501 cannot locate target of looting :","Network ID ",formatedMessage->uniInt);
             return FALSE;
		  }//end cannot find target
          if(targetObj->m_alive)
		  {//error cannot loot live player object
             m_sharedInterfaceRef->m_eventLog->AddEvent("Error 12502 attempting to loot live target");
             return FALSE;
		  }//end error cannot loot live player object
          CPlayerObject * takersObj = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID(formatedMessage->idRefrence);
          if(!takersObj)
		  {//cannot find taker
             m_sharedInterfaceRef->m_eventLog->AddEvent("Error 12503 cannot locate taker of looting :","Network ID ",formatedMessage->idRefrence);
             return FALSE;
		  }//end cannot find taker
          if(!targetObj->m_currentInventory)
		  {//error no inventory
             m_sharedInterfaceRef->m_eventLog->AddEvent("Error 12504 no valid inventory :");
             return FALSE;
		  }//end error no inventory
		  if(targetObj->m_curEDBIndex < 0)
			 return FALSE;//sick error
          CMovementObj * DBPtr = m_sharedInterfaceRef->m_entObjList->GetObjByIndex(targetObj->m_curEDBIndex);
		  if(!DBPtr)
		  {//error bad db ref on entity 
             m_sharedInterfaceRef->m_eventLog->AddEvent("Error 12507 bad target DB ref","",targetObj->m_curEDBIndex);
             return FALSE;
		  }//end error bad db ref on entity 
		  //loop through inventory skipping non disarmables that exist on death cfg
		  int trace = 0;
		  POSITION invenPosLast = NULL;
		  for(POSITION invPos = targetObj->m_currentInventory->GetHeadPosition();(invenPosLast = invPos) != NULL;)
		  {//inventory loop
             CInventoryObj * invenObj = (CInventoryObj *)targetObj->m_currentInventory->GetNext(invPos);
			 if(trace == formatedMessage->uniInt2)
			 {//LOOT this index
			   bool isDefaultItem = false;
			   if(DBPtr->m_deathCfg)//prevent looting of default items (worthless)
                for(POSITION dPos = DBPtr->m_deathCfg->GetHeadPosition();dPos != NULL;)
				{//death cfg loop
                   CCfgObj * deathCfg = (CCfgObj *)DBPtr->m_deathCfg->GetNext(dPos);
				   if(deathCfg->m_globalCFG == invenObj->m_itemData->m_globalID)                      
				   {//default item - the old 'continue' only skipped this inner loop, so it was looted anyway
					  isDefaultItem = true;
					  break;
				   }
				}//end death cfg loop
			   if(isDefaultItem)
				  break;//picked a default item - nothing to take
			   //ok we made it so lets transfer some stuff
			   targetObj->m_currentInventory->RemoveAt(invenPosLast);//remove inventory from target
			   invenObj->m_armed = FALSE;
               takersObj->m_currentInventory->AddTail(invenObj);
			   //end tranfering stuff
			   //if item taken was non disarmable-must replace with somthing
               if(!invenObj->m_itemData->m_disarmable)
			   {//must arm correct death cfg
                  if(DBPtr->m_deathCfg)
                     for(POSITION dPos = DBPtr->m_deathCfg->GetHeadPosition();dPos != NULL;)
					 {//death cfg loop
                        CCfgObj * deathCfg = (CCfgObj *)DBPtr->m_deathCfg->GetNext(dPos);
						CItemObj * dItemRef = DBPtr->GetItemByID(deathCfg->m_globalCFG);
					    if(dItemRef->m_locationID == invenObj->m_itemData->m_locationID)
						{//found item on death cfg
                           m_sharedInterfaceRef->BroadcastInventoryChange(targetObj,
										            dItemRef->m_globalID,
										            0,
										            FALSE,
													dItemRef);  
						}//end found item on death cfg
					 }//end death cfg loop
			   }//end must arm correct death cfg
			   else{//not special just uneqip msg
                    m_sharedInterfaceRef->BroadcastInventoryChange(targetObj,
										     -invenObj->m_itemData->m_globalID,
										     0,
										     FALSE,
											 invenObj->m_itemData);
			   }//end not special just uneqip msg
			   //done replacing necessary 
			 }//end LOOT this index
			 trace++;
		  }//end inventory loop
	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribRequestCharList_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
	  CAccountObject * accountPtr = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountByID_LOS(idFromLoc);
      if(!accountPtr){//error locating account
          m_sharedInterfaceRef->m_eventLog->AddEvent("Error locating account 7");
          return FALSE;
	  }//end error locating account
      m_sharedInterfaceRef->m_netComObject->SendTimeSyncPacket( DPNSEND_GUARANTEED,
																idFromLoc,
																(GetTickCount()-m_sharedInterfaceRef->m_netComObject->m_serverStartTime),
																m_sharedInterfaceRef->m_netComObject->m_frequencyLimit);
	
	  if(m_sharedInterfaceRef->m_netComObject->m_accountDBMode == 1)
	  {//db mode
         //limit requests
         if(accountPtr->m_currentCharRequestCount < 5)
		 {
            accountPtr->m_currentCharRequestCount++;
		    m_sharedInterfaceRef->m_threadedDataProcessor->AddToQue(0,accountPtr->m_accountName, formatedMessage->uniInt, idFromLoc, 0, NULL, 0, 0, 0, 0, accountPtr->m_currentlyPlayingGameName);
		 }
		 else
			 m_sharedInterfaceRef->m_eventLog->AddEvent("***Account spamming logon:"+accountPtr->m_accountName);
	  }//end db mode
	  else
	     m_sharedInterfaceRef->m_eventLog->AddEvent("***Account mode needs to be set to 1*****");

      m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
      return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribSetInvenItemSwap_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
     //ussually used when createing a character
          CAccountObject * accountPtr = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountByID_LOS(idFromLoc);
          if(!accountPtr){//error locating account
               m_sharedInterfaceRef->m_eventLog->AddEvent("Error locating account 8");
               return FALSE;
		  }//end error locating account
		  if(!accountPtr->m_characterOnAccount)
		  {//bad things
              m_sharedInterfaceRef->m_eventLog->AddEvent("No characters created yet 006");
			  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			  return FALSE;
		  }//end bad things

           int charSelection = 0;

           if(charSelection < 0)
		   {//bad things
              m_sharedInterfaceRef->m_eventLog->AddEvent("No characters created yet");
			  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			  return FALSE;
		   }//end bad things

           CPlayerObject * characterOnAccount = accountPtr->m_characterOnAccount;
		   if(characterOnAccount->m_currentInventory)
           for(POSITION posLoc = characterOnAccount->m_currentInventory->GetHeadPosition();posLoc != NULL;)
		   {
                  CInventoryObj * invObj = (CInventoryObj *)characterOnAccount->m_currentInventory->GetNext(posLoc);
                  if(invObj->m_itemData->m_locationID == formatedMessage->uniInt)
				  {//found object by location id
					 if(invObj->m_armed)
					 {
                       invObj->m_itemData->m_globalID = formatedMessage->uniInt2;//replace with new item
                       break;
					 }
				  }//end found object by location id
		   }
           m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
		   return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribAIServerRequestHousingState_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
	if (!m_sharedInterfaceRef || !formatedMessage)
		return FALSE;
	return m_sharedInterfaceRef->SendHousingState(idFromLoc, formatedMessage->uniInt);
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribSetDeformableItem_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
     //ussually used when createing a character
          CAccountObject * accountPtr = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountByID_LOS(idFromLoc);
          if(!accountPtr){//error locating account
               m_sharedInterfaceRef->m_eventLog->AddEvent("Error locating account 9");
               return FALSE;
		  }//end error locating account
		  if(!accountPtr->m_characterOnAccount)
		  {//bad things
              m_sharedInterfaceRef->m_eventLog->AddEvent("No characters created yet 006");
			  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			  return FALSE;
		  }//end bad things

           int charSelection = 0;

           if(charSelection < 0)
		   {//bad things
              m_sharedInterfaceRef->m_eventLog->AddEvent("No characters created yet");
			  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			  return FALSE;
		   }//end bad things

              CPlayerObject * characterOnAccount = accountPtr->m_characterOnAccount;
			  if(characterOnAccount->m_deformableCfg)
			  {
				if(characterOnAccount->m_deformableMeshCount > formatedMessage->uniInt)
					characterOnAccount->m_deformableCfg[formatedMessage->uniInt] = formatedMessage->uniInt2;
				else
					m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid deformable index");
			  }
		  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
		  return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribRequestCharInventory_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
          CPlayerObject * currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
          if(!currentPlayer)
		  {
              m_sharedInterfaceRef->m_eventLog->AddEvent("Requesting inventory but not spawned..");
              return FALSE;
		  }


          if(currentPlayer->m_currentInventory)
			m_sharedInterfaceRef->SendPlayerInventory(	currentPlayer->m_currentInventory,
														FALSE,
														idFromLoc,
														formatedMessage->uniInt,
														currentPlayer,
														1,
														formatedMessage->uniShort3);

		  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		  return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribRequestBadgeInfo(LPATTRIB_STRUCT formatedMessage, DPNID idFromLoc)
{
	CPlayerObject * currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
	if(!currentPlayer)
	{
		m_sharedInterfaceRef->m_eventLog->AddEvent("Requesting inventory but not spawned..");
		return FALSE;
	}
	
	m_sharedInterfaceRef->SendBadgeMessage(currentPlayer);

	m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribSetSkillTitle_safe(LPATTRIB_STRUCT formatedMessage, DPNID idFromLoc)
{
	// Get Player Object
	CPlayerObject *currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
	
	// Make Sure We Get A Valid Player Object
	if(!currentPlayer)
	{
		m_sharedInterfaceRef->m_eventLog->AddEvent("Requesting Skill Title Change But Not Spawned..");
		return FALSE;
	}

	//Store the new skill type
	currentPlayer->m_skills->m_skillToDisplay = formatedMessage->uniInt;
	m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED, idFromLoc, m_sharedInterfaceRef->m_cabLib.ConvToString(formatedMessage->uniInt), 0, currentPlayer, 0);

	currentPlayer->ReassembleName(m_sharedInterfaceRef);
	m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();

	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribRequestCharArmedInventory_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
			CPlayerObject * currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
			if(!currentPlayer)
			{
              m_sharedInterfaceRef->m_eventLog->AddEvent("Requesting inventory but not spawned..");
              return FALSE;
			}

			if(formatedMessage->uniInt2 == 1)
			{ // Include Noteriety Information
				if (currentPlayer->m_noteriety)
				{ // Player has Noteriety
					  m_sharedInterfaceRef->SendAttribMsg(DPNSEND_GUARANTEED,
			                      idFromLoc,
					              34,
				                  currentPlayer->m_noteriety->m_currentMurders,//Murders
								  0,
					              0,
					              0,
								  currentPlayer);//update client murders
				} // End Player Has Noteriety
			} // End Include Noteriety Information

			if(currentPlayer->m_currentInventory)
				m_sharedInterfaceRef->SendPlayerInventory(   currentPlayer->m_currentInventory,
																	FALSE,
																	idFromLoc,
																	formatedMessage->uniInt,
																	currentPlayer,
																	2,
																	formatedMessage->uniShort3);

			m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribRequestCharUnarmedInventory_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
		CPlayerObject * currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
		if(!currentPlayer)
		{
              m_sharedInterfaceRef->m_eventLog->AddEvent("Requesting inventory but not spawned..");
              return FALSE;
		}

		if(currentPlayer->m_currentInventory)
           m_sharedInterfaceRef->SendPlayerInventory(   currentPlayer->m_currentInventory,
														FALSE,
														idFromLoc,
														formatedMessage->uniInt,
														currentPlayer,
														0,
														formatedMessage->uniShort3);

		m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribRequestBuyItemFromStore_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
          CPlayerObject * currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
          if(!currentPlayer)
		  {
              m_sharedInterfaceRef->m_eventLog->AddEvent("Cannot figure player that is buying");
              return FALSE;
		  }

		  if(currentPlayer->m_energy < 1)
		  {
		     m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		     return FALSE;
		  }

		  if(formatedMessage->uniShort3 <= 0 || formatedMessage->uniShort3 >= 30000)//0 used to slip through as a 0 quantity
			 formatedMessage->uniShort3 = 1;


		  if(currentPlayer->m_stencilChannel < 0)//cannot find stencil
		     m_sharedInterfaceRef->m_eventLog->AddEvent("Cannot find establishment-currentPlayer->m_stencilChannel");


		  CCommerceObj * cmPtr = m_sharedInterfaceRef->m_commerceDB->GetEstablishmentByPosition(currentPlayer->m_3dPosition,currentPlayer->m_stencilChannel);//read only is ok to read accross threads
          if(cmPtr)
		  {//found store zone
             if(cmPtr->m_storeInventory->VerifyInventoryGLID(formatedMessage->uniInt))
			 {//store inventory verified
				 CItemObj * invObj = m_sharedInterfaceRef->m_itemManager->GetItemByID_LOS(formatedMessage->uniInt);
		         if(invObj)
				 {//details
				   
				   int slotsRequested = 1;
                   if(!invObj->m_stackable)
				       slotsRequested = formatedMessage->uniShort3;
				   else
				   {//stackable
				      //==prevent overbuy==//
					  CInventoryObj * itemObjTemp = (CInventoryObj *)currentPlayer->m_currentInventory->GetByGLID(formatedMessage->uniInt);
					  if(itemObjTemp)
					  {
					     if(itemObjTemp->m_quanity > 30000)//eps
						    itemObjTemp->m_quanity = 30000;

                         int allowedBuyingQuan = 30000 - itemObjTemp->m_quanity;
                         if(formatedMessage->uniShort3 > allowedBuyingQuan)
                            formatedMessage->uniShort3 = allowedBuyingQuan;//limit buying
					  }
					  else{//none currently found in inventory
                         if(formatedMessage->uniShort3 > 30000)
                            formatedMessage->uniShort3 = 30000;//limit buying
					  }//end none currently found in inventory
				   }//end stackable
				   
				   if(currentPlayer->m_currentInventory->CanTakeObjects(m_sharedInterfaceRef->m_netComObject->m_maxItemLimit_onChar,slotsRequested))
				   {//not overloaded
				    unsigned int quanCnv = formatedMessage->uniShort3;
				    unsigned int mCostCnv = invObj->m_marketCost;
				    unsigned int totalCostCnv = quanCnv * mCostCnv; 
					   
					if(m_sharedInterfaceRef->m_currencySystem->RemoveCurrencyIfEnoughExists(m_sharedInterfaceRef->m_itemManager,currentPlayer->m_currentInventory,-1,totalCostCnv))
					{//can afford this
						//to do should refresh inventory
						currentPlayer->m_currentInventory->AddInventoryItem(formatedMessage->uniShort3,
																			FALSE,
																			invObj);
					
						m_sharedInterfaceRef->SendTextMsg(	DPNSEND_GUARANTEED,
															idFromLoc,
															"Transaction Completed",
															0,
															currentPlayer,
															0);//confermation
						m_sharedInterfaceRef->m_itemManager->Unlock();
						m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
					   return TRUE;
					}//end can afford this
					else{//dont have the cash
					  m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
						                             idFromLoc,
						                             "Not Enough MDB Credits",
							                         0,
													 currentPlayer,
													 0);//ack!
					}//end dont have the cash
				   }//end can afford this
				   else{//overloaded
                      m_sharedInterfaceRef->SendGeneralMessageHandler(29,currentPlayer);//overloaded
				   }//end overloaded
				   m_sharedInterfaceRef->m_itemManager->Unlock();
				 }//end details
				 else{//failed retrieving GLinventory
                    m_sharedInterfaceRef->m_eventLog->AddEvent("Failed retrieving GL inventory..","GLID:",formatedMessage->uniInt);
				 }//end failed retrieving GLinventory
			 }//end store inventory verified
			 else{//attempt on accessing inventory item that does not exist
                m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid store item..");
			 }//end attempt on accessing inventory item that does not exist
		  }//end found store zone
		  else{//error not in store zone
             m_sharedInterfaceRef->m_eventLog->AddEvent("Buying out of store zone..");
		  }//end error not in store zone
		  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		  return FALSE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribRequestRepairItem_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
	CPlayerObject *currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
	if(!currentPlayer)
	{
		m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid player trying to repair item");
		return FALSE;
	}

	if(currentPlayer->m_stencilChannel < 0)//cannot find stencil
		m_sharedInterfaceRef->m_eventLog->AddEvent("Cannot find establishment-store currentPlayer->m_stencilChannel");

	CCommerceObj *cmPtr = m_sharedInterfaceRef->m_commerceDB->GetEstablishmentByPosition(currentPlayer->m_3dPosition,currentPlayer->m_stencilChannel);
	if(cmPtr)
	{//found the store
		CItemObj *invObj = m_sharedInterfaceRef->m_itemManager->GetItemByID_LOS(formatedMessage->uniInt);
		unsigned int totalCostCnv = (invObj->m_requiredSkillLevel * 10) + 10;
		if(invObj)
		{//retrieve item in question
			for(POSITION posLoc = currentPlayer->m_currentInventory->GetHeadPosition(); posLoc != NULL;)
			{//loop through player inventory
				CInventoryObj * itObj = (CInventoryObj *)currentPlayer->m_currentInventory->GetNext(posLoc);
				if(formatedMessage->uniInt == itObj->m_itemData->m_globalID)
				{//found item
					if(itObj->m_itemData->m_hp > 0 && itObj->m_itemData->m_hp < itObj->m_itemData->m_maxHP)
					{//check if can be repaired
						if(m_sharedInterfaceRef->m_currencySystem->RemoveCurrencyIfEnoughExists(m_sharedInterfaceRef->m_itemManager,currentPlayer->m_currentInventory,-1,totalCostCnv))
							itObj->m_itemData->m_hp = itObj->m_itemData->m_maxHP;
						else
							m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
															  idFromLoc,
															  "Not Enough MDB Credits",
															  0,
															  currentPlayer,
															  0);//ack!
					}//end check if can be repaired
					break;
				}//end found item
			}//loop through player inventory
			m_sharedInterfaceRef->m_itemManager->Unlock();
		}//end retrieve item in question
	}//end found the store
	else
	{//error not in store zone
		m_sharedInterfaceRef->m_eventLog->AddEvent("Repair attemp out of store");
		m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		return FALSE;
	}//end error not in store zone
	m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribRequestSellItemToStore_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
          CPlayerObject * currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
          if(!currentPlayer)
		  {
              m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid player selling to store");
              return FALSE;
		  }

		  if(formatedMessage->uniShort3 <= 0 || formatedMessage->uniShort3 >= 30000)//0 used to slip through as a 0 quantity
			 formatedMessage->uniShort3 = 1;
 
		  if(currentPlayer->m_stencilChannel < 0)//cannot find stencil
		     m_sharedInterfaceRef->m_eventLog->AddEvent("Cannot find establishment-store currentPlayer->m_stencilChannel");

		  CCommerceObj * cmPtr = m_sharedInterfaceRef->m_commerceDB->GetEstablishmentByPosition(currentPlayer->m_3dPosition,currentPlayer->m_stencilChannel);
          if(cmPtr)
		  {//found store zone
             
				 CItemObj * invObj = m_sharedInterfaceRef->m_itemManager->GetItemByID_LOS(formatedMessage->uniInt);
		         if(invObj)
				 {//details
					POSITION posLast;
					for(POSITION posLoc = currentPlayer->m_currentInventory->GetHeadPosition();(posLast = posLoc) != NULL;)
					{//player inventory
                        CInventoryObj * plObj = (CInventoryObj *)currentPlayer->m_currentInventory->GetNext(posLoc);
						if(formatedMessage->uniInt == plObj->m_itemData->m_globalID &&
						   plObj->m_armed == FALSE)
						{
                            if(formatedMessage->uniShort3 > plObj->m_quanity)
							   formatedMessage->uniShort3 = plObj->m_quanity;

							//int cost = (invObj->m_marketCost / 4)*(int)formatedMessage->uniShort3;
							int cost = (invObj->m_marketCost * (int)formatedMessage->uniShort3) / 4;
							int result = m_sharedInterfaceRef->m_currencySystem->AddCurrency(m_sharedInterfaceRef->m_itemManager,currentPlayer->m_currentInventory,-1,cost);
							if(result != 1)
								m_sharedInterfaceRef->m_eventLog->AddEvent("AtribRequestSellItemToStore_safe AddCurrency Error:",(DWORD)result);

							currentPlayer->m_currentInventory->DeleteByGLIDUnarmedOnlyWQuanity(invObj->m_globalID,formatedMessage->uniShort3);
							m_sharedInterfaceRef->SendTextMsg(  DPNSEND_GUARANTEED,
																idFromLoc,
																"Transaction Completed",
																0,
																currentPlayer,
																0);//confermation
		                    break;     
						}
					}//end player inventory
					m_sharedInterfaceRef->m_itemManager->Unlock();
				 }//end details
				 else{//failed retrieving GLinventory
                    m_sharedInterfaceRef->m_eventLog->AddEvent("Failed retrieving GL inventory..","GLID:",formatedMessage->uniInt);
					m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
                    return FALSE;
				 }//end failed retrieving GLinventory
		  }//end found store zone
		  else{//error not in store zone
             m_sharedInterfaceRef->m_eventLog->AddEvent("Selling attemp out of store");
			 m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
             return FALSE;
		  }//end error not in store zone
		  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		  return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribRequestToLootItem_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
          CPlayerObject * currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
          if(!currentPlayer)
		  {
              m_sharedInterfaceRef->m_eventLog->AddEvent("Requesting looting but not spawned..","",143);
              return FALSE;
		  }
		  if(currentPlayer->m_gm == TRUE && currentPlayer->m_admin == FALSE)
		  {
		     m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	         return FALSE;
		  }

		  if(formatedMessage->uniShort3 <= 0 || formatedMessage->uniShort3 >= 30000)//0 used to slip through as a 0 quantity
			 formatedMessage->uniShort3 = 1;

		  if(currentPlayer->m_energy < 1)
		  {
              m_sharedInterfaceRef->m_eventLog->AddEvent("Player attempting to loot while dead..");
			  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		      return FALSE;
		  }

		  int lootersCurrentLevel = currentPlayer->m_currentHighestLevel;

		  BOOL lootSuccess = FALSE;
		  CAxList * channelList = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetListByChannel(currentPlayer->m_currentWorldID);

		  if(channelList)
          for(CAxLinkObj * posLoc = channelList->GetHeadPosition_unsafe();posLoc != NULL;)
		  {//players on server loop
              CPlayerObject * pPtr = (CPlayerObject *)channelList->GetNext_unsafe(&posLoc);
              if(pPtr->m_energy > 0)
		         continue;
                 
	          if(m_sharedInterfaceRef->m_cabLib.GL_matrixFunctions.BoundingSphereCheck(pPtr->m_dynamicRadius,currentPlayer->m_dynamicRadius,pPtr->m_3dPosition.x,pPtr->m_3dPosition.y,pPtr->m_3dPosition.z,currentPlayer->m_3dPosition.x,currentPlayer->m_3dPosition.y,currentPlayer->m_3dPosition.z))
                 if(pPtr->LootAccessCheck(currentPlayer,m_sharedInterfaceRef->m_lootMode))//check accessability
				 {//in radius and access to loot is valid
				    BOOL chkSafeZn = TRUE;
					BOOL proceed = TRUE;
					if(!pPtr->m_aiControlled)
					{//not ai
			         if(pPtr->m_noteriety)
                       if(pPtr->m_noteriety->m_currentMurders >= m_sharedInterfaceRef->m_netComObject->m_murderCountMax)
		                  chkSafeZn = FALSE;

                     if(chkSafeZn)
			         {
			           if(pPtr->m_inSafeZone)
			           {//in safe zone
				          if(pPtr->m_accountNumberRefOld != currentPlayer->m_accountNumberRef)//looting somone elses body is not allowed in safe zone
                             proceed = FALSE;
				       }//end looting somone elses body is not allowed in safe zone
			         }//end in safe zone

					 if(proceed)
					 if(currentPlayer->m_invunerableTemp)
					 if(pPtr->m_handle != currentPlayer->m_handle)
	                 {//cannot loot while invunerable
                      if(!currentPlayer->Vulnerable())
		              {
			             if(!currentPlayer->m_aiControlled)
                             m_sharedInterfaceRef->SendGeneralMessageHandler(87,currentPlayer);
		                     proceed = FALSE;
		              }
	                 }//end cannot loot while invunerable
					}//end not ai
			        if(!proceed)
						continue;
					//remove from body
					//regular inventory
					if(!currentPlayer->m_currentInventory->CanTakeObjects(m_sharedInterfaceRef->m_netComObject->m_maxItemLimit_onChar,1))
					{//not overloaded
						m_sharedInterfaceRef->SendGeneralMessageHandler(29,currentPlayer);//overloaded
						m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
						return FALSE;
					}
					POSITION posVictimLast;
                    for(POSITION posVictim = pPtr->m_currentInventory->GetHeadPosition();(posVictimLast = posVictim) != NULL;)
					{//victim inventory
                        CInventoryObj * lootItemPtr = (CInventoryObj *)pPtr->m_currentInventory->GetNext(posVictim);
		                if(formatedMessage->uniInt == lootItemPtr->m_itemData->m_globalID && !lootItemPtr->m_itemData->m_permanent && !lootItemPtr->m_itemData->m_isInQuickSlots)
						{
                            CMovementObj * DBPtr = m_sharedInterfaceRef->m_entObjList->GetObjByIndex(pPtr->m_curEDBIndex);

							//remove item database and update visually
							if(DBPtr)
							if(lootItemPtr->m_armed)
							{//only broadcast if armed --update looted body visually
                              //if item taken was non disarmable-must replace with somthing
                              if(!lootItemPtr->m_itemData->m_disarmable)
							  {//must arm correct death cfg
                               if(DBPtr->m_deathCfg)
                               for(POSITION dPos = DBPtr->m_deathCfg->GetHeadPosition();dPos != NULL;)
							   {//death cfg loop
                                  CCfgObj * deathCfg = (CCfgObj *)DBPtr->m_deathCfg->GetNext(dPos);
								  CItemObj * dItemRef = m_sharedInterfaceRef->m_itemManager->GetItemByID_LOS(deathCfg->m_globalCFG);
					              if(dItemRef)
								  {
									  if(dItemRef->m_locationID == lootItemPtr->m_itemData->m_locationID)
									  {//found item on death cfg
										 m_sharedInterfaceRef->BroadcastInventoryChange(pPtr,
																  dItemRef->m_globalID,
																  0,
																  FALSE,
																  dItemRef);  
									  }//end found item on death cfg
									  m_sharedInterfaceRef->m_itemManager->Unlock();
								  }
							   }//end death cfg loop
							  }//end must arm correct death cfg
			                  else{//not special just uneqip msg
                                 m_sharedInterfaceRef->BroadcastInventoryChange(pPtr,
										                  -lootItemPtr->m_itemData->m_globalID,
										                  0,
										                  FALSE,
														  lootItemPtr->m_itemData);
							  }//end not special just uneqip msg
							}//end only broadcast if armed --update looted body visually

                            if(formatedMessage->uniShort3 > lootItemPtr->m_quanity)
							   formatedMessage->uniShort3 = lootItemPtr->m_quanity;

							
                            currentPlayer->m_currentInventory->AddInventoryItem(formatedMessage->uniShort3,
								                                                FALSE,
								                                                lootItemPtr->m_itemData
																                );
					        pPtr->m_currentInventory->DeleteByGLID(formatedMessage->uniInt,formatedMessage->uniShort3);

							lootSuccess = TRUE;
							m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
							return TRUE;
						}
					}//end victim inventory
				 }//end in radius
		  }//end players on server loop
		  
	// LOOTING logic for chest containers
	/*if(!lootSuccess)
	{
		CServerItemObj *pServerItem = this->m_sharedInterfaceRef->m_netComObject->m_runtimeItems->GetItemInRadius_LOSchannel(
			currentPlayer->m_Target3dPosition,
			(currentPlayer->m_dynamicRadius * 2.0f),
			currentPlayer->m_currentWorldID);
		if (pServerItem)
		{
			POSITION p1 = NULL, p2 = NULL;
			for(POSITION p1 = pServerItem->m_contents->GetHeadPosition(); (p2 = p1) != NULL;)
			{//victim inventory
				CInventoryObj *lootItemPtr = (CInventoryObj *)pServerItem->m_contents->GetNext(p1);
				if(formatedMessage->uniInt != lootItemPtr->m_itemData->m_globalID) 
					continue;

				// ERROR checking: Bounds check on quantity to loot
				if(formatedMessage->uniShort3 > lootItemPtr->m_quanity)
					formatedMessage->uniShort3 = lootItemPtr->m_quanity;

				// VERIFY if player has room in their inventory to loot
				if(currentPlayer->m_currentInventory->CanTakeObjects(
					m_sharedInterfaceRef->m_netComObject->m_maxItemLimit_onChar, formatedMessage->uniShort3))
				{
					currentPlayer->m_currentInventory->AddInventoryItem(formatedMessage->uniShort3,
						FALSE,
						lootItemPtr->m_itemData
						);
					pServerItem->m_contents->DeleteByGLID(formatedMessage->uniInt,formatedMessage->uniShort3);
					lootSuccess = TRUE;
				}
				else
				{
					m_sharedInterfaceRef->SendGeneralMessageHandler(29,currentPlayer);
					lootSuccess = FALSE;
				}
				break;
			}//end victim inventory
			this->m_sharedInterfaceRef->m_netComObject->m_runtimeItems->Unlock();
		}

		if (!lootSuccess)
			m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
				idFromLoc,
				"Too Far Away",
				0,
				currentPlayer,
				0);//confermation
	}*/
	m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribRequestToLootAllItems_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
		// Create a Player Object
		CPlayerObject * currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
		  
		  // Make sure there is a valid player object
          if(!currentPlayer)
		  {
              m_sharedInterfaceRef->m_eventLog->AddEvent("Requesting looting but not spawned..","",143);
			  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
              return FALSE;
		  }

		  
		  // Do not allow GMs to loot items
		  if(currentPlayer->m_gm == TRUE && currentPlayer->m_admin == FALSE)
		  {
		     m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	         return FALSE;
		  }
		  
		  // Make Sure Player is alive
		  if(currentPlayer->m_energy < 1)
		  {
              m_sharedInterfaceRef->m_eventLog->AddEvent("Player attempting to loot while dead..");
			  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		      return FALSE;
		  }		  

		  // Set lootSuccess to false and switch it when we get loot.
		  BOOL lootSuccess = FALSE;

		  // Loop through the players on the server and find all that are dead (energy < 0)
		  BOOL chkSafeZn = TRUE;

          CAxList * channelList = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetListByChannel(currentPlayer->m_currentWorldID);//already locked

		  if(channelList)
          for(CAxLinkObj *  posLoc = channelList->GetHeadPosition_unsafe();posLoc != NULL;)
		  {//players on server loop
              CPlayerObject * pPtr = (CPlayerObject *)channelList->GetNext_unsafe(&posLoc);
              if(pPtr->m_energy > 0)
				  continue;// Player is still alive, continue loop.
                 
				 // Make sure player is within the allowed radius
	          if(m_sharedInterfaceRef->m_cabLib.GL_matrixFunctions.BoundingSphereCheckX(pPtr->m_dynamicRadius,currentPlayer->m_dynamicRadius,pPtr->m_3dPosition.x,pPtr->m_3dPosition.y,pPtr->m_3dPosition.z,currentPlayer->m_3dPosition.x,currentPlayer->m_3dPosition.y,currentPlayer->m_3dPosition.z))
				 if(pPtr->LootAccessCheck(currentPlayer,m_sharedInterfaceRef->m_lootMode))
			     {//in radius
				    chkSafeZn = TRUE; // Gets set to false when looting a murderer
					// Determine whether or not the object being looted is AI controlled or not
					if(!pPtr->m_aiControlled)
					{ // Not AI Controlled Check
						if(pPtr->m_noteriety)
							// Character has noteriety, see if they are considered a murderer.
							// If they are a murderer allow them to be looted in safe zones.
							if(pPtr->m_noteriety->m_currentMurders >= m_sharedInterfaceRef->m_netComObject->m_murderCountMax)
								chkSafeZn = FALSE;

						if(chkSafeZn)
						{ // Safe Zone Check
						  if(pPtr->m_inSafeZone)
						  {// Safe Zone Loot Check
							 // Determine if they are trying to loot their own body or someone elses
							 if(pPtr->m_accountNumberRefOld != currentPlayer->m_accountNumberRef)
								continue;
						  }// End Safe Zone Loot Check
						}// End Safe Zone Check
 
						if(currentPlayer->m_invunerableTemp)
						if(pPtr->m_handle != currentPlayer->m_handle)
						{//cannot loot somone when invunerable
							if(!currentPlayer->Vulnerable())
							{
								if(!currentPlayer->m_aiControlled)
									m_sharedInterfaceRef->SendGeneralMessageHandler(87,currentPlayer);

								continue;
							}
						}//end cannot loot somone when invunerable
					} // End Not AI Controlled Check
			        
					// BEGIN REMOVE FROM BODY COMMANDS
					// REMOVE REGULAR INVENTORY
					POSITION posVictimLast;
                    for(POSITION posVictim = pPtr->m_currentInventory->GetHeadPosition();(posVictimLast = posVictim) != NULL;)
					{//victim inventory
                       CInventoryObj * lootItemPtr = (CInventoryObj *)pPtr->m_currentInventory->GetNext(posVictim);
					   if(!lootItemPtr->m_itemData->m_permanent && !lootItemPtr->m_itemData->m_isInQuickSlots)
					   {
						//remove item database and update visually
						CMovementObj * DBPtr = m_sharedInterfaceRef->m_entObjList->GetObjByIndex(pPtr->m_curEDBIndex);
						if(DBPtr)
						if(lootItemPtr->m_armed)
						{//only broadcast if armed --update looted body visually
                          //if item taken was non disarmable-must replace with somthing
                          if(!lootItemPtr->m_itemData->m_disarmable)
						  {//must arm correct death cfg
                           if(DBPtr->m_deathCfg)
                           for(POSITION dPos = DBPtr->m_deathCfg->GetHeadPosition();dPos != NULL;)
						   {//death cfg loop
                              CCfgObj * deathCfg = (CCfgObj *)DBPtr->m_deathCfg->GetNext(dPos);
							  CItemObj * dItemRef = m_sharedInterfaceRef->m_itemManager->GetItemByID_LOS(deathCfg->m_globalCFG);
				              if(dItemRef)
							  {
								  if(dItemRef->m_locationID == lootItemPtr->m_itemData->m_locationID)
								  {//found item on death cfg
									 m_sharedInterfaceRef->BroadcastInventoryChange(  pPtr,
															                          dItemRef->m_globalID,
																					  0,
																					  FALSE,
																					  dItemRef);  
								  }//end found item on death cfg
								  m_sharedInterfaceRef->m_itemManager->Unlock();
							  }
						   }//end death cfg loop
						  }//end must arm correct death cfg
		                  else
						  {//not special just uneqip msg
                             m_sharedInterfaceRef->BroadcastInventoryChange(pPtr,
									                  -lootItemPtr->m_itemData->m_globalID,
									                  0,
									                  FALSE,
													  lootItemPtr->m_itemData);
						  }// end not special just uneqip msg
						}//end only broadcast if armed --update looted body visually
						
						if(!currentPlayer->m_currentInventory->CanTakeObjects(m_sharedInterfaceRef->m_netComObject->m_maxItemLimit_onChar,1))
						{// Player is Overloaded
							m_sharedInterfaceRef->SendGeneralMessageHandler(29,currentPlayer);
							m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
							return FALSE;
						}// End Player is Overloaded

						// Add to Looters inventory
                        currentPlayer->m_currentInventory->AddInventoryItem(lootItemPtr->m_quanity,FALSE,lootItemPtr->m_itemData);

						// Remove from looted object
						lootItemPtr->SafeDelete();
						delete lootItemPtr;
						lootItemPtr = 0;
						pPtr->m_currentInventory->RemoveAt(posVictimLast);
						
						lootSuccess = true;
					   }
					}//end victim inventory
				 }//end in radius
		  }//end players on server loop

	// LOOTING logic for chest containers
	/*if(!lootSuccess)
	{
		CServerItemObj *pServerItem = this->m_sharedInterfaceRef->m_netComObject->m_runtimeItems->GetItemInRadius_LOSchannel(
			currentPlayer->m_Target3dPosition,
			(currentPlayer->m_dynamicRadius * 2.0f),
			currentPlayer->m_currentWorldID);
		if (pServerItem)
		{
			POSITION p1 = NULL, p2 = NULL;

			// VERIFY the player has room in their inventory to loot
			for(p1 = pServerItem->m_contents->GetHeadPosition(); (p2 = p1) != NULL;)
			{
				CInventoryObj *lootItemPtr = (CInventoryObj *)pServerItem->m_contents->GetNext(p1);
				if (!lootItemPtr) continue;
				if(!currentPlayer->m_currentInventory->CanTakeObjects(
					m_sharedInterfaceRef->m_netComObject->m_maxItemLimit_onChar, 
					lootItemPtr->m_quanity))
					lootSuccess = TRUE;
				else
				{
					lootSuccess = FALSE;
					break;
				}
			}

			if (lootSuccess)
			{
				for(p1 = pServerItem->m_contents->GetHeadPosition(); (p2 = p1) != NULL;)
				{//victim inventory
					CInventoryObj *lootItemPtr = (CInventoryObj *)pServerItem->m_contents->GetNext(p1);
					if (!lootItemPtr) continue;
					currentPlayer->m_currentInventory->AddInventoryItem(formatedMessage->uniShort3,
						FALSE,
						lootItemPtr->m_itemData);
					pServerItem->m_contents->DeleteByGLID(lootItemPtr->m_itemData->m_globalID, 
						lootItemPtr->m_quanity);
					lootSuccess = TRUE;
				}//end victim inventory
			}
			this->m_sharedInterfaceRef->m_netComObject->m_runtimeItems->Unlock();
		}

		if (!lootSuccess)
			m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
				idFromLoc,
				"Too Far Away",
				0,
				currentPlayer,
				0);//confermation
	}*/
	m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL 
CAttributeSystems::AtribRequestToBankItem_safe(LPATTRIB_STRUCT formatedMessage, DPNID idFromLoc)
{
	// GET a pointer to the player object
	CPlayerObject *pPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
	if(!pPlayer)
	{
		m_sharedInterfaceRef->m_eventLog->AddEvent("AtribRequestToBankItem_safe(): FAILED to retrieve a valid player object for banking! ");
		return FALSE;
	}

	// ERROR checking: Check for a valid world stencil ID
	if (pPlayer->m_stencilChannel < 0)
		m_sharedInterfaceRef->m_eventLog->AddEvent("AtribRequestToBankItem_safe(): FAILED to detect a valid stencil channel! ");

	// GET a pointer to the bank object
	CBankObj *pBank = m_sharedInterfaceRef->m_bankZoneList->GetBankByPosition(pPlayer->m_3dPosition,
		pPlayer->m_stencilChannel);

	// ERROR checking: FAILED if there is no valid bank
	//	OR network spamming 
	//	OR a dead player 
	//	OR the player has no inventory
	//	OR a valid deposit quantity
	if (!pBank
	|| !pPlayer->AllowAsyncQueMini()
	|| (pPlayer->m_energy < 1)
	|| (!pPlayer->m_currentInventory)
	|| ((formatedMessage->uniShort3 < 0) || (formatedMessage->uniShort3 >= 30000)))
	{
		m_sharedInterfaceRef->m_eventLog->AddEvent("AtribRequestToBankItem_safe(): FAILED! ");
		m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		return FALSE;
	}

	// GET the character on the base account
	CPlayerObject *pBaseAccountPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetCurrentCharacterByID(idFromLoc);
	if(!pBaseAccountPlayer)
	{
		m_sharedInterfaceRef->m_eventLog->AddEvent("AtribRequestToBankItem_safe(): Cannot access base character on account where DPNID = ", (DWORD)idFromLoc);
		m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		return FALSE;
	}

	// GET the correct player bank object
	//	NOTE: CREATE banks at indices that don't exist
	CInventoryObjList *pPlayerBank = NULL;
	if (formatedMessage->uniInt2 < 0 || formatedMessage->uniInt2 >= MAX_BANKS)
	{//bad bank index from the client - a negative one crashed the server, a huge one froze it making banks
		m_sharedInterfaceRef->m_eventLog->AddEvent("AtribRequestToBankItem_safe(): bad bank index - ", (DWORD)formatedMessage->uniInt2);
		m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		return FALSE;
	}
	while ((int)pBaseAccountPlayer->m_banks.size() <= formatedMessage->uniInt2)
	{
		pPlayerBank = new CInventoryObjList();
		pBaseAccountPlayer->m_banks.push_back(pPlayerBank);
	}
	pPlayerBank = pBaseAccountPlayer->m_banks[formatedMessage->uniInt2];
	if (!pPlayerBank) 
	{
		m_sharedInterfaceRef->m_eventLog->AddEvent("AtribRequestToBankItem_safe(): Cannot access the bank at the specified index - ", (DWORD)formatedMessage->uniInt2);
		m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		return FALSE;
	}

	// ERROR checking: Make sure this deposit won't exceed the bank limit
	if(pPlayerBank->GetCount() >= m_sharedInterfaceRef->m_netComObject->m_maxItemLimit_inBank)
	{
		m_sharedInterfaceRef->m_eventLog->AddEvent("AtribRequestToBankItem_safe(): This deposit would exceed the bank limit! Deposit FAILED!");
		m_sharedInterfaceRef->SendGeneralMessageHandler(30, pPlayer);
		m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		return FALSE;
	}

	int dupLocalTrace = 0;	
	for(POSITION posLoc = pPlayer->m_currentInventory->GetHeadPosition(); posLoc != NULL;)
	{
		// NOTE: Only handle a GLID match!
		CInventoryObj *pItem = (CInventoryObj*)pPlayer->m_currentInventory->GetNext(posLoc);
		if (!pItem 
		|| (formatedMessage->uniInt != pItem->m_itemData->m_globalID)) 
			continue;

		// IF the GLIDs match, then we've found the item to deposit
		/* NOTE: uniInt2 is now being used to indicate the bank index
		// DUPE tracing(?)
		if(dupTrace != formatedMessage->uniInt2)
		{
			dupTrace++;
			continue;
		}*/

		// ERROR checking: We don't allow transactions with armed items
		if(pItem->m_armed)
		{
			m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			return FALSE;
		}

		// ERROR checking: We can't deal in quantities greater than our total inventory supply
		if(formatedMessage->uniShort3 > pItem->m_quanity)
			formatedMessage->uniShort3 = pItem->m_quanity;

		// ADD the specified quantity of this item to the player's bank
		CInventoryObj *pResult = pPlayerBank->AddInventoryItem(
			formatedMessage->uniShort3,
			FALSE,
			pItem->m_itemData);

		// REMOVE the specified quantity from the player's inventory
		BOOL bResult = pPlayer->m_currentInventory->DeleteByGLID_DPTRACE(pItem->m_itemData->m_globalID,
			0/*formatedMessage->uniInt2*/,
			formatedMessage->uniShort3);

		// UNLOCK the server player collection and RETURN
		m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		return TRUE;
	}

	// UNLOCK the server player collection
	m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	return TRUE;
}

/*BOOL CAttributeSystems::AtribRequestToBankItem_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
	m_sharedInterfaceRef->m_eventLog->AddEvent("START AtribRequestToBankItem_safe()");
         CPlayerObject * currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
		 if(!currentPlayer)
		 {
             m_sharedInterfaceRef->m_eventLog->AddEvent("Cannot find current player that is banking");
             return FALSE;
		 }

		 m_sharedInterfaceRef->m_eventLog->AddEvent("AtribRequestToBankItem_safe(): FOUND PLAYER");

		 if(!currentPlayer->AllowAsyncQueMini())
		 {
			 m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			 return FALSE;//throttle
		 }

		 m_sharedInterfaceRef->m_eventLog->AddEvent("AtribRequestToBankItem_safe(): ASYNC");

		  if(currentPlayer->m_energy < 1)
		  {
		     m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		     return FALSE;
		  }

		  if(formatedMessage->uniShort3 <= 0 || formatedMessage->uniShort3 >= 30000)//0 used to slip through as a 0 quantity
			 formatedMessage->uniShort3 = 1;

		  m_sharedInterfaceRef->m_eventLog->AddEvent("AtribRequestToBankItem_safe(): uniShor3: ", "", formatedMessage->uniShort3);

          if(currentPlayer->m_stencilChannel < 0)//cannot find stencil
	         m_sharedInterfaceRef->m_eventLog->AddEvent("Cannot find bank currentPlayer->m_stencilChannel AtribRequestToBankItem_safe");

		  CBankObj * bankPtr = m_sharedInterfaceRef->m_bankZoneList->GetBankByPosition(currentPlayer->m_3dPosition,currentPlayer->m_stencilChannel);
          if(bankPtr)
		  {//in bank zone
              
			  m_sharedInterfaceRef->m_eventLog->AddEvent("AtribRequestToBankItem_safe(): IN BANK ZONE");

		  CPlayerObject * characterOnBaseAccount = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetCurrentCharacterByID(idFromLoc);
          if(!characterOnBaseAccount)
		  {
			  m_sharedInterfaceRef->m_eventLog->AddEvent("Cannot access base character on account dpnid:",(DWORD)idFromLoc);
			  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
              return FALSE;
		  }

          if(!currentPlayer)
		  {
              m_sharedInterfaceRef->m_eventLog->AddEvent("Requesting to bank item, but failed","",143);
			  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
              return FALSE;
		  }
              
          if(characterOnBaseAccount->m_bankInventory->GetCount() >= m_sharedInterfaceRef->m_netComObject->m_maxItemLimit_inBank)
		  {
			  m_sharedInterfaceRef->m_eventLog->AddEvent("FAILED AtribRequestToBankItem_safe(): EXCEEDS MAXITEMLIMIT_INBANK!");
			 m_sharedInterfaceRef->SendGeneralMessageHandler(30,currentPlayer);//overloaded
			 m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
             return FALSE;
		  }
		      //remove from client inventory
		      //POSITION posLast;
			  //BOOL inventoryValid = FALSE;
		      int dupLocalTrace = 0;
		      if(currentPlayer->m_currentInventory)
			  for(POSITION posLoc = currentPlayer->m_currentInventory->GetHeadPosition();posLoc != NULL;)
			  {//player inventory
                 CInventoryObj * invObj = (CInventoryObj *)currentPlayer->m_currentInventory->GetNext(posLoc);
				  
				  if(formatedMessage->uniInt == invObj->m_itemData->m_globalID)
				  {
					  m_sharedInterfaceRef->m_eventLog->AddEvent("AtribRequestToBankItem_safe(): CORRECT GLID");

                    if(dupLocalTrace != formatedMessage->uniInt2)
					{
                       dupLocalTrace++;
                       continue;
					}

					if(invObj->m_armed)
					{
					   m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
					   return FALSE;
					}

			       
                    if(formatedMessage->uniShort3 > invObj->m_quanity)
					   formatedMessage->uniShort3 = invObj->m_quanity;

					CInventoryObj *pResult = characterOnBaseAccount->m_bankInventory->AddInventoryItem(formatedMessage->uniShort3,
								                                              FALSE,
								                                              invObj->m_itemData
																              );
					if (!pResult)
						m_sharedInterfaceRef->m_eventLog->AddEvent("FAILED to add inventory item to bank!");
					else
						m_sharedInterfaceRef->m_eventLog->AddEvent("SUCCEEDED in adding inventory item to bank!");

					BOOL bResult = currentPlayer->m_currentInventory->DeleteByGLID_DPTRACE(invObj->m_itemData->m_globalID,formatedMessage->uniInt2,formatedMessage->uniShort3);
					if (!bResult)
						m_sharedInterfaceRef->m_eventLog->AddEvent("FAILED to delete inventory item from player's inventory!");
					else
						m_sharedInterfaceRef->m_eventLog->AddEvent("SUCCEEDED in deleting inventory item from player's inventory!");
					   
					
                    m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
                    return TRUE;
				  }
			  }//end player inventory
			  //add item to bank
		  }//end in bank zone
		  
		  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		  m_sharedInterfaceRef->m_eventLog->AddEvent("START AtribRequestToBankItem_safe()");
	      return TRUE;
}*/
//------------------------------------------------------------------------------------//

BOOL CAttributeSystems::AtribTakeItemFromBank_safe(LPATTRIB_STRUCT formatedMessage, DPNID idFromLoc)
{
	// GET a reference to the current player
	CAccountObject * accountObject = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountByID_LOS(idFromLoc);//bank is stored on the account object, its locked there as well-this needs to be locked while accessing bank 
	if (!accountObject)
	{
		m_sharedInterfaceRef->m_eventLog->AddEvent("AtribTakeItemFromBank_safe(): Cannot find current account. Bank withdrawal FAILED!");
		return FALSE;
	}

	CPlayerObject * pPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
	if (!pPlayer)
	{
		m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
		m_sharedInterfaceRef->m_eventLog->AddEvent("AtribTakeItemFromBank_safe(): Cannot find current player. Bank withdrawal FAILED!");
		return FALSE;
	}

	// ERROR checking: Check for a valid world stencil ID
	if (pPlayer->m_stencilChannel < 0)
		m_sharedInterfaceRef->m_eventLog->AddEvent("AtribTakeItemFromBank_safe(): FAILED to detect a valid stencil channel! ");

	// GET a pointer to the bank object
	CBankObj *pBank = m_sharedInterfaceRef->m_bankZoneList->GetBankByPosition(pPlayer->m_3dPosition,
		pPlayer->m_stencilChannel);

	// ERROR checking: FAILED if there is no valid bank
	//	OR network spamming 
	//	OR a dead player 
	//	OR the player has no inventory
	//	OR a valid deposit quantity
	if (!pBank
	|| !pPlayer->AllowAsyncQueMini()
	|| (pPlayer->m_energy < 1)
	|| (!pPlayer->m_currentInventory)
	|| ((formatedMessage->uniShort3 < 0) || (formatedMessage->uniShort3 >= 30000)))
	{
		m_sharedInterfaceRef->m_eventLog->AddEvent("AtribTakeItemFromBank_safe(): FAILED! ");
		m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
		m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		return FALSE;
	}

	// GET the character on the base account
	CPlayerObject *pBaseAccountPlayer = accountObject->m_characterOnAccount;
	if(!pBaseAccountPlayer)
	{
		m_sharedInterfaceRef->m_eventLog->AddEvent("AtribTakeItemFromBank_safe(): Cannot access base character on account where DPNID = ", (DWORD)idFromLoc);
		m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
		return FALSE;
	}

	// ERROR checking: Verify the character has room in their inventory for a withdrawal
	if(!pPlayer->m_currentInventory->CanTakeObjects(m_sharedInterfaceRef->m_netComObject->m_maxItemLimit_onChar, 1))
	{
		m_sharedInterfaceRef->SendGeneralMessageHandler(29, pPlayer);
		m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
		return FALSE;
	}
	
	POSITION p1 = NULL, p2 = NULL;
	BOOL inventoryValid = FALSE;
	//int dupTrace = 0;
	CInventoryObjList *pPlayerBank = NULL;
	if (formatedMessage->uniInt2 >= 0 && formatedMessage->uniInt2 < (int)pBaseAccountPlayer->m_banks.size())
		pPlayerBank = pBaseAccountPlayer->m_banks[formatedMessage->uniInt2];
	if (!pPlayerBank) 
	{
		m_sharedInterfaceRef->m_eventLog->AddEvent("AtribTakeItemFromBank_safe(): No bank at the specified index - ", (DWORD)formatedMessage->uniInt2);
		m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
		return FALSE;
	}

	for(p1 = pPlayerBank->GetHeadPosition(); (p2 = p1) != NULL;)
	{
		// GET the next item in the bank inventory
		CInventoryObj *pItem = (CInventoryObj*)pPlayerBank->GetNext(p1);

		// ERROR checking: Only handle a GLID match!
		if (!pItem
		|| (formatedMessage->uniInt != pItem->m_itemData->m_globalID)) 
			continue;

		/* NOTE: uniInt2 is now being used to indicate the bank index
		// DUPE tracing(?)
		if(dupTrace != formatedMessage->uniInt2)
		{
			dupTrace++;
			continue;
		}*/

		// ERROR checking: Make sure we have a valid quantity
		if (formatedMessage->uniShort3 > pItem->m_quanity)
			formatedMessage->uniShort3 = pItem->m_quanity;

		// ADD the item to the player's inventory
		pPlayer->m_currentInventory->AddInventoryItem(formatedMessage->uniShort3,
			FALSE,
			pItem->m_itemData);

		// REMOVE the item from the player's bank
		pPlayerBank->DeleteByGLID_DPTRACE(pItem->m_itemData->m_globalID,
			0/*formatedMessage->uniInt2*/,
			formatedMessage->uniShort3);

		inventoryValid = TRUE;
		break;     
	}

	// UNLOCK the server player collection
	m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
	return TRUE;
}

/*BOOL CAttributeSystems::AtribTakeItemFromBank_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
	m_sharedInterfaceRef->m_eventLog->AddEvent("START AtribTakeItemFromBank_safe()");

          CPlayerObject * currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
		  if(!currentPlayer)
		  {
             m_sharedInterfaceRef->m_eventLog->AddEvent("Cannot find current player take item from bank ");
             return FALSE;
		  }

		  if(!currentPlayer->AllowAsyncQueMini())
		  {
		     m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			 return FALSE;//throttle
		  }

		  if(formatedMessage->uniShort3 <= 0 || formatedMessage->uniShort3 >= 30000)//0 used to slip through as a 0 quantity
			 formatedMessage->uniShort3 = 1;
		  
          if(currentPlayer->m_stencilChannel < 0)//cannot find stencil
	         m_sharedInterfaceRef->m_eventLog->AddEvent("Cannot find bank currentPlayer->m_stencilChannel AtribTakeItemFromBank_safe");

		  CBankObj * bankPtr = m_sharedInterfaceRef->m_bankZoneList->GetBankByPosition(currentPlayer->m_3dPosition,currentPlayer->m_stencilChannel);
          if(bankPtr)
		  {//in bank zone
              
		  CPlayerObject * characterOnBaseAccount = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetCurrentCharacterByID(idFromLoc);
          if(!characterOnBaseAccount)
		  {
			  m_sharedInterfaceRef->m_eventLog->AddEvent("Cannot access base character on account dpnid:",(DWORD)idFromLoc);
			  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
              return FALSE;
		  }

          if(!currentPlayer)
		  {
              m_sharedInterfaceRef->m_eventLog->AddEvent("Requesting to bank item, but failed","",143);
			  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
              return FALSE;
		  }

		  if(currentPlayer->m_energy < 1)
		  {
		     m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
             return FALSE;
		  }
     
		  if(currentPlayer->m_currentInventory)
          if(!currentPlayer->m_currentInventory->CanTakeObjects(m_sharedInterfaceRef->m_netComObject->m_maxItemLimit_onChar,1))
		  {//overloaded
			 m_sharedInterfaceRef->SendGeneralMessageHandler(29,currentPlayer);//overloaded
			 m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
             return FALSE;
		  }//end overloaded

		      //remove from bank inventory
		      POSITION posLast;
			  BOOL inventoryValid = FALSE;
			  int dupTrace = 0;
			  if(characterOnBaseAccount->m_bankInventory)
			  for(POSITION posLoc = characterOnBaseAccount->m_bankInventory->GetHeadPosition();(posLast = posLoc) != NULL;)
			  {//Bank inventory
                 CInventoryObj * plObj = (CInventoryObj *)characterOnBaseAccount->m_bankInventory->GetNext(posLoc);
		         if(formatedMessage->uniInt == plObj->m_itemData->m_globalID)
				 {
                    

                    if(dupTrace != formatedMessage->uniInt2)
					{

                       dupTrace++;
                       continue;
					}

					if(formatedMessage->uniShort3 > plObj->m_quanity)
					   formatedMessage->uniShort3 = plObj->m_quanity;

					currentPlayer->m_currentInventory->AddInventoryItem(formatedMessage->uniShort3,
								                                        FALSE,
								                                        plObj->m_itemData
																        );
                    characterOnBaseAccount->m_bankInventory->DeleteByGLID_DPTRACE(plObj->m_itemData->m_globalID,formatedMessage->uniInt2,formatedMessage->uniShort3);
                     
					

					inventoryValid = TRUE;
		            break;     
				 }
			  }//end Bank inventory
		  }//end in bank zone
		  
		  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		  m_sharedInterfaceRef->m_eventLog->AddEvent("END AtribTakeItemFromBank_safe()");
		  return TRUE;
}*/

//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribRequestPortal_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
		  CAccountObject * accountPtr = (CAccountObject *)m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountByID_LOS(idFromLoc);
          if(!accountPtr){//error locating account
             m_sharedInterfaceRef->m_eventLog->AddEvent("Cannot locate account of portal client");
			 return FALSE;
		  }//end error locating account

		  CPlayerObject * currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByDPNID_Alive_LOS(idFromLoc);
		  if(!currentPlayer)
		  {
		     m_sharedInterfaceRef->m_eventLog->AddEvent("cannot locating player that is attempting portal ");
			 m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			 return FALSE;
		  }

		  if(currentPlayer->m_aiControlled)
		  {//ai cannot portal
             m_sharedInterfaceRef->m_eventLog->AddEvent("AI cannot portal:",accountPtr->m_accountName,19);
			 m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			 m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			 return FALSE;
		  }//end ai cannot portal

          CPortalSysObj * portal = m_sharedInterfaceRef->m_portalDatabase->ReturnPortal(currentPlayer->m_Target3dPosition,currentPlayer->m_currentWorldID);
		  if(!portal)
		  {//invalid world
			  m_sharedInterfaceRef->SendGeneralMessageHandler(12,currentPlayer);//silent cancellation
			  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			  return FALSE;
		  }//end invalid world

          if(portal->m_keyRequired != 0)
		  {//requires key
			 if(!currentPlayer->m_currentInventory->UseIfKeyInInventory(portal->m_keyRequired))
			 { 
				 m_sharedInterfaceRef->SendGeneralMessageHandler(11,currentPlayer);
				 m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
				 m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
                 return FALSE;
			 }
		  }//end required key

		  if(portal->m_targetWorld < 0)
		  {//invalid world
              m_sharedInterfaceRef->m_eventLog->AddEvent("Inalid world Index negative",accountPtr->m_accountName,16);
			  m_sharedInterfaceRef->SendGeneralMessageHandler(12,currentPlayer);//silent cancellation
			  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			  return FALSE;
		  }//end invalid world

          CString worldName = "";
		  if(!m_sharedInterfaceRef->m_netComObject->m_currentWorldsUsed->GetZoneDataByChannel_safe(portal->m_targetWorld,&worldName,NULL,NULL))
		  {//invalid world
              m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid world Channel",accountPtr->m_accountName,17);
			  m_sharedInterfaceRef->SendGeneralMessageHandler(12,currentPlayer);//silent cancellation
			  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			  return FALSE;
		  }//end invalid world*/
          
		  if(currentPlayer->m_energy < 1)
		  {//dead
             m_sharedInterfaceRef->m_eventLog->AddEvent("Attempting portal but he is dead.",accountPtr->m_accountName,19);
			 m_sharedInterfaceRef->SendGeneralMessageHandler(12,currentPlayer);//silent cancellation
			 m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			 m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			 return FALSE;
		  }//end dead
  
          if(!m_sharedInterfaceRef->m_netComObject->UpdateAccount(currentPlayer,accountPtr))
		  {//update account
	         m_sharedInterfaceRef->m_eventLog->AddEvent("Error updateing account",accountPtr->m_accountName,119);
			 m_sharedInterfaceRef->SendGeneralMessageHandler(12,currentPlayer);//silent cancellation
			 m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			 m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			 return FALSE;
		  }//end update account
		  CPlayerObject * characterOnBaseAccount = accountPtr->GetCurrentCharacter();
		  if(!characterOnBaseAccount)
		  {
			  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
              return FALSE;
		  }
	      //retrieve world info
          //if(portal->m_targetWorld != characterOnBaseAccount->m_currentWorldID)
		  //{
             characterOnBaseAccount->m_invunerableTemp = TRUE;
			 characterOnBaseAccount->m_invunerableStamp = GetTickCount();
			 characterOnBaseAccount->m_invunOptionTemp = 1;
		  //}

	      characterOnBaseAccount->m_currentWorldMap = worldName;
		  characterOnBaseAccount->m_currentWorldID  = portal->m_targetWorld;
		  //convert position
		  characterOnBaseAccount->m_3dPosition.x = portal->m_wldPosition.x;
		  characterOnBaseAccount->m_3dPosition.y = portal->m_wldPosition.y;
		  characterOnBaseAccount->m_3dPosition.z = portal->m_wldPosition.z;
  
		  if(currentPlayer->m_aiControlled == FALSE)
			 m_sharedInterfaceRef->m_netComObject->m_currentWorldsUsed->DecrementZoneConnections_safe(currentPlayer->m_currentWorldID);

		  if(m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->RemoveByNetID_Alive(currentPlayer->m_networkAssignedID))//dont let nobody do anything to this
		     currentPlayer = 0;

		  characterOnBaseAccount->m_instanceType = 0;
		  
		  m_sharedInterfaceRef->m_netComObject->SendSpawnMsg(DPNSEND_GUARANTEED,
                                            idFromLoc,
								            (short)characterOnBaseAccount->m_curEDBIndex,
								            characterOnBaseAccount->m_currentWorldMap,
										    characterOnBaseAccount->m_altCfgIndex,
										    (short)characterOnBaseAccount->m_3dPosition.x,
										    (short)characterOnBaseAccount->m_3dPosition.y,
											(short)characterOnBaseAccount->m_3dPosition.z);

		  
          m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
		  return TRUE;
}
//------------------------------------------------------------------------------------//
// one placed house, passed to the data thread for saving (same layout in AttributeSystems.cpp)
struct HOUSE_SAVE_REC { int accountBind; int worldChannel; int type; float x, y, z; };

// Which house model (EARevolution.HSF) a house item builds. The item table has no
// "misc use value" column, so match the item name to the model names:
// "Small Log Cabin" -> "Small Cabin", "Triple Story Stone Hut" -> "Stone Hut 3(Level)" ...
static CString HouseWords(CString t)
{
	t.MakeLower();
	t.Replace("triple","3"); t.Replace("three","3"); t.Replace("double","2"); t.Replace("two","2");
	t.Replace("(", " "); t.Replace(")", " "); t.Replace("-", " ");
	return " " + t + " ";
}
static int PickHouseModel(CHousingObjList * models, CString itemName, int fallback)
{
	if(!models) return fallback;
	CString item = HouseWords(itemName);
	int best = -1, bestScore = 0, index = 0;
	for(POSITION pos = models->GetHeadPosition(); pos != NULL; index++)
	{
		CHousingObj * m = (CHousingObj *)models->GetNext(pos);
		CString name = HouseWords(m->m_houseName);
		int score = 0, start = 0;
		CString word;
		while(!(word = name.Tokenize(" ", start)).IsEmpty())
		{
			if(word == "story" || word == "level") continue;
			if(item.Find(" " + word + " ") >= 0) score += 2;
		}
		CString itemWord; start = 0;
		while(!(itemWord = item.Tokenize(" ", start)).IsEmpty())
			if(name.Find(" " + itemWord + " ") < 0) score -= 1;   // extra words in the item name count against
		if(score > bestScore) { bestScore = score; best = index; }
	}
	return best >= 0 ? best : fallback;
}
BOOL CAttributeSystems::AtribUseItem_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
	      m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Lock();
	      CPlayerObject * currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
          if(!currentPlayer)
		  {
              m_sharedInterfaceRef->m_eventLog->AddEvent("Cannot find player that is useing item");
			  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
              return FALSE;
		  }

		  if(!currentPlayer->AllowAsyncQueMini())
		  {
		     m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			 m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			 return FALSE;//throttle
		  }

		  if(!currentPlayer->m_currentInventory)
		  {
			  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			  return FALSE;
		  }

		  
		  if(!currentPlayer->m_currentInventory->VerifyInventoryDUPH(formatedMessage->uniInt,formatedMessage->uniInt2))
		  {
              m_sharedInterfaceRef->SendGeneralMessageHandler(38,currentPlayer);
			  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
              return FALSE;
		  }
     
          CItemObj * invObj = m_sharedInterfaceRef->m_itemManager->GetItemByID_LOS(formatedMessage->uniInt);
		  if(!invObj)
		  {
			  m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid GLID request dpnid:",(DWORD)idFromLoc);
			  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
              return FALSE;
		  }
		  //verify item is in inventory
          
          //verify usable
		  if(invObj->m_useType == 0)
		  {
		     m_sharedInterfaceRef->m_itemManager->Unlock();
			 m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			 m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			 return FALSE;
		  }


		  long currentTime = GetTickCount();
          CSkillObject * sklRefPtr = NULL;
		  CSkillObject *sklPlayPtr = NULL;
		  CInventoryObj * weaponLink = NULL;
		  POSITION posLoc = NULL; 
		  CMovementObj * entObjDB = NULL;

		  CAccountObject * accountPtr = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountByID(idFromLoc);
		  if(!accountPtr)
		  {
			  m_sharedInterfaceRef->m_itemManager->Unlock();
			  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
              m_sharedInterfaceRef->m_eventLog->LogByPriority("AtribUseItem_safe() cannot find account:",(DWORD)idFromLoc,2);
              return FALSE;
		  }

		  //make visible
		  if(currentPlayer->m_stasis == 1)
		  {//if you use item when hiding unhide
			if((currentTime - currentPlayer->m_lastUseItemStamp) < 10000)
			{
				m_sharedInterfaceRef->SendGeneralMessageHandler(1,currentPlayer);//inform of problem with use
				m_sharedInterfaceRef->m_itemManager->Unlock();
			    m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			    m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
				return FALSE;
			}
			m_sharedInterfaceRef->Unhide(currentPlayer);
		  }//end if you use item when hiding unhide

		  int resultDyn = 0;
		  int i = 0;

          switch(invObj->m_useType)
		  {//use actions
             

		     case 0://NONE
		  
		     break;
             case 1://HEALING
	  
			 if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(currentPlayer->m_stencilChannel,currentPlayer->m_currentWorldID,WORLDRULE_NOHEAL))//filter healing in this world
			 {
				m_sharedInterfaceRef->SendGeneralMessageHandler(89,currentPlayer);//inform of problem with use
				break;
			 }
				 
			 if((currentTime - currentPlayer->m_lastUseItemStamp) < 10000)
		     {
				m_sharedInterfaceRef->SendGeneralMessageHandler(1,currentPlayer);//inform of problem with use
				break;
			 }
			 currentPlayer->m_lastUseItemStamp = currentTime;

			 if(!currentPlayer->m_currentInventory->DeleteByGLID_DPTRACE(formatedMessage->uniInt,formatedMessage->uniInt2))
			 {//FAILED
				m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid Player Inventory",(DWORD)idFromLoc);
				break;
			 }//END FAILED

		     if(currentPlayer->m_energy > 0)
			 {
		        currentPlayer->m_energy += invObj->m_miscUseValue;
				//(unlock moved: the end of this function unlocks m_itemManager - unlocking here too unlocked it twice)
			    if(currentPlayer->m_energy > currentPlayer->m_maxEnergy)
				   currentPlayer->m_energy = currentPlayer->m_maxEnergy;
			       //send update players health
			       m_sharedInterfaceRef->UpdateClientStatistics(currentPlayer,idFromLoc);
                   m_sharedInterfaceRef->SendAttribMsg( DPNSEND_GUARANTEED,
														currentPlayer->m_currentID,
														3,
														currentPlayer->m_energy,
														currentPlayer->m_networkAssignedID,
														0,
														currentPlayer->m_maxEnergy,
														currentPlayer);

				   

                if(currentPlayer->m_energy < 1)//killed self with poison
				    m_sharedInterfaceRef->HandleDeath(currentPlayer,currentPlayer);
			 }

			 // Hard Coded Effect for now
			 m_sharedInterfaceRef->SendDetailedMsgToClient(6,
														   153,
														   currentPlayer->m_networkAssignedID,
														   0,
														   currentPlayer);
			 break;

			 case 2://AMMO

                if(!currentPlayer->m_currentInventory->DeleteByGLID_DPTRACE(formatedMessage->uniInt,formatedMessage->uniInt2))
				{//FAILED
                  m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid Player Inventory",(DWORD)idFromLoc);
                  break;
				}//END FAILED

			    weaponLink = currentPlayer->m_currentInventory->GetArmedInvenItemByLocationID(12);
	            if(weaponLink)
				{//aquire weapon used
                   if(weaponLink->m_itemData->m_chargeType == invObj->m_chargeType)
				   {//compatibility
						weaponLink->m_itemData->m_chargeCurrent += invObj->m_maxCharges;
						if(weaponLink->m_itemData->m_chargeCurrent > weaponLink->m_itemData->m_maxCharges)
							weaponLink->m_itemData->m_chargeCurrent = weaponLink->m_itemData->m_maxCharges;

						//Reset the depletion so it can be used again
						weaponLink->m_itemData->m_tempDepletionHolder = 0;
						weaponLink->m_itemData->m_weaponDepletionTimer = 0;
						weaponLink->m_itemData->m_secsToGo = 0;
				   
				      m_sharedInterfaceRef->SendAttribMsg(  DPNSEND_GUARANTEED,
															currentPlayer->m_currentID,
															27,//attribute
															weaponLink->m_itemData->m_maxCharges,//
															0,
															weaponLink->m_itemData->m_chargeCurrent,
															weaponLink->m_itemData->m_shotsPerFire,
															currentPlayer);
					  currentPlayer->m_rechargeTimer = 0;
					  m_sharedInterfaceRef->UpdateClientStatistics(currentPlayer, idFromLoc);
/*					  m_sharedInterfaceRef->SendAttribMsg(	DPNSEND_GUARANTEED,
															currentPlayer->m_currentID,
															41,
															0,
															0,
															0,
															0,
															currentPlayer);
*/				   }//end compatibility
				   else
                      m_sharedInterfaceRef->SendGeneralMessageHandler(9,currentPlayer);
				}//end aquire weapon used
		     break; 

			 case 3://use skill
				sklRefPtr = m_sharedInterfaceRef->m_skillDatabase->GetSkillByType(invObj->m_requiredSkillType);
                if(sklRefPtr)
				{//known skill
				   m_sharedInterfaceRef->SendSkillAbilities(DPNSEND_GUARANTEED,
															currentPlayer->m_currentID,
															sklRefPtr,
															invObj->m_requiredSkillLevel,
															invObj->m_globalID);

				}//end known skill
				else//uknown skill attempted to use
				{
					m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid skill use",":",invObj->m_requiredSkillType);
					CString ignoreMsg = "Invalid Skill Use.";
					m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,idFromLoc,ignoreMsg, 0,
													  currentPlayer,0);

				}
			 break;

			 case 4://request to call menu 
				 m_sharedInterfaceRef->SendAttribMsg(DPNSEND_GUARANTEED,idFromLoc,24,0,0,invObj->m_miscUseValue,-9,currentPlayer);//open menu call to initiator

			 break;


			 case 5://request to combine if possible 
                 if(currentPlayer->m_currentInventory)
				 {
				    resultDyn = currentPlayer->m_currentInventory->CombineToExpObj(formatedMessage->uniInt,invObj->m_miscUseValue);
                    if(resultDyn != 1)
					{//failed for specific
                       if(resultDyn == 8)//already exists error-inform client
						  m_sharedInterfaceRef->SendGeneralMessageHandler(35,currentPlayer);
					   if(resultDyn == 2)//target item does not exist-inform client
                          m_sharedInterfaceRef->SendGeneralMessageHandler(36,currentPlayer);
					   if(resultDyn == 5)//error no exp data on item tagged with this use
                          m_sharedInterfaceRef->m_eventLog->AddEvent("error no exp data on item tagged with this EXP use");
					   if(resultDyn == 0)//could not find item
						  m_sharedInterfaceRef->m_eventLog->AddEvent("could not find item-EXP object use");
					}//end failed for specific
					else
					  m_sharedInterfaceRef->SendGeneralMessageHandler(37,currentPlayer);
				 }
			
			 break;

			 case 7://the item data marks some houses as "house installation" - treat them as houses too
			 case 6://place a house here (item misc use value = house model index)
			 {
				 CHousingZoneDB * zones = m_sharedInterfaceRef->m_netComObject->m_houseingDB;
				 int houseType = PickHouseModel(m_sharedInterfaceRef->m_housingDB,invObj->m_itemName,invObj->m_miscUseValue);
				 CHousingObj * model = m_sharedInterfaceRef->m_housingDB ? m_sharedInterfaceRef->m_housingDB->GetByIndex(houseType) : NULL;
				 int acBind = currentPlayer->m_accountNumberRef;
				 CString houseMsg;
				 if(!zones || !model)
				    houseMsg = "Houses can't be placed right now.";
				 else if(zones->GetHouseByAccountBind(acBind))
				    houseMsg = "You already own a house.";
				 else
				 {
				    BNDBOX box;
				    box.min.x = model->m_clipBox.minX; box.min.y = model->m_clipBox.minY; box.min.z = model->m_clipBox.minZ;
				    box.max.x = model->m_clipBox.maxX; box.max.y = model->m_clipBox.maxY; box.max.z = model->m_clipBox.maxZ;
				    D3DVECTOR where = currentPlayer->m_3dPosition;
				    int newID = 0;
				    int placed = zones->AddPlacementObjToTable(houseType,acBind,box,where,&newID,currentPlayer->m_stencilChannel);
				    if(placed == 5)
				       houseMsg = "Too close to another house. Move further away and try again.";
				    else if(placed != 1)
				       houseMsg = "You can't build here. Houses can only go in a housing area.";
				    else
				    {
				       currentPlayer->m_currentInventory->DeleteByGLID_DPTRACE(formatedMessage->uniInt,formatedMessage->uniInt2);
				       HOUSE_SAVE_REC rec;
				       rec.accountBind = acBind; rec.worldChannel = currentPlayer->m_stencilChannel; rec.type = houseType;
				       rec.x = where.x; rec.y = where.y; rec.z = where.z;
				       m_sharedInterfaceRef->m_threadedDataProcessor->AddToQue(0,"",0,0,11,(byte*)&rec,sizeof(rec));//save to GM_SERVERPLACEMENTS
				       m_sharedInterfaceRef->m_eventLog->AddEvent("House placed by account:",(DWORD)acBind);
				       houseMsg = "Your house has been built here!";
				    }
				 }
				 m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,currentPlayer->m_currentID,houseMsg,currentPlayer->m_networkAssignedID,currentPlayer,0);
			 }
			 break;
			 case 8://summon creature
                       m_sharedInterfaceRef->SpawnDynamicAIToAvailAiServer( invObj->m_miscUseValue,
																			currentPlayer->m_currentWorldID, 
																			currentPlayer->m_Target3dPosition,
																			-1,
																			0,
																			-1,
																			0,
																			0,
																			0,
																			0,
																			currentPlayer->m_currentWorldMap);

					   if(invObj->m_destroyOnUse)
                          currentPlayer->m_currentInventory->DeleteByGLID_DPTRACE(formatedMessage->uniInt,formatedMessage->uniInt2);

			 break;

			 case 9://Powerups				
					 
				 if((currentTime - currentPlayer->m_lastUseItemStamp) < 10000)
				 {
				  m_sharedInterfaceRef->SendGeneralMessageHandler(1,currentPlayer);//inform of problem with use
	            
				  break;
				 }
				 
				 // Set timer for inbetween item usage
				 currentPlayer->m_lastUseItemStamp = currentTime;

				 if(!currentPlayer->m_currentInventory->DeleteByGLID_DPTRACE(formatedMessage->uniInt,formatedMessage->uniInt2))
				 {//FAILED
				  m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid Player Inventory",(DWORD)idFromLoc);
				  break;
				 }//END FAILED

				 // Set it up if its a stat buff
				 if(invObj->m_bonusType > 0 && invObj->m_bonusType < 5)
					 i = 1;
				 
				 // Set it up if its a resist buff
				 else if(invObj->m_bonusType > 5)
					 i = 2;
				 
				 if(currentPlayer->m_bonus[i].m_bonusType != 0)
					 m_sharedInterfaceRef->SendGeneralMessageHandler(91, currentPlayer);
				 
				 // Check for the powerup Type
				 currentPlayer->m_bonus[i].m_bonusType = invObj->m_bonusType;

				 // Check for the duration
				 currentPlayer->m_bonus[i].m_bonusDuration = invObj->m_bonusDuration;
				 currentPlayer->m_bonus[i].m_maxDuration = invObj->m_bonusDuration;

				 // Check for the Damage Amount
				 currentPlayer->m_bonus[i].m_damageMult = invObj->m_damageMultiplier;

				 // Check for the Bonus Amount
				 currentPlayer->m_bonus[i].m_bonusAmount = invObj->m_miscUseValue;
				 
				 if(i == 1)
				 {
					 // Send the Update to Client Side for Visual Display
					 m_sharedInterfaceRef->SendAttribMsg(  DPNSEND_GUARANTEED,
														   currentPlayer->m_currentID,	// Player ID
													       31,							// Attribute (Stat)
														   invObj->m_bonusDuration,		// Current Timer
														   invObj->m_bonusType - 1,		// Visual Display
														   invObj->m_bonusDuration,		// Max Timer
														   14,							// Menu to Use
														   currentPlayer);				// Player
					currentPlayer->m_StatStamp = GetTickCount();
				 }

				 else if(i == 2)
				 {
					 // Send the Update to Client Side for Visual Display
					 m_sharedInterfaceRef->SendAttribMsg(  DPNSEND_GUARANTEED,
														   currentPlayer->m_currentID,	// Player ID
													       32,							// Attribute (Resist)
														   invObj->m_bonusDuration,		// Current Timer
														   invObj->m_bonusType - 6,		// Visual Display
														   invObj->m_bonusDuration,		// Max Timer
														   15,							// Menu to Use
														   currentPlayer);				// Player
					 currentPlayer->m_ResistStamp = GetTickCount();
				 }

				 else
				 {
					 // Send the Update to Client Side for Visual Display
					 m_sharedInterfaceRef->SendAttribMsg(  DPNSEND_GUARANTEED,						
													       currentPlayer->m_currentID,	// Player ID			
													       30,							// Attribute (Damage)
													       invObj->m_bonusDuration,		// Current Timer
													       0,							// Visual Display
													       invObj->m_bonusDuration,		// Max Timer
													       0,							// Menu to Use
													       currentPlayer);				// Player
				 }

				 // Update the statistics
				 m_sharedInterfaceRef->UpdateClientStatistics(currentPlayer,idFromLoc);

			 break;

			 case 10: // Misc Items

				 if((currentTime - currentPlayer->m_lastUseItemStamp) < 10000)
				 {
					  m_sharedInterfaceRef->SendGeneralMessageHandler(1,currentPlayer);//inform of problem with use   
					  break;
				 }
				 
				 // Set timer for inbetween item usage
				 currentPlayer->m_lastUseItemStamp = currentTime;

				 if(!currentPlayer->m_currentInventory->DeleteByGLID_DPTRACE(formatedMessage->uniInt,formatedMessage->uniInt2))
				 {//FAILED
				  m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid Player Inventory",(DWORD)idFromLoc);
				  break;
				 }//END FAILED

				 //Check for use skill item
				 if(invObj->m_requiredSkillType)
				 {
					 //Retrieve skill references
					 sklRefPtr = m_sharedInterfaceRef->m_skillDatabase->GetSkillByType(invObj->m_requiredSkillType);
					 sklPlayPtr = currentPlayer->m_skills->GetSkillByType(invObj->m_requiredSkillType);
					 
					 //Check to see if the player has the skill already
					 if(sklPlayPtr)
					 {
						 ATTRIB_STRUCT attribLocal;
						 ZeroMemory(&attribLocal,sizeof(ATTRIB_STRUCT));
						 int level = 1;
						 sklPlayPtr->EvaluateLevel(level, sklRefPtr);
						 attribLocal.uniShort3 = invObj->m_requiredSkillType;
						 attribLocal.uniInt = invObj->m_miscUseValue;
						 attribLocal.uniInt2 = level;

						 //AtribSkillAbilityUse(&attribLocal, idFromLoc);
						 AtribSkillAbilityUse(&attribLocal,idFromLoc,currentPlayer,accountPtr);
					 }
					 //Otherwise we give them the skill from default in database
					 else if(sklRefPtr)
					 {
						 ATTRIB_STRUCT attribLocal;
						 ZeroMemory(&attribLocal,sizeof(ATTRIB_STRUCT));
						 attribLocal.uniShort3 = invObj->m_requiredSkillType;
						 attribLocal.uniInt = invObj->m_miscUseValue;
						 attribLocal.uniInt2 = sklRefPtr->m_currentLevel;

						 //AtribSkillAbilityUse(&attribLocal, idFromLoc);
						 AtribSkillAbilityUse(&attribLocal,idFromLoc,currentPlayer,accountPtr);
					 }
				 }

				 // Make sure its the right item type
				 else if(invObj->m_visualRef == 150)
				 {
					 if(invObj->m_miscUseValue == -1)
						 currentPlayer->m_noteriety->m_currentMurders = 0;

					 else 
					 {
						 currentPlayer->m_noteriety->m_currentMurders -= invObj->m_miscUseValue;
						 if(currentPlayer->m_noteriety->m_currentMurders < 0)
							currentPlayer->m_noteriety->m_currentMurders = 0;
					 }

					 currentPlayer->m_noteriety->m_murderUpdateTick = 0;
				 }

				 // Increase the players health and energy
				 else if(invObj->m_visualRef == 202)
				 {
					 //send update players energy
					 m_sharedInterfaceRef->SendAttribMsg(  DPNSEND_GUARANTEED,						
													       currentPlayer->m_currentID,		// Player ID		
													       35,								// Attribute
														   invObj->m_miscUseValue * -1,		// multiplied by neg 1 to add the value					
													       0,							
													       0,							
													       0,							
													       currentPlayer);					// Player

					 currentPlayer->m_energy += invObj->m_miscUseValue;
					 if(currentPlayer->m_energy > currentPlayer->m_maxEnergy)
						currentPlayer->m_energy = currentPlayer->m_maxEnergy;
			         
					 //send update players health
			         m_sharedInterfaceRef->UpdateClientStatistics(currentPlayer,idFromLoc);
                     m_sharedInterfaceRef->SendAttribMsg( DPNSEND_GUARANTEED,
														currentPlayer->m_currentID,
														3,
														currentPlayer->m_energy,
														currentPlayer->m_networkAssignedID,
														0,
														currentPlayer->m_maxEnergy,
														currentPlayer);
				 }

				 // Energy Update Only
				 else if(invObj->m_visualRef == 208)
				 {
					 //send update players energy
					 m_sharedInterfaceRef->SendAttribMsg(  DPNSEND_GUARANTEED,						
													       currentPlayer->m_currentID,				
													       35,								// Attribute
														   invObj->m_miscUseValue * -1,		// multiplied by neg 1 to add the value					
													       0,							
													       0,							
													       0,							
													       currentPlayer);					// Player
				 }

			 break;
			
			case 11:
				//Faction Item, should do nothing if it ever reaches here
			 break;

			 case 12:

				 //Retrieve skill references
				 sklRefPtr = m_sharedInterfaceRef->m_skillDatabase->GetSkillByType(invObj->m_requiredSkillType);
				 sklPlayPtr = currentPlayer->m_skills->GetSkillByType(invObj->m_requiredSkillType);
				 
				 //Retrieve the current weapon we have equiped
				// weaponLink = currentPlayer->m_currentInventory->GetArmedInvenItemByLocationID(12);
				 BOOL canUseSkill = TRUE;
				 
				 //Check to make sure the weapon can use the skill we are using
		/*		 if(weaponLink->m_itemData->m_requiredSkillLevel >= invObj->m_requiredSkillLevel)
				 {
					 //Check to make sure we have enough ammo (if applicable) in order to use the skill
					 if(weaponLink->m_itemData->m_chargeCurrent >= invObj->m_chargeCurrent)
					 {
						 canUseSkill = TRUE;
						 //Only send an update on ammo if we have charges to subtract
						 if(invObj->m_chargeCurrent)
						 {
							 weaponLink->m_itemData->m_chargeCurrent -= invObj->m_chargeCurrent;
							 m_sharedInterfaceRef->SendAttribMsg( DPNSEND_GUARANTEED,
																  currentPlayer->m_currentID,
																  27,//attribute
																  weaponLink->m_itemData->m_maxCharges,//
																  0,
																  weaponLink->m_itemData->m_chargeCurrent,
																  0,
																  currentPlayer);
						 }//End Ammo subtraction
					 }//End Ammo Check
				 }//End usability Check
*/
				 //If we can use the skill
				 if(canUseSkill)
				 {
					 //Check to see if the player has the skill already
					 if(sklPlayPtr)
					 {
						 ATTRIB_STRUCT attribLocal;
						 ZeroMemory(&attribLocal,sizeof(ATTRIB_STRUCT));
						 attribLocal.uniShort3 = invObj->m_requiredSkillType;
						 attribLocal.uniInt = invObj->m_miscUseValue;
						 attribLocal.uniInt2 = invObj->m_requiredSkillLevel;

						 //AtribSkillAbilityUse(&attribLocal, idFromLoc);
						 AtribSkillAbilityUse(&attribLocal,idFromLoc,currentPlayer,accountPtr);
					 }
					 //Otherwise we give them the skill from default in database
					 else if(sklRefPtr)
					 {
						 ATTRIB_STRUCT attribLocal;
						 ZeroMemory(&attribLocal,sizeof(ATTRIB_STRUCT));
						 attribLocal.uniShort3 = invObj->m_requiredSkillType;
						 attribLocal.uniInt = invObj->m_miscUseValue;
						 attribLocal.uniInt2 = invObj->m_requiredSkillLevel;

						 //AtribSkillAbilityUse(&attribLocal, idFromLoc);
						 AtribSkillAbilityUse(&attribLocal,idFromLoc,currentPlayer,accountPtr);
					 }
					 else//uknown skill attempted to use
					 {
						m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid skill use",":",invObj->m_requiredSkillType);
						CString ignoreMsg = "Invalid Skill Use.";
						m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,idFromLoc,ignoreMsg, 0,
														  currentPlayer,0);

					 }
				 }

				 //Send the animation override so we are playing the correct skill visual animation
	/*			 if(invObj->m_requiredSkillType != 13 && canUseSkill)
					m_sharedInterfaceRef->SendAttribMsg(  DPNSEND_GUARANTEED,						
														  currentPlayer->m_currentID,	// Player ID			
														  36,							// Attribute
													      invObj->m_animAccessValue,	// 3rd Person Override
													      0,							// 
														  invObj->m_reloadTime,			// 1st Person Override
													      0,							// 
													      currentPlayer);				// Player
*/
			 break;

		  }//end use actions

    
	m_sharedInterfaceRef->m_itemManager->Unlock();
	m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
		 
	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribCancelTransaction_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
    //redesign
	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribAcceptTradeTerms_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
     //redesign
     return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribDestroyItemByGlid_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
          CPlayerObject * currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
		  if(!currentPlayer)
		  {
             m_sharedInterfaceRef->m_eventLog->AddEvent("Cannot find current player to destroy item ");
             return FALSE;
		  }

		  if(currentPlayer->m_stasis == 2)
          {// not while shifted
              m_sharedInterfaceRef->SendGeneralMessageHandler(54,currentPlayer);
			  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
              return FALSE;
          }// end not while shifted

		  if(currentPlayer->m_energy < 1)
		  {
		     m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		     return FALSE;
		  }
	
		  POSITION posLast;
		  int dupTrace = 0;
		  if(currentPlayer->m_currentInventory)
		   for(POSITION posLoc = currentPlayer->m_currentInventory->GetHeadPosition();(posLast = posLoc) != NULL;)
		   {//
                 CInventoryObj * plObj = (CInventoryObj *)currentPlayer->m_currentInventory->GetNext(posLoc);
				 if(formatedMessage->uniInt == plObj->m_itemData->m_globalID)
				 {
                    if(formatedMessage->uniInt2 != dupTrace)
					{
					   dupTrace++;
					   continue;
					}

                    if(plObj->m_armed)
				       break;//cannot delete armed item

                    plObj->SafeDelete();
					delete plObj;
					plObj = 0;
					currentPlayer->m_currentInventory->RemoveAt(posLast);
		            break;     
				 }
		   }
		   int totalAssets = m_sharedInterfaceRef->m_currencySystem->EvalCurrency(currentPlayer->m_currentInventory);
		   m_sharedInterfaceRef->SendAttribMsg( DPNSEND_GUARANTEED,
											currentPlayer->m_currentID,
											19,
											totalAssets,
											0,
											0,
											0,
											currentPlayer);
		   m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		   return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribQuestHandling_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
		  CAccountObject * accountPtr = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountByID_LOS(idFromLoc);
		  if(!accountPtr)
		  {
              m_sharedInterfaceRef->m_eventLog->LogByPriority("AtribQuestHandling_safe() cannot find account:",(DWORD)idFromLoc,2);
              return FALSE;
		  }
		  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Lock();
		  CPlayerObject * currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByDPNID_Alive(idFromLoc);//can only use on pc
          if(!currentPlayer)
		  {
			  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			  m_sharedInterfaceRef->m_eventLog->LogByPriority("AtribQuestHandling_safe() cannot aquire current player dpnid:",(DWORD)idFromLoc,2);
              return FALSE;
		  }

		  //aquire near quest bot asap
		  CServerItemObj * svrPtr = m_sharedInterfaceRef->m_netComObject->m_runtimeItems->GetItemInRadius_LOSchannel(currentPlayer->m_Target3dPosition,(currentPlayer->m_dynamicRadius*2.0f),currentPlayer->m_currentWorldID);
		  if(!svrPtr)
		  {
			  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
              m_sharedInterfaceRef->m_eventLog->LogByPriority("AtribQuestHandling_safe() cannot find net ID of AI:",(DWORD)formatedMessage->uniInt2,2);
              return FALSE;
		  }

          if(!svrPtr->m_collectionQuestDB || formatedMessage->uniInt < 0)
          {
              m_sharedInterfaceRef->m_eventLog->AddEvent("No valid quests on AI");
		  }
		  else
		  {
			  int showSelectionCurrent = 0;
			  CAICollectionQuestObj * questPtr = svrPtr->m_collectionQuestDB->GetByAvailIndex(formatedMessage->uniInt,currentPlayer->m_curEDBIndex,&showSelectionCurrent);
			  if(questPtr) 
			  {
				 m_questSystemsRef->HandleQuestSys(currentPlayer,accountPtr,questPtr,svrPtr,idFromLoc,showSelectionCurrent);
			  }
		  }
		  m_sharedInterfaceRef->m_netComObject->m_runtimeItems->Unlock();
		  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();

		  return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribSkillAbilityUse_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
          CAccountObject * accountPtr = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountByID_LOS(idFromLoc);
          if(!accountPtr){//error locating account
              m_sharedInterfaceRef->m_eventLog->AddEvent("cannot locate account inside skill ability use");
              return FALSE;
		  }//end error locating account
	
	
	      CPlayerObject * playerObjectData = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByDPNID_Alive_LOS(idFromLoc);
          if(!playerObjectData)
		  {
			  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
              m_sharedInterfaceRef->m_eventLog->AddEvent("cannot aquire runtime player inside skill ability use");
              return FALSE;
		  }
		  
		  BOOL res = AtribSkillAbilityUse(formatedMessage,idFromLoc,playerObjectData,accountPtr);

		  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();

		  return res;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribSkillAbilityUse(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc,CPlayerObject * playerObjectData,CAccountObject * accountPtr)
{
		  if(playerObjectData->m_aiControlled)
		  {
              m_sharedInterfaceRef->m_eventLog->AddEvent("AI cannot use skills 911");
              return FALSE;
		  }
   
		  if(!accountPtr->m_characterOnAccount)
		  {//bad things
              m_sharedInterfaceRef->m_eventLog->AddEvent("No characters created yet 01");
	          return FALSE;
		  }//end bad things
 
		  //m_sharedInterfaceRef->m_eventLog->LogByPriority(playerObjectData->m_charName+" Used Skill",(DWORD)formatedMessage->uniShort3,2);

		  CPlayerObject * characterOnAccount = accountPtr->m_characterOnAccount;
		
		  if(playerObjectData->m_stasis == 1)
		  {//skill already in use
		     m_sharedInterfaceRef->SendGeneralMessageHandler(24,playerObjectData);
             return FALSE;
		  }//end skill already in use

		  if(formatedMessage->uniShort3 != 4)
          if(playerObjectData->m_stasis != 0)
		  {//skill already in use
		     m_sharedInterfaceRef->SendGeneralMessageHandler(24,playerObjectData);
             return FALSE;
		  }//end skill already in use
      
		  long curTime = GetTickCount();

          //Vunerable

		  if(!playerObjectData->m_skills)
              playerObjectData->m_skills = new CSkillObjectList();

			 int specialInstruction = 0;
			 int miscInt = 0;
             BOOL updateStats = FALSE;//gl area
			 CSkillObject * newAddedSkillRetPtr = NULL;
			 CLevelRangeObject * levelUpObj = NULL;
			 CSkillObject * skillPtrReference = NULL;
			 int effectBind = 0;


             if(formatedMessage->uniShort3 == 12) // only for magic-book spells, not new magic
			 {//magic limitation hardcode
			    CInventoryObj * weaponLink = playerObjectData->m_currentInventory->GetArmedInvenItemByLocationID(12);
				if(weaponLink){
			     if(weaponLink->m_itemData)
				  if(!weaponLink->m_itemData->m_permanent)
				  {
				   m_sharedInterfaceRef->SendGeneralMessageHandler(78,playerObjectData);
                   return FALSE;
				  }
				}
			 }
			 //end magic limitation

			 //19 level doesnt not exist error
			 //2 invalid level editor
			 //3 exp out of range error
			 //14 stat requirement not met
			 //8 does not have required inventory
			 //10 unsuccessful throw
			 //1 success
			 int result = playerObjectData->m_skills->UseSkillByType( formatedMessage->uniShort3,//int type,
									                                  m_sharedInterfaceRef->m_skillDatabase,
									                                  0,//expOfTarget,
						                                              (formatedMessage->uniInt2-1),//skill level used
						                                              formatedMessage->uniInt,//ability index
						                                              playerObjectData->m_curEDBIndex,
						                                              playerObjectData->m_currentInventory,
						                                              &specialInstruction,
									                                  &newAddedSkillRetPtr,
									                                  playerObjectData->m_stats,
						                                              &updateStats,
									                                  &levelUpObj,
														              &skillPtrReference,
																	  &miscInt,
																	  m_sharedInterfaceRef->m_netComObject->m_maxItemLimit_onChar,
																	  &effectBind,
																	  m_sharedInterfaceRef->m_netComObject->m_manditoryWaitBeforeSkillUse,
																	  playerObjectData->m_skillUseTimeStamp);
			 
/*			 if(formatedMessage->uniShort3 == 3)
			 {
				 result = 1;
				 specialInstruction = 3;
			 }
*/			 if(result == 300)
			 {//too soon  to use this skill
                m_sharedInterfaceRef->SendGeneralMessageHandler(26,playerObjectData);
                return FALSE;
			 }//end too soon to use this skill

			 if(result == 301)
			 {//too soon  to use this skill
                m_sharedInterfaceRef->SendGeneralMessageHandler(88,playerObjectData);
                return FALSE;
			 }//end too soon to use this skill

			 playerObjectData->m_skillUseTimeStamp = curTime;

			 if(result == 1)
			 {//success
				                
				if(effectBind != -1)
				   m_sharedInterfaceRef->BroadcastEffect(effectBind,
								                         playerObjectData->m_networkAssignedID,
								                         playerObjectData);
             
                if(specialInstruction != 3)
					m_sharedInterfaceRef->SendGeneralMessageHandler(21,playerObjectData);

				if(specialInstruction == 8)
				{//heal self
                   if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(playerObjectData->m_stencilChannel,playerObjectData->m_currentWorldID,WORLDRULE_NOHEAL))
				   {//filter healing in this world
				      m_sharedInterfaceRef->SendGeneralMessageHandler(89,playerObjectData);//inform of problem with use
		              return FALSE;
				   }

				   playerObjectData->m_energy += miscInt;
			       if(playerObjectData->m_energy > playerObjectData->m_maxEnergy)
				      playerObjectData->m_energy = playerObjectData->m_maxEnergy;
						
				   if(playerObjectData->m_currentID > 0)
			          m_sharedInterfaceRef->UpdateClientStatistics(playerObjectData,playerObjectData->m_currentID);

				}//end heal self
				if(specialInstruction == 7)
				{//emit projectile
				      m_sharedInterfaceRef->SendDetailedMsgToClient(7,
																	miscInt,
																	0,
																	0,
																	playerObjectData);
					  playerObjectData->m_weaponPass = miscInt;
				}//end emit projectile
				if(specialInstruction == 6)
				{//damage-Heal radius
				   CExplosionObj* expPtr = m_sharedInterfaceRef->m_explosionDBList->GetPtr(effectBind);
		           if(expPtr){
                      
					  if(expPtr->m_radiusDamage < 0)
				      if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(playerObjectData->m_stencilChannel,playerObjectData->m_currentWorldID,WORLDRULE_NOHEAL))//filter healing in this world
					  {
						  m_sharedInterfaceRef->SendGeneralMessageHandler(89,playerObjectData);//inform of problem with use
						  return FALSE;
					  }


				      m_sharedInterfaceRef->DoRadiusDamage_Heal(expPtr->m_radiusDamage,
																expPtr->m_damageChannel,
																expPtr->m_damageRadius,
																playerObjectData);
				   }
				}//end damage-Heal radius
                if(specialInstruction == 5)
				{//action
				
				   m_sharedInterfaceRef->RevealRadius(200,playerObjectData);

				}//end action
				if(specialInstruction == 4)
				{//mount action
				   //miscInt
					if(playerObjectData->m_stasis == 2)
                    {// not while shifted
                     m_sharedInterfaceRef->SendGeneralMessageHandler(54,playerObjectData);
					}//
					else
					{// 
					 if(miscInt > -1)
					 {
					   m_sharedInterfaceRef->HandleMountRequest(playerObjectData,miscInt,0);
					 }
					}// end not while shifted
				}//end mount action
				if(specialInstruction == 3)
				{//hiding skill
                   if(m_sharedInterfaceRef->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(playerObjectData->m_stencilChannel,playerObjectData->m_currentWorldID,WORLDRULE_NOHIDE))
				   {//filter hide in this world
				      m_sharedInterfaceRef->SendGeneralMessageHandler(89,playerObjectData);//inform of problem with use
		              return FALSE;
				   }
                   if(playerObjectData->m_aiCfgOfMounted != -1){
		              m_sharedInterfaceRef->SendGeneralMessageHandler(60,playerObjectData);
				   }
				   else if(playerObjectData->m_stasis == 0)
				   {
                      playerObjectData->m_stasis = 1;//hide
                      playerObjectData->m_positionLock.x      = playerObjectData->m_Target3dPosition.x;
                      playerObjectData->m_positionLock.y      = playerObjectData->m_Target3dPosition.y;
                      playerObjectData->m_positionLock.z      = playerObjectData->m_Target3dPosition.z;

					  m_sharedInterfaceRef->UpdateClientStatistics(playerObjectData,idFromLoc);
					  m_sharedInterfaceRef->SendGeneralMessageHandler(98,playerObjectData);

					  m_sharedInterfaceRef->SendDetailedMsgToClient(6,
																	55,
																	playerObjectData->m_networkAssignedID,
																	0,
																	playerObjectData);

					  updateStats = FALSE;
				   }
				}//end hiding skill
				else if(specialInstruction == 2){//shapeshifting
				   if(playerObjectData->m_aiCfgOfMounted == -1)//cannot shape shift while mounted
				   if(playerObjectData->m_stasis != 1)//cannot shift while hidden
                   if(playerObjectData->m_stasis == 0 || miscInt == -1)
				   {//not in any other statis			   
					  //Verify ai bot cfg
                      CAIObj * aiReferencePtr = m_sharedInterfaceRef->m_aiList->GetByIndex(miscInt);
					  bool updateNeeded = false;
					  if(aiReferencePtr != NULL || miscInt == -1)
					  {
						 if(miscInt > -1)
						 {//shifting into
							playerObjectData->m_stasis = 2;//shifted
							playerObjectData->m_altCfgIndex = miscInt;
							playerObjectData->m_curEDBIndex = aiReferencePtr->m_entityCfgDBindexRef;
							characterOnAccount->m_stasis = 2;//shifted
							characterOnAccount->m_altCfgIndex = miscInt;
							characterOnAccount->m_curEDBIndex = aiReferencePtr->m_entityCfgDBindexRef;
							updateNeeded = true;
						 }//end shifting into
						 else{//shifting out of
                           if(playerObjectData->m_stasis == 2)
						   {//must be shifted
							  //delete armed items cause morphed (perminent only)
							  POSITION iPosLast = NULL;
							  if(playerObjectData->m_currentInventory)
					          for(POSITION iPos = playerObjectData->m_currentInventory->GetHeadPosition();(iPosLast=iPos) != NULL;)
							  {
					              CInventoryObj * invPtr = (CInventoryObj *) playerObjectData->m_currentInventory->GetNext(iPos);
						          if(invPtr->m_armed == TRUE && invPtr->m_itemData->m_permanent == TRUE)
								  {
								     CInventoryObj * testItIPtrTemp = NULL;
								     if(playerObjectData->m_reconfigInven)
									    testItIPtrTemp = playerObjectData->m_reconfigInven->VerifyInventory(invPtr->m_itemData->m_globalID);
								     if(!testItIPtrTemp)
									 {
									     invPtr->SafeDelete();
									     delete invPtr;
									     invPtr = 0;
							             playerObjectData->m_currentInventory->RemoveAt(iPosLast); 
									 }
								  }
							  }
							  //then apply reconfig inven
							  if(playerObjectData->m_currentInventory)
							  if(playerObjectData->m_reconfigInven)
							  {
                                for(POSITION iPos = playerObjectData->m_reconfigInven->GetHeadPosition();iPos != NULL;)
								{
					              CInventoryObj * invPtr = (CInventoryObj *) playerObjectData->m_reconfigInven->GetNext(iPos);
						          if(invPtr->m_armed == TRUE)
								  {
									  CInventoryObj * querryIPtr = playerObjectData->m_currentInventory->VerifyInventory(invPtr->m_itemData->m_globalID);
									  if(querryIPtr)
										 querryIPtr->m_armed = TRUE;
								  }
								}
								playerObjectData->m_reconfigInven->SafeDelete();
								delete playerObjectData->m_reconfigInven;
								playerObjectData->m_reconfigInven = 0;
							  }
							  //apply stasis to return to state
						      playerObjectData->m_stasis = 0;//shifted
                              playerObjectData->m_altCfgIndex = 0;
						      playerObjectData->m_curEDBIndex = playerObjectData->m_originalEDBCfg;
						      characterOnAccount->m_stasis = 0;//shifted
						      characterOnAccount->m_altCfgIndex = 0;
						      characterOnAccount->m_curEDBIndex = playerObjectData->m_originalEDBCfg;
							  updateNeeded = true;
						   }//end must be shifted
						 }//end shifting out of
                         //remove player

                         if(updateNeeded)
						 {//player needs to be respawned
							//ok so send client info already
							m_sharedInterfaceRef->m_netComObject->SendSpawnMsg(DPNSEND_GUARANTEED,
															idFromLoc,
															(short)characterOnAccount->m_curEDBIndex,
															playerObjectData->m_currentWorldMap,
															playerObjectData->m_altCfgIndex,//beta
															(short)playerObjectData->m_Target3dPosition.x,
															(short)playerObjectData->m_Target3dPosition.y,
															(short)playerObjectData->m_Target3dPosition.z);
							//remove player
							if(!m_sharedInterfaceRef->m_netComObject->UpdateAccount(playerObjectData,accountPtr))
							{//update account
								m_sharedInterfaceRef->m_eventLog->AddEvent("Error updateing account during shapeshift",accountPtr->m_accountName,114);
								m_sharedInterfaceRef->m_netComObject->Disconnect(idFromLoc);
								return FALSE;
							}//end update account
							
							if(playerObjectData->m_aiControlled == FALSE)
			                   m_sharedInterfaceRef->m_netComObject->m_currentWorldsUsed->DecrementZoneConnections_safe(playerObjectData->m_currentWorldID);
							
							if(m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->RemoveByNetID_Alive(playerObjectData->m_networkAssignedID))//dont let nobody do anything to this
								playerObjectData = 0;
							
							return TRUE;
						 }//end player needs to be respawned
					  }
					  updateStats = FALSE;
				   }//end not in any other statis
				}//end shapeshifting
			 }//end success
			 else if(result == 38){//you lack a required item
                m_sharedInterfaceRef->SendGeneralMessageHandler(33,playerObjectData);
			 }//end you lack a required item
			 else if(result == 39){//you lack a components like a scroll
                m_sharedInterfaceRef->SendGeneralMessageHandler(34,playerObjectData);
			 }//end you lack a components like a scroll
			 else if(result == 40){//inventory full
                m_sharedInterfaceRef->SendGeneralMessageHandler(29,playerObjectData);
			 }//end inventory full
			 else if(result == 10){//unsuccessful attempt (not error)
                m_sharedInterfaceRef->SendGeneralMessageHandler(18,playerObjectData);
			 }//end unsuccessful attempt (not error)
			 else if(result == 14){//stat requirment not met
                //m_sharedInterfaceRef->SendGeneralMessageHandler(19,playerObjectData);
				m_sharedInterfaceRef->SendDetailedMsgToClient(2,
									    formatedMessage->uniShort3,
									    (formatedMessage->uniInt2-1),
										formatedMessage->uniInt,
										playerObjectData);
			 }//end stat requirment not met
			 else if(result == 8){//do not have the reqired items to attemp this
                //m_sharedInterfaceRef->SendGeneralMessageHandler(20,playerObjectData);
				m_sharedInterfaceRef->SendDetailedMsgToClient(1,
									    formatedMessage->uniShort3,
									    (formatedMessage->uniInt2-1),
										formatedMessage->uniInt,
										playerObjectData);
			 }//end do not have the reqired items to attemp this
			 else if(result == 21){//you lack the skill to attempt this
                m_sharedInterfaceRef->SendGeneralMessageHandler(22,playerObjectData);
			 }//end you lack the skill to attempt this
			 else{//misc error
                 if(result == 3)
				   m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,idFromLoc,"EXP OUT OF RANGE ERROR",0,playerObjectData,0);
				 else if(result == 2)
				   m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,idFromLoc,"INVALID LEVEL ERROR",0,playerObjectData,0);
				 else if(result == 19)
				   m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,idFromLoc,"LEVEL DOES NOT EXIST ERROR",0,playerObjectData,0);
				 else
				   m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,idFromLoc,"SKILL ERROR 435",0,playerObjectData,0);
			 }//end misc error

			 if(newAddedSkillRetPtr){//Notify new skill added
			    CString msgBuild = "You begin to train the skill-";
				CSkillObject * skillPtrRef = m_sharedInterfaceRef->m_skillDatabase->GetSkillByType(newAddedSkillRetPtr->m_skillType);
				if(skillPtrRef)
			       msgBuild = operator+(msgBuild,skillPtrRef->m_skillName);
			    m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,idFromLoc,msgBuild,0,playerObjectData,0);//confermation
			 }//end Notify new skill added

			 if(skillPtrReference)
			 if(levelUpObj != NULL){//notify leveled up
				int levelNumber = 1;
				CString newTitle;
				
				
				CSkillObject * skillPtrRef = m_sharedInterfaceRef->m_skillDatabase->GetSkillByType(skillPtrReference->m_skillType);
				if(skillPtrRef)
				{
				   skillPtrReference->EvaluateLevelAndTitle(newTitle,levelNumber,skillPtrRef);
                   CString msgBuild = "You are now Level "+m_sharedInterfaceRef->m_cabLib.ConvToString(levelNumber);
			       msgBuild = msgBuild+" ";
				   msgBuild = msgBuild+skillPtrRef->m_skillName;
			       m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,idFromLoc,msgBuild,0,playerObjectData,0);//confermation
				   m_sharedInterfaceRef->SendDetailedMsgToClient(6,
																 145,
																 playerObjectData->m_networkAssignedID,
																 0,
																 playerObjectData);  
				}
			 }//end notify leveled up

             if(updateStats)
		        m_sharedInterfaceRef->UpdateClientStatistics(playerObjectData,idFromLoc);
			 
	   return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribExpandItemDataRequest_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
          CPlayerObject * currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
          if(!currentPlayer)
		  {
              m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid player request, item exp obj");
              return FALSE;
		  }
		  
	      m_sharedInterfaceRef->SendAvailableFromItem(currentPlayer,formatedMessage->uniInt,formatedMessage->uniInt2);
		  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		  return TRUE;
}
//------------------------------------------------------------------------------------//
// A client sees a house nearby it doesn't know yet (uniInt = owner account) and asks
// for it: answer with its model and position (entity cfg, category 83).
BOOL CAttributeSystems::AtribRequestHouseCfg_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
	CHousingZoneDB * zones = m_sharedInterfaceRef->m_netComObject->m_houseingDB;
	if(!zones)
		return FALSE;
	m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Lock();
	m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Lock();
	CPlayerObject * reciepiant = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByDPNID_PrefAlive(idFromLoc);
	CHPlacementObj * house = zones->GetHouseByAccountBind(formatedMessage->uniInt);
	if(reciepiant && house)
	{
		ENTITYCFG_HEADER cfg;
		ZeroMemory(&cfg,sizeof(cfg));
		cfg.m_catagorie       = 83;
		cfg.networkDefinedID  = house->m_accountBind;
		cfg.dbCfg             = (short)house->m_type;
		cfg.Xpos              = (short)house->m_wldPosition.x;
		cfg.Ypos              = (short)house->m_wldPosition.y;
		cfg.Zpos              = (short)house->m_wldPosition.z;
		reciepiant->m_playerEntityCfgOutbox->AddPacket(sizeof(cfg),(BYTE*)&cfg,0,reciepiant->m_currentWorldID);
	}
	m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribBuyItemFromHouseStore_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
		//redesign
		return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribAddItemToHouseStore_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
		//redesign
		return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribRemoveItemFromHouseStore_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
	    //redesign
		return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribRemoveCashFromHouseStore_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
		//redeign
		return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribRequestToOrganize2BankItems_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
	     m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Lock();
         CAccountObject * accountPtr = (CAccountObject *)m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountByID(idFromLoc);
	     if(!accountPtr)
	     {
		   m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
		   m_sharedInterfaceRef->m_eventLog->LogByPriority("AtribRequestToOrganize2BankItems_safe() cannot find DPNID:",(DWORD)idFromLoc,3);
		   return FALSE;
	     }

         CPlayerObject * currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
		 if(!currentPlayer)
		 {
			 m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
             m_sharedInterfaceRef->m_eventLog->AddEvent("Cannot find current player that is banking");
             return FALSE;
		 }

		  if(currentPlayer->m_energy < 1)
		  {
			 m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			 m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
		     return FALSE;
		  }

          if(currentPlayer->m_stencilChannel < 0)//cannot find stencil
	         m_sharedInterfaceRef->m_eventLog->AddEvent("Cannot find bank currentPlayer->m_stencilChannel AtribRequestToOrganize2BankItems_safe");

		  CBankObj * bankPtr = m_sharedInterfaceRef->m_bankZoneList->GetBankByPosition(currentPlayer->m_3dPosition,currentPlayer->m_stencilChannel);
          if(bankPtr)
		  {//in bank zone
              
		  CPlayerObject * characterOnBaseAccount = accountPtr->m_characterOnAccount;
		  

          if(!currentPlayer)
		  {
              m_sharedInterfaceRef->m_eventLog->AddEvent("Requesting to bank item, but failed","",143);
			  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
			  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
              return FALSE;
		  }

		  /*
			  if(characterOnBaseAccount->m_bankInventory)
			  {
				 if(characterOnBaseAccount->m_bankInventory->ReorderInventory(formatedMessage->uniInt,formatedMessage->uniInt2,1))
				 {

				 }
			  }
			  */
				for (int i=0; i<characterOnBaseAccount->m_banks.size(); i++)
				{
					CInventoryObjList *pBank = characterOnBaseAccount->m_banks[i];
					if (!pBank) continue;
					pBank->ReorderInventory(formatedMessage->uniInt, formatedMessage->uniInt2, 1);
				}
		  }//end in bank zone

		  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		  m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
	      return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::AtribRequestToOrganize2InventoryItems_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
         CPlayerObject * currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
		 if(!currentPlayer)
		 {
             m_sharedInterfaceRef->m_eventLog->AddEvent("Cannot find current player that is banking");
             return FALSE;
		 }

		  if(currentPlayer->m_energy < 1)
		  {
		     m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		     return FALSE;
		  }

          if(!currentPlayer)
		  {
			  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
              m_sharedInterfaceRef->m_eventLog->AddEvent("Invalid Player Handle 61523","",143);
              return FALSE;
		  }

		  if(currentPlayer->m_currentInventory)
		  {
			 if(currentPlayer->m_currentInventory->ReorderInventory(formatedMessage->uniInt,formatedMessage->uniInt2,1))
			 {

			 }
		  }

		  m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	      return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::HandleQuestJournalRequest_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
     CPlayerObject * currentPlayer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByDPNID_Alive(idFromLoc);
     if(!currentPlayer)
         return FALSE;

	 if(currentPlayer->m_aiControlled)
	 {
		 m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	     return FALSE;
	 }

	 if(!currentPlayer->m_playerOrigRef)
	 {
		 m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	     return FALSE;
	 }

	 if(!currentPlayer->m_playerOrigRef->m_questJournal)
	 {
         if(formatedMessage->uniShort3 != 1)
         {
             CString ignoreMsg = "No current quests are logged.";
             m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
						                       idFromLoc,
						                       ignoreMsg,
							                   0,
											   currentPlayer,
											   0);//confermation
         }

         m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
         return FALSE;
	 }

	 if(currentPlayer->m_playerOrigRef->m_questJournal->GetCount() == 0)
	 {
         if(formatedMessage->uniShort3 != 1)
         {
             CString ignoreMsg = "No current quests are logged..";
             m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
						                       idFromLoc,
						                       ignoreMsg,
							                   0,
											   currentPlayer,
											   0);//confermation
         }

         m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
         return FALSE;
	 }

     int trace = 0;
               CString fillerRequiredText = "     REQUIRED ITEMS";
			   CString fillerRequiredText2 = " ";
			   BLOCKQUEST_DATA questBlockHeader;
			   questBlockHeader.m_catagorie = 33;

			   // NOTE: This should be the quest window visual ID.
               questBlockHeader.m_itemAssignedID = -formatedMessage->uniInt;	
                
               int textMenuLength = 0;
			   POSITION qPos;
			   for(qPos = currentPlayer->m_playerOrigRef->m_questJournal->GetHeadPosition();qPos != NULL;)
			   {//aquire full data length
                   CQuestJournalObj * questPtr = (CQuestJournalObj *)currentPlayer->m_playerOrigRef->m_questJournal->GetNext(qPos);
				   textMenuLength += 1; // Packet ID
				   textMenuLength += 1; // Count of journal entries
                   textMenuLength += questPtr->m_questDescription.GetLength()+1;
                   textMenuLength += questPtr->m_npcName.GetLength()+1;
				   textMenuLength += questPtr->m_keyStatement.GetLength()+1;
				   //textMenuLength += fillerRequiredText.GetLength()+1;
				   if (questPtr->m_requiredItems)
					textMenuLength += questPtr->m_requiredItems->GetCount()+1;
				   textMenuLength += 1; // Quantity of required items
				   
				   if(questPtr->m_requiredItems)
				   {
					   for(POSITION reqPos = questPtr->m_requiredItems->GetHeadPosition();reqPos != NULL;)
					   {
						   CQJRequiredItem * reqItemPtr = (CQJRequiredItem *)questPtr->m_requiredItems->GetNext(reqPos);
						   textMenuLength += reqItemPtr->m_itemName.GetLength()+1;
					   }
				   }

				   /*
				   if(questPtr->m_status == 0)
					   fillerRequiredText2 = "Quest Status: Not Completed";
				   else
				       fillerRequiredText2 = "Quest Status: Completed";

				   textMenuLength += fillerRequiredText2.GetLength()+1;
				   */
					textMenuLength += 1; // Quest status
			   }//end aquire full data length

			   if(textMenuLength > 0)
			   {//somthing to send
                int msgSize = sizeof(BLOCKQUEST_DATA)+textMenuLength;
                BYTE * blockData = new BYTE[msgSize];
                CopyMemory(blockData,&questBlockHeader,sizeof(BLOCKQUEST_DATA));
                int traceDataBuild = sizeof(BLOCKQUEST_DATA);

				// ENCODE packet ID
				blockData[traceDataBuild] = QUEST_JOURNAL_ID;
				++traceDataBuild;

				// ENCODE count of journal entries
				blockData[traceDataBuild] = currentPlayer->m_playerOrigRef->m_questJournal->GetCount();
				++traceDataBuild;

			    for(qPos = currentPlayer->m_playerOrigRef->m_questJournal->GetHeadPosition();qPos != NULL;)
			    {//fill text data
                   CQuestJournalObj * questPtr = (CQuestJournalObj *)currentPlayer->m_playerOrigRef->m_questJournal->GetNext(qPos);
                 
				   // ENCODE quest description
				   blockData[traceDataBuild] = questPtr->m_questDescription.GetLength();
   				   traceDataBuild++;
				   int i = 0;
                   for(i=0;i<questPtr->m_questDescription.GetLength();i++)
				   {
                       blockData[traceDataBuild] = questPtr->m_questDescription.GetAt(i);
   				       traceDataBuild++;
				   }

				   // ENCODE quest NPC name
				   blockData[traceDataBuild] = questPtr->m_npcName.GetLength();
   				   traceDataBuild++;
                   for(i=0;i<questPtr->m_npcName.GetLength();i++)
				   {
                       blockData[traceDataBuild] = questPtr->m_npcName.GetAt(i);
   				       traceDataBuild++;
				   }

				   // ENCODE quest key statement
				   blockData[traceDataBuild] = questPtr->m_keyStatement.GetLength();
   				   traceDataBuild++;
                   for(i=0;i<questPtr->m_keyStatement.GetLength();i++)
				   {
                       blockData[traceDataBuild] = questPtr->m_keyStatement.GetAt(i);
   				       traceDataBuild++;
				   }

				   /*
				   blockData[traceDataBuild] = fillerRequiredText.GetLength();
   				   traceDataBuild++;
                   for(i=0;i<fillerRequiredText.GetLength();i++)
				   {
                       blockData[traceDataBuild] = fillerRequiredText.GetAt(i);
   				       traceDataBuild++;
				   }
				   */

				   // ENCODE quantity of required items
				   if (questPtr->m_requiredItems)
						blockData[traceDataBuild] = questPtr->m_requiredItems->GetCount();
				   else
					   blockData[traceDataBuild] = 0;
   				   traceDataBuild++;

				   // ENCODE required items
				   if(questPtr->m_requiredItems)
				   {
					   for(POSITION reqPos = questPtr->m_requiredItems->GetHeadPosition();reqPos != NULL;)
					   {
						   CQJRequiredItem * reqItemPtr = (CQJRequiredItem *)questPtr->m_requiredItems->GetNext(reqPos);

						   blockData[traceDataBuild] = reqItemPtr->m_itemName.GetLength();
   						   traceDataBuild++;
						   for(i=0;i<reqItemPtr->m_itemName.GetLength();i++)
						   {
							   blockData[traceDataBuild] = reqItemPtr->m_itemName.GetAt(i);
   							   traceDataBuild++;
						   }

						   // Quantity of required item
						   blockData[traceDataBuild] = reqItemPtr->m_quanity;
   						   traceDataBuild++;
					   }
				   }

				   /* OBSOLETE
				   if(questPtr->m_status == 0)
					   fillerRequiredText2 = "Quest Status: Not Completed";
				   else
				       fillerRequiredText2 = "Quest Status: Completed";

				   blockData[traceDataBuild] = fillerRequiredText2.GetLength();
   				   traceDataBuild++;
                   for(i=0;i<fillerRequiredText2.GetLength();i++)
				   {
                       blockData[traceDataBuild] = fillerRequiredText2.GetAt(i);
   				       traceDataBuild++;
				   }
				   */
					// ENCODE quest status
				   blockData[traceDataBuild] = questPtr->m_status;
					++traceDataBuild;
			    }//end fill text data
                m_sharedInterfaceRef->m_netComObject->SendData(blockData,
				                             msgSize,
				                             DPNSEND_GUARANTEED,
				                             idFromLoc,
											 m_sharedInterfaceRef->m_netComObject->m_packetTimeout);
			    if (blockData){
		           delete [] blockData;
		           blockData=0;
			    }
			   }//end somthing to send
			   else{//send ignoring msg
			    CString ignoreMsg = "No current quests are logged...";
                m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
						                         idFromLoc,
						                         ignoreMsg,
							                     0,
												 currentPlayer,
											     0);//confermation

			   }//end send ignoring msg
   m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
   return TRUE;
}
//--------------send message----------------------------------------------------------//
BOOL CAttributeSystems::ReportMurder_safe(int netAssignedID, int handle, int creditAmount)
{
    CPlayerObject * characterObj = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(netAssignedID);
    if(!characterObj)
    {
         return FALSE;
    }

	 if(!characterObj->m_playerOrigRef)
	 {
		 m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	     return FALSE;
	 }

	 if(characterObj->m_energy < 1)
	 {
		 m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	     return FALSE;
	 }


    if(!characterObj->m_noteriety)
	{
		m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	    return FALSE;
	}

	if(!characterObj->m_noteriety->m_currentAgressors)
	{
		m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		return FALSE;
	}

    if(characterObj->m_noteriety->m_currentMurders >= m_sharedInterfaceRef->m_netComObject->m_murderCountMax)
	{
	   characterObj->m_noteriety->m_currentAgressors->Flush();
	   m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	   return FALSE;
	}

	characterObj->m_noteriety->m_currentAgressors->CacheTimedOutAgressors(350000);

	POSITION posLast = NULL;
	for(POSITION posLoc = characterObj->m_noteriety->m_currentAgressors->GetHeadPosition();(posLast = posLoc) != NULL;)
	{//agressor loop
        CAggressorObj * aPtr = (CAggressorObj *)characterObj->m_noteriety->m_currentAgressors->GetNext(posLoc);

		CPlayerObject * murderer = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandle(aPtr->m_handle);//find murderer
        if(murderer)
		if(murderer->m_playerOrigRef)
	    {//found the murderer

		   if(murderer->m_handle == handle)
		   {
            //deduct cash from bank
		    /*if(characterObj->m_playerOrigRef->m_bankCash)
		       if(characterObj->m_playerOrigRef->m_bankCash->m_amount >= creditAmount && creditAmount > 0 && creditAmount < 30000000)
			   {//enough top place valid bounty
			      characterObj->m_playerOrigRef->m_bankCash->m_amount -= creditAmount;
                  murderer->m_playerOrigRef->m_currentBounty += creditAmount;
				  m_sharedInterfaceRef->m_netComObject->m_bountySystem->AddBounty(murderer->m_charName,murderer->m_playerOrigRef->m_currentBounty,murderer->m_handle,10);
			   }//end enough top place valid bounty*/
            //send attrib to open murder window
		    murderer->m_charName;
		    murderer->m_handle;
            if(characterObj->m_clanHandle != murderer->m_clanHandle || murderer->m_clanHandle == -1 || characterObj->m_clanHandle == -1 )
		    {
				 murderer->m_countDownToMurderPardon = m_sharedInterfaceRef->m_netComObject->m_goodBehaviorDuration;//10 hours
				 murderer->m_noteriety->m_currentMinuteCountSinceLastMurder = 0;
				 murderer->m_noteriety->m_currentMurders++;
			     m_sharedInterfaceRef->SendGeneralMessageHandler(39,murderer);
 
				 delete aPtr;
				 aPtr = 0;
                 characterObj->m_noteriety->m_currentAgressors->RemoveAt(posLast);

                 m_sharedInterfaceRef->RequestMurdersToReport(characterObj);//finish up any pending reports

				 m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
				 return TRUE;
		    }
			m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		    return TRUE;
		   }
		}//end found the murderer
	}//end agressor loop
	
	m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
    return FALSE;
}
//------------------------------------------------------------------------------------//
void CAttributeSystems::SendForceJetValuesIfAvail(CInventoryObjList * inventoryList,CPlayerObject * playerToReceive)
{
  for(POSITION invenPos = inventoryList->GetHeadPosition();invenPos != NULL;)
  {
	  CInventoryObj * invenObj = (CInventoryObj *) inventoryList->GetNext(invenPos);
	  if(invenObj->m_armed)
	  if(invenObj->m_itemData)
	  {
		  if(m_sharedInterfaceRef->IfItemForceJetSendIt(invenObj->m_itemData,playerToReceive))
		     return;
	  }
  }
}
//------------------------------------------------------------------------------------//
BOOL CAttributeSystems::RequestAiPendingInfo_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc)
{
      //m_sharedInterfaceRef->m_eventLog->LogByPriority("ai channel update recieved",2);
      //this is called when no ai is spawned in the world-has to happen for messaging-may redesign later on
	  if(!m_sharedInterfaceRef->SendUpdateToAIChannel_safe(idFromLoc,formatedMessage->uniInt))
	  {
          m_sharedInterfaceRef->m_eventLog->LogByPriority("**SendUpdateToAIChannel_safe() FAILED",2);
          return FALSE;
	  }
	  return TRUE;
}
