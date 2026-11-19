#include "stdafx.h"
#include "CMenuVisual.h"
#include "CMenuWindow.h"
#include "CMenuComboBox.h"
#include "CMenuListBox.h"
#include "CMenuTextBox.h"
#include "CMenuText.h"
#include "CMenuScrollBar.h"
#include "CMenuButton.h"
#include "CMenuToggle.h"
#include "CMenuProgressBar.h"
#include "CMenuImage.h"
#include "CMenuEntity.h"
#include "CMenuScript.h"
#include "CMenuInventory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMenuVisual::CMenuVisual()
{
	m_visualID		 = 0;
	m_width			 = 0;
	m_height		 = 0;
	m_posX			 = 0;
	m_posY			 = 0;
	m_enabled		 = true;
	m_visible		 = true;
	m_isFocused		 = false;
	m_childList		 = NULL;
	m_miscTimeStamp  = GetTickCount();
}

void CMenuVisual::Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice)
{
	// ERROR checking
	if (!pCopy)
		return;

	// COPY visual base properties
	if (pCopy->m_name == "")
		pCopy->m_name			= m_name;
	if (pCopy->m_visualID <= 0)
		pCopy->m_visualID		= m_visualID;  //TODO: Add instance ids for visual ids
	pCopy->m_width			= m_width;
	pCopy->m_height			= m_height;
	pCopy->m_posX			= m_posX;
	pCopy->m_posY			= m_posY;
	pCopy->m_enabled		= m_enabled;
	pCopy->m_visible		= m_visible;
	pCopy->m_isFocused		= false;

	if(m_childList)
		m_childList->Clone((CMenuVisualList**)&pCopy->m_childList, pD3DDevice);
}

void CMenuVisual::Serialize(CXMLFileIO &xml)
{
	if(xml.m_isStoring)
	{
		xml.WriteOpeningTag("VisualProperties");

		xml.WriteAttribute("Version", 1);
		xml.WriteAttribute("Name", m_name.GetBuffer());
		xml.WriteAttribute("VisualID", m_visualID);
		xml.WriteAttribute("Width", (int)m_width);
		xml.WriteAttribute("Height", (int)m_height);
		xml.WriteAttribute("PositionX", (int)m_posX);
		xml.WriteAttribute("PositionY", (int)m_posY);
		xml.WriteAttribute("Enabled", m_enabled);
		xml.WriteAttribute("Visible", m_visible);
		
		if(m_childList && m_childList->GetCount() > 0)
		{
			xml.WriteOpeningTag("ChildList");
			m_childList->Serialize(xml);
			xml.WriteClosingTag("ChildList");
		}

		xml.WriteClosingTag("VisualProperties");
	}
	else
	{
		bool closing;
		char buffer[MAX_BUFFER];
		int  version;

		xml.ReadTag(buffer, MAX_BUFFER, closing); //Read <VisualProperties>

		xml.ReadAttribute(version);
		xml.ReadAttribute(buffer, MAX_BUFFER); m_name = buffer;
		xml.ReadAttribute(m_visualID);
		xml.ReadAttribute(m_width);
		xml.ReadAttribute(m_height);
		xml.ReadAttribute(m_posX);
		xml.ReadAttribute(m_posY);
		xml.ReadAttribute(m_enabled);
		xml.ReadAttribute(m_visible);

		xml.ReadTag(buffer, MAX_BUFFER, closing); //Could be <ChildList> or </VisualProperties>
		if(!closing)
		{
			m_childList = new CMenuVisualList();
			m_childList->Serialize(xml);

			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read </VisualProperties>
		}
	}
}

/*CMenuVisual& 
CMenuVisual::operator=(const CMenuVisual &rhs)
{
	if(this != &rhs)
	{
		this->m_name			= rhs.m_name;
		this->m_visualID		= rhs.m_visualID;  //TODO: Add instance ids for visual ids
		this->m_width			= rhs.m_width;
		this->m_height			= rhs.m_height;
		this->m_posX			= rhs.m_posX;
		this->m_posY			= rhs.m_posY;
		this->m_enabled			= rhs.m_enabled;
		this->m_visible			= rhs.m_visible;
		this->m_isFocused		= false;

		//if(this->m_childList)
		//	m_childList->Clone((CMenuVisualList**)&avlue.m_childList, pD3DDevice);
		if(rhs.m_childList) this->m_childList = rhs.m_childList;
	}

	// Allow for chained assignments
	return *this;
}*/

/*CMenuVisualList&
CMenuVisualList::operator=(const CMenuVisualList &rhs)
{
	CMenuVisual *pNewVisual = NULL,
				*pTempVisual = NULL;
	POSITION pos = rhs.GetHeadPosition();
	while(pos != NULL)
	{
		pTempVisual = (CMenuVisual*)rhs.GetNext(pos);
		pNewVisual = new CMenuVisual();
		pNewVisual = pTempVisual;
		this->AddTail(pNewVisual);
	}
	
	// Allow for chained assignments
	return *this;
}*/

CMenuVisual* CMenuVisualList::GetVisualByID(int visualID, bool searchChildLists)
{
	POSITION posCurVisual = GetHeadPosition();
	while(posCurVisual != NULL)
	{
		CMenuVisual *visual = (CMenuVisual*)GetNext(posCurVisual);
		if(visual->m_visualID == visualID)
			return visual;

		if(searchChildLists && visual->m_childList)
			if(visual = visual->m_childList->GetVisualByID(visualID, true))
				return visual;
	}

	return NULL;
}

CMenuVisual* CMenuVisualList::GetVisualByName(CString visualName, bool searchChildLists)
{
	POSITION posCurVisual = GetHeadPosition();
	while(posCurVisual != NULL)
	{
		CMenuVisual *visual = (CMenuVisual*)GetNext(posCurVisual);
		if(visual->m_name.CompareNoCase(visualName) == 0)
			return visual;

		if(searchChildLists && visual->m_childList)
			if(visual = visual->m_childList->GetVisualByName(visualName, true))
				return visual;
	}

	return NULL;
}

bool CMenuVisualList::CloseByID(int visualID, int *showMenuCursor)
{
	bool closedMenu = false;

	//run scripts pass, have to do this in a separate pass because it can remove items within same loop and thats very dangerous
	POSITION posLast = NULL;
	for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
	{
		CMenuWindow *visual = (CMenuWindow*)GetNext(posLoc);
		if(visual->m_visualID == visualID)
		{
			RemoveAt(posLast);
			visual->m_isFocused = false;
			if(visual->m_showCursor)
				(*showMenuCursor)--;

			if (visual->m_closeScript)
				visual->m_closeScript->ExecuteScript();
				
			closedMenu = true;
			break;
		}
	}

	/*POSITION posLastVisual, posCurVisual = GetHeadPosition();
	while(posCurVisual != NULL)
	{
		posLastVisual = posCurVisual;
		CMenuWindow *visual = (CMenuWindow*)GetNext(posCurVisual);
		if(visual->m_visualID == visualID)
		{
			RemoveAt(posLastVisual);//remove before executing close script because that could cause a sync issue with this being mid loop and that doing a loop
			// RUN the close script
			if (visual->m_closeScript)
				visual->m_closeScript->ExecuteScript();

			visual->m_isFocused = false;
			
			//If this menu had a cursor shown, decrement the show cursor variable
			if(visual->m_showCursor)
				(*showMenuCursor)--;

			//Set Bool So We Can Return it
			closedMenu = true;

			// We've found the menu to close, so BREAK
			break;
		}
	}*/

	return closedMenu;
}

bool CMenuVisualList::CloseAll(int *showMenuCursor, bool onlyMenusWithCursor)
{
	bool			closedMenu = true;
	*showMenuCursor = 0;

	// Remove ALL menus
	if(!onlyMenusWithCursor)
	{
		//have to start over every exec because during these scripts can change contects of the list
		bool done = false;
		POSITION posLast = NULL;

		while(!done)
		{
			done = true;//switch
			
			for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
			{
				CMenuWindow *visual = (CMenuWindow*)GetNext(posLoc);
				RemoveAt(posLast);
				visual->m_isFocused		= false;
				if (visual->m_closeScript)
				{
					visual->m_closeScript->ExecuteScript();//beware this can change the list, must start from the beginning also
					done = false;
					break;
				}
			}
		}
	}
	// Remove ONLY menus with visible cursors
	else
	{

		bool done = false;
		POSITION posLast = NULL;

		while(!done)///must be done this way due to exec ascript poten tially changing list mid pass
		{
			done = true;//switch
		
			for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
			{
				CMenuWindow *visual = (CMenuWindow*)GetNext(posLoc);
				if(visual->m_showCursor)
					if(visual->m_escapable)
					{
						RemoveAt(posLast);
						//if(visual->m_showCursor)
							//(*showMenuCursor)--;	
						visual->m_isFocused		= false;
						if(visual->m_closeScript)
						{
							visual->m_closeScript->ExecuteScript();//beware this can change the list, must start from the beginning also
							done = false;
							break;
						}
					}
					else
						closedMenu = false;//having this prevents mouse from toggling
			}
		}
	
		/*while(posCurVisual != NULL)
		{
			posLastVisual = posCurVisual;
			visual = (CMenuWindow*)GetNext(posCurVisual);
			if(visual->m_showCursor)
			{
				if(visual->m_escapable)
				{
					// RUN the close script
					if (visual->m_closeScript)
						visual->m_closeScript->ExecuteScript();

					RemoveAt(posLastVisual);
				}
				else 
				{
					closedMenu = false;
					(*showMenuCursor)++;
				}
			}
		}*/
	}
	return closedMenu;
}

int CMenuVisualList::GetFreeID(int currentID)
{
	//If there's only 1 instance of the Current Visual ID, return it
	if(currentID != -1)
	if((GetVisualIDCount(currentID) == 1)
	&& currentID > 0)
		return currentID;

	for(int i = 0; i < INT_MAX; i++)
		if(GetVisualIDCount(i) == 0)
			return i;

	return -1;
}

int CMenuVisualList::GetVisualIDCount(int searchID)
{
	//This function uses recursion to count the number of times
	//that the specified ID appears in the list
	int count = 0;
	POSITION posCurVisual = GetHeadPosition();
	while(posCurVisual != NULL)
	{
		CMenuVisual *visual = (CMenuVisual*)GetNext(posCurVisual);
		if(visual->m_visualID == searchID)
			count++;

		if(visual->m_childList)
			count += visual->m_childList->GetVisualIDCount(searchID);
	}

	return count;
}

void CMenuVisualList::Clone(CMenuVisualList **pClone, LPDIRECT3DDEVICE9 pD3DDevice)
{
	*pClone = new CMenuVisualList();
	Copy(*pClone, pD3DDevice);
}

void CMenuVisualList::Copy(CMenuVisualList *pCopy, LPDIRECT3DDEVICE9 pD3DDevice)
{
	CMenuVisual * newVisual = NULL;
	POSITION posCurVisual = GetHeadPosition();
	while(posCurVisual != NULL)
	{
		CMenuVisual * visual = (CMenuVisual*) GetNext(posCurVisual);
		switch(visual->GetType())
		{
			case MENU_VISTYPE_NONE:
				if(1)
				{
					visual->Clone(&newVisual,pD3DDevice);
					pCopy->AddTail(newVisual);
				}
			break;
			case MENU_VISTYPE_WINDOW:
				if(1)
				{
					CMenuWindow * cast = (CMenuWindow *)visual;
					cast->Clone(&newVisual,pD3DDevice);
					pCopy->AddTail(newVisual);
				}
			break;
			case MENU_VISTYPE_COMBOBOX:
				if(1)
				{
					CMenuComboBox * cast = (CMenuComboBox *)visual;
					cast->Clone(&newVisual,pD3DDevice);
					pCopy->AddTail(newVisual);
				}
			break;
			case MENU_VISTYPE_LISTBOX:
				if(1)
				{
					CMenuListBox * cast = (CMenuListBox *)visual;
					cast->Clone(&newVisual,pD3DDevice);
					pCopy->AddTail(newVisual);
				}
			break;
			case MENU_VISTYPE_TEXTBOX:
				if(1)
				{
					CMenuTextBox * cast = (CMenuTextBox *)visual;
					cast->Clone(&newVisual,pD3DDevice);
					pCopy->AddTail(newVisual);
				}
			break;
			case MENU_VISTYPE_TEXT:
				if(1)
				{
					CMenuText * cast = (CMenuText *)visual;
					cast->Clone(&newVisual,pD3DDevice);
					pCopy->AddTail(newVisual);
				}
			break;
			case MENU_VISTYPE_SCROLLBAR:
				if(1)
				{
					CMenuScrollBar * cast = (CMenuScrollBar *)visual;
					cast->Clone(&newVisual,pD3DDevice);
					pCopy->AddTail(newVisual);
				}
			break;
			case MENU_VISTYPE_BUTTON:
				if(1)
				{
					CMenuButton * cast = (CMenuButton *)visual;
					cast->Clone(&newVisual,pD3DDevice);
					pCopy->AddTail(newVisual);
				}
			break;
			case MENU_VISTYPE_TOGGLE:
				if(1)
				{
					CMenuToggle * cast = (CMenuToggle *)visual;
					cast->Clone(&newVisual,pD3DDevice);
					pCopy->AddTail(newVisual);
				}
			break;
			case MENU_VISTYPE_PROGRESSBAR:
				if(1)
				{
					CMenuProgressBar * cast = (CMenuProgressBar *)visual;
					cast->Clone(&newVisual,pD3DDevice);
					pCopy->AddTail(newVisual);
				}
			break;
			case MENU_VISTYPE_IMAGE:
				if(1)
				{
					CMenuImage * cast = (CMenuImage *)visual;
					cast->Clone(&newVisual,pD3DDevice);
					pCopy->AddTail(newVisual);
				}
			break;
			case MENU_VISTYPE_ENTITY:
				if(1)
				{
					CMenuEntity * cast = (CMenuEntity *)visual;
					cast->Clone(&newVisual,pD3DDevice);
					pCopy->AddTail(newVisual);
				}
			break;
			case MENU_VISTYPE_INVENTORY:
			{
				CMenuInventory *cast = (CMenuInventory *)visual;
				cast->Clone(&newVisual, pD3DDevice);
				pCopy->AddTail(newVisual);
				break;
			}
		}
	}
}

CMenuVisualList::~CMenuVisualList()
{
	if(!m_referenceMemory && this->GetCount() > 0)
	   ClearList();
}

void CMenuVisualList::ClearList()
{
	POSITION posCurr = NULL, posPrev = NULL;
    for(posCurr = posPrev = this->GetHeadPosition(); (posPrev = posCurr) != NULL;)
	{
        CMenuVisual *mvsP = (CMenuVisual*)this->GetNext(posCurr);
		this->RemoveAt(posPrev);
		SafeDelete(mvsP);
		
		//DEPRECATED /////////////////////////////////////////////////////////////
        //DeleteByObject(mvsP);
		//////////////////////////////////////////////////////////////////////////
	}
	//RemoveAll();
}

CMenuVisual::~CMenuVisual()
{
	if (this->m_childList)
		this->m_childList->ClearList();
	SafeDelete(this->m_childList);
	/* DEPRECATED /////////////////////////////////////////////////////////////
	if(m_childList)
	{
	  delete m_childList;
	  m_childList = 0;
	}
	/////////////////////////////////////////////////////////////////////////*/
}

void CMenuVisualList::DeleteByObject(CMenuVisual *curVisual)
{
	//Delete the actual object using it's correct pointer type
	switch(curVisual->GetType())
	{
		case MENU_VISTYPE_NONE:
			delete curVisual;
			break;
		case MENU_VISTYPE_WINDOW:
			delete (CMenuWindow*)curVisual;
			break;
		case MENU_VISTYPE_COMBOBOX:
			delete (CMenuComboBox*)curVisual;
			break;
		case MENU_VISTYPE_LISTBOX:
			delete (CMenuListBox*)curVisual;
			break;
		case MENU_VISTYPE_TEXTBOX:
			delete (CMenuTextBox*)curVisual;
			break;
		case MENU_VISTYPE_TEXT:
			delete (CMenuText*)curVisual;
			break;
		case MENU_VISTYPE_SCROLLBAR:
			delete (CMenuScrollBar*)curVisual;
			break;
		case MENU_VISTYPE_BUTTON:
			delete (CMenuButton*)curVisual;
			break;
		case MENU_VISTYPE_TOGGLE:
			delete (CMenuToggle*)curVisual;
			break;
		case MENU_VISTYPE_PROGRESSBAR:
			delete (CMenuProgressBar*)curVisual;
			break;
		case MENU_VISTYPE_IMAGE:
			delete (CMenuImage*)curVisual;
			break;
		case MENU_VISTYPE_ENTITY:
			delete (CMenuEntity*)curVisual;
			break;
		case MENU_VISTYPE_INVENTORY:
			delete (CMenuInventory*)curVisual;
			break;
	}
}

void CMenuVisualList::Serialize(CXMLFileIO &xml)
{
	if(xml.m_isStoring)
	{
		xml.WriteOpeningTag("VisualList");

		//Loop through all visuals in the list
		POSITION posCurVisual = GetHeadPosition();
		while(posCurVisual != NULL)
		{
			CMenuVisual *curVisual = (CMenuVisual*)GetNext(posCurVisual);

			//Serialze the object using it's correct pointer type
			switch(curVisual->GetType())
			{
				case MENU_VISTYPE_WINDOW:
					xml.WriteOpeningTag("Window");
					((CMenuWindow*)curVisual)->Serialize(xml);
					xml.WriteClosingTag("Window");
					break;
				case MENU_VISTYPE_COMBOBOX:
					if(!((CMenuComboBox*)curVisual)->m_listBox.m_font)
						break;

					xml.WriteOpeningTag("ComboBox");
					((CMenuComboBox*)curVisual)->Serialize(xml);
					xml.WriteClosingTag("ComboBox");
					break;
				case MENU_VISTYPE_LISTBOX:
					if(!((CMenuListBox*)curVisual)->m_font)
						break;

					xml.WriteOpeningTag("ListBox");
					((CMenuListBox*)curVisual)->Serialize(xml);
					xml.WriteClosingTag("ListBox");
					break;
				case MENU_VISTYPE_TEXTBOX:
					if(!((CMenuTextBox*)curVisual)->m_font)
						break;

					xml.WriteOpeningTag("TextBox");
					((CMenuTextBox*)curVisual)->Serialize(xml);
					xml.WriteClosingTag("TextBox");
					break;
				case MENU_VISTYPE_TEXT:
					if(!((CMenuText*)curVisual)->m_font)
						break;

					xml.WriteOpeningTag("Text");
					((CMenuText*)curVisual)->Serialize(xml);
					xml.WriteClosingTag("Text");
					break;
				case MENU_VISTYPE_SCROLLBAR:
					xml.WriteOpeningTag("ScrollBar");
					((CMenuScrollBar*)curVisual)->Serialize(xml);
					xml.WriteClosingTag("ScrollBar");
					break;
				case MENU_VISTYPE_BUTTON:
					xml.WriteOpeningTag("Button");
					((CMenuButton*)curVisual)->Serialize(xml);
					xml.WriteClosingTag("Button");
					break;
				case MENU_VISTYPE_TOGGLE:
					xml.WriteOpeningTag("Toggle");
					((CMenuToggle*)curVisual)->Serialize(xml);
					xml.WriteClosingTag("Toggle");
					break;
				case MENU_VISTYPE_PROGRESSBAR:
					xml.WriteOpeningTag("ProgressBar");
					((CMenuProgressBar*)curVisual)->Serialize(xml);
					xml.WriteClosingTag("ProgressBar");
					break;
				case MENU_VISTYPE_IMAGE:
					xml.WriteOpeningTag("Image");
					((CMenuImage*)curVisual)->Serialize(xml);
					xml.WriteClosingTag("Image");
					break;
				case MENU_VISTYPE_ENTITY:
					xml.WriteOpeningTag("Entity");
					((CMenuEntity*)curVisual)->Serialize(xml);
					xml.WriteClosingTag("Entity");
					break;
				case MENU_VISTYPE_INVENTORY:
				{
					xml.WriteOpeningTag("Inventory");
					((CMenuInventory*)curVisual)->Serialize(xml);
					xml.WriteClosingTag("Inventory");
					break;
				}
			}
		}

		xml.WriteClosingTag("VisualList");
	}
	else
	{
		char buffer[MAX_BUFFER] = {0};
		bool closing;
		CMenuVisual *newVisual = NULL;

		//Read in <VisualList>
		xml.ReadTag(buffer, MAX_BUFFER, closing);

		//Loop til we hit </VisualList>
		while(1)
		{
			//Read the opening tag for the visual or </VisualList>
			xml.ReadTag(buffer, MAX_BUFFER, closing);

			//If its </VisualList>, break out of the loop
			if(closing)
				break;

			newVisual = NULL;   // never re-add the previous visual on an unknown tag (double delete)
			if(strcmp(buffer, "Window") == 0)
			{
				newVisual = new CMenuWindow();
				newVisual->Serialize(xml);
			}
			else if(strcmp(buffer, "ComboBox") == 0)
			{
				newVisual = new CMenuComboBox();
				newVisual->Serialize(xml);
			}
			else if(strcmp(buffer, "ListBox") == 0)
			{
				newVisual = new CMenuListBox();
				newVisual->Serialize(xml);
			}
			else if(strcmp(buffer, "TextBox") == 0)
			{
				newVisual = new CMenuTextBox();
				newVisual->Serialize(xml);
			}
			else if(strcmp(buffer, "Text") == 0)
			{
				newVisual = new CMenuText();
				newVisual->Serialize(xml);
			}
			else if(strcmp(buffer, "ScrollBar") == 0)
			{
				newVisual = new CMenuScrollBar();
				newVisual->Serialize(xml);
			}
			else if(strcmp(buffer, "Button") == 0)
			{
				newVisual = new CMenuButton();
				newVisual->Serialize(xml);
			}
			else if(strcmp(buffer, "Toggle") == 0)
			{
				newVisual = new CMenuToggle();
				newVisual->Serialize(xml);
			}
			else if(strcmp(buffer, "ProgressBar") == 0)
			{
				newVisual = new CMenuProgressBar();
				newVisual->Serialize(xml);
			}
			else if(strcmp(buffer, "Image") == 0)
			{
				newVisual = new CMenuImage();
				newVisual->Serialize(xml);
			}
			else if(strcmp(buffer, "Entity") == 0)
			{
				newVisual = new CMenuEntity();
				newVisual->Serialize(xml);
			}
			else if(strcmp(buffer, "Inventory") == 0)
			{
				newVisual = new CMenuInventory();
				newVisual->Serialize(xml);
			}

			if(!newVisual)
			{
				// Unknown element: the file is out of step with this build. Stop reading
				// this list rather than corrupt memory; the rest of the file is skipped.
				char msg[MAX_BUFFER + 64];
				sprintf_s(msg, "MenuDatabase: unknown visual tag <%s> - stopped reading this list\n", buffer);
				OutputDebugStringA(msg);
				break;
			}
			AddTail(newVisual);

			//NOTE: The visuals read their own closing tags
		}

		//Read in the closing tag(probably </MenuDatabase>)
		xml.ReadTag(buffer, MAX_BUFFER, closing);
	}
}
