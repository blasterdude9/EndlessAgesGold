#include "stdafx.h"
#define D3D_OVERLOADS

#include <afxtempl.h>
#include "CpendingCommandsClass.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPendingCommandObj::CPendingCommandObj()
{
    m_command       = 0;
	m_done          = 0;
	m_timeCreated   = GetTickCount();
	m_timeToExec    = 0;
	m_register0     = 0;
	m_register1     = 0;
}


void CPendingCommandObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CPendingCommandObj * spnPtr = (CPendingCommandObj *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}


CPendingCommandObj * CPendingCommandObjList::CallBack()
{
   long currentTime = GetTickCount();
   POSITION posLast;
   for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CPendingCommandObj * spnPtr = (CPendingCommandObj *)GetNext(posLoc);
        if(spnPtr->m_done)
		{
		   delete spnPtr;
		   spnPtr = 0;
		   RemoveAt(posLast);
           continue;
		}
		
		if(((long)GetTickCount()-spnPtr->m_timeCreated) > spnPtr->m_timeToExec)
		{
            spnPtr->m_done = 1;
            return spnPtr;
		}
   }
   return NULL;
}

BOOL CPendingCommandObjList::AddCommand(int command, 
										int register0, 
										int register1, 
										long duration,
										CString specialString)
{
     CPendingCommandObj * newPtr = new CPendingCommandObj();
	 newPtr->m_command = command;
	 newPtr->m_timeToExec = duration;
	 newPtr->m_register0 = register0;
	 newPtr->m_register1 = register1;
	 newPtr->m_specialString = specialString;
     AddTail(newPtr);
	 return TRUE;
}

