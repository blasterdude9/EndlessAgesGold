#pragma once
#include "clientnetworkinglogic.h"
#include "globalstatusvariables.h"
#include "usercontroller.h"
#include "aimanager.h"

class CEntityManagement
{
public:
	CEntityManagement(GlobalStatusVariables * gStatus,UserController * userControls,CAiManager * aiManager,CClientNetworkingLogic * clientNet);
	~CEntityManagement(void);


    //variables
    GlobalStatusVariables		* m_gStatus;
	UserController				* m_userControls;
	CAiManager					* m_aiManager;
	CClientNetworkingLogic      * m_clientNet;
	DWORD                         m_lastAutoQuerryStamp;
	BOOL						  m_insideInteractRadius;
	BOOL						  m_flag;

	//Functions
	void UniversalControlUnit(CWaterZoneObjList * waterZoneDB);
	void CollisionLoopEntityToEntity(CMovementObjList * entObjRuntime);
	void CollisionLoopEntityWorld(CMovementObjList * entObjRuntime);
	void RealtimeEntityCollisionCallback(CMovementObjList * entObjRuntime);
	void PossesCallback(CMovementObjList * entObjRuntime);
	void GenerateEntityLastPosition(CMovementObjList * entObjRuntime);
	void EntityDamagePathCheck(CMovementObjList * entObjRuntime);
	BOOL HandleDeadEntities(DWORD CurrentTime);
	//void CollisionLoopEntityToEntityRTC(CMovementObjList * entObjRuntime);
	BOOL CollisionLoopEntityToEntityRTC(CMovementObj * entityFocus, CMovementObj * entityCmpare, MatrixARB * mathLib);
	void HandleMovCollisionToSystem( CMovementObj * entLoc,CCollisionObj * collisionSystem,D3DMATRIX * transformOfCollisionSys,D3DMATRIX * invTransformOfCollisionSys);
    BOOL MoveEntityBasedOnLock(CMovementObj * entityFocus, CMovementObjList * entObjRuntime, MatrixARB * mathLib);
	void CollisionPipeline(MatrixARB * matLib, CMovementObjList * entObjRuntime,CMissileObjList * m_missileRuntime);
	void CollisionPipelinePerEntityWld(MatrixARB * matLib, CMovementObj * entLoc,CCollisionObj * collisionObject,bool correctiveOnly);
	void TableCollisionPipelinePerEntityCore(   MatrixARB * matLib, 
		                                        CMovementObj * entLoc,
												CCollisionObj * collisionObject,
												D3DMATRIX * transformOfCollisionSys,
												D3DMATRIX * invTransformOfCollisionSys,
												bool correctiveOnly);
	/*void DoExactPerPolyCollisionProcess(CMovementObj * entityFocus,
										CSkeletonObject * skeletalCompare,
										bool & correctiveOnly,
										D3DVECTOR & intersection,
	                                    D3DMATRIX & boneMatrix,
	                                    CFrameObj ** framePtr,
										MatrixARB * mathLib,
										D3DMATRIX & matrixEntityCmp,
										int & traceOption,
										CMovementObjList * entObjRuntime);*/
	BOOL CollisionLoopEntityToEntityHybrid(CMovementObj * entityFocus, CMovementObjList * entObjRuntime,MatrixARB * mathLib,bool correctiveOnly);
	void AutoTrigger(CMovementObj * entityFocus,CMovementObj * entityCmpare);
	void CorrectiveUnbreakableArrayToStatic(CMovementObj * entLoc,
											D3DVECTOR & intersection,
											CCollisionObj * collisionObject,
											int & indexOfObject,
											D3DVECTOR & latestNormal,
											float & dist,
											MatrixARB * mathLib,
											bool slideMethod);
	void RayHandler(int & rayIndex,
					CCollisionObj * collisionObject,
					CMovementObj * entLoc,
					D3DVECTOR & projPos,
					D3DVECTOR & position,
					D3DVECTOR & dirtemp,
					D3DVECTOR & offset,
					D3DVECTOR & latestNormal,
					MatrixARB * matLib,
					float & dist
					);
	//void UpdateRiderOfPossedEntity(CMovementObj * entityPtr);
	
	void EntityToHouseCollision(CMovementObj * entLoc,MatrixARB * mathLib,bool correctiveOnly);
	void SphereEntityToEntityCollision(CMovementObj * entityFocus,CMovementObj * entityCmpare);

};
