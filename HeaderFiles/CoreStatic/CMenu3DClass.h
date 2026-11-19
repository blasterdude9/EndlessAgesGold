
#ifndef CMENU3DCLASS_H
#define CMENU3DCLASS_H

#include "matrixarb.h"
#include "CMovementModel.h"
#include "CBoneCursorClass.h"
#include "CTextRenderClass.h"
#include "CMenuSliderClass.h"
#include "CMenuBarClass.h"
#include "CIconSystemClass.h"
#include "CABFunctionLib.h"
//#include "CMultiplayerClass.h"
#include "CSoundTrackClass.h"
#include "dplay8.h"
#include "dplobby.h"
//DirectDraw 7 removal: CABD3DFunctionLib.h deleted - keep its non-DDraw
//transitive includes explicit so downstream translation units still build
#include <mmsystem.h>
#include "dsound.h"
#include "dinput.h"
#include "CCommonCtrObj.h"
#include "ControlsClass.h"

//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
class  CMenuMergeObj : public CObject
{
	
     
	DECLARE_SERIAL( CMenuMergeObj );
	
    
public:
    CMenuMergeObj();
	void SafeDelete();
    void           Clone(CMenuMergeObj ** copy);
	int            m_menuID;
	void Serialize( CArchive& ar );
};
//--------------------------------------------//
//--------------------------------------------//
class  CMenuMergeList : public CObList
{
public:
	CMenuMergeList(){};
   void SafeDelete();
   void Clone(CMenuMergeList ** copy);
    
     
	DECLARE_SERIAL( CMenuMergeList );
	
    
};
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
class  CCLickZoneObj : public CObject
{
	
     
	DECLARE_SERIAL( CCLickZoneObj );
	
    
public:
    CCLickZoneObj();
    void           Clone(CCLickZoneObj ** copy);
	MatrixARB      GL_matrixFunctions;
    BOOL           m_radiusType;//false uses rectangle
	float          m_radius;
	D3DVECTOR      m_center;

	float          m_rectMaxX;
	float          m_rectMaxY;
	float          m_rectMinX;
    float          m_rectMinY;
	
	int            m_visualIndex;

	int            m_action;//0=none 1=close menu 2=edit text 
	int            m_miscInt;
	int            m_miscInt2;
	int            m_miscInt3;
	CString        m_miscString;

	BOOL           m_state;//0=up 1=down 2=over

	BOOL           m_execOnCloseing;
	BOOL           m_validForExec;
	BOOL           m_previouslyActuated;

	BOOL           m_switchOverride;
	int            m_switchOverrideState;
	BOOL           m_dropZone;
	BOOL           m_tempObject;
	BOOL           m_default;

	
	BOOL Update(D3DVECTOR cursorPosition, ButtonState *leftButtonState);
	BOOL UpdateCurrentlyPressedState(D3DVECTOR cursorPosition, ButtonState *leftButtonState);
	void Serialize( CArchive& ar );
};
//--------------------------------------------//
//--------------------------------------------//
class  CCLickZoneList : public CObList
{
public:
   CCLickZoneList();

   BOOL m_mouseWasPressed;
   void SafeDelete();
   void Clone(CCLickZoneList ** copy);
   void UpdateAll(D3DVECTOR cursorPosition, ButtonState *leftButtonState, BOOL affectTempOnly);
   CCLickZoneObj * GetByIndex(int index);
    
     
	DECLARE_SERIAL( CCLickZoneList );
	
    

	void ClearTempObjects();
};
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
class  CMenuVisObj : public CObject
{
	
     
	DECLARE_SERIAL( CMenuVisObj );
	
    
public:
    CMenuVisObj();
	
	
	
	void Clone(CMenuVisObj ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice);

    MatrixARB                  GL_matrixFunctions;
	CString                    m_visualName;


	int                        m_textBlendMode;
	//opening closing
	D3DVECTOR                  m_lockLocation;
    D3DVECTOR                  m_startLocation;
	D3DVECTOR                  m_endLocation;
	long                       m_transitionTime;
	BOOL                       m_fadingIn;
	BOOL                       m_fadingOut;
	long                       m_fadeTime;

	//display modifiers
	float                      m_rotationsPerSecond;
	long                       m_rotationTimeStamp;
	float                      m_currentRotation;
	int                        m_rotationDuration;

	//resize "on the fly"
	float                      m_scaleFactor;

    //creation flags
	BOOL                       m_tempMenuObject;
	int                        m_boundToClickZoneStatus;

	//visuals
	int                        m_aiEntityCfgRef;//-1 = disabled
	BOOL                       m_runtimeDB;
    CMovementObj              *m_entityObjectLink;
	int                        m_entityObjectDBIndex;//-1 = none

	CString                    m_textObject;
	CTextureRenderObj         *m_textRenderLink;
	BOOL                       m_editable;
	BOOL                       m_currentlyBiengEdited;
	CString                    m_lastKeyPressed;
	long                       m_caretTimeStamp;
	long                       m_caretDuration;
	BOOL                       m_caretOn;
	BOOL                       m_defaultcontrol;
	float                      m_colorRed;
	float                      m_colorGreen;
	float                      m_colorBlue;
	float                      m_justify;

	CSkeletonObject           *m_skeletonObject;

	CSliderBarObj             *m_sliderSystem;

	CMenuBarObj               *m_menuBarObj;
	BOOL                       m_tempObject;

	//data send if clicked
    int                        m_id;
	BOOL                       m_type;//0=text object 1=entity link 2=skeletal 3=IP Link 4=Username Link 5=Password Link 6=Slider system
	

	//skeletal system Specific
	int                        m_openAnimIndex;
	int                        m_idleAnimIndex;
	int                        m_closeAnimIndex;
	int                        m_currentAnimation;

	//font specific
	float                      m_fontSizeX;
	float                      m_fontSizeY;


	int                        m_visualID;

	int                        m_commCntlType;
	BOOL                       m_commCntlState;

	

    BOOL GetKeyIfPressed(CString *key, ButtonState keyboardKeys[KEYBOARD_MAX_KEYS]);
    void SetFadeOut(long duration);
	void SetFadeIn(long duration);
    void SafeDelete();
	BOOL BuildSkeletal(CString arbFile,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void Serialize( CArchive& ar );
	
};
//--------------------------------------------//
//--------------------------------------------//
class  CMenuVisObjList : public CObList
{
public:
   CMenuVisObjList(){};
   void SafeDelete();
   void Clone(CMenuVisObjList ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice);
   void SetClosingStates();
   CMenuVisObj * GetByID(int id);
    
     
	DECLARE_SERIAL( CMenuVisObjList );
	
    

	void ClearTempObjects();

	
};
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
class  CMenu3DObj : public CObject
{
	
     
	DECLARE_SERIAL( CMenu3DObj );
	
    
public:
    CMenu3DObj();
    void Clone(CMenu3DObj ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice);
	
    CString                    m_menuName;
    int                        m_menuAccessID;
	CMenuVisObjList           *m_visualItems;
	long                       m_timeStamp;
	CBoneCursorObj            *m_cursor;
	CBoneCursorObjList        *m_additionalCursors;
	BOOL                       m_closing;
	long                       m_closeDuration;
	long                       m_timeStampWhenClosed;
	CCLickZoneList            *m_clickActions;
	BOOL                       m_ready;
	BOOL                       m_instantClose;
	BOOL                       m_lockControls;
	int                        m_triggerSoundtrack;
	BOOL                       m_closeOnEsc;
	CIconSysList              *m_iconDynamicOverlays;
	int                        m_specialOperation;//0=none
	int                        m_specialOperationMiscInt;
	int                        m_passedInVar;
	int                        m_passedInCommand;
	long                       m_lastChangeStateTimeStamp;
    BOOL                       m_captureControlQuerry;
	int                        m_controlActionListID;
	int                        m_controlInvID;
	int                        m_controlStatusID;

	CMenuMergeList            *m_menuMergeList;

	D3DVECTOR                  m_offsetMenu;
	float                      m_scale;

	BOOL                       m_resolutionBased;
	BOOL                       m_lockedForChildMenuResponse;

	int                        m_tempMenuMiscInt;
	int                        m_tempMenuMiscInt2;
	int                        m_tempMenuMiscInt3;
	int                        m_tempMenuAction;
	BOOL                       m_centerMenu;
	BOOL                       m_clearTempMenus;


	//macro
	int                        m_macroRepeatCount;
	int                        m_macroMaxLimit;
	int                        m_macroActionType;
	int                        m_macroCurrent;
	long                       m_macroManditoryWait;
	long                       m_macroLastActionTimeStamp;
	BOOL                       m_macroRunning;
	long                       m_macroStartStamp;
	


	BOOL                       m_restrictChat;


    void SafeDelete();
	BOOL CloseMenu(D3DVECTOR * referenecePos);
	void ChangeNextTextField();
	BOOL Update(long currentTime, ButtonState *leftButtonState, D3DVECTOR & unifiedCursorPosition);
	void         AddClickAction(BOOL radiusType,
								float radius,
								D3DVECTOR center,
								float maxX,
								float maxY,
								float minX,
								float minY,
								int visIndex,
								int action,
								int miscInt,
								int miscInt2);
	void ScaleOffset(D3DVECTOR offsetPos,float scale);
	void ScaleOffset(D3DVECTOR offsetPos,float scaleX,float scaleY);
	CMenuVisObj * GetByID(int id);
	void MergeMenu(CMenu3DObj * menuToMerge,LPDIRECT3DDEVICE9 g_pd3dDevice,BOOL tempMerge);
	void ClearTempMenus();

	
	void Serialize( CArchive& ar );
	
};
//--------------------------------------------//
//--------------------------------------------//
class  CMenu3DObjList : public CObList
{
public:
   CMenu3DObjList(){};

   CABFunctionLib CABLIB;

   void SafeDelete();
   void Clone(CMenu3DObjList ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice);
    
     
	DECLARE_SERIAL( CMenu3DObjList );
	
	BOOL AddStringToMenuList(int controlID,
						     CString string,
							 int visualRefIndex,
						     int miscInt,
						     int miscInt2,
						     CString regItemName,
							 int mode,
							 CString costExt);
    
    BOOL AddStringToMenuList(int controlID,
						     CString string,
							 int visualRefIndex,
						     int miscInt,
						     int miscInt2,
						     CString regItemName,
							 int mode,
							 CString costExt,
							 int miscInt3,
						     int miscInt4);

	void LoadTexturesForMenu(CMenu3DObj * menu3DObj,CTextureDBObjList * textureDataBase,CTextureDBObjList * dynDataBase,CCommonCtrlList * cntrlDB);

	void InListActionsPlain(int currentSel,CSliderBarObj * slideBarObj,ControlDBList * ControlsList,CSkillObjectList * m_skillDatabase);
	void InListActions(int currentSel,CSliderBarObj * slideBarObj,ControlDBList * ControlsList,CSkillObjectList * m_skillDatabase);
	CMenu3DObj * SetTextureToObjectByName(  CString & visualName,CString & texturename,int textureLevel);
	CMenuVisObj * GetVisualByName(CString visualName);
	CMenu3DObj * AddStringToMenuListDistributed(CString visualName,
															CString textdata,
															float dxRenderWidth,
															float cgFontSizeX,
															void * symbolMapMem,
															float lineLength);


	BOOL CloseByID(int id);
	int GetFreeID();
	CMenu3DObj * GetByID(int id);
	
};

#endif