//OPTIMIZED FOR MOVING
typedef struct {
    int uniInt;
	int uniInt2;
	int uniInt3;
	int messageType;
	float Xpos;
	float Ypos;
	float Zpos;
	float OrientateDirX;
	float OrientateDirY;
	float OrientateDirZ;
	float OrientateUpX;
	float OrientateUpY;
	float OrientateUpZ;
	float FreeLookPitch;
	float FreeLookYaw;
	//float matrixX[4][4];
	long idRefrence;
} MOVE_STRUCT, *LPMOVE_STRUCT;


//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Manage smooth movement of network entities :))))----------------------//
void EngineXWin::ManageSmoothMovementOnNetworkEntities()
{
//there are functions useing Retained mode calls this is only useing Retainmode geometry engin
//function is addaptive to when it recieves movement packet info on network entity  
for(POSITION posEnt = EntObjRuntime->GetHeadPosition();posEnt!=NULL;){
   CMovementObj * ptrLoc = ( CMovementObj* ) EntObjRuntime->GetNext(posEnt);
   if(ptrLoc->List_networkEntity == TRUE){
	  

      float lagDist = CABLIB->MagnitudeSquared(
		                         ptrLoc->List_networkLastPositionXYZ.x,
		                         ptrLoc->List_networkLastPositionXYZ.y,
		                         ptrLoc->List_networkLastPositionXYZ.z,
		                         ptrLoc->List_networkTargetPositionXYZ.x,
		                         ptrLoc->List_networkTargetPositionXYZ.y,
				                 ptrLoc->List_networkTargetPositionXYZ.z);
      int interpolateAmount = 3+((int)(lagDist*.3));
      if(interpolateAmount > 20)
		 interpolateAmount = 20;
      

      if(ptrLoc->List_networkInterpCurStep > interpolateAmount/*(INTERPOLATIONGLOBAL+4)*/)
	     ptrLoc->List_networkInterpCurStep = interpolateAmount;

      /*CABD3DLIB->InterpolateMatrix(tempMatrixLoc,
										  ptrLoc->List_networkLastMatrix,//float StartVec[4][4], 
						                  ptrLoc->List_networkTargetMatrix,//float EndVec[4][4],
					                      (INTERPOLATIONGLOBAL+4),//int NumOfStages, 
					                      ptrLoc->List_networkInterpCurStep);//int ThisStage);*/

      D3DVECTOR newLocation = CABD3DLIB->InterpolatePoints(ptrLoc->List_networkLastPositionXYZ, 
						                        ptrLoc->List_networkTargetPositionXYZ,
					                            interpolateAmount, 
					                            ptrLoc->List_networkInterpCurStep);

	  
 	  //POSITION
	  ptrLoc->List_baseFrame->SetPosition(scene,newLocation.x,newLocation.y,newLocation.z);
	  //ORIENTATION
	  interpolateAmount = 12;//change interp for turning
	  if(ptrLoc->List_networkInterpCurStepOri > interpolateAmount)
	     ptrLoc->List_networkInterpCurStepOri = interpolateAmount;

       D3DVECTOR NewDirVect = CABD3DLIB->InterpolatePoints(
		                                        ptrLoc->List_networkLastDirVect, 
						                        ptrLoc->List_networkTargetDirVect,
					                            interpolateAmount, 
					                            ptrLoc->List_networkInterpCurStepOri);
	  D3DVECTOR NewUpVect = CABD3DLIB->InterpolatePoints(
		                                        ptrLoc->List_networkLastUpVect, 
						                        ptrLoc->List_networkTargetUpVect,
					                            interpolateAmount, 
					                            ptrLoc->List_networkInterpCurStepOri);
	  
	  
	  ptrLoc->List_baseFrame->SetOrientation(scene,
                                                 NewDirVect.x,
                                                 NewDirVect.y,
                                                 NewDirVect.z,
                                                 NewUpVect.x,
                                                 NewUpVect.y,
                                                 NewUpVect.z
                                                 );


      //set free look joint stuff   mainly for mechas type
	  float matrixToastTemp[4][4];
	  if(ptrLoc->List_movementDB->List_freeLookRefFrame){
      D3DVECTOR posLast,posTarget;
      posLast.x = ptrLoc->List_networkFreeLookCurPitchLast;
	  posLast.y = ptrLoc->List_networkFreeLookCurYawLast;
      posLast.z = 0.0f;
	  posTarget.x = ptrLoc->List_networkFreeLookCurPitchTarget;
	  posTarget.y = ptrLoc->List_networkFreeLookCurYawTarget;
      posTarget.z = 0.0f;

      D3DVECTOR NewVects = CABD3DLIB->InterpolatePoints(posLast, 
						                                posTarget,
					                                    interpolateAmount, 
					                                    ptrLoc->List_networkInterpCurStepOri);

      ptrLoc->List_movementDB->List_freeLookRefFrame->GetPosition(ptrLoc->List_baseFrame,&posLast);
	  Rotate (NewVects.x,0.0f,0.0f,matrixToastTemp);
	  ptrLoc->List_movementDB->List_freeLookRefFrame->AddTransform(D3DRMCOMBINE_REPLACE,matrixToastTemp); 
      Rotate (0.0f,NewVects.y,0.0f,matrixToastTemp);
	  ptrLoc->List_movementDB->List_freeLookRefFrame->AddTransform(D3DRMCOMBINE_AFTER,matrixToastTemp); 
	  ptrLoc->List_movementDB->List_freeLookRefFrame->SetPosition(ptrLoc->List_baseFrame,posLast.x,posLast.y,posLast.z);
	  }
      //end set free look joint stuff   mainly for mechas type
      ptrLoc->List_networkInterpCurStep++;
	  ptrLoc->List_networkInterpCurStepOri++;
      //ptrLoc->List_baseFrame->AddTransform(D3DRMCOMBINE_REPLACE,tempMatrixLoc);
   
   }
  }
}