#pragma once

#include "CMenuVisual.h"
#include "CMenuFont.h"

enum TextAlignment
{
	MENU_TEXTALIGN_LEFT = 0,
	MENU_TEXTALIGN_CENTER,
	MENU_TEXTALIGN_RIGHT
};

class CMenuText : public CMenuVisual
{
public:
	CMenuText();
	~CMenuText();
	//Visible Objects
	CEXMeshObj  *m_textMesh;

	//Settable Parameters
	CMenuFont	*m_font;
	CString		 m_text;
	int			 m_alignment;
	bool		 m_multilined;

	//Temperary Variables
	float	m_red;
	float	m_green;
	float	m_blue;
	float	m_alpha;
	int		m_textWidth;
	int		m_iNumLines;
	CString	m_fontName;

	//Scripts
	CMenuScript *m_updateScript;

	//Functions
	//CMenuText& operator=(const CMenuText &rhs);
	int  GetType() { return MENU_VISTYPE_TEXT; }
	BOOL Init(LPDIRECT3DDEVICE9 pD3DDevice);
	void Render(LPDIRECT3DDEVICE9 pD3DDevice,
				CStateManagementObj *stateManager, 
				CTextureDBObjList *textureDatabase,
				float parentX,
				float parentY);
	void  SetText(char *newText);
	void  SetText(const char *newText);
	float GetLineWidth(int startChar, int &endChar);
	float GetLongestWidth();
	void  Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice);
	void  Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice);
	void  Serialize(CXMLFileIO &xml);
};