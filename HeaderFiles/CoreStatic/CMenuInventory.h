#ifndef CMENUINVENTORY_H
#define CMENUINVENTORY_H

#include "CMenuImage.h"
#include "CMenuScript.h"
#include <vector>

using namespace std;

class CMenuInventoryItem
{
public:
	CMenuInventoryItem();
	CMenuInventoryItem(ITEMDATA_STRUCT_EX item);
	~CMenuInventoryItem();

	void				Clone(CMenuInventoryItem **pClone, LPDIRECT3DDEVICE9 pDevice);
	void				Copy(CMenuInventoryItem *pCopy, LPDIRECT3DDEVICE9 pDevice);

	CMenuImage				*m_pIcon;
	ITEMDATA_STRUCT_EX	m_data;
};
typedef vector<CMenuInventoryItem*> ITEMLIST;
typedef vector<CMenuImage*> BACKGROUNDLIST;

class CMenuInventory: public CMenuVisual
{
public:
	enum INVENTORY_SELECTION_TYPE
	{
		IST_ITEM = 0,
		IST_QUICKBAR,
		IST_EQUIPPED
	};

	CMenuInventory();
	~CMenuInventory();

	// Methods
	int			GetType() { return MENU_VISTYPE_INVENTORY; }
	void		Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pDevice);
	void		Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pDevice);
	void		Serialize(CXMLFileIO &xml);
	void		UpdateTexture(int iNewTextureIndex);
	bool		IsMouseOver(CInput *pInput, float fParentX, float fParentY);
	bool		HandleInput(CInput *pInput,
					float parentX,
					float parentY,
					DWORD curTime,
					LPDIRECT3DDEVICE9 pD3DDevice);
	void		Render(LPDIRECT3DDEVICE9 pD3DDevice,
					CStateManagementObj *stateManager,
					CTextureDBObjList *textureDatabase,
					float parentX,
					float parentY);
	void		CreateGrid();
	void		ClearGrid();
	void		ClearItems();
	void		RefreshItems();
	bool		RefreshItem(ITEMDATA_STRUCT_EX item);
	void		ClearSelectedItem();
	void		ClearHoverItem();
	bool		AddItem(ITEMDATA_STRUCT_EX item);
	bool		RemoveItem(int iGLID, int iQuantity = 1);
	int			Count();
	void		SetSelectionIndicator(CMenuInventoryItem *pItem,
					INVENTORY_SELECTION_TYPE eSelectionType,
					bool bShouldClone);
	void		RemoveSelectionIndicator(CMenuInventoryItem *pItem,
					INVENTORY_SELECTION_TYPE eSelectionType);
	CMenuInventoryItem** GetInventoryItemByIndex(int iIndex);
	bool		UpdateInventoryItem(ITEMDATA_STRUCT_EX item);

	// Accessors
	inline int SelectedItemIndex()
	{
		return this->m_iSelectedItemIndex;
	}

	inline int HoverItemIndex()
	{
		return this->m_iHoverItemIndex;
	}	

	inline CMenuInventoryItem* SelectedItem()
	{
		return this->m_pSelectedItemRef;
	}

	inline CMenuInventoryItem* HoverItem()
	{
		return this->m_pHoverItemRef;
	}

	// Properties
	bool				m_bActive;
	int					m_iVersion,
						m_iNumRows,
						m_iNumColumns,
						m_iRowSpacing,
						m_iColumnSpacing;
	CMenuScript			*m_clickScript,
						*m_mouseOverScript,
						*m_mouseOutScript,
						*m_dropScript,
						*m_rightClickScript,
						*m_updateScript;
	CMenuImage			*m_pPrototypeImage;
	CMenuInventoryItem	*m_pHoverItemRef,
						*m_pSelectedItemRef;
	ITEMLIST			m_items;
	BACKGROUNDLIST		m_backgrounds;

protected:
	void		ReadScript(CXMLFileIO &xml, char *name, CMenuScript **script, char *buffer, bool &closing);
	void		WriteScript(CXMLFileIO &xml, char *name, CMenuScript **script);
	void		GetSelectionIndicatorInfo(INVENTORY_SELECTION_TYPE eSelectionType,
					MENU_IDS &eIndicatorID,
					CString &strIndicatorPrefix);

private:
	int					m_iHoverItemIndex,
						m_iSelectedItemIndex;
	bool				m_mouseDown,
						m_mouseOver;
};


#endif