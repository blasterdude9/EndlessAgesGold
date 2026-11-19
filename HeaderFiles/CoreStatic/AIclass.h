
#include "CFrameObj.h"
#include "CAIWEBCLASS.h"
#include "CAIScriptClass.h"
#include "CAILogicScriptClass.h"
#include "CCollisionClass.h"
#include "matrixarb.h"
#include "CMenuTextBox.h"
#include "CInventoryClass.h"
#include "CPhysicsEmuClass.h"
#include "PathTracker.h"

#ifndef AICLASS_H
#define AICLASS_H

struct FeelerSystem
{ 
    D3DVECTOR     m_startPoint;
	D3DVECTOR     m_endPoint;
	BOOL          m_status;

	D3DVECTOR     m_wallFeelerStartPoint;
	D3DVECTOR     m_wallFeelerEndPoint;
    int           m_wallDirection;//0=right 1=left
	D3DVECTOR     m_mostRecentWallNormal;

	long          m_cycleTime;
	long          m_stamp;
	BOOL          m_onWallCurrently;

	float         m_incrementOnWall;
};



class CAIObj : public CObject
{
	//CAIObj(){}
	
     
    DECLARE_SERIAL( CAIObj )
    
    
public:
	
	CAIObj();

	MatrixARB         GL_matrixFunctions;
	int               List_team;//what allies does the bot have
	int               List_energyToRetreat;//at what energy does the Bot start to retreat at
	BOOL              List_blitzAttack;
	BOOL              List_takeCover;
	float             List_closeEnoughDist;
	float             List_turnTolerance;
	CFrameObj*        List_targetFrame;
	int               List_sightType;
	int               List_sightRandVariable;
	float             List_maxSightRadius;
	int               List_difficulty;
	BOOL              List_walkingType;
	int               List_startAttackZone;
	int               List_startAttackDist;
	int               List_randChillTime;
	int               List_pathfindingMethod;
	int               List_randKamakazi;
	float             List_AreaToRoam;
	int               m_condition;//0=nothin  1=under attack  
	int               m_desiredGoal;//0=not sure  1=roam 2=defense 3=retreat 4=kill 5=get nearest point on map
	int               m_lastKnownEnergy;
	float             m_currentMissileSpeed;
	float             m_normalSpeed;
	float             m_hussleSpeed;
	float             m_turnSpeed;
	int               m_routePointCount;
	int              *m_currentRoute;
	int               m_routeCurPos;
	float             m_curRadius;
	D3DVECTOR         m_wldPos;
	int               m_mapCurLocation;
	int               m_curMap;
	CString           m_aiCfgName;
	CAIScriptObj     *m_curScript;
	int               m_spawnScript;//spawn with script  -1 = none
	FeelerSystem     *m_feelerSys;
	BOOL              m_leftBlocked;
	BOOL              m_rightBlocked;
	float             m_floorTolerance;
	float             m_wallMinimumHieght;
	float             m_feelerRange;
	float             m_feelerDepth;
	long              m_feelerCycleTime;
	float             m_feelerWidth;
	float             m_feelerHieght;
	BOOL              m_feelerEnable;
	D3DVECTOR         m_lookAt;
	BOOL              m_useOverridePhysics;
	float             m_safeDistance;//at this distance-it has escaped or lost chase
	float             m_minAttackDistance;
	int               m_percentageAttack;//out of 100%
	int				  m_switchTargetPercentage;

	BOOL              m_exponentTurning;
	BOOL              m_controlled;
	float             m_travelingDesiredDist;
	BOOL              m_needToDefend;//if trapped fight to death
	long              m_delayOfAttacks;
	float             m_engageRadius;
	float             m_currentSpeedOverride;
	long              m_attackDelayStamp;

	float             m_followMaxDistance;
	float             m_followMinDistance;
	float             m_attackMaxDistance;
	float             m_attackMinDistance;

	long              m_attackDurationMax;//suto holding trigger down ;)
	long              m_attackDurationMin;//suto holding trigger down ;)
	long              m_attackDurationCurrent;//current round decision
	long              m_attackTimeBetweenMax;//suto time before holding trigger down again
	long              m_attackTimeBetweenMin;//suto time before holding trigger down again
	long              m_attackTimeBetweenCurrent;//current round decision
	BOOL              m_attackingCurrently;//currently holding trigger down
	long              m_attackStamp;//last action stamp ;)

	long              m_distanceQuerryDuration;
	long              m_distanceQuerryStamp;
	float             m_eyeLevel;

	long              m_sightCheckDuration;
	long              m_sightCheckStamp;
	BOOL              m_targetInSight;
	BOOL              m_enableVision;

	long              m_timeStampWhenTargetNotInSight;
	long              m_durationTillTargetLost;

	BOOL              m_enableJumpAbility;
	int               m_accuracy;//out of 100%
	float             m_accuracyErrorMax;
	float             m_accuracyErrorMin;


	float             m_prefernceAboveTarget;
	int               m_agressive;
	float             m_agressiveRange;
	float             m_strikeShootRange;


	//loot gen
	BOOL              m_enableCashApplied;
	int               m_maxCash;
	int               m_minCash;

	int               m_routeCurTarget;
	//
	BOOL              m_neverAttacks;
	//radius system
	BOOL              m_radiusSysEnabled;
	BOOL              m_radiusSysInUse;
	D3DVECTOR         m_radiusSysCenter;
	BOOL              m_radiusSysAttackOnExtents;
	BOOL              m_radiusSysManditoryReturn;
	int               m_radiusSysLastRoutePosition;
	float             m_radiusSysRange;
	D3DVECTOR         m_radiusSysPriorDest;
	BOOL              m_radiusSysIgnoreY;
	int               m_dbIndex;

	//CUSTOMIZATION
	CString           m_nameOvveride;
	CInventoryObjList*m_inventoryCfg;
	int              *m_spawnCfg;
	int               m_spawnCfgCount;
	int               m_entityCfgDBindexRef;
	CMovementCaps    *m_physicsModuleOverride;
	int               m_percentageOfDropping;
	float             m_curExperienceLevel;

	//
	int               m_overriderMaxEngery;

	int               m_collisionDBOverride;
	long              m_retreatStamp;
	BOOL              m_retreating;
	int               m_steeringModel;

	//NEW
	int				 m_aiFireState;
	int              m_aiMovementState;

	BOOL			 m_tamable;
	int				 m_requiredTamePoints;
	float			 m_friendPreferredDist;
	BOOL             m_lockedOnPath;
	DWORD		     m_movePosThoughtDuration;
	DWORD		     m_attackTargetThoughtDuration;
	DWORD			 m_timoutThoughtDuration;
	DWORD		     m_movePosThoughtDurationStamp;
	DWORD		     m_attackTargetThoughtDurationStamp;
	DWORD			 m_timoutThoughtDurationStamp;


	int              m_friendTargetID;
	int              m_friendsLastEnergy;
	int				 m_friendsAttackerID;
	int				 m_potentialFriendID;
	int				 m_currentFriendPoints;
	int				 m_requireTreeIncrement;

	BOOL             m_sightChecked;
	BOOL             m_sightClear;

	CFrameObj*       m_friendFrame;
	CPathTracker     m_pathTracker;

	BOOL             m_bodyCollision;
	D3DVECTOR        m_bodyCollisonVectLast;
	int              m_blockCount;

	//---- New Scripting ----//
	CAILogicScriptObjList	*m_beginningScripts;//Script events that will start encounter
	CAILogicScriptObjList	*m_randomScripts;//Script events that occur randomly during encounter
	CAILogicScriptObjList	*m_setScripts;//Script events that happen at specific points in encounter
	CAILogicScriptObjList	*m_endingScripts;//Script events that will end encounters
	CAILogicScriptObjList	*m_activeScripts;//Scripts that need to be run during the encounter

    //---- Functions ----//
	void ResetVisionIsClearX();
    BOOL CheckVision(D3DVECTOR startPoint,D3DVECTOR endPoint,CCollisionObj * m_collisionBase);
    BOOL UpdateFeelerStatus(CCollisionObj * collisionSys,
							D3DMATRIX positionMatrix,
							float floorTolerance,
							float wallMinHieght,
							float currentYpos,
							CMenuTextBox *debug,
							BOOL m_reachDestination,
							D3DVECTOR currentAIDestinationRef);
    void UpdatePolygonTransform(D3DMATRIX posMatrix,D3DVECTOR polygonLoc[3],D3DVECTOR outPut[3]);
	void FeelerSystemContruction(float distance, float depth, long cycleTime, float width, float hieght);
	BOOL CheckVisionIsClear(D3DVECTOR startPoint,
							D3DVECTOR endPoint,
							CCollisionObj * m_collisionBase,
							int * polysExactChecked,
							int * polysConsidered);
	 BOOL EnableRadiusSystem(D3DVECTOR &currentPosition, int curRouteDestNode, D3DVECTOR &currentDestination);
	 BOOL RadiusSysAllowMovement(D3DVECTOR wldSpcDestination, D3DVECTOR & newDestination, int &targetTraceRef);
	 BOOL CheckVisionIsClearX(D3DVECTOR &startPoint, D3DVECTOR &endPoint, CCollisionObj * m_collisionBase);
	
	void Serialize(CArchive& ar);
    void SafeDelete();
	void Clone(CAIObj ** clone);
};

class CAIObjList : public CObList
{
	public:
		CAIObjList(){};
	 
	DECLARE_SERIAL( CAIObjList )

	void RefreshData(void * itemManagerMem);
	void ResetIndexReferences();
	CAIObj * GetByIndex(int index);
	void SafeDelete();
	void ExportToSQL();
	BOOL ExportToSQL(CString ip, CString database, CString usename, CString password);
};

#endif



