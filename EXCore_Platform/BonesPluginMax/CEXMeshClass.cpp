//#include "stdafx.h"
#include "afxwin.h"
#include "afxext.h"
#define D3D_OVERLOADS
#include "resource.h"
#include "math.h"
#include <d3d9.h>
//#include <afxtempl.h>
//#include "afxext.h"
//

 #include "CEXMeshClass.h"


CEXMeshObj::CEXMeshObj()
{
m_lastTimeStamp=0;
m_chromeWrap = FALSE;
m_chromeUvSet = 0;
m_groupReference = -1;

List_boundingBox.minX = 0.0f;
List_boundingBox.maxX = 0.0f;
List_boundingBox.minY = 0.0f;
List_boundingBox.maxY = 0.0f;
List_boundingBox.minZ = 0.0f;
List_boundingBox.maxZ = 0.0f;
List_boundingRadius = 0.0f;

List_BlendMethod[0] = -1;
List_BlendMethod[1] = -1;
List_BlendMethod[2] = -1;
List_BlendMethod[3] = -1;
List_BlendMethod[4] = -1;
List_BlendMethod[5] = -1;
List_BlendMethod[6] = -1;
List_BlendMethod[7] = -1;
List_BlendMethod[8] = -1;
List_BlendMethod[9] = -1;
m_tempDistance = 0;
List_blendType = -1;//-1 = none 1 = even opacity 2 = opacity map darker is more transparent
List_textureSetList=0;

for(int loop=0;loop<10;loop++)
List_textureSelect[loop] = -1;
List_matrixFunctions.InitMat(m_currentMatrix.m);

m_meshCenter.x             = 0.0f;
m_meshCenter.y             = 0.0f;
m_meshCenter.z             = 0.0f;
m_meshBoundingSphereRadius = 0.0f;
List_materialObject        = NULL;
m_visibilityTrack          = 1;

m_charona = NULL;
m_shadowTemp = NULL;
m_validUvCount = 0;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CEXMeshObj::scaleMat ( float Xt, float Yt, float Zt,float TMatrix [ 4 ] [ 4 ] )
{
   TMatrix[0][0]=Xt; TMatrix[0][1]=0;   TMatrix[0][2]=0;  TMatrix[0][3]=0;
   TMatrix[1][0]=0;  TMatrix[1][1]=Yt;  TMatrix[1][2]=0;  TMatrix[1][3]=0;
   TMatrix[2][0]=0;  TMatrix[2][1]=0;   TMatrix[2][2]=Zt; TMatrix[2][3]=0;
   TMatrix[3][0]=0;  TMatrix[3][1]=0;   TMatrix[3][2]=0;  TMatrix[3][3]=1;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CEXMeshObj::OptimizeMesh(CDialogBar *wndUpdate)
{
int barProgress = 0;

ABVERTEX * tempArray = NULL;
int vCount = 0;

m_abVertexArray.GetABVERTEXStyle(&tempArray,&vCount);
D3DVECTOR tempOne,tempTwo;
int newVertexCount = m_abVertexArray.m_vertexCount;
  for(int mainLoop=0;mainLoop<m_abVertexArray.m_indecieCount;mainLoop++)
  {//main loop
	 
     int currentValue = m_abVertexArray.m_indecieArray[mainLoop];
     for(int secondaryLoop=0;secondaryLoop<m_abVertexArray.m_indecieCount;secondaryLoop++)
	 {//secondary loop
		 
		 int inVal = m_abVertexArray.m_indecieArray[secondaryLoop];
		 if(secondaryLoop != mainLoop && inVal != currentValue){//not the same
               
			 
			 
			if(tempArray[currentValue].x      == tempArray[inVal].x &&
			   tempArray[currentValue].y      == tempArray[inVal].y &&
			   tempArray[currentValue].z      == tempArray[inVal].z &&
			   tempArray[currentValue].tx0.tu == tempArray[inVal].tx0.tu &&
			   tempArray[currentValue].tx0.tv == tempArray[inVal].tx0.tv &&
			   tempArray[currentValue].tx1.tu == tempArray[inVal].tx1.tu &&
			   tempArray[currentValue].tx1.tv == tempArray[inVal].tx1.tv &&
			   tempArray[currentValue].tx2.tu == tempArray[inVal].tx2.tu &&
			   tempArray[currentValue].tx2.tv == tempArray[inVal].tx2.tv &&
			   tempArray[currentValue].tx3.tu == tempArray[inVal].tx3.tu &&
			   tempArray[currentValue].tx3.tv == tempArray[inVal].tx3.tv &&
			   tempArray[currentValue].tx4.tu == tempArray[inVal].tx4.tu &&
			   tempArray[currentValue].tx4.tv == tempArray[inVal].tx4.tv &&
			   tempArray[currentValue].tx5.tu == tempArray[inVal].tx5.tu &&
			   tempArray[currentValue].tx5.tv == tempArray[inVal].tx5.tv &&
			   tempArray[currentValue].tx6.tu == tempArray[inVal].tx6.tu &&
			   tempArray[currentValue].tx6.tv == tempArray[inVal].tx6.tv &&
			   tempArray[currentValue].tx7.tu == tempArray[inVal].tx7.tu &&
			   tempArray[currentValue].tx7.tv == tempArray[inVal].tx7.tv){//match
            
			m_abVertexArray.m_indecieArray[secondaryLoop] = currentValue;
			newVertexCount--;
			}//end match
		 }//end not the same
	 }//end secondary loop
	 barProgress++;
  }//end main loop
  
  //allocate new array
  
  ABVERTEX * newVertexArray = new ABVERTEX[newVertexCount];
  //build non duplicate indecie array//
  //---------------------------------//
  int  nonDupIndecieCount = 0;
  WORD * newIndecieArray = new WORD[m_abVertexArray.m_indecieCount];
  int indecieLoop=0;
  for(indecieLoop=0;indecieLoop<m_abVertexArray.m_indecieCount;indecieLoop++)
      newIndecieArray[indecieLoop] = m_abVertexArray.m_indecieArray[indecieLoop];
  //---------------------------------//
  //-Rebuild vertexArray and indecie-//
  //---------------------------------//
  int dupTrace = 0;
  for(indecieLoop=0;indecieLoop<m_abVertexArray.m_indecieCount;indecieLoop++)
  {//1
      int currentValue = m_abVertexArray.m_indecieArray[indecieLoop];
      BOOL logIt = TRUE;
	  for(int count=0;count<indecieLoop;count++)
	  {//check to see if this has been added
          if(m_abVertexArray.m_indecieArray[count] == currentValue){//if already has been used dont log
             logIt = FALSE;
			 break;
		  }//end if already has been used dont log
	  }//end check to see if this has been added
	  if(logIt == TRUE){
		 //set vertex data
         newVertexArray[dupTrace].x      = tempArray[currentValue].x;
		 newVertexArray[dupTrace].y      = tempArray[currentValue].y;
		 newVertexArray[dupTrace].z      = tempArray[currentValue].z;
		 newVertexArray[dupTrace].nx     = tempArray[currentValue].nx;
		 newVertexArray[dupTrace].ny     = tempArray[currentValue].ny;
		 newVertexArray[dupTrace].nz     = tempArray[currentValue].nz;
		 newVertexArray[dupTrace].tx0.tu = tempArray[currentValue].tx0.tu;
		 newVertexArray[dupTrace].tx0.tv = tempArray[currentValue].tx0.tv;
		 newVertexArray[dupTrace].tx1.tu = tempArray[currentValue].tx1.tu;
		 newVertexArray[dupTrace].tx1.tv = tempArray[currentValue].tx1.tv;
		 newVertexArray[dupTrace].tx2.tu = tempArray[currentValue].tx2.tu;
		 newVertexArray[dupTrace].tx2.tv = tempArray[currentValue].tx2.tv;
		 newVertexArray[dupTrace].tx3.tu = tempArray[currentValue].tx3.tu;
		 newVertexArray[dupTrace].tx3.tv = tempArray[currentValue].tx3.tv;
		 newVertexArray[dupTrace].tx4.tu = tempArray[currentValue].tx4.tu;
		 newVertexArray[dupTrace].tx4.tv = tempArray[currentValue].tx4.tv;
		 newVertexArray[dupTrace].tx5.tu = tempArray[currentValue].tx5.tu;
		 newVertexArray[dupTrace].tx5.tv = tempArray[currentValue].tx5.tv;
		 newVertexArray[dupTrace].tx6.tu = tempArray[currentValue].tx6.tu;
		 newVertexArray[dupTrace].tx6.tv = tempArray[currentValue].tx6.tv;
		 newVertexArray[dupTrace].tx7.tu = tempArray[currentValue].tx7.tu;
		 newVertexArray[dupTrace].tx7.tv = tempArray[currentValue].tx7.tv;
         
         //adjust indecies
         for(int indLoop=0;indLoop<m_abVertexArray.m_indecieCount;indLoop++)
		 {//check to see if this has been added
            if(m_abVertexArray.m_indecieArray[indLoop] == currentValue){//if already has been used dont log
               newIndecieArray[indLoop] = dupTrace; 
			}//end if already has been used dont log
		 }//end check to see if this has been added

         dupTrace++;
	  }
	 barProgress++;
  }//end 1
   delete [] m_abVertexArray.m_indecieArray;
   if(tempArray)
   delete [] tempArray;
   tempArray = 0;

   m_abVertexArray.m_indecieArray=0;

   m_abVertexArray.m_indecieArray = newIndecieArray;
   //m_abVertexArray.m_origVertexArray   = newVertexArray;
   m_abVertexArray.m_vertexCount  = newVertexCount;
   
   m_abVertexArray.SetABVERTEXStyle(NULL,newVertexArray,newVertexCount);


}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CEXMeshObj::OptimizeMesh(CDialogBar *wndUpdate, float smoothShadeNormalReqAngle)
{
int barProgress = 0;

ABVERTEX * tempArray = NULL;
int vCount = 0;

m_abVertexArray.GetABVERTEXStyle(&tempArray,&vCount);
D3DVECTOR tempOne,tempTwo;
int newVertexCount = m_abVertexArray.m_vertexCount;
  for(int mainLoop=0;mainLoop<m_abVertexArray.m_indecieCount;mainLoop++)
  {//main loop
	 
     int currentValue = m_abVertexArray.m_indecieArray[mainLoop];
     for(int secondaryLoop=0;secondaryLoop<m_abVertexArray.m_indecieCount;secondaryLoop++)
	 {//secondary loop
		 
		 int inVal = m_abVertexArray.m_indecieArray[secondaryLoop];
		 if(secondaryLoop != mainLoop && inVal != currentValue){//not the same

		    tempOne.x = tempArray[inVal].nx;
		    tempOne.y = tempArray[inVal].ny;
		    tempOne.z = tempArray[inVal].nz;
		    tempTwo.x = tempArray[currentValue].nx;
		    tempTwo.y = tempArray[currentValue].ny;
		    tempTwo.z = tempArray[currentValue].nz;
		    float angleDiff = List_matrixFunctions.GetAngleBetweenTwoVectors(tempOne,tempTwo);


            if(tempArray[currentValue].x      == tempArray[inVal].x &&
			   tempArray[currentValue].y      == tempArray[inVal].y &&
			   tempArray[currentValue].z      == tempArray[inVal].z &&
			   tempArray[currentValue].tx0.tu == tempArray[inVal].tx0.tu &&
			   tempArray[currentValue].tx0.tv == tempArray[inVal].tx0.tv &&
			   tempArray[currentValue].tx1.tu == tempArray[inVal].tx1.tu &&
			   tempArray[currentValue].tx1.tv == tempArray[inVal].tx1.tv &&
			   tempArray[currentValue].tx2.tu == tempArray[inVal].tx2.tu &&
			   tempArray[currentValue].tx2.tv == tempArray[inVal].tx2.tv &&
			   tempArray[currentValue].tx3.tu == tempArray[inVal].tx3.tu &&
			   tempArray[currentValue].tx3.tv == tempArray[inVal].tx3.tv &&
			   tempArray[currentValue].tx4.tu == tempArray[inVal].tx4.tu &&
			   tempArray[currentValue].tx4.tv == tempArray[inVal].tx4.tv &&
			   tempArray[currentValue].tx5.tu == tempArray[inVal].tx5.tu &&
			   tempArray[currentValue].tx5.tv == tempArray[inVal].tx5.tv &&
			   tempArray[currentValue].tx6.tu == tempArray[inVal].tx6.tu &&
			   tempArray[currentValue].tx6.tv == tempArray[inVal].tx6.tv &&
			   tempArray[currentValue].tx7.tu == tempArray[inVal].tx7.tu &&
			   tempArray[currentValue].tx7.tv == tempArray[inVal].tx7.tv &&
			   angleDiff <= smoothShadeNormalReqAngle){//match
            
			m_abVertexArray.m_indecieArray[secondaryLoop] = currentValue;
			newVertexCount--;
			}//end match
		 }//end not the same
	 }//end secondary loop
	 barProgress++;
  }//end main loop
  
  //allocate new array
  
  ABVERTEX * newVertexArray = new ABVERTEX[newVertexCount];
  //build non duplicate indecie array//
  //---------------------------------//
  int  nonDupIndecieCount = 0;
  WORD * newIndecieArray = new WORD[m_abVertexArray.m_indecieCount];
  int indecieLoop=0;
  for(indecieLoop=0;indecieLoop<m_abVertexArray.m_indecieCount;indecieLoop++)
      newIndecieArray[indecieLoop] = m_abVertexArray.m_indecieArray[indecieLoop];
  //---------------------------------//
  //-Rebuild vertexArray and indecie-//
  //---------------------------------//
  int dupTrace = 0;
  for(indecieLoop=0;indecieLoop<m_abVertexArray.m_indecieCount;indecieLoop++)
  {//1
      int currentValue = m_abVertexArray.m_indecieArray[indecieLoop];
      BOOL logIt = TRUE;
	  for(int count=0;count<indecieLoop;count++)
	  {//check to see if this has been added
          if(m_abVertexArray.m_indecieArray[count] == currentValue){//if already has been used dont log
             logIt = FALSE;
			 break;
		  }//end if already has been used dont log
	  }//end check to see if this has been added
	  if(logIt == TRUE){
		 //set vertex data
         newVertexArray[dupTrace].x      = tempArray[currentValue].x;
		 newVertexArray[dupTrace].y      = tempArray[currentValue].y;
		 newVertexArray[dupTrace].z      = tempArray[currentValue].z;
		 newVertexArray[dupTrace].nx     = tempArray[currentValue].nx;
		 newVertexArray[dupTrace].ny     = tempArray[currentValue].ny;
		 newVertexArray[dupTrace].nz     = tempArray[currentValue].nz;
		 newVertexArray[dupTrace].tx0.tu = tempArray[currentValue].tx0.tu;
		 newVertexArray[dupTrace].tx0.tv = tempArray[currentValue].tx0.tv;
		 newVertexArray[dupTrace].tx1.tu = tempArray[currentValue].tx1.tu;
		 newVertexArray[dupTrace].tx1.tv = tempArray[currentValue].tx1.tv;
		 newVertexArray[dupTrace].tx2.tu = tempArray[currentValue].tx2.tu;
		 newVertexArray[dupTrace].tx2.tv = tempArray[currentValue].tx2.tv;
		 newVertexArray[dupTrace].tx3.tu = tempArray[currentValue].tx3.tu;
		 newVertexArray[dupTrace].tx3.tv = tempArray[currentValue].tx3.tv;
		 newVertexArray[dupTrace].tx4.tu = tempArray[currentValue].tx4.tu;
		 newVertexArray[dupTrace].tx4.tv = tempArray[currentValue].tx4.tv;
		 newVertexArray[dupTrace].tx5.tu = tempArray[currentValue].tx5.tu;
		 newVertexArray[dupTrace].tx5.tv = tempArray[currentValue].tx5.tv;
		 newVertexArray[dupTrace].tx6.tu = tempArray[currentValue].tx6.tu;
		 newVertexArray[dupTrace].tx6.tv = tempArray[currentValue].tx6.tv;
		 newVertexArray[dupTrace].tx7.tu = tempArray[currentValue].tx7.tu;
		 newVertexArray[dupTrace].tx7.tv = tempArray[currentValue].tx7.tv;
         
         //adjust indecies
         for(int indLoop=0;indLoop<m_abVertexArray.m_indecieCount;indLoop++)
		 {//check to see if this has been added
            if(m_abVertexArray.m_indecieArray[indLoop] == currentValue){//if already has been used dont log
               newIndecieArray[indLoop] = dupTrace; 
			}//end if already has been used dont log
		 }//end check to see if this has been added

         dupTrace++;
	  }
	 barProgress++;
  }//end 1
   delete [] m_abVertexArray.m_indecieArray;
   if(tempArray)
   delete [] tempArray;
   tempArray = 0;

   m_abVertexArray.m_indecieArray=0;

   m_abVertexArray.m_indecieArray = newIndecieArray;
   //m_abVertexArray.m_origVertexArray   = newVertexArray;
   m_abVertexArray.m_vertexCount  = newVertexCount;
   
   m_abVertexArray.SetABVERTEXStyle(NULL,newVertexArray,newVertexCount);

}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
BOOL CEXMeshObj::UpdateShadowMesh(D3DMATRIX & objectMatrix,
								  D3DMATRIX & shadowDifference,
								  LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	/*if(m_shadowTemp){
       m_shadowTemp->SafeDelete();
	   delete m_shadowTemp;
	   m_shadowTemp=0;
	}

	this->Clone(&m_shadowTemp,g_pd3dDevice);

    m_shadowTemp->List_storedTextureFromFile[0] = -1;
    m_shadowTemp->List_storedTextureFromFile[1] = -1;
	m_shadowTemp->List_storedTextureFromFile[2] = -1;
	m_shadowTemp->List_storedTextureFromFile[3] = -1;
	m_shadowTemp->List_storedTextureFromFile[4] = -1;
	m_shadowTemp->List_storedTextureFromFile[5] = -1;
	m_shadowTemp->List_storedTextureFromFile[6] = -1;
	m_shadowTemp->List_storedTextureFromFile[7] = -1;
	m_shadowTemp->List_BlendMethod[0] = -1;
	m_shadowTemp->List_BlendMethod[1] = -1;
	m_shadowTemp->List_BlendMethod[2] = -1;
	m_shadowTemp->List_BlendMethod[3] = -1;
	m_shadowTemp->List_BlendMethod[4] = -1;
	m_shadowTemp->List_BlendMethod[5] = -1;
	m_shadowTemp->List_BlendMethod[6] = -1;
	m_shadowTemp->List_BlendMethod[7] = -1;

	if(m_shadowTemp->List_materialObject)
	   delete m_shadowTemp->List_materialObject;
	   m_shadowTemp->List_materialObject = 0;
	m_shadowTemp->List_materialObject = new CMaterialObject();

    m_shadowTemp->List_materialObject->m_baseMaterial.Diffuse.r  = 0.0f;
    m_shadowTemp->List_materialObject->m_baseMaterial.Diffuse.g  = 0.0f;
    m_shadowTemp->List_materialObject->m_baseMaterial.Diffuse.b  = 0.0f;
    m_shadowTemp->List_materialObject->m_baseMaterial.Diffuse.a  = 0.0f;
    m_shadowTemp->List_materialObject->m_baseMaterial.Ambient.r  = 0.0f;
    m_shadowTemp->List_materialObject->m_baseMaterial.Ambient.g  = 0.0f;
    m_shadowTemp->List_materialObject->m_baseMaterial.Ambient.b  = 0.0f;
    m_shadowTemp->List_materialObject->m_baseMaterial.Ambient.a  = 0.0f;
    m_shadowTemp->List_materialObject->m_baseMaterial.Specular.r = 0.0f;
    m_shadowTemp->List_materialObject->m_baseMaterial.Specular.g = 0.0f;
    m_shadowTemp->List_materialObject->m_baseMaterial.Specular.b = 0.0f;
    m_shadowTemp->List_materialObject->m_baseMaterial.Specular.a = 0.0f;
    m_shadowTemp->List_materialObject->m_baseMaterial.Emissive.r = 0.0f;
    m_shadowTemp->List_materialObject->m_baseMaterial.Emissive.g = 0.0f;
    m_shadowTemp->List_materialObject->m_baseMaterial.Emissive.b = 0.0f;
    m_shadowTemp->List_materialObject->m_baseMaterial.Emissive.a = 0.0f;
    
	m_shadowTemp->List_materialObject->Reset();*/
	/*m_shadowTemp->m_currentMatrix = List_matrixFunctions.GetShadowMeshTransformSpcified(objectMatrix,
													                                    shadowDifference);	*/
	return TRUE;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 void CEXMeshObj::InitBuffer(LPDIRECT3DDEVICE9 g_pd3dDevice,CGlobalMaterialShader * optionalShader)
{
   if(!g_pd3dDevice)
	  return;
   //m_abVertexArray.m_uvCount = GetUVlayerCount();
   int uvTarget = m_abVertexArray.m_uvCount;
   if(optionalShader)
   {
      uvTarget = optionalShader->m_requiredUvCount;
      if(optionalShader->m_enableChroming)
	  {
         this->m_chromeUvSet = optionalShader->m_chromeUvSet;
         this->m_chromeWrap  = optionalShader->m_enableChroming;
	  }
	  if(optionalShader->m_useOverrideMaterial)
	  {
	     CopyMemory(&this->List_materialObject->m_useMaterial,&optionalShader->m_overrideMaterial,sizeof(D3DMATERIAL9));
         CopyMemory(&this->List_materialObject->m_baseMaterial,&optionalShader->m_overrideMaterial,sizeof(D3DMATERIAL9));
	  }

      for(int i=0;i<7;i++)
	  {
          if(optionalShader->m_textureIndexArray[i]!=-1)
		  {
		     if(i==0)
			 {
			    this->List_BlendMethod[9] = optionalShader->m_blendArray[i];
			 }
			 else
			    this->List_BlendMethod[i-1] = optionalShader->m_blendArray[i];

             this->List_textureSelect[i] = optionalShader->m_textureIndexArray[i];
			 
		  }
	  }
   }
   
   
   m_abVertexArray.InitBuffer(g_pd3dDevice,List_materialObject->m_diffuseEffect,uvTarget);
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CEXMeshObj::ScaleIt(float factor)
{
ABVERTEX * tempArray = NULL;
int vCount = 0;

m_abVertexArray.GetABVERTEXStyle(&tempArray,&vCount);


if(!tempArray)
return;
if(vCount==0)
return;
float MatrixLocal[4][4];
scaleMat(factor,factor,factor,MatrixLocal);
  for(int loop=0;loop<m_abVertexArray.m_vertexCount;loop++){
      //
      D3DVECTOR tempConv;
	  tempConv.x = tempArray[loop].x;
	  tempConv.y = tempArray[loop].y;
	  tempConv.z = tempArray[loop].z;
	  tempConv = UpdatePolygonVertex(MatrixLocal,tempConv);
      tempArray[loop].x = tempConv.x;
	  tempArray[loop].y = tempConv.y;
	  tempArray[loop].z = tempConv.z;
  }

  m_abVertexArray.SetABVERTEXStyle(NULL,tempArray,vCount);
  delete [] tempArray;
  tempArray = 0;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CEXMeshObj::CenteredScaleIt(float factor)
{

ABVERTEX * tempArray = NULL;
int vCount = 0;
m_abVertexArray.GetABVERTEXStyle(&tempArray,&vCount);


if(!tempArray)
return;
if(m_abVertexArray.m_vertexCount==0)
return;
float MatrixLocal[4][4];
scaleMat(factor,factor,factor,MatrixLocal);
  for(int loop=0;loop<m_abVertexArray.m_vertexCount;loop++){
      //
      D3DVECTOR tempConv;
	  tempConv.x = tempArray[loop].x - m_meshCenter.x;
	  tempConv.y = tempArray[loop].y - m_meshCenter.y;
	  tempConv.z = tempArray[loop].z - m_meshCenter.z;
	  tempConv = UpdatePolygonVertex(MatrixLocal,tempConv);
      tempArray[loop].x = tempConv.x + m_meshCenter.x;
	  tempArray[loop].y = tempConv.y + m_meshCenter.y;
	  tempArray[loop].z = tempConv.z + m_meshCenter.z;
  }

  m_abVertexArray.SetABVERTEXStyle(NULL,tempArray,vCount);
  delete [] tempArray;
  tempArray = 0;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CEXMeshObj::ExchangeTextureSets(int level,int level2)
{
/*ABVERTEX * vertexArrayCopy = new ABVERTEX[m_abVertexArray.m_vertexCount];
CopyMemory(vertexArrayCopy,m_abVertexArray.m_origVertexArray,(sizeof(ABVERTEX)*m_abVertexArray.m_vertexCount));

//switch texture setting
int texturesetstore1 = List_textureSelect[level];
CString textureStoredString = List_storedTextureFromFile[level];
List_textureSelect[level] = List_textureSelect[level2];
List_storedTextureFromFile[level] = List_storedTextureFromFile[level2];
List_textureSelect[level2] = texturesetstore1;
List_storedTextureFromFile[level2] = textureStoredString;
//switch coordinates
for(int loop=0;loop<m_abVertexArray.m_vertexCount;loop++)
{
    if(level == 0 || level == 1)
		if(level2 == 0 || level2 == 1){
	       m_abVertexArray.m_origVertexArray[loop].tx1.tu = vertexArrayCopy[loop].tx0.tu;
	       m_abVertexArray.m_origVertexArray[loop].tx1.tv = vertexArrayCopy[loop].tx0.tv;
	       m_abVertexArray.m_origVertexArray[loop].tx0.tu = vertexArrayCopy[loop].tx1.tu;
	       m_abVertexArray.m_origVertexArray[loop].tx0.tv = vertexArrayCopy[loop].tx1.tv;
		}
    if(level == 0 || level == 2)
		if(level2 == 0 || level2 == 2){
	       m_abVertexArray.m_origVertexArray[loop].tx2.tu = vertexArrayCopy[loop].tx0.tu;
	       m_abVertexArray.m_origVertexArray[loop].tx2.tv = vertexArrayCopy[loop].tx0.tv;
	       m_abVertexArray.m_origVertexArray[loop].tx0.tu = vertexArrayCopy[loop].tx2.tu;
	       m_abVertexArray.m_origVertexArray[loop].tx0.tv = vertexArrayCopy[loop].tx2.tv;
		}

}
//free memory
if(vertexArrayCopy)
   delete [] vertexArrayCopy;
   vertexArrayCopy=0;


*/
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CEXMeshObj::SafeDelete()
{
    

    if(m_shadowTemp){
       m_shadowTemp->SafeDelete();
	   delete m_shadowTemp;
	   m_shadowTemp = 0;
	}

	if(m_charona){
       m_charona->SafeDelete();
	   delete m_charona;
	   m_charona=0;
	}

    if(List_textureSetList){//valid
       DumpTextureCoordSets();
    if(List_textureSetList)
       delete List_textureSetList;
       List_textureSetList=0;
	}//end valid

	if(List_materialObject)
       delete List_materialObject;
       List_materialObject=0;


       m_abVertexArray.SafeDelete();
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//update texture coords for sphere map realtime
BOOL CEXMeshObj::ApplySphereMappingToObject(LPDIRECT3DDEVICE9 g_pd3dDevice, int & vertexType, int uvSet)
{
	// Get the current world-view matrix
    D3DMATRIX matWorld, matView, matWV;
    g_pd3dDevice->GetTransform( D3DTS_VIEW,  &matView );
    g_pd3dDevice->GetTransform( D3DTS_WORLD, &matWorld );
    List_matrixFunctions.fastMatrixMultiply( matWV, matWorld, matView );
	m_abVertexArray.ApplySphereAlgorithm(matWV,vertexType,uvSet);
    
    return TRUE;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
BOOL CEXMeshObj::Clone(CEXMeshObj **copy,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
CEXMeshObj * copyLocal = new CEXMeshObj();

m_abVertexArray.CloneMinimum2(&copyLocal->m_abVertexArray,NULL);

copyLocal->m_chromeWrap  = m_chromeWrap;
copyLocal->m_chromeUvSet = m_chromeUvSet;

if(m_charona)
   m_charona->Clone(&copyLocal->m_charona);

if(List_materialObject)
List_materialObject->Clone(&copyLocal->List_materialObject);

copyLocal->List_blendType = List_blendType;//-1 = none 1 = even opacity 2 = opacity map darker is more transparent
copyLocal->m_currentMatrix = m_currentMatrix;

copyLocal->m_meshCenter.x  = m_meshCenter.x;
copyLocal->m_meshCenter.y  = m_meshCenter.y;
copyLocal->m_meshCenter.z  = m_meshCenter.z;
copyLocal->m_groupReference = -1;

copyLocal->List_boundingBox.minX = List_boundingBox.minX;
copyLocal->List_boundingBox.maxX = List_boundingBox.maxX;
copyLocal->List_boundingBox.minY = List_boundingBox.minY;
copyLocal->List_boundingBox.maxY = List_boundingBox.maxY;
copyLocal->List_boundingBox.minZ = List_boundingBox.minZ;
copyLocal->List_boundingBox.maxZ = List_boundingBox.maxZ;

copyLocal->m_meshBoundingSphereRadius = m_meshBoundingSphereRadius;
copyLocal->m_visibilityTrack          = 1;
copyLocal->List_storedTextureFromFile[0] = List_storedTextureFromFile[0];
copyLocal->List_storedTextureFromFile[1] = List_storedTextureFromFile[1];
copyLocal->List_storedTextureFromFile[2] = List_storedTextureFromFile[2];
copyLocal->List_storedTextureFromFile[3] = List_storedTextureFromFile[3];
copyLocal->List_storedTextureFromFile[4] = List_storedTextureFromFile[4];
copyLocal->List_storedTextureFromFile[5] = List_storedTextureFromFile[5];
copyLocal->List_storedTextureFromFile[6] = List_storedTextureFromFile[6];
copyLocal->List_storedTextureFromFile[7] = List_storedTextureFromFile[7];

CopyMemory(copyLocal->List_BlendMethod,List_BlendMethod,(sizeof(int)*10));
CopyMemory(copyLocal->List_textureSelect,List_textureSelect,(sizeof(int)*10));

*copy = copyLocal;

if(g_pd3dDevice)
copyLocal->InitBuffer(g_pd3dDevice,NULL);

return TRUE;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-------------------render with vertex buffer---------------------------------//
 int CEXMeshObj::Render(LPDIRECT3DDEVICE9 g_pd3dDevice,int & vertexType,int uvSet)
{

 if(m_chromeWrap)
    ApplySphereMappingToObject(g_pd3dDevice,vertexType,uvSet);

    m_abVertexArray.Render(g_pd3dDevice,vertexType);

return m_abVertexArray.m_indecieCount;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-------------------render with vertex buffer---------------------------------//
 int CEXMeshObj::RenderFast(LPDIRECT3DDEVICE9 g_pd3dDevice,int & vertexType,int & uvSet)
{

 if(m_chromeWrap)
    ApplySphereMappingToObject(g_pd3dDevice,vertexType,uvSet);

	g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                       0,
                                       0,//UINT MinIndex,
                                       m_abVertexArray.m_vertexCount,//UINT NumVertices,
                                       0,//UINT StartIndex,
                                       (m_abVertexArray.m_indecieCount/3)//UINT PrimitiveCount
                                       );

return m_abVertexArray.m_indecieCount;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-------------------render with vertex buffer---------------------------------//
void CEXMeshObj::DumpTextureCoordSets()
{
 if(List_textureSetList){//valid
  for(POSITION posLoc = List_textureSetList->GetHeadPosition();posLoc != NULL;){
      CTextureSetObj*delPtr = (CTextureSetObj*)List_textureSetList->GetNext(posLoc);
      if(delPtr->List_textureSet)
		 delete [] delPtr->List_textureSet;
	     delPtr->List_textureSet = 0;
	  delete delPtr;
	  delPtr=0;
  }
  List_textureSetList->RemoveAll();
 }//end valid
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 int CEXMeshObj::GetUVlayerCount()
{
  int returnVal = 0;
  for(int loop=0;loop<8;loop++)
  {
      if(List_textureSelect[loop] != -1)
		 returnVal++;
  }
  m_validUvCount = returnVal;
  return returnVal;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
int CEXMeshObj::GetUVlayerCountString()
{
  int returnVal = 0;
  for(int loop=0;loop<8;loop++)
  {
      if(List_storedTextureFromFile[loop] != "NONE"){
	      List_textureSelect[loop] = 0;//set non kill
		  returnVal++;

	  }
  }
  m_validUvCount = returnVal;
  m_abVertexArray.m_uvCount = m_validUvCount;
  return returnVal;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CEXMeshObj::ConvertToX(CString * fileString)
{
       CString ext = ABEXT.GetFileExt(*fileString);
       ext.MakeLower();
       if(ext == "3ds"){//begin extension conversion
       CString final = operator+("conv3ds -x -m ",*fileString);
       system( final );
       ABEXT.RemoveFileExt(fileString);
       ABEXT.AddNewFileExt(fileString,".x");
	   }
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
BOOL CEXMeshObj::SetGeometry(CString textureFilename,
							 ABVERTEX  * vertexArray,
							 WORD        vertexCount,
							 WORD *      indecieArray,
							 WORD        indecieCount)
{

m_abVertexArray.m_indecieArray = indecieArray;
m_abVertexArray.m_indecieCount = indecieCount;
m_abVertexArray.SetABVERTEXStyle(NULL,vertexArray,vertexCount);
List_storedTextureFromFile[0] = textureFilename;
List_textureSelect[0]         = 0;
GetBox();
GetCenterRadius();
return TRUE;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
BOOL CEXMeshObj::SetGeometry2(CString textureFilename,
							  ABVERTEX1L  * vertexArray,
							  WORD        vertexCount,
							  WORD *      indecieArray,
							  WORD        indecieCount)
{
m_abVertexArray.m_uvCount = 1;
m_abVertexArray.m_vertexCount = vertexCount;
m_abVertexArray.m_indecieArray = indecieArray;
m_abVertexArray.m_indecieCount = indecieCount;

m_abVertexArray.m_vertexArray1 = vertexArray;


List_storedTextureFromFile[0] = textureFilename;
List_textureSelect[0]         = 0;
GetBox();
GetCenterRadius();
return TRUE;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
BOOL CEXMeshObj::LoadGeometryFromPlugin(CString fileName,
										float * sightRange,
										LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	 float sightBasis = 0.0f;
	 BOOL  smoothShading = FALSE;
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		return FALSE;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);
	 
	 the_in_Archive >> m_abVertexArray.m_vertexCount 
		            >> m_abVertexArray.m_indecieCount 
					>> sightBasis 
					>> smoothShading;
	 //Vertexes
     ABVERTEX * abVertex = NULL;

	 


	 if(m_abVertexArray.m_vertexCount>0)//allocate
        abVertex = new ABVERTEX[m_abVertexArray.m_vertexCount];
	 int loop=0;
	 for(loop=0;loop<m_abVertexArray.m_vertexCount;loop++){//loop
          the_in_Archive >> abVertex[loop].x
			             >> abVertex[loop].y
						 >> abVertex[loop].z
						 >> abVertex[loop].nx
						 >> abVertex[loop].ny
						 >> abVertex[loop].nz
						 >> abVertex[loop].tx0.tu
						 >> abVertex[loop].tx0.tv
						 >> abVertex[loop].tx1.tu
						 >> abVertex[loop].tx1.tv
						 >> abVertex[loop].tx2.tu
						 >> abVertex[loop].tx2.tv
						 >> abVertex[loop].tx3.tu
						 >> abVertex[loop].tx3.tv
						 >> abVertex[loop].tx4.tu
						 >> abVertex[loop].tx4.tv
						 >> abVertex[loop].tx5.tu
						 >> abVertex[loop].tx5.tv
						 >> abVertex[loop].tx6.tu
						 >> abVertex[loop].tx6.tv
						 >> abVertex[loop].tx7.tu
						 >> abVertex[loop].tx7.tv;
	 }//end loop



     //indecies
     if(m_abVertexArray.m_indecieCount > 0)
		m_abVertexArray.m_indecieArray = new WORD[m_abVertexArray.m_indecieCount];

	 for(loop=0;loop<m_abVertexArray.m_indecieCount;loop++){//loop
         the_in_Archive >> m_abVertexArray.m_indecieArray[loop];
	 }//end loop
     //textures
	 for(loop=0;loop<9;loop++){
         the_in_Archive >> List_storedTextureFromFile[loop];
	 }
	 
	 the_in_Archive.Close();
     fl.Close();
     //boundingBox Generation
	 sightBasis = fabs(sightBasis);

     *sightRange = sightBasis;
     //beta
	 GetUVlayerCountString();
   
     //set abmesh values
     m_abVertexArray.SetABVERTEXStyle(g_pd3dDevice,abVertex,m_abVertexArray.m_vertexCount);
	 if(abVertex)
		delete [] abVertex;
	    abVertex = 0;

     //OptimizeMesh(NULL);
	 //GenerateSmoothNormals();
	 GetBox();
     GetCenterRadius();
	 if(m_abVertexArray.m_indicieBuffer)
	 {
        m_abVertexArray.m_indicieBuffer->Release();
		m_abVertexArray.m_indicieBuffer = 0;
	 }
	 //UnifyNormals();
	 InitBuffer(g_pd3dDevice,NULL);
	 
	 return TRUE;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CEXMeshObj::UnifyNormals()
{
                GenerateNormals(0);
	            float tolerance = .002f;
                ABVERTEX * vArray = NULL;
                int vCount = 0;
		        m_abVertexArray.GetABVERTEXStyle(&vArray,&vCount);

                ABVERTEX * chkvArray = NULL;
                int chkvCount = 0;
				m_abVertexArray.GetABVERTEXStyle(&chkvArray,&chkvCount);
				int pvLoop=0;
				for(pvLoop=0;pvLoop<m_abVertexArray.m_vertexCount;pvLoop++)
				{//primary mesh loop
                   for(int scndLoop=0;scndLoop<m_abVertexArray.m_vertexCount;scndLoop++)
				   {//secondary mesh loop
				      if(pvLoop == scndLoop)
					     continue;
					  
                      float dist = List_matrixFunctions.MagnitudeSquared(vArray[pvLoop].x,
						                                               vArray[pvLoop].y,
																	   vArray[pvLoop].z,
																	   chkvArray[scndLoop].x,
																	   chkvArray[scndLoop].y,
																	   chkvArray[scndLoop].z);
                     if(dist < tolerance)
					 {//colapse	
						chkvArray[scndLoop].nx += vArray[pvLoop].nx;
					    chkvArray[scndLoop].ny += vArray[pvLoop].ny;
					    chkvArray[scndLoop].nz += vArray[pvLoop].nz;
					 }//end colapse
				   }//end secondary mesh loop
				}//end primary mesh loop

				for(pvLoop=0;pvLoop<m_abVertexArray.m_vertexCount;pvLoop++)
				{//primary mesh loop
				   D3DVECTOR nTemp;
						nTemp.x = vArray[pvLoop].nx;
                        nTemp.y = vArray[pvLoop].ny;
						nTemp.z = vArray[pvLoop].nz;
						nTemp = List_matrixFunctions.Normalize(nTemp);
                   for(int scndLoop=0;scndLoop<m_abVertexArray.m_vertexCount;scndLoop++)
				   {//secondary mesh loop
				      //if(pvLoop == scndLoop)
					     //continue;
					  
                      float dist = List_matrixFunctions.MagnitudeSquared(vArray[pvLoop].x,
						                                               vArray[pvLoop].y,
																	   vArray[pvLoop].z,
																	   chkvArray[scndLoop].x,
																	   chkvArray[scndLoop].y,
																	   chkvArray[scndLoop].z);
                     if(dist < tolerance)
					 {//colapse
					    
						
						
						chkvArray[scndLoop].nx = nTemp.x;
					    chkvArray[scndLoop].ny = nTemp.y;
					    chkvArray[scndLoop].nz = nTemp.z;
					 }//end colapse
				   }//end secondary mesh loop
				}//end primary mesh loop

				if(chkvArray)
				{
                   m_abVertexArray.SetABVERTEXStyle(NULL,chkvArray,chkvCount);
                   delete [] chkvArray;
				   chkvArray = 0;
				}
				if(vArray)
				{
                   delete [] vArray;
				   vArray = 0;
				}
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CEXMeshObj::UnifyNormals(float angleTolerance)
{
	            D3DVECTOR tempOne,tempTwo;
	            float tolerance = .002f;
                ABVERTEX * vArray = NULL;
                int vCount = 0;
		        m_abVertexArray.GetABVERTEXStyle(&vArray,&vCount);

                ABVERTEX * chkvArray = NULL;
                int chkvCount = 0;
				m_abVertexArray.GetABVERTEXStyle(&chkvArray,&chkvCount);
				int pvLoop=0;
				for(pvLoop=0;pvLoop<m_abVertexArray.m_vertexCount;pvLoop++)
				{//primary mesh loop
                   for(int scndLoop=0;scndLoop<m_abVertexArray.m_vertexCount;scndLoop++)
				   {//secondary mesh loop
				      if(pvLoop == scndLoop)
					     continue;

					  if(fabs(vArray[pvLoop].x - chkvArray[scndLoop].x) > tolerance)
					     continue;

					  if(fabs(vArray[pvLoop].y - chkvArray[scndLoop].y) > tolerance)
					     continue;

					  if(fabs(vArray[pvLoop].z - chkvArray[scndLoop].z) > tolerance)
					     continue;
					  
					  
                      /*float dist = List_matrixFunctions.MagnitudeSquared(vArray[pvLoop].x,
						                                               vArray[pvLoop].y,
																	   vArray[pvLoop].z,
																	   chkvArray[scndLoop].x,
																	   chkvArray[scndLoop].y,
																	   chkvArray[scndLoop].z);*/
                     //if(dist < tolerance)
					 //{
                       tempOne.x = chkvArray[scndLoop].nx;
		              tempOne.y = chkvArray[scndLoop].ny;
		              tempOne.z = chkvArray[scndLoop].nz;
		              tempTwo.x = vArray[pvLoop].nx;
		              tempTwo.y = vArray[pvLoop].ny;
		              tempTwo.z = vArray[pvLoop].nz;
		              float angleDiff = List_matrixFunctions.GetAngleBetweenTwoVectors(tempOne,tempTwo);

					   if(angleDiff <= angleTolerance)
					   {//colapse	
						chkvArray[scndLoop].nx += vArray[pvLoop].nx;
					    chkvArray[scndLoop].ny += vArray[pvLoop].ny;
					    chkvArray[scndLoop].nz += vArray[pvLoop].nz;
					   }//end colapse
					 //}
				   }//end secondary mesh loop
				}//end primary mesh loop

				for(pvLoop=0;pvLoop<m_abVertexArray.m_vertexCount;pvLoop++)
				{//primary mesh loop
				   D3DVECTOR nTemp;
						nTemp.x = vArray[pvLoop].nx;
                        nTemp.y = vArray[pvLoop].ny;
						nTemp.z = vArray[pvLoop].nz;
						nTemp = List_matrixFunctions.Normalize(nTemp);
                   for(int scndLoop=0;scndLoop<m_abVertexArray.m_vertexCount;scndLoop++)
				   {//secondary mesh loop
				      //if(pvLoop == scndLoop)
					     //continue;
				      
					  
                      float dist = List_matrixFunctions.MagnitudeSquared(vArray[pvLoop].x,
						                                               vArray[pvLoop].y,
																	   vArray[pvLoop].z,
																	   chkvArray[scndLoop].x,
																	   chkvArray[scndLoop].y,
																	   chkvArray[scndLoop].z);
                     if(dist < tolerance)
					 {
                       tempOne.x = chkvArray[scndLoop].nx;
		               tempOne.y = chkvArray[scndLoop].ny;
		               tempOne.z = chkvArray[scndLoop].nz;
		               tempTwo.x = vArray[pvLoop].nx;
		               tempTwo.y = vArray[pvLoop].ny;
		               tempTwo.z = vArray[pvLoop].nz;
		               float angleDiff = List_matrixFunctions.GetAngleBetweenTwoVectors(tempOne,tempTwo);

					   if(angleDiff <= angleTolerance)
					   {//colapse
					    
						
						
						chkvArray[scndLoop].nx = nTemp.x;
					    chkvArray[scndLoop].ny = nTemp.y;
					    chkvArray[scndLoop].nz = nTemp.z;
					   }//end colapse
					 }
				   }//end secondary mesh loop
				}//end primary mesh loop

				if(chkvArray)
				{
                   m_abVertexArray.SetABVERTEXStyle(NULL,chkvArray,chkvCount);
                   delete [] chkvArray;
				   chkvArray = 0;
				}
				if(vArray)
				{
                   delete [] vArray;
				   vArray = 0;
				}
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
BOOL CEXMeshObj::GetGeometryFromPlugin(CString fileName,
									   WORD *vertexCount,
									   ABVERTEX ** vertexArray,
									   WORD *indecieCount,
									   WORD **indecieArray,
									   CString texture[10])
{
    WORD        L_vertexCount=0;
	ABVERTEX  * L_vertexArray=NULL;
	float       L_activeDistance=0;
	WORD *      L_indecieArray=NULL;
    WORD        L_indecieCount=0;

	 float L_sightBasis = 0.0f;
	 BOOL  L_smoothShading = FALSE;
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 CArchive the_in_Archive(&fl,CArchive::load);
	 
	 the_in_Archive >> L_vertexCount >> L_indecieCount >> L_activeDistance >> L_smoothShading;
	 //Vertexes
	 if(L_vertexCount>0)//allocate
        L_vertexArray = new ABVERTEX[L_vertexCount];

	 int loop=0;
	 for(loop=0;loop<L_vertexCount;loop++){//loop
          the_in_Archive >> L_vertexArray[loop].x
			             >> L_vertexArray[loop].y
						 >> L_vertexArray[loop].z
						 >> L_vertexArray[loop].nx
						 >> L_vertexArray[loop].ny
						 >> L_vertexArray[loop].nz
						 >> L_vertexArray[loop].tx0.tu
						 >> L_vertexArray[loop].tx0.tv
						 >> L_vertexArray[loop].tx1.tu
						 >> L_vertexArray[loop].tx1.tv
						 >> L_vertexArray[loop].tx2.tu
						 >> L_vertexArray[loop].tx2.tv
						 >> L_vertexArray[loop].tx3.tu
						 >> L_vertexArray[loop].tx3.tv
						 >> L_vertexArray[loop].tx4.tu
						 >> L_vertexArray[loop].tx4.tv
						 >> L_vertexArray[loop].tx5.tu
						 >> L_vertexArray[loop].tx5.tv
						 >> L_vertexArray[loop].tx6.tu
						 >> L_vertexArray[loop].tx6.tv
						 >> L_vertexArray[loop].tx7.tu
						 >> L_vertexArray[loop].tx7.tv;
	 }//end loop
     //indecies
     if(L_indecieCount > 0)
		L_indecieArray = new WORD[L_indecieCount];

	 for(loop=0;loop<L_indecieCount;loop++){//loop
         the_in_Archive >> L_indecieArray[loop];
	 }//end loop
     //textures
	 for(loop=0;loop<10;loop++){
         the_in_Archive >> texture[loop];
	 }
	 
	 the_in_Archive.Close();
     fl.Close();

     /*m_abVertexArray.InitFromData(L_vertexCount,
								  L_indecieCount,
								  L_vertexArray,
								  L_indecieArray,
									 m_validUvCount*/
     
	 return TRUE;
}


//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//move texture coords
/* void CEXMeshObj::MoveTexture(float &u,float &v,int textureSet,float &adjustfactor)
{
if(u == 0.0f &&
   v == 0.0f)
   return;
  
   m_abVertexArray.ApplyMoveTexture(u,v,textureSet,adjustfactor);
}*/
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CEXMeshObj::Serialize( CArchive& ar )
{
int dump = 0;
int   reservedInt = 0;
float reservedFloat = 0.0f;



CObject::Serialize( ar );
	if(ar.IsStoring() ){

	   ar  << List_blendType
		   << List_materialObject
		   << m_meshCenter.x
		   << m_meshCenter.y
		   << m_meshCenter.z
		   << m_meshBoundingSphereRadius
		   << m_chromeWrap
		   << m_chromeUvSet
		   << reservedInt
		   << reservedInt
		   << reservedFloat
	       << reservedFloat
		   << reservedFloat
		   << m_charona
		   << &m_abVertexArray
		   << List_boundingBox.maxX
		   << List_boundingBox.maxY
		   << List_boundingBox.maxZ
		   << List_boundingBox.minX
		   << List_boundingBox.minY
		   << List_boundingBox.minZ;

	   for(int loop=0;loop<10;loop++)
       ar  << List_textureSelect[loop] 
	       << List_storedTextureFromFile[loop]
		   << List_BlendMethod[loop];
	}
	else
	{
   
	   CAdvancedVertexObj * vertexArray = NULL;
       //test implemnt
       ar  >> List_blendType
		   >> List_materialObject
		   >> m_meshCenter.x
		   >> m_meshCenter.y
		   >> m_meshCenter.z
		   >> m_meshBoundingSphereRadius
		   >> m_chromeWrap
		   >> m_chromeUvSet
		   >> reservedInt
		   >> reservedInt
		   >> reservedFloat
	       >> reservedFloat
		   >> reservedFloat
		   >> m_charona
		   >> vertexArray
		   >> List_boundingBox.maxX
		   >> List_boundingBox.maxY
		   >> List_boundingBox.maxZ
		   >> List_boundingBox.minX
		   >> List_boundingBox.minY
		   >> List_boundingBox.minZ;

	
       vertexArray->CloneDataOnly(&m_abVertexArray);
	   vertexArray->SafeDelete();
	   delete vertexArray;
	   vertexArray=0;

	   for(int loop=0;loop<10;loop++)
       ar  >> List_textureSelect[loop] 
	       >> List_storedTextureFromFile[loop]
		   >> List_BlendMethod[loop];



	   //end test implement
       m_groupReference = -1;
       m_tempDistance = 0;
	   m_lastTimeStamp = 0;
	   m_visibilityTrack          = 1;
	   List_matrixFunctions.InitMat(m_currentMatrix.m);
	   m_shadowTemp = NULL;


	   //GetBox();
	   //GetCenterRadius();
	}
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//--Get Bounding Box--//
BOOL CEXMeshObj::GetBox()
{

ABVERTEX * abVertex = NULL;
int vCount = 0;

m_abVertexArray.GetABVERTEXStyle(&abVertex,&vCount);

if(!abVertex)
return FALSE;
if(m_abVertexArray.m_vertexCount == 0)
return FALSE;

List_boundingBox.maxX = List_boundingBox.minX = abVertex[0].x;
List_boundingBox.maxY = List_boundingBox.minY = abVertex[0].y;
List_boundingBox.maxZ = List_boundingBox.minZ = abVertex[0].z;


for(int loop=0;loop<m_abVertexArray.m_vertexCount;loop++){
	if(abVertex[loop].x < List_boundingBox.minX)
       List_boundingBox.minX = abVertex[loop].x;
	if(abVertex[loop].x > List_boundingBox.maxX)
       List_boundingBox.maxX = abVertex[loop].x;

	if(abVertex[loop].y < List_boundingBox.minY)
       List_boundingBox.minY = abVertex[loop].y;
	if(abVertex[loop].y > List_boundingBox.maxY)
       List_boundingBox.maxY = abVertex[loop].y;

	if(abVertex[loop].z < List_boundingBox.minZ)
       List_boundingBox.minZ = abVertex[loop].z;
	if(abVertex[loop].z > List_boundingBox.maxZ)
       List_boundingBox.maxZ = abVertex[loop].z;

if(List_boundingBox.minX == List_boundingBox.maxX)
   List_boundingBox.maxX = List_boundingBox.maxX + .01f;
if(List_boundingBox.minY == List_boundingBox.maxY)
   List_boundingBox.maxY = List_boundingBox.maxY + .01f;
if(List_boundingBox.minZ == List_boundingBox.maxZ)
   List_boundingBox.maxZ = List_boundingBox.maxZ + .01f;

}

 if(abVertex)
	delete [] abVertex;
    abVertex = 0;

 
 return TRUE;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//--Get Bounding Box--//
 BOOL CEXMeshObj::GetBoxWithMarix(D3DMATRIX & positionWld,ABBOX * returnBox)
{

ABVERTEX * abVertex = NULL;
int vCount = 0;

m_abVertexArray.GetABVERTEXStyle(&abVertex,&vCount);

if(!abVertex)
return FALSE;
if(m_abVertexArray.m_vertexCount == 0)
return FALSE;
D3DVECTOR posTemp;
posTemp.x = abVertex[0].x;
posTemp.y = abVertex[0].y;
posTemp.z = abVertex[0].z;
D3DVECTOR translatedPos = UpdatePolygonVertex(positionWld.m,posTemp);

returnBox->maxX = returnBox->minX = translatedPos.x;
returnBox->maxY = returnBox->minY = translatedPos.y;
returnBox->maxZ = returnBox->minZ = translatedPos.z;


for(int loop=0;loop<m_abVertexArray.m_vertexCount;loop++){
	//update by matrix
	posTemp.x = abVertex[loop].x;
    posTemp.y = abVertex[loop].y;
    posTemp.z = abVertex[loop].z;
    D3DVECTOR translatedPos = UpdatePolygonVertex(positionWld.m,posTemp);
	
	
	//end update by marix
	
	if(translatedPos.x < returnBox->minX)
       returnBox->minX = translatedPos.x;
	if(translatedPos.x > returnBox->maxX)
       returnBox->maxX = translatedPos.x;

	if(translatedPos.y < returnBox->minY)
       returnBox->minY = translatedPos.y;
	if(translatedPos.y > returnBox->maxY)
       returnBox->maxY = translatedPos.y;

	if(translatedPos.z < returnBox->minZ)
       returnBox->minZ = translatedPos.z;
	if(translatedPos.z > returnBox->maxZ)
       returnBox->maxZ = translatedPos.z;

if(returnBox->minX == returnBox->maxX)
   returnBox->maxX =  returnBox->maxX + .01f;
if(returnBox->minY == returnBox->maxY)
   returnBox->maxY =  returnBox->maxY + .01f;
if(returnBox->minZ == returnBox->maxZ)
   returnBox->maxZ =  returnBox->maxZ + .01f;

}
 if(abVertex)
 delete [] abVertex;
 abVertex = 0;


 return TRUE;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//--bounding
BOOL CEXMeshObj::CalculateBoundingRadius()
{
if(List_boundingBox.maxZ >= List_boundingBox.maxX && List_boundingBox.maxZ >= List_boundingBox.maxY)
 List_boundingRadius = List_boundingBox.maxZ;
if(List_boundingBox.maxY >= List_boundingBox.maxZ && List_boundingBox.maxY >= List_boundingBox.maxX)
 List_boundingRadius = List_boundingBox.maxY;
if(List_boundingBox.maxX >= List_boundingBox.maxZ && List_boundingBox.maxX >= List_boundingBox.maxY)
 List_boundingRadius = List_boundingBox.maxX;
return TRUE;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
D3DVECTOR CEXMeshObj::UpdatePolygonVertex(float matrixLoc[4][4],D3DVECTOR polygonLoc)
{
D3DVECTOR tempP;
tempP.x = (matrixLoc[0][0]*polygonLoc.x)+(matrixLoc[1][0]*polygonLoc.y)+(matrixLoc[2][0]*polygonLoc.z)+(matrixLoc[3][0] * 1);
tempP.y = (matrixLoc[0][1]*polygonLoc.x)+(matrixLoc[1][1]*polygonLoc.y)+(matrixLoc[2][1]*polygonLoc.z)+(matrixLoc[3][1] * 1);
tempP.z = (matrixLoc[0][2]*polygonLoc.x)+(matrixLoc[1][2]*polygonLoc.y)+(matrixLoc[2][2]*polygonLoc.z)+(matrixLoc[3][2] * 1);

polygonLoc.x = tempP.x;
polygonLoc.y = tempP.y;
polygonLoc.z = tempP.z;
return polygonLoc;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//round texture coords
//--This Function is a component of ExactCollision()-----------//
int CEXMeshObj::Round(float x)
{
 float testVal;
 testVal = fmod(x,1.0f);
 if((x >= 0) && (testVal >= 0.5))
  x = x + 1.0f;
 else if((x<0)&&(testVal<=-0.5f))
  x=x-1.0f;
 return (int)x;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CEXMeshObj::AVGNormals(float smoothBasisAngle)
{

ABVERTEX * abVertex = NULL;
int vCount = 0;

m_abVertexArray.GetABVERTEXStyle(&abVertex,&vCount);
    
	for(int x=0; x<vCount; x++ )
	  for(int i=0; i<vCount; i++ )
      {
	    if(i == x)
		   continue;
        //WORD vI1 = m_abVertexArray.m_indecieArray[i];
		//WORD vI2 = m_abVertexArray.m_indecieArray[x];
       

        if(abVertex[i].x == abVertex[x].x &&
           abVertex[i].y == abVertex[x].y &&
		   abVertex[i].z == abVertex[x].z)
		{//same pos

           D3DVECTOR tempOne,tempTwo;
           tempOne.x = abVertex[i].nx;
		   tempOne.y = abVertex[i].ny;
		   tempOne.z = abVertex[i].nz;
		   tempTwo.x = abVertex[x].nx;
		   tempTwo.y = abVertex[x].ny;
		   tempTwo.z = abVertex[x].nz;
	       float angleDiff = this->List_matrixFunctions.GetAngleBetweenTwoVectors(tempOne,tempTwo);
           if(angleDiff <= smoothBasisAngle)
		   {//combine
		    float oldIx = abVertex[i].nx;
		    abVertex[i].nx = abVertex[i].nx + abVertex[x].nx;
			abVertex[x].nx = oldIx + abVertex[x].nx;

			float oldIy = abVertex[i].ny;
			abVertex[i].ny = abVertex[i].ny + abVertex[x].ny;
			abVertex[x].ny = oldIy + abVertex[x].ny;

			float oldIz = abVertex[i].nz;
			abVertex[i].nz = abVertex[i].nz + abVertex[x].nz;
			abVertex[x].nz = oldIz + abVertex[x].nz;
		   }//end combine
		}//end same pos
      }

    // Assign the newly computed normals back to the vertices
    D3DVECTOR normV;
    for(int i=0; i<m_abVertexArray.m_vertexCount; i++ )
    {
        // Provide some relief to bogus normals
        if( abVertex[i].nx == 0.0f && abVertex[i].ny == 0.0f && abVertex[i].nz == 0.0f) 
		{
            abVertex[i].nx = 0.0f;// = D3DVECTOR( 0.0f, 0.0f, 1.0f );
            abVertex[i].ny = 0.0f;
			abVertex[i].nz = 1.0f;
		}

		normV.x = abVertex[i].nx;
        normV.y = abVertex[i].ny;
		normV.z = abVertex[i].nz;

        normV = List_matrixFunctions.Normalize(normV);

        abVertex[i].nx = normV.x;
        abVertex[i].ny = normV.y;
        abVertex[i].nz = normV.z;
    }
    m_abVertexArray.SetABVERTEXStyle(NULL,
									 abVertex,
									 vCount);
    if(abVertex)
	   delete [] abVertex;
 	   abVertex = 0;

}
//-------------//
//rebuild normals
//-------------//
//calculate normals
void CEXMeshObj::GenerateNormals(float smoothBasisAngle)
{

ABVERTEX * abVertex = NULL;
int vCount = 0;

m_abVertexArray.GetABVERTEXStyle(&abVertex,&vCount);


D3DVECTOR* pNormals = new D3DVECTOR[m_abVertexArray.m_vertexCount];
    ZeroMemory( pNormals, sizeof(D3DVECTOR)*m_abVertexArray.m_vertexCount );
	int i=0;
    for(i=0; i<m_abVertexArray.m_indecieCount; i+=3 )
    {
        WORD a = m_abVertexArray.m_indecieArray[i+0];
        WORD b = m_abVertexArray.m_indecieArray[i+1];
        WORD c = m_abVertexArray.m_indecieArray[i+2];

        D3DVECTOR* v1 = (D3DVECTOR*)&abVertex[a];
        D3DVECTOR* v2 = (D3DVECTOR*)&abVertex[b];
        D3DVECTOR* v3 = (D3DVECTOR*)&abVertex[c];


		D3DVECTOR arg1,arg2;
        arg1.x = v2->x - v1->x;
		arg1.y = v2->y - v1->y;
		arg1.z = v2->z - v1->z;

		arg2.x = v3->x - v2->x;
		arg2.y = v3->y - v2->y;
		arg2.z = v3->z - v2->z;


        D3DVECTOR n = List_matrixFunctions.Normalize( List_matrixFunctions.CrossProduct( arg1, arg2 ) );
        //D3DVECTOR n = List_matrixFunctions.Normalize( List_matrixFunctions.CrossProduct( *v2-*v1, *v3-*v2 ) );

        pNormals[a].x += n.x;
        pNormals[a].y += n.y;
		pNormals[a].z += n.z;
		
		pNormals[b].x += n.x;
		pNormals[b].y += n.y;
		pNormals[b].z += n.z;

        pNormals[c].x += n.x;
		pNormals[c].y += n.y;
		pNormals[c].z += n.z;
    }

    // Assign the newly computed normals back to the vertices
    for( i=0; i<m_abVertexArray.m_vertexCount; i++ )
    {
        // Provide some relief to bogus normals
        if( List_matrixFunctions.Magnitude( pNormals[i] ) < 0.1f ) {
            pNormals[i].x = 0.0f;// = D3DVECTOR( 0.0f, 0.0f, 1.0f );
            pNormals[i].y = 0.0f;
			pNormals[i].z = 1.0f;
		}

        pNormals[i] = List_matrixFunctions.Normalize( pNormals[i] );
        abVertex[i].nx = pNormals[i].x;
        abVertex[i].ny = pNormals[i].y;
        abVertex[i].nz = pNormals[i].z;
    }
    delete [] pNormals;
	pNormals=0;

    m_abVertexArray.SetABVERTEXStyle(NULL,
									 abVertex,
									 vCount);
    if(abVertex)
	   delete [] abVertex;
 	   abVertex = 0;

}
//calculate normals
void CEXMeshObj::NormalizeOnly()
{

ABVERTEX * abVertex = NULL;
int vCount = 0;

m_abVertexArray.GetABVERTEXStyle(&abVertex,&vCount);

    D3DVECTOR tempNormal;
    // Assign the newly computed normals back to the vertices
    for(int i=0; i<m_abVertexArray.m_vertexCount; i++ )
    {
        tempNormal.x = abVertex[i].nx;
		tempNormal.y = abVertex[i].ny;
		tempNormal.z = abVertex[i].nz;

        tempNormal = List_matrixFunctions.Normalize( tempNormal );
        abVertex[i].nx = tempNormal.x;
        abVertex[i].ny = tempNormal.y;
        abVertex[i].nz = tempNormal.z;
    }

    m_abVertexArray.SetABVERTEXStyle(NULL,
									 abVertex,
									 vCount);
    if(abVertex)
	   delete [] abVertex;
 	   abVertex = 0;

}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CEXMeshObj::GenerateSmoothNormals()
{
ABVERTEX * abVertex = NULL;
int vCount = 0;

m_abVertexArray.GetABVERTEXStyle(&abVertex,&vCount);
	
	
GenerateNormals(0);

ABVERTEX *originalVertexArray = new ABVERTEX[m_abVertexArray.m_vertexCount];
CopyMemory(originalVertexArray,
		   abVertex,
		  (m_abVertexArray.m_vertexCount*sizeof(ABVERTEX)));

    SmoothNormalsBasedOnArray(originalVertexArray);
	delete [] originalVertexArray;
	originalVertexArray=0;

	if(abVertex)
	   delete [] abVertex;
	   abVertex = 0;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CEXMeshObj::SmoothNormalsBasedOnArray(ABVERTEX *originalVertexArray)
{
GenerateNormals(0);

ABVERTEX * abVertex = NULL;
int vCount = 0;

m_abVertexArray.GetABVERTEXStyle(&abVertex,&vCount);

    for(int i=0; i<m_abVertexArray.m_vertexCount; i++ )
    {
       for(int c=0; c<m_abVertexArray.m_vertexCount; c++ )
	   {//loop secondary
		//if(c != i)
        if(abVertex[i].x == originalVertexArray[c].x &&
		   abVertex[i].y == originalVertexArray[c].y &&
		   abVertex[i].z == originalVertexArray[c].z){//match?
          
			abVertex[i].nx += originalVertexArray[c].nx;
			abVertex[i].ny += originalVertexArray[c].ny;
			abVertex[i].nz += originalVertexArray[c].nz;
            D3DVECTOR conversionVect;
			conversionVect.x = abVertex[i].nx;
			conversionVect.y = abVertex[i].ny;
			conversionVect.z = abVertex[i].nz;
			conversionVect = List_matrixFunctions.Normalize( conversionVect );
			abVertex[i].nx = conversionVect.x;
			abVertex[i].ny = conversionVect.y;
			abVertex[i].nz = conversionVect.z;
		}//end match?
	   }//end loop secondary
    }

	m_abVertexArray.SetABVERTEXStyle(NULL,
									 abVertex,
									 vCount);
    if(abVertex)
	   delete [] abVertex;
 	   abVertex = 0;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CEXMeshObj::GetCenterRadius()
{

ABVERTEX * abVertex = NULL;
int vCount = 0;

m_abVertexArray.GetABVERTEXStyle(&abVertex,&vCount);

 if(m_abVertexArray.m_vertexCount == 0)
	return;

 if(!abVertex)
	return;

      float largestX,smallestX,largestY,smallestY,largestZ,smallestZ;
      int quantity = m_abVertexArray.m_vertexCount;
      largestX=abVertex[0].x;
	  smallestX=abVertex[0].x;
	  largestY=abVertex[0].y;
	  smallestY=abVertex[0].y;
	  largestZ=abVertex[0].z;
	  smallestZ=abVertex[0].z;

	  D3DVECTOR frameOrigin;
      frameOrigin.x = 0.0f;
	  frameOrigin.y = 0.0f;
	  frameOrigin.z = 0.0f;

    for(int loop=0;loop<quantity;loop++)
	{//loop through polygons
       if(largestX < abVertex[loop].x)
          largestX = abVertex[loop].x;
       
	   if(smallestX > abVertex[loop].x)
		  smallestX = abVertex[loop].x;
	   
	   if(largestY < abVertex[loop].y)
          largestY = abVertex[loop].y;
       
	   if(smallestY > abVertex[loop].y)
		  smallestY = abVertex[loop].y;

	   if(largestZ < abVertex[loop].z)
          largestZ = abVertex[loop].z;
       
	   if(smallestZ > abVertex[loop].z)
		  smallestZ = abVertex[loop].z;
	}//end loop through polygons
	m_meshCenter.x = (largestX + smallestX)*.5f;
    m_meshCenter.y = (largestY + smallestY)*.5f;
	m_meshCenter.z = (largestZ + smallestZ)*.5f;

	m_meshBoundingSphereRadius = List_matrixFunctions.MagnitudeSquared(m_meshCenter.x,
		                                                          m_meshCenter.y,
									                              m_meshCenter.z,
																  largestX,
									                              largestY,
																  largestZ);

	if(abVertex)
	   delete [] abVertex;
	   abVertex = 0;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-------------------textuture set class---------------------//
CTextureSetObj::CTextureSetObj()
{
List_textureSet=NULL;
List_textureCoordCount=0;
}

void CTextureSetObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << List_textureCoordCount;

       for(int loop=0;loop<List_textureCoordCount;loop++){
       ar << List_textureSet[loop].tu
	      << List_textureSet[loop].tv;
	   }
	}
	else
	{
       ar >> List_textureCoordCount;
       if(List_textureCoordCount > 0)
          List_textureSet = new TXTUV[List_textureCoordCount];
	   for(int loop=0;loop<List_textureCoordCount;loop++){
       ar >> List_textureSet[loop].tu
	      >> List_textureSet[loop].tv;
	   }
	}
}     

void CEXMeshObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CEXMeshObj * meshObj = (CEXMeshObj *)GetNext(posLoc);
        meshObj->SafeDelete();
		delete meshObj;
		meshObj=0;
	}
	RemoveAll();
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CEXMeshObj::Rotate(D3DVECTOR rotation,D3DVECTOR meshCenter)
{
ABVERTEX * abVertex = NULL;
int vCount = 0;

m_abVertexArray.GetABVERTEXStyle(&abVertex,&vCount);

//reposition   List_matrixFunctions
D3DMATRIX matrixApply;
List_matrixFunctions.Rotate(rotation.x,rotation.y,rotation.z,matrixApply.m);
        //zeroMesh
        int loop=0;
        for(loop=0;loop<m_abVertexArray.m_vertexCount;loop++)
		{
		    abVertex[loop].x -= meshCenter.x;
			abVertex[loop].y -= meshCenter.y;
			abVertex[loop].z -= meshCenter.z;
		}
        //rotate it
        for(loop=0;loop<m_abVertexArray.m_vertexCount;loop++)
		{
			D3DVECTOR tempVect;
			tempVect.x = abVertex[loop].x;
			tempVect.y = abVertex[loop].y;
			tempVect.z = abVertex[loop].z;
			tempVect = List_matrixFunctions.UpdatePolygonVertexRotationOnly(matrixApply.m,tempVect);
			abVertex[loop].x = tempVect.x;
			abVertex[loop].y = tempVect.y;
			abVertex[loop].z = tempVect.z;
		}
		//offset it
		for(loop=0;loop<m_abVertexArray.m_vertexCount;loop++)
		{
		    abVertex[loop].x += meshCenter.x;
			abVertex[loop].y += meshCenter.y;
			abVertex[loop].z += meshCenter.z;
		}

        m_abVertexArray.SetABVERTEXStyle(NULL,
									     abVertex,
									     vCount);
        if(abVertex)
	       delete [] abVertex;
 	       abVertex = 0;
        
		GenerateSmoothNormals();
		GetBox();
        GetCenterRadius();
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CEXMeshObj::Move(D3DVECTOR translation,D3DVECTOR meshCenter)
{
ABVERTEX * abVertex = NULL;
int vCount = 0;

m_abVertexArray.GetABVERTEXStyle(&abVertex,&vCount);
//reposition
        for(int loop=0;loop<m_abVertexArray.m_vertexCount;loop++)
		{
			abVertex[loop].x += translation.x;
		    abVertex[loop].y += translation.y;
		    abVertex[loop].z += translation.z;
		}
		GenerateSmoothNormals();
		meshCenter.x += translation.x;
		meshCenter.y += translation.y;
		meshCenter.z += translation.z;
		
		m_abVertexArray.SetABVERTEXStyle(NULL,
									     abVertex,
									     vCount);
        if(abVertex)
	       delete [] abVertex;
 	       abVertex = 0;
		
		GetBox();
        GetCenterRadius();
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CEXMeshObj::Scale(D3DVECTOR scale,D3DVECTOR meshCenter)
{
ABVERTEX * abVertex = NULL;
int vCount = 0;

m_abVertexArray.GetABVERTEXStyle(&abVertex,&vCount);
//reposition   List_matrixFunctions
D3DMATRIX matrixApply;
List_matrixFunctions.scale(scale.x,scale.y,scale.z,matrixApply.m);
        //zeroMesh
        int loop=0;
        for(loop=0;loop<m_abVertexArray.m_vertexCount;loop++)
		{
		    abVertex[loop].x -= meshCenter.x;
			abVertex[loop].y -= meshCenter.y;
			abVertex[loop].z -= meshCenter.z;
		}
        //rotate it
        for(loop=0;loop<m_abVertexArray.m_vertexCount;loop++)
		{
			D3DVECTOR tempVect;
			tempVect.x = abVertex[loop].x;
			tempVect.y = abVertex[loop].y;
			tempVect.z = abVertex[loop].z;
			tempVect = List_matrixFunctions.UpdatePolygonVertexRotationOnly(matrixApply.m,tempVect);
			abVertex[loop].x = tempVect.x;
			abVertex[loop].y = tempVect.y;
			abVertex[loop].z = tempVect.z;
		}
		//offset it
		for(loop=0;loop<m_abVertexArray.m_vertexCount;loop++)
		{
		    abVertex[loop].x += meshCenter.x;
			abVertex[loop].y += meshCenter.y;
			abVertex[loop].z += meshCenter.z;
		}

		m_abVertexArray.SetABVERTEXStyle(NULL,
									     abVertex,
									     vCount);
        if(abVertex)
	       delete [] abVertex;
 	       abVertex = 0;
        
		GenerateSmoothNormals();
		GetBox();
        GetCenterRadius();
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
 void CEXMeshObj::TextureMovementCallback(float & gl_adjust,int & curVertexType)
{
     if(List_materialObject->m_enableTextureMovement == TRUE){//texture Movement
        if(List_materialObject->m_enableSet0 == TRUE){//set 0
           /*MoveTexture(List_materialObject->m_textureMovement[0].tu,
					   List_materialObject->m_textureMovement[0].tv,
					   0,gl_adjust);*/
		   m_abVertexArray.ApplyMoveTexture(List_materialObject->m_textureMovement[0].tu,List_materialObject->m_textureMovement[0].tv,0,gl_adjust,curVertexType);

		}//end set 0
	    if(List_materialObject->m_enableSet1 == TRUE){//set 1
           /*MoveTexture(List_materialObject->m_textureMovement[1].tu,
					   List_materialObject->m_textureMovement[1].tv,
					   1,gl_adjust);*/
		   m_abVertexArray.ApplyMoveTexture(List_materialObject->m_textureMovement[1].tu,List_materialObject->m_textureMovement[1].tv,1,gl_adjust,curVertexType);
		}//end set 1
	 }//end texture Movement
}


IMPLEMENT_SERIAL(CTextureSetObj, CObject, 0)
IMPLEMENT_SERIAL(CTextureSetObjList, CObList, 0)
IMPLEMENT_SERIAL(CEXMeshObj, CObject, 0)
IMPLEMENT_SERIAL(CEXMeshObjList, CObList, 0)

