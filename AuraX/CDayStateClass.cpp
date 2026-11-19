#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>

#include "CDayStateClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDayStateObj::CDayStateObj()
{
    m_fogStartRange     = 800;
    m_fogEndRange       = 3000;
    m_fogColorRed       = 0.0f;
    m_fogColorGreen     = 0.0f;
    m_fogColorBlue      = 0.0f;
    m_sunRotationX      = 0.0f;
    m_sunRotationY      = 0.0f;
    m_sunDistance       = 0;
    m_sunColorRed       = 0.0f;
    m_sunColorGreen     = 0.0f;
    m_sunColorBlue      = 0.0f;
	m_materialModifier  = NULL;
    m_ambientColorRed   = 0.0f;
    m_ambientColorGreen = 0.0f;
    m_ambientColorBlue  = 0.0f;
	m_duration          = 0;//in milliseconds
}


void CDayStateObj::SafeDelete()
{
	if(m_materialModifier)
	   delete m_materialModifier;
	   m_materialModifier = 0;
}

BOOL CDayStateObjList::GetCurrentDayState(long currentDayTime,
										  CDayStateObj ** currentState,
										  CDayStateObj ** targetState,
										  float * currentDuration,
										  float * currentTimePassed
										  )
{
	long timePosition = 0;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CDayStateObj * spnPtr = (CDayStateObj *)GetNext(posLoc);
		if(currentDayTime < (spnPtr->m_duration+timePosition))
		{
			*currentState = spnPtr;
			
			if(posLoc)
			    *targetState = (CDayStateObj *)GetAt(posLoc);
            else
                *targetState = (CDayStateObj *)GetHead();

            *currentDuration      = (float)spnPtr->m_duration;
			*currentTimePassed    = (float)(spnPtr->m_duration - ((spnPtr->m_duration+timePosition) - currentDayTime)); 

			return TRUE;
		}
		timePosition += spnPtr->m_duration;
	}
	return FALSE;
}

CDayStateObjList::CDayStateObjList()
{
  m_lastDayState = NULL;
}

void CDayStateObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CDayStateObj * spnPtr = (CDayStateObj *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

long CDayStateObjList::GetCompleteCycleLength()
{
	long total = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CDayStateObj * spnPtr = (CDayStateObj *)GetNext(posLoc);
	    total += spnPtr->m_duration;
	}
	return total;
}

void CDayStateObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_fogStartRange
	      << m_fogEndRange
	      << m_fogColorRed
	      << m_fogColorGreen
	      << m_fogColorBlue
		  << m_materialModifier
		  << m_sunRotationX
          << m_sunRotationY
          << m_sunDistance
          << m_sunColorRed 
          << m_sunColorGreen 
          << m_sunColorBlue
	      << m_ambientColorRed
          << m_ambientColorGreen
          << m_ambientColorBlue
		  << m_duration;
	}
	else
	{
       ar >> m_fogStartRange
	      >> m_fogEndRange
	      >> m_fogColorRed
	      >> m_fogColorGreen
	      >> m_fogColorBlue
		  >> m_materialModifier
		  >> m_sunRotationX
          >> m_sunRotationY
          >> m_sunDistance
          >> m_sunColorRed 
          >> m_sunColorGreen 
          >> m_sunColorBlue
		  >> m_ambientColorRed
          >> m_ambientColorGreen
          >> m_ambientColorBlue
		  >> m_duration;
	}
}     


IMPLEMENT_SERIAL(CDayStateObj, CObject, 0)
IMPLEMENT_SERIAL(CDayStateObjList, CObList, 0)

