#include "d3d9.h"
#include "matrixarb.h"
#pragma once

class CViewToSurface : public CObject
{
	public:
		CViewToSurface(){m_curSurface=NULL;m_tempTargetWidthPx=256;m_tempTargetHeightPx=256;};
		~CViewToSurface(){if(m_curSurface)m_curSurface->Release();m_curSurface=0;};

		D3DMATRIX			m_oMatrix;
		int                 m_textureIndex;
		char				m_type;//0=portal mirror 1=plane mirror
		float               m_left;
		float				m_right;
		float				m_top;
		float				m_bottom;
		DWORD				m_tempTargetWidthPx;
		DWORD				m_tempTargetHeightPx;
		D3DVECTOR			m_oCenter;
		D3DVECTOR			m_mirrorDir;//can be a normal on a simple object like a plane
		D3DVECTOR           m_width3DMaxPoint;//use for foc gen
		D3DVECTOR           m_width3DMaxPointN;
		D3DVECTOR           m_height3DMaxPoint;
		D3DVECTOR           m_height3DMaxPointN;
		IDirect3DSurface9  *m_curSurface;

	//FUNCTIONS
	IDirect3DSurface9 * GetRenderTargetSurfaceX(void * textureDBMem, void * gStatusMem);
	
};

class CViewToSurfaceDB : public CObList
{
	public:
		CViewToSurfaceDB(){};
		~CViewToSurfaceDB(){Flush();};

	MatrixARB m_matLib;

	void Flush()
	{
         for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
		 {
             CViewToSurface * dPtr = (CViewToSurface *)GetNext(posLoc);
             delete dPtr;
			 dPtr = 0;
		 }
		 RemoveAll();
	}

	void Add(D3DMATRIX & oMat,
		     int & texIndex,
			 char type,
			 D3DVECTOR & oCenter, 
			 D3DVECTOR * optMirrorDir, 
			 float left, 
			 float right,
			 float top,
			 float bottom)
	{
		 CViewToSurface * nPtr = new CViewToSurface();
         nPtr->m_oMatrix				= oMat;
		 nPtr->m_textureIndex			= texIndex;
		 nPtr->m_oCenter				= oCenter;
		 nPtr->m_left					= left;
		 nPtr->m_right					= right;
		 nPtr->m_top					= top;
		 nPtr->m_bottom					= bottom;
		 nPtr->m_type					= type;
		 
            ZeroMemory(&nPtr->m_mirrorDir,sizeof(D3DVECTOR));
            ZeroMemory(&nPtr->m_width3DMaxPoint,sizeof(D3DVECTOR));
			ZeroMemory(&nPtr->m_width3DMaxPointN,sizeof(D3DVECTOR));
            ZeroMemory(&nPtr->m_height3DMaxPoint,sizeof(D3DVECTOR));
			ZeroMemory(&nPtr->m_height3DMaxPointN,sizeof(D3DVECTOR));
			nPtr->m_height3DMaxPointN.y = nPtr->m_bottom;
			nPtr->m_height3DMaxPointN = m_matLib.UpdatePolygonVertex(nPtr->m_oMatrix.m,nPtr->m_height3DMaxPointN);
			nPtr->m_height3DMaxPoint.y = nPtr->m_top;
			nPtr->m_height3DMaxPoint = m_matLib.UpdatePolygonVertex(nPtr->m_oMatrix.m,nPtr->m_height3DMaxPoint);
			
			nPtr->m_width3DMaxPoint.x = nPtr->m_right;
			nPtr->m_width3DMaxPoint = m_matLib.UpdatePolygonVertex(nPtr->m_oMatrix.m,nPtr->m_width3DMaxPoint);
			nPtr->m_width3DMaxPointN.x =  nPtr->m_left;
			nPtr->m_width3DMaxPointN = m_matLib.UpdatePolygonVertex(nPtr->m_oMatrix.m,nPtr->m_width3DMaxPointN);
			nPtr->m_mirrorDir.z = 1.0f;//foward :|
			if(optMirrorDir)
			   nPtr->m_mirrorDir = *optMirrorDir;
            
			nPtr->m_mirrorDir = m_matLib.UpdatePolygonVertexRotationOnly(nPtr->m_oMatrix.m,nPtr->m_mirrorDir);

		 AddTail(nPtr);
	}

	


};