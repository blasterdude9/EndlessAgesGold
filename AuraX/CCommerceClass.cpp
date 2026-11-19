#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CCommerceClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CStoreInventoryObj::CStoreInventoryObj()
{
    m_globalInventoryID = 0;
	m_raceID            = -1;
}

void CStoreInventoryObj::SafeDelete()
{

}

void CStoreInventoryObj::Serialize( CArchive& ar )
{
BOOL reserved = 0;
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
       ar.SetObjectSchema(2);
       ASSERT(ar.GetObjectSchema() == 2);


	   ar << m_globalInventoryID 
		  << m_raceID
		  << reserved
		  << reserved
		  << reserved
		  << reserved
		  << reserved;
	}
	else
	{
       int version = ar.GetObjectSchema();

	   if(version < 2)
	   {
          ar >> m_globalInventoryID;
		  m_raceID            = -1;

	   }
	   else if(version == 2)
	   {
          ar >> m_globalInventoryID 
			 >> m_raceID
			 >> reserved
			 >> reserved
			 >> reserved
			 >> reserved
			 >> reserved;


	   }

       
	   
	}
}     

void CStoreInventoryObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CStoreInventoryObj * invPtr = (CStoreInventoryObj *)GetNext(posLoc);
		invPtr->SafeDelete();
		delete invPtr;
		invPtr=0;
	}
	RemoveAll();
}

BOOL CStoreInventoryObjList::VerifyInventoryGLID(int GLID)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CStoreInventoryObj * invPtr = (CStoreInventoryObj *)GetNext(posLoc);
		if(invPtr->m_globalInventoryID == GLID)
		   return TRUE;
	}
	return FALSE;
}
//-------------------------------------------------------------//
CCommerceObj::CCommerceObj()
{
   m_establishmentName     = "N/A";
   m_uniqueID              = 0;
   m_storeInventory        = NULL;
   m_boundingBoxMax.x      = 0.0f;
   m_boundingBoxMax.y      = 0.0f;
   m_boundingBoxMax.z      = 0.0f;
   m_boundingBoxMin.x      = 0.0f;
   m_boundingBoxMin.y      = 0.0f;
   m_boundingBoxMin.z      = 0.0f;
   m_worldID               = 0;
   m_menuBind              = 0;
   m_willDealWithCriminals = TRUE;
}

void CCommerceObj::SafeDelete()
{
	if(m_storeInventory)
	{
       m_storeInventory->SafeDelete();
	   delete m_storeInventory;
	   m_storeInventory = 0;
	}
}

void CCommerceObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_establishmentName
		  << m_uniqueID
		  << m_storeInventory
		  << m_boundingBoxMax.x
		  << m_boundingBoxMax.y
		  << m_boundingBoxMax.z
		  << m_boundingBoxMin.x
		  << m_boundingBoxMin.y
		  << m_boundingBoxMin.z
		  << m_worldID
		  << m_menuBind
		  << m_willDealWithCriminals;
	}
	else
	{
       ar >> m_establishmentName
		  >> m_uniqueID
		  >> m_storeInventory
		  >> m_boundingBoxMax.x
		  >> m_boundingBoxMax.y
		  >> m_boundingBoxMax.z
		  >> m_boundingBoxMin.x
		  >> m_boundingBoxMin.y
		  >> m_boundingBoxMin.z
		  >> m_worldID
		  >> m_menuBind
		  >> m_willDealWithCriminals;

	}
}     

CCommerceObj * CCommerceObjList::GetEstablishmentByID(int ID)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CCommerceObj * invPtr = (CCommerceObj *)GetNext(posLoc);
		if(invPtr->m_uniqueID == ID)
		return invPtr;
	}
	return NULL;
}

CCommerceObj * CCommerceObjList::GetEstablishmentByPosition(D3DVECTOR position,
														  int worldID)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CCommerceObj * cmPtr = (CCommerceObj *)GetNext(posLoc);
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

 BOOL CCommerceObjList::BoundBoxPointCheck(D3DVECTOR & point,D3DVECTOR & min,D3DVECTOR & max)
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

void CCommerceObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CCommerceObj * invPtr = (CCommerceObj *)GetNext(posLoc);
		invPtr->SafeDelete();
		delete invPtr;
		invPtr=0;
	}
	RemoveAll();
}

IMPLEMENT_SERIAL(CCommerceObj, CObject, 0)
IMPLEMENT_SERIAL(CCommerceObjList, CObList, 0)
IMPLEMENT_SERIAL(CStoreInventoryObj, CObject, VERSIONABLE_SCHEMA|2)
IMPLEMENT_SERIAL(CStoreInventoryObjList, CObList, 0)

