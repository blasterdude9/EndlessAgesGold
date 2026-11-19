#include "StdAfx.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "CameraAdvClass.h"
#include "CMovementModel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCameraAdvObj::CCameraAdvObj(void)
{
	//vars
    m_followTrace		= 0;//entity trace
	m_boneNameTrace		= "Bip01 Head";//bone on entity target
	m_autoSpeed			= .02f;//speed of camera auto correction etc
	m_pitchMax          = 80.0f;
	m_pitchMin          = -75.0f;
	m_yawMax            = 360.0f;
	m_yawMin            = -360.0f;
    m_zoomMax			= 40.0f;
	m_zoomMin			= 0.1f;
	m_curPitch			= 20.0f;
	m_orbitPitchOffset  = 0.0f;
	m_curYaw			= 0.0f;
	m_curZoom			= m_zoomMax*.5f;
	m_lastUncollidedZoom= -1.0f;
    m_cameraSize		= 1.0f;
	m_followMode		= 2;//0 = auto correct 1=manual mode
	//kick dynamics
	m_curKickX			= 0.0f;
	m_curKickY			= 0.0f;
	m_curBurstRate		= 1.0f;
	m_curBurstRateY		= 0.0f;
	m_kickBurstDet		= .95f;
	m_kickReturnRate	= .9f;
	m_clipPlane			= 60000.0f;
	m_hieghtAspect		= 1.0f;
	m_reCorrectY		= 0.0f;
	m_rightClickControl = TRUE;
	m_overrideFlag		= FALSE;
	m_overShoulderOffsetX = .3f;
	m_overShoulderOffsetY = 5.0f;
	m_reduceYAdjust			= .9f;
	m_switchStamp       = GetTickCount();
}

CCameraAdvObj::~CCameraAdvObj(void)
{
}

void CCameraAdvObj::Clone(CCameraAdvObj ** clone)
{
    CCameraAdvObj * newPtr = new CCameraAdvObj();
	//vars
    newPtr->m_followTrace		= m_followTrace;
	newPtr->m_boneNameTrace		= m_boneNameTrace;
	newPtr->m_autoSpeed			= m_autoSpeed;
	newPtr->m_pitchMax          = m_pitchMax;
	newPtr->m_pitchMin          = m_pitchMin;
	newPtr->m_yawMax            = m_yawMax;
	newPtr->m_yawMin            = m_yawMin;
    newPtr->m_zoomMax			= m_zoomMax;
	newPtr->m_zoomMin			= m_zoomMin;
	newPtr->m_curPitch			= m_curPitch;
	newPtr->m_orbitPitchOffset  = m_orbitPitchOffset;
	newPtr->m_curYaw			= m_curYaw;
	newPtr->m_curZoom			= m_curZoom;
    newPtr->m_cameraSize		= m_cameraSize;
	newPtr->m_followMode		= m_followMode;
	newPtr->m_clipPlane			= m_clipPlane;
	//kick dynamics
	newPtr->m_curKickX			= m_curKickX;
	newPtr->m_curKickY			= m_curKickY;
	newPtr->m_curBurstRate		= m_curBurstRate;
	newPtr->m_curBurstRateY		= m_curBurstRateY;
	newPtr->m_kickBurstDet		= m_kickBurstDet;
	newPtr->m_kickReturnRate	= m_kickReturnRate;
	newPtr->m_rightClickControl	= m_rightClickControl;

	*clone = newPtr;
}

bool CCameraAdvObj::ApplyMouseRotation(float X, float Y, float mouseWheel)
{
    m_curYaw		   += X;
	m_orbitPitchOffset += Y;
	m_curPitch		   += Y;
	m_curZoom		   += mouseWheel;

	while(m_curYaw > 180.0f)
	   m_curYaw -= 360.0f;

	while(m_curYaw < -180.0f)
	   m_curYaw += 360.0f;

	if(m_orbitPitchOffset > 75.0f)
	   m_orbitPitchOffset = 75.0f;

	if(m_orbitPitchOffset < -75.0f)
	   m_orbitPitchOffset = -75.0f;

	if(m_curZoom > m_zoomMax)
	   m_curZoom = m_zoomMax;

	if(m_curZoom < m_zoomMin)
	   m_curZoom = m_zoomMin;

	if(m_curPitch > m_pitchMax)
	   m_curPitch = m_pitchMax;

	if(m_curPitch < m_pitchMin)
	   m_curPitch = m_pitchMin;

	return true;
}

bool CCameraAdvObj::Update(float & timeAdjust,void * runtimeEntityMem, MatrixARB * matLib)
{
	 if(!runtimeEntityMem)
	     return false;

	 if(m_followTrace < 0)
		 return false;

	 CMovementObj * mptr = (CMovementObj *) runtimeEntityMem;
	 if(!mptr)
		 return false;



	 if(!mptr->List_skeletalObject)
		 return false;

	 switch(m_followMode)
	 {
		case 0://fixed mode

		break;
		case 1://follow mode

		break;
		case 2://over the shoulder fps 3rd person mix
		{
			float perc = matLib->InterpolateFloatsHiRes(0.0f,1.0f,m_zoomMax,m_curZoom);
			float shoulderScale = cosf(m_curYaw * 0.0174532925f);

            //generate transform
            D3DVECTOR collisionFocus;
			collisionFocus.x	= mptr->List_baseFrame->List_frameMatrix._41;
			collisionFocus.y	= (mptr->List_baseFrame->List_frameMatrix._42+mptr->List_skeletalObject->m_box.maxY);
			collisionFocus.z	= mptr->List_baseFrame->List_frameMatrix._43;

            matLib->InitMat(m_currentMatrix.m);
			m_currentTargetPos.x	= mptr->List_baseFrame->List_frameMatrix._41+((mptr->List_skeletalObject->m_box.maxX*m_overShoulderOffsetX*shoulderScale)*perc);
			m_currentTargetPos.y	= (mptr->List_baseFrame->List_frameMatrix._42+mptr->List_skeletalObject->m_box.maxY+(m_overShoulderOffsetY*perc))*m_reduceYAdjust;
			m_currentTargetPos.z	= mptr->List_baseFrame->List_frameMatrix._43;
			if(!mptr->m_firstPersonMode)
			{//third person logic
				m_curPitch = -mptr->List_skeletalObject->m_freeLookPitch + m_orbitPitchOffset;
				if(m_curPitch > 80.0f) m_curPitch = 80.0f;
				if(m_curPitch < -75.0f) m_curPitch = -75.0f;
				matLib->InitMat(m_tempMatrix.m);
				m_tempMatrix._43 = -m_curZoom;
				matLib->fastMatrixMultiply(m_currentMatrix,m_tempMatrix,m_currentMatrix);
				//pitch rotation application
				matLib->RotateX(m_curPitch,m_tempMatrix.m);//do the pitch first
				matLib->fastMatrixMultiply(m_currentMatrix,m_currentMatrix,m_tempMatrix);
				//yaw rotation application
				matLib->RotateY(180.0f + m_curYaw,m_tempMatrix.m);//do the yaw second (360-degree free orbit)
				matLib->fastMatrixMultiply(m_currentMatrix,m_currentMatrix,m_tempMatrix);
				//hieght
				m_currentMatrix._41 += -((mptr->List_skeletalObject->m_box.maxX*m_overShoulderOffsetX*shoulderScale)*perc);
				m_currentMatrix._42 += (mptr->List_skeletalObject->m_box.maxY+(m_overShoulderOffsetY*perc))*m_reduceYAdjust;
				matLib->fastMatrixMultiply(m_currentMatrix,m_currentMatrix,mptr->List_baseFrame->List_frameMatrix);

				float dist = 0.0f;
				if(mptr->m_collisionMovRefPtr)
				{//valid collision
					if(CheckCollision(collisionFocus,//collisionFocus,//m_currentTargetPos,
								      dist,
								      matLib,
								      runtimeEntityMem)==TRUE)
					{
                        if(m_lastUncollidedZoom == -1.0f)
						   m_lastUncollidedZoom = m_curZoom;

						m_curZoom -= (dist-.01f);
						if(m_curZoom > m_zoomMax)
						   m_curZoom = m_zoomMax;

	                    if(m_curZoom < m_zoomMin)
	                       m_curZoom = m_zoomMin;
					}
					else if(m_lastUncollidedZoom != -1.0f)
					{//return
						m_curZoom = m_lastUncollidedZoom;
						m_lastUncollidedZoom = -1.0f;
						//reset
					    matLib->InitMat(m_currentMatrix.m);
						//offset
						m_curPitch = -mptr->List_skeletalObject->m_freeLookPitch + m_orbitPitchOffset;
						if(m_curPitch > 80.0f) m_curPitch = 80.0f;
						if(m_curPitch < -75.0f) m_curPitch = -75.0f;
						matLib->InitMat(m_tempMatrix.m);
						m_tempMatrix._43 = -m_curZoom;
						matLib->fastMatrixMultiply(m_currentMatrix,m_tempMatrix,m_currentMatrix);
						//pitch rotation application
						matLib->RotateX(m_curPitch,m_tempMatrix.m);//do the pitch first
						matLib->fastMatrixMultiply(m_currentMatrix,m_currentMatrix,m_tempMatrix);
						//yaw rotation application
						matLib->RotateY(180.0f + m_curYaw,m_tempMatrix.m);//do the yaw second
						matLib->fastMatrixMultiply(m_currentMatrix,m_currentMatrix,m_tempMatrix);
						//hieght
						m_currentMatrix._41 += -((mptr->List_skeletalObject->m_box.maxX*m_overShoulderOffsetX*shoulderScale)*perc);
						m_currentMatrix._42 += (mptr->List_skeletalObject->m_box.maxY+(m_overShoulderOffsetY*perc))*m_reduceYAdjust;
						matLib->fastMatrixMultiply(m_currentMatrix,m_currentMatrix,mptr->List_baseFrame->List_frameMatrix);

						float dist = 0.0f;
						if(CheckCollision(collisionFocus,//collisionFocus,//m_currentTargetPos,
								          dist,
								          matLib,
								          runtimeEntityMem)==TRUE)
						{
                            if(m_lastUncollidedZoom == -1.0f)
						       m_lastUncollidedZoom = m_curZoom;

						       m_curZoom -= (dist-.03f);
						    if(m_curZoom > m_zoomMax)
						       m_curZoom = m_zoomMax;

	                        if(m_curZoom < m_zoomMin)
	                           m_curZoom = m_zoomMin;
					    }
					    else if(m_lastUncollidedZoom != -1.0f)
					    {//return
                           m_curZoom = m_lastUncollidedZoom;
                           m_lastUncollidedZoom = -1.0f;
					    }//end return
					}//end return
				}//end valid collision

                //handle final calculations
				matLib->InitMat(m_currentMatrix.m);
				//offset
				m_curPitch = -mptr->List_skeletalObject->m_freeLookPitch + m_orbitPitchOffset;
				if(m_curPitch > 80.0f) m_curPitch = 80.0f;
				if(m_curPitch < -75.0f) m_curPitch = -75.0f;
				matLib->InitMat(m_tempMatrix.m);
				m_tempMatrix._43 = -m_curZoom;
				matLib->fastMatrixMultiply(m_currentMatrix,m_tempMatrix,m_currentMatrix);
				//pitch rotation application
				matLib->RotateX(m_curPitch,m_tempMatrix.m);//do the pitch first
				matLib->fastMatrixMultiply(m_currentMatrix,m_currentMatrix,m_tempMatrix);
				//yaw rotation application
				matLib->RotateY(180.0f + m_curYaw,m_tempMatrix.m);//do the yaw second
				matLib->fastMatrixMultiply(m_currentMatrix,m_currentMatrix,m_tempMatrix);
				//hieght
				m_currentMatrix._41 += -((mptr->List_skeletalObject->m_box.maxX*m_overShoulderOffsetX*shoulderScale)*perc);
				m_currentMatrix._42 += (mptr->List_skeletalObject->m_box.maxY+(m_overShoulderOffsetY*perc))*m_reduceYAdjust;
				matLib->fastMatrixMultiply(m_currentMatrix,m_currentMatrix,mptr->List_baseFrame->List_frameMatrix);

				if(m_reCorrectY > 0.0f && abs(m_curPitch) < 85)
				{//realignment
				   float decay = 1.0f - (0.08f * timeAdjust);
				   if(decay < 0.0f) decay = 0.0f;
				   m_curYaw *= decay;
				   m_orbitPitchOffset *= decay;
				   if((float)fabs(m_curYaw) < 0.2f) m_curYaw = 0.0f;
				   if((float)fabs(m_orbitPitchOffset) < 0.2f) m_orbitPitchOffset = 0.0f;
				}//end realignment
                //override character translucency
				mptr->m_renderBodyFilter = 0;
                if(fabs(m_curZoom) > 1.8f && fabs(m_curPitch) < 78)
				{
				   //if(m_overrideFlag)
				      mptr->ResetMaterial();

				   m_overrideFlag = FALSE;
				}
				else
				{
				   //if(!m_overrideFlag)
					if(m_curZoom != m_zoomMin)
                       mptr->SetOverrideTrans(.1f);
					else
					{
					   mptr->m_renderBodyFilter = 1;
					   mptr->SetOverrideTrans(0.0f);
					}

				   m_overrideFlag = TRUE;
				}

			}//end third person logic
			else
			{//first person conversion
				CBoneObject * bone = NULL;
				mptr->List_baseFrame->GetTransform(NULL,&m_currentMatrix);
                if(mptr->List_skeletalObject->GetBoneByName(m_boneNameTrace,&bone))
				{
				   matLib->InitMat(m_tempMatrix.m);
				   m_tempMatrix._42 = bone->m_boneFrame->List_frameMatrix._42;
				   mptr->m_hardPointOverdY = m_tempMatrix._42;
				   m_tempMatrix._43 += .9f;
                   matLib->fastMatrixMultiply(m_currentMatrix,m_tempMatrix,m_currentMatrix);
				}
				else 
				{
				   matLib->InitMat(m_tempMatrix.m);
                   m_tempMatrix._42 = mptr->List_skeletalObject->m_box.maxY;
				   mptr->m_hardPointOverdY = m_tempMatrix._42;
				   m_tempMatrix._43 += .9f;
                   matLib->fastMatrixMultiply(m_currentMatrix,m_tempMatrix,m_currentMatrix);
				}
				

				
				matLib->RotateX(mptr->List_movementDB->List_freeLookCurPitch,m_tempMatrix.m);//do the yaw second
				matLib->fastMatrixMultiply(m_currentMatrix,m_tempMatrix,m_currentMatrix);

				matLib->RotateY(180,m_tempMatrix.m);//do the yaw second
				matLib->fastMatrixMultiply(m_currentMatrix,m_tempMatrix,m_currentMatrix);

				//update dir vect
				ZeroMemory(&m_dirVect,sizeof(D3DVECTOR));
				m_dirVect.z = 1.0f;
				m_dirVect = matLib->UpdatePolygonVertexRotationOnly(m_currentMatrix.m,m_dirVect);
				
			}//end first person conversion
		}
		break;
		case 3://manual mode-set and lock
            //generate transform
            matLib->InitMat(m_currentMatrix.m);
			m_currentTargetPos.x	= mptr->List_baseFrame->List_frameMatrix._41;
			m_currentTargetPos.y	= mptr->List_baseFrame->List_frameMatrix._42;
			m_currentTargetPos.z	= mptr->List_baseFrame->List_frameMatrix._43;
			if(!mptr->m_firstPersonMode)
			{//third person logic
				//hieght
				m_currentTargetPos.y += mptr->List_skeletalObject->m_box.maxY;
				//offset
				matLib->InitMat(m_tempMatrix.m);
				m_tempMatrix._43 = -m_curZoom;
				matLib->fastMatrixMultiply(m_currentMatrix,m_tempMatrix,m_currentMatrix);
				//pitch rotation application
				matLib->RotateX(m_curPitch,m_tempMatrix.m);//do the pitch first
				matLib->fastMatrixMultiply(m_currentMatrix,m_currentMatrix,m_tempMatrix);
				//yaw rotation application
				matLib->RotateY(m_curYaw-180,m_tempMatrix.m);//do the yaw second
				matLib->fastMatrixMultiply(m_currentMatrix,m_currentMatrix,m_tempMatrix);
				//offset to location
				m_currentMatrix._41		+= m_currentTargetPos.x;
				m_currentMatrix._42		+= m_currentTargetPos.y;
				m_currentMatrix._43		+= m_currentTargetPos.z;

				float dist = 0.0f;
				if(mptr->m_collisionMovRefPtr)
				{//valid collision
					if(CheckCollision(m_currentTargetPos, 
								      dist, 
								      matLib,
								      runtimeEntityMem)==TRUE)
					{
                        if(m_lastUncollidedZoom == -1.0f)
						   m_lastUncollidedZoom = m_curZoom;

						m_curZoom -= (dist-.01f);
						if(m_curZoom > m_zoomMax)
						   m_curZoom = m_zoomMax;

	                    if(m_curZoom < m_zoomMin)
	                       m_curZoom = m_zoomMin;	
					}
					else if(m_lastUncollidedZoom != -1.0f)
					{//return
                       m_curZoom = m_lastUncollidedZoom;
                       m_lastUncollidedZoom = -1.0f;
					   //reset
					   matLib->InitMat(m_currentMatrix.m);
						//offset
						matLib->InitMat(m_tempMatrix.m);
						m_tempMatrix._43 = -m_curZoom;
						matLib->fastMatrixMultiply(m_currentMatrix,m_tempMatrix,m_currentMatrix);
						//pitch rotation application
						matLib->RotateX(m_curPitch,m_tempMatrix.m);//do the pitch first
						matLib->fastMatrixMultiply(m_currentMatrix,m_currentMatrix,m_tempMatrix);
						//yaw rotation application
						matLib->RotateY(m_curYaw-180,m_tempMatrix.m);//do the yaw second
						matLib->fastMatrixMultiply(m_currentMatrix,m_currentMatrix,m_tempMatrix);
						//offset to location
						m_currentMatrix._41		+= m_currentTargetPos.x;
						m_currentMatrix._42		+= m_currentTargetPos.y;
						m_currentMatrix._43		+= m_currentTargetPos.z;
                        float dist = 0.0f;
						if(CheckCollision(m_currentTargetPos, 
								          dist, 
								          matLib,
								          runtimeEntityMem)==TRUE)
						{
                            if(m_lastUncollidedZoom == -1.0f)
						       m_lastUncollidedZoom = m_curZoom;

						       m_curZoom -= (dist-.03f);
						    if(m_curZoom > m_zoomMax)
						       m_curZoom = m_zoomMax;

	                        if(m_curZoom < m_zoomMin)
	                           m_curZoom = m_zoomMin;	
					    }
					    else if(m_lastUncollidedZoom != -1.0f)
					    {//return
                           m_curZoom = m_lastUncollidedZoom;
                           m_lastUncollidedZoom = -1.0f;
					    }//end return
					}//end return
				}//end valid collision

                //handle final calculations
				matLib->InitMat(m_currentMatrix.m);//init
				//offset
				matLib->InitMat(m_tempMatrix.m);
				m_tempMatrix._43 = -m_curZoom;
				matLib->fastMatrixMultiply(m_currentMatrix,m_tempMatrix,m_currentMatrix);
				//pitch rotation application
				matLib->RotateX(m_curPitch,m_tempMatrix.m);//do the pitch first
				matLib->fastMatrixMultiply(m_currentMatrix,m_currentMatrix,m_tempMatrix);
				//yaw rotation application
				matLib->RotateY(m_curYaw-180,m_tempMatrix.m);//do the yaw second
				matLib->fastMatrixMultiply(m_currentMatrix,m_currentMatrix,m_tempMatrix);
				//offset to location
				m_currentMatrix._41		+= m_currentTargetPos.x;
				m_currentMatrix._42		+= m_currentTargetPos.y;
				m_currentMatrix._43		+= m_currentTargetPos.z;

				if(m_reCorrectY > 0.0f && abs(m_curPitch) < 85)
				{//realignment
				  D3DVECTOR inSpacePoint,baseDir,camDir;
				  ZeroMemory(&baseDir,sizeof(D3DVECTOR));
                  baseDir.z = 1.0f;
				  baseDir = matLib->UpdatePolygonVertex(mptr->List_baseFrame->List_frameMatrix.m,baseDir);
				  
                  //determine negative or positive
                  
                   inSpacePoint = matLib->GetLocalPositionFromMatrixView(m_currentMatrix,baseDir);
                   
				   ZeroMemory(&baseDir,sizeof(D3DVECTOR));
                   baseDir.z = 1.0f;
				   baseDir = matLib->UpdatePolygonVertexRotationOnly(mptr->List_baseFrame->List_frameMatrix.m,baseDir);
				   ZeroMemory(&camDir,sizeof(D3DVECTOR));
                   camDir.z = 1.0f;
				   camDir = matLib->UpdatePolygonVertexRotationOnly(m_currentMatrix.m,camDir);
				   float angleDiff = matLib->GetAngleBetweenTwoVectors(baseDir,camDir);
				   float application = (1.0f-(angleDiff/180.0f)) * m_reCorrectY;
				   application = (1.0f-(angleDiff/180.0f)) * application;
				   
				   if(inSpacePoint.x < -.03)
				   {
                      m_curYaw += application*timeAdjust;
				   }
				   else if(inSpacePoint.x > .03f)
				   {
                      m_curYaw -= application*timeAdjust;
				   }
				}//end realignment
			}//end third person logic
			else
			{//first person conversion
				CBoneObject * bone = NULL;
				mptr->List_baseFrame->GetTransform(NULL,&m_currentMatrix);
                if(mptr->List_skeletalObject->GetBoneByName(m_boneNameTrace,&bone))
				{
				   matLib->InitMat(m_tempMatrix.m);
				   m_tempMatrix._42 = bone->m_boneFrame->List_frameMatrix._42;
				   mptr->m_hardPointOverdY = m_tempMatrix._42;
				   m_tempMatrix._43 += .9f;
                   matLib->fastMatrixMultiply(m_currentMatrix,m_tempMatrix,m_currentMatrix);
				}
				else 
				{
				   matLib->InitMat(m_tempMatrix.m);
                   m_tempMatrix._42 = mptr->List_skeletalObject->m_box.maxY;
				   mptr->m_hardPointOverdY = m_tempMatrix._42;
				   m_tempMatrix._43 += .9f;
                   matLib->fastMatrixMultiply(m_currentMatrix,m_tempMatrix,m_currentMatrix);
				}
				

				
				matLib->RotateX(mptr->List_movementDB->List_freeLookCurPitch,m_tempMatrix.m);//do the yaw second
				matLib->fastMatrixMultiply(m_currentMatrix,m_tempMatrix,m_currentMatrix);

				matLib->RotateY(180,m_tempMatrix.m);//do the yaw second
				matLib->fastMatrixMultiply(m_currentMatrix,m_tempMatrix,m_currentMatrix);

				//update dir vect
				ZeroMemory(&m_dirVect,sizeof(D3DVECTOR));
				m_dirVect.z = 1.0f;
				m_dirVect = matLib->UpdatePolygonVertexRotationOnly(m_currentMatrix.m,m_dirVect);
				
			}//end first person conversion

			
		break;
	 }
	 return true;
}

BOOL CCameraAdvObj::CheckCollision(D3DVECTOR & startPoint, 
								   float & dist, 
								   MatrixARB * matLib,
								   void * runtimeEntityMem)
{

    CMovementObj * mptr = (CMovementObj *) runtimeEntityMem;

	byte temp = 0;
	float tempDist = 0.0f;
	BOOL collided = FALSE;
	D3DVECTOR endPoint,latestNormal,intersect,startPointTemp;
    float rotateCurrent = 0.0f;
	int indexOfObject = -1;
	D3DMATRIX startMatrix = m_currentMatrix;
	startMatrix._41 = startPoint.x;
	startMatrix._42 = startPoint.y;
	startMatrix._43 = startPoint.z;
	
	for(int qLoop=0;qLoop < 4;qLoop++)
	{

        ZeroMemory(&endPoint,sizeof(D3DVECTOR));
		ZeroMemory(&startPointTemp,sizeof(D3DVECTOR));
	    if(qLoop == 0)
		{
		   endPoint.x = m_cameraSize;
		   startPointTemp.x = m_cameraSize;
		}
		else if(qLoop == 1)
		{
           endPoint.x = -m_cameraSize;
		   startPointTemp.x = -m_cameraSize;
		}
		else if(qLoop == 2)
		{
           endPoint.y = m_cameraSize;
		   startPointTemp.y = m_cameraSize;
		}
		else if(qLoop == 3)
		{
		   endPoint.y = -m_cameraSize;
		   startPointTemp.y = -m_cameraSize;
		}
	    
		endPoint = matLib->UpdatePolygonVertex(m_currentMatrix.m,endPoint);
		startPointTemp = matLib->UpdatePolygonVertex(startMatrix.m,startPointTemp);

		if(mptr->m_collisionMovRefPtr->GetClosesIntersectionUseSeg(
																				startPointTemp,
																				endPoint,
																				startPointTemp,
																				endPoint,
																				&intersect.x,
																				&intersect.y,
																				&intersect.z,
																				&indexOfObject,
																				&latestNormal,
																				true,
																				tempDist,
																				temp
																				)==TRUE)
		{//
		  if(tempDist > dist)
			 dist = tempDist;

		  collided = TRUE;
		}
		rotateCurrent += 90.0f;
	}

	return collided;
}

D3DMATRIX CCameraAdvObj::GetCameraMatrix()
{
    return m_currentMatrix;
}
	

D3DMATRIX CCameraAdvObj::GetInvCameraMatrix(MatrixARB * matLib)
{
   matLib->D3DMath_MatrixInvert(m_tempMatrix,m_currentMatrix);
   return m_tempMatrix;
}

void CCameraAdvObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
      
       ar << m_name
		  << m_boneNameTrace
		  << m_autoSpeed
		  << m_pitchMax
		  << m_pitchMin
		  << m_yawMax
		  << m_yawMin
		  << m_zoomMax
		  << m_zoomMin
		  << m_cameraSize
		  << m_followMode
		  << m_curKickX
		  << m_curKickY
		  << m_curBurstRate
		  << m_curBurstRateY
		  << m_kickBurstDet
		  << m_kickReturnRate
		  << m_clipPlane;
	}
	else
	{
       ar >> m_name
		  >> m_boneNameTrace
		  >> m_autoSpeed
		  >> m_pitchMax
		  >> m_pitchMin
		  >> m_yawMax
		  >> m_yawMin
		  >> m_zoomMax
		  >> m_zoomMin
		  >> m_cameraSize
		  >> m_followMode
		  >> m_curKickX
		  >> m_curKickY
		  >> m_curBurstRate
		  >> m_curBurstRateY
		  >> m_kickBurstDet
		  >> m_kickReturnRate
		  >> m_clipPlane;
	}
}


IMPLEMENT_SERIAL(CCameraAdvObj, CObject, 0)
IMPLEMENT_SERIAL(CCameraAdvList, CObList, 0)
