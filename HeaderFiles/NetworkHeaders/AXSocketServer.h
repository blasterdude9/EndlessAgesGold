#include <winsock2.h>
#include "axpacketclass.h"
#pragma once


struct cnPack
{
   char cat;
   char ip[56];
   long socket;
};


class CAXSvConnection : public CObject
{
    public:
	CAXSvConnection()
	{
	   m_inUse = 0;
	   m_authenticated = 0;
	   m_kbSent = 0;
	   m_kbsTimerLastKbAmount = 0;
	   m_miscLong = 0;
	   m_miscLong2 = 0;
	   m_miscDataRef = NULL;
	   m_lastMsg = GetTickCount();
	   m_kbsTimerStamp = GetTickCount();
	   m_sendAttempTimeStamp = GetTickCount();
	   m_miscTimeStamp = m_kbsTimerStamp;
	   m_streaming = false;
	   m_sending = false;
	}
    ~CAXSvConnection()
	{
		   closesocket(m_client);
	}

    //vars
    SOCKET      m_client;
	//WSAEVENT    m_event;
    sockaddr_in m_from;
	long        m_kbs;
	//DWORD       m_lastMsgStamp;
	DWORD       m_kbsTimerStamp;
	DWORD       m_miscTimeStamp;
	long        m_kbsTimerLastKbAmount;
	DWORD       m_lastMsg;
	DWORD       m_lastMsgRecieved;
	DWORD       m_connectTime;
	DWORD       m_sendAttempTimeStamp;
    bool        m_inUse;
	int         m_connectionIndex;
	int         m_authenticated;
	AuthPacket  m_authPacketRef;
	long        m_kbSent;
	long        m_miscLong;
	long        m_miscLong2;
	void       *m_miscDataRef;
	bool        m_streaming;
	bool        m_sending;
	


	//functions
	int SendData(BYTE * data, int dataLen)
	{
		DWORD currentTime = GetTickCount();
		m_kbSent += dataLen;
		m_lastMsg = currentTime;
        
        if(currentTime-m_kbsTimerStamp > 1000)
		{
           m_kbs = (m_kbSent-m_kbsTimerLastKbAmount)/1000;

		   m_kbsTimerLastKbAmount = m_kbSent;
           m_kbsTimerStamp = currentTime;
		}
		return send(m_client,(char*)data,dataLen,0);
	};

};

struct CAxConnholder
{
   CAXSvConnection * connection;
};

class CAXSocketServer
{
public:
	CAXSocketServer(void);
	BOOL StartServer(int threadCount, int port, int blockSize, int maxConnections,BOOL authEnabled,CString listenOnSpecificIP);
	~CAXSocketServer(void);


	BOOL					  m_serverInitialized;
	int						  m_threadCount;
	DWORD					* m_threadIds;
	HANDLE					* m_threadHandles;
	DWORD					* m_eventThreadIds;
	HANDLE					* m_eventThreadHandles;
	int                       m_port;
	int                       m_maxSimutaniusIps;
	//WSAEVENT                * m_events;
	int                       m_eventTrace;
	BOOL                      m_logging;
	DWORD                     m_maxConnections;
	CRITICAL_SECTION          m_connectionSection;
	CRITICAL_SECTION          m_writeDebugSection;
	CRITICAL_SECTION          m_interactSection;
	int                       m_blockSize;
	char                      m_blockSizeString[67];
	BOOL                      m_shutDown;
	HANDLE                    m_dataToProcessEvent;
	BOOL                      m_authenticationEnabled;
	int                       m_threadCreatedTracker;
	DWORD					  m_fastestSendSpeed;


	int                       m_eventMemoryShift;
	int                       m_eventShiftAmount;
	int                       m_threadsPerShift;
	int                       m_threadsMinorTrace;
	int                       m_logonThreadCount;

	SOCKET					  m_server;

	CAxConnholder           * m_connectionArray;
	WSAEVENT                * m_eventArray;

	//CAXSvConnectionList		* m_connections;
	CAXSsPacketQue          * m_packetQue;

	//function
	void WriteDebugFile(LPSTR debugString,BOOL clearIt,BOOL returnCase);
	void WriteDebugFile(long debugString,BOOL clearIt,BOOL returnCase);
	void ConvertToString(long stringTemp, char * buff);
	BOOL SendDataToID(int connectionID, BYTE * data, int length);
 
	CAXSvConnection * GetUnusedConnection()
	{
	   DWORD curTime = GetTickCount();
       for(int loop=0;(unsigned)loop<m_maxConnections;loop++)
	   {
		   EnterCriticalSection(&m_connectionSection);

		   if(m_connectionArray[loop].connection->m_inUse == 1)
           if((curTime-m_connectionArray[loop].connection->m_connectTime) > 60000 && m_connectionArray[loop].connection->m_authenticated == 0)
		   {//time out pending connection
              WriteDebugFile("**Remove Connection that never authenticated**",0,0);
			  WriteDebugFile(inet_ntoa(m_connectionArray[loop].connection->m_from.sin_addr),0,1);
			  LeaveCriticalSection(&m_connectionSection);
			  MakeConnectionAvailable(loop);
			  EnterCriticalSection(&m_connectionSection);
		   }//end timeout pending connection

		   if(m_connectionArray[loop].connection->m_inUse == 1)
           if((curTime-m_connectionArray[loop].connection->m_lastMsg) > 600000)
		   {//time out socket communications
              WriteDebugFile("**SOCKET-TIMEOUT**",0,0);
			  WriteDebugFile(inet_ntoa(m_connectionArray[loop].connection->m_from.sin_addr),0,1);
			  LeaveCriticalSection(&m_connectionSection);
			  MakeConnectionAvailable(loop);
			  EnterCriticalSection(&m_connectionSection);
		   }//end time out socket communications


           if(m_connectionArray[loop].connection->m_inUse == 0)
		   {
              m_connectionArray[loop].connection->m_lastMsg = GetTickCount();
		      m_connectionArray[loop].connection->m_connectTime = GetTickCount();
              m_connectionArray[loop].connection->m_inUse = 1;
              LeaveCriticalSection(&m_connectionSection);

			  return m_connectionArray[loop].connection;
		   }
		   LeaveCriticalSection(&m_connectionSection);
	   }
       return NULL;
	}

	bool GetDataFromConnectionID(int connectionID,CString & tempIp,SOCKET & tempSock,int & connectionIndex,long & kbs,CAXSvConnection ** ptrRef)
	{
		 if(connectionID < 0)
		    return false;

		 if((unsigned)connectionID >= m_maxConnections)
		    return false;

         EnterCriticalSection(&m_connectionSection);
	     tempIp = inet_ntoa(m_connectionArray[connectionID].connection->m_from.sin_addr);
         tempSock = m_connectionArray[connectionID].connection->m_client;
		 connectionIndex = m_connectionArray[connectionID].connection->m_connectionIndex;
		 kbs = m_connectionArray[connectionID].connection->m_kbs;
		 if(ptrRef)
		    *ptrRef = m_connectionArray[connectionID].connection;
		 LeaveCriticalSection(&m_connectionSection);

		 return true;
	}

	CString GetIPFromConnectionID(int connectionID)
	{
		CString ipBuild = "";
		 if(connectionID < 0)
		    return ipBuild;

		 if((unsigned)connectionID >= m_maxConnections)
		    return ipBuild;

         EnterCriticalSection(&m_connectionSection);
	     ipBuild = inet_ntoa(m_connectionArray[connectionID].connection->m_from.sin_addr);
		 LeaveCriticalSection(&m_connectionSection);

		 return ipBuild;
	}

	bool Authenticate(int index, AuthPacket * authPacketRef, CString & tempIp, SOCKET & tempSock)
	{
         if(index < 0)
		    return false;

		 if((unsigned)index >= m_maxConnections)
		    return false;

         EnterCriticalSection(&m_connectionSection);
		 CopyMemory(&m_connectionArray[index].connection->m_authPacketRef,authPacketRef,sizeof(AuthPacket));
      
		 m_connectionArray[index].connection->m_lastMsg = GetTickCount();
		 m_connectionArray[index].connection->m_streaming = false;
	     m_connectionArray[index].connection->m_sending = false;
         m_connectionArray[index].connection->m_authenticated = 1;
		 m_connectionArray[index].connection->m_kbSent = 0;
		 m_connectionArray[index].connection->m_kbsTimerLastKbAmount = 0;
		 tempIp = inet_ntoa(m_connectionArray[index].connection->m_from.sin_addr);
		 tempSock = m_connectionArray[index].connection->m_client;
		 LeaveCriticalSection(&m_connectionSection);
		 return true;
	}

    int IsAuthenticated(int index)
	{
		int res = 0;
		if(index < 0)
		   return res;
        
        EnterCriticalSection(&m_connectionSection);

        res = m_connectionArray[index].connection->m_authenticated;

		LeaveCriticalSection(&m_connectionSection);
		return res;
	}

	bool ReadyForMsg(int & index)
	{
		if(index < 0)
		   return false;
        
        EnterCriticalSection(&m_connectionSection);

        if(GetTickCount()-m_connectionArray[index].connection->m_sendAttempTimeStamp >= m_fastestSendSpeed)
		{
            LeaveCriticalSection(&m_connectionSection);
			return true;
		}
		LeaveCriticalSection(&m_connectionSection);
		return false;
	}

	bool MakeConnectionAvailable(int index)
	{
       if(index < 0)
		    return false;

	   EnterCriticalSection(&m_connectionSection);
       cnPack dcPost;
	   dcPost.cat = 22;
	   dcPost.socket = (long)m_connectionArray[index].connection->m_client;
	   CString tempIp = inet_ntoa(m_connectionArray[index].connection->m_from.sin_addr);
	   ZeroMemory(dcPost.ip,56);
       if(tempIp.GetLength() > 0 && tempIp.GetLength() < 56)
	   {
	      CopyMemory(dcPost.ip,tempIp.GetBuffer(),tempIp.GetLength());
          m_packetQue->AddPacket((BYTE*)&dcPost,sizeof(cnPack),m_connectionArray[index].connection->m_client,index);
	   }
	   
	   m_connectionArray[index].connection->m_inUse = 0;
	   m_connectionArray[index].connection->m_streaming = false;
       m_connectionArray[index].connection->m_sending = false;
	   m_connectionArray[index].connection->m_authenticated = 0;
       closesocket(m_connectionArray[index].connection->m_client);
	   
       LeaveCriticalSection(&m_connectionSection);
       return NULL;
	}


};

