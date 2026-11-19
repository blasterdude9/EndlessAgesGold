#include "stdafx.h"
#include "direct.h"
#include "GlobalStatusVariables.h"
#include "CMenuButton.h"
#include "CMenuScript.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern GlobalStatusVariables *globalStatus;

CMenuButton::CMenuButton()
{
	//Visible Objects
	for(int i = 0; i < MENU_BUTTONSTATE_MAX; i++)
		for(int j = 0; j < MENU_BUTTONIMAGE_MAX; j++)
			m_stateImages[i][j] = NULL;

	//Engine Variables
	m_visibleState		= MENU_BUTTONSTATE_UP;
	m_mousedOver		= false;
	m_draggingItem		= NULL;
	m_pDisplayText		= NULL;
	this->InitText();

	//Scripts
	m_clickScript		= NULL;
	m_mouseOverScript	= NULL;
	m_mouseOutScript	= NULL;
	m_dropScript		= NULL;
}

void
CMenuButton::InitText()
{
	SafeDelete(m_pDisplayText);
	m_pDisplayText = new CMenuText();
	m_pDisplayText->m_font = globalStatus->m_menuFontList->GetPointerFromIndex(0);
	m_pDisplayText->m_alignment = MENU_TEXTALIGN_CENTER;
	m_pDisplayText->Init(globalStatus->m_3dDevice);
	this->SetEnabled();
}

void
CMenuButton::SetText(LPTSTR value)
{
	if(this->m_pDisplayText)
		this->m_pDisplayText->SetText(value);
}

void 
CMenuButton::SetEnabled(bool bEnable)
{
	// Update button enabled state
	this->m_enabled = bEnable;
	this->m_visibleState = MENU_BUTTONSTATE_UP;

	// Update button text color state
	if (this->m_pDisplayText)
	{
		if (bEnable)
		{
			// Set associated text to enabled color
			this->m_pDisplayText->m_red = (0.0f / 255.0f);
			this->m_pDisplayText->m_green = (0.0f / 255.0f);
			this->m_pDisplayText->m_blue = (0.0f / 255.0f);
			this->m_pDisplayText->m_alpha = 1.0f;
		}
		else
		{
			// Set associated text to disabled color
			this->m_pDisplayText->m_red = (0.0f / 255.0f);
			this->m_pDisplayText->m_green = (0.0f / 255.0f);
			this->m_pDisplayText->m_blue = (0.0f / 255.0f);
			this->m_pDisplayText->m_alpha = 0.5f;
		}
	}

	// Refresh CMenuText mesh
	if (this->m_pDisplayText->m_text != "")
		this->m_pDisplayText->SetText(this->m_pDisplayText->m_text.GetBuffer());
}

bool CMenuButton::HandleInput(CInput *pInput, float parentX, float parentY, DWORD curTime, LPDIRECT3DDEVICE9 pD3DDevice)
{
	// If menu cursor is not showing, don't handle input
	if(!globalStatus->m_showMenuCursor) return false;

	//The button is now the parent
	parentX += m_posX;
	parentY += m_posY;

	//The default state is UP
	m_visibleState = MENU_BUTTONSTATE_UP;

	//See if the mouse is inside of the button's bounds
	bool insideButton = pInput->m_mouseAbsPosX > parentX && pInput->m_mouseAbsPosX < parentX + m_width &&
						pInput->m_mouseAbsPosY > parentY && pInput->m_mouseAbsPosY < parentY + m_height;

	//If the left mouse button is currently down
	if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].IsDown())
	{
		//If it's inside the button
		if(insideButton)
		{
			//If the button was previously pushed and the mouse still hasn't been released(m_buttonState.IsDown())
			//Or, if the player just pressed the left mouse button
			if(m_buttonState.IsDown() || pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustPressed())
			{
				m_buttonState.UpdateState(true, curTime);
				
				//If the down state exists, set the button to the down state
				if(m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]
				&& this->m_enabled)
					m_visibleState = MENU_BUTTONSTATE_DOWN;

				//We handled the input
				return true;
			}
		}
		else //Otherwise, the button's state should be set to up
			m_visibleState = MENU_BUTTONSTATE_UP;
	}
	//If the button was previously pushed, but the mouse button isn't down anymore,
	//release the button
	else if(m_buttonState.IsDown())
	{
		//Reset the button's state
		m_buttonState.UpdateState(false, 0);

		//If they're inside the button
		if(insideButton)
		{
			if (this->m_enabled)
			{
				//If there's a click script, execute it once
				if (m_clickScript)
					m_clickScript->ExecuteScript();

				//Set the visible state to hover
				m_visibleState = MENU_BUTTONSTATE_HOVER;
			}
			return true;
		}
		else
		{
			//Set the visible state to up
			m_visibleState = MENU_BUTTONSTATE_UP;

			//Clear the just released flag
			m_buttonState.UpdateState(false, 0);
		}
	}
	//If the left mouse button was just released and it was dragging an item
	else if(m_draggingItem && insideButton && pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustReleased() && /***/m_draggingItem)
	{
		//Execute the drop script, if it exists
		if(m_dropScript)
		{
			m_dropScript->ExecuteScript();
		}

		return true;
	}
	else //The mouse isn't down and the button wasn't previously pushed
	{
		//Clear the button's "Just Released" state
		m_buttonState.UpdateState(false, 0);

		//If we're inside the button
		if(insideButton)
		{
			//Goto the hover state, if it exists
			if(m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]
			&& this->m_enabled)
			{
				m_visibleState = MENU_BUTTONSTATE_HOVER;
			}

			//Continuously execute the script(allows for pausing)
			if(m_mouseOverScript)
				m_mouseOverScript->ExecuteScript();

			// Update text pos to accompany hover
			if (!m_mousedOver)
			{
				if (this->m_pDisplayText)
				{
					/* Update text pos
					this->m_pDisplayText->m_posX += 2;
					this->m_pDisplayText->m_posY -= 1;*/

					// Update text display
					this->m_pDisplayText->SetText(this->m_pDisplayText->m_text.GetBuffer());
				}
			}

			//Allow the mouse out script to run
			m_mousedOver = true;
		}
		else
		{
			//Execute Mouse Out Script
			if(m_mousedOver)
			{
				if(m_mouseOutScript)
					m_mouseOutScript->ExecuteScript();

				// Update text for MOUSEOUT
				if (this->m_pDisplayText)
				{
					/* Update text pos
					this->m_pDisplayText->m_posX -= 2;
					this->m_pDisplayText->m_posY += 1;*/

					// Update text display
					this->m_pDisplayText->SetText(this->m_pDisplayText->m_text.GetBuffer());
				}
			}

			//We're no longer hovering over the button
			m_mousedOver = false;
		}
	}

	return false;
}

void CMenuButton::Render(LPDIRECT3DDEVICE9 pD3DDevice,
						 CStateManagementObj *stateManager, 
						 CTextureDBObjList *textureDatabase,
						 float parentX,
						 float parentY)
{
	//The button is the parent now
	parentX += m_posX;
	parentY += m_posY;

	//Render the current state
	for(int i = 0; i < MENU_BUTTONIMAGE_MAX; i++)
		if(m_stateImages[m_visibleState][i])
			m_stateImages[m_visibleState][i]->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	// Render the associated text
	// NOTE: The position of toggle display text is hardcoded
	// in the params sent to the CMenuText's Render() function.
	if (this->m_pDisplayText && 
	this->m_pDisplayText->m_text != "")
	{
		this->m_pDisplayText->Render(
			pD3DDevice, 
			stateManager, 
			textureDatabase, 
			parentX + (this->m_width / 2.0f), 
			parentY + (this->m_height / 4.0f));
	}
}

void CMenuButton::Resize()
{
	//Loop through every possible state
	for(int i = 0; i < MENU_BUTTONSTATE_MAX; i++)
	{
		//Start the left and right coordinates for the middle as 0 and the button's width
		float middleLeft	= 0.0f;
		float middleRight	= m_width;

		//If this state has a left component, set its coordinates
		//and shift the middle's left coordinate to the right by the component's width
		if(m_stateImages[i][MENU_BUTTONIMAGE_LEFT])
		{
			m_stateImages[i][MENU_BUTTONIMAGE_LEFT]->m_height = m_height;
			middleLeft += m_stateImages[i][MENU_BUTTONIMAGE_LEFT]->m_width;
		}

		//If this state has a right component, set its coordinates
		//and shift the middle's right coordinate to the left by the component's width
		if(m_stateImages[i][MENU_BUTTONIMAGE_RIGHT])
		{
			m_stateImages[i][MENU_BUTTONIMAGE_RIGHT]->m_posX	= m_width - m_stateImages[i][MENU_BUTTONIMAGE_RIGHT]->m_width;
			m_stateImages[i][MENU_BUTTONIMAGE_RIGHT]->m_height	= m_height;
			middleRight -= m_stateImages[i][MENU_BUTTONIMAGE_RIGHT]->m_width;
		}

		//If this state has a middle component, set its coordinates
		if(m_stateImages[i][MENU_BUTTONIMAGE_MIDDLE] && middleRight > middleLeft)
		{
			m_stateImages[i][MENU_BUTTONIMAGE_MIDDLE]->m_posX	= middleLeft;
			m_stateImages[i][MENU_BUTTONIMAGE_MIDDLE]->m_width	= middleRight - middleLeft;
			m_stateImages[i][MENU_BUTTONIMAGE_MIDDLE]->m_height	= m_height;
		}
	}
}

void CMenuButton::Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice)
{
	*pClone = new CMenuButton();
	Copy(*pClone, pD3DDevice);
}

void CMenuButton::Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice)
{
	//Copy base properties
	CMenuVisual::Copy(pCopy, pD3DDevice);

	//Copy Button settings
	CMenuButton *buttonCopy = (CMenuButton*)pCopy;

	//Visible Objects
	for(int i = 0; i < MENU_BUTTONSTATE_MAX; i++)
		for(int j = 0; j < MENU_BUTTONIMAGE_MAX; j++)
			if(m_stateImages[i][j])
				m_stateImages[i][j]->Clone((CMenuVisual**)&buttonCopy->m_stateImages[i][j], pD3DDevice);

	// Copy the button text
	if (this->m_pDisplayText)
		this->m_pDisplayText->Clone((CMenuVisual**)&buttonCopy->m_pDisplayText, pD3DDevice);

	//Settable Parameters
	buttonCopy->m_visibleState = m_visibleState;

	//scripts
	if(m_clickScript)
	   m_clickScript->Clone(&buttonCopy->m_clickScript);

	if(m_mouseOverScript)
	   m_mouseOverScript->Clone(&buttonCopy->m_mouseOverScript);

	if(m_mouseOutScript)
	   m_mouseOutScript->Clone(&buttonCopy->m_mouseOutScript);

	if(m_dropScript)
	   m_dropScript->Clone(&buttonCopy->m_dropScript);
}

CMenuButton::~CMenuButton()
{
	DeleteMemory();
}

void CMenuButton::DeleteMemory()
{
	for(int i = 0; i < MENU_BUTTONSTATE_MAX; i++)
	{
		for(int j = 0; j < MENU_BUTTONIMAGE_MAX; j++)
		{
			if(m_stateImages[i][j])
			{
				delete m_stateImages[i][j];
				m_stateImages[i][j] = NULL;
			}
		}
	}

	//Delete all the scripts
	if(m_clickScript)
	{
		delete m_clickScript;
		m_clickScript = NULL;
	}
	if(m_mouseOverScript)
	{
		delete m_mouseOverScript;
		m_mouseOverScript = NULL;
	}
	if(m_mouseOutScript)
	{
		delete m_mouseOutScript;
		m_mouseOutScript = NULL;
	}
	if(m_dropScript)
	{
		delete m_dropScript;
		m_dropScript = NULL;
	}

	SafeDelete(m_pDisplayText);

}

void CMenuButton::Serialize(CXMLFileIO &xml)
{
	CMenuVisual::Serialize(xml);

	if(xml.m_isStoring)
	{
		xml.WriteAttribute("Version", 2);
		if (this->m_pDisplayText)
		{
			xml.WriteAttribute("DisplayText", this->m_pDisplayText->m_text.GetBuffer());
			this->m_pDisplayText->m_text.ReleaseBuffer();
		}

		//Write out the click script
		if(m_clickScript)
		if(!m_clickScript->m_scriptText.IsEmpty())
		{
			xml.WriteOpeningTag("ClickScript");
			xml.WriteMultilinedAttribute(m_clickScript->m_scriptText.GetBuffer());
			xml.WriteClosingTag("ClickScript");
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

		//Write out all the state images
		for(int i = 0; i < MENU_BUTTONSTATE_MAX; i++)
		{
			for(int j = 0; j < MENU_BUTTONIMAGE_MAX; j++)
			{
				if(m_stateImages[i][j])
				{
					xml.WriteOpeningTag("StateImage");
					xml.WriteAttribute("StateID", i);
					xml.WriteAttribute("PositionID", j);
					m_stateImages[i][j]->Serialize(xml);
					xml.WriteClosingTag("StateImage");
				}
			}
		}
	}
	else
	{
		bool closing;
		char buffer[MAX_BUFFER];
		int  version;

		xml.ReadAttribute(version);

		if (version >= 2) 
		{		
			SafeDelete(this->m_pDisplayText);
			this->InitText();
			LPTSTR strDisplay = new TCHAR[MAX_BUFFER];
			xml.ReadAttribute(strDisplay, MAX_BUFFER);		
			this->SetText(strDisplay);
			SafeDeleteArray(strDisplay);
		}

		//Read in <ClickScript>, <HoverScript>, <DropScript>, or the first <StateImage>
		xml.ReadTag(buffer, MAX_BUFFER, closing);
		if(strcmp(buffer, "ClickScript") == 0)
		{
			//We've got a click script, create a new script instance
			SafeDelete(m_clickScript);
			m_clickScript = new CMenuScript();

			while(1)
			{
				//Read next script line or </ClickScript>
				xml.ReadLineOfAttribute(buffer, MAX_BUFFER);

				//If it is </ClickScript>, break
				if(buffer[0] == '<')
					break;

				//Add the line to the script
				m_clickScript->AddLine(buffer);
			}

			//Read in <HoverScript>, <DropScript>, or the first <StateImage>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		//If the MouseOverScript is present
		if(strcmp(buffer, "MouseOverScript") == 0)
		{
			//We've got a mouseOver script, create a new script instance
			SafeDelete(m_mouseOverScript);
			m_mouseOverScript = new CMenuScript();

			while(1)
			{
				//Read next script line or </MouseOverScript>
				xml.ReadLineOfAttribute(buffer, MAX_BUFFER);

				//If it is </MouseOverScript>, break
				if(buffer[0] == '<')
					break;

				//Add the line to the script
				m_mouseOverScript->AddLine(buffer);
			}

			//Read in <DropScript> or the first <StateImage>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		//If the MouseOutScript is present
		if(strcmp(buffer, "MouseOutScript") == 0)
		{
			//We've got a mouseOut script, create a new script instance
			SafeDelete(m_mouseOutScript);
			m_mouseOutScript = new CMenuScript();

			while(1)
			{
				//Read next script line or </MouseOutScript>
				xml.ReadLineOfAttribute(buffer, MAX_BUFFER);

				//If it is </MouseOutScript>, break
				if(buffer[0] == '<')
					break;

				//Add the line to the script
				m_mouseOutScript->AddLine(buffer);
			}

			//Read in <DropScript> or the first <StateImage>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		//If the DropScript is present
		if(strcmp(buffer, "DropScript") == 0)
		{
			//We've got a drop script, create a new script instance
			SafeDelete(m_dropScript);
			m_dropScript = new CMenuScript();

			while(1)
			{
				//Read next script line or </DropScript>
				xml.ReadLineOfAttribute(buffer, MAX_BUFFER);

				//If it is </DropScript>, break
				if(buffer[0] == '<')
					break;

				//Add the line to the script
				m_dropScript->AddLine(buffer);
			}

			//Read in the first <StateImage>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		//Read in all the state images
		int curState, curPosition;
		while(!closing)
		{
			//Read in this state image
			xml.ReadAttribute(curState);
			xml.ReadAttribute(curPosition);
			
			SafeDelete(m_stateImages[curState][curPosition]);
			m_stateImages[curState][curPosition] = new CMenuImage();
			m_stateImages[curState][curPosition]->Serialize(xml);
			//The </StateImage> is read in by the image

			//Read the next <StateImage> or </Button>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		m_visibleState = MENU_BUTTONSTATE_UP;
	}
}
