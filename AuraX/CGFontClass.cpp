#include "stdafx.h"
#define D3D_OVERLOADS
#include <afxtempl.h>
#include "CGFONTCLASS.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//-----------------------------------------------------------//
//-----class is for persisting characters over a network-----//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CGFontObj::CGFontObj()
{
    m_fontTableIndex = 0;
    m_fontTableName = "NONE";
	m_fontName = "n/a";
}

void CGFontObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	ar << m_fontTableIndex 
       << m_fontTableName
	   << m_fontName;
	}
	else
	{
    ar >> m_fontTableIndex 
       >> m_fontTableName
	   >> m_fontName;
	}
}

void CGFontObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
        CGFontObj * cgPtr = (CGFontObj*)GetNext(posLoc);
		delete cgPtr;
		cgPtr=0;
	}
	RemoveAll();
}


IMPLEMENT_SERIAL(CGFontObj, CObject, 0)
IMPLEMENT_SERIAL(CGFontObjList, CObList, 0)









