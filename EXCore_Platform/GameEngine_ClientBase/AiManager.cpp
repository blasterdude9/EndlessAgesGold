#include "StdAfx.h"
#include "aimanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAiManager::CAiManager(GlobalStatusVariables * gStatus,UserController * userControls)
{
	m_gStatus			= gStatus;
	m_userControls		= userControls;
}

CAiManager::~CAiManager(void)
{
}
//--------------Atrificial Intelligence loop------------------------------------------//
BOOL CAiManager::AI_LoopStateX( CMovementObj * ptrLoc,
								CAIWebObjList * ai_webDB,
								CWaterZoneObjList * waterZoneDB, 
								CMovementObjList * entObjRuntime,
								CCollisionObj * collisionBase,
								float & timeAdjust,
								double & lastTimeAdjust)
{
	if(!ptrLoc || !ptrLoc->m_aiObj || !ptrLoc->List_movementDB)
		return TRUE;

	DWORD currentTime = GetTickCount();
	
	if(ptrLoc->m_possedBy)
		return TRUE;
  
	if(ptrLoc->m_aiObj->m_controlled)
		return TRUE;

	//set
    ptrLoc->m_aiObj->m_currentSpeedOverride = 0.0f;
	ptrLoc->List_movementDB->SetUseStatesToFalse();

    if(!IsDead(ptrLoc))
	{
		AIStateMangement(ptrLoc, currentTime,entObjRuntime);
		MovementManager(ptrLoc,ai_webDB,timeAdjust,entObjRuntime,currentTime);
	}
    
	DoAIMovement(ptrLoc,timeAdjust,lastTimeAdjust,waterZoneDB);

	if(!IsDead(ptrLoc))
        AIFireManager(ptrLoc,timeAdjust,entObjRuntime,currentTime);//best to do after so the position is correct before aiming

	return TRUE;
}
//--------------Atrificial Intelligence loop------------------------------------------//
BOOL CAiManager::AI_Loop(CMovementObj * ptrLoc,
						 CAIWebObjList * ai_webDB,
						 CWaterZoneObjList * waterZoneDB, 
						 CMovementObjList * entObjRuntime,
						 CCollisionObj * collisionBase,
						 float & timeAdjust,
						 double & lastTimeAdjust)
{
	if(!ptrLoc->m_aiObj)
		return FALSE;

	long currentTime = GetTickCount();
	if(ptrLoc->m_possedBy)
		return TRUE;
  
	if(ptrLoc->m_aiObj->m_controlled)
		return TRUE;
     
	//set
    ptrLoc->m_aiObj->m_currentSpeedOverride = 0.0f;
	ptrLoc->List_movementDB->SetUseStatesToFalse();
    
	//get AI config
	if(ptrLoc->List_energy <= 0)
	{//if died dont do anything
		ptrLoc->m_filterCollision = FALSE;

		if(!ptrLoc->List_movementDB->m_floatWhenDead)
			ptrLoc->List_movementDB->m_antiGravity = FALSE;
		   
		ptrLoc->List_deathTimeVarial = GetTickCount();
		   
		if(!m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
			ptrLoc->List_controlType = 999;//flag to dissipate
	}//end if died dont do  anything
	else
	{//do thought process
		float AnimVar = 2.0f;
        //Brain
		//--check life status--//
		if(ptrLoc->m_aiObj->m_lastKnownEnergy == -9999)
			ptrLoc->m_aiObj->m_lastKnownEnergy = ptrLoc->List_energy;            
		if(ptrLoc->m_aiObj->m_lastKnownEnergy > ptrLoc->List_energy)
		{//body takin damage why?		  
			if(AI_getTarget(ptrLoc,0,entObjRuntime,collisionBase)==TRUE)
			{//get possible attacker
				ptrLoc->m_aiObj->m_condition = 1;//under attack 
				if(ptrLoc->List_energy < 0)//hurt bad retreat?
					ptrLoc->m_aiObj->m_desiredGoal = 3;//retreat
				else if(ptrLoc->m_aiObj->m_desiredGoal != 4)
				{
					delete [] ptrLoc->m_aiObj->m_currentRoute;//detach from tree
					ptrLoc->m_aiObj->m_currentRoute = 0;
					ptrLoc->m_aiObj->m_desiredGoal = 4;//kill
				}
			}//end get possible attacker
            ptrLoc->m_aiObj->m_lastKnownEnergy = ptrLoc->List_energy;
		}//end body takin damage why?
		else
			ptrLoc->m_aiObj->m_lastKnownEnergy = ptrLoc->List_energy;

		if(AI_VerifyTarget(ptrLoc,entObjRuntime)==FALSE)
		{//verify target existence
			if(ptrLoc->m_aiObj->m_desiredGoal == 4)
				ptrLoc->m_aiObj->m_desiredGoal = 0;//target not there be happy
			if(ptrLoc->m_aiObj->m_desiredGoal == 3)
				ptrLoc->m_aiObj->m_desiredGoal = 0;//safe now be happy
		}//end verify target existence
		else
		{
			if(ptrLoc->List_targetRef)
			{
				if(ptrLoc->List_targetRef->List_energy <= 0)
				{//alive?
					RemoveTarget(ptrLoc);
				}//end alive?
			}
		}

		//test aggesive
		if(ptrLoc->m_aiObj->m_agressive != 0)
		{//ai is agressive
			if(ptrLoc->List_targetRef == NULL)
			{
				if(rand()%100 < 10)
				{
					if(AI_getTarget(ptrLoc,ptrLoc->m_aiObj->m_agressive,entObjRuntime,collisionBase)==TRUE)
					{//attack it
						ptrLoc->m_aiObj->m_desiredGoal = 4;
					}//end attack it
				}
			}
		}//end ai is agressive

		EvaluateAIVisionToTargetDest(ptrLoc,currentTime,collisionBase);//vision module
		BOOL okToEnterAttackModule = TRUE;
         
		if(ptrLoc->m_aiObj->m_feelerSys)
			if(ptrLoc->m_aiObj->m_feelerSys->m_onWallCurrently)
				okToEnterAttackModule = FALSE;//filter attack module if wall mod is enabled

		ptrLoc->List_currentlyFireing = FALSE;
		if(okToEnterAttackModule)
		{//not currently pathfinding
			if(ptrLoc->m_aiObj->m_desiredGoal == 4 || ptrLoc->m_aiObj->m_needToDefend == TRUE)
				if(ptrLoc->List_targetRef != NULL)
					AI_Attack(ptrLoc,ptrLoc->m_aiObj,currentTime,timeAdjust);
		}//end not currently pathfinding

		if(ptrLoc->m_curHitCounter > 20)
		{
			if(!ptrLoc->m_aiObj->m_retreating)
			{
				ptrLoc->m_aiObj->m_retreatStamp = GetTickCount();
				ptrLoc->m_aiObj->m_retreating = TRUE;
			}
		}

		if(ptrLoc->List_targetRef != NULL)
			if(ptrLoc->m_aiObj->m_desiredGoal == 3 || ptrLoc->m_aiObj->m_retreating == TRUE)
				if(ptrLoc->m_aiObj->m_needToDefend == FALSE || ptrLoc->m_aiObj->m_retreating == TRUE)
					AI_Retreat(ptrLoc,ptrLoc->m_aiObj);//ok to retreat
		 
		AI_TravelLookBasis(ptrLoc,ptrLoc->m_aiObj,currentTime,collisionBase,timeAdjust);//manages travel to a desired location 
		 
		if(ptrLoc->m_aiObj->m_radiusSysInUse == FALSE)//radius in use no tree incrementing pls
			if(ptrLoc->List_targetRef == NULL)
				AI_TreeWebUse(ptrLoc,ai_webDB);

		if(ptrLoc->m_aiObj->m_radiusSysInUse == FALSE)//radius in use no scripting here
			if(ptrLoc->m_aiObj->m_desiredGoal == 0)
				AI_HandleScript(ptrLoc,ai_webDB);
		 
	}//end do thought process

	if(ptrLoc->m_aiObj->m_steeringModel == 0)
		AI_Transform(ptrLoc,waterZoneDB,timeAdjust,lastTimeAdjust);//Transformation Management
	else
		ManagementPlatform(ptrLoc, ptrLoc->m_aiObj->m_lookAt,ptrLoc->List_movementDB->List_curTransZ,timeAdjust,lastTimeAdjust);
         
	return TRUE;
}
//------------------------------------------------------------------------------------//
void CAiManager::ManagementPlatform(CMovementObj * ptrLoc, 
									D3DVECTOR & targetPosition, 
									float & speed,
									float & timeAdjust,
						            double & lastTimeAdjust)
{
	float finalDistance = speed * timeAdjust;

	float distMax = m_gStatus->m_matLib.MagnitudeSquared( targetPosition.x,targetPosition.y,targetPosition.z,
														  ptrLoc->List_baseFrame->List_frameMatrix._41,
														  ptrLoc->List_baseFrame->List_frameMatrix._42,
														  ptrLoc->List_baseFrame->List_frameMatrix._43);

	if(distMax < speed)
		speed = distMax;

	D3DVECTOR direction;
	direction.x = targetPosition.x - ptrLoc->List_baseFrame->List_frameMatrix._41;
	direction.y = targetPosition.y - ptrLoc->List_baseFrame->List_frameMatrix._42;
	direction.z = targetPosition.z - ptrLoc->List_baseFrame->List_frameMatrix._43;

	m_gStatus->m_matLib.Normalize(direction);

	direction.x = direction.x * finalDistance;
	direction.y = direction.y * finalDistance;
	direction.z = direction.z * finalDistance;

	ptrLoc->List_baseFrame->List_frameMatrix._41 += direction.x;
	ptrLoc->List_baseFrame->List_frameMatrix._42 += direction.y;
	ptrLoc->List_baseFrame->List_frameMatrix._43 += direction.z;     
}
//------------------------------------------------------------------------------------//
BOOL CAiManager::AI_getTarget(CMovementObj * ptrLoc,
							  int targetRetrievalType,
							  CMovementObjList * entObjRuntime,
							  CCollisionObj * collisionBase)
{
	D3DMATRIX aiMatrix;
	ptrLoc->List_baseFrame->GetTransform(NULL,&aiMatrix);
	D3DVECTOR curPosition;
	int currentWorldChannel = ptrLoc->m_tempChannel;
	float bestDistance = -1.0f;
	ptrLoc->List_baseFrame->GetPosition(NULL,&curPosition);
	
	for(POSITION posLoc = entObjRuntime->GetHeadPosition();posLoc != NULL;)
	{//Begin world object loop
		CMovementObj* possibleTarget = ( CMovementObj* ) entObjRuntime->GetNext(posLoc);

		if(possibleTarget->List_energy <= 0)
			continue;

		if(possibleTarget->m_possedBy)
			continue;

		if(possibleTarget->List_traceNumber != ptrLoc->List_traceNumber)
		{//start methods for selecting a target
			if(targetRetrievalType == 0)
			{
				if(possibleTarget->List_traceNumber == ptrLoc->List_attackerTrace)
				{//check for same trace number
					if(ptrLoc->List_targetRef != NULL && ptrLoc->m_aiObj->m_targetInSight == TRUE)
					{//if already on a target consider switching if existing still seeable
						//use a percentage of switching future variable
						int percentage = 10;//demo of variable 10 percent change of changing targets
						if(rand()%100 < percentage)
						{//suceeded in switching targets
							ptrLoc->List_AI_target = possibleTarget->List_traceNumber;
							ptrLoc->List_targetRef = possibleTarget;
							ptrLoc->m_aiObj->EnableRadiusSystem(curPosition,
																ptrLoc->m_aiObj->m_routeCurTarget,
																ptrLoc->m_aiObj->m_lookAt);
							ptrLoc->m_aiObj->m_timeStampWhenTargetNotInSight = GetTickCount();
							ptrLoc->m_aiObj->m_targetInSight = TRUE;
						}//end suceeded in switching targets
					}//end if already on a target consider switching
					else
					{//if no other target/or not in sight go ahead and switch
						ptrLoc->List_AI_target = possibleTarget->List_traceNumber;
						ptrLoc->List_targetRef = possibleTarget;
						ptrLoc->m_aiObj->EnableRadiusSystem(curPosition,
															ptrLoc->m_aiObj->m_routeCurTarget,
															ptrLoc->m_aiObj->m_lookAt);
						ptrLoc->m_aiObj->m_timeStampWhenTargetNotInSight = GetTickCount();
						ptrLoc->m_aiObj->m_targetInSight = TRUE;
					}//end if no other target go ahead and switch
					return TRUE;
				}//end for same trace number
			}
                   
			int polysExactChecked = 0;
			int polysConsidered   = 0;
			if(targetRetrievalType == 1)
			{//find nearest enemy
				CSafeZoneObj * possibleSafeZone = m_gStatus->m_safeZoneDB->GetSafeZoneByPosition(curPosition,currentWorldChannel);//unsafe
				if(!possibleSafeZone)
				{//disable aggro in safe zones
					if(possibleTarget->m_team != ptrLoc->m_team)
					{//4
						D3DVECTOR targetPosition;					  
						possibleTarget->List_baseFrame->GetPosition(NULL,&targetPosition);
						  
						float distFromTarget = m_gStatus->m_cabLib.MagnitudeSquared(curPosition.x,
																					curPosition.y,
																					curPosition.z,
																					targetPosition.x,
																					targetPosition.y,
																					targetPosition.z);
						float maxAttackRange = ptrLoc->m_aiObj->List_maxSightRadius;

						if(distFromTarget < bestDistance || bestDistance == -1.0f)
						{//better choice
							if(distFromTarget < maxAttackRange)
							{//within sight range
								D3DVECTOR localPosition = m_gStatus->m_matLib.GetLocalPositionFromMatrixView(aiMatrix,targetPosition);
								if(localPosition.z < 0)
								{//check to see if target is in front of you
									targetPosition.y += 5.0f;
									D3DVECTOR positionWldEyeLevel;
									positionWldEyeLevel.x  = curPosition.x;
									positionWldEyeLevel.y  = curPosition.y + ptrLoc->m_aiObj->m_eyeLevel;//adjust to eyelevel
									positionWldEyeLevel.z  = curPosition.z;

									if(ptrLoc->m_aiObj->CheckVisionIsClear(positionWldEyeLevel,targetPosition,collisionBase,&polysExactChecked,&polysConsidered))
									{
										ptrLoc->List_AI_target = possibleTarget->List_traceNumber;
										ptrLoc->List_targetRef = possibleTarget;

										ptrLoc->m_aiObj->EnableRadiusSystem(curPosition,
																		    ptrLoc->m_aiObj->m_routeCurTarget,
																		    ptrLoc->m_aiObj->m_lookAt);

										ptrLoc->m_aiObj->m_timeStampWhenTargetNotInSight = GetTickCount();
										bestDistance = distFromTarget;
									}
								}//end check to see if target is in front of you
							}//end within sight range
						}//end better choice
					}//end 4
				}//end disable aggro in safe zones
			}//end find nearest enemy
			else if(targetRetrievalType == 2)
			{//find nearest murderer
				if(possibleTarget->m_murderer)
				{//4
					D3DVECTOR targetPosition;						  
					possibleTarget->List_baseFrame->GetPosition(NULL,&targetPosition);
						  
					float distFromTarget = m_gStatus->m_cabLib.MagnitudeSquared(curPosition.x,
																				curPosition.y,
																				curPosition.z,
																				targetPosition.x,
																				targetPosition.y,
																				targetPosition.z);
					float maxAttackRange = ptrLoc->m_aiObj->List_maxSightRadius;

					if(distFromTarget < bestDistance || bestDistance == -1.0f)
					{//better choice
						if(distFromTarget < maxAttackRange)
						{//within sight range
							D3DVECTOR localPosition = m_gStatus->m_matLib.GetLocalPositionFromMatrixView(aiMatrix,targetPosition);
                            if(localPosition.z < 0)
							{//check to see if target is in front of you                               
								targetPosition.y += 5.0f;
								D3DVECTOR positionWldEyeLevel;
								positionWldEyeLevel.x  = curPosition.x;
								positionWldEyeLevel.y  = curPosition.y + ptrLoc->m_aiObj->m_eyeLevel;//adjust to eyelevel
								positionWldEyeLevel.z  = curPosition.z;

								if(ptrLoc->m_aiObj->CheckVisionIsClear(positionWldEyeLevel,targetPosition,collisionBase,&polysExactChecked,&polysConsidered))
								{
									ptrLoc->List_AI_target = possibleTarget->List_traceNumber;
									ptrLoc->List_targetRef = possibleTarget;

									ptrLoc->m_aiObj->EnableRadiusSystem(curPosition,
																		ptrLoc->m_aiObj->m_routeCurTarget,
																		ptrLoc->m_aiObj->m_lookAt);

									ptrLoc->m_aiObj->m_timeStampWhenTargetNotInSight = GetTickCount();
									bestDistance = distFromTarget;
								}
							}//end check to see if target is in front of you
						}//end within sight range
					}//end better choice
				}//end 4
			}//end find nearest murderer
			else if(targetRetrievalType == 3)
			{//find nearest non clan member to attack (ussually guarding territory)
				if(possibleTarget->m_networkClanID != ptrLoc->m_networkClanID)
				{// check for not in the same clan
					if(possibleTarget->List_controlType != 5)
					{//4
						D3DVECTOR targetPosition;						  
						possibleTarget->List_baseFrame->GetPosition(NULL,&targetPosition);
							  
						float distFromTarget = m_gStatus->m_cabLib.MagnitudeSquared(curPosition.x,
																					curPosition.y,
																					curPosition.z,
																					targetPosition.x,
																					targetPosition.y,
																					targetPosition.z);
						float maxAttackRange = ptrLoc->m_aiObj->List_maxSightRadius;

						if(distFromTarget < bestDistance || bestDistance == -1.0f)
						{//better choice
							if(distFromTarget < maxAttackRange)
							{//within sight range
								D3DVECTOR localPosition = m_gStatus->m_matLib.GetLocalPositionFromMatrixView(aiMatrix,targetPosition);
								if(localPosition.z < 0)
								{//check to see if target is in front of you                               
									targetPosition.y += 5.0f;
									D3DVECTOR positionWldEyeLevel;
									positionWldEyeLevel.x  = curPosition.x;
									positionWldEyeLevel.y  = curPosition.y + ptrLoc->m_aiObj->m_eyeLevel;//adjust to eyelevel
									positionWldEyeLevel.z  = curPosition.z;

									if(ptrLoc->m_aiObj->CheckVisionIsClear(positionWldEyeLevel,targetPosition,collisionBase,&polysExactChecked,&polysConsidered))
									{								   
										ptrLoc->List_AI_target = possibleTarget->List_traceNumber;
										ptrLoc->List_targetRef = possibleTarget;

										ptrLoc->m_aiObj->EnableRadiusSystem(curPosition,
																			ptrLoc->m_aiObj->m_routeCurTarget,
																			ptrLoc->m_aiObj->m_lookAt);

										ptrLoc->m_aiObj->m_timeStampWhenTargetNotInSight = GetTickCount();
										bestDistance = distFromTarget;
									}
								}//end check to see if target is in front of you
							}//end within sight range
						}//end better choice
					}//end 4
				}// end check for not in the same clan
			}//end find nearest non clan member to attack (ussually guarding territory)
		}//end start methods for selecting a target
	}// end world object loop
	
	//nothin so far
	if(ptrLoc->List_targetRef)
		return TRUE;

	return FALSE;
}
//------------------------------------------------------------------------------------//
BOOL CAiManager::AI_VerifyTarget(CMovementObj * ptrLoc,CMovementObjList * entObjRuntime)
{
	if(!ptrLoc->List_targetRef)
		return FALSE;
		 
	BOOL targetExists = FALSE;
	CMovementObj* possibleTarget = NULL;
	for(POSITION posLoc = entObjRuntime->GetHeadPosition();posLoc != NULL;)
	{//Begin world object loop
		possibleTarget = (CMovementObj *)entObjRuntime->GetNext(posLoc);
		if(possibleTarget->List_energy > 0)
		{//is target alive?
			if(ptrLoc->List_traceNumber != possibleTarget->List_traceNumber)
			{//do not target self
				if(possibleTarget->List_traceNumber == ptrLoc->List_AI_target)
				{//found target
					ptrLoc->List_targetRef = possibleTarget;
					targetExists = TRUE;		
					break;	
				}//end found target
			}//end do not taget self
		}//end is target alive?
	}//end world object loop

	if(!targetExists)
	{//disable target			
		ptrLoc->List_targetRef = NULL;
		ptrLoc->List_AI_target = -1;			
		return FALSE;
	}//end disable target

	//add distance quit
	if(ptrLoc->m_aiObj->m_targetInSight == FALSE)
	{//target not accessable at this moment eval persistence
		if(((long)GetTickCount() - ptrLoc->m_aiObj->m_timeStampWhenTargetNotInSight) > ptrLoc->m_aiObj->m_durationTillTargetLost)
		{//timed out
			ptrLoc->List_targetRef = NULL;
			ptrLoc->List_AI_target = -1;
			return FALSE;
		}//end timed out
	}//end target not accessable at this moment eval persistence
				 
	return TRUE;
}
//------------------------------------------------------------------------------------//
void CAiManager::RemoveTarget(CMovementObj * ptrLoc)
{
	ptrLoc->List_attackerTrace		= -1;//target has been elliminated
	ptrLoc->List_targetRef			= NULL;
	ptrLoc->m_aiObj->m_desiredGoal	= 0;//safe now be happy
	ptrLoc->m_aiObj->m_needToDefend = FALSE;
	ptrLoc->List_baseFrame->GetPosition(NULL,&ptrLoc->m_aiObj->m_lookAt);
}
//------------------------------------------------------------------------------------//
void CAiManager::EvaluateAIVisionToTargetDest(CMovementObj * ptrLoc,long & currentTime,CCollisionObj * collisionBase)
{
	if(!ptrLoc->m_aiObj->m_enableVision)
		return;
	if((currentTime - ptrLoc->m_aiObj->m_sightCheckStamp) > ptrLoc->m_aiObj->m_sightCheckDuration)
	{
		BOOL lastSightState = ptrLoc->m_aiObj->m_targetInSight;
		ptrLoc->m_aiObj->m_targetInSight = AI_targetInSight(ptrLoc,ptrLoc->m_aiObj->List_maxSightRadius,collisionBase);
		ptrLoc->m_aiObj->m_sightCheckStamp = currentTime;

		if(ptrLoc->m_aiObj->m_targetInSight == TRUE || ptrLoc->List_targetRef == NULL)
			ptrLoc->m_aiObj->m_timeStampWhenTargetNotInSight = currentTime;
	}
}
//-------------------Attack check version 1-Used in AI--------------------------------//
BOOL CAiManager::AI_Attack(CMovementObj * ptrLoc,
						   CAIObj * AIPtr,
						   long & currentTime,
						   float & adjustFactor)
{
	if(AIPtr->m_targetInSight)
	{//i can see target
		long timePassed = currentTime - AIPtr->m_attackStamp; 
		if(!AIPtr->m_attackingCurrently)
		{//currently not attacking
			if(timePassed > AIPtr->m_attackTimeBetweenCurrent)
			{//done waiting set attack variables
				float percentage = ((float)(rand()%100))*.01f;
				AIPtr->m_attackDurationCurrent = ((long)(percentage * ((float)(AIPtr->m_attackDurationMax - AIPtr->m_attackDurationMin)) ))+AIPtr->m_attackDurationMin;
				
				AIPtr->m_attackStamp = currentTime;
				AIPtr->m_attackingCurrently = TRUE;
			}//end done waiting set attack variables
		}//currently not attacking
		else
		{//currently attacking
			if(timePassed > AIPtr->m_attackDurationCurrent)
			{//done with attack set time between variables
				float percentage = ((float)(rand()%100))*.01f;

				//ptrLoc->m_aiObj->m_attackTimeBetweenCurrent	= ((DWORD)(percentage * ((float)(ptrLoc->m_aiObj->m_attackTimeBetweenMax - ptrLoc->m_aiObj->m_attackTimeBetweenMin)) ))+ptrLoc->m_aiObj->m_attackTimeBetweenMin;

				AIPtr->m_attackDurationCurrent = ((long)(percentage * ((float)(AIPtr->m_attackDurationMax - AIPtr->m_attackDurationMin)) ))+AIPtr->m_attackDurationMin;
				AIPtr->m_attackStamp = currentTime;
				AIPtr->m_attackingCurrently = FALSE;
			}//end done with attack set time between variables
			else
			{//i want to fire any friendlies in view?
				ptrLoc->List_currentlyFireing = TRUE;
			}//end i want to fire any friendlies in view?
		}//end currently attacking
	}//end i can see target
			
	D3DVECTOR targetPosition,entityWldPosition;
	D3DMATRIX targetMatrix;
	ptrLoc->List_baseFrame->GetPosition(NULL,&entityWldPosition);
	ptrLoc->List_targetRef->List_baseFrame->GetPosition(ptrLoc->List_baseFrame,&targetPosition);
	float actualHieght = targetPosition.y;
	ptrLoc->List_targetRef->List_baseFrame->GetTransform(NULL,&targetMatrix); 
	//find target damage location
	if(ptrLoc->List_targetRef->List_skeletalObject->m_sphereLocationalDamageList)
	{//get vunerable point
		int desBone = 0;
		float highestMultiplier = 0.0f;
		for(POSITION posLoc = ptrLoc->List_targetRef->List_skeletalObject->m_sphereLocationalDamageList->GetHeadPosition();posLoc !=NULL;)
		{//damage point loop
			CSphereCollisionObj * sphereObj = (CSphereCollisionObj *)ptrLoc->List_targetRef->List_skeletalObject->m_sphereLocationalDamageList->GetNext(posLoc);
			if(sphereObj->m_damageMultiplier > highestMultiplier)
			{
				highestMultiplier = sphereObj->m_damageMultiplier;
				desBone = sphereObj->m_boneIndex;
			}
		}//end damage point loop
		D3DMATRIX boneMatrix = ptrLoc->List_targetRef->List_skeletalObject->GetBoneMatrixByIndex(desBone);
		m_gStatus->m_matLib.fastMatrixMultiply(targetMatrix,boneMatrix,targetMatrix);			  
	}//end get vunerable point
            
	//for chargeing
	D3DVECTOR target;
	target.x = targetMatrix._41;
	target.y = targetMatrix._42; 
	target.z = targetMatrix._43; 
	
	//lead fire adjustment		
	float distFromTarget = m_gStatus->m_cabLib.MagnitudeSquared(0.0f,0.0f,0.0f,targetPosition.x,targetPosition.y,targetPosition.z);
	D3DVECTOR leadVector;
	float errorVal = 1.0f;
            
	if((rand()%100) < AIPtr->m_accuracy)
	{//imperfect shot
		float percentage = ((float)(rand()%100))*.01f;
		errorVal = (percentage * (AIPtr->m_accuracyErrorMax - AIPtr->m_accuracyErrorMin))+AIPtr->m_accuracyErrorMin;
	}//end imperfect shot

	if((AIPtr->m_currentMissileSpeed*adjustFactor)==0.0f)
		AIPtr->m_currentMissileSpeed = 1.0f;

	float multiplier = (distFromTarget) / (AIPtr->m_currentMissileSpeed * adjustFactor);
	leadVector.x = (ptrLoc->List_targetRef->m_directionalVector.x * multiplier) * errorVal;
	leadVector.y = (ptrLoc->List_targetRef->m_directionalVector.y * multiplier) * errorVal;
	leadVector.z = (ptrLoc->List_targetRef->m_directionalVector.z * multiplier) * errorVal;

	AIPtr->m_lookAt.x = target.x;
	AIPtr->m_lookAt.y = target.y;
	AIPtr->m_lookAt.z = target.z; 

	target.x += leadVector.x;
	target.y += leadVector.y;
	target.z += leadVector.z;
			
	//----------------------//
	//-advanced pathfinding-//
	//----------------------//
	ptrLoc->List_AIdestination.x = target.x;
	ptrLoc->List_AIdestination.y = target.y += (ptrLoc->List_targetRef->List_boundingRadius/2.0f);
	ptrLoc->List_AIdestination.z = target.z;
	
	//set target dest			
	D3DMATRIX viewLoc;
	ptrLoc->List_baseFrame->GetTransform(NULL,&viewLoc);
	targetPosition = m_gStatus->m_matLib.GetLocalPositionFromMatrixView(viewLoc,target);
	float angle = 0.0f; 
	if(distFromTarget != 0.0f)
	{
		angle = atan(actualHieght / distFromTarget) * 57.3f;//(180/3.1415926535897932);
		if(ptrLoc->List_skeletalObject)
			ptrLoc->List_skeletalObject->ApplyFreeLookRotation(angle,0.0f);
		ptrLoc->List_movementDB->List_freeLookCurPitch = angle;
	}

	return TRUE;
}
//-------------------Attack check version 1-Used in AI--------------------------------//
BOOL CAiManager::AI_Retreat(CMovementObj * ptrLoc,CAIObj * AIPtr)
{
	if(!AIPtr->List_targetFrame)
		return FALSE;
	
	D3DVECTOR targetPosition;
	D3DVECTOR thisPosition;
	ptrLoc->List_baseFrame->GetPosition(NULL,&thisPosition);
	ptrLoc->List_targetRef->List_baseFrame->GetPosition(NULL,&targetPosition); 
	//D3DVECTOR refPosition;
	//ptrLoc->List_targetRef->List_baseFrame->GetPosition(ptrLoc->List_baseFrame,&refPosition);
	//float distFromTarget = m_gStatus->m_cabLib.MagnitudeSquared(0.0f,0.0f,0.0f,refPosition.x,refPosition.y,refPosition.z);
	if((GetTickCount() - ptrLoc->m_aiObj->m_retreatStamp) > 10000)
	{//escaped
		ptrLoc->List_attackerTrace = -1;//target has been elliminated
		ptrLoc->List_targetRef = NULL;
		ptrLoc->m_aiObj->m_needToDefend = FALSE;
		//ptrLoc->List_baseFrame->GetPosition(NULL,&ptrLoc->List_AIdestination);
		ptrLoc->m_curHitCounter = 0;
		ptrLoc->m_aiObj->m_retreating = FALSE;
		return FALSE;
	}//end escaped

	AIPtr->m_lookAt.x = ((thisPosition.x - targetPosition.x)+thisPosition.x);
	AIPtr->m_lookAt.y = ((thisPosition.y - targetPosition.y)+thisPosition.y);
	AIPtr->m_lookAt.z = ((thisPosition.z - targetPosition.z)+thisPosition.z);

	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAiManager::AI_TravelLookBasis(CMovementObj * ptrLoc,
									CAIObj * AIPtr,
									long currentTime,
									CCollisionObj * collisionBase,
									float & timeAdjust)
{								 
	//follow code
	//can be use to travel to a location
	//if walking just use yaw
	D3DVECTOR locationLoc,wldSpcPos;
	locationLoc.x = 0.0f;
	locationLoc.y = 0.0f;
	locationLoc.z = 0.0f;

	ptrLoc->List_baseFrame->GetPosition(NULL,&wldSpcPos);
	BOOL inProcess = FALSE;
	BOOL doNormalDrive = TRUE;

	if(AIPtr->m_feelerSys)
	{//use feel path find method
		D3DMATRIX positionalMatrix;
		ptrLoc->List_baseFrame->GetTransform(NULL,&positionalMatrix);
		if((currentTime - AIPtr->m_feelerSys->m_stamp) > AIPtr->m_feelerSys->m_cycleTime)
		{
			AIPtr->UpdateFeelerStatus(ptrLoc->m_collisionMovRefPtr,
									  positionalMatrix,
									  AIPtr->m_floorTolerance,
									  AIPtr->m_wallMinimumHieght,
									  positionalMatrix._42,
									  m_gStatus->m_systemReadout,
									  ptrLoc->List_AI_reachedDestination,
									  ptrLoc->m_aiObj->m_lookAt);
			AIPtr->m_feelerSys->m_stamp = currentTime;
		}
		if(AIPtr->m_feelerSys->m_onWallCurrently)
		{//calculate
			if(AIPtr->m_targetInSight == TRUE)
			{
				AIPtr->m_feelerSys->m_onWallCurrently = FALSE;
				if(AIPtr->m_enableJumpAbility)
					m_userControls->JumpMovementClassObj(ptrLoc);
			}

			D3DVECTOR currentPositionWld;
			D3DVECTOR nUpVect,nDirVect,adjustCrossVect;
			//project normal into 2d
			nUpVect.x  = 0.0f;
			nUpVect.y  = 1.0f;
			nUpVect.z  = 0.0f;
			nDirVect.x = AIPtr->m_feelerSys->m_mostRecentWallNormal.x;
			nDirVect.y = 0.0f;
			nDirVect.z = AIPtr->m_feelerSys->m_mostRecentWallNormal.z;

			if(AIPtr->m_feelerSys->m_wallDirection == 0)//link progreesion direction
				adjustCrossVect = m_gStatus->m_matLib.CrossProduct(nDirVect,nUpVect);//makes a "right" cross on wall
			else   
				adjustCrossVect = m_gStatus->m_matLib.CrossProduct(nUpVect,nDirVect);//makes a "left" cross on wall
			
			adjustCrossVect = m_gStatus->m_matLib.Normalize(adjustCrossVect);
		
			//increment
			AIPtr->List_closeEnoughDist = 2.0f;//go to location
			inProcess = TRUE;
			adjustCrossVect.x = adjustCrossVect.x * AIPtr->m_feelerSys->m_incrementOnWall;
			adjustCrossVect.y = adjustCrossVect.y * AIPtr->m_feelerSys->m_incrementOnWall;
			adjustCrossVect.z = adjustCrossVect.z * AIPtr->m_feelerSys->m_incrementOnWall;
			ptrLoc->List_baseFrame->GetPosition(NULL,&currentPositionWld);
			//set new destination
			ptrLoc->m_aiObj->m_lookAt.x = currentPositionWld.x + adjustCrossVect.x;
			ptrLoc->m_aiObj->m_lookAt.y = currentPositionWld.y + adjustCrossVect.y;
			ptrLoc->m_aiObj->m_lookAt.z = currentPositionWld.z + adjustCrossVect.z;
		}//end calculate
	}//end use feel path find method  

	if(doNormalDrive == TRUE)
	{//normal travel
		//AIPtr->List_targetFrame->SetPosition2(NULL,ptrLoc->List_AIdestination.x,(ptrLoc->List_AIdestination.y+AIPtr->m_prefernceAboveTarget),ptrLoc->List_AIdestination.z);             
		AIPtr->List_targetFrame->SetPosition2(NULL,ptrLoc->m_aiObj->m_lookAt.x,(ptrLoc->m_aiObj->m_lookAt.y+AIPtr->m_prefernceAboveTarget),ptrLoc->m_aiObj->m_lookAt.z);
		if(AIPtr->List_targetFrame)
			AIPtr->List_targetFrame->GetPosition(ptrLoc->List_baseFrame,&locationLoc);//get reletive location

		AdvancedAiSteering(ptrLoc,locationLoc,timeAdjust);

		if(!inProcess)
		{
			if((currentTime - AIPtr->m_distanceQuerryStamp) > AIPtr->m_distanceQuerryDuration)
			{
				//calculate range randomly
				if(!AIPtr->m_attackingCurrently)
				{//not attackin following
					float totalDistance = AIPtr->m_followMaxDistance - AIPtr->m_followMinDistance;
					float percentage = ((float)(rand()%100))*.01f;
					AIPtr->List_closeEnoughDist = AIPtr->m_followMinDistance + (percentage * totalDistance);
				}//end not attackin following
				else
				{//attacking set appropriate distance
					float totalDistance = AIPtr->m_attackMaxDistance - AIPtr->m_attackMinDistance;
					float percentage = ((float)(rand()%100))*.01f;
					AIPtr->List_closeEnoughDist = AIPtr->m_attackMinDistance + (percentage * totalDistance);
				}//end attacking set appropriate distance

				AIPtr->m_distanceQuerryStamp = currentTime;
			}
		}
		//check if desired distance is fullfilled
		float floatYtemp = locationLoc.y;
		if(ptrLoc->List_movementDB->m_antiGravity==FALSE)
			floatYtemp = 0.0f;//so he doesent spaz when th dis is far up and he cant fly  
		
		int dist = (int)m_gStatus->m_cabLib.MagnitudeSquared(0.0f,
															 0.0f,
															 0.0f,
															 locationLoc.x,
															 floatYtemp,
															 locationLoc.z);
		//end check if desired distance is fullfilled
		BOOL Aim = FALSE;
		BOOL stop = FALSE;
		if(ptrLoc->List_targetRef == NULL)
		{//following path only
			if(ptrLoc->List_targetRef == NULL && dist <= AIPtr->List_closeEnoughDist)//stop
				stop = TRUE;
		}//end following path only
		else
		{//folowing somthing
			if(dist <= AIPtr->List_closeEnoughDist && AIPtr->m_targetInSight == TRUE)//move in
				stop = TRUE;
		}//end following somthing
		if(!AIPtr->RadiusSysAllowMovement(wldSpcPos,ptrLoc->List_AIdestination,ptrLoc->List_AI_target))
			stop = TRUE;
		if(ptrLoc->List_AI_target == -1)
		{//helper of radius sys (no more taregt)
			ptrLoc->List_targetRef = NULL;
			if(AIPtr->m_radiusSysInUse)
			{
				AIPtr->m_radiusSysInUse       = FALSE;
				ptrLoc->m_aiObj->m_lookAt.x   = ptrLoc->m_aiObj->m_radiusSysCenter.x;
				ptrLoc->m_aiObj->m_lookAt.y   = ptrLoc->m_aiObj->m_radiusSysCenter.y;
				ptrLoc->m_aiObj->m_lookAt.z   = ptrLoc->m_aiObj->m_radiusSysCenter.z;
			}
		}//end helper of radius sys

		if(stop)
		{//close enough
			ptrLoc->List_AI_reachedDestination = TRUE; 
			m_userControls->ZeroForces(ptrLoc->List_movementDB);
		}//end close enough
		else
		{//too far:=], move in
			ptrLoc->List_AI_reachedDestination = FALSE;
			m_userControls->Accellerate(ptrLoc->List_movementDB,AIPtr->m_currentSpeedOverride,timeAdjust);
		}//end too far, move in
	}//end normal travel
	
	//end follow code						 
	return TRUE;
}
//------------------------------------------------------------------------------------//
void CAiManager::AI_TreeWebUse(CMovementObj * ptrLoc,CAIWebObjList * ai_webDB)
{
	if(ptrLoc->m_aiObj->m_currentRoute)
	{//valid map set
		if(ptrLoc->List_AI_reachedDestination == TRUE)
		{//index to next map point of path
			if(ptrLoc->m_aiObj->m_routeCurPos >= ptrLoc->m_aiObj->m_routePointCount)
			{//no more positions
				int treepos = ptrLoc->m_aiObj->m_currentRoute[ptrLoc->m_aiObj->m_routePointCount-1];
				ptrLoc->m_aiObj->m_mapCurLocation = treepos;//track cur
				ptrLoc->m_aiObj->m_routePointCount=0;
				delete [] ptrLoc->m_aiObj->m_currentRoute;
				ptrLoc->m_aiObj->m_currentRoute=0;
				ptrLoc->m_aiObj->m_routeCurPos=0;
				//m_gStatus->m_systemReadout->AddLine("Reached destination");
			}//end no more positions
			else
			{//set next destination
				int treepos = ptrLoc->m_aiObj->m_currentRoute[ptrLoc->m_aiObj->m_routeCurPos];
				ptrLoc->m_aiObj->m_mapCurLocation = treepos;//track cur
				POSITION posWeb = ai_webDB->FindIndex(ptrLoc->m_aiObj->m_curMap);
				if(posWeb)
				{//valid position
					CAIWebObj*webObj = (CAIWebObj*)ai_webDB->GetAt(posWeb);
					ptrLoc->m_aiObj->m_lookAt = webObj->m_web->GetPosition(treepos);
					ptrLoc->m_aiObj->m_routeCurPos++;
					ptrLoc->m_aiObj->List_closeEnoughDist = 15.0f;
					ptrLoc->List_AI_reachedDestination = FALSE;
					//m_gStatus->m_systemReadout->AddLine("next destination set");
					//m_gStatus->m_systemReadout->AddLine(treepos);
				}//end valid position
			}//end set next destination
		}//end index to next map point of path
	}//end valid map set
}
//------------------------------------------------------------------------------------//
void CAiManager::AI_HandleScript(CMovementObj * ptrLoc,CAIWebObjList * ai_webDB)
{
//Handle scripts
	//if(ptrLoc->List_AI_reachedDestination == TRUE)
	if(!ptrLoc->m_aiObj->m_currentRoute)
	{//no exec till route command is finished
		if(ptrLoc->m_aiObj->m_curScript)
		{//valid current script						 
			if(((long)GetTickCount() - ptrLoc->m_aiObj->m_curScript->m_delayStamp) > ptrLoc->m_aiObj->m_curScript->m_delayDuration)
			{//delay factor
				int count = 0;
                CAIScriptEvent * newEventPtr = ptrLoc->m_aiObj->m_curScript->GetEvent(ptrLoc->m_aiObj->m_curScript->m_currentEvent);
				if(newEventPtr)
				{//----------------------------------------//
					if(newEventPtr->m_event == 0)
					{//goTo event
						POSITION posWeb = ai_webDB->FindIndex(ptrLoc->m_aiObj->m_curMap);
                        if(posWeb)
						{//valid position
							CAIWebObj*webObj = (CAIWebObj*)ai_webDB->GetAt(posWeb);
							ptrLoc->m_aiObj->m_currentRoute = webObj->GetRoute(ptrLoc->m_aiObj->m_mapCurLocation,newEventPtr->m_miscInt,&count);	
							if(ptrLoc->m_aiObj->m_currentRoute)
							{
								ptrLoc->m_aiObj->m_routePointCount = count;
								ptrLoc->m_aiObj->m_routeCurTarget = newEventPtr->m_miscInt;
							}
							else
							{
								ptrLoc->m_aiObj->m_routePointCount = 0;
								ptrLoc->m_aiObj->m_routeCurTarget = 0;
								//m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("****Tree is not compiled**** #",(DWORD)ptrLoc->m_aiObj->m_curMap);
							}
						}//end valid position
					}//end goTo event
					
					//----------------------------------------//
					else if(newEventPtr->m_event == 1)
					{//delay event
						ptrLoc->m_aiObj->m_curScript->m_delayDuration  = newEventPtr->m_miscInt;
						ptrLoc->m_aiObj->m_curScript->m_delayStamp     = GetTickCount();
						//m_gStatus->m_systemReadout->AddLine("Delay set");
					}//end delay event
					
					//----------------------------------------//
					else if(newEventPtr->m_event == 2)
					{//set Anim by version
						if(ptrLoc->List_skeletalObject)
							ptrLoc->List_skeletalObject->SetCurrentAnimationByVersion(newEventPtr->m_miscInt,100);
					}//end set Anim by version

					//----------------------------------------//
					else if(newEventPtr->m_event == 3)
					{//random position on tree 
						POSITION posWeb = ai_webDB->FindIndex(ptrLoc->m_aiObj->m_curMap);
						if(posWeb)
						{//valid position
							CAIWebObj*webObj = (CAIWebObj*)ai_webDB->GetAt(posWeb);
							int locationAmount = (int)webObj->m_web->GetCount();
                            if(locationAmount > 0)
							{//valid location amount
								int randomPosition = rand()%locationAmount;
								ptrLoc->m_aiObj->m_routeCurTarget = randomPosition;
								ptrLoc->m_aiObj->m_currentRoute = webObj->GetRoute(ptrLoc->m_aiObj->m_mapCurLocation,randomPosition,&count);	
								if(ptrLoc->m_aiObj->m_currentRoute)
									ptrLoc->m_aiObj->m_routePointCount = count;
								else
								{
									ptrLoc->m_aiObj->m_routePointCount = 0;
									//m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("****Tree is not compiled**** #",(DWORD)ptrLoc->m_aiObj->m_curMap);
								}
							}//end valid location amount
						}//end valid position
					}//end random position on tree
					  
					//----------------------------------------//
					else if(newEventPtr->m_event == 4)
					{//loop to beggining of script
						ptrLoc->m_aiObj->m_curScript->m_currentEvent = -1;
					}//end loop to beggining of script
					
					//----------------------------------------//
					ptrLoc->m_aiObj->m_curScript->m_currentEvent++;//done ..next event pls
				}
				else
				{//script done destroy
					ptrLoc->m_aiObj->m_curScript->SafeDelete();
					delete ptrLoc->m_aiObj->m_curScript;
					ptrLoc->m_aiObj->m_curScript=0;
				}//end script done destroy
			}//end delay factor
		}//end valid current script
	}//end no exec till route command is finished
//end handle scripts
}
//------------------------------------------------------------------------------------//
BOOL CAiManager::AI_Transform(CMovementObj *ptrLoc, CWaterZoneObjList *m_waterZoneDB, double timeAdjust, double lastTimeAdjust)
{
	float doYaw = ptrLoc->List_movementDB->List_curRotY;//(float)((ptrLoc->List_movementDB->List_curRotY * exp((fabs(ptrLoc->List_movementDB->List_curSpeed) ))));
	float doPitch = ptrLoc->List_movementDB->List_curRotX;//(float)((ptrLoc->List_movementDB->List_curRotX * exp((fabs(ptrLoc->List_movementDB->List_curSpeed) ))));
	if(ptrLoc->List_baseFrame == NULL)
		return FALSE;

	//ADDITIONAL ENTITY FORCES
	float AdditionalForceX = 0.0f;
	float AdditionalForceY = 0.0f;
	float AdditionalForceZ = 0.0f;
	
	//EXTERNAL FORCES
	float ExternalForceX = m_gStatus->m_environmentPtr->m_windDirectionX + ptrLoc->List_externalVectorTemp.x;
	float ExternalForceY = m_gStatus->m_environmentPtr->m_windDirectionY + ptrLoc->List_externalVectorTemp.y;
	float ExternalForceZ = m_gStatus->m_environmentPtr->m_windDirectionZ + ptrLoc->List_externalVectorTemp.z;
	ptrLoc->List_externalVectorTemp.x = 0.0f;
	ptrLoc->List_externalVectorTemp.y = 0.0f;
	ptrLoc->List_externalVectorTemp.z = 0.0f;

	if(ptrLoc->List_movementDB->m_yawIsUsed == FALSE)
		m_userControls->DecelTurn(ptrLoc->List_movementDB);
	if(ptrLoc->List_movementDB->m_pitchIsUsed == FALSE)
		m_userControls->DecelPitch(ptrLoc->List_movementDB);
	if(ptrLoc->List_movementDB->m_rollIsUsed == FALSE)
		m_userControls->DecelRoll(ptrLoc->List_movementDB);
	if(ptrLoc->List_movementDB->m_speedControlInUse == FALSE)
		m_userControls->DecelSpeed(ptrLoc->List_movementDB);
	if(ptrLoc->List_movementDB->m_sidestepsInUse == FALSE)
		m_userControls->DecelSideStep(ptrLoc->List_movementDB);

	D3DVECTOR orig;
	float submergeValue = 0.0f;
	ptrLoc->List_baseFrame->GetPosition(NULL,&orig);
	BOOL inWater = m_waterZoneDB->IsPositionInWater(orig, &submergeValue);

	ptrLoc->List_movementDB->DoPhysics(	timeAdjust,
										ptrLoc->List_baseFrame,
										ptrLoc->List_movementDB,
										AdditionalForceX,
										AdditionalForceY,
										AdditionalForceZ,
										ExternalForceX,
										ExternalForceY,
										ExternalForceZ,
										doYaw,
										doPitch,
										ptrLoc->List_movementDB->m_rollIsUsed,
										ptrLoc->List_movementDB->m_pitchIsUsed,
										ptrLoc->List_movementDB->m_yawIsUsed,
										submergeValue,
										m_gStatus->m_environmentPtr->m_seaViscusDrag,
										m_gStatus->m_environmentPtr->m_gravity,
										lastTimeAdjust,
										ptrLoc->m_collisionRaySys,
										m_gStatus->m_systemReadout);

	m_userControls->AutoLevelEntity(ptrLoc,
									ptrLoc->List_movementDB->m_autoLevelXStrength * ptrLoc->List_movementDB->m_autoLevelX,
									ptrLoc->List_movementDB->m_autoLevelZStrength * ptrLoc->List_movementDB->m_autoLevelZ,
									ptrLoc->List_movementDB->m_autoLevelTolerance);

	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CAiManager::AI_targetInSight(CMovementObj *ptrLoc, float maxDistance, CCollisionObj *collisionBase)
{
   if(ptrLoc->List_targetRef == NULL)
	  return TRUE;

   int polysExactChecked = 0;
   int polysConsidered   = 0;
   D3DVECTOR positionWldEyeLevel;
   ptrLoc->List_baseFrame->GetPosition(NULL,&positionWldEyeLevel);
   positionWldEyeLevel.y += ptrLoc->m_aiObj->m_eyeLevel;//adjust to eyelevel

   float range = m_gStatus->m_matLib.MagnitudeSquared(positionWldEyeLevel.x,positionWldEyeLevel.y,positionWldEyeLevel.z,
	                                                 ptrLoc->List_AIdestination.x,ptrLoc->List_AIdestination.y,ptrLoc->List_AIdestination.z);

   if(range > maxDistance)
	  return FALSE;


   if(ptrLoc->m_aiObj->CheckVisionIsClear(positionWldEyeLevel,ptrLoc->m_aiObj->m_lookAt,collisionBase,&polysExactChecked,&polysConsidered))
   {
	  if(!m_gStatus->RealtimeSegmentTestUseTable(positionWldEyeLevel,ptrLoc->m_aiObj->m_lookAt))
         return TRUE;
	  else
		 return FALSE;
   }
   else
	  return FALSE;
}
//------------------------------------------------------------------------------------//
void CAiManager::AdvancedAiSteering(CMovementObj * ptrLoc,D3DVECTOR locationLoc,float & adjust)
{
	if(ptrLoc->m_aiObj->m_exponentTurning == TRUE)
		ptrLoc->List_movementDB->m_exponentTurning = true;

	if(ptrLoc->m_aiObj->m_exponentTurning == TRUE)
	{
		ptrLoc->List_movementDB->List_curRotY=0.0f;
		ptrLoc->List_movementDB->List_curRotX=0.0f;
	}
		
	if(locationLoc.z > 0.0f)
	{//behind you
		if(locationLoc.x > 0.0f)
		{
			if(ptrLoc->m_aiObj->m_exponentTurning == FALSE)
				m_userControls->TurnLeft(ptrLoc->List_movementDB);
			else
			{
				ptrLoc->List_movementDB->List_curRotY = ptrLoc->List_movementDB->List_maxYaw*adjust;
				ptrLoc->List_movementDB->m_yawIsUsed = TRUE;
			}
		}
		else
		{
			if(ptrLoc->m_aiObj->m_exponentTurning == FALSE)
				m_userControls->TurnRight(ptrLoc->List_movementDB);
			else
			{
				ptrLoc->List_movementDB->List_curRotY = -ptrLoc->List_movementDB->List_maxYaw*adjust;
				ptrLoc->List_movementDB->m_yawIsUsed = TRUE;
			}
		}		
	}//end behind you
	else
	{//steer  
		if(locationLoc.x < 0.0f)
		{
			if(ptrLoc->m_aiObj->m_exponentTurning == FALSE)
				m_userControls->TurnRight(ptrLoc->List_movementDB);
			else
			{
				if(locationLoc.z != 0.0f)
				{
					float turnReq = atan(fabs(locationLoc.x)/fabs(locationLoc.z))* 57.3f;                  
					if(turnReq > ptrLoc->List_movementDB->List_maxYaw * adjust)
						turnReq = ptrLoc->List_movementDB->List_maxYaw * adjust;
					ptrLoc->List_movementDB->List_curRotY = -turnReq;
					ptrLoc->List_movementDB->m_yawIsUsed = TRUE;
				}
			}
		}
		else if(locationLoc.x > 0.0f)
		{	
			if(ptrLoc->m_aiObj->m_exponentTurning == FALSE)
			   m_userControls->TurnLeft(ptrLoc->List_movementDB);
			else
			{
				if(locationLoc.z != 0.0f)
				{
					float turnReq = atan(fabs(locationLoc.x)/fabs(locationLoc.z))* 57.3f;
					if(turnReq > ptrLoc->List_movementDB->List_maxYaw * adjust)
						turnReq = ptrLoc->List_movementDB->List_maxYaw * adjust;
					ptrLoc->List_movementDB->List_curRotY = turnReq;
					ptrLoc->List_movementDB->m_yawIsUsed = TRUE;
				}
			}
		}
		//---------------------------------//
		//YAW
		//---------------------------------//
		if(locationLoc.y < 0.0f)
		{//pitch dive	        
			if(ptrLoc->m_aiObj->m_exponentTurning == FALSE)
				m_userControls->Dive(ptrLoc->List_movementDB);
			else
			{
				if(locationLoc.z != 0.0f)
				{
					float turnReq = atan(fabs(locationLoc.y)/fabs(locationLoc.z))* 57.3f;                  
					if(turnReq > ptrLoc->List_movementDB->List_maxPitch * adjust)
						turnReq = ptrLoc->List_movementDB->List_maxPitch * adjust;
					ptrLoc->List_movementDB->List_curRotX = turnReq;
					ptrLoc->List_movementDB->m_pitchIsUsed = TRUE;
				}
			}
		}//end pitch dive
        else if(locationLoc.y > 0.0f)
		{
			if(ptrLoc->m_aiObj->m_exponentTurning == FALSE)
				m_userControls->PullUp(ptrLoc->List_movementDB);
			else
			{
				if(locationLoc.z != 0.0f)
				{
					float turnReq = atan(fabs(locationLoc.y)/fabs(locationLoc.z))* 57.3f;
					if(turnReq > ptrLoc->List_movementDB->List_maxPitch * adjust)
						turnReq = ptrLoc->List_movementDB->List_maxPitch * adjust;
					ptrLoc->List_movementDB->List_curRotX  = -turnReq;
					ptrLoc->List_movementDB->m_pitchIsUsed = TRUE;
				}
			}
		}         
	}//end steer
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAiManager::AIStateMangement(CMovementObj * ptrLoc, 
								  DWORD & currentTime,
								  CMovementObjList * entObjRuntime)
{
	if(!ptrLoc->m_aiObj)
		return;

	ptrLoc->m_aiObj->ResetVisionIsClearX();//reset sight flags (insures vision collision only gets used once
	//null memory references
	ptrLoc->m_aiObj->m_friendFrame = NULL;//friend frame
	//ptrLoc->m_aiObj->List_targetFrame = NULL;//enemy traget frame
	ptrLoc->List_targetRef = NULL;

	//-----------------------------------//
    //move position thought-decision tree//
	//-----------------------------------//
    if(currentTime - ptrLoc->m_aiObj->m_movePosThoughtDurationStamp > ptrLoc->m_aiObj->m_movePosThoughtDuration)
	{//time to do a movement thought
		ptrLoc->m_aiObj->m_movePosThoughtDurationStamp = currentTime;
		if(ptrLoc->m_aiObj->m_lockedOnPath)
			ptrLoc->m_aiObj->m_aiMovementState = 1;//follow tree state
		else if(ptrLoc->m_aiObj->m_friendTargetID > -1)
		{//has a friend
			//verify aquire friend frame
			if(!ptrLoc->m_aiObj->m_friendFrame)
				ptrLoc->m_aiObj->m_friendFrame = entObjRuntime->GetFrameByTraceAlive(ptrLoc->m_aiObj->m_friendTargetID, ptrLoc->List_traceNumber);

			if(ptrLoc->m_aiObj->m_friendFrame)
			{//found friend
				//if(m_matLib.MagnitudeSquared(ptrLoc->m_aiObj->m_friendFrame->List_frameMatrix,ptrLoc->List_baseFrame->List_frameMatrix)>ptrLoc->m_aiObj->m_friendPreferredDist)
				ptrLoc->m_aiObj->m_aiMovementState = 2;//follow friend state
				/*else
				ptrLoc->m_aiObj->m_aiMovementState = 3;//set to idle state and wait for friend*/
			}//end found friend
			else
				ptrLoc->m_aiObj->m_aiMovementState = 3;//set to idle state and wait for friend
		}//end has a friend
		else
		{//has no friends
			if(ptrLoc->List_AI_target > -1)
			{//during attack
				//verify aquire enemy target
				if(!ptrLoc->List_targetRef)
					ptrLoc->List_targetRef = entObjRuntime->GetByTraceAlive(ptrLoc->List_AI_target, ptrLoc->List_traceNumber);

				if(ptrLoc->List_targetRef)
				{//target exists
					//if(((float)ptrLoc->List_energy) > ((float)ptrLoc->List_maxEnergy) * .3f)
						ptrLoc->m_aiObj->m_aiMovementState = 4;//follow target state
					//else
					//ptrLoc->m_aiObj->m_aiMovementState = 5;//retreat state
				}//end target exists
				else
				{//target does not exist
					ptrLoc->List_AI_target = -1;
					ptrLoc->m_aiObj->m_aiMovementState = 6;//reaquire tree mode
				}//end target does not exist
			}//end during attack
			else
			{//not attacking
				if(ptrLoc->m_aiObj->m_curMap > -1)
				{//assigned to tree
					if(ptrLoc->m_aiObj->m_aiMovementState != 6)
						ptrLoc->m_aiObj->m_aiMovementState = 1;//follow tree state
				}//end assigned to tree
				else
					ptrLoc->m_aiObj->m_aiMovementState = 3;//set to idle state and wait for friend
			}//end not attacking
		}//end has no friends
	}//end time to do a movement thought
    //--------------------------------------------//
    //end move position thought tree-decision tree//
	//--------------------------------------------//

    //--------------------------------------------//
    //Attack/heal thought-decision tree-----------//
	//--------------------------------------------//
    if(currentTime - ptrLoc->m_aiObj->m_attackTargetThoughtDurationStamp > ptrLoc->m_aiObj->m_attackTargetThoughtDuration)
	{//time to do a movement thought
		ptrLoc->m_aiObj->m_attackTargetThoughtDurationStamp = currentTime;

		if(ptrLoc->List_energy != ptrLoc->m_aiObj->m_lastKnownEnergy)
		{//energy has changed
			if(ptrLoc->List_energy > ptrLoc->m_aiObj->m_lastKnownEnergy)
			{//healed
				if(ptrLoc->m_aiObj->m_requiredTamePoints > 0 && ptrLoc->m_aiObj->m_friendTargetID < 0)
				{//valid tame ability and not already tamed
					if(ptrLoc->m_aiObj->m_potentialFriendID != ptrLoc->List_attackerTrace)
						ptrLoc->m_aiObj->m_currentFriendPoints = 0;//reset
					ptrLoc->m_aiObj->m_currentFriendPoints++;//add points

					if(ptrLoc->m_aiObj->m_currentFriendPoints >= ptrLoc->m_aiObj->m_requiredTamePoints)
					{
						ptrLoc->m_aiObj->m_friendTargetID = ptrLoc->List_attackerTrace;
						ptrLoc->m_aiObj->m_aiFireState = 2;//peace state
					}
				}//end valid tame ability
			}//end healed
			else
			{//hurt 
				if(ptrLoc->List_AI_target < 0)
				{//attack entity if in sight
					ptrLoc->m_aiObj->m_aiFireState = 3;//aquire enemy target
				}//end attack entity if in sight
				else
				{//consider changing targets to target this entity 
					if(ptrLoc->List_AI_target != ptrLoc->List_attackerTrace)
						ptrLoc->m_aiObj->m_aiFireState = 4;//consider changing targets
				}//end consider changing targets to target this entity
			}//end hurt
		}//end energy has changed
		else
		{//energy hasent changed
			if(ptrLoc->List_AI_target < 0)
			{//no target and energy hasent changed
				if(ptrLoc->m_aiObj->m_agressive > 0)
					ptrLoc->m_aiObj->m_aiFireState = 5;//hunt
				else
					ptrLoc->m_aiObj->m_aiFireState = 2;//peace
			}//no target and energy hasent change
			else
				ptrLoc->m_aiObj->m_aiFireState = 1;
		}//end energy hasent changed
		ptrLoc->m_aiObj->m_lastKnownEnergy = ptrLoc->List_energy;//sync
	}
    //--------------------------------------------//
    //end Attack/heal thought-decision tree-------//
	//--------------------------------------------//
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAiManager::DoAIMovement(CMovementObj * ptrLoc,float & timeAdjust,double & lastTimeAdjust,CWaterZoneObjList * waterZoneDB)
{
	if(!ptrLoc->m_aiObj)
		return;

	if(ptrLoc->m_aiObj->m_steeringModel == 0)
		AI_Transform(ptrLoc,waterZoneDB,timeAdjust,lastTimeAdjust);//Transformation Management
	else
		ManagementPlatform(ptrLoc, ptrLoc->m_aiObj->m_lookAt,ptrLoc->List_movementDB->List_curTransZ,timeAdjust,lastTimeAdjust);
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAiManager::MovementManager(CMovementObj * ptrLoc,
								 CAIWebObjList * ai_webDB,
								 float & timeAdjust,
								 CMovementObjList * entObjRuntime,
								 DWORD & currentTime)
{
	if(!ptrLoc->m_aiObj)
		return;

	//handle movement process
	switch(ptrLoc->m_aiObj->m_aiMovementState)
	{
		case 1://follow tree state
			FollowTreeState(ptrLoc,ai_webDB,timeAdjust);
		break;
	
		case 2://follow friend state
			FollowFriendState(ptrLoc,timeAdjust,entObjRuntime);
		break;
	
		case 3://idle state
		break;

		case 4:
			FollowAttackTargetState(ptrLoc,timeAdjust,entObjRuntime,currentTime);
		break;

		case 5:
			RetreatFromAttackTargetState(ptrLoc,timeAdjust,entObjRuntime);
		break;

		case 6:
			RequireAssignedTreeState(ptrLoc,ai_webDB,timeAdjust,currentTime);
		break;
	}
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAiManager::FollowTreeState(CMovementObj * ptrLoc,
								 CAIWebObjList * ai_webDB,
								 float & timeAdjust)
{
	//if ai is trapped move to reaquire tree state
	if(ptrLoc->m_aiObj->m_bodyCollision)
	{//currently blocked
		if(ptrLoc->m_aiObj->m_blockCount > 15)
		{
			ptrLoc->m_aiObj->m_aiMovementState = 6;
			ptrLoc->m_aiObj->m_blockCount = 0;
			return;
		}

		if(rand() % 100 < 10)
			if(ptrLoc->m_aiObj->m_enableJumpAbility)
				m_userControls->JumpMovementClassObj(ptrLoc);
		ptrLoc->m_aiObj->m_blockCount += 1;
	}//end currently blocked

	CAIWebObj * webObj = ai_webDB->GetByIndex(ptrLoc->m_aiObj->m_curMap);
	if(webObj)
	{//aquire tree
		//check distance
		float dist = m_gStatus->m_matLib.MagnitudeSquared(ptrLoc->List_baseFrame->List_frameMatrix,ptrLoc->m_aiObj->m_lookAt);
		AI_HandleScript(ptrLoc,ai_webDB);
		if(!ptrLoc->m_aiObj->m_currentRoute)
		{
			m_userControls->ZeroForces(ptrLoc->List_movementDB);
			return;
		}

		ptrLoc->m_aiObj->List_closeEnoughDist = ptrLoc->m_aiObj->m_travelingDesiredDist;//global for tree state
		if(dist <= ptrLoc->m_aiObj->List_closeEnoughDist)
		{//close enough to node get next node
			if(ptrLoc->m_aiObj->m_routeCurPos >= ptrLoc->m_aiObj->m_routePointCount)
			{//its the end of rount need new one
				int treepos = ptrLoc->m_aiObj->m_currentRoute[ptrLoc->m_aiObj->m_routePointCount-1];
				ptrLoc->m_aiObj->m_mapCurLocation		= treepos;//track cur
				ptrLoc->m_aiObj->m_routePointCount		= 0;
				delete [] ptrLoc->m_aiObj->m_currentRoute;
				ptrLoc->m_aiObj->m_currentRoute         = 0;
				ptrLoc->m_aiObj->m_routeCurPos			= 0;
			}//end-its the end of rount need new one
			else
			{//go to next position
				int treepos = ptrLoc->m_aiObj->m_currentRoute[ptrLoc->m_aiObj->m_routeCurPos];
				ptrLoc->m_aiObj->m_lookAt = webObj->m_web->GetPosition(treepos);
				ptrLoc->m_aiObj->m_routeCurPos++;
			}//end go to next position
		}//else close enough to node get next node
		else
		{//keep traveling to node
			D3DVECTOR localPos = m_matLib.GetLocalPositionFromMatrixView(ptrLoc->List_baseFrame->List_frameMatrix,ptrLoc->m_aiObj->m_lookAt);
			AdvancedAiSteering(ptrLoc,localPos,timeAdjust);
			m_userControls->Accellerate(ptrLoc->List_movementDB,ptrLoc->m_aiObj->m_currentSpeedOverride,timeAdjust);
		}//end keep traveling to node		  
	}
	
	ptrLoc->m_aiObj->m_bodyCollision = FALSE;
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAiManager::FollowFriendState(CMovementObj * ptrLoc,
								   float & timeAdjust,
								   CMovementObjList * entObjRuntime)
{
	if( ptrLoc->m_aiObj->m_friendTargetID < 0)
		return;

	if(!ptrLoc->m_aiObj->m_friendFrame)
		ptrLoc->m_aiObj->m_friendFrame = entObjRuntime->GetFrameByTraceAlive(ptrLoc->m_aiObj->m_friendTargetID, ptrLoc->List_traceNumber);

	if(!ptrLoc->m_aiObj->m_friendFrame)
		return;
 
	ptrLoc->m_aiObj->m_lookAt.x = ptrLoc->m_aiObj->m_friendFrame->List_frameMatrix._41;
	ptrLoc->m_aiObj->m_lookAt.y = ptrLoc->m_aiObj->m_friendFrame->List_frameMatrix._42;
	ptrLoc->m_aiObj->m_lookAt.z = ptrLoc->m_aiObj->m_friendFrame->List_frameMatrix._43;
	
	if(m_matLib.MagnitudeSquared(ptrLoc->m_aiObj->m_friendFrame->List_frameMatrix,ptrLoc->List_baseFrame->List_frameMatrix)>ptrLoc->m_aiObj->m_friendPreferredDist)
	{//need to move closer to friend
		D3DVECTOR localPos = m_matLib.GetLocalPositionFromMatrixView(ptrLoc->List_baseFrame->List_frameMatrix,ptrLoc->m_aiObj->m_lookAt);
		AdvancedAiSteering(ptrLoc,localPos,timeAdjust);
		m_userControls->Accellerate(ptrLoc->List_movementDB,ptrLoc->m_aiObj->m_currentSpeedOverride,timeAdjust);
	}//end need to move closer to friend
	else
	{//just face friend
		D3DVECTOR localPos = m_matLib.GetLocalPositionFromMatrixView(ptrLoc->List_baseFrame->List_frameMatrix,ptrLoc->m_aiObj->m_lookAt);
		AdvancedAiSteering(ptrLoc,ptrLoc->m_aiObj->m_lookAt,timeAdjust);
	}//end just face friend
}

//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAiManager::FollowAttackTargetState(CMovementObj * ptrLoc,
								         float & timeAdjust,
								         CMovementObjList * entObjRuntime,
										 DWORD & currentTime)
{
	if( ptrLoc->List_AI_target < 0)
		return;

	if(!ptrLoc->List_targetRef)
		ptrLoc->List_targetRef = entObjRuntime->GetByTraceAlive(ptrLoc->List_AI_target, ptrLoc->List_traceNumber);

	if(!ptrLoc->List_targetRef)
		return;

	int followMode = 1;
	//calc positions
	D3DVECTOR positionWldEyeLevel,targetPosition;
	targetPosition.x  = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._41;
	targetPosition.y  = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._42 + 5.0f;//adjust to above ground
	targetPosition.z  = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._43;

	positionWldEyeLevel.x  = ptrLoc->List_baseFrame->List_frameMatrix._41;
	positionWldEyeLevel.y  = ptrLoc->List_baseFrame->List_frameMatrix._42 + ptrLoc->m_aiObj->m_eyeLevel;//adjust to eyelevel
	positionWldEyeLevel.z  = ptrLoc->List_baseFrame->List_frameMatrix._43;
	 	 
	if(followMode == 0)
	{//simple mode
		ptrLoc->m_aiObj->List_closeEnoughDist = ptrLoc->m_aiObj->m_travelingDesiredDist;;
		ptrLoc->m_aiObj->m_lookAt.x = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._41;
		ptrLoc->m_aiObj->m_lookAt.y = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._42 + ptrLoc->m_aiObj->m_prefernceAboveTarget;
		ptrLoc->m_aiObj->m_lookAt.z = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._43;
	}//end simple mode
	else
	{//path tracking mode
		D3DVECTOR myPos, targetPos;
		ptrLoc->List_baseFrame->GetPosition(NULL,&myPos);
		ptrLoc->List_targetRef->List_baseFrame->GetPosition(NULL,&targetPos);
		ptrLoc->m_aiObj->m_pathTracker.SetParams(ptrLoc->m_aiObj->m_travelingDesiredDist, 15.0f, 100);
		ptrLoc->m_aiObj->m_pathTracker.Calculate(myPos,targetPos,ptrLoc->List_AI_target,currentTime);
		//ptrLoc->m_aiObj->m_lookAt = ptrLoc->m_aiObj->m_pathTracker.m_curTravelToPosition;
		ptrLoc->m_aiObj->m_lookAt.x = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._41;
		ptrLoc->m_aiObj->m_lookAt.y = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._42 + ptrLoc->m_aiObj->m_prefernceAboveTarget;
		ptrLoc->m_aiObj->m_lookAt.z = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._43;
		//randomize and consolidate-check vision and reconsolidate if necessary
        if(rand()%100 < 10)
		{//10% 
			if(ptrLoc->m_aiObj->CheckVisionIsClearX(positionWldEyeLevel,targetPosition,ptrLoc->m_collisionVisionRefPtr))
			{//vision is clear
				ptrLoc->m_aiObj->m_pathTracker.Consolidate(myPos,targetPos,ptrLoc->List_AI_target,currentTime);
				//recalibrates happy distance during visiual attack
				float totalDistance = ptrLoc->m_aiObj->m_attackMaxDistance - ptrLoc->m_aiObj->m_attackMinDistance;
				float percentage = ((float)(rand()%100))*.01f;
				ptrLoc->m_aiObj->List_closeEnoughDist = ptrLoc->m_aiObj->m_attackMinDistance + (percentage * totalDistance);
				ptrLoc->m_aiObj->m_timeStampWhenTargetNotInSight = currentTime;
			}//end vision is clear
			else
			{//vision is not clear
				ptrLoc->m_aiObj->List_closeEnoughDist = ptrLoc->m_aiObj->m_travelingDesiredDist;//closes gap for following
				//consider forgetting about target
				if((currentTime - ptrLoc->m_aiObj->m_timeStampWhenTargetNotInSight) > ptrLoc->m_aiObj->m_durationTillTargetLost)
				{//target lost
					ptrLoc->List_AI_target = -1;
					ptrLoc->m_aiObj->m_aiMovementState = 6;
				}//end target lost
			}//end vision is not clear
			if(ptrLoc->m_aiObj->m_bodyCollision)
			{//currently blocked
				if(rand()%100<20)
					if(ptrLoc->m_aiObj->m_enableJumpAbility)
						m_userControls->JumpMovementClassObj(ptrLoc);
			}		   
		}//end 10%
		ptrLoc->m_aiObj->m_lookAt.y += ptrLoc->m_aiObj->m_prefernceAboveTarget;
	}//end path tracking mode
	 
	if(m_matLib.MagnitudeSquared(ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix,ptrLoc->List_baseFrame->List_frameMatrix)>ptrLoc->m_aiObj->List_closeEnoughDist)
	{//need to move closer to enemy
		D3DVECTOR localPos = m_matLib.GetLocalPositionFromMatrixView(ptrLoc->List_baseFrame->List_frameMatrix,ptrLoc->m_aiObj->m_lookAt);
		AdvancedAiSteering(ptrLoc,localPos,timeAdjust);
		m_userControls->Accellerate(ptrLoc->List_movementDB,ptrLoc->m_aiObj->m_currentSpeedOverride,timeAdjust);
	}//end need to move closer to enemy
	else
	{//just face enemy
		D3DVECTOR localPos = m_matLib.GetLocalPositionFromMatrixView(ptrLoc->List_baseFrame->List_frameMatrix,ptrLoc->m_aiObj->m_lookAt);
		m_userControls->ZeroForces(ptrLoc->List_movementDB);
		AdvancedAiSteering(ptrLoc,localPos,timeAdjust);
	}//end just face enemy
	
	ptrLoc->m_aiObj->m_bodyCollision = FALSE;
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAiManager::RetreatFromAttackTargetState(CMovementObj * ptrLoc,
								              float & timeAdjust,
								              CMovementObjList * entObjRuntime)
{
	if( ptrLoc->List_AI_target < 0)
		return;

	if(!ptrLoc->m_aiObj->List_targetFrame)
		ptrLoc->m_aiObj->List_targetFrame = entObjRuntime->GetFrameByTraceAlive(ptrLoc->List_AI_target, ptrLoc->List_traceNumber);

	if(!ptrLoc->m_aiObj->List_targetFrame)
	{
		ptrLoc->m_aiObj->m_aiMovementState = 6;//reaquire tree mode
		return;
	}

	ptrLoc->m_aiObj->m_lookAt.x = ((ptrLoc->List_baseFrame->List_frameMatrix._41 - ptrLoc->m_aiObj->List_targetFrame->List_frameMatrix._41) + ptrLoc->List_baseFrame->List_frameMatrix._41);
	ptrLoc->m_aiObj->m_lookAt.y = ((ptrLoc->List_baseFrame->List_frameMatrix._42 - ptrLoc->m_aiObj->List_targetFrame->List_frameMatrix._42) + ptrLoc->List_baseFrame->List_frameMatrix._42);
	ptrLoc->m_aiObj->m_lookAt.z = ((ptrLoc->List_baseFrame->List_frameMatrix._43 - ptrLoc->m_aiObj->List_targetFrame->List_frameMatrix._43) + ptrLoc->List_baseFrame->List_frameMatrix._43);

	D3DVECTOR localPos = m_matLib.GetLocalPositionFromMatrixView(ptrLoc->List_baseFrame->List_frameMatrix,ptrLoc->m_aiObj->m_lookAt);
	AdvancedAiSteering(ptrLoc,localPos,timeAdjust);
	m_userControls->Accellerate(ptrLoc->List_movementDB,ptrLoc->m_aiObj->m_currentSpeedOverride,timeAdjust);	
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAiManager::RequireAssignedTreeState(CMovementObj * ptrLoc,
								          CAIWebObjList * ai_webDB,
								          float & timeAdjust,
										  DWORD & currentTime)
{
	//use random for delay
	if(rand() % 100 >= 10)
		return;

	if((currentTime - ptrLoc->m_spawnTimeStamp) > 10800000)//going to be regened so ingnore
		return;

	CAIWebObj * webObj = ai_webDB->GetByIndex(ptrLoc->m_aiObj->m_curMap);
	if(webObj)
	{//aquire tree
		m_userControls->ZeroForces(ptrLoc->List_movementDB);
		CAIWebNode * node = webObj->m_web->GetByIndex(ptrLoc->m_aiObj->m_requireTreeIncrement++);
		if(!node)
		{
			//ptrLoc->m_aiObj->m_aiMovementState				= 3;//reaquire tree mode
			//wait for others or hunt others if not despawn;
			ptrLoc->List_energy = 0;

			if((currentTime-ptrLoc->m_spawnTimeStamp) > 30000)
			{//minimum
				ptrLoc->m_spawnTimeStamp -= 11000000;//for despawn
				m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("cannot require netID/TreeIndex #:",m_gStatus->m_cabLib.ConvertLongToString(ptrLoc->List_networkDefinedID),(DWORD)ptrLoc->m_aiObj->m_curMap);
			}//end minimum
		}
		else
		{
			D3DVECTOR positionWldEyeLevel,targetPosition;
			targetPosition = node->m_position;
			targetPosition.y += 5.0f;

			positionWldEyeLevel.x  = ptrLoc->List_baseFrame->List_frameMatrix._41;
			positionWldEyeLevel.y  = ptrLoc->List_baseFrame->List_frameMatrix._42 + ptrLoc->m_aiObj->m_eyeLevel;//adjust to eyelevel
			positionWldEyeLevel.z  = ptrLoc->List_baseFrame->List_frameMatrix._43;
			ptrLoc->m_aiObj->ResetVisionIsClearX();
			if(ptrLoc->m_aiObj->CheckVisionIsClearX(positionWldEyeLevel,targetPosition,ptrLoc->m_collisionVisionRefPtr))
			{//node is visible
				ptrLoc->m_aiObj->m_lookAt = node->m_position;
				ptrLoc->m_aiObj->m_mapCurLocation = ptrLoc->m_aiObj->m_requireTreeIncrement-1;
				if(ptrLoc->m_aiObj->m_currentRoute)
				{
					delete [] ptrLoc->m_aiObj->m_currentRoute;
					ptrLoc->m_aiObj->m_currentRoute=0;
					ptrLoc->m_aiObj->m_routeCurPos=0;
				}
				ptrLoc->m_aiObj->m_requireTreeIncrement = 0;
				ptrLoc->m_aiObj->m_aiMovementState = 1;//follow tree mode
			}//end node is visible
		}
	}//end aquire tree
	else
		ptrLoc->m_aiObj->m_aiMovementState = 3;//idle
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAiManager::AIFireManager(CMovementObj * ptrLoc,
							   float & timeAdjust,
							   CMovementObjList * entObjRuntime,
							   DWORD & currentTime)
{
	if(!ptrLoc->m_aiObj)
		return;

	switch(ptrLoc->m_aiObj->m_aiFireState)
	{
		case 1://fire/swing at enemy
			FireAtEnemy(ptrLoc,timeAdjust,entObjRuntime,currentTime);
			MaintainTarget(ptrLoc,timeAdjust,entObjRuntime);
		break;

		case 2://peace state
			ptrLoc->List_currentlyFireing  = FALSE;
		break;

		case 3://find attacker and target him
			ptrLoc->List_currentlyFireing  = FALSE;
			TargetAttacker(ptrLoc,entObjRuntime);
		break;

		case 4://consider new attacker
			ConsiderNewtarget(ptrLoc,entObjRuntime);
		break;

		case 5://Look/Hunt for a enemy (aggressive ai)
			ptrLoc->List_currentlyFireing  = FALSE;
			HuntState(ptrLoc,entObjRuntime);
		break;
	}		 
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAiManager::FireAtEnemy(CMovementObj * ptrLoc,
							 float & timeAdjust,
							 CMovementObjList * entObjRuntime,
							 DWORD & currentTime)
{
	if(ptrLoc->List_targetRef)
	{//valid target reference
		if(!ptrLoc->m_aiObj->m_attackingCurrently)
		{//if were not currently attacking
			if((currentTime - ptrLoc->m_aiObj->m_attackStamp) > ptrLoc->m_aiObj->m_attackTimeBetweenCurrent)
			{//check if we can attack
				if(m_gStatus->m_matLib.MagnitudeSquared(ptrLoc->List_baseFrame->List_frameMatrix,ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix) < ptrLoc->m_aiObj->m_strikeShootRange)
				{//time to attack
					D3DVECTOR positionWldEyeLevel,targetPosition;
					targetPosition.x  = ptrLoc->List_baseFrame->List_frameMatrix._41;
					targetPosition.y  = ptrLoc->List_baseFrame->List_frameMatrix._42 + (ptrLoc->List_boundingRadius * .5f);
					targetPosition.z  = ptrLoc->List_baseFrame->List_frameMatrix._43;

					positionWldEyeLevel.x  = ptrLoc->List_baseFrame->List_frameMatrix._41;
					positionWldEyeLevel.y  = ptrLoc->List_baseFrame->List_frameMatrix._42 + ptrLoc->m_aiObj->m_eyeLevel;//adjust to eyelevel
					positionWldEyeLevel.z  = ptrLoc->List_baseFrame->List_frameMatrix._43;
					ptrLoc->m_aiObj->ResetVisionIsClearX();
					if(ptrLoc->m_aiObj->CheckVisionIsClearX(positionWldEyeLevel,targetPosition,ptrLoc->m_collisionVisionRefPtr))
					{//node is visible
						float percentage = ((float)(rand()%100))*.01f;
						ptrLoc->m_aiObj->m_attackDurationCurrent = ((long)(percentage * ((float)(ptrLoc->m_aiObj->m_attackDurationMax - ptrLoc->m_aiObj->m_attackDurationMin)) ))+ptrLoc->m_aiObj->m_attackDurationMin;
						if(ptrLoc->m_aiObj->m_attackDurationCurrent < 2000)
						ptrLoc->m_aiObj->m_attackDurationCurrent = 2000;

						ptrLoc->m_aiObj->m_attackingCurrently = TRUE;
						ptrLoc->m_aiObj->m_attackStamp		  = currentTime;
						ptrLoc->List_currentlyFireing         = TRUE;
					}//end node is visible
					else
					{
						ptrLoc->m_aiObj->m_attackStamp = currentTime;//wait till in sight
						ptrLoc->List_currentlyFireing  = FALSE;
					}
				}//end time to attack
			}//end check if we can attack
		}//end if were not currently attacking
	}//end valid target reference
	
	if(ptrLoc->m_aiObj->m_attackingCurrently)
	{//if currently attacking
		if((currentTime - ptrLoc->m_aiObj->m_attackStamp) > ptrLoc->m_aiObj->m_attackDurationCurrent)
		{//wait
		   float percentage = ((float)(rand() % 100)) * .01f;
		   ptrLoc->m_aiObj->m_attackTimeBetweenCurrent	= ((DWORD)(percentage * ((float)(ptrLoc->m_aiObj->m_attackTimeBetweenMax - ptrLoc->m_aiObj->m_attackTimeBetweenMin)) ))+ptrLoc->m_aiObj->m_attackTimeBetweenMin;
		   if(ptrLoc->m_aiObj->m_attackTimeBetweenCurrent < 100)
		      ptrLoc->m_aiObj->m_attackTimeBetweenCurrent = 100;
		   
		   ptrLoc->m_aiObj->m_attackStamp				= currentTime;
		   ptrLoc->m_aiObj->m_attackingCurrently		= FALSE;
		   ptrLoc->List_currentlyFireing                = FALSE;
		}//end wait
	}//end if currently attacking
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAiManager::MaintainTarget(CMovementObj * ptrLoc,
							    float & timeAdjust,
							    CMovementObjList * entObjRuntime)
{
	if(ptrLoc->m_aiObj->m_attackingCurrently)
	{//maintain target
		if(!ptrLoc->List_targetRef)
			ptrLoc->List_targetRef = entObjRuntime->GetByTraceAlive(ptrLoc->List_AI_target, ptrLoc->List_traceNumber);

		if(ptrLoc->List_targetRef)
		{//valid target reference
			float errorVal = 1.0f;
			if((rand()%100) < ptrLoc->m_aiObj->m_accuracy)
			{//inperfect shot
				float percentage = ((float)(rand() % 100)) * .01f;
				errorVal = (percentage * (ptrLoc->m_aiObj->m_accuracyErrorMax - ptrLoc->m_aiObj->m_accuracyErrorMin))+ptrLoc->m_aiObj->m_accuracyErrorMin;
			}//end imperfect shot

			float distFromTarget = m_gStatus->m_matLib.MagnitudeSquared(ptrLoc->List_baseFrame->List_frameMatrix,ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix);
			if((ptrLoc->m_aiObj->m_currentMissileSpeed*timeAdjust)==0.0f)
				ptrLoc->m_aiObj->m_currentMissileSpeed = 1.0f;
			float multiplier = distFromTarget / (ptrLoc->m_aiObj->m_currentMissileSpeed*timeAdjust);
			D3DMATRIX targetMatrix;
			ptrLoc->List_targetRef->List_baseFrame->GetTransform(NULL,&targetMatrix); 
			ptrLoc->m_aiObj->m_lookAt.x = targetMatrix._41;
			ptrLoc->m_aiObj->m_lookAt.y = targetMatrix._42;
			ptrLoc->m_aiObj->m_lookAt.z = targetMatrix._43;
			ptrLoc->List_AIdestination.x = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._41 + ((ptrLoc->List_targetRef->m_directionalVector.x*multiplier)*errorVal);
			ptrLoc->List_AIdestination.y = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._42 + ((ptrLoc->List_targetRef->m_directionalVector.y*multiplier)*errorVal);
			ptrLoc->List_AIdestination.z = ptrLoc->List_targetRef->List_baseFrame->List_frameMatrix._43 + ((ptrLoc->List_targetRef->m_directionalVector.z*multiplier)*errorVal);
			ptrLoc->m_directionalVector.x = ptrLoc->m_aiObj->m_lookAt.x;
			ptrLoc->m_directionalVector.y = ptrLoc->m_aiObj->m_lookAt.y;
			ptrLoc->m_directionalVector.z = ptrLoc->m_aiObj->m_lookAt.z;
		}//end valid target reference
	}//end maintain target
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAiManager::TargetAttacker(CMovementObj * ptrLoc,CMovementObjList * entObjRuntime)
{
	if(!ptrLoc->m_aiObj->List_targetFrame)
		ptrLoc->m_aiObj->List_targetFrame = entObjRuntime->GetFrameByTraceAlive(ptrLoc->List_attackerTrace, ptrLoc->List_traceNumber);

	if(!ptrLoc->m_aiObj->List_targetFrame)
		return;//cant find it

	ptrLoc->List_AI_target = ptrLoc->List_attackerTrace;
	ptrLoc->m_aiObj->m_aiFireState = 1;
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CAiManager::ConsiderNewtarget(CMovementObj * ptrLoc,CMovementObjList * entObjRuntime)
{
	if(!ptrLoc->m_aiObj->List_targetFrame)
		ptrLoc->m_aiObj->List_targetFrame = entObjRuntime->GetFrameByTraceAlive(ptrLoc->List_AI_target, ptrLoc->List_traceNumber);

	if(!ptrLoc->m_aiObj->List_targetFrame)
	{//current traget is invalid so SWITCH
		ptrLoc->m_aiObj->List_targetFrame = entObjRuntime->GetFrameByTraceAlive(ptrLoc->List_attackerTrace, ptrLoc->List_traceNumber);
		ptrLoc->List_AI_target = ptrLoc->List_attackerTrace;
		ptrLoc->m_aiObj->m_aiFireState = 1;
	}//end current traget is invalid so SWITCH

	//both are possibly valid roll for swicth
	if(rand() % 100 < ptrLoc->m_aiObj->m_switchTargetPercentage)
	{//switch
		ptrLoc->m_aiObj->List_targetFrame = entObjRuntime->GetFrameByTraceAlive(ptrLoc->List_attackerTrace, ptrLoc->List_traceNumber);
		ptrLoc->List_AI_target = ptrLoc->List_attackerTrace;
		ptrLoc->m_aiObj->m_aiFireState = 1;
	}//end swicth
}
//------------------------------------------------------------------------------------//

//------------------------------------------------------------------------------------//
void CAiManager::HuntState(CMovementObj * ptrLoc,CMovementObjList * entObjRuntime)
{
	if(!ptrLoc->m_aiObj)
		return;

	if(rand()%100 < 10)
		return;

	CSafeZoneObj * possibleSafeZone = m_gStatus->m_safeZoneDB->GetSafeZoneByPosition(ptrLoc->List_lastPosition,ptrLoc->m_tempChannel);//unsafe
	if(possibleSafeZone)//disable aggro in safe zones
		return;

	float maxAttackRange = ptrLoc->m_aiObj->m_agressiveRange;
	D3DVECTOR positionWldEyeLevel,targetPosition;
	ZeroMemory(&targetPosition,sizeof(D3DVECTOR));
	ZeroMemory(&positionWldEyeLevel,sizeof(D3DVECTOR));
	float bestDistance = -1.0f;
	
	//look for enemy 
	for(POSITION posLoc = entObjRuntime->GetHeadPosition();posLoc != NULL;)
	{//Begin runtime entity object loop
		CMovementObj* possibleTarget = ( CMovementObj* ) entObjRuntime->GetNext(posLoc);

		if(possibleTarget->List_energy <= 0)
			continue;

		if(possibleTarget->m_possedBy)
			continue;

		if(possibleTarget->List_traceNumber == ptrLoc->List_traceNumber)
			continue;

		if(ptrLoc->m_aiObj->m_agressive == 2)//find nearest murderer
			if(!possibleTarget->m_murderer)
				continue;
                   
		if(possibleTarget->m_team != ptrLoc->m_team)
		{//4
			float distFromTarget = m_matLib.MagnitudeSquared(ptrLoc->List_baseFrame->List_frameMatrix,possibleTarget->List_baseFrame->List_frameMatrix);

			if(distFromTarget < bestDistance || bestDistance == -1.0f)
			{//valid distance
				if(distFromTarget < maxAttackRange)
				{//within sight range
					targetPosition.x = possibleTarget->List_baseFrame->List_frameMatrix._41;
					targetPosition.y = possibleTarget->List_baseFrame->List_frameMatrix._42 + 5.0f;
					targetPosition.z = possibleTarget->List_baseFrame->List_frameMatrix._43;

					D3DVECTOR localPosition = m_gStatus->m_matLib.GetLocalPositionFromMatrixView(ptrLoc->List_baseFrame->List_frameMatrix,targetPosition);
					if(localPosition.z < 0)
					{//check to see if target is in front of you
						positionWldEyeLevel.x  = ptrLoc->List_baseFrame->List_frameMatrix._41;
						positionWldEyeLevel.y  = ptrLoc->List_baseFrame->List_frameMatrix._42 + ptrLoc->m_aiObj->m_eyeLevel;//adjust to eyelevel
						positionWldEyeLevel.z  = ptrLoc->List_baseFrame->List_frameMatrix._43;
						ptrLoc->m_aiObj->ResetVisionIsClearX();
						if(ptrLoc->m_aiObj->CheckVisionIsClearX(positionWldEyeLevel,targetPosition,ptrLoc->m_collisionVisionRefPtr))
						{
							ptrLoc->List_AI_target = possibleTarget->List_traceNumber;
							ptrLoc->List_targetRef = possibleTarget;
							ptrLoc->m_aiObj->m_timeStampWhenTargetNotInSight = GetTickCount();
							bestDistance = distFromTarget;

							//in sight roll random to just take this target instead of looking for more-this should optimize exponet in big battles
							if(rand() % 100 < 25)
							return;
						}
					}//end check to see if target is in front of you		
				}//end within sight range  
			}//end valid distance
		}//end 4	  
	}//end Begin runtime entity object loop
	
	return;
}
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CAiManager::IsDead(CMovementObj * ptrLoc)
{
	if(ptrLoc->List_energy <= 0)
	{//if dead dont do anything
		ptrLoc->m_filterCollision = FALSE;

		if(!ptrLoc->List_movementDB->m_floatWhenDead)
			ptrLoc->List_movementDB->m_antiGravity = FALSE;

		ptrLoc->List_deathTimeVarial = GetTickCount();

		if(!m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
			ptrLoc->List_controlType = 999;//flag to dissipate

		m_userControls->ZeroForces(ptrLoc->List_movementDB);
		return TRUE;
	}//end if dead dont do anything
	
	return FALSE;
}






