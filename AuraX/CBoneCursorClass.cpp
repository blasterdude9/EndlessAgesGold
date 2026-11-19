#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "direct.h"

#include "CBoneCursorClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBoneCursorObj::CBoneCursorObj()
{
    m_position.x      = 0.0f;
	m_position.y      = 0.0f;
	m_position.z      = 3.0f;
    m_limitX          = 0.6f;
    m_limitY          = 0.45f;
    m_fadeInProcess   = FALSE;
	m_fadingIn        = FALSE;
	m_fadingOut       = FALSE;
	m_fadeTimeStamp   = 0;
	m_fadeDuration    = 1000;
	m_skeletonObject  = NULL;
	m_scaleFactor     = .1f;
	m_mouseButton1pressed = FALSE;
	m_dragging        = FALSE;
	m_dragGlid        = 0;
	m_dragVisualRef   = 0;
	m_dragVisualID    = 0;
	m_dragDupPassed   = 0;
	m_cursorType      = 0;
	m_expEffectOnClick= -1;
	m_useTypeID       = 0;
}

void CBoneCursorObj::Clone(CBoneCursorObj ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     CBoneCursorObj * copyLocal = new CBoneCursorObj();
	 copyLocal->m_position.x      = m_position.x;
	 copyLocal->m_position.y      = m_position.y;
	 copyLocal->m_position.z      = m_position.z;
     copyLocal->m_limitX          = m_limitX;
     copyLocal->m_limitY          = m_limitY;
	 copyLocal->m_fadeTimeStamp   = m_fadeTimeStamp;
	 copyLocal->m_fadeDuration    = m_fadeDuration;
	 copyLocal->m_scaleFactor     = m_scaleFactor;
	 copyLocal->m_dragging        = m_dragging;
	 copyLocal->m_dragGlid        = m_dragGlid;
	 copyLocal->m_dragVisualRef   = m_dragVisualRef;
	 copyLocal->m_dragVisualID    = m_dragVisualID;
	 copyLocal->m_dragDupPassed   = m_dragDupPassed;
	 copyLocal->m_cursorType      = m_cursorType;
	 copyLocal->m_useTypeID       = m_useTypeID;
	 copyLocal->m_expEffectOnClick = m_expEffectOnClick;
   
	 if(m_skeletonObject)
		m_skeletonObject->Clone(&copyLocal->m_skeletonObject,g_pd3dDevice,NULL,1);
     
	 *copy = copyLocal;
}


BOOL CBoneCursorObj::BuildMouse(CString arbFile,
		                        float limitX,
				                float limitY,
			          	        D3DVECTOR startPosition,
								LPDIRECT3DDEVICE9 g_pd3dDevice)
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

	m_position.x  = startPosition.x;
	m_position.y  = startPosition.y;
	m_position.z  = startPosition.z;
     m_limitX      = limitX;
    m_limitY      = limitY;

return TRUE;
}

BOOL CBoneCursorObj::AnimationManagement(long currentTime,DIMOUSESTATE mouseState, float adjust)
{
	if(!m_skeletonObject)
		return FALSE;

	if(adjust > 4.0f)
	   adjust = 4.0f;
    //positional update
    m_position.x += (mouseState.lX*.001f)*adjust;
	m_position.y += -(mouseState.lY*.001f)*adjust;


    if(m_position.x > m_limitX)
	   m_position.x = m_limitX;

	if(m_position.x < -m_limitX)
	   m_position.x = -m_limitX;

    if(m_position.y > m_limitY)
	   m_position.y = m_limitY;

	if(m_position.y < -m_limitY)
	   m_position.y = -m_limitY;

	//end positional update

    int animationSelect = 1;//stand
    m_mouseButton1pressed = FALSE;
    if(mouseState.rgbButtons[0] & 0x80){//mouse button one
       animationSelect = 12;
	   m_mouseButton1pressed = TRUE;
	}//end mouse button one

	if(m_skeletonObject->m_durationCurrentLength != 0){
		if((GetTickCount()-m_skeletonObject->m_lastDurationUpdateTime)>m_skeletonObject->m_durationCurrentLength){
			m_skeletonObject->SetAnimationBasedOnAttrib(animationSelect,FALSE,0,100);	
		}
	}
    else
    m_skeletonObject->SetAnimationBasedOnAttrib(animationSelect,FALSE,0,100);

return FALSE;
}

void CBoneCursorObj::SafeDelete()
{
    if(m_skeletonObject){
	   m_skeletonObject->SafeDelete();
	   delete m_skeletonObject;
       m_skeletonObject=0;
	}
}


void CBoneCursorObj::Serialize( CArchive& ar )
{
	BOOL reserved = 0;
CObject::Serialize( ar );
	if(ar.IsStoring() ){
       ar.SetObjectSchema(2);
	   ASSERT(ar.GetObjectSchema() == 2);
	   ar << m_position.x
	      << m_position.y
	      << m_position.z
          << m_limitX
          << m_limitY
	      << m_cursorType
	      << m_fadeDuration
	      << m_skeletonObject
	      << m_scaleFactor 
	      << m_mouseButton1pressed;

	   ar << m_expEffectOnClick
	      << m_useTypeID
		  << reserved
		  << reserved;
	  
	}
	else
	{
	   int version = ar.GetObjectSchema();
       ar >> m_position.x
	      >> m_position.y
	      >> m_position.z
          >> m_limitX
          >> m_limitY
	      >> m_cursorType
	      >> m_fadeDuration
		  >> m_skeletonObject
	      >> m_scaleFactor 
	      >> m_mouseButton1pressed;	

	   if(version > 1)
	   {
          ar >> m_expEffectOnClick
			 >> m_useTypeID
			 >> reserved
			 >> reserved;

	   }


	    m_fadeTimeStamp       = 0;
	    m_fadeInProcess       = FALSE;
	    m_fadingIn            = FALSE;
	    m_fadingOut           = FALSE;
		m_mouseButton1pressed = FALSE;
		m_dragging            = FALSE;
	    m_dragGlid            = 0;
		m_dragVisualRef       = 0;
		m_dragVisualID        = 0;
		m_dragDupPassed       = 0;
	}
}     


void CBoneCursorObjList::Clone(CBoneCursorObjList ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
    CBoneCursorObjList * copyLocal = new CBoneCursorObjList();
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CBoneCursorObj * ptr = (CBoneCursorObj *)GetNext(posLoc);
        CBoneCursorObj * clonePtr = NULL;
		ptr->Clone(&clonePtr,g_pd3dDevice);
		copyLocal->AddTail(clonePtr);
	}
    *copy = copyLocal;
}

CBoneCursorObj * CBoneCursorObjList::GetByUseTypeID(int id)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CBoneCursorObj * ptr = (CBoneCursorObj *)GetNext(posLoc);
		if(ptr->m_useTypeID == id)
		   return ptr;

	}
    return NULL;
}

IMPLEMENT_SERIAL(CBoneCursorObj, CObject, VERSIONABLE_SCHEMA|2)
IMPLEMENT_SERIAL(CBoneCursorObjList, CObList, 0)

