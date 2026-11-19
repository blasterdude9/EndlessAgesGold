#include "stdafx.h"
//
#include <d3d9.h>
#include <afxtempl.h>
#include "afxext.h"
#include "Viewportclass.h"
#include "matrixarb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CViewportObj::CViewportObj(
		int L_X1,
	    int L_Y1,
		int L_X2,
		int L_Y2)
{
	    m_traceNumberef = -1;
	    m_cameraObject = NULL;
	    List_inUse = FALSE;
		List_X1 = L_X1;
	    List_Y1 = L_Y1;
		List_X2 = L_X2;
		List_Y2 = L_Y2;
		//default init
		List_imViewPort.X = L_X1;
	    List_imViewPort.Y = L_Y1;
		List_imViewPort.Width = L_X2;
	    List_imViewPort.Height = L_Y2;
		List_imViewPort.MinZ = 0.0f;
		List_imViewPort.MaxZ = 1.0f;
}


void CViewportObj::SafeDelete()
{
	if(m_cameraObject){
	   m_cameraObject->SafeDelete();
	   delete m_cameraObject;
	   m_cameraObject=0;
	}
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------------sets a viewport--------------------------------------------------//
D3DMATRIX CViewportObj::UtilizeViewPort(int curWidth,
										int curHeight, 
										LPDIRECT3DDEVICE9 d3dDevice)
{
	        
            float X1conv = (float)List_X1;
		    float Y1conv = (float)List_Y1;
		    float X2conv = (float)List_X2;
		    float Y2conv = (float)List_Y2;

		    float width = (float)curWidth;
	        int X1perc = (int)((X1conv*width)*.01);
	        int X2perc = (int)((X2conv*width)*.01);
	        float hieght = (float)curHeight;
	        int Y1perc = (int)((Y1conv*hieght)*.01);
	        int Y2perc = (int)((Y2conv*hieght)*.01);
	        
            List_imViewPort.X = X1perc;
            List_imViewPort.Y = Y1perc;		
			List_imViewPort.Width = X2perc - X1perc;
		    List_imViewPort.Height = Y2perc - Y1perc;

			HRESULT err = d3dDevice->SetViewport(&List_imViewPort);
            float fAspect = ((FLOAT)List_imViewPort.Height) / List_imViewPort.Width;
            D3DMATRIX matProj;
			matLib.InitMat(matProj.m);

			if(m_cameraObject)
			{
			     /*matLib.SetProjectionMatrix( matProj,
				                             m_cameraObject->List_fovView, 
										     fAspect, 
										     1.0f,
										     m_cameraObject->List_clipPlane,
				                             m_cameraObject->List_hieghtBaseAspect);*/

			    //m_renderStateObject->SetViewProjMatrixOnly((D3DXMATRIX*)&matProj,d3dDevice);
			}
			List_inUse=TRUE;
			return matProj;
}


void CViewportObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar << List_X1
		  << List_Y1
		  << List_X2
		  << List_Y2;
	}
	else
	{
       ar >> List_X1
		  >> List_Y1
		  >> List_X2
		  >> List_Y2;

       //default init
		List_imViewPort.X = List_X1;
	    List_imViewPort.Y = List_Y1;
		List_imViewPort.Width = List_X2;
	    List_imViewPort.Height = List_Y2;
		List_imViewPort.MinZ = 0.0f;
		List_imViewPort.MaxZ = 1.0f;
		m_cameraObject = NULL;
		m_traceNumberef = -1;
	    List_inUse = FALSE;
	}
}

CViewportObj * CViewportList::GetByEntTrace(int trace)
{
  for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
  {//remove previous entres 
	 CViewportObj* mptLoc = ( CViewportObj* ) GetNext(posLoc);
     if(mptLoc->m_traceNumberef == trace){
        return mptLoc;
	 }  
  }//end remove previous entres 
  return NULL;
}

BOOL CViewportList::DeleteByEntTrace(int trace)
{
POSITION posLast=NULL;
  for(POSITION posLoc = GetHeadPosition();(posLast=posLoc) != NULL;)
  {//remove previous entres 
	 CViewportObj* mptLoc = ( CViewportObj* ) GetNext(posLoc);
     if(mptLoc->m_traceNumberef == trace){
        mptLoc->SafeDelete();
		delete mptLoc;
		mptLoc = 0;

        RemoveAt(posLast);

        return TRUE;
	 }  
  }//end remove previous entres 
  return FALSE;
}

//----------------------------------------------------------//
//----------------------------------------------------------//
//----------------------------------------------------------//
CCameraObj * CViewportList::GetCameraObjectFromEntity(CMovementObj * ptrLoc)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{//LOOP THROUGH VIEWPORTS 
	     CViewportObj* mptLoc = ( CViewportObj* ) GetNext(posLoc);
         if(mptLoc->m_traceNumberef == ptrLoc->List_traceNumber)
		 {
            return mptLoc->m_cameraObject;
		 }
	}
	return NULL;
}
//----------------------------------------------------------//
void CViewportList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
	    CViewportObj * mPtr = (CViewportObj *)GetNext(posLoc);
		mPtr->SafeDelete();
		delete mPtr;
		mPtr = 0;
	}
	RemoveAll();
}



IMPLEMENT_SERIAL(CViewportObj, CObject, 0)
IMPLEMENT_SERIAL(CViewportList, CObList, 0)
