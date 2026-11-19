#include "StdAfx.h"
#include "sharednet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CSharedNet::CSharedNet(void) :
	SQUAD_MESSAGE_BURST_SIZE(6)
{
	m_lastTimeoutCall				= GetTickCount();
	m_worldRuleTraceStamp			= GetTickCount();
	m_realtimeLogger.m_logFileName  = "realtimeWriteLog.txt";
	m_realtimeLogger.WriteDebugFile("initialized",1,1);
	m_timeoutTraversal              = 0;
	m_primaryProcessingThreadCount  = 1;
	m_teamGlidNotifier				= 0;//should come from server configuration
	m_groupGlidNotifier				= 0;
	m_respawnHealthPercentage		= 100.0f;
	m_worldRuleTrace				= 0;
	m_shutdownAtTick				= 0;
	m_shutdownLastWarn				= -1;
	m_shutdownNow					= FALSE;
	m_raidActiveIdx					= -1;
	m_raidEndTick					= 0;
	m_raidPendingIdx				= -1;
	m_raidPendingStart				= 0;
	m_raidNextRoll					= GetTickCount() + 300000;
	m_raidCmdStart					= 0;
	m_raidCmdStop					= FALSE;
	m_collisionDatabase             = 0;
	m_regenerationTurnSpeed			= 10000;
	m_defaultHealthRegenAmount		= 2;
	m_maxCharacterCreate			= 5;
	m_lootMode						= 0;//default simple mode 1=damage based looting
	m_aggressorMaxTrack             = 10;//max store of separate agressor damage amounts
	m_maxDotTrack					= 30;//max amount of DoTs to track
	m_damageMode					= 0;//What damage algorithm to use
	m_dbIgnMode                     = 0;//non db default
	m_globalGroupCounter			= 1;
	m_arenaFactionBonus				= -1;
	m_pvpFactionBonus				= -1;
	m_clanFactionBonus				= -1;
	m_arenaItems					= NULL;
	m_arenaItemCount				= 0;
	m_eventLog                      = new CServerEventList();  
	m_characterPendingLoader        = new CCharacterPendingLoader();
	m_threadedDataProcessor         = new CCDataHandlerDB();
	m_netComObject					= new CMultiplayerObj();
    m_netComObject->m_eventLog		= m_eventLog;
	m_serverItemGenerator			= new CServerItemGenObjList();
	m_aiRaidDatabase				= new CAIRaidObjList();
	//m_safeZoneDB					= new CSafeZoneObjList();
	m_entObjList                    = new CMovementObjList();
	m_aiList                        = new CAIObjList();
	m_worldChannelRuleDB            = new CWorldChannelRuleList();
	m_skillDatabase                 = new CSkillObjectList();
	m_dotDB							= new CDotClassList();
	m_elementDB                     = new CElementObjList();
	m_commerceDB                    = new CCommerceObjList();
	m_portalDatabase                = new CPortalSysList();
	m_housingDB                     = new CHousingObjList();
	m_explosionDBList               = new CExplosionObjList();
	m_gmPageListRuntime             = new CGmPageList();
	m_battleSchedulerSys			= new CBattleSchedulerList();
	m_missileDB                     = new CMissileObjList();
	m_bankZoneList                  = new CBankObjList();
	m_actionZoneDB                  = new CActionZoneDB();
	m_itemManager					= new CItemManager();
	m_poiSystem						= new CAxServerPOISystem();
	m_currencySystem				= new CServerCurrencySystem();
	m_dbConnection				    = new CDBConnectionSystem(m_netComObject->m_sqlServerIP,
															  m_netComObject->m_sqlServerCatalog,
															  m_netComObject->m_sqlServerUsername,
															  m_netComObject->m_sqlServerPassword,
															  m_eventLog);
    if(m_dbConnection->m_connectionEstablished == FALSE)
	{
       m_realtimeLogger.WriteDebugFile("this Contruction connection could not be established,",0,1);
       m_realtimeLogger.WriteDebugFile(m_netComObject->m_sqlServerIP.GetBuffer(),0,1);
	   m_realtimeLogger.WriteDebugFile(m_netComObject->m_sqlServerCatalog.GetBuffer(),0,1);
	   m_realtimeLogger.WriteDebugFile(m_netComObject->m_sqlServerUsername.GetBuffer(),0,1);
	   m_realtimeLogger.WriteDebugFile(m_netComObject->m_sqlServerPassword.GetBuffer(),0,1);
       return;
	}

	m_groupDataBlock		 = new SQUAD_MESSAGE[SQUAD_MESSAGE_BURST_SIZE];



	m_serverScripts                 = NULL;
	m_serverScripts                 = new CServerScriptSystem();
	int scriptCount = m_serverScripts->AquireFromDatabase(m_dbConnection);
	m_eventLog->AddEvent("Scripts Loaded:",(DWORD)scriptCount);
	//aquire serverID
	if(!m_baseAccountDBConnection.AquireFromCfgFile("baseAccount.cfg"))
	{
		m_eventLog->LogByPriority("BaseAccount.cfg not found",2);
		m_realtimeLogger.WriteDebugFile("BaseAccount.cfg not found",0,1);
	}
	if(!m_authenticationDBConnection.AquireFromCfgFile("authAccount.cfg"))
	{
		m_eventLog->LogByPriority("authAccount.cfg not found",2);
		m_realtimeLogger.WriteDebugFile("authAccount.cfg not found",0,1);
	}
	m_currencySystem->InitFromSQL(m_eventLog);

    m_longMultiplier = 1000;
	GL_frequency.QuadPart = (__int64)0;
    if(QueryPerformanceFrequency(&GL_frequency)==0)
       GL_frequency.QuadPart = (__int64)0;//Failed
	if(timeBeginPeriod(1) != TIMERR_NOCANDO)
       GL_frequency.QuadPart = 0;//REMOVE
	
	
}



CSharedNet::~CSharedNet(void)
{
	if(m_itemManager)
	{
       delete m_itemManager;
	   m_itemManager = 0;
	}

	if(m_currencySystem)
	{
	   delete m_currencySystem;
	   m_currencySystem = 0;
	}

	if(m_bankZoneList)
	{
       m_bankZoneList->SafeDelete();
	   delete m_bankZoneList;
	   m_bankZoneList = 0;
	}
	if(m_dbConnection)
	{
       delete m_dbConnection;
	   m_dbConnection = 0;
	}
	if(m_missileDB)
	{
       m_missileDB->SafeDelete();
	   delete m_missileDB;
	   m_missileDB = 0;
	}
	if(m_battleSchedulerSys)
	{
	   m_battleSchedulerSys->SafeDelete();
       delete m_battleSchedulerSys;
	   m_battleSchedulerSys = 0;
	}

    if(m_netComObject)
	{
	   m_netComObject->SafeDelete();
	   delete m_netComObject;
	   m_netComObject = 0;
	}

	if(m_serverItemGenerator)
	{
	   delete m_serverItemGenerator;
	   m_serverItemGenerator = 0;
	}

	if(m_aiRaidDatabase)
	{
       m_aiRaidDatabase->SafeDelete();
	   delete m_aiRaidDatabase;
	   m_aiRaidDatabase = 0;
	}
	/*if(m_safeZoneDB)
	{ 
       m_safeZoneDB->SafeDelete();
	   delete m_safeZoneDB;
	   m_safeZoneDB = 0;
	}*/
	if(m_entObjList)
	{
       m_entObjList->SafeDelete();
	   delete m_entObjList;
	   m_entObjList = 0;
	}
	if(m_aiList)
	{
       m_aiList->SafeDelete();
	   delete m_aiList;
	   m_aiList = 0;
	}
	if(m_worldChannelRuleDB)
	{
	   delete m_worldChannelRuleDB;
	   m_worldChannelRuleDB = 0;
	}
	if(m_dotDB)
	{
		m_dotDB->SafeDelete();
		delete m_dotDB;
		m_dotDB = 0;
	}
	if(m_skillDatabase)
	{
       m_skillDatabase->SafeDelete();
	   delete m_skillDatabase;
	   m_skillDatabase = 0;
	}
	if(m_elementDB)
	{
       m_elementDB->SafeDelete();
	   delete m_elementDB;
	   m_elementDB = 0;
	}
	if(m_commerceDB)
	{
       m_commerceDB->SafeDelete();
	   delete m_commerceDB;
	   m_commerceDB = 0;
	}
	if(m_portalDatabase)
	{  
       m_portalDatabase->SafeDelete();
	   delete m_portalDatabase;
	   m_portalDatabase = 0;
	}
	if(m_housingDB)
	{
	   m_housingDB->SafeDelete();
	   delete m_housingDB;
	   m_housingDB=0;
	}
	if(m_explosionDBList)
	{
       m_explosionDBList->SafeDelete();
	   delete m_explosionDBList;
	   m_explosionDBList = 0;
	}
	if(m_gmPageListRuntime)
	{
       m_gmPageListRuntime->SafeDelete();
	   delete m_gmPageListRuntime;
	   m_gmPageListRuntime = 0;
	}
	if(m_eventLog)
	{
	   m_eventLog->OutPutTextFile("events.log");
       m_eventLog->SafeDelete();
	   delete m_eventLog;
	   m_eventLog = 0;
	}
    if(m_characterPendingLoader)
	{
       delete m_characterPendingLoader;
	   m_characterPendingLoader = 0;
	}
    if(m_threadedDataProcessor)
	{
       delete m_threadedDataProcessor;
	   m_threadedDataProcessor = 0;
	}
	if(m_serverScripts)
	{
       delete m_serverScripts;
	   m_serverScripts = 0;
	}
	if(m_actionZoneDB)
	{
       delete m_actionZoneDB;
	   m_actionZoneDB = 0;
	}
	if(m_poiSystem)
	{
		delete m_poiSystem;
		m_poiSystem = 0;
	}
	if(m_arenaItems)
	{
		delete [] m_arenaItems;
		m_arenaItems = 0;
	}
}
//------------------------------------------------------------------------------------//
void CSharedNet::Unhide(CPlayerObject * plPtr)
{
     if(!plPtr)
		 return;

     plPtr->m_stasis = 0;//cancel hide
     SendGeneralMessageHandler(23,plPtr);
	 plPtr->m_skillUseTimeStamp = GetTickCount();//reset skill use timer
}
//------------------------------------------------------------------------------------//
DWORD CSharedNet::GetTimeStampDW()
{
	return timeGetTime();
}
//------------------------------------------------------------------------------------//
DWORD CSharedNet::GetElaspeTimeMSDW(DWORD & lastStampPoint)
{
	return timeGetTime()-lastStampPoint;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::SendGeneralMessageHandler(int msgType,CPlayerObject * playerObj)
{
    if(!playerObj)
		return FALSE;

    if(playerObj->m_aiControlled)
	   return FALSE;

	if(playerObj->m_playerEntityGeneralMgsOutbox->GetCount() > 10)
	   return FALSE;


	MSGHANDLER dataPacket;
	int messageSize = sizeof(MSGHANDLER);
    dataPacket.m_catagorie = 28;
	dataPacket.m_data      = msgType;

	playerObj->m_playerEntityGeneralMgsOutbox->AddPacket(messageSize,(BYTE*)&dataPacket,0,playerObj->m_currentWorldID);

	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::SendFireMsgToOutbox(D3DVECTOR lookat,
							         short firQueIndex,
									 long refrenceID,
							         CPlayerObject * playerObjectData) 
{

    if(playerObjectData->m_firemsgsoutbox->GetCount() > 20)
	   return FALSE;

    LPFIRE_STRUCT lpChatMessage = NULL;
	DWORD				messageSize;
	messageSize = sizeof(FIRE_STRUCT);
    lpChatMessage = (LPFIRE_STRUCT) new BYTE[messageSize];
	if (lpChatMessage == NULL)
	{
		return FALSE;
	}

	lpChatMessage->m_catagorie     = 3;
	lpChatMessage->lookatPosX      = (short)lookat.x;
	lpChatMessage->lookatPosY      = (short)lookat.y;
	lpChatMessage->lookatPosZ      = (short)lookat.z;
    lpChatMessage->idRefrence      = (short)refrenceID;
	lpChatMessage->fireQueIndex    = firQueIndex;

	playerObjectData->m_firemsgsoutbox->AddPacket(messageSize,(BYTE*)lpChatMessage,playerObjectData->m_currentID,playerObjectData->m_currentWorldID);

	if (lpChatMessage){
		delete lpChatMessage;
		lpChatMessage;
	}
	return TRUE;
}
//-----------------------------------------------------------//
void CSharedNet::DisconnectPlayerAndLog(CPlayerObject * playerObjectData,CString errorMsg)
{
     m_netComObject->m_eventLog->AddEvent(errorMsg,(DWORD)playerObjectData->m_currentID);
	 m_netComObject->Disconnect(playerObjectData->m_currentID);//disconnect for incomplience   
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::SendAttribMsg(DWORD flags,
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
	lpChatMessage->CRCcheck    = m_netComObject->GetAttribCRC(lpChatMessage);
    
	
    BOOL storeIt = TRUE;
	if(runtimePlOption)
	{

	}
	else
	   storeIt = FALSE;
    
	if(sendTo == DPID_SERVERPLAYER)
	{
	   if(m_netComObject->m_attributeOutBox)   // only exists on the client
	      m_netComObject->m_attributeOutBox->AddPacket(messageSize,(BYTE*)lpChatMessage,0,0);
	}
	else if(storeIt)
	{
	   if(!runtimePlOption->m_aiControlled)
	   {
	      if(runtimePlOption->m_attributeOutbox->GetCount() < 30)
             runtimePlOption->m_attributeOutbox->AddPacket(messageSize,(BYTE*)lpChatMessage,0,runtimePlOption->m_currentWorldID);
	   }
	   else
	   {
	      CAccountObject * accountPtr = m_netComObject->m_runtimeAccountDB->GetAccountByID(runtimePlOption->m_currentID);
          if(accountPtr)
		  {
             accountPtr->AddAttribMsg(messageSize,(BYTE*)lpChatMessage,0,runtimePlOption->m_currentWorldID);
		  }
	   }
	}
	else
       m_netComObject->SendData((BYTE *)lpChatMessage,
				                          messageSize,
				                          flags,
					                      sendTo,
										  m_netComObject->m_packetTimeout);
    

	if (lpChatMessage){
		delete lpChatMessage;
		lpChatMessage=0;
	}

	return TRUE;
}

//------------------------------------------------------------------------------------//
void CSharedNet::BroadcastEffect(int expRef,int newtorkAssignedID,CPlayerObject * hostPtr)
{//Broadcast
	  float constZero = 0.0f;
	  float pushPower = 0.0f;
	  
	  CAxList * channelList = m_netComObject->m_runtimePlayersOnServer->GetListByChannel(hostPtr->m_currentWorldID);
	  
	  
	  CExplosionObj * expDBPtr = m_explosionDBList->GetPtr(expRef);
      if(expDBPtr)
	  if(channelList)
		for(CAxLinkObj * firePos = channelList->GetHeadPosition_unsafe();firePos != NULL;)
		{//player loop
           CPlayerObject * pData = (CPlayerObject *)channelList->GetNext_unsafe(&firePos);
           if(!pData->m_inGame)
			  continue;

		   if(pData->m_currentWorldID != hostPtr->m_currentWorldID)
		      continue;
		   
		   if(pData->m_currentID == 0)
			  continue;
		   
		   if(pData->m_aiControlled)
			  continue;

           if(m_matLib.BoundingSphereCheckX(m_netComObject->m_serverCalculateRadius,constZero,
					                        pData->m_3dPosition.x,pData->m_3dPosition.y,pData->m_3dPosition.z,
										    hostPtr->m_Target3dPosition.x,hostPtr->m_Target3dPosition.y,hostPtr->m_Target3dPosition.z))
		   {//bounding check
              pushPower = expDBPtr->m_pushPower;
			  if(newtorkAssignedID == pData->m_networkAssignedID)
			     pushPower = 0.0f;
              
		      SendDetailedMsgToClient(  6,
										expRef,
										newtorkAssignedID,
										(int)(pushPower * 10.0f),
										pData);


		   }//end bounding check
		}//end player loop


		//broadcast to ai server channels
		DETAILEDMSG_DATA dataPacket;
	    int messageSize = sizeof(DETAILEDMSG_DATA);
        dataPacket.m_catagorie = 36;
	    dataPacket.m_data      = 6;
	    dataPacket.m_miscInt   = expRef;
	    dataPacket.m_miscInt2  = newtorkAssignedID;
	    dataPacket.m_miscInt3  = (short)(pushPower * 10.0f);
		
		CAxList * aiSvrList = m_netComObject->m_runtimeAccountDB->GetAllOnIndex(1,"aiserver");
		if(aiSvrList)
		   for(CAxLinkObj * posAIS = aiSvrList->GetHeadPosition_unsafe();posAIS != NULL;)
		   {
               CAccountObject * acPtr = (CAccountObject * )aiSvrList->GetNext_unsafe(&posAIS);
               
			   acPtr->AddDetailMsg(messageSize,(BYTE*)&dataPacket,0,hostPtr->m_currentWorldID);
		   }
}
//------------------------------------------------------------------------------------//
void CSharedNet::BroadcastNPCAnimState(CServerItemObj * svrPtr)
{//Broadcast
      float constZero = 0.0f;
	  int trace = 0;
	  short castTemp = 0;
	  int szShort = sizeof(short);
	  int messageSize = (sizeof(short)*2)+1;
	  BYTE * dataPacket = new BYTE[messageSize];
	  ZeroMemory(dataPacket,messageSize);
	  
	  CAxList * channelList = m_netComObject->m_runtimePlayersOnServer->GetListByChannel(svrPtr->m_world);
	  
	  if(channelList)
		for(CAxLinkObj* firePos = channelList->GetHeadPosition_unsafe();firePos != NULL;)
		{//player loop
           CPlayerObject * pData = (CPlayerObject *)channelList->GetNext_unsafe(&firePos);
           if(!pData->m_inGame)
			  continue;

		   if(pData->m_currentWorldID != svrPtr->m_world)
		      continue;
		   
		   if(pData->m_currentID == 0)
			  continue;
		   
		   if(pData->m_aiControlled)
			  continue;

           if(m_matLib.BoundingSphereCheckX(m_netComObject->m_serverCalculateRadius,constZero,
					                        pData->m_3dPosition.x,pData->m_3dPosition.y,pData->m_3dPosition.z,
										    svrPtr->m_position.x,svrPtr->m_position.y,svrPtr->m_position.z))
		   {//bounding check
		      trace = 0;
		      dataPacket[trace++] = 8;
			  castTemp = (short)svrPtr->m_networkAssignedID;
	          CopyMemory(&dataPacket[trace],&castTemp,szShort);
			  trace += szShort;
			  castTemp = (short)svrPtr->m_curAnim;
			  CopyMemory(&dataPacket[trace],&castTemp,szShort);
              if(pData->m_miscOutBox->GetCount() < 20)
                 pData->m_miscOutBox->AddPacket(messageSize,(BYTE*)dataPacket,0,pData->m_currentWorldID);
		   }//end bounding check
		}//end player loop
		delete [] dataPacket;
	    dataPacket = 0;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::SendDetailedMsgToClient(   int msgType,
									        int miscInt,
									        int miscInt2,
											int miscInt3,
											CPlayerObject * playerObj)
{
    if(!playerObj)
		return FALSE;


    if(playerObj->m_playerEntityDetailedMgsOutbox->GetCount() > 10)
	   return FALSE;

	DETAILEDMSG_DATA dataPacket;
	int messageSize = sizeof(DETAILEDMSG_DATA);
    dataPacket.m_catagorie = 36;
	dataPacket.m_data      = msgType;
	dataPacket.m_miscInt   = miscInt;
	dataPacket.m_miscInt2  = miscInt2;
	dataPacket.m_miscInt3  = miscInt3;

    /*m_netComObject->SendData((BYTE *)&dataPacket,
				                          messageSize,
				                          DPNSEND_GUARANTEED,
				                          idFromLoc,
										  m_netComObject->m_packetTimeout);*/

	//playerObj->m_playerEntityDetailedMgsOutbox->AddPacket(messageSize,(BYTE*)&dataPacket,0,playerObj->m_currentWorldID);

	if(!playerObj->m_aiControlled)
        playerObj->m_playerEntityDetailedMgsOutbox->AddPacket(messageSize,(BYTE*)&dataPacket,0,playerObj->m_currentWorldID);
	/*else if(playerObj->m_accountReference)
    {
          CAccountObject * accountPtr = (CAccountObject *)playerObj->m_accountReference;
          accountPtr->AddDetailMsg(messageSize,(BYTE*)&dataPacket,0,playerObj->m_currentWorldID);
    }*/


	return TRUE;
}
//------------------------------------------------------------------------------------//
void CSharedNet::BroadcastMessage(  short radius,
									CString message,
									D3DVECTOR position,
									int world,
									int referenceID)//0 radius = to all
{
   CAxList * channelList = m_netComObject->m_runtimePlayersOnServer->GetListByChannel(world);

   if(channelList)
   for(CAxLinkObj* posLoc = channelList->GetHeadPosition_unsafe();posLoc != NULL;)
   {
       CPlayerObject * lstObj = (CPlayerObject *)channelList->GetNext_unsafe(&posLoc);
	   if(lstObj->m_aiControlled)
	      continue;

	   if(lstObj->m_currentID == 0)
		  continue;

	   if(lstObj->m_energy < 1)
	      continue;

	   if(lstObj->m_deaf)
		  continue;

	   if(world == lstObj->m_currentWorldID || world == -1){//same world //different computer link
               if(lstObj->m_curEDBIndex >= 0/* && lstObj->m_networkAssignedID != referenceID*/){//id
				   if(radius == 0){
				      SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
						          lstObj->m_currentID,
						          message,
								  referenceID,
								  lstObj,
								  0);
				   } 
				   else if(m_cabLib.BoundingSphereCheck((float)radius,0.0f,
					                                           lstObj->m_3dPosition.x,lstObj->m_3dPosition.y,lstObj->m_3dPosition.z,
											                   position.x,position.y,position.z))
				                                   SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
						                                       lstObj->m_currentID,
						                                       message,
															   referenceID,
															   lstObj,
															   0);
			   }//end id
		   }//end same world
   }
}
//------------------------------------------------------------------------------------//
static BOOL IsQuestRaid(class CAIRaidObj * raidPtr);//defined with the raid code below
// Sends a message to every player on the server, in every zone.
// (BroadcastMessage with world -1 only reached one hash cell of the channel index.)
void CSharedNet::BroadcastToAll(CString message)
{
   if(!m_netComObject || !m_netComObject->m_runtimePlayersOnServer)
      return;

   m_netComObject->m_runtimePlayersOnServer->Lock();//re-entrant - safe if the caller already holds it
   unsigned int cellCount = m_netComObject->m_runtimePlayersOnServer->m_cellCount;
   for(unsigned int cell = 0; cell < cellCount; cell++)
   {
      CAxList * cellList = m_netComObject->m_runtimePlayersOnServer->GetAllOnIndex(cell,"net_id");
      if(!cellList)
         continue;
      for(CAxLinkObj* posLoc = cellList->GetHeadPosition_unsafe();posLoc != NULL;)
      {
         CPlayerObject * lstObj = (CPlayerObject *)cellList->GetNext_unsafe(&posLoc);
         if(!lstObj || lstObj->m_aiControlled || lstObj->m_currentID == 0 || lstObj->m_curEDBIndex < 0)
            continue;
         SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
                     lstObj->m_currentID,
                     message,
                     0,
                     lstObj,
                     0);
      }
   }
   m_netComObject->m_runtimePlayersOnServer->Unlock();
}
//------------------------------------------------------------------------------------//
// "!" commands typed in normal chat by an admin/GM. Returns TRUE if the text was a command
// (it is then NOT shown as chat).
BOOL CSharedNet::HandleGMChatCommand(CPlayerObject * playerObj, CString text)
{
   if(!playerObj)
      return FALSE;
   text.Trim();
   if(text.GetLength() < 2 || text.GetAt(0) != '!')
      return FALSE;

   CString cmd = text.Mid(1), args;
   int sp = cmd.Find(' ');
   if(sp >= 0)
   {
      args = cmd.Mid(sp + 1);
      args.Trim();
      cmd = cmd.Left(sp);
   }
   cmd.MakeLower();

   CString reply;
   if(cmd == "broadcast")
   {
      if(args.IsEmpty())
         reply = "Usage: !broadcast <message>";
      else
      {
         BroadcastToAll("[SERVER] " + args);
         if(m_eventLog)
            m_eventLog->LogByPriority("GM broadcast by " + playerObj->m_charName + ": " + args, 2);
      }
   }
   else if(cmd == "shutdown")
   {
      if(args.CompareNoCase("cancel") == 0)
      {
         if(m_shutdownAtTick && !m_shutdownNow)
         {
            m_shutdownAtTick = 0;
            BroadcastToAll("[SERVER] The server shutdown has been cancelled.");
            if(m_eventLog)
               m_eventLog->LogByPriority("Shutdown cancelled by " + playerObj->m_charName, 2);
         }
         else
            reply = "No shutdown is pending.";
      }
      else
      {
         CString reason;
         CString num = args;
         int sp2 = args.Find(' ');
         if(sp2 >= 0)
         {
            num    = args.Left(sp2);
            reason = args.Mid(sp2 + 1);
            reason.Trim();
         }
         int minutes = atoi(num);
         if(minutes < 1 || minutes > 120)
            reply = "Usage: !shutdown <minutes 1-120> [reason]   or   !shutdown cancel";
         else
         {
            m_shutdownNow      = FALSE;
            m_shutdownLastWarn = minutes * 60;
            DWORD at = GetTickCount() + (DWORD)minutes * 60000;
            m_shutdownAtTick   = at ? at : 1;

            CString msg;
            msg.Format("[SERVER] The server will shut down in %d minute%s.", minutes, minutes == 1 ? "" : "s");
            if(!reason.IsEmpty())
               msg += " Reason: " + reason;
            BroadcastToAll(msg);
            if(m_eventLog)
               m_eventLog->LogByPriority("Shutdown in " + num + " min set by " + playerObj->m_charName, 2);
         }
      }
   }
   else if(cmd == "raid")
   {
      CString sub = args, rest;
      int sp3 = args.Find(' ');
      if(sp3 >= 0)
      {
         sub  = args.Left(sp3);
         rest = args.Mid(sp3 + 1);
         rest.Trim();
      }
      sub.MakeLower();
      if(sub == "list")
      {
         int idx = 0;
         for(POSITION pos = m_aiRaidDatabase ? m_aiRaidDatabase->GetHeadPosition() : NULL; pos != NULL; idx++)
         {
            CAIRaidObj * raidPtr = (CAIRaidObj *)m_aiRaidDatabase->GetNext(pos);
            if(!raidPtr)
               continue;
            CString line;
            line.Format("#%d %s - wait %dm, %d%%, %d spawns%s", idx, (LPCSTR)raidPtr->m_raidName,
                        (int)(raidPtr->m_raidManditoryWait / 60000), raidPtr->m_percentSpawn,
                        raidPtr->m_spawnPoints ? (int)raidPtr->m_spawnPoints->GetCount() : 0,
                        IsQuestRaid(raidPtr) ? " (quest - not automatic)" : "");
            SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW, playerObj->m_currentID, line,
                        playerObj->m_networkAssignedID, playerObj, 0);
         }
         if(idx == 0)
            reply = "No raids are loaded.";
      }
      else if(sub == "start")
      {
         int found = -1;
         if(!rest.IsEmpty() && rest.SpanIncluding("0123456789") == rest)
            found = GetRaidByIndex(atoi(rest)) ? atoi(rest) : -1;
         else if(!rest.IsEmpty())
         {
            CString want = rest;
            want.MakeLower();
            int idx = 0;
            for(POSITION pos = m_aiRaidDatabase ? m_aiRaidDatabase->GetHeadPosition() : NULL; pos != NULL; idx++)
            {
               CAIRaidObj * raidPtr = (CAIRaidObj *)m_aiRaidDatabase->GetNext(pos);
               CString have = raidPtr ? raidPtr->m_raidName : CString("");
               have.MakeLower();
               if(have.Find(want) >= 0)
               {
                  found = idx;
                  break;
               }
            }
         }
         if(found < 0)
            reply = "Raid not found. Use !raid list, then !raid start <#>";
         else
         {
            m_raidCmdStart = found + 1;
            reply = "Starting raid: " + GetRaidByIndex(found)->m_raidName;
            if(m_eventLog)
               m_eventLog->LogByPriority("GM raid start by " + playerObj->m_charName + ": " + GetRaidByIndex(found)->m_raidName, 2);
         }
      }
      else if(sub == "stop")
      {
         m_raidCmdStop = TRUE;
         reply = "Raid stop requested (raid mobs already out stay until their time runs out).";
      }
      else if(sub == "status")
      {
         CAIRaidObj * a = GetRaidByIndex(m_raidActiveIdx);
         CAIRaidObj * p = GetRaidByIndex(m_raidPendingIdx);
         if(a)
            reply.Format("Raid running: %s (%d min left)", (LPCSTR)a->m_raidName, (int)((long)(m_raidEndTick - GetTickCount()) / 60000));
         else if(p)
            reply = "Raid coming: " + p->m_raidName;
         else
            reply = "No raid running.";
      }
      else
         reply = "Usage: !raid list | !raid start <# or name> | !raid stop | !raid status";
   }
   else if(cmd == "help")
      reply = "GM commands: !broadcast <msg>, !shutdown <minutes> [reason], !shutdown cancel, !raid list/start/stop/status";
   else
      reply = "Unknown command. Type !help";

   if(!reply.IsEmpty())
      SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
                  playerObj->m_currentID,
                  reply,
                  playerObj->m_networkAssignedID,
                  playerObj,
                  0);
   return TRUE;
}
//------------------------------------------------------------------------------------//
// RAIDS - the raid list (EARevolution.AIR) always loaded but nothing ever started them.
// Timed raids roll every 5 minutes once their wait is over, only when players are in the
// raid's zone. Quest raids (names with "Q)") are left out. Everything here runs on the
// server window thread; GM commands only set m_raidCmdStart / m_raidCmdStop.
static BOOL IsQuestRaid(CAIRaidObj * raidPtr)
{
   return raidPtr->m_raidName.Find("Q)") >= 0;
}
CAIRaidObj * CSharedNet::GetRaidByIndex(int raidIndex)
{
   if(!m_aiRaidDatabase || raidIndex < 0)
      return NULL;
   POSITION pos = m_aiRaidDatabase->FindIndex(raidIndex);
   if(!pos)
      return NULL;
   return (CAIRaidObj *)m_aiRaidDatabase->GetAt(pos);
}
int CSharedNet::CountPlayersInWorld(int worldID)
{
   if(!m_netComObject || !m_netComObject->m_runtimePlayersOnServer)
      return 0;
   int count = 0;
   m_netComObject->m_runtimePlayersOnServer->Lock();
   unsigned int cellCount = m_netComObject->m_runtimePlayersOnServer->m_cellCount;
   for(unsigned int cell = 0; cell < cellCount; cell++)
   {
      CAxList * cellList = m_netComObject->m_runtimePlayersOnServer->GetAllOnIndex(cell,"net_id");
      if(!cellList)
         continue;
      for(CAxLinkObj* posLoc = cellList->GetHeadPosition_unsafe();posLoc != NULL;)
      {
         CPlayerObject * lstObj = (CPlayerObject *)cellList->GetNext_unsafe(&posLoc);
         if(!lstObj || lstObj->m_aiControlled || lstObj->m_currentID == 0 || lstObj->m_curEDBIndex < 0)
            continue;
         if(worldID < 0 || lstObj->m_currentWorldID == worldID)
            count++;
      }
   }
   m_netComObject->m_runtimePlayersOnServer->Unlock();
   return count;
}
BOOL CSharedNet::RaidHasPlayers(CAIRaidObj * raidPtr)
{
   if(!raidPtr || !raidPtr->m_spawnPoints)
      return FALSE;
   for(POSITION pos = raidPtr->m_spawnPoints->GetHeadPosition(); pos != NULL;)
   {
      CAISpawnPoint * sp = (CAISpawnPoint *)raidPtr->m_spawnPoints->GetNext(pos);
      if(sp && CountPlayersInWorld(sp->m_worldID) > 0)
         return TRUE;
   }
   return FALSE;
}
CString CSharedNet::GetWorldMapForRaid(int worldID)
{
   CString mapName;
   if(m_netComObject && m_netComObject->m_currentWorldsUsed)
   {
      m_netComObject->m_currentWorldsUsed->Lock();
      CWorldAvailableObj * wld = m_netComObject->m_currentWorldsUsed->GetWorldByChannel(worldID);
      if(wld)
         mapName = wld->m_worldName;
      m_netComObject->m_currentWorldsUsed->Unlock();
   }
   if(mapName.IsEmpty() && m_netComObject && m_netComObject->m_runtimePlayersOnServer)
   {//fall back to the map of a player standing in that zone
      m_netComObject->m_runtimePlayersOnServer->Lock();
      unsigned int cellCount = m_netComObject->m_runtimePlayersOnServer->m_cellCount;
      for(unsigned int cell = 0; cell < cellCount && mapName.IsEmpty(); cell++)
      {
         CAxList * cellList = m_netComObject->m_runtimePlayersOnServer->GetAllOnIndex(cell,"net_id");
         if(!cellList)
            continue;
         for(CAxLinkObj* posLoc = cellList->GetHeadPosition_unsafe();posLoc != NULL;)
         {
            CPlayerObject * lstObj = (CPlayerObject *)cellList->GetNext_unsafe(&posLoc);
            if(lstObj && !lstObj->m_aiControlled && lstObj->m_currentWorldID == worldID && lstObj->m_currentWorldMap.GetLength() > 4)
            {
               mapName = lstObj->m_currentWorldMap;
               break;
            }
         }
      }
      m_netComObject->m_runtimePlayersOnServer->Unlock();
   }
   return mapName;
}
BOOL CSharedNet::StartRaid(int raidIndex)
{
   CAIRaidObj * raidPtr = GetRaidByIndex(raidIndex);
   if(!raidPtr || !raidPtr->m_spawnPoints)
      return FALSE;

   long life = raidPtr->m_raidDuration;
   if(life < 60000)
      life = 60000;
   if(life > 2700000)//AI server caps a dynamic spawn at 45 minutes
      life = 2700000;

   int sent = 0;
   for(POSITION pos = raidPtr->m_spawnPoints->GetHeadPosition(); pos != NULL;)
   {
      CAISpawnPoint * sp = (CAISpawnPoint *)raidPtr->m_spawnPoints->GetNext(pos);
      if(!sp)
         continue;
      int aiCfg = sp->m_spawnCfg;
      if(sp->m_cfgList && sp->m_cfgList->GetCount() > 0)
      {//random pick from the point's list
         POSITION cfgPos = sp->m_cfgList->FindIndex(rand() % sp->m_cfgList->GetCount());
         CAICfgOption * opt = cfgPos ? (CAICfgOption *)sp->m_cfgList->GetAt(cfgPos) : NULL;
         if(opt)
            aiCfg = opt->m_aiCfg;
      }
      CString mapName = GetWorldMapForRaid(sp->m_worldID);
      if(mapName.IsEmpty())
      {
         if(m_eventLog)
            m_eventLog->LogByPriority("Raid: no map name for world " + raidPtr->m_raidName + " world:",(DWORD)sp->m_worldID,2);
         continue;
      }
      if(SpawnDynamicAIToAvailAiServer(aiCfg, sp->m_worldID, sp->m_position, 9000 + raidIndex, life, -1, 0, 0, 0, 360, mapName))
         sent++;
   }

   raidPtr->m_lastRaidTimeStamp = (long)GetTickCount();
   m_raidActiveIdx = raidIndex;
   m_raidEndTick   = GetTickCount() + (DWORD)life;

   CString msg = raidPtr->m_raidAlertMessage;
   msg.Trim();
   if(msg.IsEmpty())
      msg = raidPtr->m_raidName + " has begun!";
   BroadcastToAll("[RAID] " + msg);
   if(m_eventLog)
      m_eventLog->LogByPriority("Raid started: " + raidPtr->m_raidName + " spawns sent:",(DWORD)sent,2);
   return TRUE;
}
void CSharedNet::EndRaid(BOOL calledOff)
{
   CAIRaidObj * raidPtr = GetRaidByIndex(m_raidActiveIdx);
   CString name = raidPtr ? raidPtr->m_raidName : CString("The raid");
   if(raidPtr)
      raidPtr->m_lastRaidTimeStamp = (long)GetTickCount();//its wait starts again from the end
   m_raidActiveIdx = -1;
   BroadcastToAll(calledOff ? "[RAID] " + name + " has been called off." : "[RAID] " + name + " is over!");
   if(m_eventLog)
      m_eventLog->LogByPriority("Raid ended: " + name, 2);
}
void CSharedNet::RaidTick()
{
   if(!m_aiRaidDatabase)
      return;
   DWORD now = GetTickCount();

   //GM requests
   int startReq = m_raidCmdStart;
   if(startReq > 0)
   {
      m_raidCmdStart = 0;
      if(m_raidActiveIdx >= 0)
         EndRaid(TRUE);
      m_raidPendingIdx = -1;
      StartRaid(startReq - 1);
   }
   if(m_raidCmdStop)
   {
      m_raidCmdStop = FALSE;
      if(m_raidActiveIdx >= 0)
         EndRaid(TRUE);
      else if(m_raidPendingIdx >= 0)
      {
         m_raidPendingIdx = -1;
         BroadcastToAll("[RAID] The incoming raid has been called off.");
      }
   }

   //raid time up
   if(m_raidActiveIdx >= 0 && (long)(now - m_raidEndTick) >= 0)
      EndRaid(FALSE);

   //warned raid starts
   if(m_raidPendingIdx >= 0 && (long)(now - m_raidPendingStart) >= 0)
   {
      int idx = m_raidPendingIdx;
      m_raidPendingIdx = -1;
      StartRaid(idx);
   }

   //roll for a new raid every 5 minutes - one raid at a time
   if(m_raidActiveIdx < 0 && m_raidPendingIdx < 0 && (long)(now - m_raidNextRoll) >= 0)
   {
      m_raidNextRoll = now + 300000;
      int count = (int)m_aiRaidDatabase->GetCount();
      if(count > 0 && CountPlayersInWorld(-1) > 0)
      {
         int first = rand() % count;
         for(int n = 0; n < count; n++)
         {
            int idx = (first + n) % count;
            CAIRaidObj * raidPtr = GetRaidByIndex(idx);
            if(!raidPtr || IsQuestRaid(raidPtr) || !raidPtr->m_spawnPoints || raidPtr->m_spawnPoints->GetCount() < 1)
               continue;
            if((long)(now - (DWORD)raidPtr->m_lastRaidTimeStamp) < raidPtr->m_raidManditoryWait)
               continue;
            if(!RaidHasPlayers(raidPtr))
               continue;
            if((rand() % 100) >= raidPtr->m_percentSpawn)
               continue;
            m_raidPendingIdx   = idx;
            m_raidPendingStart = now + 300000;
            BroadcastToAll("[RAID] Warning! " + raidPtr->m_raidName + " begins in 5 minutes!");
            if(m_eventLog)
               m_eventLog->LogByPriority("Raid warning sent: " + raidPtr->m_raidName, 2);
            break;
         }
      }
   }
}
//------------------------------------------------------------------------------------//
// Shutdown countdown - the server window calls this once a second.
void CSharedNet::ShutdownTick()
{
   DWORD at = m_shutdownAtTick;
   if(at == 0 || m_shutdownNow)
      return;

   long left = (long)(at - GetTickCount());
   if(left <= 0)
   {
      BroadcastToAll("[SERVER] The server is shutting down now.");
      m_shutdownNow = TRUE;
      return;
   }

   int secs = (int)((left + 999) / 1000);
   static const int marks[] = {1800,900,600,300,120,60,30,10,5};
   int crossed = -1;
   for(int i = 0; i < (int)(sizeof(marks)/sizeof(marks[0])); i++)
      if(secs <= marks[i] && m_shutdownLastWarn > marks[i])
         crossed = marks[i];//keeps the smallest mark passed
   if(crossed < 0)
      return;

   m_shutdownLastWarn = crossed;
   CString msg;
   if(crossed >= 60)
      msg.Format("[SERVER] The server will shut down in %d minute%s. Please get to a safe spot and log out.", crossed / 60, crossed >= 120 ? "s" : "");
   else
      msg.Format("[SERVER] The server will shut down in %d seconds!", crossed);
   BroadcastToAll(msg);
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::DirectMessage(  short radius,
							     CString & message,
								 D3DVECTOR position,
								 int world,
								 int referenceID,
								 CString & targetPerson)//0 radius = to all
{
   
   CAxList * channelList = m_netComObject->m_runtimePlayersOnServer->GetListByChannel(world);
	
	
   BOOL foundOne = FALSE;
   if(channelList)
   for(CAxLinkObj* posLoc = channelList->GetHeadPosition_unsafe();posLoc != NULL;)
   {
       CPlayerObject * lstObj = (CPlayerObject *)channelList->GetNext_unsafe(&posLoc);
	   if(lstObj->m_aiControlled)
	      continue;

       if(lstObj->m_deaf)
		  continue;

	   if(lstObj->m_currentID == 0)
		  continue;

	   if(lstObj->m_energy < 1)
		  continue;

	   if(lstObj->m_charName.CompareNoCase(targetPerson)!=0)
	      continue;

	   if(world == lstObj->m_currentWorldID || world == -1){//same world //different computer link
               if(lstObj->m_curEDBIndex >= 0){//id
				   
				   if(radius == 0)
				   {
				        SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
									lstObj->m_currentID,
									message,
									referenceID,
									lstObj,
									0);
						foundOne = TRUE;
				   } 
				   else if(m_cabLib.BoundingSphereCheck((float)radius,0.0f,
					       lstObj->m_3dPosition.x,lstObj->m_3dPosition.y,lstObj->m_3dPosition.z,
						   position.x,position.y,position.z))     
				   {
						   SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
						                                       lstObj->m_currentID,
						                                       message,
															   referenceID,
															   lstObj,
															   0);
						   foundOne = TRUE;
				   }
			   }//end id
		   }//end same world
   }
   return foundOne;
}
//--------------send message----------------------------------------------------------//
BOOL CSharedNet::SendTextMsg(   DWORD flags,
								DPNID sendTo,//can be NULL if client
								CString message,
								int referenceID,
								CPlayerObject * playerPtr,
								int type)
{
    

	DWORD				messageSize;
	DWORD               textSize = message.GetLength();
	if(textSize < 1 || textSize > 250)
	   return FALSE;

	messageSize = sizeof(TXT_MSG) + textSize;
    BYTE * datamessage = new BYTE[messageSize];
	
	LPTXT_MSG castEntry = (LPTXT_MSG)datamessage;
	castEntry->m_catagorie = type;
    castEntry->referenceID = referenceID;
	CopyMemory(&datamessage[4],message.GetBuffer(),textSize);

	if(sendTo == DPID_SERVERPLAYER)
	{
		if(m_netComObject->m_txtOutBox && m_netComObject->m_txtOutBox->GetCount() < 5)
		   m_netComObject->m_txtOutBox->AddPacket(messageSize,(BYTE*)datamessage,0,0);
	}
	else if(playerPtr){
	   if(playerPtr->m_txtMsgOutbox)//check here because if you do it in the if statememnt above it will force a raw send if outbox is null from the server to client and this is to be avoided
	      if(playerPtr->m_txtMsgOutbox->GetCount() < 20)
             playerPtr->m_txtMsgOutbox->AddPacket(messageSize,(BYTE*)datamessage,0,playerPtr->m_currentWorldID);
	}
	else
	{//send raw
       m_netComObject->SendData((BYTE*)datamessage,
				                messageSize,
				                flags,
					            sendTo,
								m_netComObject->m_packetTimeout);

	}//end send raw

	if (datamessage)
		delete [] datamessage;
	    datamessage=0;


	return TRUE;
}
//--------------send message----------------------------------------------------------//
BOOL CSharedNet::SendTextMsg(   DPNID sendTo,//can be NULL if client
								CString message,
								CPlayerObject * playerPtr,
								int type)
{
    

	DWORD				messageSize;
	DWORD               textSize = message.GetLength();
	if(textSize < 1 || textSize > 250)
	   return FALSE;

	messageSize = sizeof(TXT_MSG) + textSize;
    BYTE * datamessage = new BYTE[messageSize];
	
	LPTXT_MSG castEntry = (LPTXT_MSG)datamessage;
	castEntry->m_catagorie = type;
    castEntry->referenceID = 0;
	CopyMemory(&datamessage[4],message.GetBuffer(),textSize);

	if(sendTo == DPID_SERVERPLAYER)
	{
		if(m_netComObject->m_txtOutBox && m_netComObject->m_txtOutBox->GetCount() < 5)
		   m_netComObject->m_txtOutBox->AddPacket(messageSize,(BYTE*)datamessage,0,0);
	}
	else if(playerPtr){
	   if(playerPtr->m_txtMsgOutbox)//check here because if you do it in the if statememnt above it will force a raw send if outbox is null from the server to client and this is to be avoided
	      if(playerPtr->m_txtMsgOutbox->GetCount() < 20)
             playerPtr->m_txtMsgOutbox->AddPacket(messageSize,(BYTE*)datamessage,0,playerPtr->m_currentWorldID);
	}
	else
	{//send raw
       m_netComObject->SendData((BYTE*)datamessage,
				                messageSize,
				                DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW,
					            sendTo,
								m_netComObject->m_packetTimeout);

	}//end send raw

	if (datamessage)
		delete [] datamessage;
	    datamessage=0;


	return TRUE;
}
//--------------send message----------------------------------------------------------//
BOOL CSharedNet::SpawnDynamicAIToAvailAiServer(int aiCFG,
											   int worldID, 
											   D3DVECTOR position,
											   int spawnID,
											   long lifeLength,
											   int  clanIDOpt,
											   int maxDynSpawnMaxCount,
											   short spawnArrayCount,
											   short spawnArrayOffset,
											   int spawnRotationRange,
											   CString worldMapName)
{

       if(worldMapName.GetLength() < 1 || worldMapName.GetLength() > 24)
	   {
		  m_eventLog->LogByPriority("**SpawnDynamicAIToAvailAiServer() Invalid world Map. :"+worldMapName,2);
          return FALSE;
	   }
	   
	   CAccountObject * aiAccount = m_netComObject->m_runtimeAccountDB->GetFirstAIServer();
	  
	   if(!aiAccount)
	   {
		   m_eventLog->LogByPriority("**SpawnDynamicAIToAvailAiServer() cant find ai server.",(DWORD)worldID,2);
		   return FALSE;
	   }

		AIDYNSPAWN_POS packetData;
		ZeroMemory(&packetData,sizeof(AIDYNSPAWN_POS));
		packetData.m_catagorie				= 40;
		packetData.m_dynaID					= spawnID;
		packetData.m_lifeLine				= lifeLength;
		packetData.m_xPos					= (short)position.x;
		packetData.m_yPos					= (short)position.y;
		packetData.m_zPos					= (short)position.z;
		packetData.m_aiCfg					= aiCFG;
		packetData.m_worldChannel			= worldID;
		packetData.m_clanID					= clanIDOpt;
		packetData.m_dynaIDMaxSpawn			= maxDynSpawnMaxCount;
		packetData.m_spawnArrayCount		= spawnArrayCount;
	    packetData.m_spawnArrayOffset		= spawnArrayOffset;
		packetData.m_spawnRotationRange		= spawnRotationRange;

        if(worldMapName.GetLength() > 4 && worldMapName.GetLength() < (int)sizeof(packetData.worldName))//worldName is char[25]
		{
			CopyMemory(packetData.worldName,worldMapName.GetBuffer(),worldMapName.GetLength());

		    m_netComObject->SendData((BYTE *)&packetData,
											sizeof(AIDYNSPAWN_POS),
											DPNSEND_GUARANTEED,
											aiAccount->m_currentID,
											m_netComObject->m_packetTimeout);

			m_eventLog->LogByPriority("**SpawnDynamicAIToAvailAiServer() Spawn AI msg sent. :"+worldMapName,(DWORD)worldID,2);
		}
		else
			m_eventLog->LogByPriority("**SpawnDynamicAIToAvailAiServer() INAVLID PARAMS :"+worldMapName,(DWORD)worldID,2);
		  


   return TRUE;
}
//--------------send message----------------------------------------------------------//
BOOL CSharedNet::SpawnAIChannel(int channel,
							    CString aiWorldEXG)
{
   if(aiWorldEXG.GetLength() >= 25)
      return FALSE;

   
    CAxList * aiSvrList = m_netComObject->m_runtimeAccountDB->GetAllOnIndex(1,"aiserver");
	if(!aiSvrList)
        return FALSE;

	if(aiSvrList->GetCount() < 1)
	    return FALSE;
 
    CAxLinkObj * posAIS = aiSvrList->GetHeadPosition_unsafe();
    if(!posAIS)
	    return FALSE;

    CAccountObject * accPtr = (CAccountObject * )aiSvrList->GetNext_unsafe(&posAIS);

	AIDYNSPAWN_POS packetData;
	ZeroMemory(&packetData,sizeof(AIDYNSPAWN_POS));
	packetData.m_catagorie = 101;
	packetData.m_worldChannel = channel;
    if(aiWorldEXG.GetLength() > 0)
		CopyMemory(packetData.worldName,aiWorldEXG.GetBuffer(),aiWorldEXG.GetLength());


	m_netComObject->SendData((BYTE *)&packetData,
			                  sizeof(AIDYNSPAWN_POS),
			                  DPNSEND_GUARANTEED,
			                  accPtr->m_currentID,
							  m_netComObject->m_packetTimeout);

	
    return TRUE;
}
//--------------send message----------------------------------------------------------//
BOOL CSharedNet::CloseAIChannel(int channel)
{
  
   
    CAxList * aiSvrList = m_netComObject->m_runtimeAccountDB->GetAllOnIndex(1,"aiserver");
	if(!aiSvrList)
        return FALSE;

	if(aiSvrList->GetCount() < 1)
	    return FALSE;
 
    CAxLinkObj * posAIS = aiSvrList->GetHeadPosition_unsafe();
    if(!posAIS)
	    return FALSE;

    CAccountObject * accPtr = (CAccountObject * )aiSvrList->GetNext_unsafe(&posAIS);

	AIDYNSPAWN_POS packetData;
	ZeroMemory(&packetData,sizeof(AIDYNSPAWN_POS));
	packetData.m_catagorie = 102;//close ai channel cat
	packetData.m_worldChannel = channel;


	m_netComObject->SendData((BYTE *)&packetData,
			                  sizeof(AIDYNSPAWN_POS),
			                  DPNSEND_GUARANTEED,
			                  accPtr->m_currentID,
							  m_netComObject->m_packetTimeout);

	
    return TRUE;
}
//------------------------------------------------------------------------------------//
void CSharedNet::RespawnToRebirthPoint(CPlayerObject * currentPlayer,CAccountObject * accountPtr,int * instanceTypeOvrd)
{
     if(!currentPlayer)
		 return;
 
     if(!accountPtr)
         return;
     
	 if(currentPlayer->m_aiControlled)
		return;

     CPlayerObject * characterOnAccount = accountPtr->GetCurrentCharacter();
     if(characterOnAccount)
	 {//valid char on account
	   if(currentPlayer->m_energy > 0)
	   {
	      m_netComObject->UpdateAccount(currentPlayer,accountPtr);//update account for skills
	      
		  if(currentPlayer->m_aiControlled == FALSE)
			 m_netComObject->m_currentWorldsUsed->DecrementZoneConnections_safe(currentPlayer->m_currentWorldID);
		  
		  if(m_netComObject->m_runtimePlayersOnServer->DeleteByOject(currentPlayer))//dont let nobody do anything to this
	         currentPlayer = 0;
	   }
		  
		  int potentialChannel = m_netComObject->m_currentWorldsUsed->GetChannelByName_safe(characterOnAccount->m_currentWorldMap);//TODO respawn needs complete rework
	      
          if(!potentialChannel)
		  {
			  m_netComObject->m_eventLog->AddEvent("Error 534..Invalid world spawn Ref:"+characterOnAccount->m_currentWorldMap);
              return;
		  }

		  characterOnAccount->m_currentWorldMap		= characterOnAccount->m_respawnWorldName;
		  characterOnAccount->m_currentWorldID		= potentialChannel;        
          characterOnAccount->m_3dPosition.x		= characterOnAccount->m_respawnPosition.x;
	      characterOnAccount->m_3dPosition.y		= characterOnAccount->m_respawnPosition.y;
	      characterOnAccount->m_3dPosition.z		= characterOnAccount->m_respawnPosition.z;

		  if(instanceTypeOvrd)
		     characterOnAccount->m_instanceType = *instanceTypeOvrd;
		  else
		     characterOnAccount->m_instanceType = 0;


          m_netComObject->SendSpawnMsg(DPNSEND_GUARANTEED,
                                            accountPtr->m_currentID,
								            (short)characterOnAccount->m_curEDBIndex,
								            characterOnAccount->m_currentWorldMap,
								            characterOnAccount->m_altCfgIndex,
								            (short)characterOnAccount->m_3dPosition.x,
								            (short)characterOnAccount->m_3dPosition.y,
								            (short)characterOnAccount->m_3dPosition.z);


	 }
}
//------------------------------------------------------------------------------------//
void CSharedNet::SpawnToRespawnPoint_requiresAcLock(CPlayerObject * currentPlayer,int * instanceTypeOvrd)
{
	 if(!currentPlayer)
		 return;

	 if(currentPlayer->m_aiControlled)
		return;

	 CAccountObject * accountPtr = m_netComObject->m_runtimeAccountDB->GetAccountByID(currentPlayer->m_currentID);
     if(!accountPtr)
         return;
     
     CPlayerObject * characterOnAccount = accountPtr->GetCurrentCharacter();
     if(characterOnAccount)
	 {//valid char on account
	   if(currentPlayer->m_energy > 0)
	   {
	      if(!m_netComObject->UpdateAccount(currentPlayer,accountPtr))//update account for skills  
              m_eventLog->LogByPriority("SpawnToPointInWld() UpdateAccount():",2);

		  if(currentPlayer->m_aiControlled == FALSE)
			 m_netComObject->m_currentWorldsUsed->DecrementZoneConnections_safe(currentPlayer->m_currentWorldID);

	      if(m_netComObject->m_runtimePlayersOnServer->RemoveByNetID_Alive(currentPlayer->m_networkAssignedID))//dont let nobody do anything to this
	         currentPlayer = 0;
	   }
		  
	      characterOnAccount->m_currentWorldMap   = characterOnAccount->m_respawnWorldName;        
          characterOnAccount->m_3dPosition.x      = characterOnAccount->m_respawnPosition.x;
	      characterOnAccount->m_3dPosition.y      = characterOnAccount->m_respawnPosition.y;
	      characterOnAccount->m_3dPosition.z      = characterOnAccount->m_respawnPosition.z;

		  if(instanceTypeOvrd)
		     characterOnAccount->m_instanceType = *instanceTypeOvrd;
		  else
		     characterOnAccount->m_instanceType = 0;

          
          m_netComObject->SendSpawnMsg(DPNSEND_GUARANTEED,
                                            accountPtr->m_currentID,
								            (short)characterOnAccount->m_curEDBIndex,
								            characterOnAccount->m_currentWorldMap,
								            characterOnAccount->m_altCfgIndex,
								            (short)characterOnAccount->m_3dPosition.x,
								            (short)characterOnAccount->m_3dPosition.y,
								            (short)characterOnAccount->m_3dPosition.z);

		  
	 }
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::SpawnFirstFromChannelToRespawn(int wldID,int * instanceTypeOvrd)
{
	 m_netComObject->m_runtimeAccountDB->Lock();
     m_netComObject->m_runtimePlayersOnServer->Lock();

	 CPlayerObject * currentPlayer = m_netComObject->m_runtimePlayersOnServer->GetFirstLiveOnChannelNoAI(wldID);
	 if(!currentPlayer)
	 {
         m_netComObject->m_runtimePlayersOnServer->Unlock();
		 m_netComObject->m_runtimeAccountDB->Unlock();
         return FALSE;
	 }
    
	 CAccountObject * accountPtr = m_netComObject->m_runtimeAccountDB->GetAccountByID(currentPlayer->m_currentID);
     if(!accountPtr)
	 {
	    m_netComObject->m_runtimePlayersOnServer->Unlock();
		m_netComObject->m_runtimeAccountDB->Unlock();
        return FALSE;
	 }
     
     CPlayerObject * characterOnAccount = accountPtr->GetCurrentCharacter();
     if(characterOnAccount)
	 {//valid char on account
	  
	      if(!m_netComObject->UpdateAccount(currentPlayer,accountPtr))//update account for skills  
              m_eventLog->LogByPriority("SpawnToPointInWld() UpdateAccount():",2);

		  if(currentPlayer->m_aiControlled == FALSE)
			 m_netComObject->m_currentWorldsUsed->DecrementZoneConnections_safe(currentPlayer->m_currentWorldID);
			 
		  m_netComObject->m_runtimePlayersOnServer->DeleteByOject(currentPlayer);
		  currentPlayer = 0;
		  
		 
	      characterOnAccount->m_currentWorldMap   = characterOnAccount->m_respawnWorldName;        
          characterOnAccount->m_3dPosition.x      = characterOnAccount->m_respawnPosition.x;
	      characterOnAccount->m_3dPosition.y      = characterOnAccount->m_respawnPosition.y;
	      characterOnAccount->m_3dPosition.z      = characterOnAccount->m_respawnPosition.z;

		  if(instanceTypeOvrd)
		     characterOnAccount->m_instanceType = *instanceTypeOvrd;
		  else
		     characterOnAccount->m_instanceType = 0;
          
              m_netComObject->SendSpawnMsg( DPNSEND_GUARANTEED,
											accountPtr->m_currentID,
											(short)characterOnAccount->m_curEDBIndex,
											characterOnAccount->m_currentWorldMap,
											characterOnAccount->m_altCfgIndex,
											(short)characterOnAccount->m_3dPosition.x,
											(short)characterOnAccount->m_3dPosition.y,
											(short)characterOnAccount->m_3dPosition.z);

		  
	 }
	 
	 m_netComObject->m_runtimePlayersOnServer->Unlock();
	 m_netComObject->m_runtimeAccountDB->Unlock();
	 return TRUE;
}
//------------------------------------------------------------------------------------//
void CSharedNet::SpawnToPointInWld(CPlayerObject * currentPlayer,D3DVECTOR position,CString worldName,int * instanceTypeOvrd)
{
	 if(!currentPlayer)
		 return;

	 if(currentPlayer->m_aiControlled)
		return;

	 if(currentPlayer->m_energy < 1)
		return;

	 CAccountObject * accountPtr = m_netComObject->m_runtimeAccountDB->GetAccountByID(currentPlayer->m_currentID);
     if(!accountPtr)
         return;
     
     CPlayerObject * characterOnAccount = accountPtr->GetCurrentCharacter();
     if(characterOnAccount)
	 {//valid char on account
	   
	  

	      if(!m_netComObject->UpdateAccount(currentPlayer,accountPtr))//update account for skills  
              m_eventLog->LogByPriority("SpawnToPointInWld() UpdateAccount():",2);   

		  if(currentPlayer->m_aiControlled == FALSE)
			 m_netComObject->m_currentWorldsUsed->DecrementZoneConnections_safe(currentPlayer->m_currentWorldID);

	      if(m_netComObject->m_runtimePlayersOnServer->RemoveByNetID_Alive(currentPlayer->m_networkAssignedID))//dont let nobody do anything to this
	         currentPlayer = 0;

	   

	      characterOnAccount->m_currentWorldID    = 0;        
          characterOnAccount->m_3dPosition.x      = position.x;
	      characterOnAccount->m_3dPosition.y      = position.y;
	      characterOnAccount->m_3dPosition.z      = position.z;
		  characterOnAccount->m_currentWorldMap   = worldName;

		  if(instanceTypeOvrd)
		     characterOnAccount->m_instanceType = *instanceTypeOvrd;
		  else
		     characterOnAccount->m_instanceType = 0;
		  

          m_netComObject->SendSpawnMsg(DPNSEND_GUARANTEED,
                                            accountPtr->m_currentID,
								            (short)characterOnAccount->m_curEDBIndex,
								            characterOnAccount->m_currentWorldMap,
								            characterOnAccount->m_altCfgIndex,
								            (short)characterOnAccount->m_3dPosition.x,
								            (short)characterOnAccount->m_3dPosition.y,
								            (short)characterOnAccount->m_3dPosition.z);

		  
	 }
}
//--------------send message----------------------------------------------------------//
void CSharedNet::ServerTimoutCallback(int processCount)
{
	if(!m_netComObject->m_runtimePlayersOnServer)
		return;

	unsigned int lastTraversal = 0;
	for(DWORD cTloop=0;cTloop<processCount;cTloop++)
	{//dyn loop-loop scales based on call freq
		if(m_timeoutTraversal > 30000)
			m_timeoutTraversal = 0;
		//traversal will be turn based, there's 30K possible net IDs 1per ms 30 seconds per total turn
		lastTraversal = m_timeoutTraversal;
		CPlayerObject * pPtr = m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(m_timeoutTraversal++);
		if(!pPtr)
			continue;     

		if(pPtr->m_justSpawned == TRUE && pPtr->m_aiControlled == FALSE)
		{
			m_netComObject->m_runtimePlayersOnServer->Unlock();
			Sleep(0);
			continue;
		}

		if(pPtr->m_alive == TRUE)
		{//make sure you are checking live clients not dead entities server side
			if((GetTickCount() - pPtr->m_timeLastUpdated) > m_netComObject->m_timeOutDuration)
			{//runtime timed out
				m_netComObject->m_runtimePlayersOnServer->Unlock();//unlock for reordering of locks
				m_netComObject->m_runtimeAccountDB->Lock();//account must always be locked before player
				pPtr = m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(lastTraversal);//revalidate
				if(!pPtr)
				{
					m_netComObject->m_runtimeAccountDB->Unlock();
					continue;
				}
				m_eventLog->LogByPriority("ServerTimoutCallback() Time out alive NetID:",(DWORD)lastTraversal,3);
				m_netComObject->LogsPlayerOut_requiredAccountLock(pPtr);//special lock case
				m_netComObject->m_runtimeAccountDB->Unlock();
				if(pPtr->m_aiControlled == FALSE)
					m_netComObject->m_currentWorldsUsed->DecrementZoneConnections_safe(pPtr->m_currentWorldID);
				m_netComObject->m_runtimePlayersOnServer->DeleteByOject(pPtr);
				m_netComObject->m_runtimePlayersOnServer->Unlock();
				continue;
			}//end runtime timed out
		}//end make sure you are checking live clients not dead entities server side
		else
		{//check dead entities
			//note: dead entities do not update accounts they are strictly server side at this point
			if(!pPtr->m_aiControlled)
			{//player character
				DWORD dedTime = m_netComObject->m_deadEntityDissolveTime;
				if(pPtr->m_currentInventory->GetTotalValue() < 1/* && m_currencySystem->EvalCurrency(pPtr->m_currentInventory) > 1*/)//dont do eval here, make sure currency glid items have value then its automatic 
					dedTime = (long)(((double)m_netComObject->m_deadEntityDissolveTime) * .01f);
				if((GetTickCount() - pPtr->m_timeLastUpdated) > dedTime)
				{//dissolved so remove 
					m_eventLog->LogByPriority("ServerTimoutCallback() Time out dead NPC NetID:",(DWORD)m_timeoutTraversal,3);		
					m_netComObject->m_runtimePlayersOnServer->DeleteByOject(pPtr);
					m_netComObject->m_runtimePlayersOnServer->Unlock();
					continue;
				}//end dissolved so remove  
			}//end player character
			else
			{//NPC
				BOOL dissolveNow = FALSE;
				if(pPtr->m_currentInventory)
				if(!pPtr->m_currentInventory->IsItemOfValuePresent())//dont use eval currency objects will have inventory value
					dissolveNow = TRUE;

				if((GetTickCount() - pPtr->m_timeLastUpdated) > m_netComObject->m_deadEntityNPCDissolveTime || dissolveNow == TRUE)
				{//dissolved so remove  
					m_eventLog->LogByPriority("ServerTimoutCallback() Time out alive NPC disolve NetID:",(DWORD)m_timeoutTraversal,3);
					m_netComObject->m_runtimePlayersOnServer->DeleteByOject(pPtr);
					m_netComObject->m_runtimePlayersOnServer->Unlock();
					continue;
				}//end dissolved so remove  
			}//end NPC
		}//end check dead entities
		m_netComObject->m_runtimePlayersOnServer->Unlock();
		Sleep(0);
	}//end dyn loop
}
//------------------------------------------------------------------------------------//
void CSharedNet::UpdateClientStatistics(CPlayerObject * player,DPID & idFromLoc)
{
	if(!player)
		return;
	
	if(!player->m_stats)
		return;

	if(player->m_aiControlled)
		return;

    DWORD currentTime = GetTickCount(); 
	
	int finalMaxEnergyBase = player->m_originalEDBEnergy;
	if(player->m_overridedEnergy > 0)
		finalMaxEnergyBase = player->m_overridedEnergy;

	 //Add in emblemBonus
	int emblemHPBonus		= player->m_currentInventory->GetEmblemHealthBonus();
	//Bonus from faction
	if((m_arenaFactionBonus > -1) && (player->m_factionStanding == m_arenaFactionBonus))
		emblemHPBonus += 50;
	player->m_maxEnergy		= player->m_stats->GetBenefitBonus(1,player->m_currentInventory) + finalMaxEnergyBase + emblemHPBonus;
	if(player->m_maxEnergy < player->m_energy)
		player->m_energy = player->m_maxEnergy;

	STATBLKHEADER_DATA header;
	ZeroMemory(&header,sizeof(STATBLKHEADER_DATA));
	int elementCount      = (int)m_elementDB->GetCount();
	int msgSize           = elementCount + sizeof(STATBLKHEADER_DATA);
	header.m_catagorie    = 22;
	header.m_hp           = player->m_energy;
	header.m_maxHp        = player->m_maxEnergy;
	header.m_stasis       = player->m_stasis;
	header.m_rechargeTimer = player->m_rechargeTimer;
	
	//Evaluate level and skill type
	header.m_skillType = 0;
	header.m_level     = 1;
	if(player->m_skills)
	{//get title
		CSkillObject* bestSkill = player->m_skills->GetBestSkillWEval(m_skillDatabase);
		if(bestSkill)
		{//valid skill
			int levelTemp = 0;
			CString title;
			CSkillObject * skillPtrRef = m_skillDatabase->GetSkillByType(bestSkill->m_skillType);
			if(skillPtrRef)
			{
				bestSkill->EvaluateLevelAndTitle(title,levelTemp,skillPtrRef);
				POSITION posLevel = skillPtrRef->m_levelRangeDB->GetHeadPosition();
				while(posLevel)
				{
					CLevelRangeObject *level = (CLevelRangeObject*)skillPtrRef->m_levelRangeDB->GetNext(posLevel);
					if(level->m_rangeStart < bestSkill->m_currentExperience && level->m_rangeEnd > bestSkill->m_currentExperience)
					{
						header.m_curXP = (int)bestSkill->m_currentExperience;
						header.m_maxXP = (int)level->m_rangeEnd;
						float levelRange = (float)level->m_rangeEnd - (float)level->m_rangeStart;
						float levelGrowth = (float)level->m_rangeEnd - (float)bestSkill->m_currentExperience;
						if(levelRange == 0.0f)
						   break;
						
						float xpHolder = levelGrowth / levelRange;
						header.m_xpPerc = (signed char)(xpHolder * 100);
						header.m_xpPerc = 100 - header.m_xpPerc;
						break;
					}
				}
			}
			header.m_skillType = bestSkill->m_skillType;
			header.m_level     = levelTemp;
			player->m_currentHighestLevel = levelTemp;
			player->m_currentHighestExperience = bestSkill->m_currentExperience;
		}//end valid skill
	}//end get title
	else
	{
		header.m_curXP = 0;
		header.m_maxXP = 1;
		header.m_xpPerc = 0;
	}

	//faction info setup
	header.m_curFaction = player->m_factionStanding;
	if((player->m_factionStanding > -1) && (player->m_factionStanding == m_arenaFactionBonus))
		header.m_facBonus1 = TRUE;
	else
		header.m_facBonus1 = FALSE;
	if((player->m_factionStanding > -1) && (player->m_factionStanding == m_pvpFactionBonus))
		header.m_facBonus2 = TRUE;
	else
		header.m_facBonus2 = FALSE;
	if((player->m_factionStanding > -1) && (player->m_factionStanding == m_clanFactionBonus))
		header.m_facBonus3 = TRUE;
	else
		header.m_facBonus3 = FALSE;
	
	 POSITION posLoc = player->m_stats->FindIndex(0);
	 if(posLoc)
	 {
         CStatObj * statPtr = (CStatObj *) player->m_stats->GetAt(posLoc);
	     header.m_str = (short)statPtr->GetValueIncludingBonusItems(player->m_currentInventory);
		 ////////////////////////
		 // Check for powerups //
		 ////////////////////////
		 if(player->m_bonus[1].m_bonusType == 1 || player->m_bonus[1].m_bonusType == 4)
		 {
			 header.m_str += player->m_bonus[1].m_bonusAmount;
			 if(header.m_str > 250 && player->m_stasis == 0)
				 header.m_str = 250;
			 header.m_maxHp += player->m_bonus[1].m_bonusAmount;
		 }
	 }
	 else
	 header.m_str = 0;

	 posLoc = player->m_stats->FindIndex(1);
	 if(posLoc)
	 {
         CStatObj * statPtr = (CStatObj *) player->m_stats->GetAt(posLoc);
	     header.m_dex = (short)statPtr->GetValueIncludingBonusItems(player->m_currentInventory);
		 ////////////////////////
		 // Check for powerups //
		 ////////////////////////
		 if(player->m_bonus[1].m_bonusType == 2 || player->m_bonus[1].m_bonusType == 4)
		 {
			 header.m_dex += player->m_bonus[1].m_bonusAmount;
			 if(header.m_dex > 250 && player->m_stasis == 0)
				 header.m_dex = 250;
		 }
	 }
	 else
	 header.m_dex = 0;

	 posLoc = player->m_stats->FindIndex(2);
	 if(posLoc)
	 {
         CStatObj * statPtr = (CStatObj *) player->m_stats->GetAt(posLoc);
	     header.m_wis = (short)statPtr->GetValueIncludingBonusItems(player->m_currentInventory);
		 ////////////////////////
		 // Check for powerups //
		 ////////////////////////
		 if(player->m_bonus[1].m_bonusType == 3 || player->m_bonus[1].m_bonusType == 4)
		 {
			 header.m_wis += player->m_bonus[1].m_bonusAmount;
			 if(header.m_wis > 250 && player->m_stasis == 0)
				 header.m_wis = 250;
		 }
	 }
	 else
	 header.m_wis = 0;

	 //Chronicle HP determination
//	 int chronicleMaxHp = ((header.m_str + header.m_dex) / 2) * header.m_wis; //current formula for hitpoints, may and more then likly will change

	 BYTE * byteBlock = new BYTE[msgSize];
	 CopyMemory(byteBlock,&header,sizeof(STATBLKHEADER_DATA));

     int tracer = sizeof(STATBLKHEADER_DATA);

	 for(int eleLoop = 0;eleLoop < elementCount;eleLoop++)
	 {
		 byteBlock[tracer] = player->m_currentInventory->GetArmourRating(eleLoop,TRUE,-99);
		 
		 ///////////////////////////////
	     // 03/24/2006 update         //
	     // Check for Reistance Bonus //
	     ///////////////////////////////
	  
	     if(player->m_bonus[2].m_bonusType == eleLoop + 6 || player->m_bonus[2].m_bonusType == 14)
		   byteBlock[tracer] += player->m_bonus[2].m_bonusAmount;
      
	     if(byteBlock[tracer] > 90)
		   byteBlock[tracer] = 90;

	     ////////////////
	     // End Update //
	     ////////////////
		 
		 tracer++;
	 }
	
	 // Check up on the player powerup for Stats
	 if(player->m_bonus[1].m_bonusType > 0 && player->m_bonus[1].m_bonusType < 5)
	 {
		 DWORD lastTime1 = currentTime - player->m_StatStamp;
		 player->m_StatStamp = currentTime;
		 
		 if(player->m_bonus[1].m_bonusDuration > 0)
			player->m_bonus[1].m_bonusDuration -= lastTime1;

		 if(player->m_bonus[1].m_bonusDuration <= 0)
		 {
			 player->m_bonus[1].m_bonusAmount = 0;
			 player->m_bonus[1].m_bonusDuration = 0;
			 player->m_bonus[1].m_bonusType = 0;
			 player->m_bonus[1].m_damageMult = 0;
			 player->m_bonus[1].m_maxDuration = 0;
			 player->m_StatStamp = 0;
		 }
	 }

	 // Check up on the player powerup for Resist
	 if(player->m_bonus[2].m_bonusType > 5)
	 {
		 DWORD lastTime2 = currentTime - player->m_ResistStamp;
		 player->m_ResistStamp = currentTime;

		 if(player->m_bonus[2].m_bonusDuration > 0)
			player->m_bonus[2].m_bonusDuration -= lastTime2;
		 
		 if(player->m_bonus[2].m_bonusDuration <= 0)
		 {
			 player->m_bonus[2].m_bonusAmount = 0;
			 player->m_bonus[2].m_bonusDuration = 0;
			 player->m_bonus[2].m_bonusType = 0;
			 player->m_bonus[2].m_damageMult = 0;
			 player->m_bonus[2].m_maxDuration = 0;
			 player->m_ResistStamp = 0;
		 }
	 }



	 player->m_statisticsOutbox->Flush();
	 player->m_statisticsOutbox->AddPacket(msgSize,(BYTE*)byteBlock,0,player->m_currentWorldID);
	 delete [] byteBlock;
	 byteBlock = 0;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::UpdateClientRecentQuestJournalEntry(CPlayerObject * targetPayer)
{
	// ERROR checking
    if(!targetPayer
	|| targetPayer->m_aiControlled
	|| !targetPayer->m_playerOrigRef
	|| !targetPayer->m_playerOrigRef->m_questJournal
	|| (targetPayer->m_playerOrigRef->m_questJournal->GetCount() == 0))
	    return FALSE;
	
	BOOL deleteQPtr = FALSE;
    CQuestJournalObj *questPtr = targetPayer->m_playerOrigRef->m_questJournal->GetMostRecentUncompleted();
    if(!questPtr)
	{
	    questPtr = new CQuestJournalObj();
		questPtr->m_npcName = "No pending Quests";
		deleteQPtr = TRUE;
	}

	// CALCULATE size of entire packet
	int currentSize = 1; // Packet ID
    currentSize += questPtr->m_questDescription.GetLength()+1;
    currentSize += questPtr->m_npcName.GetLength()+1;
	currentSize += questPtr->m_keyStatement.GetLength()+1;
	if (questPtr->m_requiredItems)
		currentSize += questPtr->m_requiredItems->GetCount();
	currentSize += 1;
	currentSize += 1; // Quantity of required items

    if(questPtr->m_requiredItems)
	{
		// ADD length of required item names
	   for(POSITION reqPos = questPtr->m_requiredItems->GetHeadPosition();reqPos != NULL;)
	   {
              CQJRequiredItem * reqItemPtr = (CQJRequiredItem *)questPtr->m_requiredItems->GetNext(reqPos);
			  if(reqItemPtr->m_logType == 4)
                 reqItemPtr->m_itemName = "?????";
			  
			  currentSize += reqItemPtr->m_itemName.GetLength()+1;
	   }
	}

	// ERROR checking: Do we have any data to send?
    if(currentSize == 1)
       return FALSE;

    // BUILD data packet
    BYTE *blockData = new BYTE[currentSize];
	blockData[0] = 3;//journal data identifier
	int traceDataBuild = 1;

	// ENCODE quest description
    blockData[traceDataBuild] = questPtr->m_questDescription.GetLength();
    traceDataBuild++;
    for(int i=0;i<questPtr->m_questDescription.GetLength();i++)
	{
        blockData[traceDataBuild] = questPtr->m_questDescription.GetAt(i);
   	    traceDataBuild++;
	}

	// ENCODE quest NPC name
	blockData[traceDataBuild] = questPtr->m_npcName.GetLength();
    traceDataBuild++;
	for(int i=0;i<questPtr->m_npcName.GetLength();i++)
	{
        blockData[traceDataBuild] = questPtr->m_npcName.GetAt(i);
   	    traceDataBuild++;
	}
	
	// ENCODE quest key statement
    blockData[traceDataBuild] = questPtr->m_keyStatement.GetLength();
    traceDataBuild++;
	for(int i=0;i<questPtr->m_keyStatement.GetLength();i++)
	{
        blockData[traceDataBuild] = questPtr->m_keyStatement.GetAt(i);
   	    traceDataBuild++;
	}

	// ENCODE quest required items
	if(questPtr->m_requiredItems)
	{
		blockData[traceDataBuild] = questPtr->m_requiredItems->GetCount();
		traceDataBuild++;	
	   for(POSITION reqPos = questPtr->m_requiredItems->GetHeadPosition();reqPos != NULL;)
	   {
              CQJRequiredItem * reqItemPtr = (CQJRequiredItem *)questPtr->m_requiredItems->GetNext(reqPos);

			  // ENCODE quest required item
			  blockData[traceDataBuild] = reqItemPtr->m_itemName.GetLength();
              traceDataBuild++;
	          for(int i=0;i<reqItemPtr->m_itemName.GetLength();i++)
	          {
                  blockData[traceDataBuild] = reqItemPtr->m_itemName.GetAt(i);
   	              traceDataBuild++;
	          }

			  // ENCODE quantity of quest required item
			  blockData[traceDataBuild] = reqItemPtr->m_quanity;
              traceDataBuild++;
	   }
	}

	// ENCODE quest status
	blockData[traceDataBuild] = questPtr->m_status;
	++traceDataBuild;
   
	// QUEUE the packet for transmission
	if(targetPayer->m_miscOutBox->GetCount() < 20)
		targetPayer->m_miscOutBox->AddPacket(currentSize,(BYTE*)blockData,0,targetPayer->m_currentWorldID);

	// RELEASE memory
    if(deleteQPtr)
       SafeDelete(questPtr);
	SafeDeleteArray(blockData);
	return TRUE;
}
//--------------send message----------------------------------------------------------//
BOOL CSharedNet::RequestMurdersToReport(CPlayerObject * characterObj)
{
    if(!characterObj)
	   return FALSE;

    if(!characterObj->m_noteriety)
	   return FALSE;

	if(!characterObj->m_noteriety->m_currentAgressors)
	   return FALSE;

    if(characterObj->m_noteriety->m_currentMurders >= m_netComObject->m_murderCountMax)
	{
	   characterObj->m_noteriety->m_currentAgressors->Flush();
	   return FALSE;
	}

	characterObj->m_noteriety->m_currentAgressors->CacheTimedOutAgressors(350000);

	for(POSITION posLoc = characterObj->m_noteriety->m_currentAgressors->GetHeadPosition();posLoc != NULL;)
	{//agressor loop
        CAggressorObj * aPtr = (CAggressorObj *)characterObj->m_noteriety->m_currentAgressors->GetNext(posLoc);

		if(m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(characterObj->m_stencilChannel,characterObj->m_currentWorldID,WORLDRULE_NOMURDERCOUNT))//filter murders in this world
           continue;

		CPlayerObject * murderer = m_netComObject->m_runtimePlayersOnServer->GetObjectByHandle(aPtr->m_handle);//find murderer
        if(murderer)
	    {//found the murderer

		   if(characterObj->m_clanHandle > -1)
		   if(characterObj->m_clanHandle == murderer->m_clanHandle)
		      continue;//find another candidate

           //send attrib to open murder window
		   int currentSize = 3;
		   currentSize += murderer->m_charName.GetLength();
		   currentSize += 4;
		   
		   //Build Data
           BYTE * blockData = new BYTE[currentSize];
	       blockData[0] = 5;//journal data identifier
		   blockData[1] = 201;//menu id
		   blockData[2] = murderer->m_charName.GetLength();//string length
	       int traceDataBuild = 3;
           int sz=0;
		   for(sz=0;sz<murderer->m_charName.GetLength();sz++)
		   {
		       blockData[traceDataBuild++] = murderer->m_charName.GetAt(sz);
		   }

		   BYTE xFloatCast[4];
		   CopyMemory(xFloatCast,&murderer->m_handle,sizeof(int));
		   for(sz=0;sz<4;sz++)
		   {
		       blockData[traceDataBuild++] = xFloatCast[sz];
		   }

		   if(characterObj->m_miscOutBox->GetCount() < 20)
		   characterObj->m_miscOutBox->AddPacket(currentSize,(BYTE*)blockData,0,characterObj->m_currentWorldID);

		   delete [] blockData;
		   blockData = 0;

		   return TRUE;
		}//end found the murderer
	}//end agressor loop
   return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::SendGuerenteedCycleMsgToClient(CPlayerObject * runtimePtr)
{
    if(!runtimePtr)
		return FALSE;

	if(runtimePtr->m_aiControlled)
	   return FALSE;

    ENTITYGUPDATE_HEADER header;
	header.m_catagorie          = 38;
	header.m_attributeMsgCount  = (int)runtimePtr->m_attributeOutbox->GetCount();
    header.m_dmgMsgCount        = 0;
	header.m_txtMsgCount        = (int)runtimePtr->m_txtMsgOutbox->GetCount();
	header.m_equipMsgCount      = (int)runtimePtr->m_equipMsgOutbox->GetCount();
	header.m_updateStatMsgCount = (int)runtimePtr->m_statisticsOutbox->GetCount();
	header.m_grpTxtMsgCount     = 0;//in ai this is the world channel ID
	header.m_miscMsgCount       = (int)runtimePtr->m_miscOutBox->GetCount();

	if(header.m_attributeMsgCount == 0 &&
	   header.m_dmgMsgCount == 0 &&
	   header.m_txtMsgCount == 0 && 
	   header.m_equipMsgCount == 0 && 
	   header.m_updateStatMsgCount == 0 && 
	   header.m_grpTxtMsgCount == 0 && 
	   header.m_miscMsgCount == 0)
	   return FALSE;//nothin here

	//get datasize for txt msgs
	int txtDataSize = 0;
	POSITION szPos;
    for(szPos = runtimePtr->m_txtMsgOutbox->GetHeadPosition();szPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) runtimePtr->m_txtMsgOutbox->GetNext(szPos);
		txtDataSize += (pkPtr->m_size+1);
	}
    //statistics size
	int statisticsSize = 0;
	for(szPos = runtimePtr->m_statisticsOutbox->GetHeadPosition();szPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) runtimePtr->m_statisticsOutbox->GetNext(szPos);
		statisticsSize += (pkPtr->m_size+1);
	}
	//statistics size
	int miscSize = 0;
	for(szPos = runtimePtr->m_miscOutBox->GetHeadPosition();szPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) runtimePtr->m_miscOutBox->GetNext(szPos);
		miscSize += (pkPtr->m_size+2);
	}

	
    int messageSize = sizeof(ENTITYGUPDATE_HEADER)+
		              (sizeof(ATTRIB_STRUCT) * header.m_attributeMsgCount)+
					  (sizeof(EQUIP_STRUCT)*header.m_equipMsgCount)+
					  txtDataSize+
					  statisticsSize+
					  miscSize;

    if(messageSize < 1)
	{
	   m_netComObject->m_eventLog->LogByPriority("*******!!SendGuerenteedCycleMsgToClient messageSize negative size "+runtimePtr->m_charName,(DWORD)messageSize,2);
	   return FALSE;
	}

    if(messageSize > 30000000)
	{//mem too large		
		runtimePtr->m_attributeOutbox->Flush();
		runtimePtr->m_txtMsgOutbox->Flush();
		runtimePtr->m_equipMsgOutbox->Flush();
		runtimePtr->m_statisticsOutbox->Flush();
		runtimePtr->m_miscOutBox->Flush();

		m_netComObject->m_eventLog->LogByPriority("*******!!SendGuerenteedCycleMsgToClient extensive size "+runtimePtr->m_charName,(DWORD)messageSize,2);
	    return FALSE;
	}



    BYTE * dataPacket = new BYTE[messageSize];

    CopyMemory(dataPacket,&header,sizeof(ENTITYGUPDATE_HEADER));
    int dataTracer = sizeof(ENTITYGUPDATE_HEADER);
    //attributes
	POSITION pkPos;
	for(pkPos = runtimePtr->m_attributeOutbox->GetHeadPosition();pkPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) runtimePtr->m_attributeOutbox->GetNext(pkPos);
        if(pkPtr->m_size > 0)
			CopyMemory(&dataPacket[dataTracer],pkPtr->m_data,pkPtr->m_size);
        dataTracer += pkPtr->m_size;
	}
	//eqip msgs
	for(pkPos = runtimePtr->m_equipMsgOutbox->GetHeadPosition();pkPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) runtimePtr->m_equipMsgOutbox->GetNext(pkPos);
		if(pkPtr->m_size > 0)
			CopyMemory(&dataPacket[dataTracer],pkPtr->m_data,pkPtr->m_size);
        dataTracer += pkPtr->m_size;
	}
	//txt msgs
	for(pkPos = runtimePtr->m_txtMsgOutbox->GetHeadPosition();pkPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) runtimePtr->m_txtMsgOutbox->GetNext(pkPos);
        dataPacket[dataTracer] = pkPtr->m_size;
		dataTracer++;
		if(pkPtr->m_size > 0)
			CopyMemory(&dataPacket[dataTracer],pkPtr->m_data,pkPtr->m_size);
        dataTracer += pkPtr->m_size;
	}
	//statistics update
	for(pkPos = runtimePtr->m_statisticsOutbox->GetHeadPosition();pkPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) runtimePtr->m_statisticsOutbox->GetNext(pkPos);
		dataPacket[dataTracer] = pkPtr->m_size;
	    dataTracer++;
		if(pkPtr->m_size > 0)
			CopyMemory(&dataPacket[dataTracer],pkPtr->m_data,pkPtr->m_size);
        dataTracer += pkPtr->m_size;
	}
	//misc msgs
	BYTE castByteTemp[2];
	short castShortTemp;
	for(pkPos = runtimePtr->m_miscOutBox->GetHeadPosition();pkPos != NULL;)
	{
        CPacketObj * pkPtr = (CPacketObj *) runtimePtr->m_miscOutBox->GetNext(pkPos);
        castShortTemp = (short)pkPtr->m_size;
		CopyMemory(castByteTemp,&castShortTemp,sizeof(short));
		dataPacket[dataTracer] = castByteTemp[0];
		dataTracer++;
        dataPacket[dataTracer] = castByteTemp[1];
		dataTracer++;
		if(pkPtr->m_size > 0)
			CopyMemory(&dataPacket[dataTracer],pkPtr->m_data,pkPtr->m_size);
        dataTracer += pkPtr->m_size;
	}
	

    m_netComObject->SendData(dataPacket,
				                  messageSize,
				                  DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH,
				                  runtimePtr->m_currentID,
								  m_netComObject->m_packetTimeout);

	delete [] dataPacket;
	dataPacket = 0;

	runtimePtr->m_attributeOutbox->Flush();
	runtimePtr->m_txtMsgOutbox->Flush();
	runtimePtr->m_equipMsgOutbox->Flush();
	runtimePtr->m_statisticsOutbox->Flush();
	runtimePtr->m_miscOutBox->Flush();

	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::RequestGroupList(CPlayerObject *player, DPNID target)
{
	 int size = 0;
     size++;//catagorie

	 if(player->m_groupDatabase)
	 {
		 for(POSITION posLoc = player->m_groupDatabase->GetHeadPosition();posLoc != NULL;)
		 {
			CCommGroupObj * cCommPtr = (CCommGroupObj *) player->m_groupDatabase->GetNext(posLoc);
			size++;//bump for header
			size += (4 + cCommPtr->m_name.GetLength());
		 }
	 }

	BYTE * dataMsg = new BYTE[size];
	int trace = 0;
	dataMsg[0] = 31;//set catagorie
	trace++;

	if(player->m_groupDatabase)
	{
		for(POSITION posLoc = player->m_groupDatabase->GetHeadPosition();posLoc != NULL;)
		{
			CCommGroupObj * cCommPtr = (CCommGroupObj *) player->m_groupDatabase->GetNext(posLoc);

			//BYTE cnvInt[4];
			//CopyMemory(cnvInt,&cCommPtr->m_handle,4);

			// Store Length of Name in Position 1
			dataMsg[trace] = cCommPtr->m_name.GetLength();//store name length
			trace++;

			// Store Handle in Position 2
			CopyMemory(&dataMsg[trace],&cCommPtr->m_handle,sizeof(int));
			trace += sizeof(int);

			if(cCommPtr->m_name.GetLength() > 0)
				CopyMemory(&dataMsg[trace],cCommPtr->m_name.GetBuffer(),cCommPtr->m_name.GetLength());
			trace += cCommPtr->m_name.GetLength();

			/*for(int loop=0;loop<4;loop++){
			dataMsg[trace] = cnvInt[loop];
			trace++;
			}
			for(loop=0;loop<cCommPtr->m_name.GetLength();loop++){
			dataMsg[trace] = cCommPtr->m_name.GetAt(loop);
			trace++;
			}*/
		}
	}

	m_netComObject->SendByteBlockMsg(DPNSEND_GUARANTEED,
		target,
		dataMsg,
		size,
		m_netComObject->m_packetTimeout);
	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::DeleteGroupMemberByHandle(CPlayerObject * player,int handle)
{

     if(!player)
		 return FALSE;

	 if(!player->m_playerOrigRef)
         return FALSE;

     if(!player->m_playerOrigRef->m_groupDatabase)
		 return FALSE;

     CPlayerObject * otherPlayer = m_netComObject->m_runtimePlayersOnServer->GetObjectByHandle(handle);

	 
     if(otherPlayer)
	 {
        if(!otherPlayer->m_aiControlled)
		{
		  if(otherPlayer->m_playerOrigRef)
          if(otherPlayer->m_playerOrigRef->m_groupDatabase)
             otherPlayer->m_playerOrigRef->m_groupDatabase->RemoveByHandle(player->m_handle);
		}
	 }
    
	    if(player->m_playerOrigRef->m_groupDatabase)
           player->m_playerOrigRef->m_groupDatabase->RemoveByHandle(handle);


	 return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::GeneralReloadCommand(CPlayerObject * currentPlayer)
{

   CInventoryObj * weaponLink = currentPlayer->m_currentInventory->GetArmedInvenItemByLocationID(12);
   if(weaponLink)
   {//aquire weapon used
	   POSITION posLast = NULL;
	   if(weaponLink->m_itemData->m_chargeCurrent >= weaponLink->m_itemData->m_maxCharges)
		  return FALSE;//ammo is full dont waste

	   for(POSITION posLoc = currentPlayer->m_currentInventory->GetHeadPosition();(posLast = posLoc) != NULL;)
	   {//inventory loop
	       CInventoryObj * plObj = (CInventoryObj *)currentPlayer->m_currentInventory->GetNext(posLoc);
           if(plObj->m_itemData->m_useType == 2)
		   {//ammo type
              if(weaponLink->m_itemData->m_chargeType == plObj->m_itemData->m_chargeType)
			  {//compatibility
				weaponLink->m_itemData->m_chargeCurrent += plObj->m_itemData->m_maxCharges;
				if(weaponLink->m_itemData->m_chargeCurrent > weaponLink->m_itemData->m_maxCharges)
					weaponLink->m_itemData->m_chargeCurrent = weaponLink->m_itemData->m_maxCharges;

				//Reset the depletion so it can be used again
				weaponLink->m_itemData->m_tempDepletionHolder = 0;
				weaponLink->m_itemData->m_weaponDepletionTimer = 0;
				weaponLink->m_itemData->m_secsToGo = 0;
	
				 SendAttribMsg(DPNSEND_GUARANTEED,
							   currentPlayer->m_currentID,
							   27,//attribute
							   weaponLink->m_itemData->m_maxCharges,//
							   0,
							   weaponLink->m_itemData->m_chargeCurrent,
							   weaponLink->m_itemData->m_shotsPerFire,
							   currentPlayer);
				 currentPlayer->m_rechargeTimer = 0;
				 UpdateClientStatistics(currentPlayer, currentPlayer->m_currentID);
			/*	 SendAttribMsg(	DPNSEND_GUARANTEED,
								currentPlayer->m_currentID,
								41,
								0,
								0,
								0,
								0,
								currentPlayer);
 */
				 currentPlayer->m_currentInventory->DeleteByGLID(plObj->m_itemData->m_globalID);

				 return TRUE;
			  }//end compatibility
		   }//end ammo type
	   }//end inventory loop
   }//end aquire weapon used
   return FALSE;
}
//--------------send message----------------------------------------------------------//
BOOL CSharedNet::SendSkillsToClient(CPlayerObject * playerFocus, bool sendTitles)
{

	if(!playerFocus->m_skills)
	{
		SendGeneralMessageHandler(25, playerFocus);//no skills notification
		return FALSE;
	}

    int skillCount = (int)playerFocus->m_skills->GetCount();

    if(skillCount == 0)
	{
		SendGeneralMessageHandler(25, playerFocus);//no skills notification
		return FALSE;
	}

	SendTextMsg(DPNSEND_GUARANTEED, playerFocus->m_currentID, "Got Here!", playerFocus->m_networkAssignedID, playerFocus, 0);

    SKILLS_HEADER header;
	header.m_catagorie = 35;
    header.m_skillCount = skillCount;

	if(sendTitles)
		header.m_catagorie = 43;

	int intMsgSz;
	if(sendTitles)
		intMsgSz = sizeof(SKILLS_HEADER) + (2 * skillCount);
	else
		intMsgSz = sizeof(SKILLS_HEADER) + (3 * skillCount);

	BYTE *byteBlock = new BYTE[intMsgSz];

	CopyMemory(byteBlock,&header,sizeof(SKILLS_HEADER));
    int tracerFill = sizeof(SKILLS_HEADER);
	for(POSITION posLoc = playerFocus->m_skills->GetHeadPosition();posLoc != NULL;)
	{
        CSkillObject * skPtr = (CSkillObject *)playerFocus->m_skills->GetNext(posLoc);
        CString name;
		int level = 0;
        CSkillObject * skillPtrRef = m_skillDatabase->GetSkillByType(skPtr->m_skillType);
		int percentageToNextLevel = 0;
		if(skillPtrRef)
		{
			if(sendTitles)
				level = (int)skPtr->m_currentExperience;
			else
			{
				skPtr->EvaluateLevelAndTitle(name,level,skillPtrRef);
				percentageToNextLevel = skPtr->GetPercentageToNextLevel(skillPtrRef);
			}
		}

		byteBlock[tracerFill++] = skPtr->m_skillType;
		byteBlock[tracerFill++] = level;

		CString output = "Skill ";
		output += m_cabLib.ConvToString(skPtr->m_skillType);
		output += ": ";
		output += m_cabLib.ConvToString(level);
		SendTextMsg(DPNSEND_GUARANTEED, playerFocus->m_currentID, output, playerFocus->m_networkAssignedID, playerFocus, 0);

		if(!sendTitles)
			byteBlock[tracerFill++] = percentageToNextLevel;
	}
	
	m_netComObject->SendData(byteBlock,
							 intMsgSz,
							 DPNSEND_GUARANTEED,
							 playerFocus->m_currentID,
							 m_netComObject->m_packetTimeout);

	if(byteBlock)
	{
		delete [] byteBlock;
		byteBlock = 0;
	}

  return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::SendItemLocationMsg(DWORD flags,DPNID sendTo,CServerItemObj * lstObj) 
{
    int itemArmedCount = 0;

	CPlayerObject * recipiant = m_netComObject->m_runtimePlayersOnServer->GetByDPNID_Alive(sendTo);
    if(recipiant)
    if(recipiant->m_aiControlled)
       recipiant = NULL;

	if(recipiant)
	   if(recipiant->m_playerItemCfgOutbox->GetCount() > 5)
		  return FALSE;
 
	if(lstObj->m_contents)
	for(POSITION posLoc = lstObj->m_contents->GetHeadPosition();posLoc != NULL;)
	{//get item armed count
        CInventoryObj * invPtr = (CInventoryObj *)lstObj->m_contents->GetNext(posLoc);
        if(invPtr->m_armed)
           itemArmedCount++;
	}//end get item armed count

    BYTE * dataBlock = NULL;
	DWORD				messageSize;
	messageSize = sizeof(ITEM_LOCATION)+
		          lstObj->m_displayedName.GetLength()+
				  lstObj->m_spawnCfgCount+
				  (itemArmedCount * sizeof(short));
    dataBlock = new BYTE[messageSize];
	ZeroMemory(dataBlock,messageSize);
	
	ITEM_LOCATION header;

	header.m_catagorie         = 10;
	header.m_rotation          = lstObj->m_rotationAdjust;
	header.m_cfgLength         = lstObj->m_spawnCfgCount;
	header.Xpos                = lstObj->m_position.x;
	header.Ypos                = lstObj->m_position.y;
	header.Zpos                = lstObj->m_position.z;
	header.curAnim             = lstObj->m_curAnim;
	header.dbIndex             = lstObj->m_dbRef;
	header.networkID           = lstObj->m_networkAssignedID;
	header.m_itemArmedLength   = itemArmedCount;
	header.m_nameLength        = lstObj->m_displayedName.GetLength();
	header.mount               = lstObj->m_mountOption;
	header.m_autoTrigger       = lstObj->m_autoTrigger;
	//optional orientation
	header.useExactOrientation = lstObj->m_useExactOrientation;
	if(header.useExactOrientation)
	{//pack up the vectors
	   header.OrientateUpX = (byte) (127.0f * lstObj->m_upVector.x);  
	   header.OrientateUpY = (byte) (127.0f * lstObj->m_upVector.y);  
	   header.OrientateUpZ = (byte) (127.0f * lstObj->m_upVector.z);  
       header.OrientateDirX = (byte) (127.0f * lstObj->m_dirVector.x);  
	   header.OrientateDirY = (byte) (127.0f * lstObj->m_dirVector.y);  
	   header.OrientateDirZ = (byte) (127.0f * lstObj->m_dirVector.z);  
	}//end pack up the vectors

	CopyMemory(dataBlock,&header,sizeof(ITEM_LOCATION));

    int tracerBuild = sizeof(ITEM_LOCATION);
	
	if(header.m_nameLength > 0)
	{//copy name block
	   CopyMemory(&dataBlock[tracerBuild],lstObj->m_displayedName.GetBuffer(),header.m_nameLength);
	   tracerBuild += header.m_nameLength;
	}//end copy name block

	for(int i=0;i<lstObj->m_spawnCfgCount;i++)
	{//must keep loop until we change block type and or handler :(
		dataBlock[tracerBuild] = lstObj->m_spawnCfg[i];//conversion to be removed eventually-todo change to block copy when types are changed
		tracerBuild++;
	}

	if(itemArmedCount > 0)
	{//armed inventory
	   short tempConversion = 0;
	   int indexSH = sizeof(short);
       for(POSITION posLoc = lstObj->m_contents->GetHeadPosition();posLoc != NULL;)
	   {
           CInventoryObj * invPtr = (CInventoryObj *)lstObj->m_contents->GetNext(posLoc);
           if(invPtr->m_armed)
		   {
              tempConversion = invPtr->m_itemData->m_globalID;//must convert due to copy alignment :(
			  CopyMemory(&dataBlock[tracerBuild],&tempConversion,indexSH);
	          tracerBuild += indexSH;
		   }
	   }
	}//end armed inventory

    if(!recipiant)
	{
	   m_netComObject->SendData(dataBlock,
				                     messageSize,
				                     flags,
					                 sendTo,
								     m_netComObject->m_sceneStatePackTimeout);
	}
    else{
       recipiant->m_playerItemCfgOutbox->AddPacket(messageSize,(BYTE*)dataBlock,0,recipiant->m_currentWorldID);
	}


	if (dataBlock){
		delete [] dataBlock;
		dataBlock=0;
	}

	return TRUE;
}
//------------------------------------------------------------------------------------//
void CSharedNet::BroadcastInventoryChange(CPlayerObject * personWhoChangedInven,
										  int index,
										  DPNID messageFromDPID,
										  BOOL  informEntity,
										  CItemObj * refItem)
{
if(!personWhoChangedInven)
    return;

//first aknowledge focus
if(informEntity){
   SendEquipMsg(index,
			    0,
				personWhoChangedInven,
				5,
				0);
   
   if(refItem)
   {
    IfItemForceJetSendIt(refItem,personWhoChangedInven);
    if(refItem->m_locationID == 12)
	{//primary item
	 if(index > 0)//arming
	 {
		 SendAttribMsg( DPNSEND_GUARANTEED,
						messageFromDPID,
						27,//attribute
						refItem->m_maxCharges,//
						0,
						refItem->m_chargeCurrent,
						refItem->m_shotsPerFire,
						personWhoChangedInven);
		 personWhoChangedInven->m_rechargeTimer = refItem->m_weaponDepletionTimer;
	 }
	 else
	 {
		 SendAttribMsg( DPNSEND_GUARANTEED,
					    messageFromDPID,
					    27,//attribute
					    0,//
					    0,
					    0,
					    0,
					    personWhoChangedInven);
		 personWhoChangedInven->m_rechargeTimer = 0;
	 }
	 UpdateClientStatistics(personWhoChangedInven, personWhoChangedInven->m_currentID);
	}//end primary item
   }
}
//broadcast to all
       
       CAxList * channelList = m_netComObject->m_runtimePlayersOnServer->GetListByChannel(personWhoChangedInven->m_currentWorldID);
       if(channelList)
       for(CAxLinkObj* firePos = channelList->GetHeadPosition_unsafe();firePos != NULL;)
	   {//player loop
           CPlayerObject * pData = (CPlayerObject *)channelList->GetNext_unsafe(&firePos);
           if(!pData->m_inGame)
			  continue;
		   
		   if(pData->m_currentID == 0)
			  continue;
		   
		   if(pData->m_aiControlled)
			  continue;

           if(m_cabLib.BoundingSphereCheck((float)m_netComObject->m_serverCalculateRadius,0.0f,
					                       pData->m_3dPosition.x,pData->m_3dPosition.y,pData->m_3dPosition.z,
										   personWhoChangedInven->m_3dPosition.x,personWhoChangedInven->m_3dPosition.y,personWhoChangedInven->m_3dPosition.z))
		   {//bounding check
		     if(pData->m_networkAssignedID != personWhoChangedInven->m_networkAssignedID)//not same player
			    if(pData->m_currentWorldID == personWhoChangedInven->m_currentWorldID){//same world 
				      SendEquipMsg(index,
						           personWhoChangedInven->m_networkAssignedID,
								   pData,
								   5,
								   0);
			  }//end same world
		   }//end bounding check
	  }//end player loop
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::SendEquipMsg(int index,
						      int refrenceID,
							  CPlayerObject * playerObjectData,
							  int catagorie,
							  int dupCount) 
{
    LPEQUIP_STRUCT lpChatMessage = NULL;
	DWORD				messageSize;
	messageSize = sizeof(EQUIP_STRUCT);
    lpChatMessage = (LPEQUIP_STRUCT) new BYTE[messageSize];
	if (lpChatMessage == NULL)
	{
		return FALSE;
	}

	lpChatMessage->m_catagorie= catagorie;
	lpChatMessage->index      = index;
    lpChatMessage->idRefrence = refrenceID;
    lpChatMessage->CRCcheck   = m_netComObject->GetEquipCRC(lpChatMessage);
	lpChatMessage->dupCount   = dupCount;
	
    if(!playerObjectData)
	{
	    if(m_netComObject->m_equipOutBox)
	    if(m_netComObject->m_equipOutBox->GetCount() < 20)
	       m_netComObject->m_equipOutBox->AddPacket(messageSize,(BYTE*)lpChatMessage,0,0);
	}
	else if(playerObjectData){
	    if(playerObjectData->m_equipMsgOutbox->GetCount() < 30)
           playerObjectData->m_equipMsgOutbox->AddPacket(messageSize,(BYTE*)lpChatMessage,0,playerObjectData->m_currentWorldID);
	}


	if (lpChatMessage){
		delete lpChatMessage;
		lpChatMessage=0;
	}

	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::IfItemForceJetSendIt(CItemObj * item,CPlayerObject * playerToReceive)
{
      if(item->m_yJetforce != 0.0f || item->m_zJetforce != 0.0f)
	  {
         SendAttribMsg(DPNSEND_GUARANTEED,
			           playerToReceive->m_currentID,
			           28,//attribute
				       (int)(item->m_yJetforce * 1000.0f),
			           0,
				       (int)(item->m_zJetforce * 1000.0f),
			           0,
				       playerToReceive);
		 return TRUE;
	  }
  return FALSE;
}
//--------------send message----------------------------------------------------------//
BOOL CSharedNet::SendHousingState(DPNID targetID,int worldID)
{
    if(!m_netComObject->m_houseingDB)
		return FALSE;

	int houseCount = 0;
	POSITION posLoc;
    for(posLoc = m_netComObject->m_houseingDB->GetHeadPosition();posLoc != NULL;)
	{
        CHousingZone * spnPtr = (CHousingZone *)m_netComObject->m_houseingDB->GetNext(posLoc);
        if(spnPtr->m_wldID != worldID)
		   continue;
		
		if(!spnPtr->m_houseDatabase)
		   continue;
		
		houseCount += (int)spnPtr->m_houseDatabase->GetCount();
	}

	if(houseCount == 0)
	   return FALSE;

	int msgSz = (sizeof(HOUSE_DATA)*houseCount)+1+4;

    HOUSE_DATA * houseBlockData = new HOUSE_DATA[houseCount];
	int trace = 0;
    for(posLoc = m_netComObject->m_houseingDB->GetHeadPosition();posLoc != NULL;)
	{
        CHousingZone * spnPtr = (CHousingZone *)m_netComObject->m_houseingDB->GetNext(posLoc);
        if(spnPtr->m_wldID != worldID)
			continue;
		if(!spnPtr->m_houseDatabase)
		   continue;
		
		for(POSITION hpos = spnPtr->m_houseDatabase->GetHeadPosition();hpos != NULL;)
		{
             CHPlacementObj * hpouse = (CHPlacementObj *)spnPtr->m_houseDatabase->GetNext(hpos);
             houseBlockData[trace].m_acID      = hpouse->m_accountBind;
			 houseBlockData[trace].m_houseType = hpouse->m_type;
			 houseBlockData[trace].m_posX      = (short)hpouse->m_wldPosition.x;
			 houseBlockData[trace].m_posY      = (short)hpouse->m_wldPosition.y;
			 houseBlockData[trace].m_posZ      = (short)hpouse->m_wldPosition.z;
			 trace++;
		}
	}

	BYTE * byteConversion = (BYTE*)houseBlockData;
	
	BYTE * byteBlock = new BYTE[msgSz];
	byteBlock[0] = 7;
	//for(int i=0;i<(msgSz-1);i++)
        //byteBlock[i+1] = byteConversion[i];
    if(msgSz > 5)
		CopyMemory(&byteBlock[1],byteConversion,(msgSz-5));
	CopyMemory(&byteBlock[msgSz-4],&worldID,4);
	


    m_netComObject->SendData((BYTE *)byteBlock,
				                          msgSz,
				                          DPNSEND_GUARANTEED,
					                      targetID,
										  m_netComObject->m_packetTimeout);
    
	delete [] houseBlockData;
	houseBlockData = 0;
	delete [] byteBlock;
	byteBlock = 0;

return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::SendBadgeMessage(CPlayerObject *pPlayer)
{
	if(!pPlayer)
		return FALSE;

	BYTE *badgeBlock = NULL;
	int itemCount = 0;
	int dataSize = 0;
	if(pPlayer->m_currentInventory->GenerateBadgeDataBlock(&badgeBlock, itemCount, dataSize))
	{
		// Add the packet to the outgoing message queue
		if(pPlayer->m_miscOutBox->GetCount() < 20)
			pPlayer->m_miscOutBox->AddPacket( dataSize,
											  badgeBlock,
											  0,
											  pPlayer->m_currentWorldID);
	}
	return TRUE;
}
BOOL 
CSharedNet::SendArenaMessage(CPlayerObject *pPlayer, ARENA_MESSAGE &message)
{
	//Make sure we have a player we are sending the data to
	if(!pPlayer)
		return FALSE;

	//Make sure we have Info to send
	int iCounter = 0;
	int msgsz = 0;
	BYTE *byteBlock = NULL;

	// Create network packet
	msgsz += sizeof(message);
	msgsz++; // +1 for the packet ID

	// Assemble the network packet
	byteBlock = new BYTE[msgsz];
	byteBlock[iCounter++] = 11; //Setup the catagory for HandleMiscBlocksClientSide
	CopyMemory(&byteBlock[iCounter], &message.ArenaMessage, sizeof(message.ArenaMessage));
	iCounter += sizeof(message.ArenaMessage);
	CopyMemory(&byteBlock[iCounter], &message.ArenaType, sizeof(message.ArenaType));
	iCounter += sizeof(message.ArenaType);
	CopyMemory(&byteBlock[iCounter], &message.Score, sizeof(message.Score));
	iCounter += sizeof(message.Score);
	CopyMemory(&byteBlock[iCounter], &message.Kills, sizeof(message.Kills));
	iCounter += sizeof(message.Kills);
	CopyMemory(&byteBlock[iCounter], &message.Deaths, sizeof(message.Deaths));
	iCounter += sizeof(message.Deaths);
	CopyMemory(&byteBlock[iCounter], &message.Damage, sizeof(message.Damage));
	iCounter += sizeof(message.Damage);
	CopyMemory(&byteBlock[iCounter], &message.Reward, sizeof(message.Reward));
	iCounter += sizeof(message.Reward);

	// Add the packet to the outgoing message queue
	if(pPlayer->m_miscOutBox->GetCount() < 20)
	pPlayer->m_miscOutBox->AddPacket(msgsz,
									(BYTE*)byteBlock,
									0,
									pPlayer->m_currentWorldID);

	// Release allocated resources
	SafeDeleteArray(byteBlock);
	return TRUE;
}

BOOL CSharedNet::SendGroupMessage(CPlayerObject *player, SQUAD_MESSAGE *groupInfo, int amount)
{
	//Make sure we have a player we are sending the data to
	if(!player) return FALSE;

	//Make sure we have Info to send
	int iCounter = 0;
	int msgsz = 0;
	BYTE *byteBlock = NULL;
	if(!groupInfo)
	{
		if(amount == -1)
		{
			// This indicates to a player that they are no longer in a group
			iCounter = 0;
			msgsz = 1;
			byteBlock = new BYTE[msgsz];
			byteBlock[iCounter] = 10;
			if(player->m_miscOutBox->GetCount() < 20)
			player->m_miscOutBox->AddPacket(msgsz, (BYTE*)byteBlock, 0, player->m_currentWorldID);

			delete[] byteBlock;
			byteBlock = 0;
			return TRUE;
		}
		return FALSE;
	}

	// Create network packet
	for(int i=0; i<amount; i++)
	{
		msgsz += sizeof(groupInfo[i].NetID) 
			+ sizeof(groupInfo[i].Health) 
			+ sizeof(groupInfo[i].Position)
			+ sizeof(groupInfo[i].PlayerNameLength)
			+ groupInfo[i].PlayerNameLength
			+ sizeof(groupInfo[i].SquadNameLength)
			+ groupInfo[i].SquadNameLength;
	}
	msgsz++;
	byteBlock = new BYTE[msgsz];
	byteBlock[iCounter++] = 9; //Setup the catagory for HandleMiscBlocksClientSide

	for(int i=0; i<amount; i++)
	{
		CopyMemory(&byteBlock[iCounter], &groupInfo[i].NetID, sizeof(groupInfo[i].NetID));
		iCounter += sizeof(groupInfo[i].NetID);
		CopyMemory(&byteBlock[iCounter], &groupInfo[i].Health, sizeof(groupInfo[i].Health));
		iCounter += sizeof(groupInfo[i].Health);
		CopyMemory(&byteBlock[iCounter], &groupInfo[i].Position, sizeof(groupInfo[i].Position));
		iCounter += sizeof(groupInfo[i].Position);
		CopyMemory(&byteBlock[iCounter], &groupInfo[i].PlayerNameLength, sizeof(groupInfo[i].PlayerNameLength));
		iCounter += sizeof(groupInfo[i].PlayerNameLength);
		CopyMemory(&byteBlock[iCounter], groupInfo[i].PlayerName, groupInfo[i].PlayerNameLength);
		iCounter += groupInfo[i].PlayerNameLength;
		CopyMemory(&byteBlock[iCounter], &groupInfo[i].SquadNameLength, sizeof(groupInfo[i].SquadNameLength));
		iCounter += sizeof(groupInfo[i].SquadNameLength);
		CopyMemory(&byteBlock[iCounter], groupInfo[i].SquadName, groupInfo[i].SquadNameLength);
		iCounter += groupInfo[i].SquadNameLength;
	}
	
	//CopyMemory(&byteBlock[1], groupInfo, (msgsz - 1));

	if(player->m_miscOutBox->GetCount() < 20)
	player->m_miscOutBox->AddPacket(msgsz,(BYTE*)byteBlock,0,player->m_currentWorldID);

	delete [] byteBlock;
	byteBlock = 0;

	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::SendUpdateToPlayer(DPNID playerId,CPlayerObject * pObj,BOOL AIMode)
{
		DWORD currentTime = this->GetTimeStampDW();
		if(pObj->m_curEDBIndex < 0){
		   m_netComObject->m_eventLog->AddEvent("Player sending data has no DB cfg ","cfg:",pObj->m_curEDBIndex);
		   return FALSE;
		}

	   //--------------------------//
	   //--Players around entity---//
	   //--------------------------//
	   ENTITYPOS_HEADER header;
	   int headerSize = sizeof(ENTITYPOS_HEADER);
	   header.m_catagorie           = 19;
	   header.m_entPosCount         = 0;
	   header.m_staticCount         = 0;
	   header.m_fireMsgCount        = (char)pObj->m_firemsgsoutbox->GetCount();//capped 20
	   header.m_itemCfgMsgCount     = (char)pObj->m_playerItemCfgOutbox->GetCount();//capped at 5
	   header.m_entitCfgMsgCount    = (char)pObj->m_playerEntityCfgOutbox->GetCount();//player capped at 5  house at 10
	   header.m_damageInfoMsgCount  = (char)pObj->m_playerEntityDamageMgsOutbox->GetCount();//capped at 10
	   header.m_generalMsgCount     = (char)pObj->m_playerEntityGeneralMgsOutbox->GetCount();//capped at 10
	   header.m_detailedMsgCount    = (char)pObj->m_playerEntityDetailedMgsOutbox->GetCount();//capped at 10

	   CHPlacementObjList * visibleList = NULL;
	   if(!pObj->m_aiControlled)
	   {
		 visibleList = m_netComObject->m_houseingDB->GetVisibleList(pObj->m_Target3dPosition,m_netComObject->m_houseSightRange,pObj->m_stencilChannel);//zones are stored by stencil channel
		 if(visibleList)
			header.m_houseMsgCount       = (short)visibleList->GetCount();//32000 //OPTIMIZE
		 else
			header.m_houseMsgCount       = 0;
	   }
	   else
		 header.m_houseMsgCount = 0;
	  
	   CAxList * channelList = m_netComObject->m_runtimePlayersOnServer->GetListByChannel(pObj->m_currentWorldID);
	   
	   if(channelList)
	   for(CAxLinkObj * posLoc = channelList->GetHeadPosition_unsafe();posLoc != NULL;)
	   {//runtime loop
		   CPlayerObject * lstObj = (CPlayerObject *)channelList->GetNext_unsafe(&posLoc);
		   if(AIMode == TRUE)
		   {//ai mode specific
			  if(lstObj->m_aiControlled == TRUE)//dont send data already known
				 continue;
			  if(lstObj->m_gm == TRUE)//dont send Gm position to server
				 continue;
		   }//end ai mode specific


		   if(lstObj->m_stasis == 1)//hidden
			  continue;

		   if(lstObj->m_justSpawned == TRUE)//hidden
			  continue;
		   
		   if(pObj->m_currentWorldID == lstObj->m_currentWorldID && 
				  lstObj->m_networkAssignedID != pObj->m_networkAssignedID){//same world //different computer link
				   if(lstObj->m_curEDBIndex >= 0)//valid
				   if(m_cabLib.BoundingSphereCheck((float)lstObj->m_visualDistanceRef,0.0f,
													lstObj->m_3dPosition.x,lstObj->m_3dPosition.y,lstObj->m_3dPosition.z,
													pObj->m_3dPosition.x,pObj->m_3dPosition.y,pObj->m_3dPosition.z) == TRUE || AIMode == TRUE)
				   {//bounding check
					  BOOL isAlive = TRUE;
					  if(lstObj->m_alive == FALSE && (currentTime - lstObj->m_timeDied) > 30000)
						 isAlive = FALSE;
					   
					   
					  if(isAlive == TRUE){//alive
						 //sizeOpt++;
						 header.m_entPosCount++;
					  }//end alive
					  else if(!AIMode){//dead
						 //staticCount++;
						 header.m_staticCount++;
					  }//end dead
				   }//end bounding check  
			   }//end same world //different computer link
	   }//end runtime loop
	   //size item loop
	   //Build Items Block
	   CAxList * rItemListByChannel = m_netComObject->m_runtimeItems->GetAllOnIndex(pObj->m_currentWorldID,"channel");//gets focused list indexed by channel
	   if(!AIMode)
	   {
		   m_netComObject->m_runtimeItems->Lock();
		   for(CAxLinkObj * iPos = rItemListByChannel->GetHeadPosition_unsafe();iPos != NULL;)
		   {//runtime item class
			   CServerItemObj * svrItem = (CServerItemObj *) rItemListByChannel->GetNext_unsafe(&iPos);
			   if(pObj->m_currentWorldID == svrItem->m_world)
			   if(m_cabLib.BoundingSphereCheck((float)svrItem->m_sightRange,0.0f,
														svrItem->m_position.x,svrItem->m_position.y,svrItem->m_position.z,
														pObj->m_3dPosition.x,pObj->m_3dPosition.y,pObj->m_3dPosition.z))
			   {//bounding check
				   header.m_staticCount++;
			   }//end bounding check  
		   }//end runtime item class
	   }

	   //dead items
	   int playerItemCfgSz = 0; 
	   int playerEntityCfgSz = 0; 
	   POSITION pioPos;
	   for(pioPos = pObj->m_playerItemCfgOutbox->GetHeadPosition();pioPos != NULL;)
	   {
			  CPacketObj * pktPtr = (CPacketObj *)pObj->m_playerItemCfgOutbox->GetNext(pioPos);
			  playerItemCfgSz++;
		      playerItemCfgSz += pktPtr->m_size;
	   }

	   for(pioPos = pObj->m_playerEntityCfgOutbox->GetHeadPosition();pioPos != NULL;)
	   {
		  CPacketObj * pktPtr = (CPacketObj *)pObj->m_playerEntityCfgOutbox->GetNext(pioPos);
		  playerEntityCfgSz++;
		  playerEntityCfgSz += pktPtr->m_size;
	   }

       //ALLOCATE AND PREP
	   int datasize = (sizeof(SHRUNK_MOVE_STRUCT)*header.m_entPosCount)+
					   headerSize+
					   (sizeof(STATICENTITY_DATA)*header.m_staticCount)+
					   (sizeof(FIRE_STRUCT)*header.m_fireMsgCount)+
					   (sizeof(MSGHANDLER)*header.m_generalMsgCount)+
					   (sizeof(DAMAGEINFO_DATA)*header.m_damageInfoMsgCount)+
					   (sizeof(DETAILEDMSG_DATA)*header.m_detailedMsgCount)+
					   (sizeof(GENERICCONFIG_STRUCT)*header.m_houseMsgCount)+
					   playerItemCfgSz+
					   playerEntityCfgSz;

	   BYTE * byteBlock = new BYTE[datasize];
	   ZeroMemory(byteBlock,datasize);
	   BYTE * headerByteForm = (BYTE*)&header;
	   if(headerSize > 0)
			CopyMemory(byteBlock,headerByteForm,headerSize);

	   int trace = 0;
	   int staticTrace = 0;
	   int staticEntMemShift = (int)headerSize+(sizeof(SHRUNK_MOVE_STRUCT)*header.m_entPosCount);
	   int dynEntMemShift = headerSize;
	   MOVE_STRUCT mvMsgBld;
	   STATICENTITY_DATA staticStruct;
	   SHRUNK_MOVE_STRUCT shMovMsg;
	   int staticStructSz = sizeof(STATICENTITY_DATA);
	   int dynEntStructSize = sizeof(SHRUNK_MOVE_STRUCT);
	  
	   if(!AIMode)
	   {
		   for(CAxLinkObj * iPos = rItemListByChannel->GetHeadPosition_unsafe();iPos != NULL;)
		   {//runtime item class
			 CServerItemObj * svrItem = (CServerItemObj *) rItemListByChannel->GetNext_unsafe(&iPos);

			 if(pObj->m_currentWorldID == svrItem->m_world)
			   if(m_cabLib.BoundingSphereCheck(svrItem->m_sightRange,0.0f,svrItem->m_position.x,svrItem->m_position.y,svrItem->m_position.z,pObj->m_3dPosition.x,pObj->m_3dPosition.y,pObj->m_3dPosition.z))
			   {//bounding check
				   staticStruct.m_networAssignedID = (short)-svrItem->m_networkAssignedID;
				   CopyMemory(&byteBlock[staticEntMemShift+(staticTrace*staticStructSz)],&staticStruct,staticStructSz);
				   staticTrace++;
			   }//end bounding check  
		   }//end runtime item class
		   m_netComObject->m_runtimeItems->Unlock();
	   }

	   if(channelList)
	   for(CAxLinkObj * posLoc = channelList->GetHeadPosition_unsafe();posLoc != NULL;)
	   {//runtime loop
		   CPlayerObject * lstObj = (CPlayerObject *)channelList->GetNext_unsafe(&posLoc);
		   if(AIMode == TRUE)
		   {//ai mode specific
			  if(lstObj->m_aiControlled == TRUE)//dont send data already known
				 continue;
			  if(lstObj->m_gm == TRUE)//dont send Gm position to server
				 continue;
		   }//end ai mode specific

		   if(lstObj->m_stasis == 1)//hidden
			  continue;

		   if(lstObj->m_justSpawned == TRUE)//hidden
			  continue;

			   if(pObj->m_currentWorldID == lstObj->m_currentWorldID && 
				  lstObj->m_networkAssignedID != pObj->m_networkAssignedID){//same world //different computer link
				   if(lstObj->m_curEDBIndex >= 0)//valid
				   if(m_cabLib.BoundingSphereCheck((float)lstObj->m_visualDistanceRef,0.0f,
													lstObj->m_3dPosition.x,lstObj->m_3dPosition.y,lstObj->m_3dPosition.z,
													pObj->m_3dPosition.x,pObj->m_3dPosition.y,pObj->m_3dPosition.z) == TRUE || AIMode == TRUE)
				   {//bounding check
					 //handle interpolation
					 InterpolateNetworkEntiesServerSide(lstObj,currentTime);
				        
					 BOOL isAlive = TRUE;
					  if(lstObj->m_alive == FALSE && (currentTime - lstObj->m_timeDied) > 30000)
						 isAlive = FALSE;

					 if(isAlive == TRUE)
					 {//alive non static
					   
					   
					   mvMsgBld.m_catagorie    = 2;
					   mvMsgBld.FreeLookYaw    = lstObj->m_freeLookYaw;
					   mvMsgBld.FreeLookPitch  = lstObj->m_freeLookPitch;
					   mvMsgBld.Xpos           = lstObj->m_3dPosition.x;
					   mvMsgBld.Ypos           = lstObj->m_3dPosition.y;
					   mvMsgBld.Zpos           = lstObj->m_3dPosition.z;
					   mvMsgBld.idRefrence     = lstObj->m_networkAssignedID;
					   mvMsgBld.OrientateDirX  = lstObj->m_dirVector.x;
     				   mvMsgBld.OrientateDirY  = lstObj->m_dirVector.y;
					   mvMsgBld.OrientateDirZ  = lstObj->m_dirVector.z;
					   mvMsgBld.OrientateUpX   = lstObj->m_upVector.x;
					   mvMsgBld.OrientateUpY   = lstObj->m_upVector.y;
					   mvMsgBld.OrientateUpZ   = lstObj->m_upVector.z;
					   mvMsgBld.curAnim        = lstObj->m_curAnim;
					   mvMsgBld.secondaryAnim  = lstObj->m_secondaryAnim;
					   
					   m_netComObject->ShrinkData(&mvMsgBld,&shMovMsg);
					   //CopyMemory(&movBlackData[trace],&shMovMsg,sizeof(SHRUNK_MOVE_STRUCT));
					   CopyMemory(&byteBlock[dynEntMemShift+(trace*dynEntStructSize)],&shMovMsg,dynEntStructSize);
					   trace++;

					 }//end alive non static
					 else if(!AIMode){//static
					   //staticBlockData[staticTrace].m_networAssignedID = lstObj->m_networkAssignedID;
					   staticStruct.m_networAssignedID = (short)lstObj->m_networkAssignedID;
					   CopyMemory(&byteBlock[staticEntMemShift+(staticTrace*staticStructSz)],&staticStruct,staticStructSz);
					   staticTrace++;
					 }//end static
				   }//end bounding check
			   }//end same world
	   }//end runtime loop
	   
	   //build fire msg block
	   int curMemPos = staticEntMemShift+(staticTrace*staticStructSz);//from end of static to maintain format of cur client
	   POSITION fbxPos;
	   for(fbxPos = pObj->m_firemsgsoutbox->GetHeadPosition();fbxPos != NULL;)
	   {//fire send out box
		  CPacketObj * pktPtr = (CPacketObj *)pObj->m_firemsgsoutbox->GetNext(fbxPos);
		  if(pktPtr->m_size > 0)
			  CopyMemory(&byteBlock[curMemPos],pktPtr->m_data,pktPtr->m_size);
		  curMemPos+=pktPtr->m_size;
	   }//end fire send out box
	   pObj->m_firemsgsoutbox->Flush();//clear stuff

	   //build dmg msg block
	   for(fbxPos = pObj->m_playerEntityDamageMgsOutbox->GetHeadPosition();fbxPos != NULL;)
	   {//dmg send out box
		  CPacketObj * pktPtr = (CPacketObj *)pObj->m_playerEntityDamageMgsOutbox->GetNext(fbxPos);
		  if(pktPtr->m_size > 0)
			  CopyMemory(&byteBlock[curMemPos],pktPtr->m_data,pktPtr->m_size);
		  curMemPos+=pktPtr->m_size;
	   }//end dmg send out box
	   pObj->m_playerEntityDamageMgsOutbox->Flush();

	   //build gen msg block
	   for(fbxPos = pObj->m_playerEntityGeneralMgsOutbox->GetHeadPosition();fbxPos != NULL;)
	   {//gen msg send out box
		  CPacketObj * pktPtr = (CPacketObj *)pObj->m_playerEntityGeneralMgsOutbox->GetNext(fbxPos);
		  if(pktPtr->m_size > 0)
			  CopyMemory(&byteBlock[curMemPos],pktPtr->m_data,pktPtr->m_size);
		  curMemPos+=pktPtr->m_size;
	   }//end gen msg send out box
	   pObj->m_playerEntityGeneralMgsOutbox->Flush();

	   //build detailed msg block
	   for(fbxPos = pObj->m_playerEntityDetailedMgsOutbox->GetHeadPosition();fbxPos != NULL;)
	   {//gen msg send out box
		  CPacketObj * pktPtr = (CPacketObj *)pObj->m_playerEntityDetailedMgsOutbox->GetNext(fbxPos);
		  if(pktPtr->m_size > 0)
			  CopyMemory(&byteBlock[curMemPos],pktPtr->m_data,pktPtr->m_size);
		  curMemPos+=pktPtr->m_size;
	   }//end detailed msg send out box
	   pObj->m_playerEntityDetailedMgsOutbox->Flush();

       POSITION icfgPos;
	   for(icfgPos = pObj->m_playerItemCfgOutbox->GetHeadPosition();icfgPos != NULL;)
	   {
			  CPacketObj * pktPtr = (CPacketObj *)pObj->m_playerItemCfgOutbox->GetNext(icfgPos);
			  byteBlock[curMemPos] = pktPtr->m_size;
			  curMemPos++;
			  if(pktPtr->m_size > 0)
				  CopyMemory(&byteBlock[curMemPos],pktPtr->m_data,pktPtr->m_size);
		      curMemPos += pktPtr->m_size;
	   }
	   pObj->m_playerItemCfgOutbox->Flush();

	   for(icfgPos = pObj->m_playerEntityCfgOutbox->GetHeadPosition();icfgPos != NULL;)
	   {
		  CPacketObj * pktPtr = (CPacketObj *)pObj->m_playerEntityCfgOutbox->GetNext(icfgPos);
		  byteBlock[curMemPos] = pktPtr->m_size;
		  curMemPos++;
		  if(pktPtr->m_size > 0)
			  CopyMemory(&byteBlock[curMemPos],pktPtr->m_data,pktPtr->m_size);
		  curMemPos += pktPtr->m_size;
	   }
	   pObj->m_playerEntityCfgOutbox->Flush();

	   if(visibleList)
	   {
		 GENERICCONFIG_STRUCT genStruct;
		 int structSz = sizeof(GENERICCONFIG_STRUCT);
		 int tracerOne = 0;
		 for(POSITION icfgPos = visibleList->GetHeadPosition();icfgPos != NULL;)
		 {
		  CHPlacementObj * hsPtr = (CHPlacementObj *)visibleList->GetNext(icfgPos);
		  genStruct.m_acID = hsPtr->m_accountBind;
		  genStruct.m_open = hsPtr->m_open;

		  CopyMemory(&byteBlock[curMemPos],&genStruct,structSz);
		  curMemPos += structSz;
		 }
	   }

	  //send data
	  if(AIMode)
	  m_netComObject->SendByteBlockMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH,
					   playerId,
					   byteBlock,
					   datasize,
					   m_netComObject->m_packetTimeout
					   ); 
	  else
	  m_netComObject->SendByteBlockMsg(DPNSEND_NOCOMPLETE | DPNSEND_PRIORITY_HIGH,
					   playerId,
					   byteBlock,
					   datasize,
					   m_netComObject->m_sceneStatePackTimeout
					   ); 

	  if(visibleList)
	  {//dont delete objects they are referenced
		 visibleList->RemoveAll();
		 delete visibleList;
		 visibleList = 0;
	  }//end dont delete contenets that are referenced


return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::SendUpdateToAIChannel_safe(DPNID playerId,int worldChannel)
{
   
   CAccountObject * accRef = m_netComObject->m_runtimeAccountDB->GetAccountByID_LOS(playerId);
   if(!accRef)
       return FALSE;

   if(!accRef->m_aiServer)
   {
       m_netComObject->m_runtimeAccountDB->Unlock();
       return FALSE;
   }


   DWORD currentTime = this->GetTimeStampDW();
   AIPOS_HEADER header;
   int headerSize = sizeof(AIPOS_HEADER);
   header.m_catagorie           = 18;
   header.m_entPosCount         = 0;
   
   header.m_entitCfgMsgCount    = 0;
   if(accRef->m_playerEntityCfgOutbox)
      header.m_entitCfgMsgCount = accRef->m_playerEntityCfgOutbox->GetCountBaseOnChannel(worldChannel);
   
   header.m_damageMsgCount		= 0;
   if(accRef->m_attributeOutbox)
      header.m_damageMsgCount = accRef->m_attributeOutbox->GetCountBaseOnChannel(worldChannel);

   header.m_detailedMsgCount		= 0;
   if(accRef->m_detailMsgOutbox)
      header.m_detailedMsgCount = accRef->m_detailMsgOutbox->GetCountBaseOnChannel(worldChannel);
   
   m_netComObject->m_runtimePlayersOnServer->Lock();
   CAxList * channelList = m_netComObject->m_runtimePlayersOnServer->GetListByChannel(worldChannel);
   
   header.m_worldChannel        = worldChannel;
   if(channelList)
   for(CAxLinkObj * posLoc = channelList->GetHeadPosition_unsafe();posLoc != NULL;)
   {//runtime loop
       CPlayerObject * lstObj = (CPlayerObject *)channelList->GetNext_unsafe(&posLoc);
	  
	   if(lstObj->m_aiControlled == TRUE)//dont send data already known
	      continue;
       if(lstObj->m_gm == TRUE)//dont send Gm position to server
	      continue;
	  
	   if(lstObj->m_stasis == 1)//hidden
	      continue;

	   if(lstObj->m_justSpawned == TRUE)//hidden
	      continue;
	   
	   if(worldChannel == lstObj->m_currentWorldID)
	   {//same world //different computer link
			   if(lstObj->m_curEDBIndex >= 0)//valid
			   {//bounding check
				  BOOL isAlive = TRUE;
				  if(lstObj->m_alive == FALSE && (currentTime - lstObj->m_timeDied) > 30000)
				     isAlive = FALSE;
				   
				  if(isAlive == TRUE)
				  {//alive
				     header.m_entPosCount++;
				  }//end alive
			   }//end bounding check  
		   }//end same world //different computer link
   }//end runtime loop
  
   //build block msg
    SHRUNK_MOVE_STRUCT   * movBlackData          = NULL;
    STATICENTITY_DATA    * staticBlockData       = NULL;
    FIRE_STRUCT          * fireBlockData         = NULL;
    MSGHANDLER           * genMsgs               = NULL;
    DAMAGEINFO_DATA      * dmgMsgs               = NULL;
    DETAILEDMSG_DATA     * detailedMsgs          = NULL;
   

   if(header.m_entPosCount > 0)
      movBlackData    = new SHRUNK_MOVE_STRUCT[header.m_entPosCount];

   int trace = 0;
   int staticTrace = 0;

   if(channelList)
   for(CAxLinkObj * posLoc = channelList->GetHeadPosition_unsafe();posLoc != NULL;)
   {//runtime loop
       CPlayerObject * lstObj = (CPlayerObject *)channelList->GetNext_unsafe(&posLoc);
   
	   if(lstObj->m_aiControlled == TRUE)//dont send data already known
	      continue;

       if(lstObj->m_gm == TRUE)//dont send Gm position to server
	      continue;

	   if(lstObj->m_stasis == 1)//hidden
	      continue;

	   if(lstObj->m_justSpawned == TRUE)//hidden
	      continue;

		   if(worldChannel == lstObj->m_currentWorldID)
		   {//same world //different computer link
			   if(lstObj->m_curEDBIndex >= 0)//valid
			   {//bounding check
                 //handle interpolation
                 InterpolateNetworkEntiesServerSide(lstObj,currentTime);
			        
				 BOOL isAlive = TRUE;
				  if(lstObj->m_alive == FALSE && (currentTime - lstObj->m_timeDied) > 30000)
				     isAlive = FALSE;

				 if(isAlive == TRUE)
				 {//alive non static
				   
				   MOVE_STRUCT mvMsgBld;
                   mvMsgBld.m_catagorie    = 2;
	               mvMsgBld.FreeLookYaw    = lstObj->m_freeLookYaw;
                   mvMsgBld.FreeLookPitch  = lstObj->m_freeLookPitch;
	               mvMsgBld.Xpos           = lstObj->m_3dPosition.x;
	               mvMsgBld.Ypos           = lstObj->m_3dPosition.y;
	               mvMsgBld.Zpos           = lstObj->m_3dPosition.z;
                   mvMsgBld.idRefrence     = lstObj->m_networkAssignedID;
                   mvMsgBld.OrientateDirX  = lstObj->m_dirVector.x;
         	       mvMsgBld.OrientateDirY  = lstObj->m_dirVector.y;
	               mvMsgBld.OrientateDirZ  = lstObj->m_dirVector.z;
	               mvMsgBld.OrientateUpX   = lstObj->m_upVector.x;
	               mvMsgBld.OrientateUpY   = lstObj->m_upVector.y;
	               mvMsgBld.OrientateUpZ   = lstObj->m_upVector.z;
	               mvMsgBld.curAnim        = lstObj->m_curAnim;
	               mvMsgBld.secondaryAnim  = lstObj->m_secondaryAnim;

				   //if(trace < 100 || AIMode == TRUE){
					  SHRUNK_MOVE_STRUCT shMovMsg;
                      m_netComObject->ShrinkData(&mvMsgBld,&shMovMsg);
				      CopyMemory(&movBlackData[trace],&shMovMsg,sizeof(SHRUNK_MOVE_STRUCT));
				   //}
				
				   trace++;

				 }//end alive non static
			   }//end bounding check
		   }//end same world
   }//end runtime loop

      int entityCfgDatasz = 0;
      if(header.m_entitCfgMsgCount > 0)
      {//aquire item cfg size
		 entityCfgDatasz = accRef->m_playerEntityCfgOutbox->GetDataSizeByChannel(worldChannel,1);
      }//end aquire item size

	  int attribDatasz = 0;
      if(header.m_damageMsgCount > 0)
      {//aquire item cfg size
		 attribDatasz = accRef->m_attributeOutbox->GetDataSizeByChannel(worldChannel,1);
      }//end aquire item size

	  int detailDatasz = 0;
      if(header.m_detailedMsgCount > 0)
      {//aquire item cfg size
		 detailDatasz = accRef->m_detailMsgOutbox->GetDataSizeByChannel(worldChannel,1);
      }//end aquire item size


      int datasize = (sizeof(SHRUNK_MOVE_STRUCT)*header.m_entPosCount)+
		             headerSize+
					 entityCfgDatasz+
					 attribDatasz+
					 detailDatasz;
      BYTE * byteBlock = new BYTE[datasize];
      BYTE * headerByteForm = (BYTE*)&header;
      CopyMemory(byteBlock,headerByteForm,headerSize);

	  int dynamicSz      = sizeof(SHRUNK_MOVE_STRUCT)*header.m_entPosCount;
	  
	  if(movBlackData){
	     //BYTE * entData = (BYTE*)movBlackData;
	     //for(int transLoop=0;transLoop<dynamicSz;transLoop++)
             //byteBlock[transLoop+headerSize] = entData[transLoop];
         CopyMemory(&byteBlock[headerSize],movBlackData,dynamicSz);
	  }

	  if(entityCfgDatasz > 0)
	  {
	     int additionalIndexer = headerSize+dynamicSz;
	     int tracerOne = 0;
		 POSITION icfgPosLast = NULL;
		 for(POSITION icfgPos = accRef->m_playerEntityCfgOutbox->GetHeadPosition();(icfgPosLast = icfgPos) != NULL;)
		 {
             CPacketObj * pktPtr = (CPacketObj *)accRef->m_playerEntityCfgOutbox->GetNext(icfgPos);
             if(pktPtr->m_channelID == worldChannel)
			 {
                byteBlock[tracerOne+additionalIndexer] = pktPtr->m_size;
                tracerOne++;
                
				if(pktPtr->m_size > 0)
					CopyMemory(&byteBlock[tracerOne+additionalIndexer],pktPtr->m_data,pktPtr->m_size);

                tracerOne += pktPtr->m_size;
				delete pktPtr;
				pktPtr = 0;
				accRef->m_playerEntityCfgOutbox->RemoveAt(icfgPosLast);
			 }
		 }
	  }


	  if(attribDatasz > 0)
	  {
	     int additionalIndexer = headerSize+dynamicSz+entityCfgDatasz;
	     int tracerOne = 0;
		 POSITION icfgPosLast = NULL;
		 for(POSITION icfgPos = accRef->m_attributeOutbox->GetHeadPosition();(icfgPosLast = icfgPos) != NULL;)
		 {
             CPacketObj * pktPtr = (CPacketObj *)accRef->m_attributeOutbox->GetNext(icfgPos);
             if(pktPtr->m_channelID == worldChannel)
			 {
                byteBlock[tracerOne+additionalIndexer] = pktPtr->m_size;
                tracerOne++;
                
				if(pktPtr->m_size > 0)
					CopyMemory(&byteBlock[tracerOne+additionalIndexer],pktPtr->m_data,pktPtr->m_size);

                tracerOne += pktPtr->m_size;
				delete pktPtr;
				pktPtr = 0;
				accRef->m_attributeOutbox->RemoveAt(icfgPosLast);
			 }
		 }
	  }

	  if(detailDatasz > 0)
	  {
	     int additionalIndexer = headerSize+dynamicSz+entityCfgDatasz+attribDatasz;
	     int tracerOne = 0;
		 POSITION icfgPosLast = NULL;
		 for(POSITION icfgPos = accRef->m_detailMsgOutbox->GetHeadPosition();(icfgPosLast = icfgPos) != NULL;)
		 {
             CPacketObj * pktPtr = (CPacketObj *)accRef->m_detailMsgOutbox->GetNext(icfgPos);
             if(pktPtr->m_channelID == worldChannel)
			 {
				if(pktPtr->m_size > 0)
					CopyMemory(&byteBlock[tracerOne+additionalIndexer],pktPtr->m_data,pktPtr->m_size);

                tracerOne += pktPtr->m_size;
				delete pktPtr;
				pktPtr = 0;
				accRef->m_detailMsgOutbox->RemoveAt(icfgPosLast);
			 }
		 }
	  }




      m_netComObject->SendByteBlockMsg(DPNSEND_GUARANTEED,
					   playerId,
					   byteBlock,
					   datasize,
					   m_netComObject->m_packetTimeout
					   ); 
      

	  if(movBlackData)
	  {
         delete [] movBlackData;
         movBlackData = 0;
	  }
	  
	  if(staticBlockData)
	  {
         delete [] staticBlockData;
         staticBlockData = 0;
	  }

	  m_netComObject->m_runtimePlayersOnServer->Unlock();
	  m_netComObject->m_runtimeAccountDB->Unlock();

return TRUE;
}
//------------------------------------------------------------------------------------//
int CSharedNet::GetNextWeaponToArm(CInventoryObjList * invList,
								   CSkillObjectList * skills,
								   CStatObjList * stats)
{
	BOOL foundWeapon = FALSE;
	int indexArmed = 0;
	POSITION posLoc;
    for(posLoc = invList->GetHeadPosition();posLoc != NULL;)
	{
       CInventoryObj * invObj = (CInventoryObj *)invList->GetNext(posLoc);
	   
 
	   if(invObj->m_itemData->m_locationID == 12)
	   {
		BOOL proceed = TRUE;
		foundWeapon = TRUE;
        if(invObj->m_itemData->m_requiredSkillType > -1 && invObj->m_itemData->m_requiredSkillLevel > 1)
		{
		  if(skills)
		  {
		   if(!skills->EvaluateRequiredSkillLevel(invObj->m_itemData->m_requiredSkillType,invObj->m_itemData->m_requiredSkillLevel,m_skillDatabase))
               proceed = FALSE;
		  }
		  else
		  proceed = FALSE;
		}

		if(stats)
		  if(!stats->VerifyStatRequirement((float)invObj->m_itemData->m_requiredStr,2,invList))
		     proceed = FALSE;
		
	    if(stats)
		  if(!stats->VerifyStatRequirement((float)invObj->m_itemData->m_requiredDex,3,invList))
		     proceed = FALSE;

	    if(stats)
		  if(!stats->VerifyStatRequirement((float)invObj->m_itemData->m_requiredKnowledge,4,invList))
		     proceed = FALSE;
		
		
		  if(proceed)
		  {
		   
           if(invObj->m_armed)
		   {

              break;
		   }
		  }
	   }
	   indexArmed++;
	}
 
	if(!foundWeapon)//no weap in inventory
		return -1;
  
	int newGlid = -1;

    for(posLoc = invList->FindIndex(indexArmed+1);posLoc != NULL;)
	{
        CInventoryObj * invObj = (CInventoryObj *)invList->GetNext(posLoc);
		if(invObj->m_itemData->m_locationID == 12)
		{
		    BOOL proceed = TRUE;
            if(invObj->m_itemData->m_requiredSkillType > -1 && invObj->m_itemData->m_requiredSkillLevel > 1)
			{
		     if(skills)
			 {
		       if(!skills->EvaluateRequiredSkillLevel(invObj->m_itemData->m_requiredSkillType,invObj->m_itemData->m_requiredSkillLevel,m_skillDatabase))
                   proceed = FALSE;
			 }
		     else
		           proceed = FALSE;
			}

		     if(stats)
		      if(!stats->VerifyStatRequirement((float)invObj->m_itemData->m_requiredStr,2,invList))
			       proceed = FALSE;
		
	   	     if(stats)
		      if(!stats->VerifyStatRequirement((float)invObj->m_itemData->m_requiredDex,3,invList))
			       proceed = FALSE;

		     if(stats)
		      if(!stats->VerifyStatRequirement((float)invObj->m_itemData->m_requiredKnowledge,4,invList))
			       proceed = FALSE;
			
			if(proceed)
			return invObj->m_itemData->m_globalID;
		}
	}

	int tracer = 0;
	for(posLoc = invList->GetHeadPosition();posLoc != NULL;)
	{
        CInventoryObj * invObj = (CInventoryObj *)invList->GetNext(posLoc);
		
		if(tracer == indexArmed)
		   return -1;
		if(invObj->m_itemData->m_locationID == 12)
		{
           BOOL proceed = TRUE;
            if(invObj->m_itemData->m_requiredSkillType > -1 && invObj->m_itemData->m_requiredSkillLevel > 1)
			{
		     if(skills)
			 {
		       if(!skills->EvaluateRequiredSkillLevel(invObj->m_itemData->m_requiredSkillType,invObj->m_itemData->m_requiredSkillLevel,m_skillDatabase))
                   proceed = FALSE;
			 }
		     else
		           proceed = FALSE;
			}

		     if(stats)
		      if(!stats->VerifyStatRequirement((float)invObj->m_itemData->m_requiredStr,2,invList))
			       proceed = FALSE;
		
	   	     if(stats)
		      if(!stats->VerifyStatRequirement((float)invObj->m_itemData->m_requiredDex,3,invList))
			       proceed = FALSE;

		     if(stats)
		      if(!stats->VerifyStatRequirement((float)invObj->m_itemData->m_requiredKnowledge,4,invList))
			       proceed = FALSE;
			
			if(proceed)
		       return invObj->m_itemData->m_globalID;
		}
		tracer++;
	}
return -1;
}
//------------------------------------------------------------------------------------//
int CSharedNet::GetPrevWeaponToArm(CInventoryObjList * invList,
								   CSkillObjectList * skills,
								   CStatObjList * stats)
{
	BOOL foundWeapon = FALSE;
	int indexArmed = 0;
	POSITION posLoc;
    for(posLoc = invList->GetTailPosition();posLoc != NULL;)
	{
        CInventoryObj * invObj = (CInventoryObj *)invList->GetPrev(posLoc);
		if(invObj->m_itemData->m_locationID == 12)
		{
		  foundWeapon = TRUE;
		  BOOL proceed = TRUE;
          if(invObj->m_itemData->m_requiredSkillType > -1 && invObj->m_itemData->m_requiredSkillLevel > 1)
		  {
		   if(skills)
		   {
		    if(!skills->EvaluateRequiredSkillLevel(invObj->m_itemData->m_requiredSkillType,invObj->m_itemData->m_requiredSkillLevel,m_skillDatabase))
               proceed = FALSE;
		   }
		   else
		       proceed = FALSE;
		  }

		   if(stats)
		     if(!stats->VerifyStatRequirement((float)invObj->m_itemData->m_requiredStr,2,invList))
			     proceed = FALSE;
		
	  	   if(stats)
		     if(!stats->VerifyStatRequirement((float)invObj->m_itemData->m_requiredDex,3,invList))
			     proceed = FALSE;

		   if(stats)
		     if(!stats->VerifyStatRequirement((float)invObj->m_itemData->m_requiredKnowledge,4,invList))
			     proceed = FALSE;
			
			
			if(proceed)
			{
		      
              if(invObj->m_armed)
			  {

                 break;
			  }
			}
		}
		indexArmed++;
	}
 
	if(!foundWeapon)//no weap in inventory
		return -1;
  
	int newGlid = -1;

	int startPoint = ((int)invList->GetCount()-indexArmed)-2;
	if(startPoint < 0)
		return -1;

    for(posLoc = invList->FindIndex(startPoint);posLoc != NULL;)
	{
        CInventoryObj * invObj = (CInventoryObj *)invList->GetPrev(posLoc);
		if(invObj->m_itemData->m_locationID == 12)
		{
		   BOOL proceed = TRUE;
            if(invObj->m_itemData->m_requiredSkillType > -1 && invObj->m_itemData->m_requiredSkillLevel > 1)
			{
		     if(skills)
			 {
		       if(!skills->EvaluateRequiredSkillLevel(invObj->m_itemData->m_requiredSkillType,invObj->m_itemData->m_requiredSkillLevel,m_skillDatabase))
                   proceed = FALSE;
			 }
		     else
		           proceed = FALSE;
			}

		     if(stats)
		      if(!stats->VerifyStatRequirement((float)invObj->m_itemData->m_requiredStr,2,invList))
			       proceed = FALSE;
		
	   	     if(stats)
		      if(!stats->VerifyStatRequirement((float)invObj->m_itemData->m_requiredDex,3,invList))
			       proceed = FALSE;

		     if(stats)
		      if(!stats->VerifyStatRequirement((float)invObj->m_itemData->m_requiredKnowledge,4,invList))
			       proceed = FALSE;
			
			if(proceed)
		       return invObj->m_itemData->m_globalID;
		}
	}

	int tracer = 0;
	for(posLoc = invList->GetTailPosition();posLoc != NULL;)
	{
        CInventoryObj * invObj = (CInventoryObj *)invList->GetPrev(posLoc);
		
		if(tracer == indexArmed)
		   return -1;
		if(invObj->m_itemData->m_locationID == 12)
		{
		   BOOL proceed = TRUE;
            if(invObj->m_itemData->m_requiredSkillType > -1 && invObj->m_itemData->m_requiredSkillLevel > 1)
			{
		     if(skills)
			 {
		       if(!skills->EvaluateRequiredSkillLevel(invObj->m_itemData->m_requiredSkillType,invObj->m_itemData->m_requiredSkillLevel,m_skillDatabase))
                   proceed = FALSE;
			 }
		     else
		           proceed = FALSE;
			}

		     if(stats)
		      if(!stats->VerifyStatRequirement((float)invObj->m_itemData->m_requiredStr,2,invList))
			       proceed = FALSE;
		
	   	     if(stats)
		      if(!stats->VerifyStatRequirement((float)invObj->m_itemData->m_requiredDex,3,invList))
			       proceed = FALSE;

		     if(stats)
		      if(!stats->VerifyStatRequirement((float)invObj->m_itemData->m_requiredKnowledge,4,invList))
			       proceed = FALSE;
			
			if(proceed)
		       return invObj->m_itemData->m_globalID;
		}
		tracer++;
	}
return -1;
}
//--------------Handles system messages-----------------------------------------------//
BOOL CSharedNet::HandleEquipMessageServerSide_safe(LPEQUIP_STRUCT formatedMessage,DPNID messageFromDPID)
{
      CPlayerObject * playerObjectData = m_netComObject->m_runtimePlayersOnServer->GetByNetID_LOS(formatedMessage->idRefrence);
	  if(!playerObjectData)
		  return FALSE;

      DWORD currentTickCount = GetTickCount();

      if(!playerObjectData->m_alive)
	  {
		  m_netComObject->m_runtimePlayersOnServer->Unlock();
		  return FALSE;
	  }

      int vIndex = 0;
	   if(formatedMessage->index < 0){
          if(formatedMessage->index != -9999)
             vIndex = abs(formatedMessage->index);
	   }
	   else
             vIndex = formatedMessage->index;

	   CInventoryObj * invenObj = (CInventoryObj *)playerObjectData->m_currentInventory->VerifyInventoryDUPH(vIndex,formatedMessage->dupCount);
	   if(invenObj)
	   {//inventory present
         if(invenObj->m_itemData->m_hp == 1)
		 {//cannot arm if broken
			SendTextMsg(messageFromDPID,invenObj->m_itemData->m_itemName + " is broken and cannot be armed!",playerObjectData);
			m_netComObject->m_runtimePlayersOnServer->Unlock();
			return FALSE;
		 }//end cannot arm if broken
		   
		 if(invenObj->m_itemData->m_locationID == 12)
		 {//weapon type
            if((currentTickCount-playerObjectData->m_weaponSwitchStamp) > 2000)
			{//1
                playerObjectData->m_weaponSwitchStamp = currentTickCount;
			}//end 1
			else
			{
				m_netComObject->m_runtimePlayersOnServer->Unlock();
				return FALSE;
			}
		 }//end weapon type

		 //If the item we are equiping is a fation based item
		 if(invenObj->m_itemData->m_useType == 11)
			 playerObjectData->m_factionStanding = invenObj->m_itemData->m_miscUseValue;

		 BOOL proceed = TRUE;
		 
		 ///////////////////////
		 // Update 03/27/2006 //
		 ///////////////////////
		 int strBonus = 0;
		 int dexBonus = 0;
		 int wisBonus = 0;

		 if(playerObjectData->m_bonus[1].m_bonusType == 1 || playerObjectData->m_bonus[1].m_bonusType == 4)
			 strBonus = playerObjectData->m_bonus[1].m_bonusAmount;
		 if(playerObjectData->m_bonus[1].m_bonusType == 2 || playerObjectData->m_bonus[1].m_bonusType == 4)
			 dexBonus = playerObjectData->m_bonus[1].m_bonusAmount;
		 if(playerObjectData->m_bonus[1].m_bonusType == 3 || playerObjectData->m_bonus[1].m_bonusType == 4)
			 wisBonus = playerObjectData->m_bonus[1].m_bonusAmount;

		 ////////////////
		 // End Update //
		 ////////////////

		 if(playerObjectData->m_stats)
		 {//valid stat DB
		     if(!playerObjectData->m_stats->VerifyStatRequirement((float)invenObj->m_itemData->m_requiredStr - strBonus,2,playerObjectData->m_currentInventory))
			 {//send info on what went wrong
			   SendDetailedMsgToClient(3,
									   invenObj->m_itemData->m_globalID,
									   0,
									   0,
									   playerObjectData);
			   proceed = FALSE;
			 }//end send info on what went wrong
			 if(proceed)
			 if(!playerObjectData->m_stats->VerifyStatRequirement((float)invenObj->m_itemData->m_requiredDex - dexBonus,3,playerObjectData->m_currentInventory))
			 {//send info on what went wrong
               SendDetailedMsgToClient(3,
									   invenObj->m_itemData->m_globalID,
									   0,
									   0,
									   playerObjectData);
			   proceed = FALSE;
			 }//end send info on what went wrong
			 if(proceed)
			 if(!playerObjectData->m_stats->VerifyStatRequirement((float)invenObj->m_itemData->m_requiredKnowledge - wisBonus,4,playerObjectData->m_currentInventory))
			 {//send info on what went wrong
			   SendDetailedMsgToClient(3,
									   invenObj->m_itemData->m_globalID,
									   0,
									   0,
									   playerObjectData);
			   proceed = FALSE;
			 }//end send info on what went wrong
			 if(proceed)
			 if(invenObj->m_itemData->m_requiredSkillType > -1 && invenObj->m_itemData->m_requiredSkillLevel > 1)
			 {//item requires a skill
                if(!playerObjectData->m_skills)
				{//skills not present
					CString skillName = "You lack the skill...";
                    //aquire the skill name for this
					CSkillObject * sklRefPtr = m_skillDatabase->GetSkillByType(invenObj->m_itemData->m_requiredSkillType);
                    if(sklRefPtr)
					{//known skill
					   skillName = operator+(skillName,sklRefPtr->m_skillName);
					SendTextMsg(DPNSEND_GUARANTEED,
						                             messageFromDPID,
						                             skillName,
							                         0,
													 playerObjectData,
													 0);//confermation
					}//end skill know skill
					else{//unknown skill
                       SendTextMsg(DPNSEND_GUARANTEED,
						                             messageFromDPID,
						                             skillName,
							                         0,
													 playerObjectData,
													 0);//confermation
					}//end unknown skill
                    proceed = FALSE;
				}//end skills not present
				else
				{//verify skills
					CSkillObject * sklRefPtr = playerObjectData->m_skills->GetSkillByType(invenObj->m_itemData->m_requiredSkillType);
                    if(sklRefPtr)
					{//have the skill
					   CString titleTemp;
					   int curLevel = 1;

                       CSkillObject * skillPtrRef = m_skillDatabase->GetSkillByType(sklRefPtr->m_skillType);
				       if(skillPtrRef)
					      sklRefPtr->EvaluateLevelAndTitle(titleTemp,curLevel,skillPtrRef);
					   if(curLevel < invenObj->m_itemData->m_requiredSkillLevel)
					   {
						  
						  CString skillName = "You must be at least Level "+m_cabLib.ConvToString(invenObj->m_itemData->m_requiredSkillLevel);
						  skillName = skillName+" ";
						  skillName = skillName+sklRefPtr->m_skillName;
						  skillName = skillName+" To Equip this...";
						  SendTextMsg(DPNSEND_GUARANTEED,
						                                   messageFromDPID,
						                                   skillName,
							                               0,
														   playerObjectData,
														   0);//confermation
					      proceed = FALSE;
					   }
					}//end have a skill
					else{//dont have the skill at all
						
					    CSkillObject * sklRefPtr = m_skillDatabase->GetSkillByType(invenObj->m_itemData->m_requiredSkillType);
                        if(sklRefPtr)
						{//known skill
						   CString skillName = "You must be at least Level "+m_cabLib.ConvToString(invenObj->m_itemData->m_requiredSkillLevel);
						   skillName = skillName+" ";
						   skillName = skillName+sklRefPtr->m_skillName;
						   skillName = skillName+" To Equip this...";
					       skillName = skillName+sklRefPtr->m_skillName;
					       SendTextMsg(DPNSEND_GUARANTEED,
						                             messageFromDPID,
						                             skillName,
							                         0,
													 playerObjectData,
													 0);//confermation
						   proceed = FALSE;
						}//end skill know skill
					    else{//unknown skill
						   CString skillName = "You lack the skill...";
                           SendTextMsg(DPNSEND_GUARANTEED,
						                             messageFromDPID,
						                             skillName,
							                         0,
													 playerObjectData,
													 0);//confermation
						   proceed = FALSE;
						}//end unknown skill
						proceed = FALSE;
					}//end dont have the skill at all
				}//end verify skills
			 }//end item require a skill

			 if(!m_itemManager->VerifyRaceCanArm_safe(playerObjectData->m_curEDBIndex,invenObj->m_itemData->m_globalID))
			 {//found DB object check if armable by this race
                SendGeneralMessageHandler(3,playerObjectData);
                proceed						= FALSE;
			 }//end found DB object check if armable by this race
		 }//end valid stat DB

		 if(proceed)
		 {//do inven arm
		  if(formatedMessage->index < 0)
		  {//disarming
		      if(invenObj->m_itemData->m_disarmable)//can disarm this
				  invenObj->m_armed = TRUE;

              else
			  {//cannot directly disarm this item
		         m_netComObject->m_runtimePlayersOnServer->Unlock();
                 return FALSE;
			  }//end cannot directly disarm this item
              //check for phy update requirement
              BOOL updatePhy = FALSE;
			  
			  if(invenObj->PhysicChanging())
			     updatePhy = TRUE;

			  CInventoryObj * unarmedWeapon = (CInventoryObj *)playerObjectData->m_currentInventory->GetUnarmedWeapon();

			  if(unarmedWeapon && invenObj->m_itemData->m_locationID == 12)
			  {
				  for(POSITION iPos = playerObjectData->m_currentInventory->GetHeadPosition();iPos != NULL;)
				  {
					  CInventoryObj * invenDBPtr = (CInventoryObj *)playerObjectData->m_currentInventory->GetNext(iPos);
					  if(!invenDBPtr->m_armed)
						  continue;
					 
					  if(invenDBPtr->m_itemData->m_locationID == unarmedWeapon->m_itemData->m_locationID)
					  {//disarm on server item will replace
						 invenDBPtr->m_armed = FALSE;
						 if(!updatePhy)
						 if(invenDBPtr->PhysicChanging())
							updatePhy = TRUE;

						  invenObj->m_armed = FALSE;
						  unarmedWeapon->m_armed = TRUE;
				  
						  if(unarmedWeapon->PhysicChanging())
							  updatePhy = TRUE;
						  if(updatePhy)
						  {
							  UpdatePhysicsBlock(playerObjectData);
							  updatePhy = FALSE;
						  }

						  BroadcastInventoryChange(playerObjectData,unarmedWeapon->m_itemData->m_globalID,messageFromDPID,TRUE,unarmedWeapon->m_itemData);
					  }//end disarm on server item will replace
				  }
			  }

			  else
			  {
				  if(invenObj->m_itemData->m_weaponDepletionTimer)
					  invenObj->m_itemData->m_secsToGo = invenObj->m_itemData->m_weaponDepletionTimer - (GetTickCount() - invenObj->m_itemData->m_tempDepletionHolder);
				  else
					  invenObj->m_itemData->m_secsToGo = 0;
				  invenObj->m_armed = FALSE;
				  BroadcastInventoryChange(playerObjectData,formatedMessage->index,messageFromDPID,TRUE,invenObj->m_itemData);
			  }
			  
			  if(updatePhy)
				  UpdatePhysicsBlock(playerObjectData);

			  // Send Attrib to client so it updates character screen
			  SendAttribMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,33,208,0,51,0,playerObjectData);

		  }//end disarming
		  else{//arming
              //find items that will be swaped
			  BOOL alreadyArmed = FALSE;
			  BOOL updatePhy = FALSE;
              for(POSITION iPos = playerObjectData->m_currentInventory->GetHeadPosition();iPos != NULL;)
			  {
                  CInventoryObj * invenDBPtr = (CInventoryObj *)playerObjectData->m_currentInventory->GetNext(iPos);
                  if(!invenDBPtr->m_armed)
				      continue;
				  
				  //if(invenDBPtr->m_armed)
				  //if(invenDBPtr->m_itemData->m_globalID == invenObj->m_itemData->m_globalID && invenDBPtr->m_itemData->m_locationID == invenObj->m_itemData->m_locationID)
                    // alreadyArmed = TRUE;
                  if(/*invenDBPtr->m_itemData->m_globalID != invenObj->m_itemData->m_globalID && */invenDBPtr->m_itemData->m_locationID == invenObj->m_itemData->m_locationID)
				  {//disarm on server item will replace
                     invenDBPtr->m_armed = FALSE;
					 if(invenDBPtr->m_itemData->m_weaponDepletionTimer)
					  invenDBPtr->m_itemData->m_secsToGo = invenDBPtr->m_itemData->m_weaponDepletionTimer - (GetTickCount() - invenDBPtr->m_itemData->m_tempDepletionHolder);
					 else
						 invenDBPtr->m_itemData->m_secsToGo = 0;
					 if(!updatePhy)
					 if(invenDBPtr->PhysicChanging())
					    updatePhy = TRUE;
				  }//end disarm on server item will replace
              }
		      invenObj->m_armed = TRUE;
			  if(invenObj->m_itemData->m_secsToGo)
			  {
				  invenObj->m_itemData->m_weaponDepletionTimer = invenObj->m_itemData->m_secsToGo;
				  invenObj->m_itemData->m_tempDepletionHolder = GetTickCount();
			  }

              if(invenObj->PhysicChanging())
                 updatePhy = TRUE;

			  if(updatePhy) 
			     UpdatePhysicsBlock(playerObjectData);

			  if(!alreadyArmed)
			  {
			      BroadcastInventoryChange(playerObjectData,formatedMessage->index,messageFromDPID,TRUE,invenObj->m_itemData);
				  
				  // Send Attrib to client so it updates character screen
				  SendAttribMsg(DPNSEND_GUARANTEED,playerObjectData->m_currentID,33,208,0,51,0,playerObjectData);
				  
			  }
		  }//end arming
		 }//end do inven arm
	   }//end inventory present
	   else{//attempted to arm weapon not in inventory
          //m_netComObject->m_eventLog->AddEvent("Attempted to arm item not in inventory:","",901);
	   }//end attempted to arm weapon not in inventory

	   UpdateClientStatistics(playerObjectData,messageFromDPID);//update armour or other bonuses
       m_netComObject->m_runtimePlayersOnServer->Unlock();
   return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::SendPlayerInventory(CInventoryObjList * inventory,
									 BOOL filterNonTranferable,
									 DPNID sendTo,
									 int visualID,
									 CPlayerObject * currentPlayer,
									 int includeArmed,
									 int filter)
{
	  if(!inventory)
	     return FALSE;
      int inventoryItemCount = (int)inventory->GetCount();
	  int totalAssets = (int)m_currencySystem->EvalCurrency(inventory);
	  SendAttribMsg(DPNSEND_GUARANTEED,
					currentPlayer->m_currentID,								
					19,												
					totalAssets,		
					0,				
					0,				
					0,												
					currentPlayer);


             if(inventoryItemCount == 0)
			 {//no inventory available
               return TRUE;
			 }//end no inventory available
		     else
			 {//send list
                //send inventory block
				BYTE * inventoryBlock = NULL;
				int blockItemCount = 0;
				int datasize = 0;
			    if(inventory->GenerateInventoryDataBlock(includeArmed,
												         &inventoryBlock,
												         blockItemCount,
														 datasize,
													     filterNonTranferable,
														 sizeof(BLOCKITEMLIST_STRUCT),
														 filter))
				{//valid send data
                    //BYTE * dataPacket = new BYTE[datasize];
					BLOCKITEMLIST_STRUCT header;
					header.m_catagorie = 20;
	                header.m_miscInt   = visualID;
                    CopyMemory(inventoryBlock,&header,sizeof(BLOCKITEMLIST_STRUCT));

					m_netComObject->SendByteBlockMsg(DPNSEND_GUARANTEED,
							                         sendTo,
								                     inventoryBlock,
									                 (datasize+4),
													 m_netComObject->m_packetTimeout);
				}//end valid send data
			 }//end send list
   return TRUE;
}
//--------------send message----------------------------------------------------------//
BOOL CSharedNet::SendSkillAbilities(DWORD flags,
							        DPNID sendTo,
						            CSkillObject * skillPtr,
									int levelToUse,
									int glid) 
{
    CLevelRangeObject * levelPtr = skillPtr->m_levelRangeDB->GetObjByIndex(levelToUse-1);
	if(!levelPtr)
		return FALSE;

	if(!levelPtr->m_abilityDB)
		return FALSE;

	if(levelPtr->m_abilityDB->GetCount()==0)
		return FALSE;

	int msgSize = sizeof(SKILLABILITIES_HEADER);
	int dataTrack = msgSize;
    SKILLABILITIES_HEADER header;
	header.m_catagorie    = 34;
	header.m_menuID       = levelPtr->m_menuID;
	header.m_glid         = glid;

    POSITION posLoc;
	for(posLoc = levelPtr->m_abilityDB->GetHeadPosition();posLoc != NULL;)
	{
        CSkillAbilityObject * aPtr = (CSkillAbilityObject *)levelPtr->m_abilityDB->GetNext(posLoc);
        msgSize++;
		msgSize += aPtr->m_abilityName.GetLength();
	}

	BYTE * data = new BYTE[msgSize];
	CopyMemory(data,&header,sizeof(SKILLABILITIES_HEADER));
		
	for(posLoc = levelPtr->m_abilityDB->GetHeadPosition();posLoc != NULL;)
	{
        CSkillAbilityObject * aPtr = (CSkillAbilityObject *)levelPtr->m_abilityDB->GetNext(posLoc);
        int textLength = aPtr->m_abilityName.GetLength();
		data[dataTrack] = textLength;
		dataTrack++;
		for(int loop=0;loop<textLength;loop++)
		{
		   data[dataTrack] = aPtr->m_abilityName.GetAt(loop);
		   dataTrack++;
		}	
	}
	
	m_netComObject->SendData(data,
				                  msgSize,
				                  flags,
					              sendTo,
								  m_netComObject->m_packetTimeout);

	if (data){
		delete [] data;
		data = 0;
	}
	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::HandleBounty(CPlayerObject * requestedData,CPlayerObject * attackerData)
{
	if(!attackerData->m_aiControlled)
       if(requestedData->m_playerOrigRef)
	     if(attackerData->m_playerOrigRef)
	       if(requestedData->m_handle != attackerData->m_handle)
	          if(requestedData->m_clanHandle != attackerData->m_clanHandle || attackerData->m_clanHandle == -1)
	             if(requestedData->m_playerOrigRef->m_currentBounty > 0) 
	               if(m_netComObject->m_bountySystem)
				   {//handle bounty shit
					  CString msgToBountyHunter = "You have been awarded (In your bank) $" + m_cabLib.ConvToString(requestedData->m_playerOrigRef->m_currentBounty);
					  msgToBountyHunter = msgToBountyHunter +" for "+requestedData->m_charName + "'s bounty.";
					  SendTextMsg(DPNSEND_GUARANTEED,
													   attackerData->m_currentID,
													   msgToBountyHunter,
													   0,
													   attackerData,
													   0);
					  //if(attackerData->m_playerOrigRef->m_bankCash)
						// attackerData->m_playerOrigRef->m_bankCash->m_amount += requestedData->m_playerOrigRef->m_currentBounty;
					  
					  requestedData->m_playerOrigRef->m_currentBounty=0;
					  m_netComObject->m_bountySystem->RemoveBountyByHandle(requestedData->m_handle);
					  return TRUE;
				   }//end handle bounty shit
      return FALSE;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::HandleArenaPoints(CPlayerObject * requestedData,CPlayerObject * attackerData)
{
          if(m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(requestedData->m_stencilChannel,requestedData->m_currentWorldID,WORLDRULE_ARENA_ADDPOINTSPERKILL))
          {//in arena world apply points
             if(requestedData->m_handle == attackerData->m_handle)
			 {
				 attackerData->m_arenaBasePoints--;
				 if(attackerData->m_arenaBasePoints < 0)
				    attackerData->m_arenaBasePoints = 0;
			 }
			 else
			 {
				 attackerData->m_arenaBasePoints+=2;
				 attackerData->m_arenaTotalKills++;
				 requestedData->m_arenaBasePoints--;
				 if(requestedData->m_arenaBasePoints < 0)
				    requestedData->m_arenaBasePoints = 0;

				 CString build = "You have killed " + requestedData->m_charName + " You have scored an Arena point";
                 SendTextMsg(DPNSEND_GUARANTEED,
						                         attackerData->m_currentID,
						                         build,
								                 0,
												 attackerData,
												 0);
			 }
			 return TRUE;
          }//end in arena world apply points
  return FALSE;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::NotifyWhatKilledPC(CPlayerObject * requestedData,CPlayerObject * attackerData)
{
    if(!requestedData->m_aiControlled)
	 if(attackerData->m_aiControlled)
	 {//NPC killer
		if(requestedData->m_currentID != 0)
		{
			CString build = "YOU HAVE BEEN KILLED BY A ";
			build = operator+(build,attackerData->m_charName);
			SendTextMsg( DPNSEND_GUARANTEED,
                         requestedData->m_currentID,
                         build,
		                 0,
						 requestedData,
						 3);
	        return TRUE;
		}
	 }//end NPC killer
	 return FALSE;
}

//------------------------------------------------------------------------------------//
BOOL CSharedNet::HandleArenaRulesOnDeath(int & stencilChannelStore,int & channelStore)
{
		// MOST WANTED vars
		int					iMostKills = 0;
		CPlayerObject		*pOldMostWanted = NULL,
							*pNewMostWanted = NULL,
							*pSoleSurvivor = NULL,
							*plObj = NULL,
							*pWinningBettor = NULL;
		CWorldAvailableObj	*pWorld = NULL;
		ARENA_MESSAGE		msg;

		//check if ffa/team arena
		if(m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(stencilChannelStore,channelStore,WORLDRULE_TEAM_ELIMINATION))
		{//see if all of one clan is left
			CAxList * rPlList = this->m_netComObject->m_runtimePlayersOnServer->GetListByChannel(channelStore);
			BOOL closeChannel = TRUE;
			short sameID = -1;
			int trace = 0;

			for(CAxLinkObj * pos = rPlList->GetHeadPosition_unsafe();pos != NULL;)
			{
				plObj = (CPlayerObject *)rPlList->GetNext_unsafe(&pos);

				if((plObj->m_currentWorldID != channelStore)
				|| (plObj->m_energy <= 0)
				|| (plObj->m_aiControlled)) 
				{
					plObj = NULL;
					continue;
				}

				if(sameID == -1)
					sameID = plObj->m_teamTemp;

				if(plObj->m_teamTemp != sameID)
					closeChannel = FALSE;

				// Check for a pre-existing MOST WANTED player
				if(plObj->m_bMostWanted) 
					pOldMostWanted = plObj;

				// Check for current MOST WANTED status
				if(plObj->m_arenaInfo && plObj->m_arenaInfo[2] > iMostKills)
				{
					iMostKills = plObj->m_arenaInfo[2];
					pNewMostWanted = plObj;
				}

				pSoleSurvivor = plObj;
				trace++;
			}

			// Handle MOST WANTED network message(s)
			if(pNewMostWanted)
			{
				// Let new MOST WANTED player know of their new status
				pNewMostWanted->m_bMostWanted = true;
				msg.ArenaMessage = ARENA_SET_MOST_WANTED;
				msg.ArenaType = pNewMostWanted->m_arenaInfo[0];
				msg.Score = pNewMostWanted->m_arenaInfo[1];
				msg.Kills = pNewMostWanted->m_arenaInfo[2];
				msg.Deaths = pNewMostWanted->m_arenaInfo[3];
				msg.Damage = pNewMostWanted->m_arenaInfo[4];
				this->SendArenaMessage(pNewMostWanted, msg);
			}

			if(pOldMostWanted && (pOldMostWanted != pNewMostWanted))
			{
				// Let old MOST WANTED player know they've been displaced
				pOldMostWanted->m_bMostWanted = false;
				msg.ArenaMessage = ARENA_REMOVE_MOST_WANTED;
				msg.ArenaType = pOldMostWanted->m_arenaInfo[0];
				msg.Score = pOldMostWanted->m_arenaInfo[1];
				msg.Kills = pOldMostWanted->m_arenaInfo[2];
				msg.Deaths = pOldMostWanted->m_arenaInfo[3];
				msg.Damage = pOldMostWanted->m_arenaInfo[4];
				this->SendArenaMessage(pOldMostWanted, msg);
			}

			if(closeChannel || trace == 1)
			{//async finalize processing
				// FIND winning bettor
				CWorldAvailableObj *pWorld = NULL;
				for(CAxLinkObj *position = rPlList->GetHeadPosition_unsafe(); position != NULL;)
				{
					//Typecast to each player object
					CPlayerObject *plObj = (CPlayerObject *)rPlList->GetNext_unsafe(&position);
					if(!plObj
					|| (plObj->m_energy <= 0)
					|| (plObj->m_aiControlled))
						continue;

					// FIND winning arena bettor out of all the betting players on the winning team...
					pWorld = this->m_netComObject->m_currentWorldsUsed->GetWorldByChannel(plObj->m_currentWorldID);
					if (plObj->m_arenaBetAmount > 0)
					{
						if (!pWinningBettor || (pWorld->m_numArenaBettors == 1))
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

				// Only players left should be victors if we're here...
				for(CAxLinkObj * pos = rPlList->GetHeadPosition_unsafe(); pos != NULL;)
				{
					plObj = (CPlayerObject*)rPlList->GetNext_unsafe(&pos);
					if((plObj->m_currentWorldID != channelStore)
					|| (plObj->m_energy <= 0)
					|| (plObj->m_aiControlled)) 
					{
						plObj = NULL;
						continue;
					}

					if(plObj)
					{
						// If there is only one player left at victory point...
						if(trace == 1)
						{
							// Send misc packet to inform him of SOLE_SURVIVOR status
							msg.ArenaMessage = ARENA_SOLE_SURVIVOR;
							msg.ArenaType = plObj->m_arenaInfo[0];
							msg.Score = plObj->m_arenaInfo[1];
							msg.Kills = plObj->m_arenaInfo[2];
							msg.Deaths = plObj->m_arenaInfo[3];
							msg.Damage = plObj->m_arenaInfo[4];
							this->SendArenaMessage(plObj, msg);
						}

						// Send misc packet to inform of ARENA VICTORY
						pWorld = this->m_netComObject->m_currentWorldsUsed->GetWorldByChannel(plObj->m_currentWorldID);
						msg.ArenaMessage = ARENA_VICTORY;
						msg.ArenaType = plObj->m_arenaInfo[0];
						msg.Score = plObj->m_arenaInfo[1];
						msg.Kills = plObj->m_arenaInfo[2];
						msg.Deaths = plObj->m_arenaInfo[3];
						msg.Damage = plObj->m_arenaInfo[4];
						if (pWorld && plObj->m_arenaBetAmount > 0)
						{
							pWinningBettor = plObj;
							msg.Reward = pWorld->m_arenaBetPool;
						}
						else
							msg.Reward = 0;
						this->SendArenaMessage(plObj, msg);
						plObj->m_arenaRespawnPosition.x = 0;
						plObj->m_arenaRespawnPosition.y = 0;
						plObj->m_arenaRespawnPosition.z = 0;
						plObj->m_arenaRespawnWorldName = "Unknown";
					}
				}

				m_threadedDataProcessor->AddToQue(channelStore,stencilChannelStore,36,0);
				m_threadedDataProcessor->AddToQue(channelStore,stencilChannelStore,31,1000);//process closing rules
				m_threadedDataProcessor->AddToQue(channelStore,0,30,10000);//close zone in 10 seconds
				//send close instance msg
			}
		}//end see if all of one clan is left
		else if(m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(stencilChannelStore,channelStore,WORLDRULE_ELIMINATION))
		{//see if theres only one person left
			int trace = 0;

			CAxList * rPlList = this->m_netComObject->m_runtimePlayersOnServer->GetListByChannel(channelStore);
			for(CAxLinkObj * pos = rPlList->GetHeadPosition_unsafe();pos != NULL;)
			{
				plObj = (CPlayerObject *)rPlList->GetNext_unsafe(&pos);
				if((plObj->m_energy <= 0) || (plObj->m_aiControlled))
				{
					plObj = NULL;
					continue;
				}

				// Check for a pre-existing MOST WANTED player
				if(plObj->m_bMostWanted) 
					pOldMostWanted = plObj;

				// Check for current MOST WANTED status
				if(!pNewMostWanted ||
				(plObj->m_arenaInfo && plObj->m_arenaInfo[2] > iMostKills))
				{
					iMostKills = plObj->m_arenaInfo[2];
					pNewMostWanted = plObj;
				}

				pSoleSurvivor = plObj;
				trace++;
			}

			// Handle MOST WANTED network message(s)
			if(pNewMostWanted)
			{
				// Let new MOST WANTED player know of their new status
				pNewMostWanted->m_bMostWanted = true;
				msg.ArenaMessage = ARENA_SET_MOST_WANTED;
				msg.ArenaType = pNewMostWanted->m_arenaInfo[0];
				msg.Score = pNewMostWanted->m_arenaInfo[1];
				msg.Kills = pNewMostWanted->m_arenaInfo[2];
				msg.Deaths = pNewMostWanted->m_arenaInfo[3];
				msg.Damage = pNewMostWanted->m_arenaInfo[4];
				this->SendArenaMessage(pNewMostWanted, msg);
			}

			if(pOldMostWanted && (pOldMostWanted != pNewMostWanted))
			{
				// Let old MOST WANTED player know they've been displaced
				pOldMostWanted->m_bMostWanted = false;
				msg.ArenaMessage = ARENA_REMOVE_MOST_WANTED;
				msg.ArenaType = pOldMostWanted->m_arenaInfo[0];
				msg.Score = pOldMostWanted->m_arenaInfo[1];
				msg.Kills = pOldMostWanted->m_arenaInfo[2];
				msg.Deaths = pOldMostWanted->m_arenaInfo[3];
				msg.Damage = pOldMostWanted->m_arenaInfo[4];
				this->SendArenaMessage(pOldMostWanted, msg);
			}

			if(trace == 1)
			{//async finalize processing
				if(pSoleSurvivor)
				{
					// Send misc packet to inform of ARENA VICTORY status
					pWorld = this->m_netComObject->m_currentWorldsUsed->GetWorldByChannel(pSoleSurvivor->m_currentWorldID);
					msg.ArenaMessage = ARENA_VICTORY;
					msg.ArenaType = pSoleSurvivor->m_arenaInfo[0];
					msg.Score = pSoleSurvivor->m_arenaInfo[1];
					msg.Kills = pSoleSurvivor->m_arenaInfo[2];
					msg.Deaths = pSoleSurvivor->m_arenaInfo[3];
					msg.Damage = pSoleSurvivor->m_arenaInfo[4];
					if (pWorld && pSoleSurvivor->m_arenaBetAmount > 0)
					{
						pWinningBettor = pSoleSurvivor;
						msg.Reward = pWorld->m_arenaBetPool;
					}
					else
						msg.Reward = 0;
					this->SendArenaMessage(pSoleSurvivor, msg);

					// Send misc packet to inform of SOLE_SURVIVOR status
					msg.ArenaMessage = ARENA_SOLE_SURVIVOR;
					msg.ArenaType = pSoleSurvivor->m_arenaInfo[0];
					msg.Score = pSoleSurvivor->m_arenaInfo[1];
					msg.Kills = pSoleSurvivor->m_arenaInfo[2];
					msg.Deaths = pSoleSurvivor->m_arenaInfo[3];
					msg.Damage = pSoleSurvivor->m_arenaInfo[4];
					this->SendArenaMessage(pSoleSurvivor, msg);
					pSoleSurvivor->m_arenaRespawnPosition.x = 0;
					pSoleSurvivor->m_arenaRespawnPosition.y = 0;
					pSoleSurvivor->m_arenaRespawnPosition.z = 0;
					pSoleSurvivor->m_arenaRespawnWorldName = "Unknown";

				}

				m_threadedDataProcessor->AddToQue(channelStore,stencilChannelStore,36,0);
				m_threadedDataProcessor->AddToQue(channelStore,stencilChannelStore,31,1000);//process closing rules
				m_threadedDataProcessor->AddToQue(channelStore,0,30,10000);//close zone in 10 seconds
			}
		}//end see if theres only one person left
		else if(m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(stencilChannelStore,channelStore,WORLDRULE_ELIMINATEMOBS_ARENA))
		{//see if mobs are eliminated
          CAxList * rPlList = this->m_netComObject->m_runtimePlayersOnServer->GetListByChannel(channelStore);
		  int trace = 0;
		  for(CAxLinkObj * pos = rPlList->GetHeadPosition_unsafe();pos != NULL;)
		  {
             CPlayerObject * plObj = (CPlayerObject *)rPlList->GetNext_unsafe(&pos);
             if(plObj->m_energy <= 0)
			    continue;

			 if(plObj->m_aiControlled)
			 {
			    trace++;
			    break;
			 }
		  }
		  if(trace == 0)
		  {//async finalize processing
		     m_threadedDataProcessor->AddToQue(channelStore,stencilChannelStore,31,0);//process closing rules
		     m_threadedDataProcessor->AddToQue(channelStore,0,30,10000);//close zone in 10 seconds
		  }
		}//end see if mobs are eliminated
		DWORD dwParam = 0;
		if(m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(stencilChannelStore,channelStore,WORLDRULE_DEATHMATCH,&dwParam))
		{//check for deathmatch
			//Acquire the list of players in the channel
			CAxList *plList = this->m_netComObject->m_runtimePlayersOnServer->GetListByChannel(channelStore);
			BOOL winner = FALSE;
			if(dwParam)
			{
				//Go through the list to see if we have found a winner
				for(CAxLinkObj *pos = plList->GetHeadPosition_unsafe(); pos != NULL;)
				{
					CPlayerObject *plObj = (CPlayerObject *)plList->GetNext_unsafe(&pos);
					if(dwParam)
					{
						if(!plObj->m_aiControlled)
						{
							if(plObj->m_arenaInfo[2] >= (int)dwParam)
							{	
								//Winner has been found, no need to continue on
								//	UPDATE: No BREAK in order to correctly handle arena betting logic
								winner = TRUE;
								//break;
							}

							// HANDLE betting logic
							pWorld = this->m_netComObject->m_currentWorldsUsed->GetWorldByChannel(plObj->m_currentWorldID);
							if (pWorld && plObj->m_arenaBetAmount > 0)
							{
								if ((pWorld->m_numArenaBettors == 1) || winner)
								{
									pWinningBettor = plObj;
									plObj->m_arenaInfo[5] = pWorld->m_arenaBetPool;
								}
								else
									plObj->m_arenaInfo[5] = -1 * plObj->m_arenaBetAmount;
							}
						}
					}
				}
				//If we have a winner, time to end the arena
				if(winner)
				{
					//Loop through all the players in the channel again
					for(CAxLinkObj *position = plList->GetHeadPosition_unsafe(); position != NULL;)
					{
						//Acquire each player object
						CPlayerObject *plObj = (CPlayerObject *)plList->GetNext_unsafe(&position);
						ARENA_MESSAGE msg;
						if(!plObj->m_aiControlled) //Make sure this player is not an AI unit to account for
						{
							//Victory Condition Met
							if(plObj->m_arenaInfo[2] >= (int)dwParam)
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
							this->SendArenaMessage(plObj, msg);

							// RESET player's respawn position
							plObj->m_arenaRespawnPosition.x = 0;
							plObj->m_arenaRespawnPosition.y = 0;
							plObj->m_arenaRespawnPosition.z = 0;
							plObj->m_arenaRespawnWorldName = "Unknown";
						}
					}
					//Close down the arena with appropriate rules
					m_threadedDataProcessor->AddToQue(channelStore,stencilChannelStore,36,0);
					m_threadedDataProcessor->AddToQue(channelStore,stencilChannelStore,32,1000);//process closing rules
					m_threadedDataProcessor->AddToQue(channelStore,0,30,10000);//close zone in 10 seconds
				}
			}
		}//end check for deathmatch
		else if(m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(stencilChannelStore,channelStore,WORLDRULE_DOMINATION,&dwParam))
		{//check for domination type arena
			//Acquire the list of players in the channel
			CAxList *plList = this->m_netComObject->m_runtimePlayersOnServer->GetListByChannel(channelStore);
			BOOL winner = FALSE;
			if(!dwParam)
				dwParam = 20;
			//Go through the list to see if we have found a winner
			for(CAxLinkObj *pos = plList->GetHeadPosition_unsafe(); pos != NULL;)
			{
				CPlayerObject *plObj = (CPlayerObject *)plList->GetNext_unsafe(&pos);
				if(!plObj->m_aiControlled)
				{
					int playerScore = 0;
					playerScore = plObj->m_arenaInfo[2] - plObj->m_arenaInfo[3];
					if(playerScore >= (int)dwParam)
					{	
						//Winner has been found
						winner = TRUE;
						plObj->m_arenaInfo[1] = 1; //Setting to one for easy check

						pWorld = this->m_netComObject->m_currentWorldsUsed->GetWorldByChannel(plObj->m_currentWorldID);
						if (pWorld && (plObj->m_arenaBetAmount > 0))
						{
							if (winner || (pWorld->m_numArenaBettors == 1))
							{
								pWinningBettor = plObj;
								plObj->m_arenaInfo[5] = pWorld->m_arenaBetPool;
							}
							else
								plObj->m_arenaInfo[5] = -1 * plObj->m_arenaBetAmount;
						}
						else plObj->m_arenaInfo[5] = 0;

						// no need to continue on
						//	UPDATE: We do have to continue now due to arena betting logic
						// break;
					}
				}
			}

			//If we have a winner, time to end the arena
			if(winner)
			{
				//Loop through all the players in the channel again
				for(CAxLinkObj *position = plList->GetHeadPosition_unsafe(); position != NULL;)
				{
					//Acquire each player object
					CPlayerObject *plObj = (CPlayerObject *)plList->GetNext_unsafe(&position);
					ARENA_MESSAGE msg;
					if(!plObj->m_aiControlled) //Make sure this player is not an AI unit to account for
					{
						//Victory Condition Met
						if(plObj->m_arenaInfo[1])
							msg.ArenaMessage = ARENA_VICTORY;
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
						this->SendArenaMessage(plObj, msg);

						// RESET player's respawn position
						plObj->m_arenaRespawnPosition.x = 0;
						plObj->m_arenaRespawnPosition.y = 0;
						plObj->m_arenaRespawnPosition.z = 0;
						plObj->m_arenaRespawnWorldName = "Unknown";
					}
				}
				//Close down the arena with appropriate rules
				m_threadedDataProcessor->AddToQue(channelStore,stencilChannelStore,36,0);
				m_threadedDataProcessor->AddToQue(channelStore,stencilChannelStore,32,1000);//process closing rules
				m_threadedDataProcessor->AddToQue(channelStore,0,30,10000);//close zone in 10 seconds
			}
		}//end check for domination type arena
		if(m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(stencilChannelStore,channelStore,WORLDRULE_TEAM_DEATHMATCH))
		{//check for team based deathmatch arena
			//Acquire the list of players in the channel
			CAxList *plList = this->m_netComObject->m_runtimePlayersOnServer->GetListByChannel(channelStore);
			int teamOneID = 0, teamOneScore = 0;
			int teamTwoID = 0, teamTwoScore = 0;
			int winningTeam = 0;
			//Go through the list to see if the vicotry condition has been met
			m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(stencilChannelStore,channelStore,WORLDRULE_SCORE_LIMIT, &dwParam);
			if(dwParam)
			{
				for(CAxLinkObj *pos = plList->GetHeadPosition_unsafe(); pos != NULL;)
				{
					//Typecast to each player object
					CPlayerObject *plObj = (CPlayerObject *)plList->GetNext_unsafe(&pos);

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
					{
						teamOneScore += plObj->m_arenaInfo[2];
						if(teamOneScore >= (int)dwParam)
						{
							winningTeam = teamOneID;
							break;
						}
					}
					else if(teamTwoID == plObj->m_teamTemp)
					{
						teamTwoScore += plObj->m_arenaInfo[2];
						if(teamTwoScore >= (int)dwParam)
						{
							winningTeam = teamTwoID;
							break;
						}
					}
				}
			}

			//Check for the winning team
			if(winningTeam)
			{
				// FIND winning bettor
				CWorldAvailableObj *pWorld = NULL;
				for(CAxLinkObj *position = plList->GetHeadPosition_unsafe(); position != NULL;)
				{
					//Typecast to each player object
					CPlayerObject *plObj = (CPlayerObject *)plList->GetNext_unsafe(&position);
					if(!plObj
					|| (plObj->m_energy <= 0)
					|| (plObj->m_aiControlled))
						continue;

					// FIND winning arena bettor out of all the betting players on the winning team...
					pWorld = this->m_netComObject->m_currentWorldsUsed->GetWorldByChannel(plObj->m_currentWorldID);
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

				for(CAxLinkObj *position = plList->GetHeadPosition_unsafe(); position != NULL;)
				{
					//Acquire each player object
					CPlayerObject *plObj = (CPlayerObject *)plList->GetNext_unsafe(&position);
					ARENA_MESSAGE msg;
					if(!plObj->m_aiControlled) //Make sure this player is not an AI unit to account for
					{
						//Victory Condition Met
						pWorld = this->m_netComObject->m_currentWorldsUsed->GetWorldByChannel(plObj->m_currentWorldID);
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
						this->SendArenaMessage(plObj, msg);

						// RESET player's respawn position
						plObj->m_arenaRespawnPosition.x = 0;
						plObj->m_arenaRespawnPosition.y = 0;
						plObj->m_arenaRespawnPosition.z = 0;
						plObj->m_arenaRespawnWorldName = "Unknown";
					}
				}
				//Close down the arena with appropriate rules
				m_threadedDataProcessor->AddToQue(channelStore,stencilChannelStore,36,0);
				m_threadedDataProcessor->AddToQue(channelStore,stencilChannelStore,32,1000);//process closing rules
				m_threadedDataProcessor->AddToQue(channelStore,0,30,10000);//close zone in 10 seconds
			}
		}//end check for team based deathmatch arena
		else if(m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(stencilChannelStore,channelStore,WORLDRULE_TEAM_DOMINATION))
		{//check for team based domination arena
			//Acquire the list of players in the channel
			CAxList *plList = this->m_netComObject->m_runtimePlayersOnServer->GetListByChannel(channelStore);
			int teamOneID = 0, teamOneScore = 0;
			int teamTwoID = 0, teamTwoScore = 0;
			int winningTeam = 0;
			m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(stencilChannelStore,channelStore,WORLDRULE_SCORE_LIMIT, &dwParam);
			if(!dwParam)
				dwParam = 20;
			//Go through the list to see if the vicotry condition has been met
			for(CAxLinkObj *pos = plList->GetHeadPosition_unsafe(); pos != NULL;)
			{
				//Typecast to each player object
				CPlayerObject *plObj = (CPlayerObject *)plList->GetNext_unsafe(&pos);
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
				{
					teamOneScore += plObj->m_arenaInfo[2];
					teamOneScore -= plObj->m_arenaInfo[3];
				}
				else if(teamTwoID == plObj->m_teamTemp)
				{
					teamTwoScore += plObj->m_arenaInfo[2];
					teamTwoScore -= plObj->m_arenaInfo[3];
				}
			}

			if(teamOneScore >= (int)dwParam)
				winningTeam = teamOneID;
			if(teamTwoScore > teamOneScore && teamTwoScore >= (int)dwParam)
				winningTeam = teamTwoID;
			if(teamTwoScore == teamOneScore)
				winningTeam = 0; //No Ties, keep playing

			//Check for the winning team
			if(winningTeam)
			{
				// FIND winning bettor
				CWorldAvailableObj *pWorld = NULL;
				for(CAxLinkObj *position = plList->GetHeadPosition_unsafe(); position != NULL;)
				{
					//Typecast to each player object
					CPlayerObject *plObj = (CPlayerObject *)plList->GetNext_unsafe(&position);
					if(!plObj
					|| (plObj->m_energy <= 0)
					|| (plObj->m_aiControlled))
						continue;

					// FIND winning arena bettor out of all the betting players on the winning team...
					pWorld = this->m_netComObject->m_currentWorldsUsed->GetWorldByChannel(plObj->m_currentWorldID);
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

				for(CAxLinkObj *position = plList->GetHeadPosition_unsafe(); position != NULL;)
				{
					//Acquire each player object
					CPlayerObject *plObj = (CPlayerObject *)plList->GetNext_unsafe(&position);
					ARENA_MESSAGE msg;
					if(!plObj->m_aiControlled) //Make sure this player is not an AI unit to account for
					{
						//Victory Condition Met
						pWorld = this->m_netComObject->m_currentWorldsUsed->GetWorldByChannel(plObj->m_currentWorldID);
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
						this->SendArenaMessage(plObj, msg);

						// RESET player's respawn position
						plObj->m_arenaRespawnPosition.x = 0;
						plObj->m_arenaRespawnPosition.y = 0;
						plObj->m_arenaRespawnPosition.z = 0;
						plObj->m_arenaRespawnWorldName = "Unknown";
					}
				}
				//Close down the arena with appropriate rules
				m_threadedDataProcessor->AddToQue(channelStore,stencilChannelStore,36,0);
				m_threadedDataProcessor->AddToQue(channelStore,stencilChannelStore,32,1000);//process closing rules
				m_threadedDataProcessor->AddToQue(channelStore,0,30,10000);//close zone in 10 seconds
			}
		}//end check for team based deathmatch arena

		// HANDLE arena bet reward if the arena is over
		if (pWinningBettor)
		{
			pWorld = this->m_netComObject->m_currentWorldsUsed->GetWorldByChannel(pWinningBettor->m_currentWorldID);
			if (pWorld)
			{
				this->m_currencySystem->AddCurrency(
				  this->m_itemManager,
				  pWinningBettor->m_currentInventory,
				  -1,
				  pWorld->m_arenaBetPool);
			}
		}
		return TRUE;
}

//------------------------------------------------------------------------------------//
BOOL CSharedNet::HandleDeath(CPlayerObject * requestedData,
							 CPlayerObject * attackerData)
{
	if(!requestedData)
		return FALSE;
	if(!attackerData)//e.g. killed by a DoT whose caster has logged off - treat as self kill (was a server crash)
		attackerData = requestedData;
	int stencilChannelStore = requestedData->m_stencilChannel;
	int channelStore = requestedData->m_currentWorldID;
	BOOL specialNonDropZone = m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(requestedData->m_stencilChannel,requestedData->m_currentWorldID,WORLDRULE_NODROP);
	
	if(requestedData->m_currentHighestLevel < m_netComObject->m_newbieLevel)
		specialNonDropZone = TRUE;

	if(requestedData->m_alive)//still alive
		if(requestedData->m_energy < 1)
		{//player has just died
	   
			HandleBounty(requestedData,attackerData);
			HandleDeathQuestJournalStuff(requestedData,attackerData);
			HandleArenaPoints(requestedData,attackerData);
			NotifyWhatKilledPC(requestedData,attackerData);

			//Arena Tracking Info
			if(/*requestedData->m_inArena*/requestedData->m_arenaInfo && requestedData->m_arenaInfo[0])
			{
				requestedData->m_arenaInfo[3]++;
				if(!attackerData->m_aiControlled && !requestedData->m_aiControlled)
				{
					if(attackerData->m_currentID != requestedData->m_currentID)
					{//Error check to make sure we didn't kill ourselves
						attackerData->m_arenaInfo[2]++;
						//Update ARENA Kill count if needed for web display
						attackerData->m_arenaKills++;
					}//End Error check to make sure we didn't kill ourselves
				}
			}
			//Update PVP Kill count if needed for web display
			if(!attackerData->m_aiControlled && !requestedData->m_aiControlled && requestedData->m_arenaInfo && !requestedData->m_arenaInfo[0])
				if(attackerData->m_currentID != requestedData->m_currentID)
					attackerData->m_pvpKills++;

			if(requestedData->m_aiCfgOfMounted != -1)
		      HandleDeMountRequest(requestedData);//twice will dismount you

			//stamp for max move when toast
			requestedData->m_movementTrackTimeStamp  = GetTickCount();
			requestedData->m_movementTrackPosStamp = requestedData->m_Target3dPosition;

			if(attackerData)
			{//valid attacker
				if(attackerData->m_aiControlled)
				{//attacker is AI inform him of info
						SendAttribMsg(  DPNSEND_GUARANTEED,
										attackerData->m_currentID,
										12,
										0,
										attackerData->m_networkAssignedID,
										0,
										0,
										attackerData);//sets aggressor info to ai server
				}//end attacker is AI inform him of info
			}//end valid attacker
	 
			CMovementObj * entObjDB = m_entObjList->GetObjByIndex(requestedData->m_curEDBIndex);
			if(entObjDB)
			{//force death animation
				requestedData->m_curAnim = entObjDB->List_skeletalObject->GetAnimationIndexFromAttrib(8);
				requestedData->m_secondaryAnim = -1;
			}//end force death animation
                 
			if(!requestedData->m_aiControlled)
			{//1
						requestedData->m_deathCount++;

						SendAttribMsg(  DPNSEND_GUARANTEED,
										requestedData->m_currentID,
										27,//attribute
										0,//
										0,
										100,
										0,
										requestedData);
						requestedData->m_rechargeTimer = 0;
						/*SendAttribMsg(  DPNSEND_GUARANTEED,
										requestedData->m_currentID,
										41,//attribute
										0,//
										0,
										0,
										0,
										requestedData);*/

						if(requestedData->m_currentHighestLevel < m_netComObject->m_newbieLevel)//inform newbie that he isnt losing shit
							SendGeneralMessageHandler(86,requestedData);
						
						CString build = "YOU HAVE BEEN KILLED BY " + attackerData->m_charName; 
				/*		if(requestedData->m_deathCount == 1)
							build += " TIME.";
						else
							build += " TIMES.";
				*/
						SendTextMsg(DPNSEND_GUARANTEED,requestedData->m_currentID,build,0,requestedData,3); // Send a text msg with current death count
				
						if(!OnDeathDataProcessByDPNID(	requestedData,
														specialNonDropZone,
														entObjDB,
														m_respawnHealthPercentage,
														m_serverScripts,
														m_netComObject))
						{
							m_netComObject->m_eventLog->AddEvent("***OnDeathDataProcessByDPNID() could not locate account-char name:"+requestedData->m_charName);
						}

						// If player is in a valid arena, 
						// send misc packet to inform of ARENA DEFEAT...
						if((requestedData->m_arenaInfo[0] == WORLDRULE_TEAM_ELIMINATION)
						|| (requestedData->m_arenaInfo[0] == WORLDRULE_ELIMINATION))
						{
							ARENA_MESSAGE msg;
							msg.ArenaMessage = ARENA_DEFEAT;
							msg.ArenaType = requestedData->m_arenaInfo[0];
							msg.Score = requestedData->m_arenaInfo[1];
							msg.Kills = requestedData->m_arenaInfo[2];
							msg.Deaths = requestedData->m_arenaInfo[3];
							msg.Damage = requestedData->m_arenaInfo[4];
							if (requestedData->m_arenaBetAmount > 0)
								msg.Reward = -1 * requestedData->m_arenaBetAmount;
							else 
								msg.Reward = 0;
							this->SendArenaMessage(requestedData, msg);

							msg.ArenaMessage = ARENA_KILLING_BLOW;
							msg.ArenaType = attackerData->m_arenaInfo[0];
							msg.Score = attackerData->m_arenaInfo[1];
							msg.Kills = attackerData->m_arenaInfo[2];
							msg.Deaths = attackerData->m_arenaInfo[3];
							msg.Damage = attackerData->m_arenaInfo[4];
							this->SendArenaMessage(attackerData, msg);
						}
						else if((requestedData->m_arenaInfo[0] == WORLDRULE_DEATHMATCH)
						|| (requestedData->m_arenaInfo[0] == WORLDRULE_TEAM_DEATHMATCH)
						|| (requestedData->m_arenaInfo[0] == WORLDRULE_DOMINATION)
						|| (requestedData->m_arenaInfo[0] == WORLDRULE_TEAM_DOMINATION))
						{
							ARENA_MESSAGE msg;
							msg.ArenaMessage = ARENA_KILLING_BLOW;
							msg.ArenaType = attackerData->m_arenaInfo[0];
							msg.Score = attackerData->m_arenaInfo[1];
							msg.Kills = attackerData->m_arenaInfo[2];
							msg.Deaths = attackerData->m_arenaInfo[3];
							msg.Damage = attackerData->m_arenaInfo[4];
							this->SendArenaMessage(attackerData, msg);
						}
			}//end 1
			else
			{//ai
              if(requestedData->m_currentInventory)
			  {
				 POSITION aiPosLocLast;
				 for(POSITION aiPosLoc = requestedData->m_currentInventory->GetHeadPosition();(aiPosLocLast = aiPosLoc) != NULL;)
				 {//1
                     CInventoryObj * invAiObj = (CInventoryObj *)requestedData->m_currentInventory->GetNext(aiPosLoc);				
					 if(invAiObj->m_itemData->m_nonDrop == TRUE && invAiObj->m_armed == TRUE)
					 {//handle non drop replacement							 
                             invAiObj->SafeDelete();
						     delete invAiObj;
							 invAiObj = 0;
                             requestedData->m_currentInventory->RemoveAt(aiPosLocLast); 
					 }//end handle non drop replacement
				 }//end 1
			  }
			}//end ai

			SendGuerenteedCycleMsgToClient(requestedData);
			if(!requestedData->m_aiControlled)
			{//non ai
				requestedData->m_accountNumberRefOld = requestedData->m_accountNumberRef;
				requestedData->m_accountNumberRef    = -1;
				requestedData->m_bodyNeedToBePlaced  = TRUE;
			}//end non ai
			requestedData->m_timeDied         = GetTickCount();
			requestedData->m_alive            = FALSE;
		 
       }//end player has just died

	   HandleArenaRulesOnDeath(stencilChannelStore,channelStore);//ussually evals present arena rules on kill
	   
return TRUE;
}
//-------------------------------------------------------------------------
void CSharedNet::HandleDeathQuestJournalStuff(CPlayerObject * requestedData,
							                  CPlayerObject * attackerData)
{
   if(!requestedData)
	   return;

   if(!attackerData)
	   return;

//quest journal drop/reward secton
   if(requestedData->m_aiControlled)
   if(!attackerData->m_aiControlled)
   if(attackerData->m_playerOrigRef)
      if(attackerData->m_playerOrigRef->m_questJournal)
	  {//valid quest journal
	     m_eventLog->LogByPriority("HandleDeathQuestJournalStuff()-Valid Quest Journal() ai cfg ref",(DWORD)requestedData->m_aiCfgReference,3);
         CQuestJournalObj * journalEntryPtr = attackerData->m_playerOrigRef->m_questJournal->GetUncompletedQuestJournalEntryByTarget(requestedData->m_aiCfgReference);
		 
		 if(journalEntryPtr)
		 if(journalEntryPtr->m_maxDropCount > 0)
		 {//can give more items

            if(!attackerData->m_currentInventory->CanTakeObjects(m_netComObject->m_maxItemLimit_onChar,1))
		    {//not overloaded
		        SendGeneralMessageHandler(29,attackerData);//overloaded
		    }//end not overloaded
			else{//give him item and adjust quest if percentage goes through

              int resultThrow = 0;
			  if(journalEntryPtr->m_dropPercentage < 100 && journalEntryPtr->m_dropPercentage != 0)
			     resultThrow = rand()%100;
              if(resultThrow <= journalEntryPtr->m_dropPercentage)
			  {//pass percentage
			    int glidStore = journalEntryPtr->m_dropGlid;
			    CItemObj * invObj = m_itemManager->GetItemByID_LOS(glidStore);
				if(invObj)
				{//found item to give
					
                   journalEntryPtr->m_maxDropCount--;
				   attackerData->m_currentInventory->AddInventoryItem(1,FALSE,invObj);
				   CString build = "You have attained: " + invObj->m_itemName;
                   SendTextMsg(DPNSEND_GUARANTEED,
						                            attackerData->m_currentID,
						                            build,
							                        0,
										            attackerData,
										            2);//confermation
                   if(journalEntryPtr->m_maxDropCount <= 0)
				   {//quest completed
				    build = "YOU HAVE ATTAINED ALL ITEMS FOR " + journalEntryPtr->m_npcName;
				    SendTextMsg(DPNSEND_GUARANTEED,
						                             attackerData->m_currentID,
						                             build,
							                         0,
										             attackerData,
										             2);//confermation
					journalEntryPtr->m_questDescription = "Return to "+journalEntryPtr->m_npcName;//update journal entry about progress
					journalEntryPtr->m_keyStatement = "";
					attackerData->m_playerOrigRef->m_questJournal->MakeMostRecentBySeer(journalEntryPtr->m_npcName);
					
                    if(journalEntryPtr->m_autoComplete)
					{//auto complete quest
				      
                    
					  if(AddExperienceToSkillByType(journalEntryPtr->m_skillExpType,
											        journalEntryPtr->m_expBonus,
											        journalEntryPtr->m_thisGainCap,
											        attackerData)==TRUE)
					  {//notify that you have gained experience
                         build = "You have gained experience..";
				         SendTextMsg(DPNSEND_GUARANTEED,
						                                  attackerData->m_currentID,
						                                  build,
							                              0,
										                  attackerData,
										                  2);//confermation
					  }//end notify that you have gained experience
					  
				      journalEntryPtr->m_status = 1;
					  if(journalEntryPtr->m_lastSelectionIndex == journalEntryPtr->m_currentProgression)
					     journalEntryPtr->m_currentProgression = journalEntryPtr->m_lastSelectionIndex + 1;
				      
					}//end auto complete quest
					UpdateClientRecentQuestJournalEntry(attackerData);
				   }//end quest completed
				   m_itemManager->Unlock();
				}//end found item to give
			  }//end pass percentage
			}//end give him item and adjust quest
		 }//end can give more items
	  }//end valid quest journal
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::HandleDeMountRequest(CPlayerObject * hostPtr)
{

		if(hostPtr->m_aiCfgOfMounted == -1)
		   return FALSE;//not mounted
	
	    hostPtr->m_aiCfgOfMounted = -1;
        //Confermation

		if(hostPtr->m_energy > 0)
        SendEquipMsg(-1,
					 hostPtr->m_networkAssignedID,
					 hostPtr,
				     40,
					 0);

        CAxList * channelList = m_netComObject->m_runtimePlayersOnServer->GetListByChannel(hostPtr->m_currentWorldID);
		//Broadcast
		if(channelList)
		for(CAxLinkObj* firePos = channelList->GetHeadPosition_unsafe();firePos != NULL;)
		{//player loop
           CPlayerObject * pData = (CPlayerObject *)channelList->GetNext_unsafe(&firePos);
           if(!pData->m_inGame)
			  continue;
		   
		   if(pData->m_currentID == 0)
			  continue;
		   
		   if(pData->m_aiControlled)
			  continue;

           if(m_cabLib.BoundingSphereCheck((float)m_netComObject->m_serverCalculateRadius,0.0f,
					                       pData->m_3dPosition.x,pData->m_3dPosition.y,pData->m_3dPosition.z,
										   hostPtr->m_3dPosition.x,hostPtr->m_3dPosition.y,hostPtr->m_3dPosition.z))
		   {//bounding check
		     if(pData->m_networkAssignedID != hostPtr->m_networkAssignedID)//not same player
			    if(pData->m_currentWorldID == hostPtr->m_currentWorldID){//same world 
					  SendEquipMsg(-1,
					               hostPtr->m_networkAssignedID,
					               pData,
				                   40,
								   0);
			  }//end same world
		   }//end bounding check
		}//end player loop
	return TRUE;
}
//--------------Handles system messages-----------------------------------------------//
CSpawnCfgObj * CSharedNet::GetSpawnPtr(int spawnCfgIndex)
{
       if(m_netComObject->m_spawnPointCfgs == NULL || m_netComObject->m_currentWorldsUsed == NULL)
	   {
           m_netComObject->m_eventLog->AddEvent("Error 546..Server has no valid preset spawn/world Cfgs");
		   return NULL;
	   }
  
	   if(spawnCfgIndex < 0)
	      return NULL;

       POSITION spwnPos = m_netComObject->m_spawnPointCfgs->FindIndex(spawnCfgIndex);
       if(!spwnPos)
	   {
           m_netComObject->m_eventLog->AddEvent("Error 547..Invalid spawn cfg",">",spawnCfgIndex);
		   return NULL;
	   }
	   CSpawnCfgObj * spwnPtr = (CSpawnCfgObj *) m_netComObject->m_spawnPointCfgs->GetAt(spwnPos);
       return spwnPtr;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::AddExperienceToSkillByType(int type,
											float m_expBonus,
											float m_thisGainCap,
											CPlayerObject * targetPayer)
{
    if(m_expBonus > 0.0f)
	{
	 if(!targetPayer->m_skills)
		 targetPayer->m_skills = new CSkillObjectList();

       CSkillObject * skillPtr = targetPayer->m_skills->GetSkillByType(type);
       
	   //see if we have used this skill before-if not add it to our database
       if(!skillPtr)
	   {//need to add this skill
          CSkillObject * dbSkillPtr = m_skillDatabase->GetSkillByType(type);
	      if(!dbSkillPtr)
		      return FALSE;
              dbSkillPtr->CloneMinimum(&skillPtr);
              targetPayer->m_skills->AddTail(skillPtr);
	   }//end need to add this skill
	  
	   if(skillPtr)
	   {
          if(skillPtr->m_currentExperience < m_thisGainCap)
		  {//you can still gain on this
             skillPtr->m_currentExperience += m_expBonus;
			 //*expGained = m_expBonus;
		  }//end you can still gain on this
		  else
		     return FALSE;
	   }
	}
	return TRUE;
}
//--------------send message----------------------------------------------------------//
BOOL CSharedNet::SendHouseUpdateToAllAIservers(int worldID,
									   	       D3DVECTOR wldPos,
										       int type,
										       int accountBind)
{

   CAxList * aiSvrList = m_netComObject->m_runtimeAccountDB->GetAllOnIndex(1,"aiserver");
	if(aiSvrList)
	   for(CAxLinkObj * posAIS = aiSvrList->GetHeadPosition_unsafe();posAIS != NULL;)
	   {
           CAccountObject * accPtr = (CAccountObject * )aiSvrList->GetNext_unsafe(&posAIS);
           SendSingleHousingState(accPtr->m_currentID,
										worldID,
										wldPos,
										type,
										accountBind);
	   }
   return TRUE;
}
//--------------send message----------------------------------------------------------//
BOOL CSharedNet::SendSingleHousingState(DPNID targetID,
										int worldID,
										D3DVECTOR wldPos,
										int type,
										int accountBind)
{
    if(!m_netComObject->m_houseingDB)
		return FALSE;

	int houseCount = 1;

	int msgSz = (sizeof(HOUSE_DATA)*houseCount)+1+4;

    HOUSE_DATA * houseBlockData = new HOUSE_DATA[houseCount];
	int trace = 0;

    houseBlockData[trace].m_acID      = accountBind;
    houseBlockData[trace].m_houseType = type;
    houseBlockData[trace].m_posX      = (short)wldPos.x;
    houseBlockData[trace].m_posY      = (short)wldPos.y;
    houseBlockData[trace].m_posZ      = (short)wldPos.z;


	BYTE * byteConversion = (BYTE*)houseBlockData;

	BYTE * byteBlock = new BYTE[msgSz];
	byteBlock[0] = 7;
	if(msgSz > 5)
		CopyMemory(&byteBlock[1],byteConversion,(msgSz-5));
	CopyMemory(&byteBlock[msgSz-4],&worldID,4);


    m_netComObject->SendData((BYTE *)byteBlock,
				                          msgSz,
				                          DPNSEND_GUARANTEED,
					                      targetID,
										  m_netComObject->m_packetTimeout);
    
	delete [] houseBlockData;
	houseBlockData = 0;
	delete [] byteBlock;
	byteBlock = 0;

return TRUE;
}
//--------------Handles system messages-----------------------------------------------//
BOOL CSharedNet::HandleMountRequest(CPlayerObject * hostPtr,
									int aiCfgIndex,
									int hostAnimStatic)
{

	    if(hostPtr->m_aiCfgOfMounted != -1){
		   HandleDeMountRequest(hostPtr);//twice will dismount you
		   return FALSE;
		}
	
	    hostPtr->m_aiCfgOfMounted = aiCfgIndex;
		hostPtr->m_curAnim        = hostAnimStatic;
        //Confermation
	
		SendEquipMsg(aiCfgIndex,
					 hostPtr->m_networkAssignedID,
					 hostPtr,
				     40,
					 0);

		//Broadcast
        CAxList * channelList = m_netComObject->m_runtimePlayersOnServer->GetListByChannel(hostPtr->m_currentWorldID);

		if(channelList)
		for(CAxLinkObj * firePos = channelList->GetHeadPosition_unsafe();firePos != NULL;)
		{//player loop
           CPlayerObject * pData = (CPlayerObject *)channelList->GetNext_unsafe(&firePos);
           if(!pData->m_inGame)
			  continue;
		   
		   if(pData->m_currentID == 0)
			  continue;
		   
		   if(pData->m_aiControlled)
			  continue;

		   if(pData->m_energy <= 0)
			  continue;

           if(m_cabLib.BoundingSphereCheck((float)m_netComObject->m_serverCalculateRadius,0.0f,
					                       pData->m_3dPosition.x,pData->m_3dPosition.y,pData->m_3dPosition.z,
										   hostPtr->m_3dPosition.x,hostPtr->m_3dPosition.y,hostPtr->m_3dPosition.z))
		   {//bounding check
		     if(pData->m_networkAssignedID != hostPtr->m_networkAssignedID)//not same player
			    if(pData->m_currentWorldID == hostPtr->m_currentWorldID){//same world 
				      /*SendDetailedMsgToClient(4,
							    hostPtr->m_networkAssignedID,
							    aiCfgIndex,
							    0,
							    pData);*/
					  SendEquipMsg(aiCfgIndex,
					               hostPtr->m_networkAssignedID,
					               pData,
				                   40,
								   0);
			  }//end same world
		   }//end bounding check
		}//end player loop
		


	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::SendStoreData(int storeID,CPlayerObject * player)
{
	if(!player)
		return FALSE;
  
	if(player->m_energy < 1)
	   return FALSE;


	 if(player->m_stasis == 2)
	 {// not whil shifted
        SendGeneralMessageHandler(54,player);
        return FALSE;
	 }// end not while shifted
          
     CCommerceObj * commercePtr = m_commerceDB->GetEstablishmentByID(storeID);
     if(!commercePtr){
         m_netComObject->m_eventLog->AddEvent("Cannot find store:","ID:",storeID);
         return FALSE;
	 }

	 if(!commercePtr->m_willDealWithCriminals)
		   if(player->m_noteriety)
           if(player->m_noteriety->m_currentMurders >= m_netComObject->m_murderCountMax)
		   {
              SendGeneralMessageHandler(53,player);
              return FALSE;
		   }

     int storeInventoryCount  = 0;//commercePtr->m_storeInventory->GetCount();
     
	 for(POSITION cSZPos = commercePtr->m_storeInventory->GetHeadPosition();cSZPos != NULL;)
	 {//inventory loop
           CStoreInventoryObj * invPtr = (CStoreInventoryObj *)commercePtr->m_storeInventory->GetNext(cSZPos);
		   if(invPtr->m_raceID > -1)
		      if(invPtr->m_raceID != player->m_curEDBIndex)
                 continue;
		   storeInventoryCount++;
	 }//end inventory loop
	 
	 
	 /*int playerInventoryCount = 0;

	 for(POSITION invPos = player->m_currentInventory->GetHeadPosition();invPos != NULL;)
	 {//inventory loop
         CInventoryObj * invPtr = (CInventoryObj *)player->m_currentInventory->GetNext(invPos);
		 if(!invPtr->m_itemData->m_nonDrop)
		 if(!invPtr->m_itemData->m_permanent)
		 if(!invPtr->m_armed)
			 playerInventoryCount++;
	 } //end inventory loop*/

	 STORE_BLOCKHEADER header;
	 int headerByteSz = sizeof(STORE_BLOCKHEADER);
	 int itemStructByteSz = sizeof(ITEMDATA_STRUCT);
	 header.m_catagorie      = 25;
	 header.m_menuBind       = commercePtr->m_menuBind;
	 header.m_storeItemCount = storeInventoryCount;
	 header.m_playerCash     = m_currencySystem->EvalCurrency(player->m_currentInventory);
	  header.m_storeCash = 0;

	 int itemCount = /*playerInventoryCount + */storeInventoryCount;
	 int itemDataByteSz = itemCount * itemStructByteSz;
	 ITEMDATA_STRUCT * itemDataBlock = NULL;
	 if(itemCount > 0)
	 {//valid info
	   itemDataBlock = new ITEMDATA_STRUCT[itemCount];
	   ZeroMemory(itemDataBlock,sizeof(ITEMDATA_STRUCT)*itemCount);
		    
	   int trace = 0;
        for(POSITION cPos = commercePtr->m_storeInventory->GetHeadPosition();cPos != NULL;)
		{//inventory loop
           CStoreInventoryObj * invPtr = (CStoreInventoryObj *)commercePtr->m_storeInventory->GetNext(cPos);
           if(invPtr->m_raceID > -1)
		      if(invPtr->m_raceID != player->m_curEDBIndex)
                 continue;

		   itemDataBlock[trace].m_GLID    = invPtr->m_globalInventoryID;
		   itemDataBlock[trace].m_quantity = 1;
		   itemDataBlock[trace].m_dMod = 0;
		   trace++;
		}//end inventory loop

	    /*for(POSITION invPos = player->m_currentInventory->GetHeadPosition();invPos != NULL;)
		{//inventory loop
           CInventoryObj * invPtr = (CInventoryObj *)player->m_currentInventory->GetNext(invPos);
		   if(!invPtr->m_itemData->m_nonDrop)
		   if(!invPtr->m_itemData->m_permanent)
		   if(!invPtr->m_armed)
		   {
		     itemDataBlock[trace].m_GLID    = invPtr->m_itemData->m_globalID;
		     itemDataBlock[trace].m_quanity = invPtr->m_quanity;
			 itemDataBlock[trace].m_dMod    = invPtr->m_itemData->m_damageModifier;
		     trace++;
		   }
		} //end inventory loop*/

	 }//end valid info

	 int msgSize = (headerByteSz + itemDataByteSz);
	 BYTE * dataByteBlock = new BYTE[msgSize];
	 CopyMemory(dataByteBlock,&header,headerByteSz);


	// BYTE * itemBlockByteConv = (BYTE*)itemDataBlock;
	 //for(int loop=0;loop<itemDataByteSz;loop++)
		 //dataByteBlock[loop+headerByteSz] = itemBlockByteConv[loop];

	 CopyMemory(&dataByteBlock[headerByteSz],itemDataBlock,itemDataByteSz);


     m_netComObject->SendByteBlockMsg(DPNSEND_GUARANTEED,
							               player->m_currentID,
							               dataByteBlock,
							               msgSize,
										   m_netComObject->m_packetTimeout
						                   );
	 if(itemDataBlock)
		delete [] itemDataBlock;
	    itemDataBlock = 0;

   return TRUE;
}
//--------------------------------------------------------------------------------------------//
BOOL CSharedNet::SendHouseStoreData(CPlayerObject * player,BOOL mustOwnHouse )
{
       if(!player)
		return FALSE;
  
	if(player->m_energy < 1)
	   return FALSE;

	 if(player->m_stasis == 2)
	 {// not whil shifted
        SendGeneralMessageHandler(54,player);
        return FALSE;
	 }// end not while shifted

	 CHPlacementObj * housePtr = NULL;
       if(!mustOwnHouse)
           housePtr = m_netComObject->m_houseingDB->GetHouseByPositionAndWldID(player->m_Target3dPosition,player->m_currentWorldID);
	   else
	       housePtr = m_netComObject->m_houseingDB->GetHousePlacementByPosAcSpecial(player->m_Target3dPosition,player->m_accountNumberRef,player->m_currentWorldID);//must be owner to do this
   
     if(!housePtr)
     {
         return FALSE;
     }

	 if(!housePtr->m_storeInventory)
		 return FALSE;

     int storeInventoryCount  = (int)housePtr->m_storeInventory->GetCount();
     int playerInventoryCount = 0;

	 for(POSITION invPos = player->m_currentInventory->GetHeadPosition();invPos != NULL;)
	 {//inventory loop
         CInventoryObj * invPtr = (CInventoryObj *)player->m_currentInventory->GetNext(invPos);
		 if(!invPtr->m_itemData->m_nonTransferable)
		 if(!invPtr->m_itemData->m_permanent)
		 if(!invPtr->m_armed)
			 playerInventoryCount++;
	 } //end inventory loop

	 STORE_BLOCKHEADER header;
	 int headerByteSz = sizeof(STORE_BLOCKHEADER);
	 int itemStructByteSz = sizeof(ITEMDATA_STRUCTEXT);
	 header.m_catagorie      = 26;
	 if(!mustOwnHouse)
	      header.m_menuBind       = 301;
	 else
		  header.m_menuBind       = 300;
	 header.m_storeItemCount = storeInventoryCount;
	 header.m_playerCash     = 0;
	 header.m_storeCash = housePtr->m_storeCurrentCash;

	 int itemCount = playerInventoryCount + storeInventoryCount;
	 int itemDataByteSz = itemCount * itemStructByteSz;
	 ITEMDATA_STRUCTEXT * itemDataBlock = NULL;
	 if(itemCount > 0)
	 {//valid info
	   itemDataBlock = new ITEMDATA_STRUCTEXT[itemCount];
		    
	   int trace = 0;
        for(POSITION cPos = housePtr->m_storeInventory->GetHeadPosition();cPos != NULL;)
		{//inventory loop
           CInventoryObj * invPtr = (CInventoryObj *)housePtr->m_storeInventory->GetNext(cPos);
		   itemDataBlock[trace].m_GLID    = invPtr->m_itemData->m_globalID;
		   itemDataBlock[trace].m_quanity = invPtr->m_quanity;
		   itemDataBlock[trace].m_price   = invPtr->m_itemData->m_marketCost;
		   itemDataBlock[trace].m_dMod    = invPtr->m_itemData->m_damageModifier;
		   trace++;
		}//end inventory loop

	    for(POSITION invPos = player->m_currentInventory->GetHeadPosition();invPos != NULL;)
		{//inventory loop
           CInventoryObj * invPtr = (CInventoryObj *)player->m_currentInventory->GetNext(invPos);
		   if(!invPtr->m_itemData->m_nonTransferable)
		   if(!invPtr->m_itemData->m_permanent)
		   if(!invPtr->m_armed)
		   {
		     itemDataBlock[trace].m_GLID    = invPtr->m_itemData->m_globalID;
		     itemDataBlock[trace].m_quanity = invPtr->m_quanity;
			 itemDataBlock[trace].m_price   = 0;
			 itemDataBlock[trace].m_dMod    = invPtr->m_itemData->m_damageModifier;
		     trace++;
		   }
		} //end inventory loop

	 }//end valid info

	 int msgSize = (headerByteSz + itemDataByteSz);
	 BYTE * dataByteBlock = new BYTE[msgSize];
	 CopyMemory(dataByteBlock,&header,headerByteSz);


	 BYTE * itemBlockByteConv = (BYTE*)itemDataBlock;
	 for(int loop=0;loop<itemDataByteSz;loop++)
		 dataByteBlock[loop+headerByteSz] = itemBlockByteConv[loop];

     m_netComObject->SendByteBlockMsg(DPNSEND_GUARANTEED,
							               player->m_currentID,
							               dataByteBlock,
							               msgSize,
										   m_netComObject->m_packetTimeout
						                   );
	 if(itemDataBlock)
		delete [] itemDataBlock;
	    itemDataBlock = 0;

   return TRUE;
}
//------------------------------------------------------------------------------------//
/*BOOL CSharedNet::BankRequest(CPlayerObject * player,DPNID idFromLoc)
{  
   //m_bankInventory
   

   if(player->m_stasis == 2)
   {// not while shifted
       SendGeneralMessageHandler(54,player);
       return FALSE;
   }// end not while shifted

   if(player->m_energy < 1)
      return FALSE;

   int stencilChannel = m_netComObject->m_currentWorldsUsed->GetStencilChannelByLiveChannel_safe(player->m_currentWorldID);//translate instance channel to stencil channel
   if(stencilChannel < 0)//cannot find stencil
	  m_eventLog->AddEvent("Cannot find bank GetStencilChannelByLiveChannel_safe() BankRequest");

   CBankObj * bankPtr = m_bankZoneList->GetBankByPosition(player->m_3dPosition,stencilChannel);
   if(bankPtr)
   {//suceeded you are in a valid bank zone 
	  
	   if(!bankPtr->m_willDealWithCriminals)
		   if(player->m_noteriety)
           if(player->m_noteriety->m_currentMurders >= m_netComObject->m_murderCountMax)
		   {
              SendGeneralMessageHandler(53,player);
              return FALSE;
		   }
      CAccountObject * accountPtr = m_netComObject->m_runtimeAccountDB->GetAccountByID(idFromLoc);
      if(!accountPtr){//error locating account
          m_netComObject->m_eventLog->AddEvent("Error locating account inside bank request");
	      return FALSE;
      }//end error locating account
	  

      //send bank inventory
	  CPlayerObject * characterOnBaseAccount = accountPtr->GetCurrentCharacter();
      if(!characterOnBaseAccount){
          m_netComObject->m_eventLog->AddEvent("Cannot access base char ",accountPtr->m_accountName,65);
          return FALSE;
	  }

      if(!characterOnBaseAccount->m_bankInventory)
	  {
		  return FALSE;
	  }

      //retrieve sizes
      int bankItemCount        = (int)characterOnBaseAccount->m_bankInventory->GetCount();
      int playerInventoryCount = 0;

	  if(player->m_currentInventory)
      for(POSITION invPos = player->m_currentInventory->GetHeadPosition();invPos != NULL;)
	  {//inventory loop
         CInventoryObj * invPtr = (CInventoryObj *)player->m_currentInventory->GetNext(invPos);
		 if(!invPtr->m_itemData->m_permanent)
		 if(!invPtr->m_armed)
			 playerInventoryCount++;
	  } //end inventory loop* /
      int headerSize = sizeof(BANK_BLOCKHEADER);
	  int itemStructSize = sizeof(ITEMDATA_STRUCT);
	  int totalSize = ((bankItemCount*itemStructSize) + (playerInventoryCount*itemStructSize)) + headerSize;
	  //if(totalSize == headerSize)
	  //	  return FALSE;
	  BYTE * dataBlock = new BYTE[totalSize];

	  BANK_BLOCKHEADER header;
	  header.m_menuBind = bankPtr->m_menuBind;//open menu call
	  header.m_bankCash = 0;//bank cash
	  header.m_bankItemCount = bankItemCount;
	  header.m_onCharCash = 0;
	  //fill data
      header.m_catagorie = 24;

	  CopyMemory(dataBlock,&header,headerSize);


	  if(totalSize > headerSize)
	  {//items to send
	      ITEMDATA_STRUCT * itemDataBlock = new ITEMDATA_STRUCT[(bankItemCount + playerInventoryCount)];

		  int trace = 0;
	      for(POSITION invPos = characterOnBaseAccount->m_bankInventory->GetHeadPosition();invPos != NULL;)
		  {//inventory loop
             CInventoryObj * invPtr = (CInventoryObj *)characterOnBaseAccount->m_bankInventory->GetNext(invPos);
			 itemDataBlock[trace].m_GLID    = invPtr->m_itemData->m_globalID;
			 itemDataBlock[trace].m_quanity = invPtr->m_quanity;
			 itemDataBlock[trace].m_dMod    = invPtr->m_itemData->m_damageModifier;
			 trace++;
		  } //end inventory loop

		  if(player->m_currentInventory)
          for(POSITION invPos = player->m_currentInventory->GetHeadPosition();invPos != NULL;)
		  {//inventory loop
             CInventoryObj * invPtr = (CInventoryObj *)player->m_currentInventory->GetNext(invPos);
		     if(!invPtr->m_itemData->m_permanent)
			 if(!invPtr->m_armed)
			 {
				 itemDataBlock[trace].m_GLID    = invPtr->m_itemData->m_globalID;
			     itemDataBlock[trace].m_quanity = invPtr->m_quanity;
				 itemDataBlock[trace].m_dMod    = invPtr->m_itemData->m_damageModifier;
			     trace++;
			 }
		  } //end inventory loop* /


		  BYTE * itemByteConv = (BYTE*)itemDataBlock;
          
		  for(int loop=0;loop<(totalSize-headerSize);loop++)
		      dataBlock[(loop+headerSize)] = itemByteConv[loop];

		  delete [] itemDataBlock;
          itemDataBlock = 0;
	  }//end items to send
		  
	  m_netComObject->SendByteBlockMsg(DPNSEND_GUARANTEED,
							                    idFromLoc,
							                    dataBlock,
							                    totalSize,
												m_netComObject->m_packetTimeout
						                        );

		  
          
   }//end suceeded you are in a valid bank zone 
   
   

   return TRUE;
}*/

BOOL 
CSharedNet::BankRequest(CPlayerObject *pPlayer, DPNID idFromLoc, int iLockerIndex)
{  
	// GET the player's account object
	CAccountObject *pAccount = m_netComObject->m_runtimeAccountDB->GetAccountByID(idFromLoc);
	if(!pAccount)
	{
		m_netComObject->m_eventLog->AddEvent("BankRequest(): FAILED to locate the player's account!");
		return FALSE;
	}

	if(!pAccount->AllowAsyncQue())
	{//throttle
		m_netComObject->m_eventLog->AddEvent("BankRequest(): spam!"+pAccount->m_accountName);
		return FALSE;
	}//end throttle


	// ERROR checking: Don't satisfy a bank request if the player is shifted
	if (pPlayer->m_stasis == 2)
	{
		SendGeneralMessageHandler(54, pPlayer);
		return FALSE;
	}

	// ERROR checking: Is the player alive?
	if(pPlayer->m_energy < 1)
		return FALSE;

	// GET the current stencil channel ID
	//	NOTE: Translate instance channel to stencil channel
	int stencilChannel = this->m_netComObject->m_currentWorldsUsed->GetStencilChannelByLiveChannel_safe(pPlayer->m_currentWorldID);
	if (stencilChannel < 0)
		m_eventLog->AddEvent("BankRequest(): FAILED to find a valid stencil channel!");

	// GET a reference to the bank object
	CBankObj *pBank = m_bankZoneList->GetBankByPosition(pPlayer->m_3dPosition,stencilChannel);
	if (!pBank) 
		return FALSE;

	// ERROR checking
	if ((!pBank->m_willDealWithCriminals)
	&& (pPlayer->m_noteriety)
	&& (pPlayer->m_noteriety->m_currentMurders >= m_netComObject->m_murderCountMax))
	{
		SendGeneralMessageHandler(53, pPlayer);
		return FALSE;
	}

	// GET the player's base account
	CPlayerObject *pBaseAccountPlayer = pAccount->m_characterOnAccount;
	if (!pBaseAccountPlayer)
	{
		m_netComObject->m_eventLog->AddEvent("**BankRequest(): FAILED pBaseAccountPlayer == NULL", 
			pAccount->m_accountName, 65);
		return FALSE;
	}
		
	if(pBaseAccountPlayer->m_banks.size() == 0)
	{
		// INIT all player's banks
		CInventoryObjList *pInventory = NULL;
		for (int i=0; i<MAX_BANKS; i++)
		{
			pInventory = new CInventoryObjList();
			pBaseAccountPlayer->m_banks.push_back(pInventory);
		}
		// first bank use on this character: empty banks were just made - open them
		// (this used to log FAILED and refuse, so the bank only opened on the 2nd try)
		m_netComObject->m_eventLog->AddEvent("BankRequest(): created empty banks for",
			pAccount->m_accountName, 66);
	}

	// CALCULATE sizes for network packet construction...
	int iHeaderSize = sizeof(BANK_BLOCKHEADER);

	// GET the number of items in the specified bank locker
	//short iNumBanks = pBaseAccountPlayer->m_banks.size();
	//int iNumBankItems = 0;
	//for (int i=0; i<pBaseAccountPlayer->m_banks.size(); i++)
	if (iLockerIndex >= pBaseAccountPlayer->m_banks.size())
		return FALSE;
	int iNumBankItems = pBaseAccountPlayer->m_banks[iLockerIndex]->GetCount();

	// GET the number of items in the player's inventory
	/*int iNumInventoryItems = 0;
	if (pPlayer->m_currentInventory)
	{
		for (POSITION pos = pPlayer->m_currentInventory->GetHeadPosition(); pos != NULL;)
		{
			CInventoryObj *pItem = (CInventoryObj*)pPlayer->m_currentInventory->GetNext(pos);
			if (pItem->m_itemData->m_permanent) continue;// && pItem->m_armed) continue; //By commenting this out, we don't count any permanant items
			++iNumInventoryItems;
		}
	}*/

	// CALCULATION key:
	//	-Size of the packet header
	//	-Number of bank inventories
	//	-Number of bank items (per bank)
	//	-Size of all the bank items
	//	-Number of inventory items => DEPRECATED
	//	-Size of all the inventory items => DEPRECATED
	int iPacketSize = iHeaderSize 
		//+ sizeof(short) 
		//+ (sizeof(short) * iNumBanks)
		+ (iNumBankItems * sizeof(ITEMDATA_STRUCT)) 
		/*+ sizeof(short) 
		+ (iNumInventoryItems * sizeof(ITEMDATA_STRUCT))*/; 

	// CREATE the network packet
	int iCount = 0;
	BYTE *pData = new BYTE[iPacketSize];

	// CREATE the packet header
	BANK_BLOCKHEADER header;
	header.m_menuBind = iLockerIndex; //pBank->m_menuBind; // For the OpenMenu() call
	header.m_bankCash = 0; // Bank cash
	header.m_bankItemCount = iNumBankItems;
	header.m_onCharCash = 0;
	header.m_catagorie = 24;

	// COPY the header into the final packet
	CopyMemory(pData, &header, iHeaderSize);
	iCount += iHeaderSize;

	// COPY in the number of banks we're encoding
	//CopyMemory(&pData[iCount], &iNumBanks, sizeof(iNumBanks));
	//iCount += sizeof(iNumBanks);

	// SETUP for encoding inventory items
	short iNumItems = 0;
	POSITION pos = NULL;
	CInventoryObj *pItem = NULL;

	// FOR EACH bank inventory present
	//for (int i=0; i<pBaseAccountPlayer->m_banks.size(); i++)
	//{
		// COPY in the number of bank items we're encoding
		/*iNumItems = (short)pBaseAccountPlayer->m_banks[i]->GetCount();
		CopyMemory(&pData[iCount], &iNumItems, sizeof(iNumItems));
		iCount += sizeof(iNumItems);*/

		// FOR EACH item in this bank inventory
		for(pos = pBaseAccountPlayer->m_banks[iLockerIndex]->GetHeadPosition(); pos != NULL;)
		{
			pItem = (CInventoryObj*)pBaseAccountPlayer->m_banks[iLockerIndex]->GetNext(pos);
			if (!pItem) continue;

			// CREATE item to add to the packet
			ITEMDATA_STRUCT item;
			item.m_GLID = pItem->m_itemData->m_globalID;
			item.m_quantity = pItem->m_quanity;
			item.m_dMod = pItem->m_itemData->m_damageModifier;
	
			// ENCODE this item into the packet
			CopyMemory(&pData[iCount], &item, sizeof(item));
			iCount += sizeof(item);
		} 
	//}
	
	/* DEPRECATED /////////////////////////////////////////////////////////////
	// NOTE: No longer sending player inventory!
	// COPY in the number of inventory items we're encoding
	iNumItems = (short)pPlayer->m_currentInventory->GetCount();
	CopyMemory(&pData[iCount], &iNumItems, sizeof(iNumItems));
	iCount += sizeof(iNumItems);

	// FOR EACH item in this inventory
	for(pos = pPlayer->m_currentInventory->GetHeadPosition(); pos != NULL;)
	{
		pItem = (CInventoryObj*)pPlayer->m_currentInventory->GetNext(pos);
		if (!pItem 
		|| (pItem->m_itemData->m_permanent))// && pItem->m_armed))  //By commenting this out, we remove any sending of permanant items
			continue;

		// CREATE item to add to the packet
		ITEMDATA_STRUCT item;
		item.m_GLID = pItem->m_itemData->m_globalID;
		item.m_quanity = pItem->m_quanity;
		item.m_dMod = pItem->m_itemData->m_damageModifier;

		// ENCODE this item into the packet
		CopyMemory(&pData[iCount], &item, sizeof(item));
		iCount += sizeof(item);
	} */

	// SEND the bank request response packet
	this->m_netComObject->SendByteBlockMsg(DPNSEND_GUARANTEED,
		idFromLoc,
		pData,
		iPacketSize,
		this->m_netComObject->m_packetTimeout);
	return TRUE;
}

//------------------------------------------------------------------------------------//
BOOL CSharedNet::SendQuestDialog(CPlayerObject * targetPayer,CString dialog)
{
    if(!targetPayer)
	    return FALSE;

	if(targetPayer->m_aiControlled)
		return FALSE;

	//get size
	int currentSize = 1;
    currentSize += dialog.GetLength();

    if(currentSize == 1)
       return FALSE;
    //Build Data
    BYTE * blockData = new BYTE[currentSize];
	blockData[0] = 4;//quest Dialog
	int traceDataBuild = 1;
    for(int i=0;i<dialog.GetLength();i++)
	{
        blockData[traceDataBuild] = dialog.GetAt(i);
   	    traceDataBuild++;
	}

	if(targetPayer->m_miscOutBox->GetCount() < 20)
	targetPayer->m_miscOutBox->AddPacket(currentSize,(BYTE*)blockData,0,targetPayer->m_currentWorldID);

	delete [] blockData;
	blockData = 0;


	 return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::SendClientScript(CPlayerObject * targetPayer,CString script,CString text)
{
    if(!targetPayer)
	    return FALSE;

	if(targetPayer->m_aiControlled)
		return FALSE;

	//get size
	int currentSize = 1;
    currentSize += script.GetLength();
	currentSize += text.GetLength();
	currentSize += 5;

    //Build Data
    BYTE * blockData = new BYTE[currentSize];
	ZeroMemory(blockData,currentSize);
	blockData[0] = 6;//script
	
	short clientScriptLength = script.GetLength();
	short textLength = text.GetLength();
	CopyMemory(&blockData[1],&clientScriptLength,2);
	CopyMemory(&blockData[3],&textLength,2);
	if(clientScriptLength > 0)
	   CopyMemory(&blockData[5],script.GetBuffer(),clientScriptLength);
	if(textLength > 0)
	   CopyMemory(&blockData[5+clientScriptLength],text.GetBuffer(),textLength);
	if(targetPayer->m_miscOutBox->GetCount() < 20)
	   targetPayer->m_miscOutBox->AddPacket(currentSize,(BYTE*)blockData,0,targetPayer->m_currentWorldID);
	delete [] blockData;
	blockData = 0;
	 
	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::SendExtendedTextToChatWindow(CPlayerObject * targetPayer, CString text)
{

      int stringLength = text.GetLength();
	  if(stringLength < 1)
	     return FALSE;
                    
      int packetCount = 1;
	  if(stringLength > 200)
         packetCount = (stringLength/200)+1;

	     CString currentString;
         for(int pkLoop=0;pkLoop<packetCount;pkLoop++)
		 {
                        
             int currentStringLength = 200;
		     if(stringLength < 200)
			    currentStringLength = stringLength;
             else{
                while(currentStringLength < stringLength && currentStringLength < 250)
			    {
                     if(text.GetAt(currentStringLength) == ' ')
					    break;

                        currentStringLength++;
				}			
			 }

             if(currentStringLength > 0)
		     {
			    currentString = text.Left(currentStringLength);
				text.Delete(0,currentStringLength);
				stringLength = stringLength - currentStringLength;
                
			    SendTextMsg(DPNSEND_GUARANTEED,
						                         targetPayer->m_currentID,
						                         currentString,
							                     0,
											     targetPayer,
												 0);//confermation
			 }			
		 }
		 return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::HandleReassignRespawnByDetails(CPlayerObject * pData,CAccountObject * accountPtr,DPNID idFromLoc,D3DVECTOR position,CString wldName)
{
	if(pData->m_energy <= 0)
	   return FALSE;
	
    if(!accountPtr->m_characterOnAccount)
	{//bad things
       m_netComObject->m_eventLog->AddEvent("No characters created yet 009");
	   return FALSE;
	}//end bad things

    CPlayerObject * characterOnAccount = accountPtr->m_characterOnAccount;
	 
    if(characterOnAccount)
	{//valid char on account
   
	      characterOnAccount->m_respawnPosition.x = position.x;
	      characterOnAccount->m_respawnPosition.y = position.y + 4.0f;
	      characterOnAccount->m_respawnPosition.z = position.z;
          characterOnAccount->m_respawnWorldName  = wldName;//pData->m_currentWorldID;
	      SendGeneralMessageHandler(7,pData);
	}

	return TRUE;
}
//------------------------------------------------------------------------------------//
void CSharedNet::DoRadiusDamage_Heal(int amount,
									 int damageChannel,
									 float radiusSz,
								     CPlayerObject * hostPtr)
{//Broadcast
    if(!hostPtr)
		return;

    CAxList * channelList = m_netComObject->m_runtimePlayersOnServer->GetListByChannel(hostPtr->m_currentWorldID);

	long currentTimeLoc = GetTickCount();
	  if(channelList)
		for(CAxLinkObj* firePos = channelList->GetHeadPosition_unsafe();firePos != NULL;)
		{//player loop
           CPlayerObject * pData = (CPlayerObject *)channelList->GetNext_unsafe(&firePos);
           
           if(pData->m_currentWorldID != hostPtr->m_currentWorldID)
		      continue;

		   if(amount > 0)//damage filters host only
		   if(hostPtr->m_handle == pData->m_handle)
			  continue;
		   
		   if(!pData->m_inGame)
			  continue;
		   
		   if(pData->m_energy < 1)
			  continue;

		   

           if(m_cabLib.BoundingSphereCheck((float)radiusSz,0.0f,
					                       pData->m_3dPosition.x,pData->m_3dPosition.y,pData->m_3dPosition.z,
										   hostPtr->m_Target3dPosition.x,hostPtr->m_Target3dPosition.y,hostPtr->m_Target3dPosition.z))
		   {//bounding check
		       float amountTemp = (float)amount;
			   if(amountTemp > 0.0)
			   {//take into account armour rating if damage is greater than zero otherwise is a healing issue
                  if(pData->m_inSafeZone)//if in safe zone skip dammage application
					 continue;

				  if(!hostPtr->m_admin)//admins not applicable
		             if(hostPtr->m_gm == TRUE || pData->m_gm == TRUE)//this makes gms invunerable and unable to deliver damage
		                continue;

				  if(pData->m_invunerableTemp)
	              {//somone is attacking somone invunerable
                     if(!pData->Vulnerable())
		             {
			             if(!pData->m_aiControlled)
                         SendGeneralMessageHandler(80,hostPtr);
		                 continue;//filter damage for now
		             }
	              }//end somone is attacking somone invunerable
                  if(hostPtr->m_invunerableTemp)
	              {//attacker is attempting tol do damage while invunerable
                     if(!hostPtr->Vulnerable())
		             {
			             if(!hostPtr->m_aiControlled)
                           SendGeneralMessageHandler(81,hostPtr);
		                   continue;//filter damage for now
		             }
	              }//end attacker is attempting tol do damage while invunerable

				  if(!hostPtr->m_aiControlled)
				  if(!pData->m_aiControlled)
			      if(!m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(pData->m_stencilChannel,pData->m_currentWorldID,WORLDRULE_NOLEVELRESTRICTIONS))
				  {
                      if(hostPtr->m_currentHighestLevel < m_netComObject->m_pkLevelRestriction)
					  {
                         //SendGeneralMessageHandler(27,pData);//no skills notification    
			             continue;
					  }
			
                      if(pData->m_currentHighestLevel < m_netComObject->m_pkLevelRestriction)
					  {
                         //SendGeneralMessageHandler(28,pData);//no skills notification
			             continue;
					  }
				  }//end level restrictions


			      float armourRating = (float)pData->m_currentInventory->GetArmourRating(damageChannel,FALSE,-1);
				  
				  ///////////////////////////////
				  // 03/24/2006 update         //
				  // Check for Reistance Bonus //
				  ///////////////////////////////
				  
				  if(damageChannel == 0 && (pData->m_bonus[2].m_bonusType == 6 || pData->m_bonus[2].m_bonusType == 14))
					armourRating += pData->m_bonus[2].m_bonusAmount;
				  if(damageChannel == 1 && (pData->m_bonus[2].m_bonusType == 7 || pData->m_bonus[2].m_bonusType == 14))
					armourRating += pData->m_bonus[2].m_bonusAmount;
				  if(damageChannel == 2 && (pData->m_bonus[2].m_bonusType == 8 || pData->m_bonus[2].m_bonusType == 14))
					armourRating += pData->m_bonus[2].m_bonusAmount;
				  if(damageChannel == 3 && (pData->m_bonus[2].m_bonusType == 9 || pData->m_bonus[2].m_bonusType == 14))
					armourRating += pData->m_bonus[2].m_bonusAmount;
				  if(damageChannel == 4 && (pData->m_bonus[2].m_bonusType == 10 || pData->m_bonus[2].m_bonusType == 14))
					armourRating += pData->m_bonus[2].m_bonusAmount;
				  if(damageChannel == 5 && (pData->m_bonus[2].m_bonusType == 11 || pData->m_bonus[2].m_bonusType == 14))
					armourRating += pData->m_bonus[2].m_bonusAmount;
				  if(damageChannel == 6 && (pData->m_bonus[2].m_bonusType == 12 || pData->m_bonus[2].m_bonusType == 14))
					armourRating += pData->m_bonus[2].m_bonusAmount;
				  if(damageChannel == 7 && (pData->m_bonus[2].m_bonusType == 13 || pData->m_bonus[2].m_bonusType == 14))
					armourRating += pData->m_bonus[2].m_bonusAmount;
	              
				  if(armourRating > 90)
					  armourRating = 90;

				  ////////////////
				  // End Update //
				  ////////////////

				  armourRating = 1.0f - (armourRating * .01f);
                  if(armourRating <= 0.0f){//error
                     armourRating = 0.0f;
                     m_netComObject->m_eventLog->AddEvent("Players armour class is bad,possible cheat:");
				  }//end error

                  amountTemp = (((float)amountTemp)*armourRating);//damage amount
			   }//end take into account armour rating if damage is greater than zero otherwise is a healing issue
	              
			   
			   
			   
			   pData->m_energy -= (int)amountTemp;
			    if(pData->m_energy > pData->m_maxEnergy)
				   pData->m_energy = pData->m_maxEnergy;
			       //send update players health
				   
				   if(amountTemp > 0)
                      hostPtr->Attack(pData,350000,(int)amountTemp);//for murder agressor lists
		               
				   if(!hostPtr->m_aiControlled)
				   {//inform caster of damage done
					   SendDamageInfoMessageHandler(amount,
								                    pData->m_networkAssignedID,
								                    hostPtr);
				   }//end inform caster of damage done

                   if(!pData->m_aiControlled)
				   {//update client that got damaged
		               if(pData->m_currentID > 0)
			              UpdateClientStatistics(pData,pData->m_currentID);
				   }//end update client that got damaged
				   else{//update ai server object
					   if(pData->m_currentID > 0)
                          SendAttribMsg(DPNSEND_GUARANTEED,
			                    pData->m_currentID,
					            3,
				                pData->m_energy,
					            pData->m_networkAssignedID,
						        hostPtr->m_networkAssignedID,//attacker assigned id
						        pData->m_maxEnergy,
								pData);
				   }//end update ai server object


                   if(pData->m_energy < 1)//killed self with poison
				   {
				      CAccountObject * accountPtr = m_netComObject->m_runtimeAccountDB->GetAccountByID(pData->m_currentID);
				      if(accountPtr)
					  {
					    HandleDeath(pData,pData);
					  }
				   }


		   }//end bounding check
		}//end player loop
}
//--------------send message----------------------------------------------------------//
BOOL CSharedNet::RevealRadius(float radius, CPlayerObject * playerDetecting)
{

   CAxList * channelList = m_netComObject->m_runtimePlayersOnServer->GetListByChannel(playerDetecting->m_currentWorldID);

   if(channelList)
   for(CAxLinkObj * posLoc = channelList->GetHeadPosition_unsafe();posLoc != NULL;)
   {
       CPlayerObject * lstObj = (CPlayerObject *)channelList->GetNext_unsafe(&posLoc);
	   if(lstObj->m_aiControlled)
	      continue;

	   if(lstObj->m_stasis == 1)//must be hidden
	   if(playerDetecting->m_currentWorldID == lstObj->m_currentWorldID){//same world //different computer link
          if(m_cabLib.BoundingSphereCheck((float)radius,
			                                     0.0f,
					                             lstObj->m_3dPosition.x,
												 lstObj->m_3dPosition.y,
												 lstObj->m_3dPosition.z,
											     playerDetecting->m_3dPosition.x,
												 playerDetecting->m_3dPosition.y,
												 playerDetecting->m_3dPosition.z))
		  {//collision
             Unhide(lstObj);		
		  }//end collision
	   }//end id
   }//end same world
   return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::SendAvailableFromItem(CPlayerObject * plPtr,int specifiedLevel,int glid)
{
   if(!plPtr->m_currentInventory)
   return FALSE;
       CInventoryObj * invPtr = plPtr->m_currentInventory->GetByGLID(glid);
       if(invPtr)
	   {//valid item
          if(!invPtr->m_itemData->m_expansiveCollection)
			  return FALSE;

		  int validItemCount = 0;
          //size counter
		  POSITION posLoc;
          for(posLoc = invPtr->m_itemData->m_expansiveCollection->GetHeadPosition();posLoc != NULL;)
		  {
              CExpansiveObj * expObj = (CExpansiveObj *)invPtr->m_itemData->m_expansiveCollection->GetNext(posLoc);
              if(expObj->m_miscInt == specifiedLevel)
                 validItemCount++;
		  }
		  if(validItemCount > 0)
		  {//valid so send packet
			 int filltrace = 0;
             ICON_POS * iconPosArray = new ICON_POS[validItemCount];
             for(posLoc = invPtr->m_itemData->m_expansiveCollection->GetHeadPosition();posLoc != NULL;)
			 {
              CExpansiveObj * expObj = (CExpansiveObj *)invPtr->m_itemData->m_expansiveCollection->GetNext(posLoc);
              if(expObj->m_miscInt == specifiedLevel)
			  {
                 iconPosArray[filltrace].m_xPos        = expObj->m_xPos;
				 iconPosArray[filltrace].m_yPos        = expObj->m_yPos;
                 iconPosArray[filltrace].m_iconDBIndex = expObj->m_iconIndex;
                 filltrace++;
			  }
			 }
			 //build packet
             //signed char catagorieLocal = 37;
             int messageSize = (sizeof(ICON_POS)*validItemCount) + 1;
			 BYTE * byteCast = (BYTE*)iconPosArray;
			 BYTE * dataBlock = new BYTE[messageSize];
             
			 dataBlock[0] = 37;
             for(long i=1;i<messageSize;i++)
			 {
                 dataBlock[i] = byteCast[i-1];
			 }

	         m_netComObject->SendData(dataBlock,
				                           messageSize,
				                           DPNSEND_GUARANTEED,
					                       plPtr->m_currentID,
								           m_netComObject->m_packetTimeout);

			 delete [] iconPosArray;
			 iconPosArray = 0;
			 delete [] dataBlock;
			 dataBlock = 0;

		  }//end valid so send packet
	   }//end valid item
   return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::SendDamageInfoMessageHandler(int damageAmount,int idOfTarget,CPlayerObject * playerObj, bool death)
{
    if(playerObj->m_aiControlled)
	   return FALSE;

	if(playerObj->m_playerEntityDamageMgsOutbox->GetCount() > 10)
	   return FALSE;


	DAMAGEINFO_DATA dataPacket;
	int messageSize = sizeof(DAMAGEINFO_DATA);
    dataPacket.m_catagorie = 32;
	dataPacket.m_damageAmount      = damageAmount;
	dataPacket.m_targetPlayerID    = idOfTarget;
	dataPacket.m_death			   = death;
 
	//CString tempFrmt;
	//tempFrmt.Format("SendDamageInfoMessageHandler int:%i short:%i",damageAmount,dataPacket.m_damageAmount);
	//m_eventLog->LogByPriority(tempFrmt,2);

	playerObj->m_playerEntityDamageMgsOutbox->AddPacket(messageSize,(BYTE*)&dataPacket,0,playerObj->m_currentWorldID);


	return TRUE;
}

//------------------------------------------------------------------------------------//
BOOL CSharedNet::HandleReassignRespawn_requiresAcLock(CPlayerObject * pData,DPNID idFromLoc)
{
	if(pData->m_energy <= 0)
	   return FALSE;

    CAccountObject * accountPtr = m_netComObject->m_runtimeAccountDB->GetAccountByID(idFromLoc);
    if(!accountPtr){//error locating account
        m_netComObject->m_eventLog->AddEvent("Error locating account 83");
        return FALSE;
	}//end error locating account
    if(!accountPtr->m_characterOnAccount)
	{//bad things
       m_netComObject->m_eventLog->AddEvent("No characters created yet 009");
	   return FALSE;
	}//end bad things

    CPlayerObject * characterOnAccount = accountPtr->m_characterOnAccount;
	 
    if(characterOnAccount)
	{//valid char on account
	   int zeroBasedIndex = 0;
	   characterOnAccount->m_respawnPosition.x = pData->m_3dPosition.x;
	   characterOnAccount->m_respawnPosition.y = pData->m_3dPosition.y+4.0f;
	   characterOnAccount->m_respawnPosition.z = pData->m_3dPosition.z;
       characterOnAccount->m_respawnWorldName  = pData->m_currentWorldMap;//pData->m_currentWorldID;
	   SendGeneralMessageHandler(7,pData);
	}
	
	return TRUE;
}
//------------------------------------------------------------------------------------//
CString CSharedNet::SanitizeFormatOut(CString text)
{
    char quoteX[2];
    quoteX[0] = '"';
    quoteX[1] = 0;
	text.Replace(quoteX,"'");

    return text;
}

//------------------------------------------------------------------------------------//
CString CSharedNet::SanitizeFormatIn(CString text)
{
    char quoteX[2];
    quoteX[0] = '"';
    quoteX[1] = 0;
	text.Replace("'",quoteX);

    return text;
}
//------------------------------------------------------------------------------------//
void CSharedNet::SendInventoryBlockToLootDump(CInventoryObjList * inventory,DPNID sendTo)
{
         int itemcount = 0;
			 LOOTBLOCK_DATA lootBlockData;
			 int headerSize                  = sizeof(LOOTBLOCK_DATA); 
			 lootBlockData.m_catagorie       = 29;
			 lootBlockData.m_menuLink        = 54;//also menu visual id
    
             if(inventory)
			  for(POSITION invPos = inventory->GetHeadPosition();invPos != NULL;)
			  {//inventory loop
                 CInventoryObj * invPtr = (CInventoryObj *)inventory->GetNext(invPos);
				  if(!invPtr->m_itemData->m_permanent && !invPtr->m_itemData->m_isInQuickSlots)
					  itemcount++;
			  }//end inventory loop
 
			 int messageSize = headerSize + (sizeof(ITEMDATA_STRUCT)*itemcount);
             BYTE * dataBlock = new BYTE[messageSize];
			 CopyMemory(dataBlock,&lootBlockData,sizeof(LOOTBLOCK_DATA));

			 if(itemcount>0)
			 {//send items
			   ITEMDATA_STRUCT itemStruct;
			   int trace = 0;
			   
			   if(inventory)
			   {//valid inventory
			    for(POSITION invPos = inventory->GetHeadPosition();invPos != NULL;)
				{//inventory loop
                 CInventoryObj * invPtr = (CInventoryObj *)inventory->GetNext(invPos);
				  if(!invPtr->m_itemData->m_permanent && !invPtr->m_itemData->m_isInQuickSlots)
				  {
				      itemStruct.m_GLID    = invPtr->m_itemData->m_globalID;
					  itemStruct.m_quantity = invPtr->m_quanity;
					  itemStruct.m_dMod    = invPtr->m_itemData->m_damageModifier;
				 
					  CopyMemory(&dataBlock[sizeof(LOOTBLOCK_DATA)+(trace*sizeof(ITEMDATA_STRUCT))],&itemStruct,sizeof(ITEMDATA_STRUCT));
					  trace++;
				  }
				}//end inventory loop
			   }//end valid inventory
			 }//end send items
			 m_netComObject->SendData((BYTE *)  dataBlock,
												messageSize,
												DPNSEND_GUARANTEED,
												sendTo,
												m_netComObject->m_packetTimeout);
			 if (dataBlock)
			 {
		         delete [] dataBlock;
		         dataBlock=0;
			 }
}
//---------------------------------------------------------------------------------------------
void CSharedNet::InterpolateNetworkEntiesServerSide(CPlayerObject * pData,DWORD & CurrentTickCount)
{
	
	  DWORD networkInterp        = pData->m_interpolationVar;
	  DWORD interpolationCurStep = (CurrentTickCount - pData->m_timeLastUpdated);
	  if((CurrentTickCount - pData->m_interpUpdateTemp) < 10)
	      return;

	  pData->m_interpUpdateTemp = CurrentTickCount;
      
	  if(interpolationCurStep > networkInterp)
	     interpolationCurStep = networkInterp;
      
	  pData->m_3dPosition  = m_cabLib.GL_matrixFunctions.InterpolatePoints(pData->m_Last3dPosition, 
						                                 pData->m_Target3dPosition,
					                                     networkInterp, 
					                                     interpolationCurStep);
     
      pData->m_dirVector   = m_cabLib.GL_matrixFunctions.InterpolatePoints(pData->m_LastDirVector, 
						                                  pData->m_TargetDirVector,
					                                      networkInterp, 
					                                      interpolationCurStep);
      
	  pData->m_upVector    = m_cabLib.GL_matrixFunctions.InterpolatePoints(
		                                                  pData->m_LastUpVector, 
						                                  pData->m_TargetUpVector,
					                                      networkInterp, 
					                                      interpolationCurStep);
	  

	  pData->m_upVector = m_cabLib.GL_matrixFunctions.Normalize(pData->m_upVector);
	  pData->m_dirVector = m_cabLib.GL_matrixFunctions.Normalize(pData->m_dirVector);
}
//------------------------------------------------------------------------------------//
BOOL CSharedNet::SendMsgByHandle(   int handle,
									CString message,
									int type)//0 radius = to all
{
   
   CPlayerObject * lstObj = m_netComObject->m_runtimePlayersOnServer->GetObjectByHandle(handle);
	
   if(lstObj)
   {
	   if(lstObj->m_aiControlled)
	      return FALSE;

	   if(lstObj->m_currentID == 0)
		  return FALSE;
         
	   if(lstObj->m_handle == handle)
		  SendTextMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_LOW, 
					  lstObj->m_currentID,
					  message,
					  lstObj->m_networkAssignedID,
					  lstObj,
					  type);

	   return TRUE;
   }
   return FALSE;
}

//------------------------------------------------------------------------------------//
BOOL CSharedNet::UpdatePhysicsBlock(CPlayerObject * playerObject)
{
    if(!playerObject)
		return FALSE;

	if(!playerObject->m_currentInventory)
	   return FALSE;

	if(!playerObject->m_phyRef)
		return FALSE;

	CMovementCaps * phyPtr = (CMovementCaps *)playerObject->m_phyRef;

    //13 floats
	
	int messageSize = (sizeof(float)*19)+1;
	BYTE * dataPacket = new BYTE[messageSize];
	ZeroMemory(dataPacket,messageSize);
    
	int trace = 0;
	float currentBonus = 0.0f;
	dataPacket[trace++] = 7;
	

	currentBonus = ((float)playerObject->m_currentInventory->GetStatBenefit(1001))+phyPtr->List_maxSpeed;
	float maxSpeedCalculated = currentBonus;
	CopyMemory(&dataPacket[trace],&currentBonus,sizeof(float)); // 1
	trace += sizeof(float);

	currentBonus = ((float)playerObject->m_currentInventory->GetStatBenefit(1002))+phyPtr->List_minSpeed;
	if(maxSpeedCalculated > 0.0f && currentBonus > -maxSpeedCalculated * 0.85f)
		currentBonus = -maxSpeedCalculated * 0.85f;
	CopyMemory(&dataPacket[trace],&currentBonus,sizeof(float)); // 2
	trace += sizeof(float);

	currentBonus = ((float)playerObject->m_currentInventory->GetStatBenefit(1003))+phyPtr->List_accellerate;
	CopyMemory(&dataPacket[trace],&currentBonus,sizeof(float)); // 3
	trace += sizeof(float);

	currentBonus = ((float)playerObject->m_currentInventory->GetStatBenefit(1004))+phyPtr->List_decellerate;
	CopyMemory(&dataPacket[trace],&currentBonus,sizeof(float)); //4 
	trace += sizeof(float);

	currentBonus = ((float)playerObject->m_currentInventory->GetStatBenefit(1005))+phyPtr->List_jumpPower;
	CopyMemory(&dataPacket[trace],&currentBonus,sizeof(float)); //5
	trace += sizeof(float);

	currentBonus = ((float)playerObject->m_currentInventory->GetStatBenefit(1006))+phyPtr->m_strafeReduction;
	CopyMemory(&dataPacket[trace],&currentBonus,sizeof(float));//Strafe Degrade //6
	trace += sizeof(float);

	currentBonus = ((float)playerObject->m_currentInventory->GetStatBenefit(1007))+phyPtr->List_groundDrag;
	CopyMemory(&dataPacket[trace],&currentBonus,sizeof(float));//Ground Grip //7
	trace += sizeof(float);

	currentBonus = ((float)playerObject->m_currentInventory->GetStatBenefit(1008))+phyPtr->m_crossSectionalArea;
	CopyMemory(&dataPacket[trace],&currentBonus,sizeof(float));//Aero Dynamic //8
	trace += sizeof(float);

	currentBonus = ((float)playerObject->m_currentInventory->GetStatBenefit(1010))+phyPtr->List_mass;
	CopyMemory(&dataPacket[trace],&currentBonus,sizeof(float));//Mass //10
	trace += sizeof(float);

	currentBonus = ((float)playerObject->m_currentInventory->GetStatBenefit(1011))+phyPtr->m_inertia;
	CopyMemory(&dataPacket[trace],&currentBonus,sizeof(float));//Inertia //11
	trace += sizeof(float);

	currentBonus = ((float)playerObject->m_currentInventory->GetStatBenefit(1012))+phyPtr->m_corneringStiffnessR;
	CopyMemory(&dataPacket[trace],&currentBonus,sizeof(float));//crnStiffnessR //12
	trace += sizeof(float);

	currentBonus = ((float)playerObject->m_currentInventory->GetStatBenefit(1013))+phyPtr->m_corneringStiffnessF;
	CopyMemory(&dataPacket[trace],&currentBonus,sizeof(float));//crnStiffnessF //13
	trace += sizeof(float);

	currentBonus = ((float)playerObject->m_currentInventory->GetStatBenefit(1014))+phyPtr->m_maxGrip;
	CopyMemory(&dataPacket[trace],&currentBonus,sizeof(float));//max Grip //14
	trace += sizeof(float);

	currentBonus = (float)(playerObject->m_currentInventory->GetStatBenefit(1015)+phyPtr->m_jumpRecoverTime);
	CopyMemory(&dataPacket[trace],&currentBonus,sizeof(float));//jump delay time //15
	trace += sizeof(float);

	currentBonus = ((float)playerObject->m_currentInventory->GetStatBenefit(1016))+phyPtr->m_airControl;
	CopyMemory(&dataPacket[trace],&currentBonus,sizeof(float));//air Control //16
	trace += sizeof(float);

	currentBonus = ((float)playerObject->m_currentInventory->GetStatBenefit(1017))+phyPtr->List_maxYaw;
	CopyMemory(&dataPacket[trace],&currentBonus,sizeof(float));//max yaw //17
	trace += sizeof(float);

	currentBonus = ((float)playerObject->m_currentInventory->GetStatBenefit(1018))+phyPtr->List_maxPitch;
	CopyMemory(&dataPacket[trace],&currentBonus,sizeof(float));//max pictch //18
	trace += sizeof(float);

	currentBonus = ((float)playerObject->m_currentInventory->GetStatBenefit(1019))+phyPtr->m_inLiquidMotionModifier;
	CopyMemory(&dataPacket[trace],&currentBonus,sizeof(float));//liquid modifier//19
	trace += sizeof(float);

	//Set Speed rating
	playerObject->m_speedRating = 1500.0f * maxSpeedCalculated;
	if(playerObject->m_speedRating < 2.0f)
	   playerObject->m_speedRating = 50.0f;

	if(playerObject->m_miscOutBox->GetCount() < 20)
	playerObject->m_miscOutBox->AddPacket(messageSize,(BYTE*)dataPacket,0,playerObject->m_currentWorldID);

	delete [] dataPacket;
	dataPacket = 0;

	return TRUE;
}


void CSharedNet::MovePlayerInWorld(CPlayerObject * currentPlayer, float x, float y, float z)
{
	// Define the New Position in the current world
	D3DVECTOR newPosition = {x, y, z};

	//SpawnToPointInWld(currentPlayer, newPosition, currentPlayer->m_currentWorldID);
}

void CSharedNet::RR_MovePlayerByOffset(CPlayerObject * currentPlayer, float xOffset, float yOffset, float zOffset)
{
	// Define the new position based of an offset
	D3DVECTOR newPosition = {currentPlayer->m_3dPosition.x + xOffset, currentPlayer->m_3dPosition.y + yOffset, currentPlayer->m_3dPosition.z + zOffset};
	
	// Move the Player
	//SpawnToPointInWld(currentPlayer, newPosition, currentPlayer->m_currentWorldID);
}

BOOL CSharedNet::SpawnZone_async(CDBConnectionSystem * hubConnection,CString exgName,int typeID,int & newChannel)
{
	CString requestWorldStencil = "SELECT * FROM SYSWORLDSTENCILS WHERE EXGNAME = '$EXGNAME'";
    //aquire world data from sql
	requestWorldStencil.Replace("$EXGNAME",exgName);
    _RecordsetPtr wldStencil = hubConnection->Query(requestWorldStencil);
    int stencilCount = hubConnection->GetCount(wldStencil);
    if(stencilCount > 0)
	{//valid stencil request
        CString maxConnections			= hubConnection->GetStringData("maxConnections",wldStencil);
		CString aiExgName				= hubConnection->GetStringData("aiExgName",wldStencil);
		int channelClone				= hubConnection->GetIntData("channel",wldStencil);//this is what we base npc / world rules on

		m_eventLog->LogByPriority("Router:channelAquireCreep",2);
		newChannel = m_netComObject->m_currentWorldsUsed->GetOpenChannel();//rewrite this function and index the world list class
		m_eventLog->LogByPriority("Router:OpenChannelAquired:",(DWORD)newChannel,2);
		BOOL zoneAddResult = m_netComObject->m_currentWorldsUsed->AddZone_safe(exgName,newChannel,typeID,aiExgName,maxConnections,channelClone);
		if(zoneAddResult)
		{
			m_eventLog->LogByPriority("Router:AddZone_safe success",2);
			m_serverItemGenerator->CloneChannelToNewChannel_safe(channelClone, newChannel);
			m_eventLog->LogByPriority("Router:CloneChannelToNewChannel",2);
			//send ai zone creation
			BOOL aiZoneRequestResult		= SpawnAIChannel(newChannel,aiExgName);
			if(!aiZoneRequestResult)
				m_eventLog->AddEvent("***Zone request failed to send to ai server***:",aiExgName);
		}
		else
			m_eventLog->AddEvent("Router:AddZone_safe failed");
		
	    
		hubConnection->Close(wldStencil);
		return TRUE;
	}
	else
	{
		m_eventLog->LogByPriority("Router:ERROR cannot find stencil world:"+exgName,2);
	}
	if(wldStencil)
	   hubConnection->Close(wldStencil);
	return FALSE;
}

BOOL CSharedNet::EnablePendingZone_async(CDBConnectionSystem * hubConnection,CString exgName,int instanceType,int * channelReturn,CString & dynRules)
{
	    //ZONENAME, CHANNEL, NODEID, INSTANCETYPE, MAX_CONNECTIONS, PENDING, AIZONENAME, STENCILCHANNEL
	    CString instanceTypeString;
	    instanceTypeString.Format("%i",instanceType);
	    CString requestPendingZone = "SELECT * FROM SYSLIVEZONEDB WHERE ZONENAME = '"+exgName+"' AND INSTANCETYPE = '"+instanceTypeString+"' AND NODEID = '"+m_netComObject->m_currentWorldsUsed->m_nodeID+"' AND PENDING = '1'";
		_RecordsetPtr pendingZoneRcd = hubConnection->Query(requestPendingZone);
        int znCount = hubConnection->GetCount(pendingZoneRcd);
        if(znCount > 0)
	    {//valid stencil request
			hubConnection->Close(pendingZoneRcd);
			CString requestWorldStencil = "SELECT * FROM SYSWORLDSTENCILS WHERE EXGNAME = '$EXGNAME'";
			requestWorldStencil.Replace("$EXGNAME",exgName);
	        _RecordsetPtr wldStencil = hubConnection->Query(requestWorldStencil);
            int wsCount = hubConnection->GetCount(wldStencil);
			if(wsCount > 0)
			{
				int stencilChannel				= hubConnection->GetIntData("channel",wldStencil);
				CString aiExgName				= hubConnection->GetStringData("aiExgName",wldStencil);
				CString maxConnections			= hubConnection->GetStringData("maxConnections",wldStencil);
	            
				m_eventLog->LogByPriority("EnablePendingZone_async:channelAquireCreep",2);
				int dynChannel = m_netComObject->m_currentWorldsUsed->GetOpenChannel();//rewrite this function and index the world list class
				m_eventLog->LogByPriority("EnablePendingZone_async:GetOpenChannel",2);
				//channel is dynamically created here
				m_netComObject->m_currentWorldsUsed->MakePendingZoneLive_safe(exgName,dynChannel,instanceType,aiExgName,maxConnections,stencilChannel);
				m_eventLog->LogByPriority("EnablePendingZone_async:AddZone_safe",2);
				m_serverItemGenerator->CloneChannelToNewChannel_safe(stencilChannel, dynChannel);
				m_eventLog->LogByPriority("EnablePendingZone_async:CloneChannelToNewChannel",2);
				
                if(channelReturn)
				   *channelReturn = dynChannel;
				//parse dynamic rules
				if(dynRules.GetLength() > 2)
				{
				   char * buf = dynRules.GetBuffer();
				   CString tempBuild;
				   int sc = 0;
				   int ruleId = 0;
                   m_eventLog->LogByPriority("EnablePendingZone_async dynamic Rules Exist:"+dynRules,2);
				   for(int tri = 0;tri < dynRules.GetLength();tri++)
				   {
                       if(buf[tri] == ',')
					   {
					      if(sc == 0)
						  {
						     ruleId = atoi(tempBuild);
							 sc = 1;
						  }
						  else if(tempBuild.GetLength() > 0)
						  {
                             CWordlChannelRuleObj * newRule = new CWordlChannelRuleObj();

							 newRule->m_primaryKey						= m_worldChannelRuleDB->GetUniqueKey();
							 newRule->m_rule							= ruleId;
							 newRule->m_wldID							= dynChannel;
							 newRule->m_param1							= atoi(tempBuild);

							 m_eventLog->LogByPriority("EnablePendingZone_async Aquired Rule Key:",(DWORD) newRule->m_primaryKey,2);
							 m_eventLog->LogByPriority("EnablePendingZone_async Added Rule ID:",(DWORD)newRule->m_rule,2);
							 m_eventLog->LogByPriority("EnablePendingZone_async Added Rule Param:",(DWORD)newRule->m_param1,2);
							 m_worldChannelRuleDB->Add_safe(newRule);
                             sc = 0;
						  }
						  tempBuild = "";
						  continue;
					   }

					   tempBuild += buf[tri];
				   }
				}
				//send ai zone creation
				BOOL aiZoneRequestResult		= SpawnAIChannel(dynChannel,aiExgName);
				if(!aiZoneRequestResult)
					m_eventLog->AddEvent("***Zone request failed to send to ai server***:",(DWORD)dynChannel);
				//see if any start rules exist
				DWORD param1 = 0;
				if(m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(stencilChannel,dynChannel,WORLDRULE_RUNSCRIPTONOPEN,&param1))
				{
				   CString errorReturn;
                   if(!m_serverScripts->ScriptProcessor(&dynChannel,sizeof(int),param1,errorReturn,this))
				   {
                       m_eventLog->LogByPriority("EnablePendingZone_async():ScriptProcessor Failed:"+errorReturn, 2);
				   }
				}
			}
			else
				m_eventLog->LogByPriority("EnablePendingZone_async-could not aquire stencil world:"+exgName,2);

			hubConnection->Close(wldStencil);
			return TRUE;
		}
		else
			m_eventLog->AddEvent("***EnablePendingZone_async error could not find zone -exg:"+exgName+" node:"+m_netComObject->m_currentWorldsUsed->m_nodeID);
	
		hubConnection->Close(pendingZoneRcd);
   return FALSE;
}
//this is a very passive only loop for the time it hasend been called for every miliselcond it will loop once and deal with one item
void CSharedNet::WorldRuleTimeCallback_safe(int searchCount)
{
	 //is Threadsafe
     DWORD param1 = 0;
	 DWORD currentTime = GetTickCount();
	 DWORD toLoopCount = searchCount;//prevents allot looping, should only loop one turn per millisecond, in full server it will probably process one per turn-idle could be 100s   but this scales per cpu
	
	 while(toLoopCount>0)
	 {//AGGRAGATED LOOP
		 if(m_worldRuleTrace >= m_netComObject->m_currentWorldsUsed->m_cellCount)
			m_worldRuleTrace = 0;
	    
		 m_netComObject->m_currentWorldsUsed->Lock();
		 for(CAxLinkObj * pos = m_netComObject->m_currentWorldsUsed->m_listArray[0].m_axList[m_worldRuleTrace].GetHeadPosition_unsafe();pos != NULL;)
		 {
			 CWorldAvailableObj * wPtr = (CWorldAvailableObj *) m_netComObject->m_currentWorldsUsed->m_listArray[0].m_axList[m_worldRuleTrace].GetNext_unsafe(&pos);
	         
			 if(m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(wPtr->m_channelRef,wPtr->m_id,WORLDRULE_TIMELIMIT,&param1))
			 {//has a time limit
				if(currentTime-wPtr->m_creationTimeStamp > param1)
				{//zone has timed out
					//m_threadedDataProcessor->AddToQue(wPtr->m_id,wPtr->m_channelRef,32,0);//process closing rules
                    if(m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(wPtr->m_channelRef,wPtr->m_id,WORLDRULE_DEATHMATCH))
					{
						if(!wPtr->m_arenaEndSent)
						{ 
							wPtr->m_arenaEndSent = TRUE;
							m_threadedDataProcessor->AddToQue(wPtr->m_id,wPtr->m_channelRef,36,0);
							m_threadedDataProcessor->AddToQue(wPtr->m_id,wPtr->m_channelRef,33,0);//call deathmath close with proper locking order
						}
					}
					else if(m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(wPtr->m_channelRef,wPtr->m_id,WORLDRULE_TEAM_DEATHMATCH))
					{
						if(!wPtr->m_arenaEndSent)
						{
							wPtr->m_arenaEndSent = TRUE;
							m_threadedDataProcessor->AddToQue(wPtr->m_id,wPtr->m_channelRef,36,0);
							m_threadedDataProcessor->AddToQue(wPtr->m_id,wPtr->m_channelRef,34,0);//call deathmath close with proper locking order
						}
					}
					else
					{
						m_threadedDataProcessor->AddToQue(wPtr->m_id,wPtr->m_channelRef,36,0);
						m_threadedDataProcessor->AddToQue(wPtr->m_id,wPtr->m_channelRef,30,0);//remove players from zone with proper locking order
					}
				}//end zone has timed out
			 }//end has a time limit
			 //room to extend if need be
		 }
		 m_netComObject->m_currentWorldsUsed->Unlock();
		 m_worldRuleTrace++;
		 toLoopCount--;
	 }
	 m_worldRuleTraceStamp = GetTickCount();
}

//------------------------------------------------------------------//
//copies necessary info to base char on account on death
BOOL CSharedNet::OnDeathDataProcessByDPNID(CPlayerObject * victimPlayerObject,
												BOOL nonDrop,
												CMovementObj * entObjDB,
												float respawnHealthPercentage,
												void * m_serverScriptsMEM,
												void * m_netComObjectMEM)
{
	 CAxList * obj = m_netComObject->m_runtimeAccountDB->GetAllOnIndex(victimPlayerObject->m_currentID,"dpnid");
     for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
	 {
         CAccountObject * plObj = (CAccountObject *)obj->GetNext_unsafe(&pos);
         if(plObj->m_currentID==victimPlayerObject->m_currentID)
		 if(plObj->m_characterOnAccount)
		 {//found target object
			CServerScriptSystem * serverScripts = (CServerScriptSystem *)m_serverScriptsMEM;
			CMultiplayerObj	    * netComObject = (CMultiplayerObj *)m_netComObjectMEM;
		    
			//update Stats
			plObj->m_characterOnAccount->m_clanHandle			= victimPlayerObject->m_clanHandle;
			plObj->m_characterOnAccount->m_arenaBasePoints		= victimPlayerObject->m_arenaBasePoints;
			plObj->m_characterOnAccount->m_pvpKills				= victimPlayerObject->m_pvpKills;
			plObj->m_characterOnAccount->m_arenaKills			= victimPlayerObject->m_arenaKills;
			plObj->m_characterOnAccount->m_arenaTotalKills		= victimPlayerObject->m_arenaTotalKills;
			plObj->m_characterOnAccount->m_deathCount			= victimPlayerObject->m_deathCount;
			plObj->m_characterOnAccount->m_factionStanding		= victimPlayerObject->m_factionStanding;
			plObj->m_characterOnAccount->m_arenaItemsGiven		= victimPlayerObject->m_arenaItemsGiven;

			//update wounds
			CopyMemory(plObj->m_characterOnAccount->m_woundChannel,victimPlayerObject->m_woundChannel,sizeof(unsigned char)*8);

			//remove mount
			plObj->m_characterOnAccount->m_aiCfgOfMounted		= -1;
			plObj->m_characterOnAccount->m_invunerableTemp		= TRUE;
			plObj->m_characterOnAccount->m_invunerableStamp		= GetTickCount();
			plObj->m_characterOnAccount->m_justDied				= 1;

			//update stats
			if(victimPlayerObject->m_stats)
			{
			   if(plObj->m_characterOnAccount->m_stats)
			   {
				  plObj->m_characterOnAccount->m_stats->SafeDelete();
				  delete plObj->m_characterOnAccount->m_stats;
				  plObj->m_characterOnAccount->m_stats = 0;
			   }
			   victimPlayerObject->m_stats->Clone(&plObj->m_characterOnAccount->m_stats);
			}

			//update noteriety
			if(victimPlayerObject->m_noteriety)
			{
			  if(plObj->m_characterOnAccount->m_noteriety)
			  {
				 delete plObj->m_characterOnAccount->m_noteriety;
				 plObj->m_characterOnAccount->m_noteriety=0;
			  }
			  victimPlayerObject->m_noteriety->Clone(&plObj->m_characterOnAccount->m_noteriety);
			}

			BOOL noDisarm = m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(victimPlayerObject->m_stencilChannel,victimPlayerObject->m_currentWorldID,WORLDRULE_NODISARM);
			m_netComObject->m_runtimeAccountDB->HandlePcInventoryOnDeath(victimPlayerObject,plObj->m_characterOnAccount,entObjDB,nonDrop,noDisarm);

			plObj->m_characterOnAccount->m_energy				= (int)(victimPlayerObject->m_maxEnergy*(respawnHealthPercentage*.01f));//percentage of health to respawn with
			plObj->m_characterOnAccount->m_deathCount			= victimPlayerObject->m_deathCount; // Add a death count
			plObj->m_characterOnAccount->m_visName				= victimPlayerObject->m_charName;
			plObj->m_characterOnAccount->m_charName				= victimPlayerObject->m_charName;
			plObj->m_characterOnAccount->m_originalEDBEnergy	= victimPlayerObject->m_originalEDBEnergy;
			plObj->m_characterOnAccount->m_squadName			= victimPlayerObject->m_squadName;
			plObj->m_characterOnAccount->m_groupID				= victimPlayerObject->m_groupID;

			if(!victimPlayerObject->m_aiControlled)
			   m_netComObject->m_currentWorldsUsed->DecrementZoneConnections_safe(victimPlayerObject->m_currentWorldID);

			//set next spawn position by respawn data
			if(victimPlayerObject->m_arenaRespawnWorldName == "Unknown")
			{
				plObj->m_characterOnAccount->m_currentWorldMap		= plObj->m_characterOnAccount->m_respawnWorldName;
				plObj->m_characterOnAccount->m_3dPosition.x			= plObj->m_characterOnAccount->m_respawnPosition.x;
				plObj->m_characterOnAccount->m_3dPosition.y			= plObj->m_characterOnAccount->m_respawnPosition.y;
				plObj->m_characterOnAccount->m_3dPosition.z			= plObj->m_characterOnAccount->m_respawnPosition.z;
				plObj->m_characterOnAccount->m_teamTemp				= -1;
			}
			else
			{//Set it back into the arena if it calls too
				plObj->m_characterOnAccount->m_currentWorldMap	= victimPlayerObject->m_arenaRespawnWorldName;
				plObj->m_characterOnAccount->m_3dPosition.x		= victimPlayerObject->m_arenaRespawnPosition.x;
				plObj->m_characterOnAccount->m_3dPosition.y		= victimPlayerObject->m_arenaRespawnPosition.y;
				plObj->m_characterOnAccount->m_3dPosition.z		= victimPlayerObject->m_arenaRespawnPosition.z;
				//save out the players arena info also
				for(int i = 0; i < ARENA_TRACKING; i++)
					plObj->m_characterOnAccount->m_arenaInfo[i] = victimPlayerObject->m_arenaInfo[i];
				//Save out the players current team assignment
				plObj->m_characterOnAccount->m_teamTemp			= victimPlayerObject->m_teamTemp;
			}

			

			//see if any scripts exist on death (most races have none - only run/log when one exists)
			CString errorCatch;
			if(serverScripts->GetByUID(1,plObj->m_currentlyPlayingGameName,plObj->m_characterOnAccount->m_curEDBIndex))
			if(!serverScripts->ScriptProcessor(plObj->m_characterOnAccount,sizeof(CPlayerObject),1,errorCatch,plObj->m_currentlyPlayingGameName,plObj->m_characterOnAccount->m_curEDBIndex,this))
				 netComObject->m_eventLog->AddEvent(errorCatch);

			if(!plObj->m_loggedOn)
			   plObj->m_currentcharInUse = -1;
			if(plObj->m_connectedTemp == FALSE)//not connected to game server anymore
			   netComObject->m_characterPendingSaveSystem->AddCharacterToPendingList(plObj->m_characterOnAccount,1,plObj->m_accountName,-1);//just log it out of

		    return TRUE;
		 }//end found target object
	 }
     return FALSE;
}

BOOL CSharedNet::HandleGroupData_safe(unsigned int iGroupID)
{
     this->m_netComObject->m_runtimePlayersOnServer->Lock();
	 BOOL res = HandleGroupData(iGroupID);
	 this->m_netComObject->m_runtimePlayersOnServer->Unlock();

     return res;
}

BOOL CSharedNet::SendSquadData_safe(CPlayerObject *pPlayer)
{
     this->m_netComObject->m_runtimePlayersOnServer->Lock();
	 BOOL bResult = SendSquadData(pPlayer);
	 this->m_netComObject->m_runtimePlayersOnServer->Unlock();
     return bResult;
}

BOOL
CSharedNet::SendSquadData(CPlayerObject *pPlayer)
{
	// Acquire the player list for the given group number
	CAxList *playerList = this->m_netComObject->m_runtimePlayersOnServer->GetListByGroupID(pPlayer->m_groupID); // Already locked at this point
	if(!playerList)
	    return FALSE;

	// PREPARE the group data for the requesting player
	int iDataFiller = 0;
	BOOL bResult = this->PrepareGroupData(playerList, pPlayer->m_groupID, iDataFiller);

	// Make sure we are dealing with only the ones in our group again
	this->SendGroupMessage(pPlayer, m_groupDataBlock, iDataFiller);

	// Release allocated space for character & squad names
	for(int i=0; i<iDataFiller; i++)
	{
		SafeDeleteArray(m_groupDataBlock[i].PlayerName);
		SafeDeleteArray(m_groupDataBlock[i].SquadName);
	}
	return TRUE;
}

BOOL CSharedNet::HandleGroupData(unsigned int iGroupID)
{
	// Acquire the player list for the given group number
	CAxList *playerList = this->m_netComObject->m_runtimePlayersOnServer->GetListByGroupID(iGroupID); // Already locked at this point
	if(!playerList)
	    return FALSE;

	int iDataFiller = 0;
	BOOL bResult = this->PrepareGroupData(playerList, iGroupID, iDataFiller);
	if (!bResult)
	{//nothing changed - still free the names PrepareGroupData made (leaked every tick a group existed)
		for(int i=0; i<iDataFiller; i++)
		{
			SafeDeleteArray(m_groupDataBlock[i].PlayerName);
			SafeDeleteArray(m_groupDataBlock[i].SquadName);
		}
		return FALSE;
	}

	// Send the update to each player in the group
	for(CAxLinkObj *posLoc = playerList->GetHeadPosition_unsafe(); posLoc != NULL;)
	{
		// Make sure we are dealing with only the ones in our group again
		CPlayerObject *player = (CPlayerObject *)playerList->GetNext_unsafe(&posLoc);
		if(player->m_groupID == iGroupID)
			this->SendGroupMessage(player, m_groupDataBlock, iDataFiller);
	}

	// Release allocated space for character & squad names
	for(int i=0; i<iDataFiller; i++)
	{
		SafeDeleteArray(m_groupDataBlock[i].PlayerName);
		SafeDeleteArray(m_groupDataBlock[i].SquadName);
	}
	return TRUE;
}

BOOL
CSharedNet::PrepareGroupData(CAxList *playerList, unsigned int iGroupID, int &iDataFiller)
{
	bool bIsDirty = false;
	if(iGroupID < 1) return FALSE;

	// Get the time for update
	DWORD newTimeUpdate = GetTickCount();
	
	//If we have a player list, start checking for update
	if(!playerList)
		return iDataFiller;
	
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
		
			// ENCODE basic squad data
			m_groupDataBlock[iDataFiller].NetID = player->m_networkAssignedID;
			m_groupDataBlock[iDataFiller].Health = (unsigned char)(player->m_energy * 100 / player->m_maxEnergy);
			m_groupDataBlock[iDataFiller].Position = player->m_groupStruct.m_groupPlacementIndex;

			// ENCODE squad Player's name
			m_groupDataBlock[iDataFiller].PlayerNameLength = player->m_charName.GetLength() + 1; // +1 for null-terminator
			m_groupDataBlock[iDataFiller].PlayerName = new char[m_groupDataBlock[iDataFiller].PlayerNameLength];
			strcpy_s( m_groupDataBlock[iDataFiller].PlayerName, 
				m_groupDataBlock[iDataFiller].PlayerNameLength, 
				player->m_charName.GetBuffer());
			player->m_charName.ReleaseBuffer();	

			// ENCODE squad name
			m_groupDataBlock[iDataFiller].SquadNameLength = player->m_squadName.GetLength() + 1; // +1 for null-terminator
			m_groupDataBlock[iDataFiller].SquadName = new char[m_groupDataBlock[iDataFiller].SquadNameLength];
			strcpy_s( m_groupDataBlock[iDataFiller].SquadName, 
				m_groupDataBlock[iDataFiller].SquadNameLength, 
				player->m_squadName.GetBuffer());
			player->m_squadName.ReleaseBuffer();	

			// UPDATE misc squad info
			player->m_groupStruct.m_squadDataIsDirty = 0; // Make sure we uncheck that is info is dirty so it doesn't get updated on the next pass
			player->m_lastGroupUpdate = newTimeUpdate; // Update their timer
			iDataFiller++; // Increment the Filler

			// Check to see if we have the max amount in the datablock
			if(iDataFiller == SQUAD_MESSAGE_BURST_SIZE)
				break;
		}
	}

	// Nothing in need of an update
	if(!bIsDirty || !iDataFiller)
		return 0;
	return iDataFiller;
}