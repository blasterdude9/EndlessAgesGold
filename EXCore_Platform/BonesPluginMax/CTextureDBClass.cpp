//#include "stdafx.h"
#include "afxwin.h"
#include "afxext.h"
#include "resource.h"
#include <d3d9.h>
//#include <afxtempl.h>
#include "direct.h"

#include "CTextureDBClass.h"


CTextureDBObj::CTextureDBObj(
                            char L_textureName[256]
		                    )
{
m_textureObject = NULL;
List_tempIndex=-1;
List_isInUseInScene = FALSE;
List_BltOpOn = FALSE;
m_bump = 0;
m_bumpType = 0;
m_peramameter1 = 0.0f;
for(int loop=0;loop<256;loop++)
List_textureName[loop] = L_textureName[loop];
}

void CTextureDBObj::SafeDelete()
{
	if(m_textureObject){
       m_textureObject->SafeDelete();
       delete m_textureObject;
	   m_textureObject=0;
	}
}

void CTextureDBObjList::RestoreAllTextures( LPDIRECT3DDEVICE9 pd3dDevice, int & maxTextureSize,D3DCAPS9 & pCaps)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CTextureDBObj * txObj = (CTextureDBObj *)GetNext(posLoc);	
		txObj->m_textureObject->Restore(pd3dDevice,
										txObj->List_BltOpOn,
										txObj->m_bump,
										txObj->m_bumpType,
										maxTextureSize,
										pCaps);
	}
	
}

void CTextureDBObjList::CacheRawData()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CTextureDBObj * txObj = (CTextureDBObj *)GetNext(posLoc);
        txObj->m_textureObject->CacheRawData();
	}
}

void CTextureDBObjList::InvalidateAllTextures( )
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CTextureDBObj * txObj = (CTextureDBObj *)GetNext(posLoc);
        txObj->m_textureObject->Invalidate();
	}
}

CString CTextureDBObjList::GetNameByIndex(int index)
{
	CString temp = "NA";
    if(index < 0)
	   return temp;


	POSITION posLoc = FindIndex(index);
	if(!posLoc)
		return temp;
    CTextureDBObj * txObj = (CTextureDBObj *)GetAt(posLoc);
	temp = txObj->List_textureName;
    return temp;
}


CTextureDBObjList::CTextureDBObjList()
{
	directAccess = NULL;
}

void CTextureDBObjList::CompileDirectAccess()
{
    int textureCount = GetCount();
	if(textureCount == 0)
	   return;

	if(directAccess)
	{
       delete [] directAccess;
       directAccess = 0;
	}
    directAccess = new DirectAccessStruct[textureCount];
	int tracer = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        directAccess[tracer].object = (CTextureDBObj *)GetNext(posLoc);
		tracer++;  
	}
	directAccessObjectCount = tracer;
}


void CTextureDBObjList::SafeDelete()
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CTextureDBObj * txObj = (CTextureDBObj *)GetNext(posLoc);
        txObj->SafeDelete();
		delete txObj;
		txObj=0;
	 }
     RemoveAll();

	 if(directAccess)
	 {
       delete [] directAccess;
       directAccess = 0;
	   directAccessObjectCount = 0;
	 }
}

void CTextureDBObj::Serialize( CArchive& ar )
{
int reserved = 0;

CObject::Serialize( ar );
	if(ar.IsStoring() ){
       ar.SetObjectSchema(2);
       ASSERT(ar.GetObjectSchema() == 2);

	   for(int loop=0;loop<256;loop++)
	   ar << List_textureName[loop];

	   ar << List_BltOpOn;

	   ar << m_textureObject;

	   ar << m_bump;
	   ar << m_bumpType;
	   ar << m_peramameter1;
	   ar << reserved;

	}
	else
	{
       int version = ar.GetObjectSchema();

	   for(int loop=0;loop<256;loop++)
       ar >> List_textureName[loop];

	   ar >> List_BltOpOn;
	   
       ar >> m_textureObject;//new var

	   m_bump=0;
       if(version > 1)
	   {
	     ar >> m_bump;
	     ar >> m_bumpType;
	     ar >> m_peramameter1;
	     ar >> reserved;
	   }
	   
	   List_isInUseInScene = FALSE;
	   List_tempIndex = -1;
	}	
}



IMPLEMENT_SERIAL(CTextureDBObj, CObject, VERSIONABLE_SCHEMA|2)
IMPLEMENT_SERIAL(CTextureDBObjList, CObList, 0)

