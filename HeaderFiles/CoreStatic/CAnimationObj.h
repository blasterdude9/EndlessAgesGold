#ifndef ANIMATIONOBJ_H
#define ANIMATIONOBJ_H
//animation vetex data or translation data
#include "CPolyObj.h"

#include <mmsystem.h>
#include "dsound.h"

class  CVertexTransObj : public CObject
{
	CVertexTransObj(){}
	
     
	DECLARE_SERIAL( CVertexTransObj );
	
    
public:
	
	CVertexTransObj(int L_vCount,
		            D3DVECTOR   L_translationXYZ,//translation animation XYZ of the vector
					D3DVECTOR   L_scaleXYZ,//translation animation XYZ of the vector
					D3DVECTOR   L_rotateXYZ,//tranlation rotation animation of the vector
					CPolyObj    L_collisionInfo[],//collision data per object
					int         L_faceCount,
					D3DVECTOR   L_meshActualCenter,
			        float       L_meshActualRadius,
					BOOL        L_saveCollisionInfo,
					D3DVERTEX   L_vertexArray[],
					WORD        L_vertexArrayCount
					);

	D3DVERTEX * List_vertexArray;
	WORD         List_vertexArrayCount;
    BOOL        List_saveCollisionInfo;
	D3DVECTOR   List_meshActualCenter;
	float       List_meshActualRadius;
	int List_faceCount;
    int List_vCount;
	CPolyObj * List_collisionInfo;
	D3DVECTOR List_translationXYZ;//translation animation XYZ of the vector
	D3DVECTOR List_scaleXYZ;//translation animation XYZ of the vector
	D3DVECTOR List_rotateXYZ;//tranlation rotation animation of the vector

	void Serialize( CArchive& ar );
	void SafeDelete();
};

class  CVertexTransObjList : public CObList
{
public:
   CVertexTransObjList(){};
    
     
	DECLARE_SERIAL( CVertexTransObjList );
	
    
};

//animation obj
class  CAnimationObj : public CObject
{
	CAnimationObj(){}
	
     
	DECLARE_SERIAL( CAnimationObj );
	
    
public:
	
	CAnimationObj(int L_type,//0 = vertex or 1 = translation animation (which one to use) others null
		          int L_howManySteps,//how many animation steps total
		          int L_looping,//does the animation contiually loop 1= back and forth 2 = reset to step 0 
		          int L_currentPosition,//position in animation
				  CVertexTransObjList * L_animContent,//vertex array or traslation
				  int L_action,//0=dormant 1=forward 2=backwards
				  CString L_animationName,//name for animation
				  int L_lastVisUpdatedPosition,//so u can compensate translation and optimize vertex anim
				  int L_animTuningVarial,//used for slowing and speeding animations
				  int L_animTuning,//used for slowing and speeding animations
				  int L_attrib,//attribute to specify walking/ect or not specific
				  IDirectSoundBuffer     *L_baseSoundBuffer,
				  IDirectSound3DBuffer   *L_buffer3DControl,
				  CString                 L_soundFileName,
				  BOOL                    L_useSound,
				  float                   L_soundFactor,
				  BOOL                    L_isPlaying,
				  BOOL                    L_useInterpolation,
				  D3DVERTEX               L_tempVertexDBIM[]
				  );

	BOOL                    List_finishAnim;
	int                     List_currentTimeLapse;
	int                     List_lastAnimationTime;
	D3DVECTOR               List_trans_BasedSpeed;
	D3DVECTOR               List_trans_Destination;
	D3DVECTOR               List_rotate_BasedSpeed;
	D3DVECTOR               List_rotate_Destination;
	int                     List_trasnsformConditional;
	D3DVERTEX                 *List_tempVertexDBIM;
	BOOL                       List_useInterpolation;
	IDirectSoundBuffer        *List_baseSoundBuffer;
    IDirectSound3DBuffer      *List_buffer3DControl;
	CString                    List_soundFileName;
	BOOL                       List_useSound;
	float                      List_soundFactor;
	BOOL                       List_isPlaying;
	int                        List_attrib;
	int                        List_animTuningVarial;
	int                        List_animTuning;
	CString                    List_animationName;//name for animation
	int                        List_action;
	int                        List_currentPosition;//position in animation of current animation
	CVertexTransObjList *      List_animContent;//list of modifications/animations to do to object
    int                        List_looping;
	int                        List_type;//0 = vertex or 1 = translation animation (which one to use) others null
	int                        List_howManySteps;//how many animation steps total
	int                        List_lastVisUpdatedPosition;//so u can compensate translation and optimize vertex anim
    
    //saveing function
	void Serialize( CArchive& ar );
	void SafeDelete();

};

class  CAnimationObjList : public CObList
{
public:
   CAnimationObjList(){};
   BOOL ResetAll();
   BOOL AnimationAttribTest(int attrib);
   CAnimationObj * RetrieveAnimationObj(int attrib,int * indexOfAnim);
    
     
	DECLARE_SERIAL( CAnimationObjList );
	
    
};



#endif