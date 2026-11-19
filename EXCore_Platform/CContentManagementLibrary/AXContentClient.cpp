#include "StdAfx.h"
#include ".\axcontentclient.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

DWORD WINAPI ClientPacketProcessor(LPVOID lpParameter);

int CAXContentClient::DownloadFileRequest(CString fileName, CString ip, int port)
{
    m_loggingSystem.WriteDebugFile("DownloadFileRequest() attempt", 0, 1);
	//m_clientSocket = NULL;
    if(m_connected == true)
	   return 5;//already connected

    if(m_downloadInProgress == true)
	   return 6;//already downloading

	m_ip					= ip;
	m_port					= port;
	m_fileNameToDownload	= fileName;

    m_downloadInProgress = true;
	m_procThreadHandle   = CreateThread(NULL,0,ClientPacketProcessor,this,0,&m_procThreadID);

	m_loggingSystem.WriteDebugFile("Download processor thread started", 0, 1);
	
	return 1;
}

void CAXContentClient::Abort()
{
     EnterCriticalSection(&m_abortSect);
     m_abort = TRUE;
     LeaveCriticalSection(&m_abortSect);
}

void CAXContentClient::WaitForThreadToClose()
{
     while(1)
	 {
        EnterCriticalSection(&m_abortSect);
        if(m_threadClosed == 1)
		{
           LeaveCriticalSection(&m_abortSect);
           return;
		}
        LeaveCriticalSection(&m_abortSect);
		Sleep(200);
	 }
}

DWORD WINAPI ClientPacketProcessor(LPVOID lpParameter)
{
	CAXContentClient * classObj = (CAXContentClient *)lpParameter;
	classObj->m_loggingSystem.WriteDebugFile("Thread spinning up", 0, 0);
	EnterCriticalSection(&classObj->m_abortSect);
	classObj->m_threadClosed = 0;
	LeaveCriticalSection(&classObj->m_abortSect);
	/*if(classObj->m_clientSocket)
	   delete classObj->m_clientSocket;
	classObj->m_clientSocket = 0;*/
	
	//classObj->m_errorMessage = "Failed trying to connect out to patch server";
	CAXSocketClient * clientSocket = new CAXSocketClient();
	int success                 = clientSocket->Connect(    classObj->m_ip.GetBuffer(),
															classObj->m_port,
															classObj->m_username.GetBuffer(),
															classObj->m_password.GetBuffer());
    if(success == 0)
	{
       classObj->m_connected = false;
       classObj->m_downloadInProgress = false;
	   delete clientSocket;
	   clientSocket = 0;
	   classObj->m_loggingSystem.WriteDebugFile("cannot connect to specified server", 0, 1);
	   classObj->m_errorMessage = "Cannot Connect To Patch Server";
	   return 0;
	}
	classObj->m_errorMessage = "";
	classObj->m_loggingSystem.WriteDebugFile("logon ok:", 0, 0);
	classObj->m_loggingSystem.WriteDebugFile(classObj->m_username.GetBuffer(), 0, 0);
	classObj->m_loggingSystem.WriteDebugFile(" ip:", 0, 0);
	classObj->m_loggingSystem.WriteDebugFile(classObj->m_ip.GetBuffer(), 0, 0);
	classObj->m_loggingSystem.WriteDebugFile(" Port:", 0, 0);
	classObj->m_loggingSystem.WriteDebugFile(classObj->m_port, 0, 1);
    //see if exists in cache then open file handle or create a new file handle in cache
	CString fileApath = classObj->m_cacheDirectory+classObj->m_fileNameToDownload;
    HANDLE hFile = CreateFile(_T(fileApath.GetBuffer()),
                                 GENERIC_WRITE, FILE_SHARE_READ,
                                 NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	CFile * fileObject = NULL;
	if (hFile == INVALID_HANDLE_VALUE)
	{
		classObj->m_connected = false;
        classObj->m_downloadInProgress = false;
		delete clientSocket;
	    clientSocket = 0;
		classObj->m_loggingSystem.WriteDebugFile("65372 Couldn't create the file!:", 0, 0);
		classObj->m_loggingSystem.WriteDebugFile(fileApath.GetBuffer(), 0, 1);
		classObj->m_errorMessage = "Cannot Access Game Directory";
		return 0;
	}
	
        classObj->m_loggingSystem.WriteDebugFile("logon ok:", 0, 0);
        classObj->m_loggingSystem.WriteDebugFile(fileApath.GetBuffer(), 0, 1);
		

		fileObject = new CFile(hFile);
		classObj->m_dowloadPosition = (long)fileObject->SeekToEnd();

    
    char * message = NULL;
    BOOL aquiredSize = FALSE;
	CAXSsPacket * pcPtr = NULL;
	fileCRC * fcrcTemp = NULL;
	Genheader * genHeader = NULL;
	Genheader gHeader;
    while(1)
	{//send and recieve until completed
        
		WaitForSingleObject(clientSocket->m_dataToProcessEvent,50);

		EnterCriticalSection(&classObj->m_abortSect);
		if(classObj->m_abort)
		{
		   LeaveCriticalSection(&classObj->m_abortSect);
		   break;
		}
		LeaveCriticalSection(&classObj->m_abortSect);

        if(clientSocket->m_packetQue->GetNextPacket(&pcPtr)==true)
		{
			
            
			if(classObj->m_fileEngaged)
			{
						EnterCriticalSection(&classObj->m_mSect);
						classObj->m_dowloadPosition += pcPtr->length;
						LeaveCriticalSection(&classObj->m_mSect);
						if(classObj->m_dowloadPosition < classObj->m_downloadLength)
						{
							fileObject->Write(pcPtr->data,pcPtr->length);
						}
						else
						{//purpose completed
							//WRITE DATA TO FILE
							fileObject->Write(pcPtr->data,pcPtr->length);
							//END WRITE DATA TO FILE


							classObj->m_loggingSystem.WriteDebugFile("File Download complete", 0, 1);
							if(fileObject)
							{
							fileObject->Close();
							delete fileObject;
							fileObject = 0;
							delete clientSocket;
							clientSocket = 0;
							delete pcPtr;
							pcPtr = 0;
							classObj->m_connected = false;
							classObj->m_downloadInProgress = false;
							classObj->m_fileEngaged = false;
							//classObj->m_dowloadPosition = 0;
							return 0;
							}
						}//end purpose complete
			}
			else
			{//parse normally
			
				switch(pcPtr->data[0])
				{//pack processor
				case 5:// Authentication Confirmation
						classObj->m_loggingSystem.WriteDebugFile("Authentication success request file", 0, 1);

						//aquire file length from server
						message = new char[classObj->m_fileNameToDownload.GetLength()+2];
						ZeroMemory(message,classObj->m_fileNameToDownload.GetLength()+2);
						message[0] = 1;
						CopyMemory(&message[1],classObj->m_fileNameToDownload.GetBuffer(),classObj->m_fileNameToDownload.GetLength());
						clientSocket->SendData(message,classObj->m_fileNameToDownload.GetLength()+2);
						delete [] message;
						message = 0;
					    
				break;
				case 6:// Authentication Denied
						classObj->m_loggingSystem.WriteDebugFile("Authentication Denied (invalid username)", 0, 1);

						//aquire file length from server
						classObj->m_accessDenied = 1;
					    
				break;
				case 7:// Authentication Denied
						classObj->m_loggingSystem.WriteDebugFile("Authentication Denied (invalid Password)", 0, 1);

						//aquire file length from server
						classObj->m_accessDenied = 2;
					    
				break;
				case 8:// Authentication Denied
						classObj->m_loggingSystem.WriteDebugFile("Authentication Denied (Banned)", 0, 1);

						//aquire file length from server
						classObj->m_accessDenied = 3;
					    
				break;
				case 9:// Authentication Denied
						classObj->m_loggingSystem.WriteDebugFile("Server is busy, try again later.", 0, 1);

						//aquire file length from server
						classObj->m_accessDenied = 4;
						
						classObj->m_errorMessage = "Server is busy, please try again later.";
				break;
				case 1:// file block to append
				
					
				break;
				case 2:// file info  CRC and size
						fcrcTemp = (fileCRC*)pcPtr->data;
						EnterCriticalSection(&classObj->m_mSect);
						classObj->m_downloadLength = fcrcTemp->length;
						if(classObj->m_dowloadPosition >= classObj->m_downloadLength)
						{//delete and recreate
							fileObject->Close();
							delete fileObject;
							fileObject = 0;
							classObj->m_dowloadPosition = 0;
							HANDLE hFileX = CreateFile(_T(fileApath.GetBuffer()),
							GENERIC_WRITE, FILE_SHARE_READ,
									NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
							if (hFileX == INVALID_HANDLE_VALUE)
							{
								classObj->m_connected = false;
								classObj->m_downloadInProgress = false;
								classObj->m_fileEngaged = false;
								delete clientSocket;
								clientSocket = 0;
								classObj->m_loggingSystem.WriteDebugFile("65372 Couldn't create the file!", 0, 1);
								return 0;
							}
							else
							{
								fileObject = new CFile(hFileX);
								classObj->m_dowloadPosition = (long)fileObject->SeekToEnd();
							}


						}//end delete and restart
						LeaveCriticalSection(&classObj->m_mSect);
						
						
						
						classObj->m_loggingSystem.WriteDebugFile("Recieved File length and CRC:", 0, 0);
						classObj->m_loggingSystem.WriteDebugFile(fcrcTemp->length, 0, 1);
						//request file blocks
						//can start a chain here
						gHeader.type = 2;//request file block from start point
						gHeader.longType = classObj->m_dowloadPosition;//start point
						gHeader.shortType = fcrcTemp->fileIndex;
						if(clientSocket->SendData((char*)&gHeader,sizeof(Genheader))==false)
						{
                           classObj->m_loggingSystem.WriteDebugFile("Send Data Failed", 0, 1);
						}
						classObj->m_fileEngaged = true;
						
				break;
				}//pack processor


			}
			

			delete pcPtr;
			pcPtr = 0;
		}
		else
		    ResetEvent(clientSocket->m_dataToProcessEvent);

	}//end send and recieve until completed

	
    if(clientSocket)
	{
	   if(fileObject)
	   {
	      fileObject->Close();
		  delete fileObject;
		  fileObject = 0;
	   }
       delete clientSocket;
	   clientSocket = 0;
	}

	EnterCriticalSection(&classObj->m_abortSect);
	classObj->m_threadClosed = 1;
	LeaveCriticalSection(&classObj->m_abortSect);

	return 0;
}