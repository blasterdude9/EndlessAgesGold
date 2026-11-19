#include "stdafx.h"
#include "direct.h"
#include "CMenuListBox.h"
#include "CMenuScript.h"
#include "GlobalStatusVariables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern GlobalStatusVariables* globalStatus;

void
CMenuListItem::AddText(CMenuText *pText, float fParentListboxWidth)
{
	// CONFIGURE the text display for line wrap
	pText->m_width = fParentListboxWidth - 25.0f;
	pText->m_multilined = true;

	// ADD the CMenuText object to the child visual list
	this->m_textList.AddTail(pText);
}

float
CMenuListItem::GetListItemHeight()
{
	float fHeight = 0.0f;
	for (POSITION pos = this->m_textList.GetHeadPosition(); pos != NULL;)
	{
		CMenuText *pText = dynamic_cast<CMenuText*>(this->m_textList.GetNext(pos));
		if (!pText) continue;
		fHeight += (pText->m_iNumLines * pText->m_font->m_height);
	}

	if (this->m_icon)
	{
		if (this->m_icon->m_height > fHeight)
			fHeight = this->m_icon->m_height;
	}
	return fHeight;
}

CMenuListBox::CMenuListBox()
{
	//Visible Objects
	m_borderLeft		= NULL;
	m_borderTop			= NULL;
	m_borderRight		= NULL;
	m_borderBottom		= NULL;
	m_cornerTopLeft		= NULL;
	m_cornerTopRight	= NULL;
	m_cornerBottomLeft	= NULL;
	m_cornerBottomRight	= NULL;
	m_background		= NULL;
	m_itemBackground	= NULL;
	m_scrollBar			= NULL;

	//Settable Parameters
	m_font						= NULL;
	m_backgroundInset			= 0.0f;
	m_itemInset					= 0.0f;
	m_cornerOffsetX				= 0.0f;
	m_cornerOffsetY				= 0.0f;
	m_scrollBarInset			= 0.0f;
	m_numColumns				= 1;
	m_columnSpacing				= 150.0f;
	m_iconSpacing				= 10.0f;
	m_extraSpacingBetweenText	= 0.0f;
	m_extraSpacingBetweenItems	= 0.0f;
	m_numTextObjectsPerItem		= 1;
	m_scrollBarAlwaysVisible	= true;
	m_dragEnabled				= false;

	//Engine Variables
	m_pFirstItem		= NULL;
	m_pLastItem			= NULL;
	m_numItems			= 0;
	m_numVisibleItems	= 1;
	m_itemHeight		= 1.0f;
	m_currentSelection	= NULL;
	m_currentHover		= NULL;
	m_itemJustSelected	= false;
	m_mousedOver		= false;
	m_showScrollBar		= false;
	m_draggingItem		= NULL;

	//Scripts
	m_selectionChangeScript	= NULL;
	m_mouseOverScript		= NULL;
	m_mouseOutScript		= NULL;
	m_dropScript			= NULL;
	m_updateScript			= NULL;

	//Temperary Variables
	m_normalRed		= 1.0f;
	m_normalGreen	= 1.0f;
	m_normalBlue	= 1.0f;
	m_normalAlpha	= 1.0f;
	m_selectedRed	= 1.0f;
	m_selectedGreen	= 0.0f;
	m_selectedBlue	= 0.0f;
	m_selectedAlpha	= 1.0f;
	m_specialRed	= 0.0f;
	m_specialGreen	= 0.0f;
	m_specialBlue	= 1.0f;
	m_specialAlpha	= 1.0f;
}

void CMenuListBox::AddItem(CMenuListItem *newItem, int beforeIndex)
{
	//Loop through and set the text's font
	POSITION posText = newItem->m_textList.GetHeadPosition();
	while(posText != NULL)
	{
		//Get the current text object and set its font
		CMenuText *curText = (CMenuText*)newItem->m_textList.GetNext(posText);
		curText->m_font = m_font;
		curText->Init(globalStatus->m_3dDevice);
	}

	//If this is the first item added to the list
	if(!m_pFirstItem)
	{
		//This is the first item in the list
		m_pLastItem = m_pFirstItem = newItem;

		//Store the index of the new item
		newItem->m_index = 0;

		//If this item has an icon
		if(newItem->m_icon)
			//Make sure its height is less than each entry
			Resize();
	}
	//If we get a -1 index, add it to the end of the list
	else if(beforeIndex == -1)
	{
		//Store the index of the new item
		newItem->m_index = m_pLastItem->m_index + 1;

		//Make the last item and new item point to each other
		m_pLastItem->m_pNext = newItem;
		newItem->m_pPrev	 = m_pLastItem;

		//Its now the new last item
		m_pLastItem = newItem;
	}
	else //Put it before the specified index
	{
		//Store the index of the new item
		newItem->m_index = beforeIndex;

		//Find the specified index
		CMenuListItem *curItem = m_pFirstItem;
		while(beforeIndex > 0)
		{
			curItem = curItem->m_pNext;
			beforeIndex--;
		}

		// Update first item pointer if the specified index was 0
		if(curItem == m_pFirstItem) 
			m_pFirstItem = newItem;

		//Set the new items next and prev pointers
		newItem->m_pPrev = curItem->m_pPrev;
		newItem->m_pNext = curItem;

		//If we have a previous, point its next to the new item
		if(curItem->m_pPrev)
			curItem->m_pPrev->m_pNext = newItem;

		//Point this item's previous to the new item
		curItem->m_pPrev = newItem;

		//Increase the index of every item after our added item
		while(curItem)
		{
			curItem->m_index++;
			curItem = curItem->m_pNext;
		}
	}

	//We've added another item
	m_numItems++;

	//Update the scroll bar if it exists
	if(m_scrollBar)
		m_scrollBar->m_numItems = m_numItems;

	// RESIZE the contents of the listbox
	//this->Resize();
}

//This deletes the item and the text object, but not the icon
void CMenuListBox::RemoveItem(int index)
{
	//Start from the first item
	CMenuListItem *curItem = m_pFirstItem;

	//If we're removing the first item
	if(index == 0)
		//Point the first item pointer to it's next
		m_pFirstItem = curItem->m_pNext;
	else
	{
		//Find the specified index
		while(index > 0)
		{
			curItem = curItem->m_pNext;
			index--;
		}

		//If we're removing the last item
		if(curItem == m_pLastItem)
			//Point the last item pointer to it's previous
			m_pLastItem = m_pLastItem->m_pPrev;
	}

	//Save the current item so we can delete it later
	CMenuListItem *deleteItem = curItem;

	//Decrease the index of every item after our removed item
	while(curItem->m_pNext)
	{
		curItem = curItem->m_pNext;
		curItem->m_index--;
	}
		
	//Point the previous item to the next item
	if(deleteItem->m_pPrev)
		deleteItem->m_pPrev->m_pNext = deleteItem->m_pNext;

	//Point the next item to the previous item
	if(deleteItem->m_pNext)
		deleteItem->m_pNext->m_pPrev = deleteItem->m_pPrev;

	//Safe Delete the text list and delete the item
	delete deleteItem;
	deleteItem = NULL;

	//We've removed an item
	m_numItems--;

	//If we deleted all items
	if(m_numItems == 0)
	{
		m_pFirstItem = NULL;
		m_pLastItem  = NULL;
	}

	//Update the scroll bar if it exists
	if(m_scrollBar)
	{
		m_scrollBar->m_numItems = m_numItems;
		m_scrollBar->Update();
	}
}

void CMenuListBox::ClearList()
{
	//If the list exists
	if(m_pFirstItem)
	{
		//Loop through and delete every item but the last one
		CMenuListItem *curItem = m_pFirstItem;
		while(curItem->m_pNext)
		{
			curItem = curItem->m_pNext;
			SafeDelete(curItem->m_pPrev);
		}

		//Delete the only item left
		SafeDelete(curItem);

		//Reset Item Count
		m_numItems = 0;
		if(m_scrollBar)
			m_scrollBar->m_numItems = 0;

		//Null out our list pointers
		m_pFirstItem		= NULL;
		m_pLastItem			= NULL;
		m_currentSelection	= NULL;
	}
}

CMenuListItem* 
CMenuListBox::GetCurrentHoverItem(CInput *pInput, float fParentX, float fParentY)
{
	// IF there are any list items
	if (m_pFirstItem)
	{
		float	fPosX = m_posX + fParentX,
				fPosY = m_posY + fParentY + this->m_height;
		int iIndex = 0;
		CMenuListItem *pListItem = NULL;

		// ERROR checking: Do we have an empty list?
		if (!m_pFirstItem)
			return NULL;
		pListItem = this->m_pFirstItem;

		// IF mouse is not within X axis bounds, RETURN NULL
		if ((pInput->m_mouseAbsPosX < fPosX)
		|| (pInput->m_mouseAbsPosX > (fPosX + this->m_width)))
			return NULL;

		// IF we have a scroll bar, the first item may not be the first visible item, 
		//	so loop through until we get to the current first item
		if (m_scrollBar)
		{
			while (iIndex < m_scrollBar->m_curFirstItem)
			{
				pListItem = pListItem->m_pNext;
				iIndex++;
			}
		}

		// LOOP through every visible list item and check the
		//	current mouse pos against its Y-boundaries
		while (pListItem 
		&& pListItem->m_pNext 
		&& ((iIndex - m_scrollBar->m_curFirstItem) <= this->m_numVisibleItems))
		{
			// UPDATE the Y-axis position
			fPosY = pListItem->m_posY + (2 * this->m_itemHeight) + this->m_height;

			// IF this is the list item that's moused over...
			if ((pInput->m_mouseAbsPosY < fPosY)
			&& (pInput->m_mouseAbsPosY > (fPosY - this->m_itemHeight)))
				return pListItem;

			// ADVANCE the item pointer and current index
			pListItem = pListItem->m_pNext;
			++iIndex;
		}
	}
	return NULL;
}

bool
CMenuListBox::HandleInput(CInput *pInput, 
						  float parentX, 
						  float parentY, 
						  DWORD curTime, 
						  LPDIRECT3DDEVICE9 pD3DDevice)
{
	// The list box is now the parent
	parentX += m_posX;
	parentY += m_posY;

	// No first item means there's nothing to handle so RETURN

	// Execute the update script
	if(m_updateScript)
	{
		if (m_updateScript->m_pFirstLine 
		&& (m_updateScript->m_pFirstLine->m_function == -1)) 
			SafeDelete(m_updateScript);
		else
			m_updateScript->ExecuteScript();
	}

	// Check to see if the cursor is within the listbox bounds
	if ((pInput->m_mouseAbsPosX > parentX) 
	&& (pInput->m_mouseAbsPosX < parentX + m_width)
	&& (pInput->m_mouseAbsPosY > parentY)
	&& (pInput->m_mouseAbsPosY < parentY + m_height))
	{
		// Continuously execute the mouseover script (allows for pausing)
		m_mousedOver = true;
		if(m_mouseOverScript)
		{
			m_mouseOverScript->ExecuteScript();
			this->m_currentHover = this->GetCurrentHoverItem(pInput, parentX, parentY);
		}
	}
	else
	{
		// If they just moved the cursor out of the listbox...
		if(m_mousedOver && m_mouseOutScript)
		{
			// This is only executed once
			m_mouseOutScript->ExecuteScript();
		}
		m_mousedOver = false;
	}

	// Invalidate any selected item and reset status flags
	if (pInput->m_keyboardKeys[DIK_ESCAPE].JustPressed())
	{
		this->m_currentSelection = NULL;
		this->m_itemJustSelected = false;
		this->m_dragEnabled = false;
		return true;
	}

	// First, see if the scroll bar wants to handle the input
	if (m_showScrollBar
	&& m_scrollBar
	&& m_scrollBar->HandleInput(pInput, parentX, parentY, curTime, pD3DDevice))
		return true;

	// If they just clicked the left mouse button...
	if (pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustPressed())
	{
		// If the cursor is inside the listbox...
		if (m_mousedOver)
		{
			// If we have a scroll bar, then we may have scrolled down
			int iIndex = 0;
			if(m_scrollBar)	
				iIndex += m_scrollBar->m_curFirstItem;

			// INIT y-pos to the top of the listbox
			float	top = (parentY + m_height - m_itemInset), 
					bottom = top;

			// Find the currently selected list item
			CMenuListItem *pSelectedItem = NULL;
			if (this->m_pFirstItem
			&& (iIndex < this->m_numItems))
			{
				pSelectedItem = this->m_pFirstItem;
				while (iIndex > 0)
				{
					pSelectedItem = pSelectedItem->m_pNext;
					--iIndex;
				}

				while (pSelectedItem)
				{
					// CALCULATE the bottom coord of the current item
					bottom -= pSelectedItem->GetListItemHeight() 
						+ this->m_extraSpacingBetweenItems;

					// EITHER we've found the selected item
					//	OR we advance the pointer
					if ((pInput->m_mouseAbsPosY > bottom)
					&& (pInput->m_mouseAbsPosY < top))
						break;
					else
						pSelectedItem = pSelectedItem->m_pNext;

					// SET the top coord of the next item equal to
					//	the bottom of the previous item
					top = bottom;
				}
			}

			// IF the same item was selected again, DESELECT it
			if (m_currentSelection == pSelectedItem)
			{
				m_currentSelection = NULL;
				m_itemJustSelected = false;
			}
			// ELSE, SET the selected item
			else
			{
				m_currentSelection = pSelectedItem;
				m_itemJustSelected = true;
			}
			/*// Get the item clicked on
			int selected = (int)(((parentY + m_height - m_itemInset) - pInput->m_mouseAbsPosY) / (m_itemHeight + m_extraSpacingBetweenItems));

			// If we have a scroll bar, then we may have scrolled down
			if(m_scrollBar)	selected += m_scrollBar->m_curFirstItem;

			// Find the currently selected list item
			CMenuListItem *pSelected = NULL;
			if(m_pFirstItem)
			if(selected < m_numItems)
			{
				pSelected = m_pFirstItem;
				while(selected > 0)
				{
					pSelected = pSelected->m_pNext;
					selected--;
				}
			}

			// IF the same item was selected again, DESELECT it
			if (m_currentSelection == pSelected)
			{
				m_currentSelection = NULL;
				m_itemJustSelected = false;
			}
			// ELSE, SET the selected item
			else
			{
				m_currentSelection = pSelected;
				m_itemJustSelected = true;
			}*/

			// Execute the selection change script if it exists
			if (m_currentSelection
			&& m_selectionChangeScript)
				m_selectionChangeScript->ExecuteScript();
			return true;
		}
	}
	// If the left mouse button has been pressed and held
	else if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].IsHeld())
	{
		// If we just selected an item
		if(m_itemJustSelected && m_currentSelection)
		{
			// If the player moves the mouse
			if(pInput->m_menuMouseChangeX || pInput->m_menuMouseChangeY)
			{
				// Reset the selected flag
				m_itemJustSelected = false;
			}
		}
	}
	else if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustReleased())
	{
		// If their mouse is released inside the list box
		if (globalStatus->m_draggingItem)
		{
			// Execute the drop script, if it exists
			if(m_dropScript) m_dropScript->ExecuteScript();
			return false;
		}
	}

	// Reset the selected flag
	//m_itemJustSelected = false;

	// We haven't handled the input so RETURN FALSE
	return false;
}

/*bool CMenuListBox::HandleInput(CInput *pInput, float parentX, float parentY, DWORD curTime, LPDIRECT3DDEVICE9 pD3DDevice)
{
	//The list box is now the parent
	parentX += m_posX;
	parentY += m_posY;

	// No items, nothing to handle
	if(!m_pFirstItem) return false;

	//First, see if the scroll bar wants to handle the input
	if(m_showScrollBar)
		if(m_scrollBar->HandleInput(pInput, parentX, parentY, curTime, pD3DDevice))
			return true;

	if(m_updateScript)
	{
		m_updateScript->ExecuteScript();
		if(m_updateScript->m_pFirstLine 
		&& (m_updateScript->m_pFirstLine->m_function == -1)) 
			SafeDelete(m_updateScript);
	}
    
	if(pInput->m_keyboardKeys[DIK_ESCAPE].JustPressed())
	{
		this->m_currentSelection = NULL;
		this->m_itemJustSelected = false;
		return true;
	}

	//If they just clicked the left mouse button
	if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustPressed())
	{
		//If they clicked inside of the list box
		if(	pInput->m_mouseAbsPosX > parentX && pInput->m_mouseAbsPosX < parentX + m_width &&
			pInput->m_mouseAbsPosY > parentY && pInput->m_mouseAbsPosY < parentY + m_height)
		{
			//Reset the current selection
			//m_currentSelection = NULL;

			//Get the item clicked on
			int selected = ((parentY + m_height - m_itemInset) - pInput->m_mouseAbsPosY) / (m_itemHeight + m_extraSpacingBetweenItems);

			//If we have a scroll bar, then we may have scrolled down
			if(m_scrollBar)
				selected += m_scrollBar->m_curFirstItem;

			//Find the currently selected list item
			CMenuListItem *pSelected = NULL;
			if(selected < m_numItems)
			{
				/*m_currentSelection = m_pFirstItem;
				while(selected > 0)
				{
					m_currentSelection = m_currentSelection->m_pNext;
					selected--;
				}* /
				pSelected = m_pFirstItem;
				while(selected > 0)
				{
					pSelected = pSelected->m_pNext;
					selected--;
				}
			}

			// If there is a valid currently selected item...
			if(m_currentSelection && (pSelected == m_currentSelection))
				m_itemJustSelected = false;
			else 
				m_itemJustSelected = true;

			// If the same item is selected twice consecutively, deselect it
			if(m_itemJustSelected/*!m_currentSelection* /)
			{
				// Set the selected item and flag
				m_currentSelection = pSelected;
				
				//Execute the selection change script, if it exists
				if(m_selectionChangeScript)
					m_selectionChangeScript->ExecuteScript();
			}
			else 
			{
				m_currentSelection = NULL;			
			}
			return true;
		}
	}
	//If the left mouse button has been pressed and held
	else if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].IsHeld())
	{
		//If we just selected an item
		if(m_itemJustSelected && m_currentSelection)
		{
			//If the player moves the mouse
			if(pInput->m_menuMouseChangeX || pInput->m_menuMouseChangeY)
			{
				//Create the dragging item and copy the current selection
				/*
				*m_draggingItem					= new CMenuListItem();
				(*m_draggingItem)->m_icon		= m_currentSelection->m_icon;
				(*m_draggingItem)->m_miscInt1	= m_currentSelection->m_miscInt1;
				(*m_draggingItem)->m_miscInt2	= m_currentSelection->m_miscInt2;
				(*m_draggingItem)->m_miscInt3	= m_currentSelection->m_miscInt3;
				(*m_draggingItem)->m_miscInt4	= m_currentSelection->m_miscInt4;
				(*m_draggingItem)->m_miscFloat	= m_currentSelection->m_miscFloat;
				(*m_draggingItem)->m_miscString	= m_currentSelection->m_miscString;
				m_currentSelection->m_textList.Copy(&(*m_draggingItem)->m_textList, pD3DDevice);
				* /

				// Set the dragging flag
				m_dragEnabled = true;

				// Reset the selected flag
				m_itemJustSelected = false;
			}

			return true;
		}
	}
	else if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustReleased())
	{
		//If their mouse is released inside the list box
		if (m_draggingItem
		&& (pInput->m_mouseAbsPosX > parentX)
		&& (pInput->m_mouseAbsPosX < parentX + m_width)
		&& (pInput->m_mouseAbsPosY > parentY)
		&& (pInput->m_mouseAbsPosY < parentY + m_height))
		{
			return true;
		}
		else
		{
			//Continuously execute the script(allows for pausing)
			if(m_mouseOverScript)
				m_mouseOverScript->ExecuteScript();

			//Execute the drop script, if it exists
			if(m_dropScript)
				m_dropScript->ExecuteScript();

			//Allow the mouse out script to run
			m_mousedOver = true;
		}
	}
	//If they just moved their mouse out of the list box
	else if(m_mousedOver)
	{
		//This is only executed once
		if(m_mouseOutScript)
			m_mouseOutScript->ExecuteScript();

		//We're no longer hovering over the list box
		m_mousedOver = false;
	}

	//Reset the selected flag
	m_itemJustSelected = false;

	return false;
}*/

void CMenuListBox::Render(	LPDIRECT3DDEVICE9 pD3DDevice,
							CStateManagementObj *stateManager, 
							CTextureDBObjList *textureDatabase,
							float parentX,
							float parentY)
{
	// CHECK for any pending list items that will necessitate a Resize() update
	bool bIsPending = false;

	//The List Box is the parent now
	parentX += m_posX;
	parentY += m_posY;

	//Render the background, if it exists
	if(m_background)
		m_background->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	//If we have a left border, we can assume
	//we have all borders and corners created
	if(m_borderTop)
	{
		m_borderTop->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);
		m_borderLeft->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);
		m_borderBottom->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);
		m_borderRight->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);
		m_cornerTopLeft->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);
		m_cornerTopRight->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);
		m_cornerBottomLeft->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);
		m_cornerBottomRight->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);
	}

	// Get the height of a single item and the width of the icons in the list
	//	NOTE: We'll calculate the line height WITHIN the loop to account for multi-line textitems 
	//	with a differing number of lines.
	m_itemHeight	= ((float)m_numTextObjectsPerItem / m_numColumns) * (/*m_font->m_height +*/ m_extraSpacingBetweenText);
	float iconWidth = 0.0f;
	if(m_pFirstItem && m_pFirstItem->m_icon)
	{
		//If the icons are taller than the text, use the icon height
		if(m_pFirstItem->m_icon->m_height > m_itemHeight)
			m_itemHeight = m_pFirstItem->m_icon->m_height;

		//Grab the icon width
		iconWidth = m_pFirstItem->m_icon->m_width;
	}

	// CHECK for any pending list items
	CMenuListItem *curItem = m_pFirstItem;
	while(curItem)
	{
		if (curItem->m_pending)
		{
			bIsPending = true;
			curItem->m_pending = false;
		}
		curItem = curItem->m_pNext;
	}

	//Render the text
	int curIndex = 0, numRenderedItems = 0;
	float curPosX;
	float itemBackgroundY = parentY + m_height;
	float textStartX = parentX + m_itemInset + iconWidth + m_iconSpacing;
	float itemStartY = parentY + m_height - m_itemInset;
	float curPosY	 = itemStartY - m_font->m_height;
	curItem = m_pFirstItem;
	while (curItem
	&& ((numRenderedItems < m_numVisibleItems) || bIsPending))
	{
		//Reset the current X position
		curPosX = parentX + m_itemInset;

		// SET the X pos for this list item
		curItem->m_posX = curPosX;
		curItem->m_posY = curPosY;

		//If we have a scroll bar, the first item may not be the first
		//visible item, so loop through until we get to the Current First Item
		if(m_scrollBar && curIndex < m_scrollBar->m_curFirstItem)
		{
			curItem = curItem->m_pNext;
			curIndex++;
			continue;
		}

		//See if this item is selected
		float red, green, blue, alpha;
		if(curItem == m_currentSelection)
		{
			red		= m_selectedRed;
			green	= m_selectedGreen;
			blue	= m_selectedBlue;
			alpha	= m_selectedAlpha;
		}
		else if(curItem->m_special)
		{
			red		= m_specialRed;
			green	= m_specialGreen;
			blue	= m_specialBlue;
			alpha	= m_specialAlpha;
		}
		else
		{
			red		= m_normalRed;
			green	= m_normalGreen;
			blue	= m_normalBlue;
			alpha	= m_normalAlpha;
		}

		//If we have a background, render it before the icon and text
		if(m_itemBackground)
		{
			itemBackgroundY -= m_itemBackground->m_height;
			m_itemBackground->Render(pD3DDevice, stateManager, textureDatabase, parentX, itemBackgroundY);
		}

		//If this item has an icon, render it
		if(curItem->m_icon)
			curItem->m_icon->Render(pD3DDevice, stateManager, textureDatabase, curPosX, itemStartY - curItem->m_icon->m_height);

		//Loop through and render the text for this item
		curPosX = textStartX;
		int curColumn = 0;
		POSITION posText = curItem->m_textList.GetHeadPosition();
		//float rMultilineTextHeight = 0.0f;
		//float fActualHeight = this->m_itemHeight;
		while(posText != NULL)
		{
			//Get the current text object
			CMenuText *curText = dynamic_cast<CMenuText*>(curItem->m_textList.GetNext(posText));
			if (!curText) continue;

			//Set the color for the text
			if(curText->m_textMesh)
			{
				curText->m_textMesh->List_materialObject->m_useMaterial.Diffuse.r  = red;
				curText->m_textMesh->List_materialObject->m_useMaterial.Diffuse.g  = green;
				curText->m_textMesh->List_materialObject->m_useMaterial.Diffuse.b  = blue;
				curText->m_textMesh->List_materialObject->m_useMaterial.Diffuse.a  = alpha;
				curText->m_textMesh->List_materialObject->m_useMaterial.Emissive.r = red;
				curText->m_textMesh->List_materialObject->m_useMaterial.Emissive.g = green;
				curText->m_textMesh->List_materialObject->m_useMaterial.Emissive.b = blue;
				curText->m_textMesh->List_materialObject->m_useMaterial.Emissive.a = alpha;
			}

			//Render it to the screen
			curText->Render(pD3DDevice, stateManager, textureDatabase, curPosX, curPosY);

			//Add to the Y coordinate if we goto the next line
			curColumn++;
			if(curColumn == m_numColumns)
			{
				// CALCULATE new Y position considering multilined text
				/*float rHeight = 0.0f;
				if (curText->m_multilined)
				{
					/*rHeight = curText->m_iNumLines * m_font->m_height;
					if (rHeight > rMultilineTextHeight)
						rMultilineTextHeight = rHeight;
					curPosY -= (rHeight + m_extraSpacingBetweenText);* /

					// ADD the height of this CMenuText object to the total item height
					fActualHeight += (m_font->m_height * curText->m_iNumLines);
					curPosY -= fActualHeight;
				}
				else
				{
					curPosY		-= (m_font->m_height + m_extraSpacingBetweenText);
				}*/
				curPosY -= (curItem->GetListItemHeight() + this->m_extraSpacingBetweenText);
				curPosX = textStartX;
				curColumn = 0;
			}
			else
				//Add to the X coordinate if we're staying on the same line
				curPosX += m_columnSpacing;
		}

		//Increase the vertical position by the height of a single item entry
		//itemStartY -= m_itemHeight + m_extraSpacingBetweenItems;
		/*if (rMultilineTextHeight > m_itemHeight)
			itemStartY -= rMultilineTextHeight + m_extraSpacingBetweenItems;
		else 
			itemStartY -= m_itemHeight + m_extraSpacingBetweenItems;*/
		itemStartY -= curItem->GetListItemHeight() + m_extraSpacingBetweenItems;
		curPosY		= itemStartY - m_font->m_height;

		//Increment the number of rendered items
		numRenderedItems++;

		//Goto the next item in the list
		curItem = curItem->m_pNext;
	}

	if (bIsPending)
		this->Resize();

	//Render the scroll bar next, if we have one
	if(m_showScrollBar)
		m_scrollBar->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);
}

void CMenuListBox::Resize()
{
	if(m_background)
	{
		m_background->m_posX	= m_backgroundInset;
		m_background->m_posY	= m_backgroundInset;
		m_background->m_width	= m_width - (2 * m_backgroundInset);
		m_background->m_height	= m_height - (2 * m_backgroundInset);
	}

	if(m_borderTop)
	{
		m_borderTop->m_posX		= m_cornerTopLeft->m_width - m_cornerOffsetX;
		m_borderTop->m_posY		= m_height - m_borderTop->m_textureHeight;
		m_borderTop->m_width	= m_width - (m_cornerTopLeft->m_width + m_cornerTopRight->m_width - (2 * m_cornerOffsetX));
		m_borderTop->m_height	= m_borderTop->m_textureHeight;

		m_borderLeft->m_posX	= 0.0f;
		m_borderLeft->m_posY	= m_cornerBottomLeft->m_height - m_cornerOffsetY;
		m_borderLeft->m_width	= m_borderLeft->m_textureWidth;
		m_borderLeft->m_height	= m_height - (m_cornerBottomLeft->m_height + m_cornerTopLeft->m_height - (2 * m_cornerOffsetY));

		m_borderBottom->m_posX	 = m_cornerBottomLeft->m_width - m_cornerOffsetX;
		m_borderBottom->m_posY	 = 0.0f;
		m_borderBottom->m_width	 = m_width - (m_cornerBottomLeft->m_width + m_cornerBottomRight->m_width - (2 * m_cornerOffsetX));
		m_borderBottom->m_height = m_borderBottom->m_textureHeight;

		m_borderRight->m_posX	= m_width - m_borderRight->m_textureWidth;
		m_borderRight->m_posY	= m_cornerBottomRight->m_height - m_cornerOffsetY;
		m_borderRight->m_width	= m_borderRight->m_textureWidth;
		m_borderRight->m_height	= m_height - (m_cornerBottomRight->m_height + m_cornerTopRight->m_height - (2 * m_cornerOffsetY));

		m_cornerTopLeft->m_posX		= -m_cornerOffsetX;
		m_cornerTopLeft->m_posY		= m_height - m_cornerTopLeft->m_textureHeight + m_cornerOffsetY;
		m_cornerTopLeft->m_width	= m_cornerTopLeft->m_textureWidth;
		m_cornerTopLeft->m_height	= m_cornerTopLeft->m_textureHeight;

		m_cornerTopRight->m_posX	= m_width - m_cornerTopRight->m_textureWidth + m_cornerOffsetX;
		m_cornerTopRight->m_posY	= m_height - m_cornerTopRight->m_textureHeight + m_cornerOffsetY;
		m_cornerTopRight->m_width	= m_cornerTopRight->m_textureWidth;
		m_cornerTopRight->m_height	= m_cornerTopRight->m_textureHeight;

		m_cornerBottomLeft->m_posX		= -m_cornerOffsetX;
		m_cornerBottomLeft->m_posY		= -m_cornerOffsetY;
		m_cornerBottomLeft->m_width		= m_cornerBottomLeft->m_textureWidth;
		m_cornerBottomLeft->m_height	= m_cornerBottomLeft->m_textureHeight;

		m_cornerBottomRight->m_posX		= m_width - m_cornerBottomRight->m_textureWidth + m_cornerOffsetX;
		m_cornerBottomRight->m_posY		= -m_cornerOffsetY;
		m_cornerBottomRight->m_width	= m_cornerBottomRight->m_textureWidth;
		m_cornerBottomRight->m_height	= m_cornerBottomRight->m_textureHeight;
	}

	/*
	m_itemHeight = ((float)m_numTextObjectsPerItem / m_numColumns) * (m_font->m_height + m_extraSpacingBetweenText);
	if(m_pFirstItem)
		if(m_pFirstItem->m_icon)
			if(m_pFirstItem->m_icon->m_height > m_itemHeight)
				m_itemHeight = m_pFirstItem->m_icon->m_height;
				*/

	// CALCULATE the total number of items
	CMenuListItem *pItem = this->m_pFirstItem;
	this->m_numItems = 0;
	while (pItem)
	{
		++this->m_numItems;
		pItem = pItem->m_pNext;
	}

	// CALCULATE the number of visible items
	int iIndex = 0;
	float fHeight = 0.0f;
	this->m_numVisibleItems = 0;
	pItem = this->m_pFirstItem;
	while (pItem)
	{
		if(this->m_scrollBar)
		{
			if (iIndex < this->m_scrollBar->m_curFirstItem)
			{
				++iIndex;
				continue;
			}
		}

		// ADD in height of the current list item
		fHeight += pItem->GetListItemHeight() + this->m_extraSpacingBetweenItems;
		if (fHeight > this->m_height) break;
		++this->m_numVisibleItems;

		// ADVANCE the item pointer
		pItem = pItem->m_pNext;
	}

	/* DEPRECATED /////////////////////////////////////////////////////////////
	/*m_numVisibleItems = (m_height - (2 * m_itemInset)) / (m_itemHeight + m_extraSpacingBetweenItems);* /
	m_numVisibleItems = (int)(ceil((m_height - (2.0f * m_itemInset)) / (m_itemHeight + m_extraSpacingBetweenItems))-1.0f);
	if(m_numVisibleItems < 0)
	   m_numVisibleItems = 0;
	//WARNING: C4244 --> float to int
	/////////////////////////////////////////////////////////////////////////*/

	if(m_scrollBar)
	{
		m_showScrollBar				= m_scrollBarAlwaysVisible || m_numItems > m_numVisibleItems;
		//m_scrollBar->m_posY			= m_scrollBarInset;
		//m_scrollBar->m_height		= m_height - (2 * m_scrollBarInset);
		m_scrollBar->m_visibleItems	= m_numVisibleItems;
		//m_scrollBar->Resize(); If we're not changing height, we dont need to call resize
		m_scrollBar->Update();
	}
}

void CMenuListBox::UpdateFontTexture()
{
	//Loop through every item
	CMenuListItem *curItem = m_pFirstItem;
	while(curItem)
	{
		//Loop through this item's text objects
		POSITION posText = curItem->m_textList.GetHeadPosition();
		while(posText != NULL)
		{
			//Get the current text object
			CMenuText *curText = (CMenuText*)curItem->m_textList.GetNext(posText);

			//Update the text mesh's texture select, if it exists
			if(curText->m_textMesh)
				curText->m_textMesh->List_textureSelect[0] = m_font->m_textureSelect;
		}

		//Goto the next item
		curItem = curItem->m_pNext;
	}
}

void CMenuListBox::Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice)
{
	*pClone = new CMenuListBox();
	Copy(*pClone, pD3DDevice);
}

void CMenuListBox::Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice)
{
	//Copy base properties
	CMenuVisual::Copy(pCopy, pD3DDevice);

	//Copy list box settings
	CMenuListBox *listBoxCopy = (CMenuListBox*)pCopy;

	//Visible Objects
	if(m_background)
		m_background->Clone((CMenuVisual**)&listBoxCopy->m_background, pD3DDevice);

	if(m_borderLeft)
	{
		m_borderLeft->Clone((CMenuVisual**)&listBoxCopy->m_borderLeft, pD3DDevice);
		m_borderTop->Clone((CMenuVisual**)&listBoxCopy->m_borderTop, pD3DDevice);
		m_borderRight->Clone((CMenuVisual**)&listBoxCopy->m_borderRight, pD3DDevice);
		m_borderBottom->Clone((CMenuVisual**)&listBoxCopy->m_borderBottom, pD3DDevice);
		m_cornerTopLeft->Clone((CMenuVisual**)&listBoxCopy->m_cornerTopLeft, pD3DDevice);
		m_cornerTopRight->Clone((CMenuVisual**)&listBoxCopy->m_cornerTopRight, pD3DDevice);
		m_cornerBottomLeft->Clone((CMenuVisual**)&listBoxCopy->m_cornerBottomLeft, pD3DDevice);
		m_cornerBottomRight->Clone((CMenuVisual**)&listBoxCopy->m_cornerBottomRight, pD3DDevice);
	}

	if(m_scrollBar)
		m_scrollBar->Clone((CMenuVisual**)&listBoxCopy->m_scrollBar, pD3DDevice);

	//Settable Parameters
	listBoxCopy->m_font						= m_font;
	listBoxCopy->m_normalRed				= m_normalRed;
	listBoxCopy->m_normalGreen				= m_normalGreen;
	listBoxCopy->m_normalBlue				= m_normalBlue;
	listBoxCopy->m_normalAlpha				= m_normalAlpha;
	listBoxCopy->m_selectedRed				= m_selectedRed;
	listBoxCopy->m_selectedGreen			= m_selectedGreen;
	listBoxCopy->m_selectedBlue				= m_selectedBlue;
	listBoxCopy->m_selectedAlpha			= m_selectedAlpha;
	listBoxCopy->m_backgroundInset			= m_backgroundInset;
	listBoxCopy->m_cornerOffsetX			= m_cornerOffsetX;
	listBoxCopy->m_cornerOffsetY			= m_cornerOffsetY;
	listBoxCopy->m_itemInset				= m_itemInset;
	listBoxCopy->m_numTextObjectsPerItem	= m_numTextObjectsPerItem;
	listBoxCopy->m_numColumns				= m_numColumns;
	listBoxCopy->m_columnSpacing			= m_columnSpacing;
	listBoxCopy->m_iconSpacing				= m_iconSpacing;
	listBoxCopy->m_extraSpacingBetweenText	= m_extraSpacingBetweenText;
	listBoxCopy->m_extraSpacingBetweenItems	= m_extraSpacingBetweenItems;
	listBoxCopy->m_scrollBarInset			= m_scrollBarInset;
	listBoxCopy->m_scrollBarAlwaysVisible	= m_scrollBarAlwaysVisible;
	listBoxCopy->m_dragEnabled				= m_dragEnabled;
	listBoxCopy->m_numItems					= m_numItems;

	//Copy list of items and set last item
	if(m_pFirstItem)
	{
		CMenuListItem *curItem = m_pFirstItem;
		CMenuListItem *curNewItem = listBoxCopy->m_pFirstItem = new CMenuListItem();
		while(1)
		{
			//Copy over the icon pointer and misc variables
			curNewItem->m_icon			= curItem->m_icon;
			curNewItem->m_miscInt1		= curItem->m_miscInt1;
			curNewItem->m_miscInt2		= curItem->m_miscInt2;
			curNewItem->m_miscInt3		= curItem->m_miscInt3;
			curNewItem->m_miscInt4		= curItem->m_miscInt4;
			curNewItem->m_miscFloat		= curItem->m_miscFloat;
			curNewItem->m_miscString	= curItem->m_miscString;

			//Copy the text list
			curItem->m_textList.Copy((CMenuVisualList*)&curNewItem->m_textList, pD3DDevice);

			//If we dont have a next, break from this loop
			if(!curItem->m_pNext)
				break;

			//Goto the next item
			curItem				= curItem->m_pNext;
			curNewItem->m_pNext	= new CMenuListItem();
			curNewItem			= curNewItem->m_pNext;
		}

		//Copy the last item we created
		listBoxCopy->m_pLastItem = curNewItem;
	}

	//Temperary Variables
	listBoxCopy->m_fontName = m_fontName;

	//Scripts

	listBoxCopy->m_selectionChangeScript = m_selectionChangeScript;
	listBoxCopy->m_mouseOverScript		 = m_mouseOverScript;
	listBoxCopy->m_mouseOutScript		 = m_mouseOutScript;
	listBoxCopy->m_dropScript			 = m_dropScript;
	listBoxCopy->m_updateScript			 = m_updateScript;
}

CMenuListBox::~CMenuListBox()
{
	DeleteMemory();
}

void CMenuListBox::DeleteMemory()
{
	//Delete the background
	if(m_background)
	{
		delete m_background;
		m_background = NULL;
	}

	//Delete the borders and corners
    SafeDelete(m_borderLeft);
	SafeDelete(m_borderTop);
	SafeDelete(m_borderRight);
	SafeDelete(m_borderBottom);
	SafeDelete(m_cornerTopLeft);
	SafeDelete(m_cornerTopRight);
	SafeDelete(m_cornerBottomLeft);
	SafeDelete(m_cornerBottomRight);

	SafeDelete(m_selectionChangeScript);
	SafeDelete(m_mouseOverScript);
	SafeDelete(m_mouseOutScript);
	SafeDelete(m_dropScript);
	SafeDelete(m_updateScript);
	SafeDelete(m_itemBackground);
	SafeDelete(m_scrollBar);
 
	//Delete our item list
	if(m_pFirstItem)
	{
		//Loop through and delete every item but the last one
		CMenuListItem *curItem = m_pFirstItem;
		while(curItem->m_pNext)
		{
			curItem = curItem->m_pNext;
			SafeDelete(curItem->m_pPrev);
		}

		//Delete the only item left
		SafeDelete(curItem);

		//Null out our list pointers
		m_pFirstItem		= NULL;
		m_pLastItem			= NULL;
		m_currentSelection	= NULL;
	}

	//TODO: Cloned items point to the same Script Objects, delete them somewhere?
}

void CMenuListBox::Serialize(CXMLFileIO &xml)
{
	CMenuVisual::Serialize(xml);

	if(xml.m_isStoring)
	{
		//Write out the list box properties
		xml.WriteAttribute("Version", 1);
		xml.WriteAttribute("Font", m_font->m_name.GetBuffer());
		xml.WriteAttribute("NormalRed", static_cast<int>(m_normalRed * 255));
		xml.WriteAttribute("NormalGreen", static_cast<int>(m_normalGreen * 255));
		xml.WriteAttribute("NormalBlue",static_cast<int>( m_normalBlue * 255));
		xml.WriteAttribute("NormalAlpha", static_cast<int>(m_normalAlpha * 255));
		xml.WriteAttribute("SelectedRed", static_cast<int>(m_selectedRed * 255));
		xml.WriteAttribute("SelectedGreen", static_cast<int>(m_selectedGreen * 255));
		xml.WriteAttribute("SelectedBlue", static_cast<int>(m_selectedBlue * 255));
		xml.WriteAttribute("SelectedAlpha", static_cast<int>(m_selectedAlpha * 255));
		xml.WriteAttribute("SpecialRed", static_cast<int>(m_specialRed * 255));
		xml.WriteAttribute("SpecialGreen", static_cast<int>(m_specialGreen * 255));
		xml.WriteAttribute("SpecialBlue", static_cast<int>(m_specialBlue * 255));
		xml.WriteAttribute("SpecialAlpha", static_cast<int>(m_specialAlpha * 255));
		xml.WriteAttribute("BackgroundInset", static_cast<int>(m_backgroundInset));
		xml.WriteAttribute("CornerOffsetX", static_cast<int>(m_cornerOffsetX));
		xml.WriteAttribute("CornerOffsetY", static_cast<int>(m_cornerOffsetY));
		xml.WriteAttribute("ItemInset", static_cast<int>(m_itemInset));
		xml.WriteAttribute("NumTextObjectsPerItem", m_numTextObjectsPerItem);
		xml.WriteAttribute("NumColumns", m_numColumns);
		xml.WriteAttribute("ColumnSpacing", static_cast<int>(m_columnSpacing));
		xml.WriteAttribute("IconSpacing", static_cast<int>(m_iconSpacing));
		xml.WriteAttribute("ExtraSpacingBetweenText", static_cast<int>(m_extraSpacingBetweenText));
		xml.WriteAttribute("ExtraSpacingBetweenItems", static_cast<int>(m_extraSpacingBetweenItems));
		xml.WriteAttribute("ScrollBarInset", static_cast<int>(m_scrollBarInset));
		xml.WriteAttribute("ScrollBarAlwaysVisible", m_scrollBarAlwaysVisible);
		xml.WriteAttribute("DragEnabled", m_dragEnabled);

		if(m_background)
		{
			xml.WriteOpeningTag("Background");
			m_background->Serialize(xml);
			xml.WriteClosingTag("Background");
		}

		if(m_borderLeft)
		{
			xml.WriteOpeningTag("BorderLeft");
			m_borderLeft->Serialize(xml);
			xml.WriteClosingTag("BorderLeft");

			xml.WriteOpeningTag("BorderTop");
			m_borderTop->Serialize(xml);
			xml.WriteClosingTag("BorderTop");

			xml.WriteOpeningTag("BorderRight");
			m_borderRight->Serialize(xml);
			xml.WriteClosingTag("BorderRight");

			xml.WriteOpeningTag("BorderBottom");
			m_borderBottom->Serialize(xml);
			xml.WriteClosingTag("BorderBottom");

			xml.WriteOpeningTag("CornerTopLeft");
			m_cornerTopLeft->Serialize(xml);
			xml.WriteClosingTag("CornerTopLeft");

			xml.WriteOpeningTag("CornerTopRight");
			m_cornerTopRight->Serialize(xml);
			xml.WriteClosingTag("CornerTopRight");

			xml.WriteOpeningTag("CornerBottomLeft");
			m_cornerBottomLeft->Serialize(xml);
			xml.WriteClosingTag("CornerBottomLeft");

			xml.WriteOpeningTag("CornerBottomRight");
			m_cornerBottomRight->Serialize(xml);
			xml.WriteClosingTag("CornerBottomRight");
		}

		if(m_scrollBar)
		{
			//Reset the item info
			m_scrollBar->m_numItems		= 1;
			m_scrollBar->m_visibleItems = 1;

			//Write it out
			xml.WriteOpeningTag("ScrollBar");
			m_scrollBar->Serialize(xml);
			xml.WriteClosingTag("ScrollBar");
		}

		//Write out the item background
		if(m_itemBackground)
		{
			xml.WriteOpeningTag("ItemBackground");
			m_itemBackground->Serialize(xml);
			xml.WriteClosingTag("ItemBackground");
		}
	
		//Write out the selection change script
		if(m_selectionChangeScript)
		if(!m_selectionChangeScript->m_scriptText.IsEmpty())
		{
			xml.WriteOpeningTag("SelectionChangeScript");
			xml.WriteMultilinedAttribute(m_selectionChangeScript->m_scriptText.GetBuffer());
			xml.WriteClosingTag("SelectionChangeScript");
		}

		//Write out the mouse over script
		if(m_mouseOverScript)
		if(!m_mouseOverScript->m_scriptText.IsEmpty())
		{
			xml.WriteOpeningTag("MouseOverScript");
			xml.WriteMultilinedAttribute(m_mouseOverScript->m_scriptText.GetBuffer());
			xml.WriteClosingTag("MouseOverScript");
		}

		//Write out the mouse out script
		if(m_mouseOutScript)
		if(!m_mouseOutScript->m_scriptText.IsEmpty())
		{
			xml.WriteOpeningTag("MouseOutScript");
			xml.WriteMultilinedAttribute(m_mouseOutScript->m_scriptText.GetBuffer());
			xml.WriteClosingTag("MouseOutScript");
		}

		//Write out the drop script
		if(m_dropScript)
		if(!m_dropScript->m_scriptText.IsEmpty())
		{
			xml.WriteOpeningTag("DropScript");
			xml.WriteMultilinedAttribute(m_dropScript->m_scriptText.GetBuffer());
			xml.WriteClosingTag("DropScript");
		}

		//Write out the update script
		if(m_updateScript)
		if(!m_updateScript->m_scriptText.IsEmpty())
		{
			xml.WriteOpeningTag("UpdateScript");
			xml.WriteMultilinedAttribute(m_updateScript->m_scriptText.GetBuffer());
			xml.WriteClosingTag("UpdateScript");
		}
	}
	else
	{
		char buffer[MAX_BUFFER];
		bool closing;
		int  version;

		//Read in the properties
		xml.ReadAttribute(version);
		xml.ReadAttribute(buffer, MAX_BUFFER); m_fontName = buffer;
		xml.ReadAttribute(m_normalRed); m_normalRed /= 255.0f;
		xml.ReadAttribute(m_normalGreen); m_normalGreen /= 255.0f;
		xml.ReadAttribute(m_normalBlue); m_normalBlue /= 255.0f;
		xml.ReadAttribute(m_normalAlpha); m_normalAlpha /= 255.0f;
		xml.ReadAttribute(m_selectedRed); m_selectedRed /= 255.0f;
		xml.ReadAttribute(m_selectedGreen); m_selectedGreen /= 255.0f;
		xml.ReadAttribute(m_selectedBlue); m_selectedBlue /= 255.0f;
		xml.ReadAttribute(m_selectedAlpha); m_selectedAlpha /= 255.0f;
		xml.ReadAttribute(m_specialRed); m_specialRed /= 255.0f;
		xml.ReadAttribute(m_specialGreen); m_specialGreen /= 255.0f;
		xml.ReadAttribute(m_specialBlue); m_specialBlue /= 255.0f;
		xml.ReadAttribute(m_specialAlpha); m_specialAlpha /= 255.0f;
		xml.ReadAttribute(m_backgroundInset);
		xml.ReadAttribute(m_cornerOffsetX);
		xml.ReadAttribute(m_cornerOffsetY);
		xml.ReadAttribute(m_itemInset);
		xml.ReadAttribute(m_numTextObjectsPerItem);
		xml.ReadAttribute(m_numColumns);
		xml.ReadAttribute(m_columnSpacing);
		xml.ReadAttribute(m_iconSpacing);
		xml.ReadAttribute(m_extraSpacingBetweenText);
		xml.ReadAttribute(m_extraSpacingBetweenItems);
		xml.ReadAttribute(m_scrollBarInset);
		xml.ReadAttribute(m_scrollBarAlwaysVisible);
		xml.ReadAttribute(m_dragEnabled);

		//Read <Background>, <BorderLeft>, <ScrollBar>, <ItemBackground>, <SomeScript>, or </ListBox>
		xml.ReadTag(buffer, MAX_BUFFER, closing);

		//If this is the background
		if(strcmp(buffer, "Background") == 0)
		{
			m_background = new CMenuImage();
			m_background->Serialize(xml);
			//The closing tag gets read by the object

			//Read in <BorderLeft>, <ScrollBar>, <ItemBackground>, <SomeScript>, or </ListBox>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		//If this is the left border
		if(strcmp(buffer, "BorderLeft") == 0)
		{
			m_borderLeft		= new CMenuImage();
			m_borderTop			= new CMenuImage();
			m_borderRight		= new CMenuImage();
			m_borderBottom		= new CMenuImage();
			m_cornerTopLeft		= new CMenuImage();
			m_cornerTopRight	= new CMenuImage();
			m_cornerBottomLeft	= new CMenuImage();
			m_cornerBottomRight	= new CMenuImage();

			m_borderLeft->Serialize(xml);
			//The closing buffer gets read by the object

			xml.ReadTag(buffer, MAX_BUFFER, closing);
			m_borderTop->Serialize(xml);
			//The closing buffer gets read by the object

			xml.ReadTag(buffer, MAX_BUFFER, closing);
			m_borderRight->Serialize(xml);
			//The closing buffer gets read by the object

			xml.ReadTag(buffer, MAX_BUFFER, closing);
			m_borderBottom->Serialize(xml);
			//The closing buffer gets read by the object

			xml.ReadTag(buffer, MAX_BUFFER, closing);
			m_cornerTopLeft->Serialize(xml);
			//The closing buffer gets read by the object

			xml.ReadTag(buffer, MAX_BUFFER, closing);
			m_cornerTopRight->Serialize(xml);
			//The closing buffer gets read by the object

			xml.ReadTag(buffer, MAX_BUFFER, closing);
			m_cornerBottomLeft->Serialize(xml);
			//The closing buffer gets read by the object

			xml.ReadTag(buffer, MAX_BUFFER, closing);
			m_cornerBottomRight->Serialize(xml);
			//The closing buffer gets read by the object

			//Read in <ScrollBar>, <ItemBackground>, <SomeScript>, or </ListBox>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		//If this is the scroll bar
		if(strcmp(buffer, "ScrollBar") == 0)
		{
			SafeDelete(m_scrollBar);
			m_scrollBar = new CMenuScrollBar();
			m_scrollBar->Serialize(xml);
			//The closing tag gets read by the object

			//Read in <ItemBackground>, <SomeScript>, or </ListBox>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		//If this is the background
		if(strcmp(buffer, "ItemBackground") == 0)
		{
			SafeDelete(m_itemBackground);
			m_itemBackground = new CMenuImage();
			m_itemBackground->Serialize(xml);
			//The closing tag gets read by the object

			//Read in <SomeScript> or </ListBox>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		//If this is the selection change script
		if(strcmp(buffer, "SelectionChangeScript") == 0)
		{
			//We've got a load script, create a new script instance
			SafeDelete(m_selectionChangeScript);
			m_selectionChangeScript = new CMenuScript();

			while(1)
			{
				//Read next script line or </SelectionChangeScript>
				xml.ReadLineOfAttribute(buffer, MAX_BUFFER);

				//If it is </LoadScript>, break
				if(buffer[0] == '<')
					break;

				//Add the line to the script
				m_selectionChangeScript->AddLine(buffer);
			}

			//Read in <SomeScript> or </ListBox>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		//If this is the mouse over script
		if(strcmp(buffer, "MouseOverScript") == 0)
		{
			//We've got a load script, create a new script instance
			SafeDelete(m_mouseOverScript);
			m_mouseOverScript = new CMenuScript();

			while(1)
			{
				//Read next script line or </MouseOverScript>
				xml.ReadLineOfAttribute(buffer, MAX_BUFFER);

				//If it is </LoadScript>, break
				if(buffer[0] == '<')
					break;

				//Add the line to the script
				m_mouseOverScript->AddLine(buffer);
			}

			//Read in <SomeScript> or </ListBox>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		//If this is the mouse out script
		if(strcmp(buffer, "MouseOutScript") == 0)
		{
			//We've got a load script, create a new script instance
			SafeDelete(m_mouseOutScript);
			m_mouseOutScript = new CMenuScript();

			while(1)
			{
				//Read next script line or </MouseOutScript>
				xml.ReadLineOfAttribute(buffer, MAX_BUFFER);

				//If it is </LoadScript>, break
				if(buffer[0] == '<')
					break;

				//Add the line to the script
				m_mouseOutScript->AddLine(buffer);
			}

			//Read in </ListBox>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		//If this is the drop script
		if(strcmp(buffer, "DropScript") == 0)
		{
			//We've got a load script, create a new script instance
			SafeDelete(m_dropScript);
			m_dropScript = new CMenuScript();

			while(1)
			{
				//Read next script line or </DropScript>
				xml.ReadLineOfAttribute(buffer, MAX_BUFFER);

				//If it is </LoadScript>, break
				if(buffer[0] == '<')
					break;

				//Add the line to the script
				m_dropScript->AddLine(buffer);
			}

			//Read in <BorderLeft>, <ScrollBar>, <SomeScript>, or </ListBox>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		//If this is the drop script
		//If the UpdateScript is present
		if(strcmp(buffer, "UpdateScript") == 0)
		{
			//We've got a update script, create a new script instance
			SafeDelete(m_updateScript);
			m_updateScript = new CMenuScript();

			while(1)
			{
				//Read next script line or </UpdateScript>
				xml.ReadLineOfAttribute(buffer, MAX_BUFFER);

				//If it is </UpdateScript>, break
				if(buffer[0] == '<')
					break;

				//Add the line to the script
				m_updateScript->AddLine(buffer);
			}

			//Read in </Image>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}
	}
}
