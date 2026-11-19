#include "stdafx.h"

#include <d3d9.h>
#include <afxtempl.h>
#include "afxext.h"
#include "CWldObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------Main object list------------------//
CWldObject::CWldObject(int GlobalTraceNumber)
{
List_groupNumber = 0; 
List_collisionType = 0;
List_identity = GlobalTraceNumber;
List_popoutRadius = 0.0f;//popout radius//calculate later
List_poppedOut = FALSE;//popped out
m_lightObject = NULL;//light interface
List_attribute = NULL;
List_baseSoundBuffer = NULL;
List_buffer3DControl = NULL;
List_soundFileName = "NONE";
List_useSound = FALSE;
List_soundFactor = .03f;//L_soundFactor,
List_isPlaying = FALSE;//L_isPlaying,
List_validRange = 0.0f;//L_validRange
List_environmentAttached = FALSE;//environment attached
List_miscFloat = 0.0f;
List_miscInt = 0;
List_usedAsSpawnPoint = -1;
List_removeEnviorment = FALSE;
List_collisionInfoBuffer = NULL;
List_popoutBox.max.x = 0.0f;
List_popoutBox.max.y = 0.0f;
List_popoutBox.max.z = 0.0f;
List_popoutBox.min.x = 0.0f;
List_popoutBox.min.y = 0.0f;
List_popoutBox.min.z = 0.0f;
List_showBox = FALSE;
m_containmentObject   = NULL;
m_lodVisualList       = NULL;
m_meshObject          = NULL;
m_renderInMirrorTag   = FALSE;
m_particleDirection.x = 0.0f;
m_particleDirection.y = 1.0f;
m_particleDirection.z = 0.0f;
m_particleEmmiter     = NULL;
m_haloLink            = 0;
m_lensFlareType       = 0;
m_fileNameRef         = "N/A";
m_collisionInfoFilter = FALSE;
m_renderFilter        = FALSE;
m_particleLink        = -1;
m_soundRange          = NULL; 
m_filterGroup         = -1;
m_node                = NULL;
m_animModule          = NULL;
m_charona             = NULL;
m_envReactionAttrib   = 0;
m_surfaceProperty     = 0;
List_triggerList = NULL;
}


void CWldObject::SafeDelete()
{
  if(m_node){
     m_node->SafeDelete();
	 delete m_node;
     m_node = 0;
  }

//delete temp lists
if(m_soundRange){
   m_soundRange->SafeDelete();
   delete m_soundRange;
   m_soundRange=0;
}

if(List_triggerList){
   List_triggerList->SafeDelete();
   delete List_triggerList;
   List_triggerList=0;
}

if(m_meshObject){
   m_meshObject->SafeDelete();
   delete m_meshObject;
   m_meshObject=0;
}

if(List_baseSoundBuffer)
   List_baseSoundBuffer->Release();
   List_baseSoundBuffer=0;
if(List_buffer3DControl)
   List_buffer3DControl->Release();
   List_buffer3DControl=0;

   if(m_containmentObject){
      delete m_containmentObject;
	  m_containmentObject=0;
   }

   if(m_lodVisualList){
      m_lodVisualList->SafeDelete();
      delete m_lodVisualList;
      m_lodVisualList=0;
   }

   if(List_collisionInfoBuffer)
      delete [] List_collisionInfoBuffer;
      List_collisionInfoBuffer=0;
   if(List_attribute)
	  delete List_attribute;
      List_attribute=0;
   if(m_lightObject){
	  delete m_lightObject;
	  m_lightObject=0;
   }
   //end content list
   if(m_animModule)
   {
      m_animModule->SafeDelete();
	  delete m_animModule;
	  m_animModule = 0;
   }

   if(m_charona)
   {
		m_charona->SafeDelete();
		delete m_charona;
		m_charona = 0;

   }
}


//------------------------//
//------------------------//
void CWldObject::AddTrigger(int action,
							int miscInt,
							float activationRadius,
							CString miscTxt,
							D3DVECTOR vectMisc,
							BOOL oneTimeOnly)
{
CTriggerObject * trgPtr = new CTriggerObject(action,
		                                     miscInt,
				                             activationRadius,
											 miscTxt);
trgPtr->m_miscVector.x = vectMisc.x;
trgPtr->m_miscVector.y = vectMisc.y;
trgPtr->m_miscVector.z = vectMisc.z;

trgPtr->m_oneTimeOnly = oneTimeOnly;
List_triggerList->AddTail(trgPtr);
}
//------------------------//
BOOL CWldObject::DeleteTrigger(int index)
{
POSITION posLoc = List_triggerList->FindIndex(index);
 if(posLoc){//valid
    CTriggerObject * trgPtr = (CTriggerObject *)List_triggerList->GetAt(posLoc);
    delete trgPtr;
	trgPtr=0;
	List_triggerList->RemoveAt(posLoc);
  return TRUE;
 }//end valid
 return FALSE;
}
//------------------------//
BOOL CWldObject::EditTrigger(int index,
							 int action,
							 int miscInt,
							 float activationRadius,
							 CString miscTxt,
							 D3DVECTOR miscVect,
							 BOOL oneTimeOnly)
{
POSITION posLoc = List_triggerList->FindIndex(index);
 if(posLoc){//valid
    CTriggerObject * trgPtr = (CTriggerObject *)List_triggerList->GetAt(posLoc);
	trgPtr->m_oneTimeOnly= oneTimeOnly;
    trgPtr->m_action     = action;
	trgPtr->m_indexMisc  = miscInt;
    trgPtr->m_radius     = activationRadius;
	trgPtr->m_miscTxt    = miscTxt;
	trgPtr->m_miscVector.x = miscVect.x;
	trgPtr->m_miscVector.y = miscVect.y;
	trgPtr->m_miscVector.z = miscVect.z;

  return TRUE;
 }//end valid
 return FALSE;
}

//------------------------//
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//---This Function will determine bounding sphere collision----//
BOOL CWldObject::BoundingSphereCheck(float radius1,float radius2,float x1,float y1,float z1,
										float x2,float y2,float z2)
{
float MinDist,dist;
MinDist = radius1+radius2;
MinDist = MinDist * MinDist;
dist = (x1-x2)*(x1-x2);
   if(dist <= MinDist)
   {
      dist = dist+(y1-y2)*(y1-y2);
	  if(dist <= MinDist)
	  {
       dist = dist+(z1-z2)*(z1-z2);
	   if(dist <= MinDist)
	   return TRUE;
	  }
   }
return FALSE;
}


BOOL CWldObject::UpdateLods()
{
   if(!m_lodVisualList)
      return FALSE;

      for(POSITION posLoc = m_lodVisualList->GetHeadPosition();posLoc != NULL;)
	  {
          CMeshLODObject * meshLod = (CMeshLODObject *)m_lodVisualList->GetNext(posLoc);
		  if(meshLod->m_meshLodLevel->List_materialObject)
		  delete meshLod->m_meshLodLevel->List_materialObject;
		  meshLod->m_meshLodLevel->List_materialObject = 0;
          m_meshObject->List_materialObject->Clone(&meshLod->m_meshLodLevel->List_materialObject);
		  
		  CopyMemory(meshLod->m_meshLodLevel->List_BlendMethod,m_meshObject->List_BlendMethod,(sizeof(int)*10));
		  CopyMemory(meshLod->m_meshLodLevel->List_textureSelect,m_meshObject->List_textureSelect,(sizeof(int)*10));
		  
		  meshLod->m_meshLodLevel->List_storedTextureFromFile[0] = m_meshObject->List_storedTextureFromFile[0];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[1] = m_meshObject->List_storedTextureFromFile[1];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[2] = m_meshObject->List_storedTextureFromFile[2];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[3] = m_meshObject->List_storedTextureFromFile[3];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[4] = m_meshObject->List_storedTextureFromFile[4];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[5] = m_meshObject->List_storedTextureFromFile[5];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[6] = m_meshObject->List_storedTextureFromFile[6];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[7] = m_meshObject->List_storedTextureFromFile[7];


	      meshLod->m_meshLodLevel->List_materialObject->Reset();
	  
	  }
	  return TRUE;
}




//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CWldObject::Move(D3DVECTOR translation,D3DVECTOR meshCenter)
{
//reposition

	    m_meshObject->Move(translation,meshCenter);
		m_meshObject->GenerateSmoothNormals();

		List_popoutBox.min.x += translation.x;
		List_popoutBox.min.y += translation.y;
		List_popoutBox.min.z += translation.z;
		List_popoutBox.max.x += translation.x;
		List_popoutBox.max.y += translation.y;
		List_popoutBox.max.z += translation.z;
		m_meshObject->m_meshCenter.x += translation.x;
		m_meshObject->m_meshCenter.y += translation.y;
		m_meshObject->m_meshCenter.z += translation.z;
		m_meshObject->GetBox();
        m_meshObject->GetCenterRadius();
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CWldObject::Rotate(D3DVECTOR rotation,D3DVECTOR meshCenter)
{
//reposition
	    m_meshObject->Rotate(rotation,meshCenter);
		m_meshObject->GenerateSmoothNormals();

		m_meshObject->GetBox();
        m_meshObject->GetCenterRadius();
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CWldObject::Scale(D3DVECTOR scale,D3DVECTOR meshCenter)
{
//reposition
	    m_meshObject->Scale(scale,meshCenter);
		m_meshObject->GenerateSmoothNormals();
		m_meshObject->GetBox();
        m_meshObject->GetCenterRadius();
}

//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CWldObject::CalibrateCenteredBoundingBox(int sightRange)
{
       List_popoutBox.max.x =  ((sightRange+1)+m_meshObject->m_meshCenter.x);
       List_popoutBox.max.y =  ((sightRange+2)+m_meshObject->m_meshCenter.y);
       List_popoutBox.max.z =  ((sightRange+3)+m_meshObject->m_meshCenter.z);
       List_popoutBox.min.x = -(sightRange+4)+m_meshObject->m_meshCenter.x;
       List_popoutBox.min.y = -(sightRange+5)+m_meshObject->m_meshCenter.y;
       List_popoutBox.min.z = -(sightRange+6)+m_meshObject->m_meshCenter.z;
}
//--------------
//--------------
void CWldObject::Serialize( CArchive& ar )
{
D3DVECTOR vectorReserved;
vectorReserved.x = 0.0f;
vectorReserved.y = 0.0f;
vectorReserved.z = 0.0f;
float reservedFloat = 0.0f;
BOOL reservedBOOL = TRUE;
CString reservedCString;
int reservedInt = 0;
int typeSaveVersion = 1;

CObject::Serialize( ar );
	if(ar.IsStoring() ){

        //if(m_animModule)
		   typeSaveVersion = 3;


		ar << List_identity 
		   << List_popoutRadius
           << m_surfaceProperty
		   << m_lightObject
		   << List_collisionType
		   << m_filterGroup
		   << m_envReactionAttrib
		   << typeSaveVersion
		   << reservedCString
		   << List_popoutBox.min.x
		   << List_popoutBox.min.y
		   << List_popoutBox.min.z
		   << List_popoutBox.max.x
		   << List_popoutBox.max.y
		   << List_popoutBox.max.z
		   << List_groupNumber 
           << reservedFloat
		   << List_attribute
		   << List_soundFileName
		   << List_useSound
		   << List_soundFactor
		   << List_isPlaying
		   << List_validRange
		   << List_environmentAttached
		   << List_usedAsSpawnPoint
		   << List_removeEnviorment
		   << List_triggerList
		   << m_containmentObject
		   << m_lodVisualList
           << m_renderInMirrorTag
		   << m_particleDirection.x
           << m_particleDirection.y
           << m_particleDirection.z
           << m_haloLink
           << m_lensFlareType
		   << m_particleLink
		   << m_fileNameRef
		   << m_meshObject
		   << m_collisionInfoFilter
           << m_renderFilter
		   << m_soundRange
		   << m_node;

           if(typeSaveVersion == 2)
		   {
		      ar << m_animModule;
		   }

		   if(typeSaveVersion == 3)
		   {
		      ar << m_animModule;

			  ar << m_charona;
		   }
		
              
		
	}
	else
	{
	
        ar >> List_identity 
		   >> List_popoutRadius
           >> m_surfaceProperty
		   >> m_lightObject
		   >> List_collisionType
		   >> m_filterGroup
		   >> m_envReactionAttrib
		   >> typeSaveVersion
		   >> reservedCString
		   >> List_popoutBox.min.x
		   >> List_popoutBox.min.y
		   >> List_popoutBox.min.z
		   >> List_popoutBox.max.x
		   >> List_popoutBox.max.y
		   >> List_popoutBox.max.z
		   >> List_groupNumber 
           >> reservedFloat
		   >> List_attribute
		   >> List_soundFileName
		   >> List_useSound
		   >> List_soundFactor
		   >> List_isPlaying
		   >> List_validRange
		   >> List_environmentAttached
		   >> List_usedAsSpawnPoint
		   >> List_removeEnviorment
		   >> List_triggerList
		   >> m_containmentObject
		   >> m_lodVisualList
		   >> m_renderInMirrorTag
		   >> m_particleDirection.x
           >> m_particleDirection.y
           >> m_particleDirection.z
           >> m_haloLink
           >> m_lensFlareType
		   >> m_particleLink
		   >> m_fileNameRef;
		   ar >> m_meshObject
		   >> m_collisionInfoFilter
           >> m_renderFilter
		   >> m_soundRange
		   >> m_node;


        m_animModule = NULL;
		m_charona = NULL;

		if(typeSaveVersion == 2)
		   ar >> m_animModule;

		if(typeSaveVersion == 3)
		{
		   ar >> m_animModule;

		   ar >> m_charona;
		}

        m_particleEmmiter = NULL;
		//m_node            = NULL;
		
	
		List_poppedOut = FALSE;
		//m_filterGroup = -1;
		List_miscInt=0;
		List_miscFloat=0;
		List_baseSoundBuffer=0;
		List_buffer3DControl=0;
		List_collisionInfoBuffer = 0;
		List_showBox = FALSE;
		if(m_meshObject)
		   m_meshObject->m_groupReference = List_groupNumber;
	}
}


void CWldObjectList::CacheUnimediateData()
{
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {
     CWldObject * wldObj = (CWldObject *)GetNext(posLoc);
	 if(wldObj->m_meshObject)
	 if(!wldObj->m_animModule)
	 if(wldObj->m_meshObject->List_materialObject)
	 if(!wldObj->m_meshObject->List_materialObject->m_enableTextureMovement)
       if(!wldObj->m_meshObject->m_chromeWrap)
               wldObj->m_meshObject->m_abVertexArray.CacheUnimediateData();
   }
}

void CWldObjectList::SafeDelete()
{
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {
     CWldObject * wldObj = (CWldObject *)GetNext(posLoc);
	 wldObj->SafeDelete();
	 delete wldObj;
	 wldObj = 0;
   }
   RemoveAll();
}

CWldObject * CWldObjectList::GetByUnid(int id)
{
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {
     CWldObject * wldObj = (CWldObject *)GetNext(posLoc);
	 if(wldObj->List_identity == id)
        return wldObj;
   }
   return NULL;
}

void CWldObjectList::IntegrityCheck()
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CWldObject * wldPtr = (CWldObject *)GetNext(posLoc);
		
		if(wldPtr->m_meshObject)
		   if(wldPtr->m_meshObject->m_abVertexArray.IntegrityCheck())
		   {
			   AfxMessageBox("Inconsistency detected and corrected:"+wldPtr->m_fileNameRef);
		   }
	 }
}

CEXMeshObj * CWldObjectList::GetMeshByName(CString meshname)
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CWldObject * wldPtr = (CWldObject *)GetNext(posLoc);
		
		if(wldPtr->m_meshObject)
		   if(wldPtr->m_fileNameRef.CompareNoCase(meshname)==0)
		   {
			   return wldPtr->m_meshObject;
		   }
	 }
	 return NULL;
}


void CWldObjectList::CompileDirectAccess()
{
    int objCount = (int)GetCount();
	if(objCount == 0)
	   return;

	if(directAccess)
	{
       delete [] directAccess;
       directAccess = 0;
	}
    directAccess = new DirectAccessStructWLD[objCount];
	int tracer = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        directAccess[tracer].object = (CWldObject *)GetNext(posLoc);
		tracer++;  
	}
	directAccessObjectCount = tracer;
}



IMPLEMENT_SERIAL(CWldObject, CObject, 0)
IMPLEMENT_SERIAL(CWldObjectList, CObList, 0)



//--------------extra helper functions for this class--------------//

