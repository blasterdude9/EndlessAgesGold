#include "StdAfx.h"
#include "clientmodule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CClientModule::CClientModule(HWND hwnd,
							 int startWidth,
							 int startHeight,
							 int widthLoc,
							 int hieghtLoc,
							 int depth,
							 BOOL fullScreen,
							 BOOL editOn,
							 BOOL ovrdRDsrfc,
							 BOOL renderingEnabled,
							 BOOL processScripts,
							 BOOL initDirectInput,
							 int maxTextureSize,
							 int shadowCount,
							 BOOL sim)
{
    m_initObject = new CClientInitialization(hwnd,
		                                     startWidth,
											 startHeight,
											 widthLoc,
											 hieghtLoc,
											 depth,
											 fullScreen,
											 editOn,
											 ovrdRDsrfc,
											 renderingEnabled,
											 processScripts,
											 initDirectInput,
											 maxTextureSize,
											 shadowCount,
											 sim);
	long currentTime = GetTickCount();
	m_lastMsgProcessed = currentTime;
}

CClientModule::~CClientModule(void)
{
	if(m_initObject)
	{
        delete m_initObject;
	   m_initObject = 0;
	}
}

void CClientModule::ClientCallback()
{
	DWORD currentTime = GetTickCount();

	if(m_initObject->m_gStatus->m_dxRenderer->m_restoreStatesPls)
	{
       m_initObject->m_gStatus->SetAllRenderStatesIM();
	   m_initObject->m_gStatus->m_textRenderList->Flush();
	   m_initObject->m_gStatus->m_dxRenderer->m_restoreStatesPls = FALSE;
	   m_initObject->m_gStatus->InitializeEnvironment(FALSE);
	   //m_initObject->m_gStatus->RefreshMenus();
	   //m_initObject->m_gStatus->m_systemReadout->Flush();
	   //m_initObject->m_gStatus->m_systemReadout->ReinitVisualSection(m_initObject->m_gStatus->m_3dDevice,m_initObject->m_gStatus->m_symbolMapDB,m_initObject->m_gStatus->m_speechMode);
	   
	   //m_initObject->m_gStatus->m_systemReadout->AddLine("...",m_initObject->m_gStatus->m_3dDevice,m_initObject->m_gStatus->m_symbolMapDB,m_initObject->m_gStatus->m_speechMode);
	}
	
	m_initObject->m_gStatus->m_musicPlayer->Callback(currentTime);
    m_initObject->m_renderManager->VPORTRENDERCOUNT = 0;
	//m_initObject->m_userControls->MouseManagerCallback();
    m_initObject->m_userControls->MouseKeyboardUpdate();
	//m_initObject->m_userControls->ProcessKeyboardBufferedData();
    m_initObject->m_renderManager->RenderLoopBeginningChecks();
	m_initObject->m_gStatus->m_soundManager->CallBack();
	m_initObject->m_entityManager->HandleDeadEntities(currentTime);
	m_initObject->m_sceneAssembler->TriggerCheckCallback();
	m_initObject->m_gStatus->m_soundTrackList->Update();
	m_initObject->m_entityManager->UniversalControlUnit(m_initObject->m_gStatus->m_waterZoneDB);
	m_initObject->m_clientNet->ClientCallback(m_initObject->m_gStatus->m_spawnGeneratorDB,m_initObject->m_gStatus->m_entObjRuntime,m_initObject->m_gStatus->m_missileRuntime,m_initObject->m_gStatus->m_explosionRuntimeList);
	m_initObject->m_clientNet->ManageSmoothMovementOnNetworkEntities(currentTime,m_initObject->m_gStatus->m_entObjRuntime);
	//lockpoint callback
	m_initObject->m_misExpManager->MissileCallback(m_initObject->m_gStatus->m_missileRuntime,m_initObject->m_gStatus->m_explosionRuntimeList,m_initObject->m_gStatus->m_timeAdjustFloat,m_initObject->m_gStatus->m_lastTimeAdjust);
    m_initObject->m_gStatus->Sound3DCallback();	
	m_initObject->m_misExpManager->ExplosionCallback(m_initObject->m_gStatus->m_entObjRuntime,m_initObject->m_gStatus->m_explosionRuntimeList);
	m_initObject->m_entityManager->CollisionPipeline(&m_initObject->m_gStatus->m_matLib,m_initObject->m_gStatus->m_entObjRuntime,m_initObject->m_gStatus->m_missileRuntime);
	m_initObject->m_clientNet->UpdateServerCallback(m_initObject->m_gStatus->m_spawnGeneratorDB,m_initObject->m_gStatus->m_entObjRuntime,m_initObject->m_gStatus->m_missileRuntime);//update server based on frequency
	m_initObject->m_entityManager->GenerateEntityLastPosition(m_initObject->m_gStatus->m_entObjRuntime);
	m_initObject->m_entityManager->PossesCallback(m_initObject->m_gStatus->m_entObjRuntime);//EXPERIMENT
	m_initObject->m_entityManager->EntityDamagePathCheck(m_initObject->m_gStatus->m_entObjRuntime);
	m_initObject->m_misExpManager->MissileCollisionPipeline(m_initObject->m_gStatus->m_entObjRuntime,m_initObject->m_gStatus->m_missileRuntime,m_initObject->m_gStatus->m_explosionRuntimeList,m_initObject->m_gStatus->m_collisionBase);
	m_initObject->m_misExpManager->ExplosionCollisionCallback(m_initObject->m_gStatus->m_entObjRuntime,m_initObject->m_gStatus->m_explosionRuntimeList);
	m_initObject->m_misExpManager->UpdateRuntimeMissileLastPosition(m_initObject->m_gStatus->m_missileRuntime);
	m_initObject->m_clientNet->PendingCommandsCallback();
	m_initObject->m_gStatus->TempMenuCallBack();
    m_initObject->m_renderManager->RenderVisualsImediateMode();//entiy animations are done here
	
	m_initObject->m_gStatus->m_textRenderList->Flush();
	m_initObject->m_userControls->GlobalControlsInLoop();
}

void CClientModule::CallBackTimeOpen()
{
	if(!m_initObject->m_gStatus)
		return;

		StartTime.QuadPart = m_initObject->m_gStatus->GetTimeStamp();
}

void CClientModule::CallBackTimeCap()
{
	if(!m_initObject->m_gStatus)
		return;

	   Sleep(0);//increse to simulate noramlly zero
	   m_initObject->m_gStatus->m_lastTimeAdjust = m_initObject->m_gStatus->m_timeAdjust;
	   double shouldWaitTime = 1000.0/m_initObject->m_gStatus->fps_cap; 
	   double seconds = (double)m_initObject->m_gStatus->GetElaspeTimeMS(StartTime.QuadPart);

	   if(shouldWaitTime > 2000.0)
	      shouldWaitTime = 2000.0;
	  
       if(seconds < shouldWaitTime)
	   {
	    DWORD sleepTime = (DWORD)(shouldWaitTime - seconds);
		if(sleepTime > 2000)
		   sleepTime = 2000;
		
		Sleep(sleepTime);

		while(seconds < shouldWaitTime)
		{//hard delay spin
			seconds = (DWORD)m_initObject->m_gStatus->GetElaspeTimeMS(StartTime.QuadPart);
		}//end hard delay spin
		m_initObject->m_gStatus->m_timeAdjust = 1.0;
	   }
	   else
	   { 
        m_initObject->m_gStatus->m_timeAdjust = seconds/shouldWaitTime;
	    if(m_initObject->m_gStatus->m_timeAdjust > 8.0)
		   m_initObject->m_gStatus->m_timeAdjust = 8.0;

	   }

	   static int start_time = clock();
	   int cur_time = clock();
	   int delta_time = cur_time - start_time;
	   start_time = cur_time;
	   static int fps_result = 0;
	   static int fps_counter = 0;
	   static int total_time = 0;

	   total_time += delta_time;
	   if(total_time >= 1000)
	   {
		   fps_result = fps_counter + 1;
		   fps_counter = total_time = 0;
	   }
	   fps_counter++;

	   m_initObject->m_gStatus->m_exactFPSHiRes = fps_result;

 	   //m_initObject->m_gStatus->m_exactFPSHiRes = (int)(1000.0/seconds);
	   m_initObject->m_gStatus->m_renderStateObject->m_glAdjust = (float)m_initObject->m_gStatus->m_timeAdjust;
	   m_initObject->m_gStatus->m_timeAdjustFloat = (float)m_initObject->m_gStatus->m_timeAdjust;
}


GlobalStatusVariables * CClientModule::GetGlobalVarInterface()
{
    if(m_initObject)
	   return m_initObject->m_gStatus;
	return NULL;
}

SceneAssembly * CClientModule::GetSceneAssemblerInterface()
{
    if(m_initObject)
	   return m_initObject->m_sceneAssembler;
	return NULL;
}

CClientNetworkingLogic * CClientModule::GetNetClientInterface()
{
    if(m_initObject)
	   return m_initObject->m_clientNet;
	return NULL;
}

UserController * CClientModule::GetUserControllerInterface()
{
    if(m_initObject)
	   return m_initObject->m_userControls;
	return NULL;
}

CMenuManagement * CClientModule::GetMenuManagerInterface()
{
    if(m_initObject)
	   return m_initObject->m_menuManagement;
	return NULL;
}

CRenderingManagement * CClientModule::GetRenderManagerInterface()
{
    if(m_initObject)
	   return m_initObject->m_renderManager;
	return NULL;
}

void CClientModule::ProcessAnalogKeyboardMsgs(UINT & chrPass)
{
return;
/*	    CString addition = m_initObject->m_gStatus->ParseData(chrPass);
	    if(m_initObject->m_gStatus->m_acceptConsoleTxt)
		{
			m_initObject->m_gStatus->m_consolTXT = m_initObject->m_gStatus->m_consolTXT + addition;//operator+(m_gStatus->m_chatBuild,TranslateCharValue(Msg.wParam));
			if( chrPass == 8 )
			{
				m_initObject->m_gStatus->m_backspaceQued = TRUE;
			}
		}
	    

		if(GetTickCount()-m_initObject->m_gStatus->m_lastChatCommandStamp > 50)
        if(m_initObject->m_gStatus->m_multiplayerObject->m_chatEnabled == TRUE ||
		   m_initObject->m_gStatus->m_multiplayerObject->m_groupChatEnabled == TRUE ||
		   m_initObject->m_gStatus->m_multiplayerObject->m_clanChatEnabled == TRUE)
		{//retrieve text dont miss
				if( chrPass != 8 )
				{
			      m_initObject->m_gStatus->m_chatBuild = m_initObject->m_gStatus->m_chatBuild+addition;//operator+(m_gStatus->m_chatBuild,TranslateCharValue(Msg.wParam));
				}
				else
				{
                  m_initObject->m_gStatus->m_chatDeleteString = m_initObject->m_gStatus->m_chatDeleteString+"x";
				}
		}//end retrieve text dont miss*/
}

