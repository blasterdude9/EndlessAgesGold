#pragma once
#include "globalstatusvariables.h"
#include "usercontroller.h"
#include "matrixarb.h"


class CAiManager
{
public:
	CAiManager(GlobalStatusVariables * gStatus,UserController * userControls);
	~CAiManager(void);



	//Variables
	GlobalStatusVariables				* m_gStatus;
	UserController						* m_userControls;
	MatrixARB                             m_matLib;

    //Functions
	void AIStateMangement(CMovementObj * ptrLoc, DWORD & currentTime,CMovementObjList * entObjRuntime);
    BOOL AI_Loop(CMovementObj * ptrLoc,CAIWebObjList * ai_webDB,CWaterZoneObjList * waterZoneDB,CMovementObjList * entObjRuntime,CCollisionObj * collisionBase,float & timeAdjust,double & lastTimeAdjust);
	BOOL AI_getTarget(CMovementObj * ptrLoc,int targetRetrievalType,CMovementObjList * entObjRuntime,CCollisionObj * collisionBase);
	BOOL AI_VerifyTarget(CMovementObj * ptrLoc,CMovementObjList * entObjRuntime);
	void RemoveTarget(CMovementObj * ptrLoc);
	void EvaluateAIVisionToTargetDest(CMovementObj * ptrLoc,long & currentTime,CCollisionObj * collisionBase);
	BOOL AI_Attack(CMovementObj * ptrLoc,CAIObj * AIPtr,long & currentTime,float & adjustFactor);
	BOOL AI_Retreat(CMovementObj * ptrLoc,CAIObj * AIPtr);
	BOOL AI_TravelLookBasis(CMovementObj * ptrLoc,CAIObj * AIPtr,long currentTime,CCollisionObj * collisionBase,float & timeAdjust);
	void AI_TreeWebUse(CMovementObj * ptrLoc,CAIWebObjList * ai_webDB);
	void AI_HandleScript(CMovementObj * ptrLoc,CAIWebObjList * ai_webDB);
	BOOL AI_Transform(CMovementObj * ptrLoc,CWaterZoneObjList * m_waterZoneDB,double timeAdjust,double lastTimeAdjust);
	BOOL AI_targetInSight(CMovementObj * ptrLoc,float maxDistance,CCollisionObj * collisionBase);
	void AdvancedAiSteering(CMovementObj * ptrLoc,D3DVECTOR locationLoc,float & adjust);
	void ManagementPlatform(CMovementObj * ptrLoc, 
							D3DVECTOR & targetPosition, 
							float &speed,
							float & timeAdjust,
						    double & lastTimeAdjust);
	void MovementManager(CMovementObj * ptrLoc,CAIWebObjList * ai_webDB,float & timeAdjust,CMovementObjList * entObjRuntime,DWORD & currentTime);
	void RetreatFromAttackTargetState(CMovementObj * ptrLoc,float & timeAdjust,CMovementObjList * entObjRuntime);
	void FollowTreeState(CMovementObj * ptrLoc,CAIWebObjList * ai_webDB,float & timeAdjust);
	void FollowFriendState(CMovementObj * ptrLoc,float & timeAdjust,CMovementObjList * entObjRuntime);
	void FollowAttackTargetState(CMovementObj * ptrLoc,float & timeAdjust,CMovementObjList * entObjRuntime,DWORD & currentTime);
	void DoAIMovement(CMovementObj * ptrLoc,float & timeAdjust,double & lastTimeAdjust,CWaterZoneObjList * waterZoneDB);
	void RequireAssignedTreeState(CMovementObj * ptrLoc,CAIWebObjList * ai_webDB,float & timeAdjust,DWORD & currentTime);
	BOOL AI_LoopStateX( CMovementObj * ptrLoc,
						CAIWebObjList * ai_webDB,
						CWaterZoneObjList * waterZoneDB, 
						CMovementObjList * entObjRuntime,
						CCollisionObj * collisionBase,
						float & timeAdjust,
						double & lastTimeAdjust);
	void MaintainTarget(CMovementObj * ptrLoc,float & timeAdjust,CMovementObjList * entObjRuntime);
	void HuntState(CMovementObj * ptrLoc,CMovementObjList * entObjRuntime);
	void TargetAttacker(CMovementObj * ptrLoc,CMovementObjList * entObjRuntime);
	void ConsiderNewtarget(CMovementObj * ptrLoc,CMovementObjList * entObjRuntime);
	void AIFireManager(CMovementObj * ptrLoc,float & timeAdjust,CMovementObjList * entObjRuntime,DWORD & currentTime);
	void FireAtEnemy(CMovementObj * ptrLoc,float & timeAdjust,CMovementObjList * entObjRuntime,DWORD & currentTime);
	BOOL IsDead(CMovementObj * ptrLoc);
};
