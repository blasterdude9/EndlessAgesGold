#include "CSSoundRange.h"
#ifndef CBONEANIMATIONCLASS_H
#define CBONEANIMATIONCLASS_H


class  mARBFloat
{
   public:
   mARBFloat(){m_cVal=0;}
   short m_cVal;

   //float GetFloat(){return ((float)m_cVal)*.008f;}
   //mARBFloat & operator=( float & temp ){m_cVal = (char)(temp*100.0f);return *this;}
   //mARBFloat & operator=( float temp ){m_cVal = (char)(temp*125.0f);return *this;}
   float GetFloat(){return ((float)m_cVal)*.0001f;}
   mARBFloat & operator=( float temp ){m_cVal = (short)(temp*10000.0f);return *this;}
};

class  mARBVect
{
   public:
   mARBVect(){};
   mARBFloat x,y,z;
   short GetXcompressed(){return x.m_cVal;}
   short GetYcompressed(){return y.m_cVal;}
   short GetZcompressed(){return z.m_cVal;}
   float GetX(){return x.GetFloat();}
   float GetY(){return y.GetFloat();}
   float GetZ(){return z.GetFloat();}
   D3DVECTOR GetD3DVECTOR(){D3DVECTOR temp; 
                            temp.x = x.GetFloat();
							temp.y = y.GetFloat();
							temp.z = z.GetFloat();
							return temp;}
   //mARBVect & operator=( D3DVECTOR &temp ){return *this;}
   mARBVect & operator=( D3DVECTOR temp ){x = temp.x;y = temp.y;z = temp.z;return *this;}
};




struct AnimationContent {
	mARBVect       upVect;
	mARBVect       dirVect;
	D3DVECTOR      position;
	float          visibility;
};

//Base skeleton Class
class  CBoneAnimationObject : public CObject
{
	
     
    DECLARE_SERIAL( CBoneAnimationObject )
    
    
public:
	
	CBoneAnimationObject();
    //animation header
	MatrixARB           GL_matrixFunctions;
    int                 m_totalFrames;
    //int                 m_currentPosition;
	int                 m_millisecondsPerFrame;
	//int                 m_baseFrame;
    //int                 m_targetFrame;
	//long                m_currentMS;
	BOOL                m_looping;
	BOOL                m_timingBased;//0=speed
	long                m_timingCapMax;
	long                m_timingCapMin;
	//BOOL                m_inReverse;
	CString             m_animationName;
	int                 m_type;
	//int                 m_lastAnimBaseFrame;
	//int                 m_lastAnimTargetFrame;
	int                 m_version;

	int                 m_minPlayDuration;


    //long                m_currentTimePassed;//tempvar
	//long                m_lastUpdatedTime;//tempVar
	BOOL                m_deleteAnimData;

	//positional animation
	AnimationContent *  m_animationData;
	//sound ranges
    CSoundRangeObjectList * m_soundRanges;

    //functions
	D3DVECTOR GetBonePositionalDifference(D3DVECTOR currentBonePosition);
	void Clone(CBoneAnimationObject ** clone);
	void CloneRaw(CBoneAnimationObject ** clone);
	BOOL SetSoundActivationEvent(				   CString & fileName,
												   long start,
												   long end,
												   int id,
												   float soundRange,
												   float soundClipRadius,
												   BOOL velocityBased,
												   BOOL looping);
	BOOL        LoadAnimationData(CString FileName,CString nodename);
	BOOL        RetrieveAnimationSpeedLength(CString fileName);
	 void GetBonePositionalDataByTime(long        & currentPosition,
											D3DVECTOR   & position,
											D3DVECTOR   & upVector,
											D3DVECTOR   & dirVector);
	D3DVECTOR   GetPositionFromString(CString string);
	void        SafeDelete();
	void        Serialize( CArchive& ar );
};

class  CBoneAnimationList : public CObList
{
public:
    void Clone(CBoneAnimationList ** clone);
   void SafeDelete();
   CBoneAnimationList(){};
   CBoneAnimationObject * GetByType(int type);
    
     
    DECLARE_SERIAL( CBoneAnimationList )
    
    
};

#endif