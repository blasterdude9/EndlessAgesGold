#include "stdafx.h"
#define D3D_OVERLOADS

#include "d3d9.h"
#include <afxtempl.h>
#include "CTextRenderClass.h"
#include "CEXMeshClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------text entry------------------//
CTextureRenderObj::CTextureRenderObj(CString text,
									 int     mode,
									 float   startPosX,
									 float   startPosY,
									 float   fontSizeX,
									 float   fontSizeY,
									 BOOL    tempObj,
									 float   red,
									 float   green,
									 float   blue)
{
    m_vertexObject = NULL;
	m_selfDeleteIt = tempObj;
    m_text = text;
    m_blendMode = mode;
    m_startPosX = startPosX;
    m_startPosY = startPosY;
    m_fontSizeX = fontSizeX;
    m_fontSizeY = fontSizeY;
	m_materialObj = new CMaterialObject();
	m_materialObj->m_baseBlendMode = mode;
	m_materialObj->m_baseMaterial.Ambient.r = 0.0f;
    m_materialObj->m_baseMaterial.Ambient.g = 0.0f;
    m_materialObj->m_baseMaterial.Ambient.b = 0.0f;
    m_materialObj->m_baseMaterial.Ambient.a = 0.0f;
    m_materialObj->m_baseMaterial.Diffuse.r = 0.0f;
    m_materialObj->m_baseMaterial.Diffuse.g = 0.0f;
    m_materialObj->m_baseMaterial.Diffuse.b = 0.0f;
    m_materialObj->m_baseMaterial.Diffuse.a = 0.0f;
    m_materialObj->m_baseMaterial.Emissive.r = red;
    m_materialObj->m_baseMaterial.Emissive.g = green;
    m_materialObj->m_baseMaterial.Emissive.b = blue;
    m_materialObj->m_baseMaterial.Emissive.a = 1.0f;
	m_measuredDistanceRef = 0;
	m_unicodeTempSection = "";

	m_lastStartPosX = -99;
	m_lastStartPosY = -99;
	
	
	m_materialObj->Reset();
}

void CTextureRenderObj::SafeDelete()
{
	if(m_materialObj){
       delete m_materialObj;
       m_materialObj=0;
	}

	if(m_vertexObject)
	{
       m_vertexObject->SafeDelete();
	   delete m_vertexObject;
	   m_vertexObject = 0;

	}
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CTextureRenderObj::Initialize(LPDIRECT3DDEVICE9 g_pd3dDevice,
								   CString textObject,
								   BOOL constantlyChanging,
								   CSymbolMapperObjList * symbolMapSys,
								   int speechMode,
								   float justifyPerc)
{

	           m_materialObj->Callback();

	           //if(m_vertexObject)
			   //if(!constantlyChanging)
	           
			   
			   if(m_vertexObject)
			   if(m_text == textObject)
			   if(m_lastStartPosX == m_startPosX && m_lastStartPosY == m_startPosY)
				  return;

                //justifyPerc = .5f;
			    float justifyOffset = 0.0f;
				if(justifyPerc != 0.0f)
			       justifyOffset = symbolMapSys->GetUnitLength(0,textObject,(m_fontSizeX*2.0f))*justifyPerc;
			   
			    m_lastStartPosX = m_startPosX;
	            m_lastStartPosY = m_startPosY;

               if(m_vertexObject != NULL && textObject.GetLength() != m_text.GetLength())
			   {
				  m_vertexObject->SafeDelete();
			      delete m_vertexObject;
				  m_vertexObject = 0;
			   }
			   if(!m_vertexObject)
               m_vertexObject = new CAdvancedVertexObj();

			   m_text = textObject;
			   m_vertexObject->m_uvCount         = 1;


               

               float m_41 = m_startPosX - justifyOffset;
               float m_42 = m_startPosY;
	           int textLength = m_text.GetLength();

	           float characterSpacing = m_fontSizeX*2.0f;
               float zDist = 3.0f;
               m_vertexObject->m_vertexCount     = textLength * 4; 
	           m_vertexObject->m_indecieCount    = textLength * 6;
			   if(!m_vertexObject->m_vertexArray1)
               m_vertexObject->m_vertexArray1 = new ABVERTEX1L[m_vertexObject->m_vertexCount];
               ZeroMemory(m_vertexObject->m_vertexArray1,sizeof(ABVERTEX1L)*m_vertexObject->m_vertexCount);
               if(!m_vertexObject->m_indecieArray)
			   m_vertexObject->m_indecieArray = new WORD[m_vertexObject->m_indecieCount];
	           //------prepare mesh-------//
	           int vertexTrace = 0;
	           int indecieTrace = 0;
			   BOOL startingChar = TRUE;
			   float lastSpacing = characterSpacing;
			   int skipFunction = 0;
			   int skipCurLength = 0;
			   BOOL enableIt = FALSE;
			   BOOL initIt = FALSE;
			   float currentMultiplier = 1.0f;
			   m_unicodeTempSection = "";
			   //BOOL noAppend = TRUE;
			   CString value,oldValue;
	           for(int loop=0;loop<textLength;loop++)
			   {//BUILD MESH
                  int indexInto = 0;
	              value = (CString)m_text.GetAt(loop);
				  oldValue = value;
	              BOOL foundSymbol = FALSE;

				  if(value == "~")
				  {
                     if((textLength-loop)>1)
					 {
						 value = "";
						 for(int buildX=loop;buildX<textLength;buildX++)
						 {
							 char xtemp = m_text.GetAt(buildX);
						     value += xtemp;
							 if(((int)xtemp) < 58 && ((int)xtemp) > 47)
							 {
							    buildX++;
                                if(buildX<textLength)
								{
								  char xtemp2 = m_text.GetAt(buildX);
							      if(((int)xtemp2) < 58 && ((int)xtemp2) > 47)
								  {
						              value += xtemp2;
								  }
								}
                                if(symbolMapSys->GetByID(value))
								{
							       foundSymbol = TRUE;
								   skipFunction = value.GetLength();
								   m_unicodeTempSection = "";
								   initIt = TRUE; 
								}		
								m_unicodeTempSection = "";
							    initIt = TRUE; 
                                   
								
							    break;
							 }
							 m_unicodeTempSection = value;
							 
						 }
						 if(!foundSymbol)
						 {
					   	     value = "~";
							 
						 }
					 }
				  }

				  
				  //if(symbolMapPtr)
				  //{
				   if(skipFunction > 0)
				   {
                     skipFunction--;
					 if(!initIt)
					    currentMultiplier = 0.0f;
					 else
                        currentMultiplier = 1.0f;
					 initIt = FALSE;
				   }
				   else{
                     currentMultiplier = 1.0f;
				   }
			      //}
                    //indecies
                    

				    
					CSymbolMapperObj * symbolMapPtr = symbolMapSys->GetByID(value);
				    if(symbolMapPtr)
					{//found symbol
					   //m_unicodeTempSection = "";
					   float currentSpacing = (characterSpacing*symbolMapPtr->m_percentageX);//offset;  
				       if(!startingChar){
                         if(symbolMapPtr->m_percentageX > 1.0)
                            lastSpacing = (characterSpacing*symbolMapPtr->m_percentageX);

						 m_41 += (((currentSpacing*.5f)+(lastSpacing*.5f))*currentMultiplier); 
					   }
					   lastSpacing = currentSpacing;

					 startingChar = FALSE;

				    
				     m_vertexObject->m_vertexArray1[vertexTrace].tx0.tu       = symbolMapPtr->m_uvIndexLeft * currentMultiplier;
                     m_vertexObject->m_vertexArray1[vertexTrace].tx0.tv       = symbolMapPtr->m_uvIndexBottom  * currentMultiplier;
		             m_vertexObject->m_vertexArray1[vertexTrace+1].tx0.tu     = symbolMapPtr->m_uvIndexLeft * currentMultiplier; 
                     m_vertexObject->m_vertexArray1[vertexTrace+1].tx0.tv     = symbolMapPtr->m_uvIndexTop * currentMultiplier;
		             m_vertexObject->m_vertexArray1[vertexTrace+2].tx0.tu     = symbolMapPtr->m_uvIndexRight * currentMultiplier;
                     m_vertexObject->m_vertexArray1[vertexTrace+2].tx0.tv     = symbolMapPtr->m_uvIndexTop * currentMultiplier;
	                 m_vertexObject->m_vertexArray1[vertexTrace+3].tx0.tu     = symbolMapPtr->m_uvIndexRight * currentMultiplier;
                     m_vertexObject->m_vertexArray1[vertexTrace+3].tx0.tv     = symbolMapPtr->m_uvIndexBottom * currentMultiplier;
	 
					 
                     //verticies
                     m_vertexObject->m_vertexArray1[vertexTrace].x = (-m_fontSizeX * currentMultiplier) + m_41;
                     m_vertexObject->m_vertexArray1[vertexTrace].y = -m_fontSizeY + m_42;
                     
                     m_vertexObject->m_vertexArray1[vertexTrace+1].x = (-m_fontSizeX * currentMultiplier) + m_41;
                     m_vertexObject->m_vertexArray1[vertexTrace+1].y = m_fontSizeY +  m_42;
                     
                     m_vertexObject->m_vertexArray1[vertexTrace+2].x = (m_fontSizeX * currentMultiplier) + m_41;
                     m_vertexObject->m_vertexArray1[vertexTrace+2].y = m_fontSizeY + m_42;
                     
                     m_vertexObject->m_vertexArray1[vertexTrace+3].x = (m_fontSizeX * currentMultiplier) +  m_41;
                     m_vertexObject->m_vertexArray1[vertexTrace+3].y = -m_fontSizeY + m_42;
                    
					}//found symbol
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

		            vertexTrace += 4;
		            indecieTrace += 6;
		            
			   }//end BUILD MESH
	           m_41 = m_startPosX;
               m_42 = m_startPosY;
			   
			   m_vertexObject->InitBuffer(g_pd3dDevice,m_vertexObject->m_uvCount);
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CTextureRenderObj::Render(LPDIRECT3DDEVICE9 g_pd3dDevice,CStateManagementObj * stateManager)
{
     if(m_vertexObject)
	 {
	    stateManager->SetDiffuseSource(m_vertexObject->m_advancedFixedMode,g_pd3dDevice);
	    stateManager->SetVertexType(m_vertexObject->m_uvCount,g_pd3dDevice);
	    m_vertexObject->Render(g_pd3dDevice,stateManager->m_currentVertexType); 
	 }
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CTextureRenderObjList::Flush()
{
   RemoveAll();
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
bool CTextureRenderObjList::InitializeExternalMesh(LPDIRECT3DDEVICE9 g_pd3dDevice,
												   CString textObject,
												   float fontSizeX,
												   float fontSizeY,
												   CSymbolMapperObjList * symbolMapSys,
												   int speechMode,
												   float justifyPerc,
												   void ** meshRef,
												   int mode,
												   float red,
												   float green,
												   float blue,
												   float newLineScaleOpt)
{
				bool scaled = false;
               CEXMeshObj * meshBuild = (CEXMeshObj *)*meshRef;
			   if(meshBuild)
			   {
			      meshBuild->SafeDelete();
                  delete meshBuild;
				  meshBuild = 0;
			   }
			   //reallocate
			   meshBuild = new CEXMeshObj();
			   meshBuild->List_materialObject = new CMaterialObject();
			   meshBuild->List_materialObject->m_baseBlendMode = mode;
			   meshBuild->List_materialObject->m_baseMaterial.Ambient.r = 0.0f;
			   meshBuild->List_materialObject->m_baseMaterial.Ambient.g = 0.0f;
			   meshBuild->List_materialObject->m_baseMaterial.Ambient.b = 0.0f;
			   meshBuild->List_materialObject->m_baseMaterial.Ambient.a = 0.0f;
			   meshBuild->List_materialObject->m_baseMaterial.Diffuse.r = 0.0f;
			   meshBuild->List_materialObject->m_baseMaterial.Diffuse.g = 0.0f;
			   meshBuild->List_materialObject->m_baseMaterial.Diffuse.b = 0.0f;
			   meshBuild->List_materialObject->m_baseMaterial.Diffuse.a = 0.0f;
			   meshBuild->List_materialObject->m_baseMaterial.Emissive.r = red;
			   meshBuild->List_materialObject->m_baseMaterial.Emissive.g = green;
			   meshBuild->List_materialObject->m_baseMaterial.Emissive.b = blue;
			   meshBuild->List_materialObject->m_baseMaterial.Emissive.a = 1.0f;
	           meshBuild->List_materialObject->Reset();
			   meshBuild->List_materialObject->m_fogFilter = TRUE;
				
			   

			   meshBuild->m_abVertexArray.m_uvCount         = 1;

	           int textLength = textObject.GetLength();

	           float characterSpacing = fontSizeX*2.0f;
               float zDist = 0.0f;
               meshBuild->m_abVertexArray.m_vertexCount     = textLength * 4; 
	           meshBuild->m_abVertexArray.m_indecieCount    = textLength * 6;
               meshBuild->m_abVertexArray.m_vertexArray1 = new ABVERTEX1L[meshBuild->m_abVertexArray.m_vertexCount];
               ZeroMemory(meshBuild->m_abVertexArray.m_vertexArray1,sizeof(ABVERTEX1L)*meshBuild->m_abVertexArray.m_vertexCount);
			   meshBuild->m_abVertexArray.m_indecieArray = new WORD[meshBuild->m_abVertexArray.m_indecieCount];
	           //------prepare mesh-------//
	           int vertexTrace = 0;
	           int indecieTrace = 0;
			   BOOL startingChar = TRUE;
			   float lastSpacing = characterSpacing;
			   int skipFunction = 0;
			   int skipCurLength = 0;
			   BOOL enableIt = FALSE;
			   BOOL initIt = FALSE;
			   float currentMultiplier = 1.0f;
			   CString m_unicodeTempSection = "";
			   //BOOL noAppend = TRUE;
			   CString value,oldValue;


               float justifyOffset = 0.0f;
			   if(justifyPerc != 0.0f)
			       justifyOffset = symbolMapSys->GetUnitLength(0,textObject,(fontSizeX*2.0f))*justifyPerc;
               float m_41 = -justifyOffset;
               float m_42 = 0.0f;
               BOOL foundSymbol = FALSE;
			   int indexInto = 0;
	           for(int loop=0;loop<textLength;loop++)
			   {//BUILD MESH
                  indexInto = 0;
	              value = (CString)textObject.GetAt(loop);
				  oldValue = value;
	              foundSymbol = FALSE;

				  if(value == "\n")
				  {
					 m_42 += (fontSizeY * 2.0f);
                     //second line size option
				     if(!scaled)
					 {
						fontSizeX *= newLineScaleOpt;
						fontSizeY *= newLineScaleOpt;
						scaled = true;
					 }

				     startingChar = TRUE;
					 characterSpacing = fontSizeX*2.0f;
					 lastSpacing = characterSpacing;
                     

					 if(loop+1<textLength)
					 {
						 justifyOffset = 0.0f;
						 if(justifyPerc != 0.0f)
							justifyOffset = symbolMapSys->GetUnitLength(loop+1,textObject,(fontSizeX*2.0f))*justifyPerc;
						 m_41 = -justifyOffset;
					 }
					 meshBuild->m_abVertexArray.m_indecieCount -= 6;
					 
					 continue;
				  }

				  if(value == "~")
				  {
                     if((textLength-loop)>1)
					 {
						 value = "";
						 for(int buildX=loop;buildX<textLength;buildX++)
						 {
							 char xtemp = textObject.GetAt(buildX);
						     value += xtemp;
							 if(((int)xtemp) < 58 && ((int)xtemp) > 47)
							 {
							    buildX++;
                                if(buildX<textLength)
								{
								  char xtemp2 = textObject.GetAt(buildX);
							      if(((int)xtemp2) < 58 && ((int)xtemp2) > 47)
								  {
						              value += xtemp2;
								  }
								}
                                if(symbolMapSys->GetByID(value))
								{
							       foundSymbol = TRUE;
								   skipFunction = value.GetLength();
								   m_unicodeTempSection = "";
								   initIt = TRUE; 
								}		
								m_unicodeTempSection = "";
							    initIt = TRUE; 
                                   
								
							    break;
							 }
							 m_unicodeTempSection = value;
							 
						 }
						 if(!foundSymbol)
						 {
					   	     value = "~";
							 
						 }
					 }
				  }

				  
				  //if(symbolMapPtr)
				  //{
				   if(skipFunction > 0)
				   {
                     skipFunction--;
					 if(!initIt)
					    currentMultiplier = 0.0f;
					 else
                        currentMultiplier = 1.0f;
					 initIt = FALSE;
				   }
				   else{
                     currentMultiplier = 1.0f;
				   }
			      //}
                    //indecies
                    

				    
					CSymbolMapperObj * symbolMapPtr = symbolMapSys->GetByID(value);
				    if(symbolMapPtr)
					{//found symbol
					   //m_unicodeTempSection = "";
					   float currentSpacing = (characterSpacing*symbolMapPtr->m_percentageX);//offset;  
				       if(!startingChar){
                         if(symbolMapPtr->m_percentageX > 1.0)
                            lastSpacing = (characterSpacing*symbolMapPtr->m_percentageX);

						 m_41 += (((currentSpacing*.5f)+(lastSpacing*.5f))*currentMultiplier); 
					   }
					   lastSpacing = currentSpacing;

					 startingChar = FALSE;

				    
				     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace].tx0.tu       = symbolMapPtr->m_uvIndexLeft * currentMultiplier;
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace].tx0.tv       = symbolMapPtr->m_uvIndexBottom  * currentMultiplier;
		             meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+1].tx0.tu     = symbolMapPtr->m_uvIndexLeft * currentMultiplier; 
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+1].tx0.tv     = symbolMapPtr->m_uvIndexTop * currentMultiplier;
		             meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+2].tx0.tu     = symbolMapPtr->m_uvIndexRight * currentMultiplier;
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+2].tx0.tv     = symbolMapPtr->m_uvIndexTop * currentMultiplier;
	                 meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+3].tx0.tu     = symbolMapPtr->m_uvIndexRight * currentMultiplier;
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+3].tx0.tv     = symbolMapPtr->m_uvIndexBottom * currentMultiplier;
	 
					 
                     //verticies
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace].x = (-fontSizeX * currentMultiplier) + m_41;
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace].y = -fontSizeY + m_42;
                     
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+1].x = (-fontSizeX * currentMultiplier) + m_41;
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+1].y = fontSizeY +  m_42;
                     
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+2].x = (fontSizeX * currentMultiplier) + m_41;
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+2].y = fontSizeY + m_42;
                     
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+3].x = (fontSizeX * currentMultiplier) +  m_41;
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+3].y = -fontSizeY + m_42;
                    
					}//found symbol
					 meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace].z = zDist;
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace].nx = 0.0f;
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace].ny = 0.0f;
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace].nz = 1.0f;
					 meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+1].z = zDist;
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+1].nx = 0.0f;
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+1].ny = 0.0f;
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+1].nz = 1.0f;
					 meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+2].z = zDist;
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+2].nx = 0.0f;
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+2].ny = 0.0f;
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+2].nz = 1.0f;
					 meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+3].z = zDist;
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+3].nx = 0.0f;
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+3].ny = 0.0f;
                     meshBuild->m_abVertexArray.m_vertexArray1[vertexTrace+3].nz = 1.0f;
				    //indecies
				    meshBuild->m_abVertexArray.m_indecieArray[indecieTrace] = vertexTrace;
                    meshBuild->m_abVertexArray.m_indecieArray[indecieTrace+1] = vertexTrace+1;
                    meshBuild->m_abVertexArray.m_indecieArray[indecieTrace+2] = vertexTrace+2;
                    meshBuild->m_abVertexArray.m_indecieArray[indecieTrace+3] = vertexTrace+2;
                    meshBuild->m_abVertexArray.m_indecieArray[indecieTrace+4] = vertexTrace+3;
                    meshBuild->m_abVertexArray.m_indecieArray[indecieTrace+5] = vertexTrace;

		            vertexTrace += 4;
		            indecieTrace += 6;
		            
			   }//end BUILD MESH
			   
			   meshBuild->m_abVertexArray.InitBuffer(g_pd3dDevice,meshBuild->m_abVertexArray.m_uvCount);
			   *meshRef = meshBuild;
			   return true;
}

//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CTextureRenderObjList::DeleteFinishedTextDisplays()
{
   POSITION posLast;
   for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
   {
       CTextureRenderObj * textPtr = (CTextureRenderObj *)GetNext(posLoc);
	   if(textPtr->m_selfDeleteIt == TRUE){
          textPtr->SafeDelete();
		  delete textPtr;
          textPtr=0;
		  RemoveAt(posLast);
	   }
   }
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CTextureRenderObjList::SafeDelete()
{
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {
       CTextureRenderObj * textPtr = (CTextureRenderObj *)GetNext(posLoc);
	   textPtr->SafeDelete();
       delete textPtr;
	   textPtr = 0;
   }
   RemoveAll();
}

void CTextureRenderObjList::AddTextObject(CString text,
									      int     mode,
									      float   startPosX,
									      float   startPosY,
									      float   fontSizeX,
									      float   fontSizeY,
										  BOOL    tempObj)
{

    CTextureRenderObj * textPtr = new CTextureRenderObj(text,
									                    mode,
									                    startPosX,
									                    startPosY,
									                    fontSizeX,
									                    fontSizeY,
														tempObj,
														1.0f,
														1.0f,
														1.0f);
	AddTail(textPtr);
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
CSymbolMapperObj::CSymbolMapperObj()
{
    m_percentageX     = 1.0f;
	m_uniqueID        = "";//used to assign char reference
    m_uvIndexLeft     = 0.0f;
	m_uvIndexRight    = 0.0f;
	m_uvIndexTop      = 0.0f;
	m_uvIndexBottom   = 0.0f;
	m_gridDensity     = 16;
	m_origGridIndexHoriz = 0.0f;
    m_origGridIndexVert  = 0.0f;
	m_group           = "";
	m_extendedOption = "";
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CSymbolMapperObj::Calculate(int gridDensity,
								 float horizOffset,
								 float vertOffset,
								 CString id,
								 float xPercentage)
{
	if(gridDensity == 0)
	{
	   AfxMessageBox("Cannot have a zero density Grid");
       return;
	}

	
    m_origGridIndexHoriz = horizOffset;
    m_origGridIndexVert  = vertOffset;
	m_gridDensity     = gridDensity;
    m_percentageX     = xPercentage;
	m_uniqueID        = id;
	//do it
	float spacing = 1.0f / ((float)gridDensity);


    m_uvIndexLeft     = horizOffset * spacing;
	m_uvIndexRight    = (horizOffset+1) * spacing;
	m_uvIndexTop      = vertOffset * spacing;
	m_uvIndexBottom   = (vertOffset+1) * spacing;
	//rescale UV Horiz useing percentage

	/*float origSz = m_uvIndexRight - m_uvIndexLeft;
	float newSz = origSz * xPercentage;
	float movAmt = (origSz - newSz)*.5f;
	m_uvIndexLeft = m_uvIndexLeft + movAmt;
    m_uvIndexRight = m_uvIndexRight - movAmt;*/
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CSymbolMapperObj::Serialize( CArchive& ar )
{
BOOL reserved = 0;
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar.SetObjectSchema(2);
	   ASSERT(ar.GetObjectSchema() == 2);
	   ar << m_percentageX
	      << m_uniqueID
	      << m_uvIndexLeft
	      << m_uvIndexRight
	      << m_uvIndexTop
		  << m_uvIndexBottom
		  << m_gridDensity
		  << m_origGridIndexHoriz
		  << m_origGridIndexVert
		  << reserved
		  << reserved
		  << m_group
		  << m_extendedOption;
	}
	else
	{
       int version = ar.GetObjectSchema();

	   if(version < 2)
	   {
	   unsigned int tempLoc;
       ar >> m_percentageX
	      >> tempLoc
	      >> m_uvIndexLeft
	      >> m_uvIndexRight
	      >> m_uvIndexTop
		  >> m_uvIndexBottom
		  >> m_gridDensity
		  >> m_origGridIndexHoriz
		  >> m_origGridIndexVert;
		  char temploc2[2];
	      temploc2[0] = (char)tempLoc;
		  temploc2[1] = 0;
	      m_uniqueID = temploc2;
		  m_group = "";
		  m_extendedOption = "";
		  
	   }
	   else{
       ar >> m_percentageX
	      >> m_uniqueID
	      >> m_uvIndexLeft
	      >> m_uvIndexRight
	      >> m_uvIndexTop
		  >> m_uvIndexBottom
		  >> m_gridDensity
		  >> m_origGridIndexHoriz
		  >> m_origGridIndexVert
		  >> reserved
		  >> reserved
	      >> m_group
		  >> m_extendedOption;
	   }

       /*if(m_percentageX == 1.0)
	      m_percentageX = 1.02f;
	   if(m_percentageX == 1.1)
	      m_percentageX = 1.02f;*/
	   

	}
}     
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CSymbolMapperObjList::SafeDelete()
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CSymbolMapperObj * spnPtr = (CSymbolMapperObj *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	 }
	 RemoveAll();
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
float CSymbolMapperObjList::GetUnitLength(int startPoint,CString string,float fontSize)
{
     int count = string.GetLength();
   
	 BOOL startingChar = TRUE;
	 float lastSpacing = fontSize;
     float length = 0.0f;
	 float currentSpacing = 0.0f;
	 for(int loop=startPoint;loop<count;loop++)
	 {
         CSymbolMapperObj * symbol = (CSymbolMapperObj *) GetByID((CString)string.GetAt(loop));
         if(!symbol)
			 break;
		 //length += (fontSize * symbol->m_percentageX);

		 currentSpacing = (fontSize*symbol->m_percentageX);//offset;  
		 if(!startingChar)
		 {
         if(symbol->m_percentageX > 1.0)
            lastSpacing = (fontSize*symbol->m_percentageX);

		 length += (((currentSpacing*.5f)+(lastSpacing*.5f))); 
		 }
		 lastSpacing = currentSpacing;
         startingChar = FALSE;
	 }
	 return length;
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
CSymbolMapperObj * CSymbolMapperObjList::GetByID(CString id)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CSymbolMapperObj * spnPtr = (CSymbolMapperObj *)GetNext(posLoc);
		if(spnPtr->m_uniqueID == id)
		{
           return spnPtr;
		}
	}
	return NULL;
}

IMPLEMENT_SERIAL(CSymbolMapperObj, CObject, VERSIONABLE_SCHEMA|2)
IMPLEMENT_SERIAL(CSymbolMapperObjList, CObList, 0)

