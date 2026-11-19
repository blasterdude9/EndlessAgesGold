// myservice.cpp
#include "stdafx.h"
#include "direct.h"
#include "myservice.h"

CMyService::CMyService()
:CNTService("ServiceController")
{
	m_iStartParam = 0;
	m_iIncParam = 1;
	m_iState = m_iStartParam;
	m_serviceController = NULL;

}

BOOL CMyService::OnInit()
{
	// Read the registry parameters
    // Try opening the registry key:
    // HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\<AppName>\Parameters
    HKEY hkey;
	char szKey[1024];
	strcpy(szKey, "SYSTEM\\CurrentControlSet\\Services\\");
	strcat(szKey, m_szServiceName);
	strcat(szKey, "\\Parameters");
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

		if(!m_serviceController)
		{
			Sleep(20000);
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

			m_serviceController = new CARBServiceController();
			

			SetSoftwareRegistry("Set Beta",m_workingPath.GetBuffer());	
			
            CFileException exc;
            CFile fl;
  
	        CArchive the_Archive(&fl,CArchive::load);
            BOOL res = fl.Open("serverstartup.cfg",CFile::modeRead, &exc);
	        if(res)
	        {
				CString misc;
				the_Archive.ReadString(m_sqlServerIP);
				the_Archive.ReadString(m_sqlServerUsername);
				the_Archive.ReadString(m_sqlServerPassword);
				the_Archive.ReadString(m_sqlServerCatalog);
				the_Archive.ReadString(m_sqlServerTable);
				the_Archive.ReadString(m_serverIdName);
			    the_Archive.Close();
			    fl.Close();
	        }
            //clear log
			CFileException exc2;
			CFile fl2;
			CArchive the_out_Archive2(&fl2,CArchive::store);
			res = fl2.Open("log.txt",CFile::modeCreate | CFile::modeWrite, &exc2);
			if(res)
			{
				the_out_Archive2.WriteString("Log Cleared");  
				the_out_Archive2.WriteString( "\n");   
				the_out_Archive2.Close();
				fl2.Close(); 
			}


		}
        //do the process
        if(m_serviceController)
		{
		   SetCurrentDirectory(m_workingPath);
           CDBConSystem * newConnection = new CDBConSystem(m_sqlServerIP,m_sqlServerCatalog,m_sqlServerUsername,m_sqlServerPassword);
           
		   
           _RecordsetPtr recordObj = newConnection->Query("Select serviceName, enabled, serverNameID from tblServiceControl where serverNameID = '"+ m_serverIdName +"'");
           int recordCount = newConnection->GetCount(recordObj);
		  
		   for(int loop=0;loop<recordCount;loop++)
		   {
               CString serviceNameTemp = newConnection->GetStringData("serviceName",recordObj);
			   int desiredState = newConnection->GetIntData("enabled",recordObj);
			   if(m_serviceController->IsServiceEnabled(serviceNameTemp) != desiredState)
			   {
                  m_serviceController->SetState(serviceNameTemp,desiredState);
				  
			   }
			   newConnection->NextRecord(recordObj);
		   }
		   newConnection->Close(recordObj);
		   delete newConnection;
		   newConnection = 0;
		}
		// Sleep for a while
        Sleep(15000);
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
	 
    
	    SetCurrentDirectory(m_workingPath);
	    if(m_serviceController)
		{
		delete m_serviceController;
		m_serviceController = 0;
		}

}


void CMyService::OnShutdown()
{
	 
	    SetCurrentDirectory(m_workingPath);
	    if(m_serviceController)
		{
		delete m_serviceController;
		m_serviceController = 0;
		}

	 CoUninitialize();
}

void CMyService::MakeStatusEntry(CString name,DWORD val)
{
            HKEY hkey = NULL;
	        char szKey[1024];
	        strcpy(szKey, "SYSTEM\\CurrentControlSet\\Services\\");
	        strcat(szKey, m_szServiceName);
	        strcat(szKey, "\\Status");
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
	        strcpy(szKey, "SYSTEM\\CurrentControlSet\\Services\\");
	        strcat(szKey, m_szServiceName);
	        strcat(szKey, "\\Status");
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
	strcpy(szKey, "SYSTEM\\CurrentControlSet\\Services\\");
	strcat(szKey, m_szServiceName);
	strcat(szKey, "\\Status");
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
	        strcpy(szKey, "SYSTEM\\CurrentControlSet\\Services\\");
	        strcat(szKey, m_szServiceName);

            lRet = RegOpenKeyEx( HKEY_LOCAL_MACHINE,
                                 szKey,
                                 0, 
								 KEY_QUERY_VALUE, 
								 &hKey );
            
			if( lRet != ERROR_SUCCESS )
               return "";

            lRet = RegQueryValueEx( hKey, valueRequest, NULL, NULL,
                                 (LPBYTE) szProductType, &dwBufLen);
            if( (lRet != ERROR_SUCCESS) || (dwBufLen > BUFSIZE) )
               return "";

            RegCloseKey( hKey );

            return szProductType;
}

void CMyService::SetSoftwareRegistry(LPSTR valueName,LPSTR valueDataX)
{
   HKEY hk;
	        char szKey[1024];
	        strcpy(szKey, "SYSTEM\\CurrentControlSet\\Services\\");
	        strcat(szKey, m_szServiceName);
			strcat(szKey, "\\Status");
   DWORD dwData; 
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
                     strlen(szBuf) + 1))       // length of value data 
   {

   }


   delete [] szBuf;
   szBuf = 0;

}
