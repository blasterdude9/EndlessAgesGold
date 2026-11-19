#include "StdAfx.h"
#include "textsystems.h"
#include <vector>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTextSystems::CTextSystems(CSharedNet * sharedInterface,CQuestSystems * questSys)
{
	 m_sharedInterfaceRef			= sharedInterface;
	 m_questInterface				= questSys;
	 this->m_pNetworkCommands		= new CNetworkCommands();
}

CTextSystems::~CTextSystems(void)
{
	if(this->m_pNetworkCommands) delete this->m_pNetworkCommands;
	this->m_pNetworkCommands = 0;
}


bool CTextSystems::GetCommand(CString & fullMsg, CString & commandReturn, CString & extendedText)
{
	long length = fullMsg.GetLength();
    if(length < 1)
	{
	   extendedText = fullMsg;
	   return false;
	}

	if(fullMsg.GetAt(0) != ':' && fullMsg.GetAt(0) != '/')
	{
	   extendedText = fullMsg;
	   return false;
	}
	extendedText = "";

	char * arrayBufTemp = fullMsg.GetBuffer();
	bool switchTemp = false;
    for(int i=1;i<length;i++)
	{
		if(!switchTemp)
		if(arrayBufTemp[i] == ' ')
		{
		   switchTemp = true;
		   continue;
		}

		if(!switchTemp)
           commandReturn += arrayBufTemp[i];
		else
		   extendedText += arrayBufTemp[i];
	}
	//extendedText = fullMsg;
	//extendedText.Replace(commandReturn,"");
	//extendedText.Replace("/","");
	//extendedText.Replace(":","");
    
	/*if(extendedText.GetLength() > 0)
	if(extendedText.GetAt(0) == ' ')
	{
       extendedText.Delete(0,1);
	}*/

	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CTextSystems
// Function:	HandleTextMessageServerSide(LPVOID, int&, DPID&)
// Description:	Process command messages submitted by players in text chat.
///////////////////////////////////////////////////////////////////////////////

BOOL
CTextSystems::HandleTextMessageServerSide_safe(LPVOID pMessage, int &iMsgSize, DPID &iFromLocID)
{
	int iStringLength = iMsgSize - 4;
	CMDID iCommandID = 0;
	CString	cnvs = 0,
			commandReturn = 0,
			strArgs = 0;

	if(iMsgSize < 5)
	{
        //has to be long enough to assume this , iCommandID = (pDataBlock[iBaseLoc]); 
		m_sharedInterfaceRef->m_eventLog->LogByPriority(
			"Outside memory write or zero write (HandleTextMessageServerSide): iMsgSize < 5", 2);
		return FALSE;
	}

	// Error checking
	if((iStringLength <= 0) || (iStringLength > 1000)) 
		return FALSE;

	// Get text message packet header info
	BYTE *pDataBlock = (BYTE*)pMessage;
	LPTXT_MSG pCastEntry = (LPTXT_MSG)pMessage;
	short iNetworkDefinedID = pCastEntry->referenceID;
	signed char iCategory = pCastEntry->m_catagorie;

	// Get player data
	CPlayerObject *playerObjectData = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(iNetworkDefinedID);
    if(!playerObjectData)
		return FALSE;
   
	// More error checking...
	if( (playerObjectData->m_energy <= 0) || (iStringLength >= 150))
	{
		m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
		return TRUE;
	}

	// Disassemble the text message packet
	const int iBaseLoc = 4;
	int iLastPos = iStringLength - sizeof(CMDID);
	if (iLastPos > 0)
	{
		CopyMemory(m_bufferTemp, &pDataBlock[iBaseLoc + sizeof(CMDID)], iLastPos); // Decode command arguments/chat-string
		m_bufferTemp[iLastPos] = '\0'; // Null-terminate argument string
		cnvs = playerObjectData->m_charName + "> ";
	    strArgs = m_bufferTemp;
	    cnvs = cnvs + m_bufferTemp;
	}

	// Decode command ID
	iCommandID = (pDataBlock[iBaseLoc]); 

	//m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,"CommandID:"+m_sharedInterfaceRef->m_cabLib.ConvToString((int)iCommandID),playerObjectData);

	// Execute the appropriate server-side command!
	switch(iCommandID)
	{
		default:
		case SCMD_UNKNOWN: // Treat all unknowns as regular chat string
		{
			//"!" GM commands (!broadcast, !shutdown ...) - admin/GM only, not shown as chat
			if((playerObjectData->m_admin || playerObjectData->m_gm) &&
			   m_sharedInterfaceRef->HandleGMChatCommand(playerObjectData, strArgs))
				break;

			if(!playerObjectData->m_mute)
			{
				m_sharedInterfaceRef->BroadcastMessage(0,
					cnvs,
					playerObjectData->m_3dPosition,
					playerObjectData->m_currentWorldID,
					playerObjectData->m_networkAssignedID);
			}
			else if(playerObjectData->m_deafMuteNotify)
			{
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
					playerObjectData->m_currentID,
					"Sorry but this feature has been disabled on your account.",
					playerObjectData->m_networkAssignedID,
					playerObjectData,
					0);
			}
			else // Passive
			{
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
					playerObjectData->m_currentID,
					cnvs,
					playerObjectData->m_networkAssignedID,
					playerObjectData,
					0);
			}
			break;
		} // End SCMD_UNKNOWN
		case SCMD_WHISPER:
		{
			CString finalResTxt = playerObjectData->m_charName + " whispers: " + strArgs;
			if(!playerObjectData->m_mute)
			{
				m_sharedInterfaceRef->BroadcastMessage(15,
					finalResTxt,
					playerObjectData->m_3dPosition,
					playerObjectData->m_currentWorldID,
					playerObjectData->m_networkAssignedID);
			}
			else if(playerObjectData->m_deafMuteNotify)
			{
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
					playerObjectData->m_currentID,
					"Sorry but this feature has been disabled on your account.",
					playerObjectData->m_networkAssignedID,
					playerObjectData,
					0);
			}
			else // Passive
			{
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
					playerObjectData->m_currentID,
					finalResTxt,
					playerObjectData->m_networkAssignedID,
					playerObjectData,
					0);
			}
			break;
		} // End SCMD_WHISPER
		case SCMD_TALK:
		{
			if(!playerObjectData->m_mute)
			{
				m_sharedInterfaceRef->BroadcastMessage(500,
					cnvs,
					playerObjectData->m_3dPosition,
					playerObjectData->m_currentWorldID,
					playerObjectData->m_networkAssignedID);
			}
			else if(playerObjectData->m_deafMuteNotify)
			{
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
					playerObjectData->m_currentID,
					"Sorry this feature has been disabled on your account.",
					playerObjectData->m_networkAssignedID,
					playerObjectData,
					0);
			}
			else // Passive
			{
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
					playerObjectData->m_currentID,
					cnvs,
					playerObjectData->m_networkAssignedID,
					playerObjectData,
					0);
			}
			break;
		} // End SCMD_TALK
		case SCMD_T: // TELL text message
		{
			if(playerObjectData->m_mute) break;
			bool aquiredName = false;
			CString targetName, finalMsg;

			int extendedLength = strArgs.GetLength();
			if(extendedLength > 2)
			{
				// Figure out who this is directed to
				char *bufXTemp = strArgs.GetBuffer();
				for(int tellLoop=0; tellLoop < extendedLength; tellLoop++)
				{
					if(bufXTemp[tellLoop] == ':')
					{
						if(targetName.GetLength() < 2) break;
						else aquiredName = true;
					}

					if(aquiredName == false)
					   targetName += bufXTemp[tellLoop];
					else
					   finalMsg += bufXTemp[tellLoop];
				}
				if(finalMsg.GetLength() < 1) aquiredName = false;
			} // end if valid size

			// Send chat string to recipient
			if(aquiredName)
			{
				finalMsg = playerObjectData->m_charName + "(private /t)>" + finalMsg;
				if(!m_sharedInterfaceRef->DirectMessage(0,
														finalMsg,
														playerObjectData->m_3dPosition,
														playerObjectData->m_currentWorldID,
														playerObjectData->m_networkAssignedID,
														targetName))
				{
					// Cannot find player
					m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
						playerObjectData->m_currentID,
						"Cannot find target player:" + targetName,
						playerObjectData->m_networkAssignedID,
						playerObjectData,
						0);
				}
				else
				{
					// All went well
					m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
						playerObjectData->m_currentID,
						finalMsg,
						playerObjectData->m_networkAssignedID,
						playerObjectData,
						0);
				}
			} // End send it
			else
			{
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
					playerObjectData->m_currentID,
					"Cannot find target player!",
					playerObjectData->m_networkAssignedID,
					playerObjectData,
					0);
			}
			break;
		} // End SCMD_T
		case SCMD_BUY: // Search for store zones
		{
			m_sharedInterfaceRef->m_eventLog->LogByPriority("Called SCMD_BUY",3);
			
			if(playerObjectData->m_stencilChannel < 0) // Cannot find stencil
				m_sharedInterfaceRef->m_eventLog->LogByPriority("Cannot find establishment-store playerObjectData->m_stencilChannel HandleTextMessageServerSide",2);

/*			CCommerceObj * cmPtr = m_sharedInterfaceRef->m_commerceDB->GetEstablishmentByPosition(playerObjectData->m_3dPosition,playerObjectData->m_stencilChannel);
			if(cmPtr)
			{// Found store zone
				m_sharedInterfaceRef->m_eventLog->LogByPriority("SCMD_BUY :: SendStoreData()",2);
				m_sharedInterfaceRef->SendStoreData(cmPtr->m_uniqueID,playerObjectData);
			}
*/	
			break;
		} // End SCMD_BUY
		case SCMD_MANAGE:
		{
			
			break;
		} // End SCMD_MANAGE
		case SCMD_GAG:
		{
			if(playerObjectData->m_admin == TRUE || playerObjectData->m_gm == TRUE)
			{
				CPlayerObject * lstObj = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByName(strArgs.GetBuffer());
				BOOL foundHim = FALSE;
				if( lstObj
					&& (lstObj->m_energy > 0)
					&& (!lstObj->m_aiControlled))
				{
					if(lstObj->m_playerOrigRef) 
					{
						if(!lstObj->m_playerOrigRef->m_mute)
							lstObj->m_playerOrigRef->m_mute = TRUE;
						else
							lstObj->m_playerOrigRef->m_mute = FALSE;
						
						if(!lstObj->m_mute)
							lstObj->m_mute = TRUE;
						else
							lstObj->m_mute = FALSE;

						foundHim = TRUE;
						
						if(lstObj->m_mute)
							m_sharedInterfaceRef->SendTextMsg(	DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
																playerObjectData->m_currentID,
																"GAGGED " + strArgs,
																playerObjectData->m_networkAssignedID,
																playerObjectData,
																0);
						else
							m_sharedInterfaceRef->SendTextMsg(	DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
																playerObjectData->m_currentID,
																"UNGAGGED " + strArgs,
																playerObjectData->m_networkAssignedID,
																playerObjectData,
																0);
					}
				}

				if(!foundHim)
				{
					m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
						playerObjectData->m_currentID,
						"CANNOT FIND "+strArgs,
						playerObjectData->m_networkAssignedID,
						playerObjectData,
						0);
				}
			}
			break;
		} // End SCMD_GAG
		case SCMD_VUP:
		{
			if(playerObjectData->m_admin)
			{
				m_sharedInterfaceRef->m_netComObject->m_vicinityTolerance = m_sharedInterfaceRef->m_netComObject->m_vicinityTolerance + 10;
				CString buildString = "VICINITY:" + m_sharedInterfaceRef->m_cabLib.ConvToString(m_sharedInterfaceRef->m_netComObject->m_vicinityTolerance);
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
					playerObjectData->m_currentID, // can be NULL if client
					buildString,
					playerObjectData->m_networkAssignedID,
					playerObjectData,
					0);
			}
			break;
		} // End SCMD_VUP
		case SCMD_VDOWN:
		{
			if(playerObjectData->m_admin)
			{
				m_sharedInterfaceRef->m_netComObject->m_vicinityTolerance = m_sharedInterfaceRef->m_netComObject->m_vicinityTolerance - 10;
				if(m_sharedInterfaceRef->m_netComObject->m_vicinityTolerance <= 0)
					m_sharedInterfaceRef->m_netComObject->m_vicinityTolerance = 10;

				CString buildString = "VICINITY:" + m_sharedInterfaceRef->m_cabLib.ConvToString(m_sharedInterfaceRef->m_netComObject->m_vicinityTolerance);
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
					playerObjectData->m_currentID, // Can be NULL if client
					buildString,
					playerObjectData->m_networkAssignedID,
					playerObjectData,
					0);
			}
			break;
		} // End SCMD_VDOWN
		case SCMD_ARENASTATS:
		{
			
			break;
		} // End SCMD_ARENASTATS
		case SCMD_ROLL: // Random number generator (0-100) at client's request
		{
			int rollResult = rand() % 100;
			CString randomBuild = m_sharedInterfaceRef->m_cabLib.ConvToString(rollResult);

			// If in group, broadcast to members
			if(playerObjectData->m_groupID > 0)
			{
				//Lock exists on group id channel only dont worry about other channel locks
				CAxList * obj = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetAllOnIndex(playerObjectData->m_groupID,"group_id");
				for(CAxLinkObj *pos = obj->GetHeadPosition_unsafe(); pos != NULL;)
				{
					CPlayerObject *plObj = (CPlayerObject*)obj->GetNext_unsafe(&pos);
					if( (plObj->m_handle != playerObjectData->m_handle) // don't send to self
						&& (plObj->m_groupID == playerObjectData->m_groupID)) // found match					
					{
						m_sharedInterfaceRef->SendTextMsg(	DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
							plObj->m_currentID, // can be NULL if client
							plObj->m_charName + " rolled a: " + randomBuild,
							plObj->m_networkAssignedID,
							plObj,
							0);
					}
				}
			}

			// Always replicate back to your self 
			m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
				playerObjectData->m_currentID, // can be NULL if client
				"You roll a: " + randomBuild,
				playerObjectData->m_networkAssignedID,
				playerObjectData,
				0);
			break;
		} // End SCMD_ROLL
/*		case SCMD_REMOVEFROMCLANHOUSE:
		{
			break;
		} // End SCMD_REMOVEFROMCLANHOUSE
*/		case SCMD_WHERE: // Display player's location
		{
			CString Location = playerObjectData->m_currentWorldMap + " (";
			Location += m_sharedInterfaceRef->m_cabLib.FloatToStingCnv(playerObjectData->m_3dPosition.x);
			Location += ", " + m_sharedInterfaceRef->m_cabLib.FloatToStingCnv(playerObjectData->m_3dPosition.y);
			Location += ", " + m_sharedInterfaceRef->m_cabLib.FloatToStingCnv(playerObjectData->m_3dPosition.z);
			Location += ")";

			m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,Location,playerObjectData->m_networkAssignedID,playerObjectData,0);
			break;
		} // End SCMD_WHERE
		case SCMD_LOCKHOUSE:
		case SCMD_UNLOCKHOUSE:
		case SCMD_REMOVEFROMCLANHOUSE: // /removehouse - take down your own house
		{
			CHousingZoneDB * zones = m_sharedInterfaceRef->m_netComObject->m_houseingDB;
			CHPlacementObj * myHouse = zones ? zones->GetHouseByAccountBind(playerObjectData->m_accountNumberRef) : NULL;
			CString houseMsg;
			if(!myHouse)
				houseMsg = "You don't own a house.";
			else if(iCommandID == SCMD_LOCKHOUSE)
			{
				myHouse->m_open = 0;
				houseMsg = "Your house is locked.";
			}
			else if(iCommandID == SCMD_UNLOCKHOUSE)
			{
				myHouse->m_open = 1;
				houseMsg = "Your house is unlocked.";
			}
			else
			{
				zones->RemoveHouseByAccountBind(playerObjectData->m_accountNumberRef);
				m_sharedInterfaceRef->m_threadedDataProcessor->AddToQue(0,"",playerObjectData->m_accountNumberRef,0,14,0,0);//delete from GM_SERVERPLACEMENTS
				houseMsg = "Your house has been taken down. You can place a new one.";
			}
			m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,houseMsg,playerObjectData->m_networkAssignedID,playerObjectData,0);
			break;
		} // End SCMD_LOCKHOUSE / UNLOCKHOUSE / REMOVEHOUSE
		case SCMD_GROUPSHARINGON:
		{
			// DEPRECATED
			//m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,"SCMD_GROUPSHARINGON",playerObjectData);
			break;
		} // End SCMD_CLANSHARINGON
		case SCMD_GROUPSHARINGOFF:
		{
			//m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,"SCMD_GROUPSHARINGOFF",playerObjectData);
			break;
		} // End SCMD_GROUPSHARINGOFF
		case SCMD_REBIRTH: // Reassign respawn point if in safe zone
		{
			m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();//have to unlock here for order
			CAccountObject * acPtrTemp = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountByID_LOS(iFromLocID);
			playerObjectData = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(iNetworkDefinedID);//revalidate
			if(!playerObjectData)
				m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Lock();//have to unlock here for order
			else if(acPtrTemp)
			{
				if(playerObjectData->m_inSafeZone)
					m_sharedInterfaceRef->HandleReassignRespawn_requiresAcLock(playerObjectData,iFromLocID);
			}
			if(acPtrTemp)
			   m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			break;
		} // End SCMD_REBIRTH
		case SCMD_TRADE: // Request trade with another player
		{
			CPlayerObject * pObj = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByName(strArgs.GetBuffer());
			if(!pObj)
			{//cannot find player
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Cannot find player:"+strArgs,0,playerObjectData,0);
				break;
			}
			if(pObj->m_handle == playerObjectData->m_handle)
			{//cannot trade with yourself
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Cannot trade with yourself:"+strArgs,0,playerObjectData,0);
                break;
			}
			if(pObj->m_tradeTargetID != -1)
			{
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"This person is already involved in a trade:"+strArgs,0,playerObjectData,0);
                break;
			}

			if(pObj->m_currentWorldID != playerObjectData->m_currentWorldID)
			{
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"You must be near this person:"+strArgs,0,playerObjectData,0);
                break;
			}

			if(!m_sharedInterfaceRef->m_cabLib.GL_matrixFunctions.BoundingSphereCheck(	30,
																						30,
																						pObj->m_3dPosition.x,
																						pObj->m_3dPosition.y,
																						pObj->m_3dPosition.z,
																						playerObjectData->m_3dPosition.x,
																						playerObjectData->m_3dPosition.y,
																						playerObjectData->m_3dPosition.z))
			{
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"You must be near this person:"+strArgs,0,playerObjectData,0);
                break;
			}



			pObj->m_tradePendingRequest					= playerObjectData->m_networkAssignedID;
			playerObjectData->m_tradePendingRequest		= pObj->m_networkAssignedID;
			pObj->m_tradeOfferTimeStamp					= GetTickCount();
			playerObjectData->m_tradeOfferTimeStamp		= GetTickCount();
			if(	playerObjectData->m_tradeOffer)
			{
				playerObjectData->m_tradeOffer->SafeDelete();
                delete playerObjectData->m_tradeOffer;
				playerObjectData->m_tradeOffer = 0;
			}
			if(	pObj->m_tradeOffer)
			{
				pObj->m_tradeOffer->SafeDelete();
                delete pObj->m_tradeOffer;
				pObj->m_tradeOffer = 0;
			}
	      
            m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Trade request send to:"+strArgs,0,playerObjectData,0);
			m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,pObj->m_currentID,playerObjectData->m_charName + " wants to trade with you. Type /TRADEACCEPT to begin secure trade."+pObj->m_charName,0,pObj,0);
			m_sharedInterfaceRef->SendAttribMsg(DPNSEND_GUARANTEED,
												pObj->m_currentID,
												40,
												0,
												0,
												0,
												0,
												pObj);

			break;
		} // End SCMD_TRADE
		case SCMD_BANK: // Request bank inventory
		{
			// PARSE out command params
			int iLockerIndex = 0;
			if (strArgs.GetLength() > 0)
				iLockerIndex = atoi(strArgs.GetBuffer());

			m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();//have to unlock here for order
			m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Lock();
			m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Lock();
			CPlayerObject * objPtr = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByDPNID_Alive(iFromLocID);
			if(objPtr)
				m_sharedInterfaceRef->BankRequest(objPtr, iFromLocID, iLockerIndex);//TODO required AC lock
			
			m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			break;
		} // End SCMD_BANK
		case SCMD_PAGEGM:
		{
			m_sharedInterfaceRef->m_gmPageListRuntime->AddPage(playerObjectData->m_handle);
			m_sharedInterfaceRef->SendGeneralMessageHandler(79,playerObjectData);
			break;
		} // End SCMD_PAGEGM
		case SCMD_ZEROMURDERS:
		{
			if(playerObjectData->m_admin == TRUE || playerObjectData->m_gm == TRUE)
			{
				if(playerObjectData->m_noteriety)
					playerObjectData->m_noteriety->m_currentMurders = 0;
			}
			break;
		} // End SCMD_ZEROMURDERS
		case SCMD_ANSWERNEXTPAGE:
		{
			if(playerObjectData->m_gm || playerObjectData->m_admin)
			{
				POSITION nxtPagePos = m_sharedInterfaceRef->m_gmPageListRuntime->GetTailPosition();
				if(nxtPagePos)
				{
					CGmPageObj *pagePtr = (CGmPageObj*)m_sharedInterfaceRef->m_gmPageListRuntime->GetAt(nxtPagePos);

					CPlayerObject *plObjThatPaged = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandle(pagePtr->m_handle);
					if(plObjThatPaged)
					{
						m_sharedInterfaceRef->SendTextMsg(	DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
															plObjThatPaged->m_currentID,//can be NULL if client
															"A GM will be with you shortly.",
															plObjThatPaged->m_networkAssignedID,
															plObjThatPaged,
															0);
						m_sharedInterfaceRef->SpawnToPointInWld(playerObjectData,plObjThatPaged->m_Target3dPosition,plObjThatPaged->m_currentWorldMap);
					}
					else
					{
						m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
							playerObjectData->m_currentID,//can be NULL if client
							"Player has went offline",
							playerObjectData->m_networkAssignedID,
							playerObjectData,
							0);
					}

					delete pagePtr;
					pagePtr = 0;
					m_sharedInterfaceRef->m_gmPageListRuntime->RemoveAt(nxtPagePos);
				}
				else
				{
					m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
						playerObjectData->m_currentID,//can be NULL if client
						"No more pages available",
						playerObjectData->m_networkAssignedID,
						playerObjectData,
						0);
				}
			}
			break;
		} // End SCMD_ANSWERNEXTPAGE
		case SCMD_GMTOPLAYER:
		{
			if(playerObjectData->m_gm || playerObjectData->m_admin)
			{
			   CPlayerObject *playerTarget = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByName(strArgs.GetBuffer());
               if(playerTarget)
					m_sharedInterfaceRef->SpawnToPointInWld(playerObjectData,playerTarget->m_Target3dPosition,playerTarget->m_currentWorldMap);
			   else
					m_sharedInterfaceRef->SendTextMsg(  DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
														playerObjectData->m_currentID,//can be NULL if client
														"Cannot find player:"+strArgs,
														playerObjectData->m_networkAssignedID,
														playerObjectData,
														0);
			}
			break;
		}
		case SCMD_PLAYERTOGM:
		{
			if(playerObjectData->m_gm || playerObjectData->m_admin)
			{
			   CPlayerObject *playerTarget = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByName(strArgs.GetBuffer());
               if(playerTarget)
					m_sharedInterfaceRef->SpawnToPointInWld(playerTarget,playerObjectData->m_Target3dPosition,playerObjectData->m_currentWorldMap);
			   else
					m_sharedInterfaceRef->SendTextMsg(  DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
														playerObjectData->m_currentID,//can be NULL if client
														"Cannot find player:"+strArgs,
														playerObjectData->m_networkAssignedID,
														playerObjectData,
														0);
			}
			break;
		}
		case SCMD_WITHDRAWAL:
		{
			break;
		} // End SCMD_WITHDRAWAL -obsolete
		case SCMD_DEPOSIT:
		{
			break;
		} // End SCMD_DEPOSIT -obsolete
		case SCMD_CREATEGROUP:
		{
			
			break;
		} // End SCMD_CREATEGROUP
		case SCMD_DISBANDGROUP:
		{
			break;
		} // End SCMD_DISBANDGROUP
		case SCMD_UC_LISTRECENT:
		{
			if(!playerObjectData->m_inSafeZone)
			{//not in safe zone you cannot access global commerce system
				m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
													"You must be in a safe zone to access the Unified Commerce System",
													playerObjectData);
				break;
			}//end not in safe zone you cannot access global commerce system

			if(!m_sharedInterfaceRef->m_unifiedCommSys.AddCommand_safe(UC_LISTRECENT,playerObjectData->m_handle,strArgs))
					m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
														"Please try this command again in a few moments",
														playerObjectData);
			break;
		}
		case SCMD_UC_LISTBYITEM:
		{
			if(!playerObjectData->m_inSafeZone)
			{//not in safe zone you cannot access global commerce system
				m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
													"You must be in a safe zone to access the Unified Commerce System",
													playerObjectData);
				break;
			}//end not in safe zone you cannot access global commerce system

			const char * buf = strArgs.GetBuffer();
			unsigned int length = strArgs.GetLength();
			unsigned int dpos = 0;

			CString  param1,param2; 

			ParseByDelimiter(	dpos,buf,length,',',param1);
			ParseByDelimiter(	dpos,buf,length,',',param2);


			if(!m_sharedInterfaceRef->m_unifiedCommSys.AddCommand_safe(UC_LISTBYITEM,playerObjectData->m_handle,param1,param2))
					m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
														"Please try this command again in a few moments",
														playerObjectData);
			break;
		}
		case SCMD_UC_SELLITEM:
		{
			if(!playerObjectData->m_inSafeZone)
			{//not in safe zone you cannot access global commerce system
				m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
													"You must be in a safe zone to access the Unified Commerce System",
													playerObjectData);
				break;
			}//end not in safe zone you cannot access global commerce system

			const char * buf = strArgs.GetBuffer();
			unsigned int length = strArgs.GetLength();
			unsigned int dpos = 0;

			CString  param1,param2,param3; 

			ParseByDelimiter(	dpos,buf,length,',',param1);
			ParseByDelimiter(	dpos,buf,length,',',param2);
			ParseByDelimiter(	dpos,buf,length,',',param3);

			if(!m_sharedInterfaceRef->m_unifiedCommSys.AddCommand_safe(UC_SELLITEM,playerObjectData->m_handle,playerObjectData->m_charName,param1,param2,param3))
					m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
														"Please try this command again in a few moments",
														playerObjectData);

			break;
		}
		case SCMD_UC_BUYITEM:
		{
			if(!playerObjectData->m_inSafeZone)
			{//not in safe zone you cannot access global commerce system
				m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
													"You must be in a safe zone to access the Unified Commerce System",
													playerObjectData);
				break;
			}//end not in safe zone you cannot access global commerce system

			const char * buf = strArgs.GetBuffer();
			unsigned int length = strArgs.GetLength();
			unsigned int dpos = 0;

			CString  param1,param2,param3,param4; 

			ParseByDelimiter(	dpos,buf,length,',',param1);
			ParseByDelimiter(	dpos,buf,length,',',param2);
			ParseByDelimiter(	dpos,buf,length,',',param3);
			ParseByDelimiter(	dpos,buf,length,',',param4);

			if(!m_sharedInterfaceRef->m_unifiedCommSys.AddCommand_safe(UC_BUYITEM,playerObjectData->m_handle,playerObjectData->m_charName,param1,param2,param3,param4))
					m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
														"Please try this command again in a few moments",
														playerObjectData);
			break;
		}
		case SCMD_UC_CASHOUT:
		{
			if(!playerObjectData->m_inSafeZone)
			{//not in safe zone you cannot access global commerce system
				m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
													"You must be in a safe zone to access the Unified Commerce System",
													playerObjectData);
				break;
			}//end not in safe zone you cannot access global commerce system

			if(!m_sharedInterfaceRef->m_unifiedCommSys.AddCommand_safe(UC_CASHOUT,playerObjectData->m_handle,playerObjectData->m_charName))
					m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
														"Please try this command again in a few moments",
														playerObjectData);

			break;
		}
		case SCMD_UC_SALESSTATS:
		{
			if(!playerObjectData->m_inSafeZone)
			{//not in safe zone you cannot access global commerce system
				m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
													"You must be in a safe zone to access the Unified Commerce System",
													playerObjectData);
				break;
			}//end not in safe zone you cannot access global commerce system

			if(!m_sharedInterfaceRef->m_unifiedCommSys.AddCommand_safe(UC_SALESSTATS,playerObjectData->m_handle,strArgs))
					m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
														"Please try this command again in a few moments",
														playerObjectData);
			break;
		}
		case SCMD_C:
		{
			
			if(playerObjectData->m_clanHandle <= 0)
			{
				m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
													"You arent in a clan",
													playerObjectData);

				break;
			}
			if(!m_sharedInterfaceRef->m_clanCommSys.AddCommand_safe(CLAN_CHAT,playerObjectData->m_handle,strArgs,""))
					m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
														"Please try this command again in a few moments",
														playerObjectData);
			break;
		}
		case SCMD_REQUESTJOINGROUP:
		{
			
		} // End SCMD_REQUESTJOINGROUP
		case SCMD_APPROVECLANRECRUIT:
		{
			if(playerObjectData->m_clanHandle <= 0)
			{
				m_sharedInterfaceRef->SendTextMsg(  DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
														playerObjectData->m_currentID,//can be NULL if client
														"You arent in a clan",
														playerObjectData->m_networkAssignedID,
														playerObjectData,
														0);

				break;
			}
			if(playerObjectData->m_clanHandle == playerObjectData->m_handle)
			{//clan owner

			   if(playerObjectData->m_currentRequestToJoinClanHandle <= 0)
			   {
					m_sharedInterfaceRef->SendTextMsg(  DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
														playerObjectData->m_currentID,
														"No one is requesting to join atm...",
														playerObjectData->m_networkAssignedID,
														playerObjectData,
														0);

					break;
			   }


			   CPlayerObject * recruit = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandleAlive(playerObjectData->m_currentRequestToJoinClanHandle);
			   if(recruit)
			   {
					m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->ReIndexByClanHandle(recruit, playerObjectData->m_clanHandle);
					recruit->m_clanAbbrev = playerObjectData->m_clanAbbrev;
					recruit->ReassembleName(m_sharedInterfaceRef);//reconstruct
				    m_sharedInterfaceRef->SendClientScript(recruit,"UpdateCharName("+recruit->m_visName+")","");
					m_sharedInterfaceRef->SendTextMsg(  DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
														playerObjectData->m_currentID,//can be NULL if client
														"Recruited: "+recruit->m_charName,
														playerObjectData->m_networkAssignedID,
														playerObjectData,
														0);

					m_sharedInterfaceRef->SendTextMsg(  DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
														recruit->m_currentID,//can be NULL if client
														"You have been accepted into: "+playerObjectData->m_clanAbbrev,
														recruit->m_networkAssignedID,
														recruit,
														0);

			   }
			   else
					m_sharedInterfaceRef->SendTextMsg(  DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
														playerObjectData->m_currentID,//can be NULL if client
														"Error Cannot find Recruit",
														playerObjectData->m_networkAssignedID,
														playerObjectData,
														0);

			   playerObjectData->m_currentRequestToJoinClanHandle = 0;//unlink

			}//end clan owner
			else
				m_sharedInterfaceRef->SendTextMsg(  DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
														playerObjectData->m_currentID,//can be NULL if client
														"Your are not a leader or captain of this clan",
														playerObjectData->m_networkAssignedID,
														playerObjectData,
														0);
			break;
		} // End SCMD_APPROVERECRUIT
		case SCMD_DESTROYMYSKILL:
		{
			CSkillObject *skillPtrRef = m_sharedInterfaceRef->m_skillDatabase->GetSkillByName(strArgs);
			if(skillPtrRef && playerObjectData->m_skills)
			{
				CSkillObject *onPlayerSkill =  playerObjectData->m_skills->GetSkillByType(skillPtrRef->m_skillType);
				if(onPlayerSkill)
				{
					onPlayerSkill->m_currentLevel = 1;
					onPlayerSkill->m_currentExperience = 5.0f;
					m_sharedInterfaceRef->SendGeneralMessageHandler(90,playerObjectData); // Acceptance notification
				}
			}
			break;
		} // End SCMD_DESTROYMYSKILL
		case SCMD_DESTROYJNLENTRY:
		{
			if(playerObjectData->m_playerOrigRef)
			{
				if(playerObjectData->m_playerOrigRef->m_questJournal->DeleteEntryByNpcName(strArgs))
				{
					m_sharedInterfaceRef->UpdateClientRecentQuestJournalEntry(playerObjectData);
					m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
						playerObjectData->m_currentID,//can be NULL if client
						"**Entry has been removed-quest deleted**",
						playerObjectData->m_networkAssignedID,
						playerObjectData,
						0);
				}
				else
				{
					m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
						playerObjectData->m_currentID,//can be NULL if client
						"Cannot find quest entry in journal...",
						playerObjectData->m_networkAssignedID,
						playerObjectData,
						0);
				}
			}
			break;
		} // End SCMD_DESTROYJNLENTRY
		case SCMD_DECLAREWAR:
		{
			
			break;
		} // End SCMD_DECLAREWAR
		case SCMD_ACCEPTWAR:
		{
		
			break;
		} // End SCMD_ACCEPTWAR
		case SCMD_SURRENDER:
		{
		
			break;
		} // End SCMD_SURRENDER
		case SCMD_SPAWNGEN:
		{
			if( (playerObjectData->m_admin || playerObjectData->m_gm)
				&& playerObjectData->m_spawnAbility)
			{
				m_sharedInterfaceRef->SpawnDynamicAIToAvailAiServer(atoi(strArgs),
					playerObjectData->m_currentWorldID, 
					playerObjectData->m_Target3dPosition,
					-1,
					0,
					-1,
					0,
					0,
					0,
					0,
					playerObjectData->m_currentWorldMap);
			}
			break;
		} // End SCMD_SPAWNGEN
		case SCMD_BROADCAST:
		{
			if(playerObjectData->m_admin || playerObjectData->m_gm)
			{
				//was BroadcastMessage(...,-1,...), which only reached part of the server
				m_sharedInterfaceRef->BroadcastToAll("[SERVER] " + strArgs);
			}
			break;
		} // End SCMD_BROADCAST
		case SCMD_INVITETOGROUP:
		{

			break;
		} // End SCMD_INVITETOGROUP
		case SCMD_REQUESTAREAPC:
		{
			CAxList * rPlList = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetListByChannel(playerObjectData->m_currentWorldID);
			CString buildList;
			for(CAxLinkObj * pos = rPlList->GetHeadPosition_unsafe();pos != NULL;)
			{//list check
				CPlayerObject * plObj = (CPlayerObject *)rPlList->GetNext_unsafe(&pos);
				if(!plObj->m_aiControlled)
					if(m_sharedInterfaceRef->m_cabLib.GL_matrixFunctions.BoundingSphereCheck(	200,
																								10,
																								plObj->m_3dPosition.x,
																								plObj->m_3dPosition.y,
																								plObj->m_3dPosition.z,
																								playerObjectData->m_3dPosition.x,
																								playerObjectData->m_3dPosition.y,
																								playerObjectData->m_3dPosition.z))
					{//clan leader filter
						buildList += plObj->m_charName + "&";
					}
			}

			if(buildList.GetLength() > 0)
				m_sharedInterfaceRef->SendClientScript(playerObjectData,"FillList(237,391,"+buildList+")","");//menu id, visual id, batch text to parse by &
			
			break;
		}
		case SCMD_REQUESTAREATRADE:
		{
			CAxList * rPlList = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetListByChannel(playerObjectData->m_currentWorldID);
			CString buildList;
			for(CAxLinkObj * pos = rPlList->GetHeadPosition_unsafe();pos != NULL;)
			{//list check
				CPlayerObject * plObj = (CPlayerObject *)rPlList->GetNext_unsafe(&pos);
				if(plObj->m_energy > 0 && !plObj->m_aiControlled)
				{//clan leader filter
					buildList += plObj->m_charName + "&";
				}
			}

			if(buildList.GetLength() > 0)
				m_sharedInterfaceRef->SendClientScript(playerObjectData,"FillList(100,73,"+buildList+")","");//menu id, visual id, batch text to parse by &

			break;
		}
		case SCMD_REQUESTAREACLANLEADERS:
		{
			CAxList * rPlList = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetListByChannel(playerObjectData->m_currentWorldID);
			CString buildList;
			for(CAxLinkObj * pos = rPlList->GetHeadPosition_unsafe();pos != NULL;)
			{//list check
				CPlayerObject * plObj = (CPlayerObject *)rPlList->GetNext_unsafe(&pos);
				if(plObj->m_clanHandle > 0 && plObj->m_clanHandle == plObj->m_handle && plObj->m_energy > 0)
				{//clan leader filter
					buildList += plObj->m_charName + "&";
				}
			}

			if(buildList.GetLength() > 0)
				m_sharedInterfaceRef->SendClientScript(playerObjectData,"FillList(237,391,"+buildList+")","");//menu id, visual id, batch text to parse by &

			break;
		}
		case SCMD_REQUESTCHARSKILLS:
		{
			if(playerObjectData->m_energy < 1)
				break;
			CString buildList;
			if(playerObjectData->m_skills)
			{
				for(POSITION skillPos = playerObjectData->m_skills->GetHeadPosition();skillPos != NULL;)
				{
                    CSkillObject *skillPtr = (CSkillObject *) playerObjectData->m_skills->GetNext(skillPos);
					CSkillObject * skillPtrRef = m_sharedInterfaceRef->m_skillDatabase->GetSkillByType(skillPtr->m_skillType);
					if(skillPtrRef)
					{
						int levelIndex = 1;
						bool foundRange = false;
						for(POSITION posLevel = skillPtrRef->m_levelRangeDB->GetHeadPosition();posLevel != NULL;)
						{
							CLevelRangeObject *level = (CLevelRangeObject*)skillPtrRef->m_levelRangeDB->GetNext(posLevel);
							if(level->m_rangeStart <= skillPtr->m_currentExperience && level->m_rangeEnd > skillPtr->m_currentExperience)
							{
								float levelRange = (float)level->m_rangeEnd - (float)level->m_rangeStart;
								float levelGrowth = (float)level->m_rangeEnd - (float)skillPtr->m_currentExperience;
								if(levelRange == 0.0f)
								   break;
								
								float xpHolder = levelGrowth / levelRange;
								float xpPerc = xpHolder * 100.0f;
								xpPerc = 100.0f - xpPerc;

								
								CString percString;
								percString.Format("%.2f",xpPerc);

								buildList += skillPtrRef->m_skillName + " - Level " + m_sharedInterfaceRef->m_cabLib.ConvToString(levelIndex) + " - " + percString + "%&";
								foundRange = true;
							}
							levelIndex++;
						}	
						if(!foundRange)
						{
							buildList += skillPtrRef->m_skillName + " - Level " + m_sharedInterfaceRef->m_cabLib.ConvToString(levelIndex) + " - MAX%&";
						}
					}
				}
			}
			
			if(buildList.GetLength() > 0)
				m_sharedInterfaceRef->SendClientScript(playerObjectData,"FillList(1426,1439,"+buildList+")","");//menu id, visual id, batch text to parse by &

			break;
		}
		case SCMD_REQUESTCLANMEMBERS:
		{
			if(playerObjectData->m_clanHandle > -1)
			{
				CAxList * rPlList = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetListByClanID(playerObjectData->m_clanHandle);
				CString buildList;
				for(CAxLinkObj * pos = rPlList->GetHeadPosition_unsafe();pos != NULL;)
				{//list check
					CPlayerObject * plObj = (CPlayerObject *)rPlList->GetNext_unsafe(&pos);
					if(plObj->m_clanHandle == playerObjectData->m_clanHandle)
					{//clan leader filter
						buildList += plObj->m_charName + "&";
					}
				}

				if(buildList.GetLength() > 0)
					m_sharedInterfaceRef->SendClientScript(playerObjectData,"FillList(336,338,"+buildList+")","");//menu id, visual id, batch text to parse by &
			}
			break;
		}
		case SCMD_CREATECLAN:
		{
			if(playerObjectData->m_clanHandle > 0)
			{
				m_sharedInterfaceRef->SendTextMsg(		playerObjectData->m_currentID,//can be NULL if client
														"You cannot create a clan when you are currently a member of one",
														playerObjectData);

				break;
			}
			//do any name filtering here
			if(!m_sharedInterfaceRef->m_clanCommSys.AddCommand_safe(CLAN_CREATE,playerObjectData->m_handle,strArgs,""))//request to que
			{//already doing a request for this handle
					m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
														"Please try this command again in a few moments",
														playerObjectData);
			}
			break;
		}
		case SCMD_DISBANCLAN:
		{
			if(playerObjectData->m_clanHandle <= 0)
			{
				m_sharedInterfaceRef->SendTextMsg(		playerObjectData->m_currentID,//can be NULL if client
														"You arent in a clan",
														playerObjectData);

				break;
			}
			if(playerObjectData->m_clanHandle == playerObjectData->m_handle)
			{//clan owner
				if(!m_sharedInterfaceRef->m_clanCommSys.AddCommand_safe(CLAN_DELETE,playerObjectData->m_handle,playerObjectData->m_clanAbbrev,""))
					m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
														"Please try this command again in a few moments",
														playerObjectData);
				else
				{
					playerObjectData->m_clanAbbrev = "";
					playerObjectData->ReassembleName(m_sharedInterfaceRef);//reconstruct
					m_sharedInterfaceRef->SendClientScript(playerObjectData,"UpdateCharName("+playerObjectData->m_visName+")","");
					m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
														"Clan Deleted",
														playerObjectData);
				}

			}//end clan owner
			else
			{//member of a group
				m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->ReIndexByClanHandle(playerObjectData, -1);
				playerObjectData->m_clanAbbrev = "";
				playerObjectData->ReassembleName(m_sharedInterfaceRef);//reconstruct
				m_sharedInterfaceRef->SendClientScript(playerObjectData,"UpdateCharName("+playerObjectData->m_visName+")","");
				m_sharedInterfaceRef->SendTextMsg(  DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
													playerObjectData->m_currentID,//can be NULL if client
													"Left the clan:"+playerObjectData->m_clanAbbrev,
													playerObjectData->m_networkAssignedID,
													playerObjectData,
													0);
			}//end member of a group
			
			break;
		}
		case SCMD_REQUESTJOINCLAN:
		{
			if(playerObjectData->m_clanHandle > 0)
			{
				m_sharedInterfaceRef->SendTextMsg(		playerObjectData->m_currentID,//can be NULL if client
														"Your already in a clan",
														playerObjectData);

				break;
			}

			CPlayerObject * clanLeader = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByNameAlive(strArgs.GetBuffer());
			if(!clanLeader)
			{
				m_sharedInterfaceRef->SendTextMsg(  DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
														playerObjectData->m_currentID,//can be NULL if client
														"Cannot find :"+strArgs,
														playerObjectData->m_networkAssignedID,
														playerObjectData,
														0);
				break;
			}

			if(clanLeader->m_handle == clanLeader->m_clanHandle)
			{//clan leader
                clanLeader->m_currentRequestToJoinClanHandle = playerObjectData->m_handle;

				m_sharedInterfaceRef->SendTextMsg(  DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
														clanLeader->m_currentID,//can be NULL if client
														playerObjectData->m_charName+" is requesting to join your clan TYPE /APPROVERECRUIT CHARNAME to accept",
														clanLeader->m_networkAssignedID,
														clanLeader,
														0);

				int iCounter = 0;
				int msgsz = 0;
				int iPlayerNameLength = playerObjectData->m_charName.GetLength();
				int iSquadNameLength = playerObjectData->m_squadName.GetLength();
				BYTE *byteBlock = NULL;
				msgsz += sizeof(short) 
					+ sizeof(char) 
					+ sizeof(char) 
					+ sizeof(char)
					+ iPlayerNameLength
					+ sizeof(char)
					+ iSquadNameLength;
				msgsz++;

				byteBlock = new BYTE[msgsz];
				byteBlock[iCounter++] = 15; //Setup the catagory for HandleMiscBlocksClientSide
				int nullStuff = 0;

				// ENCODE basic squad data
				CopyMemory(&byteBlock[iCounter], &playerObjectData->m_networkAssignedID, sizeof(unsigned short));
				iCounter += sizeof(unsigned short);
				CopyMemory(&byteBlock[iCounter], &nullStuff, sizeof(unsigned char));
				iCounter += sizeof(unsigned char);
				CopyMemory(&byteBlock[iCounter], &nullStuff, sizeof(unsigned char));
				iCounter += sizeof(unsigned char);

				// ENCODE player name data
				CopyMemory(&byteBlock[iCounter], &iPlayerNameLength, sizeof(unsigned char));
				iCounter += sizeof(unsigned char);
				CopyMemory(&byteBlock[iCounter], playerObjectData->m_charName.GetBuffer(), iPlayerNameLength);
				iCounter += iPlayerNameLength;
				playerObjectData->m_charName.ReleaseBuffer();

				// ENCODE squad name data
				CopyMemory(&byteBlock[iCounter], &iSquadNameLength, sizeof(unsigned char));
				iCounter += sizeof(unsigned char);
				CopyMemory(&byteBlock[iCounter], playerObjectData->m_squadName.GetBuffer(), iSquadNameLength);
				iCounter += iSquadNameLength;
				playerObjectData->m_squadName.ReleaseBuffer();

				if(clanLeader->m_miscOutBox->GetCount() < 20)
					clanLeader->m_miscOutBox->AddPacket(msgsz,(BYTE*)byteBlock,0,clanLeader->m_currentWorldID);

				delete [] byteBlock;
				byteBlock = 0;

				m_sharedInterfaceRef->SendAttribMsg(DPNSEND_GUARANTEED,
												clanLeader->m_currentID,
												42,
												0,
												0,
												0,
												0,
												clanLeader);

				m_sharedInterfaceRef->SendTextMsg(  DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
														playerObjectData->m_currentID,//can be NULL if client
														"Request sent to :"+strArgs,
														playerObjectData->m_networkAssignedID,
														playerObjectData,
														0);
			}//end clan Leader
			else
				m_sharedInterfaceRef->SendTextMsg(  DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
														playerObjectData->m_currentID,//can be NULL if client
														"Not a clan recruiter or leader :"+strArgs,
														playerObjectData->m_networkAssignedID,
														playerObjectData,
														0);

			break;
		}
		case SCMD_REQUESTCLANINIT:
		{
			if(playerObjectData->m_clanHandle <= 0)
			{
				//send open menu command to look for a clan to join
				m_sharedInterfaceRef->SendAttribMsg(DPNSEND_GUARANTEED,
													playerObjectData->m_currentID,
													20,
													0,
													0,
													CLAN_RECRUITMENT_MENU,
													0,
													playerObjectData);
			}
			else
			{
				//send open menu command if they are currently in a clan
				m_sharedInterfaceRef->SendAttribMsg(DPNSEND_GUARANTEED,
													playerObjectData->m_currentID,
													20,
													0,
													0,
													CLAN_MENU_ID,
													0,
													playerObjectData);
			}
			break;
		}
		case SCMD_REMOVECLANMEMBER:
		{
			if(playerObjectData->m_clanHandle <= 0)
			{
				m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
													"You arent in a group",
													playerObjectData);

				break;
			}
			if(playerObjectData->m_clanHandle != playerObjectData->m_handle)
			{
				m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
													"Your are not a group captain or leader",
													playerObjectData);

				break;
			}
			if(playerObjectData->m_charName.CompareNoCase(strArgs)==0)
			{
				m_sharedInterfaceRef->SendTextMsg(	playerObjectData->m_currentID,//can be NULL if client
													"cannot remove your self from the group, use /DISBANDGROUP",
													playerObjectData);
				break;
			}

			if(!m_sharedInterfaceRef->m_clanCommSys.AddCommand_safe(CLAN_REMOVEMEMBER,playerObjectData->m_handle,strArgs,""))
				m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
													"Please try this command again in a few moments",
													playerObjectData);
			break;

		}
		case SCMD_LEAVEGROUP:
		{
			
		} // End SCMD_LEAVEGROUP
		/*case SCMD_REMOVEFROMGROUP:
		{
			
		} // End SCMD_REMOVEFROMGROUP*/
		case SCMD_VIEWROSTER:
		{  
			if(playerObjectData->m_clanHandle <= 0)
			{
				m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
													"You are not in a clan",
													playerObjectData);

				break;
			}
			if(!m_sharedInterfaceRef->m_clanCommSys.AddCommand_safe(CLAN_REQUESTMEMBERS,playerObjectData->m_handle,m_sharedInterfaceRef->m_cabLib.ConvToString(playerObjectData->m_clanHandle),""))
				m_sharedInterfaceRef->SendTextMsg(  playerObjectData->m_currentID,//can be NULL if client
													"Please try this command again in a few moments",
													playerObjectData);
			break;
		} // End SCMD_VIEWROSTER
		case SCMD_VIEWWARLIST:
		{  
			
			break;
		} // End SCMD_VIEWWARLIST
		case SCMD_COMBINESTACKS:
		{
			break;
		} // End SCMD_COMBINESTACKS
		case SCMD_SPLITSTACK:
		{
			break;
		} // End SCMD_SPLITSTACK
		case SCMD_DROPSTACK:
		{
			break;
		} // End SCMD_DROPSTACK
		case SCMD_PICKUPSTACK:
		{
			break;
		} // End SCMD_PICKUPSTACK
		case SCMD_DELETESTACK:
		{
			break;
		} // End SCMD_DELETESTACK
		case SCMD_MOVESTACKTOPACK:
		{
			break;
		} // End SCMD_MOVESTACKTOPACK
		case SCMD_ARMSTACK:
		{
			break;
		} // End SCMD_ARMSTACK
		case SCMD_UNARMSTACK:
		{
			break;
		} // End SCMD_UNARMSTACK
		case SCMD_MURDERCOUNT:
		{
			CString MurderCountText = "You currently have "; 
			MurderCountText += m_sharedInterfaceRef->m_cabLib.ConvToString(playerObjectData->m_noteriety->m_currentMurders); 
			MurderCountText += ". It has been ";
			MurderCountText += m_sharedInterfaceRef->m_cabLib.ConvertLongToString(playerObjectData->m_noteriety->m_currentMinuteCountSinceLastMurder / 60000);
			MurderCountText += " minutes since your last murder.";
			m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,MurderCountText,playerObjectData->m_networkAssignedID,playerObjectData,0);
			break;
		} // End SCMD_MURDERCOUNT
		case SCMD_MURDERWEAR:
		{
			CString MurderCountText = CString("Update Tick: ") + m_sharedInterfaceRef->m_cabLib.ConvToString(playerObjectData->m_noteriety->m_murderUpdateTick) + CString(" Tick Difference: ") + m_sharedInterfaceRef->m_cabLib.ConvToString(GetTickCount()-playerObjectData->m_noteriety->m_murderUpdateTick);
			m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,MurderCountText,playerObjectData->m_networkAssignedID,playerObjectData,0);
			break;
		} // End SCMD_MURDERWEAR
		case SCMD_FORCEGROUPID://TEST REMOVE
		{
			break;
		} // End SCMD_FORCEGROUPID
		case SCMD_INFO://TEST REMOVE
		{
			CString instanceID,groupID;
			instanceID.Format(" instanceID:%i ",playerObjectData->m_instanceType);
			groupID.Format(" group id:%i ",playerObjectData->m_groupID);
			m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,instanceID+groupID,playerObjectData->m_networkAssignedID,playerObjectData,0);
			break;
		} // End SCMD_INFO
		case SCMD_ADMINRF:
		{
			if(!playerObjectData->m_admin)
			   break;
               
			playerObjectData->m_reflectLevel = 1.0f;

			m_sharedInterfaceRef->SendTextMsg(	DPNSEND_GUARANTEED,
												playerObjectData->m_currentID,
												"Enabled Reflect Level 100%:"+playerObjectData->m_charName,
												playerObjectData->m_networkAssignedID,
												playerObjectData,
												0);
			break;
		}
		case SCMD_ADMINEXPLODE:
		{
            if(!playerObjectData->m_admin)
			   break;

            CPlayerObject * ptr = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByNameAlive(strArgs.GetBuffer());
			if(!ptr)
			{
				m_sharedInterfaceRef->SendTextMsg(	DPNSEND_GUARANTEED,
													playerObjectData->m_currentID,
													"Cannot find player:"+strArgs,
													playerObjectData->m_networkAssignedID,
													playerObjectData,
													0);
				break;
			}

			m_sharedInterfaceRef->SendTextMsg(	DPNSEND_GUARANTEED,
												playerObjectData->m_currentID,
												"Exploded:"+ptr->m_charName,
												playerObjectData->m_networkAssignedID,
												playerObjectData,
												0);

			m_sharedInterfaceRef->BroadcastEffect(7,ptr->m_networkAssignedID,ptr);
            ptr->m_energy = 0;
			m_sharedInterfaceRef->UpdateClientStatistics(ptr,ptr->m_currentID);
			m_sharedInterfaceRef->HandleDeath( ptr,ptr);
			
		
			break;
		}
		case SCMD_ADMINSLAP:
		{
            if(!playerObjectData->m_admin)
			   break;

            CPlayerObject * ptr = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByNameAlive(strArgs.GetBuffer());
			if(!ptr)
			{
				m_sharedInterfaceRef->SendTextMsg(	DPNSEND_GUARANTEED,
													playerObjectData->m_currentID,
													"Cannot find player:"+strArgs,
													playerObjectData->m_networkAssignedID,
													playerObjectData,
													0);
				break;
			}

			m_sharedInterfaceRef->SendTextMsg(	DPNSEND_GUARANTEED,
												playerObjectData->m_currentID,
												"You have Bitch Slapped "+ptr->m_charName +" to 1 hp!!",
												playerObjectData->m_networkAssignedID,
												playerObjectData,
												0);

			m_sharedInterfaceRef->BroadcastEffect(7,ptr->m_networkAssignedID,ptr);
            ptr->m_energy = 1;
			m_sharedInterfaceRef->UpdateClientStatistics(ptr,ptr->m_currentID);
		
			break;
		}
		case SCMD_TRADEACCEPT:
		{
			if(GetTickCount()-playerObjectData->m_tradeOfferTimeStamp > 120000)
			   playerObjectData->m_tradePendingRequest = -1;//timeout

			if(playerObjectData->m_tradePendingRequest == -1)
			{// no active trade requests
			   m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"there are no active trade requests",0,playerObjectData,0);
               break;
			}

			CPlayerObject * pObj = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_Alive(playerObjectData->m_tradePendingRequest);
			if(!pObj)
			{//cannot find player
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Cannot find player that made the trade request, cancelling trade."+strArgs,0,playerObjectData,0);
				playerObjectData->m_tradePendingRequest = -1;
				break;
			}

			//lock in the trade
            pObj->m_tradeTargetID = playerObjectData->m_networkAssignedID;
			playerObjectData->m_tradeTargetID = pObj->m_networkAssignedID;

            //notify secure trade process has begun
			m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Secure trade opened with:"+pObj->m_charName,0,playerObjectData,0);
			m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,pObj->m_currentID,"Secure trade opened with:"+playerObjectData->m_charName,0,pObj,0);

			//send open menu command
			m_sharedInterfaceRef->SendAttribMsg(DPNSEND_GUARANTEED,
												playerObjectData->m_currentID,
												20,
												0,
												0,
												TRADE_MENU_ID,
												0,
												playerObjectData);

			m_sharedInterfaceRef->SendAttribMsg(DPNSEND_GUARANTEED,
												pObj->m_currentID,
												20,
												0,
												0,
												TRADE_MENU_ID,
												0,
												pObj);

			break;
		}
		case SCMD_TRADEOFFER:
		{
			if(playerObjectData->m_tradeTargetID == -1)
			{
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Trade not active",0,playerObjectData,0);
				break;
			}

			if(playerObjectData->m_tradeOffer != NULL)
			{
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Offer already made, if you like to make a new offer cancel current trade and start again.",0,playerObjectData,0);
				break;
			}
			CPlayerObject * pObj = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_Alive(playerObjectData->m_tradePendingRequest);
			if(!pObj)
			{//cannot find player
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Cannot find player that made the trade request, cancelling trade.."+strArgs,0,playerObjectData,0);
				playerObjectData->m_tradePendingRequest = -1;
				playerObjectData->m_tradeTargetID		= -1;
				//send open menu command
				m_sharedInterfaceRef->SendAttribMsg(DPNSEND_GUARANTEED,
													playerObjectData->m_currentID,
													21,
													0,
													0,
													TRADE_MENU_ID,
													0,
													playerObjectData);//close trade menus

				break;
			}
			//build offer /verify inventory

			char * bufArray = strArgs.GetBuffer();
			DWORD length = strArgs.GetLength();
			CString buildGlid,buildQty;
			int mode = 0;
			playerObjectData->m_tradeOffer = new CInventoryObjList();
			CString decoratedOfferBuild = "Received offer from "+playerObjectData->m_charName + " :";
			for(int loop=0;loop<length;loop++)
			{//parse format glid:qty;glid:qty;
                if(bufArray[loop] == ';')
				{//process 
					CInventoryObj * itemLoc = playerObjectData->m_currentInventory->VerifyInventoryUnarmed(atoi(buildGlid),atoi(buildQty));
                    if(!itemLoc)
					{//failed varification
                        m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Offered items cannot be armed.",0,playerObjectData,0);
				        if(playerObjectData->m_tradeOffer)
						{
                           playerObjectData->m_tradeOffer->SafeDelete();
						   delete playerObjectData->m_tradeOffer;
						   playerObjectData->m_tradeOffer = 0;
						}
						break;
					}//end failed verification
					decoratedOfferBuild += "(" + buildQty + ")" + itemLoc->m_itemData->m_itemName + " ";
                    playerObjectData->m_tradeOffer->AddInventoryItem(atoi(buildQty),FALSE,itemLoc->m_itemData);
					//reset
				    mode			= 0;//rec glid
					buildQty		= "";
					buildGlid		= "";
				}//end process
				else if(bufArray[loop] == ':')
				{
					mode = 1;//rec qty
				}
				else
				{
					if(mode == 0)
						buildGlid += bufArray[loop];
					else
						buildQty += bufArray[loop];
				}
			}

			if(!playerObjectData->m_tradeOffer)
				break;//an item failed verification - the offer was cancelled (used to still say 'Offer sent.' and send a NULL offer)

			//send offer 
			//command line here is for test only
			m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Offer sent.",0,playerObjectData,0);
			m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,pObj->m_currentID,decoratedOfferBuild,0,pObj,0);
			m_sharedInterfaceRef->SendPlayerInventory(playerObjectData->m_tradeOffer,
													  FALSE,
													  pObj->m_currentID,
													  270,
													  pObj,
													  1,
													  0);

			break;
		}
		case SCMD_TRADERESET:
		{
			if(playerObjectData->m_tradeTargetID == -1)
			{
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Trade not active",0,playerObjectData,0);
				break;
			}
			CPlayerObject * pObj = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_Alive(playerObjectData->m_tradePendingRequest);
			if(!pObj)
			{//cannot find player
				playerObjectData->m_tradePendingRequest = -1;
				playerObjectData->m_tradeTargetID		= -1;
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Trade Cancelled.",0,playerObjectData,0);
				break;
			}

			//Reset the offers
			if(playerObjectData->m_tradeOffer)
			{
				playerObjectData->m_tradeOffer->SafeDelete();
                delete playerObjectData->m_tradeOffer;
				playerObjectData->m_tradeOffer = 0;
			}
			//Reset the offers
			if(pObj->m_tradeOffer)
			{
				pObj->m_tradeOffer->SafeDelete();
                delete pObj->m_tradeOffer;
				pObj->m_tradeOffer = 0;
			}

			m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Offer Reset, Item removed from list.",0,playerObjectData,0);
			m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,pObj->m_currentID,"Offer Reset, Item removed from list.",0,pObj,0);
			
			break;
		}
		case SCMD_TRADECANCEL:
		{
            if(playerObjectData->m_tradeTargetID == -1)
			{
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Trade not active",0,playerObjectData,0);
				break;
			}

			CPlayerObject * pObj = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_Alive(playerObjectData->m_tradePendingRequest);
			if(!pObj)
			{//cannot find player
				playerObjectData->m_tradePendingRequest = -1;
				playerObjectData->m_tradeTargetID		= -1;
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Trade Cancelled.",0,playerObjectData,0);
				break;
			}
			playerObjectData->m_tradePendingRequest		= -1;
			playerObjectData->m_tradeTargetID			= -1;
			pObj->m_tradePendingRequest					= -1;
		    pObj->m_tradeTargetID						= -1;

			m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Trade Cancelled.",0,playerObjectData,0);
			m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,pObj->m_currentID,"Trade Cancelled.",0,pObj,0);

			//close menus
			m_sharedInterfaceRef->SendAttribMsg(DPNSEND_GUARANTEED,
												playerObjectData->m_currentID,
												21,
												0,
												0,
												0,
												0,
												playerObjectData);

			m_sharedInterfaceRef->SendAttribMsg(DPNSEND_GUARANTEED,
												pObj->m_currentID,
												21,
												0,
												0,
												0,
												0,
												pObj);

			break;
		}
		case SCMD_TRADECONFIRM:
		{
			if(playerObjectData->m_tradeTargetID == -1)
			{
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Trade not active",0,playerObjectData,0);
				break;
			}

			CPlayerObject * pObj = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_Alive(playerObjectData->m_tradePendingRequest);
			if(!pObj)
			{//cannot find player
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Cannot find player that made the trade request, cancelling trade.."+strArgs,0,playerObjectData,0);
				playerObjectData->m_tradePendingRequest = -1;
				playerObjectData->m_tradeTargetID		= -1;
				//close menus
			    m_sharedInterfaceRef->SendAttribMsg(DPNSEND_GUARANTEED,
													playerObjectData->m_currentID,
													21,
													0,
													0,
													0,
													0,
													playerObjectData);

				break;
			}

			if(!pObj->m_tradeAccepted)
			{//waiting on other trade partner to confirm
				playerObjectData->m_tradeAccepted = TRUE;//ok with terms
				m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Terms Accepted with "+pObj->m_charName,0,playerObjectData,0);
			}
			else
			{//execute trade
				//reset flags now that trade is done
				pObj->m_tradeAccepted					= FALSE;
				playerObjectData->m_tradeAccepted		= FALSE;
				playerObjectData->m_tradePendingRequest	= -1;
			    playerObjectData->m_tradeTargetID		= -1;
				pObj->m_tradePendingRequest				= -1;
				pObj->m_tradeTargetID					= -1;
				//close menus
				m_sharedInterfaceRef->SendAttribMsg(DPNSEND_GUARANTEED,
													playerObjectData->m_currentID,
													21,
													0,
													0,
													0,
													0,
													playerObjectData);

				m_sharedInterfaceRef->SendAttribMsg(DPNSEND_GUARANTEED,
													pObj->m_currentID,
													21,
													0,
													0,
													0,
													0,
													pObj);
                //first confirm inventory of both offers still are present for secure transaction
				if(playerObjectData->m_tradeOffer)
				   for(POSITION posTemp = playerObjectData->m_tradeOffer->GetHeadPosition();posTemp != NULL;)
				   {
						CInventoryObj * iPtr = (CInventoryObj *)playerObjectData->m_tradeOffer->GetNext(posTemp);
						if(!playerObjectData->m_currentInventory->VerifyInventoryUnarmed(iPtr->m_itemData->m_globalID,iPtr->m_quanity))
						{//trade failed dure to inventory not being avaiable
						   m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Trade was cancelled do to change in " + playerObjectData->m_charName + " inventory",0,playerObjectData,0);
						   m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,pObj->m_currentID,"Trade was cancelled do to change in " + playerObjectData->m_charName + " inventory",0,pObj,0);
						   break;
						}
				   }

				if(pObj->m_tradeOffer)
				   for(POSITION posTemp = pObj->m_tradeOffer->GetHeadPosition();posTemp != NULL;)
				   {
						CInventoryObj * iPtr = (CInventoryObj *)pObj->m_tradeOffer->GetNext(posTemp);
						if(!pObj->m_currentInventory->VerifyInventoryUnarmed(iPtr->m_itemData->m_globalID,iPtr->m_quanity))
						{//trade failed dure to inventory not being avaiable
						   m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Trade was cancelled do to change in " + pObj->m_charName + " inventory",0,playerObjectData,0);
						   m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,pObj->m_currentID,"Trade was cancelled do to change in " + pObj->m_charName + " inventory",0,pObj,0);
						   break;
						}
				   }

			     //at this point we are good to do the transaction
			     //trade inventory
			     if(playerObjectData->m_tradeOffer)
				 {
				   for(POSITION posTemp = playerObjectData->m_tradeOffer->GetHeadPosition();posTemp != NULL;)
				   {
						CInventoryObj * iPtr = (CInventoryObj *)playerObjectData->m_tradeOffer->GetNext(posTemp);
						CInventoryObj * moveItem = playerObjectData->m_currentInventory->GetAndRemoveAmountByGlid(iPtr->m_itemData->m_globalID, iPtr->m_quanity);
						if(moveItem)
                           pObj->m_currentInventory->AddInventoryItem(iPtr->m_quanity,FALSE,moveItem->m_itemData);
				   }
				   playerObjectData->m_tradeOffer->SafeDelete();
				   delete playerObjectData->m_tradeOffer;
				   playerObjectData->m_tradeOffer = 0;
				 }

				 if(pObj->m_tradeOffer)
				 {
				   for(POSITION posTemp = pObj->m_tradeOffer->GetHeadPosition();posTemp != NULL;)
				   {
						CInventoryObj * iPtr = (CInventoryObj *)pObj->m_tradeOffer->GetNext(posTemp);
						CInventoryObj * moveItem = pObj->m_currentInventory->GetAndRemoveAmountByGlid(iPtr->m_itemData->m_globalID, iPtr->m_quanity);
						if(moveItem)
                           playerObjectData->m_currentInventory->AddInventoryItem(iPtr->m_quanity,FALSE,moveItem->m_itemData);
				   }
				   pObj->m_tradeOffer->SafeDelete();
				   delete pObj->m_tradeOffer;
				   pObj->m_tradeOffer = 0;
				 }


				 m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,"Trade Complete",0,playerObjectData,0);
				 m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,pObj->m_currentID,"Trade Complete",0,pObj,0);
			}

			break;
		}
		case SCMD_LOOT: // Request loot (search for dead bodies and return data)
		{
			playerObjectData->m_additionalChatTemp = strArgs;
			m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();//have to unlock here for order
			CAccountObject * acPtrTemp = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountByID_LOS(iFromLocID);
			playerObjectData = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(iNetworkDefinedID);//revalidate
			if(!playerObjectData)
				m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Lock();//have to unlock here for order
			else if(acPtrTemp)
				LootInventoryRequest(playerObjectData,acPtrTemp,iFromLocID);
			if(acPtrTemp)
			   m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			break;
		} // End SCMD_LOOT
		case SCMD_INTERACTION:
		{
			// STORE additional params in the player object
			playerObjectData->m_additionalChatTemp = strArgs;

			// UNLOCK runtime players list
			this->m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();

			// LOCK account database ON SUCCESS
			CAccountObject *pAccount = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountByID_LOS(iFromLocID);

			// LOCK runtime players list ON SUCCESS
			playerObjectData = this->m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(iNetworkDefinedID); 
			
			// HANDLE arena request
			if (!playerObjectData)
			{
				//this->m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,iFromLocID,"NO PLAYER DATA!",0,playerObjectData,0);

				//if(pAccount)
					//m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
				// IF player ID retrieval failed, LOCK runtime players list
				m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Lock();
			}
			else if (pAccount)
			{
				//this->m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,iFromLocID,"WE HAVE A VALID ACCOUNT!",0,playerObjectData,0);
				// GET reference to the quest bot
				bool bIsArena = false;
				CServerItemObj *pServerItem = m_sharedInterfaceRef->m_netComObject->m_runtimeItems->GetItemInRadius_LOSchannel(
					playerObjectData->m_Target3dPosition,
					(playerObjectData->m_dynamicRadius * 2.0f),
					playerObjectData->m_currentWorldID);

				// IF there is no item to interact with, treat this as a LOOT attempt...
				if(!pServerItem)
				{
					//this->m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,iFromLocID,"LOOT INTERACTION HIT!",0,playerObjectData,0);
					// TRY TO HANDLE regular interactions
					this->LootInventoryRequest(playerObjectData, pAccount, iFromLocID);

					// UNLOCK account database
					if(pAccount)
						m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
					break;
				}
				
				// DETERMINE if this is a quest bot or an arena bot
				POSITION pos = NULL;
				for(pos = pServerItem->m_collectionQuestDB->GetHeadPosition(); pos != NULL;)
				{
					// GET reference to quest object
					CAICollectionQuestObj *pQuest = dynamic_cast<CAICollectionQuestObj*>(pServerItem->m_collectionQuestDB->GetNext(pos));
					if (pQuest->m_instanceGenerateType == 2) // Arena instance ID: 2
					{
						bIsArena = true;
					}
				}

				// UNLOCK CServerItemObj pointer collection (for arena bot)
				this->m_sharedInterfaceRef->m_netComObject->m_runtimeItems->Unlock();

				// HANDLE arena registration...
				if (bIsArena)
				{
					//this->m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,iFromLocID,"ARENA HANDLING!",0,playerObjectData,0);

					// SETUP for handling arena request
					ArenaResult result;
					BYTE *pData = NULL;

					// HANDLE request for arena TYPEs
					result = this->HandleRequestArenaTypes(playerObjectData, pAccount, iFromLocID, pData);

					// CHECK result status of any arena handler
					if (result.Result == AS_FAIL)
					{
						//this->m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,iFromLocID,"Arena information is unavailable at this time.",0,playerObjectData,0);
						this->HandleRequestArenaFailed(result, ARENA_TYPE_ID, playerObjectData, pAccount, iFromLocID, pData);
					}

					// DELETE message packet byte array
					SafeDeleteArray(pData);
				}
				// HANDLE regular interaction...
				else
				{
					//this->m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,iFromLocID,"LOOT INTERACTION HIT!",0,playerObjectData,0);
					// HANDLE looting
					this->LootInventoryRequest(playerObjectData, pAccount, iFromLocID);
				}	
			}	

			// UNLOCK account database
			if(pAccount)
				m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			break;
		} // End SCMD_INTERACTION
		case SCMD_REQUESTARENATYPES:
		case SCMD_REQUESTARENAINFO:
		case SCMD_REQUESTARENAJOIN:
		case SCMD_REQUESTARENALEAVE:
		{
			// UNLOCK runtime players list
			this->m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();

			// LOCK account database ON SUCCESS
			CAccountObject *pAccount = m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->GetAccountByID_LOS(iFromLocID);

			// LOCK runtime players list ON SUCCESS
			playerObjectData = this->m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(iNetworkDefinedID); 

			// SETUP for handling arena request
			ArenaResult result;
			BYTE *pData = NULL;
			QUEST_PACKETS ePacketType = ARENA_UNSPECIFIED_ID;

			// HANDLE arena request
			if (!playerObjectData)
			{
				// IF player ID retrieval failed, LOCK runtime players list
				if(pAccount)
					m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
				m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Lock();
				break;
			}
			else if (pAccount)
			{
				// STORE additional params in the player object
				playerObjectData->m_additionalChatTemp = strArgs;

				switch (iCommandID)
				{
					case SCMD_REQUESTARENATYPES:
					{
						// HANDLE request for arena TYPEs
						result = this->HandleRequestArenaTypes(playerObjectData, pAccount, iFromLocID, pData);
						ePacketType = ARENA_TYPE_ID;
						break;
					}
					case SCMD_REQUESTARENAINFO:
					{
						// HANDLE request for arena INFO
						result = this->HandleRequestArenaInfo(playerObjectData, pAccount, iFromLocID, pData);
						ePacketType = ARENA_INFO_ID;
						break;
					}
					case SCMD_REQUESTARENAJOIN:
					{
						// HANDLE request for arena JOIN 
						result = this->HandleRequestArenaJoin(playerObjectData, pAccount, iFromLocID, pData);
//						this->m_sharedInterfaceRef->m_eventLog->LogByPriority("Arena Result for " + playerObjectData->m_charName + ": " + m_sharedInterfaceRef->m_cabLib.ConvToString(result.Result), 2);
						ePacketType = ARENA_JOIN_ID;
						break;
					}
					case SCMD_REQUESTARENALEAVE:
					{
						// HANDLE request for arena LEAVE
						result = this->HandleRequestArenaLeave(playerObjectData, pAccount, iFromLocID, pData);
						ePacketType = ARENA_LEAVE_ID;
						break;
					}
					default: break;
				}
			}

			// CHECK result status of any arena handler
			if ((result.Result != AR_SUCCESS) && pAccount)
			{
				//this->m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,iFromLocID,"Arena information is unavailable at this time.",0,playerObjectData,0);
				this->HandleRequestArenaFailed(result, ePacketType, playerObjectData, pAccount, iFromLocID, pData);
			}

			// DELETE message packet byte array
			SafeDeleteArray(pData);
			
			// UNLOCK account database
			if(pAccount)
				m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->Unlock();
			break;
		} // End SCMD_REQUESTARENATYPES / SCMD_REQUESTARENAINFO / SCMD_REQUESTARENAJOIN / SCMD_REQUESTARENALEAVE
		case SCMD_UPDATEQUICKSLOTS:
		{
			// STORE additional params in the player object
			playerObjectData->m_additionalChatTemp = strArgs;

			// COPY GLIDs of all items in packet into the player's Quick Slots
			int iPos = 0, i = 0, j = 0, iGLID = -1;
			CString strDelimiters, strToken;
			strToken = strArgs.Tokenize(" ", iPos);

			/*for (int i=0; i<NUM_QUICK_ITEM_BARS; i++)
			{
				if (strToken == "") break;
				for (int j=0; j<NUM_QUICK_ITEM_SLOTS; j++)
				{
					if (strToken == "") break;
					iGLID = ::atoi(strToken.GetBuffer());
					strToken.ReleaseBuffer();
					playerObjectData->QuickSlots(i, j, iGLID);
					strToken = strArgs.Tokenize(" ", iPos);
				}
			}*/
			break;
		} // End SCMD_UPDATEQUICKSLOTS
		case SCMD_INVITETOSQUAD:
		{//Send an invite message
			CPlayerObject *playerToInvite = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByNameAlive(strArgs.GetBuffer());
			if(!playerToInvite)
				break;

			if(playerToInvite->m_currentID != 0)//online
			{
				if(!playerToInvite->m_aiControlled)
				{
					BOOL invalid = FALSE;

					//Make sure whose doing the inviting has privledges to do so
					if(playerObjectData->m_groupID > 0 && playerObjectData->m_groupStruct.m_groupLeadership == 0)
					{
						m_sharedInterfaceRef->SendGeneralMessageHandler(94, playerObjectData);
						invalid = TRUE;
					}
					//Make sure they aren't already in our group
					else if(playerObjectData->m_groupID > 0 && playerToInvite->m_groupID == playerObjectData->m_groupID)
					{
						m_sharedInterfaceRef->SendGeneralMessageHandler(96, playerObjectData);
						invalid = TRUE;
					}
					//Make sure the person invited isn't in a group already
					else if(playerToInvite->m_groupID > 0)
					{
						m_sharedInterfaceRef->SendGeneralMessageHandler(95, playerObjectData);
						invalid = TRUE;
					}

					if(!invalid)
					{
						//Make sure we have Info to send
						int iCounter = 0;
						int msgsz = 0;
						int iPlayerNameLength = playerObjectData->m_charName.GetLength();
						int iSquadNameLength = playerObjectData->m_squadName.GetLength();
						BYTE *byteBlock = NULL;
						msgsz += sizeof(short) 
							+ sizeof(char) 
							+ sizeof(char) 
							+ sizeof(char)
							+ iPlayerNameLength
							+ sizeof(char)
							+ iSquadNameLength;
						msgsz++;

						byteBlock = new BYTE[msgsz];
						byteBlock[iCounter++] = 13; //Setup the catagory for HandleMiscBlocksClientSide
						int nullStuff = 0;

						// ENCODE basic squad data
						CopyMemory(&byteBlock[iCounter], &playerObjectData->m_networkAssignedID, sizeof(unsigned short));
						iCounter += sizeof(unsigned short);
						CopyMemory(&byteBlock[iCounter], &nullStuff, sizeof(unsigned char));
						iCounter += sizeof(unsigned char);
						CopyMemory(&byteBlock[iCounter], &nullStuff, sizeof(unsigned char));
						iCounter += sizeof(unsigned char);

						// ENCODE player name data
						CopyMemory(&byteBlock[iCounter], &iPlayerNameLength, sizeof(unsigned char));
						iCounter += sizeof(unsigned char);
						CopyMemory(&byteBlock[iCounter], playerObjectData->m_charName.GetBuffer(), iPlayerNameLength);
						iCounter += iPlayerNameLength;
						playerObjectData->m_charName.ReleaseBuffer();

						// ENCODE squad name data
						CopyMemory(&byteBlock[iCounter], &iSquadNameLength, sizeof(unsigned char));
						iCounter += sizeof(unsigned char);
						CopyMemory(&byteBlock[iCounter], playerObjectData->m_squadName.GetBuffer(), iSquadNameLength);
						iCounter += iSquadNameLength;
						playerObjectData->m_squadName.ReleaseBuffer();

						if(playerToInvite->m_miscOutBox->GetCount() < 20)
							playerToInvite->m_miscOutBox->AddPacket(msgsz,(BYTE*)byteBlock,0,playerToInvite->m_currentWorldID);

						delete [] byteBlock;
						byteBlock = 0;
						
						// Send an invite remind to the player of who they invited.
						m_sharedInterfaceRef->SendTextMsg(	DPNSEND_GUARANTEED,
														playerObjectData->m_currentID,
														"Invited " + playerToInvite->m_charName + " to join your group.",
														0,
														playerObjectData,
														0);
					}
				}
			}
			break;
		}//End send an invite message
		case SCMD_REMOVEFROMSQUAD:
		{
			CPlayerObject *pPlayerToRemove = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByNameAlive(strArgs.GetBuffer());
			if (!pPlayerToRemove) break;

			// ERROR checking
			/*
			if((playerObjectData->m_groupID > 0) 
			&& (playerObjectData->m_groupStruct.m_groupLeadership > 0)
			&& (playerObjectData->m_networkAssignedID != pPlayerToRemove->m_networkAssignedID))
			*/
			if ((playerObjectData->m_groupID <= 0) 
			|| (playerObjectData->m_groupStruct.m_groupLeadership <= 0)
			|| (playerObjectData->m_networkAssignedID == pPlayerToRemove->m_networkAssignedID))
				break;
			
			unsigned char iPlayerPos = 0;
			CAxList *pPlayerList = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetListByGroupID(playerObjectData->m_groupID);
			if(pPlayerToRemove->m_groupID == playerObjectData->m_groupID)
			{
				// RESET the leaving player's squad data
				iPlayerPos = pPlayerToRemove->m_groupStruct.m_groupPlacementIndex;
				pPlayerToRemove->m_groupStruct.m_squadDataIsDirty = 0;
				pPlayerToRemove->m_groupStruct.m_groupLeadership = 0;
				pPlayerToRemove->m_groupStruct.m_groupPlacementIndex = 0;
				pPlayerToRemove->m_groupBlockTracker = -1;
				pPlayerToRemove->m_squadName.Empty();
				pPlayerToRemove->ReassembleName(m_sharedInterfaceRef);//reconstruct
				m_sharedInterfaceRef->SendClientScript(pPlayerToRemove,"UpdateCharName("+pPlayerToRemove->m_visName+")","");
				m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->ReIndexByGroupID(pPlayerToRemove, pPlayerToRemove->m_groupID, 0);
				pPlayerToRemove->m_groupID = 0;
			
				// RE-SORT the remaining players in this squad
				m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->ResortGroupList(playerObjectData->m_groupID, iPlayerPos);

				// SEND out confirmation messages
				m_sharedInterfaceRef->SendTextMsg(
					DPNSEND_GUARANTEED,
					pPlayerToRemove->m_currentID,
					"You have been removed from " + playerObjectData->m_charName + "'s group.",
					0,
					pPlayerToRemove,
					0);

				m_sharedInterfaceRef->SendTextMsg(
					DPNSEND_GUARANTEED,
					playerObjectData->m_currentID,
					"You have removed " + pPlayerToRemove->m_charName + " from your group.",
					0,
					playerObjectData,
					0);

				// CONFIRM squad removal of the leaving player
				//	by sending a NULL'ed squad network packet
				m_sharedInterfaceRef->SendGroupMessage(pPlayerToRemove, NULL, -1);

				// SEND updated squad info to all remaining members
				playerObjectData->m_groupStruct.m_squadDataIsDirty = 1;
				m_sharedInterfaceRef->HandleGroupData(playerObjectData->m_groupID);

				// SEND an update to each player in the group
				for(CAxLinkObj *pos = pPlayerList->GetHeadPosition_unsafe(); pos != NULL;)
				{
					CPlayerObject *pPlayer = (CPlayerObject *)pPlayerList->GetNext_unsafe(&pos);
					if (!pPlayer) continue;

					// ASSEMBLE client script
					CString strScript = "UpdateGroupCFG(" 
						+ m_sharedInterfaceRef->m_cabLib.ConvToString(pPlayerToRemove->m_networkAssignedID) 
						+ "," 
						+ m_sharedInterfaceRef->m_cabLib.ConvToString((m_sharedInterfaceRef->m_groupGlidNotifier * -1)) 
						+ ")";

					// SEND client script
					if(pPlayer->m_currentWorldID == pPlayerToRemove->m_currentWorldID)
						m_sharedInterfaceRef->SendClientScript(pPlayer, strScript, "");
				}
				// Send update to the player who got removed
				for(CAxLinkObj *pos = pPlayerList->GetHeadPosition_unsafe(); pos != NULL;)
				{
					CPlayerObject *pPlayer = (CPlayerObject *)pPlayerList->GetNext_unsafe(&pos);
					if (!pPlayer) continue;

					// ASSEMBLE client script
					CString strScript = "UpdateGroupCFG(" 
						+ m_sharedInterfaceRef->m_cabLib.ConvToString(pPlayerToRemove->m_networkAssignedID) 
						+ "," 
						+ m_sharedInterfaceRef->m_cabLib.ConvToString((m_sharedInterfaceRef->m_groupGlidNotifier * -1)) 
						+ ")";

					// SEND client script
					if(pPlayer->m_currentWorldID == pPlayerToRemove->m_currentWorldID)
						m_sharedInterfaceRef->SendClientScript(pPlayerToRemove, strScript, "");
				}
			}
			break;
		}//End SCMD_REMOVEFROMSQUAD
		case SCMD_GIVEAUTHORITYOFSQUAD:
		{//Give player privledges in group
			CPlayerObject *playerToPromote = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByNameAlive(strArgs.GetBuffer());
			if(!playerToPromote)
				break;

			if((playerObjectData->m_groupID > 0)
			&& (playerObjectData->m_groupStruct.m_groupLeadership == 1)
			&& (playerObjectData->m_networkAssignedID != playerToPromote->m_networkAssignedID))
			{
				if(playerToPromote->m_groupID == playerObjectData->m_groupID)
				{
					playerToPromote->m_groupStruct.m_groupLeadership = 2;
					playerToPromote->m_groupStruct.m_squadDataIsDirty = 1;

					// Send out confirmation messages
					m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
													  playerToPromote->m_currentID,
													  "You have been promoted to a group leader!",
													  0,
													  playerToPromote,
													  0);

					// Transmit updated group info to all members
					m_sharedInterfaceRef->HandleGroupData(playerObjectData->m_groupID);
				}
			}
			break;
		}//End give player privledges in group
		case SCMD_MAKELEADEROFSQUAD:
		{//Promote player to a leader in group
			CPlayerObject *playerToPromote = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByNameAlive(strArgs.GetBuffer());
			if(!playerToPromote)
				break;

			if((playerObjectData->m_groupID > 0)
			&& (playerObjectData->m_groupStruct.m_groupLeadership == 1)
			&& (playerObjectData->m_networkAssignedID != playerToPromote->m_networkAssignedID))
			{
				if(playerToPromote->m_groupID == playerObjectData->m_groupID)
				{
					// Preserve promoted player's position before promotion
					int iOldPosition = playerToPromote->m_groupStruct.m_groupPlacementIndex;;

					// Promote player
					playerToPromote->m_groupStruct.m_groupLeadership = 1;
					playerToPromote->m_groupStruct.m_groupPlacementIndex = 1;
					playerToPromote->m_groupStruct.m_squadDataIsDirty = 1;

					// Demote old leader
					playerObjectData->m_groupStruct.m_groupLeadership = 0;
					playerObjectData->m_groupStruct.m_groupPlacementIndex = iOldPosition;
					playerObjectData->m_groupStruct.m_squadDataIsDirty = 1;

					// Send out confirmation messages
					m_sharedInterfaceRef->SendTextMsg(	DPNSEND_GUARANTEED,
														playerObjectData->m_currentID,
														"You have transferred leadership to " + playerToPromote->m_charName + ".",
														0,
														playerObjectData,
														0);

					m_sharedInterfaceRef->SendTextMsg(	DPNSEND_GUARANTEED,
														playerToPromote->m_currentID,
														"You have received leadership priveleges from " + playerObjectData->m_charName + "!",
														0,
														playerToPromote,
														0);

					// Transmit updated group info to all members
					m_sharedInterfaceRef->HandleGroupData(playerObjectData->m_groupID);
				}
			}
			break;
		}//End promote player to a leader in group
		case SCMD_REQUESTSQUADINFO:
		{
			// Transmit group info to all members
			m_sharedInterfaceRef->SendSquadData_safe(playerObjectData);
			break;
		}
		case SCMD_CREATESQUAD:
		{
			// SAVE the squad name in the CPlayerObject
			playerObjectData->m_squadName = strArgs.GetBuffer();
			if(playerObjectData->m_groupID != 0)
				break;
		
			playerObjectData->m_groupStruct.m_groupLeadership = 1;
			playerObjectData->m_groupStruct.m_groupPlacementIndex = 1;
			playerObjectData->m_groupStruct.m_squadDataIsDirty = 1;
			playerObjectData->m_groupBlockTracker = 0;

			if(!m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->ReIndexByGroupID(playerObjectData, playerObjectData->m_groupID, m_sharedInterfaceRef->m_globalGroupCounter))
				break;

			m_sharedInterfaceRef->m_globalGroupCounter++;
			if(m_sharedInterfaceRef->m_globalGroupCounter == UINT_MAX)
				m_sharedInterfaceRef->m_globalGroupCounter = 1;

			playerObjectData->ReassembleName(m_sharedInterfaceRef);//reconstruct
			m_sharedInterfaceRef->SendClientScript(playerObjectData,"UpdateCharName("+playerObjectData->m_visName+")","");

			m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
											  playerObjectData->m_currentID,
											  "You've created a new group!",
											  0,
											  playerObjectData,
											  0);
			// Transmit updated group info to all members
			m_sharedInterfaceRef->HandleGroupData(playerObjectData->m_groupID);
			break;
		}//End SCMD_CREATESQUAD
		case SCMD_LEAVESQUAD:
		{
			if(playerObjectData->m_groupID <= 0)
				break;
		
			// CHECK if the squad leader is leaving
			unsigned char iPlayerPos = 0;
			unsigned int iPlayerGroupID = playerObjectData->m_groupID;
			bool bNeedNewLeader = false;
			if(playerObjectData->m_groupStruct.m_groupLeadership == 1)
				bNeedNewLeader = true;

			// REMOVE player from the squad
			iPlayerPos = playerObjectData->m_groupStruct.m_groupPlacementIndex;
			playerObjectData->m_groupStruct.m_squadDataIsDirty = 0;
			playerObjectData->m_groupStruct.m_groupLeadership = 0;
			playerObjectData->m_groupStruct.m_groupPlacementIndex = 0;
			playerObjectData->m_groupBlockTracker = -1;
			playerObjectData->m_squadName.Empty();
			playerObjectData->m_groupID = 0;

			// RE-INDEX the players in this group
			m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->ReIndexByGroupID(
				playerObjectData, iPlayerGroupID, 0);
		
			// RE-SORT List
			BOOL bResult = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->ResortGroupList(
				iPlayerGroupID, iPlayerPos);

			// SET first group member's squad status as dirty
			CAxList *pPlayerList = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetListByGroupID(iPlayerGroupID);
			CAxLinkObj *pos = (CAxLinkObj*)pPlayerList->GetHeadPosition_unsafe();
			if(pos)
			{
				// IF the squadleader is leaving, 
				//	SET the first player in the re-sorted squad as the new leader
				CPlayerObject *pMember = (CPlayerObject*)pPlayerList->GetNext_unsafe(&pos);
				pMember->m_groupStruct.m_squadDataIsDirty = 1;
				if (bNeedNewLeader)
					pMember->m_groupStruct.m_groupLeadership = 1;

				// SEND updated group info to all members
				m_sharedInterfaceRef->HandleGroupData(iPlayerGroupID);
			}

			playerObjectData->ReassembleName(m_sharedInterfaceRef);//reconstruct
			m_sharedInterfaceRef->SendClientScript(playerObjectData,"UpdateCharName("+playerObjectData->m_visName+")","");

			// SEND confirmation message to the leaving player
			m_sharedInterfaceRef->SendTextMsg(
				DPNSEND_GUARANTEED,
				playerObjectData->m_currentID,
				"You have successfully left your squad.",
				0,
				playerObjectData,
				0);

			// CONFIRM squad removal of the leaving player
			//	by sending a NULL'ed squad network packet
			m_sharedInterfaceRef->SendGroupMessage(playerObjectData, NULL, -1);

			// SEND an update to each player in the squad
			for (CAxLinkObj *pos = pPlayerList->GetHeadPosition_unsafe(); pos != NULL;)
			{
				CPlayerObject *pPlayer = (CPlayerObject *)pPlayerList->GetNext_unsafe(&pos);
				if (!pPlayer) continue;

				// ASSEMBLE the client script string
				CString strScript = "UpdateGroupCFG(" 
					+ m_sharedInterfaceRef->m_cabLib.ConvToString(playerObjectData->m_networkAssignedID) 
					+ "," 
					+ m_sharedInterfaceRef->m_cabLib.ConvToString((m_sharedInterfaceRef->m_groupGlidNotifier * -1)) 
					+ ")";

				// Make sure we are dealing with only players in the appropriate squad
				if (pPlayer->m_currentWorldID == playerObjectData->m_currentWorldID)
					m_sharedInterfaceRef->SendClientScript(pPlayer, strScript, "");
			}

			// SEND update to the player who got removed
			for (CAxLinkObj *removedLoc = pPlayerList->GetHeadPosition_unsafe(); removedLoc != NULL;)
			{				
				CPlayerObject *pPlayer = (CPlayerObject *)pPlayerList->GetNext_unsafe(&removedLoc);
				if (!pPlayer) continue;

				// ASSEMBLE the client script string
				CString strScript = "UpdateGroupCFG(" 
					+ m_sharedInterfaceRef->m_cabLib.ConvToString(pPlayer->m_networkAssignedID) 
					+ "," 
					+ m_sharedInterfaceRef->m_cabLib.ConvToString((m_sharedInterfaceRef->m_groupGlidNotifier * -1)) 
					+ ")";

				// Make sure we are dealing with only players in the appropriate squad
				if (pPlayer->m_currentWorldID == playerObjectData->m_currentWorldID)
					m_sharedInterfaceRef->SendClientScript(playerObjectData, strScript, "");
			}
			break;
		} //End SCMD_LEAVESQUAD
		case SCMD_REQUESTINVENTORY:
		{
			// PARSE server command params
			CString strToken;
			int iPos = 0;
			std::vector<short> indices;
			while (strToken != "")
			{
				strToken = strArgs.Tokenize(" ", iPos);
				if (strToken == "") break;
				short index = static_cast<short>(atoi(strToken));
				indices.push_back(index);
			}
			if (indices.size() <= 0)
				break;

			int inventoryItemCount = (int)playerObjectData->m_currentInventory->GetCount();
			int totalAssets = (int)this->m_sharedInterfaceRef->m_currencySystem->EvalCurrency(playerObjectData->m_currentInventory);
			this->m_sharedInterfaceRef->SendAttribMsg(DPNSEND_GUARANTEED, playerObjectData->m_currentID, 
				19, totalAssets, 0, 0, 0, playerObjectData);

			// CREATE packet header and SETUP for packet creation
			BYTE *pInventoryData = NULL;
			int iSize = 0;
			BLOCKITEMLIST_STRUCT header;
			header.m_catagorie = 20;
			header.m_miscInt = 0; //visualID;

            // CREATE inventory block
		    if (!playerObjectData->m_currentInventory->GeneratePartialInventoryDataBlock(
				indices, header, &pInventoryData, iSize))
				break;			

			this->m_sharedInterfaceRef->m_netComObject->SendByteBlockMsg(DPNSEND_GUARANTEED,
				playerObjectData->m_currentID, //sendTo,
				pInventoryData,
				iSize,//was iSize+4 - read 4 bytes past the buffer (SendByteBlockMsg frees the buffer itself)
				this->m_sharedInterfaceRef->m_netComObject->m_packetTimeout);
			break;
		} // End SCMD_REQUESTINVENTORY
		case SCMD_SUICIDE:
		{
			//See if we should cancel the current timer
			if(playerObjectData->m_suicideTimeStamp)
			{
				playerObjectData->m_suicideTimeStamp = 0;
				playerObjectData->m_suicideTimer = -1;
				// Send Message confirmation
				m_sharedInterfaceRef->SendTextMsg( DPNSEND_GUARANTEED,
												   playerObjectData->m_currentID,
												   "Countdown Aborted.",
											 	   0,
												   playerObjectData,
												   0);
			}
			else
			{
				playerObjectData->m_suicideTimeStamp = GetTickCount();
				playerObjectData->m_suicideTimer = 45;
				// Send Message confirmation
				m_sharedInterfaceRef->SendTextMsg( DPNSEND_GUARANTEED,
												   playerObjectData->m_currentID,
												   "You have 45 seconds until detonation.  To cancel the suicide, click the suicide button again.",
											 	   0,
												   playerObjectData,
												   0);
			}
			break;
		}
	}
	m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	return TRUE;
}


//------------------------------------------------------------------------------------//
BOOL CTextSystems::LootInventoryRequest(CPlayerObject * looter,CAccountObject * accountPtr,DPNID idFromLoc)
{
 if(!m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer)
    return FALSE;

 if(!looter)
	 return FALSE;
 
 if(looter->m_aiControlled)
	return FALSE;

 if(looter->m_energy < 1)
    return FALSE;

	   if(looter->m_gm == TRUE && looter->m_admin == FALSE)
	      return FALSE;

   BOOL foundSomthing = FALSE;
   BOOL wasBlockedByLootAccess = FALSE;

   CAxList * channelList = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetListByChannel(looter->m_currentWorldID);

   if(channelList)
   for(CAxLinkObj* posLoc = channelList->GetHeadPosition_unsafe();posLoc != NULL;)
   {//all entity loop
       CPlayerObject * pPtr = (CPlayerObject *)channelList->GetNext_unsafe(&posLoc);
       if(pPtr->m_currentWorldID != looter->m_currentWorldID)
		  continue;
	   
	   if(pPtr->m_energy > 0)
		  continue;

	   if(m_sharedInterfaceRef->m_matLib.BoundingSphereCheck(pPtr->m_dynamicRadius,looter->m_dynamicRadius,pPtr->m_Target3dPosition.x,pPtr->m_Target3dPosition.y,pPtr->m_Target3dPosition.z,looter->m_Target3dPosition.x,looter->m_Target3dPosition.y,looter->m_Target3dPosition.z))
	     if(pPtr->LootAccessCheck(looter,m_sharedInterfaceRef->m_lootMode))
	     {//has access
          BOOL proceed = TRUE;
		  if(!pPtr->m_aiControlled)
		  {//PC loot in safe zone protection
			 BOOL chkSafeZn = TRUE;
			 if(pPtr->m_noteriety)
                if(pPtr->m_noteriety->m_currentMurders >= m_sharedInterfaceRef->m_netComObject->m_murderCountMax)
		           chkSafeZn = FALSE;

             if(chkSafeZn)
			 {
			  if(pPtr->m_inSafeZone)
			  {//in safe zone
				 if(pPtr->m_accountNumberRefOld != looter->m_accountNumberRef){//looting somone elses body is not allowed in safe zone
                    proceed = FALSE;
					m_sharedInterfaceRef->SendGeneralMessageHandler(10,looter);
				 }//end looting somone elses body is not allowed in safe zone
			  }//end in safe zone
			 }
		  }//end PC loot in safe zone protection
          if(proceed)
		  {//ok to send loot data
			  m_sharedInterfaceRef->SendInventoryBlockToLootDump(pPtr->m_currentInventory,idFromLoc); 
			  foundSomthing = TRUE;
		  }//ok to send loot data
	     }//end has access
		 else
		    wasBlockedByLootAccess = TRUE;//in the case that the only things left are non lootable by you notify appropriatly
   }//end all entity loop

   CServerItemObj * svrPtr = m_sharedInterfaceRef->m_netComObject->m_runtimeItems->GetItemInRadius_LOSchannel(looter->m_Target3dPosition,(looter->m_dynamicRadius*2.0f),looter->m_currentWorldID);
   if(svrPtr)
   {//found somthin
	   int channelStore = looter->m_currentWorldID;
	   foundSomthing = TRUE;

	     if(svrPtr->m_staticItemType == 0)
		 {//chest container type
	      BOOL cantOpenCauseOfLock = FALSE;
	      if(svrPtr->m_requiredKey != 0)
		  {//key is required to acces this container
			 cantOpenCauseOfLock = TRUE;
			 POSITION invPosLast = NULL;
	         for(POSITION invPos = looter->m_currentInventory->GetHeadPosition();(invPosLast = invPos) != NULL;)
			 {//inventory
	             CInventoryObj * invPtr = (CInventoryObj *)looter->m_currentInventory->GetNext(invPos);
				 if(invPtr->m_itemData->m_keyID == svrPtr->m_requiredKey)
				 {
					svrPtr->m_requiredKey = 0;//unlock perminetly
                    if(invPtr->m_itemData->m_destroyOnUse)
					{//key must be destroyed
				  	   //send handle that you have used a key to unlock this-and the key is destroyed
                       invPtr->m_quanity--;
                       if(invPtr->m_quanity < 1)
					   {
		                  invPtr->SafeDelete();
		                  delete invPtr;
		                  invPtr = 0;
	                      looter->m_currentInventory->RemoveAt(invPosLast);
					   }
					   m_sharedInterfaceRef->SendGeneralMessageHandler(16,looter);
					}
					else//send basic handler that it is now unlocked
					   m_sharedInterfaceRef->SendGeneralMessageHandler(15,looter);
                    cantOpenCauseOfLock = FALSE;
                    break;
				 }
			 }//end inventory
		  }//end key is required to acces this container
		  if(!cantOpenCauseOfLock)
	      if(svrPtr->m_contents)
			m_sharedInterfaceRef->SendInventoryBlockToLootDump(svrPtr->m_contents,idFromLoc); 
		  
		  if(cantOpenCauseOfLock)
		  m_sharedInterfaceRef->SendGeneralMessageHandler(14,looter);
		 }//end chest container type
		 if(svrPtr->m_staticItemType == 1)
		 {//random chat AI/ITEM
            if(svrPtr->m_chatStrings)
			if(svrPtr->m_chatStringCount > 0)
			{//valid
               int selection = rand()%svrPtr->m_chatStringCount;
			   if(selection < svrPtr->m_chatStringCount)
                  m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
						                           idFromLoc,
						                           svrPtr->m_chatStrings[selection],
							                       0,
												   looter,
												   0);//confermation
			}//end valid
		 }//end random chat AI/ITEM
		 if(svrPtr->m_staticItemType == 2)
		 {//sequenced chat AI/ITEM
            if(svrPtr->m_chatStrings)
			if(svrPtr->m_chatStringCount > 0)
			{//valid
			   if(svrPtr->m_chatStringCurIndex >= svrPtr->m_chatStringCount)
				  svrPtr->m_chatStringCurIndex = 0;
                  
				   m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
						                           idFromLoc,
						                           svrPtr->m_chatStrings[svrPtr->m_chatStringCurIndex],
							                       0,
												   looter,
												   0);//confermation
			   svrPtr->m_chatStringCurIndex++;
			}//end valid
		 }//end sequenced chat AI/ITEM
		 if(svrPtr->m_staticItemType == 3)
		 {//quest AI
            if(svrPtr->m_collectionQuestDB)
			if(svrPtr->m_collectionQuestDB->GetCount() > 0)
			{//valid
			   BLOCKQUEST_DATA questBlockHeader;
			   questBlockHeader.m_catagorie = 33;
               questBlockHeader.m_itemAssignedID = svrPtr->m_networkAssignedID;

               int textMenuLength = 0;
			   int showCount = 1;
			   if(looter->m_playerOrigRef)
			   if(looter->m_playerOrigRef->m_questJournal)
			   {
			      CQuestJournalObj * journalEntry = looter->m_playerOrigRef->m_questJournal->GetBySeer(svrPtr->m_displayedName);
				  if(journalEntry)
				  {
                     showCount = journalEntry->m_currentProgression + 1;
					 //auto select option

					 CAICollectionQuestObj * questTemp = (CAICollectionQuestObj *)svrPtr->m_collectionQuestDB->GetByIndex(journalEntry->m_currentProgression);
                    
                     if(!questTemp)
					 {
					    journalEntry->m_currentProgression = 0;//try reset will handle quest scale changes
                        questTemp = (CAICollectionQuestObj *)svrPtr->m_collectionQuestDB->GetByIndex(journalEntry->m_currentProgression);
				     }

					 int startQuestTemp = 0;

					 if(questTemp)
					 if(questTemp->m_progessBasedByQuestJournal)
					 if(!questTemp->m_listProgressionOptions)
					 if(questTemp->m_raceSpecificAccess != -1 )
					 { // Race specific enabled
						 if(questTemp->m_raceSpecificAccess == looter->m_curEDBIndex)
							 startQuestTemp = 1;
					 } // End Race specific enabled
					 else // Race specific NOT enabled
						 startQuestTemp = 1;
						 
					 if(startQuestTemp == 1)
					 { // Start Quest
						int selectionTemp = journalEntry->m_currentProgression;
					    m_questInterface->HandleQuestSys(looter,accountPtr,questTemp,svrPtr,idFromLoc,selectionTemp);
						m_sharedInterfaceRef->m_netComObject->m_runtimeItems->Unlock();
						return TRUE;
					 } // End Start Quest
					 
				  }
			   }
			   int loopTraceTemp = 0;
			   int fullEntryCountLogged = 0;
			   int fullEntryCountSel = 0;
			   POSITION qPos;
			   for(qPos = svrPtr->m_collectionQuestDB->GetHeadPosition();qPos != NULL;)
			   {//aquire full data length
                   CAICollectionQuestObj * questPtr = (CAICollectionQuestObj *)svrPtr->m_collectionQuestDB->GetNext(qPos);
                   if(questPtr->m_raceSpecificAccess != -1)
				   {//racial Filter enabled
                      if(questPtr->m_raceSpecificAccess != looter->m_curEDBIndex)
					  {
					     loopTraceTemp++;
                         continue;
					  }
				   }//end racial Filter enabled
                   
				   if(questPtr->m_progessBasedByQuestJournal)
                   if(loopTraceTemp >= showCount)
				   {
                      continue;
				   }

                   textMenuLength += questPtr->m_title.GetLength()+1;
				   fullEntryCountSel = loopTraceTemp;
				   loopTraceTemp++;
				   fullEntryCountLogged++;
			   }//end aquire full data length

			   if(fullEntryCountLogged == 1)
			   {//only one selection-auto slect this one
			     CAICollectionQuestObj * questTemp = (CAICollectionQuestObj *)svrPtr->m_collectionQuestDB->GetByIndex(fullEntryCountSel);
			     if(questTemp)
			     {
					    m_questInterface->HandleQuestSys(looter,accountPtr,questTemp,svrPtr,idFromLoc,fullEntryCountSel);
						m_sharedInterfaceRef->m_netComObject->m_runtimeItems->Unlock();
						return TRUE;
			     }
			   }//end only one selection-auto slect this one

			   if(textMenuLength > 0)
			   {//somthing to send
                int msgSize = sizeof(BLOCKQUEST_DATA)+textMenuLength;
                BYTE * blockData = new BYTE[msgSize];
                CopyMemory(blockData,&questBlockHeader,sizeof(BLOCKQUEST_DATA));
                int traceDataBuild = sizeof(BLOCKQUEST_DATA);

				int loopTraceTemp = 0;
			    for(qPos = svrPtr->m_collectionQuestDB->GetHeadPosition();qPos != NULL;)
			    {//fill text data
                   CAICollectionQuestObj * questPtr = (CAICollectionQuestObj *)svrPtr->m_collectionQuestDB->GetNext(qPos);
                   if(questPtr->m_raceSpecificAccess != -1)
				   {//racial Filter enabled
                      if(questPtr->m_raceSpecificAccess != looter->m_curEDBIndex)
					  {
					     loopTraceTemp++;
                         continue;
					  }
				   }//end racial Filter enabled
				   
				   if(questPtr->m_progessBasedByQuestJournal)
                   if(loopTraceTemp >= showCount)
				   {
                      continue;
				   }
				   
				   blockData[traceDataBuild] = questPtr->m_title.GetLength();
   				   traceDataBuild++;
				   if(questPtr->m_title.GetLength() > 0)
					   CopyMemory(&blockData[traceDataBuild],questPtr->m_title.GetBuffer(),questPtr->m_title.GetLength());

                   traceDataBuild += questPtr->m_title.GetLength();
				   loopTraceTemp++;
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
			    CString ignoreMsg = svrPtr->m_noResponseMsg;
                m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
						                         idFromLoc,
						                         ignoreMsg,
							                     0,
												 looter,
											     0);//confermation

			   }//end send ignoring msg
			}//end valid
		 }//end quest AI
		 m_sharedInterfaceRef->m_netComObject->m_runtimeItems->Unlock();
   }//end found somthin
   if(!foundSomthing)
   {
	   if(wasBlockedByLootAccess)
	   {
          CString noaccessMsg = "You didnt kill this.";
          m_sharedInterfaceRef->SendTextMsg( DPNSEND_GUARANTEED,
					                         idFromLoc,
					                         noaccessMsg,
						                     0,
											 looter,
										     0);//confermation
	   }
	   else
	      m_sharedInterfaceRef->SendGeneralMessageHandler(17,looter);
   }
   
   return TRUE;
}
//--------------Handles system messages-----------------------------------------------//
BOOL CTextSystems::HandleTextMessageServerSideGroupStyle_safe(	LPVOID lpMessage,
																int & msgSize,
																DPID & idFromLoc)
{
	 int stringLength = msgSize - 4;
	 if(stringLength <= 0 || stringLength > 1000)
		return FALSE;
	 BYTE * dataBlock = (BYTE*)lpMessage;
	 LPTXT_MSG castEntry = (LPTXT_MSG)lpMessage;
	 short networkDefinedID = castEntry->referenceID;
     CopyMemory(m_bufferTemp,&dataBlock[4],stringLength);
	 m_bufferTemp[stringLength] = 0;

      CPlayerObject * playerObjectData = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(networkDefinedID);
	  if(playerObjectData)
	  {
	   if(!playerObjectData->m_mute)
	   {//found one 
	     CString cnvs = playerObjectData->m_charName+">";
		 CString additional = m_bufferTemp;
		 cnvs = cnvs+additional;
         //handle text commands
		 
         //parse
		 if(castEntry->m_catagorie == 10)//group
		    SendTextMessageToGroup(playerObjectData,cnvs);
	   }//end found one
	   m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	  }
	  

	   return TRUE;
}
//------------------------------------------------------------------------------------//
void CTextSystems::SendTextMessageToGroup(CPlayerObject * pPtr,CString message)
{
	//Not in a group, don't send anything
	if(pPtr->m_groupID <= 0)
		return;
	
	//Send the message to all players in the group
	CAxList *playerList = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetListByGroupID(pPtr->m_groupID); //Already locked by this point
	//If we have a player list, start sending the message
	if(playerList)
	{
		// Loop through the players
		for(CAxLinkObj *posLoc = playerList->GetHeadPosition_unsafe(); posLoc != NULL;)
		{
			// Make sure we're in the same group
			CPlayerObject *player = (CPlayerObject *)playerList->GetNext_unsafe(&posLoc);
			if(player->m_groupID == pPtr->m_groupID)
				SendMsgByHandle(player->m_handle,message,1);
		}
	}
}
//------------------------------------------------------------------------------------//
void CTextSystems::SendMsgByHandle( int handle,
									CString message,
									int type)//0 radius = to all
{

   CPlayerObject * lstObj = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandle(handle);
   if(!lstObj)
	   return;

   if(lstObj->m_aiControlled)
	  return;

   if(lstObj->m_currentID == 0)
	  return;
         
   m_sharedInterfaceRef->SendTextMsg( DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW, 
									  lstObj->m_currentID,
									  message,
									  lstObj->m_networkAssignedID,
									  lstObj,
									  type);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CTextSystems
// Function:	HandleRequestArenaTypes(CPlayerObject, CAccountObject, DPNID, BYTE*)
// Description:	-
///////////////////////////////////////////////////////////////////////////////

ArenaResult
CTextSystems::HandleRequestArenaTypes(CPlayerObject *pPlayer, 
									 CAccountObject *pAccount, 
									 DPNID iFromID,
									 BYTE *pData)
{
	// GET reference to the arena bot
	CServerItemObj *pArenaBot = m_sharedInterfaceRef->m_netComObject->m_runtimeItems->GetItemInRadius_LOSchannel(
		pPlayer->m_Target3dPosition,
		(pPlayer->m_dynamicRadius * 2.0f),
		pPlayer->m_currentWorldID);

	// ERROR checking
	ArenaResult result;
	if(!pArenaBot)
	{
		result.Severity = (ARENA_SEVERITY)(result.Severity | AS_FAIL);
		result.Result = (ARENA_RESULTS)(result.Result | AR_COULD_NOT_FIND_ARENA_BOT);
		return result;
	}

	// SETUP for assembling the response packet
	POSITION pos = NULL;
	unsigned short iType = 0;
	unsigned short *pArenaTypes = new unsigned short[ARENATYPE_MAX - 1];
	::memset(pArenaTypes, 0, (sizeof(short) * (ARENATYPE_MAX - 1)));
	
	// ASSEMBLE the response packet
	for(pos = pArenaBot->m_collectionQuestDB->GetHeadPosition(); pos != NULL;)
    {
		// GET reference to arena quest object
		CAICollectionQuestObj *pArena = dynamic_cast<CAICollectionQuestObj*>(pArenaBot->m_collectionQuestDB->GetNext(pos));
		if (!pArena) continue;

		// CHECK for racial filter
		if ((pArena->m_raceSpecificAccess != -1)
		&& (pArena->m_raceSpecificAccess != pPlayer->m_curEDBIndex))
			continue;

		// ERROR checking
		if (!pArena->m_rules					// REQUIRE world rules for arenas
		|| pArena->m_instanceGenerateType != 2)	// Arena instance ID: 2
			continue;

		CString debug;

		// ENCODE the arena type
		// NOTE: The high order byte of a short is used to encode the SOLO arena type
		//	while the low order byte of a short is used to encode the TEAM arena type.
		//	Offset by -1 because the first ARENA_TYPES value is for the UNSPECIFIED condition.
		for (int i = 0; i < pArena->m_ruleCount; i++)
		{
			switch (pArena->m_rules[i].m_rule)
			{
				// SOLO PLAYER ELIMINATION
				case WORLDRULE_ELIMINATION:
				{
					iType = ARENATYPE_PLAYERELIMINATION;
					iType <<= 8;
					pArenaTypes[ARENATYPE_PLAYERELIMINATION-1] |= iType;
					break;
				}
				// TEAM PLAYER ELIMINATION
				case WORLDRULE_TEAM_ELIMINATION:
				{
					iType = ARENATYPE_PLAYERELIMINATION;
					pArenaTypes[ARENATYPE_PLAYERELIMINATION-1] |= iType;
					break;
				}
				// SOLO DEATHMATCH
				case WORLDRULE_DEATHMATCH:
				{
					iType = ARENATYPE_DEATHMATCH;
					iType <<= 8;
					pArenaTypes[ARENATYPE_DEATHMATCH-1] |= iType;
					break;
				}
				// TEAM DEATHMATCH
				case WORLDRULE_TEAM_DEATHMATCH:
				{
					iType = ARENATYPE_DEATHMATCH;
					pArenaTypes[ARENATYPE_DEATHMATCH-1] |= iType;
					break;
				}
				// SOLO DOMINATION
				case WORLDRULE_DOMINATION:
				{
					iType = ARENATYPE_DOMINATION;
					iType <<= 8;
					pArenaTypes[ARENATYPE_DOMINATION-1] |= iType;
					break;
				}
				// TEAM DOMINATION
				case WORLDRULE_TEAM_DOMINATION:
				{
					iType = ARENATYPE_DOMINATION;
					pArenaTypes[ARENATYPE_DOMINATION-1] |= iType;
					break;
				}
				// SOLO MOB ELIMINATION
				case WORLDRULE_ELIMINATEMOBS_ARENA:
				{
					iType = ARENATYPE_MOBELIMINATION;
					iType <<= 8;
					pArenaTypes[ARENATYPE_MOBELIMINATION-1] |= iType;
					break;
				}
			} // END switch on arena type
		} // END encode arena type
    } // END assemble response packet

	// CREATE the ARENA TYPE REQUEST response packet
	BLOCKQUEST_DATA HeaderPacket;
	HeaderPacket.m_catagorie = 33;
	HeaderPacket.m_itemAssignedID = 0;

	// COPY message header into packet
	SafeDeleteArray(pData);
	int iSize = sizeof(BLOCKQUEST_DATA)				// Header 
		+ sizeof(ARENA_SEVERITY)				// ArenaResult struct
		+ sizeof(ARENA_RESULTS)
		+ sizeof(char)								// Packet ID
		+ (sizeof(short) * (ARENATYPE_MAX - 1));	// Arena TYPEs
	pData = new BYTE[iSize];
	int iPos = 0;
	::memcpy(&pData[iPos], &HeaderPacket, sizeof(BLOCKQUEST_DATA));
	iPos = sizeof(BLOCKQUEST_DATA);

	// COPY packet ID into the packet
	unsigned char iPacketID = ARENA_TYPE_ID;
	::memcpy(&pData[iPos], &iPacketID, sizeof(char));
	iPos += sizeof(char);

	// COPY result data into the packet
	::memcpy(&pData[iPos], &result.Severity, sizeof(ARENA_SEVERITY));
	iPos += sizeof(ARENA_SEVERITY);
	::memcpy(&pData[iPos], &result.Result, sizeof(ARENA_RESULTS));
	iPos += sizeof(ARENA_RESULTS);

	// COPY encoded arena TYPEs into packet
	for (int i=0; i<(ARENATYPE_MAX-1); i++)
	{
		::memcpy(&pData[iPos], &pArenaTypes[i], sizeof(short));
		iPos += sizeof(short);
	}

	// SEND response packet to requesting player
	this->m_sharedInterfaceRef->m_netComObject->SendData(pData,
         iSize,
         DPNSEND_GUARANTEED,
         iFromID,
		 m_sharedInterfaceRef->m_netComObject->m_packetTimeout);
	delete [] pData;//SendData copies the packet - free ours (was leaked every arena reply)
	pData = 0;

	// UNLOCK CServerItemObj pointer collection (for arena bot)
	m_sharedInterfaceRef->m_netComObject->m_runtimeItems->Unlock();
	return result;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CTextSystems
// Function:	HandleRequestArenaInfo(CPlayerObject, CAccountObject, DPNID, BYTE*)
// Description:	-
///////////////////////////////////////////////////////////////////////////////

ArenaResult
CTextSystems::HandleRequestArenaInfo(CPlayerObject *pPlayer, 
									 CAccountObject *pAccount, 
									 DPNID iFromID,
									 BYTE *pData)
{
	ArenaResult result;

	// GET reference to the arena bot
	CServerItemObj *pArenaBot = m_sharedInterfaceRef->m_netComObject->m_runtimeItems->GetItemInRadius_LOSchannel(
		pPlayer->m_Target3dPosition,
		(pPlayer->m_dynamicRadius * 2.0f),
		pPlayer->m_currentWorldID);

	// ERROR checking
	if(!pArenaBot)
	{
		result.Severity = (ARENA_SEVERITY)(result.Severity | AS_FAIL);
		result.Result = (ARENA_RESULTS)(result.Result | AR_COULD_NOT_FIND_ARENA_BOT);
		return result;
	}

	// Parse the server command parameters (arena MODE and TYPE)
	int iPos = 0;
	CString strParams[2];
	strParams[0] = pPlayer->m_additionalChatTemp.Tokenize(" ", iPos); // Arena MODE
	strParams[1] = pPlayer->m_additionalChatTemp.Tokenize(" ", iPos); // Arena TYPE

	// SETUP for assembling the response packet
	int iSize = 0;
	ARENA_MODES eMode;
	ARENA_TYPES eType;
	POSITION pos = NULL;
	ArenaInfoEX *pArenaInfo = NULL;
	vector<ArenaInfoEX*> Arenas;

	// CAST the server command params to their enum values
	eMode = (ARENA_MODES)::atoi(strParams[0]);
	eType = (ARENA_TYPES)::atoi(strParams[1]);
	
	// RETRIEVE info for all arenas of the requested MODE and TYPE
	for(pos = pArenaBot->m_collectionQuestDB->GetHeadPosition(); pos != NULL;)
    {
		// GET reference to arena quest object
		CAICollectionQuestObj *pArena = dynamic_cast<CAICollectionQuestObj*>(pArenaBot->m_collectionQuestDB->GetNext(pos));
		if (!pArena) continue;

		// ERROR checking: Is this a valid arena?
		if (!pArena->m_rules						// REQUIRE world rules for arenas
		|| pArena->m_instanceGenerateType != 2)		// Arena instance ID: 2
			continue;

		// ERROR checking: Does this player's race satisfy our filter check?
		if ((pArena->m_raceSpecificAccess != -1)
		&& (pArena->m_raceSpecificAccess != pPlayer->m_curEDBIndex))
			continue;

		// ERROR checking: Does this arena satisfy our filter check?
		bool bFilterMatch = false;
		if (eMode == ARENAMODE_SOLO)
		{
			for (int i=0; i<pArena->m_ruleCount; i++)
			{
				if (pArena->m_rules[i].m_rule == SOLO_ARENATYPES_TO_WORLDRULE_MAP[eType])
				{
					bFilterMatch = true;
					break;
				}
			}
		}
		else if (eMode == ARENAMODE_TEAM)
		{
			for (int i=0; i<pArena->m_ruleCount; i++)
			{
				if (pArena->m_rules[i].m_rule == TEAM_ARENATYPES_TO_WORLDRULE_MAP[eType])
				{
					bFilterMatch = true;
					break;
				}
			}
		}
		if (!bFilterMatch) continue;

		// CALCULATE time remaining for arena autostart (if necessary)
		int iTimeRemainingForAutostart = 0;
		if (pArena->m_instanceGenerateParam4 > 0)
		{
			int iTimeElapsed = ::GetTickCount() - pArena->m_arenaStartTimerStamp;
			iTimeRemainingForAutostart = pArena->m_instanceGenerateParam4 - iTimeElapsed;
		}
		if (iTimeRemainingForAutostart < 0) 
			iTimeRemainingForAutostart = 0;

		// POPULATE base arena info
		pArenaInfo = new ArenaInfoEX();
		pArenaInfo->Index = static_cast<unsigned short>(pArena->m_index);
		pArenaInfo->Name = pArena->m_title;
		pArenaInfo->WorldName = pArena->m_spawnToWorldName;
		pArenaInfo->NumCurrentPlayers = pArena->DWMEM_getCount();
		pArenaInfo->NumMinPlayers = pArena->m_instanceGenerateParam3;
		pArenaInfo->NumMaxPlayers = pArena->m_instanceGenerateParam;
		pArenaInfo->TimeRemainingForAutostart = iTimeRemainingForAutostart;
		pArenaInfo->BetPool = pArena->m_arenaBetPool;

		// UPDATE size tracking for final packet memory allocation
		iSize += sizeof(int) // Name length
			+ (sizeof(TCHAR) * pArenaInfo->Name.GetLength()) // Name
			+ sizeof(int) // World name length
			+ (sizeof(TCHAR) * pArenaInfo->WorldName.GetLength()) // World Name
			+ sizeof(char)	// Mode
			+ sizeof(char)	// Type
			+ sizeof(short)	// Index
			+ sizeof(short)	// NumCurrentPlayers
			+ sizeof(short)	// NumMinPlayers
			+ sizeof(short)	// NumMaxPlayers
			+ sizeof(int)	// TimeRemainingForAutostart
			+ sizeof(int)	// TimeLimit
			+ sizeof(int)	// Bet Pool
			+ sizeof(char);	// NumTeams

		// POPULATE the extended arena info
		for (int i=0; i<pArena->m_ruleCount; i++)
		{
			switch (pArena->m_rules[i].m_rule)
			{
				// SOLO PLAYER ELIMINATION
				case WORLDRULE_ELIMINATION:
				{
					pArenaInfo->Mode = ARENAMODE_SOLO;
					pArenaInfo->Type = ARENATYPE_PLAYERELIMINATION;
					break;
				}
				// TEAM PLAYER ELIMINATION
				case WORLDRULE_TEAM_ELIMINATION:
				{

					pArenaInfo->Mode = ARENAMODE_TEAM;
					pArenaInfo->Type = ARENATYPE_PLAYERELIMINATION;
					pArenaInfo->NumTeams = pArena->m_rules[i].m_param;
					break;
				}
				// SOLO DEATHMATCH
				case WORLDRULE_DEATHMATCH:
				{
					pArenaInfo->Mode = ARENAMODE_SOLO;
					pArenaInfo->Type = ARENATYPE_DEATHMATCH;
					break;
				}
				// TEAM DEATHMATCH
				case WORLDRULE_TEAM_DEATHMATCH:
				{
					pArenaInfo->Mode = ARENAMODE_TEAM;
					pArenaInfo->Type = ARENATYPE_DEATHMATCH;
					pArenaInfo->NumTeams = pArena->m_rules[i].m_param;
					break;
				}
				// SOLO DOMINATION
				case WORLDRULE_DOMINATION:
				{
					pArenaInfo->Mode = ARENAMODE_SOLO;
					pArenaInfo->Type = ARENATYPE_DOMINATION;
					break;
				}
				// TEAM DOMINATION
				case WORLDRULE_TEAM_DOMINATION:
				{
					pArenaInfo->Mode = ARENAMODE_TEAM;
					pArenaInfo->Type = ARENATYPE_DOMINATION;
					pArenaInfo->NumTeams = pArena->m_rules[i].m_param;
					break;
				}
				// SOLO MOB ELIMINATION
				case WORLDRULE_ELIMINATEMOBS_ARENA:
				{
					pArenaInfo->Mode = ARENAMODE_SOLO;
					pArenaInfo->Type = ARENATYPE_MOBELIMINATION;
					break;
				}
				case WORLDRULE_TIMELIMIT:
				{
					pArenaInfo->TimeLimit = pArena->m_rules[i].m_param;
					break;
				}
				default: break;
			} // END switch on rule type

			// POPULATE team info as necessary
			if (pArenaInfo->NumTeams > 0)
			{
				// UPDATE packet size tracker
				iSize += pArenaInfo->NumTeams * sizeof(short);

				// CREATE number of players per team collection
				pArenaInfo->NumPlayersOnTeam = new unsigned short[pArenaInfo->NumTeams];
				::memset(pArenaInfo->NumPlayersOnTeam, 0, (pArenaInfo->NumTeams * sizeof(short)));

				// LOOP through player collection for this CAICollectionQuestObj
				//	to determine team counts
				for (int i=0; i < pArena->m_dwMemCount; i++)
				{
					CPlayerObject *pTempPlayer = this->m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandle(pArena->m_dwMem[i]);
					if (pTempPlayer
					&& (pTempPlayer->m_energy > 0)		// Player is alive
					&& (pTempPlayer->m_teamTemp > -1)	// Player is on a valid team
					&& (pTempPlayer->m_teamTemp < pArenaInfo->NumTeams)) // Bounds checking
					{
						pArenaInfo->NumPlayersOnTeam[pTempPlayer->m_teamTemp]++;
					}
				} // END player collection loop
			}// END populating team info
		} // END populating extended arena info

		// STORE arena info into collection
		Arenas.push_back(pArenaInfo);
	} // END retrieve info for all arenas

	// SETUP for final response packet encoding
	iPos = 0;

	// CREATE the ARENA TYPE REQUEST response packet
	BLOCKQUEST_DATA HeaderPacket;
	HeaderPacket.m_catagorie = 33;
	HeaderPacket.m_itemAssignedID = 0;

	// COPY message header into packet
	SafeDeleteArray(pData);
	iSize += sizeof(BLOCKQUEST_DATA)	// Header 
		+ sizeof(ARENA_SEVERITY)	// ArenaResult struct
		+ sizeof(ARENA_RESULTS)
		+ sizeof(char)					// Packet ID
		+ sizeof(short);				// Num of arenas
	pData = new BYTE[iSize];
	::memcpy(&pData[iPos], &HeaderPacket, sizeof(BLOCKQUEST_DATA));
	iPos = sizeof(BLOCKQUEST_DATA);

	// COPY packet ID into the packet
	QUEST_PACKETS ePacket = ARENA_INFO_ID;
	::memcpy(&pData[iPos], &ePacket, sizeof(QUEST_PACKETS));
	iPos += sizeof(QUEST_PACKETS);

	// COPY arena results into the packet
	::memcpy(&pData[iPos], &result.Severity, sizeof(ARENA_SEVERITY));
	iPos += sizeof(ARENA_SEVERITY);
	::memcpy(&pData[iPos], &result.Result, sizeof(ARENA_RESULTS));
	iPos += sizeof(ARENA_RESULTS);

	// COPY number of arenas into packet
	unsigned short iNumArenas = Arenas.size();
	memcpy(&pData[iPos], &iNumArenas, sizeof(short));
	iPos += sizeof(short);

	// ENCODE final response packet
	vector<ArenaInfoEX*>::iterator iter;
	for(iter = Arenas.begin(); iter != Arenas.end(); iter++)
	{
		// GET a reference to the ArenaInfoEX object
		pArenaInfo = (ArenaInfoEX*)(*iter);

		// Index
		::memcpy(&pData[iPos], &pArenaInfo->Index, sizeof(short));
		iPos += sizeof(short);

		// Mode
		::memcpy(&pData[iPos], &pArenaInfo->Mode, sizeof(ARENA_MODES));
		iPos += sizeof(ARENA_MODES);

		// Type
		::memcpy(&pData[iPos], &pArenaInfo->Type, sizeof(ARENA_TYPES));
		iPos += sizeof(ARENA_TYPES);

		// Name length
		int iLength = pArenaInfo->Name.GetLength();
		::memcpy(&pData[iPos], &iLength, sizeof(int));
		iPos += sizeof(int);

		// Name
		LPTSTR strName = pArenaInfo->Name.GetBuffer();
		::memcpy(&pData[iPos], strName, sizeof(TCHAR) * iLength);
		pArenaInfo->Name.ReleaseBuffer();
		iPos += sizeof(TCHAR) * iLength;

		// World name length
		iLength = pArenaInfo->WorldName.GetLength();
		::memcpy(&pData[iPos], &iLength, sizeof(int));
		iPos += sizeof(int);

		// World name
		strName = pArenaInfo->WorldName.GetBuffer();
		::memcpy(&pData[iPos], strName, sizeof(TCHAR) * iLength);
		pArenaInfo->WorldName.ReleaseBuffer();
		iPos += sizeof(TCHAR) * iLength;

		// Min players
		::memcpy(&pData[iPos], &pArenaInfo->NumMinPlayers, sizeof(short));
		iPos += sizeof(short);

		// Max players
		::memcpy(&pData[iPos], &pArenaInfo->NumMaxPlayers, sizeof(short));
		iPos += sizeof(short);

		// Current players
		::memcpy(&pData[iPos], &pArenaInfo->NumCurrentPlayers, sizeof(short));
		iPos += sizeof(short);

		// Time limit
		::memcpy(&pData[iPos], &pArenaInfo->TimeLimit, sizeof(int));
		iPos += sizeof(int);

		// Arena begin countdown timestamp
		::memcpy(&pData[iPos], &pArenaInfo->TimeRemainingForAutostart, sizeof(int));
		iPos += sizeof(int);

		// Current Bet Pool
		::memcpy(&pData[iPos], &pArenaInfo->BetPool, sizeof(int));
		iPos += sizeof(int);

		// Num teams
		::memcpy(&pData[iPos], &pArenaInfo->NumTeams, sizeof(char));
		iPos += sizeof(char);

		// Num players per team
		if ((pArenaInfo->NumTeams > 0)
		&& (pArenaInfo->NumPlayersOnTeam))
		{
			for (int i=0; i<pArenaInfo->NumTeams; i++)
			{
				::memcpy(&pData[iPos], &pArenaInfo->NumPlayersOnTeam[i], sizeof(short));
				iPos += sizeof(short);
			}
		}
	} // END encode final response packet

	// SEND response packet to requesting player
	this->m_sharedInterfaceRef->m_netComObject->SendData(pData,
         iSize,
         DPNSEND_GUARANTEED,
         iFromID,
		 m_sharedInterfaceRef->m_netComObject->m_packetTimeout);
	delete [] pData;//SendData copies the packet - free ours (was leaked every arena reply)
	pData = 0;

	// UNLOCK CServerItemObj pointer collection (for arena bot)
	m_sharedInterfaceRef->m_netComObject->m_runtimeItems->Unlock();
	return result;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CTextSystems
// Function:	HandleRequestArenaJoin(CPlayerObject, CAccountObject, DPNID, BYTE*)
// Description:	-
///////////////////////////////////////////////////////////////////////////////

ArenaResult
CTextSystems::HandleRequestArenaJoin(CPlayerObject *pPlayer, 
									 CAccountObject *pAccount, 
									 DPNID iFromID,
									 BYTE *pData)
{
	ArenaResult result;

	// ERROR checking: Do we have a valid player and account object?
	if (!pPlayer || !pAccount) 
	{
		result.Severity = (ARENA_SEVERITY)(result.Severity | AS_FAIL);
		result.Result = (ARENA_RESULTS)(result.Result | AR_INVALID_PLAYER_OR_ACCOUNT);
		return result;
	}

	// PARSE the server command parameters (arena INDEX and TEAM INDEX)
	bool bResult = false;
	int iPos = 0, 
		iArenaBotID = -1, 
		iArenaIndex = 0, 
		iTeamIndex = 0,
		iBetAmount = 0;
	CString strParams[4];
	strParams[0] = pPlayer->m_additionalChatTemp.Tokenize(" ", iPos); // Arena BOT ID
	if (iPos != -1)
		strParams[1] = pPlayer->m_additionalChatTemp.Tokenize(" ", iPos); // Arena INDEX
	if (iPos != -1)
		strParams[2] = pPlayer->m_additionalChatTemp.Tokenize(" ", iPos); // Arena TEAM INDEX
	if (iPos != -1)
		strParams[3] = pPlayer->m_additionalChatTemp.Tokenize(" ", iPos); // Bet AMOUNT

	// CAST the params to their intended data type
	//	with error checking in case of no teams
	if (strParams[0] != "")
	{
		iArenaBotID = ::atoi(strParams[0].GetBuffer());
		strParams[0].ReleaseBuffer();
	}
	if (strParams[1] != "")
	{
		iArenaIndex = ::atoi(strParams[1].GetBuffer());
		strParams[1].ReleaseBuffer();
	}
	if (strParams[2] != "")
	{
		iTeamIndex = ::atoi(strParams[2].GetBuffer());
		strParams[2].ReleaseBuffer();
	}
	if (strParams[3] != "")
	{
		iBetAmount = ::atoi(strParams[3].GetBuffer());
		strParams[3].ReleaseBuffer();
	}

	// GET reference to the arena bot nearest to the player
	CServerItemObj *pNearestArenaBot = m_sharedInterfaceRef->m_netComObject->m_runtimeItems->GetItemInRadius_LOSchannel(
		pPlayer->m_Target3dPosition,
		(pPlayer->m_dynamicRadius * 2.0f),
		pPlayer->m_currentWorldID);

	// ERROR checking: Do we have a valid reference to a nearest arena bot?
	if(!pNearestArenaBot)
	{
		result.Severity = (ARENA_SEVERITY)(result.Severity | AS_FAIL);
		result.Result = (ARENA_RESULTS)(result.Result | AR_COULD_NOT_FIND_ARENA_BOT);
		return result;
	}

	// GET reference to the arena bot the player is currently registered with
	CServerItemObj *pRegisteredArenaBot = NULL;
	if (iArenaBotID != -1)
		pRegisteredArenaBot = m_sharedInterfaceRef->m_netComObject->m_runtimeItems->GetObjectBySpawnID(iArenaBotID);

	// LOOP through all arenas hosted at the arena bot the player is already registered with (if any)
	POSITION pos = NULL;
	CAICollectionQuestObj *pQuest = NULL, *pArena = NULL;
	if (pRegisteredArenaBot)
	{
		for (pos = pRegisteredArenaBot->m_collectionQuestDB->GetHeadPosition(); pos != NULL;)
		{
			// GET a reference to the current quest object
			pQuest = dynamic_cast<CAICollectionQuestObj*>(pRegisteredArenaBot->m_collectionQuestDB->GetNext(pos));

			// CHECK if the player is already registered for another arena. 
			if (pQuest && pQuest->VerifyInDWMEM(pPlayer->m_handle))
			{
				// ERROR checking: Is the player signing up for the same arena AND the same team at the same bot?
				//	IF so, return TRUE since no changes need to be made.
				if ((pRegisteredArenaBot == pNearestArenaBot)
				&& (pQuest->m_index == iArenaIndex) 
				&& (pPlayer->m_teamTemp == iTeamIndex))
				{
					// UNLOCK CServerItemObj pointer collection (for arena bot)
					m_sharedInterfaceRef->m_netComObject->m_runtimeItems->Unlock();
					{
						result.Severity = (ARENA_SEVERITY)(result.Severity | AS_SUCCESS);
						result.Result = (ARENA_RESULTS)(result.Result | AR_SUCCESS);
						return result;
					}
				}

				//	UNREGISTER the player.
				bResult = pQuest->RemoveFromDWMEM(pPlayer->m_handle);

				// FAILED to unregister successfully.
				if (!bResult) 
				{
					// UNLOCK CServerItemObj pointer collection (for arena bot)
					m_sharedInterfaceRef->m_netComObject->m_runtimeItems->Unlock();
					result.Severity = (ARENA_SEVERITY)(result.Severity | AS_FAIL);
					result.Result = (ARENA_RESULTS)(result.Result | AR_JOIN_FAILED);
					return result;
				}
			}
		}
	}

	// GET a reference to the arena the player is trying to sign up for
	pArena = pNearestArenaBot->m_collectionQuestDB->GetByIndex(iArenaIndex);
	if (!pArena) 
	{
		// UNLOCK CServerItemObj pointer collection (for arena bot)
		m_sharedInterfaceRef->m_netComObject->m_runtimeItems->Unlock();
		result.Severity = (ARENA_SEVERITY)(result.Severity | AS_FAIL);
		result.Result = (ARENA_RESULTS)(result.Result | AR_JOIN_FAILED);
		return result;
	}
	// ERROR checking: Do we have a valid value for arena max num of players?
	else if (pArena->m_instanceGenerateParam <= 0)
	{
		// UNLOCK CServerItemObj pointer collection (for arena bot)
		m_sharedInterfaceRef->m_netComObject->m_runtimeItems->Unlock();
		m_sharedInterfaceRef->m_eventLog->LogByPriority("Arena max players param [m_instanceGenerateParam] must be > 0!", 2);
		result.Severity = (ARENA_SEVERITY)(result.Severity | AS_FAIL);
		result.Result = (ARENA_RESULTS)(result.Result | AR_JOIN_FAILED);
		return result;
	}

	// HANDLE team assignment
	//	NOTE: Team indices are off by one for network messaging,
	//	i.e. 0 indicates no team assignment in network packets,
	//	while -1 indicates no team assignment for the [m_teamTemp]
	//	CPlayerObj data member. This is why we're subtracting 1 here.
	pPlayer->m_teamTemp = iTeamIndex - 1;

	// STORE the specified amount of money from the player as the arena wager
	if (m_sharedInterfaceRef->m_currencySystem->EvalCurrency(pPlayer->m_currentInventory) >= iBetAmount)
	{
		pArena->m_arenaBetPool += iBetAmount;
		pAccount->m_characterOnAccount->m_arenaBetAmount = iBetAmount;
		pPlayer->m_arenaBetAmount = iBetAmount;
	}
	else
	{
//		result.Severity = AS_WARNING;
//		result.Result = AR_INSUFFICIENT_FUNDS_FOR_BETTING;
		pPlayer->m_arenaBetAmount = 0;
//		result.Severity = (ARENA_SEVERITY)(result.Severity | AS_FAIL);
//		result.Result = (ARENA_RESULTS)(result.Result | AR_INSUFFICIENT_FUNDS_FOR_BETTING);
//		return result;
	}

	// HANDLE registering the player for the specified arena
	int iSelection = 0;
	bResult = this->m_questInterface->HandleQuestSys(pPlayer, pAccount, pArena, pNearestArenaBot, iFromID, iSelection);
	if (bResult)
	{
		this->m_sharedInterfaceRef->m_eventLog->LogByPriority("HandleQuestSys returned: true", 2);
/*		m_sharedInterfaceRef->m_eventLog->AddEvent("Player requested to join arena: " + pPlayer->m_charName);
		m_sharedInterfaceRef->m_eventLog->AddEvent("Arena Bot ID    : " + m_sharedInterfaceRef->m_cabLib.ConvToString(pNearestArenaBot->m_spawnID));
		m_sharedInterfaceRef->m_eventLog->AddEvent("Arena Index     : " + m_sharedInterfaceRef->m_cabLib.ConvToString(iArenaIndex));
		m_sharedInterfaceRef->m_eventLog->AddEvent("Arena Team Index: " + m_sharedInterfaceRef->m_cabLib.ConvToString(pPlayer->m_teamTemp));
		m_sharedInterfaceRef->m_eventLog->AddEvent("Arena Bet Amount: " + m_sharedInterfaceRef->m_cabLib.ConvToString(pPlayer->m_arenaBetAmount));
*/	}
	else
		this->m_sharedInterfaceRef->m_eventLog->LogByPriority("HandleQuestSys returned: false", 2);

	// UNLOCK CServerItemObj pointer collection (for arena bot)
	m_sharedInterfaceRef->m_netComObject->m_runtimeItems->Unlock();
	return result;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CTextSystems
// Function:	HandleRequestArenaLeave(CPlayerObject, CAccountObject, DPNID, BYTE*)
// Description:	-
///////////////////////////////////////////////////////////////////////////////

ArenaResult
CTextSystems::HandleRequestArenaLeave(CPlayerObject *pPlayer, 
									  CAccountObject *pAccount, 
									  DPNID iFromID, 
									  BYTE* pData)
{
	ArenaResult result;

	// GET reference to the arena bot
	CServerItemObj *pArenaBot = m_sharedInterfaceRef->m_netComObject->m_runtimeItems->GetItemInRadius_LOSchannel(
		pPlayer->m_Target3dPosition,
		(pPlayer->m_dynamicRadius * 2.0f),
		pPlayer->m_currentWorldID);

	// ERROR checking: Do we have a valid reference to an arena bot?
	if(!pArenaBot)
	{
		result.Severity = (ARENA_SEVERITY)(result.Severity | AS_FAIL);
		result.Result = (ARENA_RESULTS)(result.Result | AR_COULD_NOT_FIND_ARENA_BOT);
		return result;
	}

	// LOOP through all arenas hosted at this bot
	POSITION pos = NULL;
	CAICollectionQuestObj *pQuest = NULL, *pArena = NULL;
	for (pos = pArenaBot->m_collectionQuestDB->GetHeadPosition(); pos != NULL;)
	{
		// GET a reference to the current quest object
		pQuest = dynamic_cast<CAICollectionQuestObj*>(pArenaBot->m_collectionQuestDB->GetNext(pos));

		// CHECK if the player is already registered for another arena. 
		//	If so, unregister the player.
		if (pQuest && pQuest->VerifyInDWMEM(pPlayer->m_handle))
		{
			// TRY to unregister player
			bool bResult = pQuest->RemoveFromDWMEM(pPlayer->m_handle);
			if (!bResult) 
			{
				// UNLOCK CServerItemObj pointer collection (for arena bot)
				m_sharedInterfaceRef->m_netComObject->m_runtimeItems->Unlock();
				result.Severity = (ARENA_SEVERITY)(result.Severity | AS_FAIL);
				result.Result = (ARENA_RESULTS)(result.Result | AR_LEAVE_FAILED);
				return result;
			}
			break;
		}
	}

	// RESET the player's team assignment
	pPlayer->m_teamTemp = -1;

	// RESET the player's arena bet data
	pQuest->m_arenaBetPool -= pPlayer->m_arenaBetAmount;
	pAccount->m_characterOnAccount->m_arenaBetAmount = 0;
	pPlayer->m_arenaBetAmount = 0;

	// IF there are less players registered than the min number for arena autostart,
	//	DISABLE the arena autostart timer 
	if (pQuest->DWMEM_getCount() < pQuest->m_instanceGenerateParam4)
		pQuest->m_arenaStartTimerStamp = 0;

	// CREATE the ARENA TYPE REQUEST response packet
	BLOCKQUEST_DATA HeaderPacket;
	HeaderPacket.m_catagorie = 33;
	HeaderPacket.m_itemAssignedID = 0;

	// COPY message header into packet
	SafeDeleteArray(pData);
	int iPos = 0;
	int iSize = sizeof(BLOCKQUEST_DATA)	// Header 
		+ sizeof(ARENA_SEVERITY)	// ArenaResult struct
		+ sizeof(ARENA_RESULTS)
		+ sizeof(char)					// Packet ID
		+ sizeof(int);					// Result code
	pData = new BYTE[iSize];
	::memcpy(&pData[iPos], &HeaderPacket, sizeof(BLOCKQUEST_DATA));
	iPos = sizeof(BLOCKQUEST_DATA);

	// COPY packet ID into the packet
	unsigned char iPacketID = ARENA_LEAVE_ID;
	::memcpy(&pData[iPos], &iPacketID, sizeof(char));
	iPos += sizeof(char);

	// COPY arena results into the packet
	::memcpy(&pData[iPos], &result.Severity, sizeof(ARENA_SEVERITY));
	iPos += sizeof(ARENA_SEVERITY);
	::memcpy(&pData[iPos], &result.Result, sizeof(ARENA_RESULTS));
	iPos += sizeof(ARENA_RESULTS);

	// ENCODE final response packet
	//	NOTE: Returning any non negative integer value indicates success
	int iResult = 1;
	::memcpy(&pData[iPos], &iResult, sizeof(int));
	iPos += sizeof(int);

	// SEND response packet to requesting player
	this->m_sharedInterfaceRef->m_netComObject->SendData(pData,
		 iSize,
		 DPNSEND_GUARANTEED,
		 iFromID,
		 m_sharedInterfaceRef->m_netComObject->m_packetTimeout);
	delete [] pData;//SendData copies the packet - free ours (was leaked every arena reply)
	pData = 0;

	// UNLOCK CServerItemObj pointer collection (for arena bot)
	m_sharedInterfaceRef->m_netComObject->m_runtimeItems->Unlock();
	return result;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CTextSystems
// Function:	HandleRequestArenaFailed(ArenaResult eError, QUEST_PACKETS, CPlayerObject, CAccountObject, DPNID, BYTE*)
// Description:	-
///////////////////////////////////////////////////////////////////////////////

void
CTextSystems::HandleRequestArenaFailed(ArenaResult result,
									   QUEST_PACKETS ePacketType,
									   CPlayerObject *pPlayer, 
									   CAccountObject *pAccount, 
									   DPNID iFromID,
									   BYTE *pData)
{
	// CREATE the ARENA TYPE REQUEST response packet
	BLOCKQUEST_DATA HeaderPacket;
	HeaderPacket.m_catagorie = 33;
	HeaderPacket.m_itemAssignedID = 0;

	// COPY message header into packet
	SafeDeleteArray(pData);
	int iPos = 0;
	int iSize = sizeof(BLOCKQUEST_DATA)	// Header 
		+ sizeof(ARENA_SEVERITY)	// Packet ID
		+ sizeof(ARENA_RESULTS)
		+ sizeof(QUEST_PACKETS);		// Arena result
	pData = new BYTE[iSize];
	::memcpy(&pData[iPos], &HeaderPacket, sizeof(BLOCKQUEST_DATA));
	iPos = sizeof(BLOCKQUEST_DATA);

	// COPY ARENA_MESSAGE_FAIL (defined in ArenaSystems.h) into the packet
	::memcpy(&pData[iPos], &result.Severity, sizeof(char));
	iPos += sizeof(char);

	// COPY arena result into the packet
	::memcpy(&pData[iPos], &result.Result, sizeof(char));
	iPos += sizeof(char);

	// COPY packet ID into the packet
	::memcpy(&pData[iPos], &ePacketType, sizeof(char));
	iPos += sizeof(char);

	// SEND response packet to requesting player
	this->m_sharedInterfaceRef->m_netComObject->SendData(pData,
         iSize,
         DPNSEND_GUARANTEED,
         iFromID,
		 m_sharedInterfaceRef->m_netComObject->m_packetTimeout);
	delete [] pData;//SendData copies the packet - free ours (was leaked every arena reply)
	pData = 0;
}
