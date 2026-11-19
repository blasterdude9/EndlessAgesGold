
#include <winsock2.h>
#include "axpacketclass.h"
#include "loggingclass.h"


#pragma once

class CAXSocketClient
{
public:
	CAXSocketClient(void)
	{
	   m_clientLog.SetDetails(1,"AXClientLogSC.txt");
	   m_packetQue = NULL;
	   m_authEnabled = TRUE;
	   m_connected = FALSE;
	   m_threadId = 0;
	   m_threadHandle = NULL;
	   m_shutDown = FALSE;
	   m_buff = NULL;
	   m_blockSize = 512;
	   m_threadCloseCount = 0;
	   m_authenticated = 0;
	   InitializeCriticalSection(&m_interactSection);

	   m_dataToProcessEvent = CreateEvent( 
										NULL,         // no security attributes
										TRUE,         // manual-reset event
										TRUE,          // initial state is signaled
										"AXClientProcess"  // object name
										); 
	   m_clientLog.WriteDebugFile("Initialized Contructor", 1, 1);
	};
	CAXSocketClient(BOOL authEnabled)
	{
	   m_clientLog.SetDetails(1,"AXClientLogSC.txt");
	   m_packetQue = NULL;
	   m_authEnabled = authEnabled;
	   m_connected = FALSE;
	   m_threadId = 0;
	   m_threadHandle = NULL;
	   m_shutDown = FALSE;
	   m_buff = NULL;
	   m_blockSize = 512;
	   m_threadCloseCount = 0;
	   m_authenticated = 0;
	   InitializeCriticalSection(&m_interactSection);

	   m_dataToProcessEvent = CreateEvent( 
										NULL,         // no security attributes
										TRUE,         // manual-reset event
										TRUE,          // initial state is signaled
										"AXClientProcess"  // object name
										); 
	   m_clientLog.WriteDebugFile("Initialized Contructor", 1, 1);
	};
	BOOL Connect(char * ipAddress, int port,char * username,char * password);
	~CAXSocketClient(void);


	//Vars
	CLoggingClass                 m_clientLog;
	SOCKET						  m_connection;
	CAXSsPacketQue				* m_packetQue;
	BOOL                          m_connected;
	DWORD					      m_threadId;
	HANDLE					      m_threadHandle;
	int                           m_blockSize;
	char                        * m_buff;
	BOOL                          m_shutDown;
	CRITICAL_SECTION              m_interactSection;
	int                           m_threadCloseCount;
	HANDLE                        m_dataToProcessEvent;
	int                           m_authenticated;
	BOOL                          m_authEnabled;

	bool SendData(char * buff, int size);
	int GetError();
	
};
