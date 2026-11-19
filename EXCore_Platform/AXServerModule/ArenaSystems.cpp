#include "StdAfx.h"
#include "arenasystems.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CArenaSystems::CArenaSystems(CSharedNet * sharedInterface)
{
	long currentTime = GetTickCount();
	m_sharedInterfaceRef = sharedInterface;
	m_arenaCallBackStamp = currentTime;
	
}

CArenaSystems::~CArenaSystems(void)
{
  
}

//------------------------------------------------------------------------------------//
void CArenaSystems::ArenaSystemCallback()
{
   
}
//------------------------------------------------------------------------------------//
int CArenaSystems::HasSomeoneWonTheArena(int arena)
{

	CBattleSchedulerObj * arenaObj = m_sharedInterfaceRef->m_battleSchedulerSys->GetByIndex(arena);
	if(!arenaObj)
		return FALSE;

	if(arenaObj->m_currentSignupCount == 0)
		return FALSE;

	if(arenaObj->m_signups == NULL)
		return FALSE;

	int tracker = 0;
	int winner = 0;

	if(arenaObj->m_type == 0)
	{
	 for(int loop=0;loop<arenaObj->m_currentSignupCount;loop++)
	 {
        if(arenaObj->m_signups[loop] == 0)
		   continue;

		if(!m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer)
		    continue;

	    CPlayerObject * playerObjectData = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandle(arenaObj->m_signups[loop]);
		if(playerObjectData)
		if(!playerObjectData->m_aiControlled)
		if(playerObjectData->m_currentWorldID == arenaObj->m_wldChannel)
		{//found one
		   winner = arenaObj->m_signups[loop];
		   tracker++;
		}//end found one 

		if(tracker > 1)//game is still on
		   return 0;
	 }
	}
	else if(arenaObj->m_type == 1)
	{//team arena
	   int initialClan = -1;
       for(int loop=0;loop<arenaObj->m_currentSignupCount;loop++)
	   {
        if(arenaObj->m_signups[loop] == 0)
		   continue;

		if(!m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer)
		    continue;

	    CPlayerObject * playerObjectData = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandle(arenaObj->m_signups[loop]);
		if(playerObjectData)
		if(!playerObjectData->m_aiControlled)
		if(playerObjectData->m_currentWorldID == arenaObj->m_wldChannel)
		{//found one
		   if(playerObjectData->m_clanHandle != -1)
		   {
			   if(initialClan == -1)
			   {
			      initialClan = playerObjectData->m_clanHandle;
                  winner = playerObjectData->m_clanHandle;
				  tracker++;
			   }

			   if(initialClan != playerObjectData->m_clanHandle)
                  tracker++;
		   }
		}//end found one 

		if(tracker > 1)//game is still on
		   return 0;
	   }
	}//end team arena

	return winner;
}
//------------------------------------------------------------------------------------//
void CArenaSystems::BroadcastVicinityOfArenaBookies(CString msg)
{
  //OBSOLETE
  /*for(POSITION posLoc = m_sharedInterfaceRef->m_netComObject->m_runtimeItems->GetHeadPosition();posLoc != NULL;)
  {
	CServerItemObj * svrPtr = (CServerItemObj *)m_sharedInterfaceRef->m_netComObject->m_runtimeItems->GetNext(posLoc);
    if(svrPtr->m_staticItemType == 4)
	{
       m_sharedInterfaceRef->BroadcastMessage(400,
						                      msg,
					              	          svrPtr->m_position,
						                      svrPtr->m_world,
						                      0);
	}
  }*/
}
//------------------------------------------------------------------------------------//
BOOL CArenaSystems::SendMessageToArenaSignups(int arena,CString msg)
{
	CBattleSchedulerObj * arenaObj = m_sharedInterfaceRef->m_battleSchedulerSys->GetByIndex(arena);
	if(!arenaObj)
		return FALSE;

	if(arenaObj->m_currentSignupCount == 0)
		return FALSE;

	if(arenaObj->m_signups == NULL)
		return FALSE;

	for(int loop=0;loop<arenaObj->m_currentSignupCount;loop++)
	{
        if(arenaObj->m_signups[loop] == 0)
		   continue;

		if(!m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer)
		    continue;

	    CPlayerObject * playerObjectData = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandle(arenaObj->m_signups[loop]);
		if(playerObjectData)
		if(!playerObjectData->m_aiControlled)
		{//found one 
		   m_sharedInterfaceRef->SendTextMsg(DPNSEND_GUARANTEED,
						                   playerObjectData->m_currentID,
						                   msg,
							               0,
										   playerObjectData,
										   0);//confermation
		}//end found one 
	}
	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CArenaSystems::ReadyToStart(int arena)
{
    CBattleSchedulerObj * arenaObj = m_sharedInterfaceRef->m_battleSchedulerSys->GetByIndex(arena);
	if(!arenaObj)
		return FALSE;

	if(arenaObj->m_currentSignupCount == 0)
		return FALSE;

	if(arenaObj->m_signups == NULL)
		return FALSE;


    if(arenaObj->m_type == 0)
	{
       if(arenaObj->m_currentSignupCount < arenaObj->m_batlleMinimum)//was <=, so it needed one more than the minimum
          return FALSE;
	}
	else if(arenaObj->m_type == 1)
	{//TEAM ARENA
       if(arenaObj->m_currentSignupCount < arenaObj->m_batlleMinimum)//was <=, so it needed one more than the minimum
          return FALSE;
       int tracker = 0;
       int initialClan = -1;
       for(int loop=0;loop<arenaObj->m_currentSignupCount;loop++)
	   {//VERIFY 2 teams exist
        if(arenaObj->m_signups[loop] == 0)
		   continue;

		if(!m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer)
		    continue;

	    CPlayerObject * playerObjectData = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandle(arenaObj->m_signups[loop]);
		if(playerObjectData)
		if(!playerObjectData->m_aiControlled)
		{//found one
		   if(playerObjectData->m_clanHandle != -1)
		   {
			   if(initialClan == -1)
			   {
			      initialClan = playerObjectData->m_clanHandle;
				  tracker++;
			   }

			   if(initialClan != playerObjectData->m_clanHandle)
                  tracker++;
		   }
		}//end found one 

		if(tracker > 1)//2 teams verified
		   return TRUE;
	   }//VERIFY 2 teams exist
	   return FALSE;
	}//END TEAM ARENA

	return TRUE;
}
//------------------------------------------------------------------------------------//
BOOL CArenaSystems::CallAllPlayersToArena(int arena)
{
	//obsolete
	return TRUE;
}