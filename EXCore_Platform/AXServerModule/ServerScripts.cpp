#include "StdAfx.h"
#include "serverscripts.h"
#include "CDplayPlayerClass.h"
#include "DBConnectionSystem.h"
#include "sharednet.h"
#include "CServerItemClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



bool CServerScriptSystem::ScriptProcessor(void * object,
										  int objectSize,
										  int scriptID,
										  CString & error,
										  CString & gameSpecific,
										  int edbIndex,
										  void * sNetMem)
{

	CServerScripts * script = GetByUID(scriptID,gameSpecific,edbIndex);
	CSharedNet * sharedNet = (CSharedNet *)sNetMem;
	if(!script)
	{
		error = "ScriptProcessor():GetByUID Failed:  Line 26: ScriptID: " + sharedNet->m_cabLib.ConvToString(scriptID)
			+ " Game: " + gameSpecific + " edbIndex: " + sharedNet->m_cabLib.ConvToString(edbIndex);
		return false;
	}

	int cursorPosition = 0;
	int scrLength = script->m_scriptBody.GetLength();
    if(scrLength < 2)
	   return false;

    char * cArray = script->m_scriptBody.GetBuffer();
	
	while(cursorPosition < scrLength)
	{
          CString command,paramsblock;

		  if(GetNextFunctionData(cursorPosition,scrLength, cArray, command, paramsblock)==true)
		  {//aquired function data
             if(DoFunction(command,paramsblock,object,objectSize,NULL,0,sNetMem)==false)
			 {//parse error
				//error = "DoFunction() Failed:"+command+" Params:"+paramsblock;
				//sharedNet->m_eventLog->LogByPriority("DoFunction() Failed:"+command+" Params:"+paramsblock,2);
			 }//end parse error
		  }//end aquired function data
		  else
		  {//report bad format (parse error)
			  error = "GetNextFunctionData() Failed";
		      return false;
		  }//end report bad format (parse error)
	}


    return true;
}

bool CServerScriptSystem::ScriptProcessor(void * object,int objectSize,int scriptID,CString & error,void * sNetMem)
{

	CServerScripts * script = GetByUID(scriptID);
	CSharedNet * sharedNet = (CSharedNet *)sNetMem;
	if(!script)
	{
		error = "ScriptProcessor():GetByUID Failed:  Line 66: ScriptID: " + sharedNet->m_cabLib.ConvToString(scriptID);
		return false;
	}

	int cursorPosition = 0;
	int scrLength = script->m_scriptBody.GetLength();
    if(scrLength < 2)
	   return false;

    char * cArray = script->m_scriptBody.GetBuffer();
	
	while(cursorPosition < scrLength)
	{
          CString command,paramsblock;

		  if(GetNextFunctionData(cursorPosition,scrLength, cArray, command, paramsblock)==true)
		  {//aquired function data
             if(DoFunction(command,paramsblock,object,objectSize,NULL,0,sNetMem)==false)
			 {//parse error
				//error = "DoFunction() Failed:"+command+" Params:"+paramsblock;
				//sharedNet->m_eventLog->LogByPriority("DoFunction() Failed:"+command+" Params:"+paramsblock,2);
			 }//end parse error
		  }//end aquired function data
		  else
		  {//report bad format (parse error)
			  error = "GetNextFunctionData() Failed";
		      return false;
		  }//end report bad format (parse error)
	}


    return true;
}

bool CServerScriptSystem::ScriptProcessor(void * object,
										  int objectSize,
										  void * object2, 
										  int object2Size,
										  CString scriptBody,
										  CString & error,
										  void * sNetMem)
{
	int cursorPosition = 0;
	CSharedNet * sharedNet = (CSharedNet *)sNetMem;
	int scrLength = scriptBody.GetLength();
    if(scrLength < 2)
	   return false;

    char * cArray = scriptBody.GetBuffer();
	
	while(cursorPosition < scrLength)
	{
          CString command,paramsblock;

		  if(GetNextFunctionData(cursorPosition,scrLength, cArray, command, paramsblock)==true)
		  {//aquired function data
             if(DoFunction(command,paramsblock,object,objectSize,object2,object2Size,sNetMem)==false)
			 {//parse error
				//error = "DoFunction() Failed:"+command+" Params:"+paramsblock;
				//sharedNet->m_eventLog->LogByPriority("DoFunction() Failed:"+command+" Params:"+paramsblock,2);
			 }//end parse error
		  }//end aquired function data
		  else
		  {//report bad format (parse error)
			  error = "GetNextFunctionData() Failed";
		      return false;
		  }//end report bad format (parse error)
	}


    return true;
}

DWORD CServerScriptSystem::GetDWORDval(CString command)
{
    int cnt = command.GetLength();
	if(cnt == 0)
	   return 0;

    DWORD val = 0;
	for(int i=0;i<cnt;i++)
        val += command.GetAt(i);

	return val;
}

bool CServerScriptSystem::FillParams(CString & paramBlock)
{
     int count = paramBlock.GetLength();
	 char * arrayLoc = paramBlock.GetBuffer();
     int trace = 0;
	 int paramIndex = 0;
	 m_params[paramIndex] = "";
     while(trace < count)
	 {
         if(paramIndex > 99)
		    return false;//wow
		 
		 if(arrayLoc[trace] == ' ' && m_params[paramIndex].GetLength()==0)
		 {
            trace++;
		    continue;
		 }

		 if(arrayLoc[trace] == 13 || arrayLoc[trace] == 10)
		 {
		    trace++;
			continue;
		 }

	     if(arrayLoc[trace] == ',')
		 {
            trace++;
		    paramIndex++;
			m_params[paramIndex] = "";
            continue;
		 }
		 
		 m_params[paramIndex] += arrayLoc[trace];
		 trace++;
	 }
	 return true;
}

bool CServerScriptSystem::DoFunction(CString & command, 
									 CString & params,
									 void * memoryObj,
									 int memoryObjSize,
									 void * memoryObj2,
									 int memoryObj2Size,
									 void * sNetMem)
{
	CSharedNet * sNet = (CSharedNet *)sNetMem;
	if(!sNet)
	    return false;

	if(!FillParams(params))
		sNet->m_eventLog->LogByPriority("CServerScriptSystem::DoFunction FillParams failed:param src:"+params,3);
	
    switch(m_axMap.GetIDFromFunctionName(command))
	{
			default:
			{
				sNet->m_eventLog->LogByPriority("CServerScriptSystem::DoFunction cannot find:"+command,(DWORD)m_axMap.GetIDFromFunctionName(command),2);
				return true;//unknown command ok
			}
			case SS_AddFullJournalEntry: 
			{//FULL jounral entry
				if(!memoryObj)
				   return false;

				if(memoryObjSize != sizeof(CPlayerObject))
				   return false;

				CPlayerObject * playerObject = (CPlayerObject *)memoryObj;
				

				if(!playerObject->m_questJournal)
					playerObject->m_questJournal = new CQuestJournalList();

				int tempzero = 0;
				playerObject->m_questJournal->LogQuest( m_params[0],//"Find and talk with Palack, use F8 to talk",
														NULL,
														m_params[1],//"Palack (Entrance To IIA)",
														m_params[2],//"Buy a weapon then talk to Palack",
														sNet->m_itemManager,
														atoi(m_params[3]),//0,
														atoi(m_params[4]),//0,//questSelection,
														atoi(m_params[5]),//2,//questPtr->m_completedKeyEnabled,
														atoi(m_params[6]),//0,//questPtr->m_mobToHuntID,
														atoi(m_params[7]),//0,//questPtr->m_maxDropCount,
														atoi(m_params[8]),//0,//questPtr->m_dropGlid,
														atoi(m_params[9]),//100,//questPtr->m_dropPercentage,
														atoi(m_params[10]),//0,//questPtr->m_autocomplete,
														atoi(m_params[11]),//0,//questPtr->m_skillTypeBonus,
														(float)atof(m_params[12]),//0,//questPtr->m_expBonus,
														(float)atof(m_params[13]),//0,//questPtr->m_maxExpLimit,
														atoi(m_params[14]),//0,//questPtr->m_progessBasedByQuestJournal,
														atoi(m_params[15]),//0,//questPtr->m_undeletable);
														m_params[16],
														atoi(m_params[17]),
														tempzero);//"");
			   break;
			}
			case SS_SetCharPosition:
			{
				if(!memoryObj)
					return false;

				if(memoryObjSize != sizeof(CPlayerObject))
				   return false;

				CPlayerObject * playerObject = (CPlayerObject *)memoryObj;
				if(!playerObject)
					return false;

				playerObject->m_currentWorldID    = atoi(m_params[0]);
				playerObject->m_currentWorldMap   = m_params[1];
				playerObject->m_3dPosition.x      = (float)atof(m_params[2]);
				playerObject->m_3dPosition.y      = (float)atof(m_params[3]);
				playerObject->m_3dPosition.z      = (float)atof(m_params[4]);

				break;
			}
			case SS_AddJournalEntry:
			{
				if(!memoryObj)
					return false;

				if(memoryObjSize != sizeof(CPlayerObject))
				   return false;

				CPlayerObject * playerObject = (CPlayerObject *)memoryObj;
				if(!playerObject)
					return false;

				if(!playerObject->m_playerOrigRef)
					return false;

				if(!playerObject->m_playerOrigRef->m_questJournal)
					return false;


				playerObject->m_playerOrigRef->m_questJournal->LogQuest(m_params[0],//npc name
																		m_params[1],//description
																		m_params[2]);//key statement

				sNet->UpdateClientRecentQuestJournalEntry(playerObject);//updated journal live
				break;
			}
			case SS_CloseJournalByNpc:
			{
				if(!memoryObj)
					return false;

				if(memoryObjSize != sizeof(CPlayerObject))
				   return false;

				CPlayerObject * playerObject = (CPlayerObject *)memoryObj;
				if(!playerObject)
					return false;

				if(!playerObject->m_playerOrigRef)
					return false;

				if(!playerObject->m_playerOrigRef->m_questJournal)
					return false;


				playerObject->m_playerOrigRef->m_questJournal->DeleteEntryByNpcName(m_params[0]);

				sNet->UpdateClientRecentQuestJournalEntry(playerObject);//updated journal live

				break;
			}
			case SS_SetCharRebirthPosition:
			{
				if(!memoryObj)
					return false;

				if(memoryObjSize != sizeof(CPlayerObject))
				   return false;

				

				CPlayerObject * playerObject = (CPlayerObject *)memoryObj;

				CAccountObject * acPtr = sNet->m_netComObject->m_runtimeAccountDB->GetAccountByID(playerObject->m_currentID);
				if(!acPtr)
				{
					sNet->m_eventLog->LogByPriority("SS_SetCharRebirthPosition FAILED GetAccountByID():",(DWORD)playerObject->m_currentID,2);
					sNet->m_eventLog->LogByPriority("SS_SetCharRebirthPosition FAILED worldName:"+m_params[3],2);
					return false;
				}

				CPlayerObject * characterOnAccount = acPtr->m_characterOnAccount;//GetCurrentCharacter();
				if(!characterOnAccount)
				{
					sNet->m_eventLog->LogByPriority("SS_SetCharRebirthPosition FAILED GetCurrentCharacter()",2);
					return false;
				}

				characterOnAccount->m_respawnPosition.x = (float)atof(m_params[0]);
				characterOnAccount->m_respawnPosition.y = (float)atof(m_params[1])+4.0f;
				characterOnAccount->m_respawnPosition.z = (float)atof(m_params[2]);
				characterOnAccount->m_respawnWorldName  = m_params[3];

				break;
			}
			case SS_SetCharArenaRebirthPosition:
			{
				if(!memoryObj)
					return false;

				if(memoryObjSize != sizeof(CPlayerObject))
				   return false;

				CPlayerObject *playerObject = (CPlayerObject*)memoryObj;//convert memory to usable stuff
				playerObject->m_arenaRespawnPosition.x = (float)atof(m_params[0]);
				playerObject->m_arenaRespawnPosition.y = (float)atof(m_params[1])+4.0f;
				playerObject->m_arenaRespawnPosition.z = (float)atof(m_params[2]);
				playerObject->m_arenaRespawnWorldName = m_params[3];

				break;
			}
			case SS_ResetArenaStats:
			{
				if(!memoryObj)
					return false;

				if(memoryObjSize != sizeof(CPlayerObject))
				   return false;

				CPlayerObject *playerObject = (CPlayerObject*)memoryObj;//convert memory to usable stuff
				if(playerObject->m_arenaInfo)
				{
					playerObject->m_arenaInfo[0] = (int)atoi(m_params[0]);
					for(int i = 1; i < ARENA_TRACKING; i++)
						playerObject->m_arenaInfo[i] = 0;
				}

				break;
			}
			case SS_OptimizeInventory:
			{
				if(!memoryObj)
					return false;

				if(memoryObjSize != sizeof(CPlayerObject))
				   return false;

				CPlayerObject * playerObject = (CPlayerObject *)memoryObj;
				playerObject->m_currentInventory->OptimizeBestConfig();

				break;
			}
			case SS_SetNPCAnim:
			{
				if(!memoryObj)
				   return false;

				if(memoryObjSize != sizeof(CPlayerObject))
				   return false;

				if(!memoryObj2)//cannot find npc for function SetNPCAnimation()
				   return false;

				if(memoryObj2Size != sizeof(CServerItemObj))
				   return false;

				CPlayerObject * playerObject = (CPlayerObject *)memoryObj;
				CServerItemObj * svrItem = (CServerItemObj *)memoryObj2;
				svrItem->m_curAnim = atoi(m_params[0]);
				sNet->BroadcastNPCAnimState(svrItem);

				break;
			}
			case SS_ToggleNPCAnimDir:
			{
				if(!memoryObj)
				   return false;

				if(memoryObjSize != sizeof(CPlayerObject))
				   return false;

				if(!memoryObj2)//cannot find npc for function SetNPCAnimation()
				   return false;

				if(memoryObj2Size != sizeof(CServerItemObj))
				   return false;

				CPlayerObject * playerObject = (CPlayerObject *)memoryObj;
				CServerItemObj * svrItem = (CServerItemObj *)memoryObj2;
				svrItem->m_curAnim = -svrItem->m_curAnim;
				//broadcast notification of change
				sNet->BroadcastNPCAnimState(svrItem);

				break;
			}
			case SS_SetLargeMoveFlag:
			{
				if(!memoryObj)
					return false;

				if(memoryObjSize != sizeof(CPlayerObject))
				   return false;

				CPlayerObject * playerObject = (CPlayerObject *)memoryObj;

				playerObject->m_allowLargeMovement = true;

				break;
			}
			case SS_LocalTeleport:
			{
				if(!memoryObj)
					return false;

				if(memoryObjSize != sizeof(CPlayerObject))
				   return false;

				CPlayerObject * playerObject = (CPlayerObject *)memoryObj;

				playerObject->m_3dPosition.x = (float)atof(m_params[0]);
				playerObject->m_3dPosition.y = (float)atof(m_params[1]);
				playerObject->m_3dPosition.z = (float)atof(m_params[2]);
				playerObject->m_Target3dPosition = playerObject->m_3dPosition;
				playerObject->m_allowLargeMovement = true;
				break;
			}
			case SS_SendText:
			{
				if(!memoryObj)
					return false;

				if(memoryObjSize != sizeof(CPlayerObject))
				   return false;

				CPlayerObject * playerObject = (CPlayerObject *)memoryObj;

				if(!playerObject->m_aiControlled)
				{
				   sNet->SendTextMsg(DPNSEND_GUARANTEED,playerObject->m_currentID,m_params[0],0,playerObject,0);
				}
				break;
			}
			case SS_SetHP:
			{
				if(!memoryObj)
					return false;
				
				if(memoryObjSize != sizeof(CPlayerObject))
				   return false;
				
				CPlayerObject *playerObject = (CPlayerObject*)memoryObj;

				playerObject->m_energy = (int)(((float)atof(m_params[0]) * 0.01f) * playerObject->m_maxEnergy);
				break;
			}
			case SS_SpawnAI:
			{
				if(!memoryObj)
					return false;

				if(memoryObjSize != sizeof(int))
				   return false;

				int * dynChannel = (int*)memoryObj;
				
				D3DVECTOR position;
				position.x = (float)atof(m_params[2]);
				position.y = (float)atof(m_params[3]);
				position.z = (float)atof(m_params[4]);

				BOOL res = sNet->SpawnDynamicAIToAvailAiServer(	atoi(m_params[0]),//aicfg
																*dynChannel, //channel
																position,
																*dynChannel,//net id
																120000000,//life time
																-1,
																20,//max spawn per id
																0,//array count
																0,//array offset
																360,//spawn rotation range
																m_params[1]);//exg name 
				break;
			}
			case SS_AddPoi:
			{
				if(!memoryObj)
					return false;

				if(memoryObjSize != sizeof(int))
				   return false;

				int * dynChannel = (int*)memoryObj;

				D3DVECTOR position;
				position.x = (float)atof(m_params[1]);
				position.y = (float)atof(m_params[2]);
				position.z = (float)atof(m_params[3]);

				sNet->m_poiSystem->Add_safe(*dynChannel,(float)atof(m_params[0]),position);
				break;
			}
			case SS_AddItem:
			{
				if(!memoryObj)
					return false;

				if(memoryObjSize != sizeof(CPlayerObject))
				   return false;

				CPlayerObject * playerObject = (CPlayerObject *)memoryObj;

				if(!playerObject->m_currentInventory->CanTakeObjects(sNet->m_netComObject->m_maxItemLimit_onChar,1))
					return false;
			        
				int glid = atoi(m_params[0]);
				CItemObj* glInvObj = sNet->m_itemManager->GetItemByID_LOS(glid);
				if(glInvObj)
				{
					if(sNet->m_worldChannelRuleDB->DoesThisRuleExistInThisChannel_safe(playerObject->m_stencilChannel,
																				playerObject->m_currentWorldID,
																				WORLDRULE_HIDE_INVENTORY_ON_SPAWN))
					{
						if(!playerObject->m_arenaItemsGiven)
						{
							if(glInvObj->m_disarmable)
								playerObject->m_currentInventory->AddInventoryItem(1, TRUE, glInvObj);
							else
								playerObject->m_currentInventory->AddInventoryItem(1, FALSE, glInvObj);
						}
					}
					else
						playerObject->m_currentInventory->AddInventoryItem(1, FALSE, glInvObj);
					sNet->m_itemManager->Unlock();
				}
				else
				{//error could not find item by glid, 
					sNet->m_eventLog->LogByPriority("CServerScriptSystem::DoFunction script using invalid GLID(unknown item) AddItem Hook",(DWORD)glid,2);
				}
				break;
			}
			case SS_BroadcastText:
			{

				if(!memoryObj)
					return false;

				if(memoryObjSize != sizeof(CPlayerObject))
				   return false;

				CPlayerObject * playerObject = (CPlayerObject *)memoryObj;


				sNet->BroadcastMessage(	atoi(m_params[0]),
										m_params[1],
										playerObject->m_3dPosition,
										playerObject->m_currentWorldID,
										playerObject->m_networkAssignedID);

				break;
			}
	}

	return true;
}

bool CServerScriptSystem::GetNextFunctionData(int & currentPosition,int & scriptLength, char * cArray, CString & cmd, CString & paramsBlock)
{
   int flag = 0;
   while(currentPosition < scriptLength)
   {
      if(cArray[currentPosition] == '(')
	  {
	     flag = 1;
		 currentPosition++;
         continue;
	  }

	  if(cArray[currentPosition] == ')')
	  {
	     currentPosition++;
		 if(cmd.GetLength() == 0)
		    return false;

	     return true;
	  }

      if(cArray[currentPosition] == ' ' && cmd.GetLength() == 0)
	  {
         currentPosition++;
         continue;
	  }

	  if(cArray[currentPosition] != 13 && cArray[currentPosition] != 10)
	  {
	     if(flag == 0)
	        cmd += cArray[currentPosition];
		 else
		    paramsBlock += cArray[currentPosition];
	  }
	  currentPosition++;
   }

   return false;
}


bool CServerScriptSystem::AddScript(CString scriptIDName, 
									int scriptID, 
									int scriptBasis, 
									CString scriptBody, 
									CString gameDescription,
									CString gameSpecific,
									int edbSpecific)
{
    CServerScripts * newScript		= new CServerScripts();
	newScript->m_gameDescription	= gameDescription;
	newScript->m_scriptBasis		= scriptBasis;
	newScript->m_scriptBody			= scriptBody;
	newScript->m_scriptID			= scriptID;
	newScript->m_scriptName			= scriptIDName;
	newScript->m_GameSpecific       = gameSpecific;
	newScript->m_edbSpecific        = edbSpecific;

	AddTail(newScript);

    return true;
}

int CServerScriptSystem::AquireFromDatabase(void * databaseObject)
{
    CDBConnectionSystem * connPtr = (CDBConnectionSystem *)databaseObject;

	_RecordsetPtr rdPtr = connPtr->Query("SELECT id,scriptName,Description,scriptData,scriptBasis,gamespecific,edbspecific from tblServerScripts");
	if(rdPtr==NULL)
	{
		 return 0;
	}
	int recordCount = connPtr->GetCount(rdPtr);

    DeleteAllScripts();//clear all first

	for(int i=0;i<recordCount;i++)
	{

        AddScript(connPtr->GetStringData("scriptName",rdPtr),//CString scriptIDName, 
			      connPtr->GetIntData("id",rdPtr),//int scriptID, 
				  connPtr->GetIntData("scriptBasis",rdPtr),//int scriptBasis, 
				  connPtr->GetStringData("scriptData",rdPtr),//CString scriptBody, 
				  connPtr->GetStringData("Description",rdPtr),
				  connPtr->GetStringData("GameSpecific",rdPtr),
				  connPtr->GetIntData("edbSpecific",rdPtr));//CString gameDescription);
		connPtr->NextRecord(rdPtr);
	}

	connPtr->Close(rdPtr);
    return recordCount;
}

CServerScripts * CServerScriptSystem::GetByUID(int uid)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
		CServerScripts * script = (CServerScripts *)GetNext(posLoc);
		if(script->m_scriptID == uid)
		   return script;
	}
	return NULL;
}

CServerScripts * CServerScriptSystem::GetByUID(int uid, CString gameSpecific, int edbSpecific)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
		CServerScripts * script = (CServerScripts *)GetNext(posLoc);
		if(script->m_scriptID == uid)
		{
		  if(edbSpecific > -1)
		  if(script->m_edbSpecific != edbSpecific)
		     continue;
		   
		  if(script->m_GameSpecific.CompareNoCase(gameSpecific)==0)
		      return script;
		}
	}
	return NULL;
}

int CServerScriptSystem::GetUIDByName(CString ScriptName)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
		CServerScripts * script = (CServerScripts *)GetNext(posLoc);
		if(script->m_scriptName.CompareNoCase(ScriptName) == 0)
		   return script->m_scriptID;
	}
	return -9;
}

void CServerScriptSystem::DeleteAllScripts()
{
	EnterCriticalSection(&m_sect);
	for(POSITION posLoc = GetHeadPosition();posLoc!= NULL;)
	{
       CServerScripts * svPtr = (CServerScripts *)GetNext(posLoc);
	   delete svPtr;
	   svPtr = 0;
	}
	RemoveAll();
	LeaveCriticalSection(&m_sect);
}

bool CServerScriptSystem::MacroHandler(CString & script, void * playerObjectRef,void * sharedInterfaceRef)
{
   if(!playerObjectRef)
       return false;

   CSharedNet * sharedNet = (CSharedNet *)sharedInterfaceRef;
   CPlayerObject * playerObject = (CPlayerObject*)playerObjectRef;

   if(playerObject)
   {
      script.Replace("$CHARNAME",playerObject->m_charName);

   }
      
   script.Replace("$G_URL",sharedNet->m_globalRefURL);

   return true;
}
