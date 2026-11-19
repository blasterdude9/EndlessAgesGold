#pragma once

#include "CMenuVisual.h"
#include "CMenuImage.h"
#include "CMenuText.h"

//If we include CMenuListBox.h, we will create an inclusion loop
//but since we're using a pointer here, we can just typedef it
class CMenuListItem;

enum MenuButtonStates
{
	MENU_BUTTONSTATE_UP = 0,
	MENU_BUTTONSTATE_HOVER,
	MENU_BUTTONSTATE_DOWN,
	MENU_BUTTONSTATE_DISABLED,
	MENU_BUTTONSTATE_MAX
};

enum ButtonImages
{
	MENU_BUTTONIMAGE_LEFT = 0,
	MENU_BUTTONIMAGE_MIDDLE,
	MENU_BUTTONIMAGE_RIGHT,
	MENU_BUTTONIMAGE_MAX
};

class CMenuButton : public CMenuVisual
{
public:
    CMenuButton();
	~CMenuButton();

	//Visible Objects
	CMenuImage *m_stateImages[MENU_BUTTONSTATE_MAX][MENU_BUTTONIMAGE_MAX];

	//Settable Parameters
	ButtonState m_buttonState; //Used to determine actions that should be taken from input
	CMenuText *m_pDisplayText;

	//Engine Variables
	int				m_visibleState; //Used to determine which button state should be rendered
	bool			m_mousedOver;
	CMenuListItem * m_draggingItem;

	//Scripts
	CMenuScript *m_clickScript;
	CMenuScript *m_mouseOverScript;
	CMenuScript *m_mouseOutScript;
	CMenuScript *m_dropScript;

	// Accessors/Mutators
	void SetText(LPTSTR value);
	void SetEnabled(bool bEnable = true);
	int  GetType() { return MENU_VISTYPE_BUTTON; }

	//Functions
	void DeleteMemory();
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
	void SetButtonState(MenuButtonStates eState)
	{
		this->m_visibleState = eState;
	}

protected:
	void InitText();
};