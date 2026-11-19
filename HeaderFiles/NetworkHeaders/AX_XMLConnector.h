#pragma once
#include "AXSocketServer.h"
#include "axpacketclass.h"
#include "ax_xmlcommandque.h"
#include "axsocketclient.h"

DWORD WINAPI CAX_XMLServerPacketProc(LPVOID lpParameter);

class CAX_XMLConnector
{
public:
	//constructors
	CAX_XMLConnector(void){InitializeCriticalSection(&m_section);m_readyForMessages = FALSE;};
	CAX_XMLConnector(int port, int blockSize, int threadCount, int maxConnections)
	{
		m_threadsClosed = 0;
		m_type = 0;
		m_clientSocket = NULL;
		InitializeCriticalSection(&m_section);
		m_msgBuilder = new CAX_XMLCommandQueList();
		m_shutDown = FALSE;
		m_threadCount = threadCount;
        m_sServer = new CAXSocketServer();
		m_readyForMessages = FALSE;
	    m_readyForMessages = m_sServer->StartServer(3,port,blockSize,maxConnections,FALSE,"");
		m_procThreadHandle = CreateThread(NULL,0,CAX_XMLServerPacketProc,this,0,&m_threadID);
	}
	CAX_XMLConnector(int port)
	{
		m_threadsClosed = 0;
		m_type = 0;
		m_clientSocket = NULL;
		InitializeCriticalSection(&m_section);
		m_msgBuilder = new CAX_XMLCommandQueList();
		m_threadCount = 1;
		m_shutDown = FALSE;
        m_sServer = new CAXSocketServer();
		m_readyForMessages = FALSE;
	    m_readyForMessages = m_sServer->StartServer(3,port,16384,20,FALSE,"");
		m_procThreadHandle = CreateThread(NULL,0,CAX_XMLServerPacketProc,this,0,&m_threadID);
	}
	CAX_XMLConnector(int port, CString ip)
	{
		m_threadsClosed = 0;
		m_type = 1;
		InitializeCriticalSection(&m_section);
		m_msgBuilder = new CAX_XMLCommandQueList();
		m_threadCount = 0;
		m_shutDown = FALSE;
        m_sServer = NULL;
		m_readyForMessages = FALSE;
		m_clientSocket = new CAXSocketClient(FALSE);
	    int success    = m_clientSocket->Connect(    ip.GetBuffer(),
												     port,
												     "GUEST",
											         "PASS");
		if(success)
		{
		   m_threadCount = 1;
		   m_readyForMessages = TRUE;
		   m_procThreadHandle = CreateThread(NULL,0,CAX_XMLServerPacketProc,this,0,&m_threadID);
		}
	}
	~CAX_XMLConnector(void)
	{
		EnterCriticalSection(&m_section);
        m_shutDown = TRUE;
        LeaveCriticalSection(&m_section);
        
		EnterCriticalSection(&m_section);
		while(m_threadsClosed != m_threadCount)
		{
            LeaveCriticalSection(&m_section);
            Sleep(100);
			EnterCriticalSection(&m_section);
		}
		LeaveCriticalSection(&m_section);

        DeleteCriticalSection(&m_section);
		if(m_sServer)
		   delete m_sServer;
		m_sServer = 0;
		if(m_clientSocket)
		   delete m_clientSocket;
		m_clientSocket=0;
		if(m_msgBuilder)
		   delete m_msgBuilder;
		m_msgBuilder = 0;
	}

	BOOL SendMessage(BYTE * data, long lengthdata, bool includeHeader)
	{
         int sentProgress = 0;
		 int currentSendAmount = 0;
         long maxBufSize = 0;
		 long remainder = 0;
		 CAX_XMLheader header;
		 header.catagorie = 0;
		 header.length = lengthdata;
		 if(m_type == 0)
		 {
           //throw header first
		   if(includeHeader)
		      currentSendAmount = m_sServer->m_connectionArray[0].connection->SendData((BYTE*)&header,sizeof(CAX_XMLheader));

		   while(sentProgress < lengthdata)
		   {//server to client send

                maxBufSize = m_sServer->m_blockSize;// - sizeof(Genheader);
				
				remainder = lengthdata - sentProgress;
				if(remainder < maxBufSize)
					maxBufSize = remainder;


                currentSendAmount = m_sServer->m_connectionArray[0].connection->SendData((BYTE*)&data[sentProgress],maxBufSize);
                if(currentSendAmount > -1)
				   sentProgress += currentSendAmount;
				else
				{
				    switch(WSAGetLastError())
					{
						case WSANOTINITIALISED:m_sServer->WriteDebugFile("WSANOTINITIALISED",0,1);break;
						case WSAENETDOWN:m_sServer->WriteDebugFile("WSAENETDOWN",0,1);break;
						case WSAEACCES:m_sServer->WriteDebugFile("WSAEACCES",0,1);break;
						case WSAEINPROGRESS:m_sServer->WriteDebugFile("WSAEINPROGRESS",0,1);break;
						case WSAEFAULT:m_sServer->WriteDebugFile("WSAEFAULT",0,1);break;
						case WSAENETRESET:m_sServer->WriteDebugFile("WSAENETRESET",0,1);break;
						case WSAENOBUFS:m_sServer->WriteDebugFile("WSAENOBUFS",0,1);break;
						case WSAENOTCONN:m_sServer->WriteDebugFile("WSAENOTCONN",0,1);break;
						case WSAENOTSOCK:m_sServer->WriteDebugFile("WSAENOTSOCK",0,1);break;
						case WSAEOPNOTSUPP:m_sServer->WriteDebugFile("WSAEOPNOTSUPP",0,1);break;
				        case WSAESHUTDOWN:m_sServer->WriteDebugFile("WSAESHUTDOWN",0,1);break;
						case WSAEWOULDBLOCK:m_sServer->WriteDebugFile("WSAEWOULDBLOCK",0,1);break;
						case WSAEMSGSIZE:m_sServer->WriteDebugFile("WSAEMSGSIZE",0,1);break;
						case WSAEINVAL:m_sServer->WriteDebugFile("WSAEINVAL",0,1);break;
						case WSAECONNABORTED:m_sServer->WriteDebugFile("WSAECONNABORTED",0,1);break;
						case WSAECONNRESET:m_sServer->WriteDebugFile("WSAECONNRESET",0,1);break;
					}

                   return FALSE;
				}
		   }//end server to client send
		 }
		 
         if(m_type == 1)

           //throw header first
		   if(includeHeader)
		      currentSendAmount = send(m_clientSocket->m_connection,(char*)&header,sizeof(CAX_XMLheader),0);

		   while(sentProgress < lengthdata)
		   {//client to client send
                maxBufSize = m_clientSocket->m_blockSize;// - sizeof(Genheader);
				
				remainder = lengthdata - sentProgress;
				if(remainder < maxBufSize)
					maxBufSize = remainder;


			    currentSendAmount = send(m_clientSocket->m_connection,(char*)&data[sentProgress],maxBufSize,0);
				if(currentSendAmount > -1)
				   sentProgress += currentSendAmount;
				else
				{
                   return FALSE;
				}
		   }//end client to client send

         return TRUE;
	}

	//Vars
	int                               m_type;//0=server 1=client
	BOOL                              m_readyForMessages;
	CAXSocketServer					* m_sServer;
	CAXSocketClient                 * m_clientSocket;
	int                               m_threadCount;
	int                               m_threadsClosed;
	CRITICAL_SECTION                  m_section;
	DWORD                             m_threadID;
	HANDLE                            m_procThreadHandle;
	BOOL                              m_shutDown;
	CAX_XMLCommandQueList           * m_msgBuilder;
	//requests limited to 16384
};
//thread functions
DWORD WINAPI CAX_XMLServerPacketProc(LPVOID lpParameter)
{
    CAX_XMLConnector * classObj = (CAX_XMLConnector *)lpParameter;
    CAXSsPacket * packetObj = NULL;

	if(classObj->m_type == 0)
	{
	  while(1)
	  {//server block
        EnterCriticalSection(&classObj->m_section);
        if(classObj->m_shutDown)
		{
		   classObj->m_threadsClosed++;
           LeaveCriticalSection(&classObj->m_section);
           break;
		}
        LeaveCriticalSection(&classObj->m_section);


		WaitForSingleObject(classObj->m_sServer->m_dataToProcessEvent,1000);
        //get all packets and process them
		while(classObj->m_sServer->m_packetQue->GetNextPacket(&packetObj)==true)
		{
              classObj->m_msgBuilder->AppendDataX(packetObj->data,packetObj->length);//crashes
              delete packetObj;
			  packetObj=0;
		}
        ResetEvent(classObj->m_sServer->m_dataToProcessEvent);
		//get completed messages
	   }
	}//end server block

    if(classObj->m_type == 1)
	{//client block
	  while(1)
	  {
	    EnterCriticalSection(&classObj->m_section);
        if(classObj->m_shutDown)
		{
		   classObj->m_threadsClosed++;
           LeaveCriticalSection(&classObj->m_section);
           break;
		}
        LeaveCriticalSection(&classObj->m_section);
		WaitForSingleObject(classObj->m_clientSocket->m_dataToProcessEvent,1000);
        //get all packets and process them
		while(classObj->m_clientSocket->m_packetQue->GetNextPacket(&packetObj)==true)
		{
              classObj->m_msgBuilder->AppendDataX(packetObj->data,packetObj->length);
              delete packetObj;
			  packetObj=0;
		}
        ResetEvent(classObj->m_clientSocket->m_dataToProcessEvent);
		//get completed messages
	   }
	}//end client block

	return 0;
}


