#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>

#include "CGlobalMaterialShaderLib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CGlobalMaterialShader::CGlobalMaterialShader()
{
    m_requiredUvCount            = 0;
    m_chromeUvSet                = 0;
    m_enableChroming             = 0;
	m_curPixShader               = -1;
	m_curVSShader                = -1;
	m_shaderChain                = NULL;
    
	for(int i=0;i<10;i++)
	   m_textureIndexArray[i] = -1;
    
	
	ZeroMemory(m_blendArray,sizeof(int)*10);//
	
    m_useOverrideMaterial        = 0;
	ZeroMemory(&m_overrideMaterial,sizeof(D3DMATERIAL9));
}

void CGlobalMaterialShader::ScaleForRuntimeCompatibility(BOOL & bumpSupport, float & supportedPx, float & supportedVs)
{

    //shader selection
    if(m_shaderChain)
       m_shaderChain->GetCompatible(this->m_curVSShader,this->m_curPixShader,supportedVs,supportedPx);

    if(!bumpSupport)//reconfig
	for(int i=0;i<10;i++)
	{
	   //m_textureIndexArray[i] = -1;

	   if(m_blendArray[i] == 7)//incompatible blend found 
	   {
          m_blendArray[i] = m_blendArray[i+1];
		  m_textureIndexArray[i] = m_textureIndexArray[i+1];

		  m_textureStringArray[i] = m_textureStringArray[i+1];

          if(m_chromeUvSet > 0)
		     m_chromeUvSet--;


		  m_textureIndexArray[i+1] = 1;
		  m_blendArray[i+1] = -1;

		  return;
	   }
	   
	}   
}

void CGlobalMaterialShaderList::ScaleForRuntimeCompatibility(BOOL & bumpSupport, float & supportedPx, float & supportedVs)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CGlobalMaterialShader * spnPtr = (CGlobalMaterialShader *)GetNext(posLoc);
        spnPtr->ScaleForRuntimeCompatibility(bumpSupport,supportedPx,supportedVs);
	}
}

void CGlobalMaterialShaderList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CGlobalMaterialShader * spnPtr = (CGlobalMaterialShader *)GetNext(posLoc);
	
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}


CGlobalMaterialShader * CGlobalMaterialShaderList::GetByIndex(int index)
{
    if(index < 0)
	   return NULL;


	POSITION posLoc = FindIndex(index);
	if(!posLoc)
		return NULL;
    CGlobalMaterialShader * spnPtr = (CGlobalMaterialShader *)GetAt(posLoc);
    return spnPtr;
}

void CGlobalMaterialShader::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
       ar.SetObjectSchema(2);
       ASSERT(ar.GetObjectSchema() == 2);


	   ar << m_requiredUvCount
		  << m_chromeUvSet
		  << m_enableChroming
		  << m_useOverrideMaterial
		  << m_shaderName
		  << m_overrideMaterial.Diffuse.r
          << m_overrideMaterial.Diffuse.g
          << m_overrideMaterial.Diffuse.b
          << m_overrideMaterial.Diffuse.a
          << m_overrideMaterial.Ambient.r
          << m_overrideMaterial.Ambient.g
          << m_overrideMaterial.Ambient.b
          << m_overrideMaterial.Ambient.a
          << m_overrideMaterial.Specular.r
          << m_overrideMaterial.Specular.g
          << m_overrideMaterial.Specular.b
          << m_overrideMaterial.Specular.a
          << m_overrideMaterial.Emissive.r
          << m_overrideMaterial.Emissive.g
          << m_overrideMaterial.Emissive.b
          << m_overrideMaterial.Emissive.a;

	   for(int i=0;i<10;i++)
	   {
          ar << m_textureIndexArray[i] << m_textureStringArray[i] << m_blendArray[i];
	   }

	   ar << m_shaderChain 
		  << m_shaderNotes;

	}
	else
	{
       int version = ar.GetObjectSchema();

       ar >> m_requiredUvCount
		  >> m_chromeUvSet
		  >> m_enableChroming
		  >> m_useOverrideMaterial
		  >> m_shaderName
		  >> m_overrideMaterial.Diffuse.r
          >> m_overrideMaterial.Diffuse.g
          >> m_overrideMaterial.Diffuse.b
          >> m_overrideMaterial.Diffuse.a
          >> m_overrideMaterial.Ambient.r
          >> m_overrideMaterial.Ambient.g
          >> m_overrideMaterial.Ambient.b
          >> m_overrideMaterial.Ambient.a
          >> m_overrideMaterial.Specular.r
          >> m_overrideMaterial.Specular.g
          >> m_overrideMaterial.Specular.b
          >> m_overrideMaterial.Specular.a
          >> m_overrideMaterial.Emissive.r
          >> m_overrideMaterial.Emissive.g
          >> m_overrideMaterial.Emissive.b
          >> m_overrideMaterial.Emissive.a;

	   for(int i=0;i<10;i++)
	   {
          ar >> m_textureIndexArray[i] >> m_textureStringArray[i] >> m_blendArray[i];
	   }
	   //init
	   m_curPixShader               = -1;
	   m_curVSShader                = -1;
	   m_shaderChain                = NULL;
	   if(version == 2)
	   {
          ar >> m_shaderChain
			 >> m_shaderNotes;

	   }
	}
}     


IMPLEMENT_SERIAL(CShaderDefObj, CObject, 0)
IMPLEMENT_SERIAL(CShaderChain, CObList, 0)
IMPLEMENT_SERIAL(CGlobalMaterialShader, CObject, VERSIONABLE_SCHEMA|2)
IMPLEMENT_SERIAL(CGlobalMaterialShaderList, CObList, 0)

