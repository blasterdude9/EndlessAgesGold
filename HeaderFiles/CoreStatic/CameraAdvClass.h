#pragma once
#include "matrixarb.h"

class CCameraAdvObj : public CObject
{
	DECLARE_SERIAL( CCameraAdvObj )
public:
	CCameraAdvObj(void);
	~CCameraAdvObj(void);

	//vars
	CString				m_name;
    int					m_followTrace;//entity trace
	CString				m_boneNameTrace;//bone on entity target
	float				m_autoSpeed;//speed of camera auto correction etc
	float				m_pitchMax;
	float				m_pitchMin;
	float				m_yawMax;
	float				m_yawMin;
    float				m_zoomMax;
	float				m_zoomMin;
	float				m_curPitch;
	float				m_orbitPitchOffset;
	float				m_curYaw;
	float				m_curZoom;
	float               m_lastUncollidedZoom;
	float				m_reCorrectY;
    D3DVECTOR			m_collisionRayLeft;
	D3DVECTOR			m_collisionRayRight;
	D3DVECTOR           m_currentTargetPos;
	D3DVECTOR           m_dirVect;
    float				m_cameraSize;
	int					m_followMode;//0 = auto correct 1=manual mode
	D3DMATRIX			m_currentMatrix;
	D3DMATRIX           m_tempMatrix;
	BOOL                m_rightClickControl;
	float				m_clipPlane;
	float               m_hieghtAspect;
	DWORD               m_switchStamp;
	BOOL				m_overrideFlag;
	float				m_overShoulderOffsetX;
	float				m_overShoulderOffsetY;
	float				m_reduceYAdjust;
	//kick dynamics
	float				m_curKickX;
	float				m_curKickY;
	float				m_curBurstRate;
	float				m_curBurstRateY;
	float				m_kickBurstDet;
	float				m_kickReturnRate;

	//functions
	void Clone(CCameraAdvObj ** clone);
	bool Update(float & timeAdjust,void * runtimeEntityMem,MatrixARB * matLib);
	bool ApplyMouseRotation(float X, float Y, float mouseWheel);
	D3DMATRIX GetCameraMatrix();
	D3DMATRIX GetInvCameraMatrix(MatrixARB * matLib);
	BOOL CheckCollision(D3DVECTOR & startPoint, float & dist, MatrixARB * matLib,void * runtimeEntityMem);
	void Serialize( CArchive& ar );
};

class CCameraAdvList : public CObList
{
    DECLARE_SERIAL( CCameraAdvList )
public:
	CCameraAdvList(){};
	~CCameraAdvList(){};



};
