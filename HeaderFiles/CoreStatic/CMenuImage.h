#pragma once

#include "CMenuVisual.h"
#include "CEXMeshClass.h"
#include <limits>

//If we include CMenuListBox.h, we will create an inclusion loop
//but since we're using a pointer here, we can just typedef it
class CMenuListItem;

class CMenuImage : public CMenuVisual
{
public:
	//Visible Objects
	CEXMeshObj *m_imageMesh;

	//Settable Parameters
	bool m_bIsMouseOver;
	bool m_bDraggable;
	float m_texturePosX;
	float m_texturePosY;
	float m_textureWidth;
	float m_textureHeight;
	float m_fLocalOriginX;
	float m_fLocalOriginY;
	float m_fGlobalOriginX;
	float m_fGlobalOriginY;

	//Engine Variables
	float			m_fullTextureWidth;
	float			m_fullTextureHeight;
	bool			m_mousedDown;
	bool			m_mousedOver;
	CMenuListItem  *m_draggingItem;

	//Temperary Variables
	float	m_red;
	float	m_green;
	float	m_blue;
	float	m_alpha;
	int		m_blendMode;
	CString	m_textureFile;
	int		m_textureSelect;

	// Miscellaneous data
	int		m_GLID;

	//Scripts
	CMenuScript *m_clickScript;
	CMenuScript *m_mouseOverScript;
	CMenuScript *m_mouseOutScript;
	CMenuScript *m_dropScript;
	CMenuScript *m_updateScript;

	//temp override dynamic texture info-only used if a render target is specified
	IDirect3DTexture9 * m_optionalDynTexture;
	IDirect3DSurface9 * m_surfaceController;

	//Functions
	CMenuImage();
	~CMenuImage();
	//CMenuImage& operator=(const CMenuImage &rhs);
	int  GetType() { return MENU_VISTYPE_IMAGE; }
	void Init();
	void Init(LPDIRECT3DDEVICE9 pD3DDevice);
	bool HandleInput(CInput *pInput, float parentX, float parentY, DWORD curTime, LPDIRECT3DDEVICE9 pD3DDevice);
	void Render(LPDIRECT3DDEVICE9 pD3DDevice,
				CStateManagementObj *stateManager, 
				CTextureDBObjList *textureDatabase,
				float parentX,
				float parentY);
	void UpdateTextureCoords();
	void UpdateTexture(int newTextureSelect);
	void RotateTexture(float angle);
	void Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice);
	void Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice);
	void Serialize(CXMLFileIO &xml);
	bool IsMouseOver(CInput *pInput, float fParentX, float fParentY);
};
