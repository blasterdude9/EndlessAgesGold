#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "math.h"

#include "CLaserClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
CLaserObj::CLaserObj()
{
    m_speed                = 50.0f;
    m_currentLength        = -500.0f;
    m_currentWidth         = 10.0f;
    m_meshObject           = NULL;
	m_lockDown             = TRUE;
	m_millisecondsOfFade   = 200;
	m_fadeOut              = FALSE;
	m_terminated           = FALSE;

    GL_matrixFunctions.InitMat(m_orientationAndOrigin.m);
	m_fireFromLocation = m_orientationAndOrigin;
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CLaserObj::SafeDelete()
{
	if(m_meshObject){
	  m_meshObject->SafeDelete();
      delete m_meshObject;
	  m_meshObject=0;
	}
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CLaserObj::Clone(CLaserObj ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
    CLaserObj * copyLocal = new CLaserObj();
    
	copyLocal->m_currentLength      = m_currentLength;
    copyLocal->m_currentWidth       = m_currentWidth;
    copyLocal->m_speed              = m_speed;
	copyLocal->m_lockDown           = m_lockDown;
	copyLocal->m_millisecondsOfFade = m_millisecondsOfFade;
	copyLocal->m_fadeOut            = m_fadeOut;
	copyLocal->m_terminated         = FALSE;

    if(m_meshObject)
	   m_meshObject->Clone(&copyLocal->m_meshObject,g_pd3dDevice);

	*clone = copyLocal;
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CLaserObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CLaserObj * spnPtr = (CLaserObj *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CLaserObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	
	   ar << m_speed
		  << m_currentLength
		  << m_currentWidth
		  << m_meshObject
		  << m_lockDown
		  << m_millisecondsOfFade
		  << m_fadeOut;

	}
	else
	{
       ar >> m_speed
		  >> m_currentLength
		  >> m_currentWidth
		  >> m_meshObject
		  >> m_lockDown
		  >> m_millisecondsOfFade
		  >> m_fadeOut;

	   m_terminated = FALSE;

	   GL_matrixFunctions.InitMat(m_orientationAndOrigin.m);
	   m_fireFromLocation = m_orientationAndOrigin;
	}
}     

//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
void CLaserObj::InitializeMesh(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
BOOL previousDataExisted = FALSE;
int texture = 0;
CString textureName = "";
CMaterialObject * materialStored = NULL;

    if(m_meshObject)
	{
	   m_meshObject->List_materialObject->Clone(&materialStored);
       previousDataExisted = TRUE;
       texture     = m_meshObject->List_textureSelect[0];
	   textureName = m_meshObject->List_storedTextureFromFile[0];
       m_meshObject->SafeDelete();
	   delete m_meshObject;
	   m_meshObject = 0;
	}

	
       m_meshObject = new CEXMeshObj();
	   if(!materialStored)
	      m_meshObject->List_materialObject = new CMaterialObject();
	   else
	      materialStored->Clone(&m_meshObject->List_materialObject);
	   
	   m_meshObject->m_abVertexArray.m_vertexCount    = 4;
	   m_meshObject->m_abVertexArray.m_indecieCount   = 6;
	   m_meshObject->m_abVertexArray.m_uvCount = 1;

	   
       ABVERTEX * vArray = new ABVERTEX[4]; 
	   ZeroMemory(vArray,(sizeof(ABVERTEX)*4));
	   m_meshObject->m_abVertexArray.m_indecieArray   = new WORD[6];
	   ZeroMemory(m_meshObject->m_abVertexArray.m_indecieArray,(sizeof(WORD)*6));
	   if(!previousDataExisted)
	   m_meshObject->List_textureSelect[0] = 0;
	   else{
	   m_meshObject->List_textureSelect[0]         = texture;
       m_meshObject->List_storedTextureFromFile[0] = textureName;
	   }

	   //m_meshObject->List_materialObject->m_baseMaterial.Emissive.r = 1.0f;
       //m_meshObject->List_materialObject->m_baseMaterial.Emissive.g = 1.0f;
       //m_meshObject->List_materialObject->m_baseMaterial.Emissive.b = 1.0f;
	   //m_meshObject->List_materialObject->m_baseBlendMode = 2;

	   m_meshObject->m_abVertexArray.SetABVERTEXStyle(NULL,vArray,m_meshObject->m_abVertexArray.m_vertexCount);
	   if(vArray)
		  delete [] vArray;
	      vArray = 0;
	
    
	//m_meshObject->List_materialObject->m_cullOn = FALSE;
	
	
	m_meshObject->m_abVertexArray.m_uvCount = 1;
	m_meshObject->List_materialObject->Reset();

}
//---------------------------------------------------------//
//---------------------------------------------------------//
//---------------------------------------------------------//
BOOL CLaserObj::UpdateLazerMesh(D3DMATRIX cameraMatrix)
{
	D3DVECTOR cameraPositionLocal;
	cameraPositionLocal.x = cameraMatrix._41;
	cameraPositionLocal.y = cameraMatrix._42;
	cameraPositionLocal.z = cameraMatrix._43;
	cameraPositionLocal = GL_matrixFunctions.GetLocalPositionFromMatrixView(m_orientationAndOrigin,cameraPositionLocal);
    cameraPositionLocal.z = 0.0f;

    float rotationOnZ = -(atan(cameraPositionLocal.x/cameraPositionLocal.y)* 57.3f);
	if(cameraPositionLocal.y < 0.0f)
	   rotationOnZ = rotationOnZ+180.0f;

    //Vertex one
	float widthComponent = (m_currentWidth*.5f);
	
	
    m_meshObject->m_abVertexArray.m_vertexArray1[0].x      = -widthComponent;
    m_meshObject->m_abVertexArray.m_vertexArray1[0].y      = 0.0f;
	m_meshObject->m_abVertexArray.m_vertexArray1[0].z      = 0.0f;
	m_meshObject->m_abVertexArray.m_vertexArray1[0].tx0.tu = 0.0f;
	m_meshObject->m_abVertexArray.m_vertexArray1[0].tx0.tv = 0.0f;
    //vertex two
    m_meshObject->m_abVertexArray.m_vertexArray1[1].x      = widthComponent;
    m_meshObject->m_abVertexArray.m_vertexArray1[1].y      = 0.0f;
	m_meshObject->m_abVertexArray.m_vertexArray1[1].z      = 0.0f;
	m_meshObject->m_abVertexArray.m_vertexArray1[1].tx0.tu = 1.0f;
	m_meshObject->m_abVertexArray.m_vertexArray1[1].tx0.tv = 0.0f;
    //vertex three
    m_meshObject->m_abVertexArray.m_vertexArray1[2].x      = widthComponent;
    m_meshObject->m_abVertexArray.m_vertexArray1[2].y      = 0.0f;
	m_meshObject->m_abVertexArray.m_vertexArray1[2].z      = m_currentLength;
	m_meshObject->m_abVertexArray.m_vertexArray1[2].tx0.tu = 1.0f;
	m_meshObject->m_abVertexArray.m_vertexArray1[2].tx0.tv = 1.0f;
    //vertex four
    m_meshObject->m_abVertexArray.m_vertexArray1[3].x      = -widthComponent;
    m_meshObject->m_abVertexArray.m_vertexArray1[3].y      = 0.0f;
	m_meshObject->m_abVertexArray.m_vertexArray1[3].z      = m_currentLength;
	m_meshObject->m_abVertexArray.m_vertexArray1[3].tx0.tu = 0.0f;
	m_meshObject->m_abVertexArray.m_vertexArray1[3].tx0.tv = 1.0f;
    //default normals (doesnt matter cause of emmisive property of a laser)
    m_meshObject->m_abVertexArray.m_vertexArray1[0].nx = 1.0f;
	m_meshObject->m_abVertexArray.m_vertexArray1[1].nx = 1.0f;
	m_meshObject->m_abVertexArray.m_vertexArray1[2].nx = 1.0f;
    m_meshObject->m_abVertexArray.m_vertexArray1[3].nx = 1.0f;
    //set up indecies
    m_meshObject->m_abVertexArray.m_indecieArray[0] = 0;
	m_meshObject->m_abVertexArray.m_indecieArray[1] = 1;
	m_meshObject->m_abVertexArray.m_indecieArray[2] = 2;
	m_meshObject->m_abVertexArray.m_indecieArray[3] = 0;
	m_meshObject->m_abVertexArray.m_indecieArray[4] = 2;
	m_meshObject->m_abVertexArray.m_indecieArray[5] = 3;

   
	D3DMATRIX applied;
	GL_matrixFunctions.Rotate(0.0f,0.0f,rotationOnZ,applied.m);

	m_meshObject->m_currentMatrix = m_orientationAndOrigin;

	GL_matrixFunctions.fastMatrixMultiply(m_meshObject->m_currentMatrix,applied,m_orientationAndOrigin);

return TRUE;
}


IMPLEMENT_SERIAL(CLaserObj, CObject, 0)
IMPLEMENT_SERIAL(CLaserObjList, CObList, 0)

