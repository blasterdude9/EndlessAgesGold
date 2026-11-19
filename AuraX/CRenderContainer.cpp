#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>

#include "CRenderContainer.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRenderContainerObj::CRenderContainerObj(D3DMATRIX & positionalMatrix,
										 CMaterialObject * materialRef,
										 CEXMeshObj      * mesh)
{
    m_positionalMatrix          = positionalMatrix;
	m_materialReference         = materialRef;
    m_mesh                      = mesh;
}

 void CRenderContainerObj::SafeDelete()
{


}

 void CRenderContainerObj::Render(LPDIRECT3DDEVICE9     g_pd3dDevice,
										BOOL                & mirrorEnable,
										D3DMATRIX           & mirrorMatrix,
										D3DVECTOR           & locationVect,
										CStateManagementObj * stateManager,
										CTextureDBObjList   * textureDataBase,
										float               & gl_adjust,
										int                 & totalIndecieCount,
										int                 & objectsRendered)
{
	//if(!m_mesh->m_visibilityTrack)//do not render this
		//return;
    m_positionalMatrix._41 = m_positionalMatrix._41 - locationVect.x;
	m_positionalMatrix._42 = m_positionalMatrix._42 - locationVect.y;
	m_positionalMatrix._43 = m_positionalMatrix._43 - locationVect.z;
	if(mirrorEnable)
	   GL_matrixFunctions.fastMatrixMultiply(m_positionalMatrix,m_positionalMatrix,mirrorMatrix);
    //set world transform and hardware material
	//m_materialReference->Callback();
	stateManager->SetFogFilter(g_pd3dDevice,m_materialReference->m_fogFilter);
	stateManager->SetFogOverrideState(m_materialReference->m_fogOverride,g_pd3dDevice);
	stateManager->SetBlendType(g_pd3dDevice,m_blendRef);
	stateManager->SetCullState(m_materialReference->m_cullOn,g_pd3dDevice);
	stateManager->SetWireFillState(m_materialReference->m_forceWireFrame,g_pd3dDevice);
	stateManager->SetTextureTileMode(m_materialReference->m_textureTileType,g_pd3dDevice);
	stateManager->SetMaterial(g_pd3dDevice,&m_materialRef);
	//g_pd3dDevice->SetMaterial(&m_materialRef);
		
    
	stateManager->SetWorldMatrix((D3DXMATRIX*)&m_positionalMatrix,g_pd3dDevice);
	//set textures and multitexture states
	stateManager->SetTextureState(m_mesh->List_textureSelect[0],stateManager->m_intZero,g_pd3dDevice,textureDataBase);	
	if(m_mesh->List_BlendMethod[9]<0)
	   stateManager->SetBlendLevel(stateManager->m_intZero,stateManager->m_intZero,g_pd3dDevice);
	else
	   stateManager->SetBlendLevel(stateManager->m_intZero,m_mesh->List_BlendMethod[9],g_pd3dDevice);
	for(int loop=1;loop<stateManager->m_multiTextureSupported;loop++){
        stateManager->SetTextureState(m_mesh->List_textureSelect[loop],loop,g_pd3dDevice,textureDataBase);	
		stateManager->SetBlendLevel(loop,m_mesh->List_BlendMethod[(loop-1)],g_pd3dDevice);
		if(loop>7)
		   break;
	}
	
	//render 
	stateManager->SetDiffuseSource(m_mesh->m_abVertexArray.m_advancedFixedMode,g_pd3dDevice);
	stateManager->SetVertexType(m_mesh->m_abVertexArray.m_uvCount,g_pd3dDevice);
	m_mesh->TextureMovementCallback(gl_adjust,stateManager->m_currentVertexType);
	totalIndecieCount += m_mesh->Render(g_pd3dDevice,stateManager->m_currentVertexType,m_mesh->m_chromeUvSet);
	objectsRendered++;

    //emulate multipass
    //continue onto multitexture emulation utilizing multipass
    /*if(m_mesh->List_BlendMethod[0] != -1){
        if(stateManager->m_multiTextureSupported <= 1){//multipass
		//Turn on alpha blending
		stateManager->SetBlendLevelMultipass(m_mesh->List_BlendMethod[0],g_pd3dDevice);
		stateManager->SetTextureState(m_mesh->List_textureSelect[1],0,g_pd3dDevice,textureDataBase);	
        g_pd3dDevice->SetTextureStageState(0,D3DTSS_TEXCOORDINDEX,1);
        m_mesh->Render(g_pd3dDevice,stateManager->m_currentVertexType);
        //Restore state
        stateManager->SetBlendLevelMultipass(-1,g_pd3dDevice);
		}//end multipass
	}
	else{
	return;
	}

    if(m_mesh->List_BlendMethod[1] != -1){
        if(stateManager->m_multiTextureSupported <= 2){//multipass method
		//Turn on alpha blending
		stateManager->SetBlendLevelMultipass(m_mesh->List_BlendMethod[1],g_pd3dDevice);
		stateManager->SetTextureState(m_mesh->List_textureSelect[2],0,g_pd3dDevice,textureDataBase);	
        g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 2 );
        m_mesh->Render(g_pd3dDevice,stateManager->m_currentVertexType);
        //Restore state
        stateManager->SetBlendLevelMultipass(-1,g_pd3dDevice);
		}//end multipass method
	}
	else{
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	return;
	}

	if(m_mesh->List_BlendMethod[2] != -1){
        if(stateManager->m_multiTextureSupported <= 3){//multipass method
        //Turn on alpha blending
		stateManager->SetBlendLevelMultipass(m_mesh->List_BlendMethod[2],g_pd3dDevice);
		stateManager->SetTextureState(m_mesh->List_textureSelect[3],0,g_pd3dDevice,textureDataBase);	
        g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 3 );
        m_mesh->Render(g_pd3dDevice,stateManager->m_currentVertexType);
        //Restore state
        stateManager->SetBlendLevelMultipass(-1,g_pd3dDevice);
		}//end multipass method
	}
	else{
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	return;
	}

	if(m_mesh->List_BlendMethod[3] != -1){
		if(stateManager->m_multiTextureSupported <= 4){//multipass method
        //Turn on alpha blending
		stateManager->SetBlendLevelMultipass(m_mesh->List_BlendMethod[3],g_pd3dDevice);
		stateManager->SetTextureState(m_mesh->List_textureSelect[4],0,g_pd3dDevice,textureDataBase);	
        g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 4 );
        m_mesh->Render(g_pd3dDevice,stateManager->m_currentVertexType);
        //Restore state
        stateManager->SetBlendLevelMultipass(-1,g_pd3dDevice);
		}//end multipass method
	}
	else{
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	return;
	}

	if(m_mesh->List_BlendMethod[4] != -1){
		if(stateManager->m_multiTextureSupported <= 5){//multipass method
        //Turn on alpha blending
		stateManager->SetBlendLevelMultipass(m_mesh->List_BlendMethod[4],g_pd3dDevice);
		stateManager->SetTextureState(m_mesh->List_textureSelect[5],0,g_pd3dDevice,textureDataBase);	
        g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 5 );
        m_mesh->Render(g_pd3dDevice,stateManager->m_currentVertexType);
        //Restore state
        stateManager->SetBlendLevelMultipass(-1,g_pd3dDevice);
		}//end multipass method
	}
	else{
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	return;
	}

	if(m_mesh->List_BlendMethod[5] != -1){
		if(stateManager->m_multiTextureSupported <= 6){//multipass method
        //Turn on alpha blending
		stateManager->SetBlendLevelMultipass(m_mesh->List_BlendMethod[5],g_pd3dDevice);
		stateManager->SetTextureState(m_mesh->List_textureSelect[6],0,g_pd3dDevice,textureDataBase);	
        g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 6 );
        m_mesh->Render(g_pd3dDevice,stateManager->m_currentVertexType);
        //Restore state
        stateManager->SetBlendLevelMultipass(-1,g_pd3dDevice);
		}//end multipass method
	}
	else{
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	return;
	}

	if(m_mesh->List_BlendMethod[6] != -1){
		if(stateManager->m_multiTextureSupported <= 7){//multipass method
        //Turn on alpha blending
		stateManager->SetBlendLevelMultipass(m_mesh->List_BlendMethod[6],g_pd3dDevice);
		stateManager->SetTextureState(m_mesh->List_textureSelect[7],0,g_pd3dDevice,textureDataBase);	
        g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 7 );
        m_mesh->Render(g_pd3dDevice,stateManager->m_currentVertexType);
        //Restore state
        stateManager->SetBlendLevelMultipass(-1,g_pd3dDevice);
		}//end multipass method
	}
	else{
    g_pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	return;
	}*/

}

 void CRenderContainerObjList::AddEXMesh(CEXMeshObj * meshRef)
{
    CRenderContainerObj * newPtr = new CRenderContainerObj(meshRef->m_currentMatrix,
                                                         meshRef->List_materialObject,
                                                         meshRef);
	newPtr->m_blendRef      = meshRef->List_materialObject->m_blendMode;
	CopyMemory(&newPtr->m_materialRef,&meshRef->List_materialObject->m_useMaterial,sizeof(D3DMATERIAL9));
	AddTail(newPtr);
}


 void CRenderContainerObjList::Render(LPDIRECT3DDEVICE9    g_pd3dDevice,
											BOOL                &mirrorEnable,
										    D3DMATRIX           &mirrorMatrix,
											D3DVECTOR           &locationVect,
											CStateManagementObj * stateManager,
											CTextureDBObjList   * textureDataBase,
											float               & gl_adjust,
											int                 & totalIndecieCount,
											int                 & objectsRendered)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CRenderContainerObj * conPtr = (CRenderContainerObj *)GetNext(posLoc);
		conPtr->Render(g_pd3dDevice,
					   mirrorEnable,
					   mirrorMatrix,
					   locationVect,
					   stateManager,
					   textureDataBase,
					   gl_adjust,
					   totalIndecieCount,
					   objectsRendered);
	}
}

 void CRenderContainerObjList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CRenderContainerObj * conPtr = (CRenderContainerObj *)GetNext(posLoc);
		delete conPtr;
		conPtr=0;
	}
	RemoveAll();
}
