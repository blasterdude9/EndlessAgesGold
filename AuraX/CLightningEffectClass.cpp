#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include <math.h>

#include "CLightningEffectClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CLightningObj::CLightningObj()
{
   m_lightningMesh   = NULL;
   m_wldPos.x        = 0.0f;
   m_wldPos.y        = 0.0f;
   m_wldPos.z        = 0.0f;
   m_width           = 0.0f;
   m_depth           = 0.0f;
   m_duration        = 1000;
   m_timeStamp       = 0;
   m_enviormentMode  = TRUE;
}

void CLightningObj::SafeDelete()
{
	if(m_lightningMesh){
       m_lightningMesh->SafeDelete();
	   delete m_lightningMesh;
       m_lightningMesh = 0;
	}
}

void CLightningObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CLightningObj * spnPtr = (CLightningObj *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

 void CLightningObjList::AddLightningObject(D3DVECTOR & spawnPosition,
										          float     width,
										          float     depth,
										          int       textureIndex,
										          long      duration,
										          BOOL      enviormentModeOn
										   )
{
	   CLightningObj * newPtr = new CLightningObj();
	   newPtr->m_enviormentMode  = enviormentModeOn;
	   newPtr->m_duration        = duration;
       newPtr->m_timeStamp       = GetTickCount();
	   newPtr->m_wldPos.x        = spawnPosition.x;
       newPtr->m_wldPos.y        = spawnPosition.y;
       newPtr->m_wldPos.z        = spawnPosition.z;
       newPtr->m_width           = width;
       newPtr->m_depth           = depth;
	   newPtr->m_lightningMesh                       = new CEXMeshObj();
	   //newPtr->m_lightningMesh->m_abVertexArray      = new CAdvancedVertexObj();
	   newPtr->m_lightningMesh->List_materialObject  = new CMaterialObject();
       newPtr->m_lightningMesh->m_abVertexArray.m_indecieCount    = 6;
       newPtr->m_lightningMesh->m_abVertexArray.m_vertexCount     = 4; 
       ABVERTEX * vArray                             = new ABVERTEX[4];
       newPtr->m_lightningMesh->m_abVertexArray.m_indecieArray    = new WORD[6];
	   newPtr->m_lightningMesh->m_abVertexArray.m_uvCount         = 1;
	   //m_abVertexArray.m_indecieArray

       ZeroMemory(vArray,(sizeof(ABVERTEX)*4));
       ZeroMemory(newPtr->m_lightningMesh->m_abVertexArray.m_indecieArray,(sizeof(WORD)*6));
       //set texture/materials
	   newPtr->m_lightningMesh->List_materialObject->m_fogFilter = TRUE;
       newPtr->m_lightningMesh->List_materialObject->m_baseBlendMode = 2;
	   newPtr->m_lightningMesh->List_materialObject->m_baseMaterial.Emissive.r = 1.0f;
	   newPtr->m_lightningMesh->List_materialObject->m_baseMaterial.Emissive.g = 1.0f;
	   newPtr->m_lightningMesh->List_materialObject->m_baseMaterial.Emissive.b = 1.0f;
	   //newPtr->m_lightningMesh->List_materialObject->m_baseMaterial.Emissive.a = 1.0f;
	   newPtr->m_lightningMesh->List_textureSelect[0] = textureIndex;
	   newPtr->m_lightningMesh->List_materialObject->Reset();

	   ///newPtr->m_lightningMesh->List_materialObject->SetFadeOut(duration);
	   newPtr->m_lightningMesh->List_materialObject->m_timeStamp = GetTickCount();
	   newPtr->m_lightningMesh->List_materialObject->m_cycleTime = duration;
	   newPtr->m_lightningMesh->GetUVlayerCount();

	   newPtr->m_lightningMesh->m_abVertexArray.SetABVERTEXStyle(NULL,vArray,newPtr->m_lightningMesh->m_abVertexArray.m_vertexCount);
	   

       AddTail(newPtr);
}

 void CLightningObjList::RenderSystem(D3DVECTOR           & cameraPosition,
					                        LPDIRECT3DDEVICE9     g_pd3dDevice7,
					                        D3DVECTOR           & vOffset,
									        CStateManagementObj * stateSystem,
									        CTextureDBObjList   * textureDatabase)
{
  //set material
  stateSystem->SetBlendType(g_pd3dDevice7,2);
  POSITION posLast = NULL;
  for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
      CLightningObj * spnPtr = (CLightningObj *)GetNext(posLoc);	  
	  int passedTime = GetTickCount() - spnPtr->m_lightningMesh->List_materialObject->m_timeStamp;
      

      //internal fade control
	  if(passedTime > spnPtr->m_lightningMesh->List_materialObject->m_cycleTime)
	     passedTime = spnPtr->m_lightningMesh->List_materialObject->m_cycleTime;
      
	     spnPtr->m_lightningMesh->List_materialObject->m_useMaterial.Emissive.r = spnPtr->m_lightningMesh->List_materialObject->InterpolateFloats(spnPtr->m_lightningMesh->List_materialObject->m_baseMaterial.Emissive.r,0.0f,spnPtr->m_lightningMesh->List_materialObject->m_cycleTime,passedTime);
	     spnPtr->m_lightningMesh->List_materialObject->m_useMaterial.Emissive.g = spnPtr->m_lightningMesh->List_materialObject->InterpolateFloats(spnPtr->m_lightningMesh->List_materialObject->m_baseMaterial.Emissive.g,0.0f,spnPtr->m_lightningMesh->List_materialObject->m_cycleTime,passedTime);
	     spnPtr->m_lightningMesh->List_materialObject->m_useMaterial.Emissive.b = spnPtr->m_lightningMesh->List_materialObject->InterpolateFloats(spnPtr->m_lightningMesh->List_materialObject->m_baseMaterial.Emissive.b,0.0f,spnPtr->m_lightningMesh->List_materialObject->m_cycleTime,passedTime);
	     spnPtr->m_lightningMesh->List_materialObject->m_useMaterial.Diffuse.r =  spnPtr->m_lightningMesh->List_materialObject->InterpolateFloats(spnPtr->m_lightningMesh->List_materialObject->m_baseMaterial.Diffuse.r,0.0f,spnPtr->m_lightningMesh->List_materialObject->m_cycleTime,passedTime);
	     spnPtr->m_lightningMesh->List_materialObject->m_useMaterial.Diffuse.g =  spnPtr->m_lightningMesh->List_materialObject->InterpolateFloats(spnPtr->m_lightningMesh->List_materialObject->m_baseMaterial.Diffuse.g,0.0f,spnPtr->m_lightningMesh->List_materialObject->m_cycleTime,passedTime);
	     spnPtr->m_lightningMesh->List_materialObject->m_useMaterial.Diffuse.b =  spnPtr->m_lightningMesh->List_materialObject->InterpolateFloats(spnPtr->m_lightningMesh->List_materialObject->m_baseMaterial.Diffuse.b,0.0f,spnPtr->m_lightningMesh->List_materialObject->m_cycleTime,passedTime);
	     spnPtr->m_lightningMesh->List_materialObject->m_useMaterial.Ambient.r =  spnPtr->m_lightningMesh->List_materialObject->InterpolateFloats(spnPtr->m_lightningMesh->List_materialObject->m_baseMaterial.Ambient.r,0.0f,spnPtr->m_lightningMesh->List_materialObject->m_cycleTime,passedTime);
	     spnPtr->m_lightningMesh->List_materialObject->m_useMaterial.Ambient.g =  spnPtr->m_lightningMesh->List_materialObject->InterpolateFloats(spnPtr->m_lightningMesh->List_materialObject->m_baseMaterial.Ambient.g,0.0f,spnPtr->m_lightningMesh->List_materialObject->m_cycleTime,passedTime);
	     spnPtr->m_lightningMesh->List_materialObject->m_useMaterial.Ambient.b =  spnPtr->m_lightningMesh->List_materialObject->InterpolateFloats(spnPtr->m_lightningMesh->List_materialObject->m_baseMaterial.Ambient.b,0.0f,spnPtr->m_lightningMesh->List_materialObject->m_cycleTime,passedTime);    
	  //end fade control
	  
	  
	  g_pd3dDevice7->SetMaterial(&spnPtr->m_lightningMesh->List_materialObject->m_useMaterial);
      stateSystem->SetTextureState(spnPtr->m_lightningMesh->List_textureSelect[0],
		                           stateSystem->m_intZero,
								   g_pd3dDevice7,
								   textureDatabase);

	  stateSystem->SetFogFilter(g_pd3dDevice7,spnPtr->m_lightningMesh->List_materialObject->m_fogFilter);
	  stateSystem->SetCullState(spnPtr->m_lightningMesh->List_materialObject->m_cullOn,g_pd3dDevice7);

	  //set textures and multitexture states
	  for(int loop=1;loop<stateSystem->m_multiTextureSupported;loop++){
        stateSystem->SetTextureState(spnPtr->m_lightningMesh->List_textureSelect[loop],loop,g_pd3dDevice7,textureDatabase);	
		stateSystem->SetBlendLevel(loop,spnPtr->m_lightningMesh->List_BlendMethod[(loop-1)],g_pd3dDevice7);
	  }

       
         if(!spnPtr->m_lightningMesh)
		     continue;
             //build orientation vector
             D3DVECTOR orientationVector;
             orientationVector.x = spnPtr->m_wldPos.x - cameraPosition.x;
	         orientationVector.y = 0.0f;
             orientationVector.z = spnPtr->m_wldPos.z - cameraPosition.z;
             //set up base panel
             //vertexes
             spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[0].x = -spnPtr->m_width;
	         spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[0].y =  spnPtr->m_wldPos.y;

	         spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[1].x =  spnPtr->m_width;
	         spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[1].y =  spnPtr->m_wldPos.y;

	         spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[2].x =  spnPtr->m_width;
	         spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[2].y =  spnPtr->m_wldPos.y - spnPtr->m_depth;

	         spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[3].x = -spnPtr->m_width;
	         spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[3].y =  spnPtr->m_wldPos.y - spnPtr->m_depth;

		     BOOL behind = FALSE;
		     if(spnPtr->m_wldPos.z > cameraPosition.z)
			 {
		       //normals
		       spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[0].nx =   0.0f;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[0].ny =   0.0f;
		       spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[0].nz =   1.0f;
               spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[1].nx =   0.0f;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[1].ny =   0.0f;
		       spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[1].nz =   1.0f;
		       spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[2].nx =   0.0f;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[2].ny =   0.0f;
		       spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[2].nz =   1.0f;
		       spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[3].nx =   0.0f;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[3].ny =   0.0f;
	   	       spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[3].nz =   1.0f;
		       //indecies
		       spnPtr->m_lightningMesh->m_abVertexArray.m_indecieArray[0] = 0;
		       spnPtr->m_lightningMesh->m_abVertexArray.m_indecieArray[1] = 1;
		       spnPtr->m_lightningMesh->m_abVertexArray.m_indecieArray[2] = 2;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_indecieArray[3] = 0;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_indecieArray[4] = 2;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_indecieArray[5] = 3;
		       //UV
		       spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[0].tx0.tu =   0.0f;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[0].tx0.tv =   0.0f;
               spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[1].tx0.tu =   1.0f;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[1].tx0.tv =   0.0f;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[2].tx0.tu =   1.0f;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[2].tx0.tv =   1.0f;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[3].tx0.tu =   0.0f;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[3].tx0.tv =   1.0f;
			 }
		     else
			 {
               //normals
		       spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[0].nx =   0.0f;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[0].ny =   0.0f;
		       spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[0].nz =   -1.0f;
               spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[1].nx =   0.0f;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[1].ny =   0.0f;
		       spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[1].nz =   -1.0f;
		       spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[2].nx =   0.0f;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[2].ny =   0.0f;
		       spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[2].nz =   -1.0f;
		       spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[3].nx =   0.0f;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[3].ny =   0.0f;
	   	       spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[3].nz =   -1.0f;
		       //indecies
		       spnPtr->m_lightningMesh->m_abVertexArray.m_indecieArray[0] = 2;
		       spnPtr->m_lightningMesh->m_abVertexArray.m_indecieArray[1] = 1;
		       spnPtr->m_lightningMesh->m_abVertexArray.m_indecieArray[2] = 0;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_indecieArray[3] = 3;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_indecieArray[4] = 2;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_indecieArray[5] = 0;
		       //UV
		       spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[0].tx0.tu =   1.0f;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[0].tx0.tv =   0.0f;
               spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[1].tx0.tu =   0.0f;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[1].tx0.tv =   0.0f;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[2].tx0.tu =   0.0f;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[2].tx0.tv =   1.0f;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[3].tx0.tu =   1.0f;
	           spnPtr->m_lightningMesh->m_abVertexArray.m_vertexArray1[3].tx0.tv =   1.0f;
		       //
		       behind = TRUE;
			 }
	         //orientate panel to face primary viewport camera
             D3DMATRIX appliedRotation;
             float rotationOnY = atan(fabs(orientationVector.x)/fabs(orientationVector.z))* 57.3f;
             if(spnPtr->m_wldPos.x < cameraPosition.x)
		        rotationOnY = -rotationOnY;

             if(behind)
			    rotationOnY = -rotationOnY;

		        GL_matrixFunctions.Rotate (0.0f,rotationOnY,0.0f,appliedRotation.m);
                appliedRotation._41 = (spnPtr->m_wldPos.x - vOffset.x)+cameraPosition.x;
		        appliedRotation._42 = (0.0f               - vOffset.y)+cameraPosition.y;
		        appliedRotation._43 = (spnPtr->m_wldPos.z - vOffset.z)+cameraPosition.z;
                //rotate panel
		        //g_pd3dDevice7->SetTransform( D3DTS_WORLD , &appliedRotation ); 
				stateSystem->SetWorldMatrix( (D3DXMATRIX*)&appliedRotation,g_pd3dDevice7);
		        //render
				spnPtr->m_lightningMesh->InitBuffer(g_pd3dDevice7,NULL);
				stateSystem->SetDiffuseSource(spnPtr->m_lightningMesh->m_abVertexArray.m_advancedFixedMode,g_pd3dDevice7);
				stateSystem->SetVertexType(spnPtr->m_lightningMesh->m_abVertexArray.m_uvCount,g_pd3dDevice7);
				spnPtr->m_lightningMesh->Render(g_pd3dDevice7,stateSystem->m_currentVertexType,spnPtr->m_lightningMesh->m_chromeUvSet);

				if(passedTime >= spnPtr->m_duration)
				{//remove
				   spnPtr->SafeDelete();
				   delete spnPtr;
				   spnPtr=0;
                   RemoveAt(posLast);
				}//end remove
     }
}


