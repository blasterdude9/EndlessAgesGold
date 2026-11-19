#pragma once
#include "sharednet.h"


class CArenaSystems
{
public:
	CArenaSystems(CSharedNet * sharedInterface);
	~CArenaSystems(void);

	//Variables
	CSharedNet					* m_sharedInterfaceRef;
	long                          m_arenaCallBackStamp;
	


	//functions
	void ArenaSystemCallback();
	int HasSomeoneWonTheArena(int arena);
	void BroadcastVicinityOfArenaBookies(CString msg);
	BOOL SendMessageToArenaSignups(int arena,CString msg);
	BOOL ReadyToStart(int arena);
	BOOL CallAllPlayersToArena(int arena);
};
