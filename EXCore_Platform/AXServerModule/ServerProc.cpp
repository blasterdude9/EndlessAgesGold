#include "StdAfx.h"
#include "serverproc.h"
#include "httpfiledownloader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI ZoneConnectionUpdater(LPVOID lpParameter);
DWORD WINAPI NetworkThreadLoop(LPVOID lpParameter);
DWORD WINAPI NetworkLoggingLoop(LPVOID lpParameter);
DWORD WINAPI NetworkCommunicationThread(LPVOID lpParameter);
DWORD WINAPI MiscDataProcessor(LPVOID lpParameter);
DWORD WINAPI CharacterAquisition(LPVOID lpParameter);
DWORD WINAPI CharacterDataWriter(LPVOID lpParameter);
DWORD WINAPI NodeTracker(LPVOID lpParameter);
DWORD WINAPI SpawnGeneratorWorker(LPVOID lpParameter);
DWORD WINAPI NPC_ServerItemWorker(LPVOID lpParameter);
DWORD WINAPI ServerTimeOutWorker(LPVOID lpParameter);
DWORD WINAPI ServerClanWorker(LPVOID lpParameter);

CServerProc::CServerProc(void)
{
	
	
    m_serverSystem			= new CServerMain();
	m_charAquisitionThreadCount			= 0;
	m_miscDataProcessorCount            = 0;
	m_charWritterThreadCount            = 0;
	m_zoneUpdaterThreadHandle           = 0;
	m_zoneUpdaterThreadID               = 0;
    m_nodeStatusUpdaterThreadHandle     = 0;
	m_nodeStatusUpdaterThreadID         = 0;
	m_lgThreadHandle					= NULL;
	m_commThreadHandle                  = NULL;
	m_spawnGenWorkerThreadHandle		= 0;
	m_spawnGenWorkerThreadID			= 0;

	m_unifiedCommerceThreadCount		= 1;
	m_unifiedCommerceThreadID			= NULL;
	m_unifiedCommerceThreadHandle		= NULL;

	//m_interactSectionRef    = m_serverSystem->GetInteractSection();
	m_charWritterThreadHandle = NULL;
	// Everything ShutDownServer() touches must be valid even if StartServer()
	// fails below (e.g. the UDP port is already taken by another game server) -
	// otherwise pressing Shutdown crashed on these never-assigned pointers.
	m_axThreadHandleReference			= NULL;
	m_axThreadThreadID					= NULL;
	m_charWritterThreadID				= NULL;
	m_miscDataProccesorThreadHandle		= NULL;
	m_miscDataProccesorThreadID			= NULL;
	m_charAquisitionThreadHandle		= NULL;
	m_charAquisitionThreadID			= NULL;
	m_ServerItemWorkerThreadHandle		= NULL;
	m_ServerTimeoutThreadHandle			= NULL;
	m_ServerClanWorkerThreadHandle		= NULL;
	if(!m_serverSystem->StartServer())//load settings are here
	    return;
	m_serverSystem->m_sharedInterface->m_netComObject->m_eventLog->EnableCriticalSection();

	//primary processing threads
	m_serverSystem->m_sharedInterface->m_netComObject->InitSpinLocks(m_serverSystem->m_sharedInterface->m_primaryProcessingThreadCount);
	m_axThreadHandleReference           = new HANDLE[m_serverSystem->m_sharedInterface->m_primaryProcessingThreadCount];
	m_axThreadThreadID					= new DWORD[m_serverSystem->m_sharedInterface->m_primaryProcessingThreadCount];
    for(int axLoop=0;axLoop<m_serverSystem->m_sharedInterface->m_primaryProcessingThreadCount;axLoop++)
	{
		m_axThreadHandleReference[axLoop] = CreateThread(NULL,0,NetworkThreadLoop,m_serverSystem,0,&m_axThreadThreadID[axLoop]);
	    m_serverSystem->m_serverBaseThreadCount++;
	}
	m_serverSystem->m_sharedInterface->m_eventLog->LogByPriority("primaryProcessingThreadCount:",(DWORD)m_serverSystem->m_sharedInterface->m_primaryProcessingThreadCount,2);

	DWORD m_lgThreadID, commThreadID; 
    m_lgThreadHandle					= CreateThread(NULL,0,NetworkLoggingLoop,m_serverSystem,0,&m_lgThreadID);
	m_serverSystem->m_serverBaseThreadCount++;
	m_commThreadHandle					= CreateThread(NULL,0,NetworkCommunicationThread,m_serverSystem,0,&commThreadID);
	m_serverSystem->m_serverBaseThreadCount++;

	//code for thread req
	m_charAquisitionThreadCount			= 5;
	m_miscDataProcessorCount            = 5;
	m_charWritterThreadCount            = 5;

	//m_shutDownThreadCount   = 0;

	if(m_charWritterThreadCount > 0)
	{ 
       m_charWritterThreadID				= new DWORD[m_charWritterThreadCount];
       m_charWritterThreadHandle			= new HANDLE[m_charWritterThreadCount];

	   for(int wLoop = 0;wLoop < m_charWritterThreadCount;wLoop++)
	   {
	       m_charWritterThreadHandle[wLoop]		= CreateThread(NULL,0,CharacterDataWriter,m_serverSystem,0,&m_charWritterThreadID[wLoop]);
		   m_serverSystem->m_serverBaseThreadCount++;
	   }
	}

	if(m_miscDataProcessorCount > 0)
	{ 
       m_miscDataProccesorThreadID			= new DWORD[m_miscDataProcessorCount];
       m_miscDataProccesorThreadHandle		= new HANDLE[m_miscDataProcessorCount];

	   for(int wLoop = 0;wLoop < m_miscDataProcessorCount;wLoop++)
	   {
	       m_miscDataProccesorThreadHandle[wLoop]		= CreateThread(NULL,0,MiscDataProcessor,m_serverSystem,0,&m_miscDataProccesorThreadID[wLoop]);
		   m_serverSystem->m_serverBaseThreadCount++;
	   }
	}

	if(m_charAquisitionThreadCount > 0)
	{ 
       m_charAquisitionThreadID				= new DWORD[m_charAquisitionThreadCount];
       m_charAquisitionThreadHandle			= new HANDLE[m_charAquisitionThreadCount];

	   for(int wLoop = 0;wLoop < m_charAquisitionThreadCount;wLoop++)
	   {
	       m_charAquisitionThreadHandle[wLoop]		= CreateThread(NULL,0,CharacterAquisition,m_serverSystem,0,&m_charAquisitionThreadID[wLoop]);
		   m_serverSystem->m_serverBaseThreadCount++;
	   }
	}

	if(m_unifiedCommerceThreadCount > 0)
	{//enabled
		m_unifiedCommerceThreadID				= new DWORD[m_unifiedCommerceThreadCount];
		m_unifiedCommerceThreadHandle			= new HANDLE[m_unifiedCommerceThreadCount];

		for(int wLoop = 0;wLoop < m_unifiedCommerceThreadCount;wLoop++)
		{
			m_unifiedCommerceThreadHandle[wLoop]		= CreateThread(NULL,0,UnifiedCommerceWorker,m_serverSystem,0,&m_unifiedCommerceThreadID[wLoop]);
			m_serverSystem->m_serverBaseThreadCount++;
		}
	}

	m_zoneUpdaterThreadHandle = CreateThread(NULL,0,ZoneConnectionUpdater,m_serverSystem,0,&m_zoneUpdaterThreadID);
	m_serverSystem->m_serverBaseThreadCount++;

    m_nodeStatusUpdaterThreadHandle = CreateThread(NULL,0,NodeTracker,m_serverSystem,0,&m_nodeStatusUpdaterThreadID);
	m_serverSystem->m_serverBaseThreadCount++;

	m_spawnGenWorkerThreadHandle = CreateThread(NULL,0,SpawnGeneratorWorker,m_serverSystem,0,&m_spawnGenWorkerThreadID);
	m_serverSystem->m_serverBaseThreadCount++;

	m_ServerItemWorkerThreadHandle = CreateThread(NULL,0,NPC_ServerItemWorker,m_serverSystem,0,&m_ServerItemWorkerThreadID);
	m_serverSystem->m_serverBaseThreadCount++;

	m_ServerTimeoutThreadHandle = CreateThread(NULL,0,ServerTimeOutWorker,m_serverSystem,0,&m_ServerTimeoutThreadID);
	m_serverSystem->m_serverBaseThreadCount++;

	m_ServerClanWorkerThreadHandle = CreateThread(NULL,0,ServerClanWorker,m_serverSystem,0,&m_ServerClanWorkerThreadID);
	m_serverSystem->m_serverBaseThreadCount++;

}

void CServerProc::ShutDownServer()
{
	if(!m_serverSystem)   // already shut down
		return;
    EnterCriticalSection( &m_serverSystem->m_interactSection);
	m_serverSystem->m_systemShutDown = TRUE;
	LeaveCriticalSection( &m_serverSystem->m_interactSection);
    
	DWORD stampMaxStamp = GetTickCount();
    while(1)
	{
		EnterCriticalSection( &m_serverSystem->m_interactSection);
		if(m_serverSystem->m_shutDownThreadCount >= m_serverSystem->m_serverBaseThreadCount || (GetTickCount()-stampMaxStamp) > 20000)
		{

		   LeaveCriticalSection( &m_serverSystem->m_interactSection);
           break;
		}  
        LeaveCriticalSection( &m_serverSystem->m_interactSection);
		Sleep(50);
	}
	
	EnterCriticalSection( &m_serverSystem->m_interactSection);
	if(m_lgThreadHandle)
	     CloseHandle(m_lgThreadHandle);
	
   
	if(m_axThreadHandleReference && m_serverSystem->m_sharedInterface)
	for(int axLoop=0;axLoop<m_serverSystem->m_sharedInterface->m_primaryProcessingThreadCount;axLoop++)
	{
		if(m_axThreadHandleReference[axLoop])
			CloseHandle(m_axThreadHandleReference[axLoop]);
	}
    if(m_axThreadHandleReference)
	   delete [] m_axThreadHandleReference;
	m_axThreadHandleReference = NULL;

	if(m_axThreadThreadID)
	   delete [] m_axThreadThreadID;
	m_axThreadThreadID = NULL;
	
	if(m_commThreadHandle)
		CloseHandle(m_commThreadHandle);

    if(m_zoneUpdaterThreadHandle)
		CloseHandle(m_zoneUpdaterThreadHandle);

	if(m_nodeStatusUpdaterThreadHandle)
		CloseHandle(m_nodeStatusUpdaterThreadHandle);

	if(m_spawnGenWorkerThreadHandle)
		CloseHandle(m_spawnGenWorkerThreadHandle);

	if(m_ServerItemWorkerThreadHandle)
		CloseHandle(m_ServerItemWorkerThreadHandle);

	if(m_ServerTimeoutThreadHandle)
		CloseHandle(m_ServerTimeoutThreadHandle);

	if(m_ServerClanWorkerThreadHandle)
		CloseHandle(m_ServerClanWorkerThreadHandle);


	if(m_charWritterThreadCount > 0)
	{ 
	   for(int wLoop = 0;wLoop < m_charWritterThreadCount;wLoop++)
	   {
		   if(m_charWritterThreadHandle[wLoop])
		      CloseHandle(m_charWritterThreadHandle[wLoop]);
	   }

	   if(m_charWritterThreadHandle)
	      delete [] m_charWritterThreadHandle;
	      m_charWritterThreadHandle = 0;

	   if(m_charWritterThreadID)
	      delete [] m_charWritterThreadID;
	      m_charWritterThreadID = 0;
	}

	if(m_miscDataProcessorCount > 0)
	{ 
	   for(int wLoop = 0;wLoop < m_miscDataProcessorCount;wLoop++)
	   {
		   if(m_miscDataProccesorThreadHandle[wLoop])
		      CloseHandle(m_miscDataProccesorThreadHandle[wLoop]);
	   }

	   if(m_miscDataProccesorThreadHandle)
	      delete [] m_miscDataProccesorThreadHandle;
	      m_miscDataProccesorThreadHandle = 0;
	   if(m_miscDataProccesorThreadID)
	      delete [] m_miscDataProccesorThreadID;
	      m_miscDataProccesorThreadID = 0;
	}

	if(m_charAquisitionThreadCount > 0)
	{ 
	   for(int wLoop = 0;wLoop < m_charAquisitionThreadCount;wLoop++)
	   {
		   if(m_charAquisitionThreadHandle[wLoop])
		   CloseHandle(m_charAquisitionThreadHandle[wLoop]);
	   }

	   if(m_charAquisitionThreadHandle)
	      delete [] m_charAquisitionThreadHandle;
	      m_charAquisitionThreadHandle = 0;
	   
       if(m_charAquisitionThreadID)
	      delete [] m_charAquisitionThreadID;
	      m_charAquisitionThreadID = 0;
	}

	if(m_unifiedCommerceThreadCount > 0 && m_unifiedCommerceThreadHandle)
	{ 
	   for(int wLoop = 0;wLoop < m_unifiedCommerceThreadCount;wLoop++)
	   {
		   if(m_unifiedCommerceThreadHandle[wLoop])
			CloseHandle(m_unifiedCommerceThreadHandle[wLoop]);
	   }

	   SafeDeleteArray(m_unifiedCommerceThreadHandle);
	   SafeDeleteArray(m_unifiedCommerceThreadID);
	}


	LeaveCriticalSection( &m_serverSystem->m_interactSection);

	delete m_serverSystem;//will delete critical section
	m_serverSystem = 0;

}

void CServerProc::GetServerStatus(AB_serverStatusLive * serverStatus)
{
    EnterCriticalSection( &m_serverSystem->m_statusQueryLock);
	CopyMemory(serverStatus,&m_serverSystem->m_serverStatus,sizeof(AB_serverStatusLive));
	LeaveCriticalSection( &m_serverSystem->m_statusQueryLock);
}

CServerProc::~CServerProc(void)
{
    ShutDownServer();
	
}

//-----------------------------------------------------------//
DWORD WINAPI NetworkThreadLoop(LPVOID lpParameter)
{	   
	    CServerMain * classObj = (CServerMain *)lpParameter; 
		if(FAILED(CoInitialize(NULL)))
		{
		   //AfxEndThread(0,TRUE);
           return 0;
		}
		//spawn log thread
		int processCell = 0;
        EnterCriticalSection( &classObj->m_interactSection);
        processCell = classObj->m_processRotation++;
		LeaveCriticalSection( &classObj->m_interactSection);
        
		//idle or process all messages with callback variables
        while(1)
		{
			EnterCriticalSection( &classObj->m_interactSection);
			if(classObj->m_systemShutDown)
			{
			   classObj->m_shutDownThreadCount++;
			   LeaveCriticalSection( &classObj->m_interactSection);
			   break;
			}
			LeaveCriticalSection( &classObj->m_interactSection);
			classObj->ServerCallback(processCell);
		}
		CoUninitialize();
  //AfxEndThread(0,TRUE);
return 1;
}
//-----------------------------------------------------------//
DWORD WINAPI SpawnGeneratorWorker(LPVOID lpParameter)
{	   
	    CServerMain * classObj = (CServerMain *)lpParameter; 
		if(FAILED(CoInitialize(NULL)))
		{
           return 0;
		}
        while(1)
		{
			EnterCriticalSection( &classObj->m_interactSection);
			if(classObj->m_systemShutDown)
			{
			   classObj->m_shutDownThreadCount++;
			   LeaveCriticalSection( &classObj->m_interactSection);
			   break;
			}
			LeaveCriticalSection( &classObj->m_interactSection);
			classObj->ServerItemGeneratorsCallback(classObj->m_sharedInterface->m_netComObject->m_runtimeItems,classObj->m_sharedInterface->m_serverItemGenerator,100);//thread safe
			Sleep(10);
		}
		CoUninitialize();
return 1;
}
//-----------------------------------------------------------//
DWORD WINAPI NPC_ServerItemWorker(LPVOID lpParameter)
{	   
        //does a load friendly call back check on runtimeitems/npcs ie:arena callbacks / despawn functionality / self triggered events
	    CServerMain * classObj = (CServerMain *)lpParameter; 
		if(FAILED(CoInitialize(NULL)))
		{
           return 0;
		}
		int idMax = 30000;
		int idCur = 0;
		int processSegment = 10;//will end up processing all npcs withing 45-60 seconds-with very little cpu usage
        while(1)
		{
			EnterCriticalSection( &classObj->m_interactSection);
			if(classObj->m_systemShutDown)
			{
			   classObj->m_shutDownThreadCount++;
			   LeaveCriticalSection( &classObj->m_interactSection);
			   break;
			}
			LeaveCriticalSection( &classObj->m_interactSection);
			
			for(int loop=0;loop<processSegment;loop++)
			{
				classObj->m_sharedInterface->m_netComObject->m_runtimeItems->Lock();
				CServerItemObj * svrPtr = classObj->m_sharedInterface->m_netComObject->m_runtimeItems->GetObjectByID(idCur);
				if(svrPtr)
				{
				   //check for quest specific callbacks like an arena or animation changes / auto despawns all kinds of interactive stuff can be done with callbacks
				   int trace = 0;
				   if(svrPtr->m_collectionQuestDB)
                   for(POSITION qPos = svrPtr->m_collectionQuestDB->GetHeadPosition();qPos != NULL;)
				   {//quest loop
					   CAICollectionQuestObj * quest = (CAICollectionQuestObj *)svrPtr->m_collectionQuestDB->GetNext(qPos);

						// IF this is an arena
						//	AND the timeout counter for the arena is enabled...
						if ((quest->m_instanceGenerateType == 2)
						&& (quest->m_arenaStartTimerStamp != 0))
						{
							// FORCE arena start if the elapsed time since starting the countdown is > the timeout value
							int iTimeElapsed = ::GetTickCount() - quest->m_arenaStartTimerStamp;
							if (iTimeElapsed >= quest->m_instanceGenerateParam4)
							{
								//	IF at least the min number of players is still registered for the arena...
								if (quest->DWMEM_getCount() >= quest->m_instanceGenerateParam3)
								{
									classObj->m_sharedInterface->m_eventLog->LogByPriority("NPC_ServerItemWorker Triggered arena start timeout:",quest->m_instanceGenerateParam4,2);

									// yes the arena must start now, however this will require us to unlock the runtime items to 
									// relock the runtime players to maintain order for avoidance of deadlocks
									// this code block is semi rare
									classObj->m_sharedInterface->m_netComObject->m_runtimeItems->Unlock();
									classObj->m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Lock();
									classObj->m_sharedInterface->m_netComObject->m_runtimeItems->Lock();

									CServerItemObj * reverifiedPtr = classObj->m_sharedInterface->m_netComObject->m_runtimeItems->GetObjectByID(idCur);

									//still here
									if (reverifiedPtr && reverifiedPtr->m_collectionQuestDB)
									{  
										POSITION qPosTemp = reverifiedPtr->m_collectionQuestDB->FindIndex(trace);
										if (qPosTemp)
										{
											CAICollectionQuestObj * questVerified = (CAICollectionQuestObj *)reverifiedPtr->m_collectionQuestDB->GetAt(qPosTemp);
											classObj->m_questSystems->StartArena_requiresRuntimeEntityLock(questVerified);
										}
									}
									classObj->m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Unlock();
									break;
								}
								// ELSE we DISABLE the timeout counter by setting it to 0
								else
								{
									// LOOP through the member collection and send a TXT_MSG
									//	to all players letting them know the arena countdown
									//	has been cancelled.
									if (quest->m_dwMem)
									{
										classObj->m_sharedInterface->m_eventLog->LogByPriority("NPC_ServerItemWorker Triggered arena start timeout RESET!",2);

										int iPlayerID = -1;
										CPlayerObject *pPlayer = NULL;
										CString strMessage("Not enough registered players for the arena to start. The countdown will begin again when more players have joined.");

										classObj->m_sharedInterface->m_netComObject->m_runtimeItems->Unlock();
										classObj->m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Lock();
										classObj->m_sharedInterface->m_netComObject->m_runtimeItems->Lock();
										for (int i=0; i<quest->m_dwMemCount; i++)
										{
											iPlayerID = quest->m_dwMem[i];
											if (iPlayerID > -1)
											{
												// RETRIEVE player by their handle ID
												pPlayer = classObj->m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandle(iPlayerID);
												if (pPlayer)
												{
													classObj->m_sharedInterface->SendTextMsg(
														DPNSEND_GUARANTEED,
														pPlayer->m_currentID,
														strMessage,
														0,
														pPlayer,
														0);
												}
											}
										}
										classObj->m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Unlock();
										break;
									}

									// DISABLE the arena timeout counter
									quest->m_arenaStartTimerStamp = 0;
								} // END disable timeout counter
							} // END timeout to force arena start
					   } // END if this is an arena
					   trace++;
				   }//end quest loop
				}
				classObj->m_sharedInterface->m_netComObject->m_runtimeItems->Unlock();
				idCur++;
				if(idCur > idMax)
				   idCur = 0;
				Sleep(0);
			}
			Sleep(15);
		}
		CoUninitialize();
return 1;
}
//-----------------------------------------------------------//
DWORD WINAPI ServerTimeOutWorker(LPVOID lpParameter)
{	   
        //does a load friendly call back check on runtimeitems/npcs ie:arena callbacks / despawn functionality / self triggered events
	    CServerMain * classObj = (CServerMain *)lpParameter; 
		if(FAILED(CoInitialize(NULL)))
		{
           return 0;
		}
		
		int processSegment = 5;//will end up processing all npcs withing 45-60 seconds-with very little cpu usage
        while(1)
		{
			EnterCriticalSection( &classObj->m_interactSection);
			if(classObj->m_systemShutDown)
			{
			   classObj->m_shutDownThreadCount++;
			   LeaveCriticalSection( &classObj->m_interactSection);
			   break;
			}
			LeaveCriticalSection( &classObj->m_interactSection);
			
			classObj->m_sharedInterface->ServerTimoutCallback(processSegment);
	        
			Sleep(5);
		}
		CoUninitialize();
return 1;
}

//-----------------------------------------------------------//
DWORD WINAPI ServerClanWorker(LPVOID lpParameter)
{	   
		CServerMain * classObj = (CServerMain *)lpParameter; 
		if(FAILED(CoInitialize(NULL)))
		{
           return 0;
		}

		CSharedNet * sNet = classObj->m_sharedInterface;

		CDBConnectionSystem * dbConnection = new CDBConnectionSystem(classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerIP,
			                                                         classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerCatalog,
																	 classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerUsername,
																	 classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerPassword,
																	 classObj->m_sharedInterface->m_eventLog);
        if(dbConnection->m_connectionEstablished == TRUE)
		{//verify connection established
		   
		   classObj->m_sharedInterface->m_eventLog->AddEvent("ServerClanWorker Thread connected and running");
		}
		else
		{
		   delete dbConnection;
		   dbConnection = 0;
		   classObj->m_sharedInterface->m_eventLog->AddEvent("ServerClanWorker Thread Failed to connect");
		   return 1;//leave because we have no basis to use the database
		}
		
		CAxClanCommand * cmd = NULL;
        while(1)
		{
			EnterCriticalSection( &classObj->m_interactSection);
			if(classObj->m_systemShutDown)
			{
			   classObj->m_shutDownThreadCount++;
			   LeaveCriticalSection( &classObj->m_interactSection);
			   break;
			}
			LeaveCriticalSection( &classObj->m_interactSection);
			
			cmd = classObj->m_sharedInterface->m_clanCommSys.GetNextCommand();
	        if(!cmd)
			{
				Sleep(500);
				continue;
			}

			switch(cmd->m_requestType)
			{
				case CLAN_CREATE:
				{
					BOOL result = sNet->m_clanCommSys.AddClanIfNone_safe(cmd->m_param1,sNet->m_cabLib.ConvToString(cmd->m_fromhandle),dbConnection);
					if(!result)
					{
						sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
						CPlayerObject * pObj = sNet->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandle(cmd->m_fromhandle);
						if(pObj)
							sNet->SendTextMsg(pObj->m_currentID,"This clan name already exists "+cmd->m_param1,pObj);
						sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
					}
					else
					{
						sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
						CPlayerObject * pObj = sNet->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandle(cmd->m_fromhandle);
						if(pObj)
						{
							sNet->SendTextMsg(pObj->m_currentID,"CLAN CREATED -"+cmd->m_param1,pObj);
							pObj->m_clanAbbrev = cmd->m_param1;
							pObj->ReassembleName(sNet);//reconstruct
							sNet->m_netComObject->m_runtimePlayersOnServer->ReIndexByClanHandle(pObj,cmd->m_fromhandle);
							sNet->SendClientScript(pObj,"UpdateCharName("+pObj->m_visName+")","");
							//sNet->m_eventLog->LogByPriority("ServerClanWorker CLAN_CREATE script: UpdateCharName("+pObj->m_visName+")",2);
						}
						else
							sNet->m_eventLog->LogByPriority("ServerClanWorker CLAN_CREATE cannot find playerObject",2);

						sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
					}

					break;
				}
				case CLAN_DELETE:
				{
					sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
					while(sNet->m_netComObject->m_runtimePlayersOnServer->ReIndexByClanHandle(cmd->m_fromhandle, -1))
					{
						sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
						Sleep(0);
						sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
					}
					sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
					
					sNet->m_clanCommSys.DeleteClan_safe(cmd->m_param1,dbConnection);
				break;
				}
				case CLAN_CHAT:
				{//creates a distributed chat
					sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
					CPlayerObject * speakerPtr = sNet->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandle(cmd->m_fromhandle);
					if(speakerPtr)
					{ 
						if(cmd->m_param1.GetLength() > 0)
						{
							CString compiledChat = speakerPtr->m_charName+": "+cmd->m_param1;
							CAxList * obj = sNet->m_netComObject->m_runtimePlayersOnServer->GetAllOnIndex(speakerPtr->m_clanHandle,"clan_handle");
							for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
							{
								 CPlayerObject * plObj = (CPlayerObject *)obj->GetNext_unsafe(&pos);
								 classObj->m_sharedInterface->SendTextMsg(plObj->m_currentID,compiledChat,plObj,1);
							}
						}
					}
					sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
				break;
				}
				case CLAN_REQUESTMEMBERS:
				{
					CString buildMembers = " <";//will have to use database for cross node support accessing the critical gm_accountBase
					
					sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
					CAxList * obj = sNet->m_netComObject->m_runtimePlayersOnServer->GetAllOnIndex(atoi(cmd->m_param1),"clan_handle");
					for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
					{
						 CPlayerObject * plObj = (CPlayerObject *)obj->GetNext_unsafe(&pos);
						 buildMembers += plObj->m_charName + ",";
					}
					sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
					
					if(buildMembers.GetLength() > 3)
					{
						buildMembers += ">";
						sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
						CPlayerObject * retPtr = sNet->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandle(cmd->m_fromhandle);
						if(retPtr)
							classObj->m_sharedInterface->SendTextMsg(retPtr->m_currentID,buildMembers,retPtr);
						sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
					}
				break;
				}
				case CLAN_REMOVEMEMBER:
				{					
					sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
					CPlayerObject * captainPtr = sNet->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandle(cmd->m_fromhandle);
					if(!captainPtr)
					{
						sNet->m_eventLog->LogByPriority("CLAN_REMOVEMEMBER failed to aquire captain/leader object",2);
						sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
						break;
					}
					CString clanIDStore = sNet->m_cabLib.ConvToString(captainPtr->m_clanHandle);

					//handle online player
					CPlayerObject * retPtr = sNet->m_netComObject->m_runtimePlayersOnServer->GetObjectByName(cmd->m_param1.GetBuffer());
					if(retPtr)
					{
					   if(retPtr->m_clanHandle == cmd->m_fromhandle)
					   {
							sNet->m_netComObject->m_runtimePlayersOnServer->ReIndexByClanHandle(retPtr, -1);
							retPtr->m_clanAbbrev = "";
							retPtr->ReassembleName(sNet);//reconstruct
							sNet->SendClientScript(retPtr,"UpdateCharName("+retPtr->m_visName+")","");
							classObj->m_sharedInterface->SendTextMsg(DPNSEND_GUARANTEED,captainPtr->m_currentID,"Player has been removed from the clan "+cmd->m_param1,0,captainPtr,11);
					   }
					   else//not on this clan
							classObj->m_sharedInterface->SendTextMsg(DPNSEND_GUARANTEED,captainPtr->m_currentID,"This player is not on your clan "+cmd->m_param1,0,captainPtr,11);
					   
					   sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
					   break;
					}
					sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
					
					//if the player is offline
					int result = sNet->m_clanCommSys.DeleteClanMember(clanIDStore,cmd->m_param1,dbConnection);

					sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
					captainPtr = sNet->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandle(cmd->m_fromhandle);
					if(captainPtr)
						switch(result)
						{
							case 1://success
								classObj->m_sharedInterface->SendTextMsg(DPNSEND_GUARANTEED,captainPtr->m_currentID,"Player has been removed from the clan:"+cmd->m_param1,0,captainPtr,11);
							break;
							case 0://cannot find player by that name
								classObj->m_sharedInterface->SendTextMsg(DPNSEND_GUARANTEED,captainPtr->m_currentID,"Cannot find player on this clan by that name:"+cmd->m_param1,0,captainPtr,11);
							break;
							case 2://wasnt on the clan to remove
								classObj->m_sharedInterface->SendTextMsg(DPNSEND_GUARANTEED,captainPtr->m_currentID,"Player is not on the clan:"+cmd->m_param1,0,captainPtr,11);
							break;
						}
					sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();


					break;
				}
				default:
				{


				break;
				}
			}

			delete cmd;
			cmd = 0;
			Sleep(50);
		}

		delete dbConnection;
		dbConnection = 0;

		CoUninitialize();
return 1;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
DWORD WINAPI NetworkLoggingLoop(LPVOID lpParameter)
{	   
	    CServerMain * classObj = (CServerMain *)lpParameter; 
		if(FAILED(CoInitialize(NULL)))
		{
           return 0;
		}

		CServerEventList * eventLogRef = classObj->m_sharedInterface->m_netComObject->m_eventLog;
        
		//idle or process all messages with callback variables
        while(1)
		{   
			DWORD sectionStamp = GetTickCount();
			//classObj->m_sharedInterface->m_dbConnection->Reconnect(sectionStamp);
			
            //manage shutdown proc
			if(classObj->m_shuttingDown)
			{
	           if((sectionStamp - classObj->m_sutDownStartStamp) > (classObj->m_shutDownNotificationDuration-180000))
			   {//3 minutes before save disconnect everyone
				   if(classObj->m_sharedInterface->m_netComObject->m_runtimeAccountDB)
				      classObj->m_sharedInterface->m_netComObject->m_runtimeAccountDB->DisconnectAll_safe(classObj->m_sharedInterface->m_netComObject->m_netHost);
			   }//end 3 minutes before save disconnect everyone
				
			   if((sectionStamp - classObj->m_sutDownStartStamp) > classObj->m_shutDownNotificationDuration)
			   {
				   //classObj->m_sharedInterface->m_netComObject->SaveToFile("Default.svr");
				   classObj->m_shuttingDown = 0;
				   if(classObj->m_rebootOnShutdown)
				      classObj->RebootServer();
			   }
			}
			EnterCriticalSection( &classObj->m_interactSection);
			//end manage shutdown proc
			if(classObj->m_systemShutDown)
			{
			   classObj->m_shutDownThreadCount++;
               LeaveCriticalSection( &classObj->m_interactSection);
			   break;
			}
			LeaveCriticalSection( &classObj->m_interactSection);

			BOOL kingdomOwnerChanged = FALSE;
			CString kingdomThatChangedName = "";
			CString clanThatOwnsTheKingdomNow = "";

            for(int cowloop=0;cowloop<50;cowloop++)
			{
				CString temp = classObj->m_sharedInterface->SanitizeFormatIn(eventLogRef->GetOldestEntryMarkIt());
			    if(temp.GetLength() > 0)
		           classObj->m_sharedInterface->m_dbConnection->ExecQuery("Insert into tblServerLog(logMsg) values('"+temp+"')");//write log to database
				else
				   break;
				Sleep(50);		       
			}


			EnterCriticalSection( &classObj->m_sharedInterface->m_netComObject->m_statusLock);
			classObj->m_serverStatus.m_playersOnline                    = classObj->m_sharedInterface->m_netComObject->m_currentPlayersOnline;//PLAYERCOUNT
	        classObj->m_serverStatus.m_maxSpikePlayersOnline            = classObj->m_sharedInterface->m_netComObject->m_highestConstantPlayersOnline;
	        classObj->m_serverStatus.m_msgsOutPerSecond					= classObj->m_sharedInterface->m_netComObject->m_packetsOutPerPass;
	        classObj->m_serverStatus.m_msgsInPerSecond					= classObj->m_sharedInterface->m_netComObject->m_packetsRecievedPerPass;
	        classObj->m_serverStatus.m_bandwidthOutgoingPerSecond		= classObj->m_sharedInterface->m_netComObject->m_totalBandwidthOutGoing;
			classObj->m_serverStatus.m_bandwidthIncommingPerSecond		= classObj->m_sharedInterface->m_netComObject->GetTotalBandwidthIncomming_safe();
			classObj->m_serverStatus.m_totalBandwidthIn					+= classObj->m_serverStatus.m_bandwidthIncommingPerSecond;
	        classObj->m_serverStatus.m_totalBandwidthOut				+= classObj->m_sharedInterface->m_netComObject->m_totalBandwidthOutGoing;
	        classObj->m_serverStatus.m_totalBandwidthTotal				+= classObj->m_serverStatus.m_bandwidthIncommingPerSecond + classObj->m_sharedInterface->m_netComObject->m_totalBandwidthOutGoing;
			

			
			classObj->m_sharedInterface->m_netComObject->ZeroTotalBandwidthIncomming_safe();
		    classObj->m_sharedInterface->m_netComObject->m_totalBandwidthOutGoing=0;
		    classObj->m_sharedInterface->m_netComObject->m_packetsOutPerPass=0;
            classObj->m_sharedInterface->m_netComObject->m_packetsRecievedPerPass=0;
			
            LeaveCriticalSection( &classObj->m_sharedInterface->m_netComObject->m_statusLock);

			

			long divPsec = 15;
			DWORD durationToWait = 15000;//1000-(GetTickCount()-sectionStamp);
            CString dateUpdater = "lastUpdated = GETDATE(),";
			classObj->m_sharedInterface->m_dbConnection->ExecQuery(
				"Update tblMainServerStatus set CurrentConnections = '" + 
				 classObj->m_sharedInterface->m_cabLib.ConvertLongToString(classObj->m_serverStatus.m_playersOnline)+"'," +
                "ConnectionsPeak = '" + 
				classObj->m_sharedInterface->m_cabLib.ConvertLongToString(classObj->m_serverStatus.m_maxSpikePlayersOnline)+"'," +
				"msgSendPerSecond = '" + 
				classObj->m_sharedInterface->m_cabLib.ConvertLongToString(classObj->m_serverStatus.m_msgsOutPerSecond/divPsec)+"'," +
				"msgReceivedPerSecond = '" + 
				classObj->m_sharedInterface->m_cabLib.ConvertLongToString(classObj->m_serverStatus.m_msgsInPerSecond/divPsec)+"'," +
				"bandwidthOutgoingPerSecond = '" + 
				classObj->m_sharedInterface->m_cabLib.ConvertLongToString(classObj->m_serverStatus.m_bandwidthOutgoingPerSecond/divPsec)+"'," +
				"bandwidthOutTotal = '" + 
				classObj->m_sharedInterface->m_cabLib.ConvertLongToString(classObj->m_serverStatus.m_totalBandwidthOut/1000000)+"'," +
				"bandwidthInTotal = '" + 
				classObj->m_sharedInterface->m_cabLib.ConvertLongToString(classObj->m_serverStatus.m_totalBandwidthIn/1000000)+"'," +
				"bandwidthIncommingPerSecond = '" + 
				classObj->m_sharedInterface->m_cabLib.ConvertLongToString(classObj->m_serverStatus.m_bandwidthIncommingPerSecond/divPsec)+"'," +
				dateUpdater + 
				"bandwidthTotal = '" + 
				classObj->m_sharedInterface->m_cabLib.ConvertLongToString(classObj->m_serverStatus.m_totalBandwidthTotal/1000000)+"';");


			
			
			Sleep(durationToWait);
		}
		CoUninitialize();
  //AfxEndThread(0,TRUE);
return 1;
}
//-----------------------------------------------------------//
DWORD WINAPI NetworkCommunicationThread(LPVOID lpParameter)
{	   
	    CServerMain * classObj = (CServerMain *)lpParameter; 
		if(FAILED(CoInitialize(NULL)))
		{
           return 0;
		}

		CDBConnectionSystem * hubConnection = new CDBConnectionSystem(	classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerIP,
																		classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerCatalog,
																		classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerUsername,
																		classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerPassword,
																		classObj->m_sharedInterface->m_eventLog);
        if(hubConnection->m_connectionEstablished != TRUE)
		{//verify connection established
			classObj->m_sharedInterface->m_eventLog->LogByPriority("Logon proc base HUB connection failed:",1);

			delete hubConnection;
			hubConnection = 0;
		}



		//idle for databe communication
        while(1)
		{
			EnterCriticalSection( &classObj->m_interactSection);
			if(classObj->m_systemShutDown)
			{
			   classObj->m_shutDownThreadCount++;
			   LeaveCriticalSection( &classObj->m_interactSection);
			   break;
			}
			LeaveCriticalSection( &classObj->m_interactSection);

			_RecordsetPtr recordSetTemp = NULL;
			_RecordsetPtr recordSet = classObj->m_sharedInterface->m_dbConnection->Query("select id, CommandType, peram1, peram2, peram3, peram4, peram5, peram6 from tblCommandsToServer");
			if(recordSet)
			{//valid record set
               if(classObj->m_sharedInterface->m_dbConnection->GetCount(recordSet) > 0)
			   {//process requests
                   

                   CString idReference = classObj->m_sharedInterface->m_dbConnection->GetStringData("id",recordSet);
				   int commandTypeRef  = classObj->m_sharedInterface->m_dbConnection->GetIntData("CommandType",recordSet);
				   CString commandTypeRefString  = classObj->m_sharedInterface->m_dbConnection->GetStringData("CommandType",recordSet);
				   CString peram1Ref = classObj->m_sharedInterface->m_dbConnection->GetStringData("peram1",recordSet);
				   CString peram2Ref = classObj->m_sharedInterface->m_dbConnection->GetStringData("peram2",recordSet);
				   int peram2INT     = classObj->m_sharedInterface->m_dbConnection->GetIntData("peram2",recordSet);
				   CString peram3Ref = classObj->m_sharedInterface->m_dbConnection->GetStringData("peram3",recordSet);
				   int peram3IntRef  = classObj->m_sharedInterface->m_dbConnection->GetIntData("peram3",recordSet);
				   CString peram4Ref = classObj->m_sharedInterface->m_dbConnection->GetStringData("peram4",recordSet);
				   int peram4IntRef = classObj->m_sharedInterface->m_dbConnection->GetIntData("peram4",recordSet);
				   CString peram5Ref = classObj->m_sharedInterface->m_dbConnection->GetStringData("peram5",recordSet);
				   CString peram6Ref = classObj->m_sharedInterface->m_dbConnection->GetStringData("peram6",recordSet);
				   classObj->m_sharedInterface->m_dbConnection->Close(recordSet);
                   BOOL tempRegisterBool = FALSE;
				   CAccountObjectList * accountDBonline = classObj->m_sharedInterface->m_netComObject->m_runtimeAccountDB;
				   D3DVECTOR positionNew;
				   ZeroMemory(&positionNew,sizeof(D3DVECTOR));
				   int worldID = 0;
				   int miscInt = 0;
				   _RecordsetPtr recordSet = NULL;
				   classObj->m_sharedInterface->m_eventLog->LogByPriority("NetworkCommunicationThread() type:",(DWORD)commandTypeRef,2);
                   switch(commandTypeRef)
		           {//visual type switch
		              case 1://request account information by username (sends character names)
						     //******replaced by database backend
                      break;
					  case 2://request player information
                             //******replaced by database backend
					  break;
					  case 3://add item w quality
                             //******replaced by database backend
					  break;
					  case 4://request item db from server
						    
					  break;
					  case 5://name change
						  //******replaced by database backend
                          
					  break;
					  case 6://reposition
                          //******replaced by database backend
					  break;
					  case 7://delete bank item
						  //******replaced by database backend
					  break;
					  case 8://que system restart
						  //add locking 
						  
						  classObj->m_shuttingDown						= 1;
						  classObj->m_sharedInterface->m_netComObject->m_serverMode = 1;//admin logon only
	                      classObj->m_rebootOnShutdown					= 1;
	                      classObj->m_shutDownNotification                = "***Server will restart shortly***";
	                      classObj->m_sutDownStartStamp                   = GetTickCount();
	                      classObj->m_shutDownNotificationDuration        = 600000;//milliseconds
						  classObj->m_sharedInterface->BroadcastMessage(0,
						                                                classObj->m_shutDownNotification,
					              	                                    positionNew,
						                                                -1,
						                                                0);
						  classObj->m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Restart Proceedure started");
						  classObj->AddResponseCommand(commandTypeRefString,"System restart in process","","","","completed",peram6Ref);
					  break;
					  case 9://admin mode toggle
                          //add locking
						  
						  if(classObj->m_sharedInterface->m_netComObject->m_serverMode == 1)
						     classObj->m_sharedInterface->m_netComObject->m_serverMode = 0;
						  else
						     classObj->m_sharedInterface->m_netComObject->m_serverMode = 1;

						  miscInt = classObj->m_sharedInterface->m_netComObject->m_serverMode;
						 
						  if(miscInt == 1)
						     classObj->AddResponseCommand(commandTypeRefString,"Admin mode on..","","","","completed",peram6Ref);
						  else
						     classObj->AddResponseCommand(commandTypeRefString,"Admin mode off..","","","","completed",peram6Ref);
					  break;
					  case 10://manual save
                         
					  break;
					  case 11://stat change
						  //******replaced by database backend
					  break;//end stat change
					  case 12://skill change
						  //******replaced by database backend
					  break;//end skill change
					  case 14://SQL update (spawns thread)

			
				      break;
					  case 15://Disconnect all
						    if(classObj->m_sharedInterface->m_netComObject->m_runtimeAccountDB)
				               classObj->m_sharedInterface->m_netComObject->m_runtimeAccountDB->DisconnectAll_safe(classObj->m_sharedInterface->m_netComObject->m_netHost);
					  break;//end Disconnect all
					  case 16://disconnect him if online
					      tempRegisterBool = FALSE;
						  if(1)
						  {
							  CAccountObject * acPtr = classObj->m_sharedInterface->m_netComObject->m_runtimeAccountDB->GetAccountByUsername_LOS(peram1Ref.GetBuffer());
							  if(acPtr)
							  {
								 classObj->m_sharedInterface->m_netComObject->Disconnect(acPtr->m_currentID);
								 classObj->m_sharedInterface->m_netComObject->m_runtimeAccountDB->Unlock();
							  }
						  }
					  break;//end Ban player and disconnect him if online
					  case 17://Regular Maintnence
						 
					  break;// End Regular Maintnence
					  case 18://Reclaim Maintnence
						 
					  break;// End Regular Maintnence
					  case 19://Maintnence Light
						 
					  break;// End Regular Maintnence Light
					  case 21://gm toggle
						    if(classObj->m_sharedInterface->m_netComObject->m_runtimeAccountDB->ToggleGMStatus_safe(peram1Ref)==true)
							   classObj->AddResponseCommand(commandTypeRefString,"Status Change Processed","","","","completed",peram6Ref);
							else
							   classObj->AddResponseCommand(commandTypeRefString,"Status Change Failed","","","","completed",peram6Ref); 
					  break;// End GM toggle
					  case 22://update ign entity
					  {
								//refresh all generators from sql
								classObj->m_sharedInterface->m_serverItemGenerator->GetFromSQL(classObj->m_sharedInterface->m_eventLog,classObj->m_sharedInterface->m_itemManager,peram2INT,classObj->m_sharedInterface->m_netComObject->m_currentWorldsUsed);
								//clear live so they can regenerate
								int runtimeEntitiesRemoved = classObj->m_sharedInterface->m_netComObject->m_runtimeItems->DeleteAllByRefSpawnID(peram2INT);//should clear all related instances
						        classObj->m_sharedInterface->m_eventLog->LogByPriority("NPC UPDATE refSpawnId:",(DWORD)peram2INT,2);
								classObj->m_sharedInterface->m_eventLog->LogByPriority("NPC UPDATE runtimeEntitiesRemoved:",(DWORD)runtimeEntitiesRemoved,2);
					  		
					     break;// End update ign entity
					  }
					  case 23://get entity location and world channel
						   
                            if(1)
							{
								CString posX,posY,posZ,WorldChannel;
								bool foundPlayer = false;
								CPlayerObject * plObj = classObj->m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->GetObjectByName_LOS(peram1Ref.GetBuffer());	
								if(plObj)
								{
                                   posX = classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(plObj->m_3dPosition.x);
								   posY = classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(plObj->m_3dPosition.y);
								   posZ = classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(plObj->m_3dPosition.z);
								   
                                   int stencilChannel = classObj->m_sharedInterface->m_netComObject->m_currentWorldsUsed->GetStencilChannelByLiveChannel_safe(plObj->m_currentWorldID);
								   WorldChannel = classObj->m_sharedInterface->m_cabLib.ConvToString(stencilChannel);
								   
								   foundPlayer = true;
								   classObj->m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Unlock();
								}

								if(!foundPlayer)
								    classObj->AddResponseCommand(commandTypeRefString,"FAILED","","","","completed",peram6Ref);
								else
									classObj->AddResponseCommand(commandTypeRefString,posX,posY,posZ,WorldChannel,"completed",peram6Ref);

							}
					  break;//end get entity location and world channel
					  case 24://Migrate data to database
						    //if(classObj->m_sharedInterface->m_netComObject->m_accountDBMode == 0)
                               classObj->m_sharedInterface->m_netComObject->SVRExporter();
					  break;//end Migrate data to database
					  case 25://update (STREAM) current server runtime players and ids to DB 
						   peram1Ref = "INSERT INTO tblCurrentPlayerDmp (CHARNAME, NETID, CHANNEL, ALIVE, TYPE) Values('$CHARNAME','$NETID','$CHANNEL','$ALIVE','$TYPE')";
						   classObj->m_sharedInterface->m_dbConnection->ExecQuery("delete from tblCurrentPlayerDmp");//clear list
						   for(int inetid = 0;inetid < 30000;inetid++)
						   {//we use this method even though it will take a bit longer because we can async load this to the db with little cpu usage-net ids are maxed at 30000
								  CPlayerObject * cplayerPtr = (CPlayerObject *) classObj->m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(inetid);
								  if(cplayerPtr)
								  {
									  peram2Ref = peram1Ref;
									  peram2Ref.Replace("$CHARNAME",cplayerPtr->m_charName);
									  peram2Ref.Replace("$NETID",classObj->m_sharedInterface->m_cabLib.ConvToString(cplayerPtr->m_networkAssignedID));
									  peram2Ref.Replace("$CHANNEL",classObj->m_sharedInterface->m_cabLib.ConvToString(cplayerPtr->m_currentWorldID));
									  peram2Ref.Replace("$ALIVE",classObj->m_sharedInterface->m_cabLib.ConvToString(cplayerPtr->m_alive));
									  
									  if(cplayerPtr->m_aiControlled)
										 peram2Ref.Replace("$TYPE","1");//AI
									  else
										 peram2Ref.Replace("$TYPE","0");//PC
                     
									  classObj->m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Unlock();
									  classObj->m_sharedInterface->m_dbConnection->ExecQuery(peram2Ref);
								  }
                              Sleep(0);
						   }
					  break;//end update (STREAM) current server runtime players and ids to DB 
					  case 26:
					  {
							int scriptCount = classObj->m_sharedInterface->m_serverScripts->AquireFromDatabase(classObj->m_sharedInterface->m_dbConnection);
							classObj->m_sharedInterface->m_eventLog->AddEvent("Server Scripts Reloaded:",(DWORD)scriptCount);
							break;
					  }
					  case 27://update dynamic item live
					  {
						  
						  if(hubConnection)
						  {
							bool result = classObj->m_sharedInterface->m_itemManager->AquireByGlid(peram1Ref, classObj->m_sharedInterface->m_elementDB, NULL, hubConnection);

							classObj->m_sharedInterface->m_eventLog->AddEvent("AquireByGlid Dynamic Item load response:",(DWORD)result);
						  }
						break;
					  }
					  default:
					  {
							classObj->m_sharedInterface->m_eventLog->AddEvent("Unknown newtork communication type:",(DWORD)commandTypeRef);
							break;
					  }
				   }
				   
                   //delete command from list
                   classObj->m_sharedInterface->m_dbConnection->ExecQuery("delete from tblCommandsToServer where id = '"+idReference+"'");
			   }//end pocess requests
			   else
			      classObj->m_sharedInterface->m_dbConnection->Close(recordSet);
			}//end valid record set
			Sleep(1000);
		}
		if(hubConnection)
			delete hubConnection;
			hubConnection = 0;

		CoUninitialize();
return 1;
}

//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
DWORD WINAPI NodeTracker(LPVOID lpParameter)
{
        //publishes node avaiablility to cluster-this is check before zones are used or spawned 
	    //to ensure management node is up - 15 second error can occur in a crash situation
	    //TABLE is avaialble to all nodes because it resides on gameserver HUB sql

	    CServerMain * classObj = (CServerMain *)lpParameter;

		if(classObj->m_sharedInterface->m_threadedDataProcessor == NULL)
		   return 1;

		if(FAILED(CoInitialize(NULL)))
		{
           return 0;
		}

		CSharedNet * sNet = classObj->m_sharedInterface;
        CABFunctionLib * cLib = &classObj->m_sharedInterface->m_cabLib;
		

        CDBConnectionSystem * dbConnection = new CDBConnectionSystem(classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerIP,
			                                                         classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerCatalog,
																	 classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerUsername,
																	 classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerPassword,
																	 classObj->m_sharedInterface->m_eventLog);
        if(dbConnection->m_connectionEstablished == TRUE)
		{//verify connection established
		   
		   classObj->m_sharedInterface->m_eventLog->AddEvent("NodeTracker Thread connected and running");
		}
		else
		{
		   delete dbConnection;
		   dbConnection = 0;
		   classObj->m_sharedInterface->m_eventLog->AddEvent("NodeTracker Thread Failed to connect");
		   return 1;//leave because we have no basis to use the database
		}

		//initially clear by id on both tables
		CString serverID = classObj->m_sharedInterface->m_cabLib.ConvToString(classObj->m_sharedInterface->m_netComObject->m_serverID);
        
		dbConnection->ExecQuery("DELETE FROM SYSLIVESERVERNODES WHERE NODEID = '"+serverID+"'");
		//insert default node
        dbConnection->ExecQuery("INSERT INTO SYSLIVESERVERNODES (NODEID, CHECKINTIME) Values('"+serverID+"',GetDate())");
		//update loop
        CString updateNode = "UPDATE SYSLIVESERVERNODES SET CHECKINTIME = GetDate(), CONNECTIONCOUNT = '$CONNECTIONCNT', NODEIP='$NODEIP' WHERE NODEID = '"+serverID+"'";
		
		CString temp;
		//node will contually notify of its status to the table, programaticaly you would check to see avaiablility and then spawn a zone
        while(1)
		{
              EnterCriticalSection( &classObj->m_interactSection);
			  if(classObj->m_systemShutDown)
			  {
				classObj->m_shutDownThreadCount++;
			    LeaveCriticalSection( &classObj->m_interactSection);
			    break;
			  }
			  LeaveCriticalSection( &classObj->m_interactSection);

              temp = updateNode;

              EnterCriticalSection( &sNet->m_netComObject->m_statusLock);
              temp.Replace("$CONNECTIONCNT",sNet->m_cabLib.ConvToString(classObj->m_sharedInterface->m_netComObject->m_currentPlayersOnline));
			  LeaveCriticalSection( &sNet->m_netComObject->m_statusLock);
			  temp.Replace("$NODEIP",sNet->m_netComObject->m_hostIPString);

              dbConnection->ExecQuery(temp);//updates node publication to cluster
			  Sleep(15000);//wait 15 seconds between updates
		}

        delete dbConnection;
		dbConnection = 0;

		return 0;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
DWORD WINAPI ZoneConnectionUpdater(LPVOID lpParameter)
{
        //This keeps us up to date on how many are in each zone
	    //a key to know if we have reached our limit in that zone
	    //this will report group instances even though the number is capped from the opening

	    CServerMain * classObj = (CServerMain *)lpParameter;

		if(classObj->m_sharedInterface->m_threadedDataProcessor == NULL)
		   return 1;

		if(FAILED(CoInitialize(NULL)))
		{
           return 0;
		}

		CSharedNet * sNet = classObj->m_sharedInterface;
        CABFunctionLib * cLib = &classObj->m_sharedInterface->m_cabLib;
		

        CDBConnectionSystem * dbConnection = new CDBConnectionSystem(classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerIP,
			                                                         classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerCatalog,
																	 classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerUsername,
																	 classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerPassword,
																	 classObj->m_sharedInterface->m_eventLog);
        if(dbConnection->m_connectionEstablished == TRUE)
		{//verify connection established
		   
		   classObj->m_sharedInterface->m_eventLog->AddEvent("ZoneConnectionUpdater Thread connected and running");
		}
		else
		{
		   delete dbConnection;
		   dbConnection = 0;
		   classObj->m_sharedInterface->m_eventLog->AddEvent("ZoneConnectionUpdater Thread Failed to connect");
		   return 1;//leave because we have no basis to use the database
		}

		//initially clear by id on both tables
		CString serverID = cLib->ConvToString(classObj->m_sharedInterface->m_netComObject->m_serverID);
        
		CString updateStencil = "UPDATE SYSLIVEZONEDB SET CUR_CONNECTIONS = $CONNECTIONS WHERE NODEID = '"+serverID+"' AND ZONENAME = '$ZONENAME' AND CHANNEL = '$CHANNEL'";
		
		//temp vars
		CString temp;
        CString zoneName;
		int channel = 0;
        int instanceType = 0;
		int connections = 0;
        int trace = 0;
		int channelThatWasDeleted = -1;
		//these classes are thread safe everywhere
        while(1)
		{
              EnterCriticalSection( &classObj->m_interactSection);
			  if(classObj->m_systemShutDown)
			  {
				classObj->m_shutDownThreadCount++;
			    LeaveCriticalSection( &classObj->m_interactSection);
			    break;
			  }
			  LeaveCriticalSection( &classObj->m_interactSection);
              //build one query
			  
			  if(sNet->m_netComObject->m_currentWorldsUsed->GetDirtyDataAndCleanDormant_safe(trace,10,&zoneName,&channel,&instanceType,&connections,&channelThatWasDeleted,sNet->m_netComObject->m_runtimePlayersOnServer))
			  {//only updates on change-this function automatically resets trace-limits cell seacrh for more distributed processing
                 if(channelThatWasDeleted == -1)
				 {//necessary update zone connections
				    temp = updateStencil;
					temp.Replace("$ZONENAME",zoneName);
					temp.Replace("$CONNECTIONS",cLib->ConvToString(connections));
					temp.Replace("$CHANNEL",cLib->ConvToString(channel));
				
					dbConnection->ExecQuery(temp);//updates zone connection publication to cluster HUB
				 }//end necessary update zone connections
				 else
				 {//zone was deleted-need to close npc channel
				    int removedCount = 0;
					//close ai channel
					sNet->CloseAIChannel(channelThatWasDeleted);
					sNet->m_eventLog->LogByPriority("NPC CloseAIChannel() channel:",(DWORD)channelThatWasDeleted,2);
					//close generators then runtime
					BOOL result = sNet->m_netComObject->m_runtimeItems->DeleteAllOnChannel_dontNotify_safe(channelThatWasDeleted,&removedCount);
                    //ASSERT(result);
					sNet->m_eventLog->LogByPriority("NPC DeleteAllOnChannel() channel:",(DWORD)channelThatWasDeleted,2);
					sNet->m_eventLog->LogByPriority("NPC DeleteAllOnChannel() npc objects removed:",(DWORD)removedCount,2);
					removedCount = 0;
					result = sNet->m_serverItemGenerator->DeleteAllOnChannel_safe(channelThatWasDeleted,&removedCount);
                    //ASSERT(result);
					sNet->m_eventLog->LogByPriority("NPC GENERATOR DeleteAllOnChannel() channel:",(DWORD)channelThatWasDeleted,2);
					sNet->m_eventLog->LogByPriority("NPC GENERATOR DeleteAllOnChannel() npcs generators removed:",(DWORD)removedCount,2);
					
					result = sNet->m_worldChannelRuleDB->DeleteAllOnChannel_safe(channelThatWasDeleted,&removedCount);
                    //ASSERT(result);
					sNet->m_eventLog->LogByPriority("DYNAMIC RULES DeleteAllOnChannel() channel:",(DWORD)channelThatWasDeleted,2);
					sNet->m_eventLog->LogByPriority("DYNAMIC RULES DeleteAllOnChannel() removed:",(DWORD)removedCount,2);

					result = sNet->m_poiSystem->DeleteAllOnChannel_safe(channelThatWasDeleted,&removedCount);
                    //ASSERT(result);
					sNet->m_eventLog->LogByPriority("POI OBJECTS DeleteAllOnChannel() channel:",(DWORD)channelThatWasDeleted,2);
					sNet->m_eventLog->LogByPriority("POI OBJECTS DeleteAllOnChannel() removed:",(DWORD)removedCount,2);

				 }//end zone was deleted-need to close npc channel
			  }//end only updates on change
			  Sleep(250);
			  sNet->WorldRuleTimeCallback_safe(10);
			  Sleep(50);
		}

        delete dbConnection;
		dbConnection = 0;

		return 0;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
// one placed house, passed to the data thread for saving (same layout in AttributeSystems.cpp)
struct HOUSE_SAVE_REC { int accountBind; int worldChannel; int type; float x, y, z; };

// put every house from GM_SERVERPLACEMENTS back into the housing zones (server start)
static void LoadPlacedHouses(CSharedNet * sNet, CDBConnectionSystem * db)
{
	CHousingZoneDB * zones = sNet->m_netComObject->m_houseingDB;
	if(!zones || !sNet->m_housingDB)
	{
		sNet->m_eventLog->AddEvent("Placed houses not loaded: housing data (HPB/HSF) missing");
		return;
	}
	_RecordsetPtr rd = db->Query("SELECT ACCOUNTBIND, WORLDCHANNEL, POSX, POSY, POSZ, TYPE FROM GM_SERVERPLACEMENTS");
	if(rd == NULL)
	{
		sNet->m_eventLog->AddEvent("Placed houses not loaded: cannot read GM_SERVERPLACEMENTS");
		return;
	}
	int count = db->GetCount(rd), placed = 0;
	for(int i = 0; i < count; i++)
	{
		int acBind = db->GetIntData("ACCOUNTBIND",rd);
		int wld    = db->GetIntData("WORLDCHANNEL",rd);
		int type   = db->GetIntData("TYPE",rd);
		D3DVECTOR pos;
		pos.x = db->GetfloatData("POSX",rd); pos.y = db->GetfloatData("POSY",rd); pos.z = db->GetfloatData("POSZ",rd);
		CHousingObj * model = sNet->m_housingDB->GetByIndex(type);
		if(model && !zones->GetHouseByAccountBind(acBind))
		{
			BNDBOX box;
			box.min.x = model->m_clipBox.minX; box.min.y = model->m_clipBox.minY; box.min.z = model->m_clipBox.minZ;
			box.max.x = model->m_clipBox.maxX; box.max.y = model->m_clipBox.maxY; box.max.z = model->m_clipBox.maxZ;
			int newID = 0;
			if(zones->AddPlacementObjToTable(type,acBind,box,pos,&newID,wld) == 1)
				placed++;
			else
				sNet->m_eventLog->AddEvent("Placed house outside any housing zone, account:",(DWORD)acBind);
		}
		db->NextRecord(rd);
	}
	db->Close(rd);
	sNet->m_eventLog->AddEvent("Placed houses loaded:",(DWORD)placed);
}

DWORD WINAPI MiscDataProcessor(LPVOID lpParameter)
{
	    CServerMain * classObj = (CServerMain *)lpParameter;
     
        //return 0;//test

		if(classObj->m_sharedInterface->m_threadedDataProcessor == NULL)
		   return 1;

		if(FAILED(CoInitialize(NULL)))
		{
		   //AfxEndThread(0,TRUE);
           return 0;
		}

		CSharedNet * sNet = classObj->m_sharedInterface;
        CABFunctionLib * cLib = &classObj->m_sharedInterface->m_cabLib;
		
		CDBConnectionSystem * authConnection = new CDBConnectionSystem(	classObj->m_sharedInterface->m_authenticationDBConnection.m_sqlServerIP,
																		classObj->m_sharedInterface->m_authenticationDBConnection.m_sqlServerCatalog,
																		classObj->m_sharedInterface->m_authenticationDBConnection.m_sqlServerUsername,
																		classObj->m_sharedInterface->m_authenticationDBConnection.m_sqlServerPassword,
																		classObj->m_sharedInterface->m_eventLog);
        if(authConnection->m_connectionEstablished == TRUE)
		{//verify connection established
		   
		   classObj->m_sharedInterface->m_eventLog->AddEvent("MiscDataProcessor Thread connected and running");
		}
		else
		{
		   delete authConnection;
		   authConnection = 0;
		   classObj->m_sharedInterface->m_eventLog->AddEvent("MiscDataProcessor Thread Failed to connect **auth db");
		   return 1;//leave because we have no basis to use the database
		}

        CDBConnectionSystem * dbConnection = new CDBConnectionSystem(classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerIP,
			                                                         classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerCatalog,
																	 classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerUsername,
																	 classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerPassword,
																	 classObj->m_sharedInterface->m_eventLog);
        if(dbConnection->m_connectionEstablished == TRUE)
		{//verify connection established
		   
		   classObj->m_sharedInterface->m_eventLog->AddEvent("MiscDataProcessor Thread connected and running");
		}
		else
		{
		   delete dbConnection;
		   dbConnection = 0;
		   classObj->m_sharedInterface->m_eventLog->AddEvent("MiscDataProcessor Thread Failed to connect");
		   return 1;//leave because we have no basis to use the database
		}

		
		//establish base verification strings
		//check banned ip/username to DB and then log if banned
		CString getAccountData              = "SELECT * FROM GMACCOUNTBASE WHERE USERNAME = '$USER'";
		CString getAvailableCharacterCount  = "SELECT * FROM GMCHARACTERS WHERE USERNAME = '$USER' AND INUSE = '0'";
		CString getCharacterCount			= "SELECT * FROM GMCHARACTERS WHERE USERNAME = '$USER' AND INUSE = '1' AND GAMENAME = '$GAMENAME'";
		CString getMatchingAccounts			= "SELECT handle FROM GMCharacters WHERE charName = '$CHARNAME$' AND INUSE = '1'";
		CString insertCharacterData			= "INSERT INTO GMCHARACTERS (USERNAME, CHARNAME, INUSE, GAMENAME) Values('$USER','$CHARNAME','0','$GAMENAME')";
		CString characterRetrieval			= "SELECT * FROM GMCHARACTERS WHERE USERNAME = '$USER' AND INUSE = '1' AND GAMENAME = '$GAMENAME'";
		CString deleteCharacter			    = "UPDATE GMCHARACTERS SET INUSE = '0' WHERE HANDLE = '$HANDLE'";
		CString insertNewClan				= "INSERT INTO GMCLANDATA (CLANNAME, ABBREV, HANDLE) Values('$CLANNAME','$ABBREV','$HANDLE')";
		CString insertNewClanMember		    = "INSERT INTO GMCLANMEMBERLIST (HANDLE, MEMBERHANDLE, MEMBERNAME) Values('$HANDLE','$MEMBERHANDLE','$MEMBERNAME')";
		CString deleteClan				    = "DELETE FROM GMCLANDATA WHERE HANDLE = '$HANDLE'";
		CString deleteClanMembers		    = "DELETE FROM GMCLANMEMBERLIST WHERE HANDLE = '$HANDLE'";
		CString deleteClanMember		    = "DELETE FROM GMCLANMEMBERLIST WHERE MEMBERHANDLE = '$MEMBERHANDLE'";
		CString insertClanAtWar		        = "INSERT INTO GMCLANWARLIST (HANDLE, CLANATWARHANDLE) Values('$HANDLE','$CLANATWARHANDLE')";
		CString deleteClanAtWar		        = "DELETE FROM GMCLANWARLIST WHERE HANDLE = '$HANDLE' AND CLANATWARHANDLE = '$CLANATWARHANDLE'";
		CString deleteClanWarList		    = "DELETE FROM GMCLANWARLIST WHERE HANDLE = '$HANDLE'";
		CString updateClanMembersCID		= "UPDATE GMCHARACTERS SET CLANID = '$NEWCLANID' WHERE CLANID = '$CLANID'";
		CString getAllClans					= "SELECT * FROM GMCLANDATA";
		CString getAllMembers				= "SELECT * FROM GMCLANMEMBERLIST where HANDLE = '$HANDLE'";
		CString getWarList					= "SELECT * FROM GMCLANWARLIST where HANDLE = '$HANDLE'";
		CString getPlacementByAccountID     = "SELECT * FROM GM_SERVERPLACEMENTS WHERE ACCOUNTBIND = '$ACCOUNTBIND'";
		CString updatePlacementType         = "UPDATE GM_SERVERPLACEMENTS SET TYPE = '$TYPE' WHERE ACCOUNTBIND = '$ACCOUNTBIND'";
		CString insertPlacement             = "INSERT GM_SERVERPLACEMENTS (ACCOUNTBIND, WORLDCHANNEL, POSX, POSY, POSZ, TYPE) Values('$ACCOUNTBIND','$WORLDCHANNEL','$POSX','$POSY','$POSZ','$TYPE')";
		CString installVaultInstallation    = "UPDATE GM_SERVERPLACEMENTS SET VAULTINSTALLED = '$VAULTINSTALLED', VAULTCAPACITY = '$VAULTCAPACITY' WHERE ACCOUNTBIND = '$ACCOUNTBIND'";
		CString installStoreInstallation    = "UPDATE GM_SERVERPLACEMENTS SET STOREITEMCAPACITY = '$STOREITEMCAPACITY' WHERE ACCOUNTBIND = '$ACCOUNTBIND'";
		CString deleteHouse                 = "DELETE FROM GM_SERVERPLACEMENTS WHERE ACCOUNTBIND = '$ACCOUNTBIND'";
		CString deleteHouseInv              = "DELETE FROM GM_PLACEMENTINVENTORY WHERE ACCOUNTBIND = '$ACCOUNTBIND'";
        CString deletePlacementInventory    = "DELETE FROM GM_PLACEMENTINVENTORY WHERE ITEMGIL = '$ITEMGIL' AND ACCOUNTBIND = '$ACCOUNTBIND' AND STORAGELOCATION = '$STORAGELOCATION' AND PRICE = '$PRICE' AND DMGMOD = '$DMGMOD'";
        //CString deletePlacementInventory    = "DELETE GM_PLACEMENTINVENTORY FROM (SELECT TOP 1 * FROM GM_PLACEMENTINVENTORY WHERE ITEMGIL = '$ITEMGIL' AND ACCOUNTBIND = '$ACCOUNTBIND' AND STORAGELOCATION = '$STORAGELOCATION') AS t1 WHERE GM_PLACEMENTINVENTORY.orderindex = t1.orderindex AND GM_PLACEMENTINVENTORY.ACCOUNTBIND = t1.ACCOUNTBIND";		
		CString getInventoryItemQuanity     = "SELECT * FROM GM_PLACEMENTINVENTORY WHERE ACCOUNTBIND = '$ACCOUNTBIND' AND ITEMGIL = '$ITEMGIL' AND STORAGELOCATION = '$STORAGELOCATION' AND DMGMOD = '$DMGMOD'";
		CString updateInventoryItemQuanity  = "UPDATE GM_PLACEMENTINVENTORY SET QUANITY = '$QUANITY' WHERE ACCOUNTBIND = '$ACCOUNTBIND' AND ITEMGIL = '$ITEMGIL' AND STORAGELOCATION = '$STORAGELOCATION' AND DMGMOD = '$DMGMOD'";
		CString addInventoryItemQuanity     = "INSERT GM_PLACEMENTINVENTORY (ACCOUNTBIND, ITEMNAME, ITEMGIL, QUANITY, PRICE, STORAGELOCATION,DMGMOD) Values('$ACCOUNTBIND','$ITEMNAME','$ITEMGIL','$QUANITY','$PRICE','$STORAGELOCATION','$DMGMOD')";
        CString UpdateVaultCash             = "UPDATE GM_SERVERPLACEMENTS SET VAULTCURRENTCASH = '$VAULTCURRENTCASH' WHERE ACCOUNTBIND = '$ACCOUNTBIND'";
		CString UpdateStoreCash             = "UPDATE GM_SERVERPLACEMENTS SET STORECURRENTCASH = '$STORECURRENTCASH' WHERE ACCOUNTBIND = '$ACCOUNTBIND'";
		CString deleteInventory		        = "DELETE FROM GMINVENTORY WHERE HANDLE = '$HANDLE'";
		CString deleteSkills			    = "DELETE FROM GMSKILLS WHERE HANDLE = '$HANDLE'";
		CString deleteStats			        = "DELETE FROM GMSTATS WHERE HANDLE = '$HANDLE'";
		CString deleteGMCOMMCfgs		    = "DELETE FROM GMCOMMLIST WHERE HANDLE = '$HANDLE'";
		CString deleteJournal			    = "DELETE FROM GMJOURNAL WHERE HANDLE = '$HANDLE'";
		CString deleteJournalReqItems       = "DELETE FROM GMJOURNALREQUIREDITEMS WHERE HANDLE = '$HANDLE'";
		CString deleteDimCfgs		        = "DELETE FROM GMDIMCFGS WHERE HANDLE = '$HANDLE'";
		CString updateCharGameBasis		    = "UPDATE GMCHARACTERS SET GAMENAME = '$GAMENAME' WHERE HANDLE = '$HANDLE'";
		CString getMaxCharCreateByGame      = "SELECT * FROM GM_GAMESPECIFICS WHERE GAMENAME = '$GAMENAME' AND SPECIFICPARAM = 'MAXCHARCOUNT'";
		CString requestNodes		        = "SELECT * FROM SYSLIVESERVERNODES WHERE DATEADD(SECOND,25,CHECKINTIME) > GETDATE() ORDER BY CONNECTIONCOUNT"; //return data by least connections first
		CString requestSpecificNode		    = "SELECT * FROM SYSLIVESERVERNODES WHERE DATEADD(SECOND,25,CHECKINTIME) > GETDATE() AND NODEID = '$NODEID' ORDER BY CONNECTIONCOUNT";
		CString requestZones		        = "SELECT * FROM SYSLIVEZONEDB";
		CString requestSpecificZone		    = "SELECT * FROM SYSLIVEZONEDB WHERE ZONENAME = '$ZONENAME' AND INSTANCETYPE = '$INSTANCETYPE' AND CUR_CONNECTIONS < MAX_CONNECTIONS ORDER BY CUR_CONNECTIONS";
        CString requestWorldStencil		    = "SELECT * FROM SYSWORLDSTENCILS WHERE EXGNAME = '$EXGNAME'";
		

        bool snapShotOnce = true;
		CString temp;
		CCDataHandler * dataObj				= NULL;
		DWORD checkInStamp = GetTickCount();
        long longQueTemp = 0;
        while(1)
		{
			EnterCriticalSection( &classObj->m_interactSection);
			if(classObj->m_systemShutDown)
			{
				classObj->m_shutDownThreadCount++;
			    LeaveCriticalSection( &classObj->m_interactSection);
			    break;
			}
			LeaveCriticalSection( &classObj->m_interactSection);


			if(classObj->m_sharedInterface->m_threadedDataProcessor->GetNextObject(&dataObj)==false)
			{
               Sleep(500);
               continue;
			}
			else
			   Sleep(0);


			if(GetTickCount()-checkInStamp > 300000)
			{//check in thread every 5 minutes
			   longQueTemp = classObj->m_sharedInterface->m_threadedDataProcessor->GetQueCount();
               sNet->m_eventLog->AddEvent("DataProcThread Checkin queCount:",(DWORD)longQueTemp);
               checkInStamp = GetTickCount();
			   if(longQueTemp > 10)
			     if(snapShotOnce == true)
			     {//one time only snap part of que
                    snapShotOnce = false;
                    EnterCriticalSection(&classObj->m_sharedInterface->m_threadedDataProcessor->m_cSection);
                    for(POSITION posLoc = classObj->m_sharedInterface->m_threadedDataProcessor->GetHeadPosition();posLoc != NULL;)
					{
                        CCDataHandler * dbPtr = (CCDataHandler *)classObj->m_sharedInterface->m_threadedDataProcessor->GetNext(posLoc);
						sNet->m_eventLog->AddEvent("DataProcThread que snap:"+dbPtr->m_text+ " type:",(DWORD)dbPtr->m_type);
					} 
					LeaveCriticalSection(&classObj->m_sharedInterface->m_threadedDataProcessor->m_cSection);
			     }
			}//end check in thread every 5 minutes


			
			if(dataObj)
		    {//process all object

				classObj->m_sharedInterface->m_eventLog->LogByPriority("MiscDataProcessor() type:",(DWORD)dataObj->m_type,3);
				if(dataObj->m_type == 0)
				{//character processor
				  //send character list if available the wait for input/character create
                  //update current game
				  temp = getAccountData;
			      temp.Replace("$USER",dataObj->m_text);
				  _RecordsetPtr ingameNameReader = authConnection->Query(temp);
			      int acCount = authConnection->GetCount(ingameNameReader);
				  CString gameName = "";				  
				  if(acCount > 0)
				  {//has characters in this game
					 gameName = authConnection->GetStringData("InGameName",ingameNameReader);
				  }
				  authConnection->Close(ingameNameReader);
				  //-----------------------	
				  temp = characterRetrieval;
				  temp.Replace("$USER",dataObj->m_text);
				  //temp.Replace("$GAMENAME",dataObj->m_text2);
				  temp.Replace("$GAMENAME",gameName);
                  _RecordsetPtr characterReader = dbConnection->Query(temp);
				  int characterCount = dbConnection->GetCount(characterReader);
				  if(characterCount > 0)
				  {//has characters in this game
                     int sizeCalc = (characterCount * 25)+3;
                     BYTE * cData = new BYTE[sizeCalc];
					 ZeroMemory(cData,sizeCalc);
					 cData[0] = 41;
					 cData[1] = characterCount;
					 cData[2] = 50;
					 
                     for(int cLoop=0; cLoop< characterCount; cLoop++)
					 {
                        CString charName = dbConnection->GetStringData("charName",characterReader);
						int length = charName.GetLength();
						if(length > 24)
						   length = 24;
						
						if(length > 0)
							CopyMemory(&cData[3+(25*cLoop)],charName.GetBuffer(),length);
                        dbConnection->NextRecord(characterReader);
					 }
					 classObj->m_sharedInterface->m_netComObject->SendData( cData,
																			sizeCalc,
																			DPNSEND_GUARANTEED,
																			dataObj->m_dpnid,
																			classObj->m_sharedInterface->m_netComObject->m_packetTimeout);
					 delete [] cData;
					 cData = 0;
				  }//end has characters in this game
				  else
				  {//has no characters
                     classObj->m_sharedInterface->m_netComObject->SendMenuBuildPacket(  DPNSEND_GUARANTEED,
																						dataObj->m_dpnid,
																						0,
																						"No characters Created Yet",
																						dataObj->m_int);
				  }//end has no characters
                  dbConnection->Close(characterReader);
				  //add account hub to local DB if necessary
                  //send char packet
				}//end character processor
				else if(dataObj->m_type == 1)
				{//router-called by HandleSpawn()
					 int thisServerNodeID = classObj->m_sharedInterface->m_netComObject->m_serverID;//start at this server
					 //very little locking , can take its time
					 //we will assume for now that the player is requesting a normal type zone
                     LPSPAWN_STRUCT formatedSpawnMsg = (LPSPAWN_STRUCT)dataObj->m_miscData;
                     //dataObj->m_int3;//instance type search
					 //check to see if zone exists and is avaiable if theres multiple ones select lowest connection count priority
                     CString exgName		= formatedSpawnMsg->worldName;
					 int instanceTypeTarget		= dataObj->m_int2;

					 temp = requestSpecificZone;
					 temp.Replace("$ZONENAME",exgName);
					 temp.Replace("$INSTANCETYPE",sNet->m_cabLib.ConvToString(instanceTypeTarget));

                     sNet->m_eventLog->LogByPriority("Router:Looking for zone:"+exgName+" instance type:",(DWORD)instanceTypeTarget,3);

					 
					 
					 int channel = -1;
					 EnterCriticalSection( &sNet->m_netComObject->m_statusLock);
                     int curApproxConnections = classObj->m_sharedInterface->m_netComObject->m_currentPlayersOnline;//this is in regards to this node
					 LeaveCriticalSection( &sNet->m_netComObject->m_statusLock);

					 EnterCriticalSection(&classObj->m_instanceRouterSection);
					 _RecordsetPtr availZones = dbConnection->Query(temp);
				     int availZoneCount = dbConnection->GetCount(availZones);

					 CString nodeIP,dynRules;
					 
					 int nodeID = -1;
					 int pending = 0;
					 int instanceType = 0;
					 
					 for(int nodeCheckLoop = 0;nodeCheckLoop < availZoneCount;nodeCheckLoop++)
					 {//take first avail zone-if it exists-will also detect a zone thats spawned but is really a crashed node
                        nodeID			= dbConnection->GetIntData("NODEID",availZones);
						channel			= dbConnection->GetIntData("CHANNEL",availZones);
						pending			= dbConnection->GetIntData("PENDING",availZones);
						instanceType	= dbConnection->GetIntData("INSTANCETYPE",availZones);
						dynRules		= dbConnection->GetStringData("DYNRULES",availZones);
						//verify valid node
						temp = requestSpecificNode;
						temp.Replace("$NODEID",cLib->ConvToString(nodeID));
						_RecordsetPtr nodeExists = dbConnection->Query(temp);
						
						//
						if(nodeID == sNet->m_netComObject->m_serverID)
						{
							
							if(!sNet->m_netComObject->m_currentWorldsUsed->DoesWorldExistByInstanceTypeAndName(instanceType,exgName))
								if(pending == 1)//zone still needs to be created
								{
									if(!sNet->EnablePendingZone_async(dbConnection,exgName,instanceType,&channel,dynRules))
										sNet->m_eventLog->LogByPriority("**Router:EnablePendingZone_async FAILED!:",(DWORD)channel,2);
								}
							
						}
						
						
						if(dbConnection->GetCount(nodeExists) > 0)
						{
                           nodeIP = dbConnection->GetStringData("NODEIP",nodeExists);//switch target servers
						   
                           dbConnection->Close(nodeExists);
						   LeaveCriticalSection(&classObj->m_instanceRouterSection);
                           break;
						}
						else
						{//this ussually means the zone that is in the table no longer has a live node, need to handle this by closing the zone	
						   nodeID = -1;
						}
						dbConnection->Close(nodeExists);
					
						dbConnection->NextRecord(availZones);
					 }//end take first avail zone
					 dbConnection->Close(availZones);
					 LeaveCriticalSection(&classObj->m_instanceRouterSection);
					 

                     if(nodeID > -1)
					 {//Found zone
						sNet->m_eventLog->LogByPriority("Router:Found Existing zone:",3);
                        if(nodeID == sNet->m_netComObject->m_serverID)
						{//this server hosts the target zone spawn here
							//if(pending == 1)//zone still needs to be created
							 //  sNet->EnablePendingZone_async(dbConnection,exgName,instanceType,&channel,dynRules);

							CAccountObject * acPtrTemp = sNet->m_netComObject->m_runtimeAccountDB->GetAccountByID_LOS(dataObj->m_dpnid);
							if(acPtrTemp)
							{
                               if(!classObj->m_spawnSystems->HandleSpawnMessagesServerSide(formatedSpawnMsg,dataObj->m_dpnid,acPtrTemp,channel))
								   sNet->m_eventLog->LogByPriority("**Router:HandleSpawnMessagesServerSide FAILED!:",(DWORD)channel,2);

							   sNet->m_netComObject->m_runtimeAccountDB->Unlock();
							}
						}
						else
						{//a different server holds this zone we need to diconnect do a reconnect
							//this will use a base send method, because its the last packet the chaacter will get before disconnecting
	                        //note characters arent spawn so disconnect will imediatly begin to flag the logon state
							RECONNECT_STRUCT reconnectStr;
							ZeroMemory(&reconnectStr,sizeof(RECONNECT_STRUCT));
							reconnectStr.catagorie = 2;
							if(nodeIP.GetLength() < 20)
							{
								if(nodeIP.GetLength() > 0)
									CopyMemory(reconnectStr.ipaddress,nodeIP.GetBuffer(),nodeIP.GetLength());
							   sNet->m_netComObject->SendData((byte*)&reconnectStr,
				                                              sizeof(RECONNECT_STRUCT),
				                                              DPNSEND_GUARANTEED,
				                                              dataObj->m_dpnid,
											                  sNet->m_netComObject->m_packetTimeout);//must send this way because other msgs not cycling at this time
							   sNet->m_eventLog->LogByPriority("Router:sent reconnect msg to client:",(DWORD)dataObj->m_dpnid,2);
							}
							else
							   sNet->m_eventLog->AddEvent("***RECONNECT ERROR - NODEIP TOO LONG***:",nodeIP);
						}//end a different server holds this zone we need to diconnect do a reconnect
					 }
					 else
					 {//no avail zones of this type-SPAWN ZONE
                        //aquire node with the least connections
						sNet->m_eventLog->LogByPriority("Router:Attempt to instance a zone stencil:",2);
						temp = requestNodes;//gets them from least to greatest connection count
						_RecordsetPtr availNodes = dbConnection->Query(temp);
				        int nodeCount = dbConnection->GetCount(availNodes);
						int targetZoneCreate_nodeID = thisServerNodeID;
						
						int tempNodeID = -1;
					    int tempConnectionCount = 0;
						CString targetZoneCreate_ipAddress;
						for(int nodeSelLoop=0;nodeSelLoop < nodeCount;nodeSelLoop++)
						{//switch node logic
                           tempNodeID					= dbConnection->GetIntData("NODEID",availNodes);
						   tempConnectionCount			= dbConnection->GetIntData("CONNECTIONCOUNT",availNodes);
                           if(tempNodeID != thisServerNodeID)
						      if(tempConnectionCount < curApproxConnections-1)//TODO-live server this should be 10-50 and test should be 1
							  {//switch
                                 targetZoneCreate_nodeID    = tempNodeID;
						         targetZoneCreate_ipAddress	= dbConnection->GetStringData("NODEIP",availNodes);
								 break;
							  }//end switch
						   dbConnection->NextRecord(availNodes);
						}//end switch node logic
						dbConnection->Close(availNodes);

						if(targetZoneCreate_nodeID == -1)
						{//bad error-no nodes to create zone on
							sNet->m_eventLog->AddEvent("***CANNOT FIND NODE TO CREATE ZONE ON*** SVR ID",(DWORD)sNet->m_netComObject->m_serverID);
						}//end bad error-no nodes to create zone on
						else 
						{// found target node
							sNet->m_eventLog->LogByPriority("Router:Found Node:",(DWORD)targetZoneCreate_nodeID,2);
                            if(targetZoneCreate_nodeID == sNet->m_netComObject->m_serverID)
							{//this server is the correct server for the new zone
								sNet->m_eventLog->LogByPriority("Router:Zone Creation Begin",2);
                                int newChannel = 0;
								if(sNet->SpawnZone_async(dbConnection,exgName,instanceType,newChannel))
								{
									//spawn
									CAccountObject * acPtrTemp = sNet->m_netComObject->m_runtimeAccountDB->GetAccountByID_LOS(dataObj->m_dpnid);
									if(acPtrTemp)
									{
									   if(!classObj->m_spawnSystems->HandleSpawnMessagesServerSide(formatedSpawnMsg,dataObj->m_dpnid,acPtrTemp,newChannel))
										   sNet->m_eventLog->LogByPriority("***Router:HandleSpawnMessagesServerSide FAILED!!:",(DWORD)newChannel,2);
									   sNet->m_netComObject->m_runtimeAccountDB->Unlock();
									}
									sNet->m_eventLog->AddEvent("***Zone created and spawn msg sent***:",formatedSpawnMsg->worldName);
								}
							
							}//end this server is the correct server for the new zone
							else
							{//do a reconnect to different node here
								sNet->m_eventLog->LogByPriority("Router:Reconnect to different node:"+targetZoneCreate_ipAddress,2);
                                //this will use a base send method, because its the last packet the chaacter will get before disconnecting
								//note characters arent spawn so disconnect will imediatly begin to flag the logon state
								RECONNECT_STRUCT reconnectStr;
								ZeroMemory(&reconnectStr,sizeof(RECONNECT_STRUCT));
								reconnectStr.catagorie = 2;
								if(targetZoneCreate_ipAddress.GetLength() < 20)
								{
								   if(targetZoneCreate_ipAddress.GetLength() > 0)
										CopyMemory(reconnectStr.ipaddress,targetZoneCreate_ipAddress.GetBuffer(),targetZoneCreate_ipAddress.GetLength());
								   sNet->m_netComObject->SendData((byte*)&reconnectStr,
																  sizeof(RECONNECT_STRUCT),
																  DPNSEND_GUARANTEED,
																  dataObj->m_dpnid,
																  sNet->m_netComObject->m_packetTimeout);//must send this way because other msgs not cycling at this time
								   sNet->m_eventLog->LogByPriority("Router ZC:sent reconnect msg to client:",(DWORD)dataObj->m_dpnid,2);
								}
								else
								   sNet->m_eventLog->AddEvent("***RECONNECT ERROR 2 - targetZoneCreate_ipAddress TOO LONG***:",targetZoneCreate_ipAddress);
								}//end do a reconnect to different node here
						}// end found target node
					 }//end no avail zones of this type-SPAWN ZONE
					 sNet->m_eventLog->LogByPriority("End Router:",3);
				}//end router
				else if(dataObj->m_type == 2)
				{//create character
				   if(dataObj->m_miscData)
				   {
				     CString accountName, currentPlayingGame;

                     //aquire username from DPNID
				     CAccountObject * acPtrTemp = classObj->m_sharedInterface->m_netComObject->m_runtimeAccountDB->GetAccountByID_LOS(dataObj->m_dpnid);
                     if(!acPtrTemp)
					 {
                         delete dataObj;
						 dataObj = 0;
					     continue;
					 }

					 if(!acPtrTemp->AllowAsyncQue())
					 {//throttle
						 classObj->m_sharedInterface->m_netComObject->m_runtimeAccountDB->Unlock();
						 delete dataObj;
						 dataObj = 0;
						 continue;
					 }//end throttle
					 accountName = acPtrTemp->m_accountName;
					 currentPlayingGame = acPtrTemp->m_currentlyPlayingGameName;
					 classObj->m_sharedInterface->m_netComObject->m_runtimeAccountDB->Unlock();


				     int newHandle = 0;
				     //see if avail
                     temp = getAvailableCharacterCount;
					 temp.Replace("$USER",accountName);
                     _RecordsetPtr availDetection = dbConnection->Query(temp);
				     int availCharCount = dbConnection->GetCount(availDetection);
					 if(availCharCount > 0)
					 {//get handle
                        newHandle = dbConnection->GetIntData("HANDLE",availDetection);
					 }//end get handle
					 dbConnection->Close(availDetection);
                     
                     //check current character count
                     temp = getCharacterCount;		
					 temp.Replace("$USER",accountName);
					 temp.Replace("$GAMENAME",currentPlayingGame);
					 _RecordsetPtr charCountReader = dbConnection->Query(temp);
					 int currentCharCount = dbConnection->GetCount(charCountReader);
					 dbConnection->Close(charCountReader);
					 //aquire max char creation count
					 /*temp = getMaxCharCreateByGame;
					 temp.Replace("$GAMENAME",currentPlayingGame);
					 _RecordsetPtr acReader = dbConnection->Query(temp);
					 int maxCharCount = dbConnection->GetIntData("VALUE",acReader);
					 if(maxCharCount < 1)
					 {//invalid value or db error
					    sNet->m_eventLog->AddEvent("Aquiring max char create from DB failed or invalid value is set defaulting to 5");
					    maxCharCount = 5;
					 }//end invalid value or db error
					 dbConnection->Close(acReader);*/

					 int maxCharCount = sNet->m_maxCharacterCreate;

					/***************************************************************
					** Duplicate Character Check - Prevents users from creating 
					** a character with the same name as a character that already 
					** exists in the database.
					***************************************************************/
					char* nameBuffer = NULL;
					LPCREATECHARHEADER_DATA header = (LPCREATECHARHEADER_DATA)dataObj->m_miscData;
					int headerSizeCC = sizeof(CREATECHARHEADER_DATA);

					if(headerSizeCC+header->m_nameLength < dataObj->m_bufSz && header->m_nameLength > 0)
					{//in allocation bounds
						nameBuffer = new char[header->m_nameLength+1];
						ZeroMemory(nameBuffer,header->m_nameLength+1);//creates Null terminator as long as name data is shorter
						CopyMemory(nameBuffer,&dataObj->m_miscData[headerSizeCC],header->m_nameLength);
					}//end in allocation bounds
					else
					{//invalid name info abort--bug fix prevents use of invalid pointer AB 03/13/06-SV Crash
                       delete dataObj;
					   dataObj = 0;
					   continue;
					}//end invalid name info abort --bug fix prevents use of invalid pointer AB 03/13/06-SV Crash
					
					// Search Database for existing characters with the same name
					temp = getMatchingAccounts;
					temp.Replace("$CHARNAME$", nameBuffer);
					
					_RecordsetPtr ptrDuplicateCharCheck = dbConnection->Query(temp);
					int duplicateCharCount = dbConnection->GetCount(ptrDuplicateCharCheck);
					dbConnection->Close(ptrDuplicateCharCheck);

					if (duplicateCharCount > 0)
					{ // character name already exists in DB. Error out.
						// Send text and break from creation
						sNet->SendTextMsg(DPNSEND_GUARANTEED,dataObj->m_dpnid,(char*)"Sorry, A Character with that name already exists.",0,0,11);
						
						// Delete memory since it is no longer needed.
						if(nameBuffer)
						{
							delete [] nameBuffer;
							nameBuffer = 0;
						}
						
						// Delete Data Object and Exit out of Character Creation
						delete dataObj;
						dataObj = 0;
						continue;
					} // end character name already exists in DB.
					// End of Duplicate Name Check
					

					/********************************************************************
					** The nameBuffer memory is cleaned up below this point
					** to make it available to Invalid Character Name Check and length
					** restriction check
					********************************************************************/
					
					/**************************************************************
					** Character Name Length Restriction - Limit character names 
					** to 15 Characters
					**************************************************************/
					if (strlen(nameBuffer) > 15)
					{ // Create an error for the user and cleanup memory
						// Send text and break from creation
						sNet->SendTextMsg(DPNSEND_GUARANTEED,dataObj->m_dpnid,(char*)"Sorry, The Character Name you attempted to create is too long. Please limit your name to 15 characters.",0,0,11);
						
						// Delete memory since it is no longer needed.
						if(nameBuffer)
						{
							delete [] nameBuffer;
							nameBuffer = 0;
						}
					
						// Delete Data Object and Exit out of Character Creation
						delete dataObj;
						dataObj = 0;
						continue;
					} // End Create an Error for the user and cleanup memory
					// End of Character Name having too many characters check

					/*********************************************************
					** Invalid Character Name Check - see if the character 
					** name has invalid characters in it.
					*********************************************************/
					char cInvalidNameCharacters[] = "~`!@#$%^&*()-_=+';:,.<>/?[]{}\\|";
					bool bInvalidPlayerName = false;

					for(int i=0;i<(int)strlen(nameBuffer);i++)
					{
						if (bInvalidPlayerName)
						{// An Invalid Character has already been found, break.
							break;
						}// End an Invalid Character already found

						// Make sure the player's name does not consist of any invalid characters
						for(int j=0;j<(int)strlen(cInvalidNameCharacters);j++)
						{
							if (nameBuffer[i] == cInvalidNameCharacters[j])
							{// Character has an invalid character in their name
								bInvalidPlayerName = true;
								break;
							}// End Character has an invalid character in their name
						}
					}
					
					if (bInvalidPlayerName)
					{ // Create an error for the user and cleanup memory
						// Send text and break from creation
						sNet->SendTextMsg(DPNSEND_GUARANTEED,dataObj->m_dpnid,(char*)"Sorry, There Are Invalid Characters in the Character Name you Attempted to Create.",0,0,11);
						
						// Delete memory since it is no longer needed.
						if(nameBuffer)
						{
							delete [] nameBuffer;
							nameBuffer = 0;
						}
					
						// Delete Data Object and Exit out of Character Creation
						delete dataObj;
						dataObj = 0;
						continue;
					} // End Create an Error for the user and cleanup memory
					// End of Character Name having invalid characters check
					
					/*************************************************
					** nameBuffer Memory Cleanup - Leak Prevention
					*************************************************/
					if(nameBuffer)
					{
						delete [] nameBuffer;
						nameBuffer = 0;
					}
					// End nameBuffer Memory Cleanup
					

					 if(newHandle == 0)
					 {//generate new character
						if(currentCharCount < maxCharCount)
						{//add new handle - doesnt use avail count because that means thats he has a hanlde allocated but its unused
							temp = insertCharacterData;
							temp.Replace("$USER",accountName);
							temp.Replace("$GAMENAME",currentPlayingGame);
							dbConnection->ExecQuery(temp);

							temp = getAvailableCharacterCount;
							temp.Replace("$USER",accountName);
							_RecordsetPtr newHandleConnect = dbConnection->Query(temp);
							availCharCount = dbConnection->GetCount(newHandleConnect);
							if(availCharCount > 0)
							{//get handle
								newHandle = dbConnection->GetIntData("HANDLE",newHandleConnect);
							}//end get handle
							dbConnection->Close(newHandleConnect);
						}//end add new handle
						else
						{//not allowed to create character
                            sNet->SendTextMsg(  DPNSEND_GUARANTEED,
												dataObj->m_dpnid,
												(char*)"Sorry, Cannot Create more characters for this game.",
												0,
												0,
												11);//confermation

							delete dataObj;
							dataObj = 0;
							continue;
						}//end not allowed to create character
					 }//end generate new character
					 
					  temp = getAccountData;
					  temp.Replace("$USER",accountName);
					  _RecordsetPtr ingameNameReader = authConnection->Query(temp);
					  int acCount = authConnection->GetCount(ingameNameReader);
					  int maxCharacters = 1;
					  if(acCount > 0)
					  {//has characters in this game							
						 maxCharacters = authConnection->GetIntData("maxCharacters", ingameNameReader);
					  }
					  authConnection->Close(ingameNameReader);//was never closed - leaked a recordset per character created
					 
					 if(newHandle != 0 && currentCharCount < maxCharacters)//maxCharCount)
					 {//create character
                        //update game that this character will be bound to
						 temp = updateCharGameBasis;
                         temp.Replace("$HANDLE",classObj->m_sharedInterface->m_cabLib.ConvToString(newHandle));
					     temp.Replace("$GAMENAME",currentPlayingGame);
						 dbConnection->ExecQuery(temp);
					
                        //look for open handle
						classObj->m_charCreationSystems->HandleCharacterCreationOnAccount(dataObj->m_miscData,dataObj->m_bufSz,dataObj->m_dpnid,newHandle);
						dataObj->m_miscData		= 0;
						dataObj->m_bufSz		= 0;						
					 }//end create character
					 else
					 {//not allowed to create character
                        sNet->SendTextMsg(  DPNSEND_GUARANTEED,
											dataObj->m_dpnid,
											(char*)"Sorry, Cannot Create more characters for this game.",
											0,
											0,
											11);//confermation

						delete dataObj;
						dataObj = 0;
						continue;
					 }//end not allowed to create character
				   }
				   delete dataObj;
				   dataObj = 0;
				   continue;
				}//end create character
				else if(dataObj->m_type == 3)
				{//delete character
				   //get handle of selected target delete
				   temp = getCharacterCount;
				   temp.Replace("$USER",dataObj->m_text);
				   temp.Replace("$GAMENAME",dataObj->m_text2);
				   _RecordsetPtr getCharacters = dbConnection->Query(temp);
				   int availCharCount = dbConnection->GetCount(getCharacters);
				   CString targetHandle = 0;
				   for(int iLoop=0;iLoop<availCharCount;iLoop++)
				   {//get handle
				      if(iLoop == dataObj->m_int)
					  {
					     targetHandle = dbConnection->GetStringData("HANDLE",getCharacters);
				         break;
					  }
					  dbConnection->NextRecord(getCharacters);
				   }//end get handle
				   dbConnection->Close(getCharacters);
				   if(targetHandle == "0")
				   {//failed
				      delete dataObj;
				      dataObj = 0;
				      continue;
				   }//end failed
				   //delete querry
				   temp = deleteCharacter;
				   temp.Replace("$HANDLE",targetHandle);
			       dbConnection->ExecQuery(temp);

				   temp = deleteInventory;
				   temp.Replace("$HANDLE",targetHandle);
			       dbConnection->ExecQuery(temp);

                   temp = deleteSkills;
				   temp.Replace("$HANDLE",targetHandle);
			       dbConnection->ExecQuery(temp);

				   temp = deleteStats;
				   temp.Replace("$HANDLE",targetHandle);
			       dbConnection->ExecQuery(temp);

				   temp = deleteGMCOMMCfgs;
				   temp.Replace("$HANDLE",targetHandle);
			       dbConnection->ExecQuery(temp);

				   temp = deleteJournal;
				   temp.Replace("$HANDLE",targetHandle);
			       dbConnection->ExecQuery(temp);

				   temp = deleteJournalReqItems;
				   temp.Replace("$HANDLE",targetHandle);
			       dbConnection->ExecQuery(temp);

				   temp = deleteDimCfgs;
				   temp.Replace("$HANDLE",targetHandle);
			       dbConnection->ExecQuery(temp);

				   temp = deleteClanMembers;
				   temp.Replace("$HANDLE",targetHandle);
			       dbConnection->ExecQuery(temp);

				   temp = deleteClan;
				   temp.Replace("$HANDLE",targetHandle);
			       dbConnection->ExecQuery(temp);
				   

	               //send notification to client that deletetion
	               classObj->m_sharedInterface->SendAttribMsg(  DPNSEND_GUARANTEED,
																dataObj->m_dpnid,
																14,
																dataObj->m_int,
																0,
																0,
																0,
																NULL);

				}//end delete character
                else if(dataObj->m_type == 4)
				{//create clan
                    
				}//end create clan
				else if(dataObj->m_type == 5)
				{//delete clan
                   
				}//end delete clan
				else if(dataObj->m_type == 6)
				{//add member
				    
				}//end add member
				else if(dataObj->m_type == 7)
				{//delete member
				    
				}//end delete member
                else if(dataObj->m_type == 8)
				{//add clan to war list (both clans need an addition)
				   
				}//end add clan to war list  (both clans need an addition)
				else if(dataObj->m_type == 9)
				{//remove clan to war list (both clans need to be removed)
                   
				}//end remove clan to war list (both clans need to be removed)
				else if(dataObj->m_type == 10)
				{//regenerate clan database
                   
				}//end regenerate clan database
				else if(dataObj->m_type == 11)
				{//save a newly placed house (HOUSE_SAVE_REC in m_miscData)
					if(dataObj->m_miscData && dataObj->m_bufSz >= (int)sizeof(HOUSE_SAVE_REC))
					{
						HOUSE_SAVE_REC * h = (HOUSE_SAVE_REC *)dataObj->m_miscData;
						temp = deleteHouse;
						temp.Replace("$ACCOUNTBIND",cLib->ConvToString(h->accountBind));
						dbConnection->ExecQuery(temp);
						temp = insertPlacement;
						temp.Replace("$ACCOUNTBIND",cLib->ConvToString(h->accountBind));
						temp.Replace("$WORLDCHANNEL",cLib->ConvToString(h->worldChannel));
						temp.Replace("$POSX",cLib->FloatToStingCnv(h->x));
						temp.Replace("$POSY",cLib->FloatToStingCnv(h->y));
						temp.Replace("$POSZ",cLib->FloatToStingCnv(h->z));
						temp.Replace("$TYPE",cLib->ConvToString(h->type));
						dbConnection->ExecQuery(temp);
					}
				}//end save placed house
				else if(dataObj->m_type == 12)
				{//load placed houses (queued once at server start)
					LoadPlacedHouses(sNet,dbConnection);
				}
				else if(dataObj->m_type == 14)
				{//delete related house bind
				   temp = deleteHouse;
				   temp.Replace("$ACCOUNTBIND",cLib->ConvToString(dataObj->m_int));
				   dbConnection->ExecQuery(temp);

				   temp = deleteHouseInv;
				   temp.Replace("$ACCOUNTBIND",cLib->ConvToString(dataObj->m_int));
				   dbConnection->ExecQuery(temp);
				}//end delete related house bind
                else if(dataObj->m_type == 15)
				{//delete placement item by gil and quanity
                   //CString deletePlacementInventory    = "DELETE FROM GM_PLACEMENTINVENTORY WHERE ITEMGIL = '$ITEMGIL' AND ACCOUNTBIND = '$ACCOUNTBIND'";
                   //CString getInventoryItemQuanity     = "SELECT * FROM GM_PLACEMENTINVENTORY WHERE ACCOUNTBIND = '$ACCOUNTBIND' AND ITEMGIL = '$ITEMGIL'";

					temp = getInventoryItemQuanity;
					temp.Replace("$ACCOUNTBIND",cLib->ConvToString(dataObj->m_int));
					temp.Replace("$ITEMGIL",cLib->ConvToString(dataObj->m_int2));
					temp.Replace("$STORAGELOCATION",cLib->ConvToString(dataObj->m_int3));
					temp.Replace("$DMGMOD",dataObj->m_text2);
					
					DWORD currentQuanity = 0;
					int currentGil = dataObj->m_int2;

					_RecordsetPtr itemReader = dbConnection->Query(temp);
				    int itemCount = dbConnection->GetCount(itemReader);
				    if(itemCount == 0)
				    {//error
						sNet->m_eventLog->AddEvent("Cannot Remove House item AC:");
						dbConnection->Close(itemReader);
				    }//end error
					else
					{//process
                         currentQuanity = (DWORD)dbConnection->GetIntData("QUANITY",itemReader);
						 dbConnection->Close(itemReader);
						 DWORD finalQuanity = currentQuanity - dataObj->m_dword;
						 if(finalQuanity < 1)
						 {//delete item
                            temp = deletePlacementInventory;
							temp.Replace("$ACCOUNTBIND",cLib->ConvToString(dataObj->m_int));
					        temp.Replace("$ITEMGIL",cLib->ConvToString(dataObj->m_int2));
							temp.Replace("$STORAGELOCATION",cLib->ConvToString(dataObj->m_int3));
							temp.Replace("$PRICE",cLib->ConvToString(dataObj->m_dword2));
							temp.Replace("$DMGMOD",dataObj->m_text2);
                            dbConnection->ExecQuery(temp);
						 }//end delete item
						 else
						 {//update quanity
                            temp = updateInventoryItemQuanity;
							temp.Replace("$ACCOUNTBIND",cLib->ConvToString(dataObj->m_int));
					        temp.Replace("$ITEMGIL",cLib->ConvToString(dataObj->m_int2));
							temp.Replace("$QUANITY",cLib->ConvToString(finalQuanity));
							temp.Replace("$STORAGELOCATION",cLib->ConvToString(dataObj->m_int3));
							temp.Replace("$DMGMOD",dataObj->m_text2);
                            dbConnection->ExecQuery(temp);
						 }//end update quanity
					}//end process
				}//end delete placement item by gil and quanity
				else if(dataObj->m_type == 16)
				{//add placement item by gil and quanity
                   temp = getInventoryItemQuanity;
					temp.Replace("$ACCOUNTBIND",cLib->ConvToString(dataObj->m_int));
					temp.Replace("$ITEMGIL",cLib->ConvToString(dataObj->m_int2));
					temp.Replace("$STORAGELOCATION",cLib->ConvToString(dataObj->m_int3));
					temp.Replace("$DMGMOD",dataObj->m_text2);
					DWORD currentQuanity = 0;
					int currentGil = dataObj->m_int2;

					_RecordsetPtr itemReader = dbConnection->Query(temp);
				    int itemCount = dbConnection->GetCount(itemReader);
				    if(itemCount == 0)
				    {//add item
						 dbConnection->Close(itemReader);
                         temp = addInventoryItemQuanity;
					     temp.Replace("$ACCOUNTBIND",cLib->ConvToString(dataObj->m_int));
					     temp.Replace("$ITEMGIL",cLib->ConvToString(dataObj->m_int2));
						 temp.Replace("$QUANITY",cLib->ConvToString(dataObj->m_dword));
						 temp.Replace("$PRICE",cLib->ConvToString(dataObj->m_dword2));
						 temp.Replace("$ITEMNAME",dataObj->m_text);
						 temp.Replace("$STORAGELOCATION",cLib->ConvToString(dataObj->m_int3));
						 temp.Replace("$DMGMOD",dataObj->m_text2);
                         dbConnection->ExecQuery(temp);
				    }//end add item
					else
					{//add quanity
                         currentQuanity = (DWORD)dbConnection->GetIntData("QUANITY",itemReader);
						 DWORD finalQuanity = currentQuanity + dataObj->m_dword;
						 dbConnection->Close(itemReader);
                         temp = updateInventoryItemQuanity;
					     temp.Replace("$ACCOUNTBIND",cLib->ConvToString(dataObj->m_int));
					     temp.Replace("$ITEMGIL",cLib->ConvToString(dataObj->m_int2));
						 temp.Replace("$QUANITY",cLib->ConvToString(finalQuanity));
						 temp.Replace("$STORAGELOCATION",cLib->ConvToString(dataObj->m_int3));
						 temp.Replace("$DMGMOD",dataObj->m_text2);
                         dbConnection->ExecQuery(temp);
					}//end add quanity

				}//end add placement item by gil and quanity
				else if(dataObj->m_type == 17)
				{//update store cash
		        //CString UpdateStoreCash             = "UPDATE GM_SERVERPLACEMENTS SET STORECURRENTCASH = '$STORECURRENTCASH' WHERE ACCOUNTBIND = '$ACCOUNTBIND'";
				    temp = UpdateStoreCash;
					temp.Replace("$ACCOUNTBIND",cLib->ConvToString(dataObj->m_int));
					temp.Replace("$STORECURRENTCASH",cLib->ConvToString(dataObj->m_int2));
                    dbConnection->ExecQuery(temp);
				}//end update store cash
				else if(dataObj->m_type == 18)
				{//update vault cash
                //CString UpdateVaultCash             = "UPDATE GM_SERVERPLACEMENTS SET VAULTCURRENTCASH = '$VAULTCURRENTCASH' WHERE ACCOUNTBIND = '$ACCOUNTBIND'";
                    temp = UpdateVaultCash;
					temp.Replace("$ACCOUNTBIND",cLib->ConvToString(dataObj->m_int));
					temp.Replace("$VAULTCURRENTCASH",cLib->ConvToString(dataObj->m_int2));
                    dbConnection->ExecQuery(temp);
				}//end update vault cash
				else if(dataObj->m_type == 19)
				{//ask a bot a question **DISABLED BECAUSE ITS SLOW
					/*CString urlBase = sNet->m_globalBotUrl;//"http://72.29.77.21/xthreads/chatbot.aspx?botname=$bName&question=$text";     
                    if(urlBase.GetLength() > 1)
					{
						urlBase.Replace("$bName",dataObj->m_text2);
						urlBase.Replace("$text",dataObj->m_text);
						urlBase.Replace("$acid",cLib->ConvToString(dataObj->m_dword2));
						//CString response = "";
						CHttpFileDownloader * urlHanlder = new CHttpFileDownloader();
						DWORD bufLen = 0;
						BYTE * bufferX = NULL;
						if(urlHanlder->GetUrlData("botRequest",urlBase.GetBuffer(),&bufferX,&bufLen))
						{
					        
							if(bufLen > 0)
							{
									CPlayerObject * currentPlayer = sNet->m_netComObject->m_runtimePlayersOnServer->GetByNetID_IndexedAlive(dataObj->m_int);
									if(currentPlayer)
									{
									sNet->SendTextMsg(  DPNSEND_GUARANTEED,
														dataObj->m_dpnid,
														(char*)bufferX,
														0,
														currentPlayer,
														0);//confermation
									}

							}
							delete [] bufferX;
							bufferX = 0;
						}
						delete urlHanlder;
						urlHanlder = 0;
					}*/
				}//end ask a bot a question
				else if(dataObj->m_type == 21)
				{//COMBINESTACKS Command
					sNet->m_eventLog->AddEvent("Unsafe thread block called-todo-protect thread shared variables MiscdataProc 21");//FIX ME - i dont think this is called with just ai but im attempting to narrow down ill be back to rearrange this
					delete dataObj;
				    dataObj = 0;
				    continue;
					//Parse Parameters: Stack1 ID, Stack2 ID
					int comma = dataObj->m_text.Find(",");
					CString stackID1 = dataObj->m_text.Mid(0, comma);
					CString stackID2 = dataObj->m_text.Mid(comma + 2);

					//Get the player object from the NetID
					CPlayerObject *playerObj = classObj->m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->GetByNetID(dataObj->m_int);

					//Grab information for both stacks from SQL
					classObj->m_sharedInterface->BroadcastMessage(0, "SELECT handle, itemID, packID, stackQuantity, stackWeight, isStackable FROM tcInventory WHERE stackID='" + stackID1 + "' OR stackID='" + stackID2 + "'", playerObj->m_3dPosition, playerObj->m_currentWorldID, playerObj->m_networkAssignedID);
					_RecordsetPtr stacks = dbConnection->Query("SELECT handle, itemID, packID, stackQuantity, stackWeight, isStackable FROM tcInventory WHERE stackID='" + stackID1 + "' OR stackID='" + stackID2 + "'");

					//If we don't get exactly 2 results, this is a possible hack attempt
					if(dbConnection->GetCount(stacks) == 2)
					{
						//Pull information from the Recordset
						int handle1		= dbConnection->GetIntData("handle", stacks);
						int itemID1		= dbConnection->GetIntData("itemID", stacks);
						CString packID1	= dbConnection->GetStringData("packID", stacks);
						int quantity1	= dbConnection->GetIntData("stackQuantity", stacks);
						int weight1		= dbConnection->GetIntData("stackWeight", stacks);
						bool stackable	= dbConnection->GetBoolData("isStackable", stacks);
						dbConnection->NextRecord(stacks);
						int handle2		= dbConnection->GetIntData("handle", stacks);
						int itemID2		= dbConnection->GetIntData("itemID", stacks);
						int quantity2	= dbConnection->GetIntData("stackQuantity", stacks);
						int weight2		= dbConnection->GetIntData("stackWeight", stacks);
						dbConnection->Close(stacks);
						
						//Check if items are actually stackable
						if(stackable)
						{
							//If Stacks don't both belong to the player, log error and send "Unstackable" message to client.
							if(playerObj->m_handle == handle1 && playerObj->m_handle == handle2)
							{
								//If itemID of Stack1 is not equal to the ItemID of Stack2, this is a possible hack attempt, because the client engine should be checking this before ever attempting to combine the 2 stacks.  Log it.
								if(itemID1 == itemID2)
								{								
									//Delete the Stack1 entry from the Inventory table, as it is now an empty stack
									classObj->m_sharedInterface->BroadcastMessage(0, "DELETE FROM tcInventory WHERE stackID='" + stackID1 + "'", playerObj->m_3dPosition, playerObj->m_currentWorldID, playerObj->m_networkAssignedID);
									dbConnection->ExecQuery("DELETE FROM tcInventory WHERE stackID='" + stackID1 + "'");
									
									//Update Stack1 data in SQL database moving it onto Stack2
									classObj->m_sharedInterface->BroadcastMessage(0, "UPDATE tcStacks SET stackID='" + stackID2 + "' WHERE stackID='" + stackID1 + "'", playerObj->m_3dPosition, playerObj->m_currentWorldID, playerObj->m_networkAssignedID);
									dbConnection->ExecQuery("UPDATE tcStacks SET stackID='" + stackID2 + "' WHERE stackID='" + stackID1 + "'");
									
									 //Update Stack2's quantity count in the Inventory table:
									classObj->m_sharedInterface->BroadcastMessage(0, "UPDATE tcInventory SET stackQuantity='" + classObj->m_sharedInterface->m_cabLib.ConvToString(quantity1) + " + " + classObj->m_sharedInterface->m_cabLib.ConvToString(quantity2) + "', stackWeight='" + classObj->m_sharedInterface->m_cabLib.ConvToString(weight1) + " + " + classObj->m_sharedInterface->m_cabLib.ConvToString(weight2) + "' WHERE stackID='" + stackID2 + "'", playerObj->m_3dPosition, playerObj->m_currentWorldID, playerObj->m_networkAssignedID);
									dbConnection->ExecQuery("UPDATE tcInventory SET stackQuantity='" + classObj->m_sharedInterface->m_cabLib.ConvToString(quantity1 + quantity2) + "', stackWeight='" + classObj->m_sharedInterface->m_cabLib.ConvToString(weight1 + weight2) + "' WHERE stackID='" + stackID2 + "'");
                                    
									//Update Pack's numSlotsUsed
									if(packID1 == "0")
									{
									}
									else
									{
										classObj->m_sharedInterface->BroadcastMessage(0, "UPDATE tcInventory SET numSlotsUsed=numSlotsUsed-1 WHERE stackID='" + packID1 + "'", playerObj->m_3dPosition, playerObj->m_currentWorldID, playerObj->m_networkAssignedID);
										dbConnection->ExecQuery("UPDATE tcInventory SET numSlotsUsed=numSlotsUsed-1 WHERE stackID='" + packID1 + "'");
									}

									//Add Stack ID2 to the FreeIDs table
									classObj->m_sharedInterface->BroadcastMessage(0, "INSERT INTO tcFreeIDs (freeID, type) VALUES ('" + stackID1 + "', '1')", playerObj->m_3dPosition, playerObj->m_currentWorldID, playerObj->m_networkAssignedID);
									dbConnection->ExecQuery("INSERT INTO tcFreeIDs (freeID, type) VALUES ('" + stackID1 + "', '1')");

									//Send "Stacking Complete" message to client.
									classObj->m_sharedInterface->BroadcastMessage(0, "Stacking Complete", playerObj->m_3dPosition, playerObj->m_currentWorldID, playerObj->m_networkAssignedID);
								}
								else
									classObj->m_sharedInterface->BroadcastMessage(0, "HACKER! Stacks aren't of the same item type", playerObj->m_3dPosition, playerObj->m_currentWorldID, playerObj->m_networkAssignedID);
							}
							else
								classObj->m_sharedInterface->BroadcastMessage(0, "HACKER! Stacks don't both belong to you", playerObj->m_3dPosition, playerObj->m_currentWorldID, playerObj->m_networkAssignedID);
						}
						else
							classObj->m_sharedInterface->BroadcastMessage(0, "HACKER! This item isn't stackable", playerObj->m_3dPosition, playerObj->m_currentWorldID, playerObj->m_networkAssignedID);
					}
					else
						classObj->m_sharedInterface->BroadcastMessage(0, "HACKER! Wrong number of stacks returned.", playerObj->m_3dPosition, playerObj->m_currentWorldID, playerObj->m_networkAssignedID);
				}//End COMBINESTACKS Command
				else if(dataObj->m_type == 22)
				{//SPLITSTACK Command
					
				}//End SPLITSTACK Command
				else if(dataObj->m_type == 23)
				{//DROPSTACK Command
					
				}//End DROPSTACK Command
				else if(dataObj->m_type == 24)
				{//PICKUPSTACK Command
					
				}//End PICKUPSTACK Command
				else if(dataObj->m_type == 25)
				{//DELETESTACK Command
					
				}//End DELETESTACK Command
				else if(dataObj->m_type == 26)
				{//MOVESTACKTOPACK Command
					
				}//End MOVESTACKTOPACK Command
				else if(dataObj->m_type == 27)
				{//ARMSTACK Command
				}//End ARMSTACK Command
				else if(dataObj->m_type == 28)
				{//UNARMSTACK Command
				}//End UNARMSTACK Command
				else if(dataObj->m_type == 29)
				{//create pending zone Command
                    BOOL result = classObj->m_sharedInterface->m_netComObject->m_currentWorldsUsed->AddPendingZone_safe(dataObj->m_text,dataObj->m_int,dataObj->m_miscData,dataObj->m_bufSz);
				    if(!result)
						classObj->m_sharedInterface->m_eventLog->LogByPriority("MiscDataProcesor()AddPendingZone_safe:Could not create pending zone entry:"+dataObj->m_text,(DWORD)dataObj->m_int,2);
				}//End create pending zone Command
				else if(dataObj->m_type == 30)
				{//clear instance
					while(classObj->m_sharedInterface->SpawnFirstFromChannelToRespawn(dataObj->m_int))
					{//rids all people from zone
					}
				}//end clear instance
				else if(dataObj->m_type == 31)
				{//process close instance type rules
					//m_int2   //stencil channel
					//m_int	   //apply to dyn channel-ussually people within that channel
					DWORD param1 = 0;
                    if(classObj->m_sharedInterface->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(dataObj->m_int2,dataObj->m_int,WORLDRULE_RUNSCRIPTONCLOSE,&param1))
					{//run server script
						CString errorReturn;
						CAxList * rPlList = sNet->m_netComObject->m_runtimePlayersOnServer->GetListByChannel(dataObj->m_int);
						// UNLOCK runtime container objects
						sNet->m_netComObject->m_runtimeAccountDB->Lock();
						sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
						for(CAxLinkObj * pos = rPlList->GetHeadPosition_unsafe();pos != NULL;)
						{
							CPlayerObject * plObj = (CPlayerObject *)rPlList->GetNext_unsafe(&pos);
							if(!plObj->m_aiControlled)
							if(plObj->m_energy > 0)//alive only /winners
						    if(!sNet->m_serverScripts->ScriptProcessor(plObj,sizeof(CPlayerObject),param1,errorReturn,sNet))
							{
                                sNet->m_eventLog->LogByPriority("MiscDataProcesor(31):ScriptProcessor Failed:"+errorReturn, 2);
								break;
							}
						}
						sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
						sNet->m_netComObject->m_runtimeAccountDB->Unlock();
					}//end run server script
				}//end process close instance type rules
				else if(dataObj->m_type == 32)
				{//process close script for single/team victors

					DWORD param1 = 0;
					CPlayerObject *pWinningBettor = NULL;
					if(classObj->m_sharedInterface->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(dataObj->m_int2,dataObj->m_int,WORLDRULE_RUNSCRIPTONCLOSE,&param1))
					{//run server script
						CString errorReturn;

						// LOCK runtime list containers
						sNet->m_netComObject->m_runtimeAccountDB->Lock();
						sNet->m_netComObject->m_runtimePlayersOnServer->Lock();

						CAxList * rPlList = sNet->m_netComObject->m_runtimePlayersOnServer->GetListByChannel(dataObj->m_int);
						CWorldAvailableObj *pWorld = NULL;
						for(CAxLinkObj * pos = rPlList->GetHeadPosition_unsafe();pos != NULL;)
						{
							CPlayerObject * plObj = (CPlayerObject *)rPlList->GetNext_unsafe(&pos);
							if(!plObj->m_aiControlled)
							{
								if(plObj->m_arenaInfo[1] > 0)//alive only /winners
								{
									if(!sNet->m_serverScripts->ScriptProcessor(plObj,sizeof(CPlayerObject),param1,errorReturn,sNet))
									{
										sNet->m_eventLog->LogByPriority("MiscDataProcesor(32):ScriptProcessor Failed:"+errorReturn, 2);
										break;
									}
								}
							}
						}

						// UNLOCK runtime container objects
						sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
						sNet->m_netComObject->m_runtimeAccountDB->Unlock();
					}

					
				}//end process clsoe script for single/team victors
				else if(dataObj->m_type == 33)
				{//handle WORLDRULE_DEATHMATCH
					
					sNet->m_netComObject->m_runtimeAccountDB->Lock();
					sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
					sNet->m_netComObject->m_currentWorldsUsed->Lock();
					
					
					int winnerScore = 0;
					int winnerDamage = 0;
					int winnerDeaths = 100000;
					CPlayerObject *currentWinner = NULL,
						*pWinningBettor = NULL;

					//Go through the list to see if we have found a winner
					CWorldAvailableObj *pWorld = NULL;
					CAxList *plList = sNet->m_netComObject->m_runtimePlayersOnServer->GetListByChannel(dataObj->m_int);
					for(CAxLinkObj *pos = plList->GetHeadPosition_unsafe(); pos != NULL;)
					{
						CPlayerObject *plObj = (CPlayerObject *)plList->GetNext_unsafe(&pos);
						if(!plObj
						|| (plObj->m_currentWorldID != dataObj->m_int)
						|| (plObj->m_energy <= 0)
						|| (plObj->m_aiControlled))
							continue;
													
						if(plObj->m_arenaInfo[2] > winnerScore)
						{
							winnerScore = plObj->m_arenaInfo[2];
							winnerDamage = plObj->m_arenaInfo[4];
							winnerDeaths = plObj->m_arenaInfo[3];
							currentWinner = plObj;
						}
						else if(plObj->m_arenaInfo[2] == winnerScore)
						{
							if(plObj->m_arenaInfo[3] < winnerDeaths)
							{
								winnerScore = plObj->m_arenaInfo[2];
								winnerDamage = plObj->m_arenaInfo[4];
								winnerDeaths = plObj->m_arenaInfo[3];
								currentWinner = plObj;
							}
							else if(plObj->m_arenaInfo[3] == currentWinner->m_arenaInfo[3])
							{
								if(plObj->m_arenaInfo[4] >= winnerDamage)
								{
									winnerScore = plObj->m_arenaInfo[2];
									winnerDamage = plObj->m_arenaInfo[4];
									winnerDeaths = plObj->m_arenaInfo[3];
									currentWinner = plObj;
								}
							}
						}

						// FIND the winning arena bettor
						pWorld = sNet->m_netComObject->m_currentWorldsUsed->GetWorldByChannel(plObj->m_currentWorldID);
						if (plObj->m_arenaBetAmount > 0)
						{
							if ((plObj == currentWinner) || (pWorld && (pWorld->m_numArenaBettors == 1)))
								pWinningBettor = plObj;

							// INIT all bettors reward amounts to losing values
							//	NOTE: Winners will have theirs set correctly after the loop
							plObj->m_arenaInfo[5] = -1 * plObj->m_arenaBetAmount;
						}
					}

					// HANDLE arena bet reward if the arena is over
					if (pWinningBettor)
					{
						pWorld = sNet->m_netComObject->m_currentWorldsUsed->GetWorldByChannel(pWinningBettor->m_currentWorldID);
						if (pWorld)
						{
							pWinningBettor->m_arenaInfo[5] = pWorld->m_arenaBetPool;
							sNet->m_currencySystem->AddCurrency(
							  sNet->m_itemManager,
							  pWinningBettor->m_currentInventory,
							  -1,
							  pWorld->m_arenaBetPool);
						}
					}

					//Loop through all the players in the channel again
					for(CAxLinkObj *position = plList->GetHeadPosition_unsafe(); position != NULL;)
					{
						//Acquire each player object
						CPlayerObject *plObj = (CPlayerObject *)plList->GetNext_unsafe(&position);
						ARENA_MESSAGE msg;
						if(!plObj->m_aiControlled) //Make sure this player is not an AI unit to account for
						{
							//Victory Condition Met
							if(plObj->m_arenaInfo[2] == winnerScore
							&& plObj->m_arenaInfo[4] == winnerDamage
							&& plObj->m_arenaInfo[3] == winnerDeaths)
							{
								msg.ArenaMessage = ARENA_VICTORY;
								plObj->m_arenaInfo[1] = 1;
							}
							//Defeat
							else
								msg.ArenaMessage = ARENA_DEFEAT;
							
							//Send message accross and update values
							msg.ArenaType = plObj->m_arenaInfo[0];
							msg.Score = plObj->m_arenaInfo[1];
							msg.Kills = plObj->m_arenaInfo[2];
							msg.Deaths = plObj->m_arenaInfo[3];
							msg.Damage = plObj->m_arenaInfo[4];
							msg.Reward = plObj->m_arenaInfo[5];
							sNet->SendArenaMessage(plObj, msg);

							plObj->m_arenaRespawnPosition.x = 0;
							plObj->m_arenaRespawnPosition.y = 0;
							plObj->m_arenaRespawnPosition.z = 0;
							plObj->m_arenaRespawnWorldName = "Unknown";
						}
					}

					//Close down the arena with appropriate rules
					sNet->m_threadedDataProcessor->AddToQue(dataObj->m_int,dataObj->m_int2,36,0);
					sNet->m_threadedDataProcessor->AddToQue(dataObj->m_int,dataObj->m_int2,32,0);//process closing rules
					sNet->m_threadedDataProcessor->AddToQue(dataObj->m_int,0,30,10000);//close zone in 10 seconds
					
					sNet->m_netComObject->m_currentWorldsUsed->Unlock();
					sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
					sNet->m_netComObject->m_runtimeAccountDB->Unlock();
				}//end handle WORLDRULE_DEATHMATCH
				else if(dataObj->m_type == 34)
				{//handle WORLDRULE_TEAM_DEATHMATCH
					// LOCK runtime object collections
					
					sNet->m_netComObject->m_runtimeAccountDB->Lock();
					sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
					sNet->m_netComObject->m_currentWorldsUsed->Lock();
					

					CPlayerObject *pWinningBettor = NULL;
					int teamOneID = 0, teamOneScore = 0;
					int teamTwoID = 0, teamTwoScore = 0;
					int winningTeam = -1;

					//Acquire the list of players in the channel
					//Go through the list to see if the victory condition has been met
					CAxList *plList = sNet->m_netComObject->m_runtimePlayersOnServer->GetListByChannel(dataObj->m_int);
					for(CAxLinkObj *pos = plList->GetHeadPosition_unsafe(); pos != NULL;)
					{
						//Typecast to each player object
						CPlayerObject *plObj = (CPlayerObject *)plList->GetNext_unsafe(&pos);
						if(!plObj
						|| (plObj->m_currentWorldID != dataObj->m_int)
						|| (plObj->m_energy <= 0)
						|| (plObj->m_aiControlled))
							continue;

						//Set the teams if they havn't been set
						if(!teamOneID)
							teamOneID = plObj->m_teamTemp;
						else if(!teamTwoID)
						{
							if(teamOneID != plObj->m_teamTemp)
								teamTwoID = plObj->m_teamTemp;
						}

						//Tally up the scores to determine a winner
						if(teamOneID == plObj->m_teamTemp)
							teamOneScore += plObj->m_arenaInfo[2];
						else if(teamTwoID == plObj->m_teamTemp)
							teamTwoScore += plObj->m_arenaInfo[2];
					}

					if(teamOneScore > teamTwoScore)
						winningTeam = teamOneID;
					else if(teamTwoScore > teamOneScore)
						winningTeam = teamTwoID;
					else
						winningTeam = -1;

					// FIND winning bettor
					CWorldAvailableObj *pWorld = NULL;
					for(CAxLinkObj *position = plList->GetHeadPosition_unsafe(); position != NULL;)
					{
						//Typecast to each player object
						CPlayerObject *plObj = (CPlayerObject *)plList->GetNext_unsafe(&position);
						if(!plObj
						|| (plObj->m_currentWorldID != dataObj->m_int)
						|| (plObj->m_energy <= 0)
						|| (plObj->m_aiControlled))
							continue;

						// FIND winning arena bettor out of all the betting players on the winning team...
						pWorld = sNet->m_netComObject->m_currentWorldsUsed->GetWorldByChannel(plObj->m_currentWorldID);
						if (plObj->m_arenaBetAmount > 0)
						{
							if (!pWinningBettor
							|| (plObj->m_teamTemp == winningTeam)
							|| (pWorld->m_numArenaBettors == 1))
							{
								pWinningBettor = plObj;
							}
							// IF the current player has more kills than the current winner, update the winner reference
							else if (plObj->m_arenaInfo[2] > pWinningBettor->m_arenaInfo[2])
							{
								pWinningBettor = plObj;
							}
							// IF the current player has the SAME number of KILLS as the current winner,
							//	AND the current player has LESS DEATHS, update the winner reference
							else if ((plObj->m_arenaInfo[2] == pWinningBettor->m_arenaInfo[2])
							&& (plObj->m_arenaInfo[3] < pWinningBettor->m_arenaInfo[3]))
							{
								pWinningBettor = plObj;
							}
							
							// INIT all bettors reward amounts to losing values
							//	NOTE: Winners will have theirs set correctly after the loop
							plObj->m_arenaInfo[5] = -1 * plObj->m_arenaBetAmount;
						}
					}

					// HANDLE arena bet reward if the arena is over
					if (pWinningBettor)
					{
						pWorld = sNet->m_netComObject->m_currentWorldsUsed->GetWorldByChannel(pWinningBettor->m_currentWorldID);
						if (pWorld)
						{
							pWinningBettor->m_arenaInfo[5] = pWorld->m_arenaBetPool;

							sNet->m_currencySystem->AddCurrency(
							  sNet->m_itemManager,
							  pWinningBettor->m_currentInventory,
							  -1,
							  pWorld->m_arenaBetPool);
						}
					}

					//Check for the winning team
					for(CAxLinkObj *position = plList->GetHeadPosition_unsafe(); position != NULL;)
					{
						//Acquire each player object
						CPlayerObject *plObj = (CPlayerObject *)plList->GetNext_unsafe(&position);
						ARENA_MESSAGE msg;
						if(!plObj->m_aiControlled) //Make sure this player is not an AI unit to account for
						{
							//Victory Condition Met
							if(plObj->m_teamTemp == winningTeam)
							{
								msg.ArenaMessage = ARENA_VICTORY;
								plObj->m_arenaInfo[1] = 1;
							}
							//Defeat
							else
								msg.ArenaMessage = ARENA_DEFEAT;
							
							//Send message accross and update values
							msg.ArenaType = plObj->m_arenaInfo[0];
							msg.Score = plObj->m_arenaInfo[1];
							msg.Kills = plObj->m_arenaInfo[2];
							msg.Deaths = plObj->m_arenaInfo[3];
							msg.Damage = plObj->m_arenaInfo[4];
							msg.Reward = plObj->m_arenaInfo[5];
							sNet->SendArenaMessage(plObj, msg);

							plObj->m_arenaRespawnPosition.x = 0;
							plObj->m_arenaRespawnPosition.y = 0;
							plObj->m_arenaRespawnPosition.z = 0;
							plObj->m_arenaRespawnWorldName = "Unknown";
						}
					}
					
					//Close down the arena with appropriate rules
					sNet->m_threadedDataProcessor->AddToQue(dataObj->m_int,dataObj->m_int2,36,0);
					sNet->m_threadedDataProcessor->AddToQue(dataObj->m_int,dataObj->m_int2,32,0);//process closing rules
					sNet->m_threadedDataProcessor->AddToQue(dataObj->m_int,0,30,10000);//close zone in 10 seconds
					
					sNet->m_netComObject->m_currentWorldsUsed->Unlock();
					sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
					sNet->m_netComObject->m_runtimeAccountDB->Unlock();
				}//end handle WORLDRULE_TEAM_DEATHMATCH
				else if(dataObj->m_type == 35)
				{//process open instance type rules for hide inventory
					DWORD param1 = 0;
                    if(classObj->m_sharedInterface->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(dataObj->m_int2,dataObj->m_int,WORLDRULE_RUNSCRIPTONOPEN,&param1))
					{//run server script
						CString errorReturn;
						CAxList * rPlList = sNet->m_netComObject->m_runtimePlayersOnServer->GetListByChannel(dataObj->m_int);
						sNet->m_netComObject->m_runtimeAccountDB->Lock();
						sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
						for(CAxLinkObj * pos = rPlList->GetHeadPosition_unsafe();pos != NULL;)
						{
							CPlayerObject * plObj = (CPlayerObject *)rPlList->GetNext_unsafe(&pos);
							if(!plObj->m_aiControlled)
							{
								if(!sNet->m_serverScripts->ScriptProcessor(plObj,sizeof(CPlayerObject),param1,errorReturn,sNet))
								{
									sNet->m_eventLog->LogByPriority("MiscDataProcesor(35):ScriptProcessor Failed:"+errorReturn, 2);
									break;
								}
								plObj->m_arenaItemsGiven = TRUE;
							}
						}
						sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
						sNet->m_netComObject->m_runtimeAccountDB->Unlock();
					}//end run server script
				}//process open instance type rules for hide inventory
				else if(dataObj->m_type == 36)
				{//setup closing instance rules
					CAxList * rPlList = sNet->m_netComObject->m_runtimePlayersOnServer->GetListByChannel(dataObj->m_int);
					//LOCK runtime container objects
					sNet->m_netComObject->m_runtimeAccountDB->Lock();
					sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
					for(CAxLinkObj * pos = rPlList->GetHeadPosition_unsafe();pos != NULL;)
					{//list check
						CPlayerObject * plObj = (CPlayerObject *)rPlList->GetNext_unsafe(&pos);
						if(!plObj->m_aiControlled)
						{//not AI controlled
							//Make the players invulnerable for the closing rules
							plObj->m_invunerableDuration = 10000;
							plObj->m_invunerableStamp = GetTickCount();
							CAccountObject * accountPtr = sNet->m_netComObject->m_runtimeAccountDB->GetAccountByID(plObj->m_currentID);
							//Go through the account and set the instance type to 0
							if(accountPtr)
							{//valid account
								CPlayerObject *characterOnAccount = accountPtr->GetCurrentCharacter();
								if(characterOnAccount)
								{//valid player
									characterOnAccount->m_currentWorldMap   = characterOnAccount->m_respawnWorldName;        
								    characterOnAccount->m_3dPosition.x      = characterOnAccount->m_respawnPosition.x;
								    characterOnAccount->m_3dPosition.y      = characterOnAccount->m_respawnPosition.y;
								    characterOnAccount->m_3dPosition.z      = characterOnAccount->m_respawnPosition.z;
								    characterOnAccount->m_instanceType = 0;
								}//end valid player
							}//end valid account
						}//end not AI controlled
					}//end list check
					//UNLOCK runtime container objects
					sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
					sNet->m_netComObject->m_runtimeAccountDB->Unlock();
				}//end setup closing instance rules
				
				delete dataObj;
				dataObj = 0;
		   }//end process logons
		}

		delete dbConnection;
	    dbConnection = 0;

		delete authConnection;
		authConnection = 0;
		CoUninitialize();
		
        return 1;
}
//-----------------------------------------------------------//
DWORD WINAPI CharacterAquisition(LPVOID lpParameter)
{
        CServerMain * classObj = (CServerMain *)lpParameter;
     
		if(classObj->m_sharedInterface->m_characterPendingLoader == NULL)
		   return 1;

		if(FAILED(CoInitialize(NULL)))
		{
		   //AfxEndThread(0,TRUE);
           return 0;
		}

        CDBConnectionSystem * dbConnection = new CDBConnectionSystem(classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerIP,
			                                                         classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerCatalog,
																	 classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerUsername,
																	 classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerPassword,
																	 classObj->m_sharedInterface->m_eventLog);
        if(dbConnection->m_connectionEstablished == TRUE)
		{//verify connection established
		   
		   classObj->m_sharedInterface->m_eventLog->AddEvent("Character Processor Thread connected and running");
		}
		else
		{
		   delete dbConnection;
		   dbConnection = 0;
		   classObj->m_sharedInterface->m_eventLog->AddEvent("Character Processor Thread Failed to connect");
		   return 1;//leave because we have no basis to use the database
		}
 
		CString requestSpecificZone		= "SELECT * FROM SYSLIVEZONEDB WHERE ZONENAME = '$ZONENAME' AND INSTANCETYPE = '$INSTANCETYPE' AND CUR_CONNECTIONS < MAX_CONNECTIONS ORDER BY CUR_CONNECTIONS";
		CString characterData			= "SELECT * FROM GMCHARACTERS WHERE USERNAME = '$USER' AND INUSE = '1' AND GAMENAME = '$GAMENAME'";
        CString skillData				= "SELECT * FROM GMSKILLS WHERE HANDLE = '$HANDLE' ORDER BY SKILLTYPE";
		CString statsData				= "SELECT * FROM GMSTATS WHERE HANDLE = '$HANDLE' ORDER BY STATTYPE";
		CString allInventory		    = "SELECT * FROM GMINVENTORY WHERE HANDLE = '$HANDLE' ORDER BY ITEMUNIQUEID";
		CString dimCfgQuerry		    = "SELECT * FROM GMDIMCFGS WHERE HANDLE = '$HANDLE' ORDER BY ORDERINDEX";
		CString GMCOMMCFGQuerry	        = "SELECT * FROM GMCOMMLIST WHERE HANDLE = '$HANDLE' ORDER BY CHARACTERNAME";
		CString journalData	            = "SELECT * FROM GMJOURNAL WHERE HANDLE = '$HANDLE' ORDER BY UNIQUEJOURNALINDEX";
		CString journalReqItemData	    = "SELECT * FROM GMJOURNALREQUIREDITEMS WHERE HANDLE = '$HANDLE'";
		
		CString powerupData				= "SELECT * FROM GMPOWERUPS WHERE HANDLE = '$HANDLE'";
		CString woundData				= "SELECT * FROM GMWOUNDS WHERE HANDLE = '$HANDLE'";
		//CString allCombinedInventory	= "SELECT * FROM GMINVENTORY WHERE HANDLE = '$HANDLE' and UNIQUEIDBIND <> '-1'";

		CString worldNameTemp;
		DWORD logonProcessTimeStamp = 0;
		DWORD totalProcessingTime = 0;
		CCharacterPendingObj * characterToLoad = NULL;
		while(1)
		{
            
			EnterCriticalSection( &classObj->m_interactSection);
			if(classObj->m_systemShutDown)
			{
				classObj->m_shutDownThreadCount++;
			    LeaveCriticalSection( &classObj->m_interactSection);
			    break;
			}
			LeaveCriticalSection( &classObj->m_interactSection);
			
			if(!classObj->m_sharedInterface->m_characterPendingLoader->GetNextCharacterToLoad(&characterToLoad))
			{
				Sleep(500);
                continue;
			}
			else
			    Sleep(10);


			logonProcessTimeStamp = timeGetTime();
			//CString handle = characterToLoad->m_handleString;
			//int handleInt = characterToLoad->m_handle;
			CString temp;
			//add username to temp account system
			
			//CHARACTER DATA
			temp = characterData;
			
            CAccountObject * actPtr = classObj->m_sharedInterface->m_netComObject->m_runtimeAccountDB->GetAccountByUsername_LOS(characterToLoad->m_username.GetBuffer());
			if(actPtr)
			{
			   if(actPtr->m_characterOnAccount)
			   {
			      delete actPtr->m_characterOnAccount;
			      actPtr->m_characterOnAccount = 0;
			   }
			   actPtr->m_characterOnAccount = NULL;
			   
			   temp.Replace("$GAMENAME",actPtr->m_currentlyPlayingGameName);

			   classObj->m_sharedInterface->m_netComObject->m_runtimeAccountDB->Unlock();
			}
			else
			{
				totalProcessingTime = timeGetTime() - logonProcessTimeStamp;
				classObj->m_sharedInterface->m_netComObject->m_eventLog->AddEvent(characterToLoad->m_username+":CharacterAquisition() INVALID AQUISITION",totalProcessingTime);
			    delete characterToLoad;
			    characterToLoad = 0;
		        continue;
			}

 
			//CHARACTER DATA continued
			temp.Replace("$USER",characterToLoad->m_username);
			_RecordsetPtr charPtr = dbConnection->Query(temp); 

			int actualCharacterCount = dbConnection->GetCount(charPtr);
            if(characterToLoad->m_charIndex >= actualCharacterCount)
			{
			   totalProcessingTime = timeGetTime() - logonProcessTimeStamp;
			   classObj->m_sharedInterface->m_netComObject->m_eventLog->AddEvent(characterToLoad->m_username+"CharacterAquisition() INVALID INDEX AQUISITION",totalProcessingTime);
               dbConnection->Close(charPtr);
			   delete characterToLoad;
			   characterToLoad = 0;  
			   continue;
			}

			for(int indexInto=0;indexInto<characterToLoad->m_charIndex;indexInto++)
			{
                if(characterToLoad->m_charIndex == indexInto)
				   break;

				dbConnection->NextRecord(charPtr);
			}

			CString handle = dbConnection->GetStringData("HANDLE",charPtr);
			int handleInt = atoi(handle);

			

			//notification of logon proc send here
			CPlayerObject * playerObject = NULL;

			playerObject								= new CPlayerObject();
			playerObject->m_stats						= new CStatObjList();
			playerObject->m_skills						= new CSkillObjectList();
			playerObject->m_currentInventory			= new CInventoryObjList();
			//playerObject->m_bankInventory				= new CInventoryObjList();
			playerObject->m_reconfigInven               = NULL;//new CInventoryObjList();
			playerObject->m_groupDatabase               = new CCommGroupList();
			playerObject->m_noteriety                   = new CNoterietyObj();
			playerObject->m_questJournal                = new CQuestJournalList();
			
		
			playerObject->m_charName = classObj->m_sharedInterface->SanitizeFormatOut(dbConnection->GetStringData("CHARNAME",charPtr));
			playerObject->m_visName = classObj->m_sharedInterface->SanitizeFormatOut(dbConnection->GetStringData("CHARNAME",charPtr));
			playerObject->m_handle = dbConnection->GetIntData("HANDLE",charPtr);

			playerObject->m_noteriety->m_currentMurders						= dbConnection->GetIntData("MURDERCOUNT",charPtr);
			//playerObject->m_noteriety->m_currentMinuteCountSinceLastMurder	= (long)dbConnection->GetIntData("LASTMURDERMINUTECT",charPtr);
			playerObject->m_noteriety->m_currentMinuteCountSinceLastMurder = 0;

			playerObject->m_deathCount = dbConnection->GetIntData("DEATHCOUNT",charPtr);

			playerObject->m_skills->m_skillToDisplay = dbConnection->GetIntData("SKILLTODISPLAY", charPtr);

			playerObject->m_3dPosition.x = dbConnection->GetfloatData("POSX",charPtr);
			playerObject->m_3dPosition.y = dbConnection->GetfloatData("POSY",charPtr);
			playerObject->m_3dPosition.z = dbConnection->GetfloatData("POSZ",charPtr);

			playerObject->m_respawnPosition.x = dbConnection->GetfloatData("REBIRTHX",charPtr);
			playerObject->m_respawnPosition.y = dbConnection->GetfloatData("REBIRTHY",charPtr);
			playerObject->m_respawnPosition.z = dbConnection->GetfloatData("REBIRTHZ",charPtr);
			playerObject->m_currentWorldMap		= dbConnection->GetStringData("CURRENTWORLDMAP",charPtr);
			playerObject->m_respawnWorldName    = dbConnection->GetStringData("RESPAWNWORLDNAME",charPtr);
			playerObject->m_currentWorldID		= dbConnection->GetIntData("CURRENTWORLDID",charPtr);
			playerObject->m_dbCfg				= dbConnection->GetIntData("DBCFG",charPtr);
			playerObject->m_altCfgIndex			= dbConnection->GetIntData("ALTCFG",charPtr);
			playerObject->m_stasis				= dbConnection->GetIntData("CFGSTASIS",charPtr);
			playerObject->m_curEDBIndex         = dbConnection->GetIntData("CURDBCFG",charPtr);
			playerObject->m_originalEDBCfg      = dbConnection->GetIntData("ORIGINALDBCFG",charPtr);
			playerObject->m_energy              = dbConnection->GetIntData("ENERGY",charPtr);
			playerObject->m_arenaBasePoints     = dbConnection->GetIntData("ARENAPOINTS",charPtr);
			playerObject->m_pvpKills			= dbConnection->GetIntData("PVPKILLS",charPtr);
			playerObject->m_arenaKills			= dbConnection->GetIntData("ARENAKILLS",charPtr);
			playerObject->m_instanceType		= dbConnection->GetIntData("INSTANCETYPE",charPtr);
			playerObject->m_groupID				= dbConnection->GetIntData("GROUPNUMBER",charPtr);
			playerObject->m_clanHandle			= dbConnection->GetIntData("CLANID",charPtr);
			playerObject->m_factionStanding		= dbConnection->GetIntData("FACTION",charPtr);
			playerObject->m_groupStruct.m_groupLeadership = dbConnection->GetIntData("ISSQUADLEADER",charPtr);
			playerObject->m_groupStruct.m_groupPlacementIndex = dbConnection->GetIntData("SQUADPLACEMENT",charPtr);
			playerObject->m_squadName			= dbConnection->GetStringData("SQUADNAME",charPtr);


			if(playerObject->m_clanHandle > 0)//verify clan exists or unbind
			if(!classObj->m_sharedInterface->m_clanCommSys.DoesClanExist_safe(dbConnection->GetStringData("CLANID",charPtr),&playerObject->m_clanAbbrev,dbConnection))
				playerObject->m_clanHandle = -1;//unlink clan id becuase it doesnt exist anymore

			//Store the original EDB energy
			CMovementObj * EDBPtr = classObj->m_sharedInterface->m_entObjList->GetObjByIndex(playerObject->m_curEDBIndex);
			if(EDBPtr)
				playerObject->m_originalEDBEnergy = EDBPtr->List_maxEnergy;


			CMovementObj * DBPtr = classObj->m_sharedInterface->m_entObjList->GetObjByIndex(playerObject->m_dbCfg);
			if(!DBPtr)
			{//invalid db cfg ptr
			   dbConnection->Close(charPtr);
               delete characterToLoad;
			   characterToLoad = 0;  
               continue;
			}//end invalid db cfg ptr

			dbConnection->Close(charPtr);

			/* TODO: This to be implemented for Player Quest Development!
			// UPDATE character's in-game status
			temp = "UPDATE gmCharacters SET isInGame = 1 WHERE charName = '" + playerObject->m_charName + "'";
			bool success = dbConnection->ExecQuery(temp); 
			if (!success)
			{
				classObj->m_sharedInterface->m_eventLog->LogByPriority("FAILED to update the " + playerObject->m_charName + "'s in-game status!", 2);
			}
			*/

			//STATS
			temp = statsData;
			temp.Replace("$HANDLE",handle);
			_RecordsetPtr statDBPtr = dbConnection->Query(temp); 
			int statCount = dbConnection->GetCount(statDBPtr);
			if(DBPtr->m_stats)
			for(int sLoop=0;sLoop<statCount;sLoop++)
			{
				int statType		= dbConnection->GetIntData("STATTYPE",statDBPtr);
				int currentAmount	= dbConnection->GetIntData("CURRENTAMOUNT",statDBPtr);
				
				CStatObj * statObj = NULL;
				CStatObj * statObjOrig = DBPtr->m_stats->GetStatByID(statType);
				if(statObjOrig)
				{
				   statObjOrig->Clone(&statObj);
				   statObj->m_currentValue		= (float)currentAmount;
				   playerObject->m_stats->AddTail(statObj);
				}
				dbConnection->NextRecord(statDBPtr);
			}
			dbConnection->Close(statDBPtr);
			//SKILLS
			temp = skillData;
			temp.Replace("$HANDLE",handle);
			_RecordsetPtr skillDBPtr = dbConnection->Query(temp); 
			int skillCount = dbConnection->GetCount(skillDBPtr);
			int sLoop=0;
			for(sLoop=0;sLoop<skillCount;sLoop++)
			{
				int skillType		= dbConnection->GetIntData("SKILLTYPE",skillDBPtr);
				float currentExperience	= dbConnection->GetfloatData("CURRENTEXPERIENCE",skillDBPtr);
				CSkillObject * skillObj = new CSkillObject();

                

				skillObj->m_skillType				= skillType;
				skillObj->m_currentExperience		= currentExperience;

				playerObject->m_skills->AddTail(skillObj);
				dbConnection->NextRecord(skillDBPtr);
			}
			dbConnection->Close(skillDBPtr);

			//POWERUPS
			temp = powerupData;
			temp.Replace("$HANDLE", handle);
			_RecordsetPtr powerupDBPtr = dbConnection->Query(temp);
			int powerupCount = dbConnection->GetCount(powerupDBPtr);
			if (powerupCount <= MAX_BONUS)
			{ 
				for(int iCount = 0; iCount < powerupCount; iCount++)
				{
					int tracker = 0;
					
					if(dbConnection->GetIntData("BONUSTYPE", powerupDBPtr) > 0 && dbConnection->GetIntData("BONUSTYPE", powerupDBPtr) < 5)
						tracker = 1;
					if(dbConnection->GetIntData("BONUSTYPE", powerupDBPtr) > 5)
						tracker = 2;
					if(dbConnection->GetIntData("BONUSTYPE", powerupDBPtr) == 5)
						tracker = 0;

					playerObject->m_bonus[tracker].m_bonusType = dbConnection->GetIntData("BONUSTYPE", powerupDBPtr);
					playerObject->m_bonus[tracker].m_damageMult = dbConnection->GetfloatData("DAMAGEMULT", powerupDBPtr);
					playerObject->m_bonus[tracker].m_bonusDuration = dbConnection->GetIntData("BONUSDURATION", powerupDBPtr);
					playerObject->m_bonus[tracker].m_bonusAmount = dbConnection->GetIntData("BONUSAMOUNT", powerupDBPtr);
					playerObject->m_bonus[tracker].m_maxDuration = dbConnection->GetIntData("MAXDURATION", powerupDBPtr);
				
					dbConnection->NextRecord(powerupDBPtr);
				}
			} // End Valid Number of Powerups
			
			dbConnection->Close(powerupDBPtr);

			//Wounds
			temp = woundData;
			temp.Replace("$HANDLE", handle);
			_RecordsetPtr woundDBPtr = dbConnection->Query(temp);
			int woundAmount = dbConnection->GetCount(woundDBPtr);
			if(woundAmount)
			{
				int woundRows = (woundAmount > 8) ? 8 : woundAmount;//used to always read 8 rows, past the end of the results
				for(int woundCount = 0; woundCount < woundRows; woundCount++)
				{
					int tracker = 0;
					tracker = dbConnection->GetIntData("WOUNDCHANNEL", woundDBPtr);
					if(tracker >= 0 && tracker < 8)//m_woundChannel has 8 slots - a bad channel wrote over other player data
						playerObject->m_woundChannel[tracker] = dbConnection->GetIntData("WOUNDAMOUNT", woundDBPtr);
					
					dbConnection->NextRecord(woundDBPtr);
				}
			}

			dbConnection->Close(woundDBPtr);


			// GET the max storage index for bank inventories
			/*CString strMaxStorageIndex = "SELECT MAX(STORAGEINDEX) AS MAXINDEX FROM GMINVENTORY WHERE STORAGETYPE = 1";
			_RecordsetPtr rsInventoryMaxIndex = dbConnection->Query(strMaxStorageIndex);
			int iMaxStorageIndex = dbConnection->GetIntData("MAXINDEX", rsInventoryMaxIndex);
			dbConnection->Close(rsInventoryMaxIndex);*/

			// INIT all player's banks
			CInventoryObjList *pInventory = NULL;
			for (int i=0; i<MAX_BANKS; i++)
			{
				pInventory = new CInventoryObjList();
				playerObject->m_banks.push_back(pInventory);
			}

			//INVENTORY ALL TYPES
			temp = allInventory;
			temp.Replace("$HANDLE",handle);
			_RecordsetPtr inventoryDBPtr = dbConnection->Query(temp); 
			int invCount = dbConnection->GetCount(inventoryDBPtr);
			int itemCountOnCharacter = 0;
			CItemObj * gPtr = NULL;
			for(sLoop=0;sLoop<invCount;sLoop++)
			{
				int storageType			= dbConnection->GetIntData("STORAGETYPE",inventoryDBPtr);//eObj->m_miscInt2
				int storageIndex		= dbConnection->GetIntData("STORAGEINDEX",inventoryDBPtr);
				int itemUniqueID	    = dbConnection->GetIntData("ITEMUNIQUEID",inventoryDBPtr);
				int itemGilType			= dbConnection->GetIntData("ITEMGILTYPE",inventoryDBPtr);//eObj->m_iconIndex
				int uniqueIDBind        = dbConnection->GetIntData("UNIQUEIDBIND",inventoryDBPtr);//
				int currentCharges      = dbConnection->GetIntData("CURRENTCHARGES",inventoryDBPtr);//eObj->m_miscInt
				int quanity				= dbConnection->GetIntData("QUANITY",inventoryDBPtr);//eObj->m_xPos
				int armed				= dbConnection->GetIntData("ARMED",inventoryDBPtr);//eObj->m_yPos
				int miscInt1			= dbConnection->GetIntData("MISCINT1",inventoryDBPtr);//eObj->m_miscInt2
				float miscFloat1		= dbConnection->GetfloatData("MISCFLOAT1",inventoryDBPtr);//eObj->m_xPos
				float miscFloat2		= dbConnection->GetfloatData("MISCFLOAT2",inventoryDBPtr);//eObj->m_yPos
				int dmgMod		        = dbConnection->GetIntData("DMGMOD",inventoryDBPtr);//eObj->m_yPos
				int missileLoaded		= dbConnection->GetIntData("LOADEDMISSILE",inventoryDBPtr);
				
				switch(storageType)
				{
					case 0://on character
					{					
						if(uniqueIDBind > 0)
						{//handle combination items
							CInventoryObj * invPtr = (CInventoryObj *)playerObject->m_currentInventory->GetByUniqueSync(uniqueIDBind);
							if(invPtr)
							{//combine to item

							   if(!invPtr->m_itemData->m_expansiveCollection)
                                   invPtr->m_itemData->m_expansiveCollection = new CCExpansiveObjList();

							   if(!invPtr->m_itemData->m_expansiveCollection->GetByMisc2(miscInt1))
							   {
									CExpansiveObj * expObj = new CExpansiveObj();
									expObj->m_iconIndex = itemGilType;
									expObj->m_miscInt   = currentCharges;
									expObj->m_miscInt2  = miscInt1;
									expObj->m_xPos      = miscFloat1;
									expObj->m_yPos      = miscFloat2;

									invPtr->m_itemData->m_expansiveCollection->AddTail(expObj);
							   }
							
							}//end combine to item
						}//end handle combination items
						else
						{//add normal item
							if(itemCountOnCharacter <= classObj->m_sharedInterface->m_netComObject->m_maxItemLimit_onChar)
							{//inventory not maxed yet
								gPtr = classObj->m_sharedInterface->m_itemManager->GetItemByID_LOS(itemGilType);
								if(gPtr)
								{
									CInventoryObj * invPtr = playerObject->m_currentInventory->AddInventoryItem(quanity,armed,gPtr);
									if(invPtr)
									{//set specifics
										invPtr->m_itemData->m_chargeCurrent		= currentCharges;
										invPtr->m_itemData->m_missileLoaded		= missileLoaded;
										invPtr->m_tempUniqueSync				= itemUniqueID;
										invPtr->m_itemData->m_damageModifier	= dmgMod;
										if(!invPtr->m_itemData->m_permanent)
											itemCountOnCharacter++;
									}//end set specifics
									classObj->m_sharedInterface->m_itemManager->Unlock();
								}
							}//end inventory not maxed yet
							else//severe item count exploit not adding anymore items here logging priority 1 log
							   classObj->m_sharedInterface->m_eventLog->LogByPriority("*** OVERLOADED EXPLOIT MUST FIX *** "+playerObject->m_charName,(DWORD)itemCountOnCharacter,1);
						}//end add normal item
						break;
					}
					case 1://in bank
					{
						if (storageIndex < 0 || storageIndex >= (int)playerObject->m_banks.size())
							break;//bad bank slot in the database - .at() threw and crashed the server on login
						CInventoryObjList *pBank = playerObject->m_banks[storageIndex];
						if (!pBank) break;

						if(uniqueIDBind > 0)
						{//handle combination items
							CInventoryObj * invPtr = (CInventoryObj *)pBank->GetByUniqueSync(uniqueIDBind);
							if(invPtr)
							{//combine to item

								if(!invPtr->m_itemData->m_expansiveCollection)
									invPtr->m_itemData->m_expansiveCollection = new CCExpansiveObjList();

								if(!invPtr->m_itemData->m_expansiveCollection->GetByMisc2(miscInt1))
								{
										CExpansiveObj * expObj = new CExpansiveObj();
										expObj->m_iconIndex = itemGilType;
										expObj->m_miscInt   = currentCharges;
										expObj->m_miscInt2  = miscInt1;
										expObj->m_xPos      = miscFloat1;
										expObj->m_yPos      = miscFloat2;

										invPtr->m_itemData->m_expansiveCollection->AddTail(expObj);
								}
							}//end combine to item
						}//end handle combination items
						else
						{//add normal item
							gPtr = classObj->m_sharedInterface->m_itemManager->GetItemByID_LOS(itemGilType);
							if(gPtr)
							{
								CInventoryObj * invPtr = pBank->AddInventoryItem(quanity,armed,gPtr);
								if(invPtr)
								{//set specifics
									invPtr->m_itemData->m_chargeCurrent			= currentCharges;
									invPtr->m_itemData->m_missileLoaded			= missileLoaded;
									invPtr->m_tempUniqueSync					= itemUniqueID;
									invPtr->m_itemData->m_damageModifier		= dmgMod;
								}//end set specifics
								classObj->m_sharedInterface->m_itemManager->Unlock();
							}
						}//end add normal item
						/*if(uniqueIDBind > 0)
						{//handle combination items
							CInventoryObj * invPtr = (CInventoryObj *)playerObject->m_bankInventory->GetByUniqueSync(uniqueIDBind);
							if(invPtr)
							{//combine to item

								if(!invPtr->m_itemData->m_expansiveCollection)
									invPtr->m_itemData->m_expansiveCollection = new CCExpansiveObjList();

								if(!invPtr->m_itemData->m_expansiveCollection->GetByMisc2(miscInt1))
								{
										CExpansiveObj * expObj = new CExpansiveObj();
										expObj->m_iconIndex = itemGilType;
										expObj->m_miscInt   = currentCharges;
										expObj->m_miscInt2  = miscInt1;
										expObj->m_xPos      = miscFloat1;
										expObj->m_yPos      = miscFloat2;

										invPtr->m_itemData->m_expansiveCollection->AddTail(expObj);
								}
							}//end combine to item
						}//end handle combination items
						else
						{//add normal item
							gPtr = classObj->m_sharedInterface->m_itemManager->GetItemByID_LOS(itemGilType);
							if(gPtr)
							{
								CInventoryObj * invPtr = playerObject->m_bankInventory->AddInventoryItem(quanity,armed,gPtr);
								if(invPtr)
								{//set specifics
									invPtr->m_itemData->m_chargeCurrent			= currentCharges;
									invPtr->m_itemData->m_missileLoaded			= missileLoaded;
									invPtr->m_tempUniqueSync					= itemUniqueID;
									invPtr->m_itemData->m_damageModifier		= dmgMod;
								}//end set specifics
								classObj->m_sharedInterface->m_itemManager->Unlock();
							}
						}//end add normal item*/
						break;
					}
					// Reconfig inventory (for shapeshifting) 
					//	AND hidden inventory (for WORLDRULE_HIDE_INVENTORY_ON_SPAWN)
					case 2:
					{
                        if(!playerObject->m_reconfigInven)
							playerObject->m_reconfigInven = new CInventoryObjList();

						if(uniqueIDBind > 0)
						{//handle combination items
							CInventoryObj * invPtr = (CInventoryObj *)playerObject->m_reconfigInven->GetByUniqueSync(uniqueIDBind);
							if(invPtr)
							{//combine to item

							   if(!invPtr->m_itemData->m_expansiveCollection)
                                  invPtr->m_itemData->m_expansiveCollection = new CCExpansiveObjList();

                               if(!invPtr->m_itemData->m_expansiveCollection->GetByMisc2(miscInt1))
							   {
									CExpansiveObj * expObj = new CExpansiveObj();
									expObj->m_iconIndex = itemGilType;
									expObj->m_miscInt   = currentCharges;
									expObj->m_miscInt2  = miscInt1;
									expObj->m_xPos      = miscFloat1;
									expObj->m_yPos      = miscFloat2;

									invPtr->m_itemData->m_expansiveCollection->AddTail(expObj);
							   }
							}//end combine to item
						}//end handle combination items
						else
						{//add normal item
							gPtr = classObj->m_sharedInterface->m_itemManager->GetItemByID_LOS(itemGilType);
                            if(gPtr)
					        {
								CInventoryObj * invPtr = playerObject->m_reconfigInven->AddInventoryItem(quanity,armed,gPtr);
								if(invPtr)
								{//set specifics
									invPtr->m_itemData->m_chargeCurrent = currentCharges;
									invPtr->m_itemData->m_missileLoaded	= missileLoaded;
									invPtr->m_tempUniqueSync = itemUniqueID;
									invPtr->m_itemData->m_damageModifier = dmgMod;
								}//end set specifics
								classObj->m_sharedInterface->m_itemManager->Unlock();
							}
						}//end add normal item
						break;
					}
				}
				dbConnection->NextRecord(inventoryDBPtr);
			}
			dbConnection->Close(inventoryDBPtr);
            //-------------Dim Cfg---------------//
			temp = dimCfgQuerry;
			temp.Replace("$HANDLE",handle);
			_RecordsetPtr dimCfg = dbConnection->Query(temp); 
			int dimCount = dbConnection->GetCount(dimCfg);
			
			if(dimCount > 0)
			{
			   playerObject->m_deformableMeshCount = dimCount;
               playerObject->m_deformableCfg = new int[dimCount];
               for(int cfgLoop=0;cfgLoop<dimCount;cfgLoop++)
			   {
				   playerObject->m_deformableCfg[cfgLoop] = dbConnection->GetIntData("DIMCFGINDEX",dimCfg);//eObj->m_miscInt2
				   dbConnection->NextRecord(dimCfg);
			   }
			}
			dbConnection->Close(dimCfg);
			
			//----------------Quest Journal----------------------//
			//playerObject->m_questJournal
			temp = journalData;
			temp.Replace("$HANDLE",handle);
			_RecordsetPtr jnReader = dbConnection->Query(temp); 
			int jnCount = dbConnection->GetCount(jnReader);
			if(jnCount > 0)
			{
               for(int cfgLoop=0;cfgLoop<jnCount;cfgLoop++)
			   {
				   CQuestJournalObj * journalPtr = new CQuestJournalObj();
                   journalPtr->m_questDescription			= classObj->m_sharedInterface->SanitizeFormatOut(dbConnection->GetStringData("questDescription",jnReader));
				   journalPtr->m_npcName					= classObj->m_sharedInterface->SanitizeFormatOut(dbConnection->GetStringData("npcName",jnReader));
                   journalPtr->m_keyStatement				= classObj->m_sharedInterface->SanitizeFormatOut(dbConnection->GetStringData("keyStatement",jnReader));
				   journalPtr->m_currentProgression         = dbConnection->GetIntData("currentProgression",jnReader);
				   journalPtr->m_status						= dbConnection->GetIntData("status",jnReader);
				   journalPtr->m_mobToHuntID				= dbConnection->GetIntData("mobToHuntID",jnReader);
				   journalPtr->m_maxDropCount				= dbConnection->GetIntData("maxDropCount",jnReader);
				   journalPtr->m_dropGlid					= dbConnection->GetIntData("dropGlid",jnReader);
				   journalPtr->m_dropPercentage				= dbConnection->GetIntData("dropPercentage",jnReader);
				   journalPtr->m_lastSelectionIndex			= dbConnection->GetIntData("lastSelectionIndex",jnReader);
				   journalPtr->m_undeletable				= dbConnection->GetIntData("undeletable",jnReader);
				   journalPtr->m_autoComplete				= dbConnection->GetIntData("autoComplete",jnReader);
				   journalPtr->m_skillExpType				= dbConnection->GetIntData("skillExpType",jnReader);
				   journalPtr->m_expBonus					= dbConnection->GetfloatData("expBonus",jnReader);
				   journalPtr->m_thisGainCap				= dbConnection->GetfloatData("thisGainCap",jnReader);
				   int uniqueJnlIndex						= dbConnection->GetIntData("uniqueJournalIndex",jnReader);
                   //get required items
                   journalPtr->m_requiredItems				= new CQJRequiredItemList();
				   playerObject->m_questJournal->AddTail(journalPtr);

				   dbConnection->NextRecord(jnReader);
			   }
			}
			dbConnection->Close(jnReader);

			//journal required items
				temp = journalReqItemData;
				temp.Replace("$HANDLE",handle);
				//temp.Replace("$UNIQUEINDEX",classObj->m_sharedInterface->m_cabLib.ConvToString(uniqueJnlIndex));
				_RecordsetPtr jnItemReader = dbConnection->Query(temp); 
				int jnItemCount = dbConnection->GetCount(jnItemReader);
				int currentJnlIndex = -1;
				CQuestJournalObj * currentJournalPtr = NULL;
				for(int iJloop=0;iJloop<jnItemCount;iJloop++)
				{
					int uniqueJnlIndex = dbConnection->GetIntData("uniqueIndex",jnItemReader);

					if(currentJnlIndex != uniqueJnlIndex)
					{
					currentJournalPtr = playerObject->m_questJournal->GetByIndex(uniqueJnlIndex);
					currentJnlIndex = uniqueJnlIndex;
					}
					if(currentJournalPtr)
					{//add items
					CQJRequiredItem * reqItemPtr = new CQJRequiredItem();
					reqItemPtr->m_glid = dbConnection->GetIntData("glid",jnItemReader);
					reqItemPtr->m_quanity = dbConnection->GetIntData("quanity",jnItemReader);
					reqItemPtr->m_logType = dbConnection->GetIntData("LOGTYPE",jnItemReader);

					gPtr = classObj->m_sharedInterface->m_itemManager->GetItemByID_LOS(reqItemPtr->m_glid);
					if(gPtr)
					{
					   reqItemPtr->m_itemName = gPtr->m_itemName;
					   classObj->m_sharedInterface->m_itemManager->Unlock();
					}
						
					currentJournalPtr->m_requiredItems->AddTail(reqItemPtr);
					}//end add items
					dbConnection->NextRecord(jnItemReader);
				}
				dbConnection->Close(jnItemReader);
				//test


            //redirect if a target instance type zone doesnt exist anymore ie:an arena
			if(playerObject->m_instanceType > 0)
			{
				CString ztemp = requestSpecificZone;
				ztemp.Replace("$ZONENAME",playerObject->m_currentWorldMap);
				ztemp.Replace("$INSTANCETYPE",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_instanceType));
	  
				_RecordsetPtr availZones = dbConnection->Query(ztemp);
				int availZoneCount = dbConnection->GetCount(availZones);

				if(availZoneCount == 0)//no zones with this instance type need to redirect player
				{//reroute to rebirth point 
					playerObject->m_instanceType			= 0;
				    playerObject->m_currentWorldMap			= playerObject->m_respawnWorldName;
					playerObject->m_3dPosition				= playerObject->m_respawnPosition;	

					classObj->m_sharedInterface->m_eventLog->LogByPriority("Rerouted: "+playerObject->m_charName,2);
				}//end reroute to rebirth point
				dbConnection->Close(availZones);
			}
			
			//-------------add to account runtime system---------//
            actPtr = classObj->m_sharedInterface->m_netComObject->m_runtimeAccountDB->GetAccountByUsername_LOS(characterToLoad->m_username.GetBuffer());
			if(actPtr)
			{
			   actPtr->m_characterOnAccount = playerObject;
			   actPtr->m_currentcharInUse = 0;
			   
			   classObj->m_sharedInterface->m_netComObject->SendSpawnMsg(DPNSEND_GUARANTEED,
                                                                         actPtr->m_currentID,//idFromLoc,
								                                         (short)playerObject->m_curEDBIndex,
								                                         playerObject->m_currentWorldMap,
																		 playerObject->m_altCfgIndex,
																		 (short)playerObject->m_3dPosition.x,
																		 (short)playerObject->m_3dPosition.y,
																		 (short)playerObject->m_3dPosition.z);
			   classObj->m_sharedInterface->m_netComObject->m_runtimeAccountDB->Unlock();
			}
			else
			{
                classObj->m_sharedInterface->m_eventLog->LogByPriority("Character Aquisition* player must have logged off between character aquisition():"+characterToLoad->m_username,2);
				delete playerObject;
				playerObject = 0;
			}
			
            totalProcessingTime = timeGetTime() - logonProcessTimeStamp;
			classObj->m_sharedInterface->m_netComObject->m_eventLog->LogByPriority(characterToLoad->m_username+":CharacterAquisition() Aquisition complete",totalProcessingTime,3);

			delete characterToLoad;
		    characterToLoad = 0;
		}//end processor loop
		delete dbConnection;
		dbConnection = 0;
		CoUninitialize();

		return 1;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
DWORD WINAPI CharacterDataWriter(LPVOID lpParameter)
{
        CServerMain * classObj = (CServerMain *)lpParameter;

		if(FAILED(CoInitialize(NULL)))
		{
		   //AfxEndThread(0,TRUE);
           return 0;
		}

		CDBConnectionSystem * authConnection = new CDBConnectionSystem(	classObj->m_sharedInterface->m_authenticationDBConnection.m_sqlServerIP,
																		classObj->m_sharedInterface->m_authenticationDBConnection.m_sqlServerCatalog,
																		classObj->m_sharedInterface->m_authenticationDBConnection.m_sqlServerUsername,
																		classObj->m_sharedInterface->m_authenticationDBConnection.m_sqlServerPassword,
																		classObj->m_sharedInterface->m_eventLog);
        if(authConnection->m_connectionEstablished == TRUE)
		{//verify connection established
		   
		   classObj->m_sharedInterface->m_eventLog->AddEvent("MiscDataProcessor Thread connected and running");
		}
		else
		{
		   delete authConnection;
		   authConnection = 0;
		   classObj->m_sharedInterface->m_eventLog->AddEvent("MiscDataProcessor Thread Failed to connect **auth db");
		   return 1;//leave because we have no basis to use the database
		}
		
        CDBConnectionSystem * dbConnection = new CDBConnectionSystem(classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerIP,
			                                                         classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerCatalog,
																	 classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerUsername,
																	 classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerPassword,
																	 classObj->m_sharedInterface->m_eventLog);
        if(dbConnection->m_connectionEstablished == TRUE)
		{//verify connection established

		   classObj->m_sharedInterface->m_eventLog->AddEvent("Character Writer Thread connected and running");
		}
		else
		{
		   delete dbConnection;
		   dbConnection=0;
		   classObj->m_sharedInterface->m_eventLog->AddEvent("Character Writer Thread Failed to connect");
		   return 1;//leave because we have no basis to use the database
		}

		CString loggOffStatusBasic	    = "UPDATE GMACCOUNTBASE SET LOGGEDON = '$LOGGEDON' WHERE USERNAME = '$USER'";
		CString loggOffStatus		    = "UPDATE GMACCOUNTBASE WITH (ROWLOCK) SET LOGGEDON = '$LOGGEDON', MINUTESPLAYED = '$MINUTESPLAYED' WHERE USERNAME = '$USER'";
		CString characterData			= "SELECT * FROM GMCHARACTERS WHERE HANDLE = '$HANDLE'";//check if character already exists
		CString updateCharacterData		= "UPDATE GMCHARACTERS SET ARENAPOINTS = '$ARENAPOINTS', CLANID = '$CLANID', POSX = '$POSX', POSY = '$POSY', POSZ = '$POSZ', REBIRTHX = '$REBIRTHX', REBIRTHY = '$REBIRTHY', REBIRTHZ = '$REBIRTHZ', CURRENTWORLDID = '$CURRENTWORLDID', CURRENTWORLDMAP = '$CURRENTWORLDMAP', DBCFG = '$DBCFG', ALTCFG = '$ALTCFG', CFGSTASIS = '$CFGSTASIS', CURDBCFG = '$CURDBCFG', CHARNAME = '$CHARNAME', INUSE = '1', MURDERCOUNT = '$MURDERCOUNT',LASTMURDERMINUTECT = '$LASTMURDERMINUTECT',ORIGINALDBCFG = '$ORIGINALDBCFG', ENERGY = '$ENERGY', DEATHCOUNT = '$DEATHCOUNT', SKILLTODISPLAY = '$SKILLTODISPLAY', INSTANCETYPE = '$INSTANCETYPE',RESPAWNWORLDNAME = '$RESPAWNWORLDNAME', GROUPNUMBER = '$GROUPNUMBER', ARENAKILLS = '$ARENAKILLS', PVPKILLS = '$PVPKILLS', FACTION = '$FACTION', ISSQUADLEADER = '$ISSQUADLEADER', SQUADPLACEMENT = '$SQUADPLACEMENT', SQUADNAME = '$SQUADNAME' WHERE HANDLE = '$HANDLE'";//check if character already exists
		CString insertCharacterData		= "INSERT INTO GMCHARACTERS (HANDLE, USERNAME, CHARNAME, CLANID, POSX, POSY, POSZ, REBIRTHX, REBIRTHY, REBIRTHZ, RESPAWNWORLDNAME, CURRENTWORLDID, CURRENTWORLDMAP, DBCFG, ALTCFG, CFGSTASIS, CURDBCFG, INUSE, MURDERCOUNT, LASTMURDERMINUTECT,ORIGINALDBCFG, DEATHCOUNT, SKILLTODISPLAY, GROUPNUMBER, ARENAKILLS, PVPKILLS, FACTION, ISSQUADLEADER, SQUADPLACEMENT, SQUADNAME) Values('$HANDLE','$USERNAME','$CHARNAME','$CLANID','$POSX','$POSY','$POSZ','$REBIRTHX','$REBIRTHY','$REBIRTHZ','$RESPAWNWORLDNAME','$CURRENTWORLDID','$CURRENTWORLDMAP','$DBCFG','$ALTCFG','$CFGSTASIS','$CURDBCFG','1','$MURDERCOUNT','$LASTMURDERMINUTECT','$ORIGINALDBCFG','$DEATHCOUNT', '$SKILLTODISPLAY', '$GROUPNUMBER', '$ARENAKILLS', '$PVPKILLS', '$FACTION', '$ISSQUADLEADER', '$SQUADPLACEMENT', '$SQUADNAME')";
        CString insertSkill				= "INSERT INTO GMSKILLS (HANDLE, SKILLTYPE, CURRENTEXPERIENCE, NEWFLAG) Values('$HANDLE','$SKILLTYPE','$CUREXP','1')";
        CString insertStats				= "INSERT INTO GMSTATS (HANDLE, STATTYPE, CURRENTAMOUNT, NEWFLAG) Values('$HANDLE','$STATTYPE','$CURAMOUNT','1')";
		CString deleteOldSkills			= "DELETE FROM GMSKILLS WHERE HANDLE = '$HANDLE' AND NEWFLAG = '0'";
		CString deleteOldStats			= "DELETE FROM GMSTATS WHERE HANDLE = '$HANDLE' AND NEWFLAG = '0'";
		CString updateSkillsFlags	    = "UPDATE GMSKILLS SET NEWFLAG = '0' WHERE HANDLE = '$HANDLE'";
        CString updateSStatsFlags	    = "UPDATE GMSTATS SET NEWFLAG = '0' WHERE HANDLE = '$HANDLE'";
		CString insertInventory	        = "INSERT INTO GMINVENTORY (HANDLE, ITEMUNIQUEID, ITEMGILTYPE, UNIQUEIDBIND, CURRENTCHARGES, STORAGETYPE, STORAGEINDEX, ARMED, QUANITY,NEWFLAG,MISCINT1,MISCFLOAT1,MISCFLOAT2,DMGMOD,LOADEDMISSILE) Values('$HANDLE','$ITEMUNIQUEID','$ITEMGILTYPE','$UNIQUEIDBIND','$CURRENTCHARGES','$STORAGETYPE','$STORAGEINDEX','$ARMED','$QUANITY','1','$MISCINT1','$MISCFLOAT1','$MISCFLOAT2','$DMGMOD','$LOADEDMISSILE')";
		CString deleteOldInventory		= "DELETE FROM GMINVENTORY WHERE HANDLE = '$HANDLE' AND NEWFLAG = '0'";
		CString updateInventoryFlags	= "UPDATE GMINVENTORY SET NEWFLAG = '0' WHERE HANDLE = '$HANDLE'";
		CString insertDimCFG	        = "INSERT INTO GMDIMCFGS (HANDLE, DIMCFGINDEX, NEWFLAG, ORDERINDEX) Values('$HANDLE','$DIMCFGINDEX','1','$ORDERINDEX')";
		CString deleteOldDimCfgs		= "DELETE FROM GMDIMCFGS WHERE HANDLE = '$HANDLE' AND NEWFLAG = '0'";
		CString updateDimCfgFlags	    = "UPDATE GMDIMCFGS SET NEWFLAG = '0' WHERE HANDLE = '$HANDLE'";
		CString insertGMCOMMCFG	        = "INSERT INTO GMCOMMLIST (HANDLE, HANDLEOFCONTACT, CHARACTERNAME, NEWFLAG) Values('$HANDLE','$CONTACTOFHANDLE','$CHARACTERNAME','1')";
		CString deleteOldGMCOMMCfgs		= "DELETE FROM GMCOMMLIST WHERE HANDLE = '$HANDLE' AND NEWFLAG = '0'";
		CString updateGMCOMMCfgsFlags	= "UPDATE GMCOMMLIST SET NEWFLAG = '0' WHERE HANDLE = '$HANDLE'";
		CString deleteJournal			= "DELETE FROM GMJOURNAL WHERE HANDLE = '$HANDLE' AND NEWFLAG = '0'";
		CString insertJournal			= "INSERT INTO GMJOURNAL (HANDLE, QUESTDESCRIPTION, NPCNAME, KEYSTATEMENT, CURRENTPROGRESSION, STATUS, MOBTOHUNTID, MAXDROPCOUNT, DROPGLID, DROPPERCENTAGE, LASTSELECTIONINDEX, UNDELETABLE, AUTOCOMPLETE, SKILLEXPTYPE, EXPBONUS, THISGAINCAP, UNIQUEJOURNALINDEX, NEWFLAG) Values('$HANDLE','$QUESTDESCRIPTION','$NPCNAME','$KEYSTATEMENT','$CURRENTPROGRESSION','$STATUS','$MOBTOHUNTID','$MAXDROPCOUNT','$DROPGLID','$DROPPERCENTAGE','$LASTSELECTIONINDEX','$UNDELETABLE','$AUTOCOMPLETE','$SKILLEXPTYPE','$EXPBONUS','$THISGAINCAP','$UNIQUEJOURNALINDEX','1')";
		CString updateJournalFlags	    = "UPDATE GMJOURNAL SET NEWFLAG = '0' WHERE HANDLE = '$HANDLE'";
		CString deleteJournalReqItems   = "DELETE FROM GMJOURNALREQUIREDITEMS WHERE HANDLE = '$HANDLE' AND NEWFLAG = '0'";
		CString insertJournalReqItem	= "INSERT INTO GMJOURNALREQUIREDITEMS (HANDLE, GLID, QUANITY, NEWFLAG,UNIQUEINDEX,LOGTYPE) Values('$HANDLE','$GLID','$QUANITY','1','$UNIQUEINDEX','$LOGTYPE')";
		CString updateJnReqItemFlags	= "UPDATE GMJOURNALREQUIREDITEMS SET NEWFLAG = '0' WHERE HANDLE = '$HANDLE'";

		// Handle Powerups
		CString insertPowerup			= "INSERT INTO GMPOWERUPS (HANDLE, BONUSTYPE, DAMAGEMULT, BONUSDURATION, BONUSAMOUNT, MAXDURATION, NEWFLAG) Values('$HANDLE', '$BONUSTYPE', '$DAMAGEMULT', '$BONUSDURATION', '$BONUSAMOUNT', '$MAXDURATION', '1')";
		CString deleteOldPowerup		= "DELETE FROM GMPOWERUPS WHERE HANDLE = '$HANDLE' AND NEWFLAG = '0'";
		CString updatePowerupFlags		= "UPDATE GMPOWERUPS SET NEWFLAG = '0' WHERE HANDLE = '$HANDLE'";

		//Handle Wounds
		CString insertWound				= "INSERT INTO GMWOUNDS (HANDLE, WOUNDCHANNEL, WOUNDAMOUNT, NEWFLAG) Values('$HANDLE', '$WOUNDCHANNEL', '$WOUNDAMOUNT', '1')";
		CString deleteOldWound			= "DELETE FROM GMWOUNDS WHERE HANDLE = '$HANDLE' AND NEWFLAG = '0'";
		CString updateWoundFlags		= "UPDATE GMWOUNDS SET NEWFLAG = '0' WHERE HANDLE = '$HANDLE'";

		//---------------------------------------------------------------//
		//--------responsible for cleaning up logged out accounts--------//
		//--------and backing them up to the DB--------------------------//
		//---------------------------------------------------------------//
		CCharacterPendingSaveObj * pendingObject = NULL;
		DWORD logonProcessTimeStamp = 0;
		DWORD totalProcessingTime = 0;
		while(1)
		{//processor loop
			    
			    EnterCriticalSection( &classObj->m_interactSection);
			    if(classObj->m_systemShutDown)
			    {
				   classObj->m_shutDownThreadCount++;
			       LeaveCriticalSection( &classObj->m_interactSection);
			       break;
			    }
			    LeaveCriticalSection( &classObj->m_interactSection);

			    
			    if(!classObj->m_sharedInterface->m_netComObject->m_characterPendingSaveSystem->GetNextCharacterToLoad(&pendingObject))
				{
				    Sleep(500);
                    continue;
				}
				else
					Sleep(10);

				logonProcessTimeStamp = timeGetTime();
				CString temp = "";
				if(pendingObject->playerRef == NULL)
				if(pendingObject->m_logOff == 1)//if log out requested
				{//logs cgaracter
					classObj->m_sharedInterface->m_eventLog->AddEvent("Logging off Flag CDW_1:"+pendingObject->m_text);
					temp = loggOffStatus;
				    temp.Replace("$LOGGEDON","0");
				    temp.Replace("$USER",pendingObject->m_text);
					int minutesPlayedTotal = 0;
				    int result = classObj->m_sharedInterface->m_netComObject->m_runtimeAccountDB->LogOutRuntime_safe(  pendingObject->m_text,&minutesPlayedTotal);
				    temp.Replace("$MINUTESPLAYED",classObj->m_sharedInterface->m_cabLib.ConvToString(minutesPlayedTotal));
					if(result == 1)
					{//account not found
						classObj->m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Account not found LogOutRuntime()-will not rec minutes:"+pendingObject->m_text);
					    //just flag log off status
					    temp = loggOffStatusBasic;
						temp.Replace("$LOGGEDON","0");
				        temp.Replace("$USER",pendingObject->m_text);
					    authConnection->ExecQuery(temp);
					}//end account not found
					else{//success
					    authConnection->ExecQuery(temp);
					}//end success
					
					totalProcessingTime = timeGetTime() - logonProcessTimeStamp;
					classObj->m_sharedInterface->m_netComObject->m_eventLog->AddEvent("CharDataWriter() player not in game log out request 1:"+pendingObject->m_text,totalProcessingTime);
					
					delete pendingObject;
					pendingObject = 0;
					continue;
				}//end logs cgaracter
                

                CPlayerObject * playerObject = pendingObject->playerRef;
				//CAccountObject * acPtr = (CAccountObject*)playerObject->m_accountReference;

                CString handleSTR = classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_handle);
				CString clanIDSTR = classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_clanHandle);
				//see if character exists
				temp = characterData;
				temp.Replace("$HANDLE",handleSTR);
                _RecordsetPtr charReader = dbConnection->Query(temp);
                int count = dbConnection->GetCount(charReader);
				dbConnection->Close(charReader);

				// ------------------ Check for logging out with damaging dots still in affect ----------------- //
				int logOutDamage = 0;
				if(playerObject->m_dotList)
				{
					if(playerObject->m_dotList->m_currentDotAmount > 0)
					{
						for(POSITION posLoc = playerObject->m_dotList->GetHeadPosition(); posLoc != NULL;)
						{
							CDotClass *dotObj = (CDotClass *)playerObject->m_dotList->GetNext(posLoc);
							if(dotObj->m_dotType == DOTTYPE_DO_DAMAGE)
							{
								//calculate how much damage logoutdamage will be
								logOutDamage += dotObj->m_dotAmount * (dotObj->m_dotDuration * 0.0002);
							}
						}
						
						playerObject->m_energy -= logOutDamage;
						if(playerObject->m_energy < 0)
						{
							playerObject->m_energy = 0;
							playerObject->m_deathCount++;
						}
					}
				}
				// ------------------ End Check for logging out with damaging dots still in affect ------------- //
				
				if(count > 0)
				{//existing
					classObj->m_sharedInterface->SendTextMsg(DPNSEND_GUARANTEED, playerObject->m_currentID, "Writing Existing: " + classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_skills->m_skillToDisplay), 0, playerObject, 0);
					temp = updateCharacterData;
					temp.Replace("$HANDLE",handleSTR);
					temp.Replace("$ENERGY",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_energy));
					temp.Replace("$DEATHCOUNT",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_deathCount));
					temp.Replace("$SKILLTODISPLAY",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_skills->m_skillToDisplay));
					temp.Replace("$CLANID",clanIDSTR);
					temp.Replace("$CHARNAME",classObj->m_sharedInterface->SanitizeFormatIn(playerObject->m_charName));
					temp.Replace("$POSX",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(playerObject->m_3dPosition.x));
					temp.Replace("$POSY",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(playerObject->m_3dPosition.y));
					temp.Replace("$POSZ",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(playerObject->m_3dPosition.z));
					temp.Replace("$REBIRTHX",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(playerObject->m_respawnPosition.x));
					temp.Replace("$REBIRTHY",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(playerObject->m_respawnPosition.y));
					temp.Replace("$REBIRTHZ",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(playerObject->m_respawnPosition.z));
					temp.Replace("$RESPAWNWORLDNAME",playerObject->m_respawnWorldName);
					temp.Replace("$CURRENTWORLDID",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_currentWorldID));
					temp.Replace("$CURRENTWORLDMAP",playerObject->m_currentWorldMap);
					temp.Replace("$DBCFG",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_dbCfg));
					temp.Replace("$ALTCFG",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_altCfgIndex));
					temp.Replace("$CFGSTASIS",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_stasis));
					temp.Replace("$CURDBCFG",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_curEDBIndex));
					temp.Replace("$ORIGINALDBCFG",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_originalEDBCfg));
					temp.Replace("$ARENAPOINTS",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_arenaBasePoints));
					temp.Replace("$PVPKILLS",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_pvpKills));
					temp.Replace("$ARENAKILLS",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_arenaKills));
					temp.Replace("$INSTANCETYPE",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_instanceType));
					temp.Replace("$GROUPNUMBER",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_groupID));
					temp.Replace("$FACTION",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_factionStanding));
					temp.Replace("$ISSQUADLEADER", classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_groupStruct.m_groupLeadership));
					temp.Replace("$SQUADPLACEMENT", classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_groupStruct.m_groupPlacementIndex));
					temp.Replace("$SQUADNAME",playerObject->m_squadName);
				   

					//classObj->m_sharedInterface->m_netComObject->m_eventLog->LogByPriority("world:"+playerObject->m_respawnWorldName,2);
					//classObj->m_sharedInterface->m_netComObject->m_eventLog->LogByPriority("InstanceType characterDataWritter:",(DWORD)playerObject->m_instanceType,2);//remove
					

					if(playerObject->m_noteriety)
					{
						temp.Replace("$MURDERCOUNT",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_noteriety->m_currentMurders));
						temp.Replace("$LASTMURDERMINUTECT",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_noteriety->m_currentMinuteCountSinceLastMurder));
					}
					else
					{
						temp.Replace("$MURDERCOUNT","0");
						temp.Replace("$LASTMURDERMINUTECT","0");
					}


				   dbConnection->ExecQuery(temp);
				}//end existing;
				else
				{//add new character to DB USERNAME, CHARNAME, HANDLE, CLANID
					classObj->m_sharedInterface->SendTextMsg(DPNSEND_GUARANTEED, playerObject->m_currentID, "Writing New: " + classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_skills->m_skillToDisplay), 0, playerObject, 0);
                   temp = insertCharacterData;
				   temp.Replace("$USERNAME",classObj->m_sharedInterface->SanitizeFormatIn(pendingObject->m_text));
				   temp.Replace("$CHARNAME",classObj->m_sharedInterface->SanitizeFormatIn(playerObject->m_charName));
				   temp.Replace("$HANDLE",handleSTR);
				   temp.Replace("$CLANID",clanIDSTR);
				   temp.Replace("$POSX",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(playerObject->m_3dPosition.x));
				   temp.Replace("$POSY",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(playerObject->m_3dPosition.y));
				   temp.Replace("$POSZ",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(playerObject->m_3dPosition.z));
				   temp.Replace("$REBIRTHX",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(playerObject->m_respawnPosition.x));
				   temp.Replace("$REBIRTHY",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(playerObject->m_respawnPosition.y));
				   temp.Replace("$REBIRTHZ",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(playerObject->m_respawnPosition.z));
				   temp.Replace("$RESPAWNWORLDNAME",playerObject->m_respawnWorldName);
				   temp.Replace("$CURRENTWORLDID",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_currentWorldID));
				   temp.Replace("$CURRENTWORLDMAP",playerObject->m_currentWorldMap);
				   temp.Replace("$DBCFG",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_dbCfg));
				   temp.Replace("$ALTCFG",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_altCfgIndex));
				   temp.Replace("$CFGSTASIS",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_stasis));
				   temp.Replace("$CURDBCFG",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_curEDBIndex));
				   temp.Replace("$ORIGINALDBCFG",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_dbCfg));//m_originalEDBCfg
				   temp.Replace("$DEATHCOUNT",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_deathCount));
				   temp.Replace("$SKILLTODISPLAY",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_skills->m_skillToDisplay));
				   temp.Replace("$GROUPNUMBER",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_groupID));
				   temp.Replace("$ISSQUADLEADER", classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_groupStruct.m_groupLeadership));
				   temp.Replace("$SQUADPLACEMENT", classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_groupStruct.m_groupPlacementIndex));
				   temp.Replace("$SQUADNAME",playerObject->m_squadName);
				   temp.Replace("$INSTANCETYPE",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_instanceType));

				   if(playerObject->m_noteriety)
				   {
                      temp.Replace("$MURDERCOUNT",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_noteriety->m_currentMurders));
                      temp.Replace("$LASTMURDERMINUTECT",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_noteriety->m_currentMinuteCountSinceLastMurder));
				   }
				   else
				   {
                      temp.Replace("$MURDERCOUNT","0");
                      temp.Replace("$LASTMURDERMINUTECT","0");
				   }

				   dbConnection->ExecQuery(temp);
				}//end add new character to db
				//add skills '$HANDLE','$SKILLTYPE','$CUREXP'
				if(playerObject->m_skills)
				for(POSITION skillPos = playerObject->m_skills->GetHeadPosition();skillPos != NULL;)
				{
                    CSkillObject * skillPtr = (CSkillObject *) playerObject->m_skills->GetNext(skillPos);
				    temp = insertSkill;
				    temp.Replace("$HANDLE",handleSTR);
				    temp.Replace("$SKILLTYPE",classObj->m_sharedInterface->m_cabLib.ConvToString(skillPtr->m_skillType));
					temp.Replace("$CUREXP",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(skillPtr->m_currentExperience));
				    dbConnection->ExecQuery(temp);
				}
				//delete old skills and set new flags skills
				temp = deleteOldSkills;
				temp.Replace("$HANDLE",handleSTR);
				dbConnection->ExecQuery(temp);
				temp = updateSkillsFlags;
				temp.Replace("$HANDLE",handleSTR);
				dbConnection->ExecQuery(temp);
				//add stats $HANDLE','$STATTYPE','$CURAMOUNT'
				if(playerObject->m_stats)
				for(POSITION skillPos = playerObject->m_stats->GetHeadPosition();skillPos != NULL;)
				{
                    CStatObj * statPtr = (CStatObj *) playerObject->m_stats->GetNext(skillPos);
				    temp = insertStats;
				    temp.Replace("$HANDLE",handleSTR);
				    temp.Replace("$STATTYPE",classObj->m_sharedInterface->m_cabLib.ConvToString(statPtr->m_id));
					temp.Replace("$CURAMOUNT",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(statPtr->m_currentValue));
				    dbConnection->ExecQuery(temp);
				}
				//delete old stats and set new flags stats
				temp = deleteOldStats;
				temp.Replace("$HANDLE",handleSTR);
				dbConnection->ExecQuery(temp);
				temp = updateSStatsFlags;
				temp.Replace("$HANDLE",handleSTR);
				dbConnection->ExecQuery(temp);

				// POWERUP INFO
				for(int i = 0; i < MAX_BONUS; i++)
				{
					if(playerObject->m_bonus[i].m_bonusType > 0)
					{
						temp = insertPowerup;
						temp.Replace("$HANDLE",handleSTR);
						temp.Replace("$BONUSTYPE", classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_bonus[i].m_bonusType));
						temp.Replace("$DAMAGEMULT", classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(playerObject->m_bonus[i].m_damageMult));
						temp.Replace("$BONUSDURATION", classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_bonus[i].m_bonusDuration));
						temp.Replace("$BONUSAMOUNT", classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_bonus[i].m_bonusAmount));
						temp.Replace("$MAXDURATION", classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_bonus[i].m_maxDuration));
						dbConnection->ExecQuery(temp);
					}
				}
				//delete old powerups and set flags
				temp = deleteOldPowerup;
				temp.Replace("$HANDLE",handleSTR);
				dbConnection->ExecQuery(temp);
				temp = updatePowerupFlags;
				temp.Replace("$HANDLE",handleSTR);
				dbConnection->ExecQuery(temp);

				//--------------Wound Info-----------------//
				for(int i = 0; i < 8; i++)
				{
					temp = insertWound;
					temp.Replace("$HANDLE",handleSTR);
					temp.Replace("$WOUNDCHANNEL", classObj->m_sharedInterface->m_cabLib.ConvToString(i));
					temp.Replace("$WOUNDAMOUNT", classObj->m_sharedInterface->m_cabLib.ConvToString((int)playerObject->m_woundChannel[i]));
					dbConnection->ExecQuery(temp);
				}
				//delete old wounds and set flags
				temp = deleteOldWound;
				temp.Replace("$HANDLE",handleSTR);
				dbConnection->ExecQuery(temp);
				temp = updateWoundFlags;
				temp.Replace("$HANDLE",handleSTR);
				dbConnection->ExecQuery(temp);
				//----------end Wound Info-----------------//
				int uniqueIDGEN = 1;//generate by index
				if(playerObject->m_currentInventory)
				for(POSITION skillPos = playerObject->m_currentInventory->GetHeadPosition();skillPos != NULL;)
				{//INVENTORY LOOP
                    CInventoryObj * invPtr = (CInventoryObj *) playerObject->m_currentInventory->GetNext(skillPos);
					if(!invPtr->m_itemData)
						continue;
				    
					//calculate combined items
                    int expansiveCount = 0;
					if(invPtr->m_itemData->m_expansiveCollection)
					   expansiveCount = (int)invPtr->m_itemData->m_expansiveCollection->GetCount();

					if(invPtr->m_itemData->m_useType == 5 || expansiveCount < 1)
					{
					   temp = insertInventory;
				       temp.Replace("$HANDLE",handleSTR);
					   temp.Replace("$UNIQUEIDBIND","0");
					   temp.Replace("$ITEMUNIQUEID",classObj->m_sharedInterface->m_cabLib.ConvToString(uniqueIDGEN));
					   temp.Replace("$ITEMGILTYPE",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_itemData->m_globalID));
					   temp.Replace("$CURRENTCHARGES",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_itemData->m_chargeCurrent));
					   temp.Replace("$STORAGETYPE","0");
					   temp.Replace("$STORAGEINDEX","0");
					   temp.Replace("$ARMED",classObj->m_sharedInterface->m_cabLib.ConvToString((int)invPtr->m_armed));
					   temp.Replace("$QUANITY",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_quanity));
					   temp.Replace("$MISCINT1","0");
					   temp.Replace("$MISCFLOAT1","0");
					   temp.Replace("$MISCFLOAT2","0");
					   temp.Replace("$DMGMOD",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_itemData->m_damageModifier));
					   temp.Replace("$LOADEDMISSILE",classObj->m_sharedInterface->m_cabLib.ConvToString((int)invPtr->m_itemData->m_missileLoaded));
					   
					   dbConnection->ExecQuery(temp);
					   uniqueIDGEN++;
					}
					else
					{//contains stuff
					   temp = insertInventory;
					   CString uniqueIDGENSTRING = classObj->m_sharedInterface->m_cabLib.ConvToString(uniqueIDGEN);
				       temp.Replace("$HANDLE",handleSTR);
                       temp.Replace("$UNIQUEIDBIND","0");
					   temp.Replace("$ITEMUNIQUEID",uniqueIDGENSTRING);
					   temp.Replace("$ITEMGILTYPE",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_itemData->m_globalID));
					   temp.Replace("$CURRENTCHARGES",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_itemData->m_chargeCurrent));
					   temp.Replace("$STORAGETYPE","0");
					   temp.Replace("$STORAGEINDEX","0");
					   temp.Replace("$ARMED",classObj->m_sharedInterface->m_cabLib.ConvToString((int)invPtr->m_armed));
					   temp.Replace("$QUANITY",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_quanity));
					   temp.Replace("$MISCINT1","0");
					   temp.Replace("$MISCFLOAT1","0");
					   temp.Replace("$MISCFLOAT2","0");
					   temp.Replace("$DMGMOD",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_itemData->m_damageModifier));
					   temp.Replace("$LOADEDMISSILE",classObj->m_sharedInterface->m_cabLib.ConvToString((int)invPtr->m_itemData->m_missileLoaded));
					   dbConnection->ExecQuery(temp);
					   uniqueIDGEN++;

                       for(POSITION expPos = invPtr->m_itemData->m_expansiveCollection->GetHeadPosition();expPos != NULL;)
					   {
                           CExpansiveObj * eObj = (CExpansiveObj *) invPtr->m_itemData->m_expansiveCollection->GetNext(expPos);
						   temp = insertInventory;
				           temp.Replace("$HANDLE",handleSTR);
						   temp.Replace("$UNIQUEIDBIND",uniqueIDGENSTRING);
					       temp.Replace("$ITEMUNIQUEID",classObj->m_sharedInterface->m_cabLib.ConvToString(uniqueIDGEN));
					       temp.Replace("$ITEMGILTYPE",classObj->m_sharedInterface->m_cabLib.ConvToString(eObj->m_iconIndex));
					       temp.Replace("$CURRENTCHARGES",classObj->m_sharedInterface->m_cabLib.ConvToString(eObj->m_miscInt));
					       temp.Replace("$STORAGETYPE","0");
						   temp.Replace("$STORAGEINDEX","0");
					       temp.Replace("$ARMED","0");
					       temp.Replace("$QUANITY","0");
						   temp.Replace("$MISCINT1",classObj->m_sharedInterface->m_cabLib.ConvToString(eObj->m_miscInt2));
						   temp.Replace("$MISCFLOAT1",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(eObj->m_xPos));
					       temp.Replace("$MISCFLOAT2",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(eObj->m_yPos));
						   temp.Replace("$DMGMOD",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_itemData->m_damageModifier));
						   temp.Replace("$LOADEDMISSILE",classObj->m_sharedInterface->m_cabLib.ConvToString((int)invPtr->m_itemData->m_missileLoaded));
					       dbConnection->ExecQuery(temp);
                           uniqueIDGEN++;
					   }
					}//end contains stuff
				}//END INVENTORY LOOP

                //--------------Bank Inventory--------------//
                //if(playerObject->m_bankInventory)
				for (int i=0; i<playerObject->m_banks.size(); i++)
				{
					// GET the current bank
					CInventoryObjList *pBankInventory = playerObject->m_banks[i];
					if (!pBankInventory) continue;

					// LOOP through the banks contents
					for(POSITION pos = pBankInventory->GetHeadPosition(); pos != NULL;)
					{
						CInventoryObj *pItem = (CInventoryObj*)pBankInventory->GetNext(pos);
						if(!pItem->m_itemData) continue;
					    
						//calculate combined items
						int expansiveCount = 0;
						if(pItem->m_itemData->m_expansiveCollection)
						   expansiveCount = (int)pItem->m_itemData->m_expansiveCollection->GetCount();

						if(pItem->m_itemData->m_useType == 5 || expansiveCount < 1)
						{
						   temp = insertInventory;
						   temp.Replace("$HANDLE",handleSTR);
						   temp.Replace("$UNIQUEIDBIND","0");
						   temp.Replace("$ITEMUNIQUEID",classObj->m_sharedInterface->m_cabLib.ConvToString(uniqueIDGEN));
						   temp.Replace("$ITEMGILTYPE",classObj->m_sharedInterface->m_cabLib.ConvToString(pItem->m_itemData->m_globalID));
						   temp.Replace("$CURRENTCHARGES",classObj->m_sharedInterface->m_cabLib.ConvToString(pItem->m_itemData->m_chargeCurrent));
						   temp.Replace("$STORAGETYPE","1");
						   temp.Replace("$STORAGEINDEX", classObj->m_sharedInterface->m_cabLib.ConvToString(i));
						   temp.Replace("$ARMED",classObj->m_sharedInterface->m_cabLib.ConvToString((int)pItem->m_armed));
						   temp.Replace("$QUANITY",classObj->m_sharedInterface->m_cabLib.ConvToString(pItem->m_quanity));
						   temp.Replace("$MISCINT1","0");
						   temp.Replace("$MISCFLOAT1","0");
						   temp.Replace("$MISCFLOAT2","0");
						   temp.Replace("$DMGMOD",classObj->m_sharedInterface->m_cabLib.ConvToString(pItem->m_itemData->m_damageModifier));
						   temp.Replace("$LOADEDMISSILE",classObj->m_sharedInterface->m_cabLib.ConvToString((int)pItem->m_itemData->m_missileLoaded));
						   dbConnection->ExecQuery(temp);
						   uniqueIDGEN++;
						}
						else
						{//contains stuff
						   temp = insertInventory;
						   CString uniqueIDGENSTRING = classObj->m_sharedInterface->m_cabLib.ConvToString(uniqueIDGEN);
						   temp.Replace("$HANDLE",handleSTR);
						   temp.Replace("$UNIQUEIDBIND","0");
						   temp.Replace("$ITEMUNIQUEID",uniqueIDGENSTRING);
						   temp.Replace("$ITEMGILTYPE",classObj->m_sharedInterface->m_cabLib.ConvToString(pItem->m_itemData->m_globalID));
						   temp.Replace("$CURRENTCHARGES",classObj->m_sharedInterface->m_cabLib.ConvToString(pItem->m_itemData->m_chargeCurrent));
						   temp.Replace("$STORAGETYPE","1");
						   temp.Replace("$STORAGEINDEX", classObj->m_sharedInterface->m_cabLib.ConvToString(i));
						   temp.Replace("$ARMED",classObj->m_sharedInterface->m_cabLib.ConvToString((int)pItem->m_armed));
						   temp.Replace("$QUANITY",classObj->m_sharedInterface->m_cabLib.ConvToString(pItem->m_quanity));
						   temp.Replace("$MISCINT1","0");
						   temp.Replace("$MISCFLOAT1","0");
						   temp.Replace("$MISCFLOAT2","0");
						   temp.Replace("$DMGMOD",classObj->m_sharedInterface->m_cabLib.ConvToString(pItem->m_itemData->m_damageModifier));
						   temp.Replace("$LOADEDMISSILE",classObj->m_sharedInterface->m_cabLib.ConvToString((int)pItem->m_itemData->m_missileLoaded));
						   dbConnection->ExecQuery(temp);
						   
						   uniqueIDGEN++;

						   for(POSITION expPos = pItem->m_itemData->m_expansiveCollection->GetHeadPosition();expPos != NULL;)
						   {
							   CExpansiveObj * eObj = (CExpansiveObj *) pItem->m_itemData->m_expansiveCollection->GetNext(expPos);
							   temp = insertInventory;
							   temp.Replace("$HANDLE",handleSTR);
							   temp.Replace("$UNIQUEIDBIND",uniqueIDGENSTRING);
							   temp.Replace("$ITEMUNIQUEID",classObj->m_sharedInterface->m_cabLib.ConvToString(uniqueIDGEN));
							   temp.Replace("$ITEMGILTYPE",classObj->m_sharedInterface->m_cabLib.ConvToString(eObj->m_iconIndex));
							   temp.Replace("$CURRENTCHARGES",classObj->m_sharedInterface->m_cabLib.ConvToString(eObj->m_miscInt));
							   temp.Replace("$STORAGETYPE","1");
							   temp.Replace("$STORAGEINDEX", classObj->m_sharedInterface->m_cabLib.ConvToString(i));
							   temp.Replace("$ARMED","0");
							   temp.Replace("$QUANITY","0");
							   temp.Replace("$MISCINT1",classObj->m_sharedInterface->m_cabLib.ConvToString(eObj->m_miscInt2));
							   temp.Replace("$MISCFLOAT1",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(eObj->m_xPos));
							   temp.Replace("$MISCFLOAT2",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(eObj->m_yPos));
							   temp.Replace("$DMGMOD","0");
							   temp.Replace("$LOADEDMISSILE",classObj->m_sharedInterface->m_cabLib.ConvToString((int)pItem->m_itemData->m_missileLoaded));
							   dbConnection->ExecQuery(temp);
							   uniqueIDGEN++;
						   }
						}//end contains stuff
					}//END BANK INVENTORY LOOP
				}
				/*for(POSITION skillPos = playerObject->m_bankInventory->GetHeadPosition();skillPos != NULL;)
				{//BANK INVENTORY LOOP
                    CInventoryObj * invPtr = (CInventoryObj *) playerObject->m_bankInventory->GetNext(skillPos);
					if(!invPtr->m_itemData)
						continue;
				    
					//calculate combined items
                    int expansiveCount = 0;
					if(invPtr->m_itemData->m_expansiveCollection)
					   expansiveCount = (int)invPtr->m_itemData->m_expansiveCollection->GetCount();

					if(invPtr->m_itemData->m_useType == 5 || expansiveCount < 1)
					{
					   temp = insertInventory;
				       temp.Replace("$HANDLE",handleSTR);
					   temp.Replace("$UNIQUEIDBIND","0");
					   temp.Replace("$ITEMUNIQUEID",classObj->m_sharedInterface->m_cabLib.ConvToString(uniqueIDGEN));
					   temp.Replace("$ITEMGILTYPE",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_itemData->m_globalID));
					   temp.Replace("$CURRENTCHARGES",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_itemData->m_chargeCurrent));
					   temp.Replace("$STORAGETYPE","1");
					   temp.Replace("$ARMED",classObj->m_sharedInterface->m_cabLib.ConvToString((int)invPtr->m_armed));
					   temp.Replace("$QUANITY",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_quanity));
					   temp.Replace("$MISCINT1","0");
					   temp.Replace("$MISCFLOAT1","0");
					   temp.Replace("$MISCFLOAT2","0");
					   temp.Replace("$DMGMOD",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_itemData->m_damageModifier));
					   temp.Replace("$LOADEDMISSILE",classObj->m_sharedInterface->m_cabLib.ConvToString((int)invPtr->m_itemData->m_missileLoaded));
					   dbConnection->ExecQuery(temp);
					   uniqueIDGEN++;
					}
					else
					{//contains stuff
					   temp = insertInventory;
					   CString uniqueIDGENSTRING = classObj->m_sharedInterface->m_cabLib.ConvToString(uniqueIDGEN);
				       temp.Replace("$HANDLE",handleSTR);
                       temp.Replace("$UNIQUEIDBIND","0");
					   temp.Replace("$ITEMUNIQUEID",uniqueIDGENSTRING);
					   temp.Replace("$ITEMGILTYPE",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_itemData->m_globalID));
					   temp.Replace("$CURRENTCHARGES",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_itemData->m_chargeCurrent));
					   temp.Replace("$STORAGETYPE","1");
					   temp.Replace("$ARMED",classObj->m_sharedInterface->m_cabLib.ConvToString((int)invPtr->m_armed));
					   temp.Replace("$QUANITY",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_quanity));
					   temp.Replace("$MISCINT1","0");
					   temp.Replace("$MISCFLOAT1","0");
					   temp.Replace("$MISCFLOAT2","0");
					   temp.Replace("$DMGMOD",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_itemData->m_damageModifier));
					   temp.Replace("$LOADEDMISSILE",classObj->m_sharedInterface->m_cabLib.ConvToString((int)invPtr->m_itemData->m_missileLoaded));
					   dbConnection->ExecQuery(temp);
					   
					   uniqueIDGEN++;

                       for(POSITION expPos = invPtr->m_itemData->m_expansiveCollection->GetHeadPosition();expPos != NULL;)
					   {
                           CExpansiveObj * eObj = (CExpansiveObj *) invPtr->m_itemData->m_expansiveCollection->GetNext(expPos);
						   temp = insertInventory;
				           temp.Replace("$HANDLE",handleSTR);
						   temp.Replace("$UNIQUEIDBIND",uniqueIDGENSTRING);
					       temp.Replace("$ITEMUNIQUEID",classObj->m_sharedInterface->m_cabLib.ConvToString(uniqueIDGEN));
					       temp.Replace("$ITEMGILTYPE",classObj->m_sharedInterface->m_cabLib.ConvToString(eObj->m_iconIndex));
					       temp.Replace("$CURRENTCHARGES",classObj->m_sharedInterface->m_cabLib.ConvToString(eObj->m_miscInt));
					       temp.Replace("$STORAGETYPE","1");
					       temp.Replace("$ARMED","0");
					       temp.Replace("$QUANITY","0");
						   temp.Replace("$MISCINT1",classObj->m_sharedInterface->m_cabLib.ConvToString(eObj->m_miscInt2));
						   temp.Replace("$MISCFLOAT1",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(eObj->m_xPos));
					       temp.Replace("$MISCFLOAT2",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(eObj->m_yPos));
						   temp.Replace("$DMGMOD","0");
						   temp.Replace("$LOADEDMISSILE",classObj->m_sharedInterface->m_cabLib.ConvToString((int)invPtr->m_itemData->m_missileLoaded));
					       dbConnection->ExecQuery(temp);
                           uniqueIDGEN++;
					   }
					}//end contains stuff
				}//END BANK INVENTORY LOOP*/

                if(playerObject->m_reconfigInven)
				for(POSITION skillPos = playerObject->m_reconfigInven->GetHeadPosition();skillPos != NULL;)
				{//RECONFIG INVENTORY LOOP
                    CInventoryObj * invPtr = (CInventoryObj *) playerObject->m_reconfigInven->GetNext(skillPos);
					if(!invPtr->m_itemData)
						continue;
				    
					//calculate combined items
                    int expansiveCount = 0;
					if(invPtr->m_itemData->m_expansiveCollection)
					   expansiveCount = (int)invPtr->m_itemData->m_expansiveCollection->GetCount();

					if(invPtr->m_itemData->m_useType == 5 || expansiveCount < 1)
					{
					   temp = insertInventory;
				       temp.Replace("$HANDLE",handleSTR);
					   temp.Replace("$UNIQUEIDBIND","0");
					   temp.Replace("$ITEMUNIQUEID",classObj->m_sharedInterface->m_cabLib.ConvToString(uniqueIDGEN));
					   temp.Replace("$ITEMGILTYPE",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_itemData->m_globalID));
					   temp.Replace("$CURRENTCHARGES",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_itemData->m_chargeCurrent));
					   temp.Replace("$STORAGETYPE","2");
					   temp.Replace("$STORAGEINDEX","0");
					   temp.Replace("$ARMED",classObj->m_sharedInterface->m_cabLib.ConvToString((int)invPtr->m_armed));
					   temp.Replace("$QUANITY",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_quanity));
					   temp.Replace("$MISCINT1","0");
					   temp.Replace("$MISCFLOAT1","0");
					   temp.Replace("$MISCFLOAT2","0");
					   temp.Replace("$DMGMOD",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_itemData->m_damageModifier));
					   temp.Replace("$LOADEDMISSILE",classObj->m_sharedInterface->m_cabLib.ConvToString((int)invPtr->m_itemData->m_missileLoaded));
					   dbConnection->ExecQuery(temp);
					   uniqueIDGEN++;
					}
					else
					{//contains stuff
					   temp = insertInventory;
					   CString uniqueIDGENSTRING = classObj->m_sharedInterface->m_cabLib.ConvToString(uniqueIDGEN);
				       temp.Replace("$HANDLE",handleSTR);
                       temp.Replace("$UNIQUEIDBIND","0");
					   temp.Replace("$ITEMUNIQUEID",uniqueIDGENSTRING);
					   temp.Replace("$ITEMGILTYPE",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_itemData->m_globalID));
					   temp.Replace("$CURRENTCHARGES",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_itemData->m_chargeCurrent));
					   temp.Replace("$STORAGETYPE","2");
					   temp.Replace("$STORAGEINDEX","0");
					   temp.Replace("$ARMED",classObj->m_sharedInterface->m_cabLib.ConvToString((int)invPtr->m_armed));
					   temp.Replace("$QUANITY",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_quanity));
					   temp.Replace("$MISCINT1","0");
					   temp.Replace("$MISCFLOAT1","0");
					   temp.Replace("$MISCFLOAT2","0");
					   temp.Replace("$DMGMOD",classObj->m_sharedInterface->m_cabLib.ConvToString(invPtr->m_itemData->m_damageModifier));
					   temp.Replace("$LOADEDMISSILE",classObj->m_sharedInterface->m_cabLib.ConvToString((int)invPtr->m_itemData->m_missileLoaded));
					   dbConnection->ExecQuery(temp);
					   uniqueIDGEN++;

                       for(POSITION expPos = invPtr->m_itemData->m_expansiveCollection->GetHeadPosition();expPos != NULL;)
					   {
                           CExpansiveObj * eObj = (CExpansiveObj *) invPtr->m_itemData->m_expansiveCollection->GetNext(expPos);
						   temp = insertInventory;
				           temp.Replace("$HANDLE",handleSTR);
						   temp.Replace("$UNIQUEIDBIND",uniqueIDGENSTRING);
					       temp.Replace("$ITEMUNIQUEID",classObj->m_sharedInterface->m_cabLib.ConvToString(uniqueIDGEN));
					       temp.Replace("$ITEMGILTYPE",classObj->m_sharedInterface->m_cabLib.ConvToString(eObj->m_iconIndex));
					       temp.Replace("$CURRENTCHARGES",classObj->m_sharedInterface->m_cabLib.ConvToString(eObj->m_miscInt));
					       temp.Replace("$STORAGETYPE","2");
						   temp.Replace("$STORAGEINDEX","0");
					       temp.Replace("$ARMED","0");
					       temp.Replace("$QUANITY","0");
						   temp.Replace("$MISCINT1",classObj->m_sharedInterface->m_cabLib.ConvToString(eObj->m_miscInt2));
						   temp.Replace("$MISCFLOAT1",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(eObj->m_xPos));
					       temp.Replace("$MISCFLOAT2",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(eObj->m_yPos));
						   temp.Replace("$DMGMOD","0");
						   temp.Replace("$LOADEDMISSILE",classObj->m_sharedInterface->m_cabLib.ConvToString((int)invPtr->m_itemData->m_missileLoaded));
					       dbConnection->ExecQuery(temp);
                           uniqueIDGEN++;
					   }
					}//end contains stuff
				}//END RECONFIG INVENTORY LOOP

				temp = deleteOldInventory;
				temp.Replace("$HANDLE",handleSTR);
				dbConnection->ExecQuery(temp);
				temp = updateInventoryFlags;
				temp.Replace("$HANDLE",handleSTR);
				dbConnection->ExecQuery(temp);
				//dimcfgs '$HANDLE','$DIMCFGINDEX'
                for(int dimLoop=0;dimLoop<playerObject->m_deformableMeshCount;dimLoop++)
				{
					temp = insertDimCFG;
					temp.Replace("$HANDLE",handleSTR);
					temp.Replace("$ORDERINDEX",classObj->m_sharedInterface->m_cabLib.ConvToString(dimLoop));
					temp.Replace("$DIMCFGINDEX",classObj->m_sharedInterface->m_cabLib.ConvToString(playerObject->m_deformableCfg[dimLoop]));
                    dbConnection->ExecQuery(temp);
				}
                temp = deleteOldDimCfgs;
				temp.Replace("$HANDLE",handleSTR);
				dbConnection->ExecQuery(temp);
				temp = updateDimCfgFlags;
				temp.Replace("$HANDLE",handleSTR);
				dbConnection->ExecQuery(temp);
				//Comm List
				if(playerObject->m_groupDatabase)
				for(POSITION connPos = playerObject->m_groupDatabase->GetHeadPosition();connPos != NULL;)
				{
					CCommGroupObj * cCommPtr = (CCommGroupObj *) playerObject->m_groupDatabase->GetNext(connPos);
					
					temp = insertGMCOMMCFG;
					temp.Replace("$HANDLE",handleSTR);
					temp.Replace("$CONTACTOFHANDLE",classObj->m_sharedInterface->m_cabLib.ConvToString(cCommPtr->m_handle));
					temp.Replace("$CHARACTERNAME",dbConnection->SanitizeFormatIn(cCommPtr->m_name));
                    dbConnection->ExecQuery(temp);
				}
                temp = deleteOldGMCOMMCfgs;
				temp.Replace("$HANDLE",handleSTR);
				dbConnection->ExecQuery(temp);
				temp = updateGMCOMMCfgsFlags;
				temp.Replace("$HANDLE",handleSTR);
				dbConnection->ExecQuery(temp);
				//journal entries
				//HANDLE, QUESTDESCRIPTION, NPCNAME, KEYSTATEMENT, CURRENTPROGRESSION, STATUS, MOBTOHUNTID, MAXDROPCOUNT, DROPGLID, DROPPERCENTAGE, LASTSELECTIONINDEX, UNDELETEABLE, AUTOCOMPLETE, SKILLEXPTYPE, EXPBONUS, THISGAINCAP, UNIQUEJOURNALINDEX, NEWFLAG)
				int uniqueJournalIndex = 0;
				if(playerObject->m_questJournal)
				for(POSITION connPos = playerObject->m_questJournal->GetHeadPosition();connPos != NULL;)
				{
					CQuestJournalObj * journalPtr = (CQuestJournalObj *) playerObject->m_questJournal->GetNext(connPos);
				
					temp = insertJournal;
					temp.Replace("$HANDLE",handleSTR);
                    temp.Replace("$QUESTDESCRIPTION",classObj->m_sharedInterface->SanitizeFormatIn(journalPtr->m_questDescription));
					temp.Replace("$NPCNAME",classObj->m_sharedInterface->SanitizeFormatIn(journalPtr->m_npcName));
                    temp.Replace("$KEYSTATEMENT",classObj->m_sharedInterface->SanitizeFormatIn(journalPtr->m_keyStatement));
					temp.Replace("$CURRENTPROGRESSION",classObj->m_sharedInterface->m_cabLib.ConvToString(journalPtr->m_currentProgression));
					temp.Replace("$STATUS",classObj->m_sharedInterface->m_cabLib.ConvToString(journalPtr->m_status));
					temp.Replace("$MOBTOHUNTID",classObj->m_sharedInterface->m_cabLib.ConvToString(journalPtr->m_mobToHuntID));
					temp.Replace("$MAXDROPCOUNT",classObj->m_sharedInterface->m_cabLib.ConvToString(journalPtr->m_maxDropCount));
					temp.Replace("$DROPGLID",classObj->m_sharedInterface->m_cabLib.ConvToString(journalPtr->m_dropGlid));
					temp.Replace("$DROPPERCENTAGE",classObj->m_sharedInterface->m_cabLib.ConvToString(journalPtr->m_dropPercentage));
					temp.Replace("$LASTSELECTIONINDEX",classObj->m_sharedInterface->m_cabLib.ConvToString(journalPtr->m_lastSelectionIndex));
					temp.Replace("$UNDELETABLE",classObj->m_sharedInterface->m_cabLib.ConvToString(journalPtr->m_undeletable));
					temp.Replace("$AUTOCOMPLETE",classObj->m_sharedInterface->m_cabLib.ConvToString(journalPtr->m_autoComplete));
					temp.Replace("$SKILLEXPTYPE",classObj->m_sharedInterface->m_cabLib.ConvToString(journalPtr->m_skillExpType));
					temp.Replace("$EXPBONUS",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(journalPtr->m_expBonus));
					temp.Replace("$THISGAINCAP",classObj->m_sharedInterface->m_cabLib.FloatToStingCnv(journalPtr->m_thisGainCap));
                    temp.Replace("$UNIQUEJOURNALINDEX",classObj->m_sharedInterface->m_cabLib.ConvToString(uniqueJournalIndex));

                    dbConnection->ExecQuery(temp);
					//Required Items (HANDLE, GLID, QUANITY, NEWFLAG)
                    if(journalPtr->m_requiredItems)
					for(POSITION posReq = journalPtr->m_requiredItems->GetHeadPosition();posReq != NULL;)
					{
                        CQJRequiredItem * reqItemPtr = (CQJRequiredItem *)journalPtr->m_requiredItems->GetNext(posReq);
                        temp = insertJournalReqItem;
						temp.Replace("$HANDLE",handleSTR);
						temp.Replace("$GLID",classObj->m_sharedInterface->m_cabLib.ConvToString(reqItemPtr->m_glid));
						temp.Replace("$QUANITY",classObj->m_sharedInterface->m_cabLib.ConvToString(reqItemPtr->m_quanity));
                        temp.Replace("$UNIQUEINDEX",classObj->m_sharedInterface->m_cabLib.ConvToString(uniqueJournalIndex));
                        temp.Replace("$LOGTYPE",classObj->m_sharedInterface->m_cabLib.ConvToString(reqItemPtr->m_logType));
						
                        dbConnection->ExecQuery(temp);
					}
					uniqueJournalIndex++;
				}
				//se all jnl flags
				temp = deleteJournal;
				temp.Replace("$HANDLE",handleSTR);
				dbConnection->ExecQuery(temp);
				temp = updateJournalFlags;
				temp.Replace("$HANDLE",handleSTR);
				dbConnection->ExecQuery(temp);
				temp = deleteJournalReqItems;
				temp.Replace("$HANDLE",handleSTR);
				dbConnection->ExecQuery(temp);
				temp = updateJnReqItemFlags;
				temp.Replace("$HANDLE",handleSTR);
				dbConnection->ExecQuery(temp);

				
				if(pendingObject->m_logOff == 1)//if log out requested
				{//logs cgaracter
					classObj->m_sharedInterface->m_eventLog->AddEvent("Logging off Flag CDW_2:"+pendingObject->m_text);
					temp = loggOffStatus;
				    temp.Replace("$LOGGEDON","0");
				    temp.Replace("$USER",pendingObject->m_text);
					int minutesPlayedTotal = 0;

					//classObj->m_sharedInterface->m_netComObject->m_pendingLogons->LogonComplete(pendingObject->m_text);

				    int result = classObj->m_sharedInterface->m_netComObject->m_runtimeAccountDB->LogOutRuntime_safe(  pendingObject->m_text,&minutesPlayedTotal);
				    if(result == 0)
					{//success
					   temp.Replace("$MINUTESPLAYED",classObj->m_sharedInterface->m_cabLib.ConvToString(minutesPlayedTotal));
					   authConnection->ExecQuery(temp);

						/* TODO: This to be implemented for Player Quest Development!
						// UPDATE character's in-game status
						temp = "UPDATE gmCharacters SET isInGame = 0 WHERE charName = '" + playerObject->m_charName + "'";
						bool success = dbConnection->ExecQuery(temp); 
						if (!success)
						{
							classObj->m_sharedInterface->m_eventLog->LogByPriority("FAILED to update the " + playerObject->m_charName + "'s in-game status!", 2);
						}
						*/
					}//success
					else
					{//failed
                       classObj->m_sharedInterface->m_netComObject->m_eventLog->AddEvent("CharDataWriter() LogOutRuntime() failed:"+pendingObject->m_text,totalProcessingTime);
					   //just flag log off status
					   temp = loggOffStatusBasic;
					   temp.Replace("$LOGGEDON","0");
				       temp.Replace("$USER",pendingObject->m_text);
					   authConnection->ExecQuery(temp);
					}//edn failed
				}//end logs cgaracter

				totalProcessingTime = timeGetTime() - logonProcessTimeStamp;
				classObj->m_sharedInterface->m_netComObject->m_eventLog->LogByPriority("CharDataWriter() write all:"+pendingObject->m_text,totalProcessingTime,3);


				delete playerObject;
				playerObject = 0;

                pendingObject->playerRef = NULL;//ensure no delete
				delete pendingObject;
				pendingObject = 0;

		}//end processor loop
		delete dbConnection;
		dbConnection = 0;
		delete authConnection;
		authConnection = 0;
		CoUninitialize();
		return 1;
}
