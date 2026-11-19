#include "stdafx.h"

#include <d3d9.h>
#include <afxtempl.h>
#include "AIclass.h"
#include "CABFunctionLib.h"
#include "dbconnectionsystem.h"
#include "ItemManager.h"

#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAIObj::CAIObj()
{
	m_sightChecked = FALSE;
	m_sightClear   = FALSE;

	m_curExperienceLevel = 0.0f;
	List_team = 0;
	List_energyToRetreat = 40;
	List_blitzAttack = FALSE;
	List_takeCover = FALSE;
	List_closeEnoughDist = 100;
	List_turnTolerance = 10;
	List_targetFrame = NULL;
	List_sightType = 0;
	List_sightRandVariable = 100;
	List_maxSightRadius = 1500;
	List_difficulty = 2;
	List_walkingType = TRUE;
	List_startAttackZone = 200;
	List_startAttackDist = 300;
	List_randChillTime = 10;
	List_pathfindingMethod = 0;
	List_randKamakazi = 2;
	List_AreaToRoam = 0.0f;
	m_steeringModel = 0;
	m_strikeShootRange = 50;

	m_currentMissileSpeed = 1.0f;

	m_condition = 0;//0=nothin  1=under attack  
	m_desiredGoal = 0;//0=not sure  1=roam 2=defense 3=retreat 4=kill
	m_lastKnownEnergy = -9999;

	//speed overides
	m_normalSpeed = 20.0f;
	m_hussleSpeed = 30.0f;
	m_turnSpeed = 1.5f;

	//new map tree
	m_routePointCount        = 0;
	m_currentRoute           = NULL;
	m_routeCurPos            = 0;
	m_curRadius              = 0.0f;
	m_wldPos.x               = 0.0f;
	m_wldPos.y               = 0.0f;
	m_wldPos.z               = 0.0f;
	m_mapCurLocation         = 0;
	m_curMap                 = -1;

	//scripts
	m_curScript              = NULL;
	m_spawnScript            = -1;
	m_lookAt.x               = 0.0f;
	m_lookAt.y               = 0.0f;
	m_lookAt.z               = 0.0f;

	m_feelerSys              = NULL;
	m_leftBlocked            = FALSE;
	m_rightBlocked           = FALSE;

	m_floorTolerance         = .7f;
	m_wallMinimumHieght      = .3f;
	m_feelerRange            = 10.0f;
	m_feelerDepth            = -10.0f;
	m_feelerCycleTime        = 300;
	m_feelerWidth            = 3.0f;
	m_feelerHieght           = 2.0f;
	m_feelerEnable           = FALSE;
	m_aiCfgName              = "Bot Cfg";
	m_useOverridePhysics     = FALSE;
	m_safeDistance           = 3000.0f;
	m_minAttackDistance      = 100.0f;
	m_percentageAttack       = 25;
	m_accuracy               = 50;
	m_exponentTurning        = TRUE;
	m_controlled             = FALSE;
	m_travelingDesiredDist   = 2.0f;
	m_needToDefend           = FALSE;
	m_engageRadius           = 100.0f;
	m_currentSpeedOverride   = 0.0f;
	m_attackDelayStamp       = 0;
	m_delayOfAttacks         = 3000;
	m_followMaxDistance      = 100;//100
	m_followMinDistance      = 20;//20
	m_distanceQuerryDuration = 2000;
	m_distanceQuerryStamp    = 0;
	m_attackMaxDistance      = 150;//150
	m_attackMinDistance      = 20;//20
	m_eyeLevel               = 7.0f;
	m_sightCheckDuration     = 500;
	m_sightCheckStamp        = 0;
	m_targetInSight          = TRUE;
	m_enableVision           = TRUE;
	m_timeStampWhenTargetNotInSight = GetTickCount();
	m_durationTillTargetLost = 30000;

	m_attackDurationMax              = 3000;//suto holding trigger down ;)
	m_attackDurationMin              = 1000;//suto holding trigger down ;)
	m_attackTimeBetweenMax           = 7000;//suto time before holding trigger down again
	m_attackTimeBetweenMin           = 1000;//suto time before holding trigger down again
	m_attackingCurrently             = FALSE;//currently holding trigger down
	m_attackStamp                    = 0;//last action stamp ;)
	m_attackDurationCurrent          = 500;//current round decision
	m_attackTimeBetweenCurrent       = 0;//current round decision

	m_enableJumpAbility              = TRUE;

	m_accuracyErrorMax               = 6.0f;
	m_accuracyErrorMin               = 2.0f;

	m_prefernceAboveTarget           = 0.0f;
	m_agressive                      = TRUE;
	m_agressiveRange                 = 100.0f;

	m_enableCashApplied              = TRUE;
	m_maxCash                        = 100;
	m_minCash                        = 10;
	m_routeCurTarget                 = 0;

	m_neverAttacks                   = FALSE;
	//radius system
	m_radiusSysEnabled               = FALSE;
	m_radiusSysInUse                 = FALSE;
	m_radiusSysCenter.x              = 0.0f;
	m_radiusSysCenter.y              = 0.0f;
	m_radiusSysCenter.z              = 0.0f;
	m_radiusSysAttackOnExtents       = FALSE;
	m_radiusSysManditoryReturn       = FALSE;
	m_radiusSysLastRoutePosition     = 0;
	m_radiusSysRange                 = 500.0f;
	m_radiusSysPriorDest.x           = 0.0f;
	m_radiusSysPriorDest.y           = 0.0f;
	m_radiusSysPriorDest.z           = 0.0f;
	m_radiusSysIgnoreY               = TRUE;
	m_dbIndex                        = 0;

	m_nameOvveride                   = "NAME";
	m_inventoryCfg                   = NULL;
	m_spawnCfg                       = NULL;
	m_spawnCfgCount                  = 0;
	m_entityCfgDBindexRef            = 0;
	m_physicsModuleOverride          = NULL;
	m_percentageOfDropping           = 100;
	m_overriderMaxEngery             = 0;
	m_collisionDBOverride            = -1;

	m_retreatStamp							= GetTickCount();
	m_retreating							= FALSE;

	m_tamable								= FALSE;
	m_requiredTamePoints					= 0;
	m_friendPreferredDist					= 0.0f;
	m_lockedOnPath							= FALSE;
	m_movePosThoughtDuration				= 1000;
	m_attackTargetThoughtDuration			= 1000;
	m_timoutThoughtDuration					= 1000;
	m_movePosThoughtDurationStamp			= GetTickCount();
	m_attackTargetThoughtDurationStamp		= GetTickCount();
	m_timoutThoughtDurationStamp			= GetTickCount();
	m_aiFireState							= 0;
	m_aiMovementState						= 0;

	m_friendTargetID                        = -1;
	m_friendsLastEnergy                     = -1;
	m_friendsAttackerID                     = -1;
	m_potentialFriendID                     = -1;
	m_currentFriendPoints                   = 0;
	m_requireTreeIncrement                  = 0;
	m_friendFrame							= NULL;
	m_switchTargetPercentage                = 10;

	m_bodyCollision							= FALSE;
	ZeroMemory(&m_bodyCollisonVectLast,sizeof(D3DVECTOR));
	m_blockCount                            = 0;

	m_beginningScripts						= NULL;
	m_randomScripts							= NULL;
	m_setScripts							= NULL;
	m_endingScripts							= NULL;
	m_activeScripts							= NULL;
}

void CAIObj::Clone(CAIObj ** clone)
{
	CAIObj * copyLocal = new CAIObj();

	copyLocal->m_strikeShootRange			   = m_strikeShootRange;
	copyLocal->List_team                       = List_team;
	copyLocal->List_energyToRetreat            = List_energyToRetreat;
	copyLocal->List_blitzAttack                = List_blitzAttack;
	copyLocal->List_takeCover                  = List_takeCover;
	copyLocal->List_closeEnoughDist            = List_closeEnoughDist;
	copyLocal->List_turnTolerance              = List_turnTolerance;
	copyLocal->List_sightType                  = List_sightType;
	copyLocal->List_sightRandVariable          = List_sightRandVariable;
	copyLocal->List_maxSightRadius             = List_maxSightRadius;
	copyLocal->List_difficulty                 = List_difficulty;
	copyLocal->List_walkingType                = List_walkingType;
	copyLocal->List_startAttackZone            = List_startAttackZone;
	copyLocal->List_startAttackDist            = List_startAttackDist;
	copyLocal->List_randChillTime              = List_randChillTime;
	copyLocal->List_pathfindingMethod          = List_pathfindingMethod;
	copyLocal->List_randKamakazi               = List_randKamakazi;
	copyLocal->List_AreaToRoam                 = List_AreaToRoam;
	copyLocal->m_condition                     = m_condition;//0=nothin  1=under attack  
	copyLocal->m_desiredGoal                   = m_desiredGoal;//0=not sure  1=roam 2=defense 3=retreat 4=kill
	copyLocal->m_lastKnownEnergy               = m_lastKnownEnergy;
	copyLocal->m_normalSpeed                   = m_normalSpeed;
	copyLocal->m_hussleSpeed                   = m_hussleSpeed;
	copyLocal->m_turnSpeed                     = m_turnSpeed;
	copyLocal->m_currentMissileSpeed           = 1.0f;
	copyLocal->m_curScript                     = NULL;
	copyLocal->m_spawnScript                   = m_spawnScript;
	copyLocal->m_floorTolerance                = m_floorTolerance;
	copyLocal->m_wallMinimumHieght             = m_wallMinimumHieght;
	copyLocal->m_feelerRange                   = m_feelerRange;
	copyLocal->m_feelerDepth                   = m_feelerDepth;
	copyLocal->m_feelerCycleTime               = m_feelerCycleTime;
	copyLocal->m_feelerWidth                   = m_feelerWidth;
	copyLocal->m_feelerHieght                  = m_feelerHieght;
	copyLocal->m_feelerEnable                  = m_feelerEnable;
	copyLocal->m_aiCfgName                     = m_aiCfgName;
	copyLocal->m_useOverridePhysics            = m_useOverridePhysics;
	copyLocal->m_safeDistance                  = m_safeDistance;
	copyLocal->m_minAttackDistance             = m_minAttackDistance;
	copyLocal->m_percentageAttack              = m_percentageAttack;
	copyLocal->m_accuracy                      = m_accuracy;
	copyLocal->m_exponentTurning               = m_exponentTurning;
	copyLocal->m_controlled                    = m_controlled;
	copyLocal->m_travelingDesiredDist          = m_travelingDesiredDist;
	copyLocal->m_needToDefend                  = m_needToDefend;
	copyLocal->m_delayOfAttacks                = m_delayOfAttacks;
	copyLocal->m_engageRadius                  = m_engageRadius;
	copyLocal->m_currentSpeedOverride          = m_currentSpeedOverride;
	copyLocal->m_travelingDesiredDist          = List_closeEnoughDist;
	copyLocal->m_followMaxDistance             = m_followMaxDistance;
	copyLocal->m_followMinDistance             = m_followMinDistance;
	copyLocal->m_distanceQuerryDuration        = m_distanceQuerryDuration;
	copyLocal->m_distanceQuerryStamp           = m_distanceQuerryStamp;
	copyLocal->m_attackMaxDistance             = m_attackMaxDistance;
	copyLocal->m_attackMinDistance             = m_attackMinDistance;
	copyLocal->m_eyeLevel                      = m_eyeLevel;
	copyLocal->m_sightCheckDuration            = m_sightCheckDuration;
	copyLocal->m_enableVision                  = m_enableVision;
	copyLocal->m_timeStampWhenTargetNotInSight = m_timeStampWhenTargetNotInSight;
	copyLocal->m_durationTillTargetLost        = m_durationTillTargetLost;
	copyLocal->m_attackDurationMax             = m_attackDurationMax;//suto holding trigger down ;)
	copyLocal->m_attackDurationMin             = m_attackDurationMin;//suto holding trigger down ;)
	copyLocal->m_attackTimeBetweenMax          = m_attackTimeBetweenMax;//suto time before holding trigger down again
	copyLocal->m_attackTimeBetweenMin          = m_attackTimeBetweenMin;//suto time before holding trigger down again
	copyLocal->m_enableJumpAbility             = m_enableJumpAbility;
	copyLocal->m_accuracyErrorMax              = m_accuracyErrorMax;
	copyLocal->m_accuracyErrorMin              = m_accuracyErrorMax;
	copyLocal->m_prefernceAboveTarget          = m_prefernceAboveTarget;
	copyLocal->m_agressive                     = m_agressive;
	copyLocal->m_enableCashApplied             = m_enableCashApplied;
	copyLocal->m_maxCash                       = m_maxCash;
	copyLocal->m_minCash                       = m_minCash;
	copyLocal->m_routeCurTarget                = m_routeCurTarget;
	copyLocal->m_radiusSysEnabled              = m_radiusSysEnabled;
	copyLocal->m_radiusSysAttackOnExtents      = m_radiusSysAttackOnExtents;
	copyLocal->m_radiusSysRange                = m_radiusSysRange;
	copyLocal->m_dbIndex                       = m_dbIndex;


	copyLocal->m_spawnCfgCount                 = m_spawnCfgCount;
	copyLocal->m_nameOvveride                  = m_nameOvveride;
	copyLocal->m_entityCfgDBindexRef           = m_entityCfgDBindexRef;
	copyLocal->m_percentageOfDropping          = m_percentageOfDropping;
	copyLocal->m_curExperienceLevel            = m_curExperienceLevel;
	copyLocal->m_overriderMaxEngery            = m_overriderMaxEngery;
	copyLocal->m_collisionDBOverride           = m_collisionDBOverride;
	copyLocal->m_steeringModel				   = m_steeringModel;

	copyLocal->m_aiFireState				   = m_aiFireState;
	copyLocal->m_aiMovementState			   = m_aiMovementState;

	copyLocal->m_tamable					   = m_tamable;
	copyLocal->m_requiredTamePoints			   = m_requiredTamePoints;
	copyLocal->m_friendPreferredDist		   = m_friendPreferredDist;
	copyLocal->m_lockedOnPath				   = m_lockedOnPath;
	copyLocal->m_aiFireState				   = m_aiFireState;
	copyLocal->m_aiMovementState			   = m_aiMovementState;
	copyLocal->m_switchTargetPercentage		   = m_switchTargetPercentage;
	copyLocal->m_agressiveRange				   = m_agressiveRange;

	if(m_inventoryCfg)
		m_inventoryCfg->Clone(&copyLocal->m_inventoryCfg);

	if(m_beginningScripts)
		m_beginningScripts->Clone(&copyLocal->m_beginningScripts);
	
	if(m_randomScripts)
		m_randomScripts->Clone(&copyLocal->m_randomScripts);
	
	if(m_setScripts)
		m_setScripts->Clone(&copyLocal->m_setScripts);
	
	if(m_endingScripts)
		m_endingScripts->Clone(&copyLocal->m_endingScripts);
	
	if(m_activeScripts)
		m_activeScripts->Clone(&copyLocal->m_activeScripts);

	if(m_physicsModuleOverride)
		m_physicsModuleOverride->Clone(&copyLocal->m_physicsModuleOverride);

	if(m_spawnCfgCount > 0)
	{
		copyLocal->m_spawnCfg = new int[m_spawnCfgCount];
		CopyMemory(copyLocal->m_spawnCfg,m_spawnCfg,sizeof(int)*m_spawnCfgCount);
	}

	copyLocal->List_targetFrame = new CFrameObj(NULL);
	*clone = copyLocal;
}

void CAIObj::SafeDelete()
{
	if(m_feelerSys)
	{
		delete m_feelerSys;
		m_feelerSys=0;
	}

	if(List_targetFrame)
	{//cannot delete it will crash
		//List_targetFrame->SafeDelete();
		//delete List_targetFrame;
		List_targetFrame=0;
	}

	if(m_currentRoute)
	{
		delete [] m_currentRoute;
		m_currentRoute = 0;
	}

	if(m_physicsModuleOverride)
	{
		m_physicsModuleOverride->SafeDelete();
		delete m_physicsModuleOverride;
		m_physicsModuleOverride = 0;
	}

	if(m_spawnCfg)
	{
		delete [] m_spawnCfg;
		m_spawnCfg = 0;
	}

	if(m_inventoryCfg)
	{
		m_inventoryCfg->SafeDelete();
		delete m_inventoryCfg;
		m_inventoryCfg = 0;
	}

	if(m_curScript)
	{
		m_curScript->SafeDelete();
		delete m_curScript;
		m_curScript = 0;
	}

	if(m_beginningScripts)
	{
		m_beginningScripts->SafeDelete();
		delete m_beginningScripts;
		m_beginningScripts = 0;
	}

	if(m_randomScripts)
	{
		m_randomScripts->SafeDelete();
		delete m_randomScripts;
		m_randomScripts = 0;
	}

	if(m_setScripts)
	{
		m_setScripts->SafeDelete();
		delete m_setScripts;
		m_setScripts = 0;
	}
	
	if(m_endingScripts)
	{
		m_endingScripts->SafeDelete();
		delete m_endingScripts;
		m_endingScripts = 0;
	}

	if(m_activeScripts)
	{
		m_activeScripts->SafeDelete();
		delete m_activeScripts;
		m_activeScripts = 0;
	}
}

void CAIObj::Serialize( CArchive& ar )
{	
	BOOL reserved = 0;

	CObject::Serialize(ar);
	if(ar.IsStoring())
	{
		ar.SetObjectSchema(3);
		ASSERT(ar.GetObjectSchema() == 3);

		ar << List_team
		   << List_energyToRetreat
		   << List_blitzAttack
		   << List_takeCover
	       << List_closeEnoughDist
	       << List_turnTolerance
	       << List_sightType
		   << List_sightRandVariable
		   << List_maxSightRadius
		   << List_difficulty
		   << List_walkingType
		   << List_startAttackZone
		   << List_startAttackDist
           << List_randChillTime
		   << List_pathfindingMethod
		   << List_randKamakazi
		   << List_AreaToRoam
		   << m_condition
		   << m_desiredGoal
		   << m_lastKnownEnergy
           << m_currentMissileSpeed
		   << m_normalSpeed
		   << m_hussleSpeed
		   << m_turnSpeed
		   << m_routePointCount
		   << m_curRadius
		   << m_mapCurLocation
		   << m_curMap
		   << m_aiCfgName
		   << m_spawnScript
		   << m_floorTolerance
		   << m_wallMinimumHieght
		   << m_feelerRange
		   << m_feelerDepth
		   << m_feelerCycleTime
		   << m_feelerWidth
		   << m_feelerHieght
		   << m_feelerEnable
		   << m_useOverridePhysics
		   << m_safeDistance
		   << m_minAttackDistance
		   << m_percentageAttack
		   << m_accuracy
		   << m_exponentTurning
		   << m_travelingDesiredDist
		   << m_followMaxDistance   
           << m_followMinDistance   
		   << m_distanceQuerryDuration 
		   << m_attackMaxDistance     
           << m_attackMinDistance     
		   << m_eyeLevel               
		   << m_sightCheckDuration     
           << m_targetInSight         
		   << m_enableVision           
           << m_durationTillTargetLost
		   << m_attackDurationMax      
           << m_attackDurationMin    
           << m_attackTimeBetweenMax  
           << m_attackTimeBetweenMin 
           << m_attackingCurrently    
		   << m_attackDurationCurrent  
           << m_attackTimeBetweenCurrent 
		   << m_enableJumpAbility     
		   << m_accuracyErrorMax       
           << m_accuracyErrorMin    
		   << m_prefernceAboveTarget
		   << m_agressive
		   << m_enableCashApplied
           << m_maxCash
           << m_minCash
		   << m_radiusSysEnabled
		   << m_radiusSysAttackOnExtents
		   << m_radiusSysIgnoreY
		   << m_radiusSysRange
	       << m_physicsModuleOverride
           << m_nameOvveride
		   << m_spawnCfgCount
		   << m_entityCfgDBindexRef
           << m_inventoryCfg
		   << m_percentageOfDropping
		   << m_curExperienceLevel
		   << m_overriderMaxEngery
		   << m_collisionDBOverride;
          		  
		for(int loop=0;loop<m_spawnCfgCount;loop++)
			ar << m_spawnCfg[loop];
		       
		ar <<  m_steeringModel
		   <<  m_switchTargetPercentage
		   <<  m_lockedOnPath
		   <<  m_agressiveRange
		   <<  m_strikeShootRange
		   <<  reserved;

		//Save out the scripts
		ar << m_beginningScripts
		   << m_randomScripts
		   << m_setScripts
		   << m_endingScripts;
	}
	else
	{
		int version = ar.GetObjectSchema();

		ar >> List_team
		   >> List_energyToRetreat
		   >> List_blitzAttack
		   >> List_takeCover
	       >> List_closeEnoughDist
	       >> List_turnTolerance
	       >> List_sightType
		   >> List_sightRandVariable
		   >> List_maxSightRadius
		   >> List_difficulty
		   >> List_walkingType
		   >> List_startAttackZone
		   >> List_startAttackDist
           >> List_randChillTime
		   >> List_pathfindingMethod
		   >> List_randKamakazi
		   >> List_AreaToRoam
		   >> m_condition
		   >> m_desiredGoal
		   >> m_lastKnownEnergy
           >> m_currentMissileSpeed
		   >> m_normalSpeed
		   >> m_hussleSpeed
		   >> m_turnSpeed
		   >> m_routePointCount
		   >> m_curRadius
		   >> m_mapCurLocation
		   >> m_curMap
		   >> m_aiCfgName
		   >> m_spawnScript
		   >> m_floorTolerance
		   >> m_wallMinimumHieght
		   >> m_feelerRange
		   >> m_feelerDepth
		   >> m_feelerCycleTime
		   >> m_feelerWidth
		   >> m_feelerHieght
		   >> m_feelerEnable
		   >> m_useOverridePhysics
		   >> m_safeDistance
		   >> m_minAttackDistance
		   >> m_percentageAttack
		   >> m_accuracy
		   >> m_exponentTurning
		   >> m_travelingDesiredDist
		   >> m_followMaxDistance   
           >> m_followMinDistance   
		   >> m_distanceQuerryDuration 
		   >> m_attackMaxDistance     
           >> m_attackMinDistance     
		   >> m_eyeLevel               
		   >> m_sightCheckDuration     
           >> m_targetInSight         
		   >> m_enableVision           
           >> m_durationTillTargetLost
		   >> m_attackDurationMax      
           >> m_attackDurationMin    
           >> m_attackTimeBetweenMax  
           >> m_attackTimeBetweenMin 
           >> m_attackingCurrently    
		   >> m_attackDurationCurrent  
           >> m_attackTimeBetweenCurrent 
		   >> m_enableJumpAbility     
		   >> m_accuracyErrorMax       
           >> m_accuracyErrorMin    
		   >> m_prefernceAboveTarget
		   >> m_agressive
		   >> m_enableCashApplied
           >> m_maxCash
           >> m_minCash
		   >> m_radiusSysEnabled
		   >> m_radiusSysAttackOnExtents
		   >> m_radiusSysIgnoreY
		   >> m_radiusSysRange
		   >> m_physicsModuleOverride
           >> m_nameOvveride
		   >> m_spawnCfgCount
		   >> m_entityCfgDBindexRef
           >> m_inventoryCfg
		   >> m_percentageOfDropping
		   >> m_curExperienceLevel
		   >> m_overriderMaxEngery
		   >> m_collisionDBOverride;          
		  
		if(m_spawnCfgCount > 0)
			m_spawnCfg = new int[m_spawnCfgCount];			  

		for(int loop=0;loop<m_spawnCfgCount;loop++)
			ar >> m_spawnCfg[loop];

		m_steeringModel = 0;
		m_switchTargetPercentage = 10;
		m_agressiveRange = 0.0f;

		if(version == 2)
		{
			ar >>  m_steeringModel
			   >>  m_switchTargetPercentage
			   >>  m_lockedOnPath
			   >>  m_agressiveRange
			   >>  m_strikeShootRange
			   >>  reserved;
		}

		if(version == 3)
		{
			ar >>  m_steeringModel
			   >>  m_switchTargetPercentage
			   >>  m_lockedOnPath
			   >>  m_agressiveRange
			   >>  m_strikeShootRange
			   >>  reserved
			   >>  m_beginningScripts
			   >>  m_randomScripts
			   >>  m_setScripts
			   >>  m_endingScripts;

			m_activeScripts = NULL;
		}

		if(m_agressiveRange == 0.0f)
			m_agressiveRange = 100.0f;

		if(m_strikeShootRange == 0.0f)
			m_strikeShootRange = 50.0f;

		m_retreatStamp                   = GetTickCount();
		m_retreating                     = FALSE;

		List_targetFrame				 = 0;
		m_needToDefend					 = FALSE;
		m_controlled					 = FALSE;
		m_currentMissileSpeed			 = 1.0f;
		m_delayOfAttacks				 = 3000;//three seconds
		m_engageRadius					 = 100;
		m_currentSpeedOverride			 = 0.0f;

		m_attackStamp					 = 0;//last action stamp ;)
		m_timeStampWhenTargetNotInSight  = 0;
		m_sightCheckStamp				 = 0;
		m_attackDelayStamp				 = 0;
		m_distanceQuerryStamp			 = 0;

		m_attackingCurrently			 = 0;
		m_attackTimeBetweenCurrent		 = 0;
		m_routeCurTarget				 = 0;

		m_radiusSysInUse                 = FALSE;
		m_radiusSysCenter.x              = 0.0f;
		m_radiusSysCenter.y              = 0.0f;
		m_radiusSysCenter.z              = 0.0f;
		m_radiusSysManditoryReturn       = FALSE;
		m_radiusSysLastRoutePosition     = 0;
		m_radiusSysPriorDest.x           = 0.0f;
		m_radiusSysPriorDest.y           = 0.0f;
		m_radiusSysPriorDest.z           = 0.0f;
		m_aiFireState					 = 0;
		m_aiMovementState				 = 0;
		m_friendTargetID                 = -1;
		m_friendsLastEnergy              = -1;
		m_friendsAttackerID              = -1;
		m_potentialFriendID              = -1;
		m_currentFriendPoints            = 0;
		m_requireTreeIncrement           = 0;
		m_dbIndex						 = 0;
		m_switchTargetPercentage         = 10;	  
	}
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
 BOOL CAIObj::RadiusSysAllowMovement(D3DVECTOR wldSpcDestination,
									 D3DVECTOR & newDestination,
									 int       & targetTraceRef)
{
	if(!m_radiusSysEnabled)
		return TRUE;

	if(!m_radiusSysInUse)
		return TRUE;

	float distance = 0.0f;
	float dummyZero = 0.0f;
   
	if(!m_radiusSysIgnoreY)
		distance = GL_matrixFunctions.MagnitudeSquared(m_radiusSysCenter.x,m_radiusSysCenter.y,m_radiusSysCenter.z,wldSpcDestination.x,wldSpcDestination.y,wldSpcDestination.z);
	else
		distance = GL_matrixFunctions.MagnitudeSquared(m_radiusSysCenter.x,dummyZero,m_radiusSysCenter.z,wldSpcDestination.x,dummyZero,wldSpcDestination.z);

	BOOL modeShift = FALSE;
   
	if(distance < m_radiusSysRange && m_radiusSysManditoryReturn == FALSE)
	{//this is a good
		return TRUE;
	}//end this is a good
	else if(m_radiusSysAttackOnExtents == FALSE && m_radiusSysManditoryReturn == FALSE)
	{//target is out of extents return to center
		//reset destination to
		m_radiusSysManditoryReturn   = TRUE;
		newDestination.x             = m_radiusSysCenter.x;
		newDestination.y             = m_radiusSysCenter.y;
		newDestination.z             = m_radiusSysCenter.z;
		m_lookAt.x                   = m_radiusSysCenter.x;
		m_lookAt.y                   = m_radiusSysCenter.y;
		m_lookAt.z                   = m_radiusSysCenter.z;

		//nullify targets
		targetTraceRef               = -1;

		return FALSE;
	}
	else if(m_radiusSysManditoryReturn == TRUE)
	{
		//when you get back to manditory becan set previous target then return to route
		if(distance <= List_closeEnoughDist)
		{
			newDestination.x           = m_radiusSysPriorDest.x;
			newDestination.y           = m_radiusSysPriorDest.y;
			newDestination.z           = m_radiusSysPriorDest.z;
			m_lookAt.x                 = m_radiusSysPriorDest.x;
			m_lookAt.y                 = m_radiusSysPriorDest.y;
			m_lookAt.z                 = m_radiusSysPriorDest.z;

			m_radiusSysInUse           = FALSE;
			m_radiusSysManditoryReturn = FALSE;
			targetTraceRef             = -1; 
		}
		return TRUE;
	}

   return FALSE;
}
//---------------------------------------------------------//
//-------------Enable Radius system------------------------//
//---------------------------------------------------------//
 BOOL CAIObj::EnableRadiusSystem(D3DVECTOR & currentPosition,
								 int         curRouteDestNode,
								 D3DVECTOR & currentDestination)
{
	if(!m_radiusSysEnabled)
		return FALSE;

	if(m_radiusSysInUse)
		return FALSE;//even if target changes dont move pivot

	m_radiusSysInUse                = TRUE;
	m_radiusSysLastRoutePosition    = curRouteDestNode;

	if(currentDestination.x != 0.0f || currentDestination.y != 0.0f || currentDestination.z != 0.0f)
		CopyMemory(&m_radiusSysPriorDest,&currentDestination,sizeof(D3DVECTOR));
	else
		CopyMemory(&m_radiusSysPriorDest,&currentPosition,sizeof(D3DVECTOR));

	CopyMemory(&m_radiusSysCenter,&currentPosition,sizeof(D3DVECTOR));

	return TRUE;
}
//---------------------------------------------------------//
//-------------Feeler system construction------------------//
//---------------------------------------------------------//
BOOL CAIObj::CheckVisionIsClear(D3DVECTOR startPoint,
						        D3DVECTOR endPoint,
						        CCollisionObj * m_collisionBase,
								int * polysExactChecked,
								int * polysConsidered)
{
	D3DVECTOR intersect,normal;
	int objectHit;

	if(m_collisionBase->CollisionSegmentSI(endPoint, startPoint, &intersect.x, &intersect.y, &intersect.z,
										   polysExactChecked, polysConsidered, &normal, &objectHit, 50.0f, TRUE) == TRUE)
	{
		return FALSE;
	}
	return TRUE;
}
//---------------------------------------------------------//
void CAIObj::ResetVisionIsClearX()
{
	m_sightClear = FALSE;
	m_sightChecked = FALSE;
}
//---------------------------------------------------------//
BOOL CAIObj::CheckVisionIsClearX(D3DVECTOR & startPoint,
						         D3DVECTOR & endPoint,
						         CCollisionObj * m_collisionBase)
{
	if(m_sightChecked)
		return m_sightClear;

	if(m_collisionBase->CollisionSegmentSIBasic(startPoint,endPoint,50.0f))
		m_sightClear = FALSE;
	else
		m_sightClear = TRUE;

	return m_sightClear;
}
//---------------------------------------------------------//
//-------------Feeler system construction------------------//
//---------------------------------------------------------//
void CAIObj::FeelerSystemContruction(float distance,
									 float depth,
									 long cycleTime,
									 float width,
									 float hieght)
{
	if(m_feelerSys)
	{
		delete m_feelerSys;
		m_feelerSys=0;
	}

	m_feelerSys = new FeelerSystem;
	ZeroMemory(m_feelerSys,sizeof(FeelerSystem));
	m_feelerSys->m_cycleTime = cycleTime;
	m_feelerSys->m_status = FALSE;
	//poly 0 North

    m_feelerSys->m_startPoint.x           =  0.1f;
	m_feelerSys->m_startPoint.y           =  hieght;
	m_feelerSys->m_startPoint.z           =  0.0f;
	m_feelerSys->m_endPoint.x             =  -0.1f;
	m_feelerSys->m_endPoint.y             =  hieght;
	m_feelerSys->m_endPoint.z             = -distance;


	m_feelerSys->m_incrementOnWall        = 3.5f;

	m_feelerSys->m_wallDirection          = 0;//default right

    ZeroMemory(&m_feelerSys->m_wallFeelerStartPoint,sizeof(D3DVECTOR));
	ZeroMemory(&m_feelerSys->m_wallFeelerEndPoint,sizeof(D3DVECTOR));

	m_feelerSys->m_mostRecentWallNormal.x = 0.0f;
	m_feelerSys->m_mostRecentWallNormal.y = 0.0f;
	m_feelerSys->m_mostRecentWallNormal.z = 1.0f;
}
//---------------------------------------------------------//
//-------------Feeler status aquisition--------------------//
//---------------------------------------------------------//
BOOL CAIObj::UpdateFeelerStatus(CCollisionObj * collisionSys,
								D3DMATRIX positionMatrix,
								float floorTolerance,
								float wallMinHieght,
								float currentYpos,
								CMenuTextBox * debug,
                                BOOL m_reachDestination,
								D3DVECTOR currentAIDestinationRef)
{
	if(!m_feelerSys)
		return FALSE;

	int objectColided = 0;
	int polysExactChecked,polysConsidered;

	D3DVECTOR startPos      = GL_matrixFunctions.UpdatePolygonVertex(positionMatrix.m,m_feelerSys->m_startPoint);
	D3DVECTOR endPos        = GL_matrixFunctions.UpdatePolygonVertex(positionMatrix.m,m_feelerSys->m_endPoint);

	D3DVECTOR origin;
	origin.x = positionMatrix._41;
	origin.y = positionMatrix._42;
	origin.z = positionMatrix._43;
        
	if(m_feelerSys->m_onWallCurrently == TRUE)
	{//wall checker
		BOOL onWallStill = FALSE;
		BOOL floorStillhere = FALSE;
		//update wall vector based on position change ;)))))))))))))
		D3DVECTOR offsetPosVector;
		offsetPosVector.x = startPos.x - m_feelerSys->m_wallFeelerStartPoint.x;
		offsetPosVector.y = startPos.y - m_feelerSys->m_wallFeelerStartPoint.y;
		offsetPosVector.z = startPos.z - m_feelerSys->m_wallFeelerStartPoint.z;

		m_feelerSys->m_wallFeelerStartPoint.x += offsetPosVector.x;
		m_feelerSys->m_wallFeelerStartPoint.y += offsetPosVector.y;
		m_feelerSys->m_wallFeelerStartPoint.z += offsetPosVector.z;
		m_feelerSys->m_wallFeelerEndPoint.x   += offsetPosVector.x;
		m_feelerSys->m_wallFeelerEndPoint.y   += offsetPosVector.y;
		m_feelerSys->m_wallFeelerEndPoint.z   += offsetPosVector.z;

		D3DVECTOR normalDump;
		collisionSys->CollisionSegmentSIValidPath(m_feelerSys->m_wallFeelerStartPoint,
												  m_feelerSys->m_wallFeelerEndPoint,
												  &polysExactChecked,
												  &polysConsidered,
												  3,
												  m_floorTolerance,
												  m_wallMinimumHieght,
												  currentYpos,
												  origin,
												  &onWallStill,
												  &floorStillhere,
												  &normalDump);
		if(!onWallStill)
			m_feelerSys->m_onWallCurrently = FALSE;
	}//end wall checker
		
	BOOL wallInWay   = FALSE;
	BOOL floorExists = FALSE;

	collisionSys->CollisionSegmentSIValidPath(startPos,
											  endPos,
											  &polysExactChecked,
											  &polysConsidered,
											  3,
											  m_floorTolerance,
											  m_wallMinimumHieght,
											  currentYpos,
											  origin,
											  &wallInWay,
											  &floorExists,
											  &m_feelerSys->m_mostRecentWallNormal);

	if(wallInWay == TRUE/* || floorExists == FALSE*/)
	{
		m_feelerSys->m_status = TRUE;
		//set Up wall hook
		if(!m_feelerSys->m_onWallCurrently)
		{//hook onto wall
			m_feelerSys->m_onWallCurrently = TRUE;
			//copy currentFeeler to wall feeler
			CopyMemory(&m_feelerSys->m_wallFeelerStartPoint,&startPos,sizeof(D3DVECTOR));
			CopyMemory(&m_feelerSys->m_wallFeelerEndPoint  ,&endPos,sizeof(D3DVECTOR));
			//create matrix
			D3DMATRIX genOriMatrix;
			D3DVECTOR tempUpVect,tempDirVector;
			tempUpVect.x = 0.0f;
			tempUpVect.y = 1.0f;
			tempUpVect.z = 0.0f;
			tempDirVector.x = m_feelerSys->m_mostRecentWallNormal.x;
			tempDirVector.y = 0.0f;
			tempDirVector.z = m_feelerSys->m_mostRecentWallNormal.z;
			tempDirVector = GL_matrixFunctions.Normalize(tempDirVector);
			GL_matrixFunctions.InitMat(genOriMatrix.m);
			GL_matrixFunctions.SetOrientation(&genOriMatrix,tempDirVector,tempUpVect);
			genOriMatrix._41 = positionMatrix._41;
			genOriMatrix._42 = positionMatrix._42;
			genOriMatrix._43 = positionMatrix._43;
			//select logical progression
			D3DVECTOR localDestRef = GL_matrixFunctions.GetLocalPositionFromMatrixView(genOriMatrix,currentAIDestinationRef);
			if(localDestRef.x < 0)
				m_feelerSys->m_wallDirection = 0;//right
			else   
				m_feelerSys->m_wallDirection = 1;//left
		}//end hook onto wall
	}
	else
		m_feelerSys->m_status = FALSE;
       		
	return TRUE;
}
//---------------------------------------------------------//
//--------Get Update Poly non modify orig------------------//
//---------------------------------------------------------//
void CAIObj::UpdatePolygonTransform(D3DMATRIX posMatrix,D3DVECTOR polygonLoc[3],D3DVECTOR outPut[3])
{
	outPut[0].x = (posMatrix.m[0][0]*polygonLoc[0].x)+(posMatrix.m[1][0]*polygonLoc[0].y)+(posMatrix.m[2][0]*polygonLoc[0].z)+(posMatrix.m[3][0] * 1);
	outPut[0].y = (posMatrix.m[0][1]*polygonLoc[0].x)+(posMatrix.m[1][1]*polygonLoc[0].y)+(posMatrix.m[2][1]*polygonLoc[0].z)+(posMatrix.m[3][1] * 1);
	outPut[0].z = (posMatrix.m[0][2]*polygonLoc[0].x)+(posMatrix.m[1][2]*polygonLoc[0].y)+(posMatrix.m[2][2]*polygonLoc[0].z)+(posMatrix.m[3][2] * 1);

	outPut[1].x = (posMatrix.m[0][0]*polygonLoc[1].x)+(posMatrix.m[1][0]*polygonLoc[1].y)+(posMatrix.m[2][0]*polygonLoc[1].z)+(posMatrix.m[3][0] * 1);
	outPut[1].y = (posMatrix.m[0][1]*polygonLoc[1].x)+(posMatrix.m[1][1]*polygonLoc[1].y)+(posMatrix.m[2][1]*polygonLoc[1].z)+(posMatrix.m[3][1] * 1);
	outPut[1].z = (posMatrix.m[0][2]*polygonLoc[1].x)+(posMatrix.m[1][2]*polygonLoc[1].y)+(posMatrix.m[2][2]*polygonLoc[1].z)+(posMatrix.m[3][2] * 1);

	outPut[2].x = (posMatrix.m[0][0]*polygonLoc[2].x)+(posMatrix.m[1][0]*polygonLoc[2].y)+(posMatrix.m[2][0]*polygonLoc[2].z)+(posMatrix.m[3][0] * 1);
	outPut[2].y = (posMatrix.m[0][1]*polygonLoc[2].x)+(posMatrix.m[1][1]*polygonLoc[2].y)+(posMatrix.m[2][1]*polygonLoc[2].z)+(posMatrix.m[3][1] * 1);
	outPut[2].z = (posMatrix.m[0][2]*polygonLoc[2].x)+(posMatrix.m[1][2]*polygonLoc[2].y)+(posMatrix.m[2][2]*polygonLoc[2].z)+(posMatrix.m[3][2] * 1);
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CAIObjList::ResetIndexReferences()
{
	int trace = 0;
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		CAIObj * aiPtr = (CAIObj *)GetNext(posLoc);
		aiPtr->m_dbIndex = trace;
		trace++;
	}
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
CAIObj * CAIObjList::GetByIndex(int index)
{
	if(index < 0)
		return NULL;

	POSITION posLoc = FindIndex(index);
	if(!posLoc)
		return NULL;

	CAIObj * aiPtr = (CAIObj *)GetAt(posLoc);
	return aiPtr;
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CAIObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
		CAIObj * delPtr = (CAIObj *)GetNext(posLoc);
		delPtr->SafeDelete();
		delete delPtr;
		delPtr = 0;
	}
	RemoveAll();
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CAIObjList::RefreshData(void * itemManagerMem)
{
	CItemManager * iManager = (CItemManager *)itemManagerMem;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{//AI object loop
		CAIObj * aiPtr = (CAIObj *)GetNext(posLoc);

		if(aiPtr->m_inventoryCfg)
		{//valid inventory
			for(POSITION aiInvPos = aiPtr->m_inventoryCfg->GetHeadPosition();aiInvPos != NULL;)
			{//inventory loop
				CInventoryObj * invObj = (CInventoryObj *)aiPtr->m_inventoryCfg->GetNext(aiInvPos);

				CItemObj * iPtr = iManager->GetItemByID(invObj->m_itemData->m_globalID);
				//CGlobalInventoryObj * glPtr = globalInvList->GetByGLID(invObj->m_itemData->m_globalID);
				if(iPtr)
				{
					invObj->m_itemData->SafeDelete();
					delete invObj->m_itemData;
					invObj->m_itemData = 0;
					iPtr->Clone(&invObj->m_itemData);
				}
			}//End inventory loop
		}//End valid inventory
	}//End AI object loop
}

void CAIObjList::ExportToSQL()
{
	ConnectionStructureClass connectionStructTemp;
	connectionStructTemp.AquireFromCfgFile("baseAccount.cfg");

	CDBConnectionSystem * dbConnection = new CDBConnectionSystem(connectionStructTemp.m_sqlServerIP,
																 connectionStructTemp.m_sqlServerCatalog,
																 connectionStructTemp.m_sqlServerUsername,
																 connectionStructTemp.m_sqlServerPassword,
																 NULL,
																 15);


	if(!dbConnection->m_connectionEstablished)
	{
		//if(eventLog)
		//  eventLog->AddEvent("ExportToSQL() Aborted due to connection not being established");
		return;
	}

	CString clearFromGil		 = "DELETE FROM GM_AII";
	CString addObject            = "INSERT INTO GM_AII (AIGENERATORNAME, AIDISPLAYEDNAME, GENINDEX) Values('$AIGENERATORNAME','$AIDISPLAYEDNAME','$GENINDEX')";

	CABFunctionLib cablib;
	CString temp;
	dbConnection->ExecQuery(clearFromGil);

	int trace = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CAIObj * aiObj  = (CAIObj *)GetNext(posLoc);
        temp = addObject;
		temp.Replace("$AIGENERATORNAME",aiObj->m_aiCfgName);
		temp.Replace("$AIDISPLAYEDNAME",aiObj->m_nameOvveride);
		temp.Replace("$GENINDEX",cablib.ConvToString(trace));
		dbConnection->ExecQuery(temp);
		trace++;
	}

	delete dbConnection;
	dbConnection = 0;
}

BOOL CAIObjList::ExportToSQL(CString ip, CString database, CString usename, CString password)
{	
	CDBConnectionSystem * dbConnection = new CDBConnectionSystem(ip, database, usename, password, NULL,15);

	if(!dbConnection->m_connectionEstablished)
	{
		//if(eventLog)
		//  eventLog->AddEvent("ExportToSQL() Aborted due to connection not being established");
		return FALSE;
	}

	CString clearFromGil		 = "DELETE FROM GM_AII";
	CString addObject            = "INSERT INTO GM_AII (AIGENERATORNAME, AIDISPLAYEDNAME, GENINDEX) Values('$AIGENERATORNAME','$AIDISPLAYEDNAME','$GENINDEX')";

	CABFunctionLib cablib;
	CString temp;
	dbConnection->ExecQuery(clearFromGil);

	int trace = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
		CAIObj * aiObj  = (CAIObj *)GetNext(posLoc);		
		temp = addObject;
		temp.Replace("$AIGENERATORNAME",aiObj->m_aiCfgName);
		temp.Replace("$AIDISPLAYEDNAME",aiObj->m_nameOvveride);
		temp.Replace("$GENINDEX",cablib.ConvToString(trace));
		dbConnection->ExecQuery(temp);
		trace++;
	}

	delete dbConnection;
	dbConnection = 0;
	return TRUE;
}

IMPLEMENT_SERIAL(CAIObj, CObject, VERSIONABLE_SCHEMA|3)
IMPLEMENT_SERIAL(CAIObjList, CObList, 0)