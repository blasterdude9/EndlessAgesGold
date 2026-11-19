#include "stdafx.h"
#include "CInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

void ButtonState::UpdateState(bool bDown, DWORD dwTime)
{
	if(bDown)
	{
		if(m_iKeyState > BUTTONSTATE_HELD) //Optimized - BUTTONSTATE_RELEASED and BUTTONSTATE_UP are > BUTTONSTATE_HELD
		{ 
			m_dwPressTime = m_dwLastRepeatTime = dwTime;
			m_dwCurRepeatSpacing = REPEAT_MAX_SPACING;
			m_iKeyState = BUTTONSTATE_PRESSED;
		}
		else
			m_iKeyState = BUTTONSTATE_HELD;
	}
	else
	{ 
		if(IsDown())
			m_iKeyState = BUTTONSTATE_RELEASED;
		else
			m_iKeyState = BUTTONSTATE_UP;
	}
}

bool ButtonState::ShouldRepeat(DWORD dwTime)
{
	if(dwTime - m_dwLastRepeatTime >= m_dwCurRepeatSpacing)
	if(IsDown())
	{
		m_dwCurRepeatSpacing -= REPEAT_SPACINGCHANGE;
		if(m_dwCurRepeatSpacing < REPEAT_MIN_SPACING)
			m_dwCurRepeatSpacing = REPEAT_MIN_SPACING;

		m_dwLastRepeatTime = dwTime;
		return true;
	}
	return false;
}

CInput::CInput(int *screenWidth, int *screenHeight)
{
	m_directInput		= NULL;
    m_directKeyboard	= NULL;
    m_directMouse		= NULL;
	m_mouseHiResX		= 0.0f;
	m_mouseHiResY		= 0.0f;
	m_mouseAbsPosX		= 0.0f;
	m_mouseAbsPosY		= 0.0f;
	m_menuMouseChangeX	= 0.0f;
	m_menuMouseChangeY	= 0.0f;
	m_mouseMaxX			= screenWidth;
	m_mouseMaxY			= screenHeight;
	m_invertMouse		= false;
	m_shiftOn			= false;
	m_altOn				= false;
	m_ctrlOn			= false;
	m_capsOn			= false;
	m_mouseWheelZ       = 0.0f;
}

CInput::~CInput()
{
	if(m_directInput)
		m_directInput->Release();
		m_directInput = 0;
	if(m_directKeyboard)
		m_directKeyboard->Release();
		m_directKeyboard = 0;
	if(m_directMouse)
		m_directMouse->Release();
		m_directMouse = 0;
}

bool CInput::UpdateInput(DWORD curTime)
{
	//Get current keyboard state
	BYTE keystates[KEYBOARD_MAX_KEYS];
	HRESULT err = m_directKeyboard->GetDeviceState(KEYBOARD_MAX_KEYS, keystates);
    if(err != S_OK)
		return false;

	//Update all key states
	for(int i = 0; i < KEYBOARD_MAX_KEYS; i++)
		m_keyboardKeys[i].UpdateState(keystates[i] & 0x80, curTime);

	//Update the shift / Control / Alt state
	m_shiftOn = m_keyboardKeys[DIK_LSHIFT].IsDown() || m_keyboardKeys[DIK_RSHIFT].IsDown();
	m_altOn = m_keyboardKeys[DIK_LALT].IsDown() || m_keyboardKeys[DIK_RALT].IsDown();
	m_ctrlOn = m_keyboardKeys[DIK_LCONTROL].IsDown() || m_keyboardKeys[DIK_RCONTROL].IsDown();

	//Update capslock
	if(m_keyboardKeys[DIK_CAPSLOCK].JustPressed())
		m_capsOn = !m_capsOn;

	//Store old position
	m_mousepointStart.lX = m_mousepointTarget.lX;
	m_mousepointStart.lY = m_mousepointTarget.lY;
	float oldPosX = m_mouseAbsPosX;
	float oldPosY = m_mouseAbsPosY;
	
	//Get mouse movement since last frame
	err = m_directMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_mousepointTarget);
	if(err != S_OK)
		return false;

	//Update mouse position
	m_mouseHiResX	 = (float)m_mousepointTarget.lX;
	m_mouseHiResY	 = (float)m_mousepointTarget.lY;
	m_mouseWheelZ	 = (float)m_mousepointTarget.lZ;
	m_mouseAbsPosX	+= m_mouseHiResX;
	m_mouseAbsPosY	-= m_mouseHiResY;

	if(m_mouseAbsPosX < 0)
		m_mouseAbsPosX = 0;
	if(m_mouseAbsPosX > *m_mouseMaxX)
		m_mouseAbsPosX = (float)*m_mouseMaxX;

	if(m_mouseAbsPosY < 0)
		m_mouseAbsPosY = 0;
	if(m_mouseAbsPosY > *m_mouseMaxY)
		m_mouseAbsPosY = (float)*m_mouseMaxY;

	//Calculate the change for dragging menus and such
	m_menuMouseChangeX = m_mouseAbsPosX - oldPosX;
	m_menuMouseChangeY = m_mouseAbsPosY - oldPosY;

	//Update all mouse button states
	for(int i = 0; i < MOUSE_MAX_BUTTONS; i++)
		m_mouseButtons[i].UpdateState(m_mousepointTarget.rgbButtons[i] & 0x80, curTime);

	return true;
}

/*/bool CInput::GetPressedKeyString(CString & keyString)
{//semi slow
	//
    if(m_keyboardKeys[DIK_NUMPAD1].IsDown()){keyString = "Num 1";return true;}
    if(m_keyboardKeys[DIK_NUMPAD2].IsDown()){keyString = "Num 2";return true;}
	if(m_keyboardKeys[DIK_NUMPAD3].IsDown()){keyString = "Num 3";return true;}
	if(m_keyboardKeys[DIK_NUMPAD4].IsDown()){keyString = "Num 4";return true;}
	if(m_keyboardKeys[DIK_NUMPAD5].IsDown()){keyString = "Num 5";return true;}
	if(m_keyboardKeys[DIK_NUMPAD6].IsDown()){keyString = "Num 6";return true;}
	if(m_keyboardKeys[DIK_NUMPAD7].IsDown()){keyString = "Num 7";return true;}
	if(m_keyboardKeys[DIK_NUMPAD8].IsDown()){keyString = "Num 8";return true;}
	if(m_keyboardKeys[DIK_NUMPAD9].IsDown()){keyString = "Num 9";return true;}
	if(m_keyboardKeys[DIK_NUMPAD0].IsDown()){keyString = "Num 0";return true;}
	if(m_keyboardKeys[DIK_1].IsDown()){keyString = "1";return true;}
	if(m_keyboardKeys[DIK_2].IsDown()){keyString = "2";return true;}
	if(m_keyboardKeys[DIK_3].IsDown()){keyString = "3";return true;}
	if(m_keyboardKeys[DIK_4].IsDown()){keyString = "4";return true;}
	if(m_keyboardKeys[DIK_5].IsDown()){keyString = "5";return true;}
	if(m_keyboardKeys[DIK_6].IsDown()){keyString = "6";return true;}
	if(m_keyboardKeys[DIK_7].IsDown()){keyString = "7";return true;}
    if(m_keyboardKeys[DIK_8].IsDown()){keyString = "8";return true;}
	if(m_keyboardKeys[DIK_9].IsDown()){keyString = "9";return true;}
	if(m_keyboardKeys[DIK_0].IsDown()){keyString = "0";return true;}
	if(m_keyboardKeys[DIK_A].IsDown()){keyString = "A";return true;}
	if(m_keyboardKeys[DIK_B].IsDown()){keyString = "B";return true;}
	if(m_keyboardKeys[DIK_C].IsDown()){keyString = "C";return true;}
	if(m_keyboardKeys[DIK_D].IsDown()){keyString = "D";return true;}
	if(m_keyboardKeys[DIK_E].IsDown()){keyString = "E";return true;}
	if(m_keyboardKeys[DIK_F].IsDown()){keyString = "F";return true;}
	if(m_keyboardKeys[DIK_G].IsDown()){keyString = "G";return true;}
	if(m_keyboardKeys[DIK_H].IsDown()){keyString = "H";return true;}
	if(m_keyboardKeys[DIK_I].IsDown()){keyString = "I";return true;}
	if(m_keyboardKeys[DIK_J].IsDown()){keyString = "J";return true;}
	if(m_keyboardKeys[DIK_K].IsDown()){keyString = "K";return true;}
	if(m_keyboardKeys[DIK_L].IsDown()){keyString = "L";return true;}
	if(m_keyboardKeys[DIK_M].IsDown()){keyString = "M";return true;}
	if(m_keyboardKeys[DIK_N].IsDown()){keyString = "N";return true;}
	if(m_keyboardKeys[DIK_O].IsDown()){keyString = "O";return true;}
	if(m_keyboardKeys[DIK_P].IsDown()){keyString = "P";return true;}
	if(m_keyboardKeys[DIK_Q].IsDown()){keyString = "Q";return true;}
	if(m_keyboardKeys[DIK_R].IsDown()){keyString = "R";return true;}
	if(m_keyboardKeys[DIK_S].IsDown()){keyString = "S";return true;}
	if(m_keyboardKeys[DIK_T].IsDown()){keyString = "T";return true;}
	if(m_keyboardKeys[DIK_U].IsDown()){keyString = "U";return true;}
	if(m_keyboardKeys[DIK_V].IsDown()){keyString = "V";return true;}
	if(m_keyboardKeys[DIK_W].IsDown()){keyString = "W";return true;}
	if(m_keyboardKeys[DIK_X].IsDown()){keyString = "X";return true;}
	if(m_keyboardKeys[DIK_Y].IsDown()){keyString = "Y";return true;}
	if(m_keyboardKeys[DIK_Z].IsDown()){keyString = "Z";return true;}
	if(m_keyboardKeys[DIK_SPACE].IsDown()){keyString = "Space Bar";return true;}
	if(m_keyboardKeys[DIK_ADD].IsDown()){keyString = "+";return true;}
	if(m_keyboardKeys[DIK_SUBTRACT].IsDown()){keyString = "-";return true;}
	if(m_keyboardKeys[DIK_EQUALS].IsDown()){keyString = "=";return true;}
	if(m_keyboardKeys[DIK_LBRACKET].IsDown()){keyString = "[";return true;}
	if(m_keyboardKeys[DIK_RBRACKET].IsDown()){keyString = "]";return true;}
	if(m_keyboardKeys[DIK_SEMICOLON].IsDown()){keyString = ":";return true;}
	if(m_keyboardKeys[DIK_APOSTROPHE].IsDown()){keyString = "'";return true;}
	if(m_keyboardKeys[DIK_COMMA].IsDown()){keyString = ",";return true;}
	if(m_keyboardKeys[DIK_PERIOD].IsDown()){keyString = ".";return true;}
	if(m_keyboardKeys[DIK_BACKSLASH].IsDown()){keyString = "\\";return true;}
	if(m_keyboardKeys[DIK_MULTIPLY].IsDown()){keyString = "*";return true;}
	if(m_keyboardKeys[DIK_MINUS].IsDown()){keyString = "-";return true;}
	if(m_keyboardKeys[DIK_SLASH].IsDown()){keyString = "?";return true;}
	if(m_keyboardKeys[DIK_ESCAPE].IsDown()){keyString = "Escape";return true;}
	///

	
	if(m_mouseButtons[0].IsDown()){keyString = "Left Click";return true;}
	if(m_mouseButtons[1].IsDown()){keyString = "Right Click";return true;}
	if(m_mouseButtons[2].IsDown()){keyString = "Middle Click";return true;}

	return false;
}//*/

///////////////////////////////////////////////////////////////////////////////
// Class:		CInput
// Function:	GetKeyInput(DWORD, int&, InputTypes&)
// Description:	Return the the most recent input event as a string.
//	Takes the current timestamp as a parameter.
///////////////////////////////////////////////////////////////////////////////

CString
CInput::GetInputAsString(DWORD iTimeStamp, int &iKeyCode)
{
	InputTypes eType = VT_ALL;
	return this->GetInputAsString(iTimeStamp, iKeyCode, eType);
}

CString
CInput::GetInputAsString(DWORD iTimeStamp, int &iKeyCode, InputTypes &eType)
{
	CString strInput;
	switch(eType)
	{
		case VT_NUMERIC:
		{
			strInput = this->GetKeyInputAsString(iTimeStamp, iKeyCode);
			switch(iKeyCode)
			{
				case DIK_1: case DIK_NUMPAD1:
				case DIK_2: case DIK_NUMPAD2:
				case DIK_3: case DIK_NUMPAD3:
				case DIK_4: case DIK_NUMPAD4:
				case DIK_5: case DIK_NUMPAD5:
				case DIK_6: case DIK_NUMPAD6:
				case DIK_7: case DIK_NUMPAD7:
				case DIK_8: case DIK_NUMPAD8:
				case DIK_9: case DIK_NUMPAD9:
				case DIK_0: case DIK_NUMPAD0:
					return strInput;
				default: return NULL;
			}
			break;
		}
		case VT_ALPHANUMERIC:
		{
			strInput = this->GetKeyInputAsString(iTimeStamp, iKeyCode);
			break;
		}
		case VT_COMMAND:
		{
			strInput = this->GetCommandInputAsString(iTimeStamp, iKeyCode);
			break; 
		}
		case VT_MOUSE:
		{
			strInput = this->GetMouseInputAsString(iKeyCode);
			break;
		}
		case VT_ALL:
		default:
		{
			strInput = this->GetKeyInputAsString(iTimeStamp, iKeyCode);
			eType = VT_ALPHANUMERIC;
			if(strInput == "")
			{
				strInput = this->GetCommandInputAsString(iTimeStamp, iKeyCode);
				eType = VT_COMMAND;
				if(strInput == "")
				{
					strInput = this->GetMouseInputAsString(iKeyCode);
					eType = VT_MOUSE;
				}
			}
		}
	}
	return strInput;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInput
// Function:	GetKeyInputAsString(DWORD, int&)
// Description:	Return the currently pressed key interpreted as a string.
//	Takes the current timestamp as a parameter.
///////////////////////////////////////////////////////////////////////////////

CString
CInput::GetKeyInputAsString(DWORD iTimestamp, int &iKeyCode)
{
	bool bIsCapital = false;
	if(this->m_capsOn || this->m_shiftOn) bIsCapital = true;

	for(int iKey = 0; iKey < KEYBOARD_MAX_KEYS; iKey++)
	{
		if(this->m_keyboardKeys[iKey].JustPressed())
		{
			iKeyCode = iKey;
			switch(iKey)
			{
				case DIK_A:
					if(bIsCapital)
						return "A";
					else
						return "a";
				case DIK_B:
					if(bIsCapital)
						return "B";
					else
						return "b";
				case DIK_C:
					if(bIsCapital)
						return "C";
					else
						return "c";
				case DIK_D:
					if(bIsCapital)
						return "D";
					else
						return "d";
				case DIK_E:
					if(bIsCapital)
						return "E";
					else
						return "e";
				case DIK_F:
					if(bIsCapital)
						return "F";
					else
						return "f";
				case DIK_G:
					if(bIsCapital)
						return "G";
					else
						return "g";
				case DIK_H:
					if(bIsCapital)
						return "H";
					else
						return "h";
				case DIK_I:
					if(bIsCapital)
						return "I";
					else
						return "i";
				case DIK_J:
					if(bIsCapital)
						return "J";
					else
						return "j";
				case DIK_K:
					if(bIsCapital)
						return "K";
					else
						return "k";
				case DIK_L:
					if(bIsCapital)
						return "L";
					else
						return "l";
				case DIK_M:
					if(bIsCapital)
						return "M";
					else
						return "m";
				case DIK_N:
					if(bIsCapital)
						return "N";
					else
						return "n";
				case DIK_O:
					if(bIsCapital)
						return "O";
					else
						return "o";
				case DIK_P:
					if(bIsCapital)
						return "P";
					else
						return "p";
				case DIK_Q:
					if(bIsCapital)
						return "Q";
					else
						return "q";
				case DIK_R:
					if(bIsCapital)
						return "R";
					else
						return "r";
				case DIK_S:
					if(bIsCapital)
						return "S";
					else
						return "s";
				case DIK_T:
					if(bIsCapital)
						return "T";
					else
						return "t";
				case DIK_U:
					if(bIsCapital)
						return "U";
					else
						return "u";
				case DIK_V:
					if(bIsCapital)
						return "V";
					else
						return "v";
				case DIK_W:
					if(bIsCapital)
						return "W";
					else
						return "w";
				case DIK_X:
					if(bIsCapital)
						return "X";
					else
						return "x";
				case DIK_Y:
					if(bIsCapital)
						return "Y";
					else
						return "y";
				case DIK_Z:
					if(bIsCapital)
						return "Z";
					else
						return "z";
				case DIK_1:
					if(this->m_shiftOn)
						return "!";
					else
						return "1";
				case DIK_2:
					if(this->m_shiftOn)
						return "@";
					else
						return "2";
				case DIK_3:
					if(this->m_shiftOn)
						return "#";
					else
						return "3";
				case DIK_4:
					if(this->m_shiftOn)
						return "$";
					else
						return "4";
				case DIK_5:
					if(this->m_shiftOn)
						return "%";
					else
						return "5";
				case DIK_6:
					if(this->m_shiftOn)
						return "^";
					else
						return "6";
				case DIK_7:
					if(this->m_shiftOn)
						return "&";
					else
						return "7";
				case DIK_8:
					if(this->m_shiftOn)
						return "*";
					else
						return "8";
				case DIK_9:
					if(this->m_shiftOn)
						return "(";
					else
						return "9";
				case DIK_0:
					if(this->m_shiftOn)
						return ")";
					else
						return "0";
				case DIK_MINUS:
					if(this->m_shiftOn)
						return "_";
					else
						return "-";
				case DIK_EQUALS:
					if(this->m_shiftOn)
						return "+";
					else
						return "=";
				case DIK_LBRACKET:
					if(this->m_shiftOn)
						return "{";
					else
						return "[";
				case DIK_RBRACKET:
					if(this->m_shiftOn)
						return "}";
					else
						return "]";
				case DIK_BACKSLASH:
					if(this->m_shiftOn)
						return "|";
					else
						return "\\";
				case DIK_SEMICOLON:
					if(this->m_shiftOn)
						return ":";
					else
						return ";";
				case DIK_APOSTROPHE:
					if(this->m_shiftOn)
						return "\"";
					else
						return "'";
				case DIK_COMMA:
					if(this->m_shiftOn)
						return "<";
					else
						return ",";
				case DIK_PERIOD:
					if(this->m_shiftOn)
						return ">";
					else
						return ".";
				case DIK_SLASH:
					if(this->m_shiftOn)
						return "?";
					else
						return "/";
				case DIK_GRAVE:
					if(this->m_shiftOn)
						return "~";
					else
						return "`";
				case DIK_NUMPAD1:
					return "1";
				case DIK_NUMPAD2:
					return "2";
				case DIK_NUMPAD3:
					return "3";
				case DIK_NUMPAD4:
					return "4";
				case DIK_NUMPAD5:
					return "5";
				case DIK_NUMPAD6:
					return "6";
				case DIK_NUMPAD7:
					return "7";
				case DIK_NUMPAD8:
					return "8";
				case DIK_NUMPAD9:
					return "9";
				case DIK_NUMPAD0:
					return "0";
				case DIK_NUMPADPLUS:
					return "+";
				case DIK_NUMPADMINUS:
					return "-";
				case DIK_NUMPADSLASH:
					return "\\";
				case DIK_NUMPADSTAR:
					return "*";
				case DIK_SPACE:
					return " ";
			}
		}
	}
	// Fail condition
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInput
// Function:	GetCommandInputAsString(DWORD, int&)
// Description:	Return the currently pressed command key interpreted as a string.
//	Takes the current timestamp as a parameter.
///////////////////////////////////////////////////////////////////////////////

CString
CInput::GetCommandInputAsString(DWORD iTimestamp, int &iKeyCode)
{
	for(int iKey = 0; iKey < KEYBOARD_MAX_KEYS; iKey++)
	{
		if(this->m_keyboardKeys[iKey].JustPressed() 
		|| this->m_keyboardKeys[iKey].ShouldRepeat(iTimestamp))
		{
			iKeyCode = iKey;
			switch(iKey)
			{
				case DIK_RETURN:
				case DIK_NUMPADENTER:
					return "\n";
				case DIK_BACKSPACE:
					return "Backspace";
				case DIK_ESCAPE:
					return "Escape";
				case DIK_UP:
					return "[Up]";
				case DIK_DOWN:
					return "[Down]";
				case DIK_LEFT:
					return "[Left]";
				case DIK_RIGHT:
					return "[Right]";
			}
		}
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		CInput
// Function:	GetMouseInputAsString(int&)
// Description:	Return the currently pressed mouse button interpreted as a string.
///////////////////////////////////////////////////////////////////////////////

CString
CInput::GetMouseInputAsString(int &iKeyCode)
{
	CString strInput;
	if(m_mouseButtons[0].JustPressed())
	{
		iKeyCode = MOUSE_LBUTTON;
		strInput = "[Left Click]";
		return strInput;
	}
	if(m_mouseButtons[1].JustPressed())
	{
		iKeyCode = MOUSE_RBUTTON;
		strInput = "[Right Click]";
		return strInput;
	}
	if(m_mouseButtons[2].JustPressed())
	{
		iKeyCode = MOUSE_MBUTTON;
		strInput = "[Middle Click]";
		return strInput;
	}
	return NULL;
}

void
CInput::ResetInput(DWORD iTimestamp)
{
	for(int iKey = 0; iKey < KEYBOARD_MAX_KEYS; iKey++)
	{
		this->m_keyboardKeys[iKey].UpdateState(false, iTimestamp);
	}

	this->m_mouseButtons[0].UpdateState(false, iTimestamp);
	this->m_mouseButtons[1].UpdateState(false, iTimestamp);
	this->m_mouseButtons[2].UpdateState(false, iTimestamp);
}
