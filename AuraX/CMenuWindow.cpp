#include "stdafx.h"
#include <typeinfo>
#include "direct.h"
#include "CMenuWindow.h"
#include "CMenuScript.h"
#include "CMenuComboBox.h"
#include "CMenuToggle.h"
#include "CMenuProgressBar.h"
#include "GlobalStatusVariables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

extern GlobalStatusVariables *globalStatus;

CMenuWindow::CMenuWindow() : 
	m_version(2)
{
	//Visible Objects
	m_titleText			= NULL;
	m_titleLeft			= NULL;
	m_titleMiddle		= NULL;
	m_titleRight		= NULL;
	m_borderLeft		= NULL;
	m_borderTop			= NULL;
	m_borderRight		= NULL;
	m_borderBottom		= NULL;
	m_cornerTopLeft		= NULL;
	m_cornerTopRight	= NULL;
	m_cornerBottomLeft	= NULL;
	m_cornerBottomRight	= NULL;
	m_background		= NULL;

	//Settable Variables
	m_initialized			= false;
	m_backgroundInset		= 0.0f;
	m_cornerOffsetX			= 0.0f;
	m_cornerOffsetY			= 0.0f;
	m_escapable				= false;
	m_draggable				= false;
	m_dragByTitleOnly		= false;
	m_resizable				= false;
	m_stretchTitle			= false;
	m_showCursor			= true;
	m_horizontalAlignment	= -1.0f;
	m_verticalAlignment		= -1.0f;
	m_instance              = FALSE;

	//Engine Variables
	m_isDragging	 = false;
	m_titleRgnLeft	 = 0.0f;
	m_titleRgnTop	 = 0.0f;
	m_titleRgnRight	 = 0.0f;
	m_titleRgnBottom = 0.0f;

	//Scripts
	m_loadScript	= NULL;
	m_resizeScript	= NULL;
	m_closeScript	= NULL;
}

bool CMenuWindow::HandleInput(CInput *pInput, float parentX, float parentY, DWORD curTime, LPDIRECT3DDEVICE9 pD3DDevice)
{
	// If menu cursor is not showing, don't handle input
	//if(!globalStatus->m_showMenuCursor) return false;

	//If we are or were dragging, we must process this first
	if(m_isDragging)
	{
		if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].IsDown())
		{
			m_posX += pInput->m_menuMouseChangeX;
			m_posY += pInput->m_menuMouseChangeY;
			return true;
		}
		else
			m_isDragging = false;
	}

	//See if any of the children can handle the current input
	if(m_childList)
	{
		POSITION posChild = m_childList->GetHeadPosition();
		while(posChild != NULL)
		{
			CMenuVisual *curChild = dynamic_cast<CMenuVisual*>(m_childList->GetNext(posChild));
			if(curChild && curChild->m_visible && curChild->m_enabled)
			{
				if(curChild->HandleInput(pInput, parentX + m_posX, parentY + m_posY, curTime, pD3DDevice))
				{
					m_isFocused = true;
					return true;
				}
			}
		}
	}

	//If the left mouse button was just pressed, the user may be trying to drag the window
	if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustPressed())
	{
		//Find out if they clicked on the title
		bool titleClicked = pInput->m_mouseAbsPosX < parentX + m_titleRgnRight &&
							pInput->m_mouseAbsPosX > parentX + m_titleRgnLeft &&
							pInput->m_mouseAbsPosY < parentY + m_titleRgnTop &&
							pInput->m_mouseAbsPosY > parentY + m_titleRgnBottom;

		//If the mouse is inside the window's coordinates
		if(	(pInput->m_mouseAbsPosX > parentX + m_posX 
			&& pInput->m_mouseAbsPosX < parentX + m_posX + m_width 
			&& pInput->m_mouseAbsPosY > parentY + m_posY 
			&& pInput->m_mouseAbsPosY < parentY + m_posY + m_height) 
			|| titleClicked)
		{
			m_isFocused	 = true;

			if(m_draggable)
			{
				//If its not draggable only by the title or they clicked the title
				//if(!m_dragByTitleOnly || titleClicked)
					m_isDragging = true;
			}

			return true;
		}
		else
			m_isFocused = false;
	}

	return false;
}

void CMenuWindow::Render(LPDIRECT3DDEVICE9 pD3DDevice, 
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

	//Render the background if it exists
	if(m_background)
		m_background->Render(pD3DDevice, stateManager, textureDatabase, left, bottom);

	//If the borders and corners exist, render the borders and corners
	if(m_borderTop)
	{
		m_borderTop->Render(pD3DDevice, stateManager, textureDatabase, left, bottom);
		m_borderLeft->Render(pD3DDevice, stateManager, textureDatabase, left, bottom);
		m_borderBottom->Render(pD3DDevice, stateManager, textureDatabase, left, bottom);
		m_borderRight->Render(pD3DDevice, stateManager, textureDatabase, left, bottom);
		m_cornerTopLeft->Render(pD3DDevice, stateManager, textureDatabase, left, bottom);
		m_cornerTopRight->Render(pD3DDevice, stateManager, textureDatabase, left, bottom);
		m_cornerBottomLeft->Render(pD3DDevice, stateManager, textureDatabase, left, bottom);
		m_cornerBottomRight->Render(pD3DDevice, stateManager, textureDatabase, left, bottom);
	}

	//Render all the window's children
	if(m_childList)
	{
		POSITION posChild = m_childList->GetTailPosition();
		while(posChild != NULL)
		{
			CMenuVisual *curChild = (CMenuVisual*)m_childList->GetPrev(posChild);
			if(curChild && curChild->m_visible)
				curChild->Render(pD3DDevice, stateManager, textureDatabase, left, bottom);
		}
	}

	//Render title pieces, if they exist
	if(m_titleLeft)
		m_titleLeft->Render(pD3DDevice, stateManager, textureDatabase, left, bottom);
	if(m_titleMiddle)
		m_titleMiddle->Render(pD3DDevice, stateManager, textureDatabase, left, bottom);
	if(m_titleRight)
		m_titleRight->Render(pD3DDevice, stateManager, textureDatabase, left, bottom);
	if(m_titleText)
		m_titleText->Render(pD3DDevice, stateManager, textureDatabase, left, bottom);
}

void CMenuWindow::Resize()
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

	//Stretch the title if we need to
	if(m_stretchTitle)
	{
		float middleLeft	= 0.0f;
		float middleRight	= m_width;

		if(m_titleLeft)
			middleLeft += m_titleLeft->m_width;

		if(m_titleRight)
		{
			middleRight -= m_titleRight->m_width;
			m_titleRight->m_posX = middleRight;
		}

		if(m_titleMiddle && middleRight > middleLeft)
		{
			m_titleMiddle->m_posX	= middleLeft;
			m_titleMiddle->m_width	= middleRight - middleLeft;
		}
	}

	//Get the title region to test mouse clicks against
	if(m_titleLeft)
	{
		m_titleRgnLeft	 = m_posX + m_titleLeft->m_posX;
		m_titleRgnRight	 = m_titleRgnLeft + m_titleLeft->m_width;
		m_titleRgnBottom = m_posY + m_titleLeft->m_posY;
		m_titleRgnTop	 = m_titleRgnBottom + m_titleLeft->m_height;

		if(m_titleMiddle)
			m_titleRgnRight += m_titleMiddle->m_width;

		if(m_titleRight)
			m_titleRgnRight += m_titleRight->m_width;
	}
	else if(m_titleMiddle)
	{
		m_titleRgnLeft	 = m_posX + m_titleMiddle->m_posX;
		m_titleRgnRight	 = m_titleRgnLeft + m_titleMiddle->m_width;
		m_titleRgnBottom = m_posY + m_titleMiddle->m_posY;
		m_titleRgnTop	 = m_titleRgnBottom + m_titleMiddle->m_height;
	}

	//Execute the resize script, if it exists
	if(m_resizeScript)
		m_resizeScript->ExecuteScript();
}

void CMenuWindow::Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice)
{
	*pClone = new CMenuWindow();
	Copy(*pClone, pD3DDevice);
}

void CMenuWindow::Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice)
{
	//Copy base properties
	CMenuVisual::Copy(pCopy, pD3DDevice);

	//Copy window settings
	CMenuWindow *windowCopy = (CMenuWindow*)pCopy;

	if(m_background)
		m_background->Clone((CMenuVisual**)&windowCopy->m_background, pD3DDevice);

	if(m_borderLeft)
	{
		m_borderLeft->Clone((CMenuVisual**)&windowCopy->m_borderLeft, pD3DDevice);
		m_borderTop->Clone((CMenuVisual**)&windowCopy->m_borderTop, pD3DDevice);
		m_borderRight->Clone((CMenuVisual**)&windowCopy->m_borderRight, pD3DDevice);
		m_borderBottom->Clone((CMenuVisual**)&windowCopy->m_borderBottom, pD3DDevice);
		m_cornerTopLeft->Clone((CMenuVisual**)&windowCopy->m_cornerTopLeft, pD3DDevice);
		m_cornerTopRight->Clone((CMenuVisual**)&windowCopy->m_cornerTopRight, pD3DDevice);
		m_cornerBottomLeft->Clone((CMenuVisual**)&windowCopy->m_cornerBottomLeft, pD3DDevice);
		m_cornerBottomRight->Clone((CMenuVisual**)&windowCopy->m_cornerBottomRight, pD3DDevice);
	}

	if(m_titleLeft)
		m_titleLeft->Clone((CMenuVisual**)&windowCopy->m_titleLeft, pD3DDevice);

	if(m_titleMiddle)
		m_titleMiddle->Clone((CMenuVisual**)&windowCopy->m_titleMiddle, pD3DDevice);

	if(m_titleRight)
		m_titleRight->Clone((CMenuVisual**)&windowCopy->m_titleRight, pD3DDevice);
	
	if(m_titleText)
		m_titleText->Clone((CMenuVisual**)&windowCopy->m_titleText, pD3DDevice);

	//Settable Parameters
	//Font w/ color

	windowCopy->m_backgroundInset			= m_backgroundInset;
	windowCopy->m_escapable					= m_escapable;
	windowCopy->m_cornerOffsetX				= m_cornerOffsetX;
	windowCopy->m_cornerOffsetY				= m_cornerOffsetY;
	windowCopy->m_draggable					= m_draggable;
	windowCopy->m_dragByTitleOnly			= m_dragByTitleOnly;
	windowCopy->m_resizable					= m_resizable;
	windowCopy->m_stretchTitle				= m_stretchTitle;
	windowCopy->m_showCursor				= m_showCursor;
	windowCopy->m_horizontalAlignment		= m_horizontalAlignment;
	windowCopy->m_verticalAlignment			= m_verticalAlignment;

	//
    if(m_loadScript)
       m_loadScript->Clone(&windowCopy->m_loadScript);

	if(m_resizeScript)
       m_resizeScript->Clone(&windowCopy->m_resizeScript);

	if(m_closeScript)
       m_closeScript->Clone(&windowCopy->m_closeScript);
}

CMenuWindow::~CMenuWindow()
{
	SafeDelete(m_background);
	
	SafeDelete(m_borderLeft);	
	SafeDelete(m_borderTop);	
	SafeDelete(m_borderRight);	
	SafeDelete(m_borderBottom);	
	SafeDelete(m_cornerTopLeft);	
	SafeDelete(m_cornerTopRight);
	SafeDelete(m_cornerBottomLeft);	
	SafeDelete(m_cornerBottomRight);

	SafeDelete(m_titleLeft);
	SafeDelete(m_titleMiddle);
	SafeDelete(m_titleRight);
	SafeDelete(m_titleText);
	SafeDelete(m_childList);

	SafeDelete(m_loadScript);
	SafeDelete(m_resizeScript);
	SafeDelete(m_closeScript);

	/* DEPRECATED /////////////////////////////////////////////////////////////
	if(m_background)
	{
		delete m_background;
		m_background = NULL;
	}

	if(m_borderLeft)
	{
		delete m_borderLeft;
		m_borderLeft = NULL;
	
		delete m_borderTop;
		m_borderTop = NULL;
	
		delete m_borderRight;
		m_borderRight = NULL;
	
		delete m_borderBottom;
		m_borderBottom = NULL;
	
		delete m_cornerTopLeft;
		m_cornerTopLeft = NULL;
	
		delete m_cornerTopRight;
		m_cornerTopRight = NULL;

		delete m_cornerBottomLeft;
		m_cornerBottomLeft = NULL;
	
		delete m_cornerBottomRight;
		m_cornerBottomRight = NULL;
	}

	if(m_titleLeft)
	{
		delete m_titleLeft;
		m_titleLeft = NULL;
	}

	if(m_titleMiddle)
	{
		delete m_titleMiddle;
		m_titleMiddle = NULL;
	}

	if(m_titleRight)
	{
		delete m_titleRight;
		m_titleRight = NULL;
	}

	if(m_titleText)
	{
		delete m_titleText;
		m_titleText = NULL;
	}

	if(m_childList)
	{
		delete m_childList;
		m_childList = NULL;
	}
	/////////////////////////////////////////////////////////////////////////*/
}

void CMenuWindow::Serialize(CXMLFileIO &xml)
{
	CMenuVisual::Serialize(xml);

	if(xml.m_isStoring)
	{
		xml.WriteAttribute("Version", m_version);
		xml.WriteAttribute("BackgroundInset", (int)m_backgroundInset);
		xml.WriteAttribute("CornerOffsetX", (int)m_cornerOffsetX);
		xml.WriteAttribute("CornerOffsetY", (int)m_cornerOffsetY);
		xml.WriteAttribute("Draggable", m_draggable);
		xml.WriteAttribute("Resizable", m_resizable);
		xml.WriteAttribute("Escapable", m_escapable);
		xml.WriteAttribute("StretchTitle", m_stretchTitle);
		xml.WriteAttribute("ShowCursor", m_showCursor);
		xml.WriteAttribute("HorizontalAlignment", (int)m_horizontalAlignment);
		xml.WriteAttribute("VerticalAlignment", (int)m_verticalAlignment);

		if(m_titleText)
		{
			xml.WriteOpeningTag("TitleText");
			m_titleText->Serialize(xml);
			xml.WriteClosingTag("TitleText");
		}

		if(m_titleLeft)
		{
			xml.WriteOpeningTag("TitleLeft");
			m_titleLeft->Serialize(xml);
			xml.WriteClosingTag("TitleLeft");
		}

		if(m_titleMiddle)
		{
			xml.WriteOpeningTag("TitleMiddle");
			m_titleMiddle->Serialize(xml);
			xml.WriteClosingTag("TitleMiddle");
		}

		if(m_titleRight)
		{
			xml.WriteOpeningTag("TitleRight");
			m_titleRight->Serialize(xml);
			xml.WriteClosingTag("TitleRight");
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

		if(m_background)
		{
			xml.WriteOpeningTag("Background");
			m_background->Serialize(xml);
			xml.WriteClosingTag("Background");
		}
	
		if(m_loadScript)
		if(!m_loadScript->m_scriptText.IsEmpty())
		{
			xml.WriteOpeningTag("LoadScript");
			xml.WriteMultilinedAttribute(m_loadScript->m_scriptText.GetBuffer());
			xml.WriteClosingTag("LoadScript");
		}

		//Write out the resize script
		if(m_resizeScript)
		if(!m_resizeScript->m_scriptText.IsEmpty())
		{
			xml.WriteOpeningTag("ResizeScript");
			xml.WriteMultilinedAttribute(m_resizeScript->m_scriptText.GetBuffer());
			xml.WriteClosingTag("ResizeScript");
		}

		//Write out the close script
		if(m_closeScript)
		if(!m_closeScript->m_scriptText.IsEmpty())
		{
			xml.WriteOpeningTag("CloseScript");
			xml.WriteMultilinedAttribute(m_closeScript->m_scriptText.GetBuffer());
			xml.WriteClosingTag("CloseScript");
		}

	}
	else
	{
		bool closing;
		char buffer[MAX_BUFFER];
		int  version;

		xml.ReadAttribute(version);
		xml.ReadAttribute(m_backgroundInset);
		xml.ReadAttribute(m_cornerOffsetX);
		xml.ReadAttribute(m_cornerOffsetY);
		xml.ReadAttribute(m_draggable);
		xml.ReadAttribute(m_resizable);
		if(version >= 2) xml.ReadAttribute(m_escapable);
		xml.ReadAttribute(m_stretchTitle);
		xml.ReadAttribute(m_showCursor);
		xml.ReadAttribute(m_horizontalAlignment);
		xml.ReadAttribute(m_verticalAlignment);

		xml.ReadTag(buffer, MAX_BUFFER, closing);

		if(strcmp(buffer, "TitleText") == 0)
		{
			m_titleText = new CMenuText();
			m_titleText->Serialize(xml);
			//The closing buffer gets read by the object
			
			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </Window>
		}

		if(strcmp(buffer, "TitleLeft") == 0)
		{
			m_titleLeft = new CMenuImage();
			m_titleLeft->Serialize(xml);
			//The closing buffer gets read by the object

			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </Window>
		}

		if(strcmp(buffer, "TitleMiddle") == 0)
		{
			m_titleMiddle = new CMenuImage();
			m_titleMiddle->Serialize(xml);
			//The closing buffer gets read by the object

			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </Window>
		}

		if(strcmp(buffer, "TitleRight") == 0)
		{
			m_titleRight = new CMenuImage();
			m_titleRight->Serialize(xml);
			//The closing buffer gets read by the object

			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </Window>
		}

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

			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </Window>
		}

		if(strcmp(buffer, "Background") == 0)
		{
			m_background = new CMenuImage();
			m_background->Serialize(xml);
			//The closing buffer gets read by the object

			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </Window>
		}

		//If the LoadScript is present
		if(strcmp(buffer, "LoadScript") == 0)
		{
			//We've got a load script, create a new script instance
			m_loadScript = new CMenuScript();

			while(1)
			{
				//Read next script line or </LoadScript>
				xml.ReadLineOfAttribute(buffer, MAX_BUFFER);

				//If it is </LoadScript>, break
				if(buffer[0] == '<')
					break;

				//Add the line to the script
				m_loadScript->AddLine(buffer);
			}

			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </Window>
		}

		//If the ResizeScript is present
		if(strcmp(buffer, "ResizeScript") == 0)
		{
			//We've got a resize script, create a new script instance
			m_resizeScript = new CMenuScript();

			while(1)
			{
				//Read next script line or </ResizeScript>
				xml.ReadLineOfAttribute(buffer, MAX_BUFFER);

				//If it is </ResizeScript>, break
				if(buffer[0] == '<')
					break;

				//Add the line to the script
				m_resizeScript->AddLine(buffer);
			}

			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </Window>
		}

		//If the CloseScript is present
		if(strcmp(buffer, "CloseScript") == 0)
		{
			//We've got a close script, create a new script instance
			m_closeScript = new CMenuScript();

			while(1)
			{
				//Read next script line or </CloseScript>
				xml.ReadLineOfAttribute(buffer, MAX_BUFFER);

				//If it is </CloseScript>, break
				if(buffer[0] == '<')
					break;

				//Add the line to the script
				m_closeScript->AddLine(buffer);
			}

			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </Window>
		}
		
		m_isDragging = false;
	}
}
//--------------------------------------------------------------------------//
void CMenuWindow::ReinitMenuTextures(CTextureDBObjList * textureDataBase,CMenuFontList * menuFontList,LPDIRECT3DDEVICE9 device,CMenuListItem *draggingItem)
{
//Load Window Title Font
	if(m_titleText)
	{
		//Get a pointer to the correct font
		m_titleText->m_font = menuFontList->GetFontByName(m_titleText->m_fontName);

		//Load the font texture(if its not already loaded)
		m_titleText->m_font->m_textureSelect = textureDataBase->AddAssignTempTexture(device, m_titleText->m_font->m_textureFile, 0);

		//Update the mesh object's texture select
		if(m_titleText->m_textMesh)
			m_titleText->m_textMesh->List_textureSelect[0] = m_titleText->m_font->m_textureSelect;
	}

	//Load Title Components
	if(m_titleLeft)
		m_titleLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, m_titleLeft->m_textureFile, 0));
	if(m_titleMiddle)
		m_titleMiddle->UpdateTexture(textureDataBase->AddAssignTempTexture(device, m_titleMiddle->m_textureFile, 0));
	if(m_titleRight)
		m_titleRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, m_titleRight->m_textureFile, 0));

	//Load Window Borders and Corners
	if(m_borderLeft)
	{
		m_borderLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, m_borderLeft->m_textureFile, 0));
		m_borderTop->UpdateTexture(textureDataBase->AddAssignTempTexture(device, m_borderTop->m_textureFile, 0));
		m_borderRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, m_borderRight->m_textureFile, 0));
		m_borderBottom->UpdateTexture(textureDataBase->AddAssignTempTexture(device, m_borderBottom->m_textureFile, 0));
		m_cornerTopLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, m_cornerTopLeft->m_textureFile, 0));
		m_cornerTopRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, m_cornerTopRight->m_textureFile, 0));
		m_cornerBottomLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, m_cornerBottomLeft->m_textureFile, 0));
		m_cornerBottomRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, m_cornerBottomRight->m_textureFile, 0));
	}

	//Load Window Background
	if(m_background)
		m_background->UpdateTexture(textureDataBase->AddAssignTempTexture(device, m_background->m_textureFile, 0));

	//Hand out Global Status Variables pointers to all scripts
	if(m_loadScript)
		m_loadScript->m_callingMenu = this;
	if(m_resizeScript)
		m_resizeScript->m_callingMenu	= this;
	if(m_closeScript)
		m_closeScript->m_callingMenu	= this;

	//Loop through the child list, load all textures and fonts
	if(m_childList)
	{
		POSITION posChild = m_childList->GetHeadPosition();
		while(posChild != NULL)
		{
			CMenuVisual *curChild = NULL;
			try
			{
				curChild = dynamic_cast<CMenuVisual*>(m_childList->GetNext(posChild));
			}
			catch(std::bad_typeid ex)
			{
				continue;
			}

			switch(curChild->GetType())
			{
				case MENU_VISTYPE_COMBOBOX:
				{
					CMenuComboBox *comboBox = (CMenuComboBox*)curChild;

					//Get a pointer to the correct font and load the font texture(if its not already loaded)
					if(comboBox->m_textBox.m_fontName != "")
					{
						comboBox->m_textBox.m_font = menuFontList->GetFontByName(comboBox->m_textBox.m_fontName);
						comboBox->m_listBox.m_font = comboBox->m_textBox.m_font;
						comboBox->m_listBox.m_font->m_textureSelect = textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_font->m_textureFile, 0);
						comboBox->m_listBox.UpdateFontTexture();

						//Let the text object have the font as well
						comboBox->m_textBox.m_linesOfText[0].m_font = comboBox->m_textBox.m_font;

						//Update the mesh object's texture select
						if(comboBox->m_textBox.m_linesOfText[0].m_textMesh)
							comboBox->m_textBox.m_linesOfText[0].m_textMesh->List_textureSelect[0] = comboBox->m_textBox.m_font->m_textureSelect;
					}

					//Load Background Image Textures
					if(comboBox->m_textBox.m_backgroundLeft)
						comboBox->m_textBox.m_backgroundLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_textBox.m_backgroundLeft->m_textureFile, 0));
					if(comboBox->m_textBox.m_backgroundMiddle)
						comboBox->m_textBox.m_backgroundMiddle->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_textBox.m_backgroundMiddle->m_textureFile, 0));
					if(comboBox->m_textBox.m_backgroundRight)
						comboBox->m_textBox.m_backgroundRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_textBox.m_backgroundRight->m_textureFile, 0));

					//Make sure we give comboBox->m_textBox.m_text the current string
					comboBox->m_textBox.SetText(NULL);

					//Hand out Global Status Variables pointers to all scripts
					if(comboBox->m_textBox.m_enterPressScript)
					{
						comboBox->m_textBox.m_enterPressScript->m_callingMenu	= this;
						comboBox->m_textBox.m_enterPressScript->m_callingVisual	= (CMenuVisual*)comboBox;
					}

					//Adjust the size of all images
					comboBox->m_textBox.Resize();

					//Load All State Image Textures
					for(int i = 0; i < MENU_BUTTONSTATE_MAX; i++)
						for(int j = 0; j < MENU_BUTTONIMAGE_MAX; j++)
							if(comboBox->m_button.m_stateImages[i][j])
								comboBox->m_button.m_stateImages[i][j]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_button.m_stateImages[i][j]->m_textureFile, 0));

					//Hand out Global Status Variables pointers to all scripts
					if(comboBox->m_button.m_clickScript)
					{
						comboBox->m_button.m_clickScript->m_callingMenu		= this;
						comboBox->m_button.m_clickScript->m_callingVisual	= (CMenuVisual*)comboBox;
					}

					//Adjust the size of all images
					comboBox->m_button.Resize();

					//Load Window Borders and Corners
					if(comboBox->m_listBox.m_borderLeft)
					{
						comboBox->m_listBox.m_borderLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_borderLeft->m_textureFile, 0));
						comboBox->m_listBox.m_borderTop->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_borderTop->m_textureFile, 0));
						comboBox->m_listBox.m_borderRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_borderRight->m_textureFile, 0));
						comboBox->m_listBox.m_borderBottom->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_borderBottom->m_textureFile, 0));
						comboBox->m_listBox.m_cornerTopLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_cornerTopLeft->m_textureFile, 0));
						comboBox->m_listBox.m_cornerTopRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_cornerTopRight->m_textureFile, 0));
						comboBox->m_listBox.m_cornerBottomLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_cornerBottomLeft->m_textureFile, 0));
						comboBox->m_listBox.m_cornerBottomRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_cornerBottomRight->m_textureFile, 0));
					}

					//Load List Background
					if(comboBox->m_listBox.m_background)
						comboBox->m_listBox.m_background->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_background->m_textureFile, 0));

					//Load List Item Background
					if(comboBox->m_listBox.m_itemBackground)
						comboBox->m_listBox.m_itemBackground->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_itemBackground->m_textureFile, 0));

					//Load the scroll bar
					if(comboBox->m_listBox.m_scrollBar)
					{
						//Load Background Image Textures
						if(comboBox->m_listBox.m_scrollBar->m_backgroundLeft)
							comboBox->m_listBox.m_scrollBar->m_backgroundLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_scrollBar->m_backgroundLeft->m_textureFile, 0));
						if(comboBox->m_listBox.m_scrollBar->m_backgroundMiddle)
							comboBox->m_listBox.m_scrollBar->m_backgroundMiddle->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_scrollBar->m_backgroundMiddle->m_textureFile, 0));
						if(comboBox->m_listBox.m_scrollBar->m_backgroundRight)
							comboBox->m_listBox.m_scrollBar->m_backgroundRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_scrollBar->m_backgroundRight->m_textureFile, 0));

						//Load Slider Image Textures
						if(comboBox->m_listBox.m_scrollBar->m_sliderLeft)
							comboBox->m_listBox.m_scrollBar->m_sliderLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_scrollBar->m_sliderLeft->m_textureFile, 0));
						if(comboBox->m_listBox.m_scrollBar->m_sliderMiddle)
							comboBox->m_listBox.m_scrollBar->m_sliderMiddle->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_scrollBar->m_sliderMiddle->m_textureFile, 0));
						if(comboBox->m_listBox.m_scrollBar->m_sliderRight)
							comboBox->m_listBox.m_scrollBar->m_sliderRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_scrollBar->m_sliderRight->m_textureFile, 0));

						//Load Left Button Image Textures
						if(comboBox->m_listBox.m_scrollBar->m_leftButton)
						{
							if(comboBox->m_listBox.m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE])
								comboBox->m_listBox.m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
							if(comboBox->m_listBox.m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE])
								comboBox->m_listBox.m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
							if(comboBox->m_listBox.m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE])
								comboBox->m_listBox.m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
							if(comboBox->m_listBox.m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE])
								comboBox->m_listBox.m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
						}

						//Load Right Button Image Textures
						if(comboBox->m_listBox.m_scrollBar->m_rightButton)
						{
							if(comboBox->m_listBox.m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE])
								comboBox->m_listBox.m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
							if(comboBox->m_listBox.m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE])
								comboBox->m_listBox.m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
							if(comboBox->m_listBox.m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE])
								comboBox->m_listBox.m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
							if(comboBox->m_listBox.m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE])
								comboBox->m_listBox.m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, comboBox->m_listBox.m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
						}

						//Hand out Global Status Variables pointers to all scripts
						if(comboBox->m_listBox.m_scrollBar->m_changeScript)
						{
							comboBox->m_listBox.m_scrollBar->m_changeScript->m_callingMenu	 = this;
							comboBox->m_listBox.m_scrollBar->m_changeScript->m_callingVisual = (CMenuVisual*)comboBox;
						}
						if(comboBox->m_listBox.m_scrollBar->m_mouseOverScript)
						{
							comboBox->m_listBox.m_scrollBar->m_mouseOverScript->m_callingMenu	= this;
							comboBox->m_listBox.m_scrollBar->m_mouseOverScript->m_callingVisual = (CMenuVisual*)comboBox;
						}
						if(comboBox->m_listBox.m_scrollBar->m_mouseOutScript)
						{
							comboBox->m_listBox.m_scrollBar->m_mouseOutScript->m_callingMenu	= this;
							comboBox->m_listBox.m_scrollBar->m_mouseOutScript->m_callingVisual	= (CMenuVisual*)comboBox;
						}
					}

					if(comboBox->m_listBox.m_selectionChangeScript)
					{
						comboBox->m_listBox.m_selectionChangeScript->m_callingMenu	 = this;
						comboBox->m_listBox.m_selectionChangeScript->m_callingVisual = (CMenuVisual*)comboBox;
					}

					//Adjust the size of all images
					comboBox->m_listBox.Resize();

					break;
				}
				case MENU_VISTYPE_LISTBOX:
				{
					CMenuListBox *listBox = (CMenuListBox*)curChild;

					//Get a pointer to the correct font and load the font texture(if its not already loaded)
					if(listBox->m_fontName != "")
					{
						listBox->m_font = menuFontList->GetFontByName(listBox->m_fontName);
						listBox->m_font->m_textureSelect = textureDataBase->AddAssignTempTexture(device, listBox->m_font->m_textureFile, 0);
						listBox->UpdateFontTexture();
					}

					//Load Window Borders and Corners
					if(listBox->m_borderLeft)
					{
						listBox->m_borderLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_borderLeft->m_textureFile, 0));
						listBox->m_borderTop->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_borderTop->m_textureFile, 0));
						listBox->m_borderRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_borderRight->m_textureFile, 0));
						listBox->m_borderBottom->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_borderBottom->m_textureFile, 0));
						listBox->m_cornerTopLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_cornerTopLeft->m_textureFile, 0));
						listBox->m_cornerTopRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_cornerTopRight->m_textureFile, 0));
						listBox->m_cornerBottomLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_cornerBottomLeft->m_textureFile, 0));
						listBox->m_cornerBottomRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_cornerBottomRight->m_textureFile, 0));
					}

					//Load Window Background
					if(listBox->m_background)
						listBox->m_background->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_background->m_textureFile, 0));

					//Load List Item Background
					if(listBox->m_itemBackground)
						listBox->m_itemBackground->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_itemBackground->m_textureFile, 0));

					//Load the scroll bar
					if(listBox->m_scrollBar)
					{
						//Load Background Image Textures
						if(listBox->m_scrollBar->m_backgroundLeft)
							listBox->m_scrollBar->m_backgroundLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_scrollBar->m_backgroundLeft->m_textureFile, 0));
						if(listBox->m_scrollBar->m_backgroundMiddle)
							listBox->m_scrollBar->m_backgroundMiddle->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_scrollBar->m_backgroundMiddle->m_textureFile, 0));
						if(listBox->m_scrollBar->m_backgroundRight)
							listBox->m_scrollBar->m_backgroundRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_scrollBar->m_backgroundRight->m_textureFile, 0));

						//Load Slider Image Textures
						if(listBox->m_scrollBar->m_sliderLeft)
							listBox->m_scrollBar->m_sliderLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_scrollBar->m_sliderLeft->m_textureFile, 0));
						if(listBox->m_scrollBar->m_sliderMiddle)
							listBox->m_scrollBar->m_sliderMiddle->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_scrollBar->m_sliderMiddle->m_textureFile, 0));
						if(listBox->m_scrollBar->m_sliderRight)
							listBox->m_scrollBar->m_sliderRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_scrollBar->m_sliderRight->m_textureFile, 0));

						//Load Left Button Image Textures
						if(listBox->m_scrollBar->m_leftButton)
						{
							if(listBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE])
								listBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
							if(listBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE])
								listBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
							if(listBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE])
								listBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
							if(listBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE])
								listBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
						}

						//Load Right Button Image Textures
						if(listBox->m_scrollBar->m_rightButton)
						{
							if(listBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE])
								listBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
							if(listBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE])
								listBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
							if(listBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE])
								listBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
							if(listBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE])
								listBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, listBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
						}

						//Hand out Global Status Variables pointers to all scripts
						if(listBox->m_scrollBar->m_changeScript)
						{
							listBox->m_scrollBar->m_changeScript->m_callingMenu		= this;
							listBox->m_scrollBar->m_changeScript->m_callingVisual	= (CMenuVisual*)listBox;
						}
						if(listBox->m_scrollBar->m_mouseOverScript)
						{
							listBox->m_scrollBar->m_mouseOverScript->m_callingMenu	 = this;
							listBox->m_scrollBar->m_mouseOverScript->m_callingVisual = (CMenuVisual*)listBox;
						}
						if(listBox->m_scrollBar->m_mouseOutScript)
						{
							listBox->m_scrollBar->m_mouseOutScript->m_callingMenu	= this;
							listBox->m_scrollBar->m_mouseOutScript->m_callingVisual	= (CMenuVisual*)listBox;
						}
					}

					//Give it a pointer to the draggingItem
					//listBox->m_draggingItem = &draggingItem;
					listBox->m_draggingItem = draggingItem;

					//Hand out Global Status Variables pointers to all scripts
					if(listBox->m_selectionChangeScript)
					{
						listBox->m_selectionChangeScript->m_callingMenu		= this;
						listBox->m_selectionChangeScript->m_callingVisual	= (CMenuVisual*)listBox;
					}
					if(listBox->m_mouseOverScript)
					{
						listBox->m_mouseOverScript->m_callingMenu	= this;
						listBox->m_mouseOverScript->m_callingVisual	= (CMenuVisual*)listBox;
					}
					if(listBox->m_mouseOutScript)
					{
						listBox->m_mouseOutScript->m_callingMenu	= this;
						listBox->m_mouseOutScript->m_callingVisual	= (CMenuVisual*)listBox;
					}
					if(listBox->m_dropScript)
					{
						listBox->m_dropScript->m_callingMenu	= this;
						listBox->m_dropScript->m_callingVisual	= (CMenuVisual*)listBox;
					}

					//Adjust the size of all images
					listBox->Resize();

					break;
				}
				case MENU_VISTYPE_TEXTBOX:
				{
					CMenuTextBox *textBox = (CMenuTextBox*)curChild;

					//Load Background Image Textures
					if(textBox->m_backgroundLeft)
						textBox->m_backgroundLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, textBox->m_backgroundLeft->m_textureFile, 0));
					if(textBox->m_backgroundMiddle)
						textBox->m_backgroundMiddle->UpdateTexture(textureDataBase->AddAssignTempTexture(device, textBox->m_backgroundMiddle->m_textureFile, 0));
					if(textBox->m_backgroundRight)
						textBox->m_backgroundRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, textBox->m_backgroundRight->m_textureFile, 0));

					//Get a pointer to the correct font and load the font texture(if its not already loaded)
					if(textBox->m_fontName != "")
					{
						textBox->m_font = menuFontList->GetFontByName(textBox->m_fontName);
						textBox->m_font->m_textureSelect = textureDataBase->AddAssignTempTexture(device, textBox->m_font->m_textureFile, 0);

						//Let all the text objects have the font
						for(int i = 0; i < textBox->m_maxLinesOfText; i++)
						{
							textBox->m_linesOfText[i].m_font = textBox->m_font;

							if(textBox->m_linesOfText[i].m_textMesh)
								textBox->m_linesOfText[i].m_textMesh->List_textureSelect[0] = textBox->m_font->m_textureSelect;
						}
					}

					//Make sure we give textBox->m_text the current string
					textBox->SetText(NULL);

					//Load the scroll bar
					if(textBox->m_scrollBar)
					{
						//Load Background Image Textures
						if(textBox->m_scrollBar->m_backgroundLeft)
							textBox->m_scrollBar->m_backgroundLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, textBox->m_scrollBar->m_backgroundLeft->m_textureFile, 0));
						if(textBox->m_scrollBar->m_backgroundMiddle)
							textBox->m_scrollBar->m_backgroundMiddle->UpdateTexture(textureDataBase->AddAssignTempTexture(device, textBox->m_scrollBar->m_backgroundMiddle->m_textureFile, 0));
						if(textBox->m_scrollBar->m_backgroundRight)
							textBox->m_scrollBar->m_backgroundRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, textBox->m_scrollBar->m_backgroundRight->m_textureFile, 0));

						//Load Slider Image Textures
						if(textBox->m_scrollBar->m_sliderLeft)
							textBox->m_scrollBar->m_sliderLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, textBox->m_scrollBar->m_sliderLeft->m_textureFile, 0));
						if(textBox->m_scrollBar->m_sliderMiddle)
							textBox->m_scrollBar->m_sliderMiddle->UpdateTexture(textureDataBase->AddAssignTempTexture(device, textBox->m_scrollBar->m_sliderMiddle->m_textureFile, 0));
						if(textBox->m_scrollBar->m_sliderRight)
							textBox->m_scrollBar->m_sliderRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, textBox->m_scrollBar->m_sliderRight->m_textureFile, 0));

						//Load Left Button Image Textures
						if(textBox->m_scrollBar->m_leftButton)
						{
							if(textBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE])
								textBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, textBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
							if(textBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE])
								textBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, textBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
							if(textBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE])
								textBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, textBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
							if(textBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE])
								textBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, textBox->m_scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
						}

						//Load Right Button Image Textures
						if(textBox->m_scrollBar->m_rightButton)
						{
							if(textBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE])
								textBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, textBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
							if(textBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE])
								textBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, textBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
							if(textBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE])
								textBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, textBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
							if(textBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE])
								textBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, textBox->m_scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
						}

						//Hand out Global Status Variables pointers to all scripts
						if(textBox->m_scrollBar->m_changeScript)
						{
							textBox->m_scrollBar->m_changeScript->m_callingMenu		= this;
							textBox->m_scrollBar->m_changeScript->m_callingVisual	= (CMenuVisual*)textBox;
						}
						if(textBox->m_scrollBar->m_mouseOverScript)
						{
							textBox->m_scrollBar->m_mouseOverScript->m_callingMenu	 = this;
							textBox->m_scrollBar->m_mouseOverScript->m_callingVisual = (CMenuVisual*)textBox;
						}
						if(textBox->m_scrollBar->m_mouseOutScript)
						{
							textBox->m_scrollBar->m_mouseOutScript->m_callingMenu	= this;
							textBox->m_scrollBar->m_mouseOutScript->m_callingVisual	= (CMenuVisual*)textBox;
						}
					}

					//Hand out Global Status Variables pointers to all scripts
					if(textBox->m_enterPressScript)
					{
						textBox->m_enterPressScript->m_callingMenu	 = this;
						textBox->m_enterPressScript->m_callingVisual = (CMenuVisual*)textBox;
					}
					if(textBox->m_mouseOverScript)
					{
						textBox->m_mouseOverScript->m_callingMenu	= this;
						textBox->m_mouseOverScript->m_callingVisual	= (CMenuVisual*)textBox;
					}
					if(textBox->m_mouseOutScript)
					{
						textBox->m_mouseOutScript->m_callingMenu	= this;
						textBox->m_mouseOutScript->m_callingVisual	= (CMenuVisual*)textBox;
					}

					//Adjust the size of all images
					textBox->Resize();

					break;
				}
				case MENU_VISTYPE_TEXT:
				{
					CMenuText *text = (CMenuText*)curChild;
					if (!text) break;

					//Get a pointer to the correct font and load the font texture(if its not already loaded)
					if(text->m_fontName != "")
					{
						text->m_font = menuFontList->GetFontByName(text->m_fontName);
						text->m_font->m_textureSelect = textureDataBase->AddAssignTempTexture(device, text->m_font->m_textureFile, 0);

						//Update the mesh object's texture select
						if(text->m_textMesh)
							text->m_textMesh->List_textureSelect[0] = text->m_font->m_textureSelect;
					}
					SafeDelete(text->m_textMesh);

					if(text->m_updateScript)
					{
						text->m_updateScript->m_callingMenu	= this;
						text->m_updateScript->m_callingVisual = (CMenuVisual*)text;
					}

					break;
				}
				case MENU_VISTYPE_SCROLLBAR:
				{
					CMenuScrollBar *scrollBar = (CMenuScrollBar*)curChild;

					//Load Background Image Textures
					if(scrollBar->m_backgroundLeft)
						scrollBar->m_backgroundLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, scrollBar->m_backgroundLeft->m_textureFile, 0));
					if(scrollBar->m_backgroundMiddle)
						scrollBar->m_backgroundMiddle->UpdateTexture(textureDataBase->AddAssignTempTexture(device, scrollBar->m_backgroundMiddle->m_textureFile, 0));
					if(scrollBar->m_backgroundRight)
						scrollBar->m_backgroundRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, scrollBar->m_backgroundRight->m_textureFile, 0));

					//Load Slider Image Textures
					if(scrollBar->m_sliderLeft)
						scrollBar->m_sliderLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, scrollBar->m_sliderLeft->m_textureFile, 0));
					if(scrollBar->m_sliderMiddle)
						scrollBar->m_sliderMiddle->UpdateTexture(textureDataBase->AddAssignTempTexture(device, scrollBar->m_sliderMiddle->m_textureFile, 0));
					if(scrollBar->m_sliderRight)
						scrollBar->m_sliderRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, scrollBar->m_sliderRight->m_textureFile, 0));

					//Load Left Button Image Textures
					if(scrollBar->m_leftButton)
					{
						if(scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE])
							scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
						if(scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE])
							scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
						if(scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE])
							scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
						if(scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE])
							scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, scrollBar->m_leftButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
					}

					//Load Right Button Image Textures
					if(scrollBar->m_rightButton)
					{
						if(scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE])
							scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_UP][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
						if(scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE])
							scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_HOVER][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
						if(scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE])
							scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DOWN][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
						if(scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE])
							scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, scrollBar->m_rightButton->m_stateImages[MENU_BUTTONSTATE_DISABLED][MENU_BUTTONIMAGE_MIDDLE]->m_textureFile, 0));
					}

					//Hand out Global Status Variables pointers to all scripts
					if(scrollBar->m_changeScript)
					{
						scrollBar->m_changeScript->m_callingMenu	= this;
						scrollBar->m_changeScript->m_callingVisual	= (CMenuVisual*)scrollBar;
					}
					if(scrollBar->m_mouseOverScript)
					{
						scrollBar->m_mouseOverScript->m_callingMenu		= this;
						scrollBar->m_mouseOverScript->m_callingVisual	= (CMenuVisual*)scrollBar;
					}
					if(scrollBar->m_mouseOutScript)
					{
						scrollBar->m_mouseOutScript->m_callingMenu	 = this;
						scrollBar->m_mouseOutScript->m_callingVisual = (CMenuVisual*)scrollBar;
					}

					//Adjust the size of all images
					scrollBar->Resize();
					
					break;
				}
				case MENU_VISTYPE_BUTTON:
				{
					CMenuButton *button = (CMenuButton*)curChild;

					//Load All State Image Textures
					for(int i = 0; i < MENU_BUTTONSTATE_MAX; i++)
						for(int j = 0; j < MENU_BUTTONIMAGE_MAX; j++)
							if(button->m_stateImages[i][j])
								button->m_stateImages[i][j]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, button->m_stateImages[i][j]->m_textureFile, 0));

					// REFRESH the display text
					button->m_pDisplayText->SetText(button->m_pDisplayText->m_text);

					//Give it a pointer to the draggingItem
					button->m_draggingItem = /*&*/draggingItem;

					//Hand out Global Status Variables pointers to all scripts
					if(button->m_clickScript)
					{
						button->m_clickScript->m_callingMenu	= this;
						button->m_clickScript->m_callingVisual	= (CMenuVisual*)button;
					}
					if(button->m_mouseOverScript)
					{
						button->m_mouseOverScript->m_callingMenu	= this;
						button->m_mouseOverScript->m_callingVisual	= (CMenuVisual*)button;
					}
					if(button->m_mouseOutScript)
					{
						button->m_mouseOutScript->m_callingMenu		= this;
						button->m_mouseOutScript->m_callingVisual	= (CMenuVisual*)button;
					}
					if(button->m_dropScript)
					{
						button->m_dropScript->m_callingMenu		= this;
						button->m_dropScript->m_callingVisual	= (CMenuVisual*)button;
					}

					//Adjust the size of all images
					button->Resize();

					break;
				}
				case MENU_VISTYPE_TOGGLE:
				{
					CMenuToggle *toggle = (CMenuToggle*)curChild;

					//Load All State Image Textures
					for(int i = 0; i < MENU_TOGGLESTATE_MAX; i++)
						if(toggle->m_stateImages[i])
							toggle->m_stateImages[i]->UpdateTexture(textureDataBase->AddAssignTempTexture(device, toggle->m_stateImages[i]->m_textureFile, 0));

					// REFRESH the display text
					toggle->m_pDisplayText->SetText(toggle->m_pDisplayText->m_text);

					//Hand out Global Status Variables pointers to all scripts
					if(toggle->m_clickScript)
					{
						toggle->m_clickScript->m_callingMenu	= this;
						toggle->m_clickScript->m_callingVisual	= (CMenuVisual*)toggle;
					}
					if(toggle->m_mouseOverScript)
					{
						toggle->m_mouseOverScript->m_callingMenu	= this;
						toggle->m_mouseOverScript->m_callingVisual	= (CMenuVisual*)toggle;
					}
					if(toggle->m_mouseOutScript)
					{
						toggle->m_mouseOutScript->m_callingMenu		= this;
						toggle->m_mouseOutScript->m_callingVisual	= (CMenuVisual*)toggle;
					}

					break;
				}
				case MENU_VISTYPE_PROGRESSBAR:
				{
					CMenuProgressBar *progBar = (CMenuProgressBar*)curChild;

					//Load Background Image Textures
					if(progBar->m_backgroundLeft)
						progBar->m_backgroundLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, progBar->m_backgroundLeft->m_textureFile, 0));
					progBar->m_backgroundMiddle->UpdateTexture(textureDataBase->AddAssignTempTexture(device, progBar->m_backgroundMiddle->m_textureFile, 0));
					if(progBar->m_backgroundRight)
						progBar->m_backgroundRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, progBar->m_backgroundRight->m_textureFile, 0));

					//Load Foreground Image Textures
					if(progBar->m_foregroundLeft)
						progBar->m_foregroundLeft->UpdateTexture(textureDataBase->AddAssignTempTexture(device, progBar->m_foregroundLeft->m_textureFile, 0));
					progBar->m_foregroundMiddle->UpdateTexture(textureDataBase->AddAssignTempTexture(device, progBar->m_foregroundMiddle->m_textureFile, 0));
					if(progBar->m_foregroundRight)
						progBar->m_foregroundRight->UpdateTexture(textureDataBase->AddAssignTempTexture(device, progBar->m_foregroundRight->m_textureFile, 0));

					//Hand out Global Status Variables pointers to all scripts
					if(progBar->m_mouseOverScript)
					{
						progBar->m_mouseOverScript->m_callingMenu	= this;
						progBar->m_mouseOverScript->m_callingVisual	= (CMenuVisual*)progBar;
					}
					if(progBar->m_mouseOutScript)
					{
						progBar->m_mouseOutScript->m_callingMenu	= this;
						progBar->m_mouseOutScript->m_callingVisual	= (CMenuVisual*)progBar;
					}
					if(progBar->m_updateScript)
					{
						progBar->m_updateScript->m_callingMenu	 = this;
						progBar->m_updateScript->m_callingVisual = (CMenuVisual*)progBar;
					}

					//Adjust the size of all images
					progBar->Resize();

					break;
				}
				case MENU_VISTYPE_IMAGE:
				{
					//Load the Image Texture
					CMenuImage *image = (CMenuImage*)curChild;
					image->UpdateTexture(textureDataBase->AddAssignTempTexture(device, image->m_textureFile, 0));

					//Give it a pointer to the draggingItem
					image->m_draggingItem = /*&*/draggingItem;

					//Hand out Global Status Variables pointers to all scripts
					if(image->m_clickScript)
					{
						image->m_clickScript->m_callingMenu		= this;
						image->m_clickScript->m_callingVisual	= (CMenuVisual*)image;
					}
					if(image->m_mouseOverScript)
					{
						image->m_mouseOverScript->m_callingMenu		= this;
						image->m_mouseOverScript->m_callingVisual	= (CMenuVisual*)image;
					}
					if(image->m_mouseOutScript)
					{
						image->m_mouseOutScript->m_callingMenu	 = this;
						image->m_mouseOutScript->m_callingVisual = (CMenuVisual*)image;
					}
					if(image->m_dropScript)
					{
						image->m_dropScript->m_callingMenu	 = this;
						image->m_dropScript->m_callingVisual = (CMenuVisual*)image;
					}
					if(image->m_updateScript)
					{
						image->m_updateScript->m_callingMenu	= this;
						image->m_updateScript->m_callingVisual	= (CMenuVisual*)image;
					}

					break;
				}
				case MENU_VISTYPE_INVENTORY:
				{
					// Load the inventory's prototype image Texture
					CMenuInventory *inventory = (CMenuInventory*)curChild;
					inventory->UpdateTexture(textureDataBase->AddAssignTempTexture(
						device, 
						inventory->m_pPrototypeImage->m_textureFile, 
						0));

					// Give it a pointer to the draggingItem
					inventory->m_pPrototypeImage->m_draggingItem = draggingItem;

					// Hand out Global Status Variables pointers to all scripts
					if(inventory->m_clickScript)
					{
						inventory->m_clickScript->m_callingMenu		= this;
						inventory->m_clickScript->m_callingVisual	= (CMenuVisual*)inventory;
					}
					if(inventory->m_mouseOverScript)
					{
						inventory->m_mouseOverScript->m_callingMenu		= this;
						inventory->m_mouseOverScript->m_callingVisual	= (CMenuVisual*)inventory;
					}
					if(inventory->m_mouseOutScript)
					{
						inventory->m_mouseOutScript->m_callingMenu	 = this;
						inventory->m_mouseOutScript->m_callingVisual = (CMenuVisual*)inventory;
					}
					if(inventory->m_dropScript)
					{
						inventory->m_dropScript->m_callingMenu	 = this;
						inventory->m_dropScript->m_callingVisual = (CMenuVisual*)inventory;
					}
					if(inventory->m_updateScript)
					{
						inventory->m_updateScript->m_callingMenu	= this;
						inventory->m_updateScript->m_callingVisual	= (CMenuVisual*)inventory;
					}

					// REFRESH the background image textures
					// RENDER EITHER the prototype image OR the grid clones
					if (inventory->m_backgrounds.size() > 0)
					{
						for (int i=0; i<inventory->m_backgrounds.size(); i++)
						{
							CMenuImage *pImage = inventory->m_backgrounds[i];
							if (!pImage) continue;
							pImage->m_textureSelect = textureDataBase->AddAssignTempTexture(
								device, pImage->m_textureFile, 0);
							pImage->UpdateTexture(pImage->m_textureSelect);
						}
					}
					else if (inventory->m_pPrototypeImage)
					{
						inventory->m_pPrototypeImage->m_textureSelect = textureDataBase->AddAssignTempTexture(
							device, inventory->m_pPrototypeImage->m_textureFile, 0);
						inventory->m_pPrototypeImage->UpdateTexture(inventory->m_pPrototypeImage->m_textureSelect);
					}

					// RENDER the icons for all inventory items
					for (int i=0; i<inventory->m_items.size(); i++)
					{
						CMenuInventoryItem *pItem = inventory->m_items[i];
						pItem->m_pIcon->m_textureSelect = textureDataBase->AddAssignTempTexture(
							device, pItem->m_pIcon->m_textureFile, 0);
						pItem->m_pIcon->UpdateTexture(pItem->m_pIcon->m_textureSelect);
					}

					// RENDER all the children visuals
					POSITION pos = NULL;
					for (pos = this->m_childList->GetHeadPosition(); pos != NULL;)
					{
						CMenuVisual *pVisual = dynamic_cast<CMenuVisual*>(this->m_childList->GetNext(pos));
						CMenuImage *pImage = dynamic_cast<CMenuImage*>(pVisual);
						if (!pVisual) continue;
						if (pImage)
						{
							pImage->m_textureSelect = textureDataBase->AddAssignTempTexture(
								device, pImage->m_textureFile, 0);
							pImage->Init(device);
							pImage->UpdateTexture(pImage->m_textureSelect);
						}
					}

					// REFRESH inventory visual display
					inventory->RefreshItems();
					break;
				}
			}
		}
	}
}