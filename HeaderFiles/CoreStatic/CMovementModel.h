#pragma once

//#include "CWldObject.h"
#include "CPhysicsEmuClass.h"
#include <mmsystem.h>
#include "dsound.h"
#include "dplay.h"
#include "CFrameObj.h"
#include "CSpawnClass.h"
#include "CLightTrackerClass.h"
#include "CSkeletalClass.h"
#include "CSkeletalLODClass.h"
#include "CArmedInventoryClass.h"
#include "CCharacterDefinitionsClass.h"
#include "CResourceClass.h"
#include "CInventoryClass.h"
#include "AIclass.h"
#include "CItemClass.h"
#include "CCfgClass.h"
#include "CSkillClass.h"
#include "CStatClass.h"
#include "CTextRenderClass.h"
#include "collision_raysystem.h"

struct lodRangeBasis
{
    float min, max;
    float lodSteps[64];
	int lodStepCount;
};

//object list used in main class  to define hiarch of objects and animations
//data set for entity typpe varial
class   CMovementObj : public CObject
{
    DECLARE_SERIAL( CMovementObj )
   
public:
	CMovementObj(){m_collisionRaySys=NULL;m_hudLinkThirdPerson=-1;m_selectionEffectState = -1;m_optionalTextMesh=0;m_renderBodyFilter = 0;}
	~CMovementObj();
	
	CMovementObj(CString EntityName, 
				 int traceNumber);

 
	             BOOL                   m_filterLodBalancing;
	             int                    m_uniqueId;
				 BOOL                   m_tempEntity;
	             int                    m_currentLOD;
	             CSkeletalLODObjectList * List_skeletonLODList;
	             int                    List_hudLink;
				 int                    m_hudLinkThirdPerson;
                 CString                List_networkTextString;
                 int                    List_networkPopoutCount;
				 float                  List_networkFreeLookCurPitchLast;
			     float                  List_networkFreeLookCurYawLast;
				 float                  List_networkFreeLookCurPitchTarget;
			     float                  List_networkFreeLookCurYawTarget;
	             long                   List_networkDefinedID;
	             int                    List_networkAnimReq;
				 int                    List_networkAnimDirReq;
	             int                    List_networkWeaponChangeQue;
	             int                    List_deathTimeVarial;
	             int                    List_deathCam;
				 int                    List_camFirstPerson;
				 int                    List_camThirdPerson;
                 int                    List_firstPersonRef;
				 int                    List_possesedBy;
	             BOOL                   List_networkFocus;
	             D3DVECTOR              List_networkLastDirVect;
                 D3DVECTOR              List_networkTargetDirVect;
				 D3DVECTOR              List_networkLastUpVect;
                 D3DVECTOR              List_networkTargetUpVect; 
	             D3DVECTOR              List_networkTargetPositionXYZ;
	             D3DVECTOR              List_networkLastPositionXYZ;
				 float                  List_networkRotateX;
				 float                  List_networkRotateY;
	             int                    List_fullModelRef;
	             BOOL                   List_firstPersonModel;
	             int                    List_miscInt;
	             float                  List_miscFloat;
	             //int                    List_modelType;
				 DWORD                  List_networkInterpCurStep;
				 int                    List_networkInterpCurStepOri;
	             int                    List_energy;
				 int                    List_maxEnergy;
	             BOOL                   List_networkFireWeaponQue;
	             BOOL                   List_networkEntity;
	             int                    List_curAnimPlayDir;
	             int                    List_curAnimAttrib;
	             DPID                   List_idFromLoc;
				 BOOL                   List_isPlaying;
				 float                  List_validRange;
                 CString                List_collisionInfoFile;
	             int                    List_collisionInfoCount;
	             //CPolyObj              *List_collisionInfo;
	             float                  List_popoutRadius;
				 BOOL                   List_poppedOut;
	             BOOL                   List_exactCollision;
	             int                    List_AIconfig;
	             int                    List_controlConfig;
	             D3DMATRIX              m_matrix;
	             int                    List_refModel;
	             float                  List_speedBasedAnimation;
                 float                  List_boundingRadius;
                 D3DVECTOR              List_lastPosition;
                 
	             int                    List_traceNumber;
	             CString                List_name;
				 CString				List_clanName;
				 CArmedInventoryList   *List_armedInventoryDB;
				 CSkeletonObject *      List_skeletalObject;
		         CFrameObj*             List_baseFrame;
				 int                    List_controlType;
				 CMovementCaps *        List_movementDB;
				 D3DVECTOR              List_location;
				 CPolyObj              *List_collisionInfoBuffer;
				 D3DVECTOR              List_AIdestination;
				 BOOL                   List_currentlyFireing;
				 D3DVECTOR              List_AI_origin;
				 BOOL                   List_AI_reachedDestination;
				 BOOL                   List_AI_currentlyHittingWall;
				 int                    List_AI_target;//-1 none other = tracenumber of enemy
                 CMovementObj          *List_targetRef;//target Ref pointer
				 DWORD                  List_NetworkUpdateTime;
				 DWORD                  List_NetworkInterpolateVar;
				 int                    List_SpawnGeneratorLink;//used for teeling spawner that is can spawn another -1 = not a spawn controled entity
                 BOOL                   List_noInterpolationOverride;
				 D3DVECTOR              List_externalVectorTemp;
				 int                    List_attackerTrace;//-1 = none
				 int                    m_cameraTypeInUse;//-1;//none
				 BOOL                   m_firstPersonMode;
				 CResourceObjectList   *m_resources;
				 CInventoryObjList     *m_inventory;
				 CAIObj                *m_aiObj;
				 D3DVECTOR              m_recentSufaceNormal;
				 D3DVECTOR              m_directionalVector;
				 int                    m_ridable;
				 CMovementObj          *m_posses;
				 CMovementObj          *m_possedBy;
				 int                    m_team;
				 CItemObjList          *m_itemDB;
				 CCfgObjList           *m_autoCfg;
				 CCfgObjList           *m_deathCfg;
				 CSkillObjectList      *m_skills;
				 D3DVECTOR              m_networkLastPositionSent;
				 BOOL                   m_networkFirstTimeSendingPos;
				 CStatObjList          *m_stats;
				 int                    m_raceID;
				 int                    m_showShadow;//0=off 1= on and possible type
				 CTextureRenderObj     *m_textureRenderObject;
				 CTextureRenderObj     *m_textureRenderObject2;
				 BOOL                   m_validThisRound;
				 BOOL					m_GotHit;
				 int					m_level;
				 int					m_curXP;
				 int					m_nextLvlXP;
				 int					m_percXP;
				 CString				m_lastTextArea;
				 

				 int                    m_ovverrideAnimation;
				 long                   m_ovverrideStamp;
				 long                   m_ovverrideDuration;
				 BOOL                   m_filterCollision;
				 BOOL                   m_murderer;
				 DWORD                  m_spawnTimeStamp;
				 //---------------dyna
				 DWORD                  m_lifeTimeMax;//used to time out dynamicly spawned AI
				 int                    m_dynaSpawnID;//if this id isnt -1 is valid-demands to be unique
				 int                    m_clanMember;
				 int				    m_groupMember;
				 bool					m_isAI;
                 int                    m_primaryPhysicsOverd;
				 CCollision_RaySystem * m_collisionRaySys;
				 int                    m_collisionType;//1=normal 2=per poly


				 //------------------temp
				 short                  m_curHitCounter;
				 float                  m_distanceToCamTemp;
				 float                  m_hardPointOverdY;
				 int                    m_networkClanID;
				 int                    m_aiCfgRef;
				 char                   m_autoTriggerTemp;
				 char                   m_animationsDone;
				 short                  m_tempChannel;
				 int                    m_selectionEffectState;
				 char					m_renderBodyFilter;
				 
				 CEXMeshObj            *m_optionalTextMesh;
				 


				 CCollisionObj         *m_collisionMovRefPtr;
                 CCollisionObj         *m_collisionVisionRefPtr;

				 //CHRIS HACK
				 //CHRONICLE DEMO ENERGY
				 float					m_energyCurrent;
				 float					m_energyMax;
				 float					m_energyRate;

				 //Multiple missile overrides
				 char					m_fireQueIndexOverride;
				 int					m_missileLoaded;

				 //Animation attack override
				 int					m_attackAnimOverride;
				 int					m_firstPersonAnimOverride;
				 char					m_quickSlotNumber;

				 //wound and dots
				 char					m_woundChannel[8];
				 char					m_goodDots[8];
				 char					m_badDots[8];

				 // Arenas
				 bool					m_hasJoinedArena;
				 int					m_arenaBotID;

				 //Badges
				 int				   *m_badges;

				 //Faction info
				 BOOL					m_factionBonus1;
			     BOOL					m_factionBonus2;
				 BOOL					m_factionBonus3;
				 int					m_curFaction;
  
    BOOL GetFireTransform(CString boneName,CArmedInventoryObj * armedPtr, D3DMATRIX & output);
    void ApplyLODProcessing(int removeFrontLods);
	void ApplyLODProcessing(int strCount,LODOptCondition * lodProcArray, int rangeCount, lodRangeBasis * lodRange);
    BOOL DeMount();
	CMovementObj * GetObjByControlType(int ctype);
	void LoadFromDir(CString path);
	CItemObj * GetItemByID(int gl_id);
	BOOL FadeOut(long duration);
	BOOL FadeIn(long duration);
	BOOL ResetMaterial();
	BOOL SetOverrideTrans(float percentage);
    BOOL Mount(CMovementObj * target);
    void CharacterAnimationManagement(CSkeletonObject * skeletalFocus,float adjustFactor);
    BOOL GenerateCollisionPolys();
	int  QuerryLOD(float & distance);
	void Serialize( CArchive& ar );
	//void SafeDelete();
	BOOL Clone(CMovementObj ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice,int * spawnCfg);
	
	BOOL ApplyStatisticalData(CMovementObj ** target);
	BOOL ArmAttachment( int type,
						int fireQueOvrd,
					    CArmedInventoryList * globalArmedInvenList,
					    LPDIRECT3DDEVICE9 g_pd3dDevice,
					    int locationID,
					    CMovementObj * refDBEntity);
    BOOL DisarmAttachment(int type,
						  LPDIRECT3DDEVICE9 g_pd3dDevice,
						  CMovementObj * refDBEntity);
	int DisarmItem(CItemObj * itemPtr,BOOL override,CMovementObj * refDBObj,LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL ExecCfg(CCfgObjList * cfg,CMovementObj * refDBObj,void * itemManagerMem,LPDIRECT3DDEVICE9 g_pd3dDevice);
	int ArmItemByGLID(int gl_id,CMovementObj * refDBObj,void * itemManagerMem,LPDIRECT3DDEVICE9 g_pd3dDevice);
    int DisarmItemByGLID(int gl_id,BOOL override,CMovementObj * refDBObj,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void SetSecondaryTimingIntro(CSkeletonObject * skeletalFocus,long & currentTime);
    int ArmNextLocationId(int locationID,CMovementObj * refDBObj,void * itemManagerMem,LPDIRECT3DDEVICE9 g_pd3dDevice);
	CString GetFileExt(CString currentName);
	BOOL LoadFromDir(CString path,CString directory,LPDIRECT3DDEVICE9 device,BOOL resetBaseSkeleton);
	BOOL LoadLODFRomDir(CString directory,LPDIRECT3DDEVICE9 device);
	void ApplyForce(D3DVECTOR forceOrigin,float force);
	void ApplyForce(D3DVECTOR forceOrigin,float force,float range);
	void ChangeMissileIndexByLoc(int firequeIndex, int locationID, int missileIndex);
	CFireRangeObject * GetNextActiveFireQue(int & optIndexRet,CArmedInventoryObj * armedPtr, BOOL & firstPersonMode);
	BOOL GetFireTransform(int boneIndex,CArmedInventoryObj * armedPtr, D3DMATRIX & output);


};

struct cmovmMemObj
{
   char m_inUse;
   POSITION m_posInList;
   CMovementObj * m_mPtr;
};

class   CMovementObjList : public CObList
{
public:
   CMovementObjList()
   {
	   m_idStartIndex=1;
       m_directAccessArray = NULL;
	   m_directAccessArrayNID = NULL;
	   m_directAccessCount = 0;
	   m_lodScriptHasBeenExecuted = 0;
	   m_texturesHaveBeenInitialized = 0;
	   m_doAdjust = FALSE;
       m_tempDist = 0;
       m_angle = 0.0f;
       m_iLoop = 0;
       m_mCount = 0;
	   m_dist = 0.0f;
       m_additionalDist = 0.0f;
   };
   ~CMovementObjList()
   {
       if(m_directAccessArray)
	   {
          delete [] m_directAccessArray;
		  m_directAccessArray = 0;
	   }
	   if(m_directAccessArrayNID)
	   {
          delete [] m_directAccessArrayNID;
		  m_directAccessArrayNID = 0;
	   }
   };

   cmovmMemObj * m_directAccessArray;
   cmovmMemObj * m_directAccessArrayNID;
   int           m_directAccessCount;
   char          m_lodScriptHasBeenExecuted;
   char          m_texturesHaveBeenInitialized;
   //SegmentToEntity() temp variables (helps avoid allot of reallocation on critical used functions-ussually rendering/collision)
   //this is also thread safe because this list object would not be shared between threads on ai server, in world manager one of 
   //these is created for that thread to manage entities in that zone. Additionally this brings the functionality of entity cmp to 
   //skeletal anywhere the list object is used.
   D3DVECTOR		m_virtualNormal;
   D3DVECTOR		m_invVirtualNormal;
   D3DVECTOR		m_v1;
   D3DVECTOR		m_v2;
   D3DVECTOR		m_normalTemp;
   D3DVECTOR		m_invlegRayStart;
   D3DVECTOR		m_invlegRayEnd;
   D3DVECTOR		m_intersect;
   D3DMATRIX		m_wldBoneMatrix;
   D3DMATRIX		m_invBoneMatrix;
   D3DVECTOR		m_polygon[3];
   BOOL				m_doAdjust;
   float			m_tempDist;
   float			m_angle;
   int				m_iLoop;
   int				m_mCount;
   //DoExactPerPolyCollisionProcess() temp variables
   D3DVECTOR		m_offset;
   D3DVECTOR		m_position;
   D3DVECTOR		m_latestNormal;
   D3DVECTOR		m_projectedPos;
   D3DVECTOR		m_tempIntersect;
   D3DVECTOR		m_dirtemp;
   float			m_dist;
   float			m_additionalDist;

   CMovementObj * GetObjByTrace(int trace);
   CMovementObj * GetObjByNetworkID(int id);
   CMovementObj * GetObjByIndex(int index);
   CMovementObj * GetItemByNetworkID(int id);
   CMovementObj * GetObjByName(CString name);
   CMovementObj * GetObjectByDynaID(int id);
   CMovementObj * GetObjByControlType(int ctype);
   CFrameObj * GetFrameByTraceAlive(int trace, int filterTrace);
   CMovementObj * GetByTraceAlive(int trace, int filterTrace);
   int GetPhysicalIndexByUniqueID(int uniqueID);
   int GetObjectCountByDynaID(int id);
   void ExportToSQL();
   BOOL ExportToSQL(CString ip, CString database, CString usename, CString password);
   BOOL RebuildIds(int offset);
   void ClearTempEntities();
   BOOL MergeAsTempEntities(CMovementObjList * tempEntitiesDB);
   CMovementObj * GetByID(int uniqueID);
   int GetFreeID();
   int GetNextHighID();
   void SafeDelete();

   int m_idStartIndex;
   //Functions
   

   //indexed management
   void EnableDirectAccess(int maxID);
   POSITION AddTail_Indexed(CObject * newElement);
   POSITION AddHead_Indexed(CObject * newElement);
   BOOL DeleteByPtr(CMovementObj * mPtr);
   void SetNetID(CMovementObj * mPtr, int netID);
   BOOL DeleteByID(int id);
   void NotifySpawnList(CSpawnObjList * spawnList,CMovementObj * mPtr);
   BOOL InvalidateIndexedByID(CMovementObj * mPtr);
   void TransitionItemAttachmentID(int currentAttachmentID,int newAttachmentID);
   void ApplyLODProcessing(int removeFrontLods);
   void ApplyLODProcessing(int strCount,LODOptCondition * lodProcArray,int rangeCount,lodRangeBasis * lodRange);
   void ApplyLODProcessing(CString strLodScript);
   BOOL SegmentToEntity(	D3DVECTOR & startPoint,
							D3DVECTOR & endPoint,
							CSkeletonObject * skeletalCmpare,
							MatrixARB * mathLib,
							D3DVECTOR * intersectRet,
							D3DMATRIX * boneMatrix,
							CFrameObj ** frameObject,
							D3DMATRIX & compareBaseMatrix,
							D3DVECTOR & latestNormal,
							float & dist);
   void DoExactPerPolyCollisionProcess(CMovementObj * entityFocus,
									   CSkeletonObject * skeletalCompare,
									   bool & correctiveOnly,
									   D3DVECTOR & intersection,
                                       D3DMATRIX & boneMatrix,
                                       CFrameObj ** framePtr,
									   MatrixARB * mathLib,
									   D3DMATRIX & matrixEntityCmp,
									   int & traceOption);
   CMovementObj * GetByName(CString name);
   void ApplyForce(D3DVECTOR forceOrigin,float force);
   void ApplyForce(D3DVECTOR forceOrigin,float force,float range);
   BOOL SegmentToEntityDistPersistence( D3DVECTOR & startPoint,
										D3DVECTOR & endPoint,
										CSkeletonObject * skeletalCmpare,
										MatrixARB * mathLib,
										D3DVECTOR * intersectRet,
										D3DMATRIX & compareBaseMatrix,
										D3DVECTOR & latestNormal,
										D3DMATRIX * boneMatrix,
										float & dist
									);
   void Serialize( CArchive& ar );
   void ApplyFemaleVariationsAndJigglePhysics();



   DECLARE_SERIAL( CMovementObjList )
};




