#include "stdafx.h"
#define D3D_OVERLOADS
#include "resource.h"
#include <d3d9.h>
//#include <afxtempl.h>

#include "direct.h"
#include "CTreeImporter.h"
#include "TreeImportOptionsDlg.h"
//#include "afxdlgs.h"

CTreeImporterObj::CTreeImporterObj()
{
     
}

//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
CEXMeshObj * CTreeImporterObj::GetEXMeshObjFromSpeedTree(float lodVal,CSpeedTreeRT* m_pSpeedTree,CSpeedTreeRT::SGeometry* m_pGeometryCache)
{
	       GL_matrixFunctions.InitMat(MatrixInit2.m);
	       CEXMeshObj * tempMesh = NULL;

    

           m_pSpeedTree->SetLodLevel(lodVal);//0.0f - 1.0f

		   //if (!m_pSpeedTree->Compute(NULL, 1))
			//   return NULL;
       

		   //CSpeedTreeRT::SGeometry* m_pGeometryCache = NULL;	
           if (m_pGeometryCache == NULL)
		       m_pGeometryCache = new CSpeedTreeRT::SGeometry;
		   m_pSpeedTree->GetGeometry(*m_pGeometryCache);
		   

		  

		   //Port primitive
		   CSpeedTreeRT::SGeometry::SIndexed* pBranches = &(m_pGeometryCache->m_sBranches);
           if(!pBranches->m_pStripLengths)
			   return NULL;

	       int m_unBranchVertexCount = pBranches->m_usVertexCount; // we asked for a contiguous strip
		   //pBranches->m_pStripLengths
		   //pBranches->m_pStrips
           if(m_unBranchVertexCount > 0)
		   {//has branches
		      tempMesh = new CEXMeshObj();
			  tempMesh->List_materialObject = new CMaterialObject();

			  CSpeedTreeRT::STextures sTextures;
              m_pSpeedTree->GetTextures(sTextures);
		      CString branchTexture = sTextures.m_pBranchTextureFilename;

			  //branchTexture = sTextures.m_pCompositeFilename;
			
			  
			  tempMesh->List_storedTextureFromFile[0] = branchTexture;
			  tempMesh->List_storedTextureFromFile[1] = "bump_"+branchTexture;

			  tempMesh->m_abVertexArray.m_uvCount = 50;
			  tempMesh->m_abVertexArray.m_vertexCount = m_unBranchVertexCount;
			  tempMesh->m_abVertexArray.m_vertexArray3t = new ABVERTEX3Lb[m_unBranchVertexCount];
			  //fill verts
              D3DMATRIX offsetRotation;
			  D3DVECTOR tempReg;
			  GL_matrixFunctions.Rotate(-90.0f,0.0f,0.0f,offsetRotation.m);

			  tempReg = GL_matrixFunctions.UpdatePolygonVertex(offsetRotation.m,tempReg);

			  //
			  for(int v=0;v<m_unBranchVertexCount;v++)
			  {
				  tempReg.x = tempMesh->m_abVertexArray.m_vertexArray3t[v].x = pBranches->m_pCoords[v * 3];
				  tempReg.y = tempMesh->m_abVertexArray.m_vertexArray3t[v].y = pBranches->m_pCoords[v * 3+1];
				  tempReg.z = tempMesh->m_abVertexArray.m_vertexArray3t[v].z = pBranches->m_pCoords[v * 3+2];

				  tempReg = GL_matrixFunctions.UpdatePolygonVertex(offsetRotation.m,tempReg);
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].x = tempReg.x;
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].y = tempReg.y;
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].z = tempReg.z;

				  tempReg.x = tempMesh->m_abVertexArray.m_vertexArray3t[v].nx = pBranches->m_pNormals[v * 3];
				  tempReg.y = tempMesh->m_abVertexArray.m_vertexArray3t[v].ny = pBranches->m_pNormals[v * 3+1];
				  tempReg.z = tempMesh->m_abVertexArray.m_vertexArray3t[v].nz = pBranches->m_pNormals[v * 3+2];

				  tempReg = GL_matrixFunctions.UpdatePolygonVertexRotationOnly(offsetRotation.m,tempReg);
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].nx = tempReg.x;
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].ny = tempReg.y;
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].nz = tempReg.z;

				  tempReg.x = tempMesh->m_abVertexArray.m_vertexArray3t[v].tX = pBranches->m_pTangents[v * 3];
				  tempReg.y = tempMesh->m_abVertexArray.m_vertexArray3t[v].tY = pBranches->m_pTangents[v * 3+1];
				  tempReg.z = tempMesh->m_abVertexArray.m_vertexArray3t[v].tZ = pBranches->m_pTangents[v * 3+2];

				  tempReg = GL_matrixFunctions.UpdatePolygonVertexRotationOnly(offsetRotation.m,tempReg);
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].tX = tempReg.x;
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].tY = tempReg.y;
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].tZ = tempReg.z;


                  //pBranches->

				  tempMesh->m_abVertexArray.m_vertexArray3t[v].tx0.tu = pBranches->m_pTexCoords0[v * 2];
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].tx0.tv = pBranches->m_pTexCoords0[v * 2+1];
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].tx1.tu = pBranches->m_pTexCoords0[v * 2];
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].tx1.tv = pBranches->m_pTexCoords0[v * 2+1];
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].tx2.tu = pBranches->m_pWindWeights[v];
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].tx2.tv = 0.0f;
			  }
			  //generate indecies
              //first get size
			  
			  tempMesh->m_abVertexArray.m_indecieCount = pBranches->m_pStripLengths[0];
			  tempMesh->m_abVertexArray.m_indecieArray = new WORD[tempMesh->m_abVertexArray.m_indecieCount];
			  memcpy(tempMesh->m_abVertexArray.m_indecieArray, pBranches->m_pStrips[0], pBranches->m_pStripLengths[0] * sizeof(WORD));

              //delete m_pGeometryCache;
			 // m_pGeometryCache = 0;

		   }
		   return tempMesh;

}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
CEXMeshObj * CTreeImporterObj::GetFrondsEXMeshObjFromSpeedTree(float lodVal,CSpeedTreeRT* m_pSpeedTree,CSpeedTreeRT::SGeometry* m_pGeometryCache)
{
	       CEXMeshObj * tempMesh = NULL;

    

           m_pSpeedTree->SetLodLevel(lodVal);//0.0f - 1.0f

		   //if (!m_pSpeedTree->Compute(NULL, 1))
		   //	   return NULL;
       

		   //CSpeedTreeRT::SGeometry* m_pGeometryCache = NULL;	
           if (m_pGeometryCache == NULL)
		       m_pGeometryCache = new CSpeedTreeRT::SGeometry;
		   m_pSpeedTree->GetGeometry(*m_pGeometryCache);
		   

		   

		   //Port primitive
		   CSpeedTreeRT::SGeometry::SIndexed* pBranches = &(m_pGeometryCache->m_sFronds);
	       int m_unBranchVertexCount = pBranches->m_usVertexCount; // we asked for a contiguous strip
           if(!pBranches->m_pStripLengths)
               return NULL;
		   //GL_systemReadout->AddLine(m_pGeometryCache->m_fFrondAlphaTestValue,g_pd3dDevice,m_symbolMapDB,m_speechMode);
		   //pBranches->m_pStripLengths
		   //pBranches->m_pStrips
           if(m_unBranchVertexCount > 0)
		   {//has branches
		      tempMesh = new CEXMeshObj();
			  tempMesh->List_materialObject = new CMaterialObject();
			  tempMesh->List_materialObject->m_doAlphaDepthTest = TRUE;
			  //tempMesh->List_materialObject->m_blendMode = 1;
			  tempMesh->List_materialObject->m_cullOn = FALSE;

			  CSpeedTreeRT::STextures sTextures;
              m_pSpeedTree->GetTextures(sTextures);
		      CString branchTexture = sTextures.m_pFrondTextureFilenames[0];
			  branchTexture = sTextures.m_pCompositeFilename;
			  
			  tempMesh->List_storedTextureFromFile[0] = branchTexture;
			  //tempMesh->List_storedTextureFromFile[1] = "bump_"+branchTexture;

			  tempMesh->m_abVertexArray.m_uvCount = 50;
			  tempMesh->m_abVertexArray.m_vertexCount = m_unBranchVertexCount;
			  tempMesh->m_abVertexArray.m_vertexArray3t = new ABVERTEX3Lb[m_unBranchVertexCount];
			  //fill verts
              D3DMATRIX offsetRotation;
			  D3DVECTOR tempReg;
			  GL_matrixFunctions.Rotate(-90.0f,0.0f,0.0f,offsetRotation.m);

			  tempReg = GL_matrixFunctions.UpdatePolygonVertex(offsetRotation.m,tempReg);

			  //
			  for(int v=0;v<m_unBranchVertexCount;v++)
			  {
				  tempReg.x = tempMesh->m_abVertexArray.m_vertexArray3t[v].x = pBranches->m_pCoords[v * 3];
				  tempReg.y = tempMesh->m_abVertexArray.m_vertexArray3t[v].y = pBranches->m_pCoords[v * 3+1];
				  tempReg.z = tempMesh->m_abVertexArray.m_vertexArray3t[v].z = pBranches->m_pCoords[v * 3+2];

				  tempReg = GL_matrixFunctions.UpdatePolygonVertex(offsetRotation.m,tempReg);
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].x = tempReg.x;
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].y = tempReg.y;
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].z = tempReg.z;

				  tempReg.x = tempMesh->m_abVertexArray.m_vertexArray3t[v].nx = pBranches->m_pNormals[v * 3];
				  tempReg.y = tempMesh->m_abVertexArray.m_vertexArray3t[v].ny = pBranches->m_pNormals[v * 3+1];
				  tempReg.z = tempMesh->m_abVertexArray.m_vertexArray3t[v].nz = pBranches->m_pNormals[v * 3+2];

				  tempReg = GL_matrixFunctions.UpdatePolygonVertexRotationOnly(offsetRotation.m,tempReg);
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].nx = tempReg.x;
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].ny = tempReg.y;
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].nz = tempReg.z;

				  tempReg.x = tempMesh->m_abVertexArray.m_vertexArray3t[v].tX = pBranches->m_pTangents[v * 3];
				  tempReg.y = tempMesh->m_abVertexArray.m_vertexArray3t[v].tY = pBranches->m_pTangents[v * 3+1];
				  tempReg.z = tempMesh->m_abVertexArray.m_vertexArray3t[v].tZ = pBranches->m_pTangents[v * 3+2];

				  tempReg = GL_matrixFunctions.UpdatePolygonVertexRotationOnly(offsetRotation.m,tempReg);
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].tX = tempReg.x;
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].tY = tempReg.y;
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].tZ = tempReg.z;

				  tempMesh->m_abVertexArray.m_vertexArray3t[v].tx0.tu = pBranches->m_pTexCoords0[v * 2];
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].tx0.tv = pBranches->m_pTexCoords0[v * 2+1];
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].tx1.tu = pBranches->m_pTexCoords0[v * 2];
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].tx1.tv = pBranches->m_pTexCoords0[v * 2+1];
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].tx2.tu = pBranches->m_pWindWeights[v];
				  tempMesh->m_abVertexArray.m_vertexArray3t[v].tx2.tv = 0.0f;
			  }
			  //generate indecies
              //first get size
			  
			  tempMesh->m_abVertexArray.m_indecieCount = pBranches->m_pStripLengths[0];
			  tempMesh->m_abVertexArray.m_indecieArray = new WORD[tempMesh->m_abVertexArray.m_indecieCount];
			  memcpy(tempMesh->m_abVertexArray.m_indecieArray, pBranches->m_pStrips[0], pBranches->m_pStripLengths[0] * sizeof(WORD));

              //delete m_pGeometryCache;
			  //m_pGeometryCache = 0;

		   }
		   return tempMesh;

}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
CEXMeshObj * CTreeImporterObj::GetLeafBuffersEXMesh(CSpeedTreeRT* m_pSpeedTree, 
											  Ext_lockpoints ** lockPointsReturn, 
											  int & lockCount,
											  float & billboardSize,
											  CSpeedTreeRT::SGeometry* m_pGeometryCache,
											  int lodOffset,
											  float lodLevel)
{

	   CEXMeshObj * tempMesh = NULL;
	   lodOffset = 1;

	   m_pSpeedTree->SetLodLevel(lodLevel);
	   //CSpeedTreeRT::SGeometry* m_pGeometryCache = NULL;	
        if (m_pGeometryCache == NULL)
		    m_pGeometryCache = new CSpeedTreeRT::SGeometry;
	    m_pSpeedTree->GetGeometry(*m_pGeometryCache);
		
	    lockCount = m_pGeometryCache->m_sLeaves0.m_usLeafCount;
		if (lockCount < 1)
			return NULL;
        
		const CSpeedTreeRT::SGeometry::SLeaf* pLeaf = &m_pGeometryCache->m_sLeaves0;
        //Generate Billboard
		CSpeedTreeRT::STextures sTextures;
        m_pSpeedTree->GetTextures(sTextures);
        CString textureName = sTextures.m_pLeafTextureFilenames[0];      
		
		billboardSize = fabs(pLeaf->m_pLeafMapCoords[0][0]);
		
		tempMesh = new CEXMeshObj();
	    tempMesh->List_materialObject = new CMaterialObject();
		tempMesh->List_materialObject->m_doAlphaDepthTest = TRUE;
		//tempMesh->List_materialObject->m_blendMode = 1;
		tempMesh->List_storedTextureFromFile[0] = textureName;

		tempMesh->List_storedTextureFromFile[0] = sTextures.m_pCompositeFilename;
		tempMesh->m_abVertexArray.m_uvCount = 2;
   
		
		//Parse Lock Ppoints
		D3DMATRIX offsetRotation;
	    D3DVECTOR tempReg;
	    GL_matrixFunctions.Rotate(-90.0f,0.0f,0.0f,offsetRotation.m);
		Ext_lockpoints * lockPointArray = new Ext_lockpoints[lockCount];
		ZeroMemory(lockPointArray,sizeof(Ext_lockpoints)*lockCount);

		for (unsigned int unLeaf = 0; unLeaf < lockCount; ++unLeaf)
		{
			tempReg.x = pLeaf->m_pCenterCoords[unLeaf * 3];
			tempReg.y = pLeaf->m_pCenterCoords[unLeaf * 3+1];
			tempReg.z = pLeaf->m_pCenterCoords[unLeaf * 3+2];

			float weight = pLeaf->m_pWindWeights[unLeaf];

			tempReg = GL_matrixFunctions.UpdatePolygonVertexRotationOnly(offsetRotation.m,tempReg);
			lockPointArray[unLeaf].x = tempReg.x;
			lockPointArray[unLeaf].y = tempReg.y;
			lockPointArray[unLeaf].z = tempReg.z;
			lockPointArray[unLeaf].diffuse = pLeaf->m_pColors[unLeaf];

		}
		if(lockPointsReturn)
        *lockPointsReturn = lockPointArray;
		//generate mesh
        tempMesh->m_abVertexArray.m_vertexCount  = lockCount * 4;
	    tempMesh->m_abVertexArray.m_indecieCount = lockCount * 6;
		tempMesh->m_abVertexArray.m_uvCount = 70;

		tempMesh->m_abVertexArray.m_vertexArray1x = new ABVERTEX1Lx[tempMesh->m_abVertexArray.m_vertexCount];
	    ZeroMemory(tempMesh->m_abVertexArray.m_vertexArray1x, sizeof(ABVERTEX1Lx) * tempMesh->m_abVertexArray.m_vertexCount);
	    tempMesh->m_abVertexArray.m_indecieArray = new WORD[tempMesh->m_abVertexArray.m_indecieCount];
	    ZeroMemory(tempMesh->m_abVertexArray.m_indecieArray, sizeof(WORD) * tempMesh->m_abVertexArray.m_indecieCount);
		

		 for(int vertexBuild=0;vertexBuild<lockCount;vertexBuild++)
		 {
		    //verticies per lock
            
			 
			//indecies per lock 
			tempMesh->m_abVertexArray.m_indecieArray[vertexBuild*6+0] = vertexBuild*4+3;
	        tempMesh->m_abVertexArray.m_indecieArray[vertexBuild*6+1] = vertexBuild*4+0;
	        tempMesh->m_abVertexArray.m_indecieArray[vertexBuild*6+2] = vertexBuild*4+1;
	        tempMesh->m_abVertexArray.m_indecieArray[vertexBuild*6+3] = vertexBuild*4+3;
	        tempMesh->m_abVertexArray.m_indecieArray[vertexBuild*6+4] = vertexBuild*4+1;
	        tempMesh->m_abVertexArray.m_indecieArray[vertexBuild*6+5] = vertexBuild*4+2;
	     }
		 
		 //init vals
			int tempOffset=0;
			int loopOffset=lodOffset;
			if(loopOffset < 1)
			   loopOffset = 1;
			int skipTrace=0;
			int skipVal=1;
			tempMesh->m_abVertexArray.m_indecieCount = 0;
			for(int vertexBuild=0;vertexBuild<lockCount;vertexBuild = vertexBuild + loopOffset)
		    {//BEGIN LOOP
				/*if(skipTrace >= skipVal)
				{
				   skipTrace = 0;
				   continue;
				}*/
		    //diffuse
            const float* pTex = pLeaf->m_pLeafMapTexCoords[vertexBuild]; 
			const float* pCoords = pLeaf->m_pLeafMapCoords[vertexBuild]; 
            //float bSize = pCoords[2];
			int ranZpitch = (int)((billboardSize * .2f)*10000.0f);
			float upSide = ((float)(rand()%ranZpitch))*.0001f;
			float dSide = -upSide;


			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].diffuse = lockPointArray[vertexBuild].diffuse;
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].x  = lockPointArray[vertexBuild].x;
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].y  = lockPointArray[vertexBuild].y;
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].z  = lockPointArray[vertexBuild].z;//*/
	        //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tu = 1.0f;//1.0f
	        //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tv = 0.0f;//0.0f
			CopyMemory(&tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tu,&pTex[0],sizeof(float));//1.0f
	        CopyMemory(&tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tv,&pTex[1],sizeof(float));//1.0f
			//
			
            //upper right
            //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].ox = pCoords[0];
			//tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].oy = pCoords[1];
			//tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].oz = pCoords[2];
            
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].ox = billboardSize;
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].oy = billboardSize;
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].oz = upSide;
 
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx1.tu = 0.0f;//50.0f;//misc const ref
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset++].tx1.tv = pLeaf->m_pWindWeights[vertexBuild];
			//tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx2.tu = pCoords[2];//50.0f;//misc const ref
			//tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset++].tx2.tv = pLeaf->m_pWindWeights[vertexBuild];
	        //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset++].tx1.tv = pLeaf->m_pWindWeights[vertexBuild];//0.0f


	        /*m_meshObject->m_abVertexArray.m_vertexArray1x[tempOffset].nx = normal.x;
	        m_meshObject->m_abVertexArray.m_vertexArray1x[tempOffset].ny = normal.y;
	        m_meshObject->m_abVertexArray.m_vertexArray1x[tempOffset++].nz = normal.z;*/
	        //vertex 2 bottom left
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].diffuse = lockPointArray[vertexBuild].diffuse;
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].x  = lockPointArray[vertexBuild].x;
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].y  = lockPointArray[vertexBuild].y;
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].z  = lockPointArray[vertexBuild].z;//*/
	        //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tu = 1.0f;//0.0f
	        //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tv = 1.0f;//0.0f

            CopyMemory(&tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tu,&pTex[2],sizeof(float));//1.0f
	        CopyMemory(&tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tv,&pTex[3],sizeof(float));//1.0f

			//lower right
			//tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].ox = pCoords[4];
			//tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].oy = pCoords[5];
			//tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].oz = pCoords[6];

			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].ox = billboardSize;
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].oy = -billboardSize;
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].oz = dSide;
			

			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx1.tu = 0.0f;//50.0f;//misc const ref
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset++].tx1.tv = pLeaf->m_pWindWeights[vertexBuild];
			//tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx1.tu = 51.0f;//misc const ref
	        //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset++].tx1.tv = pLeaf->m_pWindWeights[vertexBuild];//0.0f
	        
			
			
			/*pVertices1[tempOffset].nx = normal.x;
	        pVertices1[tempOffset].ny = normal.y;
	        pVertices1[tempOffset++].nz = normal.z;*/
	        //vertex 3 bottom right
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].diffuse = lockPointArray[vertexBuild].diffuse;
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].x  = lockPointArray[vertexBuild].x;
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].y  = lockPointArray[vertexBuild].y;
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].z  = lockPointArray[vertexBuild].z;//*/
	        //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tu = 0.0f;//0.0f
	        //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tv = 1.0f;//1.0f
            CopyMemory(&tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tu,&pTex[4],sizeof(float));//1.0f
	        CopyMemory(&tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tv,&pTex[5],sizeof(float));//1.0f

			//lower left
			//tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].ox = pCoords[8];
			//tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].oy = pCoords[9];
			//tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].oz = pCoords[10];
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].ox = -billboardSize;
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].oy = -billboardSize;
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].oz = dSide;
			
			

			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx1.tu = 0.0f;//50.0f;//misc const ref
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset++].tx1.tv = pLeaf->m_pWindWeights[vertexBuild];
			//tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx1.tu = 52.0f;//misc const ref
	        //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset++].tx1.tv  = pLeaf->m_pWindWeights[vertexBuild];//0.0f
	        /*pVertices1[tempOffset].nx = normal.x;
	        pVertices1[tempOffset].ny = normal.y;
	        pVertices1[tempOffset++].nz = normal.z;*/
	        //vertex 4 upper right
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].diffuse = lockPointArray[vertexBuild].diffuse;
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].x  = lockPointArray[vertexBuild].x;
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].y  = lockPointArray[vertexBuild].y;
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].z  = lockPointArray[vertexBuild].z;//*/
	        //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tu = 0.0f;//1.0f
	        //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tv = 0.0f;//1.0f
            CopyMemory(&tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tu,&pTex[6],sizeof(float));//1.0f
	        CopyMemory(&tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tv,&pTex[7],sizeof(float));//1.0f

			//upper left
			//tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].ox = pCoords[12];
			//tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].oy = pCoords[13];
			//tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].oz = pCoords[14];
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].ox = -billboardSize;
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].oy = billboardSize;
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].oz = upSide;
			

			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx1.tu = 0.0f;//50.0f;//misc const ref
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset++].tx1.tv = pLeaf->m_pWindWeights[vertexBuild];
			//tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx1.tu = 53.0f;//misc const ref
	        //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset++].tx1.tv  = pLeaf->m_pWindWeights[vertexBuild];//0.0f
	        /*pVertices1[tempOffset].nx = normal.x;
	        pVertices1[tempOffset].ny = normal.y;
	        pVertices1[tempOffset++].nz = normal.z;*/
		
			tempMesh->m_abVertexArray.m_indecieCount += 6;
			skipTrace++;
			}//END LOOP
			tempMesh->m_abVertexArray.m_vertexCount = 0;
			tempMesh->m_abVertexArray.m_vertexCount = tempOffset;
		
	return tempMesh;
	
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
CEXMeshObj * CTreeImporterObj::GetBillboardBuffersEXMesh(CSpeedTreeRT* m_pSpeedTree, 
											             Ext_lockpoints ** lockPointsReturn, 
											             int & lockCount,
											             float & billboardSize,
											             CSpeedTreeRT::SGeometry* m_pGeometryCache,
											             int lodOffset,
														 CString title)
{

	   CEXMeshObj * tempMesh = NULL;

	   m_pSpeedTree->SetLodLevel(0.0f);
	   
	   //CSpeedTreeRT::SGeometry* m_pGeometryCache = NULL;	
        if (m_pGeometryCache == NULL)
		    m_pGeometryCache = new CSpeedTreeRT::SGeometry;

		m_pSpeedTree->GetGeometry(*m_pGeometryCache, SpeedTree_BillboardGeometry);
	    //m_pSpeedTree->GetGeometry(*m_pGeometryCache);
		//m_pSpeedTree->GetSimpleBillboardGeometry(*m_pGeometryCache);//(*m_pGeometryCache);
		//m_pSpeedTree->GetGeometry(*m_pGeometryCache);
		//m_pSpeedTree->GetGeometry(*m_pGeometryCache);
	    
		
		lockCount = 1;

		float pBounds[7]; 
		m_pSpeedTree->GetBoundingBox(pBounds);
        
        //Generate Billboard
		CSpeedTreeRT::STextures sTextures;
        //m_pSpeedTree->GetTextures(sTextures);
        //CString textureName = sTextures.m_pCompositeFilename;      
		
        //const float* pCoords = m_pGeometryCache->m_sHorizontalBillboard.m_pCoords;
		//const float* pTexCoords = m_pGeometryCache->m_sHorizontalBillboard.m_pTexCoords;
		const float* pTexCoords = m_pGeometryCache->m_sBillboard0.m_pTexCoords;
        if(!pTexCoords)
		pTexCoords = m_pGeometryCache->m_sHorizontalBillboard.m_pTexCoords;

		billboardSize = fabs(pBounds[3]);
		//billboardSizeY = fabs(pBounds[4]);

		//billboardSize = fabs(billboard->m_pLeafMapCoords[0][0]);
		
		tempMesh = new CEXMeshObj();
	    tempMesh->List_materialObject = new CMaterialObject();
		tempMesh->List_materialObject->m_doAlphaDepthTest = TRUE;
		//tempMesh->List_materialObject->m_blendMode = 1;
		tempMesh->List_storedTextureFromFile[0] = "comp_"+title+".tga";
        tempMesh->List_storedTextureFromFile[0] = sTextures.m_pCompositeFilename;
		tempMesh->m_abVertexArray.m_uvCount = 2;

		//tempMesh->List_boundingBox
   
		
		//Parse Lock Ppoints
		D3DMATRIX offsetRotation;
	    D3DVECTOR tempReg;
	    GL_matrixFunctions.Rotate(-90.0f,0.0f,0.0f,offsetRotation.m);
		Ext_lockpoints * lockPointArray = new Ext_lockpoints[lockCount];
		ZeroMemory(lockPointArray,sizeof(Ext_lockpoints)*lockCount);

		
		tempReg.x = 0.0f;//pLeaf->m_pCenterCoords[unLeaf * 3];
		tempReg.y = 0.0f;//pLeaf->m_pCenterCoords[unLeaf * 3+1];
		tempReg.z = 0.0f;//pLeaf->m_pCenterCoords[unLeaf * 3+2];

		float weight = 0.0f;//pLeaf->m_pWindWeights[unLeaf];
      

		tempReg = GL_matrixFunctions.UpdatePolygonVertexRotationOnly(offsetRotation.m,tempReg);
		lockPointArray[0].x = tempReg.x;
		lockPointArray[0].y = tempReg.y;
		lockPointArray[0].z = tempReg.z;
		lockPointArray[0].diffuse = D3DCOLOR_ARGB(255,255,255,255);

		
		if(lockPointsReturn)
          *lockPointsReturn = lockPointArray;
		//generate mesh
        tempMesh->m_abVertexArray.m_vertexCount  = lockCount * 4;
	    tempMesh->m_abVertexArray.m_indecieCount = lockCount * 6;
		tempMesh->m_abVertexArray.m_uvCount = 70;

		tempMesh->m_abVertexArray.m_vertexArray1x = new ABVERTEX1Lx[tempMesh->m_abVertexArray.m_vertexCount];
	    ZeroMemory(tempMesh->m_abVertexArray.m_vertexArray1x, sizeof(ABVERTEX1Lx) * tempMesh->m_abVertexArray.m_vertexCount);
	    tempMesh->m_abVertexArray.m_indecieArray = new WORD[tempMesh->m_abVertexArray.m_indecieCount];
	    ZeroMemory(tempMesh->m_abVertexArray.m_indecieArray, sizeof(WORD) * tempMesh->m_abVertexArray.m_indecieCount);
		

		 for(int vertexBuild=0;vertexBuild<lockCount;vertexBuild++)
		 {
            tempMesh->m_abVertexArray.m_indecieArray[vertexBuild*6+0] = vertexBuild*4+3;
	        tempMesh->m_abVertexArray.m_indecieArray[vertexBuild*6+1] = vertexBuild*4+0;
	        tempMesh->m_abVertexArray.m_indecieArray[vertexBuild*6+2] = vertexBuild*4+1;
	        tempMesh->m_abVertexArray.m_indecieArray[vertexBuild*6+3] = vertexBuild*4+3;
	        tempMesh->m_abVertexArray.m_indecieArray[vertexBuild*6+4] = vertexBuild*4+1;
	        tempMesh->m_abVertexArray.m_indecieArray[vertexBuild*6+5] = vertexBuild*4+2;
	     }
		 
		 //init vals
			int tempOffset=0;
			int loopOffset=lodOffset;
			if(loopOffset < 1)
			   loopOffset = 1;
			int skipTrace=0;
			int skipVal=1;
			tempMesh->m_abVertexArray.m_indecieCount = 0;
			for(int vertexBuild=0;vertexBuild<lockCount;vertexBuild = vertexBuild + loopOffset)
		    {//BEGIN LOOP
				/*if(skipTrace >= skipVal)
				{
				   skipTrace = 0;
				   continue;
				}*/
		    //diffuse
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].diffuse = lockPointArray[vertexBuild].diffuse;
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].x  = lockPointArray[vertexBuild].x;
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].y  = lockPointArray[vertexBuild].y;
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].z  = lockPointArray[vertexBuild].z;//*/
	        //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tu = 1.0f;//1.0f
	        //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tv = 0.0f;//0.0f
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tu = pTexCoords[0];//1.0f
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tv = pTexCoords[1];//0.0f
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx1.tu = 50.0f;//misc const ref
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset++].tx1.tv = 0.0f;//0.0f
	        /*m_meshObject->m_abVertexArray.m_vertexArray1x[tempOffset].nx = normal.x;
	        m_meshObject->m_abVertexArray.m_vertexArray1x[tempOffset].ny = normal.y;
	        m_meshObject->m_abVertexArray.m_vertexArray1x[tempOffset++].nz = normal.z;*/
	        //vertex 2 bottom left
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].diffuse = lockPointArray[vertexBuild].diffuse;
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].x  = lockPointArray[vertexBuild].x;
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].y  = lockPointArray[vertexBuild].y;
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].z  = lockPointArray[vertexBuild].z;//*/
	        //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tu = 1.0f;//0.0f
	        //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tv = 1.0f;//0.0f
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tu = pTexCoords[2];//1.0f
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tv = pTexCoords[3];//0.0f
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx1.tu = 51.0f;//misc const ref
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset++].tx1.tv = 0.0f;//0.0f
	        /*pVertices1[tempOffset].nx = normal.x;
	        pVertices1[tempOffset].ny = normal.y;
	        pVertices1[tempOffset++].nz = normal.z;*/
	        //vertex 3 bottom right
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].diffuse = lockPointArray[vertexBuild].diffuse;
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].x  = lockPointArray[vertexBuild].x;
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].y  = lockPointArray[vertexBuild].y;
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].z  = lockPointArray[vertexBuild].z;//*/
	        //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tu = 0.0f;//0.0f
	        //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tv = 1.0f;//1.0f
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tu = pTexCoords[4];//1.0f
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tv = pTexCoords[5];//0.0f
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx1.tu = 52.0f;//misc const ref
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset++].tx1.tv  = 0.0f;//0.0f
	        /*pVertices1[tempOffset].nx = normal.x;
	        pVertices1[tempOffset].ny = normal.y;
	        pVertices1[tempOffset++].nz = normal.z;*/
	        //vertex 4 upper right
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].diffuse = lockPointArray[vertexBuild].diffuse;
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].x  = lockPointArray[vertexBuild].x;
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].y  = lockPointArray[vertexBuild].y;
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].z  = lockPointArray[vertexBuild].z;//*/
	        //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tu = 0.0f;//1.0f
	        //tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tv = 0.0f;//1.0f
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tu = pTexCoords[6];//1.0f
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx0.tv = pTexCoords[7];//0.0f
			tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset].tx1.tu = 53.0f;//misc const ref
	        tempMesh->m_abVertexArray.m_vertexArray1x[tempOffset++].tx1.tv  = 0.0f;//0.0f
	        /*pVertices1[tempOffset].nx = normal.x;
	        pVertices1[tempOffset].ny = normal.y;
	        pVertices1[tempOffset++].nz = normal.z;*/
		
			tempMesh->m_abVertexArray.m_indecieCount += 6;
			skipTrace++;
			}//END LOOP
			tempMesh->m_abVertexArray.m_vertexCount = 0;
			tempMesh->m_abVertexArray.m_vertexCount = tempOffset;
		
	return tempMesh;
	
}
//------------------------------------------------------------------------------//
//------------------------------------------------------------------------------//
//------------------------------------------------------------------------------//
BOOL CTreeImporterObj::GetLibraryObjFromSPTFile(CReferenceObjList * refDB,
												CString * textureSearchPaths,
												int & searchPathCount,
												LPDIRECT3DDEVICE9 pd3dDevice
												)
{
   CString CurDir;
   CString fileTitle = "";
   CString fileName,textureSearchPath,texPathX,objectName;
   ABBOX glBounding;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "SPT ENGIN|*.SPT|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "SPT", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, NULL );
     if ( opendialog.DoModal() == IDOK )
	 {
	   fileName = opendialog.GetFileName();
	   textureSearchPath = opendialog.GetPathName();
	   objectName = opendialog.GetFileTitle();
	   
	 }
	 
     
	 textureSearchPath.Delete(textureSearchPath.GetLength()-fileName.GetLength(),fileName.GetLength());
	 texPathX = textureSearchPath;
	 SetCurrentDirectory(textureSearchPath);
	 textureSearchPath.Delete(0,CurDir.GetLength());
	 
    float treeSize = 0.0f;
	int leafLodCount = 5;
	CTreeImportOptionsDlg dlgOpt;
	dlgOpt.m_sizeOverride = treeSize;
	dlgOpt.m_leafLodCount = leafLodCount;
	dlgOpt.DoModal();
	treeSize = dlgOpt.m_sizeOverride;
	leafLodCount = dlgOpt.m_leafLodCount;
    
	float lod1Dist = 4.0f;
    float lod2Dist = 9.0f;
	float lod3Dist = 17.0f;
	float lod4Dist = 26.0f;
    float lod5Dist = 50.0f;

    
    CSpeedTreeRT* m_pSpeedTree = new CSpeedTreeRT();


	if(m_pSpeedTree->LoadTree(fileName))
	{
		m_pSpeedTree->SetTextureFlip(true);
		m_pSpeedTree->SetBranchLightingMethod(CSpeedTreeRT::LIGHT_DYNAMIC);
		m_pSpeedTree->SetLeafLightingMethod(CSpeedTreeRT::LIGHT_DYNAMIC);
		m_pSpeedTree->SetFrondLightingMethod(CSpeedTreeRT::LIGHT_DYNAMIC);
		m_pSpeedTree->SetBranchWindMethod(CSpeedTreeRT::WIND_GPU);
		m_pSpeedTree->SetLeafWindMethod(CSpeedTreeRT::WIND_GPU);
		m_pSpeedTree->SetFrondWindMethod(CSpeedTreeRT::WIND_GPU);
		m_pSpeedTree->SetDropToBillboard(true);

		int tractSq = 0;
	
		textureSearchPaths[searchPathCount++] = texPathX;
		 
           if(treeSize > 0)
		   m_pSpeedTree->SetTreeSize(treeSize, 0.0f);
		   
		   float vXl = 0;
		   m_pSpeedTree->GetTreeSize(treeSize,vXl);


		   //m_pSpeedTree->

		   if (!m_pSpeedTree->Compute(NULL, 1))
			   return FALSE;

		      CSpeedTreeRT::SGeometry* m_pGeometryCache = NULL;	
              if (m_pGeometryCache == NULL)
		          m_pGeometryCache = new CSpeedTreeRT::SGeometry;
		      m_pSpeedTree->GetGeometry(*m_pGeometryCache);

              //Branches/base
			  CReferenceObj * newLibObj = new CReferenceObj();
			  newLibObj->m_referenceName = objectName;
              newLibObj->m_meshObject = GetEXMeshObjFromSpeedTree(1.0f,m_pSpeedTree,m_pGeometryCache);//get primary mesh
              //generate 3 lods
			  float visiblerange = treeSize*10.0f;
			  newLibObj->m_lodVisualList = new CMeshLODObjectList();
			  newLibObj->m_lodVisualList->m_lodType = 0;


			  GetEXMeshObjFromSpeedTree(0.9f,m_pSpeedTree,m_pGeometryCache);
			  
			  CMeshLODObject * lod1Obj = new CMeshLODObject();
			  lod1Obj->m_distance = treeSize*lod1Dist;
			  lod1Obj->m_boxOption.maxX = treeSize*lod1Dist;
			  lod1Obj->m_boxOption.maxY = treeSize*lod1Dist;
			  lod1Obj->m_boxOption.maxZ = treeSize*lod1Dist;
			  lod1Obj->m_boxOption.minX = -(treeSize*lod1Dist);
			  lod1Obj->m_boxOption.minY = -(treeSize*lod1Dist);
			  lod1Obj->m_boxOption.minZ = -(treeSize*lod1Dist);
			  lod1Obj->m_meshLodLevel = GetEXMeshObjFromSpeedTree(0.7f,m_pSpeedTree,m_pGeometryCache);//get lod1 actual lod2
		      if(lod1Obj->m_meshLodLevel)
			  newLibObj->m_lodVisualList->AddTail(lod1Obj);

			  CMeshLODObject * lod2Obj = new CMeshLODObject();
			  lod2Obj->m_distance = treeSize*lod2Dist;
			  lod2Obj->m_boxOption.maxX = treeSize*lod2Dist;
			  lod2Obj->m_boxOption.maxY = treeSize*lod2Dist;
			  lod2Obj->m_boxOption.maxZ = treeSize*lod2Dist;
			  lod2Obj->m_boxOption.minX = -(treeSize*lod2Dist);
			  lod2Obj->m_boxOption.minY = -(treeSize*lod2Dist);
			  lod2Obj->m_boxOption.minZ = -(treeSize*lod2Dist);
			  lod2Obj->m_meshLodLevel = GetEXMeshObjFromSpeedTree(0.5f,m_pSpeedTree,m_pGeometryCache);//get lod2 actual lod1
              if(lod2Obj->m_meshLodLevel)
		      newLibObj->m_lodVisualList->AddTail(lod2Obj);

			  //remove valid
			  CMeshLODObject * lod3Obj = new CMeshLODObject();
			  lod3Obj->m_distance = treeSize*lod3Dist;
			  lod3Obj->m_boxOption.maxX = treeSize*lod3Dist;
			  lod3Obj->m_boxOption.maxY = treeSize*lod3Dist;
			  lod3Obj->m_boxOption.maxZ = treeSize*lod3Dist;
			  lod3Obj->m_boxOption.minX = -(treeSize*lod3Dist);
			  lod3Obj->m_boxOption.minY = -(treeSize*lod3Dist);
			  lod3Obj->m_boxOption.minZ = -(treeSize*lod3Dist);
			  lod3Obj->m_meshLodLevel = GetEXMeshObjFromSpeedTree(0.3f,m_pSpeedTree,m_pGeometryCache);//get lod2 actual lod1
			  if(lod3Obj->m_meshLodLevel)
		      newLibObj->m_lodVisualList->AddTail(lod3Obj);

			  //AddTextureToDatabase(newLibObj->m_meshObject);//apply mapping and add to DB
			  
			  newLibObj->m_popoutBox.max.x = treeSize*lod4Dist;
			  newLibObj->m_popoutBox.max.y = treeSize*lod4Dist;
			  newLibObj->m_popoutBox.max.z = treeSize*lod4Dist;
			  newLibObj->m_popoutBox.min.x = -(treeSize*lod4Dist);
			  newLibObj->m_popoutBox.min.y = -(treeSize*lod4Dist);
			  newLibObj->m_popoutBox.min.z = -(treeSize*lod4Dist);
			  refDB->AddTail(newLibObj);

			  CReferenceObj * branchRefObj = newLibObj;

			  //Fronds/base
			  newLibObj = new CReferenceObj();
			  newLibObj->m_referenceName = objectName + "_Fronds";
              newLibObj->m_meshObject = GetFrondsEXMeshObjFromSpeedTree(1.0f,m_pSpeedTree,m_pGeometryCache);//get primary mesh
			  if(newLibObj->m_meshObject)
			  {
				  tractSq++;

              //generate 3 lods
			  //visiblerange = treeSize*10.0f;
			  newLibObj->m_lodVisualList = new CMeshLODObjectList();
			  newLibObj->m_lodVisualList->m_lodType = 0;

			  GetEXMeshObjFromSpeedTree(0.9f,m_pSpeedTree,m_pGeometryCache);
			  
			  lod1Obj = new CMeshLODObject();
			  lod1Obj->m_distance = treeSize*lod1Dist;
			  lod1Obj->m_boxOption.maxX = treeSize*lod1Dist;
			  lod1Obj->m_boxOption.maxY = treeSize*lod1Dist;
			  lod1Obj->m_boxOption.maxZ = treeSize*lod1Dist;
			  lod1Obj->m_boxOption.minX = -(treeSize*lod1Dist);
			  lod1Obj->m_boxOption.minY = -(treeSize*lod1Dist);
			  lod1Obj->m_boxOption.minZ = -(treeSize*lod1Dist);
			  lod1Obj->m_meshLodLevel = GetFrondsEXMeshObjFromSpeedTree(0.8f,m_pSpeedTree,m_pGeometryCache);//get lod1 actual lod2
		      if(lod1Obj->m_meshLodLevel)
			     newLibObj->m_lodVisualList->AddTail(lod1Obj);

			  lod2Obj = new CMeshLODObject();
			  lod2Obj->m_distance = treeSize*lod2Dist;
			  lod2Obj->m_boxOption.maxX = treeSize*lod2Dist;
			  lod2Obj->m_boxOption.maxY = treeSize*lod2Dist;
			  lod2Obj->m_boxOption.maxZ = treeSize*lod2Dist;
			  lod2Obj->m_boxOption.minX = -(treeSize*lod2Dist);
			  lod2Obj->m_boxOption.minY = -(treeSize*lod2Dist);
			  lod2Obj->m_boxOption.minZ = -(treeSize*lod2Dist);
			  lod2Obj->m_meshLodLevel = GetFrondsEXMeshObjFromSpeedTree(0.5f,m_pSpeedTree,m_pGeometryCache);//get lod2 actual lod1
		      if(lod2Obj->m_meshLodLevel)
			     newLibObj->m_lodVisualList->AddTail(lod2Obj);

			  lod3Obj = new CMeshLODObject();
			  lod3Obj->m_distance = treeSize*lod3Dist;
			  lod3Obj->m_boxOption.maxX = treeSize*lod3Dist;
			  lod3Obj->m_boxOption.maxY = treeSize*lod3Dist;
			  lod3Obj->m_boxOption.maxZ = treeSize*lod3Dist;
			  lod3Obj->m_boxOption.minX = -(treeSize*lod3Dist);
			  lod3Obj->m_boxOption.minY = -(treeSize*lod3Dist);
			  lod3Obj->m_boxOption.minZ = -(treeSize*lod3Dist);
			  lod3Obj->m_meshLodLevel = GetFrondsEXMeshObjFromSpeedTree(0.3f,m_pSpeedTree,m_pGeometryCache);//get lod2 actual lod1
		      if(lod3Obj->m_meshLodLevel)
			  newLibObj->m_lodVisualList->AddTail(lod3Obj);

			  //AddTextureToDatabase(newLibObj->m_meshObject);//apply mapping and add to DB
			  newLibObj->m_popoutBox.max.x = treeSize*lod4Dist;
			  newLibObj->m_popoutBox.max.y = treeSize*lod4Dist;
			  newLibObj->m_popoutBox.max.z = treeSize*lod4Dist;
			  newLibObj->m_popoutBox.min.x = -(treeSize*lod4Dist);
			  newLibObj->m_popoutBox.min.y = -(treeSize*lod4Dist);
			  newLibObj->m_popoutBox.min.z = -(treeSize*lod4Dist);
			  refDB->AddTail(newLibObj);
			  
			  }

              //generate leaves
              newLibObj = new CReferenceObj();
			  newLibObj->m_lodVisualList = new CMeshLODObjectList();
			  newLibObj->m_lodVisualList->m_lodType = 2;
			  newLibObj->m_popoutBox.max.x = treeSize*lod5Dist;
			  newLibObj->m_popoutBox.max.y = treeSize*lod5Dist;
			  newLibObj->m_popoutBox.max.z = treeSize*lod5Dist;
			  newLibObj->m_popoutBox.min.x = -(treeSize*lod5Dist);
			  newLibObj->m_popoutBox.min.y = -(treeSize*lod5Dist);
			  newLibObj->m_popoutBox.min.z = -(treeSize*lod5Dist);
			  newLibObj->m_referenceName = objectName + "_Leaves";
			  float BillBoardSz = 0.0f;
              newLibObj->m_meshObject = GetLeafBuffersEXMesh(m_pSpeedTree, 
										                    &newLibObj->m_extLockPoints, 
											                 newLibObj->m_lockPointCount,
															 BillBoardSz,
															 m_pGeometryCache,
															 1,
															 1.0f);
			  
			  
			  if(newLibObj->m_meshObject)
			  {
			  newLibObj->m_billboardSz = BillBoardSz;
			  newLibObj->m_lodRange = treeSize*15.0f;
	          newLibObj->m_lodLevels = leafLodCount;
			  D3DVECTOR temp;
			  refDB->AddTail(newLibObj);
			  float pBounds[7]; 
		      m_pSpeedTree->GetBoundingBox(pBounds);
              newLibObj->m_meshObject->List_boundingBox.minX = pBounds[0];
			  newLibObj->m_meshObject->List_boundingBox.minZ = pBounds[1];
			  newLibObj->m_meshObject->List_boundingBox.minY = pBounds[2];
			  newLibObj->m_meshObject->List_boundingBox.maxX = pBounds[3];
			  newLibObj->m_meshObject->List_boundingBox.maxZ = pBounds[4];
			  newLibObj->m_meshObject->List_boundingBox.maxY = pBounds[5];
			  tractSq++;
			  
              
			  lod1Obj = new CMeshLODObject();
			  lod1Obj->m_distance = treeSize*lod1Dist;
			  lod1Obj->m_boxOption.maxX = treeSize*lod1Dist;
			  lod1Obj->m_boxOption.maxY = treeSize*lod1Dist;
			  lod1Obj->m_boxOption.maxZ = treeSize*lod1Dist;
			  lod1Obj->m_boxOption.minX = -(treeSize*lod1Dist);
			  lod1Obj->m_boxOption.minY = -(treeSize*lod1Dist);
			  lod1Obj->m_boxOption.minZ = -(treeSize*lod1Dist);
			  int fill;
			  lod1Obj->m_meshLodLevel = GetLeafBuffersEXMesh(m_pSpeedTree, 
										                     NULL, 
											                 fill,
															 BillBoardSz,
															 m_pGeometryCache,
															 2,
															 .7);
			  if(lod1Obj->m_meshLodLevel)
		      newLibObj->m_lodVisualList->AddTail(lod1Obj);

			  lod2Obj = new CMeshLODObject();
			  lod2Obj->m_distance = treeSize*lod2Dist;
			  lod2Obj->m_boxOption.maxX = treeSize*lod2Dist;
			  lod2Obj->m_boxOption.maxY = treeSize*lod2Dist;
			  lod2Obj->m_boxOption.maxZ = treeSize*lod2Dist;
			  lod2Obj->m_boxOption.minX = -(treeSize*lod2Dist);
			  lod2Obj->m_boxOption.minY = -(treeSize*lod2Dist);
			  lod2Obj->m_boxOption.minZ = -(treeSize*lod2Dist);
			  lod2Obj->m_meshLodLevel = GetLeafBuffersEXMesh(m_pSpeedTree, 
										                     NULL, 
											                 fill,
															 BillBoardSz,
															 m_pGeometryCache,
															 4,
															 .5);
			  if(lod2Obj->m_meshLodLevel)
		      newLibObj->m_lodVisualList->AddTail(lod2Obj);
			  
			  lod3Obj = new CMeshLODObject();
			  lod3Obj->m_distance = treeSize*lod3Dist;
			  lod3Obj->m_boxOption.maxX = treeSize*lod3Dist;
			  lod3Obj->m_boxOption.maxY = treeSize*lod3Dist;
			  lod3Obj->m_boxOption.maxZ = treeSize*lod3Dist;
			  lod3Obj->m_boxOption.minX = -(treeSize*lod3Dist);
			  lod3Obj->m_boxOption.minY = -(treeSize*lod3Dist);
			  lod3Obj->m_boxOption.minZ = -(treeSize*lod3Dist);
			  lod3Obj->m_meshLodLevel = GetLeafBuffersEXMesh(m_pSpeedTree, 
										                     NULL, 
											                 fill,
															 BillBoardSz,
															 m_pGeometryCache,
															 8,
															 0.3);
			  if(lod3Obj->m_meshLodLevel)
		      newLibObj->m_lodVisualList->AddTail(lod3Obj);

              //billboard at the end of the chain
			  CMeshLODObject * lod4Obj = new CMeshLODObject();
			  lod4Obj->m_distance = treeSize*15.0f;
			  lod4Obj->m_meshLodLevel = GetBillboardBuffersEXMesh(m_pSpeedTree, 
											                      NULL, 
											                      fill,
											                      BillBoardSz,
											                      m_pGeometryCache,
											                      0,
																  objectName);
			  if(lod4Obj->m_meshLodLevel)
		         newLibObj->m_lodVisualList->AddTail(lod4Obj);


			  }
			  
			  
			  
			  
			  AfxMessageBox("Load successful");
			  SetCurrentDirectory(MainDir);

              delete m_pGeometryCache;
			  m_pGeometryCache = 0;


			  branchRefObj->m_seqLoad = tractSq;

			  return TRUE;
		   
	}
	else
	  AfxMessageBox("Load SPT failed");
	
	delete m_pSpeedTree;
	m_pSpeedTree = 0;
 	SetCurrentDirectory(MainDir);
	return FALSE;

}