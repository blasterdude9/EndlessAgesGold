#pragma once

#include "Common.h"
#include "CInput.h"
#include "CXMLFileIO.h"
#include "CStateManagementClass.h"

//Enumerate all the different types of visual items
enum VisualTypes
{
	MENU_VISTYPE_NONE = 0,
	MENU_VISTYPE_WINDOW,
	MENU_VISTYPE_COMBOBOX,
	MENU_VISTYPE_LISTBOX,
	MENU_VISTYPE_TEXTBOX,
	MENU_VISTYPE_TEXT,
	MENU_VISTYPE_SCROLLBAR,
	MENU_VISTYPE_BUTTON,
	MENU_VISTYPE_TOGGLE,
	MENU_VISTYPE_PROGRESSBAR,
	MENU_VISTYPE_IMAGE,
	MENU_VISTYPE_ENTITY,
	MENU_VISTYPE_INVENTORY,
	MENU_VISTYPE_MAX
};

class CMenuVisualList; //Allows for m_childList in CMenuVisual
class CMenuScript; //Allows for m_someScript in all visuals

//CMenuVisual just provides a common interface for all Menu Objects to allow linked lists
class CMenuVisual : public CObject
{
public:
	//Settable Parameters
	CString m_name;
	int		m_visualID;
	float	m_width;
	float	m_height;
	float	m_posX;
	float	m_posY;
	bool	m_enabled;
	bool	m_visible;
	DWORD   m_miscTimeStamp;

	//Engine Variables
	bool			 m_isFocused;
	CMenuVisualList	*m_childList;

	//Functions
	CMenuVisual();
	~CMenuVisual();
	//virtual CMenuVisual& operator=(const CMenuVisual &copy);
	virtual int  GetType() { return MENU_VISTYPE_NONE; }
	virtual bool HandleInput(CInput *pInput, float parentX, float parentY, DWORD curTime, LPDIRECT3DDEVICE9 pD3DDevice) { return false; }
	virtual void Render(LPDIRECT3DDEVICE9 pD3DDevice, 
						CStateManagementObj *stateManager,
						CTextureDBObjList *textureDatabase,
						float parentX,
						float parentY) {}
	virtual void Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice) {}
	virtual void Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice);
	virtual void Serialize(CXMLFileIO &xml);
};

class CMenuVisualList : public CObList
{
public:
	bool m_freezeInput; //Allows for editing of buttons and toggles
	bool m_referenceMemory;

	CMenuVisualList() { m_freezeInput = false; m_referenceMemory = false; }
	~CMenuVisualList();

	//virtual CMenuVisualList& operator=(const CMenuVisualList &copy);
	CMenuVisual* GetVisualByID(int visualID, bool searchChildList);
	bool CloseByID(int visualID, int *showMenuCursor);
	bool CloseAll(int *showMenuCursor, bool onlyMenusWithCursor = true);
	int	 GetFreeID(int currentID);
	int  GetVisualIDCount(int searchID);
	void Clone(CMenuVisualList **pClone, LPDIRECT3DDEVICE9 pD3DDevice);
	void Copy(CMenuVisualList *pCopy, LPDIRECT3DDEVICE9 pD3DDevice);
	void DeleteByObject(CMenuVisual *curVisual);
	void Serialize(CXMLFileIO &xml);
	void ClearList();
	CMenuVisual* GetVisualByName(CString visualName, bool searchChildLists);
};