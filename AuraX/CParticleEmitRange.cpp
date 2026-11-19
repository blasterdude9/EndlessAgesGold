#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "matrixarb.h"
#include "direct.h"

#include "CParticleEmitRange.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Constructor-----------------------------------//
CParticleEmitRangeObj::CParticleEmitRangeObj()
{
    m_animationQue      = 0;
	m_startRange        = 0;
	m_endRange          = 0;
	m_particleDBIndex   = 0;
	m_boneLink          = 0;
	m_useExactRange     = FALSE;
	m_inRange           = FALSE;
	m_particleEmmiter   = NULL;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------Deconstructor----------------------------------//
void CParticleEmitRangeObj::SafeDelete()
{
    if(m_particleEmmiter){//no need to del
       m_particleEmmiter->m_Regenerate = FALSE;
	   m_particleEmmiter = 0;
	}//end no need to del
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------Deconstructor----------------------------------//
void CParticleEmitRangeObj::Clone(CParticleEmitRangeObj ** clone)
{
     CParticleEmitRangeObj * clonePtr = new CParticleEmitRangeObj();
	 //base data
	 clonePtr->m_animationQue      = m_animationQue;
	 clonePtr->m_startRange        = m_startRange;
	 clonePtr->m_endRange          = m_endRange;
	 clonePtr->m_particleDBIndex   = m_particleDBIndex;
	 clonePtr->m_boneLink          = m_boneLink;
	 clonePtr->m_inRange           = FALSE;
	 clonePtr->m_useExactRange     = m_useExactRange;
	 
	 *clone = clonePtr;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Constructor-----------------------------------//
void CParticleEmitRangeObj::SetRanges(long start, long end)
{
m_startRange = start;
m_endRange = end;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------Deconstructor----------------------------------//
void CParticleEmitRangeObjList::Clone(CParticleEmitRangeObjList ** clone)
{
	CParticleEmitRangeObjList * cloneList = new CParticleEmitRangeObjList;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CParticleEmitRangeObj * ptRang = (CParticleEmitRangeObj *)GetNext(posLoc);
        CParticleEmitRangeObj * clonedObject = NULL;
		ptRang->Clone(&clonedObject);
		cloneList->AddTail(clonedObject);
	}
	*clone = cloneList;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------Deconstructor----------------------------------//
void CParticleEmitRangeObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CParticleEmitRangeObj * ptRang = (CParticleEmitRangeObj *)GetNext(posLoc);
        ptRang->SafeDelete();
		delete ptRang;
		ptRang = 0;
	}
	RemoveAll();
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
CParticleEmitRangeObj * CParticleEmitRangeObjList::GetValidEmmitter(int & animationIndex,int & frame)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CParticleEmitRangeObj * ptRang = (CParticleEmitRangeObj *)GetNext(posLoc);
		if(ptRang->m_animationQue != animationIndex)
		   continue;

        if(ptRang->m_useExactRange)
		   if(frame < ptRang->m_startRange || frame > ptRang->m_endRange)
		      continue;

		return ptRang;
	}
	return NULL;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Serialize-------------------------------------//
void CParticleEmitRangeObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){

	   ar << m_animationQue             
	      << m_startRange       
	      << m_endRange                  
	      << m_particleDBIndex
		  << m_boneLink
		  << m_useExactRange;
		 
	}
	else
	{
       ar >> m_animationQue              
	      >> m_startRange       
	      >> m_endRange                  
	      >> m_particleDBIndex
		  >> m_boneLink
		  >> m_useExactRange;
  
	      m_particleEmmiter   = NULL;
		  m_inRange           = FALSE;
	}
}


IMPLEMENT_SERIAL(CParticleEmitRangeObj, CObject, 0)
IMPLEMENT_SERIAL(CParticleEmitRangeObjList, CObject, 0)
