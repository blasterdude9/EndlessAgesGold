#include "stdafx.h"

#include "direct.h"
#include "d3d9.h"
#include <afxtempl.h>

#include "CServerEventLogClass.h"
#include "LoggingClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CServerEventObj::CServerEventObj()
{
    m_posted = FALSE;

}

void CServerEventObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() )
	{	
	   ar << m_eventDescription;
	}
	else
	{
       ar >> m_eventDescription;
	   m_posted = FALSE;
	}
}     

void CServerEventList::AddEvent(CString txt,D3DVECTOR position)
{
    CString posX = m_cabLib.FloatToStingCnv(position.x);
	CString posY = m_cabLib.FloatToStingCnv(position.y);	
	CString posZ = m_cabLib.FloatToStingCnv(position.z);
	AddEvent(txt + " X:"+ posX + " Y:" + posY + " Z:" + posZ);
}

void CServerEventList::AddEvent(D3DVECTOR position)
{
    CString posX = m_cabLib.FloatToStingCnv(position.x);
	CString posY = m_cabLib.FloatToStingCnv(position.y);	
	CString posZ = m_cabLib.FloatToStingCnv(position.z);
	AddEvent("X:"+ posX + " Y:" + posY + " Z:" + posZ);
}

void CServerEventList::AddEvent(CString str1,CString str2,CString str3)
{
    CString combo = operator+(str1,":");
	combo = operator+(operator+(combo,str2),":");
	combo = operator+(combo,str3);
	AddEvent(combo);
}

void CServerEventList::AddEvent(CString str1,CString str2,DWORD integer)
{
    CString combo = operator+(str1,":");
	combo = operator+(operator+(combo,str2),":");	
	combo = operator+(combo,m_cabLib.ConvToString(integer));
	AddEvent(combo);
}


void CServerEventList::AddEvent(CString str1,CString str2)
{
    CString combo = operator+(str1,":");
	combo = operator+(combo,str2);	
	AddEvent(combo);
}

void CServerEventList::AddEvent(CString str1,float floatingPoint)
{
    CString combo = operator+(str1,":");
	combo = operator+(combo,m_cabLib.FloatToStingCnv(floatingPoint));	
	AddEvent(combo);
}

void CServerEventList::AddEvent(CString str1,DWORD integer)
{
   
	CString combo = operator+(str1,":");
	combo = operator+(combo,m_cabLib.ConvToString(integer));	
	AddEvent(combo);
}

void CServerEventList::AddEvent(float event)
{
   AddEvent(m_cabLib.FloatToStingCnv(event));
}

void CServerEventList::AddEvent(CString event)
{
   CServerEventObj * newEvent = new CServerEventObj();
   event.Replace("'","-");

   newEvent->m_eventDescription = event;
   AXLogSinkWrite(event);
   EnterCriticalSectionARB();
   AddTail(newEvent);
   LeaveCriticalSectionARB();
}

CString CServerEventList::GetOldestEntryMarkIt()
{
   
   CString temp = "";
   CServerEventObj * lastEventObj = NULL;
   EnterCriticalSectionARB();
   for(POSITION posLoc = GetTailPosition();posLoc != NULL;)
   {
	   CServerEventObj * sEventObj = (CServerEventObj *)GetPrev(posLoc);
	   if(sEventObj->m_posted == TRUE)
		  break;//posted all new messages
			
	   lastEventObj = sEventObj;
	   temp = sEventObj->m_eventDescription;
   }
   if(lastEventObj)
      lastEventObj->m_posted = TRUE;

   LeaveCriticalSectionARB();
   return temp;
}

void CServerEventList::EnableCriticalSection()
{
     if(!m_criticalSectionOption)
	 {
		 m_criticalSectionOption = new CRITICAL_SECTION;
         InitializeCriticalSection(m_criticalSectionOption);
         m_criticalSectionEnabled = TRUE;
	 }
}

void CServerEventList::EnterCriticalSectionARB()
{
     if(m_criticalSectionEnabled)
        EnterCriticalSection(m_criticalSectionOption);
}

void CServerEventList::LeaveCriticalSectionARB()
{
     if(m_criticalSectionEnabled)
        LeaveCriticalSection(m_criticalSectionOption);
}

void CServerEventList::OutPutTextFile(CString filename)
{
	EnterCriticalSectionARB();
	CFileException exc;
    CFile fl;
	CArchive the_out_Archive(&fl,CArchive::store);
    BOOL res = fl.Open(filename,CFile::modeCreate | CFile::modeWrite, &exc);
	 for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
      CServerEventObj * spnPtr = (CServerEventObj *)GetNext(posLoc);
       the_out_Archive.WriteString( spnPtr->m_eventDescription );  
	   the_out_Archive.WriteString( "\r\n");  
	 }
	 the_out_Archive.Close();
     fl.Close();
	 LeaveCriticalSectionARB();
}

void CServerEventList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CServerEventObj * spnPtr = (CServerEventObj *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();

	
}

void CServerEventList::LogByPriority(CString info, int priority)
{
     if(m_logPriority < priority)
        return;

     AddEvent(info);
}

void CServerEventList::LogByPriority(CString info,DWORD integer, int priority)
{
     if(m_logPriority < priority)
        return;

     AddEvent(info,integer);
}

IMPLEMENT_SERIAL(CServerEventObj, CObject, 0)
IMPLEMENT_SERIAL(CServerEventList, CObList, 0)

