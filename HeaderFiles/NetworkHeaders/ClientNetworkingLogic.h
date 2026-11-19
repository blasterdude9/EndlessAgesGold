#pragma once
#include <typeinfo>
#include <algorithm>
#include <vector>
#include "globalstatusvariables.h"
//#include "AttributeQueue.h"

class __declspec(dllexport) CClientNetworkingLogic
{
public:
	CClientNetworkingLogic(GlobalStatusVariables * gStatus);
	~CClientNetworkingLogic(void);

	//Variables
    GlobalStatusVariables *			m_gStatus;
	BOOL							m_bQuickSlotsVerified;
	BOOL							m_pingInQuerry;
	DWORD                           CURPING;
	DWORD                           m_pingPersistStamp;
	DWORD					        MultiplayerDelayComm;
    DWORD				            MultiplayerDelayCommVarial;

	//Functions
	void ClientCallback(CSpawnObjList * spawnGeneratorDB,CMovementObjList * entObjRuntime,CMissileObjList * missileRuntime,CExplosionObjList * explosionRuntimeList);
	void RemovePlayerRuntimeListByID(DPNID idFromLoc);
	BOOL GetRuntimeObjectFromID(DPNID idFromLoc,CMovementObj ** objRet);
	BOOL GetRuntimeObjectFromNetworkID(int networkIDTrace,CMovementObj ** objRet);
	BOOL SendAttribMsgCP(   int attribute,
							int uniInt,
							long refrenceID,
							int uniInt2,
							short uniShort3);
	void StampNetworkMark(CMovementObj * ptrLoc);
	void ManageSmoothMovementOnNetworkEntities(DWORD CurrentTickCount,CMovementObjList * entObjRuntime);
	BOOL HandleSystemMessagesClientSide(LPVOID lpMessage,DPNID idFromLoc);
	BOOL UpdateNetworkEntity(LPMOVE_STRUCT expandedStruct,BOOL isItem,int & currentRequestCount,CMovementObjList * entObjRuntime);
	BOOL HandleAttribMessageClientSide(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc,CSpawnObjList * spawnGeneratorDB,CMovementObjList * entObjRuntime,int worldChannelRef);
	BOOL DeleteCharacter(int index);
	BOOL CreateCharacterOnServer(   int     DBindex,
									int     spwnCfg,
									CString characterName,
									CMenuVisObj * entityReference);
	BOOL PlayCharacterOnAccount(int index);
	BOOL HandleFireMessagesClientSide(LPFIRE_STRUCT formatedMessage,CMissileObjList * missileRuntime);
	BOOL HandleEquipMessageClientSide(LPEQUIP_STRUCT formatedMessage,CMovementObjList * entObjRuntime,CMissileObjList * missileRuntime);
	BOOL HandleSpawnMessagesClientSide(LPSPAWN_STRUCT formatedMessage,DPNID idFromLoc);
	void HandleItemLocationalMessagesClientSide(BYTE * dataBlock,CMovementObjList * entObjRuntime,CMissileObjList * missileRuntime, int dataSize);
	void Loot_clientSide(int targetNetworkID,int myNetworkID,int itemIndex);
	BOOL HandleMenuBuildMessagesClientSide(LPMENUBUILD_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL HandleTextMessageClientSide(LPVOID lpMessage,int & msgSize,DPID & idFromLoc);
	void HandleArenaFailedResponse(QUEST_PACKETS eType, ArenaResult Result);
	bool HandleArenaTypeRequestClientSide(BYTE *pData, int &iMessageSize, DPID &iFromID);
	bool HandleArenaInfoRequestClientSide(BYTE *pData, int &iMessageSize, DPID &iFromID);
	bool HandleArenaJoinRequestClientSide(BYTE *pData, int &iMessageSize, DPID &iFromID);
	bool HandleArenaLeaveRequestClientSide(BYTE *pData, int &iMessageSize, DPID &iFromID);
	void ShowArenaRegistrationMessageBox(const char *pMessage);
	bool UpdateArenaStatusMenu(ARENA_STATUS eStatus);
	bool HandleJournalRequestClientSide(BLOCKQUEST_DATA *pHeader, BYTE *pData, int &iMessageSize, DPID &iFromID);
	BOOL ProcessJournalEntry(BYTE *pData, unsigned int &iPos, CQuestJournalObj *pJournalEntry);
	BOOL HandleClientListGlidBlockData(LPVOID lpMessage,int & msgSize,DPID & idFromLoc);
	BOOL HandleEntityCfgBlock(LPVOID lpMessage,int & msgSize,DPID & idFromLoc,CMovementObjList * entObjRuntime,CMissileObjList * missileRuntime);
	BOOL HandleClientMovBlockData(LPVOID lpMessage,int & msgSize,DPID & idFromLoc,CMovementObjList * entObjRuntime,CMissileObjList  * missileRuntime,CExplosionObjList * explosionRuntimeList);
	BOOL HandleAIMovBlockData(  LPVOID lpMessage,
								int & msgSize,
								DPID & idFromLoc,
								CMovementObjList * entObjRuntime,
								CMissileObjList  * missileRuntime,
								CExplosionObjList * explosionRuntimeList,
								CSpawnObjList * spawnGeneratorDB,
								int worldChannelRef);
	void SendPendingCfgRequestsCP();
	BOOL HandleClientStatBlockData(LPVOID lpMessage,int & msgSize,DPID & idFromLoc);
	BOOL HandleStoreDataBlock(LPVOID lpMessage,int & msgSize,DPID & idFromLoc);
	BOOL SendCommandMsg(int msgType,int miscInt,int networkAssignedID);
	BOOL ParseGroupListData(BYTE * data,int size);
	BOOL ParseClanRosterData(BYTE * data,int size);
	BOOL HandleSkillAbilitiesClientSide(BYTE * byteBlock,int msgSz,DPNID idFromLoc);
	BOOL HandleSkillsOutputClientSide(  BYTE * byteBlock,int msgSz,DPNID idFromLoc);
	BOOL HandleSkillTitlesOutputClientSide(BYTE * byteBlock, int msgSz, DPNID idFromLoc);
	BOOL SendGuerenteedCycleMsgToServer();
	BOOL HandleHouseingBlock(BYTE * byteBlock,int msgSz);
	BOOL HandleHouseStoreDataBlock(LPVOID lpMessage,int & msgSize,DPID & idFromLoc);
	BOOL SendDamageMsg( short playerThatGotHitID,
						int   type,
						short miscValue,
						short attackerNetworkID,
						short secUi,
						short targetX,
						short targetY,
						short targetZ);
	BOOL SendMovMsg(    DWORD flags,
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
						long currenttimeEL); 
	BOOL SendAIBlockMsg(DWORD flags,
						DPNID sendTo,
						SHRUNK_MOVE_STRUCT * m_dataBlock,
						int blockSize
						);
	LPSHRUNK_TOSVRMOV_STRUCT GetMovMsg( DWORD flags,
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
										long currenttimeEL);
	BOOL HandleEngineMessagesClientSide(LPVOID lpMessage,
		                                DPNID idFromLoc,
										int msgSize,
										CSpawnObjList * spawnGeneratorDB,
										CMovementObjList * entObjRuntime,
										CMissileObjList  * missileRuntime,
										CExplosionObjList * explosionRuntimeList,
										CCollisionObj * collisionBase,
									    CCollisionObjList * collisionDatabase,
										int worldChannelRef);
	void UpdateServerCallback(CSpawnObjList * spawnGeneratorDB,CMovementObjList * entObjRuntime,CMissileObjList * missileRuntime);
	BOOL HandleBankDataBlock(LPVOID lpMessage,int & msgSize,DPID & idFromLoc);
	BOOL HandleLootBlockData(LPVOID lpMessage,int & msgSize,DPID & idFromLoc);
	void HandleMiscBlocksClientSide(BYTE * miscUpdateBlock, int miscBlockSize,CMovementObjList * entObjRuntime);
	BOOL SpawnEntityBasedOnPositionAndCfg(D3DVECTOR position, 
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
										  int worldChannel);
	void HandleDmgMsgHandler(LPDAMAGEINFO_DATA formatedMessage);
	void HandleGeneralMsgHandler(LPMSGHANDLER formatedMessage);
	void HandleDetailedMsgHandler(LPDETAILEDMSG_DATA formatedMessage,CMovementObjList * entObjRuntime,CMissileObjList * missileRuntime,CExplosionObjList * explosionRuntimeList);
	BOOL DeleteStringFromMenuList(int controlID,int index);
	void ClearAllFromMenuList(int controlID);
	BOOL RespawnOnClient(int spawnType,D3DVECTOR position);
	void PendingCommandsCallback();
	BOOL SendAI_GuerenteedCycleMsgToServer();
};
