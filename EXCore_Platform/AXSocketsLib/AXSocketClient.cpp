#include "StdAfx.h"
#include ".\axsocketclient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI AXsClientThread(LPVOID lpParameter);

BOOL CAXSocketClient::Connect(  char * ipAddress, 
								int port,
								char * username,
								char * password)
{ 
	// ERROR checking
	if ((strlen(username) > 49)
	|| (strlen(password) > 49))
	   return FALSE;

    m_clientLog.WriteDebugFile("BEGIN patcher connection attempt...\r\n",0,1);	

	// SETUP for response blocksize
	const int BUFFER_SIZE = 512;
	char buffTemp[BUFFER_SIZE];
	ZeroMemory(buffTemp, BUFFER_SIZE);

	// INIT Winsock stuff
	CString s;
	WSADATA wsaData;
	bool success = false;
	m_packetQue = new CAXSsPacketQue();	
    int wsaret = WSAStartup(0x101,&wsaData);
	if (wsaret)	
	{
		m_clientLog.WriteDebugFile("WSAStartup failed!\r\n",0,1);	
		return FALSE;
	}

	// TRY to send an authentication message to the server
	//	with a timeout of 10 seconds and 5 retry attempts if unsuccessful...
	const int MAX_RETRIES = 5;
	int iNumRetries = 0;
	while ((iNumRetries < MAX_RETRIES) && !success)
	{
		try
		{
			m_clientLog.WriteDebugFile("CREATE socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)...\r\n",0,1);	
			m_connection = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
			if (m_connection == INVALID_SOCKET)
			{
				m_clientLog.WriteDebugFile("CREATE socket FAILED!",0,1);	
				return FALSE;
			}

			struct sockaddr_in server;
			server.sin_addr.s_addr = inet_addr(ipAddress);
			server.sin_family = AF_INET;
			server.sin_port = htons((u_short)port);
			m_clientLog.WriteDebugFile("ATTEMPT to connect to server...",0,1);
			if (connect(m_connection,(struct sockaddr*)&server,sizeof(server)))
			{
				closesocket(m_connection);
				m_clientLog.WriteDebugFile("FAILED to connect to server @ IP: ",0,0);
				m_clientLog.WriteDebugFile(ipAddress,0,0);
				m_clientLog.WriteDebugFile("\r\n",0,1);
				return FALSE;	
			}
			m_clientLog.WriteDebugFile("SUCCESSFULLY connected to server @ IP: ",0,0);
			m_clientLog.WriteDebugFile(ipAddress,0,0);
			m_clientLog.WriteDebugFile("\r\n",0,1);

			// SET socket send and receive timeouts
			long timeout_in_ms = 10000;
			m_clientLog.WriteDebugFile("SET socket timeout: ",0,0);
			m_clientLog.WriteDebugFile(timeout_in_ms,0,0);
			m_clientLog.WriteDebugFile(" ms\r\n",0,1);
			setsockopt(m_connection, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout_in_ms, sizeof(timeout_in_ms));
			setsockopt(m_connection, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout_in_ms, sizeof(timeout_in_ms));

			// SEND authentication OR we will be timed out
			AuthPacket authPacket;
			ZeroMemory(&authPacket,sizeof(AuthPacket));
			CopyMemory(authPacket.username,username,min(strlen(username),ACCOUNT_BUFFER_MAX_SIZE));
			CopyMemory(authPacket.password,password,min(strlen(password),ACCOUNT_BUFFER_MAX_SIZE));
			
			int sendresult = 0;
			if(m_authEnabled)
			{
				sendresult = send(m_connection,(char*)&authPacket,sizeof(AuthPacket),0);
				if(this->GetError() != 0)
				{
					m_clientLog.WriteDebugFile("FAILED to authenticate account!\r\n",0,1);
					throw(1);
				}
			}

			// DEBUG message for authentication info
			m_clientLog.WriteDebugFile("SENT authentication...",0,0);
			m_clientLog.WriteDebugFile("\r\n\tUsername: ",0,0);
			m_clientLog.WriteDebugFile(username,0,0);
			m_clientLog.WriteDebugFile("\r\n\tPassword: ",0,0);
			m_clientLog.WriteDebugFile("[protected]",0,0);
			m_clientLog.WriteDebugFile("\r\n",0,1);
			
			// TRY to receive a response from the server 
			//	with a timeout of 10 seconds and 5 retry attempts if unsuccessful...
			int bytesRead = recv(m_connection,buffTemp,m_blockSize,0);
			if(this->GetError() != 0)
			{
				m_clientLog.WriteDebugFile("FAILED to receive a response from the server in a timely manner!",0,1);
				throw(1);
			}

			if(bytesRead < 1)
			{
				m_clientLog.WriteDebugFile("Connection FAILED!\r\n",0,1);
				return FALSE;
			}

			// SET success value to BREAK out of the loop...
			success = true;
		}
		catch(...)
		{
			// CLOSE socket and try again
			m_clientLog.WriteDebugFile("CLOSING socket connection...\r\n",0,1);
			if (m_connection)
				closesocket(m_connection);

			// INCREMENT number of retries
			++iNumRetries;
			m_clientLog.WriteDebugFile("Trying again. Attempt ",0,0);
			m_clientLog.WriteDebugFile(iNumRetries,0,0);
			m_clientLog.WriteDebugFile(" of ",0,0);
			m_clientLog.WriteDebugFile(MAX_RETRIES,0,0);
			m_clientLog.WriteDebugFile("...\r\n",0,1);

			// PAUSE for 5 seconds...
			Sleep(5000);
		}
	}

	if (!success)
	{
		m_clientLog.WriteDebugFile("AUTHORIZATION FAILED!\r\n",0,1);
		return FALSE;
	}

	// SET response block size
	if (buffTemp)
	{
		m_blockSize = atoi(buffTemp);
		m_clientLog.WriteDebugFile("Received response...",0,0);
		m_clientLog.WriteDebugFile(buffTemp,0,0);
		m_clientLog.WriteDebugFile("\r\n",0,1);
	}
	else
		m_blockSize = 0;

	m_clientLog.WriteDebugFile("Set Block Size: ",0,0);
	m_clientLog.WriteDebugFile(m_blockSize,0,1);
	if(m_blockSize < 1)
	   return FALSE;

    m_buff = new char[m_blockSize];
	ZeroMemory(m_buff,m_blockSize);

	m_connected = TRUE;
	m_threadHandle = CreateThread(NULL,0,AXsClientThread,this,0,&m_threadId);
	m_clientLog.WriteDebugFile("Looks good!",0,1);
	return TRUE;
}

/*BOOL CAXSocketClient::Connect(  char * ipAddress, 
								int port,
								char * username,
								char * password)
{ 
    m_clientLog.WriteDebugFile("attempting process",0,1);	

    if(strlen(username) > 49)
	   return FALSE;

	if(strlen(password) > 49)
	   return FALSE;

	m_packetQue = new CAXSsPacketQue();
	CString s;
	WSADATA wsaData;
	m_clientLog.WriteDebugFile("WSAStartup()",0,1);	
    int wsaret=WSAStartup(0x101,&wsaData);
	if(wsaret)	
		return FALSE;

	m_clientLog.WriteDebugFile("socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)",0,1);	
	m_connection=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
	if(m_connection==INVALID_SOCKET)
		return FALSE;

	//struct hostent *hp;
	//--->unsigned int addr;
	//char * servername = "127.0.0.1";
    /*CString ipChange;
	char * listDummy[20];
    if(inet_addr(ipAddress)==INADDR_NONE)
	{
		m_clientLog.WriteDebugFile("gethostbyname():",0,0);	
		m_clientLog.WriteDebugFile(ipAddress,0,1);
		hp=gethostbyname(ipAddress);
	}
	else
	{

		m_clientLog.WriteDebugFile("gethostbyaddr():",0,0);
		m_clientLog.WriteDebugFile(ipAddress,0,1);
		addr=inet_addr(ipAddress);
		hp=gethostbyaddr((char*)&addr,sizeof(addr),AF_INET);
        if(hp == NULL)
		{
           switch(WSAGetLastError())
		   {
		      case WSANOTINITIALISED:m_clientLog.WriteDebugFile("WSANOTINITIALISED",0,1);break;
			  case WSAENETDOWN:m_clientLog.WriteDebugFile("WSAENETDOWN",0,1);break;
			  case WSAHOST_NOT_FOUND:m_clientLog.WriteDebugFile("WSAHOST_NOT_FOUND",0,1);break;
			  case WSATRY_AGAIN:m_clientLog.WriteDebugFile("WSATRY_AGAIN",0,1);break;
			  case WSANO_RECOVERY:m_clientLog.WriteDebugFile("WSANO_RECOVERY",0,1);break;
			  case WSANO_DATA:m_clientLog.WriteDebugFile("WSANO_DATA",0,1);break;
			  case WSAEINPROGRESS:m_clientLog.WriteDebugFile("WSAEINPROGRESS",0,1);break;
			  case WSAEAFNOSUPPORT:m_clientLog.WriteDebugFile("WSAEAFNOSUPPORT",0,1);break;
			  case WSAEFAULT:m_clientLog.WriteDebugFile("WSAEFAULT",0,1);break;
			  case WSAEINTR:m_clientLog.WriteDebugFile("WSAEINTR",0,1);break;
		   }
		}
	}
	if(hp==NULL)
	{
		m_clientLog.WriteDebugFile("hp Failed",0,1);
		closesocket(m_connection);
		return FALSE;
	}* /

	//s.Format("hostname/ipaddress resolved");
    //addr=inet_addr(ipAddress);
	struct sockaddr_in server;
    server.sin_addr.s_addr=inet_addr(ipAddress);//*((unsigned long*)hp->h_addr);
	server.sin_family=AF_INET;
	server.sin_port=htons((u_short)port);//20248);
	m_clientLog.WriteDebugFile("attempting connect connect(m_connection,(struct sockaddr*)&server,sizeof(server)",0,1);
	if(connect(m_connection,(struct sockaddr*)&server,sizeof(server)))
	{
		closesocket(m_connection);
		m_clientLog.WriteDebugFile("Cannot connect to server:",0,0);
		m_clientLog.WriteDebugFile(ipAddress,0,1);
		return FALSE;	
	}
	m_clientLog.WriteDebugFile("Connected to server:",0,0);
	m_clientLog.WriteDebugFile(ipAddress,0,1);

	// SET socket send and receive timeouts
	long timeout_in_ms = 10000;
	setsockopt(m_connection, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout_in_ms, sizeof(timeout_in_ms));
	setsockopt(m_connection, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout_in_ms, sizeof(timeout_in_ms));

	// DEBUG message
	m_clientLog.WriteDebugFile("Set socket timeout: ",0,0);
	m_clientLog.WriteDebugFile(timeout_in_ms,0,0);
	m_clientLog.WriteDebugFile(" ms\r\n",0,1);

	//first msg sent is authentication or it will be timed out
	AuthPacket authPacket;
	ZeroMemory(&authPacket,sizeof(AuthPacket));
	CopyMemory(authPacket.username,username,min(strlen(username),ACCOUNT_BUFFER_MAX_SIZE));
	CopyMemory(authPacket.password,password,min(strlen(password),ACCOUNT_BUFFER_MAX_SIZE));

	// TRY to send an authentication message to the server
	//	with a timeout of 10 seconds and 5 retry attempts if unsuccessful...
	const int MAX_RETRIES = 5;
	int iNumRetries = 0;
	int sendresult = 0;
	if(m_authEnabled)
	{
		while (iNumRetries < MAX_RETRIES)
		{
			sendresult = send(m_connection,(char*)&authPacket,sizeof(AuthPacket),0);
			if(this->GetError() != 0)
			{
				++iNumRetries;
				m_clientLog.WriteDebugFile("Failed to authenticate! Trying again in 1 sec intervals (attempt ",0,0);
				m_clientLog.WriteDebugFile(iNumRetries,0,0);
				m_clientLog.WriteDebugFile(" of ",0,0);
				m_clientLog.WriteDebugFile(MAX_RETRIES,0,0);
				m_clientLog.WriteDebugFile(")...\r\n",0,0);
				Sleep(1000);
			}
			else break;
		}
	}

	// DEBUG message
	m_clientLog.WriteDebugFile("Sent authentication...",0,0);
	m_clientLog.WriteDebugFile("\r\n\tUsername: ",0,0);
	m_clientLog.WriteDebugFile(username,0,0);
	m_clientLog.WriteDebugFile("\r\n\tPassword: ",0,0);
	m_clientLog.WriteDebugFile("[protected]",0,0);
	m_clientLog.WriteDebugFile("\r\n",0,1);

	//wait for blocksize
	char buffTemp[512];
	ZeroMemory(buffTemp,512);
	
	// TRY to receive a response from the server 
	//	with a timeout of 10 seconds and 5 retry attempts if unsuccessful...
	iNumRetries = 0;
	int bytesRead = 0;
	while (iNumRetries < MAX_RETRIES)
	{
		bytesRead = recv(m_connection,buffTemp,m_blockSize,0);
		if(this->GetError() != 0)
		{
			++iNumRetries;
			m_clientLog.WriteDebugFile("No response to authentication attempt! Trying again in 1 sec intervals (attempt ",0,0);
			m_clientLog.WriteDebugFile(iNumRetries,0,0);
			m_clientLog.WriteDebugFile(" of ",0,0);
			m_clientLog.WriteDebugFile(MAX_RETRIES,0,0);
			m_clientLog.WriteDebugFile(")...\r\n",0,0);
			Sleep(1000);
		}
		else break;
	}

    if(bytesRead < 1)
	{
		m_clientLog.WriteDebugFile("Connection failed!\r\n",0,1);
		return FALSE;
	}

	// DEBUG message
	m_clientLog.WriteDebugFile("Received response...",0,0);
	m_clientLog.WriteDebugFile(buffTemp,0,0);
	m_clientLog.WriteDebugFile("\r\n",0,1);

	m_blockSize = atoi(buffTemp);
	m_clientLog.WriteDebugFile("Set Block Size: ",0,0);
	m_clientLog.WriteDebugFile(m_blockSize,0,1);
	if(m_blockSize < 1)
	   return FALSE;

    m_buff = new char[m_blockSize];
	ZeroMemory(m_buff,m_blockSize);

	m_connected = TRUE;
	m_threadHandle = CreateThread(NULL,0,AXsClientThread,this,0,&m_threadId);
	m_clientLog.WriteDebugFile("Looks good!",0,1);
	return TRUE;
}*/

CAXSocketClient::~CAXSocketClient(void)
{
	closesocket(m_connection);
	EnterCriticalSection(&m_interactSection);
    m_shutDown = TRUE;
	if(m_threadHandle)
	   CloseHandle(m_threadHandle);
	LeaveCriticalSection(&m_interactSection);
	EnterCriticalSection(&m_interactSection);
	while(m_threadCloseCount == 0)
	{
		LeaveCriticalSection(&m_interactSection);
		Sleep(0);
		EnterCriticalSection(&m_interactSection);
	}
	LeaveCriticalSection(&m_interactSection);
    
	
	if(m_packetQue)
	{
       delete m_packetQue; 
	   m_packetQue = 0;
	}

    
	

    if(m_buff)
	{
       delete [] m_buff;
	   m_buff = 0;
	}

	//LeaveCriticalSection(&m_interactSection);
	DeleteCriticalSection(&m_interactSection);
	CloseHandle(m_dataToProcessEvent);
	//WSACleanup();
}


bool CAXSocketClient::SendData(char * buff, int size)
{
	int bytesSent=send(m_connection,buff,size,0);
     if(bytesSent>0)
	    return true;

	 return false;
}


DWORD WINAPI AXsClientThread(LPVOID lpParameter)
{
	CAXSocketClient * classObj = (CAXSocketClient *)lpParameter;
 
    while(1)
	{
         EnterCriticalSection(&classObj->m_interactSection);
         if(classObj->m_shutDown)
		 {
		    classObj->m_threadCloseCount++;
            LeaveCriticalSection(&classObj->m_interactSection);
		    return 0;
		 }
		 LeaveCriticalSection(&classObj->m_interactSection);

         int bytesRead = recv(classObj->m_connection,classObj->m_buff,classObj->m_blockSize,0);
         if(bytesRead > 0)
		 {
            classObj->m_packetQue->AddPacket((BYTE*)classObj->m_buff,bytesRead,classObj->m_connection,-1);
            SetEvent(classObj->m_dataToProcessEvent);
			//classObj->m_clientLog.WriteDebugFile("Added packet()", 0, 1);
		 }
		 else
		 {//bad recive
            classObj->GetError();
		 }//end bad recieve
         Sleep(0);
	}
	return 0;
}

int 
CAXSocketClient::GetError()
{
	int iResult = 0;
	iResult = WSAGetLastError();
	switch(iResult)
	{
		case WSANOTINITIALISED:m_clientLog.WriteDebugFile("WSANOTINITIALISED",0,1);break;//A successful WSAStartup call must occur before using this function.
		case WSAENETDOWN:m_clientLog.WriteDebugFile("WSAENETDOWN",0,1);break;//The network subsystem has failed. 
		case WSAEFAULT:m_clientLog.WriteDebugFile("WSAEFAULT",0,1);break;//The buf parameter is not completely contained in a valid part of the user address space. 
		case WSAENOTCONN:m_clientLog.WriteDebugFile("WSAENOTCONN",0,1);break;//The socket is not connected. 
		case WSAEINTR:m_clientLog.WriteDebugFile("WSAEINTR",0,1);break;//The (blocking) call was canceled through WSACancelBlockingCall. 
		case WSAEINPROGRESS:m_clientLog.WriteDebugFile("WSAEINPROGRESS",0,1);break;//A blocking Windows Sockets 1.1 call is in progress, or the service provider is still processing a callback function. 
		case WSAENETRESET:m_clientLog.WriteDebugFile("WSAENETRESET",0,1);break;//The connection has been broken due to the keep-alive activity detecting a failure while the operation was in progress. 
		case WSAENOTSOCK:m_clientLog.WriteDebugFile("WSAENOTSOCK",0,1);break;//WSAENOTSOCK The descriptor is not a socket. 
		case WSAEOPNOTSUPP:m_clientLog.WriteDebugFile("WSAEOPNOTSUPP",0,1);break;//WSAEOPNOTSUPP MSG_OOB was specified, but the socket is not stream-style such as type SOCK_STREAM, OOB data is not supported in the communication domain associated with this socket, or the socket is unidirectional and supports only send operations. 
		case WSAESHUTDOWN:m_clientLog.WriteDebugFile("WSAESHUTDOWN",0,1);break;//WSAESHUTDOWN The socket has been shut down; it is not possible to receive on a socket after shutdown has been invoked with how set to SD_RECEIVE or SD_BOTH. 
		case WSAEWOULDBLOCK:m_clientLog.WriteDebugFile("WSAEWOULDBLOCK",0,1);break;//The socket is marked as nonblocking and the receive operation would block. 
		case WSAEMSGSIZE:m_clientLog.WriteDebugFile("WSAEMSGSIZE",0,1);break;//The message was too large to fit into the specified buffer and was truncated. 
		case WSAEINVAL:m_clientLog.WriteDebugFile("WSAEINVAL",0,1);break;//The socket has not been bound with bind, or an unknown flag was specified, or MSG_OOB was specified for a socket with SO_OOBINLINE enabled or (for byte stream sockets only) len was zero or negative. 
		case WSAECONNABORTED:m_clientLog.WriteDebugFile("WSAECONNABORTED",0,1);break;//The virtual circuit was terminated due to a time-out or other failure. The application should close the socket as it is no longer usable. 
		case WSAETIMEDOUT:m_clientLog.WriteDebugFile("WSAETIMEDOUT",0,1);break;//The connection has been dropped because of a network failure or because the peer system failed to respond.
		case WSAECONNRESET:m_clientLog.WriteDebugFile("WSAECONNRESET",0,1);break;//The virtual circuit  
		default: iResult = 0; break;
	}
	return iResult;
}