#include "StdAfx.h"
#include "entitymanagement.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEntityManagement::CEntityManagement(GlobalStatusVariables * gStatus,UserController * userControls,CAiManager * aiManager,CClientNetworkingLogic * clientNet)
{
    m_gStatus				= gStatus;
	m_userControls			= userControls;
	m_aiManager				= aiManager;
	m_clientNet				= clientNet;
	m_lastAutoQuerryStamp   = GetTickCount();
	m_insideInteractRadius	= FALSE;
	m_flag					= 0;
}

CEntityManagement::~CEntityManagement(void)
{
}


//--------------maintains control callback on entities--------------------------------//
void CEntityManagement::UniversalControlUnit(CWaterZoneObjList * waterZoneDB) 
{
	//-1=not controlled
	//0=control config   keyboard/mouse or joystick
	for(POSITION posLoc = m_gStatus->m_entObjRuntime->GetHeadPosition();posLoc != NULL;)
	{//Begin movement entity loop
		CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetNext(posLoc);		    	
		if(ptrLoc->List_controlType != -1)
		{//if has a controller attached
			if(ptrLoc->List_controlType == 0)
			{//control config in use
				if(!m_gStatus->Lagged)
				{
					if(m_gStatus->m_editorModeOn && !m_gStatus->m_globalAvVar)
						m_gStatus->m_globalAvVar = TRUE;

					if(ptrLoc->m_firstPersonMode == TRUE)
					{
					   if(ptrLoc->List_movementDB->m_pckSys_firstPersonMode == 0 || !m_gStatus->m_showMenuCursor)//realtime control processor
					      m_userControls->UserControlUpdate(ptrLoc,ptrLoc->List_controlConfig);
					   else
                          m_userControls->PointClickController(ptrLoc);
					}
					else
					{
                       if(ptrLoc->List_movementDB->m_pckSys_thirdPersonMode == 0 || !m_gStatus->m_showMenuCursor)//realtime control processor
					      m_userControls->UserControlUpdate(ptrLoc,ptrLoc->List_controlConfig);
					   else
                          m_userControls->PointClickController(ptrLoc);
					}
				}
			}//end control config in use
			
			if(ptrLoc->List_controlType == 5)
			{//AI
				long stampTest2 = GetTickCount();
				m_aiManager->AI_LoopStateX(ptrLoc,m_gStatus->GL_aiWebDatabase,waterZoneDB,m_gStatus->m_entObjRuntime,m_gStatus->m_collisionBase,m_gStatus->m_timeAdjustFloat,m_gStatus->m_lastTimeAdjust);

				//REMOVE
				//D3DVECTOR testPos;
				//ptrLoc->List_baseFrame->GetPosition(NULL,&testPos);
				//m_gStatus->m_multiplayerObject->m_eventLog->AddEvent(ptrLoc->List_name + ":",testPos);
				//END REMOVE

				long duration = GetTickCount() - stampTest2;
				if(duration > m_gStatus->m_performanceTimeStamp)
					m_gStatus->m_performanceTimeStamp = duration;	   
			}//End AI

			if(ptrLoc->List_controlType == 35)
			{//item
			   
			}//End item
			if(ptrLoc->List_controlType == 999 || ptrLoc->List_controlType == 1000)
			{//Dead entity
				ptrLoc->List_movementDB->List_curTransX = 0.0f;
				ptrLoc->List_movementDB->List_curTransY = 0.0f;
				ptrLoc->List_movementDB->List_curTransZ = 0.0f;
				m_aiManager->AI_Transform(ptrLoc,waterZoneDB,m_gStatus->m_timeAdjustFloat,m_gStatus->m_lastTimeAdjust);
			}//End Dead Entity
		}//end if has a controller attached
	}//end movement entity loop
}//END
//--------------Collision between entities--------------------------------------------//
void CEntityManagement::CollisionLoopEntityToEntity(CMovementObjList * entObjRuntime)
{
   if(m_gStatus->m_multiplayerObject->m_AIServer)
	  return;

   m_gStatus->gl_helperString = "";

       for(POSITION posLoc = entObjRuntime->GetHeadPosition();posLoc != NULL;)
	   {//main entity loop
           CMovementObj * entityFocus = ( CMovementObj* ) entObjRuntime->GetNext(posLoc);
		   
		   entityFocus->List_movementDB->m_lastGravityStatus = entityFocus->List_movementDB->List_doGravity;
	       entityFocus->List_movementDB->List_doGravity = TRUE;
		   
		   
		   if(entityFocus->List_networkEntity)
			  continue;

		   if(entityFocus->List_controlType == 35)//avoid focusing pickup items
			  continue;


		   if(!entityFocus->List_skeletalObject->m_sphereBodyCollisionSystem)
			   continue;

		   if(entityFocus->m_posses)
			   continue;

           //only check if you move
		   D3DVECTOR currentPosition;
		   D3DMATRIX focusMatrix;
		   entityFocus->List_baseFrame->GetPosition(NULL,&currentPosition);
		   entityFocus->List_baseFrame->GetTransform(NULL,&focusMatrix);
		      POSITION lastCmpPos;
			  
		      if(entityFocus->List_skeletalObject->m_calulationSphere)
		      for(POSITION cmpPosition = entObjRuntime->GetHeadPosition();(lastCmpPos = cmpPosition) != NULL;)
			  {//secondary compare loop
               CMovementObj * entityCmpare = ( CMovementObj* ) entObjRuntime->GetNext(cmpPosition);
               if(entityCmpare->m_posses)
			      continue;
			   
			   if(entityFocus->List_traceNumber == entityCmpare->List_traceNumber)
				  continue;

			   
			   
			   D3DMATRIX positionMatrix;
			   entityCmpare->List_baseFrame->GetTransform(NULL,&positionMatrix);
                
			    if(entityCmpare->List_skeletalObject->m_calulationSphere)
				{//valid collision calc object
			     D3DVECTOR bonePosition = m_gStatus->m_matLib.UpdatePolygonVertex(positionMatrix.m,entityCmpare->List_skeletalObject->GetBonePositionByIndex(entityCmpare->List_skeletalObject->m_calulationSphere->m_boneIndex));
                 D3DVECTOR bonePositionFocus = m_gStatus->m_matLib.UpdatePolygonVertex(focusMatrix.m,entityFocus->List_skeletalObject->GetBonePositionByIndex(entityFocus->List_skeletalObject->m_calulationSphere->m_boneIndex));
				 if(m_gStatus->m_matLib.BoundingSphereCheck(entityFocus->List_skeletalObject->m_calulationSphere->m_sphereRadius,
					                                       entityCmpare->List_skeletalObject->m_calulationSphere->m_sphereRadius,
														   bonePositionFocus.x,
														   bonePositionFocus.y,
														   bonePositionFocus.z,
														   bonePosition.x,
														   bonePosition.y,
														   bonePosition.z))
				 {//in calculation radius
 
				

				  CollisionLoopEntityToEntityRTC(entityFocus,//CMovementObj * entityFocus, 
												 entityCmpare,//	   CMovementObj * entityCmpare,
												 &m_gStatus->m_matLib);
				  continue;


				  

			      if(entityFocus->List_controlType == 0)
			           if(entityCmpare->List_controlType == 35 || entityCmpare->List_networkEntity == TRUE && entityCmpare->List_skeletalObject->m_currentAnimType == 8)
					   {//check to see if auto exec or notify
                          if(entityCmpare->m_autoTriggerTemp == 1)
						  {
						     if(GetTickCount()-m_lastAutoQuerryStamp > 1000)
							 {
							    m_lastAutoQuerryStamp = GetTickCount();
								m_gStatus->SendChatString(m_gStatus->m_globalInteractCommand);
								m_gStatus->m_globalAutoTriggerLast = entityCmpare->List_traceNumber;
								
							 }
						  }
						  else if(entityCmpare->m_autoTriggerTemp == 2)
						  {
						        if(m_gStatus->m_globalAutoTriggerLast != entityCmpare->List_traceNumber)
								{
                                   m_gStatus->SendChatString(m_gStatus->m_globalInteractCommand);
								}
								else
								{ 
								   
                                   m_gStatus->gl_helperString = "Press the Interaction Key to Interact";
								   ControlDBObj * controller = m_gStatus->m_controlsList->GetByIndex(0);
								   if(controller)
								   if(controller->List_controlInfoList)
								   {
								       ControlInfoObj* controlObj = controller->List_controlInfoList->GetByActionName("Interaction");
									   if(controlObj)
									   {
											int iKey = controlObj->List_buttonDown;
											if(controlObj->List_controlType == 1) 
											   iKey += KEYBOARD_MAX_KEYS;
											m_gStatus->gl_helperString.Replace("Interaction",controller->List_controlInfoList->ReturnKeyStringByIndex(iKey));	  	  
									   }
								   }
								}
								m_gStatus->m_globalAutoTriggerLast = entityCmpare->List_traceNumber;
						  }
						  else
						  {
			                  m_gStatus->gl_helperString = "Press the Interaction Key to Interact";
							  ControlDBObj * controller = m_gStatus->m_controlsList->GetByIndex(0);
							   if(controller)
							   if(controller->List_controlInfoList)
							   {
							       ControlInfoObj* controlObj = controller->List_controlInfoList->GetByActionName("Interaction");
								   if(controlObj)
								   {
										int iKey = controlObj->List_buttonDown;
										if(controlObj->List_controlType == 1) 
										   iKey += KEYBOARD_MAX_KEYS;
										m_gStatus->gl_helperString.Replace("Interaction",controller->List_controlInfoList->ReturnKeyStringByIndex(iKey));	  	  
								   }
							   }
                              m_gStatus->m_tempMenuFlag = 950;
							  m_gStatus->m_globalAutoTriggerLast = entityCmpare->List_traceNumber;
						  }
					   }
				  
				  D3DVECTOR cmpPosition;
				  entityCmpare->List_baseFrame->GetPosition(NULL,&cmpPosition);
				  if(entityCmpare->List_skeletalObject->m_sphereBodyCollisionSystem)//has collision sys on object
				  for(POSITION csysPos = entityFocus->List_skeletalObject->m_sphereBodyCollisionSystem->GetHeadPosition();csysPos != NULL;)
				  {//g system loop
				   CSphereCollisionObj * mainBdSphPtr = (CSphereCollisionObj *)entityFocus->List_skeletalObject->m_sphereBodyCollisionSystem->GetNext(csysPos);
				  
				  for(POSITION csysPos = entityCmpare->List_skeletalObject->m_sphereBodyCollisionSystem->GetHeadPosition();csysPos != NULL;)
				  {//c system loop
				   CSphereCollisionObj * bdSphPtr = (CSphereCollisionObj *)entityCmpare->List_skeletalObject->m_sphereBodyCollisionSystem->GetNext(csysPos);
				   D3DVECTOR adjust;
				   D3DVECTOR bonePositionCmp = m_gStatus->m_matLib.UpdatePolygonVertex(positionMatrix.m,entityCmpare->List_skeletalObject->GetBonePositionByIndex(bdSphPtr->m_boneIndex));
				   
				   if(m_gStatus->m_matLib.CollisionSphereSeg(currentPosition,
					                                        entityFocus->List_lastPosition,
					                                        bdSphPtr->m_sphereRadius,
															bonePositionCmp,
														    TRUE,
															&adjust,
															mainBdSphPtr->m_sphereRadius,
															NULL)==TRUE)
				   {//hit 
                     //straight shphere reactional (works)
                     D3DMATRIX matrix;
					 /*D3DVECTOR vector;
                     vector.x = currentPosition.x - entityFocus->List_lastPosition.x;
                     vector.y = currentPosition.y - entityFocus->List_lastPosition.y;
                     vector.z = currentPosition.z - entityFocus->List_lastPosition.z;*/
					 


                     m_gStatus->m_matLib.Translate((adjust.x*.5f),
						                          (adjust.y*.5f),
												  (adjust.z*.5f),matrix.m);
					 entityFocus->List_baseFrame->AddTransformAfter(matrix);
					 
					 

				      break;
				   }//end hit            
				  }//end c system loop
				  }//end g system loop
				 }//in calculation radius
				}//end valid collision calc object
			  }//end secondary compare loop
	   }//end main entity loop	   
}
//--------------Collision between entities--------------------------------------------//
BOOL CEntityManagement::MoveEntityBasedOnLock(CMovementObj * entityFocus,
											  CMovementObjList * entObjRuntime,
											  MatrixARB * mathLib)
{
	 if(entityFocus->List_movementDB->m_useLockPoint == 0 || entityFocus->List_movementDB->m_useLockPoint > 1)
	 {
	    entityFocus->List_movementDB->m_useLockPoint = 0;
	    return FALSE;
	 }

	 D3DMATRIX currentBoneMat,curMat;
	 D3DVECTOR newPos;


	 if(entityFocus->List_movementDB->m_optLockTrace > -1)
	 {//entity runtime based
		 CMovementObj * entPtrTrc = entObjRuntime->GetObjByTrace(entityFocus->List_movementDB->m_optLockTrace);
		 if(!entPtrTrc)
		 {
			 entityFocus->List_movementDB->m_useLockPoint = 0;
			 return FALSE;
		 }
		 if(entPtrTrc->List_networkEntity)
			CollisionPipelinePerEntityWld(mathLib, entPtrTrc, entPtrTrc->m_collisionMovRefPtr,true);
		 else
			CollisionPipelinePerEntityWld(mathLib, entPtrTrc, entPtrTrc->m_collisionMovRefPtr,false);
		 entPtrTrc->List_baseFrame->GetTransform(NULL,&curMat);	 
	 }//end entity runtime based
	 else
	 {//reference based
	     CWldObject * wldObj = m_gStatus->m_skeletalReferenceObjCache.GetByUnid(abs(entityFocus->List_movementDB->m_optLockTrace));
		 if(!wldObj)
		 {
             entityFocus->List_movementDB->m_useLockPoint = 0;
			 return FALSE;
		 }
	     curMat = wldObj->m_node->m_position;
	 }//end reference based

	 if(!entityFocus->List_movementDB->m_optFramePtr)
	     return FALSE;
	 
	 entityFocus->List_movementDB->m_optFramePtr->GetTransform(NULL,&currentBoneMat);	
	 mathLib->fastMatrixMultiply(currentBoneMat,currentBoneMat,curMat);
	 entityFocus->List_baseFrame->GetPosition(NULL,&newPos);
     D3DMATRIX inverseMatrix,differenceMatrix;
	 mathLib->D3DMath_MatrixInvert(inverseMatrix,entityFocus->List_movementDB->m_lastLock);
     mathLib->fastMatrixMultiply(differenceMatrix,inverseMatrix,currentBoneMat);
	 mathLib->fastMatrixMultiply(entityFocus->List_baseFrame->List_frameMatrix,entityFocus->List_baseFrame->List_frameMatrix,differenceMatrix);
	 
	 entityFocus->List_movementDB->m_lastLock = currentBoneMat;
	 entityFocus->List_baseFrame->List_frameMatrix._42 -= .02f;
	 
	 //L MOD
     entityFocus->List_movementDB->List_lastPosition = mathLib->UpdatePolygonVertex(differenceMatrix.m,entityFocus->List_movementDB->List_lastPosition);
	 entityFocus->List_movementDB->List_lastPosition.y -= .02f;

	 entityFocus->m_collisionRaySys->m_lastCenterMassPosition = mathLib->UpdatePolygonVertex(differenceMatrix.m,entityFocus->m_collisionRaySys->m_lastCenterMassPosition);
	 entityFocus->m_collisionRaySys->m_lastCenterMassPosition.y -= .02f;
   


	 //orientate up-right
     //entityFocus->List_baseFrame->List_frameMatrix
	 if(entityFocus->List_movementDB->List_maxPitch == 0.0f)
	 {
		//D3DMATRIX origMatrix = entityFocus->List_baseFrame->List_frameMatrix;
		//mathLib->D3DMath_MatrixInvert(inverseMatrix,origMatrix);
		
		D3DVECTOR upVect,dirVect;
		mathLib->GetOrientation(entityFocus->List_baseFrame->List_frameMatrix,&dirVect,&upVect);
		upVect.x = 0.0f;
		upVect.y = 1.0f;
		upVect.z = 0.0f;
		dirVect.y = 0.0f;
		dirVect = mathLib->Normalize(dirVect);
		mathLib->SetOrientation(&entityFocus->List_baseFrame->List_frameMatrix,dirVect,upVect);
		//mathLib->fastMatrixMultiply(differenceMatrix,inverseMatrix,entityFocus->List_baseFrame->List_frameMatrix);
		//entityFocus->List_movementDB->List_lastPosition = mathLib->UpdatePolygonVertex(differenceMatrix.m,entityFocus->List_movementDB->List_lastPosition);
        //entityFocus->m_collisionRaySys->m_lastCenterMassPosition = mathLib->UpdatePolygonVertex(differenceMatrix.m,entityFocus->m_collisionRaySys->m_lastCenterMassPosition);
	    //entityFocus->List_baseFrame->GetPosition(NULL,&entityFocus->List_movementDB->List_lastPosition);
		/*entityFocus->List_lastPosition = mathLib->UpdatePolygonVertex(entityFocus->List_baseFrame->List_frameMatrix.m,localLastPos);

		m_gStatus->m_systemReadout->AddLine("Local:",localLastPos);
	    m_gStatus->m_systemReadout->AddLine("After:",entityFocus->List_movementDB->List_lastPosition);
		/*mathLib->D3DMath_MatrixInvert(inverseMatrix,entityFocus->List_baseFrame->List_frameMatrix);*/
		//entityFocus->List_lastPosition = mathLib->UpdatePolygonVertex(inverseMatrix.m,localLastPos);
		//entityFocus->m_collisionRaySys->m_lastCenterMassPosition = mathLib->UpdatePolygonVertex(inverseMatrix.m,localCenterMass);*/
	 
	 }
	 else
	 {
        //mathLib->D3DMath_MatrixInvert(inverseMatrix,entityFocus->List_baseFrame->List_frameMatrix);
		//entityFocus->List_lastPosition = mathLib->UpdatePolygonVertex(inverseMatrix.m,localLastPos);
		//entityFocus->m_collisionRaySys->m_lastCenterMassPosition = mathLib->UpdatePolygonVertex(inverseMatrix.m,localCenterMass);
	 }
     //end orientate

	 entityFocus->List_movementDB->m_useLockPoint++;
     return TRUE;
}
//--------------Collision between entities--------------------------------------------//
BOOL CEntityManagement::CollisionLoopEntityToEntityRTC(CMovementObj * entityFocus, 
													   CMovementObj * entityCmpare,
													   MatrixARB * mathLib)
{
     

	 if(!entityFocus->List_skeletalObject)
	     return FALSE;
	 //set up rays
	 D3DMATRIX entityMatrix,invBoneMatrix,compareBaseMatrix,wldBoneMatrix;
	 entityFocus->List_baseFrame->GetTransform(NULL,&entityMatrix);
	 entityCmpare->List_baseFrame->GetTransform(NULL,&compareBaseMatrix);

     D3DVECTOR legRayStart,legRayEnd,invlegRayStart,invlegRayEnd; 
	 ZeroMemory(&legRayStart,sizeof(D3DVECTOR));
	 ZeroMemory(&legRayEnd,sizeof(D3DVECTOR));
	 legRayStart.y = entityFocus->List_skeletalObject->m_box.maxY;
	 legRayStart = mathLib->UpdatePolygonVertex(entityMatrix.m,legRayStart);
     legRayEnd   = mathLib->UpdatePolygonVertex(entityMatrix.m,legRayEnd);
     //Break bones down and look at each hiarchy mesh
	    D3DVECTOR polygon[3];
		D3DVECTOR intersect,finalIntersect;
		BOOL doAdjust = FALSE;
		float bestDist = 1000000;
		float tempDist = 0;
		D3DMATRIX finalReorientateMat;
	 if(entityCmpare->List_skeletalObject)
        if(entityCmpare->List_skeletalObject->m_bonesList)
           for(POSITION posLoc = entityCmpare->List_skeletalObject->m_bonesList->GetHeadPosition();posLoc != NULL;)
		   {
               CBoneObject * bonePtr = (CBoneObject *) entityCmpare->List_skeletalObject->m_bonesList->GetNext(posLoc);
	           if(bonePtr->m_hiarchyMesh)
			   {
                   CHiarcleVisObj * hObj = bonePtr->m_hiarchyMesh->GetByIndex(0);
                   if(hObj)
				   if(hObj->m_mesh)
				   { 
				      //D3DVECTOR testVect;
				      bonePtr->m_boneFrame->GetTransform(NULL,&wldBoneMatrix);//put ray into corrective space
				      mathLib->fastMatrixMultiply(wldBoneMatrix,wldBoneMatrix,compareBaseMatrix);
					  
					  mathLib->D3DMath_MatrixInvert2(invBoneMatrix,wldBoneMatrix);
					  
					  invlegRayStart = mathLib->UpdatePolygonVertex(invBoneMatrix.m,legRayStart);
                      invlegRayEnd   = mathLib->UpdatePolygonVertex(invBoneMatrix.m,legRayEnd);

					  //testVect.x = invBoneMatrix._41;
                      //testVect.y = invBoneMatrix._42;
					  //testVect.z = invBoneMatrix._43;
                      //m_gStatus->m_systemReadout->AddLine(testVect,m_gStatus->m_3dDevice,m_gStatus->m_symbolMapDB,m_gStatus->m_speechMode);
					  //m_gStatus->m_systemReadout->AddLine(invlegRayStart,m_gStatus->m_3dDevice,m_gStatus->m_symbolMapDB,m_gStatus->m_speechMode);
					  //m_gStatus->m_systemReadout->AddLine(invlegRayEnd,m_gStatus->m_3dDevice,m_gStatus->m_symbolMapDB,m_gStatus->m_speechMode);
					  
					  ZeroMemory(polygon,sizeof(D3DVECTOR)*3);
			          for(int iLoop=0;iLoop < hObj->m_mesh->m_abVertexArray.m_indecieCount;iLoop+=3)
					  {
                          hObj->m_mesh->m_abVertexArray.GetPolygon(iLoop,polygon);
						  //cull here 
					      //float angDiff = mathLib->GetAngleBetweenTwoVectors(D3DVECTOR vOne,D3DVECTOR vTwo);
                          
                          if(mathLib->SegPolyCheck(polygon,invlegRayStart,invlegRayEnd,&intersect))
						  {//collided
                             tempDist = mathLib->MagnitudeSquared(invlegRayStart.x,invlegRayStart.y,invlegRayStart.z,intersect.x,intersect.y,intersect.z);
                             if(tempDist < bestDist)
							 {
                                CopyMemory(&finalIntersect,&intersect,sizeof(D3DVECTOR));
                                bestDist = tempDist;
								doAdjust = TRUE;
								finalReorientateMat = wldBoneMatrix;

								entityFocus->List_movementDB->m_useLockPoint	= 1;//0=currently unlocked   1=currently locked 
								entityFocus->List_movementDB->m_lastLock		= wldBoneMatrix;//
				                entityFocus->List_movementDB->m_optFramePtr		= bonePtr->m_boneFrame;
				                entityFocus->List_movementDB->m_optLockTrace	= entityCmpare->List_traceNumber;
							 }
							 //m_gStatus->m_systemReadout->AddLine("Collided:"+bonePtr->m_boneName,m_gStatus->m_3dDevice,m_gStatus->m_symbolMapDB,m_gStatus->m_speechMode);
						     //D3DVECTOR curPos;
							 //bonePtr->m_boneFrame->GetPosition(NULL,&curPos);
							 //m_gStatus->m_systemReadout->AddLine(curPos,m_gStatus->m_3dDevice,m_gStatus->m_symbolMapDB,m_gStatus->m_speechMode);
						  
						  }//end collided
					  }
					  
				   }
			   }
		   }

		   //apply movement
			if(doAdjust)
			{//apply locking and offsets
				D3DVECTOR tPos,offset;
                entityFocus->List_baseFrame->GetPosition(NULL,&tPos);

                offset.x = finalIntersect.x - invlegRayEnd.x;
				offset.y = finalIntersect.y - invlegRayEnd.y;
				offset.z = finalIntersect.z - invlegRayEnd.z;

				
				offset = mathLib->UpdatePolygonVertexRotationOnly(finalReorientateMat.m,offset);

                //tPos.x += finalIntersect.x - invlegRayEnd.x;
				tPos.y += offset.y;//finalIntersect.y - invlegRayEnd.y;
				//tPos.z += finalIntersect.z - invlegRayEnd.z;
				entityFocus->List_movementDB->List_doGravity = FALSE;

				entityFocus->List_baseFrame->SetPosition(NULL,tPos);
				CopyMemory(&entityFocus->List_movementDB->List_lastPosition,&tPos,sizeof(D3DVECTOR));
				
				
				
				
				//m_gStatus->m_systemReadout->AddLine(offset,m_gStatus->m_3dDevice,m_gStatus->m_symbolMapDB,m_gStatus->m_speechMode);
                //m_gStatus->m_systemReadout->AddLine(finalIntersect,m_gStatus->m_3dDevice,m_gStatus->m_symbolMapDB,m_gStatus->m_speechMode);
			}//end apply locking and offsets
	 return TRUE;
}
//--------------checks Collision between a world object and a entity------------------//
void CEntityManagement::CollisionLoopEntityWorld(CMovementObjList * entObjRuntime)
{
  for(POSITION posLoc = entObjRuntime->GetHeadPosition();posLoc != NULL;)
  {//Begin entity loop
    CMovementObj * entLoc = ( CMovementObj* ) entObjRuntime->GetNext(posLoc);
	entLoc->List_AI_currentlyHittingWall = FALSE;
	
	if(entLoc->m_posses)
	continue;
	
	if(entLoc->List_networkEntity)
    continue;

	if(entLoc->m_filterCollision)
    continue;

	if(entLoc->List_controlType != 15 && entLoc->List_controlType != 35){//if valid
	 //float newYLoc = -7777.7771f;
     BOOL doCmpTemp = FALSE;
	 if(entLoc->List_movementDB->List_currentlyJumping)
	    doCmpTemp = TRUE;
	 //entLoc->List_movementDB->m_lastGravityStatus = entLoc->List_movementDB->List_doGravity;
	 //entLoc->List_movementDB->List_doGravity = TRUE;
	 entLoc->List_movementDB->m_currentSurfaceProp = 0;

	 HandleMovCollisionToSystem(entLoc,entLoc->m_collisionMovRefPtr,NULL,NULL);

	 if(doCmpTemp)
	 if(entLoc->List_movementDB->m_lastGravityStatus != entLoc->List_movementDB->List_doGravity)
	    entLoc->List_movementDB->m_grvtyChngTimeStmp = GetTickCount();
     
  }//end if valid
 }//end entity loop
}
//--------------checks Collision between a world object and a entity------------------//
void CEntityManagement::RealtimeEntityCollisionCallback(CMovementObjList * entObjRuntime)
{
  for(POSITION posLoc = entObjRuntime->GetHeadPosition();posLoc != NULL;)
  {//Begin entity loop
    CMovementObj * entLoc = ( CMovementObj* ) entObjRuntime->GetNext(posLoc);
	
	if(entLoc->m_posses)
	continue;
	
	if(entLoc->List_networkEntity)
    continue;

	if(entLoc->m_filterCollision)
    continue;

	if(entLoc->List_controlType != 15 && entLoc->List_controlType != 35){//if valid
   
	 if(!m_gStatus->m_multiplayerObject->m_AIServer)
	 {//use cache for client
	   for(POSITION hPos = m_gStatus->m_houseingCache->GetHeadPosition();hPos != NULL;)
	   {//Houseing
          CHousingCacheObj * hsPtr = (CHousingCacheObj *)m_gStatus->m_houseingCache->GetNext(hPos);
	      CHousingObj * houseLib = m_gStatus->m_housingDB->GetByIndex(hsPtr->m_dbIndex);
          if(houseLib)
		  {
		     if(hsPtr->m_open)
	            HandleMovCollisionToSystem(entLoc,houseLib->m_openState,&hsPtr->m_location,&hsPtr->m_invLocation);
			 else
				HandleMovCollisionToSystem(entLoc,houseLib->m_closedState,&hsPtr->m_location,&hsPtr->m_invLocation);
		  }
	   }//end Houseing
	 }//end use cache for client
	 else{//tabled mode for ai
	   if(m_gStatus->m_multiplayerObject->m_houseingDB)
	   {//house table
          D3DVECTOR wldPosition;
          entLoc->List_baseFrame->GetPosition(NULL,&wldPosition);
          CHPlacementObjList * placementList = m_gStatus->m_multiplayerObject->m_houseingDB->GetContainer(wldPosition);
          if(placementList)
          for(POSITION hPos = placementList->GetHeadPosition();hPos != NULL;)
		  {//Houseing
             CHPlacementObj * hsPtr = (CHPlacementObj *)placementList->GetNext(hPos);
	         CHousingObj * houseLib = m_gStatus->m_housingDB->GetByIndex(hsPtr->m_type);
             if(houseLib)
			 {
		        D3DMATRIX regMatrix,inverseMatrix;
                m_gStatus->m_matLib.InitMat(regMatrix.m);
		        m_gStatus->m_matLib.InitMat(inverseMatrix.m);

		        inverseMatrix._41 = -hsPtr->m_wldPosition.x;
		        inverseMatrix._42 = -hsPtr->m_wldPosition.y;
		        inverseMatrix._43 = -hsPtr->m_wldPosition.z;
		        regMatrix._41 = hsPtr->m_wldPosition.x;
		        regMatrix._42 = hsPtr->m_wldPosition.y;
		        regMatrix._43 = hsPtr->m_wldPosition.z;
		    
				
	            //HandleMovCollisionToSystem(entLoc,houseLib->m_openState,&regMatrix,&inverseMatrix);

				if(hsPtr->m_open)
	            HandleMovCollisionToSystem(entLoc,houseLib->m_openState,&regMatrix,&inverseMatrix);
			    else
				HandleMovCollisionToSystem(entLoc,houseLib->m_closedState,&regMatrix,&inverseMatrix);
			 }
		  }//end Houseing
	   }//end house table
	 }//end tabled mode for ai
	}//end valid
  }//end entity loop
}
//--------------Function basis turn Right---------------------------------------------//
void CEntityManagement::PossesCallback(CMovementObjList * entObjRuntime)
{
  //return;
  for(POSITION entPosition = entObjRuntime->GetHeadPosition();entPosition != NULL;)
  {
      CMovementObj * entityPtr = ( CMovementObj* ) entObjRuntime->GetNext(entPosition);
	  m_gStatus->UpdateRiderOfPossedEntity(entityPtr);
	
      /*if(entityPtr->m_posses){//possesing somthing update movememnt
        //D3DMATRIX origMatrix = entityPtr->List_skeletalObject->GetBoneMatrixByIndex(0);
        CFrameObj * frameRiderObj = entityPtr->List_skeletalObject->GetBoneFrameByIndex(0);
        if(frameRiderObj)
		{
		 CFrameObj * frameBaseObj = entityPtr->m_posses->List_skeletalObject->GetBoneFrameByIndex(entityPtr->m_posses->m_ridable);
     	 if(frameBaseObj)
		 {
          D3DVECTOR upVector,dirVect,baseBonePos;
          frameRiderObj->GetPosition(NULL,&baseBonePos);
		  entityPtr->m_posses->List_baseFrame->GetOrientation(&dirVect,&upVector);

		  D3DMATRIX matrixAp = entityPtr->m_posses->List_skeletalObject->GetBoneMatrixByIndex(entityPtr->m_posses->m_ridable);
	      D3DMATRIX focusPtrMatrix;
	      entityPtr->m_posses->List_baseFrame->GetTransform(NULL,&focusPtrMatrix);
	      m_gStatus->m_matLib.fastMatrixMultiply(matrixAp,matrixAp,focusPtrMatrix);
	     
          entityPtr->List_baseFrame->SetPosition2(NULL,matrixAp._41,matrixAp._42,matrixAp._43);
          entityPtr->List_baseFrame->SetOrientation(dirVect,upVector);


		  D3DMATRIX nowMatrix;
		  entityPtr->List_baseFrame->GetTransform(NULL,&nowMatrix);
          D3DMATRIX locOffset;

	      m_gStatus->m_matLib.Translate (0.0f,-((baseBonePos.y/3.0f)*2.0f),0.0f,locOffset.m);
	      m_gStatus->m_matLib.fastMatrixMultiply(nowMatrix,locOffset,nowMatrix);
          entityPtr->List_baseFrame->AddTransformReplace(nowMatrix);
	      
		 }
		}
	  }//end possesing somthing update movememnt*/
  }
}


//------------------------------------------------------------------------------------//
void CEntityManagement::HandleMovCollisionToSystem( CMovementObj * entLoc,
													CCollisionObj * collisionSystem,
													D3DMATRIX * transformOfCollisionSys,
													D3DMATRIX * invTransformOfCollisionSys)
{
     float newYLoc = -7777.7771f;
     BOOL continueRepeat = TRUE;
     BOOL everColided = FALSE;	
     D3DVECTOR origin;
     
     //dont check collision if it hasent moved
	 if(memcmp(&entLoc->List_lastPosition,&origin,sizeof(D3DVECTOR))==0)
		return;

	 if(invTransformOfCollisionSys)
	 {
         entLoc->List_baseFrame->AddTransformAfter(*invTransformOfCollisionSys);
         entLoc->List_lastPosition.x += invTransformOfCollisionSys->_41;
		 entLoc->List_lastPosition.y += invTransformOfCollisionSys->_42;
		 entLoc->List_lastPosition.z += invTransformOfCollisionSys->_43;
		 //entLoc->List_movementDB->List_lastPosition.x += invTransformOfCollisionSys->_41;
		 //entLoc->List_movementDB->List_lastPosition.y += invTransformOfCollisionSys->_42;
		 //entLoc->List_movementDB->List_lastPosition.z += invTransformOfCollisionSys->_43;
	 }
	 entLoc->List_baseFrame->GetPosition(NULL,&origin);
     

	 D3DMATRIX entityMatrix;
	 entLoc->List_baseFrame->GetTransform(NULL,&entityMatrix);

	 int polysExactChecked=0;
     int polysConsidered=0;
     D3DVECTOR contactLocation; 
	 BOOL walkingType = TRUE;
	 float characterHieght = entLoc->List_skeletalObject->m_box.maxY;
	 float characterWidth = entLoc->List_skeletalObject->m_box.maxX;
	   if(entLoc->List_skeletalObject->m_box.maxY > entLoc->List_skeletalObject->m_box.maxX)
	      characterWidth = entLoc->List_skeletalObject->m_box.maxY;
		   
		   
     float filler=0;
	 D3DVECTOR latestNormal;
	 int howManyNormals = 0;
	 D3DVECTOR directionVector,rightShoulder,leftShoulder;
	 directionVector.x = 0.0f;
	 directionVector.y = 0.0f;
	 directionVector.z = -1.0f;
	 directionVector = m_gStatus->m_matLib.UpdatePolygonVertex(entityMatrix.m,directionVector);
	 if(entLoc->List_controlType == 0)
	 {//controler only pls
        rightShoulder.x = -(characterWidth/4);
	    rightShoulder.y = characterHieght*.5f;
	    rightShoulder.z = 0.0f;
	    rightShoulder = m_gStatus->m_matLib.UpdatePolygonVertex(entityMatrix.m,rightShoulder);
        leftShoulder.x = (characterWidth/4);
	    leftShoulder.y = characterHieght*.5f;
	    leftShoulder.z = 0.0f;
	    leftShoulder = m_gStatus->m_matLib.UpdatePolygonVertex(entityMatrix.m,leftShoulder);
	 }//end controler only pls
     //collision checks
		  float highestContactLocation = newYLoc;
		  BOOL recordHitData = TRUE;
		  if(walkingType){//walking /legs
		  
          //---------------------//
		  //directional collision//
          //---------------------//
		  D3DVECTOR adjustVector;
		  adjustVector.x = 0.0f;
		  adjustVector.y = 0.0f;
		  adjustVector.z = 0.0f;

		  float charHieghtMod = characterHieght*.5f;
		  if(entLoc->List_movementDB->m_antiGravity)
			 charHieghtMod = 0.0f;

          D3DVECTOR travelStartVector,travelEndVector,tVector;


		  BOOL slide = FALSE;
		  D3DVECTOR slideVect;
		  slideVect.x = 0.0f;
		  slideVect.y = 0.0f;
		  slideVect.z = 0.0f;
		  D3DVECTOR lastNormal;
		  
		  if(entLoc->List_controlType == 0)
		  {//HEAD COLLISION-only exec if user
		  D3DVECTOR headPoint,midsectionPoint;
		  midsectionPoint.x = origin.x;
		  midsectionPoint.y = origin.y + charHieghtMod;
		  midsectionPoint.z = origin.z;
		  headPoint.x = origin.x;
		  headPoint.y = origin.y+characterHieght;
		  headPoint.z = origin.z;
		  int objectIndexHit = 0;
		  if(charHieghtMod != 0.0f)
		   if(entLoc->m_collisionMovRefPtr->ObjectCollisionAdjustSegment(
			                                          midsectionPoint,
			                                          headPoint,
			                                          midsectionPoint,
                                                      headPoint,
			                                         &contactLocation.x,
												     &contactLocation.y,
												     &contactLocation.z,
												     &polysExactChecked,
												     &polysConsidered,
                                                     &highestContactLocation,
													 &latestNormal,
													 FALSE,
													 &adjustVector,
													 -1.0f,
													 0.0f,
													 &objectIndexHit
												     )==TRUE)
		   {//Collision valid
		         float diffY = headPoint.y - contactLocation.y;

				 D3DMATRIX initedOne;
				 entLoc->List_baseFrame->GetTransform(NULL,&initedOne);
                 initedOne._42 -= diffY;
                 entLoc->List_baseFrame->AddTransformReplace(initedOne);
			     entLoc->List_baseFrame->GetPosition(NULL,&origin);

		   }//end Collision valid
		   //shoulder
         
           if(collisionSystem->ObjectCollisionAdjustSegment(
			                                          midsectionPoint,
			                                          rightShoulder,
			                                          midsectionPoint,
                                                      rightShoulder,
			                                         &contactLocation.x,
												     &contactLocation.y,
												     &contactLocation.z,
												     &polysExactChecked,
												     &polysConsidered,
                                                     &highestContactLocation,
													 &latestNormal,
													 FALSE,
													 &adjustVector,
													 -.5f,
													 .5f,
													 &objectIndexHit
												     )==TRUE)
		   {//Collision valid
		         D3DMATRIX applicate;
			     m_gStatus->m_matLib.Translate(adjustVector.x,adjustVector.y,adjustVector.z,applicate.m);
                 entLoc->List_baseFrame->AddTransformAfter(applicate);
			     entLoc->List_baseFrame->GetPosition(NULL,&origin);
				 entLoc->List_movementDB->List_lastPosition.x += adjustVector.x;
			     entLoc->List_movementDB->List_lastPosition.y += adjustVector.y;
			     entLoc->List_movementDB->List_lastPosition.z += adjustVector.z;

		   }//end Collision valid
		   
           if(collisionSystem->ObjectCollisionAdjustSegment(
			                                          midsectionPoint,
			                                          leftShoulder,
			                                          midsectionPoint,
                                                      leftShoulder,
			                                         &contactLocation.x,
												     &contactLocation.y,
												     &contactLocation.z,
												     &polysExactChecked,
												     &polysConsidered,
                                                     &highestContactLocation,
													 &latestNormal,
													 FALSE,
													 &adjustVector,
													 -.5f,
													  .5f,
													 &objectIndexHit
												     )==TRUE)
		   {//Collision valid
		         D3DMATRIX applicate;
			     m_gStatus->m_matLib.Translate(adjustVector.x,adjustVector.y,adjustVector.z,applicate.m);
                 entLoc->List_baseFrame->AddTransformAfter(applicate);
			     entLoc->List_baseFrame->GetPosition(NULL,&origin);
				 entLoc->List_movementDB->List_lastPosition.x += adjustVector.x;
			     entLoc->List_movementDB->List_lastPosition.y += adjustVector.y;
			     entLoc->List_movementDB->List_lastPosition.z += adjustVector.z;

		   }//end Collision valid


		  }//end HEAD COLLISION-only exec if user
		  //LEG COLLISION
		  D3DVECTOR startCollisionPath,useLastPosition;

          useLastPosition.x = entLoc->List_movementDB->List_lastPosition.x;
		  useLastPosition.y = entLoc->List_movementDB->List_lastPosition.y;
		  useLastPosition.z = entLoc->List_movementDB->List_lastPosition.z;

		  startCollisionPath.x = origin.x;
		  startCollisionPath.y = entLoc->List_lastPosition.y + charHieghtMod;
		  startCollisionPath.z = origin.z;

		  if(startCollisionPath.y < (origin.y + charHieghtMod))
			 startCollisionPath.y = (origin.y + charHieghtMod);
 
		  adjustVector.x = 0.0f;
		  adjustVector.y = 0.0f;
		  adjustVector.z = 0.0f;
          int objectIndexHit = -1;
		   if(charHieghtMod != 0.0f)
		   if(collisionSystem->ObjectCollisionAdjustSegment(
			                                          startCollisionPath,
			                                          origin,
			                                          startCollisionPath,
                                                      origin,
			                                         &contactLocation.x,
												     &contactLocation.y,
												     &contactLocation.z,
												     &polysExactChecked,
												     &polysConsidered,
                                                     &highestContactLocation,
													 &latestNormal,
													 FALSE,
													 &adjustVector,
													 .2f,
													 1.0f,
													 &objectIndexHit
												     )==TRUE)
		   {//Collision valid
			  
			  entLoc->m_recentSufaceNormal.x = latestNormal.x;
		      entLoc->m_recentSufaceNormal.y = latestNormal.y;
		      entLoc->m_recentSufaceNormal.z = latestNormal.z;
			     entLoc->List_movementDB->List_currentlyJumping = FALSE;
		         entLoc->List_movementDB->List_doGravity = FALSE;
			     entLoc->List_baseFrame->GetPosition(NULL,&origin);
				 float lastY = origin.y;

				 
				 if((highestContactLocation - origin.y) > .001f){//1
				    newYLoc = highestContactLocation - .001f;
					D3DMATRIX initedOne;
					 entLoc->List_baseFrame->GetTransform(NULL,&initedOne);
                     initedOne._42 = newYLoc;
                     entLoc->List_baseFrame->AddTransformReplace(initedOne);
                     float change = initedOne._42 - lastY;
                     
                     //m_gStatus->m_systemReadout->AddLine(change);
					 if(change > (entLoc->List_movementDB->m_rampTolerance))
                        entLoc->List_movementDB->List_lastPosition.y = entLoc->List_movementDB->List_lastPosition.y + change;
				 }//end 1
		   }//end Collision valid

          entLoc->List_baseFrame->GetPosition(NULL,&origin);

		  for(int insuranceLoop = 0;insuranceLoop < 2;insuranceLoop++)
		  {//insurance loop
			  //charHieghtMod = charHieghtMod / 5.0f;
          travelStartVector.x = useLastPosition.x;
          travelStartVector.y = useLastPosition.y + charHieghtMod;
          travelStartVector.z = useLastPosition.z;

          travelEndVector.x = origin.x;
          travelEndVector.y = origin.y + charHieghtMod;
		  travelEndVector.z = origin.z;

          //travelEndVector.y = travelStartVector.y;
		   
		  tVector.x = travelEndVector.x - travelStartVector.x;
		  tVector.y = travelEndVector.y - travelStartVector.y;
		  tVector.z = travelEndVector.z - travelStartVector.z;

		  tVector = m_gStatus->m_matLib.Normalize(tVector);
		  
		  float Ymod = characterWidth;
		  if(charHieghtMod != 0.0f)
		  if(characterWidth > charHieghtMod)
			 Ymod = charHieghtMod*.9f;

		  travelEndVector.x = travelEndVector.x + (tVector.x*Ymod);
		  travelEndVector.y = travelEndVector.y + (tVector.y*Ymod);
		  travelEndVector.z = travelEndVector.z + (tVector.z*Ymod);

          if(!entLoc->List_movementDB->m_antiGravity)
		  if(travelEndVector.y <= origin.y)
			 travelEndVector.y = origin.y + .1f;
  
		  float filler=0.0f;

		   objectIndexHit = -1;
           if(collisionSystem->ObjectCollisionAdjustSegment(travelStartVector,
			                                                 travelEndVector,
			                                                 travelStartVector,
                                                             travelEndVector,
			                                                 &contactLocation.x,
												             &contactLocation.y,
												             &contactLocation.z,
												             &polysExactChecked,
												             &polysConsidered,
                                                             &filler,
													         &latestNormal,
															 TRUE,
													         &adjustVector,
													         -1.0f,
													          1.0f,
															  &objectIndexHit
												             )==TRUE)
		   {//Collision valid
             D3DMATRIX applicate;
			 m_gStatus->m_matLib.Translate(adjustVector.x,adjustVector.y,adjustVector.z,applicate.m);
             entLoc->List_baseFrame->AddTransformAfter(applicate);
			 //D3DVECTOR simVect;
			    
             entLoc->List_baseFrame->GetPosition(NULL,&origin);//get result current position

			 //entLoc->List_baseFrame->GetPosition(NULL,&entLoc->List_movementDB->List_lastPosition);
			 if(objectIndexHit > -1)
			 if(!m_gStatus->m_multiplayerObject->m_AIServer)
			 {
			    /*CWldObject * wldObjTemp = collisionSystem->m_wldObjectReferenceArray[objectIndexHit].worldObjectReference;//crashes housing
				if(wldObjTemp->m_surfaceProperty == 1)
				{//ladder
				   entLoc->List_movementDB->m_currentSurfaceProp = wldObjTemp->m_surfaceProperty;
				   entLoc->List_movementDB->m_ladderLockedDir.x = latestNormal.x;
                   entLoc->List_movementDB->m_ladderLockedDir.y = latestNormal.y;
				   entLoc->List_movementDB->m_ladderLockedDir.z = latestNormal.z;

				   entLoc->List_movementDB->m_ladderBox.maxY = wldObjTemp->m_meshObject->List_boundingBox.maxY;
				   entLoc->List_movementDB->m_ladderBox.minY = wldObjTemp->m_meshObject->List_boundingBox.minY;

				   entLoc->List_movementDB->m_ladderBox.maxX = origin.x;
				   entLoc->List_movementDB->m_ladderBox.maxZ = origin.z;
				}*/

			 }

			 if(insuranceLoop == 1){
               if(memcmp(&lastNormal,&latestNormal,sizeof(D3DVECTOR))!=0)
                  entLoc->List_baseFrame->GetPosition(NULL,&entLoc->List_movementDB->List_lastPosition);//get result current position
			 }
			 else{
             lastNormal.x = latestNormal.x;
			 lastNormal.y = latestNormal.y;
			 lastNormal.z = latestNormal.z;
			 entLoc->List_movementDB->List_lastPosition.x = entLoc->List_movementDB->List_lastPosition.x + applicate._41;
			 entLoc->List_movementDB->List_lastPosition.y = entLoc->List_movementDB->List_lastPosition.y + applicate._42;
			 entLoc->List_movementDB->List_lastPosition.z = entLoc->List_movementDB->List_lastPosition.z + applicate._43;
			  slide = TRUE;
			 }

		   }//end collision valid
		   else if(insuranceLoop == 1){
		      //entLoc->List_baseFrame->GetPosition(NULL,&entLoc->List_movementDB->List_lastPosition);
		   }
		   else	   
		   break;

		  }//end insurance loop

       
		  }//end walking / legs
          //calc proper adjust to last position

		  
		  m_gStatus->m_actualPolysCalculated += polysExactChecked;
		  m_gStatus->m_collisionPolysInUse += polysConsidered;

		  if(transformOfCollisionSys)
		  {
             entLoc->List_baseFrame->AddTransformAfter(*transformOfCollisionSys);
             entLoc->List_lastPosition.x += transformOfCollisionSys->_41;
		     entLoc->List_lastPosition.y += transformOfCollisionSys->_42;
		     entLoc->List_lastPosition.z += transformOfCollisionSys->_43;
			 entLoc->List_movementDB->List_lastPosition.x += transformOfCollisionSys->_41;
		     entLoc->List_movementDB->List_lastPosition.y += transformOfCollisionSys->_42;
		     entLoc->List_movementDB->List_lastPosition.z += transformOfCollisionSys->_43;
		  }
}
//--------------------logs last position attribute------------------------------------//
void CEntityManagement::GenerateEntityLastPosition(CMovementObjList * entObjRuntime)
{
  for(POSITION posLoc = entObjRuntime->GetHeadPosition();posLoc != NULL;)
  {//Begin entity loop
    CMovementObj * entLoc = ( CMovementObj* ) entObjRuntime->GetNext(posLoc);
	entLoc->m_directionalVector.x = entLoc->List_baseFrame->List_frameMatrix._41 - entLoc->List_lastPosition.x;
	entLoc->m_directionalVector.y = entLoc->List_baseFrame->List_frameMatrix._42 - entLoc->List_lastPosition.y;
	entLoc->m_directionalVector.z = entLoc->List_baseFrame->List_frameMatrix._43 - entLoc->List_lastPosition.z;
	entLoc->List_baseFrame->GetPosition(NULL,&entLoc->List_lastPosition);

  }
}
//--------------Missile collision check-----------------------------------------------//
void CEntityManagement::EntityDamagePathCheck(CMovementObjList * entObjRuntime)//entity check
{ 
  long currentTickCountLoc = GetTickCount();
  for(POSITION posLoc = entObjRuntime->GetHeadPosition();posLoc != NULL;)
  {//loop through focus entity objects
   CMovementObj * focusEntPtr = ( CMovementObj* ) entObjRuntime->GetNext(posLoc);
   if(focusEntPtr->List_networkEntity)
	  continue;

   if(focusEntPtr->List_controlType == 35)
	  continue;

   if(!focusEntPtr->List_currentlyFireing)
      continue;

   if(focusEntPtr->List_armedInventoryDB)
   for(POSITION armPos = focusEntPtr->List_armedInventoryDB->GetHeadPosition();armPos != NULL;)
   {//armed inven list
	CArmedInventoryObj * armInven = (CArmedInventoryObj *)focusEntPtr->List_armedInventoryDB->GetNext(armPos);
	if(armInven->m_thirdPersonSystem)
	if(armInven->m_thirdPersonSystem->m_damageSphereList){//valid damage node
    D3DMATRIX focusPtrMatrix;
	focusEntPtr->List_baseFrame->GetTransform(NULL,&focusPtrMatrix);
	D3DMATRIX armedSkMatrix = focusEntPtr->List_skeletalObject->GetBoneMatrixByIndex(armInven->m_boneAttached);
	m_gStatus->m_matLib.fastMatrixMultiply(armedSkMatrix,armedSkMatrix,focusPtrMatrix);	 
	 for(POSITION dPos = armInven->m_thirdPersonSystem->m_damageSphereList->GetHeadPosition();dPos != NULL;)
	 {//begin damage sphere loop
      CDamagePathObj* damagePathObj = (CDamagePathObj*)armInven->m_thirdPersonSystem->m_damageSphereList->GetNext(dPos);   
      
	  damagePathObj->m_damageDelay = (int)(((float)focusEntPtr->List_skeletalObject->m_scndDurationCurrentLength) * 1.3f);
	  if(damagePathObj->m_damageDelay <= 0)
		 damagePathObj->m_damageDelay = (int)(1000.0f * 1.3f);


	  D3DVECTOR dmgBonePosition = m_gStatus->m_matLib.UpdatePolygonVertex(armedSkMatrix.m,armInven->m_thirdPersonSystem->GetBonePositionByIndex(damagePathObj->m_boneIndex));
	  if((currentTickCountLoc - damagePathObj->m_damageDelayStamp) > damagePathObj->m_damageDelay)
	  {//log last position if changed

		  
		  if(damagePathObj->m_contactMade){
		     damagePathObj->m_lastPosition.x = dmgBonePosition.x;
	         damagePathObj->m_lastPosition.y = dmgBonePosition.y;
		     damagePathObj->m_lastPosition.z = dmgBonePosition.z;
		     damagePathObj->m_contactMade = FALSE;	
		     continue;
		  }
	  }//end log last position if changed

	  if(damagePathObj->m_contactMade == TRUE)
		continue;

	    for(POSITION posLoc2 = entObjRuntime->GetHeadPosition();posLoc2 != NULL;)
		{//Begin entity loop
	      CMovementObj * entPtr = ( CMovementObj* ) entObjRuntime->GetNext(posLoc2);
		  
		  if(entPtr->List_controlType == 35)
			 continue;

		  if(entPtr->List_energy < 1)
			 continue;

		  if(entPtr->m_possedBy)
			 continue;
     
		  if(m_gStatus->m_multiplayerObject->m_AIServer)
		  {
			  if(focusEntPtr->m_team == entPtr->m_team)
				 continue;

			  if(focusEntPtr->m_aiObj)
			  {//only damage target (protection)
				 if(focusEntPtr->List_AI_target != entPtr->List_traceNumber)
					continue;
			  }//only damage target (protection)
		  }

	      if(entPtr->List_traceNumber != focusEntPtr->List_traceNumber)
		  {//make sure not owner
			D3DMATRIX positionMatrix;
			entPtr->List_baseFrame->GetTransform(NULL,&positionMatrix);
            if(entPtr->List_skeletalObject->m_calulationSphere)
			{//valid collision calc object
			 D3DVECTOR bonePosition = m_gStatus->m_matLib.UpdatePolygonVertex(positionMatrix.m,entPtr->List_skeletalObject->GetBonePositionByIndex(entPtr->List_skeletalObject->m_calulationSphere->m_boneIndex));
			 
             //if(m_gStatus->m_matLib.CollisionSphereSeg(dmgBonePosition,damagePathObj->m_lastPosition,entPtr->List_skeletalObject->m_calulationSphere->m_sphereRadius,bonePosition,FALSE,NULL,damagePathObj->m_radius,NULL)==TRUE)
			 if(m_gStatus->m_matLib.BoundingSphereCheck(entPtr->List_skeletalObject->m_calulationSphere->m_sphereRadius,
				                                    damagePathObj->m_radius,
													dmgBonePosition.x,
													dmgBonePosition.y,
													dmgBonePosition.z,
                                                    positionMatrix._41,
													positionMatrix._42,
													positionMatrix._43))
			 {//in calculation radius
			  if(entPtr->List_skeletalObject->m_sphereLocationalDamageList)
			  {//valid locational damage list
		         for(POSITION dmgPos = entPtr->List_skeletalObject->m_sphereLocationalDamageList->GetHeadPosition();dmgPos != NULL;)
				 {//damage sphere loop
		             CSphereCollisionObj * dmgSph = (CSphereCollisionObj *)entPtr->List_skeletalObject->m_sphereLocationalDamageList->GetNext(dmgPos);
		             POSITION bnPos = entPtr->List_skeletalObject->m_bonesList->FindIndex(dmgSph->m_boneIndex);
                     if(bnPos)
					 {//valid
                        CBoneObject * bnPtr = (CBoneObject *)entPtr->List_skeletalObject->m_bonesList->GetAt(bnPos);
                        D3DVECTOR bnPosition;
						bnPtr->m_boneFrame->GetPosition(NULL,&bnPosition);
                        bnPosition = m_gStatus->m_matLib.UpdatePolygonVertex(positionMatrix.m,bnPosition);

                        
						if(m_gStatus->m_matLib.CollisionSphereSeg(dmgBonePosition,damagePathObj->m_lastPosition,
					                                  dmgSph->m_sphereRadius,bnPosition,
													  FALSE,NULL,damagePathObj->m_radius,NULL)==TRUE){//hit
							         //if(dmgSph->m_comment.GetLength() > 0)
							         //m_gStatus->m_systemReadout->AddLine(dmgSph->m_comment,m_gStatus->m_3dDevice,m_gStatus->m_symbolMapDB,m_gStatus->m_speechMode);
				                     //damage
                                     

									 if(!m_gStatus->m_multiplayerObject->GetEnabledClient_safe())
									 {
										 int damage = damagePathObj->m_damageMinValue + (rand() % (damagePathObj->m_damageMaxValue - damagePathObj->m_damageMinValue));
										 entPtr->List_energy -= (int)(damage * dmgSph->m_damageMultiplier);
									 }
									 else{//send network data
										 CMovementObj * entityPtrLoc = entObjRuntime->GetObjByTrace(focusEntPtr->List_traceNumber);
										 if(entityPtrLoc){
                                           entityPtrLoc->m_curHitCounter = 0;
										   if(entityPtrLoc->List_networkEntity == FALSE)
										   {//send is local
                                              if(m_gStatus->m_multiplayerObject->m_dmgOutBox->GetCount() < 5)
											  {

                                                 m_gStatus->m_multiplayerObject->m_localCLSecurityIndexer += (8);
	                                             if(m_gStatus->m_multiplayerObject->m_localCLSecurityIndexer > 30000)
	                                                m_gStatus->m_multiplayerObject->m_localCLSecurityIndexer = 0; 

												 if(entPtr->List_controlType == 5)//AI ONLY this will stop a misconfigured spam looping weapon
													entPtr->m_aiObj->m_attackingCurrently		= FALSE;

										            m_clientNet->SendDamageMsg((short)entPtr->List_networkDefinedID,
																				1,
																				(short)armInven->m_type,
																				(short)entityPtrLoc->List_networkDefinedID,
																				(short)m_gStatus->m_multiplayerObject->m_localCLSecurityIndexer,
																				(short)positionMatrix._41,
																				(short)positionMatrix._42,
																				(short)positionMatrix._43);
													return;//only one damage per turn
											  }
										   }
										   else
										   continue;
										 }

									 }//end send network data
                                     
									 //physics
									 D3DVECTOR forceVector;
									 forceVector.x = dmgBonePosition.x - damagePathObj->m_lastPosition.x;
									 forceVector.y = dmgBonePosition.y - damagePathObj->m_lastPosition.y;
									 forceVector.z = dmgBonePosition.z - damagePathObj->m_lastPosition.z;
									 forceVector = m_gStatus->m_matLib.Normalize(forceVector);
									 float pushPower = (((float)armInven->m_miscInt)*.001f);
							
									 entPtr->List_externalVectorTemp.x += forceVector.x * pushPower;
									 entPtr->List_externalVectorTemp.y += forceVector.y * pushPower;
									 entPtr->List_externalVectorTemp.z += forceVector.z * pushPower;
                                     damagePathObj->m_contactMade = TRUE;
									 damagePathObj->m_damageDelayStamp = currentTickCountLoc;
									 float factor = .03f;
									 m_gStatus->m_soundManager->PlayByID(999,FALSE,bnPosition,factor,400.0f);
									 break;
						}//end hit      
					 }//end valid
				 }//end damage sphere loop
			  }//end valid locational damage list
			 }//end in calculation radius
			}//end valid collision calc object
		  }//end make sure not owner
		}//end Begin entity loop
		damagePathObj->m_lastPosition.x = dmgBonePosition.x;
	    damagePathObj->m_lastPosition.y = dmgBonePosition.y;
		damagePathObj->m_lastPosition.z = dmgBonePosition.z;
	 }//end begin damage sphere loop
	}//end valid damage node
   }//armed inven list
  }//end loop through focus entity objects
}
//-----------removes dead entities----------------------------------------------------//
BOOL CEntityManagement::HandleDeadEntities(DWORD CurrentTime)
{
POSITION posLast;
int indexTrace = 0;
  for(POSITION posLoc = m_gStatus->m_entObjRuntime->GetHeadPosition();(posLast = posLoc) != NULL;){//runtime entity loop
	  CMovementObj * ptrLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetNext(posLoc);
      if(ptrLoc->List_controlType == 999 || ptrLoc->List_controlType == 997){
		  ptrLoc->FadeOut(3000);
	      ptrLoc->List_controlType = 1000;	 
	  }
	  else if(ptrLoc->List_controlType == 1000){
		  if((CurrentTime - ptrLoc->List_deathTimeVarial) > 3000)
		  {
			    m_gStatus->m_entObjRuntime->RemoveAt(posLast);
		        m_gStatus->DestroyRuntimeEntity(ptrLoc,m_gStatus->m_spawnGeneratorDB,m_gStatus->m_entObjRuntime);
	            ptrLoc = 0;
		  }
	  }
	  indexTrace++;
  }//end runtime entity loop
  return TRUE;
}

//--------------collision pipeline------------------//
void CEntityManagement::TableCollisionPipelinePerEntityCore(MatrixARB * matLib, 
														   CMovementObj * entLoc,
														   CCollisionObj * collisionObject,
														   D3DMATRIX * transformOfCollisionSys,
														   D3DMATRIX * invTransformOfCollisionSys,
														   bool correctiveOnly)
{
    if(invTransformOfCollisionSys)
	{
         entLoc->List_baseFrame->AddTransformAfter(*invTransformOfCollisionSys);
         entLoc->m_collisionRaySys->m_lastCenterMassPosition.x += invTransformOfCollisionSys->_41;
		 entLoc->m_collisionRaySys->m_lastCenterMassPosition.y += invTransformOfCollisionSys->_42;
		 entLoc->m_collisionRaySys->m_lastCenterMassPosition.z += invTransformOfCollisionSys->_43;
		 entLoc->List_movementDB->List_lastPosition.x += invTransformOfCollisionSys->_41;
		 entLoc->List_movementDB->List_lastPosition.y += invTransformOfCollisionSys->_42;
		 entLoc->List_movementDB->List_lastPosition.z += invTransformOfCollisionSys->_43;
		 entLoc->List_lastPosition.x += invTransformOfCollisionSys->_41;
		 entLoc->List_lastPosition.y += invTransformOfCollisionSys->_42;
		 entLoc->List_lastPosition.z += invTransformOfCollisionSys->_43;
    }

    //process speed vector
    D3DVECTOR intersection, latestNormal;
	int indexOfObject = 0;
	float dist = 0.0f;
    if(correctiveOnly==false)
	   CorrectiveUnbreakableArrayToStatic(  entLoc,
											intersection,
											collisionObject,
											indexOfObject,
											latestNormal,
											dist,
											matLib,
											true);
	
		
	D3DVECTOR offset,position,projPos,dirtemp;
	byte temp = 0;
	
    entLoc->m_collisionRaySys->UpdateRays(entLoc->List_skeletalObject);
	if(entLoc->m_collisionType == 4)
	{//VEHICLE SPECIFIC
	   //------------------------------------//
	   //--first 4 which are tire rays-------//
	   //------------------------------------//
	   int cloop=0;
       for(cloop=0;cloop<4;cloop++)
	   {//world collision
		entLoc->List_baseFrame->GetPosition(NULL,&position);
		
		if(entLoc->m_collisionRaySys->RecalculateByIndex(  cloop,
														   entLoc->List_baseFrame, 
														   entLoc->List_skeletalObject,
														   matLib,
														   &position,
														   &entLoc->List_movementDB->List_lastPosition) == false)
														   continue;
		if(collisionObject->GetClosesIntersectionUseSeg(
																		entLoc->m_collisionRaySys->m_rays[cloop].m_dynStartPoint,
																		entLoc->m_collisionRaySys->m_rays[cloop].m_dynEndPoint,
																		entLoc->m_collisionRaySys->m_rays[cloop].m_dynStartPoint,
																		entLoc->m_collisionRaySys->m_rays[cloop].m_dynEndPoint,
																		&entLoc->m_collisionRaySys->m_rays[cloop].m_lastIntersect.x,
																		&entLoc->m_collisionRaySys->m_rays[cloop].m_lastIntersect.y,
																		&entLoc->m_collisionRaySys->m_rays[cloop].m_lastIntersect.z,
																		&indexOfObject,
																		&latestNormal,
																		true,
																		dist,
																		entLoc->m_collisionRaySys->m_rays[cloop].m_directionalGen
																		)==TRUE)
		{//collided
			RayHandler(cloop,collisionObject,entLoc,projPos,position,dirtemp,offset,latestNormal,matLib,dist);
		}//end collided
		else
		{
		}
		entLoc->m_collisionRaySys->m_rays[cloop].ShockCallback();
		entLoc->m_collisionRaySys->m_rays[cloop].HandleTireSteeringRotation(matLib);
	   }//end world collision
	   //------------------------------------//
	   //--do adjustments for these specific at one time-------//
	   //------------------------------------//
	   if(entLoc->m_collisionType == 4)
		{//post collision handler
		   
		CCollision_Ray * deepestRay = entLoc->m_collisionRaySys->GetDeepestRay(3);
		if(deepestRay)
		{
			//update all rays before corrective
			entLoc->m_collisionRaySys->RecalculateByIndex(  deepestRay->m_indexRef,entLoc->List_baseFrame, entLoc->List_skeletalObject,matLib,&position,&entLoc->List_movementDB->List_lastPosition);
			entLoc->m_collisionRaySys->RecalculateByIndex(  deepestRay->m_dirPivot->m_indexRef,entLoc->List_baseFrame, entLoc->List_skeletalObject,matLib,&position,&entLoc->List_movementDB->List_lastPosition);
			entLoc->m_collisionRaySys->RecalculateByIndex(  deepestRay->m_crossPivot->m_indexRef,entLoc->List_baseFrame, entLoc->List_skeletalObject,matLib,&position,&entLoc->List_movementDB->List_lastPosition);
			      
			D3DMATRIX diff = entLoc->m_collisionRaySys->Get4pointOrientationMatrixBasedDPRay(matLib,deepestRay);
			matLib->fastMatrixMultiply(entLoc->List_baseFrame->List_frameMatrix,entLoc->List_baseFrame->List_frameMatrix,diff);
			  
			  
			//after 3 point corrective check last wheel for collisions
			deepestRay->m_xPivot->m_collidedThisRound = false;
			entLoc->List_baseFrame->GetPosition(NULL,&position);
			if(entLoc->m_collisionRaySys->RecalculateByIndex((int)deepestRay->m_xPivot->m_indexRef,
															entLoc->List_baseFrame, 
															entLoc->List_skeletalObject,
															matLib,
															&position,
															&entLoc->List_movementDB->List_lastPosition) == true)
				if(collisionObject->GetClosesIntersectionUseSeg(
																			deepestRay->m_xPivot->m_dynStartPoint,
																			deepestRay->m_xPivot->m_dynEndPoint,
																			deepestRay->m_xPivot->m_dynStartPoint,
																			deepestRay->m_xPivot->m_dynEndPoint,
																			&deepestRay->m_xPivot->m_lastIntersect.x,
																			&deepestRay->m_xPivot->m_lastIntersect.y,
																			&deepestRay->m_xPivot->m_lastIntersect.z,
																			&indexOfObject,
																			&latestNormal,
																			true,
																			dist,
																			deepestRay->m_xPivot->m_directionalGen
																			)==TRUE)
			{//collided
				deepestRay->m_xPivot->m_collidedThisRound = true;
				deepestRay->m_xPivot->m_depth = dist;
				
				if(dist > .02f)
				{//do corrective
	           
					entLoc->m_collisionRaySys->RecalculateByIndex(  deepestRay->m_xPivot->m_indexRef,entLoc->List_baseFrame, entLoc->List_skeletalObject,matLib,&position,&entLoc->List_movementDB->List_lastPosition);
					entLoc->m_collisionRaySys->RecalculateByIndex(  deepestRay->m_xPivot->m_dirPivot->m_indexRef,entLoc->List_baseFrame, entLoc->List_skeletalObject,matLib,&position,&entLoc->List_movementDB->List_lastPosition);
					entLoc->m_collisionRaySys->RecalculateByIndex(  deepestRay->m_xPivot->m_crossPivot->m_indexRef,entLoc->List_baseFrame, entLoc->List_skeletalObject,matLib,&position,&entLoc->List_movementDB->List_lastPosition);
				  	 
					diff = entLoc->m_collisionRaySys->Get4pointOrientationMatrixBasedDPRay(matLib,deepestRay->m_xPivot);
					matLib->fastMatrixMultiply(entLoc->List_baseFrame->List_frameMatrix,entLoc->List_baseFrame->List_frameMatrix,diff);

					D3DVECTOR lastLastPos = entLoc->List_movementDB->List_lastPosition;
					D3DVECTOR curPosNow;
					entLoc->List_baseFrame->GetPosition(NULL,&curPosNow);
				}//end do corrective			 
			}//end collided

			if(entLoc->m_collisionRaySys->GetCollisionCountWheelSys() > 2)
			{
				entLoc->List_movementDB->List_doGravity = FALSE;
				entLoc->List_movementDB->List_currentlyJumping = FALSE;
			}
		}
		entLoc->m_collisionRaySys->WheelCallback(matLib,entLoc->List_baseFrame->List_frameMatrix);

		}//end post collision handler
		//---------------------------------//
		//-------run rest of rays----------//
		//---------------------------------//
		for(cloop=4;cloop<entLoc->m_collisionRaySys->m_rayCount;cloop++)
	    {//world collision
			entLoc->List_baseFrame->GetPosition(NULL,&position);
			
			if(entLoc->m_collisionRaySys->RecalculateByIndex(  cloop,
															entLoc->List_baseFrame, 
															entLoc->List_skeletalObject,
															matLib,
															&position,
															&entLoc->List_movementDB->List_lastPosition) == false)
															continue;
			if(collisionObject->GetClosesIntersectionUseSeg(
																			entLoc->m_collisionRaySys->m_rays[cloop].m_dynStartPoint,
																			entLoc->m_collisionRaySys->m_rays[cloop].m_dynEndPoint,
																			entLoc->m_collisionRaySys->m_rays[cloop].m_dynStartPoint,
																			entLoc->m_collisionRaySys->m_rays[cloop].m_dynEndPoint,
																			&entLoc->m_collisionRaySys->m_rays[cloop].m_lastIntersect.x,
																			&entLoc->m_collisionRaySys->m_rays[cloop].m_lastIntersect.y,
																			&entLoc->m_collisionRaySys->m_rays[cloop].m_lastIntersect.z,
																			&indexOfObject,
																			&latestNormal,
																			true,
																			dist,
																			entLoc->m_collisionRaySys->m_rays[cloop].m_directionalGen
																			)==TRUE)
			{//collided
				RayHandler(cloop,collisionObject,entLoc,projPos,position,dirtemp,offset,latestNormal,matLib,dist);
			}//end collided
	    }//end world collision
	}//END VEHICLE SPECIFIC
	else
	{//LAND SPECIFIC
       for(int cloop=(entLoc->m_collisionRaySys->m_rayCount-1);cloop>-1;cloop--)
	   {//world collision
		entLoc->List_baseFrame->GetPosition(NULL,&position);
		
		if(entLoc->m_collisionRaySys->RecalculateByIndex(  cloop,
														   entLoc->List_baseFrame, 
														   entLoc->List_skeletalObject,
														   matLib,
														   &position,
														   &entLoc->List_movementDB->List_lastPosition) == false)
														   continue;
		if(collisionObject->GetClosesIntersectionUseSeg(
																		entLoc->m_collisionRaySys->m_rays[cloop].m_dynStartPoint,
																		entLoc->m_collisionRaySys->m_rays[cloop].m_dynEndPoint,
																		entLoc->m_collisionRaySys->m_rays[cloop].m_dynStartPoint,
																		entLoc->m_collisionRaySys->m_rays[cloop].m_dynEndPoint,
																		&entLoc->m_collisionRaySys->m_rays[cloop].m_lastIntersect.x,
																		&entLoc->m_collisionRaySys->m_rays[cloop].m_lastIntersect.y,
																		&entLoc->m_collisionRaySys->m_rays[cloop].m_lastIntersect.z,
																		&indexOfObject,
																		&latestNormal,
																		true,
																		dist,
																		entLoc->m_collisionRaySys->m_rays[cloop].m_directionalGen
																		)==TRUE)
		{//collided
			RayHandler(cloop,collisionObject,entLoc,projPos,position,dirtemp,offset,latestNormal,matLib,dist);
		}//end collided
		else
		{
		}
		//entLoc->m_collisionRaySys->m_rays[cloop].ShockCallback();
		//entLoc->m_collisionRaySys->m_rays[cloop].HandleTireSteeringRotation(matLib);
		
	   }//end world collision
	}//END LAND SPECIFIC
    
	 
	 
    
	

    if(correctiveOnly==false)
	   CorrectiveUnbreakableArrayToStatic(  entLoc,
											intersection,
											collisionObject,
											indexOfObject,
											latestNormal,
											dist,
											matLib,
											false);

    if(transformOfCollisionSys)
    {
             entLoc->List_baseFrame->AddTransformAfter(*transformOfCollisionSys);
             entLoc->m_collisionRaySys->m_lastCenterMassPosition.x += transformOfCollisionSys->_41;
		     entLoc->m_collisionRaySys->m_lastCenterMassPosition.y += transformOfCollisionSys->_42;
		     entLoc->m_collisionRaySys->m_lastCenterMassPosition.z += transformOfCollisionSys->_43;
		     entLoc->List_movementDB->List_lastPosition.x += transformOfCollisionSys->_41;
		     entLoc->List_movementDB->List_lastPosition.y += transformOfCollisionSys->_42;
		     entLoc->List_movementDB->List_lastPosition.z += transformOfCollisionSys->_43;
			 entLoc->List_lastPosition.x += transformOfCollisionSys->_41;
		     entLoc->List_lastPosition.y += transformOfCollisionSys->_42;
		     entLoc->List_lastPosition.z += transformOfCollisionSys->_43;
	}

}
//--------------collision pipeline------------------//
void CEntityManagement::CollisionPipelinePerEntityWld(MatrixARB * matLib, 
													  CMovementObj * entLoc,
													  CCollisionObj * collisionObject,
													  bool correctiveOnly)
{
	if(entLoc->m_collisionRaySys->m_worldCorrected)
	   return;

	
    TableCollisionPipelinePerEntityCore(matLib, 
									    entLoc,
									    collisionObject,
									    NULL,
									    NULL,
										correctiveOnly);

	EntityToHouseCollision(entLoc,matLib,correctiveOnly);//housing corrective

	entLoc->m_collisionRaySys->m_worldCorrected = TRUE;
}
//--------------Collision between entities--------------------------------------------//
/*void CEntityManagement::DoExactPerPolyCollisionProcess(CMovementObj * entityFocus,
													   CSkeletonObject * skeletalCompare,
													   bool & correctiveOnly,
													   D3DVECTOR & intersection,
	                                                   D3DMATRIX & boneMatrix,
	                                                   CFrameObj ** framePtr,
													   MatrixARB * mathLib,
													   D3DMATRIX & matrixEntityCmp,
													   int & traceOption,
													   CMovementObjList * entObjRuntime)
{
	 D3DVECTOR offset,position,latestNormal,projectedPos,tempIntersect,dirtemp;
	 float dist = 0.0f;
	 float additionalDist = 0.0f;
	 bool doCorner = false;
	 bool nuetralize = false;

		if(correctiveOnly == false)
		if(entObjRuntime->SegmentToEntity( entityFocus->m_collisionRaySys->m_lastCenterMassPosition,
										   entityFocus->m_collisionRaySys->m_centerMassPosition,
										   skeletalCompare,//mPtr->List_skeletalObject,
										   mathLib,
										   &intersection,
										   &boneMatrix,
										   framePtr,
										   matrixEntityCmp,
										   latestNormal,
										   dist
										   )==TRUE)
	    {//collided
				intersection = mathLib->UpdatePolygonVertex(boneMatrix.m,intersection);
				entityFocus->List_baseFrame->GetPosition(NULL,&position);
				offset.x = intersection.x - entityFocus->m_collisionRaySys->m_centerMassPosition.x;
				offset.y = intersection.y - entityFocus->m_collisionRaySys->m_centerMassPosition.y;
				offset.z = intersection.z - entityFocus->m_collisionRaySys->m_centerMassPosition.z;
				position.x += offset.x;
				position.y += offset.y;
				position.z += offset.z;
				entityFocus->List_baseFrame->SetPosition(NULL,position);
				entityFocus->m_collisionRaySys->m_centerOfMassCollided = true;
	    }//end collided

		//TUNING SEGMENT
		doCorner = false;
		nuetralize = false;
		entityFocus->List_baseFrame->GetPosition(NULL,&position);
        entityFocus->m_collisionRaySys->UpdateRays(entityFocus->List_skeletalObject);
		for(int cloop=(entityFocus->m_collisionRaySys->m_rayCount-1);cloop>-1;cloop--)
		{//poly ray collision
			if(entityFocus->m_collisionRaySys->RecalculateByIndex(  cloop,
															        entityFocus->List_baseFrame, 
															        entityFocus->List_skeletalObject,
															        mathLib,
																	&position,
																	&entityFocus->List_movementDB->List_lastPosition)==false)
																	continue;

		
			if(entObjRuntime->SegmentToEntity(  entityFocus->m_collisionRaySys->m_rays[cloop].m_dynStartPoint,
												entityFocus->m_collisionRaySys->m_rays[cloop].m_dynEndPoint,
												skeletalCompare,
												mathLib,
												&entityFocus->m_collisionRaySys->m_rays[cloop].m_lastIntersect,
												&boneMatrix,
												framePtr,
												matrixEntityCmp,
												latestNormal,
												dist
												)==TRUE)
			{//collided
				entityFocus->m_collisionRaySys->m_rays[cloop].m_lastIntersect = mathLib->UpdatePolygonVertex(boneMatrix.m,entityFocus->m_collisionRaySys->m_rays[cloop].m_lastIntersect);
				entityFocus->List_baseFrame->GetPosition(NULL,&position);
				entityFocus->m_collisionRaySys->m_rays[cloop].m_lastNormal = mathLib->UpdatePolygonVertexRotationOnly(boneMatrix.m,latestNormal);
				
				if(cloop == 0)
				{
                    offset.x = entityFocus->m_collisionRaySys->m_rays[cloop].m_lastIntersect.x - entityFocus->m_collisionRaySys->m_rays[cloop].m_dynEndPoint.x;
				    offset.y = entityFocus->m_collisionRaySys->m_rays[cloop].m_lastIntersect.y - entityFocus->m_collisionRaySys->m_rays[cloop].m_dynEndPoint.y;
				    offset.z = entityFocus->m_collisionRaySys->m_rays[cloop].m_lastIntersect.z - entityFocus->m_collisionRaySys->m_rays[cloop].m_dynEndPoint.z;
				
				    position.x += offset.x;
				    position.y += offset.y;
				    position.z += offset.z;

					entityFocus->List_baseFrame->SetPosition(NULL,position);
					entityFocus->List_baseFrame->List_frameMatrix._42 -= .02f;
					entityFocus->List_movementDB->List_doGravity = FALSE;
					entityFocus->List_movementDB->List_currentlyJumping = FALSE;
					//lock up entity
					entityFocus->List_movementDB->m_useLockPoint	= 1;//0=currently unlocked   1=currently locked 
					entityFocus->List_movementDB->m_lastLock		= boneMatrix;//
					entityFocus->List_movementDB->m_optFramePtr		= *framePtr;
					//entityFocus->List_movementDB->m_optLockTrace	= mPtr->List_traceNumber;
					if(traceOption > -1)
					   entityFocus->List_movementDB->m_optLockTrace	= traceOption;
				}
				else
				{
					projectedPos = position;
                    offset = m_gStatus->m_matLib.GetAdjustVector(dist,entityFocus->m_collisionRaySys->m_rays[cloop].m_lastNormal);
                    projectedPos.x += offset.x;
					projectedPos.y += offset.y;
					projectedPos.z += offset.z;

                    additionalDist = fabs(entityFocus->List_skeletalObject->m_box.maxZ);
				    if(additionalDist < 2.0f)
					   additionalDist = 2.0f;

					
				    dirtemp.x = projectedPos.x - entityFocus->List_lastPosition.x; 
				    dirtemp.y = projectedPos.y - entityFocus->List_lastPosition.y; 
				    dirtemp.z = projectedPos.z - entityFocus->List_lastPosition.z;

				    dirtemp = m_gStatus->m_matLib.Normalize(dirtemp);
				    dirtemp.x = dirtemp.x * additionalDist;
				    dirtemp.y = dirtemp.y * additionalDist;
				    dirtemp.z = dirtemp.z * additionalDist;
				    projectedPos.x += dirtemp.x;
				    projectedPos.y += dirtemp.y;
				    projectedPos.z += dirtemp.z;
				    doCorner = false;
					CopyMemory(&dirtemp,&entityFocus->List_lastPosition,sizeof(D3DVECTOR));
 
                    dirtemp.y = entityFocus->m_collisionRaySys->m_rays[cloop].m_dynStartPoint.y;
			        projectedPos.y = entityFocus->m_collisionRaySys->m_rays[cloop].m_dynStartPoint.y;
					
					if(entObjRuntime->SegmentToEntity(   dirtemp,
														 projectedPos,
														 skeletalCompare,
														 mathLib,
														 &tempIntersect,
														 &boneMatrix,
														 framePtr,
														 matrixEntityCmp,
														 latestNormal,
														 dist
														 )==TRUE)
											            doCorner = true;

					   if(doCorner)
					   {//Handle corners 

							offset.x = entityFocus->m_collisionRaySys->m_rays[cloop].m_lastIntersect.x - entityFocus->m_collisionRaySys->m_rays[cloop].m_dynEndPoint.x;
							offset.y = entityFocus->m_collisionRaySys->m_rays[cloop].m_lastIntersect.y - entityFocus->m_collisionRaySys->m_rays[cloop].m_dynEndPoint.y;
							offset.z = entityFocus->m_collisionRaySys->m_rays[cloop].m_lastIntersect.z - entityFocus->m_collisionRaySys->m_rays[cloop].m_dynEndPoint.z;

							position.x += offset.x;
							position.y += offset.y;
							position.z += offset.z; 

							entityFocus->List_movementDB->List_lastPosition.x = position.x;
							entityFocus->List_movementDB->List_lastPosition.y = position.y;
							entityFocus->List_movementDB->List_lastPosition.z = position.z;
					   }//end Handle corners 
					   else
					   {//handle slide
							position.x += offset.x;
							position.y += offset.y;
							position.z += offset.z; 
			           }//end handle slide
					entityFocus->List_baseFrame->SetPosition(NULL,position);
				}
			}//end collided
		}//end poly ray collision
}*/
//--------------Collision between entities--------------------------------------------//
BOOL CEntityManagement::CollisionLoopEntityToEntityHybrid(CMovementObj * entityFocus, 
													      CMovementObjList * entObjRuntime,
													      MatrixARB * mathLib,
														  bool correctiveOnly)
{
    //return FALSE;//REMOVE

	if(m_gStatus->m_multiplayerObject->m_AIServer)
	  return FALSE;

	if(entityFocus->List_networkEntity == TRUE)
	  return FALSE;

    MoveEntityBasedOnLock(entityFocus,entObjRuntime,mathLib);//if locked make the move
	
	
	entityFocus->m_collisionRaySys->UpdateCurrentPosition(entityFocus->List_baseFrame,entityFocus->List_skeletalObject,mathLib); 

	D3DVECTOR intersection;
	D3DMATRIX boneMatrix;
	CFrameObj * framePtr;

	if(entityFocus->List_controlType == 0)
	   m_flag	= 0;

	for(POSITION posLoc = entObjRuntime->GetHeadPosition();posLoc != NULL;)
	{//entity loop
        CMovementObj * mPtr = (CMovementObj *)entObjRuntime->GetNext(posLoc);
        
		if(entityFocus->m_possedBy)
		if(entityFocus->m_possedBy->List_traceNumber == mPtr->List_traceNumber)
		   continue;
		
		if(mPtr->List_traceNumber == entityFocus->List_traceNumber)
		   continue;

		//bounding checks
        if(!mathLib->BoundingSphereCheckX(  entityFocus->List_boundingRadius, 
											mPtr->List_boundingRadius,
											entityFocus->List_baseFrame->List_frameMatrix._41,
											entityFocus->List_baseFrame->List_frameMatrix._42,
											entityFocus->List_baseFrame->List_frameMatrix._43,
											mPtr->List_baseFrame->List_frameMatrix._41,
											mPtr->List_baseFrame->List_frameMatrix._42,
											mPtr->List_baseFrame->List_frameMatrix._43))
		   continue;

		if(entityFocus->List_controlType == 0)
		{
		   AutoTrigger(entityFocus,mPtr);
           m_flag = 1;
		}

		if(mPtr->m_collisionType == 1)
		{//sphere collision
           SphereEntityToEntityCollision(entityFocus,mPtr);
		}//end sphere collision
		if(mPtr->m_collisionType == 2)
		{//exact per poly collision test
		   D3DMATRIX tempMat;
		   mPtr->List_baseFrame->GetTransform(NULL,&tempMat);
		   entObjRuntime->DoExactPerPolyCollisionProcess(entityFocus,mPtr->List_skeletalObject,correctiveOnly,intersection,boneMatrix,&framePtr,mathLib,tempMat,mPtr->List_traceNumber);
		}//end exact per poly collision test
	}//end entity loop

	if(entityFocus->List_controlType == 0)
	   if(m_flag == 0)
	      m_insideInteractRadius = FALSE;
	


	//reference entities
	CReferenceObj * referenceObject = NULL;
	ABBOX tempBox;
	tempBox.maxX = entityFocus->List_skeletalObject->m_box.maxX + entityFocus->List_baseFrame->List_frameMatrix._41;
	tempBox.maxY = entityFocus->List_skeletalObject->m_box.maxY + entityFocus->List_baseFrame->List_frameMatrix._42;
	tempBox.maxZ = entityFocus->List_skeletalObject->m_box.maxZ + entityFocus->List_baseFrame->List_frameMatrix._43;
	tempBox.minX = entityFocus->List_skeletalObject->m_box.maxX - entityFocus->List_baseFrame->List_frameMatrix._41;
	tempBox.minX = entityFocus->List_skeletalObject->m_box.maxY - entityFocus->List_baseFrame->List_frameMatrix._42;
	tempBox.minX = entityFocus->List_skeletalObject->m_box.maxZ - entityFocus->List_baseFrame->List_frameMatrix._43;
	int traceNull = -1;
	if(entityFocus->List_skeletalObject)
    for(POSITION posSkLoc = m_gStatus->m_skeletalReferenceObjCache.GetHeadPosition();posSkLoc != NULL;)
	{//loop through all references that were cached near the entity
        CWldObject * wPtr = (CWldObject *) m_gStatus->m_skeletalReferenceObjCache.GetNext(posSkLoc);
		if(wPtr->m_node->m_refPtr)
		{//use mem references
           referenceObject = (CReferenceObj *)wPtr->m_node->m_refPtr;
           if(referenceObject->m_skeletalOptionLive)
		   {//check collision
              //bounding check			  
			   if(mathLib->BoundingBoxToBoxCheck(
												  wPtr->m_node->m_clipBox.maxX,
												  wPtr->m_node->m_clipBox.maxY,
												  wPtr->m_node->m_clipBox.maxZ,
												  wPtr->m_node->m_clipBox.minX,
												  wPtr->m_node->m_clipBox.minY,
												  wPtr->m_node->m_clipBox.minZ,
												  tempBox.maxX,//float & b2maxX,
												  tempBox.maxY,//float & b2maxY,
												  tempBox.maxZ,//
												  tempBox.minX,//float & b2minX,
												  tempBox.minY,//float & b2minY,
												  tempBox.minZ))//float & b2minZ))
			   {//possible collision
				    traceNull = -wPtr->List_identity;
					entObjRuntime->DoExactPerPolyCollisionProcess(	entityFocus,
																	referenceObject->m_skeletalOptionLive,
																	correctiveOnly,
																	intersection,
																	boneMatrix,
																	&framePtr,
																	mathLib,
																	wPtr->m_node->m_position,
																	traceNull);
			   }//end possible collision
		   }//end check collision
		}//end use mem references
	}//end loop through all references that were cached near the entity
	
	return TRUE;
}

//--------------checks Collision between a world object and a entity------------------//
void CEntityManagement::EntityToHouseCollision(CMovementObj * entLoc,MatrixARB * mathLib,bool correctiveOnly)
{	
	

	if(entLoc->List_controlType != 15 && entLoc->List_controlType != 35)
	{//if valid
   
	 if(!m_gStatus->m_multiplayerObject->m_AIServer)
	 {//use cache for client
	   for(POSITION hPos = m_gStatus->m_houseingCache->GetHeadPosition();hPos != NULL;)
	   {//Houseing
          CHousingCacheObj * hsPtr = (CHousingCacheObj *)m_gStatus->m_houseingCache->GetNext(hPos);
	      CHousingObj * houseLib = m_gStatus->m_housingDB->GetByIndex(hsPtr->m_dbIndex);
          if(houseLib)
		  {
		     if(hsPtr->m_open)
	            TableCollisionPipelinePerEntityCore(mathLib,entLoc,houseLib->m_openState,&hsPtr->m_location,&hsPtr->m_invLocation,correctiveOnly);
			 else
				TableCollisionPipelinePerEntityCore(mathLib,entLoc,houseLib->m_closedState,&hsPtr->m_location,&hsPtr->m_invLocation,correctiveOnly);
		  }
	   }//end Houseing
	 }//end use cache for client
	 else{//tabled mode for ai
	   if(m_gStatus->m_multiplayerObject->m_houseingDB)
	   {//house table
          D3DVECTOR wldPosition;
          entLoc->List_baseFrame->GetPosition(NULL,&wldPosition);
          CHPlacementObjList * placementList = m_gStatus->m_multiplayerObject->m_houseingDB->GetContainer(wldPosition);
          if(placementList)
          for(POSITION hPos = placementList->GetHeadPosition();hPos != NULL;)
		  {//Houseing
             CHPlacementObj * hsPtr = (CHPlacementObj *)placementList->GetNext(hPos);
	         CHousingObj * houseLib = m_gStatus->m_housingDB->GetByIndex(hsPtr->m_type);
             if(houseLib)
			 {
		        D3DMATRIX regMatrix,inverseMatrix;
                m_gStatus->m_matLib.InitMat(regMatrix.m);
		        m_gStatus->m_matLib.InitMat(inverseMatrix.m);

		        inverseMatrix._41 = -hsPtr->m_wldPosition.x;
		        inverseMatrix._42 = -hsPtr->m_wldPosition.y;
		        inverseMatrix._43 = -hsPtr->m_wldPosition.z;
		        regMatrix._41 = hsPtr->m_wldPosition.x;
		        regMatrix._42 = hsPtr->m_wldPosition.y;
		        regMatrix._43 = hsPtr->m_wldPosition.z;

				if(hsPtr->m_open)
	            TableCollisionPipelinePerEntityCore(mathLib,entLoc,houseLib->m_openState,&regMatrix,&inverseMatrix,correctiveOnly);
			    else
				TableCollisionPipelinePerEntityCore(mathLib,entLoc,houseLib->m_closedState,&regMatrix,&inverseMatrix,correctiveOnly);
			 }
		  }//end Houseing
	   }//end house table
	 }//end tabled mode for ai
	}//end valid
}
//--------------checks Collision between a world object and a entity------------------//
void CEntityManagement::AutoTrigger(CMovementObj * entityFocus,CMovementObj * entityCmpare)
{                 
		if(entityCmpare->List_controlType == 35 || entityCmpare->List_skeletalObject->m_currentAnimType == 8)
		{//check to see if auto exec or notify
            if(entityCmpare->m_autoTriggerTemp==1)
			{//constant
				if(GetTickCount()-m_lastAutoQuerryStamp > 1000)
				{
				m_lastAutoQuerryStamp = GetTickCount();
                m_gStatus->SendChatString(m_gStatus->m_globalInteractCommand);
				m_gStatus->m_globalAutoTriggerLast = entityCmpare->List_traceNumber;
				}

			}
			else if(entityCmpare->m_autoTriggerTemp == 2)
			{//once per level
				if(m_gStatus->m_globalAutoTriggerLast != entityCmpare->List_traceNumber)
				{
                    m_gStatus->SendChatString(m_gStatus->m_globalInteractCommand);
					entityCmpare->m_autoTriggerTemp = 0;//this is once per level case !!!!!!!!!!DO NOT REMOVE!!!!!!$@##@
				}
				else
				{ 
					if(entityCmpare->List_controlType == 35)
                       m_gStatus->gl_helperString = "Press the Interaction Key to Interact";
					else
					   m_gStatus->gl_helperString = "Press the Interaction Key to Loot";
					   ControlDBObj * controller = m_gStatus->m_controlsList->GetByIndex(0);
					   if(controller)
					   if(controller->List_controlInfoList)
					   {
					       ControlInfoObj* controlObj = controller->List_controlInfoList->GetByActionName("Interaction");
						   if(controlObj)
						   {
								int iKey = controlObj->List_buttonDown;
								if(controlObj->List_controlType == 1) 
								   iKey += KEYBOARD_MAX_KEYS;
								m_gStatus->gl_helperString.Replace("Interaction",controller->List_controlInfoList->ReturnKeyStringByIndex(iKey));	  	  
						   }
					   }
				}
				m_gStatus->m_globalAutoTriggerLast = entityCmpare->List_traceNumber;
			}
			else if(entityCmpare->m_autoTriggerTemp == 3)
			{//once inside radius
				if(!m_insideInteractRadius)
				{
                    m_gStatus->SendChatString(m_gStatus->m_globalInteractCommand);
					m_insideInteractRadius = TRUE;
				}
			}
			else
			{
			    if(entityCmpare->List_controlType == 35)
					m_gStatus->gl_helperString = "Press the Interaction Key to Interact";
				else
					m_gStatus->gl_helperString = "Press the Interaction Key to Loot";
				   ControlDBObj * controller = m_gStatus->m_controlsList->GetByIndex(0);
				   if(controller)
				   if(controller->List_controlInfoList)
				   {
					   ControlInfoObj* controlObj = controller->List_controlInfoList->GetByActionName("Interaction");
					   if(controlObj)
					   {
							int iKey = controlObj->List_buttonDown;
							if(controlObj->List_controlType == 1) 
							   iKey += KEYBOARD_MAX_KEYS;
							m_gStatus->gl_helperString.Replace("Interaction",controller->List_controlInfoList->ReturnKeyStringByIndex(iKey));	  	  
					   }
				   }
				m_gStatus->m_tempMenuFlag = 950;
				m_gStatus->m_globalAutoTriggerLast = entityCmpare->List_traceNumber;
			}
		}
}
//--------------checks Collision between a world object and a entity------------------//
void CEntityManagement::CorrectiveUnbreakableArrayToStatic(CMovementObj * entLoc,
														   D3DVECTOR & intersection,
														   CCollisionObj * collisionObject,
														   int & indexOfObject,
														   D3DVECTOR & latestNormal,
														   float & dist,
														   MatrixARB * mathLib,
														   bool slideMethod)
{
	byte temp = 0;
	entLoc->m_collisionRaySys->UpdateCurrentPosition(entLoc->List_baseFrame,entLoc->List_skeletalObject,mathLib);
	if(collisionObject->GetClosesIntersectionUseSeg(
																	entLoc->m_collisionRaySys->m_lastCenterMassPosition,
																	entLoc->m_collisionRaySys->m_centerMassPosition,
																	entLoc->m_collisionRaySys->m_lastCenterMassPosition,
																	entLoc->m_collisionRaySys->m_centerMassPosition,
																	&intersection.x,
																	&intersection.y,
																	&intersection.z,
																	&indexOfObject,
																	&latestNormal,
																	true,
																	dist,
																	temp
																	)==TRUE)
	{//IF COLLIDED HANDLE CORRECTIVE
			D3DVECTOR offset,position;
			entLoc->List_baseFrame->GetPosition(NULL,&position);
			//slide
            if(slideMethod)
			{
               offset = mathLib->GetAdjustVector(dist,latestNormal);
			}
			else
			{
              offset.x = intersection.x - entLoc->m_collisionRaySys->m_centerMassPosition.x;
			  offset.y = intersection.y - entLoc->m_collisionRaySys->m_centerMassPosition.y;
			  offset.z = intersection.z - entLoc->m_collisionRaySys->m_centerMassPosition.z;
			}
			
			position.x += offset.x;
			position.y += offset.y;
			position.z += offset.z;
			entLoc->List_baseFrame->SetPosition(NULL,position);
			//entLoc->List_movementDB->List_lastPosition.x += offset.x;
			//entLoc->List_movementDB->List_lastPosition.y += offset.y;
			//entLoc->List_movementDB->List_lastPosition.z += offset.z;
			
			entLoc->m_collisionRaySys->m_centerOfMassCollided = true;
	}//END IF COLLIDED HANDLE CORRECTIVE
}
//--------------collision pipeline------------------//
void CEntityManagement::RayHandler(int & rayIndex,
								   CCollisionObj * collisionObject,
								   CMovementObj * entLoc,
								   D3DVECTOR & projPos,
								   D3DVECTOR & position,
								   D3DVECTOR & dirtemp,
								   D3DVECTOR & offset,
								   D3DVECTOR & latestNormal,
								   MatrixARB * matLib,
								   float & dist
								   )
{
	
	        D3DVECTOR lastPosition;
            entLoc->m_collisionRaySys->m_rays[rayIndex].m_collidedThisRound = true;
			entLoc->List_baseFrame->GetPosition(NULL,&position);
			CopyMemory(&lastPosition,&position,sizeof(D3DVECTOR));

			if(entLoc->m_collisionRaySys->m_rays[rayIndex].m_directionalGen == 1 || entLoc->m_collisionRaySys->m_rays[rayIndex].m_directionalGen == 2)
			{
			   //CopyMemory(&lastPos,&entLoc->List_lastPosition,sizeof(D3DVECTOR));
			   CopyMemory(&projPos,&position,sizeof(D3DVECTOR));
               offset = matLib->GetAdjustVector(dist,latestNormal);
			   projPos.x += offset.x;
			   projPos.y += offset.y;
			   projPos.z += offset.z; 

			   float additionalDist = fabs(entLoc->List_skeletalObject->m_box.maxZ);
			   if(additionalDist < 2.0f)
			      additionalDist = 2.0f;

			   dirtemp.x = projPos.x - entLoc->List_lastPosition.x; 
			   dirtemp.y = projPos.y - entLoc->List_lastPosition.y; 
			   dirtemp.z = projPos.z - entLoc->List_lastPosition.z;

			   dirtemp = matLib->Normalize(dirtemp);
			   dirtemp.x = dirtemp.x * additionalDist;
			   dirtemp.y = dirtemp.y * additionalDist;
			   dirtemp.z = dirtemp.z * additionalDist;
			   projPos.x += dirtemp.x;
               projPos.y += dirtemp.y;
               projPos.z += dirtemp.z;
			   bool doCorner = false;
               CopyMemory(&dirtemp,&entLoc->List_lastPosition,sizeof(D3DVECTOR));
			   if(entLoc->m_collisionRaySys->m_rays[rayIndex].m_directionalGen == 1)
			   {
			      dirtemp.y = entLoc->m_collisionRaySys->m_rays[rayIndex].m_dynStartPoint.y;
			      projPos.y = entLoc->m_collisionRaySys->m_rays[rayIndex].m_dynStartPoint.y;
				  if(collisionObject->CollisionSegmentSIBasic(dirtemp,projPos,20.0f))
				     doCorner = true;
			   }

			   
			   
			   if(doCorner)
			   {//Handle corners 

					offset.x = entLoc->m_collisionRaySys->m_rays[rayIndex].m_lastIntersect.x - entLoc->m_collisionRaySys->m_rays[rayIndex].m_dynEndPoint.x;
					offset.y = entLoc->m_collisionRaySys->m_rays[rayIndex].m_lastIntersect.y - entLoc->m_collisionRaySys->m_rays[rayIndex].m_dynEndPoint.y;
					offset.z = entLoc->m_collisionRaySys->m_rays[rayIndex].m_lastIntersect.z - entLoc->m_collisionRaySys->m_rays[rayIndex].m_dynEndPoint.z;

					position.x += offset.x;
					position.y += offset.y;
					position.z += offset.z; 

					entLoc->List_movementDB->List_lastPosition.x = position.x;
					entLoc->List_movementDB->List_lastPosition.y = position.y;
					entLoc->List_movementDB->List_lastPosition.z = position.z;
			   }//end Handle corners 
			   else
			   {//handle slide
					position.x += offset.x;
					position.y += offset.y;
					position.z += offset.z; 
			   }//end handle slide

			   if(rayIndex == 0)
			   {
				entLoc->List_baseFrame->SetPosition(NULL,position);
				entLoc->List_movementDB->List_doGravity = FALSE;
				entLoc->List_movementDB->List_currentlyJumping = FALSE;
			   }
			   else
			   {
                entLoc->List_baseFrame->SetPosition(NULL,position);
				if(entLoc->m_aiObj)
				{
					entLoc->m_aiObj->m_bodyCollision							= TRUE;
					CopyMemory(&entLoc->m_aiObj->m_bodyCollisonVectLast,&latestNormal,sizeof(D3DVECTOR));
				}
			   }
			}
			else if(entLoc->m_collisionRaySys->m_rays[rayIndex].m_directionalGen == 0)
			{//ray solid corrective
		       offset.x = entLoc->m_collisionRaySys->m_rays[rayIndex].m_lastIntersect.x - entLoc->m_collisionRaySys->m_rays[rayIndex].m_dynEndPoint.x;
			   offset.y = entLoc->m_collisionRaySys->m_rays[rayIndex].m_lastIntersect.y - entLoc->m_collisionRaySys->m_rays[rayIndex].m_dynEndPoint.y;
			   offset.z = entLoc->m_collisionRaySys->m_rays[rayIndex].m_lastIntersect.z - entLoc->m_collisionRaySys->m_rays[rayIndex].m_dynEndPoint.z;
			   position.x += offset.x;
			   position.y += offset.y;
			   position.z += offset.z; 

			   if(rayIndex == 0)
			   {
				entLoc->List_baseFrame->SetPosition(NULL,position);
				entLoc->List_movementDB->List_doGravity = FALSE;
				entLoc->List_movementDB->List_currentlyJumping = FALSE;
			   }
			   else
                entLoc->List_baseFrame->SetPosition(NULL,position);
			}//end ray solid corrective
			else if(entLoc->m_collisionRaySys->m_rays[rayIndex].m_directionalGen == 3)
			{//tire collision handler	   
				entLoc->m_collisionRaySys->m_rays[rayIndex].m_depth = dist;
				CopyMemory(&entLoc->m_collisionRaySys->m_rays[rayIndex].m_lastNormal,&latestNormal,sizeof(D3DVECTOR)); 
			}//end tire collision handler
			else if(entLoc->m_collisionRaySys->m_rays[rayIndex].m_directionalGen == 4)
			{//
                offset = matLib->GetAdjustVector(dist,latestNormal);
				position.x += offset.x;
				position.y += offset.y;
				position.z += offset.z; 

                entLoc->List_baseFrame->SetPosition(NULL,position);
			}
			else if(entLoc->m_collisionRaySys->m_rays[rayIndex].m_directionalGen == 5)
			{//
                offset = matLib->GetAdjustVector(dist,latestNormal);
				position.x += offset.x;
			    position.y += offset.y;
			    position.z += offset.z; 
				entLoc->List_baseFrame->SetPosition(NULL,position);

			}
			
}
//----------------------------------------------------------------------------------------
void CEntityManagement::SphereEntityToEntityCollision(CMovementObj * entityFocus,CMovementObj * entityCmpare)
{
        if(!entityFocus->List_skeletalObject->m_sphereBodyCollisionSystem)
		    return;

        D3DVECTOR cmpPosition;
		entityCmpare->List_baseFrame->GetPosition(NULL,&cmpPosition);
		if(entityCmpare->List_skeletalObject->m_sphereBodyCollisionSystem)//has collision sys on object
		for(POSITION csysPos = entityFocus->List_skeletalObject->m_sphereBodyCollisionSystem->GetHeadPosition();csysPos != NULL;)
		{//g system loop
		CSphereCollisionObj * mainBdSphPtr = (CSphereCollisionObj *)entityFocus->List_skeletalObject->m_sphereBodyCollisionSystem->GetNext(csysPos);
		
		for(POSITION csysPos = entityCmpare->List_skeletalObject->m_sphereBodyCollisionSystem->GetHeadPosition();csysPos != NULL;)
		{//c system loop
		CSphereCollisionObj * bdSphPtr = (CSphereCollisionObj *)entityCmpare->List_skeletalObject->m_sphereBodyCollisionSystem->GetNext(csysPos);
		D3DVECTOR adjust;			   
		D3DVECTOR bonePositionCmp = m_gStatus->m_matLib.UpdatePolygonVertex(entityCmpare->List_baseFrame->List_frameMatrix.m,entityCmpare->List_skeletalObject->GetBonePositionByIndex(bdSphPtr->m_boneIndex));
		D3DVECTOR currentPosition;
	    entityFocus->List_baseFrame->GetPosition(NULL,&currentPosition);
		if(m_gStatus->m_matLib.CollisionSphereSeg(currentPosition,
					                              entityFocus->List_lastPosition,
					                              bdSphPtr->m_sphereRadius,
												  bonePositionCmp,
												  TRUE,
												  &adjust,
												  mainBdSphPtr->m_sphereRadius,
												  NULL)==TRUE)
		{//hit 
            //straight shphere reactional (works)
            /*D3DMATRIX matrix;
            m_gStatus->m_matLib.Translate((adjust.x*.5f),
						                  (adjust.y*.5f),
										  (adjust.z*.5f),matrix.m);
			entityFocus->List_baseFrame->AddTransformAfter(matrix);*/

			entityFocus->List_baseFrame->List_frameMatrix._41 += adjust.x;
			entityFocus->List_baseFrame->List_frameMatrix._42 += adjust.y;
			entityFocus->List_baseFrame->List_frameMatrix._43 += adjust.z;

			//entityFocus->List_movementDB->List_lastPosition.x += adjust.x;
			//entityFocus->List_movementDB->List_lastPosition.y += adjust.y;
		    //entityFocus->List_movementDB->List_lastPosition.z += adjust.z;
			break;
		}//end hit            
		}//end c system loop
		}//end g system loop
}
//--------------collision pipeline------------------//
void CEntityManagement::CollisionPipeline(MatrixARB * matLib, CMovementObjList * entObjRuntime,CMissileObjList * m_missileRuntime)
{
  if(!m_gStatus->m_multiplayerObject->m_AIServer)
      m_gStatus->gl_helperString = "";

  POSITION posLoc;
  for(posLoc = entObjRuntime->GetHeadPosition();posLoc != NULL;)
  {//Begin entity loop
    CMovementObj * entLocAct = ( CMovementObj* ) entObjRuntime->GetNext(posLoc);
	
	if(entLocAct->m_possedBy)
	   continue;

    CMovementObj * entLoc = entLocAct;
	if(entLocAct->m_posses)
	{
       
	   entLoc->m_animationsDone							= -1;
	   //m_gStatus->IndexEntityAnimations(entLoc,GetTickCount(),FALSE,m_missileRuntime);
	   entLoc = entLocAct->m_posses;
	}


    if(!entLoc->m_collisionRaySys)
	{//if system in unavailable initialize
		if(entLoc->m_collisionType == 2 || m_gStatus->m_multiplayerObject->m_AIServer == TRUE)
		   m_gStatus->IndexEntityAnimations(entLoc,GetTickCount(),FALSE,m_missileRuntime);
		entLoc->List_skeletalObject->QuerryBox();
		if(entLoc->List_movementDB->m_antiGravity == FALSE)
		{
		   if(entLoc->List_controlType == 0)
		   {
		      if(entLoc->m_collisionType == 4)
		         entLoc->m_collisionRaySys = new CCollision_RaySystem(entLoc->List_skeletalObject,4,matLib);
			  else
			     entLoc->m_collisionRaySys = new CCollision_RaySystem(entLoc->List_skeletalObject,0,matLib);
		   }
		   else
		   {
		      if(entLoc->m_collisionType == 4)
		         entLoc->m_collisionRaySys = new CCollision_RaySystem(entLoc->List_skeletalObject,4,matLib);
			  else
			     entLoc->m_collisionRaySys = new CCollision_RaySystem(entLoc->List_skeletalObject,1,matLib);
		   }
		}
		else
		{
		   entLoc->m_collisionRaySys = new CCollision_RaySystem(entLoc->List_skeletalObject,3,matLib);
		}
		
		
		

		
		entLoc->m_collisionRaySys->InitOnSpawn(entLoc->List_baseFrame,entLoc->List_skeletalObject,matLib);
		
		
	}//end if system in unavailable initialize

	entLoc->m_collisionRaySys->ResetCollisionFlags();
	entLoc->m_collisionRaySys->m_worldCorrected			= false;
	entLoc->m_collisionRaySys->m_centerOfMassCollided	= false;
	entLoc->m_animationsDone							= -1;
	
	if(entLoc->m_collisionType == 2 || m_gStatus->m_multiplayerObject->m_AIServer == TRUE)
	   m_gStatus->IndexEntityAnimations(entLoc,GetTickCount(),FALSE,m_missileRuntime);
    
 }//end entity loop
 //this above is looped first for animation  entity to entity reasons

 
float lastVdist = 0.0f;
float curVdist = 0.0f;
D3DVECTOR diffVect;

for(posLoc = entObjRuntime->GetHeadPosition();posLoc != NULL;)
{//process loop
    CMovementObj * entLocAct = ( CMovementObj* ) entObjRuntime->GetNext(posLoc);
	
    bool correctiveOnly = false;

	if(entLocAct->m_possedBy)
	   continue;

    CMovementObj * entLoc = entLocAct;
	if(entLocAct->m_posses)
	   entLoc = entLocAct->m_posses;

	if(entLoc->m_filterCollision)
       continue;

	if(entLoc->List_controlType == 35)
	   continue;

	correctiveOnly = false;
	//if(entLoc->m_collisionType == 4)
	   //correctiveOnly = true;


	if(entLoc->List_networkEntity)
	{
       if(m_gStatus->m_multiplayerObject->m_AIServer)
	      continue;

	   correctiveOnly = true;
	}
 
	//correctiveOnly = true;


	entLoc->List_movementDB->List_doGravity = TRUE;//FLAG ON
    //track current velocity vector 
    lastVdist = matLib->MagnitudeSquared(entLoc->List_baseFrame->List_frameMatrix._41,entLoc->List_baseFrame->List_frameMatrix._42,entLoc->List_baseFrame->List_frameMatrix._43,entLoc->List_movementDB->List_lastPosition.x,entLoc->List_movementDB->List_lastPosition.y,entLoc->List_movementDB->List_lastPosition.z);
    D3DVECTOR lastVVector = entLoc->List_movementDB->List_lastPosition;
	//end track current velocity vector

	CollisionLoopEntityToEntityHybrid(entLoc,entObjRuntime,matLib,correctiveOnly);//entity to entity collision

	CollisionPipelinePerEntityWld(matLib, entLoc,entLoc->m_collisionMovRefPtr,correctiveOnly);//entity to world collision

	if(entLoc->m_collisionRaySys->m_centerOfMassCollided == false)
	{
	   entLoc->m_collisionRaySys->UpdateLastPosition(entLoc->List_baseFrame,entLoc->List_skeletalObject,matLib);
	}
	else if(entLoc->List_energy < 1)
	{
            entLoc->List_movementDB->List_doGravity = FALSE;//allows for removal
	}

    //finalize velocity vector Modification


    /*if(fabs(entLoc->List_baseFrame->List_frameMatrix._41 - entLoc->List_movementDB->List_lastPosition.x) < .1)
	   entLoc->List_movementDB->List_lastPosition.x = entLoc->List_baseFrame->List_frameMatrix._41;

	if(fabs(entLoc->List_baseFrame->List_frameMatrix._42 - entLoc->List_movementDB->List_lastPosition.y) < .1)
		entLoc->List_movementDB->List_lastPosition.y = entLoc->List_baseFrame->List_frameMatrix._42;*/



	
	curVdist = matLib->MagnitudeSquared(entLoc->List_baseFrame->List_frameMatrix._41,entLoc->List_baseFrame->List_frameMatrix._42,entLoc->List_baseFrame->List_frameMatrix._43,entLoc->List_movementDB->List_lastPosition.x,entLoc->List_movementDB->List_lastPosition.y,entLoc->List_movementDB->List_lastPosition.z);
    diffVect.x = entLoc->List_movementDB->List_lastPosition.x - entLoc->List_baseFrame->List_frameMatrix._41;
	diffVect.y = entLoc->List_movementDB->List_lastPosition.y - entLoc->List_baseFrame->List_frameMatrix._42;
	diffVect.z = entLoc->List_movementDB->List_lastPosition.z - entLoc->List_baseFrame->List_frameMatrix._43;
	
	
	
	
	
	diffVect = matLib->Normalize(diffVect);


    if(curVdist < lastVdist)
	   lastVdist = curVdist;


    entLoc->List_movementDB->List_lastPosition.x = entLoc->List_baseFrame->List_frameMatrix._41 + (diffVect.x * lastVdist);
	entLoc->List_movementDB->List_lastPosition.y = entLoc->List_baseFrame->List_frameMatrix._42 + (diffVect.y * lastVdist);
    entLoc->List_movementDB->List_lastPosition.z = entLoc->List_baseFrame->List_frameMatrix._43 + (diffVect.z * lastVdist);
	

	//m_gStatus->m_systemReadout->AddLine("VV EMN Vect Dist:",(float)lastVdist);
	
	//entLoc->List_movementDB->List_lastPosition = lastVVector;

	
	
	//end finalize velocity vector Modification


 }//end process loop
}




