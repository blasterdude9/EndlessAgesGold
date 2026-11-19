#include "StdAfx.h"
#include "renderingmanagement.h"
#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRenderingManagement::CRenderingManagement( GlobalStatusVariables	* gStatus,
											SceneAssembly			* sceneAssembly,
											CClientNetworkingLogic	* clientNet, 
											UserController			* userControls,
						                    CMenuManagement         * menuManagement)
{
	 m_gStatus							= gStatus;
	m_sceneAssembly						= sceneAssembly;
	m_clientNet							= clientNet;
	m_userControls						= userControls;
	m_menuManagement                    = menuManagement;

	GL_straightRenderDuration			= 0.0;
	GL_polysRenderedPerFrame			= 0;
	VPORTRENDERCOUNT					= 0;
	MATHTIME                            = 0;
	GL_particleCalculationCount         = 0;
	RENDERTIME                          = 0;
	m_glerr                             = 0;
	m_originalRenderTarget				= 0;
	m_mapCapStep						= -1;
	m_mapCapTiles						= 0;
	m_mapCapTilePx						= 0;
	m_mapCapTileWorld					= 0.0f;
	m_mapCapImage						= NULL;
	m_mapCapScratch						= NULL;
	m_mapCapTileTop						= NULL;
	m_mapCapTileBottom					= NULL;
	m_mapCapSavedFog					= FALSE;
	m_mapCapRefined					= FALSE;
	m_noClear						= FALSE;
	m_mapCapLevels					= 0;
	m_mapCapLevel					= -1;
	m_mapCapSavedBg					= 0;
	m_mapCapMainHr					= S_OK;
	m_nonTransRenderList				= new CTranslucentPolyList();
    m_transRenderList					= new CTranslucentPolyList();
}

CRenderingManagement::~CRenderingManagement(void)
{
	if(m_nonTransRenderList)
	{
       m_nonTransRenderList->SafeDelete();
	   delete m_nonTransRenderList;
       m_nonTransRenderList = 0;
	}
	if(m_transRenderList)
	{
       m_transRenderList->SafeDelete();
	   delete m_transRenderList;
       m_transRenderList = 0;
	}
	
}
//------------------------Render Visuals Imediate Mode--------------------------------//
void CRenderingManagement::RenderVisualsImediateMode()
{
	if(m_gStatus->m_dedicatedServer)
		return;

	if(!m_gStatus->m_3dDevice)
		return;

	if(m_gStatus->m_mapCaptureRequest || m_mapCapStep >= 0)
	{//editor zone map capture runs instead of the normal frame until it is done
		MapCaptureFrame();
		return;
	}

	//this makes sure everything is initialized around char to eliminate popping from initial spawn
	if(m_sceneAssembly->m_frustum.m_bypass)
	   m_sceneAssembly->m_frustum.m_bypass = false;

	if(m_gStatus->m_justSpawned == true)
	{
       m_sceneAssembly->m_frustum.m_bypass = true;
       m_gStatus->m_justSpawned = false;
	}

	//--------------------------//
	//-------RENDER ALL---------//
	//--------------------------//
	DWORD timeTaken = 0;
	GL_straightRenderDuration = 0.0;

		GL_polysRenderedPerFrame = 0;
		VPORTRENDERCOUNT++;
        //CAM PREP
		D3DVECTOR locationVect;
		locationVect.x = 0.0f;
		locationVect.y = 0.0f;
		locationVect.z = 0.0f;

        D3DMATRIX matProj,matrixBase,invMatrixBase;
		float fov = .8f;

		CMovementObj * ptrLocX = m_gStatus->m_entObjRuntime->GetObjByTrace(m_gStatus->m_cameraModule->m_followTrace);
		if(ptrLocX)
		{//valid cam object
			//CMovementObj * ptrLocX = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetAt(entPositionX);
			//ptrLocX->List_movementDB->m_compoundCurX = mptLoc->m_cameraObject->m_curKickX + mptLoc->m_cameraObject->m_bobXRotCur;
			//ptrLocX->List_movementDB->m_compoundCurY = mptLoc->m_cameraObject->m_curKickY;
			m_gStatus->m_cameraModule->Update(m_gStatus->m_timeAdjustFloat,ptrLocX,&m_gStatus->m_matLib);
	        matrixBase			= m_gStatus->m_cameraModule->GetCameraMatrix();
			m_gStatus->m_soundCenterMatrix = matrixBase;
            m_gStatus->m_soundManager->UpdateListenerPosition(matrixBase);

	        invMatrixBase		= m_gStatus->m_cameraModule->GetInvCameraMatrix(&m_gStatus->m_matLib);
			locationVect.x		= matrixBase._41;
			locationVect.y		= matrixBase._42;
			locationVect.z		= matrixBase._43;
			fov = ptrLocX->List_movementDB->List_capabilityFovMax;
		
			D3DMATRIX viewMatrix;
			m_gStatus->m_matLib.GetViewMatrixAB(viewMatrix,matrixBase);
			m_gStatus->m_3dDevice->SetVertexShaderConstantF( VAL_CAM, (float*)&m_gStatus->m_cameraModule->m_dirVect, 1 );

			
			//begin portal mirror block
			for(POSITION vsPos = m_gStatus->m_toRenderSurfaceDB->GetHeadPosition();vsPos != NULL;)
			{
				CViewToSurface * vPtr = (CViewToSurface*)m_gStatus->m_toRenderSurfaceDB->GetNext(vsPos);
				IDirect3DSurface9 * surfaceX = vPtr->GetRenderTargetSurfaceX(m_gStatus->m_textureDataBase,m_gStatus);
				if(surfaceX)
				{
				   //vPtr->m_type = 1;//test
				   if(vPtr->m_type == 0)
				   {//portal mirror
					   D3DMATRIX reflectMat,invreflectMat,fovMatrix,rViewMatrix;
					   if(m_gStatus->m_matLib.GenerateReflectiveCameraMatrix(  vPtr->m_oCenter, 
																			   vPtr->m_mirrorDir, 
																			   matrixBase, 
																			   &reflectMat,
																			   invreflectMat,
																			   vPtr->m_width3DMaxPoint,
																			   vPtr->m_width3DMaxPointN,
																			   vPtr->m_height3DMaxPoint,
																			   vPtr->m_height3DMaxPointN,
																			   matProj,
																			   vPtr->m_oMatrix,
																			   &fovMatrix,
																			   m_gStatus->m_cameraModule->m_clipPlane
																			   ))
					   {
						 //set viewport
					     
						 m_gStatus->m_dxRenderer->SetViewPort(vPtr->m_tempTargetWidthPx,vPtr->m_tempTargetHeightPx);
						 m_gStatus->m_matLib.GetViewMatrixAB(rViewMatrix,reflectMat);
						 m_gStatus->m_renderStateObject->SetViewMatrix((D3DXMATRIX*)&rViewMatrix,m_gStatus->m_3dDevice);
						 m_gStatus->m_renderStateObject->SetViewProjMatrixOnly((D3DXMATRIX*)&matProj,m_gStatus->m_3dDevice);
						 
						 m_sceneAssembly->m_frustum.CalculateFrustum(matProj, reflectMat);
						 //m_sceneAssembly->GeneratePopoutList(   locationVect,
							//									reflectMat,
							//									invreflectMat,
							//									&m_gStatus->GL_renderEnvironment,
							//									&timeTaken,
							//									1,
							//									fov,
							//									m_gStatus->m_dxRenderer->m_currentWidth,
							//									m_gStatus->m_dxRenderer->m_currentHeight,
							//									m_gStatus->m_cameraModule->m_hieghtAspect,
							//									fovMatrix,
							//									NULL
							//									);//generates draw list

						 m_sceneAssembly->GeneratePopoutList(   locationVect,
																reflectMat,
																invreflectMat,
																&m_gStatus->GL_renderEnvironment,
																&timeTaken,
																1,
																fov,
																(float)m_gStatus->m_dxRenderer->m_currentWidth,
																(float)m_gStatus->m_dxRenderer->m_currentHeight,
																(float)m_gStatus->m_cameraModule->m_hieghtAspect,
																fovMatrix,
																NULL
																);//generates draw list
						 //WARNING: C4244 --> int to float

						 RenderVisualsBody(locationVect, reflectMat, 0, surfaceX,fov,NULL);//renders draw list
						 
					   }
				   }//end portal mirror
				   else if(vPtr->m_type == 1)
				   {//plane clip mirror
						 m_gStatus->m_renderStateObject->SetViewMatrix((D3DXMATRIX*)&viewMatrix,m_gStatus->m_3dDevice);
						 m_gStatus->m_renderStateObject->m_mirrorModeOn = TRUE;//controls culling

						 //generate mirror application matrix
						 D3DMATRIX mirrorMatrix;
						 m_gStatus->m_matLib.InitMat(mirrorMatrix.m);
						 mirrorMatrix._33 = -1.0f;//mirror on z
						 m_gStatus->m_matLib.fastMatrixMultiply(mirrorMatrix,mirrorMatrix,vPtr->m_oMatrix);
						 mirrorMatrix = m_gStatus->m_matLib.GetDifferenceMatrix(vPtr->m_oMatrix,mirrorMatrix);
						 //end generate mirror application matrix

						 //UtilizeViewPort(mptLoc,matProj);
						 SetViewPortGetProjection(m_gStatus->m_dxRenderer->m_currentWidth,m_gStatus->m_dxRenderer->m_currentHeight,matProj,m_gStatus->m_cameraModule->m_clipPlane,fov);
						 m_gStatus->m_renderStateObject->SetProjUVGenMatrix(matProj,m_gStatus->m_3dDevice);//updates sahder constant for projection rendering
						 m_gStatus->m_dxRenderer->SetViewPort(vPtr->m_tempTargetWidthPx,vPtr->m_tempTargetHeightPx);
						 
						 D3DMATRIX mOpt;
						 m_gStatus->m_matLib.fastMatrixMultiply(mOpt,matrixBase,mirrorMatrix);
						

						 m_sceneAssembly->m_frustum.CalculateFrustum(matProj, mOpt);
						 //generate new projection with custom front clip plane
						 
						 //fixed clip position matrix
						 D3DXMATRIX cnvMatrix = vPtr->m_oMatrix;           
						 cnvMatrix._41 -= matrixBase._41;
						 cnvMatrix._42 -= matrixBase._42;
						 cnvMatrix._43 -= matrixBase._43;

						 m_gStatus->m_matLib.fastMatrixMultiply(cnvMatrix,cnvMatrix,m_gStatus->m_renderStateObject->m_viewMatrixUTP);
	                     

						 float plane[4];
						 m_gStatus->m_matLib.MatrixToPLaneZ(cnvMatrix,plane);
						 
						 D3DXMATRIX projClipMatrix = ProjectionMatrixToFrontClipProjection(matProj, plane);
						 
						 m_gStatus->m_renderStateObject->SetViewProjMatrixOnly(&projClipMatrix,m_gStatus->m_3dDevice);

						 //m_sceneAssembly->GeneratePopoutList(   locationVect,
							//									matrixBase,
							//									invMatrixBase,
							//									&m_gStatus->GL_renderEnvironment,
							//									&timeTaken,
							//									1,
							//									fov,
							//									m_gStatus->m_dxRenderer->m_currentWidth,
							//									m_gStatus->m_dxRenderer->m_currentHeight,
							//									m_gStatus->m_cameraModule->m_hieghtAspect,
							//									matrixBase,
							//									&mirrorMatrix
							//								);//generates draw list
						 m_sceneAssembly->GeneratePopoutList(   locationVect,
																matrixBase,
																invMatrixBase,
																&m_gStatus->GL_renderEnvironment,
																&timeTaken,
																1,
																fov,
																(float)m_gStatus->m_dxRenderer->m_currentWidth,
																(float)m_gStatus->m_dxRenderer->m_currentHeight,
																(float)m_gStatus->m_cameraModule->m_hieghtAspect,
																matrixBase,
																&mirrorMatrix
															);//generates draw list
						 //WARNING: C4244 --> int to float

						 RenderVisualsBody(locationVect, matrixBase, 0, surfaceX,fov,&mirrorMatrix);//renders draw list
						 
						 m_gStatus->m_renderStateObject->m_mirrorModeOn = FALSE;//controls culling
				   }//end plane clip mirror
				   else if(vPtr->m_type == 2)
				   {//projection
						
				   }//end projection
				}
			}
			//end mirror block
			m_gStatus->m_renderStateObject->m_mirrorModeOn = FALSE;//controls culling
			m_gStatus->m_renderStateObject->SetViewMatrix((D3DXMATRIX*)&viewMatrix,m_gStatus->m_3dDevice);


			//UtilizeViewPort(mptLoc,matProj);
			SetViewPortGetProjection(m_gStatus->m_dxRenderer->m_currentWidth,m_gStatus->m_dxRenderer->m_currentHeight,matProj,m_gStatus->m_cameraModule->m_clipPlane,fov);
			//Render Base scene
			
			//matrixBase = mptLoc->m_cameraObject->GetCameraMatrix();
			matrixBase = m_gStatus->m_cameraModule->GetCameraMatrix();
			m_sceneAssembly->m_frustum.CalculateFrustum(matProj, matrixBase);

			
			m_gStatus->m_tgSys_camMatrixBasis = matrixBase;//store for targeting basis
			m_gStatus->m_tgSys_projMatrixBasis = matProj;//store for targeting basis
		
			
			//m_sceneAssembly->GeneratePopoutList(    locationVect,
			//										matrixBase,
			//										invMatrixBase,
			//										&m_gStatus->GL_renderEnvironment,
			//										&timeTaken,
			//										false,
			//										fov,
			//										m_gStatus->m_dxRenderer->m_currentWidth,
			//										m_gStatus->m_dxRenderer->m_currentHeight,
			//										m_gStatus->m_cameraModule->m_hieghtAspect,
			//										matrixBase,
			//										NULL
			//									);//generates draw list
			m_sceneAssembly->GeneratePopoutList(    locationVect,
													matrixBase,
													invMatrixBase,
													&m_gStatus->GL_renderEnvironment,
													&timeTaken,
													false,
													fov,
													(float)m_gStatus->m_dxRenderer->m_currentWidth,
													(float)m_gStatus->m_dxRenderer->m_currentHeight,
													(float)m_gStatus->m_cameraModule->m_hieghtAspect,
													matrixBase,
													NULL
												);//generates draw list
			//WARNING: C4244 --> int to float

			RenderVisualsBody(locationVect, matrixBase, 0, NULL,fov,NULL);//renders draw list
		}//end valid cam object
		
	m_gStatus->m_3dDevice->BeginScene();
	OrthoModeBegin();//Goto 2D Mode
	m_menuManagement->RenderOverlays();//Render Menus
	m_gStatus->m_3dDevice->EndScene();
	OrthoModeEnd();//Return to 3D Mode

	int resultX = m_gStatus->m_dxRenderer->UpdateDisplaySurfaces();//displays final render/draw state
	if(resultX != 1)
	{
		if(resultX == 2)
			m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Restored Successfully");
		else if(resultX == 3)
			m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Restored Unsuccessfully");
		else
			m_gStatus->m_multiplayerObject->m_eventLog->AddEvent("Unknown restore attempt");

	}

	
}




void CRenderingManagement::OrthoModeBegin()
{
	D3DXMATRIX matOrtho; 
	D3DXMATRIX matIdentity;
	D3DVIEWPORT9 Viewport;
    
	//Setup the orthogonal projection matrix and the default world/view matrix
	D3DXMatrixOrthoLH(&matOrtho, (float)m_gStatus->m_dxRenderer->m_currentWidth, (float)m_gStatus->m_dxRenderer->m_currentHeight, 0.0f, 1.0f);
	D3DXMatrixIdentity(&matIdentity);

	//Set transforms
	m_gStatus->m_3dDevice->SetTransform(D3DTS_PROJECTION, &matOrtho);
	m_gStatus->m_3dDevice->SetTransform(D3DTS_WORLD, &matIdentity);
	m_gStatus->m_3dDevice->SetTransform(D3DTS_VIEW, &matIdentity);

	//Setup viewport
	Viewport.X		= 0;
	Viewport.Y		= 0;
	Viewport.Width  = m_gStatus->m_dxRenderer->m_currentWidth;
	Viewport.Height = m_gStatus->m_dxRenderer->m_currentHeight;
	Viewport.MinZ   = 0.0f;
	Viewport.MaxZ   = 1.0f;

	m_gStatus->m_3dDevice->SetViewport(&Viewport);
}

void CRenderingManagement::OrthoModeEnd()
{
	//we will set the viewport before rendering projection so this should not be necessary
	/*if(!m_gStatus->m_runtimeViewportDB->IsEmpty())
	{
		CViewportObj* mptLoc = (CViewportObj*)m_gStatus->m_runtimeViewportDB->GetHead();
		UtilizeViewPort(mptLoc);
	}*/ 
}

//--------------Popout::world optimization-------------------------------------------//
void CRenderingManagement::Popout_WldObject(CViewportObj* mptLoc)
{
	/*if(!mptLoc)//invalid vieport
		return;
	long currentStamp = GetTickCount();

	D3DVECTOR pointOfView;
	D3DMATRIX camMatrix;
	mptLoc->m_cameraObject->List_cameraBase->GetPosition(NULL,&pointOfView);
	mptLoc->m_cameraObject->List_cameraBase->GetTransform(NULL,&camMatrix);
	long timeTaken = 0;
    m_sceneAssembly->GeneratePopoutList(    pointOfView,
											camMatrix,
											&m_gStatus->GL_renderEnvironment,
											mptLoc,
											&timeTaken,
											mptLoc->m_cameraObject,
											false
											);
	MATHTIME+=(GetTickCount() - currentStamp);*/
return;
}

//------------------------------------------------------------------------------------//
void CRenderingManagement::RenderVisualsBody(   D3DVECTOR locationVect, 
												D3DMATRIX matrixBase,
												int visibilityMode,
												IDirect3DSurface9 * optionalRenderTarget,
												float & fov,
												D3DMATRIX * compoundMatrix)
{
            locationVect.x = matrixBase._41;
			locationVect.y = matrixBase._42;
			locationVect.z = matrixBase._43;
            //set constants and view
			m_gStatus->m_renderStateObject->m_locationOffset = locationVect;
			D3DMATRIX resolvedCamMatrix = matrixBase;
			resolvedCamMatrix._41 = 0.0f;
			resolvedCamMatrix._42 = 0.0f;
			resolvedCamMatrix._43 = 0.0f;
			m_gStatus->m_renderStateObject->SetCamMatrix((D3DXMATRIX*)&resolvedCamMatrix,m_gStatus->m_3dDevice);//camera matrix shader constant
            //end set constants and view
            //select surface to render to :D 
	        if(optionalRenderTarget)
			{//optional Render surface specified
			   if(!m_originalRenderTarget)
			      if(m_gStatus->m_3dDevice->GetRenderTarget( 0, &m_originalRenderTarget ) != S_OK)
	                 return;
			
			   if(m_gStatus->m_3dDevice->SetRenderTarget(0,optionalRenderTarget) != S_OK)
	              return;
			}
			else
			{//standard
	           m_gStatus->m_postProcessor->GetPrimaryRenderTarget(m_gStatus->m_dxRenderer);
			   m_gStatus->m_postProcessor->EnableRenderTarget(m_gStatus->m_dxRenderer);
			}//end standard

			if(m_gStatus->m_3dDevice->BeginScene() != S_OK)
			   return;
		
			//universal clearing of the surface
			if(!m_noClear)
			m_gStatus->m_3dDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,m_gStatus->m_backgroundColorGlobal, 1.0f, 0 );

            DWORD timeStamp = GetTickCount();
	        //non blended
	        m_gStatus->GL_objectsRendered = 0;//reset her
            
			BOOL mirrorEnable = FALSE;
			m_gStatus->m_translucentPolyList->m_alphaMode = FALSE;
			m_gStatus->m_renderStateObject->SetZWritableState(TRUE,m_gStatus->m_3dDevice);
            m_gStatus->m_renderContainerAlphaFilterList->Render(m_gStatus->m_3dDevice,
										  mirrorEnable,
										  m_gStatus->GL_MirrorTransform,
										  locationVect,
										  m_gStatus->m_renderStateObject,
										  m_gStatus->m_textureDataBase,
										  m_gStatus->m_timeAdjust,
										  GL_polysRenderedPerFrame,
										  m_gStatus->GL_objectsRendered,
										  TRUE,
										  locationVect,
										  matrixBase,
										  GL_particleCalculationCount,
										  m_gStatus->m_runtimeLightSys);//one bit alpha effeciency


			RenderEnviormentAttachedObjects(FALSE,matrixBase,compoundMatrix);//Enviorment backdrop renderings


            m_gStatus->m_renderContainerList->Render(m_gStatus->m_3dDevice,
										  mirrorEnable,
										  m_gStatus->GL_MirrorTransform,
										  locationVect,
										  m_gStatus->m_renderStateObject,
										  m_gStatus->m_textureDataBase,
										  m_gStatus->m_timeAdjust,
										  GL_polysRenderedPerFrame,
										  m_gStatus->GL_objectsRendered,
										  TRUE,
										  locationVect,
										  matrixBase,
										  GL_particleCalculationCount,
										  m_gStatus->m_runtimeLightSys);

			

            //RENDER PARTICLE SYSTEMS
			GL_particleCalculationCount=0;
			
			m_gStatus->m_renderStateObject->SetZWritableState(FALSE,m_gStatus->m_3dDevice);
			if(m_gStatus->m_globalShadowType == 1)
			   RenderStencilShadows3(locationVect,matrixBase);
			if(m_gStatus->m_globalShadowType == 2)
			   RenderSpotShadows(locationVect);
			m_gStatus->m_renderStateObject->SetFogFilter(m_gStatus->m_3dDevice,TRUE);
			m_gStatus->m_lightningSystem->RenderSystem(locationVect,
				                            m_gStatus->m_3dDevice,
											locationVect,
											m_gStatus->m_renderStateObject,
											m_gStatus->m_textureDataBase);
			
			m_gStatus->m_renderStateObject->SetZWritableState(FALSE,m_gStatus->m_3dDevice);

	        m_gStatus->m_translucentPolyList->m_alphaMode = TRUE;
			m_gStatus->m_translucentPolyList->Render(m_gStatus->m_3dDevice,
										  mirrorEnable,
										  m_gStatus->GL_MirrorTransform,
										  locationVect,//cam pos
										  m_gStatus->m_renderStateObject,
										  m_gStatus->m_textureDataBase,
										  m_gStatus->m_timeAdjust,
										  GL_polysRenderedPerFrame,
										  m_gStatus->GL_objectsRendered,
										  FALSE,
										  locationVect,
										  matrixBase,//view mat
										  GL_particleCalculationCount,
										  m_gStatus->m_runtimeLightSys);

			m_gStatus->m_renderStateObject->SetZWritableState(TRUE,m_gStatus->m_3dDevice);

			if(m_gStatus->m_dVectors.m_vCount >= 2)
			{
				DWORD oldZEnable = TRUE, oldFogEnable = FALSE, oldLighting = TRUE;
				m_gStatus->m_3dDevice->GetRenderState(D3DRS_ZENABLE, &oldZEnable);
				m_gStatus->m_3dDevice->GetRenderState(D3DRS_FOGENABLE, &oldFogEnable);
				m_gStatus->m_3dDevice->GetRenderState(D3DRS_LIGHTING, &oldLighting);
				m_gStatus->m_3dDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
				m_gStatus->m_3dDevice->SetRenderState(D3DRS_LIGHTING, TRUE);

				// Sets world matrix, FVF, and texture/blend state
				m_gStatus->m_dVectors.Render(m_gStatus->m_renderStateObject,locationVect,m_gStatus->m_3dDevice);//line based helpers-ussually editor specific

				// Pass 1: X-ray pass (visible through walls/floors in bright gold/orange)
				D3DMATERIAL9 matLine;
				ZeroMemory(&matLine, sizeof(D3DMATERIAL9));
				matLine.Diffuse.a  = 1.0f;
				matLine.Emissive.r = 1.0f;
				matLine.Emissive.g = 0.65f;
				matLine.Emissive.b = 0.0f;
				matLine.Emissive.a = 1.0f;
				m_gStatus->m_3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
				m_gStatus->m_renderStateObject->SetMaterial(m_gStatus->m_3dDevice, &matLine);
				m_gStatus->m_3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, m_gStatus->m_dVectors.m_vCount / 2, m_gStatus->m_dVectors.m_lineBuffer, sizeof(D3DVECTOR));

				// Pass 2: Direct line-of-sight pass (in front of geometry in bright lime-green)
				matLine.Emissive.r = 0.2f;
				matLine.Emissive.g = 1.0f;
				matLine.Emissive.b = 0.25f;
				m_gStatus->m_3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);
				m_gStatus->m_renderStateObject->SetMaterial(m_gStatus->m_3dDevice, &matLine);
				m_gStatus->m_3dDevice->DrawPrimitiveUP(D3DPT_LINELIST, m_gStatus->m_dVectors.m_vCount / 2, m_gStatus->m_dVectors.m_lineBuffer, sizeof(D3DVECTOR));

				m_gStatus->m_3dDevice->SetRenderState(D3DRS_ZENABLE, oldZEnable);
				m_gStatus->m_3dDevice->SetRenderState(D3DRS_FOGENABLE, oldFogEnable);
				m_gStatus->m_3dDevice->SetRenderState(D3DRS_LIGHTING, oldLighting);
			}

			if(m_gStatus->m_particleTest)
			{//if valid particle preview
			   m_gStatus->m_renderStateObject->SetZWritableState(FALSE,m_gStatus->m_3dDevice);
			   D3DVECTOR tempVect;
			   D3DMATRIX testMatrix;
			   ZeroMemory(&tempVect,sizeof(D3DVECTOR));
			   tempVect.z = 14.0;
			   tempVect = m_gStatus->m_matLib.UpdatePolygonVertex(matrixBase.m,tempVect);
			   m_gStatus->m_matLib.InitMat(testMatrix.m);
			   testMatrix._41 = tempVect.x;
			   testMatrix._42 = tempVect.y;
			   testMatrix._43 = tempVect.z;

			   tempVect.y -= 10.0f;
			   
			   m_gStatus->m_matLib.FaceTowardPoint(tempVect,&testMatrix);
               //setup test shader based systems--Experimental
			   /*if(!m_gStatus->m_particleTest->m_vshaderMesh)
			   {
                   m_gStatus->m_particleTest->ConstructVSPMesh(2.0f,.8f,1.0f,1);
                   m_gStatus->m_particleTest->m_vshaderMesh->List_blendType = m_gStatus->m_particleTest->im_mesh->List_blendType;
				   m_gStatus->m_particleTest->m_vshaderMesh->List_textureSelect[0] = m_gStatus->m_particleTest->im_mesh->List_textureSelect[0];
                   m_gStatus->m_particleTest->im_mesh->List_materialObject->Clone(&m_gStatus->m_particleTest->m_vshaderMesh->List_materialObject);
				   m_gStatus->m_particleTest->m_vshaderMesh->List_materialObject->m_pixelShader = 1;
				   m_gStatus->m_particleTest->m_vshaderMesh->List_materialObject->m_vertexShader = 1;
			   }*/
               m_gStatus->m_particleTest->VSPRender( m_gStatus->m_renderStateObject,
													 m_gStatus->m_3dDevice,
													 locationVect,
													 testMatrix,
													 m_gStatus->m_textureDataBase);
   
			   m_gStatus->m_renderStateObject->SetZWritableState(TRUE,m_gStatus->m_3dDevice);
			}//end if valid particle preview

            //overlay per viewport
			EvaluateAndRenderCharonaOverlaysV3(locationVect,matrixBase,fov);//draw charonas takes place after the scene has been rendered
            //finalize render-restore targets
			m_gStatus->m_screenFlashModule->Callback(GetTickCount(), m_gStatus->m_3dDevice, m_gStatus->m_renderStateObject, m_gStatus->m_staticTextureDataBase,matrixBase);
            
			m_gStatus->m_3dDevice->EndScene();
			if(optionalRenderTarget)
			{//optional Render surface specified
               m_gStatus->m_3dDevice->SetRenderTarget(0,m_originalRenderTarget);
			}//end optional Render surface specified
			else
			{//standard
	           m_gStatus->m_postProcessor->RestorePrimaryRenderTarget(m_gStatus->m_dxRenderer);
			   m_gStatus->m_postProcessor->RenderPostEffectToView(	m_gStatus->m_textureDataBase,
																    m_gStatus->m_dxRenderer,
																    m_gStatus->m_renderStateObject);
			}//end standard
			

			RENDERTIME += (GetTickCount() - timeStamp);            
}
//------------------------------------------------------------------------------------//
BOOL CRenderingManagement::EvaluateAndRenderCharonaOverlaysV3(D3DVECTOR locationVect,D3DMATRIX & matrixBase,float & fov)
{
	 D3DVECTOR wldPosition;
	 DWORD currentVal = 0;

	 m_gStatus->m_renderStateObject->SetFogFilter(m_gStatus->m_3dDevice,TRUE);
	 m_gStatus->m_3dDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
	 m_gStatus->m_renderStateObject->SetBlendLevel(m_gStatus->m_renderStateObject->m_intZero,m_gStatus->m_renderStateObject->m_intZero,m_gStatus->m_3dDevice);
	 for(int loop=1;loop<m_gStatus->m_renderStateObject->m_multiTextureSupported;loop++)
	 {
		   m_gStatus->m_renderStateObject->SetBlendLevel(loop,m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_3dDevice);
		   if(loop>7)
			  break;
	 }
     ABBOX clipBox;
	 D3DMATRIX matrixTempLocal,scaleMatrix;
	 D3DVECTOR camPosLoc;
     for(POSITION posRen = m_gStatus->m_charonaCache->GetTailPosition();posRen != NULL;)
	 {//1
        CCharonaObj * charona = (CCharonaObj *)m_gStatus->m_charonaCache->GetPrev(posRen);
			if(charona)
			{//must be a charona
               wldPosition = m_gStatus->m_matLib.UpdatePolygonVertex(charona->m_currentMatrix.m,charona->m_center);
			   currentVal = 0;
               if(m_gStatus->m_matLib.BoundingSphereCheck(charona->m_visibleDistance,
				                                          0.0f,
														  charona->m_center.x,charona->m_center.y,charona->m_center.z,
														  locationVect.x,locationVect.y,locationVect.z))
			   {//in range
			   clipBox.maxX = wldPosition.x+1.0f;
			   clipBox.maxY = wldPosition.y+1.0f;
			   clipBox.maxZ = wldPosition.z+1.0f;

			   clipBox.minX = wldPosition.x-1.0f;
			   clipBox.minY = wldPosition.y-1.0f;
			   clipBox.minZ = wldPosition.z-1.0f;

               if(m_sceneAssembly->m_frustum.BoundingBoxInView(clipBox))
			   {//in view


			   BOOL charonaBlocked = m_gStatus->m_collisionBase->CollisionSegmentSIFilterCharona(locationVect,
					                                                  wldPosition,
					                                                  20.0f);
               
  

               if(!charonaBlocked)
			       charonaBlocked = RealtimeSegmentTestUseCache(locationVect,wldPosition);
               
			    if(charonaBlocked == FALSE)
				{//charona is blocked	
						   
						   charona->m_currentDistance = m_gStatus->m_matLib.MagnitudeSquared(wldPosition.x,wldPosition.y,wldPosition.z,matrixBase._41,matrixBase._42,matrixBase._43);
						   float adViewVar = fov / .68f;
	                          charona->m_currentDistance = charona->m_currentDistance * adViewVar;
						   if(charona->m_currentDistance < charona->m_visibleDistance){
						      charona->m_validThisRound = TRUE;
						      charona->m_material->SetFadeIn(charona->m_fadeTime,FALSE);
						   }
						   else{
							  charona->m_validThisRound = FALSE;
                              charona->m_material->SetFadeOut(charona->m_fadeTime);
						   }
				}//end charona is blocked	
			    else{//charona is not blocked	
					       charona->m_validThisRound = FALSE;
						   charona->m_material->SetFadeOut(charona->m_fadeTime);
				}//charona is not blocked	     
			   }//end charona in view
			   else{//not in view
                         charona->m_validThisRound = FALSE;  
						 charona->m_material->SetFadeOut(charona->m_fadeTime);
			   }//end not in view*/

			   }//end in range
			   else{//not in range
                    charona->m_validThisRound = FALSE;  
				    charona->m_material->SetFadeOut(charona->m_fadeTime);
			   }//end not in range
               //--------------------------------
			   //--------------------------------
               
				   
				   //render it
                      m_gStatus->m_renderStateObject->SetTextureState(charona->m_textureIndex,
						                                 m_gStatus->m_renderStateObject->m_intZero,
						                                 m_gStatus->m_3dDevice,
										                 m_gStatus->m_textureDataBase);
					   charona->m_material->m_fogFilter = TRUE;
					   charona->m_material->Callback();
					   charona->m_material->m_blendMode = 2;
					   m_gStatus->m_renderStateObject->SetPxShader(m_gStatus->m_3dDevice, m_gStatus->m_renderStateObject->m_pxShderSystemRef, -1);
						m_gStatus->m_renderStateObject->SetShader(m_gStatus->m_3dDevice, m_gStatus->m_shaderSystem, -1);
				       m_gStatus->m_renderStateObject->SetBlendType(m_gStatus->m_3dDevice,charona->m_material->m_blendMode);
				       m_gStatus->m_renderStateObject->SetCullState(charona->m_material->m_cullOn,m_gStatus->m_3dDevice);
				       m_gStatus->m_renderStateObject->SetWireFillState(charona->m_material->m_forceWireFrame,m_gStatus->m_3dDevice);
					   m_gStatus->m_renderStateObject->SetMaterial(m_gStatus->m_3dDevice,&charona->m_material->m_useMaterial);
					   matrixTempLocal = m_gStatus->m_matrixInit2;
					   camPosLoc.x = matrixBase._41;
					   camPosLoc.y = matrixBase._42;
					   camPosLoc.z = matrixBase._43;
                       wldPosition = m_gStatus->m_matLib.UpdatePolygonVertex(charona->m_currentMatrix.m,charona->m_center);
					   matrixTempLocal._41 = wldPosition.x;
				       matrixTempLocal._42 = wldPosition.y;
				       matrixTempLocal._43 = wldPosition.z;
					   m_gStatus->m_matLib.FaceTowardPoint(camPosLoc,&matrixTempLocal);
				       matrixTempLocal._41 = matrixTempLocal._41 - locationVect.x;
				       matrixTempLocal._42 = matrixTempLocal._42 - locationVect.y;
				       matrixTempLocal._43 = matrixTempLocal._43 - locationVect.z;
                       //do scaleing
                      
					   float scaleFactor = 0.0f;
					   if(charona->m_currentDistance <= charona->m_startDistance)
						  scaleFactor = 0.0f;
					   else if(charona->m_currentDistance >= charona->m_endDistance)
                          scaleFactor = 1.0f;
					   else{
                          scaleFactor = m_gStatus->m_matLib.InterpolateFloats(0.0f,
							                                                 1.0f,
																			 (int)(charona->m_endDistance - charona->m_startDistance),
                                                                             (int)(charona->m_currentDistance - charona->m_startDistance));
						  
					   }
		               m_gStatus->m_matLib.scale(scaleFactor,scaleFactor,scaleFactor,scaleMatrix.m);
		               m_gStatus->m_matLib.fastMatrixMultiply(matrixTempLocal,scaleMatrix,matrixTempLocal); 
                       //end scaleing
					   m_gStatus->m_renderStateObject->SetWorldMatrix((D3DXMATRIX*)&matrixTempLocal,m_gStatus->m_3dDevice);
					   m_gStatus->m_renderStateObject->SetDiffuseSource(charona->m_visual->m_advancedFixedMode,m_gStatus->m_3dDevice);
					   m_gStatus->m_renderStateObject->SetVertexType(charona->m_visual->m_uvCount,m_gStatus->m_3dDevice);
					   
					   charona->m_visual->Render(m_gStatus->m_3dDevice,m_gStatus->m_renderStateObject->m_currentVertexType);
                   //end render it
			}//end must be a charona
	 }//end 1
	 m_gStatus->m_3dDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	return TRUE;
}
//------------------------------------------------------------------------------------//
void CRenderingManagement::RenderEnviormentAttachedObjects(BOOL mirrorModeOn,D3DMATRIX & cameraMatrix,D3DMATRIX * compoundMatrix)
{
if(!m_gStatus->GL_renderEnvironment || m_mapCapStep >= 0)//no sky dome while drawing the zone map from above
    return;

D3DVECTOR cameraPos;
cameraPos.x = cameraMatrix._41;
cameraPos.y = cameraMatrix._42;
cameraPos.z = cameraMatrix._43;

//mptLoc->m_cameraObject->List_cameraBase->GetPosition(NULL,&cameraPos);
m_gStatus->m_renderStateObject->SetZWritableState(FALSE,m_gStatus->m_3dDevice);
m_gStatus->m_renderStateObject->SetShadeMode(1,m_gStatus->m_3dDevice);

   for(POSITION posLocal = m_gStatus->m_worldEnviormentList->GetHeadPosition();posLocal != NULL;){//loop through objects
       CWldObject * ptrLoc = ( CWldObject* ) m_gStatus->m_worldEnviormentList->GetNext(posLocal);	

	   if(!ptrLoc->m_meshObject->m_abVertexArray.m_indicieBuffer)
	   {
			ptrLoc->m_meshObject->InitBuffer(m_gStatus->m_3dDevice,m_gStatus->m_cgGlobalShaderDB->GetByIndex(ptrLoc->m_meshObject->List_materialObject->m_optionalShaderIndex-1));
	   }
	   D3DMATRIX tempLocal = m_gStatus->m_matrixInit2;
	   
	   if(ptrLoc->List_environmentAttached == 2)
	   {
          tempLocal._42 = tempLocal._42-cameraPos.y;
	   }
	   else if(cameraPos.y < m_gStatus->m_environmentPtr->m_seaLevel)
	      tempLocal._42 = m_gStatus->m_environmentPtr->m_seaLevel-cameraPos.y;
	   
	           /*if(mirrorModeOn == TRUE){
                  m_gStatus->m_matLib.fastMatrixMultiply(tempLocal,tempLocal,m_gStatus->GL_MirrorTransform);
				  m_gStatus->m_renderStateObject->SetWorldMatrix((D3DXMATRIX*)&tempLocal,m_gStatus->m_3dDevice);
			   }
			   else{
				  m_gStatus->m_renderStateObject->SetWorldMatrix((D3DXMATRIX*)&tempLocal,m_gStatus->m_3dDevice);
			   }*/
			   
				if(ptrLoc->m_meshObject->List_materialObject){
                   ptrLoc->m_meshObject->List_materialObject->Callback();
				   if(m_gStatus->m_environmentPtr->m_enviormentTimeSystem)
				   if(m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState)
				   if(m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_sunColorRed > m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_ambientColorRed || 
				      m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_sunColorGreen > m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_ambientColorGreen || 
					  m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_sunColorBlue > m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_ambientColorBlue)
				   {
                      ptrLoc->m_meshObject->List_materialObject->m_useMaterial.Emissive.r = m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_sunColorRed;
                      ptrLoc->m_meshObject->List_materialObject->m_useMaterial.Emissive.g = m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_sunColorGreen;
                      ptrLoc->m_meshObject->List_materialObject->m_useMaterial.Emissive.b = m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_sunColorBlue;
				   }
				   else{
				      ptrLoc->m_meshObject->List_materialObject->m_useMaterial.Emissive.r = m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_ambientColorRed;
                      ptrLoc->m_meshObject->List_materialObject->m_useMaterial.Emissive.g = m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_ambientColorGreen;
                      ptrLoc->m_meshObject->List_materialObject->m_useMaterial.Emissive.b = m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_ambientColorBlue;  
				   }
   
				   m_gStatus->m_renderStateObject->SetCullState(ptrLoc->m_meshObject->List_materialObject->m_cullOn,m_gStatus->m_3dDevice);
				   m_gStatus->m_renderStateObject->SetPxShader(m_gStatus->m_3dDevice,m_gStatus->m_pxShaderSystem,ptrLoc->m_meshObject->List_materialObject->m_pixelShader);
	               m_gStatus->m_renderStateObject->SetShader(m_gStatus->m_3dDevice,m_gStatus->m_shaderSystem,ptrLoc->m_meshObject->List_materialObject->m_vertexShader);
				   m_gStatus->m_renderStateObject->SetFogFilter(m_gStatus->m_3dDevice,ptrLoc->m_meshObject->List_materialObject->m_fogFilter);
				   m_gStatus->m_renderStateObject->SetBlendType(m_gStatus->m_3dDevice,ptrLoc->m_meshObject->List_materialObject->m_blendMode);
				   m_gStatus->m_renderStateObject->SetMaterial(m_gStatus->m_3dDevice,&ptrLoc->m_meshObject->List_materialObject->m_useMaterial);
				   m_gStatus->m_renderStateObject->SetTextureTileMode(ptrLoc->m_meshObject->List_materialObject->m_textureTileType,m_gStatus->m_3dDevice);
                   
				}

                if(compoundMatrix)
				   m_gStatus->m_matLib.fastMatrixMultiply(tempLocal,tempLocal,*compoundMatrix);
				   

				m_gStatus->MultipassRenderMesh(ptrLoc->m_meshObject,0,tempLocal);
				
				m_gStatus->GL_objectsRendered++;
   }//end loop through objects
   m_gStatus->m_renderStateObject->SetZWritableState(TRUE,m_gStatus->m_3dDevice);
   m_gStatus->m_renderStateObject->SetShadeMode(0,m_gStatus->m_3dDevice);

   return;
} 
//---------------------------------------------------------------//
void CRenderingManagement::RenderStencilShadows3(D3DVECTOR locationVect, D3DMATRIX camMatrix)
{
    if(m_gStatus->m_globalShadowType == 0)
	   return;


    if(m_gStatus->m_shadowObjectsList->GetCount() == 0)
	   return;


    

    LARGE_INTEGER StartTime, EndTime;
    if(m_gStatus->GL_frequency.QuadPart != 0)QueryPerformanceCounter(&StartTime);

    m_gStatus->m_renderStateObject->SetDiffuseSource(1,m_gStatus->m_3dDevice);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_intZero,m_gStatus->m_3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int1,m_gStatus->m_3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int2,m_gStatus->m_3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int3,m_gStatus->m_3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int4,m_gStatus->m_3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int5,m_gStatus->m_3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int6,m_gStatus->m_3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int7,m_gStatus->m_3dDevice,m_gStatus->m_textureDataBase);
	//m_gStatus->m_3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_gStatus->m_renderStateObject->SetLightingMode(FALSE,m_gStatus->m_3dDevice);



	m_gStatus->m_3dDevice->Clear( 0, NULL, D3DCLEAR_STENCIL,0xff00bfff, 1.0f, 0L );
	m_gStatus->m_renderStateObject->SetStencilMode(TRUE,m_gStatus->m_3dDevice);
    //m_gStatus->m_3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,  FALSE );
	m_gStatus->m_renderStateObject->SetZWritableState(FALSE,m_gStatus->m_3dDevice);
    //m_gStatus->m_3dDevice->SetRenderState( D3DRS_SHADEMODE,     D3DSHADE_FLAT );
	m_gStatus->m_renderStateObject->SetShadeMode(1,m_gStatus->m_3dDevice);//flat shade
    m_gStatus->m_3dDevice->SetRenderState( D3DRS_STENCILFUNC,  D3DCMP_ALWAYS );
    m_gStatus->m_3dDevice->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP );
    m_gStatus->m_3dDevice->SetRenderState( D3DRS_STENCILFAIL,  D3DSTENCILOP_KEEP );

    //If ztest passes, inc/decrement stencil buffer value
    m_gStatus->m_3dDevice->SetRenderState( D3DRS_STENCILREF,       0x1 );
    m_gStatus->m_3dDevice->SetRenderState( D3DRS_STENCILMASK,      0xffffffff );
    m_gStatus->m_3dDevice->SetRenderState( D3DRS_STENCILWRITEMASK, 0xffffffff );
    

	m_gStatus->m_renderStateObject->SetBlendType(m_gStatus->m_3dDevice,8);
    DWORD         vertecCount = 0;

		//Warning	853	warning C4288: nonstandard extension used : 'posLoc' : 
		//loop control variable declared in the for-loop is used outside the for-loop scope; 
		//it conflicts with the declaration in the outer scope	

	    POSITION posLoc;
		/*for(POSITION posLoc = m_gStatus->m_shadowObjectsList->GetHeadPosition();posLoc != NULL;)*/
		for(posLoc = m_gStatus->m_shadowObjectsList->GetHeadPosition();posLoc != NULL;)

		{//build all shadow volumes
            CTranslucentPolyObj * pObj = (CTranslucentPolyObj *)m_gStatus->m_shadowObjectsList->GetNext(posLoc);
		
	        if(!pObj->m_meshReference)
				continue;
			  
			    if(pObj->m_zValue > m_gStatus->m_globalShadowDist)
				   continue;
			
                 //restructure mesh for shadow render
                pObj->m_location._41 = pObj->m_location._41 - locationVect.x;
	            pObj->m_location._42 = pObj->m_location._42 - locationVect.y;
	            pObj->m_location._43 = pObj->m_location._43 - locationVect.z;

		}
        
		//Warning	853	warning C4288: nonstandard extension used : 'posLoc' : 
		//loop control variable declared in the for-loop is used outside the for-loop scope; 
		//it conflicts with the declaration in the outer scope	
		posLoc = NULL;
		//WARNING: C4288: redeclaration scope confilct

        for(posLoc = m_gStatus->m_shadowObjectsList->GetHeadPosition();posLoc != NULL;)
		{//build all shadow volumes
             CTranslucentPolyObj * pObj = (CTranslucentPolyObj *)m_gStatus->m_shadowObjectsList->GetNext(posLoc);
			
			

	        if(!pObj->m_meshReference)
				continue;
			  
			    if(pObj->m_zValue > m_gStatus->m_globalShadowDist)
				   continue;
				
                D3DXVECTOR3 vLight;
                
                vLight.x = m_gStatus->m_environmentPtr->m_sunPosition.x;//+ m._41;
                vLight.y = m_gStatus->m_environmentPtr->m_sunPosition.y;//+ m._42;
                vLight.z = m_gStatus->m_environmentPtr->m_sunPosition.z;//+ m._43;
				if(m_gStatus->m_environmentPtr->m_enviormentTimeSystem)
			    {
                      vLight.x = m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentSunPosition.x;//+ m._41;
                      vLight.y = m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentSunPosition.y;//+ m._42;
                      vLight.z = m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentSunPosition.z;//+ m._43;
			    }
				
				if(vLight.y < .1)
				   vLight.y = 1.0f;

				vLight = m_gStatus->m_matLib.Normalize(vLight);
				vLight.x = vLight.x * ((pObj->m_meshReference->m_meshBoundingSphereRadius+5)*4);
				vLight.y = vLight.y * ((pObj->m_meshReference->m_meshBoundingSphereRadius+5)*4);
				vLight.z = vLight.z * ((pObj->m_meshReference->m_meshBoundingSphereRadius+5)*4);
				D3DXMATRIXA16 m;
				D3DXMatrixInverse( &m, NULL, (D3DXMATRIX*)&pObj->m_location );
			    FLOAT Lx =   vLight.x;//m_gStatus->m_environmentPtr->m_sunPosition.x;
                FLOAT Ly =   vLight.y;//m_gStatus->m_environmentPtr->m_sunPosition.y;
                FLOAT Lz =   vLight.z;//m_gStatus->m_environmentPtr->m_sunPosition.z;
				vLight.x = Lx*m._11 + Ly*m._21 + Lz*m._31;
                vLight.y = Lx*m._12 + Ly*m._22 + Lz*m._32;
                vLight.z = Lx*m._13 + Ly*m._23 + Lz*m._33;
                
				DWORD       faceBufferVXcount = 0;
				DWORD       indecieBufferCount = 0;
				vertecCount = 0;
                
                
						
			    CTranslucentPolyObj * pObjNext = NULL;
				
				POSITION posLast = posLoc;

			    while(posLoc)
				{
			       pObjNext = (CTranslucentPolyObj *)m_gStatus->m_shadowObjectsList->GetNext(posLast);
				
				   if(memcmp(&pObjNext->m_location,&pObj->m_location,sizeof(D3DMATRIX))==0)
				   {//exact
				      pObjNext->m_meshReference->m_abVertexArray.GetFaceDataIndecie(faceBufferVx,
						                                                            &faceBufferVXcount,
																				    &indecieBufferCount,
																				    g_indecieBuffer);
					  posLoc = posLast;
				   }//end exact
				   else
				   {
				       break;
				   }
				}
				pObj->m_meshReference->m_abVertexArray.GetFaceDataIndecie(faceBufferVx,
						                                                  &faceBufferVXcount,
																		  &indecieBufferCount,
																		  g_indecieBuffer);
				DWORD faceNum = indecieBufferCount/3;
				//pObj->m_meshReference->m_abVertexArray.MakeIntoComplexShadowVolume3(m_gStatus->m_3dDevice,(D3DXVECTOR3)vLight,bufferVx,&vertecCount,15,edgeBuffer);
				pObj->m_meshReference->m_abVertexArray.GetShadowVolumeMesh3(faceNum,
											                               (D3DXVECTOR3)vLight,
											                               bufferVx,
											                               faceBufferVx,
											                               &vertecCount,
																		   edgeBuffer2,
																		   g_indecieBuffer);

				m_gStatus->m_renderStateObject->SetWorldMatrix((D3DXMATRIX*)&pObj->m_location,m_gStatus->m_3dDevice);
				m_gStatus->m_renderStateObject->SetCullState(2,m_gStatus->m_3dDevice);
	            m_gStatus->m_3dDevice->SetRenderState( D3DRS_STENCILZFAIL,      D3DSTENCILOP_INCR );
                
	            m_gStatus->m_3dDevice->SetFVF( D3DFVF_XYZ );
				m_gStatus->m_3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, vertecCount/3,bufferVx, sizeof(D3DXVECTOR3) );
				m_gStatus->m_renderStateObject->SetCullState(1,m_gStatus->m_3dDevice);//Clockwise
                m_gStatus->m_3dDevice->SetRenderState( D3DRS_STENCILZFAIL, D3DSTENCILOP_DECR );
				m_gStatus->m_3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, vertecCount/3,bufferVx, sizeof(D3DXVECTOR3) );
		}//end build all shadow volumes

		//m_gStatus->m_3dDevice->SetRenderState(D3DRS_DEPTHBIAS, 0);
	
	m_gStatus->m_renderStateObject->SetCullState(1,m_gStatus->m_3dDevice);
    //m_gStatus->m_3dDevice->SetRenderState( D3DRS_ZWRITEENABLE,     TRUE );
	m_gStatus->m_renderStateObject->SetZWritableState(TRUE,m_gStatus->m_3dDevice);
    m_gStatus->m_3dDevice->SetRenderState( D3DRS_ZENABLE,          FALSE );
	m_gStatus->m_renderStateObject->SetFogState(m_gStatus->m_3dDevice,FALSE);
	m_gStatus->m_renderStateObject->SetBlendType(m_gStatus->m_3dDevice,7);
    m_gStatus->m_3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    m_gStatus->m_3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	m_gStatus->m_renderStateObject->SetBlendLevel(m_gStatus->m_renderStateObject->m_intZero,m_gStatus->m_renderStateObject->m_intZero,m_gStatus->m_3dDevice);
    m_gStatus->m_3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
    m_gStatus->m_3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE );
    m_gStatus->m_3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
    m_gStatus->m_3dDevice->SetRenderState( D3DRS_STENCILREF,  0x1 );
    m_gStatus->m_3dDevice->SetRenderState( D3DRS_STENCILFUNC, D3DCMP_LESSEQUAL );
    m_gStatus->m_3dDevice->SetRenderState( D3DRS_STENCILPASS, D3DSTENCILOP_KEEP );
    //Draw a big, gray square
	m_gStatus->m_renderStateObject->SetWorldMatrix((D3DXMATRIX*)&m_gStatus->m_matrixInit2,m_gStatus->m_3dDevice);
    m_gStatus->m_3dDevice->SetFVF( D3DFVF_XYZRHW | D3DFVF_DIFFUSE );
    m_gStatus->m_3dDevice->SetStreamSource( 0, m_gStatus->m_dxRenderer->m_shadowSquareVB, 0, sizeof(SHADOWVERTEX) );
    m_gStatus->m_3dDevice->DrawPrimitive( D3DPT_TRIANGLESTRIP, 0, 2 );
    //Restore render states
    m_gStatus->m_3dDevice->SetRenderState( D3DRS_ZENABLE,          TRUE );
	m_gStatus->m_renderStateObject->SetStencilMode(FALSE,m_gStatus->m_3dDevice);
	m_gStatus->m_renderStateObject->m_currentVertexType = 99;
	m_gStatus->m_renderStateObject->SetShadeMode(0,m_gStatus->m_3dDevice);//gauroud shade
	//m_gStatus->m_3dDevice->SetRenderState( D3DRS_SHADEMODE, D3DSHADE_GOURAUD );
    QueryPerformanceCounter(&EndTime);
                LONG64 ElapsedTime = EndTime.QuadPart - StartTime.QuadPart;
                m_gStatus->GL_testPerformance = (float(ElapsedTime)/ m_gStatus->GL_frequency.QuadPart)*1000.0f;
	

	//m_gStatus->m_3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
    m_gStatus->m_renderStateObject->SetLightingMode(TRUE,m_gStatus->m_3dDevice);
	m_gStatus->m_renderStateObject->SetFogState(m_gStatus->m_3dDevice,TRUE);
    return;
}
//----------------------------------------------------------------//
void CRenderingManagement::RenderSpotShadows(D3DVECTOR locationVect)
{ 
    //return;
    
	if(!m_gStatus->m_shadowEntityList)
		return;

	//LARGE_INTEGER StartTime, EndTime;
    //if(m_gStatus->GL_frequency.QuadPart != 0)QueryPerformanceCounter(&StartTime);

	D3DMATRIX wldMatrix = m_gStatus->m_matrixInit2;
	wldMatrix._41 = wldMatrix._41 - locationVect.x;
	wldMatrix._42 = wldMatrix._42 - locationVect.y;
	wldMatrix._43 = wldMatrix._43 - locationVect.z;
	m_gStatus->m_3dDevice->SetTransform( D3DTS_WORLD, &wldMatrix );
	//m_gStatus->m_3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_gStatus->m_renderStateObject->SetFogFilter(m_gStatus->m_3dDevice,TRUE);
	m_gStatus->m_renderStateObject->SetLightingMode(FALSE,m_gStatus->m_3dDevice);
	m_gStatus->m_renderStateObject->SetShadeMode(1,m_gStatus->m_3dDevice);
	m_gStatus->m_renderStateObject->SetCullState(1,m_gStatus->m_3dDevice);
	m_gStatus->m_renderStateObject->SetDiffuseSource(0,m_gStatus->m_3dDevice);
	m_gStatus->m_renderStateObject->SetBlendType(m_gStatus->m_3dDevice,4);
    m_gStatus->m_renderStateObject->SetTextureTileMode(2,m_gStatus->m_3dDevice);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_shadowIndexTemp,m_gStatus->m_renderStateObject->m_intZero,m_gStatus->m_3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int1,m_gStatus->m_3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int2,m_gStatus->m_3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int3,m_gStatus->m_3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int4,m_gStatus->m_3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int5,m_gStatus->m_3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int6,m_gStatus->m_3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetTextureState(m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_renderStateObject->m_int7,m_gStatus->m_3dDevice,m_gStatus->m_textureDataBase);
	m_gStatus->m_renderStateObject->SetShader(m_gStatus->m_3dDevice,m_gStatus->m_renderStateObject->m_shderSystemRef,-1);
	m_gStatus->m_renderStateObject->SetPxShader(m_gStatus->m_3dDevice,m_gStatus->m_renderStateObject->m_pxShderSystemRef,-1);

	m_gStatus->m_renderStateObject->SetBlendLevel(m_gStatus->m_renderStateObject->m_intZero,m_gStatus->m_renderStateObject->m_intZero,m_gStatus->m_3dDevice);//REMOVE
    for(int loop=1;loop<m_gStatus->m_renderStateObject->m_multiTextureSupported;loop++)
    {
        m_gStatus->m_renderStateObject->SetBlendLevel(loop,m_gStatus->m_renderStateObject->m_intNegOne,m_gStatus->m_3dDevice);
	    if(loop>7)
		   break;
    }

	
	m_gStatus->m_3dDevice->SetFVF( D3DFVF_XYZ | D3DFVF_TEX1 );
	m_gStatus->m_renderStateObject->SetCullState(0,m_gStatus->m_3dDevice);
	int vxCount = 0;
	for(POSITION posLoc = m_gStatus->m_shadowEntityList->GetHeadPosition();posLoc != NULL;)
	{
         CMovementObj * entPtrLoc = (CMovementObj *)m_gStatus->m_shadowEntityList->GetNext(posLoc);
		 
         vxCount = 0;
		 D3DVECTOR shdPos;
		 
		   entPtrLoc->List_baseFrame->GetPosition(NULL,&shdPos);
		   GetShadowPolyMeshSimple( m_gStatus->m_collisionBase, 
							  vxBufferLoc, 
							  vxCount,
							  shdPos,
							  (entPtrLoc->List_boundingRadius*.5f),//float radius,
							  3.0f,
							  10.0f);

		   if(vxCount > 0)
		   {			 
		     m_gStatus->m_3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, vxCount/3,vxBufferLoc, sizeof(ABVERTEX1LNT) );
		   }
	}//m_gStatus->m_collisionBase
	m_gStatus->m_renderStateObject->m_currentVertexType = 99;
	//m_gStatus->m_3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	m_gStatus->m_renderStateObject->SetLightingMode(TRUE,m_gStatus->m_3dDevice);
	m_gStatus->m_renderStateObject->SetShadeMode(0,m_gStatus->m_3dDevice);

	//QueryPerformanceCounter(&EndTime);
    //LONG64 ElapsedTime = EndTime.QuadPart - StartTime.QuadPart;
    //m_gStatus->GL_testPerformance = (float(ElapsedTime)/ m_gStatus->GL_frequency.QuadPart)*1000.0f;
}
//--------------checks Collision between a world object and a entity------------------//
BOOL CRenderingManagement::RealtimeSegmentTestUseCache(D3DVECTOR & lastPos,D3DVECTOR & curPos)
{

	 D3DVECTOR curPosTemp,lastPosTemp;

	 for(POSITION hPos = m_gStatus->m_houseingCache->GetHeadPosition();hPos != NULL;)
	 {//Houseing
      CHousingCacheObj * hsPtr = (CHousingCacheObj *)m_gStatus->m_houseingCache->GetNext(hPos);
	  CHousingObj * houseLib = m_gStatus->m_housingDB->GetByIndex(hsPtr->m_dbIndex);
      if(houseLib)
	  {
         curPosTemp.x = curPos.x - hsPtr->m_location._41;
         curPosTemp.y = curPos.y - hsPtr->m_location._42;
		 curPosTemp.z = curPos.z - hsPtr->m_location._43;
         lastPosTemp.x = lastPos.x - hsPtr->m_location._41;
         lastPosTemp.y = lastPos.y - hsPtr->m_location._42;
		 lastPosTemp.z = lastPos.z - hsPtr->m_location._43;


		 if(houseLib->m_openState)
	        if(houseLib->m_openState->CollisionSegmentSIBasic(lastPosTemp,curPosTemp,20.0f))
				return TRUE;
	  }
	 }//end Houseing
	 return FALSE;
}
//-----------------------------------------------------------------//
void CRenderingManagement::GetShadowPolyMeshSimple( CCollisionObj *collisionObject, 
													ABVERTEX1LNT * buffer, 
													int & returnVCount,
													D3DXVECTOR3 objectPos,
													float radius,
													float extentUp,
													float extentDown)
{
	
    returnVCount = 0;

	D3DVECTOR startPath,endPath,intersection,latestNormal;

	int consideredCollisions = 0;
	int actualCollisions = 0;
	int indexObjectHit = 0;
	

	startPath.x = objectPos.x;
	startPath.y = objectPos.y+extentUp;
	startPath.z = objectPos.z;

	endPath.x = objectPos.x;
	endPath.y = objectPos.y-extentDown;
	endPath.z = objectPos.z;
	
	if(collisionObject->CollisionSegmentSI(
					                   startPath,//D3DVECTOR collisionPathStart,
					                   endPath,//D3DVECTOR collisionPathEnd,
					                   &intersection.x,//float * intersectX,
					                   &intersection.y,//float * intersectY,
					                   &intersection.z,//float * intersectZ,
					                   &actualCollisions,//int *actualCollisions,
					                   &consideredCollisions,//int *consideredCollision,
					                   &latestNormal,//D3DVECTOR * latestNormal,
					                   &indexObjectHit,//int * indexOfObjectHit,
					                   50.0f,//float tolerance,
					                   TRUE//BOOL returnAfterFirstContact
					                   )==TRUE)
	{//collided
       returnVCount = 6;

	   D3DMATRIX rotationMat = m_gStatus->m_matrixInit2;
	   float posX,posY,posZ;

	   m_gStatus->m_matLib.FaceTowardPoint(latestNormal,&rotationMat);
	   //rotationMat._41 = intersection.x;
	   //rotationMat._42 = intersection.y;
	   //rotationMat._43 = intersection.z;

       
	   
	   
	   
	   //VX0
	   buffer[0].x = - radius;
	   buffer[0].z = -.1f;
	   buffer[0].y = radius;
	   buffer[0].tx0.tu = 0.0f;
	   buffer[0].tx0.tv = 0.0f;

	   posX = buffer[0].x;
	   posY = buffer[0].y;
	   posZ = buffer[0].z;

	   buffer[0].x = (rotationMat.m[0][0]*posX)+(rotationMat.m[1][0]*posY)+(rotationMat.m[2][0]*posZ);
       buffer[0].y = (rotationMat.m[0][1]*posX)+(rotationMat.m[1][1]*posY)+(rotationMat.m[2][1]*posZ);
       buffer[0].z = (rotationMat.m[0][2]*posX)+(rotationMat.m[1][2]*posY)+(rotationMat.m[2][2]*posZ);

       buffer[0].x = intersection.x + buffer[0].x;
	   buffer[0].y = intersection.y + buffer[0].y;
	   buffer[0].z = intersection.z + buffer[0].z;
	   //VX1
	   buffer[1].x = radius;
	   buffer[1].z = -.1f;
	   buffer[1].y = radius;
	   buffer[1].tx0.tu = 1.0f;
	   buffer[1].tx0.tv = 0.0f;
	   
       posX = buffer[1].x;
	   posY = buffer[1].y;
	   posZ = buffer[1].z;

	   buffer[1].x = (rotationMat.m[0][0]*posX)+(rotationMat.m[1][0]*posY)+(rotationMat.m[2][0]*posZ);
       buffer[1].y = (rotationMat.m[0][1]*posX)+(rotationMat.m[1][1]*posY)+(rotationMat.m[2][1]*posZ);
       buffer[1].z = (rotationMat.m[0][2]*posX)+(rotationMat.m[1][2]*posY)+(rotationMat.m[2][2]*posZ);


       buffer[1].x = intersection.x + buffer[1].x;
	   buffer[1].y = intersection.y + buffer[1].y;
	   buffer[1].z = intersection.z + buffer[1].z;
	   //VX2
	   buffer[2].x = - radius;
	   buffer[2].z = -.1f;
	   buffer[2].y = - radius;
	   buffer[2].tx0.tu = 0.0f;
	   buffer[2].tx0.tv = 1.0f;
	   
       posX = buffer[2].x;
	   posY = buffer[2].y;
	   posZ = buffer[2].z;

	   buffer[2].x = (rotationMat.m[0][0]*posX)+(rotationMat.m[1][0]*posY)+(rotationMat.m[2][0]*posZ);
       buffer[2].y = (rotationMat.m[0][1]*posX)+(rotationMat.m[1][1]*posY)+(rotationMat.m[2][1]*posZ);
       buffer[2].z = (rotationMat.m[0][2]*posX)+(rotationMat.m[1][2]*posY)+(rotationMat.m[2][2]*posZ);
  

       buffer[2].x = intersection.x + buffer[2].x;
	   buffer[2].y = intersection.y + buffer[2].y;
	   buffer[2].z = intersection.z + buffer[2].z;
	   //VX0
	   

       buffer[3].x = buffer[1].x;
	   buffer[3].y = buffer[1].y;
	   buffer[3].z = buffer[1].z;
	   buffer[3].tx0.tu = buffer[1].tx0.tu;
	   buffer[3].tx0.tv = buffer[1].tx0.tv;
	   
	   //VX1
	   buffer[4].x = radius;
	   buffer[4].z = -.1f;
	   buffer[4].y = -radius;
	   buffer[4].tx0.tu = 1.0f;
	   buffer[4].tx0.tv = 1.0f;
	   
       posX = buffer[4].x;
	   posY = buffer[4].y;
	   posZ = buffer[4].z;

	   buffer[4].x = (rotationMat.m[0][0]*posX)+(rotationMat.m[1][0]*posY)+(rotationMat.m[2][0]*posZ);
       buffer[4].y = (rotationMat.m[0][1]*posX)+(rotationMat.m[1][1]*posY)+(rotationMat.m[2][1]*posZ);
       buffer[4].z = (rotationMat.m[0][2]*posX)+(rotationMat.m[1][2]*posY)+(rotationMat.m[2][2]*posZ);


       buffer[4].x = intersection.x + buffer[4].x;
	   buffer[4].y = intersection.y + buffer[4].y;
	   buffer[4].z = intersection.z + buffer[4].z;
	   //VX2
  

       buffer[5].x = buffer[2].x;
	   buffer[5].y = buffer[2].y;
	   buffer[5].z = buffer[2].z;
	   buffer[5].tx0.tu = buffer[2].tx0.tu;
	   buffer[5].tx0.tv = buffer[2].tx0.tv;


	}//end collided
    
}
//-------------------sets a viewport--------------------------------------------------//
void CRenderingManagement::UtilizeViewPort(CViewportObj* vptPtr)
{
     D3DMATRIX matProj;
     UtilizeViewPort(vptPtr,matProj);
}
//-------------------sets a viewport--------------------------------------------------//
void CRenderingManagement::UtilizeViewPort(CViewportObj* vptPtr,D3DMATRIX & matProj)
{
            float X1conv = (float)vptPtr->List_X1;
		    float Y1conv = (float)vptPtr->List_Y1;
		    float X2conv = (float)vptPtr->List_X2;
		    float Y2conv = (float)vptPtr->List_Y2;

		    float width = (float)m_gStatus->m_imediateRenderRECT.right - m_gStatus->m_imediateRenderRECT.left;
	        int X1perc = (int)((X1conv*width)*.01);
	        int X2perc = (int)((X2conv*width)*.01);
	        float hieght = (float)m_gStatus->m_imediateRenderRECT.bottom - m_gStatus->m_imediateRenderRECT.top;
	        int Y1perc = (int)((Y1conv*hieght)*.01);
	        int Y2perc = (int)((Y2conv*hieght)*.01);
	        
            vptPtr->List_imViewPort.X = X1perc;
            vptPtr->List_imViewPort.Y = Y1perc;		
			vptPtr->List_imViewPort.Width = X2perc - X1perc;
		    vptPtr->List_imViewPort.Height = Y2perc - Y1perc;
			//D3DVIEWPORT8* pViewport = 0;
			//HRESULT hr GetViewport(pViewport);


			HRESULT err = m_gStatus->m_3dDevice->SetViewport(&vptPtr->List_imViewPort);
            float fAspect = ((FLOAT)vptPtr->List_imViewPort.Height) / vptPtr->List_imViewPort.Width;
            
			//D3DUtil_SetProjectionMatrix( matProj, g_PI/2.0f, 1.0f, g_fNearPlane, g_fFarPlane );
			if(vptPtr->m_cameraObject)
			{
			m_gStatus->D3DUtil_SetProjectionMatrix( matProj,
				                                    vptPtr->m_cameraObject->List_fovView, 
										            fAspect, 
										            1.0f,
										            vptPtr->m_cameraObject->List_clipPlane,
				                                    vptPtr->m_cameraObject->List_hieghtBaseAspect,
													m_gStatus->m_tgSys_clipWidth,
													m_gStatus->m_tgSys_clipHeight);

			//m_gStatus->m_3dDevice->SetTransform( D3DTS_PROJECTION , &matProj );
			m_gStatus->m_renderStateObject->SetViewProjMatrixOnly((D3DXMATRIX*)&matProj,m_gStatus->m_3dDevice);
			}
}

//----------------------------------------------------------//
void CRenderingManagement::RenderLoopBeginningChecks()
{
  if(m_gStatus->LoadNewWorld == TRUE){
	 m_gStatus->LoadScene(m_gStatus->NewWorld);
     m_gStatus->LoadNewWorld = FALSE;
  }//end init once

  if(m_gStatus->m_blackOut == TRUE){//black out target surface
     m_gStatus->m_blackOut = FALSE;
  }//end black out target surface
  //RESET COUNTERS
  
  
}
//----------------------------------------------------------//
void CRenderingManagement::ReadoutsCallback()
{

	//CONVERTED TO NEW MENU SYSTEM
  //if(m_gStatus->GL_showFps == TRUE)
  //{
	  //CString FPS = "FPS: "+m_gStatus->m_cabLib.ConvToString(m_gStatus->m_exactFPSHiRes);
	  //if(!m_gStatus->m_fpsReading)
	  //{
	  //   m_gStatus->m_fpsReading = new CTextureRenderObj(FPS,
			//	                               0, //Start Mode, 0 = default
			//                                  -0.596f, //Start X
			//								   -0.229f, //Start Y
			//	                              .008f, //Size X
			//	                              .008f, //Size Y
			//							       TRUE, //Temp Obj?
			//								   1.0f, //Red
			//								   1.0f, //Green
			//								   1.0f); //Blue
	  //}	

	  //CString ping = operator+("PING: ",m_gStatus->m_cabLib.ConvertLongToString(m_clientNet->CURPING));
   //  
	  //if(!m_gStatus->m_pingReading)
	  //{
   //      m_gStatus->m_pingReading = new CTextureRenderObj(ping,
			//	                               0, //Start Mode, 0 = default
			//                                  -0.484f, //Start X
			//								   -0.229f, //Start Y
			//	                              .008f, //Size X
			//	                              .008f, //Size Y
			//							       TRUE, //Temp Obj?
			//								   1.0f, //Red
			//								   1.0f, //Green
			//								   1.0f); //Blue
	  //}
   //  m_gStatus->m_pingReading->Initialize(m_gStatus->m_3dDevice,ping,FALSE,m_gStatus->m_symbolMapDB,m_gStatus->m_speechMode,0.0f); 
   //  m_gStatus->m_fpsReading->Initialize(m_gStatus->m_3dDevice,FPS,FALSE,m_gStatus->m_symbolMapDB,m_gStatus->m_speechMode,0.0f); 
   //  m_gStatus->m_textRenderList->AddTail(m_gStatus->m_fpsReading);
   //  m_gStatus->m_textRenderList->AddTail(m_gStatus->m_pingReading);
  //}
}
//--------------------------------------------------------------------------------------------------------------------------//
D3DXMATRIX CRenderingManagement::GetClipProjectionMatrix(D3DXMATRIX matView, D3DXMATRIX matProj,D3DXPLANE & clip_plane)
{
    D3DXMATRIX matClipProj;

    
    D3DXMATRIX WorldToProjection;
    
    
    WorldToProjection = matView * matProj;

    //m_clip_plane is plane definition (world space)
    D3DXPlaneNormalize(&clip_plane, &clip_plane);

    D3DXMatrixInverse(&WorldToProjection, NULL, &WorldToProjection);
    D3DXMatrixTranspose(&WorldToProjection, &WorldToProjection);


    D3DXVECTOR4 clipPlane(clip_plane.a, clip_plane.b, clip_plane.c, clip_plane.d);
    D3DXVECTOR4 projClipPlane;

    //transform clip plane into projection space
    D3DXVec4Transform(&projClipPlane, &clipPlane, &WorldToProjection);
    D3DXMatrixIdentity(&matClipProj);


    if (projClipPlane.w == 0)  //or less than a really small value
    {
        //plane is perpendicular to the near plane
        return matProj;
    }


    if (projClipPlane.w > 0)
    {
        //flip plane to point away from eye
        D3DXVECTOR4 clipPlane(-clip_plane.a, -clip_plane.b, -clip_plane.c, -clip_plane.d);

        //transform clip plane into projection space
        D3DXVec4Transform(&projClipPlane, &clipPlane, &WorldToProjection);
        //D3DXVECTOR4 projClipPlane = clipPlane * (2.0F / D3DXVec4Dot(&clipPlane, &WorldToProjection));
    }

    //put projection space clip plane in Z column
    matClipProj(0, 2) = projClipPlane.x;
    matClipProj(1, 2) = projClipPlane.y;
    matClipProj(2, 2) = projClipPlane.z;
    matClipProj(3, 2) = projClipPlane.w;

    //multiply into projection matrix
    D3DXMATRIX projClipMatrix = matProj * matClipProj;
    



    return projClipMatrix;
}

//-------------------------------------------------------------
/*D3DMATRIX CRenderingManagement::ProjectionMatrixToFrontClipProjection(D3DMATRIX matrix, float * plane)
{
    D3DXVECTOR4    q;
	D3DXVECTOR4    clipPlane;
	clipPlane.x = plane[0];
	clipPlane.y = plane[1];
	clipPlane.z = plane[2];
	clipPlane.w = plane[3];

    //Calculate the clip-space corner point opposite the clipping plane
    //as (sgn(clipPlane.x), sgn(clipPlane.y), 1, 1) and
    //transform it into camera space by multiplying it
    //by the inverse of the projection matrix
    
    q.x = (Sgn_f(clipPlane.x) + matrix._31) / matrix._11;
    q.y = (Sgn_f(clipPlane.y) + matrix._32) / matrix._22;
    q.z = -1.0F;
    q.w = (1.0F + matrix._33) / matrix._43;
    //Calculate the scaled plane vector
    D3DXVECTOR4 c = clipPlane * (2.0F / D3DXVec4Dot(&clipPlane, &q));
    
    //Replace the third row of the projection matrix
    matrix._13 = c.x;
    matrix._23 = c.y;
    matrix._33 = c.z + 1.0F;
    matrix._43 = c.w;

    return matrix;
}*/

D3DMATRIX CRenderingManagement::ProjectionMatrixToFrontClipProjection(D3DMATRIX matrix, float * plane)
{
    D3DXVECTOR4    q;
	D3DXVECTOR4    clipPlane;
	clipPlane.x = plane[0];
	clipPlane.y = plane[1];
	clipPlane.z = plane[2];
	clipPlane.w = plane[3];

	if(clipPlane.w == 0)
	   return matrix;

    //Calculate the clip-space corner point opposite the clipping plane
    //as (sgn(clipPlane.x), sgn(clipPlane.y), 1, 1) and
    //transform it into camera space by multiplying it
    //by the inverse of the projection matrix
    
    q.x = Sgn_f(clipPlane.x) / matrix._11;
    q.y = Sgn_f(clipPlane.y) / matrix._22;
    q.z = 1.0F;
    q.w = (1.0F - matrix._33) / matrix._43;
    //Calculate the scaled plane vector
    /*D3DXVECTOR4 c;
	c.x = clipPlane.x * (1.0F / (clipPlane.x * q.x));
	c.y = clipPlane.y * (1.0F / (clipPlane.y * q.y));
	c.z = clipPlane.z * (1.0F / (clipPlane.z * q.z));
	c.w = clipPlane.w * (1.0F / (clipPlane.w * q.w));*/

	D3DXVECTOR4 c = clipPlane * (1.0F / D3DXVec4Dot(&clipPlane, &q));
    
    //Replace the third row of the projection matrix
    matrix._13 = c.x;
    matrix._23 = c.y;
    matrix._33 = c.z;
    matrix._43 = c.w;

    return matrix;
}

float CRenderingManagement::Sgn_f(float & num) 
{
	if (num > 0.0f) return(1.0f);
	else if (num < 0.0f) return(-1.0f);
	else return(0.0f);
}

//-------------------sets a viewport--------------------------------------------------//
void CRenderingManagement::SetViewPortGetProjection(int & width,
												   int & hieght,
												   D3DMATRIX & matProj,
												   float & clipPlane,
												   float & fov) 
{
            m_tempViewPort.Height	= hieght;
            m_tempViewPort.Width	= width;
			m_tempViewPort.X		= 0;
			m_tempViewPort.Y		= 0;
			m_tempViewPort.MinZ		= 0.0f;
			m_tempViewPort.MaxZ		= 1.0f;


			HRESULT err = m_gStatus->m_3dDevice->SetViewport(&m_tempViewPort);
            float fAspect = (float)m_tempViewPort.Height / (float)m_tempViewPort.Width;
            
			m_gStatus->D3DUtil_SetProjectionMatrix( matProj,
				                                    fov, 
										            fAspect, 
										            1.0f,
										            clipPlane,
				                                    1.0f,
													m_gStatus->m_tgSys_clipWidth,
													m_gStatus->m_tgSys_clipHeight);

			m_gStatus->m_renderStateObject->SetViewProjMatrixOnly((D3DXMATRIX*)&matProj,m_gStatus->m_3dDevice);
}
//------------------------------------------------------------------------------------//
BOOL CRenderingManagement::RenderEntityToSurface(CMovementObj * mPtr,
												 IDirect3DSurface9 * surface, 
												 float camPitch, 
												 float camYaw, 
												 float camDist,
												 float fov,
												 D3DCOLOR * optionalBackgroundColor,
												 DWORD viewWidth,
												 DWORD viewHeight)
{
    //clear existing
	m_transRenderList->SafeDelete();
    m_nonTransRenderList->SafeDelete();
    

	//if(m_gStatus->m_3dDevice->EndScene() != S_OK)
	//{//weird error
		//return FALSE;
	//}

    DWORD oldVportWidth,oldVportHeight;
    //setup new viewport
	
	IDirect3DSurface9 * origRenderSurface;
    if(m_gStatus->m_3dDevice->GetRenderTarget( 0, &origRenderSurface ) != S_OK)
	   return FALSE;
    if(m_gStatus->m_3dDevice->SetRenderTarget(0,surface) != S_OK)
	   return FALSE;

	if(m_gStatus->m_3dDevice->BeginScene() != S_OK)
		return FALSE;

	m_gStatus->m_dxRenderer->SetViewPort(viewWidth,viewHeight);
	
	
    //store origional states and transforms
	D3DMATRIX origProjection, origCamera, origWorld,matProj,nViewMat;
    BOOL origionalZState = m_gStatus->m_renderStateObject->m_currentZWritableState;
    m_gStatus->m_3dDevice->GetTransform(D3DTS_PROJECTION ,&origProjection);
	m_gStatus->m_3dDevice->GetTransform(D3DTS_VIEW ,&origCamera);
	m_gStatus->m_3dDevice->GetTransform(D3DTS_WORLD ,&origWorld);
	nViewMat = m_gStatus->m_matrixInit2;
	m_gStatus->m_matLib.GetViewMatrixAB(nViewMat,m_gStatus->m_matrixInit2);
    m_gStatus->m_renderStateObject->SetViewMatrix((D3DXMATRIX*)&nViewMat, m_gStatus->m_3dDevice);

	float cnvW = (float)viewWidth;
	float cnvH = (float)viewHeight;
	fov = .6f;
	m_gStatus->D3DUtil_SetProjectionMatrix( matProj,
			                     fov, 
					             1.0f, 
					             1.0f,
						         3000.0f,
			                     1.0f,
								 cnvW,
								 cnvH);
	m_gStatus->m_renderStateObject->SetViewProjMatrixOnly((D3DXMATRIX*)&matProj,m_gStatus->m_3dDevice);
	
	
	m_gStatus->m_renderStateObject->SetWorldMatrix((D3DXMATRIX*)&m_gStatus->m_matrixInit2,m_gStatus->m_3dDevice);
	//states
    m_gStatus->m_renderStateObject->SetZWritableState(TRUE,m_gStatus->m_3dDevice);
	//m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

	if(!optionalBackgroundColor)
	    m_gStatus->m_3dDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,m_gStatus->m_backgroundColorGlobal, 1.0f, 0 );//universal clearing of the surface
	else
        m_gStatus->m_3dDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,*optionalBackgroundColor, 1.0f, 0 );//universal clearing of the surface
	//render stuff
	int controlType = 0;
   
    D3DVECTOR offset;
	BOOL mirrorBased = FALSE;
	BOOL falseTemp = FALSE;
	BOOL trueTemp = TRUE;
	// Interactive 360-degree character rotation & zoom in Character Creation / Entity Preview
	static float s_previewOrbitYaw = 0.0f;
	static float s_previewZoomOffset = 0.0f;
	static POINT s_lastPreviewPt = { 0, 0 };
	static BOOL  s_wasDraggingPreview = FALSE;

	if ((GetAsyncKeyState(VK_RBUTTON) & 0x8000) || (GetAsyncKeyState(VK_MBUTTON) & 0x8000))
	{
		POINT curPt;
		if (GetCursorPos(&curPt))
		{
			if (s_wasDraggingPreview)
			{
				s_previewOrbitYaw += (float)(curPt.x - s_lastPreviewPt.x) * 0.65f;
			}
			s_lastPreviewPt = curPt;
			s_wasDraggingPreview = TRUE;
		}
	}
	else
	{
		s_wasDraggingPreview = FALSE;
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)  s_previewOrbitYaw -= 2.5f;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) s_previewOrbitYaw += 2.5f;
	if (GetAsyncKeyState(VK_UP) & 0x8000)    s_previewZoomOffset -= 0.15f;
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)  s_previewZoomOffset += 0.15f;
	if (GetAsyncKeyState(VK_HOME) & 0x8000)  { s_previewOrbitYaw = 0.0f; s_previewZoomOffset = 0.0f; }

	while (s_previewOrbitYaw > 180.0f)  s_previewOrbitYaw -= 360.0f;
	while (s_previewOrbitYaw < -180.0f) s_previewOrbitYaw += 360.0f;
	if (s_previewZoomOffset < -12.0f) s_previewZoomOffset = -12.0f;
	if (s_previewZoomOffset >  15.0f) s_previewZoomOffset =  15.0f;

	D3DMATRIX camBase = this->m_gStatus->m_matrixInit2;
	float effectiveDist = (((mPtr->List_skeletalObject->m_box.maxY - mPtr->List_skeletalObject->m_box.minY)*2.0f)+camDist) + s_previewZoomOffset;
	if (effectiveDist < 2.0f) effectiveDist = 2.0f;
	camBase._43 = -effectiveDist;
	camBase._42 = ((mPtr->List_skeletalObject->m_box.maxY - mPtr->List_skeletalObject->m_box.minY)*.5f)+camPitch;
	D3DMATRIX invCamBase;
	m_gStatus->m_matLib.D3DMath_MatrixInvert(invCamBase,camBase);
	D3DMATRIX trans = m_gStatus->m_matrixInit2;
	m_gStatus->m_matLib.RotateY(camYaw + s_previewOrbitYaw, trans.m);
	offset.x		= camBase._41;
	offset.y		= camBase._42;
	offset.z		= camBase._43;

    //setup lighting 2 point minimum
	D3DLIGHT9 lightImStructTemp;
	ZeroMemory(&lightImStructTemp,sizeof(D3DLIGHT9));
	lightImStructTemp.Type = D3DLIGHT_POINT;
	lightImStructTemp.Position.x = mPtr->List_skeletalObject->m_box.maxX * 3.0f;
	lightImStructTemp.Position.y = mPtr->List_skeletalObject->m_box.maxY * 2.0f;
	lightImStructTemp.Range = 2000.0f;
	lightImStructTemp.Attenuation0 = 1.0f;
	lightImStructTemp.Diffuse.r = 1.0f;
	lightImStructTemp.Diffuse.g = 1.0f;
	lightImStructTemp.Diffuse.b = 1.0f;
	lightImStructTemp.Diffuse.a = 1.0f;

	int lightIndex = 1;
	m_gStatus->m_3dDevice->SetLight(lightIndex , &lightImStructTemp);
	m_gStatus->m_3dDevice->LightEnable(lightIndex , TRUE );
    m_gStatus->m_renderStateObject->SetSTLight( lightIndex,
									 m_gStatus->m_3dDevice, 
									 lightImStructTemp.Position, 
									 lightImStructTemp.Diffuse.r, 
									 lightImStructTemp.Diffuse.g, 
									 lightImStructTemp.Diffuse.b, 
									 lightImStructTemp.Range);

    m_sceneAssembly->SkeletalDeformableMeshExtraction(mPtr->List_skeletalObject,
									 trans,
									 falseTemp,//firstPersonModeTemp,
									 camBase,
									 trueTemp,
									 trueTemp,
									 invCamBase,
									 camBase,
									 falseTemp,
									 mirrorBased,
									 NULL,
									 m_transRenderList,
                                     m_nonTransRenderList
									 );
	m_sceneAssembly->SkeletalHiarcleMeshExtraction(   mPtr->List_skeletalObject,
	                                 trans,
									 falseTemp,//firstPersonModeTemp,
									 camBase,
									 trueTemp,
									 trueTemp,
									 invCamBase,
									 camBase,
									 falseTemp,
									 mirrorBased,
									 NULL,
									 m_transRenderList,
									 m_nonTransRenderList);
	//appendages
       if(mPtr->List_armedInventoryDB)
		   for(POSITION appendagePos = mPtr->List_armedInventoryDB->GetHeadPosition();appendagePos != NULL;)
		   {//1
             CArmedInventoryObj * appendPtr = (CArmedInventoryObj *)mPtr->List_armedInventoryDB->GetNext(appendagePos); 
			   if(appendPtr->m_thirdPersonSystem){//valid third person
				  appendPtr->m_thirdPersonSystem->m_currentLOD = mPtr->List_skeletalObject->m_currentLOD;
                  POSITION AppBonePos = NULL;
				  if(mPtr->List_skeletalObject->m_bonesList)
				   AppBonePos = mPtr->List_skeletalObject->m_bonesList->FindIndex(appendPtr->m_boneAttached);
				    if(AppBonePos){//valid item
                      CBoneObject * boneRef = (CBoneObject *)mPtr->List_skeletalObject->m_bonesList->GetAt(AppBonePos); 
				      D3DMATRIX appenAppliedMatrix;
					  boneRef->m_boneFrame->GetTransform(NULL,&appenAppliedMatrix);
				      m_gStatus->m_matLib.fastMatrixMultiply(appenAppliedMatrix,appenAppliedMatrix,trans);
					  m_sceneAssembly->SkeletalEmitterCallback(appendPtr->m_thirdPersonSystem,&appenAppliedMatrix,camBase,&trans,m_transRenderList);
					  //hiarcle
					  m_sceneAssembly->SkeletalHiarcleMeshExtraction(appendPtr->m_thirdPersonSystem,
									                                 appenAppliedMatrix,
																	 falseTemp,//firstPersonModeTemp,
																	 camBase,
																	 trueTemp,
																	 trueTemp,
																	 invCamBase,
																	 camBase,
																	 falseTemp,
																	 mirrorBased,
																	 NULL,
																	 m_transRenderList,
																	 m_nonTransRenderList);
					  //deformable
					  
	                  m_sceneAssembly->SkeletalDeformableMeshExtraction(   appendPtr->m_thirdPersonSystem,
																	 appenAppliedMatrix,
																	 falseTemp,//firstPersonModeTemp,
																	 camBase,
																	 trueTemp,
																	 trueTemp,
																	 invCamBase,
																	 camBase,
																	 falseTemp,
																	 mirrorBased,
																	 NULL,
																	 m_transRenderList,
																	 m_nonTransRenderList);
					}//end valid item
			   }//end valid thirdperson
		   }//end 1

	int m_pcNt = 0;
	int polyRenderedPerFrame = 0;
	m_nonTransRenderList->Render( m_gStatus->m_3dDevice,
								  falseTemp,
								  m_gStatus->GL_MirrorTransform,
								  offset,
								  m_gStatus->m_renderStateObject,
								  m_gStatus->m_textureDataBase,
								  m_gStatus->m_timeAdjust,
								  polyRenderedPerFrame,
								  m_gStatus->GL_objectsRendered,
								  TRUE,
								  offset,
								  camBase,
								  m_pcNt,
								  NULL);

	m_gStatus->m_renderStateObject->SetZWritableState(FALSE,m_gStatus->m_3dDevice);
	m_transRenderList->Render( m_gStatus->m_3dDevice,
								  falseTemp,
								  m_gStatus->GL_MirrorTransform,
								  offset,
								  m_gStatus->m_renderStateObject,
								  m_gStatus->m_textureDataBase,
								  m_gStatus->m_timeAdjust,
								  polyRenderedPerFrame,
								  m_gStatus->GL_objectsRendered,
								  TRUE,
								  offset,
								  camBase,
								  m_pcNt,
								  NULL);

	//restore origional states and transforms
    m_gStatus->m_renderStateObject->SetPxShader(m_gStatus->m_3dDevice,m_gStatus->m_pxShaderSystem,-1);
	m_gStatus->m_renderStateObject->SetShader(m_gStatus->m_3dDevice,m_gStatus->m_shaderSystem,-1);
	m_gStatus->m_renderStateObject->SetZWritableState(origionalZState,m_gStatus->m_3dDevice);
	//m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
	m_gStatus->m_3dDevice->EndScene();
    m_gStatus->m_3dDevice->SetRenderTarget(0,origRenderSurface);
	
	//if(m_gStatus->m_3dDevice->BeginScene() != S_OK)
	//	return FALSE;

	OrthoModeBegin();
	
	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CRenderingManagement::RenderItemToSurface(		CItemObj * item,
													IDirect3DSurface9 * surface, 
													float camPitch, 
													float camYaw, 
													float camDist,
													float fov,
													D3DCOLOR * optionalBackgroundColor,
													DWORD viewWidth,
													DWORD viewHeight)
{
    //clear existing
	m_transRenderList->SafeDelete();
    m_nonTransRenderList->SafeDelete();
    

	

    DWORD oldVportWidth,oldVportHeight;
    //setup new viewport
	
	IDirect3DSurface9 * origRenderSurface;
    if(m_gStatus->m_3dDevice->GetRenderTarget( 0, &origRenderSurface ) != S_OK)
	   return FALSE;
    if(m_gStatus->m_3dDevice->SetRenderTarget(0,surface) != S_OK)
	   return FALSE;

	if(m_gStatus->m_3dDevice->BeginScene() != S_OK)
		return FALSE;

	m_gStatus->m_dxRenderer->SetViewPort(viewWidth,viewHeight);
	
	
    //store origional states and transforms
	D3DMATRIX origProjection, origCamera, origWorld,matProj,nViewMat;
    BOOL origionalZState = m_gStatus->m_renderStateObject->m_currentZWritableState;
    m_gStatus->m_3dDevice->GetTransform(D3DTS_PROJECTION ,&origProjection);
	m_gStatus->m_3dDevice->GetTransform(D3DTS_VIEW ,&origCamera);
	m_gStatus->m_3dDevice->GetTransform(D3DTS_WORLD ,&origWorld);
	nViewMat = m_gStatus->m_matrixInit2;
	m_gStatus->m_matLib.GetViewMatrixAB(nViewMat,m_gStatus->m_matrixInit2);
    m_gStatus->m_renderStateObject->SetViewMatrix((D3DXMATRIX*)&nViewMat, m_gStatus->m_3dDevice);

	float cnvW = (float)viewWidth;
	float cnvH = (float)viewHeight;
	fov = .6f;
	m_gStatus->D3DUtil_SetProjectionMatrix( matProj,
			                     fov, 
					             1.0f, 
					             1.0f,
						         3000.0f,
			                     1.0f,
								 cnvW,
								 cnvH);
	m_gStatus->m_renderStateObject->SetViewProjMatrixOnly((D3DXMATRIX*)&matProj,m_gStatus->m_3dDevice);
	
	
	
	m_gStatus->m_renderStateObject->SetWorldMatrix((D3DXMATRIX*)&m_gStatus->m_matrixInit2,m_gStatus->m_3dDevice);
	//states
    m_gStatus->m_renderStateObject->SetZWritableState(TRUE,m_gStatus->m_3dDevice);
	//m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );

	if(!optionalBackgroundColor)
	    m_gStatus->m_3dDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,m_gStatus->m_backgroundColorGlobal, 1.0f, 0 );//universal clearing of the surface
	else
        m_gStatus->m_3dDevice->Clear( 0, NULL, D3DCLEAR_ZBUFFER | D3DCLEAR_TARGET,*optionalBackgroundColor, 1.0f, 0 );//universal clearing of the surface
	//render stuff
	int controlType = 0;
   
    D3DVECTOR offset;
	BOOL mirrorBased = FALSE;
	BOOL falseTemp = FALSE;
	BOOL trueTemp = TRUE;
	D3DMATRIX camBase = this->m_gStatus->m_matrixInit2;
    
	//get box
	float maxY = 0.0f;
	float minY = 0.0f;
	if(item)
	 if(m_gStatus->m_itemManager->m_visuals)
       for(POSITION iPos = item->m_attachmentRefList->GetHeadPosition();iPos != NULL;)
	   {//add items to be rendered
            CAttachmentRefObj * attachmentPtr = (CAttachmentRefObj *)item->m_attachmentRefList->GetNext(iPos);
            CArmedInventoryObj *visObj = m_gStatus->m_itemManager->m_visuals->GetByTypeDirectAccess(attachmentPtr->m_attachmentType);
            if(!visObj)
			    continue;

			if(!visObj->m_thirdPersonSystem)
				continue;
            if(visObj->m_thirdPersonSystem->m_box.maxY > maxY)
			   maxY = visObj->m_thirdPersonSystem->m_box.maxY;

			if(visObj->m_thirdPersonSystem->m_box.minY < minY)
			   minY = visObj->m_thirdPersonSystem->m_box.minY;
	   }


	camBase._43 = -(((maxY - minY)*2.0f)+camDist);
	camBase._42 = ((maxY - minY)*.5f);
	D3DMATRIX invCamBase;
	m_gStatus->m_matLib.D3DMath_MatrixInvert(invCamBase,camBase);
	D3DMATRIX trans = m_gStatus->m_matrixInit2;
	offset.x		= camBase._41;
	offset.y		= camBase._42;
	offset.z		= camBase._43;

    //setup lighting 2 point minimum
	D3DLIGHT9 lightImStructTemp;
	ZeroMemory(&lightImStructTemp,sizeof(D3DLIGHT9));
	lightImStructTemp.Type = D3DLIGHT_POINT;
	lightImStructTemp.Position.x = maxY * 3.0f;
	lightImStructTemp.Position.y = maxY * 2.0f;
	lightImStructTemp.Range = 2000.0f;
	lightImStructTemp.Attenuation0 = 1.0f;
	lightImStructTemp.Diffuse.r = 1.0f;
	lightImStructTemp.Diffuse.g = 1.0f;
	lightImStructTemp.Diffuse.b = 1.0f;
	lightImStructTemp.Diffuse.a = 1.0f;

	int lightIndex = 1;
	m_gStatus->m_3dDevice->SetLight(lightIndex , &lightImStructTemp);
	m_gStatus->m_3dDevice->LightEnable(lightIndex , TRUE );
    m_gStatus->m_renderStateObject->SetSTLight( lightIndex,
									 m_gStatus->m_3dDevice, 
									 lightImStructTemp.Position, 
									 lightImStructTemp.Diffuse.r, 
									 lightImStructTemp.Diffuse.g, 
									 lightImStructTemp.Diffuse.b, 
									 lightImStructTemp.Range);

    if(item)
	 if(m_gStatus->m_itemManager->m_visuals)
       for(POSITION iPos = item->m_attachmentRefList->GetHeadPosition();iPos != NULL;)
	   {//add items to be rendered
            CAttachmentRefObj * attachmentPtr = (CAttachmentRefObj *)item->m_attachmentRefList->GetNext(iPos);
            CArmedInventoryObj *visObj = m_gStatus->m_itemManager->m_visuals->GetByTypeDirectAccess(attachmentPtr->m_attachmentType);
            if(!visObj)
			    continue;

			if(!visObj->m_thirdPersonSystem)
				continue;

			m_sceneAssembly->SkeletalDeformableMeshExtraction(	 visObj->m_thirdPersonSystem,
																 trans,
																 falseTemp,//firstPersonModeTemp,
																 camBase,
																 trueTemp,
																 trueTemp,
																 invCamBase,
																 camBase,
																 falseTemp,
																 mirrorBased,
																 NULL,
																 m_transRenderList,
																 m_nonTransRenderList
																 );
			m_sceneAssembly->SkeletalHiarcleMeshExtraction(		 visObj->m_thirdPersonSystem,
																 trans,
																 falseTemp,//firstPersonModeTemp,
																 camBase,
																 trueTemp,
																 trueTemp,
																 invCamBase,
																 camBase,
																 falseTemp,
																 mirrorBased,
																 NULL,
																 m_transRenderList,
																 m_nonTransRenderList);
	   }//end add items to be rendered

	int m_pcNt = 0;
	int polyRenderedPerFrame = 0;
	m_nonTransRenderList->Render( m_gStatus->m_3dDevice,
								  falseTemp,
								  m_gStatus->GL_MirrorTransform,
								  offset,
								  m_gStatus->m_renderStateObject,
								  m_gStatus->m_textureDataBase,
								  m_gStatus->m_timeAdjust,
								  polyRenderedPerFrame,
								  m_gStatus->GL_objectsRendered,
								  TRUE,
								  offset,
								  camBase,
								  m_pcNt,
								  NULL);

	m_gStatus->m_renderStateObject->SetZWritableState(FALSE,m_gStatus->m_3dDevice);
	m_transRenderList->Render( m_gStatus->m_3dDevice,
								  falseTemp,
								  m_gStatus->GL_MirrorTransform,
								  offset,
								  m_gStatus->m_renderStateObject,
								  m_gStatus->m_textureDataBase,
								  m_gStatus->m_timeAdjust,
								  polyRenderedPerFrame,
								  m_gStatus->GL_objectsRendered,
								  TRUE,
								  offset,
								  camBase,
								  m_pcNt,
								  NULL);

	//restore origional states and transforms
    m_gStatus->m_renderStateObject->SetPxShader(m_gStatus->m_3dDevice,m_gStatus->m_pxShaderSystem,-1);
	m_gStatus->m_renderStateObject->SetShader(m_gStatus->m_3dDevice,m_gStatus->m_shaderSystem,-1);
	m_gStatus->m_renderStateObject->SetZWritableState(origionalZState,m_gStatus->m_3dDevice);
	//m_gStatus->m_dxRenderer->m_pd3dDevice->SetRenderState( D3DRS_ZENABLE, FALSE );
	m_gStatus->m_3dDevice->EndScene();
    m_gStatus->m_3dDevice->SetRenderTarget(0,origRenderSurface);

	OrthoModeBegin();
	
	return TRUE;
}

//------------------------Zone map capture: lay the tiles over m_mapCapMin..Max--------------//
void CRenderingManagement::MapCapSetupTiles()
{
	CCollisionObj * col = m_gStatus->m_collisionBase;
	float size = m_mapCapMax.x - m_mapCapMin.x;
	if(m_mapCapMax.z - m_mapCapMin.z > size) size = m_mapCapMax.z - m_mapCapMin.z;
	if(size < 1.0f) size = 1.0f;
	delete [] m_mapCapTileTop;    m_mapCapTileTop = NULL;
	delete [] m_mapCapTileBottom; m_mapCapTileBottom = NULL;
	// square picture centred on the zone
	float cx = (m_mapCapMin.x + m_mapCapMax.x) * 0.5f, cz = (m_mapCapMin.z + m_mapCapMax.z) * 0.5f;
	m_mapCapMin.x = cx - size * 0.5f; m_mapCapMax.x = cx + size * 0.5f;
	m_mapCapMin.z = cz - size * 0.5f; m_mapCapMax.z = cz + size * 0.5f;

	m_mapCapTiles     = 32;
	m_mapCapTilePx    = 128;                      // 32 x 128 = 4096 x 4096 picture
	m_mapCapTileWorld = size / (float)m_mapCapTiles;

	// highest and lowest geometry under each tile, so the camera sits just above it
	int nt = m_mapCapTiles * m_mapCapTiles;
	m_mapCapTileTop    = new float[nt];
	m_mapCapTileBottom = new float[nt];
	for(int i = 0; i < nt; i++) { m_mapCapTileTop[i] = -1.0e30f; m_mapCapTileBottom[i] = 1.0e30f; }
	if(col->FaceDatabank)
	for(int f = 0; f < col->FaceDataBankCount; f++)
	for(int v = 0; v < 3; v++)
	{
		D3DVECTOR & p = col->FaceDatabank[f].polygon[v];
		int ix = (int)((p.x - m_mapCapMin.x) / m_mapCapTileWorld), iz = (int)((p.z - m_mapCapMin.z) / m_mapCapTileWorld);
		if(ix < 0 || iz < 0 || ix >= m_mapCapTiles || iz >= m_mapCapTiles) continue;
		int k = iz * m_mapCapTiles + ix;
		if(p.y > m_mapCapTileTop[k])    m_mapCapTileTop[k] = p.y;
		if(p.y < m_mapCapTileBottom[k]) m_mapCapTileBottom[k] = p.y;
	}
}

//------------------------Zone map capture (EngineX: Display > Capture Zone Map)----------------//
// Renders the loaded zone straight down from above, one tile per frame, and stitches the
// tiles into Maps\<zone>.png. Maps\<zone>.txt holds the world area the picture covers
// (minX maxX minZ maxZ) so the minimap / world map can place things on it.
// North (+Z) is up in the picture, east (+X) is right.
void CRenderingManagement::MapCaptureFrame()
{
	CCollisionObj * col = m_gStatus->m_collisionBase;
	IDirect3DDevice9 * dev = m_gStatus->m_3dDevice;

	if(m_mapCapStep < 0)
	{//start
		m_gStatus->m_mapCaptureRequest = FALSE;
		if(!col || !col->boxCollection)
		{
			if(m_gStatus->m_systemReadout) m_gStatus->m_systemReadout->AddLine("Map capture: load a zone first.");
			m_gStatus->m_mapCaptureResult = "no collision data in this zone";
			return;
		}
		m_mapCapMin = col->BaseBox.min;
		m_mapCapMax = col->BaseBox.max;
		if(col->FaceDatabank && col->FaceDataBankCount > 0)
		{//the real land: the collision box is padded far past it
			m_mapCapMin.x = m_mapCapMin.y = m_mapCapMin.z =  1.0e30f;
			m_mapCapMax.x = m_mapCapMax.y = m_mapCapMax.z = -1.0e30f;
			for(int f = 0; f < col->FaceDataBankCount; f++)
			{
				// skip giant faces (the sea plane / outer walls reach +/-60000 and hide the real land size)
				D3DVECTOR * q = col->FaceDatabank[f].polygon;
				float fx0 = min(q[0].x, min(q[1].x, q[2].x)), fx1 = max(q[0].x, max(q[1].x, q[2].x));
				float fz0 = min(q[0].z, min(q[1].z, q[2].z)), fz1 = max(q[0].z, max(q[1].z, q[2].z));
				if(fx1 - fx0 > 4000.0f || fz1 - fz0 > 4000.0f)
					continue;
				for(int v = 0; v < 3; v++)
				{
					D3DVECTOR & p = q[v];
					if(p.x < m_mapCapMin.x) m_mapCapMin.x = p.x; if(p.x > m_mapCapMax.x) m_mapCapMax.x = p.x;
					if(p.y < m_mapCapMin.y) m_mapCapMin.y = p.y; if(p.y > m_mapCapMax.y) m_mapCapMax.y = p.y;
					if(p.z < m_mapCapMin.z) m_mapCapMin.z = p.z; if(p.z > m_mapCapMax.z) m_mapCapMax.z = p.z;
				}
			}
			if(m_mapCapMin.x > m_mapCapMax.x)
			{//nothing small enough - fall back to the collision box
				m_mapCapMin = col->BaseBox.min;
				m_mapCapMax = col->BaseBox.max;
			}
		}
		// expand vertical range across all visual world objects (water planes, mountain peaks, trees, roofs)
		if(m_gStatus->m_wldObjList)
		{
			for(POSITION wPos = m_gStatus->m_wldObjList->GetHeadPosition(); wPos != NULL;)
			{
				CWldObject * wldObj = (CWldObject *)m_gStatus->m_wldObjList->GetNext(wPos);
				if(!wldObj || wldObj->List_environmentAttached > 0) continue;
				if(wldObj->m_meshObject)
				{
					float minY = wldObj->m_meshObject->List_boundingBox.minY;
					float maxY = wldObj->m_meshObject->List_boundingBox.maxY;
					if(maxY >= minY && (maxY - minY) < 20000.0f)
					{
						if(minY < m_mapCapMin.y) m_mapCapMin.y = minY;
						if(maxY > m_mapCapMax.y) m_mapCapMax.y = maxY;
					}
				}
				if(wldObj->m_node)
				{
					float minY = wldObj->m_node->m_clipBox.minY;
					float maxY = wldObj->m_node->m_clipBox.maxY;
					if(maxY >= minY && (maxY - minY) < 20000.0f)
					{
						if(minY < m_mapCapMin.y) m_mapCapMin.y = minY;
						if(maxY > m_mapCapMax.y) m_mapCapMax.y = maxY;
					}
				}
			}
		}
		if(m_gStatus->m_environmentPtr)
		{
			float seaY = m_gStatus->m_environmentPtr->m_seaLevel;
			if(seaY < m_mapCapMin.y) m_mapCapMin.y = seaY;
			if(seaY > m_mapCapMax.y) m_mapCapMax.y = seaY;
		}
		float sizeX = m_mapCapMax.x - m_mapCapMin.x, sizeZ = m_mapCapMax.z - m_mapCapMin.z;
		float size  = sizeX > sizeZ ? sizeX : sizeZ;
		if(size <= 1.0f)
		{
			m_gStatus->m_mapCaptureResult = "zone has no size";
			return;
		}
		m_mapCapRefined = FALSE;
		m_mapCapLevels = 0;
		m_mapCapLevel = -1;
		m_gStatus->m_mapCaptureFloors = 0;
		MapCapSetupTiles();
		// no fog in the map picture
		m_mapCapSavedFog = m_gStatus->m_renderStateObject->m_globalEnvFogOn;
		m_gStatus->m_renderStateObject->m_globalEnvFogOn = FALSE;

		DWORD w = m_gStatus->m_dxRenderer->m_currentWidth, h = m_gStatus->m_dxRenderer->m_currentHeight;
		if(w < (DWORD)m_mapCapTilePx || h < (DWORD)m_mapCapTilePx)
		{
			m_gStatus->m_renderStateObject->m_globalEnvFogOn = m_mapCapSavedFog;
			m_gStatus->m_mapCaptureResult = "EngineX window too small";
			return;
		}
		int imgPx = m_mapCapTiles * m_mapCapTilePx;
		if(FAILED(dev->CreateOffscreenPlainSurface(imgPx, imgPx, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &m_mapCapImage, NULL)))
		{
			if(m_gStatus->m_systemReadout) m_gStatus->m_systemReadout->AddLine("Map capture: not enough memory for the map picture.");
			m_gStatus->m_renderStateObject->m_globalEnvFogOn = m_mapCapSavedFog;
			m_gStatus->m_mapCaptureResult = "not enough memory for the picture";
			return;
		}
		IDirect3DSurface9 * bb = NULL;
		D3DSURFACE_DESC bbd;
		if(FAILED(dev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &bb)))
		{ m_mapCapImage->Release(); m_mapCapImage = NULL; m_gStatus->m_renderStateObject->m_globalEnvFogOn = m_mapCapSavedFog; m_gStatus->m_mapCaptureResult = "could not read the screen"; return; }
		bb->GetDesc(&bbd);
		bb->Release();
		if(FAILED(dev->CreateOffscreenPlainSurface(bbd.Width, bbd.Height, bbd.Format, D3DPOOL_SYSTEMMEM, &m_mapCapScratch, NULL)))
		{ m_mapCapImage->Release(); m_mapCapImage = NULL; m_gStatus->m_renderStateObject->m_globalEnvFogOn = m_mapCapSavedFog; m_gStatus->m_mapCaptureResult = "could not read the screen"; return; }

		m_mapCapZone = m_gStatus->m_currentWorkingFile;
		int slash = m_mapCapZone.ReverseFind('\\'); if(m_mapCapZone.ReverseFind('/') > slash) slash = m_mapCapZone.ReverseFind('/');
		if(slash >= 0) m_mapCapZone = m_mapCapZone.Mid(slash + 1);
		int dot = m_mapCapZone.ReverseFind('.');
		if(dot > 0) m_mapCapZone = m_mapCapZone.Left(dot);
		if(m_mapCapZone.IsEmpty()) m_mapCapZone = "Zone";
		m_mapCapStep = 0;
		m_gStatus->m_mapCaptureAll = TRUE;
		m_gStatus->m_mapCaptureBusy = TRUE;
		return;
	}

	int total = m_mapCapTiles * m_mapCapTiles;
	if(m_mapCapStep < total)
	{//one tile per frame
		int tx = m_mapCapStep % m_mapCapTiles, tz = m_mapCapStep / m_mapCapTiles;
		float cx = m_mapCapMin.x + ((float)tx + 0.5f) * m_mapCapTileWorld;
		float cz = m_mapCapMin.z + ((float)tz + 0.5f) * m_mapCapTileWorld;
		float topY    = m_mapCapMax.y + 500.0f;
		float bottomY = m_mapCapMin.y - 500.0f;
		dev->SetRenderState(D3DRS_FOGENABLE, FALSE);

		D3DVECTOR eye = { cx, topY, cz };
		m_gStatus->m_renderStateObject->m_locationOffset = D3DXVECTOR3(cx, topY, cz);
		float downDir[4] = { 0.0f, -1.0f, 0.0f, 0.0f };
		dev->SetVertexShaderConstantF(VAL_CAM, downDir, 1);

		// camera straight down, north (+Z) at the top of the picture
		D3DXMATRIX cam;
		D3DXMatrixIdentity(&cam);
		cam._11 = 1.0f; cam._12 = 0.0f;  cam._13 = 0.0f;   // right = +X
		cam._21 = 0.0f; cam._22 = 0.0f;  cam._23 = 1.0f;   // up    = +Z
		cam._31 = 0.0f; cam._32 = -1.0f; cam._33 = 0.0f;   // look  = -Y
		cam._41 = cx;   cam._42 = topY;  cam._43 = cz;
		D3DXMATRIX invCam;
		D3DXMatrixInverse(&invCam, NULL, &cam);
		D3DMATRIX viewMatrix;
		D3DMATRIX camM = cam;
		m_gStatus->m_matLib.GetViewMatrixAB(viewMatrix, camM);
		m_gStatus->m_renderStateObject->SetViewMatrix((D3DXMATRIX*)&viewMatrix, dev);

		D3DXMATRIX proj;
		float zNear = 1.0f, zFar = (topY - bottomY) + 500.0f;
		if(m_mapCapLevel >= 0 && m_mapCapLevel < m_mapCapLevels)
		{// one floor: slice off what is above it (half way up to the next floor) and below it
			int k = m_mapCapLevel;
			if(k < m_mapCapLevels - 1)
				zNear = max(1.0f, topY - (m_mapCapLevelHi[k] + m_mapCapLevelLo[k + 1]) * 0.5f);
			if(k > 0)
				zFar = max(zNear + 10.0f, topY - (m_mapCapLevelLo[k] - 30.0f));
		}
		D3DXMatrixOrthoLH(&proj, m_mapCapTileWorld, m_mapCapTileWorld, zNear, zFar);
		DWORD vw = m_mapCapTilePx, vh = m_mapCapTilePx;
		m_gStatus->m_dxRenderer->SetViewPort(vw, vh);
		m_gStatus->m_renderStateObject->SetViewProjMatrixOnly(&proj, dev);
		m_sceneAssembly->m_frustum.m_bypass = true;
		m_sceneAssembly->m_frustum.CalculateFrustum(proj, camM);

		DWORD timeTaken = 0;
		float fov = 1.5f;
		float fogStart = 1.0e7f, fogEnd = 2.0e7f;
		m_sceneAssembly->GeneratePopoutList(eye, camM, invCam, &m_gStatus->GL_renderEnvironment, &timeTaken, false, fov,
		                                    (float)vw, (float)vh, 1.0f, camM, NULL);
		// push fog out of reach (the zone's fog would tint the picture the sky colour)
		m_gStatus->m_renderStateObject->SetFogConstant(dev, fogStart, fogEnd);
		dev->SetRenderState(D3DRS_FOGSTART, *(DWORD *)(&fogStart));
		dev->SetRenderState(D3DRS_FOGEND,   *(DWORD *)(&fogEnd));
		m_noClear = FALSE;
		RenderVisualsBody(eye, camM, 0, NULL, fov, NULL);

		// copy the tile (top-left corner of the back buffer) into the big picture
		IDirect3DSurface9 * bb = NULL;
		if(SUCCEEDED(dev->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &bb)))
		{
			if(SUCCEEDED(dev->GetRenderTargetData(bb, m_mapCapScratch)))
			{
				RECT src = { 0, 0, m_mapCapTilePx, m_mapCapTilePx };
				int row  = (m_mapCapTiles - 1 - tz);                    // north at the top
				RECT dst = { tx * m_mapCapTilePx, row * m_mapCapTilePx, (tx + 1) * m_mapCapTilePx, (row + 1) * m_mapCapTilePx };
				D3DXLoadSurfaceFromSurface(m_mapCapImage, NULL, &dst, m_mapCapScratch, NULL, &src, D3DX_FILTER_NONE, 0);
			}
			bb->Release();
		}
		m_gStatus->m_dxRenderer->UpdateDisplaySurfaces();   // show progress
		m_mapCapStep++;
		return;
	}

	if(!m_mapCapRefined)
	{//first pass done: find where the land actually is (anything that is not the clear colour)
	 //and capture again with the picture filling just that area - sharper and no empty sea
		D3DLOCKED_RECT lr;
		int imgPx = m_mapCapTiles * m_mapCapTilePx;
		int x0 = imgPx, y0 = imgPx, x1 = -1, y1 = -1;
		// find the land by DETAIL, not colour: the sky/sea plane around the world is a smooth gradient,
		// land and water are textured. Score 16x16 blocks by how much neighbouring pixels differ.
		const int BS = 16;
		int nb = imgPx / BS;
		int * colHits = new int[nb]; int * rowHits = new int[nb];
		ZeroMemory(colHits, nb * sizeof(int)); ZeroMemory(rowHits, nb * sizeof(int));
		if(SUCCEEDED(m_mapCapImage->LockRect(&lr, NULL, D3DLOCK_READONLY)))
		{
			for(int by = 0; by < nb; by++)
			for(int bx = 0; bx < nb; bx++)
			{
				int sum = 0, cnt = 0;
				for(int y = by * BS; y < by * BS + BS; y += 2)
				{
					DWORD * row = (DWORD *)((BYTE *)lr.pBits + y * lr.Pitch);
					for(int x = bx * BS; x < bx * BS + BS - 1; x += 2)
					{
						DWORD c = row[x], d = row[x + 1];
						int dr = (int)((c >> 16) & 255) - (int)((d >> 16) & 255);
						int dg = (int)((c >> 8) & 255)  - (int)((d >> 8) & 255);
						int db = (int)(c & 255)         - (int)(d & 255);
						sum += (dr < 0 ? -dr : dr) + (dg < 0 ? -dg : dg) + (db < 0 ? -db : db); cnt++;
					}
				}
				if(cnt && sum > cnt * 6) { colHits[bx]++; rowHits[by]++; }
			}
			m_mapCapImage->UnlockRect();
		}
		// ignore stray specks: a row/column needs at least 3 detailed blocks to count
		int c0 = -1, c1 = -1, r0 = -1, r1 = -1;
		for(int i = 0; i < nb; i++) { if(colHits[i] >= 3) { if(c0 < 0) c0 = i; c1 = i; } if(rowHits[i] >= 3) { if(r0 < 0) r0 = i; r1 = i; } }
		delete [] colHits; delete [] rowHits;
		if(c0 >= 0 && r0 >= 0)
		{
			if(c0 > 0) c0--; if(r0 > 0) r0--; if(c1 < nb - 1) c1++; if(r1 < nb - 1) r1++;   // one block of margin
			x0 = c0 * BS; x1 = c1 * BS + BS - 2; y0 = r0 * BS; y1 = r1 * BS + BS - 2;
		}
		m_mapCapRefined = TRUE;
		if(x1 > x0 && y1 > y0 && (x0 > 4 || y0 > 4 || x1 < imgPx - 5 || y1 < imgPx - 5))
		{
			float worldPerPx = m_mapCapTileWorld / (float)m_mapCapTilePx;
			float nMinX = m_mapCapMin.x + (float)x0 * worldPerPx;
			float nMaxX = m_mapCapMin.x + (float)(x1 + 2) * worldPerPx;
			float nMaxZ = m_mapCapMax.z - (float)y0 * worldPerPx;          // picture row 0 = north
			float nMinZ = m_mapCapMax.z - (float)(y1 + 2) * worldPerPx;
			m_mapCapMin.x = nMinX; m_mapCapMax.x = nMaxX;
			m_mapCapMin.z = nMinZ; m_mapCapMax.z = nMaxZ;
			MapCapSetupTiles();
			if(SUCCEEDED(m_mapCapImage->LockRect(&lr, NULL, 0)))
			{
				for(int y = 0; y < imgPx; y++) ZeroMemory((BYTE *)lr.pBits + y * lr.Pitch, imgPx * 4);
				m_mapCapImage->UnlockRect();
			}
			m_mapCapStep = 0;
			return;
		}
	}

	// done - save
	CString dir = m_gStatus->GL_workingDirectory + "\\Maps";
	CreateDirectoryA(dir, NULL);
	CString png = dir + "\\" + m_mapCapZone + ".png";
	CString txt = dir + "\\" + m_mapCapZone + ".txt";
	int imgPxAll = m_mapCapTiles * m_mapCapTilePx;
	if(m_mapCapLevel < 0)
	{// the normal picture (every floor seen from above)
		m_mapCapMainHr = D3DXSaveSurfaceToFileA(png, D3DXIFF_PNG, m_mapCapImage, NULL, NULL);
		m_mapCapLevels = MapCapFindLevels();
		if(SUCCEEDED(m_mapCapMainHr) && m_mapCapLevels >= 2)
		{// floors stacked on top of each other: one more picture per floor
			m_mapCapLevel = 0;
			m_mapCapSavedBg = m_gStatus->m_backgroundColorGlobal;
			m_gStatus->m_backgroundColorGlobal = D3DCOLOR_XRGB(14, 16, 20);
			D3DLOCKED_RECT lr;
			if(SUCCEEDED(m_mapCapImage->LockRect(&lr, NULL, 0)))
			{
				for(int y = 0; y < imgPxAll; y++) ZeroMemory((BYTE *)lr.pBits + y * lr.Pitch, imgPxAll * 4);
				m_mapCapImage->UnlockRect();
			}
			m_mapCapStep = 0;
			if(m_gStatus->m_systemReadout)
			{
				CString line;
				line.Format("Map capture: %d floors found, drawing each one", m_mapCapLevels);
				m_gStatus->m_systemReadout->AddLine(line);
			}
			return;
		}
	}
	else
	{// one floor done (floor 1 = lowest)
		CString lpng;
		lpng.Format("%s\\%s_L%d.png", (const char*)dir, (const char*)m_mapCapZone, m_mapCapLevel + 1);
		if(!MapCapSaveImage(lpng, 2048))
			m_mapCapMainHr = E_FAIL;
		m_mapCapLevel++;
		if(m_mapCapLevel < m_mapCapLevels && SUCCEEDED(m_mapCapMainHr))
		{
			D3DLOCKED_RECT lr;
			if(SUCCEEDED(m_mapCapImage->LockRect(&lr, NULL, 0)))
			{
				for(int y = 0; y < imgPxAll; y++) ZeroMemory((BYTE *)lr.pBits + y * lr.Pitch, imgPxAll * 4);
				m_mapCapImage->UnlockRect();
			}
			m_mapCapStep = 0;
			return;
		}
		m_gStatus->m_backgroundColorGlobal = m_mapCapSavedBg;
	}
	HRESULT hr = m_mapCapMainHr;
	int floors = (SUCCEEDED(hr) && m_mapCapLevels >= 2) ? m_mapCapLevels : 0;
	FILE * f = fopen(txt, "w");
	if(f)
	{
		fprintf(f, "%f %f %f %f\n", m_mapCapMin.x, m_mapCapMax.x, m_mapCapMin.z, m_mapCapMax.z);
		if(floors >= 2)
		{// player height range of each floor (floor 1 = lowest), picture Maps\<zone>_L<n>.png
			fprintf(f, "levels %d\n", floors);
			for(int k = 0; k < floors; k++)
			{
				float lo = (k == 0) ? -1.0e9f : m_mapCapLevelLo[k] - 10.0f;
				float hi = (k == floors - 1) ? 1.0e9f : m_mapCapLevelLo[k + 1] - 10.0f;
				fprintf(f, "%f %f\n", lo, hi);
			}
		}
		fclose(f);
	}
	m_mapCapImage->Release();   m_mapCapImage = NULL;
	m_mapCapScratch->Release(); m_mapCapScratch = NULL;
	delete [] m_mapCapTileTop;    m_mapCapTileTop = NULL;
	delete [] m_mapCapTileBottom; m_mapCapTileBottom = NULL;
	m_gStatus->m_renderStateObject->m_globalEnvFogOn = m_mapCapSavedFog;
	m_mapCapStep = -1;
	m_mapCapLevel = -1;
	m_gStatus->m_mapCaptureAll = FALSE;
	m_gStatus->m_mapCaptureBusy = FALSE;
	m_gStatus->m_mapCaptureFloors = floors;
	m_gStatus->m_mapCaptureResult = SUCCEEDED(hr) ? "saved" : "could not save the picture";
	m_sceneAssembly->m_frustum.m_bypass = false;
	CString doneMsg;
	if(!SUCCEEDED(hr))
		doneMsg = "Could not save the zone map.";
	else if(floors >= 2)
		doneMsg.Format("Zone map saved:\n%s\n\n%d floors found - also saved %s_L1.png to _L%d.png", (const char*)png, floors, (const char*)m_mapCapZone, floors);
	else
		doneMsg = CString("Zone map saved:\n") + png;
	if(m_gStatus->m_systemReadout)
		m_gStatus->m_systemReadout->AddLine(SUCCEEDED(hr) ? CString("Map saved: Maps\\") + m_mapCapZone + ".png" : CString("Map capture: could not save the picture."));
	if(!m_gStatus->m_mapCaptureBatch)
		::MessageBoxA(NULL, doneMsg, "Capture Zone Map", MB_OK | MB_ICONINFORMATION);
}

// save the stitched picture, shrunk to size x size (floor pictures do not need 4096)
bool CRenderingManagement::MapCapSaveImage(const CString & path, int size)
{
	IDirect3DDevice9 * dev = m_gStatus->m_3dDevice;
	int imgPx = m_mapCapTiles * m_mapCapTilePx;
	if(size > 0 && size < imgPx)
	{
		IDirect3DSurface9 * smallSurf = NULL;
		if(SUCCEEDED(dev->CreateOffscreenPlainSurface(size, size, D3DFMT_X8R8G8B8, D3DPOOL_SYSTEMMEM, &smallSurf, NULL)))
		{
			HRESULT hr = D3DXLoadSurfaceFromSurface(smallSurf, NULL, NULL, m_mapCapImage, NULL, NULL, D3DX_FILTER_BOX, 0);
			if(SUCCEEDED(hr))
				hr = D3DXSaveSurfaceToFileA(path, D3DXIFF_PNG, smallSurf, NULL, NULL);
			smallSurf->Release();
			if(SUCCEEDED(hr))
				return true;
		}
	}
	return SUCCEEDED(D3DXSaveSurfaceToFileA(path, D3DXIFF_PNG, m_mapCapImage, NULL, NULL));
}

// Find floors stacked on top of each other from the walkable collision faces inside the picture.
// Hills and separate islands at different heights stay one map: two height bands only count as
// separate floors when one lies over the other. Maps\<zone>.levels (one "lowY highY" per line,
// lowest floor first) overrides the automatic split for a zone.
int CRenderingManagement::MapCapFindLevels()
{
	// manual override
	{
		CString lv = m_gStatus->GL_workingDirectory + "\\Maps\\" + m_mapCapZone + ".levels";
		FILE * f = fopen(lv, "r");
		if(f)
		{
			int n = 0;
			float a, b;
			char line[256];
			while(n < 8 && fgets(line, sizeof(line), f))
				if(sscanf(line, "%f %f", &a, &b) == 2 && b >= a)
				{
					m_mapCapLevelLo[n] = a; m_mapCapLevelHi[n] = b; n++;
				}
			fclose(f);
			if(n >= 1)
				return n;
		}
	}

	CCollisionObj * col = m_gStatus->m_collisionBase;
	if(!col || !col->FaceDatabank || col->FaceDataBankCount <= 0)
		return 0;
	float minX = m_mapCapMin.x, maxX = m_mapCapMax.x, minZ = m_mapCapMin.z, maxZ = m_mapCapMax.z;

	// walkable faces (the engine itself treats normal.y > tolerance as floor) inside the picture
	std::vector<int>   faces;
	std::vector<float> faceY, faceA;
	float yMin = 1.0e30f, yMax = -1.0e30f;
	for(int i = 0; i < col->FaceDataBankCount; i++)
	{
		CFaceStruct & fc = col->FaceDatabank[i];
		float nl = sqrtf(fc.normal.x * fc.normal.x + fc.normal.y * fc.normal.y + fc.normal.z * fc.normal.z);
		if(nl < 1.0e-6f || fc.normal.y / nl < 0.7f)
			continue;
		D3DVECTOR & p0 = fc.polygon[0]; D3DVECTOR & p1 = fc.polygon[1]; D3DVECTOR & p2 = fc.polygon[2];
		float cx = (p0.x + p1.x + p2.x) / 3.0f, cy = (p0.y + p1.y + p2.y) / 3.0f, cz = (p0.z + p1.z + p2.z) / 3.0f;
		if(cx < minX || cx > maxX || cz < minZ || cz > maxZ)
			continue;
		float ux = p1.x - p0.x, uy = p1.y - p0.y, uz = p1.z - p0.z;
		float vx = p2.x - p0.x, vy = p2.y - p0.y, vz = p2.z - p0.z;
		float crx = uy * vz - uz * vy, cry = uz * vx - ux * vz, crz = ux * vy - uy * vx;
		float area = 0.5f * sqrtf(crx * crx + cry * cry + crz * crz);
		if(area <= 0.0f)
			continue;
		faces.push_back(i); faceY.push_back(cy); faceA.push_back(area);
		if(cy < yMin) yMin = cy;
		if(cy > yMax) yMax = cy;
	}
	if(faces.size() < 8 || yMax - yMin < 60.0f)
		return 1;

	// floor area per height band
	float B = 8.0f;
	int nb = (int)((yMax - yMin) / B) + 1;
	if(nb > 8192) { B = (yMax - yMin) / 8191.0f; nb = 8192; }
	std::vector<double> band(nb, 0.0);
	double total = 0.0;
	for(size_t i = 0; i < faces.size(); i++)
	{
		int bi = (int)((faceY[i] - yMin) / B);
		if(bi < 0) bi = 0; if(bi >= nb) bi = nb - 1;
		band[bi] += faceA[i];
		total += faceA[i];
	}
	if(total <= 0.0)
		return 1;

	// groups of heights that really hold floor, split where there is 48+ units of (almost) nothing
	struct Grp { int b0, b1; double a; };
	std::vector<Grp> g;
	double sig = total * 0.004;
	int gapBins = (int)(48.0f / B + 0.5f);
	if(gapBins < 1) gapBins = 1;
	for(int bi = 0; bi < nb; bi++)
	{
		if(band[bi] < sig)
			continue;
		if(g.empty() || bi - g.back().b1 > gapBins)
		{
			Grp n = { bi, bi, 0.0 };
			g.push_back(n);
		}
		g.back().b1 = bi;
	}
	if(g.size() < 2)
		return 1;
	for(size_t k = 0; k < g.size(); k++)
	{
		int from = (k == 0) ? 0 : (g[k - 1].b1 + g[k].b0) / 2 + 1;
		int to   = (k == g.size() - 1) ? nb - 1 : (g[k].b1 + g[k + 1].b0) / 2;
		g[k].a = 0.0;
		for(int bi = from; bi <= to; bi++) g[k].a += band[bi];
	}
	// small groups (ledges, stairs, a cave) join their nearest neighbour
	while(g.size() >= 2)
	{
		int s = -1;
		for(size_t k = 0; k < g.size(); k++)
			if(g[k].a < total * 0.015 && (s < 0 || g[k].a < g[s].a)) s = (int)k;
		if(s < 0)
			break;
		int t;
		if(s == 0) t = 1;
		else if(s == (int)g.size() - 1) t = s - 1;
		else t = (g[s].b0 - g[s - 1].b1 <= g[s + 1].b0 - g[s].b1) ? s - 1 : s + 1;
		g[t].b0 = min(g[t].b0, g[s].b0); g[t].b1 = max(g[t].b1, g[s].b1); g[t].a += g[s].a;
		g.erase(g.begin() + s);
	}
	if(g.size() < 2)
		return 1;

	// which ground each group covers (seen from above), to tell stacked floors from hills / islands
	float size = max(maxX - minX, maxZ - minZ);
	int gridN = (int)(size / 16.0f);
	if(gridN > 1024) gridN = 1024;
	if(gridN < 16) gridN = 16;
	float cell = size / (float)gridN;
	size_t maskBytes = ((size_t)gridN * gridN + 7) / 8;
	std::vector< std::vector<unsigned char> > mask(g.size(), std::vector<unsigned char>(maskBytes, 0));
	std::vector<int> bandGroup(nb, -1);
	for(size_t k = 0; k < g.size(); k++)
	{
		int from = (k == 0) ? 0 : (g[k - 1].b1 + g[k].b0) / 2 + 1;
		int to   = (k == g.size() - 1) ? nb - 1 : (g[k].b1 + g[k + 1].b0) / 2;
		for(int bi = from; bi <= to; bi++) bandGroup[bi] = (int)k;
	}
	for(size_t i = 0; i < faces.size(); i++)
	{
		int bi = (int)((faceY[i] - yMin) / B);
		if(bi < 0) bi = 0; if(bi >= nb) bi = nb - 1;
		int k = bandGroup[bi];
		if(k < 0) continue;
		CFaceStruct & fc = col->FaceDatabank[faces[i]];
		float ax = fc.polygon[0].x, az = fc.polygon[0].z, bx = fc.polygon[1].x, bz = fc.polygon[1].z, qx = fc.polygon[2].x, qz = fc.polygon[2].z;
		int c0 = (int)((min(ax, min(bx, qx)) - minX) / cell), c1 = (int)((max(ax, max(bx, qx)) - minX) / cell);
		int r0 = (int)((min(az, min(bz, qz)) - minZ) / cell), r1 = (int)((max(az, max(bz, qz)) - minZ) / cell);
		if(c0 < 0) c0 = 0; if(r0 < 0) r0 = 0; if(c1 >= gridN) c1 = gridN - 1; if(r1 >= gridN) r1 = gridN - 1;
		float d = (bz - qz) * (ax - qx) + (qx - bx) * (az - qz);
		bool tiny = (c0 == c1 && r0 == r1) || fabsf(d) < 1.0e-6f;
		for(int r = r0; r <= r1; r++)
		for(int c = c0; c <= c1; c++)
		{
			if(!tiny)
			{// cell centre inside the triangle (seen from above)?
				float px = minX + (c + 0.5f) * cell, pz = minZ + (r + 0.5f) * cell;
				float l1 = ((bz - qz) * (px - qx) + (qx - bx) * (pz - qz)) / d;
				float l2 = ((qz - az) * (px - qx) + (ax - qx) * (pz - qz)) / d;
				if(l1 < -0.01f || l2 < -0.01f || l1 + l2 > 1.01f)
					continue;
			}
			size_t bit = (size_t)r * gridN + c;
			mask[k][bit >> 3] |= (unsigned char)(1 << (bit & 7));
		}
	}
	// neighbouring groups that do not lie over each other are the same "floor" for the map
	size_t k = 0;
	while(k + 1 < g.size())
	{
		size_t both = 0, ca = 0, cb = 0;
		for(size_t j = 0; j < maskBytes; j++)
		{
			unsigned char a = mask[k][j], bb = mask[k + 1][j], ab = (unsigned char)(a & bb);
			for(int t = 0; t < 8; t++) { ca += (a >> t) & 1; cb += (bb >> t) & 1; both += (ab >> t) & 1; }
		}
		size_t smaller = min(ca, cb);
		if(smaller == 0 || (double)both < 0.15 * (double)smaller)
		{
			g[k].b1 = g[k + 1].b1; g[k].a += g[k + 1].a;
			for(size_t j = 0; j < maskBytes; j++) mask[k][j] |= mask[k + 1][j];
			g.erase(g.begin() + k + 1);
			mask.erase(mask.begin() + k + 1);
		}
		else
			k++;
	}
	// at most 8 floors: join the closest ones
	while(g.size() > 8)
	{
		size_t best = 0;
		for(size_t j = 1; j + 1 < g.size(); j++)
			if(g[j + 1].b0 - g[j].b1 < g[best + 1].b0 - g[best].b1) best = j;
		g[best].b1 = g[best + 1].b1; g[best].a += g[best + 1].a;
		g.erase(g.begin() + best + 1);
	}
	if(g.size() < 2)
		return 1;
	for(size_t j = 0; j < g.size(); j++)
	{
		m_mapCapLevelLo[j] = yMin + g[j].b0 * B;
		m_mapCapLevelHi[j] = yMin + (g[j].b1 + 1) * B;
	}
	return (int)g.size();
}
