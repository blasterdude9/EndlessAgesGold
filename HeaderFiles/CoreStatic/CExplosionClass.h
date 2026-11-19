#include "matrixarb.h"
#include "cexmeshclass.h"
#include "CFrameObj.h"
#include "CSSoundRange.h"
#include "CSoundManagerClass.h"
#include "CRTLightClass.h"
#include "CSkeletalClass.h"

#ifndef CEXPLOSIONCLASS_H
#define CEXPLOSIONCLASS_H

class  CExpKeyFrameObj : public CObject
{
	
     
	DECLARE_SERIAL( CExpKeyFrameObj );
	
    
public:
    CExpKeyFrameObj();
	void                       Clone(CExpKeyFrameObj **copy);
    float                      m_scalerX;
	float                      m_scalerY;
	float                      m_scalerZ;
	float                      m_rotationX;
    float                      m_rotationY;
	float                      m_rotationZ;
	long                       m_duration;	
	void Serialize( CArchive& ar );
	
};

class  CExpKeyFrameObjList : public CObList
{
public:
   CExpKeyFrameObjList(){};
   
    
     
	DECLARE_SERIAL( CExpKeyFrameObjList );
	
    

	
	//Functions
	void Clone(CExpKeyFrameObjList **copy);
	void SafeDelete();
};
//----------------------------------------------//
//----------------------------------------------//
//----------------------------------------------//
class  CExplosionObj : public CObject
{
	
     
	DECLARE_SERIAL( CExplosionObj );
	
    
public:
    CExplosionObj();
	~CExplosionObj();

	void SafeDelete();
	void                       SetFade(long currentTime);
	BOOL                       Clone(CExplosionObj **copy,LPDIRECT3DDEVICE9 g_pd3dDevice);
    MatrixARB                  GL_matrixFunctions;
	CEXMeshObj                *m_meshObject;
	long                       m_duration;
	CFrameObj *                m_frame;


    CString                    m_explosionName;
    float                      m_baseScalerX;
	float                      m_baseScalerY;
	float                      m_baseScalerZ;
	float                      m_rotationX;
    float                      m_rotationY;
	float                      m_rotationZ;
	float                      m_lastScalerX;
	float                      m_lastScalerY;
	float                      m_lastScalerZ;
	float                      m_lastRotationX;
    float                      m_lastRotationY;
	float                      m_lastRotationZ;

    float                      m_damageRadius;
	int                        m_radiusDamage;
	long                       m_beginTimeStamp;
	int                        m_orientation;//0=none 1=to camera 2=to ground
	long                       m_startFadeAt;//<0 = none
	BOOL                       m_splashDamageFinsh;

	CExpKeyFrameObjList       *m_keyFrames;
	POSITION                   m_currentKeyPosition;
	long                       m_currentKeyDuration;
	long                       m_currentKeyTimeStamp;
	BOOL                       m_needInitPlay;
    float                      m_clipRadius;
	//--------------------------------------------//
	BOOL                       m_enableSound;
	float                      m_distanceFactor;
	int                        m_snID;
	int                        m_ownerTrace;
	float                      m_pushPower;
	int                        m_dbIndexRef;
	int                        m_damageChannel;
	int                        m_teamRef;

	int                        m_damageSpecific;

	int                        m_traceFollow;
	int                        m_criteriaForSkillUse;

	CSoundSampleObj           *m_soundHandle;
	CRTLightObj               *m_light;
	CSkeletonObject           *m_skeletalObj;

	BOOL IndexKeyframe();
    void AddKeyFrame(D3DVECTOR scale,D3DVECTOR rotation,int duration);
	BOOL LoadBasicMesh(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void Serialize( CArchive& ar );
	
};

struct DirectAccessStructEXP
{
    CExplosionObj * object;
};

class  CExplosionObjList : public CObList
{
public:
   CExplosionObjList(){directAccessObjectCount=0;directAccess=NULL;};
   
    
     
	DECLARE_SERIAL( CExplosionObjList );
	
    

	//Functions
	void SafeDelete();
	CExplosionObj * CreateExplosion(int dbIndex,
										D3DVECTOR wldPosition,
										CExplosionObjList * runtimeRenderList,
										D3DVECTOR lookatPosition,
										int ownerTrace,
										LPDIRECT3DDEVICE9 g_pd3dDevice,
										int teamRef,
										int traceFollow
										);
	BOOL AddExplosionToDB(CString fileName,
										 int duration,
										 float damageRadius,
										 int orientation,
										 int radiusDamage,
										 LPDIRECT3DDEVICE9 g_pd3dDevice);
	CExplosionObj* GetPtr(int index);
	CExplosionObj * GetByTypeDirectAccess(int type);
	void CompileDirectAccess();

	DirectAccessStructEXP * directAccess;
    int                     directAccessObjectCount;
};

#endif