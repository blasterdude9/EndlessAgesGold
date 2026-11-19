#ifndef SHAREDNET
#define SHAREDNET
#endif

#pragma once
#include "CMultiplayerClass.h"
#include "CABFunctionLib.h"
#include "CServerItemGen.h"
#include "CAIRaidClass.h"
#include "CCollisionClass.h"
#include "CSafeZones.h"
#include "ServerCurrency.h"
#include "CMovementModel.h"
#include "AIclass.h"
#include "CWorldChannelRules.h"
#include "CSkillClass.h"
#include "CElementsClass.h"
#include "CBankClass.h"
#include "CCommerceClass.h"
#include "CPortalSysClass.h"
#include "CHousingClass.h"
#include "CExplosionClass.h"
#include "CGmPageClass.h"
#include "CBattleSchedulerClass.h"
#include "CMissileClass.h"
#include "CArmedInventoryClass.h"
#include "dbconnectionsystem.h"
#include "serverscripts.h"
#include "characterpendingloader.h"
#include "CitemExpanseObj.h"
#include "actionzones.h"
#include "loggingclass.h"
#include "CDotClass.h"
#include "ItemManager.h"
#include "AxServerPOI.h"
#include "CAxClanSys.h"
#include "DmeUnifiedCommerce.h"



class CSharedNet
{
public:
	CSharedNet(void);
	~CSharedNet(void);


    //Variables
	CMultiplayerObj					* m_netComObject;
	CABFunctionLib                    m_cabLib;
	CServerItemGenObjList           * m_serverItemGenerator;
	CAIRaidObjList                  * m_aiRaidDatabase;
	CCollisionObjList               * m_collisionDatabase;
	//CSafeZoneObjList                * m_safeZoneDB;
	CMovementObjList                * m_entObjList;
	CAIObjList                      * m_aiList;
	CWorldChannelRuleList           * m_worldChannelRuleDB;
	CSkillObjectList                * m_skillDatabase;
	CElementObjList                 * m_elementDB;
	CBankObjList                    * m_bankZoneList;
	CCommerceObjList                * m_commerceDB;
	CPortalSysList                  * m_portalDatabase;
	CHousingObjList                 * m_housingDB;
	CExplosionObjList               * m_explosionDBList;
	CGmPageList						* m_gmPageListRuntime;
	MatrixARB                         m_matLib;
	CBattleSchedulerList		    * m_battleSchedulerSys;
	CMissileObjList                 * m_missileDB;
	CDotClassList					* m_dotDB;
	CDBConnectionSystem             * m_dbConnection;
	CServerEventList                * m_eventLog;
	LARGE_INTEGER				      GL_frequency;
	LARGE_INTEGER				      m_stamp_date_temp;
	LONGLONG                          m_longMultiplier;
	CServerScriptSystem             * m_serverScripts;
	CCharacterPendingLoader         * m_characterPendingLoader;
	ConnectionStructureClass          m_baseAccountDBConnection;
	ConnectionStructureClass          m_authenticationDBConnection;
	BOOL                              m_dbIgnMode;
	CString                           m_globalRefURL;
	CString                           m_globalBotUrl;
	CActionZoneDB                   * m_actionZoneDB;
	CCDataHandlerDB                 * m_threadedDataProcessor;
	CLoggingClass                     m_realtimeLogger;//realtime write log dont use in speed dependent areas
	DWORD							  m_regenerationTurnSpeed;
	DWORD							  m_defaultHealthRegenAmount;
	unsigned int                      m_timeoutTraversal;
	DWORD                             m_lastTimeoutCall;
	int								  m_lootMode;
	int								  m_aggressorMaxTrack;//sets max agressor number that the system will store per char
	int								  m_maxDotTrack;
	int								  m_damageMode;
	CItemManager					* m_itemManager;
	int								  m_worldRuleTrace;
	DWORD							  m_worldRuleTraceStamp;
	float							  m_respawnHealthPercentage;
	CAxServerPOISystem				* m_poiSystem;
	int								  m_teamGlidNotifier;
	int								  m_groupGlidNotifier;
	CServerCurrencySystem			* m_currencySystem;
	int								  m_primaryProcessingThreadCount;
	int								  m_maxCharacterCreate;
	CAxClanSystemComm				  m_clanCommSys;
	CUnifiedCommerce				  m_unifiedCommSys;
	unsigned int					  m_globalGroupCounter;
	unsigned int					  m_arenaFactionBonus;
	unsigned int					  m_pvpFactionBonus;
	unsigned int					  m_clanFactionBonus;
	int								* m_arenaItems;
	int								  m_arenaItemCount;

	//GroupInfo
	const unsigned int				  SQUAD_MESSAGE_BURST_SIZE;
	SQUAD_MESSAGE					 *m_groupDataBlock;

	


    //Functions
	BOOL SendArenaMessage(CPlayerObject *pPlayer, ARENA_MESSAGE &message);
	BOOL SendBadgeMessage(CPlayerObject *pPlayer);
	void Unhide(CPlayerObject * plPtr);
	BOOL SendGeneralMessageHandler(int msgType,CPlayerObject * playerObj);
	BOOL SendFireMsgToOutbox(D3DVECTOR lookat,short firQueIndex,long refrenceID,CPlayerObject * playerObjectData);
	void DisconnectPlayerAndLog(CPlayerObject * playerObjectData,CString errorMsg);
	BOOL SendAttribMsg( DWORD flags,
						DPNID sendTo,
						int attribute,
						int uniInt,
						long refrenceID,
						int uniInt2,
						short uniShort3,
						CPlayerObject * runtimePlOption);
	void BroadcastEffect(int expRef,int newtorkAssignedID,CPlayerObject * hostPtr);
	void BroadcastNPCAnimState(CServerItemObj * svrPtr);
	BOOL SendDetailedMsgToClient(   int msgType,
									int miscInt,
									int miscInt2,
									int miscInt3,
									CPlayerObject * playerObj);
	void BroadcastMessage(  short radius,
							CString message,
							D3DVECTOR position,
							int world,
							int referenceID);
	//Server-wide messages / GM chat commands / timed shutdown
	void BroadcastToAll(CString message);//every player in every zone
	BOOL HandleGMChatCommand(CPlayerObject * playerObj, CString text);//"!" commands typed in chat by admin/GM
	void ShutdownTick();//called once a second by the server window
	volatile DWORD					  m_shutdownAtTick;//0 = no shutdown pending
	volatile int					  m_shutdownLastWarn;//seconds left at the last warning
	volatile BOOL					  m_shutdownNow;//server window closes the server when TRUE

	//Raids (EARevolution.AIR) - run by RaidTick() from the server window, once a second
	void RaidTick();
	BOOL StartRaid(int raidIndex);
	void EndRaid(BOOL calledOff);
	class CAIRaidObj * GetRaidByIndex(int raidIndex);
	int  CountPlayersInWorld(int worldID);//-1 = whole server
	BOOL RaidHasPlayers(class CAIRaidObj * raidPtr);
	CString GetWorldMapForRaid(int worldID);
	int								  m_raidActiveIdx;//-1 = none
	DWORD							  m_raidEndTick;
	int								  m_raidPendingIdx;//raid warned about, starts at m_raidPendingStart
	DWORD							  m_raidPendingStart;
	DWORD							  m_raidNextRoll;
	volatile int					  m_raidCmdStart;//GM request: raid index + 1
	volatile BOOL					  m_raidCmdStop;
	BOOL DirectMessage(  short radius,
							CString & message,
							D3DVECTOR position,
							int world,
							int referenceID,
							CString & targetPerson);
	BOOL SendTextMsg(   DWORD flags,
						DPNID sendTo,//can be NULL if client
						CString message,
						int referenceID,
						CPlayerObject * playerPtr,
						int type);
	BOOL SpawnDynamicAIToAvailAiServer( int aiCFG,
										int worldID, 
										D3DVECTOR position,
										int spawnID,
										long lifeLength,
										int  clanIDOpt,
										int maxDynSpawnMaxCount,
										short spawnArrayCount,
										short spawnArrayOffset,
										int spawnRotationRange,
										CString worldMapName);
	void RespawnToRebirthPoint(CPlayerObject * currentPlayer,CAccountObject * accountPtr,int * instanceTypeOvrd = 0);
	//void SpawnToPointInWld(CPlayerObject * currentPlayer,D3DVECTOR position,int wldID);
	void SpawnToPointInWld(CPlayerObject * currentPlayer,D3DVECTOR position,CString worldName,int * instanceTypeOvrd = NULL);
	void DeadBodyDisconnectPlacementCallback();
	BOOL GetFirstCollisionY(int cId,D3DVECTOR currentPosition, float yDist,D3DVECTOR * returnPosition);
	void ServerTimoutCallback(int processCount);
	void UpdateClientStatistics(CPlayerObject * player,DPID & idFromLoc);
	BOOL UpdateClientRecentQuestJournalEntry(CPlayerObject * targetPayer);
	BOOL RequestMurdersToReport(CPlayerObject * characterObj);
	BOOL SendGuerenteedCycleMsgToClient(CPlayerObject * runtimePtr);
	BOOL RequestGroupList(CPlayerObject * player,DPNID target);
	BOOL DeleteGroupMemberByHandle(CPlayerObject * player,int handle);
	BOOL GeneralReloadCommand(CPlayerObject * currentPlayer);
	BOOL SendSkillsToClient(CPlayerObject * playerFocus, bool sendTitles = false);
	BOOL SendItemLocationMsg(DWORD flags,DPNID sendTo,CServerItemObj * lstObj);
	void BroadcastInventoryChange(CPlayerObject * personWhoChangedInven,int index,DPNID messageFromDPID,BOOL  informEntity,CItemObj * refItem);
    BOOL SendEquipMsg(int index,int refrenceID,CPlayerObject * playerObjectData,int catagorie,int dupCount);
	BOOL IfItemForceJetSendIt(CItemObj * item,CPlayerObject * playerToReceive);
	BOOL SendHousingState(DPNID targetID,int worldID);
	BOOL SendUpdateToPlayer(DPNID playerId,CPlayerObject * pObj,BOOL AIMode);
	int GetNextWeaponToArm(CInventoryObjList * invList,CSkillObjectList * skills,CStatObjList * stats);
	int GetPrevWeaponToArm(CInventoryObjList * invList,CSkillObjectList * skills,CStatObjList * stats);
	BOOL HandleEquipMessageServerSide_safe(LPEQUIP_STRUCT formatedMessage,DPNID messageFromDPID);
	BOOL SendPlayerInventory(CInventoryObjList * inventory,BOOL filterNonTranferable,DPNID sendTo,int visualID,CPlayerObject * currentPlayer,int includeArmed, int filter);
	BOOL SendSkillAbilities(DWORD flags,DPNID sendTo,CSkillObject * skillPtr,int levelToUse,int glid);
	BOOL HandleDeath(CPlayerObject * requestedData,CPlayerObject * attackerData);
	void HandleDeathQuestJournalStuff(CPlayerObject * requestedData,CPlayerObject * attackerData);
	BOOL HandleDeMountRequest(CPlayerObject * hostPtr);
	CSpawnCfgObj * GetSpawnPtr(int spawnCfgIndex);
	BOOL AddExperienceToSkillByType(int type,float m_expBonus,float m_thisGainCap,CPlayerObject * targetPayer);
	BOOL SendHouseUpdateToAllAIservers(int worldID,D3DVECTOR wldPos,int type,int accountBind);
	BOOL SendSingleHousingState(DPNID targetID,int worldID,D3DVECTOR wldPos,int type,int accountBind);
	BOOL HandleMountRequest(CPlayerObject * hostPtr,int aiCfgIndex,int hostAnimStatic);
	BOOL SendStoreData(int storeID,CPlayerObject * player);
	BOOL SendHouseStoreData(CPlayerObject * player,BOOL mustOwnHouse );
	BOOL BankRequest(CPlayerObject * player,DPNID idFromLoc, int iLockerIndex = 0);
	BOOL SendQuestDialog(CPlayerObject * targetPayer,CString dialog);
	BOOL SendExtendedTextToChatWindow(CPlayerObject * targetPayer, CString text);
	BOOL HandleReassignRespawnByDetails(CPlayerObject * pData,CAccountObject * accountPtr,DPNID idFromLoc,D3DVECTOR position,CString wldName);
	void DoRadiusDamage_Heal(   int amount,
								int damageChannel,
								float radiusSz,
								CPlayerObject * hostPtr);
	BOOL RevealRadius(float radius, CPlayerObject * playerDetecting);
	BOOL SendAvailableFromItem(CPlayerObject * plPtr,int specifiedLevel,int glid);
	BOOL SendDamageInfoMessageHandler(int damageAmount,int idOfTarget,CPlayerObject * playerObj, bool death = false);
	BOOL HandleReassignRespawn_requiresAcLock(CPlayerObject * pData,DPNID idFromLoc);
	void SendInventoryBlockToLootDump(CInventoryObjList * inventory,DPNID sendTo);
	void InterpolateNetworkEntiesServerSide(CPlayerObject * pData,DWORD & CurrentTickCount);
	BOOL SendUpdateToAIChannel_safe(DPNID playerId,int worldChannel);
	BOOL SendMsgByHandle(   int handle,CString message,int type);//0 radius = to all
	DWORD GetElaspeTimeMSDW(DWORD & lastStampPoint);
	DWORD GetTimeStampDW();
	CString SanitizeFormatIn(CString text);
	CString SanitizeFormatOut(CString text);
	BOOL SendClientScript(CPlayerObject * targetPayer,CString script,CString text);
	BOOL UpdatePhysicsBlock(CPlayerObject * playerObject);
	void MovePlayerInWorld(CPlayerObject * currentPlayer, float x, float y, float z);
	void RR_MovePlayerByOffset(CPlayerObject * currentPlayer, float xOffset, float yOffset, float zOffset);
    BOOL SpawnAIChannel(int channel,CString aiWorldEXG);
	BOOL SendGroupMessage(CPlayerObject *player, SQUAD_MESSAGE *groupInfo, int amount);
	BOOL CloseAIChannel(int channel);
	BOOL SpawnZone_async(CDBConnectionSystem * hubConnection,CString exgName,int typeID,int & newChannel);
	BOOL EnablePendingZone_async(CDBConnectionSystem * hubConnection,CString exgName,int instanceType,int * channelReturn,CString & dynRules);
	void WorldRuleTimeCallback_safe(int searchCount);
	void SpawnToRespawnPoint_requiresAcLock(CPlayerObject * currentPlayer,int * instanceTypeOvrd = 0);
	BOOL SpawnFirstFromChannelToRespawn(int wldID,int * instanceTypeOvrd = 0);
	BOOL HandleBounty(CPlayerObject * requestedData,CPlayerObject * attackerData);
	BOOL HandleArenaPoints(CPlayerObject * requestedData,CPlayerObject * attackerData);
	BOOL NotifyWhatKilledPC(CPlayerObject * requestedData,CPlayerObject * attackerData);
	BOOL HandleArenaRulesOnDeath(int & stencilChannelStore,int & channelStore);
	BOOL OnDeathDataProcessByDPNID(	CPlayerObject * victimPlayerObject,
											BOOL nonDrop,
											CMovementObj * entObjDB,
											float respawnHealthPercentage,
											void * m_serverScriptsMEM,
											void * m_netComObjectMEM);
	BOOL HandleGroupData(unsigned int iGroupID);
	BOOL HandleGroupData_safe(unsigned int iGroupID);
	BOOL SendSquadData_safe(CPlayerObject *pPlayer);
	BOOL SendSquadData(CPlayerObject *pPlayer);
	BOOL PrepareGroupData(CAxList *pPlayerList, unsigned int iGroupID, int &iDataFiller);
	BOOL SendTextMsg(   DPNID sendTo,//can be NULL if client
						CString message,
						CPlayerObject * playerPtr,
						int type = 0);
};
