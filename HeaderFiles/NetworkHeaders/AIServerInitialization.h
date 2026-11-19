#pragma once
#include "clientmodule.h"
#include "DBConnectionSystem.h"

struct AB_aiServerStatusLive
{
    long                        m_botsOnline;
	long                        m_botsSpikeOnline;
	long                        m_msgsOutPerSecond;
	long                        m_msgsInPerSecond;
	long                        m_bandwidthOutgoingPerSecond;
	long                        m_bandwidthIncommingPerSecond;
	long                        m_totalBandwidthIn;
	long                        m_totalBandwidthOut;
	long                        m_totalBandwidthTotal;
	int                         m_worldCount;
};

class CAIServerInitialization
{
public:
	CAIServerInitialization(void);
	~CAIServerInitialization(void);


	//Vars
	CServerEventList			* m_eventLog;
    CClientModule				* m_clientModule;
	CDBConnectionSystem			* m_adoConnection;
	CString						  m_dataPath;
	BOOL                          m_systemShutDown;
	CRITICAL_SECTION              m_interactSection;
	AB_aiServerStatusLive         m_aiServerStatus;
	CRITICAL_SECTION              m_statusQueryLock;
	DWORD                         m_aiResetTime;


	//Functions
	int							ConnectToServer(int waitTime);
	BOOL						LoadSharedInterfaces(CString tableName, CString filter);
	BOOL						LoadDataByType(CString filename);


};
