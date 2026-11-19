#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CitemExpanseObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CExpansiveObj::CExpansiveObj()
{
    m_iconIndex = 0;
    m_xPos      = 0.0f;
    m_yPos      = 0.0f;
    m_miscInt   = 0;
    m_miscInt2  = 0;
}

void CExpansiveObj::Clone(CExpansiveObj ** clone)
{
   CExpansiveObj * newCopy = new CExpansiveObj();
  
   newCopy->m_iconIndex = m_iconIndex;
   newCopy->m_xPos      = m_xPos;
   newCopy->m_yPos      = m_yPos;
   newCopy->m_miscInt   = m_miscInt;
   newCopy->m_miscInt2  = m_miscInt2;

   *clone = newCopy;
}

void CExpansiveObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_iconIndex
	      << m_xPos
	      << m_yPos
	      << m_miscInt
	      << m_miscInt2;
	}
	else
	{
       ar >> m_iconIndex
	      >> m_xPos
	      >> m_yPos
	      >> m_miscInt
	      >> m_miscInt2;
	}
} 

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CCExpansiveObjList::Clone(CCExpansiveObjList ** clone)
{
   CCExpansiveObjList * newList = new CCExpansiveObjList();
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CExpansiveObj * bnPtr = (CExpansiveObj *)GetNext(posLoc);
        CExpansiveObj * newObject = NULL;  
		bnPtr->Clone(&newObject);
		newList->AddTail(newObject);
	}
   *clone = newList;
}  
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
CExpansiveObj * CCExpansiveObjList::GetByMisc2(int miscInt2)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CExpansiveObj * spnPtr = (CExpansiveObj *)GetNext(posLoc);
	    if(spnPtr->m_miscInt2 == miscInt2)
		{
           return spnPtr;
		}
	}
	return NULL;
}  
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CCExpansiveObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CExpansiveObj * spnPtr = (CExpansiveObj *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CExpansiveObj::SerializeAlloc(CArchive& ar)
{
	if(ar.IsStoring() ){	
	    ar << m_iconIndex
	       << m_xPos
	       << m_yPos
	       << m_miscInt
	       << m_miscInt2;
	}
	else
	{
       ar >> m_iconIndex
	       >> m_xPos
	       >> m_yPos
	       >> m_miscInt
	       >> m_miscInt2;
	}
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CCExpansiveObjList::SerializeAlloc(CArchive& ar,int loadCount)
{
	if(ar.IsStoring() ){	
	   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
           CExpansiveObj * elementPtr = (CExpansiveObj *)GetNext(posLoc);
		   elementPtr->SerializeAlloc(ar);
	   }
	}
	else
	{
       for(int i=0;i<loadCount;i++)
	   {
		   CExpansiveObj * elementPtr = new CExpansiveObj();
           elementPtr->SerializeAlloc(ar);
		   AddTail(elementPtr);
	   }
	}
}

IMPLEMENT_SERIAL(CExpansiveObj, CObject, 0)
IMPLEMENT_SERIAL(CCExpansiveObjList, CObList, 0)

