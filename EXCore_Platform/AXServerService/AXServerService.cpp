// AXServerService.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "myservice.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

int _tmain(int argc, _TCHAR* argv[])
{
	CMyService MyService;
    
    // Parse for standard arguments (install, uninstall, version etc.)
    if (!MyService.ParseStandardArgs(argc, argv)) {

        // Didn't find any standard args so start the service
        // Uncomment the DebugBreak line below to enter the debugger
        // when the service is started.
        //DebugBreak();
        MyService.StartService();
    }

    // When we get here, the service has been stopped
    return MyService.m_Status.dwWin32ExitCode;
}

