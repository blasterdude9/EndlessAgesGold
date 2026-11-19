#include "stdafx.h"

#include <d3d9.h>
#include <afxtempl.h>
#include "CPhysicsEmuClass.h"
#include "collision_raysystem.h"
#include "CMenuTextBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-----------------------------------------------//
//-----------------------------------------------//
//------------Motion caps class------------------//
CMovementCaps::CMovementCaps()
{
m_pckSys_changeable				   = FALSE;//user can swicth between modes
m_pckSys_firstPersonMode           = 0;//0=realtime 1=point and click movement
m_pckSys_thirdPersonMode           = 0;//0=realtime 1=point and click movement
m_pckSys_aiControlModel            = 0;
m_lastGravityStatus                = TRUE;
List_mouseSensitivity              = .01f;
List_invertMouse                   = FALSE;
List_jointMouseFocus               = FALSE;
List_freeLookRefFrame              = NULL;
List_freeLookSpeed                 = .03f;
List_freeLookCurPitch              = 0.0f;
List_freeLookCurMaxPitch           = 60.0f;
List_freeLookCurYaw                = 0.0f;
List_freeLookCurMaxYaw             = 0.0f;
List_currentlyJumping              = FALSE;
List_jumpPower                     = 3.0f;
List_canWalk                       = FALSE;
List_doGravity                     = FALSE;
List_mouseRestriction              = 500;
List_slidingEnabled                = FALSE;
List_gravity                       = 0.0f;
List_curSpeed                      = 0.0f;
List_maxSpeed                      = 0.5f;
List_minSpeed                      = -.5f;
List_accellerate                   = .5f;
List_decellerate                   = .1f;                   
List_pitchAccel                    = .2f;
List_pitchDecel                    = .1f;
List_yawAccel                      = .2f; 
List_yawDecel                      = .1f;
List_curRotX                       = 0.0f;
List_curRotY                       = 0.0f;
List_curRotZ                       = 0.0f;
List_curTransX                     = 0.0f;
List_curTransY                     = 0.0f;
List_curTransZ                     = 0.0f;
List_rollFactor                    = .8f;
List_minYaw                        = .02f;
List_maxYaw                        = 3.0f;
List_minPitch                      = .02f;
List_maxPitch                      = .3f;
m_maxRoll                          = 0.0f;
m_rollAccel                        = 0.0f;
m_rollDecel                        = 0.0f;
m_floatWhenDead                    = FALSE;
List_letGoDecel                    = 0.0f;                   
m_burstForceY                      = 0.0f;
List_curRotAnimY                   = 0.0f;
m_autoLevelXStrength               = 0.0f;
m_autoLevelZStrength               = 0.0f;
m_autoLevelTolerance               = 0.0f;
m_autoLevelX                       = FALSE;
m_autoLevelZ                       = FALSE;
m_rotationDragConstant             = 0.9f;
m_rotationalGroundDrag             = 1.0f;
m_freeLookPichForce                = 0.0f;
m_submergedHieght                  = 0.0f;
m_inLiquidMotionModifier           = 0.0f;
m_inLiquid                         = FALSE;
m_antiGravity                      = FALSE;
List_mass                          = 100.0f;
m_curRotationVect.x                = 0.0f;
m_curRotationVect.y                = 0.0f;
m_curRotationVect.z                = 0.0f;
List_curVelocityVect.x             = 0.0f;
List_curVelocityVect.y             = 0.0f;
List_curVelocityVect.z             = 0.0f;
List_maxVelocity                   = 10.0f;
List_lastPosition.x                = 0.0f;
List_lastPosition.y                = 0.0f;
List_lastPosition.z                = 0.0f;
List_groundDrag                    = 0.99f;
List_autoDecel                     = TRUE;
List_autoDecelY                    = TRUE;                  
List_autoDecelZ                    = TRUE;
List_capabilityFovMax              = .91f;
List_capabilityFovMin              = .2f;
List_capFovChangeSensitivity       = .1f;  
List_targetFov                     = 0.0f; 
List_fovCurrentInterp              = 0;
List_lastFov                       = 0.0f;

m_frictionCoefficient              = 0.0f;
m_freeLookYawForce                  = 3.0f;
m_strafeReduction                  = 1.0f;
m_crossSectionalArea               = 1.0f;
m_displacedVolume                  = 1.0f;
m_airControl                       = 0.1f;
m_strafeReduction                  = 0.0f;
List_zeroToSpeed                   = 0.0001f;
List_crouched                      = FALSE;
m_timeStamp                        = 0;
m_jetPackInUse                     = FALSE;
m_yawIsUsed                        = FALSE;
m_pitchIsUsed                      = FALSE;
m_rollIsUsed                       = FALSE;
m_speedControlInUse                = FALSE;
m_sidestepsInUse                   = FALSE;	
m_rampTolerance                    = .5f;
m_grvtyChngTimeStmp                = GetTickCount();
m_currentSurfaceProp               = 0;

m_compoundCurX                     = 0.0f;
m_compoundCurY                     = 0.0f;

m_ladderLocked                     = FALSE;
m_ladderLockedDir.x                = 0.0f;
m_ladderLockedDir.y                = 0.0f;
m_ladderLockedDir.z                = 0.0f;

m_jumpRecoverTime                  = 0;
m_jumpRecoveryStamp                = GetTickCount();

m_useLockPoint                     = 0;//0=currently unlocked   1=currently locked 
ZeroMemory(&m_lastLock,sizeof(D3DMATRIX));
m_optFramePtr                      = NULL;
m_optLockTrace                     = -1;
m_angularvelocity                  = 0.0;
m_angleTemp                        = 0.0f;
m_inertia                          = 1500.0f;
m_corneringStiffnessF              = -5.0f;
m_corneringStiffnessR              = -5.2f;
m_maxGrip                          = 2.0f;
m_curSpeed                         = 0;

m_exponentTurning                  = false;

ZeroMemory(&m_velocity_wc,sizeof(D3DVECTOR));

ZeroMemory(&m_ladderBox,sizeof(ABBOX));
}
//-----------------------------------------------//
//-----------------------------------------------//
//------------Motion caps class------------------//
BOOL CMovementCaps::Clone(CMovementCaps ** clone)
{
CMovementCaps * midTermPtr = new CMovementCaps();
//Start Cloneing

midTermPtr->List_mouseSensitivity         = List_mouseSensitivity;
midTermPtr->List_invertMouse              = List_invertMouse;
midTermPtr->List_jointMouseFocus          = List_jointMouseFocus;
midTermPtr->List_freeLookRefFrame         = List_freeLookRefFrame;
midTermPtr->List_freeLookSpeed            = List_freeLookSpeed;
midTermPtr->List_freeLookCurPitch         = List_freeLookCurPitch;
midTermPtr->List_freeLookCurMaxPitch      = List_freeLookCurMaxPitch;
midTermPtr->List_freeLookCurYaw           = List_freeLookCurYaw;
midTermPtr->List_freeLookCurMaxYaw        = List_freeLookCurMaxYaw;
midTermPtr->List_currentlyJumping         = List_currentlyJumping;
midTermPtr->List_jumpPower                = List_jumpPower;
midTermPtr->List_canWalk                  = List_canWalk;
midTermPtr->List_doGravity                = List_doGravity;
midTermPtr->List_mouseRestriction         = List_mouseRestriction;
midTermPtr->List_slidingEnabled           = List_slidingEnabled;
midTermPtr->List_gravity                  = List_gravity;
midTermPtr->List_autoDecel                = List_autoDecel;
midTermPtr->List_curSpeed                 = List_curSpeed;
midTermPtr->List_maxSpeed                 = List_maxSpeed;
midTermPtr->List_minSpeed                 = List_minSpeed;
midTermPtr->List_accellerate              = List_accellerate;
midTermPtr->List_decellerate              = List_decellerate;
midTermPtr->List_pitchAccel               = List_pitchAccel;
midTermPtr->List_pitchDecel               = List_pitchDecel;
midTermPtr->List_yawAccel                 = List_yawAccel;
midTermPtr->List_yawDecel                 = List_yawDecel;
midTermPtr->List_curRotX                  = List_curRotX;
midTermPtr->List_curRotY                  = List_curRotY;
midTermPtr->List_curRotZ                  = List_curRotZ;
midTermPtr->List_curTransX                = List_curTransX;
midTermPtr->List_curTransY                = List_curTransY;
midTermPtr->List_curTransZ                = List_curTransZ;
midTermPtr->List_rollFactor               = List_rollFactor;
midTermPtr->List_minYaw                   = List_minYaw;
midTermPtr->List_maxYaw                   = List_maxYaw;
midTermPtr->List_minPitch                 = List_minPitch;
midTermPtr->List_maxPitch                 = List_maxPitch;
midTermPtr->m_maxRoll                     = m_maxRoll;
midTermPtr->m_rollAccel                   = m_rollAccel;
midTermPtr->m_rollDecel                   = m_rollDecel;
midTermPtr->m_floatWhenDead               = m_floatWhenDead;
midTermPtr->List_letGoDecel               = List_letGoDecel;
midTermPtr->m_burstForceY                 = m_burstForceY;
midTermPtr->List_curRotAnimY              = List_curRotAnimY;
midTermPtr->m_autoLevelXStrength          = m_autoLevelXStrength;
midTermPtr->m_autoLevelZStrength          = m_autoLevelZStrength;
midTermPtr->m_autoLevelTolerance          = m_autoLevelTolerance;
midTermPtr->m_autoLevelX                  = m_autoLevelX;
midTermPtr->m_autoLevelZ                  = m_autoLevelZ;
midTermPtr->m_rotationDragConstant        = m_rotationDragConstant;
midTermPtr->m_rotationalGroundDrag        = m_rotationalGroundDrag;
midTermPtr->m_freeLookPichForce           = m_freeLookPichForce;
midTermPtr->m_submergedHieght             = m_submergedHieght;
midTermPtr->m_inLiquidMotionModifier      = m_inLiquidMotionModifier;
midTermPtr->m_inLiquid                    = m_inLiquid;
midTermPtr->m_antiGravity                 = m_antiGravity;
midTermPtr->List_mass                     = List_mass;
midTermPtr->m_curRotationVect.x           = m_curRotationVect.x;
midTermPtr->m_curRotationVect.y           = m_curRotationVect.y;
midTermPtr->m_curRotationVect.z           = m_curRotationVect.z;
midTermPtr->List_curVelocityVect.x        = 0.0f;//List_curVelocityVect.x;
midTermPtr->List_curVelocityVect.y        = 0.0f;//List_curVelocityVect.y;
midTermPtr->List_curVelocityVect.z        = 0.0f;List_curVelocityVect.z;
midTermPtr->List_maxVelocity              = List_maxVelocity;
midTermPtr->List_lastPosition.x           = 0.0f;//List_lastPosition.x;
midTermPtr->List_lastPosition.y           = 0.0f;//List_lastPosition.y;
midTermPtr->List_lastPosition.z           = 0.0f;//List_lastPosition.z;
midTermPtr->List_groundDrag               = List_groundDrag;
midTermPtr->List_autoDecelY               = List_autoDecelY;                
midTermPtr->List_autoDecelZ               = List_autoDecelZ;
midTermPtr->List_capabilityFovMax         = List_capabilityFovMax;
midTermPtr->List_capabilityFovMin         = List_capabilityFovMin;
midTermPtr->List_capFovChangeSensitivity  = List_capFovChangeSensitivity;  
midTermPtr->List_targetFov                = List_targetFov;
midTermPtr->List_lastFov                  = 0.0f;
midTermPtr->List_fovCurrentInterp         = 0;
midTermPtr->List_crouched                 = FALSE;
midTermPtr->m_frictionCoefficient         = m_frictionCoefficient;
midTermPtr->m_freeLookYawForce            = m_freeLookYawForce;
midTermPtr->m_strafeReduction             = m_strafeReduction;
midTermPtr->m_crossSectionalArea          = m_crossSectionalArea;
midTermPtr->m_displacedVolume             = m_displacedVolume;
midTermPtr->m_airControl                  = m_airControl;
midTermPtr->m_strafeReduction             = m_strafeReduction;
midTermPtr->List_zeroToSpeed              = List_zeroToSpeed;
midTermPtr->m_rampTolerance               = m_rampTolerance;
midTermPtr->m_compoundCurX                = m_compoundCurX;
midTermPtr->m_compoundCurY                = m_compoundCurY;
midTermPtr->m_jumpRecoverTime             = m_jumpRecoverTime;
midTermPtr->m_inertia                     = m_inertia;
midTermPtr->m_corneringStiffnessF         = m_corneringStiffnessF;
midTermPtr->m_corneringStiffnessR         = m_corneringStiffnessR;
midTermPtr->m_maxGrip                     = m_maxGrip;
midTermPtr->m_pckSys_changeable			  = m_pckSys_changeable;//user can swicth between modes
midTermPtr->m_pckSys_firstPersonMode      = m_pckSys_firstPersonMode;//0=realtime 1=point and click movement
midTermPtr->m_pckSys_thirdPersonMode      = m_pckSys_thirdPersonMode;//0=realtime 1=point and click movement
midTermPtr->m_pckSys_aiControlModel		  = m_pckSys_aiControlModel;


*clone = midTermPtr;
return TRUE;
}

void CMovementCaps::SafeDelete()
{
	if(List_freeLookRefFrame){
       List_freeLookRefFrame->SafeDelete();
       delete List_freeLookRefFrame;
	   List_freeLookRefFrame=0;
	}


}
//-----------------------------------------------//
void CMovementCaps::SetUseStatesToFalse()
{
    m_yawIsUsed            = FALSE;
    m_pitchIsUsed          = FALSE;
    m_rollIsUsed           = FALSE;
    m_speedControlInUse    = FALSE;
    m_sidestepsInUse       = FALSE;		
}
//-----------------------------------------------//
float CMovementCaps::GetLookPitch()
{
      return (List_freeLookCurPitch + m_compoundCurX);
}
//-----------------------------------------------//
float CMovementCaps::GetLookYaw()
{
      return (List_freeLookCurYaw + m_compoundCurY);
}
//--------------Physics base----------------------------------------------------------//
void CMovementCaps::DoPhysics(  double timeAdjust,
								CFrameObj * framePtr,
								CMovementCaps * physicsDB,
								double AdditionalForceX,
								double AdditionalForceY,
								double AdditionalForceZ,
								double ExternalForceX,
								double ExternalForceY,
								double ExternalForceZ,
								double doYaw,
								double doPitch,
								BOOL rollIsUsed,
								BOOL pitchIsUsed,
								BOOL yawIsUsed,
								double submergedValue,
								double liquidViscusModifier,
								double environmentGravity,
								double lastTimeAdjust,
								void * raySystem,
								void * textSysVoid)
{
	CCollision_RaySystem *raySys = (CCollision_RaySystem*)raySystem;
	CMenuTextBox *textSys = (CMenuTextBox*)textSysVoid;
	D3DVECTOR lastPosition;
	framePtr->GetPosition(NULL,&lastPosition);

	if(physicsDB->m_ladderLocked == TRUE)
	{
		//verify angle
		D3DVECTOR negTempXYZ;
		negTempXYZ.x = 0.0f;
		negTempXYZ.y = 0.0f;
		negTempXYZ.z = 1.0f;
		D3DMATRIX matrixNegTemp;
		framePtr->GetTransform(NULL,&matrixNegTemp);
		negTempXYZ = m_matLib.UpdatePolygonVertexRotationOnly(matrixNegTemp.m,negTempXYZ);
		double xAngle = m_matLib.GetDifferenceAngleY(negTempXYZ,physicsDB->m_ladderLockedDir);
		   
		if(fabs(xAngle) > 45)
			physicsDB->m_ladderLocked = FALSE;
		   
		   
		D3DVECTOR lockedPos;
		   
		lockedPos.x = physicsDB->m_ladderBox.maxX;
		lockedPos.y = lastPosition.y;
		lockedPos.z = physicsDB->m_ladderBox.maxZ;

		double distanceTemp = m_matLib.MagnitudeSquared(lockedPos.x,lockedPos.y,lockedPos.z,lastPosition.x,lockedPos.y,lastPosition.z);
		if(distanceTemp > 1.0f)
		{
			physicsDB->m_ladderLocked = FALSE; 
		}

		if(lastPosition.y > physicsDB->m_ladderBox.maxY || 
			lastPosition.y < physicsDB->m_ladderBox.minY)
		{
			physicsDB->m_ladderLocked = FALSE;
		}

		if(physicsDB->m_ladderLocked)
			physicsDB->List_doGravity = FALSE;
	}



	D3DVECTOR orig;
	framePtr->GetPosition(NULL,&orig);
	//IMPLEMENTATION OF PHYSICS

	double characterHieght                          = physicsDB->m_submergedHieght;//possibly derived collision radius 
	//double submergeValue                            = 0.0f;//orig.y - EnvironmentPtr->m_seaLevel;
	//double submergeValue                            = orig.y - EnvironmentPtr->m_seaLevel;
	double viscusDrag                               = liquidViscusModifier;
	physicsDB->m_inLiquid                          = FALSE;

	double m_waterMovementModify = physicsDB->m_inLiquidMotionModifier;//0.0-2.0

	//if(inWater){//below sea level
	if(submergedValue < 0.0f){//below sea level
		if(submergedValue <= (-characterHieght*.5f))
		{
		physicsDB->m_inLiquid = TRUE;
		physicsDB->List_currentlyJumping = FALSE;
		}
		
		if(submergedValue <= -characterHieght){//completely submerged
		double buoyantForce = physicsDB->List_mass * .001f;
		ExternalForceY = ExternalForceY + buoyantForce;
		}//end completely submerged
		else{//else partialy submerged
		double multiplier = fabs(submergedValue)/characterHieght;
		double buoyantForce = (physicsDB->List_mass*multiplier) * .001f;
		ExternalForceY = ExternalForceY + buoyantForce;
		}//end partialy submerged
	}//end below sea level

	double forceStructX,forceStructY,forceStructZ;

	double EnviormentModifier = 1.0f;
	if(physicsDB->m_inLiquid)
		EnviormentModifier = m_waterMovementModify;

	forceStructX =  -(physicsDB->List_curTransX*EnviormentModifier);
	forceStructY =   (physicsDB->List_curTransY*EnviormentModifier);
	forceStructZ =  -(physicsDB->List_curTransZ*EnviormentModifier);

	if(physicsDB->m_inLiquid == FALSE)
	if(physicsDB->List_doGravity == TRUE || physicsDB->List_currentlyJumping == TRUE) 
	{ 
		forceStructX = forceStructX * physicsDB->m_airControl;
		forceStructY = forceStructY * physicsDB->m_airControl;
		forceStructZ = forceStructZ * physicsDB->m_airControl;
	}

	//INTERNAL ADDITIONAL FORCES
	forceStructX += AdditionalForceX;
	forceStructY += AdditionalForceY;
	forceStructZ += AdditionalForceZ;

	//Convert unit force vectors from local to world coordinate system
	D3DMATRIX matrixLocalania;
	framePtr->GetTransform(NULL,&matrixLocalania);

	if(physicsDB->m_currentSurfaceProp == 1)
		physicsDB->m_ladderLocked = TRUE;

	D3DVECTOR fcfVect;
	fcfVect.x = (float)forceStructX;
	fcfVect.y = (float)forceStructY;
	fcfVect.z = (float)forceStructZ;

	if(physicsDB->m_inLiquid == TRUE || physicsDB->m_ladderLocked == TRUE)
	{//liquid directional mod
		D3DMATRIX freeLookRotation,resultMatrixApplied;
		m_matLib.Rotate(physicsDB->List_freeLookCurPitch,0.0f,0.0f,freeLookRotation.m);
		m_matLib.fastMatrixMultiply(resultMatrixApplied,freeLookRotation,matrixLocalania);
		fcfVect = m_matLib.UpdatePolygonVertexRotationOnly(resultMatrixApplied.m,fcfVect);
	}//end liquid directional mod
	else
		fcfVect = m_matLib.UpdatePolygonVertexRotationOnly(matrixLocalania.m,fcfVect);

	//EXTERNAL FORCES Wind ect
	forceStructX = (double)fcfVect.x;
	forceStructY = (double)fcfVect.y;
	forceStructZ = (double)fcfVect.z;

	forceStructX += ExternalForceX;
	forceStructY += ExternalForceY;
	forceStructZ += ExternalForceZ;

	double gravityApplicable = 0.0f;
	if(physicsDB->m_antiGravity == FALSE && physicsDB->List_doGravity == TRUE)
		gravityApplicable = environmentGravity;



	BOOL m_onGround = TRUE;
	if(physicsDB->List_doGravity == TRUE)
	{
		m_onGround = FALSE;
		//physicsDB->List_currentlyJumping = TRUE;
	}

	if(physicsDB->m_ladderLocked == TRUE)
	{
		gravityApplicable = 0.0f;
		m_onGround = TRUE;
		physicsDB->List_doGravity = FALSE;
	}


	D3DVECTOR currentPosition;
	framePtr->GetPosition(NULL,&currentPosition);

	physicsDB->List_curVelocityVect.x = currentPosition.x - physicsDB->List_lastPosition.x;
	physicsDB->List_curVelocityVect.y = currentPosition.y - physicsDB->List_lastPosition.y;
	physicsDB->List_curVelocityVect.z = currentPosition.z - physicsDB->List_lastPosition.z;

	physicsDB->List_lastPosition.x = currentPosition.x;
	physicsDB->List_lastPosition.y = currentPosition.y;
	physicsDB->List_lastPosition.z = currentPosition.z;

	double additionalDrag = 1.0f - physicsDB->m_crossSectionalArea;
	double drag = physicsDB->List_groundDrag - additionalDrag;
	if(physicsDB->List_doGravity == TRUE || physicsDB->List_currentlyJumping == TRUE)
		drag = physicsDB->m_crossSectionalArea;

	if(!physicsDB->m_inLiquid)
	{//add viscus drag
		viscusDrag = 1.0f;
		//drag = physicsDB->m_crossSectionalArea;
		//drag -= (1.0f - viscusDrag);
	}//end add viscus drag

    bool filterPosRotFunctions = false;
	if(raySys)
		filterPosRotFunctions = raySys->ApplyCurSteeringRotationData((float)(-doYaw * .5));
	
	
	if(filterPosRotFunctions == true)
	{//vehicle physics
		raySys->DoCarPhysics(	physicsDB->List_curTransZ,
								-abs(physicsDB->List_curTransX),
								framePtr,
								&m_matLib,
								this,
								environmentGravity,
								textSys,
								timeAdjust,
								lastTimeAdjust,
								ExternalForceX,
								ExternalForceY,
								ExternalForceZ,
								AdditionalForceX,
								AdditionalForceY,
								AdditionalForceZ,
								EnviormentModifier);
							/*	double forceZ,
								void * frameObjVoid,
								MatrixARB * matLib,
								void * physicsObject,
								double & envGravity,
								void * textSysVoid)*/
 

			/*raySys->DoCarPhysics2D(	physicsDB->List_curTransZ,
										framePtr,
										&m_matLib,
										this,
										environmentGravity,
										textSys);*/
		//return;
	}//end vehicle physics
	else
	{//standard physics
		D3DMATRIX initedOne;
		D3DVECTOR newPosition;
		newPosition = CalculateNewPosition(	drag,
											physicsDB->m_burstForceY,
											gravityApplicable,
											currentPosition.x,
											currentPosition.y,
											currentPosition.z,
											physicsDB->List_curVelocityVect.x,
											physicsDB->List_curVelocityVect.y,
											physicsDB->List_curVelocityVect.z,
											forceStructX,
											forceStructY,
											forceStructZ,
											viscusDrag,
											timeAdjust,
											lastTimeAdjust);

		physicsDB->m_burstForceY = 0.0f;
		D3DVECTOR rotationalForce;
		rotationalForce.x = (float)-doPitch;
		rotationalForce.y = (float)-doYaw;
		rotationalForce.z = -physicsDB->List_curRotZ;
		additionalDrag = 1.0f - physicsDB->m_rotationDragConstant;
		drag = physicsDB->m_rotationalGroundDrag - additionalDrag;
		if(physicsDB->List_doGravity == TRUE || physicsDB->List_currentlyJumping == TRUE)
			drag = physicsDB->m_rotationDragConstant;

		if(physicsDB->m_exponentTurning == false)
		{
			physicsDB->m_curRotationVect = CalculateNewRotation(rotationalForce,
																drag,
																physicsDB->m_curRotationVect,
																timeAdjust,
																lastTimeAdjust);
		}
		else
		{
			physicsDB->m_curRotationVect.z = -physicsDB->List_curRotZ;
			physicsDB->m_curRotationVect.y = (float)-doYaw;
			physicsDB->m_curRotationVect.x = (float)-doPitch;
		}
		//TURNING
		D3DMATRIX matrixLoc;

		m_matLib.Rotate (physicsDB->m_curRotationVect.x,
						 physicsDB->m_curRotationVect.y,
						 physicsDB->m_curRotationVect.z,
						 matrixLoc.m);

		m_matLib.fastMatrixMultiply(initedOne,matrixLoc,framePtr->List_frameMatrix);
		initedOne._41 = newPosition.x;
		initedOne._42 = newPosition.y;
		initedOne._43 = newPosition.z;
		
		framePtr->AddTransformReplace(initedOne);

		//X epsilon
		if(lastPosition.x > framePtr->List_frameMatrix._41)
		{
			if((lastPosition.x - framePtr->List_frameMatrix._41)<.01f)
				framePtr->List_frameMatrix._41 = lastPosition.x;
		}
		else
		{
			if((framePtr->List_frameMatrix._41 - lastPosition.x)<.01f)
				framePtr->List_frameMatrix._41 = lastPosition.x;
		}
		//Y epsilon
		if(lastPosition.y > framePtr->List_frameMatrix._42)
		{
			if((lastPosition.y - framePtr->List_frameMatrix._42)<.01f)
				framePtr->List_frameMatrix._42 = lastPosition.y;
		}
		else
		{
			if((framePtr->List_frameMatrix._42 - lastPosition.y)<.01f)
				framePtr->List_frameMatrix._42 = lastPosition.y;
		}
		//Z epsilon
		if(lastPosition.z > framePtr->List_frameMatrix._43)
		{
			if((lastPosition.z - framePtr->List_frameMatrix._43)<.01f)
				framePtr->List_frameMatrix._43 = lastPosition.z;
		}
		else
		{
			if((framePtr->List_frameMatrix._43 - lastPosition.z)<.01f)
				framePtr->List_frameMatrix._43 = lastPosition.z;
		}
	}//end standard physics


	
}
//------------------------------------------------------------------------------------//
D3DVECTOR CMovementCaps::CalculateNewRotation(D3DVECTOR  forceVect,
										      double     drag,
										      D3DVECTOR  curVelVector,
										      double     timeAdjust,
										      double     lastTimeAdjust)
{

   double adjustLocal = timeAdjust / lastTimeAdjust;
   //adjustLocal = 1.0;
   float timeAdjustFloat = (float)timeAdjust;
   

   double gl_adjustLocal = timeAdjust;

   BOOL adjustedVV = FALSE;

   D3DVECTOR currentVelocityVector;

   currentVelocityVector.x = (curVelVector.x*(float)adjustLocal);
   currentVelocityVector.y = (curVelVector.y*(float)adjustLocal);
   currentVelocityVector.z = (curVelVector.z*(float)adjustLocal);

   while(gl_adjustLocal > 1.0f){//simulate time

      if(!adjustedVV){
          currentVelocityVector.x = currentVelocityVector.x/(float)timeAdjust;
          currentVelocityVector.y = currentVelocityVector.y/(float)timeAdjust;
          currentVelocityVector.z = currentVelocityVector.z/(float)timeAdjust;
          adjustedVV = TRUE;
	  }
   
      currentVelocityVector.x = (currentVelocityVector.x + forceVect.x)* (float)drag;
      currentVelocityVector.y = (currentVelocityVector.y + forceVect.y)* (float)drag;
      currentVelocityVector.z = (currentVelocityVector.z + forceVect.z)* (float)drag;
   
      gl_adjustLocal = gl_adjustLocal - 1.0f;
   }//end simulate time
	double alteredDrag = 1.0 - drag;
    alteredDrag = 1.0 - (alteredDrag * gl_adjustLocal);

	float gl_adjustLocalFloat = (float)gl_adjustLocal;
	float alteredDragFloat = (float)alteredDrag;

    currentVelocityVector.x = ((currentVelocityVector.x + (forceVect.x*gl_adjustLocalFloat))* alteredDragFloat)*timeAdjustFloat;
    currentVelocityVector.y = ((currentVelocityVector.y + (forceVect.y*gl_adjustLocalFloat))* alteredDragFloat)*timeAdjustFloat;
    currentVelocityVector.z = ((currentVelocityVector.z + (forceVect.z*gl_adjustLocalFloat))* alteredDragFloat)*timeAdjustFloat;
   
   return currentVelocityVector;
}
//-------------------------------------------------------------------------------------
D3DVECTOR CMovementCaps::CalculateNewPosition(  double drag,
												double burstForceY,
												double gravity,
												double currentPosX,
												double currentPosY,
												double currentPosZ,
												double curVelVectorX,
												double curVelVectorY,
												double curVelVectorZ,
												double forceVectX,
												double forceVectY,
												double forceVectZ,
												double externalModifier,
												double timeAdjust,
                                                double lastTimeAdjust
												)
{
  D3DVECTOR currentPos;
   double gl_adjustLocal = (double)timeAdjust;
   double gl_adjust      = (double)timeAdjust;

   double adjustLocal = timeAdjust / lastTimeAdjust;
   //adjustLocal = 1.0;

   BOOL adjustedVV = FALSE;

   double currentVelocityVectorX = (curVelVectorX*adjustLocal);
   double currentVelocityVectorY = (curVelVectorY*adjustLocal);
   double currentVelocityVectorZ = (curVelVectorZ*adjustLocal);

   while(gl_adjustLocal > 1.0){//simulate time

      if(!adjustedVV){
          currentVelocityVectorX = currentVelocityVectorX/gl_adjust;
          currentVelocityVectorY = currentVelocityVectorY/gl_adjust;
          currentVelocityVectorZ = currentVelocityVectorZ/gl_adjust;
          adjustedVV = TRUE;
	  }
   
      currentVelocityVectorX = ((currentVelocityVectorX + forceVectX)* drag)*externalModifier;
      currentVelocityVectorY = ((currentVelocityVectorY + (forceVectY+gravity+burstForceY))* drag)*externalModifier; 
	  currentVelocityVectorZ = ((currentVelocityVectorZ + forceVectZ)* drag)*externalModifier;
   
	  burstForceY = 0.0f;
      gl_adjustLocal = gl_adjustLocal - 1.0;
   }//end simulate time
	  double alteredDrag = 1.0 - drag;
      alteredDrag = 1.0 - (alteredDrag * gl_adjustLocal);

	  double enviormantDrag = 1.0 - externalModifier;
	  enviormantDrag = 1.0 - (enviormantDrag * gl_adjustLocal);

      currentVelocityVectorX = ((currentVelocityVectorX + (forceVectX*gl_adjustLocal))* alteredDrag)*enviormantDrag;
      currentVelocityVectorY = ((currentVelocityVectorY + ((forceVectY+gravity+burstForceY)*gl_adjustLocal))* alteredDrag)*enviormantDrag;
      currentVelocityVectorZ = ((currentVelocityVectorZ + (forceVectZ*gl_adjustLocal))* alteredDrag)*enviormantDrag;
	  
	  currentPos.x = (float)(currentPosX + (currentVelocityVectorX*gl_adjust));
	  currentPos.y = (float)(currentPosY + (currentVelocityVectorY*gl_adjust));
	  currentPos.z = (float)(currentPosZ + (currentVelocityVectorZ*gl_adjust));
	  
	  return currentPos;
}
//------------------------------------------------------------------------------------//
double CMovementCaps::CalculateForceValue(double force,
									     double drag,
									     double curValue,
										 double & timeAdjust,
										 double & lastTimeAdjust)
{

   double adjustLocal = timeAdjust / lastTimeAdjust;
   //adjustLocal = 1.0;

   double gl_adjustLocal = timeAdjust;

   BOOL adjustedVV = FALSE;

   double currentVelocity;

   currentVelocity = (curValue*adjustLocal);

   while(gl_adjustLocal > 1.0f){//simulate time

      if(!adjustedVV){
          currentVelocity = currentVelocity/timeAdjust;
          adjustedVV = TRUE;
	  }
   
      currentVelocity = (currentVelocity + force)* drag;
   
      gl_adjustLocal = gl_adjustLocal - 1.0f;
   }//end simulate time
	  double alteredDrag = 1.0 - drag;
      alteredDrag = 1.0 - (alteredDrag * gl_adjustLocal);

      currentVelocity = ((currentVelocity + (force*gl_adjustLocal))* alteredDrag)*timeAdjust;
      
   return currentVelocity;
}
//-------------------------------------------------------------------------------------
D3DVECTOR CMovementCaps::CalculateNewPositionIso(   double drag,
													double gravity,
													double currentPosX,
													double currentPosY,
													double currentPosZ,
													double curVelVectorX,
													double curVelVectorY,
													double curVelVectorZ,
													double externalModifier,
													double timeAdjust,
													double lastTimeAdjust
												)
{
  D3DVECTOR currentPos;
   double gl_adjustLocal = (double)timeAdjust;
   double gl_adjust      = (double)timeAdjust;

   double adjustLocal = timeAdjust / lastTimeAdjust;
   //adjustLocal = 1.0;

   BOOL adjustedVV = FALSE;

   double currentVelocityVectorX = (curVelVectorX*adjustLocal);
   double currentVelocityVectorY = (curVelVectorY*adjustLocal);
   double currentVelocityVectorZ = (curVelVectorZ*adjustLocal);

   while(gl_adjustLocal > 1.0){//simulate time

      if(!adjustedVV){
          currentVelocityVectorX = currentVelocityVectorX/gl_adjust;
          currentVelocityVectorY = currentVelocityVectorY/gl_adjust;
          currentVelocityVectorZ = currentVelocityVectorZ/gl_adjust;
          adjustedVV = TRUE;
	  }
   
      currentVelocityVectorX = ((currentVelocityVectorX)* drag)*externalModifier;
      currentVelocityVectorY = ((currentVelocityVectorY + gravity)* drag)*externalModifier; 
	  currentVelocityVectorZ = ((currentVelocityVectorZ)* drag)*externalModifier;
   
      gl_adjustLocal = gl_adjustLocal - 1.0;
   }//end simulate time
	  double alteredDrag = 1.0 - drag;
      alteredDrag = 1.0 - (alteredDrag * gl_adjustLocal);

	  double enviormantDrag = 1.0 - externalModifier;
	  enviormantDrag = 1.0 - (enviormantDrag * gl_adjustLocal);

      currentVelocityVectorX = (currentVelocityVectorX * alteredDrag)*enviormantDrag;
      currentVelocityVectorY = ((currentVelocityVectorY + ((gravity)*gl_adjustLocal))* alteredDrag)*enviormantDrag;
      currentVelocityVectorZ = (currentVelocityVectorZ * alteredDrag)*enviormantDrag;
	  
	  currentPos.x = (float)(currentPosX + (currentVelocityVectorX*gl_adjust));
	  currentPos.y = (float)(currentPosY + (currentVelocityVectorY*gl_adjust));
	  currentPos.z = (float)(currentPosZ + (currentVelocityVectorZ*gl_adjust));
	  
	  return currentPos;
}
//-----------------Serialize---------------------//

void CMovementCaps::Serialize( CArchive& ar )
{
int reserved = 0;
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	 ar 
		<< List_mouseRestriction
		<< m_inertia
		<< List_autoDecel
		<< List_gravity 
		<< List_maxSpeed
        << List_minSpeed
        << List_accellerate
        << List_decellerate
        << List_pitchAccel
        << List_pitchDecel
        << List_yawAccel
        << List_yawDecel
        << List_curRotX
        << List_curRotY
        << List_curRotZ
        << m_corneringStiffnessF
        << m_corneringStiffnessR
        << m_maxGrip
        << List_rollFactor
        << List_minYaw
        << List_maxYaw
        << List_minPitch
        << List_maxPitch
        << m_maxRoll
        << m_rollAccel
        << m_rollDecel
        << m_floatWhenDead
        << List_letGoDecel
        //animation varials
        << m_burstForceY
        << List_curRotAnimY
        << m_autoLevelXStrength
        //<< m_autoLevelZStrength
        << m_autoLevelTolerance
        << m_autoLevelX
        //<< m_autoLevelZ
        << m_rotationDragConstant
        << m_rotationalGroundDrag
        << m_freeLookPichForce
        << m_submergedHieght
        << m_inLiquidMotionModifier
        << m_inLiquid
        << List_crouched
        << m_antiGravity
		<< List_slidingEnabled
		<< TRUE//List_doGravity;
		<< List_jumpPower
		<< List_freeLookSpeed
		<< List_freeLookCurMaxPitch
		<< List_freeLookCurMaxYaw
		<< List_invertMouse
        << List_jointMouseFocus
		<< List_mouseSensitivity
		<< List_mass
        << m_curRotationVect.x
        << m_curRotationVect.y
        << m_curRotationVect.z
        << m_pckSys_changeable//List_curVelocityVect.x
        << m_pckSys_firstPersonMode//List_curVelocityVect.y
        << m_pckSys_thirdPersonMode//List_curVelocityVect.z
        << List_maxVelocity
        << m_pckSys_aiControlModel//List_lastPosition.x
        << List_lastPosition.y
        << List_lastPosition.z
	    << List_groundDrag
		<< List_autoDecelY          
        << List_autoDecelZ
		<< List_capabilityFovMax
        << List_capabilityFovMin
        << List_capFovChangeSensitivity
		//UPG
		<< m_frictionCoefficient
        << m_freeLookYawForce
        << m_strafeReduction
        << m_crossSectionalArea
        << m_displacedVolume
        << m_airControl
        << m_jumpRecoverTime
        << List_zeroToSpeed;

	}
	else
	{
     ar >> List_mouseRestriction
		>> m_inertia
		>> List_autoDecel
		>> List_gravity
		>> List_maxSpeed
        >> List_minSpeed
        >> List_accellerate
        >> List_decellerate
        >> List_pitchAccel
        >> List_pitchDecel
        >> List_yawAccel
        >> List_yawDecel
        >> List_curRotX
        >> List_curRotY
        >> List_curRotZ
        >> m_corneringStiffnessF
        >> m_corneringStiffnessR
        >> m_maxGrip
        >> List_rollFactor
        >> List_minYaw
        >> List_maxYaw
        >> List_minPitch
        >> List_maxPitch
        >> m_maxRoll
        >> m_rollAccel
        >> m_rollDecel
        >> m_floatWhenDead
        >> List_letGoDecel
        //animation varials
        >> m_burstForceY
        >> List_curRotAnimY
        >> m_autoLevelXStrength
        //>> m_autoLevelZStrength
        >> m_autoLevelTolerance
        >> m_autoLevelX
        //>> m_autoLevelZ
        >> m_rotationDragConstant
        >> m_rotationalGroundDrag
        >> m_freeLookPichForce
        >> m_submergedHieght
        >> m_inLiquidMotionModifier
        >> m_inLiquid
        >> List_crouched
        >> m_antiGravity
		>> List_slidingEnabled
		>> List_doGravity
		>> List_jumpPower
		>> List_freeLookSpeed
		>> List_freeLookCurMaxPitch
		>> List_freeLookCurMaxYaw
		>> List_invertMouse
        >> List_jointMouseFocus
		>> List_mouseSensitivity
		>> List_mass
        >> m_curRotationVect.x
        >> m_curRotationVect.y
        >> m_curRotationVect.z
		>> m_pckSys_changeable//List_curVelocityVect.x
        >> m_pckSys_firstPersonMode//List_curVelocityVect.y
        >> m_pckSys_thirdPersonMode//List_curVelocityVect.z
        >> List_maxVelocity
        >> m_pckSys_aiControlModel//List_lastPosition.x
        >> List_lastPosition.y
        >> List_lastPosition.z
	    >> List_groundDrag
		>> List_autoDecelY      
        >> List_autoDecelZ
		>> List_capabilityFovMax
        >> List_capabilityFovMin
        >> List_capFovChangeSensitivity
     //UPG
        >> m_frictionCoefficient
        >> m_freeLookYawForce
        >> m_strafeReduction
        >> m_crossSectionalArea
        >> m_displacedVolume
        >> m_airControl
        >> m_jumpRecoverTime
        >> List_zeroToSpeed;


    
	 List_curTransX                     = 0.0f;
     List_curTransY                     = 0.0f;
     List_curTransZ                     = 0.0f;
	 List_curSpeed                      = 0.0f;
	 List_doGravity                     = FALSE;
	 m_jetPackInUse                     = FALSE;
	 m_jetPackInUse                     = FALSE;
     m_yawIsUsed                        = FALSE;
     m_pitchIsUsed                      = FALSE;
     m_rollIsUsed                       = FALSE;
     m_speedControlInUse                = FALSE;
     m_sidestepsInUse                   = FALSE;		
     List_freeLookCurPitch              = 0.0f;
     List_freeLookCurYaw                = 0.0f;
	 List_freeLookRefFrame              = NULL;
	 List_currentlyJumping              = FALSE;
	 List_canWalk                       = TRUE;//varial start true
	 List_targetFov                     = 0.0f;
	 List_fovCurrentInterp              = 0;
	 List_lastFov                       = 0.0f;
	 m_inLiquid                         = FALSE;
	 m_burstForceY                      = 0.0f;
	 m_timeStamp                        = 0;
	 List_gravity                       = 0.0f;
	 m_grvtyChngTimeStmp                = GetTickCount();
	 m_currentSurfaceProp               = 0;
	 m_compoundCurX                     = 0.0f;
     m_compoundCurY                     = 0.0f;
	 m_ladderLocked                     = FALSE;
     m_ladderLockedDir.x                = 0.0f;
     m_ladderLockedDir.y                = 0.0f;
     m_ladderLockedDir.z                = 0.0f;
	 m_angularvelocity                  = 0.0;
     ZeroMemory(&m_ladderBox,sizeof(ABBOX));
	 m_jumpRecoveryStamp                = m_grvtyChngTimeStmp;
	 
	}
}

IMPLEMENT_SERIAL(CMovementCaps, CObject, 0)
