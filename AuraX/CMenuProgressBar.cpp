#include "stdafx.h"
#include "direct.h"
#include "CMenuProgressBar.h"
#include "CMenuScript.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMenuProgressBar::CMenuProgressBar()
{
	//Visible Objects
	m_backgroundLeft		= NULL;
	m_backgroundMiddle		= new CMenuImage();
	m_backgroundRight		= NULL;
	m_foregroundLeft		= NULL;
	m_foregroundLeftSize	= 0.0f; //This is used to keep the original texture width or height
	m_foregroundMiddle		= new CMenuImage();
	m_foregroundRight		= NULL;
	m_foregroundRightSize	= 0.0f; //This is used to keep the original texture width or height

	//Settable Parameters
	m_isVertical			= false;
	m_isFlipped				= false;
	m_curPercentage			= 1.0f;
	m_foregroundInset		= 0.0f;

	//Engine Variables
	m_mousedOver			= false;

	//Scripts
	m_mouseOverScript	= NULL;
	m_mouseOutScript	= NULL;
	m_updateScript		= NULL;
}

bool CMenuProgressBar::HandleInput(CInput *pInput, float parentX, float parentY, DWORD curTime, LPDIRECT3DDEVICE9 pD3DDevice )
{
	//If the mouse is inside of the progress bar
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

		//We're no longer hovering over the progress bar
		m_mousedOver = false;
	}

	return false;
}

void CMenuProgressBar::Render(	LPDIRECT3DDEVICE9 pD3DDevice,
								CStateManagementObj *stateManager, 
								CTextureDBObjList *textureDatabase,
								float parentX,
								float parentY)
{
	//The progress bar is now the parent
	parentX += m_posX;
	parentY += m_posY;

	//Render the background pieces
	if(m_backgroundLeft)
		m_backgroundLeft->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	if(m_backgroundRight)
		m_backgroundRight->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	m_backgroundMiddle->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	//Execute the update script
	if(m_updateScript)
		m_updateScript->ExecuteScript();

	//Render the foreground pieces
	float fullSize;
	float curSize;
	float middleStart;
	float middleEnd;
	float *leftPos		= NULL;
	float *leftSize		= NULL;
	float *leftTexSize	= NULL;
	float *middlePos	= NULL;
	float *middleSize	= NULL;
	float *rightPos		= NULL;
	float *rightSize	= NULL;
	float *rightTexSize	= NULL;

	//Grab pointers to position and side coordinates
	if(m_isVertical)
	{
		//If its vertical, we need pointers to Y positions and heights
		fullSize = m_height;
		
		if(m_foregroundLeft)
		{
			leftPos		= &m_foregroundLeft->m_posY;
			leftSize	= &m_foregroundLeft->m_height;
			leftTexSize	= &m_foregroundLeft->m_textureHeight;
		}

		middlePos	= &m_foregroundMiddle->m_posY;
		middleSize	= &m_foregroundMiddle->m_height;

		if(m_foregroundRight)
		{
			rightPos		= &m_foregroundRight->m_posY;
			rightSize		= &m_foregroundRight->m_height;
			rightTexSize	= &m_foregroundRight->m_textureHeight;
		}
	}
	else
	{
		//If its horizontal, we need pointers to X positions and widths
		fullSize = m_width;

		if(m_foregroundLeft)
		{
			leftPos		= &m_foregroundLeft->m_posX;
			leftSize	= &m_foregroundLeft->m_width;
			leftTexSize	= &m_foregroundLeft->m_textureWidth;
		}

		middlePos	= &m_foregroundMiddle->m_posX;
		middleSize	= &m_foregroundMiddle->m_width;

		if(m_foregroundRight)
		{
			rightPos		= &m_foregroundRight->m_posX;
			rightSize		= &m_foregroundRight->m_width;
			rightTexSize	= &m_foregroundRight->m_textureWidth;
		}
	}

	//Get the current size of the foreground bar
	curSize = (fullSize - 2 * m_foregroundInset) * m_curPercentage;

	//If its flipped, the end a beginning of the middle component are backwards
	if(m_isFlipped)
	{
		middleEnd	= fullSize - m_foregroundInset;
		middleStart	= middleEnd - curSize;
	}
	else
	{
		middleStart	= m_foregroundInset;
		middleEnd	= middleStart + curSize;
	}

	//If the foreground has a left/bottom component
	if(m_foregroundLeft)
	{
		//We plan on the left side possibly taking up the whole thing
		float size = curSize;

		//If its flipped, the left position is middleStart
		if(m_isFlipped)
			*leftPos = middleStart;
		else
		{
			//Otherwise, if its not flipped and there is a right side,
			//subtract it's size away from the size of the left side
			if(rightTexSize)
				size -= *rightTexSize;
		}

		//If the original left size is greater than the potential left size
		if(m_foregroundLeftSize > size)
		{
			//Use the potential left size
			*leftSize	 = size;
			*leftTexSize = size;
		}
		else
		{
			//Otherwise, use the original left size
			*leftSize	 = m_foregroundLeftSize;
			*leftTexSize = m_foregroundLeftSize;
		}

		//Update the texture coordinates
		if(m_foregroundLeft->m_imageMesh)
			m_foregroundLeft->UpdateTextureCoords();

		//Increase the middle's start position by the size of the left component
		middleStart += *leftSize;
	}

	//If the foreground has a right/top component
	if(m_foregroundRight)
	{
		//We plan on the left side possibly taking up the whole thing
		float size = curSize;
		//Decrease the middle's end position by the size of the right component
		middleEnd -= *rightSize;

		//If its flipped and there is a left side,
		//subtract it's size away from the size of the right side
		if(m_isFlipped)
		{
			if(leftTexSize)
				size -= *leftTexSize;
		}
		else //If its not flipped, the right position is middleEnd
			*rightPos = middleEnd;

		//If the original right size is greater than the potential right size
		if(m_foregroundRightSize >= size)
		{
			//Use the potential right size
			*rightSize		= size;
			*rightTexSize	= size;
		}
		else
		{
			//Otherwise, use the original right size
			*rightSize		= m_foregroundRightSize;
			*rightTexSize	= m_foregroundRightSize;
		}

		//Update the texture coordinates
		if(m_foregroundRight->m_imageMesh)
			m_foregroundRight->UpdateTextureCoords();
	}

	//If we have a left component and its size is positive, render it
	if(m_foregroundLeft && *leftSize > 0)
		m_foregroundLeft->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	//If we have a right component and its size is positive, render it
	if(m_foregroundRight && *rightSize > 0)
		m_foregroundRight->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	//If there's space between the left piece and the right piece, fill it with the middle piece
	if(middleEnd > middleStart)
	{
		*middlePos	= middleStart;
		*middleSize	= middleEnd - middleStart;
		m_foregroundMiddle->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);
	}
}

void CMenuProgressBar::Resize()
{
	//Vertical must be handled totally seperate from horizontal
	if(m_isVertical)
	{
		//The background's middle top coordinate starts at the height
		//and the bottom coordinate starts at 0
		float middleTop		= m_height;
		float middleBottom	= 0.0f;

		//If there's a bottom component to the background, set its coordinates
		if(m_backgroundLeft)
		{
			m_backgroundLeft->m_posX	= 0.0f;
			m_backgroundLeft->m_posY	= 0.0f;
			m_backgroundLeft->m_width	= m_width;
			m_backgroundLeft->m_height	= m_backgroundLeft->m_textureHeight;
			middleBottom += m_backgroundLeft->m_height;
		}

		//If there's a top component to the background, set its coordinates
		if(m_backgroundRight)
		{
			m_backgroundRight->m_posX	= 0.0f;
			m_backgroundRight->m_posY	= m_height - m_backgroundRight->m_height;
			m_backgroundRight->m_width	= m_width;
			m_backgroundRight->m_height	= m_backgroundRight->m_textureHeight;
			middleTop -= m_backgroundRight->m_height;
		}

		//If there's a middle component to the background, set its coordinates
		//to fill in the gap between the top and bottom components
		m_backgroundMiddle->m_posX	= 0.0f;
		m_backgroundMiddle->m_posY	= middleBottom;
		m_backgroundMiddle->m_width	= m_width;
		m_backgroundMiddle->m_height	= middleTop - middleBottom;

		//If there's a bottom component to the foreground
		if(m_foregroundLeft)
		{
			//If the progress bar isn't flipped, set the Y position to the bottom
			if(!m_isFlipped)
				m_foregroundLeft->m_posY = m_foregroundInset;

			//Set the rest of its static coordinates
			m_foregroundLeft->m_posX	= m_foregroundInset;
			m_foregroundLeft->m_width	= m_width - (2 * m_foregroundInset);
			m_foregroundLeft->m_height	= m_foregroundLeft->m_textureHeight;
		}

		//If there's a top component to the foreground
		if(m_foregroundRight)
		{
			//If the progress bar is flipped, set the Y position to the top
			if(m_isFlipped)
				m_foregroundRight->m_posY = m_height - m_foregroundInset - m_foregroundRight->m_textureHeight;

			//Set the rest of its static coordinates
			m_foregroundRight->m_posX	= m_foregroundInset;
			m_foregroundRight->m_width	= m_width - (2 * m_foregroundInset);
			m_foregroundRight->m_height	= m_foregroundRight->m_textureHeight;
		}

		//Set the foreground middle's static coordinates
		m_foregroundMiddle->m_posX	= m_foregroundInset;
		m_foregroundMiddle->m_width	= m_width - (2 * m_foregroundInset);
	}
	else
	{
		//The background's middle left coordinate starts at 0
		//and the right coordinate starts at the bar's width
		float middleLeft	= 0.0f;
		float middleRight	= m_width;

		//If there's a left component to the background, set its coordinates
		if(m_backgroundLeft)
		{
			m_backgroundLeft->m_posX	= 0.0f;
			m_backgroundLeft->m_posY	= 0.0f;
			m_backgroundLeft->m_width	= m_backgroundLeft->m_textureWidth;
			m_backgroundLeft->m_height	= m_height;
			middleLeft += m_backgroundLeft->m_width;
		}

		//If there's a right component to the background, set its coordinates
		if(m_backgroundRight)
		{
			m_backgroundRight->m_posX	= m_width - m_backgroundRight->m_width;
			m_backgroundRight->m_posY	= 0.0f;
			m_backgroundRight->m_width	= m_backgroundRight->m_textureWidth;
			m_backgroundRight->m_height	= m_height;
			middleRight -= m_backgroundRight->m_width;
		}

		//If there's a middle component to the background, set its coordinates
		//to fill in the gap between the left and right components
		m_backgroundMiddle->m_posX	 = middleLeft;
		m_backgroundMiddle->m_posY	 = 0.0f;
		m_backgroundMiddle->m_width	 = middleRight - middleLeft;
		m_backgroundMiddle->m_height = m_height;

		//If there's a left component to the foreground
		if(m_foregroundLeft)
		{
			//If the progress bar isn't flipped, set the X position to the left
			if(!m_isFlipped)
				m_foregroundLeft->m_posX = m_foregroundInset;

			//Set the rest of its static coordinates
			m_foregroundLeft->m_posY	= m_foregroundInset;
			m_foregroundLeft->m_width	= m_foregroundLeft->m_textureWidth;
			m_foregroundLeft->m_height	= m_height - (2 * m_foregroundInset);
		}

		//If there's a right component to the foreground
		if(m_foregroundRight)
		{
			//If the progress bar is flipped, set the X position to the right
			if(m_isFlipped)
				m_foregroundRight->m_posX = m_width - m_foregroundInset - m_foregroundRight->m_textureWidth;

			//Set the rest of its static coordinates
			m_foregroundRight->m_posY	= m_foregroundInset;
			m_foregroundRight->m_width	= m_foregroundRight->m_textureWidth;
			m_foregroundRight->m_height	= m_height - (2 * m_foregroundInset);
		}

		//Set the foreground middle's static coordinates
		m_foregroundMiddle->m_posY	= m_foregroundInset;
		m_foregroundMiddle->m_height = m_height - (2 * m_foregroundInset);
	}
}

CMenuProgressBar& 
CMenuProgressBar::operator=(const CMenuProgressBar &rhs)
{
	//Copy base properties
	//CMenuVisual::Copy(pCopy, pD3DDevice);

	//Copy Progress Bar settings
	//CMenuProgressBar *progressBarCopy = (CMenuProgressBar*)pCopy;

	//Visible Objects
	if(rhs.m_backgroundLeft)
		this->m_backgroundLeft = rhs.m_backgroundLeft;

	this->m_backgroundMiddle = rhs.m_backgroundMiddle;

	if(rhs.m_backgroundRight)
		this->m_backgroundRight = rhs.m_backgroundRight;

	if(rhs.m_foregroundLeft)
		this->m_foregroundLeft = rhs.m_foregroundLeft;

	this->m_foregroundMiddle = rhs.m_foregroundMiddle;

	if(rhs.m_foregroundRight)
		this->m_foregroundRight = rhs.m_foregroundRight;

	//Settable Parameters
	this->m_isVertical		= rhs.m_isVertical;
	this->m_isFlipped		= rhs.m_isFlipped;
	this->m_curPercentage	= rhs.m_curPercentage;
	this->m_foregroundInset	= rhs.m_foregroundInset;

	//scripts
	if(rhs.m_updateScript)
       this->m_updateScript = rhs.m_updateScript;
	if(rhs.m_mouseOverScript)
       this->m_mouseOverScript = rhs.m_mouseOverScript;
	if(rhs.m_mouseOutScript)
      this-> m_mouseOutScript = rhs.m_mouseOutScript;

	// Allow for chained assignments
	return *this;
}

void CMenuProgressBar::Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice)
{
	*pClone = new CMenuProgressBar();
	Copy(*pClone, pD3DDevice);
}

void CMenuProgressBar::Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice)
{
	//Copy base properties
	CMenuVisual::Copy(pCopy, pD3DDevice);

	//Copy Progress Bar settings
	CMenuProgressBar *progressBarCopy = (CMenuProgressBar*)pCopy;

	//Visible Objects
	if(m_backgroundLeft)
		m_backgroundLeft->Clone((CMenuVisual**)&progressBarCopy->m_backgroundLeft, pD3DDevice);

	m_backgroundMiddle->Clone((CMenuVisual**)&progressBarCopy->m_backgroundMiddle, pD3DDevice);

	if(m_backgroundRight)
		m_backgroundRight->Clone((CMenuVisual**)&progressBarCopy->m_backgroundRight, pD3DDevice);

	if(m_foregroundLeft)
		m_foregroundLeft->Clone((CMenuVisual**)&progressBarCopy->m_foregroundLeft, pD3DDevice);

	m_foregroundMiddle->Clone((CMenuVisual**)&progressBarCopy->m_foregroundMiddle, pD3DDevice);

	if(m_foregroundRight)
		m_foregroundRight->Clone((CMenuVisual**)&progressBarCopy->m_foregroundRight, pD3DDevice);

	//Settable Parameters
	progressBarCopy->m_isVertical		= m_isVertical;
	progressBarCopy->m_isFlipped		= m_isFlipped;
	progressBarCopy->m_curPercentage	= m_curPercentage;
	progressBarCopy->m_foregroundInset	= m_foregroundInset;

	//scripts
	if(m_updateScript)
       m_updateScript->Clone(&progressBarCopy->m_updateScript);
	if(m_mouseOverScript)
       m_mouseOverScript->Clone(&progressBarCopy->m_mouseOverScript);
	if(m_mouseOutScript)
       m_mouseOutScript->Clone(&progressBarCopy->m_mouseOutScript);
}

CMenuProgressBar::~CMenuProgressBar()
{
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

	if(m_foregroundLeft)
	{
		delete m_foregroundLeft;
		m_foregroundLeft = NULL;
	}

	if(m_foregroundMiddle)
	{
		delete m_foregroundMiddle;
		m_foregroundMiddle = NULL;
	}

	if(m_foregroundRight)
	{
		delete m_foregroundRight;
		m_foregroundRight = NULL;
	}

	//Delete all the scripts
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
	if(m_updateScript)
	{
		delete m_updateScript;
		m_updateScript = NULL;
	}
}

void CMenuProgressBar::Serialize(CXMLFileIO &xml)
{
	CMenuVisual::Serialize(xml);

	if(xml.m_isStoring)
	{
		xml.WriteAttribute("Version", 1);
		xml.WriteAttribute("IsVertical", m_isVertical);
		xml.WriteAttribute("IsFlipped", m_isFlipped);
		xml.WriteAttribute("ForegroundInset", (int)m_foregroundInset);

		if(m_backgroundLeft)
		{
			xml.WriteOpeningTag("BackgroundLeft");
			m_backgroundLeft->Serialize(xml);
			xml.WriteClosingTag("BackgroundLeft");
		}

		xml.WriteOpeningTag("BackgroundMiddle");
		m_backgroundMiddle->Serialize(xml);
		xml.WriteClosingTag("BackgroundMiddle");

		if(m_backgroundRight)
		{
			xml.WriteOpeningTag("BackgroundRight");
			m_backgroundRight->Serialize(xml);
			xml.WriteClosingTag("BackgroundRight");
		}

		if(m_foregroundLeft)
		{
			xml.WriteOpeningTag("ForegroundLeft");
			m_foregroundLeft->Serialize(xml);
			xml.WriteClosingTag("ForegroundLeft");
		}

		xml.WriteOpeningTag("ForegroundMiddle");
		m_foregroundMiddle->Serialize(xml);
		xml.WriteClosingTag("ForegroundMiddle");

		if(m_foregroundRight)
		{
			xml.WriteOpeningTag("ForegroundRight");
			m_foregroundRight->Serialize(xml);
			xml.WriteClosingTag("ForegroundRight");
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
		bool closing;
		char buffer[MAX_BUFFER];
		int  version;

		xml.ReadAttribute(version);
		xml.ReadAttribute(m_isVertical);
		xml.ReadAttribute(m_isFlipped);
		xml.ReadAttribute(m_foregroundInset);

		xml.ReadTag(buffer, MAX_BUFFER, closing);

		if(strcmp(buffer, "BackgroundLeft") == 0)
		{
			SafeDelete(m_backgroundLeft);
			m_backgroundLeft = new CMenuImage();
			m_backgroundLeft->Serialize(xml);
			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </ProgressBar>
		}

		if(strcmp(buffer, "BackgroundMiddle") == 0)
		{
		    SafeDelete(m_backgroundMiddle);
			m_backgroundMiddle = new CMenuImage();
			m_backgroundMiddle->Serialize(xml);
			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </ProgressBar>
		}

		if(strcmp(buffer, "BackgroundRight") == 0)
		{
		    SafeDelete(m_backgroundRight);
			m_backgroundRight = new CMenuImage();
			m_backgroundRight->Serialize(xml);
			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </ProgressBar>
		}

		if(strcmp(buffer, "ForegroundLeft") == 0)
		{
		    SafeDelete(m_foregroundLeft);
			m_foregroundLeft = new CMenuImage();
			m_foregroundLeft->Serialize(xml);

			//Get the left's size
			if(m_isVertical)
				m_foregroundLeftSize = m_foregroundLeft->m_height;
			else
				m_foregroundLeftSize = m_foregroundLeft->m_width;

			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </ProgressBar>
		}

		if(strcmp(buffer, "ForegroundMiddle") == 0)
		{
			SafeDelete(m_foregroundMiddle);
			m_foregroundMiddle = new CMenuImage();
			m_foregroundMiddle->Serialize(xml);
			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </ProgressBar>
		}

		if(strcmp(buffer, "ForegroundRight") == 0)
		{
			SafeDelete(m_foregroundRight);
			m_foregroundRight = new CMenuImage();
			m_foregroundRight->Serialize(xml);

			//Get the right's size
			if(m_isVertical)
				m_foregroundRightSize = m_foregroundRight->m_height;
			else
				m_foregroundRightSize = m_foregroundRight->m_width;

			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </ProgressBar>
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

			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read in </ProgressBar>
		}

		m_curPercentage = 1.0f;
	}
}
