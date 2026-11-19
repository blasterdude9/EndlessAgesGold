#pragma once

// AX_EngineControl.h : main header file for AX_EngineControl.DLL

#if !defined( __AFXCTL_H__ )
#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols


// CAX_EngineControlApp : See AX_EngineControl.cpp for implementation.

class CAX_EngineControlApp : public COleControlModule
{
public:
	BOOL InitInstance();
	int ExitInstance();
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

