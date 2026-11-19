// CBaseInterfaceModule.h : main header file for the CBaseInterfaceModule DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

// CCBaseInterfaceModuleApp
// See CBaseInterfaceModule.cpp for the implementation of this class
//

class CCBaseInterfaceModuleApp : public CWinApp
{
public:
	CCBaseInterfaceModuleApp();

// Overrides
public:
	virtual BOOL InitInstance();


	DECLARE_MESSAGE_MAP()
};
