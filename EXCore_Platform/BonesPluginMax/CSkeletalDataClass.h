#include "CFrameObj.h"
#include "CEXMeshClass.h"
#include "CPhysicsEmuClass.h"
#include "mmstream.h"
#include "amstream.h"
#include "ddstream.h"
#include "dsound.h"
#include <mmsystem.h>
#include <DSound.h>
#include "matrixarb.h"

#include "CBoneClass.h"
#include "CStructuresMisl.h"
#include "CSphereCollisionClass.h"
#include "CDamagePathClass.h"
#include "CParticleEmitRange.h"
#include "cdeformablemeshclass.h"
#include "CFireRangeClass.h"
#include "CSoundManagerClass.h"
#include "CShadowDataClass.h"

#ifndef CSKELETONCLASS_H
#define CSKELETONCLASS_H

struct DeformableMeshStructV2
{
    CDeformableMesh      * m_defMeshVisualUnit;//has internal LODs
};

//special alternate cfg class
class  CAlterUnitDBObject : public CObject
{
	
     
	DECLARE_SERIAL( CAlterUnitDBObject );
	
    
public:
	
	  CAlterUnitDBObject();
	  CString m_alterUnitName;
	  void Serialize( CArchive& ar );
	  void SafeDelete();

      CDeformableMeshList * m_configurations;//index 0 is always the default
};

class  CAlterUnitDBObjectList : public CObList//the number of objects in this list represents the amount of deformable meshes
{
public:
	CAlterUnitDBObjectList(){};

	
     
	DECLARE_SERIAL( CAlterUnitDBObjectList );
	
    

	void SafeDelete();
};
//end special alternate cfg class

//Base skeleton Class
class  CSkeletonObject : public CObject
{
	
     
	DECLARE_SERIAL( CSkeletonObject );
	
    
public:
	
	CSkeletonObject();

	int                m_currentLOD;
	CString            m_SkeletalName;
	int                m_frameSpeed;
	int                m_ticksPerFrame;
	int                m_animationInUse;
	int                m_lastAnimationInUse;

	//extra
	int m_firstFrame;
	int m_lastFrame;
	int m_frameCount;


	//global animation management
	long m_primaryAnimLastUpdatedTime;
    long m_secondaryAnimLastUpdatedTime;
	BOOL m_inReverse;
	long m_currentTimePassedPrimary;
	long m_currentTimePassedSecondary;
	int  m_currentBaseFrame;
    int  m_secondaryBaseFrame;
	int  m_currentBaseSoundFrame;
    int  m_secondaryBaseSoundFrame;
	//new sytem var for animation management
	long               m_currentAnimationTime;

	

	
	CShadowClass *     m_shadowInterface;
	BOOL               m_shadowOn;
	ABBOX              m_box;
	BOOL               m_requireUpdate;

	//secondary animation system
	int                m_secondaryAnimationInUse;
	BOOL               m_inSecondaryTransition;
	long               m_secondaryTransitionStamp;
	BOOL               m_transitionOut;
	long               m_secondaryTransitionDuration;
    D3DVECTOR          m_secondaryCOMadjust;

	int                m_secondaryCOMInheritBone;


	//post bone management types
	int                m_freeLookBone; 
	float              m_freeLookPitch;
	float              m_freeLookYaw;

	int                m_waistBone;
	float              m_waistTurn;
	float              m_waistTurnMax;
	float              m_waistTurnRate;

	CBoneList *        m_bonesList;//hiarchy
	MatrixARB          GL_matrixFunctions;
	long               m_lastUpdateTime;
	int                m_deformableMeshArrayCount;
   
	//interpolation between animations
	BOOL               m_inTransition;
	int                m_transitionTime;
	int                m_transitionStartTime;
	int                m_transitionBaseFrame;
	
	//minimum play durations
	long               m_lastDurationUpdateTime;
	long               m_durationCurrentLength;
	long               m_lastScndDurationUpdateTime;
	long               m_scndDurationCurrentLength;

    //alternate animation options
	int                m_animPreference;

	//special collision locational spheres
	CSphereCollisionObjList *m_sphereLocationalDamageList;
	CSphereCollisionObj     *m_calulationSphere;
	CSphereCollisionObjList *m_sphereBodyCollisionSystem;
	//collision path spheres
	CDamagePathObjList      *m_damageSphereList;

    //deforming mesh list ( meshes assigned to skeleton that bones will control )

	DeformableMeshStructV2 * m_deformableMeshArrayV2;

    //Particle emmition list assignments
	CParticleEmitRangeObjList * m_particleEmiterList;
    //animations
	
	//fireing ques
	CFireRangeObjectList      * m_fireQueList;
	
	//spawn initializers
	CAlterUnitDBObjectList    * m_alternateConfigurations;
	int                       * m_spawnCfg;           


    //functions
	void Invalidate();
	void NormalizeAllNormals2(LPDIRECT3DDEVICE9 g_pd3dDevice);
	void UnifyAllNormalsDeformable(LPDIRECT3DDEVICE9 g_pd3dDevice);
	D3DMATRIX GetBoneMatrixByIndex(int index);
	void Clone(CSkeletonObject ** clone,
			   LPDIRECT3DDEVICE9 g_pd3dDevice,
			   int * spawnCfgOptional);
	void SafeDelete();
	BOOL GetBoneByName(CString       boneName,
							        CBoneObject ** boneObject);
	void Serialize( CArchive& ar );
	BOOL InitBoneSystemFromPlugin(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL InitBoneSystem(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL AddBone(CString       boneName,
							  CString       parentName,
							  D3DVECTOR     dirVect,
							  D3DVECTOR     upVect,
							  D3DVECTOR     position,
							  D3DVECTOR     scale,
							  CBoneObject ** boneNodeReturn);
	BOOL LoadAnimation(CString fileName,BOOL looping,int type,int duration,int replaceIndex);
	BOOL ApplyAnimation(int animation,int frame);
	BOOL ApplyAnimationByTime(int animation,int time);
	 BOOL AnimationCallback(long currentTime,
		                          D3DMATRIX & earCenter,
								  D3DVECTOR & wldSpacePos,
								  float globalSoundFactor,
								  BOOL soundOnly,
								  CSoundBankList * m_soundManager,
								  BOOL disableSoundOption,
								  BOOL & disableDeformbleCalc,
								  LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL SetCurrentAnimationByVersion(int version,int transitionTime);
	BOOL SetCurrentAnimation(int anim,int transitionTime);
	BOOL SetAnimationSpeed(int animNum,long newSpeed);
	BOOL SetAnimationSpeedBasedOnCap(int animNum,float percentage);
	BOOL SetAnimationDirection(int anim,BOOL reverse);
	D3DVECTOR GetBonePositionByIndex(int index);
	//void CharacterAnimationManagement(CMovementCaps * movPtr);
	BOOL SetAnimationBasedOnAttrib(int attrib,BOOL reverse,int versionPref,int transitionTime);
	BOOL GetAnimationDirection(int anim);
	void EnableShadow(BOOL enable,
								   float radius,
								   float collisionRadius,
								   float depth,
								   float hieght,
								   int   textureIndex,
								   CString textureSearchName);
    BOOL DeleteAnimationByType(int type);
	BOOL GetAnimationStatus(int *version,int *attrib,int animIndex);
	BOOL DeleteAnimationByIndex(int index);
	void ApplyFreeLookRotation(float pitch,float yaw);
	 void UpdateBasedDeformationByVertexAssignments(int & dmIndex,LPDIRECT3DDEVICE9 g_pd3dDevice);
	int GetAnimationIndexFromAttrib(int attrib);
	BOOL HowSwapDim(int deformableIndex,int newCfg,CSkeletonObject * targetRuntimeEntity,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void HandleSoundRangesForAnimation(CBoneAnimationObject * animPtr,
											  D3DVECTOR & wldSpacePos,
											  float & globalSoundFactor,
											  CSoundBankList * m_soundManager,
											  BOOL secondary);
	 void UpdateAnimMS(CBoneAnimationObject * animPtr,long currentTime,BOOL secondary);
	 void ApplyFreeLook();
	 void RemoveFreeLook();
	 void RemoveWaisteControl();
	 void ApplyWaisteControl();
	 void ApplySecondaryAdjust(int secondaryAdjustBone);
	 void RemoveSecondaryAdjust(int secondaryAdjustBone);
	 void MoveBoneMangement(long & currentTime,
								  long & millisecondsPerFrame,
								  CBoneObject * bonePtr,
				                  int  & locAnimInUse,
			                      int  & locanimationFrame,
								  BOOL & update,
								  long & overtimeReturn
								 );
	int GetAnimationBasedOnAttribAndPref(int attrib,
										 int versionPref,
										 CBoneAnimationObject ** returnPtr);
	BOOL QuerryBox();
	void ResetSkeletalNormalsDeformable(LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL BuildHiarchy();
	void UnifyAllNormalsDeformable2(LPDIRECT3DDEVICE9 g_pd3dDevice, float angTol);
	BOOL FadeOut(long duration);
	BOOL FadeIn(long duration);
	BOOL FadeInAmplified(long duration);
	BOOL FadeOutAmplified(long duration);
	void NormalizeAllNormals(LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL ScaleSystem(float percentage,LPDIRECT3DDEVICE9 g_pd3dDevice);
	CFrameObj * GetBoneFrameByIndex(int index);
	void Reinit(LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL InvertNormals();
};



#endif