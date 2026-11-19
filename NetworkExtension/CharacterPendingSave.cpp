#include "StdAfx.h"
#include "characterpendingsave.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCharacterPendingSave::CCharacterPendingSave(void)
{
    InitializeCriticalSection(&m_cSection);
}

CCharacterPendingSave::~CCharacterPendingSave(void)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CCharacterPendingSaveObj * ptr = (CCharacterPendingSaveObj *)GetNext(posLoc);
        delete ptr;
		ptr = 0;
	}
	RemoveAll();
    DeleteCriticalSection(&m_cSection);
}

CCharacterPendingSaveObj::~CCharacterPendingSaveObj(void)
{
   
    if(playerRef)
	{
       delete playerRef;
	   playerRef = 0;
	}
}

bool CCharacterPendingSave::AddCharacterToPendingList(CPlayerObject * playerRef,int logOff,CString text,int handleOverride)
{
   CCharacterPendingSaveObj * newPObj = new CCharacterPendingSaveObj();
   if(playerRef)
   {
      playerRef->CloneMinimum(&newPObj->playerRef);//Time this
   }
   newPObj->m_text = text;
   newPObj->m_optionalHandle = handleOverride;


   newPObj->m_logOff = logOff;
   EnterCriticalSection(&m_cSection);
   AddTail(newPObj);
   LeaveCriticalSection(&m_cSection);
   return true;
}


bool CCharacterPendingSave::GetNextCharacterToLoad(CCharacterPendingSaveObj ** retPtr)
{
   EnterCriticalSection(&m_cSection);
   POSITION posLoc = GetHeadPosition();
   if(!posLoc)
   {
       LeaveCriticalSection(&m_cSection);
       return false;
   }
   CCharacterPendingSaveObj * curPObj = (CCharacterPendingSaveObj *)GetAt(posLoc);
   *retPtr = curPObj;
   RemoveAt(posLoc);
   LeaveCriticalSection(&m_cSection);
   return true;

}
