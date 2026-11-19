#include "StdAfx.h"
#include "axcontentserver.h"
#include "axencryptionlib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI PacketProcessor(LPVOID lpParameter);
DWORD WINAPI streamManager(LPVOID lpParameter);

CAXContentServer::CAXContentServer(void)
{
	//Initialize
	m_sServer			= NULL;
	m_dataStorage		= NULL;
	m_threadIds			= NULL;
	m_threadHandles     = NULL;
	m_eventLog          = NULL;
	m_shutDown          = FALSE;
	m_encryptor         = NULL;
	m_theadShutDownCount= 0;
	InitializeCriticalSection(&m_cSection);
    InitializeCriticalSection(&m_cStreamerSection);

	m_sServer->WriteDebugFile("starting up",0,1);
	
	m_threadCount = 1;
	m_encryptor = new CAXEncryptionLib("lucidcatscanarbcowlordarronnicol");
    //connect to database
	m_dataStorage = new CAXFileObjectDB();
	m_activeConnections = new CActiveConnectionDB();
	m_sServer = new CAXSocketServer();
	
	m_eventLog = new CServerEventList();
	m_eventLog->EnableCriticalSection();
	
	

	ConnectionStructureClass authConnectStruct;
	authConnectStruct.AquireFromCfgFile("authAccount.cfg");
	m_authConnection = new CDBConnectionSystem(authConnectStruct.m_sqlServerIP,
		                                       authConnectStruct.m_sqlServerCatalog,
											   authConnectStruct.m_sqlServerUsername,
											   authConnectStruct.m_sqlServerPassword,
											   m_eventLog);
    //Establish Connection
	//m_authConnection->EstablishConnection();
    if(!m_authConnection->m_connectionEstablished)
	{
		m_sServer->WriteDebugFile("authAccount.cfg SQL Connection Failed",0,1);
	    return;
	}
	
	ConnectionStructureClass connectStruct;
	connectStruct.AquireFromCfgFile("contentSvr.cfg");
    m_dbConnection = new CDBConnectionSystem(connectStruct.m_sqlServerIP,
		                                     connectStruct.m_sqlServerCatalog,
											 connectStruct.m_sqlServerUsername,
											 connectStruct.m_sqlServerPassword,
											 m_eventLog);
    //Establish Connection
	//m_dbConnection->EstablishConnection();
    if(!m_dbConnection->m_connectionEstablished)
	{
		m_sServer->WriteDebugFile("SQL Connection Failed",0,1);
	    return;
	}

	


	
	//
	   //AXWeb update- this block servers as a ayetem when AXHUB AKA(chain sql) is down this will check
	   //the AXWeb que and update the chain AXHUB for any username/ban/permissions etc
	   //note the most this would be called is once if the AXHUB was down during a critical variable change
	   //Generally the AXWEB will update AXHUB on a event that critical variable change unless AXHUB was down when AXWeb 
	   //made the change, in case this happens once the AXHUB comes online and these servers start spinning up
	   //querries must handle multiple execs without consiquence to avoid potential timing issues with multiple ids comming on at the exact time even though unlikly
	   //AVOID multiple servers that are chained from any timing issues by only having one server per chain access to the axweb::as a standard ill assign server ID 1 on the dev platform as an example
	   ConnectionStructureClass AXWebConnection;
	   if(AXWebConnection.AquireFromCfgFile("axweb.cfg"))
	   {//aquired connect info
		  m_axWebConnection = new CDBConnectionSystem(AXWebConnection.m_sqlServerIP,
																		  AXWebConnection.m_sqlServerCatalog,
																		  AXWebConnection.m_sqlServerUsername,
																		  AXWebConnection.m_sqlServerPassword,
																		  m_eventLog);
		  if(m_axWebConnection->m_connectionEstablished == TRUE)
		  {//verify axweb connection established
			 BOOL qryResult = 0;
			 _RecordsetPtr rdPtr = m_axWebConnection->Query("SELECT statement,dateadded FROM tblBroadcastSync WHERE HUBIP = '"+connectStruct.m_sqlServerIP+"' ORDER BY DATEADDED");
			 if(rdPtr!=NULL)
			 {//querry AXWEB for pending commands while this server parent was down
				int recordCount = m_axWebConnection->GetCount(rdPtr);
				for(int broadCLoop = 0;broadCLoop<recordCount;broadCLoop++)
				{//statements to execure on AXHUB
				   CString rawStatement = m_axWebConnection->GetStringData("statement",rdPtr);
				   CString dateAddedLoc = m_axWebConnection->GetStringData("dateadded",rdPtr);
				   qryResult = m_dbConnection->ExecQuery(m_axWebConnection->SanitizeFormatOut(rawStatement));//sanitize out will turn the formatted query live
				   m_axWebConnection->NextRecord(rdPtr);
				   if(qryResult)//only remove if the query succeeded
				   {
					  m_sServer->WriteDebugFile(rawStatement.GetBuffer(),0,0);
					  m_sServer->WriteDebugFile(":suceeded",0,1);
					  if(!m_axWebConnection->ExecQuery("DELETE FROM tblBroadcastSync where dateadded = '"+ dateAddedLoc + "'"))//delete pending command using dateadded as condition
			              m_sServer->WriteDebugFile("Clear by date failed",0,1);
				   }
				   else
					  m_sServer->WriteDebugFile((rawStatement + " :Failed").GetBuffer(),0,1);
				}//end statements to execure on AXHUB
				m_axWebConnection->Close(rdPtr);
			 }//end querry AXWEB for pending commands while this server parent was down
		  }//end axweb verify connection established
		  
	   }//end aquired connect info
	   else
	      m_sServer->WriteDebugFile("****connect info to create link for axhub is not present, unable to check for critical variable changes at this time.",0,1);
		  //m_eventLog->AddEvent("****connect info to create link for axhub is not present, unable to check for critical variable changes at this time.");
	   //end AXWeb Update
    //Clear current connections
    m_dbConnection->ExecQuery("DELETE FROM PTC_CURRENTCONNECTIONS");
    //Aquire TCP server configuration
	CString specificIP = "";
    CString configQuery = "SELECT * FROM PTC_MAINCONFIG";
    _RecordsetPtr cReader = m_dbConnection->Query(configQuery);
    if(m_dbConnection->GetCount(cReader) > 0)
	{
		
       int maxConnections	= m_dbConnection->GetIntData("MAXCONNECTIONS",cReader);
	   int tcpPort			= m_dbConnection->GetIntData("TCPPORT",cReader);
	   int blockSize		= m_dbConnection->GetIntData("BLOCKSIZE",cReader);
	   m_threadCount		= m_dbConnection->GetIntData("THREADCOUNT",cReader);
	   specificIP           = m_dbConnection->GetStringData("SPECIFICIP",cReader);
	   //m_threadCount = 1;//TEST
       //maxConnections = 5;
       //m_sServer = new CAXSocketServer();
	   BOOL result = m_sServer->StartServer(m_threadCount,tcpPort,blockSize,maxConnections,TRUE,specificIP);
	}
	m_dbConnection->Close(cReader);
	m_activeConnections->m_sServerRef = m_sServer;
	m_sServer->m_fastestSendSpeed = 0;//send speed
	//create processing threads
    m_threadHandles = new HANDLE[m_threadCount];
    m_threadIds     = new DWORD[m_threadCount];
	if(m_threadHandles)
    for(int tLoop=0;tLoop<m_threadCount/2;tLoop++)
	{
	    m_threadHandles[tLoop] = CreateThread(NULL,0,PacketProcessor,this,0,&m_threadIds[tLoop]);
	}

	if(m_threadHandles)
    for(int tLoop=m_threadCount/2;tLoop<m_threadCount;tLoop++)
	{
	    m_threadHandles[tLoop] = CreateThread(NULL,0,streamManager,this,0,&m_threadIds[tLoop]);
	}
	m_sServer->WriteDebugFile("All systems Initialized",0,1);
}

CAXContentServer::~CAXContentServer(void)
{
	EnterCriticalSection(&m_cSection);
    m_shutDown = TRUE;
	LeaveCriticalSection(&m_cSection);

	EnterCriticalSection(&m_cSection);
	while(m_theadShutDownCount < m_threadCount)
	{
		LeaveCriticalSection(&m_cSection);
        Sleep(200);
		EnterCriticalSection(&m_cSection);
		SetEvent(m_sServer->m_dataToProcessEvent);
	}
	LeaveCriticalSection(&m_cSection);

    if(m_threadHandles)
    for(int tLoop=0;tLoop<m_threadCount;tLoop++)
	{
	    CloseHandle(m_threadHandles[tLoop]);
	}
    if(m_threadHandles)
	   delete [] m_threadHandles;
	m_threadHandles = 0;

	if(m_threadIds)
	   delete [] m_threadIds;
	m_threadIds = 0;

	if(m_dbConnection)
	   delete m_dbConnection;
	m_dbConnection = 0;

	if(m_authConnection)
		delete m_authConnection;
	m_authConnection = 0;

	if(m_encryptor)
	   delete m_encryptor;
	m_encryptor = 0;

	if(m_sServer)
	   delete m_sServer;
	m_sServer = 0;

	if(m_dataStorage)
	   delete m_dataStorage;
	m_dataStorage = 0;

	if(m_axWebConnection)
		delete m_axWebConnection;
	m_axWebConnection = 0;

	if(m_eventLog)
	{
        m_eventLog->OutPutTextFile("ContentServerEvents.txt");
		m_eventLog->SafeDelete();
		delete m_eventLog;
		m_eventLog = 0;
	}
	if(m_activeConnections)
	{
       delete m_activeConnections;
	   m_activeConnections = 0;
	}

	

	DeleteCriticalSection(&m_cSection);
	DeleteCriticalSection(&m_cStreamerSection);
}


bool CAXContentServer::AddConnection(CString ip, long socket, int connectionIndex)
{
	m_activeConnections->AddConnection(connectionIndex);
    CString query = "INSERT INTO PTC_CURRENTCONNECTIONS(IP, SOCKET, CONNECTIONINDEX) VALUES('$IP', '$SOCKET', '$CONNECTIONINDEX')";
    query.Replace("$IP",ip);
	query.Replace("$SOCKET",m_cabLib.ConvertLongToString(socket));
	query.Replace("$CONNECTIONINDEX",m_cabLib.ConvertLongToString(connectionIndex));
	m_dbConnection->ExecQuery(query);
 
	return true;
}

bool CAXContentServer::AuthenticateConnectionInDB(CString ip, long socket, CString username)
{
    CString query = "UPDATE PTC_CURRENTCONNECTIONS SET USERNAME = '$USERNAME' WHERE IP = '$IP' AND SOCKET = '$SOCKET'";
    query.Replace("$IP",m_dbConnection->SanitizeFormatIn(ip));
	query.Replace("$USERNAME",m_dbConnection->SanitizeFormatIn(username));
	query.Replace("$SOCKET",m_cabLib.ConvertLongToString(socket));
	m_dbConnection->ExecQuery(query);

    return true;
}

/*bool CAXContentServer::RemoveConnection(CString ip, long socket)
{
	
    CString query = "DELETE FROM PTC_CURRENTCONNECTIONS WHERE IP = '$IP' AND SOCKET = '$SOCKET'";
    query.Replace("$IP",ip);
	query.Replace("$SOCKET",m_cabLib.ConvertLongToString(socket));
	m_dbConnection->ExecQuery(query);

    return true;
}*/

bool CAXContentServer::RemoveConnection(int connectionIndex)
{
	m_activeConnections->RemoveConnection(connectionIndex);
    CString query = "DELETE FROM PTC_CURRENTCONNECTIONS WHERE CONNECTIONINDEX = '$CONNECTIONINDEX'";
	query.Replace("$CONNECTIONINDEX",m_cabLib.ConvertLongToString(connectionIndex));
	m_dbConnection->ExecQuery(query);

    return true;
}

bool CAXContentServer::UpdateKBS(long kbs, long socket, CString ip, CString currentFileName, long currentPosition)
{
    CString query = "UPDATE PTC_CURRENTCONNECTIONS SET KBS = '$KBS', LASTUPDATED = GETDATE(), currentFileName = '$CURRENTFILENAME', currentPosition = '$CURRENTPOSITION' WHERE IP = '$IP' AND SOCKET = '$SOCKET'";
    query.Replace("$IP",m_dbConnection->SanitizeFormatIn(ip));
	query.Replace("$CURRENTFILENAME",m_dbConnection->SanitizeFormatIn(currentFileName));
	query.Replace("$KBS",m_cabLib.ConvertLongToString(kbs));
	query.Replace("$CURRENTPOSITION",m_cabLib.ConvertLongToString(currentPosition));
	query.Replace("$SOCKET",m_cabLib.ConvertLongToString(socket));
	m_dbConnection->ExecQuery(query);

    return true;
}


DWORD WINAPI streamManager(LPVOID lpParameter)
{
    CAXContentServer * classObj = (CAXContentServer *)lpParameter;
	
    CAXFileObject * fileRef = NULL;
	long fileStartpoint = 0;
	char * bufferMisc = new char[classObj->m_sServer->m_blockSize];
	ZeroMemory(bufferMisc,classObj->m_sServer->m_blockSize);

	DWORD lastSendTimeStamp = GetTickCount();
	CString tempIP;
	long kbsTemp = 0;
	fd_set statusLocalW;
	fd_set statusLocalE;
	timeval timoutForSelect;
	timoutForSelect.tv_sec = 0;
	timoutForSelect.tv_usec = 0;



	while(1)
	{//main while loop
		EnterCriticalSection(&classObj->m_cSection);
	    if(classObj->m_shutDown)
	    {
           classObj->m_theadShutDownCount++;
		   LeaveCriticalSection(&classObj->m_cSection);
		   delete [] bufferMisc;
		   bufferMisc = 0;
           return 0;
	    }
	    LeaveCriticalSection(&classObj->m_cSection);
		
		//for(int cnLoop=0;cnLoop<classObj->m_sServer->m_maxConnections;cnLoop++){
		int cnLoop = classObj->m_activeConnections->GetNextConnectionToService();
        if(cnLoop == -1)
		{
		   Sleep(5);
		   continue;
		}
            
			
			if(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_inUse == 1)
			{
				//manage manual timouts
				if(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authenticated == 0)
				if((GetTickCount()-classObj->m_sServer->m_connectionArray[cnLoop].connection->m_connectTime) > 15000)
				{//disconnect
				   classObj->m_sServer->WriteDebugFile("SocketTimeout-anuth",0,0);
				   classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
				   Sleep(classObj->m_sServer->m_fastestSendSpeed);
				   classObj->m_sServer->MakeConnectionAvailable(cnLoop);
				   continue;
				}//end disconnect
			}
			
			if(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_streaming == true)
			{
                statusLocalW.fd_array[0] = classObj->m_sServer->m_connectionArray[cnLoop].connection->m_client;
				statusLocalW.fd_count = 1;
                
				int res = select(0,//int nfds,
						         NULL,//fd_set* readfds,
						         &statusLocalW,//fd_set* writefds,
						         NULL,//fd_set* exceptfds,
						         &timoutForSelect
						         );
				if(res == 0)
				{//not ready for msgs
                   //LeaveCriticalSection(&classObj->m_cStreamerSection);
					classObj->m_activeConnections->FinishWithConnection(cnLoop,10);
                    continue;
				}//end not ready for msgs

				statusLocalE.fd_array[0] = classObj->m_sServer->m_connectionArray[cnLoop].connection->m_client;
				statusLocalE.fd_count = 1;
                //check for errors
                res = select(0,//int nfds,
						         NULL,//fd_set* readfds,
						         NULL,//fd_set* writefds,
						         &statusLocalE,//fd_set* exceptfds,
						         &timoutForSelect
						         );
				if(res == 1)
				{//error possibly close socket
				   bool closeSocket = false;
                   switch(WSAGetLastError())
				   {
				      case WSAEINPROGRESS:
					       break;
					  default:
                           closeSocket = true;
				   }
				   if(closeSocket)
				   {
				      classObj->m_sServer->MakeConnectionAvailable(cnLoop);
				      //LeaveCriticalSection(&classObj->m_cStreamerSection);
					  classObj->m_sServer->WriteDebugFile("SOKET ERROR-CLOSING IT",0,1);
				   }
				   
				   Sleep(classObj->m_sServer->m_fastestSendSpeed);
				   classObj->m_activeConnections->FinishWithConnection(cnLoop,10);
                   continue;
				}//end error possibly close socket



				classObj->m_sServer->m_connectionArray[cnLoop].connection->m_sending = true;
				//LeaveCriticalSection(&classObj->m_cStreamerSection);

	            fileRef = (CAXFileObject *)classObj->m_sServer->m_connectionArray[cnLoop].connection->m_miscDataRef;
				fileStartpoint = classObj->m_sServer->m_connectionArray[cnLoop].connection->m_miscLong;
                 
				ZeroMemory(bufferMisc,classObj->m_sServer->m_blockSize);	    
				long maxBufSize = classObj->m_sServer->m_blockSize;// - sizeof(Genheader);
				long remainder = fileRef->m_fileLength - fileStartpoint;
				if(remainder < maxBufSize)
					maxBufSize = remainder;
				
				if(maxBufSize > 0)
				{//send packet
					//Sleep(0);
					int dataSent = classObj->m_sServer->m_connectionArray[cnLoop].connection->SendData((BYTE*)&fileRef->m_data[fileStartpoint],maxBufSize);
				    if(dataSent == SOCKET_ERROR)
					{//log failure (and disconnect user)
						bool closeSocket = true;
						switch(WSAGetLastError())
				        {
				           case WSAEINPROGRESS:
							    classObj->m_sServer->WriteDebugFile("WSAEINPROGRESS-",0,0);
						        classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
					            closeSocket = false;
								break;
						   case WSAEWOULDBLOCK:
							    classObj->m_sServer->WriteDebugFile("Would Block-",0,0);
						        classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
					            closeSocket = false;
								break;
						   case WSANOTINITIALISED:
							    classObj->m_sServer->WriteDebugFile("WSANOTINITIALISED-",0,0);
						        classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
					            break;
						   case WSAENETDOWN:
						        classObj->m_sServer->WriteDebugFile("WSAENETDOWN-",0,0);
						        classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
					            break;
						   case WSAEACCES:
						        classObj->m_sServer->WriteDebugFile("WSAEACCES-",0,0);
						        classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
					            break;
						   case WSAEINTR:
							    classObj->m_sServer->WriteDebugFile("WSAEINTR-",0,0);
						        classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
					            break;
						   case WSAEFAULT:
							    classObj->m_sServer->WriteDebugFile("WSAEFAULT-",0,0);
						        classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
					            break;
						   case WSAENETRESET:
						        classObj->m_sServer->WriteDebugFile("WSAENETRESET-",0,0);
						        classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
					            break;
						   case WSAENOBUFS:
						        classObj->m_sServer->WriteDebugFile("WSAENOBUFS-",0,0);
						        classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
					            break;
						   case WSAENOTCONN:
						        classObj->m_sServer->WriteDebugFile("WSAENOTCONN-",0,0);
						        classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
					            break;
						   case WSAENOTSOCK:
						        classObj->m_sServer->WriteDebugFile("WSAENOTSOCK-",0,0);
						        classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
					            break;
						   case WSAEOPNOTSUPP:
						        classObj->m_sServer->WriteDebugFile("WSAEOPNOTSUPP-",0,0);
						        classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
					            break;
						   case WSAESHUTDOWN:
						        classObj->m_sServer->WriteDebugFile("WSAESHUTDOWN-",0,0);
						        classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
					            break;
						   case WSAEMSGSIZE:
						        classObj->m_sServer->WriteDebugFile("WSAEMSGSIZE-",0,0);
						        classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
					            break;
						   case WSAEHOSTUNREACH:
						        classObj->m_sServer->WriteDebugFile("WSAEHOSTUNREACH-",0,0);
						        classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
					            break;
						   case WSAEINVAL:
							    classObj->m_sServer->WriteDebugFile("WSAEINVAL-",0,0);
						        classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
					            break;
						   case WSAECONNABORTED:
						        classObj->m_sServer->WriteDebugFile("WSAECONNABORTED-",0,0);
						        classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
					            break;
						   case WSAECONNRESET:
						        classObj->m_sServer->WriteDebugFile("Would Block-",0,0);
						        classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
					            break;
						   case WSAETIMEDOUT:
						        classObj->m_sServer->WriteDebugFile("WSAETIMEDOUT-",0,0);
						        classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
					            break;

				        }
                        if(closeSocket)
						{
                           classObj->m_sServer->WriteDebugFile("Send Stream Failed-",0,0);
						   classObj->m_sServer->WriteDebugFile(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_authPacketRef.username,0,1);
						   classObj->m_sServer->MakeConnectionAvailable(cnLoop);
						}
						
						
						Sleep(classObj->m_sServer->m_fastestSendSpeed);
						classObj->m_activeConnections->FinishWithConnection(cnLoop,10);
						continue;
					}//end log failure (and disconnect user)
					else//success
					{
					      classObj->m_sServer->m_connectionArray[cnLoop].connection->m_miscLong += dataSent;
					}

					if(GetTickCount()-classObj->m_sServer->m_connectionArray[cnLoop].connection->m_miscTimeStamp > 10000)
			        {
					   classObj->UpdateKBS(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_kbs*8, 
						                   (long)classObj->m_sServer->m_connectionArray[cnLoop].connection->m_client, 
										   inet_ntoa(classObj->m_sServer->m_connectionArray[cnLoop].connection->m_from.sin_addr),
										   fileRef->m_publicName,
										   fileStartpoint/1000000);
					   classObj->m_sServer->m_connectionArray[cnLoop].connection->m_miscTimeStamp = GetTickCount();
				    }
				}//end send packet
				else
				{//shut down streaming to this client
                    classObj->m_sServer->m_connectionArray[cnLoop].connection->m_sending = false;
				    classObj->m_sServer->m_connectionArray[cnLoop].connection->m_streaming = false;
				}	
				
				lastSendTimeStamp = GetTickCount();
				classObj->m_sServer->m_connectionArray[cnLoop].connection->m_lastMsg = GetTickCount();
                classObj->m_sServer->m_connectionArray[cnLoop].connection->m_sending = false;		
			}
			
        classObj->m_sServer->m_connectionArray[cnLoop].connection->m_sendAttempTimeStamp = GetTickCount();
		Sleep(classObj->m_sServer->m_fastestSendSpeed);		
		classObj->m_activeConnections->FinishWithConnection(cnLoop);


	}//while loop end 

	delete [] bufferMisc;
    bufferMisc = 0;

	return 0;
}


DWORD WINAPI PacketProcessor(LPVOID lpParameter)
{
	CAXContentServer * classObj = (CAXContentServer *)lpParameter;

	CAXSsPacket * packet = NULL;
	CString queryAuth = "SELECT * FROM GMACCOUNTBASE WHERE USERNAME = '$USERNAME'";
	CString queryBannedAuth = "SELECT * FROM GMBANNEDINFO WHERE USERNAME = '$USERNAME' OR IP = '$IP'";
	//m_authConnection
	
	
	char * bufferMisc = new char[classObj->m_sServer->m_blockSize];
	//char bufferMisc[100000];
	CString formatNameTmp;
	ZeroMemory(bufferMisc,classObj->m_sServer->m_blockSize);
	CString tempQ;
	CString tempIP;
    SOCKET socketTemp;
	long kbsTemp = 0;
	CAXSvConnection * conPtrRef = NULL;
	int connectionIndex = 0;
	cnPack * cnCast = NULL;
	while(1)
	{
		EnterCriticalSection(&classObj->m_cSection);
		if(classObj->m_shutDown)
		{
           classObj->m_theadShutDownCount++;
		   LeaveCriticalSection(&classObj->m_cSection);
           return 0;
		}
	    LeaveCriticalSection(&classObj->m_cSection);
		ResetEvent(classObj->m_sServer->m_dataToProcessEvent);
        WaitForSingleObject(classObj->m_sServer->m_dataToProcessEvent,20);
        //get packets and process by first byte
        if(classObj->m_sServer->m_packetQue->GetNextPacket(&packet)==true)
		{//process packets
           
		   switch(packet->data[0])
		   {//pack processor
               case 0:// Authentication Packet
			      if(1)
				  {
                     if(packet->length < sizeof(AuthPacket))
					    break;

                     AuthPacket * authCast = (AuthPacket *)packet->data;
					 
				      
                     //do authentication here
					 CString tempXd = authCast->username;
					 CString tempXp = authCast->password;
                  	 
					 tempQ = queryAuth;
					 tempQ.Replace("$USERNAME",tempXd);
					 _RecordsetPtr userReader = classObj->m_authConnection->Query(tempQ);
                     if(classObj->m_authConnection->GetCount(userReader)==0)
					 {//username does not exist
						classObj->m_authConnection->Close(userReader);
                        SignalPacket sigPacket;
						sigPacket.type = 6;
					    classObj->m_sServer->SendDataToID(packet->connectionId,(BYTE*)&sigPacket,sizeof(SignalPacket));//send authenicate confirmation
						classObj->m_sServer->WriteDebugFile("User Access Denied(invalid username):",0,0);
						classObj->m_sServer->WriteDebugFile(tempXd.GetBuffer(),0,1);
					 }//end username does not exist
					 else if(tempXp.CompareNoCase(classObj->m_encryptor->DecryptString64(classObj->m_authConnection->GetStringData("PASSWORD",userReader).GetBuffer()))==0)
					 {//good password
						classObj->m_authConnection->Close(userReader);
                        //Verify not banned
                        tempQ = queryBannedAuth;
						tempQ.Replace("$USERNAME",tempXd);
						tempQ.Replace("$IP",classObj->m_sServer->GetIPFromConnectionID(packet->connectionId));
						_RecordsetPtr userReader = classObj->m_authConnection->Query(tempQ);
						if(classObj->m_authConnection->GetCount(userReader)>0)
						{//banned
                            classObj->m_authConnection->Close(userReader);
							SignalPacket sigPacket;
							sigPacket.type = 8;
							classObj->m_sServer->SendDataToID(packet->connectionId,(BYTE*)&sigPacket,sizeof(SignalPacket));//send authenicate confirmation
							classObj->m_sServer->WriteDebugFile("User Access Denied(Banned):",0,0);
							classObj->m_sServer->WriteDebugFile(tempXd.GetBuffer(),0,1);

						}//end banned
						else{//not banned
							classObj->m_authConnection->Close(userReader);
							//end verify not banned
							
							if(classObj->m_sServer->Authenticate(packet->connectionId,authCast,tempIP,socketTemp)==true)
							{
									classObj->AuthenticateConnectionInDB(tempIP,
																		(long)socketTemp, 
																		authCast->username);
									classObj->m_sServer->WriteDebugFile("Authenticate Connection to DB",0,1);
									SignalPacket sigPacket;
									sigPacket.type = 5;
									classObj->m_sServer->SendDataToID(packet->connectionId,(BYTE*)&sigPacket,sizeof(SignalPacket));//send authenicate confirmation
									classObj->m_sServer->WriteDebugFile("User Authenticated:",0,0);
									classObj->m_sServer->WriteDebugFile(authCast->username,0,1);
							}
							else
							{
								classObj->m_authConnection->Close(userReader);
								SignalPacket sigPacket;
								sigPacket.type = 9;
								classObj->m_sServer->SendDataToID(packet->connectionId,(BYTE*)&sigPacket,sizeof(SignalPacket));//send authenicate confirmation
								classObj->m_sServer->WriteDebugFile("User Access Denied(TOO MANY CONNECTIONS):",0,0);
						        classObj->m_sServer->WriteDebugFile(tempQ.GetBuffer(),0,1);
							}
						}//end not banned
					 }//end good password
					 else
					 {//did not authenticate
						        classObj->m_authConnection->Close(userReader);
                                SignalPacket sigPacket;
								sigPacket.type = 7;
								classObj->m_sServer->SendDataToID(packet->connectionId,(BYTE*)&sigPacket,sizeof(SignalPacket));//send authenicate confirmation
								classObj->m_sServer->WriteDebugFile("User Access Denied(wrong password):",0,0);
						        classObj->m_sServer->WriteDebugFile(tempQ.GetBuffer(),0,1);
					 }//end did not authenticate
					 
				  }
			   break;
		       case 1:// Request File Length
			   {
					  if(classObj->m_sServer->IsAuthenticated(packet->connectionId)==0)
					  {
					     classObj->m_sServer->MakeConnectionAvailable(packet->connectionId);
			             classObj->m_sServer->WriteDebugFile("Request File Length-no valid authentication",0,1);
						 //classObj->m_sServer->MakeConnectionAvailable(packet->connectionId);
						 break;
					  }
				      
					  ZeroMemory(bufferMisc,classObj->m_sServer->m_blockSize);
					  CopyMemory(bufferMisc,&packet->data[1],packet->length);
					  formatNameTmp = bufferMisc;
					  CAXFileObject * axFileObj = classObj->m_dataStorage->GetFileObjectByName(formatNameTmp);
			
						if(!axFileObj)
						{//request does not exist
							classObj->m_sServer->WriteDebugFile("Attempting to aquire file:",0,0);
							classObj->m_sServer->WriteDebugFile(formatNameTmp.GetBuffer(),0,1);
							CString configQuery = "SELECT * FROM GMCONTENT WHERE CABNAME = '"+formatNameTmp+"'";
							_RecordsetPtr cReader = classObj->m_axWebConnection->Query(configQuery);
							if(classObj->m_axWebConnection->GetCount(cReader) > 0)
							{
								CString path = classObj->m_axWebConnection->GetStringData("SECUREPATH",cReader);
								classObj->m_sServer->WriteDebugFile("Secure path:",0,0);
							    classObj->m_sServer->WriteDebugFile(path.GetBuffer(),0,1);
								if(classObj->m_dataStorage->AddFile(formatNameTmp, path)==true)
								{
								   //log addition
								   axFileObj = classObj->m_dataStorage->GetFileObjectByName(formatNameTmp);
								   classObj->m_sServer->WriteDebugFile("File added to cache:",0,0);
								   classObj->m_sServer->WriteDebugFile(formatNameTmp.GetBuffer(),0,1);
								   
								}
								else
								{
								   classObj->m_sServer->WriteDebugFile("Cannot find file in secure path:",0,0);
								   classObj->m_sServer->WriteDebugFile(formatNameTmp.GetBuffer(),0,1);
								   classObj->m_sServer->MakeConnectionAvailable(packet->connectionId);
								}
							}
							else
							{//log error
								classObj->m_sServer->WriteDebugFile("Invalid file request:",0,0);
								classObj->m_sServer->WriteDebugFile(formatNameTmp.GetBuffer(),0,1);
								classObj->m_sServer->MakeConnectionAvailable(packet->connectionId);
							}//end log error
							classObj->m_axWebConnection->Close(cReader);
						}//end request does not exist
						if(axFileObj)
						{//aquired data structure send Length packet
						   fileCRC fcrc;
						   fcrc.type = 2;
						   fcrc.length = axFileObj->m_fileLength;
						   fcrc.crc = 0;
						   fcrc.fileIndex = axFileObj->m_fileIndex;
						   
						   if(!classObj->m_sServer->SendDataToID(packet->connectionId,(BYTE*)&fcrc,sizeof(fileCRC)))
						   {//log failure (and disconnect user)

						   }//end log failure (and disconnect user)
						}//end aquired data structure send Length packet
						else
						{//log error
	                        
						}//end log error	  
			   }
               break;
               case 2:// Request File from Start Point 
                    if(classObj->m_sServer->IsAuthenticated(packet->connectionId)==0)
				       classObj->m_sServer->MakeConnectionAvailable(packet->connectionId);

					if(packet->length == sizeof(Genheader))
					{//found correct construct
						Genheader * fileBl = (Genheader *)packet->data;
						CAXFileObject * axFileObj = classObj->m_dataStorage->GetFileIndex(fileBl->shortType);
						if(axFileObj)
						{//valid file begin sending streamed data
                           if(classObj->m_sServer->GetDataFromConnectionID(packet->connectionId,tempIP,socketTemp,connectionIndex,kbsTemp,&conPtrRef)==true)
						   {//found connection
							   conPtrRef->m_miscLong = fileBl->longType;
							   conPtrRef->m_miscLong2 = fileBl->shortType;
							   conPtrRef->m_miscDataRef = axFileObj;
                               conPtrRef->m_streaming = true;//begin streaming
						   }//end found connection
						}//end valid file begin sending streamed data
					}//found correct contruct
               break;
			   case 20://disconnect
				   if(1)
				   {
					if(classObj->m_sServer->GetDataFromConnectionID(packet->connectionId,tempIP,socketTemp,connectionIndex,kbsTemp,&conPtrRef)==true)
					{
						classObj->RemoveConnection(packet->connectionId);
						classObj->m_sServer->WriteDebugFile("Remove connection from DB",0,1);
					}
					else
					{
	                      
					}
				   }
			   break;
			   case 21://connected but not authenticated
				   if(1)
				   {
				   if(classObj->m_sServer->GetDataFromConnectionID(packet->connectionId,tempIP,socketTemp,connectionIndex,kbsTemp,&conPtrRef)==true)
				   {
                      classObj->AddConnection(tempIP, (long)socketTemp, connectionIndex);
					  classObj->m_sServer->WriteDebugFile("Add Base Connection to DB",0,1);
				   }
				   else
				   {
                      
				   }
				   }
			   break;
			   case 22://m disconnect
                   if(packet->length < sizeof(cnPack))	
				      break;
 
				   cnCast = (cnPack *)packet->data;
                   classObj->RemoveConnection(packet->connectionId);
				   classObj->m_sServer->WriteDebugFile("Remove connection from DB Manually",0,1);

			   break;
			   default:
	
			   classObj->m_sServer->MakeConnectionAvailable(packet->connectionId);
			   classObj->m_sServer->WriteDebugFile("Invalid Packet-Remove connection from DB Manually",0,1);
			   break;
		   }//end packet processor
		   delete packet;
		   packet = 0;
		}//end process packets
	}

	delete [] bufferMisc;
	bufferMisc = 0;

    return 0;
}



