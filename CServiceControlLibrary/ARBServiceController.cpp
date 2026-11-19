#include "StdAfx.h"

#include ".\arbservicecontroller.h"

CARBServiceController::CARBServiceController(void)
{
	m_schSCManager = OpenSCManager(   NULL,                    // local machine 
									  NULL,                    // ServicesActive database 
								      SC_MANAGER_ALL_ACCESS);  // full access rights 
 
    if (m_schSCManager == NULL) 
	{
        m_initializedManager = FALSE;
		return;
	}
	m_initializedManager = TRUE;
}

CARBServiceController::~CARBServiceController(void)
{
    if(m_schSCManager)
	   CloseServiceHandle(m_schSCManager); 
	m_schSCManager = NULL;
}


BOOL CARBServiceController::StartServiceAB(CString serviceName)
{
     SC_HANDLE schService = OpenService( 
                                         m_schSCManager,          // SCM database 
                                         serviceName,          // service name
                                         SERVICE_ALL_ACCESS); 
	if (schService == NULL) 
    { 
		CloseServiceHandle(schService); 
        return FALSE; 
    }

	if (!StartService(
            schService,  // handle to service 
            0,           // number of arguments 
            NULL) )      // no arguments 
    {
		CloseServiceHandle(schService); 
        return FALSE; 
    }
    

	CloseServiceHandle(schService); 

    return TRUE;
}

BOOL CARBServiceController::SetState(CString serviceName, int state)
{
    if(state)
       return StartServiceAB(serviceName);
	else
       return StopServiceAB(serviceName);

	return FALSE;
}

int CARBServiceController::IsServiceEnabled(CString serviceName)
{
    return !IsServiceStopped(serviceName);

}

int CARBServiceController::IsServiceStopped(CString serviceName)
{
     SC_HANDLE schService = OpenService( 
                                         m_schSCManager,          // SCM database 
                                         serviceName,          // service name
                                         SERVICE_ALL_ACCESS); 
	if (schService == NULL) 
    { 
		CloseServiceHandle(schService); 
        return FALSE; 
    }

    SERVICE_STATUS lpServiceStatus;
	if (!QueryServiceStatus(
                schService,  // handle to service 
                &lpServiceStatus) )      // no arguments 
    {
		CloseServiceHandle(schService); 
        return FALSE; 
    }
    
	CloseServiceHandle(schService); 

	if(lpServiceStatus.dwCurrentState == SERVICE_STOPPED)
       return TRUE;

	return FALSE;
}


BOOL CARBServiceController::StopServiceAB(CString serviceName)
{
     SC_HANDLE schService = OpenService( 
                                         m_schSCManager,          // SCM database 
                                         serviceName,          // service name
                                         SERVICE_ALL_ACCESS); 
	if (schService == NULL) 
    { 
		CloseServiceHandle(schService); 
        return FALSE; 
    }

	SERVICE_STATUS lpServiceStatus;
	if(ControlService(
                      schService,
                      SERVICE_CONTROL_STOP,
                      &lpServiceStatus)== FALSE)
	{
       CloseServiceHandle(schService); 
       return FALSE; 
	}


    CloseServiceHandle(schService); 
	return TRUE;
}

BOOL CARBServiceController::RebootMachine()
{
   HANDLE hToken; 
   TOKEN_PRIVILEGES tkp; 
 
   // Get a token for this process. 
 
   if (!OpenProcessToken(GetCurrentProcess(), 
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) 
      return FALSE; 
 
   // Get the LUID for the shutdown privilege. 
 
   LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, 
        &tkp.Privileges[0].Luid); 
 
   tkp.PrivilegeCount = 1;  // one privilege to set    
   tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED; 
 
   // Get the shutdown privilege for this process. 
 
   AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, 
        (PTOKEN_PRIVILEGES)NULL, 0); 
 
   if (GetLastError() != ERROR_SUCCESS) 
      return FALSE; 

    return ExitWindowsEx(EWX_REBOOT | EWX_FORCE, 0);
}
