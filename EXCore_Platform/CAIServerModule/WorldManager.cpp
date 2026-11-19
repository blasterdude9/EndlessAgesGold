#include "stdafx.h"
#include "worldmanager.h"

CWorldManager::CWorldManager(CAIServerInitialization * aiserverInit,int aiSvrID)
{
	InitializeCriticalSection(  &m_criticalSec );
	InitializeCriticalSection(  &m_statusSec );
	
	m_curSpawnGenPos					= NULL;
    m_wldObjList						= NULL;
	m_spawnGeneratorDB					= NULL;
	m_collisionBase					    = NULL;
	m_worldGroupList					= NULL;
	m_worldChannel                      = aiSvrID;
	
	m_libraryReferenceDB				= NULL;
	m_waterZoneDB						= NULL;
	m_aiWebDatabase						= NULL;
	m_collisionDatabase                 = NULL;
	m_axThreadID						= NULL;
	m_axThreadHandle					= NULL;
	m_aiServerBaseRef                   = aiserverInit;
	m_warmUpStamp						= GetTickCount();
    m_entObjRuntime                     = new CMovementObjList();
	m_entObjRuntime->EnableDirectAccess(33000);
	m_missileRuntime                    = new CMissileObjList();
	m_explosionRuntimeList              = new CExplosionObjList();
	m_inBox                             = new CPacketObjList();
	m_inBox->EnableCriticalSectionUse();
	
	m_shutDownChannel                   = FALSE;
	m_aiCycleDuration                   = 250;
	m_glAdjustFloat                     = 1.0f;
	m_glAdjust                          = 1.0;
	QuadPartRef							= m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->GL_frequency.QuadPart;
	m_botCount                          = 0;
	CallBackTimeOpen();
	m_updateNetworkStamp                = m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->GetTimeStamp_DW();
    m_spawnTrace                        = 0;
	m_spawnGenFreq                      = 3000;
    m_spawnGenStamp                     = GetTickCount();
}

CWorldManager::~CWorldManager(void)
{
	if(m_inBox)
	{
	   delete m_inBox;
	   m_inBox = 0;
	}

	if(m_entObjRuntime)
	{
       m_entObjRuntime->SafeDelete();
	   delete m_entObjRuntime;
	   m_entObjRuntime = 0;
	}
	if(m_aiWebDatabase)
	{ 
       m_aiWebDatabase->SafeDelete();
	   delete m_aiWebDatabase;
	   m_aiWebDatabase = 0;
	}

    if(m_spawnGeneratorDB)
	{ 
       m_spawnGeneratorDB->SafeDelete();
	   delete m_spawnGeneratorDB;
	   m_spawnGeneratorDB = 0;
	}

	if(m_libraryReferenceDB)
	{ 
       m_libraryReferenceDB->SafeDelete();
	   delete m_libraryReferenceDB;
	   m_libraryReferenceDB = 0;
	}
	
    if(m_wldObjList)
	{ 
       m_wldObjList->SafeDelete();
	   delete m_wldObjList;
	   m_wldObjList = 0;
	}

	if(m_collisionBase)
	{ 
       m_collisionBase->SafeDelete();
	   delete m_collisionBase;
	   m_collisionBase = 0;
	}
	if(m_worldGroupList)
	{ 
       m_worldGroupList->SafeDelete();
	   delete m_worldGroupList;
	   m_worldGroupList = 0;
	}
    
	if(m_waterZoneDB)
	{ 
       m_waterZoneDB->SafeDelete();
	   delete m_waterZoneDB;
	   m_waterZoneDB = 0;
	}

	if(m_collisionDatabase)
	{
       m_collisionDatabase->SafeDelete();
	   delete m_collisionDatabase;
	   m_collisionDatabase = 0;
	}

	if(m_missileRuntime)
	{
       m_missileRuntime->SafeDelete();
	   delete m_missileRuntime;
	   m_missileRuntime = 0;
	}

	if(m_explosionRuntimeList)
	{
       m_explosionRuntimeList->SafeDelete();
	   delete m_explosionRuntimeList;
	   m_explosionRuntimeList = 0;
	}
    
	if(m_axThreadHandle)
	{
       CloseHandle(m_axThreadHandle);
       m_axThreadHandle = NULL;
	}

	   
	DeleteCriticalSection(  &m_criticalSec );
	DeleteCriticalSection(  &m_statusSec );
}



BOOL CWorldManager::InitWorldLocalData(CString worldIdentifierName,CString altCollisionFile)
{
	if(altCollisionFile.GetLength()>0)
	{//alt collision
	 CFileException exc;
     CFile fl;
     if(!fl.Open(m_aiServerBaseRef->m_dataPath+altCollisionFile,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_collisionDatabase)
	 {
		m_collisionDatabase->SafeDelete();
	    delete m_collisionDatabase;
	    m_collisionDatabase = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_collisionDatabase;
	 the_in_Archive.Close();
     fl.Close();
	 m_aiServerBaseRef->m_eventLog->AddEvent("AI Alt Collision world file parsed successfully:",altCollisionFile);
	}//end alt collision
     CFileException exc;
     CFile fl;
     BOOL res = fl.Open(m_aiServerBaseRef->m_dataPath+worldIdentifierName,CFile::modeRead | CFile::shareDenyWrite, &exc);
	 if(res == FALSE){
		 m_aiServerBaseRef->m_eventLog->AddEvent("Cannot find AI world file:", worldIdentifierName);
		return FALSE;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);

	 m_worldName = worldIdentifierName;//maps spawn channel

	 int                          currentTrack      = 0;
	 int                          globalShader      = 0;
	 BOOL                         wBufferPrefer     = 0;
	 CString                      reserved;
	 CString					  musicLoopFile;
	
	 
	 CBackGroundObjList         * backGroundList	= NULL;
	 CEnvironmentObj            * environmentPtr	= NULL;
	 CPersistObjList            * scenePersistList	= NULL;
	 CEXScriptObjList           * sceneScriptExec   = NULL;
	 if(m_aiWebDatabase)
	 { 
       m_aiWebDatabase->SafeDelete();
	   delete m_aiWebDatabase;
	   m_aiWebDatabase = 0;
	 }
	 
     if(m_wldObjList)
	 {
	    m_wldObjList->SafeDelete();
        delete m_wldObjList;
		m_wldObjList = 0;
	 }
	 if(m_spawnGeneratorDB)
	 {
	    m_spawnGeneratorDB->SafeDelete();
        delete m_spawnGeneratorDB;
		m_spawnGeneratorDB = 0;
	 }
	 if(m_collisionBase)
	 {
	    m_collisionBase->SafeDelete();
        delete m_collisionBase;
		m_collisionBase = 0;
	 }
	 if(m_worldGroupList)
	 {
	    m_worldGroupList->SafeDelete();
        delete m_worldGroupList;
		m_worldGroupList = 0;
	 }
	 if(m_libraryReferenceDB)
	 {
	    m_libraryReferenceDB->SafeDelete();
        delete m_libraryReferenceDB;
		m_libraryReferenceDB = 0;
	 }
	 if(m_waterZoneDB)
	 {
	    m_waterZoneDB->SafeDelete();
        delete m_waterZoneDB;
		m_waterZoneDB = 0;
	 }

	 

	 the_in_Archive >> m_wldObjList 
		            >> backGroundList 
					>> environmentPtr
					>> scenePersistList
					>> currentTrack
					>> musicLoopFile
					>> globalShader
				    >> wBufferPrefer
					>> sceneScriptExec
					>> reserved
					>> m_spawnGeneratorDB
					>> m_collisionBase
                    >> m_worldGroupList
					>> m_aiWebDatabase
					>> m_libraryReferenceDB
	                >> m_waterZoneDB;

	 if(m_collisionBase && m_wldObjList)
	    m_collisionBase->BuildObjectReferenceArray(m_wldObjList);
	 if(m_spawnGeneratorDB)
        m_spawnGeneratorDB->GetTotalAIOutput();

	 if(m_wldObjList)
	    m_wldObjList->CacheUnimediateData();

     m_entObjRuntime->m_idStartIndex = 5000;

	 m_aiServerBaseRef->m_eventLog->AddEvent("AI world file parsed successfully:",worldIdentifierName);
	 if(m_spawnGeneratorDB)
	    m_aiServerBaseRef->m_eventLog->AddEvent("SpawnGen Count:",(DWORD)m_spawnGeneratorDB->GetCount());
	 else 
        m_aiServerBaseRef->m_eventLog->AddEvent("No spawngens found");

	 if(m_waterZoneDB)
	    m_aiServerBaseRef->m_eventLog->AddEvent("Waterzone Count:",(DWORD)m_waterZoneDB->GetCount());
     if(m_aiWebDatabase)
	 {//display tree count also verify and report errors that exist
	    m_aiServerBaseRef->m_eventLog->AddEvent("AI web count:",(DWORD)m_aiWebDatabase->GetCount());
        int treeIndex = 0;
		for(POSITION aiPos = m_aiWebDatabase->GetHeadPosition();aiPos != NULL;)
		{
            CAIWebObj * aiWeb = (CAIWebObj *)m_aiWebDatabase->GetNext(aiPos);
			if(aiWeb->m_web)
            for(POSITION wPos = aiWeb->m_web->GetHeadPosition();wPos != NULL;)
			{
	            CAIWebNode * locationNode = (CAIWebNode *)aiWeb->m_web->GetNext(wPos);
                if(!locationNode->m_routeToAllFromLocationList)
				{//unlcosed tree
					m_aiServerBaseRef->m_eventLog->AddEvent("***Uncompiled Tree***:",(DWORD)treeIndex);

				   break;
				}//end unclosed tree
			}
			treeIndex++;
		}
	 }//end display tree count also verify and report errors that exist

	 if(backGroundList)
	 {
        backGroundList->SafeDelete();
		delete backGroundList;
		backGroundList = 0;
	 }
	 if(environmentPtr)
	 {
        environmentPtr->SafeDelete();
		delete environmentPtr;
		environmentPtr = 0;
	 }
	 if(scenePersistList)
	 {
        scenePersistList->SafeDelete();
		delete scenePersistList;
		scenePersistList = 0;
	 }
	 if(sceneScriptExec)
	 {
        sceneScriptExec->SafeDelete();
		delete sceneScriptExec;
		sceneScriptExec = 0;
	 }

	 the_in_Archive.Close();
     fl.Close();

	 m_warmUpStamp = GetTickCount();


	return TRUE;
}


int CWorldManager::RunWorld()
{
	CallBackTimeCap();
    CallBackTimeOpen();

	DWORD currentTime = GetTickCount();
	//SpawnGeneratorCallback(currentTime);
	//HandleAIDeadEntities(currentTime);
	AIControlUnit();
	m_aiServerBaseRef->m_clientModule->m_initObject->m_clientNet->ManageSmoothMovementOnNetworkEntities(currentTime,m_entObjRuntime);
	m_aiServerBaseRef->m_clientModule->m_initObject->m_misExpManager->MissileCallback(  m_missileRuntime,
																						m_explosionRuntimeList,
																						m_glAdjustFloat,
												                                        m_lastGLAdjust);
	m_aiServerBaseRef->m_clientModule->m_initObject->m_misExpManager->ExplosionCallback(m_entObjRuntime,m_explosionRuntimeList);
	m_aiServerBaseRef->m_clientModule->m_initObject->m_entityManager->CollisionPipeline(&m_mathLib,m_entObjRuntime,m_missileRuntime);
	m_aiServerBaseRef->m_clientModule->m_initObject->m_entityManager->PossesCallback(m_entObjRuntime);
	//process in box
	ProcessPackets();
	SendAIStateToServerCallback(currentTime);
	m_aiServerBaseRef->m_clientModule->m_initObject->m_entityManager->GenerateEntityLastPosition(m_entObjRuntime);
	m_aiServerBaseRef->m_clientModule->m_initObject->m_entityManager->EntityDamagePathCheck(m_entObjRuntime);
	m_aiServerBaseRef->m_clientModule->m_initObject->m_misExpManager->MissileCollisionPipeline(m_entObjRuntime,m_missileRuntime,m_explosionRuntimeList,m_collisionBase);
	m_aiServerBaseRef->m_clientModule->m_initObject->m_misExpManager->ExplosionCollisionCallback(m_entObjRuntime,m_explosionRuntimeList);
	m_aiServerBaseRef->m_clientModule->m_initObject->m_misExpManager->UpdateRuntimeMissileLastPosition(m_missileRuntime);
	

   return 0;
}
//------------------Process packets Callback-----------------------------------------//
void CWorldManager::ProcessPackets()
{
	int processcount = 0;
    int size;
	BYTE * data;
	DPNID id;

	while(m_inBox->GetNextPacket_safe(&size,&data,&id))
	{
	      m_aiServerBaseRef->m_clientModule->m_initObject->m_clientNet->HandleEngineMessagesClientSide(data,
			                                                                                           id,
																									   size,
																									   m_spawnGeneratorDB,
																									   m_entObjRuntime,
																									   m_missileRuntime,
																									   m_explosionRuntimeList,
																									   m_collisionBase,
																									   m_collisionDatabase,
																									   m_worldChannel);	//deletes data mem already
		  if(processcount > 30)
             break;//breath-Flooded
		  processcount++;
	}
}
//------------------Spawn Genereator Callback-----------------------------------------//
void CWorldManager::SpawnGeneratorCallback(DWORD currentTime)
{
	
   if(currentTime-m_warmUpStamp < 10000)
      return;
   //int generatorIndexTrace = 0;
   int spawnedThisRound = 0;
   int maxSpawnPerRound = 1;

   if(currentTime-m_spawnGenStamp < m_spawnGenFreq)
      return;
   
   m_spawnGenStamp = currentTime;
 
   
   if(!m_curSpawnGenPos)
   {
       m_curSpawnGenPos = m_spawnGeneratorDB->GetHeadPosition();
       m_spawnTrace = 0;
   }
   
	   
   while(m_curSpawnGenPos)
   {//Spawn Generator loop
      CSpawnObj * spawnGenPtr = (CSpawnObj *)m_spawnGeneratorDB->GetNext(m_curSpawnGenPos);
      if(spawnGenPtr->List_currentOutPut < spawnGenPtr->List_maxOutPutCapacity)
	  {//possibility of spawn
           if(spawnGenPtr->m_aiCfg < 0)
		   {//error
			  CString aiIndexString = m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->m_cabLib.ConvToString(spawnGenPtr->m_aiCfg);
			  CString spawnGenString = m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->m_cabLib.ConvToString(m_spawnTrace);
			  m_aiServerBaseRef->m_eventLog->AddEvent("Invalid AI configuration #",aiIndexString);
			  m_aiServerBaseRef->m_eventLog->AddEvent("Spawn Gen Index #",spawnGenString);
			  m_spawnTrace++;
			  continue;
		   }//end error

           CAIObj * aiPtr = m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->m_aiList->GetByIndex(spawnGenPtr->m_aiCfg);
           if(!aiPtr)
		   {//Invalid ai ptr
	          CString aiIndexString = m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->m_cabLib.ConvToString(spawnGenPtr->m_aiCfg);
			  CString spawnGenString = m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->m_cabLib.ConvToString(m_spawnTrace);
			  m_aiServerBaseRef->m_eventLog->AddEvent("Invalid AI configuration #",aiIndexString);
			  m_aiServerBaseRef->m_eventLog->AddEvent("Spawn Gen Index #",spawnGenString);
			  m_spawnTrace++;
			  continue;
		   }//end invalid ai ptr
		   
		   //add conditions for not spawning
		   D3DMATRIX successMatrice = m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->m_matrixInit2;//spawnGenPtr->List_spawnMatrix;
		   int currentTreePosition = 0;
		   if((rand()%1000) < spawnGenPtr->List_possibilityOfSpawn){//conditions for not spawning
			   if(spawnGenPtr->m_treeUse != -1){//tree override in use
                  POSITION wPos = m_aiWebDatabase->FindIndex(spawnGenPtr->m_treeUse);
				  if(wPos){//
                     CAIWebObj * wObj = (CAIWebObj *)m_aiWebDatabase->GetAt(wPos);
					 POSITION trePos = NULL;
					 if(!wObj->m_web)
					 {//error- not compiled
			            CString spawnGenString = m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->m_cabLib.ConvToString(m_spawnTrace);
						m_aiServerBaseRef->m_eventLog->AddEvent("invalid web->spwn gen #",spawnGenString);
						m_spawnTrace++;
			            continue;
					 }//end error - not compiled
					 
					 
					 
					 if(spawnGenPtr->m_treePosition == -1){//get random tree location
					    int locationAmount = (int)wObj->m_web->GetCount();
                        if(locationAmount > 0){
						   int randomPosition = rand()%locationAmount;
						   trePos = wObj->m_web->FindIndex(randomPosition);
						   currentTreePosition = randomPosition;
						}
					 }//end get random tree location
					 else
					 trePos = wObj->m_web->FindIndex(spawnGenPtr->m_treePosition);
					 if(trePos){//2
                        CAIWebNode * node = (CAIWebNode *)wObj->m_web->GetAt(trePos);
						successMatrice._41 = node->m_position.x;
                        successMatrice._42 = node->m_position.y;
						successMatrice._43 = node->m_position.z;
					 }//end 2
				  }//
			   }//end tree override in use

               CMovementObj * refPtr = NULL;
		       BOOL result = m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->ActivateMovementEntity(NULL,
			                                        spawnGenPtr->m_aiCfg,
			                                        successMatrice,
		                                            0,
											        m_worldChannel,
											        aiPtr->m_entityCfgDBindexRef,//spawnGenPtr->List_spawnIndex,
											        5,//hardcode ai to player-spawnGenPtr->List_spawnType,
											        0,
											        0,
											        FALSE,
											        FALSE,
											        FALSE,
											        m_spawnTrace,
											        -1,
											        &refPtr,
					                                NULL,
													-1,
													-1,
									                m_collisionBase,
									                m_collisionDatabase,
													m_entObjRuntime,
													m_worldName);
		       if(result){//valid spawn
			      refPtr->m_tempChannel = m_worldChannel;
		        if(refPtr->m_aiObj){//if AI
		           refPtr->m_aiObj->m_routeCurTarget = currentTreePosition;
		           refPtr->m_aiObj->m_mapCurLocation = currentTreePosition;
                   refPtr->m_aiObj->m_curMap         = spawnGenPtr->m_treeUse;
           
                   if(refPtr->m_aiObj->m_physicsModuleOverride)
				   {//Movment ovverride valid
		              if(refPtr->List_movementDB)
					  {
                         refPtr->List_movementDB->SafeDelete();
			             delete refPtr->List_movementDB;
			             refPtr->List_movementDB=0;
					  }
                     refPtr->m_aiObj->m_physicsModuleOverride->Clone(&refPtr->List_movementDB);
			         refPtr->List_movementDB->List_lastPosition.x = successMatrice._41;
                     refPtr->List_movementDB->List_lastPosition.y = successMatrice._42;
                     refPtr->List_movementDB->List_lastPosition.z = successMatrice._43;
				   }//end Movment ovverride valid
		           //ArmNextWeapon(refPtr,TRUE);
				}
           
		        spawnGenPtr->List_currentOutPut++;
				spawnedThisRound++;
				if(spawnedThisRound > maxSpawnPerRound)
				{
				   m_spawnTrace++;
				   return;
				}
			   }//valid spawn
		       else
			   {
				 m_aiServerBaseRef->m_eventLog->AddEvent("SpawnGen Failed because it cannot find entity in edb id:",(DWORD)aiPtr->m_entityCfgDBindexRef);
				 m_aiServerBaseRef->m_eventLog->AddEvent(spawnGenPtr->List_spawnName);
			   }
		   }//end conditions for not spawning
	  }//end possibility of spawn
    m_spawnTrace++;
   }//end Spawn Generator loop
}
//-----------removes dead entities----------------------------------------------------//
BOOL CWorldManager::HandleAIDeadEntities(DWORD CurrentTime)
{
POSITION posLast;
int indexTrace = 0;
  for(POSITION posLoc = m_entObjRuntime->GetHeadPosition();(posLast = posLoc) != NULL;){//runtime entity loop
	  CMovementObj * ptrLoc = ( CMovementObj* ) m_entObjRuntime->GetNext(posLoc);
	  if(ptrLoc->List_energy <= 0)
	  if(ptrLoc->List_controlType == 5 || ptrLoc->List_controlType == 1000)
		if((CurrentTime - ptrLoc->List_deathTimeVarial) > 9000){
	        m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->DestroyRuntimeEntity(ptrLoc,m_spawnGeneratorDB,posLast,m_entObjRuntime);
			ptrLoc = 0;
		}
	  indexTrace++;
  }//end runtime entity loop
  return TRUE;
}
//--------------maintains control callback on entities--------------------------------//
void CWorldManager::AIControlUnit() 
{//BEGIN
 //-1=not controlled
//0=control config   keyboard/mouse or joystick
	for(POSITION posLoc = m_entObjRuntime->GetHeadPosition();posLoc != NULL;)
	{//Begin movement entity loop
		CMovementObj * ptrLoc = ( CMovementObj* ) m_entObjRuntime->GetNext(posLoc);		    	
		if(ptrLoc->List_controlType != -1){//if has a controller attached
			if(ptrLoc->List_controlType == 5){//AI
			long stampTest2 = GetTickCount();
			     m_aiServerBaseRef->m_clientModule->m_initObject->m_aiManager->AI_LoopStateX(ptrLoc,m_aiWebDatabase,m_waterZoneDB,m_entObjRuntime,m_collisionBase,m_glAdjustFloat,m_lastGLAdjust);

			long duration = GetTickCount() - stampTest2;
			if(duration > m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->m_performanceTimeStamp)
			   m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->m_performanceTimeStamp = duration;	   
			}//End AI
			if(ptrLoc->List_controlType == 35){//item
			   
			}//End item
			if(ptrLoc->List_controlType == 999 || ptrLoc->List_controlType == 1000){//Dead entity
				if(ptrLoc->List_movementDB)
				{
					ptrLoc->List_movementDB->List_curTransX = 0.0f;
					ptrLoc->List_movementDB->List_curTransY = 0.0f;
					ptrLoc->List_movementDB->List_curTransZ = 0.0f;
				}
				m_aiServerBaseRef->m_clientModule->m_initObject->m_aiManager->AI_Transform(ptrLoc,m_waterZoneDB,m_glAdjustFloat,m_lastGLAdjust);
			}//End Dead Entity
		}//end if has a controller attached
	}//end movement entity loop
}//END
//--------------maintains control callback on entities--------------------------------//
void CWorldManager::SendAIStateToServerCallback(DWORD currentTickCount)
{
    if(currentTickCount-m_warmUpStamp < 11000)
       return;

    currentTickCount = GetTickCount();

	GlobalStatusVariables * gStatus = m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus;
	  if(gStatus->GetElaspeTimeMS_DW(m_updateNetworkStamp) > m_aiCycleDuration)
	  {//timed updates

		 char regenReason = 0;
	     BOOL regenerate = FALSE;
		 BOOL regeneratedOne = FALSE;
         //verify outgoing messages first before continueing
   	     m_updateNetworkStamp = gStatus->GetTimeStamp_DW();
		 POSITION posLast = NULL;
		 int indexTrace   = 0;
		 BOOL aiIsSpawnedRequiresInfo = FALSE; 
         int sendCount = 0;
		 POSITION posLoc;
		 for(posLoc = m_entObjRuntime->GetHeadPosition();(posLast = posLoc) != NULL;)
		 {//size loop
			 CMovementObj * mObj = (CMovementObj *)m_entObjRuntime->GetNext(posLoc);
			 if(mObj->List_networkDefinedID > 0)
			 {//has communication ID
			   if(mObj->List_controlType == 5){//AI cfg
                sendCount++;
			   }//end AI cfg
			 }
			 else if(mObj->List_networkDefinedID == 0)
			 {
				 if(mObj->List_controlType == 5)
			      if(mObj->m_spawnTimeStamp < currentTickCount)
				   if((currentTickCount - mObj->m_spawnTimeStamp) > 15000)
				   {
					 gStatus->m_multiplayerObject->m_eventLog->AddEvent("currentTime:",currentTickCount);
                     gStatus->m_multiplayerObject->m_eventLog->AddEvent("spawnTime:",mObj->m_spawnTimeStamp);
					 gStatus->m_multiplayerObject->m_eventLog->AddEvent("Retrying spawn AI:" + mObj->List_name + " wldID:" +gStatus->m_cabLib.ConvToString(m_worldChannel)+" AICFG-ref:" +gStatus->m_cabLib.ConvToString(mObj->m_aiCfgRef)+ " Loc ID:" + gStatus->m_cabLib.ConvToString(mObj->List_traceNumber)+ " Net ID:",(DWORD)mObj->List_networkDefinedID);
					 m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->DestroyRuntimeEntity(mObj,m_spawnGeneratorDB,posLast,m_entObjRuntime);
					 mObj = 0;
				   }
			 }
		 }//end size loop
		 EnterCriticalSection(&m_statusSec);
		 m_botCount = sendCount;
		 LeaveCriticalSection(&m_statusSec);
         DWORD trace = 0;
		 DWORD maxBytes = (sizeof(SHRUNK_MOVE_STRUCT)*sendCount)+1;

		 BYTE * movBlackData = NULL;
		 if(sendCount > 0)
		 {
		    movBlackData = new BYTE[maxBytes];
		    movBlackData[0] = 17;
			trace = 1;
		 }

		 D3DVECTOR upVect,dirVect,posVect,predictedPosition;
		 SHRUNK_MOVE_STRUCT shMovMsg;
         for(posLoc = m_entObjRuntime->GetHeadPosition();(posLast = posLoc) != NULL;)
		 {//runtime ai loop
			 CMovementObj * mObj = (CMovementObj *)m_entObjRuntime->GetNext(posLoc);

			 if(mObj && mObj->List_networkDefinedID > 0)//has communication ID
			 if(mObj->List_controlType == 5)
			 {//AI cfg
			    if(!mObj->List_baseFrame || !mObj->List_skeletalObject || !mObj->List_movementDB)
			       continue;

		        mObj->List_baseFrame->GetOrientation(&dirVect,&upVect);
		        mObj->List_baseFrame->GetPosition(NULL,&posVect);

                if(mObj->m_networkFirstTimeSendingPos){//init prediction
		           predictedPosition.x = posVect.x;
		           predictedPosition.y = posVect.y;
	               predictedPosition.z = posVect.z;
			       mObj->m_networkFirstTimeSendingPos = FALSE;
				}//end init prediction
		        else{//use prediction
                   if(fabs(mObj->List_movementDB->List_curTransX) > (mObj->List_movementDB->List_maxSpeed*.2f) || 
                      fabs(mObj->List_movementDB->List_curTransZ) > (mObj->List_movementDB->List_maxSpeed*.2f))
				   {
				   
			       predictedPosition.x = posVect.x + ((posVect.x - mObj->m_networkLastPositionSent.x)*.5f);//1.7
		           predictedPosition.y = posVect.y + ((posVect.y - mObj->m_networkLastPositionSent.y)*.5f);
	               predictedPosition.z = posVect.z + ((posVect.z - mObj->m_networkLastPositionSent.z)*.5f);
				   }
				   else{
                   predictedPosition.x = posVect.x;
		           predictedPosition.y = posVect.y;
	               predictedPosition.z = posVect.z;
				   }
                   //use collision on prediction model
				}//end use prediction

		        mObj->m_networkLastPositionSent.x = posVect.x;
		        mObj->m_networkLastPositionSent.y = posVect.y;
		        mObj->m_networkLastPositionSent.z = posVect.z;
				
				int animInUseCnv = mObj->List_skeletalObject->m_animationInUse;
                if(mObj->List_skeletalObject->GetAnimationDirection(animInUseCnv)==TRUE)//playing reverse?
			       animInUseCnv = animInUseCnv * -1;
                   aiIsSpawnedRequiresInfo = TRUE;
                   

				   MOVE_STRUCT mvMsgBld;
                   mvMsgBld.m_catagorie    = 2;
	               mvMsgBld.FreeLookYaw    = mObj->List_skeletalObject->m_freeLookYaw;
                   mvMsgBld.FreeLookPitch  = mObj->List_skeletalObject->m_freeLookPitch;
	               mvMsgBld.Xpos           = predictedPosition.x;
	               mvMsgBld.Ypos           = predictedPosition.y;
	               mvMsgBld.Zpos           = predictedPosition.z;
                   mvMsgBld.idRefrence     = (short)mObj->List_networkDefinedID;
                   mvMsgBld.OrientateDirX  = dirVect.x;
         	       mvMsgBld.OrientateDirY  = dirVect.y;
	               mvMsgBld.OrientateDirZ  = dirVect.z;
	               mvMsgBld.OrientateUpX   = upVect.x;
	               mvMsgBld.OrientateUpY   = upVect.y;
	               mvMsgBld.OrientateUpZ   = upVect.z;
	               //mvMsgBld.dbIndex        = mObj->List_refModel;
	               mvMsgBld.curAnim        = animInUseCnv;
	               mvMsgBld.secondaryAnim  = mObj->List_skeletalObject->m_secondaryAnimationInUse;

                   
                   gStatus->m_multiplayerObject->ShrinkData(&mvMsgBld,&shMovMsg);

				   if(movBlackData && (trace + sizeof(SHRUNK_MOVE_STRUCT)) <= maxBytes)
				   {
					   CopyMemory(&movBlackData[trace],&shMovMsg,sizeof(SHRUNK_MOVE_STRUCT));
					   trace = trace + sizeof(SHRUNK_MOVE_STRUCT);
				   }
                   regenerate = FALSE;
				   regenReason = 0;

				   if(!regeneratedOne)
				   if(mObj->List_AI_target == -1)//not in battle
					   if((currentTickCount - mObj->m_spawnTimeStamp) > m_aiServerBaseRef->m_aiResetTime)
					   {//3 hours no regen since then
					      regenerate = TRUE;
						  regenReason = 1;//
					   }

				   if(!regeneratedOne)
				   if(mObj->m_lifeTimeMax != 0)//ai has a limit on life
					   if((currentTickCount - mObj->m_spawnTimeStamp) > mObj->m_lifeTimeMax)
					   {//life line ended
					      regenerate = TRUE;
						  regenReason = 2;
					   }

                   if(mObj->List_energy < 1 || posVect.y < -30000 || regenerate == TRUE)
				   {//dead entity
                      if(posVect.y < -30000)
					  {//out of bounds
						 
						 
                         CString build = "Out of bounds:"+mObj->List_name;
						 build = build+" index:";
						 build = build+gStatus->m_cabLib.ConvToString(mObj->List_SpawnGeneratorLink);
						 build = build+" channel:";
						 build = build+gStatus->m_cabLib.ConvToString(m_worldChannel);
						 
					     gStatus->m_multiplayerObject->m_eventLog->AddEvent(build);
					     mObj->List_movementDB->List_doGravity = FALSE;//make it dissapear
						 mObj->List_energy = 0;
					  }//end out of bounds

					  if(regenerate)
					  {//been in game too long for ai
						 
						 regeneratedOne = TRUE;
                         CString build = "Regenerating...:"+mObj->List_name;
						 build = build+" index:";
						 build = build+gStatus->m_cabLib.ConvToString(mObj->List_SpawnGeneratorLink);
						 build = build+" NetID:";
						 build = build+gStatus->m_cabLib.ConvToString(mObj->List_networkDefinedID);
						 build += " time-in-game:" + gStatus->m_cabLib.ConvToString((int)(currentTickCount - mObj->m_spawnTimeStamp));
						 switch(regenReason)
						 {
						     case 0:build += " (No Reason)";break;
                             case 1:build += " (3 hour reset timout)";break;
							 case 2:build += " (Life limit defined)";break;
						 }
					     //gStatus->m_multiplayerObject->m_eventLog->AddEvent(build);
					     mObj->List_movementDB->List_doGravity = FALSE;//make it dissapear
						 mObj->List_energy = 0;
						 gStatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH,
																		DPID_SERVERPLAYER,
																		2,
																		mObj->List_networkDefinedID,
																		0,
																		0,
																		0,
																		NULL);
					  }//end been in game too long for ai

					  if(mObj->List_movementDB->List_doGravity == FALSE || mObj->List_movementDB->m_inLiquid == TRUE || mObj->List_movementDB->m_floatWhenDead == TRUE)
					  {//not in air
						    m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->DestroyRuntimeEntity(mObj,m_spawnGeneratorDB,posLast,m_entObjRuntime);
							mObj = 0;
							continue;
					  }//end not in air
				   }//end dead entity
			 }//end AI cfg
		 }//end runtime ai loop
         //send block data
         if(trace > 1)
		 {//send block update
			if(movBlackData)
			{
			   gStatus->m_multiplayerObject->SendData((BYTE *)movBlackData,
				                                              trace,
				                                              DPNSEND_GUARANTEED,
				                                              DPID_SERVERPLAYER,
					                                          gStatus->m_multiplayerObject->m_packetTimeout);


               delete [] movBlackData;
			   movBlackData = 0;
			}
			
		 }//end send block update
		 else
		 {//required pending msgs asap-this happens when no bots are spawns to trigger exchange of guarenteed packets
            if(movBlackData)
            {
               delete [] movBlackData;
               movBlackData = 0;
            }
            gStatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED,
													     DPID_SERVERPLAYER,
														 3,
														 m_worldChannel,//world channel based on server ID channel world list channel ID
														 0,
														 0,
														 0,
														 NULL);
			//gStatus->m_multiplayerObject->m_eventLog->AddEvent("request world by ID sent channel:",(DWORD)m_worldChannel);//gunna happen when no bots are in world
		 }//end required pending msgs
	  }//end timed updates
	  else{//just keep animating for upkeep of fire ques
	     SpawnGeneratorCallback(currentTickCount);
	  }//end just keep animating for upkeep of fire quest
}
//--------------------------------------------------------------------------
void CWorldManager::CallBackTimeOpen()
{
	StartTime.QuadPart = m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->GetTimeStamp();
}
//--------------------------------------------------------------------------
void CWorldManager::CallBackTimeCap()
{
	

	   Sleep(0);
	   m_lastGLAdjust = m_glAdjust;
	   double shouldWaitTime = 1000.0/m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->fps_cap; 
	   double seconds = (double)m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->GetElaspeTimeMS(StartTime.QuadPart);
	  
       if(seconds < shouldWaitTime)
	   {
	    DWORD sleepTime = (DWORD)(shouldWaitTime - seconds);
		Sleep(sleepTime);

		while(seconds < shouldWaitTime)
		{//hard delay spin
			seconds = (double)m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->GetElaspeTimeMS(StartTime.QuadPart);
		}//end hard delay spin
		m_glAdjust = 1.0;
	   }
	   else
	   { 
        m_glAdjust = seconds/shouldWaitTime;
	    if(m_glAdjust > 8.0)
		   m_glAdjust = 8.0f;

	   }
	   m_glAdjustFloat = (float)m_glAdjust;
}














