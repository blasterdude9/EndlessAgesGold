#include "stdafx.h"

#define D3D_OVERLOADS
#include <d3d9.h>
#include <afxtempl.h>
#include "CCameraClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//----------------------------------------------//
//------------Main object list------------------//
//----------------------------------------------//
CCameraObj::CCameraObj(CString   cameraName,
			           CString   fileRefVisualOptional,
			           int       type,
			           BOOL      lookAt,
					   float     clipPlane,
					   float     fovView,
					   D3DVECTOR initFloatingTarget
			           )
{
//input
List_fovView               = fovView;
List_clipPlane             = clipPlane;
List_fileRefVisualOptional = fileRefVisualOptional;
List_cameraName            = cameraName; 
List_type                  = type;
List_floatingCamTarget.x   = initFloatingTarget.x;
List_floatingCamTarget.y   = initFloatingTarget.y;
List_floatingCamTarget.z   = initFloatingTarget.z;
//hard preinit
List_fixedAfterPosX        = 0.0f;
List_fixedAfterPosY        = 0.0f;
List_fixedAfterPosZ        = 0.0f;
List_InitialPosX           = 0.0f;
List_InitialPosY           = 0.0f;
List_InitialPosZ           = 0.0f;
List_floatingMovePwr       = 0.01f;
List_floatingUsesCollision = FALSE;
List_zoomInSpeed           = 5.0f;
List_zoomOutSpeed          = 1.0f;
List_zoomInStop            = 10.0f;
List_floatingTolerance     = 0.0f;
List_cameraMaxPitch        = 60.0f;
List_cameraCurPitch        = 0.0f;
List_cameraPitchSpeed      = 1.0f; 
List_cameraMaxYaw          = 60.0f;
List_cameraCurYaw          = 0.0f;
List_cameraYawSpeed        = 1.0f;
List_hieghtBaseAspect      = 1.0f;
List_cameraTarget          = NULL;
List_cameraBase            = NULL;
List_floatingCamParent     = NULL;
List_floatingFocus         = NULL;
List_locked                = FALSE;
m_pitchRotation            = 0.0f;

m_curKickX                 = 0.0f;
m_curKickY                 = 0.0f;
m_curBurstRate             = 0.0f;
m_curBurstRateY            = 0.0f;
m_kickBurstDet             = .8f;
m_kickReturnRate           = .9f;

m_bobXRotCur               = 0.0f;
m_bobCurMaxVelocity        = 0.0f;
m_bobCurVelocity           = 0.0f;
m_bobAccelRate             = 0.001f;
m_bobDecelRate             = 0.002f;
m_bobStallTime             = 500;
m_bobSwitch                = TRUE;
m_bobAccelerating          = TRUE;
m_bobTimeStamp             = GetTickCount();
m_entityRef = NULL;

m_miscVal1 = 0;
m_miscVal2 = 0.0f;
m_miscVal3 = 0.0f;
m_miscVal4 = 0.0f;
m_filterFirstPersonMode = 0;
ZeroMemory(&m_camDirNormal,sizeof(D3DVECTOR));
}
//----------------------------------------------//
//----------------------------------------------//
//----------------------------------------------//
void CCameraObj::Clone(CCameraObj ** copy)
{
     CCameraObj * copyLocal = new CCameraObj();
     //input
     copyLocal->List_fovView               = List_fovView;
     copyLocal->List_clipPlane             = List_clipPlane;
     copyLocal->List_fileRefVisualOptional = List_fileRefVisualOptional;
     copyLocal->List_cameraName            = List_cameraName; 
     copyLocal->List_type                  = List_type;
     copyLocal->List_floatingCamTarget.x   = List_floatingCamTarget.x;
     copyLocal->List_floatingCamTarget.y   = List_floatingCamTarget.y;
     copyLocal->List_floatingCamTarget.z   = List_floatingCamTarget.z;
	 copyLocal->List_floatingUsesCollision = List_floatingUsesCollision;
	 copyLocal->List_locked                = List_locked;
     copyLocal->List_fixedAfterPosX        = List_fixedAfterPosX;
     copyLocal->List_fixedAfterPosY        = List_fixedAfterPosY;
     copyLocal->List_fixedAfterPosZ        = List_fixedAfterPosZ;
     copyLocal->List_InitialPosX           = List_InitialPosX;
     copyLocal->List_InitialPosY           = List_InitialPosY;
     copyLocal->List_InitialPosZ           = List_InitialPosZ;
     copyLocal->List_floatingMovePwr       = List_floatingMovePwr;
     copyLocal->List_zoomInSpeed           = List_zoomInSpeed;
     copyLocal->List_zoomOutSpeed          = List_zoomOutSpeed;
     copyLocal->List_zoomInStop            = List_zoomInStop;
     copyLocal->List_floatingTolerance     = List_floatingTolerance;
     copyLocal->List_cameraMaxPitch        = List_cameraMaxPitch;
     copyLocal->List_cameraCurPitch        = List_cameraCurPitch;
     copyLocal->List_cameraPitchSpeed      = List_cameraPitchSpeed; 
     copyLocal->List_cameraMaxYaw          = List_cameraMaxYaw;
     copyLocal->List_cameraCurYaw          = List_cameraCurYaw;
     copyLocal->List_cameraYawSpeed        = List_cameraYawSpeed;
     copyLocal->List_hieghtBaseAspect      = List_hieghtBaseAspect;
	 copyLocal->m_pitchRotation            = m_pitchRotation;

	 copyLocal->m_miscVal1                 = m_miscVal1;
     copyLocal->m_miscVal2                 = m_miscVal2;
     copyLocal->m_miscVal3                 = m_miscVal3;
     copyLocal->m_miscVal4                 = m_miscVal4;
	 copyLocal->m_filterFirstPersonMode    = m_filterFirstPersonMode;

     List_cameraTarget          = NULL;
     List_cameraBase            = NULL;
     List_floatingCamParent     = NULL;
     List_floatingFocus         = NULL;
     
   *copy = copyLocal;
}
//----------------------------------------------//
//----------------------------------------------//
//----------------------------------------------//
void CCameraObj::CameraBretheCallback(float glAdjust)
{
    if(m_bobCurMaxVelocity ==0.0f)
	   return;

    if(m_bobSwitch == TRUE)
	{
	   if(((long)GetTickCount()-m_bobTimeStamp) > m_bobStallTime)
	   {
        if(m_bobAccelerating)
	    {
          m_bobXRotCur = m_bobXRotCur + (glAdjust * m_bobCurVelocity);
          m_bobCurVelocity = m_bobCurVelocity + (glAdjust * m_bobAccelRate);
          if(m_bobCurVelocity >= m_bobCurMaxVelocity)
             m_bobAccelerating = FALSE;
	    }
	    else
	    {// 
          m_bobXRotCur = m_bobXRotCur + (glAdjust * m_bobCurVelocity);
          m_bobCurVelocity = m_bobCurVelocity - (glAdjust * m_bobDecelRate);
          if(m_bobCurVelocity <= 0.0f)
		  {
             m_bobAccelerating = TRUE;
			 m_bobSwitch = FALSE;
			 m_bobCurVelocity = 0.0f;
			 
		  }
	    }
	   }
	}
	else
	{
       if(m_bobAccelerating)
	   {
          m_bobXRotCur = m_bobXRotCur - (glAdjust * m_bobCurVelocity);
          m_bobCurVelocity = m_bobCurVelocity + (glAdjust * m_bobAccelRate);
          if(m_bobCurVelocity >= m_bobCurMaxVelocity)
             m_bobAccelerating = FALSE;
	   }
	   else
	   {// 
          m_bobXRotCur = m_bobXRotCur - (glAdjust * m_bobCurVelocity);
          m_bobCurVelocity = m_bobCurVelocity - (glAdjust * m_bobDecelRate);
          if(m_bobCurVelocity <= 0.0f)
		  {
             m_bobAccelerating = TRUE;
			 m_bobSwitch = TRUE;
			 m_bobCurVelocity = 0.0f;
			 m_bobXRotCur = 0.0f;
			 m_bobTimeStamp = GetTickCount();
		  }
	   }
	}
}
//----------------------------------------------//
//----------------------------------------------//
//----------------------------------------------//
void CCameraObj::CameraKickCallback(float glAdjust)
{

    m_curKickX = m_curKickX + (m_curBurstRate * glAdjust);
	float oldcurBurstRate = (m_curBurstRate * m_kickBurstDet);
    m_curBurstRate = m_curBurstRate - ((m_curBurstRate - oldcurBurstRate) * glAdjust);
	if(m_curBurstRate < .5f)
	{
       m_curBurstRate = 0;
	   if(m_curKickX > 0)
	   { 
          float oldcurKickX = (m_curKickX * m_kickReturnRate);
		  m_curKickX = m_curKickX - ((m_curKickX - oldcurKickX) * glAdjust);
		  
		  if(m_curKickX < .1)
		     m_curKickX = 0;
	   }
	}

	m_curKickY = m_curKickY + (m_curBurstRate * glAdjust);
	float oldcurBurstRateY = (m_curBurstRate * m_kickBurstDet);
    m_curBurstRateY = m_curBurstRateY - ((m_curBurstRateY - oldcurBurstRateY) * glAdjust);
	if(m_curBurstRateY < .5f)
	{
       m_curBurstRateY = 0;
	   if(m_curKickY > 0)
	   { 
          float oldcurKickY = (m_curKickY * m_kickReturnRate);
		  m_curKickY = m_curKickY - ((m_curKickY - oldcurKickY) * glAdjust);
		  
		  if(m_curKickY < .1)
		     m_curKickY = 0;
	   }
	}

	//m_curBurstRate

}
//----------------------------------------------//
//------------Main object list------------------//
//----------------------------------------------//
D3DMATRIX CCameraObj::GetCameraMatrix()
{
D3DMATRIX matrixBase,adjust;
List_cameraBase->GetTransform(NULL,&matrixBase);


gl_matrixFunctions.Rotate(m_pitchRotation,0.0f,0.0f,adjust.m);
gl_matrixFunctions.fastMatrixMultiply(matrixBase,adjust,matrixBase);
//camPtr->List_cameraBase->AddTransformBefore(adjust);

if(List_locked == TRUE)
return matrixBase;

//apply kickRotations here
/*m_maxKickX                 = 30.0f;//degrees
m_maxKickY                 = 0.0f;
m_curKickX                 = 0.0f;
m_curKickY                 = 0.0f;
m_curBurstRate             = 1.0f;
m_kickBurstRate            = 1.0f;
m_kickBurstDet             = .95f;
m_kickReturnBurst          = .5f;*/


D3DMATRIX apply;
gl_matrixFunctions.Rotate(-(List_cameraCurPitch+m_curKickX+m_bobXRotCur),0.0f,0.0f,apply.m);

gl_matrixFunctions.fastMatrixMultiply(matrixBase,apply,matrixBase);

gl_matrixFunctions.Rotate(0.0f,List_cameraCurYaw+m_curKickY,0.0f,apply.m);

D3DVECTOR lastPos;
lastPos.x = matrixBase._41;
lastPos.y = matrixBase._42;
lastPos.z = matrixBase._43;

matrixBase._41 = 0.0f;
matrixBase._42 = 0.0f;
matrixBase._43 = 0.0f;

gl_matrixFunctions.fastMatrixMultiply(matrixBase,matrixBase,apply);

matrixBase._41 = lastPos.x;
matrixBase._42 = lastPos.y;
matrixBase._43 = lastPos.z;

m_camDirNormal = gl_matrixFunctions.UpdatePolygonVertexRotationOnly(matrixBase.m,m_camDirNormal);

return matrixBase;
}
//----------------------------------------------//
//------------Main object list------------------//
//----------------------------------------------//
D3DMATRIX CCameraObj::GetCameraInvMatrix()
{
D3DMATRIX matrixBase = GetCameraMatrix();
gl_matrixFunctions.D3DMath_MatrixInvert(matrixBase,matrixBase);
return matrixBase;
}
//----------------------------------------------//
//------------Main object list------------------//
//----------------------------------------------//
void CCameraObj::SafeDelete()
{

	if(List_cameraBase){
	   List_cameraBase->SafeDelete();
       delete List_cameraBase;
	   List_cameraBase = 0;
	}
	
	if(List_floatingCamParent){
	   List_floatingCamParent->SafeDelete();
       delete List_floatingCamParent;
	   List_floatingCamParent = 0;
	}
	       
    if(List_floatingFocus){
	   List_floatingFocus->SafeDelete();
       delete List_floatingFocus;
	   List_floatingFocus = 0;
	}      
}


void CCameraObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){

		ar.SetObjectSchema(3);
        ASSERT(ar.GetObjectSchema() == 3);

	    ar << List_type 
		   << List_lookAt 
		   << List_fileRefVisualOptional 
		   << List_cameraName 
		   << List_clipPlane 
		   << List_fovView
		   << List_fixedAfterPosX
		   << List_fixedAfterPosY
		   << List_fixedAfterPosZ
		   << List_InitialPosX
		   << List_InitialPosY
		   << List_InitialPosZ
		   << List_floatingMovePwr
		   << List_floatingUsesCollision
		   << List_zoomInSpeed
           << List_zoomOutSpeed
           << List_zoomInStop
		   << List_floatingTolerance
		   << List_floatingCamTarget.x
		   << List_floatingCamTarget.y
		   << List_floatingCamTarget.z
		   << List_cameraMaxPitch
		   << List_cameraPitchSpeed
		   << List_cameraMaxYaw
           << List_cameraYawSpeed
		   << List_hieghtBaseAspect
		   << m_pitchRotation;

		   ar << m_miscVal1
              << m_miscVal2
              << m_miscVal3
              << m_miscVal4
	          << m_filterFirstPersonMode;
	}
	else
	{

        int version = ar.GetObjectSchema();

        ar >> List_type 
		   >> List_lookAt 
		   >> List_fileRefVisualOptional 
		   >> List_cameraName 
		   >> List_clipPlane 
		   >> List_fovView
		   >> List_fixedAfterPosX
		   >> List_fixedAfterPosY
		   >> List_fixedAfterPosZ
		   >> List_InitialPosX
		   >> List_InitialPosY
		   >> List_InitialPosZ
		   >> List_floatingMovePwr
		   >> List_floatingUsesCollision
		   >> List_zoomInSpeed
           >> List_zoomOutSpeed
           >> List_zoomInStop
		   >> List_floatingTolerance
		   >> List_floatingCamTarget.x
		   >> List_floatingCamTarget.y
		   >> List_floatingCamTarget.z
		   >> List_cameraMaxPitch
		   >> List_cameraPitchSpeed
		   >> List_cameraMaxYaw
           >> List_cameraYawSpeed
		   >> List_hieghtBaseAspect;

        m_pitchRotation     = 0.0f;

	   m_miscVal1 = 0;
       m_miscVal2 = 0.0f;
       m_miscVal3 = 0.0f;
       m_miscVal4 = 0.0f;
	   m_filterFirstPersonMode = 0;

       if(version > 1)
	   {
          ar >> m_pitchRotation; 
	   }

	   if(version > 2)
	   {
	       ar >> m_miscVal1
              >> m_miscVal2
              >> m_miscVal3
              >> m_miscVal4
	          >> m_filterFirstPersonMode;
	   }


	   List_cameraCurPitch = 0.0f;
	   List_floatingFocus  = NULL;
	   List_cameraTarget   = NULL;
       List_cameraBase     = NULL;
	   List_floatingCamParent = NULL;
	   
	   List_cameraCurYaw   = 0.0f;
	   List_locked         = FALSE;



       m_curKickX                 = 0.0f;
       m_curKickY                 = 0.0f;
       m_curBurstRate             = 0.0f;
       m_kickBurstDet             = .95f;
	   m_kickReturnRate           = .9f;
	   m_curBurstRateY            = 0.0f;
	   m_entityRef                = NULL;
	}
}

void CCameraObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
	    CCameraObj * mPtr = (CCameraObj *)GetNext(posLoc);
		mPtr->SafeDelete();
		delete mPtr;
		mPtr = 0;
	}
	RemoveAll();
}




IMPLEMENT_SERIAL(CCameraObj, CObject, VERSIONABLE_SCHEMA|3)
IMPLEMENT_SERIAL(CCameraObjList, CObList, 0)
