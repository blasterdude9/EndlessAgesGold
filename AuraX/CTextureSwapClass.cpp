#include "stdafx.h"

#include <d3d9.h>
#include <afxtempl.h>

#include "CTextureSwapClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//bitmap collections
CBitmapObj::CBitmapObj(char L_fileName[256]
		               //IDirect3DRMTexture3 * L_texture
			           )
{
for(int loop=0;loop<256;loop++)
List_fileName[loop] = L_fileName[loop];


//List_texture = L_texture;
}

void CBitmapObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   for(int loop=0;loop<256;loop++)
	   ar  << List_fileName[loop];
	}
	else
	{
	   for(int loop=0;loop<256;loop++)
       ar  >> List_fileName[loop];
	   //List_texture=0;
	}
}

IMPLEMENT_SERIAL(CBitmapObj, CObject, 0)
IMPLEMENT_SERIAL(CBitmapObjList, CObList, 0)

//main class
CTextureSwapObj::CTextureSwapObj(CBitmapObjList * L_bitmapList,
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
				                 )
{
List_alphaChannel = L_alphaChannel;
List_decal = L_decal;
List_delay = L_delay;
List_delayVarial = L_delayVarial;
List_alphaMapSize = L_alphaMapSize;
List_decalRed = L_decalRed;
List_decalGreen = L_decalGreen;
List_decalBlue = L_decalBlue;
List_alpha = L_alpha;
List_bitmapList = L_bitmapList;
List_collectionName = L_collectionName;
List_textureCount = L_textureCount;
}

void CTextureSwapObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar  << List_bitmapList
           << List_collectionName
           << List_textureCount
		   << List_alpha
		   << List_decal
           << List_delay
           << List_delayVarial
           << List_alphaMapSize
           << List_decalRed
           << List_decalGreen
           << List_decalBlue
		   << List_alphaChannel;
	}
	else
	{
       ar  >> List_bitmapList
           >> List_collectionName
           >> List_textureCount
		   >> List_alpha
		   >> List_decal
           >> List_delay
           >> List_delayVarial
           >> List_alphaMapSize
           >> List_decalRed
           >> List_decalGreen
           >> List_decalBlue
		   >> List_alphaChannel;
	}
}

IMPLEMENT_SERIAL(CTextureSwapObj, CObject, 0)
IMPLEMENT_SERIAL(CTextureSwapObjList, CObList, 0)
