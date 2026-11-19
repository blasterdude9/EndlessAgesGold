#ifndef BACKGROUNDCLASS_H
#define BACKGROUNDCLASS_H

//class defines background class

class AFX_EXT_CLASS CBackGroundObj : public CObject
{
	CBackGroundObj(){}
	
     
    DECLARE_SERIAL( CBackGroundObj )
    
    
public:
	
	CBackGroundObj(IDirect3DTexture9 * L_mainSurface,//main surface
		           CString L_bitmapName,//bitmap file name
		           int L_mapX,//represents the x pixel count of source image
		           int L_mapY,//represents the y pixel count of source image			
		           RECT L_usageRect,//represents the rect of what is used of the source image
	               int L_posX,//position X of offset that defines location
				   int L_posY,//position Y of offset that defines location
				   BOOL L_moving,//says if the map is going to be movable
				   int L_animX,
				   int L_animY
				   );
    
	int List_animX;
	int List_animY;
	BOOL List_moving;//is it a moving texture
	IDirect3DTexture9 * List_mainSurface;
	CString List_bitmapName;//bitmap file name
    int List_mapX;//represents the x pixel count of source image
	int List_mapY;//represents the y pixel count of source image			
	RECT List_usageRect;//represents the rect of what is used of the source image
	int List_posX;//position X of offset that defines location
	int List_posY;//position Y of offset that defines location


	void Serialize( CArchive& ar );
};

class  AFX_EXT_CLASS CBackGroundObjList : public CObList
{
public:
   CBackGroundObjList(){};
    
    void SafeDelete();
    DECLARE_SERIAL( CBackGroundObjList )
    
    
};

#endif