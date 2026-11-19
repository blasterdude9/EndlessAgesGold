#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CFloraGenClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void CFloraObj::SafeDelete()
{
	if(m_meshObj)
	{
       m_meshObj->SafeDelete();
	   delete m_meshObj;
	   m_meshObj = 0;
	}


}

CEXMeshObj * CFloraObj::GenerateMesh()
{
    D3DVECTOR iVect,normalInit,offsetNoise;
	iVect.x = 0.0f;
	iVect.y = 0.0f;
	iVect.z = m_trunkRadius1;
	normalInit.x = 0.0f;
	normalInit.y = 0.0f;
	normalInit.z = 1.0f;
	int bufferCount = 0;

	offsetNoise.x = 0.0f;
	offsetNoise.y = 0.0f;
	offsetNoise.z = m_rootLength;
 
	if(m_meshObj)
	{
       delete [] m_meshObj->m_abVertexArray.m_vertexArray2;
	   delete [] m_meshObj->m_abVertexArray.m_indecieArray;
	   m_meshObj->m_abVertexArray.m_vertexArray2 = 0;
	   m_meshObj->m_abVertexArray.m_indecieArray = 0;
	}
	else{
	  m_meshObj = new CEXMeshObj();
	  m_meshObj->List_materialObject = new CMaterialObject();
	}

	int vertexCount = m_trunkVSegments * (m_trunkHSegments+1);
    int indecieCount = vertexCount * 6;

	//m_meshObj->m_abVertexArray.

	ABVERTEX2L * vbuffer = new ABVERTEX2L[vertexCount];
	ZeroMemory(vbuffer,sizeof(ABVERTEX2L)*vertexCount);
	WORD * iBuffer = new WORD[indecieCount];

	float hSegLength = m_TrunkHieght / m_trunkVSegments;
  
	//iVect.z = m_trunkRadius1;
	float slice = (float)(360/m_trunkHSegments);

	float spikeSlice = (float)(360/m_rootCount);
    //m_rootCount
    float currentRadius = 0.0f;
	float hgtEffect = m_rootHeight * m_TrunkHieght;
    float multiplier = 0.0f;
	float curRotation = 0;
	float curTxtRotation = 0;
	float multiplierTrunkNoise = 1.0f;
	//fill vertex buffer for(int h=0;h<m_trunkHSegments;h++)
	for(int h=0;h<(m_trunkHSegments+1);h++)
	{//360
	   for(int v=0;v<m_trunkVSegments;v++)
	   {//height segs
		   multiplier = 0.0f;
           //prep offset/rotation
           iVect.x = 0.0f;
	       iVect.y = 0.0f;
	       iVect.z = 0.0f;
	       normalInit.x = 0.0f;
	       normalInit.y = 0.0f;
	       normalInit.z = 1.0f;
		   

	       
           
  
		   vbuffer[bufferCount].nx = normalInit.x;
           vbuffer[bufferCount].ny = normalInit.y;
		   vbuffer[bufferCount].nz = normalInit.z;

		   if((v*hSegLength) < hgtEffect)
		   {
              multiplier = 1.0f - ((v*hSegLength)/hgtEffect);
			  
			  multiplier -= ((curRotation - (floor(curRotation/spikeSlice)*spikeSlice))/spikeSlice)*2.0f;
			  if(multiplier < 0.0f)
			     multiplier = 0.0f;
			  
		   }
		   

		   iVect.x = 0.0f;
	       iVect.y = v*hSegLength;
		   currentRadius = GL_matrixFunctions.InterpolateFloatsHiRes(
						            m_trunkRadius1,//float StartVec, 
						            m_trunkRadius2,//float EndVec,
					                m_TrunkHieght,//float NumOfStages, 
					                (v*hSegLength));//float ThisStage)

		   if(m_trunkNoise > 0.0f)
		      multiplierTrunkNoise = 1.0f - (((float)(rand()%((int)(m_trunkNoise*1000))))*.001f);
		   

	       iVect.z = iVect.z + (multiplierTrunkNoise * currentRadius)/*m_trunkRadius1*/ + (m_rootLength*multiplier);

		   iVect = GL_matrixFunctions.RotateY_WLD( curRotation,iVect);
	       normalInit = GL_matrixFunctions.RotateY_WLD ( curRotation, normalInit);

           //end prep offset/rotation	  
		   vbuffer[bufferCount].x = iVect.x;
           vbuffer[bufferCount].y = iVect.y;
		   vbuffer[bufferCount].z = iVect.z;
		   

           vbuffer[bufferCount].tx0.tv = 1.0f*((v*hSegLength)/m_TrunkHieght);//verticle UV
		   vbuffer[bufferCount].tx1.tv = (1.0f*((v*hSegLength)/m_TrunkHieght))*5;//detail verticle UV

		   vbuffer[bufferCount].tx0.tu = 1.0f*(curTxtRotation/360);//verticle UV
		   vbuffer[bufferCount++].tx1.tu = (1.0f*(curTxtRotation/360))*3;//detail verticle UV
	
	   }//end 360
	   curRotation = curRotation + slice;
	   curTxtRotation = curTxtRotation + slice;
	   if(curRotation == 360)
	      curRotation = 0;

       
	}//end height segs
	//Build Mesh with indecies
	//int Xrip  = 0;
	//int Yrip  = 0;
	//int Yrip2 = 0;
	int iTrace = 0;
	float columOffset = 0.0f;
	float invOp = 0.0f;
	for(int h=0;h<m_trunkHSegments;h++)
	{//360
      for(int v=0;v<(m_trunkVSegments-1);v++)
	  {
		  columOffset = (float)(m_trunkVSegments*h);

		  iBuffer[iTrace++] = (WORD)(v+columOffset+m_trunkVSegments);
		  iBuffer[iTrace++] = (WORD)(v+columOffset+1);
		  iBuffer[iTrace++] = (WORD)(v+columOffset);
          
		  iBuffer[iTrace++] = (WORD)(v+columOffset+m_trunkVSegments+1);
          iBuffer[iTrace++] = (WORD)(v+columOffset+1);
		  iBuffer[iTrace++] = (WORD)(v+columOffset+m_trunkVSegments); 
	  }
	}
	//end build mesh with indecies
	//pd3dDevice->SetFVF(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2);
    //pd3dDevice->DrawPrimitiveUP( D3DPT_TRIANGLELIST, vertexCount/3,bufferVx, sizeof(D3DXVECTOR3) );
	//DrawIndexedPrimitive
	/*pd3dDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,
                                       0,//UINT MinVertexIndex,
                                       iTrace,//UINT NumVertexIndices,
                                       (iTrace/3),//UINT PrimitiveCount,
                                       iBuffer,//const void *pIndexData,
                                       D3DFMT_INDEX16,//D3DFORMAT IndexDataFormat,
                                       vbuffer,//CONST void* pVertexStreamZeroData,
                                       sizeof(ABVERTEX2L)//UINT VertexStreamZeroStride
                                       );*/

	m_meshObj->m_abVertexArray.m_vertexArray2 = vbuffer;
	m_meshObj->m_abVertexArray.m_indecieArray = iBuffer;
	m_meshObj->m_abVertexArray.m_indecieCount = iTrace;
	m_meshObj->m_abVertexArray.m_vertexCount = vertexCount;
	m_meshObj->m_abVertexArray.m_uvCount = 2;

	//delete [] vbuffer;
	//vbuffer = 0;
	//delete [] iBuffer;
	//iBuffer = 0;
	return m_meshObj;
}




