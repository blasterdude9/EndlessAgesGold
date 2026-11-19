#pragma once

#include "RemoteHost.h"

class AXMediaLib
{
public:
	AXMediaLib(void){};
	AXMediaLib(void * axHandleX);
	~AXMediaLib(void);

	//vars
	//CAxWindow  *m_pView; 
	CComPtr<IWMPPlayer4>            m_spPlayer;             // Player
    BOOL m_playerInitializedOk;


	//functions
	BOOL PlayURL(char * url);
	BOOL Stop();
};
