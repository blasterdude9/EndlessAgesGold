#ifndef CMISSILECLASS_H
#define CMISSILECLASS_H

#include "CSkeletalClass.h"
#include "CEXMeshClass.h"
#include "CRTLightClass.h"
#include "CFrameObj.h"
#include "CSSoundRange.h"
#include "CLaserClass.h"
#include "CElementStatusClass.h"

//class  is basically used for popout
class   CMissileObj : public CObject
{
	//CMissileObj(){}
	
     
	DECLARE_SERIAL( CMissileObj );
	
    
public:
	
	CMissileObj();
	virtual ~CMissileObj();

	CString                m_missileName;
	int                    m_contactAttribute;
	int                    m_skillType;
	int                    m_abilityType;
	int                    m_traceOfTarget;
	float                  m_heatSeekingPower;
	BOOL                   m_forceExplodeAtEnd;
	D3DVECTOR              m_contactPointStored;
	D3DVECTOR              m_collidedNormalStore;
	D3DVECTOR              m_lastPosition;
	int                    m_lifeTime;
	int                    m_ownerTrace;
	int                    m_minDamageValue;
	int					   m_maxDamageValue;
	int					   m_dotRefValue;
	BOOL                   m_orientateToCamera;
    int                    m_particleLink;
	float                  m_collisionRadius;
    int                    m_startTimeStamp;
    int                    m_explosionLink;
    int                    m_typeOfProjectile;//0=custom mesh 1=Billboard 2=skeleton 3=lazer segment 4=persistent laser
    int                    m_criteriaForSkillUse;//0=none 1=containment
	float                  m_pushPower;
	int                    m_missileDBRef;
	int                    m_damageChannel;
	BOOL                   m_filterCollision;
	CElementStatusList    *m_damageChannelList;

	EnvParticleSystem     *m_particleEmmiter;
	CMovementCaps         *m_physicDynamics;
	CFrameObj             *m_baseFrame;
	CSkeletonObject       *m_skeletalVisual;
	CEXMeshObj            *m_basicMesh;
	CLaserObj             *m_laserObject;
    //BASIC SND
	BOOL                  m_enableSound;
	float                 m_distanceFactor;

	BOOL                  m_needInitPlay;
	int                   m_sndID;
	CSoundSampleObj      *m_soundHandle;

	BOOL                  m_burstForceType;
	BOOL                  m_burstAppliedState;
	BOOL                  m_useFullPhysics;
	BOOL                  m_useElastics;

	CRTLightObj          *m_light;

	int                   m_teamRef;
	int                   m_damageSpecific;

	float                 m_baseKickPower;
	        
             
	void Clone(CMissileObj ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice);
    void SafeDelete();
	void Serialize( CArchive& ar );

};

struct DirectAccessStructMIS
{
    CMissileObj * object;
};

class   CMissileObjList : public CObList
{
public:
   void SafeDelete();
   CMissileObjList(){directAccessObjectCount=0;directAccess=NULL;};
    
     
	DECLARE_SERIAL( CMissileObjList );
	
    

	CMissileObj * GetPtr(int index);
	CMissileObj * GetByTypeDirectAccess(int type);
	void CompileDirectAccess();

	DirectAccessStructMIS * directAccess;
    int                     directAccessObjectCount;
};

#endif