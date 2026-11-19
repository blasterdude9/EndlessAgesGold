#ifndef CTEXTURESWAPCLASS_H
#define CTEXTURESWAPCLASS_H
//Bitmap list
class   CBitmapObj : public CObject
{
	CBitmapObj(){}
	
     
    DECLARE_SERIAL( CBitmapObj )
    
    
public:
	
	CBitmapObj(char L_fileName[256]
		       //IDirect3DRMTexture3 *L_texture
			   );
 
	char List_fileName[256];
	//IDirect3DRMTexture3 *List_texture;

	void Serialize( CArchive& ar );

};

class   CBitmapObjList : public CObList
{
public:
   CBitmapObjList(){};
   
     
    DECLARE_SERIAL( CBitmapObjList )
    
    
};

//main class
class   CTextureSwapObj : public CObject
{
	CTextureSwapObj(){}
	
     
    DECLARE_SERIAL( CTextureSwapObj )
    
    
public:
	
	CTextureSwapObj(CBitmapObjList * L_bitmapList,
		            CString L_collectionName,
					int L_textureCount,
					BOOL L_alpha,
					BOOL L_decal,
					int  L_delay,
					int  L_delayVarial,
					int  L_alphaMapSize,
					float L_decalRed,
					float L_decalGreen,
					float L_decalBlue,
					int L_alphaChannel
				   );

	int List_alphaChannel;
	float List_decalRed;
	float List_decalGreen;
	float List_decalBlue;
	BOOL List_decal;
	int  List_delay;
	int  List_delayVarial;
	int  List_alphaMapSize;
    BOOL List_alpha;
    CBitmapObjList * List_bitmapList;
	CString List_collectionName;
	int List_textureCount;

	void Serialize( CArchive& ar );

};

class   CTextureSwapObjList : public CObList
{
public:
   CTextureSwapObjList(){};
    
     
    DECLARE_SERIAL( CTextureSwapObjList )
    
    
};

#endif