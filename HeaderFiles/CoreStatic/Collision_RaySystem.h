#include "CSkeletalClass.h"
#include "CFrameObj.h"
#include "matrixarb.h"

#pragma once

class CShockClass : public CObject
{
	public:
		CShockClass(float maxY,float minY,float tightness,D3DVECTOR origPosition);
		~CShockClass(void);
		
        //vars
		D3DVECTOR m_originPosition;
		D3DVECTOR m_maxCompressPosition;
        D3DVECTOR m_maxUnCompressPosition;
		D3DVECTOR m_currentPosition;

		float     m_tightness;
		float     m_maxY;
		float     m_minY;
		//Functions
	
};


class CWheelClass : public CObject
{
	public:
		CWheelClass(void);
		CWheelClass(int axleIndex,float radius,CFrameObj * parentFrame);
		~CWheelClass(void);
		
        //vars
		int                     m_axleIndex;
		float					m_radius;
		float                   m_wheelBase;
		CFrameObj			*	m_frame;
		CFrameObj			*	m_wheelHolder;
		CFrameObj			*	m_wheelFrameRef;
		CFrameObj			*	m_pivotFrameRef;
       
		//
		float                   m_lengthBetweenFBWheels;
		float                   m_currentSteeringRotation;
		float                   m_steeringRotationMax;
		bool                    m_canSteerRotate;


		CShockClass         *   m_shock;
		//Functions
	
};

class CCollision_Ray
{
	public:
		CCollision_Ray(void);
		~CCollision_Ray(void);
		
        //static
		D3DVECTOR			m_startPoint;
		D3DVECTOR			m_endPoint;
		//dynamic
        D3DVECTOR			m_dynStartPoint;
		D3DVECTOR			m_dynEndPoint;
        //intesect
        D3DVECTOR			m_lastIntersect;
		D3DVECTOR			m_prevIntersect;
        //status
		bool				m_collidedThisRound;
		//
		BYTE                m_directionalGen;
		bool                m_stopVelVector;
		
        CWheelClass       * m_wheel;
		bool                m_collidedLastRound;
		float               m_lastRotation;
		D3DVECTOR           m_lastNormal;
		float               m_depth;
		CCollision_Ray    * m_pivotRef;
		CCollision_Ray    * m_dirPivot;
		CCollision_Ray    * m_crossPivot;
		CCollision_Ray    * m_xPivot;
		byte			    m_indexRef;
		//Functions
		void Reset();
		void HandleTireRotation(MatrixARB * matLib,D3DMATRIX & baseFrameMatrix);
		D3DVECTOR GetCorrectivePosition(MatrixARB * matLib, int calctype);
		void ShockCallback();
		void HandleTireSteeringRotation(MatrixARB * matLib);
};


class CCollision_RaySystem
{
	public:
		CCollision_RaySystem(void);
		CCollision_RaySystem(CSkeletonObject * skeletonPtr,int type,MatrixARB * matLib);
		~CCollision_RaySystem(void);


		CCollision_Ray				* m_rays;
		int							  m_rayCount;
		bool						  m_staticWorldCollisionChecked;
		D3DVECTOR					  m_centerMassPosition;
		D3DVECTOR					  m_lastCenterMassPosition;
		bool                          m_centerOfMassCollided;
		int							  m_type;
		bool                          m_worldCorrected;
		byte                          m_specialOperation;
		float                         m_elevationBasis;

		DWORD                         m_stamp;
		


		//Functions
		void Recalculate(CFrameObj * frame, CSkeletonObject * skeletonPtr,MatrixARB * matLib);
		void UpdateLastPosition(CFrameObj * frame,CSkeletonObject * skeletonPtr,MatrixARB * matLib);
		void InitOnSpawn(CFrameObj * frame,CSkeletonObject * skeletonPtr,MatrixARB * matLib);
		void UpdateRays(CSkeletonObject * skeletonPtr);
		void ResetCollisionFlags();
        void UpdateCurrentPosition(CFrameObj * frame,CSkeletonObject * skeletonPtr,MatrixARB * matLib);
		bool RecalculateByIndex(int index,
								CFrameObj * frame, 
								CSkeletonObject * skeletonPtr,
								MatrixARB * matLib,
								D3DVECTOR * currentPosition,
								D3DVECTOR * lastPosition);
		void HandleTireRotation(MatrixARB * matLib,int rayIndex,D3DMATRIX & baseFrameMatrix);
		void CrossOrientate(CFrameObj * baseFrame, CFrameObj * pivotFrame,MatrixARB * matLib);
		CString ConvToString(int number);
		int GetCollisionCount();
		void GetDepthOrderArray(int ** orderArray, int count);
		CCollision_Ray GetRayByVirtualIndex(int virtualIndex);
		CCollision_Ray * GetDeepestRay(int byDirGen);
		D3DMATRIX Get4pointOrientationMatrixBasedDPRay(MatrixARB * matLib,CCollision_Ray * deepestRay);
		void WheelCallback(MatrixARB * matLib,D3DMATRIX & entMatrix);
		int GetCollisionCountWheelSys();
		void GetCurSteeringRotationData(float & curWheelRotation,float & lengthBetweenTires);
		void SetCurSteeringRotationData(float curWheelRotation);
		void HandleTireSteeringRotation(MatrixARB * matLib,int rayIndex);
		bool ApplyCurSteeringRotationData(float curWheelRotation);
		double GetWheelBase();
		D3DMATRIX MultiPointCalculatePosition(double force,D3DMATRIX & bFrameMatrix,MatrixARB * matLib,void * physicsObject,double & envGravity,double & drag,D3DVECTOR & slipVectRet,void * textSysVoid);
		void DoCarPhysics(  double force,
			                double brakeForce,
							void * frameObjVoid,
							MatrixARB * matLib,
							void * physicsObject,
							double & envGravity,
							void * textSysVoid,
							double adjustTime,
							double & lastAdjustTime,
							double & extForceVectX,
							double & extForceVectY,
							double & extForceVectZ,
							double & additionalForceVectX,
							double & additionalForceVectY,
							double & additionalForceVectZ,
							double & externalModifier);
		float Round(const float &number, const int num_digits);
		double RoundDouble(double doValue, int nPrecision);
		void DoCarPhysics2D(  double forceZ,
											void * frameObjVoid,
											MatrixARB * matLib,
											void * physicsObject,
											double & envGravity,
											void * textSysVoid);
		void GetCurSteeringRotationData(double & curWheelRotation,double & lengthBetweenTires);
};
