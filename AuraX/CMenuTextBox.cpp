#include "stdafx.h"
#include "direct.h"
#include "CMenuTextBox.h"
#include "CMenuScript.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMenuTextBox::CMenuTextBox()
{
	m_red				= NULL;
	m_green				= NULL;
	m_blue				= NULL;
	m_alpha				= NULL;
	//Visible Objects
	m_backgroundLeft	= NULL;
	m_backgroundMiddle	= NULL;
	m_backgroundRight	= NULL;
	m_linesOfText		= NULL;
	m_scrollBar			= NULL;

	//Settable Parameters
	m_font			 = NULL;
	m_maxLinesOfText = 1;
	m_horAlignment	 = MENU_TEXTALIGN_LEFT;
	m_vertAlignment	 = MENU_TEXTALIGN_BOTTOM;
	m_multilined	 = false;
	m_editable		 = true;
	m_password		 = false;
	m_textInset		 = 0.0f;

	//Engine Variables
	m_mousedOver		= false;
	m_numVisibleLines	= 1;
	m_curLine			= 0;
	m_curCharacter		= 0;
	m_topLine			= 0;
	m_nextWriteLine		= 0;
	m_hasLooped			= false;
	m_showCursor		= false;
	m_lastCursorBlink	= 0;

	// Validation Variables
	this->m_bIsNumericOnly = false;
	this->m_iMaxLength = 0;

	//Scripts
	m_enterPressScript	= NULL;
	m_mouseOverScript	= NULL;
	m_mouseOutScript	= NULL;
}

bool CMenuTextBox::HandleInput(CInput *pInput, float parentX, float parentY, DWORD curTime, LPDIRECT3DDEVICE9 pD3DDevice)
{
	//The button is now the parent
	parentX += m_posX;
	parentY += m_posY;

	//Handle cursor blink timing
	if(m_isFocused && curTime - m_lastCursorBlink > 300)
	{
		m_showCursor = !m_showCursor;
		m_lastCursorBlink = curTime;
	}

	//If they press escape, remove focus on the text box
	if(pInput->m_keyboardKeys[DIK_ESCAPE].JustPressed())
	{
		m_isFocused  = false;
		m_showCursor = false;
	}

	//If we have a scroll bar, see if it handles the input first
	if(m_scrollBar)
		if(m_scrollBar->HandleInput(pInput, parentX, parentY, curTime, pD3DDevice))
			return true;

	//If the left mouse button was just pressed inside the text box
	if(pInput->m_mouseButtons[MOUSE_LEFTBUTTON].JustPressed() && m_editable)
	{
		if(	pInput->m_mouseAbsPosX > parentX && pInput->m_mouseAbsPosX < parentX + m_width &&
			pInput->m_mouseAbsPosY > parentY && pInput->m_mouseAbsPosY < parentY + m_height)
		{
			/*/Don't do this for demo, but figure it out later
			//Determine where the cursor should be
			m_curLine = 0;
			m_cursor.m_posX = 0;

			//If this is a multilined text box
			if(m_multilined)
			{
				//Find the line that was clicked on
				m_curLine = (m_height - (pInput->m_mouseAbsPosY - parentY)) / m_font->m_height;

				//Keep it within the bounds of what's visible
				if(m_curLine > m_numVisibleLines)
					m_curLine = m_topLine + m_numVisibleLines;
				else
					m_curLine += m_topLine;
			}

			//Get the X Coordinate of the click with respect to the text
			float clickX = pInput->m_mouseAbsPosX - parentX - m_textInset;

			//Get the character they clicked on
			int curChar, charWidth;
			while(clickX > 0.0f && m_curCharacter < m_linesOfText[m_curLine].m_text.GetLength())
			{
				//Get the current character
				curChar = m_linesOfText[m_curLine].m_text.GetAt(m_curCharacter++) - 32;

				//Get the width of this character, including spacing
				charWidth = m_font->m_characterSpacingBefore[curChar] +
							m_font->m_characterTextureWidth[curChar] +
							m_font->m_characterSpacingAfter[curChar];

				//Reduce the click X til its less than 0
				clickX -= charWidth;

				//Keep up with the cursor's X position
				m_cursor.m_posX += charWidth;
			}

			//Calculate the cursor's Y position
			m_cursor.m_posY = m_curLine * m_font->m_height;/*/

			//Set focus and eat the input
			m_isFocused	 = true;
			m_showCursor = true;
			m_lastCursorBlink = curTime;
			return true;
		}
		else
		{
			m_isFocused  = false;
			m_showCursor = false;
		}
	}
	//If the mouse is inside of the text box and the mouse isn't down
	else if(pInput->m_mouseAbsPosX > parentX && pInput->m_mouseAbsPosX < parentX + m_width &&
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

	//If the text box has focus, we should eat all key presses
	if(m_isFocused && m_editable)
	{
		//If they pressed a key
		if(HandleTyping(pInput, curTime))
		{
			//Execute the key press script, if it exists
			//if(m_keyPressScript)
			//	m_keyPressScript->ExecuteScript();

			WrapLine(m_curLine);
		}

		return true;
	}

	return false;
}

void CMenuTextBox::AddLine(CString newLine, int colorType)
{
	if(!m_linesOfText)
	    return;

	//Set the information into the text object for the line
	m_linesOfText[m_nextWriteLine].m_text	= newLine;
	m_linesOfText[m_nextWriteLine].m_red	= m_red[colorType];
	m_linesOfText[m_nextWriteLine].m_green	= m_green[colorType];
	m_linesOfText[m_nextWriteLine].m_blue	= m_blue[colorType];
	m_linesOfText[m_nextWriteLine].m_alpha	= m_alpha[colorType];

	//Make it rebuild the mesh
	if(m_linesOfText[m_nextWriteLine].m_textMesh)
	{
		m_linesOfText[m_nextWriteLine].m_textMesh->SafeDelete();
		delete m_linesOfText[m_nextWriteLine].m_textMesh;
		m_linesOfText[m_nextWriteLine].m_textMesh = NULL;
	}

	//Scroll the text if the text box is filled
	if(m_hasLooped || m_nextWriteLine >= m_numVisibleLines)
	{
		m_topLine++;

		if(m_topLine >= m_maxLinesOfText)
			m_topLine = 0;

		//If we have a scroll bar and its not all the way scrolled down
		if(m_scrollBar && ((int)m_scrollBar->m_curPosition != (int)(m_scrollBar->m_fullSize - m_scrollBar->m_sliderSize)))
		{
			//Scroll the slider up one line bottom(to keep the same text visible)
			m_scrollBar->m_curPosition -= (m_scrollBar->m_fullSize - m_scrollBar->m_sliderSize) / (m_maxLinesOfText - m_numVisibleLines);
			m_scrollBar->Update();
		}
	}

	//Save this for the Wrap Line call
	int lastLine = m_nextWriteLine;

	//NEXT!
	m_nextWriteLine++;

	//If we hit the max lines, then loop back to the beginning
	if(m_nextWriteLine == m_maxLinesOfText)
	{
		m_hasLooped		= true;
		m_nextWriteLine = 0;
	}

	//Make sure the line isn't wider than the text box
	WrapLine(lastLine, colorType);
}

void CMenuTextBox::AddLine(CString newLine, DWORD appendedInt)
{
	//Set the information into the text object for the line
	m_linesOfText[m_nextWriteLine].m_text	= newLine;
	newLine.Format("%i", appendedInt);
	m_linesOfText[m_nextWriteLine].m_text  += newLine;
	m_linesOfText[m_nextWriteLine].m_red	= m_red[MENU_TEXTCOLOR_NORMAL];
	m_linesOfText[m_nextWriteLine].m_green	= m_green[MENU_TEXTCOLOR_NORMAL];
	m_linesOfText[m_nextWriteLine].m_blue	= m_blue[MENU_TEXTCOLOR_NORMAL];
	m_linesOfText[m_nextWriteLine].m_alpha	= m_alpha[MENU_TEXTCOLOR_NORMAL];

	//Make it rebuild the mesh
	if(m_linesOfText[m_nextWriteLine].m_textMesh)
	{
		m_linesOfText[m_nextWriteLine].m_textMesh->SafeDelete();
		delete m_linesOfText[m_nextWriteLine].m_textMesh;
		m_linesOfText[m_nextWriteLine].m_textMesh = NULL;
	}

	//Scroll the down if the text box is filled
	if(m_hasLooped || m_nextWriteLine >= m_numVisibleLines)
	{
		m_topLine++;

		if(m_topLine >= m_maxLinesOfText)
			m_topLine = 0;

		//If we have a scroll bar and its not all the way scrolled down
		if(m_scrollBar && m_scrollBar->m_curPosition != m_scrollBar->m_fullSize - m_scrollBar->m_sliderSize)
		{
			//Scroll the slider up one line bottom(to keep the same text visible)
			m_scrollBar->m_curPosition -= (m_scrollBar->m_fullSize - m_scrollBar->m_sliderSize) / (m_maxLinesOfText - m_numVisibleLines);
			m_scrollBar->Update();
		}
	}

	//Save this for the Wrap Line call
	int lastLine = m_nextWriteLine;

	//NEXT!
	m_nextWriteLine++;

	//If we hit the max lines, then loop back to the beginning
	if(m_nextWriteLine == m_maxLinesOfText)
	{
		m_hasLooped		= true;
		m_nextWriteLine = 0;
	}

	//Make sure the line isn't wider than the text box
	WrapLine(lastLine);
}

void CMenuTextBox::AddLine(CString newLine, D3DVECTOR appendedVector)
{
	//Set the information into the text object for the line
	m_linesOfText[m_nextWriteLine].m_text	= newLine;
	newLine.Format("%f, %f, %f", appendedVector.x, appendedVector.y, appendedVector.z);
	m_linesOfText[m_nextWriteLine].m_text  += newLine;
	m_linesOfText[m_nextWriteLine].m_red	= m_red[MENU_TEXTCOLOR_NORMAL];
	m_linesOfText[m_nextWriteLine].m_green	= m_green[MENU_TEXTCOLOR_NORMAL];
	m_linesOfText[m_nextWriteLine].m_blue	= m_blue[MENU_TEXTCOLOR_NORMAL];
	m_linesOfText[m_nextWriteLine].m_alpha	= m_alpha[MENU_TEXTCOLOR_NORMAL];

	//Make it rebuild the mesh
	if(m_linesOfText[m_nextWriteLine].m_textMesh)
	{
		m_linesOfText[m_nextWriteLine].m_textMesh->SafeDelete();
		delete m_linesOfText[m_nextWriteLine].m_textMesh;
		m_linesOfText[m_nextWriteLine].m_textMesh = NULL;
	}

	//Scroll the down if the text box is filled
	if(m_hasLooped || m_nextWriteLine >= m_numVisibleLines)
	{
		m_topLine++;

		if(m_topLine >= m_maxLinesOfText)
			m_topLine = 0;

		//If we have a scroll bar and its not all the way scrolled down
		if(m_scrollBar && m_scrollBar->m_curPosition != m_scrollBar->m_fullSize - m_scrollBar->m_sliderSize)
		{
			//Scroll the slider up one line bottom(to keep the same text visible)
			m_scrollBar->m_curPosition -= (m_scrollBar->m_fullSize - m_scrollBar->m_sliderSize) / (m_maxLinesOfText - m_numVisibleLines);
			m_scrollBar->Update();
		}
	}

	//Save this for the Wrap Line call
	int lastLine = m_nextWriteLine;

	//NEXT!
	m_nextWriteLine++;

	//If we hit the max lines, then loop back to the beginning
	if(m_nextWriteLine == m_maxLinesOfText)
	{
		m_hasLooped		= true;
		m_nextWriteLine = 0;
	}

	//Make sure the line isn't wider than the text box
	WrapLine(lastLine);
}

void CMenuTextBox::SetText(CString newText, int line)
{
	m_linesOfText[line].m_text = newText;
	m_curLine = line;

	if (newText.GetLength() <= 0)
		this->m_fullText.Empty();

	/* We're not using the clicking for the cursor in the demo
	if(!m_multilined)
	{
		//TODO: Keep track of the left side of the left most character
		//and the right side of the right most character for use with the cursor
	}*/

	// Update full text
	for(int i=0; i<m_maxLinesOfText; i++)
	{
		if(m_linesOfText[i].m_text && m_linesOfText[i].m_text.GetLength() > 0)
		{
			this->m_fullText += m_linesOfText[i].m_text;
		}
	}

	// Update new text
	if(m_linesOfText[line].m_textMesh)
	{
		m_linesOfText[line].m_textMesh->SafeDelete();
		delete m_linesOfText[line].m_textMesh;
		m_linesOfText[line].m_textMesh = NULL;
	}

	// Update cursor pos
	if(this->m_showCursor)
	{
		//Get the X Coordinate of the click with respect to the text
		m_cursor.m_posX = 0;

		//Get the character they clicked on
		int curChar, charWidth;
		m_curCharacter = 0;
		while(m_curCharacter < m_linesOfText[m_curLine].m_text.GetLength())
		{
			//Get the current character
			curChar = m_linesOfText[m_curLine].m_text.GetAt(m_curCharacter++) - 32;

			//Get the width of this character, including spacing
			/*charWidth = m_font->m_characterSpacingBefore[curChar] +
						m_font->m_characterTextureWidth[curChar] +
						m_font->m_characterSpacingAfter[curChar];*/
			charWidth = (int)(m_font->m_characterSpacingBefore[curChar] +
						m_font->m_characterTextureWidth[curChar] +
						m_font->m_characterSpacingAfter[curChar]);
			//WARNING: C4244 --> float to int

			//Keep up with the cursor's X position
			m_cursor.m_posX += charWidth;
		}

		//Calculate the cursor's Y position
		m_cursor.m_posY = m_curLine * m_font->m_height;//
	}
}

void CMenuTextBox::WrapLine(int lineNumber, int colorType)
{
	//Figure out max width so we don't have to calculate it every time
	float maxWidth = m_width - 2 * m_textInset;

	/*int width = 0, curChar;*/
	int curChar;
	float width = 0.0f;
	//WARNING: C4244 --> float to int; maybe too refined (width comparison)
	//TODO: --> set precision


	//Multilined must be handled seperately from single line
	if(m_multilined)
	{
		//Loop from left to right through the line
		for(int i = 0; i < m_linesOfText[lineNumber].m_text.GetLength(); i++)
		{
			//Calculate the curChar because we need it a few times
			curChar = m_linesOfText[lineNumber].m_text.GetAt(i) - 32;

			//Add the spacing before and width of this character
			width += m_font->m_characterSpacingBefore[curChar] + m_font->m_characterTextureWidth[curChar];

			//If we're wider than the text box
			if(width > maxWidth)
			{
				//Search backwards to the previous space
				int spaceChar = i;
				while(spaceChar > 0)
				{
					if(m_linesOfText[lineNumber].m_text.GetAt(spaceChar - 1) == ' ')
					{
						i = spaceChar;
						break;
					}

					spaceChar--;
				}

				//Add a new line with the rest of the text
				AddLine(m_linesOfText[lineNumber].m_text.Mid(i), colorType);

				//Copy the colors
				m_linesOfText[lineNumber + 1].m_red		= m_linesOfText[lineNumber].m_red;
				m_linesOfText[lineNumber + 1].m_green	= m_linesOfText[lineNumber].m_green;
				m_linesOfText[lineNumber + 1].m_blue	= m_linesOfText[lineNumber].m_blue;
				m_linesOfText[lineNumber + 1].m_alpha	= m_linesOfText[lineNumber].m_alpha;

				//Set this lines text to be the left side
				m_linesOfText[lineNumber].m_text = m_linesOfText[lineNumber].m_text.Left(i - 1);

				//All done
				return;
			}

			//Add the spacing before the character
			width += m_font->m_characterSpacingBefore[curChar];
		}
	}
	else
	{
		// what the player SEES: the text, or one * per character for a password box
		CString shownText = m_fullText;
		if(m_password) shownText = CString('*', m_fullText.GetLength());
		//Loop from right to left through the line
		for(int i = shownText.GetLength() - 1; i >= 0; i--)
		{
			//Calculate the curChar because we need it a few times
			curChar = shownText.GetAt(i) - 32;

			//Add the spacing after and width of this character
			width += m_font->m_characterSpacingAfter[curChar] + m_font->m_characterTextureWidth[curChar];

			//If we're wider than the text box
			if(width > maxWidth)
			{
				//Set the text of the line to only the visible portion
				m_linesOfText->SetText(shownText.Mid(i + 1).GetBuffer());

				//Move the cursor to the far right of the text box
				m_cursor.m_posX = m_width + m_textInset - m_cursor.m_width;

				//All done
				return;
			}

			//Add the spacing before the character
			width += m_font->m_characterSpacingBefore[curChar];
		}

		//Set the line to be the whole text
		m_linesOfText->SetText(shownText.GetBuffer());

		//Move the cursor to the right of the text
		m_cursor.m_posX = width + m_textInset;
	}
}

void CMenuTextBox::Render(LPDIRECT3DDEVICE9 pD3DDevice,
						 CStateManagementObj *stateManager, 
						 CTextureDBObjList *textureDatabase,
						 float parentX,
						 float parentY)
{
	if(!pD3DDevice)
		return;

	//The text box is the parent now
	parentX += m_posX;
	parentY += m_posY;

	//Render each component, if they exist
	if(m_backgroundMiddle)
		m_backgroundMiddle->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	if(m_backgroundLeft)
		m_backgroundLeft->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	if(m_backgroundRight)
		m_backgroundRight->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	if(m_scrollBar)
		m_scrollBar->Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	//Get the text's starting position
	float textX = parentX + m_textInset;
	float textY = parentY + m_textInset;

	if(m_multilined)
	{
		//TODO: Test m_vertAlignment for top and middle settings

		int curLine = m_topLine + m_numVisibleLines - 1;
		int numLines = m_numVisibleLines;

		//If we have a scroll bar, check for it's scroll offset
		if(m_scrollBar)
			curLine -= m_maxLinesOfText - (m_scrollBar->m_curFirstItem + m_numVisibleLines);

		//Make sure we're not trying to render uncreated lines
		if(!m_hasLooped && curLine >= m_nextWriteLine)
			curLine = m_nextWriteLine - 1;
		else if(curLine >= m_maxLinesOfText)
			curLine %= m_maxLinesOfText;
		else if(m_hasLooped && curLine < 0)
			curLine += m_maxLinesOfText;

		//Loop through the linesOfText as long as we have a valid curLine
		while(curLine >= 0)
		{
			//Render this line
			m_linesOfText[curLine].Render(pD3DDevice, stateManager, textureDatabase, textX, textY);

			//Move to the next line up
			textY += m_font->m_height;

			//Onto the next line
			curLine--;
			numLines--;

			//If we reach the end of the array of lines
			if(curLine < 0)
			{
				//If we've already looped through, start back at the beginning
				if(m_hasLooped)
					curLine = m_maxLinesOfText - 1;
				else //Otherwise, we're done
					break;
			}

			//If we've rendered m_numVisibleLines lines, we're done
			if(numLines <= 0)
				break;
		}
	}
	else
		//Render the first line
		m_linesOfText[0].Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);

	//Render the cursor over the top of everything
	if(m_showCursor)
		m_cursor.Render(pD3DDevice, stateManager, textureDatabase, parentX, parentY);
}

void CMenuTextBox::Resize()
{
	//Start the left and right coordinates for the middle as 0 and the button's width
	float middleLeft	= 0.0f;
	float middleRight	= m_width;

	//If there's a left component, set its coordinates
	//and shift the middle's left coordinate to the right by the component's width
	if(m_backgroundLeft)
	{
		m_backgroundLeft->m_height = m_height;
		middleLeft += m_backgroundLeft->m_width;
	}

	//If there's a right component, set its coordinates
	//and shift the middle's right coordinate to the left by the component's width
	if(m_backgroundRight)
	{
		m_backgroundRight->m_posX	= m_width - m_backgroundRight->m_width;
		m_backgroundRight->m_height	= m_height;
		middleRight = m_backgroundRight->m_posX;
	}

	//If there's a middle component, set its coordinates
	if(m_backgroundMiddle && middleRight > middleLeft)
	{
		m_backgroundMiddle->m_posX	 = middleLeft;
		m_backgroundMiddle->m_width	 = middleRight - middleLeft;
		m_backgroundMiddle->m_height = m_height;
	}

	//Set the cursor's Y position to the same as the text
	m_cursor.m_posY = m_textInset;

	//Calculate the number of visible lines that will fit in the text box
	/*m_numVisibleLines = (m_height - 2 * m_textInset) / m_font->m_height;*/
	m_numVisibleLines = (int)((m_height - 2 * m_textInset) / m_font->m_height);
	//WARNING: C4244 --> float to int

	if(m_numVisibleLines > m_maxLinesOfText)
		m_numVisibleLines = m_maxLinesOfText;

	//If we have a scroll bar, set its slider properties
	if(m_scrollBar)
	{
		//Set the number of items and visible items for the scroll bar
		m_scrollBar->m_numItems		= m_maxLinesOfText;
		m_scrollBar->m_visibleItems	= m_numVisibleLines;
		m_scrollBar->Update();
	}
}

void CMenuTextBox::Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice)
{
	*pClone = new CMenuTextBox();
	Copy(*pClone, pD3DDevice);
}

void CMenuTextBox::Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice)
{
	//Copy base properties
	CMenuVisual::Copy(pCopy, pD3DDevice);

	//Copy Text Box settings
	CMenuTextBox *textBoxCopy = (CMenuTextBox*)pCopy;

	//Visible Objects
	if(m_backgroundLeft)
		m_backgroundLeft->Clone((CMenuVisual**)&textBoxCopy->m_backgroundLeft, pD3DDevice);

	if(m_backgroundMiddle)
		m_backgroundMiddle->Clone((CMenuVisual**)&textBoxCopy->m_backgroundMiddle, pD3DDevice);

	if(m_backgroundRight)
		m_backgroundRight->Clone((CMenuVisual**)&textBoxCopy->m_backgroundRight, pD3DDevice);

	if(m_scrollBar)
		m_scrollBar->Clone((CMenuVisual**)&textBoxCopy->m_scrollBar, pD3DDevice);

    if(m_linesOfText)
	   if(m_maxLinesOfText > 0)
	      textBoxCopy->m_linesOfText = new CMenuText[m_maxLinesOfText];
	
	//Settable Parameters
	textBoxCopy->m_font					= m_font;
	textBoxCopy->m_maxLinesOfText       = m_maxLinesOfText;
	textBoxCopy->m_horAlignment			= m_horAlignment;
	textBoxCopy->m_vertAlignment		= m_vertAlignment;
	textBoxCopy->m_multilined			= m_multilined;
	textBoxCopy->m_editable				= m_editable;
	textBoxCopy->m_password				= m_password;
	textBoxCopy->m_textInset			= m_textInset;

	//Temperary Variables
	textBoxCopy->m_red		= m_red;
	textBoxCopy->m_green	= m_green;
	textBoxCopy->m_blue		= m_blue;
	textBoxCopy->m_alpha	= m_alpha;
	textBoxCopy->m_numColors= m_numColors;
	textBoxCopy->m_fontName	= m_fontName;

    //scripts
    if(m_enterPressScript)
       m_enterPressScript->Clone(&textBoxCopy->m_enterPressScript);

	if(m_mouseOverScript)
       m_mouseOverScript->Clone(&textBoxCopy->m_mouseOverScript);

	if(m_mouseOutScript)
       m_mouseOutScript->Clone(&textBoxCopy->m_mouseOutScript);

}

CMenuTextBox::~CMenuTextBox()
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

	//Delete all the scripts
	if(m_enterPressScript)
	{
		delete m_enterPressScript;
		m_enterPressScript = NULL;
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
	SafeDeleteArray(m_red);
	SafeDeleteArray(m_green);
	SafeDeleteArray(m_blue);
	SafeDeleteArray(m_alpha);
	SafeDeleteArray(m_linesOfText);
	SafeDelete(m_scrollBar);
	//SafeDelete(m_font);
}

bool CMenuTextBox::HandleTyping(CInput *pInput, DWORD curTime)
{
	bool keypressed = false,
		bIsMaxLength = false;
	//bool capitalMode = pInput->m_shiftOn ^ pInput->m_capsOn;

	CString *curLine = &m_linesOfText[m_curLine].m_text;
	if(!m_multilined)
		curLine = &m_fullText;

	// Max Length
	if((this->m_iMaxLength > 0) 
	&& (this->m_fullText.GetLength() >= this->m_iMaxLength))
		bIsMaxLength = true;

	int iKeyCode = -1;
	InputTypes eType = VT_COMMAND;
	CString strCommand = pInput->GetInputAsString(curTime, iKeyCode, eType);
	if(strCommand != "") 
	{
		keypressed = true;
		if(iKeyCode == DIK_BACKSPACE) 
		{
			curLine->Delete(curLine->GetLength() - 1, 1);
		}
		else if(iKeyCode == DIK_RETURN && !bIsMaxLength)
		{
			if(!m_multilined && m_enterPressScript)
				m_enterPressScript->ExecuteScript();
		}
	}
	else if(!bIsMaxLength)
	{
		// Numeric Only
		if(this->m_bIsNumericOnly)
		{
			eType = VT_NUMERIC;
			CString strInput = pInput->GetInputAsString(curTime, iKeyCode, eType);
			if (strInput == "")
			{
				eType = VT_COMMAND;
				strInput = pInput->GetInputAsString(curTime, iKeyCode, eType);
			}

			if (strInput != "")
			{
				keypressed = true;
				(*curLine) += strInput;
			}
		}
		// Alphanumerics
		else
		{
			eType = VT_ALPHANUMERIC;
			CString strInput = pInput->GetInputAsString(curTime, iKeyCode, eType);
			if(strInput != "") 
			{
				keypressed = true;
				(*curLine) += strInput;
			}
		}
	}

	/*/
	for(int curKey = 0; curKey < KEYBOARD_MAX_KEYS; curKey++)
	{
		//See if we should actually print anything that's being typed
		if(pInput->m_keyboardKeys[curKey].JustPressed() || pInput->m_keyboardKeys[curKey].ShouldRepeat(curTime))
		{
			keypressed = true;

			switch(curKey)
			{
				case DIK_RETURN:
				case DIK_NUMPADENTER:
					if(m_multilined)
						*curLine += '\n';
					else if(m_enterPressScript)
						m_enterPressScript->ExecuteScript();
					break;
				case DIK_SPACE:
					*curLine += ' ';
					break;
				case DIK_A:
					if(capitalMode)
						*curLine += 'A';
					else
						*curLine += 'a';
					break;
				case DIK_B:
					if(capitalMode)
						*curLine += 'B';
					else
						*curLine += 'b';
					break;
				case DIK_C:
					if(capitalMode)
						*curLine += 'C';
					else
						*curLine += 'c';
					break;
				case DIK_D:
					if(capitalMode)
						*curLine += 'D';
					else
						*curLine += 'd';
					break;
				case DIK_E:
					if(capitalMode)
						*curLine += 'E';
					else
						*curLine += 'e';
					break;
				case DIK_F:
					if(capitalMode)
						*curLine += 'F';
					else
						*curLine += 'f';
					break;
				case DIK_G:
					if(capitalMode)
						*curLine += 'G';
					else
						*curLine += 'g';
					break;
				case DIK_H:
					if(capitalMode)
						*curLine += 'H';
					else
						*curLine += 'h';
					break;
				case DIK_I:
					if(capitalMode)
						*curLine += 'I';
					else
						*curLine += 'i';
					break;
				case DIK_J:
					if(capitalMode)
						*curLine += 'J';
					else
						*curLine += 'j';
					break;
				case DIK_K:
					if(capitalMode)
						*curLine += 'K';
					else
						*curLine += 'k';
					break;
				case DIK_L:
					if(capitalMode)
						*curLine += 'L';
					else
						*curLine += 'l';
					break;
				case DIK_M:
					if(capitalMode)
						*curLine += 'M';
					else
						*curLine += 'm';
					break;
				case DIK_N:
					if(capitalMode)
						*curLine += 'N';
					else
						*curLine += 'n';
					break;
				case DIK_O:
					if(capitalMode)
						*curLine += 'O';
					else
						*curLine += 'o';
					break;
				case DIK_P:
					if(capitalMode)
						*curLine += 'P';
					else
						*curLine += 'p';
					break;
				case DIK_Q:
					if(capitalMode)
						*curLine += 'Q';
					else
						*curLine += 'q';
					break;
				case DIK_R:
					if(capitalMode)
						*curLine += 'R';
					else
						*curLine += 'r';
					break;
				case DIK_S:
					if(capitalMode)
						*curLine += 'S';
					else
						*curLine += 's';
					break;
				case DIK_T:
					if(capitalMode)
						*curLine += 'T';
					else
						*curLine += 't';
					break;
				case DIK_U:
					if(capitalMode)
						*curLine += 'U';
					else
						*curLine += 'u';
					break;
				case DIK_V:
					if(capitalMode)
						*curLine += 'V';
					else
						*curLine += 'v';
					break;
				case DIK_W:
					if(capitalMode)
						*curLine += 'W';
					else
						*curLine += 'w';
					break;
				case DIK_X:
					if(capitalMode)
						*curLine += 'X';
					else
						*curLine += 'x';
					break;
				case DIK_Y:
					if(capitalMode)
						*curLine += 'Y';
					else
						*curLine += 'y';
					break;
				case DIK_Z:
					if(capitalMode)
						*curLine += 'Z';
					else
						*curLine += 'z';
					break;
				case DIK_1:
					if(pInput->m_shiftOn)
						*curLine += '!';
					else
						*curLine += '1';
					break;
				case DIK_2:
					if(pInput->m_shiftOn)
						*curLine += '@';
					else
						*curLine += '2';
					break;
				case DIK_3:
					if(pInput->m_shiftOn)
						*curLine += '#';
					else
						*curLine += '3';
					break;
				case DIK_4:
					if(pInput->m_shiftOn)
						*curLine += '$';
					else
						*curLine += '4';
					break;
				case DIK_5:
					if(pInput->m_shiftOn)
						*curLine += '%';
					else
						*curLine += '5';
					break;
				case DIK_6:
					if(pInput->m_shiftOn)
						*curLine += '^';
					else
						*curLine += '6';
					break;
				case DIK_7:
					if(pInput->m_shiftOn)
						*curLine += '&';
					else
						*curLine += '7';
					break;
				case DIK_8:
					if(pInput->m_shiftOn)
						*curLine += '*';
					else
						*curLine += '8';
					break;
				case DIK_9:
					if(pInput->m_shiftOn)
						*curLine += '(';
					else
						*curLine += '9';
					break;
				case DIK_0:
					if(pInput->m_shiftOn)
						*curLine += ')';
					else
						*curLine += '0';
					break;
				case DIK_MINUS:
					if(pInput->m_shiftOn)
						*curLine += '_';
					else
						*curLine += '-';
					break;
				case DIK_EQUALS:
					if(pInput->m_shiftOn)
						*curLine += '+';
					else
						*curLine += '=';
					break;
				case DIK_LBRACKET:
					if(pInput->m_shiftOn)
						*curLine += '{';
					else
						*curLine += '[';
					break;
				case DIK_RBRACKET:
					if(pInput->m_shiftOn)
						*curLine += '}';
					else
						*curLine += ']';
					break;
				case DIK_BACKSLASH:
					if(pInput->m_shiftOn)
						*curLine += '|';
					else
						*curLine += "\\";
					break;
				case DIK_SEMICOLON:
					if(pInput->m_shiftOn)
						*curLine += ':';
					else
						*curLine += ';';
					break;
				case DIK_APOSTROPHE:
					if(pInput->m_shiftOn)
						*curLine += '"';
					else
						*curLine += '\'';
					break;
				case DIK_COMMA:
					if(pInput->m_shiftOn)
						*curLine += '<';
					else
						*curLine += ',';
					break;
				case DIK_PERIOD:
					if(pInput->m_shiftOn)
						*curLine += '>';
					else
						*curLine += '.';
					break;
				case DIK_SLASH:
					if(pInput->m_shiftOn)
						*curLine += '?';
					else
						*curLine += '/';
					break;
				case DIK_GRAVE:
					if(pInput->m_shiftOn)
						*curLine += '~';
					else
						*curLine += '`';
					break;
				case DIK_NUMPAD1:
					*curLine += '1';
					break;
				case DIK_NUMPAD2:
					*curLine += '2';
					break;
				case DIK_NUMPAD3:
					*curLine += '3';
					break;
				case DIK_NUMPAD4:
					*curLine += '4';
					break;
				case DIK_NUMPAD5:
					*curLine += '5';
					break;
				case DIK_NUMPAD6:
					*curLine += '6';
					break;
				case DIK_NUMPAD7:
					*curLine += '7';
					break;
				case DIK_NUMPAD8:
					*curLine += '8';
					break;
				case DIK_NUMPAD9:
					*curLine += '9';
					break;
				case DIK_NUMPAD0:
					*curLine += '0';
					break;
				case DIK_NUMPADPLUS:
					*curLine += '+';
					break;
				case DIK_NUMPADMINUS:
					*curLine += '-';
					break;
				case DIK_NUMPADSLASH:
					*curLine += "\\";
					break;
				case DIK_NUMPADSTAR:
					*curLine += '*';
					break;
				case DIK_BACKSPACE:
					curLine->Delete(curLine->GetLength() - 1, 1);
					break;
			}
		}
	}
	//*/

	if(keypressed)
	{
		/*
		if(m_linesOfText[m_curLine].m_textMesh)
		{
			m_linesOfText[m_curLine].m_textMesh->SafeDelete();
			delete m_linesOfText[m_curLine].m_textMesh;
			m_linesOfText[m_curLine].m_textMesh = NULL;
		}
		*/
		m_linesOfText[m_curLine].SetText((*curLine).GetBuffer());
		(*curLine).ReleaseBuffer();
		return true;
	}

	return false;
}

void CMenuTextBox::Serialize(CXMLFileIO &xml)
{
	CMenuVisual::Serialize(xml);

	if(xml.m_isStoring)
	{
		xml.WriteAttribute("Version", 3);
		xml.WriteAttribute("Font", m_font->m_name.GetBuffer());
		//xml.WriteAttribute("Text", m_text.GetBuffer());
		xml.WriteAttribute("MaxLinesOfText", m_maxLinesOfText);
		xml.WriteAttribute("MaxLength", this->m_iMaxLength);
		xml.WriteAttribute("Alignment", m_horAlignment);
		xml.WriteAttribute("Multilined", m_multilined);
		xml.WriteAttribute("IsEditable", m_editable);
		xml.WriteAttribute("IsNumericOnly", this->m_bIsNumericOnly);
		xml.WriteAttribute("IsPassword", m_password);
		xml.WriteAttribute("TextInset", (int)m_textInset);
		xml.WriteAttribute("NumberOfColors", m_numColors);

		//Write out each color
		for(int i = 0; i < m_numColors; i++)
		{
			xml.WriteAttribute("Red", (int)(m_red[i] * 255));
			xml.WriteAttribute("Green", (int)(m_green[i] * 255));
			xml.WriteAttribute("Blue", (int)(m_blue[i] * 255));
			xml.WriteAttribute("Alpha", (int)(m_alpha[i] * 255));
		}

		//Write out the cursor
		xml.WriteOpeningTag("Cursor");
		m_cursor.Serialize(xml);
		xml.WriteClosingTag("Cursor");

		//Write out the background components
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

		//Write out the keyPress script
		if(m_enterPressScript)
		if(!m_enterPressScript->m_scriptText.IsEmpty())
		{
			xml.WriteOpeningTag("EnterPressScript");
			xml.WriteMultilinedAttribute(m_enterPressScript->m_scriptText.GetBuffer());
			xml.WriteClosingTag("EnterPressScript");
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

		xml.ReadAttribute(version);
		xml.ReadAttribute(buffer, MAX_BUFFER); m_fontName = buffer;
		//xml.ReadAttribute(buffer, MAX_BUFFER); m_text = buffer;
		xml.ReadAttribute(m_maxLinesOfText);
		if(version >= 2) xml.ReadAttribute(m_iMaxLength);
		xml.ReadAttribute(m_horAlignment);
		xml.ReadAttribute(m_multilined);
		xml.ReadAttribute(m_editable);
		if(version >= 2) xml.ReadAttribute(m_bIsNumericOnly);
		if(version >= 3) xml.ReadAttribute(m_password); else m_password = false;
		xml.ReadAttribute(m_textInset);
		xml.ReadAttribute(m_numColors);

        SafeDeleteArray(m_red);
		SafeDeleteArray(m_green);
		SafeDeleteArray(m_blue);
		SafeDeleteArray(m_alpha);

		//Create the color arrays
		m_red	= new float[m_numColors];
		m_green	= new float[m_numColors];
		m_blue	= new float[m_numColors];
		m_alpha	= new float[m_numColors];

		//Read in each color
		for(int i = 0; i < m_numColors; i++)
		{
			xml.ReadAttribute(m_red[i]);	m_red[i]	/= 255.0f;
			xml.ReadAttribute(m_green[i]);	m_green[i]	/= 255.0f;
			xml.ReadAttribute(m_blue[i]);	m_blue[i]	/= 255.0f;
			xml.ReadAttribute(m_alpha[i]);	m_alpha[i]	/= 255.0f;
		}

		//Read in the cursor
		xml.ReadTag(buffer, MAX_BUFFER, closing); //Read in <Cursor>
		m_cursor.Serialize(xml);
		//Closing tag is read by cursor

		//Read Next Opening Tag or </TextBox>
		xml.ReadTag(buffer, MAX_BUFFER, closing);

		if(strcmp(buffer, "BackgroundLeft") == 0)
		{
			SafeDelete(m_backgroundLeft);
			m_backgroundLeft = new CMenuImage();
			m_backgroundLeft->Serialize(xml);
			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </TextBox>
		}

		if(strcmp(buffer, "BackgroundMiddle") == 0)
		{
			SafeDelete(m_backgroundMiddle);
			m_backgroundMiddle = new CMenuImage();
			m_backgroundMiddle->Serialize(xml);
			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </TextBox>
		}

		if(strcmp(buffer, "BackgroundRight") == 0)
		{
			SafeDelete(m_backgroundRight);
			m_backgroundRight = new CMenuImage();
			m_backgroundRight->Serialize(xml);
			xml.ReadTag(buffer, MAX_BUFFER, closing); //Read Next Opening Tag or </TextBox>
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

		//If the KeyPressScript is present
		if(strcmp(buffer, "EnterPressScript") == 0)
		{
			//We've got a keyPress script, create a new script instance
			SafeDelete(m_enterPressScript);
			m_enterPressScript = new CMenuScript();

			while(1)
			{
				//Read next script line or </KeyPressScript>
				xml.ReadLineOfAttribute(buffer, MAX_BUFFER);

				//If it is </KeyPressScript>, break
				if(buffer[0] == '<')
					break;

				//Add the line to the script
				m_enterPressScript->AddLine(buffer);
			}

			//Read in the first <StateImage>
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
        SafeDeleteArray(m_linesOfText);
		m_linesOfText = new CMenuText[m_maxLinesOfText];
	}
}
