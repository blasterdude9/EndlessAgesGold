#pragma once

#include "CMenuVisual.h"
#include "CMenuImage.h"
#include "CMenuText.h"

class CMenuWindow : public CMenuVisual
{
public:
	CMenuWindow();
	~CMenuWindow();

	//Visible Objects
	CMenuText	*m_titleText;
	CMenuImage	*m_titleLeft;
	CMenuImage	*m_titleMiddle;
	CMenuImage	*m_titleRight;
	CMenuImage	*m_borderLeft;
	CMenuImage	*m_borderTop;
	CMenuImage	*m_borderRight;
	CMenuImage	*m_borderBottom;
	CMenuImage	*m_cornerTopLeft;
	CMenuImage	*m_cornerTopRight;
	CMenuImage	*m_cornerBottomLeft;
	CMenuImage	*m_cornerBottomRight;
	CMenuImage	*m_background;

	//Settable Parameters
	bool	m_initialized;
	float	m_backgroundInset;
	float	m_cornerOffsetX;
	float	m_cornerOffsetY;
	bool	m_escapable;
	bool	m_draggable;
	bool	m_dragByTitleOnly;
	bool	m_resizable;
	bool	m_stretchTitle;
	bool	m_showCursor;
	float	m_horizontalAlignment;
	float	m_verticalAlignment;
	BOOL    m_instance;

	//Engine Variables
	bool  m_isDragging;
	float m_titleRgnLeft;
	float m_titleRgnTop;
	float m_titleRgnRight;
	float m_titleRgnBottom;

	//Scripts
	CMenuScript *m_loadScript;
	CMenuScript *m_resizeScript;
	CMenuScript *m_closeScript;

	//Functions
	
	int  GetType() { return MENU_VISTYPE_WINDOW; }
	void Init();
	bool HandleInput(CInput *pInput, float parentX, float parentY, DWORD curTime, LPDIRECT3DDEVICE9 pD3DDevice);
	void Render(LPDIRECT3DDEVICE9 pD3DDevice,
				CStateManagementObj *stateManager,
				CTextureDBObjList *textureDatabase,
				float parentX,
				float parentY);
	void Resize();
	void Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice);
	void Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice);
	void Serialize(CXMLFileIO &xml);
	void ReinitMenuTextures(CTextureDBObjList * textureDataBase,CMenuFontList * menuFontList,LPDIRECT3DDEVICE9 device,CMenuListItem *draggingItem);

private:
	// Version
	const /* static */ int m_version;// = 2;
};