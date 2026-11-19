#include "ZpingMainFunctions.h"
#pragma once

class CMainProc
{
public:
	CMainProc(void);
	~CMainProc(void);
	CMainProc(CString workingDirectory);


    //Vars
	CMainFunctions *			m_mainFunctions;
    HANDLE                      m_lgThreadHandle;

	
	
	


};
