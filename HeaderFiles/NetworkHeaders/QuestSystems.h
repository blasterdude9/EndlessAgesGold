#pragma once
#include "sharednet.h"

class CQuestSystems
{
public:
	CQuestSystems(CSharedNet * sharedInterface);
	~CQuestSystems(void);

    //Vars
    CSharedNet * m_sharedInterfaceRef;



	//Functions
	BOOL HandleQuestSys(CPlayerObject * currentPlayer,
		                CAccountObject * accountPtrLoc,
						CAICollectionQuestObj * questPtr,
						CServerItemObj * svrPtr,
						DPNID idFromLoc,
						int & questSelection);

	BOOL HandleArena(CPlayerObject * currentPlayer,
					 CAICollectionQuestObj * questPtr,
					 CServerItemObj * svrPtr,
					 DPNID idFromLoc);

	BOOL HandleGroupInstancing( CPlayerObject * currentPlayer,
								CAICollectionQuestObj * questPtr,
								CServerItemObj * svrPtr,
								DPNID idFromLoc);

	BOOL StartArena_requiresRuntimeEntityLock(CAICollectionQuestObj * questPtr);
};
