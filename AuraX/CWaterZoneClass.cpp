#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CWaterZoneClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWaterZoneObj::CWaterZoneObj()
{
   ZeroMemory(&m_waterArea,sizeof(ABBOX));
   m_isExclusionZone = FALSE;
}


BOOL CWaterZoneObj::InitFromARW(CString filename,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
    CEXMeshObj * newMesh = new CEXMeshObj();
	float sightRangeDump = 0.0f;
    newMesh->LoadGeometryFromPlugin(filename,&sightRangeDump,g_pd3dDevice,NULL);
    CopyMemory(&m_waterArea,&newMesh->List_boundingBox,sizeof(ABBOX));
    newMesh->SafeDelete();
	delete newMesh;
	newMesh = 0;

    return TRUE;
}

BOOL CWaterZoneObj::BoundBoxPointCheck(D3DVECTOR & point,ABBOX box)
{
 if(point.x >= box.minX &&
    point.x <= box.maxX &&
    point.y >= box.minY &&
    point.y <= box.maxY &&
    point.z >= box.minZ &&
    point.z <= box.maxZ )
  return TRUE;
 return FALSE;
}

BOOL CWaterZoneObj::IsPositionInZone(D3DVECTOR position)
{
    return BoundBoxPointCheck(position,m_waterArea);
}

void CWaterZoneObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_waterArea.maxX
		  << m_waterArea.maxY
		  << m_waterArea.maxZ
		  << m_waterArea.minX
		  << m_waterArea.minY
		  << m_waterArea.minZ
		  << m_isExclusionZone
		  << m_waterZoneName;
	}
	else
	{
       ar >> m_waterArea.maxX
		  >> m_waterArea.maxY
		  >> m_waterArea.maxZ
		  >> m_waterArea.minX
		  >> m_waterArea.minY
		  >> m_waterArea.minZ
		  >> m_isExclusionZone
		  >> m_waterZoneName;

	}
}

/*void CWaterZoneObjList::Serialize( CArchive& ar )
{

CObList::Serialize( ar );
	if(ar.IsStoring() )
	{	
	   
	}
	else
	{
       
	}
}*/


BOOL CWaterZoneObjList::IsPositionInWater(D3DVECTOR position,float * submergedValue)  
{
	BOOL inWater = FALSE;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CWaterZoneObj * spnPtr = (CWaterZoneObj *)GetNext(posLoc);
		BOOL result = spnPtr->IsPositionInZone(position);
		if(result == TRUE && spnPtr->m_isExclusionZone == TRUE)
		   return FALSE;//exclusion ovverrides

		if(result == TRUE){
		   *submergedValue = position.y - spnPtr->m_waterArea.maxY;
		   inWater = TRUE;
		}
	}
	return inWater;
}

void CWaterZoneObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CWaterZoneObj * spnPtr = (CWaterZoneObj *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}


IMPLEMENT_SERIAL(CWaterZoneObj, CObject, 0)
IMPLEMENT_SERIAL(CWaterZoneObjList, CObList, 0)

