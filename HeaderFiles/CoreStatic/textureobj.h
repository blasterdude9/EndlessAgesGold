#ifndef TEXTUREOBJ_H
#define TEXTUREOBJ_H
//-----------------------------------------------------------------------------
//Defines, constants, and global variables
//-----------------------------------------------------------------------------
//Structure to hold information for a mipmap texture
//-------------------------------------------------
//----------------Legacy section-------------------
struct RgbDataStruct {
       WORD  * m_rgbaData16;
	   DWORD * m_rgbaData32;
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
    CString                m_fileName;

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
//-------------------------------------------------
//------------End Legacy section-------------------
//-------------------------------------------------
//Main class
class CTextureEX : public CObject
{
    DECLARE_SERIAL( CTextureEX )

public:
	CMapObjList           *m_mapFileList;//Legacy format use only
    int                    m_fileType;//legacy format use only
	DWORD                  m_widthBasis;
    DWORD                  m_hieghtBasis;
	DWORD                  m_depth;
	DWORD                  m_dwMipMapCount;
	CString                m_fileName;
	D3DFORMAT              m_format;
	byte                  *m_rawMapData;
	DWORD                  m_rawMapDataLength;
	byte                   m_restoreType;
	IDirect3DTexture9     *m_pddsSurface;       //Surface of the mipmap
    int                    m_tempSurface;
	DWORD                  m_compressedOvrd;//0=no override 1=uncompressed >1=format
	DWORD                  m_usage;//can be used for setting the texture as a render target



    CTextureEX();
	void Clone(CTextureEX **copy);
	void CacheRawData();
	CString GetFormatName();
	BOOL LoadIt(CString fileName,int mipLevels,LPDIRECT3DDEVICE9 pd3dDevice,int tempSurface);
	BOOL LoadItFromURL(CString url,int mipLevels,LPDIRECT3DDEVICE9 pd3dDevice,int tempSurface,bool createData,CString & advUrlBasic,CString & advUrlVending);
	BOOL SafeDelete();
	HRESULT Restore( IDirect3DDevice9 * pd3dDevice,
		             BOOL blitOn,
					 int bump,
					 int bumpType,
					 int & maxTextureSize,
					 D3DCAPS9 & pCaps,
					 int & compressIt,
					 BOOL & processImage,
					 CString & advUrl,
					 CString & advUrlVending);
	HRESULT Restore( IDirect3DDevice9 * pd3dDevice,
							 BOOL blitOn,
							 int bump,
							 int bumpType,
							 int & maxTextureSize,
							 D3DCAPS9 & pCaps,
							 int & compressIt,
							 CString & advUrl,
							 CString & advUrlVending);
    VOID    Invalidate();
	HRESULT CopyRGBADataToSurface(CMapObj * mapData,
								  IDirect3DTexture9 * surface,
					              IDirect3DTexture9 * persistMap,
							      int m_baseWidth,
								  int m_baseHieght,
								  int maxTextureWidth,
								  int maxTextureHeight,
								  int createWidth,
								  int createHeight);
	BOOL GetBaseDataFromSurface(DWORD * length, 
						        byte ** surfaceBytes,
						        DWORD * width,
						        DWORD * hieght,
						        DWORD * mipCount,
						        D3DFORMAT * d3dFormat,
						        IDirect3DSurface9 * currentMipSurface);
	BOOL GetBaseDataFromSurface(DWORD * length, 
						        byte ** surfaceBytes,
						        DWORD * width,
						        DWORD * hieght,
						        DWORD * mipCount,
						        D3DFORMAT * d3dFormat,
						        IDirect3DTexture9 * texture);
	BOOL GenerateSurfaceFromBaseData(DWORD & length, 
									 byte * surfaceBytes,
									 IDirect3DSurface9 ** surface);
	BOOL GenerateSurfaceFromBaseData(IDirect3DDevice9 * pd3dDevice,
	                                 DWORD & length, 
									 byte * surfaceBytes,
									 DWORD & width,
									 DWORD & hieght,
									 DWORD & mipCount,
									 D3DFORMAT & format,
									 IDirect3DTexture9 ** surface,
									 D3DCAPS9 & pCaps);
	HRESULT ProcessToEnvMap(LPDIRECT3DDEVICE9 pd3dDevice);
	HRESULT ProcessToEnvWaveMap(LPDIRECT3DDEVICE9 pd3dDevice, int bump);
	HRESULT GenerateNormalMap(LPDIRECT3DDEVICE9 pd3dDevice,int bump);
	void    GenerateMipMaps(IDirect3DTexture9 * texture, DWORD & mipCount);
	HRESULT RestoreLegacy( IDirect3DDevice9 * pd3dDevice,
							       BOOL & blitOn,
							       int & bump,
							       int & bumpType,
							       int & maxTextureSize,
							       D3DCAPS9 & pCaps,
							       int & compressIt);
	D3DFORMAT TransformToCompressedFormat(D3DFORMAT & format);
	short EncodeFormat(D3DFORMAT & format);
	D3DFORMAT DecodeFormat(short & id);
	DWORD CheckMaxMipCount(DWORD width, DWORD height, DWORD & currentMipCount);
	BOOL CopyDataLiveDataToBaseData(int optimizedType);
	BOOL GetBaseDataFromSurface(DWORD * length, 
										byte ** surfaceBytes,
										DWORD * width,
										DWORD * hieght,
										DWORD * mipCount,
										D3DFORMAT * d3dFormat,
										IDirect3DSurface9 * currentMipSurface,
										BOOL & optimized);
	BOOL GetBaseDataFromSurface(DWORD * length, 
						                byte ** surfaceBytes,
						                DWORD * width,
						                DWORD * hieght,
						                DWORD * mipCount,
						                D3DFORMAT * d3dFormat,
						                IDirect3DTexture9 * texture,
										BOOL optimized);
	BOOL IsTextureFormatOk( D3DFORMAT TextureFormat, 
							D3DFORMAT AdapterFormat,
							IDirect3D9 * d3d9Ptr,
							D3DFORMAT checkFormat);
	

	void Serialize( CArchive& ar );
     void Callback(LPDIRECT3DDEVICE9 pd3dDevice,int stage,int & type,float & perameter);
	 DWORD F2DW( FLOAT f ) { return *((DWORD*)&f); }
    IDirect3DTexture9 * GetSurface() { return m_pddsSurface; };
};




#endif