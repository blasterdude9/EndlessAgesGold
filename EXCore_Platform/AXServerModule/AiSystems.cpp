#include "StdAfx.h"
#include "aisystems.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAiSystems::CAiSystems(CSharedNet * sharedInterface)
{
    m_sharedInterfaceRef = sharedInterface;
}

CAiSystems::~CAiSystems(void)
{

}

//--------------Handles system messages-----------------------------------------------//
BOOL CAiSystems::HandleAIBlockData_safe(LPVOID lpMessage,int & msgSize,DPID & idFromLoc,int & channelID)
{
       BYTE * dataBlock = (BYTE *)lpMessage;
	   BOOL unknowAiPresent = FALSE;
	   short idRef = 0;
	   channelID = -1;
     
	   if(!m_sharedInterfaceRef->m_netComObject->m_runtimeAccountDB->IsAdministrator_safe(idFromLoc))
	   {
		   m_sharedInterfaceRef->m_eventLog->AddEvent("Non Account AI Block(NON ANDIM SENDER) (HACK ATTEMPT DETECT)");
		   return FALSE;
	   }

	   int movStructSize = sizeof(SHRUNK_MOVE_STRUCT);
	   if(msgSize < movStructSize)
	   {//failed
          m_sharedInterfaceRef->m_eventLog->AddEvent("Block Update Failed");
		  return FALSE;
	   }//end failed
       //get msg count
       int msgCount = msgSize/movStructSize;
	   int shMovStructSz = sizeof(SHRUNK_MOVE_STRUCT);
       LPMOVE_STRUCT expandedStruct = (LPMOVE_STRUCT) new BYTE[sizeof(MOVE_STRUCT)];
       m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Lock();
	   for(int msgParse = 0;msgParse < msgCount;msgParse++)
	   {//parse messages
		   
	         expandedStruct = m_sharedInterfaceRef->m_netComObject->ExpandData((SHRUNK_LPMOVE_STRUCT)&dataBlock[(msgParse*shMovStructSz)+1],expandedStruct);
		     CPlayerObject * playerObjectData = m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->GetByNetID(expandedStruct->idRefrence);

			 if(!playerObjectData)
			 {//found one
			     unknowAiPresent = TRUE;
				 idRef = expandedStruct->idRefrence;
			 }//end found one
             else
			 {//valid cfg and not controlled
		
				     if(playerObjectData->m_dbCfg >= 0)
					 {
				       if(playerObjectData->m_aiControlled)
					   {
					    channelID = playerObjectData->m_currentWorldID;
			            playerObjectData->LogPositionalMsg(expandedStruct);//log normally
                        //m_sharedInterfaceRef->m_eventLog->AddEvent(playerObjectData->m_3dPosition);	 
					   }
                       else
					   {
					    unknowAiPresent = TRUE;
				        idRef = expandedStruct->idRefrence;
                 
						
						m_sharedInterfaceRef->m_eventLog->AddEvent("Live player is attempting to be updated bi AI at server channel:",(DWORD)playerObjectData->m_currentWorldID);
						m_sharedInterfaceRef->m_eventLog->AddEvent("entity name:",playerObjectData->m_charName);
					    }
					 }
			 }//valid cfg
	   }//end parse messages
	   m_sharedInterfaceRef->m_netComObject->m_runtimePlayersOnServer->Unlock();
	   if(expandedStruct)
	      delete [] expandedStruct;
	      expandedStruct=0;
	   
	   
	   if(channelID != -1)
	   {//channel bases was resolved
          if(!m_sharedInterfaceRef->m_netComObject->m_currentWorldsUsed->GetZoneDataByChannel_safe(channelID,0,0,0))
		  {//verify channel that ai ust updated is open, if not close it
			m_sharedInterfaceRef->CloseAIChannel(channelID);
			m_sharedInterfaceRef->m_eventLog->LogByPriority("Informing AI server to shut channel down :",(DWORD)channelID,2);
		  }//else verify channel that ai ust updated is open, if not close it

	      if(unknowAiPresent)
		  {
		   m_sharedInterfaceRef->m_eventLog->LogByPriority("(Attempting removal) AI cannot be updated Net ID:",(DWORD)idRef,2);
 
           m_sharedInterfaceRef->SendAttribMsg(   DPNSEND_GUARANTEED,
														idFromLoc,
														10,
														idRef,
														idRef,
														channelID,
														0,
				    									NULL);//remove invalid from ai server
		  }
	   }

	   return TRUE;
}