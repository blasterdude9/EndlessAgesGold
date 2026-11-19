#pragma once

#include "CEXMeshClass.h"
#include "CXMLFileIO.h"

#define MENU_FONTCHAR_MAX	95

class CMenuFont : public CObject
{
public:
	//Settable Parameters
	CString m_name;
	float	m_characterTextureX[MENU_FONTCHAR_MAX];
	float	m_characterTextureWidth[MENU_FONTCHAR_MAX];
	float	m_characterSpacingBefore[MENU_FONTCHAR_MAX];
	float	m_characterSpacingAfter[MENU_FONTCHAR_MAX];
	float	m_height;
	int		m_charsPerLine;

	//Engine Variables
	float m_fullTextureWidth;
	float m_fullTextureHeight;

	//Temperary Variables
	int		m_blendMode;
	CString	m_textureFile;
	int		m_textureSelect;

	//Functions
    CMenuFont();
	~CMenuFont(){};

	//CMenuFont& operator=(const CMenuFont &rhs);
	void Clone(CMenuFont **pClone, LPDIRECT3DDEVICE9 pD3DDevice);
	void Copy(CMenuFont *pCopy, LPDIRECT3DDEVICE9 pD3DDevice);
	void Serialize(CXMLFileIO &xml);
};

class CMenuFontList : public CObList
{
public:
	CMenuFontList(){};
    ~CMenuFontList();

	CMenuFont* GetFontByName(CString fontName);
	CMenuFont* GetPointerFromIndex(int index);
	int  GetIndexFromPointer(CMenuFont *font);
	void Clone(CMenuFontList **pClone, LPDIRECT3DDEVICE9 pD3DDevice);
	void Copy(CMenuFontList *pCopy, LPDIRECT3DDEVICE9 pD3DDevice);
	void Serialize(CXMLFileIO &xml);
	void Flush();
};
