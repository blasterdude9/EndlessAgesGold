#include "StdAfx.h"
#include "cdatahandler.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCDataHandler::CCDataHandler(void)
{
	m_miscData					= NULL;
	m_int2						= 0;
	m_int3						= 0;
	m_dword2					= 0;
	m_timeAdded					= GetTickCount();
	m_durationTillExecution		= 0;
	m_type						= 0;
	m_int						= 0;
	m_dpnid						= 0;
	m_bufSz						= 0;
	m_dword						= 0;
}

CCDataHandler::~CCDataHandler(void)
{
    if(m_miscData)
	   delete [] m_miscData;//allocated with new byte[]
	m_miscData = 0;

}

bool CCDataHandlerDB::CheckForDuplicate(CString & text, int & type)
{
    for(POSITION posCow = GetHeadPosition();posCow != NULL;)
	{
        CCDataHandler * dbPtr = (CCDataHandler *)GetNext(posCow);
        if(dbPtr->m_type == type)
			if(dbPtr->m_text.CompareNoCase(text)==0)
			{//duplicate found
                return true;
			}//end duplicate found
	}
	return false;
}

bool CCDataHandlerDB::AddToQue(DWORD dwordType,
							   CString text,
							   int intMisc, 
							   DPNID dpnidMisc, 
							   int type, 
							   byte * data, 
							   int bufSize,
							   DWORD dwordType2,
							   int intMisc2,
							   int intMisc3,
							   CString text2)
{
	
	 CCDataHandler * newPtr = new CCDataHandler();
	 newPtr->m_text			= text;
	 newPtr->m_type			= type;
	 newPtr->m_int          = intMisc;
	 newPtr->m_dpnid        = dpnidMisc;
	 newPtr->m_bufSz        = bufSize;
	 newPtr->m_dword        = dwordType;
	 newPtr->m_dword2       = dwordType2;
	 newPtr->m_int2         = intMisc2;
	 newPtr->m_int3         = intMisc3;
	 newPtr->m_text2         = text2;
	 if(data)
       if(bufSize > 0)
	   {
          newPtr->m_miscData = new byte[bufSize];
	      CopyMemory(newPtr->m_miscData,data,bufSize);
	   }

	 EnterCriticalSection(&m_cSection);
     AddTail(newPtr);
	 m_queCount++;
	 LeaveCriticalSection(&m_cSection);
	 return true;
}



bool CCDataHandlerDB::AddToQue(DWORD dwordType,
							   CString text,
							   int intMisc, 
							   DPNID dpnidMisc, 
							   int type, 
							   byte * data, 
							   int bufSize)
{
     CCDataHandler * newPtr = new CCDataHandler();
	 newPtr->m_text			= text;
	 newPtr->m_type			= type;
	 newPtr->m_int          = intMisc;
	 newPtr->m_dpnid        = dpnidMisc;
	 newPtr->m_bufSz        = bufSize;
	 newPtr->m_dword        = dwordType;
	 if(data)
       if(bufSize > 0)
	   {
          newPtr->m_miscData = new byte[bufSize];
	      CopyMemory(newPtr->m_miscData,data,bufSize);
	   }

	 EnterCriticalSection(&m_cSection);
     AddTail(newPtr);
	 m_queCount++;
	 LeaveCriticalSection(&m_cSection);
	 return true;
}

bool CCDataHandlerDB::AddToQue(int miscInt,
							   int miscInt2,
							   int type, 
							   DWORD delay)
{
     CCDataHandler * newPtr = new CCDataHandler();
	 newPtr->m_type			= type;
	 newPtr->m_int          = miscInt;
	 newPtr->m_int2         = miscInt2;
	 newPtr->m_durationTillExecution = delay;

	 EnterCriticalSection(&m_cSection);
     AddTail(newPtr);
	 m_queCount++;
	 LeaveCriticalSection(&m_cSection);
	 return true;
}

long CCDataHandlerDB::GetQueCount()
{
	 long queCountTemp = 0;
     EnterCriticalSection(&m_cSection);
     queCountTemp = m_queCount;
     LeaveCriticalSection(&m_cSection);
	 return queCountTemp;
}


bool CCDataHandlerDB::GetNextObject(CCDataHandler ** dataObj)
{
	 EnterCriticalSection(&m_cSection);
     POSITION posLoc = GetHeadPosition();
     if(!posLoc)
	 {
		 LeaveCriticalSection(&m_cSection);
		 return false;
	 }

	 POSITION posLast = posLoc;
	 CCDataHandler * dPtr = (CCDataHandler *) GetNext(posLoc);
	 while(dPtr->m_durationTillExecution > 0)
	 {//get next object besides delayed execution objects
        if(GetTickCount()-dPtr->m_timeAdded >= dPtr->m_durationTillExecution || dPtr->m_durationTillExecution <= 0)
		   break;//process

		if(!posLoc)
		{
            LeaveCriticalSection(&m_cSection);
		    return false;
		}
		posLast = posLoc;
	    dPtr = (CCDataHandler *) GetNext(posLoc);
	 }//end get next object besides delayed execution objects


	 RemoveAt(posLast);
	 m_queCount--;
	 *dataObj = dPtr;
	 LeaveCriticalSection(&m_cSection);
	 return true;
}
