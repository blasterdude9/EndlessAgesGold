#include "StdAfx.h"
#include "missileexplosionmanager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMissileExplosionManager::CMissileExplosionManager(GlobalStatusVariables * gStatus, CClientNetworkingLogic * cNetworkObject)
{
      m_gStatus = gStatus;
	  m_cNetworkObject = cNetworkObject;
}

CMissileExplosionManager::~CMissileExplosionManager(void)
{
}

void CMissileExplosionManager::UpdateRuntimeMissileLastPosition(CMissileObjList * missileRuntime)
{
    for(POSITION posLoc = missileRuntime->GetHeadPosition();posLoc != NULL;)
	{//missile last position
        CMissileObj * mPtr = ( CMissileObj* ) missileRuntime->GetNext(posLoc);
        mPtr->m_baseFrame->GetPosition(NULL,&mPtr->m_lastPosition);
	}//end missile last position
}
//--------------Missile collision check to entities---------------------------------------//
void CMissileExplosionManager::MissileCollisionCheck(CMovementObjList * entObjRuntime,CMissileObjList * missileRuntime,CExplosionObjList * explosionRuntimeList)//entity check
{
  for(POSITION posLoc = missileRuntime->GetHeadPosition();posLoc != NULL;)
  {//loop through missile objects
      CMissileObj * misPtr = ( CMissileObj* ) missileRuntime->GetNext(posLoc);
	  if(misPtr->m_lifeTime == 0)
		 continue; 
	  if(misPtr->m_laserObject)
	    if(misPtr->m_laserObject->m_terminated == TRUE)
          continue;
	  D3DVECTOR currentPosition;
	  misPtr->m_baseFrame->GetPosition(NULL,&currentPosition);
      for(POSITION posLoc2 = entObjRuntime->GetHeadPosition();posLoc2 != NULL;)
	  {//Begin entity loop
	      CMovementObj * entPtr = ( CMovementObj* ) entObjRuntime->GetNext(posLoc2);
          
		  if(m_gStatus->m_multiplayerObject->m_AIServer)
	         if(entPtr->m_team == misPtr->m_teamRef)
				continue;
		  
		  if(entPtr->List_controlType == 35)
			 continue;

		  if(entPtr->m_possedBy)
		     continue;

		  if(!entPtr->List_baseFrame || !entPtr->List_skeletalObject)//not fully built - can't be hit
		     continue;

	      if(entPtr->List_energy > 0)//make sure alive 5 alive
		  if(entPtr->List_traceNumber != misPtr->m_ownerTrace)
		  {//make sure not owner
			D3DMATRIX positionMatrix;
			entPtr->List_baseFrame->GetTransform(NULL,&positionMatrix);
            if(entPtr->List_skeletalObject->m_calulationSphere){//valid collision calc object
			 D3DVECTOR bonePosition = m_gStatus->m_matLib.UpdatePolygonVertex(positionMatrix.m,entPtr->List_skeletalObject->GetBonePositionByIndex(entPtr->List_skeletalObject->m_calulationSphere->m_boneIndex));
             //get virtual Radius
			 float virRadius = m_gStatus->m_matLib.MagnitudeSquared(misPtr->m_lastPosition.x,misPtr->m_lastPosition.y,misPtr->m_lastPosition.z,
				                                                   currentPosition.x,currentPosition.y,currentPosition.z);	
			 virRadius = (virRadius*.5f) + misPtr->m_collisionRadius;//build radius
             D3DVECTOR virCenter;
			 virCenter.x = misPtr->m_lastPosition.x + ((currentPosition.x - misPtr->m_lastPosition.x)*.5f);
             virCenter.y = misPtr->m_lastPosition.y + ((currentPosition.y - misPtr->m_lastPosition.y)*.5f);
			 virCenter.z = misPtr->m_lastPosition.z + ((currentPosition.z - misPtr->m_lastPosition.z)*.5f);

			 if(m_gStatus->m_matLib.BoundingSphereCheck(virRadius,
                                                       entPtr->List_skeletalObject->m_calulationSphere->m_sphereRadius,
													   bonePosition.x,
													   bonePosition.y,
													   bonePosition.z,
                                                       virCenter.x,
													   virCenter.y,
												 	   virCenter.z))
			 {//in calculation radius 
			 //if(m_gStatus->m_matLib.CollisionSphereSeg(currentPosition,misPtr->m_lastPosition,entPtr->List_skeletalObject->m_calulationSphere->m_sphereRadius,bonePosition,FALSE,NULL,misPtr->m_collisionRadius,NULL)==TRUE)
			 //{//in calculation radius
			  if(entPtr->List_skeletalObject->m_sphereLocationalDamageList){//valid locational damage list
		         for(POSITION dmgPos = entPtr->List_skeletalObject->m_sphereLocationalDamageList->GetHeadPosition();dmgPos != NULL;)
				 {//damage sphere loop
		             CSphereCollisionObj * dmgSph = (CSphereCollisionObj *)entPtr->List_skeletalObject->m_sphereLocationalDamageList->GetNext(dmgPos);
		             POSITION bnPos = entPtr->List_skeletalObject->m_bonesList->FindIndex(dmgSph->m_boneIndex);
                     if(bnPos){//valid
                        CBoneObject * bnPtr = (CBoneObject *)entPtr->List_skeletalObject->m_bonesList->GetAt(bnPos);
                        D3DVECTOR bnPosition;
						bnPtr->m_boneFrame->GetPosition(NULL,&bnPosition);
                        bnPosition = m_gStatus->m_matLib.UpdatePolygonVertex(positionMatrix.m,bnPosition);
					    D3DVECTOR intersection;
						if(m_gStatus->m_matLib.CollisionSphereSeg(currentPosition,misPtr->m_lastPosition,
					                                             dmgSph->m_sphereRadius,bnPosition,
													             FALSE,NULL,misPtr->m_collisionRadius,&intersection)==TRUE){//hit 
                                     
							         //if(dmgSph->m_comment.GetLength() > 0)
							         //m_gStatus->m_systemReadout->AddLine(dmgSph->m_comment,m_gStatus->m_3dDevice,m_gStatus->m_symbolMapDB,m_gStatus->m_speechMode);
				                     //damage
									  
                                     //explosion
									 D3DVECTOR groundOriPosition,locationExpRef;
									 misPtr->m_baseFrame->GetPosition(NULL,&locationExpRef);
		                             groundOriPosition.x = 0.0f;
		                             groundOriPosition.y = 1.0f;
		                             groundOriPosition.z = 0.0f;

									 BOOL doExplosion = TRUE;

                                     if(misPtr->m_typeOfProjectile == 3)
			                           if(misPtr->m_laserObject->m_terminated)
				                          doExplosion = FALSE;

                                     if(misPtr->m_explosionLink < 0)
			                            doExplosion = FALSE;

									 if(doExplosion){
                                     m_gStatus->m_explosionDBList->CreateExplosion(misPtr->m_explosionLink,
										                                intersection,
										                                explosionRuntimeList,
										                                groundOriPosition,
																		misPtr->m_ownerTrace,
																		m_gStatus->m_3dDevice,
																		misPtr->m_teamRef,
																		-1
										                                ); 
									 misPtr->m_explosionLink =-1;//dont let this explode again
									 }

									 BOOL kill = TRUE;
                                      if(misPtr->m_typeOfProjectile == 3)
			                          if(misPtr->m_laserObject->m_lockDown)
									  {
                                         kill = FALSE;
				                         misPtr->m_laserObject->m_terminated = TRUE;
                                         misPtr->m_baseFrame->SetPosition(NULL,intersection);
									  }
		         
		                              if(kill)		  
									     misPtr->m_lifeTime = 0;

									 //physics
									 D3DVECTOR forceVector;
									 forceVector.x = currentPosition.x - misPtr->m_lastPosition.x;
									 forceVector.y = currentPosition.y - misPtr->m_lastPosition.y;
									 forceVector.z = currentPosition.z - misPtr->m_lastPosition.z;
									 forceVector = m_gStatus->m_matLib.Normalize(forceVector);
							
									 //entPtr->List_externalVectorTemp.x += forceVector.x * misPtr->m_pushPower;
									 //entPtr->List_externalVectorTemp.y += forceVector.y * misPtr->m_pushPower;
									 //entPtr->List_externalVectorTemp.z += forceVector.z * misPtr->m_pushPower;

									 if(!m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
									 {
										 int damage = 0;
										 if((misPtr->m_maxDamageValue - misPtr->m_minDamageValue) > 0)
											 damage = misPtr->m_minDamageValue + (rand() % (misPtr->m_maxDamageValue - misPtr->m_minDamageValue));
										 else
											 damage = misPtr->m_minDamageValue;
										 entPtr->List_energy -= (int)(damage * dmgSph->m_damageMultiplier);
									     entPtr->m_curHitCounter++;
										 //m_gStatus->m_screenFlashModule->DoIt(100,m_gStatus->m_3dDevice);
									 }
								     else{//send network data
                                         CMovementObj * entityPtrLoc = entObjRuntime->GetObjByTrace(misPtr->m_ownerTrace); 
                                         if(entityPtrLoc){//valid
										 if(entityPtrLoc->List_networkEntity)
											continue;

										 entityPtrLoc->m_curHitCounter = 0;

										 if(entityPtrLoc->m_aiObj)
										 {//only damage target (protection)
                                            if(entityPtrLoc->List_AI_target != entPtr->List_traceNumber)
                                               continue;
										 }//only damage target (protection)

										 if(misPtr->m_minDamageValue != 0)
                                           if(entityPtrLoc->List_networkEntity == FALSE)
										   {//send is local
                                              if(m_gStatus->m_multiplayerObject->m_dmgOutBox->GetCount() < 5)
											  {
                                                 
											     if(!m_gStatus->m_multiplayerObject->m_AIServer)
					                             {
													m_gStatus->m_multiplayerObject->m_localCLSecurityIndexer += (((int)misPtr->m_collisionRadius)+5);
													if(m_gStatus->m_multiplayerObject->m_localCLSecurityIndexer > 30000)
														m_gStatus->m_multiplayerObject->m_localCLSecurityIndexer = 0; 
												 }

										            m_cNetworkObject->SendDamageMsg((short)entPtr->List_networkDefinedID,
							                                                           0,
							                                                           (short)misPtr->m_missileDBRef,
							                                                           (short)entityPtrLoc->List_networkDefinedID,
																				       (short)m_gStatus->m_multiplayerObject->m_localCLSecurityIndexer,
																					   (short)positionMatrix._41,
																					   (short)positionMatrix._42,
																					   (short)positionMatrix._43);
											  }
										   }
										 }//end valid
									 }//end send network data

									 entPtr->List_attackerTrace = misPtr->m_ownerTrace;
									 //m_gStatus->m_systemReadout->AddLine(m_gStatus->m_cabLib.ConvToString(entPtr->List_attackerTrace));
									 break;
						}//end hit   
					 }//end valid
				 }//end damage sphere loop
			  }//end valid locational damage list
			 }//end in calculation radius
			}//end valid collision calc object
		  }//end make sure not owner
	  }//end entity loop
  }//end 
}
//--------------Missile collision check world objects---------------------------------//
void CMissileExplosionManager::MissileCollisionCheckWorld(CCollisionObj * collisionBase,CMissileObjList * missileRuntime,CExplosionObjList * explosionRuntimeList)
{
CPolyObj * collisionDatabase = NULL;
 for(POSITION posLoc = missileRuntime->GetHeadPosition();posLoc != NULL;)
  {//Begin entity loop
    CMissileObj * entLoc = ( CMissileObj* ) missileRuntime->GetNext(posLoc);	
	if(entLoc->m_filterCollision)
	   continue;
	      BOOL tallyPolyColCount = TRUE;
          D3DVECTOR origin;
		  entLoc->m_baseFrame->GetPosition(NULL,&origin);
          D3DVECTOR collisionIntersect,normalRet;
		  int polysExactChecked = 0;
		  int polysConsidered = 0;
		  int ObjectIndexOfCollision = 0;
		  BOOL result = FALSE;
     
		  result = collisionBase->CollisionSegmentSI(
					                 entLoc->m_lastPosition,
					                 origin,
					                 &collisionIntersect.x,
					                 &collisionIntersect.y,
					                 &collisionIntersect.z,
					                 &polysExactChecked,
					                 &polysConsidered,
					                 &normalRet,
					                 &ObjectIndexOfCollision,
					                 20.0f,
					                 TRUE);
          if(!result)
		  result = m_gStatus->RealtimeSegmentTest(  entLoc->m_lastPosition,
													origin,
													&normalRet,
													&collisionIntersect,
													&polysExactChecked,
													&polysConsidered);


          if(result){
              if(m_gStatus->m_selctionModeOn)
			  {
			     CWldObject * wldObj = m_gStatus->m_collisionBase->m_wldObjectReferenceArray[ObjectIndexOfCollision].worldObjectReference;
	             //m_idOfWorldObjectLastHit                = -1;
                 POSITION posWldLoc = m_gStatus->m_wldObjList->FindIndex(ObjectIndexOfCollision);
		         if(wldObj){//1
                  //CWldObject * wldObj = (CWldObject *)m_gStatus->m_wldObjList->GetAt(posWldLoc);
			      m_gStatus->m_idOfWorldObjectLastHit = wldObj->List_identity;
                  //SetCursorToSelectedObject(wldObj->List_identity);
			     }//end 1
			  }
			  /*if(SelectionModeOn){//selection mode on
               CWldObject * wldObj = m_gStatus->m_collisionBase->m_wldObjectReferenceArray[ObjectIndexOfCollision].worldObjectReference;

			   //POSITION posWldLoc = m_gStatus->m_wldObjList->FindIndex(ObjectIndexOfCollision);
		       if(wldObj){//1
                  //CWldObject * wldObj = (CWldObject *)m_gStatus->m_wldObjList->GetAt(posWldLoc);
                  SetCursorToSelectedObject(wldObj->List_identity);
			   }//end 1
			  }//end selection mode on*/
         
          //explosion application
		  D3DVECTOR groundOriPosition;
		  groundOriPosition.x = collisionIntersect.x + normalRet.x;
		  groundOriPosition.y = collisionIntersect.y + normalRet.y;
		  groundOriPosition.z = collisionIntersect.z + normalRet.z;
          
		  BOOL bounces = TRUE;
		  if(entLoc->m_useElastics)
		  {
			D3DMATRIX matrixRotation = m_gStatus->m_matLib.AxisAngleRotationV2(normalRet,180.0f/57.3f);
			
            entLoc->m_physicDynamics->List_curVelocityVect.x = -entLoc->m_physicDynamics->List_curVelocityVect.x;
            entLoc->m_physicDynamics->List_curVelocityVect.y = -entLoc->m_physicDynamics->List_curVelocityVect.y;
			entLoc->m_physicDynamics->List_curVelocityVect.z = -entLoc->m_physicDynamics->List_curVelocityVect.z;
            entLoc->m_physicDynamics->List_curVelocityVect = m_gStatus->m_matLib.UpdatePolygonVertex(matrixRotation.m,entLoc->m_physicDynamics->List_curVelocityVect);
			entLoc->m_physicDynamics->List_curVelocityVect.x = entLoc->m_physicDynamics->List_curVelocityVect.x * entLoc->m_physicDynamics->List_groundDrag;
			entLoc->m_physicDynamics->List_curVelocityVect.y = entLoc->m_physicDynamics->List_curVelocityVect.y * entLoc->m_physicDynamics->List_groundDrag;
			entLoc->m_physicDynamics->List_curVelocityVect.z = entLoc->m_physicDynamics->List_curVelocityVect.z * entLoc->m_physicDynamics->List_groundDrag;
			
			
			D3DVECTOR newPos;
			newPos.x = collisionIntersect.x + entLoc->m_physicDynamics->List_curVelocityVect.x;
			newPos.y = collisionIntersect.y + entLoc->m_physicDynamics->List_curVelocityVect.y;
			newPos.z = collisionIntersect.z + entLoc->m_physicDynamics->List_curVelocityVect.z;
			entLoc->m_baseFrame->List_frameMatrix._41 = newPos.x;
            entLoc->m_baseFrame->List_frameMatrix._42 = newPos.y;
			entLoc->m_baseFrame->List_frameMatrix._43 = newPos.z;
            entLoc->m_physicDynamics->List_lastPosition.x = collisionIntersect.x;
			entLoc->m_physicDynamics->List_lastPosition.y = collisionIntersect.y;
			entLoc->m_physicDynamics->List_lastPosition.z = collisionIntersect.z;
			entLoc->m_lastPosition.x = collisionIntersect.x;
			entLoc->m_lastPosition.y = collisionIntersect.y;
			entLoc->m_lastPosition.z = collisionIntersect.z;
			
			//prevent goin through shit
            /*D3DMATRIX orientation;
			entLoc->m_baseFrame->GetTransform(NULL,&orientation);
			VirtualCollisionAdjust(&newPos,
								    newPos,
								    posVect,
								    1.0f,
								    2.0f,
									orientation,
									FALSE);*/
			

			continue;
		  }
		  
		  BOOL doExplosion = TRUE;


          if(entLoc->m_typeOfProjectile == 3)
			  if(entLoc->m_laserObject->m_terminated)
				 doExplosion = FALSE;

          if(entLoc->m_explosionLink < 0)
			 doExplosion = FALSE;

		  if(doExplosion)
		  {
		   //m_gStatus->m_systemReadout->AddLine("EXP!!",m_gStatus->m_3dDevice,m_gStatus->m_symbolMapDB);
           m_gStatus->m_explosionDBList->CreateExplosion(entLoc->m_explosionLink,
										     collisionIntersect,
										     explosionRuntimeList,
										     groundOriPosition,
											 entLoc->m_ownerTrace,
											 m_gStatus->m_3dDevice,
											 entLoc->m_teamRef,
											 -1
										     );
		   entLoc->m_explosionLink = -1;//do only one
		  }

		  entLoc->m_collidedNormalStore = normalRet;
		  entLoc->m_contactPointStored = collisionIntersect;
		  m_gStatus->m_actualPolysCalculated += polysExactChecked;
		  m_gStatus->m_collisionPolysInUse += polysConsidered;
      
		  BOOL kill = TRUE;

		  if(entLoc->m_typeOfProjectile == 3)
			  if(entLoc->m_laserObject->m_lockDown)
			  {
                 kill = FALSE;
				 entLoc->m_laserObject->m_terminated = TRUE;
                 entLoc->m_baseFrame->SetPosition(NULL,collisionIntersect);
			  }
		         
		  if(kill)		  
		  entLoc->m_lifeTime = 0;
		  continue;
		  }
          m_gStatus->m_actualPolysCalculated += polysExactChecked;
		  m_gStatus->m_collisionPolysInUse += polysConsidered;
		  continue;
 }
}
//-------------------Explosions to entities----------------------------------------//
void CMissileExplosionManager::ExplosionCollisionCallback(CMovementObjList * entObjRuntime,CExplosionObjList * explosionRuntimeList)
{
	for(POSITION posLoc = explosionRuntimeList->GetHeadPosition();posLoc != NULL;)
	{//primary runtime exp loop
        CExplosionObj * expPtr = (CExplosionObj *) explosionRuntimeList->GetNext(posLoc);
		D3DVECTOR explosionCenterWLD;
		expPtr->m_frame->GetPosition(NULL,&explosionCenterWLD);
        if(expPtr->m_radiusDamage == 0.0f)
           continue;

		if(expPtr->m_damageRadius == 0.0f)
           continue;

		if(expPtr->m_traceFollow != -1)
		   continue;

		if(expPtr->m_splashDamageFinsh)
           continue;
		   expPtr->m_splashDamageFinsh = TRUE;
		
		for(POSITION posLoc2 = entObjRuntime->GetHeadPosition();posLoc2 != NULL;)
		{//Begin entity loop
	      CMovementObj * entPtr = ( CMovementObj* ) entObjRuntime->GetNext(posLoc2);
          
		  if(m_gStatus->m_multiplayerObject->m_AIServer)
	         if(entPtr->m_team == expPtr->m_teamRef)
				continue;
		  
		  if(entPtr->List_controlType == 35)
			 continue;

		  if(entPtr->m_possedBy)
		     continue;

	      D3DVECTOR entPosition;
		  entPtr->List_baseFrame->GetPosition(NULL,&entPosition);

		  if(m_gStatus->m_matLib.BoundingSphereCheck(expPtr->m_damageRadius,
			                                        0.0f,
												    explosionCenterWLD.x,explosionCenterWLD.y,explosionCenterWLD.z,
									                entPosition.x,entPosition.y,entPosition.z))
		  {//collides
             D3DVECTOR forceVector;
			 forceVector.x = entPosition.x - explosionCenterWLD.x;
			 forceVector.y = entPosition.y - explosionCenterWLD.y;
			 forceVector.z = entPosition.z - explosionCenterWLD.z;
             if(forceVector.x == 0.0f &&
				forceVector.y == 0.0f &&
				forceVector.z == 0.0f)
				continue;


			 forceVector = m_gStatus->m_matLib.Normalize(forceVector);
							
			
			 if(!m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
                 entPtr->List_energy -= expPtr->m_radiusDamage;
		     else{//send network data
                 CMovementObj * entityPtrLoc = entObjRuntime->GetObjByTrace(expPtr->m_ownerTrace); 
                 if(entityPtrLoc){//valid
				 if(entityPtrLoc->List_networkEntity)
											continue;

				 entityPtrLoc->m_curHitCounter = 0;
 
				 if(entPtr->List_energy > 0)
                   if(entityPtrLoc->List_networkEntity == FALSE)
				   {//send is local
                    if(m_gStatus->m_multiplayerObject->m_dmgOutBox->GetCount() < 5)
					{
					   if(!m_gStatus->m_multiplayerObject->m_AIServer)
					   {
                         m_gStatus->m_multiplayerObject->m_localCLSecurityIndexer += (((int)expPtr->m_damageRadius)+6);
	                     if(m_gStatus->m_multiplayerObject->m_localCLSecurityIndexer > 30000)
	                        m_gStatus->m_multiplayerObject->m_localCLSecurityIndexer = 0; 
					   }

						  m_cNetworkObject->SendDamageMsg((short)entPtr->List_networkDefinedID,
							                                 2,
							                                 (short)expPtr->m_dbIndexRef,
							                                 (short)entityPtrLoc->List_networkDefinedID,
													         (short)m_gStatus->m_multiplayerObject->m_localCLSecurityIndexer,
															 (short)entPosition.x,
															 (short)entPosition.y,
															 (short)entPosition.z);
					}

				    entPtr->List_attackerTrace = expPtr->m_ownerTrace;

				   }//end send is local
				 }//end valid
			 }//end send network data
		  }//end collides
		}//end entity loop
	}//end primary runtime exp loop
}
//--------------runtime missile adjuster----------------------------------------------//
void CMissileExplosionManager::MissileCallback(CMissileObjList * missileRuntime,
											   CExplosionObjList * explosionRuntimeList,
											   float & adjust,
											   double & lastAdjust)
{
  long currentTickCount = GetTickCount();
  POSITION posLast;
  for(POSITION posLoc = missileRuntime->GetHeadPosition();(posLast = posLoc) != NULL;){//visual loop
	CMissileObj * mPtr = ( CMissileObj* ) missileRuntime->GetNext(posLoc);
	D3DVECTOR currentPosition;
	mPtr->m_baseFrame->GetPosition(NULL,&mPtr->m_lastPosition);
	mPtr->m_baseFrame->GetPosition(NULL,&currentPosition);
    if(mPtr->m_skeletalVisual)
	   mPtr->m_skeletalVisual->AnimationCallback(currentTickCount,
			                                     m_gStatus->m_soundCenterMatrix,
											     currentPosition,
										         m_gStatus->GL_globalSoundFactor,
											     FALSE,
												 m_gStatus->m_soundManager,
												 FALSE,
												 m_gStatus->m_multiplayerObject->m_AIServer,
												 m_gStatus->m_3dDevice,
											     m_gStatus->m_d3dVectorInit,
												 m_gStatus->m_oneVal,
												 m_gStatus->m_lodOverride
												 );
	D3DMATRIX matrixLoc;
	   if((currentTickCount - mPtr->m_startTimeStamp) < mPtr->m_lifeTime){//still valid
       //heatseeking
	    
       //end heat seeking
       //accel basic
	   if(mPtr->m_physicDynamics->List_curSpeed < mPtr->m_physicDynamics->List_maxSpeed){
          mPtr->m_physicDynamics->List_curSpeed+=(mPtr->m_physicDynamics->List_accellerate);
	   }
       //end accel basic
	   if(!mPtr->m_useFullPhysics)
	   {//simple physics
		BOOL doSimplePhysics = TRUE;
		 
		if(mPtr->m_laserObject)
		if(mPtr->m_laserObject->m_terminated)//dont push if terminated
		   doSimplePhysics = FALSE;
		if(doSimplePhysics)
		{//8
	     m_gStatus->m_matLib.Translate ( 0.0f,0.0f,(-mPtr->m_physicDynamics->List_curSpeed * adjust),matrixLoc.m);
	     mPtr->m_baseFrame->AddTransformBefore(matrixLoc);
		}//end 8
       }//end simple physics
	   else{//full physics implemented
	     //physics enabled
         //ADDITIONAL ENTITY FORCES
         float AdditionalForceX = 0.0f;
         float AdditionalForceY = 0.0f;
         float AdditionalForceZ = 0.0f;
         //EXTERNAL FORCES
         float ExternalForceX = 0.0f;//m_gStatus->m_environmentPtr->m_windDirectionX;
         float ExternalForceY = 0.0f;//m_gStatus->m_environmentPtr->m_windDirectionY;
         float ExternalForceZ = 0.0f;//m_gStatus->m_environmentPtr->m_windDirectionZ;
       
	     mPtr->m_physicDynamics->List_curTransX = 0.0f;
	     mPtr->m_physicDynamics->List_curTransY = 0.0f;
		 mPtr->m_physicDynamics->List_curTransZ = 0.0f;
	     if(mPtr->m_burstAppliedState == FALSE || mPtr->m_burstForceType == FALSE){
			AdditionalForceZ = -mPtr->m_physicDynamics->List_maxSpeed;
            mPtr->m_burstAppliedState = TRUE;
		 }

		 D3DVECTOR orig;
         float submergeValue = 0.0f;
         mPtr->m_baseFrame->GetPosition(NULL,&orig);
         BOOL inWater = m_gStatus->m_waterZoneDB->IsPositionInWater(orig,&submergeValue);

		 mPtr->m_physicDynamics->List_doGravity = TRUE;
	     mPtr->m_physicDynamics->DoPhysics( (double)adjust,
											mPtr->m_baseFrame,
											mPtr->m_physicDynamics,
											AdditionalForceX,
											AdditionalForceY,
											AdditionalForceZ,
											ExternalForceX,
											ExternalForceY,
											ExternalForceZ,
											0.0f,
											0.0f,
											FALSE,
											FALSE,
											FALSE,
											submergeValue,
											m_gStatus->m_environmentPtr->m_seaViscusDrag,
											m_gStatus->m_environmentPtr->m_gravity,
											lastAdjust,
											NULL,
											m_gStatus->m_systemReadout);
	   
	   }//end full physics implemented
	   //patch code
	   //Update sound
	   
	   if(m_gStatus->m_dirSnd)
	   if(mPtr->m_enableSound){
          if(mPtr->m_soundHandle)
		  {
			  //D3DMATRIX matrixPosition;
			  D3DVECTOR wldSpacePos;
              mPtr->m_baseFrame->GetPosition(NULL,&wldSpacePos);
              mPtr->m_soundHandle->UpdatePosition(wldSpacePos);
		  }
		  else{
			  D3DVECTOR wldSpacePos;
              mPtr->m_baseFrame->GetPosition(NULL,&wldSpacePos);
              mPtr->m_soundHandle = m_gStatus->m_soundManager->PlayByID(mPtr->m_sndID,TRUE,wldSpacePos,mPtr->m_distanceFactor,1000.0f);
			  
		  }
	   }


         if(mPtr->m_particleEmmiter)
            mPtr->m_baseFrame->GetTransform(NULL,&mPtr->m_particleEmmiter->m_passedInMatrix);
	   }//end still valid
	   else{
	   D3DVECTOR expLocation;
	   mPtr->m_baseFrame->GetPosition(NULL,&expLocation);
	    //if(mPtr->m_contactPointStored.x != 0.0f || mPtr->m_contactPointStored.y != 0.0f || mPtr->m_contactPointStored.z != 0.0f){
	     //UseExplosion(mPtr->m_contactPointStored,mPtr->List_explosionAtribute,TRUE,mPtr->List_colidedNormalStor);
	     //SetRuntimeBulletHoleObject(mPtr->List_leaveMark,mPtr->List_contactPointStor,mPtr->List_colidedNormalStor,scene);
		//}
	    if(mPtr->m_explosionLink > -1)
	    if(mPtr->m_forceExplodeAtEnd == TRUE)
		   m_gStatus->m_explosionDBList->CreateExplosion(mPtr->m_explosionLink,
										      expLocation,
										      explosionRuntimeList,
										      expLocation,
											  mPtr->m_ownerTrace,
											  m_gStatus->m_3dDevice,
											  mPtr->m_teamRef,
											  -1
										      );
		   //UseExplosion(expLocation,mPtr->List_explosionAtribute,TRUE,mPtr->List_colidedNormalStor);
	    
		if(mPtr->m_particleEmmiter)
            mPtr->m_particleEmmiter->m_Regenerate = FALSE;
	     m_gStatus->DestroyMissileObject(mPtr);
         missileRuntime->RemoveAt(posLast);
	   }
  }
}
//------------------------------------------------------------------------------------//
void CMissileExplosionManager::ExplosionCallback(CMovementObjList * entObjRuntime,CExplosionObjList * explosionRuntimeList)
{ 
  long explosionCurTime=GetTickCount();
  POSITION posLast = NULL;
  for(POSITION misPos = explosionRuntimeList->GetHeadPosition();(posLast = misPos) != NULL;)
  {
      CExplosionObj * expObj = (CExplosionObj *)explosionRuntimeList->GetNext(misPos);
      long timePassed = explosionCurTime - expObj->m_beginTimeStamp;
      
	  if(expObj->m_traceFollow > -1)
	  {
	     CMovementObj * mPtr = entObjRuntime->GetObjByTrace(expObj->m_traceFollow);
		 if(mPtr)
		 {
            D3DVECTOR positionTemp;
            mPtr->List_baseFrame->GetPosition(NULL,&positionTemp);
            expObj->m_frame->SetPosition(NULL,positionTemp);
		 }
	  }
	  
	  if(expObj->m_duration < timePassed){//destroy this 
		 expObj->SafeDelete();
		 delete expObj;
		 expObj=0;
         explosionRuntimeList->RemoveAt(posLast);
         continue;
	  }//end destroy this
  }
}
//--------------Missile collision check pipline (all checks)---------------------------------------//
void CMissileExplosionManager::MissileCollisionPipeline(CMovementObjList * entObjRuntime,
														CMissileObjList * missileRuntime,
														CExplosionObjList * explosionRuntimeList,
														CCollisionObj * collisionBase)
{
	D3DVECTOR currentPosition,bestIntersection,groundOriPosition,latestNormal;
	for(POSITION posLoc = missileRuntime->GetHeadPosition();posLoc != NULL;)
    {//loop through missile objects
      CMissileObj * misPtr = ( CMissileObj* ) missileRuntime->GetNext(posLoc);
	  if(misPtr->m_lifeTime == 0)
		 continue; 
	  if(misPtr->m_laserObject)
	    if(misPtr->m_laserObject->m_terminated == TRUE)
          continue;

	  CMovementObj * entityThatWasAttacked = NULL;
	  float bestDistance = 100000.0f;
	  bool collided = false;
	  groundOriPosition.x = 0.0f;
	  groundOriPosition.y = 1.0f;
      groundOriPosition.z = 0.0f;
      latestNormal = groundOriPosition;
      
	  misPtr->m_baseFrame->GetPosition(NULL,&currentPosition);
	  //check to entities
	  collided = MissileToEntityUseESpheres(misPtr,
											currentPosition,
											entObjRuntime,
											missileRuntime,
											&entityThatWasAttacked,
											bestIntersection,
											bestDistance);
      //check dynamics here



	  //check to statics
	  if(MissileToStaticCollision(misPtr,
								  collisionBase,
								  missileRuntime,
							      explosionRuntimeList,
								  bestIntersection,
								  bestDistance,
								  groundOriPosition,
								  latestNormal,
								  NULL,
								  NULL)==true)
	  {//collided with closer static
         entityThatWasAttacked = NULL;
		 collided = true;
	  }//end collided with closer static
	  //check to houses
	  if(m_gStatus->RealtimeHousingSegment( misPtr->m_lastPosition,
											currentPosition,
											&latestNormal,
											&bestIntersection,
											bestDistance)==true)
	  {//collided with house
         entityThatWasAttacked = NULL;
		 collided = true;
	  }//end collided with house

	  //do realtime entity collision callback
	  /*for(POSITION pPos = entObjRuntime->GetHeadPosition();pPos != NULL;)
	  {
         CMovementObj * mPtr = (CMovementObj *) entObjRuntime->GetNext(pPos);
         entObjRuntime->SegmentToEntityDistPersistence(  misPtr->m_lastPosition,//D3DVECTOR & startPoint,
													     currentPosition,//D3DVECTOR & endPoint,
													     mPtr->List_skeletalObject,//CSkeletonObject * skeletalCmpare,
													     &m_gStatus->m_matLib,//MatrixARB * mathLib,
													     &bestIntersection,//D3DVECTOR * intersectRet,
														 mPtr->List_baseFrame->List_frameMatrix,//D3DMATRIX & compareBaseMatrix,
														 latestNormal,//D3DVECTOR & latestNormal,
														 bestDistance);

	  }*/

      //realtime to skeletal reference
	  D3DMATRIX objMatTemp;
	  D3DVECTOR boxMax,boxMin;
	  m_gStatus->m_matLib.SegmentToBox(misPtr->m_lastPosition, currentPosition, boxMax, boxMin);
	  for(POSITION posSkLoc = m_gStatus->m_skeletalReferenceObjCache.GetHeadPosition();posSkLoc != NULL;)
	  {//loop through all references that were cached near the entity
        CWldObject * wPtr = (CWldObject *) m_gStatus->m_skeletalReferenceObjCache.GetNext(posSkLoc);
		if(wPtr->m_node->m_refPtr)
		{//use mem references
           CReferenceObj * referenceObject = (CReferenceObj *)wPtr->m_node->m_refPtr;
		   		
		   if(referenceObject->m_skeletalOptionLive)
           if(m_gStatus->m_matLib.BoundingBoxToBoxCheck(
												  wPtr->m_node->m_clipBox.maxX,
												  wPtr->m_node->m_clipBox.maxY,
												  wPtr->m_node->m_clipBox.maxZ,
												  wPtr->m_node->m_clipBox.minX,
												  wPtr->m_node->m_clipBox.minY,
												  wPtr->m_node->m_clipBox.minZ,
												  boxMax.x,//float & b2maxX,
												  boxMax.y,//float & b2maxY,
												  boxMax.z,//
												  boxMin.x,//float & b2minX,
												  boxMin.y,//float & b2minY,
												  boxMin.z))
			   if(entObjRuntime->SegmentToEntityDistPersistence( misPtr->m_lastPosition,//D3DVECTOR & startPoint,
																 currentPosition,//D3DVECTOR & endPoint,
																 referenceObject->m_skeletalOptionLive,//CSkeletonObject * skeletalCmpare,
																 &m_gStatus->m_matLib,//MatrixARB * mathLib,
																 &bestIntersection,//D3DVECTOR * intersectRet,
																 wPtr->m_node->m_position,//D3DMATRIX & compareBaseMatrix,
																 latestNormal,//D3DVECTOR & latestNormal,
																 &objMatTemp,
																 bestDistance))
			   {
                  bestIntersection = m_gStatus->m_matLib.UpdatePolygonVertex(objMatTemp.m,bestIntersection);
				  latestNormal = m_gStatus->m_matLib.UpdatePolygonVertexRotationOnly(objMatTemp.m,latestNormal);
				  groundOriPosition.x = bestIntersection.x + latestNormal.x;
		          groundOriPosition.y = bestIntersection.y + latestNormal.y;
		          groundOriPosition.z = bestIntersection.z + latestNormal.z;
                  
                  entityThatWasAttacked = NULL;
		          collided = true;
			   }
		}
	  }


	  if(collided)
	  {//handle contact
        MissileCollisionHandler(misPtr,
							    groundOriPosition,
								bestIntersection,
								entityThatWasAttacked,
								explosionRuntimeList,
								currentPosition,
								entObjRuntime,
								latestNormal);
	  }//end handle contact
	  
	  //check entities for collisions
	}//end loop through missile objects
}
//--------------Missile To Entity use spheres (all checks)---------------------------------------//
bool CMissileExplosionManager::MissileToEntityUseESpheres(CMissileObj * misPtr,
														  D3DVECTOR & currentPosition,
														  CMovementObjList * entObjRuntime,
														  CMissileObjList * missileRuntime,
														  CMovementObj ** attackedEntity,
														  D3DVECTOR & closestIntesection,
														  float & bestDistance)
{
     D3DVECTOR virCenter;
	 bool collided = false;
     float virRadius,curDist;
     //build virtuals
	 virRadius = m_gStatus->m_matLib.MagnitudeSquared(misPtr->m_lastPosition.x,misPtr->m_lastPosition.y,misPtr->m_lastPosition.z,
		                                                     currentPosition.x,currentPosition.y,currentPosition.z);	
	 virRadius = (virRadius*.5f) + misPtr->m_collisionRadius;//build radius
	 virCenter.x = misPtr->m_lastPosition.x + ((currentPosition.x - misPtr->m_lastPosition.x)*.5f);
     virCenter.y = misPtr->m_lastPosition.y + ((currentPosition.y - misPtr->m_lastPosition.y)*.5f);
	 virCenter.z = misPtr->m_lastPosition.z + ((currentPosition.z - misPtr->m_lastPosition.z)*.5f);

     for(POSITION posLoc2 = entObjRuntime->GetHeadPosition();posLoc2 != NULL;)
	  {//Begin entity loop
	      CMovementObj * entPtr = ( CMovementObj* ) entObjRuntime->GetNext(posLoc2);
          
		  if(m_gStatus->m_multiplayerObject->m_AIServer)
	         if(entPtr->m_team == misPtr->m_teamRef)
				continue;
		  
		  if(entPtr->List_controlType == 35)
			 continue;

		  if(entPtr->m_possedBy)
		     continue;

		  if(!entPtr->List_baseFrame || !entPtr->List_skeletalObject)//not fully built - can't be hit
		     continue;

	      if(entPtr->List_energy > 0)//make sure alive
		  if(entPtr->List_traceNumber != misPtr->m_ownerTrace)
		  {//make sure not owner
			D3DMATRIX positionMatrix;
			entPtr->List_baseFrame->GetTransform(NULL,&positionMatrix);
            if(entPtr->List_skeletalObject->m_calulationSphere)
			{//valid collision calc object
			   D3DVECTOR bonePosition = m_gStatus->m_matLib.UpdatePolygonVertex(positionMatrix.m,entPtr->List_skeletalObject->GetBonePositionByIndex(entPtr->List_skeletalObject->m_calulationSphere->m_boneIndex));
               //get virtual Radius
			   

			   if(m_gStatus->m_matLib.BoundingSphereCheckX(virRadius,
                                                           entPtr->List_skeletalObject->m_calulationSphere->m_sphereRadius,
													       bonePosition.x,
													       bonePosition.y,
													       bonePosition.z,
                                                           virCenter.x,
													       virCenter.y,
												 	       virCenter.z))
			   {//in calculation radius
			     D3DVECTOR bnPosition,intersection;
			     if(entPtr->List_skeletalObject->m_sphereLocationalDamageList){//valid locational damage list
		         for(POSITION dmgPos = entPtr->List_skeletalObject->m_sphereLocationalDamageList->GetHeadPosition();dmgPos != NULL;)
				 {//damage sphere loop
		             CSphereCollisionObj * dmgSph = (CSphereCollisionObj *)entPtr->List_skeletalObject->m_sphereLocationalDamageList->GetNext(dmgPos);
		             POSITION bnPos = entPtr->List_skeletalObject->m_bonesList->FindIndex(dmgSph->m_boneIndex);
                     if(bnPos){//valid
                        CBoneObject * bnPtr = (CBoneObject *)entPtr->List_skeletalObject->m_bonesList->GetAt(bnPos);
						bnPtr->m_boneFrame->GetPosition(NULL,&bnPosition);
                        bnPosition = m_gStatus->m_matLib.UpdatePolygonVertex(positionMatrix.m,bnPosition);
						if(m_gStatus->m_matLib.CollisionSphereSeg(currentPosition,
																  misPtr->m_lastPosition,
					                                              dmgSph->m_sphereRadius,
																  bnPosition,
													              FALSE,
																  NULL,
																  misPtr->m_collisionRadius,
																  &intersection)==TRUE)
						{//hit
									  
                                     curDist = m_gStatus->m_matLib.MagnitudeSquared(misPtr->m_lastPosition.x,misPtr->m_lastPosition.y,misPtr->m_lastPosition.z,
										                                            intersection.x,intersection.y,intersection.z);

									 if(curDist < bestDistance)
									 {//
                                        bestDistance = curDist;
										CopyMemory(&closestIntesection,&intersection,sizeof(D3DVECTOR));
                                        *attackedEntity = entPtr;
										collided = true;
									 }//
						}//end hit   
					 }//end valid
				 }//end damage sphere loop
			  }//end valid locational damage list
			 }//end in calculation radius
			}//end valid collision calc object
		  }//end make sure not owner
	  }//end entity loop
      return collided;
}
//--------------Missile collision check world objects---------------------------------//
bool CMissileExplosionManager::MissileToStaticCollision(CMissileObj * misPtr,
													    CCollisionObj * collisionBase,
													    CMissileObjList * missileRuntime,
														CExplosionObjList * explosionRuntimeList,
														D3DVECTOR & closestIntesection,
														float & bestDistance,
														D3DVECTOR & groundOriPosition,
														D3DVECTOR & normalReference,
														D3DMATRIX * transformOfCollisionSys,
													    D3DMATRIX * invTransformOfCollisionSys)
{
	D3DVECTOR origin;
	misPtr->m_baseFrame->GetPosition(NULL,&origin);
    if(invTransformOfCollisionSys)
	{
		 misPtr->m_lastPosition.x += invTransformOfCollisionSys->_41;
		 misPtr->m_lastPosition.y += invTransformOfCollisionSys->_42;
		 misPtr->m_lastPosition.z += invTransformOfCollisionSys->_43;

		 origin.x += invTransformOfCollisionSys->_41;
		 origin.y += invTransformOfCollisionSys->_42;
		 origin.z += invTransformOfCollisionSys->_43;
    }


    CPolyObj * collisionDatabase = NULL;	
	if(misPtr->m_filterCollision)
	   return false;

	      bool collided = false;
	      BOOL tallyPolyColCount = TRUE;
          D3DVECTOR collisionIntersect,normalRet;
		  int polysExactChecked = 0;
		  int polysConsidered = 0;
		  int ObjectIndexOfCollision = 0;
		  float curDist,dist;
          byte temp = 0;
		  if(collisionBase->GetClosesIntersectionUseSeg(
																		misPtr->m_lastPosition,
																		origin,
																		misPtr->m_lastPosition,
																		origin,
																		&collisionIntersect.x,
																		&collisionIntersect.y,
																		&collisionIntersect.z,
																		&ObjectIndexOfCollision,
																		&normalRet,
																		false,
																		dist,
																		temp
																		)==TRUE)
		  {//collided 
            if(m_gStatus->m_selctionModeOn)
			  {
			     CWldObject * wldObj = m_gStatus->m_collisionBase->m_wldObjectReferenceArray[ObjectIndexOfCollision].worldObjectReference;
	             //m_idOfWorldObjectLastHit                = -1;
                 POSITION posWldLoc = m_gStatus->m_wldObjList->FindIndex(ObjectIndexOfCollision);
		         if(wldObj){//1
                  //CWldObject * wldObj = (CWldObject *)m_gStatus->m_wldObjList->GetAt(posWldLoc);
			      m_gStatus->m_idOfWorldObjectLastHit = wldObj->List_identity;
                  //SetCursorToSelectedObject(wldObj->List_identity);
			     }//end 1
			  }
            curDist = m_gStatus->m_matLib.MagnitudeSquared(misPtr->m_lastPosition.x,misPtr->m_lastPosition.y,misPtr->m_lastPosition.z,
										                   collisionIntersect.x,collisionIntersect.y,collisionIntersect.z);
			if(curDist < bestDistance)
			{//
              bestDistance = curDist;
			  CopyMemory(&closestIntesection,&collisionIntersect,sizeof(D3DVECTOR));

              groundOriPosition.x = collisionIntersect.x + normalRet.x;
		      groundOriPosition.y = collisionIntersect.y + normalRet.y;
		      groundOriPosition.z = collisionIntersect.z + normalRet.z;

			  normalReference = normalRet;
			  collided = true;
			}//
		  }//end collided

		  if(transformOfCollisionSys)
          {
		     misPtr->m_lastPosition.x += transformOfCollisionSys->_41;
		     misPtr->m_lastPosition.y += transformOfCollisionSys->_42;
		     misPtr->m_lastPosition.z += transformOfCollisionSys->_43;
	      }

		  return collided;
}
//-------------------------------missile collision reactor------------------------------//
bool CMissileExplosionManager::MissileCollisionHandler(CMissileObj * misPtr,
													   D3DVECTOR & groundOriPosition,
													   D3DVECTOR & collisionIntersect,
													   CMovementObj * entityThatWasAttacked,
													   CExplosionObjList * explosionRuntimeList,
													   D3DVECTOR & currentPosition,
													   CMovementObjList * entObjRuntime,
													   D3DVECTOR & normalRet)
{
        if(misPtr->m_useElastics)
		{
			D3DMATRIX matrixRotation = m_gStatus->m_matLib.AxisAngleRotationV2(normalRet,180.0f/57.3f);
			
            misPtr->m_physicDynamics->List_curVelocityVect.x = -misPtr->m_physicDynamics->List_curVelocityVect.x;
            misPtr->m_physicDynamics->List_curVelocityVect.y = -misPtr->m_physicDynamics->List_curVelocityVect.y;
			misPtr->m_physicDynamics->List_curVelocityVect.z = -misPtr->m_physicDynamics->List_curVelocityVect.z;
            misPtr->m_physicDynamics->List_curVelocityVect = m_gStatus->m_matLib.UpdatePolygonVertex(matrixRotation.m,misPtr->m_physicDynamics->List_curVelocityVect);
			misPtr->m_physicDynamics->List_curVelocityVect.x = misPtr->m_physicDynamics->List_curVelocityVect.x * misPtr->m_physicDynamics->List_groundDrag;
			misPtr->m_physicDynamics->List_curVelocityVect.y = misPtr->m_physicDynamics->List_curVelocityVect.y * misPtr->m_physicDynamics->List_groundDrag;
			misPtr->m_physicDynamics->List_curVelocityVect.z = misPtr->m_physicDynamics->List_curVelocityVect.z * misPtr->m_physicDynamics->List_groundDrag;
			
			
			D3DVECTOR newPos;
			newPos.x = collisionIntersect.x + misPtr->m_physicDynamics->List_curVelocityVect.x;
			newPos.y = collisionIntersect.y + misPtr->m_physicDynamics->List_curVelocityVect.y;
			newPos.z = collisionIntersect.z + misPtr->m_physicDynamics->List_curVelocityVect.z;
			misPtr->m_baseFrame->List_frameMatrix._41 = newPos.x;
            misPtr->m_baseFrame->List_frameMatrix._42 = newPos.y;
			misPtr->m_baseFrame->List_frameMatrix._43 = newPos.z;
            misPtr->m_physicDynamics->List_lastPosition.x = collisionIntersect.x;
			misPtr->m_physicDynamics->List_lastPosition.y = collisionIntersect.y;
			misPtr->m_physicDynamics->List_lastPosition.z = collisionIntersect.z;
			misPtr->m_lastPosition.x = collisionIntersect.x;
			misPtr->m_lastPosition.y = collisionIntersect.y;
			misPtr->m_lastPosition.z = collisionIntersect.z;
			
			return true;
		 }
	    D3DVECTOR locationExpRef;
	    misPtr->m_baseFrame->GetPosition(NULL,&locationExpRef);
		

		BOOL doExplosion = TRUE;

		if(misPtr->m_typeOfProjectile == 3)
		if(misPtr->m_laserObject->m_terminated)
			doExplosion = FALSE;

		if(misPtr->m_explosionLink < 0)
		   doExplosion = FALSE;

		if(doExplosion){
		   m_gStatus->m_explosionDBList->CreateExplosion(   misPtr->m_explosionLink,
															collisionIntersect,
															explosionRuntimeList,
															groundOriPosition,
															misPtr->m_ownerTrace,
															m_gStatus->m_3dDevice,
															misPtr->m_teamRef,
															-1
															); 
		   misPtr->m_explosionLink =-1;//dont let this explode again
		}

		BOOL kill = TRUE;
        if(misPtr->m_typeOfProjectile == 3)
		if(misPtr->m_laserObject->m_lockDown)
		{
            kill = FALSE;
			misPtr->m_laserObject->m_terminated = TRUE;
            misPtr->m_baseFrame->SetPosition(NULL,collisionIntersect);
		}

		if(kill)		  
			misPtr->m_lifeTime = 0;

		//physics
		if(entityThatWasAttacked)
		{
			D3DVECTOR forceVector;
			forceVector.x = currentPosition.x - misPtr->m_lastPosition.x;
			forceVector.y = currentPosition.y - misPtr->m_lastPosition.y;
			forceVector.z = currentPosition.z - misPtr->m_lastPosition.z;
			forceVector = m_gStatus->m_matLib.Normalize(forceVector);

			//entityThatWasAttacked->List_externalVectorTemp.x += forceVector.x * misPtr->m_pushPower;
			//entityThatWasAttacked->List_externalVectorTemp.y += forceVector.y * misPtr->m_pushPower;
			//entityThatWasAttacked->List_externalVectorTemp.z += forceVector.z * misPtr->m_pushPower;
		}

		if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe() == FALSE && entityThatWasAttacked != NULL)
		{
			int damage = 0;
			if((misPtr->m_maxDamageValue - misPtr->m_minDamageValue) > 0)
				damage = misPtr->m_minDamageValue + (rand() % (misPtr->m_maxDamageValue - misPtr->m_minDamageValue));
			else
				damage = misPtr->m_minDamageValue;
			float damageMult = 1.0f;
			
			D3DMATRIX positionMatrix;
			entityThatWasAttacked->List_baseFrame->GetTransform(NULL,&positionMatrix);

			if(entityThatWasAttacked->List_skeletalObject->m_sphereLocationalDamageList)
			{//valid locational damage list
				for(POSITION dmgPos = entityThatWasAttacked->List_skeletalObject->m_sphereLocationalDamageList->GetHeadPosition();dmgPos != NULL;)
				{
					CSphereCollisionObj * dmgSph = (CSphereCollisionObj *)entityThatWasAttacked->List_skeletalObject->m_sphereLocationalDamageList->GetNext(dmgPos);
					POSITION bnPos = entityThatWasAttacked->List_skeletalObject->m_bonesList->FindIndex(dmgSph->m_boneIndex);
					if(bnPos)
					{//valid
					CBoneObject * bnPtr = (CBoneObject *)entityThatWasAttacked->List_skeletalObject->m_bonesList->GetAt(bnPos);
					D3DVECTOR bnPosition;
					bnPtr->m_boneFrame->GetPosition(NULL,&bnPosition);
					bnPosition = m_gStatus->m_matLib.UpdatePolygonVertex(positionMatrix.m,bnPosition);

					if(m_gStatus->m_matLib.CollisionSphereSeg(collisionIntersect,
															  misPtr->m_lastPosition,
															  dmgSph->m_sphereRadius,
															  bnPosition,
															  FALSE,
															  NULL,
															  misPtr->m_collisionRadius,
															  NULL)==TRUE)
/*					  if(m_gStatus->m_matLib.BoundingSphereCheck( dmgSph->m_sphereRadius,
																  misPtr->m_collisionRadius,															
																  misPtr->m_lastPosition.x,
																  misPtr->m_lastPosition.y,
																  misPtr->m_lastPosition.z,
																  bnPosition.x,
																  bnPosition.y,
																  bnPosition.z))*/
							if(dmgSph->m_damageMultiplier >= damageMult)
								damageMult = dmgSph->m_damageMultiplier;
					}
				}
			}

			/*entityThatWasAttacked->List_energy -= (int)(damage) * damageMult;*/
			entityThatWasAttacked->List_energy -= (int)(((float)(damage)) * damageMult);
			//WARNING: C4244 --> float to int

			entityThatWasAttacked->m_curHitCounter++;
		}
		else if(entityThatWasAttacked)
		{//send network data
			CMovementObj * entityPtrLoc = entObjRuntime->GetObjByTrace(misPtr->m_ownerTrace); 
			if(entityPtrLoc){//valid
			if(entityPtrLoc->List_networkEntity)
			   return true;

			entityPtrLoc->m_curHitCounter = 0;

			if(entityPtrLoc->m_aiObj)
			{//only damage target (protection)
            if(entityPtrLoc->List_AI_target != entityThatWasAttacked->List_traceNumber)
               return true;
			}//only damage target (protection)

			if(misPtr->m_minDamageValue != 0)
            if(entityPtrLoc->List_networkEntity == FALSE)
			{//send is local
                if(m_gStatus->m_multiplayerObject->m_dmgOutBox->GetCountProtected() < 5 || m_gStatus->m_multiplayerObject->m_AIServer == TRUE)
				{
                    
					if(!m_gStatus->m_multiplayerObject->m_AIServer)
					{
						m_gStatus->m_multiplayerObject->m_localCLSecurityIndexer += (((int)misPtr->m_collisionRadius)+5);
						if(m_gStatus->m_multiplayerObject->m_localCLSecurityIndexer > 30000)
							m_gStatus->m_multiplayerObject->m_localCLSecurityIndexer = 0; 
					}
					
					if(entityPtrLoc->List_controlType == 5)//AI ONLY this will stop a misconfigured spam looping weapon
					   entityPtrLoc->m_aiObj->m_attackingCurrently		= FALSE;
					
					m_cNetworkObject->SendDamageMsg((short)entityThatWasAttacked->List_networkDefinedID,
							                        0,
							                        (short)misPtr->m_missileDBRef,
							                        (short)entityPtrLoc->List_networkDefinedID,
													(short)m_gStatus->m_multiplayerObject->m_localCLSecurityIndexer,
													(short)entityThatWasAttacked->List_baseFrame->List_frameMatrix._41,
													(short)entityThatWasAttacked->List_baseFrame->List_frameMatrix._42,
													(short)entityThatWasAttacked->List_baseFrame->List_frameMatrix._43);


					
				}
			}
			}//end valid
		}//end send network data

		if(entityThatWasAttacked)
	       entityThatWasAttacked->List_attackerTrace = misPtr->m_ownerTrace;	

		return true;
}