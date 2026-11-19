#include "CFrameObj.h"
#include "CBoneAnimationClass.h"
#include "CEXMeshClass.h"
#include "CMaterialClass.h"
#include "chiarclemeshclass.h"

#ifndef CBONECLASS_H
#define CBONECLASS_H

//Base skeleton Class
class  CBoneObject : public CObject
{
	
     
    DECLARE_SERIAL( CBoneObject )
    
    
public:
	
	CBoneObject();
	
	CFrameObj         *  m_boneFrame;
	CString              m_parentName;
	CString              m_boneName;
	CBoneAnimationList * m_animationList;
	MatrixARB            GL_matrixFunctions;
	D3DMATRIX            m_startPosition;
	CHiarchyMesh        *m_hiarchyMesh;
	int                  m_secondaryOverridable;

	BOOL                 m_referencedAnimList;


	//lastPositions used for interpolation
	D3DVECTOR            m_lastPosition;
	D3DVECTOR            m_lastUpVector;
	D3DVECTOR            m_lastDirVector;
	BOOL                 m_lastPositionsAreInitialized;
	
    D3DVECTOR            m_targetPosition;
	D3DVECTOR            m_targetUpVector;
	D3DVECTOR            m_targetDirVector;
	long                 m_lastToTargetDuration;
	long                 m_lastToTargetTimeStamp;

    //functions
	
	void Clone(CBoneObject ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void SafeDelete();
    BOOL ApplyAnimation(int animation,int frame);
	BOOL ApplyAnimationByTime(int animation,long time);
	void UpdateBone(CBoneAnimationObject * animPtr);
	void UpdateBoneByPosition(int baseAnim,
									   int focusAnim,
									   long millisecondInto,
									   int base,
									   int target,
									   int milisecondsPerFrame);

	 void SetTransformDataForAnimation(long duration,
											 D3DVECTOR & lastPosition,
											 D3DVECTOR & lastUpVector,
											 D3DVECTOR & lastDirVector,
											 D3DVECTOR & targetPosition,
											 D3DVECTOR & targetUpVector,
											 D3DVECTOR & targetDirVector,
											 long & currentTime);

	/*void UpdateBoneByMs(int baseAnim,
					    int focusAnim,
					    long baseAnimTime,
					    long focusAnimTime,
						long stepCount,
						long currentTime);*/

	void SetBoneToAnimationFrame(int currentAnimation,
								 int targetFrame,
								 long transitionDuration,
								 long currentTime);

	void Serialize( CArchive& ar );
};

class  CBoneList : public CObList
{
public:
   CBoneList(){};
   void Clone(CBoneList ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice);
   CBoneObject * GetBoneObjectByName(CString name,int * index);
   void SafeDelete();
   void Invalidate();
   CBoneObject * GetBoneByIndex(int index);
    
     
    DECLARE_SERIAL( CBoneList )
    
    
};



#endif