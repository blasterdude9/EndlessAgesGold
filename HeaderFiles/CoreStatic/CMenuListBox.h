#pragma once

#include "CMenuVisual.h"
#include "CMenuImage.h"
#include "CMenuScrollBar.h"
#include "CMenuFont.h"

//This class is a representation of 1 entry in
//a list(even if that entry has 2 or more text outputs)
class CMenuListItem
{
public:
	CMenuListItem()
	{
		m_pData		= NULL;
		m_icon		= NULL;
		m_pNext		= NULL;
		m_pPrev		= NULL;
		m_index		= -1;
		m_special	= false;
		m_pending	= true;
	}

	~CMenuListItem()
	{
		SafeDelete(m_pData);
		m_icon		= NULL;
		m_pNext		= NULL;
		m_pPrev		= NULL;
	}

	void	AddText(CMenuText *pText, float fParentListboxWidth);
	float	GetListItemHeight();

	bool			m_pending;
	bool			m_special;
	float			m_posX;
	float			m_posY;
	int				m_index;
	int				m_miscInt1;
	int				m_miscInt2;
	int				m_miscInt3;
	int				m_miscInt4;
	float			m_miscFloat;
	void*			m_pData;
	CString			m_miscString;
	CMenuVisualList	m_textList;
	CMenuImage		*m_icon;
	CMenuListItem	*m_pNext;
	CMenuListItem	*m_pPrev;
};

class CMenuListBox : public CMenuVisual
{
public:
    CMenuListBox();
	~CMenuListBox();

	//Visible Objects
	CMenuImage		*m_borderLeft;
	CMenuImage		*m_borderTop;
	CMenuImage		*m_borderRight;
	CMenuImage		*m_borderBottom;
	CMenuImage		*m_cornerTopLeft;
	CMenuImage		*m_cornerTopRight;
	CMenuImage		*m_cornerBottomLeft;
	CMenuImage		*m_cornerBottomRight;
	CMenuImage		*m_background;
	CMenuImage		*m_itemBackground;
	CMenuScrollBar	*m_scrollBar;

	//Settable Parameters
	CMenuFont	*m_font;
	float		 m_normalRed;
	float		 m_normalGreen;
	float		 m_normalBlue;
	float		 m_normalAlpha;
	float		 m_selectedRed;
	float		 m_selectedGreen;
	float		 m_selectedBlue;
	float		 m_selectedAlpha;
	float		 m_specialRed;
	float		 m_specialGreen;
	float		 m_specialBlue;
	float		 m_specialAlpha;
	float		 m_backgroundInset;
	float		 m_cornerOffsetX;
	float		 m_cornerOffsetY;
	float		 m_itemInset;
	int			 m_numTextObjectsPerItem;
	int			 m_numColumns;
	float		 m_columnSpacing;
	float		 m_iconSpacing;
	float		 m_extraSpacingBetweenText;
	float		 m_extraSpacingBetweenItems;
	float		 m_scrollBarInset;
	bool		 m_scrollBarAlwaysVisible;
	bool		 m_dragEnabled;

	//Engine Variables
	CMenuListItem  *m_pFirstItem;
	CMenuListItem  *m_pLastItem;
	CMenuListItem  *m_draggingItem;
	int				m_numItems;
	int				m_numVisibleItems;
	float			m_itemHeight;
	CMenuListItem  *m_currentSelection;
	CMenuListItem  *m_currentHover;
	bool			m_itemJustSelected;
	bool			m_mousedOver;
	bool			m_showScrollBar;

	//Temperary Variables
	CString	m_fontName;

	//Scripts
	CMenuScript *m_selectionChangeScript;
	CMenuScript *m_mouseOverScript;
	CMenuScript *m_mouseOutScript;
	CMenuScript *m_dropScript;
	CMenuScript *m_updateScript;

	//Functions
	void DeleteMemory();
	int  GetType() { return MENU_VISTYPE_LISTBOX; }
	void AddItem(CMenuListItem *newItem, int beforeIndex = -1);
	void RemoveItem(int index);
	void ClearList();
	bool HandleInput(CInput *pInput, float parentX, float parentY, DWORD curTime, LPDIRECT3DDEVICE9 pD3DDevice);
	void Render(LPDIRECT3DDEVICE9 pD3DDevice,
				CStateManagementObj *stateManager, 
				CTextureDBObjList *textureDatabase,
				float parentX,
				float parentY);
	void Resize();
	void UpdateFontTexture();
	void Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice);
	void Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice);
	void Serialize(CXMLFileIO &xml);
	CMenuListItem* GetCurrentHoverItem(CInput *pInput, float fParentX, float fParentY);
};
