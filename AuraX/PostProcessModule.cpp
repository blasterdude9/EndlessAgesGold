#include "StdAfx.h"
#include "postprocessmodule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPostProcessModule::CPostProcessModule(void)
{
	m_material = NULL;

	for(int loop=0;loop<25;loop++)
	{
	    m_textureAssignName[loop] = "NONE";
        m_assignIndex[loop] = -1;
	}
}

CPostProcessModule::~CPostProcessModule(void)
{
	if(m_material)
	{
       delete m_material;
       m_material = 0;
	}
}

CPostProcessModuleManager::CPostProcessModuleManager()
{
    m_matrixInit2.m [0][0] = 1.0f;  m_matrixInit2.m [0][1] = 0.0f;  m_matrixInit2.m [0][2] = 0.0f;  m_matrixInit2.m [0][3] = 0.0f;
    m_matrixInit2.m [1][0] = 0.0f;  m_matrixInit2.m [1][1] = 1.0f;  m_matrixInit2.m [1][2] = 0.0f;  m_matrixInit2.m [1][3] = 0.0f;
    m_matrixInit2.m [2][0] = 0.0f;  m_matrixInit2.m [2][1] = 0.0f;  m_matrixInit2.m [2][2] = 1.0f;  m_matrixInit2.m [2][3] = 0.0f;
    m_matrixInit2.m [3][0] = 0.0f;  m_matrixInit2.m [3][1] = 0.0f;  m_matrixInit2.m [3][2] = 0.0f;  m_matrixInit2.m [3][3] = 1.0f;

	m_primaryRenderTarget = NULL;
	m_currentEffect = NULL;
	m_indexOfCurrentEffect = -1;

	m_material = new CMaterialObject();
    //hardcode one effect and make it current
    /*CPostProcessModule * effectTemp = new CPostProcessModule();
    AddTail(effectTemp);
    
	effectTemp->m_assignIndex[0] = -5;
	effectTemp->m_assignIndex[1] = -5;
	effectTemp->m_assignIndex[2] = -5;
	effectTemp->m_material->m_pixelShader = 4;
	SetEffect(0);*/
	SetEffect(-1);
}

CPostProcessModuleManager::~CPostProcessModuleManager(void)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CPostProcessModule * pMod = (CPostProcessModule *) GetNext(posLoc);
        delete pMod;
		pMod = 0;
	}
	RemoveAll();

	if(m_material)
	{
       delete m_material;
       m_material = 0;
	}
}

CPostProcessModule * CPostProcessModuleManager::GetByIndex(int indexID)
{
     if(indexID < 0)
	    return NULL;

	 POSITION posLoc = FindIndex(indexID);
	 if(!posLoc)
	    return NULL;

     CPostProcessModule * pModule = (CPostProcessModule *)GetAt(posLoc);
     return pModule;
}


void CPostProcessModuleManager::RenderPostEffectToView( CTextureDBObjList   * textureDataBase,
														DxDeviceClass       * rnaxDevice,
														CStateManagementObj * renderStateObject)
{
    if(!m_currentEffect)
        return;

    renderStateObject->SetBlendLevel(renderStateObject->m_intZero,renderStateObject->m_intZero,rnaxDevice->m_pd3dDevice);
    for(int loop=1;loop<renderStateObject->m_multiTextureSupported;loop++)
    {
        renderStateObject->SetBlendLevel(loop,renderStateObject->m_intNegOne,rnaxDevice->m_pd3dDevice);
        if(loop>7)
		   break;
    }


    HRESULT err = 0;
	renderStateObject->SetBlendType(rnaxDevice->m_pd3dDevice,-1);
	//if( FAILED( rnaxDevice->m_pd3dDevice->BeginScene() ) )return;
	renderStateObject->SetDiffuseSource(0,rnaxDevice->m_pd3dDevice);
	renderStateObject->SetMaterial(rnaxDevice->m_pd3dDevice,&m_material->m_useMaterial);
	//Texture feature set
	if(m_currentEffect->m_assignIndex[0] == -5)
	   err = rnaxDevice->m_pd3dDevice->SetTexture(0,rnaxDevice->m_renderTargets[0]);
	else
       renderStateObject->SetTextureState(m_currentEffect->m_assignIndex[0],renderStateObject->m_intZero,rnaxDevice->m_pd3dDevice,textureDataBase);

	if(m_currentEffect->m_assignIndex[1] == -5)
	   err = rnaxDevice->m_pd3dDevice->SetTexture(1,rnaxDevice->m_renderTargets[0]);
	else
       renderStateObject->SetTextureState(m_currentEffect->m_assignIndex[1],renderStateObject->m_int1,rnaxDevice->m_pd3dDevice,textureDataBase);

	if(m_currentEffect->m_assignIndex[2] == -5)
	   rnaxDevice->m_pd3dDevice->SetTexture(2,rnaxDevice->m_renderTargets[0]);
	else
       renderStateObject->SetTextureState(m_currentEffect->m_assignIndex[2],renderStateObject->m_int2,rnaxDevice->m_pd3dDevice,textureDataBase);

	if(m_currentEffect->m_assignIndex[3] == -5)
	   rnaxDevice->m_pd3dDevice->SetTexture(3,rnaxDevice->m_renderTargets[0]);
	else
       renderStateObject->SetTextureState(m_currentEffect->m_assignIndex[3],renderStateObject->m_int3,rnaxDevice->m_pd3dDevice,textureDataBase);

	if(m_currentEffect->m_assignIndex[4] == -5)
	   rnaxDevice->m_pd3dDevice->SetTexture(4,rnaxDevice->m_renderTargets[0]);
	else
       renderStateObject->SetTextureState(m_currentEffect->m_assignIndex[4],renderStateObject->m_int4,rnaxDevice->m_pd3dDevice,textureDataBase);

	if(m_currentEffect->m_assignIndex[5] == -5)
	   rnaxDevice->m_pd3dDevice->SetTexture(5,rnaxDevice->m_renderTargets[0]);
	else
       renderStateObject->SetTextureState(m_currentEffect->m_assignIndex[5],renderStateObject->m_int5,rnaxDevice->m_pd3dDevice,textureDataBase);

	if(m_currentEffect->m_assignIndex[5] == -5)
	   rnaxDevice->m_pd3dDevice->SetTexture(6,rnaxDevice->m_renderTargets[0]);
	else
       renderStateObject->SetTextureState(m_currentEffect->m_assignIndex[6],renderStateObject->m_int6,rnaxDevice->m_pd3dDevice,textureDataBase);

	if(m_currentEffect->m_assignIndex[5] == -5)
	   rnaxDevice->m_pd3dDevice->SetTexture(7,rnaxDevice->m_renderTargets[0]);
	else
       renderStateObject->SetTextureState(m_currentEffect->m_assignIndex[7],renderStateObject->m_int7,rnaxDevice->m_pd3dDevice,textureDataBase);


	renderStateObject->SetAlphaPixelTest(FALSE,rnaxDevice->m_pd3dDevice);
	renderStateObject->SetZWritableState(FALSE,rnaxDevice->m_pd3dDevice);
	rnaxDevice->m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,FALSE );
	renderStateObject->SetFogState(rnaxDevice->m_pd3dDevice,FALSE);
	
	renderStateObject->SetWorldMatrix((D3DXMATRIX*)&m_matrixInit2,rnaxDevice->m_pd3dDevice);
	renderStateObject->SetLightingMode(FALSE,rnaxDevice->m_pd3dDevice);
	renderStateObject->SetShader(rnaxDevice->m_pd3dDevice,renderStateObject->m_shderSystemRef,-1);
	renderStateObject->SetPxShader(rnaxDevice->m_pd3dDevice,renderStateObject->m_pxShderSystemRef,m_currentEffect->m_material->m_pixelShader);
	
	HRESULT err1 = rnaxDevice->m_pd3dDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_TEX2 | D3DFVF_TEX3 );
	err1 = rnaxDevice->m_pd3dDevice->SetStreamSource( 0, rnaxDevice->m_renderTargetSquareVB, 0, sizeof(RENDERTARGETABVX) );
	if(renderStateObject->m_currentHSLeffect)
	{//snap shot render
	   HRESULT errHr = 0;
	   
	
	   if(m_currentEffect->m_material->m_peramCount == 0 || m_currentEffect->m_material->m_peramCount != renderStateObject->m_currrentEffectPeramCountCRC)
          m_currentEffect->m_material->InitPeramBlockFXShadersHSL(renderStateObject->m_currentHSLeffect);
	   
	   int curTextureAssign = 0;
       for(int pLoop = 0;pLoop<m_currentEffect->m_material->m_peramCount;pLoop++)
	   {//set perams
          //debugging use only
	      //LPDIRECT3DBASETEXTURE9 ppTexture = NULL;
	      //D3DXPARAMETER_DESC localDescStruct;
          //renderStateObject->m_currentHSLeffect->GetParameterDesc(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop),&localDescStruct);


	      switch(m_currentEffect->m_material->m_peramBlock[pLoop].type)
		  {
		     case ARBP_UKNOWN://this will do nothing, because default setting are embedded

			 break;
			 case ARBP_Projection:
             errHr = renderStateObject->m_currentHSLeffect->SetMatrix(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), &renderStateObject->m_projMatrixUTP );
			 break;
			 case ARBP_WorldInverseTranspose:
			 //D3DXMatrixInverse(&renderStateObject->m_worldInverseTransposeRegister,NULL,&m_location);
	         //D3DXMatrixTranspose( &renderStateObject->m_worldInverseTransposeRegister, &renderStateObject->m_worldInverseTransposeRegister );
			 //errHr = renderStateObject->m_currentHSLeffect->SetMatrix(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), &renderStateObject->m_worldInverseTransposeRegister );
			 break;
			 case ARBP_WorldViewProjection:
			 //renderStateObject->m_worldViewProjectionMatrixRegister = m_location * renderStateObject->m_viewMatrixUTP * renderStateObject->m_projMatrixUTP;
             //errHr = renderStateObject->m_currentHSLeffect->SetMatrix(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), &renderStateObject->m_worldViewProjectionMatrixRegister );
			 break;
			 case ARBP_WorldView:
             //renderStateObject->m_worldViewMatrixRegister = m_location * renderStateObject->m_viewMatrixUTP;
			 //errHr = renderStateObject->m_currentHSLeffect->SetMatrix(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), &renderStateObject->m_worldViewMatrixRegister );
			 break;
			 case ARBP_World:
             //errHr = renderStateObject->m_currentHSLeffect->SetMatrix(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), &m_location );
			 break;
			 case ARBP_ViewInverse://known must update every frame type
			 errHr = renderStateObject->m_currentHSLeffect->SetMatrix(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), &renderStateObject->m_invViewMatrix );
			 break;
			 case ARBP_Time://known must update every frame type
             errHr = renderStateObject->m_currentHSLeffect->SetFloat(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), ((float)GetTickCount())*.001f ); 
			 break;
			 case ARBP_DirLight://known must update every frame type
			 errHr = renderStateObject->m_currentHSLeffect->SetValue(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), &renderStateObject->m_lastDirLight, m_currentEffect->m_material->m_peramBlock[pLoop].dataSize);
             //errHr = renderStateObject->m_currentHSLeffect->SetFloat(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), (float)(GetTickCount()/1000) ); 
			 break;
			 case ARBP_Diffuse://known must update every frame type
			 errHr = renderStateObject->m_currentHSLeffect->SetValue(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), &m_currentEffect->m_material->m_useMaterial.Diffuse, m_currentEffect->m_material->m_peramBlock[pLoop].dataSize);
             //errHr = renderStateObject->m_currentHSLeffect->SetFloat(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), (float)(GetTickCount()/1000) ); 
			 break;
			 case ARBP_Specular://known must update every frame type
			 errHr = renderStateObject->m_currentHSLeffect->SetValue(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), &m_currentEffect->m_material->m_useMaterial.Specular, m_currentEffect->m_material->m_peramBlock[pLoop].dataSize);
             //errHr = renderStateObject->m_currentHSLeffect->SetFloat(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), (float)(GetTickCount()/1000) ); 
			 break;
			 case ARBP_Emmisive://known must update every frame type
			 errHr = renderStateObject->m_currentHSLeffect->SetValue(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), &m_currentEffect->m_material->m_useMaterial.Emissive, m_currentEffect->m_material->m_peramBlock[pLoop].dataSize);
             //errHr = renderStateObject->m_currentHSLeffect->SetFloat(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), (float)(GetTickCount()/1000) ); 
			 break;
			 case ARBP_Ambient://known must update every frame type
			 errHr = renderStateObject->m_currentHSLeffect->SetValue(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), &m_currentEffect->m_material->m_useMaterial.Ambient, m_currentEffect->m_material->m_peramBlock[pLoop].dataSize);
             //errHr = renderStateObject->m_currentHSLeffect->SetFloat(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), (float)(GetTickCount()/1000) ); 
			 break;
			 case ARBP_SpecPower://known must update every frame type
			 errHr = renderStateObject->m_currentHSLeffect->SetValue(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), &m_currentEffect->m_material->m_useMaterial.Power, m_currentEffect->m_material->m_peramBlock[pLoop].dataSize);
             //errHr = renderStateObject->m_currentHSLeffect->SetFloat(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), (float)(GetTickCount()/1000) ); 
			 break;
			 case ARBP_Texture:
               /*errHr = renderStateObject->m_currentHSLeffect->GetTexture(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop),&ppTexture);
               if(errHr == S_OK)
			   if(ppTexture)
			      break;*/

             if(m_currentEffect->m_assignIndex[curTextureAssign] < textureDataBase->directAccessObjectCount)
			 {
			    if(m_currentEffect->m_assignIndex[curTextureAssign] > -1)
				{    
					errHr = renderStateObject->m_currentHSLeffect->SetTexture(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop),textureDataBase->directAccess[m_currentEffect->m_assignIndex[curTextureAssign]].object->m_textureObject->GetSurface());
				}
				else if(m_currentEffect->m_assignIndex[curTextureAssign] == -5)
				{
				    errHr = renderStateObject->m_currentHSLeffect->SetTexture(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop),rnaxDevice->m_renderTargets[0]);
				}
				curTextureAssign++;
				
			 }
			 break;
			 case ARBP_VolumeTexturePrcdrl:
		     {//volume texture proc handle
				 
                  if(renderStateObject->m_currentPxFxShader->m_shaderUsedOnce == 0)
				  {//needs initialization for proceedural info
				    LPD3DXTEXTURESHADER pTextureShader = NULL;
					LPD3DXBUFFER pFunction = NULL;
					D3DXPARAMETER_DESC localDescStruct;
					D3DXHANDLE hAnnot;
				    D3DXPARAMETER_DESC AnnotDesc;
				    int Width,Height,Depth;
				    LPCSTR textureType,pstrFunction,pstrTarget,pstrName;
					textureType = pstrFunction = pstrTarget = pstrName = NULL;

                    if(renderStateObject->m_currentHSLeffect->GetParameterDesc(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop),&localDescStruct)!=S_OK)
		               break;

					if(localDescStruct.Annotations > 0)
				      for( UINT iAnnot = 0; iAnnot < localDescStruct.Annotations; iAnnot++ )
                      {
                      hAnnot = renderStateObject->m_currentHSLeffect->GetAnnotation ( renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), iAnnot );
                      renderStateObject->m_currentHSLeffect->GetParameterDesc( hAnnot, &AnnotDesc );
                      if( _strcmpi( AnnotDesc.Name, "name" ) == 0 )
                          renderStateObject->m_currentHSLeffect->GetString( hAnnot, &pstrName );
                      else if ( _strcmpi( AnnotDesc.Name, "function" ) == 0 )
                          renderStateObject->m_currentHSLeffect->GetString( hAnnot, &pstrFunction );
                      else if ( _strcmpi( AnnotDesc.Name, "target" ) == 0 )
                          renderStateObject->m_currentHSLeffect->GetString( hAnnot, &pstrTarget );
                      else if ( _strcmpi( AnnotDesc.Name, "width" ) == 0 )
                          renderStateObject->m_currentHSLeffect->GetInt( hAnnot, &Width );
                      else if ( _strcmpi( AnnotDesc.Name, "height" ) == 0 )
                          renderStateObject->m_currentHSLeffect->GetInt( hAnnot, &Height );
                      else if ( _strcmpi( AnnotDesc.Name, "depth" ) == 0 )
                          renderStateObject->m_currentHSLeffect->GetInt( hAnnot, &Depth );
                      else if( _strcmpi( AnnotDesc.Name, "type" ) == 0 )
                          renderStateObject->m_currentHSLeffect->GetString( hAnnot, &textureType );
					  }

					 HRESULT hr = 0;
                     
                     if( pstrTarget == NULL )
                         pstrTarget = "tx_1_0";

                     LPD3DXBUFFER pBufferErrors = NULL;
                     if( SUCCEEDED( hr = D3DXCompileShader( renderStateObject->currentHSLcode, (UINT)renderStateObject->currentHSLcode.GetLength(), NULL, 
                                    NULL, pstrFunction, pstrTarget, 0, &pFunction, &pBufferErrors, NULL ) ) )
                     {//compile tx shder
                         if( SUCCEEDED( hr = D3DXCreateTextureShader((DWORD *)pFunction->GetBufferPointer(), &pTextureShader) ) )
                         {//create texture shader
                             pTextureShader->SetDefaults();
                             if( Width == D3DX_DEFAULT )
                                 Width = 64;
                             if( Height == D3DX_DEFAULT )
                                 Height = 64;
                             if( Depth == D3DX_DEFAULT )
                                 Depth = 64;

                            LPDIRECT3DVOLUMETEXTURE9 pVolumeTex = NULL;
                            if( SUCCEEDED( hr = D3DXCreateVolumeTexture( rnaxDevice->m_pd3dDevice, 
                                Width, Height, Depth, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pVolumeTex) ) )
                            {
								
                                if( SUCCEEDED( hr = D3DXFillVolumeTextureTX( pVolumeTex, pTextureShader ) ) ) 
                                    hr = renderStateObject->m_currentHSLeffect->SetTexture( renderStateObject->m_currentHSLeffect->GetParameter( NULL, pLoop ), pVolumeTex );
                            }
                     
                            
                            //SAFE_RELEASE(pTex);
                            pTextureShader->Release();
							pTextureShader = 0;
						 }//end create texture shader
                         pFunction->Release();
					 }//end compile tx shder
                     /*if( pBufferErrors != NULL )
                     {
                         strErrors += (CHAR*)pBufferErrors->GetBufferPointer();
                         pBufferErrors->Release();
                     }*/
				  }//end needs initialization for proceedural info 
			 }//end volume texture proc handle
			 break;
			 case ARBP_WorldInverse:
             //D3DXMatrixInverse(&renderStateObject->m_worldInverseTransposeRegister,NULL,&m_location);
			 //errHr = renderStateObject->m_currentHSLeffect->SetMatrix(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop), &renderStateObject->m_worldInverseTransposeRegister );
			 break;
			 case ARBP_USEDEFAULT:
	         errHr = renderStateObject->m_currentHSLeffect->SetValue(renderStateObject->m_currentHSLeffect->GetParameter(NULL,pLoop),
				                                                m_currentEffect->m_material->m_peramBlock[pLoop].data, 
														        m_currentEffect->m_material->m_peramBlock[pLoop].dataSize);
			 break;
		  }
	   }//end set perams
	   //end set perams

	   //renderStateObject->SetVertexType(m_meshReference->m_abVertexArray.m_uvCount,g_pd3dDevice);
	   
	   renderStateObject->m_currentPxFxShader->m_shaderUsedOnce = 1;
	   
	   //Apply the technique contained in the effect (note: technique set in pixel state managment)
		UINT cPasses = 0;
        if(renderStateObject->m_currentHSLeffect->Begin(&cPasses, 0)==S_OK)
		{
          for (UINT iPass = 0; iPass < cPasses; iPass++)
          {
            errHr = renderStateObject->m_currentHSLeffect->BeginPass(iPass);

            //The effect interface queues up the changes and performs them 
            //with the CommitChanges call. You do not need to call CommitChanges if 
            //you are not setting any parameters between the BeginPass and EndPass.
            //V( g_pEffect->CommitChanges() );

			err1 = rnaxDevice->m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
			//totalIndecieCount += m_meshReference->Render(g_pd3dDevice,renderStateObject->m_currentVertexType,m_meshReference->m_chromeUvSet);
	        //objectsRendered++;

            //Render the mesh with the applied technique

			errHr = renderStateObject->m_currentHSLeffect->EndPass();
          }
          errHr = renderStateObject->m_currentHSLeffect->End();
		}
	}//end snap shot render
	else
	{
	err1 = rnaxDevice->m_pd3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );

	}
	renderStateObject->m_currentVertexType = 99;
	renderStateObject->SetLightingMode(TRUE,rnaxDevice->m_pd3dDevice);
	rnaxDevice->m_pd3dDevice->SetRenderState( D3DRS_ZENABLE,          TRUE );
	//Unset texture layers that were assigned via render target
	if(m_currentEffect->m_assignIndex[0] == -5)
       renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_intZero,rnaxDevice->m_pd3dDevice,textureDataBase);
	if(m_currentEffect->m_assignIndex[1] == -5)
       renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int1,rnaxDevice->m_pd3dDevice,textureDataBase);
	if(m_currentEffect->m_assignIndex[2] == -5)
       renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int2,rnaxDevice->m_pd3dDevice,textureDataBase);
	if(m_currentEffect->m_assignIndex[3] == -5)
       renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int3,rnaxDevice->m_pd3dDevice,textureDataBase);
	if(m_currentEffect->m_assignIndex[4] == -5)
       renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int4,rnaxDevice->m_pd3dDevice,textureDataBase);
	if(m_currentEffect->m_assignIndex[5] == -5)
       renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int5,rnaxDevice->m_pd3dDevice,textureDataBase);
	if(m_currentEffect->m_assignIndex[5] == -5)
       renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int6,rnaxDevice->m_pd3dDevice,textureDataBase);
	if(m_currentEffect->m_assignIndex[5] == -5)
       renderStateObject->SetTextureState(renderStateObject->m_intNegOne,renderStateObject->m_int7,rnaxDevice->m_pd3dDevice,textureDataBase);


	renderStateObject->SetPxShader(rnaxDevice->m_pd3dDevice,renderStateObject->m_pxShderSystemRef,-1);
	renderStateObject->SetShader(rnaxDevice->m_pd3dDevice,renderStateObject->m_shderSystemRef,-1);
	//end scene
	//rnaxDevice->m_pd3dDevice->EndScene();
}

bool CPostProcessModuleManager::EnableRenderTarget(DxDeviceClass * rnaxDevice)
{
	   if(!m_currentEffect)
           return false;
	   if(rnaxDevice->m_pd3dDevice->SetRenderTarget(0,rnaxDevice->m_renderTargetsSurfaces[0])==S_OK)
	   {
           return true;
	   }
	   return false;
}

void CPostProcessModuleManager::RestorePrimaryRenderTarget(DxDeviceClass * rnaxDevice)
{
	   if(!m_currentEffect)
           return;
	   HRESULT err = rnaxDevice->m_pd3dDevice->SetRenderTarget(0,m_primaryRenderTarget);
	   if(m_primaryRenderTarget)
	      m_primaryRenderTarget->Release();
	      m_primaryRenderTarget = 0;
}

void CPostProcessModuleManager::SetEffect(int effectIndex)
{
     this->m_indexOfCurrentEffect = effectIndex;
}

void CPostProcessModuleManager::GetPrimaryRenderTarget(DxDeviceClass * rnaxDevice)
{//called first
    CPostProcessModule * postEffect = GetByIndex(m_indexOfCurrentEffect);
	if(!postEffect)
	{
        m_currentEffect = NULL;
		return;
	}
	m_currentEffect = postEffect;
	rnaxDevice->m_pd3dDevice->GetRenderTarget( 0, &m_primaryRenderTarget );
}


void CPostProcessModule::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){

	   ar << m_processName << m_material;

	   for(int loop=0;loop<25;loop++)
	   {
           ar << m_textureAssignName[loop]
		      << m_assignIndex[loop];
	   }

	}
	else
	{
       ar >> m_processName >> m_material;

	   for(int loop=0;loop<25;loop++)
	   {
           ar >> m_textureAssignName[loop]
		      >> m_assignIndex[loop];
	   }

	   
	}
}     



IMPLEMENT_SERIAL(CPostProcessModule, CObject, 0)
IMPLEMENT_SERIAL(CPostProcessModuleManager, CObList, 0)


