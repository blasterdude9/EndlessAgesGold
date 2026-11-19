#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CMenuBarClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMenuBarObj::CMenuBarObj()
{
   m_verticleStyle   = FALSE;
   m_flip            = FALSE;
   m_max             = 1.0f;
   m_min             = 0.0f;
   m_barLength       = 0.4f;
   m_barWidth        = 0.2f;
   m_xRoot           = 0.0f;
   m_yRoot           = 0.0f;
   m_cur             = 0.5f;
   m_barDynamic      = NULL;
   m_barStatic       = NULL;
   m_visualReference = 0;
   m_barUseType      = 0;
}

void CMenuBarObj::SafeDelete()
{
   if(m_barDynamic)
   {
	  m_barDynamic->SafeDelete();
      delete m_barDynamic;
	  m_barDynamic = 0;
   }
 
   if(m_barStatic)
   {
	  m_barStatic->SafeDelete();
      delete m_barStatic;
	  m_barStatic = 0;
   }

}

void CMenuBarObj::SetStateByTwoAmounts(float cur,float max)
{
  if(max <= 0.0f)
     max = 1.0f;

  if(cur > max)
	  cur = max;

     m_cur = cur/max;
}

void CMenuBarObj::SetStateByDirectValue(float cur)
{
     m_cur = cur;
}

void CMenuBarObj::SetSizePosPorperties(float length, float width, float xPos, float yPos)
{
   m_barLength       = length;
   m_barWidth        = width;
   m_xRoot           = xPos;
   m_yRoot           = yPos;
}

void CMenuBarObj::SetBackColor(float r, float g, float b, float a)
{
     if(!m_barStatic)
		 return;

        if(a < 1.0f)
		   m_barStatic->List_materialObject->m_blendMode = 1;//alpha

        m_barStatic->List_materialObject->m_useMaterial.Diffuse.a = a;
		m_barStatic->List_materialObject->m_useMaterial.Emissive.r = r;
        m_barStatic->List_materialObject->m_useMaterial.Emissive.g = g;
        m_barStatic->List_materialObject->m_useMaterial.Emissive.b = b;
        m_barStatic->List_materialObject->m_useMaterial.Emissive.a = 1.0f;
}

void CMenuBarObj::SetForeColor(float r, float g, float b, float a)
{
     if(!m_barDynamic)
		 return;

	    if(a < 1.0f)
		   m_barDynamic->List_materialObject->m_blendMode = 1;//alpha

        m_barDynamic->List_materialObject->m_useMaterial.Diffuse.a = a;
		m_barDynamic->List_materialObject->m_useMaterial.Emissive.r = r;
        m_barDynamic->List_materialObject->m_useMaterial.Emissive.g = g;
        m_barDynamic->List_materialObject->m_useMaterial.Emissive.b = b;
        m_barDynamic->List_materialObject->m_useMaterial.Emissive.a = 1.0f;
}

void CMenuBarObj::Clone(CMenuBarObj ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice)
{

	CMenuBarObj * copyLocal = new CMenuBarObj();

	copyLocal->m_verticleStyle   = m_verticleStyle;
    copyLocal->m_flip            = m_flip;
    copyLocal->m_max             = m_max;
    copyLocal->m_min             = m_min;
    copyLocal->m_barLength       = m_barLength;
    copyLocal->m_barWidth        = m_barWidth;
    copyLocal->m_xRoot           = m_xRoot;
    copyLocal->m_yRoot           = m_yRoot;
	copyLocal->m_visualReference = m_visualReference;
	copyLocal->m_barUseType      = m_barUseType;

    if(m_barStatic)
	   m_barStatic->Clone(&copyLocal->m_barStatic,g_pd3dDevice);

	if(m_barDynamic)
	   m_barDynamic->Clone(&copyLocal->m_barDynamic,g_pd3dDevice);

  
	*clone = copyLocal;
}


void CMenuBarObj::Initialize(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
    if(!m_barStatic)
	{
        m_barStatic = new CEXMeshObj();
        m_barStatic->List_materialObject = new CMaterialObject();
		m_barStatic->List_materialObject->m_cullOn = FALSE;

		m_barStatic->List_materialObject->m_useMaterial.Diffuse.r = 0.0f;
        m_barStatic->List_materialObject->m_useMaterial.Diffuse.g = 0.0f;
        m_barStatic->List_materialObject->m_useMaterial.Diffuse.b = 0.0f;
        m_barStatic->List_materialObject->m_useMaterial.Diffuse.a = 1.0f;
		m_barStatic->List_materialObject->m_useMaterial.Emissive.r = 1.0f;
        m_barStatic->List_materialObject->m_useMaterial.Emissive.g = 1.0f;
        m_barStatic->List_materialObject->m_useMaterial.Emissive.b = 1.0f;
        m_barStatic->List_materialObject->m_useMaterial.Emissive.a = 1.0f;

		ABVERTEX7L *verts = new ABVERTEX7L[4];
		ZeroMemory(verts,sizeof(ABVERTEX7L)*4);
		WORD     *indecies = new WORD[6];
		ZeroMemory(indecies,sizeof(WORD)*6);
        m_barStatic->m_abVertexArray.m_uvCount = 7;
		m_barStatic->m_validUvCount            = 7;

		//indecie
        indecies[0] = 0;
		indecies[1] = 1;
		indecies[2] = 3;
		indecies[3] = 3;
		indecies[4] = 1;
		indecies[5] = 2;
		
		//N
		verts[0].nx = 0.0f;
		verts[0].ny = 0.0f;
		verts[0].nz = 3.0f;
		verts[1].nx = 0.0f;
		verts[1].ny = 0.0f;
		verts[1].nz = 3.0f;
		verts[2].nx = 0.0f;
		verts[2].ny = 0.0f;
		verts[2].nz = 3.0f;
		verts[3].nx = 0.0f;
		verts[3].ny = 0.0f;
		verts[3].nz = 3.0f;
        //UVSET1
		verts[0].tx0.tu = 0.0f;
		verts[0].tx0.tv = 0.0f;
		verts[1].tx0.tu = 1.0f;
		verts[1].tx0.tv = 0.0f;
		verts[2].tx0.tu = 1.0f;
		verts[2].tx0.tv = 1.0f;
		verts[3].tx0.tu = 0.0f;
		verts[3].tx0.tv = 1.0f;
        //UVSET2
		verts[0].tx1.tu = 0.0f;
		verts[0].tx1.tv = 0.0f;
		verts[1].tx1.tu = 1.0f;
		verts[1].tx1.tv = 0.0f;
		verts[2].tx1.tu = 1.0f;
		verts[2].tx1.tv = 1.0f;
		verts[3].tx1.tu = 0.0f;
		verts[3].tx1.tv = 1.0f;
        //UVSET3
		verts[0].tx2.tu = 0.0f;
		verts[0].tx2.tv = 0.0f;
		verts[1].tx2.tu = 1.0f;
		verts[1].tx2.tv = 0.0f;
		verts[2].tx2.tu = 1.0f;
		verts[2].tx2.tv = 1.0f;
		verts[3].tx2.tu = 0.0f;
		verts[3].tx2.tv = 1.0f;
        //UVSET4
		verts[0].tx3.tu = 0.0f;
		verts[0].tx3.tv = 0.0f;
		verts[1].tx3.tu = 1.0f;
		verts[1].tx3.tv = 0.0f;
		verts[2].tx3.tu = 1.0f;
		verts[2].tx3.tv = 1.0f;
		verts[3].tx3.tu = 0.0f;
		verts[3].tx3.tv = 1.0f;
        //UVSET5
		verts[0].tx4.tu = 0.0f;
		verts[0].tx4.tv = 0.0f;
		verts[1].tx4.tu = 1.0f;
		verts[1].tx4.tv = 0.0f;
		verts[2].tx4.tu = 1.0f;
		verts[2].tx4.tv = 1.0f;
		verts[3].tx4.tu = 0.0f;
		verts[3].tx4.tv = 1.0f;
        //UVSET6
		verts[0].tx5.tu = 0.0f;
		verts[0].tx5.tv = 0.0f;
		verts[1].tx5.tu = 1.0f;
		verts[1].tx5.tv = 0.0f;
		verts[2].tx5.tu = 1.0f;
		verts[2].tx5.tv = 1.0f;
		verts[3].tx5.tu = 0.0f;
		verts[3].tx5.tv = 1.0f;
		//UVSET7
		verts[0].tx6.tu = 0.0f;
		verts[0].tx6.tv = 0.0f;
		verts[1].tx6.tu = 1.0f;
		verts[1].tx6.tv = 0.0f;
		verts[2].tx6.tu = 1.0f;
		verts[2].tx6.tv = 1.0f;
		verts[3].tx6.tu = 0.0f;
		verts[3].tx6.tv = 1.0f;

        m_barStatic->m_abVertexArray.m_vertexCount  = 4;
        m_barStatic->m_abVertexArray.m_indecieArray = indecies;
        m_barStatic->m_abVertexArray.m_indecieCount = 6;
        m_barStatic->m_abVertexArray.m_vertexArray7 = verts;

        m_barStatic->InitBuffer(g_pd3dDevice,NULL);

	}

	if(!m_barDynamic)
	{
        m_barDynamic = new CEXMeshObj();
        m_barDynamic->List_materialObject = new CMaterialObject();
		m_barDynamic->List_materialObject->m_cullOn = FALSE;

		m_barDynamic->List_materialObject->m_useMaterial.Diffuse.r = 0.0f;
        m_barDynamic->List_materialObject->m_useMaterial.Diffuse.g = 0.0f;
        m_barDynamic->List_materialObject->m_useMaterial.Diffuse.b = 0.0f;
        m_barDynamic->List_materialObject->m_useMaterial.Diffuse.a = 1.0f;
		m_barDynamic->List_materialObject->m_useMaterial.Emissive.r = 1.0f;
        m_barDynamic->List_materialObject->m_useMaterial.Emissive.g = 1.0f;
        m_barDynamic->List_materialObject->m_useMaterial.Emissive.b = 1.0f;
        m_barDynamic->List_materialObject->m_useMaterial.Emissive.a = 1.0f;

		ABVERTEX7L *verts = new ABVERTEX7L[4];
		ZeroMemory(verts,sizeof(ABVERTEX7L)*4);
		WORD     *indecies = new WORD[6];
		ZeroMemory(indecies,sizeof(WORD)*6);
        m_barDynamic->m_abVertexArray.m_uvCount = 7;
		m_barDynamic->m_validUvCount            = 7;

        //indecie
        indecies[0] = 0;
		indecies[1] = 1;
		indecies[2] = 3;
		indecies[3] = 3;
		indecies[4] = 1;
		indecies[5] = 2;

        //N
		verts[0].nx = 0.0f;
		verts[0].ny = 0.0f;
		verts[0].nz = 1.0f;
		verts[1].nx = 0.0f;
		verts[1].ny = 0.0f;
		verts[1].nz = 1.0f;
		verts[2].nx = 0.0f;
		verts[2].ny = 0.0f;
		verts[2].nz = 1.0f;
		verts[3].nx = 0.0f;
		verts[3].ny = 0.0f;
		verts[3].nz = 1.0f;
        //UVSET1
		verts[0].tx0.tu = 0.0f;
		verts[0].tx0.tv = 0.0f;
		verts[1].tx0.tu = 1.0f;
		verts[1].tx0.tv = 0.0f;
		verts[2].tx0.tu = 1.0f;
		verts[2].tx0.tv = 1.0f;
		verts[3].tx0.tu = 0.0f;
		verts[3].tx0.tv = 1.0f;
        //UVSET2
		verts[0].tx1.tu = 0.0f;
		verts[0].tx1.tv = 0.0f;
		verts[1].tx1.tu = 1.0f;
		verts[1].tx1.tv = 0.0f;
		verts[2].tx1.tu = 1.0f;
		verts[2].tx1.tv = 1.0f;
		verts[3].tx1.tu = 0.0f;
		verts[3].tx1.tv = 1.0f;
        //UVSET3
		verts[0].tx2.tu = 0.0f;
		verts[0].tx2.tv = 0.0f;
		verts[1].tx2.tu = 1.0f;
		verts[1].tx2.tv = 0.0f;
		verts[2].tx2.tu = 1.0f;
		verts[2].tx2.tv = 1.0f;
		verts[3].tx2.tu = 0.0f;
		verts[3].tx2.tv = 1.0f;
        //UVSET4
		verts[0].tx3.tu = 0.0f;
		verts[0].tx3.tv = 0.0f;
		verts[1].tx3.tu = 1.0f;
		verts[1].tx3.tv = 0.0f;
		verts[2].tx3.tu = 1.0f;
		verts[2].tx3.tv = 1.0f;
		verts[3].tx3.tu = 0.0f;
		verts[3].tx3.tv = 1.0f;
        //UVSET5
		verts[0].tx4.tu = 0.0f;
		verts[0].tx4.tv = 0.0f;
		verts[1].tx4.tu = 1.0f;
		verts[1].tx4.tv = 0.0f;
		verts[2].tx4.tu = 1.0f;
		verts[2].tx4.tv = 1.0f;
		verts[3].tx4.tu = 0.0f;
		verts[3].tx4.tv = 1.0f;
        //UVSET6
		verts[0].tx5.tu = 0.0f;
		verts[0].tx5.tv = 0.0f;
		verts[1].tx5.tu = 1.0f;
		verts[1].tx5.tv = 0.0f;
		verts[2].tx5.tu = 1.0f;
		verts[2].tx5.tv = 1.0f;
		verts[3].tx5.tu = 0.0f;
		verts[3].tx5.tv = 1.0f;
		//UVSET7
		verts[0].tx6.tu = 0.0f;
		verts[0].tx6.tv = 0.0f;
		verts[1].tx6.tu = 1.0f;
		verts[1].tx6.tv = 0.0f;
		verts[2].tx6.tu = 1.0f;
		verts[2].tx6.tv = 1.0f;
		verts[3].tx6.tu = 0.0f;
		verts[3].tx6.tv = 1.0f;

        m_barDynamic->m_abVertexArray.m_vertexCount  = 4;
        m_barDynamic->m_abVertexArray.m_indecieArray = indecies;
        m_barDynamic->m_abVertexArray.m_indecieCount = 6;
        m_barDynamic->m_abVertexArray.m_vertexArray7 = verts;
        if(g_pd3dDevice)
		m_barDynamic->InitBuffer(g_pd3dDevice,NULL);
	}

	//update static
	int loop = 0;
	float curX = m_xRoot;
	float curY = m_yRoot;
	float inset = 3.0f;
	
     curY += (m_barWidth * .5f);
     m_barStatic->m_abVertexArray.SetPositionXYZ(loop,curX,curY,inset);
	 loop++;
	 curX += m_barLength;
     m_barStatic->m_abVertexArray.SetPositionXYZ(loop,curX,curY,inset);
     loop++;
	 curY -= m_barWidth;
     m_barStatic->m_abVertexArray.SetPositionXYZ(loop,curX,curY,inset);
	 loop++;
	 curX -= m_barLength;
     m_barStatic->m_abVertexArray.SetPositionXYZ(loop,curX,curY,inset);
	 if(g_pd3dDevice)
     m_barStatic->m_abVertexArray.ReinitializeDxVbuffersPositionOnly(g_pd3dDevice);
	 //update dynamic
	 loop = 0;
	 curX = m_xRoot;
	 curY = m_yRoot;
	
     curY += (m_barWidth * .5f);
     m_barDynamic->m_abVertexArray.SetPositionXYZ(loop,curX,curY,inset);
	 loop++;
	 curX += (m_barLength*m_cur);
     m_barDynamic->m_abVertexArray.SetPositionXYZ(loop,curX,curY,inset);
     loop++;
	 curY -= m_barWidth;
     m_barDynamic->m_abVertexArray.SetPositionXYZ(loop,curX,curY,inset);
	 loop++;
	 curX -= (m_barLength*m_cur);
     m_barDynamic->m_abVertexArray.SetPositionXYZ(loop,curX,curY,inset);
	 if(g_pd3dDevice)
     m_barDynamic->m_abVertexArray.ReinitializeDxVbuffersPositionOnly(g_pd3dDevice);

}


void CMenuBarObj::Render(LPDIRECT3DDEVICE9 g_pd3dDevice,
						 CStateManagementObj * m_stateManager,
						 CTextureDBObjList * TextureDataBase,
						 float gl_adjust
						 )
{
    if(m_max==0)
	   return;

	Initialize(g_pd3dDevice);
	
    m_barStatic->List_materialObject->Callback();
	m_stateManager->SetBlendType(g_pd3dDevice,m_barStatic->List_materialObject->m_blendMode);
    //g_pd3dDevice->SetMaterial(&m_barStatic->List_materialObject->m_useMaterial);
	m_stateManager->SetMaterial(g_pd3dDevice,&m_barStatic->List_materialObject->m_useMaterial);
	m_stateManager->SetDiffuseSource(m_barStatic->m_abVertexArray.m_advancedFixedMode,g_pd3dDevice);
	m_stateManager->SetVertexType(m_barStatic->m_abVertexArray.m_uvCount,g_pd3dDevice);
	
	m_stateManager->SetTextureState(m_barStatic->List_textureSelect[0],m_stateManager->m_intZero,g_pd3dDevice,TextureDataBase);	
	int loop=0;
	for(loop=1;loop<7;loop++){
	    m_stateManager->SetTextureState(m_barStatic->List_textureSelect[loop],loop,g_pd3dDevice,TextureDataBase);
        m_stateManager->SetBlendLevel(loop,m_barStatic->List_BlendMethod[(loop-1)],g_pd3dDevice);
	}

    for(loop=0;loop<8;loop++)
    {
	        m_stateManager->SetColorArg1Level(loop,m_barStatic->List_materialObject->m_colorArg1[loop],g_pd3dDevice);
            m_stateManager->SetColorArg2Level(loop,m_barStatic->List_materialObject->m_colorArg2[loop],g_pd3dDevice);
			m_stateManager->SetTexcoordIndexLevel(loop,m_barStatic->List_materialObject->m_texcoordLevels[loop],g_pd3dDevice);
    } 

	if(m_barStatic->List_materialObject->m_enableTextureMovement == TRUE){//texture Movement
	   if(m_barStatic->List_materialObject->m_enableSet0 == TRUE){//set 0
          m_barStatic->m_abVertexArray.ApplyMoveTexture(m_barStatic->List_materialObject->m_textureMovement[0].tu,m_barStatic->List_materialObject->m_textureMovement[0].tv,0,gl_adjust,m_stateManager->m_currentVertexType);
	   }//end set 0
	   if(m_barStatic->List_materialObject->m_enableSet1 == TRUE){//set 1
          m_barStatic->m_abVertexArray.ApplyMoveTexture(m_barStatic->List_materialObject->m_textureMovement[1].tu,m_barStatic->List_materialObject->m_textureMovement[1].tv,1,gl_adjust,m_stateManager->m_currentVertexType);
	   }//end set 1
	}//end texture Movement
	
	m_barStatic->Render(g_pd3dDevice,m_stateManager->m_currentVertexType,m_barStatic->m_chromeUvSet);


	m_barDynamic->List_materialObject->Callback();
	//g_pd3dDevice->SetMaterial(&m_barDynamic->List_materialObject->m_useMaterial);
	m_stateManager->SetBlendType(g_pd3dDevice,m_barDynamic->List_materialObject->m_blendMode);
	m_stateManager->SetMaterial(g_pd3dDevice,&m_barDynamic->List_materialObject->m_useMaterial);
	m_stateManager->SetDiffuseSource(m_barDynamic->m_abVertexArray.m_advancedFixedMode,g_pd3dDevice);
	m_stateManager->SetVertexType(m_barDynamic->m_abVertexArray.m_uvCount,g_pd3dDevice);
	
	m_stateManager->SetTextureState(m_barDynamic->List_textureSelect[0],m_stateManager->m_intZero,g_pd3dDevice,TextureDataBase);	
	for(loop=1;loop<7;loop++){
	   m_stateManager->SetTextureState(m_barDynamic->List_textureSelect[loop],loop,g_pd3dDevice,TextureDataBase);
       m_stateManager->SetBlendLevel(loop,m_barDynamic->List_BlendMethod[(loop-1)],g_pd3dDevice);
	}

	for(loop=0;loop<8;loop++)
    {
	        m_stateManager->SetColorArg1Level(loop,m_barDynamic->List_materialObject->m_colorArg1[loop],g_pd3dDevice);
            m_stateManager->SetColorArg2Level(loop,m_barDynamic->List_materialObject->m_colorArg2[loop],g_pd3dDevice);
			m_stateManager->SetTexcoordIndexLevel(loop,m_barDynamic->List_materialObject->m_texcoordLevels[loop],g_pd3dDevice);
    } 


	if(m_barDynamic->List_materialObject->m_enableTextureMovement == TRUE){//texture Movement
	   if(m_barDynamic->List_materialObject->m_enableSet0 == TRUE){//set 0
          m_barDynamic->m_abVertexArray.ApplyMoveTexture(m_barDynamic->List_materialObject->m_textureMovement[0].tu,m_barDynamic->List_materialObject->m_textureMovement[0].tv,0,gl_adjust,m_stateManager->m_currentVertexType);
	   }//end set 0
	   if(m_barDynamic->List_materialObject->m_enableSet1 == TRUE){//set 1
          m_barDynamic->m_abVertexArray.ApplyMoveTexture(m_barDynamic->List_materialObject->m_textureMovement[1].tu,m_barDynamic->List_materialObject->m_textureMovement[1].tv,1,gl_adjust,m_stateManager->m_currentVertexType);
	   }//end set 1
	}//end texture Movement
	
	m_barDynamic->Render(g_pd3dDevice,m_stateManager->m_currentVertexType,m_barDynamic->m_chromeUvSet);
}

void CMenuBarObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CMenuBarObj * spnPtr = (CMenuBarObj *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}


void CMenuBarObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_verticleStyle
          << m_flip           
          << m_max          
          << m_min             
          << m_barLength       
          << m_barWidth       
          << m_xRoot         
          << m_yRoot 
		  << m_barDynamic 
          << m_barStatic
		  << m_cur
		  << m_visualReference
		  << m_barUseType;
	}
	else
	{
       ar >> m_verticleStyle
          >> m_flip           
          >> m_max          
          >> m_min             
          >> m_barLength       
          >> m_barWidth       
          >> m_xRoot         
          >> m_yRoot          
          >> m_barDynamic 
          >> m_barStatic
		  >> m_cur
		  >> m_visualReference
		  >> m_barUseType;
	}
}     


IMPLEMENT_SERIAL(CMenuBarObj, CObject, 0)
IMPLEMENT_SERIAL(CMenuBarObjList, CObList, 0)

