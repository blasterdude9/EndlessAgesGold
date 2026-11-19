#ifndef CCLIENTCOMMCLASS_H
#define CCLIENTCOMMCLASS_H

#include "dplay8.h"
#include "dplobby.h"
#include "CStructuresMisl.h"
#include "CMultiplayerClass.h"
#include "CMenu3DClass.h"
#include "CHousingClass.h"
#include "CtextReadoutClass.h"
#include "CTextRenderClass.h"
#include "ViewportClass.h"
#include "CSpawnClass.h"

class  CClientCommObj : public CObject
{    
public:
	CClientCommObj(){};
    
	//---------------
    //Custom handlers
	//The contents of these functions can be modified to change the functionality of the game client
    //---------------
    BOOL             HandleEngineMessagesClientSide(LPVOID lpMessage,
													DPNID idFromLoc,
													int msgSize,
													CMultiplayerObj * multiplayerObj,
													CMovementObjList * EntObjRuntime,
													CHousingCacheObjList * m_houseingCache,
													BOOL & m_pingInQuerry,
													int & CURPING,
													long & m_pingPersistStamp,
													int & MultiplayerDelayComm,
													CMenu3DObjList * menuList,
													LPDIRECT3DDEVICE9 d3dDevice,
													CtextReadoutObj * GL_systemReadout,
													CSymbolMapperObjList * m_symbolMapDB,
													CIconSysList * m_iconDatabase,
													CViewportList * m_runtimeViewportDB,
													CSpawnObjList * spawnGeneratorDB,
													BOOL & m_speechMode);
    BOOL HandleAttribMessageClientSide(LPATTRIB_STRUCT formatedMessage,
									   DPNID idFromLoc,
									   CMultiplayerObj * multiplayerObj,
									   CMovementObjList * EntObjRuntime);
    BOOL HandleEquipMessageClientSide(LPEQUIP_STRUCT formatedMessage);
	BOOL HandleHouseingBlock(BYTE * byteBlock,int msgSz,CMultiplayerObj * multiplayerObj);
	BOOL HandleSpawnMessagesClientSide(LPSPAWN_STRUCT formatedMessage,DPNID idFromLoc,CMultiplayerObj * multiplayerObj);
	void HandleItemLocationalMessagesClientSide(LPITEM_LOCATION formatedMessage,BYTE * dataBlock);
    BOOL HandleMenuBuildMessagesClientSide(LPMENUBUILD_STRUCT formatedMessage,DPNID idFromLoc,CMenu3DObjList * menuList);
	BOOL HandleClientMovBlockData(LPVOID lpMessage,int & msgSize,DPID & idFromLoc,CMultiplayerObj * multiplayerObj);
	BOOL HandleClientListGlidBlockData(LPVOID lpMessage,int & msgSize,DPID & idFromLoc,CMultiplayerObj * multiplayerObj);
	BOOL HandleEntityCfgBlock(LPVOID lpMessage,int & msgSize,DPID & idFromLoc,CMultiplayerObj * multiplayerObj);
	BOOL HandleBankDataBlock(LPVOID lpMessage,int & msgSize,DPID & idFromLoc,CMultiplayerObj * multiplayerObj);
	BOOL HandleStoreDataBlock(LPVOID lpMessage,int & msgSize,DPID & idFromLoc,CMultiplayerObj * multiplayerObj);
	BOOL HandleHouseStoreDataBlock(LPVOID lpMessage,int & msgSize,DPID & idFromLoc,CMultiplayerObj * multiplayerObj);
	BOOL HandleLootBlockData(LPVOID lpMessage,int & msgSize,DPID & idFromLoc);
	BOOL ParseGroupListData(BYTE * data,int size);
	BOOL ParseClanRosterData(BYTE * data,int size);
	BOOL HandleSkillAbilitiesClientSide(BYTE * byteBlock,int msgSz,DPNID idFromLoc);
	BOOL HandleSkillsOutputClientSide(BYTE * byteBlock,int msgSz,DPNID idFromLoc);
	BOOL HandleTextMessageClientSide(LPVOID lpMessage,int & msgSize,DPID & idFromLoc);
	BOOL HandleClientStatBlockData(LPVOID lpMessage,int & msgSize,DPID & idFromLoc);
	void HandleMiscBlocksClientSide(BYTE * miscUpdateBlock, int miscBlockSize);
	BOOL SpawnEntityBasedOnPositionAndCfg(D3DVECTOR position,int aiCfg,int dynamicSpawnId,long lifeMaxTime);
	
};


#endif