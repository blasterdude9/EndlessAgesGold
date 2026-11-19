//#include "stdafx.h"
#include "afxwin.h"
#include "afxext.h"
#define D3D_OVERLOADS
#include "resource.h"
#include <d3d9.h>
//#include <afxtempl.h>

#include "CFireRangeClass.h"

CFireRangeObject::CFireRangeObject()
{
    m_animationIndex              = 0;
	m_animationFrameRangeStart    = 0;
	m_animationFrameRangeEnd      = 3;
	m_boneIndex                   = 1;
	m_missileIndex                = 0;
	m_inRangeCurrently            = FALSE;
	m_hardPointAdjust.x           = 0.0f;
	m_hardPointAdjust.y           = 0.0f;
	m_hardPointAdjust.z           = 0.0f;
}

void CFireRangeObject::Clone(CFireRangeObject ** clone)
{
     CFireRangeObject * copyLocal = new CFireRangeObject();
     copyLocal->m_animationIndex              = m_animationIndex;
	 copyLocal->m_animationFrameRangeStart    = m_animationFrameRangeStart;
	 copyLocal->m_animationFrameRangeEnd      = m_animationFrameRangeEnd;
	 copyLocal->m_boneIndex                   = m_boneIndex;
	 copyLocal->m_missileIndex                = m_missileIndex;
	 copyLocal->m_inRangeCurrently            = FALSE;

     CopyMemory(&copyLocal->m_hardPointAdjust,&m_hardPointAdjust,sizeof(D3DVECTOR));

     *clone = copyLocal; 
}

void CFireRangeObjectList::Clone(CFireRangeObjectList ** clone)
{
     CFireRangeObjectList * copyLocal = new CFireRangeObjectList();
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
         CFireRangeObject * spnPtr = (CFireRangeObject *)GetNext(posLoc);
	     CFireRangeObject * clonedPtr = NULL;
		 spnPtr->Clone(&clonedPtr);
		 copyLocal->AddTail(clonedPtr);
	 }

	 *clone = copyLocal;
}

void CFireRangeObjectList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CFireRangeObject * spnPtr = (CFireRangeObject *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}


void CFireRangeObject::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_animationIndex
		  << m_animationFrameRangeStart
		  << m_animationFrameRangeEnd
		  << m_boneIndex
		  << m_missileIndex
		  << m_hardPointAdjust.x
 	      << m_hardPointAdjust.y
	      << m_hardPointAdjust.z;
	}
	else
	{
       ar >> m_animationIndex
		  >> m_animationFrameRangeStart
		  >> m_animationFrameRangeEnd
		  >> m_boneIndex
		  >> m_missileIndex
		  >> m_hardPointAdjust.x
 	      >> m_hardPointAdjust.y
	      >> m_hardPointAdjust.z;

	   m_inRangeCurrently            = FALSE;
	}
}     


IMPLEMENT_SERIAL(CFireRangeObject, CObject, 0)
IMPLEMENT_SERIAL(CFireRangeObjectList, CObList, 0)

