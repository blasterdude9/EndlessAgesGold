#pragma once

#include "CMenuVisual.h"
#include "CMenuImage.h"
#include "CMenuText.h"

enum ToggleStates
{
	MENU_TOGGLESTATE_UP = 0,
	MENU_TOGGLESTATE_UPHOVER,
	MENU_TOGGLESTATE_DOWN,
	MENU_TOGGLESTATE_DOWNHOVER,
	MENU_TOGGLESTATE_MAX
};

class CMenuToggle : public CMenuVisual
{
public:
	//Visible Objects
	CMenuImage *m_stateImages[MENU_TOGGLESTATE_MAX];

	//Settable Parameters
	bool m_isSet;
	int m_iGroupID;
	CString m_strValue;
	CMenuText *m_pDisplayText;
	void* m_pData; // This can be used to store any generic piece of data

	//Engine Variables
	int	 m_state;
	bool m_mousedOver;
	bool m_mousedDown;

	//Scripts
	CMenuScript *m_clickScript;
	CMenuScript *m_mouseOverScript;
	CMenuScript *m_mouseOutScript;

	//Functions
	CMenuToggle();
	~CMenuToggle();
	void InitText();
	void SetText(LPTSTR value);
	void SetEnabled(bool bEnable = true);
	int  GetType() { return MENU_VISTYPE_TOGGLE; }
	bool HandleInput(CInput *pInput, float parentX, float parentY, DWORD curTime, LPDIRECT3DDEVICE9 pD3DDevice);
	void Render(LPDIRECT3DDEVICE9 pD3DDevice,
				CStateManagementObj *stateManager, 
				CTextureDBObjList *textureDatabase,
				float parentX,
				float parentY);
	void Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice);
	void Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice);
	void Serialize(CXMLFileIO &xml);
};