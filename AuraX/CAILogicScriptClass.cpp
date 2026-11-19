#include "stdafx.h"

#include <afxtempl.h>
#include "CAILogicScriptClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//---------------------------------------------------------//
//---------------- Logic Script Class----------------------//
//---------------------------------------------------------//
CAILogicScriptObj::CAILogicScriptObj()
{
    scriptName			= "Default Name";
	scriptDescription	= "Default Description";
	whenToUse			= 0;
	lastUseTimeStamp	= 0;
	howOftenTimer		= 0;
	scriptAction		= 0;
	miscInt1			= 0;
	miscInt2			= 0;
	miscInt3			= 0;
	emoteMessage		= "NA";
	listPlacement		= 0;
}

void CAILogicScriptObj::Clone(CAILogicScriptObj ** clone)
{
	CAILogicScriptObj * copyLocal	= new CAILogicScriptObj();

	copyLocal->scriptName           = scriptName;
	copyLocal->scriptDescription    = scriptDescription;
	copyLocal->whenToUse			= whenToUse;
	copyLocal->lastUseTimeStamp		= lastUseTimeStamp;
	copyLocal->howOftenTimer		= howOftenTimer;
	copyLocal->scriptAction			= scriptAction;
	copyLocal->miscInt1				= miscInt1;
	copyLocal->miscInt2				= miscInt2;
	copyLocal->miscInt3				= miscInt3;
	copyLocal->emoteMessage			= emoteMessage;
	copyLocal->listPlacement		= listPlacement;

	*clone = copyLocal;
}

void CAILogicScriptObj::Serialize(CArchive &ar)
{
	CObject::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << scriptName
		<< scriptDescription
		<< whenToUse
		<< lastUseTimeStamp
		<< howOftenTimer
		<< scriptAction
		<< miscInt1
		<< miscInt2
		<< miscInt3
		<< emoteMessage
		<< listPlacement;
	}
	else
	{
		ar >> scriptName
		>> scriptDescription
		>> whenToUse
		>> lastUseTimeStamp
		>> howOftenTimer
		>> scriptAction
		>> miscInt1
		>> miscInt2
		>> miscInt3
		>> emoteMessage
		>> listPlacement;
	}
}

CAILogicScriptObj * CAILogicScriptObjList::GetScriptObject(int index)
{
	if(index < 0)
		return NULL;
	
	POSITION posLoc = FindIndex(index);
	if(!posLoc)
		return NULL;

	CAILogicScriptObj *scriptPtr = (CAILogicScriptObj *)GetAt(posLoc);
	CAILogicScriptObj *newPtr = NULL;
	scriptPtr->Clone(&newPtr);
	return newPtr;
}

void CAILogicScriptObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
        CAILogicScriptObj *scriptObj = (CAILogicScriptObj*)GetNext(posLoc);		
		
		if(scriptObj)
		{
			delete scriptObj;
			scriptObj = 0;
		}
	}
	RemoveAll();
}

void CAILogicScriptObjList::Clone(CAILogicScriptObjList **copy)
{
	// Create and init the clone list
	CAILogicScriptObjList* copyLocal = new CAILogicScriptObjList();

	// Copy each inventory node from this list to the clone
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		CAILogicScriptObj *scriptObj = (CAILogicScriptObj*)GetNext(posLoc);
		CAILogicScriptObj *cpyPtr = NULL;
		scriptObj->Clone(&cpyPtr);
		copyLocal->AddTail(cpyPtr);
	}

	// Set the in param to point to our instantiated clone
	*copy = copyLocal;
}

IMPLEMENT_SERIAL(CAILogicScriptObj, CObject, 0)
IMPLEMENT_SERIAL(CAILogicScriptObjList, CObList, 0)