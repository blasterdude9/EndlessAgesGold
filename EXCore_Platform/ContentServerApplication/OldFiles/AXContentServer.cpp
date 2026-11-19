#include "StdAfx.h"
#include "axcontentserver.h"
#include "axencryptionlib.h"

DWORD WINAPI PacketProcessor(LPVOID lpParameter);

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
	m_threadCount = 1;
	m_encryptor = new CAXEncryptionLib("lucidcatscanarbcowlordarronnicol");
    //connect to database
	m_dataStorage = new CAXFileObjectDB();
	m_eventLog = new CServerEventList();
	m_eventLog->EnableCriticalSection();
	ConnectionStructureClass connectStruct;
	connectStruct.AquireFromCfgFile("contentSvr.cfg");
	
    m_dbConnection = new CDBConnectionSystem(connectStruct.m_sqlServerIP,
		                                     connectStruct.m_sqlServerCatalog,
											 connectStruct.m_sqlServerUsername,
											 connectStruct.m_sqlServerPassword,
											 m_eventLog);
    //Establish Connection
	m_dbConnection->EstablishConnection();
    if(!m_dbConnection->m_connectionEstablished)
	{
		m_sServer->WriteDebugFile("SQL Connection Failed",0,1);
	    return;
	}
    //Clear current connections
    m_dbConnection->ExecQuery("DELETE FROM PTC_CURRENTCONNECTIONS");
    //Aquire TCP server configuration
    CString configQuery = "SELECT * FROM PTC_MAINCONFIG";
    _RecordsetPtr cReader = m_dbConnection->Query(configQuery);
    if(m_dbConnection->GetCount(cReader) > 0)
	{
       int maxConnections	= m_dbConnection->GetIntData("MAXCONNECTIONS",cReader);
	   int tcpPort			= m_dbConnection->GetIntData("TCPPORT",cReader);
	   int blockSize		= m_dbConnection->GetIntData("BLOCKSIZE",cReader);
	   int threadCount		= m_dbConnection->GetIntData("THREADCOUNT",cReader);

       m_sServer = new CAXSocketServer();
	   BOOL result = m_sServer->StartServer(threadCount,tcpPort,blockSize,maxConnections);
	}
	m_dbConnection->Close(cReader);
	//create processing threads
    m_threadHandles = new HANDLE[m_threadCount];
    m_threadIds     = new DWORD[m_threadCount];
	if(m_threadHandles)
    for(int tLoop=0;tLoop<m_threadCount;tLoop++)
	{
	    m_threadHandles[tLoop] = CreateThread(NULL,0,PacketProcessor,this,0,&m_threadIds[tLoop]);
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

	if(m_encryptor)
	   delete m_encryptor;
	m_encryptor = 0;

	if(m_sServer)
	   delete m_sServer;
	m_sServer = 0;

	if(m_dataStorage)
	   delete m_dataStorage;
	m_dataStorage = 0;

	if(m_eventLog)
	{
        m_eventLog->OutPutTextFile("ContentServerEvents.txt");
		m_eventLog->SafeDelete();
		delete m_eventLog;
		m_eventLog = 0;
	}
	DeleteCriticalSection(&m_cSection);
}


bool CAXContentServer::AddConnection(CString ip, long socket)
{
    CString query = "INSERT INTO PTC_CURRENTCONNECTIONS(IP, SOCKET) VALUES('$IP', '$SOCKET')";
    query.Replace("$IP",ip);
	query.Replace("$SOCKET",m_cabLib.ConvertLongToString(socket));
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

bool CAXContentServer::RemoveConnection(CString ip, long socket)
{
    CString query = "DELETE FROM PTC_CURRENTCONNECTIONS WHERE IP = '$IP' AND SOCKET = '$SOCKET'";
    query.Replace("$IP",ip);
	query.Replace("$SOCKET",m_cabLib.ConvertLongToString(socket));
	m_dbConnection->ExecQuery(query);

    return true;
}

DWORD WINAPI PacketProcessor(LPVOID lpParameter)
{
	CAXContentServer * classObj = (CAXContentServer *)lpParameter;

	CAXSsPacket * packet = NULL;
	CString queryAuth = "SELECT * FROM GMACCOUNTBASE WHERE USERNAME = '$USERNAME'";
	CString queryBannedAuth = "SELECT * FROM GMBANNEDINFO WHERE USERNAME = '$USERNAME' OR IP = '$IP'";
	char bufferMisc[1024];
	char bufferMisc2[1024];
	CString formatNameTmp;
	ZeroMemory(bufferMisc,1024);
	ZeroMemory(bufferMisc2,1024);
	CString tempQ;
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
		Sleep(0);
        WaitForSingleObject(classObj->m_sServer->m_dataToProcessEvent,1000);
        //get packets and process by first byte
        if(classObj->m_sServer->m_packetQue->GetNextPacket(&packet)==true)
		{//process packets
           switch(packet->data[0])
		   {//pack processor
               case 0:// Authentication Packet
			      if(1)
				  {
                     AuthPacket * authCast = (AuthPacket *)packet->data;
                     //do authentication here
					 CString tempXd = authCast->username;
					 CString tempXp = authCast->password;
                  	 
					 tempQ = queryAuth;
					 tempQ.Replace("$USERNAME",tempXd);
					 _RecordsetPtr userReader = classObj->m_dbConnection->Query(tempQ);
                     if(classObj->m_dbConnection->GetCount(userReader)==0)
					 {//username does not exist
						classObj->m_dbConnection->Close(userReader);
                        SignalPacket sigPacket;
						sigPacket.type = 6;
					    classObj->m_sServer->SendDataToID(packet->connectionId,(BYTE*)&sigPacket,sizeof(SignalPacket));//send authenicate confirmation
						classObj->m_sServer->WriteDebugFile("User Access Denied(invalid username):",0,0);
						classObj->m_sServer->WriteDebugFile(tempXd.GetBuffer(),0,1);
					 }//end username does not exist
					 else if(tempXp.CompareNoCase(classObj->m_encryptor->DecryptString64(classObj->m_dbConnection->GetStringData("PASSWORD",userReader).GetBuffer()))==0)
					 {//good password
						classObj->m_dbConnection->Close(userReader);
                        //Verify not banned
                        tempQ = queryBannedAuth;
						tempQ.Replace("$USERNAME",tempXd);
						tempQ.Replace("$IP",classObj->m_sServer->GetIPFromConnectionID(packet->connectionId));
						_RecordsetPtr userReader = classObj->m_dbConnection->Query(tempQ);
						if(classObj->m_dbConnection->GetCount(userReader)>0)
						{//banned
                            classObj->m_dbConnection->Close(userReader);
							SignalPacket sigPacket;
							sigPacket.type = 8;
							classObj->m_sServer->SendDataToID(packet->connectionId,(BYTE*)&sigPacket,sizeof(SignalPacket));//send authenicate confirmation
							classObj->m_sServer->WriteDebugFile("User Access Denied(Banned):",0,0);
							classObj->m_sServer->WriteDebugFile(tempXd.GetBuffer(),0,1);

						}//end banned
						else{//not banned
							classObj->m_dbConnection->Close(userReader);
							//end verify not banned
							CString tempIP;
							SOCKET socketTemp;
							if(classObj->m_sServer->Authenticate(packet->connectionId,authCast,tempIP,socketTemp)==true)
							{
									classObj->AuthenticateConnectionInDB(tempIP,
																		socketTemp, 
																		authCast->username);
									classObj->m_sServer->WriteDebugFile("Authenticate Connection to DB",0,1);
									SignalPacket sigPacket;
									sigPacket.type = 5;
									classObj->m_sServer->SendDataToID(packet->connectionId,(BYTE*)&sigPacket,sizeof(SignalPacket));//send authenicate confirmation
									classObj->m_sServer->WriteDebugFile("User Authenticated:",0,0);
									classObj->m_sServer->WriteDebugFile(authCast->username,0,1);
							}
						}//end not banned
					 }//end good password
					 else
					 {//did not authenticate
						        classObj->m_dbConnection->Close(userReader);
                                SignalPacket sigPacket;
								sigPacket.type = 7;
								classObj->m_sServer->SendDataToID(packet->connectionId,(BYTE*)&sigPacket,sizeof(SignalPacket));//send authenicate confirmation
								classObj->m_sServer->WriteDebugFile("User Access Denied(wrong password):",0,0);
						        classObj->m_sServer->WriteDebugFile(tempQ.GetBuffer(),0,1);
					 }//end did not authenticate
					 
				  }
			   break;
		       case 1:// Request File Length
			      if(1)
				  {
                  ZeroMemory(bufferMisc,1024);
				  CopyMemory(bufferMisc,&packet->data[1],packet->length);
				  formatNameTmp = bufferMisc;
                  CAXFileObject * axFileObj = classObj->m_dataStorage->GetFileObjectByName(formatNameTmp);
		
					if(!axFileObj)
					{//request does not exist
						CString configQuery = "SELECT * FROM PTC_AVAILFILES WHERE NAME = '"+formatNameTmp+"'";
						_RecordsetPtr cReader = classObj->m_dbConnection->Query(configQuery);
						if(classObj->m_dbConnection->GetCount(cReader) > 0)
						{
							CString path = classObj->m_dbConnection->GetStringData("FILEPATH",cReader);
                            if(classObj->m_dataStorage->AddFile(formatNameTmp, path)==true)
							{
							   //log addition
                               axFileObj = classObj->m_dataStorage->GetFileObjectByName(formatNameTmp);
							}
						}
						else
						{//log error

						}//end log error
						classObj->m_dbConnection->Close(cReader);
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
               case 2:// Request File Block from Start Point 
					if(packet->length == sizeof(Genheader))
					{//found correct contruct
						Genheader * fileBl = (Genheader *)packet->data;
						CAXFileObject * axFileObj = classObj->m_dataStorage->GetFileIndex(fileBl->shortType);
						if(axFileObj)
						{//aquired data structure send Length packet
							ZeroMemory(bufferMisc,1024);
                          
							long fileStartpoint = fileBl->longType;
							long maxBufSize = 1024 - sizeof(Genheader);
							long remainder = axFileObj->m_fileLength - fileBl->longType;
							if(remainder < maxBufSize)
							   maxBufSize = remainder;

							Genheader newReq;
							newReq.type = 1;
							newReq.longType = maxBufSize;
                            newReq.shortType = fileBl->shortType;
							CopyMemory(&bufferMisc[0],&newReq,sizeof(Genheader));
							if(maxBufSize > 0)
                               CopyMemory(&bufferMisc[sizeof(Genheader)],&axFileObj->m_data[fileStartpoint],maxBufSize);

							
							if(!classObj->m_sServer->SendDataToID(packet->connectionId,(BYTE*)bufferMisc,maxBufSize+sizeof(Genheader)))
							{//log failure (and disconnect user)

							}//end log failure (and disconnect user)
						}//end aquired data structure send Length packet
						else
							classObj->m_sServer->WriteDebugFile("File Block request(File not found)",0,1);
					}//found correct contruct
               break;
			   case 20://disconnect
				   if(1)
				   {
                   CString tempIP;
				   SOCKET socketTemp;
				   if(classObj->m_sServer->GetDataFromConnectionID(packet->connectionId,tempIP,socketTemp)==true)
				   {
                      classObj->RemoveConnection(tempIP, socketTemp);
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
                   CString tempIP;
				   SOCKET socketTemp;
				   if(classObj->m_sServer->GetDataFromConnectionID(packet->connectionId,tempIP,socketTemp)==true)
				   {
                      classObj->AddConnection(tempIP, socketTemp);
					  classObj->m_sServer->WriteDebugFile("Add Base Connection to DB",0,1);
				   }
				   else
				   {
                      
				   }
				   }
			   break;
		   }//end packet processor
		   delete packet;
		   packet = 0;
		}//end process packets
	}

    return 0;
}



