#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include "CStructuresMisl.h"
#include <afxtempl.h>
#include "CPacketQueClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPacketObj::CPacketObj()
{
       m_data					= NULL;
	   m_size                   = 0;
	   m_communicationID        = 0;
	   m_channelID              = 0;
}

CPacketObj::~CPacketObj()
{
	if(m_data){
       delete [] m_data;
	   m_data=0;
	}
}

void CPacketObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_size
		  << m_communicationID;
	      for(int loop=0;loop<m_size;loop++)
			  ar << m_data[loop];
	}
	else
	{
       ar >> m_size
		  >> m_communicationID;
	          m_data = new BYTE[m_size];
	      for(int loop=0;loop<m_size;loop++)
			  ar >> m_data[loop];
	}
}     

void CPacketObjList::AddPacket(int size,BYTE * data,DPNID id,int channelID)
{

     if(size < 1)
	    return;

 	 CPacketObj * packetPtr = new CPacketObj();
     packetPtr->m_size = size;  
	 packetPtr->m_channelID = channelID;
	 packetPtr->m_data = new BYTE[size];
	 packetPtr->m_communicationID = id;
     CopyMemory(packetPtr->m_data,data,size);
	 EnterCriticalSectionEXA();
     AddTail(packetPtr);
	 LeaveCriticalSectionEXA();
}

CPacketObj * CPacketObjList::GetOldestPacket()
{
 	 POSITION posLoc = GetHeadPosition();
	 if(!posLoc)
		 return NULL;
	
	 CPacketObj * packetPtr = (CPacketObj *)GetAt(posLoc);
     EnterCriticalSectionEXA();
	 RemoveAt(posLoc);
	 LeaveCriticalSectionEXA();
	 return packetPtr;
}

//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CPacketObjList::GetNextPacket_safe(int * size,BYTE ** data,DPNID * id)
{
	EnterCriticalSectionEXA();
 	POSITION posLoc = GetHeadPosition();
    if(!posLoc)
	{
		LeaveCriticalSectionEXA();
	    return FALSE;
	}
	    CPacketObj * packet = (CPacketObj *)GetAt(posLoc);
		RemoveAt(posLoc);
		LeaveCriticalSectionEXA();
      
		*size = packet->m_size;
		if(packet->m_size > 0)
		{
		  *data = packet->m_data;
		  packet->m_data = NULL;
		}
		else
		{
          *data = NULL;
		}
		*id   = packet->m_communicationID;
        
	    delete packet;
	    packet=0;

	return TRUE;
}

int CPacketObjList::GetCountProtected()
{
	 int count = 0;
     EnterCriticalSectionEXA();
	 count = (int)GetCount();
	 LeaveCriticalSectionEXA();
	 return count;
}

void CPacketObjList::PurgeAllByID(DPNID id,BOOL logPurged)
{
	 POSITION posLast;
	 EnterCriticalSectionEXA();
 	 for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	 {
         CPacketObj * packetPtr = (CPacketObj *)GetNext(posLoc);
		 if(packetPtr->m_communicationID == id)
		 {
		    delete packetPtr;
		    packetPtr = 0;
		    RemoveAt(posLast);
		 }
	 }
	 LeaveCriticalSectionEXA();
}


CPacketObjList::~CPacketObjList()
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
		 CPacketObj * packetPtr = (CPacketObj *)GetNext(posLoc);
		 delete packetPtr;
		 packetPtr = 0;
	 }
	 RemoveAll();

     if(m_criticalSectionOption)
	 {
	    DeleteCriticalSection( m_criticalSectionOption);
	    delete m_criticalSectionOption;
		m_criticalSectionOption = 0;
	 } 
}

void CPacketObjList::Flush(int cnt)
{
	int trace = 0;
	POSITION posLast;
	for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	{
		CPacketObj * packetPtr = (CPacketObj *)GetNext(posLoc);
		if(packetPtr)
		{
			delete packetPtr;
			packetPtr = 0;
			RemoveAt(posLast);
			if(cnt > 0)
			   if(cnt == trace)
				   break;
		}
		trace++;
	}
}

BOOL CPacketObjList::EnableCriticalSectionUse()
{
	   if(m_criticalSectionOption)
	      return FALSE;
	
	
	   m_criticalSectionOption = new CRITICAL_SECTION;
	   InitializeCriticalSection(  m_criticalSectionOption );
	   m_criticalSectionEnabled = TRUE;
       return TRUE;
}

void CPacketObjList::EnterCriticalSectionEXA()
{
    if(m_criticalSectionEnabled)
	   EnterCriticalSection( m_criticalSectionOption);
}

void CPacketObjList::LeaveCriticalSectionEXA()
{
    if(m_criticalSectionEnabled)
	   LeaveCriticalSection( m_criticalSectionOption);
}
//-------------------------------------------------------------
int CPacketObjList::GetCountBaseOnChannel(int channel)
{
	 int trace = 0;
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
         CPacketObj * packetPtr = (CPacketObj *)GetNext(posLoc);
         if(packetPtr->m_channelID == channel)
		    trace++;
	 }
	 return trace;
}
//-------------------------------------------------------------
int CPacketObjList::GetDataSizeByChannel(int channel,int buffer)
{
	 int trace = 0;
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
         CPacketObj * packetPtr = (CPacketObj *)GetNext(posLoc);
         if(packetPtr->m_channelID == channel)
		    trace += (packetPtr->m_size+buffer);
	 }
	 return trace;
}
//---------------------------------------------------//
//---------------------------------------------------//
//---------------------------------------------------//
void CPacketObjList::SendToTextFile()
{
	EnterCriticalSectionEXA();
	CString fileName = "Packet_cluster_";

    CTime t = CTime::GetCurrentTime();
    CString s = t.Format( "%A, %H %M, %S" );
	fileName = operator+(fileName,s);
	fileName = operator+(fileName,".TXT");

     CFileException exc;
     CFile fl;
	 CArchive the_out_Archive(&fl,CArchive::store);
     BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
         CPacketObj * packetPtr = (CPacketObj *)GetNext(posLoc);
		 CString build = "ID:";
		 
		 CString id = ConvLongToString(packetPtr->m_communicationID);
		 CString size = ConvLongToString((DWORD)packetPtr->m_size);
	     LPEXMSG_CATAGORIE identityMsg = (LPEXMSG_CATAGORIE)packetPtr->m_data;
	     CString catagorieTXT = ConvLongToString((DWORD)identityMsg->m_catagorie);
			
         build = operator+(build,id);
		 build = operator+(build,"  size:");
		 build = operator+(build,size);
		 build = operator+(build,"  catagorie:");
		 build = operator+(build,catagorieTXT);
		 the_out_Archive.WriteString( build );  

	     the_out_Archive.WriteString( "\n");  
	 }
	 the_out_Archive.Close();
     fl.Close(); 
	 LeaveCriticalSectionEXA();
}
//---------------------------------------------------//
//---------------------------------------------------//
//---------------------------------------------------//
CString CPacketObjList::ConvToString(int number)
{    
	          CString temp;
			  temp.Format("%i",number);
			  return temp;
}
//---------------------------------------------------//
//---------------------------------------------------//
//---------------------------------------------------//
CString CPacketObjList::ConvLongToString(DWORD number)
{ 
CString temp;	 
char buffer[20];
_ultoa_s( number,buffer,20,10);
temp = buffer;
return temp;
}

IMPLEMENT_SERIAL(CPacketObj, CObject, 0)
IMPLEMENT_SERIAL(CPacketObjList, CObList, 0)

