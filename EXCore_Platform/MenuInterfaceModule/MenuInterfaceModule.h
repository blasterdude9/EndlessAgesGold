// MenuInterfaceModule.h : main header file for the MenuInterfaceModule DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CMenuInterfaceModuleApp
// See MenuInterfaceModule.cpp for the implementation of this class
//

class CMenuInterfaceModuleApp : public CWinApp
{
public:
	CMenuInterfaceModuleApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
