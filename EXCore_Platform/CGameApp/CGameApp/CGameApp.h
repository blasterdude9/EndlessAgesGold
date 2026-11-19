//CGameApp.h : main header file for the CGameApp application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       //main symbols


//CCGameAppApp:
//See CGameApp.cpp for the implementation of this class
//

class CCGameAppApp : public CWinApp
{
public:
	CCGameAppApp();


//Overrides
public:
	virtual BOOL InitInstance();
	virtual LRESULT ProcessWndProcException(CException* e, const MSG* pMsg);

//Implementation

public:
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CCGameAppApp theApp;