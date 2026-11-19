#pragma once

#include "dinput.h"
#include "Common.h"   // KEYBOARD_MAX_KEYS, MOUSE_* , REPEAT_* constants

typedef unsigned long DWORD;

enum ButtonStates
{
	BUTTONSTATE_PRESSED = 0,
	BUTTONSTATE_HELD,
	BUTTONSTATE_RELEASED,
	BUTTONSTATE_UP,
	BUTTONSTATE_MAX
};

enum InputTypes
{
	VT_ALPHANUMERIC = 0,
	VT_NUMERIC,
	VT_COMMAND,
	VT_MOUSE,
	VT_ALL
};

class ButtonState
{
public:
			ButtonState() { m_iKeyState = BUTTONSTATE_UP; }
	void	UpdateState(bool bDown, DWORD dwTime);
	bool	IsDown() { return (m_iKeyState < BUTTONSTATE_RELEASED); }
	bool	JustPressed() { return m_iKeyState == BUTTONSTATE_PRESSED; }
	bool	JustReleased() { return m_iKeyState == BUTTONSTATE_RELEASED; }
	bool	IsHeld() { return m_iKeyState == BUTTONSTATE_HELD; }
	bool	ShouldRepeat(DWORD dwTime);
	double	PressTime() { return m_dwPressTime; }
	

	//CString GetKeyString()
protected:
	int		m_iKeyState;
	DWORD	m_dwPressTime;
	DWORD	m_dwLastRepeatTime;
	DWORD	m_dwCurRepeatSpacing;
};

class CInput
{
public:
	//Constucts
	CInput(int *screenWidth, int *screenHeight);
	~CInput();

	//DirectInput Variables
	IDirectInput8			*m_directInput;
	LPDIRECTINPUTDEVICE8	 m_directKeyboard;
	LPDIRECTINPUTDEVICE8	 m_directMouse;

	//Useful Variables
	DIMOUSESTATE			 m_mousepointStart;
	DIMOUSESTATE			 m_mousepointTarget;
	float					 m_mouseHiResX;
	float					 m_mouseHiResY;
	float                    m_mouseWheelZ;
	float					 m_mouseAbsPosX;
	float					 m_mouseAbsPosY;
	float					 m_menuMouseChangeX;	//These must be created because the mouse technically 
	float					 m_menuMouseChangeY;	//leaves the edge of the screen, menus don't like that
	int						 *m_mouseMaxX;
	int						 *m_mouseMaxY;
	ButtonState				 m_keyboardKeys[KEYBOARD_MAX_KEYS];//BYTE	m_KB[256];
	ButtonState				 m_mouseButtons[MOUSE_MAX_BUTTONS];
	BOOL					 m_invertMouse;
	BOOL					 m_shiftOn;
	BOOL					 m_altOn;
	BOOL					 m_ctrlOn;
	BOOL					 m_capsOn;


	//Functions
	bool UpdateInput(DWORD curTime);
	//bool GetPressedKeyString(CString &keyString);

	CString GetInputAsString(DWORD iTimeStamp, int &iKeyCode);
	CString GetInputAsString(DWORD iTimeStamp, int &iKeyCode, InputTypes &eType);
	CString GetKeyInputAsString(DWORD iTimeStamp, int &iKeyCode);
	CString GetCommandInputAsString(DWORD iTimeStamp, int &iKeyCode);
	CString GetMouseInputAsString(int &iKeyCode);
	void ResetInput(DWORD iTimestamp);
};
