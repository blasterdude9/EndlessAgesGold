#include "stdafx.h"

#include <afxtempl.h>

#include "CFilterSpeechClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CFilterSpeechObj::CFilterSpeechObj()
{

}


void CFilterSpeechList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CFilterSpeechObj * spnPtr = (CFilterSpeechObj *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

void CFilterSpeechList::AddName(CString name)
{
     if(IsNameAlreadyInList(name))
		 return;

     CFilterSpeechObj * ptr = new CFilterSpeechObj();
     ptr->m_playersName = name;
     AddTail(ptr);
}

void CFilterSpeechList::UnBlockName(CString name)
{
	name.MakeUpper();
	POSITION posLast = NULL;
     for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CFilterSpeechObj * spnPtr = (CFilterSpeechObj *)GetNext(posLoc);
		if(spnPtr->m_playersName == name)
		{
		   delete spnPtr;
		   spnPtr = 0;
           RemoveAt(posLast);
		   return;
		}
	}
}

BOOL CFilterSpeechList::IsNameAlreadyInList(CString name)
{
	name.MakeUpper();
	POSITION posLast = NULL;
     for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CFilterSpeechObj * spnPtr = (CFilterSpeechObj *)GetNext(posLoc);
		if(spnPtr->m_playersName == name)
		{
		   return TRUE;
		}
	}
	return FALSE;
}

BOOL CFilterSpeechList::ShouldStatmentBeBlocked(CString statment)
{
	BOOL matches = TRUE;
	statment.MakeUpper();//format
	int statmentLength = statment.GetLength();
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CFilterSpeechObj * spnPtr = (CFilterSpeechObj *)GetNext(posLoc);
        int curNameLength = spnPtr->m_playersName.GetLength(); 
        matches = TRUE;
		for(int loop=0;loop<curNameLength;loop++)
		{
            if(loop >= statmentLength)
			   break;

            if(statment.GetAt(loop) == spnPtr->m_playersName.GetAt(loop))
			{
               
              
			}
			else{
               matches = FALSE;
			   break;
			}
		}
		if(matches)
		   return TRUE;
	}
	return FALSE;
}

void CFilterSpeechObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_playersName;
	}
	else
	{
       ar >> m_playersName;
	}
}     


IMPLEMENT_SERIAL(CFilterSpeechObj, CObject, 0)
IMPLEMENT_SERIAL(CFilterSpeechList, CObList, 0)

