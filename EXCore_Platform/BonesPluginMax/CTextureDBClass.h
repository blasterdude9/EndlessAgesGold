#include "textureObj.h"
#include "CexmeshClass.h"
#ifndef CTEXTUREDBCLASS_H
#define CTEXTUREDBCLASS_H
//class is basically used for popout
class  CTextureDBObj : public CObject
{ 
	
	
     	
    DECLARE_SERIAL( CTextureDBObj )
    
    
    
public:
	CTextureDBObj(){};
	
	CTextureDBObj(char L_textureName[256]);
    
    char List_textureName[256]; 
	CTextureEX * m_textureObject;
	BOOL List_isInUseInScene;
	BOOL List_BltOpOn;
	int  m_bump;
	int  m_bumpType;
	float m_peramameter1;
    int List_tempIndex;
	void Serialize( CArchive& ar );
    void SafeDelete();
	

};

struct DirectAccessStruct
{
    CTextureDBObj * object;
};


class  CTextureDBObjList : public CObList
{
public:
   CTextureDBObjList();


   //Pixel format info precalc

   void RestoreAllTextures( LPDIRECT3DDEVICE9 pd3dDevice, int & maxTextureSize,D3DCAPS9 & pCaps);
   void CacheRawData();
   void InvalidateAllTextures();
   void SafeDelete();
   void CompileDirectAccess();
   CString GetNameByIndex(int index);


   DirectAccessStruct * directAccess;
   int                  directAccessObjectCount;
   
   
    	
   DECLARE_SERIAL( CTextureDBObjList )
   
   
   
};

#endif
