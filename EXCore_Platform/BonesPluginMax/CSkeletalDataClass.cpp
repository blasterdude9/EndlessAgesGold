//#include "stdafx.h"
#include "afxwin.h"
#include "afxext.h"
#define D3D_OVERLOADS
#include "resource.h"
#include <d3d9.h>
//#include <afxtempl.h>

#include "math.h"

#include "CSkeletalDataClass.h"
#include "CSkeletonClassFileIO.h"

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Constructor-----------------------------------//
CSkeletonObject::CSkeletonObject()
{
m_bonesList = NULL;
m_lastUpdateTime=0;
m_SkeletalName = "BASE";
m_frameSpeed = 0;
m_ticksPerFrame = 0;
m_animationInUse = -1;
m_lastAnimationInUse = -1;

m_secondaryAnimationInUse  = -1;
m_secondaryTransitionStamp = 0;
m_inSecondaryTransition    = FALSE;

m_deformableMeshArrayCount = 0;
m_shadowInterface = NULL;
m_shadowOn = FALSE;
m_box.maxX = 0.0f;
m_box.maxY = 0.0f;
m_box.maxZ = 0.0f;
m_box.minX = 0.0f;
m_box.minY = 0.0f;
m_box.minZ = 0.0f;
m_requireUpdate = FALSE;
m_freeLookBone = -1;
m_freeLookPitch = 0;
m_freeLookYaw = 0;
m_inTransition = FALSE;
m_transitionTime = 0;
m_transitionStartTime = 0;
m_transitionBaseFrame = 0;

m_lastDurationUpdateTime = 0;
m_durationCurrentLength = 0;

m_animPreference = 0;


m_sphereLocationalDamageList = NULL;
m_calulationSphere           = NULL;
m_sphereBodyCollisionSystem  = NULL;
m_damageSphereList           = NULL;
m_particleEmiterList         = NULL;
m_deformableMeshArrayV2      = NULL;
m_alternateConfigurations    = NULL;
m_spawnCfg                   = NULL;
m_fireQueList                = NULL;


m_waistBone     = -1;
m_waistTurn     = 0.0f;
m_waistTurnMax  = 50.0f;
m_waistTurnRate = 5.0f;
m_currentLOD    = 0;
m_secondaryCOMadjust.x        = 0.0f;
m_secondaryCOMadjust.y        = 0.0f;
m_secondaryCOMadjust.z        = 0.0f;

m_secondaryTransitionDuration = 200;

m_secondaryCOMInheritBone = -1;

m_transitionOut = FALSE;
m_lastScndDurationUpdateTime  = GetTickCount();
m_scndDurationCurrentLength   = 0;

m_primaryAnimLastUpdatedTime  = m_lastScndDurationUpdateTime;
m_secondaryAnimLastUpdatedTime = m_lastScndDurationUpdateTime;
m_inReverse = FALSE;
m_currentTimePassedPrimary = 0;
m_currentTimePassedSecondary = 0;
m_currentBaseFrame = 0;
m_secondaryBaseFrame = 0;
m_currentBaseSoundFrame = -1;
m_secondaryBaseSoundFrame = -1;

m_firstFrame = 0;
m_lastFrame = 0;
m_frameCount = 0;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::ScaleSystem(float percentage,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
  D3DMATRIX scaleMatrix;
 
  GL_matrixFunctions.scale (percentage,percentage,percentage,scaleMatrix.m );



  for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;)
  {
      CBoneObject * bonePtr = (CBoneObject *)m_bonesList->GetNext(posLoc);

	  D3DVECTOR tempPos;
	  tempPos.x = bonePtr->m_startPosition._41;
	  tempPos.y = bonePtr->m_startPosition._42;
	  tempPos.z = bonePtr->m_startPosition._43;
	  //GL_matrixFunctions.fastMatrixMultiply(bonePtr->m_startPosition,bonePtr->m_startPosition,scaleMatrix);
	  tempPos = GL_matrixFunctions.UpdatePolygonVertex(scaleMatrix.m,tempPos);
	  bonePtr->m_startPosition._41 = tempPos.x;
	  bonePtr->m_startPosition._42 = tempPos.y;
	  bonePtr->m_startPosition._43 = tempPos.z;

	  for(POSITION posLocal = bonePtr->m_animationList->GetHeadPosition();posLocal != NULL;)
	  {
          CBoneAnimationObject * animPtr = (CBoneAnimationObject *)bonePtr->m_animationList->GetNext(posLocal);
          for(int loop=0;loop<animPtr->m_totalFrames;loop++)
		  {
			  animPtr->m_animationData[loop].position = GL_matrixFunctions.UpdatePolygonVertex(scaleMatrix.m,animPtr->m_animationData[loop].position);
		  }

          if(bonePtr->m_hiarchyMesh)
		  for(POSITION hPos = bonePtr->m_hiarchyMesh->m_lodChain->GetHeadPosition();hPos != NULL;)
		  {
			  CHiarcleVisObj * hVisObj = (CHiarcleVisObj *)bonePtr->m_hiarchyMesh->m_lodChain->GetNext(hPos);
			  
			  hVisObj->m_mesh->ScaleIt(percentage);
		      hVisObj->m_mesh->InitBuffer(g_pd3dDevice,NULL);
		  }
	  }
  }



  //def mes
  if(m_deformableMeshArrayCount > 0)
	 {//deformable meshs
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++)
		  {//deformable mesh loop
			  if(m_alternateConfigurations)
			  {//alternate exists - this is based off of DB gen
			    POSITION alPos = m_alternateConfigurations->FindIndex(loop);
			    if(alPos){//valid configuration
			      CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
				  for(POSITION curCfgPos = altPtr->m_configurations->GetHeadPosition();curCfgPos != NULL;)
				  {//deformable unit same array ie head0 head1
			          CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetNext(curCfgPos);
                      for(POSITION dPos = dMesh->m_lodChain->GetHeadPosition();dPos != NULL;)
					  {
					      CDeformableVisObj * dVis = (CDeformableVisObj *)dMesh->m_lodChain->GetNext(dPos);
                          
						  ABVERTEX * vArray = NULL;
						  int count = 0;
						  dVis->m_mesh->m_abVertexArray.GetABVERTEXStyle(&vArray,&count);
						  
						  for(int vertexCompLoop = 0;vertexCompLoop < dVis->m_mesh->m_abVertexArray.m_vertexCount;vertexCompLoop++)
						  {//primary vertex loop
                             D3DVECTOR vertexTemp;
							 vertexTemp.x = vArray[vertexCompLoop].x;
							 vertexTemp.y = vArray[vertexCompLoop].y;
							 vertexTemp.z = vArray[vertexCompLoop].z;
                             vertexTemp = GL_matrixFunctions.UpdatePolygonVertex(scaleMatrix.m,vertexTemp);
							 vArray[vertexCompLoop].x = vertexTemp.x;
							 vArray[vertexCompLoop].y = vertexTemp.y;
							 vArray[vertexCompLoop].z = vertexTemp.z;
							 
						  }//end primary vertex loop

						  dVis->m_mesh->m_abVertexArray.SetABVERTEXStyle(NULL,vArray,count);
                          if(vArray)
						  delete [] vArray;
						  vArray = 0;

                          for(int x=0;x<dVis->m_mesh->m_abVertexArray.m_vertexCount;x++){
							  D3DVECTOR vertexTemp;
                              vertexTemp.x = dVis->m_originalArrayVector[x].x;
							  vertexTemp.y = dVis->m_originalArrayVector[x].y;
							  vertexTemp.z = dVis->m_originalArrayVector[x].z;
                              vertexTemp = GL_matrixFunctions.UpdatePolygonVertex(scaleMatrix.m,vertexTemp);
							  dVis->m_originalArrayVector[x].x = vertexTemp.x;
							  dVis->m_originalArrayVector[x].y = vertexTemp.y;
							  dVis->m_originalArrayVector[x].z = vertexTemp.z;

						  }


						  dVis->m_mesh->InitBuffer(g_pd3dDevice,NULL);
					  }
				  }//end deformable unit same array
	
				}//end valid configuration
			  }//end alternate exists - this is based off of DB gen
			  else{//based on runtime gen
                   //m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->Clone(&cloneReturn->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit);
			  }//end based on runtime gen
		  }//end deformable mesh loop
	 }//end deformable meshes


    if(m_sphereLocationalDamageList)
       for(POSITION posLoc = m_sphereLocationalDamageList->GetHeadPosition();posLoc !=NULL;)
	   {//damage point loop
           CSphereCollisionObj * sphereObj = (CSphereCollisionObj *)m_sphereLocationalDamageList->GetNext(posLoc);
		   sphereObj->m_sphereRadius = sphereObj->m_sphereRadius * percentage;
	   }//end damage point loop

	if(m_calulationSphere)
       m_calulationSphere->m_sphereRadius = m_calulationSphere->m_sphereRadius * percentage;


	if(m_sphereBodyCollisionSystem)
       for(POSITION csysPos = m_sphereBodyCollisionSystem->GetHeadPosition();csysPos != NULL;)
	   {//g system loop
		  CSphereCollisionObj * mainBdSphPtr = (CSphereCollisionObj *)m_sphereBodyCollisionSystem->GetNext(csysPos);
		  mainBdSphPtr->m_sphereRadius = mainBdSphPtr->m_sphereRadius * percentage;

	   }

	   if(m_damageSphereList)
       for(POSITION csysPos = m_damageSphereList->GetHeadPosition();csysPos != NULL;)
	   {//g system loop
		  CDamagePathObj* damagePathObj = (CDamagePathObj*)m_damageSphereList->GetNext(csysPos);
		  damagePathObj->m_radius = damagePathObj->m_radius * percentage;

	   }

	   if(m_fireQueList)
		for(POSITION firePos = m_fireQueList->GetHeadPosition();firePos != NULL;)
		{
            CFireRangeObject * fireRange = (CFireRangeObject *)m_fireQueList->GetNext(firePos);
            fireRange->m_hardPointAdjust.x = fireRange->m_hardPointAdjust.x * percentage;
			fireRange->m_hardPointAdjust.y = fireRange->m_hardPointAdjust.y * percentage;
			fireRange->m_hardPointAdjust.z = fireRange->m_hardPointAdjust.z * percentage;

		}

  return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
D3DMATRIX CSkeletonObject::GetBoneMatrixByIndex(int index)
{
D3DMATRIX boneMatrix;
POSITION posLoc = m_bonesList->FindIndex(index);
if(!posLoc){
	GL_matrixFunctions.InitMat(boneMatrix.m);
    return boneMatrix;
}
    CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
    bonePtr->m_boneFrame->GetTransform(NULL,&boneMatrix);
    return boneMatrix;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
CFrameObj * CSkeletonObject::GetBoneFrameByIndex(int index)
{
if(index < 0)
   return NULL;

POSITION posLoc = m_bonesList->FindIndex(index);
  if(!posLoc)
      return NULL;
      CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
      return bonePtr->m_boneFrame;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
D3DVECTOR CSkeletonObject::GetBonePositionByIndex(int index)
{
D3DVECTOR bonePosition;
bonePosition.x = 0.0f;
bonePosition.y = 0.0f;
bonePosition.z = 0.0f;
POSITION posLoc = m_bonesList->FindIndex(index);
if(!posLoc)
    return bonePosition;
    CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
    bonePtr->m_boneFrame->GetPosition(NULL,&bonePosition);
    return bonePosition;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------clone------------------------------------------//
void CSkeletonObject::UnifyAllNormalsDeformable(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     

     
     //do normal operation
     if(m_deformableMeshArrayCount > 0)
	 {//deformable meshs
		  ResetSkeletalNormalsDeformable(g_pd3dDevice);//reset
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++)
		  {//deformable mesh loop
			  if(m_alternateConfigurations)
			  {//alternate exists - this is based off of DB gen
			    POSITION alPos = m_alternateConfigurations->FindIndex(loop);
			    if(alPos){//valid configuration
			      CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
				  for(POSITION curCfgPos = altPtr->m_configurations->GetHeadPosition();curCfgPos != NULL;)
				  {//deformable unit same array ie head0 head1
			          CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetNext(curCfgPos);
                      for(int secondaryLoop=0;secondaryLoop<m_deformableMeshArrayCount;secondaryLoop++)
					  {//secondary loop  
						 if(secondaryLoop == loop)
							continue;
                            POSITION secondaryAltPos = m_alternateConfigurations->FindIndex(secondaryLoop);
			                if(alPos){//valid secondary configuration
			                   CAlterUnitDBObject * secondaryAltPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(secondaryAltPos);
                               for(POSITION scndCurCfgPos = secondaryAltPtr->m_configurations->GetHeadPosition();scndCurCfgPos != NULL;)
							   {//secondary deformable unit same array ie head0 head1
			                      CDeformableMesh * secondarydMesh = (CDeformableMesh *)secondaryAltPtr->m_configurations->GetNext(scndCurCfgPos);
                                  
								  BOOL done = FALSE;
								  int loop = 0;
								  while(!done)
								  {//lod loop

                                    POSITION scndLodPos = secondarydMesh->m_lodChain->FindIndex(loop);
								    POSITION lodPos = dMesh->m_lodChain->FindIndex(loop);

								    if(scndLodPos == NULL || lodPos == NULL)
								       break;
                                     CDeformableVisObj * scndDVis = (CDeformableVisObj *)secondarydMesh->m_lodChain->GetAt(scndLodPos);
                                     CDeformableVisObj * dVis = (CDeformableVisObj *)dMesh->m_lodChain->GetAt(lodPos);
									 
									 if(dVis != NULL && scndDVis != NULL)
									 {//unify
                                         ABVERTEX * vArray = NULL;
										 int vCount = 0;
										 dVis->m_mesh->m_abVertexArray.GetABVERTEXStyle(&vArray,&vCount);


                                         for(int vertexCompLoop = 0;vertexCompLoop < dVis->m_mesh->m_abVertexArray.m_vertexCount;vertexCompLoop++)
										 {//primary vertex loop
										   
										   ABVERTEX * vArrayScndary = NULL;
										   int vCountScndry = 0;
										   scndDVis->m_mesh->m_abVertexArray.GetABVERTEXStyle(&vArrayScndary,&vCountScndry);
											 
										   for(int scndVertexCompLoop = 0;scndVertexCompLoop < scndDVis->m_mesh->m_abVertexArray.m_vertexCount;scndVertexCompLoop++)
										   {//secondary vertex loop
                                            float xDiff = fabs(vArray[vertexCompLoop].x - vArrayScndary[scndVertexCompLoop].x);
                                            float yDiff = fabs(vArray[vertexCompLoop].y - vArrayScndary[scndVertexCompLoop].y);
											float zDiff = fabs(vArray[vertexCompLoop].z - vArrayScndary[scndVertexCompLoop].z);

										    if(xDiff < .01f &&
                                               yDiff < .01f &&
											   zDiff < .01f)
											{//same position unify the normals
											   if(secondaryLoop < loop)
											   {//copy normals
                                                 vArrayScndary[scndVertexCompLoop].nx = vArray[vertexCompLoop].nx;
											     vArrayScndary[scndVertexCompLoop].ny = vArray[vertexCompLoop].ny;
											     vArrayScndary[scndVertexCompLoop].nz = vArray[vertexCompLoop].nz;
											   }//end copy normals
											   else
											   {//blend vert normals
                                                 D3DVECTOR normal;
												 normal.x = vArrayScndary[scndVertexCompLoop].nx + vArray[vertexCompLoop].nx;
											     normal.y = vArrayScndary[scndVertexCompLoop].ny + vArray[vertexCompLoop].ny;
											     normal.z = vArrayScndary[scndVertexCompLoop].nz + vArray[vertexCompLoop].nz;
											     //normal = GL_matrixFunctions.Normalize(normal);
											     vArrayScndary[scndVertexCompLoop].nx = normal.x;
												 vArrayScndary[scndVertexCompLoop].ny = normal.y;
												 vArrayScndary[scndVertexCompLoop].nz = normal.z;
												 vArray[vertexCompLoop].nx         = normal.x;
												 vArray[vertexCompLoop].ny         = normal.y;
												 vArray[vertexCompLoop].nz         = normal.z;

											   }//end blend vert normals
											}//end same position unify the normals
										   }//end secondary vertex loop
                                           
										   if(vArrayScndary)
										   {
											   scndDVis->m_mesh->m_abVertexArray.SetABVERTEXStyle(NULL,vArrayScndary,vCountScndry);
											   delete [] vArrayScndary;
											   vArrayScndary = 0;
										   }
										   
										 }//end primary vertex loop

										 if(vArray)
										 {
											   dVis->m_mesh->m_abVertexArray.SetABVERTEXStyle(NULL,vArray,vCount);
											   delete [] vArray;
											   vArray = 0;
										 }
										 //scndDVis->m_mesh->m_abVertexArray.InitBuffer();
										 //dVis->m_mesh->m_abVertexArray.InitBuffer();
									 }//end unify
									 loop++;
								  }//end lod loop
								  
							   }//secondary deformable unit same array ie head0 head1
							}//end valid secondary configuration
					  }//end secondary loop  
				  }//end deformable unit same array
	
				}//end valid configuration
				
			  }//end alternate exists - this is based off of DB gen
			  else{//based on runtime gen
                   //m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->Clone(&cloneReturn->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit);
			  }//end based on runtime gen
		  }//end deformable mesh loop
		  
	 }//end deformable meshes
     NormalizeAllNormals(g_pd3dDevice);

}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------clone------------------------------------------//
void CSkeletonObject::UnifyAllNormalsDeformable2(LPDIRECT3DDEVICE9 g_pd3dDevice, float angTol)
{
     

     
     //do normal operation
     if(m_deformableMeshArrayCount > 0)
	 {//deformable meshs
		  ResetSkeletalNormalsDeformable(g_pd3dDevice);//reset
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++)
		  {//deformable mesh loop
			  if(m_deformableMeshArrayV2)
			  {//alternate exists - this is based off of DB gen
			          CDeformableMesh * dMesh = m_deformableMeshArrayV2[loop].m_defMeshVisualUnit;
                      for(int secondaryLoop=0;secondaryLoop<m_deformableMeshArrayCount;secondaryLoop++)
					  {//secondary loop  
						 if(secondaryLoop == loop)
							continue;
                            
			                      CDeformableMesh * secondarydMesh = m_deformableMeshArrayV2[secondaryLoop].m_defMeshVisualUnit;
                                  
								  BOOL done = FALSE;
								  int loop = 0;
								  while(!done)
								  {//lod loop

                                    POSITION scndLodPos = secondarydMesh->m_lodChain->FindIndex(loop);
								    POSITION lodPos = dMesh->m_lodChain->FindIndex(loop);

								    if(scndLodPos == NULL || lodPos == NULL)
								       break;
                                     CDeformableVisObj * scndDVis = (CDeformableVisObj *)secondarydMesh->m_lodChain->GetAt(scndLodPos);
                                     CDeformableVisObj * dVis = (CDeformableVisObj *)dMesh->m_lodChain->GetAt(lodPos);
									 
									 if(dVis != NULL && scndDVis != NULL)
									 {//unify
                                         ABVERTEX * vArray = NULL;
										 int vCount = 0;
										 dVis->m_mesh->m_abVertexArray.GetABVERTEXStyle(&vArray,&vCount);


                                         for(int vertexCompLoop = 0;vertexCompLoop < dVis->m_mesh->m_abVertexArray.m_vertexCount;vertexCompLoop++)
										 {//primary vertex loop
										   
										   ABVERTEX * vArrayScndary = NULL;
										   int vCountScndry = 0;
										   scndDVis->m_mesh->m_abVertexArray.GetABVERTEXStyle(&vArrayScndary,&vCountScndry);
											 
										   for(int scndVertexCompLoop = 0;scndVertexCompLoop < scndDVis->m_mesh->m_abVertexArray.m_vertexCount;scndVertexCompLoop++)
										   {//secondary vertex loop
                                            float xDiff = fabs(vArray[vertexCompLoop].x - vArrayScndary[scndVertexCompLoop].x);
                                            float yDiff = fabs(vArray[vertexCompLoop].y - vArrayScndary[scndVertexCompLoop].y);
											float zDiff = fabs(vArray[vertexCompLoop].z - vArrayScndary[scndVertexCompLoop].z);

										    if(xDiff > .01f ||
                                               yDiff > .01f ||
											   zDiff > .01f)
										    continue;

											D3DVECTOR tempOne,tempTwo;
                                            tempOne.x = vArray[vertexCompLoop].nx;
		                                    tempOne.y = vArray[vertexCompLoop].ny;
		                                    tempOne.z = vArray[vertexCompLoop].nz;
		                                    tempTwo.x = vArrayScndary[scndVertexCompLoop].nx;
		                                    tempTwo.y = vArrayScndary[scndVertexCompLoop].ny;
		                                    tempTwo.z = vArrayScndary[scndVertexCompLoop].nz;
	                                        float angleDiff = GL_matrixFunctions.GetAngleBetweenTwoVectors(tempOne,tempTwo);
                                            if(angleDiff <= angTol)
											{//same position unify the normals
											   /*if(secondaryLoop < loop)
											   {//copy normals
                                                 vArrayScndary[scndVertexCompLoop].nx = vArray[vertexCompLoop].nx;
											     vArrayScndary[scndVertexCompLoop].ny = vArray[vertexCompLoop].ny;
											     vArrayScndary[scndVertexCompLoop].nz = vArray[vertexCompLoop].nz;
											   }//end copy normals
											   else
											   {//blend vert normals*/
                                                 D3DVECTOR normal;
												 normal.x = vArrayScndary[scndVertexCompLoop].nx + vArray[vertexCompLoop].nx;
											     normal.y = vArrayScndary[scndVertexCompLoop].ny + vArray[vertexCompLoop].ny;
											     normal.z = vArrayScndary[scndVertexCompLoop].nz + vArray[vertexCompLoop].nz;
											     //normal = GL_matrixFunctions.Normalize(normal);
											     vArrayScndary[scndVertexCompLoop].nx = normal.x;
												 vArrayScndary[scndVertexCompLoop].ny = normal.y;
												 vArrayScndary[scndVertexCompLoop].nz = normal.z;
												 vArray[vertexCompLoop].nx         = normal.x;
												 vArray[vertexCompLoop].ny         = normal.y;
												 vArray[vertexCompLoop].nz         = normal.z;

											   //}//end blend vert normals
											}//end same position unify the normals
										   }//end secondary vertex loop
                                           
										   if(vArrayScndary)
										   {
											   scndDVis->m_mesh->m_abVertexArray.SetABVERTEXStyle(NULL,vArrayScndary,vCountScndry);
											   delete [] vArrayScndary;
											   vArrayScndary = 0;
										   }
										   
										 }//end primary vertex loop

										 if(vArray)
										 {
											   dVis->m_mesh->m_abVertexArray.SetABVERTEXStyle(NULL,vArray,vCount);
											   delete [] vArray;
											   vArray = 0;
										 }
										 //scndDVis->m_mesh->m_abVertexArray.InitBuffer();
										 //dVis->m_mesh->m_abVertexArray.InitBuffer();
									 }//end unify
									 loop++;
								  }//end lod loop
					  }//end secondary loop  
			  }//end alternate exists - this is based off of DB gen
			  else{//based on runtime gen
                   //m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->Clone(&cloneReturn->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit);
			  }//end based on runtime gen
		  }//end deformable mesh loop
		  
	 }//end deformable meshes
     NormalizeAllNormals2(g_pd3dDevice);

}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------clone------------------------------------------//
void CSkeletonObject::ResetSkeletalNormalsDeformable(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     //do normal operation
     if(m_deformableMeshArrayCount > 0)
	 {//deformable meshs
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++)
		  {//deformable mesh loop
			  if(m_alternateConfigurations)
			  {//alternate exists - this is based off of DB gen
			    POSITION alPos = m_alternateConfigurations->FindIndex(loop);
			    if(alPos){//valid configuration
			      CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
				  for(POSITION curCfgPos = altPtr->m_configurations->GetHeadPosition();curCfgPos != NULL;)
				  {//deformable unit same array ie head0 head1
			          CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetNext(curCfgPos);
                      for(POSITION dPos = dMesh->m_lodChain->GetHeadPosition();dPos != NULL;)
					  {
					      CDeformableVisObj * dVis = (CDeformableVisObj *)dMesh->m_lodChain->GetNext(dPos);
                          dVis->m_mesh->GenerateNormals(0);
						  //dVis->m_mesh->GenerateSmoothNormals();
						  dVis->m_mesh->InitBuffer(g_pd3dDevice,NULL);
					  }
				  }//end deformable unit same array
	
				}//end valid configuration
			  }//end alternate exists - this is based off of DB gen
			  else{//based on runtime gen
                   //m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->Clone(&cloneReturn->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit);
			  }//end based on runtime gen
		  }//end deformable mesh loop
	 }//end deformable meshes

}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------clone------------------------------------------//
void CSkeletonObject::Reinit(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     //do normal operation
     if(m_deformableMeshArrayCount > 0)
	 {//deformable meshs
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++)
		  {//deformable mesh loop
			  if(m_alternateConfigurations)
			  {//alternate exists - this is based off of DB gen
			    POSITION alPos = m_alternateConfigurations->FindIndex(loop);
			    if(alPos){//valid configuration
			      CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
				  for(POSITION curCfgPos = altPtr->m_configurations->GetHeadPosition();curCfgPos != NULL;)
				  {//deformable unit same array ie head0 head1
			          CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetNext(curCfgPos);
                      for(POSITION dPos = dMesh->m_lodChain->GetHeadPosition();dPos != NULL;)
					  {
					      CDeformableVisObj * dVis = (CDeformableVisObj *)dMesh->m_lodChain->GetNext(dPos);
						  dVis->m_mesh->InitBuffer(g_pd3dDevice,NULL);
					  }
				  }//end deformable unit same array
	
				}//end valid configuration
			  }//end alternate exists - this is based off of DB gen
			  else{//based on runtime gen
                   //m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->Clone(&cloneReturn->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit);
			  }//end based on runtime gen
		  }//end deformable mesh loop
	 }//end deformable meshes
	 if(m_bonesList){//bones
        for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;)
		{
            CBoneObject * bonePtr = (CBoneObject *)m_bonesList->GetNext(posLoc);
			if(bonePtr->m_hiarchyMesh)
			if(bonePtr->m_hiarchyMesh->m_lodChain)
			{
			   for(POSITION posLod = bonePtr->m_hiarchyMesh->m_lodChain->GetHeadPosition();posLod != NULL;)
			   {
				   CHiarcleVisObj * hPtr = (CHiarcleVisObj *)bonePtr->m_hiarchyMesh->m_lodChain->GetNext(posLod);
                   hPtr->m_mesh->InitBuffer(g_pd3dDevice,NULL);
			   }
			}
		}
     }//end bones

}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------clone------------------------------------------//
void CSkeletonObject::NormalizeAllNormals(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     //do normal operation
     if(m_deformableMeshArrayCount > 0)
	 {//deformable meshs
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++)
		  {//deformable mesh loop
			  if(m_alternateConfigurations)
			  {//alternate exists - this is based off of DB gen
			    POSITION alPos = m_alternateConfigurations->FindIndex(loop);
			    if(alPos){//valid configuration
			      CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
				  for(POSITION curCfgPos = altPtr->m_configurations->GetHeadPosition();curCfgPos != NULL;)
				  {//deformable unit same array ie head0 head1
			          CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetNext(curCfgPos);
                      for(POSITION dPos = dMesh->m_lodChain->GetHeadPosition();dPos != NULL;)
					  {
					      CDeformableVisObj * dVis = (CDeformableVisObj *)dMesh->m_lodChain->GetNext(dPos);
                          
						  ABVERTEX * vArray = NULL;
						  int vCount = 0;
						  dVis->m_mesh->m_abVertexArray.GetABVERTEXStyle(&vArray,&vCount);
						  
						  for(int vertexCompLoop = 0;vertexCompLoop < dVis->m_mesh->m_abVertexArray.m_vertexCount;vertexCompLoop++)
						  {//primary vertex loop
                             D3DVECTOR normalMod;
							 normalMod.x = vArray[vertexCompLoop].nx;
							 normalMod.y = vArray[vertexCompLoop].ny;
							 normalMod.z = vArray[vertexCompLoop].nz;
                             normalMod = GL_matrixFunctions.Normalize(normalMod);
							 vArray[vertexCompLoop].nx = normalMod.x;
							 vArray[vertexCompLoop].ny = normalMod.y;
							 vArray[vertexCompLoop].nz = normalMod.z;
						  }//end primary vertex loop

                          if(vArray)
						  {
                             dVis->m_mesh->m_abVertexArray.SetABVERTEXStyle(NULL,vArray,vCount);
                             delete [] vArray;
							 vArray = 0;
						  }


						  dVis->m_mesh->InitBuffer(g_pd3dDevice,NULL);
					  }
				  }//end deformable unit same array
	
				}//end valid configuration
			  }//end alternate exists - this is based off of DB gen
			  else{//based on runtime gen
                   //m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->Clone(&cloneReturn->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit);
			  }//end based on runtime gen
		  }//end deformable mesh loop
	 }//end deformable meshes

}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------clone------------------------------------------//
void CSkeletonObject::NormalizeAllNormals2(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     //do normal operation
     if(m_deformableMeshArrayCount > 0)
	 {//deformable meshs
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++)
		  {//deformable mesh loop
		
			          CDeformableMesh * dMesh = m_deformableMeshArrayV2[loop].m_defMeshVisualUnit;
                      for(POSITION dPos = dMesh->m_lodChain->GetHeadPosition();dPos != NULL;)
					  {
					      CDeformableVisObj * dVis = (CDeformableVisObj *)dMesh->m_lodChain->GetNext(dPos);
                          
						  ABVERTEX * vArray = NULL;
						  int vCount = 0;
						  dVis->m_mesh->m_abVertexArray.GetABVERTEXStyle(&vArray,&vCount);
						  
						  for(int vertexCompLoop = 0;vertexCompLoop < dVis->m_mesh->m_abVertexArray.m_vertexCount;vertexCompLoop++)
						  {//primary vertex loop
                             D3DVECTOR normalMod;
							 normalMod.x = vArray[vertexCompLoop].nx;
							 normalMod.y = vArray[vertexCompLoop].ny;
							 normalMod.z = vArray[vertexCompLoop].nz;
                             normalMod = GL_matrixFunctions.Normalize(normalMod);
							 vArray[vertexCompLoop].nx = normalMod.x;
							 vArray[vertexCompLoop].ny = normalMod.y;
							 vArray[vertexCompLoop].nz = normalMod.z;
						  }//end primary vertex loop

                          if(vArray)
						  {
                             dVis->m_mesh->m_abVertexArray.SetABVERTEXStyle(NULL,vArray,vCount);
                             delete [] vArray;
							 vArray = 0;
						  }


						  dVis->m_mesh->InitBuffer(g_pd3dDevice,NULL);
					  }
		  }//end deformable mesh loop
	 }//end deformable meshes

}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------clone------------------------------------------//
void CSkeletonObject::Clone(CSkeletonObject ** clone,
							LPDIRECT3DDEVICE9 g_pd3dDevice,
							int * spawnCfgOptional)
{	
CSkeletonObject * cloneReturn = new CSkeletonObject();

cloneReturn->m_frameSpeed                 = m_frameSpeed;
cloneReturn->m_SkeletalName               = m_SkeletalName;
cloneReturn->m_ticksPerFrame              = m_ticksPerFrame;
cloneReturn->m_deformableMeshArrayCount   = m_deformableMeshArrayCount;
cloneReturn->m_shadowOn                   = m_shadowOn;
cloneReturn->m_box.maxX                   = m_box.maxX;
cloneReturn->m_box.maxY                   = m_box.maxY;
cloneReturn->m_box.maxZ                   = m_box.maxZ;
cloneReturn->m_box.minX                   = m_box.minX;
cloneReturn->m_box.minY                   = m_box.minY;
cloneReturn->m_box.minZ                   = m_box.minZ;
cloneReturn->m_freeLookBone               = m_freeLookBone;
cloneReturn->m_freeLookPitch              = m_freeLookPitch;
cloneReturn->m_freeLookYaw                = m_freeLookYaw;
cloneReturn->m_waistBone                  = m_waistBone;
cloneReturn->m_waistTurnMax               = m_waistTurnMax;
cloneReturn->m_waistTurnRate              = m_waistTurnRate;
cloneReturn->m_secondaryCOMInheritBone    = m_secondaryCOMInheritBone;


cloneReturn->m_animPreference             = 0;
cloneReturn->m_lastDurationUpdateTime     = 0;
cloneReturn->m_durationCurrentLength      = 0;

   if(m_shadowOn==TRUE)//shadows
      m_shadowInterface->Clone(&cloneReturn->m_shadowInterface);
   
   
   
   cloneReturn->m_particleEmiterList = NULL;
   if(m_particleEmiterList)
   m_particleEmiterList->Clone(&cloneReturn->m_particleEmiterList);
   
   cloneReturn->m_calulationSphere = NULL;
   if(m_calulationSphere)
   m_calulationSphere->Clone(&cloneReturn->m_calulationSphere);
   
   cloneReturn->m_damageSphereList = NULL;
   if(m_damageSphereList)
	  m_damageSphereList->Clone(&cloneReturn->m_damageSphereList);

   cloneReturn->m_sphereBodyCollisionSystem = NULL;
   if(m_sphereBodyCollisionSystem)
      m_sphereBodyCollisionSystem->Clone(&cloneReturn->m_sphereBodyCollisionSystem);

   cloneReturn->m_sphereLocationalDamageList = NULL;
   if(m_sphereLocationalDamageList)
      m_sphereLocationalDamageList->Clone(&cloneReturn->m_sphereLocationalDamageList);
   
   if(m_bonesList){//bones
      m_bonesList->Clone(&cloneReturn->m_bonesList,g_pd3dDevice);
	  cloneReturn->BuildHiarchy();
   }//end bones

   if(m_fireQueList)
      m_fireQueList->Clone(&cloneReturn->m_fireQueList);
 
   if(m_deformableMeshArrayCount > 0){//deformable meshs
          cloneReturn->m_deformableMeshArrayV2 = new DeformableMeshStructV2[m_deformableMeshArrayCount];
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++){  
			  if(m_alternateConfigurations)
			  {//alternate exists - this is based off of DB gen
			    POSITION alPos = m_alternateConfigurations->FindIndex(loop);
			    if(alPos){
			      CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
			      
				  POSITION curCfgPos = NULL;
				  if(!spawnCfgOptional)
				  {
				      curCfgPos = altPtr->m_configurations->FindIndex(abs(m_spawnCfg[loop]));
				  }
				  else
				  {
				      curCfgPos = altPtr->m_configurations->FindIndex(abs(spawnCfgOptional[loop]));
				  }
			  
			       if(curCfgPos){
			         CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetAt(curCfgPos);
			         dMesh->Clone(&cloneReturn->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit,g_pd3dDevice);
				   }
				   else
				   cloneReturn->m_deformableMeshArrayCount--;
				}
			  }//end alternate exists - this is based off of DB gen
			  else{//based on runtime gen
                   m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->Clone(&cloneReturn->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit,g_pd3dDevice);
			  }//end based on runtime gen
		  }

		  if(m_deformableMeshArrayCount > 0)
		  {
		    cloneReturn->m_spawnCfg = new int[m_deformableMeshArrayCount];
            //CopyMemory(cloneReturn->m_spawnCfg,m_spawnCfg,(sizeof(int)*m_deformableMeshArrayCount));

			if(!spawnCfgOptional)
				CopyMemory(cloneReturn->m_spawnCfg,m_spawnCfg,(sizeof(int)*m_deformableMeshArrayCount));
		    else
				CopyMemory(cloneReturn->m_spawnCfg,spawnCfgOptional,(sizeof(int)*m_deformableMeshArrayCount));
		  }
   }//end deformable meshs
            
   *clone = cloneReturn;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------Deconstructor----------------------------------//
void CSkeletonObject::EnableShadow(BOOL enable,
								   float radius,
								   float collisionRadius,
								   float depth,
								   float hieght,
								   int   textureIndex,
								   CString textureSearchName)
{
	if(enable == FALSE){
	   m_shadowOn = FALSE;
	   if(m_shadowInterface){
          delete m_shadowInterface;
		  m_shadowInterface=0;
	   }
	}
	else{
       if(m_shadowInterface){
          delete m_shadowInterface;
		  m_shadowInterface=0;
	   }
        m_shadowInterface = new CShadowClass(radius,
			                                 collisionRadius,
											 hieght,
											 depth,
											 TRUE,
											 textureIndex,
											 textureSearchName);
		m_shadowOn = TRUE;
	}
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------Deconstructor----------------------------------//
void CSkeletonObject::Invalidate()
{
    if(m_bonesList)
	   m_bonesList->Invalidate();


    if(m_deformableMeshArrayCount > 0){
	   if(m_deformableMeshArrayV2)
       for(int loop=0;loop<m_deformableMeshArrayCount;loop++){
           if(m_deformableMeshArrayV2[loop].m_defMeshVisualUnit){
              m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->Invalidate();
		   }
	   }
	}

}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//-------------------Deconstructor----------------------------------//
void CSkeletonObject::SafeDelete()
{
	if(m_bonesList){
	   m_bonesList->SafeDelete();
       delete m_bonesList;
       m_bonesList=0;
	}
	if(m_shadowInterface){
		  m_shadowInterface->SafeDelete();
          delete m_shadowInterface;
		  m_shadowInterface=0;
	}
	if(m_deformableMeshArrayCount > 0){
	   if(m_deformableMeshArrayV2)
       for(int loop=0;loop<m_deformableMeshArrayCount;loop++){
           if(m_deformableMeshArrayV2[loop].m_defMeshVisualUnit){
              m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->SafeDelete();
			  delete m_deformableMeshArrayV2[loop].m_defMeshVisualUnit;
			  m_deformableMeshArrayV2[loop].m_defMeshVisualUnit=0;
		   }
	   }
	}
	 if(m_deformableMeshArrayV2){
	     delete [] m_deformableMeshArrayV2;
	     m_deformableMeshArrayV2=0;
	 }

	if(m_sphereLocationalDamageList){
       m_sphereLocationalDamageList->SafeDelete();
	   delete m_sphereLocationalDamageList;
	   m_sphereLocationalDamageList=0;
	}

    if(m_particleEmiterList){
       m_particleEmiterList->SafeDelete();
	   delete m_particleEmiterList;
	   m_particleEmiterList=0;
	}

    if(m_calulationSphere){
       delete m_calulationSphere;
	   m_calulationSphere = 0;
	}
   
    if(m_damageSphereList){
	   m_damageSphereList->SafeDelete();
	   delete m_damageSphereList;
	   m_damageSphereList=0;
	}

    if(m_sphereBodyCollisionSystem){
       m_sphereBodyCollisionSystem->SafeDelete();
	   delete m_sphereBodyCollisionSystem;
	   m_sphereBodyCollisionSystem=0;
	}

	if(m_alternateConfigurations){
       m_alternateConfigurations->SafeDelete();
	   delete m_alternateConfigurations;
	   m_alternateConfigurations=0;
	}

	if(m_fireQueList)
	{
       m_fireQueList->SafeDelete();
	   delete m_fireQueList;
	   m_fireQueList = 0;
	}

	if(m_spawnCfg)
	{
       delete [] m_spawnCfg;
       m_spawnCfg = 0;
	}
}

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Serialize-------------------------------------//
void CSkeletonObject::Serialize( CArchive& ar )
{
BOOL reserved = 0;

CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar << m_SkeletalName
		  << m_bonesList
          << m_frameSpeed
          << m_ticksPerFrame
		  << m_deformableMeshArrayCount
		  << m_shadowOn
		  << m_shadowInterface
		  << m_freeLookBone
		  << m_sphereLocationalDamageList
		  << m_calulationSphere
		  << m_sphereBodyCollisionSystem
		  << m_damageSphereList
		  << m_particleEmiterList
		  << m_waistBone
	      << m_waistTurn
	      << m_waistTurnMax
	      << m_waistTurnRate
		  << m_fireQueList;

	   ar << m_alternateConfigurations
		  << m_secondaryCOMInheritBone
		  << reserved
		  << reserved
		  << reserved;

	   for(int loop=0;loop<m_deformableMeshArrayCount;loop++){
           ar << m_spawnCfg[loop];
	   }
	  
	}
	else
	{
       ar >> m_SkeletalName
		  >> m_bonesList
		  >> m_frameSpeed
		  >> m_ticksPerFrame
		  >> m_deformableMeshArrayCount
		  >> m_shadowOn
		  >> m_shadowInterface
		  >> m_freeLookBone
		  >> m_sphereLocationalDamageList
		  >> m_calulationSphere
		  >> m_sphereBodyCollisionSystem
		  >> m_damageSphereList
		  >> m_particleEmiterList
	      >> m_waistBone
	      >> m_waistTurn
	      >> m_waistTurnMax
	      >> m_waistTurnRate
		  >> m_fireQueList;

	   BuildHiarchy();

	   m_deformableMeshArrayV2 = NULL;

	   ar >> m_alternateConfigurations
		  >> m_secondaryCOMInheritBone
		  >> reserved
		  >> reserved
		  >> reserved;

	   if(m_deformableMeshArrayCount > 0)
	   m_spawnCfg = new int[m_deformableMeshArrayCount];
	   for(int loop=0;loop<m_deformableMeshArrayCount;loop++){
           ar >> m_spawnCfg[loop];
	   }

       
	   m_currentLOD = 0;
	   m_lastUpdateTime            = 0;
	   m_animationInUse            = -1;
       m_lastAnimationInUse        = -1;
	   m_requireUpdate             = FALSE;
	   m_freeLookPitch             = 0;
       m_freeLookYaw               = 0;
	   m_inTransition              = FALSE;
       m_transitionTime            = 0;
       m_transitionStartTime       = 0;
	   m_transitionBaseFrame       = 0;
	   m_lastDurationUpdateTime    = 0;
       m_durationCurrentLength     = 0;
	   m_lastScndDurationUpdateTime= GetTickCount();
	   m_primaryAnimLastUpdatedTime  = m_lastScndDurationUpdateTime;
       m_secondaryAnimLastUpdatedTime = m_lastScndDurationUpdateTime;
       m_scndDurationCurrentLength = 0;
	   
	   m_inSecondaryTransition     = 0;
	   m_secondaryTransitionStamp  = 0;
	   m_inSecondaryTransition     = FALSE;
	   m_transitionOut             = FALSE;
	   m_secondaryTransitionDuration = 200;
	   m_secondaryCOMadjust.x        = 0.0f;
       m_secondaryCOMadjust.y        = 0.0f;
       m_secondaryCOMadjust.z        = 0.0f;
	   m_animPreference              = 0;
	   m_inReverse                   = FALSE;
	   m_currentTimePassedPrimary    = 0;
       m_currentTimePassedSecondary  = 0;
	   m_currentBaseFrame = 0;
       m_secondaryBaseFrame = 0;
	   m_currentBaseSoundFrame = -1;
       m_secondaryBaseSoundFrame = -1;
	   
	   QuerryBox();
	}
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Init Bone System------------------------------//
BOOL CSkeletonObject::InitBoneSystemFromPlugin(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	
	 CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		return FALSE;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);

     int physiqueModCount = 0;
	 int firstFrame = 0;
	 int lastFrame = 0;
	 int frameCount = 0;

	 int mod = 0;

	 the_in_Archive >> m_SkeletalName
		            >> m_frameSpeed
		            >> m_ticksPerFrame
		            >> m_bonesList
	          	    >> m_deformableMeshArrayCount
		            >> physiqueModCount
	                >> firstFrame
                    >> lastFrame
	                >> frameCount;
     
       if(m_deformableMeshArrayCount > 0){
	      m_deformableMeshArrayV2 = new DeformableMeshStructV2[m_deformableMeshArrayCount];
	      for(int loop=0;loop<m_deformableMeshArrayCount;loop++){
              the_in_Archive >> m_deformableMeshArrayV2[loop].m_defMeshVisualUnit;
		  }
	   }

     

	 m_alternateConfigurations = NULL;
	 //port configurations
     m_alternateConfigurations = new CAlterUnitDBObjectList;
	 if(m_deformableMeshArrayCount > 0)
	 {//valid deformables
		m_spawnCfg = new int[m_deformableMeshArrayCount];
	    ZeroMemory(m_spawnCfg,(sizeof(int)*m_deformableMeshArrayCount));
	    for(int loop=0;loop<m_deformableMeshArrayCount;loop++)
		{
           CAlterUnitDBObject * newCfg = new CAlterUnitDBObject();
		   newCfg->m_configurations    = new CDeformableMeshList();
           CDeformableMesh    * dMesh = NULL;

           m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->Clone(&dMesh,g_pd3dDevice);

		   newCfg->m_configurations->AddTail(dMesh);
		   m_alternateConfigurations->AddTail(newCfg);
		}
	 }//end valid deformables

	 //return FALSE;
     //strip animation from bone list
     if(m_bonesList){
        for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;)
		{
            CBoneObject * bonePtr = (CBoneObject *)m_bonesList->GetNext(posLoc);
			if(bonePtr->m_animationList){
			   bonePtr->m_animationList->SafeDelete();
			   delete bonePtr->m_animationList;
			   bonePtr->m_animationList=0;
			   bonePtr->m_animationList = new CBoneAnimationList;
			}
		}
	 }
	 
	 the_in_Archive.Close();
     fl.Close();
   
	 BuildHiarchy();
return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Init Bone System------------------------------//
BOOL CSkeletonObject::InitBoneSystem(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
InitBoneSystemFromPlugin(fileName,g_pd3dDevice);
QuerryBox();

return TRUE;
}

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Init Bone System------------------------------//
BOOL CSkeletonObject::ApplyAnimation(int animation,int frame)
{
    for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
        CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
		bonePtr->ApplyAnimation(animation,frame);
	}
	return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Init Bone System------------------------------//
BOOL CSkeletonObject::ApplyAnimationByTime(int animation,int time)
{
    for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
        CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
		bonePtr->ApplyAnimationByTime(animation,time);
	}
	return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::SetAnimationSpeedBasedOnCap(int animNum,float percentage)
{
     //ofsset	 
     int currentTime = GetTickCount();
     BOOL calced = FALSE;
     if(m_bonesList)
     for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
         CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
		 if(bonePtr->m_animationList){
		 POSITION posAnim = bonePtr->m_animationList->FindIndex(animNum);
		  if(posAnim){
			CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetAt(posAnim);
            int newspeed = (int)((animPtr->m_timingCapMax * percentage)+animPtr->m_timingCapMin);
		    SetAnimationSpeed(animNum,newspeed);
		  }
		 }
	 }

	 return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::FadeOut(long duration)
{
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++){  
			  for(POSITION posLoc = m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetHeadPosition();posLoc != NULL;)
			  {
                  CDeformableVisObj * defMeshPtr = (CDeformableVisObj *) m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetNext(posLoc);
                  defMeshPtr->m_mesh->List_materialObject->SetFadeOutAlpha(duration);
				  //defMeshPtr->m_mesh->List_materialObject->m_blendMode = 1;
			  }
		  }

          for(POSITION bnPos = m_bonesList->GetHeadPosition();bnPos != NULL;)
		  {
              CBoneObject * boneObj = (CBoneObject *) m_bonesList->GetNext(bnPos);
			  if(boneObj->m_hiarchyMesh)
			  { 
				 for(POSITION hPos = boneObj->m_hiarchyMesh->m_lodChain->GetHeadPosition();hPos != NULL;)
				 {
					 CHiarcleVisObj * hPtr = (CHiarcleVisObj *)boneObj->m_hiarchyMesh->m_lodChain->GetNext(hPos);
					 hPtr->m_mesh->List_materialObject->SetFadeOutAlpha(duration);
					 //hPtr->m_mesh->List_materialObject->m_blendMode = 1;
				 }
			  }
		  }

return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::FadeIn(long duration)
{

		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++){  
			  if(m_deformableMeshArrayV2[loop].m_defMeshVisualUnit)
			  if(m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain)
			  for(POSITION posLoc = m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetHeadPosition();posLoc != NULL;)
			  {
                  CDeformableVisObj * defMeshPtr = (CDeformableVisObj *) m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetNext(posLoc);
                  defMeshPtr->m_mesh->List_materialObject->SetFadeInAlpha(duration);
				  //defMeshPtr->m_mesh->List_materialObject->m_blendMode = 1;
			  }
		  }

		  if(m_bonesList)
          for(POSITION bnPos = m_bonesList->GetHeadPosition();bnPos != NULL;)
		  {
              CBoneObject * boneObj = (CBoneObject *) m_bonesList->GetNext(bnPos);
			  if(boneObj->m_hiarchyMesh)
			  { 
				 for(POSITION hPos = boneObj->m_hiarchyMesh->m_lodChain->GetHeadPosition();hPos != NULL;)
				 {
					 CHiarcleVisObj * hPtr = (CHiarcleVisObj *)boneObj->m_hiarchyMesh->m_lodChain->GetNext(hPos);
					 hPtr->m_mesh->List_materialObject->SetFadeInAlpha(duration);
					 //hPtr->m_mesh->List_materialObject->m_blendMode = 1;
				 }
			  }
		  }

return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::FadeInAmplified(long duration)
{
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++){  
			  for(POSITION posLoc = m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetHeadPosition();posLoc != NULL;)
			  {
                  CDeformableVisObj * defMeshPtr = (CDeformableVisObj *) m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetNext(posLoc);
                  defMeshPtr->m_mesh->List_materialObject->SetFadeIn(duration,TRUE);
				  //defMeshPtr->m_mesh->List_materialObject->m_blendMode = 1;
			  }
		  }

          for(POSITION bnPos = m_bonesList->GetHeadPosition();bnPos != NULL;)
		  {
              CBoneObject * boneObj = (CBoneObject *) m_bonesList->GetNext(bnPos);
			  if(boneObj->m_hiarchyMesh)
			  { 
				 for(POSITION hPos = boneObj->m_hiarchyMesh->m_lodChain->GetHeadPosition();hPos != NULL;)
				 {
					 CHiarcleVisObj * hPtr = (CHiarcleVisObj *)boneObj->m_hiarchyMesh->m_lodChain->GetNext(hPos);
					 hPtr->m_mesh->List_materialObject->SetFadeIn(duration,TRUE);
					 //hPtr->m_mesh->List_materialObject->m_blendMode = 1;
				 }
			  }
		  }

return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::FadeOutAmplified(long duration)
{
		  for(int loop=0;loop<m_deformableMeshArrayCount;loop++){  
			  for(POSITION posLoc = m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetHeadPosition();posLoc != NULL;)
			  {
                  CDeformableVisObj * defMeshPtr = (CDeformableVisObj *) m_deformableMeshArrayV2[loop].m_defMeshVisualUnit->m_lodChain->GetNext(posLoc);
                  defMeshPtr->m_mesh->List_materialObject->SetFadeOut(duration);
				  //defMeshPtr->m_mesh->List_materialObject->m_blendMode = 1;
			  }
		  }

          for(POSITION bnPos = m_bonesList->GetHeadPosition();bnPos != NULL;)
		  {
              CBoneObject * boneObj = (CBoneObject *) m_bonesList->GetNext(bnPos);
			  if(boneObj->m_hiarchyMesh)
			  { 
				 for(POSITION hPos = boneObj->m_hiarchyMesh->m_lodChain->GetHeadPosition();hPos != NULL;)
				 {
					 CHiarcleVisObj * hPtr = (CHiarcleVisObj *)boneObj->m_hiarchyMesh->m_lodChain->GetNext(hPos);
					 hPtr->m_mesh->List_materialObject->SetFadeOut(duration);
					 //hPtr->m_mesh->List_materialObject->m_blendMode = 1;
				 }
			  }
		  }

return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::SetAnimationSpeed(int animNum,long newSpeed)
{
     //ofsset	 
     int currentTime = GetTickCount();
     BOOL calced = FALSE;

     for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
         CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
	     POSITION posAnim = bonePtr->m_animationList->FindIndex(animNum);
		  if(posAnim){
			CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetAt(posAnim);
            
			if(calced == FALSE){//do once
			long animLength = animPtr->m_totalFrames*animPtr->m_millisecondsPerFrame;
			long currentTimePassed = currentTime - m_lastUpdateTime;
			animPtr->m_millisecondsPerFrame = newSpeed;
		    long animLength2 = animPtr->m_totalFrames*animPtr->m_millisecondsPerFrame;
			float var1 = (float)animLength;
			float var2 = (float)animLength2;
			float percentage = var2 / var1;
			currentTimePassed = (long)(currentTimePassed*percentage);
			m_lastUpdateTime = currentTime - currentTimePassed;
			calced = TRUE;
			}//end do once
			else
			animPtr->m_millisecondsPerFrame = newSpeed;
		  }
	 }

	 return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
int CSkeletonObject::GetAnimationIndexFromAttrib(int attrib)
{
   POSITION posLoc = m_bonesList->FindIndex(0);
   if(!posLoc)
	   return -1;
       CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
	   if(!bonePtr->m_animationList)
		   return -1;
	       int trace=0; 
	       for(posLoc = bonePtr->m_animationList->GetHeadPosition();posLoc != NULL;)
		   {
		       CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetNext(posLoc);
	           if(animPtr->m_type == attrib)
				  return trace;
		       trace++;
		   }
	return -1;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::GetAnimationStatus(int *version,int *attrib,int animIndex)
{
   POSITION posLoc = m_bonesList->FindIndex(0);
   if(!posLoc)
	   return FALSE;
       CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
	   if(!bonePtr->m_animationList)
		   return FALSE;
	       POSITION animPos = bonePtr->m_animationList->FindIndex(animIndex);
		   if(!animPos)
			   return FALSE;
		       CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetAt(animPos);
	   
               *version = animPtr->m_version;
			   *attrib = animPtr->m_type;

	return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::SetAnimationBasedOnAttrib(int attrib,
												BOOL reverse,
												int versionPref,
												int transitionTime)
{
   int trace = 0;
   if(!m_bonesList)
	   return FALSE;

   POSITION posLoc = m_bonesList->FindIndex(0);
   if(posLoc){
      CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
      if(!bonePtr->m_animationList)
		  return FALSE;
      //randomize?
	  int versionDuplicateCount = 0;
	  POSITION animPos;
      for(animPos = bonePtr->m_animationList->GetHeadPosition();animPos != NULL;)
	  {
	    CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetNext(animPos);
        if(animPtr->m_type == attrib && animPtr->m_version == versionPref){
	       versionDuplicateCount++;
		}
	  }
	  int randomWinner = 0;
	  if(versionDuplicateCount > 0)
		 randomWinner = rand()%versionDuplicateCount;
      //end randomize
      int randTrace = 0;
	  for(animPos = bonePtr->m_animationList->GetHeadPosition();animPos != NULL;)
	  {
	    CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetNext(animPos);
        if(animPtr->m_type == attrib && animPtr->m_version == versionPref){
		  if(randomWinner == randTrace){//1
		   

			if(!SetCurrentAnimation(trace,transitionTime))
				return FALSE;

			m_durationCurrentLength = animPtr->m_minPlayDuration;
		    m_lastDurationUpdateTime = GetTickCount();
			
			//m_primaryAnimLastUpdatedTime = GetTickCount();//EXPERIMENT
			
			
		    SetAnimationDirection(trace,reverse); 
			
		   
            return TRUE;
		  }//end 1
		  randTrace++;
		}
	    trace++;
	  }
   }
return FALSE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
int  CSkeletonObject::GetAnimationBasedOnAttribAndPref(int attrib,
												       int versionPref,
													   CBoneAnimationObject ** returnPtr)
{
   int trace = 0;
   if(!m_bonesList)
	   return -1;

   POSITION posLoc = m_bonesList->FindIndex(0);
   if(posLoc){
      CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
      if(!bonePtr->m_animationList)
		  return -1;
      //randomize?
	  int versionDuplicateCount = 0;
	  POSITION animPos;
      for(animPos = bonePtr->m_animationList->GetHeadPosition();animPos != NULL;)
	  {
	    CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetNext(animPos);
        if(animPtr->m_type == attrib && animPtr->m_version == versionPref){
	       versionDuplicateCount++;
		}
	  }
	  int randomWinner = 0;
	  if(versionDuplicateCount > 0)
		 randomWinner = rand()%versionDuplicateCount;
	  else
		 return -1;
      //end randomize
      int randTrace = 0;
	  for(animPos = bonePtr->m_animationList->GetHeadPosition();animPos != NULL;)
	  {
	    CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetNext(animPos);
        if(animPtr->m_type == attrib && animPtr->m_version == versionPref){
		  if(randomWinner == randTrace){//1
			 if(returnPtr)
			   *returnPtr = animPtr;
             return trace;
		  }//end 1
		  randTrace++;
		}
	    trace++;
	  }
   }
return -1;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::SetCurrentAnimation(int anim,int transitionTime)
{
if(anim == m_animationInUse)
   return TRUE;

m_lastUpdateTime = GetTickCount();
m_animationInUse = anim;

POSITION posLoc = m_bonesList->FindIndex(0);
   if(!posLoc)
	   return FALSE;
       CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
       if(!bonePtr->m_animationList)
	   return FALSE;
          POSITION animPos = bonePtr->m_animationList->FindIndex(anim);
		  if(!animPos)
              return FALSE;
	          CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetAt(animPos);

              m_primaryAnimLastUpdatedTime = m_lastUpdateTime;

return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::SetCurrentAnimationByVersion(int version,int transitionTime)
{
   int trace = 0;
   if(!m_bonesList)
	   return FALSE;
   POSITION posLoc = m_bonesList->FindIndex(0);
   if(posLoc){
      CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
      if(bonePtr->m_animationList)
	  for(POSITION animPos = bonePtr->m_animationList->GetHeadPosition();animPos != NULL;)
	  {
	    CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetNext(animPos);
        if(animPtr->m_version == version){
	       m_durationCurrentLength = animPtr->m_minPlayDuration;
		   m_lastDurationUpdateTime = GetTickCount();
		   //m_primaryAnimLastUpdatedTime = GetTickCount();//EXPERIMENT
			SetAnimationDirection(trace,FALSE);
			SetCurrentAnimation(trace,transitionTime);
           return TRUE;
		}
	    trace++;
	  }
   }
return FALSE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::SetAnimationDirection(int anim,BOOL reverse)
{
   if(!m_bonesList)
	   return FALSE;
   POSITION posLoc = m_bonesList->FindIndex(0);
   if(posLoc){//valid
    CBoneObject * bone = (CBoneObject *)m_bonesList->GetAt(posLoc);
	if(!bone->m_animationList)
        return FALSE; 
	POSITION animPos = bone->m_animationList->FindIndex(anim);
	  if(animPos){//valid 2
       
	   CBoneAnimationObject * animPtr = (CBoneAnimationObject *)bone->m_animationList->GetAt(animPos);
	   if(m_inReverse != reverse){//affect
		long currentTime = GetTickCount();
        long timePassed = currentTime - m_lastUpdateTime;
        long animLength = animPtr->m_millisecondsPerFrame * animPtr->m_totalFrames;
        long timeLeft   = animLength - timePassed;
		m_lastUpdateTime = currentTime - timeLeft;
        m_inReverse = reverse;
		return TRUE;
	   }//end valid 2
	  }//end effect
   }//end valid
   return FALSE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::GetAnimationDirection(int anim)//forward = false
{
   if(!m_bonesList)
	   return FALSE;
   POSITION posLoc = m_bonesList->FindIndex(0);
   if(posLoc){//valid
    CBoneObject * bone = (CBoneObject *)m_bonesList->GetAt(posLoc);
	if(!bone->m_animationList)
        return FALSE; 
	POSITION animPos = bone->m_animationList->FindIndex(anim);
	  if(animPos){//valid 2
       
	   CBoneAnimationObject * animPtr = (CBoneAnimationObject *)bone->m_animationList->GetAt(animPos);
	   if(m_inReverse == TRUE)
		  return TRUE;
	   else 
		  return FALSE;
	  }//end valid 2
   }//end valid
   return FALSE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CSkeletonObject::UpdateAnimMS(CBoneAnimationObject * animPtr,
										  long currentTime,
										  BOOL secondary)
{
        long miliPerFrame = animPtr->m_millisecondsPerFrame;
			
			if(miliPerFrame < 1)
			   miliPerFrame = 1;
			//interpolated variables
			long animLength = animPtr->m_totalFrames*miliPerFrame;
			
			int currentTimePassed = 1;

			if(!secondary)
			    currentTimePassed = currentTime - m_primaryAnimLastUpdatedTime;
			else
				currentTimePassed = currentTime - m_secondaryAnimLastUpdatedTime;

			if(currentTimePassed >= animLength){
			   if(animPtr->m_looping == FALSE)
                  currentTimePassed = animLength-1;
			   else{
			      
				  if(!secondary)
				      m_primaryAnimLastUpdatedTime = currentTime;
				  else
                      m_secondaryAnimLastUpdatedTime = currentTime;
			      
				  
				  currentTimePassed = currentTimePassed - animLength;
			      if(currentTimePassed>animLength){
				     currentTimePassed = 0;//reset
				  }
			   }
			}
           

			//forward
               if(!secondary)
			       m_currentBaseFrame = currentTimePassed/miliPerFrame;
			   else
			       m_secondaryBaseFrame = currentTimePassed/miliPerFrame;
               
			//REVERSE //WORKING
		    if(m_inReverse == TRUE && animPtr->m_looping == TRUE){//switch animation to reverse
               //animPtr->m_baseFrame = (animPtr->m_totalFrames - animPtr->m_baseFrame)-1;
			   if(!secondary)
			       m_currentBaseFrame = (animPtr->m_totalFrames - m_currentBaseFrame)-1;
			   else
			       m_secondaryBaseFrame = (animPtr->m_totalFrames - m_secondaryBaseFrame)-1;
			}//end switch animation to reverse  			
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CSkeletonObject::HandleSoundRangesForAnimation(CBoneAnimationObject * animPtr,
														   D3DVECTOR & wldSpacePos,
														   float & globalSoundFactor,
														   CSoundBankList * m_soundManager,
														   BOOL secondary)
{
//sound activation ranges

	int baseFrameLoc = m_currentBaseFrame;
    if(secondary)
	   baseFrameLoc = m_secondaryBaseFrame;

    if(!secondary)
	{
	    if(m_currentBaseSoundFrame == m_currentBaseFrame)
		   return;

		m_currentBaseSoundFrame = m_currentBaseFrame;
	}
	else{
        if(m_secondaryBaseSoundFrame == m_secondaryBaseFrame)
		   return;

		m_secondaryBaseSoundFrame = m_secondaryBaseFrame;
	}




			if(animPtr->m_soundRanges)
				for(POSITION posSound = animPtr->m_soundRanges->GetHeadPosition();posSound != NULL;)
				{//SOUND LOOP
                   CSoundRangeObject* sndRange = (CSoundRangeObject*)animPtr->m_soundRanges->GetNext(posSound);
				   if(baseFrameLoc >= sndRange->m_startRange && baseFrameLoc <= sndRange->m_endRange){//in range
                       if(sndRange->m_inRange == FALSE){
						    /*m_soundManager->PlayByID(sndRange->m_id,
								                       FALSE,
													   wldSpacePos,
													   sndRange->m_soundFactor,
													   sndRange->m_radiusClipping);*/
								 
							sndRange->m_inRange = TRUE;
							//AfxMessageBox("play");
					   }
				   }//end range
				   else//tag out of range
				    sndRange->m_inRange = FALSE;
				}//END SOUND LOOP
            //end sound activation ranges
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CSkeletonObject::ApplyFreeLook()
{
  if(m_freeLookBone != -1){//valid freelook bone
     POSITION secondaryPos = m_bonesList->FindIndex(m_freeLookBone);
	 if(secondaryPos){//freelook secondary
	    CBoneObject * boneObject = (CBoneObject *)m_bonesList->GetAt(secondaryPos);
        D3DMATRIX applyMatrix;
	    GL_matrixFunctions.Rotate(0.0f,0.0f,m_freeLookPitch,applyMatrix.m);
	    boneObject->m_boneFrame->AddTransformBefore(applyMatrix);
	    GL_matrixFunctions.Rotate(0.0f,m_freeLookYaw,0.0f,applyMatrix.m);
	    boneObject->m_boneFrame->AddTransformAfter(applyMatrix);
	 }//end freelook secondary
  }//end valid freelook bone
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CSkeletonObject::ApplySecondaryAdjust(int secondaryAdjustBone)
{
  if(secondaryAdjustBone != -1){//valid bone
     POSITION secondaryPos = m_bonesList->FindIndex(m_freeLookBone);
	 if(secondaryPos){//1
	    CBoneObject * boneObject = (CBoneObject *)m_bonesList->GetAt(secondaryPos);
        D3DMATRIX applyMatrix;
	    GL_matrixFunctions.Translate(m_secondaryCOMadjust.x,m_secondaryCOMadjust.y,m_secondaryCOMadjust.z,applyMatrix.m);
	    boneObject->m_boneFrame->AddTransformAfter(applyMatrix);
	 }//end 1
  }//end bone
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CSkeletonObject::RemoveSecondaryAdjust(int secondaryAdjustBone)
{
  if(secondaryAdjustBone != -1){//valid bone
     POSITION secondaryPos = m_bonesList->FindIndex(m_freeLookBone);
	 if(secondaryPos){//1
	    CBoneObject * boneObject = (CBoneObject *)m_bonesList->GetAt(secondaryPos);
        D3DMATRIX applyMatrix;
	    GL_matrixFunctions.Translate(-m_secondaryCOMadjust.x,-m_secondaryCOMadjust.y,-m_secondaryCOMadjust.z,applyMatrix.m);
	    boneObject->m_boneFrame->AddTransformAfter(applyMatrix);
	 }//end 1
  }//end bone
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CSkeletonObject::RemoveFreeLook()
{
  if(m_freeLookBone != -1){//valid freelook bone
     POSITION secondaryPos = m_bonesList->FindIndex(m_freeLookBone);
	 if(secondaryPos){//freelook secondary
	    CBoneObject * boneObject = (CBoneObject *)m_bonesList->GetAt(secondaryPos);
        D3DMATRIX applyMatrix;
	    GL_matrixFunctions.Rotate(0.0f,0.0f,-m_freeLookPitch,applyMatrix.m);
	    boneObject->m_boneFrame->AddTransformBefore(applyMatrix);
	    GL_matrixFunctions.Rotate(0.0f,-m_freeLookYaw,0.0f,applyMatrix.m);
	    boneObject->m_boneFrame->AddTransformAfter(applyMatrix);
	 }//end freelook secondary
  }//end valid freelook bone
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CSkeletonObject::ApplyWaisteControl()
{
               if(m_waistBone != -1){//waiste control
				 POSITION secondaryPos = m_bonesList->FindIndex(m_waistBone);
				 if(secondaryPos){//freelook secondary
                  CBoneObject * boneObject = (CBoneObject *)m_bonesList->GetAt(secondaryPos);
                   D3DMATRIX applyMatrix;
				   GL_matrixFunctions.Rotate(0.0f,m_waistTurn,0.0f,applyMatrix.m);
	               boneObject->m_boneFrame->AddTransformAfter(applyMatrix);
				   secondaryPos = m_bonesList->FindIndex(m_waistBone+1);
				   if(secondaryPos){//unhook
                      CBoneObject * boneObject = (CBoneObject *)m_bonesList->GetAt(secondaryPos);
                      D3DMATRIX applyMatrix;
				      GL_matrixFunctions.Rotate(0.0f,-m_waistTurn,0.0f,applyMatrix.m);
	                  boneObject->m_boneFrame->AddTransformAfter(applyMatrix);
				   }//end unhook
				 }//end freelook secondary
			   }//end waiste control*
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CSkeletonObject::RemoveWaisteControl()
{
               if(m_waistBone != -1){//waiste control
				 POSITION secondaryPos = m_bonesList->FindIndex(m_waistBone);
				 if(secondaryPos){//freelook secondary
                  CBoneObject * boneObject = (CBoneObject *)m_bonesList->GetAt(secondaryPos);
                   D3DMATRIX applyMatrix;
				   GL_matrixFunctions.Rotate(0.0f,-m_waistTurn,0.0f,applyMatrix.m);
	               boneObject->m_boneFrame->AddTransformAfter(applyMatrix);
				   secondaryPos = m_bonesList->FindIndex(m_waistBone+1);
				   if(secondaryPos){//unhook
                      CBoneObject * boneObject = (CBoneObject *)m_bonesList->GetAt(secondaryPos);
                      D3DMATRIX applyMatrix;
				      GL_matrixFunctions.Rotate(0.0f,m_waistTurn,0.0f,applyMatrix.m);
	                  boneObject->m_boneFrame->AddTransformAfter(applyMatrix);
				   }//end unhook
				 }//end freelook secondary
			   }//end waiste control*
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 void CSkeletonObject::MoveBoneMangement(long & currentTime,
											   long & millisecondsPerFrame,
											   CBoneObject * bonePtr,
				                               int & locAnimInUse,
			                                   int & locanimationFrame,
											   BOOL & update,
											   long & overtimeReturn
											   )
{
               /*  if(!bonePtr->m_lastPositionsAreInitialized)
					 bonePtr->SetBoneToAnimationFrame(locAnimInUse,
										              locanimationFrame,
										              millisecondsPerFrame,
										              currentTime);

				 long boneTimePassed = currentTime - bonePtr->m_lastToTargetTimeStamp;
				 if(boneTimePassed >= millisecondsPerFrame){
					overtimeReturn = boneTimePassed - millisecondsPerFrame;
                    bonePtr->SetBoneToAnimationFrame(locAnimInUse,
										             locanimationFrame,
										             millisecondsPerFrame,
										             currentTime);

					if(overtimeReturn > 0 && overtimeReturn <= millisecondsPerFrame)
					{
                       bonePtr->m_lastToTargetTimeStamp -= overtimeReturn;
                       boneTimePassed = overtimeReturn;//overtime;
					}
					else
					boneTimePassed = 0;
				 }


				 if(boneTimePassed > millisecondsPerFrame)
					boneTimePassed = millisecondsPerFrame;
			
                 
                    D3DVECTOR newPosition = GL_matrixFunctions.InterpolatePoints(
		                                                               bonePtr->m_lastPosition, 
						                                               bonePtr->m_targetPosition, 
					                                                   millisecondsPerFrame, 
					                                                   boneTimePassed);
					D3DVECTOR upVector    = GL_matrixFunctions.InterpolatePoints(
		                                                               bonePtr->m_lastUpVector, 
						                                               bonePtr->m_targetUpVector, 
					                                                   millisecondsPerFrame, 
					                                                   boneTimePassed);
					D3DVECTOR dirVector   = GL_matrixFunctions.InterpolatePoints(
		                                                               bonePtr->m_lastDirVector, 
						                                               bonePtr->m_targetDirVector, 
					                                                   millisecondsPerFrame, 
					                                                   boneTimePassed);

					upVector  = GL_matrixFunctions.Normalize(upVector);
                    dirVector = GL_matrixFunctions.Normalize(dirVector);

                    GL_matrixFunctions.SetOrientation(&bonePtr->m_boneFrame->List_frameMatrix,
								                       dirVector,
								                       upVector);
                    bonePtr->m_boneFrame->List_frameMatrix._41 = newPosition.x;
                    bonePtr->m_boneFrame->List_frameMatrix._42 = newPosition.y;
                    bonePtr->m_boneFrame->List_frameMatrix._43 = newPosition.z;*/
					
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
 BOOL CSkeletonObject::AnimationCallback(   long currentTime,
											D3DMATRIX & earCenter,
											D3DVECTOR & wldSpacePos,
											float globalSoundFactor,
											BOOL soundOnly,
											CSoundBankList * m_soundManager,
											BOOL disableSoundOption,
											BOOL & disableDeformbleCalc,
											LPDIRECT3DDEVICE9 g_pd3dDevice)
{
    if(m_animationInUse == -1)//no animation current
       return FALSE;
	BOOL loopingAnim = FALSE;
    if(!m_bonesList)
	    return FALSE;
	//convert to ear space
	D3DVECTOR localSpacePos;
	ZeroMemory(&localSpacePos,sizeof(D3DVECTOR));
    if(!disableSoundOption)
    localSpacePos = GL_matrixFunctions.GetLocalPositionFromMatrixView(earCenter,wldSpacePos);


	POSITION posLoc = m_bonesList->FindIndex(0);
     if(!posLoc)
		 return FALSE;
	    CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetAt(posLoc);
		if(!bonePtr->m_animationList)
			return FALSE;
		POSITION posAnim = bonePtr->m_animationList->FindIndex(m_animationInUse);
		if(!posAnim)
			return FALSE;
			CBoneAnimationObject* animPtr = (CBoneAnimationObject*)bonePtr->m_animationList->GetAt(posAnim);

			
            long scndryTimePassed = 0;
		
		CBoneAnimationObject* secondaryAnim = NULL;
		if(m_secondaryAnimationInUse != -1)
		{
		  POSITION posAnim2 = bonePtr->m_animationList->FindIndex(m_secondaryAnimationInUse);
		  if(!posAnim)
			return FALSE;
			secondaryAnim = (CBoneAnimationObject*)bonePtr->m_animationList->GetAt(posAnim2);
		}	
			RemoveSecondaryAdjust(m_secondaryCOMInheritBone);
			D3DVECTOR bonePosition;
			bonePtr->m_boneFrame->GetPosition(NULL,&bonePosition);
            m_secondaryCOMadjust = animPtr->GetBonePositionalDifference(bonePosition);
            if(!secondaryAnim)
				ZeroMemory(&m_secondaryCOMadjust,sizeof(D3DVECTOR));

			BOOL updatePrimary    = FALSE;
			BOOL updateSecondary  = FALSE;
			int  lastFrame     = m_currentBaseFrame;


			
			//interpolated variables
			UpdateAnimMS(animPtr,currentTime,FALSE);

            if(secondaryAnim){
			  int lastFrameSecondary = m_secondaryBaseFrame;
			  UpdateAnimMS(secondaryAnim,currentTime,TRUE);
			  if(m_secondaryBaseFrame != lastFrameSecondary)
                 updateSecondary = TRUE;
			}

              if(m_currentBaseFrame != lastFrame)
                 updatePrimary = TRUE;
			
			if(!disableSoundOption)
			{
			  if(secondaryAnim)
			     HandleSoundRangesForAnimation(secondaryAnim,localSpacePos,globalSoundFactor,m_soundManager,TRUE);//sound range callback
			
			     HandleSoundRangesForAnimation(animPtr,localSpacePos,globalSoundFactor,m_soundManager,FALSE);//sound range callback
			}
			
			if(soundOnly)
			   return TRUE;

			//updatepositions
			 int locAnimInUse          = 0;
			 int locanimationFrame     = 0;
			 long millisecondsPerFrame = 0;
			 long secondaryOvertime    = 0;
			 long primaryOvertime      = 0;
			 long currentOvertime      = 0;
			 BOOL updateBone           = FALSE;
			 BOOL secondaryInUse       = FALSE;
			 			 
			 RemoveFreeLook();//unload free look
			 RemoveWaisteControl();//unload waise control

			 for(POSITION bPos = m_bonesList->GetHeadPosition();bPos != NULL;){//bone loop
                 CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(bPos);
				 locAnimInUse = m_animationInUse;
			     locanimationFrame = m_currentBaseFrame;
				 millisecondsPerFrame = animPtr->m_millisecondsPerFrame;
				 updateBone           = updatePrimary;
				 secondaryInUse       = FALSE;
				 if(bonePtr->m_secondaryOverridable)
				 {//secondaryAnimation
                    if(secondaryAnim)
					{
                       locAnimInUse = m_secondaryAnimationInUse;
			           locanimationFrame = m_secondaryBaseFrame;
                       millisecondsPerFrame = secondaryAnim->m_millisecondsPerFrame;
					   updateBone = updateSecondary;
					   secondaryInUse = TRUE;
					}
				 }//secondaryAnimation
                 MoveBoneMangement(currentTime,
								   millisecondsPerFrame,
								   bonePtr,
				                   locAnimInUse,
			                       locanimationFrame,
								   updateBone,
								   currentOvertime
								  );

				 if(updateBone)
				 {
                  if(secondaryInUse)
                    secondaryOvertime = currentOvertime;
				  else
				    primaryOvertime = currentOvertime;
				 }
			 }//end bone loop

			 m_primaryAnimLastUpdatedTime -= primaryOvertime;
			 if(secondaryAnim)
                m_secondaryAnimLastUpdatedTime -= secondaryOvertime;

             ApplySecondaryAdjust(m_secondaryCOMInheritBone);
			 ApplyFreeLook();//Implement Secondary free look
			 ApplyWaisteControl();//waiste control

             if(!disableDeformbleCalc)//optimization for AISERVER
             UpdateBasedDeformationByVertexAssignments(m_currentLOD,g_pd3dDevice);//VERTEX assignments
	m_requireUpdate = TRUE;
	return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------load animation--------------------------------//
 void CSkeletonObject::UpdateBasedDeformationByVertexAssignments(int & dmIndex,LPDIRECT3DDEVICE9 g_pd3dDevice)
{      
	   int tracer = 0;
       for(POSITION bPos = m_bonesList->GetHeadPosition();bPos != NULL;){//bone loop
           CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(bPos);	

           D3DMATRIX boneMatrix;
		   bonePtr->m_boneFrame->GetTransform(NULL,&boneMatrix);

           float posX,posY,posZ;

           for(int i=0;i<m_deformableMeshArrayCount;i++)
		   {


               if(m_deformableMeshArrayV2[i].m_defMeshVisualUnit){//p
                  CDeformableVisObj * deformPtr = (CDeformableVisObj*)m_deformableMeshArrayV2[i].m_defMeshVisualUnit->m_lodChain->GetByIndex(dmIndex);
                  if(deformPtr){//1
				     for(int x=0;x<deformPtr->m_assignmentCount;x++)
					 {//as
                         if(deformPtr->m_vertexAssignments[x].boneIndex == tracer)
						 {//found bone
							
							//position
                            posX = (boneMatrix.m[0][0]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].x)+(boneMatrix.m[1][0]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].y)+(boneMatrix.m[2][0]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].z)+(boneMatrix.m[3][0]);
                            posY = (boneMatrix.m[0][1]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].x)+(boneMatrix.m[1][1]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].y)+(boneMatrix.m[2][1]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].z)+(boneMatrix.m[3][1]);
                            posZ = (boneMatrix.m[0][2]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].x)+(boneMatrix.m[1][2]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].y)+(boneMatrix.m[2][2]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].z)+(boneMatrix.m[3][2]);
					        
                            int tempInt = deformPtr->m_vertexAssignments[x].vertexIndex;
 							deformPtr->m_mesh->m_abVertexArray.SetPositionXYZ(tempInt,
								                                              posX,
							                                                  posY,
																			  posZ);

							 for(int ownedLoop=0;ownedLoop<deformPtr->m_vertexAssignments[x].carriedCount;ownedLoop++)
							 {
							   //deformPtr->m_mesh->m_abVertexArray.m_origVertexArray[deformPtr->m_vertexAssignments[x].vertexesCarried[ownedLoop]].x = deformPtr->m_mesh->m_abVertexArray.m_origVertexArray[deformPtr->m_vertexAssignments[x].vertexIndex].x;
							   //deformPtr->m_mesh->m_abVertexArray.m_origVertexArray[deformPtr->m_vertexAssignments[x].vertexesCarried[ownedLoop]].y = deformPtr->m_mesh->m_abVertexArray.m_origVertexArray[deformPtr->m_vertexAssignments[x].vertexIndex].y;
							   //deformPtr->m_mesh->m_abVertexArray.m_origVertexArray[deformPtr->m_vertexAssignments[x].vertexesCarried[ownedLoop]].z = deformPtr->m_mesh->m_abVertexArray.m_origVertexArray[deformPtr->m_vertexAssignments[x].vertexIndex].z;
							   tempInt = deformPtr->m_vertexAssignments[x].vertexesCarried[ownedLoop];
							   deformPtr->m_mesh->m_abVertexArray.SetPositionXYZ(tempInt,
								                                                       posX,
							                                                           posY,
																			           posZ);
							 }
						 }//end found bone
					 }//end as
				  }//end 1
			      //deformPtr->m_mesh->InitBuffer(g_pd3dDevice);
			   }//end p
		   } 
		   tracer++;
	   }//end bone loop

       for(int r=0;r<m_deformableMeshArrayCount;r++)
	   {//reinit
               if(m_deformableMeshArrayV2[r].m_defMeshVisualUnit){//p
                  CDeformableVisObj * deformPtr = (CDeformableVisObj*)m_deformableMeshArrayV2[r].m_defMeshVisualUnit->m_lodChain->GetByIndex(dmIndex);
			      if(deformPtr)
					 deformPtr->m_mesh->m_abVertexArray.ReinitializeDxVbuffersPositionOnly(g_pd3dDevice);
				  //deformPtr->m_mesh->InitBuffer(g_pd3dDevice);
			   }
			   
	   }//end reinit
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CSkeletonObject::InvertNormals()
{
	    int tracer = 0;
		D3DMATRIX boneMatrix;
		float posX,posY,posZ,normX,normY,normZ;

        for(POSITION bPos = m_bonesList->GetHeadPosition();bPos != NULL;)
		{//bone loop
           CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(bPos);	

           
		   bonePtr->m_boneFrame->GetInverseTransform(&boneMatrix);

           for(int i=0;i<m_deformableMeshArrayCount;i++)
		   {

               if(m_deformableMeshArrayV2[i].m_defMeshVisualUnit){//p
                  CDeformableVisObj * deformPtr = (CDeformableVisObj*)m_deformableMeshArrayV2[i].m_defMeshVisualUnit->m_lodChain->GetByIndex(0);
                  if(deformPtr){//1
				     for(int x=0;x<deformPtr->m_assignmentCount;x++)
					 {//as
                         if(deformPtr->m_vertexAssignments[x].boneIndex == tracer)
						 {//found bone
							
							//position
                            posX = (boneMatrix.m[0][0]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].x)+(boneMatrix.m[1][0]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].y)+(boneMatrix.m[2][0]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].z)+(boneMatrix.m[3][0]);
                            posY = (boneMatrix.m[0][1]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].x)+(boneMatrix.m[1][1]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].y)+(boneMatrix.m[2][1]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].z)+(boneMatrix.m[3][1]);
                            posZ = (boneMatrix.m[0][2]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].x)+(boneMatrix.m[1][2]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].y)+(boneMatrix.m[2][2]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].z)+(boneMatrix.m[3][2]);
					        //normal
                            normX = (boneMatrix.m[0][0]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].x)+(boneMatrix.m[1][0]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].y)+(boneMatrix.m[2][0]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].z);
                            normY = (boneMatrix.m[0][1]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].x)+(boneMatrix.m[1][1]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].y)+(boneMatrix.m[2][1]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].z);
                            normZ = (boneMatrix.m[0][2]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].x)+(boneMatrix.m[1][2]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].y)+(boneMatrix.m[2][2]*deformPtr->m_originalArrayVector[deformPtr->m_vertexAssignments[x].vertexIndex].z);


                            int tempInt = deformPtr->m_vertexAssignments[x].vertexIndex;
 							

							 for(int ownedLoop=0;ownedLoop<deformPtr->m_vertexAssignments[x].carriedCount;ownedLoop++)
							 {
							   //deformPtr->m_mesh->m_abVertexArray.m_origVertexArray[deformPtr->m_vertexAssignments[x].vertexesCarried[ownedLoop]].x = deformPtr->m_mesh->m_abVertexArray.m_origVertexArray[deformPtr->m_vertexAssignments[x].vertexIndex].x;
							   //deformPtr->m_mesh->m_abVertexArray.m_origVertexArray[deformPtr->m_vertexAssignments[x].vertexesCarried[ownedLoop]].y = deformPtr->m_mesh->m_abVertexArray.m_origVertexArray[deformPtr->m_vertexAssignments[x].vertexIndex].y;
							   //deformPtr->m_mesh->m_abVertexArray.m_origVertexArray[deformPtr->m_vertexAssignments[x].vertexesCarried[ownedLoop]].z = deformPtr->m_mesh->m_abVertexArray.m_origVertexArray[deformPtr->m_vertexAssignments[x].vertexIndex].z;
							   tempInt = deformPtr->m_vertexAssignments[x].vertexesCarried[ownedLoop];
							   
							   deformPtr->m_mesh->m_abVertexArray.SetPositionXYZNXNYNZ(tempInt,
								                                                       posX,
							                                                           posY,
																			           posZ,
																					   normX,
																					   normY,
																					   normZ);
							 }
						 }//end found bone
					 }//end as
				  }//end 1
			   }//end p
		   } 
		   tracer++;
		}//end bone loop
   return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------load animation--------------------------------//
BOOL CSkeletonObject::DeleteAnimationByIndex(int index)
{
    for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
        CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
		  if(bonePtr->m_animationList){
			   POSITION animLast;
		       int indexTrace = 0;
			   for(POSITION animPos = bonePtr->m_animationList->GetHeadPosition();(animLast = animPos) != NULL;)
			   {//
				   CBoneAnimationObject * animationObject = (CBoneAnimationObject*)bonePtr->m_animationList->GetNext(animPos);
				   if(indexTrace == index){//found type
		              CBoneAnimationObject * animationDel = (CBoneAnimationObject*)bonePtr->m_animationList->GetAt(animLast);
				      animationDel->SafeDelete();
					  delete animationDel;
					  animationDel=0;
				      bonePtr->m_animationList->RemoveAt(animLast);
				   }//end found type
				   indexTrace++;
			   }
		  }
	}	
return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------load animation--------------------------------//
BOOL CSkeletonObject::DeleteAnimationByType(int type)
{
    for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
        CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
		  if(bonePtr->m_animationList){
			   POSITION animLast;
		       for(POSITION animPos = bonePtr->m_animationList->GetHeadPosition();(animLast = animPos) != NULL;)
			   {//
				   CBoneAnimationObject * animationObject = (CBoneAnimationObject*)bonePtr->m_animationList->GetNext(animPos);
				   if(animationObject->m_type == type){//found type
		              CBoneAnimationObject * animationDel = (CBoneAnimationObject*)bonePtr->m_animationList->GetAt(animLast);
				      animationDel->SafeDelete();
					  delete animationDel;
					  animationDel=0;
				      bonePtr->m_animationList->RemoveAt(animLast);
				   }//end found type
			   }
		  }
	}	
return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------load animation--------------------------------//
BOOL CSkeletonObject::LoadAnimation(CString fileName,
									BOOL looping,
									int type,
									int duration,
									int replaceIndex)
{
    CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		return FALSE;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);
	 //this->
	 CSkeletonObject * ghostSkeletalPtr = new CSkeletonObject();
     int physiqueModCount = 0;
	 int firstFrame = 0;
	 int lastFrame = 0;
	 int frameCount = 0;
	 
	 the_in_Archive >> ghostSkeletalPtr->m_SkeletalName
		            >> ghostSkeletalPtr->m_frameSpeed
		            >> ghostSkeletalPtr->m_ticksPerFrame
		            >> ghostSkeletalPtr->m_bonesList
	          	    >> ghostSkeletalPtr->m_deformableMeshArrayCount
		            >> physiqueModCount
	                >> firstFrame
                    >> lastFrame
	                >> frameCount;

       if(ghostSkeletalPtr->m_deformableMeshArrayCount > 0){
	      ghostSkeletalPtr->m_deformableMeshArrayV2 = new DeformableMeshStructV2[m_deformableMeshArrayCount];
	      for(int loop=0;loop<ghostSkeletalPtr->m_deformableMeshArrayCount;loop++){
              the_in_Archive >> ghostSkeletalPtr->m_deformableMeshArrayV2[loop].m_defMeshVisualUnit;
		  }
	   }
	 
	 the_in_Archive.Close();
     fl.Close();
     //apply animation data
	 for(POSITION posLoc = ghostSkeletalPtr->m_bonesList->GetHeadPosition();posLoc != NULL;){
        CBoneObject * bonePtr = (CBoneObject *) ghostSkeletalPtr->m_bonesList->GetNext(posLoc);
		CBoneObject * boneOnSkeletal = NULL;
		  if(GetBoneByName(bonePtr->m_boneName,&boneOnSkeletal)==TRUE){//found bone
			 POSITION animPos = bonePtr->m_animationList->FindIndex(0);
			 if(animPos){//1
		        CBoneAnimationObject * animationObject = (CBoneAnimationObject *)bonePtr->m_animationList->GetAt(animPos);
                CBoneAnimationObject * animObjectClone = NULL; 
				animationObject->Clone(&animObjectClone);
				animObjectClone->m_looping                 = looping;
		        animObjectClone->m_minPlayDuration         = duration;
				animObjectClone->m_millisecondsPerFrame    = ghostSkeletalPtr->m_ticksPerFrame;
	            animObjectClone->m_totalFrames             = lastFrame;
				animObjectClone->m_type                    = type;
				if(!boneOnSkeletal->m_animationList)
					boneOnSkeletal->m_animationList = new CBoneAnimationList;
				    
				
				
				    if(replaceIndex < 0)//add tail
				       boneOnSkeletal->m_animationList->AddTail(animObjectClone);
					else{
                       POSITION animToReplacePos = boneOnSkeletal->m_animationList->FindIndex(replaceIndex);
                       if(animToReplacePos)
					   {
                          CBoneAnimationObject * animationObjectExist = (CBoneAnimationObject *)boneOnSkeletal->m_animationList->GetAt(animToReplacePos);
						  animationObjectExist->SafeDelete();
						  delete animationObjectExist;
						  animationObjectExist = 0;
					    
						  boneOnSkeletal->m_animationList->InsertAfter(animToReplacePos,animObjectClone);
						  boneOnSkeletal->m_animationList->RemoveAt(animToReplacePos);
					   
					   }
					}
			 }//end 1
		  }//end found bone
	 }
	 /*ghostSkeletalPtr->SafeDelete();
	 delete ghostSkeletalPtr;
	 ghostSkeletalPtr=0;*/
	
return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------File extractions------------------------------//
BOOL CSkeletonObject::AddBone(CString       boneName,
							  CString       parentName,
							  D3DVECTOR     dirVect,
							  D3DVECTOR     upVect,
							  D3DVECTOR     position,
							  D3DVECTOR     scale,
							  CBoneObject ** boneNodeReturn)
{
     CBoneObject * bone = NULL;
     bone = new CBoneObject();
     bone->m_boneFrame = new CFrameObj(NULL);
     bone->m_boneName = boneName;
	 bone->m_parentName = parentName;
	 
	 bone->m_boneFrame->SetOrientation(dirVect,upVect);
     bone->m_boneFrame->SetPosition(NULL,position);
	 bone->m_boneFrame->GetTransform(NULL,&bone->m_startPosition);

	 m_bonesList->AddTail(bone);
	 
	 *boneNodeReturn = bone;
	 return TRUE;
  
return FALSE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Hot swap--------------------------------------//
BOOL CSkeletonObject::HowSwapDim(int deformableIndex,
								 int newCfg,
								 CSkeletonObject * targetRuntimeEntity,
								 LPDIRECT3DDEVICE9 g_pd3dDevice)
{
   if(m_deformableMeshArrayCount <= 0)
	  return FALSE;
      
		 
			  POSITION alPos = m_alternateConfigurations->FindIndex(deformableIndex);
			  if(!alPos)
				  return FALSE;
			      CAlterUnitDBObject * altPtr = (CAlterUnitDBObject *) m_alternateConfigurations->GetAt(alPos);
				  
				  
				  POSITION curCfgPos = altPtr->m_configurations->FindIndex(newCfg);
			      if(!curCfgPos)
					  return FALSE;
				      targetRuntimeEntity->m_spawnCfg[deformableIndex] = newCfg;
			          CDeformableMesh * dMesh = (CDeformableMesh *)altPtr->m_configurations->GetAt(curCfgPos);
                      if(targetRuntimeEntity->m_deformableMeshArrayV2[deformableIndex].m_defMeshVisualUnit)
					  {
						 targetRuntimeEntity->m_deformableMeshArrayV2[deformableIndex].m_defMeshVisualUnit->SafeDelete();
                         delete targetRuntimeEntity->m_deformableMeshArrayV2[deformableIndex].m_defMeshVisualUnit;
						 targetRuntimeEntity->m_deformableMeshArrayV2[deformableIndex].m_defMeshVisualUnit = 0;
					  }
			          dMesh->Clone(&targetRuntimeEntity->m_deformableMeshArrayV2[deformableIndex].m_defMeshVisualUnit,g_pd3dDevice);
      return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------File extractions------------------------------//
BOOL CSkeletonObject::GetBoneByName(CString       boneName,
							        CBoneObject ** boneObject)
{
	for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
        CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
		if(bonePtr->m_boneName.CompareNoCase(boneName)==0)
		{
           *boneObject = bonePtr;
		   return TRUE;
		}
	}
return FALSE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------File extractions------------------------------//
BOOL CSkeletonObject::QuerryBox()
{
    int indexTrace = 0;
	if(m_bonesList)
    for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
        CBoneObject * bonePtr = (CBoneObject *) m_bonesList->GetNext(posLoc);
		   D3DVECTOR vector;
		   bonePtr->m_boneFrame->GetPosition(NULL,&vector);
		   if(indexTrace == 0){ //start
              m_box.maxX = vector.x;
			  m_box.maxY = vector.y;
			  m_box.maxZ = vector.z;
              m_box.minX = vector.x;
			  m_box.minY = vector.y;
			  m_box.minZ = vector.z;
		   }//end start
           else{
              if(m_box.maxX < vector.x)
				 m_box.maxX = vector.x;
              if(m_box.maxY < vector.y)
				 m_box.maxY = vector.y;
			  if(m_box.maxZ < vector.z)
				 m_box.maxZ = vector.z;
			  
              if(m_box.minX > vector.x)
				 m_box.minX = vector.x;
              if(m_box.minY > vector.y)
				 m_box.minY = vector.y;
			  if(m_box.minZ > vector.z)
				 m_box.minZ = vector.z;
		   }
		indexTrace++;
	}




return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------File extractions------------------------------//
BOOL CSkeletonObject::BuildHiarchy()
{
     for(POSITION posLoc = m_bonesList->GetHeadPosition();posLoc != NULL;){
        CBoneObject * frmPtr = (CBoneObject *)m_bonesList->GetNext(posLoc);
        for(POSITION posLoc2 = m_bonesList->GetHeadPosition();posLoc2 != NULL;){
        CBoneObject * bonesPtr = (CBoneObject *)m_bonesList->GetNext(posLoc2);
          if(frmPtr->m_parentName == bonesPtr->m_boneName){
	         frmPtr->m_boneFrame->SetParent(bonesPtr->m_boneFrame);
			 //AfxMessageBox(operator+(frmPtr->m_boneName,operator+("  parent::",bonesPtr->m_boneName)));
		  }
		}
	 }
return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------secondary animation---------------------------//
void CSkeletonObject::ApplyFreeLookRotation(float pitch,float yaw)
{
    if(m_freeLookBone == -1)
	   return;
    if(!m_bonesList)
       return;

    m_freeLookPitch = pitch;
	m_freeLookYaw = yaw;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
CAlterUnitDBObject::CAlterUnitDBObject()
{
   m_alterUnitName = "";
   m_configurations = NULL;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CAlterUnitDBObject::SafeDelete()
{
   if(m_configurations)
   {
	  m_configurations->SafeDelete();
      delete m_configurations;
      m_configurations = 0;
   }
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CAlterUnitDBObject::Serialize( CArchive& ar )
{
 CObject::Serialize( ar );
	if(ar.IsStoring() ){	
       ar.SetObjectSchema(2);
       ASSERT(ar.GetObjectSchema() == 2);

	   ar << m_configurations;

	   ar << m_alterUnitName;
	}
	else
	{
       int version = ar.GetObjectSchema();

       ar >> m_configurations;

	   if(version > 1)
	   {
          ar >> m_alterUnitName;
	   }
	}
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CAlterUnitDBObjectList::SafeDelete()
{
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {
        CAlterUnitDBObject * dPtr = (CAlterUnitDBObject *)GetNext(posLoc);
		dPtr->SafeDelete();
		delete dPtr;
		dPtr=0;
   }
   RemoveAll();
}
////////Macros
IMPLEMENT_SERIAL(CAlterUnitDBObjectList, CObject, 0)
IMPLEMENT_SERIAL(CAlterUnitDBObject, CObject, VERSIONABLE_SCHEMA|2)
IMPLEMENT_SERIAL(CSkeletonObject, CObject, 0)
//-------------------



