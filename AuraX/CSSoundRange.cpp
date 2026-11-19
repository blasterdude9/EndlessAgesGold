#include "stdafx.h"

#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "matrixarb.h"
#include "direct.h"
#include "CSSoundRange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Constructor-----------------------------------//
CSoundRangeObject::CSoundRangeObject()
{
    m_startRange = 0;
    m_endRange = 0;
    m_inRange = FALSE;
	m_fileName = "NONE";
	m_id = 0;
	m_soundFactor = .01f;
	m_radiusClipping = 500.0f;
	m_velocityBased = FALSE;
	m_looping = FALSE;
	m_soundRef = NULL;//only use for maintaining loops
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------Deconstructor----------------------------------//
void CSoundRangeObject::SafeDelete()
{
    if(m_soundRef)
	   m_soundRef->SetToDelete();
	m_soundRef = NULL;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------Deconstructor----------------------------------//
void CSoundRangeObject::Clone(CSoundRangeObject ** clone)
{
     CSoundRangeObject * clonePtr = new CSoundRangeObject();
	 //base data
	 clonePtr->m_startRange     = m_startRange;
     clonePtr->m_endRange       = m_endRange;
     clonePtr->m_inRange        = m_inRange;
	 clonePtr->m_fileName       = m_fileName;
	 clonePtr->m_id             = m_id;
	 clonePtr->m_soundFactor    = m_soundFactor;
	 clonePtr->m_radiusClipping = m_radiusClipping;
	 clonePtr->m_velocityBased	= m_velocityBased;
	 clonePtr->m_looping		= m_looping;
	 *clone = clonePtr;

}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Constructor-----------------------------------//
void CSoundRangeObject::SetRanges(long start, long end)
{
m_startRange = start;
m_endRange = end;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------Deconstructor----------------------------------//
void CSoundRangeObjectList::Clone(CSoundRangeObjectList ** clone)
{
	CSoundRangeObjectList * cloneList = new CSoundRangeObjectList;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CSoundRangeObject * sndRang = (CSoundRangeObject *)GetNext(posLoc);
        CSoundRangeObject * clonedObject = NULL;
		sndRang->Clone(&clonedObject);
		cloneList->AddTail(clonedObject);
	}
	*clone = cloneList;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------Deconstructor----------------------------------//
void CSoundRangeObjectList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CSoundRangeObject * sndRang = (CSoundRangeObject *)GetNext(posLoc);
        sndRang->SafeDelete();
		delete sndRang;
		sndRang = 0;
	}
	RemoveAll();
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Serialize-------------------------------------//
void CSoundRangeObject::Serialize( CArchive& ar )
{
BOOL reserved = 0;
CObject::Serialize( ar );
	if(ar.IsStoring() ){

	   ar << m_startRange 
		  << m_endRange
		  << m_fileName
		  << m_id
		  << m_soundFactor
		  << m_radiusClipping
		  << m_velocityBased
		  << m_looping
		  << reserved;
		 
	}
	else
	{
       ar >> m_startRange 
		  >> m_endRange
		  >> m_fileName
		  >> m_id
		  >> m_soundFactor
		  >> m_radiusClipping
		  >> m_velocityBased
		  >> m_looping
		  >> reserved;
  
	   //m_id = 0;
	   //m_soundFactor = .01f;


    m_inRange = FALSE;
	m_soundRef = NULL;
	//m_radiusClipping = 500.0f;
	}
}

IMPLEMENT_SERIAL(CSoundRangeObject, CObject, 0)
IMPLEMENT_SERIAL(CSoundRangeObjectList, CObject, 0)
