#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d9.h>
#include <afxtempl.h>
#include "matrixarb.h"
#include "CHousingClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CHousingObj::CHousingObj()
{
    m_houseName                = "House Unamed";
    m_closedState              = NULL; 
    m_openState                = NULL; 
	m_externalVisuals          = NULL;
    m_internalVisuals          = NULL;
	m_lod2Set                  = NULL;
	m_lod3Set                  = NULL;
	m_lightObj                 = NULL;
	m_doorVisual               = NULL;

    ZeroMemory(&m_lightPosition,sizeof(D3DVECTOR));
	ZeroMemory(&m_extrnalRangeLod1,sizeof(ABBOX));
	ZeroMemory(&m_extrnalRangeLod2,sizeof(ABBOX));
	ZeroMemory(&m_extrnalRangeLod3,sizeof(ABBOX));
	ZeroMemory(&m_internalArea,sizeof(ABBOX));
	ZeroMemory(&m_clipBox,sizeof(ABBOX));
	ZeroMemory(&m_internalArea,sizeof(ABBOX));	
}

void CHousingObj::SafeDelete()
{
    if(m_closedState)
	{
       m_closedState->SafeDelete();
	   delete m_closedState;
	   m_closedState = 0;
	}
	if(m_openState)
	{
       m_openState->SafeDelete();
	   delete m_openState;
	   m_openState = 0;
	}
	if(m_externalVisuals)
	{
       m_externalVisuals->SafeDelete();
	   delete m_externalVisuals;
	   m_externalVisuals = 0;
	}
	if(m_internalVisuals)
	{
       m_internalVisuals->SafeDelete();
	   delete m_internalVisuals;
	   m_internalVisuals = 0;
	}

    if(m_lod2Set)
	{
       m_lod2Set->SafeDelete();
	   delete m_lod2Set;
	   m_lod2Set = 0;
	}

	if(m_lod3Set)
	{
       m_lod3Set->SafeDelete();
	   delete m_lod3Set;
	   m_lod3Set = 0;
	}

	if(m_lightObj)
	{
	   delete m_lightObj;
	   m_lightObj = 0;
	}

	if(m_doorVisual)
	{
       m_doorVisual->SafeDelete();
	   delete m_doorVisual;
	   m_doorVisual = 0;
	}
}




void CHousingObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_houseName
          << m_closedState 
          << m_openState  
          << m_externalVisuals  
          << m_internalVisuals     
          << m_lod2Set                 
          << m_lod3Set                
          << m_lightObj
		  << m_extrnalRangeLod1.maxX
		  << m_extrnalRangeLod1.maxY
		  << m_extrnalRangeLod1.maxZ
		  << m_extrnalRangeLod1.minX
		  << m_extrnalRangeLod1.minY
		  << m_extrnalRangeLod1.minZ
          << m_extrnalRangeLod2.maxX
		  << m_extrnalRangeLod2.maxY
		  << m_extrnalRangeLod2.maxZ
		  << m_extrnalRangeLod2.minX
		  << m_extrnalRangeLod2.minY
		  << m_extrnalRangeLod2.minZ
          << m_extrnalRangeLod3.maxX
		  << m_extrnalRangeLod3.maxY
		  << m_extrnalRangeLod3.maxZ
		  << m_extrnalRangeLod3.minX
		  << m_extrnalRangeLod3.minY
		  << m_extrnalRangeLod3.minZ
          << m_internalArea.maxX
		  << m_internalArea.maxY
		  << m_internalArea.maxZ
		  << m_internalArea.minX
		  << m_internalArea.minY
		  << m_internalArea.minZ
          << m_clipBox.maxX
		  << m_clipBox.maxY
		  << m_clipBox.maxZ
		  << m_clipBox.minX
		  << m_clipBox.minY
		  << m_clipBox.minZ
		  << m_doorVisual
		  << m_lightPosition.x
		  << m_lightPosition.y
		  << m_lightPosition.z;
	}
	else
	{
       ar >> m_houseName
          >> m_closedState 
          >> m_openState  
          >> m_externalVisuals  
          >> m_internalVisuals     
          >> m_lod2Set                 
          >> m_lod3Set                
          >> m_lightObj
		  >> m_extrnalRangeLod1.maxX
		  >> m_extrnalRangeLod1.maxY
		  >> m_extrnalRangeLod1.maxZ
		  >> m_extrnalRangeLod1.minX
		  >> m_extrnalRangeLod1.minY
		  >> m_extrnalRangeLod1.minZ
          >> m_extrnalRangeLod2.maxX
		  >> m_extrnalRangeLod2.maxY
		  >> m_extrnalRangeLod2.maxZ
		  >> m_extrnalRangeLod2.minX
		  >> m_extrnalRangeLod2.minY
		  >> m_extrnalRangeLod2.minZ
          >> m_extrnalRangeLod3.maxX
		  >> m_extrnalRangeLod3.maxY
		  >> m_extrnalRangeLod3.maxZ
		  >> m_extrnalRangeLod3.minX
		  >> m_extrnalRangeLod3.minY
		  >> m_extrnalRangeLod3.minZ
          >> m_internalArea.maxX
		  >> m_internalArea.maxY
		  >> m_internalArea.maxZ
		  >> m_internalArea.minX
		  >> m_internalArea.minY
		  >> m_internalArea.minZ
          >> m_clipBox.maxX
		  >> m_clipBox.maxY
		  >> m_clipBox.maxZ
		  >> m_clipBox.minX
		  >> m_clipBox.minY
		  >> m_clipBox.minZ
		  >> m_doorVisual
		  >> m_lightPosition.x
		  >> m_lightPosition.y
		  >> m_lightPosition.z;
	}
}     

CEXMeshObjList * CHousingObj::GenerateRenderCache(D3DVECTOR & point,BOOL open)
{

   CEXMeshObjList * renderCache = new CEXMeshObjList();

   if(m_internalVisuals)
   if(BoundBoxPointCheck(point,m_internalArea))
      for(POSITION posLoc = m_internalVisuals->GetHeadPosition();posLoc != NULL;)
	  {
          CEXMeshObj * mesh = (CEXMeshObj *) m_internalVisuals->GetNext(posLoc);
          renderCache->AddTail(mesh);
	  }

   BOOL externalSetAdded = FALSE;

   if(m_lod3Set)
	 if(!BoundBoxPointCheck(point,m_extrnalRangeLod2))
	 {
      for(POSITION posLoc = m_lod3Set->GetHeadPosition();posLoc != NULL;)
	  {
          CEXMeshObj * mesh = (CEXMeshObj *) m_lod3Set->GetNext(posLoc);
          renderCache->AddTail(mesh);
	  }
      externalSetAdded = TRUE;
	 }

   if(!externalSetAdded)
   if(m_lod2Set)
   if(!BoundBoxPointCheck(point,m_extrnalRangeLod1))
   if(BoundBoxPointCheck(point,m_extrnalRangeLod2))
   {
      for(POSITION posLoc = m_lod2Set->GetHeadPosition();posLoc != NULL;)
	  {
          CEXMeshObj * mesh = (CEXMeshObj *) m_lod2Set->GetNext(posLoc);
          renderCache->AddTail(mesh);
	  }
      externalSetAdded = TRUE;
   }

   if(!externalSetAdded)
   if(m_externalVisuals)
      for(POSITION posLoc = m_externalVisuals->GetHeadPosition();posLoc != NULL;)
	  {
          CEXMeshObj * mesh = (CEXMeshObj *) m_externalVisuals->GetNext(posLoc);
          renderCache->AddTail(mesh);
	  }

   if(!open)
   {
	  if(m_doorVisual)
         renderCache->AddTail(m_doorVisual);
   }
   else if(m_doorVisual)
   {
      if(!BoundBoxPointCheck(point,m_internalArea))
         renderCache->AddTail(m_doorVisual);
   }
   


   return renderCache;
}

 BOOL CHousingObj::BoundBoxPointCheck(D3DVECTOR & point,ABBOX & box)
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

BOOL CHousingObj::FlushCacheObject(CEXMeshObjList * cache)
{

   if(!cache)
	   return FALSE;

   cache->RemoveAll();
   delete cache;
   cache = 0;

   return TRUE;
}

CHousingObj * CHousingObjList::GetByIndex(int index)
{
    if(index < 0)
	   return NULL;


	POSITION posLoc = FindIndex(index);
	if(!posLoc)
		return NULL;
        
	    CHousingObj * spnPtr = (CHousingObj *)GetAt(posLoc);
		return spnPtr;
}

void CHousingObjList::ReinitAll(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	 for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
         CHousingObj * spnPtr = (CHousingObj *)GetNext(posLoc);
		 if(spnPtr->m_externalVisuals)
             for(POSITION mPos = spnPtr->m_externalVisuals->GetHeadPosition();mPos != NULL;)
			 {
				 CEXMeshObj * exMesh = (CEXMeshObj *) spnPtr->m_externalVisuals->GetNext(mPos);
                 exMesh->InitBuffer(g_pd3dDevice,NULL);
			 }

		 if(spnPtr->m_internalVisuals)
			 for(POSITION mPos = spnPtr->m_internalVisuals->GetHeadPosition();mPos != NULL;)
			 {
				 CEXMeshObj * exMesh = (CEXMeshObj *) spnPtr->m_internalVisuals->GetNext(mPos);
                 exMesh->InitBuffer(g_pd3dDevice,NULL);
			 }

		 if(spnPtr->m_lod2Set)
			 for(POSITION mPos = spnPtr->m_lod2Set->GetHeadPosition();mPos != NULL;)
			 {
				 CEXMeshObj * exMesh = (CEXMeshObj *) spnPtr->m_lod2Set->GetNext(mPos);
                 exMesh->InitBuffer(g_pd3dDevice,NULL);
			 }

		 if(spnPtr->m_lod3Set)
			 for(POSITION mPos = spnPtr->m_lod3Set->GetHeadPosition();mPos != NULL;)
			 {
				 CEXMeshObj * exMesh = (CEXMeshObj *) spnPtr->m_lod3Set->GetNext(mPos);
                 exMesh->InitBuffer(g_pd3dDevice,NULL);
			 }

		 if(spnPtr->m_doorVisual)
			spnPtr->m_doorVisual->InitBuffer(g_pd3dDevice,NULL);
	 }
}


void CHousingObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CHousingObj * spnPtr = (CHousingObj *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

//--
//--

CHousingPlacementObj::CHousingPlacementObj()
{
    m_dbIndex        = 0;
    m_worldID        = 0;
	m_position.x     = 0.0f;
	m_position.y     = 0.0f;
	m_position.z     = 0.0f;
    m_open           = TRUE;
}

void CHousingPlacementObj::SafeDelete()
{
  
}




void CHousingPlacementObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_dbIndex
          << m_worldID 
          << m_position.x  
          << m_position.y  
          << m_position.z     
          << m_open;
	}
	else
	{
       ar << m_dbIndex
          << m_worldID 
          << m_position.x  
          << m_position.y  
          << m_position.z     
          << m_open;
	}
}     

void CHousingPlacementList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CHousingPlacementObj * spnPtr = (CHousingPlacementObj *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}


//---
//---


CHousingCacheObj::CHousingCacheObj()
{
    m_dbIndex        = 0;
	m_position.x     = 0.0f;
	m_position.y     = 0.0f;
	m_position.z     = 0.0f;
    m_open           = TRUE;
	m_validThisRound = TRUE;
}

void CHousingCacheObj::SafeDelete()
{
    
}

void CHousingCacheObjList::PlaceHouse(D3DVECTOR position, int dbIndex, int acID)
{

     if(GetByAcID(acID))//dont double add
		return;

     CHousingCacheObj * hsPtr = new CHousingCacheObj();
	 hsPtr->m_dbIndex = dbIndex;
	 CopyMemory(&hsPtr->m_position,&position,sizeof(D3DVECTOR));
     

	 MatrixARB matLib;
	 matLib.InitMat(hsPtr->m_location.m);
	 matLib.InitMat(hsPtr->m_invLocation.m);

	 hsPtr->m_acId         = acID;
	 hsPtr->m_location._41 = position.x;
	 hsPtr->m_location._42 = position.y;
	 hsPtr->m_location._43 = position.z;

	 hsPtr->m_invLocation._41 = -position.x;
	 hsPtr->m_invLocation._42 = -position.y;
	 hsPtr->m_invLocation._43 = -position.z;
 
	 AddTail(hsPtr);
}

void CHousingCacheObjList::FlagAllInvalid()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CHousingCacheObj * spnPtr = (CHousingCacheObj *)GetNext(posLoc);
	    spnPtr->m_validThisRound = FALSE;
	}
}

CHousingCacheObj * CHousingCacheObjList::GetByAcID(int acID)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CHousingCacheObj * spnPtr = (CHousingCacheObj *)GetNext(posLoc);
	    if(spnPtr->m_acId == acID)
		   return spnPtr;
	}
	return NULL;
}

BOOL CHousingCacheObjList::FlagValidByAcID(int & id,BOOL open)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CHousingCacheObj * spnPtr = (CHousingCacheObj *)GetNext(posLoc);
		if(spnPtr->m_acId == id)
		{
	       spnPtr->m_validThisRound = TRUE;
		   spnPtr->m_open = open;
           return TRUE;
		}
	}
	return FALSE;
}

void CHousingCacheObjList::RemoveAllInvalid()
{
	POSITION posLast = NULL;
	for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CHousingCacheObj * spnPtr = (CHousingCacheObj *)GetNext(posLoc);
		if(!spnPtr->m_validThisRound)
		{
		    spnPtr->SafeDelete();
		    delete spnPtr;
		    spnPtr=0;
			RemoveAt(posLast);
		}
	}
}

void CHousingCacheObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CHousingCacheObj * spnPtr = (CHousingCacheObj *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}


IMPLEMENT_SERIAL(CHousingObj, CObject, 0)
IMPLEMENT_SERIAL(CHousingObjList, CObList, 0)
IMPLEMENT_SERIAL(CHousingPlacementObj, CObject, 0)
IMPLEMENT_SERIAL(CHousingPlacementList, CObList, 0)


