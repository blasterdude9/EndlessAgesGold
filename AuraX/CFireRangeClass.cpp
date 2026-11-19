#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d9.h>
#include <afxtempl.h>
#include "CFireRangeClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

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
	m_accuracyRating              = 0.0f;
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
	 copyLocal->m_accuracyRating              = m_accuracyRating;

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

CFireRangeObject * CFireRangeObjectList::GetByMissileIndex(int & missileIndex)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CFireRangeObject * spnPtr = (CFireRangeObject *)GetNext(posLoc);
		if(spnPtr->m_missileIndex == missileIndex)
		   return spnPtr;
	}
	return NULL;
}

CFireRangeObject *CFireRangeObjectList::GetByFireQueIndex(int &queIndex)
{
	int index = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
		CFireRangeObject *spnPtr = (CFireRangeObject *)GetNext(posLoc);
		if(index == queIndex)
			return spnPtr;
		index++;
	}
	return NULL;
}

void CFireRangeObject::Serialize( CArchive& ar )
{
BOOL reserved = 0;
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
       ar.SetObjectSchema(3);
       ASSERT(ar.GetObjectSchema() == 3);

	   ar << m_animationIndex
		  << m_animationFrameRangeStart
		  << m_animationFrameRangeEnd
		  << m_boneIndex
		  << m_missileIndex
		  << m_hardPointAdjust.x
 	      << m_hardPointAdjust.y
	      << m_hardPointAdjust.z;

	   ar << reserved
	      << reserved
		  << m_accuracyRating;
	   
	}
	else
	{
       int version = ar.GetObjectSchema();

       ar >> m_animationIndex
		  >> m_animationFrameRangeStart
		  >> m_animationFrameRangeEnd
		  >> m_boneIndex
		  >> m_missileIndex
		  >> m_hardPointAdjust.x
 	      >> m_hardPointAdjust.y
	      >> m_hardPointAdjust.z;

       m_accuracyRating = 0.0f;

	   if(version == 2)
	   {
         ar >> reserved
	        >> reserved
		    >> m_accuracyRating;
	   }

	   m_inRangeCurrently            = FALSE;
	}
}     


IMPLEMENT_SERIAL(CFireRangeObject, CObject, VERSIONABLE_SCHEMA|2)
IMPLEMENT_SERIAL(CFireRangeObjectList, CObList, 0)

