#define D3D_OVERLOADS
#define WINVER 0x0502

#include <AfxWin.h>
#include <AfxExt.h>
#include "d3d9.h"
#include "EngineXApp.h"
#include "EngineXWin.h"
 #include "resource.h"
#include "CrashReporter.h"

// AuraXDynamic.dll and NetworkExtension.dll are MFC extension DLLs whose
// DllMain deliberately does NOT call new CDynLinkLibrary(); each exports an
// init function the host app must call instead. Without it the DLLs' MFC
// runtime classes (CEXScriptObjList, CWldObject, ...) are never attached to
// the app, CRuntimeClass::FromName() cannot find them, and every CArchive
// load of a .aut/.EXG/.EDB fails with "contained an unexpected object".
extern "C" void WINAPI InitAuraXExtDLL();
extern "C" void WINAPI InitAXNetworkExtDLL();




BEGIN_MESSAGE_MAP(EngineXApp, RMApp)
	//{{AFX_MSG_MAP(EngineXApp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

EngineXApp theapp;

BOOL EngineXApp::InitInstance()
{
InstallCrashReporter();
	// Always run from the folder EngineX.exe lives in. startcfg.aut, the GameFiles folder
	// and the logs are all found relative to the working directory, and a
	// shortcut / "Run as administrator" / drag-drop launch can start us elsewhere
	// (then startcfg.aut is missed and the renderer gets -1 x -1).
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
AfxEnableControlContainer();
  


#ifdef _DEBUG
	afxTraceEnabled=FALSE;
#endif
	EngineXWin* win=new EngineXWin;
	if (!win->Create( "Aura Engine", IDI_ICON, IDR_MAINMENU ))
		return FALSE;
	m_pMainWnd=win;
	return RMApp::InitInstance();
}

//BOOL EngineXApp::InitApplication()
//{
//	
//
//	return RMApp::InitApplication();
//}
