//#include "stdafx.h"
#include "afxwin.h"
#include "afxext.h"
#include "resource.h"
#include <d3d9.h>
//#include <afxtempl.h>
#include "CPhysicsEmuClass.h"

//-----------------------------------------------//
//-----------------------------------------------//
//------------Motion caps class------------------//
CMovementCaps::CMovementCaps()
{
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
m_autoLevelStrength                = 0.0f;
m_distancePerSecond                = 0.0f;
m_autoLevel                        = FALSE;
m_rotationDragConstant             = 0.9f;
m_rotationalGroundDrag             = 1.0f;
m_freeLookPichForce                = 0.0f;
m_submergedHieght                  = 0.0f;
m_inLiquidMotionModifier           = 0.0f;
m_inLiquid                         = 0.0;
m_antiGravity                      = FALSE;
List_mass                          = 100.0f;
m_curRotationVect.x                   = 0.0f;
m_curRotationVect.y                   = 0.0f;
m_curRotationVect.z                   = 0.0f;
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
midTermPtr->m_autoLevelStrength           = m_autoLevelStrength;
midTermPtr->m_distancePerSecond           = m_distancePerSecond;
midTermPtr->m_autoLevel                   = m_autoLevel;
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
midTermPtr->List_curVelocityVect.x        = List_curVelocityVect.x;
midTermPtr->List_curVelocityVect.y        = List_curVelocityVect.y;
midTermPtr->List_curVelocityVect.z        = List_curVelocityVect.z;
midTermPtr->List_maxVelocity              = List_maxVelocity;
midTermPtr->List_lastPosition.x           = List_lastPosition.x;
midTermPtr->List_lastPosition.y           = List_lastPosition.y;
midTermPtr->List_lastPosition.z           = List_lastPosition.z;
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
//-----------------Serialize---------------------//

void CMovementCaps::Serialize( CArchive& ar )
{
int reserved = 0.0f;
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	 ar 
		<< List_mouseRestriction
		<< List_curSpeed
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
        << List_curTransX
        << List_curTransY
        << List_curTransZ
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
        << m_autoLevelStrength
        << m_distancePerSecond
        << m_autoLevel
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
        << List_curVelocityVect.x
        << List_curVelocityVect.y
        << List_curVelocityVect.z
        << List_maxVelocity
        << List_lastPosition.x
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
        << reserved
        << List_zeroToSpeed;
	}
	else
	{
     ar >> List_mouseRestriction
		>> List_curSpeed
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
        >> List_curTransX
        >> List_curTransY
        >> List_curTransZ
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
        >> m_autoLevelStrength
        >> m_distancePerSecond
        >> m_autoLevel
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
        >> List_curVelocityVect.x
        >> List_curVelocityVect.y
        >> List_curVelocityVect.z
        >> List_maxVelocity
        >> List_lastPosition.x
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
        >> reserved
        >> List_zeroToSpeed;


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
	}
}

IMPLEMENT_SERIAL(CMovementCaps, CObject, 0)
