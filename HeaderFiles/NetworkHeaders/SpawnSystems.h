#pragma once
#include "sharednet.h"

class CSpawnSystems
{
public:
	CSpawnSystems(CSharedNet * sharedInterface);
	~CSpawnSystems(void);

	//Vars
    CSharedNet * m_sharedInterfaceRef;



	//Functions
    BOOL HandleSpawnMessagesServerSide(LPSPAWN_STRUCT formatedMessage,DPNID idFromLoc,CAccountObject * accountPtr,int channelOverride);
	BOOL HandleAISpawnMessagesServerSide_safe(LPSPAWN_STRUCT formatedMessage,DPNID idFromLoc);
	void FinalizeSpawn(CPlayerObject * newPobj,CAccountObject * accountPtr,int trace);
	BOOL FlagBodyAsPlaced(int handle);

};
