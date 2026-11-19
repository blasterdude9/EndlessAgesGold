#ifndef CONTROLCLASS_H
#define CONTROLCLASS_H

#include <afxtempl.h>
#include <dinput.h>
#include <d3d9.h>
#include <map>
#include "Common.h"
#include "CInput.h"
#include "CSkillClass.h"

///////////////////////////////////////////////////////////////////////////////
// Enum:		CONTROL_TYPES
// Description:	The different types of controllers the game can handle.
///////////////////////////////////////////////////////////////////////////////

enum CONTROL_TYPES
{
	CTL_KEYBOARD = 0,
	CTL_MOUSE,
	CTL_CONTROLLER,
	CTL_FORCECONTROLLER
};

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoObj
// Description:	This class holds a list of valid keys assigned to 
//				actions, controllers, keyboards, etc.
//				L_controlType: 0=keyboard/1=mouse/2=controller/3=force controller
//				L_action: 0=shoot1/1=shoot2/2=turn Right/3=turn left/4=pull up/5=dive/6=side step right/7=side step left/8=move foward/9=move backwards/10=lock target/11=roll left/12=roll right
///////////////////////////////////////////////////////////////////////////////

class ControlInfoObj : public CObject
{
	DECLARE_SERIAL( ControlInfoObj );

public:
	float List_miscFloat1;
	float List_miscFloat2;
	float List_miscFloat3;
	CString m_miscString;
	CString List_description;
	int List_controlType;
	int List_buttonDown;
    int List_action;
	bool m_shiftDown;
	bool m_altDown;
	bool m_ctrlDown;

	ControlInfoObj();
	ControlInfoObj(	int L_controlType,
					int L_buttonDown,
					int L_action,
					CString L_description,
					float L_miscFloat1,
					float L_miscFloat2,
					float L_miscFloat3,
					bool shiftDown = false,
					bool altDown = false,
					bool ctrlDown = false);
	ControlInfoObj(const ControlInfoObj &copy);

	ControlInfoObj& operator=(const ControlInfoObj &rhs);
	CString GetActionDescription(int iActionID = -1);
	CString GetButtonVis(int button, int type);
	void Serialize(CArchive& ar);
};

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Description:	A linked list of ControlInfoObj nodes.
///////////////////////////////////////////////////////////////////////////////

typedef std::pair<int, int> KEY_CONFIG_PAIR;
typedef std::pair<int, CString> CONTROL_INFO_PAIR;

const unsigned char NUM_ACTIONS = 32;

class ControlInfoList : public CObList
{
	DECLARE_SERIAL( ControlInfoList );

public:
	ControlInfoList();
	~ControlInfoList();

	void Clear();
	void AddControlEvent(CString keyString,
		CString actionString,
		int glid,
		int miscFloat2,
		int miscFloat3,
		int miscInt3,
		BOOL replace,
		BOOL modifyExisting);
	void ReplaceControlEvent(int targetIndex,
		CString keyString,
		CString actionString,
		int glid,
		int miscFloat2,
		int miscFloat3,
		int miscInt3);
	//TODO: Switch GetCurrentKeyString to use just a CInput pointer
	int GetMouseKey(CString key);
	int GetKeyNum(CString key);
	int GetJSNum(CString key);
	int GetActionNum(CString action);
	static CString ReturnActionByIndex(int loop);
	static CString ReturnKeyStringByIndex(int loop);
	void RemoveByIndex(int index);
	CString GetKeyAssignmentByActionString(CString strAction);
	ControlInfoObj* GetControlInfoObjectByKey(CString actionString);
	ControlInfoObj* GetByActionName(CString actionName);
	ControlInfoObj UnassignByKeynum(int iKeyNum);
	CString ReturnActionByIndexAdv(int index,
		int & miscInt, 
		int & miscInt2, 
		int & miscInt3, 
		int & miscInt4,
		CSkillObjectList * m_skillDB);
	CString ReturnActionByIndexAdv(CString desc,
		int index,
		int & miscInt, 
		int & miscInt2, 
		int & miscInt3, 
		int & miscInt4,
		CSkillObjectList * m_skillDB);
	void GetByDescription(CString desc, ControlInfoObj ** cPtr, int & listIndex);
};

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlDBObj
// Description:	DB.
///////////////////////////////////////////////////////////////////////////////

class ControlDBObj : public CObject
{
	DECLARE_SERIAL( ControlDBObj );

public:
	int List_controlBasis;
	float m_mouseSensitivity;
	BOOL m_invertMouse;
	float m_musicVolume;
	CString List_controlConfigName;
	ControlInfoList * List_controlInfoList;
	BOOL m_allowMultiBind;

	ControlDBObj();
	ControlDBObj(int L_controlBasis,//0=keyboard/1=mouse/2=controller/3=force controller
		         CString L_controlConfigName,
		         ControlInfoList * L_controlInfoList
			    );
	~ControlDBObj();

	//Switch GetControlInfo and GetKeyIfPressed to take in a CInput variable
	BOOL GetControlInfo(int &type,
						int &retButton,
						ButtonState mouseButtons[MOUSE_MAX_BUTTONS],
						ButtonState keyboardKeys[KEYBOARD_MAX_KEYS]);
    BOOL GetKeyIfPressed(CString * key, ButtonState keyboardKeys[KEYBOARD_MAX_KEYS]);	
	CString GetButtonVis(int button,int type);
	void Serialize( CArchive& ar );

private:

};

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Description:	A linked list of ControlDBObj nodes.
///////////////////////////////////////////////////////////////////////////////

class ControlDBList : public CObList
{
	DECLARE_SERIAL( ControlDBList );

public:
   ControlDBList();
   ~ControlDBList();

   ControlDBObj * GetByIndex(int index)
   {
       if(index < 0)
	      return NULL;
 
	   POSITION posLoc = FindIndex(index);
	   if(!posLoc)
		   return NULL;

	   return (ControlDBObj *)GetAt(posLoc);
   }

};

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Member:		DefaultKeyConfigs[]
// Description:	The default mapping of actions-to-keys.
//				To retrieve action string: ReturnActionByIndex(int)
//				To retrieve key string: ReturnKeyStringByIndex(int)
///////////////////////////////////////////////////////////////////////////////

const KEY_CONFIG_PAIR DefaultKeyConfigs[NUM_ACTIONS] =
{
	// Forward, W
	KEY_CONFIG_PAIR(8, DIK_W),
	// Backward, S
	KEY_CONFIG_PAIR(9, DIK_S),
	// Strafe Right, D
	KEY_CONFIG_PAIR(6, DIK_D),
	// Strafe Left, A
	KEY_CONFIG_PAIR(7, DIK_A),
	// Shoot, Left Click
	KEY_CONFIG_PAIR(0, MOUSE_LBUTTON),
	// Jump, Space Bar
	KEY_CONFIG_PAIR(15, DIK_SPACE),
	//first person camera
	KEY_CONFIG_PAIR(22, DIK_Z),
	//third person camera
	KEY_CONFIG_PAIR(23, DIK_C),
	// Jet Pack, Right Click
	KEY_CONFIG_PAIR(30, MOUSE_RBUTTON),
	// Examine, X
	KEY_CONFIG_PAIR(32, DIK_X),
	// Chat, Enter
	KEY_CONFIG_PAIR(26, DIK_RETURN),
	// Reload, R
	KEY_CONFIG_PAIR(46, DIK_R),
	// Taunt 1, Unassigned
//	KEY_CONFIG_PAIR(35, DIK_UNASSIGNED),
	// Taunt 2, Unassigned
//	KEY_CONFIG_PAIR(36, DIK_UNASSIGNED),
	// Taunt 3, Unassigned
//	KEY_CONFIG_PAIR(37, DIK_UNASSIGNED),
	// Dance 1, Unassigned
	KEY_CONFIG_PAIR(38, DIK_UNASSIGNED),
	// Dance 2, Unassigned
//	KEY_CONFIG_PAIR(39, DIK_UNASSIGNED),
	// Dance 3, Unassigned
//	KEY_CONFIG_PAIR(40, DIK_UNASSIGNED),
	// Wave, Unassigned
	KEY_CONFIG_PAIR(41, DIK_UNASSIGNED),
	// Salute, Unassigned
//	KEY_CONFIG_PAIR(42, DIK_UNASSIGNED),
	// Bow, Unassigned
//	KEY_CONFIG_PAIR(43, DIK_UNASSIGNED),
	// Laugh, Unassigned
	KEY_CONFIG_PAIR(44, DIK_UNASSIGNED),
	// Equip Item, Unassigned
//	KEY_CONFIG_PAIR(45, DIK_UNASSIGNED),
	// Hide Skill Use
	KEY_CONFIG_PAIR(47, DIK_H),
	// Reveal Skill Use
	KEY_CONFIG_PAIR(48, DIK_UNASSIGNED),
	// Sit, Unassigned
	KEY_CONFIG_PAIR(51, DIK_UNASSIGNED),
	// Open Inventory, B
	KEY_CONFIG_PAIR(53, DIK_B),
	// Toggle Resistances, Unassigned
//	KEY_CONFIG_PAIR(54, DIK_UNASSIGNED),
	// Toggle View Mode, Tab
//	KEY_CONFIG_PAIR(56, DIK_TAB),
	// Open Options, Unassigned
//	KEY_CONFIG_PAIR(57, DIK_UNASSIGNED),
	// Open Functions, Unassigned
//	KEY_CONFIG_PAIR(58, DIK_UNASSIGNED),
	// Open Character, Unassigned
//	KEY_CONFIG_PAIR(59, DIK_C),
	// Toggle Autorun, Unassigned
	KEY_CONFIG_PAIR(60, DIK_UNASSIGNED),
	// Use Quick Item 1, 1
	KEY_CONFIG_PAIR(61, DIK_1),
	// Use Quick Item 2, 2
	KEY_CONFIG_PAIR(62, DIK_2),
	// Use Quick Item 3, 3
	KEY_CONFIG_PAIR(63, DIK_3),
	// Use Quick Item 4, 4
	KEY_CONFIG_PAIR(64, DIK_4),
	// Use Quick Item 5, 5
	KEY_CONFIG_PAIR(65, DIK_5),
	// Use Quick Item 6, 6
	KEY_CONFIG_PAIR(66, DIK_6),
	// Use Quick Item 7, 7
	KEY_CONFIG_PAIR(67, DIK_7),
	// Use Quick Item 8, 8
	KEY_CONFIG_PAIR(68, DIK_8),
	// Use Quick Item 9, 9
	KEY_CONFIG_PAIR(69, DIK_9),
	// Use Quick Item 10, 0
	KEY_CONFIG_PAIR(70, DIK_0),
	// Use Quick Item 11, 0
//	KEY_CONFIG_PAIR(73, DIK_MINUS),
	// Use Quick Item 12, 0
//	KEY_CONFIG_PAIR(74, DIK_EQUALS),
	// Arm all Quick Items
	KEY_CONFIG_PAIR(71, DIK_GRAVE),
	// Open Quest Journal
	KEY_CONFIG_PAIR(72, DIK_UNASSIGNED)
};

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Member:		ActionPairs[]
// Description:	The pairs of action strings and their associated ID.
///////////////////////////////////////////////////////////////////////////////

const CONTROL_INFO_PAIR ActionPairs[NUM_ACTIONS] =
{
	CONTROL_INFO_PAIR(8, "Forward"),
	CONTROL_INFO_PAIR(9, "Backward"),
	CONTROL_INFO_PAIR(6, "Strafe Right"),
	CONTROL_INFO_PAIR(7, "Strafe Left"),
	CONTROL_INFO_PAIR(0, "Shoot"),
	CONTROL_INFO_PAIR(15, "Jump"),
	CONTROL_INFO_PAIR(22, "First Person Cam"),
	CONTROL_INFO_PAIR(23, "Third Person Cam"),
	CONTROL_INFO_PAIR(30, "Jet Pack"),
	CONTROL_INFO_PAIR(32, "Interaction"),
	CONTROL_INFO_PAIR(26, "Chat"),
	CONTROL_INFO_PAIR(46, "Reload"),
//	CONTROL_INFO_PAIR(35, "Taunt 1"),
//	CONTROL_INFO_PAIR(36, "Taunt 2"),
//	CONTROL_INFO_PAIR(37, "Taunt 3"),
	CONTROL_INFO_PAIR(38, "Dance"),
//	CONTROL_INFO_PAIR(39, "Dance 2"),
//	CONTROL_INFO_PAIR(40, "Dance 3"),
	CONTROL_INFO_PAIR(41, "Wave"),
//	CONTROL_INFO_PAIR(42, "Salute"),
//	CONTROL_INFO_PAIR(43, "Bow"),
	CONTROL_INFO_PAIR(44, "Laugh"),
//	CONTROL_INFO_PAIR(45, "Equip Item"),
	CONTROL_INFO_PAIR(47, "Hide"),
	CONTROL_INFO_PAIR(48, "Reveal"),
	CONTROL_INFO_PAIR(51, "Sit"),
	CONTROL_INFO_PAIR(53, "Open Inventory"),
//	CONTROL_INFO_PAIR(54, "Toggle Resistances"),
//	CONTROL_INFO_PAIR(56, "Toggle View Mode"),
//	CONTROL_INFO_PAIR(57, "Open Options"),
//	CONTROL_INFO_PAIR(58, "Open Functions"),
//	CONTROL_INFO_PAIR(59, "Open Character"),

	CONTROL_INFO_PAIR(60, "Toggle Autorun"),
	CONTROL_INFO_PAIR(61, "Use Quick Item 1"),
	CONTROL_INFO_PAIR(62, "Use Quick Item 2"),
	CONTROL_INFO_PAIR(63, "Use Quick Item 3"),
	CONTROL_INFO_PAIR(64, "Use Quick Item 4"),
	CONTROL_INFO_PAIR(65, "Use Quick Item 5"),
	CONTROL_INFO_PAIR(66, "Use Quick Item 6"),
	CONTROL_INFO_PAIR(67, "Use Quick Item 7"),
	CONTROL_INFO_PAIR(68, "Use Quick Item 8"),
	CONTROL_INFO_PAIR(69, "Use Quick Item 9"),
	CONTROL_INFO_PAIR(70, "Use Quick Item 10"),
//	CONTROL_INFO_PAIR(73, "Use Quick Item 11"),
//	CONTROL_INFO_PAIR(74, "Use Quick Item 12"),
	CONTROL_INFO_PAIR(71, "Arm All Quick Items"),
	CONTROL_INFO_PAIR(72, "Open Quest Log")
};

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Member:		KeyPairs[]
// Description:	The pairs of key strings and their associated ID.
///////////////////////////////////////////////////////////////////////////////

const CONTROL_INFO_PAIR KeyPairs[KEYBOARD_MAX_KEYS] =
{
	CONTROL_INFO_PAIR(DIK_NUMPAD1, "Num 1"),
	CONTROL_INFO_PAIR(DIK_NUMPAD2, "Num 2"),
	CONTROL_INFO_PAIR(DIK_NUMPAD3, "Num 3"),
	CONTROL_INFO_PAIR(DIK_NUMPAD4, "Num 4"),
	CONTROL_INFO_PAIR(DIK_NUMPAD5, "Num 5"),
	CONTROL_INFO_PAIR(DIK_NUMPAD6, "Num 6"),
	CONTROL_INFO_PAIR(DIK_NUMPAD7, "Num 7"),
	CONTROL_INFO_PAIR(DIK_NUMPAD8, "Num 8"),
	CONTROL_INFO_PAIR(DIK_NUMPAD9, "Num 9"),
	CONTROL_INFO_PAIR(DIK_NUMPAD0, "Num 0"),
	CONTROL_INFO_PAIR(DIK_NUMLOCK, "Num Lock"),
	CONTROL_INFO_PAIR(DIK_NUMPADSLASH, "Num Slash"),
	CONTROL_INFO_PAIR(DIK_NUMPADSTAR, "Num Star"),
	CONTROL_INFO_PAIR(DIK_NUMPADENTER, "Num Enter"),
	CONTROL_INFO_PAIR(DIK_NUMPADPERIOD, "Num Period"),
	CONTROL_INFO_PAIR(DIK_F1, "F1"),
	CONTROL_INFO_PAIR(DIK_F2, "F2"),
	CONTROL_INFO_PAIR(DIK_F3, "F3"),
	CONTROL_INFO_PAIR(DIK_F4, "F4"),
	CONTROL_INFO_PAIR(DIK_F5, "F5"),
	CONTROL_INFO_PAIR(DIK_F6, "F6"),
	CONTROL_INFO_PAIR(DIK_F7, "F7"),
	CONTROL_INFO_PAIR(DIK_F8, "F8"),
	CONTROL_INFO_PAIR(DIK_F9, "F9"),
	CONTROL_INFO_PAIR(DIK_F10, "F10"),
	CONTROL_INFO_PAIR(DIK_1, "1"),
	CONTROL_INFO_PAIR(DIK_2, "2"),
	CONTROL_INFO_PAIR(DIK_3, "3"),
	CONTROL_INFO_PAIR(DIK_4, "4"),
	CONTROL_INFO_PAIR(DIK_5, "5"),
	CONTROL_INFO_PAIR(DIK_6, "6"),
	CONTROL_INFO_PAIR(DIK_7, "7"),
	CONTROL_INFO_PAIR(DIK_8, "8"),
	CONTROL_INFO_PAIR(DIK_9, "9"),
	CONTROL_INFO_PAIR(DIK_0, "0"),
	CONTROL_INFO_PAIR(DIK_A, "A"),
	CONTROL_INFO_PAIR(DIK_B, "B"),
	CONTROL_INFO_PAIR(DIK_C, "C"),
	CONTROL_INFO_PAIR(DIK_D, "D"),
	CONTROL_INFO_PAIR(DIK_E, "E"),
	CONTROL_INFO_PAIR(DIK_F, "F"),
	CONTROL_INFO_PAIR(DIK_G, "G"),
	CONTROL_INFO_PAIR(DIK_H, "H"),
	CONTROL_INFO_PAIR(DIK_I, "I"),
	CONTROL_INFO_PAIR(DIK_J, "J"),
	CONTROL_INFO_PAIR(DIK_K, "K"),
	CONTROL_INFO_PAIR(DIK_L, "L"),
	CONTROL_INFO_PAIR(DIK_M, "M"),
	CONTROL_INFO_PAIR(DIK_N, "N"),
	CONTROL_INFO_PAIR(DIK_O, "O"),
	CONTROL_INFO_PAIR(DIK_P, "P"),
	CONTROL_INFO_PAIR(DIK_Q, "Q"),
	CONTROL_INFO_PAIR(DIK_R, "R"),
	CONTROL_INFO_PAIR(DIK_S, "S"),
	CONTROL_INFO_PAIR(DIK_T, "T"),
	CONTROL_INFO_PAIR(DIK_U, "U"),
	CONTROL_INFO_PAIR(DIK_V, "V"),
	CONTROL_INFO_PAIR(DIK_W, "W"),
	CONTROL_INFO_PAIR(DIK_X, "X"),
	CONTROL_INFO_PAIR(DIK_Y, "Y"),
	CONTROL_INFO_PAIR(DIK_Z, "Z"),
	CONTROL_INFO_PAIR(DIK_SPACE, "[Spacebar]"),
	CONTROL_INFO_PAIR(DIK_TAB, "[Tab]"),
	CONTROL_INFO_PAIR(DIK_ADD, "+"),
	CONTROL_INFO_PAIR(DIK_SUBTRACT, "-"),
	CONTROL_INFO_PAIR(DIK_ESCAPE, "[Escape]"),
	CONTROL_INFO_PAIR(DIK_BACK, "[Backspace]"),
	CONTROL_INFO_PAIR(DIK_LSHIFT, "[Left Shift]"),
	CONTROL_INFO_PAIR(DIK_RSHIFT, "[Right Shift]"),
	CONTROL_INFO_PAIR(DIK_RCONTROL, "[Right Ctrl]"),
	CONTROL_INFO_PAIR(DIK_LCONTROL, "[Left Ctrl]"),
	CONTROL_INFO_PAIR(DIK_UP, "[Up]"),
	CONTROL_INFO_PAIR(DIK_DOWN, "[Down]"),
	CONTROL_INFO_PAIR(DIK_RIGHT, "[Right]"),
	CONTROL_INFO_PAIR(DIK_LEFT, "[Left]"),
	CONTROL_INFO_PAIR(DIK_MINUS, "-"),
	CONTROL_INFO_PAIR(DIK_EQUALS, "+"),
	CONTROL_INFO_PAIR(DIK_LBRACKET, "["),
	CONTROL_INFO_PAIR(DIK_RBRACKET, "]"),
	CONTROL_INFO_PAIR(DIK_RETURN, "[Return]"),
	CONTROL_INFO_PAIR(DIK_SEMICOLON, ";"),
	CONTROL_INFO_PAIR(DIK_APOSTROPHE, "'"),
	CONTROL_INFO_PAIR(DIK_GRAVE, "~"),
	CONTROL_INFO_PAIR(DIK_BACKSLASH, "/"),
	CONTROL_INFO_PAIR(DIK_COMMA, ","),
	CONTROL_INFO_PAIR(DIK_PERIOD, "."),
	CONTROL_INFO_PAIR(DIK_SLASH, "\\"),
	CONTROL_INFO_PAIR(DIK_MULTIPLY, "*"),
	CONTROL_INFO_PAIR(DIK_LMENU, "[Left Alt]"),
	CONTROL_INFO_PAIR(DIK_SCROLL, "[Scroll Lock]"),
	CONTROL_INFO_PAIR(DIK_DECIMAL, "."),
	CONTROL_INFO_PAIR(DIK_RMENU, "[Right Alt]"),
	CONTROL_INFO_PAIR(DIK_HOME, "[Home]"),
	CONTROL_INFO_PAIR(DIK_END, "[End]"),
	CONTROL_INFO_PAIR(DIK_NEXT, "[Page Down]"),
	CONTROL_INFO_PAIR(DIK_INSERT, "[Insert]"),
	CONTROL_INFO_PAIR(DIK_DELETE, "[Delete]"),
	CONTROL_INFO_PAIR(DIK_APPS, "[Apps]"),
	CONTROL_INFO_PAIR(DIK_UNASSIGNED, "Unassigned")
};

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Member:		MousePairs[]
// Description:	The pairs of mouse button strings and their associated ID.
///////////////////////////////////////////////////////////////////////////////

const CONTROL_INFO_PAIR MousePairs[MOUSE_MAX_BUTTONS] = 
{
	CONTROL_INFO_PAIR(MOUSE_LBUTTON, "[Left Click]"),
	CONTROL_INFO_PAIR(MOUSE_RBUTTON, "[Right Click]"),
	CONTROL_INFO_PAIR(MOUSE_MBUTTON, "[Middle Click]")
};

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Member:		ActionMap/KeyMap/MouseMap
// Description:	The hashmap containers for action, key and mouse button pairs.
///////////////////////////////////////////////////////////////////////////////

const std::map<int, CString> 
ActionMap(ActionPairs, ActionPairs + sizeof(ActionPairs) / sizeof(ActionPairs[0]));
const std::map<int, CString> 
KeyMap(KeyPairs, KeyPairs + sizeof(KeyPairs) / sizeof(KeyPairs[0]));
const std::map<int, CString> 
MouseMap(MousePairs, MousePairs + sizeof(MousePairs) / sizeof(MousePairs[0]));

#endif
