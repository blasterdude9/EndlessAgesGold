#pragma once
#include "servermain.h"



class CServerProc
{
public:
	CServerProc(void);
	~CServerProc(void);

	//Vars
	CServerMain				*   m_serverSystem;
	HANDLE                      m_lgThreadHandle;
    HANDLE                      m_commThreadHandle;
	
	
	HANDLE					   *m_axThreadHandleReference;
	DWORD					   *m_axThreadThreadID;
	
	int                         m_charWritterThreadCount;
	DWORD					   *m_charWritterThreadID;
	HANDLE                     *m_charWritterThreadHandle;
	
	int                         m_miscDataProcessorCount;
	DWORD					   *m_miscDataProccesorThreadID;
	HANDLE                     *m_miscDataProccesorThreadHandle;
	
	int                         m_charAquisitionThreadCount;
	DWORD					   *m_charAquisitionThreadID;
	HANDLE                     *m_charAquisitionThreadHandle;

	int                         m_unifiedCommerceThreadCount;
	DWORD					   *m_unifiedCommerceThreadID;
	HANDLE                     *m_unifiedCommerceThreadHandle;

	HANDLE                      m_zoneUpdaterThreadHandle;
	DWORD						m_zoneUpdaterThreadID;

    HANDLE                      m_nodeStatusUpdaterThreadHandle;
	DWORD						m_nodeStatusUpdaterThreadID;

	HANDLE                      m_spawnGenWorkerThreadHandle;
	DWORD						m_spawnGenWorkerThreadID;

	HANDLE                      m_ServerItemWorkerThreadHandle;
	DWORD						m_ServerItemWorkerThreadID;

	HANDLE                      m_ServerTimeoutThreadHandle;
	DWORD						m_ServerTimeoutThreadID;

	HANDLE                      m_ServerClanWorkerThreadHandle;
	DWORD						m_ServerClanWorkerThreadID;
	

	
	
    
	void ShutDownServer();
	void GetServerStatus(AB_serverStatusLive * serverStatus);
};
