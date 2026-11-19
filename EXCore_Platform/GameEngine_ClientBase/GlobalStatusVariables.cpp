#include "StdAfx.h"
#include "direct.h"
#include "globalstatusvariables.h"
#include "cabinet.hpp"
#include "CMenuScript.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

GlobalStatusVariables::GlobalStatusVariables(HWND & hwnd,BOOL editOn,BOOL sim)
{
	//Init paths
	char buffer[_MAX_PATH];
    _getcwd( buffer, _MAX_PATH );
    GL_workingDirectory = buffer;
    GL_pathMapModels						= GL_workingDirectory+"\\MapsModels\\";
	GL_pathGameFiles						= GL_workingDirectory+"\\GameFiles\\";
	GL_pathGameSounds						= GL_workingDirectory+"\\Sounds\\";
	//name above head presets
	m_nameAboveHeadMode                     = 1;
	m_nameAboveVisibleRange                 = 25.0f;//doubled
    m_nameAboveFontSizeX					= 0.16f;
	m_nameAboveFontSizeY					= 0.16f;
	m_nameAboveFontColorR					= .5f;
	m_nameAboveFontColorG					= .6f;
	m_nameAboveFontColorB					= 1.0f;
	m_nameAboveNewLineScale					= .75f;
    //end name above head presets

	m_iArenaTimer							= 0;
	m_iArenaTimeStamp						= 0;
	m_justSpawned                           = false;
    m_switchingConnections                  = 0;
	m_resetReferences                       = 0;
	m_globalAutoTriggerLast                 = -1;
	m_lodOverride                           = -1;
	m_showNames                             = 1;
	m_mapCaptureRequest                     = FALSE;
	m_hudInvCount                           = -1;
	m_hudInvStamp                           = 0;
	m_mapCaptureAll                         = FALSE;
	m_mapCaptureAllZonesRequest             = FALSE;
	m_mapCaptureBatch                       = FALSE;
	m_mapCaptureBusy                        = FALSE;
	m_mapCaptureFloors                      = 0;
	m_oneVal                                = 1;
	m_devMode                               = 0;
	m_tempMenuFlag							= -1;
	m_currentTempMenuFlag					= -1;
	m_axLogger.m_logFileName                = GL_workingDirectory+"\\GeneralClientLog.txt";
	m_axLogger.WriteDebugFile("Init",1,1);
	m_selctionModeOn						= false;
	m_idOfWorldObjectLastHit                = -1;
	m_autoDownloadMissingAssets             = FALSE;
    DWORD currentTime						= GetTickCount();
    m_menuClosedStamp						= currentTime;
    m_respawnTimeStamp						= currentTime;
	m_lastGlobalUnicodeIndexUse             = currentTime;
	m_controlTimeStamp						= currentTime;
	m_reloadTimerStamp						= currentTime;
	m_speedTimerStamp						= currentTime;
	m_actionDelayStamp						= currentTime;
	m_armDisarmStamp						= currentTime;
	m_lastCashUpdate						= 0;
	m_cashUpdateFrequency					= 65000;
    GL_spawnGeneratorTimeVarial             = currentTime;
	m_lastChatCommandStamp                  = currentTime;
	m_performanceTimeStamp                  = currentTime; 
	m_acceptConsoleTxt                      = FALSE;
	//targeting globals
	m_tgSys_quedButtonDownTimeStamp         = currentTime;
	m_tgSys_hoverQueryStamp                 = currentTime;//
	m_tgSys_hoverQueryDuration              = 150;
	m_tgSys_quedButtonDownTimeDuration      = 200;
	m_tgSys_quedButtonDown                  = 0;
	m_tgSys_expQue                          = -1;
	m_tgSys_hoverEntitySelected             = -1;
	m_tgSys_indexOfObjectSelected			= -1;//-1 = none
	m_tgSys_entitySelected					= -1;//-1 = none
    m_tgSys_type							= 0;
    m_tgSys_dist							= 0;
	m_tgSys_validRange                      = 500;//the longer the more computational
	m_tgSys_constantSelectionMode           = 0;//0=only on click 1=always calculating
	m_tgSys_rampMultiplier                  = 1.0f;
	m_tgSys_lastFov                         = -1.0f;
	m_tgSys_hoverState                      = 0;
	m_tgSys_clipWidth						= 1.0f;
	m_tgSys_clipHeight						= 1.0f;

	// Network Commands
	this->m_pNetworkCommands = new CNetworkCommands();

	//Minimap arrays

	m_minimapFilenames[MINIMAP_NEWBIEZONE]	= "NewbieZone.EXG";
	m_minimapScales[MINIMAP_NEWBIEZONE]		= 0.0f;
	m_minimapFilenames[MINIMAP_BOOMTOWN]	= "Boomtown.EXG";
	m_minimapScales[MINIMAP_BOOMTOWN]		= 0.0f;
	
	InitializeCriticalSection(&m_globalMeterSection);
	InitializeCriticalSection(&m_stateSection);

	//high resolution counter
	GL_exactFpsStamp.QuadPart = (__int64)0.0;
    GL_frequency.QuadPart = (__int64)0.0;
    if(QueryPerformanceFrequency(&GL_frequency)==0)
       GL_frequency.QuadPart = (__int64)0.0;//Failed
    if(GL_frequency.QuadPart != 0)
	   QueryPerformanceCounter(&m_networkFreqStampUser);

	ZeroMemory(&m_d3dVectorInit,sizeof(D3DVECTOR));

    GL_frequency.QuadPart = 0;//REMOVE
	m_highResTimerEnabled = true;
    if(timeBeginPeriod(1) == TIMERR_NOCANDO)
	{
	   if(timeBeginPeriod(2) == TIMERR_NOCANDO)
	   {
	      m_highResTimerEnabled = false;
	   }
	}

	ZeroMemory(&m_globalCursorPos,sizeof(D3DVECTOR));
	m_globalInteractCommand					= ":LOOT";
	m_contentUrl                            = "";//"http://69.12.14.137";//"http://72.29.80.113"--> bad web-server address; throws an exception at:c:\Projects\ABWebComm\HttpFileDownloader.cpp --> "http://72.29.80.113";
	m_advUrl                                = m_contentUrl + "/contentmanager.aspx?adv=loadingimg";
	m_advUrlBasic                           = m_contentUrl + "/contentmanager.aspx?adv=basicimg";
	m_advUrlVending                         = m_contentUrl + "/contentmanager.aspx?adv=vendimg";
    m_editorModeOn                          = 0;
	
	/*m_globalStatusMeterStartX				= -.2;
    m_globalStatusMeterStartY				= -.1;*/
	//WARNING: C4305; double to float
	m_globalStatusMeterStartX				= -0.2f;
    m_globalStatusMeterStartY				= -0.1f;

	m_globalMeterFontSizeX                  = .009f;
	m_globalMeterFontSizeY                  = .010f;
	m_initFontSystem                        = FALSE;
    m_showGlobalMeter                       = TRUE;
    m_globalAvVar							= TRUE;
	m_respawning							= FALSE;
	m_respawnWait							= 5000;
	m_localContentClientVersion				= 0;
	m_speechMode                            = 0;
	m_unicodeStartIndex                     = 0;
	m_hudYouSayPosX							= -.39f;
    m_hudYouSayPosY							= -.42f;

    m_hudYouSaySizeX						= .008f;
	m_hudYouSaySizeY                        = .01f;
	m_hudYouSayRed                          = 0.1f;
	m_hudYouSayGreen                        = 1.0f;
	m_hudYouSayBlue                         = 0.4f;


	m_maxUnicodeOptions						= 0;
	m_scndRec                               = 0;
	gl_svrJt								= FALSE;
    gl_frceZ								= 0.0f;
    gl_frceY								= 0.0f;
	m_speedOmeterOn                         = FALSE;
	m_startCamera							= -1;
	m_timeAdjust                            = 1.0;
	m_timeAdjustFloat                       = 1.0f;
	m_lastTimeAdjust                        = 1.0;
	m_actionDelay                           = 2000;
	m_tempTalk								= NULL;
    m_fpsReading							= NULL;
	m_pingReading							= NULL;
	m_editorModeOn                          = editOn;
	m_compressTextures                      = TRUE;
	//m_fullScreen                            = TRUE;
	m_globalShadowType						= 2;
	GL_renderEnvironment                    = TRUE;
	m_dirSnd                                = NULL;
	m_logRenderSection                      = FALSE;
	m_totalWorldPolys                       = 0;
	GL_globalSoundFactor                    = .03f;
	m_showFirstPersonShadow                 = FALSE;
	m_globalShadowDist                      = 200.0f;
	m_maxShadowCount                        = 10;
	m_backGroundList                        = NULL;
	m_currentTrack                          = -1;
	m_musicLoopFile                         = "NONE";
	m_globalWorldShader                     = -1;
	W_BufferPreffered                       = FALSE;
	GlobalTraceNumber                       = 1;
	m_backgroundColorBlack                  = D3DCOLOR_XRGB(0,0,0);
	m_backgroundColorGlobal					= D3DCOLOR_XRGB(0,0,0);
	GL_WORLD								= new CFrameObj(NULL);
	m_dedicatedServer						= FALSE;
	m_debuggingOn							= FALSE;
	m_blackOut                              = TRUE;
	m_enhancedMode							= FALSE;
	m_snapX                                 = 0;
    m_snapY                                 = 0;
	m_txbBitmapLoadOverride                 = FALSE;
	m_shadowTextureFromFile					= "128CircleShadow.tga";
	m_shadowIndexTemp                       = -1;
	m_collisionPolysInUse                   = 0;
    m_actualPolysCalculated                 = 0;
	m_closeDownCall                         = FALSE;
	gl_helperString							= "";
	m_backspaceQued                         = FALSE;
	GL_objectsRendered						= 0;
	m_renderMenuClickZoneInfo               = FALSE;
	m_soundDeviceAvailable                  = FALSE;
	m_lagged                                = FALSE;
	GL_testPerformance						= 0.0;
	GL_showFps                              = FALSE;
	ErrorOutPut                             = FALSE;
	Lagged                                  = FALSE; 
	LoadNewWorld                            = FALSE; 
	NewWorld								= "NONE";
	m_exactFPSHiRes							= 0;
	GL_mathDuration                         = 0.0;
	fps_cap                                 = 60.0f;
	fpsSimulate                             = 60.0f;
	m_globalRenderer                        = -1;
	m_forceScalarForAspect                  = 0.0f;
	m_longMultiplier                        = 1000;
	m_globalCountDownDuration				= 0;//example 20000 (20 seconds)
	m_globalCountDownNotifierStamp          = GetTickCount();

    ZeroMemory(&m_speedPositionStamp,sizeof(D3DVECTOR));
	m_soundListener							= NULL;
	m_particleTest                          = NULL;
	m_toRenderSurfaceDB						= NULL;
    m_filterSpeechDB						= NULL;
	m_entObjRuntime                         = NULL;
	m_entObjList                            = NULL;
	m_runtimeViewportDB                     = NULL;
	m_viewPortDB                            = NULL;
	m_controlsList							= NULL;
	m_multiplayerObject                     = NULL;
	m_netGameLogicObject                    = NULL;
	m_runtimeMenuDataBase                   = NULL;
	m_dxRenderer                            = NULL;
	m_symbolMapDB                           = NULL;
	m_textRenderList                        = NULL;
	m_waterZoneDB                           = NULL;
	m_environmentPtr                        = NULL;
	m_collisionBase                         = NULL;
	m_aiList                                = NULL;
	m_collisionDatabase                     = NULL;
	m_scrList                               = NULL;
	m_cameraList                            = NULL;
	m_menuDatabase                          = NULL;
	m_soundTrackList                        = NULL;
	m_skillDatabase                         = NULL;
	m_commCtrlDB                            = NULL;
	m_textureDataBase                       = NULL;
    m_dynamicTextureDataBase                = NULL;
	m_renderStateObject                     = NULL;
	m_staticTextureDataBase                 = NULL;
	m_cgGlobalShaderDB                      = NULL;
	m_translucentPolyList                   = NULL;
    m_translucentMirrorPolyList             = NULL;
	m_shadowObjectsList                     = NULL;
	m_renderContainerList                   = NULL;
	m_renderContainerAlphaFilterList        = NULL;
	m_libraryReferenceDB                    = NULL;
	m_curWldObjPrtList                      = NULL;
	m_worldTriggerList                      = NULL;
	m_runtimeLightSys                       = NULL;
	m_worldEnviormentList                   = NULL;
	m_visualsRenderList                     = NULL;
	m_charonaCache							= NULL;
	m_particleDatabase                      = NULL;
	m_inViewEntityList                      = NULL;
	m_houseingCache                         = NULL;
	m_housingDB								= NULL;
	m_lightningSystem                       = NULL;
	m_soundManager                          = NULL;
	m_wldObjList                            = NULL;
	m_worldMirrorList                       = NULL;
	m_worldRenderList                       = NULL;
	m_worldTransferList                     = NULL;
	m_shadowEntityList                      = NULL;
	m_worldListTemp                         = NULL;
	m_explosionDBList                       = NULL;
	m_explosionRuntimeList                  = NULL;
	m_missileDB                             = NULL;
    m_missileRuntime                        = NULL;
	m_dotDB									= NULL;
	m_iconDatabase							= NULL;
	m_spawnGeneratorDB                      = NULL;
	GL_ScenePersistList                     = NULL;
	m_sceneScriptExec                       = NULL;
	GL_aiWebDatabase                        = NULL;
	m_worldGroupList                        = NULL;
	m_shaderSystem                          = NULL;
	m_pxShaderSystem                        = NULL;
	MovieRList                              = NULL;
	TextureSwapList							= NULL;
	m_cgFontList                            = NULL;
	m_fontList                              = NULL;
	m_textInterfaceList                     = NULL;
	m_tempWldObjCache                       = NULL;
	//m_hudList                               = NULL;
	m_commerceDB                            = NULL;
	m_bankZoneList                          = NULL;
	m_safeZoneDB                            = NULL;
	m_serverItemGenerator                   = NULL;
	m_portalDatabase                        = NULL;
	m_elementDB                             = NULL;
	m_statDatabase                          = NULL;
	m_aiRaidDatabase                        = NULL;
	m_battleSchedulerSys                    = NULL;
	m_worldChannelRuleDB                    = NULL;
	m_objectLibrary                         = NULL;
	m_screenFlashModule                     = NULL;
	m_pendingObject                         = NULL;
	m_musicPlayer							= NULL;
	m_postProcessor                         = NULL;
	m_loadingTexture                        = NULL;
	m_loadingTextureUpdateTimeST            = GetTickCount()-20000;
	m_draggingItem							= NULL;
	m_lastItemID							= 0;
	m_lastItemAmount						= 0;
	m_releaseDraggingItem					= false;
	m_deleteDraggingItem					= false;
	m_systemReadout							= NULL;
	m_chatReadout							= NULL;
	m_menuChat								= NULL;
	m_menuChatInput							= NULL;
	m_3dDevice                              = NULL;
	m_itemManager							= NULL;
	m_inputReference                        = NULL;
	m_bIsDragging							= false;

	m_itemManager							= new CItemManager();
	m_cameraModule							= new CCameraAdvObj();
	m_textEffectRuntime						= new CTextEffectDB();
	m_toRenderSurfaceDB						= new CViewToSurfaceDB();
	m_langFilter                            = new CAXLanguageFilter();
	m_filterSpeechDB						= new CFilterSpeechList();
	m_entObjRuntime                         = new CMovementObjList();
	m_selectionEffectDB                     = new CSelectionEffectList();
	m_entObjRuntime->EnableDirectAccess(33000);//EXPERIMENT
	m_entObjList                            = new CMovementObjList();
	m_runtimeViewportDB                     = new CViewportList();
	m_viewPortDB                            = new CViewportList();
	m_controlsList							= new ControlDBList();
	m_multiplayerObject                     = new CMultiplayerObj();
	m_netGameLogicObject                    = new NetworkGameLogic(m_multiplayerObject);
	m_runtimeMenuDataBase                   = new CMenu3DObjList();
	m_runtimeMenuDatabase                   = new CMenuVisualList();
	m_runtimeMenuDatabase->m_referenceMemory = true;
	if(!sim)
	    m_dxRenderer                        = new DxDeviceClass(2048,1024,768,16,(void**)&m_3dDevice);
	m_symbolMapDB                           = new CSymbolMapperObjList();
	m_textRenderList                        = new CTextureRenderObjList();
	m_waterZoneDB                           = new CWaterZoneObjList();
	m_environmentPtr                        = new CEnvironmentObj();
	m_collisionBase                         = new CCollisionObj();
	m_aiList                                = new CAIObjList();
	m_collisionDatabase                     = new CCollisionObjList();
	m_scrList                               = new CAIScriptObjList();
	m_cameraList                            = new CCameraObjList();
	m_menuDatabase                          = new CMenuVisualList();//m_menuDatabase = new CMenu3DObjList();
	m_menuFontList							= new CMenuFontList();
	m_menuIconDatabase						= new CMenuVisualList();
	m_soundTrackList                        = new CSoundTrackObjList();
	m_skillDatabase                         = new CSkillObjectList();
	m_commCtrlDB                            = new CCommonCtrlList();
	m_textureDataBase                       = new CTextureDBObjList();
    m_dynamicTextureDataBase                = new CTextureDBObjList();
	m_renderStateObject                     = new CStateManagementObj();
	m_staticTextureDataBase                 = new CTextureDBObjList();
	m_cgGlobalShaderDB                      = new CGlobalMaterialShaderList();
	m_translucentPolyList                   = new CTranslucentPolyList();
    m_translucentMirrorPolyList             = new CTranslucentPolyList();
	m_shadowObjectsList                     = new CTranslucentPolyList();
	m_renderContainerList                   = new CTranslucentPolyList();
	m_renderContainerAlphaFilterList        = new CTranslucentPolyList();
	m_libraryReferenceDB                    = new CReferenceObjList();
	m_curWldObjPrtList                      = new CWldObjectList();
	m_worldTriggerList                      = new CWldObjectList();
	m_runtimeLightSys                       = new CRTLightObjList();
	m_worldEnviormentList                   = new CWldObjectList();
	m_visualsRenderList                     = new CEXMeshObjList();
	m_charonaCache                          = new CCharonaDB();
	m_particleDatabase                      = new EnvParticleSystemList();
	m_inViewEntityList                      = new CDynamicOverlayList();
	m_houseingCache                         = new CHousingCacheObjList();
	m_housingDB								= new CHousingObjList();/////////////////////////////
	m_lightningSystem                       = new CLightningObjList();
	m_soundManager                          = new CSoundBankList();
	m_wldObjList                            = new CWldObjectList();
	m_worldMirrorList                       = new CWldObjectList();
	m_worldRenderList                       = new CWldObjectList();
	m_worldTransferList                     = new CWldObjectList();
	m_shadowEntityList                      = new CMovementObjList();
	m_worldListTemp                         = new CWldObjectList();
	m_explosionDBList                       = new CExplosionObjList();
	m_explosionRuntimeList                  = new CExplosionObjList();
	m_missileDB                             = new CMissileObjList();
    m_missileRuntime                        = new CMissileObjList();
	m_dotDB									= new CDotClassList();
	m_iconDatabase							= new CIconSysList();
	m_spawnGeneratorDB                      = new CSpawnObjList();
	GL_ScenePersistList                     = new CPersistObjList();
	m_sceneScriptExec                       = new CEXScriptObjList();
	GL_aiWebDatabase                        = new CAIWebObjList();
	m_worldGroupList                        = new CWldGroupObjList();
	m_shaderSystem                          = new CShaderObjList();
	m_pxShaderSystem                        = new CShaderObjList();
	MovieRList                              = new CMovieRuntimeObjList();
	TextureSwapList							= new CTextureSwapObjList();
	m_cgFontList                            = new CGFontObjList();
	m_fontList                              = new CFontObjList();
	m_textInterfaceList                     = new CTextCollectionList();
	m_tempWldObjCache                       = new CEXMeshObjList();
	//m_hudList                               = new CHudObjList();
	m_commerceDB                            = new CCommerceObjList();
	m_bankZoneList                          = new CBankObjList();
	m_safeZoneDB                            = new CSafeZoneObjList();
	m_serverItemGenerator                   = new CServerItemGenObjList();
	m_portalDatabase                        = new CPortalSysList();
	m_elementDB                             = new CElementObjList();
	m_statDatabase                          = new CStatObjList();
	m_aiRaidDatabase                        = new CAIRaidObjList();
	m_battleSchedulerSys                    = new CBattleSchedulerList();
	m_worldChannelRuleDB                    = new CWorldChannelRuleList();
	m_objectLibrary                         = new CObjectCollectionList();
	m_screenFlashModule                     = new CScreenFlashObj();
	m_pendingObject                         = new CPendingCommandObjList();
	m_musicPlayer							= new CMusicAX();
	m_postProcessor                         = new CPostProcessModuleManager();
	m_contentDownloadManager                = new CAXContentClient();
	m_attribQueue							= new CAttribQueue();
	m_dynamicFontTexture                    = NULL;
	m_globalStatusMeter                     = NULL;

	//Init sound
	m_soundDeviceAvailable = MakeDirectSound(hwnd,&m_dirSnd);
	if(m_soundManager)
	   m_soundManager->Initialize(m_dirSnd,m_soundListener);
	
	if(m_soundTrackList)
	m_soundTrackList->Initialize(hwnd,m_dirSnd);
	
    
	ZeroMemory(&m_imediateRenderRECT,sizeof(RECT));

	CFileException excAuto;
     CFile flAuto;
	 CArchive the_ArchiveAuto(&flAuto,CArchive::load);
     BOOL resAuto = flAuto.Open("vis.cfg",CFile::modeRead, &excAuto);
	 if(resAuto)
	 {
	    CString vertSpacing,maxLineLength,verticleVisCount,cfFontSzX,cfFontSzY,label;
		the_ArchiveAuto.ReadString(label);
		the_ArchiveAuto.ReadString(label);
        the_ArchiveAuto.ReadString(vertSpacing);
		the_ArchiveAuto.ReadString(label);
		the_ArchiveAuto.ReadString(maxLineLength);
		the_ArchiveAuto.ReadString(label);
		the_ArchiveAuto.ReadString(verticleVisCount);
		the_ArchiveAuto.ReadString(label);
		the_ArchiveAuto.ReadString(cfFontSzX);
		the_ArchiveAuto.ReadString(label);
		the_ArchiveAuto.ReadString(cfFontSzY);
        the_ArchiveAuto.Close();
	    flAuto.Close();
	 }
	 //m_localContentClientVersion = EvaluateClientContentVersion();
	 m_localContentClientVersion = 0;

	 m_matLib.InitMat(m_matrixInit2.m);
	 m_matLib.InitMat(m_soundCenterMatrix.m);
     m_matLib.InitMat(GL_MirrorTransform.m);
	 m_matLib.InitMat(m_tgSys_camMatrixBasis.m);
	 m_matLib.InitMat(m_tgSys_projMatrixBasis.m);

	 InitSearchPaths();
	 
	//InitEditorStatus();
	m_networkFreqStamp = GetTimeStamp();

	//Create the mouse cursors
	m_showMenuCursor = 0;
	m_currentCursor	 = 0;
	for(int i = 0; i < CURSOR_MAX; i++)
	{
		m_mouseCursors[i]			= new CMenuImage();
		m_mouseCursors[i]->m_name	= "Mouse Cursor";
		m_mouseCursors[i]->m_width	= 10;
		m_mouseCursors[i]->m_height	= 10;
	}

}

//-----------destroy all textures from database---------------------------------------//
void 
GlobalStatusVariables::DestroyAllTexturesFromDatabase()
{
	if(!this->m_dynamicTextureDataBase) return;
	this->m_dynamicTextureDataBase->InvalidateAllTextures();
	this->m_dynamicTextureDataBase->SafeDelete();
	delete this->m_dynamicTextureDataBase;
	this->m_dynamicTextureDataBase = 0;
}

void 
GlobalStatusVariables::SpawnGenDatabaseInList(int sel, CDialog *pSpawnGeneratorDlg)
{
#define IDC_LIST_Spawn_listDB           9095

	if(this->m_editorModeOn==FALSE) return;
	int traceIt=0;
	if(pSpawnGeneratorDlg)
		pSpawnGeneratorDlg->SendDlgItemMessage(IDC_LIST_Spawn_listDB,LB_RESETCONTENT,0,0);	
	for(POSITION posLoc = this->m_spawnGeneratorDB->GetHeadPosition(); posLoc != NULL;){
        CSpawnObj * partPtr = (CSpawnObj *)this->m_spawnGeneratorDB->GetNext(posLoc);
		if(pSpawnGeneratorDlg)
			pSpawnGeneratorDlg->SendDlgItemMessage(IDC_LIST_Spawn_listDB,
				LB_ADDSTRING,
				0,
				(LPARAM)(LPCTSTR)operator+(this->m_cabLib.ConvToString(traceIt),
				operator+(" :",partPtr->List_spawnName)));
		traceIt++;
	}
	if(sel != -1)
		if(pSpawnGeneratorDlg)
			pSpawnGeneratorDlg->SendDlgItemMessage(IDC_LIST_Spawn_listDB,LB_SETCURSEL ,sel,0);
}

void 
GlobalStatusVariables::InListTextureDB(int sel, CDialog* pTextureDatabaseDlg) 
{
#define IDC_LIST_TXTDB_Database         6256

	if(!this->m_textureDataBase)	return;
	if(this->m_editorModeOn == FALSE) return;
	if(pTextureDatabaseDlg)
		pTextureDatabaseDlg->SendDlgItemMessage(IDC_LIST_TXTDB_Database,LB_RESETCONTENT,0,0);

	int indexTrace = 0;
  for(POSITION texPositionLoc = this->m_textureDataBase->GetHeadPosition();texPositionLoc != NULL;)
  {
    CTextureDBObj * txtPtrLoc = (CTextureDBObj *)this->m_textureDataBase->GetNext(texPositionLoc);
    CString tempLocal = this->m_cabLib.ConvToString(indexTrace)+"__";  
	CString format = "Unknown";
	CString sz = "";

	if(txtPtrLoc->m_textureObject)
	{
       format = txtPtrLoc->m_textureObject->GetFormatName();
       if(txtPtrLoc->m_textureObject->m_usage == 1)
	      format += " R-T ";
	   sz = " Size:"+this->m_cabLib.ConvToString(txtPtrLoc->m_textureObject->m_widthBasis) + "X" + this->m_cabLib.ConvToString(txtPtrLoc->m_textureObject->m_hieghtBasis);
	}
	if(pTextureDatabaseDlg)
	pTextureDatabaseDlg->SendDlgItemMessage(IDC_LIST_TXTDB_Database,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)(tempLocal+txtPtrLoc->List_textureName+" *"+format+sz));
    indexTrace++;
  }

  if(sel != -1)
	  if(pTextureDatabaseDlg)
	   pTextureDatabaseDlg->SendDlgItemMessage(IDC_LIST_TXTDB_Database,LB_SETCURSEL ,sel,0);
}

BOOL 
GlobalStatusVariables::SaveTextureBank(CString fileName, int compressed, int maxTextureSize)
{
    if(this->m_dynamicTextureDataBase->m_compressRef == 1)
      this->m_dynamicTextureDataBase->UpdateLiveDataToBaseData();
	this->m_dynamicTextureDataBase->ExportCompressedTXB(fileName, this->m_dxRenderer->m_pd3dDevice, compressed, maxTextureSize, compressed);

   return TRUE;
}

LONGLONG GlobalStatusVariables::GetTimeStamp()
{
	/*if(GL_frequency.QuadPart != 0)
	{//use high perfomance
		QueryPerformanceCounter(&m_stamp_date_temp);
		return m_stamp_date_temp.QuadPart;
	}//end use high performance*/
	if(m_highResTimerEnabled == true)
	   return (LONGLONG)timeGetTime();

	return (LONGLONG)GetTickCount();
}

DWORD GlobalStatusVariables::GetTimeStamp_DW()
{
	if(m_highResTimerEnabled == true)
	   return timeGetTime();

	return GetTickCount();
}

LONGLONG GlobalStatusVariables::GetElaspeTimeMS(LONGLONG & lastStampPoint)
{
	if(m_highResTimerEnabled == true)
	   return ((LONGLONG)timeGetTime())-lastStampPoint;


	return ((LONGLONG)GetTickCount())-lastStampPoint;
}

DWORD GlobalStatusVariables::GetElaspeTimeMS_DW(DWORD &lastStampPoint)
{
	DWORD dwResult = 0;
	if(this->m_highResTimerEnabled)
	{
		dwResult = timeGetTime() - lastStampPoint;
		//lastStampPoint = timeGetTime();
	}
	else
	{
		dwResult = GetTickCount() - lastStampPoint;
		//lastStampPoint = GetTickCount();
	}
	return dwResult;
}

void 
GlobalStatusVariables::ShowHoverText(int iVisualID, CString strMessage)
{
	// CLOSE the hover menu
	this->m_runtimeMenuDatabase->CloseByID(iVisualID, &this->m_showMenuCursor);

	// OPEN the hover menu
	this->OpenMenu(iVisualID, false);
	CMenuWindow *pHoverMenu = dynamic_cast<CMenuWindow*>(this->m_menuDatabase->GetVisualByID(iVisualID, false));
	if (!pHoverMenu) return;

	// GET the hover text visual
	//	NOTE: This will simply get the first CMenuText child of the hover menu.
	CMenuText *pHoverText = NULL;
	POSITION pos = NULL;
	if(!pHoverMenu->m_childList)
		return;
	
	for (pos = pHoverMenu->m_childList->GetHeadPosition(); pos != NULL;)
	{
		pHoverText = dynamic_cast<CMenuText*>(pHoverMenu->m_childList->GetNext(pos));
		if (pHoverText) break;
	}

	if(!pHoverText)
		return;

	// SET the menu's position based on the mouse position
	pHoverMenu->m_posX = this->m_mouseCursors[this->m_currentCursor]->m_posX;
	pHoverMenu->m_posY = this->m_mouseCursors[this->m_currentCursor]->m_posY + this->m_mouseCursors[this->m_currentCursor]->m_height + 5;

	// SET text visual to the passed in string
	pHoverText->SetText(strMessage.GetBuffer());
	strMessage.ReleaseBuffer();

	// SET the menu's width to the text's width
	int iEndCharIndex = 0;
	float fTextWidth = pHoverText->GetLongestWidth();
	pHoverMenu->m_width = (float)fTextWidth + 10;
	pHoverMenu->m_height = (float)(pHoverText->m_iNumLines * pHoverText->m_font->m_height);
	pHoverMenu->Resize();

	// REPOSITION the text based on the number of lines
	pHoverText->m_posY = ((pHoverText->m_iNumLines - 1) * pHoverText->m_font->m_height);
}

void GlobalStatusVariables::InitSearchPaths()
{
     m_textureSearchPath = NULL;
     m_searchPathCount   = 0;
	 char bufferX[_MAX_PATH];
     _getcwd( bufferX, _MAX_PATH );
     CString CurDirX = bufferX;

	 CFileException excAuto2;
     CFile flAuto2;
	 m_textureSearchPath = new CString[50];
	 CArchive the_ArchiveAuto2(&flAuto2,CArchive::load);
     BOOL resAuto = flAuto2.Open("TextureSearchPaths.aut",CFile::modeRead, &excAuto2);
	 if(resAuto)
	 {
	    CString tempString;
        
		int trace = 0;
		while(the_ArchiveAuto2.ReadString(tempString))
		{
            m_searchPathCount++;
			m_textureSearchPath[trace++] = CurDirX + tempString;
		}
        the_ArchiveAuto2.Close();
	    flAuto2.Close();
	 }
}

void GlobalStatusVariables::InitEditorStatus()
{
	 SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization


	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open("edtOff.aut",CFile::modeRead, &exc);
	 if(res == FALSE){
		m_editorModeOn = TRUE;
	 }
	 else
	 fl.Close();
	 SetCurrentDirectory(GL_workingDirectory);
}

GlobalStatusVariables::~GlobalStatusVariables(void)
{
	SafeDelete(this->m_pNetworkCommands);

    if(m_itemManager)
	{
       delete m_itemManager;
	   m_itemManager = 0;
	}

    if(m_cameraModule)
	{
       delete m_cameraModule;
	   m_cameraModule = 0;
	}

    if(m_textEffectRuntime)
	{
       delete m_textEffectRuntime;
	   m_textEffectRuntime = 0;
	}

    if(m_toRenderSurfaceDB)
	{
       delete m_toRenderSurfaceDB;
	   m_toRenderSurfaceDB = 0;
	}

    if(m_selectionEffectDB)
	{
       delete m_selectionEffectDB;
       m_selectionEffectDB = 0;
	}

	if(m_postProcessor)
	{
       delete m_postProcessor;
	   m_postProcessor = 0;
	}

	if(m_runtimeLightSys)
	{
	   if(m_dxRenderer)
          m_runtimeLightSys->FlushSys(m_3dDevice,m_renderStateObject);
	   delete m_runtimeLightSys;
	   m_runtimeLightSys = 0;
	}
	if(m_renderStateObject)
	{
	   m_renderStateObject->SafeDelete();
	   delete m_renderStateObject;
	   m_renderStateObject = 0;
	}
	if(m_backGroundList)
	 {
	    m_backGroundList->SafeDelete();
        delete m_backGroundList;
        m_backGroundList = 0;
	 }
	/*if(m_hudList)
	{
		m_hudList->SafeDelete();
		delete m_hudList;
		m_hudList = 0;
	}*/
	if(m_musicPlayer)
	{
       delete m_musicPlayer;
	   m_musicPlayer = 0;
	}
	if(m_pendingObject)
	{
       m_pendingObject->SafeDelete();
	   delete m_pendingObject;
	   m_pendingObject = 0;
	}
	if(m_screenFlashModule)
	{
       m_screenFlashModule->SafeDelete();
	   delete m_screenFlashModule;
	   m_screenFlashModule = 0;
	}
	if(m_objectLibrary)
	{
       m_objectLibrary->SafeDelete();
	   delete m_objectLibrary;
	   m_objectLibrary = 0;
	}
	if(m_worldChannelRuleDB)
	{
	   delete m_worldChannelRuleDB;
	   m_worldChannelRuleDB = 0;
	}
	if(m_battleSchedulerSys)
	{
       m_battleSchedulerSys->SafeDelete();
	   delete m_battleSchedulerSys;
	   m_battleSchedulerSys = 0;
	}
	if(m_aiRaidDatabase)
	{
       m_aiRaidDatabase->SafeDelete();
	   delete m_aiRaidDatabase;
	   m_aiRaidDatabase = 0;
	}
	if(m_statDatabase)
	{
       m_statDatabase->SafeDelete();
	   delete m_statDatabase;
	   m_statDatabase = 0;
	}
	if(m_elementDB)
	{
       m_elementDB->SafeDelete();
	   delete m_elementDB;
	   m_elementDB = 0;
	}
	if(m_portalDatabase)
	{
       m_portalDatabase->SafeDelete();
	   delete m_portalDatabase;
	   m_portalDatabase = 0;
	}
	if(m_serverItemGenerator)
	{
	   delete m_serverItemGenerator;
	   m_serverItemGenerator=0;
	}
	if(m_safeZoneDB)
	{
       m_safeZoneDB->SafeDelete();
	   delete m_safeZoneDB;
	   m_safeZoneDB = 0;
	}
	if(m_bankZoneList)
	{
       m_bankZoneList->SafeDelete();
	   delete m_bankZoneList;
	   m_bankZoneList = 0;
	}
	if(m_commerceDB)
	{
       m_commerceDB->SafeDelete();
	   delete m_commerceDB;
	   m_commerceDB = 0;
	}
	if(m_tempWldObjCache)
	{
       m_tempWldObjCache->RemoveAll();
       delete m_tempWldObjCache;
	   m_tempWldObjCache = 0;
	}
	if(m_fontList)
	{
	   m_fontList->SafeDelete();
       delete m_fontList;
	   m_fontList = 0;
	}
	if(m_textInterfaceList)
	{
	   m_textInterfaceList->SafeDelete();
       delete m_textInterfaceList;
	   m_textInterfaceList = 0;
	}
	if(m_cgFontList)
	{
       m_cgFontList->SafeDelete();
	   delete m_cgFontList;
	   m_cgFontList = 0;
	}
	if(TextureSwapList)
	{
       delete TextureSwapList;
	   TextureSwapList = 0;
	}
	if(MovieRList)
	{
       delete MovieRList;
	   MovieRList = 0;
	}
	if(GL_WORLD)
	{
	   GL_WORLD->SafeDelete();
       delete GL_WORLD;
	   GL_WORLD = 0;
	}
    if(m_shaderSystem)
	{
       m_shaderSystem->SafeDelete();
	   delete m_shaderSystem;
       m_shaderSystem=0;
	}
	if(m_pxShaderSystem)
	{
       m_pxShaderSystem->SafeDelete();
	   delete m_pxShaderSystem;
       m_pxShaderSystem=0;
	}
	if(m_worldGroupList)
	{ 
       m_worldGroupList->SafeDelete();
	   delete m_worldGroupList;
	   m_worldGroupList = 0;
	}
	if(GL_aiWebDatabase)
	{
       GL_aiWebDatabase->SafeDelete();
	   delete GL_aiWebDatabase;
	   GL_aiWebDatabase = 0;
	}
	if(m_sceneScriptExec)
	{
       m_sceneScriptExec->SafeDelete();
	   delete m_sceneScriptExec;
	   m_sceneScriptExec = 0;
	}
	if(GL_ScenePersistList)
	{
       GL_ScenePersistList->SafeDelete();
	   delete GL_ScenePersistList;
	   GL_ScenePersistList = 0;
	}
	if(m_spawnGeneratorDB)
	{
       m_spawnGeneratorDB->SafeDelete();
	   delete m_spawnGeneratorDB;
	   m_spawnGeneratorDB = 0;
	}
	
	if(m_iconDatabase)
	{
       m_iconDatabase->SafeDelete();
	   delete m_iconDatabase;
	   m_iconDatabase = 0;
	}
	if(m_entObjRuntime)
	{
	   m_entObjRuntime->SafeDelete();
	   delete m_entObjRuntime;
	   m_entObjRuntime=0;
	}
	if(m_entObjList)
	{
	   m_entObjList->SafeDelete();
	   delete m_entObjList;
	   m_entObjList=0;
	}
	if(m_missileRuntime)
	{
       m_missileRuntime->SafeDelete();
	   delete m_missileRuntime;
	   m_missileRuntime = 0;
	}
	if(m_missileDB)
	{
       m_missileDB->SafeDelete();
	   delete m_missileDB;
	   m_missileDB = 0;
	}
	if(m_dotDB)
	{
		m_dotDB->SafeDelete();
		delete m_dotDB;
		m_dotDB = 0;
	}
	if(m_explosionRuntimeList)
	{
       m_explosionRuntimeList->SafeDelete();
	   delete m_explosionRuntimeList;
	   m_explosionRuntimeList = 0;
	}
	if(m_explosionDBList)
	{
       m_explosionDBList->SafeDelete();
	   delete m_explosionDBList;
	   m_explosionDBList = 0;
	}
	if(m_worldListTemp)
	{//dont delete contents its referenced
	   m_worldListTemp->RemoveAll();
	   delete m_worldListTemp;
	   m_worldListTemp=0;
	}//end dont delete contents its referenced
	if(m_shadowEntityList)
	{
       m_shadowEntityList->RemoveAll();
	   delete m_shadowEntityList;
	   m_shadowEntityList = 0;
	}
	if(m_worldRenderList){
	   m_worldRenderList->RemoveAll();
       delete m_worldRenderList;
	   m_worldRenderList=0;
	}
    if(m_worldTransferList)
	{
	   m_worldTransferList->RemoveAll();
	   delete m_worldTransferList;
	   m_worldTransferList=0;
    }
	if(m_worldMirrorList)
	{
	   m_worldMirrorList->RemoveAll();
       delete m_worldMirrorList;
	   m_worldMirrorList = 0;
	}
	if(m_lightningSystem)
	{
       m_lightningSystem->SafeDelete();
	   delete m_lightningSystem;
	   m_lightningSystem = 0;
	}
	if(m_houseingCache)
	{
       m_houseingCache->SafeDelete();
	   delete m_houseingCache;
	   m_houseingCache = 0;
	}
	if(m_housingDB)
	{
       m_housingDB->SafeDelete();
	   delete m_housingDB;
	   m_housingDB = 0;
	}

    if(m_loadingTexture)
	{
       m_loadingTexture->SafeDelete();
	   delete m_loadingTexture;
	   m_loadingTexture = 0;
	}

	if(m_inViewEntityList)
	{
       m_inViewEntityList->SafeDelete();
	   delete m_inViewEntityList;
	   m_inViewEntityList = 0;
	}
	if(m_particleDatabase)
	{
       m_particleDatabase->SafeDelete();
	   delete m_particleDatabase;
	   m_particleDatabase = 0;
	}
	if(m_visualsRenderList)
	{
       m_visualsRenderList->RemoveAll();
	   delete m_visualsRenderList;
	   m_visualsRenderList = 0;
	}
	if(m_charonaCache)
	{
       m_charonaCache->ClearAll();
	   delete m_charonaCache;
	   m_charonaCache = 0;
	}
	if(m_worldEnviormentList)
	{
       m_worldEnviormentList->RemoveAll();
	   delete m_worldEnviormentList;
	   m_worldEnviormentList = 0;
	}
	if(m_worldTriggerList)
	{
       m_worldTriggerList->RemoveAll();
	   delete m_worldTriggerList;
	   m_worldTriggerList = 0;
	}
	if(m_curWldObjPrtList)
	{
       m_curWldObjPrtList->RemoveAll();
	   delete m_curWldObjPrtList;
	   m_curWldObjPrtList = 0;
	}
	if(m_libraryReferenceDB)
	{
       m_libraryReferenceDB->SafeDelete();
	   delete m_libraryReferenceDB;
	   m_libraryReferenceDB = 0;
	}
	if(m_renderContainerList)
	{
       m_renderContainerList->SafeDelete();
	   delete m_renderContainerList;
       m_renderContainerList = 0;
	}
	if(m_renderContainerAlphaFilterList)
	{
       m_renderContainerAlphaFilterList->SafeDelete();
	   delete m_renderContainerAlphaFilterList;
       m_renderContainerAlphaFilterList = 0;
	}
    if(m_translucentPolyList)
	{
       m_translucentPolyList->SafeDelete();
	   delete m_translucentPolyList;
       m_translucentPolyList = 0;
	}
    if(m_translucentMirrorPolyList)
	{
       m_translucentMirrorPolyList->SafeDelete();
	   delete m_translucentMirrorPolyList;
       m_translucentMirrorPolyList = 0;
	}
	if(m_shadowObjectsList)
	{
       m_shadowObjectsList->SafeDelete();
	   delete m_shadowObjectsList;
       m_shadowObjectsList = 0;
	}
    if(m_cgGlobalShaderDB)
	{
       m_cgGlobalShaderDB->SafeDelete();
	   delete m_cgGlobalShaderDB;
       m_cgGlobalShaderDB = 0;
	}
	if(m_soundTrackList)
	{
       m_soundTrackList->SafeDelete();
	   delete m_soundTrackList;
	   m_soundTrackList = 0;
	}
    if(m_tempTalk)
	{
	   m_tempTalk->SafeDelete();
	   delete m_tempTalk;
	   m_tempTalk=0;
	}
	if(m_fpsReading)
	{
	   m_fpsReading->SafeDelete();
	   delete m_fpsReading;
	   m_fpsReading=0;
	}
    if(m_pingReading)
	{
	   m_pingReading->SafeDelete();
	   delete m_pingReading;
	   m_pingReading=0;
	}
    if(m_filterSpeechDB)
	{
	   m_filterSpeechDB->SafeDelete();
	   delete m_filterSpeechDB;
	   m_filterSpeechDB=0;
	}
	if(m_runtimeViewportDB)
	{
	   m_runtimeViewportDB->SafeDelete();
	   delete m_runtimeViewportDB;
	   m_runtimeViewportDB=0;
	}
	if(m_viewPortDB)
	{
	   m_viewPortDB->SafeDelete();
	   delete m_viewPortDB;
	   m_viewPortDB=0;
	}
	SafeDelete(m_controlsList);
	/*
	if(m_controlsList)
	{
	   m_controlsList->SafeDelete();
	   delete m_controlsList;
	   m_controlsList=0;
	}
	*/
	if(m_multiplayerObject)
	{
	   m_multiplayerObject->SafeDelete();
	   delete m_multiplayerObject;
	   m_multiplayerObject=0;
	}
	if(m_netGameLogicObject)
	{
	   delete m_netGameLogicObject;
	   m_netGameLogicObject=0;
	}

	if(m_runtimeMenuDataBase)
	{
		m_runtimeMenuDataBase->SafeDelete();
		delete m_runtimeMenuDataBase;
		m_runtimeMenuDataBase=0;
	}

	if(m_runtimeMenuDatabase)
	{
        delete m_runtimeMenuDatabase;
		m_runtimeMenuDatabase = 0;
	}
	
	if(m_symbolMapDB)
	{
	   m_symbolMapDB->SafeDelete();
	   delete m_symbolMapDB;
	   m_symbolMapDB=0;
	}
	if(m_textRenderList)
	{
	   m_textRenderList->SafeDelete();
	   delete m_textRenderList;
	   m_textRenderList=0;
	}
	if(m_waterZoneDB)
	{
	   m_waterZoneDB->SafeDelete();
	   delete m_waterZoneDB;
	   m_waterZoneDB=0;
	}
	if(m_environmentPtr)
	{
	   m_environmentPtr->SafeDelete();
	   delete m_environmentPtr;
	   m_environmentPtr=0;
	}
	if(m_collisionBase)
	{
	   m_collisionBase->SafeDelete();
	   delete m_collisionBase;
	   m_collisionBase=0;
	}
	if(m_aiList)
	{
	   m_aiList->SafeDelete();
	   delete m_aiList;
	   m_aiList=0;
	}
	if(m_collisionDatabase)
	{
	   m_collisionDatabase->SafeDelete();
	   delete m_collisionDatabase;
	   m_collisionDatabase=0;
	}
	if(m_scrList)
	{
	   m_scrList->SafeDelete();
	   delete m_scrList;
	   m_scrList=0;
	}
	if(m_cameraList)
	{
	   m_cameraList->SafeDelete();
	   delete m_cameraList;
	   m_cameraList=0;
	}

	if(m_menuDatabase)
	{
	   delete m_menuDatabase;
	   m_menuDatabase = 0;
	}

	if(m_menuFontList)
	{
	   delete m_menuFontList;
	   m_menuFontList = 0;
	}

	for(int i = 0; i < CURSOR_MAX; i++)
	{
		if(m_mouseCursors[i])
		{
		   delete m_mouseCursors[i];
		   m_mouseCursors[i] = 0;
		}
	}

	if(m_dynamicFontTexture)
	{
       m_dynamicFontTexture->SafeDelete();
	   delete m_dynamicFontTexture;
	   m_dynamicFontTexture = 0;
	}
    
	if(m_skillDatabase)
	{
       m_skillDatabase->SafeDelete();
	   delete m_skillDatabase;
	   m_skillDatabase = 0;
	}
    if(m_commCtrlDB)
	{
       m_commCtrlDB->SafeDelete();
	   delete m_commCtrlDB;
	   m_commCtrlDB = 0;
	}
	if(m_textureDataBase)
	{//remove dont destroy
	   m_textureDataBase->RemoveAllTempTextures();
       m_textureDataBase->RemoveAll();
	   m_textureDataBase->SafeDelete();//removes direct access
	   delete m_textureDataBase;
	   m_textureDataBase=0;
	}//end remove dont detroy
	
	if(m_dynamicTextureDataBase){
	   m_dynamicTextureDataBase->SafeDelete();
	   delete m_dynamicTextureDataBase;
	   m_dynamicTextureDataBase=0;
	}
	if(m_wldObjList)
	{
       m_wldObjList->SafeDelete();
	   delete m_wldObjList;
	   m_wldObjList = 0;
	}
    if(m_staticTextureDataBase)
	{
       m_staticTextureDataBase->SafeDelete();
	   delete m_staticTextureDataBase;
	   m_staticTextureDataBase=0;
	}

	if(m_textureSearchPath)
	{
	   delete [] m_textureSearchPath;
	   m_textureSearchPath = 0;
	}
	if(m_soundManager)
	{
       m_soundManager->SafeDelete();
	   delete m_soundManager;
	   m_soundManager = 0;
	}
	if(m_soundListener)
	{
		m_soundListener->Release();
		m_soundListener = 0;
	}
	if(m_dirSnd)
	{
       m_dirSnd->Release();
	   m_dirSnd = 0;
	}
	if(m_dxRenderer)
	{
	   delete m_dxRenderer;
	   m_dxRenderer=0;
	}
	if(m_debuggingOn)
	{
	   m_debugfl.Close();
	}
	if(m_langFilter)
	{
       delete m_langFilter;
	   m_langFilter = 0;
	}
	if(m_contentDownloadManager)
	{
       delete m_contentDownloadManager;
	   m_contentDownloadManager = 0;

	}
	if(m_globalStatusMeter)
	{
	   m_globalStatusMeter->SafeDelete();
       delete m_globalStatusMeter;
	   m_globalStatusMeter = 0;

	}

	if(m_attribQueue)
	{
       delete m_attribQueue;
	   m_attribQueue = 0;
	}

	if(m_menuIconDatabase)
	{
       delete m_menuIconDatabase;
	   m_menuIconDatabase = 0;
	}
	

	DeleteCriticalSection(&m_globalMeterSection);
	DeleteCriticalSection(&m_stateSection);
	timeEndPeriod(1);
}

//--------------Activates a entity movement obj and assigns a control-----------------//
BOOL GlobalStatusVariables::ActivateMovementEntity( CMovementObj * existingEntity,
													int aiCfg,
													D3DMATRIX locationMatrix,
													int cameraIndex,
													int optWorldChannel,
													int indexLoc,//edb unique id
													int controlAssignment,
													int entityType,
													int idFromLoc,
													BOOL networkEntity,
													BOOL addHead,
													BOOL spawnDead,
													int generatorLinkReference,
													int networkDefinedID,
													CMovementObj ** generateModePtr,
													int * m_spawnCfg,
													int specialPhysicsOverride,
													int overrideAnim,
													CCollisionObj * collisionBase,
													CCollisionObjList * altCollisionDB,
													CMovementObjList * entObjRuntime,
													CString & worldName //level name
													)

{ 
if(indexLoc<0)
   return FALSE;
	   

//HRESULT er;
//get database object
/*POSITION posLoc = m_entObjList->FindIndex(indexLoc);
if(!posLoc)
return FALSE;
CMovementObj * ptrLoc = ( CMovementObj* ) m_entObjList->GetAt(posLoc);	*/	    
//start cloning
CMovementObj * ptrLoc = ( CMovementObj* ) m_entObjList->GetByID(indexLoc);
if(!ptrLoc)
    return FALSE;

//base model   
 CMovementObj * entityRuntimeObj = NULL;
 if(!existingEntity){//new entity
        ptrLoc->Clone(&entityRuntimeObj,NULL,m_spawnCfg);
     if(generatorLinkReference != -1)
        entityRuntimeObj->List_SpawnGeneratorLink = generatorLinkReference;
        entityRuntimeObj->List_idFromLoc = 0;
        entityRuntimeObj->List_controlType =  controlAssignment;
        entityRuntimeObj->List_AIconfig = entityType;
        entityRuntimeObj->List_networkEntity = networkEntity;
        entityRuntimeObj->List_networkDefinedID = idFromLoc;	 
        entityRuntimeObj->List_networkFireWeaponQue = 0;
        entityRuntimeObj->List_refModel = indexLoc;
		entityRuntimeObj->m_aiCfgRef = aiCfg;
        entityRuntimeObj->List_energy = entityRuntimeObj->List_maxEnergy;
        entityRuntimeObj->m_matrix=locationMatrix;
		entityRuntimeObj->m_spawnTimeStamp = GetTickCount();
		
		
 }//end new entity
 else{//existing addition entity
        entityRuntimeObj = existingEntity;
        entityRuntimeObj->m_matrix=locationMatrix;
 }//end existing addition entity

 //assign default collision reference
 entityRuntimeObj->m_collisionMovRefPtr    = collisionBase;
 entityRuntimeObj->m_collisionVisionRefPtr = collisionBase;
 //remove
 entityRuntimeObj->List_movementDB->List_lastPosition.x = locationMatrix._41;
 entityRuntimeObj->List_movementDB->List_lastPosition.y = locationMatrix._42;
 entityRuntimeObj->List_movementDB->List_lastPosition.z = locationMatrix._43;


    
 //applied to all
 entityRuntimeObj->List_traceNumber = entObjRuntime->GetFreeID();  

 if(networkDefinedID != -1)
    entityRuntimeObj->List_networkDefinedID = networkDefinedID;
   
 if(addHead == FALSE){//ADD TAIL
    entObjRuntime->AddTail_Indexed(entityRuntimeObj);
 }//END ADD TAIL
 else{//ADD HEAD -network entity should be;)
    entObjRuntime->AddHead_Indexed(entityRuntimeObj);
    entityRuntimeObj->List_networkFocus = TRUE;
 }//END ADD HEAD

 //base frame cam setup
 
 entityRuntimeObj->List_baseFrame = new CFrameObj(NULL);

 entityRuntimeObj->List_baseFrame->AddTransformReplace(entityRuntimeObj->m_matrix);
 entityRuntimeObj->List_baseFrame->GetPosition(NULL,&entityRuntimeObj->List_location);
 entityRuntimeObj->List_baseFrame->GetOrientation(&entityRuntimeObj->List_networkLastDirVect,&entityRuntimeObj->List_networkLastUpVect);
 entityRuntimeObj->List_baseFrame->GetOrientation(&entityRuntimeObj->List_networkTargetDirVect,&entityRuntimeObj->List_networkTargetUpVect);
 entityRuntimeObj->List_lastPosition = entityRuntimeObj->List_location;
 entityRuntimeObj->List_AIdestination = entityRuntimeObj->List_location;
 entityRuntimeObj->List_AI_origin = entityRuntimeObj->List_location;
 entityRuntimeObj->List_networkLastPositionXYZ = entityRuntimeObj->List_location; 
 entityRuntimeObj->List_networkTargetPositionXYZ = entityRuntimeObj->List_location;
 entityRuntimeObj->m_team = 0;//default ai will ovverride itself
  //clone direct meshes/content
  int loopCountLoc=0;
  
  //initialize light inteface
  D3DVECTOR posEntityScene;
  posEntityScene.x = 0.0f;
  posEntityScene.y = 0.0f;
  posEntityScene.z = 0.0f;
  
  CFrameObj* tempFrameBaby = entityRuntimeObj->List_baseFrame;

  if(controlAssignment==0){//if user
	  m_justSpawned = true;
	  m_cameraModule->m_followTrace = entityRuntimeObj->List_traceNumber;
      m_globalAvVar = TRUE;
	  
	  //Open Appropriate HUDs
	  if(entityRuntimeObj->List_hudLink >= 0 && entityRuntimeObj->m_firstPersonMode)
         OpenMenu(entityRuntimeObj->List_hudLink, false); //Open First Person Hud
	  else if(entityRuntimeObj->m_hudLinkThirdPerson >= 0 && !entityRuntimeObj->m_firstPersonMode)
		  OpenMenu(entityRuntimeObj->m_hudLinkThirdPerson, false); //Open Third Person Hud

	  if(aiCfg > 0)
	  {//special physics override
	  POSITION aiPos = m_aiList->FindIndex(aiCfg);
	   if(aiPos)
	   {
          CAIObj * aiPtr = (CAIObj *)m_aiList->GetAt(aiPos);
	      if(aiPtr->m_physicsModuleOverride)
		  {//Movment ovverride valid
		     if(entityRuntimeObj->List_movementDB)
			 {
              entityRuntimeObj->List_movementDB->SafeDelete();
			  delete entityRuntimeObj->List_movementDB;
			  entityRuntimeObj->List_movementDB=0;
			 }
             aiPtr->m_physicsModuleOverride->Clone(&entityRuntimeObj->List_movementDB);
			 entityRuntimeObj->List_movementDB->List_lastPosition.x = locationMatrix._41;
			 entityRuntimeObj->List_movementDB->List_lastPosition.y = locationMatrix._42;
			 entityRuntimeObj->List_movementDB->List_lastPosition.z = locationMatrix._43;
		  }//end Movment ovverride valid
	   }
	  }//end special physics ovveride 
      entityRuntimeObj->m_cameraTypeInUse = -1;

      if(entityRuntimeObj->List_camThirdPerson != m_startCamera || m_startCamera == -1)
		 entityRuntimeObj->m_firstPersonMode = TRUE;
	  else
		 entityRuntimeObj->m_firstPersonMode = FALSE;

	  if(entityRuntimeObj->m_firstPersonMode == TRUE && entityRuntimeObj->List_camFirstPerson > -1)
         ChangeToFirstPersonView(entityRuntimeObj);
	  else
         ChangeToThirdPersonView(entityRuntimeObj);

	  
  }//end if user
  else if(controlAssignment == 5){//AI type
	   POSITION aiPos = m_aiList->FindIndex(aiCfg);
	   if(aiPos){
          CAIObj * aiPtr = (CAIObj *)m_aiList->GetAt(aiPos);
		  if(altCollisionDB)
		  if(aiPtr->m_collisionDBOverride > -1)
		  {
             CCollisionObj * cPtr = altCollisionDB->GetByIndex(aiPtr->m_collisionDBOverride);
			 if(cPtr)
			 {
                entityRuntimeObj->m_collisionMovRefPtr    = cPtr;
                entityRuntimeObj->m_collisionVisionRefPtr = cPtr;
			 }
		  }

		  if(aiPtr->m_physicsModuleOverride)
		  {//Movment ovverride valid
		     if(entityRuntimeObj->List_movementDB)
			 {
              entityRuntimeObj->List_movementDB->SafeDelete();
			  delete entityRuntimeObj->List_movementDB;
			  entityRuntimeObj->List_movementDB=0;
			 }
             aiPtr->m_physicsModuleOverride->Clone(&entityRuntimeObj->List_movementDB);
			 entityRuntimeObj->List_movementDB->List_lastPosition.x = locationMatrix._41;
			 entityRuntimeObj->List_movementDB->List_lastPosition.y = locationMatrix._42;
			 entityRuntimeObj->List_movementDB->List_lastPosition.z = locationMatrix._43;
		  }//end Movment ovverride valid

		  aiPtr->Clone(&entityRuntimeObj->m_aiObj);
		  entityRuntimeObj->List_name = aiPtr->m_nameOvveride;
		  entityRuntimeObj->m_aiObj->m_lookAt.x = entityRuntimeObj->List_location.x;
		  entityRuntimeObj->m_aiObj->m_lookAt.y = entityRuntimeObj->List_location.y;
		  entityRuntimeObj->m_aiObj->m_lookAt.z = entityRuntimeObj->List_location.z;
		  entityRuntimeObj->m_team = entityRuntimeObj->m_aiObj->List_team;
		  if(entityRuntimeObj->m_aiObj->m_spawnScript != -1)//allocates
             entityRuntimeObj->m_aiObj->m_curScript = m_scrList->GetScriptObject(entityRuntimeObj->m_aiObj->m_spawnScript);
          if(entityRuntimeObj->m_aiObj->m_feelerEnable == TRUE)
			 entityRuntimeObj->m_aiObj->FeelerSystemContruction(entityRuntimeObj->m_aiObj->m_feelerRange,
			                                                    entityRuntimeObj->m_aiObj->m_feelerDepth,
																entityRuntimeObj->m_aiObj->m_feelerCycleTime,
																entityRuntimeObj->m_aiObj->m_feelerWidth,
																entityRuntimeObj->m_aiObj->m_feelerHieght);
	      
	   }
  }//end AI type
  
  

if(generateModePtr!=NULL)
*generateModePtr = entityRuntimeObj;
//entityRuntimeObj->List_skeletalObject
//NETWORK CLIENT SIDE


if(networkEntity == FALSE && m_multiplayerObject->GetEnabledClient_safe() == TRUE)
   if(addHead == TRUE || m_multiplayerObject->m_AIServer == TRUE)
   {//notify server and retreive network data
	  int useNewPresets = 0;
      if(controlAssignment == 0)
		 useNewPresets = -1;
	   
       //ai specific
	   int aiCash = 0;
	   int aiCfgIndex = 0;
	   if(entityRuntimeObj->m_aiObj)
	      aiCfgIndex = entityRuntimeObj->m_aiObj->m_dbIndex;

	   //Sending info to server, including the name of the world
	      m_multiplayerObject->SendSpawnMsg(DPNSEND_GUARANTEED | DPNSEND_PRIORITY_HIGH,
                                            NULL,
								            entityRuntimeObj->List_refModel,
								            worldName,
										    entityRuntimeObj->List_traceNumber,
										    useNewPresets,
										    optWorldChannel,
										    aiCfgIndex);
	  
   }//end notify server and retreive network data
//END NETWORK CLIENT SIDE

   if(m_multiplayerObject->GetEnabledClient_safe() == FALSE)
   if(entityRuntimeObj->m_autoCfg)//exec cfg
      entityRuntimeObj->ExecCfg(entityRuntimeObj->m_autoCfg,ptrLoc,m_itemManager,NULL);

   //set fade in
   entityRuntimeObj->FadeIn(1000);

   if(overrideAnim > -1)
   {
      entityRuntimeObj->m_ovverrideAnimation  = overrideAnim;
      entityRuntimeObj->m_ovverrideDuration   = 0;
	  entityRuntimeObj->m_ovverrideStamp      = GetTickCount();
   }

return TRUE;
}

//-----------Switch into First person-------------------------------------------------//
BOOL GlobalStatusVariables::ChangeToFirstPersonView(CMovementObj * currentEntity)
{
	if(currentEntity->m_firstPersonMode == TRUE)
		return TRUE;

	if(currentEntity->List_camFirstPerson == -1)
	    return FALSE;

	currentEntity->m_firstPersonMode = TRUE;

	//reset pitch and yaw rotations
	currentEntity->List_movementDB->List_freeLookCurPitch = 0.0f;
	currentEntity->List_movementDB->List_freeLookCurYaw = 0.0f;

	currentEntity->List_movementDB->List_curTransX = 0.0f;
	currentEntity->List_movementDB->List_curTransY = 0.0f;
	currentEntity->List_movementDB->List_curTransZ = 0.0f;
	currentEntity->List_movementDB->List_curRotX = 0.0f;
	currentEntity->List_movementDB->List_curRotY = 0.0f;
	currentEntity->List_movementDB->List_curRotZ = 0.0f;

	//reset any view selections
	m_tgSys_indexOfObjectSelected = -1;//-1 = none
	m_tgSys_entitySelected = -1;

	currentEntity->m_cameraTypeInUse = currentEntity->List_camFirstPerson;

	if(currentEntity->List_hudLink > -1 && currentEntity->m_hudLinkThirdPerson > 0)
	{
		//possibly switch HUDs
		m_runtimeMenuDatabase->CloseAll(&m_showMenuCursor, false);
		OpenMenu(currentEntity->List_hudLink, false);
	}

	return TRUE;
}
//-----------Switch into third person-------------------------------------------------//
BOOL GlobalStatusVariables::ChangeToThirdPersonView(CMovementObj * currentEntity)
{
	if(currentEntity->m_firstPersonMode == FALSE)
		return TRUE;

	if(currentEntity->List_camThirdPerson == -1)
	    return FALSE;

	currentEntity->m_firstPersonMode						= FALSE;

	//reset pitch and yaw rotations
	currentEntity->List_movementDB->List_freeLookCurPitch	= 0.0f;
	currentEntity->List_movementDB->List_freeLookCurYaw		= 0.0f;

	currentEntity->List_movementDB->List_curTransX = 0.0f;
	currentEntity->List_movementDB->List_curTransY = 0.0f;
	currentEntity->List_movementDB->List_curTransZ = 0.0f;
	currentEntity->List_movementDB->List_curRotX = 0.0f;
	currentEntity->List_movementDB->List_curRotY = 0.0f;
	currentEntity->List_movementDB->List_curRotZ = 0.0f;

	//reset any view selections
	m_tgSys_indexOfObjectSelected = -1;//-1 = none
	m_tgSys_entitySelected = -1;//-1 = none

	currentEntity->m_cameraTypeInUse = currentEntity->List_camThirdPerson;

	if(currentEntity->List_energy > 0)//log only if alive
		m_startCamera = currentEntity->List_camThirdPerson;


	if(currentEntity->List_hudLink > -1 && currentEntity->m_hudLinkThirdPerson > 0)
	{
		//possibly switch HUDs
		m_runtimeMenuDatabase->CloseAll(&m_showMenuCursor, false);
		OpenMenu(currentEntity->m_hudLinkThirdPerson, false);
	}

	return TRUE;
}
//---------------------open a menu--------------------------------------------------//
BOOL GlobalStatusVariables::OpenMenu(int visualID, bool allowMultipleInstances)
{
	//Find the menu if it exists
	CMenuWindow *curMenu = (CMenuWindow*)m_menuDatabase->GetVisualByID(visualID, false);
	if(!curMenu)
		return FALSE;

	//Loop through all menus that are currently shown
	POSITION posLastMenu;
	for(POSITION posCurMenu = m_runtimeMenuDatabase->GetHeadPosition();(posLastMenu = posCurMenu) != NULL;)
	{
		//Get a pointer to the current menu
		CMenuWindow *menu = (CMenuWindow*)m_runtimeMenuDatabase->GetNext(posCurMenu);

		//Remove focus from all other menus
		menu->m_isFocused = false;

		//Close any copy of the menu if we dont want to allow multiple instances
		if(!allowMultipleInstances && menu->m_visualID == visualID)
		{
			return TRUE;
		}
	}

	//CMenuWindow * curMenu = NULL;
	//stencilMenu->Clone((CMenuVisual **)&curMenu,m_3dDevice);//test instance

	//Add it to the runtime list, make it the focused menu
	curMenu->m_isFocused = true;
	m_runtimeMenuDatabase->AddHead(curMenu);

	//Load the textures for the menu cursors
	for(int i = 0; i < CURSOR_MAX; i++)
		m_mouseCursors[i]->UpdateTexture(m_textureDataBase->AddAssignTempTexture(m_3dDevice, m_mouseCursors[i]->m_textureFile, 0));

	//TODO: Adjust the menu's position according to alignment
	
	curMenu->ReinitMenuTextures(m_textureDataBase,m_menuFontList,m_3dDevice,m_draggingItem);

	//If the cursor's shown, increment the show cursor variable
	if(curMenu->m_showCursor)
		m_showMenuCursor++;

	//Resize the menu
	curMenu->Resize();

	//Run the load script, if it exists
	if(curMenu->m_loadScript)
		curMenu->m_loadScript->ExecuteScript();

	return TRUE;
}
//---------------------open a menu--------------------------------------------------//
BOOL GlobalStatusVariables::InstanceMenu(int visualID, int newVisualID)
{
	//Find the menu if it exists
	CMenuWindow *stencilMenu = (CMenuWindow*)m_menuDatabase->GetVisualByID(visualID, false);
	if(!stencilMenu)
		return FALSE;

	CMenuWindow * curMenu = NULL;
	stencilMenu->Clone((CMenuVisual **)&curMenu,m_3dDevice);//test instance
	curMenu->m_visualID = newVisualID;
	curMenu->m_instance = TRUE;//this should tell runtime when it closes the menu to delete it

	//Add it to the runtime list, make it the focused menu
	curMenu->m_isFocused = true;
	m_runtimeMenuDatabase->AddTail(curMenu);

	//Load the textures for the menu cursors
	for(int i = 0; i < CURSOR_MAX; i++)
		m_mouseCursors[i]->UpdateTexture(m_textureDataBase->AddAssignTempTexture(m_3dDevice, m_mouseCursors[i]->m_textureFile, 0));

	//TODO: Adjust the menu's position according to alignment
	curMenu->ReinitMenuTextures(m_textureDataBase,m_menuFontList,m_3dDevice,m_draggingItem);

	//If the cursor's shown, increment the show cursor variable
	if(curMenu->m_showCursor)
		m_showMenuCursor++;

	//Resize the menu
	curMenu->Resize();

	//Run the load script, if it exists
	if(curMenu->m_loadScript)
		curMenu->m_loadScript->ExecuteScript();

	return FALSE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::UseMenu(int dbIndex,int miscInt,int passedInVar,int passedInCommand)
{

	return TRUE;
}
//----------------------------------------------------------------//
void GlobalStatusVariables::LoadTexturesForMenu(CMenu3DObj * menu3DObj)
{
     if(menu3DObj->m_cursor)	
	    SkeletalTextureReassignmentTemp(menu3DObj->m_cursor->m_skeletonObject,menu3DObj->m_menuAccessID+5);

	 if(menu3DObj->m_additionalCursors)
	 {//chain of cursors
        for(POSITION acPos = menu3DObj->m_additionalCursors->GetHeadPosition();acPos != NULL;)
		{
            CBoneCursorObj * lCursorPtr = (CBoneCursorObj *)menu3DObj->m_additionalCursors->GetNext(acPos);
			SkeletalTextureReassignmentTemp(lCursorPtr->m_skeletonObject,menu3DObj->m_menuAccessID+5);
		}
	 }//end chain of cursors
	   //visual list 
	   for(POSITION vPos = menu3DObj->m_visualItems->GetHeadPosition();vPos != NULL;)
	   {
		   CMenuVisObj * vPtr = (CMenuVisObj *)menu3DObj->m_visualItems->GetNext(vPos);
           if(vPtr->m_skeletonObject)
			  SkeletalTextureReassignmentTemp(vPtr->m_skeletonObject,menu3DObj->m_menuAccessID+5);
		   if(vPtr->m_sliderSystem){
              if(vPtr->m_sliderSystem->m_visStart)
				 SkeletalTextureReassignmentTemp(vPtr->m_sliderSystem->m_visStart,menu3DObj->m_menuAccessID+5);
              if(vPtr->m_sliderSystem->m_visMov)
				 SkeletalTextureReassignmentTemp(vPtr->m_sliderSystem->m_visMov,menu3DObj->m_menuAccessID+5);
              if(vPtr->m_sliderSystem->m_visEnd)
				 SkeletalTextureReassignmentTemp(vPtr->m_sliderSystem->m_visEnd,menu3DObj->m_menuAccessID+5);

			  
		   }
		   if(vPtr->m_menuBarObj){
              if(vPtr->m_menuBarObj->m_barDynamic)
				 AddTextureToDatabaseTemp(vPtr->m_menuBarObj->m_barDynamic,menu3DObj->m_menuAccessID+5);
              if(vPtr->m_menuBarObj->m_barStatic)
				 AddTextureToDatabaseTemp(vPtr->m_menuBarObj->m_barStatic,menu3DObj->m_menuAccessID+5);
		   }

		   if(vPtr->m_type == 25)
		   {
				CCommonCtrlObj * cntrlObj = m_commCtrlDB->GetByIndex(vPtr->m_commCntlType);
				if(cntrlObj)
				{//comm ctrl loop
						if(cntrlObj->m_clickState)
						   SkeletalTextureReassignmentTemp(cntrlObj->m_clickState,menu3DObj->m_menuAccessID+5);
						if(cntrlObj->m_hoverState)
						   SkeletalTextureReassignmentTemp(cntrlObj->m_hoverState,menu3DObj->m_menuAccessID+5);
						if(cntrlObj->m_idleState)
						   SkeletalTextureReassignmentTemp(cntrlObj->m_idleState,menu3DObj->m_menuAccessID+5);
				}//end comm ctrl loop
		   }
	   }
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::ReinitMenuSys(CMenu3DObj * runtime,int miscInt)
{
//reinit menu sys
     for(POSITION posLoc = runtime->m_visualItems->GetHeadPosition();posLoc != NULL;)
	 {
         CMenuVisObj * visual = (CMenuVisObj *)runtime->m_visualItems->GetNext(posLoc);
		 
		 if(visual->m_type == 0 || 
			visual->m_type == 3 ||
			visual->m_type == 4 ||
			visual->m_type == 5 ||
			visual->m_type == 8 ||
			visual->m_type == 10||
			visual->m_type == 11||
			visual->m_type == 12||
			visual->m_type == 16||
			visual->m_type == 17||
			visual->m_type == 18||
			visual->m_type == 19||
			visual->m_type == 20||
			visual->m_type == 21||
			visual->m_type == 22||
			visual->m_type == 23||
			visual->m_type == 24||
			visual->m_type == 26||
			visual->m_type == 27||
			visual->m_type == 28||
			visual->m_type == 63||
			visual->m_type == 64||
			visual->m_type == 65||
			visual->m_type == 66||
		   (visual->m_type >= 67&&
			visual->m_type <= 71)||
		   (visual->m_type >= 30&&
			visual->m_type <= 57)){//text 
			
			if(!visual->m_textRenderLink)
			{
			visual->m_lockLocation.x = visual->m_startLocation.x;
			visual->m_lockLocation.y = visual->m_startLocation.y;
			visual->m_textRenderLink = new CTextureRenderObj("",
				                                             visual->m_textBlendMode,
			                                                 visual->m_startLocation.x,
				                                             visual->m_startLocation.y,
				                                             visual->m_fontSizeX,
				                                             visual->m_fontSizeY,
											                 TRUE,
														     1.0f,
															 1.0f,
															 1.0f);
			}
            

            if(visual->m_type == 44){//volume link
			   visual->m_textObject = m_cabLib.ConvToString(m_soundTrackList->m_musicPlayer->m_volumeSetting);
			}//end volume link
			if(visual->m_type == 45){//url music override link
			   visual->m_textObject = m_soundTrackList->m_musicPlayer->m_overrideStation;
		    }//end url music override lin
			if(visual->m_type == 3){//IP link
			   visual->m_textObject             = m_multiplayerObject->m_IPaddress;
			   visual->m_textRenderLink->m_text = m_multiplayerObject->m_IPaddress;
			} 
			if(visual->m_type == 4){//User name link
			   visual->m_textObject             = m_multiplayerObject->m_userName;
               visual->m_textRenderLink->m_text = m_multiplayerObject->m_userName;
			}
			if(visual->m_type == 5){//Password link
			   visual->m_textObject             = m_multiplayerObject->m_passWord;
			   visual->m_textRenderLink->m_text = m_multiplayerObject->m_passWord;
			}
			if(visual->m_type == 22){//sensitivity link
			   POSITION cntPos = m_controlsList->FindIndex(0);
			   if(cntPos){//1
                  ControlDBObj* cObj = (ControlDBObj*)m_controlsList->GetAt(cntPos);
			      visual->m_textObject = m_cabLib.ConvToString((int)(cObj->m_mouseSensitivity*102.0f));
			   }
			}
			if(visual->m_type == 27){//group joiner link
			   CMovementObj * dbPtr = m_entObjRuntime->GetObjByNetworkID(miscInt);
	           if(dbPtr)
			   {
                   visual->m_textObject = dbPtr->List_name;
			   }
			   else
				   visual->m_textObject = "-UNKNOWN-";

			}
            
			if(visual->m_defaultcontrol)
			   visual->m_currentlyBiengEdited = TRUE;
            
			visual->m_textRenderLink->Initialize(m_3dDevice,visual->m_textRenderLink->m_text,FALSE,m_symbolMapDB,m_speechMode,0.0f);
			m_textRenderList->AddTail(visual->m_textRenderLink);

		 }//end text
		 if(visual->m_type == 1){//entity clone 
		   if(!visual->m_runtimeDB){//db
			
		    CAIObj * aiPtr = m_aiList->GetByIndex(visual->m_aiEntityCfgRef);	 
			int selectedEDBIndex = visual->m_entityObjectDBIndex;
            if(aiPtr)
               selectedEDBIndex = aiPtr->m_entityCfgDBindexRef;
			   
			CMovementObj * mObj = m_entObjList->GetByID(selectedEDBIndex);
			if(mObj)
			{
	           //CMovementObj * mObj = (CMovementObj *)m_entObjList->GetAt(posTemp);
	           if(aiPtr)
                  mObj->Clone(&visual->m_entityObjectLink,NULL,aiPtr->m_spawnCfg);
			   else
			      mObj->Clone(&visual->m_entityObjectLink,NULL,NULL);
			   visual->m_entityObjectLink->List_refModel = selectedEDBIndex;
	           visual->m_entityObjectLink->List_skeletalObject->SetCurrentAnimation(visual->m_idleAnimIndex,100);
	           visual->m_entityObjectLink->List_baseFrame = new CFrameObj(NULL);
			    
			   if(aiPtr)
			   {//if ai ptr is ovverriding
                   if(aiPtr->m_inventoryCfg)
				   {//armed items
                      for(POSITION posInvT = aiPtr->m_inventoryCfg->GetHeadPosition();posInvT != NULL;)
					  {
                          CInventoryObj * invPtr = (CInventoryObj *) aiPtr->m_inventoryCfg->GetNext(posInvT);
                          if(invPtr->m_armed)
                             visual->m_entityObjectLink->ArmItemByGLID(invPtr->m_itemData->m_globalID,mObj,m_itemManager,NULL);
					  }
				   }//end armed items
			   }//end if ai ptr is ovverriding
			   else if(visual->m_entityObjectLink->m_autoCfg)//exec cfg
                       visual->m_entityObjectLink->ExecCfg(visual->m_entityObjectLink->m_autoCfg,mObj,m_itemManager,m_3dDevice);
			}
			else{
				if(m_systemReadout)
					m_systemReadout->AddLine("Invalid Entity Link");
               return FALSE;
			}
		   }//end db
		   else{//runtime
            POSITION posTemp = m_entObjRuntime->FindIndex(visual->m_entityObjectDBIndex);
			if(posTemp)
			{
	           CMovementObj * mObj = (CMovementObj *)m_entObjRuntime->GetAt(posTemp);
	           mObj->Clone(&visual->m_entityObjectLink,m_3dDevice,NULL);
			   visual->m_entityObjectLink->List_refModel = visual->m_entityObjectDBIndex;
	           visual->m_entityObjectLink->List_skeletalObject->SetCurrentAnimation(visual->m_idleAnimIndex,100);
			   visual->m_entityObjectLink->List_skeletalObject->ApplyFreeLookRotation(0.0f,0.0f);
	           visual->m_entityObjectLink->List_baseFrame = new CFrameObj(NULL);
			   if(mObj->List_armedInventoryDB)
			      mObj->List_armedInventoryDB->Clone(&visual->m_entityObjectLink->List_armedInventoryDB,m_3dDevice);
			}
			else
			{
				if(m_systemReadout)
					m_systemReadout->AddLine("Invalid Entity Link:", (DWORD)visual->m_entityObjectDBIndex);
				return FALSE;
			}
		   }//end runtime  
		 }
		 if(visual->m_type == 2){//skeletal clone 


			
		 }//end skeletal clone
		 if(visual->m_type == 7){//slider system display resolution Link
			 
			 
            if(visual->m_sliderSystem)
			for(int loop=0;loop<m_dxRenderer->m_loggedDisplayModes;loop++){
              if(m_dxRenderer->m_vmodeWidth[loop] != -2)
			  {
			  CString widthSt = m_cabLib.ConvToString(m_dxRenderer->m_vmodeWidth[loop])+" ";
		      CString hieghtSt = m_cabLib.ConvToString(m_dxRenderer->m_vmodeHeight[loop])+" ";
		      CString bppSt = m_cabLib.ConvToString(m_dxRenderer->m_vmodeDepth[loop])+" ";
		      CString final = widthSt+hieghtSt+bppSt;
		      visual->m_sliderSystem->AddString(final,-1,0,0,0,0);
			  }
			  else
			  {
              visual->m_sliderSystem->AddString("Windowed Mode",-1,0,0,0,0);
			  }
			}		
		 }//end slider system display resolution Link
		if(visual->m_sliderSystem)
		{//slider necessaries
			visual->m_sliderSystem->m_current.x = visual->m_sliderSystem->m_locStart.x;
			visual->m_sliderSystem->m_current.y = visual->m_sliderSystem->m_locStart.y;
		}//end slider necessaries
		if(visual->m_type == 9 || visual->m_type == 73)
		{//slider system controls Link
			if(visual->m_sliderSystem)
			{//valid
			   visual->m_sliderSystem->m_mode = 2;
			   visual->m_sliderSystem->m_currentSelectedContentItem = 0;
			   if(visual->m_type == 73)
			      InListActions(visual->m_sliderSystem->m_currentSelectedContentItem, visual->m_sliderSystem, TRUE);
			   else
				  InListActions(visual->m_sliderSystem->m_currentSelectedContentItem, visual->m_sliderSystem, FALSE);
			}
			/*if(visual->m_sliderSystem)
			{//valid
				visual->m_sliderSystem->m_mode = 2;
				visual->m_sliderSystem->m_currentSelectedContentItem = 0;

				POSITION cntPos = m_controlsList->FindIndex(0);
				if(cntPos)
				{//1
					ControlDBObj* cObj = (ControlDBObj*)m_controlsList->GetAt(cntPos);
					for(POSITION cPos = cObj->List_controlInfoList->GetHeadPosition(); cPos != NULL;)
					{
						ControlInfoObj *infoObj = (ControlInfoObj*)cObj->List_controlInfoList->GetNext(cPos);
						CString build;
						if(infoObj->List_miscFloat2 == 0)
							build = infoObj->GetActionDescription();
						else
						{
							CSkillObject * skillPtrRef = m_skillDatabase->GetSkillByType((int)infoObj->List_miscFloat3);
							build = "Skill Bind";
							if(skillPtrRef)
							if(skillPtrRef->m_levelRangeDB)
							{
								POSITION lvlPos = skillPtrRef->m_levelRangeDB->FindIndex((int)(infoObj->List_miscFloat2-1.0f));
								if(lvlPos)
								{
									CLevelRangeObject * lvlPtr = (CLevelRangeObject *)skillPtrRef->m_levelRangeDB->GetAt(lvlPos);
									if(lvlPtr->m_abilityDB)
									{
										CSkillAbilityObject * abilityPtr = lvlPtr->m_abilityDB->GetObjByIndex((int)infoObj->List_miscFloat1);
										if(abilityPtr)
											build = abilityPtr->m_abilityName;
									}
								}
							}
						}

						//CString assignmnt = cObj->GetButtonVis(infoObj->List_buttonDown,infoObj->List_controlType);
						//build = assignmnt + " :" + build;
						visual->m_sliderSystem->AddString(build, -1, 0, 0, 0, 0);
						visual->m_sliderSystem->AddString(cObj->GetButtonVis(infoObj->List_buttonDown,infoObj->List_controlType), -1, 0, 0, 0, 0);
					}
				}//end 1
			}//end valid*/
		}//end slider system controls Link
		if(visual->m_fadeTime > 0)
			visual->SetFadeIn(visual->m_fadeTime);
		if(visual->m_type == 29)//plain action display
			if(visual->m_sliderSystem)//valid
				InListActionsPlain(visual->m_sliderSystem->m_currentSelectedContentItem, visual->m_sliderSystem);
		if(visual->m_type == 14)//action display
			if(visual->m_sliderSystem)//valid
			{
				InListActions(visual->m_sliderSystem->m_currentSelectedContentItem, visual->m_sliderSystem, FALSE);

			}
		if(visual->m_type == 15)
		{//key available display
			if(visual->m_sliderSystem)
			{//valid
				POSITION cntPos = m_controlsList->FindIndex(0);
				if(cntPos)
				{//1
					ControlDBObj* cObj = (ControlDBObj*)m_controlsList->GetAt(cntPos);

					int cntLoop = 0;
					CString controlAction = cObj->List_controlInfoList->ReturnKeyStringByIndex(cntLoop);
					while(controlAction != "")
					{
						visual->m_sliderSystem->AddString(controlAction, -1, 0, 0, 0, 0);
						cntLoop++;
						controlAction = cObj->List_controlInfoList->ReturnKeyStringByIndex(cntLoop);
					}

					visual->m_sliderSystem->m_currentSelectedContentItem = 0;
				}//end 1
			}//end valid
		}//end key availabl display
		if(visual->m_type == 25)
		{//common cntrl visual ref
			CCommonCtrlObj * cmnContrl = m_commCtrlDB->GetByIndex(visual->m_commCntlType);
			if(cmnContrl)
			{
				BOOL stateCurrent = 0;
				if(runtime->m_clickActions)
				{//click object bind
					CCLickZoneObj * clickZonePtr = runtime->m_clickActions->GetByIndex(visual->m_boundToClickZoneStatus);
					if(clickZonePtr)
					{
						if(clickZonePtr->m_action == 7)
						{//mouse invert on/off
							clickZonePtr->m_switchOverride = TRUE;
							POSITION cntPos = m_controlsList->FindIndex(0);
							if(cntPos)
							{//1
								ControlDBObj* cObj = (ControlDBObj*)m_controlsList->GetAt(cntPos);
								if(cObj)
								{
									if(cObj->m_invertMouse)
									{
										cObj->m_invertMouse = TRUE;
										clickZonePtr->m_switchOverrideState = 3;
									}
									else
									{
										cObj->m_invertMouse = FALSE;
										clickZonePtr->m_switchOverrideState = 0;
									}
								}
							}//end 1
						}//end mouse invert on/off
						if(clickZonePtr->m_action == 66)
						{//enable disable music
							clickZonePtr->m_switchOverride = TRUE;
							if(m_soundTrackList->m_musicPlayer->m_disabled)
								clickZonePtr->m_switchOverrideState = 3;
							else
								clickZonePtr->m_switchOverrideState = 0;
						}//end enable disable music
						if(clickZonePtr->m_action == 62)
						{//mouse invert on/off
							clickZonePtr->m_switchOverride = TRUE;
							POSITION cntPos = m_controlsList->FindIndex(0);
							if(cntPos)
							{//1
								ControlDBObj* cObj = (ControlDBObj*)m_controlsList->GetAt(cntPos);
								if(cObj)
								{
									if(cObj->m_allowMultiBind)
									{
										cObj->m_allowMultiBind = TRUE;
										clickZonePtr->m_switchOverrideState = 3;
									}
									else
									{
										cObj->m_allowMultiBind = FALSE;
										clickZonePtr->m_switchOverrideState = 0;
									}
								}
							}//end 1
						}//end mouse invert on/off
					}
				}//end click object bind
			}
		}//end common cntrl visual ref
	}
	return TRUE;	 
}
//-----------------------------------------------------------------------------------
int GlobalStatusVariables::EvaluateClientContentVersion()
{
    int vs = 0;
	
	/*//Validate not trial
    char buffer[_MAX_PATH];
	_getcwd( buffer, _MAX_PATH );
    CString CurDir = buffer;
	CurDir = CurDir + "\\GameFiles\\*.*";
    
    if(m_cabLib.ValidateFileIsThere("MAINWORLD_BETA02.EXG",CurDir))//validate not cut trial
       vs = 0;*/

  return vs;
}
//-----------------------------------------------//
void GlobalStatusVariables::SkeletalTextureReassignment(CSkeletonObject * skeletonObj)
{
     SkeletalTextureReassignment(skeletonObj,10);

}
//-----------------------------------------------//
void GlobalStatusVariables::SkeletalTextureReassignmentWCompile(CSkeletonObject * skeletonObj)
{
     SkeletalTextureReassignment(skeletonObj,10);
     m_textureDataBase->CompileDirectAccess();
}
//-----------------------------------------------//
void GlobalStatusVariables::SkeletalTextureReassignment(CSkeletonObject * skeletonObj,int mipCount)
{
   //hiarch bone objects
   for(POSITION posLocal = skeletonObj->m_bonesList->GetHeadPosition();posLocal != NULL;){
	   CBoneObject * bonePtr = (CBoneObject *)skeletonObj->m_bonesList->GetNext(posLocal);
	   if(bonePtr->m_hiarchyMesh){
          for(POSITION posOni = bonePtr->m_hiarchyMesh->m_lodChain->GetHeadPosition();posOni != NULL;)
		  {
             CHiarcleVisObj * hObj = (CHiarcleVisObj *) bonePtr->m_hiarchyMesh->m_lodChain->GetNext(posOni);
		     //ReinitTextureUsage(hObj->m_mesh);
			 AddTextureToDatabase(hObj->m_mesh,mipCount);
		  }
	   }
   } 
   //spawn deformable cfg
   if(skeletonObj->m_alternateConfigurations)
   for(POSITION posLoc = skeletonObj->m_alternateConfigurations->GetHeadPosition();posLoc != NULL;)
   {
       CAlterUnitDBObject * aPtr = (CAlterUnitDBObject *)skeletonObj->m_alternateConfigurations->GetNext(posLoc);
       for(POSITION cfgPos = aPtr->m_configurations->GetHeadPosition();cfgPos != NULL;)
	   {
           CDeformableMesh * dMesh = (CDeformableMesh *)aPtr->m_configurations->GetNext(cfgPos);
		   if(dMesh->m_lodChain)
		   {
			   for(POSITION lPos = dMesh->m_lodChain->GetHeadPosition();lPos != NULL;)
			   {
				   CDeformableVisObj * vObj = (CDeformableVisObj *) dMesh->m_lodChain->GetNext(lPos);
				   //ReinitTextureUsage(vObj->m_mesh);
				   AddTextureToDatabase(vObj->m_mesh,mipCount);
			   }
		   }
	   }
   }
}
//-----------------------------------------------//
void GlobalStatusVariables::SkeletalTextureReassignmentTemp(CSkeletonObject * skeletonObj,int tempChannel)
{
   //hiarch bone objects
   for(POSITION posLocal = skeletonObj->m_bonesList->GetHeadPosition();posLocal != NULL;){
	   CBoneObject * bonePtr = (CBoneObject *)skeletonObj->m_bonesList->GetNext(posLocal);
	   if(bonePtr->m_hiarchyMesh){
          for(POSITION posOni = bonePtr->m_hiarchyMesh->m_lodChain->GetHeadPosition();posOni != NULL;)
		  {
             CHiarcleVisObj * hObj = (CHiarcleVisObj *) bonePtr->m_hiarchyMesh->m_lodChain->GetNext(posOni);
		     AddTextureToDatabaseTemp(hObj->m_mesh,tempChannel);
		  }
	   }
   } 
   //spawn deformable cfg
   if(skeletonObj->m_alternateConfigurations)
   for(POSITION posLoc = skeletonObj->m_alternateConfigurations->GetHeadPosition();posLoc != NULL;)
   {
       CAlterUnitDBObject * aPtr = (CAlterUnitDBObject *)skeletonObj->m_alternateConfigurations->GetNext(posLoc);
       for(POSITION cfgPos = aPtr->m_configurations->GetHeadPosition();cfgPos != NULL;)
	   {
           CDeformableMesh * dMesh = (CDeformableMesh *)aPtr->m_configurations->GetNext(cfgPos);
		   for(POSITION lPos = dMesh->m_lodChain->GetHeadPosition();lPos != NULL;)
		   {
	           CDeformableVisObj * vObj = (CDeformableVisObj *) dMesh->m_lodChain->GetNext(lPos);
			   AddTextureToDatabaseTemp(vObj->m_mesh,tempChannel);
		   }
	   }
   }
}
//-----------add texture--------------------------------------------------------------//
BOOL GlobalStatusVariables::AddTextureToDatabaseTemp(CEXMeshObj * meshObj,int tempChannel)
{
   for(int textureNum=0;textureNum<7;textureNum++)
   {//
       meshObj->List_textureSelect[textureNum] = m_textureDataBase->AddAssignTempTexture(m_3dDevice,
											                                             meshObj->List_storedTextureFromFile[textureNum],
																					     tempChannel);
   }
return TRUE;
}
//---------------------cloases all menus--------------------------------------------------//
void GlobalStatusVariables::InListActionsPlain(int currentSel,CSliderBarObj * slideBarObj)
{
	slideBarObj->FlushAll();
     POSITION cntPos = m_controlsList->FindIndex(0);
	 if(cntPos)
	 {//1
        ControlDBObj* cObj = (ControlDBObj*)m_controlsList->GetAt(cntPos);
        int cntLoop = 0;
        CString controlAction = cObj->List_controlInfoList->ReturnActionByIndex(cntLoop);

	    while(controlAction != "")
	    {
		      int miscTemp1 = 0;
			  int miscTemp2 = 0;
			  int miscTemp3 = 0;
			  int miscTemp4 = 0;
			  controlAction = cObj->List_controlInfoList->ReturnActionByIndexAdv(cntLoop,
												miscTemp1, 
												miscTemp2, 
												miscTemp3, 
												miscTemp4,
												m_skillDatabase);
			  if(controlAction != "")
			  {
				slideBarObj->AddString(controlAction,-1,miscTemp1,miscTemp2,miscTemp3,miscTemp4);
			    cntLoop++;
			  } 
		}
	    slideBarObj->m_currentSelectedContentItem = currentSel;
     }//end 1
}
//---------------------cloases all menus--------------------------------------------------//
void GlobalStatusVariables::InListActions(int currentSel,CSliderBarObj * slideBarObj,BOOL filterUnassigned)
{
	slideBarObj->FlushAll();
	slideBarObj->m_mode = 2;

    POSITION cntPos = m_controlsList->FindIndex(0);
	if(cntPos)
	{//1
		ControlDBObj* cObj = (ControlDBObj*)m_controlsList->GetAt(cntPos);
		int cntLoop = 0;
		CString controlAction = cObj->List_controlInfoList->ReturnActionByIndex(cntLoop);

		
		while(controlAction != "")
	    {
			int miscTemp1 = 0;
			int miscTemp2 = 0;
			int miscTemp3 = 0;
			int miscTemp4 = 0;
			//controlAction = cObj->List_controlInfoList->ReturnActionByIndex(cntLoop);
			controlAction = cObj->List_controlInfoList->ReturnActionByIndexAdv(	cntLoop,
																				miscTemp1, 
																				miscTemp2, 
																				miscTemp3, 
																				miscTemp4,
																				m_skillDatabase);
			cntLoop++;
			if(controlAction != "")
			{
				CString tempV = cObj->List_controlInfoList->GetKeyAssignmentByActionString(controlAction);
				if(filterUnassigned)
                   if(tempV.CompareNoCase("Unassigned") == 0)
				      continue;
				//CString keyAssigned = cObj->List_controlInfoList->GetKeyAssignmentByActionString(controlAction);
				//slideBarObj->AddString(keyAssigned + " :" + controlAction,-1,miscTemp1,miscTemp2,miscTemp3,miscTemp4);
				slideBarObj->AddString(controlAction, -1, miscTemp1, miscTemp2, miscTemp3, miscTemp4);
				slideBarObj->AddString(tempV, -1, miscTemp1, miscTemp2, miscTemp3, miscTemp4);
				
			} 
		}
		slideBarObj->m_currentSelectedContentItem = currentSel;
	}//end 1
}
//--------------updates a content object----------------------------------------------//
void GlobalStatusVariables::ReinitTextureUsage(CEXMeshObj * mesh) 
{
	//mesh
	AddTextureToDatabase(mesh);
	//charona extension
	if(mesh->m_charona)
	{//valid charona
		m_textureDataBase->TextureSearch(mesh->m_charona->m_textureName,mesh->m_charona->m_textureIndex,5,m_dynamicTextureDataBase,0);
	}//end valid charona
}
//-----------add texture--------------------------------------------------------------//
BOOL GlobalStatusVariables::AddTextureToDatabase(CEXMeshObj * meshObj)
{
     return AddTextureToDatabase(meshObj,10);
}
//-----------add texture--------------------------------------------------------------//
BOOL GlobalStatusVariables::AddTextureToDatabase(CEXMeshObj * meshObj,int mipMapCount)
{
	for(int textureNum = 0; textureNum < 7; textureNum++)
	{//
		char charbuf[256];
		CString temp = meshObj->List_storedTextureFromFile[textureNum];
		int chCount = temp.GetLength();
		if(chCount == 0)
			continue;
		lstrcpy(charbuf, meshObj->List_storedTextureFromFile[textureNum]);

		//make sure it doesnt already exist
		int indexTraceLoc = 0;
		BOOL continueIt = FALSE;
		for(POSITION posLoc = m_textureDataBase->GetHeadPosition(); posLoc != NULL;)
		{
			CTextureDBObj * ptrLoc = ( CTextureDBObj* ) m_textureDataBase->GetNext(posLoc);
			int result = strncmp(charbuf, ptrLoc->List_textureName, 256);
			if(result == 0)
			{//exists
				ptrLoc->m_tempVar = 1;
				meshObj->List_textureSelect[textureNum] = indexTraceLoc;
				continueIt = TRUE;
				break;//dont add
			}
			indexTraceLoc++;
		}

		if(continueIt == TRUE)
			continue;

		m_textureDataBase->TextureSearch(charbuf, 
										 meshObj->List_textureSelect[textureNum],
										 mipMapCount,
										 m_dynamicTextureDataBase,
										 0);
		//end make sure it doesnt already exist
		
		/*CString cmp = charbuf;
		if(cmp == "NONE" || cmp == "none" )
			return FALSE;
		CTextureDBObj * newPtr = new CTextureDBObj(charbuf);
		newPtr->m_textureObject = new CTextureEX();
		CString CurDir = GL_pathMapModels;
	   
		CString convString = charbuf;
		CString final = operator+(CurDir,convString);
		BOOL resultX = newPtr->m_textureObject->LoadIt(final,10,m_3dDevice,FALSE);

		if(!resultX)
		if(m_textureSearchPath)
		for(int xP=0;xP<m_searchPathCount;xP++)
		{
			CurDir = m_textureSearchPath[xP];
			final = operator+(CurDir,convString);
			resultX = newPtr->m_textureObject->LoadIt(final,10,m_3dDevice,FALSE);
			if(resultX)
				break;
		}
	   
		if(resultX)
		{//success
			if(newPtr->m_textureObject->Restore(m_3dDevice,
												newPtr->List_BltOpOn,
												newPtr->m_bump,
												newPtr->m_bumpType,
												m_dxRenderer->m_maxTextureSizeSquared,
												m_renderStateObject->m_pCaps,
												m_compressTextures)==S_OK)
			{//2
				newPtr->m_tempVar = 1;
				m_textureDataBase->AddTail(newPtr);
				m_dynamicTextureDataBase->AddTail(newPtr);
				meshObj->List_storedTextureFromFile[textureNum] = convString;
				meshObj->List_textureSelect[textureNum] = indexTraceLoc;
			}//end 2
			else
			{
				m_systemReadout->AddLine(operator+("Error loading::",final));
				//MessageBox(operator+("Error loading::",final));
				newPtr->m_textureObject->SafeDelete();
				delete newPtr->m_textureObject;
				newPtr->m_textureObject=0;
				delete newPtr;
				newPtr=0;
			}
		}//end success
		else
		{
			newPtr->m_textureObject->SafeDelete();
			delete newPtr->m_textureObject;
			newPtr->m_textureObject=0;
			delete newPtr;
			newPtr=0;
			meshObj->List_textureSelect[textureNum] = -1;
		}*/
	   	//InListTextureDB(m_textureDataBase->GetCount()-1);
	}
	return TRUE;
}

//clear bound cameras
void GlobalStatusVariables::ClearBoundCamerasViewports(int & entityTrace)
{
	POSITION posLast = NULL;
	for(POSITION posVptLoc = m_runtimeViewportDB->GetHeadPosition();(posLast = posVptLoc) != NULL;)
	{
		CViewportObj * vpPtr = (CViewportObj *)m_runtimeViewportDB->GetNext(posVptLoc);
		if(vpPtr->m_traceNumberef == entityTrace)
		{
			CViewportObj * vpPtrFocus = (CViewportObj *)m_runtimeViewportDB->GetAt(posLast);
			m_runtimeViewportDB->RemoveAt(posLast);
			vpPtrFocus->SafeDelete();
			delete vpPtrFocus;
			vpPtrFocus=0;
		}
	}
}

//--------------Destroys runtime entity-----------------------------------------------//
void GlobalStatusVariables::DestroyRuntimeEntity(CMovementObj * runtimeEntity,
												 CSpawnObjList * spawnGeneratorDB,
												 CMovementObjList * runtimeEntityList)
{ 
	if(!runtimeEntity)
		return;	

	if(spawnGeneratorDB)
	   runtimeEntityList->NotifySpawnList(spawnGeneratorDB,runtimeEntity);

	runtimeEntityList->InvalidateIndexedByID(runtimeEntity);

	ClearBoundCamerasViewports(runtimeEntity->List_traceNumber);
	
	delete runtimeEntity;
	runtimeEntity=0;
}
//--------------Destroys runtime entity-----------------------------------------------//
void GlobalStatusVariables::DestroyRuntimeEntity(CMovementObj * runtimeEntity,
												 CSpawnObjList * spawnGeneratorDB,
												 POSITION posLast,
												 CMovementObjList * runtimeEntityList)
{ 
	if(!runtimeEntity)
		return;	

	if(spawnGeneratorDB)
	   runtimeEntityList->NotifySpawnList(spawnGeneratorDB,runtimeEntity);
	//BOOL removedFromList = m_entObjRuntime->InvalidateIndexedByID(runtimeEntity);
	// DeleteByPtr() frees the entity when it finds it, so read everything we
	// still need from it first (it used to read List_traceNumber afterwards).
	int traceNumber = runtimeEntity->List_traceNumber;
	BOOL removedFromList = runtimeEntityList->DeleteByPtr(runtimeEntity);

	ClearBoundCamerasViewports(traceNumber);
	
	
	if(!removedFromList)
	{
        runtimeEntityList->RemoveAt(posLast);
		delete runtimeEntity;
	    runtimeEntity=0;
	}
}
//------------------------------------------------------------------------------------//
POSITION GlobalStatusVariables::AddStringToMenuList(int controlID,
													CString string,
													int visualRefIndex,
													int miscInt,
													int miscInt2,
													CString regItemName,
													int mode,
													CString costExt,
													bool special,
													//bool clearList,
													int miscInt3,
													int miscInt4)
{
	//Find the queue entry that's waiting for this attribute message
	CAttribQueueEntry *curEntry = NULL;
	POSITION posLast = NULL;
	POSITION posQueue = m_attribQueue->GetHeadPosition();
	while(posQueue != NULL)
	{
		posLast = posQueue;
		curEntry = dynamic_cast<CAttribQueueEntry*>(m_attribQueue->GetNext(posQueue));
		if(curEntry && curEntry->m_requestedAttrib == controlID)
			break;
		if(posQueue == NULL)
			posLast = NULL;
	}

	//If we didn't find the entry, return
	CMenuListBox *visual = NULL;
	if(posLast == NULL)
		visual = dynamic_cast<CMenuListBox*>(m_runtimeMenuDatabase->GetVisualByID(controlID, true));
	else
		if(curEntry) visual = dynamic_cast<CMenuListBox*>(curEntry->m_requester);

	if(!visual)
	{
		if(posLast && m_attribQueue)
		{
			delete curEntry;
			curEntry = 0;
			m_attribQueue->RemoveAt(posLast);
		}
		return NULL;
	}

	//Create Standard Visuals: ListBox, ListBoxItem, Text
	CMenuListItem *menuItem = new CMenuListItem();
	CMenuText *text = new CMenuText();
	
	//Set misc Ints and special to their values
	menuItem->m_miscInt1 = miscInt;
	menuItem->m_miscInt2 = miscInt2;
	menuItem->m_miscInt3 = miscInt3;
	menuItem->m_miscInt4 = miscInt4;
	menuItem->m_special	 = special;

	bool setDefaultIcon = false;

	//Determine the mode, this is no longer set but is being used as a check
	if(mode == 0)
	{
		text->m_text = string;
		menuItem->m_textList.AddTail(text);
	}
	else
	{
		text->m_text = " " + regItemName;
		menuItem->m_textList.AddTail(text);
		text = new CMenuText();

		if(miscInt2 > 0)
			text->m_text = costExt + "   Quantity " + m_cabLib.ConvToString(miscInt2);
		else
			text->m_text = costExt;

		menuItem->m_textList.AddTail(text);

		//Set the Icon Based on VisualRefIndex
		if(visualRefIndex > 0)
		{ 
			//Get Icon Based off VisualRefIndex
			CMenuImage *visualIcon = (CMenuImage*)m_menuIconDatabase->GetVisualByID(visualRefIndex, false);

			if(visualIcon)
			{
				//Valid Visual Based on Reference
				menuItem->m_icon = visualIcon;

				//Make sure the texture is loaded and update it
				menuItem->m_icon->m_textureSelect = m_textureDataBase->AddAssignTempTexture(m_3dDevice, menuItem->m_icon->m_textureFile, 0);
				menuItem->m_icon->UpdateTexture(menuItem->m_icon->m_textureSelect);
			}
			else
			{
				//Set a Default
				setDefaultIcon = true;
			}
		}
		else
		{ 
			//Set a Default Icon
			setDefaultIcon = true;
		} 

		//Set a default Icon if Needed
		if(setDefaultIcon)
		{
			POSITION posCurVisual = m_menuIconDatabase->GetHeadPosition();
			if(posCurVisual)
			{
				CMenuImage *defaultIcon = (CMenuImage*)m_menuIconDatabase->GetNext(posCurVisual);
				
				if(defaultIcon)
				{
					//There is a valid icon
					menuItem->m_icon = defaultIcon;
				}
			}
			else
			{
				//No Valid Icons in DB - This Is A Error
				menuItem->m_icon = new CMenuImage();
				menuItem->m_icon->m_width = 32.0f;
				menuItem->m_icon->m_height = 32.0f;
			}
		}
	}

	//Add the Item
	visual->AddItem(menuItem);

	return posLast;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::OverrideFireQue(	int					& fireQueIndex,
												D3DVECTOR			& target,
												CMovementObj		* entityPtr,
												CMissileObjList		* missileRuntime)
{
	CFireRangeObject * range = NULL;
	if(entityPtr->List_armedInventoryDB)
	   for(POSITION posLoc = entityPtr->List_armedInventoryDB->GetHeadPosition();posLoc != NULL;)
	   {
			CArmedInventoryObj * armedPtr = (CArmedInventoryObj *) entityPtr->List_armedInventoryDB->GetNext(posLoc);

			range = NULL;
			if(armedPtr->m_thirdPersonSystem)
			{
					if(armedPtr->m_thirdPersonSystem->m_fireQueList)
						range = armedPtr->m_thirdPersonSystem->m_fireQueList->GetByFireQueIndex(fireQueIndex);
			}
			if(!range)
			if(armedPtr->m_firstPersonSystem)
			{
					if(armedPtr->m_firstPersonSystem->m_fireQueList)
						range = armedPtr->m_firstPersonSystem->m_fireQueList->GetByFireQueIndex(fireQueIndex);
			}
			
			if(!range)
			    continue;
			
			D3DMATRIX projectileMatrix;
			if(entityPtr->m_firstPersonMode)
			{
				projectileMatrix = m_cameraModule->GetCameraMatrix();

				D3DMATRIX rMat;
				m_matLib.Rotate ( 0, 180, 0, rMat.m);
				m_matLib.fastMatrixMultiply(projectileMatrix,rMat,projectileMatrix);
			}
			else if(!entityPtr->GetFireTransform(range->m_boneIndex,armedPtr, projectileMatrix))
			{
				return FALSE;
			}

			m_matLib.FaceTowardPoint(target,&projectileMatrix);

			ExecuteFireQue( entityPtr,
							projectileMatrix,
							range,
							fireQueIndex,
							FALSE,
							range->m_missileIndex,
							missileRuntime);
			return TRUE;
	   }

   return FALSE;
}
//------------------------------------------------------------------------------------//
void GlobalStatusVariables::ExecuteFireQue( CMovementObj     * entityPtr,
											CFireRangeObject * fireRange,
											float            & pitchRotation,
											D3DMATRIX        & positionalMatrix,
											int              & entityTraceNumber,
											D3DVECTOR        * overrideTarget,
											int              & fireRangeIndex,
											BOOL               sendOverNetwork,
											float            & yawRotation,
											int              & missileDBIndex,
											CMissileObjList  * missileRuntime)
{
	               
                   
                  float baseKickPower = 0.0f;
                  D3DMATRIX tempMatrix;
                  m_matLib.Rotate(pitchRotation,0.0f,0.0f,tempMatrix.m);
                  
                  m_matLib.fastMatrixMultiply(tempMatrix,tempMatrix,positionalMatrix);
				  
				  D3DMATRIX rotation;
                  m_matLib.Rotate(0.0f,yawRotation,0.0f,rotation.m);
				  
				  D3DVECTOR tempXYZ;
				  tempXYZ.x = tempMatrix._41;
				  tempXYZ.y = tempMatrix._42;
				  tempXYZ.z = tempMatrix._43;
				  tempMatrix._41 = 0.0f;
				  tempMatrix._42 = 0.0f;
				  tempMatrix._43 = 0.0f;

                  m_matLib.fastMatrixMultiply(tempMatrix,tempMatrix,rotation);
				  tempMatrix._41 = tempXYZ.x;
				  tempMatrix._42 = tempXYZ.y;
				  tempMatrix._43 = tempXYZ.z;
				  
				  D3DMATRIX transform;
				  if(fireRange)
				  {
                    m_matLib.Translate(fireRange->m_hardPointAdjust.x,0.0f,fireRange->m_hardPointAdjust.z,transform.m);
                    m_matLib.fastMatrixMultiply(tempMatrix,transform,tempMatrix);
                    if(fireRange->m_accuracyRating > 0.0f)
					{
                       D3DMATRIX acMatrixApply = m_matLib.GetAccuracyAdjust(fireRange->m_accuracyRating);
                       m_matLib.fastMatrixMultiply(tempMatrix,acMatrixApply,tempMatrix);
					}

                    if(entityPtr->m_hardPointOverdY == 0)
					   tempMatrix._42 += fireRange->m_hardPointAdjust.y;
					else
					   tempMatrix._42 += entityPtr->m_hardPointOverdY;
				  }
				  else{
                    m_matLib.Translate(0.0f,0.0f,-.1f,transform.m);
                    m_matLib.fastMatrixMultiply(tempMatrix,transform,tempMatrix);
                    tempMatrix._42 += entityPtr->List_skeletalObject->m_box.maxY*.7f;//fireRange->m_hardPointAdjust.y;
				  }
				  
                  
		
                  if(overrideTarget)
				  {//ai overide
                     m_matLib.FaceTowardPoint(*overrideTarget,&tempMatrix);
				  }//end ai overide

				  float missileSpeedReturn = 0.0f;
                  BOOL canShoot = TRUE;
				  if(sendOverNetwork)
				  {//is a user - check for network
                     if(m_multiplayerObject->GetEnabledClient_safe())
					 {//send network info
					   if(m_multiplayerObject->m_itemGlobalMaxCharges != 0)
					   {//a
                          if(m_multiplayerObject->m_itemGlobalCurCharges <= 0) 
						  {//1
							// canShoot = FALSE;
                           //  m_systemReadout->AddLine("Out of ammo");
						  }//end 1a
					   }//end a
					   
						
					   if(canShoot)
					   {
						D3DVECTOR translateAhead;
						translateAhead.x = 0;
						translateAhead.y = 0;
						translateAhead.z = -1000;
                        translateAhead = m_matLib.UpdatePolygonVertex(tempMatrix.m,translateAhead);
						m_netGameLogicObject->SendFireMsg(  DPNSEND_NOCOMPLETE,
															translateAhead,
															fireRangeIndex,
															DPID_SERVERPLAYER,
															entityPtr->List_networkDefinedID);
					   }
					 }//end send network info
				  }//end is a user - end check for network

				  if(!m_multiplayerObject->m_rechargeTimer)
				  {
					  DoMissile(tempMatrix,
						    missileDBIndex,//fireRange->m_missileIndex,
						    0,
						    entityTraceNumber,
						    -9,
						    m_d3dVectorInit,
						    0,
						    &missileSpeedReturn,
							entityPtr->m_team,
							baseKickPower,
							missileRuntime);
						m_multiplayerObject->m_itemGlobalCurCharges -= m_multiplayerObject->m_itemNeededPerFire;
						if (m_multiplayerObject->m_itemGlobalCurCharges < 0)
							m_multiplayerObject->m_itemGlobalCurCharges = 0;
				  }
				  else if(m_multiplayerObject->m_itemGlobalMaxCharges == 0)
					  DoMissile(tempMatrix,
						    missileDBIndex,//fireRange->m_missileIndex,
						    0,
						    entityTraceNumber,
						    -9,
						    m_d3dVectorInit,
						    0,
						    &missileSpeedReturn,
							entityPtr->m_team,
							baseKickPower,
							missileRuntime);

                  if(entityPtr->m_aiObj)
					 entityPtr->m_aiObj->m_currentMissileSpeed = missileSpeedReturn;



				  if(entityPtr->List_controlType == 0)
				  {
				    CViewportObj* mptLoc = m_runtimeViewportDB->GetByEntTrace(entityPtr->List_traceNumber);
				    if(mptLoc)
				    {
	                   mptLoc->m_cameraObject->m_curBurstRate = baseKickPower;
		               //mptLoc->m_cameraObject->m_curBurstRateY = 10.0f;
		               mptLoc->m_cameraObject->m_kickBurstDet = .5f;
                       mptLoc->m_cameraObject->m_kickReturnRate = .85f;
	                }
				  }

}
//------------------------------------------------------------------------------------//
void GlobalStatusVariables::ExecuteFireQue( CMovementObj     * entityPtr,
										    D3DMATRIX        & directionalMatrix,
											CFireRangeObject * fireRange,
											int              & fireRangeIndex,
											BOOL               sendOverNetwork,
											int              & missileDBIndex,
											CMissileObjList  * missileRuntime)
{                  
                  float baseKickPower = 0.0f;

				  float missileSpeedReturn = 0.0f;
                  BOOL canShoot = TRUE;
				  if(sendOverNetwork)
				  {//is a user - check for network
                     if(m_multiplayerObject->GetEnabledClient_safe())
					 {//send network info
					   if(m_multiplayerObject->m_itemGlobalMaxCharges != 0)
					   {//a
                          if(m_multiplayerObject->m_itemGlobalCurCharges <= 0) 
						  {//1
							// canShoot = FALSE;
                          //   m_systemReadout->AddLine("Out of ammo");
						  }//end 1a
					   }//end a
					   
						m_multiplayerObject->m_itemGlobalCurCharges -= m_multiplayerObject->m_itemNeededPerFire;
					   if(canShoot)
					   {
						D3DVECTOR translateAhead;
						translateAhead.x = 0;
						translateAhead.y = 0;
						translateAhead.z = -1000;
                        translateAhead = m_matLib.UpdatePolygonVertex(directionalMatrix.m,translateAhead);
						m_netGameLogicObject->SendFireMsg(  DPNSEND_NOCOMPLETE,
															translateAhead,
															fireRangeIndex,
															DPID_SERVERPLAYER,
															entityPtr->List_networkDefinedID);
					   }

					 }//end send network info
				  }//end is a user - end check for network

				  if(!m_multiplayerObject->m_rechargeTimer)
				  {
					  DoMissile(directionalMatrix,
						    missileDBIndex,//fireRange->m_missileIndex,
						    0,
						    entityPtr->List_traceNumber,
						    -9,
						    m_d3dVectorInit,
						    0,
						    &missileSpeedReturn,
							entityPtr->m_team,
							baseKickPower,
							missileRuntime);
					  if (m_multiplayerObject->m_itemGlobalCurCharges < 0)
						m_multiplayerObject->m_itemGlobalCurCharges = 0;
				  }
				  else if(m_multiplayerObject->m_itemGlobalMaxCharges == 0)
					  DoMissile(directionalMatrix,
						    missileDBIndex,//fireRange->m_missileIndex,
						    0,
						    entityPtr->List_traceNumber,
						    -9,
						    m_d3dVectorInit,
						    0,
						    &missileSpeedReturn,
							entityPtr->m_team,
							baseKickPower,
							missileRuntime);

                  if(entityPtr->m_aiObj)
					 entityPtr->m_aiObj->m_currentMissileSpeed = missileSpeedReturn;



				  if(entityPtr->List_controlType == 0)
				  {
				    CViewportObj* mptLoc = m_runtimeViewportDB->GetByEntTrace(entityPtr->List_traceNumber);
				    if(mptLoc)
				    {
	                   mptLoc->m_cameraObject->m_curBurstRate = baseKickPower;
		               //mptLoc->m_cameraObject->m_curBurstRateY = 10.0f;
		               mptLoc->m_cameraObject->m_kickBurstDet = .5f;
                       mptLoc->m_cameraObject->m_kickReturnRate = .85f;
	                }
				  }

}
//--------------Create Runtime missile from DB----------------------------------------//
BOOL GlobalStatusVariables::DoMissile(  D3DMATRIX matrix,
										int indexDB,
										float curSpeed,
										int trace,
										int targetTrace,
										D3DVECTOR secondaryAdjust,
										float rotationAfter,
										float *missileSpeed,
										int teamref,
										float &kickRet,
										CMissileObjList * missileRuntime)
{
         CMissileObj* misPtr = m_missileDB->GetPtr(indexDB);
		 if(!misPtr)
		     return FALSE;
		 kickRet = misPtr->m_baseKickPower;
         *missileSpeed = misPtr->m_physicDynamics->List_maxSpeed;
         CMissileObj* runtimeMissile = NULL;
	     misPtr->Clone(&runtimeMissile,m_3dDevice);

		 runtimeMissile->m_teamRef = teamref;
		 
		 runtimeMissile->m_missileDBRef = indexDB;
		 runtimeMissile->m_ownerTrace = trace;
         runtimeMissile->m_baseFrame = new CFrameObj(NULL);
         D3DMATRIX transform;
         m_matLib.Translate(secondaryAdjust.x,secondaryAdjust.y,secondaryAdjust.z,transform.m);
		 m_matLib.fastMatrixMultiply(matrix,transform,matrix);
         runtimeMissile->m_baseFrame->AddTransformReplace(matrix);
		 //runtimeMissile->m_baseFrame->AddTranslationBefore(secondaryAdjust.x,secondaryAdjust.y,secondaryAdjust.z);
         runtimeMissile->m_baseFrame->GetPosition(NULL,&runtimeMissile->m_lastPosition);
         runtimeMissile->m_baseFrame->GetPosition(NULL,&runtimeMissile->m_physicDynamics->List_lastPosition);
		 runtimeMissile->m_startTimeStamp = GetTickCount();
		 if(runtimeMissile->m_laserObject){
			runtimeMissile->m_baseFrame->GetTransform(NULL,&runtimeMissile->m_laserObject->m_orientationAndOrigin);
            //runtimeMissile->m_laserObject->m_meshObject->InitBuffer(m_3dDevice);
		 }

		 if(runtimeMissile->m_skeletalVisual){
			runtimeMissile->m_skeletalVisual->SetAnimationDirection(0,FALSE);
			runtimeMissile->m_skeletalVisual->SetCurrentAnimation(0,5);
		 }

		 
		 missileRuntime->AddTail(runtimeMissile);
return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::GenAndMount(CMovementObj * hostPtr,
										int aiIndex,
										CMovementObjList * entObjRuntime,
										CMissileObjList * missileRuntime)
{
	if(hostPtr->m_posses)
	   return FALSE;

	if(aiIndex < 0)
	   return FALSE;

    CAIObj * aiPtr = m_aiList->GetByIndex(aiIndex);
    if(!aiPtr)
		return FALSE;

	//if(hostPtr->List_skeletalObject)
	//  hostPtr->List_skeletalObject->SetAnimationBasedOnAttrib(30,0,0,100);

	D3DMATRIX matrixOfHost;
	hostPtr->List_baseFrame->GetTransform(NULL,&matrixOfHost);

    int controlType = 5;
	BOOL newtorkEntity = m_multiplayerObject->GetEnabledClient_safe();
	if(m_multiplayerObject->GetEnabledClient_safe())
	{
	   controlType = -1;
	   newtorkEntity = TRUE;
	}

	
	if(hostPtr->List_controlType == 0)
	{
	   newtorkEntity = FALSE;
       controlType = 5;
	}

	CMovementObj * refPtr;
    BOOL result = ActivateMovementEntity(NULL,
			                             aiIndex,
			                             matrixOfHost,
		                                 0,
										 0,
									     aiPtr->m_entityCfgDBindexRef,//spawnGenPtr->List_spawnIndex,
										 controlType,//hardcode ai to player-spawnGenPtr->List_spawnType,
										 0,
										 0,
									     newtorkEntity,//Network entity
										 FALSE,
										 FALSE,
										 -1,
										 -1,
										 &refPtr,
					                     aiPtr->m_spawnCfg,
										 -1,
										 -1,
										 m_collisionBase,
										 m_collisionDatabase,
										 entObjRuntime,
										 m_currentWorkingFile);

	if(!result)
		return FALSE;

	if(!refPtr)
        return FALSE;

    hostPtr->Mount(refPtr);
	refPtr->CharacterAnimationManagement(refPtr->List_skeletalObject,m_timeAdjustFloat);

	if(refPtr)
    if(m_multiplayerObject->m_AIServer)
       IndexEntityAnimations(refPtr,GetTickCount(),TRUE,missileRuntime);

	return TRUE;
}
//--------------entity child animation index update-----------------------------------//
BOOL GlobalStatusVariables::IndexEntityAnimations(CMovementObj * ptrLoc,
												  long currentTickCount,
												  BOOL soundOnly,
												  CMissileObjList * missileRuntime)
{//BEGIN 

if(!ptrLoc->List_skeletalObject) 
    return FALSE;
long tCount = currentTickCount;



//end timing delay
//world list indexes based on loop
if(ptrLoc->m_animationsDone == (char)ptrLoc->List_skeletalObject->m_curLod)
   return TRUE;

D3DVECTOR worldSpacePosition;
ptrLoc->List_baseFrame->GetPosition(NULL,&worldSpacePosition);

if(ptrLoc->m_animationsDone > -1)
{//was animated already in mirror probably, so just update lod mesh
	ptrLoc->List_skeletalObject->m_curLod = ptrLoc->List_skeletalObject->m_currentLOD;
    if(m_lodOverride > -1)
	   ptrLoc->List_skeletalObject->m_curLod = m_lodOverride;
    ptrLoc->List_skeletalObject->UpdateBasedDeformationByVertexAssignments(ptrLoc->List_skeletalObject->m_curLod,m_3dDevice);
   return TRUE;
}

ptrLoc->m_animationsDone = (char)ptrLoc->List_skeletalObject->m_curLod;




	if(ptrLoc->List_skeletalObject){//valid 1  
	D3DMATRIX positionalMatrix;
	 ptrLoc->List_baseFrame->GetTransform(NULL,&positionalMatrix);
	  if(soundOnly){//sound only
	   if(ptrLoc->List_networkEntity == FALSE)
	     EntityAnimationManagmentCallback(ptrLoc);

	     BOOL disableSoundOption = FALSE;
         if(m_multiplayerObject->m_AIServer == TRUE)
			disableSoundOption = TRUE;

	     ptrLoc->List_skeletalObject->AnimationCallback(tCount,
			                                            m_soundCenterMatrix,
														worldSpacePosition,
														GL_globalSoundFactor,
														TRUE,
														m_soundManager,
														disableSoundOption,
														m_multiplayerObject->m_AIServer,
														m_3dDevice,
														ptrLoc->List_movementDB->List_curVelocityVect,
														ptrLoc->List_controlType,
														m_lodOverride);

	  }//end sound only
	  else{//normal  

       if(ptrLoc->List_networkEntity == FALSE)
	   EntityAnimationManagmentCallback(ptrLoc);
       ptrLoc->List_skeletalObject->AnimationCallback(tCount,
		                                              m_soundCenterMatrix,
													  worldSpacePosition,
													  GL_globalSoundFactor,
													  FALSE,
													  m_soundManager,
													  FALSE,
													  m_multiplayerObject->m_AIServer,
													  m_3dDevice,
													  ptrLoc->List_movementDB->List_curVelocityVect,
													  ptrLoc->List_controlType,
													  m_lodOverride);
	   if(ptrLoc->List_skeletalObject->m_shadowOn == TRUE){//1
		   D3DVECTOR shdPos;
		   ptrLoc->List_baseFrame->GetPosition(NULL,&shdPos);
		   ptrLoc->List_skeletalObject->m_shadowInterface->Calculate(shdPos,m_collisionBase);
	   }//end 1
	  }//end normal
	   //appendages
       if(ptrLoc->List_armedInventoryDB)
		  for(POSITION armedPos = ptrLoc->List_armedInventoryDB->GetHeadPosition();armedPos != NULL;)
		  {//armed appendages
             CArmedInventoryObj * armedObj = (CArmedInventoryObj *)ptrLoc->List_armedInventoryDB->GetNext(armedPos);
			 if(armedObj->m_thirdPersonSystem){
				if(soundOnly){//sound only
				   BOOL disableSound = FALSE;
	               if(ptrLoc->m_firstPersonMode)
					  disableSound = TRUE;
				   if(m_multiplayerObject->m_AIServer == TRUE)
			          disableSound = TRUE;
	               armedObj->m_thirdPersonSystem->AnimationCallback(tCount,
					                                                m_soundCenterMatrix,
																	worldSpacePosition,
																	GL_globalSoundFactor,
																	TRUE,
																	m_soundManager,
																	disableSound,
																	m_multiplayerObject->m_AIServer,
																	m_3dDevice,
																	ptrLoc->List_movementDB->List_curVelocityVect,
																	ptrLoc->List_controlType,
																	m_lodOverride);

				   if(ptrLoc->List_controlType == 0)
				   if(armedObj->m_abilityTypeUse == 12)
				      if(armedObj->m_thirdPersonSystem->m_scndEndCap == 1)
				         m_scndRec = armedObj->m_thirdPersonSystem->m_scndEndCap;

				   if(!ptrLoc->m_firstPersonMode)
						SkeletalFiringSystem(	ptrLoc,
												armedObj,
												missileRuntime);
				   
				}//end sound only
	            else{//normal  
				   BOOL disableSound = FALSE;
	               if(ptrLoc->m_firstPersonMode)
					  disableSound = TRUE;
				   D3DMATRIX aBoneMatrix = ptrLoc->List_skeletalObject->GetBoneMatrixByIndex(armedObj->m_boneAttached);
				   m_matLib.fastMatrixMultiply(aBoneMatrix,aBoneMatrix,positionalMatrix);
				   armedObj->m_thirdPersonSystem->AnimationCallback(tCount,
					                                                m_soundCenterMatrix,
					                                                worldSpacePosition,
																	GL_globalSoundFactor,
																	FALSE,
																	m_soundManager,
																	disableSound,
																	m_multiplayerObject->m_AIServer,
																	m_3dDevice,
																	ptrLoc->List_movementDB->List_curVelocityVect,
																	ptrLoc->List_controlType,
																	m_lodOverride);
				   if(ptrLoc->List_controlType == 0)
				   if(armedObj->m_abilityTypeUse == 12)
				      if(armedObj->m_thirdPersonSystem->m_scndEndCap == 1)
				         m_scndRec = armedObj->m_thirdPersonSystem->m_scndEndCap;

				   if(!ptrLoc->m_firstPersonMode)
						SkeletalFiringSystem(	ptrLoc,
												armedObj,
												missileRuntime);

				}//end normal
			 }
			 if(armedObj->m_firstPersonSystem){
                if(soundOnly){//sound only
				   BOOL disableSound = FALSE;
	               if(!ptrLoc->m_firstPersonMode)
					  disableSound = TRUE;
				   if(m_multiplayerObject->m_AIServer == TRUE)
			          disableSound = TRUE;
				   armedObj->m_firstPersonSystem->AnimationCallback(tCount,
					                                                m_soundCenterMatrix,
																	worldSpacePosition,
																	GL_globalSoundFactor,
																	TRUE,
																	m_soundManager,
																	disableSound,
																	m_multiplayerObject->m_AIServer,
																	m_3dDevice,
																	ptrLoc->List_movementDB->List_curVelocityVect,
																	ptrLoc->List_controlType,
																	m_lodOverride
																	);

				   if(ptrLoc->List_controlType == 0)
				   if(armedObj->m_abilityTypeUse == 12)
				      if(armedObj->m_firstPersonSystem->m_scndEndCap == 1)
				         m_scndRec = armedObj->m_firstPersonSystem->m_scndEndCap;

				   
				   if(ptrLoc->m_firstPersonMode)
				   SkeletalFiringSystem(ptrLoc,
					                    armedObj,
										missileRuntime);
				}//end sound only
	            else{//normal 
				   BOOL disableSound = FALSE;
	               if(!ptrLoc->m_firstPersonMode)
					  disableSound = TRUE;
				   armedObj->m_firstPersonSystem->AnimationCallback(tCount,
					                                                m_soundCenterMatrix,
					                                                worldSpacePosition,
																	GL_globalSoundFactor,
																	FALSE,
																	m_soundManager,
																	disableSound,
																	m_multiplayerObject->m_AIServer,
																	m_3dDevice,
																	ptrLoc->List_movementDB->List_curVelocityVect,
																	ptrLoc->List_controlType,
																	m_lodOverride);
 
				   if(ptrLoc->List_controlType == 0)
				   if(armedObj->m_abilityTypeUse == 12)
				      if(armedObj->m_firstPersonSystem->m_scndEndCap == 1)
				         m_scndRec = armedObj->m_firstPersonSystem->m_scndEndCap;

				   if(ptrLoc->m_firstPersonMode)
				   SkeletalFiringSystem(ptrLoc,
					                    armedObj,
										missileRuntime);
				}//end normal
			 }
		  }//end armed appendages
	}//end valid 1
return TRUE;
}//END
//--------------animation properies-determines run/jump-------------------------------//
void GlobalStatusVariables::EntityAnimationManagmentCallback(CMovementObj * entLoc)
{
   //entLoc->List_skeletalObject->CharacterAnimationManagement(entLoc->List_movementDB);
   entLoc->CharacterAnimationManagement(entLoc->List_skeletalObject,m_timeAdjustFloat);
   if(entLoc->List_armedInventoryDB)
	  for(POSITION armedPosition = entLoc->List_armedInventoryDB->GetHeadPosition();armedPosition != NULL;)
	  {
          CArmedInventoryObj * armPtr = (CArmedInventoryObj *)entLoc->List_armedInventoryDB->GetNext(armedPosition);
          if(armPtr->m_firstPersonSystem)
		  {
			  if(entLoc->m_firstPersonAnimOverride > -1)
			  {  
				  armPtr->m_firstPersonSystem->m_animPreference = entLoc->m_firstPersonAnimOverride;
				  entLoc->List_currentlyFireing = TRUE;
				  entLoc->CharacterAnimationManagement(armPtr->m_firstPersonSystem,m_timeAdjustFloat);
				  entLoc->m_firstPersonAnimOverride = -1;
				  entLoc->m_attackAnimOverride = -1;
			  }
			  else
			  {
				  armPtr->m_firstPersonSystem->m_animPreference = 0;
				  entLoc->CharacterAnimationManagement(armPtr->m_firstPersonSystem,m_timeAdjustFloat);
			  }
			 //armPtr->m_firstPersonSystem->CharacterAnimationManagement(entLoc->List_movementDB);
		  }
		  if(armPtr->m_thirdPersonSystem)
			 entLoc->CharacterAnimationManagement(armPtr->m_thirdPersonSystem,m_timeAdjustFloat);
			 //armPtr->m_thirdPersonSystem->CharacterAnimationManagement(entLoc->List_movementDB);
	  }
}
//------------------------------------------------------------------------------------//
/*void GlobalStatusVariables::SkeletalFiringSystem(   D3DMATRIX       & positionalMatrix,
													CSkeletonObject * skeletonPtr,
													int             & entityTraceNumber,
													float             pitchRotation,
													CMovementObj    * entityPtr,
													float             yawRotation,
													CMissileObjList  * missileRuntime)
{
 if(entityPtr->List_networkEntity)
	return;

 if(!skeletonPtr->m_fireQueList)
  return;
  int trace = 0;
  for(POSITION firePos = skeletonPtr->m_fireQueList->GetHeadPosition();firePos != NULL;)
  {
    CFireRangeObject * fireRange = (CFireRangeObject *) skeletonPtr->m_fireQueList->GetNext(firePos);
	
    if(skeletonPtr->m_secondaryAnimationInUse != fireRange->m_animationIndex)
	   continue;

       POSITION bonePos = skeletonPtr->m_bonesList->FindIndex(0);
	   if(!bonePos)
		   continue;
	       CBoneObject * bonePtr = (CBoneObject *)skeletonPtr->m_bonesList->GetAt(bonePos);

		   POSITION animPos = bonePtr->m_animationList->FindIndex(skeletonPtr->m_secondaryAnimationInUse);
           if(!animPos)
			   continue;
		       CBoneAnimationObject * animPtr = (CBoneAnimationObject *)bonePtr->m_animationList->GetAt(animPos);

			   if(fireRange->m_animationFrameRangeStart <= skeletonPtr->m_secondaryBaseFrame &&
                  fireRange->m_animationFrameRangeEnd   >= skeletonPtr->m_secondaryBaseFrame)
			   {//within range
				 if(!fireRange->m_inRangeCurrently)
				 {

                  D3DVECTOR * vectorOverride = NULL;
                  if(entityPtr->m_aiObj){
					 vectorOverride = &entityPtr->List_AIdestination;
				  }
                 ExecuteFireQue(entityPtr,
								fireRange,
								pitchRotation,
								positionalMatrix,
								entityTraceNumber,
								vectorOverride,
								trace,
								TRUE,
								entityPtr->List_movementDB->List_freeLookCurYaw,
								fireRange->m_missileIndex,
								missileRuntime);

				  fireRange->m_inRangeCurrently = TRUE;
				 }
			   }//end within range
			   else{//reset in range variable
                  fireRange->m_inRangeCurrently = FALSE;
			   }//end reset in range variable
  trace++;
  }
}*/
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::SkeletalFiringSystem(   CMovementObj		* entityPtr,
												    CArmedInventoryObj	* armedPtr,
													CMissileObjList		* missileRuntime)
{
 if(entityPtr->List_networkEntity)
	return FALSE;

 int trace = 0;
 CFireRangeObject * range = entityPtr->GetNextActiveFireQue(trace,armedPtr,entityPtr->m_firstPersonMode);
 if(!range)
	return FALSE;
 
 D3DMATRIX projectileMatrix;

 if(entityPtr->m_firstPersonMode && entityPtr->List_controlType == 0)
 {
    projectileMatrix = m_cameraModule->GetCameraMatrix();
	
	D3DMATRIX rMat;
	m_matLib.Rotate ( 0, 180, 0, rMat.m);
	m_matLib.fastMatrixMultiply(projectileMatrix,rMat,projectileMatrix);
 }
 else if(!entityPtr->GetFireTransform(range->m_boneIndex,armedPtr, projectileMatrix))//always uses 3rd p cam
 {
    AddLine_safe("WEAPON ERROR-cannot find fire que bone index GetFireTransform(BONE_FIRE)");
	return FALSE;
 }
 else
 {//found fire bone matrix-check camera to see if its a targeting thrid camera
	 if(m_cameraModule->m_followMode == 2 && entityPtr->List_controlType == 0)
	 {//third person targeting camera
	    float rangeOnGun = 1000.0f;
		byte temp = 0;
		D3DVECTOR startPoint, endPoint, intersect, latestNormal;
		int objectIndex = 0;
        D3DMATRIX camProjMat = m_cameraModule->GetCameraMatrix();
		ZeroMemory(&startPoint,sizeof(D3DVECTOR));
		ZeroMemory(&endPoint,sizeof(D3DVECTOR));
		endPoint.z = rangeOnGun;
		startPoint = m_matLib.UpdatePolygonVertex(camProjMat.m,startPoint);
		endPoint = m_matLib.UpdatePolygonVertex(camProjMat.m,endPoint);
		float tempDist = 0.0f;
		intersect = endPoint;

		entityPtr->m_collisionMovRefPtr->GetClosesIntersectionUseSeg(
																		startPoint,
																		endPoint,
																		startPoint,
																		endPoint,
																		&intersect.x,
																		&intersect.y,
																		&intersect.z,
																		&objectIndex,
																		&latestNormal,
																		true,
																		tempDist,
																		temp
																		);

        SegmentCollisionToAllEntities(TRUE,
								      startPoint,
								      endPoint,
								      intersect,
								      .3f,
								      tempDist);

		m_matLib.FaceTowardPoint(intersect,&projectileMatrix);
	 }//end third person targeting camera
 }//end found fire bone matrix-check camera to see if its a targeting thrid camera

 if(entityPtr->List_controlType == 5)//ai type
 if(entityPtr->m_aiObj)
 {
	m_matLib.FaceTowardPoint(entityPtr->List_AIdestination,&projectileMatrix);
 }

 int resolvedFireQue = range->m_missileIndex;
 if(armedPtr->m_fireQueOvrd > 0)
	resolvedFireQue = armedPtr->m_fireQueOvrd;

 ExecuteFireQue( entityPtr,
				 projectileMatrix,
				 range,
				 trace,
				 TRUE,
				 resolvedFireQue,
			     missileRuntime);
 
 return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::DeMount(CMovementObj * hostPtr,CMovementObjList * entObjRuntime)
{
    if(!hostPtr->m_posses)
		return FALSE;

	D3DVECTOR newPosition;
    int traceStore = hostPtr->m_posses->List_traceNumber;
	hostPtr->List_baseFrame->GetPosition(NULL,&newPosition);

    D3DVECTOR lastTangentVect = hostPtr->m_posses->m_collisionRaySys->m_lastCenterMassPosition;

	if(hostPtr->m_posses){
       //delete trace store
	   POSITION posLast = NULL;
	   for(POSITION ePos = entObjRuntime->GetHeadPosition();(posLast = ePos) != NULL;)
	   {
	      CMovementObj * entPtr = ( CMovementObj* ) entObjRuntime->GetNext(ePos);
		  if(entPtr->List_traceNumber == traceStore)
		  {
		     entObjRuntime->RemoveAt(posLast);
		     DestroyRuntimeEntity(entPtr,m_spawnGeneratorDB,entObjRuntime);
             entPtr=0;
             break;
		  }
	   }
	}
	
	D3DMATRIX newMatrix;
    m_matLib.InitMat(newMatrix.m);
	newMatrix._41 = newPosition.x;
	newMatrix._42 = newPosition.y;
	newMatrix._43 = newPosition.z;
	hostPtr->List_movementDB->List_lastPosition.x = newPosition.x;
	hostPtr->List_movementDB->List_lastPosition.y = newPosition.y;
	hostPtr->List_movementDB->List_lastPosition.z = newPosition.z;
	hostPtr->List_baseFrame->AddTransformReplace(newMatrix);
	hostPtr->m_networkFirstTimeSendingPos = TRUE;

	if(hostPtr->m_collisionRaySys)
    {
		  hostPtr->m_collisionRaySys->m_lastCenterMassPosition = lastTangentVect;

		   //delete hostPtr->m_collisionRaySys;
		   //hostPtr->m_collisionRaySys = 0;
    }

	return TRUE;
}
//------loads scene-------------------------------------------------------------------//
void GlobalStatusVariables::AXPumpMsgs()
{
     while( ::PeekMessage( &m_msg, NULL, 0, 0, PM_NOREMOVE ) ) 
             AfxPumpMessage();
}
//------loads scene-------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadScene(CString fileName)
{	
	//Figure out which map we're on for the minimap
	for(int i = 0; i < MINIMAP_MAX_MAPS; i++)
		if(m_minimapFilenames[i] == fileName)
			m_minimapCurMapIndex = i;

	m_axLogger.WriteDebugFile("Load Scene:",0,0);
	m_axLogger.WriteDebugFile(fileName.GetBuffer(),0,1);
	this->m_dxRenderer->EnableMouse();
    //m_soundTrackList->Stop();
	if(m_curWldObjPrtList)
	for(POSITION prtPos = m_curWldObjPrtList->GetHeadPosition();prtPos != NULL;)
	{//set world particle sys to non regen to flag it
     CWldObject * wldObj = (CWldObject *)m_curWldObjPrtList->GetNext(prtPos);
     if(wldObj->m_particleEmmiter)
	    wldObj->m_particleEmmiter->m_Regenerate = FALSE;
	}//end set world particle sys to non regen to flag it
	
	CacheGeneratedRenderStuff(0);
	m_runtimeMenuDatabase->CloseAll(&m_showMenuCursor,false);
	m_curWldObjPrtList->RemoveAll();

	LoadSceneInfo();
	 
    //end set loading screen
		if(m_autoDownloadMissingAssets==TRUE)
	    {//check file and if it doesnt validate download new one
           SetCurrentDirectory(m_installPath);
		   CString contentUrl = m_contentUrl + "/contentmanager.aspx?contenttype="+fileName;
		   CString ipTcpServer = "";
		   CString tcpIPUrl   = m_contentUrl + "/contentmanager.aspx?tcpserver=1";
		   CString tcpPortUrl = m_contentUrl + "/contentmanager.aspx?tcpserver=2";
		   long tcpPort = 0;
		   CString temp = "";
		   if(ReadUrl(tcpIPUrl, ipTcpServer)==false)
		   {//error
	       
		   }//end error
		   if(ReadUrl(tcpPortUrl, temp)==false)
		   {//error
	       
		   }//end error
		   tcpPort = atoi(temp);

		   DWORD returnLength = 0;
		   char * buffer = NULL;
		   //CHttpFileDownloader * urlReader = new CHttpFileDownloader();
		   CHttpFileDownloader urlReader;
		   urlReader.m_asciMode = TRUE;
		   if(urlReader.GetUrlData("Gworlds",contentUrl.GetBuffer(),(BYTE**)&buffer,&returnLength))
		   {//succeeded
		      CString startupFiles = buffer;
		      if(buffer)
			     delete [] buffer;
		      buffer = 0;
	       

			  CString	fileNameTemp = ""; 
			  long		fileSizeTemp = 0;
			  CString	cabNameTemp = "";
			  CString	offsetDirTemp = ""; 
			  unsigned long CRCTemp = 0;
			  unsigned long CAB_CRCTemp = 0;
			  int       currentSet = 0;
			  BOOL       checkCrc = TRUE;
			  while(ParseUrlStringToData(   currentSet, 
											startupFiles,//CString urlParse, 
											fileNameTemp, 
											&fileSizeTemp, 
											cabNameTemp, 
											offsetDirTemp, 
											&CRCTemp,
											&CAB_CRCTemp,
											&checkCrc))
			  {//validate or download
				    int percentagePub = 0;
				    CString fullFilePath = m_installPath + offsetDirTemp + fileNameTemp;
					UpdateMeterAdv(75,"Verifying:"+cabNameTemp);
				    if(ValidateFile(fullFilePath, CRCTemp, checkCrc)==false && m_devMode == 0)
				    {//file needs to be redownloaded
				        
						if(m_contentDownloadManager->DownloadFileRequest(cabNameTemp,ipTcpServer,tcpPort)==1)
					    {//success to start thread for downloading
							while(m_contentDownloadManager->GetPercentageOfDownload() < 100)
							{
									Sleep(50);
									percentagePub = m_contentDownloadManager->GetPercentageOfDownload();
									if(m_contentDownloadManager->m_errorMessage.GetLength()<2)
									{
									   CString kb_sec = m_cabLib.ConvToString(m_contentDownloadManager->GetKBSec()*8);
									   /*UpdateMeterAdv(percentagePub,"kb: "+kb_sec+" Downloading:"+cabNameTemp);*/
									   UpdateMeterAdv((float)percentagePub,"kb: "+kb_sec+" Downloading:"+cabNameTemp);
									   //WARNING: C4244 --> int to float
									}
									else
									{
									   /*UpdateMeterAdv(percentagePub,m_contentDownloadManager->m_errorMessage+cabNameTemp);*/
										UpdateMeterAdv((float)percentagePub,m_contentDownloadManager->m_errorMessage+cabNameTemp);
									   //WARNING: C4244 --> int to float
									}
							
									this->m_dxRenderer->EnableMouse();
									Sleep(50);
									if(this->GetCloseDownCallState()==TRUE)
									{
                                       m_contentDownloadManager->Abort();
									   m_contentDownloadManager->WaitForThreadToClose();

									   return FALSE;
									}
							}
							percentagePub = m_contentDownloadManager->GetPercentageOfDownload();
							UpdateMeterAdv((float)percentagePub,"Download Complete");
							LoadSceneInfo();
							m_contentDownloadManager->Reset();
							CString saveLocationWFile = m_cacheDirectory + cabNameTemp;
							BOOL resultFromDelete = 0;//DeleteFile(classObj->m_saveLocationWFile);//delete temp file
							//validate cabinet
							int tryCount = 0;
							UpdateMeterAdv(92,"Verifying Cabinet");
							if(ValidateFile(saveLocationWFile, CAB_CRCTemp, TRUE)==false && m_devMode == 0)
							{//Cabinet failed
								UpdateMeterAdv(92,"Extracting Vefify Failed-Redownloading");
								resultFromDelete = DeleteFile(saveLocationWFile);//delete temp file
								while(!resultFromDelete)
								{//delete failedfullFilePath
								m_axLogger.WriteDebugFile(saveLocationWFile.GetBuffer(),0,0);
								m_axLogger.WriteDebugFile("Delete Failed!!",0,1);
								resultFromDelete = DeleteFile(saveLocationWFile);//delete temp file
								Sleep(50);
								tryCount++;
								if(tryCount > 10)
									break;
								}//end delete failed
								currentSet--;
							   continue;
							}//end cabinet failed
							//extract cabinet to target directory
							
							UpdateMeterAdv(92,"Extracting Cabinet");
							//CCabinetObj newCabObj;
							//newCabObj.ExtractCabinetToDirectory(m_installPath,saveLocationWFile,&percentagePub,&m_globalMeterSection);

                            CResourceCabinet newCabObj;
							newCabObj.Init();
							newCabObj.CreateFDIContext();
							newCabObj.SetProgressCountRef(&percentagePub);
							newCabObj.SetCriticalSection(&m_globalMeterSection);
							newCabObj.Copy(saveLocationWFile.GetBuffer(),m_installPath,NULL);

                            Sleep(500);//let it digest
                            //handle bad download
							
							UpdateMeterAdv(97,"Verifying Download");
							if(ValidateFile(fullFilePath, CRCTemp, checkCrc)==false && m_devMode == 0)
							{//file extracted badly
								currentSet--;
								UpdateMeterAdv(100.0f,"File Corruption Error:"+fullFilePath);
								resultFromDelete = DeleteFile(fullFilePath);
								if(!resultFromDelete)
								{
									m_axLogger.WriteDebugFile(fullFilePath.GetBuffer(),0,0);
									m_axLogger.WriteDebugFile("Delete Failed!!",0,1);
								}
							}//end file extracted badly
							//clean up cabinet
							tryCount = 0;
							resultFromDelete = DeleteFile(saveLocationWFile);
							while(!resultFromDelete)
							{//delete failedfullFilePath
								m_axLogger.WriteDebugFile(saveLocationWFile.GetBuffer(),0,0);
								m_axLogger.WriteDebugFile("Delete Failed!!",0,1);
								resultFromDelete = DeleteFile(saveLocationWFile);//delete temp file
								Sleep(50);
								tryCount++;
								if(tryCount > 10)
									break;
						    }//end delete failed
							//handle bad download
					}//end success to start thread for downloading
				}//end file needs to be redownloaded
				currentSet++;
			}//end validate or download
		   }//end succedded
	    }//end check file and if it doesnt validate download new one

     Sleep(100);
	 UpdateMeterAdv(0,"SC:"+fileName);
     CFileException exc;
     CFile fl;
     BOOL res = fl.Open(GL_pathGameFiles+fileName,CFile::modeRead | CFile::shareDenyWrite, &exc);
     if(res == FALSE)
	 {
	    m_axLogger.WriteDebugFile("Load Scene failed:",0,0);
	    m_axLogger.WriteDebugFile(fileName.GetBuffer(),0,1);
	    UpdateMeterAdv(100,"Load Scene Failed:"+fileName);
		LoadSceneInfo();
		return FALSE;//give up
	 }
     UpdateMeterAdv(7,"SC:"+fileName);
	 
	 //
	 //destroy list that will be filled
	 DeleteSpawnDatabase();
	 DestroyWorldObjects();
	 DestroyEnvironment();
	  if(GL_ScenePersistList){//delete persist list
	    for(POSITION posLocal = GL_ScenePersistList->GetHeadPosition();posLocal != NULL;){
		    CPersistObj * perstPtr = (CPersistObj*)GL_ScenePersistList->GetNext(posLocal);
            delete perstPtr;
			perstPtr=0;
		}
	   delete GL_ScenePersistList;
	   GL_ScenePersistList=0;
	  }//end delete persist list
	 DestroyEntityRuntimeList();
	 DeleteWorldGroupList();
	 DeleteScriptList(&m_sceneScriptExec);
	 DeleteSpawnDatabase();
     
     m_worldRenderList->RemoveAll();
	 m_worldTransferList->RemoveAll();
	 if(m_collisionBase)
	 {
	  m_collisionBase->SafeDelete();
      delete m_collisionBase;
	  m_collisionBase=0;
	 }

	 UpdateMeterAdv(10,"SC:"+fileName);

	 if(m_libraryReferenceDB)
	 {
        m_libraryReferenceDB->SafeDelete();
		delete m_libraryReferenceDB;
		m_libraryReferenceDB=0;
	 }
	 if(GL_aiWebDatabase){
        GL_aiWebDatabase->SafeDelete();
		delete GL_aiWebDatabase;
	    GL_aiWebDatabase=0;
	 }

	 if(m_backGroundList)
	 {
	    m_backGroundList->SafeDelete();
        delete m_backGroundList;
        m_backGroundList = 0;
	 }

     if(m_waterZoneDB)
	 {
        m_waterZoneDB->SafeDelete();
        delete m_waterZoneDB;
		m_waterZoneDB = 0;
	 }
	 CString reserved;
	 int reservedInt = 0;
	 
	 CArchive the_in_Archive(&fl,CArchive::load);
					the_in_Archive >> m_wldObjList;
					UpdateMeterAdv(15,"SC: World Object List");
					the_in_Archive >> m_backGroundList;
					UpdateMeterAdv(20,"SC: Background List");
					the_in_Archive >> m_environmentPtr;
					UpdateMeterAdv(25,"SC: Env");
					the_in_Archive >> GL_ScenePersistList;
					UpdateMeterAdv(28,"SC: Persist");
					the_in_Archive >> m_currentTrack;
					UpdateMeterAdv(30,"SC: CurTrack");
					the_in_Archive >> m_musicLoopFile;
					UpdateMeterAdv(35,"SC: Mloop");
					the_in_Archive >> m_globalWorldShader;
					UpdateMeterAdv(40,"SC: GWShaders");
				    the_in_Archive >> W_BufferPreffered;
					UpdateMeterAdv(45,"SC: WBuf");
					the_in_Archive >> m_sceneScriptExec;
					UpdateMeterAdv(55,"SC: WBuf");
					the_in_Archive >> reserved;
					UpdateMeterAdv(61,"SC: Reserved");
					the_in_Archive >> m_spawnGeneratorDB;
					UpdateMeterAdv(70,"SC: Spawn Generator");
					the_in_Archive >> m_collisionBase;
					UpdateMeterAdv(80,"SC: Collision Base");
                    the_in_Archive >> m_worldGroupList;
					UpdateMeterAdv(83,"SC: World Groups");
					the_in_Archive >> GL_aiWebDatabase;
					UpdateMeterAdv(85,"SC: AI WebDatabase");
					the_in_Archive >> m_libraryReferenceDB;
					UpdateMeterAdv(95,"SC: Reference Library");
	                the_in_Archive >> m_waterZoneDB;
					UpdateMeterAdv(96,"SCT: WaterZone");

	 //SetCurrentDirectory(MainDir); 
     //UpdateMeterAdv(97,"SC:Reinit Refs");
	 //m_libraryReferenceDB->ReinitAll(m_3dDevice);
	 
	 UpdateMeterAdv(96,"SC: Ai Output");
	 m_spawnGeneratorDB->GetTotalAIOutput();//m_spawnGeneratorDB related
	
	 UpdateMeterAdv(96,"SC: complete...");

	 the_in_Archive.Close();
     fl.Close();

	 //if(m_dxRenderer->m_shaderSupport)
	 //   m_renderStateObject->SetShader(m_3dDevice,m_shaderSystem,m_globalWorldShader);

	 if(!m_multiplayerObject->m_AIServer)
	     ExecAllAttributes(m_sceneScriptExec);
	
	 
	 //InitializeBackground(0);
	 SpawnGenDatabaseInList(0);
	 InitializeOjectsIntoScene();
     if(m_editorModeOn == FALSE)//make room for extra ram
	 {
		//AfxMessageBox("mem save");
	    m_wldObjList->CacheUnimediateData();
	 }

	 RebuildTraceNumber();//rebuild trace numbers
	 
	 if(m_multiplayerObject->GetEnabledClient_safe() == FALSE && GL_ScenePersistList != NULL){//dont spawn stuff unless server
	    UpdateEntityPersist(&GL_ScenePersistList);//fill in runtime entities
	 }
	 
	 //reinit states
	 SetAllRenderStatesIM();
	 GlobalTraceNumber++;//needs this
     m_collisionBase->BuildObjectReferenceArray(m_wldObjList);
	 InitializeEnvironment(TRUE);
	 
	 m_axLogger.WriteDebugFile("Load Scene Sucecss:",0,0);
	 m_axLogger.WriteDebugFile(fileName.GetBuffer(),0,1);

	 if(!m_editorModeOn)
	     this->m_dxRenderer->DisableMouse();
	 //AfxMessageBox("Scene Loaded");
	 
  return TRUE;
}

//-----------------------------------------------------------//
bool GlobalStatusVariables::ValidateFile(CString & fullPath, unsigned long fileCRC, BOOL verifyFileSizeRequired)
{
     m_axLogger.WriteDebugFile("Validationg File CRC:",0,0);
	 m_axLogger.WriteDebugFile(fileCRC,0,0);
	 m_axLogger.WriteDebugFile(fullPath.GetBuffer(),0,1);

	 
	 /*if(m_crcGenerator.getcrc(fullPath)==fileCRC)
	    return true;

	 return false;*/
	 
     CFileException exc;
     CFile fl;
	 BOOL res = fl.Open(fullPath,CFile::modeRead, &exc);
	 if(res == FALSE)
	 {
        if(exc.m_cause == CFileException::fileNotFound)
		{
		 
	       return false;
		}
		Sleep(50);
		res = fl.Open(fullPath,CFile::modeRead, &exc);
		if(!res)
		   return false;
	 }
     //validate file size
     if(!verifyFileSizeRequired)
        return true;

	 if(m_crcGenerator.getcrc(&fl) == fileCRC)
	 {//all good
        fl.Close();
		return true;
	 }//end all good
	 fl.Close();
     return false;
}
//-----------------------------------------------------------//
bool GlobalStatusVariables::ParseUrlGetLine(int & curPos, CString & urlData, CString & lineString)
{
	 int length = urlData.GetLength();
	 lineString = "";
     while(curPos < length)
	 {
		 if(urlData.GetAt(curPos) == '\r')
		 {
		    curPos++;
			curPos++;
		    return true;
		 }
         lineString += urlData.GetAt(curPos);
		 curPos++;
	 } 
	 return false;
}
//-----------------------------------------------------------//
bool GlobalStatusVariables::ParseUrlStringToData(int set, 
											  CString urlParse, 
											  CString & fileName, 
											  long * fileSize, 
											  CString & cabName, 
											  CString & offsetDir, 
											  unsigned long * CRC,
											  unsigned long * cab_CRC,
											  BOOL * checkCrc)
{
    int lineSkip = 7*set;
	int currentPos = 0;
	int currentLine = 0;
	CString currentLineData = "";
	int lengthTotal = urlParse.GetLength();
	int varIndex = 0;

    while(ParseUrlGetLine(currentPos, urlParse, currentLineData))
	{
        if(currentLine < lineSkip)
		{
		   currentLine++;
		   continue;
		}

		if(varIndex == 0)
           fileName	 = currentLineData;
		else if(varIndex == 1)
           *fileSize  = (long)atol(currentLineData.GetBuffer());
		else if(varIndex == 2)
           cabName	 = currentLineData;
		else if(varIndex == 3)
		   offsetDir = currentLineData;
		else if(varIndex == 4)
		{
		   *CRC		 = (unsigned long)_atoi64(currentLineData.GetBuffer());
		}
		else if(varIndex == 5)
		{
		   *checkCrc		 = (BOOL)atoi(currentLineData.GetBuffer());
		}
		else if(varIndex == 6)
		{
		   *cab_CRC	 = (unsigned long)_atoi64(currentLineData.GetBuffer());//cab crc
		   return true;
		}
		currentLine++;
		varIndex++;
	}
	return false;
}
//-----------------------------------------------------------//
bool GlobalStatusVariables::ReadUrl(CString & url, CString & retString)
{
     DWORD returnLength = 0;
	 char * buffer = NULL;
     CHttpFileDownloader * urlReaderTCPPort = new CHttpFileDownloader();
	 urlReaderTCPPort->m_asciMode = TRUE;
	 if(urlReaderTCPPort->GetUrlData("Gworlds",url.GetBuffer(),(BYTE**)&buffer,&returnLength))
	 {
        retString = buffer;
        if(buffer)
           delete [] buffer;
		buffer = 0;
		delete urlReaderTCPPort;
	    urlReaderTCPPort = 0;
        return true;
	 }
	 delete urlReaderTCPPort;
	 urlReaderTCPPort = 0;
	 return false;
}
//----------------------------------------------------------//
void GlobalStatusVariables::CacheGeneratedRenderStuff(int mirrorBased)
{
   m_soundTrackList->m_acceptChangeFlag = TRUE;
   m_runtimeLightSys->FlushSys(m_3dDevice,m_renderStateObject);
   m_renderContainerList->SafeDelete();
   m_renderContainerAlphaFilterList->SafeDelete();
   m_visualsRenderList->RemoveAll();
   m_worldTriggerList->RemoveAll();
   m_worldMirrorList->RemoveAll();
   m_worldTransferList->RemoveAll();
   m_translucentPolyList->SafeDelete();
   m_translucentMirrorPolyList->SafeDelete();
   m_shadowObjectsList->SafeDelete();
   m_shadowEntityList->RemoveAll();
   m_charonaCache->ClearAll();
   m_collisionPolysInUse=0;//seed collison poly count
   m_actualPolysCalculated=0;//seed actual poly calc
   m_renderStateObject->m_stateChangesPerPass=0;
   m_skeletalReferenceObjCache.RemoveAll();//Clears ref skeletal object in vicinity with realtime collision list
   if(!mirrorBased)
       m_toRenderSurfaceDB->Flush();

   for(POSITION posRenLoc = m_worldEnviormentList->GetHeadPosition();posRenLoc != NULL;)
   {
	 CWldObject * wldRenObj = (CWldObject *)m_worldEnviormentList->GetNext(posRenLoc);
	 wldRenObj->List_poppedOut = TRUE;
   }
   m_worldEnviormentList->RemoveAll();  

  
}
//------set projection matrixs--------------------------------------------------------//
void GlobalStatusVariables::SetMenuViewState()
{
        CViewportObj * vptPtr = new CViewportObj(0,0,100,100);
		vptPtr->m_cameraObject = NULL;
		vptPtr->m_traceNumberef = -1;
		//ActivateVieport(vptPtr);
        D3DMATRIX projM = vptPtr->UtilizeViewPort(m_dxRenderer->m_currentWidth,m_dxRenderer->m_currentHeight,m_3dDevice);
        m_3dDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,m_backgroundColorBlack, 1.0f, 0 );
	    //--------------
		//set projection
	    //--------------
	    D3DMATRIX matProj,origMatrix,matView;
        m_matLib.InitMat(origMatrix.m);
        //initial offset
        D3DXVECTOR3 upVect,dirVect;
        upVect.x =  0.0f;
        upVect.y =  1.0f;
        upVect.z =  0.0f;
        dirVect.x = 0.0f;
        dirVect.y = 0.0f;
        dirVect.z = 1.0f;
        D3DXVECTOR3 init;init.x = 0.0f;init.y = 0.0f;init.z = 0.0f;
        SetViewMatrixAB(matView,init,dirVect,upVect);

		m_3dDevice->GetTransform(D3DTS_PROJECTION ,&origMatrix);
        float fAspect = ((FLOAT)vptPtr->List_imViewPort.Height) / vptPtr->List_imViewPort.Width;
        D3DUtil_SetProjectionMatrix( matProj,
				                    .3f, 
						             fAspect, 
						             1.0f,
							         3000.0f,
				                     1.0f,
									 m_tgSys_clipWidth,
									 m_tgSys_clipHeight);

        m_renderStateObject->SetViewMatrix((D3DXMATRIX*)&matView,m_3dDevice);
		m_renderStateObject->SetViewProjMatrixOnly((D3DXMATRIX*)&matProj,m_3dDevice);
		//------------------
		//end set projection
		//------------------
	    m_renderStateObject->SetCullState(FALSE,m_3dDevice);
		if(vptPtr)
		{
			vptPtr->SafeDelete();
			delete vptPtr;
			vptPtr = 0;
		}
}
//------loads scene-------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadSceneInfo()
{ 
    if(!m_3dDevice)
		return FALSE;

	// No content server (this client): gold-style loading screen from local files -
	// the intro art with the "Endless Ages" title flashing (two frames, swapped every 0.5s).
	CTextureEX * loadTex = NULL;
	if(m_contentUrl.GetLength() == 0 && m_editorModeOn)
		return FALSE;   // editor: no loading screen, as before
	if(m_contentUrl.GetLength() == 0)
	{
		static CTextureEX * s_goldFrames[2] = {NULL, NULL};
		static BOOL         s_goldTried = FALSE;
		if(!s_goldTried)
		{
			s_goldTried = TRUE;
			char exePath[MAX_PATH] = {0};
			GetModuleFileName(NULL, exePath, MAX_PATH);
			CString dir(exePath);
			dir = dir.Left(dir.ReverseFind('\\') + 1);
			const char * names[2] = {"MapsModels\\EA_Gold_Loading_Title.png", "MapsModels\\EA_Gold_Loading.png"};
			for(int f = 0; f < 2; f++)
			{
				s_goldFrames[f] = new CTextureEX();
				if(!s_goldFrames[f]->LoadIt(dir + names[f], 1, m_3dDevice, 1) || !s_goldFrames[f]->m_pddsSurface)
				{
					s_goldFrames[f]->SafeDelete();
					delete s_goldFrames[f];
					s_goldFrames[f] = NULL;
				}
			}
		}
		loadTex = s_goldFrames[(GetTickCount() / 500) & 1];
		if(!loadTex) loadTex = s_goldFrames[0] ? s_goldFrames[0] : s_goldFrames[1];
	}

	           if(GetTickCount()-m_loadingTextureUpdateTimeST > 17000 && m_contentUrl.GetLength() > 0)
			   {//reload image
                  if(m_loadingTexture)
				  {
                     m_loadingTexture->SafeDelete();
					 delete m_loadingTexture;
					 m_loadingTexture = 0;
				  }
				  m_loadingTexture = new CTextureEX();
                  if(!m_loadingTexture->LoadItFromURL(m_contentUrl + "/contentmanager.aspx?adv=loadingimg",1,m_3dDevice,TRUE,false,this->m_advUrlBasic,this->m_advUrlVending))
				  {
                      m_loadingTexture->SafeDelete();
					  delete m_loadingTexture;
					  m_loadingTexture = 0;
				  }
                  m_loadingTextureUpdateTimeST = GetTickCount();
			   }//end reload image

			   if(!loadTex) loadTex = m_loadingTexture;
			   SetMenuViewState();
               
			   if(loadTex)
			   {
                
				D3DMATRIX movMatrix;
				m_matLib.InitMat(movMatrix.m); 
				m_renderStateObject->SetWorldMatrix((D3DXMATRIX*)&movMatrix,m_3dDevice);
				m_renderStateObject->SetBlendLevel(m_renderStateObject->m_intZero,m_renderStateObject->m_intZero,m_3dDevice);//REMOVE
				for(int loop=1;loop<m_renderStateObject->m_multiTextureSupported;loop++)
				{
					m_renderStateObject->SetBlendLevel(loop,m_renderStateObject->m_intNegOne,m_3dDevice);
					if(loop>7)
						break;
				}
				//readout
				int blend = 1;
				m_renderStateObject->SetBlendType(m_3dDevice,blend);
				//build panel
				CEXMeshObj * im_mesh= new CEXMeshObj();
				//im_mesh->List_materialObject->m_baseMaterial.Diffuse;
		
				float range = 0;
				//hardcode billboard im_mesh->LoadGeometryFromPlugin(filename,&range,m_3dDevice);
			
				ABVERTEX1L * vertexArray  = new ABVERTEX1L[4];
				ZeroMemory(vertexArray, sizeof(D3DVERTEX) * 4);

  				WORD *      indecieArray = new WORD[6];
				ZeroMemory(indecieArray, sizeof(WORD) * 6);

				WORD        vertexCount  = 4;
				WORD        indecieCount = 6;

				float billboardSizeX = (loadTex->m_widthBasis * .001) * .59f;
				float billboardSizeY = (loadTex->m_hieghtBasis * .001) * .59f;
	    
				//vertex 1 upper left
				vertexArray[0].x  = billboardSizeX;
				vertexArray[0].y  = billboardSizeY;
				vertexArray[0].z  = 3.0f;
				//vertexArray[0].tx0.tu = 0.0f;
				//vertexArray[0].tx0.tv = 0.0f;
				vertexArray[0].tx0.tu = 1.0f;
				vertexArray[0].tx0.tv = 0.0f;
				vertexArray[0].nx = 0.0f;
				vertexArray[0].ny = 0.0f;
				vertexArray[0].nz = -1.0f;
				//vertex 2 bottom left
				vertexArray[1].x  = billboardSizeX;
				vertexArray[1].y  = -billboardSizeY;
				vertexArray[1].z  = 3.0f;
				//vertexArray[1].tx0.tu = 1.0f;
				//vertexArray[1].tx0.tv = 0.0f;
				vertexArray[1].tx0.tu = 1.0f;
				vertexArray[1].tx0.tv = 1.0f;
				vertexArray[1].nx = 0.0f;
				vertexArray[1].ny = 0.0f;
				vertexArray[1].nz = -1.0f;
				//vertex 3 bottom right
				vertexArray[2].x  = -billboardSizeX;
				vertexArray[2].y  = -billboardSizeY;
				vertexArray[2].z  = 3.0f;
				//vertexArray[2].tx0.tu = 1.0f;
				//vertexArray[2].tx0.tv = 1.0f;
				vertexArray[2].tx0.tu = 0.0f;
				vertexArray[2].tx0.tv = 1.0f;
				vertexArray[2].nx = 0.0f;
				vertexArray[2].ny = 0.0f;
				vertexArray[2].nz = -1.0f;
				//vertex 4 upper right
				vertexArray[3].x  = -billboardSizeX;
				vertexArray[3].y  = billboardSizeY;
				vertexArray[3].z  = 3.0f;
				//vertexArray[3].tx0.tu = 0.0f;
				//vertexArray[3].tx0.tv = 1.0f;
				vertexArray[3].tx0.tu = 0.0f;
				vertexArray[3].tx0.tv = 0.0f;
				vertexArray[3].nx = 0.0f;
				vertexArray[3].ny = 0.0f;
				vertexArray[3].nz = -1.0f;
				//indecies
				//no facing
					indecieArray[0] = 1;
					indecieArray[1] = 0;
					indecieArray[2] = 3;
					indecieArray[3] = 2;
					indecieArray[4] = 1;
					indecieArray[5] = 3;

				im_mesh->List_materialObject = new CMaterialObject();
	       		im_mesh->List_materialObject->m_useMaterial.Emissive.r = 1.0f;
				im_mesh->List_materialObject->m_useMaterial.Emissive.g = 1.0f;
				im_mesh->List_materialObject->m_useMaterial.Emissive.b = 1.0f;
				im_mesh->List_materialObject->m_useMaterial.Emissive.a = 0.0f;

				im_mesh->SetGeometry2("HAPPY.TGA",
								vertexArray,
								vertexCount,
								indecieArray,
								indecieCount);
			
				im_mesh->m_abVertexArray.m_uvCount = 1;
				im_mesh->InitBuffer(m_3dDevice,NULL);
				//m_3dDevice->SetMaterial(&im_mesh->List_materialObject->m_useMaterial);
				m_renderStateObject->SetMaterial(m_3dDevice, &im_mesh->List_materialObject->m_useMaterial);
				   
				   
				   
				   
				//textureObject->Restore(m_3dDevice,FALSE,0,0,m_dxRenderer->m_maxTextureSizeSquared,m_renderStateObject->m_pCaps,m_compressTextures);
				if(loadTex->m_pddsSurface)
				{
					m_3dDevice->SetTexture( 0,loadTex->m_pddsSurface);//set texture
					
					if( m_3dDevice->BeginScene() == 0 )
					{
						m_renderStateObject->SetDiffuseSource(im_mesh->m_abVertexArray.m_advancedFixedMode,m_3dDevice);
						m_renderStateObject->SetVertexType(im_mesh->m_abVertexArray.m_uvCount,m_3dDevice);
						im_mesh->Render(m_3dDevice,m_renderStateObject->m_currentVertexType,im_mesh->m_chromeUvSet);
						m_3dDevice->SetTexture( 0,0);//set texture
					    m_renderStateObject->SetTextureState(m_renderStateObject->m_intNegOne,
															m_renderStateObject->m_intZero,
															m_3dDevice,
															m_textureDataBase);
						//RenderMeter();
						m_3dDevice->EndScene();
					}

					
				}
				im_mesh->SafeDelete();
				delete im_mesh;
				im_mesh = 0;

				
			   }

			   if( m_3dDevice->BeginScene() == 0 )
				{
					
				    RenderMeter();
					m_3dDevice->EndScene();
				}

				m_dxRenderer->UpdateDisplaySurfaces();

return TRUE;
}
//------------------------------------------------------------------------------------//
void GlobalStatusVariables::SetupGlobalMeter(float length, 
											 float width, 
											 float xPos, 
											 float yPos,
											 float backRed,
											 float backGreen,
											 float backBlue,
											 float backAlpha,
											 float frontRed,
											 float frontGreen,
											 float frontBlue,
											 float frontAlpha,
											 float fontSizeX,
											 float fontSizeY)
{
	            EnterCriticalSection(&m_globalMeterSection);
                if(m_globalStatusMeter)
			    {
			       m_globalStatusMeter->SafeDelete();
				   delete m_globalStatusMeter;
				   m_globalStatusMeter = 0;
			    }
				   
			    m_globalStatusMeter = new CMenuBarObj();
			   
				m_globalMeterFontSizeX                  = fontSizeX;
				m_globalMeterFontSizeY                  = fontSizeY;
				m_globalStatusMeterStartX				= xPos;
                m_globalStatusMeterStartY				= yPos;
			    m_globalStatusMeter->SetSizePosPorperties(length, width, xPos, yPos);
				m_globalStatusMeter->Initialize(m_3dDevice);
				m_globalStatusMeter->SetBackColor(backRed, backGreen, backBlue, backAlpha);
				m_globalStatusMeter->SetForeColor(frontRed, frontGreen, frontBlue, frontAlpha);
				LeaveCriticalSection(&m_globalMeterSection);
}
//------------------------------------------------------------------------------------//
void GlobalStatusVariables::RenderMeter()
{
	 if(!m_dynamicFontTexture)
	 {//initialize entire font system from here
        m_dynamicFontTexture = new CTextureEX();
        if(m_dynamicFontTexture->LoadItFromURL(m_contentUrl+"/contentmanager.aspx?fonttexture=1",1,m_3dDevice,TRUE,false,this->m_advUrlBasic,this->m_advUrlVending))
	    {


		}
		else
		{
			m_dynamicFontTexture->SafeDelete();
			delete m_dynamicFontTexture;
			m_dynamicFontTexture = 0;
			return;
		}  
	 }//end initialize entire font system from here

	 if(!m_initFontSystem)
	 { 
	     if(!LoadTextFontInterfaceFromFile("X"))
		 {
             return;
		 }
	 }

     
     EnterCriticalSection(&m_globalMeterSection);
     if(!m_showGlobalMeter)
	 {
		 LeaveCriticalSection(&m_globalMeterSection);
		 return;
	 }

	 if(m_globalStatusMeter)
        m_globalStatusMeter->Render(m_3dDevice,
								    m_renderStateObject,
								    m_textureDataBase,
								    m_timeAdjustFloat
								    );

	 CTextureRenderObj * textObject = new CTextureRenderObj("",5,m_globalStatusMeterStartX+.01f,m_globalStatusMeterStartY,m_globalMeterFontSizeX,m_globalMeterFontSizeY,TRUE,0.5,0.5,0.5);
	 textObject->Initialize(this->m_3dDevice,m_globalStatusMeterText,TRUE,m_symbolMapDB,m_speechMode,0.0f);
	 m_3dDevice->SetTexture( 0,m_dynamicFontTexture->m_pddsSurface);//set texture
	 m_renderStateObject->SetBlendType(m_3dDevice,textObject->m_blendMode);
	 m_renderStateObject->SetMaterial(this->m_3dDevice,&textObject->m_materialObj->m_useMaterial);
	 textObject->Render(this->m_3dDevice,m_renderStateObject);
	 textObject->SafeDelete();
	 delete textObject;
	 textObject = 0;
	 m_3dDevice->SetTexture( 0,0);//reset texture
     m_renderStateObject->SetTextureState(m_renderStateObject->m_intNegOne,m_renderStateObject->m_intZero,m_3dDevice,m_textureDataBase);//reset texture

	 LeaveCriticalSection(&m_globalMeterSection);
}
//------------------------------------------------------------------------------------//
void GlobalStatusVariables::UpdateMeter(float currentPosition)
{
	           EnterCriticalSection(&m_globalMeterSection);
               //render progress meter test
			   if(!m_globalStatusMeter)
			   {
			       m_globalStatusMeter = new CMenuBarObj();
				  
				   m_globalStatusMeter->SetSizePosPorperties(0.4f, 0.1f, m_globalStatusMeterStartX, m_globalStatusMeterStartY);

				   m_globalStatusMeter->Initialize(m_3dDevice);
				   m_globalStatusMeter->SetBackColor(1.0f, 0.0f, 0.0f, .0f);
			       m_globalStatusMeter->SetForeColor(0.0f, 1.0f, 0.0f, .8f);
			   }
			   m_globalStatusMeter->SetStateByTwoAmounts(currentPosition,100.0f);
               m_globalStatusMeter->Initialize(m_3dDevice);
			   LeaveCriticalSection(&m_globalMeterSection);
}
//------------------------------------------------------------------------------------//
void GlobalStatusVariables::UpdateMeterAdv(float currentPosition, CString infoTxt)
{
	 m_globalStatusMeterText = infoTxt;
     UpdateMeter(currentPosition);
     LoadSceneInfo();
	 //AXPumpMsgs();
	 Sleep(0);
}
//------------------------------------------------------------------------------------//
void GlobalStatusVariables::DeleteSpawnDatabase() 
{
if(!m_spawnGeneratorDB)
    return;

	for(POSITION posLoc = m_spawnGeneratorDB->GetHeadPosition();posLoc!=NULL;){
		CSpawnObj * spawnGenPtr = ( CSpawnObj* )m_spawnGeneratorDB->GetNext(posLoc);
        delete spawnGenPtr;
	 	spawnGenPtr=0;
	}
	m_spawnGeneratorDB->RemoveAll();
	//SpawnGenDatabaseInList(0);
	delete m_spawnGeneratorDB;
	m_spawnGeneratorDB=0;
	
}
//--------------destroys world Object list--------------------------------------------------//
void GlobalStatusVariables::DestroyWorldObjects()
{
	if(!m_wldObjList)
		return;
	for(POSITION posLoc = m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	    CWldObject * ptrLoc = ( CWldObject* ) m_wldObjList->GetNext(posLoc);	
		ptrLoc->SafeDelete();
		delete ptrLoc;
		ptrLoc=0;
	}//end Begin world object loop
	m_wldObjList->RemoveAll();
	delete m_wldObjList;
	m_wldObjList=0;
}
//--------------destroys environment--------------------------------------------------//
void GlobalStatusVariables::DestroyEnvironment()
{
	if(!m_environmentPtr)
	return;
	m_environmentPtr->SafeDelete();
	delete m_environmentPtr;
	m_environmentPtr=0;
}
//--------------destroys entity list--------------------------------------------------//
void GlobalStatusVariables::DestroyEntityRuntimeList()
{
	if(!m_entObjRuntime)
		return;

	for(POSITION posLoc = m_entObjRuntime->GetHeadPosition();posLoc != NULL;){//visual loop
			CMovementObj * mPtr = ( CMovementObj* ) m_entObjRuntime->GetNext(posLoc);
			DestroyRuntimeEntity(mPtr,m_spawnGeneratorDB,m_entObjRuntime);
			mPtr = 0;
	}
	m_entObjRuntime->RemoveAll();
}
//------------------------------------------------------------------------------------//
void GlobalStatusVariables::DeleteWorldGroupList()
{
     if(m_worldGroupList)
	 {
      for(POSITION posLoc = m_worldGroupList->GetHeadPosition();posLoc != NULL;){
          CWldGroupObj * partPtr = (CWldGroupObj *)m_worldGroupList->GetNext(posLoc);
          delete partPtr;
		  partPtr=0;
	  }
	  m_worldGroupList->RemoveAll();
	  delete m_worldGroupList;
	  m_worldGroupList=0;
	 }
}
//------delete script pointers--------------------------------------------------------//
void GlobalStatusVariables::DeleteScriptList(CEXScriptObjList ** focusLoc)
{
CEXScriptObjList * ptrLoc = *focusLoc;
if(!ptrLoc)
return;

 for(POSITION posLoc = ptrLoc->GetHeadPosition();posLoc != NULL;)
 {
     CEXScriptObj * scrPtr = (CEXScriptObj*) ptrLoc->GetNext(posLoc);
     delete scrPtr;
	 scrPtr=0;
 }
 ptrLoc->RemoveAll();
 delete ptrLoc;
 ptrLoc = 0;
 *focusLoc = ptrLoc;
}
//-----------Run all attributes-------------------------------------------------------//
BOOL GlobalStatusVariables::ExecAllAttributes(CEXScriptObjList * scriptList)
{
//0 = play movie m_fullScreen
//1 = open scene
//2 = load missile database
//3 = load explosion database
//4 = load movie database
//5 = load controls database
//6 = load default cameras
//7 = load anim textures database
//8 = initialize mouse object
//9 = load 2D collections database
//10 = load text/font collections database
//11 = load Sound Database
//12 = start in m_fullScreen 640 / 480
//14 = load entity database
//15 = load mouse Database
//16 = show mouse
//17 = init a movie runtime
//18 = init 2D diplay collection
//19 = set all cameras to focus
//20 = init TCP net game AutoInit TCP/IP Game
//21 = Init Hud Database
//
//24 = Load Particle Systems
//26 = Load SoundTrack
//
	m_axLogger.WriteDebugFile("ExecAllAttributes: meter",0,1);
	UpdateMeterAdv(0,"CACHE");

	if(!scriptList)
		return FALSE;

    int totalScriptObjects = (int)scriptList->GetCount();
    int meterTrace = 0;
	m_axLogger.WriteDebugFile("ExecAllAttributes: objects=",0,0);
	m_axLogger.WriteDebugFile((long)totalScriptObjects,0,1);

	
	for(POSITION posLoc = scriptList->GetHeadPosition();posLoc != NULL;)
	{
        CEXScriptObj * sctPtr = (CEXScriptObj *)scriptList->GetNext(posLoc);
		WriteDebugFileLive(555,sctPtr->List_actionAttribute);
		//update progress meter
		
		/*UpdateMeterAdv((100/totalScriptObjects)*meterTrace,"CACHE:"+sctPtr->List_miscString);*/		
		UpdateMeterAdv((float)(100/totalScriptObjects)*meterTrace,"CACHE:"+sctPtr->List_miscString);
		//WARNING: C4244 --> int to float

		m_axLogger.WriteDebugFile("Process Script Object:",0,0);
		m_axLogger.WriteDebugFile((long)sctPtr->List_actionAttribute,0,0);
		m_axLogger.WriteDebugFile(" ",0,0);
	    m_axLogger.WriteDebugFile(sctPtr->List_miscString.GetBuffer(),0,1);

		switch(sctPtr->List_actionAttribute)
		{
			case 0:
			{//play full screen movie
			   PlayMoveFullscreen(sctPtr->List_miscString);
			   break;
			}//end play fullsceen movie
			case 1:
			{ //open scene
			   LoadScene(sctPtr->List_miscString);
			   m_currentWorkingFile = sctPtr->List_miscString;
			   break;
			}//end open scene
			case 2:
			{//load missile DB
			   LoadMissileDatabase(sctPtr->List_miscString);
			   break;
			}//end load missile DB
			case 3:
			{//load explosion DB
				LoadExplosionFile(sctPtr->List_miscString);
				break;
			}//end load explosion DB
			case 4:
			{//load runtime movie DB
			   LoadMoviesRuntimeDB(sctPtr->List_miscString);
			   break;
			}//end load runtime movie DB
			case 5:
			{//open controls cfg list
			   LoadControlCfgList(sctPtr->List_miscString);
			   break;
			}//end open controls cfg list
			case 6:
			{//load cameras and vports
			   LoadVportCameraConfigurations(sctPtr->List_miscString);
			   break;
			}//end load cameras and vports
			case 7:
			{//load animated texture DB
			   LoadAnimTextureSwapDB(sctPtr->List_miscString);
			   break;
			}//end load animated texture DB
			case 8:
			{//init mouse object
				break;
			}//end init mouse object
			case 9:
			{//load 2D configurations
				break;   
			}//end load 2D configurations
			case 10:
			{//load text font interface
			   LoadTextFontInterfaceFromFile(sctPtr->List_miscString);
			   break;
			}//end load  text font interface
			case 11:
			{//load sound bank
			   LoadSoundManager(sctPtr->List_miscString);
			   break;
			}//end load sound bank
			case 12:
			{//full screen initial load
			   CreateDisplayIM(TRUE,0,0,800,600,16);
			   break;
			}//end m_fullScreen initial load
			case 14:
			{//load entity database
			   LoadEntityDatabase(sctPtr->List_miscString);
			   break;
			}//end load entity database
			case 15:
			{//load mouse Database
				break;
			}//end load mouse Database
			case 16:
			{//show mouse
			   //else if(m_currentMousePtr)
				// m_currentMousePtr->List_inUse = TRUE;
				break;
			}//end show mouse
			case 17:
			{//play runtime movie
			   RunRuntimeMovie(sctPtr->List_miscInt);
			   break;
			}//end play runtime movie
			case 18:
			{//activate 2d diplay collection
				break;   
			}//end activate 2D diplay collection
			case 19:
			{//set all vieports to focus
			   //AssignAllviewportsToFocus();
				break;
			}//end set all vieports to focus
			case 20:
			{//set init TCP/IP game default dialogs
				break;
			}//end set init TCP/IP game default dialogs
			case 21:
			{//load Hud database
			   LoadHudDatabase(sctPtr->List_miscString);
			   break;
			}//end load Hud database
			case 22:
			{//load Menu database
			   BOOL bResult = LoadMenuSys(sctPtr->List_miscString);				
			   if (bResult)
			   {
				   // KLUDGE: The startup scripting system does not appear to be reading
				   //	in the associated misc INT data for CEXScriptObj objects. We
				   //	are hardcoding the misc INT assignment to open the SYSTEM READOUT
				   //	menu on a successful load of the menus system.
				   sctPtr->List_miscInt = SYSTEM_READOUT;
				   if(sctPtr->List_miscInt != -1)
					   OpenMenu(sctPtr->List_miscInt, false);
			   }
			   break;
			}//end load Hud database
			case 23:
			{//open menu
			   OpenMenu(sctPtr->List_miscInt, false);
			   break;
			}//end open menu
			case 24:
			{//Load Particle Systems
				LoadParticleDatabase(sctPtr->List_miscString);
				break;
			}//end open menu
			case 25:
			{//Load texture databank
			   LoadTextureBank(sctPtr->List_miscString);
			   break;
			}//end Load texture databank
			case 26:
			{//Load SoundTrack
			   LoadSoundTrackDB(sctPtr->List_miscString);
			   break;
			}//end Load texture databank
			case 27:
			{//Load AI cfg
			   LoadAiData(sctPtr->List_miscString);
			   break;
			}//end Load AI cfg
			case 28:
			{//Load Global Inventory
			   ///LoadGlobalInventoryData(sctPtr->List_miscString);//obsolete
				break;
			}
			case 29:
			{//Load Commerce Database
			   LoadGlobalCommerceData(sctPtr->List_miscString);
			   break;
			}
			case 30:
			{//Load Bank Zones
			   LoadBankZonesData(sctPtr->List_miscString);
			   break;
			}	
			case 31:
			{//Load Safe Zones
			   LoadSafeZonesData(sctPtr->List_miscString);
			   break;
			}
			case 32:
			{//Load item generator
				//		   LoadItemGeneratedData(sctPtr->List_miscString);				//No longer used as per DJ
				//		}//end Load item generator
				break;
			}
			case 33:
			{//static item generator
			   LoadStaticTextureBank(sctPtr->List_miscString);
			   break;
			}//end static item generator
			case 34:
			{//Load water zones DB
			   LoadWaterZoneDB(sctPtr->List_miscString);
			   break;
			}//end //Load water zones DB
			case 35:
			{//load portal database
			   LoadPortalDatabase(sctPtr->List_miscString);
			   break;
			}//end load portal database
			case 36:
			{//load element database
			   LoadElementDatabase(sctPtr->List_miscString);
			   break;
			}//end load element database
			case 37:
			{//Load Stats Database
			   LoadStatsDatabase(sctPtr->List_miscString);
			   break;
			}//end Load Stats Database
			case 38:
			{//Load Skills Database
			   LoadSkillsDatabase(sctPtr->List_miscString);
			   break;
			}//end Load Skills Database
			case 39:
			{//Load AI Collision Database
			   LoadAICollisionDB(sctPtr->List_miscString);    
			   break;
			}//end Load Skills Database
			case 40:
			{//Load Icon Database
			   LoadIconDB(sctPtr->List_miscString);    
			   break;
			}//end Load Icon Database
			case 41:
			{//Load House Zones
			   LoadHouseZoneDB(sctPtr->List_miscString);
			   break;
			}//end Load House Zones
			case 42:
			{//Load House DB
			   LoadHouseDB(sctPtr->List_miscString);
			   break;
			}//end Load House DB
			case 43:
			{//Load AI Raid DB
			   LoadAIRaidDB(sctPtr->List_miscString);
			   break;
			}//end Load AI Raid DB
			case 44:
			{//Load AI Raid DB
			   LoadCommControls(sctPtr->List_miscString);
			   break;
			}//end Load AI Raid DB
			case 45:
			{//Load Arena Sys
			   LoadArenaSys(sctPtr->List_miscString);
			   break;
			}//end Load Arena Sys
			case 46:
			{//Load World Rules
			   LoadWorldRules(sctPtr->List_miscString);
			   break;
			}//end Load World Rules
			case 47:
			{//Set Username
			   m_multiplayerObject->m_userName = sctPtr->List_miscString;
			   break;
			}//end Set Username
			case 48:
			{//Set Password
			   m_multiplayerObject->m_passWord = sctPtr->List_miscString;
			   break;
			}//end Set Password
			case 49:
			{//Set AIserverType Flags
				m_dedicatedServer = TRUE;
				m_multiplayerObject->m_AIServer = TRUE;
				break;
			}//end Set AIserverType Flags
			case 50:
			{//Set Connect Join
				// Editor: never auto-connect from a startup script. Connected, a zone does not
				// spawn its own entities (the server would), so the editor had no user entity
				// to move and the camera was stuck.
				if(m_editorModeOn)
				{
					m_axLogger.WriteDebugFile("connect step: editor - skipped (zones spawn their own entities offline)",0,1);
					break;
				}
				// Client with no credentials yet: show the in-game login window instead of
				// connecting. Its LOGIN button runs Login(user,pass) which connects.
				if(!m_editorModeOn && m_multiplayerObject->m_userName.IsEmpty())
				{
					if(OpenMenu(EA_LOGIN_MENU_ID, false))
					{
						m_axLogger.WriteDebugFile("connect step: no credentials - opened in-game login window",0,1);
						break;
					}
					m_axLogger.WriteDebugFile("connect step: login window (ID 20001) not in menu DB - connecting without credentials",0,1);
				}
				m_multiplayerObject->EnableClient(m_multiplayerObject->m_IPaddress,m_multiplayerObject->m_currentVersion);
				break;
			}//end Set Connect Join
			case 51:
			{//Trigger Spawngen
				m_spawnGeneratorDB->m_enable = TRUE;
				Sleep(10000);
				GL_spawnGeneratorTimeVarial = GetTickCount();
				break;
			}//end Trigger Spawngen
			case 52:
			{//load shader lib
				LoadShaderLibrary(sctPtr->List_miscString);
				break;
			}//end Trigger Spawngen
			case 53:
			{//Load Global Shader Lib
				LoadGlobalShaderLibrary(sctPtr->List_miscString);
				break;
			}//end Load Global Shader Lib
			case 54:
			{//Load Global Attachment DB
				//LoadGlobalAttachmentDB(sctPtr->List_miscString);//OBSOLETE
				break;
			}//end Load Global Attachment DB	
			case 55:
			{//Load Pixel Shader Lib
				LoadPixelShaderLibrary(sctPtr->List_miscString);
				break;
			}//end Load Global Attachment DB	
			case 56:
			{//Load post Processor DB
				LoadPostProcessorDB(sctPtr->List_miscString);
				break;
			}//end load post processor DB
			case 57:
			{//Load temp entities
			   LoadEntityDatabaseAsTemp(sctPtr->List_miscString);
			   break;
			}//end Load temp entities
			case 58:
			{//Load temp items
			   //LoadGlobalAttachmentDBAsTemp(sctPtr->List_miscString);
				break;
			}//end load temp items
			case 59:
			{//PlayMusic
			   if(m_soundTrackList)
				  m_soundTrackList->Play(sctPtr->List_miscInt,GL_pathGameSounds);
			   break;
			}//end play music
			case 60: //DELETE ME PLEASE
			{//Load Selection FX DB
				LoadSelectionEffectsDB(sctPtr->List_miscString);
				break;
			}//End Selection FX DB
			case 61:
			{//Load Item Manager
				CString CurDir;
				CString MainDir;
				char buffer[_MAX_PATH];
				_getcwd( buffer, _MAX_PATH );
				MainDir = CurDir = buffer;
				CurDir = CurDir + "\\GameFiles\\";
				SetCurrentDirectory(CurDir);
				delete m_itemManager;
				m_itemManager = 0;
				m_itemManager = new CItemManager();


				m_itemManager->Load(sctPtr->List_miscString.GetBuffer());
				SetCurrentDirectory(GL_workingDirectory);
				break;
			}//End Load Item Manager
			case 62:
			{//Load Dot DB
				LoadDotDB(sctPtr->List_miscString);
				break;
			}//End Load Dot DB
			default: break;
		}
		m_axLogger.WriteDebugFile("  ok",0,1);
		meterTrace++;
	}
	m_axLogger.WriteDebugFile("ExecAllAttributes: done",0,1);
    UpdateMeterAdv(100,"CACHE:Completed");

	SetCurrentDirectory(GL_workingDirectory);

    WriteDebugFileLive(601,0);
return TRUE;
}
//------initialize objects into scene-------------------------------------------------//
BOOL GlobalStatusVariables::InitializeOjectsIntoScene()
{
//HRESULT er;

if(!m_wldObjList)
return FALSE;
   for(POSITION posLoc = m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	   CWldObject * ptrLoc = ( CWldObject* ) m_wldObjList->GetNext(posLoc);
	   InitWorldObject(ptrLoc);
   }//end begin world object loop
return TRUE;
}
//------reinit trace numbers----------------------------------------------------------//
void GlobalStatusVariables::RebuildTraceNumber()
{
if(!m_wldObjList)
return;

GlobalTraceNumber=0;

POSITION posLoc;
for(posLoc = m_entObjList->GetHeadPosition();posLoc != NULL;){//visual loop
	    CMovementObj * mPtr = ( CMovementObj* ) m_entObjList->GetNext(posLoc);
        mPtr->List_traceNumber = GlobalTraceNumber++;
}

    //WORLD OBJECTS
	for(posLoc = m_wldObjList->GetHeadPosition();posLoc != NULL;)
	{//Begin world object loop
		CWldObject * ptrLoc = ( CWldObject* ) m_wldObjList->GetNext(posLoc);
		ptrLoc->List_identity = GlobalTraceNumber++;
	}
}
//----------------update mentity with persist info------------------------------------//
void GlobalStatusVariables::UpdateEntityPersist(CPersistObjList ** pList)
{
  CPersistObjList *persistList = *pList;
  for(POSITION posLoc = persistList->GetHeadPosition();posLoc != NULL;){//Begin movement entity loop
	  CPersistObj * pstLoc = ( CPersistObj* ) persistList->GetNext(posLoc);	
	  CMovementObj * mPtr = m_entObjList->GetByID(pstLoc->List_entDBIndex);
      if(mPtr)
	  {//valid database obj
	  BOOL result = ActivateMovementEntity(NULL,
		                                   0,
		                                   pstLoc->List_entMatrix,
		                                   0,
										   0,
										   pstLoc->List_entDBIndex,
										   pstLoc->List_entControlType,
										   0,
										   0,
										   FALSE,
										   FALSE,
										   FALSE,
										   -1,
										   -1,
										   NULL,
					                       NULL,
										   -1,
										   -1,
										   m_collisionBase,
										   m_collisionDatabase,
										   m_entObjRuntime,
										   m_currentWorkingFile);
	 
	 
	  }//end if valid database obj
  }	
}
//----Resets display----if you specify it will search and diregard videomode----------//
BOOL GlobalStatusVariables::SetAllRenderStatesIM()
{
  if(!m_dxRenderer)
	  return FALSE;

  if(!m_3dDevice)
	  return FALSE;

  if(m_renderStateObject){
     m_renderStateObject->m_anisoMode = m_dxRenderer->m_anisoMode;//push startcfg.aut aniso level to the state manager
     m_renderStateObject->SetPxShader(m_3dDevice,m_pxShaderSystem,-1);
     m_renderStateObject->InitializeAllStates(m_3dDevice);
  }

  return TRUE;
}
//--------------Applys any changes to the environment---------------------------------//
void GlobalStatusVariables::InitializeEnvironment(BOOL EditorRefresh)
{
if(!m_3dDevice)
    return;

if(!m_environmentPtr)
m_environmentPtr = new CEnvironmentObj();
//handle network sync
long timePassedTracker = 0;
if(m_multiplayerObject->GetEnabledClient_safe())
{
   timePassedTracker = GetTickCount()-m_multiplayerObject->m_serverStartTime;
}

m_environmentPtr->InitAll(m_3dDevice,
						GL_WORLD,
						&m_backgroundColorGlobal,
						m_dirSnd,
						m_renderStateObject,
						timePassedTracker);

    /*if(m_editorModeOn == TRUE && EditorRefresh == TRUE){
       m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_StartRange,m_cabLib.FloatToStingCnv(m_environmentPtr->m_fogStart));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_EndRange,m_cabLib.FloatToStingCnv(m_environmentPtr->m_fogRange));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_Density,m_cabLib.FloatToStingCnv(m_environmentPtr->m_fogDensity));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_FogRed,m_cabLib.FloatToStingCnv(m_environmentPtr->m_fogRed));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_FogGreen,m_cabLib.FloatToStingCnv(m_environmentPtr->m_fogGreen));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_FogBlue,m_cabLib.FloatToStingCnv(m_environmentPtr->m_fogBlue));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_BckRed,m_cabLib.FloatToStingCnv(m_environmentPtr->m_backRed));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_BckGreen,m_cabLib.FloatToStingCnv(m_environmentPtr->m_backGreen));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_BckBlue,m_cabLib.FloatToStingCnv(m_environmentPtr->m_backBlue));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_sunPosX,m_cabLib.FloatToStingCnv(m_environmentPtr->m_sunPosition.x));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENVsunPosY,m_cabLib.FloatToStingCnv(m_environmentPtr->m_sunPosition.y));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENVsunPosZ,m_cabLib.FloatToStingCnv(m_environmentPtr->m_sunPosition.z));
       m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_MaxWindStrength,m_cabLib.FloatToStingCnv(m_environmentPtr->m_maxWindStrength));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_WindDirX,m_cabLib.FloatToStingCnv(m_environmentPtr->m_windDirectionX));
	   m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_WindDirZ,m_cabLib.FloatToStingCnv(m_environmentPtr->m_windDirectionZ));
       m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_Gravity,m_cabLib.FloatToStingCnv(m_environmentPtr->m_gravity));
       m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_SeaLevel,m_cabLib.FloatToStingCnv(m_environmentPtr->m_seaLevel));
       m_enviormentBar.SetDlgItemText( IDC_EDIT_ENV_SeaDrag,m_cabLib.FloatToStingCnv(m_environmentPtr->m_seaViscusDrag));

	   if(m_environmentPtr->m_ambientLightOn==TRUE)
           m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_AmbientLightOn,BM_SETCHECK ,BST_CHECKED,0);
           else
		   m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_AmbientLightOn,BM_SETCHECK,BST_UNCHECKED,0);
       
	   if(m_environmentPtr->m_enableTimeSystem==TRUE)
           m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_TimeSystem,BM_SETCHECK ,BST_CHECKED,0);
           else
		   m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_TimeSystem,BM_SETCHECK,BST_UNCHECKED,0);
	   
	   if(m_environmentPtr->m_enableWindRandomizer==TRUE)
           m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_WindRandomizer,BM_SETCHECK ,BST_CHECKED,0);
           else
		   m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_WindRandomizer,BM_SETCHECK,BST_UNCHECKED,0);

	   if(m_environmentPtr->m_sunLightOn==TRUE)
           m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_enableSun,BM_SETCHECK ,BST_CHECKED,0);
           else
		   m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_enableSun,BM_SETCHECK,BST_UNCHECKED,0);

	   if(m_environmentPtr->m_fogOn==TRUE)
           m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_FogOn,BM_SETCHECK ,BST_CHECKED,0);
           else
		   m_enviormentBar.SendDlgItemMessage(IDC_CHECK_ENV_FogOn,BM_SETCHECK,BST_UNCHECKED,0);

	}*/
}
//---------------------delete display Unit--------------------------------------------//
BOOL GlobalStatusVariables::SetViewMatrixAB( D3DMATRIX& mat, D3DXVECTOR3& vFrom, D3DXVECTOR3& vAt, D3DXVECTOR3& vWorldUp )
{
  

    D3DVECTOR vView = vAt;//- vFrom;
    //Get the dot product, and calculate the projection of the z basis
    //vector onto the up vector. The projection is the y basis vector.
    FLOAT fDotProduct = m_matLib.DotProduct( vWorldUp, vView );
    D3DXVECTOR3 vUp = vWorldUp - fDotProduct * vView;
    D3DXVECTOR3 vRight = m_matLib.CrossProduct( vUp, vView );

    //Start building the matrix. The first three rows contains the basis
    //vectors used to rotate the view to point at the lookat point
    m_matLib.InitMat(mat.m);
    mat._11 = vRight.x;    mat._12 = vUp.x;    mat._13 = vView.x;
    mat._21 = vRight.y;    mat._22 = vUp.y;    mat._23 = vView.y;
    mat._31 = vRight.z;    mat._32 = vUp.z;    mat._33 = vView.z;

    //Do the translation values (rotations are still about the eyepoint)
    mat._41 = - m_matLib.DotProduct( vFrom, vRight );
    mat._42 = - m_matLib.DotProduct( vFrom, vUp );
    mat._43 = - m_matLib.DotProduct( vFrom, vView );
return TRUE;
}
//---------------------delete display Unit--------------------------------------------//
BOOL GlobalStatusVariables::SetViewMatrixAB( D3DMATRIX& mat, D3DVECTOR& vAt, D3DVECTOR& vWorldUp )
{
  

    D3DVECTOR vView = vAt;//- vFrom;
    //Get the dot product, and calculate the projection of the z basis
    //vector onto the up vector. The projection is the y basis vector.
    FLOAT fDotProduct = m_matLib.DotProduct( vWorldUp, vView );
    D3DVECTOR vUp;
	vUp.x = vWorldUp.x - fDotProduct * vView.x;
	vUp.y = vWorldUp.y - fDotProduct * vView.y;
	vUp.z = vWorldUp.z - fDotProduct * vView.z;
    D3DVECTOR vRight = m_matLib.CrossProduct( vUp, vView );

    //Start building the matrix. The first three rows contains the basis
    //vectors used to rotate the view to point at the lookat point
    m_matLib.InitMat(mat.m);
    mat._11 = vRight.x;    mat._12 = vUp.x;    mat._13 = vView.x;
    mat._21 = vRight.y;    mat._22 = vUp.y;    mat._23 = vView.y;
    mat._31 = vRight.z;    mat._32 = vUp.z;    mat._33 = vView.z;

    //Do the translation values (rotations are still about the eyepoint)
    mat._41 = 0.0f;//- m_matLib.DotProduct( 0.0f, vRight );
    mat._42 = 0.0f;//- m_matLib.DotProduct( vFrom, vUp );
    mat._43 = 0.0f;//- m_matLib.DotProduct( vFrom, vView );
return TRUE;
}
//-----------------------------------------------------------------------------
HRESULT GlobalStatusVariables::D3DUtil_SetProjectionMatrix( D3DMATRIX& mat, FLOAT fFOV, FLOAT fAspect,FLOAT fNearPlane, FLOAT fFarPlane,float hieghtBase,float & clipWidth, float & clipHeight)
{
 
	if( fabs(fFarPlane-fNearPlane) < 0.01f )
        return E_INVALIDARG;
    if( fabs(sin(fFOV/2)) < 0.01f )
        return E_INVALIDARG;

    float    h, w, Q;
    
	//True extents of this projection at unit depth, derived from the matrix
	//built below (fAspect is height/width in this engine: x clips where
	//x*fAspect*cot(fov/2) == z, y clips where y*hieghtBase*cot(fov/2) == z).
	//The old clipWidth formula (1+(1.08-H/W))*2tan(fov/2) was tuned to equal
	//4/3 at exactly 4:3 and drifted at every other aspect ratio: ~15% too
	//narrow at 16:9, ~29% too narrow at 21:9. These extents define the
	//targeting ray (UserController::TargetingSystemCallback maps cursor
	//pixels into this clip box), so on widescreen the aim ray drifted away
	//from what the cursor pointed at. Exact now at 4:3 (within 0.25%),
	//16:9, 21:9 and everything in between.
	float aspectSafe = (fabs(fAspect)    < 0.0001f) ? 0.75f : fAspect;
	float heightSafe = (fabs(hieghtBase) < 0.0001f) ? 1.0f  : hieghtBase;
	clipWidth  = (tan(fFOV * 0.5f)*2.0f) / aspectSafe;
	clipHeight = (tan(fFOV * 0.5f)*2.0f) / heightSafe;

    w =  fAspect * (float)1/tan(fFOV *0.5f);  //1/tan(x) == cot(x)
	//w = (float)1/tan((fAspect*fFOV) *0.5f);
    h = hieghtBase  * (float)1/tan(fFOV *0.5f);   //1/tan(x) == cot(x)
    Q = fFarPlane/(fFarPlane - fNearPlane);


    //FLOAT w = fAspect * ( cosf(fFOV/2)/sinf(fFOV/2) );
    //FLOAT h = hieghtBase  * ( cosf(fFOV/2)/sinf(fFOV/2) );
    //FLOAT Q = fFarPlane / ( fFarPlane - fNearPlane );

    ZeroMemory( &mat, sizeof(D3DMATRIX) );
    mat._11 = w;
    mat._22 = h;
    mat._33 = Q;
    mat._34 = 1.0f;
    mat._43 = -Q*fNearPlane;

    return S_OK;
}
//-----------------------------------------------------------------//
void GlobalStatusVariables::WriteDebugFileLive(int errorcode1,int errorcode2)
{
	if(!m_debuggingOn)
		return;
   
	//Keys 0=mail server loop 1=main receive loop 2=attribute func 3=closing cleanup
   
    if(errorcode1 == -1)
	{
	   m_debugfl.SetLength(0);
     
	   return;
	}
	
    CString build = m_cabLib.ConvToString(errorcode1)+" "+m_cabLib.ConvToString(errorcode2)+"\n";
	int sz = build.GetLength();
	char * buffer = new char[sz];
	for(int i=0;i<sz;i++)
	{
        buffer[i] = build.GetAt(i);
	}
	
 
	m_debugfl.Write( buffer, sz );
	
	delete [] buffer;
	buffer=0;
}
//--------------play Movie-fullScreen----------------------------------------------//
void GlobalStatusVariables::PlayMoveFullscreen(const char * pszFileName)
{
}
//-----------missile object material--------------------------------------------------//
BOOL GlobalStatusVariables::LoadMissileDatabase(CString fileName)
{
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization	 
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res)
	 {
		 DestroyAllMissileClasses();
		 m_missileRuntime = new CMissileObjList;
		 CArchive the_in_Archive(&fl,CArchive::load);
		 the_in_Archive >> m_missileDB;
		 the_in_Archive.Close();
		 fl.Close();
	 }
     SetCurrentDirectory(GL_workingDirectory);

	 return TRUE;
}
//---------------Load explosion data--------------------------------------------------//
BOOL GlobalStatusVariables::LoadExplosionFile(CString fileName)
{
	 SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization    	 
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res)
	 {
		 m_explosionDBList->SafeDelete();
		 delete m_explosionDBList;
		 m_explosionDBList=0;
		 CArchive the_in_Archive(&fl,CArchive::load);
		 the_in_Archive >> m_explosionDBList;
		 the_in_Archive.Close();
		 fl.Close();
	 }
     SetCurrentDirectory(GL_workingDirectory);
     
	 return TRUE;
}
//-----------------------------function--------------------------
BOOL GlobalStatusVariables::LoadMoviesRuntimeDB(CString fileName)
{ 
	 DestroyAllMovieObjects();
	 SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization 
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res)
	 {
		 CArchive the_in_Archive(&fl,CArchive::load);
		 the_in_Archive >> MovieRList;
		 the_in_Archive.Close();
		 fl.Close();
	 }
	 SetCurrentDirectory(GL_workingDirectory);
  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		GlobalStatusVariables
// Function:	LoadControlCfgList(CString, CMenuListBox*)
// Description:	Load the default set of controls hardcoded in the source code.
//				The default controls are created and initialized in the 
//				ControlInfoList implementation file.
///////////////////////////////////////////////////////////////////////////////

BOOL 
GlobalStatusVariables::LoadControlCfgList(CString fileName, CMenuListBox *pListBox)
{
	POSITION pos = NULL;
	ControlDBObj *pControlDB = NULL;
	CMenuListItem *pItem = NULL;
	CMenuText *pAction = NULL, *pKey = NULL;
	CString strKey = NULL, strAction = NULL;

	// Set the current directory to the Game Files directory
	SetCurrentDirectory(GL_pathGameFiles);

	// Re-initialize the list of control database objects
	DestroyControlList();
	InitializeControlDB();
	ASSERT(this->m_controlsList);

	// Create/Retrieve a ControlDBObj instance to be used for
	// storing all the action->key bindings
	pos = this->m_controlsList->FindIndex(0);
	if(pos) 
	{
		pControlDB = (ControlDBObj*)this->m_controlsList->GetAt(pos);
	}
	else
	{
		pControlDB = new ControlDBObj();
		this->m_controlsList->AddHead(pControlDB);
	}

	if(pListBox)
	{
		if(pListBox->m_scrollBar)
		{
			pListBox->m_scrollBar->m_curPosition = 0;
			pListBox->m_scrollBar->Update();
		}

		// Clear the display listbox of any pre-existing bindings
		pListBox->ClearList();
	}

	// Loop through the hardcoded default bindings,
	// adding them to a listbox for display if necessary
	// and creating event listeners for each valid binding
	for(int i=0; i<ActionMap.size(); i++)
	{
		KEY_CONFIG_PAIR pair = DefaultKeyConfigs[i];
		int iAction = pair.first;
		int iKey = pair.second;
		CString strAction = ControlInfoList::ReturnActionByIndex(iAction);

		// GET the display string for the key config menu
		std::map<int, CString>::const_iterator iter = KeyMap.find(iKey);
		if (iter != KeyMap.end()) 
		{
			strKey = iter->second;
		}
		else
		{
			iter = MouseMap.find(iKey);
			if (iter != MouseMap.end()) 
				strKey = iter->second;
		}

		// Add menu item to listbox
		if(pListBox && (strAction != ""))
		{
			pItem = new CMenuListItem();
			pAction = new CMenuText();
			pKey = new CMenuText();

			pAction->m_text = strAction;
			pKey->m_text = strKey;

			// Add menu item to CMenuListBox
			pItem->m_textList.AddHead(pKey);
			pItem->m_textList.AddHead(pAction);
			pListBox->AddItem(pItem);
		}

		// Create event listeners for this action->key binding 
		// and unassign any pre-existing binding for this key
		pControlDB->List_controlInfoList->AddControlEvent(strKey,
			strAction,//actionString,
			0,//inventoryGlid,
			0,//miscInt2,
			0,//miscInt3,
			0,//miscInt4);
			FALSE,
			FALSE);
	}

	// Set actual value for mouse sensitivity
	// NOTE: Does not necessarily correspond to the display value
	float fStepAmount = ((MAX_MOUSE_SENSITIVITY - MIN_MOUSE_SENSITIVITY) / MAX_MOUSE_SENSITIVITY_STEPS);
	float fCurrentValue = 3; //MAX_MOUSE_SENSITIVITY_STEPS / 2.0f;
	pControlDB->m_mouseSensitivity = fStepAmount * fCurrentValue;

	// Init mouse invert value
	pControlDB->m_invertMouse = FALSE;

	// Init multibind
	pControlDB->m_allowMultiBind = FALSE;

	// Init music volume value (half of the max volume)
	pControlDB->m_musicVolume = 1; //VOLUME_SCALED_MAX / 2;

	// Reset the working directory
	SetCurrentDirectory(GL_workingDirectory); 

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		GlobalStatusVariables
// Function:	LoadControlCfgFromDisk(CString, CMenuListBox*)
// Description:	Load set of controls saved on disk.
//				The file should be found in the Game Files directory and
//				named "Controls.CFG"
///////////////////////////////////////////////////////////////////////////////

BOOL
GlobalStatusVariables::LoadControlCfgFromDisk(CString fileName, CMenuListBox *pListBox)
{
	CString			strMouseSensitivity,
					strFilePath,
					strKey,
					strAction;
	char			buffer[MAX_BUFFER];
	bool			closing, 
					bInvert;
	int				version, 
					iMouseSensitivity;
	BOOL			res = FALSE;
	POSITION		pos = NULL;
	ControlDBObj	*pControlDB = NULL;
	CMenuListItem	*pItem = NULL;
	CMenuText		*pAction = NULL, 
					*pKey = NULL;
	ControlInfoObj	*pControlInfo = NULL;
	CFileException	exc;
	CFile			fl;

	// Set the current directory to the Game Files directory
	SetCurrentDirectory(GL_pathGameFiles);

	// Re-initialize the list of control database objects
	DestroyControlList();
	InitializeControlDB();
	ASSERT(this->m_controlsList);

	// Try to load the key configs from disk
	// If the saved configs can't be found, load default bindings
	res = fl.Open(fileName, CFile::modeRead, &exc);
	if(res)
	{
		// NOTE: Mouse settings and key binding event listeners
		// are auto-loaded into the ControlDBObj here since we're 
		// using MFC serialization.
		CArchive the_in_Archive(&fl, CArchive::load);
		the_in_Archive >> m_controlsList;
		the_in_Archive.Close();
		fl.Close();
	}
	else
	{
		return this->LoadControlCfgList(fileName, pListBox);
	}

	if(pListBox)
	{
		if(pListBox->m_scrollBar)
		{
			pListBox->m_scrollBar->m_curPosition = 0;
			pListBox->m_scrollBar->Update();
		}

		// Clear the display listbox of any pre-existing bindings
		pListBox->ClearList();
	}
	
	// Create/Retrieve a ControlDBObj instance to be used for
	// storing all the action->key bindings
	pos = this->m_controlsList->FindIndex(0);
	if(pos) 
	{
		pControlDB = (ControlDBObj*)this->m_controlsList->GetAt(pos);
	}
	else
	{
		pControlDB = new ControlDBObj();
		this->m_controlsList->AddHead(pControlDB);
	}
	
	// Update listbox with default action->key mappings
	// and create an event listener for each
	// NOTE: The current serialization technique only saves assigned 
	// key configs out to disk, so we have to be sure to add 
	// unassigned bindings as well.
	for(int i=0; i<ActionMap.size(); i++)
	{
		KEY_CONFIG_PAIR pair = DefaultKeyConfigs[i];
		int iAction = pair.first;
		int iKey = pair.second;
		CString strAction = ControlInfoList::ReturnActionByIndex(iAction);
		CString strKey = "Unassigned";

		// If this action->key binding has loaded from disk, 
		// replace the ACTION and KEY strings
		std::map<int, CString>::const_iterator iter;
		pControlInfo = pControlDB->List_controlInfoList->GetByActionName(strAction);
		if(pControlInfo)
		{
			iKey = pControlInfo->List_buttonDown;
			if(pControlInfo->List_controlType == 1) iKey += KEYBOARD_MAX_KEYS;

			strAction = ControlInfoList::ReturnActionByIndex(pControlInfo->List_action);
			
			// GET the display string for the key config menu
			iter = KeyMap.find(iKey);
			if (iter != KeyMap.end()) 
			{
				strKey = iter->second;
			}
			else
			{
				iter = MouseMap.find(iKey);
				if (iter != MouseMap.end()) 
					strKey = iter->second;
			}
		}

		// Add menu item to listbox
		if(pListBox && (strAction != ""))
		{
			// Read the key config item into the list
			pItem = new CMenuListItem();
			pAction = new CMenuText();
			pKey = new CMenuText();

			// Read in
			pAction->m_text = strAction;
			pKey->m_text = strKey;

			// Add menu item to CMenuListBox
			pItem->m_textList.AddHead(pKey);
			pItem->m_textList.AddHead(pAction);
			pListBox->AddItem(pItem);
		}
	}

	// SET the music volume
	if (this->m_soundTrackList)
		this->m_soundTrackList->SetVolume((int)pControlDB->m_musicVolume * VOLUME_SCALED_MAX);

	// Reset the working directory
	SetCurrentDirectory(GL_workingDirectory); 
	return TRUE;
}
//----load vieports and cameras-------------------------------------------------------//
BOOL GlobalStatusVariables::LoadVportCameraConfigurations(CString fileName)
{
	 SetCurrentDirectory(GL_pathGameFiles);
     //end use gamefiles folder for organization 	 
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res)
	 {

		 if(m_runtimeViewportDB)
		 {
			m_runtimeViewportDB->SafeDelete();
			delete m_runtimeViewportDB;
			m_runtimeViewportDB = 0;
		 }
		 if(m_viewPortDB)
		 {
			m_viewPortDB->SafeDelete();
			delete m_viewPortDB;
			m_viewPortDB = 0;
		 }
		 if(m_cameraList)
		 {
			m_cameraList->SafeDelete();
			delete m_cameraList;
			m_cameraList = 0;
		 }

		 m_runtimeViewportDB =  new CViewportList;

		 CArchive the_in_Archive(&fl,CArchive::load);
		 the_in_Archive >> m_cameraList >> m_viewPortDB;
		 the_in_Archive.Close();
		 fl.Close();
	 }
		 
	 SetCurrentDirectory(GL_workingDirectory);
	
 
  return TRUE;
}
//-----------missile object material--------------------------------------------------//
void GlobalStatusVariables::LoadAnimTextureSwapDB(CString fileName)
{
    
	 SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization 
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res)
	 {
		 DestroyTextureCollectionList(TextureSwapList);
		 CArchive the_in_Archive(&fl,CArchive::load);
		 the_in_Archive >> TextureSwapList;
		 the_in_Archive.Close();
		 fl.Close();
		 InitAllTexCollections();
	 }
	 SetCurrentDirectory(GL_workingDirectory);
	 return;
}
//--------------Load text font data to file-------------------------------------------//
BOOL GlobalStatusVariables::LoadTextFontInterfaceFromFile(CString fileName)
{
     if(m_initFontSystem)
	    return TRUE;

	 CFileException exc;
     CFile fl;
	 BOOL res = fl.Open(GL_pathGameFiles+fileName,CFile::modeRead, &exc);
	 if(res == FALSE)
	 {//if not available try url load
	    CHttpFileDownloader urlFileDownloader;
	    BYTE * buffer = NULL;
	    DWORD bufLength = 0;
		CString url = m_contentUrl+"/contentmanager.aspx?fontpallet=1";
	    if(urlFileDownloader.GetUrlData("iexplorer data",
									      url.GetBuffer(),
										  &buffer,
										  &bufLength))
		{
			m_textInterfaceList->SafeDelete();
			delete m_textInterfaceList;
			m_textInterfaceList = 0;

			m_fontList->SafeDelete();
			delete m_fontList;
			m_fontList = 0;

			m_cgFontList->SafeDelete();
			delete m_cgFontList;
			m_cgFontList=0;

			m_symbolMapDB->SafeDelete();
			delete m_symbolMapDB;
			m_symbolMapDB = 0;
             
			CMemFile g( buffer, bufLength, 0 );

	        CArchive the_in_Archive(&g,CArchive::load);
			the_in_Archive >> m_textInterfaceList >> m_fontList >> m_cgFontList >> m_symbolMapDB;
			

			//m_systemReadout->m_symbolMapSysRef = m_symbolMapDB;
			m_initFontSystem = TRUE;
          
			g.Close();
			delete [] buffer;
			buffer = 0;

			return TRUE;
		}
		else
		    return FALSE;
	 }//end if not avaiable try url load

	 
	  
     m_textInterfaceList->SafeDelete();
	 delete m_textInterfaceList;
	 m_textInterfaceList = 0;

	 m_fontList->SafeDelete();
	 delete m_fontList;
	 m_fontList = 0;

	 m_cgFontList->SafeDelete();
	 delete m_cgFontList;
	 m_cgFontList=0;

     m_symbolMapDB->SafeDelete();
	 delete m_symbolMapDB;
	 m_symbolMapDB = 0;

	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_textInterfaceList >> m_fontList >> m_cgFontList >> m_symbolMapDB;
	 the_in_Archive.Close();
     fl.Close();

	 //m_systemReadout->m_symbolMapSysRef = m_symbolMapDB;
	 m_initFontSystem = TRUE;
	 return TRUE;
}
//-----------missile object material--------------------------------------------------//
BOOL GlobalStatusVariables::LoadSoundManager(CString fileName)
{
    //use gamefiles folder for organization
    SetCurrentDirectory(GL_pathGameFiles);
    //end use gamefiles folder for organization 	 
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	 }
	 m_soundManager->SafeDelete();
	 delete m_soundManager;
	 m_soundManager = 0;
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_soundManager;
	 the_in_Archive.Close();
     fl.Close();
     SetCurrentDirectory(GL_pathGameSounds);
	 m_soundManager->Initialize(m_dirSnd,m_soundListener);
	 SetCurrentDirectory(GL_workingDirectory);
	 return TRUE;
}
//----Resets display----if you specify it will search and diregard videomode----------//
BOOL GlobalStatusVariables::CreateDisplayIM(BOOL fulscreenLoc,int driverLoc,int videoModeLoc,int widthLoc,int hieghtLoc,int depthLoc,HWND hwnd)
{
   //m_dxRenderer->m_hwnd = hwnd;
	CreateDisplayIM(fulscreenLoc,driverLoc,videoModeLoc,widthLoc,hieghtLoc,depthLoc);
	return TRUE;
}
//----Resets display----if you specify it will search and diregard videomode----------//
BOOL GlobalStatusVariables::CreateDisplayIM(BOOL fulscreenLoc,int driverLoc,int videoModeLoc,int widthLoc,int hieghtLoc,int depthLoc)
{
	CacheGeneratedRenderStuff(0);
	m_textRenderList->Flush();
	m_tempWldObjCache->RemoveAll();
	m_blackOut = TRUE;

	if(m_dxRenderer->CreateRenderingDevices(!fulscreenLoc,widthLoc,hieghtLoc,depthLoc,NULL/*m_dxRenderer->m_hwnd*/))
	{//success
        WriteCfgFile("startcfg.aut");
		SetAllRenderStatesIM();
		RestoreVports();
		CalibrateScreenSnap(100,100);

	    m_imediateRenderRECT = m_dxRenderer->GetRenderRect();
		
		float scalar = m_dxRenderer->GetWidthAspectMultiplier();
        if(m_forceScalarForAspect != 0.0f)
           scalar = m_forceScalarForAspect;

		//m_systemReadout->m_cgVertSpacing = m_readoutFontSpacing * scalar;
		//m_systemReadout->m_cgFontSizeX = m_readoutFontX * scalar;
		//m_systemReadout->m_cgFontSizeY = m_readoutFontY * scalar;
		//m_systemReadout->m_maxMeasuredLength = m_readoutMaxMeasuredLength * scalar;
		
		//RefreshMenus();
		//m_systemReadout->Flush();
		//m_systemReadout->ReinitVisualSection(m_3dDevice,m_symbolMapDB,m_speechMode);
		InitializeEnvironment(FALSE); 
 	}//end success

	return TRUE;
}
//------loads scene-------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadEntityDatabase(CString fileName)
{
	 SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res)
	 {
		 char fullEntityPath[MAX_PATH] = { 0 };
		 DWORD fullEntityPathLength = GetFullPathNameA(fileName, MAX_PATH, fullEntityPath, NULL);
		 if(fullEntityPathLength > 0 && fullEntityPathLength < MAX_PATH)
			 m_loadedEntityDatabasePath = fullEntityPath;
		 DestroyEntityList();//destroy list that will be filled
		 CArchive the_in_Archive(&fl,CArchive::load);
		 //DWORD testCt = the_in_Archive.ReadCount();
		 the_in_Archive >> m_entObjList;
		 the_in_Archive.Close();
		 fl.Close(); 
		 InitAllDatabaseEntitys();
		 if(GL_ScenePersistList)
		    UpdateEntityPersist(&GL_ScenePersistList);//fill in runtime entities
	 }
	 SetCurrentDirectory(GL_workingDirectory);
  return TRUE;
}
//------loads scene-------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadEntityDatabaseAsTemp(CString fileName)
{
	 SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization
	 
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res)
	 {	 
		 CMovementObjList * tempList = NULL;
		 CArchive the_in_Archive(&fl,CArchive::load);
		 the_in_Archive >> tempList;
		 the_in_Archive.Close();
		 fl.Close();
		 m_entObjList->ClearTempEntities();
		 m_entObjList->MergeAsTempEntities(tempList);
		 tempList->SafeDelete();
		 delete tempList;
		 tempList = 0;
		 InitAllDatabaseEntitys();
	 }
	 SetCurrentDirectory(GL_workingDirectory);
  return TRUE;
}
//----------------Load Selection Effects Database-------------------------------------//
BOOL GlobalStatusVariables::LoadSelectionEffectsDB(CString fileName)
{
	//Use GameFiles folder for organization
	CString CurDir;
	CString MainDir;
	char buffer[_MAX_PATH];
	_getcwd( buffer, _MAX_PATH );
	MainDir = CurDir = buffer;
	CurDir = CurDir + "\\GameFiles\\";
	SetCurrentDirectory(CurDir);

	//Open the file
	CFileException exc;
	CFile fl;
	BOOL res = fl.Open(fileName, CFile::modeRead, &exc);
	if(res == FALSE)
	{
		SetCurrentDirectory(MainDir); 
		return FALSE;
	}

	//Delete the old database
	delete m_selectionEffectDB;
	m_selectionEffectDB = NULL;

	//Load the Selection Effects Database
	CArchive the_in_Archive(&fl, CArchive::load);
	the_in_Archive >> m_selectionEffectDB;
	the_in_Archive.Close();
	fl.Close();

	SetCurrentDirectory(MainDir); 
	return TRUE;
}
BOOL GlobalStatusVariables::LoadDotDB(CString fileName)
{
	//Use GameFiles folder for organization
	CString CurDir;
	CString MainDir;
	char buffer[_MAX_PATH];
	_getcwd( buffer, _MAX_PATH );
	MainDir = CurDir = buffer;
	CurDir = CurDir + "\\GameFiles\\";
	SetCurrentDirectory(CurDir);

	//Open the file
	CFileException exc;
	CFile fl;
	BOOL res = fl.Open(fileName, CFile::modeRead, &exc);
	if(res == FALSE)
	{
		SetCurrentDirectory(MainDir); 
		return FALSE;
	}

	//Delete the old database
	delete m_dotDB;
	m_dotDB = NULL;

	//Load the Selection Effects Database
	CArchive the_in_Archive(&fl, CArchive::load);
	the_in_Archive >> m_dotDB;
	the_in_Archive.Close();
	fl.Close();

	SetCurrentDirectory(MainDir); 
	return TRUE;
}
//----------------Activates a runtime movie-------------------------------------------//
void GlobalStatusVariables::RunRuntimeMovie(int indexLoc)
{
//The DirectDraw 7 media-stream video backend has been removed from the engine (it cannot work
//on modern Windows with the D3D9 device). Movie entries survive as pure data - fileName, looping
//and display rects - so world triggers or scripts that fire one degrade to an inert no-op:
//display rects are still recomputed and List_playing stays FALSE.
if(!MovieRList)
return;
POSITION posLoc = MovieRList->FindIndex(indexLoc);
if(!posLoc)
return;
      CMovieRuntimeObj* mPtr = (CMovieRuntimeObj*) MovieRList->GetAt(posLoc);
if(!mPtr)
return;
CalculateAllMovieDisplayRects();
}
//---------------------------load Hud-------------------------------------------------//
BOOL GlobalStatusVariables::LoadHudDatabase(CString fileName)
{
	return FALSE;
}
//---------------------Load Menu Database---------------------------------------------//
BOOL GlobalStatusVariables::LoadMenuSys(CString filename)
{
	SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization
	CString loadedMenuPath;
	char fullMenuPath[MAX_PATH] = { 0 };
	DWORD fullMenuPathLength = GetFullPathNameA(filename, MAX_PATH, fullMenuPath, NULL);
	if(fullMenuPathLength > 0 && fullMenuPathLength < MAX_PATH)
		loadedMenuPath = fullMenuPath;

	//Attempt to open the file
	CXMLFileIO xmlIn;
	if(!xmlIn.Open(filename.GetBuffer(), false))
	{
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	}

	m_axLogger.WriteDebugFile("LoadMenuSys: freeing previous menu system",0,1);
	//Empty the old font list
	m_menuFontList->Flush();

	//Empty the old icon list
	m_menuIconDatabase->ClearList();

	//Reset cursors
	m_showMenuCursor = 0;
	m_currentCursor	 = 0;
	for(int i = 0; i < CURSOR_MAX; i++)
	{
        delete m_mouseCursors[i];
		m_mouseCursors[i] = 0;

		m_mouseCursors[i]			= new CMenuImage();
		m_mouseCursors[i]->m_name	= "Mouse Cursor";
		m_mouseCursors[i]->m_width	= 10;
		m_mouseCursors[i]->m_height	= 10;
	}

	//Empty the old menu database
	m_runtimeMenuDatabase->RemoveAll();
	m_menuDatabase->ClearList();

	//Reset cursor count variable
	m_showMenuCursor = 0;
	m_axLogger.WriteDebugFile("LoadMenuSys: previous menus freed, parsing file",0,1);

	char buffer[_MAX_PATH];
	//Read in the version
	bool closing;
	xmlIn.ReadAttribute(buffer, _MAX_PATH); //Read in the version and trash it for now

	//Read in the Font List
	xmlIn.ReadTag(buffer, _MAX_PATH, closing); //Read in <FontList>
	m_menuFontList->Serialize(xmlIn);
	//The Closing Tag gets read by the Font List

	//Read in the Icon Database
	xmlIn.ReadTag(buffer, _MAX_PATH, closing); //Read in <IconDatabase>
	m_menuIconDatabase->Serialize(xmlIn);
	//The Closing Tag gets read by the Visual List

	//Read in the Mouse Cursor
	for(int i = 0; i < CURSOR_MAX; i++)
	{
		xmlIn.ReadTag(buffer, _MAX_PATH, closing); //Read in <MouseCursor>
		m_mouseCursors[i]->Serialize(xmlIn);
		//The Closing Tag gets read by the Image
	}

	//Read in the Menu Database
	xmlIn.ReadTag(buffer, _MAX_PATH, closing); //Read in <MenuDatabase>
	m_menuDatabase->Serialize(xmlIn);
	//The Closing Tag gets read by the Visual List

	//Close the file
	xmlIn.Close();
	m_axLogger.WriteDebugFile("LoadMenuSys: file parsed",0,1);

	//Reset the current directory
	SetCurrentDirectory(GL_workingDirectory);

	//Load textures for icon db
	POSITION posCurIcon = m_menuIconDatabase->GetHeadPosition();
	CString lastIconTexture;
	int lastIconChannel;

	while(posCurIcon != NULL)
	{
		CMenuImage *visual = (CMenuImage*)m_menuIconDatabase->GetNext(posCurIcon);
		if(visual)
		{
			if(lastIconTexture.Compare(visual->m_textureFile) == 0)
			{
				visual->m_textureSelect = lastIconChannel;
				continue;
			}

			visual->m_textureSelect = lastIconChannel = m_textureDataBase->AddAssignTempTexture(m_3dDevice, visual->m_textureFile, 0);
			lastIconTexture = visual->m_textureFile;
		}
	}
	SetCurrentDirectory(GL_workingDirectory);
	m_loadedMenuSystemPath = loadedMenuPath;
	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadParticleDatabase(CString fileName)
{
if(!m_particleDatabase)
	 return FALSE;

     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization
     //end use gamefiles folder for organization 	 
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res)
	 {
		 DestroyAllParticleInterfaces();
		 DeleteParticleSystem();
		 CArchive the_in_Archive(&fl,CArchive::load);
		 the_in_Archive >> m_particleDatabase;
		 the_in_Archive.Close();
		 fl.Close();
	 }
     SetCurrentDirectory(GL_workingDirectory);
     
	 return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadTextureBank(CString fileName)
{
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization

	 CFileException exc;
     CFile fl;
	 
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	 }
	 DestroyAllTexturesFromDatabase();
	 //m_textureDataBase->RemoveAllTempTextures();
	 if(m_dynamicTextureDataBase)
	 {
		 m_dynamicTextureDataBase->SafeDelete();
		 delete m_dynamicTextureDataBase;
		 m_dynamicTextureDataBase = 0;
	 }

	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_dynamicTextureDataBase;
	 the_in_Archive.Close();
     fl.Close();

	 if(m_textureDataBase)
	   m_textureDataBase->InitReferences(GL_pathMapModels,
		                                 m_3dDevice,
									     m_dxRenderer->m_maxTextureSizeSquared,
									     m_editorModeOn,
									     m_textureSearchPath,
									     m_searchPathCount,
									     m_compressTextures,
										 this->m_advUrlBasic,
										 this->m_advUrlVending);

	 if(m_dynamicTextureDataBase)
	   m_dynamicTextureDataBase->InitReferences(GL_pathMapModels,
		                               m_3dDevice,
									   m_dxRenderer->m_maxTextureSizeSquared,
									   m_editorModeOn,
									   m_textureSearchPath,
									   m_searchPathCount,
									   m_compressTextures,
									   this->m_advUrlBasic,
									   this->m_advUrlVending);
	 
	 ReinitAllTexturesFromDatabase();
	 if(m_editorModeOn==FALSE)
	    m_dynamicTextureDataBase->CacheRawData();
	 SetCurrentDirectory(GL_workingDirectory);
	 return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadSoundTrackDB(CString fileName)
{
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization
	 
     CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		  SetCurrentDirectory(GL_workingDirectory);
	      return FALSE;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);
	 if(m_soundTrackList){
	    m_soundTrackList->SafeDelete();
	    delete m_soundTrackList;
	    m_soundTrackList=0;
	 }
	 the_in_Archive >> m_soundTrackList;
	 the_in_Archive.Close();
     fl.Close();
	 SetCurrentDirectory(GL_workingDirectory);
	 m_soundTrackList->Initialize(m_dxRenderer->m_hwnd,m_dirSnd); 
	 
	 return TRUE;
}
//-----------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadAiData(CString fileName)
{
	 
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization

     CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		 SetCurrentDirectory(GL_workingDirectory);
		 return FALSE;
	 }
	 if(m_aiList)
	 {
	   m_aiList->SafeDelete();
	   delete m_aiList;
	   m_aiList=0;
	 }
	 if(m_scrList)
	 {
	   m_scrList->SafeDelete();
	   delete m_scrList;
	   m_scrList=0;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_aiList >> m_scrList;
	 the_in_Archive.Close();
     fl.Close();
     SetCurrentDirectory(GL_workingDirectory);
	 m_aiList->ResetIndexReferences();
	 
	 return TRUE;
}

//-----------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadGlobalCommerceData(CString fileName)
{
	 
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization
     CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		 SetCurrentDirectory(GL_workingDirectory);
		 return FALSE;
	 }
	 m_commerceDB->SafeDelete();
	 delete m_commerceDB;
	 m_commerceDB = 0;
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_commerceDB;
	 the_in_Archive.Close();
     fl.Close();
     SetCurrentDirectory(GL_workingDirectory);
	 return TRUE;
}
//-----------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadBankZonesData(CString fileName)
{
	 
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization
     //end use gamefiles folder for organization
     CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		 SetCurrentDirectory(GL_workingDirectory);
		 return FALSE;
	 }
	 m_bankZoneList->SafeDelete();
	 delete m_bankZoneList;
	 m_bankZoneList = 0;
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_bankZoneList;
	 the_in_Archive.Close();
     fl.Close();
     SetCurrentDirectory(GL_workingDirectory);
	 return TRUE;
}
//-----------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadSafeZonesData(CString fileName)
{
	 
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization

     CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		 SetCurrentDirectory(GL_workingDirectory);
		 return FALSE;
	 }
	 m_safeZoneDB->SafeDelete();
	 delete m_safeZoneDB;
	 m_safeZoneDB = 0;
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_safeZoneDB;
	 the_in_Archive.Close();
     fl.Close();
     SetCurrentDirectory(GL_workingDirectory);
	 return TRUE;
}

//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadStaticTextureBank(CString fileName)
{
     if(!m_3dDevice)
		return FALSE;

     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization

	 CFileException exc;
     CFile fl;
	 
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	 }
	 
	 if(m_staticTextureDataBase){
	    //m_staticTextureDataBase->InvalidateAllTextures();
	    m_staticTextureDataBase->SafeDelete();
	    delete m_staticTextureDataBase;
	    m_staticTextureDataBase=0;
	 }
	 
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_staticTextureDataBase;
	 the_in_Archive.Close();
     fl.Close();

	 //m_staticTextureDataBase->RestoreAllTextures(m_3dDevice,m_maxTextureSizeSquared,m_renderStateObject->m_pCaps);
     if(m_staticTextureDataBase)
	   m_staticTextureDataBase->InitReferences(GL_pathMapModels,
		                               m_3dDevice,
									   m_dxRenderer->m_maxTextureSizeSquared,
									   m_editorModeOn,
									   m_textureSearchPath,
									   m_searchPathCount,
									   m_compressTextures,
									   this->m_advUrlBasic,
									   this->m_advUrlVending);

	 m_staticTextureDataBase->RestoreAllTextures(m_3dDevice,m_dxRenderer->m_maxTextureSizeSquared,m_renderStateObject->m_pCaps);
	 if(m_editorModeOn==FALSE)
	    m_staticTextureDataBase->CacheRawData();

	 SetCurrentDirectory(GL_workingDirectory);
	 return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadWaterZoneDB(CString fileName)
{
	
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization

	 CFileException exc;
     CFile fl;
	 
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	 }
	 
	 if(m_waterZoneDB){
	    m_waterZoneDB->SafeDelete();
	    delete m_waterZoneDB;
	    m_waterZoneDB=0;
	 }
	 
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_waterZoneDB;
	 the_in_Archive.Close();
     fl.Close();

	 SetCurrentDirectory(GL_workingDirectory);
	 return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadPortalDatabase(CString fileName)
{
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization
	 CFileException exc;
     CFile fl;
	 
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	 }
	 if(m_portalDatabase){
		m_portalDatabase->SafeDelete();
	    delete m_portalDatabase;
		m_portalDatabase = 0;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_portalDatabase;
	 the_in_Archive.Close();
     fl.Close();

	 SetCurrentDirectory(GL_workingDirectory);
	 return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadElementDatabase(CString fileName)
{
	
    SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization

	 CFileException exc;
     CFile fl;
	 
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	 }
	 if(m_elementDB){
		m_elementDB->SafeDelete();
	    delete m_elementDB;
		m_elementDB = 0;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_elementDB;
	 the_in_Archive.Close();
     fl.Close();

	 SetCurrentDirectory(GL_workingDirectory);
	 return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadStatsDatabase(CString fileName)
{
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization

	 CFileException exc;
     CFile fl;
	 
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	 }
	 if(m_statDatabase){
		m_statDatabase->SafeDelete();
	    delete m_statDatabase;
		m_statDatabase = 0;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_statDatabase;
	 the_in_Archive.Close();
     fl.Close();

	 
	 SetCurrentDirectory(GL_workingDirectory);
	 return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadSkillsDatabase(CString fileName)
{
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization
	 CFileException exc;
     CFile fl;
	 
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	 }
	 if(m_skillDatabase){
		m_skillDatabase->SafeDelete();
	    delete m_skillDatabase;
		m_skillDatabase = 0;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_skillDatabase;
	 the_in_Archive.Close();
     fl.Close();
 
	 SetCurrentDirectory(GL_workingDirectory);
	 return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadAICollisionDB(CString fileName)
{    
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization	 
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);

	 m_collisionDatabase->SafeDelete();
	 delete m_collisionDatabase;
	 m_collisionDatabase = 0;
	 the_in_Archive >> m_collisionDatabase;
	 the_in_Archive.Close();
     fl.Close();
     SetCurrentDirectory(GL_workingDirectory);
	
  return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadIconDB(CString fileName)
{
    
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization
	 
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);

	 m_iconDatabase->SafeDelete();
	 delete m_iconDatabase;
	 m_iconDatabase = 0;
	 the_in_Archive >> m_iconDatabase;
	 the_in_Archive.Close();
     fl.Close();

     m_iconDatabase->Initialize(m_d3dVectorInit,m_3dDevice);

     SetCurrentDirectory(GL_workingDirectory);
 
  return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadHouseZoneDB(CString fileName)
{   
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization
 
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);

	 if(m_multiplayerObject->m_houseingDB)
	 {
	   m_multiplayerObject->m_houseingDB->SafeDelete();
	   delete m_multiplayerObject->m_houseingDB;
	   m_multiplayerObject->m_houseingDB = 0;
	 }
	 the_in_Archive >> m_multiplayerObject->m_houseingDB;
	 the_in_Archive.Close();
     fl.Close();

	 
     SetCurrentDirectory(GL_workingDirectory);
 
  return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadHouseDB(CString fileName)
{
    
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization
    
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);
     if(m_housingDB)
	 {
	   m_housingDB->SafeDelete();
	   delete m_housingDB;
	   m_housingDB = 0;
	 }
	 the_in_Archive >> m_housingDB;
	 the_in_Archive.Close();
     fl.Close();

	 m_housingDB->ReinitAll(m_3dDevice);
     SetCurrentDirectory(GL_workingDirectory);
 
  return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadAIRaidDB(CString fileName)
{
    
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization
	 
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);
     if(m_aiRaidDatabase)
	 {
	   m_aiRaidDatabase->SafeDelete();
	   delete m_aiRaidDatabase;
	   m_aiRaidDatabase = 0;
	 }
	 the_in_Archive >> m_aiRaidDatabase;
	 the_in_Archive.Close();
     fl.Close();

     SetCurrentDirectory(GL_workingDirectory);
 
  return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadCommControls(CString fileName)
{
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization	 
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);
     if(m_commCtrlDB)
	 {
	   m_commCtrlDB->SafeDelete();
	   delete m_commCtrlDB;
	   m_commCtrlDB = 0;
	 }
	 the_in_Archive >> m_commCtrlDB;
	 m_commCtrlDB->ReinitAll(m_3dDevice);
	 the_in_Archive.Close();
     fl.Close();

     SetCurrentDirectory(GL_workingDirectory);
 
  return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadArenaSys(CString fileName)
{ 
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization
	 
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);
     if(m_battleSchedulerSys)
	 {
	   m_battleSchedulerSys->SafeDelete();
	   delete m_battleSchedulerSys;
	   m_battleSchedulerSys = 0;
	 }
	 the_in_Archive >> m_battleSchedulerSys;
	 m_battleSchedulerSys->Initialize();
	 the_in_Archive.Close();
     fl.Close();

     SetCurrentDirectory(GL_workingDirectory);
 
  return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadWorldRules(CString fileName)
{
     //obsolete
 
  return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadShaderLibrary(CString fileName)
{
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization
	 
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);
     if(m_shaderSystem)
	 {
	   m_shaderSystem->SafeDelete();
	   delete m_shaderSystem;
	   m_shaderSystem = 0;
	 }
	 the_in_Archive >> m_shaderSystem;
	 the_in_Archive.Close();
     fl.Close();
	 //if(m_dxRenderer->m_shaderSupport)
        
	 m_shaderSystem->Compile(m_3dDevice);
	 m_renderStateObject->m_shderSystemRef = m_shaderSystem;

     SetCurrentDirectory(GL_workingDirectory);
 
  return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadGlobalShaderLibrary(CString fileName)
{
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization	 
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);
     if(m_cgGlobalShaderDB)
	 {
	   m_cgGlobalShaderDB->SafeDelete();
	   delete m_cgGlobalShaderDB;
	   m_cgGlobalShaderDB = 0;
	 }
	 the_in_Archive >> m_cgGlobalShaderDB;
	 the_in_Archive.Close();
     fl.Close();
	 m_cgGlobalShaderDB->ScaleForRuntimeCompatibility(m_renderStateObject->m_supportBUMPENVMAP,m_renderStateObject->m_maxPixShaderVs,m_renderStateObject->m_maxVSShaderVs);
     SetCurrentDirectory(GL_workingDirectory);
 
  return TRUE;
}
//------loads scene-------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadPostProcessorDB(CString fileName)
{
	 SetCurrentDirectory(GL_pathGameFiles);
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	 }
	 //destroy list that will be filled
	 if(m_postProcessor)
	 {
		delete m_postProcessor;
		m_postProcessor = 0;
	 }
	 //end destroy list that will be filled
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_postProcessor;
	 the_in_Archive.Close();
     fl.Close();
     m_postProcessor->SetEffect(m_globalRenderer);
     SetCurrentDirectory(GL_workingDirectory); 
  return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		GlobalStatusVariables
// Function:	ShowMessageBox(CString, CString)
// Description: Shows a message box to the player with the specified title and
//	message strings.
///////////////////////////////////////////////////////////////////////////////

void
GlobalStatusVariables::ShowMessageBox(CString strTitle, CString strMessage)
{
	// OPEN the message box menu
	this->OpenMenu(ID_MESSAGE_BOX_MENU, false);

	// GET references to menu visuals
	CMenuText *pTitle = dynamic_cast<CMenuText*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_MESSAGE_BOX_TITLE_TEXT, true));
	CMenuText *pMessage = dynamic_cast<CMenuText*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_MESSAGE_BOX_TEXT, true));
	if (!pTitle || !pMessage) return;

	// SET the title text and message
	pTitle->m_text = strTitle;
	pMessage->m_text = strMessage;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		GlobalStatusVariables
// Function:	GetActiveInventory(MENU_IDS)
// Description:	Returns the active inventory control of the specified menu window.
//	If one isn't found, returns NULL.
///////////////////////////////////////////////////////////////////////////////

CMenuInventory*
GlobalStatusVariables::GetActiveInventory(MENU_IDS eMenuID)
{
	// GET a reference to the requested menu window
	CMenuWindow *pMenu = dynamic_cast<CMenuWindow*>(this->m_runtimeMenuDatabase->GetVisualByID(eMenuID, true));
	if (!pMenu) return NULL;

	// SEARCH all the menu's children for the first active inventory control
	for (POSITION pos = pMenu->m_childList->GetHeadPosition(); pos != NULL;)
	{
		CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(pMenu->m_childList->GetNext(pos));
		if (!pInventory || !pInventory->m_bActive) continue;
		return pInventory;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		GlobalStatusVariables
// Function:	SetActiveInventory(MENU_IDS)
// Description:	Set the specified inventory control as active. If it should
//	be exclusively active, de-activate all other inventory controls in the
//	current menu.
///////////////////////////////////////////////////////////////////////////////

void
GlobalStatusVariables::SetActiveInventory(MENU_IDS eMenuID, 
										  MENU_IDS eInventoryID, 
										  bool bIsExclusivelyActive)
{
	// GET a reference to the requested menu visuals
	CMenuWindow *pMenu = dynamic_cast<CMenuWindow*>(this->m_runtimeMenuDatabase->GetVisualByID(eMenuID, true));
	CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(this->m_runtimeMenuDatabase->GetVisualByID(eInventoryID, true));
	if (!pMenu || !pInventory) return;

	// SEARCH all the menu's children for the first active inventory control
	for (POSITION pos = pMenu->m_childList->GetHeadPosition(); pos != NULL;)
	{
		CMenuInventory *pCompare = dynamic_cast<CMenuInventory*>(pMenu->m_childList->GetNext(pos));
		if (!pCompare) continue;
		if (pCompare == pInventory)
		{
			pInventory->m_bActive = true;
			pInventory->m_visible = true;
		}
		else if (bIsExclusivelyActive)
		{
			pCompare->m_bActive = false;
			pCompare->m_visible = false;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class: GlobalStatusVariables
// Function: OnInventoryItemSelected(MENU_IDS)
// Description: Handles all the visual chores related to selecting an item
//	in an inventory menu visual.
///////////////////////////////////////////////////////////////////////////////

void
GlobalStatusVariables::OnInventoryItemSelected(MENU_IDS eMenuID)
{
	// GET a reference to the inventory menu
	CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(this->m_runtimeMenuDatabase->GetVisualByID(eMenuID, true));
	if (!pInventory) return;
	CMenuInventoryItem *pHoverItem = pInventory->HoverItem();

	switch (eMenuID)
	{
		case ID_BANK_INVENTORY_01:
		case ID_BANK_INVENTORY_02:
		case ID_BANK_INVENTORY_03:
		case ID_BANK_INVENTORY_04:
		{
			// GET references to menu visuals
			CMenuImage *pSelectionIndicator = dynamic_cast<CMenuImage*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_BANK_INVENTORY_SELECTION_IMAGE, true));
			CMenuButton *pWithdrawButton = dynamic_cast<CMenuButton*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_BANK_WITHDRAW_BUTTON, true));
			
			// ERROR checking
			if (!pSelectionIndicator || !pHoverItem)
			{
				// HIDE the selection indicator
				if (pSelectionIndicator)
					pSelectionIndicator->m_visible = false;

				// DISABLE the bank deposit button
				if (pWithdrawButton)
					pWithdrawButton->SetEnabled(false);
				break;
			}

			// TODO: Find a better place to put this logic!
			// SET the position and visibility of the selection indicator 
			//	to that of the selected item
			pSelectionIndicator->m_visible = true;
			int iOffsetX = (pInventory->HoverItem()->m_pIcon->m_width - pSelectionIndicator->m_width) / 2.0f;
			int iOffsetY = (pInventory->HoverItem()->m_pIcon->m_height - pSelectionIndicator->m_height) / 2.0f;
			pSelectionIndicator->m_posX = pInventory->HoverItem()->m_pIcon->m_posX + iOffsetX;
			pSelectionIndicator->m_posY = pInventory->HoverItem()->m_pIcon->m_posY + iOffsetY;

			// ENABLE the bank deposit button
			if (pWithdrawButton)
				pWithdrawButton->SetEnabled();
			break;
		}
		case ID_PLAYER_INVENTORY:
		{
			// GET references to menu visuals
			// PLAYER INVENTORY
			CMenuImage *pSelectionIndicator = dynamic_cast<CMenuImage*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY_SELECTION_IMAGE, true));
			CMenuButton *pEquipButton = dynamic_cast<CMenuButton*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY_EQUIP_BUTTON, true));
			CMenuButton *pUnequipButton = dynamic_cast<CMenuButton*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY_UNEQUIP_BUTTON, true));
			CMenuButton *pDiscardButton = dynamic_cast<CMenuButton*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY_DISCARD_BUTTON, true));
			CMenuButton *pAddToQuickbarButton = dynamic_cast<CMenuButton*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_ADD_TO_QUICKBAR_BUTTON, true));
			
			// BANK
			CMenuWindow *pBankMenu = dynamic_cast<CMenuWindow*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_BANK_INVENTORY_MENU, true));
			CMenuButton *pDepositButton = dynamic_cast<CMenuButton*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_BANK_DEPOSIT_BUTTON, true));
			CMenuButton *pWithdrawButton = dynamic_cast<CMenuButton*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_BANK_WITHDRAW_BUTTON, true));
			
			// STORE
			CMenuWindow *pStoreMenu = dynamic_cast<CMenuWindow*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_STORE_MENU, true));
			CMenuButton *pSellButton = dynamic_cast<CMenuButton*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_STORE_SELL_BUTTON, true));

			// TRADE
			CMenuWindow *pTradeMenu = dynamic_cast<CMenuWindow*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_MENU, true));
			CMenuButton *pAddButton = dynamic_cast<CMenuButton*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_ADD_BUTTON, true));
			
			// ERROR checking: Check validity of menu visual references
			if (!pSelectionIndicator || !pHoverItem)
			{
				// HIDE the selection indicator
				if (pSelectionIndicator)
					pSelectionIndicator->m_visible = false;

				// DISABLE buttons
				if (pEquipButton) pEquipButton->SetEnabled(false);
				if (pUnequipButton) pUnequipButton->SetEnabled(false);
				if (pDiscardButton) pDiscardButton->SetEnabled(false);
				if (pAddToQuickbarButton) pAddToQuickbarButton->SetEnabled(false);
				if (pDepositButton) pDepositButton->SetEnabled(false);
				if (pSellButton) pSellButton->SetEnabled(false);
				if (pAddButton) pAddButton->SetEnabled(false);
				break;
			}

			// TODO: Find a better place to put this logic!
			// SET the position and visibility of the selection indicator 
			//	to that of the selected item
			pSelectionIndicator->m_visible = true;
			int iOffsetX = (pInventory->HoverItem()->m_pIcon->m_width - pSelectionIndicator->m_width) / 2.0f;
			int iOffsetY = (pInventory->HoverItem()->m_pIcon->m_height - pSelectionIndicator->m_height) / 2.0f;
			pSelectionIndicator->m_posX = pInventory->HoverItem()->m_pIcon->m_posX + iOffsetX;
			pSelectionIndicator->m_posY = pInventory->HoverItem()->m_pIcon->m_posY + iOffsetY;

			// SET the enabled state of the inventory menu buttons
			if (pBankMenu || pStoreMenu || pTradeMenu)
			{
				// DISABLE all player inventory menu buttons
				if (pUnequipButton) pUnequipButton->SetEnabled(false);
				if (pEquipButton) pEquipButton->SetEnabled(false);
				if (pAddToQuickbarButton) pAddToQuickbarButton->SetEnabled(false);
				if (pDiscardButton) pDiscardButton->SetEnabled(false);
				if (pAddToQuickbarButton) pAddToQuickbarButton->SetEnabled(false);
			}
			else
			{
				///////////////////////////////////////////////////////////////////
				// PLAYER INVENTORY
				///////////////////////////////////////////////////////////////////

				// SET button enabled states
				if (pUnequipButton)
				{
					if (pHoverItem->m_data.m_armed) pUnequipButton->SetEnabled();
					else pUnequipButton->SetEnabled(false);
				}

				if (pEquipButton)
				{
					if (!pHoverItem->m_data.m_armed) pEquipButton->SetEnabled();
					else pEquipButton->SetEnabled(false);
				}

				if (pDiscardButton)
					pDiscardButton->SetEnabled();

				if (pAddToQuickbarButton && !this->m_multiplayerObject->IsQuickbarFull())
					pAddToQuickbarButton->SetEnabled();
			}

			///////////////////////////////////////////////////////////////////
			// BANK
			///////////////////////////////////////////////////////////////////

			// SET button enabled states
			if (pDepositButton) pDepositButton->SetEnabled();

			///////////////////////////////////////////////////////////////////
			// STORE
			///////////////////////////////////////////////////////////////////

			// SET button enabled states
			if (pSellButton) pSellButton->SetEnabled();

			///////////////////////////////////////////////////////////////////
			// TRADE
			///////////////////////////////////////////////////////////////////

			// SET button enabled states
			if (pAddButton) pAddButton->SetEnabled();
			break;
		}
		case ID_STORE_INVENTORY:
		{
			// GET references to menu visuals
			CMenuImage *pSelectionIndicator = dynamic_cast<CMenuImage*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_STORE_INVENTORY_SELECTION_IMAGE, true));
			CMenuButton *pBuyButton = dynamic_cast<CMenuButton*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_STORE_BUY_BUTTON, true));
			
			// ERROR checking: Check validity of menu visual references
			if (!pSelectionIndicator || !pHoverItem)
			{
				// HIDE the selection indicator
				if (pSelectionIndicator)
					pSelectionIndicator->m_visible = false;

				// DISABLE buttons
				if (pBuyButton)
					pBuyButton->SetEnabled(false);
				break;
			}

			// TODO: Find a better place to put this logic!
			// SET the position and visibility of the selection indicator 
			//	to that of the selected item
			pSelectionIndicator->m_visible = true;
			int iOffsetX = (pInventory->HoverItem()->m_pIcon->m_width - pSelectionIndicator->m_width) / 2.0f;
			int iOffsetY = (pInventory->HoverItem()->m_pIcon->m_height - pSelectionIndicator->m_height) / 2.0f;
			pSelectionIndicator->m_posX = pInventory->HoverItem()->m_pIcon->m_posX + iOffsetX;
			pSelectionIndicator->m_posY = pInventory->HoverItem()->m_pIcon->m_posY + iOffsetY;

			// ENABLE buttons
			if (pBuyButton)
				pBuyButton->SetEnabled();
			break;
		}
		case ID_TRADE_YOUR_INVENTORY:
		{
			// GET references to menu visuals
			CMenuImage *pSelectionIndicator = dynamic_cast<CMenuImage*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_INVENTORY_SELECTION_IMAGE, true));
			CMenuButton *pRemoveButton = dynamic_cast<CMenuButton*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_TRADE_REMOVE_BUTTON, true));
			
			// ERROR checking: Check validity of menu visual references
			if (!pSelectionIndicator || !pHoverItem)
			{
				// HIDE the selection indicator
				if (pSelectionIndicator) pSelectionIndicator->m_visible = false;

				// DISABLE buttons
				if (pRemoveButton) pRemoveButton->SetEnabled(false);
				break;
			}

			// ENABLE buttons
			if (pInventory->HoverItem())
				if (pRemoveButton) pRemoveButton->SetEnabled();

			// TODO: Find a better place to put this logic!
			// SET the position and visibility of the selection indicator 
			//	to that of the selected item
			pSelectionIndicator->m_visible = true;
			int iOffsetX = (pInventory->HoverItem()->m_pIcon->m_width - pSelectionIndicator->m_width) / 2.0f;
			int iOffsetY = (pInventory->HoverItem()->m_pIcon->m_height - pSelectionIndicator->m_height) / 2.0f;
			pSelectionIndicator->m_posX = pInventory->HoverItem()->m_pIcon->m_posX + iOffsetX;
			pSelectionIndicator->m_posY = pInventory->HoverItem()->m_pIcon->m_posY + iOffsetY;
			break;
		}
		case ID_EXAMINE_INVENTORY:
		{
			// GET references to menu visuals
			CMenuImage *pSelectionIndicator = dynamic_cast<CMenuImage*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_EXAMINE_INVENTORY_SELECTION_IMAGE, true));
			CMenuButton *pLootButton = dynamic_cast<CMenuButton*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_EXAMINE_LOOT_BUTTON, true));
			CMenuButton *pLootAllButton = dynamic_cast<CMenuButton*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_EXAMINE_LOOT_ALL_BUTTON, true));
			
			// ERROR checking: Check validity of menu visual references
			if (!pSelectionIndicator || !pHoverItem)
			{
				// HIDE the selection indicator
				if (pSelectionIndicator) pSelectionIndicator->m_visible = false;

				// DISABLE buttons
				if (pLootButton) pLootButton->SetEnabled(false);
				if (pLootAllButton) pLootAllButton->SetEnabled(false);
				break;
			}

			// ENABLE buttons
			if (pInventory->HoverItem())
			{
				if (pLootButton) pLootButton->SetEnabled();
				if (pLootAllButton) pLootAllButton->SetEnabled();
			}

			// TODO: Find a better place to put this logic!
			// SET the position and visibility of the selection indicator 
			//	to that of the selected item
			pSelectionIndicator->m_visible = true;
			int iOffsetX = (pInventory->HoverItem()->m_pIcon->m_width - pSelectionIndicator->m_width) / 2.0f;
			int iOffsetY = (pInventory->HoverItem()->m_pIcon->m_height - pSelectionIndicator->m_height) / 2.0f;
			pSelectionIndicator->m_posX = pInventory->HoverItem()->m_pIcon->m_posX + iOffsetX;
			pSelectionIndicator->m_posY = pInventory->HoverItem()->m_pIcon->m_posY + iOffsetY;
			break;
		}
		default: break;
	};
}

///////////////////////////////////////////////////////////////////////////////
// Class: GlobalStatusVariables
// Function: LoadQuickbarConfig(ITEMLIST*)
// Description: Loads Quick Items from a pre-existing configuration file
//	saved onto disk. When creating the Quick Item menu, note that this function
//	assumes all slot visual IDs are consecutive from the first. Additionally,
//	checks are made against the character's inventory to make sure all loaded
//	items are valid and extant.
///////////////////////////////////////////////////////////////////////////////

BOOL
GlobalStatusVariables::LoadQuickbarConfig(const ITEMLIST *pInventory)
{
	bool bFinished = false;
	if(this->m_multiplayerObject->m_bIsQuickBarInitialized) return FALSE;
	this->m_multiplayerObject->m_bIsQuickBarInitialized = true;

	// USE GameFiles folder for organization
	SetCurrentDirectory(GL_pathGameFiles);

	// INIT all slots to empty status
	for(int i = 0; i < NUM_QUICKBARS; i++)
	{
		for(int j = 0; j < NUM_QUICKBAR_SLOTS; j++)
		{
			CMenuInventoryItem **pItem = this->m_multiplayerObject->Quickbar(i, j);
			SafeDelete(*pItem);
		}
	}

	// LOAD the movement object
	POSITION pos = m_entObjRuntime->FindIndex(0);
	if(!pos) return FALSE;
	CMovementObj *pMovement = (CMovementObj*)this->m_entObjRuntime->GetAt(pos);

	// CREATE filename for writing
	CString strFileName = QUICK_BAR_CONFIG_FILE;
	strFileName += ".";
	strFileName += this->m_cabLib.ConvToString(pMovement->m_uniqueId);

	// CREATE a new XML object for reading
	CXMLFileIO xmlIn;

	// OPEN the file for reading
	if(!xmlIn.Open(strFileName.GetBuffer(), false))
	{
		// RESET to base working directory
		SetCurrentDirectory(GL_workingDirectory);
		strFileName.ReleaseBuffer();
		return FALSE;
	}
	strFileName.ReleaseBuffer();

	// Temporary variables
	char tmpBuffer[MAX_BUFFER];
	bool closing;
	int version;

	// READ in the version number
	xmlIn.ReadAttribute(version);

	// LOOP through all quick bars
	for(int i = 0; i < NUM_QUICKBARS; i++)
	{
		// READ in the opening bar tag, <Quickbar>
		xmlIn.ReadTag(tmpBuffer, MAX_BUFFER, closing);

		// IF we're closing, it's time to break, because we just read in </Quickbar>
		if (closing) 
		{
			bFinished = true;
			break;
		}

		// READ in all slots on this bar
		for(int j = 0; j < NUM_QUICKBAR_SLOTS; j++)
		{
			// READ in the opening slot tag, <Slot>
			xmlIn.ReadTag(tmpBuffer, MAX_BUFFER, closing);
			
			// IF we're closing, it's time to break, because we just read in </Quickbar>
			if (closing) 
			{
				bFinished = true;
				break;
			}

			// READ in the inventory item
			CMenuInventoryItem *pFromFileItem = new CMenuInventoryItem();
			int iValue = 0;
			xmlIn.ReadAttribute(iValue); // <GLID>
			pFromFileItem->m_data.m_GLID = iValue;
			xmlIn.ReadAttribute(iValue); // <Armed>
			pFromFileItem->m_data.m_armed = iValue; 
			xmlIn.ReadAttribute(iValue); // <Quantity>
			pFromFileItem->m_data.m_quantity = iValue; 
			xmlIn.ReadAttribute(iValue); // <Mod>
			pFromFileItem->m_data.m_dMod = iValue; 

			// GET the inventory item based off the GLID
			bool bIsInInventory = false;
			CItemObj *pItem = this->m_itemManager->GetItemByID(pFromFileItem->m_data.m_GLID);
			if(pItem)
			{
				// VERIFY that the character actually possesses this item in their inventory
				CMenuInventoryItem *pInventoryItem = NULL;
				ITEMLIST::const_iterator iter;
				for (iter = pInventory->begin(); iter != pInventory->end(); iter++)
				{
					pInventoryItem = *iter; //pInventory->at(i);
					CItemObj *pCompareItem = this->m_itemManager->GetItemByID(pInventoryItem->m_data.m_GLID);
					if(pItem == pCompareItem) 
					{
						pFromFileItem->m_data.m_quantity = pItem->m_chargeCurrent;
						pFromFileItem->m_data.m_armed = pItem->m_equiped;
						pFromFileItem->m_data.m_dMod = pItem->m_damageModifier;
						bIsInInventory = true;
						break;
					}
				}

				// GET references to menu visuals
				CMenuImage *pQuickItemImage = dynamic_cast<CMenuImage*>(this->m_menuDatabase->GetVisualByID(QUICKBAR_SLOT_IDS[1][j], true));
				CMenuImage *pItemIcon = dynamic_cast<CMenuImage*>(this->m_menuIconDatabase->GetVisualByID(pItem->m_visualRef, true));

				// IF we didn't find an icon for this item...
				if (!pItemIcon || !bIsInInventory)
				{
					// SET a default blank icon 
					pQuickItemImage->m_texturePosX = 0.0f; 
					pQuickItemImage->m_texturePosY = 0.0f; 
					pQuickItemImage->m_textureWidth = 32.0f; 
					pQuickItemImage->m_textureHeight = 32.0f; 
//					continue;
				}
				// ELSE, VERIFY that we have a valid item and slot
				else //if (pQuickItemImage && pItemIcon)
				{
					// UPDATE the image coordinates
					pQuickItemImage->m_texturePosX = pItemIcon->m_texturePosX;
					pQuickItemImage->m_texturePosY = pItemIcon->m_texturePosY;
					pQuickItemImage->m_textureWidth = pItemIcon->m_textureWidth;
					pQuickItemImage->m_textureHeight = pItemIcon->m_textureHeight;
					pQuickItemImage->m_fullTextureWidth	= pItemIcon->m_fullTextureWidth;
					pQuickItemImage->m_fullTextureHeight = pItemIcon->m_fullTextureHeight;
					pQuickItemImage->m_textureFile = pItemIcon->m_textureFile;
					pQuickItemImage->m_textureSelect = pItemIcon->m_textureSelect;	
				}

				// COPY the icon image into the inventory item to be stored in this slot
				pQuickItemImage->Clone((CMenuVisual**)&pFromFileItem->m_pIcon, this->m_3dDevice);

				// STORE the inventory item into the appropriate Quickbar slot
				this->AddToQuickbar(pInventoryItem);
			}

			// READ in the closing slot tag, </Slot>
			xmlIn.ReadTag(tmpBuffer, MAX_BUFFER, closing);
		}

		// IF we're finished, no need to read in the Quickbar closing tag, </Quickbar>
		if (bFinished) break;

		// READ in the closing Quickbar tag, </Quickbar>
		xmlIn.ReadTag(tmpBuffer, MAX_BUFFER, closing);
	}

	// CLOSE the file
	xmlIn.Close();

	// UPDATE Quickbar for this player on the server
	this->UpdateQuickbarServerSide();
	return TRUE;
}

void 
GlobalStatusVariables::SaveQuickbarConfig()
{
	// USE GameFiles folder for organization
	SetCurrentDirectory(this->GL_pathGameFiles);

	// CREATE a new XML object for writing
	CXMLFileIO xmlOut;

	// LOAD the movement object
	POSITION pos = this->m_entObjRuntime->FindIndex(0);
	if(!pos) return;
	CMovementObj *pMovement = (CMovementObj*)this->m_entObjRuntime->GetAt(pos);

	// CREATE filename for writing
	CString strFileName = QUICK_BAR_CONFIG_FILE;
	strFileName += ".";
	strFileName += this->m_cabLib.ConvToString(pMovement->m_uniqueId);

	// OPEN the file for writing
	if(!xmlOut.Open(strFileName.GetBuffer(), true))
	{
		SetCurrentDirectory(this->GL_workingDirectory);
		strFileName.ReleaseBuffer();
		return;
	}
	strFileName.ReleaseBuffer();

	// WRITE out the version number
	xmlOut.WriteAttribute("Version", 1);

	// LOOP through all quick bars
	for(int i = 0; i < NUM_QUICKBARS; i++)
	{
		// WRITE out the opening bar tag
		xmlOut.WriteOpeningTag("Quickbar");

		// WRITE out all slots on this bar
		for(int j = 0; j < NUM_QUICKBAR_SLOTS; j++)
		{
			// GET the item in the current slot
			CMenuInventoryItem *pItem = *(this->m_multiplayerObject->Quickbar(i, j));
			if (!pItem) continue;

			// WRITE out the item properties
			xmlOut.WriteOpeningTag("Slot");
			xmlOut.WriteAttribute("GLID", pItem->m_data.m_GLID);
			xmlOut.WriteAttribute("Quantity", pItem->m_data.m_quantity);
			xmlOut.WriteAttribute("Armed", pItem->m_data.m_armed);
			xmlOut.WriteAttribute("Mod", pItem->m_data.m_dMod);
			xmlOut.WriteClosingTag("Slot");
		}

		// WRITE the closing bar tag
		xmlOut.WriteClosingTag("Quickbar");
	}

	// CLOSE the file and save it
	xmlOut.Close();

	// UPDATE Quickbar for this player on the server
	this->UpdateQuickbarServerSide();

	// RESET the current workingd irectory
	SetCurrentDirectory(this->GL_workingDirectory);
}

void
GlobalStatusVariables::UpdateQuickbarServerSide()
{
	// ERROR checking: Make sure we are in the client app!
	if (!this->m_multiplayerObject->GetEnabledClient_safe())
		return;

	// GET client side player object
	POSITION pos = this->m_entObjRuntime->FindIndex(0);
	if (!pos) return;
	CMovementObj *pEntity = (CMovementObj*) this->m_entObjRuntime->GetAt(pos);
	if (!pEntity) return;

	// SEND message to server to update player's Quick Slots status
	int iQuickSlotsCount = NUM_QUICKBARS * NUM_QUICKBAR_SLOTS,
		iCount = 0,
		iGLID = -1;
	char buffer[10];
	CString strParams;

	// ADD item GLIDs to the param string
	CMenuInventoryItem *pItem = NULL;
	if(pEntity->List_controlType == 0)
	{
		for (int i=0; i<NUM_QUICKBARS; i++)
		{
			for (int j=0; j<NUM_QUICKBAR_SLOTS; j++)
			{
				pItem = *(this->m_multiplayerObject->Quickbar(i, j));
				if (!pItem) continue;
				::_itoa(pItem->m_data.m_GLID, buffer, 10);
				strParams.Append(buffer);
				strParams.Append(" ");
			}
		}

		// SEND the final network packet
		this->m_netGameLogicObject->SendTextMsg(DPNSEND_GUARANTEED,
			DPID_SERVERPLAYER,
			pEntity->List_networkDefinedID,
			NULL,
			TMC_COMMAND,
			strParams.GetBuffer(),
			SCMD_UPDATEQUICKSLOTS);
		strParams.ReleaseBuffer();
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		GlobalStatusVariables
// Function:	AddToQuickbar(CMenuInventoryItem*)
// Description:	Add an item to this player's Quickbar. Returns the index of
//	the slot into which the item was placed.
///////////////////////////////////////////////////////////////////////////////

int
GlobalStatusVariables::AddToQuickbar(CMenuInventoryItem *pItem)
{
	// ERROR checking
	if (!pItem)
		return -1;

	// FIND the first empty Quickbar slot
	bool bFound = false;
	int iBarIndex = 0, iSlotIndex = 0;
	for (int i=0; i<NUM_QUICKBARS; i++)
	{
		for (int j=0; j<NUM_QUICKBAR_SLOTS; j++)
		{
			CMenuInventoryItem *pTempItem = *(this->m_multiplayerObject->Quickbar(i, j));
			if (!pTempItem) 
			{
				if (!bFound)
				{
					iBarIndex = i;
					iSlotIndex = j;
					bFound = true;
				}
				continue;
			}
			else if (pTempItem->m_data.m_GLID == pItem->m_data.m_GLID)
				return -1;  // PREVENT duplicates in the Quickbar!
		}
	}

	// ERROR checking: No empty slots remain for item placement!
	if(!bFound) 
		return -1;

	// GET menu visuals for the selected Quickslot
	int iQuickbarSlotBgVisualID = QUICKBAR_SLOT_IDS[0][iSlotIndex];
	int iQuickbarSlotImageVisualID = QUICKBAR_SLOT_IDS[1][iSlotIndex];
	CMenuImage *pQuickbarSlotBg = dynamic_cast<CMenuImage*>(this->m_runtimeMenuDatabase->GetVisualByID(iQuickbarSlotBgVisualID, true));
	CMenuImage *pQuickbarSlotImage = dynamic_cast<CMenuImage*>(this->m_runtimeMenuDatabase->GetVisualByID(iQuickbarSlotImageVisualID, true));
	if (!pQuickbarSlotBg || !pQuickbarSlotImage) 
		return -1;

	// CLONE the selected inventory item for placement in the Quickbar
	CMenuInventoryItem *pCopyItem = NULL;
	pItem->Clone(&pCopyItem, this->m_3dDevice);
	pCopyItem->m_pIcon->m_blendMode = 1; // ALPHA BLEND
	this->m_multiplayerObject->Quickbar(iBarIndex, iSlotIndex, pCopyItem);

	// SET the image mesh for the selected inventory item
	pCopyItem->m_pIcon->Copy((CMenuVisual*)pQuickbarSlotImage, this->m_3dDevice);
	pQuickbarSlotImage->m_visible = true;
	pQuickbarSlotImage->m_textureSelect = this->m_textureDataBase->AddAssignTempTexture(
		this->m_3dDevice, 
		pQuickbarSlotImage->m_textureFile, 
		0);
	pQuickbarSlotImage->UpdateTexture(pQuickbarSlotImage->m_textureSelect);	

	// SET the position of the new Quickbar icon
	int iOffsetX = (pQuickbarSlotBg->m_width - pQuickbarSlotImage->m_width) / 2;
	int iOffsetY = (pQuickbarSlotBg->m_height - pQuickbarSlotImage->m_height) / 2;
	pQuickbarSlotImage->m_posX = pQuickbarSlotBg->m_posX + iOffsetX;
	pQuickbarSlotImage->m_posY = pQuickbarSlotBg->m_posY + iOffsetY;

	// SAVE the Quickbar to file
	this->SaveQuickbarConfig();

	// SET the quickbar selection indicator
	CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY, true));
	if (pInventory)
		pInventory->SetSelectionIndicator(pItem, CMenuInventory::IST_QUICKBAR, true);
	return iSlotIndex;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		GlobalStatusVariables
// Function:	RemoveFromQuickbar(ITEMDATA_STRUCT_EX)
// Description:	Remove an item from this player's Quickbar based on GLID.
///////////////////////////////////////////////////////////////////////////////

bool
GlobalStatusVariables::RemoveFromQuickbar(ITEMDATA_STRUCT_EX item)
{
	for (int i=0; i<NUM_QUICKBARS; i++)
	{
		for (int j=0; j<NUM_QUICKBAR_SLOTS; j++)
		{
			CMenuInventoryItem *pItem = *(this->m_multiplayerObject->Quickbar(i, j));
			if (pItem && (pItem->m_data.m_GLID == item.m_GLID))
				return this->RemoveFromQuickbar(i, j);
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		GlobalStatusVariables
// Function:	RemoveFromQuickbar(int, int)
// Description:	Remove an item from this player's Quickbar based on index.
///////////////////////////////////////////////////////////////////////////////

bool
GlobalStatusVariables::RemoveFromQuickbar(int iQuickbar, int iSlot)
{
	// GET the selected Quickbar item
	if (iQuickbar < 0 || iQuickbar >= NUM_QUICKBARS || iSlot < 0 || iSlot >= NUM_QUICKBAR_SLOTS)
		return false;
	CMenuInventoryItem **pItem = this->m_multiplayerObject->Quickbar(iQuickbar, iSlot);
	if (!pItem || !*pItem)
		return false;

	// HIDE the Quickbar image visual
	int iVisualID = QUICKBAR_SLOT_IDS[1][iSlot];
	CMenuImage *pQuickbarImage = dynamic_cast<CMenuImage*>(this->m_runtimeMenuDatabase->GetVisualByID(iVisualID, true));
	if (pQuickbarImage)
		pQuickbarImage->m_visible = false;

	// REMOVE the Quickbar selection indicator
	CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY, true));
	if (pInventory)
		pInventory->RemoveSelectionIndicator(*pItem, CMenuInventory::IST_QUICKBAR);

	// DELETE the selected Quickbar item
	SafeDelete(*pItem);
	this->m_multiplayerObject->m_iQuickbarSelectedIndex = -1;

	// SAVE the Quickbar to file
	this->SaveQuickbarConfig();
	return true;
}

//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::LoadPixelShaderLibrary(CString fileName)
{
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization
    	 
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);
     if(m_pxShaderSystem)
	 {
	   m_pxShaderSystem->SafeDelete();
	   delete m_pxShaderSystem;
	   m_pxShaderSystem = 0;
	 }
	 the_in_Archive >> m_pxShaderSystem;
	 the_in_Archive.Close();
     fl.Close();
	 m_pxShaderSystem->Compile(m_3dDevice);
	 m_renderStateObject->m_pxShderSystemRef = m_pxShaderSystem;
     SetCurrentDirectory(GL_workingDirectory);
 
  return TRUE;
}
//-----------init world object--------------------------------------------//
BOOL GlobalStatusVariables::InitWorldObject(CWldObject * ptrLoc)
{

	if(ptrLoc->m_charona)
	{
		ptrLoc->m_charona->m_visual->InitBuffer(m_3dDevice,ptrLoc->m_charona->m_visual->m_uvCount);
	}
	if(!ptrLoc->m_meshObject)
  	    return FALSE;

	          if(ptrLoc->m_lodVisualList)
			  {
				 ptrLoc->m_lodVisualList->Initialize(m_3dDevice,m_cgGlobalShaderDB);

			  }

			  if(ptrLoc->m_meshObject)
              if(ptrLoc->m_meshObject->m_abVertexArray.m_indecieCount == 0){//error
                 AfxMessageBox(operator+("ERROR 104- init world object()-no indecies ",ptrLoc->m_fileNameRef));
				 return FALSE;
			  }//end error

			 
               
			  if(ptrLoc->m_meshObject){
                 
				 if(ptrLoc->m_meshObject->List_materialObject)
                    ptrLoc->m_meshObject->InitBuffer(m_3dDevice,m_cgGlobalShaderDB->GetByIndex(ptrLoc->m_meshObject->List_materialObject->m_optionalShaderIndex-1));//ptrLoc->m_meshObject->List_materialObject->m_diffuseEffect);
				 else
				    ptrLoc->m_meshObject->InitBuffer(m_3dDevice,NULL);

				 if(ptrLoc->m_meshObject->m_charona){
				    //ptrLoc->m_meshObject->m_charona->InitializeVis();
				    ptrLoc->m_meshObject->m_charona->m_visual->InitBuffer(m_3dDevice,ptrLoc->m_meshObject->m_charona->m_visual->m_uvCount);
				 }

				 
			  
			  }

			  if(ptrLoc->List_attribute)
              ptrLoc->List_attribute->List_inUse=FALSE;

              ptrLoc->List_poppedOut = TRUE;
			  
              //pop out initially
			   ptrLoc->List_poppedOut = TRUE;
			   if(ptrLoc->List_useSound==TRUE){//sound extension
                  ptrLoc->List_isPlaying = FALSE;
			   }//end sound extension

			 return TRUE;
}
//--------------Destroy all missile lists---------------------------------------------//
void GlobalStatusVariables::DestroyAllMissileClasses()
{
//runtime
 if(m_missileRuntime){
 for(POSITION posLoc = m_missileRuntime->GetHeadPosition();posLoc != NULL;){//visual loop
	 CMissileObj * mPtr = ( CMissileObj* ) m_missileRuntime->GetNext(posLoc);
     DestroyMissileObject(mPtr);
 }
 m_missileRuntime->RemoveAll();
 delete m_missileRuntime;
 }
 m_missileRuntime=0;
//DB
 if(m_missileDB){
 for(POSITION posLoc = m_missileDB->GetHeadPosition();posLoc != NULL;){//visual loop
	 CMissileObj * mPtr = ( CMissileObj* ) m_missileDB->GetNext(posLoc);
     DestroyMissileObject(mPtr);
 }
 m_missileDB->RemoveAll();
 delete m_missileDB;
 }
 m_missileDB=0;
}
//----------------Destroy all Dot lists-----------------------------------------------//
void GlobalStatusVariables::DestroyAllDotClasses()
{
	if(m_dotDB)
	{
		for(POSITION posLoc = m_dotDB->GetHeadPosition(); posLoc != NULL;)
		{
			CDotClass *dPtr = (CDotClass *) m_dotDB->GetNext(posLoc);
			DestroyDotObject(dPtr);
		}
		m_dotDB->RemoveAll();
		delete m_dotDB;
	}
	m_dotDB = 0;
}
//----------------Destroy all Movie objects-------------------------------------------//
void GlobalStatusVariables::DestroyAllMovieObjects()
{
if(!MovieRList)
    return;

  for(POSITION posLoc = MovieRList->GetHeadPosition();posLoc != NULL;){//movie loop
    CMovieRuntimeObj* mPtr = (CMovieRuntimeObj*) MovieRList->GetNext(posLoc);
	delete mPtr;
	mPtr=0;
  }
  MovieRList->RemoveAll();
  delete MovieRList;
  MovieRList=0;
}
//----------------Destroy Control List------------------------------------------------//
void GlobalStatusVariables::DestroyControlList()
{
	/*
	if(!m_controlsList) return;
	for(POSITION posLoc = m_controlsList->GetHeadPosition();posLoc != NULL;)
	{//control loop
		ControlDBObj * conPtr = ( ControlDBObj* ) m_controlsList->GetNext(posLoc);
		for(POSITION posLoc2 = conPtr->List_controlInfoList->GetHeadPosition();posLoc2 != NULL;)
		{//info loop
			ControlInfoObj * infoPtr = ( ControlInfoObj* ) conPtr->List_controlInfoList->GetNext(posLoc2);
			delete infoPtr;
			infoPtr=0;
		}
		conPtr->List_controlInfoList->RemoveAll();
		delete conPtr->List_controlInfoList;
		conPtr->List_controlInfoList=0;
		delete conPtr;
		conPtr=0;
	}
	m_controlsList->RemoveAll();
	delete m_controlsList;
	m_controlsList=0;
	*/
	if(!m_controlsList) return;
	for(POSITION posLoc = m_controlsList->GetHeadPosition();posLoc != NULL;)
	{//control loop
		ControlDBObj * conPtr = ( ControlDBObj* ) m_controlsList->GetNext(posLoc);
		for(POSITION posLoc2 = conPtr->List_controlInfoList->GetHeadPosition();posLoc2 != NULL;)
		{//info loop
			ControlInfoObj * infoPtr = ( ControlInfoObj* ) conPtr->List_controlInfoList->GetNext(posLoc2);
			SafeDelete(infoPtr);
		}
		conPtr->List_controlInfoList->RemoveAll();
		SafeDelete(conPtr->List_controlInfoList);
		SafeDelete(conPtr);
	}
	m_controlsList->RemoveAll();
	SafeDelete(m_controlsList);
}
//----------------Initialize Control List---------------------------------------------//
void GlobalStatusVariables::InitializeControlDB()
{
	if(!this->m_controlsList) 
		this->m_controlsList = new ControlDBList();

	/*for(POSITION posLoc = m_controlsList->GetHeadPosition(); posLoc != NULL;)
	{//control loop
		ControlDBObj * conPtr = ( ControlDBObj* ) m_controlsList->GetNext(posLoc);
		//if(m_editorModeOn == TRUE)
		//  m_controlsDlgBar.SendDlgItemMessage(IDC_LIST_ControlDB,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)conPtr->List_controlConfigName);
		for(POSITION posLoc2 = conPtr->List_controlInfoList->GetHeadPosition();posLoc2 != NULL;)
		{//info loop
			ControlInfoObj * infoPtr = ( ControlInfoObj* ) conPtr->List_controlInfoList->GetNext(posLoc2);
		}
	}*/
}
//--------------destroy all viewports-------------------------------------------------//
void GlobalStatusVariables::DestroyAllVieports()
{
	if(!m_viewPortDB)
		return;
	for(POSITION posLoc = m_viewPortDB->GetHeadPosition();posLoc != NULL;)
	{
	     CViewportObj* mptLoc = ( CViewportObj* ) m_viewPortDB->GetNext(posLoc);
		 delete mptLoc;
		 mptLoc=0;
	}
	m_viewPortDB->RemoveAll();
	delete m_viewPortDB;
	m_viewPortDB=0;

	if(m_runtimeViewportDB)
	for(POSITION posLoc = m_runtimeViewportDB->GetHeadPosition();posLoc != NULL;)
	{
	     CViewportObj* mptLoc = ( CViewportObj* ) m_runtimeViewportDB->GetNext(posLoc);
		 mptLoc->SafeDelete();
		 delete mptLoc;
		 mptLoc=0;
	}
	m_runtimeViewportDB->RemoveAll();
	delete m_runtimeViewportDB;
	m_runtimeViewportDB=0;
}
//-------------------Destroy all collection list--------------------------------------//
void GlobalStatusVariables::DestroyTextureCollectionList(CTextureSwapObjList *textureColList)
{
if(!textureColList)
return;
 for(POSITION posLoc=textureColList->GetHeadPosition();posLoc != NULL;)
 {
     CTextureSwapObj* txtObjPtr = (CTextureSwapObj*)textureColList->GetNext(posLoc);
     DestroyTextureCollection(txtObjPtr);
	 delete txtObjPtr;
	 txtObjPtr=0;
 }
 textureColList->RemoveAll();
 delete textureColList;
 textureColList=0;
}
//-------------------Initialize all collections---------------------------------------//
BOOL GlobalStatusVariables::InitAllTexCollections()
{
//if(m_editorModeOn == TRUE)
//m_animatedTextureDBDlgBar.SendDlgItemMessage(IDC_LIST_TEX_Collection,LB_RESETCONTENT,0,0);
for(POSITION posLoc=TextureSwapList->GetHeadPosition();posLoc != NULL;)
 {
     CTextureSwapObj* txtObjPtr = (CTextureSwapObj*)TextureSwapList->GetNext(posLoc);
     //BOOL result = InitCollection(txtObjPtr);
     //if(m_editorModeOn == TRUE)
     //  m_animatedTextureDBDlgBar.SendDlgItemMessage(IDC_LIST_TEX_Collection,LB_ADDSTRING,0,(LPARAM)(LPCTSTR)operator+("Map Basis :: ",txtObjPtr->List_collectionName));

	 //if(result == FALSE)
	 //	return FALSE;
 }
return TRUE;
}

//--------------destroy all viewports-------------------------------------------------//
BOOL GlobalStatusVariables::WriteCfgFile(CString filename)
{
    CFileException exc;
    CFile fl;
	CArchive the_out_Archive(&fl,CArchive::store);
    BOOL res = fl.Open(filename,CFile::modeCreate | CFile::modeWrite, &exc);
   
    the_out_Archive.WriteString("VERSION5");
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString("Resolution Width -1 = Auto Detect");
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString(m_cabLib.ConvToString(m_dxRenderer->m_currentWidth));
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString("Resolution Hieght -1 = Auto Detect");
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString(m_cabLib.ConvToString(m_dxRenderer->m_currentHeight));
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString("Bit Depth -1 = Auto Detect");
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString(m_cabLib.ConvToString(m_dxRenderer->m_currentDepth));
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString("Enhanced Mode - Recommended off (forces 16 bit depth non stencil) required for Stencil shadows must be on");
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString(m_cabLib.ConvToString(m_enhancedMode));
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString("Shadow Type 0=NONE 1=Stencil Volumes 2=Spot Basic - Recommended off");
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString(m_cabLib.ConvToString(m_globalShadowType));
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString("Shadow Distance");
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString(m_cabLib.ConvToString((int)m_globalShadowDist));
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString("Self Shadow in first person- Recommended 0");
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString(m_cabLib.ConvToString(m_showFirstPersonShadow));
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString("Max Shadow Render");
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString(m_cabLib.ConvToString(m_maxShadowCount));
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString("Max Texture Size");
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString(m_cabLib.ConvToString(m_dxRenderer->m_maxTextureSize));
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString("Compress Textures");
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString(m_cabLib.ConvToString(m_compressTextures));
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString("Render Processor post effect");
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString(m_cabLib.ConvToString(m_globalRenderer));
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString("Show Character Names Above Head");
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString(m_cabLib.ConvToString(m_showNames));
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString("VSync 0=Off (uncapped) 1=On at monitor rate 2=Cap 60Hz 3=Cap 120Hz 4=Cap 144Hz 5=Cap 240Hz");
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString(m_cabLib.ConvToString(m_dxRenderer->m_vsyncMode));
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString("AntiAliasing MSAA 0=Off 1=2x 2=4x 3=8x");
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString(m_cabLib.ConvToString(m_dxRenderer->m_aaMode));
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString("Anisotropic Filtering 0=Off 1=2x 2=4x 3=8x 4=16x");
	the_out_Archive.WriteString( "\n");
	the_out_Archive.WriteString(m_cabLib.ConvToString(m_dxRenderer->m_anisoMode));
	the_out_Archive.WriteString( "\n");

	the_out_Archive.Close();
	fl.Close();
   return TRUE;
}
//------Restores vports based on camera data-used for display change------------------//
void GlobalStatusVariables::RestoreVports()
{
int indexLoc=0;
	for(POSITION posLoc = m_runtimeViewportDB->GetHeadPosition();posLoc != NULL;)
	{
	     CViewportObj* mptLoc = ( CViewportObj* ) m_runtimeViewportDB->GetNext(posLoc);
		 mptLoc->UtilizeViewPort(m_dxRenderer->m_currentWidth,m_dxRenderer->m_currentHeight,m_3dDevice);
		 //ActivateVieport(mptLoc);
	     indexLoc++;
	}
}
//--------------calibrate screen snap-------------------------------------------------//
void GlobalStatusVariables::CalibrateScreenSnap(int unitX,int unitY)
{
	//covert to flaot for drift
	float unitXLoc = (float)unitX;
	float unitYLoc = (float)unitY;
	float videoWidthLoc = (float)m_dxRenderer->m_currentWidth;//- imediateRenderRECT.left;
	float videoHeightLoc = (float)m_dxRenderer->m_currentHeight;//- imediateRenderRECT.top;

	m_snapX = videoWidthLoc / unitXLoc;
	m_snapY = videoHeightLoc / unitYLoc;
}
//---------------------------------------------------------------------//
/*void GlobalStatusVariables::RefreshMenus()
{
	POSITION posLast = NULL;
	int menuCurrentCount = (int)m_runtimeMenuDataBase->GetCount();
	if(menuCurrentCount < 1)
		return;
	int * menuCfgArray = new int[menuCurrentCount+10];
	int tracer = 0;
     for(POSITION posLoc = m_runtimeMenuDataBase->GetHeadPosition();(posLast = posLoc) != NULL;)
	 {
        CMenu3DObj * visObj = (CMenu3DObj *)m_runtimeMenuDataBase->GetNext(posLoc);
		int storeID = visObj->m_menuAccessID;
        if(!visObj->m_closing)
		{
		  //make sure it isnt gunna open one already
		  BOOL reopen = TRUE;
		  for(int j=0;j<tracer;j++)
		  {
		      if(menuCfgArray[j]==storeID)
			     reopen = FALSE;
		  }
		  if(reopen)
		  {
		   menuCfgArray[tracer] = storeID;
		   tracer++;
		  }
		}
		//end exec closing actions
		visObj->CloseMenu(&m_globalCursorPos);
		m_textureDataBase->RemoveAllTempTexturesByChannel(visObj->m_menuAccessID+5);
	    visObj->SafeDelete();
	    delete visObj;
	    visObj = 0;
	    m_runtimeMenuDataBase->RemoveAt(posLast);
		//
	 }
   
	 if(menuCurrentCount > 0)
	 {
		 for(int i=0;i<tracer;i++)
		     OpenMenu(menuCfgArray[i], false); //TODO: Figure out what to do, used to be TRUE

		 delete [] menuCfgArray;
		 menuCfgArray = 0;
	 }
}*/
//-------------------------------------------------------------------------
void GlobalStatusVariables::DestroyEntityList()
{
POSITION posLoc,delPos;
   if(m_entObjList){
    for(posLoc = m_entObjList->GetHeadPosition();posLoc != NULL;){//visual loop
	    CMovementObj * mPtr = ( CMovementObj* ) m_entObjList->GetNext(posLoc);
		delete mPtr;
	    mPtr=0;
	}
	m_entObjList->RemoveAll();
	delete m_entObjList;
	m_entObjList=0;
   }  
   if(m_entObjRuntime){
    for(posLoc = m_entObjRuntime->GetHeadPosition();(delPos = posLoc) != NULL;){//visual loop
	    CMovementObj * mPtr = ( CMovementObj* ) m_entObjRuntime->GetNext(posLoc);
		DestroyRuntimeEntity(mPtr,m_spawnGeneratorDB,m_entObjRuntime);
		mPtr = 0;
	}
	m_entObjRuntime->RemoveAll();
   }	
}
//--------------initialize entity in data base----------------------------------------//
void GlobalStatusVariables::InitAllDatabaseEntitys()
{
 RebuildTraceNumber();//rebuild trace numbers
}
//----------------Calculate all changable rects---------------------------------------//
void GlobalStatusVariables::CalculateAllMovieDisplayRects()
{
  for(POSITION posLoc = MovieRList->GetHeadPosition();posLoc != NULL;){//movie loop
    CMovieRuntimeObj* mPtr = (CMovieRuntimeObj*) MovieRList->GetNext(posLoc);
    float X1conv = (float)mPtr->List_rectMov.left;
		   float Y1conv = (float)mPtr->List_rectMov.top;
		   float X2conv = (float)mPtr->List_rectMov.right;
		   float Y2conv = (float)mPtr->List_rectMov.bottom;
           RECT rectConv;
		   
		   
		   float width = (float)m_dxRenderer->m_currentWidth;
	       rectConv.left = (int)((X1conv*width)*.01);
	       rectConv.right = (int)((X2conv*width)*.01);
	       float hieght = (float)m_dxRenderer->m_currentHeight;
	       rectConv.top = (int)((Y1conv*hieght)*.01);
	       rectConv.bottom = (int)((Y2conv*hieght)*.01);
		   mPtr->List_changingRect = rectConv;
  }
}
//---------------------------load Hud-------------------------------------------------//
void GlobalStatusVariables::DeleteHudList() 
{
/*if(!m_hudList)
return;
  for(POSITION posLoc = m_hudList->GetHeadPosition();posLoc != NULL;){
      CHudObj *hudPtr = (CHudObj *)m_hudList->GetNext(posLoc);
      hudPtr->SafeDelete();
	  delete hudPtr;
	  hudPtr=0;
  }	
  m_hudList->RemoveAll();
  delete m_hudList;
  m_hudList=0;*/
}
//------------------------------------------------------------------------------------//
void GlobalStatusVariables::DestroyAllParticleInterfaces()
{

     if(m_particleDatabase){
      for(POSITION posLoc = m_particleDatabase->GetHeadPosition();posLoc != NULL;){
        EnvParticleSystem * partPtr = (EnvParticleSystem *)m_particleDatabase->GetNext(posLoc);
        delete partPtr;
		partPtr=0;
	  }
	  m_particleDatabase->RemoveAll();
	  delete m_particleDatabase;
	  m_particleDatabase=0;
	 }
}
//--------------Clean Up Particle System----------------------------------------------//
BOOL GlobalStatusVariables::DeleteParticleSystem()
{
 if(!m_particleDatabase)
    return FALSE;

	for(POSITION posLocal = m_particleDatabase->GetHeadPosition();posLocal != NULL;){
        EnvParticleSystem * partiPtr = (EnvParticleSystem *)m_particleDatabase->GetNext(posLocal);
        delete partiPtr;
		partiPtr=0;
	}
	m_particleDatabase->RemoveAll();
	delete m_particleDatabase;
	m_particleDatabase=0;
 return TRUE;
}
//-----------reinit all textures from database----------------------------------------//
void GlobalStatusVariables::ReinitAllTexturesFromDatabase()
{
if(!m_3dDevice)
return;

if(!m_textureDataBase)
return;

//m_textureDataBase->m_compressRef = compressed;

//flag notifiers-guide the user when textures should be initialized before editing some objects
if(m_itemManager->m_visuals)
   m_itemManager->m_visuals->m_texturesHaveBeenInitialized = 1;
m_entObjList->m_texturesHaveBeenInitialized = 1;

//m_textureDataBase->RemoveAllTempTextures();
m_textureDataBase->RemoveAll();//clear
m_textureDataBase->SafeDelete();//removes direct access

//append
for(POSITION staticPos = m_staticTextureDataBase->GetHeadPosition();staticPos != NULL;)
{
	CTextureDBObj * texture = (CTextureDBObj *)m_staticTextureDataBase->GetNext(staticPos);
    m_textureDataBase->AddTail(texture);
}
//append
for(POSITION dynamicPos = m_dynamicTextureDataBase->GetHeadPosition();dynamicPos != NULL;)
{
	CTextureDBObj * texture = (CTextureDBObj *)m_dynamicTextureDataBase->GetNext(dynamicPos);
    m_textureDataBase->AddTail(texture);
}

//m_dynamicTextureDataBase->RestoreAllTextures(m_3dDevice,m_dxRenderer->m_maxTextureSizeSquared,m_renderStateObject->m_pCaps);
    int textureCount = m_dynamicTextureDataBase->GetCount();
	int progTracer = 0;
	for(POSITION posLoc = m_dynamicTextureDataBase->GetHeadPosition();posLoc != NULL;)
	{
        CTextureDBObj * txObj = (CTextureDBObj *)m_dynamicTextureDataBase->GetNext(posLoc);	
		float currentProg = ((float)progTracer/(float)textureCount)*100.0f;
		UpdateMeterAdv(currentProg,"INITIALIZING ALL TEXTURES");
		
		txObj->m_textureObject->Restore(m_3dDevice,
										0,
										txObj->m_bump,
										txObj->m_bumpType,
										m_dxRenderer->m_maxTextureSizeSquared,
										m_renderStateObject->m_pCaps,
										m_dynamicTextureDataBase->m_compressRef,
										m_advUrlBasic,
									    m_advUrlVending);
 
        
         

		progTracer++;
	}
    UpdateMeterAdv(100.0f,"INITIALIZING GAME");



ReinitTextureUseage();
m_textureDataBase->CompileDirectAccess();
//MessageBox("Completed");
}

//-----------reinit all textures from database----------------------------------------//
void GlobalStatusVariables::ReinitAllTexturesFromDatabaseRemoveUnused()
{
if(!m_3dDevice)
return;

if(!m_textureDataBase)
return;

//m_textureDataBase->RemoveAllTempTextures();
m_textureDataBase->RemoveAll();//clear
m_textureDataBase->SafeDelete();//removes direct access

//append
POSITION posLast = NULL;
for(POSITION staticPos = m_staticTextureDataBase->GetHeadPosition();(posLast = staticPos) != NULL;)
{
	CTextureDBObj * texture = (CTextureDBObj *)m_staticTextureDataBase->GetNext(staticPos);
    if(texture->m_tempVar != 0)
	{
       m_textureDataBase->AddTail(texture);
	}
	else
	{
       texture->SafeDelete();
	   delete texture;
	   texture = 0;
       m_staticTextureDataBase->RemoveAt(posLast);
	}
}
//append
for(POSITION dynamicPos = m_dynamicTextureDataBase->GetHeadPosition();(posLast = dynamicPos) != NULL;)
{
	CTextureDBObj * texture = (CTextureDBObj *)m_dynamicTextureDataBase->GetNext(dynamicPos);
    if(texture->m_tempVar != 0)
	{
       m_textureDataBase->AddTail(texture);
	}
	else
	{
       texture->SafeDelete();
	   delete texture;
	   texture = 0;
       m_dynamicTextureDataBase->RemoveAt(posLast);
	}
}

m_dynamicTextureDataBase->RestoreAllTextures(m_3dDevice,m_dxRenderer->m_maxTextureSizeSquared,m_renderStateObject->m_pCaps);
ReinitTextureUseage();
m_textureDataBase->CompileDirectAccess();
//MessageBox("Completed");
}

//--------------initialize a DB missile Object in DB----------------------------------//
void GlobalStatusVariables::DestroyMissileObject(CMissileObj* mislPtr)
{
	mislPtr->SafeDelete();
	delete mislPtr;
	mislPtr=0;
}
//--------------Destroy a Dot Object--------------------------------------------------//
void GlobalStatusVariables::DestroyDotObject(CDotClass *dotPtr)
{
	dotPtr->SafeDelete();
	delete dotPtr;
	dotPtr = 0;
}
//-------------------destroy collection-----------------------------------------------//
void GlobalStatusVariables::DestroyTextureCollection(CTextureSwapObj* txtObjPtr)
{
POSITION posLast = NULL;
  for(POSITION posLoc=txtObjPtr->List_bitmapList->GetHeadPosition();(posLast = posLoc) != NULL;)
  {
      CBitmapObj * bmpObj = (CBitmapObj *)txtObjPtr->List_bitmapList->GetNext(posLoc);
	  delete bmpObj;
	  bmpObj=0;
  }
  txtObjPtr->List_bitmapList->RemoveAll();
  delete txtObjPtr->List_bitmapList;
  txtObjPtr->List_bitmapList = 0;
  return;
}
//--------------Reinit all for texture useage-----------------------------------------//
void GlobalStatusVariables::ReinitTextureUseage()
{
//enviorment
	if(m_environmentPtr){
           m_environmentPtr->ReinitAllTextures(m_textureDataBase,
			                                   m_dynamicTextureDataBase,
								               m_3dDevice,
								               m_renderStateObject,
											   m_txbBitmapLoadOverride,
											   m_dxRenderer->m_maxTextureSizeSquared);
		   if(m_environmentPtr->m_enviormentTimeSystem)
              if(m_environmentPtr->m_enviormentTimeSystem->m_visual)
	          ReinitTextureUsage(m_environmentPtr->m_enviormentTimeSystem->m_visual);
	
	}


POSITION posLoc;

//SELECTION EFFECTS
for(posLoc = m_selectionEffectDB->GetHeadPosition();posLoc != NULL;)
{
    CSelectionEffect * effectPtr = (CSelectionEffect *)m_selectionEffectDB->GetNext(posLoc);
    SkeletalTextureReassignment(effectPtr->m_skeletalPtr);
}
//Library objects
for(posLoc = m_objectLibrary->GetHeadPosition();posLoc != NULL;)
{
	CObjectCollection * cPtr = (CObjectCollection *)m_objectLibrary->GetNext(posLoc);
    for(POSITION colPos = cPtr->m_collection->GetHeadPosition();colPos != NULL;)
	{
        CObjectLibraryObj * cLibPtr = (CObjectLibraryObj *) cPtr->m_collection->GetNext(colPos);
        
		if(cLibPtr->m_wldObject->m_meshObject)
	       ReinitTextureUsage(cLibPtr->m_wldObject->m_meshObject);

		if(cLibPtr->m_wldObject->m_lodVisualList)
	    for(POSITION lPos = cLibPtr->m_wldObject->m_lodVisualList->GetHeadPosition();lPos != NULL;)
		{
            CMeshLODObject * lodPtr = (CMeshLODObject *)cLibPtr->m_wldObject->m_lodVisualList->GetNext(lPos);
            ReinitTextureUsage(lodPtr->m_meshLodLevel);
		}
	}
}
//WORLD OBJECTS
for(posLoc = m_wldObjList->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CWldObject * ptrLoc = ( CWldObject* ) m_wldObjList->GetNext(posLoc);
	if(ptrLoc->m_meshObject)
	ReinitTextureUsage(ptrLoc->m_meshObject);
	if(ptrLoc->m_lodVisualList)
	for(POSITION lPos = ptrLoc->m_lodVisualList->GetHeadPosition();lPos != NULL;)
	{
        CMeshLODObject * lodPtr = (CMeshLODObject *)ptrLoc->m_lodVisualList->GetNext(lPos);
        ReinitTextureUsage(lodPtr->m_meshLodLevel);
	}
}//end begin world object loop
//housing
for(posLoc = m_housingDB->GetHeadPosition();posLoc != NULL;){//Begin world object loop
	CHousingObj * hPtr = ( CHousingObj* ) m_housingDB->GetNext(posLoc);
	if(hPtr->m_externalVisuals)
	for(POSITION lPos = hPtr->m_externalVisuals->GetHeadPosition();lPos != NULL;)
	{
        CEXMeshObj * mPtr = (CEXMeshObj *)hPtr->m_externalVisuals->GetNext(lPos);
        ReinitTextureUsage(mPtr);
	}
	if(hPtr->m_internalVisuals)
	for(POSITION lPos = hPtr->m_internalVisuals->GetHeadPosition();lPos != NULL;)
	{
        CEXMeshObj * mPtr = (CEXMeshObj *)hPtr->m_internalVisuals->GetNext(lPos);
        ReinitTextureUsage(mPtr);
	}
	if(hPtr->m_lod2Set)
	for(POSITION lPos = hPtr->m_lod2Set->GetHeadPosition();lPos != NULL;)
	{
        CEXMeshObj * mPtr = (CEXMeshObj *)hPtr->m_lod2Set->GetNext(lPos);
        ReinitTextureUsage(mPtr);
	}
	if(hPtr->m_lod3Set)
	for(POSITION lPos = hPtr->m_lod3Set->GetHeadPosition();lPos != NULL;)
	{
        CEXMeshObj * mPtr = (CEXMeshObj *)hPtr->m_lod3Set->GetNext(lPos);
        ReinitTextureUsage(mPtr);
	}
	if(hPtr->m_doorVisual)
        ReinitTextureUsage(hPtr->m_doorVisual);
}//end begin world object loop

/*for(posLoc = m_commCtrlDB->GetHeadPosition();posLoc != NULL;)
{//comm ctrl loop
	CCommonCtrlObj * cntrlObj = ( CCommonCtrlObj* ) m_commCtrlDB->GetNext(posLoc);
   
	if(cntrlObj->m_clickState)
       SkeletalTextureReassignment(cntrlObj->m_clickState,1);
	if(cntrlObj->m_hoverState)
       SkeletalTextureReassignment(cntrlObj->m_hoverState,1);
	if(cntrlObj->m_idleState)
       SkeletalTextureReassignment(cntrlObj->m_idleState,1);
}//end comm ctrl loop*/
//runtime menu OBJECTS

for(posLoc = m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;){//RM
	CMenu3DObj * menu3DObj = ( CMenu3DObj* ) m_runtimeMenuDataBase->GetNext(posLoc);
    if(menu3DObj->m_cursor)	
	   SkeletalTextureReassignment(menu3DObj->m_cursor->m_skeletonObject);
	   //visual list 
	   for(POSITION vPos = menu3DObj->m_visualItems->GetHeadPosition();vPos != NULL;)
	   {
		   CMenuVisObj * vPtr = (CMenuVisObj *)menu3DObj->m_visualItems->GetNext(vPos);
           if(vPtr->m_skeletonObject)
			  SkeletalTextureReassignment(vPtr->m_skeletonObject);
		   if(vPtr->m_sliderSystem){
              if(vPtr->m_sliderSystem->m_visStart)
				 SkeletalTextureReassignment(vPtr->m_sliderSystem->m_visStart);
              if(vPtr->m_sliderSystem->m_visMov)
				 SkeletalTextureReassignment(vPtr->m_sliderSystem->m_visMov);
              if(vPtr->m_sliderSystem->m_visEnd)
				 SkeletalTextureReassignment(vPtr->m_sliderSystem->m_visEnd);
		   }
		   if(vPtr->m_menuBarObj){
              if(vPtr->m_menuBarObj->m_barDynamic)
				 ReinitTextureUsage(vPtr->m_menuBarObj->m_barDynamic);
              if(vPtr->m_menuBarObj->m_barStatic)
				 ReinitTextureUsage(vPtr->m_menuBarObj->m_barStatic);
		   }
	   }
}//end RM
for(POSITION misPos = m_explosionDBList->GetHeadPosition();misPos != NULL;)
{
      CExplosionObj * expObj = (CExplosionObj *)m_explosionDBList->GetNext(misPos);
	  if(expObj->m_meshObject)
		 ReinitTextureUsage(expObj->m_meshObject);

	  if(expObj->m_skeletalObj)
		 SkeletalTextureReassignment(expObj->m_skeletalObj);
}
//CG fonts
for(posLoc = m_cgFontList->GetHeadPosition();posLoc != NULL;){//Begin CG loop
	CGFontObj * ptrLoc = ( CGFontObj* ) m_cgFontList->GetNext(posLoc);
	int indexLoc = 0;
	if(m_textureDataBase->TextureSearch(ptrLoc->m_fontTableName,indexLoc,1,m_dynamicTextureDataBase,0)==TRUE)
	   ptrLoc->m_fontTableIndex = indexLoc;
}//end CG loop
//Weapon SYS /Missiles
for(posLoc = m_missileDB->GetHeadPosition();posLoc != NULL;)
{
    CMissileObj * mPtr = (CMissileObj *)m_missileDB->GetNext(posLoc);
    if(mPtr->m_basicMesh)
       ReinitTextureUsage(mPtr->m_basicMesh);
	if(mPtr->m_skeletalVisual)
	   SkeletalTextureReassignment(mPtr->m_skeletalVisual);
	if(mPtr->m_laserObject)
       ReinitTextureUsage(mPtr->m_laserObject->m_meshObject);
}
//ENTITIES
for(posLoc = m_entObjList->GetHeadPosition();posLoc != NULL;){//Begin movement entity loop
	CMovementObj * ptrLoc = ( CMovementObj* ) m_entObjList->GetNext(posLoc);
    TextureReassignEntity(ptrLoc);
}
//Global Appendage database

if(m_itemManager)
if(m_itemManager->m_visuals)
   for(POSITION armedPos = m_itemManager->m_visuals->GetHeadPosition();armedPos != NULL;)
   {//appendage Loop
      CArmedInventoryObj* armedObj = (CArmedInventoryObj*)m_itemManager->m_visuals->GetNext(armedPos);
      if(armedObj->m_thirdPersonSystem)//third person
         SkeletalTextureReassignment(armedObj->m_thirdPersonSystem);
	   //first person
	   if(armedObj->m_firstPersonSystem)//third person
          SkeletalTextureReassignment(armedObj->m_firstPersonSystem);
    }//end appendage loop
//Reference library
ReinitTextureLibraryTextures();
/*for(posLoc = m_libraryReferenceDB->GetHeadPosition();posLoc != NULL;)
{
    CReferenceObj * referenceObject = (CReferenceObj *) m_libraryReferenceDB->GetNext(posLoc);
    ReinitTextureUsage(referenceObject->m_meshObject);
	if(referenceObject->m_lodVisualList)
	   if(referenceObject->m_lodVisualList->m_lodType == 2)
	   {//
           for(POSITION lPos = referenceObject->m_lodVisualList->GetHeadPosition();lPos != NULL;)
		   {
               CMeshLODObject * lodPtr = (CMeshLODObject *) referenceObject->m_lodVisualList->GetNext(lPos);
			   ReinitTextureUsage(lodPtr->m_meshLodLevel);
		   }
		   continue;
	   }//

	referenceObject->UpdateLods();
}*/
//Particle systemsReinitTextureUsage
for(posLoc = m_particleDatabase->GetHeadPosition();posLoc != NULL;)
{
        EnvParticleSystem * partPtr = (EnvParticleSystem *)m_particleDatabase->GetNext(posLoc);
		int indexLoc = 0;
	    if(m_textureDataBase->TextureSearch(partPtr->m_textureName,indexLoc,10,m_dynamicTextureDataBase,0)==TRUE)
	       partPtr->m_textureIndexDyn = indexLoc;
}

//CG fonts
for(posLoc = m_iconDatabase->GetHeadPosition();posLoc != NULL;){//Begin CG loop
	CIconSysObj * iconPtr = ( CIconSysObj* ) m_iconDatabase->GetNext(posLoc);
	int indexLoc = 0;
	if(m_textureDataBase->TextureSearch(iconPtr->m_textureName,indexLoc,1,m_dynamicTextureDataBase,0)==TRUE)
	   iconPtr->m_textureIndex = indexLoc;

	
}//end CG loop
//-------------------
if(m_shadowTextureFromFile.GetLength() > 0)
{
   int indexLoc = 0; 
   if(m_textureDataBase->TextureSearch(m_shadowTextureFromFile,indexLoc,1,m_dynamicTextureDataBase,0)==TRUE)
      m_shadowIndexTemp = indexLoc;    
}

//Global Shader lib
for(posLoc = m_cgGlobalShaderDB->GetHeadPosition();posLoc != NULL;){//Begin CG loop
	CGlobalMaterialShader * shdPtr = ( CGlobalMaterialShader* ) m_cgGlobalShaderDB->GetNext(posLoc);
	
    for(int i=0;i<6;i++)
	{
      if(shdPtr->m_textureIndexArray[i] != -1)//assign it
	  {
	     int indexLoc = 0;
	     if(m_textureDataBase->TextureSearch(shdPtr->m_textureStringArray[i],indexLoc,8,m_dynamicTextureDataBase,0)==TRUE)
	        shdPtr->m_textureIndexArray[i] = indexLoc;
	  }
	}
}//end CG loop
//InitParticleDatabase();
return;
}
//-----------------------------------------------------------------
void GlobalStatusVariables::ReinitTextureLibraryTextures()
{
	for(POSITION posLoc = m_libraryReferenceDB->GetHeadPosition();posLoc != NULL;)
	{
		CReferenceObj * referenceObject = (CReferenceObj *) m_libraryReferenceDB->GetNext(posLoc);
		  
		if(referenceObject->m_skeletalOption)
		   SkeletalTextureReassignment(referenceObject->m_skeletalOption);


		if(referenceObject->m_meshObject)
		{//mesh basis
			ReinitTextureUsage(referenceObject->m_meshObject);
			if(referenceObject->m_lodVisualList)
			if(referenceObject->m_lodVisualList->m_lodType == 2)
			{//
                
 

				for(POSITION lPos = referenceObject->m_lodVisualList->GetHeadPosition();lPos != NULL;)
				{
					CMeshLODObject * lodPtr = (CMeshLODObject *) referenceObject->m_lodVisualList->GetNext(lPos);
					ReinitTextureUsage(lodPtr->m_meshLodLevel);
					
				}
				continue;
			}//
			referenceObject->UpdateLods();
		}//end mesh basis
	}
}
//------------------------------------------------------------------------------------//
void GlobalStatusVariables::TextureReassignEntity(CMovementObj * ptrLoc)
{
	if(ptrLoc->List_skeletalObject){//valid main bone sys 	
	//deformable skin meshes
	SkeletalTextureReassignment(ptrLoc->List_skeletalObject);
    //shadow
	if(ptrLoc->List_skeletalObject->m_shadowOn == TRUE){
	    int indexLocal = 0;
		if(m_textureDataBase->TextureSearch(ptrLoc->List_skeletalObject->m_shadowInterface->m_textureSearchName,indexLocal,1,m_dynamicTextureDataBase,0)==TRUE)
	       ptrLoc->List_skeletalObject->m_shadowInterface->m_textureIndex = indexLocal;  
	}
	}//end valid main bone sys 
     //appendages
	 if(ptrLoc->List_armedInventoryDB)
	    for(POSITION armedPos = ptrLoc->List_armedInventoryDB->GetHeadPosition();armedPos != NULL;)
		{//appendage Loop
           CArmedInventoryObj* armedObj = (CArmedInventoryObj*)ptrLoc->List_armedInventoryDB->GetNext(armedPos);
		   if(armedObj->m_thirdPersonSystem)//third person
              SkeletalTextureReassignment(armedObj->m_thirdPersonSystem);
		   //first person
		   if(armedObj->m_firstPersonSystem)//third person
              SkeletalTextureReassignment(armedObj->m_firstPersonSystem);
		}//end appendage loop
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::SetMenuTextItem(int controlID, CString string)
{
   for(POSITION posLoc = m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
   {//loop through menus
      CMenu3DObj* menu = (CMenu3DObj*)m_runtimeMenuDataBase->GetNext(posLoc);
	  if(menu->m_visualItems)
		 for(POSITION visPos = menu->m_visualItems->GetHeadPosition();visPos != NULL;)
		 {//visual items loop
             CMenuVisObj * visPtr = (CMenuVisObj *)menu->m_visualItems->GetNext(visPos);
			 if(visPtr->m_textObject)
			 {//valid slider system
                if(visPtr->m_visualID == controlID)
				{//correct control ID
                     visPtr->m_textObject = string;
					return TRUE;
				}//end correct control ID
			 }//end valid slider system
		 }//end visual items loop
   }//end loop through menus
   return FALSE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::VirtualCollisionAdjust(D3DVECTOR * modPosition,
										D3DVECTOR & currentPosition,
										D3DVECTOR & lastPosition,
										int         characterWidth,
										int         characterHieght,
										D3DMATRIX & entityMatrix,
										BOOL        antiGravity)
{
	 modPosition->x = currentPosition.x;
	 modPosition->y = currentPosition.y;
	 modPosition->z = currentPosition.z;
	 float newYLoc = -7777.7771f;
     BOOL continueRepeat = TRUE;
     BOOL everColided = FALSE;	
     //dont check collision if it hasent moved
	 if(memcmp(&lastPosition,&currentPosition,sizeof(D3DVECTOR))==0)
		return FALSE;

	 int polysExactChecked=0;
     int polysConsidered=0;
     D3DVECTOR contactLocation; 
	 BOOL walkingType = TRUE;	   
		   
     float filler=0;
	 D3DVECTOR latestNormal;
	 int howManyNormals = 0;
	 D3DVECTOR directionVector;
	 directionVector.x = 0.0f;
	 directionVector.y = 0.0f;
	 directionVector.z = -1.0f;
	 directionVector = m_matLib.UpdatePolygonVertex(entityMatrix.m,directionVector);
     //collision checks
		  float highestContactLocation = newYLoc;
		  BOOL recordHitData = TRUE;
		  if(walkingType){//walking /legs
		  
          //---------------------//
		  //directional collision//
          //---------------------//
		  D3DVECTOR adjustVector;
		  adjustVector.x = 0.0f;
		  adjustVector.y = 0.0f;
		  adjustVector.z = 0.0f;

		  float charHieghtMod = characterHieght*.5f;
		  if(antiGravity)
			 charHieghtMod = 0.0f;

          D3DVECTOR travelStartVector,travelEndVector,tVector;

		  BOOL slide = FALSE;
		  D3DVECTOR slideVect;
		  slideVect.x = 0.0f;
		  slideVect.y = 0.0f;
		  slideVect.z = 0.0f;
         

		  for(int insuranceLoop = 0;insuranceLoop < 2;insuranceLoop++)
		  {//insurance loop
            travelStartVector.x = lastPosition.x;
            travelStartVector.y = lastPosition.y + charHieghtMod;
            travelStartVector.z = lastPosition.z;

            travelEndVector.x = modPosition->x;
            travelEndVector.y = modPosition->y + charHieghtMod;
		    travelEndVector.z = modPosition->z;
		   
		  tVector.x = travelEndVector.x - travelStartVector.x;
		  tVector.y = travelEndVector.y - travelStartVector.y;
		  tVector.z = travelEndVector.z - travelStartVector.z;

		  tVector = m_matLib.Normalize(tVector);
		  
		  float Ymod = (float)characterWidth;
		  if(charHieghtMod != 0.0f)
		  if(characterWidth > charHieghtMod)
			 Ymod = charHieghtMod*.9f;

		  travelEndVector.x = travelEndVector.x + (tVector.x*Ymod);
		  travelEndVector.y = travelEndVector.y + (tVector.y*Ymod);
		  travelEndVector.z = travelEndVector.z + (tVector.z*Ymod);
  
		  float filler=0.0f;

		  int objectIndexHit = 0;
		   
           if(m_collisionBase->ObjectCollisionAdjustSegment(travelStartVector,
			                                                 travelEndVector,
			                                                 travelStartVector,
                                                             travelEndVector,
			                                                 &contactLocation.x,
												             &contactLocation.y,
												             &contactLocation.z,
												             &polysExactChecked,
												             &polysConsidered,
                                                             &filler,
													         &latestNormal,
															 TRUE,
													         &adjustVector,
													         -1.0f,
													          1.0f,
															  &objectIndexHit
												             )==TRUE)
		   {//Collision valid
			 modPosition->x += adjustVector.x;
	         modPosition->y += adjustVector.y;
	         modPosition->z += adjustVector.z;
		   }//end collision valid
		  }//end insurance loop
          //---------------------//
		  //Check Leg to ground verticle after base collision 
		  //checks adjust hieght reletive to ground
		  //---------------------//
		  
		  D3DVECTOR startCollisionPath;
		  startCollisionPath.x = currentPosition.x;
		  startCollisionPath.y = lastPosition.y + charHieghtMod;
		  startCollisionPath.z = currentPosition.z;

		  if(startCollisionPath.y < (currentPosition.y + charHieghtMod))
			 startCollisionPath.y = (currentPosition.y + charHieghtMod);
 
		  adjustVector.x = 0.0f;
		  adjustVector.y = 0.0f;
		  adjustVector.z = 0.0f;

		  int objectIndexHit = 0;
		 
		   if(charHieghtMod != 0.0f)
		   if(m_collisionBase->ObjectCollisionAdjustSegment(
			                                          startCollisionPath,
			                                          *modPosition,
			                                          startCollisionPath,
                                                      *modPosition,
			                                         &contactLocation.x,
												     &contactLocation.y,
												     &contactLocation.z,
												     &polysExactChecked,
												     &polysConsidered,
                                                     &highestContactLocation,
													 &latestNormal,
													 FALSE,
													 &adjustVector,
													 .2f,
													 1.0f,
													 &objectIndexHit
												     )==TRUE)
		   {//Collision valid
			  if(modPosition->y < highestContactLocation){//1
                 modPosition->y = highestContactLocation;
			  }//end 1
		   }//end Collision valid
		  }//end walking / legs
		  m_actualPolysCalculated += polysExactChecked;
		  m_collisionPolysInUse += polysConsidered;
		  return TRUE;
}
//-------------------------------------------//
BOOL GlobalStatusVariables::DoExpEffect(int expRef,
										int networkID,
										CExplosionObjList * explosionRuntimeList,
										float pushPower,
										float range)
{
     

     CMovementObj * ptrLoc = m_entObjRuntime->GetObjByNetworkID(networkID);
	 if(!ptrLoc)
		 return FALSE;

		 D3DVECTOR entPosition;
		 ptrLoc->List_baseFrame->GetPosition(NULL,&entPosition);

		 D3DVECTOR dummyNormal;
	     dummyNormal.x = entPosition.x;//0.0f;
  	     //dummyNormal.y = entPosition.y + 1.0f;
		 dummyNormal.y = entPosition.y + (ptrLoc->List_skeletalObject->m_clipRadius * 0.5f);
	     dummyNormal.z = entPosition.z;

         CExplosionObj * expReturn = m_explosionDBList->CreateExplosion(expRef,
																		entPosition,
																		explosionRuntimeList,
																		dummyNormal,
																		ptrLoc->List_traceNumber,
																		m_3dDevice,
																		ptrLoc->m_team,
																		ptrLoc->List_traceNumber
																		); 

		 
         if(pushPower > 0.0f)
		 {//handle 
               if(expReturn)
                 if(range == 0.0f)
                     range = expReturn->m_damageRadius;

			   if(range > 0.0f)
                  m_entObjRuntime->ApplyForce(entPosition,pushPower,range);
		 }

return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::AddSelectedFromMenuList(int controlID,int glid,int quanity)
{
   for(POSITION posLoc = m_runtimeMenuDataBase->GetHeadPosition();posLoc != NULL;)
   {//loop through menus
      CMenu3DObj* menu = (CMenu3DObj*)m_runtimeMenuDataBase->GetNext(posLoc);
	  if(menu->m_visualItems)
		 for(POSITION visPos = menu->m_visualItems->GetHeadPosition();visPos != NULL;)
		 {//visual items loop
             CMenuVisObj * visPtr = (CMenuVisObj *)menu->m_visualItems->GetNext(visPos);
			 if(visPtr->m_sliderSystem)
			 {//valid slider system
				if(visPtr->m_sliderSystem->m_contentList)
                if(visPtr->m_sliderSystem->m_controlID == controlID)
				{//correct control ID
					CSliderContentObj * slObjPtr = NULL;
				    if(visPtr->m_sliderSystem->m_mode == 0)
					   slObjPtr = visPtr->m_sliderSystem->m_contentList->GetByMiscInt(glid);
					else if(visPtr->m_sliderSystem->m_mode == 1)
					   slObjPtr = visPtr->m_sliderSystem->m_contentList->GetByMiscIntQuanity(glid);
                    if(!slObjPtr)
						return FALSE;

					    int originalQuanity = slObjPtr->m_miscInt2;
						slObjPtr->m_miscInt2 += quanity;

                           CString oldString = m_cabLib.ConvToString(originalQuanity);
						   //CString newString = m_cabLib.ConvToString(slObjPtr->m_miscInt2);
                           //int diff = oldString.GetLength();
                           int strOrigLength = slObjPtr->m_text.GetLength();
						   int removeCount = oldString.GetLength();

                           slObjPtr->m_text.Delete((strOrigLength-removeCount),removeCount);
						   
                           slObjPtr->m_text = operator+(slObjPtr->m_text,m_cabLib.ConvToString(slObjPtr->m_miscInt2));

						return TRUE;
				}//end correct control ID
			 }//end valid slider system
		 }//end visual items loop
   }//end loop through menus
   return FALSE;
}
//------------------------------------------------------------------------------------//
void GlobalStatusVariables::OverideMissile(     CMovementObj     * entityPtr,
												float            & pitchRotation,
												D3DMATRIX        & positionalMatrix,
												int              & entityTraceNumber,
												BOOL               sendOverNetwork,
												float            & yawRotation,
												int                missileType,
												CMissileObjList  * missileRuntime)
{
                   
                  D3DMATRIX tempMatrix;
				  float baseKickReturn = 0.0f;
                  m_matLib.Rotate(pitchRotation,0.0f,0.0f,tempMatrix.m);
                  
                  m_matLib.fastMatrixMultiply(tempMatrix,tempMatrix,positionalMatrix);
				  
				  D3DMATRIX rotation;
                  m_matLib.Rotate(0.0f,yawRotation,0.0f,rotation.m);
				  
				  D3DVECTOR tempXYZ;
				  tempXYZ.x = tempMatrix._41;
				  tempXYZ.y = tempMatrix._42;
				  tempXYZ.z = tempMatrix._43;
				  tempMatrix._41 = 0.0f;
				  tempMatrix._42 = 0.0f;
				  tempMatrix._43 = 0.0f;

                  m_matLib.fastMatrixMultiply(tempMatrix,tempMatrix,rotation);
				  tempMatrix._41 = tempXYZ.x;
				  tempMatrix._42 = tempXYZ.y;
				  tempMatrix._43 = tempXYZ.z;
				  
				  D3DMATRIX transform;
                  m_matLib.Translate(-1.0f,0.0f,-2.0f,transform.m);
                  m_matLib.fastMatrixMultiply(tempMatrix,transform,tempMatrix);
                  tempMatrix._42 +=  entityPtr->List_skeletalObject->m_box.maxY*.7f;
		
                  //if(overrideTarget)
				  //{//ai overide
                  //  m_matLib.FaceTowardPoint(*overrideTarget,&tempMatrix);
				  //}//end ai overide

				  float missileSpeedReturn = 0.0f;
                  BOOL canShoot = TRUE;
				  if(sendOverNetwork)
				  {//is a user - check for network
                     if(m_multiplayerObject->GetEnabledClient_safe())
					 {//send network info
                       	
					   if(canShoot)
					   {
						D3DVECTOR translateAhead;
						translateAhead.x = 0;
						translateAhead.y = 0;
						translateAhead.z = -1000;
                        translateAhead = m_matLib.UpdatePolygonVertex(tempMatrix.m,translateAhead);
                        #ifndef LEGACYEA
						m_netGameLogicObject->SendFireMsg(  DPNSEND_NOCOMPLETE,
															translateAhead,
															-missileType,
															DPID_SERVERPLAYER,
															entityPtr->List_networkDefinedID);
                        #endif
                        #ifdef LEGACYEA
                        m_netGameLogicObject->SendFireMsgCP(  DPNSEND_NOCOMPLETE,
															translateAhead,
															-missileType,
															DPID_SERVERPLAYER,
															entityPtr->List_networkDefinedID);
                        #endif
					   }

					 }//end send network info
				  }//end is a user - end check for network

				  if(canShoot)
                     DoMissile( tempMatrix,
								missileType,
								0,
								entityTraceNumber,
								-9,
								m_d3dVectorInit,
								0,
								&missileSpeedReturn,
								entityPtr->m_team,
								baseKickReturn,
								missileRuntime);

                  if(entityPtr->m_aiObj)
					 entityPtr->m_aiObj->m_currentMissileSpeed = missileSpeedReturn;
}
//----------------------------------------------------------------------------
BOOL GlobalStatusVariables::ProssesCfgFile(CString filename)
{
   CFileException exc;
     CFile fl;
	 if(!m_dxRenderer)
	     return FALSE;
	 

	 int xResolution = m_dxRenderer->m_currentWidth;
	 int yResolution = m_dxRenderer->m_currentHeight;
	 int depthRes = m_dxRenderer->m_defaultDepth;

	 CArchive the_Archive(&fl,CArchive::load);
     BOOL res = fl.Open(filename,CFile::modeRead, &exc);
	 if(res == FALSE)
	    return FALSE;

	 CString tempBuffer;
     if(!the_Archive.ReadString(tempBuffer))
		 return FALSE;

	 tempBuffer.MakeUpper();
	 if(tempBuffer == "SETTINGS")
	 {//version 1

	 if(the_Archive.ReadString(tempBuffer))//X rez
		xResolution = atoi(tempBuffer);

	 if(the_Archive.ReadString(tempBuffer))//Y rez
		yResolution = atoi(tempBuffer);

	 if(the_Archive.ReadString(tempBuffer))//depth rez
		depthRes = atoi(tempBuffer);

	 if(the_Archive.ReadString(tempBuffer))//shadow type
		m_globalShadowType = atoi(tempBuffer);

	 if(the_Archive.ReadString(tempBuffer))//shadow dist
		m_globalShadowDist = (float)atoi(tempBuffer);

	 if(the_Archive.ReadString(tempBuffer))//show first person shadow
		m_showFirstPersonShadow = atoi(tempBuffer);

	 if(the_Archive.ReadString(tempBuffer))//max shadow count
		m_maxShadowCount = atoi(tempBuffer);
	 }//end version 1
	 else if(tempBuffer == "VERSION5")
	 {
      if(!the_Archive.ReadString(tempBuffer))
	      return TRUE;

      if(the_Archive.ReadString(tempBuffer))//X rez
		xResolution = atoi(tempBuffer);

      if(!the_Archive.ReadString(tempBuffer))
	      return TRUE;

	  if(the_Archive.ReadString(tempBuffer))//Y rez
		yResolution = atoi(tempBuffer);

	  if(!the_Archive.ReadString(tempBuffer))
	      return TRUE;

	  if(the_Archive.ReadString(tempBuffer))//depth rez
		depthRes = atoi(tempBuffer);

	  m_dxRenderer->m_currentWidth   = xResolution;
	  m_dxRenderer->m_currentHeight  = yResolution;
	  m_dxRenderer->m_defaultDepth   = depthRes;

	  if(!the_Archive.ReadString(tempBuffer))
	      return TRUE;

	  if(the_Archive.ReadString(tempBuffer))//enhanced mode
		m_enhancedMode = atoi(tempBuffer);

	  if(!the_Archive.ReadString(tempBuffer))
	      return TRUE;


	  if(!the_Archive.ReadString(tempBuffer))//shadow type
	      return TRUE;
	  //if(the_Archive.ReadString(tempBuffer))//shadow type
		//m_globalShadowType = atoi(tempBuffer);

	  if(!the_Archive.ReadString(tempBuffer))
	      return TRUE;

	  if(the_Archive.ReadString(tempBuffer))//shadow dist
		m_globalShadowDist = (float)atoi(tempBuffer);

	  if(!the_Archive.ReadString(tempBuffer))
	      return TRUE;

	  if(the_Archive.ReadString(tempBuffer))//show first person shadow
		m_showFirstPersonShadow = atoi(tempBuffer);

	  if(!the_Archive.ReadString(tempBuffer))
	      return TRUE;

	  if(the_Archive.ReadString(tempBuffer))//max shadow count
		m_maxShadowCount = atoi(tempBuffer);

	  if(!the_Archive.ReadString(tempBuffer))
	      return TRUE;

	  if(the_Archive.ReadString(tempBuffer))//max texture size
		 m_dxRenderer->m_maxTextureSize = atoi(tempBuffer);

	  if(!the_Archive.ReadString(tempBuffer))
	      return TRUE;

	  if(the_Archive.ReadString(tempBuffer))//compress textures
		 m_compressTextures = 1;//atoi(tempBuffer);

	  if(!the_Archive.ReadString(tempBuffer))
	      return TRUE;

	  if(the_Archive.ReadString(tempBuffer))//global render processor
		 m_globalRenderer = atoi(tempBuffer);

	  if(!the_Archive.ReadString(tempBuffer))
	      return TRUE;

	  if(the_Archive.ReadString(tempBuffer))//show names
		 this->m_showNames = atoi(tempBuffer);

	  if(!the_Archive.ReadString(tempBuffer))//vsync comment line - old cfg files end here and that is fine
	      return TRUE;

	  if(the_Archive.ReadString(tempBuffer))//vsync mode
	  {
		 m_dxRenderer->m_vsyncMode = atoi(tempBuffer);
		 if(m_dxRenderer->m_vsyncMode < 0 || m_dxRenderer->m_vsyncMode > 5)
			 m_dxRenderer->m_vsyncMode = 1;//garbage in the file -> safe default (vsync at monitor rate)
	  }

	  if(!the_Archive.ReadString(tempBuffer))//AA comment line - old cfg files end here and that is fine
	      return TRUE;

	  if(the_Archive.ReadString(tempBuffer))//multisample AA mode
	  {
		 m_dxRenderer->m_aaMode = atoi(tempBuffer);
		 if(m_dxRenderer->m_aaMode < 0 || m_dxRenderer->m_aaMode > 3)
			 m_dxRenderer->m_aaMode = 2;//garbage in the file -> safe default (4x when supported)
	  }

	  if(!the_Archive.ReadString(tempBuffer))//aniso comment line - old cfg files end here and that is fine
	      return TRUE;

	  if(the_Archive.ReadString(tempBuffer))//anisotropic filtering mode
	  {
		 m_dxRenderer->m_anisoMode = atoi(tempBuffer);
		 if(m_dxRenderer->m_anisoMode < 0 || m_dxRenderer->m_anisoMode > 4)
			 m_dxRenderer->m_anisoMode = 4;//garbage in the file -> safe default (16x when supported)
	  }

	  
	 }
	 
	 m_dxRenderer->m_currentWidth   = xResolution;
	 m_dxRenderer->m_currentHeight  = yResolution;
	 m_dxRenderer->m_defaultDepth   = depthRes;

     the_Archive.Close();
	 fl.Close();
   return TRUE;
}
//----------------Save control cnf list-----------------------------------------------//
void GlobalStatusVariables::SaveControlConfig(CString fileName) 
{
     SetCurrentDirectory(GL_pathGameFiles);//use gamefiles folder for organization

     CFileException exc;
     CFile fl;
     BOOL result = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
     if(result)
	 {
		 CArchive the_out_Archive(&fl,CArchive::store);
		 the_out_Archive << m_controlsList;
		 the_out_Archive.Close();
		 fl.Close();	
	 }
	 SetCurrentDirectory(GL_workingDirectory);	
}
//------------------------------------------------------------------------------------//
int GlobalStatusVariables::BonesSystemRender(   CSkeletonObject * sklPtr,
												D3DVECTOR offsetVector,
												D3DMATRIX positionMatrix)
{
	if(!sklPtr->m_bonesList)
		return 0;
	//Hiarchy items
	for(POSITION posLoc = sklPtr->m_bonesList->GetHeadPosition();posLoc != NULL;)
	{
		CBoneObject * bone = (CBoneObject *)sklPtr->m_bonesList->GetNext(posLoc);
		//mesh hiarchle anim
		if(!bone->m_hiarchyMesh)
			continue;
		
		CHiarcleVisObj * vPtr = bone->m_hiarchyMesh->GetByIndex(sklPtr->m_currentLOD);
		if(!vPtr)
			continue;
		
		D3DMATRIX boneMeshMatrix;
		bone->m_boneFrame->GetTransform(NULL,&boneMeshMatrix);

		m_matLib.fastMatrixMultiply(boneMeshMatrix,boneMeshMatrix,positionMatrix);				   
				  
		vPtr->m_mesh->m_currentMatrix = boneMeshMatrix;

		HighLevelMeshRender(vPtr->m_mesh,offsetVector,FALSE);	
	}	            
	//end hiarchy items
	//begin deformable
	for(int mCount=0;mCount<sklPtr->m_deformableMeshArrayCount;mCount++)
	{
		POSITION vLpos = sklPtr->m_deformableMeshArrayV2[mCount].m_defMeshVisualUnit->m_lodChain->FindIndex(sklPtr->m_currentLOD);
		if(!vLpos)
			continue;

		CDeformableVisObj * dVisPtr = (CDeformableVisObj *)sklPtr->m_deformableMeshArrayV2[mCount].m_defMeshVisualUnit->m_lodChain->GetAt(vLpos);
		if(!dVisPtr)
			continue;

		D3DMATRIX skinMatrix = m_matrixInit2;
		m_matLib.fastMatrixMultiply(skinMatrix,positionMatrix,skinMatrix);
				  
		dVisPtr->m_mesh->m_currentMatrix = skinMatrix;
		HighLevelMeshRender(dVisPtr->m_mesh,offsetVector,FALSE);		
	}
	//end render skin items
            
	sklPtr->m_requireUpdate = FALSE;
	return 1;
}
//------------------------------------------------------------------------------------//
void GlobalStatusVariables::HighLevelMeshRender(CEXMeshObj * meshex,
											    D3DVECTOR & locationVect,
											    BOOL mirrorEnable)
{
	D3DMATRIX matrixTempLocal = meshex->m_currentMatrix;
	matrixTempLocal._41 = matrixTempLocal._41 - locationVect.x;
	matrixTempLocal._42 = matrixTempLocal._42 - locationVect.y;
	matrixTempLocal._43 = matrixTempLocal._43 - locationVect.z;
	
	/*if(mirrorEnable)
	m_matLib.fastMatrixMultiply(matrixTempLocal,matrixTempLocal,GL_MirrorTransform);
	m_renderStateObject->SetWorldMatrix((D3DXMATRIX*)&matrixTempLocal,m_3dDevice);*/
	m_renderStateObject->SetPxShader(m_3dDevice,m_pxShaderSystem,meshex->List_materialObject->m_pixelShader);
	m_renderStateObject->SetShader(m_3dDevice,m_shaderSystem,meshex->List_materialObject->m_vertexShader);
	
	if(meshex->List_materialObject)
	{
		meshex->List_materialObject->Callback();
		m_renderStateObject->SetFogOverrideState(meshex->List_materialObject->m_fogOverride,m_3dDevice);
		m_renderStateObject->SetZBias(m_3dDevice,meshex->List_materialObject->m_zBias);
		m_renderStateObject->SetBlendType(m_3dDevice,meshex->List_materialObject->m_blendMode);
		m_renderStateObject->SetFogFilter(m_3dDevice,meshex->List_materialObject->m_fogFilter);
		m_renderStateObject->SetCullState(meshex->List_materialObject->m_cullOn,m_3dDevice);
		m_renderStateObject->SetWireFillState(meshex->List_materialObject->m_forceWireFrame,m_3dDevice);
		m_renderStateObject->SetMaterial(m_3dDevice,&meshex->List_materialObject->m_useMaterial);
		m_renderStateObject->SetTextureTileMode(meshex->List_materialObject->m_textureTileType,m_3dDevice);
	}

	if(!meshex->m_abVertexArray.m_indicieBuffer)
		meshex->InitBuffer(m_3dDevice,m_cgGlobalShaderDB->GetByIndex(meshex->List_materialObject->m_optionalShaderIndex-1));
	
	MultipassRenderMesh(meshex,0,matrixTempLocal);
	GL_objectsRendered++;
}
//-------------------multipass rendering----------------------------------------------//
void GlobalStatusVariables::MultipassRenderMesh(CEXMeshObj * mesh,int lod, D3DMATRIX & wldMatrix)
{
	m_renderStateObject->SetTextureState(mesh->List_textureSelect[0],m_renderStateObject->m_intZero,m_3dDevice,m_textureDataBase);	
	if(mesh->List_BlendMethod[9]<0)
	   m_renderStateObject->SetBlendLevel(m_renderStateObject->m_intZero,m_renderStateObject->m_intZero,m_3dDevice);
	else
	   m_renderStateObject->SetBlendLevel(m_renderStateObject->m_intZero,mesh->List_BlendMethod[9],m_3dDevice);



	int loop=0;
	for(loop=1;loop<m_renderStateObject->m_multiTextureSupported;loop++){
        m_renderStateObject->SetTextureState(mesh->List_textureSelect[loop],loop,m_3dDevice,m_textureDataBase);	
		m_renderStateObject->SetBlendLevel(loop,mesh->List_BlendMethod[(loop-1)],m_3dDevice);
		if(loop>7)
		   break;
	}

	for(loop=0;loop<8;loop++)
    {
	        m_renderStateObject->SetColorArg1Level(loop,mesh->List_materialObject->m_colorArg1[loop],m_3dDevice);
            m_renderStateObject->SetColorArg2Level(loop,mesh->List_materialObject->m_colorArg2[loop],m_3dDevice);
			m_renderStateObject->SetTexcoordIndexLevel(loop,mesh->List_materialObject->m_texcoordLevels[loop],m_3dDevice);
    } 
	
	m_renderStateObject->SetDiffuseSource(mesh->m_abVertexArray.m_advancedFixedMode,m_3dDevice);
    m_renderStateObject->SetVertexType(mesh->m_abVertexArray.m_uvCount,m_3dDevice);
	float convert = m_timeAdjustFloat;
	mesh->TextureMovementCallback(convert,m_renderStateObject->m_currentVertexType);
	
    m_renderStateObject->SetWorldMatrix((D3DXMATRIX*)&wldMatrix,m_3dDevice);

	mesh->Render(m_3dDevice,m_renderStateObject->m_currentVertexType,mesh->m_chromeUvSet);
}
//-----------------------------------------------------------------------------
BOOL GlobalStatusVariables::MakeDirectSound(HWND hwnd,IDirectSound8 **lpds)
{
    if FAILED(DirectSoundCreate8(NULL,lpds, NULL))         
	   return FALSE; 
    
	if FAILED(IDirectSound_SetCooperativeLevel(*lpds, hwnd, DSSCL_PRIORITY))        
	   return FALSE;


	   DWORD dwPrimaryChannels		= 2;
       DWORD dwPrimaryFreq			= 44100;
       DWORD dwPrimaryBitRate		= 16;

				HRESULT hr;
	            IDirectSound8 * localDs = *lpds;
				LPDIRECTSOUNDBUFFER lpDSBuff;
                DSBUFFERDESC dsbd;
				memset(&dsbd, 0, sizeof(DSBUFFERDESC));
				dsbd.dwSize = sizeof(DSBUFFERDESC);
				dsbd.dwFlags = DSBCAPS_PRIMARYBUFFER | DSBCAPS_CTRL3D;
				if(localDs->CreateSoundBuffer(&dsbd, &lpDSBuff, NULL)==0){	
					//Make the primary 44.1 KHz so that it sounds better
					WAVEFORMATEX wfx;
					ZeroMemory( &wfx, sizeof(WAVEFORMATEX) );
					wfx.wFormatTag = WAVE_FORMAT_PCM;
					/*wfx.nChannels = 1;//2;
					wfx.nSamplesPerSec = 22050;//44100
					wfx.nAvgBytesPerSec = 22050*2;//22050*2*2;
					wfx.nBlockAlign = 2;//4;
					wfx.wBitsPerSample = 16;
					wfx.cbSize = 0;*/
					wfx.nChannels       = (WORD) dwPrimaryChannels;
					wfx.nSamplesPerSec  = (DWORD) dwPrimaryFreq;
					wfx.wBitsPerSample  = (WORD) dwPrimaryBitRate;
					wfx.nBlockAlign     = (WORD) (wfx.wBitsPerSample / 8 * wfx.nChannels);
					wfx.nAvgBytesPerSec = (DWORD) (wfx.nSamplesPerSec * wfx.nBlockAlign);

					HRESULT err = lpDSBuff->SetFormat(&wfx);
					if(err != 0)
					{
                       AfxMessageBox("dmusic fmt set failed");
					}

					if( FAILED( hr = lpDSBuff->QueryInterface( IID_IDirectSound3DListener,(VOID**)&m_soundListener ) ) )
					{
						return FALSE;
					}
				}

return TRUE;
}
//------------------------------------------------------------------------
BOOL GlobalStatusVariables::RunScript(CString fileName)
{
     SetCurrentDirectory(GL_workingDirectory);
	 m_axLogger.WriteDebugFile("RunScript():",0,1);
	 m_axLogger.WriteDebugFile(fileName.GetBuffer(),0,1);
	 CEXScriptObjList * scriptList;
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		m_axLogger.WriteDebugFile(("RunScript() can not find script file!!:"+fileName).GetBuffer(),0,1);
		SetCurrentDirectory(GL_workingDirectory);
		return FALSE;
	 }
	 // Any MFC exception here (bad archive, missing data file, out of memory)
	 // used to escape straight out of WM_CREATE and silently kill the app.
	 // Log it to GeneralClientLog.txt and keep the editor alive instead.
	 try
	 {
		 CArchive the_in_Archive(&fl,CArchive::load);
		 the_in_Archive >> scriptList;
		 the_in_Archive.Close();
		 fl.Close();
		 if(!scriptList || scriptList->GetCount() == 0)
		 {//a 0/2-byte or otherwise empty .aut: nothing would be loaded and the editor would sit there with no data
			 m_axLogger.WriteDebugFile(("RunScript() script file is EMPTY or corrupt (no script objects): "+fileName).GetBuffer(),0,1);
			 AfxMessageBox("The script file is empty or corrupt (it contains no script entries):\n\n" + fileName +
			               "\n\nRestore it from a good copy (e.g. EA_dev.aut) and try again.", MB_ICONERROR);
			 DeleteScriptList(&scriptList);
			 SetCurrentDirectory(GL_workingDirectory);
			 return FALSE;
		 }
		 //RUN DATA SCRIPT
		 m_axLogger.WriteDebugFile("ExecAllAttributes()",0,1);
		 ExecAllAttributes(scriptList);
		 DeleteScriptList(&scriptList);
	 }
	 catch(CException* e)
	 {
		 TCHAR msg[512] = {0};
		 e->GetErrorMessage(msg, 512);
		 CRuntimeClass* rc = e->GetRuntimeClass();
		 CString line; line.Format("RunScript() MFC EXCEPTION %s: %s", rc ? rc->m_lpszClassName : "CException", msg);
		 m_axLogger.WriteDebugFile(line.GetBuffer(),0,1);
		 AfxMessageBox(line + "\n\nScript: " + fileName + "\nSee GeneralClientLog.txt", MB_ICONERROR);
		 e->Delete();
		 SetCurrentDirectory(GL_workingDirectory);
		 return FALSE;
	 }
	 //set data binds
	 SetCurrentDirectory(GL_workingDirectory);
return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL GlobalStatusVariables::ConnectToServerNode(CString serverIP)
{
        m_multiplayerObject->Disconnect();
		CacheGeneratedRenderStuff(0);//clear
		DestroyEntityRuntimeList();//clear
		
		Sleep(2500);
		int attemptCount = 0;
		m_multiplayerObject->m_IPaddress = serverIP;
		m_multiplayerObject->EnableClient(m_multiplayerObject->m_IPaddress,m_multiplayerObject->m_currentVersion);
		Sleep(2500);
		while(!m_multiplayerObject->GetEnabledClient_safe())
		{
          Sleep(1000);
		  attemptCount++;
		  if(attemptCount > 25)//5 seconds
		     return FALSE;

		  if(m_multiplayerObject->GetConnectionStatus() == 3)
		  {//retry
			 m_multiplayerObject->Disconnect();
             Sleep(3000);
			 m_multiplayerObject->EnableClient(m_multiplayerObject->m_IPaddress,m_multiplayerObject->m_currentVersion);
		  }//end retry
		}
 
		return TRUE;
}
//--------------checks Collision between a world object and a entity------------------//
BOOL GlobalStatusVariables::RealtimeSegmentTest(D3DVECTOR & lastPos,
												D3DVECTOR & curPos,
												D3DVECTOR * normalRet,
												D3DVECTOR * collisionIntersect,
												int * actualCollisions,
												int * consideredCollision)
{
	 BOOL tallyPolyColCount = TRUE;
     int polysExactChecked = 0;
     int polysConsidered = 0;
     int ObjectIndexOfCollision = 0;
     BOOL result = FALSE;
 
	 

	  if(!m_multiplayerObject->m_AIServer)
	  {//use cache for client

	   
       for(POSITION hPos = m_houseingCache->GetHeadPosition();hPos != NULL;)
	   {//Houseing
          CHousingCacheObj * hsPtr = (CHousingCacheObj *)m_houseingCache->GetNext(hPos);
	      CHousingObj * houseLib = m_housingDB->GetByIndex(hsPtr->m_dbIndex);
          if(houseLib)
		  {
			   if(hsPtr->m_open)
			   {//open handle
		        if(houseLib->m_openState->CollisionSegmentSItranslated(
					                     lastPos,
					                     curPos,
					                     &collisionIntersect->x,
					                     &collisionIntersect->y,
					                     &collisionIntersect->z,
					                     &polysExactChecked,
					                     &polysConsidered,
					                     normalRet,
					                     &ObjectIndexOfCollision,
					                     20.0f,
					                     TRUE,
									     hsPtr->m_location,
									     hsPtr->m_invLocation))
				{


                   *actualCollisions = polysExactChecked;
	               *consideredCollision = polysConsidered;

		           return TRUE;
				}
			   }//end open handle
			   else{//closed handle
                if(houseLib->m_closedState->CollisionSegmentSItranslated(
					                     lastPos,
					                     curPos,
					                     &collisionIntersect->x,
					                     &collisionIntersect->y,
					                     &collisionIntersect->z,
					                     &polysExactChecked,
					                     &polysConsidered,
					                     normalRet,
					                     &ObjectIndexOfCollision,
					                     20.0f,
					                     TRUE,
									     hsPtr->m_location,
									     hsPtr->m_invLocation))
				{


                   *actualCollisions = polysExactChecked;
	               *consideredCollision = polysConsidered;

		           return TRUE;
				}
			   }//end closed handle
		  }
	   }//end Houseing
	  }//end use cache for client
	  else{//use tabled mode
       if(!m_multiplayerObject->m_houseingDB)
	       return FALSE;

       CHPlacementObjList * placementList = m_multiplayerObject->m_houseingDB->GetContainer(curPos);
       if(placementList)
        for(POSITION hPos = placementList->GetHeadPosition();hPos != NULL;)
		{//Houseing
         CHPlacementObj * hsPtr = (CHPlacementObj *)placementList->GetNext(hPos);
	     CHousingObj * houseLib = m_housingDB->GetByIndex(hsPtr->m_type);
         if(houseLib)
		 {
		   
		 D3DMATRIX regMatrix,inverseMatrix;
         m_matLib.InitMat(regMatrix.m);
		 m_matLib.InitMat(inverseMatrix.m);

		 inverseMatrix._41 = -hsPtr->m_wldPosition.x;
		 inverseMatrix._42 = -hsPtr->m_wldPosition.y;
		 inverseMatrix._43 = -hsPtr->m_wldPosition.z;
		 regMatrix._41 = hsPtr->m_wldPosition.x;
		 regMatrix._42 = hsPtr->m_wldPosition.y;
		 regMatrix._43 = hsPtr->m_wldPosition.z;
		    
		  if(houseLib->m_openState->CollisionSegmentSItranslated(
					                     lastPos,
					                     curPos,
					                     &collisionIntersect->x,
					                     &collisionIntersect->y,
					                     &collisionIntersect->z,
					                     &polysExactChecked,
					                     &polysConsidered,
					                     normalRet,
					                     &ObjectIndexOfCollision,
					                     20.0f,
					                     TRUE,
									     regMatrix,
									     inverseMatrix))
		  {


           *actualCollisions = polysExactChecked;
	       *consideredCollision = polysConsidered;
		   return TRUE;
		  }
		 }
		}//end Houseing
	  }//end use tabled mode
	 
	 return FALSE;
}
//--------------checks Collision between a world object and a entity------------------//
BOOL GlobalStatusVariables::RealtimeSegmentTestUseTable(D3DVECTOR & lastPos,D3DVECTOR & curPos)
{
       if(!m_multiplayerObject->m_houseingDB)
		   return FALSE;
 

	   D3DVECTOR curPosTemp,lastPosTemp;
	   CHPlacementObjList * placementList = m_multiplayerObject->m_houseingDB->GetContainer(curPos);
       if(placementList)
       for(POSITION hPos = placementList->GetHeadPosition();hPos != NULL;)
	   {//Houseing
         CHPlacementObj * hsPtr = (CHPlacementObj *)placementList->GetNext(hPos);
	     CHousingObj * houseLib = m_housingDB->GetByIndex(hsPtr->m_type);
         if(houseLib)
		 {
		   
		 curPosTemp.x = curPos.x - hsPtr->m_wldPosition.x;
         curPosTemp.y = curPos.y - hsPtr->m_wldPosition.y;
		 curPosTemp.z = curPos.z - hsPtr->m_wldPosition.z;
         lastPosTemp.x = lastPos.x - hsPtr->m_wldPosition.x;
         lastPosTemp.y = lastPos.y - hsPtr->m_wldPosition.y;
		 lastPosTemp.z = lastPos.z - hsPtr->m_wldPosition.z;


		 if(houseLib->m_openState)
	        if(houseLib->m_openState->CollisionSegmentSIBasic(lastPosTemp,curPosTemp,20.0f))
			{
		       return TRUE;
			}
		 }
	   }//end Houseing

	 return FALSE;
}
//-------------3D sound callback------------------------------------------------------//
void GlobalStatusVariables::Sound3DCallback()
{
		//APPLY SOUND CENTER TO FIRST ENTITY
		/*POSITION posLoc = m_entObjRuntime->FindIndex(0);
		if(!posLoc)
		return;
		//set focus
		CMovementObj * focusLoc = ( CMovementObj* ) m_entObjRuntime->GetAt(posLoc);
		//adjust sound
		focusLoc->List_baseFrame->GetTransform(NULL,&m_soundCenterMatrix);*/
}
/*----------------------------------------------------------------//
CString GlobalStatusVariables::ParseData(unsigned int & msg)
{
   SHORT shiftState = GetKeyState(VK_SHIFT);
   BOOL capitalOption = FALSE;
   BOOL shiftOn = FALSE;
   if(shiftState != 0 && shiftState != 1){
      capitalOption = TRUE;
      shiftOn = TRUE;
   }
   else if(GetKeyState(VK_CAPITAL))
	  capitalOption = TRUE;


   switch(msg)
   {

	   case 'A':
		 if(capitalOption)
            return "A";
		 else
            return "a";

	   case 'B':
	     if(capitalOption)
            return "B";
		 else
            return "b";

	   case 'C':
	     if(capitalOption)
            return "C";
		 else
            return "c";

	   case 'D':
	     if(capitalOption)
            return "D";
		 else
            return "d";

	   case 'E':
	     if(capitalOption)
            return "E";
		 else
            return "e";
  
       case 'F':
	     if(capitalOption)
            return "F";
		 else
            return "f";
 
       case 'G':
	     if(capitalOption)
            return "G";
		 else
            return "g";

	   case 'H':
	     if(capitalOption)
            return "H";
		 else
            return "h";

	   case 'I':
	     if(capitalOption)
            return "I";
		 else
            return "i";
	
	   case 'J':
	     if(capitalOption)
            return "J";
		 else
            return "j";

	   case 'K':
	     if(capitalOption)
            return "K";
		 else
            return "k";

	   case 'L':
	     if(capitalOption)
            return "L";
		 else
            return "l";
	
	   case 'M':
	     if(capitalOption)
            return "M";
		 else
            return "m";

	   case 'N':
	     if(capitalOption)
            return "N";
		 else
            return "n";

	   case 'O':
	     if(capitalOption)
            return "O";
		 else
            return "o";
	
	   case 'P':
	     if(capitalOption)
            return "P";
		 else
            return "p";

	   case 'Q':
	     if(capitalOption)
            return "Q";
		 else
            return "q";

	   case 'R':
	     if(capitalOption)
            return "R";
		 else
            return "r";

	   case 'S':
	     if(capitalOption)
            return "S";
		 else
            return "s";
	
	   case 'T':
	     if(capitalOption)
            return "T";
		 else
            return "t";

	   case 'U':
	     if(capitalOption)
            return "U";
		 else
            return "u";

	   case 'V':
	     if(capitalOption)
            return "V";
		 else
            return "v";
	
	   case 'W':
	     if(capitalOption)
            return "W";
		 else
            return "w";

	   case 'X':
	     if(capitalOption)
            return "X";
		 else
            return "x";

	   case 'Y':
	     if(capitalOption)
            return "Y";
		 else
            return "y";

	   case 'Z':
	     if(capitalOption)
            return "Z";
		 else
            return "z";

       case '1':
	     if(shiftOn)
            return "!";
		 else
            return "1";

	   case '2':
	     if(shiftOn)
            return "@";
		 else
            return "2";

	   case '3':
	     if(shiftOn)
            return "#";
		 else
            return "3";

	   case '4':
	     if(shiftOn)
            return "$";
		 else
            return "4";

	   case '5':
	     if(shiftOn)
            return "%";
		 else
            return "5";

	   case '6':
	     if(shiftOn)
            return "^";
		 else
            return "6";

	   case '7':
	     if(shiftOn)
            return "&";
		 else
            return "7";

	   case '8':
	     if(shiftOn)
            return "*";
		 else
            return "8";

	   case '9':
	     if(shiftOn)
            return "(";
		 else
            return "9";

	   case '0':
	     if(shiftOn)
            return ")";
		 else
            return "0";

	   case 189:
		   if(shiftOn)
            return "_";
		 else
            return "-";

	   case 187:
	     if(shiftOn)
            return "+";
		 else
            return "=";


	   case VK_ADD:
          return "+";

       case VK_SUBTRACT:
          return "_";

	   
	   case VK_DIVIDE:
          return "/";


	   case VK_MULTIPLY:
          return "*";
		  
	   
	   
	   case 219:
		   if(shiftOn)
            return "{";
		 else
            return "[";


	   case 221:
	     if(shiftOn)
            return "}";
		 else
            return "]";

	   case 220:
		   if(shiftOn)
            return "|";
		 else
            return "\\";


	   case 186:
	     if(shiftOn)
            return ":";
		 else
            return ";";

	   case 222:
		if(shiftOn)
		   return (CString)'"';
		else
           return "'";

	   case 188:
		if(shiftOn)
		   return "<";
		else
           return ",";
		 
	   case 190:
		if(shiftOn)
		   return ">";
		else
           return ".";

	   case 191:
		if(shiftOn)
		   return "?";
		else
           return "/";

	   case 192:
		if(shiftOn)
		   return "~";
		else
           return "`";

	   case 32:
		return " ";

   }

   return "";

}*/

/*//--------------Collision between entities--------------------------------------------//
BOOL GlobalStatusVariables::SegmentToEntity(D3DVECTOR & startPoint,
											D3DVECTOR & endPoint,
											CSkeletonObject * skeletalCmpare,
											MatrixARB * mathLib,
											D3DVECTOR * intersectRet,
											D3DMATRIX * boneMatrix,
											CFrameObj ** frameObject,
											D3DMATRIX & compareBaseMatrix,
											D3DVECTOR & latestNormal,
											float & dist
										)
{
    D3DVECTOR virtualNormal,invVirtualNormal,v1,v2,normalTemp,invlegRayStart,invlegRayEnd,intersect;
	D3DMATRIX wldBoneMatrix,invBoneMatrix;
    D3DVECTOR polygon[3];
	BOOL doAdjust = FALSE;
	float tempDist = 0;
	float angle = 0.0f;
	int iLoop = 0;
	int mCount = 0;


	//dynamically generate virtual directon based on segment-used to eliminate backface collisions by degree
    virtualNormal.x = endPoint.x - startPoint.x;
    virtualNormal.y = endPoint.y - startPoint.y;
    virtualNormal.z = endPoint.z - startPoint.z;
	virtualNormal = mathLib->Normalize(virtualNormal);
	dist = 1000000;
	
	if(!skeletalCmpare)
		return FALSE;

    if(skeletalCmpare->m_bonesList)
           for(POSITION posLoc = skeletalCmpare->m_bonesList->GetHeadPosition();posLoc != NULL;)
		   {
               CBoneObject * bonePtr = (CBoneObject *) skeletalCmpare->m_bonesList->GetNext(posLoc);
	           if(bonePtr->m_hiarchyMesh)
			   if(!bonePtr->m_hiarchyMesh->m_collisionFiltered)//see if collision should even be checked by this thing
			   {
                   CHiarcleVisObj * hObj = bonePtr->m_hiarchyMesh->GetByIndex(0);
                   if(hObj)
				   if(hObj->m_mesh)
				   { 
				      bonePtr->m_boneFrame->GetTransform(NULL,&wldBoneMatrix);//put ray into corrective space
				      mathLib->fastMatrixMultiply(wldBoneMatrix,wldBoneMatrix,compareBaseMatrix);
					  mathLib->D3DMath_MatrixInvert(invBoneMatrix,wldBoneMatrix);//requires full inverse cannot use fast calc matrix inv2
					  
					  invlegRayStart = mathLib->UpdatePolygonVertex(invBoneMatrix.m,startPoint);
                      invlegRayEnd   = mathLib->UpdatePolygonVertex(invBoneMatrix.m,endPoint);
					  invVirtualNormal  = mathLib->UpdatePolygonVertexRotationOnly(invBoneMatrix.m,virtualNormal);
					  
			          for(iLoop=0;iLoop < hObj->m_mesh->m_abVertexArray.m_indecieCount;iLoop+=3)
					  {//check polygons
                          hObj->m_mesh->m_abVertexArray.GetPolygon(iLoop,polygon);
                       
                          if(mathLib->SegPolyCheck(polygon,invlegRayStart,invlegRayEnd,&intersect))
						  {//collided
						     //generate normal
                             v1.x = polygon[1].x - polygon[0].x;
							 v1.y = polygon[1].y - polygon[0].y;
							 v1.z = polygon[1].z - polygon[0].z;
							 v2.x = polygon[2].x - polygon[0].x;
							 v2.y = polygon[2].y - polygon[0].y;
							 v2.z = polygon[2].z - polygon[0].z;
							 normalTemp = mathLib->Normalize(mathLib->CrossProduct(v1,v2));
							  
							 angle = mathLib->GetAngleBetweenTwoVectorsFast(invVirtualNormal,normalTemp);
		                     if(angle < 89)
		                        continue;
 
                             tempDist = mathLib->MagnitudeSquared(invlegRayEnd.x,invlegRayEnd.y,invlegRayEnd.z,intersect.x,intersect.y,intersect.z);
                             if(tempDist < dist)
							 {
                                CopyMemory(intersectRet,&intersect,sizeof(D3DVECTOR));
                                dist = tempDist;
								doAdjust = TRUE;
								*boneMatrix			= wldBoneMatrix;
								*frameObject		= bonePtr->m_boneFrame;
								latestNormal		= normalTemp;
							 }
						  }//end collided
					  }//end check polygons
				   }
			   }
		   }
		   //Deformable meshs
		   if(skeletalCmpare->m_deformableMeshArrayCount > 0)
		   {//valid def count
		    mathLib->D3DMath_MatrixInvert(invBoneMatrix,compareBaseMatrix);//only needs one basis of transform-this is good news
		    invlegRayStart = mathLib->UpdatePolygonVertex(invBoneMatrix.m,startPoint);
		    invlegRayEnd   = mathLib->UpdatePolygonVertex(invBoneMatrix.m,endPoint);
		    invVirtualNormal  = mathLib->UpdatePolygonVertexRotationOnly(invBoneMatrix.m,virtualNormal);
			   for(mCount=0;mCount<skeletalCmpare->m_deformableMeshArrayCount;mCount++)
			   {
					   POSITION vLpos = skeletalCmpare->m_deformableMeshArrayV2[mCount].m_defMeshVisualUnit->m_lodChain->FindIndex(0);
					   if(!vLpos)
						  continue;

					   CDeformableVisObj * dVisPtr = (CDeformableVisObj *)skeletalCmpare->m_deformableMeshArrayV2[mCount].m_defMeshVisualUnit->m_lodChain->GetAt(vLpos);
					   if(!dVisPtr)
						   continue;

					   //dVisPtr->m_mesh
                       for(iLoop=0;iLoop < dVisPtr->m_mesh->m_abVertexArray.m_indecieCount;iLoop+=3)
					   {//check polygons
                          dVisPtr->m_mesh->m_abVertexArray.GetPolygon(iLoop,polygon);
                       
                          if(mathLib->SegPolyCheck(polygon,invlegRayStart,invlegRayEnd,&intersect))
						  {//collided
						     //generate normal
                             v1.x = polygon[1].x - polygon[0].x;
							 v1.y = polygon[1].y - polygon[0].y;
							 v1.z = polygon[1].z - polygon[0].z;
							 v2.x = polygon[2].x - polygon[0].x;
							 v2.y = polygon[2].y - polygon[0].y;
							 v2.z = polygon[2].z - polygon[0].z;
							 normalTemp = mathLib->Normalize(mathLib->CrossProduct(v1,v2));
							  
							 angle = mathLib->GetAngleBetweenTwoVectorsFast(invVirtualNormal,normalTemp);
		                     if(angle < 89)
		                        continue;
 
                             tempDist = mathLib->MagnitudeSquared(invlegRayEnd.x,invlegRayEnd.y,invlegRayEnd.z,intersect.x,intersect.y,intersect.z);
                             if(tempDist < dist)
							 {
                                CopyMemory(intersectRet,&intersect,sizeof(D3DVECTOR));
                                dist = tempDist;
								doAdjust = TRUE;
								*boneMatrix			= compareBaseMatrix;
								//*frameObject		= bonePtr->m_boneFrame;
								latestNormal		= normalTemp;
							 }
						  }//end collided
					   }//end check polygons
			   }
		   }//end valid def count

		   if(doAdjust)
		      return TRUE;

		   return FALSE;
}*/

//--------------checks Collision between a world object and a entity------------------//
bool GlobalStatusVariables::RealtimeHousingSegment( D3DVECTOR & lastPos,
												    D3DVECTOR & curPos,
												    D3DVECTOR * normalRet,
												    D3DVECTOR * collisionIntersect,
													float & bestDistance)
{
	  float tempDist = 0.0f;
	  float dist = 0.0f;
	  int objectHit = 0;
	  D3DVECTOR lastP,curP,intersect;
	  byte temp = 0;
	  if(!m_multiplayerObject->m_AIServer)
	  {//use cache for client

       for(POSITION hPos = m_houseingCache->GetHeadPosition();hPos != NULL;)
	   {//Houseing
          CHousingCacheObj * hsPtr = (CHousingCacheObj *)m_houseingCache->GetNext(hPos);
	      CHousingObj * houseLib = m_housingDB->GetByIndex(hsPtr->m_dbIndex);
          if(houseLib)
		  {
			  lastP.x = lastPos.x + hsPtr->m_invLocation._41;
		      lastP.y = lastPos.y + hsPtr->m_invLocation._42;
		      lastP.z = lastPos.z + hsPtr->m_invLocation._43;
		      curP.x = curPos.x + hsPtr->m_invLocation._41;
		      curP.y = curPos.y + hsPtr->m_invLocation._42;
		      curP.z = curPos.z + hsPtr->m_invLocation._43;
			   if(hsPtr->m_open)
			   {//open handle
          
		        
 
		        if(houseLib->m_openState->GetClosesIntersectionUseSeg(
																		lastP,
																		curP,
																		lastP,
																		curP,
																		&intersect.x,
																		&intersect.y,
																		&intersect.z,
																		&objectHit,
																		normalRet,
																		false,
																		dist,
																		temp
																		))
				{ 
				   intersect.x +=  hsPtr->m_location._41;
				   intersect.y +=  hsPtr->m_location._42;
				   intersect.z +=  hsPtr->m_location._43;
				   tempDist = m_matLib.MagnitudeSquared(lastPos.x,lastPos.y,lastPos.z,intersect.x,intersect.y,intersect.z);
                   if(tempDist < bestDistance)
 				   {
				      
                      
                      CopyMemory(collisionIntersect,&intersect,sizeof(D3DVECTOR));


                      bestDistance = tempDist;
		              return true;
				   }
				}
			   }//end open handle
			   else{//closed handle
                if(houseLib->m_closedState->GetClosesIntersectionUseSeg(
																		lastP,
																		curP,
																		lastP,
																		curP,
																		&intersect.x,
																		&intersect.y,
																		&intersect.z,
																		&objectHit,
																		normalRet,
																		false,
																		dist,
																		temp
																		))
				{
				   intersect.x +=  hsPtr->m_location._41;
				   intersect.y +=  hsPtr->m_location._42;
				   intersect.z +=  hsPtr->m_location._43;
		           tempDist = m_matLib.MagnitudeSquared(lastPos.x,lastPos.y,lastPos.z,intersect.x,intersect.y,intersect.z);
                   if(tempDist < bestDistance)
				   {
                      
                      CopyMemory(collisionIntersect,&intersect,sizeof(D3DVECTOR));

                      bestDistance = tempDist;
		              return true;
				   }
				}
			   }//end closed handle
		  }
	   }//end Houseing
	  }//end use cache for client
	  else{//use tabled mode
       if(!m_multiplayerObject->m_houseingDB)
	       return false;

       CHPlacementObjList * placementList = m_multiplayerObject->m_houseingDB->GetContainer(curPos);
       if(placementList)
        for(POSITION hPos = placementList->GetHeadPosition();hPos != NULL;)
		{//Houseing
         CHPlacementObj * hsPtr = (CHPlacementObj *)placementList->GetNext(hPos);
	     CHousingObj * houseLib = m_housingDB->GetByIndex(hsPtr->m_type);
         if(houseLib)
		 {
		   
		 /*D3DMATRIX regMatrix,inverseMatrix;
         m_matLib.InitMat(regMatrix.m);
		 m_matLib.InitMat(inverseMatrix.m);

		 inverseMatrix._41 = -hsPtr->m_wldPosition.x;
		 inverseMatrix._42 = -hsPtr->m_wldPosition.y;
		 inverseMatrix._43 = -hsPtr->m_wldPosition.z;
		 regMatrix._41 = hsPtr->m_wldPosition.x;
		 regMatrix._42 = hsPtr->m_wldPosition.y;
		 regMatrix._43 = hsPtr->m_wldPosition.z;*/

		 lastP.x = lastPos.x - hsPtr->m_wldPosition.x;
	     lastP.y = lastPos.y - hsPtr->m_wldPosition.y;
	     lastP.z = lastPos.z - hsPtr->m_wldPosition.z;
		 curP.x = curPos.x - hsPtr->m_wldPosition.x;
		 curP.y = curPos.y - hsPtr->m_wldPosition.y;
		 curP.z = curPos.z - hsPtr->m_wldPosition.z;
		    
		  
		  if(houseLib->m_openState->GetClosesIntersectionUseSeg(
																		lastP,
																		curP,
																		lastP,
																		curP,
																		&intersect.x,
																		&intersect.y,
																		&intersect.z,
																		&objectHit,
																		normalRet,
																		false,
																		dist,
																		temp
																		))
		  {
			       intersect.x +=  hsPtr->m_wldPosition.x;
				   intersect.y +=  hsPtr->m_wldPosition.y;
				   intersect.z +=  hsPtr->m_wldPosition.z;
		           tempDist = m_matLib.MagnitudeSquared(lastPos.x,lastPos.y,lastPos.z,intersect.x,intersect.y,intersect.z);
                   
				   if(tempDist < bestDistance)
				   {
                      
                      CopyMemory(collisionIntersect,&intersect,sizeof(D3DVECTOR));

                      bestDistance = tempDist;
		              return true;
				   }
		  }
		 }
		}//end Houseing
	  }//end use tabled mode
	 
	 return false;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		GlobalStatusVariables
// Function:	ActivateItem(CMenuInventoryItem*)
// Description:	Wrapper function for GlobalStatusVariables::ArmEntityItemSys().
//	Handles arming and/or using items as well as visual side-effects for the 
//	menu system.
///////////////////////////////////////////////////////////////////////////////

bool 
GlobalStatusVariables::ActivateItem(CMenuInventoryItem *pInventoryItem)
{
	// ERROR checking
	if (!pInventoryItem)
		return false;

	// GET reference to inventory menu
	CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY, true));

	// GET the client-side player character
	int dupPassed = 0;
	POSITION pos = this->m_entObjRuntime->FindIndex(0);
	if(!pos) return false;
	CMovementObj *pMovementObject = dynamic_cast<CMovementObj*>(m_entObjRuntime->GetAt(pos));
	CItemObj *pItem = this->m_itemManager->GetItemByID(pInventoryItem->m_data.m_GLID);
	if (!pItem) return false;

	// ARM item
	// NOTE: 'm_useType' 11 is for faction based items.
	if ((pItem->m_useType == 0) 
	|| (pItem->m_useType == 11))
	{
		// CHECK for a valid inventory item
		if ((pInventoryItem->m_data.m_GLID != -1) 
		&& (pMovementObject->List_controlType == 0)
		&& (pMovementObject->m_level >= pItem->m_requiredSkillLevel))
		{
			// ARM the item
			if (this->ArmEntityItemSys(pMovementObject, pInventoryItem->m_data.m_GLID, dupPassed))
				pInventoryItem->m_data.m_armed = true;

			// REFRESH the inventory menu
			/*if (pInventory)
				pInventory->RefreshItems();*/
			return true;
		}
		return false;
	}
	// USE item
	else
	{
		// SEND server command to activate/use the specified item
		m_netGameLogicObject->SendAttribMsg(
			DPNSEND_GUARANTEED,
			DPID_SERVERPLAYER,
			27, // USE ITEM
			pInventoryItem->m_data.m_GLID,
			pMovementObject->List_networkDefinedID,
			dupPassed,
			0,
			NULL); 

		// DECREMENT item count since it's being used
		if (pInventory && pItem->m_destroyOnUse)
			pInventory->RemoveItem(pInventoryItem->m_data.m_GLID, 1);
		return true;
	}
	return false;
}

/*
bool GlobalStatusVariables::ActivateItem(int glID, CMenuListItem *listItem)
{
	static long m_durationToServerStamp = GetTickCount() - 11000;
	int m_durationToServer = 10000;
	POSITION entPosition = m_entObjRuntime->FindIndex(0);

	if(entPosition)
	{//Valid entPosition
		CMovementObj * ptrLoc = ( CMovementObj* ) m_entObjRuntime->GetAt(entPosition);
		int dupPassed = 0;
		CItemObj * invObj = m_itemManager->GetItemByID(glID);
		
		if(invObj)
		{//Valid Inventory Object
			if(invObj->m_useType != 0 && invObj->m_useType != 11) //Added in 11 for faction based item
			{//Dont Use Items Without a Use Type
				//RemoveSelectedFromMenuList(clickObj->m_miscInt);
				BOOL useItemNow = TRUE;
				/*
				** CHRIS NEEDS TO FIX: HOUSE PLACEMENT VIA ITEM!
				
				if(invObj->m_itemData->m_useType == 6)
				{//house item verify placement validity locally before even using this item
					D3DVECTOR positionWld;
					ptrLoc->List_baseFrame->GetPosition(NULL,&positionWld);
					CHousingObj * houseBasePtr = m_housingDB->GetByIndex(invObj->m_itemData->m_miscUseValue);
					if(houseBasePtr)
					{//valid database base
						if(!HousePlacementValidCheck(positionWld,houseBasePtr->m_clipBox))
						{
							useItemNow = FALSE;
							m_systemReadout->AddLine("Ground would not support your house well here...");
						}
					}//end valid database base
					else
					{//cannot find house DB ref
						 useItemNow = FALSE;
						 m_systemReadout->AddLine("Invalid database error 23464...");
					}//end cannot find house DB ref
					//m_clickHandlerStamp = currentTime;
				}//end house item verify placement validity locally before even using this item

				* END CHRIS FIX
				* /
				if(useItemNow)
				{
					//MinusSelectedFromMenuList(clickObj->m_miscInt,glID,1);
					//Remove Item from Visual List
					//visual->RemoveItem(visual->m_currentSelection->m_index);
					if(invObj->m_reloadTime == 0)
					{						
						//For Conjugation and skill use
						if(invObj->m_useType == 12)
						{
							if((GetTickCount() - m_durationToServerStamp) > m_durationToServer)
							{
								if(invObj->m_requiredSkillType == 13)
								{
									POSITION entPosition = m_entObjRuntime->FindIndex(0);
									if(m_multiplayerObject->GetEnabledClient_safe())
									if(entPosition)
									{//valid Alpha  
										CMovementObj * ptrLoc = ( CMovementObj* ) m_entObjRuntime->GetAt(entPosition);
										if(ptrLoc->List_controlType == 0)
										{
											int EnergyCost = 0;
											POSITION posLoc = m_entObjRuntime->FindIndex(0);
											if(posLoc)
											{														
												EnergyCost = 10; //hardcoded for the moment
												CMovementObj * ptrLoc = ( CMovementObj* ) m_entObjRuntime->GetAt(posLoc);
												if(ptrLoc)
												{
													ptrLoc->m_energyCurrent -= EnergyCost;
													if(ptrLoc->m_energyCurrent < 0)
													{
														ptrLoc->m_energyCurrent = 0;
														m_systemReadout->AddLine("Not enough energy!");
														return false;
													}
												}
											}
										}
									}//End valid alpha
								}//End valid timeing
							}//End check for valid Skill type
							else
								return false;
						}//End for conjugation

						m_netGameLogicObject->SendAttribMsg(DPNSEND_GUARANTEED,
							DPID_SERVERPLAYER,27,//use item
							glID,ptrLoc->List_networkDefinedID,
							dupPassed,0,NULL); 
					}
					else
						 m_pendingObject->AddCommand(1,glID,ptrLoc->List_networkDefinedID,invObj->m_reloadTime,"");
				}
			}//End Dont Use Items Without a Use Type
			else
			{//Attempt to Arm Item
				if(glID != -1)
				{//Valid Object Selected						
					if(ptrLoc->List_controlType == 0)
					{//Attempt to Arm Item
						if(listItem)
							listItem->m_special = true;

						ArmEntityItemSys(ptrLoc,glID,dupPassed);
					}//End Attempt to Arm Item
				}//End Valid Object Selected
			}//End Attempt to Arm Item
		}//End Valid Inventory Object
	}//End Valid entPosition

	//Set Click Handler Timestamp
	//m_clickHandlerStamp = currentTime;

	return true;
}
*/

//--------------Handles system messages-----------------------------------------------//
BOOL
GlobalStatusVariables::ArmEntityItemSys(CMovementObj *mainPtr,
										int itemID,
										int dupPassedOption)
{
	/*long currentTickCount = GetTickCount();
	if ((currentTickCount - m_armDisarmStamp) > 1000)
	{
		m_armDisarmStamp = currentTickCount;*/
		if(mainPtr->List_energy <= 0)
			return FALSE;

		if(m_multiplayerObject->GetEnabledClient_safe())
		{
			m_netGameLogicObject->SendEquipMsg(
				itemID,
				mainPtr->List_networkDefinedID,
				NULL,
				5,
				dupPassedOption);
			return TRUE;
		}

		CMovementObj * DBPtr = m_entObjList->GetByID(mainPtr->List_refModel);
		if(!DBPtr) return FALSE;
		mainPtr->ArmItemByGLID(itemID,DBPtr,m_itemManager,NULL);
		return TRUE;
	//}
	return FALSE;
}
//--------------Handles system messages-----------------------------------------------//

///////////////////////////////////////////////////////////////////////////////
// Class:		GlobalStatusVariables
// Function:	DeactivateItem(CMenuInventoryItem*)
// Description:	Wrapper function for GlobalStatusVariables::DisarmEntityItemSys().
//	Handles unarming items as well as visual side-effects for the menu system.
///////////////////////////////////////////////////////////////////////////////

bool 
GlobalStatusVariables::DeactivateItem(CMenuInventoryItem *pInventoryItem)
{
	if (!pInventoryItem)
		return FALSE;

	// GET the client-side player character
	POSITION pos = this->m_entObjRuntime->FindIndex(0);
	if(!pos) return FALSE;
	CMovementObj *pMovementObject = dynamic_cast<CMovementObj*>(m_entObjRuntime->GetAt(pos));
	CItemObj *pItem = this->m_itemManager->GetItemByID(pInventoryItem->m_data.m_GLID);
	if (!pItem) return FALSE;

	// ERROR checking: Is this player alive?
	if(pMovementObject->List_energy <= 0)
		return FALSE;

	// DISARM this item on the client-side
	CMovementObj *DBPtr = this->m_entObjList->GetByID(pMovementObject->List_refModel);
	if(!DBPtr) return FALSE;
	pMovementObject->DisarmItemByGLID(pInventoryItem->m_data.m_GLID, FALSE, DBPtr, NULL);
	
	// DISARM this item on the server-side
	return this->DisarmEntityItemSys(pMovementObject, 
		pInventoryItem->m_data.m_GLID, 0);
		pInventoryItem->m_data.m_armed = false;

	// REFRESH the inventory menu
	CMenuInventory *pInventory = dynamic_cast<CMenuInventory*>(this->m_runtimeMenuDatabase->GetVisualByID(ID_PLAYER_INVENTORY, true));
	if (pInventory)
		pInventory->RefreshItems();
}

BOOL 
GlobalStatusVariables::DisarmEntityItemSys(CMovementObj *mainPtr,
										   int itemID,
										   int dupPassedOption)
{
	if (!this->m_multiplayerObject->GetEnabledClient_safe())
		return FALSE;

	/*long currentTickCount = GetTickCount();
	if ((currentTickCount - m_armDisarmStamp) > 1000)
	{
		this->m_armDisarmStamp = currentTickCount;*/
		int indexprep = -itemID;
		if (indexprep == 0)
			indexprep = -9999;

		// DISARM this item on the server-side
		this->m_netGameLogicObject->SendEquipMsg(indexprep,
			mainPtr->List_networkDefinedID,
			NULL,
			5,
			0 /*dupPassedOption*/);
		return TRUE;
	//}
	return FALSE;
}

/*
BOOL GlobalStatusVariables::DisarmEntityItemSys(CMovementObj * mainPtr,int itemID,int dupPassedOption)
{
  long currentTickCount = GetTickCount();
  if((currentTickCount - m_armDisarmStamp) > 1000)
  {
	m_armDisarmStamp = currentTickCount;

	if(mainPtr->List_energy <= 0)
	   return FALSE;

    if(m_multiplayerObject->GetEnabledClient_safe()){
       int indexprep = -itemID;
	   if(indexprep == 0)
		  indexprep = -9999;

	   m_netGameLogicObject->SendEquipMsg(   indexprep,
											 mainPtr->List_networkDefinedID,
											 NULL,
											 5,
											 dupPassedOption);
       return TRUE;
	}

	 CMovementObj * DBPtr = m_entObjList->GetByID(mainPtr->List_refModel);
		if(!DBPtr)
			return FALSE;
	       

     mainPtr->DisarmItemByGLID(itemID,FALSE,DBPtr,NULL);
  }
	 return TRUE;
}
*/
//---------------------------------------------------------------------------//
void GlobalStatusVariables::SetCloseDownCallState(BOOL state)
{
     EnterCriticalSection(&m_stateSection);
     m_closeDownCall = state;
     LeaveCriticalSection(&m_stateSection);
}
//---------------------------------------------------------------------------//
BOOL GlobalStatusVariables::GetCloseDownCallState()
{
	 BOOL temp = 0;
     EnterCriticalSection(&m_stateSection);
     temp = m_closeDownCall;
     LeaveCriticalSection(&m_stateSection);
	 return temp;
}
//------------------------------------------------------------------------------------//
void GlobalStatusVariables::UpdateRiderOfPossedEntity(CMovementObj * entityPtr)
{
     if(entityPtr->m_posses)
	 {//possesing somthing update movememnt
        IndexEntityAnimations(entityPtr->m_posses,GetTickCount(),FALSE,m_missileRuntime);

	 if(entityPtr->m_possedBy)
        IndexEntityAnimations(entityPtr->m_possedBy,GetTickCount(),FALSE,m_missileRuntime);

	    CFrameObj * frameRiderObj = NULL;
        if(entityPtr->m_ridable > -1)
           frameRiderObj = entityPtr->List_skeletalObject->GetBoneFrameByIndex(entityPtr->m_ridable);//aquire riders bone basis of mount connection
	
		 CFrameObj * frameBaseObj = entityPtr->m_posses->List_skeletalObject->GetBoneFrameByIndex(entityPtr->m_posses->m_ridable);//aquire mounts bone index

		 if(!frameBaseObj)
		 {
             frameBaseObj = entityPtr->m_posses->List_skeletalObject->GetBoneFrameByIndex(0);
             entityPtr->m_posses->m_ridable = 0;
		 }

     	 if(frameBaseObj)
		 {
          D3DVECTOR upVector,dirVect;
		  entityPtr->m_posses->List_baseFrame->GetOrientation(&dirVect,&upVector);
		  D3DMATRIX matrixAp = entityPtr->m_posses->List_skeletalObject->GetBoneMatrixByIndex(entityPtr->m_posses->m_ridable);
	      D3DMATRIX focusPtrMatrix;
	      entityPtr->m_posses->List_baseFrame->GetTransform(NULL,&focusPtrMatrix);
	      m_matLib.fastMatrixMultiply(matrixAp,matrixAp,focusPtrMatrix);

		  if(!frameRiderObj)
		  {//no basis bone
             entityPtr->List_baseFrame->SetPosition2(NULL,matrixAp._41,matrixAp._42-2.0,matrixAp._43);
             entityPtr->List_baseFrame->SetOrientation(dirVect,upVector);
		  }//end no basis bone
		  else
		  {//base character position by offset of mount bone
			 D3DVECTOR baseBonePos;
             frameRiderObj->GetPosition(NULL,&baseBonePos);
             entityPtr->List_baseFrame->SetPosition2(NULL,matrixAp._41,matrixAp._42-baseBonePos.y,matrixAp._43);
             entityPtr->List_baseFrame->SetOrientation(dirVect,upVector);
		  }//end base character position by offset of mount bone
		 }
		
	  
	 }//end possesing somthing update movememnt

}
//------------------------------------------------------------------------------------//
void GlobalStatusVariables::TempMenuCallBack()
{
    if(m_tempMenuFlag != m_currentTempMenuFlag)
	{//action is required
	   if(m_tempMenuFlag == -1)
	   {//closing menu
          m_runtimeMenuDataBase->CloseByID(m_currentTempMenuFlag);
   

		  if(!m_runtimeMenuDataBase->GetByID(m_currentTempMenuFlag))
		     m_currentTempMenuFlag = m_tempMenuFlag;
	   }//end closing menu
	   else
	   {//opening menu
           OpenMenu(m_tempMenuFlag, false);
		   m_currentTempMenuFlag = m_tempMenuFlag;
	   }//end opening menu
	}//action is required
	m_tempMenuFlag = -1;//flag for potential change next round
}
//------------------------------------------------------------------------------------//
void GlobalStatusVariables::AddLine_safe(CString string)
{
	if(m_systemReadout
	&& (m_runtimeMenuDatabase->GetCount() > 0) 
	&& m_runtimeMenuDatabase)
		m_systemReadout->AddLine(string);
}
//------------------------------------------------------------------------------------//
///////////////////////////////////////////////////////////////////////////////
// Class:		GlobalStatusVariables
// Function:	SendChatString(CString)
// Description:	Parse and handle player submitted text chat strings.
///////////////////////////////////////////////////////////////////////////////

void
GlobalStatusVariables::SendChatString(CString string)
{
	// Get the client's entity
	POSITION pos;
	if(!(pos = m_entObjRuntime->GetHeadPosition()))	return;
	
	string = string.Trim(); // Clean up string
	if(string.GetLength() == 0) return; // No need to process an empty string.

	// Store the client's entity pointer for later use
	CMovementObj* entityPtr = (CMovementObj*)m_entObjRuntime->GetNext(pos);
	
	// If the client is enabled...
	if(m_multiplayerObject->GetEnabledClient_safe())
	{
		CSimpleString::XCHAR chFirstChar = string.GetAt(0); // Check first char for escape character 

		// If this string is not escaped, it's just a regular chat message
		if(chFirstChar != ':' && chFirstChar != '/')
		{
			/*
			m_netGameLogicObject->SendTextMsg(DPNSEND_GUARANTEED,
				DPID_SERVERPLAYER,
				SCMD_UNKNOWN,
				string,
				entityPtr->List_networkDefinedID,
				NULL,
				TMC_BROADCAST);
				*/
			m_netGameLogicObject->SendTextMsg(DPNSEND_GUARANTEED,
				DPID_SERVERPLAYER,
				entityPtr->List_networkDefinedID,
				NULL,
				TMC_BROADCAST,
				string,
				SCMD_UNKNOWN);
			entityPtr->m_lastTextArea = "All";
			return;
		}
		// Otherwise, we have been issued a command message
		else
		{
			int		iLastIndex = 0;
			CString	strCommand = 0,
					strArguments = 0;

			// Find the 0-based index of the end of the command 
			iLastIndex = string.Find(' ');  
			if(iLastIndex == -1) iLastIndex = string.GetLength();

			// Isolate the command keyword and make it all uppercase for ID lookup
			strCommand = string.Mid(1, (iLastIndex - 1)).MakeUpper();

			// Isolate any accompanying arguments
			if(iLastIndex < string.GetLength())
			{
				strArguments = string.Mid(iLastIndex + 1);
			}

			// Perform command ID lookup
			COMMAND_MAP::const_iterator iter;
			std::string stdCommand(static_cast<LPCTSTR>(strCommand));

			// Is CLIENT command?
			iter = this->m_pNetworkCommands->ClientCommands->find(stdCommand);
			if(iter != this->m_pNetworkCommands->ClientCommands->end())
			{
				switch(iter->second)
				{
/*					case CCMD_INVITE:
					{
						// Ask to join our group
						CMovementObj *entityToInvite = m_entObjRuntime->GetObjByName(strArguments);
						if(entityToInvite)
							m_netGameLogicObject->SendCommandMsg(3, entityToInvite->List_networkDefinedID, entityPtr->List_networkDefinedID);

						// Open group status window
						this->OpenMenu(GROUP_STATUS_WINDOW_ID, false);
						break;
					}
					case CCMD_REMOVE:
					{
						// Remove the entity
						CMovementObj *entityToRemove = m_entObjRuntime->GetObjByName(strArguments);
						if(entityToRemove)
							m_netGameLogicObject->SendCommandMsg(10, entityToRemove->List_networkDefinedID, entityPtr->List_networkDefinedID);

						// Open group status window
						this->OpenMenu(GROUP_STATUS_WINDOW_ID, false);
						break;
					}
					case CCMD_GIVEAUTHORITY:
					{
						// Promote the entity
						CMovementObj *entityToPromote = m_entObjRuntime->GetObjByName(strArguments);
						if(entityToPromote)
							m_netGameLogicObject->SendCommandMsg(11, entityToPromote->List_networkDefinedID, entityPtr->List_networkDefinedID);
						break;
					}
					case CCMD_MAKELEADER:
					{
						// Make entity a leader
						CMovementObj *entityToPromote = m_entObjRuntime->GetObjByName(strArguments);
						if(entityToPromote)
							m_netGameLogicObject->SendCommandMsg(12, entityToPromote->List_networkDefinedID, entityPtr->List_networkDefinedID);
						break;
					}
					case CCMD_CREATEGROUP:
					{
						// Start forming a group
						m_netGameLogicObject->SendCommandMsg(15, 0, entityPtr->List_networkDefinedID);

						// Open group status window
						this->OpenMenu(GROUP_STATUS_WINDOW_ID, false);
						break;
					}
					case CCMD_LEAVEGROUP:
					{
						// Remove myself from group
						m_netGameLogicObject->SendCommandMsg(13, 0, entityPtr->List_networkDefinedID);

						// Open group status window
						this->OpenMenu(GROUP_STATUS_WINDOW_ID, false);
						break;
					}
*/					case CCMD_SQUADMENU:
					{
						// Request the group list
						m_netGameLogicObject->SendCommandMsg(4, 0, entityPtr->List_networkDefinedID);

						// Open group status window
						this->OpenMenu(ID_SQUAD_STATUS_MENU, false);
						break;
					}
					case CCMD_SQUELCH:
					{
						// Add the name to the filter list
						m_filterSpeechDB->AddName(strArguments);

						// Let the user know that the command went through
						if(m_systemReadout)
							m_systemReadout->AddLine(strArguments + " has been squelched...");
						break;
					}
					case CCMD_DESQUELCH:
					{
						// Add the name to the filter list
						m_filterSpeechDB->UnBlockName(strArguments);

						// Let the user know that the command went through
						if(m_systemReadout)
							m_systemReadout->AddLine(strArguments + " has been desquelched...");
						break;
					}
					case CCMD_SQUAD:
					{
						// Send the message to their group
						m_multiplayerObject->SendTextMsgToSquad(DPNSEND_GUARANTEED,
							DPID_SERVERPLAYER,
							strArguments,
							entityPtr->List_networkDefinedID);
						entityPtr->m_lastTextArea = "/C";
						break;
					}
					case CCMD_ALL:
					{
						m_netGameLogicObject->SendTextMsg(DPNSEND_GUARANTEED,
							DPID_SERVERPLAYER,
							entityPtr->List_networkDefinedID,
							NULL,
							TMC_BROADCAST,
							strArguments,
							SCMD_UNKNOWN);
						entityPtr->m_lastTextArea = "All";
						break;
					}
					case CCMD_GROUP:
					{
						// Send the message to their clan
						m_multiplayerObject->SendTextMsgToGroup(DPNSEND_GUARANTEED,
							DPID_SERVERPLAYER,
							strArguments,
							entityPtr->List_networkDefinedID);
						entityPtr->m_lastTextArea = "/GROUP";
						break;
					}
					case CCMD_DANCE:
					{
						entityPtr->m_ovverrideAnimation  = 21;
						entityPtr->m_ovverrideDuration   = 0;
						entityPtr->m_ovverrideStamp      = GetTickCount();
						break;
					}
					case CCMD_SIT:
					{
						entityPtr->m_ovverrideAnimation  = 28;
						entityPtr->m_ovverrideDuration   = 0;
						entityPtr->m_ovverrideStamp      = GetTickCount();
						break;
					}
					default: /* Should never get here */ break;
				}
			}
			 // Else, is SERVER command.
			else
			{
				iter = this->m_pNetworkCommands->ServerCommands->find(stdCommand);

				// Unrecognized command?
				if(iter == this->m_pNetworkCommands->ServerCommands->end()) return;

				// Package the command string up and queue it for transmission to the server
				/*
				m_netGameLogicObject->SendTextMsg(DPNSEND_GUARANTEED,
					DPID_SERVERPLAYER,
					iter->second,
					strArguments,
					entityPtr->List_networkDefinedID,
					NULL,
					TMC_COMMAND);
					*/
				m_netGameLogicObject->SendTextMsg(DPNSEND_GUARANTEED,
					DPID_SERVERPLAYER,
					entityPtr->List_networkDefinedID,
					NULL,
					TMC_COMMAND,
					strArguments,
					iter->second);
			}
		}
	}
	// ...otherwise, just print the text string and we're done.
	else
	{
		// Add the line to the system readout and we're done
		if(m_systemReadout) m_systemReadout->AddLine(string);
		return;
	}
}

/* DEPRECATED /////////////////////////////////////////////////////////////////
void GlobalStatusVariables::SendChatString(CString &string)
{
	//Get the client's entity
	POSITION pos;
	if(!(pos = m_entObjRuntime->GetHeadPosition()))
		return;
	
	//Store the client's entity pointer for later use
	CMovementObj* entityPtr = (CMovementObj*)m_entObjRuntime->GetNext(pos);
	
	//Make sure we're not in the editor
	if(m_multiplayerObject->GetEnabledClient_safe())
	{
		//If this is a command
		if(string.GetAt(0) == ':' || string.GetAt(0) == '/')
		{
			//Delete the : or /
			string.Delete(0);
			int length = string.GetLength();

			//If this is an invite command
			if(length > 6 && string.Left(6).CompareNoCase("INVITE") == 0)
			{
				//Cut out the command
				string.Delete(0,7);

				//Search for the name
				CMovementObj * entityToInvite = m_entObjRuntime->GetObjByName(string);
				if(entityToInvite)
					m_netGameLogicObject->SendCommandMsg(3, entityToInvite->List_networkDefinedID, entityPtr->List_networkDefinedID);//ask to join our group

				//We're done here
				return;
			}
			///////////////////////
			// NEW TEXT COMMANDS //
			///////////////////////
			else if(length > 6 && string.Left(6).CompareNoCase("REMOVE") == 0)
			{
				//Cut out the command
				string.Delete(0, 7);

				//Search for the name
				CMovementObj *entityToRemove = m_entObjRuntime->GetObjByName(string);
				
				if(entityToRemove)
					m_netGameLogicObject->SendCommandMsg(10, entityToRemove->List_networkDefinedID, entityPtr->List_networkDefinedID);

				//We be all done
				return;
			}
			else if(length > 13 && string.Left(13).CompareNoCase("GIVEAUTHORITY") == 0)
			{
				//Cut out the command
				string.Delete(0, 14);

				//Search for the name
				CMovementObj *entityToPromote = m_entObjRuntime->GetObjByName(string);

				if(entityToPromote)
					m_netGameLogicObject->SendCommandMsg(11, entityToPromote->List_networkDefinedID, entityPtr->List_networkDefinedID);

				//We be all done
				return;
			}
			else if(length > 10 && string.Left(10).CompareNoCase("MAKELEADER") == 0)
			{
				//Cut out the command
				string.Delete(0, 11);

				//Search for the name
				CMovementObj *entityToPromote = m_entObjRuntime->GetObjByName(string);

				if(entityToPromote)
					m_netGameLogicObject->SendCommandMsg(12, entityToPromote->List_networkDefinedID, entityPtr->List_networkDefinedID);

				//We be all done
				return;
			}
			else if(length == 10 && string.Left(10).CompareNoCase("LEAVEGROUP") == 0)
			{
				//Remove myself from group
				m_netGameLogicObject->SendCommandMsg(13, 0, entityPtr->List_networkDefinedID);

				//We be all done
				return;
			}
			else if(length == 11 && string.Left(11).CompareNoCase("CREATEGROUP") == 0)
			{
				//Start forming a group
				m_netGameLogicObject->SendCommandMsg(15, 0, entityPtr->List_networkDefinedID);

				//We be all done
				return;
			}
			///////////////////////////
			// END NEW TEXT COMMANDS //
			///////////////////////////
			//Otherwise, if its a request for the group menu
			else if(length == 9 && string.Left(9).CompareNoCase("GROUPMENU") == 0)
			{
				//Request the group list
				m_netGameLogicObject->SendCommandMsg(4, 0, entityPtr->List_networkDefinedID);
			
				//We're done here
				return;
			}
			//Otherwise, if its a request to squelch a player
			else if(length > 7 && string.Left(7).CompareNoCase("SQUELCH") == 0)
			{
				//Cut out the command
				string.Delete(0,8);

				//Add the name to the filter list
				m_filterSpeechDB->AddName(string);

				//Let the user know that the command went through
				m_systemReadout->AddLine(string + " has been squelched...");

				//We're done here
				return;
			}
			//Otherwise, if its a request to desquelch a player
			else if(length > 9 && string.Left(9).CompareNoCase("DESQUELCH") == 0)
			{
				//Cut out the command
				string.Delete(0,10);

				//Add the name to the filter list
				m_filterSpeechDB->UnBlockName(string);

				//Let the user know that the command went through
				m_systemReadout->AddLine(string + " has been desquelched...");

				//We're done here
				return;
			}
			//Otherwise, if its a group send
			else if(length > 5 && string.Left(5).CompareNoCase("GROUP") == 0)
			{
				//Cut out the command
				string.Delete(0,6);

				//Send the message to their group
				m_multiplayerObject->SendTextMsgToGroup(DPNSEND_GUARANTEED,
														DPID_SERVERPLAYER,//can be NULL if client
														string,
														entityPtr->List_networkDefinedID);

				//We're done here
				return;
			}
			//Otherwise, if its a clan send
			else if(length > 4 && string.Left(4).CompareNoCase("CLAN") == 0)
			{
				//Cut out the command
				string.Delete(0,5);

				//Send the message to their clan
				m_multiplayerObject->SendTextMsgToClan(	DPNSEND_GUARANTEED,
														DPID_SERVERPLAYER,//can be NULL if client
														string,
														entityPtr->List_networkDefinedID);

				//We're done here
				return;
			}
			else
			{
				//m_systemReadout->AddLine("Unknown Command");
				//Send As Normal Chat so Server Can Handle it and Search for
				//Additional Commands
				m_netGameLogicObject->SendTextMsg(	DPNSEND_GUARANTEED,
													DPID_SERVERPLAYER,//can be NULL if client
													"/" + string,
													entityPtr->List_networkDefinedID,
													NULL,
													0);
			}
		}
		else
			//If all else fails, send it as normal chat
			m_netGameLogicObject->SendTextMsg(	DPNSEND_GUARANTEED,
												DPID_SERVERPLAYER,//can be NULL if client
												string,
												entityPtr->List_networkDefinedID,
												NULL,
												0);
	}
	//Otherwise, we're in the engine
	else if(m_systemReadout)
		m_systemReadout->AddLine(string);//So just add the line to the system readout
}
/////////////////////////////////////////////////////////////////////////////*/
//------------------------------------------------------------------------------------//
void GlobalStatusVariables::UI_Log(CString msg)
{
     AddLine_safe(msg);
     m_axLogger.WriteDebugFile(msg.GetBuffer(),0,1);
}
//------------------------------------------------------------------------------------//
void GlobalStatusVariables::UI_Log(CString msg,CString msg2)
{
	 CString temp = msg+":"+msg2;
     AddLine_safe(temp);
     m_axLogger.WriteDebugFile(temp.GetBuffer(),0,1);
}

//---used by client-------------------------------------------------------------------//
int GlobalStatusVariables::CreateCharacter_Send(        int     spawnPosIndex,
														CString characterName,
														CMovementObj * entityConfigured)
{ 
	if(!m_multiplayerObject->GetEnabledClient_safe())
        return 2;//not connected to server

	if(characterName.GetLength() == 0 || characterName.GetLength() > 25)
		return 3;//invalid name


    CREATECHARHEADER_DATA header;
    header.m_catagorie         = 19;
    header.m_nameLength        = characterName.GetLength();
    header.m_spawnCfg          = spawnPosIndex;//refernced on serverside for db index of position and world
    header.m_entityType        = entityConfigured->List_refModel;
    header.m_attachmentCount   = 0;
	header.m_defCfgCount       = entityConfigured->List_skeletalObject->m_deformableMeshArrayCount;
  
	//get size
	if(entityConfigured->m_itemDB)//update attachment
	 for(POSITION armedPos = entityConfigured->m_itemDB->GetHeadPosition();armedPos != NULL;)
	 {
        CItemObj * itemPtr = (CItemObj *)entityConfigured->m_itemDB->GetNext(armedPos);
	    if(itemPtr->m_equiped)//make sure hes wearin it    
		    header.m_attachmentCount++;
	 }


	int headerSize = sizeof(CREATECHARHEADER_DATA);
    int itemStructSize = sizeof(ITEMDATA_STRUCT);
	
	int totalSize = ((header.m_defCfgCount*itemStructSize) + (header.m_attachmentCount*itemStructSize)) + headerSize + header.m_nameLength;
    BYTE * dataBlock = new BYTE[totalSize];

    CopyMemory(dataBlock,&header,headerSize);

	for(int j=0;j<header.m_nameLength;j++)
        dataBlock[(j+headerSize)] = characterName.GetAt(j);

	if((header.m_defCfgCount+header.m_attachmentCount) > 0)
	{
	   ITEMDATA_STRUCT * itemDataStruct = new ITEMDATA_STRUCT[header.m_defCfgCount+header.m_attachmentCount];
       
	   //set data
	   int trace = 0;
	   if(entityConfigured->m_itemDB)
	   for(POSITION armedPos = entityConfigured->m_itemDB->GetHeadPosition();armedPos != NULL;)
	   {//update configuration/customized 
          CItemObj * itemPtr = (CItemObj *)entityConfigured->m_itemDB->GetNext(armedPos);
		  if(itemPtr->m_equiped){//make sure hes wearin it    
		     itemDataStruct[trace].m_GLID      = itemPtr->m_globalID;
		     itemDataStruct[trace].m_quantity   = itemPtr->m_locationID;
			 itemDataStruct[trace].m_dMod      = itemPtr->m_damageModifier;
             trace++;
		  }//end make sure hes wearin it  
	   }//end update configuration/customized 

  	   for(int loop=0;loop<entityConfigured->List_skeletalObject->m_deformableMeshArrayCount;loop++)
	   {//deformable mesh loop
		  itemDataStruct[trace].m_GLID    = loop;
		  itemDataStruct[trace].m_quantity = entityConfigured->List_skeletalObject->m_spawnCfg[loop];
		  itemDataStruct[trace].m_dMod    = 0;
		  trace++;
	   }//end deformable mesh loop

       BYTE * cast = (BYTE*)itemDataStruct;

       for(int i=0;i<((totalSize-headerSize)-header.m_nameLength);i++)
           dataBlock[(headerSize+i+header.m_nameLength)] = cast[i];


	   if(itemDataStruct)
	   {
          delete [] itemDataStruct;
          itemDataStruct = 0;
	   }
    }

    m_multiplayerObject->SendByteBlockMsg(DPNSEND_GUARANTEED,
										  NULL,
										  dataBlock,
										  totalSize,
										  m_multiplayerObject->m_packetTimeout
										  );//deleted the buffer internally

	return TRUE;
}
//--------------Digital Controls In Loop----------------------------------------------//
BOOL GlobalStatusVariables::SegmentCollisionToAllEntities( BOOL filterPrimary,
														   D3DVECTOR & start,
														   D3DVECTOR & end,
														   D3DVECTOR & intersect,
														   float segmentThickness,
														   float & bestDist)
{
	    D3DVECTOR tempIntersect;
        //check ray to entities
	    float rayThickness	= .01f;
		float curDist		= 0.0f;
		float hvRadius		= 500.0f;
		D3DMATRIX			positionMatrix;
		D3DVECTOR			bnPosition,centerSegVect;
		BOOL				foundOne = FALSE;
		centerSegVect.x = start.x + ((end.x - start.x) * .5f);
		centerSegVect.y = start.y + ((end.y - start.y) * .5f);
	    centerSegVect.z = start.z + ((end.z - start.z) * .5f);
        for(POSITION posLoc2 = m_entObjRuntime->GetHeadPosition();posLoc2 != NULL;)
	    {//Begin entity loop
	        CMovementObj * entPtr = ( CMovementObj* ) m_entObjRuntime->GetNext(posLoc2);
			if(filterPrimary)
			if(entPtr->List_controlType == 0)
			   continue;
			
			entPtr->List_baseFrame->GetTransform(NULL,&positionMatrix);
            if(entPtr->List_skeletalObject->m_calulationSphere)
			{//valid collision calc object
			   D3DVECTOR bonePosition = m_matLib.UpdatePolygonVertex(positionMatrix.m,entPtr->List_skeletalObject->GetBonePositionByIndex(entPtr->List_skeletalObject->m_calulationSphere->m_boneIndex));
               //get virtual Radius
			   
			   if(m_matLib.BoundingSphereCheckX(hvRadius,
												entPtr->List_skeletalObject->m_calulationSphere->m_sphereRadius,
												bonePosition.x,
												bonePosition.y,
												bonePosition.z,
												centerSegVect.x,
												centerSegVect.y,
									 			centerSegVect.z))
			   {//in calculation radius
			     
			     if(entPtr->List_skeletalObject->m_sphereLocationalDamageList){//valid locational damage list
		         for(POSITION dmgPos = entPtr->List_skeletalObject->m_sphereLocationalDamageList->GetHeadPosition();dmgPos != NULL;)
				 {//damage sphere loop
		             CSphereCollisionObj * dmgSph = (CSphereCollisionObj *)entPtr->List_skeletalObject->m_sphereLocationalDamageList->GetNext(dmgPos);
		             POSITION bnPos = entPtr->List_skeletalObject->m_bonesList->FindIndex(dmgSph->m_boneIndex);
                     if(bnPos){//valid
                        CBoneObject * bnPtr = (CBoneObject *)entPtr->List_skeletalObject->m_bonesList->GetAt(bnPos);
						bnPtr->m_boneFrame->GetPosition(NULL,&bnPosition);
                        bnPosition = m_matLib.UpdatePolygonVertex(positionMatrix.m,bnPosition);
						
						if(m_matLib.CollisionSphereSeg(		start,
															end,
															dmgSph->m_sphereRadius,
															bnPosition,
															FALSE,
															NULL,
															segmentThickness,
															&tempIntersect)==TRUE)
						{//hit
					         curDist = m_matLib.MagnitudeSquared(start.x,start.y,start.z,tempIntersect.x,tempIntersect.y,tempIntersect.z);
							 if(curDist < bestDist)
							 {//closer hit
							    intersect = tempIntersect;
								bestDist = curDist;
								foundOne = TRUE;
							 }//end closer hit
						}//end hit   
					 }//end valid
				 }//end damage sphere loop
			  }//end valid locational damage list
			 }//end in calculation radius
			}//end valid collision calc object
		}//end entity loop	
		return foundOne;
}

void
GlobalStatusVariables::RefreshQuestJournal(int iQuestListID, bool bSaveCurrentSelection)
{
	if (!this->m_runtimeMenuDatabase || !this->m_multiplayerObject || !this->m_multiplayerObject->m_journalEntries)
		return;

	// GET references to menu visuals
	CMenuListBox *pQuestList = dynamic_cast<CMenuListBox*>(this->m_runtimeMenuDatabase->GetVisualByID(iQuestListID, true));

	// REFRESH the quest list
	if (pQuestList)
	{
		// SAVE the currently selected list item
		int iCurrentSelection = -1;
		if (bSaveCurrentSelection && pQuestList->m_currentSelection)
			iCurrentSelection = pQuestList->m_currentSelection->m_index;

		// CLEAR the quest list
		pQuestList->ClearList();

		// POPULATE the quest list (active quests first, then completed quests)
		for (int pass = 0; pass < 2; ++pass)
		{
			for (POSITION pos = this->m_multiplayerObject->m_journalEntries->GetHeadPosition(); pos != NULL;)
			{
				CQuestJournalObj *pJournalEntry = dynamic_cast<CQuestJournalObj*>(this->m_multiplayerObject->m_journalEntries->GetNext(pos));
				if (!pJournalEntry)
					continue;

				bool isCompleted = (pJournalEntry->m_status != 0);
				if ((pass == 0 && isCompleted) || (pass == 1 && !isCompleted))
					continue;

				// CREATE a new list item for the quest list
				CMenuListItem *pListItem = new CMenuListItem();
				pListItem->m_miscString = pJournalEntry->m_npcName;
				pListItem->m_special = isCompleted;

				CMenuText *pItemText = new CMenuText();
				if (isCompleted)
					pItemText->m_text = "[Done] " + pJournalEntry->m_npcName;
				else
					pItemText->m_text = pJournalEntry->m_npcName;

				pListItem->AddText(pItemText, pQuestList->m_width);

				// ADD the new list item to the menu
				pQuestList->AddItem(pListItem);
			}
		}

		// SET the scroll bar first item for the list box
		if (pQuestList->m_scrollBar && pQuestList->m_pFirstItem)
			pQuestList->m_scrollBar->m_curFirstItem = pQuestList->m_pFirstItem->m_index;

		// SET selected list item
		if (iCurrentSelection != -1 && pQuestList->m_pFirstItem)
		{
			CMenuListItem *pItem = pQuestList->m_pFirstItem;
			for (int i=0; i<iCurrentSelection; i++)
			{
				if (pItem && pItem->m_pNext)
					pItem = pItem->m_pNext;
			}
			pQuestList->m_currentSelection = pItem;
		}
		else if (pQuestList->m_pFirstItem)
			pQuestList->m_currentSelection = pQuestList->m_pFirstItem;

		// EXECUTE the selection change script
		if (pQuestList->m_selectionChangeScript)
			pQuestList->m_selectionChangeScript->ExecuteScript();
	}
}
