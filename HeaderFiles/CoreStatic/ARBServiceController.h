#include "Winsvc.h"
#pragma once

class CARBServiceController
{
public:
	CARBServiceController(void);
	~CARBServiceController(void);

    //
	BOOL					m_initializedManager;
	SC_HANDLE				m_schSCManager;



	//functions
	BOOL StartServiceAB(CString serviceName);
	int IsServiceStopped(CString serviceName);
	BOOL StopServiceAB(CString serviceName);
	BOOL RebootMachine();
	int IsServiceEnabled(CString serviceName);
	BOOL SetState(CString serviceName, int state);

};
