#pragma once

#include <typeinfo>
#include <vector>
#include "clientnetworkinglogic.h"
#include "sceneassembly.h"
#include "globalstatusvariables.h"
//#include "usercontroller.h"

//GUI Elements
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

class CMovementObj;

// one point of interest from Maps\<zone>.poi  (T|x|z|name  T = Q quest npc, S store, B bank, P portal)
struct HudMapPoi
{
	char    type;
	float   x, z;
	float   y;       // height (used to tell floors apart), valid when hasY
	bool    hasY;
	CString name;
};

struct HudMapQuestRow
{
	float top, bottom;
	int   poi;     // index in m_zoneMapPois, -1 = not in this zone
};

class CMenuManagement
{
public:
	CMenuManagement(GlobalStatusVariables * gStatus,SceneAssembly * sceneAssembly,CClientNetworkingLogic * clientNet);//, UserController * userControls);
	~CMenuManagement(void);

    //Variables
	GlobalStatusVariables	*m_gStatus;
	SceneAssembly			*m_sceneAssembly;
	CClientNetworkingLogic	*m_clientNet;
    
	long m_clickHandlerStamp;
	long m_clickStamp;
	long m_groupLastRequest;
	long m_durationToServer;
    long m_durationToServerStamp;

	// Modern HUD Overlay (Minimap / Radar + Active Quest Tracker + Screenshot toast)
	LPD3DXFONT			m_hudFontSmall;
	LPD3DXFONT			m_hudFontTitle;
	LPDIRECT3DDEVICE9	m_hudFontDevice;
	int					m_minimapMode;          // 0 = Normal, 1 = Large, 2 = Compact
	float				m_minimapZoomRadius;    // Radar radius in world units/meters
	bool				m_minimapNorthUp;       // false = Heading-Up, true = North-Up
	bool				m_questTrackerVisible;  // true = Expanded, false = Collapsed
	bool				m_questJournalSynced;   // true once initial silent quest sync sent
	DWORD				m_hudInvRequestStamp;   // last time the tracker asked the server what you carry
	DWORD				m_lastHudKeyTime;
	float				m_lastMouseScreenX;
	float				m_lastMouseScreenY;
	CString				m_hudToastText;
	DWORD				m_hudToastTime;

	// Zone map (EngineX Display > Capture Zone Map makes Maps\<zone>.png + .txt, make_map_poi.py makes .poi)
	LPDIRECT3DTEXTURE9	m_zoneMapTex;
	LPDIRECT3DDEVICE9	m_zoneMapDevice;
	CString				m_zoneMapName;
	bool				m_zoneMapLoaded;        // files for m_zoneMapName have been read
	bool				m_zoneMapHasBounds;
	float				m_zoneMapMinX, m_zoneMapMaxX, m_zoneMapMinZ, m_zoneMapMaxZ;
	std::vector<HudMapPoi>	m_zoneMapPois;
	std::vector<char>	m_zoneMapPoiQuest;      // per poi: 0 none, 1 active quest npc, 2 other journal npc
	DWORD				m_zoneMapQuestStamp;
	int					m_zoneMapLevels;        // 2+ = zone has floors, picture per floor Maps\<zone>_L<n>.png
	float				m_zoneMapLevelLo[8], m_zoneMapLevelHi[8];   // player height range of each floor
	LPDIRECT3DTEXTURE9	m_zoneMapLevelTex[8];
	int					m_zoneMapPlayerLevel;   // floor you are on
	int					m_worldMapViewLevel;    // floor shown on the M map, -1 = yours
	float				m_wmFloorL, m_wmFloorR, m_wmFloorT[8], m_wmFloorB[8];

	// Big world map (M)
	bool				m_worldMapOpen;
	float				m_worldMapZoom;         // 1 = whole zone
	float				m_worldMapCenterX, m_worldMapCenterZ;
	bool				m_worldMapDragging;
	float				m_wmDragX, m_wmDragY;
	float				m_wmLeft, m_wmTop, m_wmSize;            // last drawn layout (used by input)
	float				m_wmViewMinX, m_wmViewMaxZ, m_wmScale;
	float				m_wmFull, m_wmMidX, m_wmMidZ;
	float				m_wmCloseL, m_wmCloseT, m_wmCloseR, m_wmCloseB;
	float				m_wmListL, m_wmListR;
	std::vector<HudMapQuestRow>	m_wmQuestRows;

	//Functions
	bool HandleInput(CInput *pInput);
	bool HandleHudOverlayInput(CInput *pInput);
	void EnsureHudFonts(LPDIRECT3DDEVICE9 pDevice);
	void ReleaseHudFonts();
	void RenderModernHudOverlays(long currentTime);
	void EnsureZoneMap(LPDIRECT3DDEVICE9 dev, const CString & zoneName);
	void ReleaseZoneMap();
	void UpdatePoiQuestStates();
	int  ZoneMapLevelOf(float y);
	float QuestTrackerRight(float screenW);
	LPDIRECT3DTEXTURE9 ZoneMapTexture(int level);
	bool IsHudWorldActive();
	void SetWorldMapOpen(bool open);
	void ClampWorldMapCenter();
	void RenderWorldMap(LPDIRECT3DDEVICE9 dev, CMovementObj * ptrPlayer, float screenW, float screenH, float playerYawRad);
	void ToggleQuestJournalWindow();
	bool CaptureScreenshot();
	int ClickActionHandler(CCLickZoneObj* clickObj,
						   CMenu3DObj* menu,
						   BOOL overrideDelay);
	int GetCurSelFromMenuList(int controlID,CMenuVisObj ** visRef);
	CMenuVisObj * GetVisualByID(int controlID);
	BOOL CreateCharacterThroughMenus(int nameID,
									 int entityID,
								     int spawnIndex);
	BOOL IndexDownDim(  CSkeletonObject * runtimeSkeletal,
						int cfg,
						CMovementObjList * entityDB,
						int refModel);
	BOOL IndexUpDim(CSkeletonObject * runtimeSkeletal,
					int cfg,
					CMovementObjList * entityDB,
					int refModel);
	int GetMiscIntFromMenuList(int controlID);
	int MinusSelectedFromMenuList(int controlID,int glid,int quanity);
    int MinusSelectedFromMenuList(int controlID,int glid,int quanity,CString & lastItemName);
	CString RemoveSelectedFromMenuList(int controlID);
	BOOL HousePlacementValidCheck(D3DVECTOR charPosition,ABBOX box);
	void RestoreFromDefaults();
	BOOL RenderCGMenus(long currentTime);
	BOOL GetCurrentJetData(CMovementObj * ptrLoc,int & cur,int & max);
	CString GetCompasDir(CMovementObj * dbPtr);
	//GetEntityDir returns the angle (degrees) in which the player is facing
	float GetEntityDir(CMovementObj * dbPtr);
	BOOL RenderOverlays();
	void RenderTextCache(CTextureRenderObjList*textRList,CViewportObj* vptPtr,int fontSysDB);
	void RenderCGMenuCursors(long currentTime);
	BOOL CGCursorRender(CBoneCursorObj * cursor);
	int GetMiscIntFromMenuList(int controlID, int & dupPassed);
	//void EvaluateFinalCursorPosition(D3DVECTOR & finalCurPositionRet);
};
