#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CObjectLibraryClass.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CObjectLibraryObj::CObjectLibraryObj()
{
    m_wldObject = NULL;
}

void CObjectLibraryObj::SafeDelete()
{
    if(m_wldObject){
	   m_wldObject->SafeDelete();
	   delete m_wldObject;
	   m_wldObject = 0;
	}
}

CObjectLibraryObjList::CObjectLibraryObjList()
{
   m_altCollisionModel = NULL;
}

BOOL CObjectLibraryObjList::AddObject(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
            CObjectLibraryObj * newPtr = new CObjectLibraryObj();
            BOOL result = newPtr->InitFromFile(fileName,g_pd3dDevice);
            if(!result)
				return FALSE;

                AddTail(newPtr);
				return TRUE;
}

BOOL CObjectLibraryObjList::AddCollisionObject(CString filename,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
    if(m_altCollisionModel)
	   delete m_altCollisionModel;
	   m_altCollisionModel = 0;

       m_altCollisionModel = new CWldObject(0);  

       m_altCollisionModel->List_triggerList = new CTriggerObjectList;
       m_altCollisionModel->m_fileNameRef = filename;
       m_altCollisionModel->m_meshObject = new CEXMeshObj();
       m_altCollisionModel->m_meshObject->List_materialObject = new CMaterialObject();
       float sightRange = 0.0f;
       BOOL result = m_altCollisionModel->m_meshObject->LoadGeometryFromPlugin(filename,&sightRange,g_pd3dDevice,NULL);//proprietory file format
       if(!result)
	       return FALSE;    
           m_altCollisionModel->CalibrateCenteredBoundingBox((int)sightRange);
return TRUE;
}

void CObjectLibraryObjList::RemoveCollisionObject()
{
    if(m_altCollisionModel)
	   delete m_altCollisionModel;
	   m_altCollisionModel = 0;
}


void CObjectLibraryObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CObjectLibraryObj * spnPtr = (CObjectLibraryObj *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

void CObjectLibraryObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_wldObject
		  << m_description;
	}
	else
	{
       ar >> m_wldObject
		  >> m_description;
	}
}     

void CObjectLibraryObjList::Serialize( CArchive& ar )
{
CObList::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_altCollisionModel
		  << m_description;
	}
	else
	{
       ar >> m_altCollisionModel
		  >> m_description;	
	}
}     

CObjectCollection::CObjectCollection()
{
m_collection = NULL;
}

void CObjectCollection::SafeDelete()
{
	if(m_collection){
       m_collection->SafeDelete();
       delete m_collection;
	   m_collection=0;
	}
}

void CObjectCollectionList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CObjectCollection * spnPtr = (CObjectCollection *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

BOOL CObjectLibraryObj::InitFromFile(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
   m_wldObject = new CWldObject(0);  

   m_wldObject->List_triggerList = new CTriggerObjectList;
   m_wldObject->m_fileNameRef = fileName;
   m_wldObject->m_meshObject = new CEXMeshObj();
   m_wldObject->m_meshObject->List_materialObject = new CMaterialObject();
   float sightRange = 0.0f;
   BOOL result = m_wldObject->m_meshObject->LoadGeometryFromPlugin(fileName,&sightRange,g_pd3dDevice,&m_wldObject->List_popoutBox);//proprietory file format
   if(!result)
	   return FALSE;    
       //m_wldObject->CalibrateCenteredBoundingBox((int)sightRange);
       
return TRUE;
}

void CObjectCollection::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_collection;
	}
	else
	{
       ar >> m_collection;
	}
}


IMPLEMENT_SERIAL(CObjectLibraryObj, CObject, 0)
IMPLEMENT_SERIAL(CObjectLibraryObjList, CObList, 0)
IMPLEMENT_SERIAL(CObjectCollection, CObList, 0)
IMPLEMENT_SERIAL(CObjectCollectionList, CObList, 0)


