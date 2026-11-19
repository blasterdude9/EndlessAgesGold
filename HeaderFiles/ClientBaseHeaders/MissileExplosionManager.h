#pragma once
#include "globalstatusvariables.h"
#include "clientnetworkinglogic.h"


class CMissileExplosionManager
{
public:
	CMissileExplosionManager(GlobalStatusVariables * gStatus, CClientNetworkingLogic * cNetworkObject);
	~CMissileExplosionManager(void);


	//variables
	GlobalStatusVariables					* m_gStatus;
	CClientNetworkingLogic                  * m_cNetworkObject;


	//Functions
	void UpdateRuntimeMissileLastPosition(CMissileObjList * missileRuntime);
	void MissileCollisionCheck(CMovementObjList * entObjRuntime,CMissileObjList * missileRuntime,CExplosionObjList * explosionRuntimeList);
	void MissileCollisionCheckWorld(CCollisionObj * collisionBase,CMissileObjList * missileRuntime,CExplosionObjList * explosionRuntimeList);
	void ExplosionCollisionCallback(CMovementObjList * entObjRuntime,CExplosionObjList * explosionRuntimeList);
	void MissileCallback(CMissileObjList * missileRuntime,CExplosionObjList * explosionRuntimeList,float & adjust,double & lastAdjust);
	void ExplosionCallback(CMovementObjList * entObjRuntime,CExplosionObjList * explosionRuntimeList);
	void MissileCollisionPipeline(CMovementObjList * entObjRuntime,CMissileObjList * missileRuntime,CExplosionObjList * explosionRuntimeList,CCollisionObj * collisionBase);
	bool MissileToEntityUseESpheres(CMissileObj * misPtr,
														  D3DVECTOR & currentPosition,
														  CMovementObjList * entObjRuntime,
														  CMissileObjList * missileRuntime,
														  CMovementObj ** attackedEntity,
														  D3DVECTOR & closestIntesection,
														  float & bestDistance);
	bool MissileToStaticCollision(  CMissileObj * misPtr,
									CCollisionObj * collisionBase,
									CMissileObjList * missileRuntime,
									CExplosionObjList * explosionRuntimeList,
									D3DVECTOR & closestIntesection,
									float & bestDistance,
									D3DVECTOR & groundOriPosition,
									D3DVECTOR & normalReference,
									D3DMATRIX * transformOfCollisionSys,
								    D3DMATRIX * invTransformOfCollisionSys);
	bool MissileCollisionHandler(   CMissileObj * misPtr,
									D3DVECTOR & groundOriPosition,
									D3DVECTOR & collisionIntersect,
									CMovementObj * entityThatWasAttacked,
									CExplosionObjList * explosionRuntimeList,
									D3DVECTOR & currentPosition,
									CMovementObjList * entObjRuntime,
									D3DVECTOR & normalRet);
};
