// myservice.cpp
#include "stdafx.h"
#include "direct.h"
#include "myservice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CMyService::CMyService()
:CNTService("AXServerService")
{
	 m_iStartParam = 0;
	m_iIncParam = 1;
	m_iState = m_iStartParam;

	m_axserver = NULL;
}

BOOL CMyService::OnInit()
{
	// Read the registry parameters
    // Try opening the registry key:
    // HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\<AppName>\Parameters
    HKEY hkey;
	char szKey[1024];
	strcpy_s(szKey, "SYSTEM\\CurrentControlSet\\Services\\");
	strcat_s(szKey, m_szServiceName);
	strcat_s(szKey, "\\Parameters");
    if (RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                     szKey,
                     0,
                     KEY_QUERY_VALUE,
                     &hkey) == ERROR_SUCCESS) {
        // Yes we are installed
        DWORD dwType = 0;
        DWORD dwSize = sizeof(m_iStartParam);
        RegQueryValueEx(hkey,
                        "Start",
                        NULL,
                        &dwType,
                        (BYTE*)&m_iStartParam,
                        &dwSize);
        dwSize = sizeof(m_iIncParam);
        RegQueryValueEx(hkey,
                        "Inc",
                        NULL,
                        &dwType,
                        (BYTE*)&m_iIncParam,
                        &dwSize);
        RegCloseKey(hkey);
    }

    //fire up the server
	CoInitialize(NULL);
	
    
    
	// Set the initial state
	m_iState = m_iStartParam;

	return TRUE;
}

void CMyService::Run()
{
    
    while (m_bIsRunning) {

		if(!m_axserver)
		{
            m_workingPath = GetSoftwareRegkeyValue("ImagePath","ImagePath");
            for(int trimLoop=m_workingPath.GetLength()-1;trimLoop>1;trimLoop--)
			{
                  if(m_workingPath.GetAt(trimLoop) == '\\')
				  {
				     break;
				  }
				  m_workingPath.Delete(trimLoop,1);
			}

			SetSoftwareRegistry("Set Beta",m_workingPath.GetBuffer());

			SetCurrentDirectory(m_workingPath);
		    m_axserver = new CServerProc();
			
		}
		// Sleep for a while
        Sleep(1000);
		// Update the current state
		m_iState += m_iIncParam;
    }
}

// Process user control requests
BOOL CMyService::OnUserControl(DWORD dwOpcode)
{
    switch (dwOpcode) {
    case SERVICE_CONTROL_USER + 0:

        // Save the current status in the registry
        SaveStatus();
        return TRUE;

    default:
        break;
    }
    return FALSE; // say not handled
}

void CMyService::OnStop()
{
	 
     if(m_axserver)
	 {
	    SetCurrentDirectory(m_workingPath);
	    delete m_axserver;
        m_axserver = 0;
	 }

}


void CMyService::OnShutdown()
{
	 
     if(m_axserver)
	 {
	    SetCurrentDirectory(m_workingPath);
	    delete m_axserver;
        m_axserver = 0;
	 }

	 CoUninitialize();
}

void CMyService::MakeStatusEntry(CString name,DWORD val)
{
            HKEY hkey = NULL;
	        char szKey[1024];
	        strcpy_s(szKey, "SYSTEM\\CurrentControlSet\\Services\\");
	        strcat_s(szKey, m_szServiceName);
	        strcat_s(szKey, "\\Status");
            DWORD dwDisp;
	        DWORD dwErr;
            DebugMsg("Creating key: %s", szKey);
            dwErr = RegCreateKeyEx(	HKEY_LOCAL_MACHINE,
                           	szKey,
                   			0,
                   			"",
                   			REG_OPTION_NON_VOLATILE,
                   			KEY_WRITE,
                   			NULL,
                   			&hkey,
                   			&dwDisp);
	        if (dwErr == ERROR_SUCCESS) {
		        RegSetValueEx(hkey,
                              name,
                              0,
                              REG_DWORD,
                              (BYTE*)&val,
                              sizeof(DWORD));
			}
}

void CMyService::MakeStatusEntryString(CString name,CString val)
{
            HKEY hkey = NULL;
	        char szKey[1024];
	        strcpy_s(szKey, "SYSTEM\\CurrentControlSet\\Services\\");
	        strcat_s(szKey, m_szServiceName);
	        strcat_s(szKey, "\\Status");
            DWORD dwDisp;
	        DWORD dwErr;
            DebugMsg("Creating key: %s", szKey);
            dwErr = RegCreateKeyEx(	HKEY_LOCAL_MACHINE,
                           	szKey,
                   			0,
                   			"",
                   			REG_OPTION_NON_VOLATILE,
                   			KEY_WRITE,
                   			NULL,
                   			&hkey,
                   			&dwDisp);
	        if (dwErr == ERROR_SUCCESS) {
		        RegSetValueEx(hkey,
                              name,
                              0,
                              REG_EXPAND_SZ,
                              (BYTE*)val.GetBuffer(),
                              name.GetLength()+1);
			}
}
// Save the current status in the registry
void CMyService::SaveStatus()
{
    DebugMsg("Saving current status");
    // Try opening the registry key:
    // HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\<AppName>\...
    HKEY hkey = NULL;
	char szKey[1024];
	strcpy_s(szKey, "SYSTEM\\CurrentControlSet\\Services\\");
	strcat_s(szKey, m_szServiceName);
	strcat_s(szKey, "\\Status");
    DWORD dwDisp;
	DWORD dwErr;
    DebugMsg("Creating key: %s", szKey);
    dwErr = RegCreateKeyEx(	HKEY_LOCAL_MACHINE,
                           	szKey,
                   			0,
                   			"",
                   			REG_OPTION_NON_VOLATILE,
                   			KEY_WRITE,
                   			NULL,
                   			&hkey,
                   			&dwDisp);
	if (dwErr != ERROR_SUCCESS) {
		DebugMsg("Failed to create Status key (%lu)", dwErr);
		return;
	}	

    // Set the registry values
	DebugMsg("Saving 'Current' as %ld", m_iState); 
    RegSetValueEx(hkey,
                  "Current",
                  0,
                  REG_DWORD,
                  (BYTE*)&m_iState,
                  sizeof(m_iState));


    // Finished with key
    RegCloseKey(hkey);

}

CString CMyService::GetSoftwareRegkeyValue(LPSTR headerX,LPSTR valueRequest)
{
	CString header = headerX;
			int BUFSIZE = 500;
            char szProductType[500];
			ZeroMemory(szProductType,BUFSIZE);
            DWORD dwBufLen=BUFSIZE;
            LONG lRet;

			HKEY hKey;
	        char szKey[1024];
	        strcpy_s(szKey, "SYSTEM\\CurrentControlSet\\Services\\");
	        strcat_s(szKey, m_szServiceName);

            lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                                 szKey,
                                 0, 
								 KEY_QUERY_VALUE, 
								 &hKey );
            
			if( lRet != ERROR_SUCCESS )
               return "";

            lRet = RegQueryValueEx( hKey, valueRequest, NULL, NULL,
                                 (LPBYTE) szProductType, &dwBufLen);
            if( (lRet != ERROR_SUCCESS) || (dwBufLen > (DWORD)BUFSIZE) )
               return "";

            RegCloseKey( hKey );

			CString ret = szProductType;
            return ret;
}

void CMyService::SetSoftwareRegistry(LPSTR valueName,LPSTR valueDataX)
{
   HKEY hk;
	        char szKey[1024];
	        strcpy_s(szKey, "SYSTEM\\CurrentControlSet\\Services\\");
	        strcat_s(szKey, m_szServiceName);
			strcat_s(szKey, "\\Status");
   CString valueData = valueDataX;

   if(RegCreateKey(HKEY_LOCAL_MACHINE, 
                   szKey,
				   &hk))
   {
        
   }

   if(valueData.GetLength() < 1)
      return;

   char * szBuf = new char[valueData.GetLength()+1];
   ZeroMemory(szBuf,valueData.GetLength()+1);

   for(int i=0;i<valueData.GetLength();i++)
   {
       szBuf[i] = valueData.GetAt(i);
   }

   if (RegSetValueEx(hk,             // subkey handle 
                     valueName,       // value name 
                     0,                        // must be zero 
                     REG_EXPAND_SZ,            // value type 
                     (LPBYTE) szBuf,           // pointer to value data 
                     (DWORD)(strlen(szBuf) + 1)))       // length of value data 
   {

   }


   delete [] szBuf;
   szBuf = 0;

}
