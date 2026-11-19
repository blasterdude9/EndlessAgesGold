#include "stdafx.h"
#include <d3d9.h>
#include <afxtempl.h>
#include "CMissileClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMissileObj::CMissileObj()
{
    m_contactAttribute           = 0;
	m_skillType                  = -1;
	m_abilityType                = -1;
	m_traceOfTarget              = -1;
	m_heatSeekingPower           = 1.0f;
	m_forceExplodeAtEnd          = FALSE;
    m_contactPointStored.x       = 0.0f;
	m_contactPointStored.y       = 0.0f;
	m_contactPointStored.z       = 0.0f;
	m_collidedNormalStore.x      = 0.0f;
	m_collidedNormalStore.y      = 0.0f;
	m_collidedNormalStore.z      = 0.0f;
	m_lastPosition.x             = 0.0f;
	m_lastPosition.y             = 0.0f;
	m_lastPosition.z             = 0.0f;
	m_lifeTime                   = 2000;//milliseconds
	m_ownerTrace                 = -1;
	m_minDamageValue             = 1;
	m_maxDamageValue			 = 5;
	m_dotRefValue				 = -1;
	m_orientateToCamera          = FALSE;
    m_particleLink               = -1;
	m_collisionRadius            = 5.0f;
	m_startTimeStamp             = 0;
	m_explosionLink              = -1;
	m_typeOfProjectile           = 0;
	m_missileName                = "Missile";
	m_criteriaForSkillUse        = 0;
	m_pushPower                  = 0.0f;

    m_particleEmmiter            = NULL;
	m_baseFrame                  = NULL;
	m_skeletalVisual             = NULL;
	m_basicMesh                  = NULL;
	m_physicDynamics             = NULL;

	m_enableSound                = FALSE;
	m_distanceFactor             = .03f; 
	m_needInitPlay               = TRUE;
	m_laserObject                = NULL;
	m_missileDBRef               = -1;
    m_sndID                      = 0;
	m_soundHandle                = NULL;
	m_burstForceType             = FALSE;
    m_burstAppliedState          = FALSE;
	m_useFullPhysics             = FALSE;
	m_useElastics                = FALSE;
	m_light                      = NULL;
	m_damageChannel              = 0;
	m_filterCollision            = FALSE;
	m_teamRef                    = -1;
	m_damageSpecific             = -1;
	m_baseKickPower              = 0.0f;

	m_damageChannelList			 = NULL;
}

CMissileObj::~CMissileObj() 
{
	SafeDelete();
}

void CMissileObj::Clone(CMissileObj ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
   CMissileObj * copyLocal = new CMissileObj();

    copyLocal->m_contactAttribute           = m_contactAttribute;
	copyLocal->m_skillType                  = m_skillType;
	copyLocal->m_abilityType                = m_abilityType;
	copyLocal->m_traceOfTarget              = m_traceOfTarget;
	copyLocal->m_heatSeekingPower           = m_heatSeekingPower;
	copyLocal->m_forceExplodeAtEnd          = m_forceExplodeAtEnd;
    copyLocal->m_contactPointStored.x       = m_contactPointStored.x;
	copyLocal->m_contactPointStored.y       = m_contactPointStored.y;
	copyLocal->m_contactPointStored.z       = m_contactPointStored.z;
	copyLocal->m_collidedNormalStore.x      = m_collidedNormalStore.x;
	copyLocal->m_collidedNormalStore.y      = m_collidedNormalStore.y;
	copyLocal->m_collidedNormalStore.z      = m_collidedNormalStore.z;
	copyLocal->m_lastPosition.x             = m_lastPosition.x;
	copyLocal->m_lastPosition.y             = m_lastPosition.y;
	copyLocal->m_lastPosition.z             = m_lastPosition.z;
	copyLocal->m_lifeTime                   = m_lifeTime;
	copyLocal->m_ownerTrace                 = m_ownerTrace;
	copyLocal->m_minDamageValue             = m_minDamageValue;
	copyLocal->m_maxDamageValue				= m_maxDamageValue;
	copyLocal->m_dotRefValue				= m_dotRefValue;
	copyLocal->m_orientateToCamera          = m_orientateToCamera;
	copyLocal->m_particleLink               = m_particleLink;
	copyLocal->m_collisionRadius            = m_collisionRadius;
	copyLocal->m_explosionLink              = m_explosionLink;
	copyLocal->m_typeOfProjectile           = m_typeOfProjectile;
	copyLocal->m_missileName                = m_missileName;
	copyLocal->m_criteriaForSkillUse        = m_criteriaForSkillUse;
	copyLocal->m_pushPower                  = m_pushPower;
	copyLocal->m_sndID                      = m_sndID;
	copyLocal->m_enableSound                = m_enableSound;


	copyLocal->m_startTimeStamp             = 0;
	copyLocal->m_baseFrame                  = NULL;
	copyLocal->m_skeletalVisual             = NULL;
	copyLocal->m_basicMesh                  = NULL;
	copyLocal->m_physicDynamics             = NULL;
	copyLocal->m_burstForceType             = m_burstForceType;
    copyLocal->m_burstAppliedState          = m_burstAppliedState;	
	copyLocal->m_useFullPhysics             = m_useFullPhysics;
	copyLocal->m_useElastics                = m_useElastics;
	copyLocal->m_damageChannel              = m_damageChannel;
	copyLocal->m_filterCollision            = m_filterCollision;
	copyLocal->m_damageSpecific             = m_damageSpecific;
	copyLocal->m_baseKickPower              = m_baseKickPower;
    
    if(m_light)
	   m_light->Clone(&copyLocal->m_light);

    if(m_physicDynamics)
	   m_physicDynamics->Clone(&copyLocal->m_physicDynamics);
	
	if(m_skeletalVisual)
       m_skeletalVisual->Clone(&copyLocal->m_skeletalVisual,g_pd3dDevice,NULL);
	
	if(m_basicMesh)
       m_basicMesh->Clone(&copyLocal->m_basicMesh,g_pd3dDevice);
	   
	if(m_laserObject)
	   m_laserObject->Clone(&copyLocal->m_laserObject,NULL);

	if(m_damageChannelList)
		m_damageChannelList->Clone(&copyLocal->m_damageChannelList);

	*copy = copyLocal;

}



void CMissileObj::SafeDelete()
{
	if(m_baseFrame){
       m_baseFrame->SafeDelete();
	   delete m_baseFrame;
	   m_baseFrame=0;
	}
    if(m_skeletalVisual){
       m_skeletalVisual->SafeDelete();
	   delete m_skeletalVisual;
	   m_skeletalVisual=0;
	}
    if(m_basicMesh){
       m_basicMesh->SafeDelete();
	   delete m_basicMesh;
	   m_basicMesh=0;
	}
	if(m_physicDynamics){
       delete m_physicDynamics;
	   m_physicDynamics=0;
	}
    if(m_laserObject){
       m_laserObject->SafeDelete();
	   delete m_laserObject;
       m_laserObject = 0;
	}
	if(m_soundHandle){
       m_soundHandle->SetToDelete();
	   m_soundHandle = NULL;
	}
	if(m_light){
	   delete m_light;
       m_light = 0;
	}
	if(m_damageChannelList)
	{
		m_damageChannelList->SafeDelete();
		delete m_damageChannelList;
		m_damageChannelList = 0;
	}
}


void CMissileObj::Serialize( CArchive& ar )
{
int reservedInt = 0;
float reserveredFloat = 0.0f;


CObject::Serialize( ar );
	if(ar.IsStoring() ){

	   ar.SetObjectSchema(2);
       ASSERT(ar.GetObjectSchema() == 2);
		
	   ar  << m_contactAttribute
           << m_skillType
           << m_abilityType
           << m_traceOfTarget
           << m_heatSeekingPower
           << m_forceExplodeAtEnd
           << m_contactPointStored.x
           << m_contactPointStored.y
           << m_contactPointStored.z
           << m_collidedNormalStore.x
           << m_collidedNormalStore.y
           << m_collidedNormalStore.z
           << m_lastPosition.x 
           << m_lastPosition.y 
           << m_lastPosition.z 
           << m_lifeTime 
           << m_ownerTrace      
           << m_minDamageValue
           << m_orientateToCamera                 
           << m_skeletalVisual          
           << m_basicMesh
		   << m_particleLink
		   << m_collisionRadius
		   << m_physicDynamics
		   << m_explosionLink
		   << m_typeOfProjectile
		   << m_missileName
		   << m_criteriaForSkillUse
		   << m_damageChannelList
		   << m_enableSound
		   << m_pushPower
		   << m_distanceFactor
		   << m_laserObject
		   << m_sndID
		   << m_burstForceType
		   << m_useFullPhysics
		   << m_useElastics
		   << m_light
		   << m_filterCollision
		   << m_damageSpecific//migrated
		   << m_baseKickPower//migrated
		   << m_maxDamageValue
		   << m_dotRefValue
		   << m_damageChannel;
	   
	}
	else
	{
       int version = ar.GetObjectSchema();

	   //m_damageSpecific     = -1;
	   //m_filterCollision    = FALSE;

	   m_teamRef            = -1;
	   m_baseFrame          = NULL;
	   m_startTimeStamp     = 0;
	   m_particleEmmiter    = NULL;
	   m_needInitPlay       = TRUE;
	   m_missileDBRef       = -1;
	   m_soundHandle        = NULL;
	   //m_burstForceType     = FALSE;
	   m_burstAppliedState  = FALSE;
	   //m_useFullPhysics     = FALSE;
	   //m_useElastics        = FALSE;

	   if(version == 2)
	   {
		   ar  >> m_contactAttribute
			   >> m_skillType
			   >> m_abilityType
			   >> m_traceOfTarget
			   >> m_heatSeekingPower
			   >> m_forceExplodeAtEnd
			   >> m_contactPointStored.x
			   >> m_contactPointStored.y
			   >> m_contactPointStored.z
			   >> m_collidedNormalStore.x
			   >> m_collidedNormalStore.y
			   >> m_collidedNormalStore.z
			   >> m_lastPosition.x 
			   >> m_lastPosition.y 
			   >> m_lastPosition.z 
			   >> m_lifeTime 
			   >> m_ownerTrace      
			   >> m_minDamageValue           
			   >> m_orientateToCamera                 
			   >> m_skeletalVisual          
			   >> m_basicMesh
			   >> m_particleLink
			   >> m_collisionRadius
			   >> m_physicDynamics
			   >> m_explosionLink
			   >> m_typeOfProjectile
			   >> m_missileName
			   >> m_criteriaForSkillUse
			   >> m_damageChannelList
			   >> m_enableSound
			   >> m_pushPower
			   >> m_distanceFactor
			   >> m_laserObject
			   >> m_sndID
			   >> m_burstForceType
			   >> m_useFullPhysics
			   >> m_useElastics
			   >> m_light
			   >> m_filterCollision
			   >> m_damageSpecific//migrated
			   >> m_baseKickPower//migrated
			   >> m_maxDamageValue
			   >> m_dotRefValue
			   >> m_damageChannel;
	   }
	   else if(version == 1)
	   {
		   ar  >> m_contactAttribute
			   >> m_skillType
			   >> m_abilityType
			   >> m_traceOfTarget
			   >> m_heatSeekingPower
			   >> m_forceExplodeAtEnd
			   >> m_contactPointStored.x
			   >> m_contactPointStored.y
			   >> m_contactPointStored.z
			   >> m_collidedNormalStore.x
			   >> m_collidedNormalStore.y
			   >> m_collidedNormalStore.z
			   >> m_lastPosition.x 
			   >> m_lastPosition.y 
			   >> m_lastPosition.z 
			   >> m_lifeTime 
			   >> m_ownerTrace      
			   >> m_minDamageValue           
			   >> m_orientateToCamera                 
			   >> m_skeletalVisual          
			   >> m_basicMesh
			   >> m_particleLink
			   >> m_collisionRadius
			   >> m_physicDynamics
			   >> m_explosionLink
			   >> m_typeOfProjectile
			   >> m_missileName
			   >> m_criteriaForSkillUse
			   >> m_damageChannelList
			   >> m_enableSound
			   >> m_pushPower
			   >> m_distanceFactor
			   >> m_laserObject
			   >> m_sndID
			   >> m_burstForceType
			   >> m_useFullPhysics
			   >> m_useElastics
			   >> m_light
			   >> m_filterCollision
			   >> m_damageSpecific//migrated
			   >> m_baseKickPower//migrated
			   >> m_maxDamageValue
			   >> m_dotRefValue;
	   }
	   else
	   {
			ar >> m_contactAttribute
			   >> m_skillType
			   >> m_abilityType
			   >> m_traceOfTarget
			   >> m_heatSeekingPower
			   >> m_forceExplodeAtEnd
			   >> m_contactPointStored.x
			   >> m_contactPointStored.y
			   >> m_contactPointStored.z
			   >> m_collidedNormalStore.x
			   >> m_collidedNormalStore.y
			   >> m_collidedNormalStore.z
			   >> m_lastPosition.x 
			   >> m_lastPosition.y 
			   >> m_lastPosition.z 
			   >> m_lifeTime 
			   >> m_ownerTrace      
			   >> m_minDamageValue           
			   >> m_orientateToCamera                 
			   >> m_skeletalVisual          
			   >> m_basicMesh
			   >> m_particleLink
			   >> m_collisionRadius
			   >> m_physicDynamics
			   >> m_explosionLink
			   >> m_typeOfProjectile
			   >> m_missileName
			   >> m_criteriaForSkillUse
			   >> m_damageChannel
			   >> m_enableSound
			   >> m_pushPower
			   >> m_distanceFactor
			   >> m_laserObject
			   >> m_sndID
			   >> m_burstForceType
			   >> m_useFullPhysics
			   >> m_useElastics
			   >> m_light
			   >> m_filterCollision
			   >> m_damageSpecific//migrated
			   >> m_baseKickPower;//migrated; 

		   m_maxDamageValue		= m_minDamageValue + 5;
		   m_dotRefValue		= -1;
	   }       
	}
}


CMissileObj * CMissileObjList::GetPtr(int index)
{
   if(index < 0)
      return NULL;

   if(directAccess)
      return GetByTypeDirectAccess(index);

   POSITION posLoc = FindIndex(index);
   if(!posLoc)
	   return NULL;
        CMissileObj*misPtr=(CMissileObj*)GetAt(posLoc);
		return misPtr;
}

void CMissileObjList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CMissileObj*misPtr=(CMissileObj*)GetNext(posLoc);
		misPtr->SafeDelete();
		delete misPtr;
		misPtr=0;
	}
    RemoveAll();
	if(directAccess)
	{
       delete [] directAccess;
       directAccess = 0;
	}
}

CMissileObj * CMissileObjList::GetByTypeDirectAccess(int type)
{
     if(!directAccess)
		 return NULL;

     if(type < 0)
	    return NULL;

     if(type >= directAccessObjectCount)
	    return NULL;


	 return directAccess[type].object;
}

void CMissileObjList::CompileDirectAccess()
{
    int misCount = (int)GetCount();
	if(misCount == 0)
	   return;

	if(directAccess)
	{
       delete [] directAccess;
       directAccess = 0;
	}
    directAccess = new DirectAccessStructMIS[misCount];
	int tracer = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        directAccess[tracer].object = (CMissileObj *)GetNext(posLoc);
		tracer++;  
	}
	directAccessObjectCount = tracer;
}


IMPLEMENT_SERIAL(CMissileObj, CObject, VERSIONABLE_SCHEMA|2)
IMPLEMENT_SERIAL(CMissileObjList, CObList, 0)

