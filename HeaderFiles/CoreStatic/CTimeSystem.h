
#include "CEXMeshClass.h"
#include "CDayStateClass.h"
#include "CStormClass.h"
#include "matrixarb.h"
#include "CLightningEffectClass.h"
#include "CABFunctionLib.h"
#include "CSoundManagerClass.h"

#ifndef CTIMESYSTEMCLASS_H
#define CTIMESYSTEMCLASS_H



class   CTimeSystemObj : public CObject
{
	
     
	DECLARE_SERIAL( CTimeSystemObj );
	
    
public:
    CTimeSystemObj();
    
	CABFunctionLib m_cablib;
    MatrixARB   m_abMathLib;

    DWORD       m_timeStamp;
	long        m_currentSystemDay;
	long        m_currentSystemMonth;
	long        m_currentSystemYear;
	long        m_currentDayTime;
    
	//cycle variable
	long        m_monthsPerYear;
	long        m_daysPerMonth;

	//Generate values

	long        m_millisecondsPerDay;
 
    CDayStateObj *     m_currentState;

	//sun visual
	CEXMeshObj   *     m_visual;
	D3DVECTOR          m_currentSunPosition;


	//currentStorm if applicable
    BOOL               m_stormInUse;
    CStormObj         *m_stormPtr;
	long               m_frequencyStormCheck;
	long               m_stormCheckTimeStamp;
	CDayStateObjList * m_dayStateList;
    CStormObjList    * m_stormDB;

  
	void AddStorm(long duration,
				  long frequency,
				  CString stormName,
				  long stormTransition,
				  int particleSysInt);
	void Serialize( CArchive& ar );
	void SafeDelete();
	void AddDayState(float minutes,
					 float fogStartRange,
					 float fogEndRange,
					 float fogColorRed,
					 float fogColorGreen,
					 float fogColorBlue,
					 float sunColorRed,
					 float sunColorGreen,
					 float sunColorBlue,
					 float ambientColorRed,
					 float ambientColorGreen,
					 float ambientColorBlue,
					 float sunRotationX,
					 float sunRotationY,
					 float sunDistance);
	void UpdateSystem(long currentTime,
		              CLightningObjList * lightningEffectSystem,
					  D3DMATRIX soundCenter,
					  CSoundBankList * soundManager);
	void StartSystem(long currentTime,long totalTimePassed);
	
};

#endif