// AXEncryptDLL.h : main header file for the AXEncryptDLL DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols


// CAXEncryptDLLApp
// See AXEncryptDLL.cpp for the implementation of this class
//

class CAXEncryptDLLApp : public CWinApp
{
public:
	CAXEncryptDLLApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
