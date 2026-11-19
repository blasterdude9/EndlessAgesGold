#include "StdAfx.h"
#include ".\urlreader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CUrlReader::CUrlReader(void)
{

}

CUrlReader::~CUrlReader(void)
{
}

int CUrlReader::ReadUrl(LPSTR url, CString & returnString)
{
     CString strServerName;
     INTERNET_PORT nPort;
	 CString strObject;
     DWORD dwServiceType;
	 LPCTSTR pszURL = NULL;
	 const TCHAR szHeaders[] = _T("Accept: text/*\r\nUser-Agent: MFC\r\n");
	 DWORD dwHttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_RELOAD;

	 CString buildUrl = url;
	 pszURL = (LPCTSTR)buildUrl;
	 
	 CInternetSession * iSession = new CInternetSession();

     if (!AfxParseURL(pszURL, dwServiceType, strServerName, strObject, nPort) ||
		  dwServiceType != INTERNET_SERVICE_HTTP)
	 {
		  return 2;
	 }
 

	 CHttpConnection* pServer = NULL;
	 CHttpFile* pFile = NULL;
	 try
	 {
		

		pServer = iSession->GetHttpConnection(strServerName, nPort);

		pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
				strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
		 
		pFile->AddRequestHeaders(szHeaders);
		pFile->SendRequest();

	 }
     catch(CInternetException * e)
	 {
        char error[256];
        e->GetErrorMessage(error,255,0);

        return 2;
	 }



	 DWORD dwRet;
		pFile->QueryInfoStatusCode(dwRet);

			if (dwRet != HTTP_STATUS_OK)
			{
				iSession->Close();
	            delete iSession;
	            iSession=0;
				return 3;
			}

	    TCHAR sz[1024];
		ZeroMemory(&sz,1024);
		CString ipadressBuild;

		int index = -2;

		while (pFile->ReadString(sz, 1023))
		{
			   ipadressBuild += sz;
			   index++;
		      
			ZeroMemory(&sz,1024);	  
		}
	 
	 pFile->Close();
	 delete pFile;
	 pFile = 0;
     pServer->Close();
	 delete pServer;
	 pServer = 0;
	 
     iSession->Close();
	 delete iSession;
	 iSession=0;
	 returnString = ipadressBuild;
	 return TRUE;

}
