//#include "stdafx.h"
#include "afxwin.h"
#include "afxext.h"
#define D3D_OVERLOADS
#include "resource.h"
#include <d3d9.h>
//#include <afxtempl.h>

#include "CAdvancedVertexClass.h"

CAdvancedVertexObj::CAdvancedVertexObj()
{
    m_uvCount                  = 0;
    
	//original data
    //m_origVertexArray          = NULL;
    m_vertexArray0             = NULL;
    m_vertexArray1             = NULL;    
    m_vertexArray2             = NULL;
    m_vertexArray3             = NULL;
    m_vertexArray4             = NULL;
    m_vertexArray5             = NULL;
    m_vertexArray6             = NULL;
    m_vertexArray7             = NULL;
    m_vertexArray8             = NULL;
	//buffers

	//fixed data
    m_indecieArray             = NULL;
    m_indecieCount             = 0;
	m_vertexCount              = 0; 

	m_vertexBuffer             = NULL;
	m_indicieBuffer            = NULL;

	m_advancedFixedMode        = 0;

	m_lockFlags = 0;

}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 void CAdvancedVertexObj::CacheUnimediateData()
{
    if(m_indecieArray){
       delete [] m_indecieArray;
	   m_indecieArray = 0;
	}
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 void CAdvancedVertexObj::Fill0UVArrayData(ABVERTEX0L * array)
{
	int i=0;
	//m_vertexCount
       switch(m_uvCount)
	   {
	    case 0:
			CopyMemory(array,m_vertexArray0,sizeof(ABVERTEX0L)*m_vertexCount);	
		break;
	   
	    case 1:
		    for(i=0;i<m_vertexCount;i++)
			   CopyMemory(&array[i],&m_vertexArray1[i],sizeof(ABVERTEX0L));	
		break;
	   
	    case 2:
			for(i=0;i<m_vertexCount;i++)
			   CopyMemory(&array[i],&m_vertexArray2[i],sizeof(ABVERTEX0L));	
	    break;
	   
	    case 3:
			for(i=0;i<m_vertexCount;i++)
			   CopyMemory(&array[i],&m_vertexArray3[i],sizeof(ABVERTEX0L));	
	    break;

		case 4:
			for(i=0;i<m_vertexCount;i++)
			   CopyMemory(&array[i],&m_vertexArray4[i],sizeof(ABVERTEX0L));	
	    break;

		case 5:
			for(i=0;i<m_vertexCount;i++)
			   CopyMemory(&array[i],&m_vertexArray5[i],sizeof(ABVERTEX0L));	
	    break;

		case 6:
			for(i=0;i<m_vertexCount;i++)
			   CopyMemory(&array[i],&m_vertexArray6[i],sizeof(ABVERTEX0L));	
	    break;

		case 7:
			for(i=0;i<m_vertexCount;i++)
			   CopyMemory(&array[i],&m_vertexArray7[i],sizeof(ABVERTEX0L));	
	    break;

		case 8:
			for(i=0;i<m_vertexCount;i++)
			   CopyMemory(&array[i],&m_vertexArray8[i],sizeof(ABVERTEX0L));	
	    break;
	   
	   }
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 void CAdvancedVertexObj::FillD3DVECTORArrayData(D3DVECTOR * array)
{
	int i=0;
	//m_vertexCount
       switch(m_uvCount)
	   {
	    case 0:
			for(i=0;i<m_vertexCount;i++)
			   CopyMemory(&array[i],&m_vertexArray0[i],sizeof(D3DVECTOR));	
		break;
	   
	    case 1:
		    for(i=0;i<m_vertexCount;i++)
			   CopyMemory(&array[i],&m_vertexArray1[i],sizeof(D3DVECTOR));	
		break;
	   
	    case 2:
			for(i=0;i<m_vertexCount;i++)
			   CopyMemory(&array[i],&m_vertexArray2[i],sizeof(D3DVECTOR));	
	    break;
	   
	    case 3:
			for(i=0;i<m_vertexCount;i++)
			   CopyMemory(&array[i],&m_vertexArray3[i],sizeof(D3DVECTOR));	
	    break;

		case 4:
			for(i=0;i<m_vertexCount;i++)
			   CopyMemory(&array[i],&m_vertexArray4[i],sizeof(D3DVECTOR));	
	    break;

		case 5:
			for(i=0;i<m_vertexCount;i++)
			   CopyMemory(&array[i],&m_vertexArray5[i],sizeof(D3DVECTOR));	
	    break;

		case 6:
			for(i=0;i<m_vertexCount;i++)
			   CopyMemory(&array[i],&m_vertexArray6[i],sizeof(D3DVECTOR));	
	    break;

		case 7:
			for(i=0;i<m_vertexCount;i++)
			   CopyMemory(&array[i],&m_vertexArray7[i],sizeof(D3DVECTOR));	
	    break;

		case 8:
			for(i=0;i<m_vertexCount;i++)
			   CopyMemory(&array[i],&m_vertexArray8[i],sizeof(D3DVECTOR));	
	    break;
	   
	   }
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 void CAdvancedVertexObj::CloneMinimum(CAdvancedVertexObj ** clone,
											 LPDIRECT3DDEVICE9 g_pd3dDevice)
{
       CAdvancedVertexObj * copyLocal = new CAdvancedVertexObj();
       copyLocal->m_uvCount             = m_uvCount;
	   copyLocal->m_indecieCount        = m_indecieCount;
	   copyLocal->m_vertexCount         = m_vertexCount;

	   copyLocal->m_indecieArray = new WORD[m_indecieCount];
	   CopyMemory(copyLocal->m_indecieArray,m_indecieArray,sizeof(WORD)*m_indecieCount);
       
	   switch(m_uvCount)
	   {
	    case 0:
			copyLocal->m_vertexArray0 = new ABVERTEX0L[m_vertexCount];
	        CopyMemory(copyLocal->m_vertexArray0,m_vertexArray0,sizeof(ABVERTEX0L)*m_vertexCount);
	    break;
	   
	    case 1:
			copyLocal->m_vertexArray1 = new ABVERTEX1L[m_vertexCount];
	        CopyMemory(copyLocal->m_vertexArray1,m_vertexArray1,sizeof(ABVERTEX1L)*m_vertexCount);
	    break;
	   
	    case 2:
			copyLocal->m_vertexArray2 = new ABVERTEX2L[m_vertexCount];
	        CopyMemory(copyLocal->m_vertexArray2,m_vertexArray2,sizeof(ABVERTEX2L)*m_vertexCount);
	    break;
	   
	    case 3:
			copyLocal->m_vertexArray3 = new ABVERTEX3L[m_vertexCount];
	        CopyMemory(copyLocal->m_vertexArray3,m_vertexArray3,sizeof(ABVERTEX3L)*m_vertexCount);
	    break;

		case 4:
			copyLocal->m_vertexArray4 = new ABVERTEX4L[m_vertexCount];
	        CopyMemory(copyLocal->m_vertexArray4,m_vertexArray4,sizeof(ABVERTEX4L)*m_vertexCount);
	    break;

		case 5:
			copyLocal->m_vertexArray5 = new ABVERTEX5L[m_vertexCount];
	        CopyMemory(copyLocal->m_vertexArray5,m_vertexArray5,sizeof(ABVERTEX5L)*m_vertexCount);
	    break;

		case 6:
			copyLocal->m_vertexArray6 = new ABVERTEX6L[m_vertexCount];
	        CopyMemory(copyLocal->m_vertexArray6,m_vertexArray6,sizeof(ABVERTEX6L)*m_vertexCount);
	    break;

		case 7:
			copyLocal->m_vertexArray7 = new ABVERTEX7L[m_vertexCount];
	        CopyMemory(copyLocal->m_vertexArray7,m_vertexArray7,sizeof(ABVERTEX7L)*m_vertexCount);
	    break;

		case 8:
			copyLocal->m_vertexArray8 = new ABVERTEX8L[m_vertexCount];
	        CopyMemory(copyLocal->m_vertexArray8,m_vertexArray8,sizeof(ABVERTEX0L)*m_vertexCount);
	    break;
	   
	   }

	   InitBuffer(g_pd3dDevice,copyLocal->m_uvCount);

       
	   *clone = copyLocal;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 void CAdvancedVertexObj::GetABVERTEXStyle(ABVERTEX ** vArray,
												 int * vertexCount)
{
   if(m_vertexCount == 0)
	  return;
   ABVERTEX * newVertexArray = new ABVERTEX[m_vertexCount];
   ZeroMemory(newVertexArray,(sizeof(ABVERTEX)*m_vertexCount));
   
   int loop=0;
	   switch(m_uvCount)
	   {
	    case 0:
            
            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&newVertexArray[loop],&m_vertexArray0[loop],sizeof(ABVERTEX0L));

	    break;
	   
	    case 1:

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&newVertexArray[loop],&m_vertexArray1[loop],sizeof(ABVERTEX1L));
	    break;
	   
	    case 2:

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&newVertexArray[loop],&m_vertexArray2[loop],sizeof(ABVERTEX2L));
	    break;
	   
	    case 3:

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&newVertexArray[loop],&m_vertexArray3[loop],sizeof(ABVERTEX3L));
	    break;

		case 4:
          
            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&newVertexArray[loop],&m_vertexArray4[loop],sizeof(ABVERTEX4L));
	    break;

		case 5:

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&newVertexArray[loop],&m_vertexArray5[loop],sizeof(ABVERTEX5L));
	    break;

		case 6:

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&newVertexArray[loop],&m_vertexArray6[loop],sizeof(ABVERTEX6L));
	    break;

		case 7:

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&newVertexArray[loop],&m_vertexArray7[loop],sizeof(ABVERTEX7L));
	    break;

		case 8:

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&newVertexArray[loop],&m_vertexArray8[loop],sizeof(ABVERTEX8L));
	    break;
	   
    }
   
   *vertexCount = m_vertexCount;
   *vArray = newVertexArray;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 void CAdvancedVertexObj::SetABVERTEXStyle(IDirect3DDevice9 * pd3dDevice,
												 ABVERTEX * vArray,
												 int vertexCount)
{
	m_vertexCount = vertexCount;
    int loop=0;
	   switch(m_uvCount)
	   {
	    case 0:
            if(m_vertexArray0)
			   delete [] m_vertexArray0;
			   m_vertexArray0 = 0;

			m_vertexArray0 = new ABVERTEX0L[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&m_vertexArray0[loop],&vArray[loop],sizeof(ABVERTEX0L));

	    break;
	   
	    case 1:
            if(m_vertexArray1)
			   delete [] m_vertexArray1;
			   m_vertexArray1 = 0;

			m_vertexArray1 = new ABVERTEX1L[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&m_vertexArray1[loop],&vArray[loop],sizeof(ABVERTEX1L));
	    break;
	   
	    case 2:
            if(m_vertexArray2)
			   delete [] m_vertexArray2;
			   m_vertexArray2 = 0;

			m_vertexArray2 = new ABVERTEX2L[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&m_vertexArray2[loop],&vArray[loop],sizeof(ABVERTEX2L));
	    break;
	   
	    case 3:
            if(m_vertexArray3)
			   delete [] m_vertexArray3;
			   m_vertexArray3 = 0;

			m_vertexArray3 = new ABVERTEX3L[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&m_vertexArray3[loop],&vArray[loop],sizeof(ABVERTEX3L));
	    break;

		case 4:
            if(m_vertexArray4)
			   delete [] m_vertexArray4;
			   m_vertexArray4 = 0;

			m_vertexArray4 = new ABVERTEX4L[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&m_vertexArray4[loop],&vArray[loop],sizeof(ABVERTEX4L));
	    break;

		case 5:
            if(m_vertexArray5)
			   delete [] m_vertexArray5;
			   m_vertexArray5 = 0;

			m_vertexArray5 = new ABVERTEX5L[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&m_vertexArray5[loop],&vArray[loop],sizeof(ABVERTEX5L));
	    break;

		case 6:
            if(m_vertexArray6)
			   delete [] m_vertexArray6;
			   m_vertexArray6 = 0;

			m_vertexArray6 = new ABVERTEX6L[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&m_vertexArray6[loop],&vArray[loop],sizeof(ABVERTEX6L));
	    break;

		case 7:
            if(m_vertexArray7)
			   delete [] m_vertexArray7;
			   m_vertexArray7 = 0;

			m_vertexArray7 = new ABVERTEX7L[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&m_vertexArray7[loop],&vArray[loop],sizeof(ABVERTEX7L));
	    break;

		case 8:
            if(m_vertexArray8)
			   delete [] m_vertexArray8;
			   m_vertexArray8 = 0;

			m_vertexArray8 = new ABVERTEX8L[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&m_vertexArray8[loop],&vArray[loop],sizeof(ABVERTEX8L));
	    break;
	   
	   }

	   if(pd3dDevice)
	      InitBuffer(pd3dDevice,this->m_uvCount);
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CAdvancedVertexObj::ReinitializeDxVbuffers(IDirect3DDevice9 * pd3dDevice,
												D3DPOOL memManage,
												DWORD bufUsage,
												DWORD vUsage,
												int targetUVcount)
{
       if(m_vertexBuffer)
	   {
          m_vertexBuffer->Release();
	      m_vertexBuffer = 0;
	   }

	   if(m_vertexCount == 0 || m_indecieCount == 0)
		  return;

	   /*if(m_indicieBuffer)
	   {
          m_indicieBuffer->Release();
		  m_indicieBuffer = 0;
	   }*/

       if(!m_indicieBuffer)
	   {//needs to be created once
		  int datasize = m_indecieCount * sizeof(WORD);
		  if(pd3dDevice->CreateIndexBuffer(datasize,
                                           bufUsage  ,
                                           D3DFMT_INDEX16,
                                           memManage,//D3DPOOL_MANAGED  ,
                                           &m_indicieBuffer,
										   NULL)==0)
		  {
			 
			 WORD * indecieDxBuffer = NULL;
             if(m_indicieBuffer->Lock(0,datasize,(VOID**)&indecieDxBuffer, m_lockFlags )==0)
			 {
                CopyMemory(indecieDxBuffer,m_indecieArray,datasize);
				m_indicieBuffer->Unlock();
			 }
		  }
	   }//end needs to be created once


       //determine uvset
       /*if(m_vertexArray0)
	      m_uvCount = 0;
	   else if(m_vertexArray1)
	      m_uvCount = 1;
	   else if(m_vertexArray2)
	      m_uvCount = 2;
	   else if(m_vertexArray3)
	      m_uvCount = 3;
	   else if(m_vertexArray4)
	      m_uvCount = 4;
	   else if(m_vertexArray5)
	      m_uvCount = 5;
	   else if(m_vertexArray6)
	      m_uvCount = 6;
	   else if(m_vertexArray7)
	      m_uvCount = 7;*/


       int requiredUvAdditional = 0;
	   int originalUvCount = m_uvCount;

	   if(targetUVcount > m_uvCount)
	   {//additional uv sets required
          requiredUvAdditional = targetUVcount - m_uvCount;
		  m_uvCount = targetUVcount;
	   }//end additional uv sets required

	   long datasize = 0;
       switch(m_uvCount)
	   {
	    case 0:
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX0L),
                                               vUsage    , (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX0),
                                               memManage  ,
											   &m_vertexBuffer,
											   NULL)!=0)
											   return;
			}
            // Create a vertex buffer
               ABVERTEX0L* pVertices0;
			   datasize = m_vertexCount*sizeof(ABVERTEX0L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices0, m_lockFlags )==0)
			   {//lock buffer
                  //for(int loop=0;loop<m_vertexCount;loop++)
	              //    memcpy(&pVertices0[loop],&m_origVertexArray[loop],sizeof(ABVERTEX0L));

                  CopyMemory(pVertices0,m_vertexArray0,(sizeof(ABVERTEX0L)*m_vertexCount));

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;
		case 1:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX1L),
                                               vUsage   , (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1),
                                               memManage  ,
											   &m_vertexBuffer,
											   NULL))
											   return;
			}
            // Create a vertex buffer
               ABVERTEX1L* pVertices1;
			   datasize = m_vertexCount*sizeof(ABVERTEX1L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices1, m_lockFlags  )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	                  //memcpy(&pVertices1[loop],&m_origVertexArray[loop],sizeof(ABVERTEX1L));
                  //CopyMemory(pVertices1,m_vertexArray1,(sizeof(ABVERTEX1L)*m_vertexCount));
                  if(requiredUvAdditional == 0)
				     CopyMemory(pVertices1,m_vertexArray1,(sizeof(ABVERTEX1L)*m_vertexCount));
				  else if(requiredUvAdditional == 1)
				  {
					   if(!m_vertexArray1)
					       m_vertexArray1 = new ABVERTEX1L[m_vertexCount];

                       for(int loop=0;loop<m_vertexCount;loop++)
					   {
	                       memcpy(&m_vertexArray1[loop],&m_vertexArray0[loop],sizeof(ABVERTEX0L));
						   ZeroMemory(&m_vertexArray1[loop].tx0,sizeof(TXTUV));
					   }
					   if(m_vertexArray0)
					   {
                          delete [] m_vertexArray0;
						  m_vertexArray0 = 0;
					   }
					   CopyMemory(pVertices1,m_vertexArray1,(sizeof(ABVERTEX1L)*m_vertexCount));
				  }

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 2:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX2L),
                                                 vUsage   , (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2),
                                                 memManage  ,
												 &m_vertexBuffer,
												 NULL)!=0)
												 return;
			}
               ABVERTEX2L* pVertices2;
			   datasize = m_vertexCount*sizeof(ABVERTEX2L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices2, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	              //    memcpy(&pVertices2[loop],&m_origVertexArray[loop],sizeof(ABVERTEX2L));

				  //CopyMemory(pVertices2,m_vertexArray2,(sizeof(ABVERTEX2L)*m_vertexCount));
                  if(requiredUvAdditional == 0)
				     CopyMemory(pVertices2,m_vertexArray2,(sizeof(ABVERTEX2L)*m_vertexCount));
				  else if(requiredUvAdditional == 1)
				  {
					   if(!m_vertexArray2)
					       m_vertexArray2 = new ABVERTEX2L[m_vertexCount];

                       for(int loop=0;loop<m_vertexCount;loop++)
					   {
	                       memcpy(&m_vertexArray2[loop],&m_vertexArray1[loop],sizeof(ABVERTEX1L));
						   memcpy(&m_vertexArray2[loop].tx1,&m_vertexArray2[loop].tx0,sizeof(TXTUV));
					   }
					   if(m_vertexArray1)
					   {
                          delete [] m_vertexArray1;
						  m_vertexArray1 = 0;
					   }
					   CopyMemory(pVertices2,m_vertexArray2,(sizeof(ABVERTEX2L)*m_vertexCount));
				  }
				  else if(requiredUvAdditional == 2)
				  {
					   if(!m_vertexArray2)
					       m_vertexArray2 = new ABVERTEX2L[m_vertexCount];

                       for(int loop=0;loop<m_vertexCount;loop++)
					   {
	                       memcpy(&m_vertexArray2[loop],&m_vertexArray0[loop],sizeof(ABVERTEX0L));
						   ZeroMemory(&m_vertexArray2[loop].tx0,sizeof(TXTUV));
                           ZeroMemory(&m_vertexArray2[loop].tx1,sizeof(TXTUV));
					   }
					   if(m_vertexArray0)
					   {
                          delete [] m_vertexArray0;
						  m_vertexArray0 = 0;
					   }
					   CopyMemory(pVertices2,m_vertexArray2,(sizeof(ABVERTEX2L)*m_vertexCount));
				  }

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 3:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX3L),
                                               vUsage , (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX3),
                                               memManage ,
											   &m_vertexBuffer,
											   NULL)!=0)
											   return;
			}
            // Create a vertex buffer
               ABVERTEX3L* pVertices3;
			   datasize = m_vertexCount*sizeof(ABVERTEX3L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices3, m_lockFlags )==0)
			   {//lock buffer
				  
                  if(requiredUvAdditional == 0)
				     CopyMemory(pVertices3,m_vertexArray3,(sizeof(ABVERTEX3L)*m_vertexCount));
				  else if(requiredUvAdditional == 1)
				  {
					   if(!m_vertexArray3)
					       m_vertexArray3 = new ABVERTEX3L[m_vertexCount];

                       for(int loop=0;loop<m_vertexCount;loop++)
					   {
	                       memcpy(&m_vertexArray3[loop],&m_vertexArray2[loop],sizeof(ABVERTEX2L));
						   memcpy(&m_vertexArray3[loop].tx2,&m_vertexArray3[loop].tx0,sizeof(TXTUV));
					   }
					   if(m_vertexArray2)
					   {
                          delete [] m_vertexArray2;
						  m_vertexArray2 = 0;
					   }
					   CopyMemory(pVertices3,m_vertexArray3,(sizeof(ABVERTEX3L)*m_vertexCount));
				  }
				  else if(requiredUvAdditional == 2)
				  {
					   if(!m_vertexArray3)
					       m_vertexArray3 = new ABVERTEX3L[m_vertexCount];

                       for(int loop=0;loop<m_vertexCount;loop++)
					   {
	                       memcpy(&m_vertexArray3[loop],&m_vertexArray1[loop],sizeof(ABVERTEX1L));
						   memcpy(&m_vertexArray3[loop].tx1,&m_vertexArray3[loop].tx0,sizeof(TXTUV));
						   memcpy(&m_vertexArray3[loop].tx2,&m_vertexArray3[loop].tx0,sizeof(TXTUV));
                     
					   }
					   if(m_vertexArray1)
					   {
                          delete [] m_vertexArray1;
						  m_vertexArray1 = 0;
					   }
					   CopyMemory(pVertices3,m_vertexArray3,(sizeof(ABVERTEX3L)*m_vertexCount));
				  }
				  else if(requiredUvAdditional == 3)
				  {
					   if(!m_vertexArray3)
					       m_vertexArray3 = new ABVERTEX3L[m_vertexCount];
					   
                       for(int loop=0;loop<m_vertexCount;loop++)
					   {
	                       memcpy(&m_vertexArray3[loop],&m_vertexArray0[loop],sizeof(ABVERTEX0L));
						   ZeroMemory(&m_vertexArray3[loop].tx0,sizeof(TXTUV));
                           ZeroMemory(&m_vertexArray3[loop].tx1,sizeof(TXTUV));
						   ZeroMemory(&m_vertexArray3[loop].tx2,sizeof(TXTUV));
					   }
					   if(m_vertexArray0)
					   {
                          delete [] m_vertexArray0;
						  m_vertexArray0 = 0;
					   }
					   CopyMemory(pVertices3,m_vertexArray3,(sizeof(ABVERTEX3L)*m_vertexCount));
				  }
			   

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 4:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX4L),
                                               vUsage   , (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX4),
                                               memManage  ,
											   &m_vertexBuffer,
											   NULL)!=0)
											   return;
			}
			
      
               ABVERTEX4L* pVertices4;
			   datasize = m_vertexCount*sizeof(ABVERTEX4L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices4, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	               //   memcpy(&pVertices4[loop],&m_origVertexArray[loop],sizeof(ABVERTEX4L));
                  if(requiredUvAdditional == 0)
				     CopyMemory(pVertices4,m_vertexArray4,(sizeof(ABVERTEX4L)*m_vertexCount));
				  else if(requiredUvAdditional == 1)
				  {
					   if(!m_vertexArray4)
					       m_vertexArray4 = new ABVERTEX4L[m_vertexCount];

                       for(int loop=0;loop<m_vertexCount;loop++)
					   {
	                       memcpy(&m_vertexArray4[loop],&m_vertexArray3[loop],sizeof(ABVERTEX3L));
						   memcpy(&m_vertexArray4[loop].tx3,&m_vertexArray4[loop].tx0,sizeof(TXTUV));
					   }
					   if(m_vertexArray3)
					   {
                          delete [] m_vertexArray3;
						  m_vertexArray3 = 0;
					   }
					   CopyMemory(pVertices4,m_vertexArray4,(sizeof(ABVERTEX4L)*m_vertexCount));
				  }
				  else if(requiredUvAdditional == 2)
				  {
					   if(!m_vertexArray4)
					       m_vertexArray4 = new ABVERTEX4L[m_vertexCount];

                       for(int loop=0;loop<m_vertexCount;loop++)
					   {
	                       memcpy(&m_vertexArray4[loop],&m_vertexArray2[loop],sizeof(ABVERTEX2L));
						   memcpy(&m_vertexArray4[loop].tx2,&m_vertexArray4[loop].tx0,sizeof(TXTUV));
						   memcpy(&m_vertexArray4[loop].tx3,&m_vertexArray4[loop].tx0,sizeof(TXTUV));
                     
					   }
					   if(m_vertexArray2)
					   {
                          delete [] m_vertexArray2;
						  m_vertexArray2 = 0;
					   }
					   CopyMemory(pVertices4,m_vertexArray4,(sizeof(ABVERTEX4L)*m_vertexCount));
				  }
				  else if(requiredUvAdditional == 3)
				  {
					   if(!m_vertexArray4)
					       m_vertexArray4 = new ABVERTEX4L[m_vertexCount];
					   
                       for(int loop=0;loop<m_vertexCount;loop++)
					   {
	                       memcpy(&m_vertexArray4[loop],&m_vertexArray1[loop],sizeof(ABVERTEX1L));
						   memcpy(&m_vertexArray4[loop].tx1,&m_vertexArray4[loop].tx0,sizeof(TXTUV));
						   memcpy(&m_vertexArray4[loop].tx2,&m_vertexArray4[loop].tx0,sizeof(TXTUV));
						   memcpy(&m_vertexArray4[loop].tx3,&m_vertexArray4[loop].tx0,sizeof(TXTUV));
					   }
					   if(m_vertexArray1)
					   {
                          delete [] m_vertexArray1;
						  m_vertexArray1 = 0;
					   }
					   CopyMemory(pVertices4,m_vertexArray4,(sizeof(ABVERTEX4L)*m_vertexCount));
				  }
				  else if(requiredUvAdditional == 4)
				  {
					   if(!m_vertexArray4)
					       m_vertexArray4 = new ABVERTEX4L[m_vertexCount];
					   
                       for(int loop=0;loop<m_vertexCount;loop++)
					   {
	                       memcpy(&m_vertexArray4[loop],&m_vertexArray0[loop],sizeof(ABVERTEX0L));
						   ZeroMemory(&m_vertexArray4[loop].tx0,sizeof(TXTUV));
						   ZeroMemory(&m_vertexArray4[loop].tx1,sizeof(TXTUV));
                           ZeroMemory(&m_vertexArray4[loop].tx2,sizeof(TXTUV));
						   ZeroMemory(&m_vertexArray4[loop].tx3,sizeof(TXTUV));
					   }
					   if(m_vertexArray0)
					   {
                          delete [] m_vertexArray0;
						  m_vertexArray0 = 0;
					   }
					   CopyMemory(pVertices4,m_vertexArray4,(sizeof(ABVERTEX4L)*m_vertexCount));
				  }
				  
				  
				  
				  m_vertexBuffer->Unlock();
			   }//end lock beffer
		
			//-----------------------------
			//-----------------------------
	    break;
		case 5:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX5L),
                                               vUsage   , (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX5),
                                               memManage  ,
											   &m_vertexBuffer,
											   NULL)!=0)
											   return;
			}
            // Create a vertex buffer
               ABVERTEX5L* pVertices5;
			   datasize = m_vertexCount*sizeof(ABVERTEX5L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices5, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	              //    memcpy(&pVertices5[loop],&m_origVertexArray[loop],sizeof(ABVERTEX5L));
 
				  CopyMemory(pVertices5,m_vertexArray5,(sizeof(ABVERTEX5L)*m_vertexCount));

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 6:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX6L),
                                               vUsage   , (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX6),
                                               memManage  ,
											   &m_vertexBuffer ,
											   NULL)!=0)
											   return;
			}
            // Create a vertex buffer
               ABVERTEX6L* pVertices6;
			   datasize = m_vertexCount*sizeof(ABVERTEX6L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices6, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	              //    memcpy(&pVertices6[loop],&m_origVertexArray[loop],sizeof(ABVERTEX6L));
                  
                  CopyMemory(pVertices6,m_vertexArray6,(sizeof(ABVERTEX6L)*m_vertexCount));
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 7:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			    if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX7L),
                                                vUsage   , (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX7),
                                                memManage  ,
												&m_vertexBuffer ,
												NULL)!=0)
												return;
			}
            // Create a vertex buffer
               ABVERTEX7L* pVertices7;
			   datasize = m_vertexCount*sizeof(ABVERTEX7L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices7, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	                  //memcpy(&pVertices7[loop],&m_origVertexArray[loop],sizeof(ABVERTEX7L));

				  CopyMemory(pVertices7,m_vertexArray7,(sizeof(ABVERTEX7L)*m_vertexCount));

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 8:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX8L),
                                               vUsage   , (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX8),
                                               memManage  ,
											   &m_vertexBuffer ,
											   NULL)!=0)
											   return;
			}
            // Create a vertex buffer
               ABVERTEX8L* pVertices8;
			   datasize = m_vertexCount*sizeof(ABVERTEX8L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices8, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	                 // memcpy(&pVertices8[loop],&m_origVertexArray[loop],sizeof(ABVERTEX8L));
                  CopyMemory(pVertices8,m_vertexArray8,(sizeof(ABVERTEX8L)*m_vertexCount));

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;

	   }
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
D3DCOLOR CAdvancedVertexObj::DiffuseGradiant(float & curY,float & startRed, float & startGreen, float & startBlue,
											              float & endRed, float & endGreen, float & endBlue,
														  float & constantEffectStart,float & constantEffectEnd)
{
         /*if(curY <= startRed)
	     {
            if(curY > constantEffectEnd)
		    {
			   float NumOfStages = (constantEffectStart-constantEffectEnd);
               if (NumOfStages <= 0.0f)
                   NumOfStages = 1.0f;
               float propfactor = (float)(curY+constantEffectStart)/NumOfStages;
               return D3DCOLOR_ARGB(255,(int)(startRed + propfactor * (endRed - startRed)),(int)(startGreen + propfactor * (endGreen - startGreen)),(int)(startBlue + propfactor * (endBlue - startBlue)));
		    }
		    else
			   return D3DCOLOR_ARGB(255,(int)endRed,(int)endGreen,(int)endBlue);
		 }*/
	

         /*if(curY < constantEffectStart)
		 return D3DCOLOR_ARGB(255,(int)endRed,(int)endGreen,(int)endBlue);
		 else
		 return D3DCOLOR_ARGB(255,(int)startRed,(int)startGreen,(int)startBlue);*/

	float distance = constantEffectEnd;
	float currentPosition = (-curY)+constantEffectStart;
	float multiplier = 1.0f;
    if(currentPosition == 0.0f)
	   multiplier = 1.0f;
	if(currentPosition > distance)
       multiplier = 0.0f;
    else if(currentPosition < 0.0f)
       multiplier = 1.0f;
	else{
       multiplier = 1.0f - (currentPosition/distance);

	}

    return D3DCOLOR_ARGB(255,(int)(startRed * multiplier),(int)(startGreen * multiplier),(int)255);
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CAdvancedVertexObj::ReinitializeDxVbuffersAdvanced(IDirect3DDevice9 * pd3dDevice,
												        D3DPOOL memManage,
												        DWORD bufUsage,
												        DWORD vUsage,
														int effect)
{
	if(m_vertexBuffer)
	{
       m_vertexBuffer->Release();
	   m_vertexBuffer = 0;
	}
	   m_advancedFixedMode = 1;

       float constantEffectStart    = 0.0f;
	   float constantEffectMaxEnd   = 100.0f;
	   float colorStartRed            = 255;
       float colorStartGreen          = 255;
	   float colorStartBlue           = 255;
	   float colorStartAlpha          = 255;

	   float colorEndRed              = 0;
       float colorEndGreen            = 0;
	   float colorEndBlue             = 255;
	   float colorEndAlpha            = 255;

	   
	   
	   

	   

	   if(m_vertexCount == 0 || m_indecieCount == 0)
		  return;

       if(!m_indicieBuffer)
	   {//needs to be created once
		  int datasize = m_indecieCount * sizeof(WORD);
		  if(pd3dDevice->CreateIndexBuffer(datasize,
                                           bufUsage  ,
                                           D3DFMT_INDEX16,
                                           memManage,//D3DPOOL_MANAGED  ,
                                           &m_indicieBuffer,
										   NULL)==0)
		  {
			 
			 WORD * indecieDxBuffer = NULL;
             if(m_indicieBuffer->Lock(0,datasize,(VOID**)&indecieDxBuffer, m_lockFlags )==0)
			 {
                CopyMemory(indecieDxBuffer,m_indecieArray,datasize);
				m_indicieBuffer->Unlock();
			 }
		  }
	   }//end needs to be created once

	   long datasize = 0;
       switch(m_uvCount)
	   {
	    case 0:
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX0La),
                                               vUsage    , (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX0),
                                               memManage  ,
											   &m_vertexBuffer,
											   NULL)!=0)
											   return;
			}
            // Create a vertex buffer
               ABVERTEX0La* pVertices0;
			   datasize = m_vertexCount*sizeof(ABVERTEX0La);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices0, m_lockFlags )==0)
			   {//lock buffer
                  for(int loop=0;loop<m_vertexCount;loop++)
				  {
                      pVertices0[loop].x  = m_vertexArray0[loop].x;
					  pVertices0[loop].y  = m_vertexArray0[loop].y;
					  pVertices0[loop].z  = m_vertexArray0[loop].z;
					  pVertices0[loop].nx = m_vertexArray0[loop].nx;
					  pVertices0[loop].ny = m_vertexArray0[loop].ny;
					  pVertices0[loop].nz = m_vertexArray0[loop].nz;
					  
					  pVertices0[loop].diffuse = DiffuseGradiant(pVertices0[loop].y,colorStartRed, colorStartGreen, colorStartBlue,
											                     colorEndRed, colorEndGreen, colorEndBlue,
														         constantEffectStart,constantEffectMaxEnd);
					  
				  }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;
		case 1:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX1La),
                                               vUsage   , (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1),
                                               memManage  ,
											   &m_vertexBuffer,
											   NULL))
											   return;
			}
            // Create a vertex buffer
               ABVERTEX1La* pVertices1;
			   datasize = m_vertexCount*sizeof(ABVERTEX1La);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices1, m_lockFlags  )==0)
			   {//lock buffer
				  for(int loop=0;loop<m_vertexCount;loop++)
				  {
					  pVertices1[loop].x  = m_vertexArray1[loop].x;
					  pVertices1[loop].y  = m_vertexArray1[loop].y;
					  pVertices1[loop].z  = m_vertexArray1[loop].z;
					  pVertices1[loop].nx = m_vertexArray1[loop].nx;
					  pVertices1[loop].ny = m_vertexArray1[loop].ny;
					  pVertices1[loop].nz = m_vertexArray1[loop].nz;
					  pVertices1[loop].tx0.tu = m_vertexArray1[loop].tx0.tu;
					  pVertices1[loop].tx0.tv = m_vertexArray1[loop].tx0.tv;
					  
					  pVertices1[loop].diffuse = DiffuseGradiant(pVertices1[loop].y,colorStartRed, colorStartGreen, colorStartBlue,
											                     colorEndRed, colorEndGreen, colorEndBlue,
														         constantEffectStart,constantEffectMaxEnd);
					
				  }

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 2:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX2La),
                                                 vUsage   , (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2),
                                                 memManage  ,
												 &m_vertexBuffer,
												 NULL)!=0)
												 return;
			}
               ABVERTEX2La* pVertices2;
			   datasize = m_vertexCount*sizeof(ABVERTEX2La);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices2, m_lockFlags )==0)
			   {//lock buffer
				  for(int loop=0;loop<m_vertexCount;loop++)
				  {
					  pVertices2[loop].x  = m_vertexArray2[loop].x;
					  pVertices2[loop].y  = m_vertexArray2[loop].y;
					  pVertices2[loop].z  = m_vertexArray2[loop].z;
					  pVertices2[loop].nx = m_vertexArray2[loop].nx;
					  pVertices2[loop].ny = m_vertexArray2[loop].ny;
					  pVertices2[loop].nz = m_vertexArray2[loop].nz;
					  pVertices2[loop].tx0.tu = m_vertexArray2[loop].tx0.tu;
					  pVertices2[loop].tx0.tv = m_vertexArray2[loop].tx0.tv;
					  pVertices2[loop].tx1.tu = m_vertexArray2[loop].tx1.tu;
					  pVertices2[loop].tx1.tv = m_vertexArray2[loop].tx1.tv;
					  

					  pVertices2[loop].diffuse = DiffuseGradiant(pVertices2[loop].y,colorStartRed, colorStartGreen, colorStartBlue,
											                     colorEndRed, colorEndGreen, colorEndBlue,
														         constantEffectStart,constantEffectMaxEnd);
					  
				  }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 3:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX3L),
                                               vUsage , (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX3),
                                               memManage ,
											   &m_vertexBuffer,
											   NULL)!=0)
											   return;
			}
            // Create a vertex buffer
               ABVERTEX3La* pVertices3;
			   datasize = m_vertexCount*sizeof(ABVERTEX3La);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices3, m_lockFlags )==0)
			   {//lock buffer
				  for(int loop=0;loop<m_vertexCount;loop++)
				  {
					  CopyMemory(&pVertices3[loop],&m_vertexArray3[loop],sizeof(ABVERTEX3L));
                     
					  pVertices3[loop].diffuse = DiffuseGradiant(pVertices3[loop].y,colorStartRed, colorStartGreen, colorStartBlue,
											                     colorEndRed, colorEndGreen, colorEndBlue,
														         constantEffectStart,constantEffectMaxEnd);
					
				  }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 4:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX4La),
                                               vUsage   , (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX4),
                                               memManage  ,
											   &m_vertexBuffer,
											   NULL)!=0)
											   return;
			}
			
      
               ABVERTEX4La* pVertices4;
			   datasize = m_vertexCount*sizeof(ABVERTEX4La);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices4, m_lockFlags )==0)
			   {//lock buffer
				  for(int loop=0;loop<m_vertexCount;loop++)
				  {
					  CopyMemory(&pVertices4[loop],&m_vertexArray4[loop],sizeof(ABVERTEX4L));
                     
					  pVertices4[loop].diffuse = DiffuseGradiant(pVertices4[loop].y,colorStartRed, colorStartGreen, colorStartBlue,
											                     colorEndRed, colorEndGreen, colorEndBlue,
														         constantEffectStart,constantEffectMaxEnd);
					 
				  }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
		
			//-----------------------------
			//-----------------------------
	    break;
	   }
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CAdvancedVertexObj::ReinitializeDxVbuffersPositionOnly(IDirect3DDevice9 * pd3dDevice)
{

       

	   if(m_vertexCount == 0)
		  return;

	   if(!this->m_indicieBuffer)
	      return;

	   long datasize = 0;
       switch(m_uvCount)
	   {
	    case 0:
               ABVERTEX0L* pVertices0;
			   datasize = m_vertexCount*sizeof(ABVERTEX0L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices0, 0 )==0)
			   {//lock buffer
				   for(int loop=0;loop<m_vertexCount;loop++){
                       pVertices0[loop].x = m_vertexArray0[loop].x;
					   pVertices0[loop].y = m_vertexArray0[loop].y;
					   pVertices0[loop].z = m_vertexArray0[loop].z;

				   }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;
		case 1:
			//-----------------------------
			//-----------------------------
            // Create a vertex buffer
               ABVERTEX1L* pVertices1;
			   datasize = m_vertexCount*sizeof(ABVERTEX1L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices1, 0  )==0)
			   {//lock buffer
				   for(int loop=0;loop<m_vertexCount;loop++){
	                   pVertices1[loop].x = m_vertexArray1[loop].x;
					   pVertices1[loop].y = m_vertexArray1[loop].y;
					   pVertices1[loop].z = m_vertexArray1[loop].z;
				   }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 2:
			//-----------------------------
			//-----------------------------
               ABVERTEX2L* pVertices2;
			   datasize = m_vertexCount*sizeof(ABVERTEX2L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices2, 0 )==0)
			   {//lock buffer
				   for(int loop=0;loop<m_vertexCount;loop++){
	                   pVertices2[loop].x = m_vertexArray2[loop].x;
					   pVertices2[loop].y = m_vertexArray2[loop].y;
					   pVertices2[loop].z = m_vertexArray2[loop].z;
				   }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 3:
			//-----------------------------
			//-----------------------------
            // Create a vertex buffer
               ABVERTEX3L* pVertices3;
			   datasize = m_vertexCount*sizeof(ABVERTEX3L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices3, 0 )==0)
			   {//lock buffer
				   for(int loop=0;loop<m_vertexCount;loop++){
                       pVertices3[loop].x = m_vertexArray3[loop].x;
					   pVertices3[loop].y = m_vertexArray3[loop].y;
					   pVertices3[loop].z = m_vertexArray3[loop].z;
				   }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 4:
			//-----------------------------
			//-----------------------------
               ABVERTEX4L* pVertices4;
			   datasize = m_vertexCount*sizeof(ABVERTEX4L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices4, 0 )==0)
			   {//lock buffer
				  for(int loop=0;loop<m_vertexCount;loop++)
				  {
	                  pVertices4[loop].x = m_vertexArray4[loop].x;
					  pVertices4[loop].y = m_vertexArray4[loop].y;
					  pVertices4[loop].z = m_vertexArray4[loop].z;
				  }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
		
			//-----------------------------
			//-----------------------------
	    break;
		case 5:
			//-----------------------------
			//-----------------------------
            // Create a vertex buffer
               ABVERTEX5L* pVertices5;
			   datasize = m_vertexCount*sizeof(ABVERTEX5L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices5, 0 )==0)
			   {//lock buffer
				   for(int loop=0;loop<m_vertexCount;loop++){
	                  pVertices5[loop].x = m_vertexArray5[loop].x;
					  pVertices5[loop].y = m_vertexArray5[loop].y;
					  pVertices5[loop].z = m_vertexArray5[loop].z;
				   }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 6:
			//-----------------------------
			//-----------------------------
            // Create a vertex buffer
               ABVERTEX6L* pVertices6;
			   datasize = m_vertexCount*sizeof(ABVERTEX6L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices6, 0 )==0)
			   {//lock buffer
				   for(int loop=0;loop<m_vertexCount;loop++){
	                  pVertices6[loop].x = m_vertexArray6[loop].x;
					  pVertices6[loop].y = m_vertexArray6[loop].y;
					  pVertices6[loop].z = m_vertexArray6[loop].z;
				   }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 7:
			//-----------------------------
			//-----------------------------
            // Create a vertex buffer
               ABVERTEX7L* pVertices7;
			   datasize = m_vertexCount*sizeof(ABVERTEX7L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices7, 0 )==0)
			   {//lock buffer
				   for(int loop=0;loop<m_vertexCount;loop++){
	                  pVertices7[loop].x = m_vertexArray7[loop].x;
					  pVertices7[loop].y = m_vertexArray7[loop].y;
					  pVertices7[loop].z = m_vertexArray7[loop].z;
				   }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 8:
			//-----------------------------
			//-----------------------------
            // Create a vertex buffer
               ABVERTEX8L* pVertices8;
			   datasize = m_vertexCount*sizeof(ABVERTEX8L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices8, 0 )==0)
			   {//lock buffer
				  for(int loop=0;loop<m_vertexCount;loop++){
	                  pVertices8[loop].x = m_vertexArray8[loop].x;
					  pVertices8[loop].y = m_vertexArray8[loop].y;
					  pVertices8[loop].z = m_vertexArray8[loop].z;
				   }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;

	   }
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CAdvancedVertexObj::CloneDataOnly(CAdvancedVertexObj * clone2)
{
       clone2->m_uvCount             = m_uvCount;
	   clone2->m_indecieCount        = m_indecieCount;
	   clone2->m_vertexCount         = m_vertexCount;

       if(clone2->m_indecieArray)
		  delete [] clone2->m_indecieArray;
	      clone2->m_indecieArray = 0;

	   clone2->m_indecieArray = new WORD[m_indecieCount];
	   CopyMemory(clone2->m_indecieArray,m_indecieArray,sizeof(WORD)*m_indecieCount);
       
	   switch(m_uvCount)
	   {
	    case 0:
            if(clone2->m_vertexArray0){
			   delete [] clone2->m_vertexArray0;
			   clone2->m_vertexArray0 = 0;
			}

			clone2->m_vertexArray0 = new ABVERTEX0L[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray0,m_vertexArray0,sizeof(ABVERTEX0L)*m_vertexCount);
	    break;
	   
	    case 1:
			if(clone2->m_vertexArray1){
			   delete [] clone2->m_vertexArray1;
			   clone2->m_vertexArray1 = 0;
			}
			clone2->m_vertexArray1 = new ABVERTEX1L[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray1,m_vertexArray1,sizeof(ABVERTEX1L)*m_vertexCount);
	    break;
	   
	    case 2:
			if(clone2->m_vertexArray2){
			   delete [] clone2->m_vertexArray2;
			   clone2->m_vertexArray2 = 0;
			}
			clone2->m_vertexArray2 = new ABVERTEX2L[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray2,m_vertexArray2,sizeof(ABVERTEX2L)*m_vertexCount);
	    break;
	   
	    case 3:
			if(clone2->m_vertexArray3){
			   delete [] clone2->m_vertexArray3;
			   clone2->m_vertexArray3 = 0;
			}
			clone2->m_vertexArray3 = new ABVERTEX3L[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray3,m_vertexArray3,sizeof(ABVERTEX3L)*m_vertexCount);
	    break;

		case 4:
			if(clone2->m_vertexArray4){
			   delete [] clone2->m_vertexArray4;
			   clone2->m_vertexArray4 = 0;
			}
			clone2->m_vertexArray4 = new ABVERTEX4L[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray4,m_vertexArray4,sizeof(ABVERTEX4L)*m_vertexCount);
	    break;

		case 5:
			if(clone2->m_vertexArray5){
			   delete [] clone2->m_vertexArray5;
			   clone2->m_vertexArray5 = 0;
			}
			clone2->m_vertexArray5 = new ABVERTEX5L[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray5,m_vertexArray5,sizeof(ABVERTEX5L)*m_vertexCount);
	    break;

		case 6:
			if(clone2->m_vertexArray6){
			   delete [] clone2->m_vertexArray6;
			   clone2->m_vertexArray6 = 0;
			}
			clone2->m_vertexArray6 = new ABVERTEX6L[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray6,m_vertexArray6,sizeof(ABVERTEX6L)*m_vertexCount);
	    break;

		case 7:
			if(clone2->m_vertexArray7){
			   delete [] clone2->m_vertexArray7;
			   clone2->m_vertexArray7 = 0;
			}
			clone2->m_vertexArray7 = new ABVERTEX7L[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray7,m_vertexArray7,sizeof(ABVERTEX7L)*m_vertexCount);
	    break;

		case 8:
			if(clone2->m_vertexArray8){
			   delete [] clone2->m_vertexArray8;
			   clone2->m_vertexArray8 = 0;
			}
			clone2->m_vertexArray8 = new ABVERTEX8L[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray8,m_vertexArray8,sizeof(ABVERTEX0L)*m_vertexCount);
	    break;
	   
	   }
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CAdvancedVertexObj::CloneMinimum2(CAdvancedVertexObj * clone2,
									   LPDIRECT3DDEVICE9 g_pd3dDevice)
{
       clone2->m_uvCount             = m_uvCount;
	   clone2->m_indecieCount        = m_indecieCount;
	   clone2->m_vertexCount         = m_vertexCount;

	   if(clone2->m_indecieArray)
		  delete [] clone2->m_indecieArray;
	      clone2->m_indecieArray = 0;

	   clone2->m_indecieArray = new WORD[m_indecieCount];
	   CopyMemory(clone2->m_indecieArray,m_indecieArray,sizeof(WORD)*m_indecieCount);
       
       switch(m_uvCount)
	   {
	    case 0:
            if(clone2->m_vertexArray0){
			   delete [] clone2->m_vertexArray0;
			   clone2->m_vertexArray0 = 0;
			}

			clone2->m_vertexArray0 = new ABVERTEX0L[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray0,m_vertexArray0,sizeof(ABVERTEX0L)*m_vertexCount);
	    break;
	   
	    case 1:
			if(clone2->m_vertexArray1){
			   delete [] clone2->m_vertexArray1;
			   clone2->m_vertexArray1 = 0;
			}
			clone2->m_vertexArray1 = new ABVERTEX1L[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray1,m_vertexArray1,sizeof(ABVERTEX1L)*m_vertexCount);
	    break;
	   
	    case 2:
			if(clone2->m_vertexArray2){
			   delete [] clone2->m_vertexArray2;
			   clone2->m_vertexArray2 = 0;
			}
			clone2->m_vertexArray2 = new ABVERTEX2L[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray2,m_vertexArray2,sizeof(ABVERTEX2L)*m_vertexCount);
	    break;
	   
	    case 3:
			if(clone2->m_vertexArray3){
			   delete [] clone2->m_vertexArray3;
			   clone2->m_vertexArray3 = 0;
			}
			clone2->m_vertexArray3 = new ABVERTEX3L[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray3,m_vertexArray3,sizeof(ABVERTEX3L)*m_vertexCount);
	    break;

		case 4:
			if(clone2->m_vertexArray4){
			   delete [] clone2->m_vertexArray4;
			   clone2->m_vertexArray4 = 0;
			}
			clone2->m_vertexArray4 = new ABVERTEX4L[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray4,m_vertexArray4,sizeof(ABVERTEX4L)*m_vertexCount);
	    break;

		case 5:
			if(clone2->m_vertexArray5){
			   delete [] clone2->m_vertexArray5;
			   clone2->m_vertexArray5 = 0;
			}
			clone2->m_vertexArray5 = new ABVERTEX5L[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray5,m_vertexArray5,sizeof(ABVERTEX5L)*m_vertexCount);
	    break;

		case 6:
			if(clone2->m_vertexArray6){
			   delete [] clone2->m_vertexArray6;
			   clone2->m_vertexArray6 = 0;
			}
			clone2->m_vertexArray6 = new ABVERTEX6L[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray6,m_vertexArray6,sizeof(ABVERTEX6L)*m_vertexCount);
	    break;

		case 7:
			if(clone2->m_vertexArray7){
			   delete [] clone2->m_vertexArray7;
			   clone2->m_vertexArray7 = 0;
			}
			clone2->m_vertexArray7 = new ABVERTEX7L[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray7,m_vertexArray7,sizeof(ABVERTEX7L)*m_vertexCount);
	    break;

		case 8:
			if(clone2->m_vertexArray8){
			   delete [] clone2->m_vertexArray8;
			   clone2->m_vertexArray8 = 0;
			}
			clone2->m_vertexArray8 = new ABVERTEX8L[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray8,m_vertexArray8,sizeof(ABVERTEX0L)*m_vertexCount);
	    break;
	   
	   }

	   
	   if(g_pd3dDevice)
          InitBuffer(g_pd3dDevice,clone2->m_uvCount);     
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 void CAdvancedVertexObj::ApplyMoveTexture(float &u,
												 float &v,
												 int   textureSet,
												 float &adjustfactor,
												 int & vertexType)
{

  //return;

  if(!m_vertexBuffer)
	  return;

  /*if(vertexType == 1)
  {
     vertexType = 1;
  }*/

  switch(vertexType)
  {

    case 1:
     MoveTextureL1(u,v,textureSet,adjustfactor);
    break;
	case 2:
     MoveTextureL2(u,v,textureSet,adjustfactor);
    break;
	case 3:
     MoveTextureL3(u,v,textureSet,adjustfactor);
    break;
	case 4:
     MoveTextureL4(u,v,textureSet,adjustfactor);
    break;
	case 5:
     MoveTextureL5(u,v,textureSet,adjustfactor);
    break;
	case 6:
     MoveTextureL6(u,v,textureSet,adjustfactor);
    break;
	case 7:
     MoveTextureL7(u,v,textureSet,adjustfactor);
    break;
	case 8:
     MoveTextureL8(u,v,textureSet,adjustfactor);
    break;
	case 10:
     MoveTextureL1a(u,v,textureSet,adjustfactor);
    break;
	case 11:
     MoveTextureL2a(u,v,textureSet,adjustfactor);
    break;
  }
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 void CAdvancedVertexObj::ApplySphereAlgorithm(D3DMATRIX matWV,int & vertexType,int uvSet)
{
   //return;


   if(!m_vertexBuffer)
	   return;

      switch(this->m_uvCount)
	  {
	    case 1:
         SphereMapL1(matWV,uvSet);
        break;
		case 2:
         SphereMapL2(matWV,uvSet);
        break;
		case 3:
         SphereMapL3(matWV,uvSet);
        break;
		case 4:
         SphereMapL4(matWV,uvSet);
        break;
		case 5:
         SphereMapL5(matWV,uvSet);
        break;
		case 6:
         SphereMapL6(matWV,uvSet);
        break;
		case 7:
         SphereMapL7(matWV,uvSet);
        break;
		case 8:
         SphereMapL8(matWV,uvSet);
	    break;
		case 10:
         SphereMapL1a(matWV,uvSet);
        break;
		case 11:
         SphereMapL2a(matWV,uvSet);
        break;
		case 12:
         SphereMapL3a(matWV,uvSet);
        break;
	  }
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 void CAdvancedVertexObj::SetPositionXYZ(int & vIndex,float & x,float & y,float & z)
{ //too slow
	switch(m_uvCount)
	   {
	    case 0:
            m_vertexArray0[vIndex].x = x;
			m_vertexArray0[vIndex].y = y;
			m_vertexArray0[vIndex].z = z;
	    break;
		case 1:
			m_vertexArray1[vIndex].x = x;
			m_vertexArray1[vIndex].y = y;
			m_vertexArray1[vIndex].z = z;
	    break;
		case 2:
			m_vertexArray2[vIndex].x = x;
			m_vertexArray2[vIndex].y = y;
			m_vertexArray2[vIndex].z = z;
	    break;
		case 3:
			m_vertexArray3[vIndex].x = x;
			m_vertexArray3[vIndex].y = y;
			m_vertexArray3[vIndex].z = z;
	    break;
		case 4:
			m_vertexArray4[vIndex].x = x;
			m_vertexArray4[vIndex].y = y;
			m_vertexArray4[vIndex].z = z;
	    break;
		case 5:
			m_vertexArray5[vIndex].x = x;
			m_vertexArray5[vIndex].y = y;
			m_vertexArray5[vIndex].z = z;
	    break;
		case 6:
			m_vertexArray6[vIndex].x = x;
			m_vertexArray6[vIndex].y = y;
			m_vertexArray6[vIndex].z = z;
	    break;
		case 7:
			m_vertexArray7[vIndex].x = x;
			m_vertexArray7[vIndex].y = y;
			m_vertexArray7[vIndex].z = z;
	    break;
		case 8:
			m_vertexArray8[vIndex].x = x;
			m_vertexArray8[vIndex].y = y;
			m_vertexArray8[vIndex].z = z;
	    break;

	   }
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 void CAdvancedVertexObj::SetPositionXYZNXNYNZ(int & vIndex,
											   float & x,float & y,float & z,
											   float & nx,float & ny,float & nz)
{ //too slow
	switch(m_uvCount)
	   {
	    case 0:
            m_vertexArray0[vIndex].x = x;
			m_vertexArray0[vIndex].y = y;
			m_vertexArray0[vIndex].z = z;
			m_vertexArray0[vIndex].nx = nx;
			m_vertexArray0[vIndex].ny = ny;
			m_vertexArray0[vIndex].nz = nz;
	    break;
		case 1:
			m_vertexArray1[vIndex].x = x;
			m_vertexArray1[vIndex].y = y;
			m_vertexArray1[vIndex].z = z;
			m_vertexArray1[vIndex].nx = nx;
			m_vertexArray1[vIndex].ny = ny;
			m_vertexArray1[vIndex].nz = nz;
	    break;
		case 2:
			m_vertexArray2[vIndex].x = x;
			m_vertexArray2[vIndex].y = y;
			m_vertexArray2[vIndex].z = z;
			m_vertexArray2[vIndex].nx = nx;
			m_vertexArray2[vIndex].ny = ny;
			m_vertexArray2[vIndex].nz = nz;
	    break;
		case 3:
			m_vertexArray3[vIndex].x = x;
			m_vertexArray3[vIndex].y = y;
			m_vertexArray3[vIndex].z = z;
			m_vertexArray3[vIndex].nx = nx;
			m_vertexArray3[vIndex].ny = ny;
			m_vertexArray3[vIndex].nz = nz;
	    break;
		case 4:
			m_vertexArray4[vIndex].x = x;
			m_vertexArray4[vIndex].y = y;
			m_vertexArray4[vIndex].z = z;
			m_vertexArray4[vIndex].nx = nx;
			m_vertexArray4[vIndex].ny = ny;
			m_vertexArray4[vIndex].nz = nz;
	    break;
		case 5:
			m_vertexArray5[vIndex].x = x;
			m_vertexArray5[vIndex].y = y;
			m_vertexArray5[vIndex].z = z;
			m_vertexArray5[vIndex].nx = nx;
			m_vertexArray5[vIndex].ny = ny;
			m_vertexArray5[vIndex].nz = nz;
	    break;
		case 6:
			m_vertexArray6[vIndex].x = x;
			m_vertexArray6[vIndex].y = y;
			m_vertexArray6[vIndex].z = z;
			m_vertexArray6[vIndex].nx = nx;
			m_vertexArray6[vIndex].ny = ny;
			m_vertexArray6[vIndex].nz = nz;
	    break;
		case 7:
			m_vertexArray7[vIndex].x = x;
			m_vertexArray7[vIndex].y = y;
			m_vertexArray7[vIndex].z = z;
			m_vertexArray7[vIndex].nx = nx;
			m_vertexArray7[vIndex].ny = ny;
			m_vertexArray7[vIndex].nz = nz;
	    break;
		case 8:
			m_vertexArray8[vIndex].x = x;
			m_vertexArray8[vIndex].y = y;
			m_vertexArray8[vIndex].z = z;
			m_vertexArray8[vIndex].nx = nx;
			m_vertexArray8[vIndex].ny = ny;
			m_vertexArray8[vIndex].nz = nz;
	    break;

	   }
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 void CAdvancedVertexObj::SetPositionXYZNXNYNZMEM(int  vIndex,
											   float  x,float  y,float  z,
											   float  nx,float  ny,float  nz)
{ //too slow
	switch(m_uvCount)
	   {
	    case 0:
            m_vertexArray0[vIndex].x = x;
			m_vertexArray0[vIndex].y = y;
			m_vertexArray0[vIndex].z = z;
			m_vertexArray0[vIndex].nx = nx;
			m_vertexArray0[vIndex].ny = ny;
			m_vertexArray0[vIndex].nz = nz;
	    break;
		case 1:
			m_vertexArray1[vIndex].x = x;
			m_vertexArray1[vIndex].y = y;
			m_vertexArray1[vIndex].z = z;
			m_vertexArray1[vIndex].nx = nx;
			m_vertexArray1[vIndex].ny = ny;
			m_vertexArray1[vIndex].nz = nz;
	    break;
		case 2:
			m_vertexArray2[vIndex].x = x;
			m_vertexArray2[vIndex].y = y;
			m_vertexArray2[vIndex].z = z;
			m_vertexArray2[vIndex].nx = nx;
			m_vertexArray2[vIndex].ny = ny;
			m_vertexArray2[vIndex].nz = nz;
	    break;
		case 3:
			m_vertexArray3[vIndex].x = x;
			m_vertexArray3[vIndex].y = y;
			m_vertexArray3[vIndex].z = z;
			m_vertexArray3[vIndex].nx = nx;
			m_vertexArray3[vIndex].ny = ny;
			m_vertexArray3[vIndex].nz = nz;
	    break;
		case 4:
			m_vertexArray4[vIndex].x = x;
			m_vertexArray4[vIndex].y = y;
			m_vertexArray4[vIndex].z = z;
			m_vertexArray4[vIndex].nx = nx;
			m_vertexArray4[vIndex].ny = ny;
			m_vertexArray4[vIndex].nz = nz;
	    break;
		case 5:
			m_vertexArray5[vIndex].x = x;
			m_vertexArray5[vIndex].y = y;
			m_vertexArray5[vIndex].z = z;
			m_vertexArray5[vIndex].nx = nx;
			m_vertexArray5[vIndex].ny = ny;
			m_vertexArray5[vIndex].nz = nz;
	    break;
		case 6:
			m_vertexArray6[vIndex].x = x;
			m_vertexArray6[vIndex].y = y;
			m_vertexArray6[vIndex].z = z;
			m_vertexArray6[vIndex].nx = nx;
			m_vertexArray6[vIndex].ny = ny;
			m_vertexArray6[vIndex].nz = nz;
	    break;
		case 7:
			m_vertexArray7[vIndex].x = x;
			m_vertexArray7[vIndex].y = y;
			m_vertexArray7[vIndex].z = z;
			m_vertexArray7[vIndex].nx = nx;
			m_vertexArray7[vIndex].ny = ny;
			m_vertexArray7[vIndex].nz = nz;
	    break;
		case 8:
			m_vertexArray8[vIndex].x = x;
			m_vertexArray8[vIndex].y = y;
			m_vertexArray8[vIndex].z = z;
			m_vertexArray8[vIndex].nx = nx;
			m_vertexArray8[vIndex].ny = ny;
			m_vertexArray8[vIndex].nz = nz;
	    break;

	   }
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 void CAdvancedVertexObj::InitFromData(WORD vCount,
											 WORD indecieCount,
											 ABVERTEX * vArray,
											 WORD * indecieArray,
											 int uvCount,
											 LPDIRECT3DDEVICE9 g_pd3dDevice
											 )
{

	m_uvCount                  = uvCount;
    m_indecieCount             = indecieCount;
	m_vertexCount              = vCount;  

    m_indecieArray             = new WORD[m_indecieCount];
	//m_origVertexArray          = new ABVERTEX[m_vertexCount];
    CopyMemory(m_indecieArray,indecieArray,sizeof(WORD)*m_indecieCount);
    //CopyMemory(m_origVertexArray,vArray,sizeof(ABVERTEX)*m_vertexCount);
	int loop=0;
	switch(m_uvCount)
	   {
	    case 0:
			m_vertexArray0 = new ABVERTEX0L[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&m_vertexArray0[loop],&vArray[loop],sizeof(ABVERTEX0L));

	    break;
	   
	    case 1:
			m_vertexArray1 = new ABVERTEX1L[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&m_vertexArray1[loop],&vArray[loop],sizeof(ABVERTEX1L));
	    break;
	   
	    case 2:
			m_vertexArray2 = new ABVERTEX2L[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&m_vertexArray2[loop],&vArray[loop],sizeof(ABVERTEX2L));
	    break;
	   
	    case 3:
			m_vertexArray3 = new ABVERTEX3L[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&m_vertexArray3[loop],&vArray[loop],sizeof(ABVERTEX3L));
	    break;

		case 4:
			m_vertexArray4 = new ABVERTEX4L[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&m_vertexArray4[loop],&vArray[loop],sizeof(ABVERTEX4L));
	    break;

		case 5:
			m_vertexArray5 = new ABVERTEX5L[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&m_vertexArray5[loop],&vArray[loop],sizeof(ABVERTEX5L));
	    break;

		case 6:
			m_vertexArray6 = new ABVERTEX6L[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&m_vertexArray6[loop],&vArray[loop],sizeof(ABVERTEX6L));
	    break;

		case 7:
			m_vertexArray7 = new ABVERTEX7L[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&m_vertexArray7[loop],&vArray[loop],sizeof(ABVERTEX7L));
	    break;

		case 8:
			m_vertexArray8 = new ABVERTEX8L[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&m_vertexArray8[loop],&vArray[loop],sizeof(ABVERTEX8L));
	    break;
	   
	   }
		
	InitBuffer(g_pd3dDevice,this->m_uvCount);
}

//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 void CAdvancedVertexObj::Render(LPDIRECT3DDEVICE9 g_pd3dDevice, int & vertexType)
{
	if(!m_vertexBuffer)
		return;

	if(!m_indicieBuffer)
		return;

	if(!m_vertexCount)
		return;

    switch(vertexType)
    {
	    case 0:
	        //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX0) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX0L) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
		break;
		case 1:
            //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX1L) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
		break;
		case 2:
            //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX2L) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
		break;
		case 3:
            //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX3) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX3L) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
		break;
		case 4:
            //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX4) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX4L) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
		break;
		case 5:
            //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX5) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX5L) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
		break;
		case 6:
            //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX6) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX6L) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
		break;
		case 7:
            //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX7) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX7L) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
		break;
		case 8:
            //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX8) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX8L) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
		break;
		case 9:
            //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX8) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX0La) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
		break;
		case 10:
            //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX8) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX1La) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
		break;
		case 11:
            //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX8) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX2La) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
		break;
		case 12:
            //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX8) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX3La) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
		break;
		case 13:
            //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX8) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX4La) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
		break;
	}

}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 void CAdvancedVertexObj::InitBuffer(LPDIRECT3DDEVICE9 g_pd3dDevice, int requiredUVCount)
{
   m_lockFlags = 0;
   ReinitializeDxVbuffers(g_pd3dDevice,D3DPOOL_MANAGED,0,0,requiredUVCount);  
     
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 void CAdvancedVertexObj::InitBuffer(LPDIRECT3DDEVICE9 g_pd3dDevice,int effect, int requiredUVCount)
{
   m_lockFlags = 0;
   if(effect == 0)
      ReinitializeDxVbuffers(g_pd3dDevice,D3DPOOL_MANAGED,0,0,requiredUVCount);  
   else
      ReinitializeDxVbuffersAdvanced(g_pd3dDevice,D3DPOOL_MANAGED,0,0,effect);  
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 void CAdvancedVertexObj::InitBufferDyn(LPDIRECT3DDEVICE9 g_pd3dDevice, int requiredUVCount)
{
   m_lockFlags = 0;//D3DLOCK_DISCARD;
   ReinitializeDxVbuffers(g_pd3dDevice,
	                      D3DPOOL_MANAGED,//D3DPOOL_DEFAULT,
						  0,
						  0,
						  requiredUVCount);
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CAdvancedVertexObj::Invalidate()
{
    if(m_vertexBuffer)
	{
       m_vertexBuffer->Release();
       m_vertexBuffer = 0;
	}

	if(m_indicieBuffer){
       m_indicieBuffer->Release();
       m_indicieBuffer = 0;
	}
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CAdvancedVertexObj::SafeDelete()
{
	
    if(m_vertexBuffer)
	{
       m_vertexBuffer->Release();
       m_vertexBuffer = 0;
	}

	if(m_indicieBuffer){
       m_indicieBuffer->Release();
       m_indicieBuffer = 0;
	}

    if(m_indecieArray){
       delete [] m_indecieArray;
	   m_indecieArray = 0;
	}

    if(m_vertexArray0){
       delete [] m_vertexArray0;
	   m_vertexArray0 = 0;
	}
 
    if(m_vertexArray1){
       delete [] m_vertexArray1;
	   m_vertexArray1 = 0;
	}

	if(m_vertexArray2){
       delete [] m_vertexArray2;
	   m_vertexArray2 = 0;
	}

	if(m_vertexArray3){
       delete [] m_vertexArray3;
	   m_vertexArray3 = 0;
	}

	if(m_vertexArray4){
       delete [] m_vertexArray4;
	   m_vertexArray4= 0;
	}

	if(m_vertexArray5){
       delete [] m_vertexArray5;
	   m_vertexArray5= 0;
	}

	if(m_vertexArray6){
       delete [] m_vertexArray6;
	   m_vertexArray6= 0;
	}

	if(m_vertexArray7){
       delete [] m_vertexArray7;
	   m_vertexArray7= 0;
	}

	if(m_vertexArray8){
       delete [] m_vertexArray8;
	   m_vertexArray8= 0;
	}

}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CAdvancedVertexObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_uvCount 
		  << m_indecieCount
		  << m_vertexCount;

	   int loop=0;
	   for(loop=0;loop<m_indecieCount;loop++)
	   ar << m_indecieArray[loop];

	   switch(m_uvCount)
	   {//save specific
	    case 0:

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  << m_vertexArray0[loop].x
		        << m_vertexArray0[loop].y
		        << m_vertexArray0[loop].z
		        << m_vertexArray0[loop].nx
		        << m_vertexArray0[loop].ny
		        << m_vertexArray0[loop].nz;
			}
	      
	    break;

		case 1:

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  << m_vertexArray1[loop].x
		        << m_vertexArray1[loop].y
		        << m_vertexArray1[loop].z
		        << m_vertexArray1[loop].nx
		        << m_vertexArray1[loop].ny
		        << m_vertexArray1[loop].nz;
	      
	        ar << m_vertexArray1[loop].tx0.tu
		       << m_vertexArray1[loop].tx0.tv;
			}
		  
	    break;

		case 2:

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  << m_vertexArray2[loop].x
		        << m_vertexArray2[loop].y
		        << m_vertexArray2[loop].z
		        << m_vertexArray2[loop].nx
		        << m_vertexArray2[loop].ny
		        << m_vertexArray2[loop].nz;
	      
	        ar << m_vertexArray2[loop].tx0.tu
		       << m_vertexArray2[loop].tx0.tv;

			ar << m_vertexArray2[loop].tx1.tu
		       << m_vertexArray2[loop].tx1.tv;
			}
		  
	    break;
	   
		case 3:

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  << m_vertexArray3[loop].x
		        << m_vertexArray3[loop].y
		        << m_vertexArray3[loop].z
		        << m_vertexArray3[loop].nx
		        << m_vertexArray3[loop].ny
		        << m_vertexArray3[loop].nz;
	      
	        ar << m_vertexArray3[loop].tx0.tu
		       << m_vertexArray3[loop].tx0.tv;

			ar << m_vertexArray3[loop].tx1.tu
		       << m_vertexArray3[loop].tx1.tv;

			ar << m_vertexArray3[loop].tx2.tu
		       << m_vertexArray3[loop].tx2.tv;
			}
		  
	    break;

		case 4:

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  << m_vertexArray4[loop].x
		        << m_vertexArray4[loop].y
		        << m_vertexArray4[loop].z
		        << m_vertexArray4[loop].nx
		        << m_vertexArray4[loop].ny
		        << m_vertexArray4[loop].nz;
	      
	        ar << m_vertexArray4[loop].tx0.tu
		       << m_vertexArray4[loop].tx0.tv;

			ar << m_vertexArray4[loop].tx1.tu
		       << m_vertexArray4[loop].tx1.tv;

			ar << m_vertexArray4[loop].tx2.tu
		       << m_vertexArray4[loop].tx2.tv;

			ar << m_vertexArray4[loop].tx3.tu
		       << m_vertexArray4[loop].tx3.tv;

			}
		  
	    break;

		case 5:

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  << m_vertexArray5[loop].x
		        << m_vertexArray5[loop].y
		        << m_vertexArray5[loop].z
		        << m_vertexArray5[loop].nx
		        << m_vertexArray5[loop].ny
		        << m_vertexArray5[loop].nz;
	      
	        ar << m_vertexArray5[loop].tx0.tu
		       << m_vertexArray5[loop].tx0.tv;

			ar << m_vertexArray5[loop].tx1.tu
		       << m_vertexArray5[loop].tx1.tv;

			ar << m_vertexArray5[loop].tx2.tu
		       << m_vertexArray5[loop].tx2.tv;

			ar << m_vertexArray5[loop].tx3.tu
		       << m_vertexArray5[loop].tx3.tv;

			ar << m_vertexArray5[loop].tx4.tu
		       << m_vertexArray5[loop].tx4.tv;

			}
		  
	    break;

		case 6:

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  << m_vertexArray6[loop].x
		        << m_vertexArray6[loop].y
		        << m_vertexArray6[loop].z
		        << m_vertexArray6[loop].nx
		        << m_vertexArray6[loop].ny
		        << m_vertexArray6[loop].nz;
	      
	        ar << m_vertexArray6[loop].tx0.tu
		       << m_vertexArray6[loop].tx0.tv;

			ar << m_vertexArray6[loop].tx1.tu
		       << m_vertexArray6[loop].tx1.tv;

			ar << m_vertexArray6[loop].tx2.tu
		       << m_vertexArray6[loop].tx2.tv;

			ar << m_vertexArray6[loop].tx3.tu
		       << m_vertexArray6[loop].tx3.tv;

			ar << m_vertexArray6[loop].tx4.tu
		       << m_vertexArray6[loop].tx4.tv;

			ar << m_vertexArray6[loop].tx5.tu
		       << m_vertexArray6[loop].tx5.tv;

			}
		  
	    break;

		case 7:

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  << m_vertexArray7[loop].x
		        << m_vertexArray7[loop].y
		        << m_vertexArray7[loop].z
		        << m_vertexArray7[loop].nx
		        << m_vertexArray7[loop].ny
		        << m_vertexArray7[loop].nz;
	      
	        ar << m_vertexArray7[loop].tx0.tu
		       << m_vertexArray7[loop].tx0.tv;

			ar << m_vertexArray7[loop].tx1.tu
		       << m_vertexArray7[loop].tx1.tv;

			ar << m_vertexArray7[loop].tx2.tu
		       << m_vertexArray7[loop].tx2.tv;

			ar << m_vertexArray7[loop].tx3.tu
		       << m_vertexArray7[loop].tx3.tv;

			ar << m_vertexArray7[loop].tx4.tu
		       << m_vertexArray7[loop].tx4.tv;

			ar << m_vertexArray7[loop].tx5.tu
		       << m_vertexArray7[loop].tx5.tv;

			ar << m_vertexArray7[loop].tx6.tu
		       << m_vertexArray7[loop].tx6.tv;

			}
		  
	    break;

		case 8:

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  << m_vertexArray8[loop].x
		        << m_vertexArray8[loop].y
		        << m_vertexArray8[loop].z
		        << m_vertexArray8[loop].nx
		        << m_vertexArray8[loop].ny
		        << m_vertexArray8[loop].nz;
	      
	        ar << m_vertexArray8[loop].tx0.tu
		       << m_vertexArray8[loop].tx0.tv;

			ar << m_vertexArray8[loop].tx1.tu
		       << m_vertexArray8[loop].tx1.tv;

			ar << m_vertexArray8[loop].tx2.tu
		       << m_vertexArray8[loop].tx2.tv;

			ar << m_vertexArray8[loop].tx3.tu
		       << m_vertexArray8[loop].tx3.tv;

			ar << m_vertexArray8[loop].tx4.tu
		       << m_vertexArray8[loop].tx4.tv;

			ar << m_vertexArray8[loop].tx5.tu
		       << m_vertexArray8[loop].tx5.tv;

			ar << m_vertexArray8[loop].tx6.tu
		       << m_vertexArray8[loop].tx6.tv;

			ar << m_vertexArray8[loop].tx7.tu
		       << m_vertexArray8[loop].tx7.tv;

			}
		  
	    break;


	    
	   }//end save specific


       /*for(loop=0;loop<m_vertexCount;loop++){//store vertex array
       ar  << m_origVertexArray[loop].x
		   << m_origVertexArray[loop].y
		   << m_origVertexArray[loop].z
		   << m_origVertexArray[loop].nx
		   << m_origVertexArray[loop].ny
		   << m_origVertexArray[loop].nz;
	       if(m_uvCount >= 1){
	       ar << m_origVertexArray[loop].tx0.tu
		      << m_origVertexArray[loop].tx0.tv;
		   }
           if(m_uvCount >= 2){
		   ar << m_origVertexArray[loop].tx1.tu
		      << m_origVertexArray[loop].tx1.tv;
		   }
		   if(m_uvCount >= 3){
		   ar << m_origVertexArray[loop].tx2.tu
		      << m_origVertexArray[loop].tx2.tv;
		   }
		   if(m_uvCount >= 4){
		   ar << m_origVertexArray[loop].tx3.tu
		      << m_origVertexArray[loop].tx3.tv;
		   }
		   if(m_uvCount >= 5){
		   ar << m_origVertexArray[loop].tx4.tu
		      << m_origVertexArray[loop].tx4.tv;
		   }
		   if(m_uvCount >= 6){
		   ar << m_origVertexArray[loop].tx5.tu
		      << m_origVertexArray[loop].tx5.tv;
		   }
		   if(m_uvCount >= 7){
		   ar << m_origVertexArray[loop].tx6.tu
		      << m_origVertexArray[loop].tx6.tv;
		   }
		   if(m_uvCount >= 8){
		   ar << m_origVertexArray[loop].tx7.tu
		      << m_origVertexArray[loop].tx7.tv;
		   }
	   }//end store vertex array*/



	}
	else
	{
       ar >> m_uvCount 
		  >> m_indecieCount
		  >> m_vertexCount;

	   m_vertexArray0             = NULL;
       m_vertexArray1             = NULL;    
       m_vertexArray2             = NULL;
       m_vertexArray3             = NULL;
       m_vertexArray4             = NULL;
       m_vertexArray5             = NULL;
       m_vertexArray6             = NULL;
       m_vertexArray7             = NULL;
       m_vertexArray8             = NULL;


       if(m_indecieCount > 0)
		  m_indecieArray = new WORD[m_indecieCount];

	   int loop=0;
       for(loop=0;loop<m_indecieCount;loop++)
	   ar >> m_indecieArray[loop];

	   
	  
	   switch(m_uvCount)
	   {//save specific
	    case 0:

			if(m_vertexCount > 0)
			m_vertexArray0 = new ABVERTEX0L[m_vertexCount];

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  >> m_vertexArray0[loop].x
		        >> m_vertexArray0[loop].y
		        >> m_vertexArray0[loop].z
		        >> m_vertexArray0[loop].nx
		        >> m_vertexArray0[loop].ny
		        >> m_vertexArray0[loop].nz;
			}
	      
	    break;

		case 1:

			if(m_vertexCount > 0)
            m_vertexArray1 = new ABVERTEX1L[m_vertexCount];

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  >> m_vertexArray1[loop].x
		        >> m_vertexArray1[loop].y
		        >> m_vertexArray1[loop].z
		        >> m_vertexArray1[loop].nx
		        >> m_vertexArray1[loop].ny
		        >> m_vertexArray1[loop].nz;
	      
	        ar >> m_vertexArray1[loop].tx0.tu
		       >> m_vertexArray1[loop].tx0.tv;
			}
		  
	    break;

		case 2:

			if(m_vertexCount > 0)
            m_vertexArray2 = new ABVERTEX2L[m_vertexCount];

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  >> m_vertexArray2[loop].x
		        >> m_vertexArray2[loop].y
		        >> m_vertexArray2[loop].z
		        >> m_vertexArray2[loop].nx
		        >> m_vertexArray2[loop].ny
		        >> m_vertexArray2[loop].nz;
	      
	        ar >> m_vertexArray2[loop].tx0.tu
		       >> m_vertexArray2[loop].tx0.tv;

			ar >> m_vertexArray2[loop].tx1.tu
		       >> m_vertexArray2[loop].tx1.tv;
			}
		  
	    break;
	   
		case 3:

			if(m_vertexCount > 0)
            m_vertexArray3 = new ABVERTEX3L[m_vertexCount];

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  >> m_vertexArray3[loop].x
		        >> m_vertexArray3[loop].y
		        >> m_vertexArray3[loop].z
		        >> m_vertexArray3[loop].nx
		        >> m_vertexArray3[loop].ny
		        >> m_vertexArray3[loop].nz;
	      
	        ar >> m_vertexArray3[loop].tx0.tu
		       >> m_vertexArray3[loop].tx0.tv;

			ar >> m_vertexArray3[loop].tx1.tu
		       >> m_vertexArray3[loop].tx1.tv;

			ar >> m_vertexArray3[loop].tx2.tu
		       >> m_vertexArray3[loop].tx2.tv;
			}
		  
	    break;

		case 4:

			if(m_vertexCount > 0)
            m_vertexArray4 = new ABVERTEX4L[m_vertexCount];

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  >> m_vertexArray4[loop].x
		        >> m_vertexArray4[loop].y
		        >> m_vertexArray4[loop].z
		        >> m_vertexArray4[loop].nx
		        >> m_vertexArray4[loop].ny
		        >> m_vertexArray4[loop].nz;
	      
	        ar >> m_vertexArray4[loop].tx0.tu
		       >> m_vertexArray4[loop].tx0.tv;

			ar >> m_vertexArray4[loop].tx1.tu
		       >> m_vertexArray4[loop].tx1.tv;

			ar >> m_vertexArray4[loop].tx2.tu
		       >> m_vertexArray4[loop].tx2.tv;

			ar >> m_vertexArray4[loop].tx3.tu
		       >> m_vertexArray4[loop].tx3.tv;

			}
		  
	    break;

		case 5:
       
			if(m_vertexCount > 0)
			m_vertexArray5 = new ABVERTEX5L[m_vertexCount];

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  >> m_vertexArray5[loop].x
		        >> m_vertexArray5[loop].y
		        >> m_vertexArray5[loop].z
		        >> m_vertexArray5[loop].nx
		        >> m_vertexArray5[loop].ny
		        >> m_vertexArray5[loop].nz;
	      
	        ar >> m_vertexArray5[loop].tx0.tu
		       >> m_vertexArray5[loop].tx0.tv;

			ar >> m_vertexArray5[loop].tx1.tu
		       >> m_vertexArray5[loop].tx1.tv;

			ar >> m_vertexArray5[loop].tx2.tu
		       >> m_vertexArray5[loop].tx2.tv;

			ar >> m_vertexArray5[loop].tx3.tu
		       >> m_vertexArray5[loop].tx3.tv;

			ar >> m_vertexArray5[loop].tx4.tu
		       >> m_vertexArray5[loop].tx4.tv;

			}
		  
	    break;

		case 6:

			if(m_vertexCount > 0)
			m_vertexArray6 = new ABVERTEX6L[m_vertexCount];

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  >> m_vertexArray6[loop].x
		        >> m_vertexArray6[loop].y
		        >> m_vertexArray6[loop].z
		        >> m_vertexArray6[loop].nx
		        >> m_vertexArray6[loop].ny
		        >> m_vertexArray6[loop].nz;
	      
	        ar >> m_vertexArray6[loop].tx0.tu
		       >> m_vertexArray6[loop].tx0.tv;

			ar >> m_vertexArray6[loop].tx1.tu
		       >> m_vertexArray6[loop].tx1.tv;

			ar >> m_vertexArray6[loop].tx2.tu
		       >> m_vertexArray6[loop].tx2.tv;

			ar >> m_vertexArray6[loop].tx3.tu
		       >> m_vertexArray6[loop].tx3.tv;

			ar >> m_vertexArray6[loop].tx4.tu
		       >> m_vertexArray6[loop].tx4.tv;

			ar >> m_vertexArray6[loop].tx5.tu
		       >> m_vertexArray6[loop].tx5.tv;

			}
		  
	    break;

		case 7:

			if(m_vertexCount > 0)
			m_vertexArray7 = new ABVERTEX7L[m_vertexCount];

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  >> m_vertexArray7[loop].x
		        >> m_vertexArray7[loop].y
		        >> m_vertexArray7[loop].z
		        >> m_vertexArray7[loop].nx
		        >> m_vertexArray7[loop].ny
		        >> m_vertexArray7[loop].nz;
	      
	        ar >> m_vertexArray7[loop].tx0.tu
		       >> m_vertexArray7[loop].tx0.tv;

			ar >> m_vertexArray7[loop].tx1.tu
		       >> m_vertexArray7[loop].tx1.tv;

			ar >> m_vertexArray7[loop].tx2.tu
		       >> m_vertexArray7[loop].tx2.tv;

			ar >> m_vertexArray7[loop].tx3.tu
		       >> m_vertexArray7[loop].tx3.tv;

			ar >> m_vertexArray7[loop].tx4.tu
		       >> m_vertexArray7[loop].tx4.tv;

			ar >> m_vertexArray7[loop].tx5.tu
		       >> m_vertexArray7[loop].tx5.tv;

			ar >> m_vertexArray7[loop].tx6.tu
		       >> m_vertexArray7[loop].tx6.tv;

			}
		  
	    break;

		case 8:

			if(m_vertexCount > 0)
			m_vertexArray8 = new ABVERTEX8L[m_vertexCount];

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  >> m_vertexArray8[loop].x
		        >> m_vertexArray8[loop].y
		        >> m_vertexArray8[loop].z
		        >> m_vertexArray8[loop].nx
		        >> m_vertexArray8[loop].ny
		        >> m_vertexArray8[loop].nz;
	      
	        ar >> m_vertexArray8[loop].tx0.tu
		       >> m_vertexArray8[loop].tx0.tv;

			ar >> m_vertexArray8[loop].tx1.tu
		       >> m_vertexArray8[loop].tx1.tv;

			ar >> m_vertexArray8[loop].tx2.tu
		       >> m_vertexArray8[loop].tx2.tv;

			ar >> m_vertexArray8[loop].tx3.tu
		       >> m_vertexArray8[loop].tx3.tv;

			ar >> m_vertexArray8[loop].tx4.tu
		       >> m_vertexArray8[loop].tx4.tv;

			ar >> m_vertexArray8[loop].tx5.tu
		       >> m_vertexArray8[loop].tx5.tv;

			ar >> m_vertexArray8[loop].tx6.tu
		       >> m_vertexArray8[loop].tx6.tv;

			ar >> m_vertexArray8[loop].tx7.tu
		       >> m_vertexArray8[loop].tx7.tv;

			}
		  
	    break;


	    
	   }//end save specific
	  
	   
	   /*if(m_vertexCount > 0)
          m_origVertexArray = new ABVERTEX[m_vertexCount];

       for(loop=0;loop<m_vertexCount;loop++){//store vertex array
       ar  >> m_origVertexArray[loop].x
		   >> m_origVertexArray[loop].y
		   >> m_origVertexArray[loop].z
		   >> m_origVertexArray[loop].nx
		   >> m_origVertexArray[loop].ny
		   >> m_origVertexArray[loop].nz;
	       if(m_uvCount >= 1){
	       ar >> m_origVertexArray[loop].tx0.tu
		      >> m_origVertexArray[loop].tx0.tv;
		   }
		   else{
              m_origVertexArray[loop].tx0.tu = 0.0f;
		      m_origVertexArray[loop].tx0.tv = 0.0f;
		   }
           if(m_uvCount >= 2){
		   ar >> m_origVertexArray[loop].tx1.tu
		      >> m_origVertexArray[loop].tx1.tv;
		   }
		   else{
              m_origVertexArray[loop].tx1.tu = 0.0f;
		      m_origVertexArray[loop].tx1.tv = 0.0f;
		   }
		   if(m_uvCount >= 3){
		   ar >> m_origVertexArray[loop].tx2.tu
		      >> m_origVertexArray[loop].tx2.tv;
		   }
		   else{
              m_origVertexArray[loop].tx2.tu = 0.0f;
		      m_origVertexArray[loop].tx2.tv = 0.0f;
		   }
		   if(m_uvCount >= 4){
		   ar >> m_origVertexArray[loop].tx3.tu
		      >> m_origVertexArray[loop].tx3.tv;
		   }
		   else{
              m_origVertexArray[loop].tx3.tu = 0.0f;
		      m_origVertexArray[loop].tx3.tv = 0.0f;
		   }
		   if(m_uvCount >= 5){
		   ar >> m_origVertexArray[loop].tx4.tu
		      >> m_origVertexArray[loop].tx4.tv;
		   }
		   else{
              m_origVertexArray[loop].tx4.tu = 0.0f;
		      m_origVertexArray[loop].tx4.tv = 0.0f;
		   }
		   if(m_uvCount >= 6){
		   ar >> m_origVertexArray[loop].tx5.tu
		      >> m_origVertexArray[loop].tx5.tv;
		   }
		   else{
              m_origVertexArray[loop].tx5.tu = 0.0f;
		      m_origVertexArray[loop].tx5.tv = 0.0f;
		   }
		   if(m_uvCount >= 7){
		   ar >> m_origVertexArray[loop].tx6.tu
		      >> m_origVertexArray[loop].tx6.tv;
		   }
		   else{
              m_origVertexArray[loop].tx6.tu = 0.0f;
		      m_origVertexArray[loop].tx6.tv = 0.0f;
		   }
		   if(m_uvCount >= 8){
		   ar >> m_origVertexArray[loop].tx7.tu
		      >> m_origVertexArray[loop].tx7.tv;
		   }
		   else{
              m_origVertexArray[loop].tx7.tu = 0.0f;
		      m_origVertexArray[loop].tx7.tv = 0.0f;
		   }
	   }//end store vertex array*/

	   m_vertexBuffer       = NULL;
	   m_indicieBuffer      = NULL;
	   m_lockFlags          = 0;
	   m_advancedFixedMode  = 0;
       //InitBuffer();
	}
}    
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//move texture coords
 void CAdvancedVertexObj::SphereMapL1(D3DMATRIX matWV,int uvSetIndex)
{
      ABVERTEX1L* pVertices = NULL;
	  long datasize = m_vertexCount*sizeof(ABVERTEX1L);
      if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx0.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx0.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//move texture coords
 void CAdvancedVertexObj::SphereMapL1a(D3DMATRIX matWV,int uvSetIndex)
{
      ABVERTEX1La* pVertices = NULL;
	  long datasize = m_vertexCount*sizeof(ABVERTEX1La);
      if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx0.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx0.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//move texture coords
 void CAdvancedVertexObj::SphereMapL2(D3DMATRIX matWV,int uvSetIndex)
{
	
      ABVERTEX2L* pVertices = NULL;
	  long datasize = m_vertexCount*sizeof(ABVERTEX2L);

	  switch(uvSetIndex)
	  {

	  case 0:
      if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx0.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx0.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer
	  break;
	  case 1:
      if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx1.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx1.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer
	  break;


	  }
} 
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//move texture coords
 void CAdvancedVertexObj::SphereMapL2a(D3DMATRIX matWV,int uvSetIndex)
{
	
      ABVERTEX2La* pVertices = NULL;
	  long datasize = m_vertexCount*sizeof(ABVERTEX2La);
      
	  switch(uvSetIndex)
	  {
	  case 0:
	  if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx0.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx0.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer
	  break;
      case 1:
	  if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx1.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx1.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer
	  break;
	  }
} 
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//move texture coords
 void CAdvancedVertexObj::SphereMapL3(D3DMATRIX matWV,int uvSetIndex)
{
	
      ABVERTEX3L* pVertices = NULL;
	  long datasize = m_vertexCount*sizeof(ABVERTEX3L);
	  switch(uvSetIndex)
	  {
	  case 0:
      if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx0.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx0.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer
	  break;
	  case 1:
	  if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx1.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx1.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer
	  break;
	  case 2:
	  if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx2.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx2.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer
	  break;
	  }
} 
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//move texture coords
 void CAdvancedVertexObj::SphereMapL3a(D3DMATRIX matWV,int uvSetIndex)
{
	
      ABVERTEX3La* pVertices = NULL;
	  long datasize = m_vertexCount*sizeof(ABVERTEX3La);
	  switch(uvSetIndex)
	  {
	  case 0:
      if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx0.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx0.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer
	  break;
	  case 1:
	  if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx1.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx1.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer
	  break;
	  case 2:
	  if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx2.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx2.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer
	  break;
	  }
} 
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//move texture coords
 void CAdvancedVertexObj::SphereMapL4(D3DMATRIX matWV,int uvSetIndex)
{
  	  ABVERTEX4L* pVertices = NULL;
	  long datasize = m_vertexCount*sizeof(ABVERTEX4L);
      switch(uvSetIndex)
	  {
	  case 0:
      if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx0.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx0.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer
	  break;
	  case 1:
	  if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx1.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx1.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer
	  break;
	  case 2:
	  if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx2.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx2.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer
	  break;
	  case 3:
	  if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx3.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx3.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer
	  break;
	  }

      /*if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx0.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx0.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer*/
} 
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//move texture coords
 void CAdvancedVertexObj::SphereMapL5(D3DMATRIX matWV,int uvSetIndex)
{
	  ABVERTEX5L* pVertices = NULL;
	  long datasize = m_vertexCount*sizeof(ABVERTEX5L);
      if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx0.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx0.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer
} 
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//move texture coords
 void CAdvancedVertexObj::SphereMapL6(D3DMATRIX matWV,int uvSetIndex)
{
	  ABVERTEX6L* pVertices = NULL;
	  long datasize = m_vertexCount*sizeof(ABVERTEX6L);
      if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx0.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx0.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer
} 
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//move texture coords
 void CAdvancedVertexObj::SphereMapL7(D3DMATRIX matWV,int uvSetIndex)
{
	  ABVERTEX7L* pVertices = NULL;
	  long datasize = m_vertexCount*sizeof(ABVERTEX7L);
      if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx0.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx0.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer
} 
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//move texture coords
 void CAdvancedVertexObj::SphereMapL8(D3DMATRIX matWV,int uvSetIndex)
{
	  ABVERTEX8L* pVertices = NULL;
	  long datasize = m_vertexCount*sizeof(ABVERTEX8L);
      if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
	  {//lock buffer
		  for(int loop=0;loop<m_vertexCount;loop++){
             if( pVertices[loop].nx*matWV._13 + pVertices[loop].ny*matWV._23 + pVertices[loop].nz*matWV._33 > 0.0f )
              continue;
		  
		     // Assign the spheremap's texture coordinates
             pVertices[loop].tx0.tu = 0.5f * ( 1.0f + ( pVertices[loop].nx*matWV._11 + pVertices[loop].ny*matWV._21 + pVertices[loop].nz*matWV._31 ) );
             pVertices[loop].tx0.tv = 0.5f * ( 1.0f - ( pVertices[loop].nx*matWV._12 + pVertices[loop].ny*matWV._22 + pVertices[loop].nz*matWV._32 ) );
		  }
          m_vertexBuffer->Unlock();
	  }//end lock beffer
} 
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
 void CAdvancedVertexObj::MoveTextureL1(float &u,
									          float &v,
									          int &textureSet,
									          float &adjustfactor)
{


  BOOL resetU = FALSE;
  BOOL resetV = FALSE;
   
  ABVERTEX1L* pVertices = NULL;
  long datasize = m_vertexCount*sizeof(ABVERTEX1L);
  if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
  {//lock buffer
	 for(int loop=0;loop<m_vertexCount;loop++){
		 switch(textureSet){//begin
		  case 0:
		   for(loop = 0;loop < m_vertexCount;loop++){
		     pVertices[loop].tx0.tu += (u * adjustfactor);
             pVertices[loop].tx0.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx0,m_vertexArray1[0].tx0,resetU,resetV);		   
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx0.tu =  m_vertexArray1[loop].tx0.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx0.tv =  m_vertexArray1[loop].tx0.tv;
		    
		   break;
		 }//end
	 }
     m_vertexBuffer->Unlock();
  }//end lock beffer	     
}
//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
 void CAdvancedVertexObj::MoveTextureL1a(float &u,
									          float &v,
									          int &textureSet,
									          float &adjustfactor)
{


  BOOL resetU = FALSE;
  BOOL resetV = FALSE;
   
  ABVERTEX1La* pVertices = NULL;
  long datasize = m_vertexCount*sizeof(ABVERTEX1La);
  if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
  {//lock buffer
	 for(int loop=0;loop<m_vertexCount;loop++){
		 switch(textureSet){//begin
		  case 0:
		   for(loop = 0;loop < m_vertexCount;loop++){
		     pVertices[loop].tx0.tu += (u * adjustfactor);
             pVertices[loop].tx0.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx0,m_vertexArray1[0].tx0,resetU,resetV);		   
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx0.tu =  m_vertexArray1[loop].tx0.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx0.tv =  m_vertexArray1[loop].tx0.tv;
		    
		   break;
		 }//end
	 }
     m_vertexBuffer->Unlock();
  }//end lock beffer	     
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//move texture coords
 void CAdvancedVertexObj::MoveTextureL2(float &u,
									  float &v,
									  int &textureSet,
									  float &adjustfactor)
{
  BOOL resetU = FALSE;
  BOOL resetV = FALSE;

  ABVERTEX2L* pVertices = NULL;
  long datasize = m_vertexCount*sizeof(ABVERTEX2L);
  if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
  {//lock buffer

	     int loop;
		 switch(textureSet){//begin
		  case 0:
		   for(loop = 0;loop < m_vertexCount;loop++){
		     pVertices[loop].tx0.tu += (u * adjustfactor);
             pVertices[loop].tx0.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx0,m_vertexArray2[0].tx0,resetU,resetV);
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx0.tu =  m_vertexArray2[loop].tx0.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx0.tv =  m_vertexArray2[loop].tx0.tv;
		   
		  break;
		  case 1:
		   for(loop = 0;loop < m_vertexCount;loop++){
		      pVertices[loop].tx1.tu += (u * adjustfactor);
              pVertices[loop].tx1.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx1,m_vertexArray2[0].tx1,resetU,resetV);
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx1.tu =  m_vertexArray2[loop].tx1.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx1.tv =  m_vertexArray2[loop].tx1.tv;
		  break;
		 }//end
   
     m_vertexBuffer->Unlock();
  }//end lock beffer	   
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//move texture coords
 void CAdvancedVertexObj::MoveTextureL2a(float &u,
									           float &v,
									           int &textureSet,
									           float &adjustfactor)
{
  BOOL resetU = FALSE;
  BOOL resetV = FALSE;

  ABVERTEX2La* pVertices = NULL;
  long datasize = m_vertexCount*sizeof(ABVERTEX2La);
  if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
  {//lock buffer

	     int loop;
		 switch(textureSet){//begin
		  case 0:
		   for(loop = 0;loop < m_vertexCount;loop++){
		     pVertices[loop].tx0.tu += (u * adjustfactor);
             pVertices[loop].tx0.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx0,m_vertexArray2[0].tx0,resetU,resetV);
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx0.tu =  m_vertexArray2[loop].tx0.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx0.tv =  m_vertexArray2[loop].tx0.tv;
		   
		  break;
		  case 1:
		   for(loop = 0;loop < m_vertexCount;loop++){
		      pVertices[loop].tx1.tu += (u * adjustfactor);
              pVertices[loop].tx1.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx1,m_vertexArray2[0].tx1,resetU,resetV);
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx1.tu =  m_vertexArray2[loop].tx1.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx1.tv =  m_vertexArray2[loop].tx1.tv;
		  break;
		 }//end
   
     m_vertexBuffer->Unlock();
  }//end lock beffer	   
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//move texture coords
 void CAdvancedVertexObj::MoveTextureL3(float &u,
									          float &v,
									          int &textureSet,
									          float &adjustfactor)
{
  BOOL resetU = FALSE;
  BOOL resetV = FALSE;

  ABVERTEX3L* pVertices = NULL;
  long datasize = m_vertexCount*sizeof(ABVERTEX3L);
  if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
  {//lock buffer

	     int loop;
		 switch(textureSet){//begin
		  case 0:
		   for(loop = 0;loop < m_vertexCount;loop++){
		     pVertices[loop].tx0.tu += (u * adjustfactor);
             pVertices[loop].tx0.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx0,m_vertexArray3[0].tx0,resetU,resetV);
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx0.tu =  m_vertexArray3[loop].tx0.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx0.tv =  m_vertexArray3[loop].tx0.tv;
		   
		  break;
		  case 1:
		   for(loop = 0;loop < m_vertexCount;loop++){
		      pVertices[loop].tx1.tu += (u * adjustfactor);
              pVertices[loop].tx1.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx1,m_vertexArray3[0].tx1,resetU,resetV);
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx1.tu =  m_vertexArray3[loop].tx1.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx1.tv =  m_vertexArray3[loop].tx1.tv;
		  break;
		 }//end

	m_vertexBuffer->Unlock();
  }//end lock beffer	
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//move texture coords
 void CAdvancedVertexObj::MoveTextureL4(float &u,
									          float &v,
									          int &textureSet,
									          float &adjustfactor)
{
  BOOL resetU = FALSE;
  BOOL resetV = FALSE;

  ABVERTEX4L* pVertices = NULL;
  long datasize = m_vertexCount*sizeof(ABVERTEX4L);
  if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
  {//lock buffer

	     int loop;
		 switch(textureSet){//begin
		  case 0:
		   for(loop = 0;loop < m_vertexCount;loop++){
		     pVertices[loop].tx0.tu += (u * adjustfactor);
             pVertices[loop].tx0.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx0,m_vertexArray4[0].tx0,resetU,resetV);
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx0.tu =  m_vertexArray4[loop].tx0.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx0.tv =  m_vertexArray4[loop].tx0.tv;
		   
		  break;
		  case 1:
		   for(loop = 0;loop < m_vertexCount;loop++){
		      pVertices[loop].tx1.tu += (u * adjustfactor);
              pVertices[loop].tx1.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx1,m_vertexArray4[0].tx1,resetU,resetV);
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx1.tu =  m_vertexArray4[loop].tx1.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx1.tv =  m_vertexArray4[loop].tx1.tv;
		  break;
		 }//end

		m_vertexBuffer->Unlock();
  }//end lock beffer	 
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//move texture coords
 void CAdvancedVertexObj::MoveTextureL5(float &u,
									          float &v,
									          int &textureSet,
									          float &adjustfactor)
{
  BOOL resetU = FALSE;
  BOOL resetV = FALSE;

  ABVERTEX5L* pVertices = NULL;
  long datasize = m_vertexCount*sizeof(ABVERTEX5L);
  if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
  {//lock buffer

	     int loop;
		 switch(textureSet){//begin
		  case 0:
		   for(loop = 0;loop < m_vertexCount;loop++){
		     pVertices[loop].tx0.tu += (u * adjustfactor);
             pVertices[loop].tx0.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx0,m_vertexArray5[0].tx0,resetU,resetV);
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx0.tu =  m_vertexArray5[loop].tx0.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx0.tv =  m_vertexArray5[loop].tx0.tv;
		   
		  break;
		  case 1:
		   for(loop = 0;loop < m_vertexCount;loop++){
		      pVertices[loop].tx1.tu += (u * adjustfactor);
              pVertices[loop].tx1.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx1,m_vertexArray5[0].tx1,resetU,resetV);
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx1.tu =  m_vertexArray5[loop].tx1.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx1.tv =  m_vertexArray5[loop].tx1.tv;
		  break;
		 }//end

  	  m_vertexBuffer->Unlock();
  }//end lock beffer	
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//move texture coords
 void CAdvancedVertexObj::MoveTextureL6(float &u,
									          float &v,
									          int &textureSet,
									          float &adjustfactor)
{
  BOOL resetU = FALSE;
  BOOL resetV = FALSE;

  ABVERTEX6L* pVertices = NULL;
  long datasize = m_vertexCount*sizeof(ABVERTEX6L);
  if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
  {//lock buffer

	     int loop;
		 switch(textureSet){//begin
		  case 0:
		   for(loop = 0;loop < m_vertexCount;loop++){
		     pVertices[loop].tx0.tu += (u * adjustfactor);
             pVertices[loop].tx0.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx0,m_vertexArray6[0].tx0,resetU,resetV);
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx0.tu =  m_vertexArray6[loop].tx0.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx0.tv =  m_vertexArray6[loop].tx0.tv;
		   
		  break;
		  case 1:
		   for(loop = 0;loop < m_vertexCount;loop++){
		      pVertices[loop].tx1.tu += (u * adjustfactor);
              pVertices[loop].tx1.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx1,m_vertexArray6[0].tx1,resetU,resetV);
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx1.tu =  m_vertexArray6[loop].tx1.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx1.tv =  m_vertexArray6[loop].tx1.tv;
		  break;
		 }//end
	 m_vertexBuffer->Unlock();
  }//end lock beffer	
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//move texture coords
 void CAdvancedVertexObj::MoveTextureL7(float &u,
									          float &v,
									          int &textureSet,
									          float &adjustfactor)
{
  BOOL resetU = FALSE;
  BOOL resetV = FALSE;

  ABVERTEX7L* pVertices = NULL;
  long datasize = m_vertexCount*sizeof(ABVERTEX7L);
  if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
  {//lock buffer

	     int loop;
		 switch(textureSet){//begin
		  case 0:
		   for(loop = 0;loop < m_vertexCount;loop++){
		     pVertices[loop].tx0.tu += (u * adjustfactor);
             pVertices[loop].tx0.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx0,m_vertexArray7[0].tx0,resetU,resetV);
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx0.tu =  m_vertexArray7[loop].tx0.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx0.tv =  m_vertexArray7[loop].tx0.tv;
		   
		  break;
		  case 1:
		   for(loop = 0;loop < m_vertexCount;loop++){
		      pVertices[loop].tx1.tu += (u * adjustfactor);
              pVertices[loop].tx1.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx1,m_vertexArray7[0].tx1,resetU,resetV);
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx1.tu =  m_vertexArray7[loop].tx1.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx1.tv =  m_vertexArray7[loop].tx1.tv;
		  break;
		 }//end

	m_vertexBuffer->Unlock();
  }//end lock beffer	
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//move texture coords
 void CAdvancedVertexObj::MoveTextureL8(float &u,
									          float &v,
									          int &textureSet,
									          float &adjustfactor)
{
  BOOL resetU = FALSE;
  BOOL resetV = FALSE;

  ABVERTEX8L* pVertices = NULL;
  long datasize = m_vertexCount*sizeof(ABVERTEX8L);
  if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, 0 )==0)
  {//lock buffer


	     int loop;
		 switch(textureSet){//begin
		  case 0:
		   for(loop = 0;loop < m_vertexCount;loop++){
		     pVertices[loop].tx0.tu += (u * adjustfactor);
             pVertices[loop].tx0.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx0,m_vertexArray8[0].tx0,resetU,resetV);
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx0.tu =  m_vertexArray8[loop].tx0.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx0.tv =  m_vertexArray8[loop].tx0.tv;
		   
		  break;
		  case 1:
		   for(loop = 0;loop < m_vertexCount;loop++){
		      pVertices[loop].tx1.tu += (u * adjustfactor);
              pVertices[loop].tx1.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx1,m_vertexArray8[0].tx1,resetU,resetV);
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx1.tu =  m_vertexArray8[loop].tx1.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx1.tv =  m_vertexArray8[loop].tx1.tv;
		  break;
		 }//end

     m_vertexBuffer->Unlock();
  }//end lock beffer	
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
 void CAdvancedVertexObj::MoveTextureReset(float &u,
									             float &v,
									             float &adjustfactor,
									             TXTUV &uvStruct,
									             TXTUV &origUvStruct,
									             BOOL  &resetU,
									             BOOL  &resetV)
{
		 if(!resetV){
		   if(uvStruct.tv > (origUvStruct.tv+1.0f))
		    resetV = TRUE;
		   if(uvStruct.tv < (origUvStruct.tv-1.0f))
		    resetV = TRUE;
		 }
		 if(!resetU){
		   if(uvStruct.tu > (origUvStruct.tu+1.0f))
		    resetU = TRUE;
		   if(uvStruct.tu < (origUvStruct.tu-1.0f))
	        resetU = TRUE;   
		 }
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CAdvancedVertexObjList::SafeDelete()
{
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {
       CAdvancedVertexObj * vObj = (CAdvancedVertexObj *)GetNext(posLoc); 
       vObj->SafeDelete();
	   delete vObj;
	   vObj=0;
   }
   RemoveAll();
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
/*void CAdvancedVertexObjList::Render(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {
       CAdvancedVertexObj * vObj = (CAdvancedVertexObj *)GetNext(posLoc); 
       vObj->Render(g_pd3dDevice);
   }
}*/


IMPLEMENT_SERIAL(CAdvancedVertexObj, CObject, 0)
IMPLEMENT_SERIAL(CAdvancedVertexObjList, CObList, 0)

