#include "stdafx.h"
#include <d3d9.h>
#include <afxtempl.h>
#include "CContainmentClass.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CContainmentObject::CContainmentObject()
{
    m_containmentBasis    = 0;//0=NONE 1=resource
    m_miscType            = 0;
    m_respawnTime         = 20000;
    m_lastRespawnTime     = 20000;
    m_currentAmount       = 10;
    m_maxAmount           = 10;
    m_dispersalAmount     = 1;
}

int CContainmentObject::UseIt()
{
	if(m_currentAmount < m_maxAmount)
	{
		if(((long)GetTickCount()-m_lastRespawnTime) > m_respawnTime){
		   m_currentAmount = m_maxAmount;
           m_lastRespawnTime = GetTickCount();
		}
		if(m_currentAmount >= m_dispersalAmount){
		  m_currentAmount -= m_dispersalAmount;
	      return m_dispersalAmount;
		}
		if(m_currentAmount > 0){
		  m_currentAmount = 0;
		  return m_currentAmount;
		}
	    return 0;
	}

	if(m_currentAmount == m_maxAmount){//full
       m_lastRespawnTime = GetTickCount();
	   m_currentAmount -= m_dispersalAmount;
	   
	    if(m_currentAmount >= m_dispersalAmount){
	       return m_dispersalAmount;
		}
		if(m_currentAmount > 0){
		   return m_currentAmount;
		}
	}//end full
 return 0;
}

void CContainmentObject::Clone(CContainmentObject ** copy)
{
    CContainmentObject*copyLocal = new CContainmentObject();
    copyLocal->m_containmentBasis    = m_containmentBasis;
	copyLocal->m_currentAmount       = m_currentAmount;
	copyLocal->m_dispersalAmount     = m_dispersalAmount;
    copyLocal->m_maxAmount           = m_maxAmount;
	copyLocal->m_miscType            = m_miscType;
	copyLocal->m_respawnTime         = m_respawnTime;	
	copyLocal->m_lastRespawnTime     = 0;
	*copy = copyLocal;
}

void CContainmentObject::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	    
	   ar << m_containmentBasis
          << m_miscType
          << m_respawnTime
          << m_lastRespawnTime
          << m_currentAmount
          << m_maxAmount
          << m_dispersalAmount;

	}
	else
	{
       ar >> m_containmentBasis
          >> m_miscType
          >> m_respawnTime
          >> m_lastRespawnTime
          >> m_currentAmount
          >> m_maxAmount
          >> m_dispersalAmount;
	}
}

void CContainmentObjectList::SafeDelete()
{
     for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;)
	 {
         CContainmentObject * cntObject = (CContainmentObject *)GetNext(posLoc);
         delete cntObject;
		 cntObject=0;
	 }
	 RemoveAll();
}


IMPLEMENT_SERIAL(CContainmentObject, CObject, 0)
IMPLEMENT_SERIAL(CContainmentObjectList, CObList, 0)

