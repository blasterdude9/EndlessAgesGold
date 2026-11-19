#include "stdafx.h"
#define D3D_OVERLOADS

#include <afxtempl.h>
#include "d3d9.h"
#include "CBankClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBankObj::CBankObj()
{
    m_bankName              = "VAULT";
    m_worldID               = 0;
	m_boundingBoxMax.x      =  60000.0f;
	m_boundingBoxMax.y      =  60000.0f;
	m_boundingBoxMax.z      =  60000.0f;
	m_boundingBoxMin.x      = -60000.0f;
	m_boundingBoxMin.y      = -60000.0f;
	m_boundingBoxMin.z      = -60000.0f;
	m_menuBind              = 0;
	m_willDealWithCriminals = TRUE;
}


void CBankObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CBankObj * spnPtr = (CBankObj *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

 BOOL CBankObjList::BoundBoxPointCheck(D3DVECTOR & point,D3DVECTOR & min,D3DVECTOR & max)
{
 if(point.x >= min.x &&
    point.x <= max.x &&
    point.y >= min.y &&
    point.y <= max.y &&
    point.z >= min.z &&
    point.z <= max.z)
  return TRUE;
 return FALSE;
}

CBankObj * CBankObjList::GetBankByPosition(D3DVECTOR position,
										   int worldID)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CBankObj * cmPtr = (CBankObj *)GetNext(posLoc);
	    if(worldID == cmPtr->m_worldID)//same world    
	       if(BoundBoxPointCheck(position,
						         cmPtr->m_boundingBoxMin,
								 cmPtr->m_boundingBoxMax)==TRUE)
		   {//in zone
              return cmPtr;
		   }//end in zone
	}
	return NULL;
}

void CBankObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_bankName
		  << m_worldID
		  << m_boundingBoxMax.x
		  << m_boundingBoxMax.y
		  << m_boundingBoxMax.z
		  << m_boundingBoxMin.x
		  << m_boundingBoxMin.y
		  << m_boundingBoxMin.z
		  << m_menuBind
		  << m_willDealWithCriminals;
	}
	else
	{
       ar >> m_bankName
		  >> m_worldID
		  >> m_boundingBoxMax.x
		  >> m_boundingBoxMax.y
		  >> m_boundingBoxMax.z
		  >> m_boundingBoxMin.x
		  >> m_boundingBoxMin.y
		  >> m_boundingBoxMin.z
		  >> m_menuBind
		  >> m_willDealWithCriminals;
	}
}     


IMPLEMENT_SERIAL(CBankObj, CObject, 0)
IMPLEMENT_SERIAL(CBankObjList, CObList, 0)

