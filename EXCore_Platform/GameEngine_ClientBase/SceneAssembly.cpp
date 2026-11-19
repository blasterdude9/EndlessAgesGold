#include "StdAfx.h"
#include "sceneassembly.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

SceneAssembly::SceneAssembly(GlobalStatusVariables * gStatus)
{
    m_gStatus						= gStatus;
	GL_objectsFromTable				= 0;
	m_showAllNames                  = TRUE;
	m_groupFilterCount              = 0;
	m_triggerDelayStamp             = GetTickCount();
	m_laserPointer					= NULL;
	m_false                         = FALSE;
	m_true                          = FALSE;   
	m_proceedTemp                   = FALSE;
	m_mirrorBased					= 0;
	m_compoundMatrix				= NULL;
	
	ZeroGroupFilters();
	
}

SceneAssembly::~SceneAssembly(void)
{
	if(m_laserPointer)
	{
       m_laserPointer->SafeDelete();
	   delete m_laserPointer;
	   m_laserPointer = 0;
	}

	m_refSkeletalResetList.RemoveAll();
}

void SceneAssembly::ZeroGroupFilters()
{
     ZeroMemory(m_groupFilterArray,(sizeof(int)*2048));
}
//----------------------------------------------------------//
BOOL SceneAssembly::GeneratePopoutList(     D3DVECTOR pointLocation,
											D3DMATRIX camMatrix,
											D3DMATRIX invCamGeneratedMatrix,
											BOOL * renderEnviorment,
											DWORD * timeTaken,
											int mirrorBased,
											float fovView,
											float viewWidth,
											float viewHieght,
											float heightAspect,
											D3DMATRIX & fovMatrix,
											D3DMATRIX * compoundMatrix
											)
{
ZeroGroupFilters();
m_compoundMatrix = compoundMatrix;
pointLocation.x = camMatrix._41;
pointLocation.y = camMatrix._42;
pointLocation.z = camMatrix._43;

m_mirrorBased = mirrorBased;
CReferenceObj * referenceObject = NULL;
m_gStatus->GL_renderEnvironment = TRUE;//reset    
float screenScalar = m_gStatus->m_dxRenderer->GetWidthAspectMultiplier();
if(m_gStatus->m_forceScalarForAspect != 0.0f)
   screenScalar = m_gStatus->m_forceScalarForAspect;
long currentTime = GetTickCount();
D3DMATRIX camGeneratedMatrix = camMatrix;
D3DVECTOR camPosXYZ,focusPoint;
camPosXYZ.x = camGeneratedMatrix._41;
camPosXYZ.y = camGeneratedMatrix._42;
camPosXYZ.z = camGeneratedMatrix._43;
D3DMATRIX genMatrix;
D3DMATRIX positionMatrix,shadowApplicationDiff;
int m_prevLodTemp = -2;


 //----WORLD OBJECTS----//
 //---------------------//
 DWORD curTimeCollision = GetTickCount();///point 1

 if(!m_gStatus->m_collisionBase)
 return FALSE;
 if(!m_gStatus->m_collisionBase->boxCollection)
 return FALSE;

 m_gStatus->CacheGeneratedRenderStuff(mirrorBased);//clears persistent references
 m_gStatus->m_lodBalancer.Reset();//resets lod allowanced back to maximums that are allowed for new frame render
 m_refSkeletalResetList.FlagAllForAnimUpdate();

 POSITION prtPos;
 for(prtPos = m_gStatus->m_curWldObjPrtList->GetHeadPosition();prtPos != NULL;)
 {//set world particle sys to non regen to flag it
     CWldObject * wldObj = (CWldObject *)m_gStatus->m_curWldObjPrtList->GetNext(prtPos);
     if(wldObj->m_particleEmmiter)
	    wldObj->m_particleEmmiter->m_Regenerate = FALSE;
 }//end set world particle sys to non regen to flag it
 RenderTempListPreviews(camGeneratedMatrix,
						invCamGeneratedMatrix,
						pointLocation,
						camMatrix,
						fovView);

 //aquire selected objects
 
 *renderEnviorment = TRUE;
 int Xpos,Ypos,Zpos;
 D3DVECTOR viewPoint;
 D3DVECTOR center,transformedCenter;
 int oldXpos = -1;
 int oldYpos = -1;
 int oldZpos = -1;
 int oldCurSubLevel=-1;
//vertex one
 int collisionCheckCount = 0;
 int collisionConsideration = 0;
 int CurSubLevel;
 float minX,oldMinX;
 float minY,oldMinY;
 float minZ,oldMinZ;
 REFBOX ***CurrentSubBox;
 REFBOX *GoodSubBox;

 oldMinX = m_gStatus->m_collisionBase->BaseBox.min.x - 1.0f;
 oldMinY = m_gStatus->m_collisionBase->BaseBox.min.y - 1.0f;
 oldMinZ = m_gStatus->m_collisionBase->BaseBox.min.z - 1.0f;
 
 int temptemp = 1;
 if(m_gStatus->m_logRenderSection)
 {
	 if(m_gStatus->m_soundManager->m_enableLogging == FALSE)
		m_gStatus->m_soundManager->EnableLogger("SoundsPlayed.Txt");
	 else
		m_gStatus->m_soundManager->DisableLogger();
 }

  
  if((m_gStatus->m_mapCaptureAll && m_gStatus->m_wldObjList) || m_gStatus->m_collisionBase->BoundBoxPointCheck(pointLocation,m_gStatus->m_collisionBase->BaseBox.min,m_gStatus->m_collisionBase->BaseBox.max)==TRUE)
  {//if in bounds
   if(m_gStatus->m_mapCaptureAll && m_gStatus->m_wldObjList)
   {//map capture: use every world object instead of just this cell's list
	   static REFBOX s_allBox;
	   static WORD * s_allList = NULL;
	   static int s_allCap = 0;
	   int n = m_gStatus->m_wldObjList->GetCount();
	   if(n > 65535) n = 65535;
	   if(n > s_allCap)
	   {
		   if(s_allList) delete [] s_allList;
		   s_allList = new WORD[n];
		   for(int i = 0; i < n; i++) s_allList[i] = (WORD)i;
		   s_allCap = n;
	   }
	   ZeroMemory(&s_allBox, sizeof(s_allBox));
	   s_allBox.objectReferenceCount = (WORD)n;
	   s_allBox.objectReferenceList = s_allList;
	   GoodSubBox = &s_allBox;
   }
   else
   {
    Xpos = m_gStatus->m_collisionBase->ConvertPositionToIndex(pointLocation.x,m_gStatus->m_collisionBase->boxSizeX[0],m_gStatus->m_collisionBase->BaseBox.min.x,m_gStatus->m_collisionBase->SizeSquaredX[0]);
    Ypos = m_gStatus->m_collisionBase->ConvertPositionToIndex(pointLocation.y,m_gStatus->m_collisionBase->boxSizeY[0],m_gStatus->m_collisionBase->BaseBox.min.y,m_gStatus->m_collisionBase->SizeSquaredY[0]);
    Zpos = m_gStatus->m_collisionBase->ConvertPositionToIndex(pointLocation.z,m_gStatus->m_collisionBase->boxSizeZ[0],m_gStatus->m_collisionBase->BaseBox.min.z,m_gStatus->m_collisionBase->SizeSquaredZ[0]);
    CurSubLevel = 0;
    GoodSubBox = &m_gStatus->m_collisionBase->boxCollection[Xpos][Ypos][Zpos];
    CurrentSubBox = m_gStatus->m_collisionBase->boxCollection[Xpos][Ypos][Zpos].SubBoxCollection;
    minX = m_gStatus->m_collisionBase->BaseBox.min.x;
    minY = m_gStatus->m_collisionBase->BaseBox.min.y;
    minZ = m_gStatus->m_collisionBase->BaseBox.min.z;
    while (CurrentSubBox != NULL)
    {

     minX = minX + (Xpos*m_gStatus->m_collisionBase->boxSizeX[CurSubLevel]);
     minY = minY + (Ypos*m_gStatus->m_collisionBase->boxSizeY[CurSubLevel]);
     minZ = minZ + (Zpos*m_gStatus->m_collisionBase->boxSizeZ[CurSubLevel]);
      CurSubLevel++;
     Xpos = m_gStatus->m_collisionBase->ConvertPositionToIndex(pointLocation.x,m_gStatus->m_collisionBase->boxSizeX[CurSubLevel],minX,m_gStatus->m_collisionBase->SizeSquaredX[CurSubLevel]);
     Ypos = m_gStatus->m_collisionBase->ConvertPositionToIndex(pointLocation.y,m_gStatus->m_collisionBase->boxSizeY[CurSubLevel],minY,m_gStatus->m_collisionBase->SizeSquaredY[CurSubLevel]);
     Zpos = m_gStatus->m_collisionBase->ConvertPositionToIndex(pointLocation.z,m_gStatus->m_collisionBase->boxSizeZ[CurSubLevel],minZ,m_gStatus->m_collisionBase->SizeSquaredZ[CurSubLevel]);

	 GoodSubBox = &CurrentSubBox[Xpos][Ypos][Zpos];
     CurrentSubBox = CurrentSubBox[Xpos][Ypos][Zpos].SubBoxCollection;
    }
   }
   //build render list
   GL_objectsFromTable = GoodSubBox->objectReferenceCount;
   //get group filters
   int count=0;
   for(count=0;count<GoodSubBox->objectReferenceCount;count++){//L2
       CWldObject * wldObj = m_gStatus->m_collisionBase->m_wldObjectReferenceArray[GoodSubBox->objectReferenceList[count]].worldObjectReference;
       if(wldObj){
	   if(wldObj->m_filterGroup != -1 && !m_gStatus->m_mapCaptureAll){
		    if(m_gStatus->m_collisionBase->BoundBoxPointCheck(pointLocation,wldObj->List_popoutBox.min,wldObj->List_popoutBox.max)==TRUE)
			AddGroupFilter(wldObj->m_filterGroup);
	   }
	   }
   }//end L2
   //end get group filters
    BOOL proceed = FALSE;
    for(count=0;count<GoodSubBox->objectReferenceCount;count++){//object loop
	   CWldObject * wldObj = m_gStatus->m_collisionBase->m_wldObjectReferenceArray[GoodSubBox->objectReferenceList[count]].worldObjectReference;
	   if(wldObj){//valid
        if(wldObj->m_renderFilter)
		   if(wldObj->m_particleLink == -1)
		      continue;
			
		if(m_groupFilterArray[wldObj->List_groupNumber])
		   continue;
			
	    if(m_gStatus->m_mapCaptureAll || m_gStatus->m_collisionBase->BoundBoxPointCheck(pointLocation,wldObj->List_popoutBox.min,wldObj->List_popoutBox.max)==TRUE)
		{//validity
		proceed = FALSE;

		//------------------------------
	    if(m_gStatus->m_logRenderSection)
		{//LOG SECTION
		   
		   if(wldObj->m_lightObject)
		   {//LIGHT OBJECT
		     CString lightStateDesc = "LIGHT is ON ";
			 if(!wldObj->m_lightObject->m_lightOn)
			     lightStateDesc = "LIGHT is OFF ";
			 if(wldObj->m_meshObject)
			 {
			   LogRenderObj(lightStateDesc+"::LIGHT OBJECT: group:"+m_gStatus->m_cabLib.ConvToString((int)wldObj->List_groupNumber),0,0,0); 
			 }
		     else if(!wldObj->m_node)
			 {//not node
               if(wldObj->m_lightObject->m_lightOn)
			   {
                  float distanceToCam = m_gStatus->m_matLib.MagnitudeSquared(wldObj->m_lightObject->m_wldspacePos.x,wldObj->m_lightObject->m_wldspacePos.y,wldObj->m_lightObject->m_wldspacePos.z,
		                                                                     camMatrix._41,camMatrix._42,camMatrix._43);
				  LogRenderObj(lightStateDesc+"::LIGHT OBJECT: Dist: "+m_gStatus->m_cabLib.FloatToStingCnv(distanceToCam)+" group:"+m_gStatus->m_cabLib.ConvToString((int)wldObj->List_groupNumber),wldObj->m_lightObject->m_wldspacePos); 

			   }

               if(IsPointInView(wldObj->m_lightObject->m_wldspacePos,camGeneratedMatrix))
		       {//in fulstrum
		          if(wldObj->m_charona)
		             LogRenderObj(wldObj->m_fileNameRef+"::CHARONA :group:"+m_gStatus->m_cabLib.ConvToString((int)wldObj->List_groupNumber),0,0,0); 

                  if(wldObj->m_particleLink > -1)
				     LogRenderObj(wldObj->m_fileNameRef+"::PARTICLE EMITTER :group:"+m_gStatus->m_cabLib.ConvToString((int)wldObj->List_groupNumber),0,0,0); 
			   }
			 }//end not node
		   }//END LIGHT OBJECT
		}//END LOG SECTION
	    //-------------------------------
         
		if(wldObj->List_removeEnviorment && !m_gStatus->m_mapCaptureAll)
		{
		    m_gStatus->GL_renderEnvironment = FALSE;
            *renderEnviorment = FALSE;
		}
     
		if(wldObj->List_triggerList)
		if(wldObj->List_triggerList->GetCount() != 0)
		   m_gStatus->m_worldTriggerList->AddTail(wldObj);

		
		if(wldObj->m_animModule)
		   if(wldObj->m_meshObject)
		      wldObj->m_animModule->ApplyAnimation(&wldObj->m_meshObject->m_abVertexArray,m_gStatus->m_3dDevice);

		 if(wldObj->List_environmentAttached > 0)
			proceed=TRUE;

		 if(wldObj->m_lightObject)
		 {//add light
			
			if(wldObj->m_meshObject)
			{
				  if(wldObj->m_lightObject->m_lightOn)
		             m_gStatus->m_runtimeLightSys->AddLight(wldObj->m_lightObject,wldObj->m_meshObject->m_meshCenter);				  
			}
			else if(!wldObj->m_node)
			{//non node
               if(wldObj->m_lightObject->m_lightOn)
			   {
			      m_gStatus->m_runtimeLightSys->AddLight(wldObj->m_lightObject,wldObj->m_lightObject->m_wldspacePos);

			   }
			   
			   if(IsPointInView(wldObj->m_lightObject->m_wldspacePos,camGeneratedMatrix))
			   {//in fulstrum
				   if(wldObj->m_charona)
					  m_gStatus->m_charonaCache->AddCharona(wldObj->m_charona,m_gStatus->m_matrixInit2,wldObj->m_lightObject->m_wldspacePos);


				   if(wldObj->m_particleLink > -1)
				   {//particle expansion
					  if(wldObj->m_particleEmmiter){//prolong existing
						 wldObj->m_particleEmmiter->m_Regenerate = TRUE;
						 wldObj->m_particleEmmiter->m_on = TRUE;
					  }//end prolong existing
					  else{//create new
						 POSITION particlePos = m_gStatus->m_particleDatabase->FindIndex(wldObj->m_particleLink);
						 if(particlePos)
						 {//ref db
								EnvParticleSystem* pSys = (EnvParticleSystem*)m_gStatus->m_particleDatabase->GetAt(particlePos);
								m_gStatus->m_matLib.InitMat(genMatrix.m);
								genMatrix._41 = wldObj->m_lightObject->m_wldspacePos.x;
								genMatrix._42 = wldObj->m_lightObject->m_wldspacePos.y;
								genMatrix._43 = wldObj->m_lightObject->m_wldspacePos.z;
								focusPoint.x = wldObj->m_lightObject->m_wldspacePos.x - wldObj->m_particleDirection.x;
								focusPoint.y = wldObj->m_lightObject->m_wldspacePos.y - wldObj->m_particleDirection.y;
								focusPoint.z = wldObj->m_lightObject->m_wldspacePos.z - wldObj->m_particleDirection.z; 
								m_gStatus->m_matLib.FaceTowardPoint(focusPoint,&genMatrix);
								if(m_compoundMatrix)
								{
								   m_gStatus->m_matLib.fastMatrixMultiply(m_compoundTemp,genMatrix,*m_compoundMatrix);
								   m_compoundCenterTemp =  m_gStatus->m_matLib.UpdatePolygonVertex(m_compoundMatrix->m,wldObj->m_lightObject->m_wldspacePos);
								   m_gStatus->m_translucentPolyList->CacheMesh(  NULL,
																				 pSys,
																				 camGeneratedMatrix,
																				 m_compoundTemp,
																				 m_compoundCenterTemp,
																				 NULL);
								}
								else
								   m_gStatus->m_translucentPolyList->CacheMesh(  NULL,
																				 pSys,
																				 camGeneratedMatrix,
																				 genMatrix,
																				 wldObj->m_lightObject->m_wldspacePos,
																				 NULL);
						 }//end ref db
					  }//end create new
					}//end particle expansion
			   }//end in fulstrum
			}//end non node
			else if(wldObj->m_node)
			{
              
			   
			}
		 }//end add light

		   if(wldObj->List_environmentAttached > 0)
		   {//env attached
              m_gStatus->m_worldEnviormentList->AddTail(wldObj);
              wldObj->List_poppedOut = FALSE;
		   }//end env attached
		   else if(wldObj->m_node)
		   {//node type
              if(!proceed)
              if(!m_frustum.BoundingBoxInView(wldObj->m_node->m_clipBox))
                 continue;
              
			  if(wldObj->m_node->m_refPtr == NULL || m_gStatus->m_resetReferences == TRUE)
                 wldObj->m_node->m_refPtr = m_gStatus->m_libraryReferenceDB->GetByIndex(wldObj->m_node->m_libraryReference);
	    
		      referenceObject = (CReferenceObj *)wldObj->m_node->m_refPtr;

			  if(referenceObject)
			  {//valid reference object
			    if(referenceObject->m_skeletalOption)
				{//skeletal based reference parse
                    //init option-only if hasnt been done before
					if(!referenceObject->m_skeletalOptionLive)
						referenceObject->m_skeletalOption->Clone(&referenceObject->m_skeletalOptionLive,NULL,NULL);
					
					if(referenceObject->m_skeletalOptionLive->m_hasPerPolyCollisionsTemp)
					   m_gStatus->m_skeletalReferenceObjCache.AddTail(wldObj);//add to collision vicinity list

                    m_prevLodTemp = referenceObject->m_skeletalOptionLive->m_currentLOD;
					if(m_gStatus->m_mapCaptureAll)
                       referenceObject->m_skeletalOptionLive->m_currentLOD = 0;
					else if(referenceObject->m_skeletalOptionLive->m_lodRanges)
                       referenceObject->m_skeletalOptionLive->m_currentLOD = referenceObject->m_skeletalOptionLive->m_lodRanges->GetLod(camMatrix,wldObj->m_node->m_position);

					   if(referenceObject->m_animationUpdatedThisRound != currentTime || m_prevLodTemp != referenceObject->m_skeletalOptionLive->m_currentLOD)
					   {//update only one time per rally
						 m_tempCenter.x = wldObj->m_node->m_position._41;
						 m_tempCenter.y = wldObj->m_node->m_position._42;
						 m_tempCenter.z = wldObj->m_node->m_position._43;
						 referenceObject->m_skeletalOptionLive->SetCurrentAnimation(0,175);
						 referenceObject->m_skeletalOptionLive->AnimationCallback(  currentTime,
																				m_gStatus->m_soundCenterMatrix,
																				m_tempCenter,
																				m_gStatus->GL_globalSoundFactor,
																				FALSE,
																				m_gStatus->m_soundManager,
																				FALSE,
																				m_gStatus->m_multiplayerObject->m_AIServer,
																				m_gStatus->m_3dDevice,
																				m_gStatus->m_d3dVectorInit,
																				m_gStatus->m_oneVal,
																				m_gStatus->m_lodOverride
																				);
						  //if(referenceObject->m_skeletalOptionLive->m_currentLOD == 0)
							 referenceObject->m_animationUpdatedThisRound = currentTime;
							 if(referenceObject->m_skeletalOptionLive->m_currentLOD == 0)
							 if(referenceObject->m_skeletalOptionLive->m_hasPerPolyCollisionsTemp)
							   m_refSkeletalResetList.AddTail(referenceObject);//adds one time per rally for reset anim state on generatepopoutlist start
					   }//end update only one time per rally 
					   //else if(m_prevLodTemp != referenceObject->m_skeletalOptionLive->m_currentLOD)
							//referenceObject->m_skeletalOptionLive->UpdateBasedDeformationByVertexAssignments(referenceObject->m_skeletalOptionLive->m_curLod,m_gStatus->m_3dDevice);


                      if(m_gStatus->m_logRenderSection)
 					  {
					     D3DVECTOR temp;
						 temp.x = wldObj->m_node->m_position._41;
						 temp.y = wldObj->m_node->m_position._42;
						 temp.z = wldObj->m_node->m_position._43;
                         LogRenderObj("Skeletal Ref POPPED IN:"+referenceObject->m_referenceName,temp);
					  }

					  SkeletalDeformableMeshExtraction(referenceObject->m_skeletalOptionLive,
														wldObj->m_node->m_position,
														m_false,
														camMatrix,
														//ViewPort,
														m_false,
														m_false,
														invCamGeneratedMatrix,
														shadowApplicationDiff,
														m_false,
														m_mirrorBased,
														m_compoundMatrix,
														m_gStatus->m_translucentPolyList,
									                    m_gStatus->m_renderContainerList
														);

					 SkeletalHiarcleMeshExtraction(referenceObject->m_skeletalOptionLive,
												   wldObj->m_node->m_position,
												   m_false,
												   camMatrix,
												   m_false,
												   m_false,
												   //ViewPort,
												   invCamGeneratedMatrix,
												   shadowApplicationDiff,
												   m_true,
												   m_mirrorBased,
												   m_compoundMatrix,
												   m_gStatus->m_translucentPolyList,
									               m_gStatus->m_renderContainerList);
				}//end skeletal based reference parse
				else
				{//--Mesh based reference parse---//
				 if(wldObj->m_lightObject)
				 {//add light
			        if(referenceObject->m_meshObject)
					{
					   D3DVECTOR lightCenter;
				       lightCenter.x = referenceObject->m_meshObject->m_meshCenter.x + wldObj->m_node->m_position._41;
				       lightCenter.y = referenceObject->m_meshObject->m_meshCenter.y + wldObj->m_node->m_position._42;
				       lightCenter.z = referenceObject->m_meshObject->m_meshCenter.z + wldObj->m_node->m_position._43; 
		               m_gStatus->m_runtimeLightSys->AddLight(wldObj->m_lightObject,
									               lightCenter);
					}
				 }//end add light
				 



				 center.x = wldObj->m_node->m_position._41;
				 center.y = wldObj->m_node->m_position._42;
				 center.z = wldObj->m_node->m_position._43;
				 transformedCenter = m_gStatus->m_matLib.UpdatePolygonVertex(wldObj->m_node->m_position.m,referenceObject->m_meshObject->m_meshCenter);
				 if( referenceObject->m_lodVisualList && (!m_gStatus->m_mapCaptureAll || !referenceObject->m_meshObject) )
				 {//LOD sys exists
                    
					 if(referenceObject->m_lodVisualList->m_lodType == 0)
					 {//range based 
                      viewPoint.x = camGeneratedMatrix._41;
					  viewPoint.y = camGeneratedMatrix._42;
					  viewPoint.z = camGeneratedMatrix._43;
					 }//end range based
					 else if(referenceObject->m_lodVisualList->m_lodType == 1)
					 {//bounding box based
					  viewPoint.x = camGeneratedMatrix._41 - wldObj->m_node->m_position._41;
					  viewPoint.y = camGeneratedMatrix._42 - wldObj->m_node->m_position._42;
					  viewPoint.z = camGeneratedMatrix._43 - wldObj->m_node->m_position._43;
					 }//end bounding box based
				     else
					 {//overlap style
					  viewPoint.x = camGeneratedMatrix._41;
					  viewPoint.y = camGeneratedMatrix._42;
					  viewPoint.z = camGeneratedMatrix._43;
					 }//end overlap style
			  
					  float alphaTest1 = 1.0f;
					  float alphaTest2 = 1.0f;
					  CEXMeshObj * mObjSecond = NULL; 
					  
					  
                      CEXMeshObj * mObj = referenceObject->m_lodVisualList->GetLodVisualExt(viewPoint,center,fovView,&mObjSecond,&alphaTest1,&alphaTest2,m_gStatus->m_lodOverride);
					  if(mObjSecond)
					  {
					     mObjSecond->m_currentMatrix = wldObj->m_node->m_position;
						 mObjSecond->List_materialObject->m_useMaterial.Diffuse.a = alphaTest1;
						 mObjSecond->List_materialObject->m_baseMaterial.Diffuse.a = alphaTest1;
						 if(!wldObj->m_renderFilter)
					         CatigorizeMesh(mObjSecond,
											camMatrix,
											transformedCenter,
											invCamGeneratedMatrix,
											center,
											referenceObject,
											m_gStatus->m_translucentPolyList,
											m_gStatus->m_renderContainerList,
											m_mirrorBased,
											m_compoundMatrix);
					
					  }
					  if(mObj){//valid LOD
					     mObj->m_currentMatrix = wldObj->m_node->m_position;
						 mObj->List_materialObject->m_useMaterial.Diffuse.a = alphaTest2;
						 mObj->List_materialObject->m_baseMaterial.Diffuse.a = alphaTest2;
                         //if(m_logRenderSection){//LOG tracer render snapshot
                         //     LogRenderObj(wldObj->m_fileNameRef,mObj->m_abVertexArray.m_indecieCount/3);
						 //}//end LOG tracer render snapshot
						 if(referenceObject->m_faceCamera)
						    m_gStatus->m_matLib.FaceTowardPoint(camPosXYZ,&mObj->m_currentMatrix);

						 if(!wldObj->m_renderFilter)
						     CatigorizeMesh(mObj,
											camMatrix,
											transformedCenter,
											invCamGeneratedMatrix,
											center,
											referenceObject,
											m_gStatus->m_translucentPolyList,
											m_gStatus->m_renderContainerList,
											m_mirrorBased,
											m_compoundMatrix);
					  }//end valid LOD
					  else{//invalalid lod add standard
                      referenceObject->m_meshObject->m_currentMatrix = wldObj->m_node->m_position;
                      
					  //if(m_logRenderSection){//LOG tracer render snapshot
                      //        LogRenderObj(referenceObject->m_referenceName,referenceObject->m_meshObject->m_abVertexArray.m_indecieCount/3);
					  //}//end LOG tracer render snapshot
					  if(referenceObject->m_faceCamera)
						 m_gStatus->m_matLib.FaceTowardPoint(camPosXYZ,&referenceObject->m_meshObject->m_currentMatrix);
					  
                      referenceObject->m_meshObject->List_materialObject->m_useMaterial.Diffuse.a = alphaTest2;
					  referenceObject->m_meshObject->List_materialObject->m_baseMaterial.Diffuse.a = alphaTest2;

					  if(!wldObj->m_renderFilter)
					      CatigorizeMesh(referenceObject->m_meshObject,
										camMatrix,
										transformedCenter,
										invCamGeneratedMatrix,
										center,
										referenceObject,
										m_gStatus->m_translucentPolyList,
										m_gStatus->m_renderContainerList,
										m_mirrorBased,
										m_compoundMatrix);
					  }//end invalid lod add standard
                 }//end LOD sys exists
				 else
				 {//standard implement
				   referenceObject->m_meshObject->m_currentMatrix = wldObj->m_node->m_position;
				   if(referenceObject->m_faceCamera)
				    m_gStatus->m_matLib.FaceTowardPoint(camPosXYZ,&referenceObject->m_meshObject->m_currentMatrix);
				   if(!wldObj->m_renderFilter)
                    CatigorizeMesh(referenceObject->m_meshObject,
						           camMatrix,
						           transformedCenter,
						           invCamGeneratedMatrix,
								   center,
								   referenceObject,
								   m_gStatus->m_translucentPolyList,
								   m_gStatus->m_renderContainerList,
								   m_mirrorBased,
								   m_compoundMatrix);
				 }//end standard implement
				}//--Mesh based reference parse---//
			  }//end valid reference object
		   }//end node type
		   else if(wldObj->m_meshObject)
		   {//regular objects
			 if(wldObj->m_meshObject->m_charona)
			    m_gStatus->m_charonaCache->AddCharona(wldObj->m_meshObject->m_charona,wldObj->m_meshObject->m_currentMatrix,wldObj->m_meshObject->m_meshCenter);


				   if(wldObj->m_lodVisualList && (!m_gStatus->m_mapCaptureAll || !wldObj->m_meshObject))
				   {//LOD list exists
					  viewPoint.x = camGeneratedMatrix._41;
					  viewPoint.y = camGeneratedMatrix._42;
					  viewPoint.z = camGeneratedMatrix._43;
					  CEXMeshObj * mObj = NULL;
					  mObj = (CEXMeshObj *)wldObj->m_lodVisualList->GetLodVisual(viewPoint,wldObj->m_meshObject->m_meshCenter,fovView,m_gStatus->m_lodOverride);
                      if(mObj)
					  {
						   //if(!proceed) 
						   if(!IsMeshInView(mObj,camGeneratedMatrix))
						   {
						       continue;
						   }
						   ManageEnvEffects(wldObj,mObj);
						   if(wldObj->m_particleLink > -1){//particle expansion
						         if(wldObj->m_particleEmmiter){//prolong existing
						            wldObj->m_particleEmmiter->m_Regenerate = TRUE;
									wldObj->m_particleEmmiter->m_on = TRUE;
								 }//end prolong existing
								 else{//create new
                                   POSITION particlePos = m_gStatus->m_particleDatabase->FindIndex(wldObj->m_particleLink);
			                       if(particlePos){//ref db
			                        EnvParticleSystem* pSys = (EnvParticleSystem*)m_gStatus->m_particleDatabase->GetAt(particlePos);
                                    m_gStatus->m_matLib.InitMat(genMatrix.m);
				                    genMatrix._41 = wldObj->m_meshObject->m_meshCenter.x;
				                    genMatrix._42 = wldObj->m_meshObject->m_meshCenter.y;
				                    genMatrix._43 = wldObj->m_meshObject->m_meshCenter.z;
				                    focusPoint.x = wldObj->m_meshObject->m_meshCenter.x - wldObj->m_particleDirection.x;
				                    focusPoint.y = wldObj->m_meshObject->m_meshCenter.y - wldObj->m_particleDirection.y;
				                    focusPoint.z = wldObj->m_meshObject->m_meshCenter.z - wldObj->m_particleDirection.z; 
				                    m_gStatus->m_matLib.FaceTowardPoint(focusPoint,&genMatrix);
									if(m_compoundMatrix)
									{
									   m_gStatus->m_matLib.fastMatrixMultiply(m_compoundTemp,genMatrix,*m_compoundMatrix);
									   m_compoundCenterTemp =  m_gStatus->m_matLib.UpdatePolygonVertex(m_compoundMatrix->m,wldObj->m_meshObject->m_meshCenter);
									   m_gStatus->m_translucentPolyList->CacheMesh(  NULL,
																					 pSys,
																					 camGeneratedMatrix,
																					 m_compoundTemp,
																					 m_compoundCenterTemp,
																					 NULL);
									}
									else
									   m_gStatus->m_translucentPolyList->CacheMesh( NULL,
																				 pSys,
																				 camGeneratedMatrix,
																				 genMatrix,
																				 wldObj->m_meshObject->m_meshCenter,
																				 NULL);
									
								   }//end ref db
								 }//end create new
						   }//end particle expansion
						   if(!wldObj->m_renderFilter)
						       CatigorizeMesh(  mObj,
												camMatrix,
												wldObj->m_meshObject->m_meshCenter,
												invCamGeneratedMatrix,
												wldObj->m_meshObject->m_meshCenter,
												NULL,
												m_gStatus->m_translucentPolyList,
												m_gStatus->m_renderContainerList,
												m_mirrorBased,
												m_compoundMatrix);
					  }
					  else
					  {//has lod list but no lod in use
						    //if(!proceed)
						    if(!IsMeshInView(wldObj->m_meshObject,camGeneratedMatrix))
							{
						       continue;
							}
							ManageEnvEffects(wldObj,wldObj->m_meshObject);
                             if(wldObj->m_particleLink > -1){//particle expansion
						         if(wldObj->m_particleEmmiter){//prolong existing
						            wldObj->m_particleEmmiter->m_Regenerate = TRUE;
									wldObj->m_particleEmmiter->m_on = TRUE;
								 }//end prolong existing
								 else{//create new
                                   POSITION particlePos = m_gStatus->m_particleDatabase->FindIndex(wldObj->m_particleLink);
			                       if(particlePos){//ref db
			                        EnvParticleSystem* pSys = (EnvParticleSystem*)m_gStatus->m_particleDatabase->GetAt(particlePos);
                                    m_gStatus->m_matLib.InitMat(genMatrix.m);
				                    genMatrix._41 = wldObj->m_meshObject->m_meshCenter.x;
				                    genMatrix._42 = wldObj->m_meshObject->m_meshCenter.y;
				                    genMatrix._43 = wldObj->m_meshObject->m_meshCenter.z;
				                    focusPoint.x = wldObj->m_meshObject->m_meshCenter.x - wldObj->m_particleDirection.x;
				                    focusPoint.y = wldObj->m_meshObject->m_meshCenter.y - wldObj->m_particleDirection.y;
				                    focusPoint.z = wldObj->m_meshObject->m_meshCenter.z - wldObj->m_particleDirection.z; 
									m_gStatus->m_matLib.FaceTowardPoint(focusPoint,&genMatrix);
									if(m_compoundMatrix)
									{
									   m_gStatus->m_matLib.fastMatrixMultiply(m_compoundTemp,genMatrix,*m_compoundMatrix);
									   m_compoundCenterTemp =  m_gStatus->m_matLib.UpdatePolygonVertex(m_compoundMatrix->m,wldObj->m_meshObject->m_meshCenter);
									   m_gStatus->m_translucentPolyList->CacheMesh(  NULL,
																					 pSys,
																					 camGeneratedMatrix,
																					 m_compoundTemp,
																					 m_compoundCenterTemp,
																					 NULL);
									}
									else
									   m_gStatus->m_translucentPolyList->CacheMesh( NULL,
																				    pSys,
																				    camGeneratedMatrix,
																				    genMatrix,
																				    wldObj->m_meshObject->m_meshCenter,
																				    NULL);
									
									
								   }//end ref db
								 }//end create new
							 }//end particle expansion
						    if(!wldObj->m_renderFilter)
						        CatigorizeMesh(   wldObj->m_meshObject,
												  camMatrix,
												  wldObj->m_meshObject->m_meshCenter,
												  invCamGeneratedMatrix,
												  wldObj->m_meshObject->m_meshCenter,
												  NULL,
												  m_gStatus->m_translucentPolyList,
												  m_gStatus->m_renderContainerList,
												  m_mirrorBased,
												  m_compoundMatrix);
					  }//has lod list but no lod in use

				   }//end LOD list exists
				   else{
					     //if(!proceed)
						 //if(!wldObj->m_renderFilter)
				         if(!IsMeshInView(wldObj->m_meshObject,camGeneratedMatrix))
						 {
						
					        continue;
						 }
                         ManageEnvEffects(wldObj,wldObj->m_meshObject);
                           if(wldObj->m_particleLink > -1){//particle expansion
						         if(wldObj->m_particleEmmiter){//prolong existing
						            wldObj->m_particleEmmiter->m_Regenerate = TRUE;
									wldObj->m_particleEmmiter->m_on = TRUE;
								 }//end prolong existing
								 else{//create new
                                   POSITION particlePos = m_gStatus->m_particleDatabase->FindIndex(wldObj->m_particleLink);
			                       if(particlePos){//ref db
			                        EnvParticleSystem* pSys = (EnvParticleSystem*)m_gStatus->m_particleDatabase->GetAt(particlePos);
                                    m_gStatus->m_matLib.InitMat(genMatrix.m);
				                    genMatrix._41 = wldObj->m_meshObject->m_meshCenter.x;
				                    genMatrix._42 = wldObj->m_meshObject->m_meshCenter.y;
				                    genMatrix._43 = wldObj->m_meshObject->m_meshCenter.z;
				                    focusPoint.x = wldObj->m_meshObject->m_meshCenter.x - wldObj->m_particleDirection.x;
				                    focusPoint.y = wldObj->m_meshObject->m_meshCenter.y - wldObj->m_particleDirection.y;
				                    focusPoint.z = wldObj->m_meshObject->m_meshCenter.z - wldObj->m_particleDirection.z; 
									m_gStatus->m_matLib.FaceTowardPoint(focusPoint,&genMatrix);
                                    if(m_compoundMatrix)
									{
									   m_gStatus->m_matLib.fastMatrixMultiply(m_compoundTemp,genMatrix,*m_compoundMatrix);
									   m_compoundCenterTemp =  m_gStatus->m_matLib.UpdatePolygonVertex(m_compoundMatrix->m,wldObj->m_meshObject->m_meshCenter);
									   m_gStatus->m_translucentPolyList->CacheMesh(  NULL,
																					 pSys,
																					 camGeneratedMatrix,
																					 m_compoundTemp,
																					 m_compoundCenterTemp,
																					 NULL);
									}
									else
									   m_gStatus->m_translucentPolyList->CacheMesh( NULL,
																				    pSys,
																				    camGeneratedMatrix,
																				    genMatrix,
																				    wldObj->m_meshObject->m_meshCenter,
																				    NULL);
								   }//end ref db
								 }//end create new
						   }//end particle expansion
			       if(!wldObj->m_renderFilter)
				   CatigorizeMesh(wldObj->m_meshObject,
								  camMatrix,
								  wldObj->m_meshObject->m_meshCenter,
								  invCamGeneratedMatrix,
								  wldObj->m_meshObject->m_meshCenter,
								  NULL,
								  m_gStatus->m_translucentPolyList,
								  m_gStatus->m_renderContainerList,
								  m_mirrorBased,
								  m_compoundMatrix);
				   }
				//}
		   }//end regular objects
		}//validity
	   }//end valid
	}//object loop
  }//end if in bounds

  POSITION prtPosLast;
  for(prtPos = m_gStatus->m_curWldObjPrtList->GetHeadPosition();(prtPosLast = prtPos) != NULL;)
  {//remove link if still set to non regen
     CWldObject * wldObj = (CWldObject *)m_gStatus->m_curWldObjPrtList->GetNext(prtPos);
     if(wldObj->m_particleEmmiter)
	 {
	    if(wldObj->m_particleEmmiter->m_Regenerate == FALSE)
		{
		   wldObj->m_particleEmmiter->m_on = FALSE;
           wldObj->m_particleEmmiter = 0;//unlink this cause its gunna auto delete
		   m_gStatus->m_curWldObjPrtList->RemoveAt(prtPosLast);
		}
	 }
  }//end remove link if still set to non regen*/
  //---------------------//
  //--Environment object-//
  //---------------------//
  if(m_gStatus->m_environmentPtr->m_enviormentTimeSystem)
  {
	//post sun visual
   if(m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState)
   if(m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_sunColorRed   != 0.0f ||
	  m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_sunColorGreen != 0.0f ||
	  m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_sunColorBlue  != 0.0f)
	if(m_gStatus->GL_renderEnvironment)
	if(m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_visual)
	{//s visual

	 m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentSunPosition;
     m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_visual->m_currentMatrix = m_gStatus->m_matrixInit2;
     
	 m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_visual->m_meshCenter.x = m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentSunPosition.x + camMatrix._41;
     m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_visual->m_meshCenter.y = m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentSunPosition.y + camMatrix._42;
	 m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_visual->m_meshCenter.z = m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentSunPosition.z + camMatrix._43;


	 if(m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_visual->m_charona)
	    m_gStatus->m_charonaCache->AddCharona(m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_visual->m_charona,m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_visual->m_currentMatrix,m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_visual->m_meshCenter);
     //m_gStatus->m_visualsRenderList->AddTail(m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_visual);
	}//
  }

	//--Cursor exp effect--//
	if(m_gStatus->m_tgSys_expQue > -1)
	{//effect parse
		D3DVECTOR groundOriPosition;
		groundOriPosition.x = m_gStatus->m_tgSys_latestWldIntersection.x + m_gStatus->m_tgSys_latestNormal.x;
		groundOriPosition.y = m_gStatus->m_tgSys_latestWldIntersection.y + m_gStatus->m_tgSys_latestNormal.y;
		groundOriPosition.z = m_gStatus->m_tgSys_latestWldIntersection.z + m_gStatus->m_tgSys_latestNormal.z;
		m_gStatus->m_explosionDBList->CreateExplosion( m_gStatus->m_tgSys_expQue,
													m_gStatus->m_tgSys_latestWldIntersection,
													m_gStatus->m_explosionRuntimeList,
													groundOriPosition,
													-1,
													m_gStatus->m_3dDevice,
													-1,
													-1
													); 
		m_gStatus->m_tgSys_expQue = -1;

	}//end effect parse
	//--Explosion objects--//
	ParseExplosionObjects(camGeneratedMatrix,
						  camMatrix,
						  invCamGeneratedMatrix);
	//---------------------//
	//--Missile Objects----//
	//---------------------//
	MissileParse(camMatrix,invCamGeneratedMatrix);
	//---------------------//
	//--temp text effects--//
	//---------------------//
	m_gStatus->m_matLib.InitMat(m_gStatus->m_textEffectRuntime->m_curMatrix.m);
	POSITION posLast;
	for(POSITION posTempText = m_gStatus->m_textEffectRuntime->GetHeadPosition();(posLast = posTempText) != NULL;)
	{
        CTextEffectClass * tPtr = (CTextEffectClass *) m_gStatus->m_textEffectRuntime->GetNext(posTempText);
        if(GetTickCount()-tPtr->m_timeStampStart > tPtr->m_duration)
		{
           delete tPtr;
		   tPtr = 0;
           m_gStatus->m_textEffectRuntime->RemoveAt(posLast);
		   continue;
		}

        m_gStatus->m_textEffectRuntime->m_curMatrix._41 = tPtr->m_spawnPos.x;
		m_gStatus->m_textEffectRuntime->m_curMatrix._42 = tPtr->m_spawnPos.y;
		m_gStatus->m_textEffectRuntime->m_curMatrix._43 = tPtr->m_spawnPos.z;
		if(m_compoundMatrix)
		{
		   m_gStatus->m_matLib.fastMatrixMultiply(m_compoundTemp,m_gStatus->m_textEffectRuntime->m_curMatrix,*m_compoundMatrix);
           m_compoundCenterTemp =  m_gStatus->m_matLib.UpdatePolygonVertex(m_compoundMatrix->m,tPtr->m_spawnPos);
		   m_gStatus->m_translucentPolyList->CacheMesh(  tPtr->m_mesh,
														 NULL,
														 camGeneratedMatrix,
														 m_compoundTemp,
														 m_compoundCenterTemp,
														 NULL);
		}
		else
		   m_gStatus->m_translucentPolyList->CacheMesh(  tPtr->m_mesh,
														 NULL,
														 camGeneratedMatrix,
														 m_gStatus->m_textEffectRuntime->m_curMatrix,
														 tPtr->m_spawnPos,
														 NULL);
	}
	//---------------------//
	//---ENTITY OBJECTS----//
	//---------------------//
	m_gStatus->m_inViewEntityList->SafeDelete();
	m_gStatus->m_selectionEffectDB->Reset();
	ABBOX bndBox;
	float adViewVar = fovView/.68f;//precalc
	float distanceToCam = 0.0f;
	D3DVECTOR textDisplayPos,positionScreen;
    float multiplier;
    BOOL firstPersonModeTemp = 0;
	for(POSITION posloc = m_gStatus->m_entObjRuntime->GetHeadPosition();posloc != NULL;)
	{//entity loop
		CMovementObj * movObj = (CMovementObj *)m_gStatus->m_entObjRuntime->GetNext(posloc);
		
		if(mirrorBased != 1)
            firstPersonModeTemp = movObj->m_firstPersonMode;

		movObj->List_baseFrame->GetTransform(NULL,&positionMatrix);
		center.x = positionMatrix._41;
		center.y = positionMatrix._42;
		center.z = positionMatrix._43;
		bndBox.maxX = movObj->List_boundingRadius;
		bndBox.maxY = movObj->List_boundingRadius;
		bndBox.maxZ = movObj->List_boundingRadius;
		bndBox.minX = -movObj->List_boundingRadius;
		bndBox.minY = -movObj->List_boundingRadius;
		bndBox.minZ = -movObj->List_boundingRadius;
		movObj->List_poppedOut = TRUE;

		distanceToCam = m_gStatus->m_matLib.MagnitudeSquared(center.x,center.y,center.z,camMatrix._41,camMatrix._42,camMatrix._43);
	    movObj->m_distanceToCamTemp = distanceToCam;
		distanceToCam = distanceToCam * adViewVar;
		movObj->QuerryLOD(distanceToCam);
		//at this point the lod load balancer will decide weather the lod will be used or downgraded
		if(movObj->List_controlType != 0)
		  if(!movObj->m_filterLodBalancing)
		  {//dont load balance main player lod
		     m_gStatus->m_lodBalancer.TranslateLOD(movObj->List_skeletalObject->m_currentLOD);//load balance lod
             movObj->m_currentLOD = movObj->List_skeletalObject->m_currentLOD;
		  }

		if(distanceToCam < movObj->List_popoutRadius)
		if(OutOfFovUntransformed(positionMatrix,bndBox,camGeneratedMatrix)==FALSE || firstPersonModeTemp == TRUE)
		{//popped in 
		if(movObj->m_showShadow > 0)
			CachEntityForShadow(movObj);

		if(m_gStatus->m_logRenderSection)
			LogRenderObj("ENTITY POPPED IN:"+movObj->List_name,center);

	    //selection effect system
		if(movObj->m_selectionEffectState > -1)
		{
			 CSelectionEffect * effect = m_gStatus->m_selectionEffectDB->GetByIndex(movObj->m_selectionEffectState);
			 if(effect)
			 {
			 
				 if(!effect->m_usedThisRound)
				 {
					 effect->m_skeletalPtr->SetCurrentAnimation(0,175);
					 effect->m_skeletalPtr->AnimationCallback(  currentTime,
																m_gStatus->m_soundCenterMatrix,
																center,
																m_gStatus->GL_globalSoundFactor,
																FALSE,
																m_gStatus->m_soundManager,
																FALSE,
																m_gStatus->m_multiplayerObject->m_AIServer,
																m_gStatus->m_3dDevice,
																m_gStatus->m_d3dVectorInit,
																m_gStatus->m_oneVal,
																m_gStatus->m_lodOverride
																);
				 }
				 
				 SkeletalDeformableMeshExtraction(effect->m_skeletalPtr,
												  positionMatrix,
												  m_false,
												  camMatrix,
												  m_false,
												  m_false,
												  invCamGeneratedMatrix,
												  shadowApplicationDiff,
												  m_false,
												  m_mirrorBased,
												  m_compoundMatrix,
												  m_gStatus->m_translucentPolyList,
									              m_gStatus->m_renderContainerList
												  );

				 SkeletalHiarcleMeshExtraction(effect->m_skeletalPtr,
											   positionMatrix,
											   m_false,
											   camMatrix,
											   m_false,
											   m_false,
											   invCamGeneratedMatrix,
											   shadowApplicationDiff,
											   m_true,
											   m_mirrorBased,
											   m_compoundMatrix,
											   m_gStatus->m_translucentPolyList,
									           m_gStatus->m_renderContainerList);
				 effect->m_usedThisRound = TRUE;
			 }
		}

		//display text on player if requested
		if(m_gStatus->m_showNames)
		if(!mirrorBased)//no names in mirrors
		if(firstPersonModeTemp == FALSE)
		{
			
      
			if(!movObj->m_possedBy)
			{
                

				/*float red	 = 0.5f;
				float green	 = 0.5f;
				float blue	 = 1.0f;*/
				float offset = 0.0f;

				if(movObj->m_murderer)
				{
					m_gStatus->m_nameAboveFontColorR		= 1.0f;
					m_gStatus->m_nameAboveFontColorG	= 0.1f;
					m_gStatus->m_nameAboveFontColorB	= 0.1f;
				}

				if(m_gStatus->m_nameAboveHeadMode == 0)
				{//post rendered text-problem with this is that is constantly is changing the name causeing the mesh to reconstruct 2 times a frame-text will need to support a new line
				    
				}//end post rendered text
				else
				{//3d base text
                  float filterDist = 2.5f;
				  float modY = positionMatrix._42 + movObj->List_boundingRadius;
				  
					  
				  if(!m_gStatus->m_matLib.BoundingSphereCheckX(filterDist,
						                                      filterDist,
															  camPosXYZ.x,
															  camPosXYZ.y,
															  camPosXYZ.z,
															  positionMatrix._41,
															  modY,
															  positionMatrix._43))
				  if(m_gStatus->m_matLib.BoundingSphereCheckX(m_gStatus->m_nameAboveVisibleRange,
						                                      m_gStatus->m_nameAboveVisibleRange,
															  camPosXYZ.x,
															  camPosXYZ.y,
															  camPosXYZ.z,
															  positionMatrix._41,
															  modY,
															  positionMatrix._43))
				  {//in clip range optimization
                    if(!movObj->m_optionalTextMesh)
					{//once
                        if(m_gStatus->m_editorModeOn)//only if editor is on do a test clan display
						   movObj->List_clanName = "[Test Clan]";

                        CString finalDisplayedName = movObj->List_name;
						//if(movObj->List_clanName.GetLength() > 0)
                         // finalDisplayedName = movObj->List_name + "\n" + movObj->List_clanName;
						

						m_gStatus->m_textRenderList->InitializeExternalMesh(m_gStatus->m_3dDevice,
																			finalDisplayedName,
																			m_gStatus->m_nameAboveFontSizeX,
																			m_gStatus->m_nameAboveFontSizeY,
																			m_gStatus->m_symbolMapDB,
																			m_gStatus->m_speechMode,
																			0.5f,//center justified
																			(void**)&movObj->m_optionalTextMesh,
																			2,
																			m_gStatus->m_nameAboveFontColorR,
													                        m_gStatus->m_nameAboveFontColorG,
													                        m_gStatus->m_nameAboveFontColorB,
																			m_gStatus->m_nameAboveNewLineScale);
						//aquire specific font
						POSITION fntPos = m_gStatus->m_cgFontList->FindIndex(0);
	                    if(fntPos)
						{
                           CGFontObj * cg_fntObject = (CGFontObj *)m_gStatus->m_cgFontList->GetAt(fntPos);
						   movObj->m_optionalTextMesh->List_textureSelect[0] = cg_fntObject->m_fontTableIndex;
						}
					}//end once

					//update position and color
                    movObj->m_optionalTextMesh->m_currentMatrix._41 = positionMatrix._41;
					movObj->m_optionalTextMesh->m_currentMatrix._42 = positionMatrix._42 + movObj->List_boundingRadius;
					movObj->m_optionalTextMesh->m_currentMatrix._43 = positionMatrix._43;
					movObj->m_optionalTextMesh->m_meshCenter.x = positionMatrix._41;
					movObj->m_optionalTextMesh->m_meshCenter.y = positionMatrix._42;
					movObj->m_optionalTextMesh->m_meshCenter.z = positionMatrix._43;
					m_gStatus->m_matLib.FaceTowardPoint(camPosXYZ,&movObj->m_optionalTextMesh->m_currentMatrix);

                    if(movObj->m_optionalTextMesh)
                       m_gStatus->m_translucentPolyList->CacheMesh(movObj->m_optionalTextMesh,
				                                                NULL,
									                            camGeneratedMatrix,
									                            movObj->m_optionalTextMesh->m_currentMatrix,
											                    movObj->m_optionalTextMesh->m_meshCenter,
											                    NULL);
				  }//end in clip range optimization
				}//end 3d base text

				//m_optionalTextMesh
			}
		}//end display text on player if requested
		movObj->List_poppedOut = FALSE;  
		m_gStatus->IndexEntityAnimations(movObj,GetTickCount(),FALSE,m_gStatus->m_missileRuntime);
        SkeletalEmitterCallback(movObj->List_skeletalObject,&positionMatrix,camMatrix,m_compoundMatrix,m_gStatus->m_translucentPolyList);
		
        if(movObj->m_renderBodyFilter == 0)
		{//render body objects
			SkeletalDeformableMeshExtraction(movObj->List_skeletalObject,
											positionMatrix,
											firstPersonModeTemp,
											camMatrix,
											m_false,
											m_true,
											invCamGeneratedMatrix,
											shadowApplicationDiff,
											m_true,
											m_mirrorBased,
											m_compoundMatrix,
											m_gStatus->m_translucentPolyList,
											m_gStatus->m_renderContainerList
											);
			//shadow
			if(movObj->List_skeletalObject->m_shadowOn==TRUE)//insert shadow here
			{
					  CEXMeshObj *shadowPtr = NULL;
					  movObj->List_skeletalObject->m_shadowInterface->GetCEXMeshObject(&shadowPtr);
					  if(shadowPtr)
					  {	
					   if(firstPersonModeTemp == FALSE)
							CatigorizeMesh(shadowPtr,
										   camMatrix,
										   center,
										   invCamGeneratedMatrix,
										   center,
										   NULL,
										   m_gStatus->m_translucentPolyList,
										   m_gStatus->m_renderContainerList,
										   m_mirrorBased,
										   m_compoundMatrix);
					  }

			}
			//hiarchl
			SkeletalHiarcleMeshExtraction(	movObj->List_skeletalObject,
											positionMatrix,
											firstPersonModeTemp,
											camMatrix,
											m_false,
											m_true,
											invCamGeneratedMatrix,
											shadowApplicationDiff,
											m_true,
											m_mirrorBased,
											m_compoundMatrix,
											m_gStatus->m_translucentPolyList,
											m_gStatus->m_renderContainerList);
		}//end render body objects
	   //appendages
       if(movObj->List_armedInventoryDB)
		   for(POSITION appendagePos = movObj->List_armedInventoryDB->GetHeadPosition();appendagePos != NULL;)
		   {//1
             CArmedInventoryObj * appendPtr = (CArmedInventoryObj *)movObj->List_armedInventoryDB->GetNext(appendagePos); 
			   if(appendPtr->m_thirdPersonSystem != NULL && movObj->m_renderBodyFilter == 0){//valid third person
				  appendPtr->m_thirdPersonSystem->m_currentLOD = movObj->List_skeletalObject->m_currentLOD;
                  POSITION AppBonePos = NULL;
				  if(movObj->List_skeletalObject->m_bonesList)
				   AppBonePos = movObj->List_skeletalObject->m_bonesList->FindIndex(appendPtr->m_boneAttached);
				    if(AppBonePos){//valid item
                      CBoneObject * boneRef = (CBoneObject *)movObj->List_skeletalObject->m_bonesList->GetAt(AppBonePos); 
				      
						  boneRef->m_boneFrame->GetTransform(NULL,&m_appenAppliedMatrix);
						  m_gStatus->m_matLib.fastMatrixMultiply(m_appenAppliedMatrix,m_appenAppliedMatrix,positionMatrix);
						  SkeletalEmitterCallback(appendPtr->m_thirdPersonSystem,&m_appenAppliedMatrix,camMatrix,m_compoundMatrix,m_gStatus->m_translucentPolyList);
							
						  //hiarcle
							  SkeletalHiarcleMeshExtraction(appendPtr->m_thirdPersonSystem,
															m_appenAppliedMatrix,
															firstPersonModeTemp,
															camMatrix,
															m_false,
															m_true,
															invCamGeneratedMatrix,
															shadowApplicationDiff,
															m_true,
															m_mirrorBased,
															m_compoundMatrix,
															m_gStatus->m_translucentPolyList,
															m_gStatus->m_renderContainerList);
							  //deformable
							  
							  SkeletalDeformableMeshExtraction(appendPtr->m_thirdPersonSystem,
															   m_appenAppliedMatrix,
															   firstPersonModeTemp,
															   camMatrix,
															   m_false,
															   m_true,
															   invCamGeneratedMatrix,
															   shadowApplicationDiff,
															   m_true,
															   m_mirrorBased,
															   m_compoundMatrix,
															   m_gStatus->m_translucentPolyList,
															   m_gStatus->m_renderContainerList);
					}//end valid item
			   }//end valid thirdperson
			   if(movObj->List_controlType == 0)//only do controller player
			   if(appendPtr->m_firstPersonSystem)
               if(firstPersonModeTemp == TRUE || appendPtr->m_alwaysShowFirstPerson == TRUE){//add first person visuals if possible
				  appendPtr->m_firstPersonSystem->m_currentLOD = movObj->List_skeletalObject->m_currentLOD;
				  m_appenAppliedMatrix = camGeneratedMatrix;
				  //appenAppliedMatrix._41 = positionMatrix._41;
				  //appenAppliedMatrix._42 = positionMatrix._42;
				  //appenAppliedMatrix._43 = positionMatrix._43;
				  //m_gStatus->m_matLib.fastMatrixMultiply(appenAppliedMatrix,appenAppliedMatrix,positionMatrix);
				  
				  SkeletalEmitterCallback(appendPtr->m_firstPersonSystem,&m_appenAppliedMatrix,camMatrix,m_compoundMatrix,m_gStatus->m_translucentPolyList);
                  //hiarcle
				  SkeletalHiarcleMeshExtraction(appendPtr->m_firstPersonSystem,
									            m_appenAppliedMatrix,
									            m_false,
									            camMatrix,
												m_true,
												m_false,
												invCamGeneratedMatrix,
												shadowApplicationDiff,
												m_false,
												m_mirrorBased,
												m_compoundMatrix,
												m_gStatus->m_translucentPolyList,
									            m_gStatus->m_renderContainerList);
                  //Deformable
	              SkeletalDeformableMeshExtraction(appendPtr->m_firstPersonSystem,
										           m_appenAppliedMatrix,
										           m_false,
										           camMatrix,
												   m_true,
												   m_false,
												   invCamGeneratedMatrix,
												   shadowApplicationDiff,
												   m_false,
												   m_mirrorBased,
												   m_compoundMatrix,
												   m_gStatus->m_translucentPolyList,
									               m_gStatus->m_renderContainerList);


			   }//end add first person visuals if possible
		   }//end 1
    }//end popped in
	else if(!mirrorBased){//just update current sound

	   if(m_gStatus->m_logRenderSection)
		  LogRenderObj("ENTITY POPPED OUT:"+movObj->List_name,center);
	   BOOL soundOnly = TRUE;
	   if(movObj->m_possedBy)
	   {
          if(movObj->m_possedBy->List_controlType == 0)
             soundOnly = FALSE;
	   }
		   
	   m_gStatus->IndexEntityAnimations(movObj,GetTickCount(),soundOnly,m_gStatus->m_missileRuntime);
	}//end just update current sound
  }//end entity loop
  //--Houseing
  for(POSITION posLoc = m_gStatus->m_houseingCache->GetHeadPosition();posLoc != NULL;)
  {//Houseing
      CHousingCacheObj * hsPtr = (CHousingCacheObj *)m_gStatus->m_houseingCache->GetNext(posLoc);
	  CHousingObj * houseLib = m_gStatus->m_housingDB->GetByIndex(hsPtr->m_dbIndex);
      if(houseLib)
	  {
         if(OutOfFovUntransformed(hsPtr->m_location,houseLib->m_clipBox,camGeneratedMatrix)==FALSE)
		 {//render visuals that arent clipped
		  
		   D3DVECTOR transPos;
           transPos.x = camGeneratedMatrix._41 + hsPtr->m_invLocation._41;
		   transPos.y = camGeneratedMatrix._42 + hsPtr->m_invLocation._42;
		   transPos.z = camGeneratedMatrix._43 + hsPtr->m_invLocation._43;
		   if(houseLib->m_lightObj)
		   {//house has lighting 
			  if(houseLib->BoundBoxPointCheck(transPos,houseLib->m_internalArea))
			  {
                 D3DVECTOR translatedLightPosition;
				 translatedLightPosition.x = houseLib->m_lightPosition.x + hsPtr->m_location._41;
				 translatedLightPosition.y = houseLib->m_lightPosition.y + hsPtr->m_location._42;
				 translatedLightPosition.z = houseLib->m_lightPosition.z + hsPtr->m_location._43;

                 m_gStatus->m_runtimeLightSys->AddLight(houseLib->m_lightObj,translatedLightPosition);
			  }
		   }//end house lighting
           

		   CEXMeshObjList * meshList = houseLib->GenerateRenderCache(transPos,hsPtr->m_open);
		   if(meshList)
           for(POSITION hsPos = meshList->GetHeadPosition();hsPos != NULL;)
		   {
			CEXMeshObj * exMeshPtr = (CEXMeshObj *) meshList->GetNext(hsPos);
            
			exMeshPtr->m_currentMatrix = hsPtr->m_location;
			CatigorizeMesh(exMeshPtr,
					       camMatrix,
					       hsPtr->m_position,
					       invCamGeneratedMatrix,
					       hsPtr->m_position,
						   NULL,
						   m_gStatus->m_translucentPolyList,
						   m_gStatus->m_renderContainerList,
						   m_mirrorBased,
						   m_compoundMatrix);
		   }
           
		   houseLib->FlushCacheObject(meshList);
		 }//end render visuals that arent clipped
	  }
  }//end Houseing

   *timeTaken = GetTickCount() - curTimeCollision;
   m_groupFilterCount = 0;//flush


   if(m_gStatus->m_environmentPtr){
	  m_gStatus->m_environmentPtr->CallBack(currentTime,
											m_gStatus->m_3dDevice,
											m_gStatus->GL_WORLD,
											&m_gStatus->m_backgroundColorGlobal,
											m_gStatus->m_particleDatabase,
											m_gStatus->m_lightningSystem,
											m_gStatus->m_dirSnd,
											m_gStatus->m_soundCenterMatrix,
											m_gStatus->m_soundManager,
											m_gStatus->GL_renderEnvironment,
											m_gStatus->m_renderStateObject,
											m_gStatus->m_visualsRenderList);
   }

   //Commit lighting constants for advanced shaders
   m_gStatus->m_runtimeLightSys->SetLightingConstants(m_gStatus->m_3dDevice,m_gStatus->m_renderStateObject,pointLocation);

   

  if(m_gStatus->m_logRenderSection)
  {//PRIMARY LOGGING SECTION

	 POSITION pLoc;
	 int inLoop = 0;
	 for(inLoop=0;inLoop<m_gStatus->m_renderContainerAlphaFilterList->m_validCount;inLoop++)
	 for(pLoc = m_gStatus->m_renderContainerAlphaFilterList->m_indexedDBAccess[inLoop].m_list.GetHeadPosition();pLoc != NULL;)
	 {
         CTranslucentPolyObj * listObj = (CTranslucentPolyObj *)m_gStatus->m_renderContainerAlphaFilterList->m_indexedDBAccess[inLoop].m_list.GetNext(pLoc);
       
		 POSITION posLoc;
		 for(posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;)
		 {
             CWldObject * ptrLocX = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
			 if(ptrLocX->m_meshObject)
             if(ptrLocX->m_meshObject == listObj->m_meshReference)
			 {

				LogRenderObj(ptrLocX->m_fileNameRef+" dist from Cam:"+m_gStatus->m_cabLib.ConvToString((int)listObj->m_zValue),ptrLocX->m_meshObject->m_abVertexArray.m_indecieCount/3,ptrLocX->m_meshObject->m_abVertexArray.m_indecieCount,ptrLocX->m_meshObject->m_abVertexArray.m_vertexCount);
                if(ptrLocX->m_charona)
		           LogRenderObj("CHARONA GROUP ("+m_gStatus->m_cabLib.ConvToString(ptrLocX->List_groupNumber)+") dist from Cam:"+m_gStatus->m_cabLib.ConvToString((int)listObj->m_zValue),0,0,0);

                if(ptrLocX->m_particleLink > -1)//particle expansion
	               LogRenderObj("PARTICLE GROUP ("+m_gStatus->m_cabLib.ConvToString(ptrLocX->List_groupNumber)+") dist from Cam:"+m_gStatus->m_cabLib.ConvToString((int)listObj->m_zValue),0,0,0);
				continue;
			 }
		 }

		 if(listObj->m_referenceOption)
		 for(posLoc = m_gStatus->m_libraryReferenceDB->GetHeadPosition();posLoc != NULL;)
		 {
             CReferenceObj * ptrLocX = ( CReferenceObj* ) m_gStatus->m_libraryReferenceDB->GetNext(posLoc);	
             if(listObj->m_referenceOption == ptrLocX)
			 {

				LogRenderObj(ptrLocX->m_referenceName+" dist from Cam:"+m_gStatus->m_cabLib.ConvToString((int)listObj->m_zValue),ptrLocX->m_meshObject->m_abVertexArray.m_indecieCount/3,ptrLocX->m_meshObject->m_abVertexArray.m_indecieCount,ptrLocX->m_meshObject->m_abVertexArray.m_vertexCount);
                
				continue;
			 }
		 }
	 }
	  
	 for(inLoop=0;inLoop<m_gStatus->m_translucentPolyList->m_validCount;inLoop++)
	 for(pLoc = m_gStatus->m_translucentPolyList->m_indexedDBAccess[inLoop].m_list.GetHeadPosition();pLoc != NULL;)
	 {
         CTranslucentPolyObj * listObj = (CTranslucentPolyObj *)m_gStatus->m_translucentPolyList->m_indexedDBAccess[inLoop].m_list.GetNext(pLoc);
         POSITION posLoc;
		 for(posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;)
		 {
             CWldObject * ptrLocX = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
			 if(ptrLocX->m_meshObject)
             if(ptrLocX->m_meshObject == listObj->m_meshReference)
			 {
                if(ptrLocX->m_charona)
		           LogRenderObj("CHARONA GROUP ("+m_gStatus->m_cabLib.ConvToString(ptrLocX->List_groupNumber)+") dist from Cam:"+m_gStatus->m_cabLib.ConvToString((int)listObj->m_zValue),0,0,0);

                if(ptrLocX->m_particleLink > -1)//particle expansion
	               LogRenderObj("PARTICLE GROUP ("+m_gStatus->m_cabLib.ConvToString(ptrLocX->List_groupNumber)+") dist from Cam:"+m_gStatus->m_cabLib.ConvToString((int)listObj->m_zValue),0,0,0);
				
				LogRenderObj(ptrLocX->m_fileNameRef+" dist from Cam:"+m_gStatus->m_cabLib.ConvToString((int)listObj->m_zValue),ptrLocX->m_meshObject->m_abVertexArray.m_indecieCount/3,ptrLocX->m_meshObject->m_abVertexArray.m_indecieCount,ptrLocX->m_meshObject->m_abVertexArray.m_vertexCount);
                continue;
			 }
		 }

		 if(listObj->m_referenceOption)
		 for(posLoc = m_gStatus->m_libraryReferenceDB->GetHeadPosition();posLoc != NULL;)
		 {
             CReferenceObj * ptrLocX = ( CReferenceObj* ) m_gStatus->m_libraryReferenceDB->GetNext(posLoc);	
             if(listObj->m_referenceOption == ptrLocX)
			 {

				 LogRenderObj(ptrLocX->m_referenceName+" dist from Cam:"+m_gStatus->m_cabLib.ConvToString((int)listObj->m_zValue),ptrLocX->m_meshObject->m_abVertexArray.m_indecieCount/3,ptrLocX->m_meshObject->m_abVertexArray.m_indecieCount,ptrLocX->m_meshObject->m_abVertexArray.m_vertexCount);
                continue;
			 }
		 }
	 }

	 for(inLoop=0;inLoop<m_gStatus->m_renderContainerList->m_validCount;inLoop++)
	 for(pLoc = m_gStatus->m_renderContainerList->m_indexedDBAccess[inLoop].m_list.GetHeadPosition();pLoc != NULL;)
	 {
         CTranslucentPolyObj * listObj = (CTranslucentPolyObj *)m_gStatus->m_renderContainerList->m_indexedDBAccess[inLoop].m_list.GetNext(pLoc);
         POSITION posLoc;
		 for(posLoc = m_gStatus->m_wldObjList->GetHeadPosition();posLoc != NULL;)
		 {
             CWldObject * ptrLocX = ( CWldObject* ) m_gStatus->m_wldObjList->GetNext(posLoc);	
			 if(ptrLocX->m_meshObject)
             if(ptrLocX->m_meshObject == listObj->m_meshReference)
			 {
                if(ptrLocX->m_charona)
		           LogRenderObj("CHARONA GROUP ("+m_gStatus->m_cabLib.ConvToString(ptrLocX->List_groupNumber)+") dist from Cam:"+m_gStatus->m_cabLib.ConvToString((int)listObj->m_zValue),0,0,0);

                if(ptrLocX->m_particleLink > -1)//particle expansion
	               LogRenderObj("PARTICLE GROUP ("+m_gStatus->m_cabLib.ConvToString(ptrLocX->List_groupNumber)+") dist from Cam:"+m_gStatus->m_cabLib.ConvToString((int)listObj->m_zValue),0,0,0);

				LogRenderObj(ptrLocX->m_fileNameRef+" dist from Cam:"+m_gStatus->m_cabLib.ConvToString((int)listObj->m_zValue),ptrLocX->m_meshObject->m_abVertexArray.m_indecieCount/3,ptrLocX->m_meshObject->m_abVertexArray.m_indecieCount,ptrLocX->m_meshObject->m_abVertexArray.m_vertexCount);
                continue;
			 }
		 }

		 if(listObj->m_referenceOption)
		 for(posLoc = m_gStatus->m_libraryReferenceDB->GetHeadPosition();posLoc != NULL;)
		 {
             CReferenceObj * ptrLocX = ( CReferenceObj* ) m_gStatus->m_libraryReferenceDB->GetNext(posLoc);	
             if(listObj->m_referenceOption == ptrLocX)
			 {

				LogRenderObj(ptrLocX->m_referenceName+" dist from Cam:"+m_gStatus->m_cabLib.ConvToString((int)listObj->m_zValue),ptrLocX->m_meshObject->m_abVertexArray.m_indecieCount/3,ptrLocX->m_meshObject->m_abVertexArray.m_indecieCount,ptrLocX->m_meshObject->m_abVertexArray.m_vertexCount);
                continue;
			 }
		 }
	 }
	 LogRenderObj("worse non trasparent indexed cluster",m_gStatus->m_renderContainerAlphaFilterList->GetWorseList());
     LogRenderObj("worse non trasparent indexed cluster",m_gStatus->m_renderContainerList->GetWorseList());
     LogRenderObj("worse trasparent indexed cluster",m_gStatus->m_translucentPolyList->GetWorseList());
     AppendInfoToFile("rendersection.log", operator+("Total world polys rendered this section: ",m_gStatus->m_cabLib.ConvToString(m_gStatus->m_totalWorldPolys)));
  }//end log loop
m_gStatus->m_logRenderSection = FALSE;//used for debugging current render set
m_gStatus->m_resetReferences = FALSE;

return TRUE;
}


//----------------------------------------------------------//
/*void SceneAssembly::CreateFrustumPlanes(     D3DVECTOR *refPt, 
											 D3DVALUE *frustPlanes,
											 float & fov,
											 D3DMATRIX & camMatrix,
											 float & viewWidth,
											 float & viewHieght,
											 float & heightAspect)
{

	D3DXVECTOR3 thirdZClipPt;
	ZeroMemory(&thirdZClipPt, sizeof(D3DVECTOR));

	D3DXVECTOR3 thirdLRPt;
	ZeroMemory(&thirdLRPt, sizeof(D3DVECTOR));

	D3DXVECTOR3 referencePt;
	ZeroMemory(&referencePt, sizeof(D3DVECTOR));

	D3DXVECTOR3 frustumVerticies[5];
	ZeroMemory(frustumVerticies, sizeof(D3DVECTOR)*5);

	referencePt.z = 100.0f;

	//third point used to help create the back, top, and bottom clipping planes
	thirdZClipPt.x = 100.0f;

	//third point used to help create the left and right clipping planes
	thirdLRPt.y = 100.0f; 

	float aspect = viewWidth/viewHieght;
    //left plane
	frustumVerticies[1].x = -100.0f*aspect*tan(fov/2.0f);
	frustumVerticies[1].z = 100.0f;
    //right plane
	frustumVerticies[2].x = 100.0f*aspect*tan(fov/2.0f);
	frustumVerticies[2].z = 100.0f;
    //bottom plane
	frustumVerticies[3].y = -100.0f/heightAspect*tan(fov/2.0f);
	frustumVerticies[3].z = 100.0f;
	//top plane
	frustumVerticies[4].y = 100.0f/heightAspect*tan(fov/2.0f);
	frustumVerticies[4].z = 100.0f;
	//transform viewing frustum points to world space

	for(int i=0; i < 5; i++)
	frustumVerticies[i] = m_gStatus->m_matLib.UpdatePolygonVertex(camMatrix.m,frustumVerticies[i]);

	referencePt  = m_gStatus->m_matLib.UpdatePolygonVertex(camMatrix.m,referencePt);
	thirdZClipPt  = m_gStatus->m_matLib.UpdatePolygonVertex(camMatrix.m,thirdZClipPt);
    thirdLRPt  = m_gStatus->m_matLib.UpdatePolygonVertex(camMatrix.m,thirdLRPt);

	//create planes from frustum points
	float frustumPlanes[5][4];
	
	CreatePlane( &frustumPlanes[0][0], frustumVerticies[0], thirdLRPt, thirdZClipPt);
	CreatePlane( &frustumPlanes[1][0], frustumVerticies[0], frustumVerticies[1], thirdLRPt);
	CreatePlane( &frustumPlanes[2][0], frustumVerticies[0], frustumVerticies[2], thirdLRPt);
	CreatePlane( &frustumPlanes[3][0], frustumVerticies[0], frustumVerticies[3], thirdZClipPt);
	CreatePlane( &frustumPlanes[4][0], frustumVerticies[0], frustumVerticies[4], thirdZClipPt);
	
	//return variables
	*refPt = referencePt;
	CopyMemory(frustPlanes, frustumPlanes, sizeof(D3DVALUE)*5*4);
}*/
//----------------------------------------------------------//
BOOL SceneAssembly::RenderTempListPreviews( D3DMATRIX camGeneratedMatrix,
											D3DMATRIX invCamGeneratedMatrix,
											D3DVECTOR pointLocation,
											D3DMATRIX camMatrix,
											float fovView)
{
 //force Visuals
 int m_prevLodTemp = -2;
 CReferenceObj * referenceObject = NULL;
 DWORD currentTime = GetTickCount();
 D3DVECTOR viewPoint;
 for(POSITION posD = m_gStatus->m_worldListTemp->GetHeadPosition();posD != NULL;)
 {//force render temp objects
     CWldObject * wldRenObj = (CWldObject *)m_gStatus->m_worldListTemp->GetNext(posD);
     if(wldRenObj->m_renderFilter)
		continue;

     if(!m_gStatus->m_collisionBase->BoundBoxPointCheck(pointLocation,wldRenObj->List_popoutBox.min,wldRenObj->List_popoutBox.max))
        continue;

     if(wldRenObj->m_node)
	 {//node type
		if(wldRenObj->m_node->m_refPtr == NULL || m_gStatus->m_resetReferences == TRUE)
           wldRenObj->m_node->m_refPtr = m_gStatus->m_libraryReferenceDB->GetByIndex(wldRenObj->m_node->m_libraryReference);
	    
		referenceObject = (CReferenceObj *)wldRenObj->m_node->m_refPtr;
		if(referenceObject)
		{//valid reference object
		   if(referenceObject->m_skeletalOption)
		   {//skeletal based reference parse
			       //init option-only if hasnt been done before
				   if(!referenceObject->m_skeletalOptionLive)
					   referenceObject->m_skeletalOption->Clone(&referenceObject->m_skeletalOptionLive,NULL,NULL);
                   
				   m_gStatus->m_skeletalReferenceObjCache.AddTail(wldRenObj);//add to collision vicinity list
				   //eval lod
				   m_prevLodTemp = referenceObject->m_skeletalOptionLive->m_currentLOD;
			       if(referenceObject->m_skeletalOptionLive->m_lodRanges)
                      referenceObject->m_skeletalOptionLive->m_currentLOD = referenceObject->m_skeletalOptionLive->m_lodRanges->GetLod(camMatrix,wldRenObj->m_node->m_position);
			         
				  

                   if(referenceObject->m_animationUpdatedThisRound != currentTime || m_prevLodTemp != referenceObject->m_skeletalOptionLive->m_currentLOD)
				   {//update only one time per rally
				     D3DVECTOR center;
			         center.x = wldRenObj->m_node->m_position._41;
			         center.y = wldRenObj->m_node->m_position._42;
			         center.z = wldRenObj->m_node->m_position._43;
                     referenceObject->m_skeletalOptionLive->SetCurrentAnimation(0,175);
					 referenceObject->m_skeletalOptionLive->AnimationCallback(  currentTime,
																			m_gStatus->m_soundCenterMatrix,
																			center,
																			m_gStatus->GL_globalSoundFactor,
																			FALSE,
																			m_gStatus->m_soundManager,
																			FALSE,
																			m_gStatus->m_multiplayerObject->m_AIServer,
																			m_gStatus->m_3dDevice,
																			m_gStatus->m_d3dVectorInit,
																			m_gStatus->m_oneVal,
																			m_gStatus->m_lodOverride
																			);
					 
					  referenceObject->m_animationUpdatedThisRound = currentTime;
					  if(referenceObject->m_skeletalOptionLive->m_currentLOD == 0)
					  if(referenceObject->m_skeletalOptionLive->m_hasPerPolyCollisionsTemp)
						 m_refSkeletalResetList.AddTail(referenceObject);//adds one time per rally for reset anim state on generatepopoutlist start
				   }//end update only one time per rally 
                   
                     SkeletalEmitterCallback(referenceObject->m_skeletalOptionLive,&wldRenObj->m_node->m_position,camMatrix,m_compoundMatrix,m_gStatus->m_translucentPolyList);
				     SkeletalDeformableMeshExtraction(referenceObject->m_skeletalOptionLive,
														wldRenObj->m_node->m_position,
														m_false,
														camMatrix,
														//ViewPort,
														m_false,
														m_false,
														invCamGeneratedMatrix,
														invCamGeneratedMatrix,
														m_false,
														m_mirrorBased,
														m_compoundMatrix,
														m_gStatus->m_translucentPolyList,
									                    m_gStatus->m_renderContainerList
														);

					 SkeletalHiarcleMeshExtraction(referenceObject->m_skeletalOptionLive,
												   wldRenObj->m_node->m_position,
												   m_false,
												   camMatrix,
												   m_false,
												   m_false,
												   //ViewPort,
												   invCamGeneratedMatrix,
												   invCamGeneratedMatrix,
												   m_true,
												   m_mirrorBased,
												   m_compoundMatrix,
												   m_gStatus->m_translucentPolyList,
									               m_gStatus->m_renderContainerList);
		   }//end skeletal based reference parse
		   else
		   {//--Mesh based reference parse---//
			   if(wldRenObj->m_lightObject)
			   {//add light
				  if(referenceObject->m_meshObject)
				  {
						   D3DVECTOR lightCenter;
						   lightCenter.x = referenceObject->m_meshObject->m_meshCenter.x + wldRenObj->m_node->m_position._41;
						   lightCenter.y = referenceObject->m_meshObject->m_meshCenter.y + wldRenObj->m_node->m_position._42;
						   lightCenter.z = referenceObject->m_meshObject->m_meshCenter.z + wldRenObj->m_node->m_position._43; 
						   m_gStatus->m_runtimeLightSys->AddLight(wldRenObj->m_lightObject,
													   lightCenter);
				  }
			   }//end add light
			   //D3DVECTOR center;
			   m_tempCenter.x = wldRenObj->m_node->m_position._41;
			   m_tempCenter.y = wldRenObj->m_node->m_position._42;
			   m_tempCenter.z = wldRenObj->m_node->m_position._43;    
			   if( referenceObject->m_lodVisualList )
			   {//LOD sys exists
				  viewPoint.x = camGeneratedMatrix._41;viewPoint.y = camGeneratedMatrix._42;viewPoint.z = camGeneratedMatrix._43;
				  CEXMeshObj * mObj = referenceObject->m_lodVisualList->GetLodVisual(viewPoint,m_tempCenter,fovView,m_gStatus->m_lodOverride);
				  if(mObj){//valid LOD
					 mObj->m_currentMatrix = wldRenObj->m_node->m_position;
					 CatigorizeMesh(mObj,
									camMatrix,
									m_tempCenter,
									invCamGeneratedMatrix,
									m_tempCenter,
									NULL,
									m_gStatus->m_translucentPolyList,
									m_gStatus->m_renderContainerList,
									m_mirrorBased,
									m_compoundMatrix);
				  }//end valid LOD
				  else{//invalalid lod add standard
					 referenceObject->m_meshObject->m_currentMatrix = wldRenObj->m_node->m_position;
					 CatigorizeMesh(referenceObject->m_meshObject,
									camMatrix,
									m_tempCenter,
									invCamGeneratedMatrix,
									m_tempCenter,
									NULL,
									m_gStatus->m_translucentPolyList,
									m_gStatus->m_renderContainerList,
								    m_mirrorBased,
									m_compoundMatrix);
				  }//end invalid lod add standard
			   }//end LOD sys exists
			   else
			   {//standard implement
					 referenceObject->m_meshObject->m_currentMatrix = wldRenObj->m_node->m_position;
					 CatigorizeMesh(referenceObject->m_meshObject,
									camMatrix,
									m_tempCenter,
									invCamGeneratedMatrix,
									m_tempCenter,
									NULL,
									m_gStatus->m_translucentPolyList,
									m_gStatus->m_renderContainerList,
								    m_mirrorBased,
								    m_compoundMatrix);
			   }//end standard implement
			  }
		}//end valid reference object
	 }//end node type
	 else{//regular
	   if(wldRenObj->m_lodVisualList)
	   {//1
	     
	     viewPoint.x = camGeneratedMatrix._41;viewPoint.y = camGeneratedMatrix._42;viewPoint.z = camGeneratedMatrix._43;
		
	     CEXMeshObj * mObj = (CEXMeshObj *)wldRenObj->m_lodVisualList->GetLodVisual(viewPoint,wldRenObj->m_meshObject->m_meshCenter,fovView,m_gStatus->m_lodOverride);
         if(mObj){
		   mObj->List_textureSelect[0] = wldRenObj->m_meshObject->List_textureSelect[0];
		   mObj->m_chromeWrap = wldRenObj->m_meshObject->m_chromeWrap;
		   CopyMemory(&mObj->List_materialObject->m_useMaterial,&wldRenObj->m_meshObject->List_materialObject->m_baseMaterial,sizeof(D3DMATERIAL9));
		   mObj->List_materialObject->m_blendMode       = wldRenObj->m_meshObject->List_materialObject->m_blendMode;
		   mObj->List_materialObject->m_fadeVisDist     = wldRenObj->m_meshObject->List_materialObject->m_fadeVisDist;
		   mObj->List_materialObject->m_fadeMaxDist     = wldRenObj->m_meshObject->List_materialObject->m_fadeMaxDist;
		   mObj->List_materialObject->m_fadeByDistance  = wldRenObj->m_meshObject->List_materialObject->m_fadeByDistance;
		   CatigorizeMesh(mObj,
						  camMatrix,
						  wldRenObj->m_meshObject->m_meshCenter,
						  invCamGeneratedMatrix,
						  wldRenObj->m_meshObject->m_meshCenter,
						  NULL,
						  m_gStatus->m_translucentPolyList,
						  m_gStatus->m_renderContainerList,
						  m_mirrorBased,
						  m_compoundMatrix);
		}
		else{
		   CatigorizeMesh(wldRenObj->m_meshObject,
						  camMatrix,
						  wldRenObj->m_meshObject->m_meshCenter,
						  invCamGeneratedMatrix,
						  wldRenObj->m_meshObject->m_meshCenter,
						  NULL,
						  m_gStatus->m_translucentPolyList,
						  m_gStatus->m_renderContainerList,
						  m_mirrorBased,
						  m_compoundMatrix);
		}
	   }//end 1
	   else{
         CatigorizeMesh(wldRenObj->m_meshObject,
						  camMatrix,
						  wldRenObj->m_meshObject->m_meshCenter,
						  invCamGeneratedMatrix,
						  wldRenObj->m_meshObject->m_meshCenter,
						  NULL,
						  m_gStatus->m_translucentPolyList,
						  m_gStatus->m_renderContainerList,
						  m_mirrorBased,
						  m_compoundMatrix);
	   }
	 }
 }//end force render temp objects
 //end force Visuals
 return TRUE;
}
//------------------------------------------------------------------------------------//
void SceneAssembly::AddGroupFilter(int & groupNumber)
{
    if(groupNumber >= 2048)
	   return;
	m_groupFilterArray[groupNumber] = 1;
}
//------------------------------------------------------------------------------------//
BOOL SceneAssembly::ValidateGroup(int & groupNumber)
{
    if(!m_groupFilterArray[groupNumber])
		return TRUE;
	
    return FALSE;
}
//------------------------------------------------------------------------------------//
/*BOOL SceneAssembly::IsBoxInView(ABBOX & cl_box,
							    D3DMATRIX & camGenMatrix
							    )
{

			m_boxmin.x = cl_box.minX;
			m_boxmin.y = cl_box.minY;
			m_boxmin.z = cl_box.minZ;

			m_boxmax.x = cl_box.maxX;
			m_boxmax.y = cl_box.maxY;
			m_boxmax.z = cl_box.maxZ;
		
			if(!OutOfFOV(m_boxmin, m_boxmax, camGenMatrix))	
		    return TRUE;
return FALSE;
}*/
//------------------------------------------------------------------------------------//
BOOL SceneAssembly::IsPointInView(D3DVECTOR & cl_point,
							      D3DMATRIX & camGenMatrix
							       )
{

			m_boxmin.x = cl_point.x-1.0f;
			m_boxmin.y = cl_point.y-1.0f;
			m_boxmin.z = cl_point.z-1.0f;

			m_boxmax.x = cl_point.x+1.0f;
			m_boxmax.y = cl_point.y+1.0f;
			m_boxmax.z = cl_point.z+1.0f;
		
			if(!OutOfFOV(m_boxmin, m_boxmax, camGenMatrix))	
		    return TRUE;
return FALSE;
}
//------------------------------------------------------------------------------------//
BOOL SceneAssembly::IsMeshInView(CEXMeshObj * mesh,
							     D3DMATRIX & camGenMatrix
							     )
{

			m_boxmin.x = mesh->List_boundingBox.minX;
			m_boxmin.y = mesh->List_boundingBox.minY;
			m_boxmin.z = mesh->List_boundingBox.minZ;

			m_boxmax.x = mesh->List_boundingBox.maxX;
			m_boxmax.y = mesh->List_boundingBox.maxY;
			m_boxmax.z = mesh->List_boundingBox.maxZ;
		
			if(!OutOfFOV(m_boxmin, m_boxmax, camGenMatrix))	
		    return TRUE;
return FALSE;
}
//----------------------------------------------------------------------------
void SceneAssembly::ManageEnvEffects(CWldObject * wldObj,CEXMeshObj * mObj)
{
                         if(wldObj->m_envReactionAttrib > 0)
		                 {//swich style for enviorment state
			                 if(m_gStatus->m_environmentPtr->m_enviormentTimeSystem)
			                 if(m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState)
			                 if(m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_sunColorRed > m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_ambientColorRed || 
				                m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_sunColorGreen > m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_ambientColorGreen || 
			                    m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_sunColorBlue > m_gStatus->m_environmentPtr->m_enviormentTimeSystem->m_currentState->m_ambientColorBlue)
			                 {
			                    if(wldObj->m_envReactionAttrib == 1)
				                {
                                   mObj->List_BlendMethod[0] = -1;
								   mObj->List_materialObject->m_useMaterial.Emissive.r = 0.0f;
                                   mObj->List_materialObject->m_useMaterial.Emissive.g = 0.0f;
                                   mObj->List_materialObject->m_useMaterial.Emissive.b = 0.0f;
                                   mObj->List_materialObject->m_useMaterial.Emissive.a = 0.0f;
				                 }
								 else if(wldObj->m_envReactionAttrib == 2)
								 {
                                   mObj->List_materialObject->m_useMaterial.Emissive.r = 0.0f;
                                   mObj->List_materialObject->m_useMaterial.Emissive.g = 0.0f;
                                   mObj->List_materialObject->m_useMaterial.Emissive.b = 0.0f;
                                   mObj->List_materialObject->m_useMaterial.Emissive.a = 0.0f;
								 }
								 else if(wldObj->m_envReactionAttrib == 3)
								 {
								   mObj->List_BlendMethod[0] = 0;
                                   mObj->List_BlendMethod[1] = -1;
								 }
		                     }
			                 else{
				                if(wldObj->m_envReactionAttrib == 1)
				                {
                                   mObj->List_BlendMethod[0] = 0;
								   mObj->List_materialObject->m_useMaterial.Emissive.r = 1.0f;
                                   mObj->List_materialObject->m_useMaterial.Emissive.g = 1.0f;
                                   mObj->List_materialObject->m_useMaterial.Emissive.b = 1.0f;
                                   mObj->List_materialObject->m_useMaterial.Emissive.a = 1.0f;
				                }
								else if(wldObj->m_envReactionAttrib == 2)
								{
                                   mObj->List_materialObject->m_useMaterial.Emissive.r = 1.0f;
                                   mObj->List_materialObject->m_useMaterial.Emissive.g = 1.0f;
                                   mObj->List_materialObject->m_useMaterial.Emissive.b = 1.0f;
                                   mObj->List_materialObject->m_useMaterial.Emissive.a = 1.0f;
								}
								else if(wldObj->m_envReactionAttrib == 3)
								 {
								   mObj->List_BlendMethod[0] = 5;
                                   mObj->List_BlendMethod[1] = 5;
								 }
			                 }
		                  }//end swich style for enviorment state
}
//------------------------------------------------------------------------------------//
void SceneAssembly::ParseExplosionObjects(D3DMATRIX & camGeneratedMatrix,
										  D3DMATRIX & camMatrix,
										  D3DMATRIX & invCamGeneratedMatrix)
{
  BOOL falseBool = FALSE;
  long explosionCurTime=GetTickCount();
  POSITION posLast;
  for(POSITION misPos = m_gStatus->m_explosionRuntimeList->GetHeadPosition();(posLast = misPos) != NULL;)
  {
      CExplosionObj * expObj = (CExplosionObj *)m_gStatus->m_explosionRuntimeList->GetNext(misPos);
	  
      long timePassed = explosionCurTime - expObj->m_beginTimeStamp;
      
        
      if(m_gStatus->m_dirSnd)
	   if(expObj->m_enableSound){
          if(expObj->m_soundHandle)
		  {
			  //D3DMATRIX matrixPosition;
			  D3DVECTOR wldSpacePos;
              expObj->m_frame->GetPosition(NULL,&wldSpacePos);
              expObj->m_soundHandle->UpdatePosition(wldSpacePos);
		  }
		  else{
			  D3DVECTOR wldSpacePos;
              expObj->m_frame->GetPosition(NULL,&wldSpacePos);
              expObj->m_soundHandle = m_gStatus->m_soundManager->PlayByID(expObj->m_snID,FALSE,wldSpacePos,expObj->m_distanceFactor,2000.0f);
			  if(expObj->m_soundHandle)
			  expObj->m_soundHandle->m_selfDeleteing = FALSE;
		  }
	   }
 
		D3DMATRIX centerMatrix;
		expObj->m_frame->GetTransform(NULL,&centerMatrix);
        ABBOX box;

		if(expObj->m_light)
		{//1
		   D3DVECTOR tempVector;
		   tempVector.x = centerMatrix._41;
		   tempVector.y = centerMatrix._42;
		   tempVector.z = centerMatrix._43;
			
           m_gStatus->m_runtimeLightSys->AddLight(expObj->m_light,
									              tempVector);
		}//end 1

        box.maxX = expObj->m_clipRadius;
        box.maxY = expObj->m_clipRadius;
        box.maxZ = expObj->m_clipRadius;

        box.minX = - expObj->m_clipRadius;
        box.minY = - expObj->m_clipRadius;
        box.minZ = - expObj->m_clipRadius;

        if(OutOfFovUntransformed(centerMatrix,box,camGeneratedMatrix)==FALSE)
		{//popped in 

          if(expObj->m_startFadeAt > -1){//valid fade
			  if(timePassed >= expObj->m_startFadeAt)
                 expObj->SetFade(explosionCurTime);
		  }//end valid fade
		  if(expObj->m_orientation == 1){//camera orientate
             D3DVECTOR camPos;camPos.x = camMatrix._41;camPos.y = camMatrix._42;camPos.z = camMatrix._43;
			 expObj->m_frame->FaceTowardPoint(camPos);
		  }
         //-----------------------//
		 //----keyframe animation-//
		 //-----------------------//
		 long timePassedAnim = explosionCurTime - expObj->m_currentKeyTimeStamp;
		 if(timePassedAnim >= expObj->m_currentKeyDuration){
			timePassedAnim = expObj->m_currentKeyDuration;
            if(expObj->IndexKeyframe()){
		        timePassedAnim = explosionCurTime - expObj->m_currentKeyTimeStamp;
				if(timePassedAnim >= expObj->m_currentKeyDuration)
			       timePassedAnim = expObj->m_currentKeyDuration;
			}
		 }
		 


		 D3DMATRIX positionMatrix;
	     D3DVECTOR positionCenter;
	     expObj->m_frame->GetPosition(NULL,&positionCenter);
	     expObj->m_frame->GetTransform(NULL,&positionMatrix);
         
		 if(expObj->m_currentKeyDuration < 0)
		 {//1
			D3DVECTOR dummyNormal;
			dummyNormal.x = 0.0f;
			dummyNormal.y = 1.0f;
			dummyNormal.z = 0.0f;
			
            m_gStatus->m_explosionDBList->CreateExplosion( abs(expObj->m_currentKeyDuration),
														   positionCenter,
														   m_gStatus->m_explosionRuntimeList,
														   dummyNormal,
														   expObj->m_ownerTrace,
														   m_gStatus->m_3dDevice,
														   expObj->m_teamRef,
														   -1
														   ); 


            expObj->m_currentKeyDuration = 1;

		 }//end 1
		 else{//normal handle
		 
		 D3DVECTOR startScaler,currentScaler,targetScaler;
		 D3DVECTOR startRotation,currentRotation,targetRotation;
		 startScaler.x   = expObj->m_lastScalerX;
		 startScaler.y   = expObj->m_lastScalerY;
		 startScaler.z   = expObj->m_lastScalerZ;
		 targetScaler.x  = expObj->m_baseScalerX;
		 targetScaler.y  = expObj->m_baseScalerY;
		 targetScaler.z  = expObj->m_baseScalerZ;
		 startRotation.x = expObj->m_lastRotationX;
		 startRotation.y = expObj->m_lastRotationY;
		 startRotation.z = expObj->m_lastRotationZ;
	     targetRotation.x = expObj->m_rotationX;
		 targetRotation.y = expObj->m_rotationY;
		 targetRotation.z = expObj->m_rotationZ;
               
		 currentScaler =  m_gStatus->m_matLib.InterpolatePoints(startScaler,
			                                                   targetScaler,
					                                           expObj->m_currentKeyDuration, 
					                                           timePassedAnim);
		 currentRotation =  m_gStatus->m_matLib.InterpolatePoints(startRotation,
			                                                     targetRotation,
					                                             expObj->m_currentKeyDuration, 
					                                             timePassedAnim);
		 D3DMATRIX scaleMatrix,rotateMatrix;
		 m_gStatus->m_matLib.Rotate(currentRotation.x,currentRotation.y,currentRotation.z,rotateMatrix.m);
		 m_gStatus->m_matLib.scale(currentScaler.x,currentScaler.y,currentScaler.z,scaleMatrix.m);
		 m_gStatus->m_matLib.fastMatrixMultiply(positionMatrix,scaleMatrix,positionMatrix); 
		 m_gStatus->m_matLib.fastMatrixMultiply(positionMatrix,rotateMatrix,positionMatrix); 

		 }//end normal handle
		 
		 
		 if(expObj->m_meshObject){//valid mesh object
		    expObj->m_meshObject->m_currentMatrix = positionMatrix;
		    CatigorizeMesh(expObj->m_meshObject,
					       camMatrix,
					       positionCenter,
						   invCamGeneratedMatrix,
						   positionCenter,
						   NULL,
						   m_gStatus->m_translucentPolyList,
						   m_gStatus->m_renderContainerList,
						   m_mirrorBased,
						   m_compoundMatrix);
		 }//end valid mesh object
         if(expObj->m_skeletalObj)
		 {//explosion object
		  
          //deformable
		  expObj->m_skeletalObj->AnimationCallback(explosionCurTime,
		                                                 m_gStatus->m_soundCenterMatrix,
													     positionCenter,
													     m_gStatus->GL_globalSoundFactor,
													     FALSE,
													     m_gStatus->m_soundManager,
													     FALSE,
													     m_gStatus->m_multiplayerObject->m_AIServer,
													     m_gStatus->m_3dDevice,
														 m_gStatus->m_d3dVectorInit,
														 m_gStatus->m_oneVal,
														 m_gStatus->m_lodOverride);
          SkeletalEmitterCallback(expObj->m_skeletalObj,&positionMatrix,camMatrix,m_compoundMatrix,m_gStatus->m_translucentPolyList);
	      SkeletalDeformableMeshExtraction(expObj->m_skeletalObj,
										   positionMatrix,
									  	   m_false,
										   camGeneratedMatrix,
										   m_false,
										   m_true,
										   invCamGeneratedMatrix,
										   positionMatrix,
									  	   falseBool,
										   m_mirrorBased,
										   m_compoundMatrix,
										   m_gStatus->m_translucentPolyList,
									       m_gStatus->m_renderContainerList
										   );
          //hiarchl
	      SkeletalHiarcleMeshExtraction(expObj->m_skeletalObj,
									    positionMatrix,
									    m_false,
									    camGeneratedMatrix,
									    m_false,
									    m_true,
									    invCamGeneratedMatrix,
									    positionMatrix,
									    falseBool,
										m_mirrorBased,
										m_compoundMatrix,
									    m_gStatus->m_translucentPolyList,
									    m_gStatus->m_renderContainerList);
		 }//end explosion object
		}//end popped in 
  }
}
//--------------Handles system messages-----------------------------------------------//
void SceneAssembly::MissileParse(D3DMATRIX &   cameraMatrix,
							     D3DMATRIX &   inverseCameraMatrix)
{
	
	long currentTime = GetTickCount();
	BOOL drawShadow = FALSE;
      for(POSITION posLoc = m_gStatus->m_missileRuntime->GetHeadPosition();posLoc != NULL;){//missile last position
        CMissileObj * mPtr = ( CMissileObj* ) m_gStatus->m_missileRuntime->GetNext(posLoc);
 


		D3DMATRIX positionMatrix;
		D3DVECTOR missileCenter;
		mPtr->m_baseFrame->GetPosition(NULL,&missileCenter);
	    mPtr->m_baseFrame->GetTransform(NULL,&positionMatrix);
		float clipRadius = 10.0f;
        
        if(mPtr->m_light)
		{//light exists
           m_gStatus->m_runtimeLightSys->AddLight(mPtr->m_light,missileCenter);
		}//end light exists

		if(mPtr->m_particleLink != -1){
            POSITION particlePos = m_gStatus->m_particleDatabase->FindIndex(mPtr->m_particleLink);
			if(particlePos){
			   EnvParticleSystem* pSys = (EnvParticleSystem*)m_gStatus->m_particleDatabase->GetAt(particlePos);
			   m_gStatus->m_translucentPolyList->CacheMesh( NULL,
												 pSys,
												 cameraMatrix,
												 positionMatrix,
												 missileCenter,
												 NULL);
			   
			}
		 }


		ABBOX bndBox;
	    bndBox.maxX = clipRadius;
	    bndBox.maxY = clipRadius;
	    bndBox.maxZ = clipRadius;
	    bndBox.minX = -clipRadius;
	    bndBox.minY = -clipRadius;
	    bndBox.minZ = -clipRadius;

        if(mPtr->m_typeOfProjectile == 3)//laser clipping mod
		   if(mPtr->m_laserObject){//;)
			  D3DXVECTOR3 min,max;
			  max.x = clipRadius + missileCenter.x;
	          max.y = clipRadius + missileCenter.y;
	          max.z = clipRadius + missileCenter.z;
	          min.x = -clipRadius + missileCenter.x;
	          min.y = -clipRadius + missileCenter.y;
	          min.z = -clipRadius + missileCenter.z;
              if(mPtr->m_laserObject->m_fireFromLocation._41 < min.x)
                 min.x = mPtr->m_laserObject->m_fireFromLocation._41;
              if(mPtr->m_laserObject->m_fireFromLocation._42 < min.y)
                 min.y = mPtr->m_laserObject->m_fireFromLocation._42;
			  if(mPtr->m_laserObject->m_fireFromLocation._43 < min.z)
                 min.z = mPtr->m_laserObject->m_fireFromLocation._43;
			  if(mPtr->m_laserObject->m_fireFromLocation._41 > max.x)
                 max.x = mPtr->m_laserObject->m_fireFromLocation._41;
              if(mPtr->m_laserObject->m_fireFromLocation._42 > max.y)
                 max.y = mPtr->m_laserObject->m_fireFromLocation._42;
			  if(mPtr->m_laserObject->m_fireFromLocation._43 > max.z)
                 max.z = mPtr->m_laserObject->m_fireFromLocation._43;
			  if(OutOfFOV(min, max, cameraMatrix)==TRUE)
		         continue;
		   }//end ;)
	    
		if(OutOfFovUntransformed(positionMatrix,bndBox,cameraMatrix)==TRUE)//popped out
		continue;

		switch(mPtr->m_typeOfProjectile)
		{//visual type switch
		 case 0://static mesh type
		   if(mPtr->m_orientateToCamera){//face towards cam???
		      D3DVECTOR camPos;
		      camPos.x = cameraMatrix._41;camPos.y = cameraMatrix._42;camPos.z = cameraMatrix._43;
		      m_gStatus->m_matLib.FaceTowardPoint(camPos,&positionMatrix);
			  D3DMATRIX matrixRotational;
		      m_gStatus->m_matLib.Rotate(0.0f,180.0f,0.0f,matrixRotational.m);
		      m_gStatus->m_matLib.fastMatrixMultiply(positionMatrix,matrixRotational,positionMatrix); 
		   }//end face towards cam
		   mPtr->m_basicMesh->m_currentMatrix = positionMatrix;
		   CatigorizeMesh(mPtr->m_basicMesh,
						  cameraMatrix,
						  missileCenter,
						  inverseCameraMatrix,
						  missileCenter,
						  NULL,
						  m_gStatus->m_translucentPolyList,
						  m_gStatus->m_renderContainerList,
						  m_mirrorBased,
						  m_compoundMatrix);
		 break;//exit static mesh type
		 case 1://Billboard type
		   if(mPtr->m_orientateToCamera){//face towards cam???
		      D3DVECTOR camPos;
		      camPos.x = cameraMatrix._41;camPos.y = cameraMatrix._42;camPos.z = cameraMatrix._43;
		      m_gStatus->m_matLib.FaceTowardPoint(camPos,&positionMatrix);
			  D3DMATRIX matrixRotational;
		      m_gStatus->m_matLib.Rotate(0.0f,180.0f,0.0f,matrixRotational.m);
		      m_gStatus->m_matLib.fastMatrixMultiply(positionMatrix,matrixRotational,positionMatrix); 
		   }//end face towards cam
		   mPtr->m_basicMesh->m_currentMatrix = positionMatrix;
		   CatigorizeMesh(mPtr->m_basicMesh,
						  cameraMatrix,
						  missileCenter,
						  inverseCameraMatrix,
						  missileCenter,
						  NULL,
						  m_gStatus->m_translucentPolyList,
						  m_gStatus->m_renderContainerList,
						  m_mirrorBased,
						  m_compoundMatrix);
		 break;//exit billboard type
		 case 2://skeletal System visual
          SkeletalEmitterCallback(mPtr->m_skeletalVisual,&positionMatrix,cameraMatrix,m_compoundMatrix,m_gStatus->m_translucentPolyList);
          SkeletalDeformableMeshExtraction(mPtr->m_skeletalVisual,
										   positionMatrix,
										   m_false,
										   cameraMatrix,
										   //viewPort,
										   m_false,
									  	   m_true,
										   inverseCameraMatrix,
										   inverseCameraMatrix,
										   drawShadow,
										   m_mirrorBased,
										   m_compoundMatrix,
										   m_gStatus->m_translucentPolyList,
									       m_gStatus->m_renderContainerList);
	      //hiarchl
	      SkeletalHiarcleMeshExtraction(mPtr->m_skeletalVisual,
									    positionMatrix,
									    m_false,
									    cameraMatrix,
									    m_false,
									    m_true,
									    //viewPort,
									    inverseCameraMatrix,
										inverseCameraMatrix,
										drawShadow,
									    m_mirrorBased,
									    m_compoundMatrix,
										m_gStatus->m_translucentPolyList,
									    m_gStatus->m_renderContainerList);

	     break;//exit skeletal System visual
		 case 3://laser segment
		 if(mPtr->m_laserObject)
		 {   
			if(!mPtr->m_laserObject->m_lockDown)
			    mPtr->m_baseFrame->GetTransform(NULL,&mPtr->m_laserObject->m_orientationAndOrigin);
            else
			{//locked down / adjust length dynamically
                D3DVECTOR fowardPosition;
				mPtr->m_baseFrame->GetPosition(NULL,&fowardPosition);
				mPtr->m_laserObject->m_currentLength = -m_gStatus->m_matLib.MagnitudeSquared(fowardPosition.x,fowardPosition.y,fowardPosition.z,
					                                                                        mPtr->m_laserObject->m_orientationAndOrigin._41,
													                                        mPtr->m_laserObject->m_orientationAndOrigin._42,
													                                        mPtr->m_laserObject->m_orientationAndOrigin._43);

			}

            if(mPtr->m_laserObject->m_fadeOut)
			{
               long finalTime = mPtr->m_lifeTime - mPtr->m_laserObject->m_millisecondsOfFade;
               long timePassed = currentTime - mPtr->m_startTimeStamp;
               long timeBeforeFade = mPtr->m_lifeTime - timePassed;
			   if(timeBeforeFade <= mPtr->m_laserObject->m_millisecondsOfFade)
			   {
                   mPtr->m_laserObject->m_meshObject->List_materialObject->SetFadeOut(timeBeforeFade);
  

			   }

			}

            mPtr->m_laserObject->UpdateLazerMesh(cameraMatrix);
			mPtr->m_laserObject->m_meshObject->InitBuffer(m_gStatus->m_3dDevice,NULL);
			CatigorizeMesh(mPtr->m_laserObject->m_meshObject,
						   cameraMatrix,
						   missileCenter,
						   inverseCameraMatrix,
						   missileCenter,
						   NULL,
						   m_gStatus->m_translucentPolyList,
						   m_gStatus->m_renderContainerList,
						   m_mirrorBased,
						   m_compoundMatrix);
		 }
	     break;
		}//end visual type switch
	  }//end missile last position
}
//----------------------------------------------------------//
BOOL SceneAssembly::OutOfFovUntransformed(  D3DMATRIX & matrix,
											ABBOX & bndBox,
											D3DMATRIX & cameraMatrix)
{
   D3DXVECTOR3 min,max;

   max.x = bndBox.maxX + matrix._41;
   max.y = bndBox.maxY + matrix._42;
   max.z = bndBox.maxZ + matrix._43;

   min.x = bndBox.minX + matrix._41;
   min.y = bndBox.minY + matrix._42;
   min.z = bndBox.minZ + matrix._43;

   if(OutOfFOV(min, max, cameraMatrix)==FALSE) 
	  return FALSE;
return TRUE;
}
//----------------------------------------------------------------//
void SceneAssembly::CachEntityForShadow(CMovementObj * entity)
{
	
     if(m_gStatus->m_globalShadowType == 2)
	 {
       if(!m_gStatus->m_showFirstPersonShadow)
	    if(entity->m_firstPersonMode)
		   return;

       if(entity->m_distanceToCamTemp > m_gStatus->m_globalShadowDist)
	      return;

	   if(m_gStatus->m_shadowEntityList->GetCount() >= m_gStatus->m_maxShadowCount)
	      return;

       for(POSITION posLoc = m_gStatus->m_shadowEntityList->GetHeadPosition();posLoc != NULL;)
	   {
         CMovementObj * entPtrLoc = (CMovementObj *)m_gStatus->m_shadowEntityList->GetNext(posLoc);
         if(entity->m_distanceToCamTemp < entPtrLoc->m_distanceToCamTemp)
		 {
            

            m_gStatus->m_shadowEntityList->InsertBefore(posLoc,entity);
			return;
		 }
	   }
	   m_gStatus->m_shadowEntityList->AddTail(entity);
	 }
}

//------------------------------------------------------------------------------------//
BOOL SceneAssembly::GetScreen3dPosition(D3DVECTOR & wldSpcPos,
										D3DMATRIX & cameraMatrix,
										float * multiplier,
										D3DVECTOR * returnScreenPos,
										float maxDist,
										float minDist,
										float & fov,
										float & viewHeightBasedAspect
										)
{

      D3DVECTOR localPosition = m_gStatus->m_matLib.GetLocalPositionFromMatrixView(cameraMatrix,wldSpcPos);

	  if(localPosition.z > maxDist || localPosition.z < minDist)
		 return FALSE;

      //float zPos = localPosition.z;
	  double xLocal = 0.0f;
	  double yLocal = 0.0f;

	   if(GetScreenXY(localPosition.z,
				  1.0f,
				  localPosition,
				  &xLocal,
				  &yLocal,
				  fov,
				  viewHeightBasedAspect,
				  1.2f,
				  .9f))
	   {
           float adViewVar = fov / .68f;
           float multiplierLocal = (localPosition.z*adViewVar)/maxDist;

           *multiplier = (1.0f - multiplierLocal);
		   returnScreenPos->x = (float)xLocal;
		   returnScreenPos->y = (float)yLocal;
		   returnScreenPos->z = 3.0f;
		   
		   return TRUE;
	   }
	   return FALSE;
}
//------------------------------------------------------------------------------------//
void SceneAssembly::DisplayTextBasedOn3dPosition(D3DVECTOR & wldSpcPos,
												CViewportObj* ViewPort,
												D3DMATRIX & cameraMatrix,
												CString & textString,
												float maxDistance,
												float minDistance,
												float fontSize,
												CTextureRenderObj* &textureRenderObject,
												float red,
												float green,
												float blue,
												float screenYOffset)//sort of a hack fix for clan name
{

      D3DVECTOR localPosition = m_gStatus->m_matLib.GetLocalPositionFromMatrixView(cameraMatrix,wldSpcPos);

      //float zPos = localPosition.z;
	  double xLocal = 0.0f;
	  double yLocal = 0.0f;
	  if(localPosition.z > maxDistance || localPosition.z < minDistance)
		 return;

	   if(GetScreenXY(localPosition.z,
				  1.0f,
				  localPosition,
				  &xLocal,
				  &yLocal,
				  ViewPort->m_cameraObject->List_fovView,
				  ViewPort->m_cameraObject->List_hieghtBaseAspect,
				  1.2f,
				  .9f))
	   {
		   int index = textString.GetLength() / 2;
		   if(index < 1)
			  index = 1;
           float adViewVar = ViewPort->m_cameraObject->List_fovView / .68f;
           float multiplier = (localPosition.z*adViewVar)/maxDistance;
		   if(multiplier > .7f)
			  return;
           fontSize = fontSize * (1.0f - multiplier);
		   float characterSpacing = fontSize*2.0f;
		   
           if(!textureRenderObject)
               textureRenderObject = new CTextureRenderObj(textString,
															2,
															(float)xLocal,
															(float)yLocal,
															fontSize,
															fontSize,
															TRUE,
															red,
															green,
															blue);

		   //xLocal -= (characterSpacing*index);
		   textureRenderObject->m_measuredDistanceRef = m_gStatus->m_symbolMapDB->GetUnitLength(0,textString,characterSpacing);
		   xLocal -= textureRenderObject->m_measuredDistanceRef*.5f;
           
		   textureRenderObject->m_startPosX = (float)xLocal;
	       textureRenderObject->m_startPosY = (float)yLocal + screenYOffset;
           textureRenderObject->m_fontSizeX = fontSize;
           textureRenderObject->m_fontSizeY = fontSize;

		   textureRenderObject->Initialize(m_gStatus->m_3dDevice,textString,TRUE,m_gStatus->m_symbolMapDB,m_gStatus->m_speechMode,0.0f); 
           m_gStatus->m_textRenderList->AddTail(textureRenderObject);
	   }
}


//------------------------------------------------------------------------------------//
void SceneAssembly::LogRenderObj(CString & objectname,int polyCount,int indecieCount, int vertexCount)
{
	CString filename = "rendersection.log";
	m_gStatus->m_totalWorldPolys += polyCount;
    CString build = objectname+" poly-count:";
    build = build+m_gStatus->m_cabLib.ConvToString(polyCount);

	build += +" indecie-count:";
    build = build+m_gStatus->m_cabLib.ConvToString(indecieCount);

	build += " vertex-count:";
    build = build+m_gStatus->m_cabLib.ConvToString(vertexCount);

    AppendInfoToFile(filename, build);
}
//------------------------------------------------------------------------------------//
void SceneAssembly::LogRenderObj(CString & objectname,D3DVECTOR position)
{
	CString filename = "rendersection.log";
    CString build = objectname;

	build += +" X:";
    /*build = build+m_gStatus->m_cabLib.ConvToString(position.x);*/
	build = build+m_gStatus->m_cabLib.ConvToString((int)position.x);
	//WARNING: C4244 --> int to float

	build += " Y:";
    /*build = build+m_gStatus->m_cabLib.ConvToString(position.y);*/
	build = build+m_gStatus->m_cabLib.ConvToString((int)position.y);
	//WARNING: C4244 --> int to float

	build += " Z:";
    /*build = build+m_gStatus->m_cabLib.ConvToString(position.z);*/
	build = build+m_gStatus->m_cabLib.ConvToString((int)position.z);
	//WARNING: C4244 --> int to float

    AppendInfoToFile(filename, build);
}
//------------------------------------------------------------------------------------//
void SceneAssembly::LogRenderObj(CString misc,int intMisc)
{
	CString filename = "rendersection.log";
    CString build = misc;

	build += +" :";
    build = build+m_gStatus->m_cabLib.ConvToString(intMisc);

    AppendInfoToFile(filename, build);
}
//------------------------------------------------------------------------------------//
void SceneAssembly::AppendInfoToFile(CString filename, CString data) 
{
    CFileException exc;
    CFile fl;
	CArchive the_out_Archive(&fl,CArchive::store);
    BOOL res = fl.Open(filename,CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite, &exc);
	fl.SeekToEnd( );
	the_out_Archive.WriteString( data );  
    the_out_Archive.WriteString( "\r\n");   
    the_out_Archive.Close();
    fl.Close(); 
}
//----------------------------------------------------------//
void SceneAssembly::CreatePlane(D3DVALUE *plane, 
								D3DXVECTOR3 & a, 
								D3DXVECTOR3 & b, 
								D3DXVECTOR3 & c)
{
	D3DXVECTOR3 n = m_gStatus->m_matLib.Normalize ( m_gStatus->m_matLib.CrossProduct( a - b, c - a ));

	plane[0] = n.x;
	plane[1] = n.y;
	plane[2] = n.z;
	plane[3] = -( n.x*c.x + n.y*c.y + n.z*c.z );
}

//----------------------------------------------------------//
BOOL SceneAssembly::OutOfFOV(   D3DXVECTOR3 & min, 
								D3DXVECTOR3 & max,
								D3DMATRIX & cameraMatrix)
{	

	return m_frustum.BoundingBoxOutOfFrustum(min,max);

}

//------------------------------------------------------------------------------------//
BOOL SceneAssembly::GetScreenXY(double localZpos,
							        double clipPlane,
									D3DVECTOR localizePosition,
									double * xScreenPosition,
									double * yScreenPosition,
									double   fov,
									double   vPortHAspect,
									float    screenWidth,
									float    screenHieght)
{
    double fAspect = screenHieght / screenWidth;

	double compromizer =  (1.0+(1.0-fAspect));
    double compromizer2 = (1.0+(1.0-vPortHAspect));
    double leftClipPlane = (-1* tan((0.51*compromizer) * fov)); 
	double rightClipPlane = (   tan((0.51*compromizer) * fov)); 
    double topClipPlane =    (    tan(((0.51*compromizer2) * fov)));
    double bottomClipPlane = (-1* tan(((0.51*compromizer2) * fov)));
    
	double ScaleFactor;
    double dScrnPosHorz;
    double dScrnPosVert;
 
    if(localZpos<0)
	   return FALSE;//behind u

    ScaleFactor = clipPlane/localZpos;

    dScrnPosHorz = ((localizePosition.x * ScaleFactor) - leftClipPlane)
              * (1.2f / (rightClipPlane - leftClipPlane));

    dScrnPosVert = (topClipPlane - (localizePosition.y * ScaleFactor))
              * (.9f / (topClipPlane - bottomClipPlane));

    *xScreenPosition = dScrnPosHorz -.6f;
    *yScreenPosition = -(dScrnPosVert -.45f);
    
	return TRUE;
}
//------------------------------------------------------------------------------------//
void SceneAssembly::TriggerCheckCallback()
{
  for(POSITION posLoc = m_gStatus->m_entObjRuntime->GetHeadPosition();posLoc != NULL;)
  {//Begin entity loop
    CMovementObj * entLoc = ( CMovementObj* ) m_gStatus->m_entObjRuntime->GetNext(posLoc);
      D3DVECTOR origin;
      entLoc->List_baseFrame->GetPosition(NULL,&origin);
	  if(entLoc->List_controlType == 0)
      if(EvaluateTriggers(origin,entLoc->List_baseFrame,entLoc->List_controlType,entLoc)==9)
		 return;
  }
}
//------------------------------------------------------------------------------------//
int SceneAssembly::EvaluateTriggers(D3DVECTOR focusPosition,
								    CFrameObj * frame,
								    int controlType,
								    CMovementObj * entityPtr)
{
  long currentTickCount = GetTickCount();
  for(POSITION posLoc = m_gStatus->m_worldTriggerList->GetHeadPosition();posLoc != NULL;){//world list
      CWldObject * wldPtr = (CWldObject *)m_gStatus->m_worldTriggerList->GetNext( posLoc );
        for(POSITION posLoc = wldPtr->List_triggerList->GetHeadPosition();posLoc != NULL;){//triggerlist
           CTriggerObject * trgPtr = (CTriggerObject *)wldPtr->List_triggerList->GetNext( posLoc );
		
		   if(trgPtr->m_oneTimeOnly)
		      if(trgPtr->m_everActuated)
                 continue;

		   
		   D3DVECTOR objectPosition;
		   ZeroMemory(&objectPosition,sizeof(D3DVECTOR));

		   if(wldPtr->m_meshObject)
		   {
		       CopyMemory(&objectPosition,&wldPtr->m_meshObject->m_meshCenter,sizeof(D3DVECTOR));
		   }
		   else if(wldPtr->m_node)
		   {
               
               CopyMemory(&objectPosition,&wldPtr->m_node->m_position,sizeof(D3DVECTOR));

		   }


		   if(m_gStatus->m_cabLib.BoundingSphereCheck(0.0f,trgPtr->m_radius,
			                      focusPosition.x,focusPosition.y,focusPosition.z,
							      objectPosition.x,objectPosition.y,objectPosition.z)==TRUE)
		   {//actuate trigger
			   trgPtr->m_everActuated = TRUE;
		    if(trgPtr->m_action == 1){//activate animation
             
			}//end activate animation
			if(trgPtr->m_action == 2){//activate portal
			   //D3DMATRIX matrixLocalTemp;
               //GetSpawnPointMatrix(trgPtr->m_indexMisc,&matrixLocalTemp);
			   //g_pD3D->EvictManagedTextures();
			   entityPtr->List_baseFrame->SetPosition(NULL,trgPtr->m_miscVector);
			   entityPtr->List_baseFrame->GetPosition(NULL,&entityPtr->List_lastPosition);
			   entityPtr->List_baseFrame->GetPosition(NULL,&entityPtr->List_movementDB->List_lastPosition);
			}//end activate portal
			if(trgPtr->m_action == 3 && controlType == 0){//activate external portal
			   if(m_gStatus->m_multiplayerObject->GetEnabledClient_safe()){//network side
	              //if(!m_gStatus->m_multiplayerObject->m_requestingPortal)
				  //{
                     if((currentTickCount-m_triggerDelayStamp) > 1500)
					 {
                      m_triggerDelayStamp = currentTickCount;
                      trgPtr->m_indexMisc;//index of world
					  trgPtr->m_miscVector;//position in world
				      m_gStatus->m_netGameLogicObject->SendAttribMsg(   DPNSEND_GUARANTEED,
																		DPID_SERVERPLAYER,
																		26,//attribute
																		(int)trgPtr->m_miscVector.x,
																		(long)trgPtr->m_miscVector.y,
																		(int)trgPtr->m_miscVector.z,
																		(short)trgPtr->m_indexMisc,
																		NULL);

					 }
				  //}
               }//end network side
			   else{//local
				
			   }//end local
			}//end activate external portal
			if(trgPtr->m_action == 4){//activate runtime movie
			   m_gStatus->RunRuntimeMovie(trgPtr->m_indexMisc);
			}//end runtime movie
			if(trgPtr->m_action == 5){//activate force pad
			   entityPtr->List_externalVectorTemp.x = trgPtr->m_miscVector.x;
			   entityPtr->List_externalVectorTemp.y = trgPtr->m_miscVector.y; 
			   entityPtr->List_externalVectorTemp.z = trgPtr->m_miscVector.z; 
			}//end runtime movie
			if(trgPtr->m_action == 6 && controlType == 0){//change soundtrack song
			   //prevent more than one change per round
			   if(m_gStatus->m_soundTrackList->m_acceptChangeFlag)
			   {
			    int loopOptions = atoi(trgPtr->m_miscTxt);
			    m_gStatus->m_soundTrackList->Play(trgPtr->m_indexMisc,m_gStatus->GL_pathGameSounds);
			    m_gStatus->m_soundTrackList->m_acceptChangeFlag = FALSE;
			   }
			}//end change soundtrack song
			if(trgPtr->m_action == 7 && controlType == 0){//test trigger
			   if(m_gStatus->m_systemReadout)
			      m_gStatus->m_systemReadout->AddLine(trgPtr->m_miscTxt);
			}//end test trigger
		   }//end actuate trigger
		}//end triggerlist
	}//end world list
	return 1;
}
//----------------------------------------------------------//
void SceneAssembly::CatigorizeMesh( CEXMeshObj * imMesh,
									D3DMATRIX & cameraMatrix,
									D3DVECTOR & actualCenter,
									D3DMATRIX & invCameraMatrix,
									D3DVECTOR & fadeCenter,
									CReferenceObj * refOpt,
									CTranslucentPolyList * transRenderList,
									CTranslucentPolyList * renderList,
									int & mirrorBased,
									D3DMATRIX * compoundMatrix)
{
        if(!imMesh)
			return;

		if(mirrorBased)
			if(imMesh->List_materialObject->m_mirrorEnabled)
			   return;

	    if(!imMesh->m_abVertexArray.m_indicieBuffer)
			 imMesh->InitBuffer(m_gStatus->m_3dDevice,m_gStatus->m_cgGlobalShaderDB->GetByIndex(imMesh->List_materialObject->m_optionalShaderIndex-1));

        if(imMesh->List_materialObject->m_fadeByDistance && !m_gStatus->m_mapCaptureAll)
		{//requires distance log
             imMesh->List_materialObject->m_curDistFromCam = m_gStatus->m_matLib.MagnitudeSquared(cameraMatrix._41,
				                                                                                  cameraMatrix._42,
																							      cameraMatrix._43,
				                                                                                  fadeCenter.x,
				                                                                                  fadeCenter.y,
				                                                                                  fadeCenter.z);
		}//end requires distance log
		imMesh->List_materialObject->Callback();

		if(!mirrorBased)//not currently rendering a mirror surface
		if(imMesh->List_materialObject->m_mirrorEnabled)
		{//add to mirror list if mirror mode is off
		   if(imMesh->List_textureSelect[imMesh->List_materialObject->m_mirrorEnabled-1] > -1 && imMesh->m_abVertexArray.m_indecieCount > 0)
		   {
              m_gStatus->m_toRenderSurfaceDB->Add(imMesh->m_currentMatrix,
			                                      imMesh->List_textureSelect[imMesh->List_materialObject->m_mirrorEnabled-1],
												  (char)imMesh->List_materialObject->m_mirrorProcessType,
												  actualCenter,
												  NULL,
												  imMesh->List_boundingBox.minX,
												  imMesh->List_boundingBox.maxX,
												  imMesh->List_boundingBox.maxY,
												  imMesh->List_boundingBox.minY);
		   }

		}//end add to mirror list if mirror mode is off

		char oneBitAlpha = 0;
		if(imMesh->List_materialObject->m_doAlphaDepthTest)
		      oneBitAlpha = 1;

        if(imMesh->List_materialObject->m_blendMode != -1 && oneBitAlpha == 0)
		{//transparent
			 
			if(compoundMatrix)
			{//compund transform
               m_gStatus->m_matLib.fastMatrixMultiply(m_compoundTemp,imMesh->m_currentMatrix,*compoundMatrix);
			   m_compoundCenterTemp = m_gStatus->m_matLib.UpdatePolygonVertex(compoundMatrix->m,actualCenter);
 
			   transRenderList->CacheMesh(imMesh,
				                             NULL,
									         cameraMatrix,
									         m_compoundTemp,
											 m_compoundCenterTemp,
											 refOpt);
			}
			else
			   transRenderList->CacheMesh(imMesh,
				                             NULL,
									         cameraMatrix,
									         imMesh->m_currentMatrix,
											 actualCenter,
											 refOpt);
		}//end transparent
		else
		{
			if(compoundMatrix)
			{//compund transform
                  m_gStatus->m_matLib.fastMatrixMultiply(m_compoundTemp,imMesh->m_currentMatrix,*compoundMatrix);
			      m_compoundCenterTemp =  m_gStatus->m_matLib.UpdatePolygonVertex(compoundMatrix->m,actualCenter);
				  renderList->CacheMeshNoSort(imMesh,
											  NULL,
											  cameraMatrix,
											  m_compoundTemp,
											  m_compoundCenterTemp,
											  refOpt);

			}//compund transform
			else
				  renderList->CacheMeshNoSort(imMesh,
											  NULL,
											  cameraMatrix,
											  imMesh->m_currentMatrix,
											  actualCenter,
											  refOpt);
		}	
}
//----------------------------------------------------------//
void SceneAssembly::SkeletalDeformableMeshExtraction(   CSkeletonObject * skObj,
														D3DMATRIX & positionMatrix,
														BOOL & firstPersonMode,
														D3DMATRIX & camMatrix,
														BOOL & forceAdd,
														BOOL & renderInMirror,
														D3DMATRIX & invCameraMatrix,
														D3DMATRIX & shadowTransform,
														BOOL & enableModelShadowing,
									                    int & mirrorBased,
									                    D3DMATRIX * compoundMatrix,
														CTranslucentPolyList * transRenderList,
									                    CTranslucentPolyList * renderList)
{

	   m_tempCenter.x = positionMatrix._41;
	   m_tempCenter.y = positionMatrix._42;
	   m_tempCenter.z = positionMatrix._43;
       for(int mCount=0;mCount<skObj->m_deformableMeshArrayCount;mCount++){

               if(!skObj->m_deformableMeshArrayV2[mCount].m_defMeshVisualUnit)
			       continue;
		       
			   CDeformableVisObj * dVisPtr = (CDeformableVisObj *)skObj->m_deformableMeshArrayV2[mCount].m_defMeshVisualUnit->GetLOD(skObj->m_currentLOD,m_gStatus->m_lodOverride);
			   if(!dVisPtr)
				   continue;
			   dVisPtr->m_mesh->m_currentMatrix = positionMatrix;

			   if(enableModelShadowing)
			      dVisPtr->m_mesh->List_materialObject->m_shadowObject = 1;//REMOVE	   

				   if(firstPersonMode == FALSE)
				      CatigorizeMesh( dVisPtr->m_mesh,
								      camMatrix,
								      m_tempCenter,
									  invCameraMatrix,
									  m_tempCenter,
									  NULL,
									  transRenderList,
									  renderList,
									  mirrorBased,
									  compoundMatrix);
	   }
}
//----------------------------------------------------------//
void SceneAssembly::SkeletalHiarcleMeshExtraction(  CSkeletonObject * skObj,
													D3DMATRIX &positionMatrix,
													BOOL & firstPersonMode,
													D3DMATRIX &camMatrix,
													BOOL & forceAdd,
													BOOL & renderInMirror,
													D3DMATRIX &invCameraMatrix,
													D3DMATRIX & shadowTransform,
													BOOL & enableModelShadowing,
													int & mirrorBased,
													D3DMATRIX * compoundMatrix,
													CTranslucentPolyList * transRenderList,
													CTranslucentPolyList * renderList)
{
	   for(POSITION hiarchPos = skObj->m_bonesList->GetHeadPosition();hiarchPos != NULL;)
	   {
          CBoneObject * bone = (CBoneObject *)skObj->m_bonesList->GetNext(hiarchPos);
		  //mesh hiarchle anim
		  if(bone->m_hiarchyMesh){//valid
			 CHiarcleVisObj * vPtr = bone->m_hiarchyMesh->GetLOD(skObj->m_currentLOD,m_gStatus->m_lodOverride);
			 if(!vPtr)
				 continue;
				   //D3DMATRIX boneMeshMatrix;
				   if(enableModelShadowing)
				      vPtr->m_mesh->List_materialObject->m_shadowObject = 1;//REMOVE

                   vPtr->m_mesh->m_visibilityTrack = bone->m_currentVisibility;

				   bone->m_boneFrame->GetTransform(NULL,&m_matrixTemp);
				   m_gStatus->m_matLib.fastMatrixMultiply(m_matrixTemp,m_matrixTemp,positionMatrix);
				   vPtr->m_mesh->m_currentMatrix = m_matrixTemp;
				   m_tempCenter.x = vPtr->m_mesh->m_currentMatrix._41;
				   m_tempCenter.y = vPtr->m_mesh->m_currentMatrix._42;
				   m_tempCenter.z = vPtr->m_mesh->m_currentMatrix._43;

				   if(firstPersonMode == FALSE)
				   CatigorizeMesh(     vPtr->m_mesh,
								       camMatrix,
								       m_tempCenter,
									   invCameraMatrix,
									   m_tempCenter,
									   NULL,
									   transRenderList,
									   renderList,
									   mirrorBased,
									   compoundMatrix);
		  }//end valid
	   }
}
//----------------------------------------------------------//
void SceneAssembly::SkeletalEmitterCallback(CSkeletonObject * skeletalPtr,
											D3DMATRIX * positionMatrix,
											D3DMATRIX & camMatrix,
											D3DMATRIX * compoundMatrix,
											CTranslucentPolyList * transRenderList)
{
	if(skeletalPtr->m_particleEmiterList)
	{
		CParticleEmitRangeObj * emmiter =  skeletalPtr->m_particleEmiterList->GetValidEmmitter(skeletalPtr->m_animationInUse,skeletalPtr->m_currentBaseFrame);
		if(emmiter)
		{//valid particle 
				POSITION particlePos = m_gStatus->m_particleDatabase->FindIndex(emmiter->m_particleDBIndex);
				if(particlePos)
				{//valid particle db object
					EnvParticleSystem* pSys = (EnvParticleSystem*)m_gStatus->m_particleDatabase->GetAt(particlePos);
					D3DMATRIX boneMatrix = skeletalPtr->GetBoneMatrixByIndex(emmiter->m_boneLink);
					if(positionMatrix)
					   m_gStatus->m_matLib.fastMatrixMultiply(boneMatrix,boneMatrix,*positionMatrix);
					
					m_tempCenter.x = boneMatrix._41;
					m_tempCenter.y = boneMatrix._42;
					m_tempCenter.z = boneMatrix._43;
					
                    if(compoundMatrix)
					{//compund transform
					   m_gStatus->m_matLib.fastMatrixMultiply(m_compoundTemp,boneMatrix,*compoundMatrix);
					   m_compoundCenterTemp =  m_gStatus->m_matLib.UpdatePolygonVertex(compoundMatrix->m,m_tempCenter);


					   transRenderList->CacheMesh(  NULL,
																	 pSys,
																	 camMatrix,
																	 m_compoundTemp,
																	 m_compoundCenterTemp,
																	 NULL);
					}
					else
					   transRenderList->CacheMesh(  NULL,
																	 pSys,
																	 camMatrix,
																	 boneMatrix,
																	 m_tempCenter,
																	 NULL);
				}//end valid particle db object
		}//end valid particle 
	}
}

