#include "stdafx.h"
#include "CMenuInventory.h"
#include "GlobalStatusVariables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern GlobalStatusVariables *globalStatus;

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventoryItem
// Function:	Ctor/Dtor
///////////////////////////////////////////////////////////////////////////////

CMenuInventoryItem::CMenuInventoryItem()
{
	// INIT data members
	this->m_pIcon = NULL;
}

CMenuInventoryItem::CMenuInventoryItem(ITEMDATA_STRUCT_EX item)
{
	// INIT data members
	this->m_data = item;
	this->m_pIcon = NULL;

	// DETERMINE the actual item by its global ID
	CItemObj *pItem = globalStatus->m_itemManager->GetItemByID(item.m_GLID);

	// RETRIEVE the item icon
	if (pItem)
	{
		// SET the received flag for this item
		//	NOTE: This is referenced in CMenuScript::ExecuteScript(),
		//	for the MENU_SCRIPTFUNC_UPDATELOADINGITEMS script.
		this->m_data.m_received = true;
		if (pItem->m_visualRef != -1)
		{
			// LOAD the icon for this item
			CMenuImage *pIcon = dynamic_cast<CMenuImage*>(globalStatus->m_menuIconDatabase->GetVisualByID(pItem->m_visualRef, true));
			if (pIcon)
				pIcon->Clone((CMenuVisual**)&this->m_pIcon, globalStatus->m_3dDevice);
		}
	}

	// IF we still don't have an icon, SET a default icon
	if (!this->m_pIcon)
	{
		POSITION pos = globalStatus->m_menuIconDatabase->GetHeadPosition();
		ASSERT(pos); 
		CMenuImage *pDefaultIcon = dynamic_cast<CMenuImage*>(globalStatus->m_menuIconDatabase->GetNext(pos));
		ASSERT(pDefaultIcon);
		SafeDelete(this->m_pIcon);
		pDefaultIcon->Clone((CMenuVisual**)&this->m_pIcon, globalStatus->m_3dDevice);
	}

	// UPDATE the texture
	this->m_pIcon->m_visualID = globalStatus->m_runtimeMenuDatabase->GetFreeID(this->m_pIcon->m_visualID);
	this->m_pIcon->m_blendMode = 1; // ALPHA BLEND
	this->m_pIcon->m_textureSelect = globalStatus->m_textureDataBase->AddAssignTempTexture(
		globalStatus->m_3dDevice, 
		this->m_pIcon->m_textureFile, 
		0);
	this->m_pIcon->UpdateTexture(this->m_pIcon->m_textureSelect);
}

CMenuInventoryItem::~CMenuInventoryItem()
{
	SafeDelete(this->m_pIcon);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventoryItem
// Function:	Clone/Copy
///////////////////////////////////////////////////////////////////////////////

void
CMenuInventoryItem::Clone(CMenuInventoryItem **pClone, LPDIRECT3DDEVICE9 pDevice)
{
	SafeDelete(*pClone);
	*pClone = new CMenuInventoryItem();
	this->Copy(*pClone, pDevice);
}

void
CMenuInventoryItem::Copy(CMenuInventoryItem *pCopy, LPDIRECT3DDEVICE9 pDevice)
{
	pCopy->m_data = this->m_data;
	this->m_pIcon->Clone((CMenuVisual**)&pCopy->m_pIcon, pDevice);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	Ctor/Dtor
///////////////////////////////////////////////////////////////////////////////

CMenuInventory::CMenuInventory():
	m_iVersion(2)
{
	this->m_bActive = true;
	this->m_iHoverItemIndex = -1;
	this->m_iSelectedItemIndex = -1;
	this->m_iNumRows = 0;
	this->m_iNumColumns = 0;
	this->m_iRowSpacing = 0;
	this->m_iColumnSpacing = 0;
	this->m_pHoverItemRef = NULL;
	this->m_pSelectedItemRef = NULL;
	this->m_clickScript = NULL;
	this->m_rightClickScript = NULL;
	this->m_mouseOverScript = NULL;
	this->m_mouseOutScript = NULL;
	this->m_dropScript = NULL;
	this->m_updateScript = NULL;
	this->m_childList = new CMenuVisualList();
	this->m_pPrototypeImage = new CMenuImage();
	this->m_pPrototypeImage->Init(globalStatus->m_3dDevice);
	this->m_pPrototypeImage->m_texturePosX = 0;
	this->m_pPrototypeImage->m_texturePosY = 0;
	this->m_pPrototypeImage->m_textureWidth = 0;
	this->m_pPrototypeImage->m_textureHeight = 0;
	this->m_items.clear();
}

CMenuInventory::~CMenuInventory()
{
	this->m_pHoverItemRef = NULL;
	this->m_pSelectedItemRef = NULL;
	SafeDelete(this->m_clickScript);
	SafeDelete(this->m_rightClickScript);
	SafeDelete(this->m_mouseOverScript);
	SafeDelete(this->m_mouseOutScript);
	SafeDelete(this->m_dropScript);
	SafeDelete(this->m_updateScript);
	SafeDelete(this->m_pPrototypeImage);
	this->ClearItems();
	this->ClearGrid();
	SafeDelete(this->m_childList);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	Clone/Copy
// Description:	Performs a deep copy of this object.
///////////////////////////////////////////////////////////////////////////////

void
CMenuInventory::Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pDevice)
{
	*pClone = new CMenuInventory();
	this->Copy(*pClone, pDevice);
}

void
CMenuInventory::Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pDevice)
{
	__super::Copy(pCopy, pDevice);
	CMenuInventory *pInventoryCopy = static_cast<CMenuInventory*>(pCopy);

	// CREATE a new visual ID for the copy
	pInventoryCopy->m_visualID = globalStatus->m_menuDatabase->GetFreeID(this->m_visualID);

	// COPY menu properties
	pInventoryCopy->m_iNumRows = this->m_iNumRows;
	pInventoryCopy->m_iNumColumns = this->m_iNumColumns;
	pInventoryCopy->m_iRowSpacing = this->m_iRowSpacing;
	pInventoryCopy->m_iColumnSpacing = this->m_iColumnSpacing;

	// COPY all the inventory items
	pInventoryCopy->m_items.assign(this->m_items.begin(), this->m_items.end());

	// COPY all children visuals
	for (POSITION pos = this->m_childList->GetHeadPosition(); pos != NULL;)
	{
		CMenuVisual *pVisual = dynamic_cast<CMenuVisual*>(this->m_childList->GetNext(pos));
		if (!pVisual) continue;
		pInventoryCopy->m_childList->AddTail(pVisual);
	}

	// COPY prototype image
	this->m_pPrototypeImage->Clone((CMenuVisual**)&pInventoryCopy->m_pPrototypeImage, pDevice);

	// COPY scripts
	if (this->m_clickScript) this->m_clickScript->Clone(&pInventoryCopy->m_clickScript);
	if (this->m_rightClickScript) this->m_rightClickScript->Clone(&pInventoryCopy->m_rightClickScript);
	if (this->m_dropScript) this->m_dropScript->Clone(&pInventoryCopy->m_dropScript);
	if (this->m_mouseOutScript) this->m_mouseOutScript->Clone(&pInventoryCopy->m_mouseOutScript);
	if (this->m_mouseOverScript) this->m_mouseOverScript->Clone(&pInventoryCopy->m_mouseOverScript);
	if (this->m_updateScript) this->m_updateScript->Clone(&pInventoryCopy->m_updateScript);

	// REFRESH the item grid for the copy
	pInventoryCopy->CreateGrid();

	// REFRESH the inventory copy's display
	pInventoryCopy->RefreshItems();
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	Count()
// Description:	Get the number of items in the inventory.
///////////////////////////////////////////////////////////////////////////////

int
CMenuInventory::Count()
{
	return this->m_items.size();
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	CreateGrid()
// Description:	Creates a grid of item placeholder images based on values
//	specified by the user in the engine. Number of rows/columns and row/column
//	spacing are the primary variables in use here.
///////////////////////////////////////////////////////////////////////////////

void
CMenuInventory::CreateGrid()
{
	// CLEAR any pre-existing clones
	this->ClearGrid();

	// CREATE the clones to form the inventory item grid
	char buffer[255];
	const char* strClonePrefix = "InventoryItem_";
	CMenuVisual *pItem = NULL;
	for (int i=0; i<this->m_iNumRows; i++)
	{
		for (int j=0; j<this->m_iNumColumns; j++)
		{
			// CREATE and INIT new item placeholder
			pItem = NULL;
			this->m_pPrototypeImage->Clone(&pItem, globalStatus->m_3dDevice);
			pItem->m_visualID = globalStatus->m_menuDatabase->GetFreeID(-1);
			pItem->m_name = strClonePrefix;
			itoa(pItem->m_visualID, buffer, 10);
			pItem->m_name.Append(buffer);
			pItem->m_posX += (this->m_iColumnSpacing * j) + (pItem->m_width * j);
			pItem->m_posY -= (this->m_iRowSpacing * i) + (pItem->m_height * i);
			this->m_backgrounds.push_back((CMenuImage*)pItem);
			((CMenuImage*)pItem)->UpdateTexture(((CMenuImage*)pItem)->m_textureSelect);
			((CMenuImage*)pItem)->UpdateTextureCoords();
		}
	}

	// UPDATE this control's dimensions
	this->m_width = ((this->m_iNumColumns - 1) * this->m_iColumnSpacing) + (this->m_iNumColumns * this->m_pPrototypeImage->m_width);
	this->m_height = ((this->m_iNumRows - 1) * this->m_iRowSpacing) + (this->m_iNumRows * this->m_pPrototypeImage->m_height);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	CreateGrid()
// Description:	Safely deletes the grid of item placeholder images.
///////////////////////////////////////////////////////////////////////////////

void
CMenuInventory::ClearGrid()
{
	for (int i=0; i<this->m_backgrounds.size(); i++)
	{
		CMenuImage *pImage = this->m_backgrounds[i];
		SafeDelete(pImage);
	}
	this->m_backgrounds.clear();
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	UpdateTexture(int)
// Description:	Update and refresh the textures of all images contained in this
//	menu control. 
///////////////////////////////////////////////////////////////////////////////

void
CMenuInventory::UpdateTexture(int iNewTextureIndex)
{
	if (this->m_pPrototypeImage)
		this->m_pPrototypeImage->m_textureSelect = iNewTextureIndex;

	// DELETE the image mesh for a visual refresh
	SafeDelete(this->m_pPrototypeImage->m_imageMesh);
	this->m_pPrototypeImage->Init(globalStatus->m_3dDevice);
	this->m_pPrototypeImage->m_imageMesh->List_textureSelect[0] = iNewTextureIndex;

	// LOAD the image textures for each image placeholder
	for (int i=0; i<this->m_backgrounds.size(); i++)
	{
		CMenuImage *pImage = this->m_backgrounds[i];
		if (!pImage) continue;
		SafeDelete(pImage->m_imageMesh);
		pImage->Init(globalStatus->m_3dDevice);
		pImage->m_imageMesh->List_textureSelect[0] = iNewTextureIndex;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	IsMouseOver(CInput*, float, float)
// Description:	Check to see if the game cursor is in the bounds of this menu.
///////////////////////////////////////////////////////////////////////////////

bool
CMenuInventory::IsMouseOver(CInput *pInput, float fParentX, float fParentY)
{
	fParentX += this->m_posX;
	fParentY += this->m_posY;

	if ((pInput->m_mouseAbsPosX > fParentX)
	&& (pInput->m_mouseAbsPosX < (fParentX + this->m_width))
	&& (pInput->m_mouseAbsPosY < fParentY) 
	&& (pInput->m_mouseAbsPosY > (fParentY - this->m_height)))
	{
		return true;
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	HandleInput(int)
// Description:	Handle input events as appropriate. Returns a boolean indicating
//	if events have been handled by this control or not.
///////////////////////////////////////////////////////////////////////////////

bool
CMenuInventory::HandleInput(CInput *pInput,
							float parentX,
							float parentY,
							DWORD curTime,
							LPDIRECT3DDEVICE9 pD3DDevice)
{
	// TEST each of the inventory icons for mouse over
	/*CMenuInventoryItem *pItem = NULL;
	for (int i=0; i<this->m_items.size(); i++)
	{
		pItem = this->m_items[i];
		if (pItem->m_pIcon->IsMouseOver(pInput, parentX, parentY))
		{
			this->m_pHoverItemRef = this->m_items[i];
			this->m_iHoverItemIndex = i;
			break;
		}
		pItem = NULL;
	}

	// UNSET hover item status, if necessary
	if (!pItem)
	{
		this->m_pHoverItemRef = NULL;
		this->m_iHoverItemIndex = -1;
	}*/

	// FIND the mouse over item, IF any
	int iIndex = 0;
	ITEMLIST::iterator iter;
	CMenuInventoryItem *pItem = NULL;
	for (iter = this->m_items.begin(); iter != this->m_items.end(); iter++)
	{
		pItem = *iter;
		pItem->m_pIcon->HandleInput(pInput, 
			parentX + this->m_posX, 
			parentY + this->m_posY, 
			curTime, 
			pD3DDevice);
		if (pItem->m_pIcon->m_bIsMouseOver)
		{
			this->m_pHoverItemRef = pItem;
			this->m_iHoverItemIndex = iIndex;
			break;
		}
		++iIndex;
		pItem = NULL;
	}

	// UNSET hover item status, if necessary
	if (!pItem)
	{
		this->m_pHoverItemRef = NULL;
		this->m_iHoverItemIndex = -1;
	}

	// HANDLE input for each of the item placeholder background images
	for (int i=0; i<this->m_backgrounds.size(); i++)
	{
		CMenuImage *pBgImage = this->m_backgrounds[i];
		if (!pBgImage) continue;
		pBgImage->HandleInput(pInput, parentX + this->m_posX, parentY + this->m_posY, curTime, pD3DDevice);
	}

	// EXECUTE the UPDATE script
	if(this->m_updateScript)
	{
		if (this->m_updateScript->m_pFirstLine 
		&& (this->m_updateScript->m_pFirstLine->m_function == -1)) 
			SafeDelete(this->m_updateScript);
		else
			this->m_updateScript->ExecuteScript();
	}

	// HANDLE mouse events
	int iPosX = parentX + this->m_pPrototypeImage->m_posX;
	int iPosY = parentY + this->m_pPrototypeImage->m_posY + this->m_pPrototypeImage->m_height;
	if (this->IsMouseOver(pInput, iPosX, iPosY))
	{
		// MOUSE PRESSED
		if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustPressed())
		{
			this->m_mouseDown = true;

			// EXECUTE the click script
			if (this->m_clickScript)
				this->m_clickScript->ExecuteScript();

			// SET/UNSET the selected item status
			if (this->m_iHoverItemIndex != -1)
			{
				this->m_iSelectedItemIndex = this->m_iHoverItemIndex;
				this->m_pSelectedItemRef = this->m_pHoverItemRef;
			}
			else
			{
				this->m_iSelectedItemIndex = -1;
				this->m_pSelectedItemRef = NULL;
			}
			return true;
		}
		// RIGHT MOUSE RELEASED
		else if(pInput->m_mouseButtons[MOUSE_RIGHTBUTTON].JustPressed())
		{
			this->m_mouseDown = true;

			// Set/UNSET item status
			// SET/UNSET the selected item status
			if (this->m_iHoverItemIndex != -1)
			{
				this->m_iSelectedItemIndex = this->m_iHoverItemIndex;
				this->m_pSelectedItemRef = this->m_pHoverItemRef;
			}
			else
			{
				this->m_iSelectedItemIndex = -1;
				this->m_pSelectedItemRef = NULL;
			}
			
			if(this->m_rightClickScript)
				this->m_rightClickScript->ExecuteScript();
			
			return true;
		}
		// MOUSE RELEASED
		else if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustReleased())
		{
			this->m_mouseDown = false;
			if (globalStatus->m_draggingItem)
			{
				if (this->m_dropScript)
					this->m_dropScript->ExecuteScript();
				return true;
			}
		}
		// MOUSE OVER
		else
		{
			this->m_mouseOver = true;
			if (this->m_mouseOverScript)
				this->m_mouseOverScript->ExecuteScript();
		}
	}
	else
	{
		// MOUSE OUT
		if (this->m_mouseOver)
		{
			this->m_mouseOver = false;
			if (this->m_mouseOutScript)
				this->m_mouseOutScript->ExecuteScript();
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	Render(LPDIRECT3DDEVICE9, CStateManagementObj, CTextureDBObjList, float, float)
// Description:	Render all the graphical elements of this control to the screen.
///////////////////////////////////////////////////////////////////////////////

void
CMenuInventory::Render(LPDIRECT3DDEVICE9 pD3DDevice,
					   CStateManagementObj *stateManager,
					   CTextureDBObjList *textureDatabase,
					   float parentX,
					   float parentY)
{
	//parentX and parentY come in as half the resolution, so the position is:
	float left	 = parentX + m_posX;
	float right  = left + m_width;
	float bottom = parentY + m_posY;
	float top	 = bottom + m_height;

	// RENDER EITHER the prototype image OR the grid clones
	if (this->m_backgrounds.size() > 0)
	{
		for (int i=0; i<this->m_backgrounds.size(); i++)
		{
			CMenuImage *pImage = this->m_backgrounds[i];
			if (!pImage) continue;
			pImage->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);
		}
	}
	else if (this->m_pPrototypeImage)
		this->m_pPrototypeImage->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	// RENDER the icons for all inventory items
	for (int i=0; i<this->m_items.size(); i++)
	{
		CMenuInventoryItem *pItem = this->m_items[i];
		pItem->m_pIcon->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);
	}

	// RENDER all the children visuals
	POSITION pos = NULL;
	for (pos = this->m_childList->GetHeadPosition(); pos != NULL;)
	{
		CMenuVisual *pVisual = dynamic_cast<CMenuVisual*>(this->m_childList->GetNext(pos));
		if (!pVisual) continue;
		pVisual->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	AddItem(ITEMDATA_STRUCT_EX)
// Description:	Add a local copy of an item to this player's inventory based 
//	on the network structure (ITEMDATA_STRUCT_EX) used to transfer inventory data 
//	from the server to the client.
///////////////////////////////////////////////////////////////////////////////

bool
CMenuInventory::AddItem(ITEMDATA_STRUCT_EX item)
{
	// ERROR checking: Are we at the inventory's capacity?
	if ((this->m_items.size()) == (this->m_iNumRows * this->m_iNumColumns))
		return false;

	// CREATE the new inventory item
	CMenuInventoryItem *pInventoryItem = new CMenuInventoryItem(item);
	
	// DETERMINE the actual item by its global ID
	CItemObj *pItem = globalStatus->m_itemManager->GetItemByID(item.m_GLID);

	// IF this item is stackable AND already in the inventory, 
	//	just increase the quantity of the already present item...
	if (pItem)
	{
		if (pItem->m_stackable)
		{
			for (int i=0; i<this->m_items.size(); i++)
			{
				CMenuInventoryItem *pTempItem = this->m_items[i];
				if (pTempItem && (pTempItem->m_data.m_GLID == item.m_GLID))
				{
					pTempItem->m_data.m_quantity += item.m_quantity;
					return true;
				}
			}
		}
	}

	// SET the icon image position
	int iCurrentIndex = this->m_items.size();
	int iRow = iCurrentIndex / this->m_iNumColumns;
	int iColumn = (iCurrentIndex % this->m_iNumColumns);
	int iOffsetX = (this->m_pPrototypeImage->m_width - pInventoryItem->m_pIcon->m_width) / 2;
	int iOffsetY = (this->m_pPrototypeImage->m_height - pInventoryItem->m_pIcon->m_height) / 2;
	pInventoryItem->m_pIcon->m_posX = this->m_pPrototypeImage->m_posX 
		+ ((iColumn * this->m_pPrototypeImage->m_width) + (iColumn * this->m_iColumnSpacing))
		+ iOffsetX;
	pInventoryItem->m_pIcon->m_posY = this->m_pPrototypeImage->m_posY 
		- ((iRow * this->m_pPrototypeImage->m_height) + (iRow * this->m_iRowSpacing))
		+ iOffsetY;

	// UPDATE the texture
	pInventoryItem->m_pIcon->m_blendMode = 1; // ALPHA BLEND
	pInventoryItem->m_pIcon->m_textureSelect = globalStatus->m_textureDataBase->AddAssignTempTexture(
		globalStatus->m_3dDevice, 
		pInventoryItem->m_pIcon->m_textureFile, 
		0);
	pInventoryItem->m_pIcon->UpdateTexture(pInventoryItem->m_pIcon->m_textureSelect);

	// STORE the new inventory item
	this->m_items.push_back(pInventoryItem);

	// REFRESH the visual display of all items
	this->RefreshItems();
	return true;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	RemoveItem(int, int)
// Description:	Removed the specified amount of an item from the player's
//	inventory. If all copies of an item are removed, then the item is completely
//	dumped from the player's inventory.
///////////////////////////////////////////////////////////////////////////////

bool
CMenuInventory::RemoveItem(int iGLID, int iQuantity)
{
	// IF the item to remove is already selected...
	if (this->m_pSelectedItemRef
	&& this->m_pSelectedItemRef->m_data.m_GLID == iGLID)
	{
		this->m_pSelectedItemRef->m_data.m_quantity -= iQuantity;
		if (this->m_pSelectedItemRef->m_data.m_quantity <= 0)
		{
			this->m_items.erase(this->m_items.begin() + this->m_iSelectedItemIndex);
			SafeDelete(this->m_pSelectedItemRef);
		}

		// REFRESH the visual display of all items
		this->ClearSelectedItem();
		this->RefreshItems();
		return true;
	}

	// ELSE, we have to search for the target to remove!
	std::vector<CMenuInventoryItem*>::iterator iter;
	for (iter = this->m_items.begin(); iter != this->m_items.end(); iter++)
	{
		CMenuInventoryItem *pTemp = *iter;
		if (pTemp->m_data.m_GLID == iGLID)
		{
			pTemp->m_data.m_quantity -= iQuantity;
			if (pTemp->m_data.m_quantity <= 0)
			{
				this->m_items.erase(iter);
				SafeDelete(pTemp);

				// REFRESH the visual display of all items
				this->ClearSelectedItem();
				this->RefreshItems();
			}
			return true;
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	ClearItems()
// Description:	Delete all the local copies of any items in this player's inventory.
///////////////////////////////////////////////////////////////////////////////

void
CMenuInventory::ClearItems()
{
	for (int i=0; i<this->m_items.size(); i++)
	{
		CMenuInventoryItem *pItem = this->m_items[i];
		SafeDelete(pItem);
	}
	this->m_items.clear();
	this->m_pHoverItemRef = NULL;
	this->m_pSelectedItemRef = NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	RefreshItem(ITEMDATA_STRUCT_EX)
// Description:	Refresh the visual display of all item icons.
///////////////////////////////////////////////////////////////////////////////

bool
CMenuInventory::RefreshItem(ITEMDATA_STRUCT_EX item)
{
	ITEMLIST::iterator iter;
	for (iter = this->m_items.begin(); iter != this->m_items.end(); iter++)
	{
		if (item == (*iter)->m_data)
		{
			// DETERMINE the actual item by its global ID
			CItemObj *pItem = globalStatus->m_itemManager->GetItemByID(item.m_GLID);
			if (!pItem) return false;
			if (pItem->m_visualRef != -1)
			{
				// LOAD the icon for this item
				CMenuImage *pIcon = dynamic_cast<CMenuImage*>(globalStatus->m_menuIconDatabase->GetVisualByID(pItem->m_visualRef, false));
				if (pIcon)
					pIcon->Clone((CMenuVisual**)&(*iter)->m_pIcon, globalStatus->m_3dDevice);
			}
			return false;
		}
	}
	return false;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	RefreshItems()
// Description:	Refresh the visual display of all item icons.
///////////////////////////////////////////////////////////////////////////////

void
CMenuInventory::RefreshItems()
{
	// REMOVE all Quickbar OR Equipped selection indicators
	POSITION p1 = NULL, p2 = NULL;
	for (p1 = p2 = this->m_childList->GetHeadPosition(); (p2 = p1) != NULL;)
	{
		CMenuImage *pImage = dynamic_cast<CMenuImage*>(this->m_childList->GetNext(p1));
		if (!pImage) continue;
		CString strName = pImage->m_name;
		if (strName.MakeLower().Find("indicator_") != -1)
		{
			this->m_childList->RemoveAt(p2);
			SafeDelete(pImage);
		}
	}

	for (int i=0; i<this->m_items.size(); i++)
	{
		CMenuInventoryItem *pInventoryItem = this->m_items[i];

		// SET the icon image position
		int iRow = i / this->m_iNumColumns;
		int iColumn = (i % this->m_iNumColumns);
		int iOffsetX = (this->m_pPrototypeImage->m_width - pInventoryItem->m_pIcon->m_width) / 2;
		int iOffsetY = (this->m_pPrototypeImage->m_height - pInventoryItem->m_pIcon->m_height) / 2;
		pInventoryItem->m_pIcon->m_posX = this->m_pPrototypeImage->m_posX 
			+ ((iColumn * this->m_pPrototypeImage->m_width) + (iColumn * this->m_iColumnSpacing))
			+ iOffsetX;
		pInventoryItem->m_pIcon->m_posY = this->m_pPrototypeImage->m_posY 
			- ((iRow * this->m_pPrototypeImage->m_height) + (iRow * this->m_iRowSpacing))
			+ iOffsetY;

		// UPDATE the texture
		pInventoryItem->m_pIcon->m_blendMode = 1; // ALPHA BLEND
		pInventoryItem->m_pIcon->m_textureSelect = globalStatus->m_textureDataBase->AddAssignTempTexture(
			globalStatus->m_3dDevice, 
			pInventoryItem->m_pIcon->m_textureFile, 
			0);
		pInventoryItem->m_pIcon->UpdateTexture(pInventoryItem->m_pIcon->m_textureSelect);

		// IF the item is equipped, SET the equip selection indicator
		if (pInventoryItem->m_data.m_armed)
			this->SetSelectionIndicator(pInventoryItem, CMenuInventory::IST_EQUIPPED, true);

		// IF this item is in the Quickbar, SET the Quickbar selection indicator
		if (globalStatus->m_multiplayerObject->IsInQuickbars(pInventoryItem))
			this->SetSelectionIndicator(pInventoryItem, CMenuInventory::IST_QUICKBAR, true);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	ClearSelectedItem()
// Description:	Clear the currently selected item, if it exists.
///////////////////////////////////////////////////////////////////////////////

void
CMenuInventory::ClearSelectedItem()
{
	this->m_iSelectedItemIndex = -1;
	this->m_pSelectedItemRef = NULL;
	globalStatus->OnInventoryItemSelected((MENU_IDS)this->m_visualID);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	ClearHoverItem()
// Description:	Clear the currently hovered item, if it exists.
///////////////////////////////////////////////////////////////////////////////

void
CMenuInventory::ClearHoverItem()
{
	this->m_iHoverItemIndex = -1;
	this->m_pHoverItemRef = NULL;
	globalStatus->OnInventoryItemSelected((MENU_IDS)this->m_visualID);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	WriteScript(CXMLFileIO, char*, CMenuScript**)
// Description:	Utility function to handle writing a script to a MOD file.
///////////////////////////////////////////////////////////////////////////////

void	
CMenuInventory::WriteScript(CXMLFileIO &xml, char *name, CMenuScript **script)
{
	if ((*script)
	&& (!(*script)->m_scriptText.IsEmpty()))
	{
		xml.WriteOpeningTag(name);
		xml.WriteMultilinedAttribute((*script)->m_scriptText.GetBuffer());
		xml.WriteClosingTag(name);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	ReadScript(CXMLFileIO, char*, CMenuScript**, char*, bool)
// Description:	Utility function to handle reading a script from a MOD file.
///////////////////////////////////////////////////////////////////////////////

void
CMenuInventory::ReadScript(CXMLFileIO &xml,
						   char *name,
						   CMenuScript **script,
						   char* buffer,
						   bool &closing)
{
	if (closing)
		return;

	// PROCESS script
	if(strcmp(buffer, name) == 0)
	{
		SafeDelete(*script);
		*script = new CMenuScript();

		// LOOP through all scripts line-by-line
		//	and ADD them to this particular menu event
		while(1)
		{
			xml.ReadLineOfAttribute(buffer, MAX_BUFFER);
			if(buffer[0] == '<') break;
			(*script)->AddLine(buffer);
		}

		// READ in the next tag
		xml.ReadTag(buffer, MAX_BUFFER, closing);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	ClearItems()
// Description:	CObject overload for I/O serialization of this object.
///////////////////////////////////////////////////////////////////////////////

void
CMenuInventory::Serialize(CXMLFileIO &xml)
{
	// NOTE: Don't serialize the child list!
	__super::Serialize(xml);
	
	if(xml.m_isStoring)
	{
		// WRITE properties
		xml.WriteAttribute("Version", this->m_iVersion);
		xml.WriteAttribute("NumRows", this->m_iNumRows);
		xml.WriteAttribute("NumColumns", this->m_iNumColumns);
		xml.WriteAttribute("RowSpacing", this->m_iRowSpacing);
		xml.WriteAttribute("ColumnSpacing", this->m_iColumnSpacing);

		// WRITE image prototype
		if (this->m_iVersion >= 2)
		{
			xml.WriteOpeningTag("PrototypeImage");
			this->m_pPrototypeImage->Serialize(xml);
			xml.WriteClosingTag("PrototypeImage");
		}

		// WRITE scripts
		this->WriteScript(xml, "ClickScript", &this->m_clickScript);
		this->WriteScript(xml, "MouseOverScript", &this->m_mouseOverScript);
		this->WriteScript(xml, "MouseOutScript", &this->m_mouseOutScript);
		this->WriteScript(xml, "DropScript", &this->m_dropScript);
		this->WriteScript(xml, "UpdateScript", &this->m_updateScript);
		this->WriteScript(xml, "RightClickScript", &this->m_rightClickScript);
	}
	else
	{
		// SETUP for reading in the menu inventory object
		int  version;
		char buffer[MAX_BUFFER];
		bool closing;

		// READ properties
		xml.ReadAttribute(version);
		xml.ReadAttribute(this->m_iNumRows);
		xml.ReadAttribute(this->m_iNumColumns);
		xml.ReadAttribute(this->m_iRowSpacing);
		xml.ReadAttribute(this->m_iColumnSpacing);

		// READ image prototype
		if (version >= 2)
		{
			xml.ReadTag(buffer, MAX_BUFFER, closing);
			this->m_pPrototypeImage->Serialize(xml);		
			this->m_pPrototypeImage->UpdateTextureCoords();
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}		

		// READ scripts
		this->ReadScript(xml, "ClickScript", &this->m_clickScript, buffer, closing);
		this->ReadScript(xml, "MouseOverScript", &this->m_mouseOverScript, buffer, closing);
		this->ReadScript(xml, "MouseOutScript", &this->m_mouseOutScript, buffer, closing);
		this->ReadScript(xml, "DropScript", &this->m_dropScript, buffer, closing);
		this->ReadScript(xml, "UpdateScript", &this->m_updateScript, buffer, closing);
		this->ReadScript(xml, "RightClickScript", &this->m_rightClickScript, buffer, closing);

		// CREATE the image grid on serialization read
		this->CreateGrid();
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	GetInventoryItemByIndex(int)
// Description:	Get an inventory item by its index in the inventory.
///////////////////////////////////////////////////////////////////////////////

CMenuInventoryItem**
CMenuInventory::GetInventoryItemByIndex(int iIndex)
{
	if ((iIndex < 0) || (iIndex >= this->m_items.size()))
		return NULL;
	return &this->m_items[iIndex];
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	SetSelectionIndicator(CMenuInventoryItem*, INVENTORY_SELECTION_TYPE, bool)
// Description:	Set the position and visibility of the specified selection
//	indicator menu image to that of the selected inventory item. The boolean
//	parameter indicates if the prototype indicator should be cloned or if the
//	prototype indicator is the visual to be positioned.
///////////////////////////////////////////////////////////////////////////////

void
CMenuInventory::SetSelectionIndicator(CMenuInventoryItem *pItem,
									  INVENTORY_SELECTION_TYPE eSelectionType,
									  bool bShouldClone)
{
	// GET indicator visual ID and name prefix
	CString strPrefix;
	MENU_IDS eIndicatorID;
	this->GetSelectionIndicatorInfo(eSelectionType, eIndicatorID, strPrefix);

	// ERROR checking: Ensure that we get the correct item from the player's inventory
	//	NOTE: Sometimes a call can be made from the Quickbar and the CMenuInventoryItem param
	//	reflects the position of the Quickbar icon.
	CMenuInventoryItem **pInventoryItem = this->GetInventoryItemByIndex(pItem->m_data.m_index);
	if(!pInventoryItem) return;
	
	if (!*pInventoryItem) return;

	// GET references to menu visuals
	CMenuImage *pPrototypeIndicator = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(eIndicatorID, true));
	if (!pPrototypeIndicator) return;

	// IF we're cloning the prototype indicator...
	CMenuImage *pIndicator = NULL;
	if (bShouldClone)
	{
		// CLONE the prototype indicator
		pPrototypeIndicator->Clone((CMenuVisual**)&pIndicator, globalStatus->m_3dDevice);
		pIndicator->m_visible = true;

		// ADD the indicator visual to the inventory's child list
		pIndicator->m_visualID = globalStatus->m_runtimeMenuDatabase->GetFreeID(pIndicator->m_visualID);
		pIndicator->m_name = strPrefix + globalStatus->m_cabLib.ConvToString(pItem->m_data.m_GLID);
		this->m_childList->AddHead(pIndicator);

		// UPDATE the texture
		pIndicator->m_blendMode = 1; // ALPHA BLEND
		pIndicator->m_textureSelect = globalStatus->m_textureDataBase->AddAssignTempTexture(
			globalStatus->m_3dDevice, 
			pIndicator->m_textureFile, 
			0);
		pIndicator->UpdateTexture(pIndicator->m_textureSelect);
	}
	else pIndicator = pPrototypeIndicator;

	// SET the position and visibility of the selection indicator 
	//	to that of the selected item
	int iOffsetX = ((*pInventoryItem)->m_pIcon->m_width - pIndicator->m_width) / 2.0f;
	int iOffsetY = ((*pInventoryItem)->m_pIcon->m_height - pIndicator->m_height) / 2.0f;
	pIndicator->m_posX = (*pInventoryItem)->m_pIcon->m_posX + iOffsetX;
	pIndicator->m_posY = (*pInventoryItem)->m_pIcon->m_posY + iOffsetY;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	RemoveSelectionIndicator(CMenuInventoryItem*, INVENTORY_SELECTION_TYPE)
// Description:	Remove the specified selection indicator from the specified
//	inventory item.
///////////////////////////////////////////////////////////////////////////////

void
CMenuInventory::RemoveSelectionIndicator(CMenuInventoryItem *pItem, 
										 INVENTORY_SELECTION_TYPE eSelectionType)
{
	// GET indicator visual ID and name prefix
	CString strPrefix;
	MENU_IDS eIndicatorID;
	this->GetSelectionIndicatorInfo(eSelectionType, eIndicatorID, strPrefix);

	// GET references to menu visuals
	CMenuImage *pPrototypeIndicator = dynamic_cast<CMenuImage*>(globalStatus->m_runtimeMenuDatabase->GetVisualByID(eIndicatorID, true));
	if (!pPrototypeIndicator) return;

	// GET the GLID of the selected item as a string
	CString strVisualID = globalStatus->m_cabLib.ConvToString(pItem->m_data.m_GLID);

	POSITION p1 = NULL, p2 = NULL;
	for (p1 = p2 = this->m_childList->GetHeadPosition(); (p2 = p1) != NULL;)
	{
		CMenuImage *pIndicator = dynamic_cast<CMenuImage*>(this->m_childList->GetNext(p1));
		if (!pIndicator) continue;

		if ((pIndicator->m_name.Find(strPrefix) != -1)
		&& (pIndicator->m_name.Find(strVisualID) != -1))
		{
			this->m_childList->RemoveAt(p2);
			SafeDelete(pIndicator);
			//this->RefreshItems();
			return;
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	GetSelectionIndicatorInfo(INVENTORY_SELECTION_TYPE, MENU_IDS&, CString&)
// Description:	This is necessary for consistently generating/retrieving names for 
//	cloned selection indicators.
///////////////////////////////////////////////////////////////////////////////

void	
CMenuInventory::GetSelectionIndicatorInfo(INVENTORY_SELECTION_TYPE eSelectionType,
										  MENU_IDS &eIndicatorID,
										  CString &strIndicatorPrefix)
{
	// DETERMINE the appropriate selection indicator visual ID
	//	AND SELECT the string prefix for generating the clone's name
	switch (this->m_visualID)
	{
		default: return;
		case ID_PLAYER_INVENTORY: 
		{
			switch (eSelectionType)
			{
				default: return;
				case IST_ITEM: 
					eIndicatorID = ID_PLAYER_INVENTORY_SELECTION_IMAGE;
					strIndicatorPrefix = "InventorySelectedIndicator_";
					break;
				case IST_QUICKBAR: 
					eIndicatorID = ID_PLAYER_INVENTORY_QUICKBAR_SELECTION_IMAGE;
					strIndicatorPrefix = "InventoryQuickbarIndicator_";
					break;
				case IST_EQUIPPED: 
					eIndicatorID = ID_PLAYER_INVENTORY_EQUIPPED_SELECTION_IMAGE;
					strIndicatorPrefix = "InventoryEquippedIndicator_";
					break;
			}
			break;
		}
		case ID_BANK_INVENTORY_01: 
		case ID_BANK_INVENTORY_02:
		case ID_BANK_INVENTORY_03:
		case ID_BANK_INVENTORY_04:
			eIndicatorID = ID_BANK_INVENTORY_SELECTION_IMAGE; 
			strIndicatorPrefix = "BankSelectedIndicator_";
			break;
		case ID_STORE_INVENTORY: 
			eIndicatorID = ID_STORE_INVENTORY_SELECTION_IMAGE; 
			strIndicatorPrefix = "StoreSelectedIndicator_";
			break;
		case ID_TRADE_YOUR_INVENTORY: 
			eIndicatorID = ID_TRADE_INVENTORY_SELECTION_IMAGE; 
			strIndicatorPrefix = "TradeSelectedIndicator_";
			break;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CMenuInventory
// Function:	UpdateInventoryItem(ITEMDATA_STRUCT_EX)
// Description:	Update specified item info and refresh the associated visual.
///////////////////////////////////////////////////////////////////////////////

bool
CMenuInventory::UpdateInventoryItem(ITEMDATA_STRUCT_EX item)
{
	std::vector<CMenuInventoryItem*>::iterator iter;
	for (iter = this->m_items.begin(); iter != this->m_items.end(); iter++)
		if ((*iter)->m_data.m_index == item.m_index) break;
	if (iter == this->m_items.end()) return false;

	// UPDATE the current item's values
	(*iter)->m_data = item;

	// UPDATE item armed indicator
	if (item.m_armed)
		this->SetSelectionIndicator((*iter), CMenuInventory::IST_EQUIPPED, true);
	else
		this->RemoveSelectionIndicator((*iter), CMenuInventory::IST_EQUIPPED);
	return true;
}