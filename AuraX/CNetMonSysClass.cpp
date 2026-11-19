#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>

#include "CNetMonSysClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CNetMonObj::CNetMonObj()
{
    m_dpnid              = 0;
	m_currentMsgCount    = 0;
	m_currentTimeStamp   = 0;
}


void CNetMonSystem::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CNetMonObj * spnPtr = (CNetMonObj *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

CNetMonObj * CNetMonSystem::GetObjByID(DPNID id)
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CNetMonObj * spnPtr = (CNetMonObj *)GetNext(posLoc);
		if(spnPtr->m_dpnid == id)
		   return spnPtr;
	 }
   return NULL;
}

BOOL CNetMonSystem::VerifyOnline(DPNID id)
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CNetMonObj * spnPtr = (CNetMonObj *)GetNext(posLoc);
		if(spnPtr->m_dpnid == id)
		   return TRUE;
	 }
   return FALSE;
}

BOOL CNetMonSystem::AddByID(DPNID id)
{
    CNetMonObj * newNetMonPtr = new CNetMonObj();
	newNetMonPtr->m_dpnid              = id;
	newNetMonPtr->m_currentMsgCount    = 0;
	newNetMonPtr->m_currentTimeStamp   = GetTickCount();
	currentConnectionCount++;
    AddTail(newNetMonPtr);
	return TRUE;
}

BOOL CNetMonSystem::DeleteByID(DPNID id)
{
	POSITION posLast;
    for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CNetMonObj * spnPtr = (CNetMonObj *)GetNext(posLoc);
		if(spnPtr->m_dpnid == id)
		{
           delete spnPtr;
           spnPtr = 0;
           RemoveAt(posLast);
		   currentConnectionCount--;
		}
	}
	return TRUE;
}

BOOL CNetMonSystem::VerifyIsOkToTakeMsg(DPNID id,long maxMsgCountPerSec)
{
     DWORD currentTime = GetTickCount();
 
	 CNetMonObj * spnPtr = GetObjByID(id);
     if(!spnPtr)
		 return FALSE;

	 if(spnPtr->m_currentMsgCount<maxMsgCountPerSec)
	 {
        spnPtr->m_currentMsgCount++;
	 }
	 else
	   return FALSE;
	 
	 
	 
	 if((currentTime - spnPtr->m_currentTimeStamp) > 1000) 
	 {
		 spnPtr->m_currentMsgCount = 0;
         spnPtr->m_currentTimeStamp = currentTime;
	 }
	 return TRUE;
}