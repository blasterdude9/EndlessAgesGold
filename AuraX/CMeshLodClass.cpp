#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CMeshLodClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CMeshLODObject::CMeshLODObject()
{
	m_distance          = 0.0f;//greater than activate it
	m_boxOption.maxX    = 0.0f;
	m_boxOption.maxY    = 0.0f;
	m_boxOption.maxZ    = 0.0f;
	m_boxOption.minX    = 0.0f;
	m_boxOption.minY    = 0.0f;
	m_boxOption.minZ    = 0.0f;
    m_meshLodLevel = NULL;
}

void CMeshLODObject::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	ar << m_distance  
	   << m_boxOption.maxX 
	   << m_boxOption.maxY 
	   << m_boxOption.maxZ  
	   << m_boxOption.minX   
	   << m_boxOption.minY   
	   << m_boxOption.minZ   
       << m_meshLodLevel;    
	}
	else
	{
    ar >> m_distance  
	   >> m_boxOption.maxX 
	   >> m_boxOption.maxY 
	   >> m_boxOption.maxZ  
	   >> m_boxOption.minX   
	   >> m_boxOption.minY   
	   >> m_boxOption.minZ   
       >> m_meshLodLevel;    

	}
}

void CMeshLODObject::SafeDelete()
{
    if(m_meshLodLevel){
	   m_meshLodLevel->SafeDelete();
       delete m_meshLodLevel;
	   m_meshLodLevel=0;
	}


}


CEXMeshObj * CMeshLODObjectList::GetLodVisual(D3DVECTOR & viewPoint,
											  D3DVECTOR & objPoint,
											  float & currentFov,
											  int & lodOverride)
{
    if(lodOverride > -1)
       return GetByIndex(lodOverride);

	if(m_lodType == 0){//distance based
	  
	  float distance = GL_matrixFunctions.MagnitudeSquared(viewPoint.x,viewPoint.y,viewPoint.z,
	                                                       objPoint.x,objPoint.y,objPoint.z);
	  float adViewVar = currentFov/.68f;//
	  distance = distance * adViewVar;
      CEXMeshObj * refMesh = NULL;
	  for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
          CMeshLODObject * skPtr = (CMeshLODObject *)GetNext(posLoc);
		  if(distance > skPtr->m_distance){
             refMesh = skPtr->m_meshLodLevel;
			 //break;
		  }
	  }
	  if(refMesh)
		 return refMesh;
	}//end distance based
    if(m_lodType == 1){//box based
      CEXMeshObj * refMesh = NULL;
	  for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
          CMeshLODObject * skPtr = (CMeshLODObject *)GetNext(posLoc); 
		  

		  if(BoundBoxPointCheck(viewPoint,skPtr->m_boxOption)==FALSE){
             refMesh = skPtr->m_meshLodLevel;
			 //break;
		  }
	  }
	  if(refMesh)
		 return refMesh;
	}//end distance based




	return NULL;
}

CEXMeshObj * CMeshLODObjectList::GetByIndex(int lod)
{
	if(lod < 0)
	   return NULL;
    POSITION posLoc = FindIndex(lod);
    if(!posLoc)
		return NULL;
	
	CMeshLODObject * lPtr = (CMeshLODObject *)GetAt(posLoc);
	return lPtr->m_meshLodLevel;
}

CEXMeshObj * CMeshLODObjectList::GetLodVisualExt(D3DVECTOR & viewPoint,
											     D3DVECTOR & objPoint,
											     float     & currentFov,
                                                 CEXMeshObj** refObj2,
												 float     * obj1AlpaOpt,
												 float     * obj2AlpaOpt,
												 int & lodOverride)
{
	//m_lodType = 2;
	if(lodOverride > -1)
       return GetByIndex(lodOverride);

	if(m_lodType == 0){//distance based
	  
	  float distance = GL_matrixFunctions.MagnitudeSquared2(viewPoint.x,viewPoint.y,viewPoint.z,
	                                                        objPoint.x,objPoint.y,objPoint.z);
	  float adViewVar = currentFov/.68f;//
	  distance = distance * adViewVar;
      CEXMeshObj * refMesh = NULL;
	  for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
          CMeshLODObject * skPtr = (CMeshLODObject *)GetNext(posLoc);
		  if(distance > skPtr->m_distance){
             refMesh = skPtr->m_meshLodLevel;
			 //break;
		  }
	  }
	  if(refMesh)
		 return refMesh;
	}//end distance based
    if(m_lodType == 1){//box based
      CEXMeshObj * refMesh = NULL;
	  for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
          CMeshLODObject * skPtr = (CMeshLODObject *)GetNext(posLoc); 
		  

		  if(BoundBoxPointCheck(viewPoint,skPtr->m_boxOption)==FALSE){
             refMesh = skPtr->m_meshLodLevel;
			 //break;
		  }
	  }
	  if(refMesh)
		 return refMesh;
	}//end distance based
    if(m_lodType == 2){//ranged adv fade based
      float distance = GL_matrixFunctions.MagnitudeSquared(viewPoint.x,viewPoint.y,viewPoint.z,
	                                                       objPoint.x,objPoint.y,objPoint.z);

	  //m_optionalOverlapDist
	  CEXMeshObj * lastMesh = NULL;
      CEXMeshObj * refMesh = NULL;
	  for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;)
	  {//account list
          CMeshLODObject * skPtr = (CMeshLODObject *)GetNext(posLoc); 
		  if(distance > (skPtr->m_distance-m_optionalOverlapDist))
		  {
             if(distance < (skPtr->m_distance+m_optionalOverlapDist))
			 {//within transition
                *obj2AlpaOpt = 1.0f * (((skPtr->m_distance+m_optionalOverlapDist) - distance)/(m_optionalOverlapDist*2.0f));
                *obj1AlpaOpt = 1.0f - *obj2AlpaOpt;

                *refObj2 = skPtr->m_meshLodLevel;
				return lastMesh;
			 }//end within transition
			 else 
			 {//clean change
			    *obj1AlpaOpt = 1.0f;
				*obj2AlpaOpt = 1.0f;
                refMesh = skPtr->m_meshLodLevel;
			 }//end clean change
		  }
		  lastMesh = skPtr->m_meshLodLevel;
	  }
	  if(refMesh)
		 return refMesh;
	}//end ranged adv fade based

	return NULL;
}



BOOL CMeshLODObjectList::AddLOD(int type,
								ABBOX * box,
								float distance,
								CString filename,
								CMaterialObject * materialObject,
								LPDIRECT3DDEVICE9 g_pd3dDevice)
{
    CMeshLODObject * meshObj = new CMeshLODObject();
	 m_lodType = type;
    if(type == 0){//distance based
       meshObj->m_distance = distance;
	   
       meshObj->m_meshLodLevel = new CEXMeshObj();
	   materialObject->Clone(&meshObj->m_meshLodLevel->List_materialObject);
	   float sightRange = 0.0f;
	   BOOL result = meshObj->m_meshLodLevel->LoadGeometryFromPlugin(filename,&sightRange,g_pd3dDevice,NULL);
	   if(result == TRUE){
	   meshObj->m_meshLodLevel->InitBuffer(g_pd3dDevice,NULL);
	   materialObject->Clone(&meshObj->m_meshLodLevel->List_materialObject);
	   AddTail(meshObj);
	   return TRUE;
	   }
	   else
	   return FALSE;
	}//end distance based
	if(type == 1 && box != NULL){//distance based
       meshObj->m_boxOption.maxX = box->maxX;
	   meshObj->m_boxOption.maxY = box->maxY;
	   meshObj->m_boxOption.maxZ = box->maxZ;
	   meshObj->m_boxOption.minX = box->minX;
	   meshObj->m_boxOption.minY = box->minY;
	   meshObj->m_boxOption.minZ = box->minZ;
       meshObj->m_meshLodLevel = new CEXMeshObj();
	   materialObject->Clone(&meshObj->m_meshLodLevel->List_materialObject);
	   float sightRange = 0.0f;
	   BOOL result = meshObj->m_meshLodLevel->LoadGeometryFromPlugin(filename,&sightRange,g_pd3dDevice,NULL);

	   if(result == TRUE){
       materialObject->Clone(&meshObj->m_meshLodLevel->List_materialObject);
	   meshObj->m_meshLodLevel->InitBuffer(g_pd3dDevice,NULL);
	   //materialObject->Clone(&meshObj->m_meshLodLevel->List_materialObject);
	   AddTail(meshObj);
	   return TRUE;
	   }
	   else
	   return FALSE;
	}//end distance based
    

	return FALSE;
}

//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----Get File Ext--------------------------------------------//
CString CMeshLODObjectList::GetFileExt(CString currentName)
{
CString ext;
int length = currentName.GetLength();
BOOL done = FALSE;
 while(done == FALSE){
  CString chr = (CString)currentName.GetAt(length-1);
  
  if(chr == ".")
	 break;

  ext = operator+(chr,ext);
  length = length -1;
 }
return ext;
}




BOOL CMeshLODObjectList::BoundBoxPointCheck(D3DVECTOR & point,ABBOX & box)
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



void CMeshLODObject::Clone(CMeshLODObject ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
  CMeshLODObject * copyLocal = new CMeshLODObject();
  copyLocal->m_distance          = m_distance;
  copyLocal->m_boxOption.maxX    = m_boxOption.maxX;
  copyLocal->m_boxOption.maxY    = m_boxOption.maxY;
  copyLocal->m_boxOption.maxZ    = m_boxOption.maxZ;
  copyLocal->m_boxOption.minX    = m_boxOption.minX;
  copyLocal->m_boxOption.minY    = m_boxOption.minY;
  copyLocal->m_boxOption.minZ    = m_boxOption.minZ;

  if(m_meshLodLevel)
     m_meshLodLevel->Clone(&copyLocal->m_meshLodLevel,g_pd3dDevice);

  *copy = copyLocal;
}


CMeshLODObjectList::CMeshLODObjectList()
{
   m_lodType = 0;//default distance type
   m_optionalOverlapDist = 10.0f;//test var
}


void CMeshLODObjectList::Clone(CMeshLODObjectList ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	CMeshLODObjectList * copyLocal = new CMeshLODObjectList();
	copyLocal->m_lodType = m_lodType;
    for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
        CMeshLODObject * skPtr = (CMeshLODObject *)GetNext(posLoc);
	    CMeshLODObject * newPtr = NULL;
	    skPtr->Clone(&newPtr,g_pd3dDevice);
	    copyLocal->AddTail(newPtr);
	}
    *copy = copyLocal;
}

void CMeshLODObjectList::RemoveLod(int index)
{
   int indexTrace = 0;
   POSITION posLast;
	for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) !=NULL;){//account list
        CMeshLODObject * skPtr = (CMeshLODObject *)GetNext(posLoc);
		if(index == indexTrace){
			skPtr->SafeDelete();
			delete skPtr;
			skPtr=0;
            RemoveAt(posLast);
           return;
		}
		indexTrace++;
	}
}

void CMeshLODObjectList::Initialize(LPDIRECT3DDEVICE9 g_pd3dDevice, void * cgMaterialDB)
{
	CGlobalMaterialShaderList * cgList = (CGlobalMaterialShaderList*)cgMaterialDB;
	for(POSITION posLoc = GetHeadPosition();posLoc !=NULL;){//account list
        CMeshLODObject * skPtr = (CMeshLODObject *)GetNext(posLoc);
		if(skPtr->m_meshLodLevel->List_materialObject)
           skPtr->m_meshLodLevel->InitBuffer(g_pd3dDevice,cgList->GetByIndex(skPtr->m_meshLodLevel->List_materialObject->m_optionalShaderIndex-1));//ptrLoc->m_meshObject->List_materialObject->m_diffuseEffect);
		else
		   skPtr->m_meshLodLevel->InitBuffer(g_pd3dDevice,NULL);
	}
}


void CMeshLODObjectList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
        CMeshLODObject * skPtr = (CMeshLODObject *)GetNext(posLoc);
		skPtr->SafeDelete();
		delete skPtr;
		skPtr=0;
	}
	RemoveAll();
}

void CMeshLODObjectList::Serialize( CArchive& ar )
{
CObList::Serialize( ar );
	if(ar.IsStoring() ){
	ar << m_lodType;    
	}
	else
	{
    ar >> m_lodType;    
	m_optionalOverlapDist = 10.0f;//test var
	}
}

IMPLEMENT_SERIAL(CMeshLODObject, CObject, 0)
IMPLEMENT_SERIAL(CMeshLODObjectList, CObList, 0)

