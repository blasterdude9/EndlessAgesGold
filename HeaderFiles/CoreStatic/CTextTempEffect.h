#include "CTextRenderClass.h"
#include "CEXMeshClass.h"
#include "CGFontClass.h"

#pragma once


class CTextEffectClass : public CObject
{
	public:
		CTextEffectClass()
		{
		   m_mesh = NULL;
		};
		~CTextEffectClass()
		{
		   if(m_mesh)
		   {
              m_mesh->SafeDelete();
			  delete m_mesh;
			  m_mesh = 0;
		   }
		};

    //variables
	DWORD			m_duration;
	DWORD			m_timeStampStart;
    float			m_riseMax;
	D3DVECTOR		m_spawnPos;
	CEXMeshObj	   *m_mesh;
};

class CTextEffectDB : public CObList
{
	public:
		CTextEffectDB(){};
		~CTextEffectDB()
		{
		   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
		   {
		       CTextEffectClass * tePtr = (CTextEffectClass *)GetNext(posLoc);
			   delete tePtr;
			   tePtr = 0;
		   }
		   RemoveAll();
		};

	D3DMATRIX m_curMatrix;
	float m_tempFloatArray[4];

	BOOL SpawnText(   CString text,
					  DWORD duration,
					  D3DVECTOR spawnPos,
					  float riseMax,
					  float red,
					  float green,
					  float blue,
					  float fontSzX,
					  float fontSzY,
					  CTextureRenderObjList * textRenderList,
					  CSymbolMapperObjList * symbolMap,
					  LPDIRECT3DDEVICE9 g_pd3dDevice,
					  CGFontObjList * cgFontList);
    
};