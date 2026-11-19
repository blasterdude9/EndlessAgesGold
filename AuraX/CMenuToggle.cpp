#include "stdafx.h"
#include "direct.h"
#include "CMenuToggle.h"
#include "CMenuScript.h"
#include "GlobalStatusVariables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern GlobalStatusVariables *globalStatus;

CMenuToggle::CMenuToggle()
{
	//Visible Objects
	m_stateImages[MENU_TOGGLESTATE_UP]			= new CMenuImage();
	m_stateImages[MENU_TOGGLESTATE_UPHOVER]		= NULL;
	m_stateImages[MENU_TOGGLESTATE_DOWN]		= new CMenuImage();
	m_stateImages[MENU_TOGGLESTATE_DOWNHOVER]	= NULL;

	//Settable Parameters
	m_iGroupID				= 0;
	m_isSet					= false;
	//m_strValue				= new TCHAR[MAX_BUFFER];
	m_pDisplayText			= NULL;
	this->InitText();
	m_pData					= NULL;

	//Engine Variables
	m_state				= MENU_TOGGLESTATE_UP;
	m_mousedOver		= false;
	m_mousedDown		= false;

	//Scripts
	m_clickScript		= NULL;
	m_mouseOverScript	= NULL;
	m_mouseOutScript	= NULL;
}

void
CMenuToggle::InitText()
{
	SafeDelete(m_pDisplayText);
	m_pDisplayText = new CMenuText();
	m_pDisplayText->m_font = globalStatus->m_menuFontList->GetPointerFromIndex(0);
	m_pDisplayText->Init(globalStatus->m_3dDevice);
}

void
CMenuToggle::SetText(LPTSTR value)
{
	if (m_pDisplayText)
		m_pDisplayText->SetText(value);
}

void 
CMenuToggle::SetEnabled(bool bEnable)
{
	// Set control enabled state
	this->m_enabled = bEnable;

	// Change control text to mirror enabled state
	if (bEnable)
	{
		// Restore the text
		if (this->m_pDisplayText)
		{
			// Set associated text to enabled color
			this->m_pDisplayText->m_red = 1.0f;
			this->m_pDisplayText->m_green = 1.0f;
			this->m_pDisplayText->m_blue = 0.0f;
			this->m_pDisplayText->m_alpha = 1.0f;
		}
	}
	else
	{
		// Set control selected state
		this->m_state = MENU_TOGGLESTATE_UP;
		this->m_isSet = false;

		// Set associated text to disabled color
		if (this->m_pDisplayText)
		{
			this->m_pDisplayText->m_red = 1.0f;
			this->m_pDisplayText->m_green = 1.0f;
			this->m_pDisplayText->m_blue = 1.0f;
			this->m_pDisplayText->m_alpha = 0.5f;
		}
	}

	// Refresh CMenuText mesh
	if (this->m_pDisplayText->m_text != "")
		this->m_pDisplayText->SetText(this->m_pDisplayText->m_text.GetBuffer());
}

/*bool CMenuToggle::HandleInput(CInput *pInput, float parentX, float parentY, DWORD curTime, LPDIRECT3DDEVICE9 pD3DDevice)
{
	//The toggle is now the parent
	parentX += m_posX;
	parentY += m_posY;

	//If the mouse is currently inside the toggle's borders
	if(	pInput->m_mouseAbsPosX > parentX && pInput->m_mouseAbsPosX < parentX + m_width &&
		pInput->m_mouseAbsPosY > parentY && pInput->m_mouseAbsPosY < parentY + m_height)
	{
		//If the mouse was just pressed
		if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustPressed())
		{
			m_mousedDown = true;
			return true;
		}
		else if(m_mousedDown)
		{
			//If they just released the mouse button after clicking on the toggle
			if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustReleased())
			{
				//Inverse the toggle's state
				m_isSet = !m_isSet;

				//Get ready for the next click
				m_mousedDown = false;
			}

			//Inverse get the new state image
			if(m_isSet)
			{
				if(m_stateImages[MENU_TOGGLESTATE_DOWNHOVER])
					m_state = MENU_TOGGLESTATE_DOWNHOVER;
				else
					m_state = MENU_TOGGLESTATE_DOWN;
			}
			else
			{
				if(m_stateImages[MENU_TOGGLESTATE_UPHOVER])
					m_state = MENU_TOGGLESTATE_UPHOVER;
				else
					m_state = MENU_TOGGLESTATE_UP;
			}

			//If we have a click script, execute it
			if(m_clickScript)
				m_clickScript->ExecuteScript();

			return true;
		}
		//If the mouse isn't down
		else if(!pInput->m_mouseButtons[MOUSE_LEFTBUTTON].IsDown())
		{
			//Set the hover state based on the current state
			if(m_stateImages[MENU_TOGGLESTATE_UPHOVER] && m_state == MENU_TOGGLESTATE_UP)
				m_state = MENU_TOGGLESTATE_UPHOVER;
			else if(m_stateImages[MENU_TOGGLESTATE_DOWNHOVER] && m_state == MENU_TOGGLESTATE_DOWN)
				m_state = MENU_TOGGLESTATE_DOWNHOVER;

			//Continuously execute the mouse over script(allows for pausing)
			if(m_mouseOverScript)
				m_mouseOverScript->ExecuteScript();

			//Allow the mouse out script to run
			m_mousedOver = true;
		}
	}
	else
	{
		//If they released the mouse, we're no longer moused down
		if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustReleased())
			m_mousedDown = false;

		//If the mouse is outside, set the state to Up or Down based on isSet
		if(m_isSet)
			m_state = MENU_TOGGLESTATE_DOWN;
		else
			m_state = MENU_TOGGLESTATE_UP;

		//If the mouse was hovered over the toggle
		if(m_mousedOver)
		{
			//This is only executed once
			if(m_mouseOutScript)
				m_mouseOutScript->ExecuteScript();

			//We're no longer hovering over the toggle
			m_mousedOver = false;
		}
	}

	return false;
}*/


bool CMenuToggle::HandleInput(CInput *pInput, float parentX, float parentY, DWORD curTime, LPDIRECT3DDEVICE9 pD3DDevice)
{
	bool bHandled = false;

	//The toggle is now the parent
	parentX += m_posX;
	parentY += m_posY;

	//If the mouse is currently inside the toggle's borders
	if(	pInput->m_mouseAbsPosX > parentX && pInput->m_mouseAbsPosX < parentX + m_width &&
		pInput->m_mouseAbsPosY > parentY && pInput->m_mouseAbsPosY < parentY + m_height)
	{
		//If the mouse was just pressed
		if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustPressed())
		{
			//Inverse the toggle's state
			m_isSet = !m_isSet;

			m_mousedDown = true;
			bHandled = true;

			//If we have a click script, execute it
			if(m_clickScript)
				m_clickScript->ExecuteScript();
		}
		//If they just released the mouse button after clicking on the toggle
		else if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustReleased())
		{
			//Get ready for the next click
			m_mousedDown = false;
			bHandled = true;
		}

		// Update the new state image
		if(m_isSet)
		{
			if(m_stateImages[MENU_TOGGLESTATE_DOWNHOVER])
				m_state = MENU_TOGGLESTATE_DOWNHOVER;
			else
				m_state = MENU_TOGGLESTATE_DOWN;
		}
		else
		{
			if(m_stateImages[MENU_TOGGLESTATE_UPHOVER])
				m_state = MENU_TOGGLESTATE_UPHOVER;
			else
				m_state = MENU_TOGGLESTATE_UP;
		}

		//If the mouse isn't down
		if(!m_mousedDown)
		{
			//Set the hover state based on the current state
			if(m_stateImages[MENU_TOGGLESTATE_UPHOVER] && m_state == MENU_TOGGLESTATE_UP)
				m_state = MENU_TOGGLESTATE_UPHOVER;
			else if(m_stateImages[MENU_TOGGLESTATE_DOWNHOVER] && m_state == MENU_TOGGLESTATE_DOWN)
				m_state = MENU_TOGGLESTATE_DOWNHOVER;

			//Continuously execute the mouse over script(allows for pausing)
			if(m_mouseOverScript)
				m_mouseOverScript->ExecuteScript();

			//Allow the mouse out script to run
			m_mousedOver = true;
		}

		return bHandled;
	}
	else
	{
		//If they released the mouse, we're no longer moused down
		if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustReleased())
			m_mousedDown = false;

		//If the mouse is outside, set the state to Up or Down based on isSet
		if(m_isSet)
			m_state = MENU_TOGGLESTATE_DOWN;
		else
			m_state = MENU_TOGGLESTATE_UP;

		//If the mouse was hovered over the toggle
		if(m_mousedOver)
		{
			//This is only executed once
			if(m_mouseOutScript)
				m_mouseOutScript->ExecuteScript();

			//We're no longer hovering over the toggle
			m_mousedOver = false;
		}
	}

	return false;
}

void CMenuToggle::Render(LPDIRECT3DDEVICE9 pD3DDevice,
						 CStateManagementObj *stateManager, 
						 CTextureDBObjList *textureDatabase,
						 float parentX,
						 float parentY)
{
	// Render the current state
	m_stateImages[m_state]->Render(pD3DDevice, 
		stateManager, 
		textureDatabase, 
		parentX + m_posX, 
		parentY + m_posY);

	// Render the associated text
	// NOTE: The position of toggle display text is hardcoded
	// in the params sent to the CMenuText's Render() function.
	if (this->m_pDisplayText 
	&& this->m_pDisplayText->m_text != "")
	{
		this->m_pDisplayText->Render(
			pD3DDevice, 
			stateManager, 
			textureDatabase, 
			parentX + m_posX + this->m_width + 5.0f, 
			parentY + m_posY + (this->m_height / 2.0f) - 5.0f);
	}
}

void CMenuToggle::Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice)
{
	*pClone = new CMenuToggle();
	Copy(*pClone, pD3DDevice);
}

void CMenuToggle::Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice)
{
	//Copy base properties
	CMenuVisual::Copy(pCopy, pD3DDevice);

	//Copy Progress Bar settings
	CMenuToggle *toggleCopy = (CMenuToggle*)pCopy;

	//Visible Objects
	for(int i = 0; i < MENU_TOGGLESTATE_MAX; i++)
		if(m_stateImages[i])
			m_stateImages[i]->Clone((CMenuVisual**)&toggleCopy->m_stateImages[i], pD3DDevice);

	//Settable Parameters
	toggleCopy->m_isSet = m_isSet;
	toggleCopy->m_strValue = m_strValue;
	toggleCopy->m_state	= m_state;
}

CMenuToggle::~CMenuToggle()
{
	for(int i = 0; i < MENU_TOGGLESTATE_MAX; i++)
	{
		/*if(m_stateImages[i])
		{
			delete [] m_stateImages[i];
			m_stateImages[i] = NULL;
		}*/
		SafeDelete(m_stateImages[i]);
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

	SafeDelete(m_pDisplayText);
}

void CMenuToggle::Serialize(CXMLFileIO &xml)
{
	CMenuVisual::Serialize(xml);

	if(xml.m_isStoring)
	{
		xml.WriteAttribute("Version", 2);
		xml.WriteAttribute("GroupID", (int)m_iGroupID);
		xml.WriteAttribute("Value", m_strValue.GetBuffer());
		m_strValue.ReleaseBuffer();
		if (m_pDisplayText) 
		{
			xml.WriteAttribute("DisplayText", m_pDisplayText->m_text.GetBuffer());
			m_pDisplayText->m_text.ReleaseBuffer();
		}
		xml.WriteAttribute("StartDown", m_isSet);

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

		//Write out all the state images
		for(int i = 0; i < MENU_TOGGLESTATE_MAX; i++)
		{
			if(m_stateImages[i])
			{
				xml.WriteOpeningTag("StateImage");
				xml.WriteAttribute("StateID", i);
				m_stateImages[i]->Serialize(xml);
				xml.WriteClosingTag("StateImage");
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
			xml.ReadAttribute(m_iGroupID);

			this->m_strValue.Empty();
			LPTSTR strValue = this->m_strValue.GetBuffer(MAX_BUFFER);
			xml.ReadAttribute(strValue, MAX_BUFFER);
			this->m_strValue.ReleaseBuffer();
		
			SafeDelete(this->m_pDisplayText);
			this->InitText();
			
			if (!this->m_pDisplayText)
				this->m_pDisplayText = new CMenuText();
			strValue = this->m_pDisplayText->m_text.GetBuffer(MAX_BUFFER);
			xml.ReadAttribute(strValue, MAX_BUFFER);		
			this->SetText(strValue);
			this->m_pDisplayText->m_text.ReleaseBuffer();
		}
		xml.ReadAttribute(m_isSet);

		//Set starting state based on m_isSet
		if(m_isSet)
			m_state = MENU_TOGGLESTATE_DOWN;
		else
			m_state = MENU_TOGGLESTATE_UP;

		//Read in <ClickScript> or the first <StateImage>
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

			//Read in the first <StateImage>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		//If the MouseOverScript is present
		if(strcmp(buffer, "MouseOverScript") == 0)
		{
			//We've got a mouseOver script, create a new script instance
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

		//Read in all the state images
		int curState;
		while(!closing)
		{
			xml.ReadAttribute(curState);
            if(m_stateImages[curState])
               delete m_stateImages[curState];
			m_stateImages[curState] = 0;

			m_stateImages[curState] = new CMenuImage();
			m_stateImages[curState]->Serialize(xml);
			//The </StateImage> is read in by the image

			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read the next <StateImage> or </Toggle>
		}
	}
}
