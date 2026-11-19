#include "StdAfx.h"
#include "servermain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define MAX_PLAYERS 6


CServerMain::CServerMain(void) :
	SQUAD_MESSAGE_BURST_SIZE(6)
{
	m_processRotation		 = 0;
	m_serverBaseThreadCount  = 0;
	m_shutDownThreadCount    = 0;
	m_sqlStatusUpdater		 = NULL;
	m_attackSystemsManagment = NULL;
	m_movementSystems        = NULL;
	m_arenaSystems           = NULL;
	m_spawnSystems           = NULL;
	m_aiSystems              = NULL;
	m_questSystems           = NULL;
	m_charCreationSystems    = NULL;
	m_attributeSystems       = NULL;
	m_dmgSystems             = NULL;
	m_systemsLoader          = NULL;
	m_textSystems            = NULL;
	m_eventSystems           = NULL;
	InitializeCriticalSection(	&m_instanceRouterSection);
	InitializeCriticalSection(  &m_interactSection );
    InitializeCriticalSection(  &m_statusQueryLock );
    m_sharedInterface		 = new CSharedNet();
	m_attackSystemsManagment = new CAttackSystemsManagement(m_sharedInterface);
	m_movementSystems        = new CServerMovementSystems(m_sharedInterface);
	m_arenaSystems           = new CArenaSystems(m_sharedInterface);
	m_spawnSystems           = new CSpawnSystems(m_sharedInterface);
	m_aiSystems              = new CAiSystems(m_sharedInterface);
	m_questSystems           = new CQuestSystems(m_sharedInterface);
	m_charCreationSystems    = new CCharacterCreationSystems(m_sharedInterface);
	m_attributeSystems       = new CAttributeSystems(m_sharedInterface,m_arenaSystems,m_questSystems);
	m_dmgSystems             = new CDamageSystems(m_sharedInterface);
	m_systemsLoader          = new CSystemsLoader(m_sharedInterface);
	m_textSystems            = new CTextSystems(m_sharedInterface,m_questSystems);
	m_eventSystems           = new CEventSystems(m_sharedInterface);
	m_systemShutDown	     = FALSE;
	m_groupDataBlock		 = new SQUAD_MESSAGE[SQUAD_MESSAGE_BURST_SIZE];

    m_shuttingDown						= 0;
	m_rebootOnShutdown					= 1;
	m_shutDownNotification              = "Server shutting down for reset shortly...";
	m_sutDownStartStamp                 = GetTickCount();
	m_shutDownNotificationDuration      = 600000;//milliseconds


	ClearLiveLog("tblServerLog");
	ZeroMemory(&m_serverStatus,sizeof(AB_serverStatusLive));
}

CServerMain::~CServerMain(void)
{
	if(m_systemsLoader)
	{
       delete m_systemsLoader;
	   m_systemsLoader = 0;
	}
	if(m_dmgSystems)
	{
       delete m_dmgSystems;
	   m_dmgSystems = 0;
	}
	if(m_sharedInterface)
	{
	   delete m_sharedInterface;
	   m_sharedInterface = 0;
	}
	if(m_attackSystemsManagment)
 	{
       delete m_attackSystemsManagment;
	   m_attackSystemsManagment = 0;
	}
    if(m_movementSystems)
 	{
       delete m_movementSystems;
	   m_movementSystems = 0;
	}
	if(m_arenaSystems)
	{
       delete m_arenaSystems;
	   m_arenaSystems = 0;
	}
	if(m_aiSystems)
	{
       delete m_aiSystems;
	   m_aiSystems = 0;
	}
	if(m_charCreationSystems)
	{
       delete m_charCreationSystems;
       m_charCreationSystems=0;
	}
	if(m_attributeSystems)
	{
	   delete m_attributeSystems;
	   m_attributeSystems = 0;
	}
	if(m_spawnSystems)
	{
       delete m_spawnSystems;
	   m_spawnSystems = 0;
	}
	if(m_questSystems)
	{
       delete m_questSystems;
	   m_questSystems = 0;
	}
	if(m_textSystems)
	{
	   delete m_textSystems;
	   m_textSystems=0;
	}
	if(m_groupDataBlock)
	{
		delete [] m_groupDataBlock;
		m_groupDataBlock = 0;
	}
    if(m_eventSystems)
	{
		delete m_eventSystems;
		m_eventSystems = 0;
	}

	DeleteCriticalSection(&m_interactSection);
    DeleteCriticalSection(&m_statusQueryLock);
	DeleteCriticalSection(&m_instanceRouterSection);
}
//------------------------------------------------------------------------------------//
void CServerMain::ClearLogOnFlagsForServerID(int svrID)
{
    

}
//------------------------------------------------------------------------------------//
int CServerMain::StartServer()
{
	m_sharedInterface->m_realtimeLogger.WriteDebugFile("StartServer()",0,1);
	m_sharedInterface->m_realtimeLogger.WriteDebugFile("ProcessLoadDataTable()",0,1);
    m_systemsLoader->ProcessLoadDataTable("tblDataLoadConfig where usedby <> 'aiserver'");		//load data lists from database configuration table
	m_sharedInterface->m_realtimeLogger.WriteDebugFile("ProcessMainCfg()",0,1);
    m_systemsLoader->ProcessMainCfg("tblMainConfiguration");		//load server settings
	m_sharedInterface->m_realtimeLogger.WriteDebugFile("LoadSupportedWorldsFromDB()",0,1);
    m_systemsLoader->LoadSupportedWorldsFromDB();
	m_sharedInterface->m_realtimeLogger.WriteDebugFile("LoadSpawnPointsFromDB()",0,1);
	m_systemsLoader->LoadSpawnPointsFromDB();
	m_sharedInterface->m_realtimeLogger.WriteDebugFile("LoadSupportedArenaItems()",0,1);
	m_systemsLoader->LoadSupportedArenaItems();
	m_sharedInterface->m_realtimeLogger.WriteDebugFile("CWorldChannelRuleList:GetFromSQL()",0,1);
	m_sharedInterface->m_worldChannelRuleDB->GetFromSQL(m_sharedInterface->m_netComObject->m_eventLog);//load world rules from db
	m_sharedInterface->m_threadedDataProcessor->AddToQue(0,"",0,0,12,0,0);//load placed houses from GM_SERVERPLACEMENTS


	
	/*if(m_sharedInterface->m_dbIgnMode > 0)
	{
	   m_sharedInterface->m_realtimeLogger.WriteDebugFile("Aquire Quest Entities from DB",0,1);
	   if(m_sharedInterface->m_serverItemGenerator)
	   {
          delete m_sharedInterface->m_serverItemGenerator;
		  m_sharedInterface->m_serverItemGenerator = 0;
	   }
	   m_sharedInterface->m_serverItemGenerator = new CServerItemGenObjList();
	   if(m_sharedInterface->m_dbIgnMode != 2)
	      m_sharedInterface->m_serverItemGenerator->GetFromSQL(m_sharedInterface->m_netComObject->m_eventLog,m_sharedInterface->m_itemManager);
	   
	   m_sharedInterface->m_realtimeLogger.WriteDebugFile("Aquire Quest Entities from DB-complete",0,1);
	}*/

    if(m_sharedInterface->m_netComObject->m_accountDBMode == 1)
	{
	   m_sharedInterface->m_realtimeLogger.WriteDebugFile("Aquire clans",0,1);
	   m_sharedInterface->m_threadedDataProcessor->AddToQue(0,"",0,0,10,0,0);//que clan loader
       
    
	   CDBConnectionSystem * authConnection = new CDBConnectionSystem(	m_sharedInterface->m_authenticationDBConnection.m_sqlServerIP,
																		m_sharedInterface->m_authenticationDBConnection.m_sqlServerCatalog,
																		m_sharedInterface->m_authenticationDBConnection.m_sqlServerUsername,
																		m_sharedInterface->m_authenticationDBConnection.m_sqlServerPassword,
																		m_sharedInterface->m_eventLog);
       if(authConnection->m_connectionEstablished == TRUE)
	   {//verify connection established

			m_sharedInterface->m_eventLog->AddEvent("Reset logged on flags");
			//clear node and zone status
			//unflag all players logged into this server
			authConnection->ExecQuery("UPDATE GMACCOUNTBASE SET LOGGEDON = '0' WHERE LOGGEDON = '"+m_sharedInterface->m_cabLib.ConvToString(m_sharedInterface->m_netComObject->m_serverID)+"'");
	   }
	   else
	   {
			delete authConnection;
			authConnection=0;
			m_sharedInterface->m_eventLog->OutPutTextFile("BadInitConnect.txt");
			m_sharedInterface->m_realtimeLogger.WriteDebugFile(m_sharedInterface->m_authenticationDBConnection.m_sqlServerIP.GetBuffer(),0,1);
			m_sharedInterface->m_realtimeLogger.WriteDebugFile(m_sharedInterface->m_authenticationDBConnection.m_sqlServerCatalog.GetBuffer(),0,1);
			m_sharedInterface->m_realtimeLogger.WriteDebugFile(m_sharedInterface->m_authenticationDBConnection.m_sqlServerUsername.GetBuffer(),0,1);
			//m_sharedInterface->m_realtimeLogger.WriteDebugFile(m_sharedInterface->m_authenticationDBConnection.m_sqlServerPassword.GetBuffer(),0,1);
			m_sharedInterface->m_eventLog->AddEvent("Reset logged on flags *CONNECT FAILED* Server will HALT **auth");
			m_sharedInterface->m_realtimeLogger.WriteDebugFile("Reset logged on flags *CONNECT FAILED* Server will HALT **auth",0,1);
			return 0;//leave because we have no basis to use the database
	   }
	   delete authConnection;
	   authConnection=0;

	   CDBConnectionSystem * dbConnection = new CDBConnectionSystem(m_sharedInterface->m_baseAccountDBConnection.m_sqlServerIP,
			                                                        m_sharedInterface->m_baseAccountDBConnection.m_sqlServerCatalog,
																    m_sharedInterface->m_baseAccountDBConnection.m_sqlServerUsername,
															        m_sharedInterface->m_baseAccountDBConnection.m_sqlServerPassword,
																    m_sharedInterface->m_eventLog);
       if(dbConnection->m_connectionEstablished == TRUE)
	   {//verify connection established

			bool dynItemsLoaded = m_sharedInterface->m_itemManager->ImportAllDynamicItems(m_sharedInterface->m_elementDB,NULL,dbConnection);
			m_sharedInterface->m_eventLog->AddEvent("Dynamic Items Added:",(DWORD)dynItemsLoaded);
		   
			m_sharedInterface->m_eventLog->AddEvent("Reset pending flagged db items DB *Connected*.");
			//clear node and zone status
			dbConnection->ExecQuery("DELETE FROM SYSLIVESERVERNODES WHERE NODEID = '"+m_sharedInterface->m_cabLib.ConvToString(m_sharedInterface->m_netComObject->m_serverID)+"'");
	   }
	   else
	   {
		   delete dbConnection;
		   dbConnection=0;
		   m_sharedInterface->m_eventLog->AddEvent("Reset pending flagged db items DB *CONNECT FAILED* Server will HALT");
		   m_sharedInterface->m_realtimeLogger.WriteDebugFile("Reset pending flagged db items DB *CONNECT FAILED* Server will HALT",0,1);
		   return 0;//leave because we have no basis to use the database
	   }

	   //load ign data after dyn items aquisition
	   m_sharedInterface->m_serverItemGenerator = new CServerItemGenObjList();
	   m_sharedInterface->m_serverItemGenerator->GetFromSQL(m_sharedInterface->m_netComObject->m_eventLog,m_sharedInterface->m_itemManager);
	   
       
       //AXWeb update- this block servers as a ayetem when AXHUB AKA(chain sql) is down this will check
	   //the AXWeb que and update the chain AXHUB for any username/ban/permissions etc
	   //note the most this would be called is once if the AXHUB was down during a critical variable change
	   //Generally the AXWEB will update AXHUB on a event that critical variable change unless AXHUB was down when AXWeb 
	   //made the change, in case this happens once the AXHUB comes online and these servers start spinning up
	   //querries must handle multiple execs without consiquence to avoid potential timing issues with multiple ids comming on at the exact time even though unlikly
	   //AVOID multiple servers that are chained from any timing issues by only having one server per chain access to the axweb::as a standard ill assign server ID 1 on the dev platform as an example
	   ConnectionStructureClass AXWebConnection;
	   if(AXWebConnection.AquireFromCfgFile("axweb.cfg"))
	   {//aquired connect info
		  CDBConnectionSystem * axWebConnection = new CDBConnectionSystem(AXWebConnection.m_sqlServerIP,
																		  AXWebConnection.m_sqlServerCatalog,
																		  AXWebConnection.m_sqlServerUsername,
																		  AXWebConnection.m_sqlServerPassword,
																		  m_sharedInterface->m_eventLog);
		  if(axWebConnection->m_connectionEstablished == TRUE)
		  {//verify axweb connection established
			 BOOL qryResult = 0;
			 _RecordsetPtr rdPtr = axWebConnection->Query("SELECT statement,dateadded FROM tblBroadcastSync WHERE HUBIP = '"+m_sharedInterface->m_baseAccountDBConnection.m_sqlServerIP+"' ORDER BY DATEADDED");
			 if(rdPtr!=NULL)
			 {//querry AXWEB for pending commands while this server parent was down
				int recordCount = axWebConnection->GetCount(rdPtr);
				for(int broadCLoop = 0;broadCLoop<recordCount;broadCLoop++)
				{//statements to execure on AXHUB
				   CString rawStatement = axWebConnection->GetStringData("statement",rdPtr);
				   CString dateAddedLoc = axWebConnection->GetStringData("dateadded",rdPtr);
				   qryResult = dbConnection->ExecQuery(axWebConnection->SanitizeFormatOut(rawStatement));//sanitize out will turn the formatted query live
				   axWebConnection->NextRecord(rdPtr);
				   if(qryResult)//only remove if the query succeeded
				   {
					  if(!axWebConnection->ExecQuery("DELETE FROM tblBroadcastSync where dateadded = '"+ dateAddedLoc + "'"))//delete pending command using dateadded as condition
						  m_sharedInterface->m_eventLog->AddEvent("Clear by date failed:+"+dateAddedLoc);
				   }
				   else
				      m_sharedInterface->m_eventLog->AddEvent(rawStatement + " :Failed");
				}//end statements to execure on AXHUB
				axWebConnection->Close(rdPtr);
			 }//end querry AXWEB for pending commands while this server parent was down
		  }//end axweb verify connection established
		  delete axWebConnection;
		  axWebConnection = 0;
	   }//end aquired connect info
	   else
	      m_sharedInterface->m_eventLog->AddEvent("****connect info to create link for axhub is not present, unable to check for critical variable changes at this time.");
		  //m_eventLog->AddEvent("****connect info to create link for axhub is not present, unable to check for critical variable changes at this time.");
	   //end AXWeb Update


	   delete dbConnection;
	   dbConnection=0;
	}
	m_sharedInterface->m_realtimeLogger.WriteDebugFile("EnableServer()",0,1);
	return m_sharedInterface->m_netComObject->EnableServer();		//start it up
}
//------------------------------------------------------------------------------------//
void CServerMain::AddResponseCommand(CString commandType, 
									 CString peram1, 
									 CString peram2, 
									 CString peram3,
									 CString peram4,
									 CString peram5,
									 CString peram6)
{
     m_sharedInterface->m_dbConnection->ExecQuery("insert into tblServerResponse (CommandType, peram1, peram2, peram3, peram4, peram5, peram6) Values('"+commandType+"', '"+peram1+"', '"+peram2+"', '"+peram3+"', '"+peram4+"', '"+peram5+"', '"+peram6+"')" );
}
//------------------------------------------------------------------------------------//
CRITICAL_SECTION CServerMain::GetInteractSection()
{
    return m_interactSection;
}
//------------------------------------------------------------------------------------//
void CServerMain::ClearLiveLog(CString tableName)
{
    m_sharedInterface->m_dbConnection->ExecQuery("delete "+tableName);
}
//------------------------------------------------------------------------------------//
void CServerMain::ServerCallback(int & channel)
{
    LPVOID lpMessage = NULL;
    DPNID messageFrom = 0;
    int sizeTracker = 0;

	int packetCount = (int)m_sharedInterface->m_netComObject->m_packetQue->GetCountProtected();
	int processCount = 0;

	
	if(packetCount==0)
	{//process stall until work to do
	   ResetEvent(m_sharedInterface->m_netComObject->m_pausePrimarySpinEvent[channel]);
	   WaitForSingleObject(m_sharedInterface->m_netComObject->m_pausePrimarySpinEvent[channel],500);
	   packetCount=1;//arbitrary
	}//end process stall until work to do

	while(m_sharedInterface->m_netComObject->m_packetQue->GetNextPacket_safe(&sizeTracker,(BYTE**)&lpMessage,&messageFrom))
	{//if theres a message handle it
	 m_sharedInterface->m_netComObject->AddToTotalBandwidthIncomming_safe((unsigned long)sizeTracker);
     HandleEngineMessagesServerSide_safe(lpMessage,messageFrom,sizeTracker);
	 
     if(processCount > 50)
	    break;
     processCount++;
	}//end if theres a message handle it
}
//------------------------------------------------------------------------------------//
void CServerMain::ServerItemGeneratorsCallback(CServerItemList * runtimeItems,
											   CServerItemGenObjList * serverGeneratorsList,
											   int processCount)
{
    if(!runtimeItems)
	    return;

	if(!serverGeneratorsList)
	    return;

	DWORD currentTime = GetTickCount();

	for(int pLoop=0;pLoop<processCount;pLoop++)
	{
		CServerItemGenObj * spnGenPtr = serverGeneratorsList->GetBySpawnID_LOS(serverGeneratorsList->IncrementUpdateIndex());
		if(spnGenPtr)
		{
			if(!spnGenPtr->m_existsOnServer)
			{
				if((currentTime - spnGenPtr->m_timeStamp) >= spnGenPtr->m_respawnDuration)
				{
					CServerItemObj * itemPtr = NULL;
					spnGenPtr->m_serverItem->Clone(&itemPtr);
					itemPtr->m_networkAssignedID = m_sharedInterface->m_netComObject->GetFreeRuntimeEntityNetworkID();
					assert(itemPtr->m_networkAssignedID != -1);
					
					
					itemPtr->m_spawnID = spnGenPtr->m_spawnGenID;
					itemPtr->m_refSpawnId = spnGenPtr->m_refSpawnGenID;
				
					m_sharedInterface->m_eventLog->LogByPriority("SpawnedNPC() NetID:",(DWORD)itemPtr->m_networkAssignedID,3);
					m_sharedInterface->m_eventLog->LogByPriority("SpawnedNPC() Channel:",(DWORD)itemPtr->m_world,3);
					m_sharedInterface->m_eventLog->LogByPriority("SpawnedNPC() SpawnID:",(DWORD)itemPtr->m_spawnID,3);

					spnGenPtr->m_existsOnServer = TRUE;
					serverGeneratorsList->Unlock();
					runtimeItems->Add_safe(itemPtr);
				}
				else 
			        serverGeneratorsList->Unlock();
			}
			else 
			    serverGeneratorsList->Unlock();
		}
		Sleep(0);//ensures other threads turn
    }
}
//------------------------------------------------------------------------------------//
//--------------Handles system messages-----------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CServerMain::HandleEngineMessagesServerSide_safe(LPVOID lpMessage,DPNID idFromLoc,int msgSize)
{
	if(msgSize <= 0)
	{
       m_sharedInterface->m_netComObject->m_eventLog->AddEvent("zero packet size disconnecting-security violation","dpnid:",idFromLoc);
	   m_sharedInterface->m_netComObject->Disconnect(idFromLoc);//disconnect for incomplience
       return FALSE;
	}
	//end handle new player communications
    LPEXMSG_CATAGORIE identityMsg = (LPEXMSG_CATAGORIE)lpMessage;
    int catagorieINT = identityMsg->m_catagorie;
	int tempInt = -1;
	BYTE * blockCast = (BYTE*)lpMessage;

	if(m_sharedInterface->m_eventLog->m_logPriority >= 3)//non-allocation occulder-not necessary for low frequency logging
	   m_sharedInterface->m_eventLog->LogByPriority("HandleEngineMessagesServerSide_safe() cat:",(DWORD)identityMsg->m_catagorie,3);
    //end handle new player communications
	switch(identityMsg->m_catagorie)
	{//switch on...
		case 4://CLIENT MOVEMENT HANDLER (SECURED/CLEANED)
			MovMsgHandler_safe(lpMessage,idFromLoc,msgSize);
			SafeDeleteArray(blockCast);
			return TRUE;
		break;
		case 5://CLIENT MOVEMENT HANDLER WITH CHAIN FIRE MSGS (SECURED/CLEANED)
			HandleMovFire_safe(lpMessage,idFromLoc,msgSize);
			SafeDeleteArray(blockCast);
			return TRUE;
		break;
		case 8://CLIENT SPAWN HANDLER
		    HandleSpawn_safe(lpMessage,idFromLoc,msgSize);//(SECURED/CLEANED)
			SafeDeleteArray(blockCast);
			return TRUE;
		break;
		case 17://AI BLOCK DATA HANDLER (SECURED/CLEANED)
		    //input positional information
			if(m_aiSystems->HandleAIBlockData_safe(lpMessage,msgSize,idFromLoc,tempInt))
			   if(tempInt > -1)
	              m_attributeSystems->m_sharedInterfaceRef->SendUpdateToAIChannel_safe(idFromLoc,tempInt);//

			SafeDeleteArray(blockCast);
			return FALSE;
		break;
		case 18://HANDLE CMD MSG - Misc functionality
			HandleCommandMsg_safe(lpMessage,idFromLoc,msgSize);//(SECURED/CLEANED)
			SafeDeleteArray(blockCast);
		    return TRUE; 
		break;
		case 19://CHARACTER CREATION
               this->m_sharedInterface->m_threadedDataProcessor->AddToQue(0,"",0,idFromLoc,2,(byte*)lpMessage,msgSize);
               return TRUE;
		break;
		case 24://BLOCK ATTRIBUTES outside guarenteed block
			ProcessBlockAttribMsgs_safe(lpMessage,idFromLoc,msgSize);//(SECURE/CLEANED)
			SafeDeleteArray(blockCast);
		    return TRUE;
		break;
		case 26://GUARENTEED CLIENT MSG BLOCK- non constant
            ProcessGuarenteedMsg_safe(lpMessage,idFromLoc,msgSize);//(SECURE/CLEANED)
			SafeDeleteArray(blockCast);
		    return TRUE;
		break;
		case 28://GUARENTEED CLIENT MSG BLOCK- non constant (SECURE/CLEANED)
			ProcessGuarenteedMsgAI_safe(lpMessage,idFromLoc,msgSize);
			SafeDeleteArray(blockCast);
		    return TRUE;
		break;
	}//end switch off


	m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Unknown catagorie -disconnecting...","byte:",identityMsg->m_catagorie);
	m_sharedInterface->m_netComObject->Disconnect(idFromLoc);//disconnect for incomplience
	//delete memory
	
	delete [] blockCast;
	blockCast = 0;
	
	return FALSE;
}
//-------------------------------------------------------------------------------------------------------------//
BOOL CServerMain::Security(CPlayerObject * playerObjectData,
						   CAccountObject * accountPtr, 
						   DWORD & currentTickCount,
						   LPSHRUNK_TOSVRMOV_STRUCT formatedMessage,
						   DPNID & idFromLoc)
{
        if(!accountPtr)
            return FALSE;

        long timePassed = currentTickCount - playerObjectData->m_lastMsgTimeStamp;
	    if(timePassed < 50)
	    {//too soon-innocent caused by lag
	       return FALSE;
	    }//end too soon-innocent caused by lag

        if(m_sharedInterface->GetElaspeTimeMSDW(accountPtr->m_syncTestReset) > 600000)
	       accountPtr->m_syncTrackingTime = 0;
				 
		if(accountPtr->m_syncTrackingTime == 0)
		{
			if(m_sharedInterface->GL_frequency.QuadPart != 0)
				m_sharedInterface->m_netComObject->m_eventLog->AddEvent("No Multimedia Timer, hardware no good");

			accountPtr->m_syncTrackingTime	    = m_sharedInterface->GetTimeStampDW()-formatedMessage->tmel;
			accountPtr->m_syncTestReset		    = m_sharedInterface->GetTimeStampDW();
			accountPtr->m_lastELtime			= (DWORD)formatedMessage->tmel;
		}

        /*long correctTimePassed = (DWORD)m_sharedInterface->GetElaspeTimeMSDW(accountPtr->m_syncTrackingTime);
		DWORD durationABS = abs(formatedMessage->tmel - correctTimePassed);
		if(accountPtr->m_lastELtime != -9999999999999)
		{
			if((accountPtr->m_lastELtime-5) > (DWORD)formatedMessage->tmel)
			{//anti reset exploit
				m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Possible ISEC problem R3-Disconnecting :",accountPtr->m_accountName);
                m_sharedInterface->m_netComObject->Disconnect(idFromLoc);//disconnect for incomplience
				return FALSE;
			}//end anti reset exploit
		}
				  
		accountPtr->m_lastELtime = (DWORD)formatedMessage->tmel;
		if(durationABS > 5000)
		{
			accountPtr->m_syncViolations++;
			if(accountPtr->m_syncViolations > 9)
			{
			m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Possible ISEC problem-Disconnecting :",accountPtr->m_accountName);
            m_sharedInterface->m_netComObject->Disconnect(idFromLoc);//disconnect for incomplience
			return FALSE;
			}
		}
		else
			accountPtr->m_syncViolations = 0;


		if(timePassed < m_sharedInterface->m_netComObject->m_frequencyLimit)
		   playerObjectData->m_violationsInARow++;
		else
		   playerObjectData->m_violationsInARow = 0;

        if(playerObjectData->m_violationsInARow > m_sharedInterface->m_netComObject->m_violationsToDisconnect)
	    {
			m_sharedInterface->DisconnectPlayerAndLog(playerObjectData,"Frequency Violation ");
			return FALSE;
	    }*/
	    playerObjectData->m_lastMsgTimeStamp = currentTickCount;
            //skip processing if too fast (caused by latency)
        if((currentTickCount - playerObjectData->m_lastSurroundingsUpdateTime) < (DWORD)(m_sharedInterface->m_netComObject->m_frequencyLimit/2))
		{
			return FALSE;
		}
        playerObjectData->m_lastSurroundingsUpdateTime = currentTickCount;


        //movement dist tracker
		long timePassedMovChk = currentTickCount - playerObjectData->m_movementTrackTimeStamp;
		if(playerObjectData->m_movementTrackTimeStamp == 0)
		{//first time initialize
            playerObjectData->m_movementTrackTimeStamp = currentTickCount;
			if(playerObjectData->m_energy > 0)
			{
                playerObjectData->m_movementTrackPosStamp.x = formatedMessage->Xpos;
				playerObjectData->m_movementTrackPosStamp.y = formatedMessage->Ypos;
				playerObjectData->m_movementTrackPosStamp.z = formatedMessage->Zpos;
			}
		}//end first time initialize
		else if(timePassedMovChk > 3000)
		{//test
			float distAllowed = (((float)timePassedMovChk)/1000.0f) * playerObjectData->m_speedRating;
			float zeroTemp = 0.0f;
			float distTraveled = m_sharedInterface->m_cabLib.MagnitudeSquared(playerObjectData->m_movementTrackPosStamp.x,zeroTemp,playerObjectData->m_movementTrackPosStamp.z,
																				formatedMessage->Xpos,zeroTemp,formatedMessage->Zpos);
			if(distTraveled > distAllowed)
			{
				if(playerObjectData->m_allowLargeMovement == false)
				{//disconnect-moved too much without a pass
					m_sharedInterface->DisconnectPlayerAndLog(playerObjectData,"Movement Distance Violation ");
					formatedMessage->Xpos = playerObjectData->m_movementTrackPosStamp.x;
					formatedMessage->Ypos = playerObjectData->m_movementTrackPosStamp.y;
					formatedMessage->Zpos = playerObjectData->m_movementTrackPosStamp.z;
				}//end disconnect-moved too much without a pass
				else
				{//saved by a pass stamp normally and remove pass
                    playerObjectData->m_allowLargeMovement = false;
                    playerObjectData->m_movementTrackTimeStamp = currentTickCount;
					if(playerObjectData->m_energy > 0)
					{
						playerObjectData->m_movementTrackPosStamp.x = formatedMessage->Xpos;
						playerObjectData->m_movementTrackPosStamp.y = formatedMessage->Ypos;
						playerObjectData->m_movementTrackPosStamp.z = formatedMessage->Zpos;
					}
				}//end saved by a pass stamp normally and remove pass
			}
			else{//stamp
				playerObjectData->m_movementTrackTimeStamp = currentTickCount;
				if(playerObjectData->m_energy > 0)
				{
					playerObjectData->m_movementTrackPosStamp.x = formatedMessage->Xpos;
					playerObjectData->m_movementTrackPosStamp.y = formatedMessage->Ypos;
					playerObjectData->m_movementTrackPosStamp.z = formatedMessage->Zpos;
				}
			}//end stamp
	    }//end test
        //end time incoming for too fast
    return TRUE;
}
//-------------------------------------------------------------------------------------------------------------//
void CServerMain::HandleBackupInterval(CPlayerObject * playerObjectData,CAccountObject * accountPtr, DWORD & currentTickCount)
{
    if(m_sharedInterface->m_netComObject->m_autoBackupFrequency < 1)
	   return;

	if(m_sharedInterface->m_netComObject->m_accountDBMode == 0)
	{//local file mode
		if(!playerObjectData->m_aiControlled)//make sure not ai
		if(playerObjectData->m_energy > 0)//make sure alive
        if((currentTickCount - playerObjectData->m_spawnTimeStamp) > (DWORD)m_sharedInterface->m_netComObject->m_autoBackupFrequency)
		{//account needs to be written back into primary db to make sure when backup goes we get recent chasnges
            m_sharedInterface->m_netComObject->UpdateAccount(playerObjectData,accountPtr);
			playerObjectData->m_spawnTimeStamp = currentTickCount;//SUSPECT does this corrilate to side effects, should we make a new varaiable
		}//end account needs to be written back into primary db to make sure when backup goes we get recent chasnges
	}//end local file mode
	else if(m_sharedInterface->m_netComObject->m_accountDBMode == 1)
	{//DB Mode
		if(!playerObjectData->m_aiControlled)//make sure not ai
			if(playerObjectData->m_energy > 0)//make sure alive
            if((currentTickCount - accountPtr->m_backupStampX) > (DWORD)m_sharedInterface->m_netComObject->m_autoBackupFrequency)
			{//account needs to be written back into primary db to make sure when backup goes we get recent chasnges
                //m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Backup character triggered:"+accountPtr->m_accountName);
				m_sharedInterface->m_netComObject->UpdateAccount(playerObjectData,accountPtr); 
				if(playerObjectData->m_playerOrigRef)
                   m_sharedInterface->m_netComObject->m_characterPendingSaveSystem->AddCharacterToPendingList(playerObjectData->m_playerOrigRef,0,accountPtr->m_accountName,-1);
                accountPtr->m_backupStampX = currentTickCount;
			}//account needs to be written back into primary db to make sure when backup goes we get recent chasnges
	}//END DB MODE
}
//-------------------------------------------------------------------------------------------------------------//
BOOL CServerMain::HandlePlayerStasis(CPlayerObject * playerObjectData,CAccountObject * accountPtr, DWORD & currentTickCount)
{
	       ActionListStructure actionsStruct;
		   BOOL inSafeZone = FALSE;
		   if(m_sharedInterface->m_actionZoneDB->GetAllApplicableActions(playerObjectData->m_3dPosition, playerObjectData->m_stencilChannel, &actionsStruct))
		   {//process actions
              for(int acLoop=0;acLoop<actionsStruct.actionReturnCount;acLoop++)
			  {
                  switch(actionsStruct.actionArray[acLoop])
				  {
					case 1://damage zone (dmg applied once per second)
                        if(currentTickCount - playerObjectData->m_envDamageStamp > 3000)
						{
						   if(playerObjectData->m_energy > 0)
						   {
                                playerObjectData->m_energy += actionsStruct.actionAmountArray[acLoop];
								m_sharedInterface->UpdateClientStatistics(playerObjectData,playerObjectData->m_currentID);

                                if(actionsStruct.attribArray[acLoop] > -1)
								{
								   m_sharedInterface->SendDetailedMsgToClient(      6,
																					actionsStruct.attribArray[acLoop],
																					playerObjectData->m_networkAssignedID,
																					0,
																					playerObjectData);
								}


								if(playerObjectData->m_energy < 1)
								{
									m_sharedInterface->HandleDeath(playerObjectData,playerObjectData);
									return FALSE;
								}
						   }
						   playerObjectData->m_envDamageStamp = currentTickCount;  
						}
					break;
					case 2://safe zone
                       inSafeZone = TRUE;
					break;
					case 3://safe zone
                       inSafeZone = TRUE;
					break;
				  }
			  }
		   }//end process actions
		   //Notify of safe zone changes
           if(playerObjectData->m_inSafeZone != inSafeZone || playerObjectData->m_justSpawned == TRUE)
		   {//notification is necessary
              playerObjectData->m_inSafeZone = inSafeZone;
              if(inSafeZone)
                 m_sharedInterface->SendGeneralMessageHandler(6,playerObjectData);
			  else
			     m_sharedInterface->SendGeneralMessageHandler(5,playerObjectData);
		   }//end notification is necessary

           
		   //handle stasis
		   if(playerObjectData->m_stasis == 1)
		   {//hidden
		      float distance = m_sharedInterface->m_cabLib.MagnitudeSquared(playerObjectData->m_Target3dPosition.x,
				                                        playerObjectData->m_Target3dPosition.y,
									                    playerObjectData->m_Target3dPosition.z,
									                    playerObjectData->m_positionLock.x,
														playerObjectData->m_positionLock.y,
														playerObjectData->m_positionLock.z);
			  if(distance > 4.0f)
			  {//moved too much
                 playerObjectData->m_stasis = 0;//cancel hide
				 m_sharedInterface->SendGeneralMessageHandler(23,playerObjectData);
				 playerObjectData->m_skillUseTimeStamp = GetTickCount();//reset skill use timer
			  }//end moved too much
		   }//end hidden
		   //handle out of bounds
		   return TRUE;
}
//-------------------------------------------------------------------------------------------------------------//
BOOL CServerMain::HandleMovData_safe(LPSHRUNK_TOSVRMOV_STRUCT formatedMessage,DPNID & idFromLoc)
{
	   DWORD currentTickCount = GetTickCount();	
	   //test CRC
		float serverSideMsgCRC = m_sharedInterface->m_netComObject->GetMovMsgCrC(formatedMessage);
		if(m_sharedInterface->m_netComObject->CRCInValid(serverSideMsgCRC,formatedMessage->CRCCHECK)){//verify CRC integrity
           m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Invalid CRC from account :Guess what, time to diconnect him");
		   m_sharedInterface->m_netComObject->m_eventLog->AddEvent("SVR calculated CRC :",serverSideMsgCRC);
		   m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Client calculated CRC :",formatedMessage->CRCCHECK);
           m_sharedInterface->m_netComObject->Disconnect(idFromLoc);//disconnect for incomplience
		   return FALSE;
		}//end verify CRC integrity

	   BOOL alive = FALSE;
	   m_sharedInterface->m_netComObject->m_runtimeAccountDB->Lock();
	   CPlayerObject * playerObjectData = m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->GetByNetID_GetAliveStatus_LOS((unsigned int)formatedMessage->idReference,&alive);
	   if(!playerObjectData)
	   {//invalid
           m_sharedInterface->m_netComObject->m_runtimeAccountDB->Unlock();
		   m_sharedInterface->m_eventLog->LogByPriority("HandleMovData() cannot find NetID:",(DWORD)formatedMessage->idReference,3);
		   return FALSE;
	   }//end invalid
	   else{//valid
           if(playerObjectData->m_aiControlled)
		   {
		      m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Unlock();
			  m_sharedInterface->m_netComObject->m_runtimeAccountDB->Unlock();
              return FALSE;
		   }

		   CAccountObject * accountPtr = m_sharedInterface->m_netComObject->m_runtimeAccountDB->GetAccountByID(idFromLoc);
		   if(!accountPtr)
		   {
			   m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Unlock();
			   m_sharedInterface->m_netComObject->m_runtimeAccountDB->Unlock();
			   return FALSE;
		   }

           if(!Security(playerObjectData,accountPtr,currentTickCount,formatedMessage,idFromLoc))
		   {
			   m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Unlock();
			   m_sharedInterface->m_netComObject->m_runtimeAccountDB->Unlock();
			   return FALSE;
		   }
             
		   if(alive)
		   {//backup/regen interval
		      HandleBackupInterval(playerObjectData,accountPtr,currentTickCount);

			  if(!m_sharedInterface->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(playerObjectData->m_stencilChannel,playerObjectData->m_currentWorldID,WORLDRULE_NOHEALTHREGEN))
			  {
				  if(currentTickCount-playerObjectData->m_regenStamp > m_sharedInterface->m_regenerationTurnSpeed)
				  {//handle regeneration
					 if(playerObjectData->m_energy < playerObjectData->m_maxEnergy)
					 {
						playerObjectData->m_energy += m_sharedInterface->m_defaultHealthRegenAmount;
						if(playerObjectData->m_groupID > 0)
						{
							playerObjectData->m_groupStruct.m_squadDataIsDirty = 1;
							// Transmit updated group info to all members
							m_sharedInterface->HandleGroupData(playerObjectData->m_groupID);
						}
						if(playerObjectData->m_energy > playerObjectData->m_maxEnergy)
						   playerObjectData->m_energy = playerObjectData->m_maxEnergy;
						m_sharedInterface->UpdateClientStatistics(playerObjectData,playerObjectData->m_currentID);
					 }
					 playerObjectData->m_regenStamp = currentTickCount;
				  }//end handle regeneration
			  }

			  //Check the suicide timer
			  if(playerObjectData->m_suicideTimeStamp)
			  {
				  if((currentTickCount - playerObjectData->m_suicideTimeStamp) > 1000)
				  {
					  playerObjectData->m_suicideTimeStamp = currentTickCount;
					  playerObjectData->m_suicideTimer--;
					  if(playerObjectData->m_suicideTimer % 5 == 0 || playerObjectData->m_suicideTimer < 5)
					  {
						  // Send Message confirmation
							m_sharedInterface->SendTextMsg( DPNSEND_GUARANTEED,
															playerObjectData->m_currentID,
															"Countdown: " + m_sharedInterface->m_cabLib.ConvToString((int)playerObjectData->m_suicideTimer),
											 				0,
															playerObjectData,
															0);
					  }
					  if(!playerObjectData->m_suicideTimer)
					  {
						  m_sharedInterface->BroadcastEffect(7,playerObjectData->m_networkAssignedID,playerObjectData);
						  playerObjectData->m_energy = 0;
						  m_sharedInterface->UpdateClientStatistics(playerObjectData,playerObjectData->m_currentID);
						  m_sharedInterface->HandleDeath(playerObjectData,playerObjectData);
					      m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Unlock();
					      m_sharedInterface->m_netComObject->m_runtimeAccountDB->Unlock();
						  return FALSE;
					  }
				  }
			  }

			  CInventoryObj *weaponLink = playerObjectData->m_currentInventory->GetArmedInvenItemByLocationID(12);
			  if(weaponLink)
			  {
				  if((currentTickCount - playerObjectData->m_ammoRegenStamp) > (weaponLink->m_itemData->m_ammoSecTick * 1000))
				  {
					  if(weaponLink->m_itemData->m_chargeCurrent < weaponLink->m_itemData->m_maxCharges)
					  {						
						  weaponLink->m_itemData->m_chargeCurrent += (weaponLink->m_itemData->m_ammoRegenRate);
						  if(weaponLink->m_itemData->m_chargeCurrent > weaponLink->m_itemData->m_maxCharges)
							  weaponLink->m_itemData->m_chargeCurrent = weaponLink->m_itemData->m_maxCharges;
						  
						  m_sharedInterface->SendAttribMsg(	DPNSEND_GUARANTEED,
															playerObjectData->m_currentID,
															27,
															weaponLink->m_itemData->m_maxCharges,
															0,
															weaponLink->m_itemData->m_chargeCurrent,
															weaponLink->m_itemData->m_shotsPerFire,
															playerObjectData);
						  
						  //Check to see if they can fire
							if(weaponLink->m_itemData->m_tempDepletionHolder)
							{
								if((GetTickCount() - weaponLink->m_itemData->m_tempDepletionHolder) > weaponLink->m_itemData->m_weaponDepletionTimer)
								{
									weaponLink->m_itemData->m_tempDepletionHolder = 0;
									weaponLink->m_itemData->m_weaponDepletionTimer = 0;
									weaponLink->m_itemData->m_secsToGo = 0;
								}
							}
							playerObjectData->m_rechargeTimer = weaponLink->m_itemData->m_weaponDepletionTimer;
							m_sharedInterface->UpdateClientStatistics(playerObjectData, idFromLoc);
					/*	  m_sharedInterface->SendAttribMsg(	DPNSEND_GUARANTEED,
															playerObjectData->m_currentID,
															41,
															weaponLink->m_itemData->m_weaponDepletionTimer,
															0,
															0,
															0,
															playerObjectData); */
					  }
				      
					  playerObjectData->m_ammoRegenStamp = currentTickCount;
				  }
			  }
		   }//end backup/regen interval

		   m_sharedInterface->SendGuerenteedCycleMsgToClient(playerObjectData);//send cache

		   playerObjectData->LogPositionalMsg(formatedMessage);//log normally
			 
		   if(HandlePlayerStasis(playerObjectData,accountPtr,currentTickCount))
		   {
			   if(playerObjectData->m_Target3dPosition.y < -30000)
				  m_sharedInterface->RespawnToRebirthPoint(playerObjectData,accountPtr);//no internal lock
			   else
				  m_sharedInterface->SendUpdateToPlayer(idFromLoc,playerObjectData,FALSE);//send data that exists around player-locks items runtime internally
		   }
		   m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Unlock();
		   m_sharedInterface->m_netComObject->m_runtimeAccountDB->Unlock();
		   return TRUE;
	   }//end valid	 

	   return FALSE;
}
//------------------------------------------- Dot Updates ---------------------------------------//
BOOL CServerMain::HandleDotData(CPlayerObject *playerObjectData,CAccountObject * accountPtr)
{
	//Acquire the player
	CPlayerObject *playerWhoAddedDot = NULL;
	//Check to make sure we have a player
	if(!playerObjectData)
		return FALSE;

	//Error Check
	if(!playerObjectData->m_dotList)
		return FALSE;

	//Make sure the player has dots
	if(!playerObjectData->m_dotList->m_currentDotAmount)
		return FALSE;

	//Time to go through the DoTs
	for(POSITION posLoc = playerObjectData->m_dotList->GetHeadPosition(); posLoc != NULL;)
	{
		CDotClass *dotObj = (CDotClass *)playerObjectData->m_dotList->GetNext(posLoc);
		if((GetTickCount() - dotObj->m_dotTimeStamp) > 5000)
		{
			//Retrieve the player who added the dot
			playerWhoAddedDot = m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->GetByNetID(dotObj->m_dotPlayerID);
			
			//Typical Do Damage
			if(dotObj->m_dotType == DOTTYPE_DO_DAMAGE)
			{
				//remember to take into wound channels into concideration
				int damage = m_dmgSystems->WoundChannelCalculation(playerObjectData, dotObj->m_dotAmount, dotObj->m_dotChannel);
				if(playerObjectData->m_alive)
				{
					//If we have wounds in that channel, do not increase the healing amount
					if(damage <= 0)
						playerObjectData->m_energy -= dotObj->m_dotAmount;
					else
						playerObjectData->m_energy -= damage;
				}
				
				//Make sure we don't get overhealed on this DoT
				if(playerObjectData->m_energy > (playerObjectData->m_maxEnergy - playerObjectData->m_dotLowerMaxHP))
					playerObjectData->m_energy = playerObjectData->m_maxEnergy - playerObjectData->m_dotLowerMaxHP;
			}			
			//Drain Hp
			else if(dotObj->m_dotType == DOTTYPE_DRAIN_HP)
			{
				if(playerObjectData->m_alive)
				{
					playerObjectData->m_energy -= dotObj->m_dotAmount;
					//Check to make sure the player is valid
					if(playerWhoAddedDot)
					{
						playerWhoAddedDot->m_energy += dotObj->m_dotAmount;
						//Take into concideration their DoT's
						if(playerWhoAddedDot->m_energy > (playerWhoAddedDot->m_maxEnergy - playerWhoAddedDot->m_dotLowerMaxHP))
							playerWhoAddedDot->m_energy = playerWhoAddedDot->m_maxEnergy - playerWhoAddedDot->m_dotLowerMaxHP;
						
						//Only send an update if they are not an AI unit
						if(!playerWhoAddedDot->m_aiControlled)
							m_sharedInterface->UpdateClientStatistics(playerWhoAddedDot, playerWhoAddedDot->m_currentID);
					}
				}
			}
			//Drain MP
			//TODO:  change this when mana is transfered over to server side
			else if(dotObj->m_dotType == DOTTYPE_DRAIN_MP)
			{
				if(playerObjectData->m_alive)
				{
					playerObjectData->m_energy -= dotObj->m_dotAmount;
					if(playerWhoAddedDot)
					{
						playerWhoAddedDot->m_energy += dotObj->m_dotAmount;
						if(!playerWhoAddedDot->m_aiControlled)
							m_sharedInterface->UpdateClientStatistics(playerWhoAddedDot, playerWhoAddedDot->m_currentID);
					}
				}
			}

			//Play the effect if it has the attachment
			if(dotObj->m_dotEffectRef > -1)
				m_sharedInterface->SendDetailedMsgToClient(6,
														   dotObj->m_dotEffectRef,
														   playerObjectData->m_networkAssignedID,
														   0,
														   playerObjectData);
			//Update the client statistics (players only - the ! was missing, so players never saw DoT damage on their HUD)
			if(!playerObjectData->m_aiControlled)
				m_sharedInterface->UpdateClientStatistics(playerObjectData, playerObjectData->m_currentID);
		
			//If the player has died, no more need to go through DoTs
			if(playerObjectData->m_energy < 1)
				break;

			//Update the time stamp and lower the duration by 5 seconds
			dotObj->m_dotTimeStamp = GetTickCount();
			dotObj->m_dotDuration -= 5000;
			
			//Check the durations for player calculations
			if(dotObj->m_dotDuration <= 0)
			{
				if(dotObj->m_dotType == DOTTYPE_LOWER_DAMAGE)
					playerObjectData->m_dotDamageReduction[dotObj->m_dotChannel] -= dotObj->m_dotAmount;

				else if(dotObj->m_dotType == DOTTYPE_AMP_DAMAGE)
					playerObjectData->m_dotDamageTakeIncrease[dotObj->m_dotChannel] -= dotObj->m_dotAmount;

				else if(dotObj->m_dotType == DOTTYPE_LOWER_MAXHP)
					playerObjectData->m_dotLowerMaxHP -= dotObj->m_dotAmount;
				
				else if(dotObj->m_dotType == DOTTYPE_LOWER_MAXMP)
					playerObjectData->m_dotLowerMaxMP -= dotObj->m_dotAmount;
				
				else if(dotObj->m_dotType == DOTTYPE_NULL_JETS)
					playerObjectData->m_dotJetEffect = 0;
				
				else if(dotObj->m_dotType == DOTTYPE_NULL_SHIELDS)
					playerObjectData->m_dotShieldEffect = 0;
			}
		}
	}
	
	//Check to see if player died from a DoT
	if(playerObjectData->m_energy < 1)
	{
		playerObjectData->m_dotList->RemoveAllDots();
		playerObjectData->m_dotShieldEffect = 0;
		playerObjectData->m_dotJetEffect = 0;
		playerObjectData->m_dotLowerMaxHP = 0;
		playerObjectData->m_dotLowerMaxMP = 0;
		m_sharedInterface->HandleDeath(playerObjectData, playerWhoAddedDot);
	}
	
	//Otherwise check to see what DoTs need to be removed
	else
	{
		//Remove DoTs that time has expired
		playerObjectData->m_dotList->RemoveDotCheckByDuration();
	}

	return TRUE;
}
//------------------------------------------- Group Updates -------------------------------------//
/*
BOOL CServerMain::HandleGroupData_safe(DPNID &idFromLoc)
{
	CPlayerObject * playerObjectData = m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->GetByDPNID_Alive_LOS(idFromLoc);
	//Check to make sure we have a player
	if(!playerObjectData)
		return FALSE;
	
	//Make sure the player is in a group
	if(!playerObjectData->m_groupID)
	{
		m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Unlock();
		return FALSE;
	}

	//Lets only handle an update on players who have dirty info to send
	if(!playerObjectData->m_groupStruct.m_squadDataIsDirty)
	{
		m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Unlock();
		return FALSE;
	}

	//Acquire the player list for the given group number
	CAxList *playerList = m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->GetListByGroupID(playerObjectData->m_groupID);//already locked at this point
	ASSERT(playerList);

	//Tracker
	int dataFiller = 0;
	//Get the time for update
	DWORD newTimeUpdate = GetTickCount();
	
	//If we have a player list, start checking for update
	if(playerList)
	{
		//Loop through the players
		for(CAxLinkObj *posLoc = playerList->GetHeadPosition_unsafe(); posLoc != NULL;)
		{
			CPlayerObject *player = (CPlayerObject *)playerList->GetNext_unsafe(&posLoc);
			//Make sure we're in the same group
			if(player->m_groupID == playerObjectData->m_groupID)
			{
				/*
				if(player->m_groupStruct.m_groupPlacementIndex >= player->m_groupBlockTracker * 5 + 1 &&
			       player->m_groupStruct.m_groupPlacementIndex <= player->m_groupBlockTracker + 5)
				//
				{
					//Check for dirty information
					//if(player->m_groupStruct.m_squadDataIsDirty)
					{
						//Add to the packet here
						/*
						m_groupDataBlock[dataFiller].m_playerNetID = player->m_networkAssignedID;
						m_groupDataBlock[dataFiller].m_healthPercentage = (unsigned char)(player->m_energy * 100 / player->m_maxEnergy);
						m_groupDataBlock[dataFiller].m_manaPercentage = (unsigned char)(player->m_energy * 100 / player->m_maxEnergy); //TODO:  Change this to mana once its converted over to server side
						//
						m_groupDataBlock[dataFiller].NetID = player->m_networkAssignedID;
						m_groupDataBlock[dataFiller].Health = (unsigned char)(player->m_energy * 100 / player->m_maxEnergy);
						m_groupDataBlock[dataFiller].Position = player->m_groupStruct.m_groupPlacementIndex;

						// Ensure byte-aligned allocation for character names
						/*
						int iLength = player->m_charName.GetLength();
						int iPadding = iLength % 4;
						iLength += iPadding;
						ATL::CSimpleStringT<TCHAR, false>::PXSTR pName = player->m_charName.GetBufferSetLength(iLength);
						//

						m_groupDataBlock[dataFiller].NameLength = player->m_charName.GetLength() + 1; // +1 for null-terminator
						m_groupDataBlock[dataFiller].Name = new char[m_groupDataBlock[dataFiller].NameLength];
						strcpy_s( m_groupDataBlock[dataFiller].Name, 
							m_groupDataBlock[dataFiller].NameLength, 
							player->m_charName.GetBuffer());
						player->m_charName.ReleaseBuffer();						
 
						//Make sure we uncheck that is info is dirty so it doesn't get updated on the next pass
						player->m_groupStruct.m_squadDataIsDirty = 0;
						//Update their timer
						player->m_lastGroupUpdate = newTimeUpdate;
						//Increment the Filler
						dataFiller++;
					}

					//Check to see if we have the max amount in the datablock
					if(dataFiller == SQUAD_MESSAGE_BURST_SIZE)
						break;
				}
			}
			/*
			//Increment the block tracker
			player->m_groupBlockTracker++;
			//Make sure the next block isn't over the max amount of players, if it is, set it to back to the first block
			if(player->m_groupBlockTracker * 5 >= MAX_PLAYERS)
				player->m_groupBlockTracker = 0;
			//
		}

		//Nothing in need of an update
		if(!dataFiller)
		{
			m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Unlock();
			return FALSE;
		}

		//Send the update to each player in the group
		for(CAxLinkObj *posLoc = playerList->GetHeadPosition_unsafe(); posLoc != NULL;)
		{
			CPlayerObject *player = (CPlayerObject *)playerList->GetNext_unsafe(&posLoc);
			//Make sure we are dealing with only the ones in our group again
			if(player->m_groupID == playerObjectData->m_groupID)
			{
				/*
				player->m_groupBlockTracker++;
				if(player->m_groupBlockTracker * 5 >= MAX_PLAYERS)
					player->m_groupBlockTracker = 0;
				//

				m_sharedInterface->SendGroupMessage(player, m_groupDataBlock, dataFiller);
			}
		}

		// Release allocated space for character names
		for(int i=0; i<dataFiller; i++)
		{
			if(m_groupDataBlock[i].Name)
			{
				delete[] m_groupDataBlock[i].Name;
				m_groupDataBlock[i].Name = 0;
			}
		}
	}
	m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Unlock();
	return TRUE;
}
*/
//----------------------------------

/*BOOL CServerMain::HandleGroupData_safe(unsigned int iGroupID)
{
     m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Lock();
	 BOOL res = HandleGroupData(iGroupID);
	 m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Unlock();

     return res;
}

BOOL CServerMain::HandleGroupData(unsigned int iGroupID)
{
	bool bIsDirty = false;
	int dataFiller = 0;
	if(iGroupID < 1) return FALSE;

	// Acquire the player list for the given group number
	CAxList *playerList = m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->GetListByGroupID(iGroupID); // Already locked at this point
	if(!playerList)
	    return FALSE;

	// Get the time for update
	DWORD newTimeUpdate = GetTickCount();
	
	//If we have a player list, start checking for update
	if(playerList)
	{
		// Loop through the players
		for(CAxLinkObj *posLoc = playerList->GetHeadPosition_unsafe(); posLoc != NULL;)
		{
			// Make sure we're in the same group
			CPlayerObject *player = (CPlayerObject *)playerList->GetNext_unsafe(&posLoc);
			if(player->m_groupID == iGroupID)
			{
				// Check for dirty information
				if(player->m_groupStruct.m_squadDataIsDirty)
					bIsDirty = true;
			
				// Add to the packet here
				m_groupDataBlock[dataFiller].NetID = player->m_networkAssignedID;
				m_groupDataBlock[dataFiller].Health = (unsigned char)(player->m_energy * 100 / player->m_maxEnergy);
				m_groupDataBlock[dataFiller].Position = player->m_groupStruct.m_groupPlacementIndex;

				m_groupDataBlock[dataFiller].NameLength = player->m_charName.GetLength() + 1; // +1 for null-terminator
				m_groupDataBlock[dataFiller].Name = new char[m_groupDataBlock[dataFiller].NameLength];
				strcpy_s( m_groupDataBlock[dataFiller].Name, 
					m_groupDataBlock[dataFiller].NameLength, 
					player->m_charName.GetBuffer());
				player->m_charName.ReleaseBuffer();						

				player->m_groupStruct.m_squadDataIsDirty = 0; // Make sure we uncheck that is info is dirty so it doesn't get updated on the next pass
				player->m_lastGroupUpdate = newTimeUpdate; // Update their timer
				dataFiller++; // Increment the Filler

				// Check to see if we have the max amount in the datablock
				if(dataFiller == SQUAD_MESSAGE_BURST_SIZE)
					break;
			}
		}

		// Nothing in need of an update
		if(!bIsDirty || !dataFiller)
		{
			return FALSE;
		}

		// Send the update to each player in the group
		for(CAxLinkObj *posLoc = playerList->GetHeadPosition_unsafe(); posLoc != NULL;)
		{
			// Make sure we are dealing with only the ones in our group again
			CPlayerObject *player = (CPlayerObject *)playerList->GetNext_unsafe(&posLoc);
			if(player->m_groupID == iGroupID)
			{
				m_sharedInterface->SendGroupMessage(player, m_groupDataBlock, dataFiller);
			}
		}

		// Release allocated space for character names
		for(int i=0; i<dataFiller; i++)
		{
			if(m_groupDataBlock[i].Name)
			{
				delete[] m_groupDataBlock[i].Name;
				m_groupDataBlock[i].Name = 0;
			}
		}
	}
	return TRUE;
}*/

BOOL CServerMain::AccountAssignment(LPVOID & lpMessage,DPNID & idFromLoc,int & msgSize)
{
	if(msgSize > 0)
	{
		BYTE * blockCast = (BYTE*)lpMessage;
		delete [] blockCast;
		blockCast = 0;
	}

	return TRUE;
}
//----------------------------------
BOOL CServerMain::HandleCommandMsg_safe(LPVOID & lpMessage,DPNID & idFromLoc,int & msgSize)
{
	if(sizeof(COMMAND_DATA) == msgSize)
	{//valid command packet sz 
		LPCOMMAND_DATA formatedMessage = (LPCOMMAND_DATA)lpMessage;
		CPlayerObject * playerObjectData = m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->m_networkID);
		if(playerObjectData)
		{//valid player object
			if(formatedMessage->m_data == 0)
			{//withdrawal command obsolete
				
			}//end withdrawal command obsolete
			if(formatedMessage->m_data == 1)
			{//deposit command obsolete
				
			}//end deposit command
			if(formatedMessage->m_data == 2)
			{//join groups
				CPlayerObject * playerToInvite = m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->GetByNetID_Alive(formatedMessage->m_miscInt);
  
				if(!playerToInvite)
				{
					m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Unlock();
					return FALSE;
				}
			
				//Setup their group information
				if(playerObjectData->m_groupID <= 0)
				{	
					playerObjectData->m_groupStruct.m_groupLeadership = 1;
					playerObjectData->m_groupStruct.m_groupPlacementIndex = 1;
					playerObjectData->m_groupStruct.m_squadDataIsDirty = 1;
					playerObjectData->m_groupBlockTracker = 0;

					m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->ReIndexByGroupID(playerObjectData, playerObjectData->m_groupID, m_sharedInterface->m_globalGroupCounter);
				
					m_sharedInterface->m_globalGroupCounter++;
					if(m_sharedInterface->m_globalGroupCounter == UINT_MAX)
						m_sharedInterface->m_globalGroupCounter = 1;
				}
			
				CAxList *playerList = m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->GetListByGroupID(playerObjectData->m_groupID);
				if(!playerList) 
				{
					m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Unlock();
					return FALSE;
				}
				int groupCount = 1;
				for(CAxLinkObj *pos = playerList->GetHeadPosition_unsafe(); pos != NULL;)
				{
					CPlayerObject * plObj = (CPlayerObject *)playerList->GetNext_unsafe(&pos);
					if(plObj->m_groupID == playerObjectData->m_groupID)
						groupCount++;
				}

				// If the invited player is not the inviting player...
				if(playerToInvite->m_networkAssignedID != playerObjectData->m_networkAssignedID) 
				{
					playerToInvite->m_groupStruct.m_groupPlacementIndex = groupCount;
					playerToInvite->m_groupStruct.m_squadDataIsDirty = 1;
					playerToInvite->m_squadName = playerObjectData->m_squadName;
					playerToInvite->ReassembleName(m_sharedInterface);//reconstruct
					m_sharedInterface->SendClientScript(playerToInvite,"UpdateCharName("+playerToInvite->m_visName+")","");
					//playerToInvite->m_groupBlockTracker = playerObjectData->m_groupBlockTracker;
					m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->ReIndexByGroupID(playerToInvite, playerToInvite->m_groupID, playerObjectData->m_groupID);

					// Send out confirmation message
					m_sharedInterface->SendTextMsg(DPNSEND_GUARANTEED,
						playerToInvite->m_currentID,
						"You have joined " + playerObjectData->m_charName + "'s group.",
						0,
						playerToInvite,
						0);
				}

				// Send the update to each player in the group
				for(CAxLinkObj *posLoc = playerList->GetHeadPosition_unsafe(); posLoc != NULL;)
				{
					// Make sure we are dealing with only the ones in our group again
					CPlayerObject *player = (CPlayerObject *)playerList->GetNext_unsafe(&posLoc);
					if(player != playerToInvite)
					{
						if(player->m_groupID == playerObjectData->m_groupID)
							if(player->m_currentWorldID == playerToInvite->m_currentWorldID)
								m_sharedInterface->SendClientScript(player,"UpdateGroupCFG(" + m_sharedInterface->m_cabLib.ConvToString(playerToInvite->m_networkAssignedID) + "," + m_sharedInterface->m_cabLib.ConvToString(m_sharedInterface->m_groupGlidNotifier) + ")","");
					}
					else
					{
						for(CAxLinkObj *posLoc2 = playerList->GetHeadPosition_unsafe(); posLoc2 != NULL;)
						{
							CPlayerObject *player2 = (CPlayerObject *)playerList->GetNext_unsafe(&posLoc2);
							if(player2 != playerToInvite)
								if(playerToInvite->m_currentWorldID == player2->m_currentWorldID)
									m_sharedInterface->SendClientScript(playerToInvite,"UpdateGroupCFG(" + m_sharedInterface->m_cabLib.ConvToString(player2->m_networkAssignedID) + "," + m_sharedInterface->m_cabLib.ConvToString(m_sharedInterface->m_groupGlidNotifier) + ")","");
						}
					}
				}

				m_sharedInterface->SendAttribMsg(DPNSEND_GUARANTEED,
												 playerObjectData->m_currentID,
												 20,
												 0,
												 0,
												 ID_SQUAD_STATUS_MENU,
												 0,
												 playerObjectData);

				// Transmit updated group info to all members
				m_sharedInterface->HandleGroupData(playerObjectData->m_groupID);
			}//end join groups
			if(formatedMessage->m_data == 3)
			{//request join group
				CPlayerObject * playerToInvite = m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->GetByNetID(formatedMessage->m_miscInt);

				if(!playerToInvite)
				{
					m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Unlock();
					return FALSE;
				}

				if(playerToInvite->m_currentID!=0)//online
				{
					if(!playerToInvite->m_aiControlled)
					{
						BOOL invalid = FALSE;

						//Make sure whose doing the inviting has privledges to do so
						if(playerObjectData->m_groupID > 0 && playerObjectData->m_groupStruct.m_groupLeadership == 0)
						{
							m_sharedInterface->SendGeneralMessageHandler(94, playerObjectData);
							invalid = TRUE;
						}
						//Make sure they aren't already in our group
						else if(playerObjectData->m_groupID > 0 && playerToInvite->m_groupID == playerObjectData->m_groupID)
						{
							m_sharedInterface->SendGeneralMessageHandler(96, playerObjectData);
							invalid = TRUE;
						}
						//Make sure the person invited isn't in a group already
						else if(playerToInvite->m_groupID > 0)
						{
							m_sharedInterface->SendGeneralMessageHandler(95, playerObjectData);
							invalid = TRUE;
						}

						if(!invalid)
						{
							m_sharedInterface->SendAttribMsg(DPNSEND_GUARANTEED,
															 playerToInvite->m_currentID,
															 26,
															 201,//94,
			 												 playerObjectData->m_networkAssignedID,
															 0,
															 0,
															 playerToInvite);

							// Send an invite remind to the player of who they invited.
							m_sharedInterface->SendTextMsg(	DPNSEND_GUARANTEED,
															playerObjectData->m_currentID,
															"Invited " + playerToInvite->m_charName + " to join your group.",
															0,
															playerObjectData,
															0);
						}
					}
				}
			}//end request join group
			if(formatedMessage->m_data == 4)
			{//request group list
				//m_sharedInterface->RequestGroupList(playerObjectData->m_playerOrigRef,playerObjectData->m_currentID);//OPTIMIZED
				playerObjectData->m_groupStruct.m_squadDataIsDirty = 1;
				m_sharedInterface->HandleGroupData(playerObjectData->m_groupID);
			}//end request group list
			if(formatedMessage->m_data == 5)
			{//delete member by handle
				if(formatedMessage->m_miscInt > 0)
					m_sharedInterface->DeleteGroupMemberByHandle(playerObjectData,formatedMessage->m_miscInt);
			}//end delete member by handle
			if(formatedMessage->m_data == 6)
			{//general reload
				m_sharedInterface->GeneralReloadCommand(playerObjectData);
			}//end general reload
			if(formatedMessage->m_data == 7)
			{//send skill data
				m_sharedInterface->SendSkillsToClient(playerObjectData);
			}//end send skill data
			if(formatedMessage->m_data == 8)
			{//send skill data
				
			}//end send skill data
			if(formatedMessage->m_data == 10)
			{//Remove player from group
				CPlayerObject *playerToRemove = m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->GetByNetID(formatedMessage->m_miscInt);
				if((playerObjectData->m_groupID > 0) 
				&& (playerObjectData->m_groupStruct.m_groupLeadership > 0)
				&& (playerObjectData->m_networkAssignedID != playerToRemove->m_networkAssignedID))
				{
					unsigned char playerPositionHolder = 0;
					CAxList *playerList = m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->GetListByGroupID(playerObjectData->m_groupID);
					if(playerToRemove->m_groupID == playerObjectData->m_groupID)
					{
						playerPositionHolder = playerToRemove->m_groupStruct.m_groupPlacementIndex;
						playerToRemove->m_groupStruct.m_squadDataIsDirty = 0;
						playerToRemove->m_groupStruct.m_groupLeadership = 0;
						playerToRemove->m_groupStruct.m_groupPlacementIndex = 0;
						playerToRemove->m_groupBlockTracker = -1;
						m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->ReIndexByGroupID(playerToRemove, playerToRemove->m_groupID, 0);
					
						//Merge the players in the list
						m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->ResortGroupList(playerObjectData->m_groupID, playerPositionHolder);

						// Send out confirmation messages
						m_sharedInterface->SendTextMsg(DPNSEND_GUARANTEED,
							playerToRemove->m_currentID,
							"You have been removed from " + playerObjectData->m_charName + "'s group.",
							0,
							playerToRemove,
							0);

						m_sharedInterface->SendTextMsg(DPNSEND_GUARANTEED,
							playerObjectData->m_currentID,
							"You have removed " + playerToRemove->m_charName + " from your group.",
							0,
							playerObjectData,
							0);

						// Inform removed player of their removal
						m_sharedInterface->SendGroupMessage(playerToRemove, NULL, -1);

						// Transmit updated group info to all members
						playerObjectData->m_groupStruct.m_squadDataIsDirty = 1;
						m_sharedInterface->HandleGroupData(playerObjectData->m_groupID);
					}
				}
			}//End Remove player from group
			if(formatedMessage->m_data == 11)
			{//Give Authority to player
				CPlayerObject *playerToPromote = m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->GetByNetID(formatedMessage->m_miscInt);
				if((playerObjectData->m_groupID > 0)
				&& (playerObjectData->m_groupStruct.m_groupLeadership == 1)
				&& (playerObjectData->m_networkAssignedID != playerToPromote->m_networkAssignedID))
				{
					if(playerToPromote->m_groupID == playerObjectData->m_groupID)
					{
						playerToPromote->m_groupStruct.m_groupLeadership = 2;
						playerToPromote->m_groupStruct.m_squadDataIsDirty = 1;

						// Send out confirmation messages
						m_sharedInterface->SendTextMsg(DPNSEND_GUARANTEED,
							playerToPromote->m_currentID,
							"You have been promoted to a group leader!",
							0,
							playerToPromote,
							0);

						// Transmit updated group info to all members
						m_sharedInterface->HandleGroupData(playerObjectData->m_groupID);
					}
				}
			}//End Give Authority to player
			if(formatedMessage->m_data == 12)
			{//Handover Leadership
				CPlayerObject *playerToPromote = m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->GetByNetID(formatedMessage->m_miscInt);
				if((playerObjectData->m_groupID > 0)
				&& (playerObjectData->m_groupStruct.m_groupLeadership == 1)
				&& (playerObjectData->m_networkAssignedID != playerToPromote->m_networkAssignedID))
				{
					if(playerToPromote->m_groupID == playerObjectData->m_groupID)
					{
						// Preserve promoted player's position before promotion
						int iOldPosition = playerToPromote->m_groupStruct.m_groupPlacementIndex;;

						// Promote player
						playerToPromote->m_groupStruct.m_groupLeadership = 1;
						playerToPromote->m_groupStruct.m_groupPlacementIndex = 1;
						playerToPromote->m_groupStruct.m_squadDataIsDirty = 1;

						// Demote old leader
						playerObjectData->m_groupStruct.m_groupLeadership = 0;
						playerObjectData->m_groupStruct.m_groupPlacementIndex = iOldPosition;
						playerObjectData->m_groupStruct.m_squadDataIsDirty = 1;

						// Send out confirmation messages
						m_sharedInterface->SendTextMsg(DPNSEND_GUARANTEED,
							playerObjectData->m_currentID,
							"You have transferred leadership to " + playerToPromote->m_charName + ".",
							0,
							playerObjectData,
							0);

						m_sharedInterface->SendTextMsg(DPNSEND_GUARANTEED,
							playerToPromote->m_currentID,
							"You have received leadership priveleges from " + playerObjectData->m_charName + "!",
							0,
							playerToPromote,
							0);

						// Transmit updated group info to all members
						m_sharedInterface->HandleGroupData(playerObjectData->m_groupID);
					}
				}
			}//End Handover Leadership
			if(formatedMessage->m_data == 13)
			{//Leave group
				if(playerObjectData->m_groupID > 0)
				{
					unsigned char playerPositionHolder = 0;
					unsigned int groupHolder = playerObjectData->m_groupID;
				
					// Remove them from the group
					playerPositionHolder = playerObjectData->m_groupStruct.m_groupPlacementIndex;
					playerObjectData->m_groupStruct.m_squadDataIsDirty = 0;
					playerObjectData->m_groupStruct.m_groupLeadership = 0;
					playerObjectData->m_groupStruct.m_groupPlacementIndex = 0;
					playerObjectData->m_groupBlockTracker = -1;
					m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->ReIndexByGroupID(playerObjectData, playerObjectData->m_groupID, 0);
				
					// Resort List
					BOOL bResult = m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->ResortGroupList(groupHolder, playerPositionHolder);

					// Mark first group member's group struct as dirty
					CAxList *playerList = m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->GetListByGroupID(playerPositionHolder);
					CAxLinkObj *posLoc = (CAxLinkObj*)playerList->GetHeadPosition_unsafe();
					if(posLoc)
					{
						CPlayerObject *pMember = (CPlayerObject*)playerList->GetNext_unsafe(&posLoc);
						pMember->m_groupStruct.m_squadDataIsDirty = 1;

						// Transmit updated group info to all members
						m_sharedInterface->HandleGroupData(groupHolder);
					}

					// Send out confirmation message
					m_sharedInterface->SendTextMsg(DPNSEND_GUARANTEED,
							playerObjectData->m_currentID,
							"You have successfully left your group.",
							0,
							playerObjectData,
							0);

					// Inform removed player of their removal
					m_sharedInterface->SendGroupMessage(playerObjectData, NULL, -1);
				}
			}//end Leave group
			if(formatedMessage->m_data == 14)
			{//send skill data
				m_sharedInterface->SendSkillsToClient(playerObjectData, true);
			}//end send skill data
			if(formatedMessage->m_data == 15)
			{//create a group
				if(playerObjectData->m_groupID == 0)
				{
					playerObjectData->m_groupStruct.m_groupLeadership = 1;
					playerObjectData->m_groupStruct.m_groupPlacementIndex = 1;
					playerObjectData->m_groupStruct.m_squadDataIsDirty = 1;
					playerObjectData->m_groupBlockTracker = 0;

					if(!m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->ReIndexByGroupID(
						playerObjectData, 
						playerObjectData->m_groupID, 
						m_sharedInterface->m_globalGroupCounter))
						return FALSE;
					m_sharedInterface->m_globalGroupCounter++;
					if(m_sharedInterface->m_globalGroupCounter == UINT_MAX)
						m_sharedInterface->m_globalGroupCounter = 1;

					m_sharedInterface->SendTextMsg(DPNSEND_GUARANTEED,
						playerObjectData->m_currentID,
						"You've created a new group!",
						0,
						playerObjectData,
						0);//confermation

					// Send out the group data to the group creator
					m_sharedInterface->HandleGroupData(playerObjectData->m_groupID);
				}
			}//end create a group
	   }//end valid player object

	   m_sharedInterface->m_netComObject->m_runtimePlayersOnServer->Unlock();
	}//valid command packet sz 
	else
	{//disconnect
		m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Packet size invalid-disconnect Command Struct",idFromLoc); 
		m_sharedInterface->m_netComObject->Disconnect(idFromLoc);//disconnect for incomplience
	}//end disconnect
    
	return TRUE;
}
//----------------------------------
BOOL CServerMain::ProcessGuarenteedMsg_safe(LPVOID & lpMessage,DPNID & idFromLoc,int & msgSize)
{
       BYTE * blockCast = (BYTE*)lpMessage;
	   LPENTITYGUPDATE_HEADER formatedMessage = (LPENTITYGUPDATE_HEADER)lpMessage;
       int dataTracer = sizeof(ENTITYGUPDATE_HEADER);

       if(formatedMessage->m_attributeMsgCount > 0)
	   {//attribute batch coming in
		  int byteSize = sizeof(ATTRIB_STRUCT)*formatedMessage->m_attributeMsgCount;
	   
		  if((dataTracer+byteSize) > msgSize)
		  {//bounds check
		      m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Incorrect sized msg Attrib ProcessGuarenteedMsg_safe()");
              return TRUE;
		  }//end bounds check
		  
		  int offsetA = sizeof(ATTRIB_STRUCT);
		  for(int atLoop=0;atLoop<formatedMessage->m_attributeMsgCount;atLoop++)
		  {
              m_attributeSystems->HandleAttribMessageServerSide_safe((LPATTRIB_STRUCT)&blockCast[dataTracer],idFromLoc);
			  dataTracer+=offsetA;
		  }
	   }//end attribute batch coming in
	   if(formatedMessage->m_dmgMsgCount > 0)
	   {//dmg batch coming in
		  int byteSize = sizeof(HITPLAYER_DATA)*formatedMessage->m_dmgMsgCount;
	     
		  if((dataTracer+byteSize) > msgSize)
		  {//bounds check
		      m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Incorrect sized msg dmg ProcessGuarenteedMsg_safe()");
              return TRUE;
		  }//end bounds check
		 
		  int dmgTracer = 0;
		  int msSzTemp = sizeof(HITPLAYER_DATA);
		  for(int atLoop=0;atLoop<formatedMessage->m_dmgMsgCount;atLoop++)
		  {
			  
              m_dmgSystems->HandleDamageMsgServerSide_safe((LPHITPLAYER_DATA)&blockCast[dataTracer],msSzTemp,idFromLoc,dmgTracer);
			  dataTracer+=msSzTemp;
		  }
	   }//end dmg batch coming in
	   if(formatedMessage->m_equipMsgCount > 0)
	   {//equip batch coming in
		  int byteSize = sizeof(EQUIP_STRUCT)*formatedMessage->m_equipMsgCount;
	      
		  if((dataTracer+byteSize) > msgSize)
		  {//bounds check
		      m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Incorrect sized msg equip ProcessGuarenteedMsg()");
              return TRUE;
		  }//end bounds check
		  
		  int offsetA = sizeof(EQUIP_STRUCT);
		  for(int atLoop=0;atLoop<formatedMessage->m_equipMsgCount;atLoop++)
		  {
			  LPEQUIP_STRUCT equipStructTemp = (LPEQUIP_STRUCT)&blockCast[dataTracer];
			  m_sharedInterface->HandleEquipMessageServerSide_safe(equipStructTemp,idFromLoc);
			  dataTracer+=offsetA;
		  }
	   }//end equip batch coming in
	   if(formatedMessage->m_txtMsgCount > 0)
	   {//txt coming in
		  for(int txtLoop=0;txtLoop<formatedMessage->m_txtMsgCount;txtLoop++)
		  {
              if((dataTracer+1) > msgSize)
			  {//bounds check
		         m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Incorrect sized msg txt alloc ProcessGuarenteedMsg_safe()");
                 return TRUE;
			  }//end bounds check

		      int msgSz = blockCast[dataTracer];
			  dataTracer++;

			  if((dataTracer+msgSz) > msgSize)
			  {//bounds check
		         m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Incorrect sized msg txt ProcessGuarenteedMsg_safe()");
                 return TRUE;
			  }//end bounds check

                 m_textSystems->HandleTextMessageServerSide_safe(&blockCast[dataTracer],msgSz,idFromLoc);
			  dataTracer+=msgSz;
		  }
	   }//end txt coming in
	   if(formatedMessage->m_grpTxtMsgCount > 0)
	   {//group txt coming in
		  for(int txtLoop=0;txtLoop<formatedMessage->m_grpTxtMsgCount;txtLoop++)
		  {
              if((dataTracer+1) > msgSize)
			  {//bounds check
		         m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Incorrect sized msg group txt Alloc ProcessGuarenteedMsg_safe()");
                 return TRUE;
			  }//end bounds check

		      int msgSz = blockCast[dataTracer];
			  dataTracer++;
              
			  if((dataTracer+msgSz) > msgSize)
			  {//bounds check
		         m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Incorrect sized msg group txt ProcessGuarenteedMsg_safe()",(DWORD)idFromLoc);
                 return TRUE;
			  }//end bounds check
			  
			  m_textSystems->HandleTextMessageServerSideGroupStyle_safe(&blockCast[dataTracer],msgSz,idFromLoc);
		  }
	   }//end group txt coming in
	   if(formatedMessage->m_miscMsgCount > 0)
	   {//misc coming in
		  for(int txtLoop=0;txtLoop<formatedMessage->m_miscMsgCount;txtLoop++)
		  {
              if((dataTracer+1) > msgSize)
			  {//bounds check
		         m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Incorrect sized msg group mics alloc ProcessGuarenteedMsg_safe()");
                 return TRUE;
			  }//end bounds check

		      int msgSz = blockCast[dataTracer];
			  if(msgSz < sizeof(TRADE_HEADER))
			  {//bounds check
		         m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Incorrect definedSize smaller than trade header (2) ProcessGuarenteedMsg_safe()");
                 return TRUE;
			  }//end bounds check
			  dataTracer++;

              if((dataTracer+msgSz) > msgSize)
			  {//bounds check
		         m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Incorrect sized msg group misc ProcessGuarenteedMsg_safe()");
                 return TRUE;
			  }//end bounds check

			  dataTracer += msgSz;

		  }
	   }//end group txt coming in
	   return TRUE;
}

//----------------------------------
BOOL CServerMain::ProcessGuarenteedMsgAI_safe(LPVOID & lpMessage,DPNID & idFromLoc,int & msgSize)
{
	   if(!m_sharedInterface->m_netComObject->m_runtimeAccountDB->VerifyAdmin_safe(idFromLoc))
	   {
		  m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Non admin account -disconnecting ProcessGuarenteedMsgAI_safe()...","dpnid:",idFromLoc);
	      m_sharedInterface->m_netComObject->Disconnect(idFromLoc);//disconnect for incomplience
          return FALSE;
	   }

       BYTE * blockCast = (BYTE*)lpMessage;
	   AI_LPENTITYGUPDATE_HEADER formatedMessage = (AI_LPENTITYGUPDATE_HEADER)lpMessage;
       int dataTracer = sizeof(AI_ENTITYGUPDATE_HEADER);

       if(formatedMessage->m_attributeMsgCount > 0)
	   {//attribute batch coming in
		  int byteSize = sizeof(ATTRIB_STRUCT)*formatedMessage->m_attributeMsgCount;
		  if((dataTracer+byteSize) > msgSize)
		  {//bounds check
		      m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Incorrect sized msg Attrib ProcessGuarenteedMsgAI_safe()");
              return TRUE;
		  }//end bounds check
		  for(int atLoop=0;atLoop<formatedMessage->m_attributeMsgCount;atLoop++)
		  {
              m_attributeSystems->HandleAttribMessageServerSide_safe((LPATTRIB_STRUCT)&blockCast[dataTracer],idFromLoc);
			  dataTracer += sizeof(ATTRIB_STRUCT);
		  }
	   }//end attribute batch coming in
	   if(formatedMessage->m_dmgMsgCount > 0)
	   {//dmg batch coming in
		  int byteSize = sizeof(HITPLAYER_DATA)*formatedMessage->m_dmgMsgCount;
		  if((dataTracer+byteSize) > msgSize)
		  {//bounds check
		      m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Incorrect sized msg dmg ProcessGuarenteedMsgAI_safe()");
              return TRUE;
		  }//end bounds check
		  
		  int dmgTracer = 0;
	      int msSzTemp = sizeof(HITPLAYER_DATA);
		  for(int atLoop=0;atLoop<formatedMessage->m_dmgMsgCount;atLoop++)
		  {
			  //int msSzTemp = sizeof(HITPLAYER_DATA);
              m_dmgSystems->HandleDamageMsgServerSide_safe(&blockCast[dataTracer],msSzTemp,idFromLoc,dmgTracer);
			  dataTracer += sizeof(HITPLAYER_DATA);
		  }
	   }//end dmg batch coming in
	   if(formatedMessage->m_equipMsgCount > 0)
	   {//equip batch coming in
		  int byteSize = sizeof(EQUIP_STRUCT)*formatedMessage->m_equipMsgCount;
	      
		  if((dataTracer+byteSize) > msgSize)
		  {//bounds check
		      m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Incorrect sized msg equip ProcessGuarenteedMsgAI_safe()");
              return TRUE;
		  }//end bounds check
		  
		  EQUIP_STRUCT eStruct;
		  for(int atLoop=0;atLoop<formatedMessage->m_equipMsgCount;atLoop++)
		  {
              CopyMemory(&eStruct,&blockCast[dataTracer],sizeof(EQUIP_STRUCT));
			  m_sharedInterface->HandleEquipMessageServerSide_safe(&eStruct,idFromLoc);
			  dataTracer += sizeof(EQUIP_STRUCT);
		  }
	   }//end equip batch coming in
	   if(formatedMessage->m_txtMsgCount > 0)
	   {//txt coming in
		  for(int txtLoop=0;txtLoop<formatedMessage->m_txtMsgCount;txtLoop++)
		  {
              if((dataTracer+1) > msgSize)
			  {//bounds check
		         m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Incorrect sized msg txt alloc ProcessGuarenteedMsgAI_safe()");
                 return TRUE;
			  }//end bounds check

		      int msgSz = blockCast[dataTracer];
			  dataTracer++;

			  if((dataTracer+msgSz) > msgSize)
			  {//bounds check
		         m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Incorrect sized msg txt ProcessGuarenteedMsgAI_safe()");
                 return TRUE;
			  }//end bounds check

              m_textSystems->HandleTextMessageServerSide_safe(&blockCast[dataTracer],msgSz,idFromLoc);
		  }
	   }//end txt coming in
	   if(formatedMessage->m_grpTxtMsgCount > 0)
	   {//group txt coming in
		  for(int txtLoop=0;txtLoop<formatedMessage->m_grpTxtMsgCount;txtLoop++)
		  {
              if((dataTracer+1) > msgSize)
			  {//bounds check
		         m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Incorrect sized msg group txt Alloc ProcessGuarenteedMsgAI_safe()");
                 return TRUE;
			  }//end bounds check

		      int msgSz = blockCast[dataTracer];
			  dataTracer++;
              
			  if((dataTracer+msgSz) > msgSize)
			  {//bounds check
		         m_sharedInterface->m_netComObject->m_eventLog->AddEvent("AI Incorrect sized msg group txt ProcessGuarenteedMsgAI_safe()",(DWORD)idFromLoc);
                 return TRUE;
			  }//end bounds check
			  
			  BYTE * txtBlock = new BYTE[msgSz];
              for(int i=0;i<msgSz;i++)
			  {
                  txtBlock[i] = blockCast[dataTracer];
			      dataTracer++;
			  }
			  m_textSystems->HandleTextMessageServerSideGroupStyle_safe(txtBlock,msgSz,idFromLoc);

			  
              delete [] txtBlock;
			  txtBlock = 0;
		  }
	   }//end group txt coming in
	   if(formatedMessage->m_miscMsgCount > 0)
	   {//misc coming in
		  for(int txtLoop=0;txtLoop<formatedMessage->m_miscMsgCount;txtLoop++)
		  {
              if((dataTracer+1) > msgSize)
			  {//bounds check
		         m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Incorrect sized msg group mics alloc ProcessGuarenteedMsgAI_safe()");
                 return TRUE;
			  }//end bounds check

		      int msgSz = blockCast[dataTracer];
			  dataTracer++;

              if((dataTracer+msgSz) > msgSize)
			  {//bounds check
		         m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Incorrect sized msg group misc ProcessGuarenteedMsgAI_safe()");
                 return TRUE;
			  }//end bounds check

              dataTracer += msgSz;
		  }
	   }//end group txt coming in
	   if(formatedMessage->m_fireMsgCount > 0)
	   {//fire msg batch coming in
		  int byteSize = sizeof(FIRE_STRUCT)*formatedMessage->m_fireMsgCount;
		  if((dataTracer+byteSize) > msgSize)
		  {//bounds check
		      m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Incorrect sized msg Attrib ProcessGuarenteedMsgAI_safe()");
              return TRUE;
		  }//end bounds check

		  for(int atLoop=0;atLoop<formatedMessage->m_fireMsgCount;atLoop++)
		  {
			  dataTracer++;
			  m_attackSystemsManagment->HandleFireMessagesServerSide_safe((LPFIRE_STRUCT)&blockCast[dataTracer],idFromLoc);
			  dataTracer+=sizeof(FIRE_STRUCT);
		  }
	   }//end fire msg batch coming in

	   return TRUE;
}
//----------------------------------
BOOL CServerMain::ProcessBlockAttribMsgs_safe(LPVOID & lpMessage,DPNID & idFromLoc,int & msgSize)
{
       BYTE * blockCast = (BYTE*)lpMessage;
	   int attribCount = msgSize/sizeof(ATTRIB_STRUCT);
	   if(attribCount == 0 || msgSize < 1)
	   {//invalid
          return TRUE;
	   }//end invalid
	  
	   int offset = sizeof(ATTRIB_STRUCT);
	   for(int atLoop=0;atLoop<attribCount;atLoop++)
	   {
           m_attributeSystems->HandleAttribMessageServerSide_safe((LPATTRIB_STRUCT)&blockCast[(atLoop*offset)+1],idFromLoc);
	   }

       return TRUE;
}
//----------------------------------
BOOL CServerMain::AdminAcRequestConfirm(LPVOID & lpMessage,DPNID & idFromLoc,int & msgSize)
{
      if(sizeof(EXMSG_CATAGORIE) == msgSize)
	  {//valid add account packet sz 
       LPEXMSG_CATAGORIE formatedMessage = (LPEXMSG_CATAGORIE)lpMessage;
          EXMSG_CATAGORIE dataPacket;
	      int messageSize = sizeof(EXMSG_CATAGORIE);
          dataPacket.m_catagorie = 1;

          m_sharedInterface->m_netComObject->SendData((BYTE *)&dataPacket,
				                             messageSize,
				                             DPNSEND_GUARANTEED,
				                             idFromLoc,
											 m_sharedInterface->m_netComObject->m_packetTimeout);

	   delete formatedMessage;
	   formatedMessage=0;
	  }//end valid add account packet sz 
	  else{//diconnect
         m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Packet size invalid-disconnect Add account reciet Struct",idFromLoc); 
		 m_sharedInterface->m_netComObject->Disconnect(idFromLoc);//disconnect for incomplience
		 BYTE * blockCast = (BYTE*)lpMessage;
		 delete [] blockCast;
		 blockCast = 0;
	  }//end diconnect
      return TRUE;
}
//----------------------------------
BOOL CServerMain::HandleSpawn_safe(LPVOID & lpMessage,DPNID & idFromLoc,int & msgSize)
{
        if(sizeof(SPAWN_STRUCT) == msgSize)
		{//valid packet sz 
          LPSPAWN_STRUCT formatedMessage = (LPSPAWN_STRUCT)lpMessage;
          BOOL isAiServer = FALSE;

          if(!m_sharedInterface->m_netComObject->m_runtimeAccountDB->VerifyAIserver_safe(idFromLoc,isAiServer))
		  {
              m_sharedInterface->m_netComObject->m_eventLog->AddEvent("***VerifyAIserver_safe(dpnid)  cannot find account by dpnid",idFromLoc); 
		      m_sharedInterface->m_netComObject->Disconnect(idFromLoc);//disconnect for incomplience
              return FALSE;
		  }

          float serverSideMsgCRC = (float)m_sharedInterface->m_netComObject->GetSpawnCRC(formatedMessage);
          float crcCheck = (float)formatedMessage->CRCcheck;
	      if(!m_sharedInterface->m_netComObject->CRCInValid(serverSideMsgCRC,crcCheck))//check packet validity
		  {//crc
                if(isAiServer)
                   m_spawnSystems->HandleAISpawnMessagesServerSide_safe(formatedMessage,idFromLoc);
				else
				{//post to router
                   //verify target world via account
				   int optInstanceType = 0;
				   CAccountObject * acPtrTemp = m_sharedInterface->m_netComObject->m_runtimeAccountDB->GetAccountByID_LOS(idFromLoc);
				   if(acPtrTemp)
				   {//locked account
                       if(acPtrTemp->m_characterOnAccount)
					   {//valid character 
						 if(acPtrTemp->m_characterOnAccount->m_currentWorldMap.GetLength() < 25)
					     {
                           ZeroMemory(formatedMessage->worldName,25);
						   if(acPtrTemp->m_characterOnAccount->m_currentWorldMap.GetLength() > 0)
								CopyMemory(formatedMessage->worldName,acPtrTemp->m_characterOnAccount->m_currentWorldMap.GetBuffer(),acPtrTemp->m_characterOnAccount->m_currentWorldMap.GetLength());
						   optInstanceType = acPtrTemp->m_characterOnAccount->m_instanceType;

						   // Add spawn packet to message queue
						   m_sharedInterface->m_threadedDataProcessor->AddToQue(    0,//dword
																					"",//text
																					0,//int
																					idFromLoc,//dpnid
																					1,//type
																					(byte*)formatedMessage,//byte data
																					sizeof(SPAWN_STRUCT),//byte length
																					0,//dword2
																					optInstanceType,
																					0,
																					0);//int2
						 }
						 else
                           m_sharedInterface->m_eventLog->LogByPriority("HandleSpawn_safe() FAILED WORLD LENGTH:"+acPtrTemp->m_characterOnAccount->m_currentWorldMap,2);
					   }//end valid character
					   else
					      m_sharedInterface->m_eventLog->LogByPriority("HandleSpawn_safe() FAILED VALIDIDATE CHARACTER:",2);
				   
				       m_sharedInterface->m_netComObject->m_runtimeAccountDB->Unlock();
				   }//end locked account
				   else
				      m_sharedInterface->m_eventLog->LogByPriority("HandleSpawn_safe() FAILED VALIDIDATE ACCOUNT:",2);

				   
				}//end post to router
		  }//end crc
	      else
		  {//bad spawn msg CRC
		        m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Bad Spawn CRC Recorded");
				m_sharedInterface->m_netComObject->Disconnect(idFromLoc);//disconnect for incomplience
		  }//end bad spawn msg CRC
		}//end valid packet sz 
		else{//diconnect
          m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Packet size invalid-disconnect spawn Struct",idFromLoc); 
		  m_sharedInterface->m_netComObject->Disconnect(idFromLoc);//disconnect for incomplience
		}//end diconnect
	return TRUE;
}
//----------------------------------
BOOL CServerMain::HandleMovFire_safe(LPVOID & lpMessage,DPNID & idFromLoc,int & msgSize)
{
            BYTE * blockCast = (BYTE*)lpMessage;
			if(msgSize < (2+sizeof(SHRUNK_TOSVRMOV_STRUCT)))
			{		  
				m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Bad mov cat:5 msg"); 
				return FALSE;
			}
			HandleMovData_safe((LPSHRUNK_TOSVRMOV_STRUCT)&blockCast[2],idFromLoc);//process this data
			int fireMsgs = blockCast[1];
			int datatracer = 2+sizeof(SHRUNK_TOSVRMOV_STRUCT);

			//Handle the Group Info
			m_sharedInterface->HandleGroupData_safe(idFromLoc);

			if(fireMsgs <= 0)
			{
		       return FALSE;
			}
		
			if((fireMsgs*(int)sizeof(FIRE_STRUCT))+datatracer > msgSize)
			{
				return FALSE;
			}

			for(int iLx=0;iLx<fireMsgs;iLx++)
			{
				m_attackSystemsManagment->HandleFireMessagesServerSide_safe((LPFIRE_STRUCT)&blockCast[datatracer],idFromLoc);
                datatracer += sizeof(FIRE_STRUCT);
			}

	return TRUE;
}
//----------------------------------
BOOL CServerMain::FireMsgHandler(LPVOID & lpMessage,DPNID & idFromLoc,int & msgSize)
{
            if(sizeof(FIRE_STRUCT) == msgSize)
			{//valid packet sz
				m_attackSystemsManagment->HandleFireMessagesServerSide_safe((LPFIRE_STRUCT)lpMessage,idFromLoc);
			}//end valid packet sz
			else
			{//invalid packet sz
				m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Packet size invalid-disconnect Fire Struct",idFromLoc); 
				m_sharedInterface->m_netComObject->Disconnect(idFromLoc);//disconnect for incomplience
			}//end invalid packet sz
			BYTE * blockCast = (BYTE*)lpMessage;
			delete [] blockCast;
		    blockCast = 0;
			return FALSE;
}
//----------------------------------
BOOL CServerMain::MovMsgHandler_safe(LPVOID & lpMessage,DPNID & idFromLoc,int & msgSize)
{
            if(sizeof(SHRUNK_TOSVRMOV_STRUCT) == msgSize)
			{//valid packet sz 
				HandleMovData_safe((LPSHRUNK_TOSVRMOV_STRUCT)lpMessage,idFromLoc);
			}//end valid packet sz 
			else{//diconnect
				m_sharedInterface->m_netComObject->m_eventLog->AddEvent("Packet size invalid-disconnect Mov Struct",idFromLoc); 
				m_sharedInterface->m_netComObject->Disconnect(idFromLoc);//disconnect for incomplience
			}//end diconnect
			return FALSE;
}
//----------------------------------
BOOL CServerMain::RebootServer()
{
   HANDLE hToken; 
   TOKEN_PRIVILEGES tkp; 
 
   // Get a token for this process. 
 
   if (!OpenProcessToken(GetCurrentProcess(), 
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
      return FALSE; 
 
   // Get the LUID for the shutdown privilege. 
 
   LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, 
        &tkp.Privileges[0].Luid); 
 
   tkp.PrivilegeCount = 1;  // one privilege to set    
   tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
 
   // Get the shutdown privilege for this process. 
 
   AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
        (PTOKEN_PRIVILEGES)NULL, 0); 
 
   if (GetLastError() != ERROR_SUCCESS) 
      return FALSE; 

    return ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);
}
