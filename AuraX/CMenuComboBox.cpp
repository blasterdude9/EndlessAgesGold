#include "stdafx.h"
#include "direct.h"
#include "CMenuComboBox.h"
#include "CMenuScript.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMenuComboBox::CMenuComboBox()
{
	//Visible Objects
	m_textBox.m_linesOfText = new CMenuText[1];

	//Engine Variables
	m_listBoxShown			= false;

	//Scripts
	m_selectionChangeScript	= NULL;
	m_mouseOverScript		= NULL;
	m_mouseOutScript		= NULL;
}

void CMenuComboBox::AddItem(CMenuListItem *newItem, int beforeIndex)
{
	//Done this way for possible later additions(like default selections and such)
	m_listBox.AddItem(newItem, beforeIndex);
}

void CMenuComboBox::RemoveItem(int index)
{
	//Done this way for possible later additions(like changing to next text item if the removed one was selected)
	m_listBox.RemoveItem(index);
}

bool CMenuComboBox::HandleInput(CInput *pInput, float parentX, float parentY, DWORD curTime, LPDIRECT3DDEVICE9 pD3DDevice)
{
	//The combo box is the parent now
	parentX += m_posX;
	parentY += m_posY;

	if(m_button.HandleInput(pInput, parentX, parentY, curTime, pD3DDevice))
		return true;

	if(m_textBox.HandleInput(pInput, parentX, parentY, curTime, pD3DDevice))
		return true;

	//If they clicked the button, toggle the list box
	if(m_button.m_buttonState.JustReleased())
		m_listBoxShown = !m_listBoxShown;

	if(m_listBoxShown && m_listBox.HandleInput(pInput, parentX, parentY, curTime, pD3DDevice))
	{
		//Update the text boxes text if they have selected an item
		if(m_listBox.m_itemJustSelected && m_listBox.m_currentSelection)
		{
			m_listBoxShown = false;
			m_textBox.SetText(((CMenuText*)m_listBox.m_currentSelection->m_textList.GetHead())->m_text.GetBuffer());
		}

		return true;
	}
	else if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustPressed())
		//If they click somwhere else, hide the list box
		m_listBoxShown = false;

	return false;
}

void CMenuComboBox::Render(	LPDIRECT3DDEVICE9 pD3DDevice,
							CStateManagementObj *stateManager, 
							CTextureDBObjList *textureDatabase,
							float parentX,
							float parentY)
{
	//The combo box is the parent now
	parentX += m_posX;
	parentY += m_posY;

	//Render the components
	m_textBox.Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);
	m_button.Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	//If the list box should be rendered
	if(m_listBoxShown)
		m_listBox.Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);
}

void CMenuComboBox::Resize()
{
	//Resize the text box
	m_textBox.m_width	= m_width;
	m_textBox.m_height	= m_height;
	m_textBox.Resize();

	//Resize the list box
	m_listBox.m_width	= m_width;
	m_listBox.m_posY	= -m_listBox.m_height;
	m_listBox.Resize();
}

void CMenuComboBox::Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice)
{
	*pClone = new CMenuListBox();
	Copy(*pClone, pD3DDevice);
}

void CMenuComboBox::Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice)
{
	//Copy base properties
	CMenuVisual::Copy(pCopy, pD3DDevice);

	//Copy list box settings
	CMenuComboBox *comboBoxCopy = (CMenuComboBox*)pCopy;

	//Visual Components
	m_textBox.Copy((CMenuVisual*)&comboBoxCopy->m_textBox, pD3DDevice);
	m_button.Copy((CMenuVisual*)&comboBoxCopy->m_button, pD3DDevice);
	m_listBox.Copy((CMenuVisual*)&comboBoxCopy->m_listBox, pD3DDevice);
	
	//Engine Variables
	comboBoxCopy->m_listBoxShown = false;

	//Scripts
	comboBoxCopy->m_selectionChangeScript	= m_selectionChangeScript;
	comboBoxCopy->m_mouseOverScript			= m_mouseOverScript;
	comboBoxCopy->m_mouseOutScript			= m_mouseOutScript;
}

void CMenuComboBox::SafeDelete()
{
	//Safe delete visual components
	m_button.DeleteMemory();
	m_listBox.DeleteMemory();

	//Delete scripts
	if(m_selectionChangeScript)
	{
		delete m_selectionChangeScript;
		m_selectionChangeScript = NULL;
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
}

void CMenuComboBox::Serialize(CXMLFileIO &xml)
{
	CMenuVisual::Serialize(xml);

	if(xml.m_isStoring)
	{
		//Write out the version
		xml.WriteAttribute("Version", 1);

		//Clear out the unnecessary child script pointers
		m_textBox.m_mouseOverScript			= NULL;
		m_textBox.m_mouseOutScript			= NULL;
		m_button.m_mouseOverScript			= NULL;
		m_button.m_mouseOutScript			= NULL;
		m_listBox.m_selectionChangeScript	= NULL;
		m_listBox.m_mouseOverScript			= NULL;
		m_listBox.m_mouseOutScript			= NULL;

		xml.WriteOpeningTag("TextBox");
		m_textBox.Serialize(xml);
		xml.WriteClosingTag("TextBox");

		xml.WriteOpeningTag("Button");
		m_button.Serialize(xml);
		xml.WriteClosingTag("Button");

		xml.WriteOpeningTag("ListBox");
		m_listBox.Serialize(xml);
		xml.WriteClosingTag("ListBox");

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
	}
	else
	{
		char buffer[MAX_BUFFER];
		bool closing;
		int  version;

		//Read in the version
		xml.ReadAttribute(version);

		//Read in <TextBox>
		xml.ReadTag(buffer, MAX_BUFFER, closing);
		m_textBox.Serialize(xml);
		//The close tag is read in my the object

		//Read in <Button>
		xml.ReadTag(buffer, MAX_BUFFER, closing);
		m_button.Serialize(xml);
		//The close tag is read in my the object

		//Read in <ListBox>
		xml.ReadTag(buffer, MAX_BUFFER, closing);
		m_listBox.Serialize(xml);
		//The close tag is read in my the object

		//Read <SelectionChangeScript>, <MouseOverScript>, <MouseOutScript>, or </ComboBox>
		xml.ReadTag(buffer, MAX_BUFFER, closing);

		//If this is the selection change script
		if(strcmp(buffer, "SelectionChangeScript") == 0)
		{
			//We've got a load script, create a new script instance
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

			//Read in <MouseOverScript>, <MouseOutScript>, or </ComboBox>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		//If this is the mouse over script
		if(strcmp(buffer, "MouseOverScript") == 0)
		{
			//We've got a load script, create a new script instance
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

			//Read in <MouseOutScript> or </ComboBox>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		//If this is the mouse out script
		if(strcmp(buffer, "MouseOutScript") == 0)
		{
			//We've got a load script, create a new script instance
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

			//Read in </ComboBox>
			xml.ReadTag(buffer, MAX_BUFFER, closing);
		}

		//Point all children to the scripts
		m_textBox.m_mouseOverScript			= m_mouseOverScript;
		m_textBox.m_mouseOutScript			= m_mouseOutScript;
		m_button.m_mouseOverScript			= m_mouseOverScript;
		m_button.m_mouseOutScript			= m_mouseOutScript;
		m_listBox.m_selectionChangeScript	= m_selectionChangeScript;
		m_listBox.m_mouseOverScript			= m_mouseOverScript;
		m_listBox.m_mouseOutScript			= m_mouseOutScript;
	}
}
