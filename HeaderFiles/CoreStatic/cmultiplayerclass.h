#pragma once

#include "dplay8.h"
#include "dplobby.h"
#include "dbconnectionsystem.h"
#include "CABFunctionLib.h"
#include "CDplayPlayerClass.h"
#include "CStructuresMisl.h"
#include "CAccountClass.h"
#include "CWorldAvailableClass.h"
#include "CPacketQueClass.h"
#include "CServerEventLogClass.h"
#include "matrixarb.h"
#include "CServerItemClass.h"
#include "CBlockIpListClass.h"
#include "CNetMonSysClass.h"
#include "CClanSystemClass.h"
#include "CHousePlacementClass.h"
#include "CBountySystem.h"
#include "pendinglogonslist.h"
#include "cdatahandler.h"
#include "characterpendingsave.h"
#include "Mmsystem.h"
#include <limits.h>
#include "CMenuInventory.h"
#include "AuraNetHost.h"   // modern ENet transport (replaces DirectPlay)

//--------------------------------------------------------------//
class CMultiplayerObj : public CObject
{
private:
	int				m_ActiveItem[2]; //[0] = Visual ID for Quick Slot [1] = GLID to Use
	unsigned long	m_totalBandwidthIncoming;
	BOOL			m_enabledClient;
	CString			m_connectionErrorCode;
	int             m_connectionError;

public:
	CMenuInventoryItem** Quickbar(int iBar, int iSlot, CMenuInventoryItem* pItem = NULL);
	bool IsQuickbarFull();
	bool IsInQuickbars(CMenuInventoryItem *pItem);
	CMenuInventoryItem** GetSelectedQuickbarItem();

	int m_iQuickbarLastActivatedIndex;
	int m_iQuickbarSelectedIndex;
	bool m_bIsQuickBarInitialized;

private:
	CMenuInventoryItem*	m_Quickbars[NUM_QUICKBARS][NUM_QUICKBAR_SLOTS];
	
public:
	CMultiplayerObj();
    //CRITICAL_SECTION			g_packetRecieve;

    CString						m_serverKey;

    //Server only vars
	int                         m_maxPlayers;
    CPlayerObjectList			* m_runtimePlayersOnServer;
	CPlayerObjectList			* m_runtimeItemsOnServer;
	CPacketObjList				* m_packetQue;
	CServerEventList			* m_eventLog;
	CNetMonSystem				* m_netMonSys;
	int							m_serverMode;

	
	byte						m_axThreadPaused;

	//CClanSystemObjList          * m_clanDatabase;
	
	CServerItemList             * m_runtimeItems;
    //variables
	
	CABFunctionLib				m_cabLib;
	BOOL                        m_errorExists;
	CString                     m_errorMsg;                          
	BOOL                        m_passwordRequired;
	
	int                         m_frequencyLimit;
	int                         m_frequencyBuffer;
	int                         m_frequencyIncrement;
	int                         m_currentPlayersOnServer;
	BOOL                        m_serverStarted;
	BOOL                        m_clientSideFireing;
	CString                     m_userName;
	CString                     m_passWord;
    BOOL                        m_PersistentType;
	CString                     m_servername;
	BOOL                        m_isHost;
	DPNID                       m_thisPlayerID;
	BOOL                        m_messagingEnabledDplay;
	CString                     m_IPaddress;
	
	unsigned long               m_totalBandwidthOutGoing;
	
	unsigned long               m_packetsOutPerPass;
    unsigned long               m_packetsRecievedPerPass;
	int                         m_bandwidthAvailable;
	int                         m_communicationFrequency;
	float                       m_serverCalculateRadius;
    float                       m_serverCalculateRadiusItems;
	CWorldAvailableObjList *    m_currentWorldsUsed;
	CSpawnCfgObjList *          m_spawnPointCfgs;
	DWORD                       m_deadEntityDissolveTime;
	DWORD                       m_deadEntityNPCDissolveTime;
	int                         m_interpolationOnNetworkEntities;
	BOOL                        m_AIServer;
	long                        m_AIUpdateFrequency;
	long                        m_AIUpdateStamp;
	BOOL						m_primaryPlayerCreated;
	DWORD                       m_lagMSTolerance;
	DWORD						m_lastCommunicationTime;
    long                        m_clientCharacterMaxCount;
	DWORD                       m_timeOutDuration;//time out duration for runtime characters

    int                         m_currentCharacterInUse;//-1 none

	long                        m_maxMessagesOutGoing;

	DWORD                       m_aiserverCycleDuration;
	long                        m_cashCount;
	long                        m_bankCashCount;
	MatrixARB                   GL_matrixFunctions;
	BOOL                        m_requestingPortal;
	long                        m_serverStartTime;
	long                        m_autoBackupFrequency;
	CString                     m_autoBackupFile;
	BOOL                        m_enableAutoBackup;
	long                        m_autoBackupTimeStamp;
	int							m_currentVersion;
	

	int							m_menuQue;
    short                       m_idSearchStart;

	short                       m_localCLSecurityIndexer;

	CBlockIPList				*m_ipBlockList;

	BOOL                        m_enabledCommunicationInit;


	BOOL                        m_chatEnabled;
	BOOL                        m_groupChatEnabled;
	BOOL                        m_clanChatEnabled;

	int                         m_potentialGroupJoiner;
	CString						m_squadName;

    //global client track
	int                         m_itemGlobalMaxCharges;//if zero its infinet
	int                         m_itemGlobalCurCharges;
	DWORD						m_rechargeTimerStart;
	DWORD						m_rechargeTimer;
	int							m_itemNeededPerFire;
	CElementStatusList          *m_elementStatus;//server updated
	int                         m_currentStat1;//str
	int                         m_currentStat2;//dex
	int                         m_currentStat3;//wis
	CString                     m_titleLink;
	CString                     m_currentStasis;
	CString                     m_currentlyTradingWith;
	CString                     m_ingameNameRef;

	BOOL                        m_nonPkServer;

	int                         m_packetsDropped;
	

    int                         m_currentBackupSlot;

	//Bonus Tracking
	int							m_CurBonusTimeStat;
	int							m_MaxBonusTimeStat;
	int							m_StatIcon;
	long						m_StatTimeStamp;
	int							m_CurBonusTimeResist;
	int							m_MaxBonusTimeResist;
	int							m_ResistIcon;
	long						m_ResistTimeStamp;
	int							m_CurBonusTimeDamage;
	int							m_MaxBonusTimeDamage;
	int							m_DamageIcon;
	

	//Sys Timer Vars
	long                        m_manditoryWaitBeforeSkillUse;


	//autobackup
    //CArchive					*m_autobackupArchive;
	//int						m_autobackupCurrentSel;
	//CFileException			m_autoBackupexc;
    //CFile						*m_autoBackFl;

	//Global Limits
	int                         m_maxItemLimit_onChar;
	int                         m_maxItemLimit_inBank;
	DWORD                       m_maxBandwithPerPerson;
	DWORD                       m_maxPacketsPerChar;
	DWORD                       m_packetTimeout;
	DWORD                       m_sceneStatePackTimeout;
    DWORD                       m_imediatePackTimeout;//bullets etc
	//client side timeoutspecs
	DWORD                       m_clientPacketTimeout;
	DWORD                       m_clientScenePacketTimeout;
	DWORD                       m_clientImediatePacketTimeout;
	
	DWORD						m_maxPacketSizeNonAdmin;
	DWORD						m_maxPacketCountPerSecond;
	DWORD                       m_maxRecieveTime;

	DWORD                       m_lastMovMsgRecievedTime;
	DWORD                       m_lagLimitOnSceneEffect;


	//
	int                         m_murderCountMax;//till murderer status
	int							m_currentMurderCount; //Player's current murder count
	LONGLONG                    m_timeElStamp;
	
	CPacketObjList              * m_clientCfgRequestOutbox;
	CPacketObjList              * m_attributeOutBox;
	CPacketObjList              * m_equipOutBox;
	CPacketObjList              * m_txtOutBox;
	CPacketObjList              * m_dmgOutBox;
	CPacketObjList              * m_txtgroupOutBox;
	CPacketObjList              * m_fireOutBox;
	CPacketObjList              * m_miscOutBox;

	CHousingZoneDB              * m_houseingDB;   


	bool						m_bRecentJournalUpdate;
	CQuestJournalObj            *m_recentQuestPtr;
	CQuestJournalList			*m_journalEntries;
	CBountyObjList              * m_bountySystem;

	CRITICAL_SECTION			m_bandwidthPacketTracker;
	CRITICAL_SECTION			m_primaryServerThreadControllerSw;
	CRITICAL_SECTION			m_statusLock;
	CRITICAL_SECTION			m_varLock;
	CRITICAL_SECTION			m_packetPerPassCountLock;
	CRITICAL_SECTION			m_bandwidthIncommingLock;
	CRITICAL_SECTION			m_connectionErrorSect;
	HANDLE						* m_pausePrimarySpinEvent;
	int							m_primarySpinCount;
	CRITICAL_SECTION			m_triggerPrimarySect;
	int							m_triggerPrimaryCurrent;


    int							m_highestConstantPlayersOnline;
	int							m_currentPlayersOnline;
	int							m_goodBehaviorDuration;//till murder count reduces

	int							m_maxCfgRequestsPerRound;
	int							m_pkLevelRestriction;


	float						m_maxMovementDistance;
	int							m_violationsToDisconnect;


	DWORD						m_currentRetrySendDuration;
    DWORD						m_retrySendDuration;


	float                       m_houseSightRange;
	


	int                         m_dmgRngBaseMaxPerc;
	int                         m_dmgRngBaseMinPerc;


	CAccountObjectList			*m_runtimeAccountDB;

    int                         m_vicinityTolerance;
	int                         m_assignedIDGlobal;

	//autobak variables
	BOOL                        m_autoBackWriting;
	HANDLE                      m_workerThreadHandle;
	//CClanSystemObjList		* m_staticClanDatabase;
	//CHousingZoneDB			* m_staticHouseingDB; 
	DWORD						dwThrdParam; 
    DWORD						m_workerThreadID;
	DWORD                       m_invunDuration;
	//POSITION					m_autoBackupPos;
	BOOL                        m_attempConnectionEnabled;
	DWORD                       m_attemptConnectionStamp;

    //Other plugin systems
	CPendingLogonsList			*m_pendingLogons;
	CPendingLogonsList			*m_pendingLogoffs;
	CCharacterPendingSave		*m_characterPendingSaveSystem;


	int                         m_authenticationMode;
	int							m_accountDBMode;

	CString                     m_playingGameName;

	int                         m_logonProcThreadCount;
	HANDLE                      *m_logonProcThreadHanldes;
	CRITICAL_SECTION            m_logonSection;
	BOOL                        m_shutDownLogonThreads;
	int							m_serverID;
	          

	//datadase Update Info
	CString						m_sqlServerIP;
	CString						m_sqlServerCatalog;
	CString						m_sqlServerUsername;
	CString						m_sqlServerPassword;
	long						m_sqlServerTimeStamp;
	BOOL						m_sqlServerUpdateEnabled;
	CString						m_sqlServerTable;
	int							m_newbieLevel;
	int							m_allowOnlyPaidAccountsToConnect;


	//datadase target status Update Info
	CString						m_sqlStatusServerIP;
	CString						m_sqlStatusServerCatalog;
	CString						m_sqlStatusServerUsername;
	CString						m_sqlStatusServerPassword;
	CString						m_sqlStatusServerTable;

	IDirectPlay8Address			* m_pDP8AddressHost;
    IDirectPlay8Address			* m_pDP8AddressLocal;

	CString						m_hostIPString;

	//new revision
	IDirectPlay8Client			* m_clientInterface;
	IDirectPlay8Server			* m_serverInterface;
	AuraNetHost					* m_netHost;   // new transport; coexists with DirectPlay during the swap
	HANDLE						  m_netPumpThreadHandle; // ENet receive-pump thread (server)
	volatile bool				  m_netPumpRun;          // pump loop control
	std::map<AuraNetID, LPCONTEXT_VALIDATION> m_netContext; // per-peer login context (ENet) - replaces DirectPlay pvPlayerContext
	DWORD						m_port;
	DPNHANDLE					m_dpAsncEnumHostHandle;






	///////////////
	// Fucntions //
	///////////////

	//new revision    
	BOOL CRCInValid(float & CRCONE,float & CRCTWO);
	void SafeDelete();
	BOOL EnumHosts(CString TCPIP);
	BOOL SendItemChkToClientMsg(DWORD flags,DPNID sendTo,long nID);
	BOOL SendPingMsg(DWORD flags,
		             DPNID sendTo,
				     short data);
	BOOL Disconnect(DPNID id);
	//void CommunicationFrequencyAdjust();
	//BOOL SendUpdateToPlayer(DPNID playerId,CPlayerObject * pObj,BOOL AIMode);
	 BOOL GetNextPacket_unsafe(int * size,BYTE ** data,DPNID * id);

    //CRC
	float GetMovMsgCrC(LPSHRUNK_TOSVRMOV_STRUCT packet);
    short GetLogMsgCrC(LPLOG_STRUCT packet);
	short GetEquipCRC(LPEQUIP_STRUCT equipStruct);
	short GetAttribCRC(LPATTRIB_STRUCT attribStruct);
	short GetFireCRC(LPFIRE_STRUCT fireStruct);
	short GetSpawnCRC(LPSPAWN_STRUCT spawnStruct);
	short GetTxtMsgCrC(LPTXT_MSG packet);
	short GetItemLocationCRC(LPITEM_LOCATION iLocationStruct);

    //Messageing
	BOOL SendSpawnMsg(DWORD flags,
				      DPNID sendTo,
					  short dbIndex,
					  CString worldFile,
					  int traceNumber,
				      short uniInt1,
					  short uniInt2,
				      short uniInt3
				      );
	BOOL SendLogFast(DWORD flags,
								  DPNID sendTo,
								  CString userName,
								  CString password);

    SHRUNK_LPMOVE_STRUCT ShrinkData(LPMOVE_STRUCT OLD_DATA,SHRUNK_LPMOVE_STRUCT SHRUNK_DATA);
    LPMOVE_STRUCT ExpandData(SHRUNK_LPMOVE_STRUCT SHRUNK_DATA,LPMOVE_STRUCT DATA);
    BOOL SendMenuBuildPacket(                        DWORD flags,
								                     DPNID sendTo,
											         short messageType,
								                     CString uniText,
											         short miscInt);
	short GetBuildMenuMsgCrC(LPMENUBUILD_STRUCT packet);
	BOOL EnableServer();
	BOOL EnableClient(CString IPaddress,int version);
	/*static HRESULT WINAPI DirectPlayMessageHandler( PVOID pvUserContext, 
                                                    DWORD dwMessageId, 
                                                    PVOID pMsgBuffer );*/
	void ConvertGenericStringToWide( WCHAR* wstrDestination, 
												  const TCHAR* tstrSource, 
                                                  int cchDestChar );
	void ConvertAnsiStringToWide( WCHAR* wstrDestination, const CHAR* strSource, 
                                     int cchDestChar );
	void WINAPI ConvertWideStringToAnsi( CHAR* strDestination, const WCHAR* wstrSource, 
                                     int cchDestChar );
	void SendData(BYTE * data,
				  DWORD size,
			      DWORD flags,
				  DPNID sendTo,
			      DWORD timeout);
	BOOL StopEnumerating();
	CString WINAPI ConvertWideStringToCString(const WCHAR* wstrSource);
	CString WINAPI GetIPAddress(IDirectPlay8Address * pAddress);
	CString WINAPI GetIPAddress(DPNID id);
	void PumpNetwork();   // drain ENet events into the packet queue (server side)
	void HandleLoginPacket(AuraNetID id, const unsigned char * data, unsigned long size); // server: validate first packet (3d)
	void HandlePlayerLogoff(AuraNetID id);   // server: port of DPN_MSGID_DESTROY_PLAYER (3d)
	void SendLoginReject(AuraNetID id, char code); // server: reply code + disconnect (3d)
	/*BOOL SendTextMsg(DWORD flags,
					 DPNID sendTo,//can be NULL if client
					 CString message,
					 int referenceID,
					 CPlayerObject * playerPtr,
					 int type);*/
	/*void BroadcastMessage(short radius,
									   CString message,
									   D3DVECTOR position,
									   int world,
									   int referenceID);//0 radius = to all*/
	//void ServerTimoutCallback();
	BOOL LogsPlayerOut_requiredAccountLock(CPlayerObject * runtimePtr);
	//int GetUntakenAccountNumber();
	BOOL UpdateAccount(CPlayerObject  * runtimePlayerObj,CAccountObject * accountPtr);
	 int GetFreeRuntimeEntityNetworkID();
	BOOL SendNameMsg(DWORD   flags,
					 DPNID   sendTo,//can be NULL if client
					 short   networkAssignedID,
					 short   messageType,
					 CString message);
	/*BOOL SendAIBlockMsg(     DWORD flags,
							 DPNID sendTo,
						     SHRUNK_MOVE_STRUCT * m_dataBlock,
						     int blockSize
					   );*/
	BOOL SendByteBlockMsg(    DWORD flags,
							  DPNID sendTo,
							  BYTE * m_dataBlock,
							  int blockSize,
							  DWORD timeout
						     );
	LPSHRUNK_TOSVRMOV_STRUCT ShrinkDataCP(LPMOVE_STRUCT OLD_DATA,
										  LPSHRUNK_TOSVRMOV_STRUCT SHRUNK_DATA); 
	LPMOVE_STRUCT ExpandDataCP(LPSHRUNK_TOSVRMOV_STRUCT SHRUNK_DATA,LPMOVE_STRUCT DATA);
	float GetDamageMsgCrC(LPHITPLAYER_DATA packet);
	BOOL SendTimeSyncPacket( DWORD flags,DPNID sendTo,DWORD timePassedSync,short freqSync );
	BOOL SendTextMsgToSquad(DWORD flags,DPNID sendTo,CString message,int referenceID);
	BOOL SendTextMsgToGroup(DWORD flags,
								        DPNID sendTo,//can be NULL if client
								        CString message,
								        int referenceID);
	void SnapShotCurrentPacketQue();
	CPlayerObject * GetObjectByAliveAndAccountNumber(int accNum);
	long GetVersionFromFile();//ussually client side
	CString GetIPDataFromDisk();
	void WriteDebugFile(CString filename,CString debugString);
	void UNBanIPByUsername(CString username);
    void BanIPByUsername(CString username);
	BOOL SendAttribMsg(DWORD flags,
							   DPNID sendTo,
							   int attribute,
							   int uniInt,
						       long refrenceID,
							   int uniInt2,
							   short uniShort3,
							   CPlayerObject * runtimePlOption);
	void SetMultithreadSafeFlags();
	int WINAPI DBLogonProcessor(	CString userName,
									CString password,
									CString currentIP,
									CDBConnectionSystem * authConnection);
	int SVRExporter();
	int FinalizeLogon(CString & userName, CDBConnectionSystem * dbConnection,CDBConnectionSystem * authConnection);
	void Disconnect();
	void UpdatePlayersOnline(int adjustAmount);
	void AddPacketPerPass();
	DWORD GetLastCommunicationTime();
	void SetEnabledClient_safe(BOOL enabled);
    BOOL GetEnabledClient_safe();
	unsigned long GetTotalBandwidthIncomming_safe();
    void AddToTotalBandwidthIncomming_safe(unsigned long add);
	void ZeroTotalBandwidthIncomming_safe();
	int RotateTrigger_safe();
	void InitSpinLocks(int count);
	void SetConnectionStatus(CString desc, int code);
	void GetConnectionStatus(CString * desc, int * code);
	int GetConnectionStatus();
	CString GetConnectionStatusDesc();
};

class CMultiplayerObjList : public CObList
{
public:
   CMultiplayerObjList(){};
    
     
	DECLARE_SERIAL( CMultiplayerObjList );
	
    
};

