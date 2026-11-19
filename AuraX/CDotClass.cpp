#include "stdafx.h"
#include <afxtempl.h>
#include "CDotClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Default Constructor for a DoT
CDotClass::CDotClass()
{
	m_dotType		= -1;
	m_dotChannel	= -1;
	m_dotDuration	= 0;
	m_dotAmount		= 0;
	m_dotPlayerID	= 0;
	m_dotEffectRef	= -1;
	m_dotApplyPer	= 0;
	m_dotName		= "Default";
	m_dotIndex		= -1;
	m_dotTimeStamp	= GetTickCount();
}

void CDotClass::Clone(CDotClass **copy)
{
	CDotClass *copyLocal		= new CDotClass();
	copyLocal->m_dotAmount		= m_dotAmount;
	copyLocal->m_dotChannel		= m_dotChannel;
	copyLocal->m_dotDuration	= m_dotDuration;
	copyLocal->m_dotPlayerID	= m_dotPlayerID;
	copyLocal->m_dotType		= m_dotType;
	copyLocal->m_dotEffectRef	= m_dotEffectRef;
	copyLocal->m_dotName		= m_dotName;
	copyLocal->m_dotApplyPer	= m_dotApplyPer;
	copyLocal->m_dotTimeStamp	= m_dotTimeStamp;
	copyLocal->m_dotIndex		= m_dotIndex;

	*copy = copyLocal;
}

void CDotClass::SafeDelete()
{

}

void CDotClass::Serialize(CArchive &ar)
{
	BOOL reseved = FALSE;

	CObject::Serialize(ar);
	if(ar.IsStoring())
	{		
		ar << m_dotName
		   << m_dotDuration
		   << m_dotEffectRef
		   << m_dotAmount
		   << m_dotType
		   << m_dotChannel
		   << m_dotApplyPer
		   << m_dotIndex;
	}
	else
	{		
		ar >> m_dotName
		   >> m_dotDuration
		   >> m_dotEffectRef
		   >> m_dotAmount
		   >> m_dotType
		   >> m_dotChannel
		   >> m_dotApplyPer
		   >> m_dotIndex;

		m_dotTimeStamp = GetTickCount();
	}
}

//Default Constructor for the List of DoTs
CDotClassList::CDotClassList()
{
	m_dotLimit			= 0;
	m_currentDotAmount	= 0;
}

void CDotClassList::Clone(CDotClassList **copy)
{
	CDotClassList *copyLocal		= new CDotClassList();
	copyLocal->m_currentDotAmount	= m_currentDotAmount;
	copyLocal->m_dotLimit			= m_dotLimit;

	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		CDotClass *dotObj = (CDotClass *)GetNext(posLoc);
		CDotClass *cpyPtr = NULL;
		dotObj->Clone(&cpyPtr);
		copyLocal->AddTail(cpyPtr);
	}
	
	*copy = copyLocal;
}

void CDotClassList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		CDotClass *dotObj = (CDotClass *)GetNext(posLoc);
		delete dotObj;
		dotObj = 0;
	}

	RemoveAll();
}

//Adds a DoT to the list
void CDotClassList::AddDotObject(CDotClass *dotInfo)
{
	if(m_currentDotAmount > m_dotLimit)
		return;

	CDotClass *dotObj = new CDotClass();
	dotObj->Clone(&dotInfo);
	m_currentDotAmount++;
	AddTail(dotObj);
}

//Remove a DoT if the timer reaches 0
void CDotClassList::RemoveDotCheckByDuration()
{
	if(m_currentDotAmount > 0)
	{
		POSITION posLast = NULL;
		for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
		{
			CDotClass *dotObj = (CDotClass *)GetNext(posLoc);
			if(dotObj->m_dotDuration <= 0)
			{
				delete dotObj;
				dotObj = 0;
				RemoveAt(posLast);
				m_currentDotAmount--;
			}
		}
	}
}

//Remove All DoTs from player List
void CDotClassList::RemoveAllDots()
{
	if(m_currentDotAmount > 0)
	{
		POSITION posLast = NULL;
		for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
		{
			CDotClass *dotObj = (CDotClass *)GetNext(posLoc);
			if(dotObj)
			{
				delete dotObj;
				dotObj = 0;
				RemoveAt(posLast);
				m_currentDotAmount--;
			}
		}
	}
}

//Remove a specific DoT by ID
void CDotClassList::RemoveDotByID(int id)
{
	//Check for valid ID
	if(id < 0)
		return;

	//Make sure we have a DoT 
	if(m_currentDotAmount <= 0)
		return;

	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		CDotClass *dotObj = (CDotClass *)GetNext(posLoc);
		if(dotObj->m_dotIndex == id)
		{
			delete dotObj;
			dotObj = 0;
			RemoveAt(posLoc);
			m_currentDotAmount--;
		}
	}
}

//Removes a specific DoT by type
BOOL CDotClassList::RemoveDotByType(int type)
{
	if(m_currentDotAmount > 0)
	{
		POSITION posLast;
		for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
		{
			CDotClass *dotObj = (CDotClass *)GetNext(posLoc);
			if(type == dotObj->m_dotType)
			{
				delete dotObj;
				dotObj = 0;
				RemoveAt(posLast);

				return TRUE;
			}
		}
	}

	return FALSE;
}

//Removes a specific DoT by channel
BOOL CDotClassList::RemoveDotByChannel(int channel)
{
	if(m_currentDotAmount > 0)
	{
		POSITION posLast;
		for(POSITION posLoc = GetHeadPosition(); (posLast = posLoc) != NULL;)
		{
			CDotClass *dotObj = (CDotClass *)GetNext(posLoc);
			if(channel == dotObj->m_dotChannel)
			{
				delete dotObj;
				dotObj = 0;
				RemoveAt(posLast);

				return TRUE;
			}
		}
	}

	return FALSE;
}

//Acquire a Dot by ID
CDotClass* CDotClassList::GetDotByID(int dotIndex)
{
	for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	{
		CDotClass *pObj = (CDotClass *)GetNext(posLoc);
		if(pObj->m_dotIndex == dotIndex)
			return pObj;
	}
	
	return NULL;
}

IMPLEMENT_SERIAL(CDotClass, CObject, 0)
IMPLEMENT_SERIAL(CDotClassList, CObList, 0)