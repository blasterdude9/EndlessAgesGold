#include "stdafx.h"
#define D3D_OVERLOADS

#include "d3d9.h"
#include <afxtempl.h>
#include "CSafeZones.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSafeZoneObj::CSafeZoneObj()
{
    m_zoneName           = "SAFE";
    m_worldID            = 0;
	m_boundingBoxMax.x   =  60000.0f;
	m_boundingBoxMax.y   =  60000.0f;
	m_boundingBoxMax.z   =  60000.0f;
	m_boundingBoxMin.x   = -60000.0f;
	m_boundingBoxMin.y   = -60000.0f;
	m_boundingBoxMin.z   = -60000.0f;
	m_rebirthableZone    = FALSE;
}


void CSafeZoneObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CSafeZoneObj * spnPtr = (CSafeZoneObj *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

 BOOL CSafeZoneObjList::BoundBoxPointCheck(D3DVECTOR & point,
												 D3DVECTOR & min,
												 D3DVECTOR & max)
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

 CSafeZoneObj * CSafeZoneObjList::GetSafeZoneByPosition(D3DVECTOR & position,
										                      int worldID)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CSafeZoneObj * cmPtr = (CSafeZoneObj *)GetNext(posLoc);
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

void CSafeZoneObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_zoneName
		  << m_worldID
		  << m_boundingBoxMax.x
		  << m_boundingBoxMax.y
		  << m_boundingBoxMax.z
		  << m_boundingBoxMin.x
		  << m_boundingBoxMin.y
		  << m_boundingBoxMin.z
		  << m_rebirthableZone;
	}
	else
	{
       ar >> m_zoneName
		  >> m_worldID
		  >> m_boundingBoxMax.x
		  >> m_boundingBoxMax.y
		  >> m_boundingBoxMax.z
		  >> m_boundingBoxMin.x
		  >> m_boundingBoxMin.y
		  >> m_boundingBoxMin.z
		  >> m_rebirthableZone;
	}
}     


IMPLEMENT_SERIAL(CSafeZoneObj, CObject, 0)
IMPLEMENT_SERIAL(CSafeZoneObjList, CObList, 0)

