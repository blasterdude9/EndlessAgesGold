#pragma once
#include "sharednet.h"

class CAiSystems
{
public:
	CAiSystems(CSharedNet * sharedInterface);
	~CAiSystems(void);

	//Var
    CSharedNet * m_sharedInterfaceRef;

	//functions
    BOOL HandleAIBlockData_safe(LPVOID lpMessage,int & msgSize,DPID & idFromLoc,int & channelID);

};
