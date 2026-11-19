#ifndef CDAYSTATECLASS_H
#define CDAYSTATECLASS_H

#include "CMaterialClass.h"


class  CDayStateObj : public CObject
{
	
     
	DECLARE_SERIAL( CDayStateObj );
	
    
public:
    CDayStateObj();
    
	//fog
    float m_fogStartRange;
	float m_fogEndRange;
	float m_fogColorRed;
	float m_fogColorGreen;
	float m_fogColorBlue;

	//length of day section
	long m_duration;//in milliseconds

	//lighting sun ect
    float           m_sunRotationX;//enviormental center
	float           m_sunRotationY;//enviormental center
    long            m_sunDistance;//vector distance from center
	float           m_sunColorRed;//0 is basically sun off no color
	float           m_sunColorGreen;//0 is basically sun off no color
	float           m_sunColorBlue;//0 is basically sun off no color
	float           m_ambientColorRed;
	float           m_ambientColorGreen;
	float           m_ambientColorBlue;

	//modifiers
	CMaterialObject * m_materialModifier;
  
	void Serialize( CArchive& ar );
	void SafeDelete();
	
};

class  CDayStateObjList : public CObList
{
public:
   CDayStateObjList();
   void SafeDelete();
   
    
     
	DECLARE_SERIAL( CDayStateObjList );
	
    

    CDayStateObj * m_lastDayState;

	long GetCompleteCycleLength();
	CDayStateObj * GetCurrentDayState(long currentDayTime);
	BOOL GetCurrentDayState(long currentDayTime,
						    CDayStateObj ** currentState,
						    CDayStateObj ** targetState,
						    float * currentDuration,
						    float * currentTimePassed);
};

#endif