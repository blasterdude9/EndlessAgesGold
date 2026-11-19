#pragma once

#include "CMenuVisual.h"
#include "CMenuImage.h"
#include "CMenuText.h"
#include "CMenuScrollBar.h"

enum TextColorTypes
{
	MENU_TEXTCOLOR_NORMAL = 0,
	MENU_TEXTCOLOR_CLAN,
	MENU_TEXTCOLOR_GROUP,
	MENU_TEXTCOLOR_SERVER,
	MENU_TEXTCOLOR_DAMAGE,
	MENU_TEXTCOLOR_HEAL,
	MENU_TEXTCOLOR_MUSIC,
	MENU_TEXTCOLOR_MAX
};

enum VertTextAlignment
{
	MENU_TEXTALIGN_TOP = 0,
	MENU_TEXTALIGN_MIDDLE,
	MENU_TEXTALIGN_BOTTOM
};

class CMenuTextBox : public CMenuVisual
{
public:
    CMenuTextBox();
	~CMenuTextBox();

	//Visible Objects
	CMenuImage		*m_backgroundLeft;
	CMenuImage		*m_backgroundMiddle;
	CMenuImage		*m_backgroundRight;
	CMenuText		*m_linesOfText;
	CMenuImage		 m_cursor;
	CMenuScrollBar	*m_scrollBar;

	//Settable Parameters
	CMenuFont	*m_font;
	int			 m_maxLinesOfText;
	int			 m_horAlignment;
	int			 m_vertAlignment;
	bool		 m_multilined;
	bool		 m_editable;
	bool		 m_password;   // show * instead of the typed characters (login password box)
	float		 m_textInset;

	//Engine Variables
	CString m_fullText;
	bool	m_mousedOver;
	int		m_numVisibleLines;
	int		m_curLine;
	int		m_curCharacter;
	int		m_topLine;
	int		m_nextWriteLine;
	bool	m_hasLooped;
	bool	m_showCursor;
	DWORD	m_lastCursorBlink;

	//Temperary Variables
	float	*m_red;
	float	*m_green;
	float	*m_blue;
	float	*m_alpha;
	int		 m_numColors;
	CString	 m_fontName;

	// Validation Variables
	bool	m_bIsNumericOnly;
	int		m_iMaxLength;

	//Scripts
	CMenuScript *m_enterPressScript;
	CMenuScript *m_mouseOverScript;
	CMenuScript *m_mouseOutScript;

	//Functions
	int  GetType() { return MENU_VISTYPE_TEXTBOX; }
	bool HandleInput(CInput *pInput, float parentX, float parentY, DWORD curTime, LPDIRECT3DDEVICE9 pD3DDevice);
	void AddLine(CString newLine, int colorType = MENU_TEXTCOLOR_NORMAL);
	void AddLine(CString newLine, DWORD appendedInt);
	void AddLine(CString newLine, D3DVECTOR appendedVector);
	void SetText(CString newText, int line = 0);
	void WrapLine(int lineNumber = 0, int colorType = MENU_TEXTCOLOR_NORMAL);
	void Render(LPDIRECT3DDEVICE9 pD3DDevice,
				CStateManagementObj *stateManager, 
				CTextureDBObjList *textureDatabase,
				float parentX,
				float parentY);
	void Resize();
	bool HandleTyping(CInput *pInput, DWORD curTime);
	void Clone(CMenuVisual **pClone, LPDIRECT3DDEVICE9 pD3DDevice);
	void Copy(CMenuVisual *pCopy, LPDIRECT3DDEVICE9 pD3DDevice);
	void Serialize(CXMLFileIO &xml);
};