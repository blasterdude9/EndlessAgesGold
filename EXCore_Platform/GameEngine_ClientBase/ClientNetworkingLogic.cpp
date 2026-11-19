#include "StdAfx.h"
#include "direct.h"
#include "clientnetworkinglogic.h"
#include <vector>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CClientNetworkingLogic::CClientNetworkingLogic(GlobalStatusVariables * gStatus)
{
   m_gStatus						= gStatus;
   m_bQuickSlotsVerified			= FALSE;
   m_pingInQuerry					= FALSE;
   CURPING							= 500;
   m_pingPersistStamp               = 0;
   MultiplayerDelayComm				= 1000;
   MultiplayerDelayCommVarial		= 0;
}

CClientNetworkingLogic::~CClientNetworkingLogic(void)
{
}
//-------------------------------------------
//Primary callback for network client actions
//-------------------------------------------
void CClientNetworkingLogic::ClientCallback(CSpawnObjList * spawnGeneratorDB,
											CMovementObjList * entObjRuntime,
											CMissileObjList  * missileRuntime,
											CExplosionObjList * explosionRuntimeList)
{
    //UpdateMonitor();//place in application
   if(m_gStatus->m_multiplayerObject->GetConnectionStatus() != 0)
   {//server informing of problem 
	  if(m_gStatus->m_systemReadout)
         m_gStatus->m_systemReadout->AddLine(m_gStatus->m_multiplayerObject->GetConnectionStatusDesc());

	  m_gStatus->m_multiplayerObject->SetConnectionStatus("", 0);
	  return;
   }//end server informing of problem

   if(!m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
	   return;

   if(!m_gStatus->m_multiplayerObject->m_enabledCommunicationInit)
   {//must do this first and once
	  m_gStatus->m_multiplayerObject->m_timeElStamp = m_gStatus->GetTimeStamp();
	  m_gStatus->m_multiplayerObject->m_enabledCommunicationInit = TRUE;
   }
   //check connection status

   if(m_gStatus->m_multiplayerObject->m_menuQue != -1)
   {//one of the few ways to que menus client side
      m_gStatus->OpenMenu(m_gStatus->m_multiplayerObject->m_menuQue, false);
	  m_gStatus->m_multiplayerObject->m_menuQue = -1;
   }//end one of the few ways to que menus client side

       //send update to server
       LPVOID lpMessage = NULL;
       DPNID messageFrom = 0;
       long currentTickCount = GetTickCount();
       int sizeTracker = 0;

	   
       while(m_gStatus->m_multiplayerObject->m_packetQue->GetNextPacket_safe(&sizeTracker,(BYTE**)&lpMessage,&messageFrom)==TRUE)
	   {//if theres a message handle it
		  m_gStatus->m_multiplayerObject->AddToTotalBandwidthIncomming_safe((unsigned long)sizeTracker);
          HandleEngineMessagesClientSide(lpMessage,
			                             messageFrom,
										 sizeTracker,
										 spawnGeneratorDB,
										 entObjRuntime,
										 missileRuntime,
										 explosionRuntimeList,
										 NULL,
										 NULL,
										 0);
	   }//end if theres a message handle it

  //UpdateServerCallback(spawnGeneratorDB,entObjRuntime,missileRuntime);//update server based on frequency
}
//--------------Primary Handler---------------------------------------------------//
BOOL CClientNetworkingLogic::HandleEngineMessagesClientSide(LPVOID lpMessage,
															DPNID idFromLoc,
															int msgSize,
															CSpawnObjList * spawnGeneratorDB,
															CMovementObjList * entObjRuntime,
															CMissileObjList  * missileRuntime,
															CExplosionObjList * explosionRuntimeList,
															CCollisionObj * collisionBase,
														    CCollisionObjList * collisionDatabase,
															int worldChannelRef)
{
	//cast into an identity
    LPEXMSG_CATAGORIE identityMsg = (LPEXMSG_CATAGORIE)lpMessage;
    if(msgSize <= 0)
	   return TRUE;
	//-----------------------------------------------------------//

    if(identityMsg->m_catagorie == 2)
	{//reconnect to node-added separate packet because this is the last msg the client will recieve from that node till new connection is made
        LPRECONNECT_STRUCT formatedMessage = (LPRECONNECT_STRUCT)lpMessage;
		
		m_gStatus->ConnectToServerNode(formatedMessage->ipaddress);
		
		delete formatedMessage;
		formatedMessage = 0;
		
	    return TRUE;
	}//end reconnect to node
	if(identityMsg->m_catagorie == 3)
	{//menu que
	   if(m_gStatus->m_multiplayerObject->m_currentCharacterInUse == -1)
          m_gStatus->m_multiplayerObject->m_menuQue = 50;
	   else
	   {//auto select
          m_gStatus->m_netGameLogicObject->SendAttribMsg(     DPNSEND_GUARANTEED,
															  DPID_SERVERPLAYER,
															  9,
															  m_gStatus->m_multiplayerObject->m_currentCharacterInUse,
															  0,
															  m_gStatus->m_localContentClientVersion,
															  15,//VERSION EXPLOIT
															  NULL);
		  m_gStatus->m_networkFreqStamp				= m_gStatus->GetTimeStamp_DW();//ensures guarenteed cycle goes out to the server
		  m_gStatus->m_switchingConnections			= TRUE;
	   }//end auto select
	   delete [] (BYTE*)lpMessage;//was leaked and fell through to 'Unknown catagorie'
	   return TRUE;
	}//end manu que
	//-----------------------------------------------------------//
	if(identityMsg->m_catagorie == 5)
	{//equip msg
       LPEQUIP_STRUCT formatedMessage = (LPEQUIP_STRUCT)lpMessage;
	   if(m_gStatus->m_multiplayerObject->GetEquipCRC(formatedMessage)==formatedMessage->CRCcheck)//check packet CRC
	      HandleEquipMessageClientSide(formatedMessage,entObjRuntime,missileRuntime);
	   else{//bad CRC
            m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Invalid Equip CRC from Server");
	   }//end bad CRC
	   delete formatedMessage;
	   formatedMessage=0;
	   return TRUE;
	}//end eqip msg
	//-----------------------------------------------------------//
	if(identityMsg->m_catagorie == 6)
	{//attrib msg
       LPATTRIB_STRUCT formatedMessage = (LPATTRIB_STRUCT)lpMessage;
	   if(m_gStatus->m_multiplayerObject->GetAttribCRC(formatedMessage)==formatedMessage->CRCcheck)//check packet CRC
	      HandleAttribMessageClientSide(formatedMessage,idFromLoc,spawnGeneratorDB,entObjRuntime,worldChannelRef);
	   else{//bad CRC
            m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Invalid Equip CRC from Server");
	   }//end bad CRC
	   delete formatedMessage;
	   formatedMessage=0;
	   return TRUE;
	}//end attrib msg
	//---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 7)
	{//house block info ai server only
       return HandleHouseingBlock((BYTE *)lpMessage,msgSize);
	}//end house block info ai server only
    //---------------------------------------------------------------//
	if(identityMsg->m_catagorie == 8)
	{//spawn msg
       LPSPAWN_STRUCT formatedMessage = (LPSPAWN_STRUCT)lpMessage;
       HandleSpawnMessagesClientSide(formatedMessage,idFromLoc);
	   delete formatedMessage;
	   formatedMessage=0;
	   return TRUE;
	}//end spawn msg
	//---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 10)
	{//item cfg msg
       LPITEM_LOCATION itemLocationMsg = (LPITEM_LOCATION)lpMessage;
	   BYTE * dataBlock = (BYTE*)lpMessage;

	   if(!m_gStatus->m_multiplayerObject->m_requestingPortal)
           HandleItemLocationalMessagesClientSide(dataBlock,entObjRuntime,missileRuntime,msgSize); //UPGRADE

	   delete [] dataBlock;
	   dataBlock = 0;

	   return TRUE;
	}//end item cfg msg
	//---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 11)
	{//txt msg
	   HandleTextMessageClientSide(lpMessage,msgSize,idFromLoc);
       BYTE * dataBlock = (BYTE*)lpMessage;
       delete [] dataBlock;
	   dataBlock = 0;
	   return TRUE;
	}//end txt msg
	//---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 12)
	{//menu build msg
       LPMENUBUILD_STRUCT formatedMessage = (LPMENUBUILD_STRUCT)lpMessage;
	   if(m_gStatus->m_multiplayerObject->GetBuildMenuMsgCrC(formatedMessage)==formatedMessage->CRCcheck){//check packet CRC
          HandleMenuBuildMessagesClientSide(formatedMessage,idFromLoc);
	   }//end check packet validity
	   else{//bad CRC
          m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Invalid Menu Build CRC from Server");
	   }//end bad CRC
	   delete formatedMessage;
	   formatedMessage=0;
	   return TRUE;
	}//end menu build msg
	//---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 18)
	{//ai mov block
	   POSITION tagPos;
       for(tagPos = entObjRuntime->GetHeadPosition();tagPos != NULL;)
	   {//initialize all entities
           CMovementObj * mPtr = (CMovementObj *)entObjRuntime->GetNext(tagPos);
		   mPtr->m_validThisRound = FALSE;
	   }//end initialize all entities
	 

	   //handle mov data
	   HandleAIMovBlockData(lpMessage,msgSize,idFromLoc,entObjRuntime,missileRuntime,explosionRuntimeList,spawnGeneratorDB,worldChannelRef);

	   //remove any entities that are not valid anymore
       POSITION tagPosLast = NULL;
       for(tagPos = entObjRuntime->GetHeadPosition();(tagPosLast = tagPos) != NULL;)
	   {
           CMovementObj * mPtr = (CMovementObj *)entObjRuntime->GetNext(tagPos);
		   if(mPtr->m_validThisRound == FALSE && mPtr->List_networkEntity == TRUE)
		   {//remove
              m_gStatus->DestroyRuntimeEntity(mPtr,spawnGeneratorDB,tagPosLast,entObjRuntime);
		   }//end remove
	   }

	   return TRUE;
	}//end mov block
	//---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 19)
	{//mov block
	   POSITION tagPos;
       for(tagPos = entObjRuntime->GetHeadPosition();tagPos != NULL;)
	   {//initialize all entities
           CMovementObj * mPtr = (CMovementObj *)entObjRuntime->GetNext(tagPos);
		   mPtr->m_validThisRound = FALSE;
	   }//end initialize all entities
	   m_gStatus->m_houseingCache->FlagAllInvalid();//set flag on all current houses in cache
		
	   //handleping
	   m_pingInQuerry = FALSE;
	   m_gStatus->m_multiplayerObject->m_lastMovMsgRecievedTime = GetTickCount();
       CURPING = m_gStatus->GetElaspeTimeMS_DW(m_pingPersistStamp);
	   
	   
	   if(CURPING < (DWORD)m_gStatus->m_multiplayerObject->m_frequencyLimit){
		  MultiplayerDelayComm = m_gStatus->m_multiplayerObject->m_frequencyLimit+25;
          m_gStatus->m_multiplayerObject->m_communicationFrequency = m_gStatus->m_multiplayerObject->m_frequencyLimit+25;
		  m_gStatus->m_multiplayerObject->m_retrySendDuration = m_gStatus->m_multiplayerObject->m_frequencyLimit*2;
	   }
	   else{
	   
	   MultiplayerDelayComm = CURPING+50;
       if(MultiplayerDelayComm > 1500)
		  MultiplayerDelayComm = 1500;

	   m_gStatus->m_multiplayerObject->m_retrySendDuration = (long)MultiplayerDelayComm*2;

	   m_gStatus->m_multiplayerObject->m_communicationFrequency = (int)MultiplayerDelayComm;
	   }

	   //handle mov data
	   HandleClientMovBlockData(lpMessage,msgSize,idFromLoc,entObjRuntime,missileRuntime,explosionRuntimeList);

	   //remove any entities that are not valid anymore
       POSITION tagPosLast = NULL;
       for(tagPos = entObjRuntime->GetHeadPosition();(tagPosLast = tagPos) != NULL;)
	   {
           CMovementObj * mPtr = (CMovementObj *)entObjRuntime->GetNext(tagPos);
		   if(mPtr->m_validThisRound == FALSE && mPtr->List_networkEntity == TRUE)
		   {//remove
			   entObjRuntime->RemoveAt(tagPosLast);
               m_gStatus->DestroyRuntimeEntity(mPtr,spawnGeneratorDB,entObjRuntime);
	           mPtr = 0;
		   }//end remove
	   }
	   m_gStatus->m_houseingCache->RemoveAllInvalid();//remove All that were not in current list


	   return TRUE;
	}//end mov block
	//---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 20)
	{//list block data (GLIDs)
       return HandleClientListGlidBlockData(lpMessage,msgSize,idFromLoc);
	}//end list block data (GLIDs)
    //---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 21)
	{//entity cfg block
	   BYTE * byteBlockData = (BYTE*)lpMessage;
	   HandleEntityCfgBlock(lpMessage,msgSize,idFromLoc,entObjRuntime,missileRuntime);
	   delete [] byteBlockData;
	   byteBlockData = 0;

	   return TRUE;
	}//end entity cfg block
	//---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 24)
	{//bank block transfer
       return HandleBankDataBlock(lpMessage,msgSize,idFromLoc);
	}//end bank block transfer
    //---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 25)
	{//store block transfer
	   return HandleStoreDataBlock(lpMessage,msgSize,idFromLoc);
	}//end store block transfer
	//---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 26)
	{//store block transfer
	   return HandleHouseStoreDataBlock(lpMessage,msgSize,idFromLoc);
	}//end store block transfer
    //---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 28)
	{//misc msg handler
       LPMSGHANDLER formatedMessage = (LPMSGHANDLER)lpMessage;

       delete formatedMessage;
	   formatedMessage=0;
	   return TRUE;
	}//end store block transfer
    //---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 29)
	{//handle loot block
       return HandleLootBlockData(lpMessage,msgSize,idFromLoc);
	}//end handle loot block
	//---------------------------------------------------------------//
    if(identityMsg->m_catagorie == 30)
	{//sync timing for enviorments
       LPTIMESYNC_DATA formatedMessage = (LPTIMESYNC_DATA)lpMessage;
       
       m_gStatus->m_multiplayerObject->m_serverStartTime = GetTickCount() - formatedMessage->m_timePassed;
	   m_gStatus->m_multiplayerObject->m_frequencyLimit = formatedMessage->m_freqState;

       delete formatedMessage;
	   formatedMessage=0;
	   return TRUE;
	}//end sync timing for enviorments
	if(identityMsg->m_catagorie == 31)
	{//recieve group data
	   return ParseGroupListData((BYTE*)lpMessage,msgSize);
	}//end recieve group data
    if(identityMsg->m_catagorie == 32)
	{//recieve group data
	   return ParseClanRosterData((BYTE*)lpMessage,msgSize);
	}//end recieve group data
	//----------------------------------
	if(identityMsg->m_catagorie == 33)
	{
		// SETUP arena registration response packet
		bool bResult = false;
		ArenaResult result;
		int iPos = sizeof(BLOCKQUEST_DATA);
		BLOCKQUEST_DATA *pHeader = (BLOCKQUEST_DATA*)lpMessage;
		BYTE *pData = (BYTE*)lpMessage;

		// INIT arena result to SUCCESS
		result.Severity = AS_SUCCESS;
		result.Result = AR_SUCCESS;

		// GET the arena message ID of this packet
		unsigned char iPacketID = 0;
		::memcpy(&iPacketID, &pData[iPos], sizeof(char));
		iPos += sizeof(char);

		switch (iPacketID)
		{
			case ARENA_TYPE_ID:
			case ARENA_INFO_ID:
			case ARENA_JOIN_ID:
			case ARENA_LEAVE_ID:
			{
				// GET the arena result status
				::memcpy(&result.Severity, &pData[iPos], sizeof(result.Severity));
				iPos += sizeof(result.Severity);
				::memcpy(&result.Result, &pData[iPos], sizeof(result.Result));
				iPos += sizeof(result.Result);
				break;
			}
			default: break;
		}

		// INCREMENT the packet start address so our function 
		//	doesn't have to deal with header stuff
		int iPacketSize = msgSize - iPos; 
		BYTE *pPacket = (BYTE*)pHeader + iPos; 
		
		// HANDLE the arena request server response
		switch (iPacketID)
		{
			case ARENA_TYPE_ID:
			{
				bResult = this->HandleArenaTypeRequestClientSide(pPacket, iPacketSize, idFromLoc);
				break;
			}
			case ARENA_INFO_ID:
			{
				bResult = this->HandleArenaInfoRequestClientSide(pPacket, iPacketSize, idFromLoc);
				break;
			}
			case ARENA_JOIN_ID:
			{
				bResult = this->HandleArenaJoinRequestClientSide(pPacket, iPacketSize, idFromLoc);
				if (bResult)
				{
					// UPDATE arena status UI element
					this->UpdateArenaStatusMenu(ARENASTATUS_COMMITTED);
				}
				break;
			}
			case ARENA_LEAVE_ID:
			{
				bResult = this->HandleArenaLeaveRequestClientSide(pPacket, iPacketSize, idFromLoc);
				if (bResult)
				{
					// UPDATE arena status UI element
					this->UpdateArenaStatusMenu(ARENASTATUS_NONE);
				}
				break;
			}
			case QUEST_JOURNAL_ID:
			{
				// HANDLE quest journals
				bResult = this->HandleJournalRequestClientSide(pHeader, pPacket, iPacketSize, idFromLoc);
				break;
			}
			default: 
			{
				// HANDLE list-based progression quests
				CMenuListBox *pListBox = NULL;
				int whatListAreWe = 0;

				m_gStatus->OpenMenu(INTERACTWITHLIST_MENU_ID, false);
				pListBox = dynamic_cast<CMenuListBox*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(INTERACTWITHLIST_LISTBOX_ID, true));
				whatListAreWe = INTERACTWITHLIST_LISTBOX_ID;

				if(!pListBox)
					break;
				pListBox->ClearList();

				int tracerBuild = sizeof(BLOCKQUEST_DATA);
				while(tracerBuild < msgSize)
				{
					int curStrLen = pData[tracerBuild];
					char *listText = new char[curStrLen + 1];
					tracerBuild++;

					for(int loop = 0;loop < curStrLen; loop++)
					{
						listText[loop] = pData[tracerBuild];
						tracerBuild++;
					}

					listText[curStrLen] = '\0';

					m_gStatus->AddStringToMenuList(whatListAreWe, listText, -1, pHeader->m_itemAssignedID, 0, listText, 0, "");
				}

				// KLUDGE: RESET the ArenaResult struct to SUCCESS state
				result.Severity = AS_SUCCESS;
				result.Result = AR_SUCCESS;
				bResult = true;
				break;
			}
		}

		// ERROR checking: Did we fail to successfully handle the arena request?
		if ((result.Result != AR_SUCCESS) || !bResult)
		{
			this->HandleArenaFailedResponse((QUEST_PACKETS)iPacketID, result);
			return FALSE;
		}

		// DELETE the incoming packet byte array
		SafeDeleteArray(pData);
			
		// Bye!
		return TRUE;
		
		/* DEPRECATED /////////////////////////////////////////////////////////
		//open menu by id
		LPBLOCKQUEST_DATA formatedMessage = (LPBLOCKQUEST_DATA)lpMessage;
		BYTE * byteBlockData = (BYTE*)lpMessage;
		//m_gStatus->OpenMenu(INTERACTWITHLIST_MENU_ID, false);
		//Clear Existing List
		//while(visualReference->m_pFirstItem != NULL)
			//  visualReference->RemoveItem(0);
		//parse out test objects

		m_gStatus->OpenMenu(INTERACTWITHLIST_MENU_ID, false); //TODO: Switch this to use a value from the "important menu" array
		CMenuListBox *pListBox = dynamic_cast<CMenuListBox*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(INTERACTWITHLIST_LISTBOX_ID, true));
		pListBox->ClearList();

		int tracerBuild = sizeof(BLOCKQUEST_DATA);
		while(tracerBuild < msgSize)
		{
			int curStrLen = byteBlockData[tracerBuild];
			char *listText = new char[curStrLen + 1];
			tracerBuild++;

			for(int loop = 0;loop < curStrLen; loop++)
			{
				listText[loop] = byteBlockData[tracerBuild];
				tracerBuild++;
			}

			listText[curStrLen] = '\0';

			if(pListBox)
			{
				m_gStatus->AddStringToMenuList(181, listText, -1, formatedMessage->m_itemAssignedID, 0, listText, 0, "");
			}
		}

		delete [] byteBlockData;
		byteBlockData=0;	
		return TRUE;
		*/
	} // End arena registration response packet
	if(identityMsg->m_catagorie == 34)
	{//recieve abilities available
	   BYTE * byteBlockData = (BYTE*)lpMessage;
	   HandleSkillAbilitiesClientSide(byteBlockData,msgSize,idFromLoc);
	   delete [] byteBlockData;
	   byteBlockData=0;	
	   return TRUE;
	}//end recieve abilities available
    if(identityMsg->m_catagorie == 35)
	{//recieve existing skills
	   BYTE * byteBlockData = (BYTE*)lpMessage;
	   HandleSkillsOutputClientSide(byteBlockData,msgSize,idFromLoc);
	   delete [] byteBlockData;
	   byteBlockData=0;	
	   return TRUE;
	}//end recieve existing skills
    //----------------------------------
	if(identityMsg->m_catagorie == 36)
	{//recieve arena list-check world avaiability
		LPBLOCKQUEST_DATA formatedMessage = (LPBLOCKQUEST_DATA)lpMessage;
		BYTE * byteBlockData = (BYTE*)lpMessage;
	   
		/*open menu by id
		for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
		{//1
			CMenu3DObj * menu = (CMenu3DObj *)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
			if(menu->m_closeOnEsc)
				menu->CloseMenu(&m_gStatus->m_globalCursorPos);
		}//end 1*/
	   
		CString arenaName;

		//parse out test objects
		int loopTrace = 0;
		BOOL openMenuAllGood = TRUE;
		int tracerBuild = sizeof(BLOCKQUEST_DATA);
		while(tracerBuild < msgSize)
		{
			int curStrLen = byteBlockData[tracerBuild];
			char * listText = new char[curStrLen+1];
			tracerBuild++;

			for(int loop=0;loop<curStrLen;loop++)
			{
				listText[loop] = byteBlockData[tracerBuild];
				tracerBuild++;
			}

			listText[curStrLen] = 0;//null terminator
			if(loopTrace == 0)
				arenaName = listText;

			loopTrace++;
		}

		if(openMenuAllGood)
		{
			m_gStatus->OpenMenu(INTERACTWITHLIST_MENU_ID, false); //TODO: Switch this to use a value from the "important menu" array
			CMenuListBox *pListBox = dynamic_cast<CMenuListBox*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(INTERACTWITHLIST_LISTBOX_ID, true));
			if(pListBox)
			{
				pListBox->ClearList();
				m_gStatus->AddStringToMenuList(INTERACTWITHLIST_LISTBOX_ID, arenaName, -1, formatedMessage->m_itemAssignedID, 0, arenaName, 0, "");
			}
		}

		delete [] byteBlockData;
		byteBlockData = 0;	
		return TRUE;
	}//end recieve arena list
	if(identityMsg->m_catagorie == 37)
	{//recieve icon display
	   BYTE * memoryArray = (BYTE*)lpMessage;
       int structureSz = sizeof(ICON_POS);
 
       int iconDispCount = msgSize/structureSz;

       if(iconDispCount < 1)
	   {
          delete [] memoryArray;
		  memoryArray = 0;
          return FALSE;
	   }
       
       ICON_POS * iconList = new ICON_POS[iconDispCount];
	   BYTE * dataRefCast = (BYTE*)iconList;  
       //fill list
       for(int fillerCount=1;fillerCount<msgSize;fillerCount++)
	   {
           dataRefCast[fillerCount-1] = memoryArray[fillerCount];
	   }
	   //get menu
	   CMenu3DObj * menuObj = NULL;
	   for(POSITION rMenuPos = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();rMenuPos != NULL;)
	   {
           menuObj  = (CMenu3DObj *)m_gStatus->m_runtimeMenuDataBase->GetNext(rMenuPos);
		   if(menuObj->m_specialOperation != 0)
              break;
	   }
	   //fill menu buffer for icon overlays
	   if(menuObj)
	   for(int loop=0;loop<iconDispCount;loop++)
	   {
           
		   D3DVECTOR screenPos;
		   screenPos.x = iconList[loop].m_xPos;
           screenPos.y = iconList[loop].m_yPos;
		   screenPos.z = 3.0f;

           if(!menuObj->m_iconDynamicOverlays)
			   menuObj->m_iconDynamicOverlays = new CIconSysList();

		       menuObj->m_iconDynamicOverlays->AddIconFromDBtoThisDB(m_gStatus->m_iconDatabase,
										                             iconList[loop].m_iconDBIndex,
										                             screenPos,
										                             m_gStatus->m_3dDevice);
	   }

       delete [] iconList;
	   iconList = 0;

	   delete [] memoryArray;
	   memoryArray = 0;
	   return TRUE;
	}//end recieve icon display
    if(identityMsg->m_catagorie == 38)
	{//recieve gurenteed block data
	   BYTE * blockCast = (BYTE*)lpMessage;
	   LPENTITYGUPDATE_HEADER formatedMessage = (LPENTITYGUPDATE_HEADER)lpMessage;
       int dataTracer = sizeof(ENTITYGUPDATE_HEADER);

       if(formatedMessage->m_attributeMsgCount > 0)
	   {//attribute batch coming in
		  int byteSize = sizeof(ATTRIB_STRUCT)*formatedMessage->m_attributeMsgCount;
	      ATTRIB_STRUCT * attribArray = new ATTRIB_STRUCT[formatedMessage->m_attributeMsgCount];
		  BYTE * atCast = (BYTE*)attribArray;
		  
		  for(int i=0;i<byteSize;i++)
		  {
              atCast[i] = blockCast[dataTracer];
			  dataTracer++;
		  }
		  for(int atLoop=0;atLoop<formatedMessage->m_attributeMsgCount;atLoop++)
		  {
           HandleAttribMessageClientSide(&attribArray[atLoop],idFromLoc,spawnGeneratorDB,entObjRuntime,worldChannelRef);
		  }
		  delete [] attribArray;
		  attribArray = 0;
	   }//end attribute batch coming in
	   if(formatedMessage->m_equipMsgCount > 0)
	   {//equip batch coming in
		  int byteSize = sizeof(EQUIP_STRUCT)*formatedMessage->m_equipMsgCount;
	      EQUIP_STRUCT * equipArray = new EQUIP_STRUCT[formatedMessage->m_equipMsgCount];
		  BYTE * atCast = (BYTE*)equipArray;
	      for(int i=0;i<byteSize;i++)
		  {
              atCast[i] = blockCast[dataTracer];
			  dataTracer++;
		  }
		  for(int atLoop=0;atLoop<formatedMessage->m_equipMsgCount;atLoop++)
		  {
           HandleEquipMessageClientSide(&equipArray[atLoop],entObjRuntime,missileRuntime);
		  }
		  delete [] equipArray;
		  equipArray = 0;
	   }//end equi batch coming in
	   if(formatedMessage->m_txtMsgCount > 0)
	   {//txt batch coming in
		  for(int txtLoop=0;txtLoop<formatedMessage->m_txtMsgCount;txtLoop++)
		  {//txt msg loop
             int txtMsgSz = blockCast[dataTracer];
			 dataTracer++;
			 if(txtMsgSz > 0)
			 {//valid sz
                BYTE * tempBlock = new BYTE[txtMsgSz];
                for(int i=0;i<txtMsgSz;i++)
				{
                    tempBlock[i] = blockCast[dataTracer];
			        dataTracer++;
				}
                HandleTextMessageClientSide(tempBlock,txtMsgSz,idFromLoc);
				delete [] tempBlock;
				tempBlock = 0;
			 }//end valid sz
		  }//end txt msg loop
	   }//end txt batch coming in
	   if(formatedMessage->m_updateStatMsgCount > 0)
	   {//statistic batch coming in
		  int statBlockSize = blockCast[dataTracer];
		  dataTracer++;
		  if(statBlockSize > 0)
		  {
	       BYTE * statUpdateBlock = new BYTE[statBlockSize];		  
		   for(int i=0;i<statBlockSize;i++)
		   {
              statUpdateBlock[i] = blockCast[dataTracer];
			  dataTracer++;
		   }
		   HandleClientStatBlockData(statUpdateBlock,statBlockSize,idFromLoc);
		  }
	   }//end statistic batch coming in
	   if(formatedMessage->m_miscMsgCount > 0)
	   {//misc batch coming in
		 for(int miscLoop=0;miscLoop<formatedMessage->m_miscMsgCount;miscLoop++)
		 {
		  BYTE castByteTemp[2];
          castByteTemp[0] = blockCast[dataTracer];
		  dataTracer++;
		  castByteTemp[1] = blockCast[dataTracer];
		  dataTracer++;
		  short miscBlockSize = 0;
		  CopyMemory(&miscBlockSize,castByteTemp,sizeof(short));
		  //int miscBlockSize = blockCast[dataTracer];
		  //dataTracer++;
		  if(miscBlockSize > 0)
		  {//1
	       /*BYTE * miscUpdateBlock = new BYTE[miscBlockSize];		  
		   for(int i=0;i<miscBlockSize;i++)
		   {
              miscUpdateBlock[i] = blockCast[dataTracer];
			  dataTracer++;
		   }*/
			

		   HandleMiscBlocksClientSide(&blockCast[dataTracer],miscBlockSize,entObjRuntime);
		   dataTracer += miscBlockSize;
		   /*if(miscUpdateBlock)
			  delete [] miscUpdateBlock;
		      miscUpdateBlock = 0;*/
		  }//end 1
		 }
	   }//end misc batch coming in

	   delete [] blockCast;
	   blockCast=0;

       return TRUE;
	}//end recieve gurenteed block data
	if(identityMsg->m_catagorie == 40)
	{//dynamic server ai spawn

	   LPAIDYNSPAWN_POS formattedMsg = (LPAIDYNSPAWN_POS)lpMessage;
	   
	   
       if(formattedMsg->m_spawnArrayCount > 0)
	   {//create spawn array

		   D3DVECTOR offsetPos,dynPos;
		   offsetPos.x = 0.0f;
		   offsetPos.y = 0.0f;
		   offsetPos.z = (float)formattedMsg->m_spawnArrayOffset;
		   float roatationPer = 360.0f / (float)formattedMsg->m_spawnArrayCount;
		   float currentRotate = 0.0f;
           for(int pcLoop=0;pcLoop<formattedMsg->m_spawnArrayCount;pcLoop++)
		   {
			   currentRotate = roatationPer * (float)pcLoop;
               dynPos = m_gStatus->m_matLib.RotateY_WLD ( currentRotate, offsetPos);

			   dynPos.x += formattedMsg->m_xPos;
			   dynPos.y += formattedMsg->m_yPos;
			   dynPos.z += formattedMsg->m_zPos;
			   if(!SpawnEntityBasedOnPositionAndCfg(dynPos,
												formattedMsg->m_aiCfg,
												formattedMsg->m_dynaID,
												formattedMsg->m_lifeLine,
												entObjRuntime,
												formattedMsg->m_clanID,
												collisionBase,
												collisionDatabase,
												formattedMsg->m_dynaIDMaxSpawn,
												formattedMsg->worldName,
												formattedMsg->m_spawnRotationRange,
												formattedMsg->m_worldChannel))
			   {
					m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("SpawnEntityBasedOnPositionAndCfg() Array Failed");
			   }
		   }
	   }//end create spawn array
	   else
	   {//simple single spawn
		   D3DVECTOR currentPosCnv;
           currentPosCnv.x = formattedMsg->m_xPos;
		   currentPosCnv.y = formattedMsg->m_yPos;
		   currentPosCnv.z = formattedMsg->m_zPos;
		   if(!SpawnEntityBasedOnPositionAndCfg( currentPosCnv,
												formattedMsg->m_aiCfg,
												formattedMsg->m_dynaID,
												formattedMsg->m_lifeLine,
												entObjRuntime,
												formattedMsg->m_clanID,
												collisionBase,
												collisionDatabase,
												formattedMsg->m_dynaIDMaxSpawn,
												formattedMsg->worldName,
												formattedMsg->m_spawnRotationRange,
												formattedMsg->m_worldChannel))
		   {
			   m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("SpawnEntityBasedOnPositionAndCfg() Failed");
		   }
	   }//end simple single spawn


	   delete formattedMsg;
	   formattedMsg=0;	
	   return TRUE;
	}//end dynamic server controlled ai spawn
	if(identityMsg->m_catagorie == 41)
	{//character list for a game or any list on a menu
       BYTE * data = (BYTE*)lpMessage;
	   POSITION tempPosition = NULL;
       
       for(int cLoop=0;cLoop<data[1];cLoop++)
	   {//update menu control by ID
           char buffer[25];
	       ZeroMemory(buffer,25);
		   CopyMemory(buffer,&data[3+(25*cLoop)],25);
		   
		   tempPosition = m_gStatus->AddStringToMenuList(data[2],buffer,-1,0,0,buffer,0,"");
	   }//end update menu control by ID

	   delete [] data;
	   data=0;

	   //Cleanup the Attribute Que List
	   if (tempPosition)
	   {
			CAttribQueueEntry * curEntry = dynamic_cast<CAttribQueueEntry*>(m_gStatus->m_attribQueue->GetAt(tempPosition));
			if(curEntry)
			{
	   			delete curEntry;
				curEntry = 0;
			}
			m_gStatus->m_attribQueue->RemoveAt(tempPosition);
	   }

	   return TRUE;
	}//end character list for a game

	if(identityMsg->m_catagorie == 42)
	{//Handle Group information client side
	   delete [] (BYTE*)lpMessage;//nothing uses it yet - free it
	   return TRUE;
	}//End handle group information client side
    if(identityMsg->m_catagorie == 43)
	{//recieve skill title list
		BYTE *byteBlockData = (BYTE*)lpMessage;
		HandleSkillTitlesOutputClientSide(byteBlockData, msgSize, idFromLoc);
		delete [] byteBlockData;
		return TRUE;
	}//end recieve skill title list

	m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Unknown catagorie ","byte:",identityMsg->m_catagorie);
	{
	   static const int known[] = {2,3,5,6,7,8,10,11,12,18,19,20,21,24,25,26,28,29,30,31,32,33,34,35,36,37,38,40,41,42,43};
	   bool isKnown = false;
	   for(int k = 0; k < (int)(sizeof(known)/sizeof(known[0])); k++)
	      if(identityMsg->m_catagorie == known[k]) isKnown = true;
	   if(!isKnown)
	      delete [] (BYTE*)lpMessage;//unknown packet - nobody else will free it
	}
	//disconeect the mofo
	
	return FALSE;
}
//--------------non guarenteed msgs-----------------------------------------------//
BOOL CClientNetworkingLogic::HandleClientMovBlockData(LPVOID lpMessage,
													  int & msgSize,
													  DPID & idFromLoc,
													  CMovementObjList * entObjRuntime,
													  CMissileObjList  * missileRuntime,
													  CExplosionObjList * explosionRuntimeList)
{
		BYTE * dataBlock = (BYTE *)lpMessage;
		LPENTITYPOS_HEADER header = (LPENTITYPOS_HEADER)lpMessage;  
		int headerSize = sizeof(ENTITYPOS_HEADER);
		int movStructSize = sizeof(SHRUNK_MOVE_STRUCT);
		if(m_gStatus->m_multiplayerObject->m_requestingPortal)
		{//failed
		  delete [] dataBlock;
		  dataBlock=0;
		  return TRUE;
		}//end failed*/

		int currentCfgRequestCnt = 0;
		int indexInto = headerSize;

		STATICENTITY_DATA * staticData                  = NULL;
		FIRE_STRUCT * fireData                          = NULL;
		MSGHANDLER *  msgData                           = NULL;
		DAMAGEINFO_DATA * dmgData                       = NULL;
		DETAILEDMSG_DATA * detMsgData                   = NULL;
		GENERICCONFIG_STRUCT * surroundingHousesMsgData = NULL;
		CMovementObj * mPtr								= NULL;
		MOVE_STRUCT expandedStruct;

		for(int msgParse = 0;msgParse < header->m_entPosCount;msgParse++)
		{//parse messages
		 m_gStatus->m_multiplayerObject->ExpandData((SHRUNK_MOVE_STRUCT*)&dataBlock[indexInto],&expandedStruct);
		 UpdateNetworkEntity(&expandedStruct,FALSE,currentCfgRequestCnt,entObjRuntime);
		 indexInto += sizeof(SHRUNK_MOVE_STRUCT);
		}//end parse messages

		for(int msgParse = 0;msgParse < header->m_staticCount;msgParse++)
		{//parse messages
         staticData = (STATICENTITY_DATA *)&dataBlock[indexInto];
		 indexInto += sizeof(STATICENTITY_DATA);

		 if(staticData->m_networAssignedID < 0)
			mPtr = entObjRuntime->GetItemByNetworkID(abs(staticData->m_networAssignedID));
		 else
			mPtr = entObjRuntime->GetObjByNetworkID(staticData->m_networAssignedID);
				
		 if(mPtr)
		 {//already here refresh
			mPtr->m_validThisRound = TRUE;
			if(mPtr->m_posses)
			   mPtr->m_posses->m_validThisRound = TRUE;
		 }//end already here refresh
		 else if(currentCfgRequestCnt < m_gStatus->m_multiplayerObject->m_maxCfgRequestsPerRound)
		 {//inquire about this seed
		   currentCfgRequestCnt++; 

		   if(staticData->m_networAssignedID < 0)
		   {//send item data
			SendAttribMsgCP(4,
							abs(staticData->m_networAssignedID),
	 						0,
							0,
							0);
		   }//end send item data
		   else{//send static entity data
			SendAttribMsgCP(1,
							0,
	 						staticData->m_networAssignedID,
							staticData->m_networAssignedID,
							FALSE);
		   }//end send static entity data
		 }//end inquire about this seed
		}//end parse messages

       if(header->m_fireMsgCount > 0)
	   {
	      fireData = new FIRE_STRUCT[header->m_fireMsgCount];
	      int byteCount = (header->m_fireMsgCount*sizeof(FIRE_STRUCT));
          BYTE * conversionBlock = (BYTE *)fireData;
          for(int shiftLoop=0;shiftLoop<byteCount;shiftLoop++)
	          conversionBlock[shiftLoop] = dataBlock[shiftLoop+indexInto];
          indexInto += byteCount;
	   }

	   if(header->m_damageInfoMsgCount > 0)
	   {
	      dmgData = new DAMAGEINFO_DATA[header->m_damageInfoMsgCount];
	      int byteCount = (header->m_damageInfoMsgCount*sizeof(DAMAGEINFO_DATA));
          BYTE * conversionBlock = (BYTE *)dmgData;
          for(int shiftLoop=0;shiftLoop<byteCount;shiftLoop++)
	          conversionBlock[shiftLoop] = dataBlock[shiftLoop+indexInto];
          indexInto += byteCount;
	   }

	   if(header->m_generalMsgCount > 0)
	   {
	      msgData = new MSGHANDLER[header->m_generalMsgCount];
	      int byteCount = (header->m_generalMsgCount*sizeof(MSGHANDLER));
          BYTE * conversionBlock = (BYTE *)msgData;
          for(int shiftLoop=0;shiftLoop<byteCount;shiftLoop++)
	          conversionBlock[shiftLoop] = dataBlock[shiftLoop+indexInto];
          indexInto += byteCount;
	   }

	   if(header->m_detailedMsgCount > 0)
	   {
	      detMsgData = new DETAILEDMSG_DATA[header->m_detailedMsgCount];
	      int byteCount = (header->m_detailedMsgCount*sizeof(DETAILEDMSG_DATA));
          BYTE * conversionBlock = (BYTE *)detMsgData;
          for(int shiftLoop=0;shiftLoop<byteCount;shiftLoop++)
	          conversionBlock[shiftLoop] = dataBlock[shiftLoop+indexInto];
          indexInto += byteCount;
	   }

       if(header->m_itemCfgMsgCount > 0)
	   {//item cfgs
		  
		  for(int itemLoop=0;itemLoop<header->m_itemCfgMsgCount;itemLoop++)
		  {
              int dataBlockSz = dataBlock[indexInto];//UPGRADE****
              indexInto++;
 
			  if(dataBlockSz > 0)
			  {
				 HandleItemLocationalMessagesClientSide(&dataBlock[indexInto],entObjRuntime,missileRuntime,dataBlockSz);
				 indexInto += dataBlockSz;
			  }
		  }
	   }//end item cfgs

       if(header->m_entitCfgMsgCount > 0)
	   {//entity cfgs
		  for(int itemLoop=0;itemLoop<header->m_entitCfgMsgCount;itemLoop++)
		  {
              int dataBlockSz = dataBlock[indexInto];
              indexInto++;
 
			  if(dataBlockSz > 0)
			  {
			     BYTE * byteBlock = new BYTE[dataBlockSz];
                 for(int i=0;i<dataBlockSz;i++)
				 {
                     byteBlock[i] = dataBlock[indexInto];
                     indexInto++;
				 }
                 HandleEntityCfgBlock(byteBlock,dataBlockSz,idFromLoc,entObjRuntime,missileRuntime);

				 delete [] byteBlock;
				 byteBlock = 0;
			  }
		  }
	   }//end entity cfgs

	   if(header->m_houseMsgCount > 0)
	   {
		  surroundingHousesMsgData = new GENERICCONFIG_STRUCT[header->m_houseMsgCount];
		  int byteCount = (header->m_houseMsgCount*sizeof(GENERICCONFIG_STRUCT));
          BYTE * conversionBlock = (BYTE *)surroundingHousesMsgData;
          for(int shiftLoop=0;shiftLoop<byteCount;shiftLoop++)
			  conversionBlock[shiftLoop] = dataBlock[shiftLoop+indexInto];

		  int requestCfgCount = 0;
          for(int i=0;i<header->m_houseMsgCount;i++)
		  {
              if(!m_gStatus->m_houseingCache->FlagValidByAcID(surroundingHousesMsgData[i].m_acID,surroundingHousesMsgData[i].m_open))
			  {
				 if(requestCfgCount<10)
				    SendAttribMsgCP(38,
				                    surroundingHousesMsgData[i].m_acID,
				 	                0,
						            0,
						            0);

				 requestCfgCount++;
			  }
		  }

          delete [] surroundingHousesMsgData;
		  surroundingHousesMsgData = 0;
		  indexInto += byteCount; 
	   }
       
	   if(fireData)
	   {//parse fire msgs
		  for(int floop=0;floop<header->m_fireMsgCount;floop++)
              HandleFireMessagesClientSide(&fireData[floop],missileRuntime);

          if(fireData)
			 delete [] fireData;
		     fireData = 0;
	   }//end parse fire msgs

	   if(dmgData)
	   {//parse dmg msgs
          for(int floop=0;floop<header->m_damageInfoMsgCount;floop++)
              HandleDmgMsgHandler(&dmgData[floop]);

          if(dmgData)
			 delete [] dmgData;
		     dmgData = 0;
	   }//end parse dmg msgs

	   if(msgData)
	   {//parse dmg msgs
          for(int floop=0;floop<header->m_generalMsgCount;floop++)
              HandleGeneralMsgHandler(&msgData[floop]);

          if(msgData)
			 delete [] msgData;
		     msgData = 0;
	   }//end parse dmg msgs

	   if(detMsgData)
	   {//parse det msgs
          for(int floop=0;floop<header->m_detailedMsgCount;floop++)
              HandleDetailedMsgHandler(&detMsgData[floop],entObjRuntime,missileRuntime,explosionRuntimeList);

          if(detMsgData)
			 delete [] detMsgData;
		     detMsgData = 0;
	   }//end parse det msgs

       SendPendingCfgRequestsCP();//send all pending cfg requests
	
 
    delete [] dataBlock;
    dataBlock=0;
	return TRUE;
}
//--------------non guarenteed msgs-----------------------------------------------//
BOOL CClientNetworkingLogic::HandleAIMovBlockData(LPVOID lpMessage,
											      int & msgSize,
												  DPID & idFromLoc,
												  CMovementObjList * entObjRuntime,
												  CMissileObjList  * missileRuntime,
												  CExplosionObjList * explosionRuntimeList,
												  CSpawnObjList * spawnGeneratorDB,
												  int worldChannelRef)
{
	
    BYTE * dataBlock = (BYTE *)lpMessage;
    LPAIPOS_HEADER header = (LPAIPOS_HEADER)lpMessage;  
	int headerSize = sizeof(AIPOS_HEADER);
    int movStructSize = sizeof(SHRUNK_MOVE_STRUCT);   
	int indexInto = headerSize;

        //SHRUNK_MOVE_STRUCT * movBlockData = NULL;
	    int currentCfgRequestCnt = 0;
	    MOVE_STRUCT expandedStruct;
	    int shiftSize = sizeof(SHRUNK_MOVE_STRUCT);
	   
		for(int posLoop = 0;posLoop < header->m_entPosCount;posLoop++)
		{
	        m_gStatus->m_multiplayerObject->ExpandData((SHRUNK_LPMOVE_STRUCT)&dataBlock[indexInto],&expandedStruct);
		    UpdateNetworkEntity(&expandedStruct,FALSE,currentCfgRequestCnt,entObjRuntime);
		
		    indexInto += shiftSize;
        }

       if(header->m_entitCfgMsgCount > 0)
	   {//entity cfgs
		  for(int itemLoop=0;itemLoop<header->m_entitCfgMsgCount;itemLoop++)
		  {
              int dataBlockSz = dataBlock[indexInto];
              indexInto++;
 
			  if(dataBlockSz > 0)
			  {
				 HandleEntityCfgBlock(&dataBlock[indexInto],dataBlockSz,idFromLoc,entObjRuntime,missileRuntime);
				 indexInto+=dataBlockSz;
			  }
		  }
		  //m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Receiving entity cfg, cnt:",(DWORD)header->m_entitCfgMsgCount);//REMOVE
		  //m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("World Channel:",(DWORD)header->m_worldChannel);//Remove
	   }//end entity cfgs

	   if(header->m_damageMsgCount > 0)
	   {//entity cfgs
		  for(int itemLoop=0;itemLoop<header->m_damageMsgCount;itemLoop++)
		  {
              int dataBlockSz = dataBlock[indexInto];
              indexInto++;
 
			  if(dataBlockSz > 0)
			  {
				 //HandleEntityCfgBlock(&dataBlock[indexInto],dataBlockSz,idFromLoc,entObjRuntime,missileRuntime);
				 HandleAttribMessageClientSide((LPATTRIB_STRUCT)&dataBlock[indexInto],idFromLoc,spawnGeneratorDB,entObjRuntime,worldChannelRef);
				 indexInto+=dataBlockSz;
			  }
		  }
		  //m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Receiving entity attrib, cnt:",(DWORD)header->m_damageMsgCount);//REMOVE
		  //m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("World Channel:",(DWORD)header->m_worldChannel);
	   }//end entity cfgs
       shiftSize = sizeof(DETAILEDMSG_DATA);
	   for(int floop=0;floop<header->m_detailedMsgCount;floop++)
	   {
            HandleDetailedMsgHandler((LPDETAILEDMSG_DATA)&dataBlock[indexInto],entObjRuntime,missileRuntime,explosionRuntimeList);

			indexInto+=shiftSize;
	   }
       //SendPendingCfgRequestsCP();//send all pending cfg requests
	
    delete [] dataBlock;
    dataBlock=0;
	return TRUE;
}
//----------------------------------------------------------------------------------//
void CClientNetworkingLogic::RemovePlayerRuntimeListByID(DPNID idFromLoc)
{
  int indexCount = 0;
  POSITION posLast;
  
  for(POSITION posLoc = m_gStatus->m_entObjRuntime->GetHeadPosition();(posLast = posLoc) != NULL;)
  {
	     CMovementObj* ptrLocal = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetNext(posLoc);
         if(ptrLocal->List_idFromLoc == idFromLoc)
		 {
		    m_gStatus->m_entObjRuntime->RemoveAt(posLast);
            m_gStatus->DestroyRuntimeEntity(ptrLocal,NULL,m_gStatus->m_entObjRuntime);
			ptrLocal=0;
			break;
		 }
		 indexCount++;
  }
  //InListRuntimeEntities(indexCount-1);
}
//------------------------------------------------------------------------------------//
BOOL CClientNetworkingLogic::GetRuntimeObjectFromID(DPNID idFromLoc,CMovementObj ** objRet)
{
  int indexCount = 0;
  POSITION posLast;
  
  for(POSITION posLoc = m_gStatus->m_entObjRuntime->GetHeadPosition();(posLast = posLoc) != NULL;)
  {
	     CMovementObj* ptrLocal = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetNext(posLoc);
         if(ptrLocal->List_idFromLoc == idFromLoc)
		 {
            *objRet = ptrLocal;
			return TRUE;
		 }
  }
return FALSE;
}
//------------------------------------------------------------------------------------//
BOOL CClientNetworkingLogic::GetRuntimeObjectFromNetworkID(int networkIDTrace,CMovementObj ** objRet)
{
  int indexCount = 0;  
  for(POSITION posLoc = m_gStatus->m_entObjRuntime->GetHeadPosition();posLoc != NULL;)
  {
	     CMovementObj* ptrLocal = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetNext(posLoc);
         if(ptrLocal->List_networkDefinedID == networkIDTrace){
            *objRet = ptrLocal;
			return TRUE;
		 }
  }
return FALSE;
}
//-------------------------------------------------//
BOOL CClientNetworkingLogic::SendAttribMsgCP(   int attribute,
												int uniInt,
												long refrenceID,
												int uniInt2,
												short uniShort3) 
{
    LPATTRIB_STRUCT lpChatMessage = NULL;
	DWORD				messageSize;
	messageSize = sizeof(ATTRIB_STRUCT);
    lpChatMessage = (LPATTRIB_STRUCT) new BYTE[messageSize];

	if (lpChatMessage == NULL)
	{
		return FALSE;
	}

	lpChatMessage->m_catagorie = 6;
	lpChatMessage->attribute   = attribute;
	lpChatMessage->uniInt      = uniInt;
    lpChatMessage->idRefrence  = (short)refrenceID;
	lpChatMessage->uniInt2     = uniInt2;
	lpChatMessage->uniShort3   = uniShort3;
	lpChatMessage->CRCcheck    = m_gStatus->m_multiplayerObject->GetAttribCRC(lpChatMessage);
    
	m_gStatus->m_multiplayerObject->m_clientCfgRequestOutbox->AddPacket(messageSize,(BYTE*)lpChatMessage,0,0);

	if (lpChatMessage){
		delete lpChatMessage;
		lpChatMessage=0;
	}

	return TRUE;
}
//----------------------Stamp Network Mark--------------------------------------------//
void CClientNetworkingLogic::StampNetworkMark(CMovementObj * ptrLoc)
{
	  ptrLoc->List_networkInterpCurStep = 0;
	  ptrLoc->List_networkInterpCurStepOri = 0;
  
	  DWORD lastUpdatedTime = ptrLoc->List_NetworkUpdateTime;
	  ptrLoc->List_NetworkUpdateTime = m_gStatus->GetTimeStamp_DW();
	  ptrLoc->List_NetworkInterpolateVar = m_gStatus->GetElaspeTimeMS_DW(lastUpdatedTime);

	  if(ptrLoc->List_NetworkInterpolateVar > 3000)
	  {//interpolation epsilon
         ptrLoc->List_NetworkInterpolateVar = 3000;
	  }//end interpolation epsilon
}
//--------------Manage smooth movement of network entities :))))----------------------//
void CClientNetworkingLogic::ManageSmoothMovementOnNetworkEntities(DWORD CurrentTickCount,CMovementObjList * entObjRuntime)
{
//CurrentTickCount = GetTickCount();
for(POSITION posEnt = entObjRuntime->GetHeadPosition();posEnt!=NULL;){
   CMovementObj * ptrLoc = ( CMovementObj* ) entObjRuntime->GetNext(posEnt);
   if(ptrLoc->List_controlType == -1)
   if(ptrLoc->List_networkEntity == TRUE){
	  ptrLoc->List_networkInterpCurStep = m_gStatus->GetElaspeTimeMS_DW(ptrLoc->List_NetworkUpdateTime);
	   
      
	  if(ptrLoc->List_networkInterpCurStep > ptrLoc->List_NetworkInterpolateVar)
	     ptrLoc->List_networkInterpCurStep = ptrLoc->List_NetworkInterpolateVar;
      
      
	   D3DVECTOR newLocation = m_gStatus->m_matLib.InterpolatePointsX(   ptrLoc->List_networkLastPositionXYZ, 
																		 ptrLoc->List_networkTargetPositionXYZ,
																		 (double)ptrLoc->List_NetworkInterpolateVar, 
																		 (double)ptrLoc->List_networkInterpCurStep);

	  
 	  //POSITION
	  ptrLoc->List_baseFrame->SetPosition2(NULL,newLocation.x,newLocation.y,newLocation.z);
	  //ORIENTATION
	  int interpolateAmount = 12;//change interp for turning
	  if(ptrLoc->List_networkInterpCurStepOri > interpolateAmount)
	     ptrLoc->List_networkInterpCurStepOri = interpolateAmount;

       D3DVECTOR NewDirVect = m_gStatus->m_matLib.InterpolatePointsX(
		                                        ptrLoc->List_networkLastDirVect, 
						                        ptrLoc->List_networkTargetDirVect,
					                            (double)ptrLoc->List_NetworkInterpolateVar, 
					                            (double)ptrLoc->List_networkInterpCurStep);
	  D3DVECTOR NewUpVect = m_gStatus->m_matLib.InterpolatePointsX(
		                                        ptrLoc->List_networkLastUpVect, 
						                        ptrLoc->List_networkTargetUpVect,
					                            (double)ptrLoc->List_NetworkInterpolateVar, 
					                            (double)ptrLoc->List_networkInterpCurStep);
	  
	
      NewUpVect = m_gStatus->m_matLib.Normalize(NewUpVect);
	  NewDirVect = m_gStatus->m_matLib.Normalize(NewDirVect);
	                    
      ptrLoc->List_baseFrame->SetOrientation(NewDirVect,NewUpVect);
      //set free look joint stuff   mainly for mechas type
      D3DVECTOR posLast,posTarget;
      posLast.x = ptrLoc->List_networkFreeLookCurPitchLast;
	  posLast.y = ptrLoc->List_networkFreeLookCurYawLast;
      posLast.z = 0.0f;
	  posTarget.x = ptrLoc->List_networkFreeLookCurPitchTarget;
	  posTarget.y = ptrLoc->List_networkFreeLookCurYawTarget;
      posTarget.z = 0.0f;

	  
      D3DVECTOR NewVects = m_gStatus->m_matLib.InterpolatePointsX(posLast, 
						                                          posTarget,
					                                              (double)ptrLoc->List_NetworkInterpolateVar, 
					                                              (double)ptrLoc->List_networkInterpCurStep);

	  ptrLoc->List_skeletalObject->ApplyFreeLookRotation(NewVects.x,NewVects.y);
	  ptrLoc->List_networkInterpCurStepOri++;
   
   }
  }
}
//--------------Handles system messages-----------------------------------------------//
BOOL CClientNetworkingLogic::HandleSystemMessagesClientSide(LPVOID lpMessage,DPNID idFromLoc)
{
  LPDPMSG_GENERIC lpMsg = (LPDPMSG_GENERIC)lpMessage;
  LPSTR		lpszStr = NULL;
     if(lpMsg->dwType == DPSYS_DESTROYPLAYERORGROUP)
	 {//if client left
	    LPDPMSG_DESTROYPLAYERORGROUP	lp = (LPDPMSG_DESTROYPLAYERORGROUP)lpMsg;
	    LPSTR							lpszPlayerName;
	    //get pointer to player name
	    if (lp->dpnName.lpszShortNameA)
			lpszPlayerName = lp->dpnName.lpszShortNameA;
		else
			lpszPlayerName = "Unknown";

		
            CString temp = lpszPlayerName;
			temp = temp+" Disconnected";
			//m_multiPlayer.SendDlgItemMessage(IDC_LIST_TCPIP_Monitor,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)temp);	
			m_gStatus->m_multiplayerObject->m_eventLog->AddEvent(temp);
		
	 }//end if client left
     //if you lost connection shut down game
	 if(lpMsg->dwType == DPSYS_SESSIONLOST)
	 {
		   m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Lost Connection");
           //m_multiPlayer.SendDlgItemMessage(IDC_LIST_TCPIP_Monitor,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)"Lost Connection");		
	 }
     //end if you lost connection shut down game
	 if(lpMsg->dwType == DPSYS_CREATEPLAYERORGROUP){//if player joined
	    LPDPMSG_CREATEPLAYERORGROUP 	lp = (LPDPMSG_CREATEPLAYERORGROUP) lpMsg;
	    LPSTR							lpszPlayerName;
		//get pointer to player name
        if (lp->dpnName.lpszShortNameA)
		    lpszPlayerName = lp->dpnName.lpszShortNameA;
	    else
		    lpszPlayerName = "unknown";

		    CString temp = lpszPlayerName;
			temp = temp+" Entered";

			   m_gStatus->m_multiplayerObject->m_eventLog->AddEvent(temp);	
	 }//end if player joined
return TRUE;
}//end if system message
//-------------------------------------------------------------------------------------------------------
BOOL CClientNetworkingLogic::UpdateNetworkEntity(LPMOVE_STRUCT expandedStruct,BOOL isItem,int & currentRequestCount,CMovementObjList * entObjRuntime)
{
    //if(expandedStruct->dbIndex < 0)
	//  return FALSE;//bad value
 
	int controlType = -1;
	CMovementObj * mPtr = NULL;
	if(!isItem)
	mPtr = entObjRuntime->GetObjByNetworkID(expandedStruct->idRefrence);
    else{
	   mPtr = entObjRuntime->GetItemByNetworkID(expandedStruct->idRefrence);
       controlType = 35;//define network item
	}

	long currentTime = GetTickCount();
    if(!mPtr){//entity not found

           if(currentRequestCount > m_gStatus->m_multiplayerObject->m_maxCfgRequestsPerRound)
              return TRUE;
		      currentRequestCount++; 
	      
		   if(!isItem)
		   SendAttribMsgCP(1,
				           0,
				 	       expandedStruct->idRefrence,
						   expandedStruct->idRefrence,
						   isItem);
		   else{//ask for item
		   SendAttribMsgCP(4,
				           expandedStruct->idRefrence,
				 	       0,
						   0,
						   0);
		   }
		   
    //end had to spawn request config here	
	 return TRUE;
	}//end entity not found
            CMovementObj * movBaseEnt = NULL;
			if(mPtr->m_posses)
			   movBaseEnt = mPtr->m_posses;
			else
			   movBaseEnt = mPtr;

            mPtr->List_networkPopoutCount = 0;//updated dont pop out
			mPtr->List_networkInterpCurStep = 0;  
	        mPtr->List_networkInterpCurStepOri = 0;
			movBaseEnt->List_networkPopoutCount = 0;//updated dont pop out
			movBaseEnt->List_networkInterpCurStep = 0;  
	        movBaseEnt->List_networkInterpCurStepOri = 0;
	        StampNetworkMark(mPtr);
			if(mPtr->m_posses){
			   StampNetworkMark(movBaseEnt);
			   movBaseEnt->m_validThisRound = TRUE;
			}

            mPtr->m_validThisRound                    = TRUE;
            movBaseEnt->List_baseFrame->GetPosition(NULL,&movBaseEnt->List_networkLastPositionXYZ);//AI server crash base frame is invalid at this point 
	        movBaseEnt->List_baseFrame->GetOrientation(&movBaseEnt->List_networkLastDirVect,&movBaseEnt->List_networkLastUpVect); 
	        mPtr->List_networkFreeLookCurYawLast = mPtr->List_skeletalObject->m_freeLookYaw;
	        mPtr->List_networkFreeLookCurPitchLast = mPtr->List_skeletalObject->m_freeLookPitch;

			movBaseEnt->List_networkTargetPositionXYZ.x     = expandedStruct->Xpos;
	        movBaseEnt->List_networkTargetPositionXYZ.y     = expandedStruct->Ypos;
	        movBaseEnt->List_networkTargetPositionXYZ.z     = expandedStruct->Zpos;
            movBaseEnt->List_networkTargetDirVect.x         = expandedStruct->OrientateDirX;
	        movBaseEnt->List_networkTargetDirVect.y         = expandedStruct->OrientateDirY;
	        movBaseEnt->List_networkTargetDirVect.z         = expandedStruct->OrientateDirZ;
	        movBaseEnt->List_networkTargetUpVect.x          = expandedStruct->OrientateUpX;
            movBaseEnt->List_networkTargetUpVect.y          = expandedStruct->OrientateUpY;
            movBaseEnt->List_networkTargetUpVect.z          = expandedStruct->OrientateUpZ;
			mPtr->List_networkFreeLookCurYawTarget          = expandedStruct->FreeLookYaw;
	        mPtr->List_networkFreeLookCurPitchTarget        = expandedStruct->FreeLookPitch;

			//standBounds
			BOOL calcNewAnim = TRUE;
			if(mPtr->List_skeletalObject->GetAnimationAttribByIndex(abs(expandedStruct->curAnim)) == 1)
			{//stand req (dont swicth till hes about stopped
				if(m_gStatus->m_matLib.MagnitudeSquared(movBaseEnt->List_networkLastPositionXYZ.x,
					                                                movBaseEnt->List_networkLastPositionXYZ.y,
													                movBaseEnt->List_networkLastPositionXYZ.z,
													                movBaseEnt->List_networkTargetPositionXYZ.x,
					                                                movBaseEnt->List_networkTargetPositionXYZ.y,
													                movBaseEnt->List_networkTargetPositionXYZ.z)>.6f)
				{
                   calcNewAnim = FALSE;
				}
			}//end stand req
			
			
            if(expandedStruct->secondaryAnim != -1)
            if(mPtr->List_skeletalObject->m_secondaryAnimationInUse != expandedStruct->secondaryAnim)
			{
			   mPtr->List_skeletalObject->m_secondaryAnimationInUse = expandedStruct->secondaryAnim;
               mPtr->SetSecondaryTimingIntro(mPtr->List_skeletalObject,currentTime);
			   mPtr->List_skeletalObject->m_lastScndDurationUpdateTime = currentTime;
			}

			mPtr->List_skeletalObject->m_secondaryAnimationInUse = expandedStruct->secondaryAnim;

            


			if(mPtr->m_posses)
			{
		       mPtr->List_skeletalObject->SetAnimationBasedOnAttrib(1,FALSE,0,100);
			}
			
			if(calcNewAnim)
			if(movBaseEnt->List_skeletalObject->SetCurrentAnimation(abs(expandedStruct->curAnim),100))
			{

			if(expandedStruct->curAnim < 0)//reverse animation
			   movBaseEnt->List_skeletalObject->SetAnimationDirection(abs(expandedStruct->curAnim),TRUE);
			else
			   movBaseEnt->List_skeletalObject->SetAnimationDirection(abs(expandedStruct->curAnim),FALSE);
		
		       movBaseEnt->List_skeletalObject->m_lastDurationUpdateTime = currentTime;
			}  
                
			   int version=0;int attrib=0;
			   int secondaryVersion=0;int secondaryAttrib=0;
			   if(mPtr->List_armedInventoryDB){
				   mPtr->List_skeletalObject->GetAnimationStatus(&version,&attrib,abs(expandedStruct->curAnim));
				   mPtr->List_skeletalObject->GetAnimationStatus(&secondaryVersion,&secondaryAttrib,abs(expandedStruct->secondaryAnim));
				   for(POSITION posArm = mPtr->List_armedInventoryDB->GetHeadPosition();posArm!= NULL;){
					   CArmedInventoryObj * armPtr = (CArmedInventoryObj *)mPtr->List_armedInventoryDB->GetNext(posArm);
                       if(armPtr->m_thirdPersonSystem){
						   int indexer = armPtr->m_thirdPersonSystem->GetAnimationIndexFromAttrib(attrib);
			               int secondaryIndexer = armPtr->m_thirdPersonSystem->GetAnimationIndexFromAttrib(secondaryAttrib);
						   
						   if(expandedStruct->secondaryAnim != -1){//valid secondary
						   if(armPtr->m_thirdPersonSystem->m_secondaryAnimationInUse != secondaryIndexer)
						   {
							  armPtr->m_thirdPersonSystem->m_secondaryAnimationInUse = secondaryIndexer;
                              mPtr->SetSecondaryTimingIntro(armPtr->m_thirdPersonSystem,currentTime);
							  armPtr->m_thirdPersonSystem->m_lastScndDurationUpdateTime = currentTime;
						   }	   
						   armPtr->m_thirdPersonSystem->m_secondaryAnimationInUse = secondaryIndexer;
						   }//end valid secondary
						   else
						   armPtr->m_thirdPersonSystem->m_secondaryAnimationInUse = expandedStruct->secondaryAnim;
						   
						   if(!armPtr->m_thirdPersonSystem->SetCurrentAnimation(abs(indexer),100))
							   continue;
		                       armPtr->m_thirdPersonSystem->m_lastDurationUpdateTime = currentTime;
			
							   if(expandedStruct->curAnim < 0)
		                          armPtr->m_thirdPersonSystem->SetAnimationDirection(abs(indexer),TRUE); 
							   else
							      armPtr->m_thirdPersonSystem->SetAnimationDirection(abs(indexer),FALSE); 
					   }
				   }
			   }

  return TRUE;
}
//--------------Handles system messages-----------------------------------------------//
BOOL CClientNetworkingLogic::HandleAttribMessageClientSide(LPATTRIB_STRUCT formatedMessage,
											               DPNID idFromLoc,
														   CSpawnObjList * spawnGeneratorDB,
														   CMovementObjList * entObjRuntime,
														   int worldChannelRef)
{
	if(formatedMessage->attribute == 3)
	{//update on players health
		CMovementObj * attacker = NULL;
		if(formatedMessage->uniInt2 > -1)
			attacker = entObjRuntime->GetObjByNetworkID(formatedMessage->uniInt2);

		if(!attacker)
			m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("cannot locate attacker:",(DWORD)formatedMessage->uniInt2);

		CMovementObj * mPtr = entObjRuntime->GetObjByNetworkID(formatedMessage->idRefrence);
		if(mPtr)
		{//valid movement pointer
			int oldEnergy = mPtr->List_energy;
			mPtr->List_energy = formatedMessage->uniInt;
			mPtr->List_maxEnergy = formatedMessage->uniShort3;
		   
			if(oldEnergy > mPtr->List_energy)
			{//under attack
				if(!m_gStatus->m_multiplayerObject->m_AIServer)
				{
					m_gStatus->m_screenFlashModule->DoIt(500,m_gStatus->m_3dDevice);
                    m_gStatus->m_screenFlashModule->m_on = TRUE;
				}
				if(attacker)
				{//log attacker trace
					if(attacker->List_traceNumber != mPtr->List_traceNumber)
					{//dont aggro self
						mPtr->List_attackerTrace = attacker->List_traceNumber;
						if(mPtr->m_aiObj)
							mPtr->m_curHitCounter++;
					}//end dont aggro self
				}//end log attacker trace
			}//end under attack
		}//end valid movement pointer
		else
			m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("cannot find entity that took damage");
	}//end update on players health
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 7)
	{//dissipate entity that died 
	}//end dissipate entity that died
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 8)
	{//set network id for communication purposes
		//MessageBox("Communications set");
		CMovementObj * selfPtr = entObjRuntime->GetObjByTrace(formatedMessage->idRefrence);
		if(selfPtr)
		{
			entObjRuntime->SetNetID(selfPtr,formatedMessage->uniInt);
			m_gStatus->m_multiplayerObject->m_assignedIDGlobal = selfPtr->List_networkDefinedID;
			//request my start up cfgs
			m_gStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
															DPID_SERVERPLAYER,
															1,
															0,
		 													selfPtr->List_networkDefinedID,
															selfPtr->List_networkDefinedID,
															0,
															NULL);


			selfPtr->List_energy = formatedMessage->uniInt2;
			selfPtr->List_maxEnergy = formatedMessage->uniShort3;
			//m_gStatus->m_multiplayerObject->m_eventLog->AddEvent(" Spawned:" + selfPtr->List_name + " wldCh:"+m_gStatus->m_cabLib.ConvToString(worldChannelRef)+" Energy set:" + m_gStatus->m_cabLib.ConvToString(selfPtr->List_energy) + " Loc ID:" + m_gStatus->m_cabLib.ConvToString(selfPtr->List_traceNumber)+" NetID:",(DWORD)selfPtr->List_networkDefinedID);

			// INIT quick slots and key configs
			if(!m_bQuickSlotsVerified && !m_gStatus->m_multiplayerObject->m_AIServer)
			{
				// Toggle verification flag
				this->m_bQuickSlotsVerified = TRUE;

				// Send Attrib Message for Inventory List
				this->m_gStatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED,
					DPID_SERVERPLAYER,
					19,
					51,//Requested Attrib ID
					selfPtr->List_networkDefinedID,
					0,
					3, //Filter Type 0 = none, 1 = skills only, 2 = spells only, 3 = no skills / spells
					NULL);//request character inventory//*/

				// Init key configs for the player character
				CString strKeyConfigFile = KEY_CONFIG_FILE;
				strKeyConfigFile += ".";
				strKeyConfigFile += m_gStatus->m_cabLib.ConvToString(selfPtr->m_uniqueId);
				m_gStatus->LoadControlCfgFromDisk(strKeyConfigFile);
			}
			return TRUE;
		}

		m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Comm assignment did not find trace:"," wldCh:"+m_gStatus->m_cabLib.ConvToString(worldChannelRef)+" idRef ",formatedMessage->idRefrence);//output for ai server
	}//end set network id for communication purposes
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 9)
	{//ok to mount entity
		CMovementObj * mPtr = entObjRuntime->GetObjByNetworkID(formatedMessage->idRefrence);
		if(mPtr)
		{//valid
			POSITION posLoc = entObjRuntime->FindIndex(0);
			if(posLoc)
			{//user is here
				CMovementObj * hostPtr = ( CMovementObj* ) entObjRuntime->GetAt(posLoc);
				if(hostPtr->List_controlType == 0)
					//main player entity
					hostPtr->Mount(mPtr);
			}//end user is here
		}//end valid
	}//end ok to mount entity
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 10)
	{//delete entity
		POSITION posLast;
		for(POSITION posLoc = entObjRuntime->GetHeadPosition();(posLast = posLoc) != NULL;)
		{
			CMovementObj * mPtr = ( CMovementObj* ) entObjRuntime->GetNext(posLoc);
			if(formatedMessage->idRefrence == mPtr->List_networkDefinedID)
			{//found it
				m_gStatus->DestroyRuntimeEntity(mPtr,spawnGeneratorDB,posLast,entObjRuntime);
				return TRUE;
			}//end found it
		}
		m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("cannot locate target DELETE attrib:",(DWORD)10);
		return FALSE;
	}//end delete entity
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 11)
	{//demount entity
		POSITION posLoc = entObjRuntime->FindIndex(0);
		if(posLoc)
		{//user is here
			CMovementObj * hostPtr = ( CMovementObj* ) entObjRuntime->GetAt(posLoc);
			if(hostPtr->List_controlType == 0)
				//main player entity
				hostPtr->DeMount();
		}//end user is here
	}//end demount entity
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 12)
	{//tell entity (ai) target has been eliminated
		CMovementObj * mPtr = entObjRuntime->GetObjByNetworkID(formatedMessage->idRefrence);
		if(mPtr)
		{//valid		     
			if(mPtr->m_aiObj)
			{//ai
				mPtr->List_AI_target = -1;//set target to none
				mPtr->List_targetRef = NULL;
			}//end ai
		}//end valid
	}//end //tell entity (ai) target has been eliminated
	else if(formatedMessage->attribute == 15)
	{//change def cfg on entity
		CMovementObj * mPtr = entObjRuntime->GetObjByNetworkID(formatedMessage->idRefrence);
		if(mPtr)
		{//valid		     
			if(mPtr->List_skeletalObject->m_deformableMeshArrayCount <= formatedMessage->uniInt)
				return FALSE;

			CMovementObj * dbPtr = m_gStatus->m_entObjList->GetByID(mPtr->List_refModel);
			if(!dbPtr)
				return FALSE;

			if(!dbPtr->List_skeletalObject->HotSwapDim(formatedMessage->uniInt,formatedMessage->uniInt2,mPtr->List_skeletalObject,m_gStatus->m_3dDevice,TRUE))
				return FALSE;
		}//end valid
	}//end //change def cfg on entity
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 18)
	{//call store menu
		POSITION entPosition = entObjRuntime->FindIndex(0);
		if(entPosition)
		{//valid    
			CMovementObj * ptrLoc = ( CMovementObj* ) entObjRuntime->GetAt(entPosition);
	     
			m_gStatus->OpenMenu(formatedMessage->uniInt2, false);
		}//end valid
	}//end call store menu
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 19)
	{//update on cash amount
		m_gStatus->m_multiplayerObject->m_cashCount = formatedMessage->uniInt;
		m_gStatus->m_lastCashUpdate = GetTickCount();
	}//end update on cash amount
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 20)
	{//open a menu
		m_gStatus->OpenMenu(formatedMessage->uniInt2, false);
		if(formatedMessage->uniInt2 == TRADE_MENU_ID)
		{
			CMenuButton *pButton = (CMenuButton*)m_gStatus->m_runtimeMenuDatabase->GetVisualByID(TRADE_REQUEST_ID, true);
			CMenuImage *pImage = (CMenuImage*)m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ACCEPT_CHECK_ID, true);
			if(pButton)
				pButton->m_visible = FALSE;
			if(pImage)
				pImage->m_visible = FALSE;
		}
	}//end open a menu
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 21)
	{//close all menus
		m_gStatus->m_runtimeMenuDatabase->CloseAll(&m_gStatus->m_showMenuCursor);
	}//end close all menus
	else if(formatedMessage->attribute == 22)
	{//update on bank cash amount
		m_gStatus->m_multiplayerObject->m_bankCashCount = formatedMessage->uniInt;
	}//end update on bank cash amount
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 23)
	{//update time sync
		//m_gStatus->m_multiplayerObject->m_serverStartTime = GetTickCount()-formatedMessage->idRefrence;
	}//end update time sync
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 24)
	{//basic menu open
		if(formatedMessage->uniShort3 == -9)
		{//close all if specified
			for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
			{//1
				CMenu3DObj * menu = (CMenu3DObj *)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
				if(menu->m_closeOnEsc)
					menu->CloseMenu(&m_gStatus->m_globalCursorPos);
			}//end 1
		}//end close all if specified
		if(formatedMessage->uniInt != 0)
		{//trade window special instruction
			CMovementObj * entityPtr = entObjRuntime->GetObjByNetworkID(formatedMessage->uniInt);
			if(entityPtr)
				m_gStatus->m_multiplayerObject->m_currentlyTradingWith = entityPtr->List_name;
		}//end trade window special instruction
		m_gStatus->OpenMenu(formatedMessage->uniInt2, false);
	}//end basic menu open
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 25)
	{//close menu that is specified
		m_gStatus->m_runtimeMenuDataBase->CloseByID(formatedMessage->uniInt);
	}//end close menu that is specified
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 26)
	{//somone is offering 
		if(formatedMessage->idRefrence > -1)
		{ 
			m_gStatus->m_multiplayerObject->m_potentialGroupJoiner = formatedMessage->idRefrence;
			m_gStatus->OpenMenu(formatedMessage->uniInt, false); //m_gStatus->m_multiplayerObject->m_potentialGroupJoiner,0,0,FALSE);		     
			
			//Change the text to show who invited us
			CMovementObj *targetEntity = m_gStatus->m_entObjRuntime->GetObjByNetworkID(formatedMessage->idRefrence);

			// Open Group Invite menu for accept/decline
//			m_gStatus->OpenMenu(GROUP_INVITED_MENU_ID, false);
			CMenuText *pPlayer = (CMenuText*)m_gStatus->m_menuDatabase->GetVisualByID(PLAYER_NAME_TEXT_ID, true);
			//CMenuButton *pButton = (CMenuButton*)m_gStatus->m_runtimeMenuDatabase->GetVisualByID(GROUP_REQUEST_ID, true);
			CMenuImage *pImage = (CMenuImage*)m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ACCEPT_CHECK_ID, true);
			/*if(pButton)
				pButton->m_visible = TRUE;*/
			if(pImage)
				pImage->m_visible = TRUE;
			if(targetEntity)
			{
				pPlayer->SetText(targetEntity->List_name.GetBuffer());
				targetEntity->List_name.ReleaseBuffer();
			}
			else pPlayer->SetText("Unknown Player");
		}
	}//end close menu that is specified
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 27)
	{//set charges
		m_gStatus->m_multiplayerObject->m_itemGlobalMaxCharges   = formatedMessage->uniInt;
		m_gStatus->m_multiplayerObject->m_itemGlobalCurCharges   = formatedMessage->uniInt2;
		if(m_gStatus->m_multiplayerObject->m_itemGlobalCurCharges < 0)
			m_gStatus->m_multiplayerObject->m_itemGlobalCurCharges = 0;
		m_gStatus->m_multiplayerObject->m_itemNeededPerFire = formatedMessage->uniShort3;
	}//end set charges
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 28)
	{//set jets
		m_gStatus->gl_svrJt = TRUE;
		m_gStatus->gl_frceY = ((float)formatedMessage->uniInt)*.001f;
		m_gStatus->gl_frceZ = ((float)formatedMessage->uniInt2)*.001f;
	}//end set jets
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 30)
	{//set damage timer
		//This is the first pass and reset of the damage timer
		if(formatedMessage->uniShort3 == 0)
		{
			m_gStatus->m_multiplayerObject->m_CurBonusTimeDamage = formatedMessage->uniInt;
			m_gStatus->m_multiplayerObject->m_MaxBonusTimeDamage = formatedMessage->uniInt2;
			m_gStatus->m_multiplayerObject->m_DamageIcon = formatedMessage->idRefrence;
			m_gStatus->OpenMenu(204, FALSE); //TODO: Switch to using array of important menus
		}
		else //Only update based on the shot fired
			m_gStatus->m_multiplayerObject->m_CurBonusTimeDamage = formatedMessage->uniInt;
	}//end set damage timer
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 31)
	{//set stat timer
		int cur = formatedMessage->uniInt;
		int max = formatedMessage->uniInt2;
		int icon = formatedMessage->idRefrence;

		m_gStatus->m_multiplayerObject->m_CurBonusTimeStat = cur;
		m_gStatus->m_multiplayerObject->m_MaxBonusTimeStat = max;
		m_gStatus->m_multiplayerObject->m_StatIcon = icon;
		m_gStatus->m_multiplayerObject->m_StatTimeStamp = GetTickCount();
		m_gStatus->OpenMenu(207, FALSE); //TODO: Switch to using array of important menus
	}//end set stat timer
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 32)
	{//set resist timer
		int cur = formatedMessage->uniInt;
		int max = formatedMessage->uniInt2;
		int icon = formatedMessage->idRefrence;

		m_gStatus->m_multiplayerObject->m_CurBonusTimeResist = cur;
		m_gStatus->m_multiplayerObject->m_MaxBonusTimeResist = max;
		m_gStatus->m_multiplayerObject->m_ResistIcon = icon;
		m_gStatus->m_multiplayerObject->m_ResistTimeStamp = GetTickCount();
		m_gStatus->OpenMenu(203, FALSE); //TODO: Switch to using array of important menus
	}//end set resist timer
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 34)
	{//Update Murderer Count
		m_gStatus->m_multiplayerObject->m_currentMurderCount = formatedMessage->uniInt;
	}//End Update Murderer Count
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 35)
	{//Update for player Energy
		CMovementObj * mPtr = entObjRuntime->GetObjByNetworkID(formatedMessage->idRefrence);
		mPtr->m_energyCurrent -= formatedMessage->uniInt;
		if(mPtr->m_energyCurrent < 0.0f)
			mPtr->m_energyCurrent = 0.0f;
		else if(mPtr->m_energyCurrent > mPtr->m_energyMax)
			mPtr->m_energyCurrent = mPtr->m_energyMax;
	}//End Update player Energy
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 36)
	{//Test for changing animation
		long currentTime = GetTickCount();
		CMovementObj *entityPtr = entObjRuntime->GetObjByNetworkID(formatedMessage->idRefrence);
		entityPtr->m_attackAnimOverride = formatedMessage->uniInt;
		entityPtr->m_firstPersonAnimOverride = formatedMessage->uniInt2;

		CBoneAnimationObject* animPtrRet = NULL;
		entityPtr->List_skeletalObject->m_secondaryAnimationInUse = entityPtr->List_skeletalObject->GetAnimationBasedOnAttribAndPref(12,entityPtr->m_attackAnimOverride,&animPtrRet);

		entityPtr->SetSecondaryTimingIntro(entityPtr->List_skeletalObject,currentTime);
		if(animPtrRet)
			entityPtr->List_skeletalObject->m_scndDurationCurrentLength = animPtrRet->m_minPlayDuration;
		entityPtr->List_skeletalObject->m_lastScndDurationUpdateTime = currentTime;
	}//End test for changing animation
	//-----------------------------------------------------------
	else if(formatedMessage->attribute == 37)
	{//Swap out the gun projectile
		CMovementObj *mPtr = entObjRuntime->GetObjByNetworkID(formatedMessage->idRefrence);
		mPtr->ChangeMissileIndexByLoc(0, 12, formatedMessage->uniInt);
	}//End swap out the gun projectile
	else if(formatedMessage->attribute == 38)
	{
		CMovementObj *mPtr = entObjRuntime->GetObjByNetworkID(formatedMessage->idRefrence);
		mPtr->m_level = formatedMessage->uniShort3;
		mPtr->m_curXP = formatedMessage->uniInt;
		mPtr->m_nextLvlXP = formatedMessage->uniInt2;
	}
	else if(formatedMessage->attribute == 39)
	{
		CMovementObj *mPtr = entObjRuntime->GetObjByNetworkID(formatedMessage->idRefrence);
		mPtr->m_percXP = formatedMessage->uniInt;
	}
	else if(formatedMessage->attribute == 40)
	{
		CMenuButton *pButton = (CMenuButton*)m_gStatus->m_runtimeMenuDatabase->GetVisualByID(TRADE_REQUEST_ID, true);
		CMenuImage *pImage = (CMenuImage*)m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ACCEPT_CHECK_ID, true);
		if(pButton)
			pButton->m_visible = TRUE;
		if(pImage)
			pImage->m_visible = TRUE;
	}
	else if(formatedMessage->attribute == 41)
	{//set charges
		if(m_gStatus->m_multiplayerObject->m_rechargeTimer != formatedMessage->uniInt)
			m_gStatus->m_multiplayerObject->m_rechargeTimerStart = GetTickCount();
		m_gStatus->m_multiplayerObject->m_rechargeTimer			 = formatedMessage->uniInt;
	}//end set charges
	else if(formatedMessage->attribute == 42)
	{
		CMenuButton *pButton = (CMenuButton*)m_gStatus->m_runtimeMenuDatabase->GetVisualByID(CLAN_RECRUIT_NOTIFY_BUTTON, true);
		if(pButton)
			pButton->m_visible = TRUE;
	}
	//-----------------------------------------------------------
	else //A menu is waiting for a reply
	{
		//Find the queue entry that's waiting for this attribute message
		CAttribQueueEntry *curEntry;
		POSITION posLast, posQueue = m_gStatus->m_attribQueue->GetHeadPosition();
		while((posLast = posQueue) != NULL)
		{
			curEntry = (CAttribQueueEntry*)m_gStatus->m_attribQueue->GetNext(posQueue);
			if(curEntry->m_requestedAttrib == formatedMessage->attribute)
			{
				delete curEntry;
				curEntry = 0;
				m_gStatus->m_attribQueue->RemoveAt(posLast);
				break;
			}
		}

		//If we didn't find the entry, return
		if(posLast == NULL)
			return FALSE;

		if(formatedMessage->attribute == 14)
		{//confermation of deleteion of character
			DeleteStringFromMenuList(50,formatedMessage->uniInt);
		}//end confermation of deleteion of character
		//-----------------------------------------------------------
		else if(formatedMessage->attribute == 16)
		{//inventory list fill
			CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(formatedMessage->uniInt);
			if(invObj)
			{
				m_gStatus->AddStringToMenuList(formatedMessage->uniShort3,invObj->m_itemName,invObj->m_visualRef,formatedMessage->uniInt,0,invObj->m_itemName,0,"");
			}
			else
				m_gStatus->AddStringToMenuList(formatedMessage->uniShort3,"Unknown",-1,formatedMessage->uniInt,0,"Unknown",0,"");
		}//end inventory list fill
		//-----------------------------------------------------------
		else if(formatedMessage->attribute == 17)
		{//store list fill
			CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(formatedMessage->uniInt);
			if(invObj)
			{
				CString output = invObj->m_itemName+" cost: ";
				output = output+m_gStatus->m_cabLib.ConvToString(invObj->m_marketCost);
				m_gStatus->AddStringToMenuList(formatedMessage->uniShort3,output,invObj->m_visualRef,formatedMessage->uniInt,0,output,1,"");
			}
			else
				m_gStatus->AddStringToMenuList(formatedMessage->uniShort3,"Unknown",-1,formatedMessage->uniInt,0,"Unknown",1,"");
		}//end store list fill
		//-----------------------------------------------------------
		else if(formatedMessage->attribute == 21)
		{//loot menu list fill
			CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(formatedMessage->uniInt);
			if(invObj)
			{
				m_gStatus->AddStringToMenuList(formatedMessage->uniShort3,invObj->m_itemName,invObj->m_visualRef,formatedMessage->uniInt,0,invObj->m_itemName,0,"");
			}
			else
			{//alternate
				if(formatedMessage->uniInt == -50)
				{//cash
					CString output = operator+("CREDITS :",m_gStatus->m_cabLib.ConvToString(formatedMessage->uniInt2));
					m_gStatus->AddStringToMenuList(formatedMessage->uniShort3,output,-1,formatedMessage->uniInt,0,output,0,"");
				}//end cash
				else//ship!
					m_gStatus->AddStringToMenuList(formatedMessage->uniShort3,"Unknown",-1,formatedMessage->uniInt,0,"Unknown",0,"");
			}//end alternate
		}//end loot menu list fill
		//-----------------------------------------------------------
		else if(formatedMessage->attribute == 33)
		{//Update Character Screen
			if(formatedMessage->uniInt)
			{//Have Menu ID and Visual IDs to Update
				POSITION entPosition = entObjRuntime->FindIndex(0);
				if(entPosition)
				{//valid ent position
					CMovementObj * ptrLoc = ( CMovementObj* ) entObjRuntime->GetAt(entPosition);

					for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
					{//loop through menus
						CMenu3DObj* menu = (CMenu3DObj*)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
						if(menu->m_menuAccessID == formatedMessage->uniInt)
						{//Menu ID Match
							//Clear the Armed Inventory List
							ClearAllFromMenuList(formatedMessage->uniInt);

							//Request Updated Armed Inventory
							m_gStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED,
																			DPID_SERVERPLAYER,
																			49,
																			208,
																			ptrLoc->List_networkDefinedID,
																			0,
																			0,
																			NULL);

						}//End Menu ID Match
					}//end loop through menus
				}//End Valid Ent Position
			}//End Have Menu IDs to Update
		}//End Update Character Screen
	}//End Menu Waiting For Reply

	return TRUE;
}
//---used by client-------------------------------------------------------------------//
BOOL CClientNetworkingLogic::DeleteCharacter(int index)
{
if(!m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
    return FALSE;

    m_gStatus->m_netGameLogicObject->SendAttribMsg( DPNSEND_GUARANTEED,
													DPID_SERVERPLAYER,
													10,
													index,
													0,
													0,
													0,
													NULL);

	return TRUE;
}
//---used by client-------------------------------------------------------------------//
BOOL CClientNetworkingLogic::CreateCharacterOnServer(   int     DBindex,
														int     spwnCfg,
														CString characterName,
														CMenuVisObj * entityReference)
{ 
	if(!m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
    return FALSE;

	if(characterName.GetLength() == 0 || characterName.GetLength() > 25)
	{
		if(m_gStatus->m_systemReadout)
			m_gStatus->m_systemReadout->AddLine("Invalid Name selection");
		return FALSE;
	}


    CREATECHARHEADER_DATA header;
    header.m_catagorie         = 19;
    header.m_nameLength        = characterName.GetLength();
    header.m_spawnCfg          = spwnCfg;
    header.m_entityType        = DBindex;
    header.m_attachmentCount   = 0;
	header.m_defCfgCount       = entityReference->m_entityObjectLink->List_skeletalObject->m_deformableMeshArrayCount;
  
	//get size
	if(entityReference->m_entityObjectLink->m_itemDB)//update attachment
	 for(POSITION armedPos = entityReference->m_entityObjectLink->m_itemDB->GetHeadPosition();armedPos != NULL;)
	 {
        CItemObj * itemPtr = (CItemObj *)entityReference->m_entityObjectLink->m_itemDB->GetNext(armedPos);
	    if(itemPtr->m_equiped)//make sure hes wearin it    
		    header.m_attachmentCount++;
	 }


	int headerSize = sizeof(CREATECHARHEADER_DATA);
    int itemStructSize = sizeof(ITEMDATA_STRUCT);
	
	int totalSize = ((header.m_defCfgCount*itemStructSize) + (header.m_attachmentCount*itemStructSize)) + headerSize + header.m_nameLength;
    BYTE * dataBlock = new BYTE[totalSize];
	


    CopyMemory(dataBlock,&header,headerSize);



	for(int j=0;j<header.m_nameLength;j++)
        dataBlock[(j+headerSize)] = characterName.GetAt(j);

	if((header.m_defCfgCount+header.m_attachmentCount) > 0)
	{
	   ITEMDATA_STRUCT * itemDataStruct = new ITEMDATA_STRUCT[header.m_defCfgCount+header.m_attachmentCount];
       
	   //set data
	   int trace = 0;
	   if(entityReference->m_entityObjectLink->m_itemDB)
	   for(POSITION armedPos = entityReference->m_entityObjectLink->m_itemDB->GetHeadPosition();armedPos != NULL;)
	   {//update configuration/customized 
          CItemObj * itemPtr = (CItemObj *)entityReference->m_entityObjectLink->m_itemDB->GetNext(armedPos);
		  if(itemPtr->m_equiped){//make sure hes wearin it    
		     itemDataStruct[trace].m_GLID      = itemPtr->m_globalID;
		     itemDataStruct[trace].m_quantity   = itemPtr->m_locationID;
			 itemDataStruct[trace].m_dMod      = itemPtr->m_damageModifier;
             trace++;
		  }//end make sure hes wearin it  
	   }//end update configuration/customized 

  	   for(int loop=0;loop<entityReference->m_entityObjectLink->List_skeletalObject->m_deformableMeshArrayCount;loop++)
	   {//deformable mesh loop
		  itemDataStruct[trace].m_GLID    = loop;
		  itemDataStruct[trace].m_quantity = entityReference->m_entityObjectLink->List_skeletalObject->m_spawnCfg[loop];
		  itemDataStruct[trace].m_dMod    = 0;
		  trace++;
	   }//end deformable mesh loop

       BYTE * cast = (BYTE*)itemDataStruct;

       for(int i=0;i<((totalSize-headerSize)-header.m_nameLength);i++)
           dataBlock[(headerSize+i+header.m_nameLength)] = cast[i];


	   if(itemDataStruct)
	   {
          delete [] itemDataStruct;
          itemDataStruct = 0;
	   }
    }

    m_gStatus->m_multiplayerObject->SendByteBlockMsg(DPNSEND_GUARANTEED,
							              NULL,
							              dataBlock,
							              totalSize,
										  m_gStatus->m_multiplayerObject->m_packetTimeout
						                  );

	return TRUE;
}
//---used by client-------------------------------------------------------------------//
BOOL CClientNetworkingLogic::PlayCharacterOnAccount(int index)
{
	
  if(index < 0)
	 return FALSE;

  if(!m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
      return FALSE;

      m_gStatus->m_netGameLogicObject->SendAttribMsg(   DPNSEND_GUARANTEED,
														DPID_SERVERPLAYER,
														9,
														index,
														0,
														m_gStatus->m_localContentClientVersion,
														15,//VERSION EXPLOIT
														NULL);

	  m_gStatus->m_multiplayerObject->m_currentCharacterInUse = index;//pending though
      return TRUE;
}
//--------------Handles system messages-----------------------------------------------//
BOOL CClientNetworkingLogic::HandleFireMessagesClientSide(LPFIRE_STRUCT formatedMessage, CMissileObjList * missileRuntime)
{
//broadcast to all around player that fired
	   D3DVECTOR lookat;
	   lookat.x = formatedMessage->lookatPosX;
	   lookat.y = formatedMessage->lookatPosY;
	   lookat.z = formatedMessage->lookatPosZ;
	   CMovementObj * mPtr = m_gStatus->m_entObjRuntime->GetObjByNetworkID(formatedMessage->idRefrence);
       if(mPtr)
	   {
		  int fireQueCast = formatedMessage->fireQueIndex;
          m_gStatus->OverrideFireQue(fireQueCast,lookat,mPtr,missileRuntime);
	   }
	  
return TRUE;
}
//--------------Handles system messages-----------------------------------------------//
BOOL CClientNetworkingLogic::HandleEquipMessageClientSide(LPEQUIP_STRUCT formatedMessage,
														  CMovementObjList * entObjRuntime,
														  CMissileObjList  * missileRuntime)
{
	if(formatedMessage->m_catagorie == 40)
	{
		CMovementObj * targetPtr = (CMovementObj *)entObjRuntime->GetObjByNetworkID(formatedMessage->idRefrence);
		if(targetPtr)
		{
			if(formatedMessage->index > 0)
				m_gStatus->GenAndMount(targetPtr,abs(formatedMessage->index),entObjRuntime,missileRuntime);
			else
				m_gStatus->DeMount(targetPtr,entObjRuntime);
		}
		return TRUE;
	}

	if(formatedMessage->idRefrence == 0)
	{//this is confermation
		POSITION posLoc = entObjRuntime->FindIndex(0);
		if(posLoc)
		{
			CMovementObj * selfPtr = ( CMovementObj* ) entObjRuntime->GetAt(posLoc);
			// Disarm Item
			if(formatedMessage->index < 0)
			{
				int vIndex = 0;
				if(formatedMessage->index != -9999)
					vIndex = abs(formatedMessage->index);
				CMovementObj * DBPtr = m_gStatus->m_entObjList->GetByID(selfPtr->List_refModel);
				if(DBPtr)
					selfPtr->DisarmItemByGLID(vIndex,TRUE,DBPtr,NULL);

				/*// REMOVE the item equipped selection indicator
				CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY, true));
				if (pInventory)
				{
					// SET button enabled states
					CMenuButton *pEquipButton = dynamic_cast<CMenuButton*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY_EQUIP_BUTTON, true));
					CMenuButton *pUnequipButton = dynamic_cast<CMenuButton*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY_UNEQUIP_BUTTON, true));
					if (pEquipButton) pEquipButton->SetEnabled();
					if (pUnequipButton) pUnequipButton->SetEnabled(false);

					// REFRESH visual display of inventory items
					pInventory->SelectedItem()->m_data.m_armed = false;
					pInventory->RefreshItems();
				}*/
				// UPDATE the inventory item equipped indicator
				CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY, true));
				if (pInventory)
				{
					// SET button enabled states
					CMenuButton *pEquipButton = dynamic_cast<CMenuButton*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY_EQUIP_BUTTON, true));
					CMenuButton *pUnequipButton = dynamic_cast<CMenuButton*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY_UNEQUIP_BUTTON, true));
					if (pEquipButton) pEquipButton->SetEnabled(false);
					if (pUnequipButton) pUnequipButton->SetEnabled();

					if (pInventory->SelectedItem())
						pInventory->SelectedItem()->m_data.m_armed = false;

					// CHECK to see if this item was activated from the Quickbar
					/*CMenuInventoryItem **pQuickbarItem = this->m_gStatus->m_multiplayerObject->Quickbar(0, this->m_gStatus->m_multiplayerObject->m_iQuickbarLastActivatedIndex);
					if (pQuickbarItem)
					{
						CMenuInventoryItem **ppInventoryItem = pInventory->GetInventoryItemByIndex((*pQuickbarItem)->m_data.m_index);
						if (ppInventoryItem)
							(*ppInventoryItem)->m_data.m_armed = false;
					}*/

					// REFRESH visual display of inventory items
					pInventory->RefreshItems();
				}
			}
			// Arm Item
			else
			{
				int GLID = abs(formatedMessage->index);
				CMovementObj * DBPtr = m_gStatus->m_entObjList->GetByID(selfPtr->List_refModel);
				if(DBPtr)
				{//valid ref model
					BOOL result = selfPtr->ArmItemByGLID(GLID,DBPtr,m_gStatus->m_itemManager,NULL);
					if (!result 
					&& (GLID >= 15000))
					{//dynamic item we need to request this from the server and have it arm on return
						m_gStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH,
																		DPID_SERVERPLAYER,
																		64,
																		GLID,
																		selfPtr->List_networkDefinedID,
																		0,
																		0,
																		0);
					}

					// GET a reference to the equipped item
					//CItemObj *pItem = m_gStatus->m_itemManager->GetItemByID(GLID);

					// UPDATE the inventory item equipped indicator
					CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY, true));
					if (pInventory)
					{
						// SET button enabled states
						CMenuButton *pEquipButton = dynamic_cast<CMenuButton*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY_EQUIP_BUTTON, true));
						CMenuButton *pUnequipButton = dynamic_cast<CMenuButton*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY_UNEQUIP_BUTTON, true));
						if (pEquipButton) pEquipButton->SetEnabled(false);
						if (pUnequipButton) pUnequipButton->SetEnabled();

						// CHECK to see if this item was activated from the Quickbar
						CMenuInventoryItem **pQuickbarItem = this->m_gStatus->m_multiplayerObject->Quickbar(0, this->m_gStatus->m_multiplayerObject->m_iQuickbarLastActivatedIndex);
						// the slot itself can be empty (never filled, or removed from the quickbar)
						this->m_gStatus->m_multiplayerObject->m_iQuickbarLastActivatedIndex = -1;
						if (pQuickbarItem && *pQuickbarItem)
						{
							CMenuInventoryItem **ppInventoryItem = pInventory->GetInventoryItemByIndex((*pQuickbarItem)->m_data.m_index);
							if (ppInventoryItem)
								(*ppInventoryItem)->m_data.m_armed = true;
						}
						else if (pInventory->SelectedItem())
						{
							pInventory->SelectedItem()->m_data.m_armed = true;
						}

						// REFRESH visual display of inventory items
						pInventory->RefreshItems();
					}
				}//end valid ref model
				else//failed
					m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Arm Item Error:","Index",abs(formatedMessage->index));
				
				/*CMenuListBox *pInventoryList = dynamic_cast<CMenuListBox*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(INVENTORY_LISTBOX_ID, true));
				if(pInventoryList)
				{
					// Loop through every item and remove SPECIAL status from
					// any item with the same Location ID as the newly equipped item
					CMenuListItem *pCurrentItem = pInventoryList->m_pFirstItem;
					while(pCurrentItem)
					{
						CItemObj *pTempItem = m_gStatus->m_itemManager->GetItemByID(pCurrentItem->m_miscInt1);
						if(pTempItem
						&& (pEquippedItem->m_globalID != pTempItem->m_globalID)
						&& (pEquippedItem->m_locationID == pTempItem->m_locationID)
						&& pCurrentItem->m_special)
						{
							pCurrentItem->m_special = false;
							break;
						}

						// Goto the next item
						pCurrentItem = pCurrentItem->m_pNext;
					}
				}*/
			}
		}
	}//end this is confermation
	else
	{//somone else
		CMovementObj * mPtr = entObjRuntime->GetObjByNetworkID(formatedMessage->idRefrence);
		if(mPtr)
		{//valid
			if(formatedMessage->index < 0)
			{
				int vIndex = 0;
				if(formatedMessage->index != -9999)
					vIndex = abs(formatedMessage->index);
				CMovementObj * DBPtr = m_gStatus->m_entObjList->GetByID(mPtr->List_refModel);
				if(DBPtr)//valid ref model
					mPtr->DisarmItemByGLID(vIndex,TRUE,DBPtr,NULL);
			}
			else
			{
				CMovementObj * DBPtr = m_gStatus->m_entObjList->GetByID(mPtr->List_refModel);
				if(DBPtr)
				{//valid ref model
					int glid = abs(formatedMessage->index);
					BOOL result = mPtr->ArmItemByGLID(glid,DBPtr,m_gStatus->m_itemManager,NULL);
					if(!result)
						if(glid >= 15000)
						{//dynamic item we need to request this from the server and have it arm on return
							m_gStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH,
																			DPID_SERVERPLAYER,
																			64,
																			glid,
																			mPtr->List_networkDefinedID,
																			0,
																			0,
																			0);
						}
				}//end valid ref model
				else//failed
					m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Arm Item Error:","Index",abs(formatedMessage->index));
			}
		}//end valid
		else
		{
			m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Error locating player with id");
		}
	}//end somone else
	return TRUE;
}
//--------------Handles spawn messages------------------------------------------------//
BOOL CClientNetworkingLogic::HandleSpawnMessagesClientSide(LPSPAWN_STRUCT formatedMessage,
											               DPNID idFromLoc)
{
	if(m_gStatus->m_entObjRuntime)
	{
	  int trace = 0;
      for(POSITION posLoc = m_gStatus->m_entObjRuntime->GetHeadPosition();posLoc != NULL;){//visual loop
	    CMovementObj * mPtr = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetNext(posLoc);
		m_gStatus->DestroyRuntimeEntity(mPtr,NULL,m_gStatus->m_entObjRuntime);
		mPtr = 0;
		trace++;
	  }
	  m_gStatus->m_entObjRuntime->RemoveAll();
	}	

	// Reset Quick Slots verfication flag
	this->m_bQuickSlotsVerified = FALSE;

	// Get references to arena menu images
	CMenuWindow *pArenaStats = dynamic_cast<CMenuWindow*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_STATS_MENU, true));
	CMenuImage *pVictory = dynamic_cast<CMenuImage*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_BANNER_VICTORY_ID, true));
	CMenuImage *pDefeat = dynamic_cast<CMenuImage*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_BANNER_DEFEAT_ID, true));
	CMenuImage *pMostWanted = dynamic_cast<CMenuImage*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_BADGE_MOST_WANTED_ID, true));
	CMenuImage *pSoleSurvivor = dynamic_cast<CMenuImage*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_BADGE_SOLE_SURVIVOR_ID, true));
	CMenuImage *pKillingBlow = dynamic_cast<CMenuImage*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_BADGE_KILLING_BLOW_ID, true));

	// Close the arena stats menu if currently open
	m_gStatus->m_runtimeMenuDatabase->CloseByID(ARENA_STATS_MENU, &m_gStatus->m_showMenuCursor);

	// Make arena menu images invisible if they exist
	if(pVictory)
	{
		// Close the arena countdown menu if currently open
		if(pVictory->m_alpha == 1.0f)
			m_gStatus->m_runtimeMenuDatabase->CloseByID(ARENA_TIMER_MENU, &m_gStatus->m_showMenuCursor);
		pVictory->m_alpha = 0.0f;
		pVictory->Init(m_gStatus->m_3dDevice);
		pVictory->UpdateTexture(pVictory->m_textureSelect);
		pVictory->UpdateTextureCoords();
	}

	if(pDefeat)
	{
		// Close the arena countdown menu if currently open
		if(pDefeat->m_alpha == 1.0f)
			m_gStatus->m_runtimeMenuDatabase->CloseByID(ARENA_TIMER_MENU, &m_gStatus->m_showMenuCursor);
		pDefeat->m_alpha = 0.0f;
		pDefeat->Init(m_gStatus->m_3dDevice);
		pDefeat->UpdateTexture(pDefeat->m_textureSelect);
		pDefeat->UpdateTextureCoords();
	}

	if(pMostWanted)
	{
		pMostWanted->m_alpha = 0.0f;
		pMostWanted->Init(m_gStatus->m_3dDevice);
		pMostWanted->UpdateTexture(pMostWanted->m_textureSelect);
		pMostWanted->UpdateTextureCoords();
	}

	if(pSoleSurvivor)
	{
		pSoleSurvivor->m_alpha = 0.0f;
		pSoleSurvivor->Init(m_gStatus->m_3dDevice);
		pSoleSurvivor->UpdateTexture(pSoleSurvivor->m_textureSelect);
		pSoleSurvivor->UpdateTextureCoords();
	}

	if(pKillingBlow)
	{
		pKillingBlow->m_alpha = 0.0f;
		pKillingBlow->Init(m_gStatus->m_3dDevice);
		pKillingBlow->UpdateTexture(pKillingBlow->m_textureSelect);
		pKillingBlow->UpdateTextureCoords();
	}
	
	//this is only for client use / not AI servers
	if(formatedMessage->worldName != m_gStatus->m_currentWorkingFile)
	{//necessary world change
       while(!m_gStatus->LoadScene(formatedMessage->worldName))
	   {//failed loading world
          CString build = formatedMessage->worldName;
		  build = operator+("Error opening file :",build);
		  if(m_gStatus->m_systemReadout)
			m_gStatus->m_systemReadout->AddLine(build);
		  return FALSE;
	   }//end failed loading world
	   m_gStatus->m_currentWorkingFile = formatedMessage->worldName;
	}//end necessary world change
    //flush unecessary
	if(m_gStatus->m_multiplayerObject->m_clientCfgRequestOutbox)
       m_gStatus->m_multiplayerObject->m_clientCfgRequestOutbox->Flush();
	if(m_gStatus->m_multiplayerObject->m_attributeOutBox)
       m_gStatus->m_multiplayerObject->m_attributeOutBox->Flush();
	//end flush unecessary

    D3DMATRIX locationalMatrix;
    m_gStatus->m_matLib.InitMat(locationalMatrix.m);
	locationalMatrix._41 = formatedMessage->uniInt1;
	locationalMatrix._42 = formatedMessage->uniInt2;
	locationalMatrix._43 = formatedMessage->uniInt3;

	D3DMATRIX successMatrice = m_gStatus->m_matrixInit2;
	successMatrice._41 = locationalMatrix._41;
	successMatrice._42 = locationalMatrix._42;
	successMatrice._43 = locationalMatrix._43;
	 
	//add the rotation of the entity
	int range = 360;
	range = rand() % 360;
	D3DMATRIX rotationMatrix;

	/*m_gStatus->m_matLib.RotateY(abs(range) - 180, rotationMatrix.m);*/
	 m_gStatus->m_matLib.RotateY((float)(abs(range) - 180), rotationMatrix.m);
	 //WARNING: C4244 --> int to float

	m_gStatus->m_matLib.fastMatrixMultiply(successMatrice, rotationMatrix, successMatrice);

    BOOL result = m_gStatus->ActivateMovementEntity(NULL,
                                         formatedMessage->currentTrace,
                                         successMatrice,//locationalMatrix,
                                         0,
								         0,
								         formatedMessage->dbIndex,//db
								         0,//user
                                         0,
								         0,
								         FALSE,
								         TRUE,
								         FALSE,
								         -1,
								         -1,
								         NULL,
					                     NULL,
										 -1,
										 -1,
									     m_gStatus->m_collisionBase,
									     m_gStatus->m_collisionDatabase,
										 m_gStatus->m_entObjRuntime,
										 m_gStatus->m_currentWorkingFile);
    if(!result)
		AfxMessageBox("Base spawn failed");

	m_gStatus->m_multiplayerObject->m_requestingPortal = FALSE;
	return TRUE;
}
//------------------------------------------------------------------------------------//
void CClientNetworkingLogic::HandleItemLocationalMessagesClientSide(BYTE * dataBlock,CMovementObjList * entObjRuntime,CMissileObjList * missileRuntime, int dataSize)
{
	   bool legacy = false;
	   LPITEM_LOCATION formatedMessage = (ITEM_LOCATION *)dataBlock;
	   int szMatch = dataSize - (formatedMessage->m_nameLength + formatedMessage->m_cfgLength + (formatedMessage->m_itemArmedLength * sizeof(short)));
	   if(sizeof(ITEM_LOCATION) == szMatch)
	      formatedMessage = (ITEM_LOCATION *)dataBlock;
	   else if(sizeof(ITEM_LOCATION_LEGACY) == szMatch)
	   {//legacy packet
          ITEM_LOCATION * tempPtr = new ITEM_LOCATION;
		  ZeroMemory(tempPtr,sizeof(ITEM_LOCATION));
          LPITEM_LOCATION_LEGACY temp = (ITEM_LOCATION_LEGACY *)dataBlock;
          
		  
		  tempPtr->curAnim				= temp->curAnim;
		  tempPtr->dbIndex				= temp->dbIndex;
		  tempPtr->m_autoTrigger 		= temp->m_autoTrigger;
		  tempPtr->m_catagorie			= temp->m_catagorie;
		  tempPtr->m_cfgLength			= temp->m_cfgLength;//bytes
		  tempPtr->m_itemArmedLength	= temp->m_itemArmedLength;//short values
		  tempPtr->m_nameLength			= temp->m_nameLength;//bytes
		  tempPtr->m_rotation			= temp->m_rotation;
		  tempPtr->mount				= temp->mount;
		  tempPtr->networkID			= temp->networkID;
		  tempPtr->Xpos                 = (float)temp->Xpos;
          tempPtr->Ypos                 = temp->Ypos;
		  tempPtr->Zpos                 = (float)temp->Zpos;

		  formatedMessage = (ITEM_LOCATION *)tempPtr;
          legacy = true;
	   }
	   else
          return;//failed

	   
	   //ITEM_LOCATION
	
	   if(!entObjRuntime->GetObjByNetworkID(formatedMessage->networkID))//if already spawned forget it
	   if(formatedMessage->dbIndex > -1){//spawn the item
		  CString name;
		  int tracer = szMatch;//sizeof(ITEM_LOCATION);
		  if(formatedMessage->m_nameLength > 0)
		  {//unpack name
		     char * nameTemp = new char[formatedMessage->m_nameLength+1];
			 ZeroMemory(nameTemp,formatedMessage->m_nameLength+1);
			 CopyMemory(nameTemp,&dataBlock[tracer],formatedMessage->m_nameLength);
			 tracer+=formatedMessage->m_nameLength;
			 
			 name = nameTemp;
			 delete [] nameTemp;
			 nameTemp = 0;
		  }//end unpack name

		  int * spawnCfg = NULL;
		  if(formatedMessage->m_cfgLength > 0)
		  {
			 spawnCfg = new int[formatedMessage->m_cfgLength+10];
		     ZeroMemory(spawnCfg,sizeof(int)*(formatedMessage->m_cfgLength+10));
		  }
		  //spawn cfg
		  for(int cfg=0;cfg<formatedMessage->m_cfgLength;cfg++)
		  {
		      spawnCfg[cfg] = dataBlock[tracer];
			  tracer++;
		  }
		  short * glidArray = NULL;
		  if(formatedMessage->m_itemArmedLength > 0){//equip glid package
             glidArray = new short[formatedMessage->m_itemArmedLength];
			 CopyMemory(glidArray,&dataBlock[tracer],formatedMessage->m_itemArmedLength*sizeof(short));
			 tracer+=formatedMessage->m_itemArmedLength*sizeof(short);
		  }//end equip glid package

		  CMovementObj * mObj = NULL;
          if(m_gStatus->ActivateMovementEntity(NULL,
			                      0,
	                              m_gStatus->m_matrixInit2,
	                              -1,
	                              -1,
	                               formatedMessage->dbIndex,
	                              35,
	                               0,
	                               formatedMessage->networkID,
	                               TRUE,
	                               FALSE,
	                               FALSE,
	                               formatedMessage->networkID,
	                               -1,
								   &mObj,
					               spawnCfg,
								   -1,
								   -1,
									m_gStatus->m_collisionBase,
									m_gStatus->m_collisionDatabase,
									entObjRuntime,
									m_gStatus->m_currentWorkingFile)){//valid item

			  mObj->m_autoTriggerTemp = formatedMessage->m_autoTrigger;
              //m_gStatus->m_systemReadout->AddLine(mObj->List_name,m_gStatus->m_3dDevice,m_gStatus->m_symbolMapDB,0.1f,1.0f,0.1f);//REMOVE
			  if(glidArray){//equip with list
			   CMovementObj * DBPtr = m_gStatus->m_entObjList->GetByID(mObj->List_refModel);
			     if(DBPtr)
                 for(int j=0;j<formatedMessage->m_itemArmedLength;j++)
				 {//glid loop
				    //mObj->ArmItemByGLID(glidArray[j],DBPtr,m_gStatus->m_itemManager,NULL);
					if(!mObj->ArmItemByGLID(glidArray[j],DBPtr,m_gStatus->m_itemManager,NULL))
					   if(glidArray[j] >= 15000)
					   {//request dyn item
							m_gStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH,
																	DPID_SERVERPLAYER,
																	64,
																	glidArray[j],
																	mObj->List_networkDefinedID,
																	0,
																	0,
																	0);
					   }//end request dyn item
				 }//end glid loop
			  }//end equip with list

		  D3DMATRIX positionalMatrix;
		  if(!formatedMessage->useExactOrientation)
		  {//use simple rotation method
			 m_gStatus->m_matLib.Rotate (0.0f,((float)formatedMessage->m_rotation)*2.0f,0.0f,positionalMatrix.m);
			 positionalMatrix._41 = formatedMessage->Xpos;
			 positionalMatrix._42 = formatedMessage->Ypos;
			 positionalMatrix._43 = formatedMessage->Zpos;
		  }//use simple rotation method
		  else
		  {//sugically transformed-ussually exported from engine to quest database with a separate tool from the world editor
             D3DVECTOR tempUp, tempDir;
			 tempUp.x				= formatedMessage->OrientateUpX / 127.0f;
	         tempUp.y				= formatedMessage->OrientateUpY / 127.0f;
	         tempUp.z				= formatedMessage->OrientateUpZ / 127.0f;
             tempDir.x				= formatedMessage->OrientateDirX / 127.0f;
	         tempDir.y				= formatedMessage->OrientateDirY / 127.0f;
	         tempDir.z				= formatedMessage->OrientateDirZ / 127.0f;
             m_gStatus->m_matLib.InitMat(positionalMatrix.m);
             m_gStatus->m_matLib.SetOrientation(&positionalMatrix,tempDir,tempUp);
             positionalMatrix._41	= formatedMessage->Xpos;
			 positionalMatrix._42	= formatedMessage->Ypos;
			 positionalMatrix._43	= formatedMessage->Zpos;
		  }//end sugically transformed
		  mObj->List_baseFrame->AddTransformReplace(positionalMatrix);//transform to location by replacement

		  mObj->m_validThisRound = TRUE;
		  mObj->List_networkPopoutCount = 0;
		  mObj->List_name = name;
		  if(formatedMessage->curAnim < 0)//reverse animation
			   mObj->List_skeletalObject->SetAnimationDirection(abs(formatedMessage->curAnim),TRUE);
			else
			   mObj->List_skeletalObject->SetAnimationDirection(abs(formatedMessage->curAnim),FALSE);
			   mObj->List_skeletalObject->SetCurrentAnimation(abs(formatedMessage->curAnim),100);
                
			   int version=0;int attrib=0;
			   if(mObj->List_armedInventoryDB){
				   mObj->List_skeletalObject->GetAnimationStatus(&version,&attrib,abs(formatedMessage->curAnim));
				   for(POSITION posArm = mObj->List_armedInventoryDB->GetHeadPosition();posArm!= NULL;){
					   CArmedInventoryObj * armPtr = (CArmedInventoryObj *)mObj->List_armedInventoryDB->GetNext(posArm);
                       if(armPtr->m_thirdPersonSystem){
						   int indexer = armPtr->m_thirdPersonSystem->GetAnimationIndexFromAttrib(attrib);
			               armPtr->m_thirdPersonSystem->SetCurrentAnimation(indexer,100);
					   }
				   }
			   }
		  }//end valid item
		  if(glidArray)
			 delete [] glidArray;
		     glidArray = 0;
		  if(spawnCfg)
	      {
             delete [] spawnCfg;
		     spawnCfg = 0;
	      }
		  if(mObj)
          if(formatedMessage->mount > -1)
             m_gStatus->GenAndMount(mObj,formatedMessage->mount,entObjRuntime,missileRuntime);
	   }//end spawn the item

	   if(legacy == true)
	   {
	      delete formatedMessage;
		  formatedMessage = 0;
	   }
}
//------------------------------------------------------------------------------------//
void CClientNetworkingLogic::Loot_clientSide(int targetNetworkID,
								             int myNetworkID,
								             int itemIndex)
{
  if(!m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
	   return;
           m_gStatus->m_netGameLogicObject->SendAttribMsg(  DPNSEND_GUARANTEED,
															DPID_SERVERPLAYER,
															11,//attrib
															targetNetworkID,
				 											myNetworkID,
															itemIndex,
															0,
															NULL);
}
//--------------Handles spawn messages------------------------------------------------//
BOOL CClientNetworkingLogic::HandleMenuBuildMessagesClientSide(LPMENUBUILD_STRUCT formatedMessage,DPNID idFromLoc)
{
	if(formatedMessage->messageType == 0)
	{//update menu control by ID
       m_gStatus->AddStringToMenuList(formatedMessage->miscint,formatedMessage->uniText,-1,0,0,formatedMessage->uniText,0,"");
	}//end update menu control by ID
    
    return TRUE;
}
//--------------Handles system messages-----------------------------------------------//
BOOL CClientNetworkingLogic::HandleTextMessageClientSide(LPVOID lpMessage,int & msgSize,DPID & idFromLoc)
{
		int stringLength = msgSize - 4;
		if(stringLength <= 0)
		return FALSE;
		BYTE * dataBlock = (BYTE*)lpMessage;
		LPTXT_MSG header = (LPTXT_MSG)lpMessage;
		char * newString = new char[stringLength+1];
		ZeroMemory(newString,(stringLength+1));
		//filter multiple spacing / spacing-prevent spoof
		BOOL lastSpace = FALSE;
		int stringTrace = 0;

		for(int charLoop=0;charLoop<stringLength;charLoop++)
		{
		 if(lastSpace)
			if(dataBlock[charLoop+4] == ' ')//skip
			   continue;
		 newString[stringTrace] = dataBlock[charLoop+4];
		 stringTrace++;
		 if(dataBlock[charLoop+4] == ' ')
			lastSpace = TRUE;
		 else
			lastSpace = FALSE;
		}

		newString[stringLength] = 0;

		CString loader = newString;
		if(newString)
			delete [] newString;
			newString = 0;


		if(!m_gStatus->m_systemReadout)
			return FALSE;
	 
		#ifdef _chinaClient
			if(dataBlock[0] == 0)//normal green
				m_gStatus->m_systemReadout->AddLineChina(loader,m_gStatus->m_3dDevice,m_gStatus->m_symbolMapDB,0.1f,1.0f,0.1f,m_gStatus->m_speechMode);
			if(dataBlock[0] == 1)//group light purple
				m_gStatus->m_systemReadout->AddLineChina(loader,m_gStatus->m_3dDevice,m_gStatus->m_symbolMapDB,0.8f,0.5f,0.8f,m_gStatus->m_speechMode);
			if(dataBlock[0] == 2)//clan light cyan
				m_gStatus->m_systemReadout->AddLineChina(loader,m_gStatus->m_3dDevice,m_gStatus->m_symbolMapDB,0.39f,.74f,0.82f,m_gStatus->m_speechMode);
		#endif
		#ifndef _chinaClient
		if(dataBlock[0] == 0)//normal readout color
		{
			if(header->referenceID <= 0 || m_gStatus->m_multiplayerObject->m_assignedIDGlobal == header->referenceID){}
			else m_gStatus->m_langFilter->FilterX(loader);//language filter
				
//			if(m_gStatus->m_chatReadout)
//				m_gStatus->m_chatReadout->AddLine(loader);
			if(m_gStatus->m_systemReadout)
				m_gStatus->m_systemReadout->AddLine(loader);
		}
		else if(dataBlock[0] == 1)//clan
		{
//			if(m_gStatus->m_chatReadout)m_gStatus->m_chatReadout->AddLine(loader, MENU_TEXTCOLOR_CLAN);
			if(m_gStatus->m_systemReadout)m_gStatus->m_systemReadout->AddLine(loader, MENU_TEXTCOLOR_CLAN);
		}
		else if(dataBlock[0] == 2)//group
		{
//			if(m_gStatus->m_chatReadout)m_gStatus->m_chatReadout->AddLine(loader, MENU_TEXTCOLOR_GROUP);
			if(m_gStatus->m_systemReadout)m_gStatus->m_systemReadout->AddLine(loader, MENU_TEXTCOLOR_GROUP);
		}
		else
		{
			if(m_gStatus->m_systemReadout)m_gStatus->m_systemReadout->AddLine(loader, MENU_TEXTCOLOR_SERVER);
//			if(m_gStatus->m_chatReadout)m_gStatus->m_chatReadout->AddLine(loader, MENU_TEXTCOLOR_SERVER);
		}
		#endif

	return TRUE;
}

BOOL 
CClientNetworkingLogic::HandleClientListGlidBlockData(LPVOID lpMessage,
													  int &msgSize,
													  DPID &idFromLoc)
{
	LPBLOCKITEMLIST_STRUCT header = (LPBLOCKITEMLIST_STRUCT)lpMessage;  
	BYTE *data = (BYTE*)lpMessage;

	// REMEMBER what the player carries - the quest tracker counts quest items from this,
	//	even while the inventory window is closed
	if (header->m_miscInt == HUD_QUEST_INV_REQ || header->m_miscInt == ID_PLAYER_INVENTORY)
	{
		int invCount = (msgSize > (int)sizeof(BLOCKITEMLIST_STRUCT))
			? (msgSize - (int)sizeof(BLOCKITEMLIST_STRUCT)) / (int)sizeof(ITEMDATA_STRUCT) : 0;
		int kept = 0;
		for (int i = 0; i < invCount && kept < 512; i++)
		{
			ITEMDATA_STRUCT invItem;
			::CopyMemory(&invItem, &data[sizeof(BLOCKITEMLIST_STRUCT) + (sizeof(ITEMDATA_STRUCT) * i)], sizeof(ITEMDATA_STRUCT));
			if (invItem.m_GLID <= 0)
				continue;
			m_gStatus->m_hudInvGlid[kept] = invItem.m_GLID;
			m_gStatus->m_hudInvQty[kept]  = (invItem.m_quantity > 0) ? invItem.m_quantity : 1;
			kept++;
		}
		m_gStatus->m_hudInvCount = kept;
		m_gStatus->m_hudInvStamp = GetTickCount();
		if (header->m_miscInt == HUD_QUEST_INV_REQ)
		{// the tracker's own request: nothing else to update
			SafeDeleteArray(data);
			return TRUE;
		}
	}

	// ERROR checking
	if ((msgSize-sizeof(BLOCKITEMLIST_STRUCT)) <= sizeof(ITEMDATA_STRUCT))
	{
		m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Block Update Failed");
		SafeDeleteArray(data);
		data=0;
		return TRUE;
	}

	// GET a reference to the menu visual 
	//	AND the number of items encoded in the message packet
	ITEMLIST items;
	int msgCount = (msgSize-sizeof(BLOCKITEMLIST_STRUCT)) / sizeof(ITEMDATA_STRUCT);
	CMenuInventory *pMenu = dynamic_cast<CMenuInventory*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(header->m_miscInt, true));
	if (pMenu)
	{
		// CLEAR the old, local copies of items out of the inventory
		pMenu->ClearItems();
		items = pMenu->m_items;
	}
	
	// PARSE items and ADD them to the player's inventory
	for (int msgParse = 0; msgParse < msgCount; msgParse++)
	{
		ITEMDATA_STRUCT item;
		::CopyMemory(&item, 
			&data[sizeof(BLOCKITEMLIST_STRUCT) + (sizeof(ITEMDATA_STRUCT) * msgParse)],
			sizeof(ITEMDATA_STRUCT));

		// HANDLE dynamic items
		if (!m_gStatus->m_itemManager->DoesItemExist(item.m_GLID)
		&& (item.m_GLID >= 15000))
		{
			m_gStatus->m_netGameLogicObject->SendAttribMsg(
				DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH,
				DPID_SERVERPLAYER,
				64,
				item.m_GLID,
				0,
				0,
				0,
				0);
		}

		// NOTE: SET the unique ordering index for player inventory items
		//	based on the order they are received by the client from the server
		ITEMDATA_STRUCT_EX client_item(item);
		client_item.m_index = msgParse; 
		if (!pMenu)
		{
			CMenuInventoryItem *pItem = new CMenuInventoryItem(client_item);
			items.push_back(pItem);
		}
		else pMenu->AddItem(client_item);
	}

	// VERIFY any items in the player's Quickbar
	m_gStatus->LoadQuickbarConfig(&items);
	SafeDeleteArray(data);
	return TRUE;
}

//--------------Handles system messages-----------------------------------------------//
/*
BOOL CClientNetworkingLogic::HandleClientListGlidBlockData(LPVOID lpMessage,int & msgSize,DPID & idFromLoc)
{
	POSITION tempPosition;
	LPBLOCKITEMLIST_STRUCT header = (LPBLOCKITEMLIST_STRUCT)lpMessage;  
	BYTE * dataBlock = (BYTE *)lpMessage;
	CItemObjList inventory; // Temp inventory collection

	if((msgSize-sizeof(BLOCKITEMLIST_STRUCT)) <= sizeof(ITEMDATA_STRUCT))
	{//failed
		m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Block Update Failed");
		delete [] dataBlock;
		dataBlock=0;
		return TRUE;
	}//end failed
	//get msg count
	int msgCount = (msgSize-sizeof(BLOCKITEMLIST_STRUCT))/sizeof(ITEMDATA_STRUCT);

	/*CMenuWindow *pWindow = dynamic_cast<CMenuWindow*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(header->m_miscInt, true));
	if (!pWindow)
		return FALSE;
		* /
	CMenuInventory *pMenu = dynamic_cast<CMenuInventory*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(header->m_miscInt, true));
	if (!pMenu)
		return FALSE;
	
	ITEMDATA_STRUCT item;
	for (int msgParse = 0; msgParse < msgCount; msgParse++)
	{//parse messages
		//item = new ITEMDATA_STRUCT();
		::CopyMemory(&item, 
			&dataBlock[sizeof(BLOCKITEMLIST_STRUCT) + (sizeof(ITEMDATA_STRUCT) * msgParse)],
			sizeof(ITEMDATA_STRUCT));
		//pWindow->m_pData->AddHead((CObject*)item);
		pMenu->AddItem(item);

		/ *
		CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(itemData.m_GLID);
		if(invObj)
		{
			CString output = invObj->m_itemName;
			CString append = "";
			if(itemData.m_dMod > 0)
			{
				append = " +" + m_gStatus->m_cabLib.ConvToString((int)itemData.m_dMod);
				output += append;
			}

			if(invObj->m_stackable)
				output += " X" + m_gStatus->m_cabLib.ConvToString(itemData.m_quanity);
			tempPosition = m_gStatus->AddStringToMenuList(header->m_miscInt, output, invObj->m_visualRef, itemData.m_GLID, itemData.m_quanity, invObj->m_itemName+append, 1, "", itemData.m_armed/*, clear* /);

			// Add item to temp collection
			inventory.AddTail(invObj);
		}
		else
		{
			tempPosition = m_gStatus->AddStringToMenuList(header->m_miscInt, "Unknown", -1, itemData.m_GLID, 0, "Unknown", 1, ""/*, clear* /);
		}* /

		//clear = false;
	}//end parse messages
	delete [] dataBlock;
	dataBlock=0;

	//Cleanup the Attribute Que List
	if (tempPosition)
		m_gStatus->m_attribQueue->RemoveAt(tempPosition);

	// Perform verification check on Quick Items
	m_gStatus->LoadQuickbarConfig(&inventory);
	return TRUE;
}
*/
//--------------Handles system messages-----------------------------------------------//
BOOL CClientNetworkingLogic::HandleEntityCfgBlock(LPVOID lpMessage,
												  int & msgSize,
												  DPID & idFromLoc,
												  CMovementObjList * entObjRuntime,
												  CMissileObjList * missileRuntime)
{
   LPENTITYCFG_HEADER header = (LPENTITYCFG_HEADER)lpMessage;  
   BYTE * dataBlock = (BYTE *)lpMessage;

   if(header->m_catagorie == 83)
   {//Housing specific
	  D3DVECTOR wldHsPosition;
      wldHsPosition.x = header->Xpos;
	  wldHsPosition.y = header->Ypos;
	  wldHsPosition.z = header->Zpos;
	  m_gStatus->m_houseingCache->PlaceHouse(wldHsPosition,header->dbCfg,header->networkDefinedID);
      return TRUE;
   }//end Housing specific
   int headerSz = sizeof(ENTITYCFG_HEADER);
   int invenItemCount = header->invenDataSize/sizeof(SHORTTYPE_DATA);

   int * spawnCfg = NULL;
   int defCfgItemCount = header->defCfgDataSize/sizeof(SHORTTYPE_DATA);
   if(defCfgItemCount > 0)
   {//valid deformable
     SHORTTYPE_DATA * defCfgBuildBlock = new SHORTTYPE_DATA[defCfgItemCount];
     BYTE * defCfgByteAccess = (BYTE*)defCfgBuildBlock;
     int offset = headerSz + abs(header->nameDataSize) + header->invenDataSize;
     for(int nloop=0;nloop<header->defCfgDataSize;nloop++)//def cfg loop
         defCfgByteAccess[nloop] = dataBlock[nloop+offset];

     if(defCfgItemCount>0)
	 {
	    spawnCfg = new int[defCfgItemCount+10];
        ZeroMemory(spawnCfg,sizeof(int)*(defCfgItemCount+10));
	 }

	 for(int apDefLoop=0;apDefLoop<defCfgItemCount;apDefLoop++)
	     spawnCfg[apDefLoop] = defCfgBuildBlock[apDefLoop].m_data;
         //for(int apDefLoop=0;apDefLoop<defCfgItemCount;apDefLoop++)//do def cfg
             //DBPtr->List_skeletalObject->HowSwapDim(apDefLoop,defCfgBuildBlock[apDefLoop].m_data,mPtr->List_skeletalObject,m_gStatus->m_3dDevice);
        
	  if(defCfgBuildBlock)
	  delete [] defCfgBuildBlock;
      defCfgBuildBlock = 0;
   }//end valid deformable
   
   
   if(!entObjRuntime->GetObjByNetworkID(header->networkDefinedID))
   {//not found spawn it
	  //m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("spawn from cfg net id:",(DWORD)header->networkDefinedID);//Remove
	  CMovementObj * temp = NULL;
	  int controlType = -1;
	  if(header->m_isItem)
		 controlType = 35;
      if(m_gStatus->ActivateMovementEntity(NULL,
			                    0,
	                            m_gStatus->m_matrixInit2,
	                            -1,
	                            -1,
	                            header->dbCfg,
	                            controlType,
	                            0,
	                            header->networkDefinedID,
	                            TRUE,
	                            FALSE,
	                            FALSE,
	                            -1,
	                            header->networkDefinedID,
							    &temp,
					            spawnCfg,
								-1,
								-1,
								m_gStatus->m_collisionBase,
								m_gStatus->m_collisionDatabase,
								entObjRuntime,
								m_gStatus->m_currentWorkingFile)){
	       if(spawnCfg)
           {
              delete [] spawnCfg;
              spawnCfg = 0;
           }
		   //initialize
		   temp->m_clanMember = header->m_clanMember;
		   temp->m_groupMember = header->m_groupMember;
		   temp->m_isAI = header->isAI;
		   
		   temp->List_baseFrame->SetPosition2(NULL,header->Xpos,header->Ypos,header->Zpos);
	       temp->List_networkPopoutCount               = 0;//updated dont pop out
		   temp->List_networkInterpCurStep             = 0;  
	       temp->List_networkInterpCurStepOri          = 0;
	       StampNetworkMark(temp);
		   temp->m_validThisRound                      = TRUE;
           temp->List_baseFrame->GetPosition(NULL,&temp->List_networkLastPositionXYZ);
	       temp->List_baseFrame->GetOrientation(&temp->List_networkLastDirVect,&temp->List_networkLastUpVect); 
	       temp->List_networkFreeLookCurYawLast        = 0.0f;
	       temp->List_networkFreeLookCurPitchLast      = 0.0f;

			temp->List_networkTargetPositionXYZ.x     = header->Xpos;
	        temp->List_networkTargetPositionXYZ.y     = header->Ypos;
	        temp->List_networkTargetPositionXYZ.z     = header->Zpos;
			temp->List_networkFreeLookCurYawTarget    = 0.0f;
	        temp->List_networkFreeLookCurPitchTarget  = 0.0f;

			if(temp->List_skeletalObject->SetCurrentAnimation(abs(header->anim),100))
			{		
			  if(header->anim < 0)//reverse animation
			     temp->List_skeletalObject->SetAnimationDirection(abs(header->anim),TRUE);
		  	  else
			     temp->List_skeletalObject->SetAnimationDirection(abs(header->anim),FALSE);
			}  
		   //end initialize
		   
	  }
   }//end not found spawn it

   
   
   //retrieve entity
   CMovementObj * mPtr = entObjRuntime->GetObjByNetworkID(header->networkDefinedID);//verify
   if(!mPtr)
   {
	   if(spawnCfg)
       {
          delete [] spawnCfg;
          spawnCfg = 0;
       }
       return FALSE;
   }
  
   if(mPtr->List_networkEntity)
      mPtr->m_networkClanID = header->clanID;

   if(mPtr->List_networkEntity == FALSE && mPtr->List_controlType == 0)
   m_gStatus->m_multiplayerObject->m_requestingPortal = FALSE;
          
   CMovementObj * DBPtr = m_gStatus->m_entObjList->GetByID(mPtr->List_refModel);
   if(!DBPtr)
   {
	   if(spawnCfg)
       {
          delete [] spawnCfg;
          spawnCfg = 0;
       }
       return FALSE;
   }
   
   //

   if(header->nameDataSize < 0)
   {
      header->nameDataSize = abs(header->nameDataSize);
      mPtr->m_murderer = TRUE;
   }

   if(mPtr->m_textureRenderObject)
   {
      mPtr->m_textureRenderObject->SafeDelete();
      delete mPtr->m_textureRenderObject;
	  mPtr->m_textureRenderObject = 0;
   }

   if(mPtr->m_textureRenderObject2)
   {
      mPtr->m_textureRenderObject2->SafeDelete();
      delete mPtr->m_textureRenderObject2;
	  mPtr->m_textureRenderObject2 = 0;
   }

   //Set name
   if(header->nameDataSize > 0)
   {//valid name data
		int colonLocation = 0;
		while(dataBlock[headerSz + colonLocation] != ':')
		{
			if(colonLocation == header->nameDataSize)
			{
				colonLocation = -1;
				break;
			}
			colonLocation++;
		}

		int nameSize = header->nameDataSize - colonLocation;
		if(colonLocation > 0)
		{
			char *clanString = new char[colonLocation + 1];

			for(int nloop = 0; nloop < colonLocation; nloop++)
				clanString[nloop] = dataBlock[nloop + headerSz];

			clanString[colonLocation]	= 0;
			mPtr->List_clanName			= CString("<") + clanString + ">";

			if(clanString)
				delete [] clanString;
		}

		char *nameString = new char[nameSize];
		int i = 0;
		for(int nloop = colonLocation + 1; nloop < header->nameDataSize; nloop++)
			nameString[i++] = dataBlock[nloop + headerSz];

		nameString[nameSize - 1] = 0;
		mPtr->List_name = nameString;//set to ptr

		if(mPtr->m_optionalTextMesh)
		{
			mPtr->m_optionalTextMesh->SafeDelete();
			mPtr->m_optionalTextMesh = 0;
		}

		if(nameString)
			delete [] nameString;
	}//end valid name data
   
   //set inventory Cfg
   if(spawnCfg)
   if(defCfgItemCount > 0)
   {//valid deformable
         for(int apDefLoop=0;apDefLoop<defCfgItemCount;apDefLoop++)//do def cfg
             DBPtr->List_skeletalObject->HotSwapDim(apDefLoop,spawnCfg[apDefLoop],mPtr->List_skeletalObject,m_gStatus->m_3dDevice,TRUE);
   }//end valid deformable
   
   if(invenItemCount > 0)
   {//valid inven
     SHORTTYPE_DATA * invenBuildBlock = new SHORTTYPE_DATA[invenItemCount];
     BYTE * invenByteAccess = (BYTE*)invenBuildBlock;
     int offset = headerSz + header->nameDataSize;
     for(int nloop=0;nloop<header->invenDataSize;nloop++)
	 {//inventory build loop
       invenByteAccess[nloop] = dataBlock[nloop+offset];
	 }//end inventory build loop
	 int curPhyObj = 0;
     for(int invenLoop=0;invenLoop<invenItemCount;invenLoop++)
	 {//inven action loop
		 if(invenBuildBlock[invenLoop].m_data<0)
		 {//server side physics assigned
	        if(mPtr->List_movementDB)
			{
               /*if(curPhyObj == 0)
			   {
                  mPtr->List_movementDB->List_maxSpeed = ((float)abs(invenBuildBlock[invenLoop].m_data))*.001f;
				  mPtr->List_movementDB->List_minSpeed = -mPtr->List_movementDB->List_maxSpeed;
				  mPtr->List_movementDB->List_accellerate = mPtr->List_movementDB->List_maxSpeed;
				  mPtr->List_movementDB->List_decellerate = mPtr->List_movementDB->List_maxSpeed;
			   }
			   if(curPhyObj == 1)
                  mPtr->List_movementDB->List_jumpPower = ((float)abs(invenBuildBlock[invenLoop].m_data))*.001f;
			   if(curPhyObj == 2)
                  mPtr->List_movementDB->m_inLiquidMotionModifier = ((float)abs(invenBuildBlock[invenLoop].m_data))*.001f;
			   if(curPhyObj == 3)
                  mPtr->List_movementDB->m_crossSectionalArea = ((float)abs(invenBuildBlock[invenLoop].m_data))*.001f;
               if(curPhyObj == 4)
                  mPtr->List_movementDB->m_airControl = ((float)abs(invenBuildBlock[invenLoop].m_data))*.001f;
			   curPhyObj++;*/
			}
		 }//end server side physics assigned
		 else
		 {
		   int glid = abs(invenBuildBlock[invenLoop].m_data);
		   if(!mPtr->ArmItemByGLID(glid,DBPtr,m_gStatus->m_itemManager,NULL))
			   if(glid >= 15000)
			   {//request dyn item
					m_gStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH,
															DPID_SERVERPLAYER,
															64,
															glid,
															mPtr->List_networkDefinedID,
															0,
															0,
															0);
			   }//end request dyn item
		 }
	 }//end inven action loop

     if(invenBuildBlock)
	    delete [] invenBuildBlock;
        invenBuildBlock = 0;
   //set def cfg
   }//end valid inven
   

   if(spawnCfg)
   {
      delete [] spawnCfg;
      spawnCfg = 0;
   }
   //handle mount function
   if(mPtr)
   if(header->mount > -1)
      m_gStatus->GenAndMount(mPtr,header->mount,entObjRuntime,missileRuntime);

   if(mPtr)
   if(m_gStatus->m_multiplayerObject->m_AIServer)
      m_gStatus->IndexEntityAnimations(mPtr,GetTickCount(),TRUE,missileRuntime);

	// REQUEST squad info from the server
	m_gStatus->SendChatString("/REQUESTSQUADINFO");

   return TRUE;
}
//------------------------------------------------------------------------------------//
void CClientNetworkingLogic::SendPendingCfgRequestsCP()
{
    int segCount = (int)m_gStatus->m_multiplayerObject->m_clientCfgRequestOutbox->GetCount();

    if(segCount < 1)
	   return;//none to request


	int datasz = (sizeof(ATTRIB_STRUCT)*segCount)+1;

	BYTE * rawDataBlock = new BYTE[datasz];
    
	rawDataBlock[0] = 24;//catagorie
   
	int datatrace = 1;

	for(POSITION posLoc = m_gStatus->m_multiplayerObject->m_clientCfgRequestOutbox->GetHeadPosition();posLoc != NULL;)
	{
        CPacketObj * pckPtr = (CPacketObj *)m_gStatus->m_multiplayerObject->m_clientCfgRequestOutbox->GetNext(posLoc);
        for(int i=0;i<pckPtr->m_size;i++){
            rawDataBlock[datatrace] = pckPtr->m_data[i];
            datatrace++;
		}
	}

	m_gStatus->m_multiplayerObject->SendData(rawDataBlock,
				                  datasz,
				                  DPNSEND_NOCOMPLETE | DPNSEND_PRIORITY_HIGH,
				                  NULL,
							      m_gStatus->m_multiplayerObject->m_packetTimeout);

    m_gStatus->m_multiplayerObject->m_clientCfgRequestOutbox->Flush();//flush

	delete [] rawDataBlock;
	rawDataBlock = 0;
}
//--------------Handles system messages-----------------------------------------------//
BOOL CClientNetworkingLogic::HandleClientStatBlockData(LPVOID lpMessage,int & msgSize,DPID & idFromLoc)
{
    LPSTATBLKHEADER_DATA statBlockData = (LPSTATBLKHEADER_DATA)lpMessage;
	BYTE * dataBlock = (BYTE *)lpMessage;
	
	POSITION posLoc = m_gStatus->m_entObjRuntime->FindIndex(0);
	if(posLoc)
	{
	   CMovementObj * mPtr = (CMovementObj *)m_gStatus->m_entObjRuntime->GetAt(posLoc);
	   if(mPtr)
	   {
		  int oldEnergy = mPtr->List_energy;
	      mPtr->List_energy = statBlockData->m_hp;
          mPtr->List_maxEnergy = statBlockData->m_maxHp;
		  mPtr->m_level = statBlockData->m_level;
		  mPtr->m_curXP = statBlockData->m_curXP;
		  mPtr->m_nextLvlXP = statBlockData->m_maxXP;
		  mPtr->m_percXP = statBlockData->m_xpPerc;
		  mPtr->m_factionBonus1 = statBlockData->m_facBonus1;
		  mPtr->m_factionBonus2 = statBlockData->m_facBonus2;
		  mPtr->m_factionBonus3 = statBlockData->m_facBonus3;
		  mPtr->m_curFaction = statBlockData->m_curFaction;

		  if(m_gStatus->m_multiplayerObject->m_rechargeTimer != statBlockData->m_rechargeTimer)
			m_gStatus->m_multiplayerObject->m_rechargeTimerStart = GetTickCount();
		  m_gStatus->m_multiplayerObject->m_rechargeTimer			 = statBlockData->m_rechargeTimer;

		  if(!m_gStatus->m_multiplayerObject->m_AIServer)
		  if(oldEnergy > mPtr->List_energy)
		  {	  
			  //m_gStatus->m_screenFlashModule->DoIt(500,m_gStatus->m_3dDevice);
			  
			  //Additonal feature for the border flash
			  const int SIZE = 10;
			  CMenuImage *pImageRight = NULL;
			  CMenuImage *pImageLeft = NULL;
			  CMenuImage *pImageTop = NULL;
			  CMenuImage *pImageBottom = NULL;
			  CMenuImage *pImageFullscreen = NULL;

			  pImageRight = dynamic_cast<CMenuImage*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(RIGHT_FLASH, true));
			  pImageLeft = dynamic_cast<CMenuImage*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(LEFT_FLASH, true));
			  pImageTop = dynamic_cast<CMenuImage*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(TOP_FLASH, true));
			  pImageBottom = dynamic_cast<CMenuImage*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(BOTTOM_FLASH, true));
			  pImageFullscreen = dynamic_cast<CMenuImage*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(FULLSCREEN_FLASH, true));

			  if(pImageRight)
			  {
					// Make the image visible and update the texture for render
					pImageRight->m_visible = true;
					pImageRight->m_alpha = 1.0f;
					pImageRight->Init(m_gStatus->m_3dDevice);
					pImageRight->m_posX = m_gStatus->m_dxRenderer->m_currentWidth - (m_gStatus->m_dxRenderer->m_currentWidth * 0.12f) + 15;
					pImageRight->m_posY = 0;

					/*pImageRight->m_height = m_gStatus->m_dxRenderer->m_currentHeight;*/
					pImageRight->m_height = (float)m_gStatus->m_dxRenderer->m_currentHeight;
					//WARNING: C4244 --> int to float

					pImageRight->m_width = m_gStatus->m_dxRenderer->m_currentWidth * 0.12f;
					pImageRight->m_imageMesh->List_blendType = 1;
					pImageRight->UpdateTexture(pImageRight->m_textureSelect);
					pImageRight->UpdateTextureCoords();
					
					// Create the update script string...
					char strParam[SIZE];
					CString strUpdateScript = "FadeImage(";
					itoa(RIGHT_FLASH, strParam, 10);
					strUpdateScript += strParam;
					strUpdateScript += ",";
					
					/*CString strFadeRate = m_gStatus->m_cabLib.FloatToStingCnv(1.64);*/
					CString strFadeRate = m_gStatus->m_cabLib.FloatToStingCnv(1.64f);
					//WARNING: C4305 --> truncation from double to float

					strUpdateScript += strFadeRate; 
					strUpdateScript += ")";

					// Attach the update script
					if(!pImageRight->m_updateScript)
						pImageRight->m_updateScript = new CMenuScript();
					pImageRight->m_updateScript->ClearScript();
					pImageRight->m_updateScript->AddLine(strUpdateScript.GetBuffer());
					strUpdateScript.ReleaseBuffer();
					pImageRight->m_updateScript->m_pFirstLine->m_function = MENU_SCRIPTFUNC_FADEIMAGE;
			  }

			  if(pImageLeft)
			  {
					// Make the image visible and update the texture for render
					pImageLeft->m_visible = true;
					pImageLeft->m_alpha = 1.0f;
					pImageLeft->Init(m_gStatus->m_3dDevice);
					pImageLeft->m_posX = 0;
					pImageLeft->m_posY = 0;
					pImageLeft->m_height;
					pImageLeft->m_width = m_gStatus->m_dxRenderer->m_currentWidth * 0.12f;
					pImageLeft->m_imageMesh->List_blendType = 1;
					pImageLeft->UpdateTexture(pImageLeft->m_textureSelect);
					pImageLeft->UpdateTextureCoords();
					
					// Create the update script string...
					char strParam[SIZE];
					CString strUpdateScript = "FadeImage(";
					itoa(LEFT_FLASH, strParam, 10);
					strUpdateScript += strParam;
					strUpdateScript += ",";
					
					/*CString strFadeRate = m_gStatus->m_cabLib.FloatToStingCnv(1.64);*/
					CString strFadeRate = m_gStatus->m_cabLib.FloatToStingCnv(1.64f);
					//WARNING: C4305 --> truncation from double to float

					strUpdateScript += strFadeRate; 
					strUpdateScript += ")";

					// Attach the update script
					if(!pImageLeft->m_updateScript)
						pImageLeft->m_updateScript = new CMenuScript();
					pImageLeft->m_updateScript->ClearScript();
					pImageLeft->m_updateScript->AddLine(strUpdateScript.GetBuffer());
					strUpdateScript.ReleaseBuffer();
					pImageLeft->m_updateScript->m_pFirstLine->m_function = MENU_SCRIPTFUNC_FADEIMAGE;
			  }

			  if(pImageTop)
			  {
					// Make the image visible and update the texture for render
					pImageTop->m_visible = true;
					pImageTop->m_alpha = 1.0f;
					pImageTop->Init(m_gStatus->m_3dDevice);
					pImageTop->m_posX = 0;
					pImageTop->m_posY = m_gStatus->m_dxRenderer->m_currentHeight - (m_gStatus->m_dxRenderer->m_currentHeight * 0.09f) + 15;
					pImageTop->m_height = m_gStatus->m_dxRenderer->m_currentHeight * 0.09f;

					/*pImageTop->m_width = m_gStatus->m_dxRenderer->m_currentWidth;*/
					pImageTop->m_width = (float)m_gStatus->m_dxRenderer->m_currentWidth;
					//WARNING: C4244 --> int to float

					pImageTop->m_imageMesh->List_blendType = 1;
					pImageTop->UpdateTexture(pImageTop->m_textureSelect);
					pImageTop->UpdateTextureCoords();

					// Create the update script string...
					char strParam[SIZE];
					CString strUpdateScript = "FadeImage(";
					itoa(TOP_FLASH, strParam, 10);
					strUpdateScript += strParam;
					strUpdateScript += ",";

					/*CString strFadeRate = m_gStatus->m_cabLib.FloatToStingCnv(1.64);*/
					CString strFadeRate = m_gStatus->m_cabLib.FloatToStingCnv(1.64f);
					//WARNING: C4305 --> truncation from double to float
					
					strUpdateScript += strFadeRate; 
					strUpdateScript += ")";

					// Attach the update script
					if(!pImageTop->m_updateScript)
						pImageTop->m_updateScript = new CMenuScript();
					pImageTop->m_updateScript->ClearScript();
					pImageTop->m_updateScript->AddLine(strUpdateScript.GetBuffer());
					strUpdateScript.ReleaseBuffer();
					pImageTop->m_updateScript->m_pFirstLine->m_function = MENU_SCRIPTFUNC_FADEIMAGE;
			  }

			  if(pImageBottom)
			  {
					// Make the image visible and update the texture for render
					pImageBottom->m_visible = true;
					pImageBottom->m_alpha = 1.0f;
					pImageBottom->Init(m_gStatus->m_3dDevice);
					pImageBottom->m_posX = 0;
					pImageBottom->m_posY = -15;
					pImageBottom->m_height = m_gStatus->m_dxRenderer->m_currentHeight * 0.09f;
					
					/*pImageBottom->m_width = m_gStatus->m_dxRenderer->m_currentWidth;*/
					pImageBottom->m_width = (float)m_gStatus->m_dxRenderer->m_currentWidth;
					//WARNING: C4244 --> int to float
					
					pImageBottom->m_imageMesh->List_blendType = 1;
					pImageBottom->UpdateTexture(pImageRight->m_textureSelect);
					pImageBottom->UpdateTextureCoords();

					// Create the update script string...
					char strParam[SIZE];
					CString strUpdateScript = "FadeImage(";
					itoa(BOTTOM_FLASH, strParam, 10);
					strUpdateScript += strParam;
					strUpdateScript += ",";
					
					/*CString strFadeRate = m_gStatus->m_cabLib.FloatToStingCnv(1.64);*/
					CString strFadeRate = m_gStatus->m_cabLib.FloatToStingCnv(1.64f);
					//WARNING: C4305 --> truncation from double to float

					strUpdateScript += strFadeRate; 
					strUpdateScript += ")";

					// Attach the update script
					if(!pImageBottom->m_updateScript)
						pImageBottom->m_updateScript = new CMenuScript();
					pImageBottom->m_updateScript->ClearScript();
					pImageBottom->m_updateScript->AddLine(strUpdateScript.GetBuffer());
					strUpdateScript.ReleaseBuffer();
					pImageBottom->m_updateScript->m_pFirstLine->m_function = MENU_SCRIPTFUNC_FADEIMAGE;
			  }

			  if(pImageFullscreen)
			  {
					// Make the image visible and update the texture for render
					pImageFullscreen->m_visible = true;
					pImageFullscreen->m_alpha = 0.45f;
					pImageFullscreen->Init(m_gStatus->m_3dDevice);
					pImageFullscreen->m_posX = 0;
					pImageFullscreen->m_posY = 0;
					
					/*pImageFullscreen->m_height = m_gStatus->m_dxRenderer->m_currentHeight;*/
					pImageFullscreen->m_height = (float)m_gStatus->m_dxRenderer->m_currentHeight;
					//WARNING: C4244 --> int to float

					/*pImageFullscreen->m_width = m_gStatus->m_dxRenderer->m_currentWidth;*/
					pImageFullscreen->m_width = (float)m_gStatus->m_dxRenderer->m_currentWidth;
					//WARNING: C4244 --> int to float

					pImageFullscreen->m_imageMesh->List_blendType = 1;
					pImageFullscreen->UpdateTexture(pImageRight->m_textureSelect);
					pImageFullscreen->UpdateTextureCoords();

					// Create the update script string...
					char strParam[SIZE];
					CString strUpdateScript = "FadeImage(";
					itoa(FULLSCREEN_FLASH, strParam, 10);
					strUpdateScript += strParam;
					strUpdateScript += ",";
					
					/*CString strFadeRate = m_gStatus->m_cabLib.FloatToStingCnv(1.64);*/
					CString strFadeRate = m_gStatus->m_cabLib.FloatToStingCnv(1.64f);
					//WARNING: C4305 --> truncation from double to float

					strUpdateScript += strFadeRate; 
					strUpdateScript += ",0)";

					// Attach the update script
					if(!pImageFullscreen->m_updateScript)
						pImageFullscreen->m_updateScript = new CMenuScript();
					pImageFullscreen->m_updateScript->ClearScript();
					pImageFullscreen->m_updateScript->AddLine(strUpdateScript.GetBuffer());
					strUpdateScript.ReleaseBuffer();
					pImageFullscreen->m_updateScript->m_pFirstLine->m_function = MENU_SCRIPTFUNC_FADEIMAGE;
			  }
		  }
          if(statBlockData->m_hp <= 0)
		     m_gStatus->m_globalAvVar = FALSE;

	   }
	}

	m_gStatus->m_multiplayerObject->m_currentStat1 = statBlockData->m_str;
	m_gStatus->m_multiplayerObject->m_currentStat2 = statBlockData->m_dex;
    m_gStatus->m_multiplayerObject->m_currentStat3 = statBlockData->m_wis;
  
	if(statBlockData->m_stasis == 0)
       m_gStatus->m_multiplayerObject->m_currentStasis = "NORMAL";
	if(statBlockData->m_stasis == 1)
       m_gStatus->m_multiplayerObject->m_currentStasis = "INVIS";
	if(statBlockData->m_stasis == 2)
       m_gStatus->m_multiplayerObject->m_currentStasis = "SHIFTED";

	if(m_gStatus->m_skillDatabase)//get title
       m_gStatus->m_multiplayerObject->m_titleLink = m_gStatus->m_skillDatabase->GetTitleByLevelAndType(statBlockData->m_skillType,statBlockData->m_level);

	
    int parseTrace = sizeof(STATBLKHEADER_DATA);
	int elementCount = msgSize - parseTrace;

    
    if(m_gStatus->m_multiplayerObject->m_elementStatus)
	{
       m_gStatus->m_multiplayerObject->m_elementStatus->SafeDelete();
	   delete m_gStatus->m_multiplayerObject->m_elementStatus;
	   m_gStatus->m_multiplayerObject->m_elementStatus = 0;
	}
       m_gStatus->m_multiplayerObject->m_elementStatus = new CElementStatusList();

	for(int eleLoop = 0;eleLoop < elementCount;eleLoop++)
	{
        CElementStatusObj * elementPtr = new CElementStatusObj();
		elementPtr->m_currentValue = dataBlock[parseTrace];
	    elementPtr->m_elementIndex = eleLoop;
		m_gStatus->m_multiplayerObject->m_elementStatus->AddTail(elementPtr);
        parseTrace++;
	}
    

	delete [] dataBlock;
    dataBlock=0;

	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL
CClientNetworkingLogic::HandleStoreDataBlock(LPVOID pMessage,
											 int &iMessageSize,
											 DPID &iFromID)
{
	POSITION pos;
    BYTE *pData = (BYTE*)pMessage;
    LPSTORE_BLOCKHEADER header = (LPSTORE_BLOCKHEADER)pMessage;

	// OPEN store specified menu
    m_gStatus->OpenMenu(header->m_menuBind, false);
	CMenuInventory *pInventory = m_gStatus->GetActiveInventory(ID_STORE_MENU);

	// UPDATE bank data
    m_gStatus->m_multiplayerObject->m_cashCount = header->m_playerCash;

	// GET data sizes for packet processing
	int iHeaderSize = sizeof(STORE_BLOCKHEADER);
	int iDataSize = (iMessageSize - iHeaderSize);
	int iItemCount = (iDataSize / sizeof(ITEMDATA_STRUCT));
	if (!pInventory
	|| (iDataSize <= 0)
	|| (iItemCount <= 0))
	{
		SafeDeleteArray(pData);
		return FALSE;
	}
	
	// ADVANCE the pointer past the header,
	//	to the start of the item data
	BYTE *pItemData = pData + iHeaderSize;

	// PARSE out the items in the packet
	for (int i=0; i<iItemCount; i++)
	{
		ITEMDATA_STRUCT item;
		CopyMemory(&item, &pItemData[i * sizeof(ITEMDATA_STRUCT)], sizeof(ITEMDATA_STRUCT));

		// GET the base item object
		CItemObj *pItem = m_gStatus->m_itemManager->GetItemByID(item.m_GLID);

		// ENCODE the market cost into the quantity data member
		item.m_quantity = pItem->m_marketCost;

		// ADD the item to the store inventory
		ITEMDATA_STRUCT_EX client_item(item);
		pInventory->AddItem(client_item);
	}

	// RELEASE memory resources
	SafeDeleteArray(pData);
	return TRUE;
}

/*BOOL CClientNetworkingLogic::HandleStoreDataBlock(LPVOID lpMessage,int & msgSize,DPID & idFromLoc)
{
	POSITION tempPosition;
    BYTE * dataBlock = (BYTE *)lpMessage;
    LPSTORE_BLOCKHEADER header = (LPSTORE_BLOCKHEADER)lpMessage;

        m_gStatus->OpenMenu(header->m_menuBind, false);//open store specified menu
        m_gStatus->m_multiplayerObject->m_cashCount = header->m_playerCash;//update bank data

		int headerSize = sizeof(STORE_BLOCKHEADER);
		int itemDataBlockSize = (msgSize-headerSize);
		if(itemDataBlockSize > 0)
		{//valid
		int itemDataSize = sizeof(ITEMDATA_STRUCT);
		int invenCount = (itemDataBlockSize/itemDataSize);
        ITEMDATA_STRUCT * itemDataBlock = new ITEMDATA_STRUCT[invenCount];
		BYTE * conversionBlock = (BYTE*)itemDataBlock;
		int loop=0;
        for(loop=0;loop<itemDataBlockSize;loop++)
            conversionBlock[loop] = dataBlock[loop+headerSize];

        //--------------------
		//int tracer = 0;
		for(loop=0;loop<header->m_storeItemCount;loop++)
		{//fill in bank data
		  CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(itemDataBlock[loop].m_GLID);
		  if(invObj){
              CString output = invObj->m_itemName;
              CString appendDmg;
              if(itemDataBlock[loop].m_dMod > 0)
                 appendDmg = " +" + m_gStatus->m_cabLib.ConvToString((int)itemDataBlock[loop].m_dMod);
			  output += " cost: ";
              output += m_gStatus->m_cabLib.ConvToString(invObj->m_marketCost);
			  tempPosition = m_gStatus->AddStringToMenuList(54,output,invObj->m_visualRef,itemDataBlock[loop].m_GLID,0,invObj->m_itemName+appendDmg,1,(" cost: " + m_gStatus->m_cabLib.ConvToString(invObj->m_marketCost)));
		  }
		  else{//alternate
             tempPosition = m_gStatus->AddStringToMenuList(54,"Unknown",-1,itemDataBlock[loop].m_GLID,0,"Unknown",1,"");
		  }//end alternate
		  //tracer++;
		}//end fill in bank data
		}//end valid
	delete [] dataBlock;
    dataBlock=0;

	//Cleanup the Attribute Que List
	if (tempPosition)
	   m_gStatus->m_attribQueue->RemoveAt(tempPosition);

	return TRUE;
}*/
//------------------------------------------------------------------------------------//
BOOL CClientNetworkingLogic::SendCommandMsg(int msgType,int miscInt,int networkAssignedID)
{

	COMMAND_DATA dataPacket;
	int messageSize = sizeof(COMMAND_DATA);
    dataPacket.m_catagorie = 18;
	dataPacket.m_data      = msgType;
	dataPacket.m_miscInt   = miscInt;
	dataPacket.m_networkID = networkAssignedID;

    m_gStatus->m_multiplayerObject->SendData((BYTE *)&dataPacket,
				                          messageSize,
				                          DPNSEND_GUARANTEED,
				                          NULL,
										  m_gStatus->m_multiplayerObject->m_packetTimeout);

	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CClientNetworkingLogic::ParseGroupListData(BYTE * data,int size)
{
    //close runtime use Menus
    for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
	{//1
        CMenu3DObj * menu = (CMenu3DObj *)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
	    if(menu->m_closeOnEsc)
		   menu->CloseMenu(&m_gStatus->m_globalCursorPos);
	}//end 1
	m_gStatus->OpenMenu(99, false); //open group menu  //TODO: Switch this to use a value from the "important menu" array
	//fill data
	int trace = 1;//start at one because of catagorie
    
    int handle = 0;
	
    while(trace < size)
	{//fill in player inventory
       int handle = 0;
	   int stringSize = (int)data[trace];//get string size
	   char * string = new char[stringSize+1];
	   ZeroMemory(string,(stringSize+1));
	   trace++;
       //fill in handle
       BYTE handleParse[4];
	   handleParse[0] = data[trace];trace++;
	   handleParse[1] = data[trace];trace++;
	   handleParse[2] = data[trace];trace++;
	   handleParse[3] = data[trace];trace++;
	   CopyMemory(&handle,handleParse,4);
       //fill in name
	   for(int i=0;i<stringSize;i++){
           string[i] = data[trace];
		   trace++;
	   }

       m_gStatus->AddStringToMenuList(99,string,-1,handle,0,string,0,"");
	   if(string)
	   {
          delete [] string;
		  string = 0;
	   }
	}//end fill in player inventory

	if(data)
	{
	  delete [] data;
	  data = 0;
	}

	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CClientNetworkingLogic::ParseClanRosterData(BYTE * data,int size)
{
    //close runtime use Menus
    for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
	{//1
        CMenu3DObj * menu = (CMenu3DObj *)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
	    if(menu->m_closeOnEsc)
		   menu->CloseMenu(&m_gStatus->m_globalCursorPos);
	}//end 1
	m_gStatus->OpenMenu(800, false); //TODO: Switch this to use a value from the "important menu" array
	//fill data
	int trace = 1;//start at one because of catagorie
    
    int handle = 0;
	
    while(trace < size)
	{//fill in player inventory
       int handle = 0;
	   int stringSize = (int)data[trace];//get string size
	   char * string = new char[stringSize+1];
	   ZeroMemory(string,(stringSize+1));
	   trace++;
       //fill in name
	   for(int i=0;i<stringSize;i++){
           string[i] = data[trace];
		   trace++;
	   }

       m_gStatus->AddStringToMenuList(800,string,-1,handle,0,string,0,"");
	   if(string)
	   {
          delete [] string;
		  string = 0;
	   }
	}//end fill in player inventory

	if(data)
	{
	  delete [] data;
	  data = 0;
	}

	return TRUE;
}
//--------------send message----------------------------------------------------------//
BOOL CClientNetworkingLogic::HandleSkillAbilitiesClientSide(BYTE * byteBlock,
															int msgSz,
															DPNID idFromLoc)
{
    LPSKILLABILITIES_HEADER formatedMessage = (LPSKILLABILITIES_HEADER)byteBlock;
    int headerSize = sizeof(SKILLABILITIES_HEADER);
	int dataTrack = headerSize;

	//close existing
	for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
	{//emergency close all menus
         CMenu3DObj * menu = (CMenu3DObj *)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
	     if(menu->m_closeOnEsc)
	     menu->CloseMenu(&m_gStatus->m_globalCursorPos);
	}//end emergency close all menus
    //open menu
	m_gStatus->OpenMenu(formatedMessage->m_menuID, false);  //TODO: Switch this to use a value from the "important menu" array

    while(dataTrack < msgSz)
	{
        int textSz = byteBlock[dataTrack];
		dataTrack++;

		char * textFinal = new char[textSz+1];

		for(int loop=0;loop<textSz;loop++)
		{
		    textFinal[loop] = byteBlock[dataTrack];
		    dataTrack++;
		}
		textFinal[textSz] = 0;
		m_gStatus->AddStringToMenuList(formatedMessage->m_menuID,textFinal,-1,formatedMessage->m_glid,0,textFinal,1,"");
	}

	return TRUE;
}
//--------------send message----------------------------------------------------------//
BOOL CClientNetworkingLogic::HandleSkillsOutputClientSide(  BYTE * byteBlock,
															int msgSz,
															DPNID idFromLoc)
{
    LPSKILLS_HEADER formatedMessage = (LPSKILLS_HEADER)byteBlock;
    int headerSize = sizeof(SKILLS_HEADER);
	int dataTrack = headerSize;

	//close existing
	for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
	{//emergency close all menus
         CMenu3DObj * menu = (CMenu3DObj *)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
	     if(menu->m_closeOnEsc)
	     menu->CloseMenu(&m_gStatus->m_globalCursorPos);
	}//end emergency close all menus
    //open menu
	m_gStatus->OpenMenu(141, false); //TODO: Switch this to use a value from the "important menu" array

    for(int loop=0;loop<formatedMessage->m_skillCount;loop++)
	{
		int type = byteBlock[dataTrack];
		dataTrack++;
		CString level = m_gStatus->m_cabLib.ConvToString(((int)byteBlock[dataTrack]));
		dataTrack++;
        CString percentageTillNextLevel = m_gStatus->m_cabLib.ConvToString(((int)byteBlock[dataTrack]));
		dataTrack++;
		
	    CSkillObject * skillResult = (CSkillObject *)m_gStatus->m_skillDatabase->GetSkillByType(type);
        if(skillResult)
		{
           CString build = skillResult->m_skillName+" Level:";
		   build = build+level;
		   build = build+" %";
		   build = build+percentageTillNextLevel;
		   m_gStatus->AddStringToMenuList(141,build,-1,0,0,build,0,"");
		}
		else
		{
           CString build = "Unknown Skill Level:";
		   build = build+level;
		   m_gStatus->AddStringToMenuList(141,build,-1,0,0,build,0,"");
		}
	}

	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CClientNetworkingLogic::HandleSkillTitlesOutputClientSide(	BYTE * byteBlock,
																int msgSz,
																DPNID idFromLoc)
{
	//Find the queue entry that's waiting for this attribute message
	CAttribQueueEntry *curEntry;
	POSITION posLast, posQueue = m_gStatus->m_attribQueue->GetHeadPosition();
	while((posLast = posQueue) != NULL)
	{
		curEntry = (CAttribQueueEntry*)m_gStatus->m_attribQueue->GetNext(posQueue);
		if(curEntry->m_requestedAttrib == 43)
		{
			delete curEntry;
			curEntry = 0;
			m_gStatus->m_attribQueue->RemoveAt(posLast);
			break;
		}
	}

	//If we didn't find the entry, return
	if(posLast == NULL)
		return FALSE;

	LPSKILLS_HEADER formatedMessage = (LPSKILLS_HEADER)byteBlock;
	int headerSize = sizeof(SKILLS_HEADER);
	int dataTrack = headerSize;

    for(int loop = 0; loop < formatedMessage->m_skillCount; loop++)
	{
		int type = byteBlock[dataTrack];
		dataTrack++;
		int experience = (int)byteBlock[dataTrack];
		dataTrack++;
		
	    CSkillObject *baseSkill = (CSkillObject*)m_gStatus->m_skillDatabase->GetSkillByType(type);
        if(baseSkill)
		{
			POSITION posLevel = baseSkill->m_levelRangeDB->GetHeadPosition();
			while(posLevel)
			{
				CLevelRangeObject *level = (CLevelRangeObject*)baseSkill->m_levelRangeDB->GetNext(posLevel);
				if(level->m_rangeStart < experience && level->m_rangeEnd > experience)
					m_gStatus->AddStringToMenuList(curEntry->m_miscInt1, level->m_levelTitle, -1, type, 0, level->m_levelTitle, 0, "");
			}
		}
		else
			m_gStatus->AddStringToMenuList(curEntry->m_miscInt1, "Unknown Skill Level", -1, 0, 0, "Unknown Skill Level", 0, "");
	}

	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CClientNetworkingLogic::SendAI_GuerenteedCycleMsgToServer()
{
	
	//lock size and stuffs
	m_gStatus->m_multiplayerObject->m_attributeOutBox->EnterCriticalSectionEXA();
	m_gStatus->m_multiplayerObject->m_dmgOutBox->EnterCriticalSectionEXA();
    m_gStatus->m_multiplayerObject->m_txtOutBox->EnterCriticalSectionEXA();
	m_gStatus->m_multiplayerObject->m_equipOutBox->EnterCriticalSectionEXA();
	m_gStatus->m_multiplayerObject->m_txtgroupOutBox->EnterCriticalSectionEXA();
	m_gStatus->m_multiplayerObject->m_miscOutBox->EnterCriticalSectionEXA();
	m_gStatus->m_multiplayerObject->m_fireOutBox->EnterCriticalSectionEXA();



    AI_ENTITYGUPDATE_HEADER header;
	header.m_catagorie          = 28;
	header.m_attributeMsgCount  = (int)m_gStatus->m_multiplayerObject->m_attributeOutBox->GetCount();
    header.m_dmgMsgCount        = (int)m_gStatus->m_multiplayerObject->m_dmgOutBox->GetCount();
	header.m_txtMsgCount        = (int)m_gStatus->m_multiplayerObject->m_txtOutBox->GetCount();
	header.m_equipMsgCount      = (int)m_gStatus->m_multiplayerObject->m_equipOutBox->GetCount();
	header.m_updateStatMsgCount = 0;
	header.m_grpTxtMsgCount     = (int)m_gStatus->m_multiplayerObject->m_txtgroupOutBox->GetCount();
	header.m_miscMsgCount       = (int)m_gStatus->m_multiplayerObject->m_miscOutBox->GetCount();
	header.m_fireMsgCount       = (int)m_gStatus->m_multiplayerObject->m_fireOutBox->GetCount();

	if(header.m_attributeMsgCount == 0 &&
	   header.m_dmgMsgCount == 0 &&
	   header.m_txtMsgCount == 0 && 
	   header.m_equipMsgCount == 0 &&
	   header.m_updateStatMsgCount == 0 &&
	   header.m_grpTxtMsgCount == 0 &&
	   header.m_miscMsgCount == 0 && 
	   header.m_fireMsgCount == 0)
	{
       m_gStatus->m_multiplayerObject->m_attributeOutBox->LeaveCriticalSectionEXA();
	   m_gStatus->m_multiplayerObject->m_dmgOutBox->LeaveCriticalSectionEXA();
       m_gStatus->m_multiplayerObject->m_txtOutBox->LeaveCriticalSectionEXA();
	   m_gStatus->m_multiplayerObject->m_equipOutBox->LeaveCriticalSectionEXA();
	   m_gStatus->m_multiplayerObject->m_txtgroupOutBox->LeaveCriticalSectionEXA();
	   m_gStatus->m_multiplayerObject->m_miscOutBox->LeaveCriticalSectionEXA();
	   m_gStatus->m_multiplayerObject->m_fireOutBox->LeaveCriticalSectionEXA();

       return FALSE;//nothin here
	}
	   

    //get txt size
	int txtDataSize = 0;
	POSITION szPos;
    for(szPos = m_gStatus->m_multiplayerObject->m_txtOutBox->GetHeadPosition();szPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) m_gStatus->m_multiplayerObject->m_txtOutBox->GetNext(szPos);
		txtDataSize += (pkPtr->m_size+1);
	}
	//get group txt size
	int grptxtDataSize = 0;
    for(szPos = m_gStatus->m_multiplayerObject->m_txtgroupOutBox->GetHeadPosition();szPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) m_gStatus->m_multiplayerObject->m_txtgroupOutBox->GetNext(szPos);
		grptxtDataSize += (pkPtr->m_size+1);
	}
	//get misc size
	int miscDataSize = 0;
    for(szPos = m_gStatus->m_multiplayerObject->m_miscOutBox->GetHeadPosition();szPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) m_gStatus->m_multiplayerObject->m_miscOutBox->GetNext(szPos);
		miscDataSize += (pkPtr->m_size+1);
	}
	//get misc size
	int fireDataSize = 0;
    for(szPos = m_gStatus->m_multiplayerObject->m_fireOutBox->GetHeadPosition();szPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) m_gStatus->m_multiplayerObject->m_fireOutBox->GetNext(szPos);
		fireDataSize += (pkPtr->m_size+1);
	}

	
    int messageSize = (sizeof(AI_ENTITYGUPDATE_HEADER))+
		              (sizeof(ATTRIB_STRUCT) * header.m_attributeMsgCount)+
					  (sizeof(HITPLAYER_DATA) * header.m_dmgMsgCount)+
					  (sizeof(EQUIP_STRUCT) * header.m_equipMsgCount)+
					  grptxtDataSize+
					  txtDataSize+
					  miscDataSize+
					  fireDataSize;

    BYTE * dataPacket = new BYTE[messageSize];

    CopyMemory(dataPacket,&header,sizeof(AI_ENTITYGUPDATE_HEADER));
    int dataTracer = sizeof(AI_ENTITYGUPDATE_HEADER);
    //attribute
	POSITION pkPos;
	for(pkPos = m_gStatus->m_multiplayerObject->m_attributeOutBox->GetHeadPosition();pkPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) m_gStatus->m_multiplayerObject->m_attributeOutBox->GetNext(pkPos);
        CopyMemory(&dataPacket[dataTracer],pkPtr->m_data,pkPtr->m_size);
		dataTracer += pkPtr->m_size;
	}
	//dmgs 
	for(pkPos = m_gStatus->m_multiplayerObject->m_dmgOutBox->GetHeadPosition();pkPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) m_gStatus->m_multiplayerObject->m_dmgOutBox->GetNext(pkPos);
		CopyMemory(&dataPacket[dataTracer],pkPtr->m_data,pkPtr->m_size);
		dataTracer += pkPtr->m_size;
	}
	//equip msgs 
	for(pkPos = m_gStatus->m_multiplayerObject->m_equipOutBox->GetHeadPosition();pkPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) m_gStatus->m_multiplayerObject->m_equipOutBox->GetNext(pkPos);
		CopyMemory(&dataPacket[dataTracer],pkPtr->m_data,pkPtr->m_size);
		dataTracer += pkPtr->m_size;
	}
    //txt msgs 
	for(pkPos = m_gStatus->m_multiplayerObject->m_txtOutBox->GetHeadPosition();pkPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) m_gStatus->m_multiplayerObject->m_txtOutBox->GetNext(pkPos);
        
		dataPacket[dataTracer] = pkPtr->m_size;
		dataTracer++;
		CopyMemory(&dataPacket[dataTracer],pkPtr->m_data,pkPtr->m_size);
		dataTracer += pkPtr->m_size;
	}
	//grp txt msgs 
	for(pkPos = m_gStatus->m_multiplayerObject->m_txtgroupOutBox->GetHeadPosition();pkPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) m_gStatus->m_multiplayerObject->m_txtgroupOutBox->GetNext(pkPos);
        
		dataPacket[dataTracer] = pkPtr->m_size;
		dataTracer++;
		CopyMemory(&dataPacket[dataTracer],pkPtr->m_data,pkPtr->m_size);
		dataTracer += pkPtr->m_size;
	}
	//misc msgs
	for(pkPos = m_gStatus->m_multiplayerObject->m_miscOutBox->GetHeadPosition();pkPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) m_gStatus->m_multiplayerObject->m_miscOutBox->GetNext(pkPos);
        
		dataPacket[dataTracer] = pkPtr->m_size;
		dataTracer++;
		CopyMemory(&dataPacket[dataTracer],pkPtr->m_data,pkPtr->m_size);
		dataTracer += pkPtr->m_size;
	}
	//fire msgs
	for(pkPos = m_gStatus->m_multiplayerObject->m_fireOutBox->GetHeadPosition();pkPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) m_gStatus->m_multiplayerObject->m_fireOutBox->GetNext(pkPos);
        
		dataPacket[dataTracer] = pkPtr->m_size;
		dataTracer++;
		CopyMemory(&dataPacket[dataTracer],pkPtr->m_data,pkPtr->m_size);
		dataTracer += pkPtr->m_size;
	}

    m_gStatus->m_multiplayerObject->SendData(dataPacket,
											messageSize,
											DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH,
											DPID_SERVERPLAYER,
											m_gStatus->m_multiplayerObject->m_packetTimeout);

	delete [] dataPacket;
	dataPacket = 0;

	m_gStatus->m_multiplayerObject->m_attributeOutBox->Flush();
	m_gStatus->m_multiplayerObject->m_dmgOutBox->Flush();
	m_gStatus->m_multiplayerObject->m_equipOutBox->Flush();
	m_gStatus->m_multiplayerObject->m_txtOutBox->Flush();
	m_gStatus->m_multiplayerObject->m_txtgroupOutBox->Flush();
	m_gStatus->m_multiplayerObject->m_miscOutBox->Flush();
	m_gStatus->m_multiplayerObject->m_fireOutBox->Flush();

	m_gStatus->m_multiplayerObject->m_attributeOutBox->LeaveCriticalSectionEXA();
	m_gStatus->m_multiplayerObject->m_dmgOutBox->LeaveCriticalSectionEXA();
    m_gStatus->m_multiplayerObject->m_txtOutBox->LeaveCriticalSectionEXA();
    m_gStatus->m_multiplayerObject->m_equipOutBox->LeaveCriticalSectionEXA();
	m_gStatus->m_multiplayerObject->m_txtgroupOutBox->LeaveCriticalSectionEXA();
	m_gStatus->m_multiplayerObject->m_miscOutBox->LeaveCriticalSectionEXA();
	m_gStatus->m_multiplayerObject->m_fireOutBox->LeaveCriticalSectionEXA();

	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CClientNetworkingLogic::SendGuerenteedCycleMsgToServer()
{
    ENTITYGUPDATE_HEADER header;
	header.m_catagorie          = 26;
	
	header.m_attributeMsgCount  = (signed char)m_gStatus->m_multiplayerObject->m_attributeOutBox->GetCount();
    if(header.m_attributeMsgCount > MAXATTRIBUTESEND || header.m_attributeMsgCount < 0)
		header.m_attributeMsgCount = MAXATTRIBUTESEND;//stream these at 5 max

	if(header.m_attributeMsgCount > 100)//major spam detection
		m_gStatus->m_systemReadout->AddLine("SendGuerenteedCycleMsgToServer-ATTRIBUTE SPAM:",(DWORD)header.m_attributeMsgCount);
	
	
	header.m_dmgMsgCount        = (signed char)m_gStatus->m_multiplayerObject->m_dmgOutBox->GetCount();
	header.m_txtMsgCount        = (signed char)m_gStatus->m_multiplayerObject->m_txtOutBox->GetCount();
	header.m_equipMsgCount      = (signed char)m_gStatus->m_multiplayerObject->m_equipOutBox->GetCount();
	header.m_updateStatMsgCount = 0;
	header.m_grpTxtMsgCount     = (signed char)m_gStatus->m_multiplayerObject->m_txtgroupOutBox->GetCount();
	header.m_miscMsgCount       = (signed char)m_gStatus->m_multiplayerObject->m_miscOutBox->GetCount();

	if(header.m_attributeMsgCount == 0 &&
	   header.m_dmgMsgCount == 0 &&
	   header.m_txtMsgCount == 0 && 
	   header.m_equipMsgCount == 0 &&
	   header.m_updateStatMsgCount == 0 &&
	   header.m_grpTxtMsgCount == 0 &&
	   header.m_miscMsgCount == 0)
	   return FALSE;//nothin here

    //get txt size
	int txtDataSize = 0;
	POSITION szPos;
    for(szPos = m_gStatus->m_multiplayerObject->m_txtOutBox->GetHeadPosition();szPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) m_gStatus->m_multiplayerObject->m_txtOutBox->GetNext(szPos);
		txtDataSize += (pkPtr->m_size+1);
	}
	//get group txt size
	int grptxtDataSize = 0;
    for(szPos = m_gStatus->m_multiplayerObject->m_txtgroupOutBox->GetHeadPosition();szPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) m_gStatus->m_multiplayerObject->m_txtgroupOutBox->GetNext(szPos);
		grptxtDataSize += (pkPtr->m_size+1);
	}
	//get misc size
	int miscDataSize = 0;
    for(szPos = m_gStatus->m_multiplayerObject->m_miscOutBox->GetHeadPosition();szPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) m_gStatus->m_multiplayerObject->m_miscOutBox->GetNext(szPos);
		miscDataSize += (pkPtr->m_size+1);
	}

	
    int messageSize = (sizeof(ENTITYGUPDATE_HEADER))+
		              (sizeof(ATTRIB_STRUCT) * header.m_attributeMsgCount)+
					  (sizeof(HITPLAYER_DATA) * header.m_dmgMsgCount)+
					  (sizeof(EQUIP_STRUCT) * header.m_equipMsgCount)+
					  grptxtDataSize+
					  txtDataSize+
					  miscDataSize;

    BYTE * dataPacket = new BYTE[messageSize];

    CopyMemory(dataPacket,&header,sizeof(ENTITYGUPDATE_HEADER));
    int dataTracer = sizeof(ENTITYGUPDATE_HEADER);
    //attributes
	POSITION pkPos;
	int streamTrace = 0;
	for(pkPos = m_gStatus->m_multiplayerObject->m_attributeOutBox->GetHeadPosition();pkPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) m_gStatus->m_multiplayerObject->m_attributeOutBox->GetNext(pkPos);
        if(streamTrace >= header.m_attributeMsgCount)
		   break;
		
		if(pkPtr->m_size > 0)
		   CopyMemory(&dataPacket[dataTracer],pkPtr->m_data,pkPtr->m_size);

		dataTracer += pkPtr->m_size;

		streamTrace++;
	}
	//dmgs 
	for(pkPos = m_gStatus->m_multiplayerObject->m_dmgOutBox->GetHeadPosition();pkPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) m_gStatus->m_multiplayerObject->m_dmgOutBox->GetNext(pkPos);
        if(pkPtr->m_size > 0)
		   CopyMemory(&dataPacket[dataTracer],pkPtr->m_data,pkPtr->m_size);

		dataTracer += pkPtr->m_size;
	}
	//equip msgs 
	for(pkPos = m_gStatus->m_multiplayerObject->m_equipOutBox->GetHeadPosition();pkPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) m_gStatus->m_multiplayerObject->m_equipOutBox->GetNext(pkPos);
        if(pkPtr->m_size > 0)
		   CopyMemory(&dataPacket[dataTracer],pkPtr->m_data,pkPtr->m_size);

		dataTracer += pkPtr->m_size;
	}
    //txt msgs 
	for(pkPos = m_gStatus->m_multiplayerObject->m_txtOutBox->GetHeadPosition();pkPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) m_gStatus->m_multiplayerObject->m_txtOutBox->GetNext(pkPos);
        
		dataPacket[dataTracer] = pkPtr->m_size;
		dataTracer++;
		
		if(pkPtr->m_size > 0)
		   CopyMemory(&dataPacket[dataTracer],pkPtr->m_data,pkPtr->m_size);

		dataTracer += pkPtr->m_size;
	}
	//grp txt msgs 
	for(pkPos = m_gStatus->m_multiplayerObject->m_txtgroupOutBox->GetHeadPosition();pkPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) m_gStatus->m_multiplayerObject->m_txtgroupOutBox->GetNext(pkPos);
        
		dataPacket[dataTracer] = pkPtr->m_size;
		dataTracer++;

        if(pkPtr->m_size > 0)
		   CopyMemory(&dataPacket[dataTracer],pkPtr->m_data,pkPtr->m_size);

		dataTracer += pkPtr->m_size;
	}
	//misc msgs
	for(pkPos = m_gStatus->m_multiplayerObject->m_miscOutBox->GetHeadPosition();pkPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) m_gStatus->m_multiplayerObject->m_miscOutBox->GetNext(pkPos);
        
		dataPacket[dataTracer] = pkPtr->m_size;
		dataTracer++;

        if(pkPtr->m_size > 0)
		   CopyMemory(&dataPacket[dataTracer],pkPtr->m_data,pkPtr->m_size);

		dataTracer += pkPtr->m_size;
	}



    m_gStatus->m_multiplayerObject->SendData(dataPacket,
				                  messageSize,
				                  DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH,
				                  DPID_SERVERPLAYER,
								  m_gStatus->m_multiplayerObject->m_packetTimeout);

	delete [] dataPacket;
	dataPacket = 0;

	m_gStatus->m_multiplayerObject->m_attributeOutBox->Flush(MAXATTRIBUTESEND);
	m_gStatus->m_multiplayerObject->m_dmgOutBox->Flush();
	m_gStatus->m_multiplayerObject->m_equipOutBox->Flush();
	m_gStatus->m_multiplayerObject->m_txtOutBox->Flush();
	m_gStatus->m_multiplayerObject->m_txtgroupOutBox->Flush();
	m_gStatus->m_multiplayerObject->m_miscOutBox->Flush();

	return TRUE;
}
//--------------send message----------------------------------------------------------//
BOOL CClientNetworkingLogic::HandleHouseingBlock(BYTE * byteBlock,int msgSz)
{
   
	int structSz = sizeof(HOUSE_DATA);
	int houseStructureCount = msgSz/structSz;


	if(!m_gStatus->m_housingDB)
	{
       m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Invalid House Database");
       houseStructureCount = 0;
	}

	if(!m_gStatus->m_multiplayerObject->m_houseingDB)
	{
       m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Invalid Housing Zones Database");
       houseStructureCount = 0;
	}

	if(houseStructureCount == 0)
	{
	   delete [] byteBlock;
	   byteBlock = 0;
	   return FALSE;
	}

	

	HOUSE_DATA * houseBlockData = new HOUSE_DATA[houseStructureCount];
	//BYTE * conversionBlock = (BYTE*)houseBlockData;
    
    //for(int i=0;i<((msgSz-4)-1);i++)
    //   conversionBlock[i] = byteBlock[i+1];
	CopyMemory(houseBlockData,&byteBlock[1],((msgSz-4)-1));

    for(int loop=0;loop<houseStructureCount;loop++)
	{
        CHousingObj * houseBasePtr = m_gStatus->m_housingDB->GetByIndex(houseBlockData[loop].m_houseType);
	    if(houseBasePtr)
		{//valid database base
		   int houseID = 0;
           BNDBOX clipBoxCast;
		   D3DVECTOR wldPosition;
		   wldPosition.x = houseBlockData[loop].m_posX;
		   wldPosition.y = houseBlockData[loop].m_posY;
		   wldPosition.z = houseBlockData[loop].m_posZ;
           clipBoxCast.max.x = houseBasePtr->m_clipBox.maxX;
		   clipBoxCast.max.y = houseBasePtr->m_clipBox.maxY;
		   clipBoxCast.max.z = houseBasePtr->m_clipBox.maxZ;
		   clipBoxCast.min.x = houseBasePtr->m_clipBox.minX;
	       clipBoxCast.min.y = houseBasePtr->m_clipBox.minY;
		   clipBoxCast.min.z = houseBasePtr->m_clipBox.minZ;

		   int result = m_gStatus->m_multiplayerObject->m_houseingDB->AddPlacementObjToTable(houseBlockData[loop].m_houseType,
                                                                                  houseBlockData[loop].m_acID,
                                                                                  clipBoxCast,
										                                          wldPosition,
											                                      &houseID,
																				  -1);
           if(result == 0)
		   {
              m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("House placement failed");
		   }
		   else if(result == 1)
		   {
			   m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("House placement successful");
		   }
		   else if(result == 5)
		   {
			  m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("House placement failed ver 2");
		   }
		}//end valid database base
	}

    delete [] byteBlock;
    byteBlock = 0;
return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CClientNetworkingLogic::HandleHouseStoreDataBlock(LPVOID lpMessage,int & msgSize,DPID & idFromLoc)
{
    BYTE * dataBlock = (BYTE *)lpMessage;
    LPSTORE_BLOCKHEADER header = (LPSTORE_BLOCKHEADER)lpMessage;

        m_gStatus->OpenMenu(header->m_menuBind, false);//open store specified menu
		m_gStatus->SetMenuTextItem(188, m_gStatus->m_cabLib.ConvToString(header->m_storeCash));
        m_gStatus->m_multiplayerObject->m_cashCount = header->m_playerCash;//update bank data
		int headerSize = sizeof(STORE_BLOCKHEADER);
		int itemDataBlockSize = (msgSize-headerSize);
		if(itemDataBlockSize > 0)
		{//valid
		int itemDataSize = sizeof(ITEMDATA_STRUCTEXT);
		int invenCount = (itemDataBlockSize/itemDataSize);
        ITEMDATA_STRUCTEXT * itemDataBlock = new ITEMDATA_STRUCTEXT[invenCount];
		BYTE * conversionBlock = (BYTE*)itemDataBlock;
		int loop=0;
        for(loop=0;loop<itemDataBlockSize;loop++)
            conversionBlock[loop] = dataBlock[loop+headerSize];
        //--------------------
		int tracer = 0;
		for(loop=0;loop<header->m_storeItemCount;loop++)
		{//fill in bank data
          CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(itemDataBlock[loop].m_GLID);
		  if(invObj){
              CString output = invObj->m_itemName;
              CString append;
			  if(itemDataBlock[loop].m_dMod > 0)
			     append = " +" + m_gStatus->m_cabLib.ConvToString(itemDataBlock[loop].m_dMod);
			  output += " cost: ";
			  output += m_gStatus->m_cabLib.ConvToString(itemDataBlock[loop].m_price);
			  if(invObj->m_stackable)
			     output = output+" X"+m_gStatus->m_cabLib.ConvToString(itemDataBlock[loop].m_quanity);
			 m_gStatus->AddStringToMenuList(52,output,invObj->m_visualRef,itemDataBlock[loop].m_GLID,itemDataBlock[loop].m_quanity,invObj->m_itemName+append,1,(" cost: " + m_gStatus->m_cabLib.ConvToString(itemDataBlock[loop].m_price)));
		  }
		  else{//alternate
             m_gStatus->AddStringToMenuList(52,"Unknown",0,itemDataBlock[loop].m_GLID,itemDataBlock[loop].m_quanity,"Unknown",1,"");
		  }//end alternate
		  tracer++;
		}//end fill in bank data
		//----------------------
		
		for(loop=tracer;loop<invenCount;loop++)
		{//fill in player inventory
          CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(itemDataBlock[loop].m_GLID);
		   if(invObj){
             CString output = invObj->m_itemName;
			 CString append;
			 if(itemDataBlock[loop].m_dMod > 0)
			     append = " +" + m_gStatus->m_cabLib.ConvToString(itemDataBlock[loop].m_dMod);
			 
             if(invObj->m_stackable)
			    output = output+" X"+m_gStatus->m_cabLib.ConvToString(itemDataBlock[loop].m_quanity);
             m_gStatus->AddStringToMenuList(51,output,invObj->m_visualRef,itemDataBlock[loop].m_GLID,itemDataBlock[loop].m_quanity,invObj->m_itemName+append,1,"");
		   }
		   else{//alternate
             m_gStatus->AddStringToMenuList(51,"Unknown",-1,itemDataBlock[loop].m_GLID,1,"Unknown",1,"");
		   }//end alternate
		}//end fill in player inventory

        delete [] itemDataBlock;
		itemDataBlock = 0;
		}//end valid

	delete [] dataBlock;
    dataBlock=0;
	return TRUE;
}
//------------------------------------------------------------------------------------//
 BOOL CClientNetworkingLogic::SendDamageMsg(short playerThatGotHitID,
											int   type,
											short miscValue,
											short attackerNetworkID,
											short secUi,
											short targetX,
											short targetY,
											short targetZ)
{
 

	HITPLAYER_DATA dataPacket;
	int messageSize                  = sizeof(HITPLAYER_DATA);
    dataPacket.m_catagorie           = 20;
	dataPacket.m_attackerNetowkID    = attackerNetworkID;
	dataPacket.m_playerThatGotHitID  = playerThatGotHitID;
	dataPacket.m_securityIndexer     = secUi;
	dataPacket.m_miscValue           = miscValue;
	dataPacket.m_type                = type;
	dataPacket.m_targetX             = targetX;
	dataPacket.m_targetY             = targetY;
	dataPacket.m_targetZ             = targetZ;
	dataPacket.CRCCHECK              = m_gStatus->m_multiplayerObject->GetDamageMsgCrC(&dataPacket);
	
    
	m_gStatus->m_multiplayerObject->m_dmgOutBox->AddPacket(messageSize,(BYTE*)&dataPacket,0,0);
  
	return TRUE;
}
//------------------------------------------------------------------------------------//
void CClientNetworkingLogic::UpdateServerCallback(CSpawnObjList * spawnGeneratorDB,
												  CMovementObjList * entObjRuntime,
												  CMissileObjList * missileRuntime)
{
  //long currentTickCount = GetTickCount();
  
    if(!m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
	    return;

	//if client only send client info
	if(!m_gStatus->m_multiplayerObject->m_AIServer)
	{//regular client
		
      DWORD timePassed = m_gStatus->GetElaspeTimeMS_DW(m_gStatus->m_networkFreqStamp);
	  if(timePassed < MultiplayerDelayComm)
		 return;
	  
	  BOOL makeAnReattempt = FALSE;
	  if(m_pingInQuerry == TRUE)//still waiting-must be lagg or packet loss
	  if(timePassed > m_gStatus->m_multiplayerObject->m_currentRetrySendDuration)
	  {
           m_gStatus->m_multiplayerObject->m_currentRetrySendDuration = m_gStatus->m_multiplayerObject->m_currentRetrySendDuration + m_gStatus->m_multiplayerObject->m_currentRetrySendDuration; 
           if(m_gStatus->m_multiplayerObject->m_currentRetrySendDuration > 1500)
		      m_gStatus->m_multiplayerObject->m_currentRetrySendDuration = 1500;
		   
		   makeAnReattempt = TRUE;
		   //tag packet loss or lag
		   m_gStatus->m_multiplayerObject->m_packetsDropped++;
	  }
      
	  if(m_pingInQuerry == FALSE || makeAnReattempt == TRUE)
	  {//timed updates
   	   m_gStatus->m_networkFreqStamp = m_gStatus->GetTimeStamp_DW();

	   if(m_pingInQuerry == FALSE || m_gStatus->m_switchingConnections == TRUE)
	   {
	      m_gStatus->m_multiplayerObject->m_currentRetrySendDuration = m_gStatus->m_multiplayerObject->m_retrySendDuration;
		  m_gStatus->m_switchingConnections = FALSE;
	   }

       

	   if(!makeAnReattempt)
       SendGuerenteedCycleMsgToServer();

	   POSITION posLoc = entObjRuntime->FindIndex(0);
	   if(!posLoc)
		 return;
	     CMovementObj * mainPtr = ( CMovementObj* ) entObjRuntime->GetAt(posLoc);

		 if(mainPtr->List_controlType != 0)//user?
			return;

		 //if(m_gStatus->m_multiplayerObject->m_requestingPortal == TRUE)
		 //return;
		 
		 if(mainPtr->List_networkDefinedID < 1)//has no way of defining who he is
			return;

		 CMovementObj * userPtr = mainPtr;
		 if(mainPtr->m_posses)
            userPtr = mainPtr->m_posses;
		 D3DVECTOR upVect,dirVect,posVect,predictedPosition;

		 userPtr->List_baseFrame->GetOrientation(&dirVect,&upVect);
		 userPtr->List_baseFrame->GetPosition(NULL,&posVect);

		 if(userPtr->m_networkFirstTimeSendingPos){//init prediction

		    predictedPosition.x = posVect.x;
		    predictedPosition.y = posVect.y;
	        predictedPosition.z = posVect.z;

			userPtr->m_networkFirstTimeSendingPos = FALSE;
		 }//end init prediction
		 else{//use prediction

            if(fabs(userPtr->List_movementDB->List_curTransX) > (userPtr->List_movementDB->List_maxSpeed*.2f) || 
               fabs(userPtr->List_movementDB->List_curTransZ) > (userPtr->List_movementDB->List_maxSpeed*.2f))
			{
				   
			   predictedPosition.x = posVect.x + ((posVect.x - userPtr->m_networkLastPositionSent.x)*0.5f);//1.7
		       predictedPosition.y = posVect.y + ((posVect.y - userPtr->m_networkLastPositionSent.y)*0.5f);
	           predictedPosition.z = posVect.z + ((posVect.z - userPtr->m_networkLastPositionSent.z)*0.5f);
		    }
		    else{
               predictedPosition.x = posVect.x;
		       predictedPosition.y = posVect.y;
	           predictedPosition.z = posVect.z;
		    }

            //use collision on prediction model
              //build variables  
			  D3DMATRIX entMatrix;
			  userPtr->List_baseFrame->GetTransform(NULL,&entMatrix);
			  float characterHieght = userPtr->List_skeletalObject->m_box.maxY;
	          float characterWidth = userPtr->List_skeletalObject->m_box.maxX;
	           if(userPtr->List_skeletalObject->m_box.maxY > userPtr->List_skeletalObject->m_box.maxX)
	              characterWidth = userPtr->List_skeletalObject->m_box.maxY;
			
			      /*m_gStatus->VirtualCollisionAdjust(&predictedPosition,
													predictedPosition,
													posVect,
													(int)characterWidth,
													(int)characterHieght,
													entMatrix,
													(int)userPtr->List_movementDB->m_antiGravity);*/

		 }//end use prediction

		 userPtr->m_networkLastPositionSent.x = posVect.x;
		 userPtr->m_networkLastPositionSent.y = posVect.y;
		 userPtr->m_networkLastPositionSent.z = posVect.z;
		 
         int animInUseCnv = userPtr->List_skeletalObject->m_animationInUse;
         if(userPtr->List_skeletalObject->GetAnimationDirection(animInUseCnv)==TRUE)//playing reverse?
			animInUseCnv = animInUseCnv * -1;

	     //m_gStatus->m_multiplayerObject->m_lastElLocalStamp = m_gStatus->GetTimeStamp();
		 
	     #ifdef LEGACYEA
         SendMovMsg(DPNSEND_NOCOMPLETE | DPNSEND_PRIORITY_HIGH,
														DPID_SERVERPLAYER,
														predictedPosition,
														mainPtr->List_networkDefinedID,
														dirVect, //here are the dir and up vectors
														upVect,
														mainPtr->List_skeletalObject->m_freeLookYaw,
														mainPtr->List_skeletalObject->m_freeLookPitch,
														userPtr->List_refModel,
														animInUseCnv,
														mainPtr->List_skeletalObject->m_secondaryAnimationInUse,
														(long)m_gStatus->GetElaspeTimeMS(m_gStatus->m_multiplayerObject->m_timeElStamp));
         #endif
		 #ifndef LEGACYEA
		 LPSHRUNK_TOSVRMOV_STRUCT movStruct = GetMovMsg(DPNSEND_NOCOMPLETE | DPNSEND_PRIORITY_HIGH,
														DPID_SERVERPLAYER,
														predictedPosition,
														mainPtr->List_networkDefinedID,
														dirVect, //here are the dir and up vectors
														upVect,
														mainPtr->List_skeletalObject->m_freeLookYaw,
														mainPtr->List_skeletalObject->m_freeLookPitch,
														userPtr->List_refModel,
														animInUseCnv,
														mainPtr->List_skeletalObject->m_secondaryAnimationInUse,
														(long)m_gStatus->GetElaspeTimeMS(m_gStatus->m_multiplayerObject->m_timeElStamp));
		 //Build data packet
		 int fireMsgs = (int)m_gStatus->m_multiplayerObject->m_fireOutBox->GetCount();
         int msgSize = (sizeof(FIRE_STRUCT)*fireMsgs)+sizeof(SHRUNK_TOSVRMOV_STRUCT)+2;
		 byte * msgBlock = new byte[msgSize];
		 int dataTrace = 0;
         msgBlock[dataTrace++] = 5;//catagorie 5
		 msgBlock[dataTrace++] = fireMsgs;//fire msg count
		 CopyMemory(&msgBlock[dataTrace],movStruct,sizeof(SHRUNK_TOSVRMOV_STRUCT));
		 dataTrace += sizeof(SHRUNK_TOSVRMOV_STRUCT);
		 for(POSITION posLoc = m_gStatus->m_multiplayerObject->m_fireOutBox->GetHeadPosition();posLoc != NULL;)
		 {
		     CPacketObj * packPtr = (CPacketObj *)m_gStatus->m_multiplayerObject->m_fireOutBox->GetNext(posLoc);
			 CopyMemory(&msgBlock[dataTrace],packPtr->m_data,sizeof(FIRE_STRUCT));
		     dataTrace += packPtr->m_size;
		 }
		 m_gStatus->m_multiplayerObject->m_fireOutBox->Flush();//Flush it
         m_gStatus->m_multiplayerObject->SendData((BYTE *)  msgBlock,
															dataTrace,
															DPNSEND_NOCOMPLETE | DPNSEND_PRIORITY_HIGH,
															DPID_SERVERPLAYER,
															m_gStatus->m_multiplayerObject->m_packetTimeout);
		 delete [] msgBlock;
		 msgBlock = 0;
		 delete movStruct;
		 movStruct = 0;
         #endif
		 //end build data packet

		 if(!m_pingInQuerry)
		 {
		   m_pingPersistStamp = m_gStatus->GetTimeStamp_DW();//simulate ping here fakers
           m_pingInQuerry = TRUE;
		 }
	  }//timed updates
	}//end regular client
	/*else{//AI server SUS update to management server
		DWORD currentTickCount = GetTickCount();
	  if(m_pingInQuerry == FALSE || m_gStatus->GetElaspeTimeMS_DW(m_gStatus->m_networkFreqStamp) > 3000)
	  if(m_gStatus->GetElaspeTimeMS_DW(m_gStatus->m_networkFreqStamp) > m_gStatus->m_multiplayerObject->m_aiserverCycleDuration)
	  {//timed updates
		 BOOL regeneratedOne = FALSE;
         //verify outgoing messages first before continueing
   	     m_gStatus->m_networkFreqStamp = m_gStatus->GetTimeStamp_DW();
		 POSITION posLast = NULL;
		 int indexTrace   = 0;
		 BOOL aiIsSpawnedRequiresInfo = FALSE; 
         int sendCount = 0;
		 POSITION posLoc;
		 for(posLoc = entObjRuntime->GetHeadPosition();(posLast = posLoc) != NULL;)
		 {//size loop
			 CMovementObj * mObj = (CMovementObj *)entObjRuntime->GetNext(posLoc);
			 if(mObj->List_networkDefinedID > 0)
			 {//has communication ID
			   if(mObj->List_controlType == 5){//AI cfg
                sendCount++;
			   }//end AI cfg
			 }
			 else if(mObj->List_networkDefinedID == 0)
			 {
				 if(mObj->List_controlType == 5)
				 if((currentTickCount - mObj->m_spawnTimeStamp) > 15000)
				 {
					 m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Retrying spawn:" + mObj->List_name + " Loc ID:" + m_gStatus->m_cabLib.ConvToString(mObj->List_traceNumber)+ " Net ID:",(DWORD)mObj->List_networkDefinedID);
					 entObjRuntime->RemoveAt(posLast);
					 m_gStatus->DestroyRuntimeEntity(mObj,NULL,entObjRuntime);
					 mObj = 0;
				 }
			 }
		 }//end size loop

		 SHRUNK_MOVE_STRUCT * movBlackData = NULL;
		 if(sendCount > 0)
		    movBlackData = new SHRUNK_MOVE_STRUCT[sendCount];

		 int trace = 0;
         for(posLoc = entObjRuntime->GetHeadPosition();(posLast = posLoc) != NULL;)
		 {//runtime ai loop
			 CMovementObj * mObj = (CMovementObj *)entObjRuntime->GetNext(posLoc);
			 
			 
			 
			 if(mObj->List_networkDefinedID > 0)//has communication ID
			 if(mObj->List_controlType == 5){//AI cfg
				//m_gStatus->IndexEntityAnimations(mObj,GetTickCount(),TRUE,missileRuntime);
                D3DVECTOR upVect,dirVect,posVect,predictedPosition;
		        mObj->List_baseFrame->GetOrientation(&dirVect,&upVect);
		        mObj->List_baseFrame->GetPosition(NULL,&posVect);

                if(mObj->m_networkFirstTimeSendingPos){//init prediction
		           predictedPosition.x = posVect.x;
		           predictedPosition.y = posVect.y;
	               predictedPosition.z = posVect.z;
			       mObj->m_networkFirstTimeSendingPos = FALSE;
				}//end init prediction
		        else{//use prediction
                   if(fabs(mObj->List_movementDB->List_curTransX) > (mObj->List_movementDB->List_maxSpeed*.2f) || 
                      fabs(mObj->List_movementDB->List_curTransZ) > (mObj->List_movementDB->List_maxSpeed*.2f))
				   {
				   
			       predictedPosition.x = posVect.x + ((posVect.x - mObj->m_networkLastPositionSent.x)*.5f);//1.7
		           predictedPosition.y = posVect.y + ((posVect.y - mObj->m_networkLastPositionSent.y)*.5f);
	               predictedPosition.z = posVect.z + ((posVect.z - mObj->m_networkLastPositionSent.z)*.5f);
				   }
				   else{
                   predictedPosition.x = posVect.x;
		           predictedPosition.y = posVect.y;
	               predictedPosition.z = posVect.z;
				   }
				}//end use prediction

		        mObj->m_networkLastPositionSent.x = posVect.x;
		        mObj->m_networkLastPositionSent.y = posVect.y;
		        mObj->m_networkLastPositionSent.z = posVect.z;
				
				int animInUseCnv = mObj->List_skeletalObject->m_animationInUse;
                if(mObj->List_skeletalObject->GetAnimationDirection(animInUseCnv)==TRUE)//playing reverse?
			       animInUseCnv = animInUseCnv * -1;
                   aiIsSpawnedRequiresInfo = TRUE;
                   

				   MOVE_STRUCT mvMsgBld;
                   mvMsgBld.m_catagorie    = 2;
	               mvMsgBld.FreeLookYaw    = mObj->List_skeletalObject->m_freeLookYaw;
                   mvMsgBld.FreeLookPitch  = mObj->List_skeletalObject->m_freeLookPitch;
	               mvMsgBld.Xpos           = predictedPosition.x;
	               mvMsgBld.Ypos           = predictedPosition.y;
	               mvMsgBld.Zpos           = predictedPosition.z;
                   mvMsgBld.idRefrence     = (short)mObj->List_networkDefinedID;
                   mvMsgBld.OrientateDirX  = dirVect.x;
         	       mvMsgBld.OrientateDirY  = dirVect.y;
	               mvMsgBld.OrientateDirZ  = dirVect.z;
	               mvMsgBld.OrientateUpX   = upVect.x;
	               mvMsgBld.OrientateUpY   = upVect.y;
	               mvMsgBld.OrientateUpZ   = upVect.z;
	               //mvMsgBld.dbIndex        = mObj->List_refModel;
	               mvMsgBld.curAnim        = animInUseCnv;
	               mvMsgBld.secondaryAnim  = mObj->List_skeletalObject->m_secondaryAnimationInUse;

                   SHRUNK_MOVE_STRUCT shMovMsg;
                   m_gStatus->m_multiplayerObject->ShrinkData(&mvMsgBld,&shMovMsg);

				   CopyMemory(&movBlackData[trace],&shMovMsg,sizeof(SHRUNK_MOVE_STRUCT));
                   BOOL regenerate = FALSE;

				   if(!regeneratedOne)
				   if(mObj->List_AI_target == -1)//not in battle
				   if((currentTickCount - mObj->m_spawnTimeStamp) > 10800000)//3 hours no regen since then
					   regenerate = TRUE;

				   if(!regeneratedOne)
				   if(mObj->m_lifeTimeMax != 0)//ai has a limit on life
				   if((currentTickCount - mObj->m_spawnTimeStamp) > mObj->m_lifeTimeMax)//life line ended
					   regenerate = TRUE;

				   trace++;
                   if(mObj->List_energy < 1 || posVect.y < -30000 || regenerate == TRUE)
				   {//dead entity
                      if(posVect.y < -30000)
					  {//out of bounds
						 
						 
                         CString build = "Out of bounds:"+mObj->List_name;
						 build = build+" index:";
						 build = build+m_gStatus->m_cabLib.ConvToString(mObj->List_SpawnGeneratorLink);
						 
					     m_gStatus->m_multiplayerObject->m_eventLog->AddEvent(build);
					     mObj->List_movementDB->List_doGravity = FALSE;//make it dissapear
						 mObj->List_energy = 0;
					  }//end out of bounds

					  if(regenerate)
					  {//been in game too long for ai
						 
						 regeneratedOne = TRUE;
                         CString build = "Regenerating...:"+mObj->List_name;
						 build = build+" index:";
						 build = build+m_gStatus->m_cabLib.ConvToString(mObj->List_SpawnGeneratorLink);
						 
					     m_gStatus->m_multiplayerObject->m_eventLog->AddEvent(build);
					     mObj->List_movementDB->List_doGravity = FALSE;//make it dissapear
						 mObj->List_energy = 0;
						 m_gStatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH,
																		DPID_SERVERPLAYER,
																		2,
																		mObj->List_networkDefinedID,
																		0,
																		0,
																		0,
																		NULL);
					  }//end been in game too long for ai

					  if(mObj->List_movementDB->List_doGravity == FALSE || mObj->List_movementDB->m_inLiquid == TRUE || mObj->List_movementDB->m_floatWhenDead == TRUE)
					  {//not in air
						 entObjRuntime->RemoveAt(posLast);
						 m_gStatus->DestroyRuntimeEntity(mObj,NULL,entObjRuntime);
						 mObj = 0;
						 continue;
                         //end remove entity
					  }//end not in air
				   }//end dead entity
				
			 }//end AI cfg
		 }//end runtime ai loop
         //send block data
         if(trace > 0)
		 {//send block update
			if(movBlackData)
			{
               SendAIBlockMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH,
						                           DPID_SERVERPLAYER,
					                               movBlackData,
						                           trace
						                           );
               delete [] movBlackData;
			   movBlackData = 0;
			}
			
		 }//end send block update
		 if(movBlackData)
		 {//nothing moved this tick - still free the block (was leaked)
			delete [] movBlackData;
			movBlackData = 0;
		 }
         //ask server for update
		 if(aiIsSpawnedRequiresInfo){
           m_gStatus->m_netGameLogicObject->SendAttribMsg(  DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH,
															DPID_SERVERPLAYER,
															16,
															0,
															0,
															0,
															0,
															NULL);
		   
		   m_pingPersistStamp = m_gStatus->GetTimeStamp_DW();
		   m_pingInQuerry = TRUE;
		 }
		 SendGuerenteedCycleMsgToServer();
         //end ask server for update
	  }//end timed updates
	  else{//just keep animating for upkeep of fire ques
        
	  }//end just keep animating for upkeep of fire quest
	}//end AI server SUS update to management server*/
}
//------------------------------------------------------------------------------------//
BOOL CClientNetworkingLogic::HandleBankDataBlock(LPVOID lpMessage,
												 int &msgSize,
												 DPID &idFromLoc)
{
	BYTE *pFullPacket = (BYTE*)lpMessage;
    LPBANK_BLOCKHEADER header = (LPBANK_BLOCKHEADER)lpMessage;

	// GET cash info from the packet header
	m_gStatus->m_multiplayerObject->m_bankCashCount = header->m_bankCash;
	m_gStatus->m_multiplayerObject->m_cashCount = header->m_onCharCash;

	// ADVANCE the pointer to ignore the packet header
	BYTE *pData = pFullPacket + sizeof(BANK_BLOCKHEADER);

	// GET the number of banks we'll be processing
	int iCount = 0;
	/*short iNumBanks = 0;
	CopyMemory(&iNumBanks, &pData[iCount], sizeof(iNumBanks));
	iCount += sizeof(iNumBanks);*/

	// FOR EACH bank in the incoming packet...
	//for (int i=0; i<iNumBanks; i++)
	//{
		// GET the number of items in this bank
		/*short iNumItems = 0;
		CopyMemory(&iNumItems, &pData[iCount], sizeof(iNumItems));
		iCount += sizeof(iNumItems);
		if (iNumItems <= 0) continue;*/

		// GET the appropriate bank inventory
		MENU_IDS eInventoryID;
		switch (header->m_menuBind)
		{
			default:
			case 0: eInventoryID = ID_BANK_INVENTORY_01; break;
			case 1: eInventoryID = ID_BANK_INVENTORY_02; break;
			case 2: eInventoryID = ID_BANK_INVENTORY_03; break;
			case 3: eInventoryID = ID_BANK_INVENTORY_04; break;
		}

		// OPEN the bank menu
		if (!m_gStatus->OpenMenu(ID_BANK_INVENTORY_MENU, false)) return FALSE;

		// SET the active inventory for the bank menu
		this->m_gStatus->SetActiveInventory(ID_BANK_INVENTORY_MENU, eInventoryID, true);

		// GET a reference to the inventory control
		CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(eInventoryID, true));
		if (!pInventory) return FALSE;

		// CLEAR the old contents before populating
		pInventory->ClearItems();
		
		// FOR EACH item in this bank inventory...
		for (int i=0; i<header->m_bankItemCount; i++)
		{
			// ADD this item to the current bank inventory
			ITEMDATA_STRUCT item;
			CopyMemory(&item, &pData[iCount], sizeof(item));

			// HANDLE dynamic items
			if (!m_gStatus->m_itemManager->DoesItemExist(item.m_GLID)
			&& (item.m_GLID >= 15000))
			{
				m_gStatus->m_netGameLogicObject->SendAttribMsg(
					DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH,
					DPID_SERVERPLAYER,
					64,
					item.m_GLID,
					0,
					0,
					0,
					0);
			}

			// ADD the item to the inventory
			pInventory->AddItem(item);
			iCount += sizeof(item);
		}
	//}

	/* DEPRECATED /////////////////////////////////////////////////////////////
	// NOTE: Player inventory is no longer being transmitted here!
	// GET the number of items in the player's inventory
	short iNumItems = 0;
	CopyMemory(&iNumItems, &pData[iCount], sizeof(iNumItems));
	iCount += sizeof(iNumItems);
	if (iNumItems > 0)
	{
		// GET a reference to the inventory control
		CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY, true));
		if (!pInventory) return FALSE;

		// CLEAR the old contents before populating
		pInventory->ClearItems();
		
		// FOR EACH item in this player's inventory...
		for (int i=0; i<iNumItems; i++)
		{
			// ADD this item to the current player's inventory
			ITEMDATA_STRUCT item;
			CopyMemory(&item, &pData[iCount], sizeof(item));
			
			if(!m_gStatus->m_itemManager->DoesItemExist(item.m_GLID))
			if(item.m_GLID >= 15000)
					m_gStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH,
																	DPID_SERVERPLAYER,
																	64,
																	item.m_GLID,
																	0,
																	0,
																	0,
																	0);
			
			
			pInventory->AddItem(item);
			iCount += sizeof(item);
		}
	}*/
	
	// RELEASE the incoming packet resources
	SafeDeleteArray(pFullPacket);
	return TRUE;
	/*
    BYTE * dataBlock = (BYTE *)lpMessage;
    LPBANK_BLOCKHEADER header = (LPBANK_BLOCKHEADER)lpMessage;
        
   
        m_gStatus->OpenMenu(header->m_menuBind, false); //header->m_menuBind,0,0,FALSE);//open bank specified menu
        m_gStatus->m_multiplayerObject->m_bankCashCount = header->m_bankCash;//update bank data
		m_gStatus->m_multiplayerObject->m_cashCount     = header->m_onCharCash;

		int headerSize = sizeof(BANK_BLOCKHEADER);
		int itemDataBlockSize = (msgSize-headerSize);
		if(itemDataBlockSize > 0)
		{//valid
		int itemDataSize = sizeof(ITEMDATA_STRUCT);
		int invenCount = (itemDataBlockSize/itemDataSize);
        ITEMDATA_STRUCT * itemDataBlock = new ITEMDATA_STRUCT[invenCount];
		BYTE * conversionBlock = (BYTE*)itemDataBlock;
		int loop=0;
        for(loop=0;loop<itemDataBlockSize;loop++)
            conversionBlock[loop] = dataBlock[loop+headerSize];

		//Clear Existing List
		CMenuListBox* visualReference = (CMenuListBox*)m_gStatus->m_runtimeMenuDatabase->GetVisualByID(BANK_LISTBOX_ID, true);
		if(visualReference)
			visualReference->ClearList();

        //--------------------
		int tracer = 0;
		for(loop=0;loop<header->m_bankItemCount;loop++)
		{//fill in bank data
		  CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(itemDataBlock[loop].m_GLID);
		  if(invObj){
			 CString output = invObj->m_itemName;
             CString dmgMod = "";
             if(itemDataBlock[loop].m_dMod > 0)
			    dmgMod = "+ " + m_gStatus->m_cabLib.ConvToString((int)itemDataBlock[loop].m_dMod);
			 
			 if(invObj->m_stackable)
			    output += dmgMod + " X"+m_gStatus->m_cabLib.ConvToString(itemDataBlock[loop].m_quanity);
              
			 m_gStatus->AddStringToMenuList(BANK_LISTBOX_ID,output,invObj->m_visualRef,itemDataBlock[loop].m_GLID,itemDataBlock[loop].m_quanity,invObj->m_itemName+dmgMod,1,"");
		  }
		  else{//alternate
             m_gStatus->AddStringToMenuList(BANK_LISTBOX_ID,"Unknown",-1,itemDataBlock[loop].m_GLID,0,"Unknown",1,"");
		  }//end alternate
		  tracer++;
		}//end fill in bank data
		//----------------------	
		
/*		for(loop=tracer;loop<invenCount;loop++)
		{//fill in player inventory
		  CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(itemDataBlock[loop].m_GLID);
		  if(invObj){
			 CString output = invObj->m_itemName;
             CString dmgMod = "";
             if(itemDataBlock[loop].m_dMod > 0)
			    dmgMod = "+ " + m_gStatus->m_cabLib.ConvToString((int)itemDataBlock[loop].m_dMod);

             if(invObj->m_stackable)
			    output += dmgMod+" X"+m_gStatus->m_cabLib.ConvToString(itemDataBlock[loop].m_quanity);
             m_gStatus->AddStringToMenuList(51,output,invObj->m_visualRef,itemDataBlock[loop].m_GLID,itemDataBlock[loop].m_quanity,invObj->m_itemName+dmgMod,1,"");
		  }
		  else{//alternate
             m_gStatus->AddStringToMenuList(51,"Unknown",-1,itemDataBlock[loop].m_GLID,0,"Unknown",1,"");
		  }//end alternate
		}//end fill in player inventory
* /
			delete [] itemDataBlock;
			itemDataBlock = 0;
		}//end valid

	delete [] dataBlock;
    dataBlock=0;
	return TRUE;*/
}
//------------------------------------------------------------------------------------//
BOOL CClientNetworkingLogic::HandleLootBlockData(LPVOID pMessage,
												 int &iMessageSize,
												 DPID &iFromID)
{
	BYTE* pData = (BYTE*)pMessage;
	BYTE* pItemData = pData + sizeof(LOOTBLOCK_DATA);

	// CALCULATE sizes for packet processing
	LPLOOTBLOCK_DATA pHeader = (LPLOOTBLOCK_DATA)pMessage;
	int iRawItemSize = iMessageSize - sizeof(LOOTBLOCK_DATA);
	int iItemCount = iRawItemSize / sizeof(ITEMDATA_STRUCT);

	// GET references to menu visuals
	bool bResult = m_gStatus->OpenMenu(pHeader->m_menuLink, false);
	CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_LOOT_INVENTORY, true));
	if(!bResult 
	|| !pInventory
	|| (iItemCount <= 0))
	{
		SafeDeleteArray(pData);
		return FALSE;
	}

	// IF item count > 0, ENABLE the loot all button
	CMenuButton *pLootAllButton = dynamic_cast<CMenuButton*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_EXAMINE_LOOT_ALL_BUTTON, true));
	if (pLootAllButton) pLootAllButton->SetEnabled();

	// CLEAR the inventory list of old items
	pInventory->ClearItems();

	int iCount = 0;
	ITEMDATA_STRUCT_EX item;
	ITEMDATA_STRUCT * castItemStencil = NULL;
	for (int i=0; i<iItemCount; i++)
	{
		// PARSE out the next item struct
		//ITEMDATA_STRUCT_EX item;
		ITEMDATA_STRUCT * castItemStencil = (ITEMDATA_STRUCT *)&pItemData[iCount];
		item.m_armed		= castItemStencil->m_armed;
		item.m_dMod			= castItemStencil->m_dMod;
		item.m_GLID			= castItemStencil->m_GLID;
		item.m_quantity		= castItemStencil->m_quantity;

		CItemObj *pItem = m_gStatus->m_itemManager->GetItemByID(item.m_GLID);
		if(!pItem)
		if(item.m_GLID >= 15000)
		{
			item.m_received = 0;

			m_gStatus->m_netGameLogicObject->SendAttribMsg(	DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH,
															DPID_SERVERPLAYER,
															64,
															item.m_GLID,
															0,
															0,
															0,
															0);//request dynamic item
		}

		iCount += sizeof(ITEMDATA_STRUCT);

		// ADD the loot item to the inventory
		pInventory->AddItem(item);
	}

	// RELEASE memory resources
	SafeDeleteArray(pData);	
	return TRUE;
}
//----------------------------------------------------------------//
/*bool PlayerPosGreater(SQUAD_MESSAGE p1, SQUAD_MESSAGE p2)
{
	return p1.Position > p2.Position;
}*/

bool PlayerPosGreater(SQUAD_MESSAGE *p1, SQUAD_MESSAGE *p2)
{
	return p1->Position > p2->Position;
}

void CClientNetworkingLogic::HandleMiscBlocksClientSide(BYTE *miscUpdateBlock,
														int miscBlockSize,
														CMovementObjList *entObjRuntime)
{
	    switch(miscUpdateBlock[0])
	    {
			// HANDLE trade offers
			case 2:
			{
				// ERROR checking: Make sure we have enough raw bytes to process!
				if (miscBlockSize <= sizeof(TRADE_HEADER))
					break;
				
				// PARSE packet pHeader
				TRADE_HEADER *pHeader = (TRADE_HEADER*)miscUpdateBlock;
				BYTE *pData = miscUpdateBlock + sizeof(TRADE_HEADER);

				// GET references to menu visuals
				CMenuInventory *pTheirInventory = dynamic_cast<CMenuInventory*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_THEIR_INVENTORY, true));
				if (!pTheirInventory) break;

				// CALCULATE sizes for packet processing
				int iCount = sizeof(TRADE_HEADER);
				int iItemCount = (miscBlockSize - sizeof(TRADE_HEADER)) / sizeof(ITEMDATA_STRUCT);

				// ERROR checking: Do we have items to process?
				if (iItemCount <= 0) break;

				// PARSE items out of the raw packet stream
				for (int i=0; i<iItemCount; i++)
				{
					ITEMDATA_STRUCT item;
					CopyMemory(&item, &pData[iCount], sizeof(ITEMDATA_STRUCT));
					iCount += sizeof(ITEMDATA_STRUCT);

					// ADD the item to the offering player's inventory
					pTheirInventory->AddItem(item);
				}
				break;
				/*if(miscBlockSize > sizeof(TRADE_HEADER))
				{
					int headerSz = sizeof(TRADE_HEADER);
					int istructSz = sizeof(ITEMDATA_STRUCT);
					int itemCount = (miscBlockSize-headerSz)/istructSz;

					BYTE * headerBytes = NULL;

					TRADE_HEADER * header = (TRADE_HEADER * )miscUpdateBlock;

					if(header->m_cashAmount > 0)
					{
						CString output = operator+("CREDITS :",m_gStatus->m_cabLib.ConvToString(header->m_cashAmount));
						m_gStatus->AddStringToMenuList(59,output,-1,-50,0,output,0,"");
					}

					if(itemCount > 0)
					{
						int valueByteCount = sizeof(ITEMDATA_STRUCT)*itemCount;
						ITEMDATA_STRUCT * itemArray = NULL;
						BYTE * castBytes = NULL;
						itemArray = new ITEMDATA_STRUCT[itemCount];
						castBytes = (BYTE *)itemArray;
						for(int j=0;j<valueByteCount;j++)
						castBytes[j] = miscUpdateBlock[j+headerSz];


						for(int menuFillLoop=0;menuFillLoop<itemCount;menuFillLoop++)
						{//menu fill loop
							CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(itemArray[menuFillLoop].m_GLID);
							if(invObj)
							{
								if(!invObj->m_stackable)
								{
									CString outPut = invObj->m_itemName;
									if(itemArray[menuFillLoop].m_dMod > 0)
										outPut += " +" + m_gStatus->m_cabLib.ConvToString((int)itemArray[menuFillLoop].m_dMod);

									m_gStatus->AddStringToMenuList(59,outPut,invObj->m_visualRef,itemArray[menuFillLoop].m_GLID,1,outPut,1,"");
								}
								else if(!m_gStatus->AddSelectedFromMenuList(59,itemArray[menuFillLoop].m_GLID,itemArray[menuFillLoop].m_quanity))
								{
									CString outPutX = invObj->m_itemName;
									CString dmgPlus = "";
									if(itemArray[menuFillLoop].m_dMod > 0)
										dmgPlus = " +" + m_gStatus->m_cabLib.ConvToString((int)itemArray[menuFillLoop].m_dMod);

									if(invObj->m_stackable)
									{
										CString quanityTxt = m_gStatus->m_cabLib.ConvToString(itemArray[menuFillLoop].m_quanity);
										CString outPut = outPutX + " X" + quanityTxt;
										m_gStatus->AddStringToMenuList(59,outPut,invObj->m_visualRef,itemArray[menuFillLoop].m_GLID,itemArray[menuFillLoop].m_quanity,invObj->m_itemName+dmgPlus,1,"");
									}
									else
									{
										m_gStatus->AddStringToMenuList(59,outPutX,invObj->m_visualRef,itemArray[menuFillLoop].m_GLID,1,invObj->m_itemName+dmgPlus,1,"");
									}
								}
							}//alternate
						}//end menu fill loop
						delete [] itemArray;
						itemArray = 0;
					}
				}//end fill in client offered items
				break;*/
			}
			case 3: // Recent quest journal entry update
			if(miscBlockSize > 1)
			{
				unsigned int tracerBuild = 1;

				// RELEASE old references
				if(m_gStatus->m_multiplayerObject->m_recentQuestPtr)
					m_gStatus->m_multiplayerObject->m_recentQuestPtr = NULL;

				// ALLOCATE new memory
				CQuestJournalObj *pJournalEntry = new CQuestJournalObj();
				m_gStatus->m_multiplayerObject->m_recentQuestPtr = pJournalEntry;

				// PROCESS the incoming quest journal entry
				if (this->ProcessJournalEntry(miscUpdateBlock, 
					tracerBuild, 
					m_gStatus->m_multiplayerObject->m_recentQuestPtr))
					m_gStatus->m_multiplayerObject->m_bRecentJournalUpdate = true;
				else
				{
					// FAILED to process incoming journal entry, so cleanup and BREAK
					m_gStatus->m_multiplayerObject->m_bRecentJournalUpdate = false;
					m_gStatus->m_multiplayerObject->m_recentQuestPtr = NULL;
					SafeDelete(pJournalEntry);
					break;
				}

				// ADD journal entry to the multiplayer object
				m_gStatus->m_multiplayerObject->m_journalEntries->AddQuestJournalEntry(pJournalEntry, CQuestJournalList::QJ_HEAD);

				// REFRESH the quest journal list
				m_gStatus->RefreshQuestJournal(ID_QUEST_JOURNAL_MAIN_LIST, true);
			}
			break;
		   case 4://recent quest dialog
		    if(miscBlockSize > 1)
			{
              for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
	          {//1
                  CMenu3DObj * menu = (CMenu3DObj *)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
	              if(menu->m_closeOnEsc)
			         menu->CloseMenu(&m_gStatus->m_globalCursorPos);
	          }//end 1
	          m_gStatus->OpenMenu(92, false); //TODO: Switch this to use a value from the "important menu" array

	          //parse out test objects
			  //int maxLineLength = 35;
			  int maxLineLength = 30;
			  int tracerBuild = 1;
			  float lineLengthMeasured = 0.0f;
			  float maxLineLengthMeasured = 0.3f;
			  if(m_gStatus->m_dxRenderer->m_currentWidth > 1000)
			  {
                 maxLineLengthMeasured = maxLineLengthMeasured * .8f;
			  }
			  float characterSpacing = m_gStatus->m_systemReadout->m_font->m_height * 2.0f;
			  CString tempCS = "";
              while(tracerBuild < miscBlockSize)
	          {
		          //int curStrLen = miscUpdateBlock[tracerBuild];
		          //char * listText = new char[curStrLen+1];
		          //tracerBuild++;

				  CString curBuild = "";
                  lineLengthMeasured = 0.0f;
                  char additionTemp;
                  for(int loop=0;loop<20000;loop++)
		          {
					  if(tracerBuild >= miscBlockSize)
					     break;
		              //listText[loop] = miscUpdateBlock[tracerBuild];
					  additionTemp = miscUpdateBlock[tracerBuild];
					  curBuild = curBuild + additionTemp;
					  tempCS = additionTemp;
                      CSymbolMapperObj * tempref = m_gStatus->m_symbolMapDB->GetByID(tempCS);
					  lineLengthMeasured += characterSpacing * tempref->m_percentageX;

			          tracerBuild++;
					  if(lineLengthMeasured > maxLineLengthMeasured)
					  {
					    //tracerBuild--;
                        for(int j=0;j<tracerBuild;j++)
						{
							
							additionTemp = miscUpdateBlock[tracerBuild];
							if(additionTemp == ' ')
							   break;

                            tracerBuild--;
							curBuild.Delete(curBuild.GetLength()-1,1);
						}
					    break;
					  }

					  
		          }   
		          //m_gStatus->AddStringToMenuList(91,curBuild,-1,0,0,curBuild,0,"");
				  CMenuText* visual = (CMenuText*)m_gStatus->m_menuDatabase->GetVisualByID(91, true);
				  if(visual)
				  {
					  visual->m_text = curBuild;

					  //Delete Text Mesh
					  if(visual->m_textMesh)
					  {
						  visual->m_textMesh->SafeDelete();
						  delete visual->m_textMesh;
						  visual->m_textMesh = NULL;
					  }
				  }
	          }
			}
		   break;
		   case 5://report murder option
		    if(miscBlockSize > 1)
		    {
			  int tracerBuild = 3;
			  int menuID = miscUpdateBlock[1];
			  int nameLength = miscUpdateBlock[2];
			  int handleTemp = 0;

              CString m_murdererName = "";
		      char * listText = new char[nameLength+1];
			  int loop=0;
              for(loop=0;loop<nameLength;loop++)
		      {
		       listText[loop] = miscUpdateBlock[tracerBuild++];
		      }   
		      listText[nameLength] = 0;//null terminator
			  m_murdererName = listText;
			  delete [] listText;
			  listText = 0;

			  BYTE xFloatCast[4];
			  for(loop = 0;loop<4;loop++) 
				  xFloatCast[loop] = miscUpdateBlock[tracerBuild++];
			  CopyMemory(&handleTemp,xFloatCast,sizeof(int));

			  m_gStatus->OpenMenu(menuID, false); //0,handleTemp,0,FALSE);
			  m_gStatus->SetMenuTextItem(10,"You have been murdered by "+m_murdererName);
			}
		   break;
		   case 6://call client script
		    if(miscBlockSize > 1)
			{
              short scriptLength = 0;
			  short textLength = 0;
			  
			  CopyMemory(&scriptLength,&miscUpdateBlock[1],2);
			  CopyMemory(&textLength,&miscUpdateBlock[3],2);
			   
			  if(scriptLength > 0)
			  {
                 CString error;
				 char * scriptArray = new char[scriptLength+3];
				 char * textArray = new char[textLength+3];
				 ZeroMemory(scriptArray,scriptLength+3);
				 ZeroMemory(textArray,textLength+3);

				 if(scriptLength > 0)
				    CopyMemory(scriptArray,&miscUpdateBlock[5],scriptLength);

				 if(textLength > 0)
				    CopyMemory(textArray,&miscUpdateBlock[5+scriptLength],textLength);

			     if(m_gStatus->m_clientScriptor.ScriptProcessor(scriptArray,m_gStatus,error,textArray)==false)
				    m_gStatus->m_multiplayerObject->m_eventLog->AddEvent(error);


				 //m_gStatus->m_systemReadout->AddLine("script processed",1.0f,.1f,.1f);

				 delete [] scriptArray;
				 scriptArray = 0;
				 delete [] textArray;
				 textArray = 0;
			  }
			}
		   break;
		   case 7://Physics Update
		    if(miscBlockSize > 1)
		    {
              CMovementObj * entREFLoc = (CMovementObj *)m_gStatus->m_entObjRuntime->GetObjByIndex(0);
			  int trace = 1;
			  if(entREFLoc)
			  if(entREFLoc->List_movementDB)
			  if(entREFLoc->List_controlType == 0)
			  {//primary
			    CopyMemory(&entREFLoc->List_movementDB->List_maxSpeed,&miscUpdateBlock[trace],sizeof(float)); //1
				trace += sizeof(float);

				CopyMemory(&entREFLoc->List_movementDB->List_minSpeed,&miscUpdateBlock[trace],sizeof(float)); //2
				trace += sizeof(float);
				if(entREFLoc->List_movementDB->List_maxSpeed > 0.0f &&
				   entREFLoc->List_movementDB->List_minSpeed > -entREFLoc->List_movementDB->List_maxSpeed * 0.85f)
				{
					entREFLoc->List_movementDB->List_minSpeed = -entREFLoc->List_movementDB->List_maxSpeed * 0.85f;
				}

				CopyMemory(&entREFLoc->List_movementDB->List_accellerate,&miscUpdateBlock[trace],sizeof(float)); //3
				trace += sizeof(float);

				CopyMemory(&entREFLoc->List_movementDB->List_decellerate,&miscUpdateBlock[trace],sizeof(float)); //4 
				trace += sizeof(float);

				CopyMemory(&entREFLoc->List_movementDB->List_jumpPower,&miscUpdateBlock[trace],sizeof(float)); //5
				trace += sizeof(float);

				CopyMemory(&entREFLoc->List_movementDB->m_strafeReduction,&miscUpdateBlock[trace],sizeof(float));//Strafe Degrade //6
				trace += sizeof(float);

				CopyMemory(&entREFLoc->List_movementDB->List_groundDrag,&miscUpdateBlock[trace],sizeof(float));//Ground Grip //7
				trace += sizeof(float);

				CopyMemory(&entREFLoc->List_movementDB->m_crossSectionalArea,&miscUpdateBlock[trace],sizeof(float));//Aero Dynamic //8
				trace += sizeof(float);

				CopyMemory(&entREFLoc->List_movementDB->List_mass,&miscUpdateBlock[trace],sizeof(float));//Mass //10
				trace += sizeof(float);

				CopyMemory(&entREFLoc->List_movementDB->m_inertia,&miscUpdateBlock[trace],sizeof(float));//Inertia //11
				trace += sizeof(float);

				CopyMemory(&entREFLoc->List_movementDB->m_corneringStiffnessR,&miscUpdateBlock[trace],sizeof(float));//crnStiffnessR //12
				trace += sizeof(float);

				CopyMemory(&entREFLoc->List_movementDB->m_corneringStiffnessF,&miscUpdateBlock[trace],sizeof(float));//crnStiffnessF //13
				trace += sizeof(float);

				CopyMemory(&entREFLoc->List_movementDB->m_maxGrip,&miscUpdateBlock[trace],sizeof(float));//max Grip //14
				trace += sizeof(float);

				float miscFloatLoc = 0.0f;
				CopyMemory(&miscFloatLoc,&miscUpdateBlock[trace],sizeof(float));//jump recover time //15
				trace += sizeof(float);
				entREFLoc->List_movementDB->m_jumpRecoverTime = (int)miscFloatLoc;

				CopyMemory(&entREFLoc->List_movementDB->m_airControl,&miscUpdateBlock[trace],sizeof(float));//air Control //16
				trace += sizeof(float);

				CopyMemory(&entREFLoc->List_movementDB->List_maxYaw,&miscUpdateBlock[trace],sizeof(float));//max yaw //17
				trace += sizeof(float);

				CopyMemory(&entREFLoc->List_movementDB->List_maxPitch,&miscUpdateBlock[trace],sizeof(float));//max pictch //18
				trace += sizeof(float);

				CopyMemory(&entREFLoc->List_movementDB->m_inLiquidMotionModifier,&miscUpdateBlock[trace],sizeof(float));//liquid modifier//19
				trace += sizeof(float);

				
			  }//end primary
		    }
		   break;
		   case 8://animation change for static AI
		    if(miscBlockSize > 3)
		    {
		      short networkDefinedID = 0;
              short animationIndex = 0;//negative defines backwards or fowards, good for opening and closing a door - both directions are handled and synced with one animation :O
			  int parser = 1;
              CopyMemory(&networkDefinedID,&miscUpdateBlock[parser],sizeof(short));
              parser += sizeof(short);
			  CopyMemory(&animationIndex,&miscUpdateBlock[parser],sizeof(short));


			  CMovementObj * targetEntity = m_gStatus->m_entObjRuntime->GetObjByNetworkID(networkDefinedID);
			  //aquire entity
			  if(targetEntity)
			  if(targetEntity->List_skeletalObject->SetCurrentAnimation(abs(animationIndex),100))
			  {		
			     if(animationIndex < 0)//reverse animation
			        targetEntity->List_skeletalObject->SetAnimationDirection(abs(animationIndex),TRUE);
		  	     else
			        targetEntity->List_skeletalObject->SetAnimationDirection(abs(animationIndex),FALSE);
			  }		   
		    }
		   break;
			case 9:
			{ // SQUAD data update
				if(miscBlockSize > 1)
				{
					// Parse through our data
					int parser = 1;
					SQUAD_MESSAGE *msg;
					std::vector<SQUAD_MESSAGE*> members;

					// Keep going until we have no more data that needs updating
					while(parser < miscBlockSize)
					{
						msg = new SQUAD_MESSAGE();

						// PARSE basic struct members
						CopyMemory(&msg->NetID, &miscUpdateBlock[parser], sizeof(msg->NetID));
						parser += sizeof(msg->NetID);
						CopyMemory(&msg->Health, &miscUpdateBlock[parser], sizeof(msg->Health));
						parser += sizeof(msg->Health);
						CopyMemory(&msg->Position, &miscUpdateBlock[parser], sizeof(msg->Position));
						parser += sizeof(msg->Position);

						// PARSE player name
						CopyMemory(&msg->PlayerNameLength, &miscUpdateBlock[parser], sizeof(msg->PlayerNameLength));
						parser += sizeof(msg->PlayerNameLength);
						msg->PlayerName = new char[msg->PlayerNameLength];
						CopyMemory(msg->PlayerName, &miscUpdateBlock[parser], msg->PlayerNameLength);
						parser += msg->PlayerNameLength;

						// PARSE squad name
						CopyMemory(&msg->SquadNameLength, &miscUpdateBlock[parser], sizeof(msg->SquadNameLength));
						parser += sizeof(msg->SquadNameLength);
						msg->SquadName = new char[msg->SquadNameLength];
						CopyMemory(msg->SquadName, &miscUpdateBlock[parser], msg->SquadNameLength);
						parser += msg->SquadNameLength;

						// NULL terminate the char* struct members
						msg->PlayerName[msg->PlayerNameLength-1] = '\0';
						msg->SquadName[msg->SquadNameLength-1] = '\0';

						// Add to the collection
						members.push_back(msg);
					}

					// Sort the group members by group position
					sort(members.begin(), members.end(), PlayerPosGreater);

					// SAVE the squad name locally
					std::vector<SQUAD_MESSAGE*>::iterator iter = members.begin();
					m_gStatus->m_multiplayerObject->m_squadName = (*iter)->SquadName;

					// SET the title text of the squad status menu
					// Open group status window
					CMenuWindow *pSquadMenu = dynamic_cast<CMenuWindow*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_SQUAD_STATUS_MENU, true));
					CMenuListBox *pSquadList = dynamic_cast<CMenuListBox*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_SQUAD_STATUS_LIST, true));
					if(pSquadMenu)
						if(pSquadMenu->m_titleText)
							pSquadMenu->m_titleText->SetText(m_gStatus->m_multiplayerObject->m_squadName);

					if(pSquadList)
					{
						// SAVE the name of the selected player (if it exists)
						CMenuText *pSelectedPlayer = NULL;
						CString strSelectedPlayerName;
						if (pSquadList->m_currentSelection)
						{
							pSelectedPlayer = (CMenuText*)pSquadList->m_currentSelection->m_textList.GetHead();
							strSelectedPlayerName = pSelectedPlayer->m_text;
						}

						pSquadList->ClearList();
						for(iter = members.begin(); iter != members.end(); iter++)
						{
							// Add this player to the group menu listbox
							SQUAD_MESSAGE* msg = (*iter);
							CMenuListItem *pItem = new CMenuListItem();
							CMenuText *pHealth = new CMenuText();
							CMenuText *pName = new CMenuText();

							CString strHealth = m_gStatus->m_cabLib.ConvToString((int)msg->Health);
							CString strName = msg->PlayerName;
							if(msg->Position == 1) strName.Append(" (*)");

							pHealth->SetText(strHealth.GetBuffer());
							pName->SetText(strName.GetBuffer());

							strHealth.ReleaseBuffer();
							strName.ReleaseBuffer();

							pItem->m_textList.AddHead(pHealth);
							pItem->m_textList.AddHead(pName);

							pSquadList->AddItem(pItem, 0);
						}

						// RESTORE the selected item (if it exists)
						if (pSelectedPlayer && pSquadList->m_pFirstItem)
						{
							CMenuListItem *pItem = NULL;
							CMenuText *pName = NULL;
							for (pItem = pSquadList->m_pFirstItem; pItem != NULL; pItem = pItem->m_pNext)
							{
								pName = (CMenuText*)pItem->m_textList.GetHead();
								if (pName->m_text.CompareNoCase(strSelectedPlayerName) == 0)
								{
									pSquadList->m_currentSelection = pItem;
									break;
								}
							}
						}
					}

					// Release allocated resources for group member names
					for(iter = members.begin(); iter != members.end(); iter++)
					{
						msg = (*iter);
						SafeDelete(msg);
					}

					// Erase all elements of the collection
					members.clear();
				}
				break;
			}
			case 10:
			{ // GROUP leave/removed
				CMenuListBox *pGroupList = dynamic_cast<CMenuListBox*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_SQUAD_STATUS_LIST, true));
				if(pGroupList) 
					pGroupList->ClearList();
				m_gStatus->m_multiplayerObject->m_squadName.Empty();
				break;
			}
			case 11:
			{ // Parse arena message
				if(miscBlockSize > 1)
				{
					int parser = 1,
						iImageID = -1,
						iSoundID = -1;
					ARENA_MESSAGE msg;
					CMenuImage* pImage = NULL;

					// Copy network packet into the ARENA_MESSAGE struct
					CopyMemory(&msg.ArenaMessage, &miscUpdateBlock[parser], sizeof(msg.ArenaMessage));
					parser += sizeof(msg.ArenaMessage);
					CopyMemory(&msg.ArenaType, &miscUpdateBlock[parser], sizeof(msg.ArenaType));
					parser += sizeof(msg.ArenaType);
					CopyMemory(&msg.Score, &miscUpdateBlock[parser], sizeof(msg.Score));
					parser += sizeof(msg.Score);
					CopyMemory(&msg.Kills, &miscUpdateBlock[parser], sizeof(msg.Kills));
					parser += sizeof(msg.Kills);
					CopyMemory(&msg.Deaths, &miscUpdateBlock[parser], sizeof(msg.Deaths));
					parser += sizeof(msg.Deaths);
					CopyMemory(&msg.Damage, &miscUpdateBlock[parser], sizeof(msg.Damage));
					parser += sizeof(msg.Damage);
					CopyMemory(&msg.Reward, &miscUpdateBlock[parser], sizeof(msg.Reward));
					parser += sizeof(msg.Reward);

					// Determine the correct menu image ID
					switch(msg.ArenaMessage)
					{
						case ARENA_BEGIN:
						{
							iSoundID = SND_ARENA_BEGIN;
							iImageID = ARENA_BANNER_BEGIN_ID;
							if(m_gStatus->m_systemReadout)
							m_gStatus->m_systemReadout->AddLine("ARENA BEGIN!", MENU_TEXTCOLOR_SERVER);

							// Open the arena countdown menu
							m_gStatus->OpenMenu(ARENA_TIMER_MENU, false);

							// UPDATE arena status UI element
							this->UpdateArenaStatusMenu(ARENASTATUS_ENGAGED);

							// Determine arena start synchro time, received as UTC
							// NOTE: The Score and Kills fields hold arena synchro start time
							// info used to init a FILETIME struct
							FILETIME ftServer, ftLocal;
							SYSTEMTIME stServer, stLocal, stArena;
							WORD	iOffsetMinutes = 0,
									iOffsetSeconds = 0;

							// Get server time as FILETIME
							ftServer.dwHighDateTime = msg.Score;
							ftServer.dwLowDateTime = msg.Kills;

							// Convert to SYSTEMTIME
							::FileTimeToSystemTime(&ftServer, &stServer);
							::GetSystemTime(&stLocal);
							m_gStatus->m_iArenaTimer = msg.Deaths; 

							// Init the GSV arena timestamp
							if(m_gStatus->m_highResTimerEnabled)
								m_gStatus->m_iArenaTimeStamp = timeGetTime();
							else
								m_gStatus->m_iArenaTimeStamp = GetTickCount();
							break;
						}
						case ARENA_VICTORY:
						{
							iImageID = ARENA_BANNER_VICTORY_ID;
							if(m_gStatus->m_systemReadout)
							m_gStatus->m_systemReadout->AddLine("VICTORY!", MENU_TEXTCOLOR_SERVER);
							CMovementObj *targetEntity = (CMovementObj *)m_gStatus->m_entObjRuntime->GetObjByIndex(0);
							if(targetEntity)
							{
								targetEntity->m_hasJoinedArena = false;
								targetEntity->m_arenaBotID = -1;
							}
							break;
						}
						case ARENA_DEFEAT:
						{
							iImageID = ARENA_BANNER_DEFEAT_ID;
							if(m_gStatus->m_systemReadout)
							m_gStatus->m_systemReadout->AddLine("DEFEAT!", MENU_TEXTCOLOR_SERVER);
							CMovementObj *targetEntity = (CMovementObj *)m_gStatus->m_entObjRuntime->GetObjByIndex(0);
							if(targetEntity)
							{
								targetEntity->m_hasJoinedArena = false;
								targetEntity->m_arenaBotID = -1;
							}
							break;
						}
						case ARENA_KILLING_BLOW:
						{
							iImageID = ARENA_BADGE_KILLING_BLOW_ID;
							iSoundID = SND_ARENA_KILLING_BLOW;
							if(m_gStatus->m_systemReadout)
							m_gStatus->m_systemReadout->AddLine("KILLING BLOW!", MENU_TEXTCOLOR_SERVER);
							break;
						}
						case ARENA_SET_MOST_WANTED:
						{
							if(m_gStatus->m_systemReadout)
								m_gStatus->m_systemReadout->AddLine("MOST WANTED!", MENU_TEXTCOLOR_SERVER);
							// Fallthrough...
						}
						case ARENA_REMOVE_MOST_WANTED:
						{
							iImageID = ARENA_BADGE_MOST_WANTED_ID;
							iSoundID = SND_ARENA_MOST_WANTED;
							break;
						}
						case ARENA_SOLE_SURVIVOR:
						{
							iImageID = ARENA_BADGE_SOLE_SURVIVOR_ID;
							iSoundID = SND_ARENA_SOLE_SURVIVOR;
							if(m_gStatus->m_systemReadout)
								m_gStatus->m_systemReadout->AddLine("SOLE SURVIVOR!", MENU_TEXTCOLOR_SERVER);
							break;
						}
					}

					// Play the appropriate event sound
					if(iSoundID >= 0)
					{
						m_gStatus->m_soundTrackList->Play(iSoundID, m_gStatus->GL_pathGameSounds, VARIANT_FALSE);
						SetCurrentDirectory(m_gStatus->GL_workingDirectory);
						m_gStatus->m_soundTrackList->m_lastSongPlayed = "";
					}

					// If this is the end of the match for this player,
					// display their final arena stats
					if((msg.ArenaMessage == ARENA_VICTORY) 
					|| (msg.ArenaMessage == ARENA_DEFEAT))
					{
						// Add arena stats menu to the runtime database
						bool bSuccess = m_gStatus->OpenMenu(ARENA_STATS_MENU, false);

						// Get a reference to the arena menu objects
						CMenuWindow *pArenaStats = dynamic_cast<CMenuWindow*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_STATS_MENU, true));								
						CMenuText *pKillsText = dynamic_cast<CMenuText*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_STATS_KILLS_TEXT, true));
						CMenuText *pDamageText = dynamic_cast<CMenuText*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_STATS_DAMAGE_TEXT, true));
						CMenuText *pDeathsText = dynamic_cast<CMenuText*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_STATS_DEATHS_TEXT, true));
						CMenuText *pRewardText = dynamic_cast<CMenuText*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_BET_REWARD_TEXT, true));

						// Update arena stats for display
						if (pArenaStats)
						{
							CString strStat; 
							if (pKillsText)
							{
								strStat = m_gStatus->m_cabLib.ConvToString(msg.Kills);
								pKillsText->SetText(strStat.GetBuffer());
								strStat.ReleaseBuffer();
								strStat.Empty();
							}

							if (pDamageText)
							{
								strStat = m_gStatus->m_cabLib.ConvToString(msg.Damage);
								pDamageText->SetText(strStat.GetBuffer());
								strStat.ReleaseBuffer();
								strStat.Empty();
							}

							if (pDeathsText)
							{
								strStat = m_gStatus->m_cabLib.ConvToString(msg.Deaths);
								pDeathsText->SetText(strStat.GetBuffer());
								strStat.ReleaseBuffer();
								strStat.Empty();
							}

							if (pRewardText)
							{	
								if (msg.ArenaMessage == ARENA_VICTORY)
								{
									if (msg.Reward > 0)
									{
										strStat.Append("You won: ");
										strStat.Append(m_gStatus->m_cabLib.ConvToString(msg.Reward));
										strStat.Append(" Credits!");
									}
									else
									{
										strStat.Append("You won the arena battle!");
									}
								}
								else if (msg.ArenaMessage == ARENA_DEFEAT)
								{
									if (msg.Reward < 0)
									{
										strStat.Append("You lost: ");
										strStat.Append(m_gStatus->m_cabLib.ConvToString(abs(msg.Reward)));
										strStat.Append(" Credits!");
									}
									else
									{
										strStat.Append("You lost the arena battle!");
									}
								}
								pRewardText->SetText(strStat.GetBuffer());
								strStat.ReleaseBuffer();
								strStat.Empty();
							}
						}

						// RESET arena status UI element
						this->UpdateArenaStatusMenu(ARENASTATUS_NONE);
					}

					// Get a reference to the arena banner/badge menu image
					pImage = dynamic_cast<CMenuImage*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(iImageID, true));
					if(pImage) 
					{						
						// Make the image visible and update the texture for render
						pImage->m_visible = true;
						pImage->m_alpha = 1.0f;
						pImage->Init(m_gStatus->m_3dDevice);
						pImage->m_imageMesh->List_blendType = 1;
						pImage->UpdateTexture(pImage->m_textureSelect);
						pImage->UpdateTextureCoords();

						// If we're removing the MOST WANTED badge...
						if(msg.ArenaMessage == ARENA_REMOVE_MOST_WANTED)
						{
							// Make the badge invisible and we're done!
							pImage->m_alpha = 0.0f;
							pImage->Init(m_gStatus->m_3dDevice);
							pImage->m_imageMesh->List_blendType = 1;
							pImage->UpdateTexture(pImage->m_textureSelect);
							pImage->UpdateTextureCoords();
							return;
						}
						// If this is a KILLING BLOW badge, attach the FADEIMAGE update script...
						else if((msg.ArenaMessage == ARENA_KILLING_BLOW)
						|| (msg.ArenaMessage == ARENA_BEGIN))
						{
							// Create the update script string...
							const int SIZE = 10;
							char strParam[SIZE];
							CString strUpdateScript = "FadeImage(";
							itoa(iImageID, strParam, 10);
							strUpdateScript += strParam;
							strUpdateScript += ",";
							CString strFadeRate = m_gStatus->m_cabLib.FloatToStingCnv(FADE_RATE);
							strUpdateScript += strFadeRate; 
							strUpdateScript += ")";

							// Attach the update script
							if(!pImage->m_updateScript)	pImage->m_updateScript = new CMenuScript();
							pImage->m_updateScript->ClearScript();
							pImage->m_updateScript->AddLine(strUpdateScript.GetBuffer());
							strUpdateScript.ReleaseBuffer();
							pImage->m_updateScript->m_pFirstLine->m_function = MENU_SCRIPTFUNC_FADEIMAGE;
						}
					}
				}
				break;
			} // End parse arena message
			case 12:
			{
				if(miscBlockSize > 1)
				{
					int parser = 1;
					int amount = 0;
					CopyMemory(&amount,&miscUpdateBlock[parser],sizeof(short));
					parser += sizeof(short);

					CMovementObj *targetEntity = (CMovementObj *)m_gStatus->m_entObjRuntime->GetObjByIndex(0);
					//aquire entity
					if(targetEntity)
					{
						for(int i = 0; i < amount; i++)
						{
							CopyMemory(&targetEntity->m_badges[i], &miscUpdateBlock[parser], sizeof(short));
							parser += sizeof(short);
						}
					}
				}
				break;
			}
			case 13:
			{// INVITED to join a squad
				if(miscBlockSize > 1)
				{
					int parser = 1;
					SQUAD_MESSAGE *msg = new SQUAD_MESSAGE();

					//Copy Memory and increase parser
					//	NOTE: We should only be processing one SQUAD_MESSAGE packet!
					CopyMemory(&msg->NetID, &miscUpdateBlock[parser], sizeof(msg->NetID));
					parser += sizeof(msg->NetID);
					CopyMemory(&msg->Health, &miscUpdateBlock[parser], sizeof(msg->Health));
					parser += sizeof(msg->Health);
					CopyMemory(&msg->Position, &miscUpdateBlock[parser], sizeof(msg->Position));
					parser += sizeof(msg->Position);

					// PARSE player name
					CopyMemory(&msg->PlayerNameLength, &miscUpdateBlock[parser], sizeof(msg->PlayerNameLength));
					parser += sizeof(msg->PlayerNameLength);
					msg->PlayerName = new char[msg->PlayerNameLength+1];
					CopyMemory(msg->PlayerName, &miscUpdateBlock[parser], msg->PlayerNameLength);
					parser += msg->PlayerNameLength;
					msg->PlayerName[msg->PlayerNameLength] = '\0';

					// PARSE squad name
					CopyMemory(&msg->SquadNameLength, &miscUpdateBlock[parser], sizeof(msg->SquadNameLength));
					parser += sizeof(msg->SquadNameLength);
					msg->SquadName = new char[msg->SquadNameLength+1];
					CopyMemory(msg->SquadName, &miscUpdateBlock[parser], msg->SquadNameLength);
					parser += msg->SquadNameLength;
					msg->SquadName[msg->SquadNameLength] = '\0';

					// SAVE the squad name locally
					m_gStatus->m_multiplayerObject->m_squadName = msg->SquadName;

					// SET the title text of the squad status menu
					CMenuWindow *pSquadMenu = dynamic_cast<CMenuWindow*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_SQUAD_STATUS_MENU, true));
					if(pSquadMenu)
						if(pSquadMenu->m_titleText)
							pSquadMenu->m_titleText->SetText(m_gStatus->m_multiplayerObject->m_squadName + " Squad");
					
					//Check validity
					if(msg->NetID > -1)
					{
						// OPEN Group Invite menu for accept/decline
						m_gStatus->m_multiplayerObject->m_potentialGroupJoiner = msg->NetID;
						m_gStatus->OpenMenu(ID_SQUAD_INVITE_NOTIFICATION_MENU, false);
						CMenuText *pPlayerNameText = (CMenuText*)m_gStatus->m_runtimeMenuDatabase->GetVisualByID(PLAYER_NAME_TEXT_ID, true);
						if (pPlayerNameText)
							pPlayerNameText->SetText(msg->PlayerName);
					}

					// RELEASE heap memory
					SafeDelete(msg);
				}
				break;
			}
			case 14:
			{
				m_gStatus->m_itemManager->AddItemByDataPacket(miscUpdateBlock);
				DYNITEMCREATE * header = (DYNITEMCREATE *)miscUpdateBlock;
				if(header->m_optNetID > 0)
				{//this item has a return command to arm the item to the net id
					CMovementObj * mPtr = entObjRuntime->GetObjByNetworkID(header->m_optNetID);
					if(mPtr)
					{//valid
						CMovementObj * DBPtr = m_gStatus->m_entObjList->GetByID(mPtr->List_refModel);
						mPtr->ArmItemByGLID(header->m_glid,DBPtr,m_gStatus->m_itemManager,NULL);
					}
				}
				
				break;
			}
			case 15:
			{
				if(miscBlockSize > 1)
				{
					int parser = 1;
					SQUAD_MESSAGE *msg = new SQUAD_MESSAGE();

					//Copy Memory and increase parser
					//	NOTE: We should only be processing one SQUAD_MESSAGE packet!
					CopyMemory(&msg->NetID, &miscUpdateBlock[parser], sizeof(msg->NetID));
					parser += sizeof(msg->NetID);
					CopyMemory(&msg->Health, &miscUpdateBlock[parser], sizeof(msg->Health));
					parser += sizeof(msg->Health);
					CopyMemory(&msg->Position, &miscUpdateBlock[parser], sizeof(msg->Position));
					parser += sizeof(msg->Position);

					// PARSE player name
					CopyMemory(&msg->PlayerNameLength, &miscUpdateBlock[parser], sizeof(msg->PlayerNameLength));
					parser += sizeof(msg->PlayerNameLength);
					msg->PlayerName = new char[msg->PlayerNameLength+1];
					CopyMemory(msg->PlayerName, &miscUpdateBlock[parser], msg->PlayerNameLength);
					parser += msg->PlayerNameLength;
					msg->PlayerName[msg->PlayerNameLength] = '\0';

					// PARSE squad name
					CopyMemory(&msg->SquadNameLength, &miscUpdateBlock[parser], sizeof(msg->SquadNameLength));
					parser += sizeof(msg->SquadNameLength);
					msg->SquadName = new char[msg->SquadNameLength+1];
					CopyMemory(msg->SquadName, &miscUpdateBlock[parser], msg->SquadNameLength);
					parser += msg->SquadNameLength;
					msg->SquadName[msg->SquadNameLength] = '\0';

					// SAVE the squad name locally
					m_gStatus->m_multiplayerObject->m_squadName = msg->SquadName;
					
					//Check validity
					if(msg->NetID > -1)
					{
						// OPEN Group Invite menu for accept/decline
						CMenuText *pPlayerNameText = (CMenuText*)m_gStatus->m_menuDatabase->GetVisualByID(CLAN_RECRUIT_NAME, true);
						if (pPlayerNameText)
							pPlayerNameText->SetText(msg->PlayerName);
					}

					// RELEASE heap memory
					SafeDelete(msg);
				}
				break;
			}
		}//end switch
}
//-----------------------------------------------------------//
BOOL CClientNetworkingLogic::SpawnEntityBasedOnPositionAndCfg(  D3DVECTOR position, 
																int aiCfg,
																int dynamicSpawnId,
																long lifeMaxTime,
																CMovementObjList * entObjRuntime,
																int clanID,
																CCollisionObj * collisionBase,
																CCollisionObjList * collisionDatabase,
																int maxSpawnPerDynaID,
																CString worldFile,
																int rotationRange,
																int worldChannel
																)
{

     if(!collisionBase)
		 collisionBase = m_gStatus->m_collisionBase;

	 if(!collisionDatabase)
		 collisionDatabase = m_gStatus->m_collisionDatabase;


	 CAIObj * aiPtr = m_gStatus->m_aiList->GetByIndex(aiCfg);
     if(!aiPtr)
	 {//Invalid ai ptr
	   return FALSE;
	 }//end invalid ai ptr


     //idiot protection
	 if(lifeMaxTime > 2700000)
		lifeMaxTime = 2700000;

     if(maxSpawnPerDynaID > 0)
	 if(dynamicSpawnId > -1)
	 {//this spawn cannot preexist in the world-check to see if already spawned
        //if(m_gStatus->m_entObjRuntime->GetObjectByDynaID(dynamicSpawnId))
		   //return FALSE;

		if(entObjRuntime->GetObjectCountByDynaID(dynamicSpawnId) > maxSpawnPerDynaID)
		   return FALSE;
	 }//end this spawn cannot preexist in the world
		   
     //add conditions for not spawning
     D3DMATRIX successMatrice = m_gStatus->m_matrixInit2;//spawnGenPtr->List_spawnMatrix;
	 successMatrice._41 = position.x;
	 successMatrice._42 = position.y;
	 successMatrice._43 = position.z;
	 
	 //add the rotation of the entity
	 int range = rotationRange;
	 if(range > 0)
		 range = rand() % rotationRange;
	 D3DMATRIX rotationMatrix;

	 /*m_gStatus->m_matLib.RotateY(abs(range) - 180, rotationMatrix.m);*/
	m_gStatus->m_matLib.RotateY((float)(abs(range) - 180), rotationMatrix.m);
	//WARNING: C4244 --> int to float

	 m_gStatus->m_matLib.fastMatrixMultiply(successMatrice, rotationMatrix, successMatrice);

     CMovementObj * refPtr = NULL;
     BOOL result = m_gStatus->ActivateMovementEntity(NULL,
			                              aiCfg,
			                              successMatrice,
		                                  0,
										  worldChannel,
										  aiPtr->m_entityCfgDBindexRef,//spawnGenPtr->List_spawnIndex,
										  5,//hardcode ai to player-spawnGenPtr->List_spawnType,
										  0,
										  0,
										  FALSE,
										  FALSE,
										  FALSE,
										  -1,
										  -1,
										  &refPtr,
					                      NULL,
										  -1,
										  -1,
									      collisionBase,
									      collisionDatabase,
										  entObjRuntime,
										  worldFile);
	  if(result){//valid spawn
	   if(refPtr->m_aiObj){//if AI
		  refPtr->m_aiObj->m_routeCurTarget = 0;
		  refPtr->m_aiObj->m_mapCurLocation = 0;
          //refPtr->m_aiObj->m_curMap         = spawnGenPtr->m_treeUse;
		  refPtr->m_dynaSpawnID = dynamicSpawnId;
		  refPtr->m_lifeTimeMax = lifeMaxTime;
		  refPtr->m_networkClanID = clanID;
           
          if(refPtr->m_aiObj->m_physicsModuleOverride)
		  {//Movment ovverride valid
		     if(refPtr->List_movementDB)
			 {
                refPtr->List_movementDB->SafeDelete();
			    delete refPtr->List_movementDB;
			    refPtr->List_movementDB=0;
			 }
             refPtr->m_aiObj->m_physicsModuleOverride->Clone(&refPtr->List_movementDB);
			 refPtr->List_movementDB->List_lastPosition.x = successMatrice._41;
             refPtr->List_movementDB->List_lastPosition.y = successMatrice._42;
             refPtr->List_movementDB->List_lastPosition.z = successMatrice._43;
		  }//end Movment ovverride valid
	   }
	  }
	  return TRUE;
}
//-----------------------------------------------------------//
void CClientNetworkingLogic::HandleDmgMsgHandler(LPDAMAGEINFO_DATA formatedMessage)
{
	   if(formatedMessage->m_damageAmount > 0)
	   {
          CMovementObj * targetPtr = (CMovementObj *)m_gStatus->m_entObjRuntime->GetObjByNetworkID(formatedMessage->m_targetPlayerID);
	      if(targetPtr)
		  {
		    CString build = "You have damaged "+targetPtr->List_name+" for ";
	        build = build+m_gStatus->m_cabLib.ConvToString(formatedMessage->m_damageAmount);
			if(m_gStatus->m_systemReadout)
	           m_gStatus->m_systemReadout->AddLine(build, MENU_TEXTCOLOR_DAMAGE);
			if(formatedMessage->m_death)
			{
				if(m_gStatus->m_tgSys_entitySelected > -1)
				{
					CMovementObj * selectedEntPtr = m_gStatus->m_entObjRuntime->GetObjByTrace(m_gStatus->m_tgSys_entitySelected);
					if(selectedEntPtr)
						selectedEntPtr->m_selectionEffectState = -1;

					m_gStatus->m_tgSys_entitySelected = -1;
				}

				if(m_gStatus->m_systemReadout)
				   m_gStatus->m_systemReadout->AddLine("You have killed " + targetPtr->List_name);
				m_gStatus->m_tgSys_indexOfObjectSelected = -1;
				targetPtr->List_energy = -1;
			}
	
		  //Show the Damage overhead
		  D3DVECTOR pos;
		  targetPtr->List_baseFrame->GetPosition(NULL, &pos);
		  pos.y += (targetPtr->List_skeletalObject->m_box.maxY + 1.0f);

		  m_gStatus->m_textEffectRuntime->SpawnText(m_gStatus->m_cabLib.ConvToString(formatedMessage->m_damageAmount),
													2500,
													pos,
													2.0f,
													1.0f,
													0.2f,
													0.2f,
													0.9f,
													0.9f,
													m_gStatus->m_textRenderList,
													m_gStatus->m_symbolMapDB,
													m_gStatus->m_3dDevice,
													m_gStatus->m_cgFontList);

		  }
	   }
	   else if(formatedMessage->m_damageAmount < 0)
	   {
          CMovementObj * targetPtr = (CMovementObj *)m_gStatus->m_entObjRuntime->GetObjByNetworkID(formatedMessage->m_targetPlayerID);
	      if(targetPtr)
		  {
		    CString build = "You have healed "+targetPtr->List_name+" for ";
	        build = build+m_gStatus->m_cabLib.ConvToString(abs(formatedMessage->m_damageAmount));
	        
			if(m_gStatus->m_systemReadout)
			m_gStatus->m_systemReadout->AddLine(build, MENU_TEXTCOLOR_HEAL);

			//Show the Healing overhead
			D3DVECTOR pos;
			targetPtr->List_baseFrame->GetPosition(NULL, &pos);
			pos.y += (targetPtr->List_skeletalObject->m_box.maxY + 1.0f);

			m_gStatus->m_textEffectRuntime->SpawnText(  m_gStatus->m_cabLib.ConvToString(formatedMessage->m_damageAmount),
														1500,
														pos,
														1.7f,
														0.2f,
														1.0f,
														0.2f,
														0.9f,
														0.9f,
														m_gStatus->m_textRenderList,
														m_gStatus->m_symbolMapDB,
														m_gStatus->m_3dDevice,
														m_gStatus->m_cgFontList);
		  }
	   }
	   else 
	   {
		   if(m_gStatus->m_systemReadout)
				m_gStatus->m_systemReadout->AddLine("Weapon Ineffective...");
	   }
}
//-----------------------------------------------------------//
void CClientNetworkingLogic::HandleGeneralMsgHandler(LPMSGHANDLER formatedMessage)
{
	if (!m_gStatus->m_systemReadout)
		return;

	   switch(formatedMessage->m_data)
	   {
		case 1:m_gStatus->m_systemReadout->AddLine("MUST WAIT BEFORE USING ANOTHER ITEM..", MENU_TEXTCOLOR_SERVER);break;
		case 3:m_gStatus->m_systemReadout->AddLine("CANNOT BE WORN BY THIS RACE..", MENU_TEXTCOLOR_SERVER);break;
		case 5:m_gStatus->m_systemReadout->AddLine("YOU ARE NO LONGER UNDER PROTECTION..", MENU_TEXTCOLOR_SERVER);break;
		case 6:m_gStatus->m_systemReadout->AddLine("YOU HAVE ENTERED A PROTECTED AREA..", MENU_TEXTCOLOR_SERVER);break;
		case 7:m_gStatus->m_systemReadout->AddLine("YOU REMEMBER THIS PLACE WELL..", MENU_TEXTCOLOR_SERVER);break;
		case 8:m_gStatus->m_systemReadout->AddLine("NOT ENOUGH AMMO..", MENU_TEXTCOLOR_SERVER);break;
		case 9:m_gStatus->m_systemReadout->AddLine("INCOMPATIBLE..", MENU_TEXTCOLOR_SERVER);break;
		case 10:m_gStatus->m_systemReadout->AddLine("CANNOT LOOT THIS TYPE IN SAFE ZONE..", MENU_TEXTCOLOR_SERVER);break;
		case 11:
		{
			m_gStatus->m_systemReadout->AddLine("THE KEY TO ENTRY IS NOT IN YOUR POSSESION...", MENU_TEXTCOLOR_SERVER);
			m_gStatus->m_multiplayerObject->m_requestingPortal = FALSE;
			break;
		}
		case 12:m_gStatus->m_multiplayerObject->m_requestingPortal = FALSE;break;
		case 14:m_gStatus->m_systemReadout->AddLine("ITEM HAS SOME SORT OF LOCKING SYSTEM... AND YOU HAVE NO KEY IN YOUR POSSESION...", MENU_TEXTCOLOR_SERVER);break;
		case 15:m_gStatus->m_systemReadout->AddLine("YOU HAVE UNLOCKED IT", MENU_TEXTCOLOR_SERVER);break;
		case 16:m_gStatus->m_systemReadout->AddLine("YOU HAVE UNLOCKED THIS.. BUT IN THE PROCESS DESTROYED THE KEY...", MENU_TEXTCOLOR_SERVER);
		case 17:m_gStatus->m_systemReadout->AddLine("YOU SEE NOTHING INTERESTING HERE...", MENU_TEXTCOLOR_SERVER);break;
		case 18:m_gStatus->m_systemReadout->AddLine("YOU HAVE FAILED AT THE ATTEMPT", MENU_TEXTCOLOR_SERVER);break;
		case 19:m_gStatus->m_systemReadout->AddLine("YOU LACK THE STATISTICS TO ATTEMPT THIS...", MENU_TEXTCOLOR_SERVER);break;
		case 20:m_gStatus->m_systemReadout->AddLine("YOU LACK THE REQUIRED ITEMS TO ATTEMPT THIS...", MENU_TEXTCOLOR_SERVER);break;
		case 21:m_gStatus->m_systemReadout->AddLine("THE ATTEMPT SUCCEEDED...", MENU_TEXTCOLOR_SERVER);break;
		case 22:m_gStatus->m_systemReadout->AddLine("YOU LACK THE SKILL LEVEL TO ATTEMPT THIS...", MENU_TEXTCOLOR_SERVER);break;
		case 23:
		{
			m_gStatus->m_systemReadout->AddLine("YOU ARE NOW VISIBLE...", MENU_TEXTCOLOR_SERVER);
			m_gStatus->m_runtimeMenuDatabase->CloseByID(HIDDEN_SCREEN_FLASH, &m_gStatus->m_showMenuCursor);
			m_gStatus->m_multiplayerObject->m_currentStasis = "NORMAL";
			break;
		}
		case 24:m_gStatus->m_systemReadout->AddLine("YOU ARE ALREADY USING A SKILL...", MENU_TEXTCOLOR_SERVER);break;
		case 25:m_gStatus->m_systemReadout->AddLine("YOU HAVE LEARNED NO SKILLS YET...", MENU_TEXTCOLOR_SERVER);break;
		case 26:m_gStatus->m_systemReadout->AddLine("YOU CANNOT USE ANOTHER SKILL YET...", MENU_TEXTCOLOR_SERVER);break;
		case 27:m_gStatus->m_systemReadout->AddLine("YOU CANNOT ATTACK ANOTHER PLAYER TILL YOU ARE LEVEL 10 OR GREATER IN ANY ONE SKILL...", MENU_TEXTCOLOR_SERVER);break;
		case 28:m_gStatus->m_systemReadout->AddLine("YOU CANNOT ATTACK ANOTHER PLAYER THAT IS DOES NOT HAVE AT LEAST LEVEL 10 IN ANY SKILL...", MENU_TEXTCOLOR_SERVER);break;
		case 29:m_gStatus->m_systemReadout->AddLine("You are overloaded and cannot hold anymore...", MENU_TEXTCOLOR_SERVER);break;
		case 30:m_gStatus->m_systemReadout->AddLine("Your bank cannot hold anymore separate items, it is overloaded...", MENU_TEXTCOLOR_SERVER);break;
		case 31:m_gStatus->m_systemReadout->AddLine("You do not have the room in your inventory to complete the trade...", MENU_TEXTCOLOR_SERVER);break;
		case 32:m_gStatus->m_systemReadout->AddLine("The person that you are trading with does not have room enough in his inventory to complete the trade...", MENU_TEXTCOLOR_SERVER);break;
		case 33:m_gStatus->m_systemReadout->AddLine("Item not present...", MENU_TEXTCOLOR_SERVER);break;
		case 34:m_gStatus->m_systemReadout->AddLine("You lack a component...", MENU_TEXTCOLOR_SERVER);break;
		case 35:m_gStatus->m_systemReadout->AddLine("Target item already contains this...", MENU_TEXTCOLOR_SERVER);break;
		case 36:m_gStatus->m_systemReadout->AddLine("You have no item in your possesion that will combine with this...", MENU_TEXTCOLOR_SERVER);break;
		case 37:m_gStatus->m_systemReadout->AddLine("You have combined this item sucessfully...", MENU_TEXTCOLOR_SERVER);break;
		case 38:m_gStatus->m_systemReadout->AddLine("You lack this item in your inventory...", MENU_TEXTCOLOR_SERVER);break;
		case 39:m_gStatus->m_systemReadout->AddLine("Your murder count has increased...", MENU_TEXTCOLOR_SERVER);break;
		case 40:m_gStatus->m_systemReadout->AddLine("You are already a bound to a clan...", MENU_TEXTCOLOR_SERVER);break;
		case 41:m_gStatus->m_systemReadout->AddLine("You have successfully created a new clan...", MENU_TEXTCOLOR_SERVER);break;
		case 42:m_gStatus->m_systemReadout->AddLine("Clan name already in use...", MENU_TEXTCOLOR_SERVER);break;
		case 43:m_gStatus->m_systemReadout->AddLine("Clan prefix too long...", MENU_TEXTCOLOR_SERVER);break;
		case 44:m_gStatus->m_systemReadout->AddLine("Clan not found...", MENU_TEXTCOLOR_SERVER);break;
		case 45:m_gStatus->m_systemReadout->AddLine("Clan request has been sent to clan leader...", MENU_TEXTCOLOR_SERVER);break;
		case 46:m_gStatus->m_systemReadout->AddLine("Clan leader must be near you...", MENU_TEXTCOLOR_SERVER);break;
		case 47:m_gStatus->m_systemReadout->AddLine("You have been accepted...", MENU_TEXTCOLOR_SERVER);break;
		case 48:m_gStatus->m_systemReadout->AddLine("New clan member successfully added...", MENU_TEXTCOLOR_SERVER);break;
		case 49:m_gStatus->m_systemReadout->AddLine("Cannot find the player that requested approval...", MENU_TEXTCOLOR_SERVER);break;
		case 50:m_gStatus->m_systemReadout->AddLine("Member has been removed from clan...", MENU_TEXTCOLOR_SERVER);break;
		case 51:m_gStatus->m_systemReadout->AddLine("Cannot find the member...", MENU_TEXTCOLOR_SERVER);break;
		case 52:m_gStatus->m_systemReadout->AddLine("You have left your clan...", MENU_TEXTCOLOR_SERVER);break;
		case 53:m_gStatus->m_systemReadout->AddLine("We will not deal with the likes of you here...", MENU_TEXTCOLOR_SERVER);break;
		case 54:m_gStatus->m_systemReadout->AddLine("You cannot do this shape shifted...", MENU_TEXTCOLOR_SERVER);break;
		case 55:m_gStatus->m_systemReadout->AddLine("You must be level 8 in any skill to loot this type...", MENU_TEXTCOLOR_SERVER);break;
		case 56:m_gStatus->m_systemReadout->AddLine("A house has already been placed on this account...", MENU_TEXTCOLOR_SERVER);break;
		case 57:m_gStatus->m_systemReadout->AddLine("You cannot place this here, you are not in a housing zone...", MENU_TEXTCOLOR_SERVER);break;
		case 58:m_gStatus->m_systemReadout->AddLine("You have placed your house successfully...", MENU_TEXTCOLOR_SERVER);break;
		case 59:m_gStatus->m_systemReadout->AddLine("You are too close to another house...", MENU_TEXTCOLOR_SERVER);break;
		case 60:m_gStatus->m_systemReadout->AddLine("Cannot hide while riding...", MENU_TEXTCOLOR_SERVER);break;
		case 61:
		   {
			  if(!m_gStatus->m_multiplayerObject->m_nonPkServer)
				 m_gStatus->m_systemReadout->AddLine("Cannot loot a corpse that is under level ten in any skill...", MENU_TEXTCOLOR_SERVER);
			  else
				 m_gStatus->m_systemReadout->AddLine("Cannot loot this players corpse...", MENU_TEXTCOLOR_SERVER);
			  break;
		   }
		case 62:m_gStatus->m_systemReadout->AddLine("You must be the leader of a clan...", MENU_TEXTCOLOR_SERVER);break;
		case 63:m_gStatus->m_systemReadout->AddLine("Clan sharing is enabled...", MENU_TEXTCOLOR_SERVER);break;
		case 64:m_gStatus->m_systemReadout->AddLine("Clan sharing is disabled...", MENU_TEXTCOLOR_SERVER);break;
		case 65:m_gStatus->m_systemReadout->AddLine("You don't own own a property...", MENU_TEXTCOLOR_SERVER);break;
		case 66:m_gStatus->m_systemReadout->AddLine("No Vault Currently Installed...", MENU_TEXTCOLOR_SERVER);break;
		case 67:m_gStatus->m_systemReadout->AddLine("Installed...", MENU_TEXTCOLOR_SERVER);break;
		case 68:m_gStatus->m_systemReadout->AddLine("The cannot have more than 100 people in your group list...", MENU_TEXTCOLOR_SERVER);break;
		case 69:m_gStatus->m_systemReadout->AddLine("The other party's group list is full...", MENU_TEXTCOLOR_SERVER);break;
		case 70:m_gStatus->m_systemReadout->AddLine("Upgrade successful...", MENU_TEXTCOLOR_SERVER);break;
		case 71:m_gStatus->m_systemReadout->AddLine("You must be near your house to upgrade it...", MENU_TEXTCOLOR_SERVER);break;
		case 72:m_gStatus->m_systemReadout->AddLine("You declared War...", MENU_TEXTCOLOR_SERVER);break;
		case 73:m_gStatus->m_systemReadout->AddLine("The war begins...", MENU_TEXTCOLOR_SERVER);break;
		case 74:m_gStatus->m_systemReadout->AddLine("You have surrendered...", MENU_TEXTCOLOR_SERVER);break;
		case 75:m_gStatus->m_systemReadout->AddLine("You are at war with too many clans...", MENU_TEXTCOLOR_SERVER);break;
		case 76:m_gStatus->m_systemReadout->AddLine("Other clan leader must be online...", MENU_TEXTCOLOR_SERVER);break;
		case 77:m_gStatus->m_systemReadout->AddLine("You are already at war with this clan...", MENU_TEXTCOLOR_SERVER);break;
		case 78:m_gStatus->m_systemReadout->AddLine("You cannot cast while armed...", MENU_TEXTCOLOR_SERVER);break;
		case 79:m_gStatus->m_systemReadout->AddLine("Your request has been placed into the queue, and a gamemaster will be with you as soon as possible...", MENU_TEXTCOLOR_SERVER);break;
		case 80:m_gStatus->m_systemReadout->AddLine("Player just spawned...", MENU_TEXTCOLOR_SERVER);break;
		case 81:m_gStatus->m_systemReadout->AddLine("You cannot do damage or take damage for 10 seconds after zoning between worlds.. ", MENU_TEXTCOLOR_SERVER);break;
		case 82:m_gStatus->m_systemReadout->AddLine("There is no store here.", MENU_TEXTCOLOR_SERVER);break;
		case 83:m_gStatus->m_systemReadout->AddLine("You cannot afford this.", MENU_TEXTCOLOR_SERVER);break;
		case 84:m_gStatus->m_systemReadout->AddLine("Transfer Completed.", MENU_TEXTCOLOR_SERVER);break;
		case 85:m_gStatus->m_systemReadout->AddLine("No more space available.", MENU_TEXTCOLOR_SERVER);break;
		case 86:m_gStatus->m_systemReadout->AddLine("Since you are new to this realm, it is too early to lose your possesions. They are now back in your possession.", MENU_TEXTCOLOR_SERVER);break;
		case 87:m_gStatus->m_systemReadout->AddLine("Cannot do this yet... you are still invunerable.", MENU_TEXTCOLOR_SERVER);break;
		case 88:m_gStatus->m_systemReadout->AddLine("You must wait longer before you can use this skill again.", MENU_TEXTCOLOR_SERVER);break;
		case 89:m_gStatus->m_systemReadout->AddLine("You cannot do this here...", MENU_TEXTCOLOR_SERVER);break;
		case 90:m_gStatus->m_systemReadout->AddLine("Completed...", MENU_TEXTCOLOR_SERVER);break;
		case 91:m_gStatus->m_systemReadout->AddLine("Powerup is being overwritten...", MENU_TEXTCOLOR_SERVER);break;
		case 92:m_gStatus->m_systemReadout->AddLine("You lack the amount of crystals to use this.", MENU_TEXTCOLOR_SERVER);break;
		case 93:m_gStatus->m_systemReadout->AddLine("You lack the Energy to attempt this.", MENU_TEXTCOLOR_SERVER);break;
		case 94:m_gStatus->m_systemReadout->AddLine("You do not have leadership privledges.", MENU_TEXTCOLOR_SERVER);break;
		case 95:m_gStatus->m_systemReadout->AddLine("Player is already in a group.", MENU_TEXTCOLOR_SERVER);break;
		case 96:m_gStatus->m_systemReadout->AddLine("Player is already in your group.", MENU_TEXTCOLOR_SERVER);break;
		case 97:m_gStatus->m_systemReadout->AddLine("Reloading ammo type that is different than what is currently armed.", MENU_TEXTCOLOR_SERVER);break;
		case 98:
		{
//			m_gStatus->m_chatReadout->AddLine("You have hidden.", MENU_TEXTCOLOR_SERVER);
			m_gStatus->m_systemReadout->AddLine("You have hidden.", MENU_TEXTCOLOR_SERVER);
			m_gStatus->OpenMenu(HIDDEN_SCREEN_FLASH, false);
			CMenuImage *pImageFullscreen = dynamic_cast<CMenuImage*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(HIDDEN_SCREEN_VISUAL, true));
			if(pImageFullscreen)
			{
				pImageFullscreen->m_height = (float)m_gStatus->m_dxRenderer->m_currentHeight;
				pImageFullscreen->m_width = (float)m_gStatus->m_dxRenderer->m_currentWidth;
			}
			break;
		}
		default:m_gStatus->m_systemReadout->AddLine("HandleGeneralMsgHandler error cannot find case:",(DWORD)formatedMessage->m_data);break;
	}
}
//-----------------------------------------------------------//
void CClientNetworkingLogic::HandleDetailedMsgHandler(LPDETAILEDMSG_DATA formatedMessage,
													  CMovementObjList * entObjRuntime,
													  CMissileObjList * missileRuntime,
													  CExplosionObjList * explosionRuntimeList)
{
	   if(formatedMessage->m_data == 1)
	   {//required items for a skill display
          CSkillObject * skillPtr = m_gStatus->m_skillDatabase->GetSkillByType(formatedMessage->m_miscInt);
		  if(skillPtr)
		  {
			if(skillPtr->m_levelRangeDB)
			{
             CLevelRangeObject * levelPtr = skillPtr->m_levelRangeDB->GetObjByIndex(formatedMessage->m_miscInt2);
             if(levelPtr)
			 {
                if(levelPtr->m_abilityDB)
				{
                   CSkillAbilityObject * abObj = levelPtr->m_abilityDB->GetObjByIndex(formatedMessage->m_miscInt3);
				   if(abObj)
				   {
					   CString build = "To do this you will need:";
					   if(abObj->m_requiredItems)
						  for(POSITION reqItemPos = abObj->m_requiredItems->GetHeadPosition();reqItemPos != NULL;)
						  {
                              CInventoryObj * invPtr = (CInventoryObj *)abObj->m_requiredItems->GetNext(reqItemPos);
							  if(invPtr->m_itemData){
								 build = build+"-";
								 build = build+invPtr->m_itemData->m_itemName;
							  }
						  }

					   if(m_gStatus->m_systemReadout)
							m_gStatus->m_systemReadout->AddLine(build);
				   }
				}
			 }
			}
		  }
	   }//end required items for a skill display
	   if(formatedMessage->m_data == 2)
	   {//required statistics
          CSkillObject * skillPtr = m_gStatus->m_skillDatabase->GetSkillByType(formatedMessage->m_miscInt);
		  if(skillPtr)
		  {
			if(skillPtr->m_levelRangeDB)
			{
             CLevelRangeObject * levelPtr = skillPtr->m_levelRangeDB->GetObjByIndex(formatedMessage->m_miscInt2);
             if(levelPtr)
			 {
                if(levelPtr->m_abilityDB)
				{
                   CSkillAbilityObject * abObj = levelPtr->m_abilityDB->GetObjByIndex(formatedMessage->m_miscInt3);
				   if(abObj)
				   {
					   CString build = "You lack the stat requirement:";
					   if(abObj->m_requiredStatistics)
						  for(POSITION reqItemPos = abObj->m_requiredStatistics->GetHeadPosition();reqItemPos != NULL;)
						  {
                              CStatObj * invPtr = (CStatObj *)abObj->m_requiredStatistics->GetNext(reqItemPos);
							  build = build+"-";
							  build = build+invPtr->m_statName;
							  build = build+" ";
							  build = build+m_gStatus->m_cabLib.ConvToString((int)invPtr->m_currentValue);
						  }

						if(m_gStatus->m_systemReadout)
							m_gStatus->m_systemReadout->AddLine(build);
				   }
				}
			 }
			}
		  }
	   }//end required statistics
	   if(formatedMessage->m_data == 3)
	   {//required statistics by glid
          CItemObj * invObj = m_gStatus->m_itemManager->GetItemByID(formatedMessage->m_miscInt);
		  if(invObj)
		  {//valid
			 CString build = "You lack the stat requirement:";
             
             //str
			 build = build+"STR";
			 build = build+" ";
			 build = build+m_gStatus->m_cabLib.ConvToString(invObj->m_requiredStr);
             //dex
			 build = build+"-";
			 build = build+"DEX";
			 build = build+" ";
			 build = build+m_gStatus->m_cabLib.ConvToString(invObj->m_requiredDex);
             //wis
			 build = build+"-";
			 build = build+"WIS";
			 build = build+" ";
			 build = build+m_gStatus->m_cabLib.ConvToString(invObj->m_requiredKnowledge);

			 if(m_gStatus->m_systemReadout)
                m_gStatus->m_systemReadout->AddLine(build);
		  }//end valid
	   }//end required statisic by glid
	   if(formatedMessage->m_data == 4)
	   {//somone has mounted a pet or vehicle
          CMovementObj * targetPtr = (CMovementObj *)m_gStatus->m_entObjRuntime->GetObjByNetworkID(formatedMessage->m_miscInt);
	      if(targetPtr)
		  {
			m_gStatus->GenAndMount(targetPtr,formatedMessage->m_miscInt2,entObjRuntime,missileRuntime);
		  }
	   }//end somone has mounted a pet or vehicle
	   if(formatedMessage->m_data == 5)
	   {//somone has demounted a pet or vehicle
          CMovementObj * targetPtr = (CMovementObj *)m_gStatus->m_entObjRuntime->GetObjByNetworkID(formatedMessage->m_miscInt);
	      if(targetPtr)
		  {
			m_gStatus->DeMount(targetPtr,entObjRuntime);
		  }
	   }//end somone has demounted a pet or vehicle
	   if(formatedMessage->m_data == 6)
	   {//spawn effect message
		  float convPush = (float)formatedMessage->m_miscInt3/10.0f; //push
          m_gStatus->DoExpEffect(formatedMessage->m_miscInt,formatedMessage->m_miscInt2,explosionRuntimeList,convPush,0.0f);
	   }//end spawn effect message
       if(formatedMessage->m_data == 7)
	   {//launch missile
	      POSITION entPosition = m_gStatus->m_entObjRuntime->FindIndex(0);
		  if(entPosition)
		  {//valid entity   
	       CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPosition);
		   D3DMATRIX positionalMatrix;
		   ptrLoc->List_baseFrame->GetTransform(NULL,&positionalMatrix);
		   if(ptrLoc->List_controlType == 0)
		      m_gStatus->OverideMissile(ptrLoc,
										ptrLoc->List_movementDB->List_freeLookCurPitch,
										positionalMatrix,
										ptrLoc->List_traceNumber,
										TRUE,//BOOL               sendOverNetwork,
										ptrLoc->List_movementDB->List_freeLookCurYaw,
										formatedMessage->m_miscInt,
										missileRuntime);//int                missileType);
		  }
	   }//end launch missile
}
//------------------------------------------------------------------------------------//
BOOL CClientNetworkingLogic::DeleteStringFromMenuList(int controlID,int index)
{
   for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
   {//loop through menus
      CMenu3DObj* menu = (CMenu3DObj*)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
	  if(menu->m_visualItems)
		 for(POSITION visPos = menu->m_visualItems->GetHeadPosition();visPos != NULL;)
		 {//visual items loop
             CMenuVisObj * visPtr = (CMenuVisObj *)menu->m_visualItems->GetNext(visPos);
			 if(visPtr->m_sliderSystem)
			 {//valid slider system
                if(visPtr->m_sliderSystem->m_controlID == controlID)
				{//correct control ID
                    visPtr->m_sliderSystem->DeleteString(index);
					return TRUE;
				}//end correct control ID
			 }//end valid slider system
		 }//end visual items loop
   }//end loop through menus
   return FALSE;
}
//------------------------------------------------------------------------------------//
void CClientNetworkingLogic::ClearAllFromMenuList(int controlID)
{
   for(POSITION posLoc = m_gStatus->m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
   {//loop through menus
      CMenu3DObj* menu = (CMenu3DObj*)m_gStatus->m_runtimeMenuDataBase->GetNext(posLoc);
	  if(menu->m_visualItems)
		 for(POSITION visPos = menu->m_visualItems->GetHeadPosition();visPos != NULL;)
		 {//visual items loop
             CMenuVisObj * visPtr = (CMenuVisObj *)menu->m_visualItems->GetNext(visPos);
			 if(visPtr->m_sliderSystem)
			 {//valid slider system
				if(visPtr->m_sliderSystem->m_contentList)
                if(visPtr->m_sliderSystem->m_controlID == controlID)
				{//correct control ID
					for(POSITION posObjContent = visPtr->m_sliderSystem->m_contentList->GetHeadPosition();posObjContent != NULL;)
					{
					    CSliderContentObj * slObjPtr = (CSliderContentObj *)visPtr->m_sliderSystem->m_contentList->GetNext(posObjContent);
					    delete slObjPtr;
						slObjPtr = 0;
					}
					visPtr->m_sliderSystem->m_contentList->RemoveAll();
					return;
				}//end correct control ID
			 }//end valid slider system
		 }//end visual items loop
   }//end loop through menus
   return;
}
//--------------send message----------------------------------------------------------//
BOOL CClientNetworkingLogic::SendMovMsg(            DWORD flags,
													DPNID sendTo,
													D3DVECTOR curPosition,
													long refrenceID,
													D3DVECTOR dirVect, //here are the dir and up vectors
													D3DVECTOR upVect,
													float FreeLookYaw,
													float FreeLookPitch,
													short dbIndex,
													short curAnim,
													short secondaryAnim,
													long currenttimeEL) 
{

    LPMOVE_STRUCT lpChatMessage = NULL;
	DWORD				messageSize;
	messageSize = sizeof(MOVE_STRUCT);
    lpChatMessage = new MOVE_STRUCT;
	if (lpChatMessage == NULL)
	{
		return FALSE;
	}

	lpChatMessage->m_catagorie    = 2;
	lpChatMessage->FreeLookYaw    = FreeLookYaw;
    lpChatMessage->FreeLookPitch  = FreeLookPitch;
	lpChatMessage->Xpos           = curPosition.x;
	lpChatMessage->Ypos           = curPosition.y;
	lpChatMessage->Zpos           = curPosition.z;
    lpChatMessage->idRefrence     =  (short)refrenceID;
    lpChatMessage->OrientateDirX  = dirVect.x;
	lpChatMessage->OrientateDirY  = dirVect.y;
	lpChatMessage->OrientateDirZ  = dirVect.z;
	lpChatMessage->OrientateUpX   = upVect.x;
	lpChatMessage->OrientateUpY   = upVect.y;
	lpChatMessage->OrientateUpZ   = upVect.z;
	//lpChatMessage->dbIndex        = dbIndex;
	lpChatMessage->curAnim        = curAnim;
	lpChatMessage->secondaryAnim  = secondaryAnim;


	//shink data
	LPSHRUNK_TOSVRMOV_STRUCT shrunkMessage = NULL;
	DWORD				shrunkMessageSize;
	shrunkMessageSize = sizeof(SHRUNK_TOSVRMOV_STRUCT);
    shrunkMessage = new SHRUNK_TOSVRMOV_STRUCT;
	        
	if (shrunkMessage == NULL)
	{
		m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Cannot allocate memory in mov msg send");
		return FALSE;
	}
	shrunkMessage->tmel = currenttimeEL;
    shrunkMessage = m_gStatus->m_multiplayerObject->ShrinkDataCP(lpChatMessage,shrunkMessage);
	
	m_gStatus->m_multiplayerObject->SendData((BYTE *)shrunkMessage,
				     shrunkMessageSize,
				     flags,
				     sendTo,
					 m_gStatus->m_multiplayerObject->m_packetTimeout);

	if (lpChatMessage)
		delete lpChatMessage;
	    lpChatMessage=0;
	if (shrunkMessage)
        delete shrunkMessage;
	    shrunkMessage=0;

	return TRUE;
}
//--------------send message----------------------------------------------------------//
LPSHRUNK_TOSVRMOV_STRUCT CClientNetworkingLogic::GetMovMsg(     DWORD flags,
																DPNID sendTo,
																D3DVECTOR curPosition,
																long refrenceID,
																D3DVECTOR dirVect, //here are the dir and up vectors
																D3DVECTOR upVect,
																float FreeLookYaw,
																float FreeLookPitch,
																short dbIndex,
																short curAnim,
																short secondaryAnim,
																long currenttimeEL) 
{
    LPMOVE_STRUCT lpChatMessage = NULL;
	DWORD				messageSize;
	messageSize = sizeof(MOVE_STRUCT);
    lpChatMessage = new MOVE_STRUCT;
	if (lpChatMessage == NULL)
	{
		return NULL;
	}

	lpChatMessage->m_catagorie    = 2;
	lpChatMessage->FreeLookYaw    = FreeLookYaw;
    lpChatMessage->FreeLookPitch  = FreeLookPitch;
	lpChatMessage->Xpos           = curPosition.x;
	lpChatMessage->Ypos           = curPosition.y;
	lpChatMessage->Zpos           = curPosition.z;
    lpChatMessage->idRefrence     =  (short)refrenceID;
    lpChatMessage->OrientateDirX  = dirVect.x;
	lpChatMessage->OrientateDirY  = dirVect.y;
	lpChatMessage->OrientateDirZ  = dirVect.z;
	lpChatMessage->OrientateUpX   = upVect.x;
	lpChatMessage->OrientateUpY   = upVect.y;
	lpChatMessage->OrientateUpZ   = upVect.z;
	lpChatMessage->curAnim        = curAnim;
	lpChatMessage->secondaryAnim  = secondaryAnim;
	//shink data
	LPSHRUNK_TOSVRMOV_STRUCT shrunkMessage = NULL;
	DWORD				shrunkMessageSize;
	shrunkMessageSize = sizeof(SHRUNK_TOSVRMOV_STRUCT);
    shrunkMessage = new SHRUNK_TOSVRMOV_STRUCT;
	        
	if (shrunkMessage == NULL)
	{
		m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Cannot allocate memory in mov msg send");
		return FALSE;
	}
	shrunkMessage->tmel = currenttimeEL;
    shrunkMessage = m_gStatus->m_multiplayerObject->ShrinkDataCP(lpChatMessage,shrunkMessage);
	
	/*m_gStatus->m_multiplayerObject->SendData((BYTE *)shrunkMessage,
				     shrunkMessageSize,
				     flags,
				     sendTo,
					 m_gStatus->m_multiplayerObject->m_packetTimeout);*/

	if (lpChatMessage)
		delete lpChatMessage;
	    lpChatMessage=0;
	/*if (shrunkMessage)
        delete shrunkMessage;
	    shrunkMessage=0;*/

	return shrunkMessage;
}
//--------------send message----------------------------------------------------------//
BOOL CClientNetworkingLogic::SendAIBlockMsg(    DWORD flags,
												DPNID sendTo,
												SHRUNK_MOVE_STRUCT * m_dataBlock,
												int blockSize
												) 
{
    if(blockSize == 0)
	   return FALSE;

    BYTE * lpChatMessage = NULL;
	DWORD				 messageSize;
	messageSize = sizeof(AI_BLOCKPOSUPDATE_STRUCT) + sizeof(SHRUNK_MOVE_STRUCT)*blockSize;
    lpChatMessage = new BYTE[messageSize];
	if (lpChatMessage == NULL)
	{
		return FALSE;
	}

    

    BYTE * blockConversion = (BYTE *)m_dataBlock;//new BYTE[(sizeof(SHRUNK_MOVE_STRUCT)*blockSize)];
	lpChatMessage[0] = 17;
	
	for(int byteLoop = 0;byteLoop < (int)(messageSize-1);byteLoop++)
	    lpChatMessage[byteLoop+1] = blockConversion[byteLoop];//shift data


	
    m_gStatus->m_multiplayerObject->SendData((BYTE *)lpChatMessage,
				     messageSize,
				     flags,
				     sendTo,
					 m_gStatus->m_multiplayerObject->m_packetTimeout);


	if (lpChatMessage)
		delete [] lpChatMessage;
	    lpChatMessage=0;

	return TRUE;
}
//--------------multiplayer callback thingy-------------------------------------------//
BOOL CClientNetworkingLogic::RespawnOnClient(int spawnType,D3DVECTOR position)
{
  //delete node if exists
  POSITION posLoc = m_gStatus->m_entObjRuntime->FindIndex(0);
	  if(posLoc){//2
         CMovementObj * entityRuntimeObj = (CMovementObj *)m_gStatus->m_entObjRuntime->GetAt(posLoc);
         if(entityRuntimeObj->List_controlType == 0){//bingo
		    m_gStatus->m_entObjRuntime->RemoveAt(posLoc);
            m_gStatus->DestroyRuntimeEntity(entityRuntimeObj,m_gStatus->m_spawnGeneratorDB,m_gStatus->m_entObjRuntime);
			entityRuntimeObj=0;
		 }//end bingo
	  }//end 2
      D3DMATRIX matrixPos = m_gStatus->m_matrixInit2;
	  matrixPos._41 = position.x;
	  matrixPos._42 = position.y;
	  matrixPos._43 = position.z;
    
      BOOL result = m_gStatus->ActivateMovementEntity(
		                           NULL,
		                           0,
                                   matrixPos,
                                   0,
								   0,
								   spawnType,
								   0,//control typr
                                   0,
								   0,
								   FALSE,
								   TRUE,
								   FALSE,
								   -1,
								   -1,
								   NULL,
					               NULL,
								   -1,
								   -1,
								   m_gStatus->m_collisionBase,
								   m_gStatus->m_collisionDatabase,
								   m_gStatus->m_entObjRuntime,
								   m_gStatus->m_currentWorkingFile);
	  //InListRuntimeEntities(0);
	  if(!result)
		  return FALSE;

	  return TRUE;
}
//-----------------------------------------------------------------//
void CClientNetworkingLogic::PendingCommandsCallback()
{

	CPendingCommandObj * pObj = m_gStatus->m_pendingObject->CallBack();
	if(!pObj)
		return;//no commands at this time

	switch(pObj->m_command)
    {
	    case 1:
			m_gStatus->m_netGameLogicObject->SendAttribMsg( DPNSEND_GUARANTEED,
															DPID_SERVERPLAYER,
															27,//use item
															pObj->m_register0,
															pObj->m_register1,
															0,
															0,
															NULL);
		break;
	}
}

bool 
CClientNetworkingLogic::HandleArenaTypeRequestClientSide(BYTE *pData, 
														 int &iMessageSize, 
														 DPID &iFromID)
{
	// OPEN the arena registration menu
	m_gStatus->OpenMenu(ID_ARENA_REGISTRATION_01_MENU, false);

	// Retrieve references to the arena registration visuals
	CMenuWindow *pParentMenu = dynamic_cast<CMenuWindow*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_ARENA_REGISTRATION_01_MENU, true));
	CMenuToggle *pSoloArenasToggle = dynamic_cast<CMenuToggle*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_MODE_SOLO_TOGGLE, true));
	CMenuToggle *pTeamArenasToggle = dynamic_cast<CMenuToggle*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_MODE_TEAM_TOGGLE, true));
	CMenuToggle *pTypeToggle = dynamic_cast<CMenuToggle*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_TYPE_TOGGLE, true));
	CMenuButton *pNextButton = dynamic_cast<CMenuButton*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_REGISTRATION_NEXT_BUTTON, true));
	
	// SETUP for parsing the arena TYPEs
	const unsigned short BITMASK = 0xFF;
	int iPos = 0; 
	unsigned short iSoloType = 0, iTeamType = 0;
	ArenaInfo *pSolo = NULL, *pTeam = NULL;

	// ERROR checking: Have we gotten all of our menu visuals?
	//	Did we receive a FAIL SIGNAL from the server?
	if (!pParentMenu
	|| !pSoloArenasToggle 
	|| !pTeamArenasToggle 
	|| !pTypeToggle
	|| !pNextButton
	|| (!pSoloArenasToggle->m_clickScript && pTeamArenasToggle->m_clickScript)) 
	{
		// Try to disable the next button if we failed to load any visuals
		if (pNextButton) 
			pNextButton->SetEnabled(false);

		// Try to set a load fail message on the template toggle
		if (pTypeToggle)
		{
			pTypeToggle->SetText("No arenas available.");
			pTypeToggle->m_visible = true;
			pTypeToggle->m_enabled = false;
		}
		return false;
	}

	// CREATE containers for the SOLO and TEAM arena MODEs
	vector<ArenaInfo*> *SoloArenas = NULL; 
	vector<ArenaInfo*> *TeamArenas = NULL; 

	// Parse the arena TYPEs
	for (int i=0; i<(ARENATYPE_MAX - 1); i++)
	{
		unsigned short iSoloType = 0, iTeamType = 0;
		::memcpy(&iSoloType, &pData[iPos], sizeof(short));
		iPos += sizeof(short);
		iTeamType = iSoloType;

		// Decode the arena types
  		iTeamType &= BITMASK;
		iSoloType >>= 8;
		iSoloType &= BITMASK;

		// Store the SOLO arena TYPE, if available
		if (iSoloType != 0)
		{
			if(!SoloArenas) SoloArenas = new vector<ArenaInfo*>();
			pSolo = new ArenaInfo();
			pSolo->Mode = ARENAMODE_SOLO;
			pSolo->Type = static_cast<ARENA_TYPES>(iSoloType);
			SoloArenas->push_back(pSolo);
		}

		// Store the TEAM arena TYPE, if available
		if (iTeamType != 0)
		{
			if(!TeamArenas) TeamArenas = new vector<ArenaInfo*>();
			pTeam = new ArenaInfo();
			pTeam->Mode = ARENAMODE_TEAM;
			pTeam->Type = static_cast<ARENA_TYPES>(iTeamType);
			TeamArenas->push_back(pTeam);
		}
	}

	// Store the data for the arena MODE toggle controls
	pSoloArenasToggle->m_pData = SoloArenas;
	pTeamArenasToggle->m_pData = TeamArenas;

	// UNSET both arena MODE toggle controls
	pSoloArenasToggle->m_isSet = false;
	pTeamArenasToggle->m_isSet = false;

	// Execute a MODE toggle control's click script to init the interface
	if (SoloArenas && pSoloArenasToggle->m_clickScript)
	{
		pSoloArenasToggle->m_isSet = true;
		pSoloArenasToggle->m_clickScript->ExecuteScript();
	}
	else if (TeamArenas && pTeamArenasToggle->m_clickScript)
	{
		pTeamArenasToggle->m_isSet = true;
		pTeamArenasToggle->m_clickScript->ExecuteScript();
	}

	return true;
}

bool
CClientNetworkingLogic::HandleArenaInfoRequestClientSide(BYTE *pData, 
														 int &iMessageSize, 
														 DPID &iFromID)
{
	// SETUP for parsing the message packet
	int iPos = 0; 
	int iNumArenas = 0;

	// GET how many arenas we are processing
	::memcpy(&iNumArenas, pData, sizeof(short));
	iPos += sizeof(short);

	// ERROR checking: Are there no arenas returned?
	//	Did we receive a FAIL SIGNAL from the server?
	if (iNumArenas <= 0)
		return false;

	// SETUP for parsing the arena TYPEs
	vector<ArenaInfoEX*> Arenas;
	ArenaInfoEX *pArenaInfo = NULL;

	// PARSE out all arena info
	for (int i=0; i<iNumArenas; i++)
	{
		// CREATE new ArenaInfoEX
		pArenaInfo = new ArenaInfoEX();

		// Index
		short iIndex = 0;
		::memcpy(&iIndex, &pData[iPos], sizeof(short));
		iPos += sizeof(short);
		pArenaInfo->Index = iIndex;

		// Mode
		::memcpy(&pArenaInfo->Mode, &pData[iPos], sizeof(ARENA_MODES));
		iPos += sizeof(ARENA_MODES);

		// Type
		::memcpy(&pArenaInfo->Type, &pData[iPos], sizeof(ARENA_TYPES));
		iPos += sizeof(ARENA_TYPES);

		// Name length
		int iLength = 0;
		::memcpy(&iLength, &pData[iPos], sizeof(int));
		iPos += sizeof(int);

		// Name
		TCHAR *strName = new TCHAR[iLength + 1];
		::memcpy(strName, &pData[iPos], (sizeof(TCHAR) * iLength));
		strName[iLength] = '\0';
		pArenaInfo->Name = strName;
		iPos += (sizeof(TCHAR) * iLength);
		SafeDeleteArray(strName);

		// World name length
		iLength = 0;
		::memcpy(&iLength, &pData[iPos], sizeof(int));
		iPos += sizeof(int);

		// World name
		strName = new TCHAR[iLength + 1];
		::memcpy(strName, &pData[iPos], (sizeof(TCHAR) * iLength));
		strName[iLength] = '\0';
		pArenaInfo->WorldName = strName;
		iPos += (sizeof(TCHAR) * iLength);
		SafeDeleteArray(strName);

		// Min players
		::memcpy(&pArenaInfo->NumMinPlayers, &pData[iPos], sizeof(short));
		iPos += sizeof(short);

		// Max players
		::memcpy(&pArenaInfo->NumMaxPlayers, &pData[iPos], sizeof(short));
		iPos += sizeof(short);

		// Current players
		::memcpy(&pArenaInfo->NumCurrentPlayers, &pData[iPos], sizeof(short));
		iPos += sizeof(short);

		// Time limit
		::memcpy(&pArenaInfo->TimeLimit, &pData[iPos], sizeof(int));
		iPos += sizeof(int);

		// Arena begin countdown timestamp
		::memcpy(&pArenaInfo->TimeRemainingForAutostart, &pData[iPos], sizeof(int));
		iPos += sizeof(int);

		// Current arena bet pool
		int iBetPool = 0;
		::memcpy(&iBetPool, &pData[iPos], sizeof(int));
		iPos += sizeof(int);
		if (iBetPool <= 0) pArenaInfo->BetPool = 0;
		else pArenaInfo->BetPool = iBetPool;

		// Num teams
		::memcpy(&pArenaInfo->NumTeams, &pData[iPos], sizeof(char));
		iPos += sizeof(char);

		// Num players per team
		if (pArenaInfo->NumTeams > 0)
		{
			pArenaInfo->NumPlayersOnTeam = new unsigned short[pArenaInfo->NumTeams];
			for (int i=0; i<pArenaInfo->NumTeams; i++)
			{
				::memcpy(&pArenaInfo->NumPlayersOnTeam[i], &pData[iPos], sizeof(short));
				iPos += sizeof(short);
			}
		} // END num players per team

		// ADD ArenaInfoEX to collection
		Arenas.push_back(pArenaInfo);
	} // END PARSE LOOP

	// GET references to menu visuals
	CMenuListBox *pListbox = dynamic_cast<CMenuListBox*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_MAPS_LIST, true));
	CMenuText *pFilterText = dynamic_cast<CMenuText*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_FILTER_TEXT, true));
	
	// ERROR checking: Did we get our references to the menu visuals?
	if (!pFilterText
	|| !pListbox
	|| !pListbox->m_selectionChangeScript
	|| Arenas.empty())//empty list also crashed on Arenas[0] below
	{
		for (size_t a = 0; a < Arenas.size(); a++)
			delete Arenas[a];//were leaked
		return false;
	}

	// SETUP for the filter text display
	CString strFilterText;
	ARENA_MODES eMode = Arenas[0]->Mode;
	ARENA_TYPES eType = Arenas[0]->Type;

	// Determine MODE
	if (eMode == ARENAMODE_SOLO)
		strFilterText = "Solo Arenas > ";
	else
		strFilterText = "Team Arenas > ";

	// Determine TYPE
	strFilterText += ARENA_TYPES_MAP[eType];

	// SET the filter text
	pFilterText->SetText(strFilterText.GetBuffer());
	strFilterText.ReleaseBuffer();

	// CLEAR the listbox of all entries
	pListbox->ClearList();

	// INIT the listbox with all the received arena info data
	CMenuListItem *pItem = NULL;
	CMenuText *pText = NULL;
	vector<ArenaInfoEX*>::iterator iter;
	LPTSTR strName = NULL;
	for (iter = Arenas.begin(); iter != Arenas.end(); iter++)
	{
		// CREATE new list item
		pItem = new CMenuListItem();
		pText = new CMenuText();

		// CONFIGURE the text display for line wrap
		pText->m_width = 150;
		pText->m_multilined = true;
		
		// SET display text
		pText->SetText((*iter)->Name.GetBuffer());
		(*iter)->Name.ReleaseBuffer();
		pItem->m_textList.AddTail(pText);

		// SET the list item's data
		pItem->m_pData = (void*)(*iter);
		
		// ADD the list item to the listbox
		pListbox->AddItem(pItem);
	}

	// INIT the listbox by selecting the first item
	if (pListbox->m_pFirstItem)
		pListbox->m_currentSelection = pListbox->m_pFirstItem;

	// Execute the listbox selection change script
	// to init the arena reg'n menu window
	pListbox->m_selectionChangeScript->ExecuteScript();
	return true;
}

bool
CClientNetworkingLogic::HandleArenaJoinRequestClientSide(BYTE *pData, 
														 int &iMessageSize, 
														 DPID &iFromID)
{
	int iPos = 0;
	ArenaResult result;
	result.Severity = AS_FAIL;
	result.Result = AR_JOIN_FAILED;

	// GET the client's entity
	POSITION pos = this->m_gStatus->m_entObjRuntime->GetHeadPosition();
	if (!pos) return false;
	CMovementObj *pEntity = (CMovementObj*)this->m_gStatus->m_entObjRuntime->GetNext(pos);
	if (!pEntity) return false;
	
	// SET joined arena flag on the player's entity
	pEntity->m_hasJoinedArena = true;

	// GET the unique ID of the arena bot the player registered with
	::memcpy(&pEntity->m_arenaBotID, &pData[iPos], sizeof(int));
	iPos += sizeof(int);

	// DISPLAY success message to the player
	this->ShowArenaRegistrationMessageBox("You have successfully registered for the arena battle!");

	// ENABLE the arena LEAVE button
	CMenuButton *pLeaveButton = dynamic_cast<CMenuButton*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_LEAVE_BUTTON, true));
	if (pLeaveButton) pLeaveButton->SetEnabled();

	// DISABLE the JOIN button
	CMenuButton *pJoinButton = dynamic_cast<CMenuButton*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_JOIN_BUTTON, true));
	if (pJoinButton) pJoinButton->SetEnabled(false);

	// REFRESH the arena info
	CMenuButton *pRefreshButton = dynamic_cast<CMenuButton*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_REFRESH_BUTTON, true));
	if (pRefreshButton && pRefreshButton->m_clickScript) 
		pRefreshButton->m_clickScript->ExecuteScript();
	return true;
}

bool
CClientNetworkingLogic::HandleArenaLeaveRequestClientSide(BYTE *pData, 
														  int &iMessageSize, 
														  DPID &iFromID)
{
	int iPos = 0;
	ArenaResult result;
	result.Severity = AS_FAIL;
	result.Result = AR_LEAVE_FAILED;

	// GET the client's entity
	POSITION pos = this->m_gStatus->m_entObjRuntime->GetHeadPosition();
	if (!pos) return false;
	CMovementObj *pEntity = (CMovementObj*)this->m_gStatus->m_entObjRuntime->GetNext(pos);
	if (!pEntity) return false;
	
	// SET joined arena flag on the player's entity
	pEntity->m_hasJoinedArena = false;

	// RESET arena bot ID on the player
	pEntity->m_arenaBotID = -1;

	// DISPLAY success message to the player
	this->ShowArenaRegistrationMessageBox("You are now unregistered from your arena battle.");

	// DISABLE the arena LEAVE button
	CMenuButton *pLeaveButton = dynamic_cast<CMenuButton*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_LEAVE_BUTTON, true));
	if (pLeaveButton) pLeaveButton->SetEnabled(false);

	// ENABLE the JOIN button
	CMenuButton *pJoinButton = dynamic_cast<CMenuButton*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_JOIN_BUTTON, true));
	if (pJoinButton) pJoinButton->SetEnabled();

	// REFRESH the arena info
	CMenuButton *pRefreshButton = dynamic_cast<CMenuButton*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_REFRESH_BUTTON, true));
	if (pRefreshButton && pRefreshButton->m_clickScript) 
		pRefreshButton->m_clickScript->ExecuteScript();
	return true;
}

void 
CClientNetworkingLogic::HandleArenaFailedResponse(QUEST_PACKETS eType, ArenaResult Result)
{
	switch (eType)
	{
		case ARENA_TYPE_ID:
		{
			// GET references to menu visuals
			CMenuWindow *pParentMenu = dynamic_cast<CMenuWindow*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_ARENA_REGISTRATION_01_MENU, true));
			CMenuToggle *pSoloArenasToggle = dynamic_cast<CMenuToggle*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_MODE_SOLO_TOGGLE, true));
			CMenuToggle *pTeamArenasToggle = dynamic_cast<CMenuToggle*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_MODE_TEAM_TOGGLE, true));
			CMenuToggle *pTypeToggle = dynamic_cast<CMenuToggle*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_TYPE_TOGGLE, true));
			CMenuButton *pNextButton = dynamic_cast<CMenuButton*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_REGISTRATION_NEXT_BUTTON, true));
			CMenuText *pArenaDescriptionText = dynamic_cast<CMenuText*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_DESCRIPTION_TEXT, true));

			// TRY TO REMOVE all arena TYPE toggle controls
			if (pParentMenu && pParentMenu->m_childList)
			{
				// LOOP through all the child controls of the open window
				POSITION posCurrent = NULL, posPrevious = NULL;
				CMenuToggle *pToggle = NULL;
				for (posCurrent = pParentMenu->m_childList->GetHeadPosition(); posCurrent != NULL;)
				{					
					// Remove all toggle controls whose ID is > the template toggle control's ID
					posPrevious = posCurrent;
					pToggle = dynamic_cast<CMenuToggle*>(pParentMenu->m_childList->GetNext(posCurrent));
					if (pToggle 
					&& (pToggle->m_visualID != ARENA_MODE_SOLO_TOGGLE)
					&& (pToggle->m_visualID != ARENA_MODE_TEAM_TOGGLE)
					&& (pToggle->m_visualID != ARENA_TYPE_TOGGLE))
					{
						pParentMenu->m_childList->RemoveAt(posPrevious);
						SafeDelete(pToggle);
					}
				} // END FOR
			} // END IF

			// TRY DISABLE the next button
			if (pNextButton) 
				pNextButton->SetEnabled(false);

			// TRY TO SET a default arena description message
			if (pArenaDescriptionText)
				pArenaDescriptionText->SetText(ARENA_TYPES_DESCRIPTIONS_MAP[ARENATYPE_UNSPECIFIED]);

			// TRY TO SET a load fail message on the template toggle
			if (pTypeToggle)
			{
				pTypeToggle->SetText(ARENA_TYPES_MAP[ARENATYPE_UNSPECIFIED]);
				pTypeToggle->m_visible = true;
				pTypeToggle->m_enabled = false;
			}
			break;
		}
		case ARENA_INFO_ID:
		{
			// GET references to menu visuals
			CMenuWindow *pWindow = dynamic_cast<CMenuWindow*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ID_ARENA_REGISTRATION_02_MENU, true));
			CMenuListBox *pListbox = dynamic_cast<CMenuListBox*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_MAPS_LIST, true));
			CMenuButton *pJoinButton = dynamic_cast<CMenuButton*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_JOIN_BUTTON, true));
			CMenuText *pCountdownText = dynamic_cast<CMenuText*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_AUTOSTART_COUNTDOWN_TEXT, true));
			CMenuText *pMinmaxSizeText = dynamic_cast<CMenuText*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_MINMAX_SIZE_TEXT, true));
			CMenuText *pCurrentSizeText = dynamic_cast<CMenuText*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_CURRENT_SIZE_TEXT, true));

			// TRY TO DISABLE the JOIN ARENA button
			if (pJoinButton) 
				pJoinButton->SetEnabled(false);

			// TRY TO CLEAR the listbox of all content
			if (pListbox)
				pListbox->ClearList();

			// TRY TO INIT all text visuals
			if (pCountdownText)
				pCountdownText->SetText("00:00");

			if (pMinmaxSizeText)
				pMinmaxSizeText->SetText("0/0");

			if (pCurrentSizeText)
				pCurrentSizeText->SetText("0");
			break;
		}
		case ARENA_JOIN_ID: break;
		default: break;
	}

	if (Result.Result & AR_JOIN_FAILED)
		this->ShowArenaRegistrationMessageBox("You are unable to join the arena. Please try again in a minute.");
	if (Result.Result & AR_LEAVE_FAILED)
		this->ShowArenaRegistrationMessageBox("You are unable to leave the arena. Please try again in a minute.");
	if (Result.Result & AR_COULD_NOT_FIND_ARENA_BOT)
		this->ShowArenaRegistrationMessageBox("There aren't any arena bots in the vicinity.");
	if (Result.Result & AR_INVALID_PLAYER_OR_ACCOUNT)
		this->ShowArenaRegistrationMessageBox("Invalid player character or player account.");
	if (Result.Result & AR_INSUFFICIENT_FUNDS_FOR_BETTING)
		this->ShowArenaRegistrationMessageBox("You don't have enough credits to place a bet in this arena!");
}

void 
CClientNetworkingLogic::ShowArenaRegistrationMessageBox(const char *pMessage)
{
	// OPEN the arena registration message box
	if (!this->m_gStatus->OpenMenu(ARENA_REGISTRATION_MESSAGE_BOX, true))
		return;

	// GET references to menu visuals
	CMenuText *pMessageText = dynamic_cast<CMenuText*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_REGISTRATION_MESSAGE_TEXT, true));

	// ERROR checking: Did we get the references to the menu visuals?
	if (!pMessageText)
		return;

	// SET the message box display text
	pMessageText->SetText(pMessage);
}

bool
CClientNetworkingLogic::UpdateArenaStatusMenu(ARENA_STATUS eStatus)
{
	// GET a reference to the menu visual
	CMenuText *pArenaStatusText = dynamic_cast<CMenuText*>(this->m_gStatus->m_runtimeMenuDatabase->GetVisualByID(ARENA_STATUS_TEXT, true));
	if (!pArenaStatusText) return false;

	// SET the menu text color based on the current status
	switch (eStatus)
	{
		case ARENASTATUS_NONE:
		{
			// SET text color to WHITE
			pArenaStatusText->m_red = 1.0f;
			pArenaStatusText->m_green = 1.0f;
			pArenaStatusText->m_blue = 1.0f;

			// SET the text string
			pArenaStatusText->SetText("Arena Status: None");
			break;
		}
		case ARENASTATUS_COMMITTED:
		{
			// SET text color to YELLOW
			pArenaStatusText->m_red = 1.0f;
			pArenaStatusText->m_green = 1.0f;
			pArenaStatusText->m_blue = 0.0f;

			// SET the text string
			pArenaStatusText->SetText("Arena Status: Committed");
			break;
		}
		case ARENASTATUS_ENGAGED:
		{
			// SET text color to RED
			pArenaStatusText->m_red = 1.0f;
			pArenaStatusText->m_green = 0.0f;
			pArenaStatusText->m_blue = 0.0f;

			// SET the text string
			pArenaStatusText->SetText("Arena Status: Engaged");
			break;
		}
	}
	return true;
}

bool 
CClientNetworkingLogic::HandleJournalRequestClientSide(BLOCKQUEST_DATA *pHeader,
													   BYTE *pData, 
													   int &iMessageSize, 
													   DPID &iFromID)
{
	// HANDLE list-based progression quests and quest journal
	CMenuListBox *pListBox = NULL;
	int iListBoxID = 0;
	pListBox = dynamic_cast<CMenuListBox*>(m_gStatus->m_runtimeMenuDatabase->GetVisualByID(abs(pHeader->m_itemAssignedID), true));
	iListBoxID = abs(pHeader->m_itemAssignedID);

	// ERROR checking: Did we get a valid reference to a listbox?
	if(!pListBox)
		return false;

	unsigned int iPos = 0, iNumEntries = 0;
	unsigned char iLength = 0;

	// PARSE num of journal entries to process
	::memcpy(&iNumEntries, &pData[iPos], sizeof(char));
	iPos += sizeof(char);
	if (iNumEntries <= 0)
		return false;

	// CREATE the collection to store all journal entries
	if (this->m_gStatus->m_multiplayerObject->m_journalEntries)
	{
		this->m_gStatus->m_multiplayerObject->m_journalEntries->SafeDelete();
		::SafeDelete(this->m_gStatus->m_multiplayerObject->m_journalEntries);
	}
	this->m_gStatus->m_multiplayerObject->m_journalEntries = new CQuestJournalList();

	for (int i=0; i<iNumEntries; i++)
	{
		// CREATE a new journal entry
		CQuestJournalObj *pJournalEntry = new CQuestJournalObj();

		// PROCESS a single journal entry
		this->ProcessJournalEntry(pData, iPos, pJournalEntry);

		// ADD the journal entry to the multiplayer object
		m_gStatus->m_multiplayerObject->m_recentQuestPtr = pJournalEntry;
		m_gStatus->m_multiplayerObject->m_journalEntries->AddQuestJournalEntry(pJournalEntry, CQuestJournalList::QJ_TAIL);
	}	

	this->m_gStatus->RefreshQuestJournal(ID_QUEST_JOURNAL_MAIN_LIST, true);
	return true;
}

BOOL 
CClientNetworkingLogic::ProcessJournalEntry(BYTE *pData, 
											unsigned int &iPos, 
											CQuestJournalObj *pJournalEntry)
{
	unsigned int iLength = 0;
	if (!pJournalEntry)
		pJournalEntry = new CQuestJournalObj();

	// PARSE quest description length
	::memcpy(&iLength, &pData[iPos], sizeof(char));
	iPos += sizeof(char);

	// PARSE quest description
	if (iLength > 0)
	{
		char *strDescription = new char[iLength+1];
		::memcpy(strDescription, &pData[iPos], (sizeof(char) * iLength));
		strDescription[iLength] = '\0';
		iPos += (sizeof(char) * iLength);

		// SET the journal description value
		pJournalEntry->m_questDescription = strDescription;

		delete [] strDescription;
		strDescription = 0;
	}
	else pJournalEntry->m_questDescription = "";

	// PARSE quest NPC name length
	::memcpy(&iLength, &pData[iPos], sizeof(char));
	iPos += sizeof(char);

	// PARSE quest NPC name
	if (iLength > 0)
	{
		char *strNpcName = new char[iLength+1];
		::memcpy(strNpcName, &pData[iPos], (sizeof(char) * iLength));
		strNpcName[iLength] = '\0';
		iPos += (sizeof(char) * iLength);

		// SET the journal NPC name value
		pJournalEntry->m_npcName = strNpcName;

		delete [] strNpcName;
		strNpcName = 0;
	}
	else pJournalEntry->m_npcName = "";

	// PARSE quest key statment
	::memcpy(&iLength, &pData[iPos], sizeof(char));
	iPos += sizeof(char);

	// PARSE quest key statement
	if (iLength > 0)
	{
		char *strKeyStatement = new char[iLength+1];
		::memcpy(strKeyStatement, &pData[iPos], (sizeof(char) * iLength));
		strKeyStatement[iLength] = '\0';
		iPos += (sizeof(char) * iLength);

		// SET the journal NPC name value
		pJournalEntry->m_keyStatement = strKeyStatement;
		delete [] strKeyStatement;
		strKeyStatement = 0;
	}
	else pJournalEntry->m_keyStatement = "";

	// PARSE quest required items count
	::memcpy(&iLength, &pData[iPos], sizeof(char));
	iPos += sizeof(char);

	// PARSE quest required items
	if (iLength > 0)
	{
		int iCount = 0, iNameLength = 0;
		for (int j=0; j<iLength; j++)
		{
			// PARSE quest required item name length
			::memcpy(&iNameLength, &pData[iPos], sizeof(char));
			iPos += sizeof(char);

			if (iNameLength > 0)
			{
				// PARSE quest required item name
				CQJRequiredItem *pRequiredItem = new CQJRequiredItem();
				char *strItemName = new char[iNameLength+1];
				::memcpy(strItemName, &pData[iPos], (sizeof(char) * iNameLength));
				strItemName[iNameLength] = '\0';
				iPos += (sizeof(char) * iNameLength);

				// PARSE quantity of quest required item
				::memcpy(&iNameLength, &pData[iPos], sizeof(char));
				iPos += sizeof(char);
				pRequiredItem->m_quanity = iNameLength;

				// ADD quest required item
				pRequiredItem->m_itemName = strItemName;
				if (!pJournalEntry->m_requiredItems)
					pJournalEntry->m_requiredItems = new CQJRequiredItemList();
				pJournalEntry->m_requiredItems->AddTail(pRequiredItem);

				delete [] strItemName;
				strItemName = 0;
			}
		}
	}

	// PARSE quest status
	::memcpy(&iLength, &pData[iPos], sizeof(char));
	iPos += sizeof(char);
	pJournalEntry->m_status = iLength;
	return TRUE;
}
