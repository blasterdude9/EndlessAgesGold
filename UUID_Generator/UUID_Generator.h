// UUID_Generator.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CUUID_GeneratorApp:
// See UUID_Generator.cpp for the implementation of this class
//

class CUUID_GeneratorApp : public CWinApp
{
public:
	CUUID_GeneratorApp();

// Overrides
	public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
};

extern CUUID_GeneratorApp theApp;