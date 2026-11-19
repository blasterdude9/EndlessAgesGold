
#include "CFrameObj.h"
#include "CEXMeshClass.h"
#include "CPhysicsEmuClass.h"
#include "dsound.h"
#include <mmsystem.h>
#include <DSound.h>
#include "CShadowClass.h"
#include "CBoneClass.h"
#include "CStructuresMisl.h"
#include "CSphereCollisionClass.h"
#include "CDamagePathClass.h"
#include "CParticleEmitRange.h"
#include "cdeformablemeshclass.h"
#include "CFireRangeClass.h"
#include "CSoundManagerClass.h"
#include "CTextureDBClass.h"
#include "CSkeletalLODClass.h"

#pragma once



struct DeformableMeshStructV2
{
    CDeformableMesh      * m_defMeshVisualUnit;//has internal LODs
};


struct LODOptCondition
{
   //If current LOD count, remove lod #s zero based //example: 10000,6000,3000,1500,500 0,1,2,3,4
   //4,4,2,0;3,0,2;3,0 example of string that gets parsed to generate thse structures in an array form
   //always make sure lod that you will keep are listed greatest to least, this will keep proper lod removal
   int lodMatch;//must have this many lods to proceed with lod removal 
   int removalCount;//how much of the array is actually used
   int removalArray[256];
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
	
    CAlterUnitDBObject * GetByName(CString name);
    CString AltStripper(CString & current);


  
	void SafeDelete();
};
//end special alternate cfg class

//Base skeleton Class
class  CSkeletonObject : public CObject
{
	
     
	DECLARE_SERIAL( CSkeletonObject );
	
    
public:
	
	CSkeletonObject();


	//BOOL               m_inSecondaryTransition;
	long               m_inSecondaryTransitionTime;

	int                m_currentLOD;
	CString            m_SkeletalName;
	int                m_frameSpeed;
	int                m_ticksPerFrame;
	int                m_animationInUse;
	int                m_lastAnimationInUse;
	int                m_curLod;

	char               m_scndEndCap;


	//global animation management
	DWORD m_primaryAnimLastUpdatedTime;
    DWORD m_secondaryAnimLastUpdatedTime;
	BOOL m_inReverse;
	long m_currentTimePassedPrimary;
	long m_currentTimePassedSecondary;
	int  m_currentBaseFrame;
    int  m_secondaryBaseFrame;
	int  m_currentBaseSoundFrame;
    int  m_secondaryBaseSoundFrame;
	//new sytem var for animation management
	long               m_currentAnimationTime;
	int                m_currentAnimVersion;
	int				   m_currentAnimType;
	DWORD              m_minSameVersionAnimaPlay;
	DWORD			   m_minSaveVerTimeStamp;

	

	
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
	BOOL               m_secondaryCOMadjustApply;
	BOOL               m_flagForDeactivateAdjust;


	int                m_secondaryCOMInheritBone;


	//post bone management types
	int                m_freeLookBone; 
	int                m_freeLookBone2;
	float              m_freeLookPitch;
	float              m_freeLookYaw;

	int                m_waistBone;
	float              m_waistTurn;
	float              m_waistTurnMax;
	float              m_waistTurnRate;

	CBoneList *        m_bonesList;//hiarchy
	MatrixARB          GL_matrixFunctions;
	DWORD              m_lastUpdateTime;
	int                m_deformableMeshArrayCount;
   
	//interpolation between animations
	BOOL               m_inTransition;
	int                m_transitionTime;
	int                m_transitionStartTime;
	int                m_transitionBaseFrame;
	
	//minimum play durations
	DWORD               m_lastDurationUpdateTime;
	DWORD               m_durationCurrentLength;
	DWORD               m_lastScndDurationUpdateTime;
	DWORD               m_scndDurationCurrentLength;

    //alternate animation options
	int					m_animPreference;
	int					m_animPrefDefault;

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

	//render track
	CFireRangeObjectList      * m_visibilityTrackList;//if not null then active
	
	//spawn initializers
	CAlterUnitDBObjectList    * m_alternateConfigurations;
	int                       * m_spawnCfg;  

	bool                        m_hasPerPolyCollisionsTemp;

	//base sound
	BOOL                        m_sndVelocityBased;
	int                         m_sndID;
    float                       m_clipRadius;
	float                       m_soundFactor;
	float                       m_startFrequency;
	float                       m_sndVelocityMaxDist;
	CSoundSampleObj           * m_soundRef;
	CSkeletalLODObjectList    * m_lodRanges;//internal lod used by some independent systems

	// Real-time spring-damper jiggle physics state (boobs & butt)
	BOOL                        m_enableJigglePhysics;
	// bone jiggle (BoneJiggleApply): bones named boob/breast/butt lag on a spring
	int                         m_bjCount;      // -1 = not looked for yet
	int                         m_bjBone[4];
	BOOL                        m_bjInit[4];
	float                       m_bjPos[4][3];
	float                       m_bjVel[4][3];
	float                       m_bjMaxOff[4];
	D3DMATRIX                   m_bjSaved[4];
	DWORD                       m_bjTick;
	void BoneJiggleApply();
	void BoneJiggleRestore();
	BOOL                        m_bjHaveWorld, m_bjHadWorld;
	float                       m_bjWorldY, m_bjLastY, m_bjLastVy;
	BOOL GiveTopsJiggle(CString & report);   // copy jiggle weights to every other top in the slot
	BOOL                        m_jiggleBoundsValid;
	float                       m_jiggleMinX;
	float                       m_jiggleMaxX;
	float                       m_jiggleMinY;
	float                       m_jiggleMaxY;
	float                       m_jiggleMinZ;
	float                       m_jiggleMaxZ;
	DWORD                       m_jiggleLastTick;
	int                         m_jiggleLastAnimType;
	float                       m_jiggleLastBoneX;
	float                       m_jiggleLastBoneY;
	float                       m_jiggleLastBoneZ;
	float                       m_jiggleBustPosY;
	float                       m_jiggleBustVelY;
	float                       m_jiggleBustPosX;
	float                       m_jiggleBustVelX;
	float                       m_jiggleBustPosZ;
	float                       m_jiggleBustVelZ;
	float                       m_jiggleButtPosY;
	float                       m_jiggleButtVelY;
	float                       m_jiggleButtPosX;
	float                       m_jiggleButtVelX;
	float                       m_jigglePhase;


    //functions
	void InitBasicDeformables(LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL MergeAsAltCfg(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL MergeAsAltCfg2(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void Invalidate();
	BOOL InvertNormals();
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
	 BOOL AnimationCallback(DWORD currentTime,
		                          D3DMATRIX & earCenter,
								  D3DVECTOR & wldSpacePos,
								  float globalSoundFactor,
								  BOOL soundOnly,
								  CSoundBankList * m_soundManager,
								  BOOL disableSoundOption,
								  BOOL & disableDeformbleCalc,
								  LPDIRECT3DDEVICE9 g_pd3dDevice,
								  D3DVECTOR & velocityVect,
								  int & controlType,
								  int & lodOverride);
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
	BOOL HotSwapDim(int deformableIndex,int newCfg,CSkeletonObject * targetRuntimeEntity,LPDIRECT3DDEVICE9 g_pd3dDevice,BOOL changeBaseCfg);
	 void HandleSoundRangesForAnimation(CBoneAnimationObject * animPtr,
											  D3DVECTOR & wldSpacePos,
											  float & globalSoundFactor,
											  CSoundBankList * m_soundManager,
											  BOOL secondary,
											  D3DVECTOR & sndVelocityVect);
	 int GetDimCount(int index);
	 void UpdateAnimMS(CBoneAnimationObject * animPtr,DWORD currentTime,BOOL secondary);
	 void ApplyFreeLook();
	 void RemoveFreeLook();
	 void RemoveWaisteControl();
	 void ApplyWaisteControl();
	 void ApplySecondaryAdjust(int secondaryAdjustBone);
	 void RemoveSecondaryAdjust(int secondaryAdjustBone);
	 void MoveBoneMangement(DWORD & currentTime,
								  long & millisecondsPerFrame,
								  CBoneObject * bonePtr,
				                  int  & locAnimInUse,
			                      int  & locanimationFrame,
								  BOOL & update,
								  long & overtimeReturn,
								  BOOL & secondaryInUse,
								  BOOL & newFrame
								 );
	int GetAnimationBasedOnAttribAndPref(int attrib,
										 int versionPref,
										 CBoneAnimationObject ** returnPtr);
	BOOL QuerryBox();
	ABBOX QuerryBoxExact(D3DMATRIX * compoundMatrix);//takes a longer time, dont use in a realtime scenario
	void ResetSkeletalNormalsDeformable(LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL BuildHiarchy();
	void GenerateTangents(LPDIRECT3DDEVICE9 g_pd3dDevice,CDeformableVisObj * deformPtr);
	BOOL FadeOut(long duration);
	BOOL FadeIn(long duration);
	BOOL ResetMaterial();
	BOOL SetOverrideTrans(float percentage);
	BOOL FadeInAmplified(long duration);
	BOOL FadeOutAmplified(long duration);
	void NormalizeAllNormals(LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL ScaleSystem(float percentage,LPDIRECT3DDEVICE9 g_pd3dDevice);
	CFrameObj * GetBoneFrameByIndex(int index);
	void Reinit(LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL TransformSystem(D3DMATRIX scaleMatrix,LPDIRECT3DDEVICE9 g_pd3dDevice,BOOL rotationOnly);
	int  GetAnimationAttribByIndex(int anim);
	void SkeletalTextureReassignment(CTextureDBObjList * textureDatabase,CTextureDBObjList * m_dynamicTextureDataBase);
	void SkeletalTextureReassignmentTemp(int tempChannel,CTextureDBObjList * textureDatabase);
	void GenerateTangents(LPDIRECT3DDEVICE9 g_pd3dDevice);
    int  GetBoneIndexByName(CString & boneName);
	CBoneObject * GetBoneByIndex(int & index);
	BOOL LoadLod(CString fileName,int configIndex);
	void UpdateBone(CBoneObject * bonePtr,
								 CBoneAnimationObject* animPtr,
								 long & currentOvertime,
								 DWORD & currentTime,
								 BOOL & deactivate,
								 int & locAnimInUse,
			                     int & locanimationFrame,
								 long & millisecondsPerFrame,
								 BOOL & updateBone,
								 BOOL & secondaryInUse,
								 BOOL & secondaryEverUsed,
								 BOOL & secondOffsetInitialized,
								 BOOL & updatePrimary,
								 BOOL & updateSecondary,
								 long & primaryOvertime,
								 long & secondaryOvertime,
								 long & tempMiliPerFrame,
								 CBoneAnimationObject* secondaryAnim);
	void Clone(CSkeletonObject ** clone,
							LPDIRECT3DDEVICE9 g_pd3dDevice,
							int * spawnCfgOptional,
							int copyAnimations);
	BOOL MergeAsLod(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice);
    BOOL MergeAsLod(CString fileName,CString ext,LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL LoadLODFRomDir(CString directory,LPDIRECT3DDEVICE9 device);
	BOOL LoadFullSkeletalFromDirectory(CString path,CString directory,LPDIRECT3DDEVICE9 device);
	BOOL Visible();
	void RemoveLODFromChain(int index);
	void RemoveLodsByStruct(int strCount,LODOptCondition * lodProcArray);
	BOOL GetBoneMatrixByName(CString & boneName,D3DMATRIX & outPut);
	BOOL GetBoneMatrixByIndex(int & index,D3DMATRIX & outPut);
	void DeleteUneededServerObjects();
	BOOL ExportToObj(CString fileName);
	BOOL ComputeSkinBounds(float &outMinX, float &outMaxX, float &outMinY, float &outMaxY, float &outMinZ, float &outMaxZ);
	void SculptFemaleDeformableMesh(CDeformableMesh *defMesh, float scaleFactor, float minX, float maxX, float minY, float maxY, float minZ, float maxZ);
	void GenerateFemaleBodyVariations(int headSlotIndex, BOOL sculptExistingConfigs, float existingScaleFactor);

	// ------------------------------------------------------------------
	// Female anatomy bones (R_boob / L_boob / R_butt / L_butt)
	//
	// Female skeletons get four extra helper bones: the breasts are parented to
	// Bip01 Spine3 (Bip01 Spine2 when a rig has no Spine3) and the glutes to
	// Bip01 Pelvis.  The bones carry NO animation keys of their own (0 bytes of
	// animation keyframe RAM) - MoveBoneMangement() drives their frame matrix
	// from the parent bone, so they stay locked to the chest / pelvis through
	// every animation instead of collapsing their vertices to the origin.
	// ------------------------------------------------------------------
	BOOL EnsureFemaleAnatomyBones(CString spineParentName = "Bip01 Spine3",
	                              CString pelvisParentName = "Bip01 Pelvis");
	bool BoneHasAnimationFrame(CBoneObject * bonePtr, int animationIndex);
	void ApplyParentDrivenBoneFrame(CBoneObject * bonePtr);
	CBoneObject * GetAnatomyParentBone(CBoneObject * bonePtr);
	// Rewrites the vertex assignments of one deformable visual unit so the
	// vertices listed in targetBone[] / targetWeight[] additionally ride the
	// female anatomy bones.  Handles the engine's two skinning flavours
	// (m_blendedType == 0 rigid and m_blendedType == 1 weighted) and always
	// keeps the weights normalised so the rest pose is bit-identical
	// (M_world * inverse(M_rest) == identity for every bone -> no deformation).
	int  BindFemaleAnatomyWeightsOnVisObj(CDeformableVisObj * defVis,
	                                      const int * targetBone,
	                                      const float * targetWeight,
	                                      const float * restX,
	                                      const float * restY,
	                                      const float * restZ,
	                                      const float * restNX,
	                                      const float * restNY,
	                                      const float * restNZ);

	// ------------------------------------------------------------------
	// 1-click auto rig (EngineX "Load Entity" button)
	//
	// Takes a mesh freshly imported by CAssimpModelImporter (FBX / glTF / glb /
	// OBJ / Collada / 3DS), auto-scales & aligns it to this character skeleton,
	// ensures the female anatomy bones when isFemale is set, computes 4 bone
	// inverse-distance skin weights (m_blendedType == 1) with bone local
	// offsetVects, sculpts it when isFemale and attaches the resulting
	// CDeformableMesh to m_alternateConfigurations (slot 0).
	// ------------------------------------------------------------------
	BOOL AutoRigMeshToSkeleton(CEXMeshObj * sourceMesh,
	                           BOOL isFemale,
	                           BOOL isArmorVariation,
	                           LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL FitImportedMeshToSkeleton(CEXMeshObj * mesh);
	CDeformableVisObj * BuildAutoRigSkinWeights(CEXMeshObj * mesh);
	int  AttachDeformableMeshToSlot(CDeformableMesh * defMesh, BOOL isArmorVariation);
	BOOL ExportToGltf(CString fileName, CStringArray & textureNames, CString & error, BOOL allOutfits = FALSE); // for Blender: bones + skinned parts
	BOOL ImportFromGltf(CString fileName, LPDIRECT3DDEVICE9 g_pd3dDevice, CString & report); // back from Blender onto this skeleton
};
