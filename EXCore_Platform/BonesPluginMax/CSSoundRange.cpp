//#include "stdafx.h"
#include "afxwin.h"
#include "afxext.h"
#define D3D_OVERLOADS
#include "resource.h"
#include <d3d9.h>
//#include <afxtempl.h>
#include "matrixarb.h"
#include "direct.h"
#include "CSSoundRange.h"
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
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------Deconstructor----------------------------------//
void CSoundRangeObject::SafeDelete()
{
  
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
		  << reserved
		  << reserved
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
		  >> reserved
		  >> reserved
		  >> reserved;
  
	   //m_id = 0;
	   //m_soundFactor = .01f;
    m_inRange = FALSE;
	//m_radiusClipping = 500.0f;
	}
}

IMPLEMENT_SERIAL(CSoundRangeObject, CObject, 0)
IMPLEMENT_SERIAL(CSoundRangeObjectList, CObject, 0)
