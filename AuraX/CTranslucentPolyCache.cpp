#include "stdafx.h"
#define D3D_OVERLOADS

#include "d3d9.h"
#include <afxtempl.h>
#include "CTranslucentPolyCache.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

 void CTranslucentPolyList::CacheMesh(CEXMeshObj * mesh,
											EnvParticleSystem *particleSysOptionRef,
									        D3DMATRIX  & viewMatrix,
									        D3DMATRIX  & wldMatrix,
											D3DVECTOR  & meshCenter,
											CReferenceObj * refOpt)
{

        InsertPolygonOrMeshRef(NULL,
					           viewMatrix,
					           wldMatrix,
							   mesh,
							   meshCenter,
							   particleSysOptionRef,
							   refOpt
					           );
}
void CTranslucentPolyList::CacheMeshNoSort(CEXMeshObj * mesh,
											EnvParticleSystem *particleSysOptionRef,
									        D3DMATRIX  & viewMatrix,
									        D3DMATRIX  & wldMatrix,
											D3DVECTOR  & meshCenter,
											CReferenceObj * refOpt)
{

        InsertPolygonOrMeshRefNonTrans(NULL,
					           viewMatrix,
					           wldMatrix,
							   mesh,
							   meshCenter,
							   particleSysOptionRef,
							   refOpt
					           );
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
CTranslucentPolyList::CTranslucentPolyList()
{
	m_currentObjectCount	= 0;
	m_lastPosition			= 0;
	m_lastValue				= 0;
	m_alphaMode				= FALSE;
	m_indexDensity			= 5;
	m_validRange			= 4000;
	m_validCount			= m_validRange/m_indexDensity;
	m_currentIndexTemp		= 0;
	m_allocTracer			= 0;


}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
 void CTranslucentPolyObj::Render(   LPDIRECT3DDEVICE9   g_pd3dDevice,
										   BOOL                &mirrorEnable,
										   D3DMATRIX           &mirrorMatrix,
										   D3DVECTOR           &locationVect,
										   CStateManagementObj * stateManager,
										   CTextureDBObjList   * textureDataBase,
										   float              & gl_adjust,
										   int                 & totalIndecieCount,
										   int                 & objectsRendered,
										   D3DVECTOR           & camLocation,
										   D3DMATRIX           & matrixLoc, 
										   int                 & particleCalculationCount,
										   CEXMeshObj          ** lastMeshREendered,
										   BOOL                & alphaMode,
										   CRTLightObjList     * m_lightDatabase)
{
    if(this->m_particleSysOption)
	{//particle option
		    this->m_particleSysOption->VSPRender(stateManager,g_pd3dDevice,locationVect,m_location,textureDataBase);
			particleCalculationCount += this->m_particleSysOption->m_NumParticles;
		    *lastMeshREendered = NULL;
	}//end particle option
	
	if(!m_meshReference)
		return;
	
	if(m_meshReference->m_visibilityTrack <= 0.01f)//do not render this
		return;

	char lPinit = 0;
	if(m_referenceOption)
	if(m_referenceOption->m_extLockPoints)
	{
       float propfactor = m_materialRef.Diffuse.a/1.0f;
       float StageVec = 255.0f + propfactor * (84.0f - 255.0f);
	   g_pd3dDevice->SetRenderState(D3DRS_ALPHAREF, DWORD(StageVec));
	   lPinit = 1;
	}

    //this->m_meshReference->m_meshCenter
	D3DVECTOR tempCenterWS;
	tempCenterWS.x = m_location._41 + this->m_meshReference->m_meshCenter.x;
	tempCenterWS.y = m_location._42 + this->m_meshReference->m_meshCenter.y;
	tempCenterWS.z = m_location._43 + this->m_meshReference->m_meshCenter.z;
    
	
    int shaderMode = 0;
	if(m_meshReference->List_materialObject->m_vertexShader > -1 || m_meshReference->List_materialObject->m_pixelShader > -1)
	   shaderMode = 1;

    if(m_lightDatabase)
	m_lightDatabase->EnableLightingPerObject(g_pd3dDevice,
											 locationVect,
											 stateManager,
											 tempCenterWS,
                                             this->m_meshReference->m_meshBoundingSphereRadius,
											 shaderMode);


    
	m_location._41 = m_location._41 - locationVect.x;
	m_location._42 = m_location._42 - locationVect.y;
	m_location._43 = m_location._43 - locationVect.z;
	 

	if(m_blendRef != -1)
	if(m_meshReference->m_visibilityTrack < 1.0f)
	{//we know if this case validates that is has additional visibility track on the object that will need to be compounded with the current material in mind
       //unless this is amplified blend we must be in alpha mode
       //m_materialRef
	   //m_materialRef.Diffuse.a //alpha controller on material
	   //for amplified blend we must darken to black overall material
	   //m_blendRef -  can be changed without effecting original 1=alpha 2=amplified
	   if(m_blendRef == 1)
	   {//alpha based
	      m_blendRef = 1;
          m_materialRef.Diffuse.a = m_materialRef.Diffuse.a * m_meshReference->m_visibilityTrack;//apply visibility modifier
	   }
	   else
	   {
          m_materialRef.Diffuse.r = m_materialRef.Diffuse.r * m_meshReference->m_visibilityTrack;
		  m_materialRef.Diffuse.g = m_materialRef.Diffuse.g * m_meshReference->m_visibilityTrack;
		  m_materialRef.Diffuse.b = m_materialRef.Diffuse.b * m_meshReference->m_visibilityTrack;

		  m_materialRef.Emissive.r = m_materialRef.Emissive.r * m_meshReference->m_visibilityTrack;
		  m_materialRef.Emissive.g = m_materialRef.Emissive.g * m_meshReference->m_visibilityTrack;
		  m_materialRef.Emissive.b = m_materialRef.Emissive.b * m_meshReference->m_visibilityTrack;
	   }
	}//end we know if this case validates that is has additional visibility track on the object that will need to be compounded with the current material in mind

	if(*lastMeshREendered != m_meshReference || stateManager->m_currentHSLeffect != NULL)
	{//new mesh type  ARBP_WorldInverse



    stateManager->SetShader(g_pd3dDevice,stateManager->m_shderSystemRef,m_meshReference->List_materialObject->m_vertexShader);
	stateManager->SetPxShader(g_pd3dDevice,stateManager->m_pxShderSystemRef,m_meshReference->List_materialObject->m_pixelShader);

	if(stateManager->m_currentHSLeffect)
	{//snap shot render
	   HRESULT errHr = 0;
	
     

	   if(m_meshReference->List_materialObject->m_peramCount == 0 || m_meshReference->List_materialObject->m_peramCount != stateManager->m_currrentEffectPeramCountCRC)
          m_meshReference->List_materialObject->InitPeramBlockFXShadersHSL(stateManager->m_currentHSLeffect);

	   
	   int curTextureAssign = 0;
       for(int pLoop = 0;pLoop<m_meshReference->List_materialObject->m_peramCount;pLoop++)
	   {//set perams
          //debugging use only
	      //LPDIRECT3DBASETEXTURE9 ppTexture = NULL;
	      //D3DXPARAMETER_DESC localDescStruct;
          //stateManager->m_currentHSLeffect->GetParameterDesc(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop),&localDescStruct);


	      switch(m_meshReference->List_materialObject->m_peramBlock[pLoop].type)
		  {
		     case ARBP_UKNOWN://this will do nothing, because default setting are embedded

			 break;
			 case ARBP_Projection:
             errHr = stateManager->m_currentHSLeffect->SetMatrix(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), &stateManager->m_projMatrixUTP );
			 break;
			 case ARBP_WorldInverseTranspose:
			 D3DXMatrixInverse(&stateManager->m_worldInverseTransposeRegister,NULL,&m_location);
	         D3DXMatrixTranspose( &stateManager->m_worldInverseTransposeRegister, &stateManager->m_worldInverseTransposeRegister );
			 errHr = stateManager->m_currentHSLeffect->SetMatrix(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), &stateManager->m_worldInverseTransposeRegister );
			 break;
			 case ARBP_WorldViewProjection:
			 stateManager->m_worldViewProjectionMatrixRegister = m_location * stateManager->m_viewMatrixUTP * stateManager->m_projMatrixUTP;
             errHr = stateManager->m_currentHSLeffect->SetMatrix(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), &stateManager->m_worldViewProjectionMatrixRegister );
			 break;
			 case ARBP_WorldView:
             stateManager->m_worldViewMatrixRegister = m_location * stateManager->m_viewMatrixUTP;
			 errHr = stateManager->m_currentHSLeffect->SetMatrix(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), &stateManager->m_worldViewMatrixRegister );
			 break;
			 case ARBP_World:
             errHr = stateManager->m_currentHSLeffect->SetMatrix(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), &m_location );
			 break;
			 case ARBP_ViewInverse://known must update every frame type
			 errHr = stateManager->m_currentHSLeffect->SetMatrix(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), &stateManager->m_invViewMatrix );
			 break;
			 case ARBP_Time://known must update every frame type
             errHr = stateManager->m_currentHSLeffect->SetFloat(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), ((float)GetTickCount())*.001f ); 
			 break;
			 case ARBP_DirLight://known must update every frame type
			 errHr = stateManager->m_currentHSLeffect->SetValue(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), &stateManager->m_lastDirLight, m_meshReference->List_materialObject->m_peramBlock[pLoop].dataSize);
             //errHr = stateManager->m_currentHSLeffect->SetFloat(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), (float)(GetTickCount()/1000) ); 
			 break;
			 case ARBP_Diffuse://known must update every frame type
			 errHr = stateManager->m_currentHSLeffect->SetValue(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), &m_meshReference->List_materialObject->m_useMaterial.Diffuse, m_meshReference->List_materialObject->m_peramBlock[pLoop].dataSize);
             //errHr = stateManager->m_currentHSLeffect->SetFloat(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), (float)(GetTickCount()/1000) ); 
			 break;
			 case ARBP_Specular://known must update every frame type
			 errHr = stateManager->m_currentHSLeffect->SetValue(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), &m_meshReference->List_materialObject->m_useMaterial.Specular, m_meshReference->List_materialObject->m_peramBlock[pLoop].dataSize);
             //errHr = stateManager->m_currentHSLeffect->SetFloat(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), (float)(GetTickCount()/1000) ); 
			 break;
			 case ARBP_Emmisive://known must update every frame type
			 errHr = stateManager->m_currentHSLeffect->SetValue(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), &m_meshReference->List_materialObject->m_useMaterial.Emissive, m_meshReference->List_materialObject->m_peramBlock[pLoop].dataSize);
             //errHr = stateManager->m_currentHSLeffect->SetFloat(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), (float)(GetTickCount()/1000) ); 
			 break;
			 case ARBP_Ambient://known must update every frame type
			 errHr = stateManager->m_currentHSLeffect->SetValue(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), &m_meshReference->List_materialObject->m_useMaterial.Ambient, m_meshReference->List_materialObject->m_peramBlock[pLoop].dataSize);
             //errHr = stateManager->m_currentHSLeffect->SetFloat(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), (float)(GetTickCount()/1000) ); 
			 break;
			 case ARBP_SpecPower://known must update every frame type
			 errHr = stateManager->m_currentHSLeffect->SetValue(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), &m_meshReference->List_materialObject->m_useMaterial.Power, m_meshReference->List_materialObject->m_peramBlock[pLoop].dataSize);
             //errHr = stateManager->m_currentHSLeffect->SetFloat(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), (float)(GetTickCount()/1000) ); 
			 break;
			 case ARBP_Texture:
               /*errHr = stateManager->m_currentHSLeffect->GetTexture(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop),&ppTexture);
               if(errHr == S_OK)
			   if(ppTexture)
			      break;*/

             if(m_meshReference->List_textureSelect[curTextureAssign] < textureDataBase->directAccessObjectCount)
			 {
			    if(m_meshReference->List_textureSelect[curTextureAssign] > -1)
				{
		        errHr = stateManager->m_currentHSLeffect->SetTexture(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop),textureDataBase->directAccess[m_meshReference->List_textureSelect[curTextureAssign]].object->m_textureObject->GetSurface());
	            //stateManager->GlobalTextureIndex[curTextureAssign] = m_meshReference->List_textureSelect[curTextureAssign];
				}
				else
				{
				//errHr = stateManager->m_currentHSLeffect->SetTexture(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop),NULL);
                //stateManager->GlobalTextureIndex[curTextureAssign] = m_meshReference->List_textureSelect[curTextureAssign];
				}
				curTextureAssign++;
				
			 }
			 break;
			 case ARBP_VolumeTexturePrcdrl:
		     {//volume texture proc handle
				 
                  if(stateManager->m_currentPxFxShader->m_shaderUsedOnce == 0)
				  {//needs initialization for proceedural info
				    LPD3DXTEXTURESHADER pTextureShader = NULL;
					LPD3DXBUFFER pFunction = NULL;
					D3DXPARAMETER_DESC localDescStruct;
					D3DXHANDLE hAnnot;
				    D3DXPARAMETER_DESC AnnotDesc;
				    int Width,Height,Depth;
				    LPCSTR textureType,pstrFunction,pstrTarget,pstrName;
					textureType = pstrFunction = pstrTarget = pstrName = NULL;

                    if(stateManager->m_currentHSLeffect->GetParameterDesc(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop),&localDescStruct)!=S_OK)
		               break;

					if(localDescStruct.Annotations > 0)
				      for( UINT iAnnot = 0; iAnnot < localDescStruct.Annotations; iAnnot++ )
                      {
                      hAnnot = stateManager->m_currentHSLeffect->GetAnnotation ( stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), iAnnot );
                      stateManager->m_currentHSLeffect->GetParameterDesc( hAnnot, &AnnotDesc );
                      if( _strcmpi( AnnotDesc.Name, "name" ) == 0 )
                          stateManager->m_currentHSLeffect->GetString( hAnnot, &pstrName );
                      else if ( _strcmpi( AnnotDesc.Name, "function" ) == 0 )
                          stateManager->m_currentHSLeffect->GetString( hAnnot, &pstrFunction );
                      else if ( _strcmpi( AnnotDesc.Name, "target" ) == 0 )
                          stateManager->m_currentHSLeffect->GetString( hAnnot, &pstrTarget );
                      else if ( _strcmpi( AnnotDesc.Name, "width" ) == 0 )
                          stateManager->m_currentHSLeffect->GetInt( hAnnot, &Width );
                      else if ( _strcmpi( AnnotDesc.Name, "height" ) == 0 )
                          stateManager->m_currentHSLeffect->GetInt( hAnnot, &Height );
                      else if ( _strcmpi( AnnotDesc.Name, "depth" ) == 0 )
                          stateManager->m_currentHSLeffect->GetInt( hAnnot, &Depth );
                      else if( _strcmpi( AnnotDesc.Name, "type" ) == 0 )
                          stateManager->m_currentHSLeffect->GetString( hAnnot, &textureType );
					  }

					 HRESULT hr = 0;
                     
                     if( pstrTarget == NULL )
                         pstrTarget = "tx_1_0";

                     LPD3DXBUFFER pBufferErrors = NULL;
                     if( SUCCEEDED( hr = D3DXCompileShader( stateManager->currentHSLcode, (UINT)stateManager->currentHSLcode.GetLength(), NULL, 
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
                            if( SUCCEEDED( hr = D3DXCreateVolumeTexture( g_pd3dDevice, 
                                Width, Height, Depth, 1, 0, D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &pVolumeTex) ) )
                            {
								
                                if( SUCCEEDED( hr = D3DXFillVolumeTextureTX( pVolumeTex, pTextureShader ) ) ) 
                                    hr = stateManager->m_currentHSLeffect->SetTexture( stateManager->m_currentHSLeffect->GetParameter( NULL, pLoop ), pVolumeTex );
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
             D3DXMatrixInverse(&stateManager->m_worldInverseTransposeRegister,NULL,&m_location);
			 errHr = stateManager->m_currentHSLeffect->SetMatrix(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop), &stateManager->m_worldInverseTransposeRegister );
			 break;
			 case ARBP_USEDEFAULT:
	         errHr = stateManager->m_currentHSLeffect->SetValue(stateManager->m_currentHSLeffect->GetParameter(NULL,pLoop),
				                                                m_meshReference->List_materialObject->m_peramBlock[pLoop].data, 
														        m_meshReference->List_materialObject->m_peramBlock[pLoop].dataSize);
			 break;
		  }
	   }//end set perams
	   //end set perams

	   stateManager->SetVertexType(m_meshReference->m_abVertexArray.m_uvCount,g_pd3dDevice);
	   
	   stateManager->m_currentPxFxShader->m_shaderUsedOnce = 1;
	   
	   //Apply the technique contained in the effect (note: technique set in pixel state managment)
		UINT cPasses = 0;
        if(stateManager->m_currentHSLeffect->Begin(&cPasses, 0)==S_OK)
		{
          for (UINT iPass = 0; iPass < cPasses; iPass++)
          {
            errHr = stateManager->m_currentHSLeffect->BeginPass(iPass);

            //The effect interface queues up the changes and performs them 
            //with the CommitChanges call. You do not need to call CommitChanges if 
            //you are not setting any parameters between the BeginPass and EndPass.
            //V( g_pEffect->CommitChanges() );
			totalIndecieCount += m_meshReference->Render(g_pd3dDevice,stateManager->m_currentVertexType,m_meshReference->m_chromeUvSet);
	        objectsRendered++;

            //Render the mesh with the applied technique

			errHr = stateManager->m_currentHSLeffect->EndPass();
          }
          errHr = stateManager->m_currentHSLeffect->End();
		}

       return;
	}//end snap shot render

    if(m_meshReference->List_materialObject)
	 for(int loop=0;loop<7;loop++)
     {
	    stateManager->SetColorArg1Level(loop,m_meshReference->List_materialObject->m_colorArg1[loop],g_pd3dDevice);
        stateManager->SetColorArg2Level(loop,m_meshReference->List_materialObject->m_colorArg2[loop],g_pd3dDevice);
		stateManager->SetTexcoordIndexLevel(loop,m_meshReference->List_materialObject->m_texcoordLevels[loop],g_pd3dDevice);
     } 

	 


	stateManager->SetFogFilter(g_pd3dDevice,m_meshReference->List_materialObject->m_fogFilter);
	stateManager->SetFogOverrideState(m_meshReference->List_materialObject->m_fogOverride,g_pd3dDevice);
	stateManager->SetBlendType(g_pd3dDevice,m_blendRef);

	stateManager->SetCullState(m_meshReference->List_materialObject->m_cullOn,g_pd3dDevice);
	stateManager->SetWireFillState(m_meshReference->List_materialObject->m_forceWireFrame,g_pd3dDevice);
	stateManager->SetTextureTileMode(m_meshReference->List_materialObject->m_textureTileType,g_pd3dDevice);
	stateManager->SetMaterial(g_pd3dDevice,&m_materialRef);
	//g_pd3dDevice->SetMaterial(&m_materialRef);
	stateManager->SetAlphaPixelTest(m_meshReference->List_materialObject->m_doAlphaDepthTest,g_pd3dDevice);

	//set textures and multitexture states
	stateManager->SetTextureState(m_meshReference->List_textureSelect[0],stateManager->m_intZero,g_pd3dDevice,textureDataBase);
	if(m_meshReference->List_BlendMethod[9]<0)
	   stateManager->SetBlendLevel(stateManager->m_intZero,stateManager->m_intZero,g_pd3dDevice);
	else
	   stateManager->SetBlendLevel(stateManager->m_intZero,m_meshReference->List_BlendMethod[9],g_pd3dDevice);

	for(int loop=1;loop<stateManager->m_multiTextureSupported;loop++){
        stateManager->SetTextureState(m_meshReference->List_textureSelect[loop],loop,g_pd3dDevice,textureDataBase);	
		stateManager->SetBlendLevel(loop,m_meshReference->List_BlendMethod[(loop-1)],g_pd3dDevice);
		if(loop>7)
		   break;
	}
	
	//render 

    stateManager->SetDiffuseSource(m_meshReference->m_abVertexArray.m_advancedFixedMode,g_pd3dDevice);
	stateManager->SetVertexType(m_meshReference->m_abVertexArray.m_uvCount,g_pd3dDevice);
	m_meshReference->TextureMovementCallback(gl_adjust,stateManager->m_currentVertexType);
	
	stateManager->SetWorldMatrix((D3DXMATRIX*)&m_location,g_pd3dDevice);
    //constant block
    if(m_meshReference->m_constantBlock)
	{//optional shader constant block
       for(int cLoop=0;cLoop<m_meshReference->m_constantBlock->constantCount;cLoop++)
	   {
           switch(m_meshReference->m_constantBlock->constantArray[cLoop].type)
		   {
		       case 0: //float
                  g_pd3dDevice->SetVertexShaderConstantF( m_meshReference->m_constantBlock->constantArray[cLoop].registerIndex, (float*)&m_meshReference->m_constantBlock->constantArray[cLoop].register2, 1 );
			   break;
			   case 1: //integer
                  g_pd3dDevice->SetVertexShaderConstantI( m_meshReference->m_constantBlock->constantArray[cLoop].registerIndex, (int*)&m_meshReference->m_constantBlock->constantArray[cLoop].register1, 1 );
			   break;
			   case 2: //time
                  if(1)
				  {//alloc trick
				     DWORD tempDwd = GetTickCount();
	                 tempDwd = tempDwd-m_meshReference->m_constantBlock->constantArray[cLoop].timeStamp;
                     m_meshReference->m_constantBlock->constantArray[cLoop].timeStamp += (tempDwd/(m_meshReference->m_constantBlock->constantArray[cLoop].duration+1))*(m_meshReference->m_constantBlock->constantArray[cLoop].duration+1);//floored offset
                     //get single cycle position
					 tempDwd = tempDwd-m_meshReference->m_constantBlock->constantArray[cLoop].timeStamp;
					 //0.0-1.0 scales position in cycle
                     m_meshReference->m_constantBlock->constantArray[cLoop].register2[0] = (float)tempDwd/(float)m_meshReference->m_constantBlock->constantArray[cLoop].duration;
                    
					 g_pd3dDevice->SetVertexShaderConstantF( m_meshReference->m_constantBlock->constantArray[cLoop].registerIndex, (float*)&m_meshReference->m_constantBlock->constantArray[cLoop].register2, 1 );
				  }//end alloc trick
		       break;
			   case 3: //time capped
                  if(1)
				  {//alloc trick
				     DWORD tempDwd = GetTickCount();
	                 tempDwd = tempDwd-m_meshReference->m_constantBlock->constantArray[cLoop].timeStamp;
					 if(tempDwd > m_meshReference->m_constantBlock->constantArray[cLoop].duration)
					    tempDwd = m_meshReference->m_constantBlock->constantArray[cLoop].duration;//cap it at max
					 //0.0-1.0 scales position in cycle
                     m_meshReference->m_constantBlock->constantArray[cLoop].register2[0] = (float)tempDwd/(float)m_meshReference->m_constantBlock->constantArray[cLoop].duration;
                    
					 g_pd3dDevice->SetVertexShaderConstantF( m_meshReference->m_constantBlock->constantArray[cLoop].registerIndex, (float*)&m_meshReference->m_constantBlock->constantArray[cLoop].register2, 1 );
				  }//end alloc trick
		       break;
		   }
	   }
	}//end optional shader constant block
    
	totalIndecieCount += m_meshReference->Render(g_pd3dDevice,stateManager->m_currentVertexType,m_meshReference->m_chromeUvSet);
	objectsRendered++;
    *lastMeshREendered = m_meshReference;
	}//end new mesh type
	else{//same mesh type

		stateManager->SetWorldMatrix((D3DXMATRIX*)&m_location,g_pd3dDevice);
		stateManager->SetMaterial(g_pd3dDevice,&m_materialRef);
        totalIndecieCount += m_meshReference->RenderFast(g_pd3dDevice,stateManager->m_currentVertexType,m_meshReference->m_chromeUvSet);

	}//end same mesh type


	
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//--this functiongets the distance between 2 3dpoints----------//
float CTranslucentPolyObj::MagnitudeSquared(float &Mx1,float &My1,
								         float &Mz1,float &Mx2,
								         float &My2,float &Mz2){
float temp_Mxyz = (((Mx1 - Mx2) * (Mx1 - Mx2)) + ((My1 - My2) * (My1 - My2)) + ((Mz1 - Mz2) * (Mz1 - Mz2)));

return (float)sqrt(temp_Mxyz);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
 void CTranslucentPolyList::Render(   LPDIRECT3DDEVICE9    g_pd3dDevice,
											BOOL                &mirrorEnable,
										    D3DMATRIX           &mirrorMatrix,
											D3DVECTOR           &locationVect,
											CStateManagementObj * stateManager,
											CTextureDBObjList   * textureDataBase,
											double               & gl_adjust,
											int                 & totalIndecieCount,
											int                 & objectsRendered,
											BOOL                  frontToBack,
											D3DVECTOR           & camLocation,
						                    D3DMATRIX           & matrixLoc, 
											int                 & particleCalculationCount,
											CRTLightObjList     * m_lightDatabase)
{
    m_lastMeshREendered = NULL;

	float glAdjstFloat = (float)gl_adjust;

	if(frontToBack)
	{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CTranslucentPolyObj * pObj = (CTranslucentPolyObj *)GetNext(posLoc);
	    pObj->Render(g_pd3dDevice,
					 mirrorEnable,
					 mirrorMatrix,
					 locationVect,
					 stateManager,
					 textureDataBase,
					 glAdjstFloat,
					 totalIndecieCount,
					 objectsRendered,
					 camLocation,
					 matrixLoc,
					 particleCalculationCount,
					 &m_lastMeshREendered,
					 m_alphaMode,
					 m_lightDatabase);
	 }
	}
	else{
     for(POSITION posLoc = GetTailPosition();posLoc != NULL;){
        CTranslucentPolyObj * pObj = (CTranslucentPolyObj *)GetPrev(posLoc);
	    pObj->Render(g_pd3dDevice,
					 mirrorEnable,
					 mirrorMatrix,
					 locationVect,
					 stateManager,
					 textureDataBase,
					 glAdjstFloat,
					 totalIndecieCount,
					 objectsRendered,
					 camLocation,
					 matrixLoc,
					 particleCalculationCount,
					 &m_lastMeshREendered,
					 m_alphaMode,
					 m_lightDatabase);
	 }
	}
	//render method index 2
    if(frontToBack)
	{
	 int inLoop=0;
	 POSITION posLoc;
	 for(inLoop=0;inLoop<m_validCount;inLoop++)
      for(posLoc = m_indexedDBAccess[inLoop].m_list.GetHeadPosition();posLoc != NULL;){
          CTranslucentPolyObj * pObj = (CTranslucentPolyObj *)m_indexedDBAccess[inLoop].m_list.GetNext(posLoc);
	      pObj->Render(  g_pd3dDevice,
						 mirrorEnable,
						 mirrorMatrix,
						 locationVect,
						 stateManager,
						 textureDataBase,
						 glAdjstFloat,
						 totalIndecieCount,
						 objectsRendered,
						 camLocation,
						 matrixLoc,
						 particleCalculationCount,
						 &m_lastMeshREendered,
						 m_alphaMode,
						 m_lightDatabase);
	  }
	}
	else{
	 int inLoop=0;
	 POSITION posLoc;
	 for(inLoop=(m_validCount-1);inLoop>-1;inLoop--)
      for(POSITION posLoc = m_indexedDBAccess[inLoop].m_list.GetTailPosition();posLoc != NULL;){
          CTranslucentPolyObj * pObj = (CTranslucentPolyObj *)m_indexedDBAccess[inLoop].m_list.GetPrev(posLoc);
	      pObj->Render(  g_pd3dDevice,
						 mirrorEnable,
						 mirrorMatrix,
						 locationVect,
						 stateManager,
						 textureDataBase,
						 glAdjstFloat,
						 totalIndecieCount,
						 objectsRendered,
						 camLocation,
						 matrixLoc,
						 particleCalculationCount,
						 &m_lastMeshREendered,
						 m_alphaMode,
						 m_lightDatabase);
	 }
	}



	stateManager->SetAlphaPixelTest(FALSE,g_pd3dDevice);
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
 void CTranslucentPolyList::InsertPolygonOrMeshRef(ABVERTEX         *vertex,
					                                     D3DMATRIX        &viewMatrix,
										                 D3DMATRIX        &wldMatrix,
												         CEXMeshObj       *meshRef,
														 D3DVECTOR        &meshCenter,
														 EnvParticleSystem *m_particleSysOptionRef,
														 CReferenceObj * refOpt
												         )
{
	 if(m_allocTracer >= MAXRENDEROBJECTS)
		return;

     CTranslucentPolyObj * pObj  = &m_preAllocObjs[m_allocTracer++];
	 pObj->m_referenceOption = refOpt;
	 pObj->m_meshReference = NULL;
        if(meshRef)
		{
	       pObj->m_meshReference = meshRef;
		   CopyMemory(&pObj->m_materialRef,&meshRef->List_materialObject->m_useMaterial,sizeof(D3DMATERIAL9));
	       pObj->m_blendRef      = meshRef->List_materialObject->m_blendMode;
		   
		}


		CopyMemory(&pObj->m_location,&wldMatrix,sizeof(D3DMATRIX));

		pObj->m_particleSysOption = m_particleSysOptionRef;
		   
		
        pObj->m_zValue = GL_matrixFunctions.MagnitudeSquared(viewMatrix._41,viewMatrix._42,
								                             viewMatrix._43,meshCenter.x,
								                             meshCenter.y,meshCenter.z);
      
      POSITION posLast;
      m_currentIndexTemp = (int)pObj->m_zValue/m_indexDensity;//indexed position
	  
	  if(m_currentIndexTemp >= m_validCount)
	     m_currentIndexTemp = m_validCount-1;
	  
		  for(POSITION posLoc = m_indexedDBAccess[m_currentIndexTemp].m_list.GetHeadPosition();(posLast = posLoc) != NULL;)
		  {//
			 CTranslucentPolyObj * listObj = (CTranslucentPolyObj *)m_indexedDBAccess[m_currentIndexTemp].m_list.GetNext(posLoc);
			 if(pObj->m_zValue < listObj->m_zValue)
			 {
				m_lastPosition = m_indexedDBAccess[m_currentIndexTemp].m_list.InsertBefore(posLast,pObj);
				m_lastValue = pObj->m_zValue;
				m_currentObjectCount++;
				return;
			 }
		  }
		  m_indexedDBAccess[m_currentIndexTemp].m_list.AddTail(pObj);

      m_currentObjectCount++;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
 void CTranslucentPolyList::InsertPolygonOrMeshRefNonTrans(ABVERTEX         *vertex,
					                                       D3DMATRIX        &viewMatrix,
										                   D3DMATRIX        &wldMatrix,
												           CEXMeshObj       *meshRef,
														   D3DVECTOR        &meshCenter,
														   EnvParticleSystem *m_particleSysOptionRef,
														   CReferenceObj * refOpt
												           )
{
	 if(m_allocTracer >= MAXRENDEROBJECTS)
		return;

     CTranslucentPolyObj * pObj  = &m_preAllocObjs[m_allocTracer++];
	 //int sizeTest = sizeof(CTranslucentPolyObj);
	 pObj->m_referenceOption = refOpt;
	 pObj->m_meshReference = NULL;
        if(meshRef)
		{
	       pObj->m_meshReference = meshRef;
		   CopyMemory(&pObj->m_materialRef,&meshRef->List_materialObject->m_useMaterial,sizeof(D3DMATERIAL9));
	       pObj->m_blendRef      = meshRef->List_materialObject->m_blendMode;
		   
		}
		CopyMemory(&pObj->m_location,&wldMatrix,sizeof(D3DMATRIX));

		pObj->m_particleSysOption = m_particleSysOptionRef;
		   
        pObj->m_zValue = GL_matrixFunctions.MagnitudeSquared(viewMatrix._41,viewMatrix._42,
								                             viewMatrix._43,meshCenter.x,
								                             meshCenter.y,meshCenter.z);
      
      POSITION posLast;
      m_currentIndexTemp = (int)pObj->m_zValue/m_indexDensity;//indexed position
	  
	  if(m_currentIndexTemp >= m_validCount)
	     m_currentIndexTemp = m_validCount-1;

		  for(POSITION posLoc = m_indexedDBAccess[m_currentIndexTemp].m_list.GetHeadPosition();(posLast = posLoc) != NULL;)
		  {//
			 CTranslucentPolyObj * listObj = (CTranslucentPolyObj *)m_indexedDBAccess[m_currentIndexTemp].m_list.GetNext(posLoc);
			 if(pObj->m_zValue < listObj->m_zValue)
			 {
				m_lastPosition = m_indexedDBAccess[m_currentIndexTemp].m_list.InsertBefore(posLast,pObj);
				m_lastValue = pObj->m_zValue;
				m_currentObjectCount++;
				return;
			 }
		  }
		  m_indexedDBAccess[m_currentIndexTemp].m_list.AddTail(pObj);
	  
      m_currentObjectCount++;
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void CTranslucentPolyList::Flush()
{
     m_currentObjectCount	= 0;
	 m_allocTracer			= 0;

	 for(int inLoop=0;inLoop<m_validCount;inLoop++)
		  m_indexedDBAccess[inLoop].m_list.RemoveAll();
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
void CTranslucentPolyList::SafeDelete()
{
	POSITION posLoc;
	for(posLoc = GetHeadPosition();posLoc != NULL;){
        CTranslucentPolyObj * pObj = (CTranslucentPolyObj *)GetNext(posLoc);
		delete pObj;
		pObj=0;
	}

	for(int inLoop=0;inLoop<m_validCount;inLoop++)
	{
      m_indexedDBAccess[inLoop].m_list.RemoveAll();
	}

	m_currentObjectCount	= 0;
    m_lastPosition			= 0;
    m_lastValue				= 0.0f;
	m_allocTracer			= 0;
	RemoveAll();
}
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
//--------------------------------------------------------------------//
int CTranslucentPolyList::GetWorseList()
{
	int trace = 0;

	for(int inLoop=0;inLoop<m_validCount;inLoop++)
	{
      
      if(trace < m_indexedDBAccess[inLoop].m_list.GetCount())
	     trace = m_indexedDBAccess[inLoop].m_list.GetCount();
	}
	return trace;
}