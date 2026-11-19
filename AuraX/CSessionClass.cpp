#include "stdafx.h"

#include <afxtempl.h>

#include "CSessionClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSessionObj::CSessionObj()
{
    m_applicationDesc  = NULL;
    m_hostAddress      = NULL;
    m_deviceInfo       = NULL;
}


void CSessionObj::SafeDelete()
{
    if(m_hostAddress)
       m_hostAddress->Release();
	   m_hostAddress=0;

	if(m_deviceInfo)
       m_deviceInfo->Release();
	   m_deviceInfo=0;
}

BOOL CSessionObjList::AddObjectIfNotAdded(DPN_APPLICATION_DESC * applicationDesc,
                                          IDirectPlay8Address  * hostAddress,
	                                      IDirectPlay8Address  * deviceInfo)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CSessionObj * sessionPtr = (CSessionObj *)GetNext(posLoc);
        if(memcmp(sessionPtr->m_applicationDesc,applicationDesc,sizeof(DPN_APPLICATION_DESC))==0){
           
			
			
			return FALSE;//found it already
		}
	}
    
	CSessionObj * sessionObj = new CSessionObj();
	sessionObj->m_applicationDesc = new DPN_APPLICATION_DESC;
	CopyMemory(sessionObj->m_applicationDesc,applicationDesc,sizeof(DPN_APPLICATION_DESC));
    sessionObj->m_hostAddress      = hostAddress;
    sessionObj->m_deviceInfo       = deviceInfo;
    AddTail(sessionObj);
    return TRUE;
}

void CSessionObjList::SafeDelete()
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
         CSessionObj * sessionPtr = (CSessionObj *)GetNext(posLoc);
         sessionPtr->SafeDelete();
		 delete sessionPtr;
		 sessionPtr = 0;
	 }
     RemoveAll();
}