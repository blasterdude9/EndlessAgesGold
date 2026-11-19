#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d9.h>
#include <afxtempl.h>
#include "CPortalSysClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPortalSysObj::CPortalSysObj()
{
    m_keyRequired = 0;//0=no key required
    m_targetWorld = 0;
	m_zoneWldID   = 0;
	ZeroMemory(&m_wldPosition,sizeof(D3DVECTOR));//target location
    ZeroMemory(&m_portalZone,sizeof(ABBOX));
}


BOOL CPortalSysObj::InZone(D3DVECTOR currentLocation,int curWorld)
{
	 if(curWorld != m_zoneWldID)
		return FALSE;

     return BoundBoxPointCheck(currentLocation,m_portalZone);
}

 BOOL CPortalSysObj::BoundBoxPointCheck(D3DVECTOR & point,
										      ABBOX & box)
{
	
 if(point.x >= box.minX &&
    point.x <= box.maxX &&
    point.y >= box.minY &&
    point.y <= box.maxY &&
    point.z >= box.minZ &&
    point.z <= box.maxZ)
  return TRUE;
 return FALSE;
}

CPortalSysObj * CPortalSysList::ReturnPortal(D3DVECTOR currentPosition,int curWorld)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CPortalSysObj * spnPtr = (CPortalSysObj *)GetNext(posLoc);
		if(spnPtr->InZone(currentPosition,curWorld))
		   return spnPtr;
	}
	return NULL;
}

void CPortalSysList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CPortalSysObj * spnPtr = (CPortalSysObj *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}


void CPortalSysObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_portalName
		  << m_keyRequired
		  << m_targetWorld
		  << m_wldPosition.x
		  << m_wldPosition.y
		  << m_wldPosition.z
		  << m_portalZone.maxX
		  << m_portalZone.maxY
		  << m_portalZone.maxZ
		  << m_portalZone.minX
		  << m_portalZone.minY
		  << m_portalZone.minZ
		  << m_zoneWldID;
	}
	else
	{
       ar >> m_portalName
		  >> m_keyRequired
		  >> m_targetWorld
		  >> m_wldPosition.x
		  >> m_wldPosition.y
		  >> m_wldPosition.z
		  >> m_portalZone.maxX
		  >> m_portalZone.maxY
		  >> m_portalZone.maxZ
		  >> m_portalZone.minX
		  >> m_portalZone.minY
		  >> m_portalZone.minZ
		  >> m_zoneWldID;

	}
}     


IMPLEMENT_SERIAL(CPortalSysObj, CObject, 0)
IMPLEMENT_SERIAL(CPortalSysList, CObList, 0)

