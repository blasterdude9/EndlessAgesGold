#pragma once
#include "sharednet.h"

class CCharacterCreationSystems
{
public:
	CCharacterCreationSystems(CSharedNet * sharedInterface);
	~CCharacterCreationSystems(void);


    //Vars
    CSharedNet * m_sharedInterfaceRef;



    //Functions
	BOOL HandleCharacterCreationOnAccount(LPVOID lpMessage,int & msgSize,DPID & idFromLoc, int handleOverride);

};
