#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>

#include "CCharonaClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCharonaObj::CCharonaObj()
{
	m_centerPixelSnapShot = 0;
    m_validThisRound      = FALSE;
    m_size                = 200.0f;
	m_scaleFactor         = 1.0f;
	m_fadeTime            = 1000;
    m_material            = NULL;
    vertexCount           = 4;
	indecieCount          = 6;
    m_textureName         = "NONE";
	m_textureIndex        = 0;
	m_currentlyVisible    = FALSE;
	m_changeStamp         = 0;
	m_startDistance       = 80.0f;
    m_endDistance         = 1500.0f;
	m_visibleDistance     = 1500.0f;
	m_currentDistance     = 0.0f;
	m_visual              = NULL;

	ZeroMemory(&m_currentMatrix,sizeof(D3DMATRIX));
	ZeroMemory(&m_center,sizeof(D3DVECTOR));


    ZeroMemory(&vertexArray,sizeof(ABVERTEX)*4);
    ZeroMemory(&indecieArray,sizeof(WORD)*6);
}

void CCharonaObj::Clone(CCharonaObj ** clone)
{
    CCharonaObj * copyLocal = new CCharonaObj();

	copyLocal->m_centerPixelSnapShot = m_centerPixelSnapShot;
    copyLocal->m_size                = m_size;
	copyLocal->m_scaleFactor         = m_scaleFactor;
	copyLocal->m_fadeTime            = m_fadeTime;
	copyLocal->m_textureName         = m_textureName;
	copyLocal->m_textureIndex        = m_textureIndex;
	copyLocal->m_startDistance       = m_startDistance;
    copyLocal->m_endDistance         = m_endDistance;
	copyLocal->m_visibleDistance     = m_visibleDistance;
    
    CopyMemory(copyLocal->vertexArray,vertexArray,sizeof(ABVERTEX)*4);
    CopyMemory(copyLocal->indecieArray,indecieArray,sizeof(WORD)*6);

	if(m_material)
	   m_material->Clone(&copyLocal->m_material);

	*clone = copyLocal;
}


BOOL CCharonaObj::InitializeVis()
{
    if(!m_material){//no material set yet
        m_material = new CMaterialObject();
        m_material->m_blendMode = 2;
	    m_material->m_baseMaterial.Diffuse.r  = 0.0f;
        m_material->m_baseMaterial.Diffuse.g  = 0.0f;
        m_material->m_baseMaterial.Diffuse.b  = 0.0f;
        m_material->m_baseMaterial.Diffuse.a  = 0.0f;
        m_material->m_baseMaterial.Ambient.r  = 0.0f;
        m_material->m_baseMaterial.Ambient.g  = 0.0f;
        m_material->m_baseMaterial.Ambient.b  = 0.0f;
        m_material->m_baseMaterial.Ambient.a  = 0.0f;
        m_material->m_baseMaterial.Emissive.r = 1.0f;
        m_material->m_baseMaterial.Emissive.g = 1.0f;
        m_material->m_baseMaterial.Emissive.b = 1.0f;
        m_material->m_baseMaterial.Emissive.a = 1.0f;
        m_material->Reset();
	}
	if(!m_visual){
    m_visual = new CAdvancedVertexObj();
	m_visual->m_indecieArray             = new WORD[6];
	}
    m_visual->m_indecieCount             = indecieCount;
	m_visual->m_vertexCount              = vertexCount; 
	
	m_visual->m_uvCount = 1;
 
	ZeroMemory(&vertexArray,sizeof(ABVERTEX)*4);
    ZeroMemory(&indecieArray,sizeof(WORD)*6);
   
    //build visual
	vertexArray[0].x  = -m_size;
	vertexArray[0].y  =  m_size;
	vertexArray[0].tx0.tu =  0.0f;
    vertexArray[0].tx0.tv =  0.0f;
	vertexArray[0].nz =  -1.0f;

	vertexArray[1].x =  m_size;
	vertexArray[1].y =  m_size;
	vertexArray[1].tx0.tu =  1.0f;
    vertexArray[1].tx0.tv =  0.0f;
	vertexArray[1].nz =  -1.0f;

	vertexArray[2].x =  m_size;
	vertexArray[2].y = -m_size;
	vertexArray[2].tx0.tu =  1.0f;
    vertexArray[2].tx0.tv =  1.0f;
	vertexArray[2].nz =  -1.0f;

	vertexArray[3].x = -m_size;
	vertexArray[3].y = -m_size;
	vertexArray[3].tx0.tu =  0.0f;
    vertexArray[3].tx0.tv =  1.0f;
	vertexArray[3].nz =  -1.0f;

    //for(int i=0;i<4;i++)
	    //CopyMemory(&m_visual->m_origVertexArray[i],&vertexArray[i],sizeof(ABVERTEX));

	m_visual->SetABVERTEXStyle(NULL,vertexArray,m_visual->m_vertexCount);


	indecieArray[0] = 0;
	indecieArray[1] = 1;
	indecieArray[2] = 3;
	indecieArray[3] = 1;
	indecieArray[4] = 2;
	indecieArray[5] = 3;

	m_visual->m_indecieArray[0] = 0;
	m_visual->m_indecieArray[1] = 1;
	m_visual->m_indecieArray[2] = 3;
	m_visual->m_indecieArray[3] = 1;
	m_visual->m_indecieArray[4] = 2;
	m_visual->m_indecieArray[5] = 3;


	

    //m_material->SetFadeOut(m_fadeTime);
    //m_changeStamp = GetTickCount();
	m_material->m_useMaterial.Emissive.r = 0.0f;
    m_material->m_useMaterial.Emissive.g = 0.0f;
    m_material->m_useMaterial.Emissive.b = 0.0f;
    m_material->m_useMaterial.Emissive.a = 0.0f;



return TRUE;
}


void CCharonaObj::SafeDelete()
{
    if(m_material)
	   delete m_material;
	   m_material=0;

	if(m_visual)
	{
       m_visual->SafeDelete();
	   delete m_visual;
	   m_visual = 0;
	}
}


void CCharonaObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){

	   ar << m_size
		  << m_scaleFactor
		  << m_fadeTime
		  << m_material
          << vertexCount
	      << indecieCount
		  << m_textureName
	      << m_textureIndex
		  << m_startDistance
          << m_endDistance
		  << m_visibleDistance;

	}
	else
	{
       ar >> m_size
		  >> m_scaleFactor
		  >> m_fadeTime
		  >> m_material
		  >> vertexCount
	      >> indecieCount
		  >> m_textureName
	      >> m_textureIndex
		  >> m_startDistance
          >> m_endDistance
		  >> m_visibleDistance;

	   ZeroMemory(&m_currentMatrix,sizeof(D3DMATRIX));
	   ZeroMemory(&m_center,sizeof(D3DVECTOR));
	   m_visual = NULL;
	   InitializeVis();
	   m_changeStamp = 0;
	   m_centerPixelSnapShot = 0;
       m_validThisRound = FALSE;
	   m_currentlyVisible = FALSE;
	   m_currentDistance = 0.0f;
	   
	}
}     


IMPLEMENT_SERIAL(CCharonaObj, CObject, 0)


