#include "stdafx.h"

#include <d3d9.h>
#include <afxtempl.h>
#include "CBackgroundclass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------Main object list------------------//
CBackGroundObj::CBackGroundObj(IDirect3DTexture9 * L_mainSurface,//main surface
							   CString L_bitmapName,//bitmap file name
							   int L_mapX,//represents the x pixel count of source image
		                       int L_mapY,//represents the y pixel count of source image			
		                       RECT L_usageRect,//represents the rect of what is used of the source image
	                           int L_posX,//position X of offset that defines location
				               int L_posY,//position Y of offset that defines location
				               BOOL L_moving,//is the map movable
							   int L_animX,
				               int L_animY
							   )
{
List_animX = L_animX;
List_animY = L_animY;
List_moving = L_moving;
List_mainSurface = L_mainSurface;
List_bitmapName = L_bitmapName;
List_mapX = L_mapX;
List_mapY = L_mapY;
List_usageRect = L_usageRect;
List_posX = L_posX;
List_posY = L_posY;
}

//------------Main object list------------------//


void CBackGroundObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
		ar << List_posX 
		   << List_posY 
		   << List_mapX 
		   << List_mapY 
		   << List_bitmapName 
		   << List_usageRect 
		   << List_moving 
		   << List_animX 
		   << List_animY;
		
	}
	else
	{
        ar >> List_posX 
		   >> List_posY 
		   >> List_mapX 
		   >> List_mapY 
		   >> List_bitmapName 
		   >> List_usageRect 
		   >> List_moving 
		   >> List_animX 
		   >> List_animY;
		List_mainSurface=0;
	}
}

//------------Main object list------------------//
void CBackGroundObjList::SafeDelete()
{
     for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CBackGroundObj * skPtr = (CBackGroundObj *)GetNext(posLoc);
		 delete skPtr;
		 skPtr=0;
	 }
	 RemoveAll();
}

IMPLEMENT_SERIAL(CBackGroundObj, CObject, 0)
IMPLEMENT_SERIAL(CBackGroundObjList, CObList, 0)