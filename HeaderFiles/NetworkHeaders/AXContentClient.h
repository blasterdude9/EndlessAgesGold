#include "AXSocketClient.h"
#include "loggingclass.h"
#pragma once

class CAXContentClient
{
public:
	CAXContentClient(void)
	{
		m_loggingSystem.SetDetails(1,"ClientLog.txt");
	    m_loggingSystem.WriteDebugFile("Starting contentClient", 1, 1);
		InitializeCriticalSection(&m_mSect);
		InitializeCriticalSection(&m_abortSect);
		
	    m_connected				= false;
	    m_downloadInProgress    = false;
	    m_dowloadPosition       = 0;
		m_downloadLength        = 0;
		m_procThreadHandle      = 0;
	    m_procThreadID          = 0;
		m_lastTrackTimeStamp    = GetTickCount();
		m_lastKBTrackPoint      = 0;
		m_lastKbAvg             = 0;
		m_accessDenied          = 0;
		m_curChainCount         = 0;
		m_abort                 = 0;
		m_threadClosed          = 1;
		m_fileEngaged           = false;

	}
	~CAXContentClient(void)
	{
		if(m_procThreadHandle)
           CloseHandle(m_procThreadHandle);
        /*if(m_clientSocket)
		   delete m_clientSocket;
		m_clientSocket = 0;*/
   
   
	    DeleteCriticalSection(&m_mSect);
		DeleteCriticalSection(&m_abortSect);
	}

    //VARS
	bool              m_fileEngaged;
	BOOL              m_accessDenied;
	CLoggingClass     m_loggingSystem;
	CString           m_cacheDirectory;
	CString           m_fileNameToDownload;
	CString           m_errorMessage;
	CString           m_ip;
	CString           m_username;
	CString           m_password;
	int               m_port;
	bool			  m_connected;
	bool              m_downloadInProgress;
	long              m_dowloadPosition;
	long			  m_downloadLength;
	HANDLE            m_procThreadHandle;
	DWORD             m_procThreadID;
	CRITICAL_SECTION  m_mSect;
	//CAXSocketClient * m_clientSocket;
	DWORD             m_lastTrackTimeStamp;
	long              m_lastKBTrackPoint;
	long              m_lastKbAvg;
	int               m_curChainCount;
	BOOL              m_abort;
	int               m_threadClosed;
	CRITICAL_SECTION  m_abortSect;


    //FUNCTIONS
	void WaitForThreadToClose();
	int  DownloadFileRequest(CString fileName, CString ip, int port);
	void SetCacheDirectory(CString dir)
	{
         m_cacheDirectory = dir;
	}
	void SetUsernamePassword(CString user, CString pass)
	{
         m_username = user;
		 m_password = pass;
	}

	void Abort();

	int GetKBSec()
	{
        EnterCriticalSection(&m_mSect);
        DWORD durationPassed = GetTickCount()-m_lastTrackTimeStamp;
		if(durationPassed < 1000)
		{
		   LeaveCriticalSection(&m_mSect);
		   return m_lastKbAvg;
		}

		DWORD passedOne = durationPassed/1000;
		DWORD kbsSinceLastStamp = m_dowloadPosition - m_lastKBTrackPoint;
		if(kbsSinceLastStamp == 0)
		{
		   m_lastKbAvg = 0;
		}
		else if(passedOne == 0)
		{
		   m_lastKbAvg = 0;
		}
		else
          m_lastKbAvg = (kbsSinceLastStamp / passedOne)/1000;


        m_lastKBTrackPoint      = m_dowloadPosition;
		m_lastTrackTimeStamp    = GetTickCount();
		LeaveCriticalSection(&m_mSect);
		return m_lastKbAvg;

	}

	void Reset()
	{
		m_dowloadPosition = 0;
        m_downloadInProgress = false;
		m_fileEngaged = false;
	}

	int GetPercentageOfDownload()
	{
		 int percentage = 0;
		 if(m_downloadLength < 1)
		    return 0;

         EnterCriticalSection(&m_mSect);
		 percentage = (int)((((double)m_dowloadPosition)/((double)m_downloadLength))*100.0);
		 LeaveCriticalSection(&m_mSect);
         return percentage;
	}
	
};
