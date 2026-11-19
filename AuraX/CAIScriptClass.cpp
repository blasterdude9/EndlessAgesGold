#include "stdafx.h"

#include <afxtempl.h>
#include "CAIScriptClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//---------------------------------------------------------//
//-------------------Event Class---------------------------//
//---------------------------------------------------------//
CAIScriptEvent::CAIScriptEvent()
{
	int m_event    = 0;//0=goTo location on tree 
	int m_miscInt  = 0;
}

void CAIScriptEvent::Clone(CAIScriptEvent ** clone)
{
	CAIScriptEvent * copyLocal = new CAIScriptEvent();
	copyLocal->m_event    = m_event;//0=goTo location on tree 
	copyLocal->m_miscInt  = m_miscInt;
	*clone = copyLocal;
}

void CAIScriptEvent::Serialize( CArchive& ar )
{
	CObject::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_event
		   << m_miscInt;
	}
	else
	{
		ar >> m_event
		   >> m_miscInt;
	}
}

CAIScriptEvent* CAIScriptEventList::GetEvent(int index)
{
	if(index < 0)
		return NULL;

	POSITION posLoc = FindIndex(index);
	if(!posLoc)
		return NULL;
	
	return (CAIScriptEvent*)GetAt(posLoc);
}

void CAIScriptEventList::AddEvent(int event,int miscInt)
{
	CAIScriptEvent * eventPtr = new CAIScriptEvent();
	eventPtr->m_event   = event;
	eventPtr->m_miscInt = miscInt;
	AddTail(eventPtr);
}

void CAIScriptEventList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		CAIScriptEvent * eventPtr = (CAIScriptEvent *)GetNext(posLoc);
		delete eventPtr;
		eventPtr = 0;
	}
	RemoveAll();
}

void CAIScriptEventList::Clone(CAIScriptEventList ** clone)
{
	CAIScriptEventList * copyLocal = new CAIScriptEventList;
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		CAIScriptEvent * resPtr = (CAIScriptEvent *)GetNext(posLoc);
		CAIScriptEvent * clonePtr = NULL;
		resPtr->Clone(&clonePtr);
		copyLocal->AddTail(clonePtr);
	}
	*clone = copyLocal;
}

IMPLEMENT_SERIAL(CAIScriptEvent, CObject, 0)
IMPLEMENT_SERIAL(CAIScriptEventList, CObList, 0)
//---------------------------------------------------------//
//-------------------Script Class--------------------------//
//---------------------------------------------------------//
CAIScriptObj::CAIScriptObj()
{
	m_events             = NULL;
	scriptName           = "NA";
	scriptDescription    = "NA";
	m_currentEvent       = 0;
	m_delayDuration      = 0;
	m_delayStamp         = 0;
}

void CAIScriptObj::Clone(CAIScriptObj ** clone)
{
	CAIScriptObj * copyLocal = new CAIScriptObj();

	copyLocal->m_events = NULL;
	if(m_events)
		m_events->Clone(&copyLocal->m_events);

	copyLocal->scriptName           = scriptName;
	copyLocal->scriptDescription    = scriptDescription;
	copyLocal->m_currentEvent = 0;

	*clone = copyLocal;
}

CAIScriptEvent* CAIScriptObj::GetEvent(int index)
{
	if(m_events)
		return m_events->GetEvent(index);
	return NULL;
}

void CAIScriptObj::SafeDelete()
{
	if(m_events)
	{
		m_events->SafeDelete();
		delete m_events;
		m_events = 0;
	}
}

void CAIScriptObj::Serialize( CArchive& ar )
{
	CObject::Serialize(ar);
	if(ar.IsStoring())
	{
		ar << m_events
		   << scriptName
		   << scriptDescription;
	}
	else
	{
		ar >> m_events
		   >> scriptName
		   >> scriptDescription;

		m_currentEvent = 0;
	}
}

CAIScriptObj * CAIScriptObjList::GetScriptObject(int index)
{
	if(index<0)
		return NULL;
	
	POSITION posLoc = FindIndex(index);
	if(!posLoc)
		return NULL;
	
	CAIScriptObj * eventPtr = (CAIScriptObj *)GetAt(posLoc);
	CAIScriptObj * newPtr = NULL;
	eventPtr->Clone(&newPtr);
	
	return newPtr;
} 

void CAIScriptObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
		CAIScriptObj * eventPtr = (CAIScriptObj *)GetNext(posLoc);
		eventPtr->SafeDelete();
		delete eventPtr;
		eventPtr = 0;
	}
	RemoveAll();
}

IMPLEMENT_SERIAL(CAIScriptObj, CObject, 0)
IMPLEMENT_SERIAL(CAIScriptObjList, CObList, 0)