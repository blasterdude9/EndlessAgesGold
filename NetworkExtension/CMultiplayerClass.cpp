

#include "stdafx.h"
#define D3D_OVERLOADS
#define NAMEMAX 200

#include <d3d9.h>
#include <afxtempl.h>
#include "CMultiplayerClass.h"
#include "CStructuresMisl.h"
#include <math.h>
#include <direct.h>
#include "WINDOWSX.H"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//GUID g_guidApp = { 0xede9493e, 0x6ac8, 0x4f15, { 0x8d, 0x1, 0x8b, 0x16, 0x32, 0x0, 0xb9, 0x66 } };

//GUID g_guidApp = { 0x5296ffaf, 0x9fc1, 0x4c73, { 0x84, 0x7c, 0xa, 0xeb, 0x72, 0xfd, 0xb48, 0x7e } };
GUID g_guidApp = { 0x5296ffaf, 0x9fc1, 0x4c73, { 0x84, 0x7c, 0xa, 0xeb, 0x72, 0xfd, 0x48, 0x7e } };

//WARNING: C4309 --> truncation of constant value
//{5296FFAF-9FC1-4C73-847C-A0EB72FD487E}

DWORD WINAPI LogOnProcThread(LPVOID lpParameter);
DWORD WINAPI NetPumpThread(LPVOID lpParameter);
HRESULT WINAPI DirectPlayMessageHandler( PVOID pvUserContext, 
                                         DWORD dwMessageId, 
                                         PVOID pMsgBuffer );

CMultiplayerObj::CMultiplayerObj()
{
	InitializeCriticalSection(&m_logonSection);
	m_allowOnlyPaidAccountsToConnect	= 0;
	m_triggerPrimaryCurrent				= 0;
	m_primarySpinCount					= 0;
	m_serverID                          = 1;
	m_logonProcThreadCount				= 0;
	m_shutDownLogonThreads              = FALSE;
	m_logonProcThreadHanldes			= NULL;
	m_pDP8AddressHost                   = NULL;
	m_pDP8AddressLocal                  = NULL;
	m_errorExists						= FALSE; 
	m_assignedIDGlobal					= 0;
	m_autoBackWriting					= FALSE;
	m_workerThreadHandle				= NULL;
	DWORD test							= 23823912;
	DWORD ackerman						= 38723912;
	m_frequencyLimit                    = 500;
	m_bandwidthAvailable                = 5600;//bytes per second
	m_currentPlayersOnServer            = 0;
	m_totalBandwidthIncoming            = 0;
	m_clientSideFireing                 = FALSE;
	m_PersistentType                    = TRUE;
	m_messagingEnabledDplay             = FALSE;
	m_isHost                            = FALSE;
	m_clientInterface                   = NULL;
	m_serverInterface                   = NULL;
	m_netHost                           = NULL;
	m_netPumpThreadHandle               = NULL;
	m_netPumpRun                        = false;
	m_enabledClient                     = FALSE;
	m_IPaddress                         = "";
	m_userName                          = "";
	m_passWord                          = "";
	m_totalBandwidthOutGoing            = 0;
	m_maxPlayers                        = 200;
	m_packetsOutPerPass                 = 0;
	m_packetsRecievedPerPass            = 0;
	m_serverStarted                     = FALSE;
	m_communicationFrequency            = 200;
	m_frequencyBuffer                   = 50;
	m_frequencyIncrement                = 25;
	m_serverCalculateRadius             = 3000;
	m_serverCalculateRadiusItems        = 1000;
	m_serverKey                         = "xpass01101aa2";
	m_servername                        = "AuraConnect";
	m_passwordRequired                  = TRUE;
	m_runtimePlayersOnServer            = NULL;
	m_runtimeItemsOnServer              = NULL;
	m_deadEntityDissolveTime            = 60000*10;//10 min
	m_deadEntityNPCDissolveTime         = 60000*4;//4 min
	m_interpolationOnNetworkEntities    = 200;
	m_currentWorldsUsed                 = NULL;
	m_AIServer                          = FALSE;
	m_AIUpdateFrequency                 = 250;
	m_AIUpdateStamp                     = 0;
	m_port                              = 25857;
	m_packetQue                         = NULL;
	m_dpAsncEnumHostHandle              = NULL;
	m_eventLog                          = NULL;
	m_thisPlayerID                      = 0;
	m_primaryPlayerCreated              = FALSE;
	m_timeOutDuration                   = 30000;//30 seconds
	m_lagMSTolerance                    = 5000;//ten seconds of no communication stops gameplay 
	m_lastCommunicationTime             = 0;
	m_clientCharacterMaxCount           = 3;
	m_spawnPointCfgs                    = NULL;
	m_currentCharacterInUse             = -1;
	m_maxMessagesOutGoing               = 50;
	m_aiserverCycleDuration             = 250;
	m_menuQue                           = -1;
	m_cashCount                         = 0;
	m_bankCashCount                     = 0;
	m_requestingPortal                  = FALSE;
	m_serverStartTime                   = 0;
	m_autoBackupFrequency               = 60000;//1 minutes;
	m_autoBackupFile                    = "AutoBackup";
	m_enableAutoBackup                  = FALSE;
	m_autoBackupTimeStamp               = 0;
	m_idSearchStart                     = 1;
	m_runtimeItems                      = NULL;
	m_currentVersion                    = 0;
	m_localCLSecurityIndexer            = 0;
	m_ipBlockList                       = 0;
	m_enabledCommunicationInit          = FALSE;
	m_chatEnabled                       = FALSE;
	m_groupChatEnabled                  = FALSE;
	m_clanChatEnabled                   = FALSE;
	m_potentialGroupJoiner              = -1;
	m_itemGlobalMaxCharges              = 0;
	m_rechargeTimerStart				= 0;
	m_rechargeTimer						= 0;
	m_itemGlobalCurCharges              = 0;
	m_itemNeededPerFire					= 0;
	m_elementStatus                     = NULL;
	m_currentStat1                      = 0;
	m_currentStat2                      = 0;//dex
	m_currentStat3                      = 0;//wis
	m_manditoryWaitBeforeSkillUse       = 6000;
	//m_autobackupCurrentSel              = 0;
	//m_autobackupArchive                 = NULL;
	//m_autoBackFl                        = NULL;
	m_netMonSys                         = NULL;

	m_maxItemLimit_onChar               = 60;
	m_maxItemLimit_inBank               = 250;
	m_maxBandwithPerPerson              = 2500;//in bytes
	m_maxPacketsPerChar                 = 20;
	m_maxPacketSizeNonAdmin             = 2500;
	m_maxPacketCountPerSecond           = 16;
	m_maxRecieveTime                    = 1000;


	m_packetTimeout                     = 60000;
	m_sceneStatePackTimeout             = 2500;
	m_imediatePackTimeout               = 1100;
	m_clientPacketTimeout               = 45000;
	m_clientScenePacketTimeout          = 1500;
	m_clientImediatePacketTimeout       = 1100;//FIRE / DMG
	m_lastMovMsgRecievedTime            = 0;
	m_lagLimitOnSceneEffect             = 2000;//if client doesnt recive his cycle messge that defines ping he will not send out specific data

	m_clientCfgRequestOutbox            = NULL;
	m_attributeOutBox                   = NULL;
	m_equipOutBox                       = NULL;
	m_txtOutBox                         = NULL;
	m_dmgOutBox                         = NULL;
	m_txtgroupOutBox                    = NULL;
	m_fireOutBox                        = NULL;
	m_miscOutBox                        = NULL;
	m_recentQuestPtr                    = NULL;
	m_journalEntries					= NULL;
	this->m_bRecentJournalUpdate		= false;
	m_murderCountMax                    = 7;
	m_currentMurderCount				= 0; //This will be updated whenever a player's murder statis changes or they open the character screen.
	m_highestConstantPlayersOnline      = 0;
	m_currentPlayersOnline              = 0;
	m_goodBehaviorDuration              = 300000;//5 min. THIS NUMBER IS OVERWRITTEN IN SYSTEMSLOADER line 74!
	m_houseingDB                        = NULL;
	m_maxCfgRequestsPerRound            = 10;
	m_pkLevelRestriction                = 0;
	m_maxMovementDistance               = 350.0f;//per second
	m_violationsToDisconnect            = 24;
	m_currentRetrySendDuration          = 1000;
	m_retrySendDuration                 = 1000;
	m_houseSightRange                   = 2000;
	m_sqlServerIP                       = "";
	m_sqlServerUsername                 = "";
	m_sqlServerPassword                 = "";
	m_sqlServerCatalog                  = "";
	m_sqlServerTimeStamp                = GetTickCount();
	m_sqlServerUpdateEnabled            = FALSE;
	m_sqlServerTable                    = "";
	m_newbieLevel                       = 10;
	m_dmgRngBaseMaxPerc                 = 10;
	m_dmgRngBaseMinPerc                 = -30;
	m_runtimeAccountDB                  = NULL;
	m_vicinityTolerance                 = 50;
	m_invunDuration                     = 30000;
	m_serverMode                        = 0;
	m_nonPkServer                       = FALSE;
	m_timeElStamp                       = 0;//GetTickCount();
	m_bountySystem                      = NULL;
	m_packetsDropped                    = 0;
	m_eventLog							= NULL;
	m_pendingLogons                     = NULL;
	m_pendingLogoffs                    = NULL;
	m_characterPendingSaveSystem        = NULL;

	m_axThreadPaused                    = 0;
	m_currentBackupSlot                 = 0;
	m_attempConnectionEnabled           = FALSE;
	m_attemptConnectionStamp            = GetTickCount();
	m_authenticationMode                = 0;
	m_accountDBMode						= 1;

	m_CurBonusTimeStat					= 0;
	m_MaxBonusTimeStat					= 0;
	m_CurBonusTimeResist				= 0;
	m_MaxBonusTimeResist				= 0;
	m_CurBonusTimeDamage				= 0;
	m_MaxBonusTimeDamage				= 0;
	m_connectionError                   = 0;
	m_StatIcon							= -1;
	m_ResistIcon						= -1;
	m_DamageIcon						= -1;
	m_StatTimeStamp						= GetTickCount();
	m_ResistTimeStamp					= GetTickCount();

	//Quick Bars and Active Item
	m_bIsQuickBarInitialized			= false;
	m_iQuickbarSelectedIndex			= -1;
	m_iQuickbarLastActivatedIndex		= -1;
	for (int i=0; i<NUM_QUICKBARS; i++)
	{
		for (int j=0; j<NUM_QUICKBAR_SLOTS; j++)
		{
			this->m_Quickbars[i][j] = NULL;
		}
	}

	CFileException exc;
	CFile fl;
  
	CArchive the_Archive(&fl,CArchive::load);
	BOOL res = fl.Open("serverstartup.cfg",CFile::modeRead, &exc);
	if(res)
	{
		CString misc;
		the_Archive.ReadString(m_sqlServerIP);
		the_Archive.ReadString(m_sqlServerUsername);
		the_Archive.ReadString(m_sqlServerPassword);
		the_Archive.ReadString(m_sqlServerCatalog);
		the_Archive.ReadString(m_sqlServerTable);
		if(the_Archive.ReadString(misc))
			m_pkLevelRestriction = atoi(misc);
		if(the_Archive.ReadString(misc))
			m_maxBandwithPerPerson = atoi(misc);
		if(the_Archive.ReadString(misc))
			m_maxPacketSizeNonAdmin = atoi(misc);
		if(the_Archive.ReadString(misc))
			m_houseSightRange = (float)atoi(misc);

		the_Archive.Close();
		fl.Close();
	}

	m_pausePrimarySpinEvent = NULL;//CreateEvent(	NULL,				//no security attributes
									//		TRUE,				//manual-reset event
									//		TRUE,				//initial state is signaled
									//		"ServerProcess");	//object name

	//Misc plugin systems

	InitializeCriticalSection(&m_bandwidthPacketTracker);
	InitializeCriticalSection(&m_primaryServerThreadControllerSw);
	InitializeCriticalSection(&m_statusLock);
	InitializeCriticalSection(&m_varLock);
	InitializeCriticalSection(&m_connectionErrorSect);
	InitializeCriticalSection(&m_packetPerPassCountLock);
    InitializeCriticalSection(&m_bandwidthIncommingLock);
	InitializeCriticalSection(&m_triggerPrimarySect);	
}

void CMultiplayerObj::SetEnabledClient_safe(BOOL enabled)
{
    EnterCriticalSection(&m_varLock);
    m_enabledClient = enabled;
	LeaveCriticalSection(&m_varLock);
}

BOOL CMultiplayerObj::GetEnabledClient_safe()
{
    EnterCriticalSection(&m_varLock);
	BOOL ret = m_enabledClient;
	LeaveCriticalSection(&m_varLock);
	return ret;
}

unsigned long CMultiplayerObj::GetTotalBandwidthIncomming_safe()
{
    EnterCriticalSection(&m_bandwidthIncommingLock);
	unsigned long ret = m_totalBandwidthIncoming;
	LeaveCriticalSection(&m_bandwidthIncommingLock);
	return ret;
}

int CMultiplayerObj::RotateTrigger_safe()
{
    EnterCriticalSection(&m_triggerPrimarySect);
	int ret = m_triggerPrimaryCurrent++;
	if(m_triggerPrimaryCurrent >= m_primarySpinCount)
	   m_triggerPrimaryCurrent = 0;

	LeaveCriticalSection(&m_triggerPrimarySect);
	return ret;
}

void CMultiplayerObj::InitSpinLocks(int count)
{
     if(count < 1)
	    return;

     m_primarySpinCount = count;
	 m_pausePrimarySpinEvent = new HANDLE[count];
     for(int loop=0;loop<m_primarySpinCount;loop++)
			m_pausePrimarySpinEvent[loop] = CreateEvent(	NULL,				//no security attributes
															TRUE,				//manual-reset event
															TRUE,				//initial state is signaled
															"ServerProcess_"+m_cabLib.ConvToString(loop));	//object name

}

void CMultiplayerObj::AddToTotalBandwidthIncomming_safe(unsigned long add)
{
    EnterCriticalSection(&m_bandwidthIncommingLock);
	m_totalBandwidthIncoming += add;
	LeaveCriticalSection(&m_bandwidthIncommingLock);
}

void CMultiplayerObj::ZeroTotalBandwidthIncomming_safe()
{
    EnterCriticalSection(&m_bandwidthIncommingLock);
	m_totalBandwidthIncoming = 0;
	LeaveCriticalSection(&m_bandwidthIncommingLock);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMultiplayerObj
// Function:	IsQuickbarFull()
// Description:	Checks to see if the Quickbar is full.
///////////////////////////////////////////////////////////////////////////////

bool 
CMultiplayerObj::IsQuickbarFull()
{
	for (int i=0; i<NUM_QUICKBARS; i++)
	{
		for (int j=0; j<NUM_QUICKBAR_SLOTS; j++)
		{
			if (!this->m_Quickbars[i][j])
				return false;
		}
	}
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMultiplayerObj
// Function:	IsInQuickbars(CMenuInventoryItem*)
// Description:	Checks to see if the passed in item is in the Quickbars.
///////////////////////////////////////////////////////////////////////////////

bool	
CMultiplayerObj::IsInQuickbars(CMenuInventoryItem *pItem)
{
	for (int i=0; i<NUM_QUICKBARS; i++)
	{
		for (int j=0; j<NUM_QUICKBAR_SLOTS; j++)
		{
			if ((this->m_Quickbars[i][j])
			&& (pItem->m_data.m_GLID == (this->m_Quickbars[i][j])->m_data.m_GLID))
				return true;
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMultiplayerObj
// Function:	GetSelectedQuickbarItem()
// Description:	Bounds-checked access the the selected Quickbar item/index.
///////////////////////////////////////////////////////////////////////////////

CMenuInventoryItem**
CMultiplayerObj::GetSelectedQuickbarItem()
{
	// ERROR checking: Bounds-checking for index params
	if ((this->m_iQuickbarSelectedIndex >= NUM_QUICKBAR_SLOTS)
	|| (this->m_iQuickbarSelectedIndex < 0)) 
		return NULL;
	return &this->m_Quickbars[0][this->m_iQuickbarSelectedIndex];
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMultiplayerObj
// Function:	Quickbar(int, int, CMenuInventoryItem*)
// Description:	Bounds-checked accessor to the Quickbar collection.
///////////////////////////////////////////////////////////////////////////////

CMenuInventoryItem**
CMultiplayerObj::Quickbar(int iBar, int iSlot, CMenuInventoryItem* pItem)
{
	// ERROR checking: Bounds-checking for index params
	if ((iBar >= NUM_QUICKBARS)
	|| (iBar < 0)
	|| (iSlot >= NUM_QUICKBAR_SLOTS)
	|| (iSlot < 0)) 
		return NULL;

	// READ Access
	if(!pItem) 
	{
		return &this->m_Quickbars[iBar][iSlot];
	}
	// WRITE Access
	else
	{
		this->m_Quickbars[iBar][iSlot] = pItem;
		return &this->m_Quickbars[iBar][iSlot];			
	}
}

//-------------END FUNCTION--------------------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
BOOL CMultiplayerObj::CRCInValid(float & CRCONE,float & CRCTWO)
{
     if((CRCONE - CRCTWO) <= 1.0f)
         return FALSE;

     if((CRCTWO - CRCONE) <= 1.0f)
         return FALSE;

   return TRUE;
}
//-------------END FUNCTION--------------------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
void CMultiplayerObj::SetMultithreadSafeFlags()
{
	if(m_packetQue)
       m_packetQue->EnableCriticalSectionUse();
	if(m_clientCfgRequestOutbox)
       m_clientCfgRequestOutbox->EnableCriticalSectionUse();
	if(m_attributeOutBox)
       m_attributeOutBox->EnableCriticalSectionUse();
	if(m_equipOutBox)
       m_equipOutBox->EnableCriticalSectionUse();
	if(m_txtOutBox)
       m_txtOutBox->EnableCriticalSectionUse();
	if(m_dmgOutBox)
       m_dmgOutBox->EnableCriticalSectionUse();
	if(m_txtgroupOutBox)
       m_txtgroupOutBox->EnableCriticalSectionUse();
	if(m_fireOutBox)
       m_fireOutBox->EnableCriticalSectionUse();
	if(m_miscOutBox)
       m_miscOutBox->EnableCriticalSectionUse();
}
//-------------END FUNCTION--------------------------------------//
//---------------------------------------------------------------//
//------------Init Sever data------------------------------------//
BOOL CMultiplayerObj::EnableServer()
{
  //return TRUE;//rem
  //m_currentVersion = GetVersionFromFile();
  //check server cfg file
  

  m_sqlServerTimeStamp                = GetTickCount();
  m_sqlServerUpdateEnabled            = TRUE;
 
	
  //end check server cofg file
  HRESULT hr;
  PDIRECTPLAY8ADDRESS pDP8AddrLocal = NULL;
  
  if(!m_spawnPointCfgs)
     m_spawnPointCfgs			= new CSpawnCfgObjList();

  if(!m_currentWorldsUsed)
     m_currentWorldsUsed		= new CWorldAvailableObjList();

  if(!m_houseingDB)
	  m_houseingDB				= new CHousingZoneDB();

  if(!m_bountySystem)
      m_bountySystem			= new CBountyObjList();

  if(!m_runtimePlayersOnServer)
  {
      m_runtimePlayersOnServer		= new CPlayerObjectList();
	  m_runtimePlayersOnServer->WriteStatus("init.txt");
	  m_eventLog->AddEvent("CPlayerObjectList Indexing Enabled");
  }
  
  if(!m_runtimeItems)
  {
      m_runtimeItems				= new CServerItemList();
  }
  if(!m_netMonSys)
      m_netMonSys					= new CNetMonSystem();
  
  if(!m_runtimeAccountDB)
  {
      m_runtimeAccountDB			= new CAccountObjectList();
  }

  
  
  if(m_packetQue){
       delete m_packetQue;
	   m_packetQue = 0;
  }

  m_packetQue = new CPacketObjList();
  if(!m_eventLog)
      m_eventLog = new CServerEventList();
  
  //zero leak above
  
  if(!m_pendingLogons)
      m_pendingLogons				= new CPendingLogonsList(m_eventLog);
  

  if(!m_pendingLogoffs)
     m_pendingLogoffs              = new CPendingLogonsList(m_eventLog);
  
  if(!m_characterPendingSaveSystem)
      m_characterPendingSaveSystem  = new CCharacterPendingSave();



  //====== modern ENet transport (replaces DirectPlay8 server hosting) ======
  {
     unsigned short enetPort = m_port ? (unsigned short)m_port : 25857; // original default
     if(!m_netHost)
        m_netHost = new AuraNetHost();
     if(!m_netHost->StartServer(enetPort, (int)m_maxPlayers))
     {
        m_eventLog->AddEvent("ENet server failed to start on port",(DWORD)enetPort);
        return FALSE;
     }
     m_serverStarted = TRUE;
     m_hostIPString  = "0.0.0.0"; // ENet binds all interfaces; external IP resolved elsewhere
     m_eventLog->AddEvent("UDP Server Started (ENet)");
     m_eventLog->AddEvent("Client Limit set at",(DWORD)m_maxPlayers);
  }
  //====== end ENet transport ======
#if 0  // ---- legacy DirectPlay8 server hosting (preserved; removed in step 3f) ----
        if( FAILED( hr = CoCreateInstance( CLSID_DirectPlay8Server, 
			                               NULL, 
                                           CLSCTX_INPROC_SERVER,
                                           IID_IDirectPlay8Server, 
                                           (LPVOID*) &m_serverInterface ) ) ){
            return FALSE;
		}

		if( FAILED( hr = m_serverInterface->Initialize( this, DirectPlayMessageHandler, 0 ) ) )
            return FALSE;

		if( FAILED( hr = CoCreateInstance( CLSID_DirectPlay8Address, NULL, 
                                           CLSCTX_INPROC_SERVER, IID_IDirectPlay8Address, 
                                           (LPVOID*) &pDP8AddrLocal ) ) )
            return FALSE;
        
		if( FAILED( hr = pDP8AddrLocal->SetSP( &CLSID_DP8SP_TCPIP ) ) )//set tcpip for service provider
            return FALSE;

		if( m_port != 0 )
		{
            if( FAILED( hr = pDP8AddrLocal->AddComponent( DPNA_KEY_PORT, 
                                                          &m_port, sizeof(m_port),
                                                           DPNA_DATATYPE_DWORD ) ) )
            return FALSE;
		}

		

		LPSTR sessionCsName = "Avarian Session";
        WCHAR       sessionName[MAX_PATH];
        MultiByteToWideChar(CP_ACP, 0, sessionCsName, -1, sessionName, sizeof(sessionName)/sizeof(sessionName[0]));

		DPN_APPLICATION_DESC dpnAppDesc;
        ZeroMemory( &dpnAppDesc, sizeof(DPN_APPLICATION_DESC) );
        dpnAppDesc.dwSize           = sizeof( DPN_APPLICATION_DESC );
		  if(!m_passwordRequired)
              dpnAppDesc.dwFlags          = DPNSESSION_CLIENT_SERVER | DPNSESSION_NODPNSVR;//REV
          else
		  {//set password details
		      dpnAppDesc.dwFlags          = DPNSESSION_CLIENT_SERVER | DPNSESSION_REQUIREPASSWORD | DPNSESSION_NODPNSVR;
		      WCHAR  wCharPassCnv[MAX_PATH];
              MultiByteToWideChar(CP_ACP, 0, m_serverKey, -1, wCharPassCnv, sizeof(wCharPassCnv)/sizeof(wCharPassCnv[0]));
		      dpnAppDesc.pwszPassword = wCharPassCnv;
		  }//end set password details
        dpnAppDesc.guidApplication  = g_guidApp;
        dpnAppDesc.pwszSessionName  = sessionName;
		dpnAppDesc.dwMaxPlayers     = m_maxPlayers;
  
        if( FAILED( hr = m_serverInterface->Host( &dpnAppDesc, &pDP8AddrLocal, 1, NULL, NULL, NULL, 0  ) ) )
        return FALSE;//error here


		pDP8AddrLocal->Release();
		pDP8AddrLocal=0;


        m_serverStarted = TRUE;

		DWORD buffer = 0;
		IDirectPlay8Address * hostIpAdress = NULL;
        m_serverInterface->GetLocalHostAddresses(NULL,&buffer,0);
		m_serverInterface->GetLocalHostAddresses(&hostIpAdress,&buffer,0);


        if(hostIpAdress)
		{
		   m_hostIPString = GetIPAddress(hostIpAdress);//function causes crash if inputs are invalid (PROTECT ME)
		   m_eventLog->AddEvent("UDP Server Started",sessionCsName,m_hostIPString);
		   hostIpAdress->Release();
		   hostIpAdress=0;
		}

		m_eventLog->AddEvent("UDP Server Started");
		
		m_eventLog->AddEvent("Client Limit set at",dpnAppDesc.dwMaxPlayers);

        //set service provider caps
        DPN_SP_CAPS spCaps;
		ZeroMemory(&spCaps,sizeof(DPN_SP_CAPS));
		spCaps.dwSize = sizeof(DPN_SP_CAPS);
		m_serverInterface->GetSPCaps(&CLSID_DP8SP_TCPIP,
			                         &spCaps,
                                     0);

		spCaps.dwBuffersPerThread = spCaps.dwBuffersPerThread + 2;

		spCaps.dwSystemBufferSize = spCaps.dwSystemBufferSize * 2;

		spCaps.dwNumThreads = spCaps.dwNumThreads * 5;

		m_eventLog->AddEvent("Buffers per thread:",spCaps.dwBuffersPerThread);
		m_eventLog->AddEvent("System Buffer Size:",spCaps.dwSystemBufferSize);
		m_eventLog->AddEvent("Thread Count:",spCaps.dwNumThreads);

		HRESULT startupResult = m_serverInterface->SetSPCaps(&CLSID_DP8SP_TCPIP,
			                         &spCaps,
									 0);

		if(startupResult == S_OK)
		   m_eventLog->AddEvent("Server caps set successfully");
		else
           m_eventLog->AddEvent("Server caps set Failed");
		//end set service provider caps
#endif // ---- end legacy DirectPlay8 server hosting ----

		m_serverStartTime = GetTickCount();

		SetMultithreadSafeFlags();
        //start up threads
		m_logonProcThreadCount = 4;
		DWORD threadID;
        //init
        m_logonProcThreadHanldes = new HANDLE[m_logonProcThreadCount];
		for(int lpLoop = 0;lpLoop<m_logonProcThreadCount;lpLoop++)
		{
		    m_logonProcThreadHanldes[lpLoop] = CreateThread(NULL,0,LogOnProcThread,this,0,&threadID);
		}

		// start the ENet receive pump (replaces DirectPlay's own callback threads)
		m_netPumpRun = true;
		m_netPumpThreadHandle = CreateThread(NULL,0,NetPumpThread,this,0,&threadID);

		return TRUE;
}
//-------------END FUNCTION--------------------------------------//
//---------------------------------------------------------------//
//------------enum hosts-----------------------------------------//
BOOL CMultiplayerObj::StopEnumerating()
{
 if(!m_clientInterface)
	 return FALSE;

 if(!m_dpAsncEnumHostHandle )
     return FALSE;

     m_clientInterface->CancelAsyncOperation( m_dpAsncEnumHostHandle, 0 );

	 m_dpAsncEnumHostHandle = 0;
     return TRUE;
}
//-------------END FUNCTION--------------------------------------//
//---------------------------------------------------------------//
//------------enum hosts-----------------------------------------//
BOOL CMultiplayerObj::EnumHosts(CString TCPIP)
{
    if(!m_clientInterface)
		return FALSE;

    StopEnumerating();//cancel last enumeration

	HRESULT hr;
	DPN_APPLICATION_DESC   dpnAppDesc;
    //IDirectPlay8Address*   pDP8AddressHost  = NULL;
    //IDirectPlay8Address*   pDP8AddressLocal = NULL;

    //Create the local device address object
    if( FAILED( hr = CoCreateInstance( CLSID_DirectPlay8Address, NULL, 
                                       CLSCTX_INPROC_SERVER, IID_IDirectPlay8Address,
                                       (LPVOID*) &m_pDP8AddressLocal ) ) )
    return FALSE;

    //Set IP service provider
    if( FAILED( hr = m_pDP8AddressLocal->SetSP( &CLSID_DP8SP_TCPIP ) ) )
    return FALSE;


    //Create the remote host address object
    if( FAILED( hr = CoCreateInstance( CLSID_DirectPlay8Address, NULL, 
                                       CLSCTX_INPROC_SERVER, IID_IDirectPlay8Address,
                                       (LPVOID*) &m_pDP8AddressHost ) ) )
    return FALSE;
	//Set IP service provider
    if( FAILED( hr = m_pDP8AddressHost->SetSP( &CLSID_DP8SP_TCPIP ) ) )
    return FALSE;

	TCHAR strIPAddress[MAX_PATH];
	lstrcpy(strIPAddress,TCPIP);


    ZeroMemory( &dpnAppDesc, sizeof( DPN_APPLICATION_DESC ) );
    dpnAppDesc.dwSize = sizeof( DPN_APPLICATION_DESC );
    dpnAppDesc.guidApplication = g_guidApp;


	if(TCPIP.GetLength() != 0){//specific IP
      WCHAR       wszHostName[MAX_PATH];
      MultiByteToWideChar(CP_ACP, 0,TCPIP, -1, wszHostName, sizeof(wszHostName)/sizeof(wszHostName[0]));

      hr = m_pDP8AddressHost->AddComponent( DPNA_KEY_HOSTNAME, wszHostName, 
                                         (DWORD)(wcslen(wszHostName)+1)*sizeof(WCHAR), 
                                          DPNA_DATATYPE_STRING );
	  if( FAILED(hr))
		  return FALSE;
	}//end specific IP
    if(m_port != 0){//specific port
	     hr = m_pDP8AddressHost->AddComponent( DPNA_KEY_PORT, 
                                             &m_port, sizeof(m_port),
                                             DPNA_DATATYPE_DWORD );
         if( FAILED(hr))
		     return FALSE;
	}//end specific port

	//get local device

	hr = m_clientInterface->EnumHosts( &dpnAppDesc, m_pDP8AddressHost, 
                                        m_pDP8AddressLocal, NULL, 
                                        0, INFINITE, 0, INFINITE, NULL, 
                                        &m_dpAsncEnumHostHandle,0);

	if( FAILED(hr))
	    return FALSE;

	return TRUE;
}
//---------------------------------------------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
void CMultiplayerObj::Disconnect()
{ 
	SetEnabledClient_safe(FALSE);
    if(m_clientInterface)
	{//release client interface
       m_clientInterface->Close(0);
	   m_clientInterface->Release();
	   m_clientInterface = 0;
	}
}
//-------------END FUNCTION--------------------------------------//
//---------------------------------------------------------------//
//------------Init Client data-----------------------------------//
BOOL CMultiplayerObj::EnableClient(CString IPaddress,int version)
{
    SetEnabledClient_safe(FALSE);
    m_connectionError = 0;
	m_connectionErrorCode = "";

    if(m_clientInterface)
	{//release client interface
       m_clientInterface->Close(0);
	   m_clientInterface->Release();
	   m_clientInterface = 0;
	}

	m_attempConnectionEnabled			= TRUE;
	m_attemptConnectionStamp            = GetTickCount();
	//retrieve version from file
	if(version == 0)
       m_currentVersion = GetVersionFromFile();
	else
	   m_currentVersion = version;

	if(m_IPaddress == "")
	   m_IPaddress      = GetIPDataFromDisk();
    
	if(!m_eventLog)
       m_eventLog = new CServerEventList();


	if(m_packetQue){
       delete m_packetQue;
	   m_packetQue = 0;
	}
	m_packetQue              = new CPacketObjList();

	if(!m_clientCfgRequestOutbox)m_clientCfgRequestOutbox = new CPacketObjList();
	else m_clientCfgRequestOutbox->Flush();
	
	if(!m_attributeOutBox) m_attributeOutBox = new CPacketObjList();
	else m_attributeOutBox->Flush();

	if(!m_equipOutBox) m_equipOutBox = new CPacketObjList();
	else m_equipOutBox->Flush();

	if(!m_txtOutBox) m_txtOutBox = new CPacketObjList();
	else m_txtOutBox->Flush();

	if(!m_dmgOutBox) m_dmgOutBox = new CPacketObjList();
	else m_dmgOutBox->Flush();
  
	if(!m_txtgroupOutBox) m_txtgroupOutBox = new CPacketObjList();
	else m_txtgroupOutBox->Flush();

	if(!m_fireOutBox) m_fireOutBox = new CPacketObjList();
	else m_fireOutBox->Flush();
	
	if(!m_miscOutBox) m_miscOutBox = new CPacketObjList();
	else m_miscOutBox->Flush();

	//if(!m_recentQuestPtr) 
		//m_recentQuestPtr = new CQuestJournalObj();
	this->m_bRecentJournalUpdate = false;

	if (this->m_journalEntries)
	{
		this->m_journalEntries->SafeDelete();
		::SafeDelete(this->m_journalEntries);
	}
	this->m_journalEntries = new CQuestJournalList();

	SetMultithreadSafeFlags();

#if 0  // ---- legacy DirectPlay8 client connect+enum (preserved; removed in 3f) ----
	HRESULT hr;
    if( FAILED( hr = CoCreateInstance( CLSID_DirectPlay8Client, NULL, 
                                       CLSCTX_INPROC_SERVER,
                                       IID_IDirectPlay8Client, 
                                       (LPVOID*) &m_clientInterface ) ) )
        return FALSE;
	//Init IDirectPlay8Client
    if( FAILED( hr = m_clientInterface->Initialize( this, DirectPlayMessageHandler, 0 ) ) )
        return FALSE;

	//enumerate hosts
	IDirectPlay8Address*   pDP8AddressHost  = NULL;
    IDirectPlay8Address*   pDP8AddressLocal = NULL;

    //Create the local device address object
    if( FAILED( hr = CoCreateInstance( CLSID_DirectPlay8Address, NULL, 
                                       CLSCTX_INPROC_SERVER, IID_IDirectPlay8Address,
                                       (LPVOID*) &pDP8AddressLocal ) ) )
        return FALSE;

    //Set IP service provider
    if( FAILED( hr = pDP8AddressLocal->SetSP( &CLSID_DP8SP_TCPIP ) ) )
        return FALSE;

	//Create the remote host address object
    if( FAILED( hr = CoCreateInstance( CLSID_DirectPlay8Address, NULL, 
                                       CLSCTX_INPROC_SERVER, IID_IDirectPlay8Address,
                                       (LPVOID*) &pDP8AddressHost ) ) )
        return FALSE;
	//Set IP service provider
    if( FAILED( hr = pDP8AddressHost->SetSP( &CLSID_DP8SP_TCPIP ) ) )
        return FALSE;

	CString TCPIP = IPaddress;
	TCHAR strIPAddress[MAX_PATH];
	lstrcpy(strIPAddress,TCPIP);


	if(TCPIP.GetLength() != 0){//specific IP
       WCHAR       wszHostName[MAX_PATH];
       MultiByteToWideChar(CP_ACP, 0,TCPIP, -1, wszHostName, sizeof(wszHostName)/sizeof(wszHostName[0]));

       hr = pDP8AddressHost->AddComponent( DPNA_KEY_HOSTNAME, wszHostName, 
                                          2*((UINT)wcslen(wszHostName) + 1), 
                                          DPNA_DATATYPE_STRING );
	   if( FAILED(hr))
		   return FALSE;
	}//end specific IP
    if(m_port != 0){//specific port
	     hr = pDP8AddressHost->AddComponent( DPNA_KEY_PORT, 
                                             &m_port, sizeof(DWORD),
                                             DPNA_DATATYPE_DWORD );
         if( FAILED(hr))
		     return FALSE;
	}//end specific port

	if(!EnumHosts(IPaddress))
	    m_eventLog->AddEvent("Enum Failed");
	else
	    m_eventLog->AddEvent("Enum Started");
#endif // ---- end legacy DirectPlay8 client connect+enum ----
  //====== modern ENet transport (replaces DirectPlay8 client connect + enum) ======
  {
     unsigned short enetPort = m_port ? (unsigned short)m_port : 25857;
     // "host:port" (e.g. a tunnel address like name.ply.gg:31214) overrides the port.
     // Only one ':' allowed, so a bare IPv6 address is never mangled.
     {
        int colon = IPaddress.Find(':');
        if(colon > 0 && IPaddress.Find(':', colon + 1) < 0)
        {
           int p = atoi(IPaddress.Mid(colon + 1));
           if(p > 0 && p < 65536)
              enetPort = (unsigned short)p;
           IPaddress = IPaddress.Left(colon);
        }
     }
     if(!m_netHost)
        m_netHost = new AuraNetHost();
     if(!m_netHost->StartClient())
     {
        m_eventLog->AddEvent("ENet client failed to start");
        return FALSE;
     }
     AuraNetID sid = m_netHost->Connect((LPCSTR)IPaddress, enetPort, 5000);
     if(sid == 0)
     {
        m_eventLog->AddEvent("ENet connect failed to",IPaddress);
        return FALSE;
     }
     // send the login handshake - same CONTEXT_VALIDATION DirectPlay sent as connect data
     CONTEXT_VALIDATION login;
     ZeroMemory(&login,sizeof(login));
     lstrcpyn(login.username,m_userName,25);
     lstrcpyn(login.password,m_passWord,25);
     login.versionInfo = (short)m_currentVersion;
     m_netHost->Send(sid,(const unsigned char*)&login,sizeof(CONTEXT_VALIDATION),true);

     // drive the receive pump on the client too
     m_netPumpRun = true;
     DWORD netThreadID;
     m_netPumpThreadHandle = CreateThread(NULL,0,NetPumpThread,this,0,&netThreadID);

     SetEnabledClient_safe(TRUE);
     m_eventLog->AddEvent("ENet client connected to",IPaddress);
  }
  //====== end ENet transport ======

	

return TRUE;
}
//-------------END FUNCTION--------------------------------------//
//---------------------------------------------------------------//
//------------Safly delete all-----------------------------------//
void CMultiplayerObj::SafeDelete()
{
    //shutDownThreads If Nessessary
    if(m_logonProcThreadCount > 0)
	{//wait for threads to shut down
		int startThreadCount = m_logonProcThreadCount;
		EnterCriticalSection(&m_logonSection);
		m_shutDownLogonThreads = TRUE;
		while(m_logonProcThreadCount > 0)
		{
			LeaveCriticalSection(&m_logonSection);
			Sleep(200);
			EnterCriticalSection(&m_logonSection);
		}
		LeaveCriticalSection(&m_logonSection);
		//clean hanldes

		for(int lpLoop = 0;lpLoop<startThreadCount;lpLoop++)
		{
			if(m_logonProcThreadHanldes[lpLoop])
				CloseHandle(m_logonProcThreadHanldes[lpLoop]);
		}
		delete [] m_logonProcThreadHanldes;
	}//end wait for threads to shut down


    if(m_pDP8AddressHost)
	{
       m_pDP8AddressHost->Release();
	   m_pDP8AddressHost = 0;
	}

	if(m_pDP8AddressLocal)
	{ 
       m_pDP8AddressLocal->Release();
	   m_pDP8AddressLocal = 0;
	}

    if(m_serverInterface)
	{
       m_serverInterface->Close(0);
       m_serverInterface->Release();
       m_serverInterface = 0;
	}
    if(m_clientInterface){
       m_clientInterface->Close(0);
       m_clientInterface->Release();
       m_clientInterface = 0;
	}

    if(m_characterPendingSaveSystem)
	{ 
       delete m_characterPendingSaveSystem;
	   m_characterPendingSaveSystem = 0;
	}

    if(m_pendingLogons)
    {
       delete m_pendingLogons;
	   m_pendingLogons = 0;
    }

	if(m_pendingLogoffs)
	{
       delete m_pendingLogoffs;
	   m_pendingLogoffs = 0;
	}

	if(m_bountySystem)
	{
	   m_bountySystem->SafeDelete();
	   delete m_bountySystem;
	   m_bountySystem = 0;
	}


    if(m_elementStatus)
	{
       m_elementStatus->SafeDelete();
	   delete m_elementStatus;
	   m_elementStatus = 0;
	}


	if(m_runtimeAccountDB)
	{  
	   delete m_runtimeAccountDB;
	   m_runtimeAccountDB = 0;
	}
	
	if(m_houseingDB)
	{
	   m_houseingDB->SafeDelete();
	   delete m_houseingDB;
	   m_houseingDB = 0;
	}
	
	if(m_netMonSys)
	{
       m_netMonSys->SafeDelete();
	   delete m_netMonSys;
	   m_netMonSys = 0;

	}

	if(m_runtimeItems)
	{
	   delete m_runtimeItems;
	   m_runtimeItems = 0;
	}
	
	if(m_packetQue){
	   delete m_packetQue;
	   m_packetQue=0;
	}

	if(!m_serverStarted)
    if(m_eventLog)
	{
	   m_eventLog->AddEvent("Packets Dropped during session",":",m_packetsDropped);
       m_eventLog->OutPutTextFile("last.log");
	   m_eventLog->SafeDelete();
	   delete m_eventLog;
	   m_eventLog=0;
	}
	
   m_messagingEnabledDplay = FALSE;
   
   if(m_runtimePlayersOnServer){
	  delete m_runtimePlayersOnServer;
	  m_runtimePlayersOnServer=0;
   }
   if(m_currentWorldsUsed){
      delete m_currentWorldsUsed;
	  m_currentWorldsUsed=0;
   }
   if(m_runtimeItemsOnServer){
	  delete m_runtimeItemsOnServer;
	  m_runtimeItemsOnServer=0;
   }
   
   if(m_spawnPointCfgs)
   {
      m_spawnPointCfgs->SafeDelete();
	  delete m_spawnPointCfgs;
	  m_spawnPointCfgs = 0;
   }

   if(m_ipBlockList)
   {
      m_ipBlockList->SafeDelete();
	  delete m_ipBlockList;
	  m_ipBlockList = 0;
   }

   if(m_clientCfgRequestOutbox)
   {
	  delete m_clientCfgRequestOutbox;
	  m_clientCfgRequestOutbox = 0;
   }

   if(m_attributeOutBox)
   {
	  delete m_attributeOutBox;
	  m_attributeOutBox = 0;
   }

   if(m_equipOutBox)
   {
	  delete m_equipOutBox;
	  m_equipOutBox = 0;
   }

   if(m_txtOutBox)
   {
	  delete m_txtOutBox;
	  m_txtOutBox = 0;
   }

   if(m_dmgOutBox)
   {
	  delete m_dmgOutBox;
	  m_dmgOutBox = 0;
   }

   if(m_txtgroupOutBox)
   {
	  delete m_txtgroupOutBox;
	  m_txtgroupOutBox = 0;
   }

   if(m_fireOutBox)
   {
	  delete m_fireOutBox;
	  m_fireOutBox = 0;
   }

   if(m_miscOutBox)
   {
	  delete m_miscOutBox;
      m_miscOutBox = 0;
   }

	if (this->m_journalEntries)
	{
		this->m_journalEntries->SafeDelete();
		::SafeDelete(this->m_journalEntries);
	}

   
   DeleteCriticalSection(  &m_logonSection);
   DeleteCriticalSection(  &m_bandwidthPacketTracker );
   DeleteCriticalSection(  &m_primaryServerThreadControllerSw);
   DeleteCriticalSection(  &m_statusLock);
   DeleteCriticalSection(  &m_packetPerPassCountLock);
   DeleteCriticalSection(  &m_varLock);
   DeleteCriticalSection(  &m_connectionErrorSect);
   DeleteCriticalSection(  &m_bandwidthIncommingLock);
   DeleteCriticalSection(  &m_triggerPrimarySect);
   
   

   if(m_pausePrimarySpinEvent)
   {
      CloseHandle(m_pausePrimarySpinEvent);
      m_pausePrimarySpinEvent = 0;
   }

   
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Logs player out-------------------------------------------------------//
BOOL CMultiplayerObj::LogsPlayerOut_requiredAccountLock(CPlayerObject * runtimePtr)
{
	 DPNID storedDPNID = runtimePtr->m_currentID;
     CAccountObject * accountPtr = m_runtimeAccountDB->GetAccountByID(storedDPNID);
     if(!accountPtr)
	 {//error locating account
         m_eventLog->AddEvent("Error locating account(768)","DPNID>",storedDPNID);
		 return FALSE;
	 }//end error locating account
     
	 long logDuration = GetTickCount() - accountPtr->m_logonTimeStamp;
     long seconds = logDuration / 1000;
     long minutes = seconds/60;
	 if(!accountPtr->m_aiServer)
	     m_eventLog->LogByPriority(accountPtr->m_accountName+"left game: current time:",(DWORD)minutes,2);
	 else
	 {
		 m_eventLog->LogByPriority(accountPtr->m_accountName+" TIME OUT: netID:",(DWORD)runtimePtr->m_networkAssignedID,3);
	 }
     
     if(accountPtr->m_currentcharInUse != -1)
	 {//character is in use
        if(!accountPtr->m_characterOnAccount)
		{//no characters created on account
           m_eventLog->AddEvent("Account logging out has no characters","->",accountPtr->m_accountName);
		   accountPtr->m_currentcharInUse  = -1;
		   return FALSE;
		}//end no characters created on account
        if(!UpdateAccount(runtimePtr,accountPtr)){
			return FALSE;
		}
		
        Disconnect(accountPtr->m_currentID);//timed out
		//m_eventLog->AddEvent("Account Timed Out",".ID>",accountPtr->m_accountName);
		
		if(m_accountDBMode == 1)
		{//put into save que and remove account object
		      m_characterPendingSaveSystem->AddCharacterToPendingList(accountPtr->m_characterOnAccount,1,accountPtr->m_accountName,-1);
		}//end put into save que and remove account object
		
		accountPtr->m_currentcharInUse  = -1;	
	 }//end character is in use
	 return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CMultiplayerObj::UpdateAccount(CPlayerObject  * runtimePlayerObj,
									CAccountObject * accountPtr)
{
	if(!accountPtr)
		return FALSE;

	if(!runtimePlayerObj)
		return FALSE;
	
	if(accountPtr->m_currentcharInUse < 0)
	   return FALSE;

	if(!accountPtr->m_characterOnAccount)
	{
       m_eventLog->AddEvent("Invalid character in use","->",accountPtr->m_accountName);
	   accountPtr->m_currentcharInUse  = -1;
       return FALSE;
	}

		CPlayerObject * playerObject    = accountPtr->m_characterOnAccount;

		//Check for Murder Count Decreasing
		if(runtimePlayerObj->m_noteriety)
		{ //End Player Has Noteriety
			if(runtimePlayerObj->m_noteriety->m_currentMurders > 0)
			{ //Player has a murder count
				runtimePlayerObj->m_noteriety->m_currentMinuteCountSinceLastMurder += (GetTickCount()-runtimePlayerObj->m_noteriety->m_murderUpdateTick);
				
				if(runtimePlayerObj->m_noteriety->m_currentMinuteCountSinceLastMurder > m_goodBehaviorDuration)
				{//see if we have timed out a murder
					if(runtimePlayerObj->m_noteriety->m_currentMurders > 0)
					{ //Player has murders
						runtimePlayerObj->m_noteriety->m_currentMinuteCountSinceLastMurder = 0;
						runtimePlayerObj->m_noteriety->m_currentMurders--;
					} //End Player has murders
				}//end see if we have timed out a murder
				
				playerObject->m_countDownToMurderPardon = runtimePlayerObj->m_countDownToMurderPardon;
				if(playerObject->m_noteriety)
				{
				   playerObject->m_noteriety->m_currentMurders = runtimePlayerObj->m_noteriety->m_currentMurders;
				   playerObject->m_noteriety->m_currentMinuteCountSinceLastMurder = runtimePlayerObj->m_noteriety->m_currentMinuteCountSinceLastMurder;
				   playerObject->m_noteriety->m_murderUpdateTick = runtimePlayerObj->m_noteriety->m_murderUpdateTick;
				   
				   m_currentMurderCount = runtimePlayerObj->m_noteriety->m_currentMurders; //Update the local count
				}

				runtimePlayerObj->m_noteriety->m_murderUpdateTick = GetTickCount(); //Note that player has been updated.
			} //End Player has a murder count
		}//End Player Has Noteriety
		

		playerObject->m_countDownToMurderPardon = runtimePlayerObj->m_countDownToMurderPardon;
		playerObject->m_3dPosition.x            = runtimePlayerObj->m_3dPosition.x;
		playerObject->m_3dPosition.y            = runtimePlayerObj->m_3dPosition.y;
		playerObject->m_3dPosition.z            = runtimePlayerObj->m_3dPosition.z;
		playerObject->m_energy                  = runtimePlayerObj->m_energy;
		playerObject->m_maxEnergy               = runtimePlayerObj->m_maxEnergy;
		playerObject->m_aiCfgOfMounted          = runtimePlayerObj->m_aiCfgOfMounted;
		playerObject->m_clanHandle              = runtimePlayerObj->m_clanHandle;
		playerObject->m_arenaBasePoints         = runtimePlayerObj->m_arenaBasePoints;
		playerObject->m_pvpKills				= runtimePlayerObj->m_pvpKills;
		playerObject->m_arenaKills				= runtimePlayerObj->m_arenaKills;
        playerObject->m_arenaTotalKills         = runtimePlayerObj->m_arenaTotalKills;
		playerObject->m_instanceType			= runtimePlayerObj->m_instanceType;
		playerObject->m_groupID					= runtimePlayerObj->m_groupID;
		playerObject->m_teamTemp				= runtimePlayerObj->m_teamTemp;
		playerObject->m_clanAbbrev				= runtimePlayerObj->m_clanAbbrev;
		playerObject->m_factionStanding			= runtimePlayerObj->m_factionStanding;
		playerObject->m_arenaItemsGiven			= runtimePlayerObj->m_arenaItemsGiven;
		playerObject->m_squadName				= runtimePlayerObj->m_squadName;
		playerObject->m_groupStruct				= runtimePlayerObj->m_groupStruct;

		//playerObject->m_groupStruct.m_groupLeadership		= runtimePlayerObj->m_groupStruct.m_groupLeadership;
		//playerObject->m_groupStruct.m_groupPlacementIndex	= runtimePlayerObj->m_groupStruct.m_groupPlacementIndex;

		//update powerup info
		for(int i = 0; i < MAX_BONUS; i++)
		{
			playerObject->m_bonus[i].m_bonusAmount		= runtimePlayerObj->m_bonus[i].m_bonusAmount;
			playerObject->m_bonus[i].m_bonusDuration	= runtimePlayerObj->m_bonus[i].m_bonusDuration;
			playerObject->m_bonus[i].m_bonusType		= runtimePlayerObj->m_bonus[i].m_bonusType;
			playerObject->m_bonus[i].m_damageMult		= runtimePlayerObj->m_bonus[i].m_damageMult;
			playerObject->m_bonus[i].m_maxDuration		= runtimePlayerObj->m_bonus[i].m_maxDuration;
		}
		
		//update inventory
		if(playerObject->m_currentInventory)
		{
		   playerObject->m_currentInventory->SafeDelete();
		   delete playerObject->m_currentInventory;
		   playerObject->m_currentInventory=0;
		}
		runtimePlayerObj->m_currentInventory->Clone(&playerObject->m_currentInventory);	

        if(playerObject->m_reconfigInven)
		{
		   playerObject->m_reconfigInven->SafeDelete();
		   delete playerObject->m_reconfigInven;
		   playerObject->m_reconfigInven=0;
		}
		if(runtimePlayerObj->m_reconfigInven)
		runtimePlayerObj->m_reconfigInven->Clone(&playerObject->m_reconfigInven);	

		if(playerObject->m_noteriety)
		{
		   delete playerObject->m_noteriety;
		   playerObject->m_noteriety=0;
		}
		runtimePlayerObj->m_noteriety->Clone(&playerObject->m_noteriety);	

		//update stats gained
        if(playerObject->m_stats != NULL && runtimePlayerObj->m_stats != NULL){//valid for stats
           playerObject->m_stats->SafeDelete();
	       delete playerObject->m_stats;
		   playerObject->m_stats=0;
	       runtimePlayerObj->m_stats->Clone(&playerObject->m_stats);
		}//end valid for stats
		
	return TRUE;
}
//-------------END FUNCTION--------------------------------------//
//---------------------------------------------------------------//
//---------------------------------------------------------------//
/*BOOL CMultiplayerObj::SetItemOnServer(CString worldName,
									  int dbIndex,
									  D3DVECTOR position,
									  int curAnim,
									  int curID)
{
     if(!m_runtimeItemsOnServer)
	 {
		 m_runtimeItemsOnServer = new CPlayerObjectList();
	 }
	 
	 CPlayerObject * obj      = new CPlayerObject();
	 obj->m_3dPosition.x      = position.x;
	 obj->m_3dPosition.y      = position.y;
	 obj->m_3dPosition.z      = position.z;
	 obj->m_dirVector.x       = 0.0f;
	 obj->m_dirVector.y       = 0.0f;
	 obj->m_dirVector.z       = 1.0f;
	 obj->m_upVector.x        = 0.0f;
	 obj->m_upVector.y        = 1.0f;
	 obj->m_upVector.z        = 0.0f;
	 obj->m_curAnim           = curAnim;
	 obj->m_networkAssignedID = curID;
	 obj->m_currentWorldID    = 0;
	 obj->m_currentWorldMap   = worldName;
	 obj->m_dbCfg             = dbIndex;

	 m_runtimeItemsOnServer->AddTail(obj);
	return TRUE;
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------disconnect player-----------------------------------------------------//
BOOL CMultiplayerObj::Disconnect(DPNID id)
{
     // ENet transport: m_serverInterface (DirectPlay8) is always NULL now
     if(m_netHost)
        m_netHost->DisconnectPlayer((AuraNetID)id);
     return TRUE;
}
//-----------------------------------------------------------//
void CMultiplayerObj::SnapShotCurrentPacketQue()
{ 
       m_packetQue->SendToTextFile();
}
//-----------------Build CRC----------------------------------------------------------//
short CMultiplayerObj::GetTxtMsgCrC(LPTXT_MSG packet)
{

return 1;
}
//------------------------------------------------------------------------------------//
//-----------------Build CRC----------------------------------------------------------//
short CMultiplayerObj::GetBuildMenuMsgCrC(LPMENUBUILD_STRUCT packet)
{
int factor=4;
for(int loop=0;loop<25;loop++){
	factor += (int)(packet->uniText[loop]);
}
return (short)((factor+packet->messageType+packet->miscint)/3);
}
//------------------------------------------------------------------------------------//
//-----------------Build CRC----------------------------------------------------------//
float CMultiplayerObj::GetMovMsgCrC(LPSHRUNK_TOSVRMOV_STRUCT packet)
{

float    genCRC = 1.134f;

genCRC =(packet->Xpos + 
         packet->Ypos + 
		 packet->Zpos +
		 ((float)packet->secondaryAnim) +
		 ((float)packet->curAnim));

         genCRC = genCRC * .11f;
     
return genCRC;
}
//------------------------------------------------------------------------------------//
//-----------------Build CRC----------------------------------------------------------//
float CMultiplayerObj::GetDamageMsgCrC(LPHITPLAYER_DATA packet)
{

float    genCRC = 0.0f;

genCRC =(((float)packet->m_attackerNetowkID) + 
         ((float)packet->m_playerThatGotHitID) + 
		 ((float)packet->m_miscValue) +
		 ((float)packet->m_type) +
		 ((float)packet->m_securityIndexer));


         genCRC = genCRC * .1f;
     
return genCRC;
}
//------------------------------------------------------------------------------------//
//-----------------Build CRC----------------------------------------------------------//
short CMultiplayerObj::GetItemLocationCRC(LPITEM_LOCATION iLocationStruct)
{

long genCRC = 0;

genCRC = (long)(iLocationStruct->Xpos + 
          iLocationStruct->Ypos + 
		  iLocationStruct->Zpos +
		  iLocationStruct->curAnim +
		  iLocationStruct->networkID +
		  iLocationStruct->dbIndex);

while(labs(genCRC) > 30000)
      genCRC = genCRC / 10;
     
short finalValue = (short) genCRC;

return finalValue;
}
//------------------------------------------------------------------------------------//
//-----------------Build CRC----------------------------------------------------------//
short CMultiplayerObj::GetEquipCRC(LPEQUIP_STRUCT equipStruct)
{
return((equipStruct->index)-210)/2;
}
//------------------------------------------------------------------------------------//
//-----------------Build CRC----------------------------------------------------------//
short CMultiplayerObj::GetAttribCRC(LPATTRIB_STRUCT attribStruct)
{
long genCRC = 0;

genCRC = attribStruct->attribute + 
         attribStruct->uniInt + 
		 attribStruct->uniInt2;

while(labs(genCRC) > 30000)
      genCRC = genCRC / 10;
     
short finalValue = (short) genCRC;

return finalValue;
}
//-----------------Build CRC----------------------------------------------------------//
short CMultiplayerObj::GetFireCRC(LPFIRE_STRUCT fireStruct)
{
return((fireStruct->lookatPosX +
		fireStruct->lookatPosY + 
		fireStruct->lookatPosZ +
		fireStruct->fireQueIndex)-212)/2;
}
//-----------------Build CRC----------------------------------------------------------//
short CMultiplayerObj::GetSpawnCRC(LPSPAWN_STRUCT spawnStruct)
{

long genCRC = 0;

for(int loop=0;loop<25;loop++)
	genCRC += (int)(spawnStruct->worldName[loop]);

    genCRC += (spawnStruct->dbIndex + spawnStruct->currentTrace)-1;
while(labs(genCRC) > 30000)
      genCRC = genCRC / 9;
     
short finalValue = (short) genCRC-4;

return finalValue;
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
BOOL CMultiplayerObj::SendByteBlockMsg(    DWORD flags,
							               DPNID sendTo,
								           BYTE * m_dataBlock,
										   int blockSize,
										   DWORD timeout
								           ) 
{
    if(blockSize == 0)
	   return FALSE;

	if(m_dataBlock == NULL)
	   return FALSE;//new
	
       SendData((BYTE *)m_dataBlock,
				        blockSize,
				        flags,
				        sendTo,
						timeout);

	if(m_dataBlock)
	   delete [] m_dataBlock;
	   m_dataBlock = 0;

	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Ping------------------------------------------------------------------//
SHRUNK_LPMOVE_STRUCT CMultiplayerObj::ShrinkData(LPMOVE_STRUCT OLD_DATA,SHRUNK_LPMOVE_STRUCT SHRUNK_DATA) 
{
	SHRUNK_DATA->Xpos               = OLD_DATA->Xpos;  //shorts are inherently signed
	SHRUNK_DATA->Ypos               = OLD_DATA->Ypos;
 	SHRUNK_DATA->Zpos               = OLD_DATA->Zpos;
	SHRUNK_DATA->OrientateDirX      = (byte) (127 * OLD_DATA->OrientateDirX);  
	SHRUNK_DATA->OrientateDirY      = (byte) (127 * OLD_DATA->OrientateDirY);
	SHRUNK_DATA->OrientateDirZ      = (byte) (127 * OLD_DATA->OrientateDirZ);
	SHRUNK_DATA->OrientateUpX       = (byte) (127 * OLD_DATA->OrientateUpX);  
	SHRUNK_DATA->OrientateUpY       = (byte) (127 * OLD_DATA->OrientateUpY);
	SHRUNK_DATA->OrientateUpZ       = (byte) (127 * OLD_DATA->OrientateUpZ);
	SHRUNK_DATA->FreeLookPitch      = (signed char)(OLD_DATA->FreeLookPitch);
	//SHRUNK_DATA->FreeLookYaw        = (signed char)(OLD_DATA->FreeLookYaw);
    //SHRUNK_DATA->dbIndex            = (short) OLD_DATA->dbIndex;
	SHRUNK_DATA->curAnim            = (signed char) OLD_DATA->curAnim;
	SHRUNK_DATA->secondaryAnim      = (signed char) OLD_DATA->secondaryAnim;
	SHRUNK_DATA->idReference        = OLD_DATA->idRefrence;

	return(SHRUNK_DATA);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Ping------------------------------------------------------------------//
LPMOVE_STRUCT CMultiplayerObj::ExpandData(SHRUNK_LPMOVE_STRUCT SHRUNK_DATA,LPMOVE_STRUCT DATA) 
{ 
	DATA->Xpos                = SHRUNK_DATA->Xpos;        
	DATA->Ypos                = SHRUNK_DATA->Ypos;
	DATA->Zpos                = SHRUNK_DATA->Zpos;
	DATA->OrientateDirX       = (SHRUNK_DATA->OrientateDirX / 127.0f);
	DATA->OrientateDirY       = (SHRUNK_DATA->OrientateDirY / 127.0f);
	DATA->OrientateDirZ       = (SHRUNK_DATA->OrientateDirZ / 127.0f);
	DATA->OrientateUpX        = (SHRUNK_DATA->OrientateUpX / 127.0f);
	DATA->OrientateUpY        = (SHRUNK_DATA->OrientateUpY / 127.0f);
	DATA->OrientateUpZ        = (SHRUNK_DATA->OrientateUpZ / 127.0f);
	DATA->FreeLookYaw         = 0.0f;//SHRUNK_DATA->FreeLookYaw;    
	DATA->FreeLookPitch       = SHRUNK_DATA->FreeLookPitch;
	//DATA->dbIndex             = SHRUNK_DATA->dbIndex;
	DATA->curAnim             = SHRUNK_DATA->curAnim;
	DATA->secondaryAnim       = SHRUNK_DATA->secondaryAnim;
	DATA->idRefrence          = SHRUNK_DATA->idReference;		

	return(DATA);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Ping------------------------------------------------------------------//
LPSHRUNK_TOSVRMOV_STRUCT CMultiplayerObj::ShrinkDataCP(LPMOVE_STRUCT OLD_DATA,
													   LPSHRUNK_TOSVRMOV_STRUCT SHRUNK_DATA) 
{

    //m_localCLSecurityIndexer += (sizeof(SHRUNK_TOSVRMOV_STRUCT)/2);
	//if(m_localCLSecurityIndexer > 30000)
	   //m_localCLSecurityIndexer = 0;


	SHRUNK_DATA->m_catagorie        = 4;
	SHRUNK_DATA->Xpos               = OLD_DATA->Xpos;  //shorts are inherently signed
	SHRUNK_DATA->Ypos               = OLD_DATA->Ypos;
 	SHRUNK_DATA->Zpos               = OLD_DATA->Zpos;
	SHRUNK_DATA->OrientateDirX      = (byte) (127 * OLD_DATA->OrientateDirX);  
	SHRUNK_DATA->OrientateDirY      = (byte) (127 * OLD_DATA->OrientateDirY);
	SHRUNK_DATA->OrientateDirZ      = (byte) (127 * OLD_DATA->OrientateDirZ);
	SHRUNK_DATA->OrientateUpX       = (byte) (127 * OLD_DATA->OrientateUpX);  
	SHRUNK_DATA->OrientateUpY       = (byte) (127 * OLD_DATA->OrientateUpY);
	SHRUNK_DATA->OrientateUpZ       = (byte) (127 * OLD_DATA->OrientateUpZ);
	SHRUNK_DATA->FreeLookPitch      = (signed char)(OLD_DATA->FreeLookPitch);
	SHRUNK_DATA->curAnim            = (signed char) OLD_DATA->curAnim;
	SHRUNK_DATA->secondaryAnim      = (signed char) OLD_DATA->secondaryAnim;
	SHRUNK_DATA->idReference        = OLD_DATA->idRefrence;
	//SHRUNK_DATA->indexer            = m_localCLSecurityIndexer;
	SHRUNK_DATA->CRCCHECK           = GetMovMsgCrC(SHRUNK_DATA);

	return(SHRUNK_DATA);
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Ping------------------------------------------------------------------//
LPMOVE_STRUCT CMultiplayerObj::ExpandDataCP(LPSHRUNK_TOSVRMOV_STRUCT SHRUNK_DATA,LPMOVE_STRUCT DATA) 
{ 
	DATA->Xpos                = SHRUNK_DATA->Xpos;        
	DATA->Ypos                = SHRUNK_DATA->Ypos;
	DATA->Zpos                = SHRUNK_DATA->Zpos;
	DATA->OrientateDirX       = (SHRUNK_DATA->OrientateDirX /  127.0f);
	DATA->OrientateDirY       = (SHRUNK_DATA->OrientateDirY / 127.0f);
	DATA->OrientateDirZ       = (SHRUNK_DATA->OrientateDirZ / 127.0f);
	DATA->OrientateUpX        = (SHRUNK_DATA->OrientateUpX / 127.0f);
	DATA->OrientateUpY        = (SHRUNK_DATA->OrientateUpY / 127.0f);
	DATA->OrientateUpZ        = (SHRUNK_DATA->OrientateUpZ / 127.0f);
	DATA->FreeLookYaw         = 0.0f;//SHRUNK_DATA->FreeLookYaw;    
	DATA->FreeLookPitch       = SHRUNK_DATA->FreeLookPitch;
	DATA->curAnim             = SHRUNK_DATA->curAnim;
	DATA->secondaryAnim       = SHRUNK_DATA->secondaryAnim;
	DATA->idRefrence          = SHRUNK_DATA->idReference;		

	return(DATA);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
BOOL CMultiplayerObj::SendItemChkToClientMsg(DWORD flags,
								             DPNID sendTo,
								             long  nID)
{
    LPITEM_CHECKTOCLIENT lpChatMessage = NULL;
	DWORD				 messageSize;
	messageSize = sizeof(ITEM_CHECKTOCLIENT);
    lpChatMessage = (LPITEM_CHECKTOCLIENT) new BYTE[messageSize];
	if (lpChatMessage == NULL)
	{
		return FALSE;
	}

	lpChatMessage->m_catagorie = 9;
    lpChatMessage->networkID   = (short)nID;

    SendData((BYTE *)lpChatMessage,
				     messageSize,
				     flags,
				     sendTo,
					 m_packetTimeout);


	if (lpChatMessage)
		delete lpChatMessage;
	    lpChatMessage=0;

	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
BOOL CMultiplayerObj::SendPingMsg(DWORD flags,
								  DPNID sendTo,//can be NULL if client
								  short data)
{
    LPPING_STRUCT datamessage = NULL;
	DWORD				messageSize;
	messageSize = sizeof(PING_STRUCT);
    datamessage = (LPPING_STRUCT) new BYTE[messageSize];
	if (datamessage == NULL)
	{
		return FALSE;
	}
	datamessage->m_catagorie = 7;
    datamessage->data   = data;

       SendData((BYTE *)datamessage,
				        messageSize,
						flags,
						sendTo,
						m_packetTimeout);

	if (datamessage)
		delete datamessage;
	    datamessage=0;


	return TRUE;
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
BOOL CMultiplayerObj::SendTextMsgToSquad(DWORD flags,
								         DPNID sendTo,//can be NULL if client
								         CString message,
								         int referenceID)
{
    

	DWORD				messageSize;
	DWORD               textSize = message.GetLength();
	if(textSize == 0)
	   return FALSE;
	messageSize = sizeof(TXT_MSG) + textSize;
    BYTE * datamessage = new BYTE[messageSize];
	if (datamessage == NULL)
	{
		return FALSE;
	}

	BYTE * shortConv = (BYTE *)referenceID;
	
	LPTXT_MSG castEntry = (LPTXT_MSG)datamessage;
	castEntry->m_catagorie = 10;
    castEntry->referenceID = referenceID;
      
    for(int txtLoop=0;txtLoop < (int)textSize;txtLoop++)
        datamessage[txtLoop+4] = message.GetAt(txtLoop);

       /*SendData((BYTE *)datamessage,
				        messageSize,
						flags,
						sendTo,
						m_packetTimeout);*/
	   if(m_txtgroupOutBox->GetCount() < 5)
	      m_txtgroupOutBox->AddPacket(messageSize,(BYTE*)datamessage,0,0);

	if (datamessage)
		delete [] datamessage;
	    datamessage=0;


	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
BOOL CMultiplayerObj::SendTextMsgToGroup(DWORD flags,
								        DPNID sendTo,//can be NULL if client
								        CString message,
								        int referenceID)
{
    

	DWORD				messageSize;
	DWORD               textSize = message.GetLength();
	if(textSize == 0)
	   return FALSE;
	messageSize = sizeof(TXT_MSG) + textSize;
    BYTE * datamessage = new BYTE[messageSize];
	if (datamessage == NULL)
	{
		return FALSE;
	}

	BYTE * shortConv = (BYTE *)referenceID;
	
	LPTXT_MSG castEntry = (LPTXT_MSG)datamessage;
	castEntry->m_catagorie = 11;
    castEntry->referenceID = referenceID;
      
    for(int txtLoop=0;txtLoop < (int)textSize;txtLoop++)
        datamessage[txtLoop+4] = message.GetAt(txtLoop);

       /*SendData((BYTE *)datamessage,
				        messageSize,
						flags,
						sendTo,
						m_packetTimeout);*/
	   if(m_txtgroupOutBox->GetCount() < 5)
	      m_txtgroupOutBox->AddPacket(messageSize,(BYTE*)datamessage,0,0);

	if (datamessage)
		delete [] datamessage;
	    datamessage=0;


	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
BOOL CMultiplayerObj::SendNameMsg(DWORD   flags,
								  DPNID   sendTo,//can be NULL if client
								  short   networkAssignedID,
								  short   messageType,
								  CString message)
{
    LPNAME_STRUCT datamessage = NULL;
	DWORD				messageSize;
	messageSize = sizeof(NAME_STRUCT);
    datamessage = (LPNAME_STRUCT) new BYTE[messageSize];
	if (datamessage == NULL)
	{
		return FALSE;
	}

	datamessage->m_catagorie          = 4;
    datamessage->messageType          = messageType;
	datamessage->networkAssignedID    = networkAssignedID;

	lstrcpy(datamessage->uniText,message);

       SendData((BYTE *)datamessage,
				        messageSize,
						flags,
						sendTo,
						m_packetTimeout);

	if (datamessage)
		delete datamessage;
	    datamessage=0;


	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CMultiplayerObj::SendData(BYTE * data,
							   DWORD size,
							   DWORD flags,
							   DPNID sendTo,
							   DWORD timeout)
{
	//Universal data send for both client or server (ENet)
	bool reliable = (flags & DPNSEND_GUARANTEED) != 0;

	EnterCriticalSection(&m_statusLock);
	m_totalBandwidthOutGoing += size;
	m_packetsOutPerPass++;
	LeaveCriticalSection(&m_statusLock);

	if(!m_netHost)
		return;

	if(m_netHost->IsServer())
	{//server side
		if(sendTo == 0)
		{
			byte catagorieLoc = 0;
			if(size > 0)
				catagorieLoc = data[0];
			m_eventLog->AddEvent("send() to NULL wrong :cat:",(DWORD)catagorieLoc);
			return;
		}
		m_netHost->Send(sendTo,(const unsigned char*)data,size,reliable);
	}
	else
	{//client side - playerID ignored, routed to the one server peer
		m_netHost->Send(sendTo,(const unsigned char*)data,size,reliable);
	}
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
BOOL CMultiplayerObj::SendSpawnMsg(DWORD flags,
								   DPNID sendTo,
								   short dbIndex,
							       CString worldFile,
								   int traceNumber,
								   short uniInt1,
								   short uniInt2,
								   short uniInt3) 
{
    LPSPAWN_STRUCT lpChatMessage = NULL;
	DWORD				messageSize;
	messageSize = sizeof(SPAWN_STRUCT);
    lpChatMessage = (LPSPAWN_STRUCT) new BYTE[messageSize];
	if (lpChatMessage == NULL)
	{
		return FALSE;
	}

	lpChatMessage->m_catagorie     = 8;
	lpChatMessage->currentTrace    = traceNumber;
    lpChatMessage->dbIndex         = dbIndex;
	lpChatMessage->uniInt1         = uniInt1;
    lpChatMessage->uniInt2         = uniInt2;
	lpChatMessage->uniInt3         = uniInt3;
    lstrcpy(lpChatMessage->worldName,   worldFile);
    lpChatMessage->CRCcheck = GetSpawnCRC(lpChatMessage);
	
    SendData((BYTE *)lpChatMessage,
				     messageSize,
				     flags,
					 sendTo,
					 m_packetTimeout);

	if (lpChatMessage)
		delete lpChatMessage;
	    lpChatMessage=0;

	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
BOOL CMultiplayerObj::SendLogFast(DWORD flags,
								  DPNID sendTo,
								  CString userName,
								  CString password) 
{
    LPLOG_STRUCT lpChatMessage = NULL;
	DWORD				messageSize;
	messageSize = sizeof(LOG_STRUCT);
    lpChatMessage = (LPLOG_STRUCT) new BYTE[messageSize];
	if (lpChatMessage == NULL)
	{
		return FALSE;
	}
	lpChatMessage->m_catagorie = 1;
	lstrcpy(lpChatMessage->logInName, userName);
	lstrcpy(lpChatMessage->logInPass, password);

	SendData((BYTE *)lpChatMessage,
				     messageSize,
				     flags,
					 sendTo,
					 m_packetTimeout);

	if (lpChatMessage)
		delete lpChatMessage;
	    lpChatMessage=0;

	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
BOOL CMultiplayerObj::SendMenuBuildPacket(    DWORD flags,
								              DPNID sendTo,
											  short messageType,
								              CString uniText,
											  short miscInt
											  ) 
{
    LPMENUBUILD_STRUCT lpChatMessage = NULL;
	DWORD				messageSize;
	messageSize = sizeof(MENUBUILD_STRUCT);
    lpChatMessage = (LPMENUBUILD_STRUCT) new BYTE[messageSize];
	if (lpChatMessage == NULL)
	{
		return FALSE;
	}

	lpChatMessage->m_catagorie = 12;
    lpChatMessage->miscint = miscInt;
    lpChatMessage->messageType = messageType;
    lstrcpy(lpChatMessage->uniText,uniText);
    lpChatMessage->CRCcheck = GetBuildMenuMsgCrC(lpChatMessage);

	SendData((BYTE *)lpChatMessage,
				     messageSize,
				     flags,
					 sendTo,
					 m_packetTimeout);
	
	if (lpChatMessage)
		delete [] lpChatMessage;
	    lpChatMessage=0;

	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
BOOL CMultiplayerObj::SendTimeSyncPacket(     DWORD flags,
								              DPNID sendTo,
											  DWORD timePassedSync,
											  short freqSync
											  ) 
{
    LPTIMESYNC_DATA lpChatMessage = NULL;
	DWORD				messageSize;
	messageSize = sizeof(TIMESYNC_DATA);
    lpChatMessage = (LPTIMESYNC_DATA) new BYTE[messageSize];
	if (lpChatMessage == NULL)
	{
		return FALSE;
	}

	lpChatMessage->m_catagorie  = 30;
    lpChatMessage->m_timePassed = timePassedSync;
	lpChatMessage->m_freqState = freqSync;

	SendData((BYTE *)lpChatMessage,
				     messageSize,
				     flags,
					 sendTo,
					 m_packetTimeout);
	
	if (lpChatMessage)
		delete lpChatMessage;
	    lpChatMessage=0;

	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 BOOL CMultiplayerObj::GetNextPacket_unsafe(int * size,BYTE ** data,DPNID * id)
{
 	POSITION posLoc = m_packetQue->FindIndex(0);
    if(!posLoc){
	    return FALSE;
	}
	    CPacketObj * packet = (CPacketObj *)m_packetQue->GetAt(posLoc);
		m_packetQue->RemoveAt(posLoc);
      
		*size = packet->m_size;
		if(packet->m_size > 0)
		{
		  *data = packet->m_data;
		  packet->m_data = NULL;
		}
		else
		{
          *data = NULL;
		}
		*id   = packet->m_communicationID;
        
		delete packet;
		packet=0;
        

	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
int CMultiplayerObj::GetFreeRuntimeEntityNetworkID()
{
	if(!m_runtimePlayersOnServer)
		return -1;

	if(!m_runtimeItems)
		return -1;

    if(m_idSearchStart > 30000)
	   m_idSearchStart = 1;

	int trace = m_idSearchStart;
	int completeCycles = 0;
	while(1)
	{
     if(trace > 30000)
	 {
	    trace = 1;
        completeCycles++;
		if(completeCycles > 2)
		   return -1;
	 }

	 if(!m_runtimePlayersOnServer->VerifyValidByNetID_safe(trace))
		 if(!m_runtimeItems->VerifyValidByNetID_safe(trace))
		 {//trace found
           m_idSearchStart = trace+1;
		   return trace;
		}//end trace found
		trace++;
	}
	return -1;
}




//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
HRESULT WINAPI DirectPlayMessageHandler( PVOID pvUserContext, 
                                                          DWORD dwMessageId, 
                                                          PVOID pMsgBuffer )
{
	CMultiplayerObj * mPtr = (CMultiplayerObj *)pvUserContext;
    //Try not to stay in this message handler for too long, otherwise
    //there will be a backlog of data.  The best solution is to 
    //queue data as it comes in, and then handle it on other threads.
    
    //This function is called by the DirectPlay message handler pool of 
    //threads, so be careful of thread synchronization problems with shared memory
    //dwMessageId

    switch( dwMessageId )
    {
        case DPN_MSGID_CREATE_PLAYER:
        {
			mPtr->UpdatePlayersOnline(1);
			PDPNMSG_CREATE_PLAYER createPlayerMsg;
            createPlayerMsg = (PDPNMSG_CREATE_PLAYER)pMsgBuffer;
            
            //set log on flags and things here
			if(mPtr->m_primaryPlayerCreated)
			{//Normal proc
				if(createPlayerMsg->pvPlayerContext)
				{
					LPCONTEXT_VALIDATION cntxValidation = (LPCONTEXT_VALIDATION)createPlayerMsg->pvPlayerContext;
					if(!mPtr->m_pendingLogons->SetDPNID(createPlayerMsg->dpnidPlayer, cntxValidation->username))
					{//set SetDPNIDandCONTEXT failed
						mPtr->m_eventLog->AddEvent("SetDPNID()-Failed:",cntxValidation->username);
					}//end set SetDPNIDandCONTEXT failed
				}
				else
				{
					mPtr->m_eventLog->AddEvent("***NULL:pvPlayerContext DPN_MSGID_CREATE_PLAYER***");
					break;
				}
			}//end Normal proc
			else
			{
		        mPtr->m_eventLog->AddEvent("Primary Server Player Created");
                mPtr->m_primaryPlayerCreated = TRUE;
			}

			break;
        }

        case DPN_MSGID_DESTROY_PLAYER:
        {
			PDPNMSG_DESTROY_PLAYER disconnectDataMsg;
			disconnectDataMsg = (PDPNMSG_DESTROY_PLAYER)pMsgBuffer;
            mPtr->UpdatePlayersOnline(-1);
            

			LPCONTEXT_VALIDATION cntxValidation = (LPCONTEXT_VALIDATION)disconnectDataMsg->pvPlayerContext;
            if(!cntxValidation)
			{
				mPtr->m_eventLog->AddEvent("Cannot find account to log off NULL pvPlayerContext:",disconnectDataMsg->dpnidPlayer);
			    break;
			}

			mPtr->m_pendingLogons->LogonComplete(cntxValidation->username);//POSSIBLY CRASHES HERE-cant make dependant on account because somtimes it doesnt get created

			CAccountObject * acPtr = (CAccountObject *) mPtr->m_runtimeAccountDB->GetAccountByID_LOS(disconnectDataMsg->dpnidPlayer);//account runtime locked here
			if(!acPtr)
			{
				mPtr->m_eventLog->AddEvent("Cannot find account to log off DPN_MSGID_DESTROY_PLAYER:",disconnectDataMsg->dpnidPlayer);
				delete cntxValidation;
				cntxValidation = 0;
			    break;
			}

			CString userName = acPtr->m_accountName;
			acPtr->m_connectedTemp = FALSE;
            //calculate minutes played
			acPtr->m_timePlayedMinutes += (GetTickCount() - acPtr->m_logonTimeStamp)/60000;//calculate
        
			if(acPtr->m_aiServer == TRUE)
			{
			    mPtr->m_eventLog->AddEvent("AISERVER Logging off:",acPtr->m_accountName);
				//clear all ai npcs based on ai server connection DPNID
				mPtr->m_runtimePlayersOnServer->Lock();
				while(mPtr->m_runtimePlayersOnServer->RemoveByDPNID(acPtr->m_currentID))
				{
                 
				}
				mPtr->m_runtimePlayersOnServer->Unlock();
				
				mPtr->m_characterPendingSaveSystem->AddCharacterToPendingList(NULL,1,userName,-1);//removes account
				mPtr->m_runtimeAccountDB->Unlock();
				delete cntxValidation;
				cntxValidation = 0;
                break;
			}

            CPlayerObject * currentPlayer = mPtr->m_runtimePlayersOnServer->GetByDPNID_Alive_LOS(disconnectDataMsg->dpnidPlayer);
            if(currentPlayer)
			{
			    mPtr->m_eventLog->AddEvent("cannot imediatly log off-so flag is set to timeout-:",acPtr->m_accountName);
			    currentPlayer->m_justSpawned = FALSE;
				mPtr->m_runtimePlayersOnServer->Unlock();
			}
			
            if(!currentPlayer)
			{//no live player is ok to que save point
				bool placedPendingData = false;
				if(acPtr->m_characterOnAccount)
				{
				        mPtr->m_characterPendingSaveSystem->AddCharacterToPendingList(acPtr->m_characterOnAccount,1,acPtr->m_accountName,-1);
						placedPendingData = true;
				}
				if(placedPendingData == false)
					mPtr->m_characterPendingSaveSystem->AddCharacterToPendingList(NULL,1,userName,-1);//removes account
			}//end no live player is ok to que save point

			mPtr->m_runtimeAccountDB->Unlock();
			
			delete cntxValidation;
			cntxValidation = 0;
			
            break;
        }

		case DPN_MSGID_RETURN_BUFFER:
	    {
            //PDPNMSG_RETURN_BUFFER rBuf = (PDPNMSG_RETURN_BUFFER)pMsgBuffer;

			break;
		}

		case DPN_MSGID_INDICATED_CONNECT_ABORTED:
		{//lost connection before player was created
			mPtr->m_eventLog->AddEvent("***Lost connection before player created***");
            PDPNMSG_INDICATED_CONNECT_ABORTED lostDataMsg = (PDPNMSG_INDICATED_CONNECT_ABORTED)pMsgBuffer;
			if(lostDataMsg->dwSize < sizeof(CONTEXT_VALIDATION))
			{
			   mPtr->m_eventLog->AddEvent("***Lost connection before player created-Bad Size CONTEXT_VALIDATION***");
			   break;
			}

			if(lostDataMsg->pvPlayerContext)
			{
				LPCONTEXT_VALIDATION cntxValidation = (LPCONTEXT_VALIDATION)lostDataMsg->pvPlayerContext;
				mPtr->m_eventLog->AddEvent("***Lost connection before player created-starting verification:",cntxValidation->username);
				mPtr->m_pendingLogons->LogonComplete(cntxValidation->username);//close off any pending connections
				delete cntxValidation;
				cntxValidation = 0;
			}
			else
			{
                mPtr->m_eventLog->AddEvent("***NULL:pvPlayerContext***");
			}
			
			break;
		}//end lost connection before player was created

        case DPN_MSGID_INDICATE_CONNECT:
		{
			//break;
			
			int test = sizeof(DPN_MSGID_INDICATE_CONNECT);
			//Validate Connection
            PDPNMSG_INDICATE_CONNECT connectDataMsg;

			connectDataMsg = (PDPNMSG_INDICATE_CONNECT)pMsgBuffer;
			connectDataMsg->pvPlayerContext = NULL;

			if(connectDataMsg->dwUserConnectDataSize == sizeof(CONTEXT_VALIDATION)){
               LPCONTEXT_VALIDATION cntxValidation = (LPCONTEXT_VALIDATION)connectDataMsg->pvUserConnectData;
               CString userName = cntxValidation->username;
               CString password = cntxValidation->password;

			   connectDataMsg->dwReplyDataSize = 1;
			   int locVersion = mPtr->m_currentVersion;
			   
			   if(cntxValidation->versionInfo != locVersion)
			   {//out of date version
				   char replyCode = 'v';
			       connectDataMsg->pvReplyData = &replyCode;
                   mPtr->m_eventLog->AddEvent("Invalid version"+userName);
                   return 2;
			   }//end of out of date version	

               if(!connectDataMsg->pAddressPlayer){//cannot log IP
				   //m_eventLog->AddEvent("Cannot log IP.. rejecting connection");
				   char replyCode = 'i';
			       connectDataMsg->pvReplyData = &replyCode;
				   return 2;
			   }//end cannot log IP

			   if(mPtr->m_accountDBMode == 1)
			   {//db connection authenticate option
					CString ipAdress = mPtr->GetIPAddress(connectDataMsg->pAddressPlayer);
					//This is a critical timeframe
					//security wise we will at this time make sure IP and username dont spam, but will allow half access
					//we will authenticate the password in Finalize Logon
					int result = mPtr->m_pendingLogons->Logon(userName,password,ipAdress);
					mPtr->m_eventLog->LogByPriority("Pre Auth "+userName+" :",(DWORD)result,2);

					switch(result)
					{
						case 3:
						{//ip spam
							char replyCode = 'i';
							connectDataMsg->pvReplyData = &replyCode;
							mPtr->m_eventLog->AddEvent("***IP SPAM***",userName,ipAdress);
							return 2;//REJECT
						}//end ip spam
						case 2:
						{//username spam
							char replyCode = 'u';
							connectDataMsg->pvReplyData = &replyCode;
							mPtr->m_eventLog->AddEvent("***USERNAME SPAM***",userName,ipAdress);
							return 2;//REJECT
						}//end username spam
						default:
						{//success
							int strLen = (int)userName.GetLength();
							if(strLen > 24)
								strLen = 24;
							if(strLen > 0)
							{
								CONTEXT_VALIDATION * context = new CONTEXT_VALIDATION;
								ZeroMemory(context,sizeof(CONTEXT_VALIDATION));
								connectDataMsg->pvPlayerContext = context;
								CopyMemory(context->username,userName.GetBuffer(),strLen);
								mPtr->m_eventLog->AddEvent("Authenicated into session:",userName,ipAdress);
								return S_OK;//allow log in even though he may be rejected in finalize logon
							}
							mPtr->m_eventLog->AddEvent("***INVALID LOGON DATA***",userName,ipAdress);
							return 2;//REJECT
						}//end success
					}
					/*else if(result == 4)
					{//no connection available
					  char replyCode = 'x';
					  connectDataMsg->pvReplyData = &replyCode;
					  mPtr->m_eventLog->AddEvent("***INVALID PENDING LOGON MEM***",userName,ipAdress);
					  return 2;//REJECT
					}
					else if(result == 5)
					{//no connection available
					  char replyCode = 'm';
					  connectDataMsg->pvReplyData = &replyCode;
					  mPtr->m_eventLog->AddEvent("***NON-PUBLIC MODE***",userName,ipAdress);
					  return 2;//REJECT
					}
					else if(result == 6)
					{//no connection available
					  char replyCode = 'c';
					  connectDataMsg->pvReplyData = &replyCode;
					  mPtr->m_eventLog->AddEvent("***CONNECTION UNAVAILABLE***",userName,ipAdress);
					  return 2;//REJECT
					}//end no connection available
					else if(result == 11)
					{//logged on already
					  char replyCode = 'l';
					  connectDataMsg->pvReplyData = &replyCode;
					  mPtr->m_eventLog->AddEvent("*ALREADY LOGGED ON*",userName,ipAdress);
					  return 2;//REJECT
					}//end logged on already
					else if(result == 10)
					{//username or password is incorrect
					  char replyCode = 'a';
					  connectDataMsg->pvReplyData = &replyCode;
					  mPtr->m_eventLog->AddEvent("*INCORRECT USERNAME PASSWORD*",userName,ipAdress);
					  return 2;//REJECT
					}//end username or password is incorrect
					else if(result == 9)
					{//account suspended
					  char replyCode = 'x';
					  connectDataMsg->pvReplyData = &replyCode;
					  mPtr->m_eventLog->AddEvent("*ACCOUNT SUSPENDED LOGON ATTEMPT*",userName,ipAdress);
					  return 2;//REJECT
					}//end account suspended
					else if(result == 8)
					{//username spam
					  char replyCode = 'u';
					  connectDataMsg->pvReplyData = &replyCode;
					  mPtr->m_eventLog->AddEvent("***USERNAME SPAM***",userName,ipAdress);
					  return 2;//REJECT
					}//end username spam
					else if(result == 7)
					{//IP spam
					  char replyCode = 'i';
					  connectDataMsg->pvReplyData = &replyCode;
					  mPtr->m_eventLog->AddEvent("***IP SPAM***",userName,ipAdress);
					  return 2;//REJECT
					}//end IP spam
					else if(result == 15)
					{//IP spam
					  char replyCode = 'w';
					  connectDataMsg->pvReplyData = &replyCode;
					  mPtr->m_eventLog->AddEvent("***PASSWORD INCORRECT***",userName,ipAdress);
					  return 2;//REJECT
					}//end IP spam
					else if(result == 18)
					{//expired account
					  char replyCode = 'x';
					  connectDataMsg->pvReplyData = &replyCode;
					  mPtr->m_eventLog->AddEvent("***EXPIRED ACCOUNT***",userName,ipAdress);
					  return 2;//REJECT
					}//end expired account
					else
					{//UNKNOWN
					  char replyCode = 'm';
					  connectDataMsg->pvReplyData = &replyCode;
					  mPtr->m_eventLog->AddEvent("***UNKNOWN REJECTION VALUE***",userName,ipAdress);
					  return 2;//REJECT
					}//end UNKNOWN*/
					return 2;
			   }//end db connection authenticate option

            
			}
            mPtr->m_eventLog->AddEvent("General Rejection-possible out of date version");
			return 2;//just reject if it gets here
		}

		case DPN_MSGID_CONNECT_COMPLETE:
		{//client specific
            PDPNMSG_CONNECT_COMPLETE connectCompleteMsg;
            connectCompleteMsg = (PDPNMSG_CONNECT_COMPLETE)pMsgBuffer;

			if(connectCompleteMsg->dwApplicationReplyDataSize > 0)
			{
			   char * tempConv = (char *)connectCompleteMsg->pvApplicationReplyData;
			   char replyCode = *tempConv;
			   if(replyCode == 'i')
				  mPtr->SetConnectionStatus("Cannot log IP - Ctl+Q to quit", 2);
			   if(replyCode == 'a')
			      mPtr->SetConnectionStatus("Invalid Username or Password - Ctl+Q to quit", 2);
			   if(replyCode == 'k')
			      mPtr->SetConnectionStatus("Log in success", 1);//connected ok
			   if(replyCode == 'b')
			      mPtr->SetConnectionStatus("Log in denied e-mail avaria@endlessages.com for info - Ctl+Q to quit", 2);
			   if(replyCode == 'v')
			      mPtr->SetConnectionStatus("Invalid version - Ctl+Q to quit", 2);
			   if(replyCode == 'x')
			      mPtr->SetConnectionStatus("Account has expired - Ctl+Q to quit", 2);
			   if(replyCode == 'l')
			      mPtr->SetConnectionStatus("Account is already logged in - Ctl+Q to quit", 2);
			   if(replyCode == 'e')
			      mPtr->SetConnectionStatus("Server isnt open yet-try back shortly - Ctl+Q to quit", 2);
			   if(replyCode == 'c')
			      mPtr->SetConnectionStatus("Server Connection currently unavailable please try back shortly - Ctl+Q to quit", 2);
			   if(replyCode == 'l')
			      mPtr->SetConnectionStatus("Already logged on - Ctl+Q to quit", 3);//signal to wait then try again
			   if(replyCode == 'u')
			      mPtr->SetConnectionStatus("Already logging on - Ctl+Q to quit", 2);
			   if(replyCode == 'i')
			      mPtr->SetConnectionStatus("Already logging on ** - Ctl+Q to quit", 2);
			   if(replyCode == 'w')
			      mPtr->SetConnectionStatus("Incorrect Password ** - Ctl+Q to quit", 2);
			   if(replyCode == 'm')
			      mPtr->SetConnectionStatus("Server is Restarting try back later ** - Ctl+Q to quit", 2);
			}

			if(DPNERR_INVALIDPASSWORD == connectCompleteMsg->hResultCode){
			   mPtr->m_eventLog->AddEvent("Invalid password");
			    
			   break;
			}
       	       
			if(DPNERR_HOSTREJECTEDCONNECTION == connectCompleteMsg->hResultCode){
			   mPtr->m_eventLog->AddEvent("Server Rejected Connection");
			   break;
			} 

			if(DPNERR_SESSIONFULL == connectCompleteMsg->hResultCode){
               mPtr->m_eventLog->AddEvent("Server Full");
			   break;
			}

			if(S_OK == connectCompleteMsg->hResultCode)
			{
			   mPtr->SetEnabledClient_safe(TRUE);
			   
			   break;
			}
			
			mPtr->m_eventLog->AddEvent("Connection failed...");
            break;
		}

        case DPN_MSGID_TERMINATE_SESSION:
        {
			PDPNMSG_TERMINATE_SESSION temSessionMsg = (PDPNMSG_TERMINATE_SESSION)pMsgBuffer;
			if(temSessionMsg->dwTerminateDataSize < 1)
			   break;
			
			char * res = (char *)temSessionMsg->pvTerminateData;

			switch(*res)
			{
				case 'n':
				{
					mPtr->SetConnectionStatus("Server is closed atm, try back shortly - Ctl+Q to quit", 2);
					break;
				}
				case 'a':
				{
					mPtr->SetConnectionStatus("Account Already Logged on - Ctl+Q to quit", 2);
					break;
				}
				case 'i':
				{
					mPtr->SetConnectionStatus("Inactive Account - Ctl+Q to quit", 2);
					break;
				}
				case 'u':
				{
					mPtr->SetConnectionStatus("Invalid Username or password - Ctl+Q to quit", 2);
					break;
				}
				case 'y'://unknown error disconnecting
				{
					break;
				}
				default:
				{
					mPtr->SetConnectionStatus("Disconnected from server - Ctl+Q to quit", 2);
					break;
				}
			}

            break;
        }

		case DPN_MSGID_ENUM_HOSTS_RESPONSE:
        {//client specific
            PDPNMSG_ENUM_HOSTS_RESPONSE pEnumHostsResponseMsg;
            pEnumHostsResponseMsg = (PDPNMSG_ENUM_HOSTS_RESPONSE)pMsgBuffer;
            DPNHANDLE                asyncLocalHandle;
			//specify password
            WCHAR  wCharPassCnv[MAX_PATH];
            MultiByteToWideChar(CP_ACP, 0, mPtr->m_serverKey, -1, wCharPassCnv, sizeof(wCharPassCnv)/sizeof(wCharPassCnv[0]));
            DPN_APPLICATION_DESC dpnAppDesc = *pEnumHostsResponseMsg->pApplicationDescription;
			dpnAppDesc.pwszPassword = wCharPassCnv;
			mPtr->m_eventLog->AddEvent("Attemting connection to",mPtr->ConvertWideStringToCString(dpnAppDesc.pwszSessionName));
			
			LPCONTEXT_VALIDATION cntxValidation = NULL;
	        DWORD messageSize = sizeof(CONTEXT_VALIDATION);
			cntxValidation = (LPCONTEXT_VALIDATION) new BYTE[messageSize];
			lstrcpy(cntxValidation->password,mPtr->m_passWord);
			lstrcpy(cntxValidation->username,mPtr->m_userName);

			cntxValidation->versionInfo = mPtr->m_currentVersion;
			
			HRESULT hr = mPtr->m_clientInterface->Connect(&dpnAppDesc,                          // the application desc
                                                    pEnumHostsResponseMsg->pAddressSender,                              //address of the host of the session
                                                    pEnumHostsResponseMsg->pAddressDevice,    //address of the local device the enum responses were received on
                                                    NULL, 
													NULL,                          //DPN_SECURITY_DESC, DPN_SECURITY_CREDENTIALS
                                                    (BYTE*)cntxValidation,         //user data
													messageSize,                   //user data size
                                                    NULL, 
													&asyncLocalHandle,                          //async context, async handle,
                                                    DPNCONNECT_OKTOQUERYFORADDRESSING); //flags
         
			if(cntxValidation)
			{
               delete [] cntxValidation;
			   cntxValidation = 0;
			}

            if(mPtr->m_dpAsncEnumHostHandle)
               mPtr->m_clientInterface->CancelAsyncOperation( mPtr->m_dpAsncEnumHostHandle, 0 );
	  
            break;
        }

        case DPN_MSGID_RECEIVE:
        {
            PDPNMSG_RECEIVE pReceiveMsg;
            pReceiveMsg = (PDPNMSG_RECEIVE)pMsgBuffer;
			
			if(pReceiveMsg->dwReceiveDataSize <= 0){//invalid data
               mPtr->m_eventLog->AddEvent("Invalid packet Size ",pReceiveMsg->dwReceiveDataSize);
			}//end invalid data
			else{//add packet to que
			
			   BOOL addPacket = TRUE;
			   if(mPtr->m_serverInterface)
			   {//server specific
				if(pReceiveMsg->dwReceiveDataSize > mPtr->m_maxPacketSizeNonAdmin)
				{//packet size too large 
				   BOOL killUser = TRUE;
				   mPtr->m_runtimeAccountDB->Lock();
				   CAccountObject * accountTrackPtr = (CAccountObject *) mPtr->m_runtimeAccountDB->GetAccountByID(pReceiveMsg->dpnidSender);
			       if(accountTrackPtr)
				   {
				      if(accountTrackPtr->m_administratorAccess)
                         killUser = FALSE;
				   }
				   else 
					   mPtr->m_eventLog->AddEvent("Security Violation cannot find account in runtime DB:",(DWORD)pReceiveMsg->dpnidSender);

				   if(killUser)
				   {
					  if(mPtr->m_serverInterface){
				         mPtr->m_serverInterface->DestroyClient(pReceiveMsg->dpnidSender,NULL,NULL,0);//bad
                         mPtr->m_packetQue->PurgeAllByID(pReceiveMsg->dpnidSender,FALSE);
					  }
			          if(accountTrackPtr)
						 mPtr->m_eventLog->AddEvent(operator+("Security Violation paket too large: ",accountTrackPtr->m_accountName),pReceiveMsg->dwReceiveDataSize);
					  else
						 mPtr->m_eventLog->AddEvent("Security Violation ",pReceiveMsg->dwReceiveDataSize);
				      addPacket = FALSE;
				   }
                   mPtr->m_runtimeAccountDB->Unlock();
				}//end packet size too large
			   }//end server specific

               if(addPacket)
			   if(pReceiveMsg->dwReceiveDataSize > 0)
			   if(pReceiveMsg->pReceiveData)
			   {
				   mPtr->m_packetQue->AddPacket(pReceiveMsg->dwReceiveDataSize,
				                                pReceiveMsg->pReceiveData,
								                pReceiveMsg->dpnidSender,
										        0); 
				   mPtr->AddPacketPerPass();
				   if(mPtr->m_pausePrimarySpinEvent)
				      SetEvent(mPtr->m_pausePrimarySpinEvent[mPtr->RotateTrigger_safe()]); 
			   }
			}//end add packet to que
            
			break;
        }
    }

    return S_OK;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CMultiplayerObj::ConvertGenericStringToWide( WCHAR* wstrDestination, 
												  const TCHAR* tstrSource, 
                                                  int cchDestChar )
{
    if( wstrDestination==NULL || tstrSource==NULL )
        return;

#ifdef _UNICODE
    if( cchDestChar == -1 )
	    wcscpy( wstrDestination, tstrSource );
    else
	    wcsncpy( wstrDestination, tstrSource, cchDestChar );
#else
    ConvertAnsiStringToWide( wstrDestination, tstrSource, cchDestChar );
#endif
}
//-----------------------------------------------------------------------------
//Name: DXUtil_ConvertAnsiStringToWide()
//Desc: This is a UNICODE conversion utility to convert a CHAR string into a
//      WCHAR string. cchDestChar defaults -1 which means it 
//      assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
void CMultiplayerObj::ConvertAnsiStringToWide( WCHAR* wstrDestination, const CHAR* strSource, 
                                     int cchDestChar )
{
    if( wstrDestination==NULL || strSource==NULL )
        return;

    if( cchDestChar == -1 )
        cchDestChar = (int)strlen(strSource)+1;

    MultiByteToWideChar( CP_ACP, 0, strSource, -1, 
                         wstrDestination, cchDestChar-1 );

    wstrDestination[cchDestChar-1] = 0;
}
//-----------------------------------------------------------------------------
//Name: DXUtil_ConvertWideStringToAnsi()
//Desc: This is a UNICODE conversion utility to convert a WCHAR string into a
//      CHAR string. cchDestChar defaults -1 which means it 
//      assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
void WINAPI CMultiplayerObj::ConvertWideStringToAnsi( CHAR* strDestination, const WCHAR* wstrSource, 
                                     int cchDestChar )
{
    if( strDestination==NULL || wstrSource==NULL )
        return;

    if( cchDestChar == -1 )
        cchDestChar = (int)wcslen(wstrSource)+1;

    WideCharToMultiByte( CP_ACP, 0, wstrSource, -1, strDestination, 
                         cchDestChar-1, NULL, NULL );

    strDestination[cchDestChar-1] = 0;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
CString WINAPI CMultiplayerObj::ConvertWideStringToCString(const WCHAR* wstrSource)
{
    char temp[256];
	ConvertWideStringToAnsi(temp,wstrSource,-1);
    CString ret=temp;
    return ret;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
CString WINAPI CMultiplayerObj::GetIPAddress(IDirectPlay8Address * pAddress)
{

               WCHAR wstrHostname[ 256 ] = {0};
               DWORD dwSize = sizeof(wstrHostname);
               DWORD dwDataType = 0;
               HRESULT hr = pAddress->GetComponentByName( L"hostname", wstrHostname, &dwSize, &dwDataType );
               if( FAILED(hr) )
                   return "Undefined";

               return ConvertWideStringToCString(wstrHostname);

              
			   return "Undefined";
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
CString WINAPI CMultiplayerObj::GetIPAddress(DPNID id)
{
   if(!m_serverInterface)
	   return "Undefined";

   IDirectPlay8Address * pAddress = NULL;
   if(m_serverInterface->GetClientAddress(id,&pAddress,0)==S_OK)
   {
               CString returnString;
	           DWORD bufferSize=0;
			   DWORD type = DPNA_DATATYPE_STRING;
			   pAddress->GetComponentByName(DPNA_KEY_HOSTNAME,
			                                                      NULL,
                                                                  &bufferSize,
								                                  &type);

			   WCHAR  ipAdress[250];
			   if(bufferSize < 240 && bufferSize > 0 )//reasonable range
			   if(pAddress->GetComponentByName(DPNA_KEY_HOSTNAME,
			                                                         &ipAdress,
                                                                     &bufferSize,
																	 &type)==S_OK){
				  
				   
				  returnString = ConvertWideStringToCString(ipAdress);
				  if(pAddress){
                     pAddress->Release();
					 pAddress=0;
				  }
				  return returnString;
			   }
			   return "Undefined";

   }
   return "Undefined";
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
long CMultiplayerObj::GetVersionFromFile()//ussually client side
{
    CFileException exc,exec2;
    CFile fl,file;
    BOOL res = fl.Open("VERSIONINF.VER",CFile::modeRead, &exc);
	 if(res == FALSE){
	    return 1;
	 }
	 else
	 fl.Close();

    //end verify files existance	
	//CFile * file = new CFile( "VERSIONINF.VER", CFile::modeReadWrite );
	res = file.Open("VERSIONINF.VER", CFile::modeReadWrite, &exec2);
	if(res == FALSE)
	{
	    return 1;
	}

    BYTE * fileRawData = NULL;
    DWORD fileLengthInBytes = 0;
	fileLengthInBytes = (DWORD)file.GetLength();
	if(fileLengthInBytes != 0)
	{//BUILD old file byte array
       
	   fileRawData = new BYTE[fileLengthInBytes];
	   file.Read(fileRawData,fileLengthInBytes);
	   file.Close();

	   long version = 0;
       CopyMemory(&version,fileRawData,sizeof(long));
	   
	   delete [] fileRawData;
	   fileRawData = 0;
	   return version;
	}//END BUILD old file byte array
    
	file.Close();
    return 1;
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CMultiplayerObj::WriteDebugFile(CString filename,CString debugString)
{
	CFileException exc;
    CFile fl;
	CArchive the_out_Archive(&fl,CArchive::store);
    BOOL res = fl.Open(filename,CFile::modeCreate | CFile::modeWrite, &exc);
       the_out_Archive.WriteString( debugString );  
	   the_out_Archive.WriteString( "\n");  
	
	 the_out_Archive.Close();
     fl.Close(); 
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
CString CMultiplayerObj::GetIPDataFromDisk()
{
     CFileException exc,exc2;
    CFile fl,file;
    BOOL res = fl.Open("CONDAT.VER",CFile::modeRead, &exc);
	 if(res == FALSE){
	    return "";
	 }
	 else
	 fl.Close();

    //end verify files existance	
	//CFile file = new CFile( "CONDAT.VER", CFile::modeReadWrite );
	res = file.Open("CONDAT.VER", CFile::modeReadWrite, &exc2);
	if(res == FALSE){
	    return "";
	 }

    BYTE * fileRawData = NULL;
    DWORD fileLengthInBytes = 0;
	fileLengthInBytes = (DWORD)file.GetLength();
	if(fileLengthInBytes != 0)
	{//BUILD old file byte array 
	   fileRawData = new BYTE[fileLengthInBytes];
	   file.Read(fileRawData,fileLengthInBytes);
	   file.Close();
	   

       if(fileLengthInBytes > 0)
	   {
	   char * dataString = new char[fileLengthInBytes+1];
	   ZeroMemory(dataString,(fileLengthInBytes+1));
	   for(int i=0;i<(int)fileLengthInBytes;i++)
	   {
         dataString[i] = fileRawData[i];
	   }
       CString tempRet = dataString;
	   delete [] dataString;
	   dataString = 0;

	   delete [] fileRawData;
	   fileRawData = 0;
	   
	   return tempRet;
	   }
	}//END BUILD old file byte array
    	
    return "";
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
 void  CMultiplayerObj::BanIPByUsername(CString username)
{
     CAccountObject * accountPtr = m_runtimeAccountDB->GetAccountByUsername_LOS(username.GetBuffer());
	 if(!accountPtr)
		 return;
	 //disconnect if online
	 if(accountPtr->m_loggedOn)
	    Disconnect(accountPtr->m_currentID);

	 //Ban IP
     m_ipBlockList->AddIpToBlockList(accountPtr->m_currentIP);
	 m_runtimeAccountDB->Unlock();
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
 void  CMultiplayerObj::UNBanIPByUsername(CString username)
{
     CAccountObject * accountPtr = m_runtimeAccountDB->GetAccountByUsername_LOS(username.GetBuffer());
	 if(!accountPtr)
		 return;

     //Ban IP
     m_ipBlockList->UnblockIP(accountPtr->m_currentIP);
	 m_runtimeAccountDB->Unlock();
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
BOOL CMultiplayerObj::SendAttribMsg(DWORD flags,
							   DPNID sendTo,
							   int attribute,
							   int uniInt,
						       long refrenceID,
							   int uniInt2,
							   short uniShort3,
							   CPlayerObject * runtimePlOption) 
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
	lpChatMessage->CRCcheck    = GetAttribCRC(lpChatMessage);
    
	
    BOOL storeIt = TRUE;
	if(runtimePlOption)
	{
	   if(runtimePlOption->m_aiControlled)
		  storeIt = FALSE;
	}
	else
	   storeIt = FALSE;
    
	if(sendTo == DPID_SERVERPLAYER)
	{
	   if(m_attributeOutBox)   // client only
	      m_attributeOutBox->AddPacket(messageSize,(BYTE*)lpChatMessage,0,0);
	}
	else if(storeIt)
	{
	   if(runtimePlOption->m_attributeOutbox->GetCount() < 30)
       runtimePlOption->m_attributeOutbox->AddPacket(messageSize,(BYTE*)lpChatMessage,0,runtimePlOption->m_currentWorldID);
	}
	else
       SendData((BYTE *)lpChatMessage,
				        messageSize,
				        flags,
					    sendTo,
					    m_packetTimeout);
    

	if (lpChatMessage){
		delete lpChatMessage;
		lpChatMessage=0;
	}

	return TRUE;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
int WINAPI CMultiplayerObj::DBLogonProcessor(   CString userName,
												CString password,
												CString currentIP,
												CDBConnectionSystem * authConnection)
{
	
		if(m_pendingLogons == NULL)
		   return 4;

		m_eventLog->AddEvent("Attempt Auth:"+userName+" IP:"+currentIP);

		char quoteX[2];
        quoteX[0] = '"';
        quoteX[1] = 0;
	    userName.Replace("'",quoteX);
		password.Replace("'",quoteX);

		if(m_serverMode == 1)
            return 7;//non-public mode

		DWORD logonProcessTimeStamp = GetTickCount();

		//establish base verification strings
		//check banned ip/username to DB and then log if banned
		
		CString temp;
		//check banned ip/username to DB and then log if banned
		temp = m_pendingLogons->m_ipUsernameBannedCheck;
		temp.Replace("$USER",userName);
		temp.Replace("$IP",currentIP);
        _RecordsetPtr recPtr = authConnection->Query(temp);
		if(authConnection->GetCount(recPtr) > 0)
		{//reject banned
            authConnection->Close(recPtr);
			DWORD processingTimeElapsed = GetTickCount()-logonProcessTimeStamp;
			m_eventLog->AddEvent("Banned Username attempted connection:"+userName,processingTimeElapsed);
			m_pendingLogons->LogonComplete(userName);
			return 9;
		}//end reject banned
		authConnection->Close(recPtr);
		//check if logged in already use database not local then set log on flag
		temp = m_pendingLogons->m_authenticateUserPass;
		temp.Replace("$USERNAME",userName);
		//check if logged on
		int loggedOnStatus = 0;
		_RecordsetPtr accPtr = authConnection->Query(temp);

		m_eventLog->LogByPriority("username pass query:"+userName,2);
		if(authConnection->GetCount(accPtr) > 0)
		{//valid
			loggedOnStatus = authConnection->GetIntData("LOGGEDON",accPtr);
		    if(loggedOnStatus == 0)
			{
			   CString encPass = authConnection->SanitizeFormatOutENC(authConnection->GetStringData("PASSWORD",accPtr));

			   EnterCriticalSection(&m_pendingLogons->m_cSection);
			   CString decryptedPassword = m_pendingLogons->m_encryptionManager->DecryptString64(encPass.GetBuffer());//thread violation possible
			   LeaveCriticalSection(&m_pendingLogons->m_cSection);
               
			   // Accept either the encrypted form (what the old web signup stored) or a
			   // plain-text row (dev accounts inserted by hand). Proper password hashing
			   // replaces both when the account system is modernized.
			   if(password.CompareNoCase(decryptedPassword)!=0 && password.Compare(encPass)!=0)
			   {//password is incorrect
                  m_eventLog->AddEvent("Password is incorrect:"+userName);
				  authConnection->Close(accPtr);
			      m_pendingLogons->LogonComplete(userName);
				  return 5;
			   }//end password is incorrect

			   if(m_allowOnlyPaidAccountsToConnect == TRUE)
				   if(authConnection->GetIntData("PAIDAC",accPtr) <= 0)//non-active-paid account
				   {
				      m_eventLog->AddEvent("Account is inactive:"+userName);
					  authConnection->Close(accPtr);
				      m_pendingLogons->LogonComplete(userName);
				      return 2;//rejecting for unpaid
				   }
			}
			else
			{
			   authConnection->Close(accPtr);
               DWORD processingTimeElapsed = GetTickCount()-logonProcessTimeStamp;
			   m_eventLog->AddEvent("Already logged on:"+userName,processingTimeElapsed);//crashed here ++ATL
			   m_pendingLogons->LogonComplete(userName);
			   return 6;
			}
		}//end valid
		else
		{//didnt autenticate at all
           authConnection->Close(accPtr);
           DWORD processingTimeElapsed = GetTickCount()-logonProcessTimeStamp;
		   m_eventLog->AddEvent("Log on failed:"+userName,processingTimeElapsed);
		   m_pendingLogons->LogonComplete(userName);	
		   return 5;
		}//end didnt autenticate at all
		authConnection->Close(accPtr);
		
		//prelim is all good so let connection through
		DWORD processingTimeElapsed = GetTickCount()-logonProcessTimeStamp;
		m_eventLog->AddEvent("User Authenticated:"+userName+" IP:"+currentIP,processingTimeElapsed);
		return 1;
}

int CMultiplayerObj::FinalizeLogon(CString & userName, 
								   CDBConnectionSystem * dbConnection,
								   CDBConnectionSystem * authConnection)
{
	DWORD logonProcessTimeStamp = GetTickCount();
	
    CString temp;
	BOOL gm = 0;
	BOOL admin = 0;
	BOOL aiserver = 0;
	int minutesPlayed = 0;
	int acID = 0;
	int currentCharCount = 0;
	int houseExists = 0;
	BOOL deafMute = 0;
	CString gameName = "";
	int rcPaidCount = 0;

	temp = m_pendingLogons->m_authenticateUserPass;
    temp.Replace("$USERNAME",userName);
	_RecordsetPtr accPtr = authConnection->Query(temp);
	if(authConnection->GetCount(accPtr) > 0)
	{//valid
		acID				= authConnection->GetIntData("ID",accPtr);
		minutesPlayed		= authConnection->GetIntData("MinutesPlayed",accPtr);
		gameName            = authConnection->GetStringData("InGameName",accPtr);
		deafMute            = authConnection->GetIntData("muteDeaf",accPtr);
        if(authConnection->GetIntData("paidAc",accPtr) > 0)
			rcPaidCount = 1;

		if(m_allowOnlyPaidAccountsToConnect == TRUE && rcPaidCount == 0)
		{//account is unpaid (recurringdate < todats date) and the server is set to disallow connections if unpaid
           authConnection->Close(accPtr);
		   m_pendingLogons->LogonComplete(userName);
		   return 2;
		}
		
	}//end valid
	else
	{ 
        authConnection->Close(accPtr);
		m_pendingLogons->LogonComplete(userName);
		return 3;//cannot find account
	}
	authConnection->Close(accPtr);
	
	//Set Log On Flags
	temp = m_pendingLogons->m_checkLoggedOnDBStatusChange;
	temp.Replace("$USER",userName);
	temp.Replace("$LOGGEDON",m_cabLib.ConvToString(m_serverID));//set logged on flags
	authConnection->ExecQuery(temp);

    //Get Character Count-do we need this?
	temp = m_pendingLogons->m_getCharacterCount;
	temp.Replace("$USER",userName);
	temp.Replace("$GAMENAME",gameName);
	_RecordsetPtr ccreader = dbConnection->Query(temp);
	currentCharCount = dbConnection->GetCount(ccreader);
	dbConnection->Close(ccreader);
	//set permissions
	temp = m_pendingLogons->m_permissions;
	temp.Replace("$USER",userName);
	_RecordsetPtr preader = dbConnection->Query(temp);
	int permissionCount = dbConnection->GetCount(preader);
	CString tCompareLoc = "";
	for(int pLoop=0;pLoop<permissionCount;pLoop++)
	{//valid
		tCompareLoc = dbConnection->GetStringData("PERMISSIONNAME",preader);
        if(tCompareLoc.CompareNoCase("GAMEADMIN")==0)
		{
            admin = TRUE;
			m_eventLog->AddEvent("User ADMIN Permission:"+userName);
		}

		if(tCompareLoc.CompareNoCase("AISERVER")==0)
		{
            aiserver = TRUE;
			m_eventLog->AddEvent("User AISERVER Permission:"+userName);
		}

		if(tCompareLoc.CompareNoCase("GAMEMASTER")==0)
		{
            gm = TRUE;
			m_eventLog->AddEvent("User GAMEMASTER Permission:"+userName);
		}
	
		dbConnection->NextRecord(preader);
	}//end valid
    dbConnection->Close(preader);
		
	//paid status on account
	CString ipLocal;
	DPNID dpnidStore = 0;
    if(!m_pendingLogons->GetDataByUsername(userName,&dpnidStore,&ipLocal))
	{
		m_eventLog->AddEvent("USER DCED BEFORE LOGON COULD CREATE***:"+userName);
		//unset log on flags
		temp = m_pendingLogons->m_checkLoggedOnDBStatusChange;
	    temp.Replace("$USER",userName);
	    temp.Replace("$LOGGEDON","0");
	    authConnection->ExecQuery(temp);
		m_pendingLogons->LogonComplete(userName);
	    return 4;//disconnected in between logon and finalize
	}
	
    //add account
	CAccountObject * acPtr = new CAccountObject(userName,"");
	
    acPtr->StampLogOn();
	acPtr->m_currentCharRequestCount    = 0;
	acPtr->m_initializedHousingStates	= FALSE;
	acPtr->m_currentIP					= ipLocal;
	acPtr->m_securityIndexer			= 0;//reset indxer
	acPtr->m_logonTimeStamp				= GetTickCount();
	acPtr->m_administratorAccess		= admin;
	acPtr->m_aiServer					= aiserver;
	acPtr->m_gm							= gm;
	acPtr->m_accountNumber				= acID; 
	acPtr->m_timePlayedMinutes			= minutesPlayed;
	acPtr->m_charsCountDMMode			= currentCharCount;
	acPtr->m_currentlyPlayingGameName	= gameName;
	acPtr->m_housingBind                = houseExists;
	acPtr->m_deafMute                   = deafMute;
	acPtr->m_connectedTemp              = TRUE;
	acPtr->m_currentID					= dpnidStore;
	acPtr->m_loggedOn					= TRUE;
	acPtr->m_syncTrackingTime			= 0;
	acPtr->m_syncViolations				= 0;

	if(rcPaidCount > 0)
	{
        acPtr->m_expansionID = 1;//paid account
	}
	else
	{
        acPtr->m_expansionID = 0;//free account
	}
	
	m_eventLog->AddEvent("------ New Log for User Authenticaion for: " + userName + " ------");
	m_runtimeAccountDB->Add_safe(acPtr);
    
	DWORD processingTimeElapsed = GetTickCount()-logonProcessTimeStamp;
	m_eventLog->AddEvent("User Authenticated Complete:"+userName+" IP:"+ipLocal,processingTimeElapsed);
	//send menu call option

	// Tell the client it is in: one byte, category 3 = show the character menu.
	// (Was a DirectPlay SendTo on the long-gone IDirectPlay8Server - crashed here.)
	BYTE catagorie = 3;
	if(m_netHost)
		m_netHost->Send(dpnidStore,(const unsigned char*)&catagorie,1,true);
	else
		m_eventLog->AddEvent("FinalizeLogon: no network host to send menu call to:"+userName);

	m_pendingLogons->LogonComplete(userName);
    
	return 1;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
DWORD WINAPI LogOnProcThread(LPVOID lpParameter)
{
     CMultiplayerObj * classObj			= (CMultiplayerObj *)lpParameter;
     ConnectionStructureClass              connectionStructTemp;
	 ConnectionStructureClass              authConnectionStructTemp;
	 
	 if(authConnectionStructTemp.AquireFromCfgFile("authAccount.cfg")==false)
	 {
			classObj->m_eventLog->AddEvent("LogOnProcThread AquireFromCfgFile(authAccount.cfg) failed**");
			classObj->m_logonProcThreadCount--;
		return 0;
	 }
     
	 CDBConnectionSystem * authConnection = new CDBConnectionSystem(authConnectionStructTemp.m_sqlServerIP,
			                                                      authConnectionStructTemp.m_sqlServerCatalog,
																  authConnectionStructTemp.m_sqlServerUsername,
																  authConnectionStructTemp.m_sqlServerPassword,
																  classObj->m_eventLog);
    if(authConnection->m_connectionEstablished == TRUE)
	{//verify connection established
		classObj->m_eventLog->AddEvent("LogOnProcThread DB SYS connected auth**");
	}
	else
	{
		delete authConnection;
		authConnection=0;
		classObj->m_eventLog->AddEvent("LogOnProcThread DB SYS Failed to connect auth**");
		classObj->m_logonProcThreadCount--;
		return 0;
	}
	 
	 connectionStructTemp.AquireFromCfgFile("baseAccount.cfg");
     CDBConnectionSystem * dbConnection = new CDBConnectionSystem(connectionStructTemp.m_sqlServerIP,
			                                                      connectionStructTemp.m_sqlServerCatalog,
																  connectionStructTemp.m_sqlServerUsername,
																  connectionStructTemp.m_sqlServerPassword,
																  classObj->m_eventLog);
    if(dbConnection->m_connectionEstablished == TRUE)
	{//verify connection established
		classObj->m_eventLog->AddEvent("LogOnProcThread DB SYS connected");
	}
	else
	{
		delete authConnection;
		authConnection = 0;
		delete dbConnection;
		dbConnection=0;
		classObj->m_eventLog->AddEvent("LogOnProcThread DB SYS Failed to connect");
		classObj->m_logonProcThreadCount--;
		return 0;
	}


	 classObj->m_eventLog->AddEvent("LogOnProcThread-spinning up");
	 DPNID refDpnid						= 0;
	 CString userRef,passRef,ip;
	 int currentQueCount				= 0;
	 while(1)
	 {
	   EnterCriticalSection(&classObj->m_logonSection);
       if(classObj->m_shutDownLogonThreads)
	   {
          classObj->m_logonProcThreadCount--;
		  classObj->m_eventLog->AddEvent("LogOnProcThread Closing Down",(DWORD)classObj->m_logonProcThreadCount);
	      LeaveCriticalSection(&classObj->m_logonSection);
	      break;
	   }
	   LeaveCriticalSection(&classObj->m_logonSection);

		if(!classObj->m_pendingLogons->GetPtrThatsReadyForFinalize(&ip,&passRef,&userRef,&refDpnid,&currentQueCount))
		{
			Sleep(1000);
			continue;
		}
        //process
		 classObj->m_eventLog->AddEvent("FinalizeLogon() que count:",(DWORD)currentQueCount);
		//userRef = ptrLoc->m_username;
		//refDpnid = ptrLoc->m_dpnid;
		int returnCode = classObj->DBLogonProcessor(userRef,passRef,ip,authConnection);
		if(returnCode == 1)
		   returnCode = classObj->FinalizeLogon(userRef,dbConnection,authConnection);
		 
		switch(returnCode)
		{//handle failure
			default:
			{
				char replyCode = 'y';
                classObj->m_eventLog->AddEvent("FinalizeLogon() Unknown error disconnecting"+userRef);
				classObj->SendLoginReject(refDpnid,replyCode);
				break;
			}
			case 1:
			{//succes
				break;
			}
			case 2:
			{
				char replyCode = 'i';
				classObj->m_eventLog->AddEvent("FinalizeLogon() Account is Inactive:"+userRef);
				classObj->SendLoginReject(refDpnid,replyCode);

				break;
			}
			case 3:
			{
				char replyCode = 'u';
				classObj->m_eventLog->AddEvent("FinalizeLogon() disconnecting for unknown account name:"+userRef);
				classObj->SendLoginReject(refDpnid,replyCode);

				break;
			}
			case 4:
			{
				char replyCode = 'x';
				classObj->m_eventLog->AddEvent("FinalizeLogon() disconnected in between logon and finalize:"+userRef);
				classObj->SendLoginReject(refDpnid,replyCode);
				break;
			}
			case 5:
			{
				char replyCode = 'u';
				classObj->m_eventLog->AddEvent("FinalizeLogon() Invalid username Pass:"+userRef);
				classObj->SendLoginReject(refDpnid,replyCode);
				break;
			}
			case 6:
			{
				char replyCode = 'a';
				classObj->m_eventLog->AddEvent("FinalizeLogon() Already Logged on:"+userRef);
				classObj->SendLoginReject(refDpnid,replyCode);
				break;
			}
			case 7:
			{
				char replyCode = 'n';
				classObj->m_eventLog->AddEvent("FinalizeLogon() Server is in non-public mode:"+userRef);
				classObj->SendLoginReject(refDpnid,replyCode);
				break;
			}
			case 9:
			{
				char replyCode = 'b';
				classObj->m_eventLog->AddEvent("FinalizeLogon() Account is banned:"+userRef);
				classObj->SendLoginReject(refDpnid,replyCode);
				break;
			}
		}//end handle failure
	 }

	 delete dbConnection;
	 dbConnection=0;

	 delete authConnection;
	 authConnection = 0;

	 return 0;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
int CMultiplayerObj::SVRExporter()
{
	return 0;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CMultiplayerObj::UpdatePlayersOnline(int adjustAmount)
{
    EnterCriticalSection(&m_statusLock);
    m_currentPlayersOnline += adjustAmount;
	if(m_highestConstantPlayersOnline < m_currentPlayersOnline)
	   m_highestConstantPlayersOnline = m_currentPlayersOnline; 
	LeaveCriticalSection(&m_statusLock);
}
//-----------------------------------------------------------//
void CMultiplayerObj::AddPacketPerPass()
{
    EnterCriticalSection(&m_statusLock);
    m_lastCommunicationTime = GetTickCount();//client specific
	m_packetsRecievedPerPass++;
	LeaveCriticalSection(&m_statusLock);
}

//-----------------------------------------------------------//
// ENet receive pump : the ENet equivalent of DirectPlayMessageHandler.
// DirectPlay called that handler on its own hidden threads; ENet is
// poll-based, so NetPumpThread drives this on one dedicated thread and
// feeds the SAME m_packetQue the game already reads from.  The game above
// this layer is unchanged.
//
// NOTE (step 3c): the data receive path is fully reproduced here.  The
// login handshake (version / IP / spam validation) and the logoff/account
// cleanup are ported in step 3d - marked TODO(3d) below.
//-----------------------------------------------------------//
void CMultiplayerObj::PumpNetwork()
{
	if(!m_netHost)
		return;

	AuraNetEvent ev;
	while(m_netHost && m_netHost->PollEvent(ev, 0))
	{
		switch(ev.type)
		{
			case ANET_CONNECT:
			{
				// Transport-level connect only. On the server the player is NOT
				// authenticated yet - we wait for the login packet (first
				// ANET_RECEIVE below). On the client this event is consumed inside
				// AuraNetHost::Connect(), so it is not seen here.
				m_eventLog->AddEvent("ENet peer connected - awaiting login",(DWORD)ev.playerID);
				break;
			}

			case ANET_DISCONNECT:
			{
				if(m_netHost->IsServer())
					HandlePlayerLogoff(ev.playerID);
				else
				{//client: the server dropped us
					m_eventLog->AddEvent("ENet connection to server lost");
					SetConnectionStatus("Connection lost - Ctl+Q to quit", 2);
				}
				break;
			}

			case ANET_RECEIVE:
			{
				if(ev.size <= 0 || !ev.data)
				{
					m_eventLog->AddEvent("Invalid packet Size ",(DWORD)ev.size);
					break;
				}

				// Server: the FIRST packet from a peer we have no login context for
				// is the login handshake (the ENet stand-in for DirectPlay's
				// INDICATE_CONNECT user-connect-data). Validate it; do not queue it.
				if(m_netHost->IsServer() && m_netContext.find(ev.playerID) == m_netContext.end())
				{
					HandleLoginPacket(ev.playerID, ev.data, ev.size);
					break;
				}

				BOOL addPacket = TRUE;

				if(m_netHost->IsServer() && ev.size > m_maxPacketSizeNonAdmin)
				{//packet size too large - same security check DirectPlay did
					BOOL killUser = TRUE;
					m_runtimeAccountDB->Lock();
					CAccountObject * accountTrackPtr = (CAccountObject *) m_runtimeAccountDB->GetAccountByID(ev.playerID);
					if(accountTrackPtr)
					{
						if(accountTrackPtr->m_administratorAccess)
							killUser = FALSE;
					}
					else
						m_eventLog->AddEvent("Security Violation cannot find account in runtime DB:",(DWORD)ev.playerID);

					if(killUser)
					{
						m_netHost->DisconnectPlayer(ev.playerID);
						m_packetQue->PurgeAllByID(ev.playerID,FALSE);
						if(accountTrackPtr)
							m_eventLog->AddEvent(operator+("Security Violation paket too large: ",accountTrackPtr->m_accountName),(DWORD)ev.size);
						else
							m_eventLog->AddEvent("Security Violation ",(DWORD)ev.size);
						addPacket = FALSE;
					}
					m_runtimeAccountDB->Unlock();
				}

				if(addPacket)
				{
					m_packetQue->AddPacket((int)ev.size, (BYTE*)ev.data, ev.playerID, 0);
					AddPacketPerPass();
					if(m_pausePrimarySpinEvent)
						SetEvent(m_pausePrimarySpinEvent[RotateTrigger_safe()]);
				}
				break;
			}

			default:
				break;
		}
	}
}
//-----------------------------------------------------------//
// Server login gate - ENet port of DPN_MSGID_INDICATE_CONNECT (+ CREATE_PLAYER).
// DirectPlay validated username/password/version inside the connection
// handshake; ENet accepts the UDP connection first, so we validate the first
// packet instead. Same checks, same reject codes.
//-----------------------------------------------------------//
void CMultiplayerObj::SendLoginReject(AuraNetID id, char code)
{
	if(!m_netHost)
		return;
	unsigned char reply = (unsigned char)code;
	m_netHost->Send(id, &reply, 1, true);   // reliable, best-effort before drop
	m_netHost->DisconnectPlayer(id);
}
//-----------------------------------------------------------//
void CMultiplayerObj::HandleLoginPacket(AuraNetID id, const unsigned char * data, unsigned long size)
{
	if(size != sizeof(CONTEXT_VALIDATION))
	{
		m_eventLog->AddEvent("Login rejected - bad CONTEXT_VALIDATION size:",(DWORD)size);
		SendLoginReject(id,'v');
		return;
	}

	LPCONTEXT_VALIDATION incoming = (LPCONTEXT_VALIDATION)data;
	CString userName = incoming->username;
	CString password = incoming->password;

	if(incoming->versionInfo != m_currentVersion)
	{//out of date version
		m_eventLog->AddEvent("Invalid version"+userName);
		SendLoginReject(id,'v');
		return;
	}

	char ipbuf[64]; ipbuf[0]=0;
	if(!m_netHost->GetPlayerIP(id, ipbuf, sizeof(ipbuf)))
	{//cannot log IP
		SendLoginReject(id,'i');
		return;
	}
	CString ipAdress = ipbuf;

	if(m_accountDBMode == 1)
	{//db connection authenticate option
		int result = m_pendingLogons->Logon(userName,password,ipAdress);
		m_eventLog->LogByPriority("Pre Auth "+userName+" :",(DWORD)result,2);

		switch(result)
		{
			case 3:
				SendLoginReject(id,'i');
				m_eventLog->AddEvent("***IP SPAM***",userName,ipAdress);
				return;
			case 2:
				SendLoginReject(id,'u');
				m_eventLog->AddEvent("***USERNAME SPAM***",userName,ipAdress);
				return;
			default:
			{//success
				int strLen = (int)userName.GetLength();
				if(strLen > 24)
					strLen = 24;
				if(strLen > 0)
				{
					CONTEXT_VALIDATION * context = new CONTEXT_VALIDATION;
					ZeroMemory(context,sizeof(CONTEXT_VALIDATION));
					CopyMemory(context->username,userName.GetBuffer(),strLen);
					m_netContext[id] = context;
					m_pendingLogons->SetDPNID(id, context->username); // CREATE_PLAYER equivalent
					UpdatePlayersOnline(1);
					m_eventLog->AddEvent("Authenicated into session:",userName,ipAdress);
					return;
				}
				m_eventLog->AddEvent("***INVALID LOGON DATA***",userName,ipAdress);
				SendLoginReject(id,'u');
				return;
			}
		}
	}

	m_eventLog->AddEvent("General Rejection-possible out of date version");
	SendLoginReject(id,'v');
}
//-----------------------------------------------------------//
void CMultiplayerObj::HandlePlayerLogoff(AuraNetID id)
{
	UpdatePlayersOnline(-1);

	std::map<AuraNetID, LPCONTEXT_VALIDATION>::iterator it = m_netContext.find(id);
	LPCONTEXT_VALIDATION cntxValidation = (it != m_netContext.end()) ? it->second : 0;

	if(!cntxValidation)
	{
		m_eventLog->AddEvent("Cannot find account to log off NULL context:",(DWORD)id);
		return;
	}

	m_pendingLogons->LogonComplete(cntxValidation->username);

	CAccountObject * acPtr = (CAccountObject *) m_runtimeAccountDB->GetAccountByID_LOS(id);//account runtime locked here
	if(!acPtr)
	{
		m_eventLog->AddEvent("Cannot find account to log off DESTROY:",(DWORD)id);
		delete cntxValidation;
		m_netContext.erase(it);
		return;
	}

	CString userName = acPtr->m_accountName;
	acPtr->m_connectedTemp = FALSE;
	acPtr->m_timePlayedMinutes += (GetTickCount() - acPtr->m_logonTimeStamp)/60000;

	if(acPtr->m_aiServer == TRUE)
	{
		m_eventLog->AddEvent("AISERVER Logging off:",acPtr->m_accountName);
		m_runtimePlayersOnServer->Lock();
		while(m_runtimePlayersOnServer->RemoveByDPNID(acPtr->m_currentID))
		{
		}
		m_runtimePlayersOnServer->Unlock();
		m_characterPendingSaveSystem->AddCharacterToPendingList(NULL,1,userName,-1);
		m_runtimeAccountDB->Unlock();
		delete cntxValidation;
		m_netContext.erase(it);
		return;
	}

	CPlayerObject * currentPlayer = m_runtimePlayersOnServer->GetByDPNID_Alive_LOS(id);
	if(currentPlayer)
	{
		m_eventLog->AddEvent("cannot imediatly log off-so flag is set to timeout-:",acPtr->m_accountName);
		currentPlayer->m_justSpawned = FALSE;
		m_runtimePlayersOnServer->Unlock();
	}

	if(!currentPlayer)
	{//no live player is ok to que save point
		bool placedPendingData = false;
		if(acPtr->m_characterOnAccount)
		{
			m_characterPendingSaveSystem->AddCharacterToPendingList(acPtr->m_characterOnAccount,1,acPtr->m_accountName,-1);
			placedPendingData = true;
		}
		if(placedPendingData == false)
			m_characterPendingSaveSystem->AddCharacterToPendingList(NULL,1,userName,-1);
	}

	m_runtimeAccountDB->Unlock();

	delete cntxValidation;
	m_netContext.erase(it);
}
//-----------------------------------------------------------//
DWORD WINAPI NetPumpThread(LPVOID lpParameter)
{
	CMultiplayerObj * classObj = (CMultiplayerObj *)lpParameter;
	classObj->m_eventLog->AddEvent("NetPumpThread-spinning up");
	while(classObj->m_netPumpRun)
	{
		classObj->PumpNetwork();
		Sleep(1);
	}
	classObj->m_eventLog->AddEvent("NetPumpThread Closing Down");
	return 0;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
DWORD CMultiplayerObj::GetLastCommunicationTime()
{
    EnterCriticalSection(&m_statusLock);
    DWORD ret = m_lastCommunicationTime;//client specific
	LeaveCriticalSection(&m_statusLock);
	return ret;
}
//-----------------------------------------------------------//
void CMultiplayerObj::SetConnectionStatus(CString desc, int code)
{
    EnterCriticalSection(&m_connectionErrorSect);
    m_connectionError = code;
	m_connectionErrorCode = desc;
	LeaveCriticalSection(&m_connectionErrorSect);
}
//-----------------------------------------------------------//
void CMultiplayerObj::GetConnectionStatus(CString * desc, int * code)
{
    EnterCriticalSection(&m_connectionErrorSect);
	if(code)
       *code = m_connectionError;
	if(desc)
       *desc = m_connectionErrorCode;
	
	LeaveCriticalSection(&m_connectionErrorSect);
}
//-----------------------------------------------------------//
CString CMultiplayerObj::GetConnectionStatusDesc()
{
    EnterCriticalSection(&m_connectionErrorSect);
	CString ret = m_connectionErrorCode;
	LeaveCriticalSection(&m_connectionErrorSect);
	return ret;
}
//-----------------------------------------------------------//
int CMultiplayerObj::GetConnectionStatus()
{
    EnterCriticalSection(&m_connectionErrorSect);
	int ret = m_connectionError;
	LeaveCriticalSection(&m_connectionErrorSect);
	return ret;
}


IMPLEMENT_SERIAL(CMultiplayerObjList, CObList, 0)
