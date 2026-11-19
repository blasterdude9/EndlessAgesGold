#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d9.h>
#include <afxtempl.h>
#include "CExplosionClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CExplosionObj::CExplosionObj()
{
    m_meshObject         = NULL;
	m_frame              = NULL;
	m_keyFrames          = NULL;
	m_duration           = 1000;
    m_baseScalerX        = 1.0f;
	m_baseScalerY        = 1.0f;
	m_baseScalerZ        = 1.0f;
    m_damageRadius       = 0.0f;
    m_radiusDamage       = 0;
	m_beginTimeStamp     = 0;
	m_orientation        = 0;
	m_startFadeAt        = 0;
	m_currentKeyPosition = NULL;
	m_currentKeyDuration = 0;
    m_currentKeyTimeStamp= 0;
    m_lastScalerX        = 1.0f;
    m_lastScalerY        = 1.0f;
    m_lastScalerZ        = 1.0f;
    m_rotationX          = 0.0f;
    m_rotationY          = 0.0f;
    m_rotationZ          = 0.0f;
    m_lastRotationX      = 0.0f;
    m_lastRotationY      = 0.0f;
    m_lastRotationZ      = 0.0f;
	m_explosionName      = "NONE";
	m_enableSound        = FALSE;
	m_distanceFactor     = .03f;
	m_needInitPlay       = TRUE;
	m_clipRadius         = 50.0f;
	m_snID               = 0;
	m_soundHandle        = NULL;
	m_ownerTrace         = 0;
	m_splashDamageFinsh  = FALSE;
	m_pushPower          = 5.0f;
	m_light              = NULL;
	m_skeletalObj        = NULL;
	m_dbIndexRef         = 0;
	m_damageChannel      = 0;
	m_teamRef            = -1;
	m_damageSpecific     = -1;
	m_traceFollow        = -1;
    m_criteriaForSkillUse= 0;
}

CExplosionObj::~CExplosionObj()
{
	SafeDelete();
}

//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CExplosionObj::SafeDelete()
{
    if(m_meshObject){
       m_meshObject->SafeDelete();
       delete m_meshObject;
       m_meshObject=0;
	}
	if(m_frame){
       m_frame->SafeDelete();
	   delete m_frame;
       m_frame = 0;
	}
	if(m_keyFrames){
       m_keyFrames->SafeDelete();
	   delete m_keyFrames;
	   m_keyFrames=0;
	}
	if(m_soundHandle)
	{
       m_soundHandle->SetToDelete();
	   m_soundHandle = NULL;
	}
	if(m_light)
	{
	   delete m_light;
	   m_light = 0;
	}

	if(m_skeletalObj)
	{
       m_skeletalObj->SafeDelete();
	   delete m_skeletalObj;
       m_skeletalObj = 0;
	}
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
BOOL CExplosionObj::Clone(CExplosionObj ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     CExplosionObj * copyLocal              = new CExplosionObj();
	 copyLocal->m_duration                  = m_duration;
	 copyLocal->m_baseScalerX               = m_baseScalerX;
	 copyLocal->m_baseScalerY               = m_baseScalerY;
	 copyLocal->m_baseScalerZ               = m_baseScalerZ;
	 copyLocal->m_damageRadius              = m_damageRadius;
	 copyLocal->m_radiusDamage              = m_radiusDamage;
	 copyLocal->m_orientation               = m_orientation;
	 copyLocal->m_startFadeAt               = m_startFadeAt;
	 copyLocal->m_enableSound               = m_enableSound;
	 copyLocal->m_distanceFactor            = m_distanceFactor;
	 copyLocal->m_snID                      = m_snID;
	 copyLocal->m_pushPower                 = m_pushPower;
	 copyLocal->m_damageChannel             = m_damageChannel;
	 copyLocal->m_damageSpecific            = m_damageSpecific;
	 copyLocal->m_traceFollow               = m_traceFollow;
	 copyLocal->m_criteriaForSkillUse       = m_criteriaForSkillUse;
	

	 if(m_light)
		m_light->Clone(&copyLocal->m_light);

     if(m_meshObject)
		m_meshObject->Clone(&copyLocal->m_meshObject,g_pd3dDevice);

	 if(m_keyFrames)
		m_keyFrames->Clone(&copyLocal->m_keyFrames);

	 if(m_skeletalObj)
		m_skeletalObj->Clone(&copyLocal->m_skeletalObj,g_pd3dDevice,NULL);


	 *clone = copyLocal;
  return TRUE;
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CExplosionObj::AddKeyFrame(D3DVECTOR scale,D3DVECTOR rotation,int duration)
{
   if(!m_keyFrames)
	   m_keyFrames = new CExpKeyFrameObjList();

       CExpKeyFrameObj * newKeyFrame = new CExpKeyFrameObj();
	   newKeyFrame->m_scalerX  = scale.x;
	   newKeyFrame->m_scalerY  = scale.y;
	   newKeyFrame->m_scalerZ  = scale.z;
	   newKeyFrame->m_rotationX  = rotation.x;
	   newKeyFrame->m_rotationY  = rotation.y;
	   newKeyFrame->m_rotationZ  = rotation.z;
	   newKeyFrame->m_duration = duration;

       m_keyFrames->AddTail(newKeyFrame);
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CExplosionObj::Serialize( CArchive& ar )
{

BOOL reserved = 0;

CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_meshObject
	      << m_duration
	      << m_baseScalerX
	      << m_baseScalerY
	      << m_baseScalerZ
	      << m_damageRadius 
	      << m_radiusDamage
		  << m_startFadeAt
		  << m_explosionName
		  << m_keyFrames
		  << m_orientation
		  << m_enableSound
	      << m_distanceFactor
		  << m_clipRadius
		  << m_snID
		  << m_pushPower
		  << m_light
		  << m_skeletalObj
		  << m_damageChannel
		  << m_criteriaForSkillUse
		  << reserved
		  << m_damageSpecific;//migrated

	}
	else
	{
       ar >> m_meshObject
	      >> m_duration
	      >> m_baseScalerX
	      >> m_baseScalerY
	      >> m_baseScalerZ
	      >> m_damageRadius 
	      >> m_radiusDamage
		  >> m_startFadeAt
		  >> m_explosionName
		  >> m_keyFrames
		  >> m_orientation
		  >> m_enableSound
	      >> m_distanceFactor
		  >> m_clipRadius
		  >> m_snID
		  >> m_pushPower
		  >> m_light
		  >> m_skeletalObj
		  >> m_damageChannel
		  >> m_criteriaForSkillUse
		  >> reserved
		  >> m_damageSpecific;//migrated
	  
       //m_damageSpecific     = -1;

	   m_needInitPlay       = TRUE;
	   m_soundHandle        = NULL;
	   m_ownerTrace         = 0;
	   m_splashDamageFinsh  = FALSE;
	   m_dbIndexRef         = 0;
	   m_teamRef            = -1;
	   m_traceFollow        = -1;

	   //m_skeletalObj        = NULL;
	   //m_clipRadius   = 50.0f;
	}
}   
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CExplosionObj::SetFade(long currentTime)
{
    if(!m_meshObject)
		return;

	if(m_meshObject->List_materialObject->m_transition == 4)
	   return;
	m_meshObject->List_materialObject->m_cycleTime = m_duration - (currentTime - m_beginTimeStamp);
    m_meshObject->List_materialObject->m_transition = 4;//fade out
    m_meshObject->List_materialObject->m_timeStamp = GetTickCount();
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
BOOL CExplosionObj::LoadBasicMesh(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	if(m_meshObject){
       m_meshObject->SafeDelete();
	   delete m_meshObject;
	   m_meshObject=0;
	}
	m_meshObject = new CEXMeshObj();
    float dumpRange = 0;
    if(!m_meshObject->LoadGeometryFromPlugin(fileName,&dumpRange,g_pd3dDevice,NULL))
		return FALSE;
	
	    m_meshObject->List_materialObject = new CMaterialObject();
		m_meshObject->List_materialObject->m_blendMode = 2;
		m_meshObject->List_materialObject->m_baseMaterial.Emissive.r   = 1.0f;
	    m_meshObject->List_materialObject->m_baseMaterial.Emissive.g   = 1.0f;
	    m_meshObject->List_materialObject->m_baseMaterial.Emissive.b   = 1.0f;	   
	    
		
		
		m_meshObject->List_materialObject->Reset();

return TRUE;
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
BOOL CExplosionObjList::AddExplosionToDB(CString fileName,
										 int duration,
										 float damageRadius,
										 int orientation,
										 int radiusDamage,
										 LPDIRECT3DDEVICE9 g_pd3dDevice)
{
   CExplosionObj * newExp = new CExplosionObj();
   newExp->m_damageRadius              = damageRadius;
   newExp->m_orientation               = orientation;
   newExp->m_radiusDamage              = radiusDamage;
   newExp->m_duration                  = duration;

   if(!newExp->LoadBasicMesh(fileName,g_pd3dDevice)){
	   delete newExp;
	   newExp=0;
	   return FALSE;
   }

   /*D3DVECTOR scalerWale,rotationWale;
   scalerWale.x = 2.0f;
   scalerWale.y = 2.0f;
   scalerWale.z = .5f;
   rotationWale.x = 0.0f;
   rotationWale.y = 0.0f;
   rotationWale.z = 180.0f;
   newExp->AddKeyFrame(scalerWale,rotationWale,200);
   scalerWale.x = 4.0f;
   scalerWale.y = 4.0f;
   scalerWale.z = 1.5f;
   rotationWale.z = 360.0f;
   newExp->AddKeyFrame(scalerWale,rotationWale,600);
   scalerWale.x = 4.5f;
   scalerWale.y = 4.5f;
   scalerWale.z = 1.2f;
   rotationWale.z = 720.0f;
   newExp->AddKeyFrame(scalerWale,rotationWale,500);
   scalerWale.x = 4.8f;
   scalerWale.y = 4.8f;
   scalerWale.z = 1.1f;
   rotationWale.z = 920.0f;
   newExp->AddKeyFrame(scalerWale,rotationWale,200);*/

   AddTail(newExp);
   return TRUE;
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
BOOL CExplosionObj::IndexKeyframe()
{
	if(!m_keyFrames)
		return FALSE;
	if(!m_currentKeyPosition)
		return FALSE;
        if(m_keyFrames){
           CExpKeyFrameObj * keyframe = (CExpKeyFrameObj *)m_keyFrames->GetNext(m_currentKeyPosition);
		   
		   m_lastScalerX        = m_baseScalerX;
           m_lastScalerY        = m_baseScalerY;
           m_lastScalerZ        = m_baseScalerZ;
		   
		   m_baseScalerX        = keyframe->m_scalerX;
	       m_baseScalerY        = keyframe->m_scalerY;
	       m_baseScalerZ        = keyframe->m_scalerZ;
		   
		   m_lastRotationX      = m_rotationX;
           m_lastRotationY      = m_rotationY;
           m_lastRotationZ      = m_rotationZ;
		   
		   m_rotationX          = keyframe->m_rotationX;
	       m_rotationY          = keyframe->m_rotationY;
	       m_rotationZ          = keyframe->m_rotationZ;
		   m_currentKeyDuration = keyframe->m_duration;
           m_currentKeyTimeStamp= GetTickCount();
		}
		return TRUE;
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
CExplosionObj * CExplosionObjList::CreateExplosion(int dbIndex,
										D3DVECTOR wldPosition,
										CExplosionObjList * runtimeRenderList,
										D3DVECTOR lookatPosition,
										int ownerTrace,
										LPDIRECT3DDEVICE9 g_pd3dDevice,
										int teamRef,
										int traceFollow
										)
{
         CExplosionObj * expObj = GetPtr(dbIndex);
         if(!expObj)
		     return NULL;
		 
		 CExplosionObj * newExp = NULL;
		 expObj->Clone(&newExp,g_pd3dDevice);
		 newExp->m_ownerTrace = ownerTrace;
		 newExp->m_dbIndexRef = dbIndex;
		 newExp->m_teamRef = teamRef;
		 newExp->m_traceFollow = traceFollow;

		 if(newExp->m_skeletalObj)
		 {
		    newExp->m_skeletalObj->m_animationInUse = -2;
		    newExp->m_skeletalObj->SetCurrentAnimation(0,100);
		 }
		 /*if(newExp->m_soundRange){
			if(newExp->m_soundRange->m_soundBuffer)
               newExp->m_soundRange->m_soundBuffer->Play(0,0,0);
		 }*/
		 newExp->m_frame = new CFrameObj(NULL);
         newExp->m_frame->SetPosition(NULL,wldPosition);
         if(newExp->m_keyFrames){
		 POSITION keyPosition = newExp->m_keyFrames->FindIndex(0);
		    if(keyPosition){
               CExpKeyFrameObj * keyframe = (CExpKeyFrameObj *)newExp->m_keyFrames->GetNext(keyPosition);
			   newExp->m_baseScalerX        = keyframe->m_scalerX;
	           newExp->m_baseScalerY        = keyframe->m_scalerY;
	           newExp->m_baseScalerZ        = keyframe->m_scalerZ;
			   newExp->m_rotationX          = keyframe->m_rotationX;
	           newExp->m_rotationY          = keyframe->m_rotationY;
	           newExp->m_rotationZ          = keyframe->m_rotationZ;
			   newExp->m_currentKeyDuration = keyframe->m_duration;
               newExp->m_currentKeyTimeStamp= GetTickCount();
			   newExp->m_currentKeyPosition = keyPosition;
			}
		 }
		 if(newExp->m_orientation == 2)//Ground Orientate
            newExp->m_frame->FaceTowardPoint(lookatPosition);

		 newExp->m_beginTimeStamp = GetTickCount();
         runtimeRenderList->AddTail(newExp);
     return newExp;
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
CExplosionObj* CExplosionObjList::GetPtr(int index)
{
    if(index < 0)
	   return NULL;

	if(directAccess)
      return GetByTypeDirectAccess(index);

    POSITION posLoc = FindIndex(index);
	if(posLoc){
        CExplosionObj* expObj = (CExplosionObj*)GetAt(posLoc);
	    return expObj;
	}
	return NULL;
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CExplosionObjList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CExplosionObj* expObj = (CExplosionObj*)GetNext(posLoc);
		expObj->SafeDelete();
		delete expObj;
		expObj=0;
	}
	RemoveAll();
	if(directAccess)
	{
       delete [] directAccess;
       directAccess = 0;
	}
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
CExpKeyFrameObj::CExpKeyFrameObj()
{
  m_scalerX   = 0.0f;
  m_scalerY   = 0.0f;
  m_scalerZ   = 0.0f;
  m_rotationX = 0.0f;
  m_rotationY = 0.0f;
  m_rotationZ = 0.0f;
  m_duration  = 300;
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CExpKeyFrameObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_scalerX
	      << m_scalerY
	      << m_scalerZ
		  << m_duration
		  << m_rotationX
          << m_rotationY
          << m_rotationZ;
	}
	else
	{
       ar >> m_scalerX
	      >> m_scalerY
	      >> m_scalerZ
		  >> m_duration
		  >> m_rotationX
          >> m_rotationY
          >> m_rotationZ;
	}
}   
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CExpKeyFrameObj::Clone(CExpKeyFrameObj **copy)
{
   CExpKeyFrameObj * copyLocal              = new CExpKeyFrameObj();
	 copyLocal->m_scalerX                   = m_scalerX;
	 copyLocal->m_scalerY                   = m_scalerY;
	 copyLocal->m_scalerZ                   = m_scalerZ;
	 copyLocal->m_rotationX                 = m_rotationX;
	 copyLocal->m_rotationY                 = m_rotationY;
	 copyLocal->m_rotationZ                 = m_rotationZ;
	 copyLocal->m_duration                  = m_duration;
	
	 *copy = copyLocal;
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CExpKeyFrameObjList::SafeDelete()
{
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CExpKeyFrameObj* kObj = (CExpKeyFrameObj*)GetNext(posLoc);
		delete kObj;
		kObj=0;
	}
	RemoveAll();
	
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CExpKeyFrameObjList::Clone(CExpKeyFrameObjList **copy)
{
    CExpKeyFrameObjList * copyLocal = new CExpKeyFrameObjList();
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CExpKeyFrameObj* kObj = (CExpKeyFrameObj*)GetNext(posLoc);
		CExpKeyFrameObj* clonedObj = NULL;
		kObj->Clone(&clonedObj);
		copyLocal->AddTail(clonedObj);
	}
  *copy = copyLocal;
}
//--------------------------------------------------------------------------------
CExplosionObj * CExplosionObjList::GetByTypeDirectAccess(int type)
{
     if(!directAccess)
		 return NULL;

     if(type < 0)
	    return NULL;

     if(type >= directAccessObjectCount)
	    return NULL;


	 return directAccess[type].object;
}
//-------------------------------------------------------------------------------
void CExplosionObjList::CompileDirectAccess()
{
    int misCount = (int)GetCount();
	if(misCount == 0)
	   return;

	if(directAccess)
	{
       delete [] directAccess;
       directAccess = 0;
	}
    directAccess = new DirectAccessStructEXP[misCount];
	int tracer = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        directAccess[tracer].object = (CExplosionObj *)GetNext(posLoc);
		tracer++;  
	}
	directAccessObjectCount = tracer;
}


IMPLEMENT_SERIAL(CExpKeyFrameObj, CObject, 0)
IMPLEMENT_SERIAL(CExpKeyFrameObjList, CObList, 0)
IMPLEMENT_SERIAL(CExplosionObj, CObject, 0)
IMPLEMENT_SERIAL(CExplosionObjList, CObList, 0)