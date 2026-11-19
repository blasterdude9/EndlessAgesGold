#pragma once
#include "sharednet.h"

class CEventSystems
{
public:
	//Constructors
	CEventSystems(CSharedNet *sharedInterface);

	//Base Functions
	void RR_RunEventsFromText(CDBConnectionSystem *dbConnection, CPlayerObject *pPlayerObj, CString strEvents);
	void RR_IfStatement(CDBConnectionSystem *dbConnection, CPlayerObject *pPlayerObj, CString strParams);		//Params: test statements:true statements:false statements

	//NPC Chat Events
	void RR_SendChatReply(CDBConnectionSystem *dbConnection, CPlayerObject *pPlayerObj, CString iReplyIndex);	//Params: replyIndex
	void RR_SetChatLevel(CDBConnectionSystem *dbConnection, CPlayerObject *pPlayerObj, CString strParams);		//Params: npcId, chatId, chatLevel, freezeChat, forceDemote
	
	//NPC Events
	void RR_SpawnNPCToLocation(CString strParams);
	void RR_SpawnNPCToOffsetFromPlayer(CPlayerObject *pPlayerObj, CString strParams);

	//Alter Player Events
	void RR_KillPlayer(CPlayerObject *pPlayerObj);
	void RR_AdjustHealth(CPlayerObject *pPlayerObj, int iHealthAdjustment);
    	
	//Item Events
	void RR_GiveItem(CPlayerObject *pPlayerObj, CString strParams);
	void RR_EquipItem(CPlayerObject *pPlayerObj, int iItemId);
	bool RR_PlayerHasItem(CPlayerObject *pPlayerObj, CString strParams) { return pPlayerObj->m_currentInventory->VerifyPlayerHasItem(atoi(strParams)); }
	bool RR_PlayerHasItemName(CPlayerObject *pPlayerObj, CString strParams) { return pPlayerObj->m_currentInventory->VerifyPlayerHasItem(strParams); }
	bool RR_PlayerHasItemNameArmed(CPlayerObject *pPlayerObj, CString strParams) { return pPlayerObj->m_currentInventory->VerifyPlayerHasItem(atoi(strParams)); }
	bool RR_PlayerHasItemArmed(CPlayerObject *pPlayerObj, CString strParams) { return pPlayerObj->m_currentInventory->VerifyPlayerHasItem(strParams); }

	//Variables
	CSharedNet *m_sharedInterfaceRef;
};