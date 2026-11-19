#pragma once
#include "Common.h"
#include "networkgamelogic.h"
#include "matrixarb.h"
#include "CCameraClass.h"
#include "Viewportclass.h"
#include "dxdeviceclass.h"
#include "CMenu3DClass.h"
#include "CMultiplayerClass.h"
#include "CMovementModel.h"
#include "networkgamelogic.h"
#include "CABFunctionLib.h"
#include "CtextReadoutClass.h"
#include "CFilterSpeechClass.h"
#include "CTextRenderClass.h"
#include "CWaterZoneClass.h"
#include "CEnvironmentClass.h"
#include "AIclass.h"
#include "CTranslucentPolyCache.h"
#include "CDynamicOverlay.h"
#include "CHousingClass.h"
#include "CRenderContainer.h"
#include "CExplosionClass.h"
#include "CMissileClass.h"
#include "CPersistClass.h"
#include "CEXScriptClass.h"
#include "CBackgroundClass.h"
#include "CWorldGroupClass.h"
#include "CMovieRuntimeClass.h"
#include "CTextureSwapClass.h"
#include "CGFontClass.h"
#include "CTextClass.h"
//#include "CHudClass.h"
#include "CCommerceClass.h"
#include "CBankClass.h"
#include "CSafeZones.h"
#include "CServerItemGen.h"
#include "CPortalSysClass.h"
#include "CElementsClass.h"
#include "CStatClass.h"
#include "CAIRaidClass.h"
#include "CBattleSchedulerClass.h"
#include "CWorldChannelRules.h"
#include "CObjectLibraryClass.h"
#include "CScreenFlashClass.h"
#include "CPendingCommandsClass.h"
#include "CMusicClass.h"
#include "postprocessmodule.h"
#include "axlanguagefilter.h"
#include "clientscripts.h"
#include "AXContentClient.h"
#include "CMenuBarClass.h"
#include "httpfiledownloader.h"
#include "crcgenerator.h"
#include "DrawVectors.h"
#include "SelectionEffect.h"
#include "LodLoadBalancer.h"
#include "AttributeQueue.h"
#include "CViewToSurface.h"
#include "CTextTempEffect.h"
#include "CameraAdvClass.h"
#include "ItemManager.h"
#include "CDotClass.h"
#include "CInput.h"
#include "CTranslucentPolyCache.h"
#include "CNetworkCommands.h"
#include "ControlsClass.h"

//New Menu System Includes
#include "CMenuVisual.h"
#include "CMenuWindow.h"
#include "CMenuComboBox.h"
#include "CMenuListBox.h"
#include "CMenuTextBox.h"
#include "CMenuText.h"
#include "CMenuScrollBar.h"
#include "CMenuButton.h"
#include "CMenuToggle.h"
#include "CMenuProgressBar.h"
#include "CMenuImage.h"
#include "CMenuEntity.h"
#include "CMenuScript.h"
#include "CMenuInventory.h"
#include "CWindowRadar.h"

///////////////////////////////////////////////////////////////////////////////
// GLOBAL ENUMS
///////////////////////////////////////////////////////////////////////////////

enum MouseCursors
{
	CURSOR_NORMAL = 0,
	CURSOR_ENEMYHOVER,
	CURSOR_EXAMINE,
	CURSOR_MAX
};

enum Minimaps
{
	MINIMAP_NEWBIEZONE = 0,
	MINIMAP_BOOMTOWN,
	MINIMAP_MAX_MAPS
};

// In-game login window (EARevolution.MOD): opened by the start-up script's connect step
// when no credentials are set; closed once the client is connected.
#define EA_LOGIN_MENU_ID 20001

class GlobalStatusVariables
{
public:
	GlobalStatusVariables(HWND & hwnd,BOOL editOn,BOOL sim);
	~GlobalStatusVariables(void);

    //Variables
	DWORD						m_iArenaTimer;
	DWORD						m_iArenaTimeStamp;
	CCRCgenerator               m_crcGenerator;
	DWORD						m_menuClosedStamp;
	BOOL						m_globalAvVar;
	BOOL						m_respawning;
	DWORD				        m_respawnTimeStamp;
	DWORD				        m_respawnWait;
	int							m_localContentClientVersion;
	BOOL                        m_speechMode;
	//CString                     m_chatDeleteString;
	//CString                     m_chatBuild;
	int                         m_unicodeStartIndex;
	float                       m_hudYouSayPosX;
	float                       m_hudYouSayPosY;

    float                       m_hudYouSaySizeX;
	float			            m_hudYouSaySizeY;
	float                       m_hudYouSayRed;
	float                       m_hudYouSayGreen;
	float                       m_hudYouSayBlue;

	int                         m_maxUnicodeOptions;
	CString                     m_globalUnicodeSection;
	CString                     m_lastGlobalUnicodeSection;
    long                        m_lastGlobalUnicodeIndexUse;
	char                        m_scndRec;
	BOOL                        gl_svrJt;
    float                       gl_frceZ;
    float                       gl_frceY;
	BOOL                        m_speedOmeterOn;
	D3DVECTOR                   m_speedPositionStamp;
	int                         m_startCamera;
	CString                     m_currentWorkingFile; //?
	BOOL                        m_mapCaptureAllZonesRequest; //editor: Display > Capture All Zone Maps
	BOOL                        m_mapCaptureBatch;    //all-zones run in progress: no message boxes
	BOOL                        m_mapCaptureBusy;     //a capture is drawing right now
	CString                     m_mapCaptureResult;   //how the last capture ended ("saved" or why not)
	int                         m_mapCaptureFloors;   //floors found by the last capture (0/1 = single map)
	BOOL                        m_mapCaptureAll;      //map capture: draw every world object, ignore popout visibility
	BOOL                        m_mapCaptureRequest;
#define HUD_QUEST_INV_REQ 31337        // inventory requests made by the quest tracker (not a menu)
	int                         m_hudInvCount;        //what the player carries, -1 = not known yet (quest tracker counts)
	int                         m_hudInvGlid[512];
	int                         m_hudInvQty[512];
	DWORD                       m_hudInvStamp;  //editor: Display > Capture Zone Map (RenderingManagement does the work)
	BOOL						m_editorModeOn;
	int                         m_compressTextures;
	//BOOL                        m_fullScreen;
	BOOL                        m_globalShadowType;
    BOOL						GL_renderEnvironment;
	D3DMATRIX                   m_matrixInit2;
	D3DMATRIX                   m_soundCenterMatrix;
	int                         m_totalWorldPolys;
	float                       GL_globalSoundFactor;
	BOOL                        m_showFirstPersonShadow;
	D3DVECTOR                   m_d3dVectorInit;
	D3DMATRIX                   GL_MirrorTransform;
	int                         m_currentTrack;
	CString                     m_musicLoopFile;
	int                         m_globalWorldShader;//-1 = no shader-enable fixed pipeline
	BOOL                        W_BufferPreffered;
	int                         GlobalTraceNumber;
	D3DCOLOR                    m_backgroundColorBlack;
	BOOL                        m_dedicatedServer;
	DWORD                       GL_spawnGeneratorTimeVarial;
	CFrameObj                   * GL_WORLD;
	D3DCOLOR                    m_backgroundColorGlobal;
	BOOL                        m_debuggingOn;
	CFile						m_debugfl;
	BOOL                        m_blackOut; 
	BOOL                        m_enhancedMode;
	float                       m_snapX;
    float                       m_snapY;
	BOOL                        m_txbBitmapLoadOverride;
	CString						m_shadowTextureFromFile;
	int							m_shadowIndexTemp;
	DWORD                       m_networkFreqStamp;
	int							m_collisionPolysInUse;
    int							m_actualPolysCalculated;
	BOOL                        m_closeDownCall;
	CString                     gl_helperString;
	BOOL                        m_backspaceQued;
	CString                     m_consolTXT;
	BOOL                        m_acceptConsoleTxt;
	int							GL_objectsRendered;
	RECT                        m_imediateRenderRECT;
	BOOL                        m_soundDeviceAvailable; 
	double						GL_testPerformance;
	BOOL                        GL_showFps;
	BOOL                        ErrorOutPut; 
	BOOL                        Lagged;
	long                        m_performanceTimeStamp;
	BOOL                        LoadNewWorld; //??
	CString                     NewWorld;
	int							m_exactFPSHiRes;
	double						GL_mathDuration;
	double						fps_cap;
	float						fpsSimulate;
	BOOL                        m_devMode;
	int                         m_oneVal;
	bool                        m_justSpawned;

	//Timing
	double                      m_timeAdjust;//GL_adjust
	float                       m_timeAdjustFloat;//float version
	double                      m_lastTimeAdjust;
	long                        m_actionDelay;
	//Time stamps
	long                        m_controlTimeStamp;
	long                        m_reloadTimerStamp;
	long                        m_speedTimerStamp;
	long                        m_actionDelayStamp;
	long                        m_armDisarmStamp;
	BOOL                        m_logRenderSection;
	float						m_globalShadowDist;
	int							m_maxShadowCount;
	BOOL						m_renderMenuClickZoneInfo;
	float                       m_forceScalarForAspect;
    BOOL                        m_lagged;
	int                         m_globalRenderer;
	CClientScripts              m_clientScriptor;
	CLoggingClass               m_axLogger;
	bool                        m_selctionModeOn;
	int                         m_idOfWorldObjectLastHit;
	int                         m_tempMenuFlag;
	int                         m_currentTempMenuFlag;
	long						m_globalCountDownDuration;//example 20000 (20 seconds)
	DWORD                       m_globalCountDownNotifierStamp;//
	int                         m_globalAutoTriggerLast;
	int                         m_showNames;
	CDrawVectors                m_dVectors;
	int                         m_lodOverride;
	long						m_lastCashUpdate;
	int							m_cashUpdateFrequency;
	BOOL                        m_switchingConnections;

	// Network Commands
	CNetworkCommands			*m_pNetworkCommands;	
	
	//3d text overhead constants
	int                         m_nameAboveHeadMode;//0=projection 1=3d 
	float                       m_nameAboveVisibleRange;//used for optimization
	float                       m_nameAboveFontSizeX;
	float                       m_nameAboveFontSizeY;
	float                       m_nameAboveFontColorR;
	float                       m_nameAboveFontColorG;
	float                       m_nameAboveFontColorB;
	float                       m_nameAboveNewLineScale;

	//targeting globals
	int							m_tgSys_indexOfObjectSelected;//-1 = none
	D3DVECTOR					m_tgSys_latestNormal;
	D3DVECTOR					m_tgSys_latestWldIntersection;
	int							m_tgSys_entitySelected;//-1 = none
    byte						m_tgSys_type;
    float						m_tgSys_dist;
	float						m_tgSys_validRange;//the longer the more computational
	BOOL                        m_tgSys_constantSelectionMode;//0=only on click 1=always calculating
	float                       m_tgSys_rampMultiplier;
	float                       m_tgSys_lastFov;
	int                         m_tgSys_expQue; //-1 = none > -1 = exp index uses normal and pos intersection
	int                         m_tgSys_quedButtonDown; 
	DWORD                       m_tgSys_quedButtonDownTimeStamp;
	DWORD                       m_tgSys_quedButtonDownTimeDuration;
	DWORD					    m_tgSys_hoverQueryStamp;//
	DWORD					    m_tgSys_hoverQueryDuration;//
	int							m_tgSys_hoverEntitySelected;
	int                         m_tgSys_hoverState;
	D3DMATRIX                   m_tgSys_camMatrixBasis;
	D3DMATRIX                   m_tgSys_projMatrixBasis;
	float                       m_tgSys_clipWidth;
	float                       m_tgSys_clipHeight;

	//Minimap Arrays
	CString						m_minimapFilenames[MINIMAP_MAX_MAPS];
	float						m_minimapScales[MINIMAP_MAX_MAPS];
	int							m_minimapCurMapIndex;

	LARGE_INTEGER				GL_frequency;
	LARGE_INTEGER               m_networkFreqStampUser;
	LARGE_INTEGER				GL_exactFpsStamp;
	LARGE_INTEGER				m_stamp_date_temp;
	LONGLONG                    m_longMultiplier;
	
	//Path Globals
	CString                     GL_pathMapModels;
	MSG							m_msg;
	CString                     GL_pathGameFiles;
	CString                     GL_pathGameSounds;
	CString                     GL_workingDirectory;
	//global displays
	CTextureRenderObj			* m_fpsReading;
    CTextureRenderObj		    * m_pingReading;
    CTextureRenderObj			* m_tempTalk;
    //global objects/Dbs
	CFilterSpeechList         * m_filterSpeechDB;
	CMovementObjList          * m_entObjRuntime;
	CMovementObjList          * m_entObjList;
	CString                     m_loadedEntityDatabasePath; // absolute path of the last successfully loaded EDB
	CViewportList             * m_runtimeViewportDB;
	CViewportList             * m_viewPortDB;
    ControlDBList             * m_controlsList;
    CMultiplayerObj           * m_multiplayerObject;
	NetworkGameLogic          * m_netGameLogicObject;

	//Menus
	CMenu3DObjList			  * m_menuDataBase;
	CMenu3DObjList			  * m_runtimeMenuDataBase;
	CMenuVisualList			  * m_menuDatabase;
	CString                    m_loadedMenuSystemPath; // absolute path of the last successfully loaded MOD
	CMenuVisualList			  * m_runtimeMenuDatabase;
	CMenuFontList			  * m_menuFontList;
	CMenuImage				  * m_mouseCursors[CURSOR_MAX];
	int							m_currentCursor;
	int							m_showMenuCursor;
	CMenuVisualList			  * m_menuIconDatabase;
	CMenuListItem			  * m_draggingItem;
	int							m_lastItemID;
	int							m_lastItemAmount;
	bool						m_releaseDraggingItem;
	bool						m_deleteDraggingItem;
	//CMenuImage				  * m_draggingItem;
	CMenuTextBox			  * m_systemReadout;
	CMenuTextBox			  * m_chatReadout;
	CMenuWindow				  * m_menuChat;
	CMenuTextBox			  * m_menuChatInput;
	CWindowRadar				m_radar;
	bool						m_bIsDragging;
	//CtextReadoutObj           * m_systemReadout;

	DxDeviceClass             * m_dxRenderer;
	CSymbolMapperObjList      * m_symbolMapDB;
	MatrixARB                   m_matLib;
	CABFunctionLib              m_cabLib;
	CTextureRenderObjList     * m_textRenderList;
	CWaterZoneObjList         * m_waterZoneDB;
	CEnvironmentObj           * m_environmentPtr;//
	CCollisionObj             * m_collisionBase;
	CAIObjList                * m_aiList;
	CCollisionObjList         * m_collisionDatabase;
	CAIScriptObjList          * m_scrList;
	CCameraObjList            * m_cameraList;
	CSoundTrackObjList        * m_soundTrackList;
	CSkillObjectList          * m_skillDatabase;
	CCommonCtrlList           * m_commCtrlDB;
	CTextureDBObjList         * m_textureDataBase;
    CTextureDBObjList         * m_dynamicTextureDataBase;
	CString                   * m_textureSearchPath;
	int                         m_searchPathCount;
	CStateManagementObj       * m_renderStateObject;
	CTextureDBObjList         * m_staticTextureDataBase;
	CGlobalMaterialShaderList * m_cgGlobalShaderDB;
	CViewToSurfaceDB          * m_toRenderSurfaceDB;
	CTranslucentPolyList      * m_translucentPolyList;
    CTranslucentPolyList      * m_translucentMirrorPolyList;
	CTranslucentPolyList      * m_shadowObjectsList;
	CTranslucentPolyList      * m_renderContainerList;
	CTranslucentPolyList      * m_renderContainerAlphaFilterList;
    CReferenceObjList         * m_libraryReferenceDB;
	CWldObjectList            * m_curWldObjPrtList;
	CWldObjectList            * m_worldTriggerList;
	CRTLightObjList           * m_runtimeLightSys;
	CWldObjectList            * m_worldEnviormentList;
	CEXMeshObjList            * m_visualsRenderList;
	EnvParticleSystemList     * m_particleDatabase;
	CDynamicOverlayList       * m_inViewEntityList;
	CHousingCacheObjList      * m_houseingCache;
	CHousingObjList           * m_housingDB;
	CLightningObjList         * m_lightningSystem;
	IDirectSound8             * m_dirSnd;
	LPDIRECTSOUND3DLISTENER     m_soundListener;
	CSoundBankList            * m_soundManager;
	CWldObjectList            * m_wldObjList;
	CWldObjectList            * m_worldMirrorList;
	CWldObjectList            * m_worldRenderList;
	CWldObjectList            * m_worldTransferList;
	CMovementObjList          * m_shadowEntityList;
	CWldObjectList            * m_worldListTemp;
	CExplosionObjList         * m_explosionDBList;
	CExplosionObjList         * m_explosionRuntimeList;
	CMissileObjList           * m_missileDB;
    CMissileObjList           * m_missileRuntime;
	CDotClassList			  * m_dotDB;
	CIconSysList              * m_iconDatabase;
	CSpawnObjList             * m_spawnGeneratorDB;
	CPersistObjList           * GL_ScenePersistList;
	CEXScriptObjList          * m_sceneScriptExec;
	CAIWebObjList             * GL_aiWebDatabase;
	CBackGroundObjList        * m_backGroundList;
	CWldGroupObjList          * m_worldGroupList;
	CShaderObjList            * m_shaderSystem;
	CShaderObjList            * m_pxShaderSystem;
	CMovieRuntimeObjList      * MovieRList;
	CTextureSwapObjList       * TextureSwapList;
	CGFontObjList             * m_cgFontList;
	CTextCollectionList       * m_textInterfaceList;
	CFontObjList              * m_fontList;
	CEXMeshObjList            * m_tempWldObjCache;	
	//CHudObjList               * m_hudList;
	CCommerceObjList          * m_commerceDB;
	CBankObjList              * m_bankZoneList;
	CSafeZoneObjList          * m_safeZoneDB;
	CServerItemGenObjList     * m_serverItemGenerator;
	CPortalSysList            * m_portalDatabase;
	CElementObjList           * m_elementDB;
	CStatObjList              * m_statDatabase;
	CAIRaidObjList            * m_aiRaidDatabase;
	CBattleSchedulerList      * m_battleSchedulerSys;
	CWorldChannelRuleList     * m_worldChannelRuleDB;
	CObjectCollectionList     * m_objectLibrary;
	CScreenFlashObj           * m_screenFlashModule;
	CPendingCommandObjList    * m_pendingObject;
	CMusicAX                  * m_musicPlayer;
	CPostProcessModuleManager * m_postProcessor;
	CAXLanguageFilter         * m_langFilter;
	CAXContentClient          * m_contentDownloadManager;
	CCharonaDB                * m_charonaCache;
	DWORD                       m_lastChatCommandStamp;
	bool                        m_highResTimerEnabled;
	CLodLoadBalancer            m_lodBalancer;
	CAttribQueue			  * m_attribQueue;
	CTextEffectDB             * m_textEffectRuntime;

	//global meter
	CMenuBarObj               * m_globalStatusMeter;
	float                       m_globalStatusMeterStartX;
	float                       m_globalStatusMeterStartY;
	float                       m_globalMeterFontSizeX;
	float                       m_globalMeterFontSizeY;
	CString                     m_globalStatusMeterText;
	BOOL                        m_showGlobalMeter;
	CRITICAL_SECTION            m_globalMeterSection;
	BOOL                        m_autoDownloadMissingAssets;
	CString                     m_contentUrl;
	CString                     m_advUrl;
	CString                     m_advUrlBasic;
	CString                     m_advUrlVending;
	CString                     m_installPath;
	CString						m_cacheDirectory;
	CTextureEX                * m_loadingTexture;
	DWORD                       m_loadingTextureUpdateTimeST;
	CRITICAL_SECTION            m_stateSection;
	D3DVECTOR                   m_globalCursorPos;
	CString						m_globalInteractCommand;

	//selction state
	CSelectionEffectList      * m_selectionEffectDB;
	
	//Dynamic font system
	CTextureEX                * m_dynamicFontTexture;
	BOOL                        m_initFontSystem;

	//reference objects with realtime collision tracking
	CWldObjectList              m_skeletalReferenceObjCache;
	BOOL                        m_resetReferences;
	//Temp Vars
	EnvParticleSystem         * m_particleTest;
	CCameraAdvObj			  * m_cameraModule;
	CItemManager			  * m_itemManager;
	CInput	                  * m_inputReference;
	CString						m_jnlLastKeyStatement;
	CString						m_jnlLastQuestDescription;
	CString						m_jnlLastNpcName;

	//BOOL
	LPDIRECT3DDEVICE9         m_3dDevice;

	
	

    //Functions
	void ShowMessageBox(CString strTitle, CString strMessage);
	CMenuInventory* GetActiveInventory(MENU_IDS eMenuID);
	void SetActiveInventory(MENU_IDS eMenuID, MENU_IDS eInventoryID, bool bIsExclusivelyActive);
	void OnInventoryItemSelected(MENU_IDS eMenuID);
	int AddToQuickbar(CMenuInventoryItem* pItem);
	bool RemoveFromQuickbar(ITEMDATA_STRUCT_EX item);
	bool RemoveFromQuickbar(int iQuickbar, int iSlot);
	BOOL LoadQuickbarConfig(const ITEMLIST *pInventory);
	void SaveQuickbarConfig();
	void UpdateQuickbarServerSide();
	void RefreshQuestJournal(int iQuestListID, bool bSaveCurrentSelection);
	void DestroyAllTexturesFromDatabase();
	void SpawnGenDatabaseInList(int sel, CDialog *pSpawnGeneratorDlg = 0);
	void InListTextureDB(int sel, CDialog* pTextureDatabaseDlg = 0);
	BOOL SaveTextureBank(CString fileName, int compressed, int maxTextureSize);

	BOOL ConnectToServerNode(CString serverIP);
	void SkeletalTextureReassignmentWCompile(CSkeletonObject * skeletonObj);
	void SetMenuViewState();
	//bool ActivateItem(int glID, CMenuListItem *listItem = NULL);
	bool ActivateItem(CMenuInventoryItem *pInventoryItem);
	BOOL ArmEntityItemSys(CMovementObj *mainPtr, int itemID, int dupPassedOption);
	bool DeactivateItem(CMenuInventoryItem *pInventoryItem);	
	BOOL DisarmEntityItemSys(CMovementObj *mainPtr, int itemID, int dupPassedOption);
	BOOL ActivateMovementEntity(CMovementObj * existingEntity,
								int aiCfg,
								D3DMATRIX locationMatrix,
								int cameraIndex,
								int optWorldChannel,
								int indexLoc,
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
								CString & worldName);
	BOOL ChangeToFirstPersonView(CMovementObj * currentEntity);
	BOOL ChangeToThirdPersonView(CMovementObj * currentEntity);
	void DestroyRuntimeEntity(CMovementObj * runtimeEntity,CSpawnObjList * spawnGeneratorDB,CMovementObjList * runtimeEntityList);
	void DestroyRuntimeEntity(CMovementObj * runtimeEntity,
						      CSpawnObjList * spawnGeneratorDB,
						      POSITION posLast,
						      CMovementObjList * runtimeEntityList);
	void ClearBoundCamerasViewports(int & entityTrace);
	void InitEditorStatus();
	BOOL OverrideFireQue(   int             & fireQueIndex,
							D3DVECTOR       & target,
							CMovementObj    * entityPtr,
							CMissileObjList  * missileRuntime);
	BOOL DoExpEffect(int expRef,int networkID,CExplosionObjList * explosionRuntimeList,float pushPower,float range);
	void ExecuteFireQue(CMovementObj     * entityPtr,
						CFireRangeObject * fireRange,
						float            & pitchRotation,
						D3DMATRIX        & positionalMatrix,
						int              & entityTraceNumber,
						D3DVECTOR        * overrideTarget,
						int              & fireRangeIndex,
						BOOL               sendOverNetwork,
						float            & yawRotation,
						int              & missileDBIndex,
						CMissileObjList  * missileRuntime);
	void ExecuteFireQue(CMovementObj     * entityPtr,
						D3DMATRIX        & directionalMatrix,
						CFireRangeObject * fireRange,
						int              & fireRangeIndex,
						BOOL               sendOverNetwork,
						int              & missileDBIndex,
						CMissileObjList  * missileRuntime);
	BOOL DoMissile( D3DMATRIX matrix,
					int indexDB,
					float curSpeed,
					int trace,
					int targetTrace,
					D3DVECTOR secondaryAdjust,
					float rotationAfter,
					float *missileSpeed,
					int teamref,
					float &kickRet,
					CMissileObjList * missileRuntime);
	void OverideMissile(    CMovementObj     * entityPtr,
							float            & pitchRotation,
							D3DMATRIX        & positionalMatrix,
							int              & entityTraceNumber,
							BOOL               sendOverNetwork,
							float            & yawRotation,
							int                missileType,
							CMissileObjList  * missileRuntime);
	BOOL GenAndMount(CMovementObj * hostPtr,
		             int aiIndex,
					 CMovementObjList * entObjRuntime,
					 CMissileObjList  * missileRuntime);
	BOOL IndexEntityAnimations(CMovementObj * ptrLoc,long currentTickCount,BOOL soundOnly,CMissileObjList * missileRuntime);
	void EntityAnimationManagmentCallback(CMovementObj * entLoc);
	void SkeletalFiringSystem(  D3DMATRIX       & positionalMatrix,
								CSkeletonObject * skeletonPtr,
								int             & entityTraceNumber,
								float             pitchRotation,
								CMovementObj    * entityPtr,
								float             yawRotation,
								CMissileObjList  * missileRuntime);
	BOOL VirtualCollisionAdjust(D3DVECTOR * modPosition,
								D3DVECTOR & currentPosition,
								D3DVECTOR & lastPosition,
								int         characterWidth,
								int         characterHieght,
								D3DMATRIX & entityMatrix,
								BOOL        antiGravity);
	void HighLevelMeshRender(   CEXMeshObj * meshex,
								D3DVECTOR & locationVect,
								BOOL mirrorEnable);
	BOOL SkeletalFiringSystem(  CMovementObj		* entityPtr,
								CArmedInventoryObj	* armedPtr,
								CMissileObjList		* missileRuntime);
	BOOL DeMount(CMovementObj * hostPtr,CMovementObjList * entObjRuntime);
	BOOL LoadScene(CString fileName);
	void CacheGeneratedRenderStuff(int mirrorBased);
	BOOL LoadSceneInfo();
	void DeleteSpawnDatabase();
	void DestroyWorldObjects();
	void DestroyEnvironment();
	void DestroyEntityRuntimeList();
	void DeleteWorldGroupList();
	void DeleteScriptList(CEXScriptObjList ** focusLoc);
	BOOL ExecAllAttributes(CEXScriptObjList * scriptList);
	BOOL InitializeOjectsIntoScene();
	void RebuildTraceNumber();
	void UpdateEntityPersist(CPersistObjList ** pList);
	BOOL SetAllRenderStatesIM();
	void InitializeEnvironment(BOOL EditorRefresh);
	BOOL SetViewMatrixAB( D3DMATRIX& mat, D3DXVECTOR3& vFrom, D3DXVECTOR3& vAt, D3DXVECTOR3& vWorldUp );
	BOOL SetViewMatrixAB( D3DMATRIX& mat, D3DVECTOR& vAt, D3DVECTOR& vWorldUp );
	HRESULT D3DUtil_SetProjectionMatrix( D3DMATRIX& mat, FLOAT fFOV, FLOAT fAspect,FLOAT fNearPlane, FLOAT fFarPlane,float hieghtBase ,float & clipWidth, float & clipHeight);
    void WriteDebugFileLive(int errorcode1,int errorcode2);
	void PlayMoveFullscreen(const char * pszFileName);
	BOOL LoadMissileDatabase(CString fileName);
	BOOL LoadExplosionFile(CString fileName);
	BOOL LoadMoviesRuntimeDB(CString fileName);
	BOOL LoadControlCfgList(CString fileName, CMenuListBox *pListBox = NULL);
	BOOL LoadControlCfgFromDisk(CString fileName, CMenuListBox *pListBox = NULL);
	BOOL LoadVportCameraConfigurations(CString fileName);
	void LoadAnimTextureSwapDB(CString fileName);
	BOOL LoadTextFontInterfaceFromFile(CString fileName);
	BOOL LoadSoundManager(CString fileName);
	BOOL CreateDisplayIM(BOOL fulscreenLoc,int driverLoc,int videoModeLoc,int widthLoc,int hieghtLoc,int depthLoc);
	BOOL LoadEntityDatabase(CString fileName);
	void RunRuntimeMovie(int indexLoc);
	BOOL LoadHudDatabase(CString fileName);
	BOOL LoadMenuSys(CString filename);
	BOOL LoadParticleDatabase(CString fileName);
	BOOL LoadTextureBank(CString fileName);
	BOOL LoadSoundTrackDB(CString fileName);
	BOOL LoadAiData(CString fileName);
	BOOL LoadGlobalCommerceData(CString fileName);
	BOOL LoadBankZonesData(CString fileName);
	BOOL LoadSafeZonesData(CString fileName);
	BOOL LoadStaticTextureBank(CString fileName);
	BOOL LoadWaterZoneDB(CString fileName);
	BOOL LoadPortalDatabase(CString fileName);
	BOOL LoadElementDatabase(CString fileName);
	BOOL LoadStatsDatabase(CString fileName);
	BOOL LoadSkillsDatabase(CString fileName);
	BOOL LoadAICollisionDB(CString fileName);
	BOOL LoadIconDB(CString fileName);
	BOOL LoadHouseZoneDB(CString fileName);
	BOOL LoadHouseDB(CString fileName);
	BOOL LoadAIRaidDB(CString fileName);
	BOOL LoadCommControls(CString fileName);
	BOOL LoadArenaSys(CString fileName);
	BOOL LoadWorldRules(CString fileName);
	BOOL LoadShaderLibrary(CString fileName);
	BOOL LoadGlobalShaderLibrary(CString fileName);
	BOOL LoadPixelShaderLibrary(CString fileName);
	BOOL LoadPostProcessorDB(CString fileName);
	BOOL LoadDotDB(CString fileName);
	BOOL InitWorldObject(CWldObject * ptrLoc);
	void DestroyAllMissileClasses();
	void DestroyAllDotClasses();
	void DestroyAllMovieObjects();
	void DestroyControlList();
	void InitializeControlDB();
	void DestroyAllVieports();
	void DestroyTextureCollectionList(CTextureSwapObjList *textureColList);
	BOOL InitAllTexCollections();
	void UpdateMeterAdv(float currentPosition, CString infoTxt);
	void DestroyTextInterface();
	BOOL WriteCfgFile(CString filename);
	void RestoreVports();
	void CalibrateScreenSnap(int unitX,int unitY);
	//void RefreshMenus();
	void DestroyEntityList();
	void InitAllDatabaseEntitys();
	void CalculateAllMovieDisplayRects();
	void DeleteHudList();
	void DestroyAllParticleInterfaces();
	BOOL DeleteParticleSystem();
	void ReinitAllTexturesFromDatabase();
	void ReinitAllTexturesFromDatabaseRemoveUnused();
	void DestroyMissileObject(CMissileObj* mislPtr);
	void DestroyDotObject(CDotClass *dotPtr);
	void DestroyTextureCollection(CTextureSwapObj* txtObjPtr);
	void ReinitTextureUseage();
	BOOL LoadEntityDatabaseAsTemp(CString fileName);
	BOOL LoadSelectionEffectsDB(CString fileName);
	void UpdateMeter(float currentPosition);
	void SetupGlobalMeter(  float length, 
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
							float fontSizeY);
	void RenderMeter();
	void TextureReassignEntity(CMovementObj * ptrLoc);
	BOOL SetMenuTextItem(int controlID, CString string);
	BOOL AddSelectedFromMenuList(int controlID,int glid,int quanity);
	BOOL ProssesCfgFile(CString filename);
	void SaveControlConfig(CString fileName);
	int BonesSystemRender(   CSkeletonObject * sklPtr,D3DVECTOR offsetVector,D3DMATRIX positionMatrix);
	void MultipassRenderMesh(CEXMeshObj * mesh,int lod, D3DMATRIX & wldMatrix);
	BOOL RunScript(CString fileName);
	BOOL RealtimeSegmentTest(   D3DVECTOR & lastPos,
								D3DVECTOR & curPos,
								D3DVECTOR * normalRet,
								D3DVECTOR * collisionIntersect,
								int * actualCollisions,
								int * consideredCollision);
	BOOL RealtimeSegmentTestUseTable(D3DVECTOR & lastPos,D3DVECTOR & curPos);
	void Sound3DCallback();
	//CString ParseData(unsigned int & msg);
	void ReinitTextureLibraryTextures();
	BOOL CreateDisplayIM(BOOL fulscreenLoc,int driverLoc,int videoModeLoc,int widthLoc,int hieghtLoc,int depthLoc,HWND hwnd);
	
	//-------Sound functions-----------//
	BOOL MakeDirectSound(HWND hwnd,IDirectSound8 **lpds);


    //-------Texture Functions---------//
	void SkeletalTextureReassignment(CSkeletonObject * skeletonObj);
	void SkeletalTextureReassignment(CSkeletonObject * skeletonObj, int mipCount);
    void SkeletalTextureReassignmentTemp(CSkeletonObject * skeletonObj,int tempChannel);
	void LoadTexturesForMenu(CMenu3DObj * menu3DObj);
	BOOL AddTextureToDatabaseTemp(CEXMeshObj * meshObj,int tempChannel);
	BOOL AddTextureToDatabase(CEXMeshObj * meshObj,int mipMapCount);
	void ReinitTextureUsage(CEXMeshObj * mesh);
	BOOL AddTextureToDatabase(CEXMeshObj * meshObj);
	void InitSearchPaths();

    //-------Version Functions---------//
	int EvaluateClientContentVersion();

	//-------GLOBAL MENU ACCESS--------//
	BOOL OpenMenu(int visualID, bool allowMultipleInstances);
	BOOL UseMenu(int dbIndex, int miscInt, int passedInVar, int passedInCommand);
	BOOL ReinitMenuSys(CMenu3DObj * runtime,int miscInt);
	void InListActionsPlain(int currentSel, CSliderBarObj * slideBarObj);//Menu slider helper
	void InListActions(int currentSel, CSliderBarObj * slideBarObj, BOOL filterUnassigned);//menu slider helper
	POSITION AddStringToMenuList(int controlID,
								 CString string,
								 int visualRefIndex,
								 int miscInt,
								 int miscInt2,
								 CString regItemName,
								 int mode,
								 CString costExt,
								 bool special = false,
								 int miscInt3 = 0,
							     int miscInt4 = 0);
	LONGLONG GetTimeStamp();
	LONGLONG GetElaspeTimeMS(LONGLONG & lastStampPoint);
	void ShowHoverText(int iVisualID, CString strMessage);

	bool RealtimeHousingSegment(D3DVECTOR & lastPos,
								D3DVECTOR & curPos,
								D3DVECTOR * normalRet,
								D3DVECTOR * collisionIntersect,
								float & bestDistance);
	bool ValidateFile(CString & fullPath, unsigned long fileCRC, BOOL verifyFileSizeRequired);
	bool ReadUrl(CString & url, CString & retString);
	bool                 ParseUrlStringToData(int set, 
											  CString urlParse, 
											  CString & fileName, 
											  long * fileSize, 
											  CString & cabName, 
											  CString & offsetDir, 
											  unsigned long * CRC,
											  unsigned long * cab_CRC,
											  BOOL * checkCrc);
	bool ParseUrlGetLine(int & curPos, CString & urlData, CString & lineString);
	void AXPumpMsgs();
	void SetCloseDownCallState(BOOL state);
    BOOL GetCloseDownCallState();
	void UpdateRiderOfPossedEntity(CMovementObj * entityPtr);
	void TempMenuCallBack();
	BOOL LoadByFileName(CString fileName);
	DWORD GetElaspeTimeMS_DW(DWORD & lastStampPoint);
	DWORD GetTimeStamp_DW();
	void AddLine_safe(CString string);
	void UI_Log(CString msg);
    void UI_Log(CString msg,CString msg2);
	void SendChatString(CString string);
	int CreateCharacter_Send(int spawnPosIndex,
							 CString characterName,
							 CMovementObj * entityConfigured);
	BOOL InstanceMenu(int visualID, int newVisualID);
	BOOL SegmentCollisionToAllEntities(BOOL filterPrimary,
									   D3DVECTOR & start,
									   D3DVECTOR & end,
									   D3DVECTOR & intersect,
									   float segmentThickness,
									   float & bestDist);
};
