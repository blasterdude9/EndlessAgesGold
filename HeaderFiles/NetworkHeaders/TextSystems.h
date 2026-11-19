#pragma once
#include "sharednet.h"
#include "questsystems.h"
#include "CNetworkCommands.h"

class CTextSystems
{
public:
	CTextSystems(CSharedNet * sharedInterface,CQuestSystems * questSys);
	~CTextSystems(void);

    //Vars
    CSharedNet * m_sharedInterfaceRef;
	CQuestSystems * m_questInterface;
	CNetworkCommands *m_pNetworkCommands;
	char m_bufferTemp[1024];

	//Functions
	BOOL HandleTextMessageServerSide_safe(LPVOID lpMessage,int & msgSize,DPID & idFromLoc);
	ArenaResult HandleRequestArenaTypes(CPlayerObject *pPlayer, CAccountObject *pAccount, DPNID iFromID, BYTE* pData);
	ArenaResult HandleRequestArenaInfo(CPlayerObject *pPlayer, CAccountObject *pAccount, DPNID iFromID, BYTE* pData);
	ArenaResult HandleRequestArenaJoin(CPlayerObject *pPlayer, CAccountObject *pAccount, DPNID iFromID, BYTE* pData);
	ArenaResult HandleRequestArenaLeave(CPlayerObject *pPlayer, CAccountObject *pAccount, DPNID iFromID, BYTE* pData);
	void HandleRequestArenaFailed(ArenaResult eError, QUEST_PACKETS ePacketType, CPlayerObject *pPlayer, CAccountObject *pAccount, DPNID iFromID, BYTE* pData);
	BOOL LootInventoryRequest(CPlayerObject * looter,CAccountObject * accountPtr,DPNID idFromLoc);
	BOOL HandleTextMessageServerSideGroupStyle_safe(LPVOID lpMessage,int & msgSize,DPID & idFromLoc);
	void SendTextMessageToGroup(CPlayerObject * pPtr,CString message);
	void SendMsgByHandle(int handle,CString message,int type);//0 radius = to all
	bool GetCommand(CString & fullMsg, CString & commandReturn, CString & extendedText);

private:
	//bool HandleLootServerItem(CPlayerObject *pLooter, CServerItemObj *pServerItem, CAccountObject *pAccount, DPNID iFromLocID);
};
