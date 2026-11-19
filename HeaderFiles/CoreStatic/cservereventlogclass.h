
#include "CABFunctionLib.h"
#ifndef CSERVEREVENTLOG_H
#define CSERVEREVENTLOG_H



class CServerEventObj : public CObject
{
	
     
	DECLARE_SERIAL( CServerEventObj );
	
    
public:
    CServerEventObj();
    
	CString m_eventDescription;
	BOOL    m_posted;

	void Serialize( CArchive& ar );
	
};

class CServerEventList : public CObList
{
public:
   CServerEventList(){m_criticalSectionOption=NULL;m_criticalSectionEnabled=FALSE;m_logPriority=3;};
   ~CServerEventList()
   {
       for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	   {
		   CServerEventObj * ptr = (CServerEventObj *) GetNext(posLoc);
		   delete ptr;
		   ptr = 0;
	   }
	   RemoveAll();

	   if(m_criticalSectionOption)
	   {
		   DeleteCriticalSection( m_criticalSectionOption);
		   delete m_criticalSectionOption;
		   m_criticalSectionOption=0;
	   }
   };

   CRITICAL_SECTION * m_criticalSectionOption;
   BOOL m_criticalSectionEnabled;

   CABFunctionLib                m_cabLib;
   int							 m_logPriority;

   void SafeDelete();
   void AddEvent(CString event);
   void AddEvent(float event);
   void AddEvent(CString str1,DWORD integer);
   void AddEvent(CString str1,float floatingPoint);
   void AddEvent(CString str1,CString str2);
   void AddEvent(CString str1,CString str2,CString str3);
   void AddEvent(CString str1,CString str2,DWORD integer);
   void OutPutTextFile(CString filename);
   CString GetOldestEntryMarkIt();
   void AddEvent(D3DVECTOR position);
   void AddEvent(CString txt,D3DVECTOR position);
   void EnableCriticalSection();
   void EnterCriticalSectionARB();
   void LeaveCriticalSectionARB();
   void LogByPriority(CString info, int priority);
   void LogByPriority(CString info,DWORD integer, int priority);

   DECLARE_SERIAL( CServerEventList );
	
    
};

#endif