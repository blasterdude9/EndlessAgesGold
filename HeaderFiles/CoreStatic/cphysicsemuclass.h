
#include "CFrameObj.h"
#include "CStructuresMisl.h"

#ifndef PHYSICSEMU_H
#define PHYSICSEMU_H

//movement definition  phisics emulation
class CMovementCaps : public CObject
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
		MatrixARB        m_matLib;
	    float            List_mass;
		float            m_inertia;
		D3DVECTOR        m_curRotationVect;
		D3DVECTOR        List_curVelocityVect;
        float            List_maxVelocity;
		D3DVECTOR        List_lastPosition;
		float            List_groundDrag;
		float            m_corneringStiffnessF;
		float            m_corneringStiffnessR;
		float            m_maxGrip;

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

		float m_compoundCurX;
		float m_compoundCurY;
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
		float m_autoLevelXStrength;
		float m_autoLevelZStrength;
		float m_autoLevelTolerance;
		BOOL  m_autoLevelX;
		BOOL  m_autoLevelZ;
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

		long			m_grvtyChngTimeStmp;
		int				m_currentSurfaceProp;

		BOOL			m_ladderLocked;
		D3DVECTOR		m_ladderLockedDir;
		ABBOX			m_ladderBox;
		int				m_jumpRecoverTime;
		long			m_jumpRecoveryStamp;
		int             m_curSpeed;

		//Matrix locking 
		int				m_useLockPoint;//0=currently unlocked   1=currently locked 
		D3DMATRIX		m_lastLock;//
		CFrameObj      *m_optFramePtr;
		int             m_optLockTrace;

		bool            m_exponentTurning;
		

		
		
		BOOL            m_lastGravityStatus;
		double          m_angularvelocity;
		float           m_angleTemp;
		D3DVECTOR       m_velocity_wc;

		//control type in perspective of point and click movement VS realtime
		//
		BOOL            m_pckSys_changeable;//user can swicth between modes
		int             m_pckSys_firstPersonMode;//0=realtime 1=point and click movement
        int             m_pckSys_thirdPersonMode;//0=realtime 1=point and click movement
        int             m_pckSys_aiControlModel;//used perameters from ai to 


		
		

	void Serialize( CArchive& ar );
	BOOL Clone(CMovementCaps ** clone);
	void SetUseStatesToFalse();
	float GetLookYaw();
	float GetLookPitch();
	double CalculateForceValue(double force,
									     double drag,
									     double curValue,
										 double & timeAdjust,
										 double & lastTimeAdjust);
	void DoPhysics(        double timeAdjust,
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
								  void * textSysVoid);
	D3DVECTOR CalculateNewRotation( D3DVECTOR forceVect,
									double     drag,
									D3DVECTOR curVelVector,
									double timeAdjust,
									double lastTimeAdjust);
	D3DVECTOR CalculateNewPosition( double drag,
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
									);
	D3DVECTOR CalculateNewPositionIso(  double drag,
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
									  );

};
#endif