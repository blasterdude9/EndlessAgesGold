#include "StdAfx.h"
#include "EventSystems.h"
#include "ServerMain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEventSystems::CEventSystems(CSharedNet *sharedInterface)
{
	m_sharedInterfaceRef = sharedInterface;
}

void CEventSystems::RR_RunEventsFromText(CDBConnectionSystem *dbConnection, CPlayerObject *pPlayerObj, CString strEvents)
{
    m_sharedInterfaceRef->m_eventLog->LogByPriority("RR_RunEventsFromText()",2);

	int i = 0;
	while(i < strEvents.GetLength())
	{
		int next = strEvents.Find(':', i);
		if(next == -1)
			next = strEvents.GetLength();

		CString strCommand	= strEvents.Mid(i, 3);
		i += 4;
		CString strParams	= strEvents.Mid(i, next - i);
		i = next + 1;

		//m_sharedInterfaceRef->BroadcastMessage(0, "Command: " + strCommand, pPlayerObj->m_3dPosition, pPlayerObj->m_currentWorldID, pPlayerObj->m_networkAssignedID);
		//m_sharedInterfaceRef->BroadcastMessage(0, "Parameters: " + strParams, pPlayerObj->m_3dPosition, pPlayerObj->m_currentWorldID, pPlayerObj->m_networkAssignedID);

		if(strCommand == "iff")
			//Params: npcId, chatId, chatLevel, freezeChat, forceDemote
			RR_IfStatement(dbConnection, pPlayerObj, strParams);
		else if(strCommand == "scr")
			//Params: replyIndex
			RR_SendChatReply(dbConnection, pPlayerObj, strParams);
		else if(strCommand == "scl")
			//Params: npcId, chatId, chatLevel, freezeChat, forceDemote
			RR_SetChatLevel(dbConnection, pPlayerObj, strParams);
		else if(strCommand == "kip")
			RR_KillPlayer(pPlayerObj);
		else if(strCommand == "gph")
			RR_AdjustHealth(pPlayerObj, atoi(strParams));
		else if(strCommand == "gpi")
			RR_GiveItem(pPlayerObj, strParams);
		else if(strCommand == "iff")
			//Params: npcId, chatId, chatLevel, freezeChat, forceDemote
			RR_IfStatement(dbConnection, pPlayerObj, strParams);
		else if(strCommand == "spn")
			//Params: glid, edbRef, animation, x, y, z, worldId, respawnDuration, name
			RR_SpawnNPCToLocation(strParams);
		else if(strCommand == "sno")
			//Params: glid, edbRef, animation, x, y, z, respawnDuration, name
			RR_SpawnNPCToOffsetFromPlayer(pPlayerObj, strParams);


	}
}

void CEventSystems::RR_IfStatement(CDBConnectionSystem *dbConnection, CPlayerObject *pPlayerObj, CString strParams)
{
	//Params: test statements:true statements:false statements
	
	//Split params into test, true, and false statement sets
	CString lstStatements[3];
	int semiColon = strParams.Find(';', 0);
	lstStatements[0] = strParams.Mid(0, semiColon++);
	
	int semiColon2 = strParams.Find(';', semiColon);
	lstStatements[1] = strParams.Mid(semiColon, semiColon2++ - semiColon);
	lstStatements[1].Replace('-', ':');
	
	lstStatements[2] = strParams.Mid(semiColon2, strParams.GetLength() - semiColon2);
	lstStatements[2].Replace('-', ':');

	m_sharedInterfaceRef->BroadcastMessage(0, "Test: " + lstStatements[0] + " | True: " + lstStatements[1] + " | False: " + lstStatements[2], pPlayerObj->m_3dPosition, pPlayerObj->m_currentWorldID, pPlayerObj->m_networkAssignedID);
	
	//Loop through test statements and check each of them
	int length = strParams.GetLength();
	int i = 0;
	while(i < length)
	{
		//Grab the Commands, Params, and update i
		int next = lstStatements[0].Find('-', i);
		if(next == -1) next = length;

		CString command	= lstStatements[0].Mid(i, 3);
		i += 4;
		CString params	= lstStatements[0].Mid(i, next - i);
		i = next + 1;

		m_sharedInterfaceRef->BroadcastMessage(0, "Command: " + command + " | Params: " + params, pPlayerObj->m_3dPosition, pPlayerObj->m_currentWorldID, pPlayerObj->m_networkAssignedID);
		//Figure out which command it is and test it
		//if(command == "phi") //Player Has Item
			//if(RR_PlayerHasItem(pPlayerObj, params))
				//continue;

		//If we get here, then one of the test statements failed, run false statements
		//RR_RunEventsFromText(dbConnection, pPlayerObj, lstStatements[2]);
		//return;
	}

	//If we get here, all of the test statements passed, run true statements
	RR_RunEventsFromText(dbConnection, pPlayerObj, lstStatements[1]);
}

void CEventSystems::RR_SendChatReply(CDBConnectionSystem *dbConnection, CPlayerObject *pPlayerObj, CString strReplyIndex)
{
	_RecordsetPtr npcText = dbConnection->Query("SELECT text FROM gmNpcChatReplies WHERE id='" + strReplyIndex + "'");
	CString strText = dbConnection->GetStringData("text", npcText);
	m_sharedInterfaceRef->BroadcastMessage(0, "Sending Chat Reply: " + strText, pPlayerObj->m_3dPosition, pPlayerObj->m_currentWorldID, pPlayerObj->m_networkAssignedID);
}
																	   //TODO: Remove CPlayerObject for Release
void CEventSystems::RR_SetChatLevel(CDBConnectionSystem *dbConnection, CPlayerObject *pPlayerObj, CString strParams)
{
	//Params: npcId, chatId, chatLevel, freezeChat, forceDemote
	CString lstParams[5];
	int lastComma = 0;
	for(int i = 0; i < 4; i++)
	{
		int newComma = strParams.Find(",", lastComma);
		lstParams[i] = strParams.Mid(lastComma, newComma - lastComma);
		lastComma = newComma + 1;
	}
	lstParams[4] = strParams.Mid(lastComma);

	//m_sharedInterfaceRef->BroadcastMessage(0, "Setting Chat Level - npcId: " + lstParams[0] + "; chatId: " + lstParams[1] + "; chatLevel: " + lstParams[2] + "; setFrozen: " + lstParams[3] + "; forceDemote: " + lstParams[4], pPlayerObj->m_3dPosition, pPlayerObj->m_currentWorldID, pPlayerObj->m_networkAssignedID);
	char buffer[10] = {0};
	_RecordsetPtr prevChat = dbConnection->Query("SELECT id, chatLevel FROM gmNpcChatProgress WHERE handle='" + m_sharedInterfaceRef->m_cabLib.ConvToString(pPlayerObj->m_handle) + "' AND npcId='" + lstParams[0] + "' AND chatId='" + lstParams[1] + "'");
	int iCountChats = dbConnection->GetCount(prevChat);

	if(iCountChats > 0)
	{
		//m_sharedInterfaceRef->BroadcastMessage(0, "ChatCount > 0", pPlayerObj->m_3dPosition, pPlayerObj->m_currentWorldID, pPlayerObj->m_networkAssignedID);
		CString oldChatId = dbConnection->GetStringData("id", prevChat);
		int oldChatLevel = dbConnection->GetIntData("chatLevel", prevChat);
		if(atoi(lstParams[2]) >= oldChatLevel || lstParams[4] == "1")
		{
			//m_sharedInterfaceRef->BroadcastMessage(0, "UPDATE gmNpcChatProgress SET chatLevel='" + lstParams[2] + "', isFrozen='" + lstParams[3] + "' WHERE id='" + oldChatId + "'", pPlayerObj->m_3dPosition, pPlayerObj->m_currentWorldID, pPlayerObj->m_networkAssignedID);
			dbConnection->Query("UPDATE gmNpcChatProgress SET chatLevel='" + lstParams[2] + "', isFrozen='" + lstParams[3] + "' WHERE id='" + oldChatId + "'");
		}
	}
	else
	{
		//m_sharedInterfaceRef->BroadcastMessage(0, "ChatCount <= 0", pPlayerObj->m_3dPosition, pPlayerObj->m_currentWorldID, pPlayerObj->m_networkAssignedID);
		//m_sharedInterfaceRef->BroadcastMessage(0, "INSERT INTO gmNpcChatProgress (id, handle, npcId, chatId, chatLevel, isFrozen) VALUES ('', '" + CString(itoa(pPlayerObj->m_handle, buffer, 10)) + "', '" + lstParams[0] + "', '" + lstParams[1] + "', '" + lstParams[2] + "', '" + lstParams[3] + "')", pPlayerObj->m_3dPosition, pPlayerObj->m_currentWorldID, pPlayerObj->m_networkAssignedID);
		dbConnection->Query("INSERT INTO gmNpcChatProgress (handle, npcId, chatId, chatLevel, isFrozen) VALUES ('" + m_sharedInterfaceRef->m_cabLib.ConvToString(pPlayerObj->m_handle) + "', '" + lstParams[0] + "', '" + lstParams[1] + "', '" + lstParams[2] + "', '" + lstParams[3] + "')");
	}
}

void CEventSystems::RR_SpawnNPCToLocation(CString strParams)
{
	//Params: glId, edbRef, animation, x, y, z, channelId, respawnDuration, name
	/*CString lstParams[9];
	int lastComma = 0;
	for(int i = 0; i < 8; i++)
	{
		int newComma = strParams.Find(",", lastComma);
		lstParams[i] = strParams.Mid(lastComma, newComma - lastComma);
		lastComma = newComma + 1;
	}
	lstParams[8] = strParams.Mid(lastComma);

	D3DVECTOR pos = {(float)atof(lstParams[3]), (float)atof(lstParams[4]), (float)atof(lstParams[5])};
	CServerItemGenObj *spnGenPtr = m_sharedInterfaceRef->m_serverItemGenerator->AddGenItem(atoi(lstParams[0]), atoi(lstParams[2]), pos, atoi(lstParams[1]), atoi(lstParams[6]), atoi(lstParams[7]));

	CServerItemObj *itemPtr = NULL;
	spnGenPtr->m_serverItem->Clone(&itemPtr);
	itemPtr->m_networkAssignedID = m_sharedInterfaceRef->m_netComObject->GetFreeRuntimeEntityNetworkID();
    itemPtr->m_spawnID = spnGenPtr->m_spawnGenID;
	itemPtr->m_displayedName = lstParams[8];
	
    m_sharedInterfaceRef->m_netComObject->m_runtimeItems->AddIndexed(itemPtr);
    spnGenPtr->m_existsOnServer = TRUE;*/
}

void CEventSystems::RR_SpawnNPCToOffsetFromPlayer(CPlayerObject *pPlayerObj, CString strParams)
{
	//Params: glid, edbRef, animation, x, y, z, respawnDuration, name
	/*CString lstParams[8];
	int lastComma = 0;
	for(int i = 0; i < 7; i++)
	{
		int newComma = strParams.Find(",", lastComma);
		lstParams[i] = strParams.Mid(lastComma, newComma - lastComma);
		lastComma = newComma + 1;
	}
	lstParams[7] = strParams.Mid(lastComma);

	D3DVECTOR pos = {pPlayerObj->m_3dPosition.x + (float)atof(lstParams[3]), pPlayerObj->m_3dPosition.y + (float)atof(lstParams[4]), pPlayerObj->m_3dPosition.z + (float)atof(lstParams[5])};
	CServerItemGenObj *spnGenPtr = m_sharedInterfaceRef->m_serverItemGenerator->AddGenItem(atoi(lstParams[0]), atoi(lstParams[2]), pos, atoi(lstParams[1]), pPlayerObj->m_currentWorldID, atoi(lstParams[6]));

	CServerItemObj *itemPtr = NULL;
	spnGenPtr->m_serverItem->Clone(&itemPtr);
	itemPtr->m_networkAssignedID = m_sharedInterfaceRef->m_netComObject->GetFreeRuntimeEntityNetworkID();
    itemPtr->m_spawnID = spnGenPtr->m_spawnGenID;
	itemPtr->m_displayedName = lstParams[7];
	
    m_sharedInterfaceRef->m_netComObject->m_runtimeItems->AddIndexed(itemPtr);
    spnGenPtr->m_existsOnServer = TRUE;*/
}

/* PLAYER HEALTH FUNCTIONS */
void CEventSystems::RR_KillPlayer(CPlayerObject *pPlayerObj)
{

}

void CEventSystems::RR_AdjustHealth(CPlayerObject *pPlayerObj, int healthAdjustment)
{
	
}

void CEventSystems::RR_GiveItem(CPlayerObject *pPlayerObj, CString strParams)
{
	
}

void CEventSystems::RR_EquipItem(CPlayerObject *pPlayerObj, int iItemId)
{
}
