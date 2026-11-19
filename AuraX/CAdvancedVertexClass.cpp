#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <d3dx9.h>
#include <afxtempl.h>
#include "CAdvancedVertexClass.h"
#include "matrixarb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAdvancedVertexObj::CAdvancedVertexObj()
{
    m_uvCount                  = 0;
    m_vertexArray0             = NULL;
    m_vertexArray1             = NULL;    
    m_vertexArray2             = NULL;
    m_vertexArray3             = NULL;
    m_vertexArray4             = NULL;
    m_vertexArray5             = NULL;
    m_vertexArray6             = NULL;
    m_vertexArray7             = NULL;
    m_vertexArray8             = NULL;
	m_vertexArray0t            = NULL;
    m_vertexArray1t            = NULL;    
    m_vertexArray2t            = NULL;
    m_vertexArray3t            = NULL;
    m_vertexArray4t            = NULL;
    m_vertexArray5t            = NULL;
    m_vertexArray6t            = NULL;
  
	//buffers
    m_vertexArray1x            = NULL;
	//fixed data
    m_indecieArray             = NULL;
    m_indecieCount             = 0;
	m_vertexCount              = 0; 

	m_vertexBuffer             = NULL;
	m_indicieBuffer            = NULL;

	m_advancedFixedMode        = 0;

	m_lockFlags = 0;

	m_tangentExists = 0;

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

	if(m_vertexArray0)
	{
       delete [] m_vertexArray0;
	   m_vertexArray0 = 0;
	}
	if(m_vertexArray1)
	{
       delete [] m_vertexArray1;
	   m_vertexArray1 = 0;
	}
	if(m_vertexArray2)
	{
       delete [] m_vertexArray2;
	   m_vertexArray2 = 0;
	}
	if(m_vertexArray3)
	{
       delete [] m_vertexArray3;
	   m_vertexArray3 = 0;
	}
	if(m_vertexArray4)
	{
       delete [] m_vertexArray4;
	   m_vertexArray4 = 0;
	}
	if(m_vertexArray5)
	{
       delete [] m_vertexArray5;
	   m_vertexArray5 = 0;
	}
	if(m_vertexArray6)
	{
       delete [] m_vertexArray6;
	   m_vertexArray6 = 0;
	}
	if(m_vertexArray7)
	{
       delete [] m_vertexArray7;
	   m_vertexArray7 = 0;
	}
	if(m_vertexArray8)
	{
       delete [] m_vertexArray8;
	   m_vertexArray8 = 0;
	}
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
BOOL CAdvancedVertexObj::IntegrityCheck()
{

     int oldICount = m_indecieCount;

     int expanded = m_indecieCount / 3;
	 m_indecieCount = expanded * 3;

     if(oldICount != m_indecieCount)
	    return TRUE;

	 return FALSE;
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
int CAdvancedVertexObj::GetPolyCount()
{
    return m_indecieCount/3;
}
//-----------------------------------------------------------------------------//
void CAdvancedVertexObj::GetPolygon(int & indecieStart,D3DVECTOR * poly)
{
       switch(m_uvCount)
	   {
	    case 0:
			CopyMemory(&poly[0],&m_vertexArray0[m_indecieArray[indecieStart]],sizeof(D3DVECTOR));
			CopyMemory(&poly[1],&m_vertexArray0[m_indecieArray[indecieStart+1]],sizeof(D3DVECTOR));
			CopyMemory(&poly[2],&m_vertexArray0[m_indecieArray[indecieStart+2]],sizeof(D3DVECTOR));
		break;
	   
	    case 1:
		    CopyMemory(&poly[0],&m_vertexArray1[m_indecieArray[indecieStart]],sizeof(D3DVECTOR));
			CopyMemory(&poly[1],&m_vertexArray1[m_indecieArray[indecieStart+1]],sizeof(D3DVECTOR));
			CopyMemory(&poly[2],&m_vertexArray1[m_indecieArray[indecieStart+2]],sizeof(D3DVECTOR));
		break;
	   
	    case 2:
			CopyMemory(&poly[0],&m_vertexArray2[m_indecieArray[indecieStart]],sizeof(D3DVECTOR));
			CopyMemory(&poly[1],&m_vertexArray2[m_indecieArray[indecieStart+1]],sizeof(D3DVECTOR));
			CopyMemory(&poly[2],&m_vertexArray2[m_indecieArray[indecieStart+2]],sizeof(D3DVECTOR));
	    break;
	   
	    case 3:
			CopyMemory(&poly[0],&m_vertexArray3[m_indecieArray[indecieStart]],sizeof(D3DVECTOR));
			CopyMemory(&poly[1],&m_vertexArray3[m_indecieArray[indecieStart+1]],sizeof(D3DVECTOR));
			CopyMemory(&poly[2],&m_vertexArray3[m_indecieArray[indecieStart+2]],sizeof(D3DVECTOR));
	    break;

		case 4:
			CopyMemory(&poly[0],&m_vertexArray4[m_indecieArray[indecieStart]],sizeof(D3DVECTOR));
			CopyMemory(&poly[1],&m_vertexArray4[m_indecieArray[indecieStart+1]],sizeof(D3DVECTOR));
			CopyMemory(&poly[2],&m_vertexArray4[m_indecieArray[indecieStart+2]],sizeof(D3DVECTOR));
	    break;

		case 5:
			CopyMemory(&poly[0],&m_vertexArray5[m_indecieArray[indecieStart]],sizeof(D3DVECTOR));
			CopyMemory(&poly[1],&m_vertexArray5[m_indecieArray[indecieStart+1]],sizeof(D3DVECTOR));
			CopyMemory(&poly[2],&m_vertexArray5[m_indecieArray[indecieStart+2]],sizeof(D3DVECTOR));
	    break;

		case 6:
			CopyMemory(&poly[0],&m_vertexArray6[m_indecieArray[indecieStart]],sizeof(D3DVECTOR));
			CopyMemory(&poly[1],&m_vertexArray6[m_indecieArray[indecieStart+1]],sizeof(D3DVECTOR));
			CopyMemory(&poly[2],&m_vertexArray6[m_indecieArray[indecieStart+2]],sizeof(D3DVECTOR));
	    break;

		case 7:
			CopyMemory(&poly[0],&m_vertexArray7[m_indecieArray[indecieStart]],sizeof(D3DVECTOR));
			CopyMemory(&poly[1],&m_vertexArray7[m_indecieArray[indecieStart+1]],sizeof(D3DVECTOR));
			CopyMemory(&poly[2],&m_vertexArray7[m_indecieArray[indecieStart+2]],sizeof(D3DVECTOR));
	    break;

		case 8:
			CopyMemory(&poly[0],&m_vertexArray8[m_indecieArray[indecieStart]],sizeof(D3DVECTOR));
			CopyMemory(&poly[1],&m_vertexArray8[m_indecieArray[indecieStart+1]],sizeof(D3DVECTOR));
			CopyMemory(&poly[2],&m_vertexArray8[m_indecieArray[indecieStart+2]],sizeof(D3DVECTOR));
	    break;
	   
	   }
}
//-----------------------------------------------------------------------------//
BOOL CAdvancedVertexObj::GetVertexTraverseData(byte ** vBufferData, 
											   int * XYZstep, 
											   int * vertexStep,
											   long * datasize)
{
       switch(m_uvCount)
	   {
	    case 0:
			*vBufferData = (byte*)m_vertexArray0;
            *XYZstep     = sizeof(float);
			*vertexStep  = sizeof(ABVERTEX0L);
			*datasize    = sizeof(ABVERTEX0L) * m_vertexCount;
			return TRUE;
		break;
	   
	    case 1:
		    *vBufferData = (byte*)m_vertexArray1;
            *XYZstep     = sizeof(float);
			*vertexStep  = sizeof(ABVERTEX1L);	
			return TRUE;
		break;
	   
	    case 2:
			*vBufferData = (byte*)m_vertexArray2;
            *XYZstep     = sizeof(float);
			*vertexStep  = sizeof(ABVERTEX2L);
			return TRUE;
	    break;
	   
	    case 3:
			*vBufferData = (byte*)m_vertexArray3;
            *XYZstep     = sizeof(float);
			*vertexStep  = sizeof(ABVERTEX3L);	
			return TRUE;
	    break;

		case 4:
			*vBufferData = (byte*)m_vertexArray4;
            *XYZstep     = sizeof(float);
			*vertexStep  = sizeof(ABVERTEX4L);
			return TRUE;
	    break;

		case 5:
			*vBufferData = (byte*)m_vertexArray5;
            *XYZstep     = sizeof(float);
			*vertexStep  = sizeof(ABVERTEX5L);
			return TRUE;
	    break;

		case 6:
			*vBufferData = (byte*)m_vertexArray6;
            *XYZstep     = sizeof(float);
			*vertexStep  = sizeof(ABVERTEX6L);
			return TRUE;
	    break;

		case 7:
			*vBufferData = (byte*)m_vertexArray7;
            *XYZstep     = sizeof(float);
			*vertexStep  = sizeof(ABVERTEX7L);
			return TRUE;
	    break;

		case 8:
			*vBufferData = (byte*)m_vertexArray8;
            *XYZstep     = sizeof(float);
			*vertexStep  = sizeof(ABVERTEX8L);
			return TRUE;
	    break;
	   
	   }
	   return FALSE;
}

//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
float CAdvancedVertexObj::GetVertexX(int i)
{
	//int i=0;
	//m_vertexCount
       switch(m_uvCount)
	   {
	    case 0:
             return m_vertexArray0[i].x;
		break;
	   
	    case 1:
		    return m_vertexArray1[i].x;
		break;
	   
	    case 2:
			return m_vertexArray2[i].x;
	    break;
	   
	    case 3:
			return m_vertexArray3[i].x;
	    break;

		case 4:
			return m_vertexArray4[i].x;
	    break;

		case 5:
			return m_vertexArray5[i].x;
	    break;

		case 6:
			return m_vertexArray6[i].x;
	    break;

		case 7:
			return m_vertexArray7[i].x;
	    break;

		case 8:
			return m_vertexArray8[i].x;
	    break;
	   
	   }
   return 0.0f;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
float CAdvancedVertexObj::GetVertexY(int i)
{
	//int i=0;
	//m_vertexCount
       switch(m_uvCount)
	   {
	    case 0:
             return m_vertexArray0[i].y;
		break;
	   
	    case 1:
		    return m_vertexArray1[i].y;
		break;
	   
	    case 2:
			return m_vertexArray2[i].y;
	    break;
	   
	    case 3:
			return m_vertexArray3[i].y;
	    break;

		case 4:
			return m_vertexArray4[i].y;
	    break;

		case 5:
			return m_vertexArray5[i].y;
	    break;

		case 6:
			return m_vertexArray6[i].y;
	    break;

		case 7:
			return m_vertexArray7[i].y;
	    break;

		case 8:
			return m_vertexArray8[i].y;
	    break;
	   
	   }
   return 0.0f;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
float CAdvancedVertexObj::GetVertexZ(int i)
{
	//int i=0;
	//m_vertexCount
       switch(m_uvCount)
	   {
	    case 0:
             return m_vertexArray0[i].z;
		break;
	   
	    case 1:
		    return m_vertexArray1[i].z;
		break;
	   
	    case 2:
			return m_vertexArray2[i].z;
	    break;
	   
	    case 3:
			return m_vertexArray3[i].z;
	    break;

		case 4:
			return m_vertexArray4[i].z;
	    break;

		case 5:
			return m_vertexArray5[i].z;
	    break;

		case 6:
			return m_vertexArray6[i].z;
	    break;

		case 7:
			return m_vertexArray7[i].z;
	    break;

		case 8:
			return m_vertexArray8[i].z;
	    break;
	   
	   }
   return 0.0f;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
float CAdvancedVertexObj::GetVertexNX(int i)
{
	//int i=0;
	//m_vertexCount
       switch(m_uvCount)
	   {
	    case 0:
             return m_vertexArray0[i].nx;
		break;
	   
	    case 1:
		    return m_vertexArray1[i].nx;
		break;
	   
	    case 2:
			return m_vertexArray2[i].nx;
	    break;
	   
	    case 3:
			return m_vertexArray3[i].nx;
	    break;

		case 4:
			return m_vertexArray4[i].nx;
	    break;

		case 5:
			return m_vertexArray5[i].nx;
	    break;

		case 6:
			return m_vertexArray6[i].nx;
	    break;

		case 7:
			return m_vertexArray7[i].nx;
	    break;

		case 8:
			return m_vertexArray8[i].nx;
	    break;
	   
	   }
   return 0.0f;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
float CAdvancedVertexObj::GetVertexNY(int i)
{
	//int i=0;
	//m_vertexCount
       switch(m_uvCount)
	   {
	    case 0:
             return m_vertexArray0[i].ny;
		break;
	   
	    case 1:
		    return m_vertexArray1[i].ny;
		break;
	   
	    case 2:
			return m_vertexArray2[i].ny;
	    break;
	   
	    case 3:
			return m_vertexArray3[i].ny;
	    break;

		case 4:
			return m_vertexArray4[i].ny;
	    break;

		case 5:
			return m_vertexArray5[i].ny;
	    break;

		case 6:
			return m_vertexArray6[i].ny;
	    break;

		case 7:
			return m_vertexArray7[i].ny;
	    break;

		case 8:
			return m_vertexArray8[i].ny;
	    break;
	   
	   }
   return 0.0f;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
float CAdvancedVertexObj::GetVertexNZ(int i)
{
	//int i=0;
	//m_vertexCount
       switch(m_uvCount)
	   {
	    case 0:
             return m_vertexArray0[i].nz;
		break;
	   
	    case 1:
		    return m_vertexArray1[i].nz;
		break;
	   
	    case 2:
			return m_vertexArray2[i].nz;
	    break;
	   
	    case 3:
			return m_vertexArray3[i].nz;
	    break;

		case 4:
			return m_vertexArray4[i].nz;
	    break;

		case 5:
			return m_vertexArray5[i].nz;
	    break;

		case 6:
			return m_vertexArray6[i].nz;
	    break;

		case 7:
			return m_vertexArray7[i].nz;
	    break;

		case 8:
			return m_vertexArray8[i].nz;
	    break;
	   
	   }
   return 0.0f;
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
	   copyLocal->m_tangentExists       = m_tangentExists;

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
		case 19:
			copyLocal->m_vertexArray1t = new ABVERTEX1Lb[m_vertexCount];
	        CopyMemory(copyLocal->m_vertexArray1t,m_vertexArray1t,sizeof(ABVERTEX1Lb)*m_vertexCount);
	    break;
		case 20:
			copyLocal->m_vertexArray2t = new ABVERTEX2Lb[m_vertexCount];
	        CopyMemory(copyLocal->m_vertexArray2t,m_vertexArray2t,sizeof(ABVERTEX2Lb)*m_vertexCount);
	    break;
		case 21:
			copyLocal->m_vertexArray3t = new ABVERTEX3Lb[m_vertexCount];
	        CopyMemory(copyLocal->m_vertexArray3t,m_vertexArray3t,sizeof(ABVERTEX3Lb)*m_vertexCount);
	    break;
		case 22:
			copyLocal->m_vertexArray4t = new ABVERTEX4Lb[m_vertexCount];
	        CopyMemory(copyLocal->m_vertexArray4t,m_vertexArray4t,sizeof(ABVERTEX4Lb)*m_vertexCount);
	    break;
		case 23:
			copyLocal->m_vertexArray5t = new ABVERTEX5Lb[m_vertexCount];
	        CopyMemory(copyLocal->m_vertexArray5t,m_vertexArray5t,sizeof(ABVERTEX5Lb)*m_vertexCount);
	    break;
		case 24:
			copyLocal->m_vertexArray6t = new ABVERTEX6Lb[m_vertexCount];
	        CopyMemory(copyLocal->m_vertexArray6t,m_vertexArray6t,sizeof(ABVERTEX6Lb)*m_vertexCount);
	    break;
		case 50:
			copyLocal->m_vertexArray3t = new ABVERTEX3Lb[m_vertexCount];
	        CopyMemory(copyLocal->m_vertexArray3t,m_vertexArray3t,sizeof(ABVERTEX3Lb)*m_vertexCount);
	    break;
		case 70:
			copyLocal->m_vertexArray1x = new ABVERTEX1Lx[m_vertexCount];
	        CopyMemory(copyLocal->m_vertexArray1x,m_vertexArray1x,sizeof(ABVERTEX1Lx)*m_vertexCount);
	    break;
	   
	   }

	   InitBuffer(g_pd3dDevice,copyLocal->m_uvCount);

       
	   *clone = copyLocal;
}
//get normal by index
D3DVECTOR CAdvancedVertexObj::GetNormal(int i)
{
       D3DVECTOR reNorm;  
	   reNorm.x = 0.0f;
	   reNorm.y = 0.0f;
	   reNorm.z = -1.0f;//defaul ax foward
	   if(i >= m_vertexCount)
	      return reNorm;
     
	   switch(m_uvCount)
	   {
	    case 0:
            reNorm.x = m_vertexArray0[i].nx;
	        reNorm.y = m_vertexArray0[i].ny;
	        reNorm.z = m_vertexArray0[i].nz;
	    break;
	   
	    case 1:
			reNorm.x = m_vertexArray1[i].nx;
	        reNorm.y = m_vertexArray1[i].ny;
	        reNorm.z = m_vertexArray1[i].nz;
	    break;
	   
	    case 2:
			reNorm.x = m_vertexArray2[i].nx;
	        reNorm.y = m_vertexArray2[i].ny;
	        reNorm.z = m_vertexArray2[i].nz;
	    break;
	   
	    case 3:
			reNorm.x = m_vertexArray3[i].nx;
	        reNorm.y = m_vertexArray3[i].ny;
	        reNorm.z = m_vertexArray3[i].nz;
	    break;

		case 4:
			reNorm.x = m_vertexArray4[i].nx;
	        reNorm.y = m_vertexArray4[i].ny;
	        reNorm.z = m_vertexArray4[i].nz;
	    break;

		case 5:
			reNorm.x = m_vertexArray5[i].nx;
	        reNorm.y = m_vertexArray5[i].ny;
	        reNorm.z = m_vertexArray5[i].nz;
	    break;

		case 6:
			reNorm.x = m_vertexArray6[i].nx;
	        reNorm.y = m_vertexArray6[i].ny;
	        reNorm.z = m_vertexArray6[i].nz;
	    break;

		case 7:
			reNorm.x = m_vertexArray7[i].nx;
	        reNorm.y = m_vertexArray7[i].ny;
	        reNorm.z = m_vertexArray7[i].nz;
	    break;

		case 8:
			reNorm.x = m_vertexArray8[i].nx;
	        reNorm.y = m_vertexArray8[i].ny;
	        reNorm.z = m_vertexArray8[i].nz;
	    break;
		case 19:
			reNorm.x = m_vertexArray1t[i].nx;
	        reNorm.y = m_vertexArray1t[i].ny;
	        reNorm.z = m_vertexArray1t[i].nz;
	    break;
		case 20:
			reNorm.x = m_vertexArray2t[i].nx;
	        reNorm.y = m_vertexArray2t[i].ny;
	        reNorm.z = m_vertexArray2t[i].nz;
	    break;
		case 21:
			reNorm.x = m_vertexArray3t[i].nx;
	        reNorm.y = m_vertexArray3t[i].ny;
	        reNorm.z = m_vertexArray3t[i].nz;
	    break;
		case 22:
			reNorm.x = m_vertexArray4t[i].nx;
	        reNorm.y = m_vertexArray4t[i].ny;
	        reNorm.z = m_vertexArray4t[i].nz;
	    break;
		case 23:
			reNorm.x = m_vertexArray5t[i].nx;
	        reNorm.y = m_vertexArray5t[i].ny;
	        reNorm.z = m_vertexArray5t[i].nz;
	    break;
		case 24:
			reNorm.x = m_vertexArray6t[i].nx;
	        reNorm.y = m_vertexArray6t[i].ny;
	        reNorm.z = m_vertexArray6t[i].nz;
	    break;
		case 50:
			reNorm.x = m_vertexArray3t[i].nx;
	        reNorm.y = m_vertexArray3t[i].ny;
	        reNorm.z = m_vertexArray3t[i].nz;
	    break;
	   
	   }

	   return reNorm;
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
		case 19:

            for(loop=0;loop<m_vertexCount;loop++)
			{
	            CopyMemory(&newVertexArray[loop],&m_vertexArray1t[loop],sizeof(ABVERTEX1Lb));
				newVertexArray[loop].tx = m_vertexArray1t[loop].tX;
				newVertexArray[loop].ty = m_vertexArray1t[loop].tY;
				newVertexArray[loop].tz = m_vertexArray1t[loop].tZ;
			}
	    break;
		case 20:

            for(loop=0;loop<m_vertexCount;loop++)
			{
	            CopyMemory(&newVertexArray[loop],&m_vertexArray2t[loop],sizeof(ABVERTEX2Lb));
                newVertexArray[loop].tx = m_vertexArray2t[loop].tX;
				newVertexArray[loop].ty = m_vertexArray2t[loop].tY;
				newVertexArray[loop].tz = m_vertexArray2t[loop].tZ;
			}
	    break;
		case 21:

            for(loop=0;loop<m_vertexCount;loop++)
			{
	            CopyMemory(&newVertexArray[loop],&m_vertexArray3t[loop],sizeof(ABVERTEX3Lb));
                newVertexArray[loop].tx = m_vertexArray3t[loop].tX;
				newVertexArray[loop].ty = m_vertexArray3t[loop].tY;
				newVertexArray[loop].tz = m_vertexArray3t[loop].tZ;
			}
	    break;
		case 22:

            for(loop=0;loop<m_vertexCount;loop++)
			{
	            CopyMemory(&newVertexArray[loop],&m_vertexArray4t[loop],sizeof(ABVERTEX4Lb));
				newVertexArray[loop].tx = m_vertexArray4t[loop].tX;
				newVertexArray[loop].ty = m_vertexArray4t[loop].tY;
				newVertexArray[loop].tz = m_vertexArray4t[loop].tZ;
			}
	    break;
		case 23:

            for(loop=0;loop<m_vertexCount;loop++)
			{
	            CopyMemory(&newVertexArray[loop],&m_vertexArray5t[loop],sizeof(ABVERTEX5Lb));
				newVertexArray[loop].tx = m_vertexArray5t[loop].tX;
				newVertexArray[loop].ty = m_vertexArray5t[loop].tY;
				newVertexArray[loop].tz = m_vertexArray5t[loop].tZ;
			}
	    break;
		case 24:

            for(loop=0;loop<m_vertexCount;loop++)
			{
	            CopyMemory(&newVertexArray[loop],&m_vertexArray6t[loop],sizeof(ABVERTEX6Lb));
				newVertexArray[loop].tx = m_vertexArray6t[loop].tX;
				newVertexArray[loop].ty = m_vertexArray6t[loop].tY;
				newVertexArray[loop].tz = m_vertexArray6t[loop].tZ;
			}
	    break;


		
		case 50:

            for(loop=0;loop<m_vertexCount;loop++)
			{
	            CopyMemory(&newVertexArray[loop],&m_vertexArray3t[loop],sizeof(ABVERTEX3Lb));
				newVertexArray[loop].tx = m_vertexArray3t[loop].tX;
				newVertexArray[loop].ty = m_vertexArray3t[loop].tY;
				newVertexArray[loop].tz = m_vertexArray3t[loop].tZ;
			}
	    break;

		case 70:
            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&newVertexArray[loop],&m_vertexArray1x[loop],sizeof(ABVERTEX1Lx));
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
		case 19:
			if(m_vertexArray1t)
			   delete [] m_vertexArray1t;
			   m_vertexArray1t = 0;

			m_vertexArray1t = new ABVERTEX1Lb[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
			{
	            CopyMemory(&m_vertexArray1t[loop],&vArray[loop],sizeof(ABVERTEX1Lb));
				m_vertexArray1t[loop].tX = vArray[loop].tx;
				m_vertexArray1t[loop].tY = vArray[loop].ty;
				m_vertexArray1t[loop].tZ = vArray[loop].tz;
			}
	    break;
		case 20:
           if(m_vertexArray2t)
			   delete [] m_vertexArray2t;
			   m_vertexArray2t = 0;

			m_vertexArray2t = new ABVERTEX2Lb[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
			{
	            CopyMemory(&m_vertexArray2t[loop],&vArray[loop],sizeof(ABVERTEX2Lb));
				m_vertexArray2t[loop].tX = vArray[loop].tx;
				m_vertexArray2t[loop].tY = vArray[loop].ty;
				m_vertexArray2t[loop].tZ = vArray[loop].tz;
			}
	    break;
		case 21:

            if(m_vertexArray3t)
			   delete [] m_vertexArray3t;
			   m_vertexArray3t = 0;

			m_vertexArray3t = new ABVERTEX3Lb[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
			{
	            CopyMemory(&m_vertexArray3t[loop],&vArray[loop],sizeof(ABVERTEX3Lb));
				m_vertexArray3t[loop].tX = vArray[loop].tx;
				m_vertexArray3t[loop].tY = vArray[loop].ty;
				m_vertexArray3t[loop].tZ = vArray[loop].tz;
			}
	    break;
		case 22:

            if(m_vertexArray4t)
			   delete [] m_vertexArray4t;
			   m_vertexArray4t = 0;

			m_vertexArray4t = new ABVERTEX4Lb[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
			{
	            CopyMemory(&m_vertexArray4t[loop],&vArray[loop],sizeof(ABVERTEX4Lb));
				m_vertexArray4t[loop].tX = vArray[loop].tx;
				m_vertexArray4t[loop].tY = vArray[loop].ty;
				m_vertexArray4t[loop].tZ = vArray[loop].tz;
			}
	    break;
		case 23:

            
            if(m_vertexArray5t)
			   delete [] m_vertexArray5t;
			   m_vertexArray5t = 0;

			m_vertexArray5t = new ABVERTEX5Lb[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
			{
	            CopyMemory(&m_vertexArray5t[loop],&vArray[loop],sizeof(ABVERTEX5Lb));
				m_vertexArray5t[loop].tX = vArray[loop].tx;
				m_vertexArray5t[loop].tY = vArray[loop].ty;
				m_vertexArray5t[loop].tZ = vArray[loop].tz;
			}
	    break;
		case 24:

            if(m_vertexArray6t)
			   delete [] m_vertexArray6t;
			   m_vertexArray6t = 0;

			m_vertexArray6t = new ABVERTEX6Lb[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
			{
	            CopyMemory(&m_vertexArray6t[loop],&vArray[loop],sizeof(ABVERTEX6Lb));
				m_vertexArray6t[loop].tX = vArray[loop].tx;
				m_vertexArray6t[loop].tY = vArray[loop].ty;
				m_vertexArray6t[loop].tZ = vArray[loop].tz;
			}
	    break;


		
		case 50:

              if(m_vertexArray3t)
			   delete [] m_vertexArray3t;
			   m_vertexArray3t = 0;

			m_vertexArray3t = new ABVERTEX3Lb[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
			{
	            CopyMemory(&m_vertexArray3t[loop],&vArray[loop],sizeof(ABVERTEX3Lb));
				m_vertexArray3t[loop].tX = vArray[loop].tx;
				m_vertexArray3t[loop].tY = vArray[loop].ty;
				m_vertexArray3t[loop].tZ = vArray[loop].tz;
			}
	    break;
		case 70:
		    if(m_vertexArray1x)
			   delete [] m_vertexArray1x;
			   m_vertexArray1x = 0;

			m_vertexArray1x = new ABVERTEX1Lx[m_vertexCount];

            for(loop=0;loop<m_vertexCount;loop++)
	            CopyMemory(&m_vertexArray1x[loop],&vArray[loop],sizeof(ABVERTEX1Lx));
	    break;
	   
	   }



	   

	   if(pd3dDevice)
	      InitBuffer(pd3dDevice,this->m_uvCount);
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
/*void CAdvancedVertexObj::LogVertexsForFaceVolume(D3DXVECTOR3 * returnBuffer,int & vertexCurCount,D3DVECTOR * light)
{



}*/
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CAdvancedVertexObj::MakeIntoSimpleShadowVolume(IDirect3DDevice9 * pd3dDevice,
													float max,
													float min)
{

	   if(m_vertexCount == 0 || m_indecieCount == 0)
		  return;

	   if(!m_vertexBuffer)
		   return;

	   //vertexCount = 0;

	   long datasize = 0;
	   int datasizeIndecie = 0;

	   D3DVECTOR lightPosition;
	   lightPosition.x = 0.0f;
	   lightPosition.y = 50.0f;
	   lightPosition.z = 0.0f;
	   float lightCastDist = 30;

       int vxCount = 0;
	   WORD * pIndices;
       switch(m_uvCount)
	   {
	    case 0:
            //Create a vertex buffer
               ABVERTEX0L* pVertices0;
			   datasize = m_vertexCount*sizeof(ABVERTEX0L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices0, m_lockFlags )==0)
			   {//lock buffer
                  //for(int loop=0;loop<m_vertexCount;loop++)
	              //   memcpy(&pVertices0[loop],&m_origVertexArray[loop],sizeof(ABVERTEX0L));

                  //CopyMemory(pVertices0,m_vertexArray0,(sizeof(ABVERTEX0L)*m_vertexCount));

                  for(int loop=0;loop<m_vertexCount;loop++)
				  {
                      if(pVertices0[loop].ny > 0)
					  {
                         pVertices0[loop].y = max;

					  }
					  else
					  {
                         pVertices0[loop].y = min;
					  }
				  }

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;
		case 1:
			
			   datasizeIndecie = m_indecieCount * sizeof(WORD);
               m_indicieBuffer->Lock(0,datasizeIndecie,(VOID**)&pIndices, m_lockFlags );
               ABVERTEX1L* pVertices1;
			   datasize = m_vertexCount*sizeof(ABVERTEX1L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices1, m_lockFlags  )==0)
			   {//lock buffer
				  for(int loop=0;loop<m_vertexCount;loop++)
				  {
                      if(pVertices1[loop].ny < 0)
					  {
                         pVertices1[loop].y = min;

					  }
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
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices2, m_lockFlags )==0)
			   {//lock buffer
				  for(int loop=0;loop<m_vertexCount;loop++)
				  {
                      if(pVertices2[loop].ny < 0)
					  {
                         pVertices2[loop].y = min;

					  }
				  }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 3:
			//-----------------------------
			//-----------------------------
               ABVERTEX3L* pVertices3;
			   datasize = m_vertexCount*sizeof(ABVERTEX3L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices3, m_lockFlags )==0)
			   {//lock buffer


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
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices4, m_lockFlags )==0)
			   {//lock buffer

				  m_vertexBuffer->Unlock();
			   }//end lock beffer
		
			//-----------------------------
			//-----------------------------
	    break;
		case 5:
			//-----------------------------
			//-----------------------------
            //Create a vertex buffer
               ABVERTEX5L* pVertices5;
			   datasize = m_vertexCount*sizeof(ABVERTEX5L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices5, m_lockFlags )==0)
			   {//lock buffer
				 

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 6:
			//-----------------------------
			//-----------------------------
            //Create a vertex buffer
               ABVERTEX6L* pVertices6;
			   datasize = m_vertexCount*sizeof(ABVERTEX6L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices6, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	              //   memcpy(&pVertices6[loop],&m_origVertexArray[loop],sizeof(ABVERTEX6L));
                  
                  
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 7:
			//-----------------------------
			//-----------------------------
               ABVERTEX7L* pVertices7;
			   datasize = m_vertexCount*sizeof(ABVERTEX7L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices7, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	                  //memcpy(&pVertices7[loop],&m_origVertexArray[loop],sizeof(ABVERTEX7L));


                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 8:
			//-----------------------------
			//----------------------------
            //Create a vertex buffer
               ABVERTEX8L* pVertices8;
			   datasize = m_vertexCount*sizeof(ABVERTEX8L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices8, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	                 //memcpy(&pVertices8[loop],&m_origVertexArray[loop],sizeof(ABVERTEX8L));
                  //CopyMemory(pVertices8,m_vertexArray8,(sizeof(ABVERTEX8L)*m_vertexCount));

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
void CAdvancedVertexObj::MakeIntoComplexShadowVolume(IDirect3DDevice9 * pd3dDevice,
													 D3DXVECTOR3 light,
													 D3DXVECTOR3 * returnBuffer,
													 int * vertexCount,
													 float maxDepth)
{

	   
	   if(m_vertexCount == 0 || m_indecieCount == 0)
		  return;

	   if(!m_vertexBuffer)
		   return;

	   int datasizeIndecie = 0;

       int vxCount = *vertexCount;
	   long datasize = 0;
	   D3DXVECTOR3 reg1,reg2,tempNormal;
	   WORD wFace0,wFace1,wFace2;
	   WORD * pIndices;
       switch(m_uvCount)
	   {
	    case 0:
            //Create a vertex buffer
               ABVERTEX0L* pVertices0;
			   datasize = m_vertexCount*sizeof(ABVERTEX0L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices0, m_lockFlags )==0)
			   {//lock buffer
                  //for(int loop=0;loop<m_vertexCount;loop++)
	              //   memcpy(&pVertices0[loop],&m_origVertexArray[loop],sizeof(ABVERTEX0L));

                  //CopyMemory(pVertices0,m_vertexArray0,(sizeof(ABVERTEX0L)*m_vertexCount));

                 

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;
		case 1:
			 datasizeIndecie = m_indecieCount * sizeof(WORD);
			 if(datasizeIndecie > 0)
			 {
			   
               m_indicieBuffer->Lock(0,datasizeIndecie,(VOID**)&pIndices, m_lockFlags );
               ABVERTEX1L* pVertices;
			   datasize = m_vertexCount*sizeof(ABVERTEX1L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, m_lockFlags  )==0)
			   {//lock buffer
			
                  DWORD dwNumFaces    = m_indecieCount/3;
                 
                  DWORD dwNumEdges = 0;

                  //For each face
                  for( DWORD i=0; i<dwNumFaces; i++ )
                  {
                       wFace0 = pIndices[3*i+0];
                       wFace1 = pIndices[3*i+1];
                       wFace2 = pIndices[3*i+2];

					   reg1.x = pVertices[wFace2].x - pVertices[wFace1].x;
					   reg1.y = pVertices[wFace2].y - pVertices[wFace1].y;
					   reg1.z = pVertices[wFace2].z - pVertices[wFace1].z;
					   reg2.x = pVertices[wFace1].x - pVertices[wFace0].x;
					   reg2.y = pVertices[wFace1].y - pVertices[wFace0].y;
					   reg2.z = pVertices[wFace1].z - pVertices[wFace0].z;

					   tempNormal.x = reg1.y * reg2.z - reg1.z * reg2.y;
                       tempNormal.y = reg1.z * reg2.x - reg1.x * reg2.z;
                       tempNormal.z = reg1.x * reg2.y - reg1.y * reg2.x;

					   //if( tempNormal.y >= 0.0f )
					   
                       if( (tempNormal.x*light.x + tempNormal.y * light.y + tempNormal.z*light.z) >= 0.0f )
                       {
						   /*for( DWORD i=0; i < dwNumEdges; i++ )
                           {
                                if( ( pEdges[2*i+0] == v0 && pEdges[2*i+1] == v1 ) ||
                                    ( pEdges[2*i+0] == v1 && pEdges[2*i+1] == v0 ) )
                                {
                                    if( dwNumEdges > 1 )
                                    {
                                        pEdges[2*i+0] = pEdges[2*(dwNumEdges-1)+0];
                                        pEdges[2*i+1] = pEdges[2*(dwNumEdges-1)+1];
                                    }
                                dwNumEdges--;
                                return;
                           }*/
						  
     
						   
						   //pEdges[2*dwNumEdges+0] = wFace0;
                           //pEdges[2*dwNumEdges+1] = wFace1;
                           //edge roll out
                           returnBuffer[vxCount].x   = pVertices[wFace0].x;//v1;
						   returnBuffer[vxCount].y   = pVertices[wFace0].y;//v1;
						   returnBuffer[vxCount++].z = pVertices[wFace0].z;//v1;

                           returnBuffer[vxCount].x     = pVertices[wFace1].x;//v2;
						   returnBuffer[vxCount].y     = pVertices[wFace1].y;//v2;
						   returnBuffer[vxCount++].z   = pVertices[wFace1].z;//v2;

						   returnBuffer[vxCount].x     = pVertices[wFace0].x - light.x;//v3;
						   returnBuffer[vxCount].y     = pVertices[wFace0].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = pVertices[wFace0].z - light.z;//v3;

						   returnBuffer[vxCount].x     = pVertices[wFace1].x;//v2;
						   returnBuffer[vxCount].y     = pVertices[wFace1].y;//v2;
						   returnBuffer[vxCount++].z   = pVertices[wFace1].z;//v2;

						   returnBuffer[vxCount].x     = pVertices[wFace1].x - light.x;//v4;
						   returnBuffer[vxCount].y     = pVertices[wFace1].y - light.y;//v4;
						   returnBuffer[vxCount++].z   = pVertices[wFace1].z - light.z;//v4;

						   returnBuffer[vxCount].x     = pVertices[wFace0].x - light.x;//v3;
						   returnBuffer[vxCount].y     = pVertices[wFace0].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = pVertices[wFace0].z - light.z;//v3;
						   //end edge roll out
                           dwNumEdges++;
                           //AddEdge( pEdges, dwNumEdges, wFace1, wFace2 );
						   //pEdges[2*dwNumEdges+0] = wFace1;
                           //pEdges[2*dwNumEdges+1] = wFace2;
						   //edge roll out
                           returnBuffer[vxCount].x   = pVertices[wFace1].x;//v1;
						   returnBuffer[vxCount].y   = pVertices[wFace1].y;//v1;
						   returnBuffer[vxCount++].z = pVertices[wFace1].z;//v1;

                           returnBuffer[vxCount].x     = pVertices[wFace2].x;//v2;
						   returnBuffer[vxCount].y     = pVertices[wFace2].y;//v2;
						   returnBuffer[vxCount++].z   = pVertices[wFace2].z;//v2;

						   returnBuffer[vxCount].x     = pVertices[wFace1].x - light.x;//v3;
						   returnBuffer[vxCount].y     = pVertices[wFace1].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = pVertices[wFace1].z - light.z;//v3;
						  

						   returnBuffer[vxCount].x     = pVertices[wFace2].x;//v2;
						   returnBuffer[vxCount].y     = pVertices[wFace2].y;//v2;
						   returnBuffer[vxCount++].z   = pVertices[wFace2].z;//v2;

						   returnBuffer[vxCount].x     = pVertices[wFace2].x - light.x;//v4;
						   returnBuffer[vxCount].y     = pVertices[wFace2].y - light.y;//v4;
						   returnBuffer[vxCount++].z   = pVertices[wFace2].z - light.z;//v4;
						  

						   returnBuffer[vxCount].x     = pVertices[wFace1].x - light.x;//v3;
						   returnBuffer[vxCount].y     = pVertices[wFace1].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = pVertices[wFace1].z - light.z;//v3;
						  
						   //end edge roll out
                           dwNumEdges++;
                           //AddEdge( pEdges, dwNumEdges, wFace2, wFace0 );
						   //pEdges[2*dwNumEdges+0] = wFace2;
                           //pEdges[2*dwNumEdges+1] = wFace0;
						   //edge roll out
                           returnBuffer[vxCount].x   = pVertices[wFace2].x;//v1;
						   returnBuffer[vxCount].y   = pVertices[wFace2].y;//v1;
						   returnBuffer[vxCount++].z = pVertices[wFace2].z;//v1;

                           returnBuffer[vxCount].x     = pVertices[wFace0].x;//v2;
						   returnBuffer[vxCount].y     = pVertices[wFace0].y;//v2;
						   returnBuffer[vxCount++].z   = pVertices[wFace0].z;//v2;

						   returnBuffer[vxCount].x     = pVertices[wFace2].x - light.x;//v3;
						   returnBuffer[vxCount].y     = pVertices[wFace2].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = pVertices[wFace2].z - light.z;//v3;

						   returnBuffer[vxCount].x     = pVertices[wFace0].x;//v2;
						   returnBuffer[vxCount].y     = pVertices[wFace0].y;//v2;
						   returnBuffer[vxCount++].z   = pVertices[wFace0].z;//v2;

						   returnBuffer[vxCount].x     = pVertices[wFace0].x - light.x;//v4;
						   returnBuffer[vxCount].y     = pVertices[wFace0].y - light.y;//v4;
						   returnBuffer[vxCount++].z   = pVertices[wFace0].z - light.z;//v4;

						   returnBuffer[vxCount].x     = pVertices[wFace2].x - light.x;//v3;
						   returnBuffer[vxCount].y     = pVertices[wFace2].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = pVertices[wFace2].z - light.z;//v3;
						   //end edge roll out
						   
                           dwNumEdges++;


                       }
					    
                   }

                  
				   *vertexCount = vxCount;
                   //Delete the temporary edge list
                   //delete[] pEdges;

            
                  m_indicieBuffer->Unlock();
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			}
			//-----------------------------
			//-----------------------------
	    break;
		case 2:
			//-----------------------------
			//-----------------------------
             datasizeIndecie = m_indecieCount * sizeof(WORD);
			 if(datasizeIndecie > 0)
			 {
			   WORD * pIndices;
               m_indicieBuffer->Lock(0,datasizeIndecie,(VOID**)&pIndices, m_lockFlags );
               ABVERTEX2L* pVertices;
			   datasize = m_vertexCount*sizeof(ABVERTEX2L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, m_lockFlags  )==0)
			   {//lock buffer
			
                  DWORD dwNumFaces    = m_indecieCount/3;
                 
                  DWORD dwNumEdges = 0;

                  //For each face
                  for( DWORD i=0; i<dwNumFaces; i++ )
                  {
                       wFace0 = pIndices[3*i+0];
                       wFace1 = pIndices[3*i+1];
                       wFace2 = pIndices[3*i+2];

					   reg1.x = pVertices[wFace2].x - pVertices[wFace1].x;
					   reg1.y = pVertices[wFace2].y - pVertices[wFace1].y;
					   reg1.z = pVertices[wFace2].z - pVertices[wFace1].z;
					   reg2.x = pVertices[wFace1].x - pVertices[wFace0].x;
					   reg2.y = pVertices[wFace1].y - pVertices[wFace0].y;
					   reg2.z = pVertices[wFace1].z - pVertices[wFace0].z;

					   tempNormal.x = reg1.y * reg2.z - reg1.z * reg2.y;
                       tempNormal.y = reg1.z * reg2.x - reg1.x * reg2.z;
                       tempNormal.z = reg1.x * reg2.y - reg1.y * reg2.x;

					   //if( tempNormal.y >= 0.0f )
					   
                       if( (tempNormal.x*light.x + tempNormal.y * light.y + tempNormal.z*light.z) >= 0.0f )
                       {
						   
						   //pEdges[2*dwNumEdges+0] = wFace0;
                           //pEdges[2*dwNumEdges+1] = wFace1;
                           //edge roll out
                           returnBuffer[vxCount].x   = pVertices[wFace0].x;//v1;
						   returnBuffer[vxCount].y   = pVertices[wFace0].y;//v1;
						   returnBuffer[vxCount++].z = pVertices[wFace0].z;//v1;

                           returnBuffer[vxCount].x     = pVertices[wFace1].x;//v2;
						   returnBuffer[vxCount].y     = pVertices[wFace1].y;//v2;
						   returnBuffer[vxCount++].z   = pVertices[wFace1].z;//v2;

						   returnBuffer[vxCount].x     = pVertices[wFace0].x - light.x;//v3;
						   returnBuffer[vxCount].y     = pVertices[wFace0].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = pVertices[wFace0].z - light.z;//v3;

						   returnBuffer[vxCount].x     = pVertices[wFace1].x;//v2;
						   returnBuffer[vxCount].y     = pVertices[wFace1].y;//v2;
						   returnBuffer[vxCount++].z   = pVertices[wFace1].z;//v2;

						   returnBuffer[vxCount].x     = pVertices[wFace1].x - light.x;//v4;
						   returnBuffer[vxCount].y     = pVertices[wFace1].y - light.y;//v4;
						   returnBuffer[vxCount++].z   = pVertices[wFace1].z - light.z;//v4;

						   returnBuffer[vxCount].x     = pVertices[wFace0].x - light.x;//v3;
						   returnBuffer[vxCount].y     = pVertices[wFace0].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = pVertices[wFace0].z - light.z;//v3;
						   //end edge roll out
                           dwNumEdges++;
                           //AddEdge( pEdges, dwNumEdges, wFace1, wFace2 );
						   //pEdges[2*dwNumEdges+0] = wFace1;
                           //pEdges[2*dwNumEdges+1] = wFace2;
						   //edge roll out
                           returnBuffer[vxCount].x   = pVertices[wFace1].x;//v1;
						   returnBuffer[vxCount].y   = pVertices[wFace1].y;//v1;
						   returnBuffer[vxCount++].z = pVertices[wFace1].z;//v1;

                           returnBuffer[vxCount].x     = pVertices[wFace2].x;//v2;
						   returnBuffer[vxCount].y     = pVertices[wFace2].y;//v2;
						   returnBuffer[vxCount++].z   = pVertices[wFace2].z;//v2;

						   returnBuffer[vxCount].x     = pVertices[wFace1].x - light.x;//v3;
						   returnBuffer[vxCount].y     = pVertices[wFace1].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = pVertices[wFace1].z - light.z;//v3;

						   returnBuffer[vxCount].x     = pVertices[wFace2].x;//v2;
						   returnBuffer[vxCount].y     = pVertices[wFace2].y;//v2;
						   returnBuffer[vxCount++].z   = pVertices[wFace2].z;//v2;

						   returnBuffer[vxCount].x     = pVertices[wFace2].x - light.x;//v4;
						   returnBuffer[vxCount].y     = pVertices[wFace2].y - light.y;//v4;
						   returnBuffer[vxCount++].z   = pVertices[wFace2].z - light.z;//v4;

						   returnBuffer[vxCount].x     = pVertices[wFace1].x - light.x;//v3;
						   returnBuffer[vxCount].y     = pVertices[wFace1].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = pVertices[wFace1].z - light.z;//v3;
						   //end edge roll out
                           dwNumEdges++;
                           //AddEdge( pEdges, dwNumEdges, wFace2, wFace0 );
						   //pEdges[2*dwNumEdges+0] = wFace2;
                           //pEdges[2*dwNumEdges+1] = wFace0;
						   //edge roll out
                           returnBuffer[vxCount].x   = pVertices[wFace2].x;//v1;
						   returnBuffer[vxCount].y   = pVertices[wFace2].y;//v1;
						   returnBuffer[vxCount++].z = pVertices[wFace2].z;//v1;

                           returnBuffer[vxCount].x     = pVertices[wFace0].x;//v2;
						   returnBuffer[vxCount].y     = pVertices[wFace0].y;//v2;
						   returnBuffer[vxCount++].z   = pVertices[wFace0].z;//v2;

						   returnBuffer[vxCount].x     = pVertices[wFace2].x - light.x;//v3;
						   returnBuffer[vxCount].y     = pVertices[wFace2].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = pVertices[wFace2].z - light.z;//v3;

						   returnBuffer[vxCount].x     = pVertices[wFace0].x;//v2;
						   returnBuffer[vxCount].y     = pVertices[wFace0].y;//v2;
						   returnBuffer[vxCount++].z   = pVertices[wFace0].z;//v2;

						   returnBuffer[vxCount].x     = pVertices[wFace0].x - light.x;//v4;
						   returnBuffer[vxCount].y     = pVertices[wFace0].y - light.y;//v4;
						   returnBuffer[vxCount++].z   = pVertices[wFace0].z - light.z;//v4;

						   returnBuffer[vxCount].x     = pVertices[wFace2].x - light.x;//v3;
						   returnBuffer[vxCount].y     = pVertices[wFace2].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = pVertices[wFace2].z - light.z;//v3;
						   //end edge roll out
                           dwNumEdges++;


                       }
					    
                   }

                  
				   *vertexCount = vxCount;

            
                  m_indicieBuffer->Unlock();
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			 }
			//-----------------------------
			//-----------------------------
	    break;
		case 3:
			//-----------------------------
			//-----------------------------
               ABVERTEX3L* pVertices3;
			   datasize = m_vertexCount*sizeof(ABVERTEX3L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices3, m_lockFlags )==0)
			   {//lock buffer


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
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices4, m_lockFlags )==0)
			   {//lock buffer

				  m_vertexBuffer->Unlock();
			   }//end lock beffer
		
			//-----------------------------
			//-----------------------------
	    break;
		case 5:
			//-----------------------------
			//-----------------------------
            //Create a vertex buffer
               ABVERTEX5L* pVertices5;
			   datasize = m_vertexCount*sizeof(ABVERTEX5L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices5, m_lockFlags )==0)
			   {//lock buffer
				 

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 6:
			//-----------------------------
			//-----------------------------
            //Create a vertex buffer
               ABVERTEX6L* pVertices6;
			   datasize = m_vertexCount*sizeof(ABVERTEX6L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices6, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	              //   memcpy(&pVertices6[loop],&m_origVertexArray[loop],sizeof(ABVERTEX6L));
                  
                  
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 7:
			//-----------------------------
			//-----------------------------
               ABVERTEX7L* pVertices7;
			   datasize = m_vertexCount*sizeof(ABVERTEX7L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices7, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	                  //memcpy(&pVertices7[loop],&m_origVertexArray[loop],sizeof(ABVERTEX7L));


                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 8:
			//-----------------------------
			//----------------------------
            //Create a vertex buffer
               ABVERTEX8L* pVertices8;
			   datasize = m_vertexCount*sizeof(ABVERTEX8L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices8, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	                 //memcpy(&pVertices8[loop],&m_origVertexArray[loop],sizeof(ABVERTEX8L));
                  //CopyMemory(pVertices8,m_vertexArray8,(sizeof(ABVERTEX8L)*m_vertexCount));

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
void CAdvancedVertexObj::MakeIntoComplexShadowVolume2(IDirect3DDevice9 * pd3dDevice,
													 D3DXVECTOR3 light,
													 D3DXVECTOR3 * returnBuffer,
													 DWORD * vertexCount,
													 float maxDepth)
{

	   
	   if(m_vertexCount == 0 || m_indecieCount == 0)
		  return;

	   if(!m_vertexBuffer)
		   return;

	   int datasizeIndecie = 0;

       int vxCount = *vertexCount;
	   long datasize = 0;
	   D3DXVECTOR3 reg1,reg2,tempNormal;
	   WORD wFace0,wFace1,wFace2;
       switch(m_uvCount)
	   {
	    case 0:
            //Create a vertex buffer
               ABVERTEX0L* pVertices0;
			   datasize = m_vertexCount*sizeof(ABVERTEX0L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices0, m_lockFlags )==0)
			   {//lock buffer
                  //for(int loop=0;loop<m_vertexCount;loop++)
	              //   memcpy(&pVertices0[loop],&m_origVertexArray[loop],sizeof(ABVERTEX0L));

                  //CopyMemory(pVertices0,m_vertexArray0,(sizeof(ABVERTEX0L)*m_vertexCount));

                 

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;
		case 1:
			 //datasizeIndecie = m_indecieCount * sizeof(WORD);
			 if(1)
			 {
			   
               //m_indicieBuffer->Lock(0,datasizeIndecie,(VOID**)&pIndices, m_lockFlags );
               //ABVERTEX1L* pVertices;
			   //datasize = m_vertexCount*sizeof(ABVERTEX1L);
               //if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, m_lockFlags  )==0)
			   //{//lock buffer
			
                  DWORD dwNumFaces    = m_indecieCount/3;
                  WORD* pEdges        = new WORD[dwNumFaces*6];
                 
                  DWORD dwNumEdges = 0;

                  //For each face
				  DWORD i = 0;
                  for(i=0; i<dwNumFaces; i++ )
                  {
					  wFace0 = m_indecieArray[3*i+0];
                       wFace1 = m_indecieArray[3*i+1];
                       wFace2 = m_indecieArray[3*i+2];
					   m_vertexArray1[wFace0].ny = 1.0f;
							m_vertexArray1[wFace1].ny = 1.0f;
							m_vertexArray1[wFace2].ny = 1.0f;

				  }

                  for( i=0; i<dwNumFaces; i++ )
                  {
                       wFace0 = m_indecieArray[3*i+0];
                       wFace1 = m_indecieArray[3*i+1];
                       wFace2 = m_indecieArray[3*i+2];

					   reg1.x = m_vertexArray1[wFace2].x - m_vertexArray1[wFace1].x;
					   reg1.y = m_vertexArray1[wFace2].y - m_vertexArray1[wFace1].y;
					   reg1.z = m_vertexArray1[wFace2].z - m_vertexArray1[wFace1].z;
					   reg2.x = m_vertexArray1[wFace1].x - m_vertexArray1[wFace0].x;
					   reg2.y = m_vertexArray1[wFace1].y - m_vertexArray1[wFace0].y;
					   reg2.z = m_vertexArray1[wFace1].z - m_vertexArray1[wFace0].z;

					   tempNormal.x = reg1.y * reg2.z - reg1.z * reg2.y;
                       tempNormal.y = reg1.z * reg2.x - reg1.x * reg2.z;
                       tempNormal.z = reg1.x * reg2.y - reg1.y * reg2.x;

					   //if( tempNormal.y >= 0.0f )
					   
                       if( (tempNormal.x*light.x + tempNormal.y * light.y + tempNormal.z*light.z) >= 0.0f )
                       {
						    m_vertexArray1[wFace0].ny = 2.0f;
							m_vertexArray1[wFace1].ny = 2.0f;
							m_vertexArray1[wFace2].ny = 2.0f;

							returnBuffer[vxCount].x   = m_vertexArray1[wFace0].x;//v1;
						   returnBuffer[vxCount].y   = m_vertexArray1[wFace0].y;//v1;
						   returnBuffer[vxCount++].z = m_vertexArray1[wFace0].z;//v1;

                           returnBuffer[vxCount].x     = m_vertexArray1[wFace1].x;//v2;
						   returnBuffer[vxCount].y     = m_vertexArray1[wFace1].y;//v2;
						   returnBuffer[vxCount++].z   = m_vertexArray1[wFace1].z;//v2;

						   returnBuffer[vxCount].x     = m_vertexArray1[wFace2].x;//v3;
						   returnBuffer[vxCount].y     = m_vertexArray1[wFace2].y;//v3;
						   returnBuffer[vxCount++].z   = m_vertexArray1[wFace2].z;//v3;


						  
                           returnBuffer[vxCount].x   = m_vertexArray1[wFace2].x - light.x;//v1;
						   returnBuffer[vxCount].y   = m_vertexArray1[wFace2].y - light.y;//v1;
						   returnBuffer[vxCount++].z = m_vertexArray1[wFace2].z - light.z;//v1;

                           returnBuffer[vxCount].x     = m_vertexArray1[wFace1].x - light.x;//v2;
						   returnBuffer[vxCount].y     = m_vertexArray1[wFace1].y - light.y;//v2;
						   returnBuffer[vxCount++].z   = m_vertexArray1[wFace1].z - light.z;//v2;

						   returnBuffer[vxCount].x     = m_vertexArray1[wFace0].x - light.x;//v3;
						   returnBuffer[vxCount].y     = m_vertexArray1[wFace0].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = m_vertexArray1[wFace0].z - light.z;//v3;
                       }
					   else
					   {
						   //pVertices[wFace1].ny = -1.0f;
                           
					   }
					    
                   }

				   for( i=0; i<dwNumFaces; i++ )
                   {
                       wFace0 = m_indecieArray[3*i+0];
                       wFace1 = m_indecieArray[3*i+1];
                       wFace2 = m_indecieArray[3*i+2];

					   reg1.x = m_vertexArray1[wFace2].x - m_vertexArray1[wFace1].x;
					   reg1.y = m_vertexArray1[wFace2].y - m_vertexArray1[wFace1].y;
					   reg1.z = m_vertexArray1[wFace2].z - m_vertexArray1[wFace1].z;
					   reg2.x = m_vertexArray1[wFace1].x - m_vertexArray1[wFace0].x;
					   reg2.y = m_vertexArray1[wFace1].y - m_vertexArray1[wFace0].y;
					   reg2.z = m_vertexArray1[wFace1].z - m_vertexArray1[wFace0].z;

					   tempNormal.x = reg1.y * reg2.z - reg1.z * reg2.y;
                       tempNormal.y = reg1.z * reg2.x - reg1.x * reg2.z;
                       tempNormal.z = reg1.x * reg2.y - reg1.y * reg2.x;

					   //if( tempNormal.y >= 0.0f )
					   
                       if( (tempNormal.x*light.x + tempNormal.y * light.y + tempNormal.z*light.z) >= 0.0f )
                       {
						    
                       }
					   else
					   {

						   if(m_vertexArray1[wFace0].ny == 2.0f && m_vertexArray1[wFace1].ny == 2.0f)
						   {
                              pEdges[dwNumEdges] = wFace0;
							  dwNumEdges++;
                              pEdges[dwNumEdges] = wFace1;
							  dwNumEdges++;
						   }
						   else if(m_vertexArray1[wFace0].ny == 2.0f && m_vertexArray1[wFace2].ny == 2.0f)
						   {
                              pEdges[dwNumEdges] = wFace2;
							  dwNumEdges++;
                              pEdges[dwNumEdges] = wFace0;
							  dwNumEdges++;
						   }
						   else if(m_vertexArray1[wFace1].ny == 2.0f && m_vertexArray1[wFace2].ny == 2.0f)
						   {
                              pEdges[dwNumEdges] = wFace1;
							  dwNumEdges++;
                              pEdges[dwNumEdges] = wFace2;
							  dwNumEdges++;
						   }
						   
                           
					   }
					    
                    }

				   for( i=0; i<dwNumEdges; i++ )
                   {
                             returnBuffer[vxCount].x   = m_vertexArray1[pEdges[i]].x;//v1;
						     returnBuffer[vxCount].y   = m_vertexArray1[pEdges[i]].y;//v1;
						     returnBuffer[vxCount++].z = m_vertexArray1[pEdges[i]].z;//v1;

                             returnBuffer[vxCount].x     = m_vertexArray1[pEdges[i+1]].x;//v2;
						     returnBuffer[vxCount].y     = m_vertexArray1[pEdges[i+1]].y;//v2;
						     returnBuffer[vxCount++].z   = m_vertexArray1[pEdges[i+1]].z;//v2;

						     returnBuffer[vxCount].x     = m_vertexArray1[pEdges[i]].x - light.x;//v3;
						     returnBuffer[vxCount].y     = m_vertexArray1[pEdges[i]].y - light.y;//v3;
						     returnBuffer[vxCount++].z   = m_vertexArray1[pEdges[i]].z - light.z;//v3;

						     returnBuffer[vxCount].x     = m_vertexArray1[pEdges[i+1]].x;//v2;
						     returnBuffer[vxCount].y     = m_vertexArray1[pEdges[i+1]].y;//v2;
						     returnBuffer[vxCount++].z   = m_vertexArray1[pEdges[i+1]].z;//v2;

						     returnBuffer[vxCount].x     = m_vertexArray1[pEdges[i+1]].x - light.x;//v4;
						     returnBuffer[vxCount].y     = m_vertexArray1[pEdges[i+1]].y - light.y;//v4;
						     returnBuffer[vxCount++].z   = m_vertexArray1[pEdges[i+1]].z - light.z;//v4;

						     returnBuffer[vxCount].x     = m_vertexArray1[pEdges[i]].x - light.x;//v3;
						     returnBuffer[vxCount].y     = m_vertexArray1[pEdges[i]].y - light.y;//v3;
						     returnBuffer[vxCount++].z   = m_vertexArray1[pEdges[i]].z - light.z;//v3;
							 i++;
                   }

                  
				   *vertexCount = vxCount;
                   //Delete the temporary edge list
                   delete[] pEdges;
				   pEdges = 0;

            
                  //m_indicieBuffer->Unlock();
                  //m_vertexBuffer->Unlock();
			   //}//end lock beffer
			}
			//-----------------------------
			//-----------------------------
	    break;
		case 2:
			//-----------------------------
			//-----------------------------
            if(1)
			 {
			   
               //m_indicieBuffer->Lock(0,datasizeIndecie,(VOID**)&pIndices, m_lockFlags );
               //ABVERTEX1L* pVertices;
			   //datasize = m_vertexCount*sizeof(ABVERTEX1L);
               //if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, m_lockFlags  )==0)
			   //{//lock buffer
			
                  DWORD dwNumFaces    = m_indecieCount/3;
                  WORD* pEdges        = new WORD[dwNumFaces*6];
                 
                  DWORD dwNumEdges = 0;

                  //For each face
				  DWORD i = 0;
                  for(i=0; i<dwNumFaces; i++ )
                  {
					  wFace0 = m_indecieArray[3*i+0];
                       wFace1 = m_indecieArray[3*i+1];
                       wFace2 = m_indecieArray[3*i+2];
					   m_vertexArray2[wFace0].ny = 1.0f;
							m_vertexArray2[wFace1].ny = 1.0f;
							m_vertexArray2[wFace2].ny = 1.0f;

				  }

                  for( i=0; i<dwNumFaces; i++ )
                  {
                       wFace0 = m_indecieArray[3*i+0];
                       wFace1 = m_indecieArray[3*i+1];
                       wFace2 = m_indecieArray[3*i+2];

					   reg1.x = m_vertexArray2[wFace2].x - m_vertexArray2[wFace1].x;
					   reg1.y = m_vertexArray2[wFace2].y - m_vertexArray2[wFace1].y;
					   reg1.z = m_vertexArray2[wFace2].z - m_vertexArray2[wFace1].z;
					   reg2.x = m_vertexArray2[wFace1].x - m_vertexArray2[wFace0].x;
					   reg2.y = m_vertexArray2[wFace1].y - m_vertexArray2[wFace0].y;
					   reg2.z = m_vertexArray2[wFace1].z - m_vertexArray2[wFace0].z;

					   tempNormal.x = reg1.y * reg2.z - reg1.z * reg2.y;
                       tempNormal.y = reg1.z * reg2.x - reg1.x * reg2.z;
                       tempNormal.z = reg1.x * reg2.y - reg1.y * reg2.x;

					   //if( tempNormal.y >= 0.0f )
					   
                       if( (tempNormal.x*light.x + tempNormal.y * light.y + tempNormal.z*light.z) >= 0.0f )
                       {
						    m_vertexArray2[wFace0].ny = 2.0f;
							m_vertexArray2[wFace1].ny = 2.0f;
							m_vertexArray2[wFace2].ny = 2.0f;

							returnBuffer[vxCount].x   = m_vertexArray2[wFace0].x;//v1;
						   returnBuffer[vxCount].y   = m_vertexArray2[wFace0].y;//v1;
						   returnBuffer[vxCount++].z = m_vertexArray2[wFace0].z;//v1;

                           returnBuffer[vxCount].x     = m_vertexArray2[wFace1].x;//v2;
						   returnBuffer[vxCount].y     = m_vertexArray2[wFace1].y;//v2;
						   returnBuffer[vxCount++].z   = m_vertexArray2[wFace1].z;//v2;

						   returnBuffer[vxCount].x     = m_vertexArray2[wFace2].x;//v3;
						   returnBuffer[vxCount].y     = m_vertexArray2[wFace2].y;//v3;
						   returnBuffer[vxCount++].z   = m_vertexArray2[wFace2].z;//v3;


						  
                           returnBuffer[vxCount].x   = m_vertexArray2[wFace2].x - light.x;//v1;
						   returnBuffer[vxCount].y   = m_vertexArray2[wFace2].y - light.y;//v1;
						   returnBuffer[vxCount++].z = m_vertexArray2[wFace2].z - light.z;//v1;

                           returnBuffer[vxCount].x     = m_vertexArray2[wFace1].x - light.x;//v2;
						   returnBuffer[vxCount].y     = m_vertexArray2[wFace1].y - light.y;//v2;
						   returnBuffer[vxCount++].z   = m_vertexArray2[wFace1].z - light.z;//v2;

						   returnBuffer[vxCount].x     = m_vertexArray2[wFace0].x - light.x;//v3;
						   returnBuffer[vxCount].y     = m_vertexArray2[wFace0].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = m_vertexArray2[wFace0].z - light.z;//v3;
                       }
					   else
					   {
						   //pVertices[wFace1].ny = -1.0f;
                           
					   }
					    
                   }

				   for( i=0; i<dwNumFaces; i++ )
                   {
                       wFace0 = m_indecieArray[3*i+0];
                       wFace1 = m_indecieArray[3*i+1];
                       wFace2 = m_indecieArray[3*i+2];

					   reg1.x = m_vertexArray2[wFace2].x - m_vertexArray2[wFace1].x;
					   reg1.y = m_vertexArray2[wFace2].y - m_vertexArray2[wFace1].y;
					   reg1.z = m_vertexArray2[wFace2].z - m_vertexArray2[wFace1].z;
					   reg2.x = m_vertexArray2[wFace1].x - m_vertexArray2[wFace0].x;
					   reg2.y = m_vertexArray2[wFace1].y - m_vertexArray2[wFace0].y;
					   reg2.z = m_vertexArray2[wFace1].z - m_vertexArray2[wFace0].z;

					   tempNormal.x = reg1.y * reg2.z - reg1.z * reg2.y;
                       tempNormal.y = reg1.z * reg2.x - reg1.x * reg2.z;
                       tempNormal.z = reg1.x * reg2.y - reg1.y * reg2.x;

					   //if( tempNormal.y >= 0.0f )
					   
                       if( (tempNormal.x*light.x + tempNormal.y * light.y + tempNormal.z*light.z) >= 0.0f )
                       {
						    
                       }
					   else
					   {

						   if(m_vertexArray2[wFace0].ny == 2.0f && m_vertexArray2[wFace1].ny == 2.0f)
						   {
                              pEdges[dwNumEdges] = wFace1;
							  dwNumEdges++;
                              pEdges[dwNumEdges] = wFace0;
							  dwNumEdges++;
						   }
						   else if(m_vertexArray2[wFace0].ny == 2.0f && m_vertexArray2[wFace2].ny == 2.0f)
						   {
                              pEdges[dwNumEdges] = wFace0;
							  dwNumEdges++;
                              pEdges[dwNumEdges] = wFace2;
							  dwNumEdges++;
						   }
						   else if(m_vertexArray2[wFace1].ny == 2.0f && m_vertexArray2[wFace2].ny == 2.0f)
						   {
                              pEdges[dwNumEdges] = wFace2;
							  dwNumEdges++;
                              pEdges[dwNumEdges] = wFace1;
							  dwNumEdges++;
						   }
						   
                           
					   }
					    
                    }

				   for( i=0; i<dwNumEdges; i++ )
                   {
                             returnBuffer[vxCount].x   = m_vertexArray2[pEdges[i]].x;//v1;
						     returnBuffer[vxCount].y   = m_vertexArray2[pEdges[i]].y;//v1;
						     returnBuffer[vxCount++].z = m_vertexArray2[pEdges[i]].z;//v1;

                             returnBuffer[vxCount].x     = m_vertexArray2[pEdges[i+1]].x;//v2;
						     returnBuffer[vxCount].y     = m_vertexArray2[pEdges[i+1]].y;//v2;
						     returnBuffer[vxCount++].z   = m_vertexArray2[pEdges[i+1]].z;//v2;

						     returnBuffer[vxCount].x     = m_vertexArray2[pEdges[i]].x - light.x;//v3;
						     returnBuffer[vxCount].y     = m_vertexArray2[pEdges[i]].y - light.y;//v3;
						     returnBuffer[vxCount++].z   = m_vertexArray2[pEdges[i]].z - light.z;//v3;

						     returnBuffer[vxCount].x     = m_vertexArray2[pEdges[i+1]].x;//v2;
						     returnBuffer[vxCount].y     = m_vertexArray2[pEdges[i+1]].y;//v2;
						     returnBuffer[vxCount++].z   = m_vertexArray2[pEdges[i+1]].z;//v2;

						     returnBuffer[vxCount].x     = m_vertexArray2[pEdges[i+1]].x - light.x;//v4;
						     returnBuffer[vxCount].y     = m_vertexArray2[pEdges[i+1]].y - light.y;//v4;
						     returnBuffer[vxCount++].z   = m_vertexArray2[pEdges[i+1]].z - light.z;//v4;

						     returnBuffer[vxCount].x     = m_vertexArray2[pEdges[i]].x - light.x;//v3;
						     returnBuffer[vxCount].y     = m_vertexArray2[pEdges[i]].y - light.y;//v3;
						     returnBuffer[vxCount++].z   = m_vertexArray2[pEdges[i]].z - light.z;//v3;
							 i++;
                   }

                  
				   *vertexCount = vxCount;
                   //Delete the temporary edge list
                   delete[] pEdges;
				   pEdges = 0;

            
                  //m_indicieBuffer->Unlock();
                  //m_vertexBuffer->Unlock();
			   //}//end lock beffer
			}
			//-----------------------------
			//-----------------------------
	    break;
		case 3:
			//-----------------------------
			//-----------------------------
               ABVERTEX3L* pVertices3;
			   datasize = m_vertexCount*sizeof(ABVERTEX3L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices3, m_lockFlags )==0)
			   {//lock buffer


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
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices4, m_lockFlags )==0)
			   {//lock buffer

				  m_vertexBuffer->Unlock();
			   }//end lock beffer
		
			//-----------------------------
			//-----------------------------
	    break;
		case 5:
			//-----------------------------
			//-----------------------------
            //Create a vertex buffer
               ABVERTEX5L* pVertices5;
			   datasize = m_vertexCount*sizeof(ABVERTEX5L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices5, m_lockFlags )==0)
			   {//lock buffer
				 

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 6:
			//-----------------------------
			//-----------------------------
            //Create a vertex buffer
               ABVERTEX6L* pVertices6;
			   datasize = m_vertexCount*sizeof(ABVERTEX6L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices6, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	              //   memcpy(&pVertices6[loop],&m_origVertexArray[loop],sizeof(ABVERTEX6L));
                  
                  
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 7:
			//-----------------------------
			//-----------------------------
               ABVERTEX7L* pVertices7;
			   datasize = m_vertexCount*sizeof(ABVERTEX7L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices7, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	                  //memcpy(&pVertices7[loop],&m_origVertexArray[loop],sizeof(ABVERTEX7L));


                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 8:
			//-----------------------------
			//----------------------------
            //Create a vertex buffer
               ABVERTEX8L* pVertices8;
			   datasize = m_vertexCount*sizeof(ABVERTEX8L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices8, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	                 //memcpy(&pVertices8[loop],&m_origVertexArray[loop],sizeof(ABVERTEX8L));
                  //CopyMemory(pVertices8,m_vertexArray8,(sizeof(ABVERTEX8L)*m_vertexCount));

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
void CAdvancedVertexObj::MakeIntoComplexShadowVolume3(IDirect3DDevice9 * pd3dDevice,
													 D3DXVECTOR3 light,
													 D3DXVECTOR3 * returnBuffer,
													 DWORD * vertexCount,
													 float maxDepth,
													 WORD* pEdges)
{

	   
	   if(m_vertexCount == 0 || m_indecieCount == 0)
		  return;

	   if(!m_vertexBuffer)
		   return;

	   int datasizeIndecie = 0;

       int vxCount = *vertexCount;
	   long datasize = 0;
	   D3DXVECTOR3 reg1,reg2,tempNormal;
	   WORD wFace0,wFace1,wFace2;
       switch(m_uvCount)
	   {
	    case 0:
            //Create a vertex buffer
               ABVERTEX0L* pVertices0;
			   datasize = m_vertexCount*sizeof(ABVERTEX0L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices0, m_lockFlags )==0)
			   {//lock buffer
                  //for(int loop=0;loop<m_vertexCount;loop++)
	              //   memcpy(&pVertices0[loop],&m_origVertexArray[loop],sizeof(ABVERTEX0L));

                  //CopyMemory(pVertices0,m_vertexArray0,(sizeof(ABVERTEX0L)*m_vertexCount));

                 

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;
		case 1:
			   if(1)
			   {
                  DWORD dwNumFaces    = m_indecieCount/3;
                  //WORD* pEdges        = new WORD[dwNumFaces*6];
                  DWORD dwNumEdges    = 0;
                  DWORD i = 0;
                  //For each face
                  for( i=0; i<dwNumFaces; i++ )
                  {//face loop
                       wFace0 = m_indecieArray[3*i+0];
                       wFace1 = m_indecieArray[3*i+1];
                       wFace2 = m_indecieArray[3*i+2];

					   reg1.x = m_vertexArray1[wFace2].x - m_vertexArray1[wFace1].x;
					   reg1.y = m_vertexArray1[wFace2].y - m_vertexArray1[wFace1].y;
					   reg1.z = m_vertexArray1[wFace2].z - m_vertexArray1[wFace1].z;
					   reg2.x = m_vertexArray1[wFace1].x - m_vertexArray1[wFace0].x;
					   reg2.y = m_vertexArray1[wFace1].y - m_vertexArray1[wFace0].y;
					   reg2.z = m_vertexArray1[wFace1].z - m_vertexArray1[wFace0].z;

					   tempNormal.x = reg1.y * reg2.z - reg1.z * reg2.y;
                       tempNormal.y = reg1.z * reg2.x - reg1.x * reg2.z;
                       tempNormal.z = reg1.x * reg2.y - reg1.y * reg2.x;

					   //if( tempNormal.y >= 0.0f )
					   
                       if( (tempNormal.x*light.x + tempNormal.y * light.y + tempNormal.z*light.z) >= 0.0f )
                       {
						  
						   returnBuffer[vxCount].x   = m_vertexArray1[wFace0].x;//v1;
						   returnBuffer[vxCount].y   = m_vertexArray1[wFace0].y;//v1;
						   returnBuffer[vxCount++].z = m_vertexArray1[wFace0].z;//v1;

                           returnBuffer[vxCount].x     = m_vertexArray1[wFace1].x;//v2;
						   returnBuffer[vxCount].y     = m_vertexArray1[wFace1].y;//v2;
						   returnBuffer[vxCount++].z   = m_vertexArray1[wFace1].z;//v2;

						   returnBuffer[vxCount].x     = m_vertexArray1[wFace2].x;//v3;
						   returnBuffer[vxCount].y     = m_vertexArray1[wFace2].y;//v3;
						   returnBuffer[vxCount++].z   = m_vertexArray1[wFace2].z;//v3;


						  
                           returnBuffer[vxCount].x   = m_vertexArray1[wFace2].x - light.x;//v1;
						   returnBuffer[vxCount].y   = m_vertexArray1[wFace2].y - light.y;//v1;
						   returnBuffer[vxCount++].z = m_vertexArray1[wFace2].z - light.z;//v1;

                           returnBuffer[vxCount].x     = m_vertexArray1[wFace1].x - light.x;//v2;
						   returnBuffer[vxCount].y     = m_vertexArray1[wFace1].y - light.y;//v2;
						   returnBuffer[vxCount++].z   = m_vertexArray1[wFace1].z - light.z;//v2;

						   returnBuffer[vxCount].x     = m_vertexArray1[wFace0].x - light.x;//v3;
						   returnBuffer[vxCount].y     = m_vertexArray1[wFace0].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = m_vertexArray1[wFace0].z - light.z;//v3;

                           AddEdge( pEdges, dwNumEdges, wFace0, wFace2 );
						   
                           AddEdge( pEdges, dwNumEdges, wFace2, wFace1 );
						  
                           AddEdge( pEdges, dwNumEdges, wFace1, wFace0 );

                       }
					   else
					   {
                           

						   
					   }
					    
                   }//end face loop

				  for( i=0; i<dwNumEdges; i++ )
                  {
                             returnBuffer[vxCount].x   = m_vertexArray1[pEdges[2*i+0]].x;//v1;
						     returnBuffer[vxCount].y   = m_vertexArray1[pEdges[2*i+0]].y;//v1;
						     returnBuffer[vxCount++].z = m_vertexArray1[pEdges[2*i+0]].z;//v1;

                             returnBuffer[vxCount].x     = m_vertexArray1[pEdges[2*i+1]].x;//v2;
						     returnBuffer[vxCount].y     = m_vertexArray1[pEdges[2*i+1]].y;//v2;
						     returnBuffer[vxCount++].z   = m_vertexArray1[pEdges[2*i+1]].z;//v2;

						     returnBuffer[vxCount].x     = m_vertexArray1[pEdges[2*i+0]].x - light.x;//v3;
						     returnBuffer[vxCount].y     = m_vertexArray1[pEdges[2*i+0]].y - light.y;//v3;
						     returnBuffer[vxCount++].z   = m_vertexArray1[pEdges[2*i+0]].z - light.z;//v3;

						     returnBuffer[vxCount].x     = m_vertexArray1[pEdges[2*i+1]].x;//v2;
						     returnBuffer[vxCount].y     = m_vertexArray1[pEdges[2*i+1]].y;//v2;
						     returnBuffer[vxCount++].z   = m_vertexArray1[pEdges[2*i+1]].z;//v2;

						     returnBuffer[vxCount].x     = m_vertexArray1[pEdges[2*i+1]].x - light.x;//v4;
						     returnBuffer[vxCount].y     = m_vertexArray1[pEdges[2*i+1]].y - light.y;//v4;
						     returnBuffer[vxCount++].z   = m_vertexArray1[pEdges[2*i+1]].z - light.z;//v4;

						     returnBuffer[vxCount].x     = m_vertexArray1[pEdges[2*i+0]].x - light.x;//v3;
						     returnBuffer[vxCount].y     = m_vertexArray1[pEdges[2*i+0]].y - light.y;//v3;
						     returnBuffer[vxCount++].z   = m_vertexArray1[pEdges[2*i+0]].z - light.z;//v3;
                  }
                  //Delete the temporary edge list
                  //delete[] pEdges;

                  
				   *vertexCount = vxCount;
			   }
			//-----------------------------
	    break;
		case 2:
			//-----------------------------
			//-----------------------------
             /*datasizeIndecie = m_indecieCount * sizeof(WORD);
			 if(datasizeIndecie > 0)
			 {
			   WORD * pIndices;
               m_indicieBuffer->Lock(0,datasizeIndecie,(VOID**)&pIndices, m_lockFlags );
               ABVERTEX2L* pVertices;
			   datasize = m_vertexCount*sizeof(ABVERTEX2L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices, m_lockFlags  )==0)
			   {//lock buffer*/
			   if(1)
			   {
                  DWORD dwNumFaces    = m_indecieCount/3;
                  //WORD* pEdges        = new WORD[dwNumFaces*6];
                  DWORD dwNumEdges    = 0;
				  DWORD i = 0;

                  //For each face
                  for( i=0; i<dwNumFaces; i++ )
                  {//face loop
                       wFace0 = m_indecieArray[3*i+0];
                       wFace1 = m_indecieArray[3*i+1];
                       wFace2 = m_indecieArray[3*i+2];

					   reg1.x = m_vertexArray2[wFace2].x - m_vertexArray2[wFace1].x;
					   reg1.y = m_vertexArray2[wFace2].y - m_vertexArray2[wFace1].y;
					   reg1.z = m_vertexArray2[wFace2].z - m_vertexArray2[wFace1].z;
					   reg2.x = m_vertexArray2[wFace1].x - m_vertexArray2[wFace0].x;
					   reg2.y = m_vertexArray2[wFace1].y - m_vertexArray2[wFace0].y;
					   reg2.z = m_vertexArray2[wFace1].z - m_vertexArray2[wFace0].z;

					   tempNormal.x = reg1.y * reg2.z - reg1.z * reg2.y;
                       tempNormal.y = reg1.z * reg2.x - reg1.x * reg2.z;
                       tempNormal.z = reg1.x * reg2.y - reg1.y * reg2.x;

					   //if( tempNormal.y >= 0.0f )
					   
                       if( (tempNormal.x*light.x + tempNormal.y * light.y + tempNormal.z*light.z) >= 0.0f )
                       {
						   returnBuffer[vxCount].x   = m_vertexArray2[wFace0].x;//v1;
						   returnBuffer[vxCount].y   = m_vertexArray2[wFace0].y;//v1;
						   returnBuffer[vxCount++].z = m_vertexArray2[wFace0].z;//v1;

                           returnBuffer[vxCount].x     = m_vertexArray2[wFace1].x;//v2;
						   returnBuffer[vxCount].y     = m_vertexArray2[wFace1].y;//v2;
						   returnBuffer[vxCount++].z   = m_vertexArray2[wFace1].z;//v2;

						   returnBuffer[vxCount].x     = m_vertexArray2[wFace2].x;//v3;
						   returnBuffer[vxCount].y     = m_vertexArray2[wFace2].y;//v3;
						   returnBuffer[vxCount++].z   = m_vertexArray2[wFace2].z;//v3;


						  
                           returnBuffer[vxCount].x   = m_vertexArray2[wFace2].x - light.x;//v1;
						   returnBuffer[vxCount].y   = m_vertexArray2[wFace2].y - light.y;//v1;
						   returnBuffer[vxCount++].z = m_vertexArray2[wFace2].z - light.z;//v1;

                           returnBuffer[vxCount].x     = m_vertexArray2[wFace1].x - light.x;//v2;
						   returnBuffer[vxCount].y     = m_vertexArray2[wFace1].y - light.y;//v2;
						   returnBuffer[vxCount++].z   = m_vertexArray2[wFace1].z - light.z;//v2;

						   returnBuffer[vxCount].x     = m_vertexArray2[wFace0].x - light.x;//v3;
						   returnBuffer[vxCount].y     = m_vertexArray2[wFace0].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = m_vertexArray2[wFace0].z - light.z;//v3;

                           AddEdge( pEdges, dwNumEdges, wFace0, wFace2 );
						   
                           AddEdge( pEdges, dwNumEdges, wFace2, wFace1 );
						  
                           AddEdge( pEdges, dwNumEdges, wFace1, wFace0 );
						    

                       }
					   else
					   {
                           

						   
					   }
					    
                   }//end face loop

				  for( i=0; i<dwNumEdges; i++ )
                  {
                             returnBuffer[vxCount].x   = m_vertexArray2[pEdges[2*i+0]].x;//v1;
						     returnBuffer[vxCount].y   = m_vertexArray2[pEdges[2*i+0]].y;//v1;
						     returnBuffer[vxCount++].z = m_vertexArray2[pEdges[2*i+0]].z;//v1;

                             returnBuffer[vxCount].x     = m_vertexArray2[pEdges[2*i+1]].x;//v2;
						     returnBuffer[vxCount].y     = m_vertexArray2[pEdges[2*i+1]].y;//v2;
						     returnBuffer[vxCount++].z   = m_vertexArray2[pEdges[2*i+1]].z;//v2;

						     returnBuffer[vxCount].x     = m_vertexArray2[pEdges[2*i+0]].x - light.x;//v3;
						     returnBuffer[vxCount].y     = m_vertexArray2[pEdges[2*i+0]].y - light.y;//v3;
						     returnBuffer[vxCount++].z   = m_vertexArray2[pEdges[2*i+0]].z - light.z;//v3;

						     returnBuffer[vxCount].x     = m_vertexArray2[pEdges[2*i+1]].x;//v2;
						     returnBuffer[vxCount].y     = m_vertexArray2[pEdges[2*i+1]].y;//v2;
						     returnBuffer[vxCount++].z   = m_vertexArray2[pEdges[2*i+1]].z;//v2;

						     returnBuffer[vxCount].x     = m_vertexArray2[pEdges[2*i+1]].x - light.x;//v4;
						     returnBuffer[vxCount].y     = m_vertexArray2[pEdges[2*i+1]].y - light.y;//v4;
						     returnBuffer[vxCount++].z   = m_vertexArray2[pEdges[2*i+1]].z - light.z;//v4;

						     returnBuffer[vxCount].x     = m_vertexArray2[pEdges[2*i+0]].x - light.x;//v3;
						     returnBuffer[vxCount].y     = m_vertexArray2[pEdges[2*i+0]].y - light.y;//v3;
						     returnBuffer[vxCount++].z   = m_vertexArray2[pEdges[2*i+0]].z - light.z;//v3;
                  }
                  //Delete the temporary edge list
                  //delete[] pEdges;

                  
				   *vertexCount = vxCount;
			   }
                   //Delete the temporary edge list
                   //delete[] pEdges;

            
                  /*m_indicieBuffer->Unlock();
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			 }*/
			//-----------------------------
			//-----------------------------
	    break;
		case 3:
			//-----------------------------
			//-----------------------------
               ABVERTEX3L* pVertices3;
			   datasize = m_vertexCount*sizeof(ABVERTEX3L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices3, m_lockFlags )==0)
			   {//lock buffer


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
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices4, m_lockFlags )==0)
			   {//lock buffer

				  m_vertexBuffer->Unlock();
			   }//end lock beffer
		
			//-----------------------------
			//-----------------------------
	    break;
		case 5:
			//-----------------------------
			//-----------------------------
            //Create a vertex buffer
               ABVERTEX5L* pVertices5;
			   datasize = m_vertexCount*sizeof(ABVERTEX5L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices5, m_lockFlags )==0)
			   {//lock buffer
				 

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 6:
			//-----------------------------
			//-----------------------------
            //Create a vertex buffer
               ABVERTEX6L* pVertices6;
			   datasize = m_vertexCount*sizeof(ABVERTEX6L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices6, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	              //   memcpy(&pVertices6[loop],&m_origVertexArray[loop],sizeof(ABVERTEX6L));
                  
                  
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 7:
			//-----------------------------
			//-----------------------------
               ABVERTEX7L* pVertices7;
			   datasize = m_vertexCount*sizeof(ABVERTEX7L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices7, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	                  //memcpy(&pVertices7[loop],&m_origVertexArray[loop],sizeof(ABVERTEX7L));


                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 8:
			//-----------------------------
			//----------------------------
            //Create a vertex buffer
               ABVERTEX8L* pVertices8;
			   datasize = m_vertexCount*sizeof(ABVERTEX8L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices8, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	                 //memcpy(&pVertices8[loop],&m_origVertexArray[loop],sizeof(ABVERTEX8L));
                  //CopyMemory(pVertices8,m_vertexArray8,(sizeof(ABVERTEX8L)*m_vertexCount));

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;

	   }
}

void CAdvancedVertexObj::AddEdge( WORD* pEdges, DWORD& dwNumEdges, WORD v0, WORD v1 )
{
    //Remove interior edges (which appear in the list twice)
    for( DWORD i=0; i < dwNumEdges; i++ )
    {
        if( ( pEdges[2*i+0] == v0 && pEdges[2*i+1] == v1 ) ||
            ( pEdges[2*i+0] == v1 && pEdges[2*i+1] == v0 ) )
        {
            if( dwNumEdges > 1 )
            {
                pEdges[2*i+0] = pEdges[2*(dwNumEdges-1)+0];
                pEdges[2*i+1] = pEdges[2*(dwNumEdges-1)+1];
            }
            dwNumEdges--;
            return;
        }
    }

    pEdges[2*dwNumEdges+0] = v0;
    pEdges[2*dwNumEdges+1] = v1;
    dwNumEdges++;
}

void CAdvancedVertexObj::AddEdge2( edge_struct* pEdges, DWORD& dwNumEdges, WORD & v0, WORD & v1 )
{
    //Remove interior edges (which appear in the list twice)
    for( DWORD i=0; i < dwNumEdges; i++ )
    {
        //if( ( pEdges[i].v1 == v0 && pEdges[i].v2 == v1 ) ||
        //   ( pEdges[i].v1 == v1 && pEdges[i].v2 == v0 ) )
		//if(pEdges[i].v1 == v0)
	    //if(pEdges[i].v2 == v1)
        //if(pEdges[i].v1 == v1)
		//if(pEdges[i].v2 == v0)
		if( ( pEdges[i].v1 == v0 && pEdges[i].v2 == v1 ) ||
            ( pEdges[i].v1 == v1 && pEdges[i].v2 == v0 ) )
        {
            if( dwNumEdges > 1 )
            {
                pEdges[i].v1 = pEdges[dwNumEdges-1].v1;
                pEdges[i].v2 = pEdges[dwNumEdges-1].v2;
            }
            dwNumEdges--;
            return;
        }
    }

    pEdges[dwNumEdges].v1 = v0;
	pEdges[dwNumEdges].v2 = v1;

    dwNumEdges++;
}

void CAdvancedVertexObj::GetShadowVolumeMesh(DWORD dwNumFaces,
											 D3DXVECTOR3 light,
											 D3DXVECTOR3 * returnBuffer,
											 D3DXVECTOR3 * faceBuffer,
											 DWORD * vertexCount,
											 WORD * pEdges)
{
                  //WORD* pEdges        = new WORD[dwNumFaces*6];
                  DWORD dwNumEdges    = 0;
				  BOOL replaced = FALSE;
				  D3DXVECTOR3 reg1,reg2,tempNormal;
	              WORD wFace0,wFace1,wFace2;
				  int vxCount = 0;
				  DWORD countX=0;
				  DWORD i=0;
                  //For each face
				  
                  for( i=0; i<dwNumFaces; i++ )
                  {//face loop
                       wFace0 = (WORD)(3*i+0);
                       wFace1 = (WORD)(3*i+1);
                       wFace2 = (WORD)(3*i+2);

					   reg1.x = faceBuffer[wFace2].x - faceBuffer[wFace1].x;
					   reg1.y = faceBuffer[wFace2].y - faceBuffer[wFace1].y;
					   reg1.z = faceBuffer[wFace2].z - faceBuffer[wFace1].z;
					   reg2.x = faceBuffer[wFace1].x - faceBuffer[wFace0].x;
					   reg2.y = faceBuffer[wFace1].y - faceBuffer[wFace0].y;
					   reg2.z = faceBuffer[wFace1].z - faceBuffer[wFace0].z;

					   tempNormal.x = reg1.y * reg2.z - reg1.z * reg2.y;
                       tempNormal.y = reg1.z * reg2.x - reg1.x * reg2.z;
                       tempNormal.z = reg1.x * reg2.y - reg1.y * reg2.x;

					   //if( tempNormal.y >= 0.0f )
					   
                       if( (tempNormal.x*light.x + tempNormal.y * light.y + tempNormal.z*light.z) >= 0.0f )
                       {
						   returnBuffer[vxCount].x   = faceBuffer[wFace0].x;//v1;
						   returnBuffer[vxCount].y   = faceBuffer[wFace0].y;//v1;
						   returnBuffer[vxCount++].z = faceBuffer[wFace0].z;//v1;

                           returnBuffer[vxCount].x     = faceBuffer[wFace1].x;//v2;
						   returnBuffer[vxCount].y     = faceBuffer[wFace1].y;//v2;
						   returnBuffer[vxCount++].z   = faceBuffer[wFace1].z;//v2;

						   returnBuffer[vxCount].x     = faceBuffer[wFace2].x;//v3;
						   returnBuffer[vxCount].y     = faceBuffer[wFace2].y;//v3;
						   returnBuffer[vxCount++].z   = faceBuffer[wFace2].z;//v3;


						  
                           returnBuffer[vxCount].x   = faceBuffer[wFace2].x - light.x;//v1;
						   returnBuffer[vxCount].y   = faceBuffer[wFace2].y - light.y;//v1;
						   returnBuffer[vxCount++].z = faceBuffer[wFace2].z - light.z;//v1;

                           returnBuffer[vxCount].x     = faceBuffer[wFace1].x - light.x;//v2;
						   returnBuffer[vxCount].y     = faceBuffer[wFace1].y - light.y;//v2;
						   returnBuffer[vxCount++].z   = faceBuffer[wFace1].z - light.z;//v2;

						   returnBuffer[vxCount].x     = faceBuffer[wFace0].x - light.x;//v3;
						   returnBuffer[vxCount].y     = faceBuffer[wFace0].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = faceBuffer[wFace0].z - light.z;//v3;

                           AddEdge( pEdges, dwNumEdges, wFace0, wFace2 );
						   
                           AddEdge( pEdges, dwNumEdges, wFace2, wFace1 );
						  
                           AddEdge( pEdges, dwNumEdges, wFace1, wFace0 );
                           
                       }
					   else
					   {
                           /*returnBuffer[vxCount].x   = faceBuffer[wFace2].x;//v1;
						   returnBuffer[vxCount].y   = faceBuffer[wFace2].y;//v1;
						   returnBuffer[vxCount++].z = faceBuffer[wFace2].z;//v1;

                           returnBuffer[vxCount].x     = faceBuffer[wFace1].x;//v2;
						   returnBuffer[vxCount].y     = faceBuffer[wFace1].y;//v2;
						   returnBuffer[vxCount++].z   = faceBuffer[wFace1].z;//v2;

						   returnBuffer[vxCount].x     = faceBuffer[wFace0].x;//v3;
						   returnBuffer[vxCount].y     = faceBuffer[wFace0].y;//v3;
						   returnBuffer[vxCount++].z   = faceBuffer[wFace0].z;//v3;


						  
                           returnBuffer[vxCount].x   = faceBuffer[wFace0].x - light.x;//v1;
						   returnBuffer[vxCount].y   = faceBuffer[wFace0].y - light.y;//v1;
						   returnBuffer[vxCount++].z = faceBuffer[wFace0].z - light.z;//v1;

                           returnBuffer[vxCount].x     = faceBuffer[wFace1].x - light.x;//v2;
						   returnBuffer[vxCount].y     = faceBuffer[wFace1].y - light.y;//v2;
						   returnBuffer[vxCount++].z   = faceBuffer[wFace1].z - light.z;//v2;

						   returnBuffer[vxCount].x     = faceBuffer[wFace2].x - light.x;//v3;
						   returnBuffer[vxCount].y     = faceBuffer[wFace2].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = faceBuffer[wFace2].z - light.z;//v3;

                           //AddEdge( pEdges, dwNumEdges, wFace2, wFace0 );
						   replaced = FALSE;
						   for( countX=0; countX < dwNumEdges; countX++ )
                           {
                                if( ( pEdges[2*countX+0] == wFace2 && pEdges[2*countX+1] == wFace0 ) ||
                                  ( pEdges[2*countX+0] == wFace0 && pEdges[2*countX+1] == wFace2 ) )
                                {
                                   if( dwNumEdges > 1 )
                                   {
                                        pEdges[2*countX+0] = pEdges[2*(dwNumEdges-1)+0];
                                        pEdges[2*countX+1] = pEdges[2*(dwNumEdges-1)+1];
                                   }
                                   dwNumEdges--;
								   replaced = TRUE;
                                 break;
                                 }
                           }
						   if(!replaced)
						   {
                             pEdges[2*dwNumEdges+0] = wFace2;
                             pEdges[2*dwNumEdges+1] = wFace0;
                             dwNumEdges++;
						   }
                           //AddEdge( pEdges, dwNumEdges, wFace1, wFace2 );
						   replaced = FALSE;
						   for( countX=0; countX < dwNumEdges; countX++ )
                           {
                                if( ( pEdges[2*countX+0] == wFace1 && pEdges[2*countX+1] == wFace2 ) ||
                                  ( pEdges[2*countX+0] == wFace2 && pEdges[2*countX+1] == wFace1 ) )
                                {
                                   if( dwNumEdges > 1 )
                                   {
                                        pEdges[2*countX+0] = pEdges[2*(dwNumEdges-1)+0];
                                        pEdges[2*countX+1] = pEdges[2*(dwNumEdges-1)+1];
                                   }
                                   dwNumEdges--;
								   replaced = TRUE;
                                 break;
                                 }
                           }
						   if(!replaced)
						   {
                             pEdges[2*dwNumEdges+0] = wFace1;
                             pEdges[2*dwNumEdges+1] = wFace2;
                             dwNumEdges++;
						   }
                           //AddEdge( pEdges, dwNumEdges, wFace0, wFace1 );
                           replaced = FALSE;
						   for( countX=0; countX < dwNumEdges; countX++ )
                           {
                                if( ( pEdges[2*countX+0] == wFace0 && pEdges[2*countX+1] == wFace1 ) ||
                                  ( pEdges[2*countX+0] == wFace1 && pEdges[2*countX+1] == wFace0 ) )
                                {
                                   if( dwNumEdges > 1 )
                                   {
                                        pEdges[2*countX+0] = pEdges[2*(dwNumEdges-1)+0];
                                        pEdges[2*countX+1] = pEdges[2*(dwNumEdges-1)+1];
                                   }
                                   dwNumEdges--;
								   replaced = TRUE;
                                 break;
                                 }
                           }
						   if(!replaced)
						   {
                             pEdges[2*dwNumEdges+0] = wFace0;
                             pEdges[2*dwNumEdges+1] = wFace1;
                             dwNumEdges++;
						   }*/
						   
					   }
					    
                   }//end face loop

			      WORD indexTemp = 0;
				  for( i=0; i<dwNumEdges; i++ )
                  {
					         indexTemp = pEdges[2*i+0];
                             returnBuffer[vxCount].x   = faceBuffer[indexTemp].x;//v1;
						     returnBuffer[vxCount].y   = faceBuffer[indexTemp].y;//v1;
						     returnBuffer[vxCount++].z = faceBuffer[indexTemp].z;//v1;

							 indexTemp = pEdges[2*i+1];
                             returnBuffer[vxCount].x     = faceBuffer[indexTemp].x;//v2;
						     returnBuffer[vxCount].y     = faceBuffer[indexTemp].y;//v2;
						     returnBuffer[vxCount++].z   = faceBuffer[indexTemp].z;//v2;

							 indexTemp = pEdges[2*i+0];
						     returnBuffer[vxCount].x     = faceBuffer[indexTemp].x - light.x;//v3;
						     returnBuffer[vxCount].y     = faceBuffer[indexTemp].y - light.y;//v3;
						     returnBuffer[vxCount++].z   = faceBuffer[indexTemp].z - light.z;//v3;

							 indexTemp = pEdges[2*i+1];
						     returnBuffer[vxCount].x     = faceBuffer[indexTemp].x;//v2;
						     returnBuffer[vxCount].y     = faceBuffer[indexTemp].y;//v2;
						     returnBuffer[vxCount++].z   = faceBuffer[indexTemp].z;//v2;

							 indexTemp = pEdges[2*i+1];
						     returnBuffer[vxCount].x     = faceBuffer[indexTemp].x - light.x;//v4;
						     returnBuffer[vxCount].y     = faceBuffer[indexTemp].y - light.y;//v4;
						     returnBuffer[vxCount++].z   = faceBuffer[indexTemp].z - light.z;//v4;

							 indexTemp = pEdges[2*i+0];
						     returnBuffer[vxCount].x     = faceBuffer[indexTemp].x - light.x;//v3;
						     returnBuffer[vxCount].y     = faceBuffer[indexTemp].y - light.y;//v3;
						     returnBuffer[vxCount++].z   = faceBuffer[indexTemp].z - light.z;//v3;
                  }
                  //Delete the temporary edge list
                  //delete[] pEdges;
				  *vertexCount = vxCount;
}
void CAdvancedVertexObj::GetShadowVolumeMesh2(DWORD dwNumFaces,
											  D3DXVECTOR3 light,
											  D3DXVECTOR3 * returnBuffer,
											  D3DXVECTOR3 * faceBuffer,
											  DWORD * vertexCount,
											  edge_struct * pEdges)
{
                  //WORD* pEdges        = new WORD[dwNumFaces*6];
                  DWORD dwNumEdges    = 0;
				  BOOL replaced = FALSE;
				  D3DXVECTOR3 reg1,reg2,tempNormal;
	              WORD wFace0,wFace1,wFace2;
				  int vxCount = 0;
				  DWORD countX=0;
				  DWORD i=0;
                  //For each face
				  
                  for( i=0; i<dwNumFaces; i++ )//indecie loop
                  {//face loop
                       wFace0 = (WORD)(3*i+0);
                       wFace1 = (WORD)(3*i+1);
                       wFace2 = (WORD)(3*i+2);

					   reg1.x = faceBuffer[wFace2].x - faceBuffer[wFace1].x;
					   reg1.y = faceBuffer[wFace2].y - faceBuffer[wFace1].y;
					   reg1.z = faceBuffer[wFace2].z - faceBuffer[wFace1].z;
					   reg2.x = faceBuffer[wFace1].x - faceBuffer[wFace0].x;
					   reg2.y = faceBuffer[wFace1].y - faceBuffer[wFace0].y;
					   reg2.z = faceBuffer[wFace1].z - faceBuffer[wFace0].z;

					   tempNormal.x = reg1.y * reg2.z - reg1.z * reg2.y;
                       tempNormal.y = reg1.z * reg2.x - reg1.x * reg2.z;
                       tempNormal.z = reg1.x * reg2.y - reg1.y * reg2.x;

					   //if( tempNormal.y >= 0.0f )
					   
                       if( (tempNormal.x*light.x + tempNormal.y * light.y + tempNormal.z*light.z) >= 0.0f )
                       {
						   returnBuffer[vxCount].x   = faceBuffer[wFace0].x;//v1;
						   returnBuffer[vxCount].y   = faceBuffer[wFace0].y;//v1;
						   returnBuffer[vxCount++].z = faceBuffer[wFace0].z;//v1;

                           returnBuffer[vxCount].x     = faceBuffer[wFace1].x;//v2;
						   returnBuffer[vxCount].y     = faceBuffer[wFace1].y;//v2;
						   returnBuffer[vxCount++].z   = faceBuffer[wFace1].z;//v2;

						   returnBuffer[vxCount].x     = faceBuffer[wFace2].x;//v3;
						   returnBuffer[vxCount].y     = faceBuffer[wFace2].y;//v3;
						   returnBuffer[vxCount++].z   = faceBuffer[wFace2].z;//v3;


						  
                           returnBuffer[vxCount].x   = faceBuffer[wFace2].x - light.x;//v1;
						   returnBuffer[vxCount].y   = faceBuffer[wFace2].y - light.y;//v1;
						   returnBuffer[vxCount++].z = faceBuffer[wFace2].z - light.z;//v1;

                           returnBuffer[vxCount].x     = faceBuffer[wFace1].x - light.x;//v2;
						   returnBuffer[vxCount].y     = faceBuffer[wFace1].y - light.y;//v2;
						   returnBuffer[vxCount++].z   = faceBuffer[wFace1].z - light.z;//v2;

						   returnBuffer[vxCount].x     = faceBuffer[wFace0].x - light.x;//v3;
						   returnBuffer[vxCount].y     = faceBuffer[wFace0].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = faceBuffer[wFace0].z - light.z;//v3;

                           AddEdge2( pEdges, dwNumEdges, wFace0, wFace2 );
						   
                           AddEdge2( pEdges, dwNumEdges, wFace2, wFace1 );
						  
                           AddEdge2( pEdges, dwNumEdges, wFace1, wFace0 );

                           
                       }
					   else
					   {
                           
						   
					   }
					    
                   }//end face loop

				  for( i=0; i<dwNumEdges; i++ )
                  {
                             returnBuffer[vxCount].x   = faceBuffer[pEdges[i].v1].x;//v1;
						     returnBuffer[vxCount].y   = faceBuffer[pEdges[i].v1].y;//v1;
						     returnBuffer[vxCount++].z = faceBuffer[pEdges[i].v1].z;//v1;

                             returnBuffer[vxCount].x     = faceBuffer[pEdges[i].v2].x;//v2;
						     returnBuffer[vxCount].y     = faceBuffer[pEdges[i].v2].y;//v2;
						     returnBuffer[vxCount++].z   = faceBuffer[pEdges[i].v2].z;//v2;

						     returnBuffer[vxCount].x     = faceBuffer[pEdges[i].v1].x - light.x;//v3;
						     returnBuffer[vxCount].y     = faceBuffer[pEdges[i].v1].y - light.y;//v3;
						     returnBuffer[vxCount++].z   = faceBuffer[pEdges[i].v1].z - light.z;//v3;

						     returnBuffer[vxCount].x     = faceBuffer[pEdges[i].v2].x;//v2;
						     returnBuffer[vxCount].y     = faceBuffer[pEdges[i].v2].y;//v2;
						     returnBuffer[vxCount++].z   = faceBuffer[pEdges[i].v2].z;//v2;

						     returnBuffer[vxCount].x     = faceBuffer[pEdges[i].v2].x - light.x;//v4;
						     returnBuffer[vxCount].y     = faceBuffer[pEdges[i].v2].y - light.y;//v4;
						     returnBuffer[vxCount++].z   = faceBuffer[pEdges[i].v2].z - light.z;//v4;

						     returnBuffer[vxCount].x     = faceBuffer[pEdges[i].v1].x - light.x;//v3;
						     returnBuffer[vxCount].y     = faceBuffer[pEdges[i].v1].y - light.y;//v3;
						     returnBuffer[vxCount++].z   = faceBuffer[pEdges[i].v1].z - light.z;//v3;
                  }
                  //Delete the temporary edge list
                  //delete[] pEdges;
				  *vertexCount = vxCount;
}
void CAdvancedVertexObj::GetShadowVolumeMesh3(DWORD dwNumFaces,
											  D3DXVECTOR3 light,
											  D3DXVECTOR3 * returnBuffer,
											  ABSPECFACE * faceBuffer,
											  DWORD * vertexCount,
											  edge_struct * pEdges,
											  WORD * indecieBuffer)
{
	
                  //WORD* pEdges        = new WORD[dwNumFaces*6];
                  DWORD dwNumEdges    = 0;
				  BOOL replaced = FALSE;
				  D3DXVECTOR3 reg1,reg2,tempNormal;
	              WORD wFace0,wFace1,wFace2;
				  int vxCount = 0;
				  DWORD countX=0;
				  DWORD i=0;
                  //For each face
				  
                  for( i=0; i<dwNumFaces; i++ )
                  {//face loop
                       wFace0 = indecieBuffer[3*i+0];
                       wFace1 = indecieBuffer[3*i+1];
                       wFace2 = indecieBuffer[3*i+2];

					   reg1.x = faceBuffer[wFace2].x - faceBuffer[wFace1].x;
					   reg1.y = faceBuffer[wFace2].y - faceBuffer[wFace1].y;
					   reg1.z = faceBuffer[wFace2].z - faceBuffer[wFace1].z;
					   reg2.x = faceBuffer[wFace1].x - faceBuffer[wFace0].x;
					   reg2.y = faceBuffer[wFace1].y - faceBuffer[wFace0].y;
					   reg2.z = faceBuffer[wFace1].z - faceBuffer[wFace0].z;

					   tempNormal.x = reg1.y * reg2.z - reg1.z * reg2.y;
                       tempNormal.y = reg1.z * reg2.x - reg1.x * reg2.z;
                       tempNormal.z = reg1.x * reg2.y - reg1.y * reg2.x;

					   //if( tempNormal.y >= 0.0f )
					   
                       if( (tempNormal.x*light.x + tempNormal.y * light.y + tempNormal.z*light.z) >= 0.0f )
                       {
						   returnBuffer[vxCount].x   = faceBuffer[wFace0].x;//v1;
						   returnBuffer[vxCount].y   = faceBuffer[wFace0].y;//v1;
						   returnBuffer[vxCount++].z = faceBuffer[wFace0].z;//v1;

                           returnBuffer[vxCount].x     = faceBuffer[wFace1].x;//v2;
						   returnBuffer[vxCount].y     = faceBuffer[wFace1].y;//v2;
						   returnBuffer[vxCount++].z   = faceBuffer[wFace1].z;//v2;

						   returnBuffer[vxCount].x     = faceBuffer[wFace2].x;//v3;
						   returnBuffer[vxCount].y     = faceBuffer[wFace2].y;//v3;
						   returnBuffer[vxCount++].z   = faceBuffer[wFace2].z;//v3;


						  
                           returnBuffer[vxCount].x   = faceBuffer[wFace2].x - light.x;//v1;
						   returnBuffer[vxCount].y   = faceBuffer[wFace2].y - light.y;//v1;
						   returnBuffer[vxCount++].z = faceBuffer[wFace2].z - light.z;//v1;

                           returnBuffer[vxCount].x     = faceBuffer[wFace1].x - light.x;//v2;
						   returnBuffer[vxCount].y     = faceBuffer[wFace1].y - light.y;//v2;
						   returnBuffer[vxCount++].z   = faceBuffer[wFace1].z - light.z;//v2;

						   returnBuffer[vxCount].x     = faceBuffer[wFace0].x - light.x;//v3;
						   returnBuffer[vxCount].y     = faceBuffer[wFace0].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = faceBuffer[wFace0].z - light.z;//v3;

                           AddEdge2( pEdges, dwNumEdges, wFace0, wFace2 );
						   
                           AddEdge2( pEdges, dwNumEdges, wFace2, wFace1 );
						  
                           AddEdge2( pEdges, dwNumEdges, wFace1, wFace0 );

                           
                       }
					   else
					   {
                           
						   
					   }
					    
                   }//end face loop

				  for( i=0; i<dwNumEdges; i++ )
                  {
                             returnBuffer[vxCount].x   = faceBuffer[pEdges[i].v1].x;//v1;
						     returnBuffer[vxCount].y   = faceBuffer[pEdges[i].v1].y;//v1;
						     returnBuffer[vxCount++].z = faceBuffer[pEdges[i].v1].z;//v1;

                             returnBuffer[vxCount].x     = faceBuffer[pEdges[i].v2].x;//v2;
						     returnBuffer[vxCount].y     = faceBuffer[pEdges[i].v2].y;//v2;
						     returnBuffer[vxCount++].z   = faceBuffer[pEdges[i].v2].z;//v2;

						     returnBuffer[vxCount].x     = faceBuffer[pEdges[i].v1].x - light.x;//v3;
						     returnBuffer[vxCount].y     = faceBuffer[pEdges[i].v1].y - light.y;//v3;
						     returnBuffer[vxCount++].z   = faceBuffer[pEdges[i].v1].z - light.z;//v3;

						     returnBuffer[vxCount].x     = faceBuffer[pEdges[i].v2].x;//v2;
						     returnBuffer[vxCount].y     = faceBuffer[pEdges[i].v2].y;//v2;
						     returnBuffer[vxCount++].z   = faceBuffer[pEdges[i].v2].z;//v2;

						     returnBuffer[vxCount].x     = faceBuffer[pEdges[i].v2].x - light.x;//v4;
						     returnBuffer[vxCount].y     = faceBuffer[pEdges[i].v2].y - light.y;//v4;
						     returnBuffer[vxCount++].z   = faceBuffer[pEdges[i].v2].z - light.z;//v4;

						     returnBuffer[vxCount].x     = faceBuffer[pEdges[i].v1].x - light.x;//v3;
						     returnBuffer[vxCount].y     = faceBuffer[pEdges[i].v1].y - light.y;//v3;
						     returnBuffer[vxCount++].z   = faceBuffer[pEdges[i].v1].z - light.z;//v3;
                  }
                  //Delete the temporary edge list
                  //delete[] pEdges;
				  *vertexCount = vxCount;
}
void CAdvancedVertexObj::GetShadowVolumeMesh4(DWORD dwNumFaces,
											  D3DXVECTOR3 light,
											  D3DXVECTOR3 * returnBuffer,
											  ABSPECFACE * faceBuffer,
											  DWORD * vertexCount,
											  edge_struct * pEdges,
											  WORD * indecieBuffer)
{
	
                  //WORD* pEdges        = new WORD[dwNumFaces*6];
                  DWORD dwNumEdges    = 0;
				  BOOL replaced = FALSE;
				  D3DXVECTOR3 reg1,reg2,tempNormal;
	              WORD wFace0,wFace1,wFace2;
				  int vxCount = 0;
				  DWORD countX=0;
				  DWORD i=0;
                  //For each face
				  for(i=0; i<dwNumFaces; i++)
				  {
                      wFace0 = indecieBuffer[3*i+0];
                      wFace1 = indecieBuffer[3*i+1];
                      wFace2 = indecieBuffer[3*i+2];
					  faceBuffer[wFace0].sw = 0;
					  faceBuffer[wFace1].sw = 0;
					  faceBuffer[wFace2].sw = 0;
				  }
					  //faceBuffer[j].sw = 0;


				  for( i=0; i<dwNumFaces; i++ )
                  {//face loop
                       wFace0 = indecieBuffer[3*i+0];
                       wFace1 = indecieBuffer[3*i+1];
                       wFace2 = indecieBuffer[3*i+2];

					   reg1.x = faceBuffer[wFace2].x - faceBuffer[wFace1].x;
					   reg1.y = faceBuffer[wFace2].y - faceBuffer[wFace1].y;
					   reg1.z = faceBuffer[wFace2].z - faceBuffer[wFace1].z;
					   reg2.x = faceBuffer[wFace1].x - faceBuffer[wFace0].x;
					   reg2.y = faceBuffer[wFace1].y - faceBuffer[wFace0].y;
					   reg2.z = faceBuffer[wFace1].z - faceBuffer[wFace0].z;

					   tempNormal.x = reg1.y * reg2.z - reg1.z * reg2.y;
                       tempNormal.y = reg1.z * reg2.x - reg1.x * reg2.z;
                       tempNormal.z = reg1.x * reg2.y - reg1.y * reg2.x;

					   //if( tempNormal.y >= 0.0f )
					   
                       if( (tempNormal.x*light.x + tempNormal.y * light.y + tempNormal.z*light.z) >= 0.0f )
                       {
					
                              if(faceBuffer[wFace0].sw != 1)
                                 faceBuffer[wFace0].sw = -1;
							  if(faceBuffer[wFace1].sw != 1)
                                 faceBuffer[wFace1].sw = -1;
						      if(faceBuffer[wFace2].sw != 1)
                                 faceBuffer[wFace2].sw = -1;
                           
                       }
					   else
					   {
                           //flag edges
						   
						      faceBuffer[wFace0].sw = 1;
							  faceBuffer[wFace1].sw = 1;
							  faceBuffer[wFace2].sw = 1;
						   
						   
					   }
					    
                   }//end face loop

				  
                  for( i=0; i<dwNumFaces; i++ )
                  {//face loop
                       wFace0 = indecieBuffer[3*i+0];
                       wFace1 = indecieBuffer[3*i+1];
                       wFace2 = indecieBuffer[3*i+2];

					   reg1.x = faceBuffer[wFace2].x - faceBuffer[wFace1].x;
					   reg1.y = faceBuffer[wFace2].y - faceBuffer[wFace1].y;
					   reg1.z = faceBuffer[wFace2].z - faceBuffer[wFace1].z;
					   reg2.x = faceBuffer[wFace1].x - faceBuffer[wFace0].x;
					   reg2.y = faceBuffer[wFace1].y - faceBuffer[wFace0].y;
					   reg2.z = faceBuffer[wFace1].z - faceBuffer[wFace0].z;

					   tempNormal.x = reg1.y * reg2.z - reg1.z * reg2.y;
                       tempNormal.y = reg1.z * reg2.x - reg1.x * reg2.z;
                       tempNormal.z = reg1.x * reg2.y - reg1.y * reg2.x;

					   //if( tempNormal.y >= 0.0f )
					   
                       if( (tempNormal.x*light.x + tempNormal.y * light.y + tempNormal.z*light.z) >= 0.0f )
                       {
						   returnBuffer[vxCount].x   = faceBuffer[wFace0].x;//v1;
						   returnBuffer[vxCount].y   = faceBuffer[wFace0].y;//v1;
						   returnBuffer[vxCount++].z = faceBuffer[wFace0].z;//v1;

                           returnBuffer[vxCount].x     = faceBuffer[wFace1].x;//v2;
						   returnBuffer[vxCount].y     = faceBuffer[wFace1].y;//v2;
						   returnBuffer[vxCount++].z   = faceBuffer[wFace1].z;//v2;

						   returnBuffer[vxCount].x     = faceBuffer[wFace2].x;//v3;
						   returnBuffer[vxCount].y     = faceBuffer[wFace2].y;//v3;
						   returnBuffer[vxCount++].z   = faceBuffer[wFace2].z;//v3;


						  
                           returnBuffer[vxCount].x   = faceBuffer[wFace2].x - light.x;//v1;
						   returnBuffer[vxCount].y   = faceBuffer[wFace2].y - light.y;//v1;
						   returnBuffer[vxCount++].z = faceBuffer[wFace2].z - light.z;//v1;

                           returnBuffer[vxCount].x     = faceBuffer[wFace1].x - light.x;//v2;
						   returnBuffer[vxCount].y     = faceBuffer[wFace1].y - light.y;//v2;
						   returnBuffer[vxCount++].z   = faceBuffer[wFace1].z - light.z;//v2;

						   returnBuffer[vxCount].x     = faceBuffer[wFace0].x - light.x;//v3;
						   returnBuffer[vxCount].y     = faceBuffer[wFace0].y - light.y;//v3;
						   returnBuffer[vxCount++].z   = faceBuffer[wFace0].z - light.z;//v3;

                           //flag edges
						   if(faceBuffer[wFace0].sw > -1 && faceBuffer[wFace2].sw > -1)
						      AddEdge2( pEdges, dwNumEdges, wFace0, wFace2 );
		
						   if(faceBuffer[wFace2].sw > -1 && faceBuffer[wFace1].sw > -1)
						      AddEdge2( pEdges, dwNumEdges, wFace2, wFace1 );
						   
						   if(faceBuffer[wFace1].sw > -1 && faceBuffer[wFace0].sw > -1)
						      AddEdge2( pEdges, dwNumEdges, wFace1, wFace0 );
						   
						   
						   //AddEdge2( pEdges, dwNumEdges, wFace0, wFace2 );
						   
                           //AddEdge2( pEdges, dwNumEdges, wFace2, wFace1 );
						  
                           //AddEdge2( pEdges, dwNumEdges, wFace1, wFace0 );

                           
                       }
					   else
					   {
                           
						   
					   }
					    
                   }//end face loop

				  for( i=0; i<dwNumEdges; i++ )
                  {
                             returnBuffer[vxCount].x   = faceBuffer[pEdges[i].v1].x;//v1;
						     returnBuffer[vxCount].y   = faceBuffer[pEdges[i].v1].y;//v1;
						     returnBuffer[vxCount++].z = faceBuffer[pEdges[i].v1].z;//v1;

                             returnBuffer[vxCount].x     = faceBuffer[pEdges[i].v2].x;//v2;
						     returnBuffer[vxCount].y     = faceBuffer[pEdges[i].v2].y;//v2;
						     returnBuffer[vxCount++].z   = faceBuffer[pEdges[i].v2].z;//v2;

						     returnBuffer[vxCount].x     = faceBuffer[pEdges[i].v1].x - light.x;//v3;
						     returnBuffer[vxCount].y     = faceBuffer[pEdges[i].v1].y - light.y;//v3;
						     returnBuffer[vxCount++].z   = faceBuffer[pEdges[i].v1].z - light.z;//v3;

						     returnBuffer[vxCount].x     = faceBuffer[pEdges[i].v2].x;//v2;
						     returnBuffer[vxCount].y     = faceBuffer[pEdges[i].v2].y;//v2;
						     returnBuffer[vxCount++].z   = faceBuffer[pEdges[i].v2].z;//v2;

						     returnBuffer[vxCount].x     = faceBuffer[pEdges[i].v2].x - light.x;//v4;
						     returnBuffer[vxCount].y     = faceBuffer[pEdges[i].v2].y - light.y;//v4;
						     returnBuffer[vxCount++].z   = faceBuffer[pEdges[i].v2].z - light.z;//v4;

						     returnBuffer[vxCount].x     = faceBuffer[pEdges[i].v1].x - light.x;//v3;
						     returnBuffer[vxCount].y     = faceBuffer[pEdges[i].v1].y - light.y;//v3;
						     returnBuffer[vxCount++].z   = faceBuffer[pEdges[i].v1].z - light.z;//v3;
                  }
                  //Delete the temporary edge list
                  //delete[] pEdges;
				  *vertexCount = vxCount;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 void CAdvancedVertexObj::GetFaceData(D3DXVECTOR3 * faceBuffer,
									  DWORD * vCount)
{
   if(m_vertexCount == 0)
	  return;

   if(m_indecieCount == 0)
	  return;

   int curVCount = *vCount;
   
   int loop=0;
	   switch(m_uvCount)
	   {
	    case 0:
            
            for(loop=0;loop<m_indecieCount;loop++)
			{
                faceBuffer[curVCount].x = m_vertexArray0[m_indecieArray[loop]].x;
	            faceBuffer[curVCount].y = m_vertexArray0[m_indecieArray[loop]].y;
				faceBuffer[curVCount++].z = m_vertexArray0[m_indecieArray[loop]].z;
			}

	    break;
	   
	    case 1:

            for(loop=0;loop<m_indecieCount;loop++)
			{
                faceBuffer[curVCount].x = m_vertexArray1[m_indecieArray[loop]].x;
	            faceBuffer[curVCount].y = m_vertexArray1[m_indecieArray[loop]].y;
				faceBuffer[curVCount++].z = m_vertexArray1[m_indecieArray[loop]].z;
			}
	    break;
	   
	    case 2:

            for(loop=0;loop<m_indecieCount;loop++)
			{
                faceBuffer[curVCount].x = m_vertexArray2[m_indecieArray[loop]].x;
	            faceBuffer[curVCount].y = m_vertexArray2[m_indecieArray[loop]].y;
				faceBuffer[curVCount++].z = m_vertexArray2[m_indecieArray[loop]].z;
			}
	    break;
	   
	    case 3:
            for(loop=0;loop<m_indecieCount;loop++)
			{
                faceBuffer[curVCount].x = m_vertexArray3[m_indecieArray[loop]].x;
	            faceBuffer[curVCount].y = m_vertexArray3[m_indecieArray[loop]].y;
				faceBuffer[curVCount++].z = m_vertexArray3[m_indecieArray[loop]].z;
			}
	    break;

		case 4:
            for(loop=0;loop<m_indecieCount;loop++)
			{
                faceBuffer[curVCount].x = m_vertexArray4[m_indecieArray[loop]].x;
	            faceBuffer[curVCount].y = m_vertexArray4[m_indecieArray[loop]].y;
				faceBuffer[curVCount++].z = m_vertexArray4[m_indecieArray[loop]].z;
			}
	    break;

		case 5:
            for(loop=0;loop<m_indecieCount;loop++)
			{
                faceBuffer[curVCount].x = m_vertexArray5[m_indecieArray[loop]].x;
	            faceBuffer[curVCount].y = m_vertexArray5[m_indecieArray[loop]].y;
				faceBuffer[curVCount++].z = m_vertexArray5[m_indecieArray[loop]].z;
			}
	    break;

		case 6:
            for(loop=0;loop<m_indecieCount;loop++)
			{
                faceBuffer[curVCount].x = m_vertexArray6[m_indecieArray[loop]].x;
	            faceBuffer[curVCount].y = m_vertexArray6[m_indecieArray[loop]].y;
				faceBuffer[curVCount++].z = m_vertexArray6[m_indecieArray[loop]].z;
			}
	    break;

		case 7:
            for(loop=0;loop<m_indecieCount;loop++)
			{
                faceBuffer[curVCount].x = m_vertexArray7[m_indecieArray[loop]].x;
	            faceBuffer[curVCount].y = m_vertexArray7[m_indecieArray[loop]].y;
				faceBuffer[curVCount++].z = m_vertexArray7[m_indecieArray[loop]].z;
			}
	    break;

		case 8:
            for(loop=0;loop<m_indecieCount;loop++)
			{
                faceBuffer[loop].x = m_vertexArray7[m_indecieArray[loop]].x;
	            faceBuffer[loop].y = m_vertexArray7[m_indecieArray[loop]].y;
				faceBuffer[curVCount++].z = m_vertexArray7[m_indecieArray[loop]].z;
			}
	    break;
	   
    }
    *vCount = curVCount;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 void CAdvancedVertexObj::GetFaceDataIndecie(ABSPECFACE * faceBuffer,
									         DWORD * vCount,
											 DWORD * indecieCount,
                                             WORD * indecieBuffer
									         )
{
   if(m_vertexCount == 0)
	  return;

   if(m_indecieCount == 0)
	  return;

   int curVCount      = *vCount;
   int iCountLocal    = *indecieCount;
   int iCountOffset    = *vCount;
   
   int loop=0;

   for(loop=0;loop<m_indecieCount;loop++)
   {
       indecieBuffer[iCountLocal++]   = m_indecieArray[loop]+iCountOffset;
   }


	   switch(m_uvCount)
	   {
	    case 0:
            
            for(loop=0;loop<m_vertexCount;loop++)
			{
                faceBuffer[curVCount].x   = m_vertexArray0[loop].x;
	            faceBuffer[curVCount].y   = m_vertexArray0[loop].y;
				faceBuffer[curVCount++].z = m_vertexArray0[loop].z;
			}

	    break;
	   
	    case 1:

            for(loop=0;loop<m_vertexCount;loop++)
			{
                faceBuffer[curVCount].x = m_vertexArray1[loop].x;
	            faceBuffer[curVCount].y = m_vertexArray1[loop].y;
				faceBuffer[curVCount++].z = m_vertexArray1[loop].z;
			}
			

	    break;
	   
	    case 2:

            for(loop=0;loop<m_vertexCount;loop++)
			{
                faceBuffer[curVCount].x = m_vertexArray2[loop].x;
	            faceBuffer[curVCount].y = m_vertexArray2[loop].y;
				faceBuffer[curVCount++].z = m_vertexArray2[loop].z;
			}
	    break;
	   
	    case 3:
            for(loop=0;loop<m_vertexCount;loop++)
			{
                faceBuffer[curVCount].x = m_vertexArray3[loop].x;
	            faceBuffer[curVCount].y = m_vertexArray3[loop].y;
				faceBuffer[curVCount++].z = m_vertexArray3[loop].z;
			}
	    break;

		case 4:
            for(loop=0;loop<m_vertexCount;loop++)
			{
                faceBuffer[curVCount].x = m_vertexArray4[loop].x;
	            faceBuffer[curVCount].y = m_vertexArray4[loop].y;
				faceBuffer[curVCount++].z = m_vertexArray4[loop].z;
			}
	    break;

		case 5:
            for(loop=0;loop<m_vertexCount;loop++)
			{
                faceBuffer[curVCount].x = m_vertexArray5[loop].x;
	            faceBuffer[curVCount].y = m_vertexArray5[loop].y;
				faceBuffer[curVCount++].z = m_vertexArray5[loop].z;
			}
	    break;

		case 6:
            for(loop=0;loop<m_vertexCount;loop++)
			{
                faceBuffer[curVCount].x = m_vertexArray6[loop].x;
	            faceBuffer[curVCount].y = m_vertexArray6[loop].y;
				faceBuffer[curVCount++].z = m_vertexArray6[loop].z;
			}
	    break;

		case 7:
            for(loop=0;loop<m_vertexCount;loop++)
			{
                faceBuffer[curVCount].x = m_vertexArray7[loop].x;
	            faceBuffer[curVCount].y = m_vertexArray7[loop].y;
				faceBuffer[curVCount++].z = m_vertexArray7[loop].z;
			}
	    break;

		case 8:
            for(loop=0;loop<m_vertexCount;loop++)
			{
                faceBuffer[loop].x = m_vertexArray7[loop].x;
	            faceBuffer[loop].y = m_vertexArray7[loop].y;
				faceBuffer[curVCount++].z = m_vertexArray7[loop].z;
			}
	    break;
	   
    }
    *vCount = curVCount;
	*indecieCount = iCountLocal;
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
       
	   int tempUVCount = m_uvCount;
	   
	   switch(tempUVCount)
	   {
	    case 0:
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX0L),
                                               vUsage    , 0,//(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX0),
                                               memManage  ,
											   &m_vertexBuffer,
											   NULL)!=0)
											   return;
			}
            //Create a vertex buffer
               ABVERTEX0L* pVertices0;
			   datasize = m_vertexCount*sizeof(ABVERTEX0L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices0, m_lockFlags )==0)
			   {//lock buffer
                  //for(int loop=0;loop<m_vertexCount;loop++)
	              //   memcpy(&pVertices0[loop],&m_origVertexArray[loop],sizeof(ABVERTEX0L));

                  CopyMemory(pVertices0,m_vertexArray0,(sizeof(ABVERTEX0L)*m_vertexCount));

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;
		case 1:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX1L),
                                               vUsage   , 0,//(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1),
                                               memManage  ,
											   &m_vertexBuffer,
											   NULL))
											   return;
			}
            //Create a vertex buffer
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
                                                 vUsage   , 0,//(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2),
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
	              //   memcpy(&pVertices2[loop],&m_origVertexArray[loop],sizeof(ABVERTEX2L));

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
                                               vUsage , 0,//(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX3),
                                               memManage ,
											   &m_vertexBuffer,
											   NULL)!=0)
											   return;
			}
            //Create a vertex buffer
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
                                               vUsage   , 0,//(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX4),
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
	               //  memcpy(&pVertices4[loop],&m_origVertexArray[loop],sizeof(ABVERTEX4L));
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
                                               vUsage   , 0,//(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX5),
                                               memManage  ,
											   &m_vertexBuffer,
											   NULL)!=0)
											   return;
			}
            //Create a vertex buffer
               ABVERTEX5L* pVertices5;
			   datasize = m_vertexCount*sizeof(ABVERTEX5L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices5, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	              //   memcpy(&pVertices5[loop],&m_origVertexArray[loop],sizeof(ABVERTEX5L));
 
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
                                               vUsage   , 0,//(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX6),
                                               memManage  ,
											   &m_vertexBuffer ,
											   NULL)!=0)
											   return;
			}
            //Create a vertex buffer
               ABVERTEX6L* pVertices6;
			   datasize = m_vertexCount*sizeof(ABVERTEX6L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices6, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	              //   memcpy(&pVertices6[loop],&m_origVertexArray[loop],sizeof(ABVERTEX6L));
                  
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
                                                vUsage   , 0,//(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX7),
                                                memManage  ,
												&m_vertexBuffer ,
												NULL)!=0)
												return;
			}
            //Create a vertex buffer
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
                                               vUsage   , 0,//(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX8),
                                               memManage  ,
											   &m_vertexBuffer ,
											   NULL)!=0)
											   return;
			}
            //Create a vertex buffer
               ABVERTEX8L* pVertices8;
			   datasize = m_vertexCount*sizeof(ABVERTEX8L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices8, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	                 //memcpy(&pVertices8[loop],&m_origVertexArray[loop],sizeof(ABVERTEX8L));
                  CopyMemory(pVertices8,m_vertexArray8,(sizeof(ABVERTEX8L)*m_vertexCount));

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 18:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX0Lb),
                                               vUsage   , 0,
                                               memManage  ,
											   &m_vertexBuffer ,
											   NULL)!=0)
											   return;
			}
            //Create a vertex buffer
               ABVERTEX0Lb* pVertices18;
			   datasize = m_vertexCount*sizeof(ABVERTEX0Lb);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices18, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	                 //memcpy(&pVertices8[loop],&m_origVertexArray[loop],sizeof(ABVERTEX8L));
                  CopyMemory(pVertices18,m_vertexArray0t,(sizeof(ABVERTEX0Lb)*m_vertexCount));

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 19:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX1Lb),
                                               vUsage   , 0,
                                               memManage  ,
											   &m_vertexBuffer ,
											   NULL)!=0)
											   return;
			}
            //Create a vertex buffer
               ABVERTEX1Lb* pVertices19;
			   datasize = m_vertexCount*sizeof(ABVERTEX1Lb);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices19, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	                 //memcpy(&pVertices8[loop],&m_origVertexArray[loop],sizeof(ABVERTEX8L));
                  CopyMemory(pVertices19,m_vertexArray1t,(sizeof(ABVERTEX1Lb)*m_vertexCount));

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 20:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX2Lb),
                                               vUsage   , 0,
                                               memManage  ,
											   &m_vertexBuffer ,
											   NULL)!=0)
											   return;
			}
            //Create a vertex buffer
               ABVERTEX2Lb* pVertices20;
			   datasize = m_vertexCount*sizeof(ABVERTEX2Lb);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices20, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	                 //memcpy(&pVertices8[loop],&m_origVertexArray[loop],sizeof(ABVERTEX8L));
                  CopyMemory(pVertices20,m_vertexArray2t,(sizeof(ABVERTEX2Lb)*m_vertexCount));

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 21:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX3Lb),
                                               vUsage   , 0,
                                               memManage  ,
											   &m_vertexBuffer ,
											   NULL)!=0)
											   return;
			}
            //Create a vertex buffer
               ABVERTEX3Lb* pVertices21;
			   datasize = m_vertexCount*sizeof(ABVERTEX3Lb);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices21, m_lockFlags )==0)
			   {//lock buffer
                  CopyMemory(pVertices21,m_vertexArray3t,(sizeof(ABVERTEX3Lb)*m_vertexCount));
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 22:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX4Lb),
                                               vUsage   , 0,
                                               memManage  ,
											   &m_vertexBuffer ,
											   NULL)!=0)
											   return;
			}
            //Create a vertex buffer
               ABVERTEX4Lb* pVertices22;
			   datasize = m_vertexCount*sizeof(ABVERTEX4Lb);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices22, m_lockFlags )==0)
			   {//lock buffer
                  CopyMemory(pVertices22,m_vertexArray4t,(sizeof(ABVERTEX4Lb)*m_vertexCount));
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 23:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX5Lb),
                                               vUsage   , 0,
                                               memManage  ,
											   &m_vertexBuffer ,
											   NULL)!=0)
											   return;
			}
            //Create a vertex buffer
               ABVERTEX5Lb* pVertices23;
			   datasize = m_vertexCount*sizeof(ABVERTEX5Lb);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices23, m_lockFlags )==0)
			   {//lock buffer
                  CopyMemory(pVertices23,m_vertexArray5t,(sizeof(ABVERTEX5Lb)*m_vertexCount));
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 24:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX6Lb),
                                               vUsage   , 0,
                                               memManage  ,
											   &m_vertexBuffer ,
											   NULL)!=0)
											   return;
			}
            //Create a vertex buffer
               ABVERTEX6Lb* pVertices24;
			   datasize = m_vertexCount*sizeof(ABVERTEX6Lb);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices24, m_lockFlags )==0)
			   {//lock buffer
                  CopyMemory(pVertices24,m_vertexArray6t,(sizeof(ABVERTEX6Lb)*m_vertexCount));
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 50:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX3Lb),
                                               vUsage   , 0,
                                               memManage  ,
											   &m_vertexBuffer ,
											   NULL)!=0)
											   return;
			}
            //Create a vertex buffer
               ABVERTEX3Lb* pVertices50;
			   datasize = m_vertexCount*sizeof(ABVERTEX3Lb);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices50, m_lockFlags )==0)
			   {//lock buffer
				  //for(int loop=0;loop<m_vertexCount;loop++)
	                 //memcpy(&pVertices8[loop],&m_origVertexArray[loop],sizeof(ABVERTEX8L));
                  CopyMemory(pVertices50,m_vertexArray3t,(sizeof(ABVERTEX3Lb)*m_vertexCount));

                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 70:
			//-----------------------------
			//-----------------------------
			if(!m_vertexBuffer){
			   if(pd3dDevice->CreateVertexBuffer( m_vertexCount*sizeof(ABVERTEX1Lx),
                                               vUsage   , 0,
                                               memManage  ,
											   &m_vertexBuffer ,
											   NULL)!=0)
											   return;
			}
            //Create a vertex buffer
               ABVERTEX1Lx* pVerticesX;
			   datasize = m_vertexCount*sizeof(ABVERTEX1Lx);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVerticesX, m_lockFlags )==0)
			   {//lock buffer
                  CopyMemory(pVerticesX,m_vertexArray1x,(sizeof(ABVERTEX1Lx)*m_vertexCount));
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
                                               vUsage    , 0,//(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX0),
                                               memManage  ,
											   &m_vertexBuffer,
											   NULL)!=0)
											   return;
			}
            //Create a vertex buffer
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
                                               vUsage   , 0,//(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1),
                                               memManage  ,
											   &m_vertexBuffer,
											   NULL))
											   return;
			}
            //Create a vertex buffer
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
                                                 vUsage   , 0,//(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2),
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
            //Create a vertex buffer
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
            //Create a vertex buffer
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
            //Create a vertex buffer
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
            //Create a vertex buffer
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
            //Create a vertex buffer
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
            //Create a vertex buffer
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
            //Create a vertex buffer
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
void CAdvancedVertexObj::ReinitializeDxVbuffersPosNormOnly(IDirect3DDevice9 * pd3dDevice)
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
					   pVertices0[loop].nx = m_vertexArray0[loop].nx;
					   pVertices0[loop].ny = m_vertexArray0[loop].ny;
					   pVertices0[loop].nz = m_vertexArray0[loop].nz;

				   }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;
		case 1:
			//-----------------------------
			//-----------------------------
            //Create a vertex buffer
               ABVERTEX1L* pVertices1;
			   datasize = m_vertexCount*sizeof(ABVERTEX1L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices1, 0  )==0)
			   {//lock buffer
				   for(int loop=0;loop<m_vertexCount;loop++){
	                   pVertices1[loop].x = m_vertexArray1[loop].x;
					   pVertices1[loop].y = m_vertexArray1[loop].y;
					   pVertices1[loop].z = m_vertexArray1[loop].z;
					   pVertices1[loop].nx = m_vertexArray1[loop].nx;
					   pVertices1[loop].ny = m_vertexArray1[loop].ny;
					   pVertices1[loop].nz = m_vertexArray1[loop].nz;
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
					   pVertices2[loop].nx = m_vertexArray2[loop].nx;
					   pVertices2[loop].ny = m_vertexArray2[loop].ny;
					   pVertices2[loop].nz = m_vertexArray2[loop].nz;
				   }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 3:
			//-----------------------------
			//-----------------------------
            //Create a vertex buffer
               ABVERTEX3L* pVertices3;
			   datasize = m_vertexCount*sizeof(ABVERTEX3L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices3, 0 )==0)
			   {//lock buffer
				   for(int loop=0;loop<m_vertexCount;loop++){
                       pVertices3[loop].x = m_vertexArray3[loop].x;
					   pVertices3[loop].y = m_vertexArray3[loop].y;
					   pVertices3[loop].z = m_vertexArray3[loop].z;
					   pVertices3[loop].nx = m_vertexArray3[loop].nx;
					   pVertices3[loop].ny = m_vertexArray3[loop].ny;
					   pVertices3[loop].nz = m_vertexArray3[loop].nz;
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
					  pVertices4[loop].nx = m_vertexArray4[loop].nx;
					  pVertices4[loop].ny = m_vertexArray4[loop].ny;
					  pVertices4[loop].nz = m_vertexArray4[loop].nz;
				  }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
		
			//-----------------------------
			//-----------------------------
	    break;
		case 5:
			//-----------------------------
			//-----------------------------
            //Create a vertex buffer
               ABVERTEX5L* pVertices5;
			   datasize = m_vertexCount*sizeof(ABVERTEX5L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices5, 0 )==0)
			   {//lock buffer
				   for(int loop=0;loop<m_vertexCount;loop++){
	                  pVertices5[loop].x = m_vertexArray5[loop].x;
					  pVertices5[loop].y = m_vertexArray5[loop].y;
					  pVertices5[loop].z = m_vertexArray5[loop].z;
					  pVertices5[loop].nx = m_vertexArray5[loop].nx;
					  pVertices5[loop].ny = m_vertexArray5[loop].ny;
					  pVertices5[loop].nz = m_vertexArray5[loop].nz;
				   }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 6:
			//-----------------------------
			//-----------------------------
            //Create a vertex buffer
               ABVERTEX6L* pVertices6;
			   datasize = m_vertexCount*sizeof(ABVERTEX6L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices6, 0 )==0)
			   {//lock buffer
				   for(int loop=0;loop<m_vertexCount;loop++){
	                  pVertices6[loop].x = m_vertexArray6[loop].x;
					  pVertices6[loop].y = m_vertexArray6[loop].y;
					  pVertices6[loop].z = m_vertexArray6[loop].z;
					  pVertices6[loop].nx = m_vertexArray6[loop].nx;
					  pVertices6[loop].ny = m_vertexArray6[loop].ny;
					  pVertices6[loop].nz = m_vertexArray6[loop].nz;
				   }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 7:
			//-----------------------------
			//-----------------------------
            //Create a vertex buffer
               ABVERTEX7L* pVertices7;
			   datasize = m_vertexCount*sizeof(ABVERTEX7L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices7, 0 )==0)
			   {//lock buffer
				   for(int loop=0;loop<m_vertexCount;loop++){
	                  pVertices7[loop].x = m_vertexArray7[loop].x;
					  pVertices7[loop].y = m_vertexArray7[loop].y;
					  pVertices7[loop].z = m_vertexArray7[loop].z;
					  pVertices7[loop].nx = m_vertexArray7[loop].nx;
					  pVertices7[loop].ny = m_vertexArray7[loop].ny;
					  pVertices7[loop].nz = m_vertexArray7[loop].nz;
				   }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 8:
			//-----------------------------
			//-----------------------------
            //Create a vertex buffer
               ABVERTEX8L* pVertices8;
			   datasize = m_vertexCount*sizeof(ABVERTEX8L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices8, 0 )==0)
			   {//lock buffer
				  for(int loop=0;loop<m_vertexCount;loop++){
	                  pVertices8[loop].x = m_vertexArray8[loop].x;
					  pVertices8[loop].y = m_vertexArray8[loop].y;
					  pVertices8[loop].z = m_vertexArray8[loop].z;
					  pVertices8[loop].nx = m_vertexArray8[loop].nx;
					  pVertices8[loop].ny = m_vertexArray8[loop].ny;
					  pVertices8[loop].nz = m_vertexArray8[loop].nz;
				   }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 19:
			//-----------------------------
			ABVERTEX1Lb* pVertices19;
			   datasize = m_vertexCount*sizeof(ABVERTEX1Lb);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices19, 0 )==0)
			   {//lock buffer
				  for(int loop=0;loop<m_vertexCount;loop++){
	                  pVertices19[loop].x = m_vertexArray1t[loop].x;
					  pVertices19[loop].y = m_vertexArray1t[loop].y;
					  pVertices19[loop].z = m_vertexArray1t[loop].z;
					  pVertices19[loop].nx = m_vertexArray1t[loop].nx;
					  pVertices19[loop].ny = m_vertexArray1t[loop].ny;
					  pVertices19[loop].nz = m_vertexArray1t[loop].nz;
					  pVertices19[loop].tX = m_vertexArray1t[loop].tX;
					  pVertices19[loop].tY = m_vertexArray1t[loop].tY;
					  pVertices19[loop].tZ = m_vertexArray1t[loop].tZ;
				   }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;
		case 20:
			//-----------------------------
			ABVERTEX2Lb* pVertices20;
			   datasize = m_vertexCount*sizeof(ABVERTEX2Lb);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices20, 0 )==0)
			   {//lock buffer
				  for(int loop=0;loop<m_vertexCount;loop++){
	                  pVertices20[loop].x = m_vertexArray2t[loop].x;
					  pVertices20[loop].y = m_vertexArray2t[loop].y;
					  pVertices20[loop].z = m_vertexArray2t[loop].z;
					  pVertices20[loop].nx = m_vertexArray2t[loop].nx;
					  pVertices20[loop].ny = m_vertexArray2t[loop].ny;
					  pVertices20[loop].nz = m_vertexArray2t[loop].nz;
					  pVertices20[loop].tX = m_vertexArray2t[loop].tX;
					  pVertices20[loop].tY = m_vertexArray2t[loop].tY;
					  pVertices20[loop].tZ = m_vertexArray2t[loop].tZ;
				   }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;
		case 21:
			//-----------------------------
			ABVERTEX3Lb* pVertices21;
			   datasize = m_vertexCount*sizeof(ABVERTEX3Lb);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices21, 0 )==0)
			   {//lock buffer
				  for(int loop=0;loop<m_vertexCount;loop++){
	                  pVertices21[loop].x = m_vertexArray3t[loop].x;
					  pVertices21[loop].y = m_vertexArray3t[loop].y;
					  pVertices21[loop].z = m_vertexArray3t[loop].z;
					  pVertices21[loop].nx = m_vertexArray3t[loop].nx;
					  pVertices21[loop].ny = m_vertexArray3t[loop].ny;
					  pVertices21[loop].nz = m_vertexArray3t[loop].nz;
					  pVertices21[loop].tX = m_vertexArray3t[loop].tX;
					  pVertices21[loop].tY = m_vertexArray3t[loop].tY;
					  pVertices21[loop].tZ = m_vertexArray3t[loop].tZ;
				   }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;
		case 22:
			//-----------------------------
			ABVERTEX3Lb* pVertices22;
			   datasize = m_vertexCount*sizeof(ABVERTEX3Lb);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices22, 0 )==0)
			   {//lock buffer
				  for(int loop=0;loop<m_vertexCount;loop++){
	                  pVertices22[loop].x = m_vertexArray4t[loop].x;
					  pVertices22[loop].y = m_vertexArray4t[loop].y;
					  pVertices22[loop].z = m_vertexArray4t[loop].z;
					  pVertices22[loop].nx = m_vertexArray4t[loop].nx;
					  pVertices22[loop].ny = m_vertexArray4t[loop].ny;
					  pVertices22[loop].nz = m_vertexArray4t[loop].nz;
					  pVertices22[loop].tX = m_vertexArray4t[loop].tX;
					  pVertices22[loop].tY = m_vertexArray4t[loop].tY;
					  pVertices22[loop].tZ = m_vertexArray4t[loop].tZ;
				   }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;
		case 23:
			//-----------------------------
			ABVERTEX4Lb* pVertices23;
			   datasize = m_vertexCount*sizeof(ABVERTEX4Lb);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices23, 0 )==0)
			   {//lock buffer
				  for(int loop=0;loop<m_vertexCount;loop++){
	                  pVertices23[loop].x = m_vertexArray5t[loop].x;
					  pVertices23[loop].y = m_vertexArray5t[loop].y;
					  pVertices23[loop].z = m_vertexArray5t[loop].z;
					  pVertices23[loop].nx = m_vertexArray5t[loop].nx;
					  pVertices23[loop].ny = m_vertexArray5t[loop].ny;
					  pVertices23[loop].nz = m_vertexArray5t[loop].nz;
					  pVertices23[loop].tX = m_vertexArray5t[loop].tX;
					  pVertices23[loop].tY = m_vertexArray5t[loop].tY;
					  pVertices23[loop].tZ = m_vertexArray5t[loop].tZ;
				   }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;
		case 24:
			//-----------------------------
			ABVERTEX5Lb* pVertices24;
			   datasize = m_vertexCount*sizeof(ABVERTEX5Lb);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices24, 0 )==0)
			   {//lock buffer
				  for(int loop=0;loop<m_vertexCount;loop++){
	                  pVertices24[loop].x = m_vertexArray6t[loop].x;
					  pVertices24[loop].y = m_vertexArray6t[loop].y;
					  pVertices24[loop].z = m_vertexArray6t[loop].z;
					  pVertices24[loop].nx = m_vertexArray6t[loop].nx;
					  pVertices24[loop].ny = m_vertexArray6t[loop].ny;
					  pVertices24[loop].nz = m_vertexArray6t[loop].nz;
					  pVertices24[loop].tX = m_vertexArray6t[loop].tX;
					  pVertices24[loop].tY = m_vertexArray6t[loop].tY;
					  pVertices24[loop].tZ = m_vertexArray6t[loop].tZ;
				   }
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;

	   }
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CAdvancedVertexObj::ReinitializeDxVbuffersFS(IDirect3DDevice9 * pd3dDevice)
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
				  CopyMemory(pVertices0,m_vertexArray0,datasize);
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;
		case 1:
			//-----------------------------
			//-----------------------------
            //Create a vertex buffer
               ABVERTEX1L* pVertices1;
			   datasize = m_vertexCount*sizeof(ABVERTEX1L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices1, 0  )==0)
			   {//lock buffer
			      CopyMemory(pVertices1,m_vertexArray1,datasize);
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
				  CopyMemory(pVertices2,m_vertexArray2,datasize);
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 3:
			//-----------------------------
			//-----------------------------
            //Create a vertex buffer
               ABVERTEX3L* pVertices3;
			   datasize = m_vertexCount*sizeof(ABVERTEX3L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices3, 0 )==0)
			   {//lock buffer
				   CopyMemory(pVertices3,m_vertexArray3,datasize);
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
				  CopyMemory(pVertices4,m_vertexArray4,datasize);
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
		
			//-----------------------------
			//-----------------------------
	    break;
		case 5:
			//-----------------------------
			//-----------------------------
            //Create a vertex buffer
               ABVERTEX5L* pVertices5;
			   datasize = m_vertexCount*sizeof(ABVERTEX5L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices5, 0 )==0)
			   {//lock buffer
				   CopyMemory(pVertices5,m_vertexArray5,datasize);
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 6:
			//-----------------------------
			//-----------------------------
            //Create a vertex buffer
               ABVERTEX6L* pVertices6;
			   datasize = m_vertexCount*sizeof(ABVERTEX6L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices6, 0 )==0)
			   {//lock buffer
				   CopyMemory(pVertices6,m_vertexArray6,datasize);
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 7:
			//-----------------------------
			//-----------------------------
            //Create a vertex buffer
               ABVERTEX7L* pVertices7;
			   datasize = m_vertexCount*sizeof(ABVERTEX7L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices7, 0 )==0)
			   {//lock buffer
				   CopyMemory(pVertices7,m_vertexArray7,datasize);
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 8:
			//-----------------------------
			//-----------------------------
            //Create a vertex buffer
               ABVERTEX8L* pVertices8;
			   datasize = m_vertexCount*sizeof(ABVERTEX8L);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices8, 0 )==0)
			   {//lock buffer
				   CopyMemory(pVertices8,m_vertexArray8,datasize);
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
			//-----------------------------
			//-----------------------------
	    break;
		case 19:
			//-----------------------------
			ABVERTEX1Lb* pVertices19;
			   datasize = m_vertexCount*sizeof(ABVERTEX1Lb);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices19, 0 )==0)
			   {//lock buffer
			       CopyMemory(pVertices19,m_vertexArray1t,datasize);
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;
		case 20:
			//-----------------------------
			ABVERTEX2Lb* pVertices20;
			   datasize = m_vertexCount*sizeof(ABVERTEX2Lb);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices20, 0 )==0)
			   {//lock buffer
				  CopyMemory(pVertices20,m_vertexArray2t,datasize);
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;
		case 21:
			//-----------------------------
			ABVERTEX3Lb* pVertices21;
			   datasize = m_vertexCount*sizeof(ABVERTEX3Lb);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices21, 0 )==0)
			   {//lock buffer
				  CopyMemory(pVertices21,m_vertexArray3t,datasize);
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;
		case 22:
			//-----------------------------
			ABVERTEX3Lb* pVertices22;
			   datasize = m_vertexCount*sizeof(ABVERTEX3Lb);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices22, 0 )==0)
			   {//lock buffer
				  CopyMemory(pVertices22,m_vertexArray4t,datasize);
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;
		case 23:
			//-----------------------------
			ABVERTEX4Lb* pVertices23;
			   datasize = m_vertexCount*sizeof(ABVERTEX4Lb);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices23, 0 )==0)
			   {//lock buffer
				 CopyMemory(pVertices23,m_vertexArray5t,datasize);
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;
		case 24:
			//-----------------------------
			ABVERTEX5Lb* pVertices24;
			   datasize = m_vertexCount*sizeof(ABVERTEX5Lb);
               if(m_vertexBuffer->Lock( 0, datasize,(VOID**)&pVertices24, 0 )==0)
			   {//lock buffer
				  CopyMemory(pVertices24,m_vertexArray6t,datasize);
                  m_vertexBuffer->Unlock();
			   }//end lock beffer
	    break;

	   }
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CAdvancedVertexObj::CloneDataOnly(CAdvancedVertexObj * clone2)
{
       if(m_indecieCount < 1)
          return;

       clone2->m_uvCount             = m_uvCount;
	   clone2->m_indecieCount        = m_indecieCount;
	   clone2->m_vertexCount         = m_vertexCount;
	   clone2->m_tangentExists       = m_tangentExists;

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
		case 19:
			if(clone2->m_vertexArray1t){
			   delete [] clone2->m_vertexArray1t;
			   clone2->m_vertexArray1t = 0;
			}
			clone2->m_vertexArray1t = new ABVERTEX1Lb[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray1t,m_vertexArray1t,sizeof(ABVERTEX1Lb)*m_vertexCount);
	    break;
		case 20:
			if(clone2->m_vertexArray2t){
			   delete [] clone2->m_vertexArray2t;
			   clone2->m_vertexArray2t = 0;
			}
			clone2->m_vertexArray2t = new ABVERTEX2Lb[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray2t,m_vertexArray2t,sizeof(ABVERTEX2Lb)*m_vertexCount);
	    break;
		case 21:
			if(clone2->m_vertexArray3t){
			   delete [] clone2->m_vertexArray3t;
			   clone2->m_vertexArray3t = 0;
			}
			clone2->m_vertexArray3t = new ABVERTEX3Lb[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray3t,m_vertexArray3t,sizeof(ABVERTEX3Lb)*m_vertexCount);
	    break;
		case 22:
			if(clone2->m_vertexArray4t){
			   delete [] clone2->m_vertexArray4t;
			   clone2->m_vertexArray4t = 0;
			}
			clone2->m_vertexArray4t = new ABVERTEX4Lb[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray4t,m_vertexArray4t,sizeof(ABVERTEX4Lb)*m_vertexCount);
	    break;
		case 23:
			if(clone2->m_vertexArray5t){
			   delete [] clone2->m_vertexArray5t;
			   clone2->m_vertexArray5t = 0;
			}
			clone2->m_vertexArray5t = new ABVERTEX5Lb[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray5t,m_vertexArray5t,sizeof(ABVERTEX5Lb)*m_vertexCount);
	    break;
		case 24:
			if(clone2->m_vertexArray6t){
			   delete [] clone2->m_vertexArray6t;
			   clone2->m_vertexArray6t = 0;
			}
			clone2->m_vertexArray6t = new ABVERTEX6Lb[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray6t,m_vertexArray6t,sizeof(ABVERTEX6Lb)*m_vertexCount);
	    break;
		case 50:
			if(clone2->m_vertexArray3t){
			   delete [] clone2->m_vertexArray3t;
			   clone2->m_vertexArray3t = 0;
			}
			clone2->m_vertexArray3t = new ABVERTEX3Lb[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray3t,m_vertexArray3t,sizeof(ABVERTEX3Lb)*m_vertexCount);
	    break;
		case 70:
			if(clone2->m_vertexArray1x){
			   delete [] clone2->m_vertexArray1x;
			   clone2->m_vertexArray1x = 0;
			}
			clone2->m_vertexArray1x = new ABVERTEX1Lx[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray1x,m_vertexArray1x,sizeof(ABVERTEX1Lx)*m_vertexCount);
	    break;
	   
	   }
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CAdvancedVertexObj::CloneMinimum2(CAdvancedVertexObj * clone2,
									   LPDIRECT3DDEVICE9 g_pd3dDevice)
{
       if(m_indecieCount < 1)
          return;


       clone2->m_uvCount             = m_uvCount;
	   clone2->m_indecieCount        = m_indecieCount;
	   clone2->m_vertexCount         = m_vertexCount;
	   clone2->m_tangentExists       = m_tangentExists;

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
		case 19:
			if(clone2->m_vertexArray1t){
			   delete [] clone2->m_vertexArray1t;
			   clone2->m_vertexArray1t = 0;
			}
			clone2->m_vertexArray1t = new ABVERTEX1Lb[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray1t,m_vertexArray1t,sizeof(ABVERTEX1Lb)*m_vertexCount);
	    break;
		case 20:
			if(clone2->m_vertexArray2t){
			   delete [] clone2->m_vertexArray2t;
			   clone2->m_vertexArray2t = 0;
			}
			clone2->m_vertexArray2t = new ABVERTEX2Lb[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray2t,m_vertexArray2t,sizeof(ABVERTEX2Lb)*m_vertexCount);
	    break;
		case 21:
			if(clone2->m_vertexArray3t){
			   delete [] clone2->m_vertexArray3t;
			   clone2->m_vertexArray3t = 0;
			}
			clone2->m_vertexArray3t = new ABVERTEX3Lb[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray3t,m_vertexArray3t,sizeof(ABVERTEX3Lb)*m_vertexCount);
	    break;
		case 22:
			if(clone2->m_vertexArray4t){
			   delete [] clone2->m_vertexArray4t;
			   clone2->m_vertexArray4t = 0;
			}
			clone2->m_vertexArray4t = new ABVERTEX4Lb[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray4t,m_vertexArray4t,sizeof(ABVERTEX4Lb)*m_vertexCount);
	    break;
		case 23:
			if(clone2->m_vertexArray5t){
			   delete [] clone2->m_vertexArray5t;
			   clone2->m_vertexArray5t = 0;
			}
			clone2->m_vertexArray5t = new ABVERTEX5Lb[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray5t,m_vertexArray5t,sizeof(ABVERTEX5Lb)*m_vertexCount);
	    break;
		case 24:
			if(clone2->m_vertexArray6t){
			   delete [] clone2->m_vertexArray6t;
			   clone2->m_vertexArray6t = 0;
			}
			clone2->m_vertexArray6t = new ABVERTEX6Lb[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray6t,m_vertexArray6t,sizeof(ABVERTEX6Lb)*m_vertexCount);
	    break;
		case 50:
			if(clone2->m_vertexArray3t){
			   delete [] clone2->m_vertexArray3t;
			   clone2->m_vertexArray3t = 0;
			}
			clone2->m_vertexArray3t = new ABVERTEX3Lb[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray3t,m_vertexArray3t,sizeof(ABVERTEX3Lb)*m_vertexCount);
	    break;
		case 70:
			if(clone2->m_vertexArray1x){
			   delete [] clone2->m_vertexArray1x;
			   clone2->m_vertexArray1x = 0;
			}
			clone2->m_vertexArray1x = new ABVERTEX1Lx[m_vertexCount];
	        CopyMemory(clone2->m_vertexArray1x,m_vertexArray1x,sizeof(ABVERTEX1Lx)*m_vertexCount);
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
D3DVECTOR CAdvancedVertexObj::GetTangentVector(int vIndex)
{
D3DVECTOR retVect;
ZeroMemory(&retVect,sizeof(D3DVECTOR));
       switch(m_uvCount)
	   {
	    
		case 19:
			retVect.x = m_vertexArray1t[vIndex].tX;
			retVect.y = m_vertexArray1t[vIndex].tY;
			retVect.z = m_vertexArray1t[vIndex].tZ;
		break;
		case 20:
            retVect.x = m_vertexArray2t[vIndex].tX;
			retVect.y = m_vertexArray2t[vIndex].tY;
			retVect.z = m_vertexArray2t[vIndex].tZ;
		break;
		case 21:
            retVect.x = m_vertexArray3t[vIndex].tX;
			retVect.y = m_vertexArray3t[vIndex].tY;
			retVect.z = m_vertexArray3t[vIndex].tZ;
		break;
		case 22:
            retVect.x = m_vertexArray4t[vIndex].tX;
			retVect.y = m_vertexArray4t[vIndex].tY;
			retVect.z = m_vertexArray4t[vIndex].tZ;
		break;
		case 23:
            retVect.x = m_vertexArray5t[vIndex].tX;
			retVect.y = m_vertexArray5t[vIndex].tY;
			retVect.z = m_vertexArray5t[vIndex].tZ;
		break;
		case 24:
            retVect.x = m_vertexArray6t[vIndex].tX;
			retVect.y = m_vertexArray6t[vIndex].tY;
			retVect.z = m_vertexArray6t[vIndex].tZ;
		break;

	   }
	   return retVect;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
 void CAdvancedVertexObj::SetPositionXYZNXNYNZTGT(int & vIndex,
											      float & x,float & y,float & z,
											      float & nx,float & ny,float & nz,
												  float & tx,float & ty,float & tz)
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
		case 19:
            m_vertexArray1t[vIndex].x = x;
			m_vertexArray1t[vIndex].y = y;
			m_vertexArray1t[vIndex].z = z;
			m_vertexArray1t[vIndex].nx = nx;
			m_vertexArray1t[vIndex].ny = ny;
			m_vertexArray1t[vIndex].nz = nz;
			m_vertexArray1t[vIndex].tX = tx;
			m_vertexArray1t[vIndex].tY = ty;
			m_vertexArray1t[vIndex].tZ = tz;
		break;
		case 20:
            m_vertexArray2t[vIndex].x = x;
			m_vertexArray2t[vIndex].y = y;
			m_vertexArray2t[vIndex].z = z;
			m_vertexArray2t[vIndex].nx = nx;
			m_vertexArray2t[vIndex].ny = ny;
			m_vertexArray2t[vIndex].nz = nz;
			m_vertexArray2t[vIndex].tX = tx;
			m_vertexArray2t[vIndex].tY = ty;
			m_vertexArray2t[vIndex].tZ = tz;
		break;
		case 21:
            m_vertexArray3t[vIndex].x = x;
			m_vertexArray3t[vIndex].y = y;
			m_vertexArray3t[vIndex].z = z;
			m_vertexArray3t[vIndex].nx = nx;
			m_vertexArray3t[vIndex].ny = ny;
			m_vertexArray3t[vIndex].nz = nz;
			m_vertexArray3t[vIndex].tX = tx;
			m_vertexArray3t[vIndex].tY = ty;
			m_vertexArray3t[vIndex].tZ = tz;
		break;
		case 22:
            m_vertexArray4t[vIndex].x = x;
			m_vertexArray4t[vIndex].y = y;
			m_vertexArray4t[vIndex].z = z;
			m_vertexArray4t[vIndex].nx = nx;
			m_vertexArray4t[vIndex].ny = ny;
			m_vertexArray4t[vIndex].nz = nz;
			m_vertexArray4t[vIndex].tX = tx;
			m_vertexArray4t[vIndex].tY = ty;
			m_vertexArray4t[vIndex].tZ = tz;
		break;
		case 23:
            m_vertexArray5t[vIndex].x = x;
			m_vertexArray5t[vIndex].y = y;
			m_vertexArray5t[vIndex].z = z;
			m_vertexArray5t[vIndex].nx = nx;
			m_vertexArray5t[vIndex].ny = ny;
			m_vertexArray5t[vIndex].nz = nz;
			m_vertexArray5t[vIndex].tX = tx;
			m_vertexArray5t[vIndex].tY = ty;
			m_vertexArray5t[vIndex].tZ = tz;
		break;
		case 24:
            m_vertexArray6t[vIndex].x = x;
			m_vertexArray6t[vIndex].y = y;
			m_vertexArray6t[vIndex].z = z;
			m_vertexArray6t[vIndex].nx = nx;
			m_vertexArray6t[vIndex].ny = ny;
			m_vertexArray6t[vIndex].nz = nz;
			m_vertexArray6t[vIndex].tX = tx;
			m_vertexArray6t[vIndex].tY = ty;
			m_vertexArray6t[vIndex].tZ = tz;
		break;

	   }
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void * CAdvancedVertexObj::GetVertexBuf()
{ //too slow
	switch(m_uvCount)
	   {
	    case 0:return m_vertexArray0;
		case 1:return m_vertexArray1;
		case 2:return m_vertexArray2;
		case 3:return m_vertexArray3;
		case 4:return m_vertexArray4;
		case 5:return m_vertexArray5;
		case 6:return m_vertexArray6;
		case 7:return m_vertexArray7;
		case 8:return m_vertexArray8;
		case 19:return m_vertexArray1t;
		case 20:return m_vertexArray2t;
		case 21:return m_vertexArray3t;
		case 22:return m_vertexArray4t;
		case 23:return m_vertexArray5t;
		case 24:return m_vertexArray6t;
	   }
	return 0;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
int CAdvancedVertexObj::GetVertexSize()
{ //too slow
	switch(m_uvCount)
	   {
	    case 0:return sizeof(ABVERTEX0L);
		case 1:return sizeof(ABVERTEX1L);
		case 2:return sizeof(ABVERTEX2L);
		case 3:return sizeof(ABVERTEX3L);
		case 4:return sizeof(ABVERTEX4L);
		case 5:return sizeof(ABVERTEX5L);
		case 6:return sizeof(ABVERTEX6L);
		case 7:return sizeof(ABVERTEX7L);
		case 8:return sizeof(ABVERTEX8L);
		case 19:return sizeof(ABVERTEX1Lb);
		case 20:return sizeof(ABVERTEX2Lb);
		case 21:return sizeof(ABVERTEX3Lb);
		case 22:return sizeof(ABVERTEX4Lb);
		case 23:return sizeof(ABVERTEX5Lb);
		case 24:return sizeof(ABVERTEX6Lb);
	   }
	return 0;
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
int CAdvancedVertexObj::Render(LPDIRECT3DDEVICE9 g_pd3dDevice, int & vertexType)
{
	int polysIRendered = 0;
	if(!m_vertexBuffer)
		return polysIRendered;

	if(!m_indicieBuffer)
		return polysIRendered;

	if(!m_vertexCount)
		return polysIRendered;

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

			polysIRendered = m_indecieCount;
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
			polysIRendered = m_indecieCount;
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
			polysIRendered = m_indecieCount;
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
			polysIRendered = m_indecieCount;
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
			polysIRendered = m_indecieCount;
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
			polysIRendered = m_indecieCount;
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
			polysIRendered = m_indecieCount;
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
			polysIRendered = m_indecieCount;
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
			polysIRendered = m_indecieCount;
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
			polysIRendered = m_indecieCount;
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
			polysIRendered = m_indecieCount;
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
			polysIRendered = m_indecieCount;
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
			polysIRendered = m_indecieCount;
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
			polysIRendered = m_indecieCount;
		break;
		case 19:
            //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX8) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX1Lb) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
			polysIRendered = m_indecieCount;
		break;
		case 20:
            //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX8) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX2Lb) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
			polysIRendered = m_indecieCount;
			
		break;
		case 21:
            //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX8) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX3Lb) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
			polysIRendered = m_indecieCount;
			
		break;
		case 22:
            //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX8) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX4Lb) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
			polysIRendered = m_indecieCount;
			
		break;
		case 23:
            //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX8) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX5Lb) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
			polysIRendered = m_indecieCount;
			
		break;
		case 24:
            //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX8) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX6Lb) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
			polysIRendered = m_indecieCount;
			
		break;

		case 50:
            //g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX8) );
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX3Lb) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			/*g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );*/
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               m_indecieCount-2//UINT PrimitiveCount
                                               );

			polysIRendered = (m_indecieCount-2)*3;
		break;
		case 70:
            g_pd3dDevice->SetStreamSource( 0, m_vertexBuffer, 0,sizeof(ABVERTEX1Lx) );
			g_pd3dDevice->SetIndices(m_indicieBuffer);
			g_pd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST,
				                               0,
                                               0,//UINT MinIndex,
                                               m_vertexCount,//UINT NumVertices,
                                               0,//UINT StartIndex,
                                               (m_indecieCount/3)//UINT PrimitiveCount
                                               );
			polysIRendered = m_indecieCount;
			
		break;
	}
return polysIRendered;
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

	if(m_vertexArray0t){
       delete [] m_vertexArray0t;
	   m_vertexArray0t= 0;
	}
	if(m_vertexArray1t){
       delete [] m_vertexArray1t;
	   m_vertexArray1t= 0;
	}
	if(m_vertexArray2t){
       delete [] m_vertexArray2t;
	   m_vertexArray2t= 0;
	}
	if(m_vertexArray3t){
       delete [] m_vertexArray3t;
	   m_vertexArray3t= 0;
	}
	if(m_vertexArray4t){
       delete [] m_vertexArray4t;
	   m_vertexArray4t= 0;
	}
	if(m_vertexArray5t){
       delete [] m_vertexArray5t;
	   m_vertexArray5t= 0;
	}
	if(m_vertexArray6t){
       delete [] m_vertexArray6t;
	   m_vertexArray6t= 0;
	}
	
    if(m_vertexArray1x){
       delete [] m_vertexArray1x;
	   m_vertexArray1x= 0;
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

		case 19:

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  << m_vertexArray1t[loop].x
		        << m_vertexArray1t[loop].y
		        << m_vertexArray1t[loop].z
		        << m_vertexArray1t[loop].nx
		        << m_vertexArray1t[loop].ny
		        << m_vertexArray1t[loop].nz;
	      
	        ar << m_vertexArray1t[loop].tx0.tu
		       << m_vertexArray1t[loop].tx0.tv;

			ar << m_vertexArray1t[loop].tX
		       << m_vertexArray1t[loop].tY
			   << m_vertexArray1t[loop].tZ;
			}
		  
	    break;
		case 20:

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  << m_vertexArray2t[loop].x
		        << m_vertexArray2t[loop].y
		        << m_vertexArray2t[loop].z
		        << m_vertexArray2t[loop].nx
		        << m_vertexArray2t[loop].ny
		        << m_vertexArray2t[loop].nz;
	      
	        ar << m_vertexArray2t[loop].tx0.tu
		       << m_vertexArray2t[loop].tx0.tv;

			ar << m_vertexArray2t[loop].tx1.tu
		       << m_vertexArray2t[loop].tx1.tv;

			ar << m_vertexArray2t[loop].tX
		       << m_vertexArray2t[loop].tY
			   << m_vertexArray2t[loop].tZ;
			}
		  
	    break;
        case 21:

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  << m_vertexArray3t[loop].x
		        << m_vertexArray3t[loop].y
		        << m_vertexArray3t[loop].z
		        << m_vertexArray3t[loop].nx
		        << m_vertexArray3t[loop].ny
		        << m_vertexArray3t[loop].nz;
	      
	        ar << m_vertexArray3t[loop].tx0.tu
		       << m_vertexArray3t[loop].tx0.tv;

			ar << m_vertexArray3t[loop].tx1.tu
		       << m_vertexArray3t[loop].tx1.tv;

			ar << m_vertexArray3t[loop].tx2.tu
		       << m_vertexArray3t[loop].tx2.tv;

			ar << m_vertexArray3t[loop].tX
		       << m_vertexArray3t[loop].tY
			   << m_vertexArray3t[loop].tZ;
			}
		  
	    break;
		case 22:

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  << m_vertexArray4t[loop].x
		        << m_vertexArray4t[loop].y
		        << m_vertexArray4t[loop].z
		        << m_vertexArray4t[loop].nx
		        << m_vertexArray4t[loop].ny
		        << m_vertexArray4t[loop].nz;
	      
	        ar << m_vertexArray4t[loop].tx0.tu
		       << m_vertexArray4t[loop].tx0.tv;

			ar << m_vertexArray4t[loop].tx1.tu
		       << m_vertexArray4t[loop].tx1.tv;

			ar << m_vertexArray4t[loop].tx2.tu
		       << m_vertexArray4t[loop].tx2.tv;

			ar << m_vertexArray4t[loop].tx3.tu
		       << m_vertexArray4t[loop].tx3.tv;

			ar << m_vertexArray4t[loop].tX
		       << m_vertexArray4t[loop].tY
			   << m_vertexArray4t[loop].tZ;
			}
		  
	    break;

		case 23:

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  << m_vertexArray5t[loop].x
		        << m_vertexArray5t[loop].y
		        << m_vertexArray5t[loop].z
		        << m_vertexArray5t[loop].nx
		        << m_vertexArray5t[loop].ny
		        << m_vertexArray5t[loop].nz;
	      
	        ar << m_vertexArray5t[loop].tx0.tu
		       << m_vertexArray5t[loop].tx0.tv;

			ar << m_vertexArray5t[loop].tx1.tu
		       << m_vertexArray5t[loop].tx1.tv;

			ar << m_vertexArray5t[loop].tx2.tu
		       << m_vertexArray5t[loop].tx2.tv;

			ar << m_vertexArray5t[loop].tx3.tu
		       << m_vertexArray5t[loop].tx3.tv;

			ar << m_vertexArray5t[loop].tx4.tu
		       << m_vertexArray5t[loop].tx4.tv;

			ar << m_vertexArray5t[loop].tX
		       << m_vertexArray5t[loop].tY
			   << m_vertexArray5t[loop].tZ;
			}
		  
	    break;


		case 50:

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  << m_vertexArray3t[loop].x
		        << m_vertexArray3t[loop].y
		        << m_vertexArray3t[loop].z
		        << m_vertexArray3t[loop].nx
		        << m_vertexArray3t[loop].ny
		        << m_vertexArray3t[loop].nz;
	      
	        ar << m_vertexArray3t[loop].tx0.tu
		       << m_vertexArray3t[loop].tx0.tv;

			ar << m_vertexArray3t[loop].tx1.tu
		       << m_vertexArray3t[loop].tx1.tv;

			ar << m_vertexArray3t[loop].tx2.tu
		       << m_vertexArray3t[loop].tx2.tv;

			ar << m_vertexArray3t[loop].tX
		       << m_vertexArray3t[loop].tY
			   << m_vertexArray3t[loop].tZ;
			}
		  
	    break;
		
		case 70:

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  << m_vertexArray1x[loop].x
		        << m_vertexArray1x[loop].y
		        << m_vertexArray1x[loop].z;

			ar << m_vertexArray1x[loop].diffuse;
	      
	        ar << m_vertexArray1x[loop].tx0.tu
		       << m_vertexArray1x[loop].tx0.tv;

			ar << m_vertexArray1x[loop].tx1.tu
		       << m_vertexArray1x[loop].tx1.tv;

			}
		  
	    break;


	    
	   }//end save specific


      



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

	   m_vertexArray0t            = NULL;
       m_vertexArray1t            = NULL;    
       m_vertexArray2t            = NULL;
       m_vertexArray3t            = NULL;
       m_vertexArray4t            = NULL;

	   m_tangentExists            = 0;


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
        case 19:
         
			if(m_vertexCount > 0)
			m_vertexArray1t = new ABVERTEX1Lb[m_vertexCount];

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  >> m_vertexArray1t[loop].x
		        >> m_vertexArray1t[loop].y
		        >> m_vertexArray1t[loop].z
		        >> m_vertexArray1t[loop].nx
		        >> m_vertexArray1t[loop].ny
		        >> m_vertexArray1t[loop].nz;
	      
	        ar >> m_vertexArray1t[loop].tx0.tu
		       >> m_vertexArray1t[loop].tx0.tv;

			ar >> m_vertexArray1t[loop].tX
		       >> m_vertexArray1t[loop].tY
			   >> m_vertexArray1t[loop].tZ;
			}
		  
	    break;
		case 20:
         
			if(m_vertexCount > 0)
			m_vertexArray2t = new ABVERTEX2Lb[m_vertexCount];

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  >> m_vertexArray2t[loop].x
		        >> m_vertexArray2t[loop].y
		        >> m_vertexArray2t[loop].z
		        >> m_vertexArray2t[loop].nx
		        >> m_vertexArray2t[loop].ny
		        >> m_vertexArray2t[loop].nz;
	      
	        ar >> m_vertexArray2t[loop].tx0.tu
		       >> m_vertexArray2t[loop].tx0.tv;

			ar >> m_vertexArray2t[loop].tx1.tu
		       >> m_vertexArray2t[loop].tx1.tv;

			ar >> m_vertexArray2t[loop].tX
		       >> m_vertexArray2t[loop].tY
			   >> m_vertexArray2t[loop].tZ;
			}
		  
	    break;
		case 21:
         
			if(m_vertexCount > 0)
			m_vertexArray3t = new ABVERTEX3Lb[m_vertexCount];

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  >> m_vertexArray3t[loop].x
		        >> m_vertexArray3t[loop].y
		        >> m_vertexArray3t[loop].z
		        >> m_vertexArray3t[loop].nx
		        >> m_vertexArray3t[loop].ny
		        >> m_vertexArray3t[loop].nz;
	      
	        ar >> m_vertexArray3t[loop].tx0.tu
		       >> m_vertexArray3t[loop].tx0.tv;

			ar >> m_vertexArray3t[loop].tx1.tu
		       >> m_vertexArray3t[loop].tx1.tv;

			ar >> m_vertexArray3t[loop].tx2.tu
		       >> m_vertexArray3t[loop].tx2.tv;

			ar >> m_vertexArray3t[loop].tX
		       >> m_vertexArray3t[loop].tY
			   >> m_vertexArray3t[loop].tZ;
			}
		  
	    break;
        case 22:
         
			if(m_vertexCount > 0)
			m_vertexArray4t = new ABVERTEX4Lb[m_vertexCount];

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  >> m_vertexArray4t[loop].x
		        >> m_vertexArray4t[loop].y
		        >> m_vertexArray4t[loop].z
		        >> m_vertexArray4t[loop].nx
		        >> m_vertexArray4t[loop].ny
		        >> m_vertexArray4t[loop].nz;
	      
	        ar >> m_vertexArray4t[loop].tx0.tu
		       >> m_vertexArray4t[loop].tx0.tv;

			ar >> m_vertexArray4t[loop].tx1.tu
		       >> m_vertexArray4t[loop].tx1.tv;

			ar >> m_vertexArray4t[loop].tx2.tu
		       >> m_vertexArray4t[loop].tx2.tv;

			ar >> m_vertexArray4t[loop].tx3.tu
		       >> m_vertexArray4t[loop].tx3.tv;

			ar >> m_vertexArray4t[loop].tX
		       >> m_vertexArray4t[loop].tY
			   >> m_vertexArray4t[loop].tZ;
			}
		  
	    break;
		case 23:
         
			if(m_vertexCount > 0)
			m_vertexArray5t = new ABVERTEX5Lb[m_vertexCount];

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  >> m_vertexArray5t[loop].x
		        >> m_vertexArray5t[loop].y
		        >> m_vertexArray5t[loop].z
		        >> m_vertexArray5t[loop].nx
		        >> m_vertexArray5t[loop].ny
		        >> m_vertexArray5t[loop].nz;
	      
	        ar >> m_vertexArray5t[loop].tx0.tu
		       >> m_vertexArray5t[loop].tx0.tv;

			ar >> m_vertexArray5t[loop].tx1.tu
		       >> m_vertexArray5t[loop].tx1.tv;

			ar >> m_vertexArray5t[loop].tx2.tu
		       >> m_vertexArray5t[loop].tx2.tv;

			ar >> m_vertexArray5t[loop].tx3.tu
		       >> m_vertexArray5t[loop].tx3.tv;

			ar >> m_vertexArray5t[loop].tx4.tu
		       >> m_vertexArray5t[loop].tx4.tv;

			ar >> m_vertexArray5t[loop].tX
		       >> m_vertexArray5t[loop].tY
			   >> m_vertexArray5t[loop].tZ;
			}
		  
	    break;

		case 50:
         
			if(m_vertexCount > 0)
			m_vertexArray3t = new ABVERTEX3Lb[m_vertexCount];

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  >> m_vertexArray3t[loop].x
		        >> m_vertexArray3t[loop].y
		        >> m_vertexArray3t[loop].z
		        >> m_vertexArray3t[loop].nx
		        >> m_vertexArray3t[loop].ny
		        >> m_vertexArray3t[loop].nz;
	      
	        ar >> m_vertexArray3t[loop].tx0.tu
		       >> m_vertexArray3t[loop].tx0.tv;

			ar >> m_vertexArray3t[loop].tx1.tu
		       >> m_vertexArray3t[loop].tx1.tv;

			ar >> m_vertexArray3t[loop].tx2.tu
		       >> m_vertexArray3t[loop].tx2.tv;

			ar >> m_vertexArray3t[loop].tX
		       >> m_vertexArray3t[loop].tY
			   >> m_vertexArray3t[loop].tZ;
			}
		  
	    break;

		
		case 70:
         
			if(m_vertexCount > 0)
			m_vertexArray1x = new ABVERTEX1Lx[m_vertexCount];

			for(loop=0;loop<m_vertexCount;loop++){//store vertex array
            ar  >> m_vertexArray1x[loop].x
		        >> m_vertexArray1x[loop].y
		        >> m_vertexArray1x[loop].z;
				
			ar >> m_vertexArray1x[loop].diffuse;
	      
	        ar >> m_vertexArray1x[loop].tx0.tu
		       >> m_vertexArray1x[loop].tx0.tv;

			ar >> m_vertexArray1x[loop].tx1.tu
		       >> m_vertexArray1x[loop].tx1.tv;

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
		  
		     //Assign the spheremap's texture coordinates
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
		  
		     //Assign the spheremap's texture coordinates
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
		  
		     //Assign the spheremap's texture coordinates
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
		  
		     //Assign the spheremap's texture coordinates
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
		  
		     //Assign the spheremap's texture coordinates
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
		  
		     //Assign the spheremap's texture coordinates
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
		  
		     //Assign the spheremap's texture coordinates
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
		  
		     //Assign the spheremap's texture coordinates
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
		  
		     //Assign the spheremap's texture coordinates
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
		  
		     //Assign the spheremap's texture coordinates
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
		  
		     //Assign the spheremap's texture coordinates
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
		  
		     //Assign the spheremap's texture coordinates
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
		  
		     //Assign the spheremap's texture coordinates
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
		  
		     //Assign the spheremap's texture coordinates
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
		  
		     //Assign the spheremap's texture coordinates
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
		  
		     //Assign the spheremap's texture coordinates
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
		  
		     //Assign the spheremap's texture coordinates
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
		  
		     //Assign the spheremap's texture coordinates
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
		  
		     //Assign the spheremap's texture coordinates
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
		  
		     //Assign the spheremap's texture coordinates
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
		  
		     //Assign the spheremap's texture coordinates
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

  if(m_vertexCount <= 0)
     return;

  if(!m_vertexArray1)
     return;
   
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

  if(m_vertexCount <= 0)
     return;

  if(!m_vertexArray1)
     return;
   
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

  if(m_vertexCount <= 0)
     return;

  if(!m_vertexArray2)
     return;

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

  if(m_vertexCount <= 0)
     return;

  if(!m_vertexArray2)
     return;

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

  if(m_vertexCount <= 0)
     return;

  if(!m_vertexArray3)
     return;

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
		  case 2:
		   for(loop = 0;loop < m_vertexCount;loop++){
		      pVertices[loop].tx2.tu += (u * adjustfactor);
              pVertices[loop].tx2.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx2,m_vertexArray3[0].tx2,resetU,resetV);
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx2.tu =  m_vertexArray3[loop].tx2.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx2.tv =  m_vertexArray3[loop].tx2.tv;
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

  if(m_vertexCount <= 0)
     return;

  if(!m_vertexArray4)
     return;

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
		  case 2:
		   for(loop = 0;loop < m_vertexCount;loop++){
		      pVertices[loop].tx2.tu += (u * adjustfactor);
              pVertices[loop].tx2.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx2,m_vertexArray4[0].tx2,resetU,resetV);
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx2.tu =  m_vertexArray4[loop].tx2.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx2.tv =  m_vertexArray4[loop].tx2.tv;
		  break;
          case 3:
		   for(loop = 0;loop < m_vertexCount;loop++){
		      pVertices[loop].tx3.tu += (u * adjustfactor);
              pVertices[loop].tx3.tv += (v * adjustfactor); 
		   }
		   MoveTextureReset(u,v,adjustfactor,pVertices[0].tx3,m_vertexArray4[0].tx3,resetU,resetV);
		   if(resetU == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx3.tu =  m_vertexArray4[loop].tx3.tu;
		   if(resetV == TRUE)
           for(loop = 0;loop < m_vertexCount;loop++)
             pVertices[loop].tx3.tv =  m_vertexArray4[loop].tx3.tv;
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
D3DVECTOR CAdvancedVertexObj::CalculateTangentVector(D3DVERTEX & vector2,
													 D3DVERTEX & vector3)
{
   D3DVERTEX vector1;
   D3DVECTOR tangent;

   vector1.x = vector3.x   - vector2.x;
   vector1.y = vector3.y   - vector2.y;
   vector1.z = vector3.z   - vector2.z;
   vector1.tu = vector3.tu - vector2.tu;

   vector2.x = vector1.x   - vector2.x;
   vector2.y = vector1.y   - vector2.y;
   vector2.z = vector1.z   - vector2.z;
   vector2.tu = vector1.tu - vector2.tu;

   float deltaU1 = vector3.tu - vector2.tu;
   float deltaU2 = vector1.tu - vector2.tu;

   tangent.x = deltaU2*vector1.x - deltaU1*vector2.x;
   tangent.y = deltaU2*vector1.y - deltaU1*vector2.y;
   tangent.z = deltaU2*vector1.z - deltaU1*vector2.z;

   
   float dist = sqrt(tangent.x*tangent.x + tangent.y*tangent.y + tangent.z*tangent.z);
   
   tangent.x = tangent.x / dist;
   tangent.y = tangent.y / dist;
   tangent.z = tangent.z / dist;


   return tangent;

   /*Vec1 = Vertex3 – Vertex2
   Vec2 = Vertex1 – Vertex2
   DeltaU1 = Vertex3.u - Vertex2.u
   DeltaU2 = Vertex1.u - Vertex2.u
   DirectionV = |DeltaU2*Vec1-DeltaU1*Vec2|
   DirectionU = |DirectionV crossproduct Vertex.N|
   DirectionW = |DirectionU crossproduct DirectionV|*/
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CAdvancedVertexObj::GenerateTangents(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	MatrixARB matLib;
	D3DVERTEX vector1,vector2;
    D3DVECTOR tangent;
    float dist = 0;
    float deltaU1 = 0.0f;
    float deltaU2 = 0.0f;

	switch(m_uvCount)
	{
	  case 1:
	  //1UV
	  if(m_vertexArray1)
	  { 
       if(m_vertexArray1t)
	   {
	      delete [] m_vertexArray1t;
	      m_vertexArray1t = 0;
	   }

	   m_uvCount = 19;//translate to two UV sets and tangent

	   m_vertexArray1t = new ABVERTEX1Lb[m_vertexCount];
	   ZeroMemory(m_vertexArray1t,sizeof(ABVERTEX1Lb)*m_vertexCount);

	   int v=0;
	   for(v=0;v<m_vertexCount;v++)//clone data
		   CopyMemory(&m_vertexArray1t[v],&m_vertexArray1[v],sizeof(ABVERTEX1L));    

       //fill tangent data
	   
       for(v=0;v<m_indecieCount;v=v+3)
	   {
       
           vector1.x  = m_vertexArray1t[m_indecieArray[v+2]].x       - m_vertexArray1t[m_indecieArray[v+1]].x;
           vector1.y  = m_vertexArray1t[m_indecieArray[v+2]].y       - m_vertexArray1t[m_indecieArray[v+1]].y;
           vector1.z  = m_vertexArray1t[m_indecieArray[v+2]].z       - m_vertexArray1t[m_indecieArray[v+1]].z;
           vector1.tu = m_vertexArray1t[m_indecieArray[v+2]].tx0.tu  - m_vertexArray1t[m_indecieArray[v+1]].tx0.tu;

           vector2.x  = vector1.x   - m_vertexArray1t[m_indecieArray[v+1]].x;
           vector2.y  = vector1.y   - m_vertexArray1t[m_indecieArray[v+1]].y;
           vector2.z  = vector1.z   - m_vertexArray1t[m_indecieArray[v+1]].z;
           vector2.tu = vector1.tu  - m_vertexArray1t[m_indecieArray[v+1]].tx0.tu;

           deltaU1 = m_vertexArray1t[m_indecieArray[v+2]].tx0.tu - vector2.tu;
           deltaU2 = vector1.tu - vector2.tu;

           tangent.x = deltaU2*vector1.x - deltaU1*vector2.x;
           tangent.y = deltaU2*vector1.y - deltaU1*vector2.y;
           tangent.z = deltaU2*vector1.z - deltaU1*vector2.z;

           dist = sqrt(tangent.x*tangent.x + tangent.y*tangent.y + tangent.z*tangent.z);
   
           tangent.x = tangent.x / dist;
           tangent.y = tangent.y / dist;
           tangent.z = tangent.z / dist;


		   m_vertexArray1t[m_indecieArray[v]].tX += tangent.x;//vert1
		   m_vertexArray1t[m_indecieArray[v]].tY += tangent.y;//vert1
		   m_vertexArray1t[m_indecieArray[v]].tZ += tangent.z;//vert1
           
		   m_vertexArray1t[m_indecieArray[v+1]].tX += tangent.x;//vert1
		   m_vertexArray1t[m_indecieArray[v+1]].tY += tangent.y;//vert1
		   m_vertexArray1t[m_indecieArray[v+1]].tZ += tangent.z;//vert1

           m_vertexArray1t[m_indecieArray[v+2]].tX += tangent.x;//vert1
		   m_vertexArray1t[m_indecieArray[v+2]].tY += tangent.y;//vert1
		   m_vertexArray1t[m_indecieArray[v+2]].tZ += tangent.z;//vert1

	   }
	   //normalize all tangents
	   
	   for(int v=0;v<m_vertexCount;v++)
	   {

		   dist = sqrt(m_vertexArray1t[v].tX*m_vertexArray1t[v].tX + m_vertexArray1t[v].tY*m_vertexArray1t[v].tY + m_vertexArray1t[v].tZ*m_vertexArray1t[v].tZ);
   
           m_vertexArray1t[v].tX = m_vertexArray1t[v].tX / dist;
           m_vertexArray1t[v].tY = m_vertexArray1t[v].tY / dist;
           m_vertexArray1t[v].tZ = m_vertexArray1t[v].tZ / dist;
	   }

	   if(m_vertexArray1)
	   {
	      delete [] m_vertexArray1;
	      m_vertexArray1 = 0;
	   }
	  }
      break;
	  case 2:
	  //2UV
	  if(m_vertexArray2)
	  { 
       if(m_vertexArray2t)
	   {
	      delete [] m_vertexArray2t;
	      m_vertexArray2t = 0;
	   }

	   m_uvCount = 20;//translate to two UV sets and tangent

	   m_vertexArray2t = new ABVERTEX2Lb[m_vertexCount];
	   ZeroMemory(m_vertexArray2t,sizeof(ABVERTEX2Lb)*m_vertexCount);
       int v=0;
	   for(v=0;v<m_vertexCount;v++)//clone data
		   CopyMemory(&m_vertexArray2t[v],&m_vertexArray2[v],sizeof(ABVERTEX2L));    

       //fill tangent data
	   D3DVECTOR tempOne,tempTwo,vNormalTemp;
	   
       for(v=0;v<m_indecieCount;v=v+3)
	   {
           tempOne.x  = m_vertexArray2t[m_indecieArray[v+2]].x - m_vertexArray2t[m_indecieArray[v+1]].x;
           tempOne.y  = m_vertexArray2t[m_indecieArray[v+2]].y - m_vertexArray2t[m_indecieArray[v+1]].y;
           tempOne.z  = m_vertexArray2t[m_indecieArray[v+2]].z - m_vertexArray2t[m_indecieArray[v+1]].z;
          
           tempTwo.x  = m_vertexArray2t[m_indecieArray[v]].x - m_vertexArray2t[m_indecieArray[v+1]].x;
           tempTwo.y  = m_vertexArray2t[m_indecieArray[v]].y - m_vertexArray2t[m_indecieArray[v+1]].y;
           tempTwo.z  = m_vertexArray2t[m_indecieArray[v]].z - m_vertexArray2t[m_indecieArray[v+1]].z;

		   deltaU1 = m_vertexArray2t[m_indecieArray[v+2]].tx0.tu - m_vertexArray2t[m_indecieArray[v+1]].tx0.tu;
           deltaU2 = m_vertexArray2t[m_indecieArray[v]].tx0.tu - m_vertexArray2t[m_indecieArray[v+1]].tx0.tu;



		   D3DVECTOR dirV = deltaU2*tempOne-deltaU1*tempTwo;
		   dirV = matLib.Normalize(dirV);//|DeltaU2*Vec1-DeltaU1*Vec2|

		   vNormalTemp = matLib.Normalize(matLib.CrossProduct(tempOne,tempTwo));//DirectionU = |DirectionV x Vertex.N| //Tangent
        
		   D3DVECTOR dirU;

		   //vNormalTemp.x = m_vertexArray2t[m_indecieArray[v]].nx;
		   //vNormalTemp.y = m_vertexArray2t[m_indecieArray[v]].ny;
		   //vNormalTemp.z = m_vertexArray2t[m_indecieArray[v]].nz;


		   dirU = matLib.Normalize(matLib.CrossProduct(dirV,vNormalTemp));//DirectionU = |DirectionV x Vertex.N| //Tangent

		   tangent = dirU;

		   m_vertexArray2t[m_indecieArray[v]].tX += tangent.x;//vert1
		   m_vertexArray2t[m_indecieArray[v]].tY += tangent.y;//vert1
		   m_vertexArray2t[m_indecieArray[v]].tZ += tangent.z;//vert1
           
		   m_vertexArray2t[m_indecieArray[v+1]].tX += tangent.x;//vert1
		   m_vertexArray2t[m_indecieArray[v+1]].tY += tangent.y;//vert1
		   m_vertexArray2t[m_indecieArray[v+1]].tZ += tangent.z;//vert1

           m_vertexArray2t[m_indecieArray[v+2]].tX += tangent.x;//vert1
		   m_vertexArray2t[m_indecieArray[v+2]].tY += tangent.y;//vert1
		   m_vertexArray2t[m_indecieArray[v+2]].tZ += tangent.z;//vert1

           
	   }
	   //normalize all tangents
	   
	   for(int v=0;v<m_vertexCount;v++)
	   {

		   dist = sqrt(m_vertexArray2t[v].tX*m_vertexArray2t[v].tX + m_vertexArray2t[v].tY*m_vertexArray2t[v].tY + m_vertexArray2t[v].tZ*m_vertexArray2t[v].tZ);
   
           m_vertexArray2t[v].tX = m_vertexArray2t[v].tX / dist;
           m_vertexArray2t[v].tY = m_vertexArray2t[v].tY / dist;
           m_vertexArray2t[v].tZ = m_vertexArray2t[v].tZ / dist;
	   }

	   if(m_vertexArray2)
	   {
	      delete [] m_vertexArray2;
	      m_vertexArray2 = 0;
	   }
	  }
    break;
	case 3:
	  //3UV
	  if(m_vertexArray3)
	  { 
       if(m_vertexArray3t)
	   {
	      delete [] m_vertexArray3t;
	      m_vertexArray3t = 0;
	   }

	   m_uvCount = 21;//translate to two UV sets and tangent

	   m_vertexArray3t = new ABVERTEX3Lb[m_vertexCount];
	   ZeroMemory(m_vertexArray3t,sizeof(ABVERTEX3Lb)*m_vertexCount);
       int v=0;
	   for(v=0;v<m_vertexCount;v++)//clone data
		   CopyMemory(&m_vertexArray3t[v],&m_vertexArray3[v],sizeof(ABVERTEX3Lb));    

       //fill tangent data
	   
       for(v=0;v<m_indecieCount;v=v+3)
	   {
       
           vector1.x  = m_vertexArray3t[m_indecieArray[v+2]].x       - m_vertexArray3t[m_indecieArray[v+1]].x;
           vector1.y  = m_vertexArray3t[m_indecieArray[v+2]].y       - m_vertexArray3t[m_indecieArray[v+1]].y;
           vector1.z  = m_vertexArray3t[m_indecieArray[v+2]].z       - m_vertexArray3t[m_indecieArray[v+1]].z;
           vector1.tu = m_vertexArray3t[m_indecieArray[v+2]].tx0.tu  - m_vertexArray3t[m_indecieArray[v+1]].tx0.tu;

           vector2.x  = vector1.x   - m_vertexArray3t[m_indecieArray[v+1]].x;
           vector2.y  = vector1.y   - m_vertexArray3t[m_indecieArray[v+1]].y;
           vector2.z  = vector1.z   - m_vertexArray3t[m_indecieArray[v+1]].z;
           vector2.tu = vector1.tu  - m_vertexArray3t[m_indecieArray[v+1]].tx0.tu;

           deltaU1 = m_vertexArray3t[m_indecieArray[v+2]].tx0.tu - vector2.tu;
           deltaU2 = vector1.tu - vector2.tu;

           tangent.x = deltaU2*vector1.x - deltaU1*vector2.x;
           tangent.y = deltaU2*vector1.y - deltaU1*vector2.y;
           tangent.z = deltaU2*vector1.z - deltaU1*vector2.z;

           dist = sqrt(tangent.x*tangent.x + tangent.y*tangent.y + tangent.z*tangent.z);
   
           tangent.x = tangent.x / dist;
           tangent.y = tangent.y / dist;
           tangent.z = tangent.z / dist;


		   m_vertexArray3t[m_indecieArray[v]].tX += tangent.x;//vert1
		   m_vertexArray3t[m_indecieArray[v]].tY += tangent.y;//vert1
		   m_vertexArray3t[m_indecieArray[v]].tZ += tangent.z;//vert1
           
		   m_vertexArray3t[m_indecieArray[v+1]].tX += tangent.x;//vert1
		   m_vertexArray3t[m_indecieArray[v+1]].tY += tangent.y;//vert1
		   m_vertexArray3t[m_indecieArray[v+1]].tZ += tangent.z;//vert1

           m_vertexArray3t[m_indecieArray[v+2]].tX += tangent.x;//vert1
		   m_vertexArray3t[m_indecieArray[v+2]].tY += tangent.y;//vert1
		   m_vertexArray3t[m_indecieArray[v+2]].tZ += tangent.z;//vert1

	   }
	   //normalize all tangents
	   
	   for(int v=0;v<m_vertexCount;v++)
	   {

		   dist = sqrt(m_vertexArray3t[v].tX*m_vertexArray3t[v].tX + m_vertexArray3t[v].tY*m_vertexArray3t[v].tY + m_vertexArray3t[v].tZ*m_vertexArray3t[v].tZ);
   
           m_vertexArray3t[v].tX = m_vertexArray3t[v].tX / dist;
           m_vertexArray3t[v].tY = m_vertexArray3t[v].tY / dist;
           m_vertexArray3t[v].tZ = m_vertexArray3t[v].tZ / dist;
	   }

	   if(m_vertexArray3)
	   {
	      delete [] m_vertexArray3;
	      m_vertexArray3 = 0;
	   }
	  }
    break;
	case 4:
	  //4UV
	  if(m_vertexArray4)
	  { 
       if(m_vertexArray4t)
	   {
	      delete [] m_vertexArray4t;
	      m_vertexArray4t = 0;
	   }

	   m_uvCount = 22;//translate to two UV sets and tangent

	   m_vertexArray4t = new ABVERTEX4Lb[m_vertexCount];
	   ZeroMemory(m_vertexArray4t,sizeof(ABVERTEX4Lb)*m_vertexCount);
       int v=0;
	   for(v=0;v<m_vertexCount;v++)//clone data
		   CopyMemory(&m_vertexArray4t[v],&m_vertexArray4[v],sizeof(ABVERTEX4Lb));    

       //fill tangent data
	   
       for(v=0;v<m_indecieCount;v=v+3)
	   {
       
           vector1.x  = m_vertexArray4t[m_indecieArray[v+2]].x       - m_vertexArray4t[m_indecieArray[v+1]].x;
           vector1.y  = m_vertexArray4t[m_indecieArray[v+2]].y       - m_vertexArray4t[m_indecieArray[v+1]].y;
           vector1.z  = m_vertexArray4t[m_indecieArray[v+2]].z       - m_vertexArray4t[m_indecieArray[v+1]].z;
           vector1.tu = m_vertexArray4t[m_indecieArray[v+2]].tx0.tu  - m_vertexArray4t[m_indecieArray[v+1]].tx0.tu;

           vector2.x  = vector1.x   - m_vertexArray4t[m_indecieArray[v+1]].x;
           vector2.y  = vector1.y   - m_vertexArray4t[m_indecieArray[v+1]].y;
           vector2.z  = vector1.z   - m_vertexArray4t[m_indecieArray[v+1]].z;
           vector2.tu = vector1.tu  - m_vertexArray4t[m_indecieArray[v+1]].tx0.tu;

           deltaU1 = m_vertexArray4t[m_indecieArray[v+2]].tx0.tu - vector2.tu;
           deltaU2 = vector1.tu - vector2.tu;

           tangent.x = deltaU2*vector1.x - deltaU1*vector2.x;
           tangent.y = deltaU2*vector1.y - deltaU1*vector2.y;
           tangent.z = deltaU2*vector1.z - deltaU1*vector2.z;

           dist = sqrt(tangent.x*tangent.x + tangent.y*tangent.y + tangent.z*tangent.z);
   
           tangent.x = tangent.x / dist;
           tangent.y = tangent.y / dist;
           tangent.z = tangent.z / dist;


		   m_vertexArray4t[m_indecieArray[v]].tX += tangent.x;//vert1
		   m_vertexArray4t[m_indecieArray[v]].tY += tangent.y;//vert1
		   m_vertexArray4t[m_indecieArray[v]].tZ += tangent.z;//vert1
           
		   m_vertexArray4t[m_indecieArray[v+1]].tX += tangent.x;//vert1
		   m_vertexArray4t[m_indecieArray[v+1]].tY += tangent.y;//vert1
		   m_vertexArray4t[m_indecieArray[v+1]].tZ += tangent.z;//vert1

           m_vertexArray4t[m_indecieArray[v+2]].tX += tangent.x;//vert1
		   m_vertexArray4t[m_indecieArray[v+2]].tY += tangent.y;//vert1
		   m_vertexArray4t[m_indecieArray[v+2]].tZ += tangent.z;//vert1

	   }
	   //normalize all tangents
	   
	   for(int v=0;v<m_vertexCount;v++)
	   {

		   dist = sqrt(m_vertexArray4t[v].tX*m_vertexArray4t[v].tX + m_vertexArray4t[v].tY*m_vertexArray4t[v].tY + m_vertexArray4t[v].tZ*m_vertexArray4t[v].tZ);
   
           m_vertexArray4t[v].tX = m_vertexArray4t[v].tX / dist;
           m_vertexArray4t[v].tY = m_vertexArray4t[v].tY / dist;
           m_vertexArray4t[v].tZ = m_vertexArray4t[v].tZ / dist;
	   }

	   if(m_vertexArray4)
	   {
	      delete [] m_vertexArray4;
	      m_vertexArray4 = 0;
	   }
	  }
    break;
	}


	AVGTangents(60);


	if(g_pd3dDevice)
	   InitBuffer(g_pd3dDevice,this->m_uvCount);
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
BOOL CAdvancedVertexObj::GenerateTangentsPerfect(LPDIRECT3DDEVICE9 g_pd3dDevice,
												 BOOL refresh,
												 int basis)
{
	//AfxMessageBox("hi3");
	MatrixARB matrixLib;
    D3DVECTOR tangent;
    float dist = 0;
    float deltaU1 = 0.0f;
    float deltaU2 = 0.0f;
    ABVERTEX * abVertex = NULL;
    int vCount = 0;

    GetABVERTEXStyle(&abVertex,&vCount);

	   if(m_uvCount == 1)
	      m_uvCount = 19;
	   else if(m_uvCount == 2)
	      m_uvCount = 20;
	   else if(m_uvCount == 3)
	      m_uvCount = 21;
	   else if(m_uvCount == 4)
	      m_uvCount = 22;
	   else if(m_uvCount == 5)
	      m_uvCount = 23;
	   else if(m_uvCount == 6)
	      m_uvCount = 24;
	   else if(m_uvCount ==7)
	      m_uvCount = 25;
	   else if(!refresh)
	   {//already formatted
          if(abVertex)
	      delete [] abVertex;
 	      abVertex = 0;
          return TRUE;
	   }//end already formatted

       //fill tangent data
	   D3DVECTOR tempOne,tempTwo,vNormalTemp;

       //zero tangents
       for(int v=0;v<m_indecieCount;v=v+3)
	   {
           abVertex[m_indecieArray[v]].tx = 0.0f;//vert1
		   abVertex[m_indecieArray[v]].ty = 0.0f;//vert1
		   abVertex[m_indecieArray[v]].tz = 0.0f;//vert1
           
		   abVertex[m_indecieArray[v+1]].tx = 0.0f;//vert1
		   abVertex[m_indecieArray[v+1]].ty = 0.0f;//vert1
		   abVertex[m_indecieArray[v+1]].tz = 0.0f;//vert1

           abVertex[m_indecieArray[v+2]].tx = 0.0f;//vert1
		   abVertex[m_indecieArray[v+2]].ty = 0.0f;//vert1
		   abVertex[m_indecieArray[v+2]].tz = 0.0f;//vert1
	   }
	   //end zero tangents
       for(int v=0;v<m_indecieCount;v=v+3)
	   {

           
		   tempOne.x  = abVertex[m_indecieArray[v+2]].x - abVertex[m_indecieArray[v+1]].x;
           tempOne.y  = abVertex[m_indecieArray[v+2]].y - abVertex[m_indecieArray[v+1]].y;
           tempOne.z  = abVertex[m_indecieArray[v+2]].z - abVertex[m_indecieArray[v+1]].z;
		   tempOne = matrixLib.Normalize(tempOne);
          
           tempTwo.x  = abVertex[m_indecieArray[v]].x - abVertex[m_indecieArray[v+1]].x;
           tempTwo.y  = abVertex[m_indecieArray[v]].y - abVertex[m_indecieArray[v+1]].y;
           tempTwo.z  = abVertex[m_indecieArray[v]].z - abVertex[m_indecieArray[v+1]].z;
		   tempTwo = matrixLib.Normalize(tempTwo);

		   //deltaU1 = abVertex[m_indecieArray[v+2]].tx0.tu - abVertex[m_indecieArray[v+1]].tx0.tu;
           //deltaU2 = abVertex[m_indecieArray[v]].tx0.tu - abVertex[m_indecieArray[v+1]].tx0.tu;

		   
		   deltaU1 = abVertex[m_indecieArray[v+2]].tx0.tv - abVertex[m_indecieArray[v+1]].tx0.tv;
           deltaU2 = abVertex[m_indecieArray[v]].tx0.tv - abVertex[m_indecieArray[v+1]].tx0.tv;
		   if(basis == 1)
		   {
              deltaU1 = -1.0f;
		      deltaU2 = -1.0f;
		   }
		   

		   D3DVECTOR dirV = deltaU2*tempOne-deltaU1*tempTwo;
		   dirV = matrixLib.Normalize(dirV);//|DeltaU2*Vec1-DeltaU1*Vec2|

		   vNormalTemp = matrixLib.Normalize(matrixLib.CrossProduct(tempOne,tempTwo));//DirectionU = |DirectionV x Vertex.N| //Tangent
        
		   D3DVECTOR dirU;

		   dirU = matrixLib.Normalize(matrixLib.CrossProduct(dirV,vNormalTemp));//DirectionU = |DirectionV x Vertex.N| //Tangent

		   tangent = dirV;
		   /*tangent.x = dirV.x;
		   tangent.y = dirV.y;
		   tangent.z = dirV.z;*/

		   abVertex[m_indecieArray[v]].tx += tangent.x;//vert1
		   abVertex[m_indecieArray[v]].ty += tangent.y;//vert1
		   abVertex[m_indecieArray[v]].tz += tangent.z;//vert1
           
		   abVertex[m_indecieArray[v+1]].tx += tangent.x;//vert1
		   abVertex[m_indecieArray[v+1]].ty += tangent.y;//vert1
		   abVertex[m_indecieArray[v+1]].tz += tangent.z;//vert1

           abVertex[m_indecieArray[v+2]].tx += tangent.x;//vert1
		   abVertex[m_indecieArray[v+2]].ty += tangent.y;//vert1
		   abVertex[m_indecieArray[v+2]].tz += tangent.z;//vert1

           
	   }
	   //normalize all tangents
	   
	   for(int v=0;v<m_vertexCount;v++)
	   {
		   dist = sqrt(abVertex[v].tx*abVertex[v].tx + abVertex[v].ty*abVertex[v].ty + abVertex[v].tz*abVertex[v].tz);
   
           abVertex[v].tx = abVertex[v].tx / dist;
           abVertex[v].ty = abVertex[v].ty / dist;
           abVertex[v].tz = abVertex[v].tz / dist;
	   }

	   SetABVERTEXStyle(NULL,
				        abVertex,
					    vCount);
       if(abVertex)
	      delete [] abVertex;
 	      abVertex = 0;

	   AVGTangents(60);
	   return TRUE;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CAdvancedVertexObj::AVGTangents(float smoothBasisAngle)
{
MatrixARB matrixLib;
ABVERTEX * abVertex = NULL;
int vCount = 0;

GetABVERTEXStyle(&abVertex,&vCount);
    
	for(int x=0; x<vCount; x++ )
	  for(int i=0; i<vCount; i++ )
      {
	    if(i == x)
		   continue;
        //WORD vI1 = m_abVertexArray.m_indecieArray[i];
		//WORD vI2 = m_abVertexArray.m_indecieArray[x];
       

        if(abVertex[i].x == abVertex[x].x &&
           abVertex[i].y == abVertex[x].y &&
		   abVertex[i].z == abVertex[x].z)
		{//same pos

           D3DVECTOR tempOne,tempTwo;
           tempOne.x = abVertex[i].tx;
		   tempOne.y = abVertex[i].ty;
		   tempOne.z = abVertex[i].tz;
		   tempTwo.x = abVertex[x].tx;
		   tempTwo.y = abVertex[x].ty;
		   tempTwo.z = abVertex[x].tz;
	       float angleDiff = matrixLib.GetAngleBetweenTwoVectors(tempOne,tempTwo);
           if(angleDiff <= smoothBasisAngle)
		   {//combine
		    float oldIx = abVertex[i].tx;
		    abVertex[i].tx = abVertex[i].tx + abVertex[x].tx;
			abVertex[x].tx = oldIx + abVertex[x].tx;

			float oldIy = abVertex[i].ty;
			abVertex[i].ty = abVertex[i].ty + abVertex[x].ty;
			abVertex[x].ty = oldIy + abVertex[x].ty;

			float oldIz = abVertex[i].tz;
			abVertex[i].tz = abVertex[i].tz + abVertex[x].tz;
			abVertex[x].tz = oldIz + abVertex[x].tz;
		   }//end combine
		}//end same pos
      }

    //Assign the newly computed normals back to the vertices
    D3DVECTOR normV;
    for(int i=0; i<m_vertexCount; i++ )
    {
        //Provide some relief to bogus normals
        if( abVertex[i].tx == 0.0f && abVertex[i].ty == 0.0f && abVertex[i].tz == 0.0f) 
		{
            abVertex[i].tx = 0.0f;//= D3DVECTOR( 0.0f, 0.0f, 1.0f );
            abVertex[i].ty = 0.0f;
			abVertex[i].tz = 1.0f;
		}

		normV.x = abVertex[i].tx;
        normV.y = abVertex[i].ty;
		normV.z = abVertex[i].tz;

        normV = matrixLib.Normalize(normV);

        abVertex[i].tx = normV.x;
        abVertex[i].ty = normV.y;
        abVertex[i].tz = normV.z;
    }
    SetABVERTEXStyle(NULL,
				     abVertex,
					 vCount);
    if(abVertex)
	   delete [] abVertex;
 	   abVertex = 0;

}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CAdvancedVertexObj::AVGNormals(float smoothBasisAngle)
{
MatrixARB matrixLib;
ABVERTEX * abVertex = NULL;
int vCount = 0;

GetABVERTEXStyle(&abVertex,&vCount);
    
	for(int x=0; x<vCount; x++ )
	  for(int i=0; i<vCount; i++ )
      {
	    if(i == x)
		   continue;
        //WORD vI1 = m_abVertexArray.m_indecieArray[i];
		//WORD vI2 = m_abVertexArray.m_indecieArray[x];
       

        if(abVertex[i].x == abVertex[x].x &&
           abVertex[i].y == abVertex[x].y &&
		   abVertex[i].z == abVertex[x].z)
		{//same pos

           D3DVECTOR tempOne,tempTwo;
           tempOne.x = abVertex[i].nx;
		   tempOne.y = abVertex[i].ny;
		   tempOne.z = abVertex[i].nz;
		   tempTwo.x = abVertex[x].nx;
		   tempTwo.y = abVertex[x].ny;
		   tempTwo.z = abVertex[x].nz;
	       float angleDiff = matrixLib.GetAngleBetweenTwoVectors(tempOne,tempTwo);
           if(angleDiff <= smoothBasisAngle)
		   {//combine
		    float oldIx = abVertex[i].nx;
		    abVertex[i].nx = abVertex[i].nx + abVertex[x].nx;
			abVertex[x].nx = oldIx + abVertex[x].nx;

			float oldIy = abVertex[i].ny;
			abVertex[i].ny = abVertex[i].ny + abVertex[x].ny;
			abVertex[x].ny = oldIy + abVertex[x].ny;

			float oldIz = abVertex[i].nz;
			abVertex[i].nz = abVertex[i].nz + abVertex[x].nz;
			abVertex[x].nz = oldIz + abVertex[x].nz;
		   }//end combine
		}//end same pos
      }

    //Assign the newly computed normals back to the vertices
    D3DVECTOR normV;
    for(int i=0; i<m_vertexCount; i++ )
    {
        //Provide some relief to bogus normals
        if( abVertex[i].nx == 0.0f && abVertex[i].ny == 0.0f && abVertex[i].nz == 0.0f) 
		{
            abVertex[i].nx = 0.0f;//= D3DVECTOR( 0.0f, 0.0f, 1.0f );
            abVertex[i].ny = 0.0f;
			abVertex[i].nz = 1.0f;
		}

		normV.x = abVertex[i].nx;
        normV.y = abVertex[i].ny;
		normV.z = abVertex[i].nz;

        normV = matrixLib.Normalize(normV);

        abVertex[i].nx = normV.x;
        abVertex[i].ny = normV.y;
        abVertex[i].nz = normV.z;
    }
    SetABVERTEXStyle(NULL,
									 abVertex,
									 vCount);
    if(abVertex)
	   delete [] abVertex;
 	   abVertex = 0;

}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CAdvancedVertexObj::UnifyNormals(float angleTolerance)
{
	            MatrixARB matrixLib;
	            D3DVECTOR tempOne,tempTwo;
	            float tolerance = .002f;
                ABVERTEX * vArray = NULL;
                int vCount = 0;
		        GetABVERTEXStyle(&vArray,&vCount);

                ABVERTEX * chkvArray = NULL;
                int chkvCount = 0;
				GetABVERTEXStyle(&chkvArray,&chkvCount);
				int pvLoop=0;
				for(pvLoop=0;pvLoop<m_vertexCount;pvLoop++)
				{//primary mesh loop
                   for(int scndLoop=0;scndLoop<m_vertexCount;scndLoop++)
				   {//secondary mesh loop
				      if(pvLoop == scndLoop)
					     continue;

					  if(fabs(vArray[pvLoop].x - chkvArray[scndLoop].x) > tolerance)
					     continue;

					  if(fabs(vArray[pvLoop].y - chkvArray[scndLoop].y) > tolerance)
					     continue;

					  if(fabs(vArray[pvLoop].z - chkvArray[scndLoop].z) > tolerance)
					     continue;
					  
					  
                      /*float dist = List_matrixFunctions.MagnitudeSquared(vArray[pvLoop].x,
						                                               vArray[pvLoop].y,
																	   vArray[pvLoop].z,
																	   chkvArray[scndLoop].x,
																	   chkvArray[scndLoop].y,
																	   chkvArray[scndLoop].z);*/
                     //if(dist < tolerance)
					 //{
                       tempOne.x = chkvArray[scndLoop].nx;
		              tempOne.y = chkvArray[scndLoop].ny;
		              tempOne.z = chkvArray[scndLoop].nz;
		              tempTwo.x = vArray[pvLoop].nx;
		              tempTwo.y = vArray[pvLoop].ny;
		              tempTwo.z = vArray[pvLoop].nz;
		              float angleDiff = matrixLib.GetAngleBetweenTwoVectors(tempOne,tempTwo);

					   if(angleDiff <= angleTolerance)
					   {//colapse	
						chkvArray[scndLoop].nx += vArray[pvLoop].nx;
					    chkvArray[scndLoop].ny += vArray[pvLoop].ny;
					    chkvArray[scndLoop].nz += vArray[pvLoop].nz;
					   }//end colapse
					 //}
				   }//end secondary mesh loop
				}//end primary mesh loop

				for(pvLoop=0;pvLoop<m_vertexCount;pvLoop++)
				{//primary mesh loop
				   D3DVECTOR nTemp;
						nTemp.x = vArray[pvLoop].nx;
                        nTemp.y = vArray[pvLoop].ny;
						nTemp.z = vArray[pvLoop].nz;
						nTemp = matrixLib.Normalize(nTemp);
                   for(int scndLoop=0;scndLoop<m_vertexCount;scndLoop++)
				   {//secondary mesh loop
				      //if(pvLoop == scndLoop)
					     //continue;
				      
					  
                      float dist = matrixLib.MagnitudeSquared(vArray[pvLoop].x,
						                                               vArray[pvLoop].y,
																	   vArray[pvLoop].z,
																	   chkvArray[scndLoop].x,
																	   chkvArray[scndLoop].y,
																	   chkvArray[scndLoop].z);
                     if(dist < tolerance)
					 {
                       tempOne.x = chkvArray[scndLoop].nx;
		               tempOne.y = chkvArray[scndLoop].ny;
		               tempOne.z = chkvArray[scndLoop].nz;
		               tempTwo.x = vArray[pvLoop].nx;
		               tempTwo.y = vArray[pvLoop].ny;
		               tempTwo.z = vArray[pvLoop].nz;
		               float angleDiff = matrixLib.GetAngleBetweenTwoVectors(tempOne,tempTwo);

					   if(angleDiff <= angleTolerance)
					   {//colapse
					    
						
						
						chkvArray[scndLoop].nx = nTemp.x;
					    chkvArray[scndLoop].ny = nTemp.y;
					    chkvArray[scndLoop].nz = nTemp.z;
					   }//end colapse
					 }
				   }//end secondary mesh loop
				}//end primary mesh loop

				if(chkvArray)
				{
                   SetABVERTEXStyle(NULL,chkvArray,chkvCount);
                   delete [] chkvArray;
				   chkvArray = 0;
				}
				if(vArray)
				{
                   delete [] vArray;
				   vArray = 0;
				}
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
void CAdvancedVertexObj::NormalizeOnly()
{
MatrixARB matrixLib;
ABVERTEX * abVertex = NULL;
int vCount = 0;

GetABVERTEXStyle(&abVertex,&vCount);

    D3DVECTOR tempNormal;
    //Assign the newly computed normals back to the vertices
    for(int i=0; i<m_vertexCount; i++ )
    {
        tempNormal.x = abVertex[i].nx;
		tempNormal.y = abVertex[i].ny;
		tempNormal.z = abVertex[i].nz;

        tempNormal = matrixLib.Normalize( tempNormal );
        abVertex[i].nx = tempNormal.x;
        abVertex[i].ny = tempNormal.y;
        abVertex[i].nz = tempNormal.z;
    }

    SetABVERTEXStyle(NULL,
									 abVertex,
									 vCount);
    if(abVertex)
	   delete [] abVertex;
 	   abVertex = 0;

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

