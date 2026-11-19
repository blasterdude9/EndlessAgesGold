#include "StdAfx.h"
#include "worldmanager.h"

CWorldManager::CWorldManager(CAIServerInitialization * aiserverInit,int aiSvrID)
{
	InitializeCriticalSection(  &m_criticalSec );
	InitializeCriticalSection(  &m_statusSec );
	
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
	m_missileRuntime                    = new CMissileObjList();
	m_explosionRuntimeList              = new CExplosionObjList();
	m_inBox                             = new CPacketObjList();
	
	m_aiCycleDuration                   = 250;
	m_glAdjustFloat                     = 1.0f;
	m_glAdjust                          = 1.0;
	QuadPartRef							= m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->GL_frequency.QuadPart;
	m_botCount                          = 0;
	CallBackTimeOpen();
	m_updateNetworkStamp                = m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->GetTimeStamp();
}

CWorldManager::~CWorldManager(void)
{
	if(m_inBox)
	{
       m_inBox->SafeDelete();
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

CWorldManagerDatabase::~CWorldManagerDatabase(void)
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
         CWorldManager * wManager = (CWorldManager *) GetNext(posLoc);
         delete wManager;
         wManager = 0;
	 }
	 RemoveAll();
}

CWorldManager * CWorldManagerDatabase::GetByIndex(int index)
{
   if(index < 0)
	  return NULL;

   POSITION posLoc = FindIndex(index);
   if(!posLoc)
       return NULL;
   CWorldManager * localPtr = (CWorldManager *)GetAt(posLoc);
   return localPtr;
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

	 m_collisionBase->BuildObjectReferenceArray(m_wldObjList);
     m_spawnGeneratorDB->GetTotalAIOutput();

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
	    m_aiServerBaseRef->m_eventLog->AddEvent("AI web count:",(DWORD)m_aiWebDatabase->GetCount());


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
	SpawnGeneratorCallback(currentTime);
	HandleAIDeadEntities(currentTime);
	AIControlUnit();
	m_aiServerBaseRef->m_clientModule->m_initObject->m_clientNet->ManageSmoothMovementOnNetworkEntities(currentTime,m_entObjRuntime);
	m_aiServerBaseRef->m_clientModule->m_initObject->m_misExpManager->MissileCallback(  m_missileRuntime,
																						m_explosionRuntimeList,
																						m_glAdjustFloat,
												                                        m_lastGLAdjust);
	m_aiServerBaseRef->m_clientModule->m_initObject->m_misExpManager->ExplosionCallback(m_entObjRuntime,m_explosionRuntimeList);
	//m_aiServerBaseRef->m_clientModule->m_initObject->m_entityManager->CollisionLoopEntityToEntity(m_entObjRuntime);
	//m_aiServerBaseRef->m_clientModule->m_initObject->m_entityManager->CollisionLoopEntityWorld(m_entObjRuntime);
	//m_aiServerBaseRef->m_clientModule->m_initObject->m_entityManager->RealtimeEntityCollisionCallback(m_entObjRuntime);
	m_aiServerBaseRef->m_clientModule->m_initObject->m_entityManager->CollisionPipeline(&m_mathLib,m_entObjRuntime,m_missileRuntime);
	m_aiServerBaseRef->m_clientModule->m_initObject->m_entityManager->PossesCallback(m_entObjRuntime);
	//process in box
	ProcessPackets();
	SendAIStateToServerCallback(currentTime);
	m_aiServerBaseRef->m_clientModule->m_initObject->m_entityManager->GenerateEntityLastPosition(m_entObjRuntime);
	m_aiServerBaseRef->m_clientModule->m_initObject->m_entityManager->EntityDamagePathCheck(m_entObjRuntime);
	m_aiServerBaseRef->m_clientModule->m_initObject->m_misExpManager->MissileCollisionPipeline(m_entObjRuntime,m_missileRuntime,m_explosionRuntimeList);
	//m_aiServerBaseRef->m_clientModule->m_initObject->m_misExpManager->MissileCollisionCheck(m_entObjRuntime,m_missileRuntime,m_explosionRuntimeList);
	//m_aiServerBaseRef->m_clientModule->m_initObject->m_misExpManager->MissileCollisionCheckWorld(m_collisionBase,m_missileRuntime,m_explosionRuntimeList);
	m_aiServerBaseRef->m_clientModule->m_initObject->m_misExpManager->ExplosionCollisionCallback(m_entObjRuntime,m_explosionRuntimeList);
	m_aiServerBaseRef->m_clientModule->m_initObject->m_misExpManager->UpdateRuntimeMissileLastPosition(m_missileRuntime);
	

   return 0;
}
//------------------Process packets Callback-----------------------------------------//
void CWorldManager::ProcessPackets()
{
    EnterCriticalSection( &m_criticalSec);
	POSITION posLast = NULL;
	for(POSITION posLoc = m_inBox->GetHeadPosition();(posLast = posLoc)!= NULL;)
	{
		CPacketObj * packetPtr = (CPacketObj *)m_inBox->GetNext(posLoc);
		LeaveCriticalSection( &m_criticalSec);
	    m_aiServerBaseRef->m_clientModule->m_initObject->m_clientNet->HandleEngineMessagesClientSide(packetPtr->m_data,
			                                                                                         packetPtr->m_communicationID,
																									 packetPtr->m_size,
																									 m_spawnGeneratorDB,
																									 m_entObjRuntime,
																									 m_missileRuntime,
																									 m_explosionRuntimeList,
																									 m_collisionBase,
																									 m_collisionDatabase,
																									 m_worldChannel);				
		EnterCriticalSection( &m_criticalSec);
		delete packetPtr;
		packetPtr = 0;
		m_inBox->RemoveAt(posLast);
	}
	LeaveCriticalSection( &m_criticalSec);
}
//------------------Spawn Genereator Callback-----------------------------------------//
void CWorldManager::SpawnGeneratorCallback(DWORD currentTime)
{
	
   if(currentTime-m_warmUpStamp < 10000)
      return;
   int generatorIndexTrace = 0;
   int spawnedThisRound = 0;
   int maxSpawnPerRound = 5;
   //if(!m_spawnGeneratorDB->m_enable)
	   //return;

   for(POSITION posLocal = m_spawnGeneratorDB->GetHeadPosition();posLocal != NULL;)
   {//Spawn Generator loop
      CSpawnObj * spawnGenPtr = (CSpawnObj *)m_spawnGeneratorDB->GetNext(posLocal);
      if(spawnGenPtr->List_currentOutPut < spawnGenPtr->List_maxOutPutCapacity)
	  {//possibility of spawn
           if(spawnGenPtr->m_aiCfg < 0)
		   {//error
			  CString aiIndexString = m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->m_cabLib.ConvToString(spawnGenPtr->m_aiCfg);
			  CString spawnGenString = m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->m_cabLib.ConvToString(generatorIndexTrace);
			  m_aiServerBaseRef->m_eventLog->AddEvent("Invalid AI configuration #",aiIndexString);
			  m_aiServerBaseRef->m_eventLog->AddEvent("Spawn Gen Index #",spawnGenString);
			  continue;
		   }//end error

           CAIObj * aiPtr = m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->m_aiList->GetByIndex(spawnGenPtr->m_aiCfg);
           if(!aiPtr)
		   {//Invalid ai ptr
	          CString aiIndexString = m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->m_cabLib.ConvToString(spawnGenPtr->m_aiCfg);
			  CString spawnGenString = m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->m_cabLib.ConvToString(generatorIndexTrace);
			  m_aiServerBaseRef->m_eventLog->AddEvent("Invalid AI configuration #",aiIndexString);
			  m_aiServerBaseRef->m_eventLog->AddEvent("Spawn Gen Index #",spawnGenString);
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
			            CString spawnGenString = m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->m_cabLib.ConvToString(generatorIndexTrace);
						m_aiServerBaseRef->m_eventLog->AddEvent("invalid web->spwn gen #",spawnGenString);
			            continue;
					 }//end error - not compiled
					 
					 
					 
					 if(spawnGenPtr->m_treePosition == -1){//get random tree location
					    int locationAmount = wObj->m_web->GetCount();
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
											        0,
											        aiPtr->m_entityCfgDBindexRef,//spawnGenPtr->List_spawnIndex,
											        5,//hardcode ai to player-spawnGenPtr->List_spawnType,
											        0,
											        0,
											        FALSE,
											        FALSE,
											        FALSE,
											        generatorIndexTrace,
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
				   return;
			   }//valid spawn
		       else
			   {
				 m_aiServerBaseRef->m_eventLog->AddEvent("SpawnGen Failed");
				 m_aiServerBaseRef->m_eventLog->AddEvent(spawnGenPtr->List_spawnName);
			   }
		   }//end conditions for not spawning
	  }//end possibility of spawn
   generatorIndexTrace++;
   }//end Spawn Generator loop
}
//-----------removes dead entities----------------------------------------------------//
BOOL CWorldManager::HandleAIDeadEntities(DWORD CurrentTime)
{
POSITION posLast;
int indexTrace = 0;
  for(POSITION posLoc = m_entObjRuntime->GetHeadPosition();(posLast = posLoc) != NULL;){//runtime entity loop
	  CMovementObj * ptrLoc = ( CMovementObj* ) m_entObjRuntime->GetNext(posLoc);
      if(ptrLoc->List_controlType == 999 || ptrLoc->List_controlType == 997){
		  ptrLoc->FadeOut(3000);
	      ptrLoc->List_controlType = 1000;	 
	  }
	  else if(ptrLoc->List_controlType == 1000){
		  if((CurrentTime - ptrLoc->List_deathTimeVarial) > 3000){
				ptrLoc->SafeDelete(NULL,m_spawnGeneratorDB);
                delete ptrLoc;
	            ptrLoc = 0;
	            m_entObjRuntime->RemoveAt(posLast);
				//InListRuntimeEntities(indexTrace);
		  }
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
			     m_aiServerBaseRef->m_clientModule->m_initObject->m_aiManager->AI_Loop(ptrLoc,m_aiWebDatabase,m_waterZoneDB,m_entObjRuntime,m_collisionBase,m_glAdjustFloat,m_lastGLAdjust);

			long duration = GetTickCount() - stampTest2;
			if(duration > m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->m_performanceTimeStamp)
			   m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->m_performanceTimeStamp = duration;	   
			}//End AI
			if(ptrLoc->List_controlType == 35){//item
			   
			}//End item
			if(ptrLoc->List_controlType == 999 || ptrLoc->List_controlType == 1000){//Dead entity
				ptrLoc->List_movementDB->List_curTransX = 0.0f;
				ptrLoc->List_movementDB->List_curTransY = 0.0f;
				ptrLoc->List_movementDB->List_curTransZ = 0.0f;
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


	GlobalStatusVariables * gStatus = m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus;
	  if(gStatus->GetElaspeTimeMS(m_updateNetworkStamp) > m_aiCycleDuration)
	  {//timed updates
		 BOOL regeneratedOne = FALSE;
         //verify outgoing messages first before continueing
   	     m_updateNetworkStamp = gStatus->GetTimeStamp();
		 POSITION posLast = NULL;
		 int indexTrace   = 0;
		 BOOL aiIsSpawnedRequiresInfo = FALSE; 
         int sendCount = 0;
		 for(POSITION posLoc = m_entObjRuntime->GetHeadPosition();(posLast = posLoc) != NULL;)
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
				 if((currentTickCount - mObj->m_spawnTimeStamp) > 5000)
				 {
					 gStatus->m_multiplayerObject->m_eventLog->AddEvent("Retrying spawn AI:" + mObj->List_name + " wldID:" +gStatus->m_cabLib.ConvToString(m_worldChannel)+" AICFG-ref:" +gStatus->m_cabLib.ConvToString(mObj->m_aiCfgRef)+ " Loc ID:" + gStatus->m_cabLib.ConvToString(mObj->List_traceNumber)+ " Net ID:",(DWORD)mObj->List_networkDefinedID);
                     mObj->SafeDelete(NULL,m_spawnGeneratorDB);
					 delete mObj;
					 mObj = 0;
	                 m_entObjRuntime->RemoveAt(posLast);
				 }
			 }
		 }//end size loop
		 EnterCriticalSection(&m_statusSec);
		 m_botCount = sendCount;
		 LeaveCriticalSection(&m_statusSec);

		 SHRUNK_MOVE_STRUCT * movBlackData = NULL;
		 if(sendCount > 0)
		    movBlackData = new SHRUNK_MOVE_STRUCT[sendCount]+1;

		 int trace = 0;
		 /*BYTE * movBlackData = NULL;
		 if(sendCount > 0)
		 {
		    movBlackData = new BYTE[(sizeof(SHRUNK_MOVE_STRUCT)*sendCount)+1];
		    movBlackData[0] = 17;
			trace = 1;
		 }*/
		 
		 D3DVECTOR upVect,dirVect,posVect,predictedPosition;
         for(posLoc = m_entObjRuntime->GetHeadPosition();(posLast = posLoc) != NULL;)
		 {//runtime ai loop
			 CMovementObj * mObj = (CMovementObj *)m_entObjRuntime->GetNext(posLoc);

			 if(mObj->List_networkDefinedID > 0)//has communication ID
			 if(mObj->List_controlType == 5){//AI cfg
				//gStatus->IndexEntityAnimations(mObj,GetTickCount(),TRUE,m_missileRuntime);
                
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
                   //build variables  
			       /*D3DMATRIX entMatrix;
			       mObj->List_baseFrame->GetTransform(NULL,&entMatrix);
			       float characterHieght = mObj->List_skeletalObject->m_box.maxY;
	               float characterWidth = mObj->List_skeletalObject->m_box.maxX;
	               if(mObj->List_skeletalObject->m_box.maxY > mObj->List_skeletalObject->m_box.maxX)
	                  characterWidth = mObj->List_skeletalObject->m_box.maxY;*/
			          /*gStatus->VirtualCollisionAdjust(      &predictedPosition,
															predictedPosition,
															posVect,
															(int)characterWidth,
															(int)characterHieght,
															entMatrix,
															(int)mObj->List_movementDB->m_antiGravity);*/
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

                   SHRUNK_MOVE_STRUCT shMovMsg;
                   gStatus->m_multiplayerObject->ShrinkData(&mvMsgBld,&shMovMsg);

				   CopyMemory(&movBlackData[trace],&shMovMsg,sizeof(SHRUNK_MOVE_STRUCT));
                   BOOL regenerate = FALSE;

				   if(!regeneratedOne)
				   if(mObj->List_AI_target == -1)//not in battle
				   if((currentTickCount - mObj->m_spawnTimeStamp) > 10800000)//3 hours no regen since then
					   regenerate = TRUE;

				   if(!regeneratedOne)
				   if(mObj->m_lifeTimeMax != 0)//ai has a limit on life
				   if((currentTickCount - mObj->m_spawnTimeStamp) > mObj->m_lifeTimeMax)//life line ended
					   regenerate = TRUE;

				   //trace += sizeof(SHRUNK_MOVE_STRUCT);
				   trace++;
                   if(mObj->List_energy < 1 || posVect.y < -30000 || regenerate == TRUE)
				   {//dead entity
                      if(posVect.y < -30000)
					  {//out of bounds
						 
						 
                         CString build = "Out of bounds:"+mObj->List_name;
						 build = build+" index:";
						 build = build+gStatus->m_cabLib.ConvToString(mObj->List_SpawnGeneratorLink);
						 
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
						 
					     gStatus->m_multiplayerObject->m_eventLog->AddEvent(build);
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
						 mObj->SafeDelete(NULL,m_spawnGeneratorDB);
						 delete mObj;
						 mObj = 0;
	                     m_entObjRuntime->RemoveAt(posLast);
						 continue;
					  }//end not in air
				   }//end dead entity
			 }//end AI cfg
		 }//end runtime ai loop
         //send block data
         if(trace > 0)
		 {//send block update
			if(movBlackData)
			{
               m_aiServerBaseRef->m_clientModule->m_initObject->m_clientNet->SendAIBlockMsg(DPNSEND_GUARANTEED,
																							DPID_SERVERPLAYER,
																							movBlackData,
																							trace
																							);
			   /*gStatus->m_multiplayerObject->SendData((BYTE *)movBlackData,
				                                              trace,
				                                              DPNSEND_GUARANTEED,
				                                              DPID_SERVERPLAYER,
					                                          gStatus->m_multiplayerObject->m_packetTimeout);*/
               delete [] movBlackData;
			   movBlackData = 0;
			}
			
		 }//end send block update
		 else
		 {//required pending msgs asap
            gStatus->m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED,
													     DPID_SERVERPLAYER,
														 3,
														 m_worldChannel,//world channel based on server ID channel world list channel ID
														 0,
														 0,
														 0,
														 NULL);
			gStatus->m_multiplayerObject->m_eventLog->AddEvent("request world by ID sent channel:",(DWORD)m_worldChannel);
		 }//end required pending msgs
		 //ask server for update
		 //if(aiIsSpawnedRequiresInfo){
           /*gStatus->m_netGameLogicObject->SendAttribMsg(  DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH,
															DPID_SERVERPLAYER,
															16,
															0,
															0,
															0,
															0,
															NULL);*/
		   //m_pingPersistStamp = currentTickCount;
		   //m_pingInQuerry = TRUE;
		 //}
		 
		 //m_aiServerBaseRef->m_clientModule->m_initObject->m_clientNet->SendGuerenteedCycleMsgToServer();
         //end ask server for update
	  }//end timed updates
	  else{//just keep animating for upkeep of fire ques
         /*for(POSITION posLoc = m_entObjRuntime->GetHeadPosition();posLoc != NULL;)
		 {//runtime ai loop
			 CMovementObj * mObj = (CMovementObj *)m_entObjRuntime->GetNext(posLoc);
			 if(mObj->List_networkDefinedID > 0)//has communication ID
			 if(mObj->List_controlType == 5){//AI cfg
				gStatus->IndexEntityAnimations(mObj,GetTickCount(),TRUE,m_missileRuntime);
			 }//end AI cfg
		 }//end runtime ai loop*/
	  }//end just keep animating for upkeep of fire quest
}
//--------------------------------------------------------------------------
CWorldManager * CWorldManagerDatabase::GetByID(int index)
{
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {
       CWorldManager * ptrLoc = (CWorldManager*)GetNext(posLoc);
       if(ptrLoc->m_worldChannel == index)
	      return ptrLoc;
   }
   return NULL;
}
//--------------------------------------------------------------------------
void CWorldManager::CallBackTimeOpen()
{
    //m_startTimeEmulate = GetTickCount();
    //if(QuadPartRef != 0)QueryPerformanceCounter(&StartTime);

	StartTime.QuadPart = m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->GetTimeStamp();
}
//--------------------------------------------------------------------------
void CWorldManager::CallBackTimeCap()
{
	/*DWORD exactFPSHighRez = 1;
    //Calculate timing
	double seconds = 16;
	if(QuadPartRef != 0)
	{//exact FPS display
       QueryPerformanceCounter(&EndTime);
       seconds = (((double(EndTime.QuadPart - StartTime.QuadPart)) * 1000.0) / ((double)QuadPartRef));
	   if(seconds == 0.0)
	      exactFPSHighRez = 1;
	   else
		  exactFPSHighRez = (DWORD)(1000.0/seconds);   
	}//end exact FPS display
	else
	    m_aiServerBaseRef->m_eventLog->AddEvent("No high resolution counter found, update machine specs");

	double fps_capD = (double)m_aiServerBaseRef->m_clientModule->m_initObject->m_gStatus->fps_cap;
	m_lastGLAdjust = m_glAdjust;
	DWORD l_exactFPSHiRes = exactFPSHighRez;
	
    if(fps_capD < l_exactFPSHiRes)
	{//plenty of speed
	   double shouldWaitTime = 1000.0/fps_capD;
       DWORD sleepTime = (DWORD)(shouldWaitTime - seconds);
	   if(sleepTime > 0)
	      Sleep(sleepTime);
	}//end plenty of speed
	else
	{//slower than cap   Calculate
	   m_glAdjust = fps_capD/((double)l_exactFPSHiRes);
	   if(m_glAdjust > 20)
		  m_glAdjust = 1.0f;
	}//end slower than cap   Calculate
	
	if(m_glAdjust < 1.0f)
	   m_glAdjust = 1.0f;

	 m_glAdjustFloat = (float)m_glAdjust;*/

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














