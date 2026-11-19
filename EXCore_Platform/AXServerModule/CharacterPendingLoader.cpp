#include "StdAfx.h"
#include "characterpendingloader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCharacterPendingLoader::CCharacterPendingLoader(void)
{ 
    InitializeCriticalSection(&m_sect);
}

CCharacterPendingLoader::~CCharacterPendingLoader(void)
{
    DeleteCriticalSection(&m_sect);
}

bool CCharacterPendingLoader::AddCharacterToPendingList(CString username,int charIndex)
{
   CCharacterPendingObj * newPObj = new CCharacterPendingObj();
   newPObj->m_charIndex			= charIndex;
   newPObj->m_username			= username;
   newPObj->m_objectOption		= NULL;
   CAxLinkObj * nPtr = new CAxLinkObj();
   nPtr->m_thisObject = newPObj;

   AddTail(nPtr);

   return true;
}


bool CCharacterPendingLoader::GetNextCharacterToLoad(CCharacterPendingObj ** retPtr)
{
   EnterCriticalSection(&m_sect);
   CAxLinkObj * posLoc = GetHeadPosition_unsafe();
   if(!posLoc)
   {
       LeaveCriticalSection(&m_sect);
       return false;
   }
   *retPtr = (CCharacterPendingObj *)posLoc->m_thisObject;
   RemoveAt_unsafe(posLoc);
   LeaveCriticalSection(&m_sect);
   return true;

}

