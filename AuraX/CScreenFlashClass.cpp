#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>

#include "CScreenFlashClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CScreenFlashObj::CScreenFlashObj()
{
    m_visual      = NULL;	
    m_on          = FALSE;
    m_duration    = 50;//milliseconds
    m_stamp       = 0;
}

void CScreenFlashObj::SafeDelete()
{
   if(m_visual)
   { 
      m_visual->SafeDelete();
	  delete m_visual;
	  m_visual = 0;
   }
}

void CScreenFlashObj::DoIt(DWORD duration,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
    m_on          = TRUE;
    m_duration    = duration;//milliseconds
    m_stamp       = GetTickCount();

    if(!m_visual)
	{//need initialization
       m_visual = new CEXMeshObj();
       m_visual->List_materialObject = new CMaterialObject();
	   m_visual->List_materialObject->m_cullOn = FALSE;

	   m_visual->List_materialObject->m_useMaterial.Diffuse.r = 0.0f;
	   m_visual->List_materialObject->m_useMaterial.Diffuse.g = 0.0f;
	   m_visual->List_materialObject->m_useMaterial.Diffuse.b = 0.0f;
	   m_visual->List_materialObject->m_useMaterial.Diffuse.a = 0.0f;
	   m_visual->List_materialObject->m_useMaterial.Ambient.r = 0.0f;
	   m_visual->List_materialObject->m_useMaterial.Ambient.g = 0.0f;
	   m_visual->List_materialObject->m_useMaterial.Ambient.b = 0.0f;
	   m_visual->List_materialObject->m_useMaterial.Ambient.a = 0.0f;
	   m_visual->List_materialObject->m_useMaterial.Emissive.r = 0.2f;
       m_visual->List_materialObject->m_useMaterial.Emissive.g = 0.0f;
       m_visual->List_materialObject->m_useMaterial.Emissive.b = 0.0f;
       m_visual->List_materialObject->m_useMaterial.Emissive.a = 0.0f;

		ABVERTEX0L *verts = new ABVERTEX0L[4];
		ZeroMemory(verts,sizeof(ABVERTEX0L)*4);
		WORD     *indecies = new WORD[6];
		ZeroMemory(indecies,sizeof(WORD)*6);
        m_visual->m_abVertexArray.m_uvCount = 0;
		m_visual->m_validUvCount            = 0;

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

        m_visual->m_abVertexArray.m_vertexCount  = 4;
        m_visual->m_abVertexArray.m_indecieArray = indecies;
        m_visual->m_abVertexArray.m_indecieCount = 6;
        m_visual->m_abVertexArray.m_vertexArray0 = verts;


        int loop     = 0;
	    float curX   = 0.0f;
	    float curY   = 0.0f;
	    float inset  = 3.0f;
		float width  = 8.0f;
		float hieght = 8.0f;
	
        curY += (hieght * .5f);
		curX -= (width  * .5f);
        m_visual->m_abVertexArray.SetPositionXYZ(loop,curX,curY,inset);
	    loop++;
	    curX += width;
        m_visual->m_abVertexArray.SetPositionXYZ(loop,curX,curY,inset);
        loop++;
	    curY -= hieght;
        m_visual->m_abVertexArray.SetPositionXYZ(loop,curX,curY,inset);
	    loop++;
	    curX -= width;
        m_visual->m_abVertexArray.SetPositionXYZ(loop,curX,curY,inset);
	    
        m_visual->InitBuffer(g_pd3dDevice,NULL);
	}//end need initialization
}

void CScreenFlashObj::Callback(DWORD currentTime,
							   LPDIRECT3DDEVICE9 g_pd3dDevice,
							   CStateManagementObj * m_stateManager,
							   CTextureDBObjList * TextureDataBase,
							   D3DMATRIX & camMatrix)
{
    if(!m_on)
		return;

     if((currentTime - m_stamp) > m_duration)
	 {
        m_on = FALSE;
		return;
	 }

	 if(m_visual)
	 Render(g_pd3dDevice,m_stateManager,TextureDataBase,camMatrix);

}

void CScreenFlashObj::Render(LPDIRECT3DDEVICE9 g_pd3dDevice,
							 CStateManagementObj * m_stateManager,
							 CTextureDBObjList * TextureDataBase,
							 D3DMATRIX camMatrix)
{
    //m_stateManager->SetWorldMatrix(&m_stateManager->m_matrixInit,g_pd3dDevice);

	camMatrix._41 = 0.0f;
	camMatrix._42 = 0.0f;
	camMatrix._43 = 0.0f;
	m_stateManager->SetWorldMatrix((D3DXMATRIX*)&camMatrix,g_pd3dDevice);

    m_stateManager->SetShader(g_pd3dDevice,m_stateManager->m_shderSystemRef,m_stateManager->m_intNegOne);
	m_stateManager->SetPxShader(g_pd3dDevice,m_stateManager->m_pxShderSystemRef,m_stateManager->m_intNegOne);

	m_stateManager->SetBlendType(g_pd3dDevice,2);
    g_pd3dDevice->SetMaterial(&m_visual->List_materialObject->m_useMaterial);
	
	m_stateManager->SetTextureState(m_stateManager->m_intNegOne,m_stateManager->m_intZero,g_pd3dDevice,TextureDataBase);	
	for(int loop=1;loop<7;loop++){
	    m_stateManager->SetTextureState(loop,m_stateManager->m_intNegOne,g_pd3dDevice,TextureDataBase);
        m_stateManager->SetBlendLevel(loop,m_stateManager->m_intNegOne,g_pd3dDevice);
	}
	m_stateManager->SetDiffuseSource(m_visual->m_abVertexArray.m_advancedFixedMode,g_pd3dDevice);
	m_stateManager->SetVertexType(m_visual->m_abVertexArray.m_uvCount,g_pd3dDevice);
	m_visual->Render(g_pd3dDevice,m_stateManager->m_currentVertexType,0);
}