#ifndef VIEWPORTCLASS_H
#define VIEWPORTCLASS_H

//#include "CMovementModel.h"
#include "CCameraClass.h"
#include "CFrameObj.h"
#include "matrixarb.h"

class  CViewportObj : public CObject
{
	 CViewportObj(){}
	  
       
      DECLARE_SERIAL( CViewportObj )
      
      
public:

	 CViewportObj(
		int L_X1,
	    int L_Y1,
		int L_X2,
		int L_Y2
     );
	    MatrixARB matLib;
	    D3DVIEWPORT9 List_imViewPort;
	    BOOL         List_inUse;
		CCameraObj * m_cameraObject;
		int List_X1;
	    int List_Y1;
		int List_X2;
		int List_Y2;
		int m_traceNumberef; //-1 = NONE
        void SafeDelete();
		void Serialize( CArchive& ar );

		//functions
		D3DMATRIX UtilizeViewPort(int curWidth,int curHeight, LPDIRECT3DDEVICE9 d3dDevice);

};

class  CViewportList : public CObList
{
public:
      CViewportList(){};
	  void SafeDelete();
      CViewportObj * GetByEntTrace(int trace);
	  BOOL DeleteByEntTrace(int trace);
	  CCameraObj * GetCameraObjectFromEntity(CMovementObj * ptrLoc);

       
      DECLARE_SERIAL( CViewportList )
      
      
};
#endif