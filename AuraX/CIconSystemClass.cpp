#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CIconSystemClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CIconSysObj::CIconSysObj()
{
   m_iconName     = "name";
   m_vertexObject = NULL;
   m_materialObj  = NULL;
   m_gridDensity  = 4.0f;
   m_xIndex       = 0.0f;
   m_yIndex       = 0.0f;
   m_size         = 0.2f;
   m_textureIndex = -1;
   m_textureName  = "NONE";
   m_controlIDRef = 0;
   m_controlContentIDRef = 0;
   ZeroMemory(&m_refPosition,sizeof(D3DVECTOR));
   m_refreshed = FALSE;
   m_indexRef = 0;
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CIconSysObj::Clone(CIconSysObj ** clone)
{
    CIconSysObj * copyLocal = new CIconSysObj();

	copyLocal->m_iconName     = "name";
    copyLocal->m_gridDensity  = m_gridDensity;
    copyLocal->m_xIndex       = m_xIndex;
    copyLocal->m_yIndex       = m_yIndex;
    copyLocal->m_size         = m_size;
	copyLocal->m_textureIndex = m_textureIndex;
	copyLocal->m_textureName  = m_textureName;
	copyLocal->m_controlIDRef = m_controlIDRef;
    copyLocal->m_controlContentIDRef = m_controlContentIDRef;
    copyLocal->m_refPosition.x =  m_refPosition.x;
	copyLocal->m_refPosition.y =  m_refPosition.y;
	copyLocal->m_refPosition.z =  m_refPosition.z;
    copyLocal->m_refreshed    = m_refreshed;
	copyLocal->m_indexRef     = m_indexRef;


	if(m_materialObj)
       m_materialObj->Clone(&copyLocal->m_materialObj);

	*clone = copyLocal;
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CIconSysObj::Initialize(D3DVECTOR position,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     if(!m_vertexObject)
         m_vertexObject = new CAdvancedVertexObj();

	 if(!m_materialObj)
	     m_materialObj = new CMaterialObject();

	 m_vertexObject->m_uvCount         = 1;

     float m_41 = position.x;
     float m_42 = position.y;
     float zDist = position.z;
     m_vertexObject->m_vertexCount     = 4; 
	 m_vertexObject->m_indecieCount    = 6;
	 if(!m_vertexObject->m_vertexArray1)
         m_vertexObject->m_vertexArray1 = new ABVERTEX1L[m_vertexObject->m_vertexCount];
         ZeroMemory(m_vertexObject->m_vertexArray1,sizeof(ABVERTEX1L)*m_vertexObject->m_vertexCount);
     if(!m_vertexObject->m_indecieArray)
	     m_vertexObject->m_indecieArray = new WORD[m_vertexObject->m_indecieCount];
	         //------prepare mesh-------//
	         int vertexTrace = 0;
	         int indecieTrace = 0;
		     BOOL startingChar = TRUE;
	         //BUILD MESH
                int indexInto = 0;
                float uvLeft,uvRight,uvUp,uvDown;

                float sectionSz = 1.0f/m_gridDensity;
                uvLeft = sectionSz * m_xIndex;
				uvRight = sectionSz * (m_xIndex+1);
		        uvUp = sectionSz * m_yIndex;
				uvDown = sectionSz * (m_yIndex+1);
			       
                   startingChar = FALSE;    
				   m_vertexObject->m_vertexArray1[vertexTrace].tx0.tu       = uvLeft;
                   m_vertexObject->m_vertexArray1[vertexTrace].tx0.tv       = uvDown;
		           m_vertexObject->m_vertexArray1[vertexTrace+1].tx0.tu     = uvLeft; 
                   m_vertexObject->m_vertexArray1[vertexTrace+1].tx0.tv     = uvUp;
		           m_vertexObject->m_vertexArray1[vertexTrace+2].tx0.tu     = uvRight;
                   m_vertexObject->m_vertexArray1[vertexTrace+2].tx0.tv     = uvUp;
	               m_vertexObject->m_vertexArray1[vertexTrace+3].tx0.tu     = uvRight;
                   m_vertexObject->m_vertexArray1[vertexTrace+3].tx0.tv     = uvDown;
	 
                   //verticies
                   m_vertexObject->m_vertexArray1[vertexTrace].x = -m_size + m_41;
                   m_vertexObject->m_vertexArray1[vertexTrace].y = -m_size + m_42;
                     
                   m_vertexObject->m_vertexArray1[vertexTrace+1].x = -m_size + m_41;
                   m_vertexObject->m_vertexArray1[vertexTrace+1].y = m_size +  m_42;
                     
                     m_vertexObject->m_vertexArray1[vertexTrace+2].x = m_size + m_41;
                     m_vertexObject->m_vertexArray1[vertexTrace+2].y = m_size + m_42;
                     
                     m_vertexObject->m_vertexArray1[vertexTrace+3].x = m_size +  m_41;
                     m_vertexObject->m_vertexArray1[vertexTrace+3].y = -m_size + m_42;
                    
					//}//found symbol
					 m_vertexObject->m_vertexArray1[vertexTrace].z = zDist;
                     m_vertexObject->m_vertexArray1[vertexTrace].nx = 0.0f;
                     m_vertexObject->m_vertexArray1[vertexTrace].ny = 0.0f;
                     m_vertexObject->m_vertexArray1[vertexTrace].nz = -1.0f;
					 m_vertexObject->m_vertexArray1[vertexTrace+1].z = zDist;
                     m_vertexObject->m_vertexArray1[vertexTrace+1].nx = 0.0f;
                     m_vertexObject->m_vertexArray1[vertexTrace+1].ny = 0.0f;
                     m_vertexObject->m_vertexArray1[vertexTrace+1].nz = -1.0f;
					 m_vertexObject->m_vertexArray1[vertexTrace+2].z = zDist;
                     m_vertexObject->m_vertexArray1[vertexTrace+2].nx = 0.0f;
                     m_vertexObject->m_vertexArray1[vertexTrace+2].ny = 0.0f;
                     m_vertexObject->m_vertexArray1[vertexTrace+2].nz = -1.0f;
					 m_vertexObject->m_vertexArray1[vertexTrace+3].z = zDist;
                     m_vertexObject->m_vertexArray1[vertexTrace+3].nx = 0.0f;
                     m_vertexObject->m_vertexArray1[vertexTrace+3].ny = 0.0f;
                     m_vertexObject->m_vertexArray1[vertexTrace+3].nz = -1.0f;
				    //indecies
				    m_vertexObject->m_indecieArray[indecieTrace] = vertexTrace;
                    m_vertexObject->m_indecieArray[indecieTrace+1] = vertexTrace+1;
                    m_vertexObject->m_indecieArray[indecieTrace+2] = vertexTrace+2;
                    m_vertexObject->m_indecieArray[indecieTrace+3] = vertexTrace+2;
                    m_vertexObject->m_indecieArray[indecieTrace+4] = vertexTrace+3;
                    m_vertexObject->m_indecieArray[indecieTrace+5] = vertexTrace;
			   
			   m_vertexObject->InitBuffer(g_pd3dDevice,m_vertexObject->m_uvCount);

}

//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CIconSysObj::Render(LPDIRECT3DDEVICE9 g_pd3dDevice,
						 CStateManagementObj * stateManager,
						 CTextureDBObjList * textureDatabase)
{
     if(!m_vertexObject)
		 return;

     if(!m_materialObj)
		 return;
   
	 for(int loop=1;loop<stateManager->m_multiTextureSupported;loop++)
         stateManager->SetBlendLevel(loop,stateManager->m_intNegOne,g_pd3dDevice);

	 //g_pd3dDevice->SetMaterial(&m_materialObj->m_useMaterial);  
	 stateManager->SetMaterial(g_pd3dDevice,&m_materialObj->m_useMaterial);
	 stateManager->SetTextureState(m_textureIndex,stateManager->m_intZero,g_pd3dDevice,textureDatabase);
	 stateManager->SetBlendType(g_pd3dDevice,m_materialObj->m_blendMode);
	 stateManager->SetDiffuseSource(m_vertexObject->m_advancedFixedMode,g_pd3dDevice);
	 stateManager->SetVertexType(m_vertexObject->m_uvCount,g_pd3dDevice);
	 m_vertexObject->Render(g_pd3dDevice,stateManager->m_currentVertexType);               
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CIconSysObj::SafeDelete()
{
   if(m_vertexObject)
   {
      m_vertexObject->SafeDelete();
	  delete m_vertexObject;
	  m_vertexObject = 0;
   }

   if(m_materialObj)
   {
	  delete m_materialObj;
	  m_materialObj = 0;
   }
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CIconSysObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_iconName
	      << m_materialObj
		  << m_vertexObject
	      << m_gridDensity
	      << m_xIndex
	      << m_yIndex
		  << m_size
		  << m_textureIndex
          << m_textureName;

	}
	else
	{
       ar >> m_iconName
	      >> m_materialObj
		  >> m_vertexObject
	      >> m_gridDensity
	      >> m_xIndex
	      >> m_yIndex
		  >> m_size
		  >> m_textureIndex
          >> m_textureName;

	   m_controlIDRef = 0;
       m_controlContentIDRef = 0;
       ZeroMemory(&m_refPosition,sizeof(D3DVECTOR));
	   m_refreshed = FALSE;
	   m_indexRef = 0;
	}
}     
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CIconSysList::SafeDelete()
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
         CIconSysObj * iconPtr = (CIconSysObj *) GetNext(posLoc);
		 iconPtr->SafeDelete();
         delete iconPtr;
		 iconPtr = 0;
	 }
	 RemoveAll();
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
CIconSysObj * CIconSysList::GetByIndex(int index)
{
    if(index < 0)
	   return NULL;

     POSITION posLoc = FindIndex(index);
	 if(posLoc)
	 {
        CIconSysObj * icPtr = (CIconSysObj *)GetAt(posLoc);
        return icPtr;
	 }

	 return NULL;
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
BOOL CIconSysList::AddIconFromDBtoThisDB(CIconSysList * primaryDB,
										 int index,
										 D3DVECTOR screenPos,
										 LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     CIconSysObj * icPtr = primaryDB->GetByIndex(index);
	 if(!icPtr)
	     return FALSE;
	
	 
         CIconSysObj * newIcPtr = NULL;
         icPtr->Clone(&newIcPtr);//copys material and densities
         newIcPtr->Initialize(screenPos,g_pd3dDevice);
		 newIcPtr->m_controlIDRef = -300;
	     AddTail(newIcPtr);
         return TRUE;
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
BOOL CIconSysList::AddIconFromDBtoThisDBWspecs(CIconSysList * primaryDB,
										       int index,
										       D3DVECTOR screenPos,
										       LPDIRECT3DDEVICE9 g_pd3dDevice,
											   int controlIDRef,
                                               int controlContentIDRef,
											   float scale,
											   BOOL temp)
{
     CIconSysObj * seekIcon = this->GetIconByCriteria(g_pd3dDevice,
							                    controlIDRef,
                                                controlContentIDRef,
											    screenPos,
												index);

	 if(seekIcon)
	 {//already correct just refresh 
         seekIcon->m_refreshed = TRUE;
		 return TRUE;
	 }//end already correct just refresh 
  

     CIconSysObj * icPtr = primaryDB->GetByIndex(index);
	 if(!icPtr)
	     return FALSE;
	
         CIconSysObj * newIcPtr = NULL;
         icPtr->Clone(&newIcPtr);//copys material and densities
		 newIcPtr->m_size = newIcPtr->m_size * scale;
         newIcPtr->Initialize(screenPos,g_pd3dDevice);
		 newIcPtr->m_indexRef = index;
		 CopyMemory(&newIcPtr->m_refPosition,&screenPos,sizeof(D3DVECTOR));
		 newIcPtr->m_controlIDRef = controlIDRef;
		 newIcPtr->m_controlContentIDRef = controlContentIDRef;
		 newIcPtr->m_refreshed = TRUE;
		 newIcPtr->m_tempIcon = temp;
	     AddTail(newIcPtr);
         return TRUE;
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
CIconSysObj * CIconSysList::GetIconByCriteria(LPDIRECT3DDEVICE9 g_pd3dDevice,
							                  int controlIDRef,
                                              int controlContentIDRef,
										      D3DVECTOR position,
											  int indexRef
											  )
{
	 for(POSITION posLoc = this->GetHeadPosition();posLoc != NULL;)
	 {
         CIconSysObj * icPtr = (CIconSysObj *)this->GetNext(posLoc);
		 if(icPtr->m_indexRef == indexRef)
		 if(controlIDRef == icPtr->m_controlIDRef)
			 if(controlContentIDRef == icPtr->m_controlContentIDRef)
			    if(icPtr->m_refPosition.x == position.x && icPtr->m_refPosition.y == position.y && icPtr->m_refPosition.z == position.z)
			    {


				 return icPtr;
			    }
	 }
     return NULL;
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CIconSysList::ClearTempIcons()
{
	POSITION posLast = NULL;
	for(POSITION posLoc = this->GetHeadPosition();(posLast = posLoc) != NULL;)
	{
		CIconSysObj * icPtr = (CIconSysObj *)this->GetNext(posLoc);
		if(icPtr->m_tempIcon)
		{
			icPtr->SafeDelete();
			delete icPtr;
			icPtr = 0;
			this->RemoveAt(posLast);
		}
	}
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
BOOL CIconSysList::TagAllRefreshed(LPDIRECT3DDEVICE9 g_pd3dDevice,
							       int controlIDRef,
								   BOOL refreshState)
{
	 for(POSITION posLoc = this->GetHeadPosition();posLoc != NULL;)
	 {
         CIconSysObj * icPtr = (CIconSysObj *)this->GetNext(posLoc);
		 if(controlIDRef == icPtr->m_controlIDRef)
	        icPtr->m_refreshed = refreshState;
	 }
     return TRUE;
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
BOOL CIconSysList::RemoveAllNonRefreshed(LPDIRECT3DDEVICE9 g_pd3dDevice,
							             int controlIDRef)
{
	POSITION posLast = NULL;
	 for(POSITION posLoc = this->GetHeadPosition();(posLast = posLoc) != NULL;)
	 {
         CIconSysObj * icPtr = (CIconSysObj *)this->GetNext(posLoc);
		 if(controlIDRef == icPtr->m_controlIDRef && icPtr->m_refreshed == FALSE)
		 {
			 icPtr->SafeDelete();
			 delete icPtr;
			 icPtr = 0;
	         this->RemoveAt(posLast);
		 }
	 }
     return TRUE;
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CIconSysList::RenderAll(LPDIRECT3DDEVICE9 g_pd3dDevice,
						     CStateManagementObj * stateManager,
						     CTextureDBObjList * textureDatabase)
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
         CIconSysObj * iconPtr = (CIconSysObj *) GetNext(posLoc);
		 iconPtr->Render(g_pd3dDevice,stateManager,textureDatabase);
	 }
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CIconSysList::Initialize(D3DVECTOR position,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
         CIconSysObj * iconPtr = (CIconSysObj *) GetNext(posLoc);
		 iconPtr->Initialize(position,g_pd3dDevice);
	 }
}



IMPLEMENT_SERIAL(CIconSysObj, CObject, 0)
IMPLEMENT_SERIAL(CIconSysList, CObList, 0)

