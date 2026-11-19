#pragma once

#include "CMenuVisual.h"
#include "CMenuListBox.h"
#include "CMenuTextBox.h"
#include "CMenuButton.h"

class CMenuComboBox : public CMenuVisual
{
public:
	//Visible Objects
	CMenuTextBox	m_textBox;
	CMenuButton		m_button;
	CMenuListBox	m_listBox;

	//Engine Variables
	bool m_listBoxShown;

	//Scripts
	CMenuScript *m_selectionChangeScript;
	CMenuScript *m_mouseOverScript;
	CMenuScript *m_mouseOutScript;

	//Functions
	CMenuComboBox();
	int  GetType() { return MENU_VISTYPE_COMBOBOX; }
	void AddItem(CMenuListItem *newItem, int beforeIndex = -1);
	void RemoveItem(int index);
	bool HandleInput(CInput *pInput, float parentX, float parentY, DWORD curTime, LPDIRECT3DDEVICE9 pD3DDevice);
	void Render(LPDIRECT3DDEVICE9 pD3DDevice,
				CStateManagementObj *stateManager, 
				CTextureDBObjList *textureDatabase,
				float parentX,
				float parentY);
	void Resize();
	void Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice);
	void Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice);
	void SafeDelete();
	void Serialize(CXMLFileIO &xml);
};