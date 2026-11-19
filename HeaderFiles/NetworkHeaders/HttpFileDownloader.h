#include <afxinet.h>
#include "Wininet.h"
#include "directoryregistrywrapper.h"
#pragma once

class CHttpFileDownloader
{
public:
	/*CHttpFileDownloader(LPSTR sessionName,
					    LPSTR savePath,
					    LPSTR url);*/
	CHttpFileDownloader();
    BOOL       DwldFile(LPSTR sessionName,
					    LPSTR savePath,
					    LPSTR url,
					    BOOL contFlag,
						int * progressDone,
						CRITICAL_SECTION * sectionC);

	~CHttpFileDownloader(void);
 
	//Variables
	CDirectoryRegistryWrapper m_dirRegWrp;
    CInternetSession * m_session;
	BOOL m_systemsAllOK;
	DWORD m_dwRead;
	HINTERNET m_hINet;
	HINTERNET m_hConnection;
	HINTERNET m_hData;
	//CHttpFile* pFile;
	//CStdioFile * pFile;
	long m_dataSz;
    long m_traceProgress;
    long m_totalProgLength;
    long m_totalBytesDownloaded;
	BYTE *szBuff;
	HANDLE m_hFile; 
	CFile * m_testGenFile;
	long m_chunkSize;
	BYTE m_tempChunk[256];
    CString m_lastError;
	BOOL m_asciMode;



	//Functions
	DWORD GetUrlByteSize(LPSTR sessionName,
					     LPSTR url);
	long GetPercentageCompleted();
	int DownloadChunk(BYTE * buffer);
	BOOL                  DownloadFileINET(     LPSTR sessionName,
												LPSTR savePath,
												LPSTR url,
											    int * progress,
												int * totalMegs,
												DWORD * kbDld,
												BOOL * abort,
												CRITICAL_SECTION * sectionC);
	BOOL DownloadFileToMemINET(    LPSTR sessionName,
													LPSTR url,
													BYTE ** buffer,
													DWORD * bufReturnLength
												);
	BOOL GetUrlData(LPSTR sessionName,
								      LPSTR url,
									  BYTE ** buffer,
								      DWORD * bufReturnLength);
	BOOL GetUrlDataLocked(LPSTR sessionName,
								           LPSTR url,
									       BYTE ** buffer,
								           DWORD * bufReturnLength,
										   int * currentDownloadProgress,
										   CRITICAL_SECTION * sectionC);
};
