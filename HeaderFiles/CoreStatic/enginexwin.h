#include "RMWin.h"

#include <mmsystem.h>
#include <DSound.h>
#include "resource.h"
#include "..\AuraUI\resource.h"
#include "CWldObject.h"
#include "CBackGroundClass.h"
#include "CMovementModel.h"
#include "CCameraClass.h"
#include "CEnvironmentClass.h"
#include "CTextClass.h"
#include "CPersistClass.h"
#include "CMovieRuntimeClass.h"
#include "ControlsClass.h"
#include "AIclass.h"
#include "CMissileClass.h"
#include "CAttributeClass.h"
#include "CDplayPlayerClass.h"
#include "CTextureSwapClass.h"
#include "CEXScriptClass.h"
#include "CInventoryClass.h"
#include "CEXMeshClass.h"
#include "CTextureDBClass.h"
#include "CNetworkRecordClass.h"
#include "C2DVectorDrawClass.h"
#include "CHudClass.h"
#include "EnvParticleSystem.h"
#include "CFrameObj.h"
#include "viewportclass.h"
#include "CCollisionClass.h"
#include "CLightTrackerClass.h"
#include "textureObj.h"
#include "CAITraceClass.h"
#include <afxcmn.h>
#include "CSpawnClass.h"
#include "CWorldGroupClass.h"
#include "CMultiplayerClass.h"
#include "CSkeletalClass.h"
#include "CSSoundRange.h"
#include "CShadowClass.h"
#include "CCharacterDefinitionsClass.h"
#include "CResourceClass.h"
#include "CTextReadoutClass.h"
#include "CTextRenderClass.h"
#include "CGFONTCLASS.h"
#include "CAIWebClass.h"
#include "CAIScriptClass.h"
#include "CStructuresMisl.h"
#include "CExplosionClass.h"
#include "CSoundTrackClass.h"
#include "CBoneCursorClass.h"
#include "CMenu3DClass.h"
#include "CTranslucentPolyCache.h"

// #include <dmusicc.h>  // DirectMusic removed from modern Windows SDK; unused (no DMUS_/IDirectMusic symbols)
// #include <dmusici.h>  // DirectMusic removed from modern Windows SDK; unused
#include "CObjectLibraryClass.h"
#include "CLightningEffectClass.h"
#include "CStateManagementClass.h"
#include "CLaserClass.h"

#include "AccountViewerModule.h"
//#include "CSkillClass.h"
//#include "CRenderContainer.h"
//#include "CReferenceLibrary.h"
#include "ProgressMeterDlg.h"
#include "RuntimeEntityViewerDlg.h"
#include "CurrentWorldsDlg.h"
#include "NewAccountsModule.h"
#include "NodeImportDlg.h"
#include "CSoundManagerClass.h"
#include "CCurrencyClass.h"
#include "CCommerceClass.h"
#include "CBankClass.h"
#include "CSafeZones.h"
#include "CRTLightClass.h"
#include "CStatClass.h"
#include "CServerItemGen.h"
#include "CFilePatchClass.h"
#include "CMenuBarClass.h"
#include "CScreenFlashClass.h"
#include "CWaterZoneClass.h"
#include "CPortalSysClass.h"
#include "CElementsClass.h"
#include "CCompressionClass.h"
#include "CFilterSpeechClass.h"
#include "CHousingClass.h"
#include "CDynamicOverlay.h"
#include "CAIRaidClass.h"
#include "CGmPageClass.h"
#include "CCommonCtrObj.h"
#include "CBattleSchedulerClass.h"
#include "CWorldChannelRules.h"
#include "CShaderLibClass.h"
#include "CGlobalMaterialShaderLib.h"
#include "CFloraGenClass.h"
#include "CpendingCommandsClass.h"
#include "CReferenceImporterExporter.h"
#include <Afxdisp.h>
#include "AXmediaFrame.h"
//Optional Lib
#include "CMalInterface.h"

#include "CDplayModule.h"

//New Menu System
#include "AddMenu.h"
#include "EditCursors.h"
#include "EditFont.h"
#include "EditIcons.h"
#include "EditComboBox.h"
#include "EditListBox.h"
#include "EditTextBox.h"
#include "EditText.h"
#include "EditScrollBar.h"
#include "EditButton.h"
#include "EditToggle.h"
#include "EditProgBar.h"
#include "EditImage.h"
#include "EditEntity.h"
#include "EditInventory.h"

#include "CExplosionsDatabaseDlg.h"
#include "CWeaponSystemDlg.h"
#include "CEnvironmentSettingsDlg.h"
#include "CAISystemDlg.h"
#include "CSpawnGeneratorDlg.h"
#include "CTextureDatabaseDlg.h"

#ifndef ENGINEXWIN_H
#define ENGINEXWIN_H
// Quick menu commands (menu lives in MenuInterfaceModule.rc, same numbers)
#ifndef ID_QUICK_PLAYHERE
#define ID_QUICK_PLAYHERE               32787
#define ID_QUICK_BACKTOEDITOR           32788
#define ID_QUICK_WHEREAMI               32789
#define ID_QUICK_GOTO                   32790
#define ID_QUICK_EXPORTCHARACTER        32791
#define ID_QUICK_IMPORTCHARACTER        32792
#define ID_QUICK_GIVETOPSJIGGLE         32793
#define ID_QUICK_EXPORTOUTFITS          32794
#define ID_QUICK_COPYENTITY             32795
#endif

// The Quick-menu resource ID is defined in MenuInterfaceModule/resource.h,
// which is not included by the EngineX command-map translation unit.
#ifndef ID_QUICK_COPYITEMRACE
#define ID_QUICK_COPYITEMRACE 32796
#endif
#ifndef ID_QUICK_SENDTOGAME
#define ID_QUICK_SENDTOGAME 32797
#endif
#ifndef ID_QUICK_SETGAMEPATHS
#define ID_QUICK_SETGAMEPATHS 32798
#endif
#ifndef ID_QUICK_NEWRACEWIZARD
#define ID_QUICK_NEWRACEWIZARD 32799
#endif

class EngineXWin : public RMWin
{
public:
	// Explosions Database Dialog
	CExplosionsDatabaseDlg *m_pExplosionsDbDlg;

	// Weapon System Dialog
	CWeaponSystemDlg *m_pWeaponSystemDlg;

	// Environment Settings Dialog
	CEnvironmentSettingsDlg *m_pEnvironmentSettingsDlg;

	// AI System Dialog
	CAISystemDlg *m_pAISystemDlg;

	// Spawn Generator Dialog
	CSpawnGeneratorDlg *m_pSpawnGeneratorDlg;

	// Texture Database Dialog
	CTextureDatabaseDlg *m_pTextureDatabaseDlg;

	//Media Link
	CAXmediaFrame * m_axmedia;
    
	BOOL m_refreshInterface;
	//INTERFACE HOOK (MFC LINE)
	CMalInterface * m_newMalInterface;
private:
	CMenuWindow	*m_pClipboardMenuWindow;
	CMenuVisual *m_pClipboardMenuVisual;

	WORD edgeBuffer[30000];
	int g_switch;
	
	int m_dbgSequencer;
	
	int g_currentWidth;

	
HANDLE                        m_globalSqlHandle;
DWORD                         m_globalSqlThreadID;

float testLength;




BOOL m_snapshotPacketQue;
BOOL m_editorTextModeOn;
//CSymbolMapperObjList * m_gStatus->m_symbolMapDB;


long m_skillTimeStampLocal;
BOOL m_placementHotKeyEnabled;


int m_totalEntitypolys;

CDialog m_progressDlg;
CDialog * m_progressInternal;
int m_miscIndex;
BOOL GL_takingDamage;
long GL_takingDamageStamp;
long m_useItemTimeStamp;



//plugin separate thread modules
CCurrentWorldsDlg    * m_currentWldsSpawnPointModule;
CAccountViewerModule * m_accountViewerModule;
CRuntimeEntityViewerDlg * m_entityViewerModule;
#ifndef _ClientOutput
CNodeImportDlg         m_nodeImporterModule;
#endif
CNewAccountsModule   * m_accountModule;

BOOL                 m_restrictMovement;
DWORD              m_respawnTimeStamp;



long                   m_itemRequestDuration;
long                   m_itemRequestStamp;
//end global prefernces
    

BOOL       m_triggerTestTemp;

float      m_forceSpeed;
float      m_lastKnownFov; 


DWORD m_movementStamp;
int GL_counterThing;
long m_networkPingStamp;
long m_networkPingDuration;
CSkeletonObject * GL_skeletalTest;
BOOL setIt;
CString GL_miscHiResStamp;
float GL_mouseHiResX;
float GL_mouseHiResY;

int gl_indexTemp;
long GL_mislTimeStampTest;
//Seach path
int GL_exactFPS;
double GL_physicsAdjust;
double GL_physicsLastAdjust;
BOOL GL_specularOn;

int issex;
int GL_TracerMan;
//GLOBAL frame world coordinate sys


BOOL GL_showBoundingBoxs;
CFrameObj *test;
BOOL GL_dedicatedRenderLoop;
BOOL GL_QueOutPut;
//GLOBAL Text File defined Issues
BOOL InvertMouse;
int GL_dumpMessagesVar;
int GL_pingFrequency;
int GL_pingFrequencyVar;

//GLOBAL OPTIMIZERS

int GL_lastMultitextureMode1;
int GL_lastMultitextureMode2;
int GL_lastMultitextureMode3;
int GL_lastMultitextureMode4;
int GL_lastMultitextureMode5;
int GL_lastMultitextureMode6;
int GL_lastMultitextureMode7;
//mouse 
int GL_mouseCurrent;
//Pen Globals
HPEN GL_GreenPen;
CHudObj *GLHUDTEST;
//MISC
BOOL GL_RecordNetworkActivity;
BOOL EntityCollisionOn;

//EFFECT
int Blend_TwoXValid;
//timing 
DWORD updateTimeBasis;
DWORD lastUpdatesdTimeBasis;
DWORD GL_RenderTimingBasis;
DWORD GL_LastUpdatedRenderTime;
DWORD GL_MovTimingBasis;
DWORD GL_AnimTimingBasis;
DWORD GL_LastUpdatedAnimTime;
DWORD GL_MouseState;
DWORD GL_MouseStateVarial;
//Scene initial calls
//timed calls



//PORT stuff
//ImediateMode Globals
D3DMATERIAL9 GlobalStateMaterial;
D3DMATERIAL9 g_materialInit7;
float VicinityAdditionalUnits;
BOOL VicinityObjLoadCalled;
CEXScriptObjList * GL_scriptList;
CString            m_scriptFilePath;   // .aut the Scripter bar loaded / will build to (empty = GameFiles\autoexec.aut)
//client
int LagCount;
int MaxLagCount;
BOOL RENDERALL;

CString ErrorMessage;
int ResolutionHieght;
int ResolutionWidth;
//savable scene varials

IDirectSoundBuffer *musicLoop;


int maxBulletHoleObjects;

//performance  issues



//timely functions

//text display interface
HTREEITEM TextDisplayTreeROOT;
int updateEditorVarial;

//initializes
  D3DRMBOX2  D3DboxInit;
  float MatrixInit[4][4];
//mouse
BOOL SelectionModeOn;
int MouseControlX;
int MouseControlY;
float GlobalMouseSensitivity;
int GlobalLastPosMouseX;
int GlobalLastPosMouseY;
int GlobalMouseAdjustX;
int GlobalMouseAdjustY;
RECT GlobalMouseRect;
POINT GlobalMouseCenterPoint;

BOOL mouseButtonOneDown;
BOOL mouseButtonTwoDown;
//lists
//Trace ways objects
BOOL initOnce;	
//network record list
	CNetworkKeyObjList       * NetworkRecordList;  
//Currency
	CCurrencyObj             * m_currencyObject;
	
	
//toolbars  UI stuff
    CDialogBar  m_dlgBarObject;
	CDialogBar  m_wndDlgBar;
    CDialogBar  m_backgroundDlgBar;
	CDialogBar  m_controlsDlgBar;
	CDialogBar  m_moviesDlgBar;
	CDialogBar  m_entityDlgBar;
	CDialogBar  m_camVptDlgBar;
	CDialogBar  m_mainMenus;
	CDialogBar  m_mainCharacter;
	CDialogBar  m_mainEnvironment;
	CDialogBar  m_mainFX;
	CDialogBar  m_mouseDlgBar;
	CDialogBar  m_2DinterfaceBar;
	CDialogBar  m_enviormentBar;
	CDialogBar  m_textFontDlgBar;
	CDialogBar  m_musicDlgBar;
    CDialogBar  m_missileDlgBar;
	CDialogBar  m_bulletHoleDlgBar;
	CDialogBar  m_multiPlayer;
	CDialogBar  m_animatedTextureDBDlgBar;
	CDialogBar  m_scriptDlgBar;
	CDialogBar  m_textureDBDlgBar;
	CDialogBar  m_hudDBDlgBar;
	CDialogBar  m_menuDBDlgBar;
	CDialogBar  m_particleDBDlgBar;
	CDialogBar  m_spawnGenDBDlgBar;
	CDialogBar  m_aiDBDlgBar;
	CDialogBar  m_haloEffectsDlgBar;
	CDialogBar	m_explosionDlgBar;
	
	
    float GlobalIndex;
	BOOL CreateScene();
int PINGLAST;

int RENDERLOOPTIME;

int LIGHTSINUSE;
int skipVarial;
int SKIPFACTOR;
int PREVTICCOUNT;
int CURRTICCOUNT;
int GL_totalTimePassed;
int GL_interpolateFpsBasis;
int FPSCURACTUAL;
//int FPSLIMIT;

int GL_FpsTimeTrack;
int GL_FpsLastTime;
//global varials misc
int AnimationTimingCurIndex;
int AnimationTimingVarial;
int EnvironmentAnimationTimingCurIndex;
int EnvironmentAnimationTimingVarial;


//Handle
HWND hwndGlobal;

//test
CFrameObj* host;
D3DVECTOR * testVertexDB;
DWORD CountGlob;


//sound


IDirectSoundBuffer *sndLoc;
IDirectSound3DBuffer *sndLoc3D;
IDirectSound3DListener * sound3DListener;

//MIRROR VARIABLES
	BOOL GL_MirrorRendering;
  //if set to true will black out screen

	//Variables for the new menu system
	CMenuWindow		 m_cursorsWindow;		//Temperary window used to show the menu system cursor when we edit it
	CMenuWindow		 m_fontsWindow;			//Temperary window used to render fonts when we edit them
	CMenuWindow		 m_iconsWindow;			//Temperary window used to render icons when we edit them
	CAddMenu		*m_addMenuDlg;			//Add Menu Dialog(Needed to allow visible editing)
	CEditCursors	*m_cursorsDlg;			//Cursors Dialog(Needed to allow visible editing)
	CEditFont		*m_fontsDlg;			//Fonts Dialog(Needed to allow visible editing)
	CEditIcons		*m_iconsDlg;			//Icons Dialog(Need to allow visible editing)
	CEditComboBox	*m_editComboBoxDlg;		//Edit Combo Box Dialog(Needed to allow visible editing)
	CEditListBox	*m_editListBoxDlg;		//Edit List Box Dialog(Needed to allow visible editing)
	CEditTextBox	*m_editTextBoxDlg;		//Edit Text Box Dialog(Needed to allow visible editing)
	CEditText		*m_editTextDlg;			//Edit Text Dialog(Needed to allow visible editing)
	CEditScrollBar	*m_editScrollBarDlg;	//Edit Scroll Bar Dialog(Needed to allow visible editing)
	CEditButton		*m_editButtonDlg;		//Edit Button Dialog(Needed to allow visible editing)
	CEditToggle		*m_editToggleDlg;		//Edit Toggle Dialog(Needed to allow visible editing)
	CEditProgBar	*m_editProgBarDlg;		//Edit Progress Bar Dialog(Needed to allow visible editing)
	CEditImage		*m_editImageDlg;		//Edit Image Dialog(Needed to allow visible editing)
	CEditInventory	*m_editInventoryDlg;	//Edit Inventory Dialog(Needed to allow visible editing)
	CEditEntity		*m_editEntityDlg;		//Edit Image Dialog(Needed to allow visible editing)
	CMenuVisualList	*m_menuThemeDatabase;	//A linked list of themes(They are just MenuWindows with every type of visual item)

//public funtions
public:
    EngineXWin();

private:
    //MIRROR HELPER FUNCTIONS
	FLOAT* MirrCreatePlane(FLOAT *plane, D3DXVECTOR3& a, D3DXVECTOR3& b, D3DXVECTOR3& c );
	void OrderVerticies(CEXMeshObj *mesh, ABVERTEX *vOut);
	D3DVECTOR GetOffset(ABVERTEX *vIn);
	//Get last position
	//RENDER LOOPS
	BOOL MovementUpdate(DWORD curTickCount);
    //optimize
	void TallyLightsInUse();
    //startup
	void BaseSceneSetup();
    //mouse
    //viewports
	BOOL TearDownAllVieportInterfaces();
	BOOL DisableVieport(int viewportIndex);
	BOOL DisableAllVieports();
	//BOOL ActivateVieport(CViewportObj * vptPtr);
	BOOL CreateCustomViewPort(int X1perc,int Y1perc,int X2perc,int Y2perc);
	void OnResetAll();
	BOOL IndexAnimations(int currentTickCount);
	BOOL SaveScene(CString fileName);
	BOOL LoadWorldObject(CString fileName,CWldObject ** retLoc);
	void TearDownWorldObjects();
	void WorldEnvironmentUpdate(CFrameObj* frame);
	//world objects
	void ExecAttributeOfObject(CWldObject * wldObjLoc);
	void FreeAllThenClose();
	void ControlsInLoop();
	void Effects();
    BOOL CreateDisplay(BOOL fulscreenLoc,int driverLoc,int videoModeLoc,int widthLoc,int hieghtLoc,int depthLoc);
	BOOL CreateBackGroundObject(CString bitmapName,int xMax, int yMax,int startX,int startY,RECT usageRect,BOOL movingMap);
    BOOL InitializeBackground(int numberOfInitialization);
	BOOL BackgroundInLoop(int inUse);
	void BackGroundScrollRight(int pixelIndex,int listIndex);
	void BackGroundScrollLeft(int pixelIndex,int listIndex);
	void BackGroundScrollDown(int pixelIndex,int listIndex);
	void BackGroundScrollUp(int pixelIndex,int listIndex);
	void DeleteBackground();
	void SqlAIToServer();
	//Render Functions
	void RenderLoop();
	void MapBatchStep();          // Display > Capture All Zone Maps
	// Quick menu (QuickTools.h)
	afx_msg void OnQuickPlayHere();
	afx_msg void OnQuickBackToEditor();
	afx_msg void OnQuickWhereAmI();
	afx_msg void OnQuickGoTo();
	afx_msg void OnQuickExportCharacter();
	afx_msg void OnQuickExportOutfits();
	afx_msg void OnQuickCopyEntity();
	afx_msg void OnQuickCopyItemRace();
	afx_msg void OnQuickNewRaceWizard();
	afx_msg void OnQuickSendToGame();
	afx_msg void OnQuickSetGamePaths();
	void QuickExportCharacterImpl(BOOL allOutfits);
	afx_msg void OnQuickImportCharacter();
	afx_msg void OnQuickGiveTopsJiggle();
	CMovementObj * QuickCameraEntity();
	void QuickRemoveTestCharacter();
	int          m_quickEditorTrace; // camera trace to return to (-1 = none)
	int          m_quickPlayTrace;   // Play Here test character (-1 = none)
	CString      m_quickLastGoTo;
	CString      m_quickOldTitle;
	CString      m_quickEntityDatabasePath; // last EDB loaded or saved in the Entity panel
	CString      m_quickMenuSystemPath;      // last menu system loaded or saved in the Menu System panel
	int          m_mapBatchIndex; // -1 = not running
	int          m_mapBatchPhase;
	DWORD        m_mapBatchStamp;
	int          m_mapBatchOk, m_mapBatchFail;
	CStringArray m_mapBatchZones;
	CString      m_mapBatchLog;
    //ToolbarFunctions
	void CloseAllToolDialogs();
	void OpenToolBar(CDialogBar *barLoc);

	//tree helper functions
	HTREEITEM AddTreeRoot(CString name,UINT id,CDialogBar *barLoc);
	HTREEITEM AddTreeChild(CString name,UINT id,CDialogBar *barLoc,HTREEITEM treeItem);
	CString GetTreeItemText(UINT id,CDialogBar *barLoc,HTREEITEM treeItem,int maxLen);
	int GetTreeChildIndex(HWND treeHwnd,HTREEITEM item) ;
	int GetTreeSelectionLevel(UINT id,CDialogBar *barLoc);
	BOOL SetTreeItemText(CString newText,UINT id,CDialogBar *barLoc,HTREEITEM treeItem,int maxLen);
    //control functions
    //list functions
	void DeleteObjectByTrace(int trace);
	void ObjectTreeViewInlist(int groupNum,int traceNum);//belongs to base objects
	void VisualPropertiesInList();
	void ObjectsInListAll();
	void WorldGroupInList(int sel);

	//entity functions
	void EnitityInList(); 
	BOOL GetIntoEntity(CMovementObj * currentEntity,CMovementObj * targetEntity);
	BOOL GetIntoEntityByIndexes(int indexSrc,int indexDest);

	//control functions
	void UpdateControlList();
	BOOL ForceFeedbackInit();
	void StopAllForceEffects();

	//collision functions
	void SphereCollideSlide(CFrameObj* frame2,CFrameObj* frame,float speed,float radiusesCombined);
	D3DVECTOR UpdateTransD3dvector(D3DVECTOR v1,D3DVECTOR v2);
    BOOL LoadCollisionDataFromImMesh(CEXMeshObj* imMeshPtr,CPolyObj * collisionData);
	void UpdateEntitiesLastPosition();
	void FindOriginAndRadius(D3DVECTOR * origin,
									 float * spereRadius,
									 CPolyObj polyArray[],
									 int polyCount,
									 CFrameObj * mainFrame);
	void CollisionAdjust(CMovementObj * entLoc,int howManyNormals,float storedDistance[20],D3DVECTOR storedNormals[20]);
	//camera functions
	BOOL DisableCamera(int cameraIndex);
	void FloatingCameraCallback();
	void ZoomCameraCallback();
	//2d
	void TearDownBackgroundInterface();
	void TextFontInterfaceInlist();
	BOOL SaveTextFontInterfaceFromFile(CString fileName);
	void DeActivateAllTextDisplays();
	BOOL ActivateTextDisplay(int collectionIndex);
	BOOL DeActivateTextDisplay(int collectionIndex);
	BOOL AddTextEntry(int collectionIndex,CString text,int percX,int percY);
	void CreateTextCollection(CString CollectionName,int timeOn,int fontSelection,COLORREF color);
    void Tree2DinterfaceInList();
	//cleanup
	void DesroyMouseList();
	void DestroyInventorylist(CInventoryObjList * listFocus);
	
	//environment
	void EnvironmentUpdate(CFrameObj* frame);
    //material
	//mesh stuff


	//font
	void AddFont(int sizeX,int sizeY);
	void InitAllFonts();

	//explosions

	//persisting info
	BOOL GeneratePersistInfo(CPersistObjList ** pList);
	BOOL DestroyPersistInfo(CPersistObjList ** pList);
	//movie
	BOOL AddMovieRuntimeObject(CString fileName,RECT displayRect,BOOL looping);
  
	//controls
	BOOL EntityFireNow(CMovementObj* ptrLocal);
	void AddControlConfig(CString configName,int controlBasis);
	void AddControllerEvent(ControlDBObj* controller,
									int controlType,
									int keyLoc,
									int eventAction,
									CString description,
									float miscFloat1,
									float miscFloat2,
									float miscFloat3);
    void JoystickTurnLeft(CMovementCaps *movDB,float Joy1RotY);
    void JoystickTurnRight(CMovementCaps *movDB,float Joy1RotY);
	void JoystickPullUp(CMovementCaps *movDB,float Joy1RotX);
    void JoystickDive(CMovementCaps *movDB,float Joy1RotX);
	void DecelPitchMouse(CMovementCaps *movDB);
	void DecelTurnMouse(CMovementCaps *movDB);
	void InListControlEvents();
    void DecelSpeedManditory(CMovementCaps *movDB);
	BOOL GetKeyIfPressed(CString * key,BYTE diKeys[256]);


	
    

	//Atrificail Intelligence
	/*
	void AI_DBInList(int sel);
	void AddAIObject(        float     closeEnoughDist,
		                     float     turnTolerance,
		                     int       sightType,
		                     int       sightRandVariable,
		                     float     maxSightRadius,
		                     int       difficulty,
		                     BOOL      walkingType,
		                     int       startAttackZone,
		                     int       startAttackDist,
                             int       randChillTime,
		                     int       pathfindingMethod,
		                     int       randKamakazi);

    BOOL AI_Roam(CMovementObj * ptrLoc,CAIObj * AIPtr);
	BOOL AI_UseWeapon(CMovementObj * ptrLoc,CAIObj * AIPtr);
	*/
	//popout
	void Popout_Entity(CViewportObj* mptLoc);
    //missile stuff
    //BOOL InitDBMissileObject(CMissileObj* mislPtr);
	//void MissileDatabaseInList(int sel);
	//error posting
	void ReportError(CString errorMessageLocal);
	void CloseErrorMessage();
	//texture swap
    BOOL AddTextureSwapCollection(CString collectionName,
										  int totalImages,
										  BOOL alphaTexture,
										  BOOL decalTex,
										  int alphaMapSize,
										  float decalRed,
										  float decalGreen,
										  float decalBlue,
										  int delay,
										  int alphaChannel);
   
	//SCRIPTING
	void AddScriptEvent(int attribute,CString miscString,int miscInt,CEXScriptObjList * scriptList);
	BOOL SaveScriptFile(CString fileName,CEXScriptObjList * scriptList);
	int GetScriptEventNum(CString txtDesc); 
	CString GetScriptEventDesc(int num);
	void ScriptInList(CEXScriptObjList * ptrLoc);
	//spawn
	BOOL GetSpawnPointMatrix(int selectMethod,D3DMATRIX *matrixLocal);
	BOOL Respawn(CMovementObj * currentEntity);

	//PORT matrix
	void GetFloatMatrix(D3DMATRIX mat,float Mat[4][4]);
	void GetD3DMatrix(D3DMATRIX * mat,float Mat[4][4]);
	void InvFloatMatrix(float Mat[4][4]);
    inline DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }
	//void UtilizeViewPort(CViewportObj* vptPtr);
	//void InListTextureDB(int sel);
	//void DestroyAllTexturesFromDatabase();
	void SearchAndAddTextureDataWorldObjs();
	BOOL SetCursorToSelectedObject(int trace);
	//CONVERSION
	void ConvertToX(CString * fileString);

    //PHYSICS
	void AccelMove(        D3DVECTOR *NewPos, 
						   D3DVECTOR *NewVel,
                           D3DVECTOR CurPos, 
						   D3DVECTOR CurVel,
                           D3DVECTOR CurAccel, 
						   double TmSlice);


	//HUD
	void InListHud();
	void InListDisplayUnits();
    
    //A
	void ScaleAll();

    //Particle
    void ParticleSystem(int bLoopLoc,D3DVECTOR camLocation,D3DMATRIX matrixLoc, float fov);
	void ParticleDatabaseInList(int sel);
	void AddParticleSystem
		(
		CString    fileName,
		float      meshScale,
		int        lifeTime,
		BOOL       sortOn,
		float      startRed,
		float      startGreen,
		float      startBlue,
		float      radius,
		BOOL       sphereType,
		float      startHieght,
		int      fadeStart,
		int      fadeEnd,
		int        scaleStart,
		int        scaleEnd,
		BOOL       endSpawn,
		int        texture,
		float      initialSpeedX,
		float      initialSpeedY,
		float      initialSpeedZ,
		float      directpartSpeed,
		float      directpartSpread,
		float      bspd,
		float      bspddr,
		float      bsprd,
		float      bsprddr,
		BOOL       isDirectional,
		BOOL       fadeTime,
		BOOL       Regenerate,
		BOOL	   VaryLifetime,
		int        howManyParticles,
		BOOL       billBoardOn,
		EnvParticleSystem ** retPtr
		);

	//Halo
	void UpdateHaloListbox();

    //Spawn Generator
    void SpawnGeneratorCallback();
	//void SpawnGenDatabaseInList(int sel);

	//normals modifier
    void WorldNormalModifier(int process, int group);

	//upgradeing tools
	void UpgradeFileFormats();
	void WorldObjectProperties();

	//bounding box UI
	void DrawBox(LPDIRECT3DDEVICE9 pd3dDevice, 
		         D3DVECTOR max, 
				 D3DVECTOR min,
				 D3DVECTOR offset);
    void DrawSimpleBox(LPDIRECT3DDEVICE9 pd3dDevice, 
					   D3DVECTOR max, 
					   D3DVECTOR min,
					   D3DVECTOR offset);
    void DrawSegment(LPDIRECT3DDEVICE9 pd3dDevice, 
						     D3DVECTOR max, 
						     D3DVECTOR min,
						     D3DVECTOR offset);

    //texture State Changes Optimizations
	void ReorderObjectsBasedOnTextureIndex();
    //
	void DumpTempDatabases();

	//texture
	//BOOL SaveTextureBank(CString fileName,int compressed,int maxTextureSize);
	void TextureRebuildWithSearch(int mipLevels);


	//lightmap load
	void LightmapSearchAndload();



	//path 
	void AddTextureFromSearchPath(CString path,CEXMeshObj * mesh);
	int BonesSystemRenderSkeleton(CSkeletonObject * sklPtr,
		                          D3DVECTOR offsetVector,
								  D3DMATRIX positionMatrix);

    //renderloop assist
    BOOL         LoadWorldObjectFromData(ABVERTEX * vArray,
										 WORD * iArray,
										 WORD vCount,
										 WORD iCount,
										 CString texture1,
										 CString texture2,
										 CString texture3,
										 CString texture4,
										 CString texture5,
										 CString texture6,
										 CString texture7,
										 CString texture8,
										 CString objectName,
										 float sightRange,
										 BOOL smoothShading);

    //collision
	//inline void CreateFrustumPlanes( CViewportObj* ViewPort, D3DVECTOR *refPt, D3DVALUE *frustPlanes);
	D3DVECTOR TransformPt(D3DMATRIX transform, D3DVECTOR pt);
	BOOL ArmEntitySys(CMovementObj * movObj,
							  int itemIndex,
					          CMovementObjList * referenceDatabase,
							  BOOL overrideNetwork);
	BOOL DisArmEntitySys(CMovementObj * movObj,int itemIndex,BOOL networkOverride);
	void SkeletalTextureAdd(CSkeletonObject * skeletonObj);
	void InListEntityAppendages();
	//bounding
	void BoundingBoxesRender(D3DVECTOR offset);
	//texture UI extention
	void RenderTextureIndex(int index,CViewportObj* vptPtr);
   
	//font renderer
	void InListCGFonts(int sel);
	/*
	void ScriptInList(int sel);
	void ScriptEventsInList(int sel);
	void AITreeInList(int sel);
	void AI_HandleScript(CMovementObj * ptrLoc);
	*/
	void RenderFeelers(CMovementObj * ptrLoc,D3DVECTOR offset);
	BOOL EnableServer();
	BOOL SendEquipMsg(int index,
				      long refrenceID,
					  CPlayerObject * playerObjectData,
					  int catagorie);
	//void InListExplosionDB(int sel);
	void SoundTrackInList(int sel);
    //void MenuInList(int sel);
	//void MenuVisualsInList(int sel);
	void InListClickZones(int sel);
	void InListRuntimeEntities(int sel);
    void UpdateInfoEverySecond();
	void RenderLoopMessageHandling();
	void HandleFPSTimeStamp();
	BOOL GetScreenPos(int *ScrnPosHorz, 
					  int *ScrnPosVert, 
					  int HieghtRes,
                      int WidthRes, 
					  double ClpPlaneLeft, 
					  double ClpPlaneBottom,
                      double ClpPlaneRight, 
					  double ClpPlaneTop,
                      double ClpPlaneDist, 
					  D3DVECTOR My3DPnt,
					  float Aspect);
	BOOL GetPixelSnap(D3DMATRIX camMatrix,
							   float fovLoc,
							   D3DVECTOR targetWldPosition,
							   DWORD * pixelValue,
							   DWORD surfaceBBP,
							   BYTE* pBytes,
							   float vPortHAspect,
							   DWORD pitch
							   );
	BOOL EvaluateAndRenderBaseCharonas(CViewportObj* mptLoc,D3DVECTOR locationVect);
	BOOL EvaluateAndRenderCharonaOverlays(CViewportObj* mptLoc,D3DVECTOR locationVect);
	void StampLog(long stampTest2);
	BOOL SphereInFovStatus(double radius,
						   double fov,
						   D3DMATRIX cameraMatrix,
						   D3DVECTOR wldPosition);
    inline BOOL IsMeshInView(CEXMeshObj * mesh,
						     D3DMATRIX & camGenMatrix);
	void      UpdateMonitor();
	void      InListAccounts(int sel);
	BOOL      TargetNotVisible(D3DVECTOR eyePoint,D3DVECTOR target);
	void      SetFadeOnSkeletal(CSkeletonObject * skeleton,long duration);
    BOOL      BrushObjectCopy(D3DMATRIX newPosition,
		                      D3DVECTOR rotation,
		                      D3DVECTOR offset,
							  D3DVECTOR scale);
	BNDBOX    BoundingBoxGroupSet(     int group,
									   float maxBumpX,
									   float maxBumpY,
									   float maxBumpZ,
									   float minBumpX,
									   float minBumpY,
									   float minBumpZ);

	BOOL BrushObjectCopyLowLevel(CWldObject * ptrLoc,
										 D3DMATRIX newPosition,
								         D3DVECTOR rotation,
								         D3DVECTOR offset,
								         D3DVECTOR scale,
										 CWldObject ** newPtrRef,
										 D3DVECTOR meshCenter);
	void InListEntityDefcfgs(int sel);
	void UnionVertexesPositionalyByTolerance(float tolerance,int group);
	BOOL RenderTranslucentMirroredPolys(D3DVECTOR locationVect,BOOL mirrorEnable);
	void AddNodeByData(int groupIndex,int selection,CMovementObj * entityPtr,D3DMATRIX * overrideMatrix);
	void LoadSoundManager(CString fileName);
	inline CString TranslateCharValue(int value);
	inline BOOL SendGeneralMessageHandler(int msgType,CPlayerObject * playerObj);
	void GetEnviormentFilter();
	void LoadPatchDatabase(CString fileName);
	void ConvertExt(CString & currentFile,CString newExt);
	BOOL HandleDamageMsgServerSide(LPVOID lpMessage,int & msgSize,DPID & idFromLoc,int & curSimutanius);
	inline void ExplosionCallback();
	void InListMisChannels(int sel);
    void OutPutInfoFile(CString filename);
	void RemoveDuplicatesThatExistFromFile();
	//void InListMissileSkillCriteria(int sel);
	BOOL LoadStatsDatabase(CString fileName);
	//void InListCollisionDB(int sel);
	void FormatAllNames();
	void SnapShotRenderSection();
	void HandleGeneralMsgHandler(LPMSGHANDLER formatedMessage);
	void HandleDmgMsgHandler(LPDAMAGEINFO_DATA formatedMessage);
	void HandleDetailedMsgHandler(LPDETAILEDMSG_DATA formatedMessage);
	inline void OverideMissile(CMovementObj     * entityPtr,
									   float            & pitchRotation,
									   D3DMATRIX        & positionalMatrix,
									   int              & entityTraceNumber,
									   BOOL               sendOverNetwork,
									   float            & yawRotation,
									   int                missileType);
	DWORD SqlItemUpdateToServer();
	void CheckAccountDBSyncronization();
	int SetJet(CMovementObj * ptrLoc,float forceY,float forceZ);
	BOOL IfItemForceJetSendIt(CItemObj * item,CPlayerObject * playerToReceive);
	void ComputeLight(D3DXVECTOR3 &lightDir, D3DXMATRIXA16 *pObject, D3DXMATRIXA16 *pView, D3DXMATRIXA16 *pProj);
	BOOL DeleteAccountsBasedOnFile();
	BOOL AtribDestroyItemByGlid(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	inline DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }
	void MigradeEDBToGLAppendages();
	BOOL HandleQuestSys(CPlayerObject * currentPlayer,CAICollectionQuestObj * questPtr,CServerItemObj * svrPtr,DPNID idFromLoc,int & showSelectionCurrent);
    void RenderStencilShadows2(D3DVECTOR locationVect, D3DMATRIX camMatrix);
	void GetShadowPolyMesh( CCollisionObj *collisionObject, 
									  ABVERTEX1LNT * buffer, 
									  int & returnVCount,
									  D3DXVECTOR3 objectPos,
									  float radius,
									  float extentUp,
									  float extentDown);
	BOOL DuplicateTri(CPolyRetStruct *Polys, int cPolys, CPolyRetStruct TestPoly);
	void ManageEnvEffects(CWldObject * wldObj,CEXMeshObj * mObj);
	void SpawnGenDataSQLOutput();
	//void AIdimIntegrityCheck();
	void RenderStencilShadows3(D3DVECTOR locationVect, D3DMATRIX camMatrix);
	void LoadRNEFile();
	BOOL ReplaceObjectByName(CString objectNameBasis,CString path);
	void MaintnenceOnAccounts(BOOL reclaimMemory);
	void InitFXSahdersOnSkeletal(CSkeletonObject * skeletal);
	BOOL SendMovMsg(    DWORD flags,
						DPNID sendTo,
						D3DVECTOR curPosition,
						long refrenceID,
						D3DVECTOR dirVect, //here are the dir and up vectors
						D3DVECTOR upVect,
						float FreeLookYaw,
						float FreeLookPitch,
						short dbIndex,
						short curAnim,
						short secondaryAnim,
						long currenttimeEL) ;
	void RenderCGMenuModCallback(long currentTime);
	void RenderClickZone(CMenu3DObj* menu,int index);
	//void RefreshEnviorment();

	//New Missile Channel
	//void InListElementStatus(int sel);

	//New Menu System Utility Functions
	void RefreshMenuThemesList(int selection);
	void RefreshMenuList(int selection);
	void RefreshMenuVisualsList(int selection);
	void LoadCursorTextures();
	void LoadFontTextures();
	void LoadIconTextures();
	//void TestMenu(CMenuWindow *pMenu);

	//{{AFX_MSG(EngineXWin)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	afx_msg void OnButtonQuit();
	afx_msg void OnButtonSave();
	afx_msg void OnButtonMenu();
	afx_msg void OnButtonTreetest();
	afx_msg void OnButtonObjectload();
	afx_msg void OnButtonObjectdelete();
	afx_msg void OnButtonSwitchObject();
	afx_msg void OnBUTTONBKEDITORswtch();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonMvUp();
	afx_msg void OnButtonMvDown();
	afx_msg void OnButtonMvLeft();
	afx_msg void OnButtonMvRight();
	afx_msg void OnButtonAddnewstaticBk();
	afx_msg void OnBUTTONBKSETTINGSapply();
	afx_msg void OnButtonControls();
	afx_msg void OnButtonTestEffect();
	afx_msg void OnButtonStopEffect();
	afx_msg void OnButtonMovies();
	afx_msg void OnButtonTestmovie();
	afx_msg void OnBtnEntityCreatenew();
	afx_msg void OnBtnEntityActivate();
	afx_msg void OnButtonSwEntities();
	afx_msg void OnButtonTestfullscreen();
	afx_msg void OnBtnViewwireframe();
	afx_msg void OnButtonViewsmoothshaded();
	afx_msg void OnBtnEntityMovement();
	afx_msg void OnBtnEntityDeactivate();
	afx_msg void OnBtnDeleteEntity();
	afx_msg void OnBTNENTITYdeleteAnim();
	afx_msg void OnBTNENTITYDeleteChld();
	afx_msg void OnBtnEntityAddchild();
	afx_msg void OnBtnNewall();
	afx_msg void OnBTNCameraViewportsSw();
	afx_msg void OnCAMERACreateNewCamera();
	afx_msg void OnBTNCreateNewViewport();
	afx_msg void OnBTNCameraVportActivate();
	afx_msg void OnBTNvportDeactivate();
	afx_msg void OnBTNMOUSEcreate();
	afx_msg void OnBTNUImouseSW();
	afx_msg void OnBTNMOUSEfileBrowse();
	afx_msg void OnBTNMOUSEactivate();
	afx_msg void OnBTNMOUSEdelete();
	afx_msg void OnBTNMOUSEdeactivate();
	afx_msg void OnBTN2DINTERFACEsw();
	afx_msg void OnBTN2DCreateCollection();
	afx_msg void OnBTN2DADDunit();
	afx_msg void OnBtn2dActivate();
	afx_msg void OnBtn2dDeactivate();
	afx_msg void OnBTN2DBrowseFileUp();
	afx_msg void OnBTN2DBrowseFileDown();
	afx_msg void OnBTN2DgetSelectedInfo();
	afx_msg void OnBTN2DApplyModifications();
	afx_msg void OnBTN2DDeleteCollection();
	afx_msg void OnBTN2DDeleteUnit();
	afx_msg void OnBUTTONEnviormentSw();
	/*
	afx_msg void OnBTNENVBckRgb();
	afx_msg void OnBTNENVFogRgb();
	afx_msg void OnBTNENVApplyChanges();
	afx_msg void OnBTNENVMatch();
	afx_msg void OnCHECKENVFogOn();
	afx_msg void OnBTNENVBrowseSurObj();
	afx_msg void OnCHECKEnableSurrounding();
	afx_msg void OnBTNENVemmisive();
	afx_msg void OnCHECKENVenableSun();
	afx_msg void OnBTNENVsunColor();
	*/
	afx_msg void OnBTNObjectMaterial();
	afx_msg void OnBTNEntityMaterial();
	afx_msg void OnBUTTONtextAddTextCollection();
	afx_msg void OnBTNtextFontSW();
	afx_msg void OnBUTTONtextAddTextObject();
	afx_msg void OnBTNtextActivate();
	afx_msg void OnBTNtextDeactivate();
	afx_msg void OnBUTTONtextTextColor();
	afx_msg void OnBTNtextApplyChanges();
	afx_msg void OnBTNtextGetCurrentInfo();
	afx_msg void OnBTNtextApplyFont();
	afx_msg void OnBUTTONtextAddFont();
	afx_msg void OnBTNtextApplySizeChange();
	afx_msg void OnBUTTONtextDeleteFont();
	afx_msg void OnBUTTONtextDeleteTextCollection();
	afx_msg void OnBUTTONtextDeleteTextObject();
	afx_msg void OnBTNtestSave();
	afx_msg void OnBTNtestLoad();
	afx_msg void OnBTNcamVportSave();
	afx_msg void OnBTNcamVportLoad();
	afx_msg void OnBTNDeleteCamera();
	afx_msg void OnBTNDeleteViewport();
	afx_msg void OnBTN2DSaveConfig();
	afx_msg void OnBTN2DLoadConfig();
	afx_msg void OnBTNExplosionDBSW();
	afx_msg void OnBTNEXPAddExplosion();
	afx_msg void OnBTNEXPLoadExplosions();
	afx_msg void OnBTNEXPSaveExplosions();
	afx_msg void OnBTNEXPDeleteExplosion();
	afx_msg void OnBTNCameraAdvanced();
	afx_msg void OnBTNMovieAddMovie();
	afx_msg void OnBTNMovieTestMovie();
	afx_msg void OnBTNMovieEditMovie();
	afx_msg void OnBTNMovieDeleteMovie();
	afx_msg void OnBTNMoviesSaveDB();
	afx_msg void OnBTNMoviesLoadDB();
	afx_msg void OnBTNMovieStopMovies();
	afx_msg void OnBTNControlAddObject();
	afx_msg void OnBTNControlAddEvent();
	afx_msg void OnBTNControlCfgSave();
	afx_msg void OnBTNControlCfgLoad();
	afx_msg void OnBTNControlDeleteObject();
	afx_msg void OnBTNControlDeleteEvent();
	afx_msg void OnSelchangeLISTControlDB();
	afx_msg void OnSelchangeLISTAItree();
	afx_msg void OnBTNControlEditObject();
	afx_msg void OnBTNControlEditEvent();
	afx_msg void OnBTNUIBaseAdvanced();
	afx_msg void OnBTNObjectArrayAdd();
	afx_msg void OnBTNENTITYProperties();
	afx_msg void OnBTNOBJECTProperties();
	afx_msg void OnBTNOBJDeleteLight();
	afx_msg void OnBtnObjAddlight();
	afx_msg void OnBTNENTDestroyLight();
	afx_msg void OnBTNENTAddLight();
	afx_msg void OnBTNUIMusic();
	afx_msg void OnBTNMusicAssignTrack();
	afx_msg void OnBTNMusicAssignWav();
	afx_msg void OnBTNMusicUnassignTrack();
	afx_msg void OnBTNMusicUnassignWav();
	afx_msg void OnBTNMusicBrowseWav();
	afx_msg void OnBTNUITextureAssign();
	afx_msg void OnBTNUIMissileDatabase();
	/*
	afx_msg void OnBTNMISAddMissile();
	afx_msg void OnBTNMISBrowseFileName();
	afx_msg void OnBTNMISDeleteMissile();
	afx_msg void OnBTNMISsave();
	afx_msg void OnBTNMISLoad();
	*/
	afx_msg void OnBTNUIBulletHole();
	afx_msg void OnBTNBHAddHole();
	afx_msg void OnBTNBHBrowse();
	afx_msg void OnBTNBHAlphaTexture();
	afx_msg void OnBTNBHDeleteHole();
	afx_msg void OnBTNBHload();
	afx_msg void OnBTNBHMaterial();
	afx_msg void OnBTNBHSave();
	afx_msg void OnBTNEntityshadow();
	afx_msg void OnBTNObjectShadow();
	afx_msg void OnBTNENTAnimProperties();
	afx_msg void OnBTNEntitySound();
	afx_msg void OnBTNEntitysetSoundCenter();
	afx_msg void OnBTNOBJSound();
	afx_msg void OnBTNOBJAlphaTexture();
	afx_msg void OnBTNENTAnimSound();
	afx_msg void OnBTNTCPIPStartHost();
	afx_msg void OnBTNTCPIPStopHost();
	afx_msg void OnBTNTCPIPJoinHost();
	afx_msg void OnBTNUIMultiplayer();
	afx_msg void OnBTNEntityCombineEntityVis();
	afx_msg void OnBTNENTContentProperties();
	afx_msg void OnBTNTEXAddCollection();
	afx_msg void OnBTNTEXDeleteCollection();
	afx_msg void OnBTNUIAnimTextureDB();
	afx_msg void OnBTNTEXLoadCollection();
	afx_msg void OnBTNTEXSaveCollection();
	afx_msg void OnBTNENTLoadDB();
	afx_msg void OnBTNENTSaveDB();
	afx_msg void OnBtnUiScript();
	afx_msg void OnBTNSCRAddScript();
	afx_msg void OnBTNSCRDeleteEvent();
	afx_msg void OnBTNSCRLoadExecFile();
	afx_msg void OnBTNSCRBuild();
	afx_msg void OnBTNSCREditEvent();
	afx_msg void OnBTNOBJEditLight();
	afx_msg void OnBTNENTLoadEntity();
	afx_msg void OnBTNENTSaveEntity();
	afx_msg void OnBTNMSSaveDB();
	afx_msg void OnBTNMSLoadDB();
	/*
	afx_msg void OnBtnTxtdbAdd();
	afx_msg void OnBtnTxtdbDelete();
	afx_msg void OnBtnTxtdbEdit();
	afx_msg void OnBtnTxtdbReinitAll();
	afx_msg void OnBTNTXTDBBrowse();
	afx_msg void OnBTNTXTDBTextureReport();
	afx_msg void OnSelchangeLISTTXTDBDatabase();
	afx_msg void OnBtnTxtdbAddUrl();
	*/
	afx_msg void OnBTNOBJClearTexCoords();
	afx_msg void OnBTNOBJAddTexCoords();
	afx_msg void OnBTNSCRSetToScene();
	afx_msg void OnBTNSCRClearScene();
	afx_msg void OnBTNSCRLoadFromScene();
	afx_msg void OnBTNNTAdvanced();
	afx_msg void OnBTNNTBuildTxtRecord();
	afx_msg void OnBTNUIHudInterface();
	afx_msg void OnBTNHUDAddDisplayUnit();
	afx_msg void OnBTNHUDAddNewHud();
	afx_msg void OnBTNHUDSaveDB();
	afx_msg void OnBTNHUDLoadDB();
	afx_msg void OnSelchangeListHudDb();
	afx_msg void OnBTNHUDDeleteHud();
	afx_msg void OnBTNHUDDeleteDisplayUnit();
	afx_msg void OnBTNHUDReplaceUnit();
	afx_msg void OnBTNUIMenuSystem();
	//afx_msg void OnBTNMENUAddMenu();
	//afx_msg void OnBTNMENUAddUnit();
	//afx_msg void OnBTNMENUSave();
	//afx_msg void OnBTNMENULoad();
	//afx_msg void OnBTNMENUEditMenu();
	//afx_msg void OnBTNMENUEditDisplayUnit();
	//afx_msg void OnBTNMENUDeleteMenu();
	//afx_msg void OnBTNMENUDeleteUnit();
	afx_msg void OnBTNENTUpdate();
	afx_msg void OnCHECKTEXBltOn();
	afx_msg void OnBTNMenuClone();
	afx_msg void OnBTNMenuMaterial3D();
	afx_msg void OnBTNMenuAddAnimation();
	afx_msg void OnSelchangeLISTMenuAnimations();
	afx_msg void OnSelchangeLISTMENUUnitList();
	afx_msg void OnBTNMenuproperties();
	/*
	afx_msg void OnBTNENVFogDay();
	afx_msg void OnCHECKENVAmbientLightOn();
	afx_msg void OnBTNENVambientDayLight();
	afx_msg void OnCHECKENVTimeSystem();
	afx_msg void OnCHECKENVWindRandomizer();
	*/
	afx_msg void OnBTNUIParticleSys();
	afx_msg void OnBTNPARTAddParticle();
	afx_msg void OnBTNPARTEditSystem();
	afx_msg void OnBTNPARTDeleteSystem();
	afx_msg void OnBTNPARTLoadDatabase();
	afx_msg void OnBTNPARTSaveDatabase();
	afx_msg void OnBTNPARTTestSystem();
	afx_msg void OnBTNPARTStopTest();
	afx_msg void OnBTNUISpawnGen();
	/*
	afx_msg void OnBTNSpawnAddGen();
	afx_msg void OnBTNSpawnDeleteGen();
	afx_msg void OnBTNSpawnEditGen();
	*/
	afx_msg void OnBTNObjectBuildCollisionMap();
	afx_msg void OnClickObjectTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangedObjectTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBTNObjectsAddGroup();
	afx_msg void OnBTNObjectsDeleteGroup();
	afx_msg void OnBTNObjectsEditGroup();
	afx_msg void OnSelchangeLISTObjectsGroupList2Box();
	afx_msg void OnDblclkLISTObjectsGroupList2Box();
	afx_msg void OnDblclkObjectTree(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnBTNUIMerge();
	afx_msg void OnBTNTCPDisconnectPlayer();
	afx_msg void OnBTNTCPServerLoad();
	afx_msg void OnBTNTCPServerSave();
	afx_msg void OnBTNOptimizeStates();
	afx_msg void OnBTNTCPNewAccount();
	afx_msg void OnBTNTCPDeleteAccount();
	afx_msg void OnBTNOBJAddTrigger();
	afx_msg void OnBTNOBJUpdateObject();
	afx_msg void OnCHECKTCPRegistered();
	afx_msg void OnSelchangeLISTTCPIPCurrentPlayers();
	/*
	afx_msg void OnBTNTXSaveTextureBank();
	afx_msg void OnBTNTXLoadTextureBank();
	*/
	afx_msg void OnBTNMNUAnimProperties();
	afx_msg void OnBTNMNUDeleteAnimation();
	afx_msg void OnBTNOBJGetSignificant();
	afx_msg void OnBTNOBJTextureSetMacro();
	afx_msg void OnBtnUiAi();
	/*
	afx_msg void OnBTNAICreateNew();
	afx_msg void OnBTNAIDelete();
	*/
	afx_msg void OnBtnObjLodadd();
	afx_msg void OnBTNOBJRemoveLODLevel();
	afx_msg void OnBTNOBJEditLOD();
	afx_msg void OnBTNENTITYAnimations();
	afx_msg void OnBTNENTAppendCustomDB();
	afx_msg void OnSelchangeLISTENTITYRuntimeList();
	afx_msg void OnDblclkLISTENTITYRuntimeList();
	afx_msg void OnBTNENTAppendArm();
	afx_msg void OnBTNENTAppendDisarm();
	afx_msg void OnBTNUIHaloEffects();
	afx_msg void OnBTNAddHalo();
	afx_msg void OnBTNDeleteHalo();
	afx_msg void OnBTNEditHalo();
	afx_msg void OnBTNHaloLoad();
	afx_msg void OnBTNHaloSave();
	afx_msg void OnBTNAddLensFlare();
	afx_msg void OnBTNEditLensFlare();
	afx_msg void OnBTNDeleteLensFlare();
	afx_msg void OnBTNGenerateMesh();
	afx_msg void OnBTNENTLODSystems();
	afx_msg void OnBTNENTBaseSkills();
	afx_msg void OnBTNENTResourceBase();
	afx_msg void OnBTNENTViewRuntimeStats();
	afx_msg void OnBTNENTViewRuntimeResources();
	/*
	afx_msg void OnSelchangeLISTMISmissileDataBase();
	afx_msg void OnBTNMISReplaceValues();
	afx_msg void OnBTNMISEditVisual();
	afx_msg void OnBTNMISAddBillboardMissile();
	afx_msg void OnBTNMISEditDynamics();
	*/
	afx_msg void OnBTNENTEditStartInven();
	afx_msg void OnDropdownCMBOCGTEXFontTableTexture();
	afx_msg void OnBTNCGTEXAdd();
	afx_msg void OnBTNCGTEXDelete();
	/*
	afx_msg void OnBTNAIAddEvent();
	afx_msg void OnBTNAIAddScript();
	afx_msg void OnBTNAIEditScript();
	afx_msg void OnBTNAIDeleteScript();
	afx_msg void OnBTNAIEditEvent();
	afx_msg void OnBTNAIDeleteEvent();
	afx_msg void OnBTNAIAddTree();
	afx_msg void OnBTNAIDeleteTree();
	afx_msg void OnBTNAIEdit();
	afx_msg void OnBTNAISaveAiData();
	afx_msg void OnBTNAILoadAiData();
	afx_msg void OnSelchangeLISTAIScripts();
	*/
	afx_msg void OnBTNENTSaveAppendages();
	afx_msg void OnBTNENTLoadAppendages();
	afx_msg void OnBTNENTAddAppendages();
	afx_msg void OnBtnEntMount();
	afx_msg void OnBtnEntDemount();
	afx_msg void OnBTNEXPEditExplosionObj();
	//afx_msg void OnBTNMISSound();
	afx_msg void OnBTNMUSICAddTrack();
	afx_msg void OnBTNMUSICDeleteTrack();
	afx_msg void OnBTNMUSICLoadDB();
	afx_msg void OnBTNMUSICSaveDB();
	afx_msg void OnBTNMUSICTestTrack();
	afx_msg void OnBTNMUSICStopSoundTrack();
	afx_msg void OnBTNMenuCursor();
	afx_msg void OnBTNMenuTest();
	afx_msg void OnBTNMenuCloseAllMenus();
	afx_msg void OnSelchangeLISTMENUMainDB();
	afx_msg void OnBTNMenucursorScale();
	afx_msg void OnBTNMenucursorDelete();
	afx_msg void OnBTNMNUAddClickZone();
	afx_msg void OnBTNMNUeditClickZone();
	afx_msg void OnBTNMNUGetCursor();
	afx_msg void OnBTNTXPurgeTextures();
	afx_msg void OnChangeEDITTCPIPadress();
	afx_msg void OnChangeTCPuserName();
	afx_msg void OnChangeTCPpassword();
	afx_msg void OnBTNSystemGoingDown();
	afx_msg void OnBTNENTAddItem();
	afx_msg void OnBTNENTItemTest();
	afx_msg void OnBTNENTDeleteItem();
	afx_msg void OnBTNENTDisarmItem();
	afx_msg void OnBTNENTEditItem();
	afx_msg void OnBTNOBJClone();
	afx_msg void OnBTNOBJTransformation();
	afx_msg void OnBTNOBJBoundingGroupSet();
	afx_msg void OnBTNOBJLibraryAccess();
	//afx_msg void OnBTNENVTimeSystem();
	/*
	afx_msg void OnBTNMISAddSkeletalMissile();
	afx_msg void OnBTNMISAddLaser();
	*/
	afx_msg void OnBTNENTdefSwapNext();
	afx_msg void OnBTNENTdimPrev();
	//afx_msg void OnBTNAIEditAITree();
	afx_msg void OnBTNTCPSupportedWorlds();
	afx_msg void OnBTNENTEditDeathCfg();
	afx_msg void OnBTNTCPViewAccount();
	afx_msg void OnCHECKMNUrenderClizeZoneInfo();
	afx_msg void OnBTNMNUDeleteClickZone();
	afx_msg void OnCHECKTXBBitmapLoadOverride();
	afx_msg void OnBTNUISkillBuilder();
	afx_msg void OnBTNOBJAddNode();
	afx_msg void OnBTNOBJReferenceLibrary();
	afx_msg void OnBTNUISoundDB();
	afx_msg void OnBTNUIRadarSettings();
	afx_msg void OnBTNUIGlInventoryDB();
	afx_msg void OnBtnUiCurrencysys();
	afx_msg void OnBTNUICommerceDB();
	afx_msg void OnBTNUIBankZones();
	afx_msg void OnBtnUiSafezones();
	afx_msg void OnBTNENTRefreshList();
	afx_msg void OnBTNENTUnifySleletalMesh();
	/*
	afx_msg void OnBTNMISDeleteLight();
	afx_msg void OnBTNMISLightSys();
	*/
	afx_msg void OnBtnUiStatdb();
	afx_msg void OnBTNTCPItemGeneratorDB();
	afx_msg void OnBtnPatchsys();
	afx_msg void OnBTNTCPUpdateCurrentOnlinePlayers();
	afx_msg void OnBTNENTScaleEntity();
	afx_msg void OnCHECKSPNEnableSystem();
	//afx_msg void OnBTNTXBSetMipLevels();
	afx_msg void OnBTNTCPUpdateAllAcounts();
	afx_msg void OnBTNENTReplaceEntityWSIE();
	afx_msg void OnBTNTCPIPBlocking();
	afx_msg void OnBtnUiWaterzones();
	afx_msg void OnBTNMNUSaveUnit();
	afx_msg void OnBTNMNUImportUnit();
	afx_msg void OnBtnUiPortalzonesdb();
	afx_msg void OnBTNUIElementDB();
	afx_msg void OnBTNUIDotDB();
	afx_msg void OnBTNTCPUpgradeWorm();
	afx_msg void OnBTNEXPSaveUnit();
	afx_msg void OnBTNEXPLoadUnit();
	afx_msg void OnBTNTCPDisconnectAll();
	afx_msg void OnBTNPRTSaveUnit();
	afx_msg void OnBTNPRTLoadUnit();
	afx_msg void OnBTNTCPPrintReports();
	//afx_msg void OnBTNTXBRemoveDuplicates();
	afx_msg void OnBTNMNUExportVisual();
	afx_msg void OnBTNMNUImportVisual();
	/*
	afx_msg void OnBTNAILoadAiUnit();
	afx_msg void OnBTNAISaveAiUnit();
	afx_msg void OnBTNAIImportCollisionSys();
	afx_msg void OnBTNAIDeleteSys();
	afx_msg void OnBTNAIExportCurrentCol();
	afx_msg void OnBTNAISaveColDB();
	afx_msg void OnBTNAILoadColDB();
	afx_msg void OnBTNAIIntegrityCheck();
	*/
	/*
	afx_msg void OnBTNMISSaveUnit();
	afx_msg void OnBTNMISLoadUnit();
	*/
	/*
	afx_msg void OnBTNSPNExportSys();
	afx_msg void OnBTNSPNImportSys();
	*/
	afx_msg void OnBTNTCPSearchForAccount();
	afx_msg void OnBTNTCPRestoreBackup();
	afx_msg void OnBTNTXTSymbolDB();
	afx_msg void OnCHECKEditorTxtMode();
	afx_msg void OnBTNENTMassFormatNames();
	afx_msg void OnCHKOBJEnablePlaementHotKey();
	afx_msg void OnBTNOBJReportCurRenderCache();
	afx_msg void OnBTNUIIconDBACCESS();
	afx_msg void OnBTNTCPSnapShotPacketQue();
	afx_msg void OnBTNTCPGenerateLogBasedOnTime();
	afx_msg void OnBTNUIHousingSys();
	afx_msg void OnBTNMNUTestIcon();
	afx_msg void OnBTNUIHszoneDB();
	afx_msg void OnFileRunScript();   // File > Run Startup Script (.aut)...
	afx_msg void OnFileLoadZone();    // File > Load Zone (.EXG)...
	afx_msg void OnBTNChkshowSpeed();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBTNTCPExportAccount();
	afx_msg void OnBTNTCPImportAccount();
	afx_msg void OnBTNTCPGenerateClanDatabase();
	afx_msg void OnBTNTCPClearInactive();
	afx_msg void OnBTNUIAIRainSys();
	/*
	afx_msg void OnBTNAInewTree();
	afx_msg void OnBTNAIExportToSql();
	*/
	afx_msg void OnBTNENTsaveItemDB();
	afx_msg void OnBTNENTLoadItemDB();
	afx_msg void OnBTNSPNAppendSys();
	afx_msg void OnButtonLoad();
	afx_msg void OnBTNTestAssign();
	afx_msg void OnBTNCommCntrlLibBtnA();
	afx_msg void OnBTNWorldRule();
	afx_msg void OnBTNArenaManager();
	//afx_msg void OnBtnUpdateAllTextures();
	afx_msg void OnCHECKWireMode();
	afx_msg void OnCHECKShadeMode();
	afx_msg void OnChangeEDITForceLodGlobal();
	afx_msg void OnBTNMainEnvironment(); 
	afx_msg void OnBTNMainCharacter(); 
	afx_msg void OnBTNMainMenus(); 
	afx_msg void OnBTNMainFX();
	//afx_msg void OnBTNAddMissileChannel();
	//afx_msg void OnBTNRemoveMissileChannel();

	//New Menu System Functions
	afx_msg void OnRefreshListThemes();
	afx_msg void OnButtonAddTheme();
	afx_msg void OnButtonEditTheme();
	afx_msg void OnButtonTestTheme();
	afx_msg void OnButtonApplyTheme();
	afx_msg void OnButtonExportTheme();
	afx_msg void OnButtonImportTheme();
	afx_msg void OnButtonDeleteTheme();
	afx_msg void OnRefreshListMenus();
	afx_msg void OnButtonAddMenu();
	afx_msg void OnButtonEditMenu();
	afx_msg void OnButtonMoveMenuUpList();
	afx_msg void OnButtonMoveMenuDownList();
	afx_msg void OnButtonTestMenu();
	afx_msg void OnButtonCloseAllMenus();
	afx_msg void OnButtonExportMenu();
	afx_msg void OnButtonImportMenu();
	afx_msg void OnButtonCloneMenu();
	afx_msg void OnButtonDeleteMenu();
	afx_msg void OnRefreshListMenuVisuals();
	afx_msg void OnButtonAddVisual();
	afx_msg void OnButtonEditVisual();
	afx_msg void OnButtonMoveVisualUpList();
	afx_msg void OnButtonMoveVisualDownList();
	afx_msg void OnButtonExportVisual();
	afx_msg void OnButtonImportVisual();
	afx_msg void OnButtonDeleteVisual();
	afx_msg void OnButtonCloneVisual();
	afx_msg void OnButtonIDCheck();
	afx_msg void OnButtonEditMenuSystemFonts();
	afx_msg void OnButtonEditMenuSystemCursor();
	afx_msg void OnButtonEditMenuIcons();
	afx_msg void OnButtonSaveMenuSystem();
	afx_msg void OnButtonLoadMenuSystem();
	afx_msg void OnButtonSaveKeyConfig();
	afx_msg void OnBTNEXPTest();
    afx_msg void OnButtonTestAsInstance();
	afx_msg void OnButtonCopyMenuWindow();
	afx_msg void OnButtonPasteMenuWindow();
	afx_msg void OnButtonCopyMenuVisual();
	afx_msg void OnButtonPasteMenuVisual();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:	
};
#endif
