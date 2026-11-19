#include "stdafx.h"
#include "ControlsClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IMPLEMENT_SERIAL(ControlInfoObj, CObject, VERSIONABLE_SCHEMA|2)
IMPLEMENT_SERIAL(ControlInfoList, CObList, 0)
IMPLEMENT_SERIAL(ControlDBObj, CObject, VERSIONABLE_SCHEMA|2)
IMPLEMENT_SERIAL(ControlDBList, CObList, 0)

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoObj
// Function:	Ctor/Dtor
// Description:	Control event class.
//				L_controlType:	0=keyboard | 1=mouse | 2=controller | 3=force controller
//				L_action:		0=shoot1 | 1=shoot2 | 2=turn Right | 3=turn left | 4=pull up |
//								5=dive | 6=side step right | 7=side step left | 8=move foward |
//								9=move backwards | 10 lock target
///////////////////////////////////////////////////////////////////////////////

ControlInfoObj::ControlInfoObj() {}

ControlInfoObj::ControlInfoObj(int L_controlType,
							   int L_buttonDown,
							   int L_action,
							   CString L_description,
							   float L_miscFloat1,
							   float L_miscFloat2,
							   float L_miscFloat3,
							   bool shiftDown,
							   bool altDown,
							   bool ctrlDown)
{
	List_miscFloat1	 = L_miscFloat1;
	List_miscFloat2	 = L_miscFloat2;
	List_miscFloat3	 = L_miscFloat3;
	List_description = L_description;
	List_controlType = L_controlType;
	List_buttonDown	 = L_buttonDown;
	List_action		 = L_action;
	m_miscString	 = ":LOOT";
	m_shiftDown		 = shiftDown;
	m_altDown		 = altDown;
	m_ctrlDown		 = ctrlDown;
}

ControlInfoObj::ControlInfoObj(const ControlInfoObj &copy)
{
	this->List_miscFloat1 = copy.List_miscFloat1;
	this->List_miscFloat2 = copy.List_miscFloat2;
	this->List_miscFloat3 = copy.List_miscFloat3;
	this->List_description = copy.List_description;
	this->List_controlType = copy.List_controlType;
	this->List_buttonDown = copy.List_buttonDown;
	this->List_action = copy.List_action;
	this->m_miscString = copy.m_miscString;
	this->m_shiftDown = copy.m_shiftDown;
	this->m_altDown = copy.m_altDown;
	this->m_ctrlDown = copy.m_ctrlDown;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoObj
// Function:	operator=(const ControlInfoObj&)
// Description:	Overloaded assignment operator.
///////////////////////////////////////////////////////////////////////////////

ControlInfoObj& 
ControlInfoObj::operator=(const ControlInfoObj &rhs)
{
	this->List_miscFloat1 = rhs.List_miscFloat1;
	this->List_miscFloat2 = rhs.List_miscFloat2;
	this->List_miscFloat3 = rhs.List_miscFloat3;
	this->List_description = rhs.List_description;
	this->List_controlType = rhs.List_controlType;
	this->List_buttonDown = rhs.List_buttonDown;
	this->List_action = rhs.List_action;
	this->m_miscString = rhs.m_miscString;
	this->m_shiftDown = rhs.m_shiftDown;
	this->m_altDown = rhs.m_altDown;
	this->m_ctrlDown = rhs.m_ctrlDown;
	return *this;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoObj
// Function:	GetActionDescription(int)
// Description:	Return the action string given the action ID.
///////////////////////////////////////////////////////////////////////////////

CString
ControlInfoObj::GetActionDescription(int iActionID)
{
	if(iActionID == -1) iActionID = this->List_action;
	std::map<int, CString>::const_iterator iter = ActionMap.find(iActionID);
	return iter->second;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoObj
// Function:	GetButtonVis(int, int)
// Description:	Returns a string description of a button/key provided its ID.
///////////////////////////////////////////////////////////////////////////////

CString
ControlInfoObj::GetButtonVis(int button, int type)
{
	CString strResult = "Unassigned";
	std::map<int, CString>::const_iterator iter;

	switch(type)
	{
		case CTL_KEYBOARD:
		{
			iter = KeyMap.find(button);
			if(iter != KeyMap.end())
				strResult.SetString(iter->second);
			break;
		}
		case CTL_MOUSE:
		{
			iter = MouseMap.find(button);
			if(iter != MouseMap.end())
				strResult.SetString(iter->second);
			break;
		}
		default: break;
	}
	return strResult;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoObj
// Function:	Serialize(CArchive&)
// Description:	Serialize this class to storage.
///////////////////////////////////////////////////////////////////////////////

void
ControlInfoObj::Serialize(CArchive& ar)
{
	BOOL reserved = 0;
	CObject::Serialize( ar );
	if(ar.IsStoring())
	{
		ar.SetObjectSchema(2);
		ASSERT(ar.GetObjectSchema() == 2);

		ar	<< List_controlType
			<< List_buttonDown
			<< List_action
			<< List_description
			<< List_miscFloat1
			<< List_miscFloat2
			<< List_miscFloat3
			<< m_shiftDown
			<< m_altDown
			<< m_ctrlDown;
	}
	else
	{
		int version = ar.GetObjectSchema();

        ar	>> List_controlType
			>> List_buttonDown
			>> List_action
			>> List_description
			>> List_miscFloat1
			>> List_miscFloat2
			>> List_miscFloat3;

		if(version > 1)
		{
			ar	>> m_shiftDown
				>> m_altDown
				>> m_ctrlDown;
		}
		else
		{
			m_shiftDown = false;
			m_altDown	= false;
			m_ctrlDown	= false;
		}

		// Upgrade obsolete descriptions
		// NOTE: These names can never be used again
		if(List_description.CompareNoCase("SideStep right")==0)
		   List_description = "Strafe Right";

		if(List_description.CompareNoCase("SideStep left")==0)
		   List_description = "Strafe Left";

		if(List_description.CompareNoCase("shoot1")==0)
		   List_description = "Shoot";

		if(List_description.CompareNoCase("Use Force Jet")==0)
		   List_description = "Use Jet Pack";

		if(List_description.CompareNoCase("First Person View")==0)
		   List_description = "1st Person";

		if(List_description.CompareNoCase("First Person View")==0)
		   List_description = "1st Person";

		if(List_description.CompareNoCase("3rd Person View")==0)
		   List_description = "3rd Person";

		m_miscString = ":LOOT";
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Function:	Ctor/Dtor
///////////////////////////////////////////////////////////////////////////////

ControlInfoList::ControlInfoList() {}

ControlInfoList::~ControlInfoList()
{
	POSITION pos = NULL, prev = NULL;
	for(pos = GetHeadPosition(); pos != NULL;)
	{
		prev = pos;
	    ControlInfoObj *mPtr = (ControlInfoObj*)GetNext(pos);
		RemoveAt(prev);
		SafeDelete(mPtr);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Function:	Clear()
// Description:	Clear this linked list of all nodes.
///////////////////////////////////////////////////////////////////////////////

void
ControlInfoList::Clear()
{
	POSITION pos = NULL, prev = NULL;
	for(pos = GetHeadPosition(); pos != NULL;)
	{
		prev = pos;
	    ControlInfoObj *mPtr = (ControlInfoObj*)GetNext(pos);
		RemoveAt(prev);
		SafeDelete(mPtr);
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Function:	GetKeyNum(CString)
// Description:	Return the key ID based on its text description.
///////////////////////////////////////////////////////////////////////////////

int
ControlInfoList::GetKeyNum(CString strDescription)
{
	CString strTemp;
	std::map<int, CString>::const_iterator iter;
	for(iter = KeyMap.begin(); iter != KeyMap.end(); iter++)
	{
		strTemp = iter->second;
		if(strTemp.MakeLower() == strDescription.MakeLower()) 
			return iter->first;
	}
	return -1;
}  

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Function:	GetMouseKey(CString)
// Description:	Return the mouse button ID based on its text description.
///////////////////////////////////////////////////////////////////////////////

int
ControlInfoList::GetMouseKey(CString strDescription)
{
	CString strTemp;
	std::map<int, CString>::const_iterator iter;
	for(iter = MouseMap.begin(); iter != MouseMap.end(); iter++)
	{
		strTemp = iter->second;
		if(strTemp.MakeLower() == strDescription.MakeLower()) 
			return iter->first;
	}
	return -1;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Function:	GetJSNum(CString)
// Description:	-
///////////////////////////////////////////////////////////////////////////////

int
ControlInfoList::GetJSNum(CString key)
{
	if(key == "JS_BUTTON1")
		return 0;
	if(key == "JS_BUTTON2")
		return 1;
	if(key == "JS_BUTTON3")
		return 2;
	if(key == "JS_BUTTON4")
		return 3;
	if(key == "JS_BUTTON5")
		return 4;
	if(key == "JS_BUTTON6")
		return 5;
	if(key == "JS_BUTTON7")
		return 6;
	if(key == "JS_BUTTON8")
		return 7;
	if(key == "JS_BUTTON9")
		return 8;

	return -1;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Function:	GetActionNum(CString)
// Description:	Return the action ID based on its text description.
///////////////////////////////////////////////////////////////////////////////

int
ControlInfoList::GetActionNum(CString strDescription)
{
	std::map<int, CString>::const_iterator iter;
	for(iter = ActionMap.begin(); iter != ActionMap.end(); iter++)
	{
		if(iter->second == strDescription) 
			return iter->first;
	}
	return -1;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Function:	ReturnActionByIndexAdv(int, int&, int&, int&, int&, CSkillObjectList*)
// Description:	-
///////////////////////////////////////////////////////////////////////////////

CString
ControlInfoList::ReturnActionByIndexAdv(int iIndex,
										int &iMiscInt,
										int &iMiscInt2,
										int &iMiscInt3,
										int &iMiscInt4,
										CSkillObjectList *pSkillDB)
{
	CString strAction = "";
	iMiscInt = 0;
	iMiscInt2 = 0;
	iMiscInt3 = 0;
	iMiscInt4 = 0;
	strAction = ReturnActionByIndex(iIndex);
	if(strAction != "")
		return strAction;

	// Find in list skill by type (magic)
	int iIndexCount = 68;
	int skillTypeLoc = 12;
	CSkillObject * skillDBPtrRef = pSkillDB->GetSkillByType(skillTypeLoc);
	if(skillDBPtrRef)
	{
		int levelLoc = 1;
		if(skillDBPtrRef->m_levelRangeDB)
		{
			for(POSITION lrangePos = skillDBPtrRef->m_levelRangeDB->GetHeadPosition();lrangePos != NULL;)
			{
				CLevelRangeObject * levelRange = (CLevelRangeObject *)skillDBPtrRef->m_levelRangeDB->GetNext(lrangePos);
				int abilityLoc = 0;
				if(levelRange->m_abilityDB)
				for(POSITION lvlPos = levelRange->m_abilityDB->GetHeadPosition();lvlPos != NULL;)
				{
					CSkillAbilityObject * lvlPtr = (CSkillAbilityObject *)levelRange->m_abilityDB->GetNext(lvlPos);
					if(iIndex == iIndexCount)
					{
						iMiscInt = abilityLoc; 
						iMiscInt2 = levelLoc;
						iMiscInt3 = skillTypeLoc; 
						iMiscInt4 = 50;
						return lvlPtr->m_abilityName;
					}
					abilityLoc++;
					iIndexCount++;
				}
				levelLoc++;
			}		
		}
	}
	return strAction;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Function:	ReturnActionByIndexAdv(CString, int, int&, int&, int&, int&, CSkillObjectList*)
// Description:	-
///////////////////////////////////////////////////////////////////////////////

CString
ControlInfoList::ReturnActionByIndexAdv(CString desc,
										int index,
										int & miscInt, 
										int & miscInt2, 
										int & miscInt3, 
										int & miscInt4,
										CSkillObjectList * m_skillDB)
{
	CString action = "";
	miscInt = 0;
	miscInt2 = 0;
	miscInt3 = 0;
	miscInt4 = 0;
	ControlInfoObj * controltemp = NULL;
	int indexTemp = 0;
	GetByDescription(desc, &controltemp, indexTemp);
	if(controltemp)
	{
		action = controltemp->List_description;
		miscInt = (int)controltemp->List_miscFloat1;
		miscInt = (int)controltemp->List_miscFloat2;
		miscInt = (int)controltemp->List_miscFloat3;
		return action;
	}

	//in list skill by type (magic)
	int indexCount = 67;
	int skillTypeLoc = 12;
	CSkillObject * skillDBPtrRef = m_skillDB->GetSkillByType(skillTypeLoc);
	if(skillDBPtrRef)
	{
		int levelLoc = 1;
		if(skillDBPtrRef->m_levelRangeDB)
		{
			for(POSITION lrangePos = skillDBPtrRef->m_levelRangeDB->GetHeadPosition();lrangePos != NULL;)
			{
				CLevelRangeObject * levelRange = (CLevelRangeObject *)skillDBPtrRef->m_levelRangeDB->GetNext(lrangePos);
				int abilityLoc = 0;
				if(levelRange->m_abilityDB)
				{
					for(POSITION lvlPos = levelRange->m_abilityDB->GetHeadPosition();lvlPos != NULL;)
					{
						CSkillAbilityObject * lvlPtr = (CSkillAbilityObject *)levelRange->m_abilityDB->GetNext(lvlPos);
						//visual->m_sliderSystem->AddString(lvlPtr->m_abilityName,0,abilityLoc,levelLoc,skillTypeLoc,50);
						if(index == indexCount)
						{
							miscInt = abilityLoc; 
							miscInt2 = levelLoc;
							miscInt3 = skillTypeLoc; 
							miscInt4 = 50;
							return lvlPtr->m_abilityName;
						}
						abilityLoc++;
						indexCount++;
					}
				}
				levelLoc++;
			}
		}			   
	}
	return action;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Function:	ReturnActionByIndex(int)
// Description:	Return a text description of an action given its ID.
//				OLD NOTE: MAKE SURE YOU CHANGE INDEXCOUNT IN ControlInfoList::ReturnActionByIndexAdv
//				TO BE ONE MORE THAN THE HIGHEST CASE.
///////////////////////////////////////////////////////////////////////////////

CString 
ControlInfoList::ReturnActionByIndex(int iIndex)
{
	std::map<int, CString>::const_iterator iter;
	iter = ActionMap.find(iIndex);
	if(iter != ActionMap.end())
		return iter->second;
	return "";
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Function:	ReturnKeyStringByIndex(int)
// Description:	Return a text description of a key given its ID.
///////////////////////////////////////////////////////////////////////////////

CString
ControlInfoList::ReturnKeyStringByIndex(int iIndex)
{
	std::map<int, CString>::const_iterator iter;
	iter = KeyMap.find(iIndex);

	if(iter != KeyMap.end())
		return iter->second;

	iter = MouseMap.find(iIndex);
	if(iter != MouseMap.end())
		return iter->second;

	return "Unassigned";
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Function:	GetKeyAssignmentByActionString(CString)
// Description:	Determine what key is assigned to the given action string.
//				Return the text description of the key.
///////////////////////////////////////////////////////////////////////////////

CString
ControlInfoList::GetKeyAssignmentByActionString(CString strAction)
{
   for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
   {
       ControlInfoObj *pControlInfo = (ControlInfoObj*) GetNext(posLoc);
       if(pControlInfo->List_description.CompareNoCase(strAction)==0)
          return pControlInfo->GetButtonVis(pControlInfo->List_buttonDown, pControlInfo->List_controlType);
   }
   return "Unassigned";
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Function:	GetControlInfoObjectByKey(CString)
// Description:	Determine what action is assigned the given key string.
//				Return the ControlInfoObj.
///////////////////////////////////////////////////////////////////////////////

ControlInfoObj*
ControlInfoList::GetControlInfoObjectByKey(CString strKey)
{
	int iKey = -1;
	std::map<int, CString>::const_iterator iter;

	// Determine the button/key ID
	iKey = GetKeyNum(strKey);
	if(iKey == -1) 
	{
		iKey = GetMouseKey(strKey);
		if(iKey != -1)
			iKey -= KEYBOARD_MAX_KEYS;
		else
			return NULL;
	}

	// Find the ControlInfoObj using the button/key ID
	POSITION pCurrent = NULL, pPrevious = NULL;
	for(pCurrent = GetHeadPosition(); (pPrevious = pCurrent) != NULL;)
	{
		ControlInfoObj *pControlInfo = (ControlInfoObj*)GetNext(pCurrent);
		if(pControlInfo
		&& (pControlInfo->List_buttonDown == iKey))
			return pControlInfo;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Function:	RemoveByIndex(int)
// Description:	Remove a ControlInfoObj from the linked list given its index.
///////////////////////////////////////////////////////////////////////////////

void
ControlInfoList::RemoveByIndex(int index)
{
   if(index < 0) return;

   POSITION pos = FindIndex(index);
   if(!pos) return;
   ControlInfoObj *pControlInfo = (ControlInfoObj*)GetAt(pos);
   SafeDelete(pControlInfo);
   RemoveAt(pos);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Function:	GetByActionName(CString)
// Description:	Retrieve the correct ControlInfoObj given the action description.
///////////////////////////////////////////////////////////////////////////////

ControlInfoObj*
ControlInfoList::GetByActionName(CString strAction)
{
	if(!this) return NULL;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
	   ControlInfoObj *cPtr = (ControlInfoObj*) GetNext(posLoc);
	   if(cPtr->List_description == strAction) 
		  return cPtr;
	}
	return NULL;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Function:	UnassignByKeynum(int)
// Description:	Remove an action->key binding given the key ID.
//				Return the old ControlInfoObj.
///////////////////////////////////////////////////////////////////////////////

ControlInfoObj
ControlInfoList::UnassignByKeynum(int iKeyID)
{
	int iControlType = 0;
	ControlInfoObj pOldControlInfo;
	if(iKeyID == -1) return pOldControlInfo;

	POSITION pCurrent = NULL, pLast = NULL;

	if(iKeyID >= KEYBOARD_MAX_KEYS)
	{
		iKeyID -= KEYBOARD_MAX_KEYS;
		iControlType = 1;
	}

	for(POSITION pCurrent = GetHeadPosition(); (pLast = pCurrent) != NULL;)
	{
		ControlInfoObj *pControlInfo = (ControlInfoObj*)GetNext(pCurrent);
		if((pControlInfo->List_buttonDown == iKeyID) 
		&& (pControlInfo->List_controlType == iControlType))
		{
			pOldControlInfo = *pControlInfo;
			SafeDelete(pControlInfo);
			RemoveAt(pLast);
			return pOldControlInfo;
		}
	}
	return pOldControlInfo;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Function:	AddControlEvent(CString, CString, int, int, int, int, BOOL, BOOL)
// Description:	Create/replace a ControlInfoObj for an action->key binding.
///////////////////////////////////////////////////////////////////////////////

void
ControlInfoList::AddControlEvent(CString strKey,
								 CString strAction,
								 int glid,
								 int iMiscFloat2,
								 int iMiscFloat3,
								 int iMiscInt3,
								 BOOL bReplace,
								 BOOL bModifyExisting)
{
	int iControlType = 0;
	int iFinalKeyNum = GetKeyNum(strKey);

	if(iFinalKeyNum == -1)
	{
		iFinalKeyNum = GetMouseKey(strKey);
		if(iFinalKeyNum != -1) 
			iControlType = 1;
	}
	if(iFinalKeyNum == -1) return;

	int iActionNum = 0; //50;
	if(iMiscInt3 != 50) iActionNum = GetActionNum(strAction);
	if(bReplace) UnassignByKeynum(iFinalKeyNum);

	ControlInfoObj* pExisting = GetByActionName(strAction);
	if(iControlType == 1) iFinalKeyNum %= KEYBOARD_MAX_KEYS;
	if(pExisting != NULL && bModifyExisting == TRUE)
	{
		pExisting->List_miscFloat1 = (float)glid;
		pExisting->List_miscFloat2 = (float)iMiscFloat2;
		pExisting->List_miscFloat3 = (float)iMiscFloat3;
		pExisting->List_description = strAction;
		pExisting->List_controlType = iControlType;
		pExisting->List_buttonDown = iFinalKeyNum;
		pExisting->List_action = iActionNum;
	}
	else
	{
		ControlInfoObj *cPtr = new ControlInfoObj(iControlType,
			iFinalKeyNum,
			iActionNum,
			strAction,
			(float)glid,
			(float)iMiscFloat2,
			(float)iMiscFloat3);
		AddTail(cPtr);
	}   
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Function:	AddControlEvent(CString, ControlInfoObj**, int&)
// Description:	Retrieve a ControlInfoObj based on its action text description.
///////////////////////////////////////////////////////////////////////////////

void
ControlInfoList::GetByDescription(CString strDescription, ControlInfoObj **pControlInfo, int &iListIndex)
{
	 *pControlInfo = NULL;
	 iListIndex = 0;
     for(POSITION pos = GetHeadPosition(); pos != NULL;)
	 {
         ControlInfoObj *pTemp = (ControlInfoObj*)GetNext(pos);
		 if(pTemp->List_description.CompareNoCase(strDescription) == 0)
		 {
		    *pControlInfo = pTemp;
			break;
		 }
         iListIndex++;
	 }
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlInfoList
// Function:	ReplaceControlEvent(int, CString, CString, int, int, int, int)
// Description:	Rplace a ControlInfoObj (action->key binding) in this
//				linked list.
///////////////////////////////////////////////////////////////////////////////

void
ControlInfoList::ReplaceControlEvent(int iTargetIndex,
									 CString strKey,
									 CString strAction,
									 int glid,
									 int iMiscFloat2,
									 int iMiscFloat3,
									 int iMiscInt3)
{
	if(iTargetIndex < 0) return;

	// Find the key/button ID based on its text description
	int iControlType = 0;
	int iFinalKeyNum = GetKeyNum(strKey);

	if(iFinalKeyNum == -1)
	{
		iFinalKeyNum = GetMouseKey(strKey);
		if(iFinalKeyNum != -1) 
		{
			iControlType = 1;
			iFinalKeyNum %= KEYBOARD_MAX_KEYS;
		}
	}
	if(iFinalKeyNum != -1) return;

	// Retrieve the ControlInfoObj we want to replace and update its info
	int iActionNum = 0; //50;
	if(iMiscInt3 != 50)
		iActionNum = GetActionNum(strAction);

	POSITION pos = FindIndex(iTargetIndex);
	if(!pos) return;

	ControlInfoObj * pControlInfo = (ControlInfoObj*)GetAt(pos);
	pControlInfo->List_miscFloat1 = (float)glid;
	pControlInfo->List_miscFloat2 = (float)iMiscFloat2;
	pControlInfo->List_miscFloat3 = (float)iMiscFloat3;
	pControlInfo->List_description = strAction;
	pControlInfo->List_controlType = iControlType;
	pControlInfo->List_buttonDown = iFinalKeyNum;
	pControlInfo->List_action = iActionNum;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlDBObj
// Function:	Ctor/Dtor
///////////////////////////////////////////////////////////////////////////////

ControlDBObj::ControlDBObj()
{
	List_controlBasis = 1;
	List_controlConfigName = "";
	List_controlInfoList = new ControlInfoList();
	m_invertMouse = FALSE;
	m_mouseSensitivity = 0.0f;
	m_musicVolume = 0.0f;
	m_allowMultiBind = FALSE;
}

ControlDBObj::ControlDBObj(int L_controlBasis, 
						   CString L_controlConfigName,
		                   ControlInfoList *L_controlInfoList)
{
	List_controlBasis = L_controlBasis;
	List_controlConfigName = L_controlConfigName;
	List_controlInfoList = L_controlInfoList;
	m_invertMouse = FALSE;
	m_mouseSensitivity = 0.0f;
	m_musicVolume = 0.0f;
	m_allowMultiBind = FALSE;
}

ControlDBObj::~ControlDBObj()
{
	SafeDelete(List_controlInfoList);
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlDBObj
// Function:	GetButtonVis(int, int)
// Description:	Returns a string description of a button/key provided its ID.
///////////////////////////////////////////////////////////////////////////////

CString
ControlDBObj::GetButtonVis(int button, int type)
{
	CString strResult = "Unassigned";
	std::map<int, CString>::const_iterator iter;

	switch(type)
	{
		case CTL_KEYBOARD:
		{
			iter = KeyMap.find(button);
			strResult.SetString(iter->second);
			break;
		}
		case CTL_MOUSE:
		{
			iter = MouseMap.find(button);
			strResult.SetString(iter->second);
			break;
		}
		default: break;			
	}
	return strResult;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlDBObj
// Function:	GetControlInfo(int, int, ButtonState[], ButtonState[])
// Description:	Determine what button/key has most recently been pressed.
//				Type notes: 0=keyboard/1=mouse/2=controller/3=force controller
///////////////////////////////////////////////////////////////////////////////

BOOL
ControlDBObj::GetControlInfo(int &iType,
							 int &iButtonID,
							 ButtonState aMouseButtons[MOUSE_MAX_BUTTONS],
							 ButtonState aKeyboardKeys[KEYBOARD_MAX_KEYS])
{
	std::map<int, CString>::const_iterator iter;

	// Check keyboard keys
	for(iter = KeyMap.begin(); iter != KeyMap.end(); iter++)
	{
		if(aKeyboardKeys[iter->first].IsDown())
		{
			iType = 0;
			iButtonID = iter->first;
			return TRUE;
		}
	}

	// Check mouse buttons
	for(iter = MouseMap.begin(); iter != MouseMap.end(); iter++)
	{
		if(aMouseButtons[iter->first].IsDown())
		{
			iType = 1;
			iButtonID = iter->first;
			return TRUE;
		}
	}

	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlDBObj
// Function:	GetKeyIfPressed(CString*, ButtonState[])
// Description:	Determine what key has most recently been pressed.
///////////////////////////////////////////////////////////////////////////////

BOOL 
ControlDBObj::GetKeyIfPressed(CString *pKey, ButtonState aKeyboardKeys[KEYBOARD_MAX_KEYS])
{
	*pKey = "5555555";//def

	std::map<int, CString>::const_iterator iter;
	for(iter = KeyMap.begin(); iter != KeyMap.end(); iter++)
	{
		if(aKeyboardKeys[iter->first].IsDown())
		{
			*pKey = iter->second;
			return TRUE;
		}
	}
	return FALSE;
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlDBObj
// Function:	Serialize(CArchive&)
// Description:	Write this ControlDBObj to storage.
///////////////////////////////////////////////////////////////////////////////

void
ControlDBObj::Serialize(CArchive& ar)
{
	BOOL reserved = 0;
	CObject::Serialize( ar );
	if(ar.IsStoring())
	{
		ar.SetObjectSchema(2);
		//ASSERT(ar.GetObjectSchema() == 2);

		ar << List_controlConfigName 
		   << List_controlInfoList 
		   << List_controlBasis 
		   << m_invertMouse
		   << m_allowMultiBind
		   << reserved
		   << reserved
		   << reserved
		   << m_mouseSensitivity
		   << m_musicVolume;
	}
	else
	{
		int version = ar.GetObjectSchema();

        ar >> List_controlConfigName 
		   >> List_controlInfoList 
		   >> List_controlBasis 
		   >> m_invertMouse
		   >> m_allowMultiBind
		   >> reserved
		   >> reserved
		   >> reserved
		   >> m_mouseSensitivity;

		if (version >= 2)
			ar >> m_musicVolume;
	}
}

///////////////////////////////////////////////////////////////////////////////
// Class:		ControlDBList
// Function:	Ctor/Dtor
///////////////////////////////////////////////////////////////////////////////

ControlDBList::ControlDBList(){}

ControlDBList::~ControlDBList()
{
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
	    ControlDBObj *pControlDB = (ControlDBObj*)GetNext(posLoc);
		SafeDelete(pControlDB);
	}
	RemoveAll();
}
