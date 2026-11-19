#include "textureObj.h"
#ifndef CTEXTUREDBCLASS_H
#define CTEXTUREDBCLASS_H
//class  is basically used for popout
class CTextureDBObj : public CObject
{ 
	
	
     	
    DECLARE_SERIAL( CTextureDBObj )
    
    
    
public:
	CTextureDBObj(){};
	
	CTextureDBObj(char L_textureName[256]);
    
    char List_textureName[256]; 
	CTextureEX * m_textureObject;
	BOOL List_isInUseInScene;
	int  m_formatOvrd;
	int  m_bump;
	int  m_bumpType;
	float m_peramameter1;
    int List_tempIndex;
	char m_tempVar;
	char m_allocatedType;
	void Serialize( CArchive& ar );
    void SafeDelete();
	void Clone(CTextureDBObj **copy);
	BOOL UpdateTextureFromSearchDir(CString & mapsModelsDir,
									CString * textureSearchPathRef,
									int & searchPathCountRef,
									LPDIRECT3DDEVICE9 d3dDeviceRef,
									int maxTextureSizeSquareRef,
									D3DCAPS9 & pCapsRef,
									int & compressRef,
									CString & advUrl,
									CString & advUrlVending);
	

};

struct DirectAccessStruct
{
    CTextureDBObj * object;
};


class CTextureDBObjList : public CObList
{
public:
   CTextureDBObjList();

   ~CTextureDBObjList();
   

   int m_compressRef;

   void InitReferences(CString & mapsModelsPath,
					   LPDIRECT3DDEVICE9 dedDeviceRef,
				       int & maxTextureSizeSquareRef,
					   BOOL & editorModeOn,
					   CString * textureSearchPath,
					   int & textureSearchPathCount,
					   int & compressIt,
					   CString & advUrl,
					   CString & advUrlVending
					   );

   CString GL_advURL,GL_advURLVending;
   //storage reference
   CString GL_pathMapModelsRef;
   LPDIRECT3DDEVICE9 d3dDeviceRef;
   int m_maxTextureSizeSquareRef;
   D3DCAPS9 m_pCapsRef;
   BOOL m_editorModeOn;
   CString * m_textureSearchPathRef;
   int m_searchPathCountRef;
   //Pixel format info precalc
   int AddAssignTempTexture(LPDIRECT3DDEVICE9 pd3dDevice, CString &textureName,int tempChannel);
   void RestoreAllTextures( LPDIRECT3DDEVICE9 pd3dDevice, int & maxTextureSize,D3DCAPS9 & pCaps);
   void CacheRawData();
   void InvalidateAllTextures();
   void SafeDelete();
   void CompileDirectAccess();
   CString GetNameByIndex(int index);
   void RemoveAllTempTexturesByChannel(int tempSurfaceID);
   void RemoveAllByTempVal();
   void RemoveAllTempTextures();
   void FlushAllByAllocID(int alID);
   void DeleteAllByAllocID(int alID);
   void Clone(CTextureDBObjList ** copy);
   void ExportCompressedTXB(CString outputPath,
							LPDIRECT3DDEVICE9 pd3dDevice,
							int compressed,
							int maxTextureSize,
							int fileCompressType);
   void UpdateLiveDataToBaseData();

   BOOL TextureSearch(CString textureName, 
					  int & index,
					  int maxMipLevels,
					  CTextureDBObjList * m_dynamicTextureDataBase,
					  int tempChannel
					  );

   BOOL AddTextureToDatabase(CString * storedTextures,
						    int * assignmentArray,
						    CTextureDBObjList * m_dynamicTextureDataBase
						    );
   BOOL AddTextureToDatabaseTemp(CString * storedTextures,
								 int * assignmentArray,
								 int tempChannel);
   

   DirectAccessStruct * directAccess;
   int                  directAccessObjectCount;
   void DirectoryUpdateAllTextures();
   DECLARE_SERIAL( CTextureDBObjList )
};

#endif
