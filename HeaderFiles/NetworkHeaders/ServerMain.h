#pragma once
#include "sharednet.h"
#include "attacksystemsmanagement.h"
#include "servermovementsystems.h"
#include "arenasystems.h"
#include "spawnsystems.h"
#include "aisystems.h"
#include "charactercreationsystems.h"
#include "attributesystems.h"
#include "damagesystems.h"
#include "systemsloader.h"
#include "textsystems.h"
#include "questsystems.h"
#include "EventSystems.h"




struct AB_serverStatusLive
{
    unsigned long                        m_playersOnline;
	unsigned long                        m_maxSpikePlayersOnline;
	unsigned long                        m_msgsOutPerSecond;
	unsigned long                        m_msgsInPerSecond;
	unsigned long                        m_bandwidthOutgoingPerSecond;
	unsigned long                        m_bandwidthIncommingPerSecond;
	unsigned long                        m_totalBandwidthIn;
	unsigned long                        m_totalBandwidthOut;
	unsigned long                        m_totalBandwidthTotal;
};

class CServerMain
{
public:
	CServerMain(void);
	~CServerMain(void);

	//Variables
    CSharedNet                      * m_sharedInterface;
	CAttackSystemsManagement        * m_attackSystemsManagment;
	CServerMovementSystems          * m_movementSystems;
	CArenaSystems                   * m_arenaSystems;
	CSpawnSystems                   * m_spawnSystems;
	CAiSystems                      * m_aiSystems;
	CCharacterCreationSystems       * m_charCreationSystems;
	CAttributeSystems               * m_attributeSystems;
	CTextSystems                    * m_textSystems;
	CQuestSystems                   * m_questSystems;
	CDamageSystems                  * m_dmgSystems;
	CEventSystems					* m_eventSystems;
	CSystemsLoader                  * m_systemsLoader;
	BOOL                              m_systemShutDown;
	CRITICAL_SECTION                  m_interactSection;
	AB_serverStatusLive               m_serverStatus;
	int                               m_serverBaseThreadCount;
	int                               m_shutDownThreadCount;
	CRITICAL_SECTION				  m_instanceRouterSection;


	//shutdown/restart mode
	int                               m_shuttingDown;
	int                               m_rebootOnShutdown;
	CString                           m_shutDownNotification;
	DWORD                             m_sutDownStartStamp;
	DWORD                             m_shutDownNotificationDuration;


    //status
	HANDLE                            m_sqlStatusUpdater;
	int								  m_processRotation;

	//GroupInfo
	const unsigned int				  SQUAD_MESSAGE_BURST_SIZE;
	SQUAD_MESSAGE					 *m_groupDataBlock;
	
	
	

	

	CRITICAL_SECTION                  m_statusQueryLock;
 

	//Functions
	void ServerCallback(int & channel);
	int  StartServer();
	BOOL Security(  CPlayerObject * playerObjectData,
					CAccountObject * accountPtr, 
					DWORD & currentTickCount,
					LPSHRUNK_TOSVRMOV_STRUCT formatedMessage,
					DPNID & idFromLoc);
	BOOL HandleEngineMessagesServerSide_safe(LPVOID lpMessage,DPNID idFromLoc,int msgSize);
	BOOL HandleMovData_safe(LPSHRUNK_TOSVRMOV_STRUCT formatedMessage,DPNID & idFromLoc);
	//BOOL HandleGroupData_safe(DPNID &idFromLoc);
	//BOOL HandleGroupData_safe(unsigned int iGroupID);
	BOOL HandleDotData(CPlayerObject *playerObjectData,CAccountObject * accountPtr);
	BOOL AccountAssignment(LPVOID & lpMessage,DPNID & idFromLoc,int & msgSize);
	BOOL HandleCommandMsg_safe(LPVOID & lpMessage,DPNID & idFromLoc,int & msgSize);
	BOOL ProcessGuarenteedMsg_safe(LPVOID & lpMessage,DPNID & idFromLoc,int & msgSize);
	BOOL ProcessBlockAttribMsgs_safe(LPVOID & lpMessage,DPNID & idFromLoc,int & msgSize);
	CRITICAL_SECTION GetInteractSection();//call before thread creation
	BOOL AdminAcRequestConfirm(LPVOID & lpMessage,DPNID & idFromLoc,int & msgSize);
	BOOL HandleSpawn_safe(LPVOID & lpMessage,DPNID & idFromLoc,int & msgSize);
	BOOL HandleMovFire_safe(LPVOID & lpMessage,DPNID & idFromLoc,int & msgSize);
	BOOL FireMsgHandler(LPVOID & lpMessage,DPNID & idFromLoc,int & msgSize);
	BOOL MovMsgHandler_safe(LPVOID & lpMessage,DPNID & idFromLoc,int & msgSize);
	void ClearLiveLog(CString tableName);
	void ClearLogOnFlagsForServerID(int svrID);
	BOOL ProcessGuarenteedMsgAI_safe(LPVOID & lpMessage,DPNID & idFromLoc,int & msgSize);
	void AddResponseCommand(CString commandType, 
									 CString peram1, 
									 CString peram2, 
									 CString peram3,
									 CString peram4,
									 CString peram5,
									 CString peram6);
	void HandleBackupInterval(CPlayerObject * playerObjectData,CAccountObject * accountPtr, DWORD & currentTickCount);
	BOOL HandlePlayerStasis(CPlayerObject * playerObjectData,CAccountObject * accountPtr, DWORD & currentTickCount);
	BOOL RebootServer();
	void ServerItemGeneratorsCallback(CServerItemList * runtimeItems,CServerItemGenObjList * serverGeneratorsList,int processCount);
	//BOOL HandleGroupData(unsigned int iGroupID);
};
