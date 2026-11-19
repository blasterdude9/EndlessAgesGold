#pragma once

#include "CMenuVisual.h"
#include "CMenuButton.h"
#include "CMenuImage.h"

class CMenuScrollBar : public CMenuVisual
{
public:

    CMenuScrollBar();
	~CMenuScrollBar();

	//Visible Objects
	CMenuImage	*m_backgroundLeft;
	CMenuImage	*m_backgroundMiddle;
	CMenuImage	*m_backgroundRight;
	CMenuImage	*m_sliderLeft;
	CMenuImage	*m_sliderMiddle;
	CMenuImage	*m_sliderRight;
	CMenuButton *m_leftButton;
	CMenuButton *m_rightButton;

	//Settable Parameters
	bool m_isVertical;
	int  m_numItems;
	int  m_visibleItems;

	//Engine Variables
	bool	m_isDragging;
	float	m_sliderSize;	//The width or height of the slider piece
	float	m_sliderBegin;	//The left or bottom edge of the slider piece
	float	m_fullSize;		//The size of the scroll bar minus the button sizes(useful in scroll button click calculations)
	int		m_curFirstItem;	//Current top visible item(very useful with lists)
	float	m_curPosition;	//Current slider position in pixels
	bool	m_mousedOver;

	//Scripts
	CMenuScript *m_changeScript;
	CMenuScript *m_mouseOverScript;
	CMenuScript *m_mouseOutScript;

	//Functions
	
	int  GetType() { return MENU_VISTYPE_SCROLLBAR; }
	bool HandleInput(CInput *pInput, float parentX, float parentY, DWORD curTime, LPDIRECT3DDEVICE9 pD3DDevice);
	void Update();
	void Render(LPDIRECT3DDEVICE9 pD3DDevice,
				CStateManagementObj *stateManager, 
				CTextureDBObjList *textureDatabase,
				float parentX,
				float parentY);
	void Resize();
	void Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice);
	void Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice);
	void Serialize(CXMLFileIO &xml);
};