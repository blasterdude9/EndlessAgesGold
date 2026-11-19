#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>

#include "CMenu3DClass.h"
#include "CTextRenderClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CMenuVisObj::CMenuVisObj()
{
	m_justify               = 0.0f;
	m_lockLocation.x        = 0.0f;
	m_lockLocation.y        = 0.0f;
	m_lockLocation.z        = 0.0f;
	m_startLocation.x       = 0.0f;
	m_startLocation.y       = 0.0f;
	m_startLocation.z       = 0.0f;
    m_endLocation.x         = 0.0f;
	m_endLocation.y         = 0.0f;
	m_endLocation.z         = 0.0f;
    m_transitionTime        = 2000;
    m_fadingIn              = FALSE;
    m_fadingOut             = FALSE;
    m_fadeTime              = -1;
    m_rotationsPerSecond    = 0;
    m_rotationTimeStamp     = GetTickCount();
    m_scaleFactor           = 0.0015f;
	m_tempMenuObject        = FALSE;
    m_entityObjectLink      = NULL;
	m_entityObjectDBIndex   = -1;//-1 = none
    m_skeletonObject        = NULL;
    m_id                    = 0;
	m_type                  = 0;
	m_currentRotation       = 0;
	m_rotationDuration      = 1000;
    m_openAnimIndex         = 0;
    m_idleAnimIndex         = 1;
    m_closeAnimIndex        = 2;
	m_currentAnimation      = 0;
	m_textRenderLink        = NULL;
    m_fontSizeX             = .015f;
    m_fontSizeY             = .015f;
	m_runtimeDB             = FALSE;
    m_editable              = FALSE;
    m_currentlyBiengEdited  = FALSE;
	m_lastKeyPressed        = "zz";
	m_caretTimeStamp        = GetTickCount();
    m_caretDuration         = 250;
	m_caretOn               = FALSE;
	m_visualName            = "Unknown";
	m_textBlendMode         = 2;
	m_sliderSystem          = NULL;
	m_visualID              = 0;
	m_menuBarObj            = NULL;
	m_aiEntityCfgRef        = -1;
	m_boundToClickZoneStatus = -1;
	m_defaultcontrol        = FALSE;
	m_commCntlType          = 0;
	m_commCntlState         = 0;
	m_tempObject            = FALSE;
	m_colorRed              = 1.0f;
    m_colorGreen            = 1.0f;
    m_colorBlue             = 1.0f;
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
void CMenuVisObj::Clone(CMenuVisObj ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     CMenuVisObj* copyLocal = new CMenuVisObj();

	copyLocal->m_lockLocation.x        = m_lockLocation.x;
	copyLocal->m_lockLocation.y        = m_lockLocation.y;
	copyLocal->m_lockLocation.z        = m_lockLocation.z;
	copyLocal->m_startLocation.x       = m_startLocation.x;
	copyLocal->m_startLocation.y       = m_startLocation.y;
	copyLocal->m_startLocation.z       = m_startLocation.z;
    copyLocal->m_endLocation.x         = m_endLocation.x;
	copyLocal->m_endLocation.y         = m_endLocation.y ;
	copyLocal->m_endLocation.z         = m_endLocation.z;
    copyLocal->m_transitionTime        = m_transitionTime;
    copyLocal->m_fadingIn              = m_fadingIn;
    copyLocal->m_fadingOut             = m_fadingOut;
    copyLocal->m_fadeTime              = m_fadeTime;
    copyLocal->m_rotationsPerSecond    = m_rotationsPerSecond;
    copyLocal->m_rotationTimeStamp     = m_rotationTimeStamp;
    copyLocal->m_scaleFactor           = m_scaleFactor;
	copyLocal->m_tempMenuObject        = m_tempMenuObject; 
	copyLocal->m_entityObjectDBIndex   = m_entityObjectDBIndex;
    copyLocal->m_id                    = m_id;
	copyLocal->m_type                  = m_type;
	copyLocal->m_currentRotation       = m_currentRotation;
	copyLocal->m_rotationDuration      = m_rotationDuration;
    copyLocal->m_openAnimIndex         = m_openAnimIndex;
    copyLocal->m_idleAnimIndex         = m_idleAnimIndex;
    copyLocal->m_closeAnimIndex        = m_closeAnimIndex;
	copyLocal->m_currentAnimation      = m_currentAnimation;
	copyLocal->m_textObject            = m_textObject;
	copyLocal->m_fontSizeX             = m_fontSizeX;
    copyLocal->m_fontSizeY             = m_fontSizeY;
	copyLocal->m_runtimeDB             = m_runtimeDB;
	copyLocal->m_editable              = m_editable;
    copyLocal->m_currentlyBiengEdited  = m_currentlyBiengEdited;
	copyLocal->m_caretDuration         = m_caretDuration;
	copyLocal->m_caretOn               = m_caretOn;
	copyLocal->m_visualName            = m_visualName;
	copyLocal->m_textBlendMode         = m_textBlendMode;
	copyLocal->m_visualID              = m_visualID;
	copyLocal->m_aiEntityCfgRef        = m_aiEntityCfgRef;
	copyLocal->m_boundToClickZoneStatus = m_boundToClickZoneStatus;
	copyLocal->m_defaultcontrol        = m_defaultcontrol;
	copyLocal->m_commCntlType          = m_commCntlType;
	copyLocal->m_commCntlState         = m_commCntlState;
	copyLocal->m_tempObject            = m_tempObject;
	copyLocal->m_colorRed              = m_colorRed;
    copyLocal->m_colorGreen            = m_colorGreen;
    copyLocal->m_colorBlue             = m_colorBlue;
	copyLocal->m_justify			   = m_justify;



	if(m_skeletonObject)
    m_skeletonObject->Clone(&copyLocal->m_skeletonObject,g_pd3dDevice,NULL);

	if(m_sliderSystem)
	m_sliderSystem->Clone(&copyLocal->m_sliderSystem,g_pd3dDevice);

	if(m_menuBarObj)
	m_menuBarObj->Clone(&copyLocal->m_menuBarObj,g_pd3dDevice);

	*copy = copyLocal;
}
//---------------------END FUNCTION----------------------------//
//-------------------------------------------------------------//
//--this functiongets the distance between 2 3dpoints----------//
BOOL CMenuVisObj::GetKeyIfPressed(CString * key, ButtonState keyboardKeys[KEYBOARD_MAX_KEYS])
{
	*key = "5555555";//def
	//----
	if(keyboardKeys[DIK_NUMPAD1].IsDown())
	{
		*key = "1";
		return TRUE;
	}
	if(keyboardKeys[DIK_NUMPAD2].IsDown())
	{
		*key = "2";
		return TRUE;
	}
	if(keyboardKeys[DIK_NUMPAD3].IsDown())
	{
		*key = "3";
		return TRUE;
	}
	if(keyboardKeys[DIK_NUMPAD4].IsDown())
	{
		*key = "4";
		return TRUE;
	}
	if(keyboardKeys[DIK_NUMPAD5].IsDown())
	{
		*key = "5";
		return TRUE;
	}
	if(keyboardKeys[DIK_NUMPAD6].IsDown())
	{
		*key = "6";
		return TRUE;
	}
	if(keyboardKeys[DIK_NUMPAD7].IsDown())
	{
		*key = "7";
		return TRUE;
	}
	if(keyboardKeys[DIK_NUMPAD8].IsDown())
	{
		*key = "8";
		return TRUE;
	}
	if(keyboardKeys[DIK_NUMPAD9].IsDown())
	{
		*key = "9";
		return TRUE;
	}
	if(keyboardKeys[DIK_NUMPAD0].IsDown())
	{
		*key = "0";
		return TRUE;
	}
	if(keyboardKeys[DIK_1].IsDown())
	{
		*key = "1";
		return TRUE;
	}
	if(keyboardKeys[DIK_2].IsDown())
	{
		*key = "2";
		return TRUE;
	}
	if(keyboardKeys[DIK_3].IsDown())
	{
		*key = "3";
		return TRUE;
	}
	if(keyboardKeys[DIK_4].IsDown())
	{
		*key = "4";
		return TRUE;
	}
	if(keyboardKeys[DIK_5].IsDown())
	{
		*key = "5";
		return TRUE;
	}
	if(keyboardKeys[DIK_6].IsDown())
	{
		*key = "6";
		return TRUE;
	}
	if(keyboardKeys[DIK_7].IsDown())
	{
		*key = "7";
		return TRUE;
	}
	if(keyboardKeys[DIK_8].IsDown())
	{
		*key = "8";
		return TRUE;
	}
	if(keyboardKeys[DIK_9].IsDown())
	{
		*key = "9";
		return TRUE;
	}
	if(keyboardKeys[DIK_0].IsDown())
	{
		*key = "0";
		return TRUE;
	}
	if(keyboardKeys[DIK_A].IsDown())
	{
		*key = "A";
		return TRUE;
	}
	if(keyboardKeys[DIK_B].IsDown())
	{
		*key = "B";
		return TRUE;
	}
	if(keyboardKeys[DIK_C].IsDown())
	{
		*key = "C";
		return TRUE;
	}
	if(keyboardKeys[DIK_D].IsDown())
	{
		*key = "D";
		return TRUE;
	}
	if(keyboardKeys[DIK_E].IsDown())
	{
		*key = "E";
		return TRUE;
	}
	if(keyboardKeys[DIK_F].IsDown())
	{
		*key = "F";
		return TRUE;
	}
	if(keyboardKeys[DIK_G].IsDown())
	{
		*key = "G";
		return TRUE;
	}
	if(keyboardKeys[DIK_H].IsDown())
	{
		*key = "H";
		return TRUE;
	}
	if(keyboardKeys[DIK_I].IsDown())
	{
		*key = "I";
		return TRUE;
	}
	if(keyboardKeys[DIK_J].IsDown())
	{
		*key = "J";
		return TRUE;
	}
	if(keyboardKeys[DIK_K].IsDown())
	{
		*key = "K";
		return TRUE;
	}
	if(keyboardKeys[DIK_L].IsDown())
	{
		*key = "L";
		return TRUE;
	}
	if(keyboardKeys[DIK_M].IsDown())
	{
		*key = "M";
		return TRUE;
	}
	if(keyboardKeys[DIK_N].IsDown())
	{
		*key = "N";
		return TRUE;
	}
	if(keyboardKeys[DIK_O].IsDown())
	{
		*key = "O";
		return TRUE;
	}
	if(keyboardKeys[DIK_P].IsDown())
	{
		*key = "P";
		return TRUE;
	}
	if(keyboardKeys[DIK_Q].IsDown())
	{
		*key = "Q";
		return TRUE;
	}
	if(keyboardKeys[DIK_R].IsDown())
	{
		*key = "R";
		return TRUE;
	}
	if(keyboardKeys[DIK_S].IsDown())
	{
		*key = "S";
		return TRUE;
	}
	if(keyboardKeys[DIK_T].IsDown())
	{
		*key = "T";
		return TRUE;
	}
	if(keyboardKeys[DIK_U].IsDown())
	{
		*key = "U";
		return TRUE;
	}
	if(keyboardKeys[DIK_V].IsDown())
	{
		*key = "V";
		return TRUE;
	}
	if(keyboardKeys[DIK_W].IsDown())
	{
		*key = "W";
		return TRUE;
	}
	if(keyboardKeys[DIK_X].IsDown())
	{
		*key = "X";
		return TRUE;
	}
	if(keyboardKeys[DIK_Y].IsDown())
	{
		*key = "Y";
		return TRUE;
	}
	if(keyboardKeys[DIK_Z].IsDown())
	{
		*key = "Z";
		return TRUE;
	}
	if(keyboardKeys[DIK_SPACE].IsDown())
	{
		*key = " ";
		return TRUE;
	}
	if(keyboardKeys[DIK_ADD].IsDown())
	{
		*key = "+";
		return TRUE;
	}
	if(keyboardKeys[DIK_SUBTRACT].IsDown())
	{
		*key = "-";
		return TRUE;
	}
	if(keyboardKeys[DIK_EQUALS].IsDown())
	{
		*key = "=";
		return TRUE;
	}
	if(keyboardKeys[DIK_LBRACKET].IsDown())
	{
		*key = "[";
		return TRUE;
	}
	if(keyboardKeys[DIK_RBRACKET].IsDown())
	{
		*key = "]";
		return TRUE;
	}
	if(keyboardKeys[DIK_SEMICOLON].IsDown())
	{
		*key = ":";
		return TRUE;
	}
	if(keyboardKeys[DIK_APOSTROPHE].IsDown())
	{
		*key = "'";
		return TRUE;
	}
	if(keyboardKeys[DIK_COMMA].IsDown())
	{
		*key = ",";
		return TRUE;
	}
	if(keyboardKeys[DIK_PERIOD].IsDown())
	{
		*key = ".";
		return TRUE;
	}
	if(keyboardKeys[DIK_BACKSLASH].IsDown())
	{
		*key = "\\";
		return TRUE;
	}
	if(keyboardKeys[DIK_MULTIPLY].IsDown())
	{
		*key = "*";
		return TRUE;
	}
	if(keyboardKeys[DIK_MINUS].IsDown())
	{
		*key = "-";
		return TRUE;
	}
	if(keyboardKeys[DIK_SLASH].IsDown())
	{
		*key = "?";
		return TRUE;
	}

	return FALSE;
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
void CMenuVisObj::SetFadeIn(long duration)
{
    if(m_entityObjectLink)
	   m_entityObjectLink->List_skeletalObject->FadeInAmplified(duration);
	if(m_skeletonObject)
       m_skeletonObject->FadeInAmplified(duration);
	if(m_textRenderLink){//text object
       m_textRenderLink->m_materialObj->SetFadeIn(duration,TRUE);
	}//end text object
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
void CMenuVisObj::SetFadeOut(long duration)
{
    if(m_entityObjectLink)
	   m_entityObjectLink->List_skeletalObject->FadeOutAmplified(duration);
	if(m_skeletonObject)
	   m_skeletonObject->FadeOutAmplified(duration);
	if(m_textRenderLink)//text object
       m_textRenderLink->m_materialObj->SetFadeOut(duration);
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
void CMenuVisObj::SafeDelete()
{
    if(m_entityObjectLink){
	   delete m_entityObjectLink;
	   m_entityObjectLink=0;
	}
    if(m_skeletonObject){
       m_skeletonObject->SafeDelete();
	   delete m_skeletonObject;
       m_skeletonObject=0;
	}
	if(m_textRenderLink){
       m_textRenderLink->SafeDelete();
	   delete m_textRenderLink;
	   m_textRenderLink = 0;
       //m_textRenderLink->m_selfDeleteIt = TRUE;
	}  
	if(m_sliderSystem){
       m_sliderSystem->SafeDelete();
	   delete m_sliderSystem;
	   m_sliderSystem=0;
	}

	if(m_menuBarObj)
	{
       m_menuBarObj->SafeDelete();
	   delete m_menuBarObj;
       m_menuBarObj = 0;
	}

	
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
void CMenuVisObj::Serialize( CArchive& ar )
{
BOOL reservedBool = 0;


CObject::Serialize( ar );
	if(ar.IsStoring() ){

       ar.SetObjectSchema(2);
       ASSERT(ar.GetObjectSchema() == 2);

	   ar << m_lockLocation.x 
	      << m_lockLocation.y
	      << m_lockLocation.z 
	      << m_startLocation.x 
	      << m_startLocation.y 
	      << m_startLocation.z  
	      << m_endLocation.x   
	      << m_endLocation.y      
	      << m_endLocation.z      
	      << m_transitionTime  
	      << m_fadingIn             
	      << m_fadingOut          
	      << m_fadeTime            
	      << m_scaleFactor          
	      << m_tempMenuObject        
	      << m_entityObjectLink     
	      << m_entityObjectDBIndex   
	      << m_skeletonObject       
	      << m_id
		  << m_type
		  << m_rotationsPerSecond
          << m_rotationTimeStamp
		  << m_openAnimIndex
		  << m_idleAnimIndex
		  << m_closeAnimIndex
		  << m_fontSizeX
		  << m_fontSizeY
		  << m_runtimeDB
		  << m_editable
		  << m_caretDuration
		  << m_visualName
		  << m_textBlendMode
		  << m_textObject
		  << m_sliderSystem
		  << m_visualID
		  << m_menuBarObj
		  << m_aiEntityCfgRef
		  << m_boundToClickZoneStatus
		  << m_defaultcontrol
		  << m_commCntlType;

	   ar << m_colorRed
	      << m_colorGreen
	      << m_colorBlue
		  << m_rotationDuration
		  << m_justify
		  << reservedBool;
     

	}
	else
	{
       int version = ar.GetObjectSchema();

       ar >> m_lockLocation.x 
	      >> m_lockLocation.y
	      >> m_lockLocation.z 
	      >> m_startLocation.x 
	      >> m_startLocation.y 
	      >> m_startLocation.z  
	      >> m_endLocation.x   
	      >> m_endLocation.y      
	      >> m_endLocation.z      
	      >> m_transitionTime  
	      >> m_fadingIn             
	      >> m_fadingOut          
	      >> m_fadeTime            
	      >> m_scaleFactor          
	      >> m_tempMenuObject        
	      >> m_entityObjectLink     
	      >> m_entityObjectDBIndex   
	      >> m_skeletonObject       
	      >> m_id
		  >> m_type
		  >> m_rotationsPerSecond
          >> m_rotationTimeStamp
		  >> m_openAnimIndex
		  >> m_idleAnimIndex
		  >> m_closeAnimIndex
		  >> m_fontSizeX
		  >> m_fontSizeY
		  >> m_runtimeDB
		  >> m_editable
		  >> m_caretDuration
		  >> m_visualName
		  >> m_textBlendMode
		  >> m_textObject
		  >> m_sliderSystem
		  >> m_visualID
		  >> m_menuBarObj
		  >> m_aiEntityCfgRef
		  >> m_boundToClickZoneStatus
		  >> m_defaultcontrol
		  >> m_commCntlType;              
       

	    m_colorRed              = 1.0f;
	    m_colorGreen            = 1.0f;
	    m_colorBlue             = 1.0f;
		m_rotationDuration      = 4000;

		if(version == 2)
		{
		ar >> m_colorRed
	       >> m_colorGreen
	       >> m_colorBlue
		   >> m_rotationDuration
		   >> m_justify
		   >> reservedBool;
		}

		if(m_rotationDuration == 0)
		   m_rotationDuration = 4000;

       //m_menuBarObj            = NULL;

	   //m_boundToClickZoneStatus = -1;

	   m_caretOn               = 0;
	   m_currentRotation       = 0;
	   m_rotationTimeStamp     = 0;
	   m_currentAnimation      = 0;
	   m_textRenderLink        = NULL;
	   m_currentlyBiengEdited  = FALSE;
	   m_lastKeyPressed        = "zz";
	   m_caretTimeStamp        = 0;
	   

	   //m_commCntlType          = 0;
	   m_commCntlState         = 0;
	   m_tempObject            = 0;

	  

	   
	   
	   //m_visualID              = 0;
	   //m_sliderSystem          = NULL;
	  

	}
} 
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
BOOL CMenuVisObj::BuildSkeletal(CString arbFile,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	if(m_skeletonObject){
       m_skeletonObject->SafeDelete();
       delete m_skeletonObject;
	   m_skeletonObject=0;
	}
    m_skeletonObject = new CSkeletonObject();
    if(!m_skeletonObject->InitBoneSystem(arbFile,g_pd3dDevice)){//failed
       m_skeletonObject->SafeDelete();
	   delete m_skeletonObject;
	   m_skeletonObject=0;
       return FALSE;
	}//end failed

	m_type           = 2;

return TRUE;
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
void CMenuVisObjList::Clone(CMenuVisObjList ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     CMenuVisObjList * copyLocal = new CMenuVisObjList();
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CMenuVisObj * visObj = (CMenuVisObj *)GetNext(posLoc);
        CMenuVisObj * clonedPtr = NULL; 
		visObj->Clone(&clonedPtr,g_pd3dDevice);
		copyLocal->AddTail(clonedPtr);
	 }
	 *copy = copyLocal; 
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
CMenuVisObj * CMenuVisObjList::GetByID(int id)
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CMenuVisObj * visObj = (CMenuVisObj *)GetNext(posLoc);
        if(visObj->m_visualID == id)
		   return visObj;

	 }
	 return NULL;
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
void CMenuVisObjList::SetClosingStates()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CMenuVisObj * visObj = (CMenuVisObj *)GetNext(posLoc);
        visObj->m_currentAnimation = visObj->m_closeAnimIndex;
	}
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
void CMenuVisObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CMenuVisObj * menuObj = (CMenuVisObj *)GetNext(posLoc);
		menuObj->SafeDelete();
        delete menuObj;
		menuObj=0;
	}
	RemoveAll();
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CMenuVisObjList::ClearTempObjects()
{
	POSITION posLast = NULL;
    for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	{
        CMenuVisObj * cObj = (CMenuVisObj *)GetNext(posLoc);
		if(cObj->m_tempObject)
		{
		   cObj->SafeDelete();
           delete cObj;
		   cObj=0;
		   RemoveAt(posLast);
		}
	}
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
CMenu3DObj::CMenu3DObj()
{
	m_additionalCursors   = NULL;
    m_menuAccessID        = 0;
    m_visualItems         = NULL;
	m_timeStamp           = GetTickCount();
	m_cursor              = NULL;
	m_closing             = FALSE;
    m_closeDuration       = 2000;
	m_timeStampWhenClosed = GetTickCount();
	m_clickActions        = NULL;
	m_ready               = FALSE;
	m_instantClose        = TRUE;
	m_lockControls        = TRUE;
	m_triggerSoundtrack   = 0;
	m_closeOnEsc          = FALSE;
	m_iconDynamicOverlays = NULL;
	m_specialOperation    = 0;
	m_specialOperationMiscInt = 0;
	m_passedInVar           = 0;
	m_passedInCommand       = 0;
	m_captureControlQuerry  = FALSE;
    m_controlActionListID   = -1;
	m_controlInvID          = -1;
	m_controlStatusID       = 0;
	m_offsetMenu.x          = 0.0f;
	m_offsetMenu.y          = 0.0f;
	m_offsetMenu.z          = 0.0f;
    m_scale                 = 1.0f;
	m_menuMergeList         = NULL;
	m_resolutionBased       = FALSE;
	m_lockedForChildMenuResponse = 0;

	m_tempMenuMiscInt       = 0;
	m_tempMenuMiscInt2      = 0;
	m_tempMenuMiscInt3      = 0;
	m_tempMenuAction        = 0;

	m_centerMenu            = 0;
	m_clearTempMenus        = 0;

	m_macroRepeatCount         = 1;
	m_macroMaxLimit            = 1000;
	m_macroActionType          = 0;
	m_macroCurrent             = 0;
	m_macroManditoryWait       = 10000;
	m_macroLastActionTimeStamp = GetTickCount();
	m_macroRunning             = FALSE;
	m_macroStartStamp          = m_macroLastActionTimeStamp;

	m_restrictChat                = FALSE;
	
	m_lastChangeStateTimeStamp = GetTickCount();
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
CMenuVisObj * CMenu3DObj::GetByID(int id)
{
   if(!m_visualItems)
	   return NULL;


   for(POSITION posLoc = m_visualItems->GetHeadPosition();posLoc != NULL;)
   {
       CMenuVisObj * visPtr = (CMenuVisObj *)m_visualItems->GetNext(posLoc);
	   if(visPtr)
	   {
          if(visPtr->m_visualID == id)
             return visPtr;
	   }
   }
   return NULL;
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
void CMenu3DObj::Clone(CMenu3DObj ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     CMenu3DObj* copyLocal = new CMenu3DObj();
    
     copyLocal->m_menuAccessID        = m_menuAccessID;
     copyLocal->m_timeStamp           = m_timeStamp;
     copyLocal->m_closing             = m_closing;
     copyLocal->m_closeDuration       = m_closeDuration;
     copyLocal->m_timeStampWhenClosed = m_timeStampWhenClosed;
	 copyLocal->m_instantClose        = m_instantClose;
	 copyLocal->m_lockControls        = m_lockControls;
	 copyLocal->m_triggerSoundtrack   = m_triggerSoundtrack;
	 copyLocal->m_closeOnEsc          = m_closeOnEsc;
	 copyLocal->m_specialOperation    = m_specialOperation;
	 copyLocal->m_specialOperationMiscInt = m_specialOperationMiscInt;
	 copyLocal->m_passedInVar         = m_passedInVar;
	 copyLocal->m_passedInCommand     = m_passedInCommand;  
	 copyLocal->m_captureControlQuerry= m_captureControlQuerry;
     copyLocal->m_controlActionListID = m_controlActionListID;
	 copyLocal->m_controlInvID        = m_controlInvID;
	 copyLocal->m_controlStatusID     = m_controlStatusID;
	 copyLocal->m_offsetMenu.x        = m_offsetMenu.x;
	 copyLocal->m_offsetMenu.y        = m_offsetMenu.y;
	 copyLocal->m_offsetMenu.z        = m_offsetMenu.z;
     copyLocal->m_scale               = m_scale;
	 copyLocal->m_resolutionBased     = m_resolutionBased;
	 copyLocal->m_lockedForChildMenuResponse = m_lockedForChildMenuResponse;
	 copyLocal->m_centerMenu          = m_centerMenu;
	 copyLocal->m_clearTempMenus      = m_clearTempMenus;
	 copyLocal->m_restrictChat        = m_restrictChat;

     if(m_additionalCursors)
	    m_additionalCursors->Clone(&copyLocal->m_additionalCursors,g_pd3dDevice);

     if(m_menuMergeList)
	    m_menuMergeList->Clone(&copyLocal->m_menuMergeList);

	 if(m_clickActions)
        m_clickActions->Clone(&copyLocal->m_clickActions);
     
	 if(m_visualItems)
     m_visualItems->Clone(&copyLocal->m_visualItems,g_pd3dDevice);
	 
	 if(m_cursor)
	 m_cursor->Clone(&copyLocal->m_cursor,g_pd3dDevice);

	 *copy = copyLocal;
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CMenu3DObj::AddClickAction(BOOL radiusType,
								float radius,
								D3DVECTOR center,
								float maxX,
								float maxY,
								float minX,
								float minY,
								int visIndex,
								int action,
								int miscInt,
								int miscInt2)
{

	if(!m_clickActions)
		m_clickActions = new CCLickZoneList();

    CCLickZoneObj* clickObj = new CCLickZoneObj();

	clickObj->m_radiusType   = radiusType;//false uses rectangle
    clickObj->m_radius       = radius;

    clickObj->m_rectMaxX     = maxX;
    clickObj->m_rectMaxY     = maxY;
    clickObj->m_rectMinX     = minX;
    clickObj->m_rectMinY     = minY;
	
    clickObj->m_visualIndex  = visIndex;
    clickObj->m_action       = action;//0=none 1=close 
    clickObj->m_miscInt      = miscInt;
    clickObj->m_miscInt2     = miscInt2;
	clickObj->m_center.x     = center.x;
	clickObj->m_center.y     = center.y;
	clickObj->m_center.z     = center.z;

	clickObj->m_miscInt3;
	clickObj->m_miscString;

	m_clickActions->AddTail(clickObj);
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
BOOL CMenu3DObj::CloseMenu(D3DVECTOR * referenecePos)
{
   if(!m_ready)
	  return FALSE;

   if(m_closing)
	  return FALSE;

   if(referenecePos)
   if(this->m_cursor)
   {
      referenecePos->x = this->m_cursor->m_position.x;
	  referenecePos->y = this->m_cursor->m_position.y;
   }

   m_closing = TRUE;
   m_timeStampWhenClosed = GetTickCount();
   m_visualItems->SetClosingStates();
   for(POSITION posLoc = m_visualItems->GetHeadPosition();posLoc != NULL;)
   {//apply fades if need be
     CMenuVisObj*visPtr = (CMenuVisObj*)m_visualItems->GetNext(posLoc);
     if(visPtr->m_fadeTime > 0)
		visPtr->SetFadeOut(visPtr->m_fadeTime);

   }//end apply fades if need be
   return TRUE;
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CMenu3DObj::ClearTempMenus()
{
	if(m_visualItems)
		m_visualItems->ClearTempObjects();

	if(m_clickActions)
		m_clickActions->ClearTempObjects();

	if(m_iconDynamicOverlays)
		m_iconDynamicOverlays->ClearTempIcons();

	m_lockedForChildMenuResponse = FALSE;
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CMenu3DObj::MergeMenu(CMenu3DObj * menuToMerge,LPDIRECT3DDEVICE9 g_pd3dDevice,BOOL tempMerge)
{
     if(!m_visualItems)
		 return;

	 if(!menuToMerge)
		 return;

	 if(!menuToMerge->m_visualItems)
		 return;

	 int currentVisItems = (int)m_visualItems->GetCount();
	 int clickActionCountCurrent = 0;
	 if(m_clickActions)
	    clickActionCountCurrent = (int)m_clickActions->GetCount();

     POSITION posLast;
	 for(POSITION posLoc = menuToMerge->m_visualItems->GetHeadPosition();(posLast = posLoc) != NULL;)
	 {
         CMenuVisObj*visPtr = (CMenuVisObj*)menuToMerge->m_visualItems->GetNext(posLoc);
         /*CMenuVisObj*newCopy = NULL;
		 visPtr->Clone(&newCopy,g_pd3dDevice);*/
         if(tempMerge)
            visPtr->m_tempObject = TRUE;
		 if(visPtr->m_boundToClickZoneStatus != -1)
		 visPtr->m_boundToClickZoneStatus += clickActionCountCurrent;
		 m_visualItems->AddTail(visPtr);
		 menuToMerge->m_visualItems->RemoveAt(posLast);
	 }

	 if(!m_clickActions)
		 return;

	 for(POSITION posLoc = menuToMerge->m_clickActions->GetHeadPosition();(posLast = posLoc) != NULL;)
	 {
         CCLickZoneObj*visPtr = (CCLickZoneObj*)menuToMerge->m_clickActions->GetNext(posLoc);
         /*CCLickZoneObj*newCopy = NULL;
		 visPtr->Clone(&newCopy);*/
		 if(tempMerge)
            visPtr->m_tempObject = TRUE;

		 visPtr->m_visualIndex += currentVisItems;
		 m_clickActions->AddTail(visPtr);
		 menuToMerge->m_clickActions->RemoveAt(posLast);
	 }

}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CMenu3DObj::SafeDelete()
{
   if(m_visualItems)
   {
      m_visualItems->SafeDelete();
	  delete m_visualItems;
	  m_visualItems=0;
   }

   if(m_cursor){
      m_cursor->SafeDelete();
	  delete m_cursor;
      m_cursor=0;
   }

   if(m_clickActions){
      m_clickActions->SafeDelete();
	  delete m_clickActions;
	  m_clickActions=0;
   }

   if(m_iconDynamicOverlays)
   {
      m_iconDynamicOverlays->SafeDelete();
	  delete m_iconDynamicOverlays;
	  m_iconDynamicOverlays = 0;
   }
   if(m_menuMergeList)
   {
      m_menuMergeList->SafeDelete();
	  delete m_menuMergeList;
	  m_menuMergeList = 0;

   }
   if(m_additionalCursors)
	{
       delete m_additionalCursors;
	   m_additionalCursors = 0;
	}
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
BOOL CMenu3DObj::Update(long currentTime,
						ButtonState *leftButtonState,
						D3DVECTOR & unifiedCursorPosition)
{   
 long timePassed = currentTime - m_timeStamp;
 
 if(timePassed > 1000)
	m_ready = TRUE;
 //click/select updates
 if(m_clickActions != NULL && m_cursor != NULL){//click actions exist
    m_clickActions->UpdateAll(unifiedCursorPosition, leftButtonState, m_lockedForChildMenuResponse);
 }//end click actions exist
 //visual updates
 for(POSITION posLoc = m_visualItems->GetHeadPosition();posLoc != NULL;)
 {
     CMenuVisObj*visPtr = (CMenuVisObj*)m_visualItems->GetNext(posLoc);
     
	 if(timePassed > visPtr->m_transitionTime)
		timePassed = visPtr->m_transitionTime;
	 //position modifier
	 visPtr->m_lockLocation = visPtr->GL_matrixFunctions.InterpolatePoints(visPtr->m_startLocation,
		                                                                   visPtr->m_endLocation,
												                           visPtr->m_transitionTime,
												                           timePassed);

	 if(m_closing == TRUE){//override close
     long closeTimepassed = currentTime - m_timeStampWhenClosed;
	 if(closeTimepassed > m_closeDuration)
		closeTimepassed = m_closeDuration;
	 visPtr->m_lockLocation = visPtr->GL_matrixFunctions.InterpolatePoints(visPtr->m_endLocation,
		                                                                   visPtr->m_startLocation,
												                           m_closeDuration,
												                           closeTimepassed);
	 }//end override close
	 //rotation modifier
	 if(visPtr->m_rotationsPerSecond != 0.0f){//valid rotation
	 float lengthRotations = (float)((float)visPtr->m_rotationsPerSecond * 360.0f);
     int rotationalTimePassed = currentTime - visPtr->m_rotationTimeStamp;
	  if(rotationalTimePassed > visPtr->m_rotationDuration){
        rotationalTimePassed = visPtr->m_rotationDuration;
        visPtr->m_rotationTimeStamp = currentTime;
	  }
     
     visPtr->m_currentRotation = visPtr->GL_matrixFunctions.InterpolateFloats(0.0f,lengthRotations,visPtr->m_rotationDuration,rotationalTimePassed);
     }//valid rotation
	 //------------------------------------//
	 //skeletal animation basis index based//
     if(visPtr->m_skeletonObject){//valid skeletal pointer
	   if(visPtr->m_skeletonObject->m_durationCurrentLength != 0){
		if((currentTime-visPtr->m_skeletonObject->m_lastDurationUpdateTime)>visPtr->m_skeletonObject->m_durationCurrentLength){
			visPtr->m_skeletonObject->SetCurrentAnimation(visPtr->m_currentAnimation,100);	
		}
	   }
       else
       visPtr->m_skeletonObject->SetCurrentAnimation(visPtr->m_currentAnimation,100);	
     }//end valid skeletal pointer
 }

return TRUE;
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CMenu3DObj::ChangeNextTextField()
{
 if(!m_visualItems)
	   return;

 if((GetTickCount()-m_lastChangeStateTimeStamp) < 500)
	 return;

   BOOL foundNextItem = FALSE;
   BOOL foundCurrent = FALSE;
   for(POSITION posLoc = m_visualItems->GetHeadPosition();posLoc != NULL;)
   {
       CMenuVisObj * visPtr = (CMenuVisObj *)m_visualItems->GetNext(posLoc);
	   
	   if(visPtr->m_editable)
	   if(foundCurrent)
	   {
          visPtr->m_currentlyBiengEdited = TRUE;
		  //visPtr->m_editable = TRUE;
          foundNextItem = TRUE;
		  m_lastChangeStateTimeStamp = GetTickCount();
		  return;

	   }
	   
	   
	   if(visPtr->m_currentlyBiengEdited)
	   {


          visPtr->m_currentlyBiengEdited = FALSE;
		  //visPtr->m_editable = FALSE;
          foundCurrent = TRUE;
	   }
   
	   
   }


   if(!foundNextItem)
   for(POSITION posLoc = m_visualItems->GetHeadPosition();posLoc != NULL;)
   {
       CMenuVisObj * visPtr = (CMenuVisObj *)m_visualItems->GetNext(posLoc);
	   if(visPtr->m_editable)
	   {
		  visPtr->m_currentlyBiengEdited = TRUE;
		  //visPtr->m_editable = TRUE;
		  m_lastChangeStateTimeStamp = GetTickCount();
		  return;
	   }
	   
   }

   m_lastChangeStateTimeStamp = GetTickCount();
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CMenu3DObj::ScaleOffset(D3DVECTOR offsetPos,float scale)
{
    this->m_scale = scale;



     if(m_clickActions)
	 for(POSITION posActions = m_clickActions->GetHeadPosition();posActions != NULL;)
	 {
		 CCLickZoneObj * cZonePtr = (CCLickZoneObj *)m_clickActions->GetNext(posActions);
		 cZonePtr->m_center.x = cZonePtr->m_center.x * scale;
		 cZonePtr->m_center.y = cZonePtr->m_center.y * scale;
		 cZonePtr->m_center.x = cZonePtr->m_center.x + offsetPos.x;
		 cZonePtr->m_center.y = cZonePtr->m_center.y + offsetPos.y;
         
		 cZonePtr->m_radius = cZonePtr->m_radius * scale;
		 cZonePtr->m_rectMaxX = cZonePtr->m_rectMaxX * scale;
		 cZonePtr->m_rectMaxY = cZonePtr->m_rectMaxY * scale;
		 cZonePtr->m_rectMinX = cZonePtr->m_rectMinX * scale;
		 cZonePtr->m_rectMinY = cZonePtr->m_rectMinY * scale;
		 cZonePtr->m_rectMaxX = cZonePtr->m_rectMaxX + offsetPos.x;
		 cZonePtr->m_rectMaxY = cZonePtr->m_rectMaxY + offsetPos.y;
		 cZonePtr->m_rectMinX = cZonePtr->m_rectMinX + offsetPos.x;
		 cZonePtr->m_rectMinY = cZonePtr->m_rectMinY + offsetPos.y;
		 
	 }
	 for(POSITION posLoc = m_visualItems->GetHeadPosition();posLoc != NULL;)
	 {
         CMenuVisObj * visual = (CMenuVisObj *)m_visualItems->GetNext(posLoc);
		 //scalar/transform
		 if(visual->m_menuBarObj)
		 {
		    visual->m_menuBarObj->m_xRoot = visual->m_menuBarObj->m_xRoot * scale;
			visual->m_menuBarObj->m_yRoot = visual->m_menuBarObj->m_yRoot * scale;
            visual->m_menuBarObj->m_xRoot = visual->m_menuBarObj->m_xRoot + offsetPos.x;
			visual->m_menuBarObj->m_yRoot = visual->m_menuBarObj->m_yRoot + offsetPos.y;
			visual->m_menuBarObj->m_barWidth = visual->m_menuBarObj->m_barWidth * scale;
			visual->m_menuBarObj->m_barLength = visual->m_menuBarObj->m_barLength * scale;
		 }
		 visual->m_scaleFactor = visual->m_scaleFactor * scale;
		 visual->m_fontSizeX = visual->m_fontSizeX * scale;
		 visual->m_fontSizeY = visual->m_fontSizeY * scale;
		 visual->m_endLocation.x = visual->m_endLocation.x * scale;
         visual->m_endLocation.y = visual->m_endLocation.y * scale;
		 visual->m_endLocation.x = visual->m_endLocation.x + offsetPos.x;
         visual->m_endLocation.y = visual->m_endLocation.y + offsetPos.y;
		 
		 //visual->m_endLocation.z = visual->m_endLocation.z * scale;
		 visual->m_lockLocation.x = visual->m_lockLocation.x * scale;
         visual->m_lockLocation.y = visual->m_lockLocation.y * scale;
		 visual->m_lockLocation.x = visual->m_lockLocation.x + offsetPos.x;
         visual->m_lockLocation.y = visual->m_lockLocation.y + offsetPos.y;
		 
		 //visual->m_lockLocation.z = visual->m_lockLocation.z * scale;
		 visual->m_startLocation.x = visual->m_startLocation.x * scale;
		 visual->m_startLocation.y = visual->m_startLocation.y * scale;
		 visual->m_startLocation.x = visual->m_startLocation.x + offsetPos.x;
         visual->m_startLocation.y = visual->m_startLocation.y + offsetPos.y;
		 
		 //visual->m_startLocation.z = visual->m_startLocation.z * scale;
		 if(visual->m_sliderSystem)
		 {
			 visual->m_sliderSystem->m_boxClickSize = visual->m_sliderSystem->m_boxClickSize * scale;
			 visual->m_sliderSystem->m_center.x = visual->m_sliderSystem->m_center.x * scale;
			 visual->m_sliderSystem->m_center.y = visual->m_sliderSystem->m_center.y * scale;
			 visual->m_sliderSystem->m_center.x = visual->m_sliderSystem->m_center.x + offsetPos.x;
			 visual->m_sliderSystem->m_center.y = visual->m_sliderSystem->m_center.y + offsetPos.y;
	         
			 //visual->m_sliderSystem->m_center.z = visual->m_sliderSystem->m_center.z * scale;
			 visual->m_sliderSystem->m_locStart.x = visual->m_sliderSystem->m_locStart.x * scale;
			 visual->m_sliderSystem->m_locStart.y = visual->m_sliderSystem->m_locStart.y * scale;
			 visual->m_sliderSystem->m_locStart.x = visual->m_sliderSystem->m_locStart.x + offsetPos.x;
			 visual->m_sliderSystem->m_locStart.y = visual->m_sliderSystem->m_locStart.y + offsetPos.y;
	         
			 //visual->m_sliderSystem->m_locStart.z = visual->m_sliderSystem->m_locStart.z * scale;
			 visual->m_sliderSystem->m_locEnd.x = visual->m_sliderSystem->m_locEnd.x * scale;
			 visual->m_sliderSystem->m_locEnd.y = visual->m_sliderSystem->m_locEnd.y * scale;
			 visual->m_sliderSystem->m_locEnd.x = visual->m_sliderSystem->m_locEnd.x + offsetPos.x;
			 visual->m_sliderSystem->m_locEnd.y = visual->m_sliderSystem->m_locEnd.y + offsetPos.y;
	         
			 //visual->m_sliderSystem->m_locEnd.z = visual->m_sliderSystem->m_locEnd.z * scale;
			 visual->m_sliderSystem->m_current.x = visual->m_sliderSystem->m_current.x * scale;
			 visual->m_sliderSystem->m_current.y = visual->m_sliderSystem->m_current.y * scale;
			 visual->m_sliderSystem->m_current.x = visual->m_sliderSystem->m_current.x + offsetPos.x;
			 visual->m_sliderSystem->m_current.y = visual->m_sliderSystem->m_current.y + offsetPos.y;
	         
			 //visual->m_sliderSystem->m_current.z = visual->m_sliderSystem->m_current.z * scale;
	         visual->m_sliderSystem->m_visOffsetX = visual->m_sliderSystem->m_visOffsetX * scale;
	         visual->m_sliderSystem->m_visSpacing = visual->m_sliderSystem->m_visSpacing * scale;
	         visual->m_sliderSystem->m_clickWidth = visual->m_sliderSystem->m_clickWidth * scale;
	         visual->m_sliderSystem->m_fontSizeX = visual->m_sliderSystem->m_fontSizeX * scale;
	         visual->m_sliderSystem->m_fontSizeY = visual->m_sliderSystem->m_fontSizeY * scale;
		 }
		 //end scalar/transform
	 }
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CMenu3DObj::ScaleOffset(D3DVECTOR offsetPos,float scaleX,float scaleY)
{
    this->m_scale = scaleX;



     if(m_clickActions)
	 for(POSITION posActions = m_clickActions->GetHeadPosition();posActions != NULL;)
	 {
		 CCLickZoneObj * cZonePtr = (CCLickZoneObj *)m_clickActions->GetNext(posActions);
		 cZonePtr->m_center.x = cZonePtr->m_center.x * scaleX;
		 cZonePtr->m_center.y = cZonePtr->m_center.y * scaleY;
		 cZonePtr->m_center.x = cZonePtr->m_center.x + offsetPos.x;
		 cZonePtr->m_center.y = cZonePtr->m_center.y + offsetPos.y;
         
		 cZonePtr->m_radius = cZonePtr->m_radius * scaleX;
		 cZonePtr->m_rectMaxX = cZonePtr->m_rectMaxX * scaleX;
		 cZonePtr->m_rectMaxY = cZonePtr->m_rectMaxY * scaleY;
		 cZonePtr->m_rectMinX = cZonePtr->m_rectMinX * scaleX;
		 cZonePtr->m_rectMinY = cZonePtr->m_rectMinY * scaleY;
		 cZonePtr->m_rectMaxX = cZonePtr->m_rectMaxX + offsetPos.x;
		 cZonePtr->m_rectMaxY = cZonePtr->m_rectMaxY + offsetPos.y;
		 cZonePtr->m_rectMinX = cZonePtr->m_rectMinX + offsetPos.x;
		 cZonePtr->m_rectMinY = cZonePtr->m_rectMinY + offsetPos.y;
		 
	 }
	 for(POSITION posLoc = m_visualItems->GetHeadPosition();posLoc != NULL;)
	 {
         CMenuVisObj * visual = (CMenuVisObj *)m_visualItems->GetNext(posLoc);
		 //scalar/transform
		 if(visual->m_menuBarObj)
		 {
		    visual->m_menuBarObj->m_xRoot = visual->m_menuBarObj->m_xRoot * scaleX;
			visual->m_menuBarObj->m_yRoot = visual->m_menuBarObj->m_yRoot * scaleY;
            visual->m_menuBarObj->m_xRoot = visual->m_menuBarObj->m_xRoot + offsetPos.x;
			visual->m_menuBarObj->m_yRoot = visual->m_menuBarObj->m_yRoot + offsetPos.y;
			visual->m_menuBarObj->m_barWidth = visual->m_menuBarObj->m_barWidth * scaleY;
			visual->m_menuBarObj->m_barLength = visual->m_menuBarObj->m_barLength * scaleX;
		 }
		 visual->m_scaleFactor = visual->m_scaleFactor * scaleX;
		 visual->m_fontSizeX = visual->m_fontSizeX * scaleX;
		 visual->m_fontSizeY = visual->m_fontSizeY * scaleY;
		 visual->m_endLocation.x = visual->m_endLocation.x * scaleX;
         visual->m_endLocation.y = visual->m_endLocation.y * scaleY;
		 visual->m_endLocation.x = visual->m_endLocation.x + offsetPos.x;
         visual->m_endLocation.y = visual->m_endLocation.y + offsetPos.y;
		 
		 //visual->m_endLocation.z = visual->m_endLocation.z * scale;
		 visual->m_lockLocation.x = visual->m_lockLocation.x * scaleX;
         visual->m_lockLocation.y = visual->m_lockLocation.y * scaleY;
		 visual->m_lockLocation.x = visual->m_lockLocation.x + offsetPos.x;
         visual->m_lockLocation.y = visual->m_lockLocation.y + offsetPos.y;
		 
		 //visual->m_lockLocation.z = visual->m_lockLocation.z * scale;
		 visual->m_startLocation.x = visual->m_startLocation.x * scaleX;
		 visual->m_startLocation.y = visual->m_startLocation.y * scaleY;
		 visual->m_startLocation.x = visual->m_startLocation.x + offsetPos.x;
         visual->m_startLocation.y = visual->m_startLocation.y + offsetPos.y;
		 
		 //visual->m_startLocation.z = visual->m_startLocation.z * scale;
		 if(visual->m_sliderSystem)
		 {
			 visual->m_sliderSystem->m_boxClickSize = visual->m_sliderSystem->m_boxClickSize * scaleX;
			 visual->m_sliderSystem->m_center.x = visual->m_sliderSystem->m_center.x * scaleX;
			 visual->m_sliderSystem->m_center.y = visual->m_sliderSystem->m_center.y * scaleY;
			 visual->m_sliderSystem->m_center.x = visual->m_sliderSystem->m_center.x + offsetPos.x;
			 visual->m_sliderSystem->m_center.y = visual->m_sliderSystem->m_center.y + offsetPos.y;
	         
			 //visual->m_sliderSystem->m_center.z = visual->m_sliderSystem->m_center.z * scale;
			 visual->m_sliderSystem->m_locStart.x = visual->m_sliderSystem->m_locStart.x * scaleX;
			 visual->m_sliderSystem->m_locStart.y = visual->m_sliderSystem->m_locStart.y * scaleY;
			 visual->m_sliderSystem->m_locStart.x = visual->m_sliderSystem->m_locStart.x + offsetPos.x;
			 visual->m_sliderSystem->m_locStart.y = visual->m_sliderSystem->m_locStart.y + offsetPos.y;
	         
			 //visual->m_sliderSystem->m_locStart.z = visual->m_sliderSystem->m_locStart.z * scale;
			 visual->m_sliderSystem->m_locEnd.x = visual->m_sliderSystem->m_locEnd.x * scaleX;
			 visual->m_sliderSystem->m_locEnd.y = visual->m_sliderSystem->m_locEnd.y * scaleY;
			 visual->m_sliderSystem->m_locEnd.x = visual->m_sliderSystem->m_locEnd.x + offsetPos.x;
			 visual->m_sliderSystem->m_locEnd.y = visual->m_sliderSystem->m_locEnd.y + offsetPos.y;
	         
			 //visual->m_sliderSystem->m_locEnd.z = visual->m_sliderSystem->m_locEnd.z * scale;
			 visual->m_sliderSystem->m_current.x = visual->m_sliderSystem->m_current.x * scaleX;
			 visual->m_sliderSystem->m_current.y = visual->m_sliderSystem->m_current.y * scaleY;
			 visual->m_sliderSystem->m_current.x = visual->m_sliderSystem->m_current.x + offsetPos.x;
			 visual->m_sliderSystem->m_current.y = visual->m_sliderSystem->m_current.y + offsetPos.y;
	         
			 //visual->m_sliderSystem->m_current.z = visual->m_sliderSystem->m_current.z * scale;
	         visual->m_sliderSystem->m_visOffsetX = visual->m_sliderSystem->m_visOffsetX * scaleX;
	         visual->m_sliderSystem->m_visSpacing = visual->m_sliderSystem->m_visSpacing * scaleX;
	         visual->m_sliderSystem->m_clickWidth = visual->m_sliderSystem->m_clickWidth * scaleX;
	         visual->m_sliderSystem->m_fontSizeX = visual->m_sliderSystem->m_fontSizeX * scaleX;
	         visual->m_sliderSystem->m_fontSizeY = visual->m_sliderSystem->m_fontSizeY * scaleY;
		 }
		 //end scalar/transform
	 }
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CMenu3DObj::Serialize( CArchive& ar )
{
BOOL reserved = 0;

CObject::Serialize( ar );
	if(ar.IsStoring() ){	

       ar.SetObjectSchema(2);
       ASSERT(ar.GetObjectSchema() == 2);

	   ar << m_menuName 
		  << m_menuAccessID
		  << m_visualItems
		  << m_cursor
		  << m_closeDuration
		  << m_clickActions
		  << m_instantClose
		  << m_lockControls
		  << m_triggerSoundtrack
		  << m_closeOnEsc
		  << m_specialOperation
		  << m_specialOperationMiscInt
		  << m_offsetMenu.x
	      << m_offsetMenu.y
	      << m_offsetMenu.z
          << m_scale
	      << m_menuMergeList
		  << m_resolutionBased
		  << m_centerMenu
		  << m_restrictChat;

	   ar << m_additionalCursors;
	   
	}
	else
	{
       int version = ar.GetObjectSchema();

       ar >> m_menuName 
		  >> m_menuAccessID
		  >> m_visualItems
		  >> m_cursor
		  >> m_closeDuration
		  >> m_clickActions
		  >> m_instantClose
		  >> m_lockControls
		  >> m_triggerSoundtrack
		  >> m_closeOnEsc
		  >> m_specialOperation
		  >> m_specialOperationMiscInt
		  >> m_offsetMenu.x
	      >> m_offsetMenu.y
	      >> m_offsetMenu.z
          >> m_scale
	      >> m_menuMergeList
		  >> m_resolutionBased
		  >> m_centerMenu
		  >> m_restrictChat;

	   if(version > 1)
	   ar >> m_additionalCursors;


	   //m_restrictChat = FALSE;
  

	   m_iconDynamicOverlays = NULL;
	   m_closing             = FALSE;
	   m_timeStamp           = 0;
	   m_timeStampWhenClosed = 0;
	   m_ready               = FALSE;
	   m_passedInVar           = 0;
	   m_passedInCommand       = 0;  
	   //m_instantClose        = TRUE;
	   //m_lockControls        = TRUE;
	   m_captureControlQuerry  = FALSE;
       m_controlActionListID   = -1;
	   m_controlInvID          = -1;
	   m_controlStatusID       = 0;

	   m_lockedForChildMenuResponse = 0;
	   /*m_offsetMenu.x          = 0.0f;
	   m_offsetMenu.y          = 0.0f;
	   m_offsetMenu.z          = 0.0f;
       m_scale                 = 1.0f;
	   m_menuMergeList         = NULL;*/
	   //m_resolutionBased       = FALSE;
	   m_tempMenuMiscInt       = 0;
	   m_tempMenuMiscInt2      = 0;
	   m_tempMenuMiscInt3      = 0;
	   m_tempMenuAction        = 0;
	   //m_centerMenu            = 0;
	   m_clearTempMenus        = 0;


	   m_macroRepeatCount         = 1;
	   m_macroMaxLimit            = 1000;
	   m_macroActionType          = 0;
	   m_macroCurrent             = 0;
	   m_macroManditoryWait       = 10000;
	   m_macroLastActionTimeStamp = GetTickCount();
	   m_macroRunning             = FALSE;
	   m_macroStartStamp          = m_macroLastActionTimeStamp;
	}
} 
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
CMenu3DObj * CMenu3DObjList::GetByID(int id)
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CMenu3DObj * visObj = (CMenu3DObj *)GetNext(posLoc);
		if(visObj->m_menuAccessID == id)
		   return visObj;
        
	 }
	return NULL;
}
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
//---------------------------------------------------------------------//
void CMenu3DObjList::Clone(CMenu3DObjList ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     CMenu3DObjList * copyLocal = new CMenu3DObjList();
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CMenu3DObj * visObj = (CMenu3DObj *)GetNext(posLoc);
        CMenu3DObj * clonedPtr = NULL; 
		visObj->Clone(&clonedPtr,g_pd3dDevice);
		copyLocal->AddTail(clonedPtr);
	 }
	 *copy = copyLocal;
}

//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CMenu3DObjList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CMenu3DObj * menuObj = (CMenu3DObj *)GetNext(posLoc);
		menuObj->SafeDelete();
        delete menuObj;
		menuObj=0;
	}
	RemoveAll();
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
BOOL CMenu3DObjList::CloseByID(int id)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CMenu3DObj * menuObj = (CMenu3DObj *)GetNext(posLoc);
		if(menuObj->m_menuAccessID == id){
           menuObj->CloseMenu(NULL);
		   return TRUE;
		}
	}
	return FALSE;
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
int CMenu3DObjList::GetFreeID()
{
 int newID = 200;
 BOOL completed = FALSE;
  while(!completed)
  {
    completed = TRUE;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CMenu3DObj * menuObj = (CMenu3DObj *)GetNext(posLoc);
		if(menuObj->m_menuAccessID == newID){
		   newID++;
		   completed = FALSE;
           break;
		}
	}
  }
  return newID;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
CMenuVisObj * CMenu3DObjList::GetVisualByName(CString visualName)
{
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {//loop through menus
      CMenu3DObj* menu = (CMenu3DObj*)GetNext(posLoc);
	  if(menu->m_visualItems)
		 for(POSITION visPos = menu->m_visualItems->GetHeadPosition();visPos != NULL;)
		 {//visual items loop
             CMenuVisObj * visPtr = (CMenuVisObj *)menu->m_visualItems->GetNext(visPos);
			 if(visPtr->m_visualName.CompareNoCase(visualName)==0)
			    return visPtr;
		 }
   }
   return NULL;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CMenu3DObjList::AddStringToMenuList(int controlID,
									     CString string,
									     int visualRefIndex,
									     int miscInt,
									     int miscInt2,
									     CString regItemName,
									     int mode,
									     CString costExt)
{
    return AddStringToMenuList( controlID,
							    string,
							    visualRefIndex,
							    miscInt,
							    miscInt2,
							    regItemName,
						        mode,
						        costExt,
								0,
								0);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CMenu3DObjList::AddStringToMenuList(int controlID,
									     CString string,
									     int visualRefIndex,
									     int miscInt,
									     int miscInt2,
									     CString regItemName,
									     int mode,
									     CString costExt,
										 int miscInt3,
										 int miscInt4)
{
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {//loop through menus
      CMenu3DObj* menu = (CMenu3DObj*)GetNext(posLoc);
	  if(menu->m_visualItems)
		 for(POSITION visPos = menu->m_visualItems->GetHeadPosition();visPos != NULL;)
		 {//visual items loop
             CMenuVisObj * visPtr = (CMenuVisObj *)menu->m_visualItems->GetNext(visPos);
			 if(visPtr->m_sliderSystem)
			 {//valid slider system
			    
                if(visPtr->m_sliderSystem->m_controlID == controlID)
				{//correct control ID
					visPtr->m_sliderSystem->m_mode = mode;
                    if(visPtr->m_sliderSystem->m_mode == 0)
                       visPtr->m_sliderSystem->AddString(string,visualRefIndex,miscInt,miscInt2,0,0);
					else{
                       visPtr->m_sliderSystem->AddString((" " + regItemName),visualRefIndex,miscInt,miscInt2,0,0);
					   if(miscInt2 > 0)
					      visPtr->m_sliderSystem->AddString(costExt + "   Quantity " + CABLIB.ConvToString(miscInt2),visualRefIndex,miscInt,miscInt2,0,0);
					   else
						  visPtr->m_sliderSystem->AddString(costExt,visualRefIndex,miscInt,miscInt2,0,0);
					}
					return TRUE;
				}//end correct control ID
			 }//end valid slider system
		 }//end visual items loop
   }//end loop through menus
   return FALSE;
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
CMenu3DObj * CMenu3DObjList::SetTextureToObjectByName(  CString & visualName,
									                    CString & texturename,
												        int textureLevel)
{
   if(textureLevel > 9)
      return FALSE;

   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {//loop through menus
      CMenu3DObj* menu = (CMenu3DObj*)GetNext(posLoc);
	  if(menu->m_visualItems)
		 for(POSITION visPos = menu->m_visualItems->GetHeadPosition();visPos != NULL;)
		 {//visual items loop
             CMenuVisObj * visPtr = (CMenuVisObj *)menu->m_visualItems->GetNext(visPos);
             if(visPtr->m_visualName.CompareNoCase(visualName)!=0)
			    continue;

			 if(visPtr->m_skeletonObject)
			 {//valid skeletal system
                for(POSITION posLocal = visPtr->m_skeletonObject->m_bonesList->GetHeadPosition();posLocal != NULL;)
				{
	                CBoneObject * bonePtr = (CBoneObject *)visPtr->m_skeletonObject->m_bonesList->GetNext(posLocal);
	                if(bonePtr->m_hiarchyMesh)
                    for(POSITION posOni = bonePtr->m_hiarchyMesh->m_lodChain->GetHeadPosition();posOni != NULL;)
		            {
                      CHiarcleVisObj * hObj = (CHiarcleVisObj *) bonePtr->m_hiarchyMesh->m_lodChain->GetNext(posOni);
				      hObj->m_mesh->List_storedTextureFromFile[textureLevel] = texturename;
		            }
				} 
                //deformable cfg
                if(visPtr->m_skeletonObject->m_alternateConfigurations)
                for(POSITION posLoc = visPtr->m_skeletonObject->m_alternateConfigurations->GetHeadPosition();posLoc != NULL;)
                {
					CAlterUnitDBObject * aPtr = (CAlterUnitDBObject *)visPtr->m_skeletonObject->m_alternateConfigurations->GetNext(posLoc);
					if(aPtr->m_configurations)
					for(POSITION cfgPos = aPtr->m_configurations->GetHeadPosition();cfgPos != NULL;)
					{
						CDeformableMesh * dMesh = (CDeformableMesh *)aPtr->m_configurations->GetNext(cfgPos);
						for(POSITION lPos = dMesh->m_lodChain->GetHeadPosition();lPos != NULL;)
						{
							CDeformableVisObj * vObj = (CDeformableVisObj *) dMesh->m_lodChain->GetNext(lPos);
						    vObj->m_mesh->List_storedTextureFromFile[textureLevel] = texturename;
						}
					}
                }	
			 }//end valid skeletal system
			 return menu;
		 }//end visual items loop
   }//end loop through menus
   return NULL;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
CMenu3DObj * CMenu3DObjList::AddStringToMenuListDistributed(CString visualName,
															CString textdata,
															float dxRenderWidth,
															float cgFontSizeX,
															void * symbolMapMem,
															float lineLength)
{
	          CSymbolMapperObjList * symbolMap = (CSymbolMapperObjList *)symbolMapMem;

              CMenuVisObj * visObject = this->GetVisualByName(visualName);
			  if(!visObject)
			      return NULL;
              
			  if(!visObject->m_sliderSystem)
			      return NULL;

			  int maxLineLength = 35;
			  int tracerBuild = 0;
			  int miscBlockSize = textdata.GetLength();
			  char * miscUpdateBlock = textdata.GetBuffer();
			  float lineLengthMeasured = 0.0f;
			  float maxLineLengthMeasured = lineLength;//.3
			  if(dxRenderWidth > 1000)
			  {
                 maxLineLengthMeasured = maxLineLengthMeasured * .8f;
			  }
			  float characterSpacing = cgFontSizeX*2.0f;
			  CString tempCS = "";
              while(tracerBuild < miscBlockSize)
	          {
				  CString curBuild = "";
                  lineLengthMeasured = 0.0f;
                  char additionTemp;
                  for(int loop=0;loop<20000;loop++)
		          {
					  if(tracerBuild >= miscBlockSize)
					     break;

					  additionTemp = miscUpdateBlock[tracerBuild];
					  curBuild = curBuild + additionTemp;
					  tempCS = additionTemp;
                      CSymbolMapperObj * tempref = symbolMap->GetByID(tempCS);
					  if(!tempref)
					  {
						  tracerBuild++;
					      continue;
					  }
					  lineLengthMeasured += characterSpacing * tempref->m_percentageX;

			          tracerBuild++;
					  if(lineLengthMeasured > maxLineLengthMeasured)
					  {
					    //tracerBuild--;
                        for(int j=0;j<tracerBuild;j++)
						{
							
							additionTemp = miscUpdateBlock[tracerBuild];
							if(additionTemp == ' ')
							   break;

                            tracerBuild--;
							curBuild.Delete(curBuild.GetLength()-1,1);
						}
					    break;
					  }
		          }   
			      visObject->m_sliderSystem->AddString(curBuild,-1,0,0,0,0);
	          }
			  return NULL;
}

//----------------------------------------------------------------//
//----------------------------------------------------------------//
//----------------------------------------------------------------//
void CMenu3DObjList::LoadTexturesForMenu(CMenu3DObj * menu3DObj,
										 CTextureDBObjList * textureDataBase,
										 CTextureDBObjList * dynDataBase,
										 CCommonCtrlList * cntrlDB)
{
      if(menu3DObj->m_cursor)	
	   menu3DObj->m_cursor->m_skeletonObject->SkeletalTextureReassignmentTemp(menu3DObj->m_menuAccessID+5,textureDataBase);
	   //visual list 
	   for(POSITION vPos = menu3DObj->m_visualItems->GetHeadPosition();vPos != NULL;)
	   {
		   CMenuVisObj * vPtr = (CMenuVisObj *)menu3DObj->m_visualItems->GetNext(vPos);
		   
		   if(vPtr->m_skeletonObject)
			  vPtr->m_skeletonObject->SkeletalTextureReassignmentTemp(menu3DObj->m_menuAccessID+5,textureDataBase);
		   if(vPtr->m_sliderSystem){
              if(vPtr->m_sliderSystem->m_visStart)
				 vPtr->m_sliderSystem->m_visStart->SkeletalTextureReassignmentTemp(menu3DObj->m_menuAccessID+5,textureDataBase);
              if(vPtr->m_sliderSystem->m_visMov)
				 vPtr->m_sliderSystem->m_visMov->SkeletalTextureReassignmentTemp(menu3DObj->m_menuAccessID+5,textureDataBase);
              if(vPtr->m_sliderSystem->m_visEnd)
				 vPtr->m_sliderSystem->m_visEnd->SkeletalTextureReassignmentTemp(menu3DObj->m_menuAccessID+5,textureDataBase); 
		   }
		   if(vPtr->m_menuBarObj){
              if(vPtr->m_menuBarObj->m_barDynamic)
			     textureDataBase->AddTextureToDatabaseTemp(vPtr->m_menuBarObj->m_barDynamic->List_storedTextureFromFile,//CString * storedTextures,
											               vPtr->m_menuBarObj->m_barDynamic->List_textureSelect,
													       menu3DObj->m_menuAccessID+5);
              if(vPtr->m_menuBarObj->m_barStatic)
				 textureDataBase->AddTextureToDatabaseTemp(vPtr->m_menuBarObj->m_barStatic->List_storedTextureFromFile,//CString * storedTextures,
											               vPtr->m_menuBarObj->m_barStatic->List_textureSelect,
													       menu3DObj->m_menuAccessID+5);
		   }
          
		   if(vPtr->m_type == 25)
		   {
				CCommonCtrlObj * cntrlObj = cntrlDB->GetByIndex(vPtr->m_commCntlType);
				if(cntrlObj)
				{//comm ctrl loop
						if(cntrlObj->m_clickState)
						   cntrlObj->m_clickState->SkeletalTextureReassignmentTemp(menu3DObj->m_menuAccessID+5,textureDataBase);
						if(cntrlObj->m_hoverState)
						   cntrlObj->m_hoverState->SkeletalTextureReassignmentTemp(menu3DObj->m_menuAccessID+5,textureDataBase);
						if(cntrlObj->m_idleState)
						   cntrlObj->m_idleState->SkeletalTextureReassignmentTemp(menu3DObj->m_menuAccessID+5,textureDataBase);
				}//end comm ctrl loop
		   }
	   }
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//---------------------cloases all menus--------------------------------------------------//
void CMenu3DObjList::InListActions(int currentSel,CSliderBarObj * slideBarObj,ControlDBList * ControlsList,CSkillObjectList * m_skillDatabase)
{
	slideBarObj->FlushAll();
	POSITION cntPos = ControlsList->FindIndex(0);
	if(cntPos)
	{//1
		ControlDBObj* cObj = (ControlDBObj*)ControlsList->GetAt(cntPos);
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
				if(slideBarObj->m_mode == 2)
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
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//---------------------cloases all menus--------------------------------------------------//
void CMenu3DObjList::InListActionsPlain(int currentSel,CSliderBarObj * slideBarObj,ControlDBList * ControlsList,CSkillObjectList * m_skillDatabase)
{
	slideBarObj->FlushAll();
     POSITION cntPos = ControlsList->FindIndex(0);
	 if(cntPos)
	 {//1
        ControlDBObj* cObj = (ControlDBObj*)ControlsList->GetAt(cntPos);
        int cntLoop = 0;
        CString controlAction = cObj->List_controlInfoList->ReturnActionByIndex(cntLoop);

	    while(controlAction != "")
	    {
		      int miscTemp1 = 0;
			  int miscTemp2 = 0;
			  int miscTemp3 = 0;
			  int miscTemp4 = 0;
              //controlAction = cObj->List_controlInfoList->ReturnActionByIndex(cntLoop);
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

//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
CCLickZoneObj::CCLickZoneObj()
{
    m_radiusType   = FALSE;//false uses rectangle
    m_radius       = 0.0f;

    m_rectMaxX     = 0.0f;
    m_rectMaxY     = 0.0f;
    m_rectMinX     = 0.0f;
    m_rectMinY     = 0.0f;
	
    m_visualIndex  = 0;
    m_action       = 0;//0=none 1=close 
    m_miscInt      = 0;
    m_miscInt2     = 0;
	m_miscInt3     = 0;
	m_miscString   = "NA";
	m_center.x     = 0.0f;
	m_center.y     = 0.0f;
	m_center.z     = 0.0f;
    m_state               = 0;//0=up 1=down 2=over
	m_execOnCloseing      = FALSE;
	m_validForExec        = FALSE;
	m_previouslyActuated  = FALSE;
	m_switchOverride      = FALSE;
	m_switchOverrideState = 0;
	m_dropZone            = FALSE;
	m_tempObject          = 0;
	m_default             = 0;
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
BOOL CCLickZoneObj::Update(D3DVECTOR cursorPosition, ButtonState *leftButtonState)//,DIMOUSESTATE mouseState)
{
    if(m_action == 0)
	   return FALSE;

	m_state=0;
	if(m_radiusType)
	{//radius type
		if(GL_matrixFunctions.BoundingSphereCheck(	m_radius,0.0f,
													m_center.x,
													m_center.y,
													0.0f,
													cursorPosition.x,
													cursorPosition.y,
													0.0f))
		{
			m_validForExec = TRUE;
			m_state = 1;
			return TRUE;
		}
	}//end radius type
	else
	{//rectangle type
		if(cursorPosition.x < m_rectMaxX &&
		   cursorPosition.x > m_rectMinX &&
		   cursorPosition.y < m_rectMaxY &&
		   cursorPosition.y > m_rectMinY)
		{
			m_validForExec = TRUE;
			m_state = 1;
			return TRUE;
		}
	}//end rectangle type

	return FALSE;//return true if somthing needs to happen
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
BOOL CCLickZoneObj::UpdateCurrentlyPressedState(D3DVECTOR cursorPosition, ButtonState *leftButtonState)
{
    if(m_action == 0)
	   return FALSE;

	m_state = 0;
    if(m_radiusType)
	{//radius type
		if(GL_matrixFunctions.BoundingSphereCheck(	m_radius,
													0.0f, 
													m_center.x,
													m_center.y,
													0.0f,
													cursorPosition.x,
													cursorPosition.y,
													0.0f))
		{
			m_state = 3;
			return TRUE;
		}
	}//end radius type
	else
	{//rectangle type
		if(cursorPosition.x < m_rectMaxX &&
		   cursorPosition.x > m_rectMinX &&
		   cursorPosition.y < m_rectMaxY &&
		   cursorPosition.y > m_rectMinY)
		{
			m_state = 3;
			return TRUE;
		}
	}//end rectangle type

	return FALSE;//return true if somthing needs to happen
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CCLickZoneObj::Clone(CCLickZoneObj ** copy)
{
   CCLickZoneObj * copyLocal = new CCLickZoneObj();
    
   
    copyLocal->m_radiusType     = m_radiusType;//false uses rectangle
    copyLocal->m_radius         = m_radius;
    copyLocal->m_rectMaxX       = m_rectMaxX;
    copyLocal->m_rectMaxY       = m_rectMaxY;
    copyLocal->m_rectMinX       = m_rectMinX;
    copyLocal->m_rectMinY       = m_rectMinY;
    copyLocal->m_visualIndex    = m_visualIndex;
    copyLocal->m_action         = m_action;//0=none 1=close 
    copyLocal->m_miscInt        = m_miscInt;
    copyLocal->m_miscInt2       = m_miscInt2;
	copyLocal->m_center.x       = m_center.x;
	copyLocal->m_center.y       = m_center.y;
	copyLocal->m_center.z       = m_center.z;
    copyLocal->m_state          = m_state;//0=up 1=down 2=over
	copyLocal->m_miscInt3       = m_miscInt3;
	copyLocal->m_miscString     = m_miscString;
	copyLocal->m_execOnCloseing = m_execOnCloseing;
	copyLocal->m_switchOverride = m_switchOverride;
	copyLocal->m_switchOverrideState = m_switchOverrideState;
	copyLocal->m_dropZone       = m_dropZone;
	copyLocal->m_tempObject     = m_tempObject;
	copyLocal->m_default        = m_default;

	*copy = copyLocal;
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CCLickZoneObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_radiusType 
		  << m_radius
		  << m_rectMaxX
		  << m_rectMaxY
		  << m_rectMinX
		  << m_rectMinY
		  << m_visualIndex
		  << m_action
		  << m_miscInt
		  << m_miscInt2
		  << m_state
		  << m_center.x
		  << m_center.y
		  << m_center.z
          << m_miscInt3
          << m_miscString
		  << m_execOnCloseing
		  << m_dropZone
		  << m_default;
	   
	}
	else
	{
       ar >> m_radiusType 
		  >> m_radius
		  >> m_rectMaxX
		  >> m_rectMaxY
		  >> m_rectMinX
		  >> m_rectMinY
		  >> m_visualIndex
		  >> m_action
		  >> m_miscInt
		  >> m_miscInt2
		  >> m_state
		  >> m_center.x
		  >> m_center.y
		  >> m_center.z
          >> m_miscInt3 
          >> m_miscString
		  >> m_execOnCloseing
		  >> m_dropZone
		  >> m_default;

	      ///m_default = 0;
          m_validForExec   = FALSE;
		  m_previouslyActuated = FALSE;
		  m_switchOverride = FALSE;
	      m_switchOverrideState = 0;
		  m_tempObject = 0;
		  //m_dropZone = FALSE;
	}
} 
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
CCLickZoneList::CCLickZoneList()
{
    m_mouseWasPressed = FALSE;
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CCLickZoneList::UpdateAll(D3DVECTOR cursorPosition, ButtonState *leftButtonState, BOOL affectTempOnly)
{
	if(leftButtonState->IsDown())
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
		CCLickZoneObj * cObj = (CCLickZoneObj *)GetNext(posLoc);
		if(cObj->m_tempObject == FALSE && affectTempOnly == TRUE)
			continue;
		cObj->UpdateCurrentlyPressedState(cursorPosition, leftButtonState);//,mouseState);
	}

	if(leftButtonState->JustReleased())
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CCLickZoneObj * cObj = (CCLickZoneObj *)GetNext(posLoc);
        cObj->Update(cursorPosition, leftButtonState);
	}
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CCLickZoneList::Clone(CCLickZoneList ** copy)
{
     CCLickZoneList * copyLocal = new CCLickZoneList();
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CCLickZoneObj * visObj = (CCLickZoneObj *)GetNext(posLoc);
        CCLickZoneObj * clonedPtr = NULL; 
		visObj->Clone(&clonedPtr);
		copyLocal->AddTail(clonedPtr);
	 }
	 *copy = copyLocal;
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
CCLickZoneObj * CCLickZoneList::GetByIndex(int index)
{
	if(index < 0)
	   return NULL;
     
     POSITION posLoc = FindIndex(index);
	 if(!posLoc)
		 return NULL;
        
	 CCLickZoneObj * visObj = (CCLickZoneObj *)GetAt(posLoc);
	 return visObj;
     
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CCLickZoneList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CCLickZoneObj * cObj = (CCLickZoneObj *)GetNext(posLoc);
        delete cObj;
		cObj=0;
	}
	RemoveAll();
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CCLickZoneList::ClearTempObjects()
{
	POSITION posLast = NULL;
    for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	{
        CCLickZoneObj * cObj = (CCLickZoneObj *)GetNext(posLoc);
		if(cObj->m_tempObject)
		{
           delete cObj;
		   cObj=0;
		   RemoveAt(posLast);
		}
	}
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
CMenuMergeObj::CMenuMergeObj()
{
     m_menuID = 0;
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CMenuMergeObj::SafeDelete()
{
 
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CMenuMergeObj::Clone(CMenuMergeObj ** copy)
{
   CMenuMergeObj * copyLocal = new CMenuMergeObj();
    
   copyLocal->m_menuID     = m_menuID;//false uses rectangle
   *copy = copyLocal;
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CMenuMergeObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_menuID;
	   
	}
	else
	{
       ar >> m_menuID;
	}
} 
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CMenuMergeList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CMenuMergeObj * cObj = (CMenuMergeObj *)GetNext(posLoc);
		cObj->SafeDelete();
        delete cObj;
		cObj=0;
	}
	RemoveAll();
}
//--------------------------------------------//
//--------------------------------------------//
//--------------------------------------------//
void CMenuMergeList::Clone(CMenuMergeList ** copy)
{
     CMenuMergeList * copyLocal = new CMenuMergeList();
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CMenuMergeObj * visObj = (CMenuMergeObj *)GetNext(posLoc);
        CMenuMergeObj * clonedPtr = NULL; 
		visObj->Clone(&clonedPtr);
		copyLocal->AddTail(clonedPtr);
	 }
	 *copy = copyLocal;
}


IMPLEMENT_SERIAL(CMenuMergeObj, CObject, 0)
IMPLEMENT_SERIAL(CMenuMergeList, CObList, 0)
IMPLEMENT_SERIAL(CCLickZoneObj, CObject, 0)
IMPLEMENT_SERIAL(CCLickZoneList, CObList, 0)
IMPLEMENT_SERIAL(CMenu3DObj, CObject, VERSIONABLE_SCHEMA|2)
IMPLEMENT_SERIAL(CMenu3DObjList, CObList, 0)
IMPLEMENT_SERIAL(CMenuVisObj, CObject, VERSIONABLE_SCHEMA|2)
IMPLEMENT_SERIAL(CMenuVisObjList, CObList, 0)

