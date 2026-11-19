// PatchServer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PatchServer.h"
#include "PatchServerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPatchServerApp

BEGIN_MESSAGE_MAP(CPatchServerApp, CWinApp)
	//{{AFX_MSG_MAP(CPatchServerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatchServerApp construction

CPatchServerApp::CPatchServerApp()
{
	//Verify DirectX8 is installed
	// Simply see if D3D8.dll exists.
	HINSTANCE hD3D8DLL = NULL;
    hD3D8DLL = LoadLibrary( "D3D8.DLL" );
    if( hD3D8DLL == NULL )
    {//not the correct version of directX
       AfxMessageBox("Must have DirectX8 Installed");
       exit(1);//close after starting primary application	
    }//end not the correct version of directX
    FreeLibrary( hD3D8DLL );
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPatchServerApp object

CPatchServerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPatchServerApp initialization

BOOL CPatchServerApp::InitInstance()
{
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CPatchServerDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
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
