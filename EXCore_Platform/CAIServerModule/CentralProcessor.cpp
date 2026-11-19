#include "stdafx.h"
#include "centralprocessor.h"

DWORD WINAPI PrimaryPacketProcessing(LPVOID lpParameter);
DWORD WINAPI NetworkThreadLoop(LPVOID lpParameter);
DWORD WINAPI NetworkLoggingLoop(LPVOID lpParameter);

CCentralProcessor::CCentralProcessor(int connectionAttempts)
{
	InitializeCriticalSection(  &m_criticalWldSpawn);
	m_threadCount		= 0;
	m_connected			= FALSE;
	m_axThreadID        = NULL;
	m_axThreadHandle    = NULL;
	m_lgThreadHandle    = NULL;
	m_gCycleUpdateStamp = GetTickCount();
	m_gCycleUpdateFrequency = 100;
    m_aiWorldsList		= new CWorldManagerDatabase();
	m_aiServer			= new CAIServerInitialization();
	if(!m_aiServer->m_adoConnection->m_connectionEstablished)
	{
        m_aiServer->m_eventLog->AddEvent("not starting ai server due to connection in sql not being established");
	}
	else if(m_aiServer->ConnectToServer(connectionAttempts)==TRUE)
	{//if connection is good
       //set safe multithread flags
	   m_aiServer->m_eventLog->EnableCriticalSection();
       m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->SetMultithreadSafeFlags();
	   m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_AIServer = TRUE;
       
       //end set safe multithread flags
       m_connected = TRUE;
	   m_axThreadHandle = CreateThread(NULL,0,PrimaryPacketProcessing,this,0,&m_axThreadID);//create primary processing thread
       m_threadCount++;

	   DWORD m_lgThreadID; 
       m_lgThreadHandle = CreateThread(NULL,0,NetworkLoggingLoop,this,0,&m_lgThreadID);
	   m_threadCount++;
       //Load worlds
	   LoadWorldSystems();
	}//end if connection is good
}

void CCentralProcessor::LoadWorldSystems()
{
    _RecordsetPtr rdPtr = m_aiServer->m_adoConnection->Query("select * from tblAIBaseWorlds");
	if(rdPtr==NULL)
	{
         m_aiServer->m_eventLog->AddEvent("No AI worlds to load");
		 return;
	}
	int wCount = m_aiServer->m_adoConnection->GetCount(rdPtr);
	m_aiServer->m_eventLog->AddEvent("AI worlds aquired count::",(DWORD)wCount);
	for(int loop=0;loop<wCount;loop++)
	{
        int worldChannel = m_aiServer->m_adoConnection->GetIntData("worldChannel",rdPtr);
        CString exgBaseFileName = m_aiServer->m_adoConnection->GetStringData("EXGBaseName",rdPtr);
		CString altCollisonFileName = m_aiServer->m_adoConnection->GetStringData("AltCollisionFile",rdPtr);
        SpawnAIWorld(exgBaseFileName,altCollisonFileName,worldChannel);
		m_aiServer->m_adoConnection->NextRecord(rdPtr);
	}
}

CCentralProcessor::~CCentralProcessor(void)
{
	m_aiServer->m_eventLog->OutPutTextFile("Log.txt");
	EnterCriticalSection( &m_aiServer->m_interactSection);
    m_aiServer->m_systemShutDown = TRUE;
    LeaveCriticalSection( &m_aiServer->m_interactSection);
	Sleep(2000);

	EnterCriticalSection( &m_aiServer->m_interactSection);
	while(m_threadCount > 0)
	{
		LeaveCriticalSection( &m_aiServer->m_interactSection);
		Sleep(1000);
		EnterCriticalSection( &m_aiServer->m_interactSection);
	}
	LeaveCriticalSection( &m_aiServer->m_interactSection);
	
    if(m_aiWorldsList)
	{
       delete m_aiWorldsList;
	   m_aiWorldsList = 0;
	}
	if(m_aiServer)
	{
       delete m_aiServer;
	   m_aiServer = 0;
	}
	if(m_axThreadHandle)
	{
	   CloseHandle(m_axThreadHandle);
	}
	if(m_lgThreadHandle)
	{
       CloseHandle(m_lgThreadHandle);
	}
	DeleteCriticalSection(&m_criticalWldSpawn);
}

//------------------------------------------------------------------------------------//
void CCentralProcessor::SendPendingCfgRequestsCP()
{
	m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_clientCfgRequestOutbox->EnterCriticalSectionEXA();
    int segCount = (int)m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_clientCfgRequestOutbox->GetCount();

    if(segCount < 1)
	{
	   m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_clientCfgRequestOutbox->LeaveCriticalSectionEXA();
	   return;//none to request
	}

	int totalDataSize = 1;
	for(POSITION posLoc = m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_clientCfgRequestOutbox->GetHeadPosition();posLoc != NULL;)
	{
        CPacketObj * pckPtr = (CPacketObj *)m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_clientCfgRequestOutbox->GetNext(posLoc);
        if(pckPtr) totalDataSize += pckPtr->m_size;
	}

	BYTE * rawDataBlock = new BYTE[totalDataSize];
	rawDataBlock[0] = 24;//catagorie
	int datatrace = 1;

	for(POSITION posLoc = m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_clientCfgRequestOutbox->GetHeadPosition();posLoc != NULL;)
	{
        CPacketObj * pckPtr = (CPacketObj *)m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_clientCfgRequestOutbox->GetNext(posLoc);
        if(pckPtr && pckPtr->m_data && pckPtr->m_size > 0 && (datatrace + pckPtr->m_size) <= totalDataSize)
        {
            CopyMemory(&rawDataBlock[datatrace], pckPtr->m_data, pckPtr->m_size);
            datatrace += pckPtr->m_size;
        }
	}
	m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_clientCfgRequestOutbox->Flush();//flush
    m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_clientCfgRequestOutbox->LeaveCriticalSectionEXA();

	m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->SendData(rawDataBlock,
				                             datatrace,
				                             DPNSEND_NOCOMPLETE | DPNSEND_PRIORITY_HIGH,
				                             NULL,
							                 m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_packetTimeout);

	delete [] rawDataBlock;
	rawDataBlock = 0;
}
//-----------------------------------------------------------//
BOOL CCentralProcessor::SpawnAIWorld(CString worldEXG, CString altCollisionCRD, int channelID)
{
	int aiID = channelID;
    CWorldManager * aiWldPtr = new CWorldManager(m_aiServer,aiID);
	if(!aiWldPtr->InitWorldLocalData(worldEXG,altCollisionCRD))
	{
        delete aiWldPtr;
		aiWldPtr = 0;
	    return FALSE;
	}
	aiWldPtr->m_worldChannel = channelID;
	aiWldPtr->m_axThreadHandle = CreateThread(NULL,0,NetworkThreadLoop,aiWldPtr,0,&aiWldPtr->m_axThreadID);
    m_aiServer->m_clientModule->m_initObject->m_gStatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED,
																                             DPID_SERVERPLAYER,
																							 47,
																							 channelID,
				 																			 0,
																							 0,
																							 0,
																							 NULL);//request house game state for the channel
	
	EnterCriticalSection(&m_criticalWldSpawn);
	bool result = m_aiWorldsList->Add_safe(aiWldPtr);
	LeaveCriticalSection(&m_criticalWldSpawn);
	if(result)
	   m_aiServer->m_eventLog->AddEvent("SpawnAIWorld() wld:"+worldEXG+" Channel:",(DWORD)channelID);
	else
	   m_aiServer->m_eventLog->AddEvent("Failed SpawnAIWorld() wld:"+worldEXG+" Channel:",(DWORD)channelID);

	return TRUE;
}
//-----------------------------------------------------------//
DWORD WINAPI PrimaryPacketProcessing(LPVOID lpParameter)
{	   
	    CCentralProcessor * classObj = (CCentralProcessor *)lpParameter; 
		if(FAILED(CoInitialize(NULL)))
		{
           return 0;
		}
		//initialize logon
        if(!classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_enabledCommunicationInit)
        {//must do this first and once
            
			classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_timeElStamp					= classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->GetTimeStamp();
			classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_enabledCommunicationInit		= TRUE;
        }
		//continue to process loop
		LPVOID lpMessage = NULL;
        DPNID messageFrom = 0;
        int sizeTracker = 0;
        DWORD currentTickCount = GetTickCount();

        while(1)
		{
			currentTickCount = GetTickCount();
            EnterCriticalSection( &classObj->m_aiServer->m_interactSection);
            if(classObj->m_aiServer->m_systemShutDown==TRUE)
			{
               LeaveCriticalSection( &classObj->m_aiServer->m_interactSection);
			   break;
			}
			LeaveCriticalSection( &classObj->m_aiServer->m_interactSection);

			while(classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_packetQue->GetNextPacket_safe(&sizeTracker,(BYTE**)&lpMessage,&messageFrom)==TRUE)
			{//if theres a message handle it
                LPEXMSG_CATAGORIE identityMsg = (LPEXMSG_CATAGORIE)lpMessage;
                int worldChannel = -1;
				if(identityMsg->m_catagorie == 18)
				{//primary game state data
                    LPAIPOS_HEADER headerX = (LPAIPOS_HEADER)lpMessage;
					worldChannel = headerX->m_worldChannel;
				}//end primary game state data
				else if(identityMsg->m_catagorie == 40)
				{//dynamic ai spawn command
                    LPAIDYNSPAWN_POS headerX = (LPAIDYNSPAWN_POS)lpMessage;
					worldChannel = headerX->m_worldChannel;

					
					//classObj->m_aiServer->m_eventLog->AddEvent("AI DYN SPAWN COMMAND CHNL:",(DWORD)abs(worldChannel));
					//classObj->m_aiServer->m_eventLog->AddEvent("rcv currentTime:",currentTickCount);
				}//end dynamic ai spawn command
				else if(identityMsg->m_catagorie == 7)
				{//house static placement
					byte * castByteArray = (byte*)lpMessage;
                    CopyMemory(&worldChannel,&castByteArray[sizeTracker-4],4);
				}//end house static placement
				else if(identityMsg->m_catagorie == 6)
				{//Attribute placement
					LPATTRIB_STRUCT headerA = (LPATTRIB_STRUCT)lpMessage;
					if(headerA->attribute == 10)
                       worldChannel = headerA->uniInt2;
				}//end Attribute placement
				else if(identityMsg->m_catagorie == 101)
				{//dynamic ai channel instance spawned
					LPAIDYNSPAWN_POS headerX = (LPAIDYNSPAWN_POS)lpMessage;
					CWorldManager * wldCheck = classObj->m_aiWorldsList->GetByChannel(headerX->m_worldChannel);
					if(wldCheck)
					   worldChannel = -102;//que for error report
					else
					{//do spawn world
                       classObj->SpawnAIWorld(headerX->worldName,"",headerX->m_worldChannel);//spawns instance onto set channel
					   worldChannel = -101;
					}//end do spawn world
				}//end dynamic ai channel instance spawned
				else if(identityMsg->m_catagorie == 102)
				{//delete channel (worldmanager)
					LPAIDYNSPAWN_POS headerX = (LPAIDYNSPAWN_POS)lpMessage;
					classObj->m_aiWorldsList->RemoveByObject_safe(headerX->m_worldChannel);//close channel-flags thread to close
					worldChannel = -103;
				}//end delete channel (worldmanager)

                if(worldChannel > -1)
				{//ai server data
					CPacketObj * packetPtr = new CPacketObj();
					packetPtr->m_size = sizeTracker;
				    packetPtr->m_data = (byte*)lpMessage;
				    packetPtr->m_communicationID = messageFrom;
					if(!classObj->m_aiWorldsList->AddPacket_safe(packetPtr,worldChannel))//will delete packet mem if world doesnt exist
						classObj->m_aiServer->m_eventLog->AddEvent("AI world not found Channel:",(DWORD)worldChannel);
				}//end ai server data
				else
				{//not found delete memory
                   if(worldChannel == -101)
				   {//dynamic ai instance spawned
					   LPAIDYNSPAWN_POS headerX = (LPAIDYNSPAWN_POS)lpMessage;
                       classObj->m_aiServer->m_eventLog->AddEvent("Dynamic ai instance spawned:",headerX->worldName,0);
				   }
				   else if(worldChannel == -102)
				   {//dynamic ai instance spawned
					   LPAIDYNSPAWN_POS headerX = (LPAIDYNSPAWN_POS)lpMessage;
                       classObj->m_aiServer->m_eventLog->AddEvent("Dynamic ai instance spawned failed due to duplicate channel:",headerX->worldName,0);
				   }
				   else if(worldChannel == -103)
				   {//dynamic ai instance spawned
					   LPAIDYNSPAWN_POS headerX = (LPAIDYNSPAWN_POS)lpMessage;
                       classObj->m_aiServer->m_eventLog->AddEvent("Dynamic ai instance cued to close Channel:",(DWORD)headerX->m_worldChannel);
				   }
				   else
				   {//failed
					   classObj->m_aiServer->m_eventLog->AddEvent("Invalid AI server msg incomming cat:",(DWORD)identityMsg->m_catagorie);
					   if(sizeTracker > 0)
					   {
						  if(identityMsg->m_catagorie == 6 && sizeof(ATTRIB_STRUCT) == sizeTracker)
						  {//error detection use
							 ATTRIB_STRUCT * attrib = (ATTRIB_STRUCT *)lpMessage;
							 classObj->m_aiServer->m_eventLog->AddEvent("Catagorie identified as attribute:",(DWORD)attrib->attribute);
						  }//end error detection use
					   }
					   
				   }
				   //clear memory
				   if(sizeTracker > 0)
				   {
					   byte * byteArray = (byte*)lpMessage;
					   delete [] byteArray;
					   byteArray = 0;
				   }
				   else
					   classObj->m_aiServer->m_eventLog->AddEvent("****Error: Zero sized packet****:");
				}//end not found delete memory
				
				classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->AddToTotalBandwidthIncomming_safe((unsigned long)sizeTracker);//m_totalBandwidthIncoming += ((long)sizeTracker);
			}//end if theres a message handle it
		    if(currentTickCount-classObj->m_gCycleUpdateStamp > classObj->m_gCycleUpdateFrequency)
			{
			   classObj->m_aiServer->m_clientModule->m_initObject->m_clientNet->SendAI_GuerenteedCycleMsgToServer();
			   classObj->m_aiServer->m_clientModule->m_initObject->m_clientNet->PendingCommandsCallback();
	           classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_textRenderList->Flush();
			   classObj->SendPendingCfgRequestsCP();
			   classObj->m_gCycleUpdateStamp = currentTickCount;
			}
			Sleep(5);
		}
		EnterCriticalSection( &classObj->m_aiServer->m_interactSection);
		classObj->m_threadCount--;
		LeaveCriticalSection( &classObj->m_aiServer->m_interactSection);
		CoUninitialize();
return 1;
}
//-----------------------------------------------------------//
DWORD WINAPI NetworkThreadLoop(LPVOID lpParameter)
{	   
	    CWorldManager * classObj = (CWorldManager *)lpParameter; 
		if(FAILED(CoInitialize(NULL)))
		{
           return 0;
		}
		//spawn log thread
        
        while(1)
		{
            EnterCriticalSection( &classObj->m_aiServerBaseRef->m_interactSection);
            if(classObj->m_aiServerBaseRef->m_systemShutDown==TRUE || classObj->m_shutDownChannel == TRUE)
			{
               LeaveCriticalSection( &classObj->m_aiServerBaseRef->m_interactSection);
			   break;
			}
			LeaveCriticalSection( &classObj->m_aiServerBaseRef->m_interactSection);
			
			classObj->RunWorld();
			Sleep(50);
		}
        // If this channel was shut down dynamically (not whole system shutdown),
        // delete the world manager here. On system shutdown, CWorldManagerDatabase
        // cleans up the worlds after threads exit.
        if(classObj->m_shutDownChannel && !classObj->m_aiServerBaseRef->m_systemShutDown)
        {
            delete classObj;
            classObj = 0;
        }

		CoUninitialize();
return 1;
}
//-----------------------------logging and status therad----------------------------------
DWORD WINAPI NetworkLoggingLoop(LPVOID lpParameter)
{	   
	    CCentralProcessor * classObj = (CCentralProcessor *)lpParameter; 
		if(FAILED(CoInitialize(NULL)))
		{
           return 0;
		}

		CServerEventList * eventLogRef = classObj->m_aiServer->m_eventLog;
        classObj->m_aiServer->m_adoConnection->ExecQuery("delete from tblAiServerLog");
		//idle or process all messages with callback variables
        while(1)
		{   
			DWORD sectionStamp = GetTickCount();
			//classObj->m_aiServer->m_adoConnection->Reconnect(sectionStamp);
			EnterCriticalSection( &classObj->m_aiServer->m_interactSection);
            if(classObj->m_aiServer->m_systemShutDown)
			{
               LeaveCriticalSection( &classObj->m_aiServer->m_interactSection);
			   break;
			}
			LeaveCriticalSection( &classObj->m_aiServer->m_interactSection);

			
			
			classObj->m_aiServer->m_aiServerStatus.m_botsOnline						= 0;//
	        classObj->m_aiServer->m_aiServerStatus.m_worldCount = classObj->m_aiWorldsList->GetCount_safe();
			classObj->m_aiWorldsList->Lock();
			for(unsigned int cLoop=0;cLoop<classObj->m_aiWorldsList->m_cellCount;cLoop++)
			{
				 for(CAxLinkObj * pos = classObj->m_aiWorldsList->m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
				 {
					 CWorldManager * wldPtr = (CWorldManager *) classObj->m_aiWorldsList->m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
					 if(wldPtr)
					 {
						 EnterCriticalSection( &wldPtr->m_statusSec);
						 classObj->m_aiServer->m_aiServerStatus.m_botsOnline += wldPtr->m_botCount;
						 LeaveCriticalSection( &wldPtr->m_statusSec);
					 }
				 }
			}
			classObj->m_aiWorldsList->Unlock();

			EnterCriticalSection( &classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_statusLock);
					
			if(classObj->m_aiServer->m_aiServerStatus.m_botsOnline > classObj->m_aiServer->m_aiServerStatus.m_botsSpikeOnline)
			   classObj->m_aiServer->m_aiServerStatus.m_botsSpikeOnline = classObj->m_aiServer->m_aiServerStatus.m_botsOnline;
			
	        classObj->m_aiServer->m_aiServerStatus.m_msgsOutPerSecond				= classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_packetsOutPerPass;
	        classObj->m_aiServer->m_aiServerStatus.m_msgsInPerSecond				= classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_packetsRecievedPerPass;
	        classObj->m_aiServer->m_aiServerStatus.m_bandwidthOutgoingPerSecond		= classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_totalBandwidthOutGoing;
			classObj->m_aiServer->m_aiServerStatus.m_bandwidthIncommingPerSecond	= classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->GetTotalBandwidthIncomming_safe();
			classObj->m_aiServer->m_aiServerStatus.m_totalBandwidthIn				+= classObj->m_aiServer->m_aiServerStatus.m_bandwidthIncommingPerSecond;
	        classObj->m_aiServer->m_aiServerStatus.m_totalBandwidthOut				+= classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_totalBandwidthOutGoing;
	        classObj->m_aiServer->m_aiServerStatus.m_totalBandwidthTotal			+= classObj->m_aiServer->m_aiServerStatus.m_bandwidthIncommingPerSecond + classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_totalBandwidthOutGoing;
			

			classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->ZeroTotalBandwidthIncomming_safe();
		    classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_totalBandwidthOutGoing	=0;
		    classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_packetsOutPerPass			=0;
            classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_packetsRecievedPerPass	=0;

			LeaveCriticalSection( &classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_statusLock);
			
			for(int cowloop=0;cowloop<50;cowloop++)
			{
				CString temp = eventLogRef->GetOldestEntryMarkIt();
				temp.Replace("'","-");
			    if(temp.GetLength() > 0)
		           classObj->m_aiServer->m_adoConnection->ExecQuery("Insert into tblAiServerLog(logMsg) values('"+temp+"')");//write log to database
				else
				   break;
				Sleep(50);
			}
            CString dateUpdater = "lastUpdated = GETDATE(),";
			long divPsec = 5;
			classObj->m_aiServer->m_adoConnection->ExecQuery(
				"Update tblAiServerStatus set currentbots = '" + 
				 classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_cabLib.ConvertLongToString(classObj->m_aiServer->m_aiServerStatus.m_botsOnline)+"'," +
                "currentBotPeak = '" + 
				classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_cabLib.ConvertLongToString(classObj->m_aiServer->m_aiServerStatus.m_botsSpikeOnline)+"'," +
				"packetsoutpersecond = '" + 
				classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_cabLib.ConvertLongToString(classObj->m_aiServer->m_aiServerStatus.m_msgsOutPerSecond/divPsec)+"'," +
				"packetsinpersecond = '" + 
				classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_cabLib.ConvertLongToString(classObj->m_aiServer->m_aiServerStatus.m_msgsInPerSecond/divPsec)+"'," +
				"bandwidthOutgoingPerSecond = '" + 
				classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_cabLib.ConvertLongToString(classObj->m_aiServer->m_aiServerStatus.m_bandwidthOutgoingPerSecond/divPsec)+"'," +
				"totalbandwidthout = '" + 
				classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_cabLib.ConvertLongToString(classObj->m_aiServer->m_aiServerStatus.m_totalBandwidthOut/1000000)+"'," +
				"totalbandwidthin = '" + 
				classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_cabLib.ConvertLongToString(classObj->m_aiServer->m_aiServerStatus.m_totalBandwidthIn/1000000)+"'," +
				"bandwidthIncommingPerSecond = '" + 
				classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_cabLib.ConvertLongToString(classObj->m_aiServer->m_aiServerStatus.m_bandwidthIncommingPerSecond/divPsec)+"'," +
				"WorldMngCount = '" + 
				classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_cabLib.ConvertLongToString(classObj->m_aiServer->m_aiServerStatus.m_worldCount)+"'," +
				dateUpdater + 
				"totalbandwidthuse = '" + 
				classObj->m_aiServer->m_clientModule->m_initObject->m_gStatus->m_cabLib.ConvertLongToString(classObj->m_aiServer->m_aiServerStatus.m_totalBandwidthTotal/1000000)+"';");

			DWORD durationToWait = 5000;//1000-(GetTickCount()-sectionStamp);
			Sleep(durationToWait);
		}

		EnterCriticalSection( &classObj->m_aiServer->m_interactSection);
		classObj->m_threadCount--;
		LeaveCriticalSection( &classObj->m_aiServer->m_interactSection);
		CoUninitialize();
  //AfxEndThread(0,TRUE);
return 1;
}

//-----------------------------------------------------------//
// The AI server is an ENet client of the main game server. When that link is
// lost (game server shut down, crashed, or timed out) the multiplayer object
// sets connection status 2. The host window polls this and restarts the AI
// server so it reconnects instead of retrying spawns forever.
//-----------------------------------------------------------//
BOOL CCentralProcessor::IsConnectionLost()
{
	if(!m_connected || !m_aiServer || !m_aiServer->m_clientModule ||
	   !m_aiServer->m_clientModule->m_initObject ||
	   !m_aiServer->m_clientModule->m_initObject->m_gStatus ||
	   !m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject)
		return FALSE;
	return m_aiServer->m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->GetConnectionStatus() == 2;
}
