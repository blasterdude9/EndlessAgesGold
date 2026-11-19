

#ifndef TEXTUREOBJ_H
#define TEXTUREOBJ_H
//-----------------------------------------------------------------------------
// Defines, constants, and global variables
//-----------------------------------------------------------------------------
// Structure to hold information for a mipmap texture
//#include "D3DUtil.h"

struct RgbDataStruct {
       WORD * m_rgbaData;
	   WORD   m_tgaWidth;
	   WORD   m_tgaHieght;
	   WORD   m_tgaBBP;
};

class CMapObj : public CObject
{
	CMapObj(){}
      DECLARE_SERIAL( CMapObj )
   
	public:
	CMapObj(CString filename);
	int                    m_mapFileType;          
	RgbDataStruct          m_rgbData;
    CString m_fileName;

public:
	void Serialize( CArchive& ar );

};

class CMapObjList : public CObList
{
public:
	CMapObjList(){};
	  int AddTextureSet(CString fileName,int mipLevels);
	  void RemoveFileExt(CString *currentName);   
      DECLARE_SERIAL( CMapObjList )
   
};


//------------------
//------------------
//Main class
class  CTextureEX : public CObject
{
    
    
     
    DECLARE_SERIAL( CTextureEX )
    
    
   
    
public:
	CMapObjList           *m_mapFileList;
    DWORD                  m_dwMipMapCount;     // Levels of mipmap
    int                    m_fileType;
	DWORD                  m_widthBasis;
    DWORD                  m_hieghtBasis;
	IDirect3DTexture9     *m_pddsSurface;       // Surface of the mipmap


	




    CTextureEX();
	void CacheRawData();
	BOOL LoadIt(CString fileName,int mipLevels,LPDIRECT3DDEVICE9 pd3dDevice);
	BOOL SafeDelete();
	HRESULT Restore( IDirect3DDevice9 * pd3dDevice,
		             BOOL blitOn,
					 int bump,
					 int bumpType,
					 int & maxTextureSize,
					 D3DCAPS9 & pCaps);
    VOID    Invalidate();
	HRESULT CopyRGBADataToSurface(WORD * tgaData,
								  IDirect3DTexture9 * surface,
					              IDirect3DTexture9 * persistMap,
							      int m_baseWidth,
								  int m_baseHieght,
								  int maxTextureWidth,
								  int maxTextureHeight,
								  int createWidth,
								  int createHeight);

	void Serialize( CArchive& ar );
     void Callback(LPDIRECT3DDEVICE9 pd3dDevice,int stage,int & type,float & perameter);
	 DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }
    IDirect3DTexture9 * GetSurface() { return m_pddsSurface; };
};




#endif