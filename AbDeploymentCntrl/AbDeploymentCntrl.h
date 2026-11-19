#pragma once

// AbDeploymentCntrl.h : main header file for AbDeploymentCntrl.DLL

#if !defined( __AFXCTL_H__ )
#error include 'afxctl.h' before including this file
#endif

#include "resource.h"       // main symbols



// CAbDeploymentCntrlApp : See AbDeploymentCntrl.cpp for implementation.

class CAbDeploymentCntrlApp : public COleControlModule
{
public:
    
	BOOL InitInstance();
	int ExitInstance();
//	virtual BOOL OnIdle(LONG lCount);
};

extern const GUID CDECL _tlid;
extern const WORD _wVerMajor;
extern const WORD _wVerMinor;

