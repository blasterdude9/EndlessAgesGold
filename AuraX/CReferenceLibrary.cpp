#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CReferenceLibrary.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CReferenceObj::CReferenceObj()
{

    m_referenceName     = "N/A";
    m_meshObject        = NULL;
	m_lodVisualList     = NULL;
	m_collisionModel    = NULL;
	m_popoutBox.max.x   = 0.0f;
	m_popoutBox.max.y   = 0.0f;
	m_popoutBox.max.z   = 0.0f;
	m_popoutBox.min.x   = 0.0f;
	m_popoutBox.min.y   = 0.0f;
	m_popoutBox.min.z   = 0.0f;
	m_animModule = NULL;
	m_faceCamera = FALSE;
	m_animationUpdatedThisRound = 0;
	m_extLockPoints     = NULL;
	m_lockPointCount    = 0;
	m_lodRange          = 1000.0;
	m_lodLevels         = 10;
	m_billboardSz       = 0.0f;
	m_seqLoad           = 0;
	m_skeletalOption    = NULL;
	m_skeletalOptionLive=NULL;
}

void CReferenceObj::SafeDelete()
{
    if(m_meshObject)
	{
       m_meshObject->SafeDelete();
	   delete m_meshObject;
	   m_meshObject = 0;
	}

	if(m_lodVisualList){
       m_lodVisualList->SafeDelete();
	   delete m_lodVisualList;
	   m_lodVisualList = 0;
	}

	if(m_collisionModel){
       m_collisionModel->SafeDelete();
       delete m_collisionModel;
       m_collisionModel = 0;
	}

    if(m_animModule)
	{
       m_animModule->SafeDelete();
	   delete m_animModule;
	   m_animModule = 0;
	}

	if(m_extLockPoints)
	{
       delete [] m_extLockPoints;
	   m_extLockPoints = 0;
	}

	if(m_skeletalOption)
	{
       m_skeletalOption->SafeDelete();
       delete m_skeletalOption;
	   m_skeletalOption = 0;
	}

	if(m_skeletalOptionLive)
	{
       m_skeletalOptionLive->SafeDelete();
       delete m_skeletalOptionLive;
	   m_skeletalOptionLive = 0;
	}
}

void CReferenceObj::CopyValues(CReferenceObj * stencil,LPDIRECT3DDEVICE9 d3dDevice)
{
    if(!stencil)
	    return;

	SafeDelete();//clears existing data on ptr
	m_referenceName					= stencil->m_referenceName;
	m_faceCamera					= stencil->m_faceCamera;
	m_seqLoad						= stencil->m_seqLoad;
	m_lockPointCount				= stencil->m_lockPointCount;
	m_lodRange						= stencil->m_lodRange;
	m_lodLevels						= stencil->m_lodLevels;
	m_billboardSz					= stencil->m_billboardSz;

	CopyMemory(&m_popoutBox,&stencil->m_popoutBox,sizeof(BNDBOX));

	if(stencil->m_meshObject)
       stencil->m_meshObject->Clone(&m_meshObject,d3dDevice);

	if(stencil->m_lodVisualList)
       stencil->m_lodVisualList->Clone(&m_lodVisualList,d3dDevice);

	if(stencil->m_collisionModel)
       stencil->m_collisionModel->Clone(&m_collisionModel,d3dDevice);

	if(stencil->m_animModule)
       stencil->m_animModule->Clone(&m_animModule);

	if(stencil->m_skeletalOption)
       stencil->m_skeletalOption->Clone(&m_skeletalOption,d3dDevice,NULL,1);

}

void CReferenceObj::InitFromPath(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     if(m_meshObject)
	 {
        m_meshObject->SafeDelete();
		delete m_meshObject;
		m_meshObject = 0;
	 }

     m_meshObject = new CEXMeshObj();
	 m_meshObject->List_materialObject = new CMaterialObject();
     float sightRange = 0.0f;
	 m_meshObject->LoadGeometryFromPlugin(fileName,&sightRange,g_pd3dDevice,NULL);
}

void CReferenceObj::GenerateLockPointMesh(D3DMATRIX & matrix,
										  D3DMATRIX objMatrix,
										  IDirect3DDevice9 * pd3dDevice,
										  float distToCam)
{
       
	
	    float lodOffset = InterpolateFloatsHiRes(
						                       1.0f,//float StartVec, 
						                       (float)m_lodLevels,//float EndVec,
					                           m_lodRange,//float NumOfStages, 
					                           distToCam);//float ThisStage)
		if(lodOffset > m_lodLevels)
		   lodOffset = (float)m_lodLevels;

		



		if(!m_meshObject->m_abVertexArray.m_vertexBuffer)
		    lodOffset = 1.0f;




	    m_meshObject->m_abVertexArray.m_vertexCount  = m_lockPointCount * 4;
	    m_meshObject->m_abVertexArray.m_indecieCount = m_lockPointCount * 6;
		m_meshObject->m_abVertexArray.m_advancedFixedMode = 1;

		m_meshObject->m_abVertexArray.m_vertexCount = m_meshObject->m_abVertexArray.m_vertexCount;
		m_meshObject->m_abVertexArray.m_indecieCount = m_meshObject->m_abVertexArray.m_indecieCount;
         
		if(!m_meshObject->m_abVertexArray.m_vertexBuffer)
		{
         if(m_meshObject->m_abVertexArray.m_vertexArray1)
		    delete [] m_meshObject->m_abVertexArray.m_vertexArray1;
		 m_meshObject->m_abVertexArray.m_vertexArray1 = 0;

		 if(m_meshObject->m_abVertexArray.m_indecieArray)
		    delete [] m_meshObject->m_abVertexArray.m_indecieArray;
		 m_meshObject->m_abVertexArray.m_indecieArray = 0;


		 m_meshObject->m_abVertexArray.m_vertexArray1 = new ABVERTEX1L[m_meshObject->m_abVertexArray.m_vertexCount];
	     ZeroMemory(m_meshObject->m_abVertexArray.m_vertexArray1, sizeof(ABVERTEX1L) * m_meshObject->m_abVertexArray.m_vertexCount);
	     m_meshObject->m_abVertexArray.m_indecieArray = new WORD[m_meshObject->m_abVertexArray.m_indecieCount];
	     ZeroMemory(m_meshObject->m_abVertexArray.m_indecieArray, sizeof(WORD) * m_meshObject->m_abVertexArray.m_indecieCount);
		

		 for(int vertexBuild=0;vertexBuild<m_lockPointCount;vertexBuild++)
		 {
            m_meshObject->m_abVertexArray.m_indecieArray[vertexBuild*6+0] = vertexBuild*4+3;
	        m_meshObject->m_abVertexArray.m_indecieArray[vertexBuild*6+1] = vertexBuild*4+0;
	        m_meshObject->m_abVertexArray.m_indecieArray[vertexBuild*6+2] = vertexBuild*4+1;
	        m_meshObject->m_abVertexArray.m_indecieArray[vertexBuild*6+3] = vertexBuild*4+3;
	        m_meshObject->m_abVertexArray.m_indecieArray[vertexBuild*6+4] = vertexBuild*4+1;
	        m_meshObject->m_abVertexArray.m_indecieArray[vertexBuild*6+5] = vertexBuild*4+2;
	     }

		 /*m_meshObject->m_abVertexArray.ReinitializeDxVbuffers(pd3dDevice,
												          D3DPOOL_MANAGED,
												          0,
												          0,
												          1);*/
		 //m_meshObject->m_abVertexArray.ReinitializeDxVbuffersAdvanced(pd3dDevice,D3DPOOL_MANAGED,0,0,0);  
		 int datasize = m_meshObject->m_abVertexArray.m_indecieCount * sizeof(WORD);
		 if(pd3dDevice->CreateIndexBuffer(datasize,
                                          D3DUSAGE_WRITEONLY,
                                          D3DFMT_INDEX16,
                                          D3DPOOL_MANAGED,//D3DPOOL_MANAGED  ,
                                          &m_meshObject->m_abVertexArray.m_indicieBuffer,
										  NULL)==0)
		  {
			 
			 WORD * indecieDxBuffer = NULL;
             if(m_meshObject->m_abVertexArray.m_indicieBuffer->Lock(0,0,(VOID**)&indecieDxBuffer, 0 )==0)
			 {
                CopyMemory(indecieDxBuffer,m_meshObject->m_abVertexArray.m_indecieArray,datasize);
				m_meshObject->m_abVertexArray.m_indicieBuffer->Unlock();
			 }
		  }
		 
		 if(pd3dDevice->CreateVertexBuffer( m_meshObject->m_abVertexArray.m_vertexCount*sizeof(ABVERTEX1La),
                                            D3DUSAGE_WRITEONLY, 
										    0,//(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2),
                                            D3DPOOL_MANAGED ,
										    &m_meshObject->m_abVertexArray.m_vertexBuffer,
										    NULL)!=0)
										    return;
		}

		ABVERTEX1La* pVertices1;
	    //int datasize = m_meshObject->m_abVertexArray.m_vertexCount*sizeof(ABVERTEX1La);
        if(m_meshObject->m_abVertexArray.m_vertexBuffer->Lock( 0, 0,(VOID**)&pVertices1, D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK  )==0)
	    {//lock buffer
		    D3DVECTOR one,two,three,four,tempP,normal;
			int tempOffset = 0;
			one.x = m_billboardSz;
			one.y = m_billboardSz;
			one.z = 0.0f;

            tempP.x = (matrix.m[0][0]*one.x)+(matrix.m[1][0]*one.y)+(matrix.m[2][0]*one.z);
            tempP.y = (matrix.m[0][1]*one.x)+(matrix.m[1][1]*one.y)+(matrix.m[2][1]*one.z);
            tempP.z = (matrix.m[0][2]*one.x)+(matrix.m[1][2]*one.y)+(matrix.m[2][2]*one.z);

            /*one.x = tempP.x;
            one.y = tempP.y;
            one.z = tempP.z;

            tempP.x = (objMatrix.m[0][0]*one.x)+(objMatrix.m[1][0]*one.y)+(objMatrix.m[2][0]*one.z);
            tempP.y = (objMatrix.m[0][1]*one.x)+(objMatrix.m[1][1]*one.y)+(objMatrix.m[2][1]*one.z);
            tempP.z = (objMatrix.m[0][2]*one.x)+(objMatrix.m[1][2]*one.y)+(objMatrix.m[2][2]*one.z);*/

            one.x = tempP.x;
            one.y = tempP.y;
            one.z = tempP.z;

			two.x = m_billboardSz;
			two.y = -m_billboardSz;
			two.z = 0.0f;

			tempP.x = (matrix.m[0][0]*two.x)+(matrix.m[1][0]*two.y)+(matrix.m[2][0]*two.z);
            tempP.y = (matrix.m[0][1]*two.x)+(matrix.m[1][1]*two.y)+(matrix.m[2][1]*two.z);
            tempP.z = (matrix.m[0][2]*two.x)+(matrix.m[1][2]*two.y)+(matrix.m[2][2]*two.z);

            two.x = tempP.x;
            two.y = tempP.y;
            two.z = tempP.z;

			/*tempP.x = (objMatrix.m[0][0]*two.x)+(objMatrix.m[1][0]*two.y)+(objMatrix.m[2][0]*two.z);
            tempP.y = (objMatrix.m[0][1]*two.x)+(objMatrix.m[1][1]*two.y)+(objMatrix.m[2][1]*two.z);
            tempP.z = (objMatrix.m[0][2]*two.x)+(objMatrix.m[1][2]*two.y)+(objMatrix.m[2][2]*two.z);

            two.x = tempP.x;
            two.y = tempP.y;
            two.z = tempP.z;*/

			three.x = -m_billboardSz;
			three.y = -m_billboardSz;
			three.z = 0.0f;

			tempP.x = (matrix.m[0][0]*three.x)+(matrix.m[1][0]*three.y)+(matrix.m[2][0]*three.z);
            tempP.y = (matrix.m[0][1]*three.x)+(matrix.m[1][1]*three.y)+(matrix.m[2][1]*three.z);
            tempP.z = (matrix.m[0][2]*three.x)+(matrix.m[1][2]*three.y)+(matrix.m[2][2]*three.z);

            three.x = tempP.x;
            three.y = tempP.y;
            three.z = tempP.z;

			/*tempP.x = (objMatrix.m[0][0]*three.x)+(objMatrix.m[1][0]*three.y)+(objMatrix.m[2][0]*three.z);
            tempP.y = (objMatrix.m[0][1]*three.x)+(objMatrix.m[1][1]*three.y)+(objMatrix.m[2][1]*three.z);
            tempP.z = (objMatrix.m[0][2]*three.x)+(objMatrix.m[1][2]*three.y)+(objMatrix.m[2][2]*three.z);

            three.x = tempP.x;
            three.y = tempP.y;
            three.z = tempP.z;*/

			four.x = -m_billboardSz;
			four.y = m_billboardSz;
			four.z = 0.0f;

			tempP.x = (matrix.m[0][0]*four.x)+(matrix.m[1][0]*four.y)+(matrix.m[2][0]*four.z);
            tempP.y = (matrix.m[0][1]*four.x)+(matrix.m[1][1]*four.y)+(matrix.m[2][1]*four.z);
            tempP.z = (matrix.m[0][2]*four.x)+(matrix.m[1][2]*four.y)+(matrix.m[2][2]*four.z);

            four.x = tempP.x;
            four.y = tempP.y;
            four.z = tempP.z;
			
            /*tempP.x = (objMatrix.m[0][0]*four.x)+(objMatrix.m[1][0]*four.y)+(objMatrix.m[2][0]*four.z);
            tempP.y = (objMatrix.m[0][1]*four.x)+(objMatrix.m[1][1]*four.y)+(objMatrix.m[2][1]*four.z);
            tempP.z = (objMatrix.m[0][2]*four.x)+(objMatrix.m[1][2]*four.y)+(objMatrix.m[2][2]*four.z);

            four.x = tempP.x;
            four.y = tempP.y;
            four.z = tempP.z;*/

			normal.x = 0.0f;
	        normal.y = 0.0f;
	        normal.z = -1.0f;

			tempP.x = (matrix.m[0][0]*normal.x)+(matrix.m[1][0]*normal.y)+(matrix.m[2][0]*normal.z);
            tempP.y = (matrix.m[0][1]*normal.x)+(matrix.m[1][1]*normal.y)+(matrix.m[2][1]*normal.z);
            tempP.z = (matrix.m[0][2]*normal.x)+(matrix.m[1][2]*normal.y)+(matrix.m[2][2]*normal.z);

            normal.x = tempP.x;
            normal.y = tempP.y;
            normal.z = tempP.z;

			/*tempP.x = (objMatrix.m[0][0]*normal.x)+(objMatrix.m[1][0]*normal.y)+(objMatrix.m[2][0]*normal.z);
            tempP.y = (objMatrix.m[0][1]*normal.x)+(objMatrix.m[1][1]*normal.y)+(objMatrix.m[2][1]*normal.z);
            tempP.z = (objMatrix.m[0][2]*normal.x)+(objMatrix.m[1][2]*normal.y)+(objMatrix.m[2][2]*normal.z);

            normal.x = tempP.x;
            normal.y = tempP.y;
            normal.z = tempP.z;*/
			
			//int lockTemp = 0;
			//float offsetLoop = invPercentage * ((float)m_lockPointCount);
            int loopOffset = 1;
            int skipVal = m_lockPointCount;
			int skipTrace = 0;
			
			int castSw = (int)lodOffset;
			switch(castSw)
            {
                case 2:
                  skipVal = 6;
				break;
				case 3:
                  skipVal = 5;
				break;
				case 4:
                  skipVal = 4;
				break;
				case 5:
                  skipVal = 3;
				break;
				case 6:
                  skipVal = 2;
				break;
				case 7:
                  skipVal = 1;
				break;
				case 8:
                  skipVal = 1;
				  loopOffset = 2;
				break;
				case 9:
                  skipVal = 1;
				  loopOffset = 3;
				break;
				case 10:
                  skipVal = 1;
				  loopOffset = 4;
				break;
				case 11:
                  skipVal = 1;
				  loopOffset = 5;
				break;


			}


			tempOffset=0;
			m_meshObject->m_abVertexArray.m_indecieCount = 0;
			for(int vertexBuild=0;vertexBuild<m_lockPointCount;vertexBuild = vertexBuild + loopOffset)
		    {

				if(skipTrace >= skipVal)
				{
				   skipTrace = 0;
				   continue;
				}
			//tempOffset = vertexBuild*4;
			//lockTemp = vertexBuild;
			//get lock points
			tempP.x = (objMatrix.m[0][0]*m_extLockPoints[vertexBuild].x)+(objMatrix.m[1][0]*m_extLockPoints[vertexBuild].y)+(objMatrix.m[2][0]*m_extLockPoints[vertexBuild].z)+(objMatrix.m[3][0]);
            tempP.y = (objMatrix.m[0][1]*m_extLockPoints[vertexBuild].x)+(objMatrix.m[1][1]*m_extLockPoints[vertexBuild].y)+(objMatrix.m[2][1]*m_extLockPoints[vertexBuild].z)+(objMatrix.m[3][1]);
            tempP.z = (objMatrix.m[0][2]*m_extLockPoints[vertexBuild].x)+(objMatrix.m[1][2]*m_extLockPoints[vertexBuild].y)+(objMatrix.m[2][2]*m_extLockPoints[vertexBuild].z)+(objMatrix.m[3][2]);
		    //diffuse
			pVertices1[tempOffset].diffuse = m_extLockPoints[vertexBuild].diffuse;
			//vertex 1 upper left
	        pVertices1[tempOffset].x  = tempP.x + one.x;
	        pVertices1[tempOffset].y  = tempP.y + one.y;
	        pVertices1[tempOffset].z  = tempP.z + one.z;
	        pVertices1[tempOffset].tx0.tu = 1.0f;//1.0f
	        pVertices1[tempOffset].tx0.tv = 0.0f;//0.0f
	        pVertices1[tempOffset].nx = normal.x;
	        pVertices1[tempOffset].ny = normal.y;
	        pVertices1[tempOffset++].nz = normal.z;
	        //vertex 2 bottom left
			pVertices1[tempOffset].diffuse = m_extLockPoints[vertexBuild].diffuse;
	        pVertices1[tempOffset].x  = tempP.x + two.x;
	        pVertices1[tempOffset].y  = tempP.y + two.y;
	        pVertices1[tempOffset].z  = tempP.z + two.z;
	        pVertices1[tempOffset].tx0.tu = 1.0f;//0.0f
	        pVertices1[tempOffset].tx0.tv = 1.0f;//0.0f
	        pVertices1[tempOffset].nx = normal.x;
	        pVertices1[tempOffset].ny = normal.y;
	        pVertices1[tempOffset++].nz = normal.z;
	        //vertex 3 bottom right
			pVertices1[tempOffset].diffuse = m_extLockPoints[vertexBuild].diffuse;
	        pVertices1[tempOffset].x  = tempP.x + three.x;
	        pVertices1[tempOffset].y  = tempP.y + three.y;
	        pVertices1[tempOffset].z  = tempP.z + three.z;
	        pVertices1[tempOffset].tx0.tu = 0.0f;//0.0f
	        pVertices1[tempOffset].tx0.tv = 1.0f;//1.0f
	        pVertices1[tempOffset].nx = normal.x;
	        pVertices1[tempOffset].ny = normal.y;
	        pVertices1[tempOffset++].nz = normal.z;
	        //vertex 4 upper right
			pVertices1[tempOffset].diffuse = m_extLockPoints[vertexBuild].diffuse;
	        pVertices1[tempOffset].x  = tempP.x + four.x;
	        pVertices1[tempOffset].y  = tempP.y + four.y;
	        pVertices1[tempOffset].z  = tempP.z + four.z;
	        pVertices1[tempOffset].tx0.tu = 0.0f;//1.0f
	        pVertices1[tempOffset].tx0.tv = 0.0f;//1.0f
	        pVertices1[tempOffset].nx = normal.x;
	        pVertices1[tempOffset].ny = normal.y;
	        pVertices1[tempOffset++].nz = normal.z;
	        //indecies
	        //no facing
		
			m_meshObject->m_abVertexArray.m_indecieCount += 6;
			skipTrace++;
		}
                            
			m_meshObject->m_abVertexArray.m_vertexBuffer->Unlock();
	    }//end lock beffer
}
//----------------------------------------
//----------------------------------------
void CReferenceObj::GenerateLockPointMeshGPU(D3DMATRIX & matrix,
										     D3DMATRIX objMatrix,
										     IDirect3DDevice9 * pd3dDevice,
										     float distToCam,
											 D3DVECTOR & constOne,
											 D3DVECTOR & constTwo,
											 D3DVECTOR & constThree,
											 D3DVECTOR & constFour,
											 int ovrdLOD)
{
       
	
	    float lodOffset = InterpolateFloatsHiRes(
						                       1.0f,//float StartVec, 
						                       (float)m_lodLevels,//float EndVec,
					                           m_lodRange,//float NumOfStages, 
					                           distToCam);//float ThisStage)
		if(lodOffset > m_lodLevels)
		   lodOffset = (float)m_lodLevels;

		
		if(!m_meshObject->m_abVertexArray.m_vertexBuffer)
		    lodOffset = 1.0f;


		lodOffset = (float)ovrdLOD;

		int loopOffset = 1;
            int skipVal = m_lockPointCount;
			int skipTrace = 0;
			
			int castSw = (int)lodOffset;
			switch(castSw)
            {
                case 2:
                  skipVal = 6;
				break;
				case 3:
                  skipVal = 5;
				break;
				case 4:
                  skipVal = 4;
				break;
				case 5:
                  skipVal = 3;
				break;
				case 6:
                  skipVal = 2;
				break;
				case 7:
                  skipVal = 1;
				break;
				case 8:
                  skipVal = 1;
				  loopOffset = 2;
				break;
				case 9:
                  skipVal = 1;
				  loopOffset = 3;
				break;
				case 10:
                  skipVal = 1;
				  loopOffset = 4;
				break;
				case 11:
                  skipVal = 1;
				  loopOffset = 5;
				break;


			}

	    m_meshObject->m_abVertexArray.m_vertexCount  = m_lockPointCount * 4;
	    m_meshObject->m_abVertexArray.m_indecieCount = m_lockPointCount * 6;
		m_meshObject->m_abVertexArray.m_advancedFixedMode = 1;
		m_meshObject->m_abVertexArray.m_uvCount = 2;


		    D3DVECTOR tempP,normal;
			int tempOffset = 0;
			constOne.x = m_billboardSz;
			constOne.y = m_billboardSz;
			constOne.z = 0.0f;

            tempP.x = (matrix.m[0][0]*constOne.x)+(matrix.m[1][0]*constOne.y)+(matrix.m[2][0]*constOne.z);
            tempP.y = (matrix.m[0][1]*constOne.x)+(matrix.m[1][1]*constOne.y)+(matrix.m[2][1]*constOne.z);
            tempP.z = (matrix.m[0][2]*constOne.x)+(matrix.m[1][2]*constOne.y)+(matrix.m[2][2]*constOne.z);

            constOne.x = tempP.x;
            constOne.y = tempP.y;
            constOne.z = tempP.z;

			constTwo.x = m_billboardSz;
			constTwo.y = -m_billboardSz;
			constTwo.z = 0.0f;

			tempP.x = (matrix.m[0][0]*constTwo.x)+(matrix.m[1][0]*constTwo.y)+(matrix.m[2][0]*constTwo.z);
            tempP.y = (matrix.m[0][1]*constTwo.x)+(matrix.m[1][1]*constTwo.y)+(matrix.m[2][1]*constTwo.z);
            tempP.z = (matrix.m[0][2]*constTwo.x)+(matrix.m[1][2]*constTwo.y)+(matrix.m[2][2]*constTwo.z);

            constTwo.x = tempP.x;
            constTwo.y = tempP.y;
            constTwo.z = tempP.z;

			constThree.x = -m_billboardSz;
			constThree.y = -m_billboardSz;
			constThree.z = 0.0f;

			tempP.x = (matrix.m[0][0]*constThree.x)+(matrix.m[1][0]*constThree.y)+(matrix.m[2][0]*constThree.z);
            tempP.y = (matrix.m[0][1]*constThree.x)+(matrix.m[1][1]*constThree.y)+(matrix.m[2][1]*constThree.z);
            tempP.z = (matrix.m[0][2]*constThree.x)+(matrix.m[1][2]*constThree.y)+(matrix.m[2][2]*constThree.z);

            constThree.x = tempP.x;
            constThree.y = tempP.y;
            constThree.z = tempP.z;

			constFour.x = -m_billboardSz;
			constFour.y = m_billboardSz;
			constFour.z = 0.0f;

			tempP.x = (matrix.m[0][0]*constFour.x)+(matrix.m[1][0]*constFour.y)+(matrix.m[2][0]*constFour.z);
            tempP.y = (matrix.m[0][1]*constFour.x)+(matrix.m[1][1]*constFour.y)+(matrix.m[2][1]*constFour.z);
            tempP.z = (matrix.m[0][2]*constFour.x)+(matrix.m[1][2]*constFour.y)+(matrix.m[2][2]*constFour.z);

            constFour.x = tempP.x;
            constFour.y = tempP.y;
            constFour.z = tempP.z;

			normal.x = 0.0f;
	        normal.y = 0.0f;
	        normal.z = -1.0f;

			tempP.x = (matrix.m[0][0]*normal.x)+(matrix.m[1][0]*normal.y)+(matrix.m[2][0]*normal.z);
            tempP.y = (matrix.m[0][1]*normal.x)+(matrix.m[1][1]*normal.y)+(matrix.m[2][1]*normal.z);
            tempP.z = (matrix.m[0][2]*normal.x)+(matrix.m[1][2]*normal.y)+(matrix.m[2][2]*normal.z);

            normal.x = tempP.x;
            normal.y = tempP.y;
            normal.z = tempP.z;

	     
		if(!m_meshObject->m_abVertexArray.m_vertexBuffer)
		{//needs initializing
         if(m_meshObject->m_abVertexArray.m_vertexArray2)
		    delete [] m_meshObject->m_abVertexArray.m_vertexArray2;
		 m_meshObject->m_abVertexArray.m_vertexArray2 = 0;

		 if(m_meshObject->m_abVertexArray.m_indecieArray)
		    delete [] m_meshObject->m_abVertexArray.m_indecieArray;
		 m_meshObject->m_abVertexArray.m_indecieArray = 0;


		 m_meshObject->m_abVertexArray.m_vertexArray2 = new ABVERTEX2L[m_meshObject->m_abVertexArray.m_vertexCount];
	     ZeroMemory(m_meshObject->m_abVertexArray.m_vertexArray2, sizeof(ABVERTEX2L) * m_meshObject->m_abVertexArray.m_vertexCount);
	     m_meshObject->m_abVertexArray.m_indecieArray = new WORD[m_meshObject->m_abVertexArray.m_indecieCount];
	     ZeroMemory(m_meshObject->m_abVertexArray.m_indecieArray, sizeof(WORD) * m_meshObject->m_abVertexArray.m_indecieCount);
		

		 for(int vertexBuild=0;vertexBuild<m_lockPointCount;vertexBuild++)
		 {
            m_meshObject->m_abVertexArray.m_indecieArray[vertexBuild*6+0] = vertexBuild*4+3;
	        m_meshObject->m_abVertexArray.m_indecieArray[vertexBuild*6+1] = vertexBuild*4+0;
	        m_meshObject->m_abVertexArray.m_indecieArray[vertexBuild*6+2] = vertexBuild*4+1;
	        m_meshObject->m_abVertexArray.m_indecieArray[vertexBuild*6+3] = vertexBuild*4+3;
	        m_meshObject->m_abVertexArray.m_indecieArray[vertexBuild*6+4] = vertexBuild*4+1;
	        m_meshObject->m_abVertexArray.m_indecieArray[vertexBuild*6+5] = vertexBuild*4+2;
	     }

		 int datasize = m_meshObject->m_abVertexArray.m_indecieCount * sizeof(WORD);
		 if(pd3dDevice->CreateIndexBuffer(datasize,
                                          D3DUSAGE_WRITEONLY,
                                          D3DFMT_INDEX16,
                                          D3DPOOL_MANAGED,//D3DPOOL_MANAGED  ,
                                          &m_meshObject->m_abVertexArray.m_indicieBuffer,
										  NULL)==0)
		  {
			 
			 WORD * indecieDxBuffer = NULL;
             if(m_meshObject->m_abVertexArray.m_indicieBuffer->Lock(0,0,(VOID**)&indecieDxBuffer, 0 )==0)
			 {
                CopyMemory(indecieDxBuffer,m_meshObject->m_abVertexArray.m_indecieArray,datasize);
				m_meshObject->m_abVertexArray.m_indicieBuffer->Unlock();
			 }
		  }
		 
		 if(pd3dDevice->CreateVertexBuffer( m_meshObject->m_abVertexArray.m_vertexCount*sizeof(ABVERTEX2La),
                                            D3DUSAGE_WRITEONLY, 
										    0,//(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2),
                                            D3DPOOL_MANAGED ,
										    &m_meshObject->m_abVertexArray.m_vertexBuffer,
										    NULL)==0)
		 {//initialize lock points
           ABVERTEX2La* pVertices1;
	       int datasize = m_meshObject->m_abVertexArray.m_vertexCount*sizeof(ABVERTEX2La);
           if(m_meshObject->m_abVertexArray.m_vertexBuffer->Lock( 0, 0,(VOID**)&pVertices1, D3DLOCK_DISCARD | D3DLOCK_NOSYSLOCK  )==0)
	       {//lock buffer 
			tempOffset=0;
			m_meshObject->m_abVertexArray.m_indecieCount = 0;
			for(int vertexBuild=0;vertexBuild<m_lockPointCount;vertexBuild = vertexBuild + loopOffset)
		    {//BEGIN LOOP
				if(skipTrace >= skipVal)
				{
				   skipTrace = 0;
				   continue;
				}
		    //diffuse
			pVertices1[tempOffset].diffuse = m_extLockPoints[vertexBuild].diffuse;
			//vertex 1 upper left
	        /*pVertices1[tempOffset].x  = m_extLockPoints[vertexBuild].x + constOne.x;
	        pVertices1[tempOffset].y  = m_extLockPoints[vertexBuild].y + constOne.y;
	        pVertices1[tempOffset].z  = m_extLockPoints[vertexBuild].z + constOne.z;//*/
			pVertices1[tempOffset].x  = m_extLockPoints[vertexBuild].x;
	        pVertices1[tempOffset].y  = m_extLockPoints[vertexBuild].y;
	        pVertices1[tempOffset].z  = m_extLockPoints[vertexBuild].z;//*/
	        pVertices1[tempOffset].tx0.tu = 1.0f;//1.0f
	        pVertices1[tempOffset].tx0.tv = 0.0f;//0.0f
			pVertices1[tempOffset].tx1.tu = 50.0f;//misc const ref
	        pVertices1[tempOffset].tx1.tv = 0.0f;//0.0f
	        pVertices1[tempOffset].nx = normal.x;
	        pVertices1[tempOffset].ny = normal.y;
	        pVertices1[tempOffset++].nz = normal.z;
	        //vertex 2 bottom left
			pVertices1[tempOffset].diffuse = m_extLockPoints[vertexBuild].diffuse;
	        /*pVertices1[tempOffset].x  = m_extLockPoints[vertexBuild].x + constTwo.x;
	        pVertices1[tempOffset].y  = m_extLockPoints[vertexBuild].y + constTwo.y;
	        pVertices1[tempOffset].z  = m_extLockPoints[vertexBuild].z + constTwo.z;//*/
			pVertices1[tempOffset].x  = m_extLockPoints[vertexBuild].x;
	        pVertices1[tempOffset].y  = m_extLockPoints[vertexBuild].y;
	        pVertices1[tempOffset].z  = m_extLockPoints[vertexBuild].z;//*/
	        pVertices1[tempOffset].tx0.tu = 1.0f;//0.0f
	        pVertices1[tempOffset].tx0.tv = 1.0f;//0.0f
			pVertices1[tempOffset].tx1.tu = 51.0f;//misc const ref
	        pVertices1[tempOffset].tx1.tv = 0.0f;//0.0f
	        pVertices1[tempOffset].nx = normal.x;
	        pVertices1[tempOffset].ny = normal.y;
	        pVertices1[tempOffset++].nz = normal.z;
	        //vertex 3 bottom right
			pVertices1[tempOffset].diffuse = m_extLockPoints[vertexBuild].diffuse;
	        /*pVertices1[tempOffset].x  = m_extLockPoints[vertexBuild].x + constThree.x;
	        pVertices1[tempOffset].y  = m_extLockPoints[vertexBuild].y + constThree.y;
	        pVertices1[tempOffset].z  = m_extLockPoints[vertexBuild].z + constThree.z;//*/
			pVertices1[tempOffset].x  = m_extLockPoints[vertexBuild].x;
	        pVertices1[tempOffset].y  = m_extLockPoints[vertexBuild].y;
	        pVertices1[tempOffset].z  = m_extLockPoints[vertexBuild].z;//*/
	        pVertices1[tempOffset].tx0.tu = 0.0f;//0.0f
	        pVertices1[tempOffset].tx0.tv = 1.0f;//1.0f
			pVertices1[tempOffset].tx1.tu = 52.0f;//misc const ref
	        pVertices1[tempOffset].tx1.tv = 0.0f;//0.0f
	        pVertices1[tempOffset].nx = normal.x;
	        pVertices1[tempOffset].ny = normal.y;
	        pVertices1[tempOffset++].nz = normal.z;
	        //vertex 4 upper right
			pVertices1[tempOffset].diffuse = m_extLockPoints[vertexBuild].diffuse;
	        /*pVertices1[tempOffset].x  = m_extLockPoints[vertexBuild].x + constFour.x;
	        pVertices1[tempOffset].y  = m_extLockPoints[vertexBuild].y + constFour.y;
	        pVertices1[tempOffset].z  = m_extLockPoints[vertexBuild].z + constFour.z;//*/
			pVertices1[tempOffset].x  = m_extLockPoints[vertexBuild].x;
	        pVertices1[tempOffset].y  = m_extLockPoints[vertexBuild].y;
	        pVertices1[tempOffset].z  = m_extLockPoints[vertexBuild].z;//*/
	        pVertices1[tempOffset].tx0.tu = 0.0f;//1.0f
	        pVertices1[tempOffset].tx0.tv = 0.0f;//1.0f
			pVertices1[tempOffset].tx1.tu = 53.0f;//misc const ref
	        pVertices1[tempOffset].tx1.tv = 0.0f;//0.0f
	        pVertices1[tempOffset].nx = normal.x;
	        pVertices1[tempOffset].ny = normal.y;
	        pVertices1[tempOffset++].nz = normal.z;
		
			m_meshObject->m_abVertexArray.m_indecieCount += 6;
			skipTrace++;
			}//END LOOP
		    m_meshObject->m_abVertexArray.m_vertexBuffer->Unlock();
		   }//END LOCK BUFFER
		 }//end initialize lock points
		}//end needs initializing	    
}
//----------------------------------------
//----------------------------------------
void CReferenceObj::GenerateGPUContants(D3DMATRIX & matrix,
									    IDirect3DDevice9 * pd3dDevice,
									    float & distToCam,
									    D3DVECTOR & constOne,
									    D3DVECTOR & constTwo,
										D3DVECTOR & constThree,
									    D3DVECTOR & constFour,
										D3DVECTOR & constNorm,
										float & maxX,
										float & maxY,
										WORD & vCount)
{

	        
            float scalar = 1.0f;
			float billboardX = m_billboardSz;
			float billboardY = m_billboardSz;
			float billboardYground = m_billboardSz;
			if(vCount > 4)
			      scalar = InterpolateFloatsHiRes(
						                          1.0f,//float StartVec, 
						                          2.5f,//float EndVec,
					                              m_lodRange,//float NumOfStages, 
					                              distToCam);//float ThisStage)
			else
			{
                 billboardX = maxX;
                 billboardY = maxY;
			     billboardYground = 0.0f;
			}
   
			if(scalar < 1.0f)
			   scalar = 1.0f;

		

			//upper right
		    D3DVECTOR tempP;
			constOne.x = billboardX * scalar;
			constOne.y = billboardY * scalar;
			constOne.z = 0.0f;

            tempP.x = (matrix.m[0][0]*constOne.x)+(matrix.m[1][0]*constOne.y)+(matrix.m[2][0]*constOne.z);
            tempP.y = (matrix.m[0][1]*constOne.x)+(matrix.m[1][1]*constOne.y)+(matrix.m[2][1]*constOne.z);
            tempP.z = (matrix.m[0][2]*constOne.x)+(matrix.m[1][2]*constOne.y)+(matrix.m[2][2]*constOne.z);

            constOne.x = tempP.x;
            constOne.y = tempP.y;
            constOne.z = tempP.z;

			//lower right
			constTwo.x = billboardX * scalar;
			constTwo.y = -billboardYground * scalar;
			constTwo.z = 0.0f;

			tempP.x = (matrix.m[0][0]*constTwo.x)+(matrix.m[1][0]*constTwo.y)+(matrix.m[2][0]*constTwo.z);
            tempP.y = (matrix.m[0][1]*constTwo.x)+(matrix.m[1][1]*constTwo.y)+(matrix.m[2][1]*constTwo.z);
            tempP.z = (matrix.m[0][2]*constTwo.x)+(matrix.m[1][2]*constTwo.y)+(matrix.m[2][2]*constTwo.z);

            constTwo.x = tempP.x;
            constTwo.y = tempP.y;
            constTwo.z = tempP.z;

			//lower left
			constThree.x = -billboardX * scalar;
			constThree.y = -billboardYground * scalar;
			constThree.z = 0.0f;

			tempP.x = (matrix.m[0][0]*constThree.x)+(matrix.m[1][0]*constThree.y)+(matrix.m[2][0]*constThree.z);
            tempP.y = (matrix.m[0][1]*constThree.x)+(matrix.m[1][1]*constThree.y)+(matrix.m[2][1]*constThree.z);
            tempP.z = (matrix.m[0][2]*constThree.x)+(matrix.m[1][2]*constThree.y)+(matrix.m[2][2]*constThree.z);

            constThree.x = tempP.x;
            constThree.y = tempP.y;
            constThree.z = tempP.z;


			//upper left
			constFour.x = -billboardX * scalar;
			constFour.y = billboardY * scalar;
			constFour.z = 0.0f;

			tempP.x = (matrix.m[0][0]*constFour.x)+(matrix.m[1][0]*constFour.y)+(matrix.m[2][0]*constFour.z);
            tempP.y = (matrix.m[0][1]*constFour.x)+(matrix.m[1][1]*constFour.y)+(matrix.m[2][1]*constFour.z);
            tempP.z = (matrix.m[0][2]*constFour.x)+(matrix.m[1][2]*constFour.y)+(matrix.m[2][2]*constFour.z);

            constFour.x = tempP.x;
            constFour.y = tempP.y;
            constFour.z = tempP.z;

			constNorm.x = 0.0f;
	        constNorm.y = 0.0f;
	        constNorm.z = -1.0f;

			tempP.x = (matrix.m[0][0]*constNorm.x)+(matrix.m[1][0]*constNorm.y)+(matrix.m[2][0]*constNorm.z);
            tempP.y = (matrix.m[0][1]*constNorm.x)+(matrix.m[1][1]*constNorm.y)+(matrix.m[2][1]*constNorm.z);
            tempP.z = (matrix.m[0][2]*constNorm.x)+(matrix.m[1][2]*constNorm.y)+(matrix.m[2][2]*constNorm.z);

            constNorm.x = tempP.x;
            constNorm.y = tempP.y;
            constNorm.z = tempP.z;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Interpolation between animations BABY!! Imediate mode style-----------//
float CReferenceObj::InterpolateFloatsHiRes(
						            float StartVec, 
						            float EndVec,
					                float NumOfStages, 
					                float ThisStage)
{
 float propfactor;
 if (NumOfStages <= 0)
  NumOfStages = 1;
  propfactor = ThisStage/NumOfStages;
  float StageVec = StartVec + propfactor * (EndVec - StartVec);

 return StageVec;
}
//-----------------------------------------------------//
//-----------------------------------------------------//
void CReferenceObj::D3DMath_MatrixInvert( D3DMATRIX& q, D3DMATRIX& a )
{
    FLOAT fDetInv = 1.0f / ( a._11 * ( a._22 * a._33 - a._23 * a._32 ) -
                             a._12 * ( a._21 * a._33 - a._23 * a._31 ) +
                             a._13 * ( a._21 * a._32 - a._22 * a._31 ) );

    q._11 =  fDetInv * ( a._22 * a._33 - a._23 * a._32 );
    q._12 = -fDetInv * ( a._12 * a._33 - a._13 * a._32 );
    q._13 =  fDetInv * ( a._12 * a._23 - a._13 * a._22 );
    q._14 = 0.0f;

    q._21 = -fDetInv * ( a._21 * a._33 - a._23 * a._31 );
    q._22 =  fDetInv * ( a._11 * a._33 - a._13 * a._31 );
    q._23 = -fDetInv * ( a._11 * a._23 - a._13 * a._21 );
    q._24 = 0.0f;

    q._31 =  fDetInv * ( a._21 * a._32 - a._22 * a._31 );
    q._32 = -fDetInv * ( a._11 * a._32 - a._12 * a._31 );
    q._33 =  fDetInv * ( a._11 * a._22 - a._12 * a._21 );
    q._34 = 0.0f;

    q._41 = -( a._41 * q._11 + a._42 * q._21 + a._43 * q._31 );
    q._42 = -( a._41 * q._12 + a._42 * q._22 + a._43 * q._32 );
    q._43 = -( a._41 * q._13 + a._42 * q._23 + a._43 * q._33 );
    q._44 = 1.0f;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
void CReferenceObj::fastMatrixMultiply(D3DMATRIX & R,D3DMATRIX A, D3DMATRIX B)
{
	D3DMATRIX result;
	register D3DMATRIX *result_pointer = &result;
	register D3DMATRIX *A_pointer = &A;
	register D3DMATRIX *B_pointer = &B;

	
	result_pointer->_11 = 	A_pointer->_11 * B_pointer->_11 + 
						        A_pointer->_12 *	B_pointer->_21 + 
							    A_pointer->_13 * B_pointer->_31 + 
								A_pointer->_14 * B_pointer->_41;
	result_pointer->_12 = 	A_pointer->_11 * B_pointer->_12 + 
								A_pointer->_12 *	B_pointer->_22 + 
								A_pointer->_13 * B_pointer->_32 + 
								A_pointer->_14 * B_pointer->_42;
	result_pointer->_13 = 	A_pointer->_11 * B_pointer->_13 + 
								A_pointer->_12 *	B_pointer->_23 + 
								A_pointer->_13 * B_pointer->_33 + 
								A_pointer->_14 * B_pointer->_43;
	result_pointer->_14 = 	A_pointer->_11 * B_pointer->_14 + 
								A_pointer->_12 *	B_pointer->_24 + 
								A_pointer->_13 * B_pointer->_34 + 
								A_pointer->_14 * B_pointer->_44;
	result_pointer->_21 = 	A_pointer->_21 * B_pointer->_11 + 
								A_pointer->_22 *	B_pointer->_21 + 
								A_pointer->_23 * B_pointer->_31 + 
								A_pointer->_24 * B_pointer->_41;
	result_pointer->_22 = 	A_pointer->_21 * B_pointer->_12 + 
								A_pointer->_22 *	B_pointer->_22 + 
								A_pointer->_23 * B_pointer->_32 + 
								A_pointer->_24 * B_pointer->_42;
	result_pointer->_23 = 	A_pointer->_21 * B_pointer->_13 + 
								A_pointer->_22 *	B_pointer->_23 + 
								A_pointer->_23 * B_pointer->_33 + 
								A_pointer->_24 * B_pointer->_43;
	result_pointer->_24 = 	A_pointer->_21 * B_pointer->_14 + 
								A_pointer->_22 *	B_pointer->_24 + 
								A_pointer->_23 * B_pointer->_34 + 
								A_pointer->_24 * B_pointer->_44;
	result_pointer->_31 = 	A_pointer->_31 * B_pointer->_11 + 
								A_pointer->_32 *	B_pointer->_21 + 
								A_pointer->_33 * B_pointer->_31 + 
								A_pointer->_34 * B_pointer->_41;
	result_pointer->_32 = 	A_pointer->_31 * B_pointer->_12 + 
								A_pointer->_32 *	B_pointer->_22 + 
								A_pointer->_33 * B_pointer->_32 + 
								A_pointer->_34 * B_pointer->_42;
	result_pointer->_33 = 	A_pointer->_31 * B_pointer->_13 + 
								A_pointer->_32 *	B_pointer->_23 + 
								A_pointer->_33 * B_pointer->_33 + 
								A_pointer->_34 * B_pointer->_43;
	result_pointer->_34 = 	A_pointer->_31 * B_pointer->_14 + 
								A_pointer->_32 *	B_pointer->_24 + 
								A_pointer->_33 * B_pointer->_34 + 
								A_pointer->_34 * B_pointer->_44;
	result_pointer->_41 = 	A_pointer->_41 * B_pointer->_11 + 
								A_pointer->_42 *	B_pointer->_21 + 
								A_pointer->_43 * B_pointer->_31 + 
								A_pointer->_44 * B_pointer->_41;
	result_pointer->_42 = 	A_pointer->_41 * B_pointer->_12 + 
								A_pointer->_42 *	B_pointer->_22 + 
								A_pointer->_43 * B_pointer->_32 + 
								A_pointer->_44 * B_pointer->_42;
	result_pointer->_43 = 	A_pointer->_41 * B_pointer->_13 + 
								A_pointer->_42 *	B_pointer->_23 + 
								A_pointer->_43 * B_pointer->_33 + 
								A_pointer->_44 * B_pointer->_43;
	result_pointer->_44 = 	A_pointer->_41 * B_pointer->_14 + 
								A_pointer->_42 *	B_pointer->_24 + 
								A_pointer->_43 * B_pointer->_34 + 
								A_pointer->_44 * B_pointer->_44;
	R = result;
}

void CReferenceObj::InitCollisionObjectFromPath(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     m_collisionModel = new CEXMeshObj();
	 m_collisionModel->List_materialObject = new CMaterialObject();
     float sightRange = 0.0f;
	 m_collisionModel->LoadGeometryFromPlugin(fileName,&sightRange,g_pd3dDevice,NULL);
}

 BOOL CReferenceObj::UpdateLods()
{
   if(!m_lodVisualList)
      return FALSE;

      for(POSITION posLoc = m_lodVisualList->GetHeadPosition();posLoc != NULL;)
	  {
          CMeshLODObject * meshLod = (CMeshLODObject *)m_lodVisualList->GetNext(posLoc);
		  if(meshLod->m_meshLodLevel->List_materialObject)
		  delete meshLod->m_meshLodLevel->List_materialObject;
		  meshLod->m_meshLodLevel->List_materialObject = 0;
          m_meshObject->List_materialObject->Clone(&meshLod->m_meshLodLevel->List_materialObject);
		  
		  CopyMemory(meshLod->m_meshLodLevel->List_BlendMethod,m_meshObject->List_BlendMethod,(sizeof(int)*10));
		  CopyMemory(meshLod->m_meshLodLevel->List_textureSelect,m_meshObject->List_textureSelect,(sizeof(int)*10));
		  
		  meshLod->m_meshLodLevel->List_storedTextureFromFile[0] = m_meshObject->List_storedTextureFromFile[0];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[1] = m_meshObject->List_storedTextureFromFile[1];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[2] = m_meshObject->List_storedTextureFromFile[2];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[3] = m_meshObject->List_storedTextureFromFile[3];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[4] = m_meshObject->List_storedTextureFromFile[4];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[5] = m_meshObject->List_storedTextureFromFile[5];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[6] = m_meshObject->List_storedTextureFromFile[6];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[7] = m_meshObject->List_storedTextureFromFile[7];


	      meshLod->m_meshLodLevel->List_materialObject->Reset();
	  
	  }
	  return TRUE;
}


BOOL CReferenceObj::UpdateLods(BOOL filterTextureCarry)
{
   if(!m_lodVisualList)
      return FALSE;

      for(POSITION posLoc = m_lodVisualList->GetHeadPosition();posLoc != NULL;)
	  {
          CMeshLODObject * meshLod = (CMeshLODObject *)m_lodVisualList->GetNext(posLoc);
		  if(meshLod->m_meshLodLevel->List_materialObject)
		  delete meshLod->m_meshLodLevel->List_materialObject;
		  meshLod->m_meshLodLevel->List_materialObject = 0;
          m_meshObject->List_materialObject->Clone(&meshLod->m_meshLodLevel->List_materialObject);
		  
		  CopyMemory(meshLod->m_meshLodLevel->List_BlendMethod,m_meshObject->List_BlendMethod,(sizeof(int)*10));
		  CopyMemory(meshLod->m_meshLodLevel->List_textureSelect,m_meshObject->List_textureSelect,(sizeof(int)*10));
		  
		  if(!filterTextureCarry)
		  {

		  meshLod->m_meshLodLevel->List_storedTextureFromFile[0] = m_meshObject->List_storedTextureFromFile[0];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[1] = m_meshObject->List_storedTextureFromFile[1];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[2] = m_meshObject->List_storedTextureFromFile[2];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[3] = m_meshObject->List_storedTextureFromFile[3];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[4] = m_meshObject->List_storedTextureFromFile[4];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[5] = m_meshObject->List_storedTextureFromFile[5];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[6] = m_meshObject->List_storedTextureFromFile[6];
          meshLod->m_meshLodLevel->List_storedTextureFromFile[7] = m_meshObject->List_storedTextureFromFile[7];

		  }

	      meshLod->m_meshLodLevel->List_materialObject->Reset();
	  
	  }
	  return TRUE;
}


void CReferenceObj::Serialize( CArchive& ar )
{
BOOL reserved = 0;
CObject::Serialize( ar );
	if(ar.IsStoring() ){

	   ar.SetObjectSchema(5);
       ASSERT(ar.GetObjectSchema() == 5);

	   ar << m_referenceName
	      << m_meshObject
		  << m_lodVisualList
		  << m_collisionModel
		  << m_popoutBox.max.x
	      << m_popoutBox.max.y
	      << m_popoutBox.max.z
	      << m_popoutBox.min.x
	      << m_popoutBox.min.y
	      << m_popoutBox.min.z;

	   ar << m_animModule
		  << m_faceCamera
		  << m_lockPointCount
		  << m_lodRange
		  << m_lodLevels;

       ar << m_billboardSz;

	   for(int li=0;li<m_lockPointCount;li++)
	   {
           ar << m_extLockPoints[li].x
			  << m_extLockPoints[li].y
			  << m_extLockPoints[li].z
			  << m_extLockPoints[li].diffuse;
	   }

	   ar << m_seqLoad
		  << reserved
		  << reserved
		  << reserved;

	   ar << m_skeletalOption;

	}
	else
	{

       int version = ar.GetObjectSchema();

       ar >> m_referenceName
	      >> m_meshObject
		  >> m_lodVisualList
		  >> m_collisionModel
		  >> m_popoutBox.max.x
	      >> m_popoutBox.max.y
	      >> m_popoutBox.max.z
	      >> m_popoutBox.min.x
	      >> m_popoutBox.min.y
	      >> m_popoutBox.min.z;

	   m_extLockPoints = NULL;
	   if(version < 2)
	   {
          m_animModule = NULL;

	   }
       if(version > 1)
	   {
          ar >> m_animModule 
			 >> m_faceCamera
			 >> m_lockPointCount
			 >> m_lodRange
			 >> m_lodLevels;
	   }
	   if(version > 2)
	   {
		   ar >> m_billboardSz;
		   if(m_lockPointCount > 0)
		   {
		     m_extLockPoints = new Ext_lockpoints[m_lockPointCount];
	         for(int li=0;li<m_lockPointCount;li++)
			 {
	            ar >> m_extLockPoints[li].x
				   >> m_extLockPoints[li].y
				   >> m_extLockPoints[li].z
				   >> m_extLockPoints[li].diffuse;

			 }

		   }
	   }
	   m_seqLoad = 0;
       if(version > 3)
	   {
	   ar >> m_seqLoad
		  >> reserved
		  >> reserved
		  >> reserved;
	   }

	   if(version > 4)
	   {
          ar >> m_skeletalOption;
	   }


	}
}     

 CReferenceObj * CReferenceObjList::GetByIndex(int & index)
{
   POSITION posLoc = FindIndex(index);
   if(!posLoc)
	   return NULL;
       return (CReferenceObj*)GetAt(posLoc);
}

void CReferenceObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CReferenceObj * refPtr = (CReferenceObj *)GetNext(posLoc);
		refPtr->SafeDelete();
		delete refPtr;
		refPtr=0;
	}
	RemoveAll();
}

void CReferenceObjList::FlagAllForAnimUpdate()
{
	/*for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CReferenceObj * refPtr = (CReferenceObj *)GetNext(posLoc);
		refPtr->m_animationUpdatedThisRound = FALSE;
		if(refPtr->m_skeletalOptionLive)
		   if(refPtr->m_skeletalOptionLive->m_lodRanges)
		      refPtr->m_skeletalOptionLive->m_lodRanges->ResetFlags();
	}*/
	RemoveAll();
}

CReferenceObj * CReferenceObjList::GetReferenceObjByName(CString name)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CReferenceObj * refPtr = (CReferenceObj *)GetNext(posLoc);
		if(refPtr->m_referenceName == name)
		   return refPtr;
	}
	return NULL;
}

int CReferenceObjList::GetReferenceIndexByName(CString name)
{
	int trace = -1;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CReferenceObj * refPtr = (CReferenceObj *)GetNext(posLoc);
		if(trace == -1)
		   trace = 0;
		
		if(refPtr->m_referenceName.CompareNoCase(name)==0)
		   return trace;

        

		trace++;
	}
	return -1;
}

void CReferenceObjList::ClearInstances()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CReferenceObj * refPtr = (CReferenceObj *)GetNext(posLoc);
		if(refPtr->m_skeletalOptionLive)
		{
		   refPtr->m_skeletalOptionLive->SafeDelete();
           delete refPtr->m_skeletalOptionLive;
		   refPtr->m_skeletalOptionLive = 0;
		}
	}
}

void CReferenceObjList::ReinitAll(LPDIRECT3DDEVICE9 g_pd3dDevice)
{//obsolete
     /*for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CReferenceObj * refPtr = (CReferenceObj *)GetNext(posLoc);
		//if(!refPtr->m_extLockPoints)
		/*if(refPtr->m_meshObject)
		   refPtr->m_meshObject->InitBuffer(g_pd3dDevice,NULL);

		

		if(refPtr->m_lodVisualList)
		   refPtr->m_lodVisualList->Initialize(g_pd3dDevice);
	 }*/
}


IMPLEMENT_SERIAL(CReferenceObj, CObject, VERSIONABLE_SCHEMA|5)
IMPLEMENT_SERIAL(CReferenceObjList, CObList, 0)

