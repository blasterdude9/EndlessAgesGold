// AXServerApplication.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "AXServerApplication.h"
#include "AXServerApplicationDlg.h"
#include "CrashReporter.h"

// MFC extension DLLs (AuraXDynamic.dll, NetworkExtension.dll) must register their
// CRuntimeClass chains with this exe or CArchive loads of their classes fail.
extern "C" void WINAPI InitAuraXExtDLL();
extern "C" void WINAPI InitAXNetworkExtDLL();

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAXServerApplicationApp

BEGIN_MESSAGE_MAP(CAXServerApplicationApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CAXServerApplicationApp construction

CAXServerApplicationApp::CAXServerApplicationApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CAXServerApplicationApp object

CAXServerApplicationApp theApp;


// CAXServerApplicationApp initialization

BOOL CAXServerApplicationApp::InitInstance()
{
	InstallCrashReporter();
	// Work relative to the exe: serverstartup.cfg / authAccount.cfg / baseAccount.cfg and
	// GameFiles\ are looked up from the current directory.
	{
		char exeDir[MAX_PATH] = {0};
		if (GetModuleFileNameA(NULL, exeDir, MAX_PATH))
		{
			char* slash = strrchr(exeDir, '\\');
			if (slash) { *slash = 0; SetCurrentDirectoryA(exeDir); }
		}
	}
	InitAuraXExtDLL();
	InitAXNetworkExtDLL();

	// InitCommonControls() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	CAXServerApplicationDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
