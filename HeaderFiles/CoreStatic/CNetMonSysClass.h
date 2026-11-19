#include "dplay8.h"
#ifndef CNEWMONSYSCLASS_H
#define CNEWMONSYSCLASS_H

class   CNetMonObj : public CObject
{
public:
    CNetMonObj();
    
	DPNID           m_dpnid;
	long            m_currentMsgCount;
	DWORD           m_currentTimeStamp;
};

class   CNetMonSystem : public CObList
{
   
public:
   CNetMonSystem(){currentConnectionCount=0;};
   void SafeDelete();
   int currentConnectionCount;

   BOOL VerifyOnline(DPNID id);
   BOOL AddByID(DPNID id);
   BOOL DeleteByID(DPNID id);
   BOOL VerifyIsOkToTakeMsg(DPNID id,long maxMsgCountPerSec);//returns TRUE if ok
   CNetMonObj * GetObjByID(DPNID id);
};

#endif