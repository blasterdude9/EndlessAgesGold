
#include "CFrameObj.h"
#ifndef PHYSICSEMU_H
#define PHYSICSEMU_H

//movement definition  phisics emulation
class  CMovementCaps : public CObject
{
	
     
	DECLARE_SERIAL( CMovementCaps );
	
    
public:
	
	CMovementCaps();
	void SafeDelete();

    //Capabilities
        float            List_capabilityFovMax;
        float            List_capabilityFovMin;
		float            List_capFovChangeSensitivity;
		float            List_targetFov;
		float            List_lastFov;
		int              List_fovCurrentInterp;
		
    //PHYSICS Hardcore
	    float            List_mass;
		D3DVECTOR        m_curRotationVect;
		D3DVECTOR        List_curVelocityVect;
        float            List_maxVelocity;
		D3DVECTOR        List_lastPosition;
		float            List_groundDrag;

		float            m_frictionCoefficient;//List_staticFriction;
		
		float            m_strafeReduction;
		float            m_crossSectionalArea;
		float            m_displacedVolume;
		float            m_airControl;//List_dynamicTraction;
		//float            m_strafeReduction;
		float            List_zeroToSpeed;
		
		BOOL  List_autoDecel;//auto decel for x forces
		BOOL  List_autoDecelY;//'' Y forces                     
        BOOL  List_autoDecelZ;//'' Z forces     
	//appended
        float List_mouseSensitivity;
	    BOOL List_invertMouse;
		BOOL List_jointMouseFocus;
		CFrameObj* List_freeLookRefFrame;
		float List_freeLookSpeed;
		float List_freeLookCurPitch;
		float List_freeLookCurMaxPitch;
		float List_freeLookCurYaw;
		float List_freeLookCurMaxYaw;
    //special actions
	    BOOL  List_currentlyJumping;
        float List_jumpPower;
    //restrictives
	    BOOL List_canWalk;
	    BOOL List_doGravity;
	    float List_mouseRestriction;
	    BOOL  List_slidingEnabled;
	//effects
	    float List_gravity;
    //movement
		//--
		float List_curSpeed;
		float List_maxSpeed;
		float List_minSpeed;
		float List_accellerate;
		float List_decellerate;
		float List_pitchAccel;
		float List_pitchDecel;
		float List_yawAccel;
		float List_yawDecel;
        float List_curRotX;
		float List_curRotY;
		float List_curRotZ;
		float List_curTransX;
		float List_curTransY;
		float List_curTransZ;
		float List_rollFactor;
		float List_minYaw;
		float List_maxYaw;
		float List_minPitch;
		float List_maxPitch;
		
		float List_letGoDecel;
        //animation varials
		float m_burstForceY;
        float List_curRotAnimY;
		float m_autoLevelStrength;
		float m_distancePerSecond;
		BOOL  m_autoLevel;
		float m_rotationDragConstant;
		float m_rotationalGroundDrag;
		
		float m_submergedHieght;
		float m_inLiquidMotionModifier;
		BOOL  m_inLiquid;
		BOOL  List_crouched;//0 = normal
		float m_antiGravity;
		float m_freeLookPichForce;
		float m_freeLookYawForce;

		float m_maxRoll;
		float m_rollAccel;
		float m_rollDecel;
		long  m_timeStamp;
		BOOL  m_jetPackInUse;

		BOOL  m_yawIsUsed;
        BOOL  m_pitchIsUsed;
        BOOL  m_rollIsUsed;
        BOOL  m_speedControlInUse;
        BOOL  m_sidestepsInUse;	

		float m_rampTolerance;
		BOOL  m_floatWhenDead;

		long m_grvtyChngTimeStmp;
		
		

	void Serialize( CArchive& ar );
	BOOL Clone(CMovementCaps ** clone);
	void SetUseStatesToFalse();

};
#endif