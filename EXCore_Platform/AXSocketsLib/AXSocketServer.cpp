#include "StdAfx.h"
#include ".\axsocketserver.h"
#include <fstream>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI ServerThread(LPVOID lpParameter);
DWORD WINAPI ConnectionManagement(LPVOID lpParameter);

CAXSocketServer::CAXSocketServer()
{
    m_threadIds				= NULL;
	m_threadHandles			= NULL;
	m_eventThreadHandles    = NULL;
	m_eventThreadIds		= NULL;
	m_packetQue             = NULL;
	m_connectionArray       = NULL;
	m_eventArray            = NULL;
	m_shutDown              = FALSE;
	m_eventMemoryShift      = 0;
	m_threadsMinorTrace     = 0;
	m_threadCreatedTracker  = 0;
	m_eventShiftAmount      = 20;
	m_logonThreadCount      = 3;
	m_authenticationEnabled = TRUE;
	m_fastestSendSpeed      = 0;

	
	InitializeCriticalSection(&m_interactSection);
	InitializeCriticalSection(&m_connectionSection);
	InitializeCriticalSection(&m_writeDebugSection);

	m_dataToProcessEvent = CreateEvent( 
										NULL,         // no security attributes
										TRUE,         // manual-reset event
										TRUE,          // initial state is signaled
										"ServerProcess"  // object name
										); 
}

BOOL CAXSocketServer::StartServer(int threadCount, 
								  int port, 
								  int blockSize,
								  int maxConnections,
								  BOOL authEnabled,
								  CString listenOnSpecificIP)
{
	
	WSADATA wsaData;
	int wsaret=WSAStartup(0x101,&wsaData);
	
	m_authenticationEnabled = authEnabled;
    m_serverInitialized		= FALSE;
	m_blockSize             = blockSize;
	ZeroMemory(m_blockSizeString,67);
	ConvertToString((long)m_blockSize, m_blockSizeString);
	m_maxSimutaniusIps      = 5;
	m_maxConnections        = maxConnections;
	//m_threadCount			= threadCount;
	m_eventTrace			= 0;
	m_threadCreatedTracker  = 0;
	m_logging               = TRUE;
	m_port                  = port;
	m_eventShiftAmount      = WSA_MAXIMUM_WAIT_EVENTS;
   
	WriteDebugFile("Server Initializing",1,1);
	//even out allowed connections
    m_maxConnections = ((m_maxConnections/m_eventShiftAmount)+1)*WSA_MAXIMUM_WAIT_EVENTS;
    WriteDebugFile("Max Connections Rounded up:",0,0);
	WriteDebugFile(m_maxConnections,0,1);


    WriteDebugFile("Max Wait Events:",0,0);
	WriteDebugFile(WSA_MAXIMUM_WAIT_EVENTS,0,1);

	if(m_eventShiftAmount > 100)
       m_eventShiftAmount = 100;

	if(m_maxConnections < (unsigned)m_eventShiftAmount)
	   m_maxConnections = m_eventShiftAmount;

	WriteDebugFile("Final Max Connections:",0,0);
	WriteDebugFile(m_maxConnections,0,1);

	m_threadsPerShift		= threadCount;
	m_threadCount		    = (m_maxConnections / m_eventShiftAmount)*m_threadsPerShift;

	WriteDebugFile("Threads per tcp section:",0,0);
	WriteDebugFile(m_threadsPerShift,0,1);
	WriteDebugFile("Final Thread Count for sockets backend:",0,0);
	WriteDebugFile(m_threadCount,0,1);


	

	m_threadIds				= new DWORD[m_logonThreadCount];
	m_threadHandles			= new HANDLE[m_logonThreadCount];
	m_eventThreadHandles    = new HANDLE[m_threadCount];
	m_eventThreadIds		= new DWORD[m_threadCount];
	m_packetQue             = new CAXSsPacketQue();
	m_connectionArray		= new CAxConnholder[m_maxConnections];
	m_eventArray			= new WSAEVENT[m_maxConnections];

	for(int conLoopInit = 0;(unsigned)conLoopInit<m_maxConnections;conLoopInit++)
	{
		m_connectionArray[conLoopInit].connection = new CAXSvConnection();
		//ZeroMemory(m_connectionArray[conLoopInit].connection,sizeof(CAXSvConnection));
        m_connectionArray[conLoopInit].connection->m_connectionIndex = conLoopInit;
		m_eventArray[conLoopInit] = WSACreateEvent();
		if(WSA_INVALID_EVENT == m_eventArray[conLoopInit])
		   WriteDebugFile("Could not create event handle WSACreateEvent()",0,1);
		
	}


    //create server
    //WSADATA is a struct that is filled up by the call 
    //to WSAStartup
    //WSADATA wsaData;

    //The sockaddr_in specifies the address of the socket
    //for TCP/IP sockets. Other protocols use similar structures.
    sockaddr_in local;

    //WSAStartup initializes the program for calling WinSock.
    //The first parameter specifies the highest version of the 
    //WinSock specification, the program is allowed to use.
    //int wsaret=WSAStartup(0x101,&wsaData);

    //WSAStartup returns zero on success.
    //If it fails we exit.
    if(wsaret!=0)
    {
        return 0;
    }

    //Now we populate the sockaddr_in structure
    local.sin_family=AF_INET; //Address family
    
	if(listenOnSpecificIP.GetLength() < 2)
	   local.sin_addr.s_addr=INADDR_ANY; //Wild card IP address
	else
       local.sin_addr.s_addr=inet_addr(listenOnSpecificIP.GetBuffer()); //Wild card IP address
	
	local.sin_port=htons((u_short)m_port); //port to use

    //the socket function creates our SOCKET
    m_server=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    //m_server=socket(AF_INET,SOCK_STREAM,SOL_SOCKET);

    //If the socket() function fails we exit
    if(m_server==INVALID_SOCKET)
    {
        return 0;
    }

	



	
	/*char newValue = 0;
	if(setsockopt(m_server,
					IPPROTO_TCP,
					SO_SNDBUF,
					&newValue,
					sizeof(char))!= 0)
	{//failed
        WriteDebugFile("SETSOCK Failed SO_SNDBUF",0,1);
	}//end failed*/

	//set options
	//set socket options
	
	/*char newValue = 1;
	if(setsockopt(m_server,
					IPPROTO_TCP,
					TCP_NODELAY,
					&newValue,
					sizeof(char))!= 0)
	{//failed
        WriteDebugFile("SETSOCK Failed",0,1);
	}//end failed*/

	/*int newSendBufferSize = 0;
	if(setsockopt(m_server,
				          SOL_SOCKET,
				          SO_SNDBUF,
				          (char*)&newSendBufferSize,
				          sizeof(int)) != 0)
	{//failed
        WriteDebugFile("setsockopt SO_SNDBUF",0,1);
	}//end failed*/
	 


	

    



    //bind links the socket we just created with the sockaddr_in 
    //structure. Basically it connects the socket with 
    //the local address and a specified port.
    //If it returns non-zero quit, as this indicates error
    if(bind(m_server,(sockaddr*)&local,sizeof(local))!=0)
    {
        return 0;
    }

	

    //set socket to non-blocking
    /*u_long NonBlock = 1;
    if (ioctlsocket(m_server, FIONBIO, &NonBlock) == SOCKET_ERROR)
    {
		WriteDebugFile("Could not make non-blocking ioctlsocket()",0,1);
		return 0;
    }*/

    //listen instructs the socket to listen for incoming 
    //connections from clients. The second arg is the backlog
    if(listen(m_server,10)!=0)
    {
        return 0;
    }

    int tLoop=0;
	for(tLoop=0;tLoop<m_logonThreadCount;tLoop++)
	{
	    m_threadHandles[tLoop] = CreateThread(NULL,0,ServerThread,this,0,&m_threadIds[tLoop]);
		
	}

	for(tLoop=0;tLoop<m_threadCount;tLoop++)
	{
	    m_eventThreadHandles[tLoop] = CreateThread(NULL,0,ConnectionManagement,this,0,&m_eventThreadIds[tLoop]);
	}


	m_serverInitialized = TRUE;
	WriteDebugFile("Server Started",0,1);
	return TRUE;
}

BOOL CAXSocketServer::SendDataToID(int connectionID, BYTE * data, int length)
{ 
	CAXSvConnection * conPtr = m_connectionArray[connectionID].connection;
	EnterCriticalSection(&m_connectionSection);
    if(conPtr->m_inUse == false)
	{
       LeaveCriticalSection(&m_connectionSection);
       return FALSE;
	}
	LeaveCriticalSection(&m_connectionSection);
	if(conPtr->SendData(data,length)>0)
       return TRUE;

	return FALSE;
}

CAXSocketServer::~CAXSocketServer(void)
{
    EnterCriticalSection(&m_interactSection);
    m_shutDown = TRUE;
	LeaveCriticalSection(&m_interactSection);
	closesocket(m_server);
	WSACleanup( );
	EnterCriticalSection(&m_interactSection);
    while(m_threadCreatedTracker > 0)
	{ 
       LeaveCriticalSection(&m_interactSection);
       Sleep(200);
	   EnterCriticalSection(&m_interactSection);
	}
	LeaveCriticalSection(&m_interactSection);
	//close threads
	if(m_threadHandles)
	for(int tLoop=0;tLoop<m_logonThreadCount;tLoop++)
	{
	    CloseHandle(m_threadHandles[tLoop]);
	}
	if(m_eventThreadHandles)
	for(int tLoop=0;tLoop<m_threadCount;tLoop++)
	{
	    CloseHandle(m_eventThreadHandles[tLoop]);
	}

	if(m_threadIds)
	   delete [] m_threadIds;
	m_threadIds = 0;

	if(m_eventThreadIds)
	   delete [] m_eventThreadIds;
	m_eventThreadIds = 0;

	if(m_threadHandles)
	   delete [] m_threadHandles;
	m_threadHandles = 0;

	if(m_eventThreadHandles)
	   delete [] m_eventThreadHandles;
	m_eventThreadHandles = 0;

	if(m_packetQue)
	{
       delete m_packetQue;
	   m_packetQue = 0;
	}

	if(m_connectionArray)
	for(int conLoopInit = 0;(unsigned)conLoopInit<m_maxConnections;conLoopInit++)
	{
		if(m_connectionArray[conLoopInit].connection)
		   delete m_connectionArray[conLoopInit].connection;
        m_connectionArray[conLoopInit].connection = 0;
	}

	if(m_connectionArray)
	   delete [] m_connectionArray;
	m_connectionArray = 0;

	if(m_eventArray)
	for(int iCl = 0;(unsigned)iCl < m_maxConnections;iCl++)
	{
        WSACloseEvent(m_eventArray[iCl]);
	}
	if(m_eventArray)
	   delete [] m_eventArray;
	m_eventArray = 0;

	DeleteCriticalSection(&m_connectionSection);
	DeleteCriticalSection(&m_interactSection);
	
	CloseHandle(m_dataToProcessEvent);
	WriteDebugFile("Closed Cleanly",0,1);
	DeleteCriticalSection(&m_writeDebugSection);

	
	
}

void CAXSocketServer::ConvertToString(long stringTemp, char * buff)
{
		if(stringTemp==0)
		{
		   buff[0] = '0';
		   buff[1] = 0;
		}
		else
		{
        _ltoa(stringTemp,buff,10);
		}
}


void CAXSocketServer::WriteDebugFile(long debugString,BOOL clearIt,BOOL returnCase)
{
	    char buffer[67];
        if(debugString==0)
		{
		   buffer[0] = '0';
		   buffer[1] = 0;
		}
		else
		{
        _ltoa_s(debugString,buffer, sizeof(debugString),10);
		}

		WriteDebugFile(buffer,clearIt,returnCase);
}
//-----------------------------------------------------------------//
void CAXSocketServer::WriteDebugFile(LPSTR debugString,BOOL clearIt,BOOL returnCase)
{
	if(!m_logging)
		return;

	EnterCriticalSection(&m_writeDebugSection);

    int oldFile = 0;
	if(clearIt)	
	   oldFile = open( "log.txt", _O_WRONLY | _O_TRUNC  | _O_CREAT  |_O_BINARY , _S_IWRITE);  
	else
	{
	   oldFile = open( "log.txt", _O_WRONLY  | _O_CREAT  |_O_BINARY , _S_IWRITE); 
	   if(oldFile == -1)
	   {
	      LeaveCriticalSection(&m_writeDebugSection);
	      return;
	   }
	   _lseek( oldFile, 0L, SEEK_END );
	}
	
	int trace = 0;
    while(debugString[trace++]!= 0)
	{
        //trace++;
	}
	long bytesRead = _write( oldFile, debugString, trace );
	if(returnCase)
	{
	 LPSTR returnCase = "\r\n";
     _write( oldFile, returnCase, 2 );
	}
 
	_close( oldFile );

	LeaveCriticalSection(&m_writeDebugSection);

    
}


DWORD WINAPI ConnectionManagement(LPVOID lpParameter)
{
	CAXSocketServer * classObj = (CAXSocketServer *)lpParameter;
	EnterCriticalSection(&classObj->m_interactSection);
    classObj->m_threadCreatedTracker++;
	LeaveCriticalSection(&classObj->m_interactSection);
	//classObj->m_blockSize
	WSANETWORKEVENTS resultStruct;
    char * buff = new char[classObj->m_blockSize];
	ZeroMemory(buff,classObj->m_blockSize);

    int thisShift = 0;
	int finalShiftAmount = 0;//connections this thread will handle
	EnterCriticalSection(&classObj->m_interactSection);
	if(classObj->m_threadsMinorTrace == classObj->m_threadsPerShift)
	{
       classObj->m_threadsMinorTrace = 0;
       classObj->m_eventMemoryShift++;
	}
	classObj->m_threadsMinorTrace++;
	thisShift = classObj->m_eventMemoryShift;
	classObj->WriteDebugFile("Thread Started Covering:",0,0);
	classObj->WriteDebugFile(classObj->m_eventShiftAmount,0,1);
	classObj->WriteDebugFile("Event Array Started at:",0,0);
	classObj->WriteDebugFile(thisShift,0,1);
	LeaveCriticalSection(&classObj->m_interactSection);
	

	while(1)
	{
        int nReturnCode = ::WSAWaitForMultipleEvents(classObj->m_eventShiftAmount, &classObj->m_eventArray[thisShift], FALSE, 20, FALSE);
       
		EnterCriticalSection(&classObj->m_interactSection);
        if(classObj->m_shutDown == TRUE)
		{
           LeaveCriticalSection(&classObj->m_interactSection);
		   break;
		}
	    LeaveCriticalSection(&classObj->m_interactSection);

        if(WSA_WAIT_TIMEOUT == nReturnCode)
           continue;


		if(nReturnCode < 0)
		{
		   int error = WSAGetLastError();
           switch(error)
		   { 
		   case WSANOTINITIALISED:
                classObj->WriteDebugFile("WSANOTINITIALISED WSAWaitForMultipleEvents()",1,1);
		   break;
		   case WSAENETDOWN:
                classObj->WriteDebugFile("WSAENETDOWN WSAWaitForMultipleEvents()",1,1);
		   break;
		   case WSAEINPROGRESS:
                classObj->WriteDebugFile("WSAEINPROGRESS WSAWaitForMultipleEvents()",1,1);
		   break;
		   case WSA_NOT_ENOUGH_MEMORY:
                classObj->WriteDebugFile("WSA_NOT_ENOUGH_MEMORY WSAWaitForMultipleEvents()",1,1);
		   break;
		   case WSA_INVALID_HANDLE:
                classObj->WriteDebugFile("WSA_INVALID_HANDLE WSAWaitForMultipleEvents()",1,1);
		   break;
		   case WSA_INVALID_PARAMETER:
                classObj->WriteDebugFile("WSA_INVALID_PARAMETER WSAWaitForMultipleEvents()",1,1);
		   break;
		   }
           Sleep(20);
		}
       

		int index = nReturnCode - WSA_WAIT_EVENT_0;
		if(index < 0)
		{
		   classObj->WriteDebugFile("Index out of bounds ConnectionManagement()",0,1);
		   Sleep(0);
           continue;
		}
		CAXSvConnection * conPtr = classObj->m_connectionArray[index].connection;
		int result = 0;
		if(conPtr)
		{
			
            
			conPtr->m_lastMsgRecieved = GetTickCount();

			if(::WSAEnumNetworkEvents(conPtr->m_client,classObj->m_eventArray[index],&resultStruct)==0)
			{
               if(resultStruct.lNetworkEvents & FD_ACCEPT)
			   {//authenticated a new connection
                  //classObj->WriteDebugFile("FD_ACCEPT",0,1);

			   }//end authenticated a new connection
			   if(resultStruct.lNetworkEvents & FD_WRITE)
			   {//sending client data

                  //classObj->WriteDebugFile("FD_WRITE",0,1);
			   }//sending client data
			   if(resultStruct.lNetworkEvents & FD_READ)
			   {//get packet from client
                  int bytesRecieved=recv(conPtr->m_client,buff,classObj->m_blockSize,0);
				  if(bytesRecieved>0)
				  {
				     buff[bytesRecieved+1] = 0;
				     classObj->m_packetQue->AddPacket((BYTE*)buff,bytesRecieved,conPtr->m_client,index);
                     SetEvent(classObj->m_dataToProcessEvent);
				  }
				  else
				  {
					 classObj->WriteDebugFile("Unable to add client data to PacketQue)",0,0);
					 classObj->WriteDebugFile(index,0,0);
					 classObj->WriteDebugFile(inet_ntoa(conPtr->m_from.sin_addr),0,1);
				  }				
			   }//end get packet from client
			   if(resultStruct.lNetworkEvents & FD_CLOSE)
			   {//client disconnected
				  buff[0] = 20;
				  buff[1] = 0;
				  buff[2] = 0;
				  classObj->m_packetQue->AddPacket((BYTE*)buff,2,conPtr->m_client,index);
                  SetEvent(classObj->m_dataToProcessEvent);

				  classObj->WriteDebugFile("Client Disconnected (EVENT ID)",0,0);
				  classObj->WriteDebugFile(index,0,0);
		          classObj->WriteDebugFile(inet_ntoa(conPtr->m_from.sin_addr),0,1);
				  classObj->MakeConnectionAvailable(index);
			   }//end client disconnected
			}
			else
			{//error
				switch(result)
				{
					case WSANOTINITIALISED:
                         classObj->WriteDebugFile("WSANOTINITIALISED",0,1);
					break;
					case WSAENETDOWN:
                         classObj->WriteDebugFile("WSAENETDOWN",0,1);
					break;
					case WSAEINVAL:
                         classObj->WriteDebugFile("WSAEINVAL",0,1);
					break;
					case WSAEINPROGRESS:
                         classObj->WriteDebugFile("WSAEINPROGRESS",0,1);
					break;
					case WSAENOTSOCK:
                         classObj->WriteDebugFile("WSAENOTSOCK",0,1);
					break;
					case WSAEFAULT:
                         classObj->WriteDebugFile("WSAEFAULT",0,1);
					break;
					case SOCKET_ERROR:
                         classObj->WriteDebugFile("SOCKET_ERROR",0,1);
					break;
				}
			}
			WSAResetEvent(classObj->m_eventArray[index]);
            
		}
	
		Sleep(0);
	}

	if(buff)
	   delete [] buff;
	buff = 0;
	EnterCriticalSection(&classObj->m_interactSection);
    classObj->m_threadCreatedTracker--;
	LeaveCriticalSection(&classObj->m_interactSection);

	return 0;
}


DWORD WINAPI ServerThread(LPVOID lpParameter)
{
     //cout << "Starting up TCP server\r\n";
	CAXSocketServer * classObj = (CAXSocketServer *)lpParameter;
	EnterCriticalSection(&classObj->m_interactSection);
    classObj->m_threadCreatedTracker++;
	LeaveCriticalSection(&classObj->m_interactSection);
    //A SOCKET is simply a typedef for an unsigned int.
    //In Unix, socket handles were just about same as file 
    //handles which were again unsigned ints.
    //Since this cannot be entirely true under Windows
    //a new data type called SOCKET was defined.
    /*SOCKET server;

    //WSADATA is a struct that is filled up by the call 
    //to WSAStartup
    WSADATA wsaData;

    //The sockaddr_in specifies the address of the socket
    //for TCP/IP sockets. Other protocols use similar structures.
    sockaddr_in local;

    //WSAStartup initializes the program for calling WinSock.
    //The first parameter specifies the highest version of the 
    //WinSock specification, the program is allowed to use.
    int wsaret=WSAStartup(0x101,&wsaData);

    //WSAStartup returns zero on success.
    //If it fails we exit.
    if(wsaret!=0)
    {
        return 0;
    }

    //Now we populate the sockaddr_in structure
    local.sin_family=AF_INET; //Address family
    local.sin_addr.s_addr=INADDR_ANY; //Wild card IP address
    local.sin_port=htons((u_short)classObj->m_port); //port to use

    //the socket function creates our SOCKET
    server=socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);

    //If the socket() function fails we exit
    if(server==INVALID_SOCKET)
    {
        return 0;
    }

    //bind links the socket we just created with the sockaddr_in 
    //structure. Basically it connects the socket with 
    //the local address and a specified port.
    //If it returns non-zero quit, as this indicates error
    if(bind(server,(sockaddr*)&local,sizeof(local))!=0)
    {
        return 0;
    }
    //listen instructs the socket to listen for incoming 
    //connections from clients. The second arg is the backlog
    if(listen(server,10)!=0)
    {
        return 0;
    }
    */

    //we will need variables to hold the client socket.
    //thus we declare them here.
	char minibuff[5];
	ZeroMemory(minibuff,5);
	long closedSocketFrTimeout = -1;
	CString closedSocketBuf;
    
    while(true)//we are looping endlessly
    {
        
		
        //SOCKET client;
        sockaddr_in from;
        //CAXSvConnection * newConnection = new CAXSvConnection();
		int fromlen=sizeof(from);
        //accept() will accept an incoming
        //client connection
        //newConnection->m_client=accept(server,(struct sockaddr*)&newConnection->m_from,&fromlen);
        //authenticate 
		SOCKET newsocket = accept(classObj->m_server,(struct sockaddr*)&from,&fromlen);
         
        EnterCriticalSection(&classObj->m_interactSection);
        if(classObj->m_shutDown == TRUE)
		{
           LeaveCriticalSection(&classObj->m_interactSection);
		   break;
		}
	    LeaveCriticalSection(&classObj->m_interactSection);

        if(newsocket == INVALID_SOCKET)
		{
           classObj->WriteDebugFile("Invalid Socket returned(SERVER THREAD)",0,1);
           Sleep(0);
		   continue;
		} 
        /*char newValue = 1;
		if(setsockopt(newsocket,
					IPPROTO_TCP,
					TCP_NODELAY,
					&newValue,
					sizeof(char))!= 0)
	    {//failed
            classObj->WriteDebugFile("SETSOCK Failed",0,1);
	    }//end failed*/

		//set new socket to non-blocking
		u_long NonBlock = 1;
        if (ioctlsocket(newsocket, FIONBIO, &NonBlock) == SOCKET_ERROR)
        {
		    classObj->WriteDebugFile("Could not make newsocket non-blocking ioctlsocket() ServerThread",0,1);
        }

        //if authenticate fails
        //delete newConnection;
		//newConnection = 0;
		if(classObj->m_connectionArray)
		{
		   //int evRes = ::WSAEventSelect(newConnection->m_client, classObj->m_events[classObj->m_eventTrace++],  FD_WRITE | FD_READ | FD_CLOSE);
		   //classObj->m_connections->AddConnection(newConnection);//added user
		   
		   CAXSvConnection * openObjectConn = classObj->GetUnusedConnection();
		   if(closedSocketFrTimeout > -1)
		   {//close connection

		   }//end close connection

		   if(openObjectConn)
		   {//found available connection
		    openObjectConn->m_client = newsocket;//bind socket
			if(WSAEventSelect(openObjectConn->m_client, classObj->m_eventArray[openObjectConn->m_connectionIndex],  FD_WRITE | FD_READ | FD_CLOSE)==0)
			{
                //set socket options
				/*char newValue = 1;
				if(setsockopt(openObjectConn->m_client,
					          IPPROTO_TCP,
						      SO_DONTLINGER,
						      &newValue,
						      sizeof(char))!= 0)
				{//failed
                   classObj->WriteDebugFile("SETSOCK Failed",0,1);
				}//end failed*/

				/*char newValue = 0;
				if(setsockopt(openObjectConn->m_client,
								IPPROTO_TCP,
								SO_SNDBUF,
								&newValue,
								sizeof(char))!= 0)
				{//failed
5					classObj->WriteDebugFile("SETSOCK Failed SO_SNDBUF",0,1);
				}//end failed*/



				CopyMemory(&openObjectConn->m_from,&from,sizeof(sockaddr_in));
				//char * connectionMsg = "Connected...";
				openObjectConn->SendData((BYTE*)classObj->m_blockSizeString,(int)strlen(classObj->m_blockSizeString));//send communication rate
				classObj->WriteDebugFile("Client Connected (EVENT ID)",0,0);
				classObj->WriteDebugFile(openObjectConn->m_connectionIndex,0,0);
				classObj->WriteDebugFile(":",0,0);
				classObj->WriteDebugFile(inet_ntoa(openObjectConn->m_from.sin_addr),0,1);

				//notify of unauth connection
                if(classObj->m_authenticationEnabled)
				{
					ZeroMemory(minibuff,5);
					minibuff[0] = 21;
					classObj->m_packetQue->AddPacket((BYTE*)minibuff,3,openObjectConn->m_client,openObjectConn->m_connectionIndex);
					SetEvent(classObj->m_dataToProcessEvent);
				}
			}
			else
			{
                closesocket(newsocket);
			    classObj->WriteDebugFile("Event Assignment Failed(EVENT ID)",0,0);
				classObj->WriteDebugFile(openObjectConn->m_connectionIndex,0,0);
				classObj->WriteDebugFile(":",0,0);
			    classObj->WriteDebugFile(inet_ntoa(from.sin_addr),0,1);
			}
		   }//end found available connection
		   else
		   {
            closesocket(newsocket);
			classObj->WriteDebugFile("Client Rejected due to not enough connections:",0,0);
			classObj->WriteDebugFile(inet_ntoa(from.sin_addr),0,1);
		   }
		}
		else
		{
           closesocket(newsocket);
		}
		Sleep(0);
        
    }

    //closesocket() closes the socket and releases the socket descriptor
    //closesocket(server);

    //originally this function probably had some use
    //currently this is just for backward compatibility
    //but it is safer to call it as I still believe some
    //implementations use this to terminate use of WS2_32.DLL 
    //WSACleanup();

	EnterCriticalSection(&classObj->m_interactSection);
    classObj->m_threadCreatedTracker--;
    LeaveCriticalSection(&classObj->m_interactSection);

    return 0;
}