#include "StdAfx.h"
#include ".\axcontentclient.h"

DWORD WINAPI ClientPacketProcessor(LPVOID lpParameter);

int CAXContentClient::DownloadFileRequest(CString fileName, CString ip, int port)
{
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

DWORD WINAPI ClientPacketProcessor(LPVOID lpParameter)
{
	CAXContentClient * classObj = (CAXContentClient *)lpParameter;
	
	/*if(classObj->m_clientSocket)
	   delete classObj->m_clientSocket;
	classObj->m_clientSocket = 0;*/
	
	CAXSocketClient * clientSocket = new CAXSocketClient();
	int success                 = clientSocket->Connect(  classObj->m_ip.GetBuffer(),
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
	   return 0;
	}
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
		classObj->m_loggingSystem.WriteDebugFile("65372 Couldn't create the file!", 0, 1);
		return 0;
	}
	else
	{
		fileObject = new CFile(hFile);
		classObj->m_dowloadPosition = fileObject->SeekToEnd();
	}

    
    char * message = NULL;
    BOOL aquiredSize = FALSE;
	CAXSsPacket * pcPtr = NULL;
	fileCRC * fcrcTemp = NULL;
	Genheader * genHeader = NULL;
	Genheader gHeader;
    while(1)
	{//send and recieve until completed


		ResetEvent(clientSocket->m_dataToProcessEvent);
		Sleep(0);
        WaitForSingleObject(clientSocket->m_dataToProcessEvent,1000);

        if(clientSocket->m_packetQue->GetNextPacket(&pcPtr)==true)
		{
			
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
			   case 1:// file block to append
				    //classObj->m_loggingSystem.WriteDebugFile("Recieved File Block", 0, 1);
				    genHeader = (Genheader*)pcPtr->data;
                    fileObject->Write(&pcPtr->data[sizeof(Genheader)],pcPtr->length-sizeof(Genheader));
					EnterCriticalSection(&classObj->m_mSect);
					classObj->m_dowloadPosition += pcPtr->length-sizeof(Genheader);
					LeaveCriticalSection(&classObj->m_mSect);
					if(classObj->m_dowloadPosition < classObj->m_downloadLength)
					{
						//request more if needed
						gHeader.type = 2;//request file block from start point
						gHeader.longType = classObj->m_dowloadPosition;//start point
						gHeader.shortType = genHeader->shortType;
						clientSocket->SendData((char*)&gHeader,sizeof(Genheader));
					}
					else
					{//purpose completed
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
                           return 0;
						}
					}//end purpose completed
			   break;
			   case 2:// file info  CRC and size
                    fcrcTemp = (fileCRC*)pcPtr->data;
					EnterCriticalSection(&classObj->m_mSect);
					classObj->m_downloadLength = fcrcTemp->length;
					LeaveCriticalSection(&classObj->m_mSect);
					classObj->m_loggingSystem.WriteDebugFile("Recieved File length and CRC:", 0, 0);
					classObj->m_loggingSystem.WriteDebugFile(fcrcTemp->length, 0, 1);
					//request file blocks
					gHeader.type = 2;//request file block from start point
					gHeader.longType = classObj->m_dowloadPosition;//start point
					gHeader.shortType = fcrcTemp->fileIndex;
					clientSocket->SendData((char*)&gHeader,sizeof(Genheader));
			   break;
			}//pack processor
			delete pcPtr;
			pcPtr = 0;
		}

	}//end send and recieve until completed

	
    if(clientSocket)
	{
       delete clientSocket;
	   clientSocket = 0;
	}

	return 0;
}