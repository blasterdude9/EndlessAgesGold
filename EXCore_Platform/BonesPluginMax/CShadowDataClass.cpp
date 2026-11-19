//#include "stdafx.h"
#include "afxwin.h"
#include "afxext.h"
#define D3D_OVERLOADS
#include "d3d9.h"
#include "CTextureDBClass.h"
#include "CShadowDataClass.h"
#include "math.h"

//D3DVERTEX ToVertex(D3DXVECTOR3 pt);
//BOOL DuplicateTri(CPolyRetStruct *Polys, int cPolys, CPolyRetStruct TestPoly);

CShadowClass::CShadowClass(float radius,
						   float collRadius,
						   float extentUp,
						   float extentDown,
						   BOOL scale,
						   int textureIndex,
						   CString textureSearchName)
{
    m_type          = 0;
	m_lastNormal.x  = 0.0f;
	m_lastNormal.y  = 1.0f;
	m_lastNormal.z  = 0.0f;
	ZeroMemory(&m_wldPosition,sizeof(D3DXVECTOR3));
    m_valid         = FALSE;

	m_color.r = 1.0f;
	m_color.g = 1.0f;
	m_color.b = 1.0f;

	//m_pd3dDevice8 = NULL;	

	m_normals = NULL;
	
	m_vertices = NULL;
	m_numvertices = 0;
				
	m_collisionVects = NULL;
	m_numcollisionVects = 0;

	m_collisionScaleVects = NULL;
	m_numcollisionScaleVects = 0;

	m_firstTime = TRUE;

	m_scaleTexture = scale;
	m_beginScaling = FALSE;

	m_originalRadius       = radius;
	m_collisionRadius      = collRadius;
	m_extentUp             = (float)fabs(extentUp);   //just to be safe...
	m_extentDown           = (float)fabs(extentDown);
	m_textureSearchName    = textureSearchName;
    m_textureIndex         = textureIndex;

	//initialize shadow mesh object
	m_shadowMesh = NULL;
	m_shadowMesh = new CEXMeshObj;
	m_shadowMesh->List_blendType = 5;
	m_shadowMesh->List_textureSelect[0] = textureIndex;
	/*m_shadowMesh->List_materialObject = new CMaterialObject();
	m_shadowMesh->List_materialObject->m_textureTileType = 2;
	m_shadowMesh->List_materialObject->m_baseMaterial.Emissive.r = 1.0f;
    m_shadowMesh->List_materialObject->m_baseMaterial.Emissive.g = 1.0f;
    m_shadowMesh->List_materialObject->m_baseMaterial.Emissive.b = 1.0f;
    m_shadowMesh->List_materialObject->m_baseMaterial.Emissive.a = 1.0f;*/
}

void CShadowClass::SafeDelete()
{
	if(m_normals) 
		{ 
		delete [] m_normals;
		m_normals = NULL;
		}

	if(m_vertices) 
		{
		delete [] m_vertices;
		m_vertices = NULL;
		}

	if(m_collisionVects) 
		{
		delete [] m_collisionVects;
		m_collisionVects = NULL;
		}

	if(m_collisionScaleVects) 
		{
		delete [] m_collisionScaleVects;
		m_collisionScaleVects = NULL;
		}

	if(m_shadowMesh)
		{
		m_shadowMesh->SafeDelete();
		delete m_shadowMesh;
		m_shadowMesh = NULL;
		}
}


CShadowClass::~CShadowClass()
{

}

//FUNCTIONS

void CShadowClass::EditParameters(float radius,
								  float collRadius,
						          float extentUp,
						          float extentDown,
								  BOOL scale)
{
	m_originalRadius       = radius;
	m_collisionRadius      = collRadius;
	m_extentUp             = (float)fabs(extentUp);   //just to be safe...
	m_extentDown           = (float)fabs(extentDown);
	m_scaleTexture         = scale;
}

void CShadowClass::Clone(CShadowClass ** clone)
{
	CShadowClass * returnClone = new CShadowClass(m_originalRadius,
		                                          m_collisionRadius,
												  m_extentUp,
                                                  m_extentDown,
												  m_scaleTexture,
												  m_textureIndex,
												  m_textureSearchName);
	
	//returnClone->
	*clone = returnClone;
}






void CShadowClass::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_originalRadius
	      << m_collisionRadius
	      << m_extentUp
	      << m_extentDown
		  << m_color.r
		  << m_color.g
		  << m_color.b
		  << m_scaleTexture
		  << m_textureSearchName
          << m_textureIndex;
	}
	else
	{
       ar >> m_originalRadius
	      >> m_collisionRadius
	      >> m_extentUp
	      >> m_extentDown
		  >> m_color.r
		  >> m_color.g
		  >> m_color.b
		  >> m_scaleTexture
		  >> m_textureSearchName
          >> m_textureIndex;


	   m_type          = 0;
	   m_lastNormal.x  = 0.0f;
	   m_lastNormal.y  = 1.0f;
	   m_lastNormal.z  = 0.0f;
       m_valid         = FALSE;
     
	   //m_pd3dDevice8 = NULL;	
       m_normals = NULL;

	   m_vertices = NULL;
	   m_numvertices = 0;

	   m_collisionVects = NULL;
	   m_numcollisionVects = 0;

   	   m_firstTime = TRUE;

	   m_beginScaling = FALSE;
	   m_collisionScaleVects = NULL;

	   //m_shadowMesh = NULL;
	   //m_shadowMesh = new CEXMeshObj;

	   /*m_shadowMesh->List_blendType = 5;
	   m_shadowMesh->List_materialObject = new CMaterialObject();
 	   m_shadowMesh->List_materialObject->m_textureTileType = 2;
	   m_shadowMesh->List_materialObject->m_baseMaterial.Emissive.r = 1.0f;
       m_shadowMesh->List_materialObject->m_baseMaterial.Emissive.g = 1.0f;
       m_shadowMesh->List_materialObject->m_baseMaterial.Emissive.b = 1.0f;
       m_shadowMesh->List_materialObject->m_baseMaterial.Emissive.a = 1.0f;*/
	
	}
}





IMPLEMENT_SERIAL(CShadowClass, CObject, 0)