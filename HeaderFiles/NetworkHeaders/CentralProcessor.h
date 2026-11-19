#pragma once
#include "worldmanager.h"


class CCentralProcessor
{
public:
	CCentralProcessor(int connectionAttempts);
	~CCentralProcessor(void);


    //Vars
    CWorldManagerDatabase			* m_aiWorldsList;
    CAIServerInitialization			* m_aiServer;
    BOOL                              m_connected;
	DWORD                             m_axThreadID;
	HANDLE                            m_axThreadHandle;
	DWORD                             m_gCycleUpdateStamp;
	DWORD                             m_gCycleUpdateFrequency;
	HANDLE                            m_lgThreadHandle;
	CRITICAL_SECTION                  m_criticalWldSpawn;
	int							      m_threadCount;
	
	//Functions
    BOOL SpawnAIWorld(CString worldEXG, CString altCollisionCRD, int channelID);
	void LoadWorldSystems();
	void SendPendingCfgRequestsCP();
	BOOL IsConnectionLost();   // TRUE once the main game server has dropped us

};
