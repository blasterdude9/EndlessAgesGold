#ifndef CCAMERACLASS_H
#define CCAMERACLASS_H

#include "CFrameObj.h"
#include "matrixarb.h"
#include "CMovementModel.h"

class  CCameraObj : public CObject
{
	CCameraObj(){}
	
     
    DECLARE_SERIAL( CCameraObj )
    
    
public:
	
	CCameraObj(CString   cameraName,
			   CString   fileRefVisualOptional,
			   int       type,
			   BOOL      lookAt,
		       float     clipPlane,
			   float     fovView,
			   D3DVECTOR initFloatingTarget
			   );

    MatrixARB          gl_matrixFunctions;
    CFrameObj *        List_floatingFocus;
	CFrameObj *        List_floatingCamParent;
    CFrameObj *        List_cameraTarget;
	CFrameObj *        List_cameraBase;
	float              List_hieghtBaseAspect;
	float              List_cameraMaxYaw;
	float              List_cameraCurYaw;
	float              List_cameraYawSpeed;
	float              List_cameraMaxPitch;
	float              List_cameraCurPitch;
	float              List_cameraPitchSpeed;
	D3DVECTOR          List_floatingCamTarget;
	float              List_floatingTolerance;
	float              List_zoomInSpeed;
	float              List_zoomOutSpeed;
	float              List_zoomInStop;
	BOOL               List_floatingUsesCollision;
	float              List_fixedAfterPosX;
	float              List_fixedAfterPosY;
	float              List_fixedAfterPosZ;
	float              List_InitialPosX;
	float              List_InitialPosY;
	float              List_InitialPosZ;
	float              List_floatingMovePwr;
	float              List_clipPlane;
	float              List_fovView;
    CString            List_cameraName;
	CString            List_fileRefVisualOptional;
    int                List_type;
	BOOL               List_lookAt;
	BOOL               List_locked;
	float              m_pitchRotation;
    //kick dynamics
	float              m_curKickX;
	float              m_curKickY;
	float              m_curBurstRate;
	float              m_curBurstRateY;
	float              m_kickBurstDet;
	float              m_kickReturnRate;
    //brethe/move dynamics
	float              m_bobXRotCur;
	float              m_bobCurMaxVelocity;
	float              m_bobCurVelocity;
	float              m_bobAccelRate;
	float              m_bobDecelRate;
	long               m_bobStallTime;
	BOOL               m_bobSwitch;
	BOOL               m_bobAccelerating;
	long               m_bobTimeStamp;

	CMovementObj      *m_entityRef;

	int                m_miscVal1;
	float              m_miscVal2;
    float              m_miscVal3;
	float              m_miscVal4;

	BOOL               m_filterFirstPersonMode;

	D3DVECTOR          m_camDirNormal;


    
    void SafeDelete();
	void Clone(CCameraObj ** copy);
	D3DMATRIX GetCameraInvMatrix();
	D3DMATRIX GetCameraMatrix();
	void CameraKickCallback(float glAdjust);
	void CameraBretheCallback(float glAdjust);
	void Serialize( CArchive& ar );
};

class  CCameraObjList : public CObList
{
public:
   CCameraObjList(){};
    
   void SafeDelete();
    DECLARE_SERIAL( CCameraObjList )
    
    
};


#endif