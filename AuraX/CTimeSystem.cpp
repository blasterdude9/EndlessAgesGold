#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CTimeSystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CTimeSystemObj::CTimeSystemObj()
{
    m_timeStamp           = 0;
    m_currentSystemDay    = 0;
    m_currentSystemMonth  = 0;
    m_currentSystemYear   = 0;
    m_currentDayTime      = 0;
    
	//cycle variable
    m_monthsPerYear       = 12;
    m_daysPerMonth        = 30;

	//Generate values
    m_millisecondsPerDay  = 0;
	m_dayStateList        = NULL;
	m_currentState        = NULL;
	m_visual              = NULL;

	m_currentSunPosition.x = 0.0f;
	m_currentSunPosition.y = 0.0f;
	m_currentSunPosition.z = 0.0f;


    m_stormInUse          = FALSE;
    m_stormPtr            = NULL;
	m_frequencyStormCheck = 5000;
	m_stormCheckTimeStamp = 0;

	m_stormDB             = NULL;
}

void CTimeSystemObj::SafeDelete()
{
    if(m_dayStateList)
	{
	   m_dayStateList->SafeDelete();
	   delete m_dayStateList;
	   m_dayStateList=0;
	}

    if(m_currentState)
	{
       m_currentState->SafeDelete();
       delete m_currentState;
	   m_currentState = 0;
	}

	if(m_visual)
	{
       m_visual->SafeDelete();
	   delete m_visual;
	   m_visual = 0;
	}

	if(m_stormDB)
	{
	   m_stormDB->SafeDelete();
       delete m_stormDB;
       m_stormDB = 0;
	}
}

void CTimeSystemObj::AddDayState(float minutes,
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
								 float sunDistance)
{
    CDayStateObj * statePtr        = new CDayStateObj();
    statePtr->m_duration           = (long)(60000.0f * minutes);
	statePtr->m_fogColorBlue       = fogColorBlue;
    statePtr->m_fogColorGreen      = fogColorGreen;
	statePtr->m_fogColorRed        = fogColorRed;
	statePtr->m_fogEndRange        = fogEndRange;
	statePtr->m_sunColorRed        = sunColorRed;
	statePtr->m_sunColorGreen      = sunColorGreen;
	statePtr->m_sunColorBlue       = sunColorBlue;
	statePtr->m_ambientColorRed    = ambientColorRed;
	statePtr->m_ambientColorGreen  = ambientColorGreen;
	statePtr->m_ambientColorBlue   = ambientColorBlue;
	statePtr->m_sunRotationX       = sunRotationX;
	statePtr->m_sunRotationY       = sunRotationY;
	statePtr->m_sunDistance        = (long)sunDistance;

    if(!m_dayStateList)
		m_dayStateList = new CDayStateObjList();

	m_dayStateList->AddTail(statePtr);
}


void CTimeSystemObj::StartSystem(long currentTime,long totalTimePassed)
{
     m_timeStamp           = currentTime;
	 m_currentDayTime      = 0;
	 m_stormCheckTimeStamp = currentTime;


	 if(m_dayStateList){


		m_millisecondsPerDay = m_dayStateList->GetCompleteCycleLength();
		if(m_millisecondsPerDay > 0)
		{
		  if(totalTimePassed > m_millisecondsPerDay)
		  {
           long uneededTime = (totalTimePassed / m_millisecondsPerDay)*m_millisecondsPerDay;
		   long timePassedThisDay = totalTimePassed - uneededTime;
		   m_currentDayTime = timePassedThisDay;
		   //m_timeStamp -= timePassedThisDay;
           //m_stormCheckTimeStamp -= timePassedThisDay;
		  }
		  else{
		   m_currentDayTime = totalTimePassed;
           ////m_timeStamp -= totalTimePassed;
           //m_stormCheckTimeStamp -= totalTimePassed;
		  }
		}
	 }
	 
}

void CTimeSystemObj::AddStorm(long duration,
							  long frequency,
							  CString stormName,
							  long stormTransition,
							  int particleSysInt)
{
  if(!m_stormDB)
	  m_stormDB = new CStormObjList();


  CStormObj * strmPtr             = new CStormObj();
  strmPtr->m_stormTransition      = stormTransition;
  strmPtr->m_stormName            = stormName;
  strmPtr->m_particleSystemRef    = particleSysInt;
  strmPtr->m_frequency            = frequency;
  strmPtr->m_duration             = duration;
  m_stormDB->AddTail(strmPtr);
}

void CTimeSystemObj::UpdateSystem(long currentTime,
								  CLightningObjList * lightningEffectSystem,
								  D3DMATRIX soundCenter,
								  CSoundBankList * soundManager)
{
     m_currentDayTime += (currentTime - m_timeStamp);
	 m_timeStamp = currentTime;
	 
	 //evaluate storms
	 if(!m_stormPtr)
	 {//storm not in use get random storm if random succeeds
	  if((currentTime - m_stormCheckTimeStamp) > m_frequencyStormCheck)
	  {
	     if(m_stormDB)
		 {
			for(POSITION posLoc = m_stormDB->GetHeadPosition();posLoc != NULL;)
			{
	            CStormObj * strmPtr = (CStormObj *)m_stormDB->GetNext(posLoc);
                if((rand()%100) < strmPtr->m_frequency)
				{
                   strmPtr->Clone(&m_stormPtr);
				   m_stormCheckTimeStamp = currentTime;
				   break;
				}
			}
		 }
		 m_stormCheckTimeStamp = currentTime;
	  }
	 }//end storm not in use get random storm if random succeeds
	 else{//storm in use
        //manage lightning strikes
		if(m_stormPtr->m_totalStrikeCountPerMinute > 0)
		if(m_stormPtr->m_VariationLightningCount > 0)
		{//strike is possible
         if((currentTime - m_stormPtr->m_lightningStamp) > m_stormPtr->m_strikeDuration)
		 {//strike check
		  int percentage = 100/(100/m_stormPtr->m_totalStrikeCountPerMinute);
		 
		  if((rand()%100) < percentage)
		  {//strike succeeded
			int strikeSelection = rand()%m_stormPtr->m_VariationLightningCount;
			if(strikeSelection == m_stormPtr->m_VariationLightningCount)
			   strikeSelection = m_stormPtr->m_VariationLightningCount-1;
			float stormRadius = m_stormPtr->m_stormRadius;
			D3DVECTOR spawnPosition;
            spawnPosition.x = 0.0f;
			spawnPosition.y = 0.0f;
            spawnPosition.z = 0.0f;
			spawnPosition = m_cablib.GetRandomPointInRadius(stormRadius,spawnPosition,(stormRadius * .5f));
			
			
			spawnPosition.y += m_stormPtr->m_strikeSpawnHieght; 
			
		    lightningEffectSystem->AddLightningObject(spawnPosition,
				                                      m_stormPtr->m_strikePanelWidth,
													  m_stormPtr->m_strikeDepth,
                                                      m_stormPtr->m_strikeVariationArray[strikeSelection].textureByIndex,
                                                      1000,
													  TRUE);

	
			D3DVECTOR localPosition = m_abMathLib.UpdatePolygonVertexRotationOnly(soundCenter.m,spawnPosition);
			soundManager->PlayByID(m_stormPtr->m_strikeVariationArray[strikeSelection].m_sndID,
				                   FALSE,
								   localPosition,
			                       m_stormPtr->m_strikeVariationArray[strikeSelection].m_soundFactor,
								   50000.0f);
			
		  }//end strick succeeded
		  m_stormPtr->m_lightningStamp = currentTime;
		 }//end strike check
		}//end strike is possible
		//end manage lightning strikes
		if((currentTime - m_stormCheckTimeStamp) > m_stormPtr->m_duration)
		{//storm timed up
           m_stormPtr->SafeDelete();
		   delete m_stormPtr;
		   m_stormPtr=0;
		}//end storm timed up
	 }//end storm in use
	 //end evaluate storms
     if(!m_dayStateList)//state database
		 return;

	 if(m_currentDayTime >= m_millisecondsPerDay)
	 {//day
        m_currentSystemDay++;
		m_currentDayTime = 0;
	 }//end day
     if(m_currentSystemDay >= m_daysPerMonth)
	 {//month
        m_currentSystemDay = 0;
        m_currentSystemMonth++;
	 }//end month
	 if(m_currentSystemMonth >= m_monthsPerYear)
	 {//year
		m_currentSystemMonth = 0;
        m_currentSystemYear++;
	 }//end year

     if(!m_currentState)
         m_currentState = new CDayStateObj();

         float m_currentDuration   = 0.0f;
		 float m_currentTimePassed = 0.0f;

	     CDayStateObj * curState    = NULL;
		 CDayStateObj * targetState = NULL;

	     BOOL result = m_dayStateList->GetCurrentDayState(m_currentDayTime,
						                                  &curState,
						                                  &targetState,
							                              &m_currentDuration,
						                                  &m_currentTimePassed
		                                                  );

		 if(result)
		 {
			 if(m_currentTimePassed > m_currentDuration)
				m_currentTimePassed = m_currentDuration;
			    //fog
                m_currentState->m_fogColorBlue = m_abMathLib.InterpolateFloatsHiRes(curState->m_fogColorBlue,targetState->m_fogColorBlue,m_currentDuration,m_currentTimePassed);
                m_currentState->m_fogColorRed = m_abMathLib.InterpolateFloatsHiRes(curState->m_fogColorRed,targetState->m_fogColorRed,m_currentDuration,m_currentTimePassed);
				m_currentState->m_fogColorGreen = m_abMathLib.InterpolateFloatsHiRes(curState->m_fogColorGreen, targetState->m_fogColorGreen,m_currentDuration,m_currentTimePassed);
				m_currentState->m_fogEndRange = m_abMathLib.InterpolateFloatsHiRes(curState->m_fogEndRange,targetState->m_fogEndRange,m_currentDuration,m_currentTimePassed);
				m_currentState->m_fogStartRange = m_abMathLib.InterpolateFloatsHiRes(curState->m_fogStartRange,targetState->m_fogStartRange,m_currentDuration,m_currentTimePassed);
                //end fog
                //sun
                m_currentState->m_sunColorBlue = m_abMathLib.InterpolateFloatsHiRes(curState->m_sunColorBlue,targetState->m_sunColorBlue,m_currentDuration,m_currentTimePassed);
                m_currentState->m_sunColorRed = m_abMathLib.InterpolateFloatsHiRes(curState->m_sunColorRed,targetState->m_sunColorRed,m_currentDuration,m_currentTimePassed);
				m_currentState->m_sunColorGreen = m_abMathLib.InterpolateFloatsHiRes(curState->m_sunColorGreen, targetState->m_sunColorGreen,m_currentDuration,m_currentTimePassed);
				m_currentState->m_sunDistance = (long)m_abMathLib.InterpolateFloatsHiRes((float)curState->m_sunDistance,(float)targetState->m_sunDistance,m_currentDuration,m_currentTimePassed);
				m_currentState->m_sunRotationX = m_abMathLib.InterpolateFloatsHiRes(curState->m_sunRotationX,targetState->m_sunRotationX,m_currentDuration,m_currentTimePassed);
                m_currentState->m_sunRotationY = m_abMathLib.InterpolateFloatsHiRes(curState->m_sunRotationY,targetState->m_sunRotationY,m_currentDuration,m_currentTimePassed);
				//end sun
                //Ambient Color
                m_currentState->m_ambientColorRed = m_abMathLib.InterpolateFloatsHiRes(curState->m_ambientColorRed,targetState->m_ambientColorRed,m_currentDuration,m_currentTimePassed);
                m_currentState->m_ambientColorGreen = m_abMathLib.InterpolateFloatsHiRes(curState->m_ambientColorGreen,targetState->m_ambientColorGreen,m_currentDuration,m_currentTimePassed);
				m_currentState->m_ambientColorBlue = m_abMathLib.InterpolateFloatsHiRes(curState->m_ambientColorBlue, targetState->m_ambientColorBlue,m_currentDuration,m_currentTimePassed);
				//end Ambient Color

		 }		
}

void CTimeSystemObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_timeStamp
          << m_currentSystemDay
          << m_currentSystemMonth
          << m_currentSystemYear
          << m_currentDayTime
          << m_monthsPerYear
          << m_daysPerMonth
          << m_millisecondsPerDay
          << m_dayStateList
		  << m_visual
		  << m_frequencyStormCheck
		  << m_stormDB;

	}
	else
	{
       ar >> m_timeStamp
          >> m_currentSystemDay
          >> m_currentSystemMonth
          >> m_currentSystemYear
          >> m_currentDayTime
          >> m_monthsPerYear
          >> m_daysPerMonth
          >> m_millisecondsPerDay
          >> m_dayStateList
		  >> m_visual
		  >> m_frequencyStormCheck
		  >> m_stormDB;

	   m_currentSunPosition.x = 0.0f;
	   m_currentSunPosition.y = 0.0f;
	   m_currentSunPosition.z = 0.0f;

	   if(m_dayStateList)
		  m_dayStateList->m_lastDayState = NULL;

	   m_stormInUse          = FALSE;
       m_stormPtr            = NULL;
       m_stormCheckTimeStamp = 0;
	}
}     


IMPLEMENT_SERIAL(CTimeSystemObj, CObject, 0)





