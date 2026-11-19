#include "stdafx.h"
#include "direct.h"
#include "CMenuScrollBar.h"
#include "CMenuScript.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMenuScrollBar::CMenuScrollBar()
{
	//Visibile Items
	m_backgroundLeft	= NULL;
	m_backgroundMiddle	= NULL;
	m_backgroundRight	= NULL;
	m_sliderLeft		= NULL;
	m_sliderMiddle		= NULL;
	m_sliderRight		= NULL;
	m_leftButton		= NULL;
	m_rightButton		= NULL;

	//Settable Parameters
	m_isVertical	= true;
	m_numItems		= 1;
	m_visibleItems	= 1;

	//Engine Variables
	m_isDragging	= false;
	m_sliderSize	= 0.0f;
	m_sliderBegin	= 0.0f;
	m_fullSize		= 0.0f;
	m_curFirstItem	= 0;
	m_curPosition	= 0.0f;
	m_mousedOver	= false;

	//Scripts
	m_changeScript		= NULL;
	m_mouseOverScript	= NULL;
	m_mouseOutScript	= NULL;
}

bool CMenuScrollBar::HandleInput(CInput *pInput, float parentX, float parentY, DWORD curTime, LPDIRECT3DDEVICE9 pD3DDevice)
{
	//The scroll bar is now the parent
	parentX += m_posX;
	parentY += m_posY;

	//If we're dragging the slider, handle that first
	if(m_isDragging)
	{
		//While the mouse is down, we continue dragging
		if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].IsDown())
		{
			//If it's vertical, we drag along the Y axis, otherwise use the X axis
			if(m_isVertical)
				m_curPosition -= pInput->m_menuMouseChangeY;
			else
				m_curPosition += pInput->m_menuMouseChangeX;

			Update(); //Update the slider's components' positions
			return true;
		}

		//If the mouse isn't down, we're no longer dragging
		m_isDragging = false;
		return false;
	}

	//If the user just clicked the left mouse button
	if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustPressed())
	{
		//Check to see if the user clicked on the slider
		if(m_isVertical)
		{
			if(	pInput->m_mouseAbsPosX > parentX &&
				pInput->m_mouseAbsPosX < parentX + m_width &&
				pInput->m_mouseAbsPosY > parentY + m_sliderBegin &&
				pInput->m_mouseAbsPosY < parentY + m_sliderBegin + m_sliderSize)
			{
				m_isDragging = true;
				return true;
			}
		}
		else
		{
			if(	pInput->m_mouseAbsPosY > parentY &&
				pInput->m_mouseAbsPosY < parentY + m_height && 
				pInput->m_mouseAbsPosX > parentX + m_sliderBegin &&
				pInput->m_mouseAbsPosX < parentX + m_sliderBegin + m_sliderSize)
			{
				m_isDragging = true;
				return true;
			}
		}
	}

	//If there's a left button, let it try to handle the input
	if(m_leftButton)
	if(m_leftButton->HandleInput(pInput, parentX, parentY, curTime, pD3DDevice))
	{
		//If the button handles the input, see if we need to move the slider
		if(m_leftButton->m_buttonState.JustPressed() || m_leftButton->m_buttonState.ShouldRepeat(curTime))
		{
			m_curPosition -= ((m_fullSize - m_sliderSize) / (m_numItems - m_visibleItems));
			Update(); //Update the slider's components' positions
		}

		//We handled the input
		return true;
	}
	
	//If there's a right button, let it try to handle the input
	if(m_rightButton)
	if(m_rightButton->HandleInput(pInput, parentX, parentY, curTime, pD3DDevice))
	{
		//If the button handles the input, see if we need to move the slider
		if(m_rightButton->m_buttonState.JustPressed() || m_rightButton->m_buttonState.ShouldRepeat(curTime))
		{
			m_curPosition += ((m_fullSize - m_sliderSize) / (m_numItems - m_visibleItems));
			Update(); //Update the slider's components' positions
		}

		//We handled the input
		return true;
	}

	//If the mouse is inside of the image
	if(	pInput->m_mouseAbsPosX > parentX && pInput->m_mouseAbsPosX < parentX + m_width &&
		pInput->m_mouseAbsPosY > parentY && pInput->m_mouseAbsPosY < parentY + m_height)
	{
		//Continuously execute the script(allows for pausing)
		if(m_mouseOverScript)
			m_mouseOverScript->ExecuteScript();

		//Allow the mouse out script to run
		m_mousedOver = true;
	}
	else if(m_mousedOver)
	{
		//This is only executed once
		if(m_mouseOutScript)
			m_mouseOutScript->ExecuteScript();

		//We're no longer hovering over the list box
		m_mousedOver = false;
	}

	return false;
}

void CMenuScrollBar::Update()
{
	//If there's no middle portion for the slider, we don't need to draw the slider
	if(!m_sliderMiddle)
		return;

	//Make sure we don't go outside of the bounds of the background
	if(m_curPosition < 0)
		m_curPosition = 0;

	//Vertical must be handled seperately from horizontal
	if(m_isVertical)
	{
		//When vertical, m_sliderBegin is the bottom edge of the slider
		m_sliderBegin			 = 0.0f;
		float sliderTopHeight	 = 0.0f;
		float sliderBottomHeight = 0.0f;

		//When vertical, m_fullSize is the height
		m_fullSize = m_height;

		//Subtract out the height of the Top Button, if it exists
		if(m_leftButton)
			m_fullSize -= m_leftButton->m_height;

		//Subtract out the height of the Bottom Button, if it exists
		//Also, move m_sliderBegin up by the height
		if(m_rightButton)
		{
			m_sliderBegin	 = m_rightButton->m_height;
			m_fullSize		-= m_sliderBegin;
		}

		//Grab the height of the top component of the slider, if it exists
		if(m_sliderLeft)
			sliderTopHeight = m_sliderLeft->m_height;

		//Grab the height of the bottom component of the slider, if it exists
		if(m_sliderRight)
			sliderBottomHeight = m_sliderRight->m_height;

		//The slider's size is the number of pixels per item(m_fullSize / m_numItems)
		//times the number of visible items
		m_sliderSize = (m_fullSize / (float)(m_numItems + 2)) * (float)m_visibleItems;

		//Make sure the slider's size isn't less than the top and bottom pieces put together
		if(m_sliderSize < sliderTopHeight + sliderBottomHeight)
			m_sliderSize = sliderTopHeight + sliderBottomHeight;

		//Make sure the current position doesn't push the slider outside of its bounds
		if(m_curPosition > m_fullSize - m_sliderSize)
			m_curPosition = m_fullSize - m_sliderSize;

		//If the slider was supposed to be bigger than the bar, then truncate it
		if(m_sliderSize >= m_fullSize)
		{
			m_curFirstItem	= 0;
			m_curPosition	= 0.0f;
			m_sliderSize	= m_fullSize;
		}
		else //Otherwise, set the first item based on the current position
			m_curFirstItem = (int)(m_curPosition / ((m_fullSize - m_sliderSize) / (float)(m_numItems - m_visibleItems)));

		//Get the top and bottom coordinates for the slider up to this point
		float middleTop		= m_sliderBegin + m_fullSize - m_curPosition;
		m_sliderBegin		= middleTop - m_sliderSize;
		float middleBottom	= m_sliderBegin;

		//Set the top component's location, if it exists,
		//and lower the top coordinate for the middle component
		if(m_sliderLeft)
		{
			middleTop -= m_sliderLeft->m_height;
			m_sliderLeft->m_posY = middleTop;
		}

		//Set the bottom component's location, if it exists,
		//and raise the bottom coordinate for the middle component
		if(m_sliderRight)
		{
			m_sliderRight->m_posY = middleBottom;
			middleBottom += m_sliderRight->m_height;
		}

		//Set the middle's coordinates
		m_sliderMiddle->m_posY	 = middleBottom;
		m_sliderMiddle->m_height = middleTop - middleBottom;
	}
	else
	{
		//When horizontal, m_sliderBegin is the left edge of the slider
		m_sliderBegin			= 0.0f;
		float sliderLeftWidth	= 0.0f;
		float sliderRightWidth	= 0.0f;

		//When horizontal, m_fullSize is the width
		m_fullSize = m_width;

		//Subtract out the height of the Left Button, if it exists
		//Also, change m_sliderBegin to it's width
		if(m_leftButton)
		{
			m_fullSize -= m_leftButton->m_width;
			m_sliderBegin = m_leftButton->m_width;
		}

		//Subtract out the width of the Right Button, if it exists
		if(m_rightButton)
			m_fullSize -= m_rightButton->m_width;

		//Grab the width of the left component of the slider, if it exists
		if(m_sliderLeft)
			sliderLeftWidth = m_sliderLeft->m_width;

		//Grab the width of the right component of the slider, if it exists
		if(m_sliderRight)
			sliderRightWidth = m_sliderRight->m_width;

		//The slider's size is the number of pixels per item(m_fullSize / m_numItems)
		//times the number of visible items
		m_sliderSize = (m_fullSize / (float)m_numItems) * (float)m_visibleItems;

		//Make sure the slider's size isn't less than the left and right pieces put together
		if(m_sliderSize < sliderLeftWidth + sliderRightWidth)
			m_sliderSize = sliderLeftWidth + sliderRightWidth;

		//Make sure the current position doesn't push the slider outside of its bounds
		if(m_curPosition > m_fullSize - m_sliderSize)
			m_curPosition = m_fullSize - m_sliderSize;

		//If the slider was supposed to be bigger than the bar, then truncate it
		if(m_sliderSize >= m_fullSize)
		{
			m_curFirstItem	= 0;
			m_curPosition	= 0.0f;
			m_sliderSize	= m_fullSize;
		}
		else //Otherwise, set the first item based on the current position
			/*m_curFirstItem = m_curPosition / ((m_fullSize - m_sliderSize) / (m_numItems - m_visibleItems));*/
			m_curFirstItem = (int)(m_curPosition / ((m_fullSize - m_sliderSize) / (float)(m_numItems - m_visibleItems)));
		//WARNING: C4244 --> float to int

		//Get the left and right coordinates for the slider up to this point
		m_sliderBegin		+= m_curPosition;
		float middleLeft	 = m_sliderBegin;
		float middleRight	 = middleLeft + m_sliderSize;

		//Set the left component's location, if it exists,
		//and shift the left coordinate for the middle component
		if(m_sliderLeft)
		{
			m_sliderLeft->m_posX = middleLeft;
			middleLeft += m_sliderLeft->m_width;
		}

		//Set the right component's location, if it exists,
		//and shift the right coordinate for the middle component
		if(m_sliderRight)
		{
			middleRight -= m_sliderRight->m_width;
			m_sliderRight->m_posX = middleRight;
		}

		//Set the middle's coordinates
		m_sliderMiddle->m_posX	= middleLeft;
		m_sliderMiddle->m_width	= middleRight - middleLeft;
	}

	//Execute the change script, if it exists
	if(m_changeScript)
		m_changeScript->ExecuteScript();
}

void CMenuScrollBar::Render(LPDIRECT3DDEVICE9 pD3DDevice,
							CStateManagementObj *stateManager, 
							CTextureDBObjList *textureDatabase,
							float parentX,
							float parentY)
{
	//The scroll bar is now the parent
	parentX += m_posX;
	parentY += m_posY;

	//Simply render each component
	if(m_backgroundLeft)
		m_backgroundLeft->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	if(m_backgroundMiddle)
		m_backgroundMiddle->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	if(m_backgroundRight)
		m_backgroundRight->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	if(m_sliderLeft)
		m_sliderLeft->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	if(m_sliderMiddle)
		m_sliderMiddle->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	if(m_sliderRight)
		m_sliderRight->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	if(m_leftButton)
		m_leftButton->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	if(m_rightButton)
		m_rightButton->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);
}

void CMenuScrollBar::Resize()
{
	//We must handle vertical seperately from horizontal
	if(m_isVertical)
	{
		float bgTop		= m_height;
		float bgBottom	= 0.0f;

		//Set the slider's top component's width to the scroll bar's width
		if(m_sliderLeft)
			m_sliderLeft->m_width = m_width;

		//Set the slider's middle component's width to the scroll bar's width
		if(m_sliderMiddle)
			m_sliderMiddle->m_width = m_width;

		//Set the slider's bottom component's width to the scroll bar's width
		if(m_sliderRight)
			m_sliderRight->m_width = m_width;

		//Offset the background's bottom coordinate by the bottom button's height
		if(m_rightButton)
			bgBottom += m_rightButton->m_height;

		//Offset the background's top coordinate by the top button's height
		//Also, set the top button's Y coordinate
		if(m_leftButton)
		{
			bgTop -= m_leftButton->m_height;
			m_leftButton->m_posY = bgTop;
		}

		//If we have a top component to the background, set its coordinates
		if(m_backgroundLeft)
		{
			bgTop -= m_backgroundLeft->m_textureHeight;
			m_backgroundLeft->m_posX	= 0.0f;
			m_backgroundLeft->m_posY	= bgTop;
			m_backgroundLeft->m_width	= m_width;
			m_backgroundLeft->m_height	= m_backgroundLeft->m_textureHeight;
		}

		//If we have a bottom component to the background, set its coordinates
		if(m_backgroundRight)
		{
			m_backgroundRight->m_posX	= 0.0f;
			m_backgroundRight->m_posY	= bgBottom;
			m_backgroundRight->m_width	= m_width;
			m_backgroundRight->m_height	= m_backgroundRight->m_textureHeight;
			bgBottom += m_backgroundRight->m_height;
		}

		//If we have a middle component to the background, set its coordinates
		if(m_backgroundMiddle)
		{
			m_backgroundMiddle->m_posX	 = 0.0f;
			m_backgroundMiddle->m_posY	 = bgBottom;
			m_backgroundMiddle->m_width	 = m_width;
			m_backgroundMiddle->m_height = bgTop - bgBottom;
		}
	}
	else
	{
		float bgLeft	= 0.0f;
		float bgRight	= m_width;

		//Set the slider's left component's height to the scroll bar's height
		if(m_sliderLeft)
			m_sliderLeft->m_height = m_height;

		//Set the slider's middle component's height to the scroll bar's height
		if(m_sliderMiddle)
			m_sliderMiddle->m_height = m_height;

		//Set the slider's right component's height to the scroll bar's height
		if(m_sliderRight)
			m_sliderRight->m_height = m_height;

		//Offset the background's left coordinate by the left button's width
		if(m_leftButton)
			bgLeft += m_leftButton->m_width;

		//Offset the background's right coordinate by the right button's width
		//Also, set the right button's X coordinate
		if(m_rightButton)
		{
			bgRight -= m_rightButton->m_width;
			m_rightButton->m_posX = bgRight;
		}

		//If we have a left component to the background, set its coordinates
		if(m_backgroundLeft)
		{
			m_backgroundLeft->m_posX	= bgLeft;
			m_backgroundLeft->m_posY	= 0.0f;
			m_backgroundLeft->m_width	= m_backgroundLeft->m_textureWidth;
			m_backgroundLeft->m_height	= m_height;
			bgLeft += m_backgroundLeft->m_width;
		}

		//If we have a right component to the background, set its coordinates
		if(m_backgroundRight)
		{
			bgRight -= m_backgroundRight->m_textureWidth;
			m_backgroundRight->m_posX	= bgRight;
			m_backgroundRight->m_posY	= 0.0f;
			m_backgroundRight->m_width	= m_backgroundLeft->m_textureWidth;
			m_backgroundRight->m_height	= m_height;
		}

		//If we have a middle component to the background, set its coordinates
		if(m_backgroundMiddle)
		{
			m_backgroundMiddle->m_posX	 = bgLeft;
			m_backgroundMiddle->m_posY	 = 0.0f;
			m_backgroundMiddle->m_width	 = bgRight - bgLeft;
			m_backgroundMiddle->m_height = m_height;
		}
	}

	Update();
}

void CMenuScrollBar::Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice)
{
	*pClone = new CMenuScrollBar();
	Copy(*pClone, pD3DDevice);
}

void CMenuScrollBar::Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice)
{
	//Copy base properties
	CMenuVisual::Copy(pCopy, pD3DDevice);

	//Copy Progress Bar settings
	CMenuScrollBar *scrollBarCopy = (CMenuScrollBar*)pCopy;

	//Visible Objects
	if(m_backgroundLeft)
		m_backgroundLeft->Clone((CMenuVisual**)&scrollBarCopy->m_backgroundLeft, pD3DDevice);

	if(m_backgroundMiddle)
		m_backgroundMiddle->Clone((CMenuVisual**)&scrollBarCopy->m_backgroundMiddle, pD3DDevice);

	if(m_backgroundRight)
		m_backgroundRight->Clone((CMenuVisual**)&scrollBarCopy->m_backgroundRight, pD3DDevice);

	if(m_sliderLeft)
		m_sliderLeft->Clone((CMenuVisual**)&scrollBarCopy->m_sliderLeft, pD3DDevice);

	if(m_sliderMiddle)
		m_sliderMiddle->Clone((CMenuVisual**)&scrollBarCopy->m_sliderMiddle, pD3DDevice);

	if(m_sliderRight)
		m_sliderRight->Clone((CMenuVisual**)&scrollBarCopy->m_sliderRight, pD3DDevice);

	if(m_leftButton)
		m_leftButton->Clone((CMenuVisual**)&scrollBarCopy->m_leftButton, pD3DDevice);

	if(m_rightButton)
		m_rightButton->Clone((CMenuVisual**)&scrollBarCopy->m_rightButton, pD3DDevice);

	//Settable Parameters
	scrollBarCopy->m_isVertical		= m_isVertical;
	scrollBarCopy->m_numItems		= m_numItems;
	scrollBarCopy->m_visibleItems	= m_visibleItems;

	//Engine Variables
	scrollBarCopy->m_curFirstItem		= m_curFirstItem;
	scrollBarCopy->m_curPosition	= m_curPosition;
}

CMenuScrollBar::~CMenuScrollBar()
{
	//Delete every visual component
	if(m_backgroundLeft)
	{
		delete m_backgroundLeft;
		m_backgroundLeft = NULL;
	}
	if(m_backgroundMiddle)
	{
		delete m_backgroundMiddle;
		m_backgroundMiddle = NULL;
	}
	if(m_backgroundRight)
	{
		delete m_backgroundRight;
		m_backgroundRight = NULL;
	}
	if(m_sliderLeft)
	{
		delete m_sliderLeft;
		m_sliderLeft = NULL;
	}
	if(m_sliderMiddle)
	{
		delete m_sliderMiddle;
		m_sliderMiddle = NULL;
	}
	if(m_sliderRight)
	{
		delete m_sliderRight;
		m_sliderRight = NULL;
	}
	if(m_leftButton)
	{
		delete m_leftButton;
		m_leftButton = NULL;
	}
	if(m_rightButton)
	{
		delete m_rightButton;
		m_rightButton = NULL;
	}

	//Delete all the scripts
	if(m_changeScript)
	{
		delete m_changeScript;
		m_changeScript = NULL;
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

void CMenuScrollBar::Serialize(CXMLFileIO &xml)
{
	CMenuVisual::Serialize(xml);

	if(xml.m_isStoring)
	{
		xml.WriteAttribute("Version", 1);
		xml.WriteAttribute("IsVertical", m_isVertical);
		xml.WriteAttribute("NumItems", m_numItems);
		xml.WriteAttribute("VisibleItems", m_visibleItems);

		if(m_backgroundLeft)
		{
			xml.WriteOpeningTag("BackgroundLeft");
			m_backgroundLeft->Serialize(xml);
			xml.WriteClosingTag("BackgroundLeft");
		}

		if(m_backgroundMiddle)
		{
			xml.WriteOpeningTag("BackgroundMiddle");
			m_backgroundMiddle->Serialize(xml);
			xml.WriteClosingTag("BackgroundMiddle");
		}

		if(m_backgroundRight)
		{
			xml.WriteOpeningTag("BackgroundRight");
			m_backgroundRight->Serialize(xml);
			xml.WriteClosingTag("BackgroundRight");
		}

		if(m_sliderLeft)
		{
			xml.WriteOpeningTag("SliderLeft");
			m_sliderLeft->Serialize(xml);
			xml.WriteClosingTag("SliderLeft");
		}

		if(m_sliderMiddle)
		{
			xml.WriteOpeningTag("SliderMiddle");
			m_sliderMiddle->Serialize(xml);
			xml.WriteClosingTag("SliderMiddle");
		}

		if(m_sliderRight)
		{
			xml.WriteOpeningTag("SliderRight");
			m_sliderRight->Serialize(xml);
			xml.WriteClosingTag("SliderRight");
		}

		if(m_leftButton)
		{
			xml.WriteOpeningTag("LeftButton");
			m_leftButton->Serialize(xml);
			xml.WriteClosingTag("LeftButton");
		}

		if(m_rightButton)
		{
			xml.WriteOpeningTag("RightButton");
			m_rightButton->Serialize(xml);
			xml.WriteClosingTag("RightButton");
		}

		//Write out the change script
		if(m_changeScript)
		if(!m_changeScript->m_scriptText.IsEmpty())
		{
			xml.WriteOpeningTag("ChangeScript");
			xml.WriteMultilinedAttribute(m_changeScript->m_scriptText.GetBuffer());
			xml.WriteClosingTag("ChangeScript");
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
		bool closing;
		char buffer[MAX_BUFFER];
		int  version;

		xml.ReadAttribute(version);
		xml.ReadAttribute(m_isVertical);
		xml.ReadAttribute(m_numItems);
		xml.ReadAttribute(m_visibleItems);

		xml.ReadTag(buffer, MAX_BUFFER, closing);

		if(strcmp(buffer, "BackgroundLeft") == 0)
		{
			SafeDelete(m_backgroundLeft);
			m_backgroundLeft = new CMenuImage();
			m_backgroundLeft->Serialize(xml);
			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </ScrollBar>
		}

		if(strcmp(buffer, "BackgroundMiddle") == 0)
		{
			SafeDelete(m_backgroundMiddle);
			m_backgroundMiddle = new CMenuImage();
			m_backgroundMiddle->Serialize(xml);
			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </ScrollBar>
		}

		if(strcmp(buffer, "BackgroundRight") == 0)
		{
			SafeDelete(m_backgroundRight);
			m_backgroundRight = new CMenuImage();
			m_backgroundRight->Serialize(xml);
			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </ScrollBar>
		}

		if(strcmp(buffer, "SliderLeft") == 0)
		{
			SafeDelete(m_sliderLeft);
			m_sliderLeft = new CMenuImage();
			m_sliderLeft->Serialize(xml);
			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </ScrollBar>
		}

		if(strcmp(buffer, "SliderMiddle") == 0)
		{
			SafeDelete(m_sliderMiddle);
			m_sliderMiddle = new CMenuImage();
			m_sliderMiddle->Serialize(xml);
			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </ScrollBar>
		}

		if(strcmp(buffer, "SliderRight") == 0)
		{
			SafeDelete(m_sliderRight);
			m_sliderRight = new CMenuImage();
			m_sliderRight->Serialize(xml);
			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </ScrollBar>
		}

		if(strcmp(buffer, "LeftButton") == 0)
		{
			SafeDelete(m_leftButton);
			m_leftButton = new CMenuButton();
			m_leftButton->Serialize(xml);
			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </ScrollBar>
		}

		if(strcmp(buffer, "RightButton") == 0)
		{
			SafeDelete(m_rightButton);
			m_rightButton = new CMenuButton();
			m_rightButton->Serialize(xml);
			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </ScrollBar>
		}

		//If the ChangeScript is present
		if(strcmp(buffer, "ChangeScript") == 0)
		{
			//We've got a change script, create a new script instance
			SafeDelete(m_changeScript);
			m_changeScript = new CMenuScript();

			while(1)
			{
				//Read next script line or </ChangeScript>
				xml.ReadLineOfAttribute(buffer, MAX_BUFFER);

				//If it is </ChangeScript>, break
				if(buffer[0] == '<')
					break;

				//Add the line to the script
				m_changeScript->AddLine(buffer);
			}

			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read in </ScrollBar>
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

		m_isDragging	= false;
		m_sliderSize	= 0.0f;
		m_sliderBegin	= 0.0f;
		m_fullSize		= 0.0f;
		m_curFirstItem	= 0;
		m_curPosition	= 0.0f;
	}
}
