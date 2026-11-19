#include "StdAfx.h"
#include "direct.h"
#include ".\httpfiledownloader.h"
#include "common.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CHttpFileDownloader::CHttpFileDownloader()
{
	m_testGenFile = NULL;
    m_hConnection = NULL;
	szBuff = NULL;
	m_hINet = NULL;
	m_hData = NULL;
	m_asciMode = FALSE;
}

CHttpFileDownloader::~CHttpFileDownloader(void)
{
	if(m_testGenFile)
	{
       m_testGenFile->Close();
       delete m_testGenFile;
	   m_testGenFile = 0;
	}
   
	if(szBuff)
	{
       delete [] szBuff;
	   szBuff = 0;
	}

	if(m_hConnection)
	   InternetCloseHandle(m_hConnection);
	if(m_hINet)
       InternetCloseHandle(m_hINet);
	if(m_hData)
	   InternetCloseHandle(m_hData);
	
}

BOOL CHttpFileDownloader::GetUrlData(LPSTR sessionName,
								      LPSTR url,
									  BYTE ** buffer,
								      DWORD * bufReturnLength)
{
    CInternetSession * iSession = new CInternetSession(sessionName);
    //DWORD tempLength = 0;
	BYTE * localBuffer = NULL;
	CStdioFile * tempFile = NULL;
	try
	{
		if(!m_asciMode)
		    tempFile			= iSession->OpenURL(url,
												    1,
												    INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_RESYNCHRONIZE,
												    NULL,
												    NULL);
		else
            tempFile			= iSession->OpenURL(url,
												    1,
												    INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_RESYNCHRONIZE,
												    NULL,
												    NULL);

		if(!tempFile)
		{
			m_lastError = "File is zero, request failed";
			SafeDelete(iSession);
			return 0;
		}

        *bufReturnLength = (DWORD)tempFile->SeekToEnd();
		tempFile->SeekToBegin();

        if(*bufReturnLength == 0)
		{
			SafeDelete(iSession);
			return 0;
		}

		localBuffer = new BYTE[*bufReturnLength+1];
		ZeroMemory(localBuffer,*bufReturnLength+1);

        DWORD readPos = 0;
		DWORD bytesRead = 0;
		BYTE bufferTemp[1024];
        while(1)
		{
			bytesRead = tempFile->Read(bufferTemp,1024);
            if(bytesRead > 0)
			   CopyMemory(&localBuffer[readPos],bufferTemp,bytesRead);
			
			readPos += bytesRead;

			

			if(bytesRead >= *bufReturnLength || bytesRead == 0)
			   break;
		}

		tempFile->Close();
		*buffer = localBuffer;
		delete tempFile;
		tempFile = 0;
		iSession->Close();
	}
	catch(CInternetException * e)
	{
		char error[256];
        e->GetErrorMessage(error,255,0);
		m_lastError += error;

        if(localBuffer)
		{
           delete [] localBuffer;
		   localBuffer = 0;
		   *buffer = NULL;
		}
 
		if(tempFile)
		{
           tempFile->Close();
		   delete tempFile;
		   tempFile = 0;
		}

		if(iSession)
		{
		   delete iSession;
	       iSession = 0;
		}



        return 0;
	}

	SafeDelete(iSession);

	return TRUE;
}

BOOL CHttpFileDownloader::GetUrlDataLocked(LPSTR sessionName,
								           LPSTR url,
									       BYTE ** buffer,
								           DWORD * bufReturnLength,
										   int * currentDownloadProgress,
										   CRITICAL_SECTION * sectionC)
{
    CInternetSession * iSession = new CInternetSession(sessionName);
    //DWORD tempLength = 0;
	BYTE * localBuffer = NULL;
	CStdioFile * tempFile = NULL;
	try
	{
		if(!m_asciMode)
		    tempFile			= iSession->OpenURL(url,
												    1,
												    INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_RESYNCHRONIZE,
												    NULL,
												    NULL);
		else
            tempFile			= iSession->OpenURL(url,
												    1,
												    INTERNET_FLAG_TRANSFER_ASCII | INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_RESYNCHRONIZE,
												    NULL,
												    NULL);

		if(!tempFile)
		{
			m_lastError = "File is zero, request failed";
			SafeDelete(iSession);
			return 0;
		}

        *bufReturnLength = (DWORD)tempFile->SeekToEnd();
		tempFile->SeekToBegin();

        if(*bufReturnLength == 0)
		{
			SafeDelete(iSession);
			return 0;
		}

		localBuffer = new BYTE[*bufReturnLength+1];
		ZeroMemory(localBuffer,*bufReturnLength+1);

        DWORD readPos = 0;
		DWORD bytesRead = 0;
		EnterCriticalSection(sectionC);
		*currentDownloadProgress = 0;
		LeaveCriticalSection(sectionC);
		BYTE bufferTemp[1024];
		DWORD lengthLoc = *bufReturnLength;
        while(1)
		{
			bytesRead = tempFile->Read(bufferTemp,1024);
            if(bytesRead > 0)
			   CopyMemory(&localBuffer[readPos],bufferTemp,bytesRead);
			
			readPos += bytesRead;
			EnterCriticalSection(sectionC);
			*currentDownloadProgress = (int)((((double)(readPos/1000))/((double)(lengthLoc/1000)))*100.0);
			LeaveCriticalSection(sectionC);
			

			if(bytesRead >= *bufReturnLength || bytesRead == 0)
			{
			   EnterCriticalSection(sectionC);
			   *currentDownloadProgress = 100;
			   LeaveCriticalSection(sectionC);
			   break;
			}
		}

		tempFile->Close();
		*buffer = localBuffer;
		delete tempFile;
		tempFile = 0;
		iSession->Close();
	}
	catch(CInternetException * e)
	{
        char error[256];
        e->GetErrorMessage(error,255,0);
		m_lastError += error;
		
        if(localBuffer)
		{
           delete [] localBuffer;
		   localBuffer = 0;
		   *buffer = NULL;
		}
 
		if(tempFile)
		{
           tempFile->Close();
		   delete tempFile;
		   tempFile = 0;
		}

		if(iSession)
		{
		   delete iSession;
	       iSession = 0;
		}

        return 0;
	}
	SafeDelete(iSession);

	return TRUE;
}

DWORD CHttpFileDownloader::GetUrlByteSize(LPSTR sessionName,
										  LPSTR url)
{
    CInternetSession * iSession = new CInternetSession("Url querry");
    DWORD tempLength = 0;
	try
	{
		CStdioFile * tempFile = iSession->OpenURL(url,
												1,
												INTERNET_FLAG_TRANSFER_BINARY | INTERNET_FLAG_EXISTING_CONNECT,
												NULL,
												NULL);
		if(!tempFile)
		{
			SafeDelete(iSession);
			return 0;
		}

		tempLength = (DWORD)tempFile->SeekToEnd();
		tempFile->Close();
		delete tempFile;
		tempFile = 0;
		iSession->Close();
	}
	catch(CInternetException * e)
	{
        char error[256];
        e->GetErrorMessage(error,255,0);
		m_lastError += error;
	}
	SafeDelete(iSession);

	return tempLength;
}



BOOL CHttpFileDownloader::DownloadFileToMemINET(    LPSTR sessionName,
													LPSTR url,
													BYTE ** buffer,
													DWORD * bufReturnLength
												)
{  
    DWORD bytes = GetUrlByteSize(sessionName,url);
    if(bytes < 1)
	   return FALSE;

	//return FALSE;

	*bufReturnLength = bytes;
    BYTE * localData = new BYTE[bytes];
	ZeroMemory(localData,bytes);
	

	m_dwRead = 0;
    m_systemsAllOK = FALSE;
    //open web location
	m_hFile = NULL;
	m_testGenFile = NULL;

    //parse
    CString strServerName;
    INTERNET_PORT nPort;
	CString strObject;
    DWORD dwServiceType;
	LPCTSTR pszURL = NULL;
	pszURL = (LPCTSTR)url;
	if (!AfxParseURL(pszURL, dwServiceType, strServerName, strObject, nPort) ||
		  dwServiceType != INTERNET_SERVICE_HTTP)
	{
		  return FALSE;
	}
	 //end parse
    m_hINet = InternetOpen("patcher", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );

    if ( !m_hINet )
    {
       return FALSE;
    }
	
	m_hConnection = InternetConnect( m_hINet, strServerName, 80, " "," ", INTERNET_SERVICE_HTTP, 0, 0 );
    if ( !m_hConnection )
    {
          InternetCloseHandle(m_hINet);
          return FALSE;
    }

    //Get data
    m_hData = HttpOpenRequest( m_hConnection, "GET", strObject, NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_RESYNCHRONIZE, 0 );

    if ( !m_hData )
    {
          InternetCloseHandle(m_hConnection);
          InternetCloseHandle(m_hINet);
          return FALSE;
    }

	if(!HttpSendRequest( m_hData, NULL, 0, NULL, 0))
	    return FALSE;

    m_dataSz						= 1;
    m_traceProgress					= 0;
    m_totalProgLength				= bytes;
    m_totalBytesDownloaded			= 0;
	m_systemsAllOK = TRUE;
	m_chunkSize = 255;
	
	*buffer = localData;
	 return TRUE;
}

int CHttpFileDownloader::DownloadChunk(BYTE * buffer)
{
	
           InternetReadFile( m_hData, m_tempChunk, m_chunkSize, &m_dwRead );

           if ( m_dwRead == 0)
		   {
			   if(m_traceProgress >= m_totalProgLength)
		          return 2;
               return 0;
		   }

           m_totalBytesDownloaded += m_dwRead;
		   CopyMemory(&buffer[m_traceProgress],m_tempChunk,m_dwRead);
		   m_traceProgress += m_dwRead;


           if(m_traceProgress >= m_totalProgLength)
		      return 2;

		   return 1;
}

BOOL CHttpFileDownloader::DownloadFileINET(     LPSTR sessionName,
												LPSTR savePath,
												LPSTR url,
											    int * progress,
												int * totalMegs,
												DWORD * kbDld,
												BOOL * abort,
												CRITICAL_SECTION * sectionC
												)
{  

	DWORD bytes = GetUrlByteSize(sessionName,url);
	m_dwRead = 0;
    m_systemsAllOK = FALSE;
    //open web location
	m_hFile = NULL;
	m_testGenFile = NULL;

    //parse
    CString strServerName;
    INTERNET_PORT nPort;
	CString strObject;
    DWORD dwServiceType;
	LPCTSTR pszURL = NULL;
	pszURL = (LPCTSTR)url;
	if (!AfxParseURL(pszURL, dwServiceType, strServerName, strObject, nPort) ||
		  dwServiceType != INTERNET_SERVICE_HTTP)
	{
		  return FALSE;
	}
	 //end parse
    m_hINet = InternetOpen("patcher", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0 );

    if ( !m_hINet )
    {
       return FALSE;
    }
	
	m_hConnection = InternetConnect( m_hINet, strServerName, 80, " "," ", INTERNET_SERVICE_HTTP, 0, 0 );
    if ( !m_hConnection )
    {
          InternetCloseHandle(m_hINet);
          return FALSE;
    }

    //Get data
    m_hData = HttpOpenRequest( m_hConnection, "GET", strObject, NULL, NULL, NULL, INTERNET_FLAG_KEEP_CONNECTION | INTERNET_FLAG_RESYNCHRONIZE, 0 );
	//| INTERNET_FLAG_RELOAD | INTERNET_FLAG_NO_CACHE_WRITE

    if ( !m_hData )
    {
          InternetCloseHandle(m_hConnection);
          InternetCloseHandle(m_hINet);
          return FALSE;
    }

	if(!HttpSendRequest( m_hData, NULL, 0, NULL, 0))
	    return FALSE;

   

	if(sectionC)
	{
	EnterCriticalSection( sectionC);
    if(totalMegs)
	   *totalMegs = (int)(bytes/1000000);
	LeaveCriticalSection( sectionC);
	}


    m_dataSz						= 1;
    m_traceProgress					= 0;
    m_totalProgLength				= bytes;
    m_totalBytesDownloaded			= 0;
	m_systemsAllOK = TRUE;

	m_chunkSize = 255;

	m_hFile = CreateFile(savePath,
                         GENERIC_WRITE, FILE_SHARE_READ,
                         NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	//CREATE_ALWAYS

     if (m_hFile == INVALID_HANDLE_VALUE)
     {
       //parse directory
       m_dirRegWrp.MakeDirectory(savePath);

	   m_hFile = CreateFile(savePath,
                            GENERIC_WRITE, FILE_SHARE_READ,
                            NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	   if(m_hFile == INVALID_HANDLE_VALUE)
	      return FALSE;
     }
	
	 m_testGenFile = new CFile(m_hFile);



	szBuff = new BYTE[m_chunkSize];

	while(1)
    {
		   InternetReadFile( m_hData, szBuff, m_chunkSize, &m_dwRead );
           if ( m_dwRead == 0)
           {
		      if(m_totalBytesDownloaded < m_totalProgLength)
			  {
                 DWORD error = GetLastError();

                 //Sleep(1);
				 
			  }

              break;
           }

		   EnterCriticalSection( sectionC);
		   if(*abort)
		   {
		      LeaveCriticalSection( sectionC);
		      break;
		   }
		   LeaveCriticalSection( sectionC);

		   //m_testGenFile->Write( szBuff,m_dwRead);
           m_totalBytesDownloaded += m_dwRead;
		   if(sectionC)
		   {
				EnterCriticalSection( sectionC);
				if(progress)
				*progress = GetPercentageCompleted();
				if(kbDld)
					*kbDld = m_totalBytesDownloaded/1000;
				LeaveCriticalSection( sectionC);
				m_testGenFile->Write( szBuff,m_dwRead);
		   }
		   m_traceProgress++;
     }

	 
	 InternetCloseHandle(m_hConnection);
     InternetCloseHandle(m_hINet);
	 InternetCloseHandle(m_hData);
	
     delete szBuff;
	 szBuff = 0;

	 m_testGenFile->Close();
	 m_hFile = 0;
     delete m_testGenFile;
	 m_testGenFile = 0;
	 
	 return TRUE;
}




long CHttpFileDownloader::GetPercentageCompleted()
{
     if(m_totalBytesDownloaded == m_totalProgLength)
	    return 100;

     return (long)((((double)(m_totalBytesDownloaded/1000))/((double)(m_totalProgLength/1000)))*100.0);
}


