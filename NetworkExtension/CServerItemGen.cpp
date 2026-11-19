#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "comutil.h"
#include "CServerItemGen.h"
#include "CABFunctionLib.h"
#include "CWorldAvailableClass.h"
#include "ItemManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CServerItemGenObj::CServerItemGenObj()
{
    m_serverItem          = NULL;
    m_existsOnServer      = FALSE;
    m_respawnDuration     = 0;
    m_timeStamp           = GetTickCount();
	m_spawnGenID          = 0;
	m_genName             = "NONE";
	m_lastCallbackUpdate  = GetTickCount();
	m_refSpawnGenID       = -1;
	
}

BOOL CServerItemGenObjList::CloneChannelToNewChannel_safe(int stencilChannel, int targetChannel)
{
    CAxList * channelStencil = GetAllOnIndex(stencilChannel,"channel");
    if(!channelStencil)
	    return FALSE;

	CAxList * channelTarget = GetAllOnIndex(targetChannel,"channel");
    if(!channelTarget)
	    return FALSE;

	Lock();
    for(CAxLinkObj * pos = channelStencil->GetHeadPosition_unsafe();pos != NULL;)
	{
         CServerItemGenObj * spnPtr = (CServerItemGenObj *)channelStencil->GetNext_unsafe(&pos);
		 CServerItemGenObj * cloneApnPtr = NULL;
		 spnPtr->Clone(&cloneApnPtr);
		 Unlock();
		 cloneApnPtr->m_refSpawnGenID			= spnPtr->m_spawnGenID;
		 cloneApnPtr->m_serverItem->m_world		= targetChannel;
		 cloneApnPtr->m_spawnGenID				= GetUniqID_safe();
		 Add_u(cloneApnPtr);//this is ok because it is outside this cell
		 Lock();
		 Sleep(0);
	}
	Unlock();
	return TRUE;
}

BOOL CServerItemGenObjList::InformGeneratorThatItCanRespawn(int generatorID)
{
	 CAxList * obj = GetAllOnIndex(generatorID,"spawnid");
     for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
	 {
         CServerItemGenObj * spnPtr = (CServerItemGenObj *)obj->GetNext_unsafe(&pos);
		 if(spnPtr->m_spawnGenID == generatorID)
		 {
			spnPtr->m_existsOnServer = FALSE;
		    spnPtr->m_timeStamp = GetTickCount();
            return TRUE;
		 }
	 }
	 return FALSE;
}

int CServerItemGenObjList::GetUniqID_safe()
{
    if(m_spawnGenIDSearch >= 60000)
	   m_spawnGenIDSearch = 30000;

    while(VerifySpawnID_safe(m_spawnGenIDSearch))
	{
      m_spawnGenIDSearch++;
	  if(m_spawnGenIDSearch >= 60000)
	     m_spawnGenIDSearch = 30000;
	}
    return m_spawnGenIDSearch;
}

//-------------------------------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
bool CServerItemGenObjList::GetFromSQL(CServerEventList *eventLog,void * itemGenMem)
{
	CABFunctionLib cabLib;
	ConnectionStructureClass connectionStructTemp;
    connectionStructTemp.AquireFromCfgFile("baseAccount.cfg");
    CDBConnectionSystem * dbConnection = new CDBConnectionSystem(connectionStructTemp.m_sqlServerIP,
			                                                     connectionStructTemp.m_sqlServerCatalog,
																 connectionStructTemp.m_sqlServerUsername,
																 connectionStructTemp.m_sqlServerPassword,
																 eventLog);


	if(!dbConnection->m_connectionEstablished)
	{
		if(eventLog)
	       eventLog->AddEvent("GetFromSQL() Aborted due to connection not being established");
        return FALSE;
	}
    //clear existing
	//this->SafeDelete();//clears link list
	if(eventLog)
	   eventLog->AddEvent("GetFromSQL() Existing IGN Cleared:",(DWORD)this->GetCount());

    //set up GET querys
    CString getEntities				= "SELECT * FROM GM_IGNENTITY";
	
    //aquire from SQL server
 
	_RecordsetPtr entReader = dbConnection->Query(getEntities);
	int entityCount = dbConnection->GetCount(entReader);
	if(eventLog)
	   eventLog->AddEvent("GetFromSQL() Static AI to load:",(DWORD)entityCount);
    for(int eloop=0;eloop<entityCount;eloop++)
	{
	    CServerItemGenObj * spnEnt = GetIGNEntityFromReader(entReader,dbConnection,cabLib,itemGenMem,eventLog);
		if(spnEnt)
		   Add_safe(spnEnt);
       
		dbConnection->NextRecord(entReader);
	}
	dbConnection->Close(entReader);
	if(eventLog)
       eventLog->AddEvent("GetFromSQL() Completed..");

	delete dbConnection;
	dbConnection = 0;

	return true;
}

//-------------------------------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
bool CServerItemGenObjList::GetFromSQL(CServerEventList *eventLog,void * itemGenMem,int spawnID,void * worldAvailMem)
{
	CWorldAvailableObjList * zoneWorldList = (CWorldAvailableObjList *)worldAvailMem;
	CABFunctionLib cabLib;
	ConnectionStructureClass connectionStructTemp;
    connectionStructTemp.AquireFromCfgFile("baseAccount.cfg");
	if(!m_dbConnection)
    m_dbConnection = new CDBConnectionSystem(connectionStructTemp.m_sqlServerIP,
			                                                     connectionStructTemp.m_sqlServerCatalog,
																 connectionStructTemp.m_sqlServerUsername,
																 connectionStructTemp.m_sqlServerPassword,
																 eventLog);


	if(!m_dbConnection->m_connectionEstablished)
	{
        delete m_dbConnection;
		m_dbConnection = 0;

		if(eventLog)
	       eventLog->AddEvent("GetFromSQL(SERVERID SPECIFIC) Aborted due to connection not being established");
        return FALSE;
	}

	CString spawnIDstring = cabLib.ConvToString(spawnID);
   

    //set up GET querys
    CString getEntities				= "SELECT * FROM GM_IGNENTITY WHERE SPAWNID = '"+spawnIDstring+"'";
	
    //aquire from SQL server
 
	_RecordsetPtr entReader = m_dbConnection->Query(getEntities);
	int entityCount = m_dbConnection->GetCount(entReader);
	if(eventLog)
	   eventLog->AddEvent("GetFromSQL(SERVERID SPECIFIC) Static AI to load:",(DWORD)entityCount);
    if(entityCount>0)
	{
	    CServerItemGenObj * spnEnt = GetIGNEntityFromReader(entReader,m_dbConnection,cabLib,itemGenMem,eventLog);
	
		if(!UpdateObjectOrAdd_safe(spnEnt,spawnID))
		{
            eventLog->AddEvent("UpdateObjectOrAdd_safe() Failed..");
		}
		//ensure generator exists in each clone of the stencil zone-this is ussually needed when a new entity is added
		//spnEnt->m_serverItem->m_world;//stencil channel
		//spnEnt->m_spawnGenID;//stencil spawn gen will corrilate to instances as the refspawngenid

		zoneWorldList->Lock();
        CAxList * iList = zoneWorldList->GetAllOnIndex(spnEnt->m_serverItem->m_world,"channel_ref");
	    if(iList)
	    for(CAxLinkObj * posLoc = iList->GetHeadPosition_unsafe();posLoc != NULL;)
	    {//loop through all zones cloned from stencil 
		   CWorldAvailableObj * wPtr = (CWorldAvailableObj *)iList->GetNext_unsafe(&posLoc);
		   if(wPtr->m_channelRef == spnEnt->m_serverItem->m_world)
		   {
			  //next verify generator with refSpawngen id exists in channel :: spnEnt->m_spawnGenID
			  if(!VerifyRefSpawnIDbyChannel_safe(spnEnt->m_spawnGenID, wPtr->m_id))
			  {//must add it
                 CServerItemGenObj * newPtr = NULL;
				 spnEnt->Clone(&newPtr);
				 newPtr->m_spawnGenID = GetUniqID_safe();
				 newPtr->m_serverItem->m_world = wPtr->m_id;
				 newPtr->m_refSpawnGenID = spnEnt->m_spawnGenID;
				 eventLog->AddEvent("GetFromSQL() Added new object to channel:",(DWORD)newPtr->m_serverItem->m_world);
                 Add_safe(newPtr);
			  }//end must add it
		   }
	    }
		zoneWorldList->Unlock();

	}
	else
	{
		eventLog->AddEvent("GetFromSQL(SERVERID SPECIFIC) could not find entity to update..");
	    m_dbConnection->Close(entReader);
		return false;
	}
	m_dbConnection->Close(entReader);
	if(eventLog)
       eventLog->AddEvent("GetFromSQL(SERVERID SPECIFIC) update base questCompleted..");

	

	return true;
}

CServerItemGenObj * CServerItemGenObjList::GetIGNEntityFromReader(_RecordsetPtr & entReader,
																  CDBConnectionSystem * dbConnection,
																  CABFunctionLib & cabLib,
																  void * itemGenMem,
																  CServerEventList *eventLog)
{

	    CItemManager * iManager = (CItemManager *)itemGenMem;
        CString temp;
		CString getQuestsForEntity		= "SELECT * FROM GM_IGNQUESTDATA WHERE SPAWNID = '$SPAWNID' ORDER BY QUESTINDEX";
        CString getInventoryByQuest		= "SELECT * FROM GM_IGNINVENTORY WHERE SPAWNID = '$SPAWNID' AND QUESTINDEX = '$QUESTINDEX'";
		CString getEntInventoryByID		= "SELECT * FROM GM_IGNINVENTORY WHERE SPAWNID = '$SPAWNID' AND QUESTINDEX = '-1'";
	    CString getChatStringsByEntity	= "SELECT * FROM GM_IGNCHATSTRINGS WHERE SPAWNID = '$SPAWNID' ORDER BY CHATINDEX";
		CString getDimCfgs				= "SELECT * FROM GM_IGNDIMCFGS WHERE SPAWNID = '$SPAWNID' ORDER BY INDEXDIM";
		CString getDynRules				= "SELECT * FROM GM_IGNDYNAMICRULES WHERE SPAWNID = '$SPAWNID' AND QUESTINDEX = '$QUESTINDEX'";

        CServerItemGenObj * spnGenPtr	= new CServerItemGenObj();
		spnGenPtr->m_serverItem			= new CServerItemObj();
        
		spnGenPtr->m_genName								= SanitizeFormatOut(dbConnection->GetStringData("GENERATORNAME",entReader));
		spnGenPtr->m_respawnDuration						= dbConnection->GetIntData("RESPAWNDURATION",entReader);
        spnGenPtr->m_spawnGenID								= dbConnection->GetIntData("SPAWNID",entReader);
		spnGenPtr->m_serverItem->m_autoTrigger				= dbConnection->GetIntData("AUTOTRIGGER",entReader);
		spnGenPtr->m_serverItem->m_chatStringCount			= dbConnection->GetIntData("CHATSTRINGCOUNT",entReader);
		
		spnGenPtr->m_serverItem->m_collectionQuestDB		= new CAICollectionQuestList();
		spnGenPtr->m_serverItem->m_contents					= new CInventoryObjList();
		spnGenPtr->m_serverItem->m_curAnim					= dbConnection->GetIntData("CURANIMATION",entReader);
		spnGenPtr->m_serverItem->m_dbRef					= dbConnection->GetIntData("EDBID",entReader);
		spnGenPtr->m_serverItem->m_displayedName			= SanitizeFormatOut(dbConnection->GetStringData("DISPLAYNAME",entReader));
		spnGenPtr->m_serverItem->m_glid						= dbConnection->GetIntData("GLID",entReader);
		spnGenPtr->m_serverItem->m_interactRadius			= dbConnection->GetfloatData("INTERACTRADIUS",entReader);
		spnGenPtr->m_serverItem->m_mountOption				= dbConnection->GetIntData("MOUNTOPTION",entReader);
		spnGenPtr->m_serverItem->m_noResponseMsg		    = SanitizeFormatOut(dbConnection->GetStringData("NORESPONSEMSG",entReader));
		spnGenPtr->m_serverItem->m_organizeGroupID			= dbConnection->GetIntData("ORGANIZEGROUPID",entReader);
		spnGenPtr->m_serverItem->m_position.x			    = dbConnection->GetfloatData("POSX",entReader);
		spnGenPtr->m_serverItem->m_position.y			    = dbConnection->GetfloatData("POSY",entReader);
		spnGenPtr->m_serverItem->m_position.z			    = dbConnection->GetfloatData("POSZ",entReader);
		spnGenPtr->m_serverItem->m_useExactOrientation		= dbConnection->GetBoolData("useExactOrientation",entReader);
		spnGenPtr->m_serverItem->m_upVector.x				= dbConnection->GetfloatData("upVectorX",entReader);
		spnGenPtr->m_serverItem->m_upVector.y				= dbConnection->GetfloatData("upVectorY",entReader);
		spnGenPtr->m_serverItem->m_upVector.z				= dbConnection->GetfloatData("upVectorZ",entReader);
		spnGenPtr->m_serverItem->m_dirVector.x				= dbConnection->GetfloatData("dirVectX",entReader);
		spnGenPtr->m_serverItem->m_dirVector.y				= dbConnection->GetfloatData("dirVectY",entReader);
		spnGenPtr->m_serverItem->m_dirVector.z				= dbConnection->GetfloatData("dirVectZ",entReader);
		spnGenPtr->m_serverItem->m_randomAnimation          = dbConnection->GetIntData("RANDOMANIMATION",entReader);
        spnGenPtr->m_serverItem->m_randomExpEffect          = dbConnection->GetIntData("RANDOMEXPEFFECT",entReader);
		spnGenPtr->m_serverItem->m_randomInterval			= dbConnection->GetIntData("RANDOMINTERVAL",entReader);
		spnGenPtr->m_serverItem->m_requiredKey				= dbConnection->GetIntData("REQUIREDKEY",entReader);
		spnGenPtr->m_serverItem->m_rotationAdjust			= dbConnection->GetIntData("ROTATIONADJUST",entReader);
		spnGenPtr->m_serverItem->m_spawnCfgCount			= dbConnection->GetIntData("DIMCFGCOUNT",entReader);
		spnGenPtr->m_serverItem->m_botBrain			        = dbConnection->GetStringData("BOTBRAIN",entReader);
        spnGenPtr->m_serverItem->m_spawnID					= dbConnection->GetIntData("SPAWNID",entReader);
		spnGenPtr->m_serverItem->m_staticItemType			= (ServerItemTypes)dbConnection->GetIntData("ITEMTYPE",entReader);
		spnGenPtr->m_serverItem->m_spawnID					= dbConnection->GetIntData("SPAWNID",entReader);
		spnGenPtr->m_serverItem->m_world					= dbConnection->GetIntData("WORLDCHANNEL",entReader);
		spnGenPtr->m_serverItem->m_sightRange				= dbConnection->GetfloatData("SIGHTRANGE",entReader);

        //dim cfgs
		temp = getDimCfgs;
		temp.Replace("$SPAWNID", cabLib.ConvToString(spnGenPtr->m_serverItem->m_spawnID));
		_RecordsetPtr dimReader = dbConnection->Query(temp);
	    int entityDimCount = dbConnection->GetCount(dimReader);
		spnGenPtr->m_serverItem->m_spawnCfgCount = entityDimCount;
		if(spnGenPtr->m_serverItem->m_spawnCfgCount > 0)
		{
			spnGenPtr->m_serverItem->m_spawnCfg			    = new int[spnGenPtr->m_serverItem->m_spawnCfgCount];
			ZeroMemory(spnGenPtr->m_serverItem->m_spawnCfg,sizeof(int)*spnGenPtr->m_serverItem->m_spawnCfgCount);
		}
		for(int dloop=0;dloop<entityDimCount;dloop++)
		{
		    spnGenPtr->m_serverItem->m_spawnCfg[dloop] = dbConnection->GetIntData("DIMCFG",dimReader);
			dbConnection->NextRecord(dimReader);
		}
		dbConnection->Close(dimReader);

        //chat strings
        temp = getChatStringsByEntity;
		temp.Replace("$SPAWNID", cabLib.ConvToString(spnGenPtr->m_serverItem->m_spawnID));
		_RecordsetPtr chatReader = dbConnection->Query(temp);
	    int entityChatCount = dbConnection->GetCount(chatReader);
		spnGenPtr->m_serverItem->m_chatStringCount = entityChatCount;
		if(spnGenPtr->m_serverItem->m_chatStringCount > 0) 
		   spnGenPtr->m_serverItem->m_chatStrings			= new CString[spnGenPtr->m_serverItem->m_chatStringCount];
			for(int eloop=0;eloop<entityChatCount;eloop++)
			{
			    spnGenPtr->m_serverItem->m_chatStrings[eloop] = SanitizeFormatOut(dbConnection->GetStringData("CHATSTRING",chatReader));
				dbConnection->NextRecord(chatReader);
			}
		dbConnection->Close(chatReader);

		//contents- armed items
        temp = getEntInventoryByID;
		temp.Replace("$SPAWNID", cabLib.ConvToString(spnGenPtr->m_serverItem->m_spawnID));
		_RecordsetPtr eInvReader = dbConnection->Query(temp);
	    int entityInvCount = dbConnection->GetCount(eInvReader);
		int gIdTemp = 0;
        for(int eloop=0;eloop<entityInvCount;eloop++)
	    {
			gIdTemp = dbConnection->GetIntData("GLID",eInvReader);
			CItemObj * invGLPtr = iManager->GetItemByID_LOS(gIdTemp);
		    if(invGLPtr)
			{
			   spnGenPtr->m_serverItem->m_contents->AddInventoryItem(1,dbConnection->GetIntData("ARMED",eInvReader),invGLPtr);
               iManager->Unlock();
			}
			else
			{
				eventLog->AddEvent("GetIGNEntityFromReader ENTITY referenced armed items/equipment ids that do not exist in APX Name:"+spnGenPtr->m_genName+" glid:",(DWORD)gIdTemp);
			}
			dbConnection->NextRecord(eInvReader);
			
		}
		dbConnection->Close(eInvReader);


		//quests
        temp = getQuestsForEntity;
		temp.Replace("$SPAWNID", cabLib.ConvToString(spnGenPtr->m_serverItem->m_spawnID));
		_RecordsetPtr questReader = dbConnection->Query(temp);
	    int questCount = dbConnection->GetCount(questReader);
		for(int qLoop = 0;qLoop<questCount;qLoop++)
		{
			CAICollectionQuestObj * qPtr = new CAICollectionQuestObj();
            //int questIndex						= dbConnection->GetIntData("QUESTINDEX",questReader);
			qPtr->m_index						= dbConnection->GetIntData("QUESTINDEX",questReader);
			qPtr->m_completedKeyEnabled			= dbConnection->GetIntData("COMPLETEDKEYENABLED",questReader);
			qPtr->m_title						= SanitizeFormatOut(dbConnection->GetStringData("TITLE",questReader));
			qPtr->m_questDesription				= SanitizeFormatOut(dbConnection->GetStringData("QUESTDESCRIPTION",questReader));
			qPtr->m_questCompletionMessage		= SanitizeFormatOut(dbConnection->GetStringData("QUESTCOMPLETIONMESSAGE",questReader));
			qPtr->m_type						= dbConnection->GetIntData("TYPE",questReader);
			qPtr->m_skillTypeBonus				= dbConnection->GetIntData("SKILLTYPEBONUS",questReader);
			qPtr->m_expBonus					= dbConnection->GetfloatData("EXPBONUS",questReader);
			qPtr->m_maxExpLimit					= dbConnection->GetfloatData("MAXEXPLIMIT",questReader);
			qPtr->m_spawnAiCfg					= dbConnection->GetIntData("SPAWNAICFG",questReader);
			qPtr->m_spawnPosition.x				= dbConnection->GetfloatData("SPAWNPOSX",questReader);
			qPtr->m_spawnPosition.y				= dbConnection->GetfloatData("SPAWNPOSY",questReader);
			qPtr->m_spawnPosition.z				= dbConnection->GetfloatData("SPAWNPOSZ",questReader);
			qPtr->m_spawnArrayCountOption       = dbConnection->GetIntData("SPAWNARRAYCOUNT",questReader);
            qPtr->m_spawnArrayOffset			= dbConnection->GetfloatData("SPAWNARRAYOFFSET",questReader);
			qPtr->m_spawnRotationRange			= dbConnection->GetIntData("SPAWNROTATIONRANGE", questReader);
			qPtr->m_worldID						= dbConnection->GetIntData("WORLDID",questReader);
			qPtr->m_spawnedAiLifeline			= dbConnection->GetIntData("SPAWNEDAILIFELINE",questReader);
			qPtr->m_randomRewardType			= dbConnection->GetIntData("RANDOMREWARDTYPE",questReader);
			qPtr->m_spawnToLocationOverride		= dbConnection->GetIntData("SPAWNTOLOCATIONOVERRIDE",questReader);
			qPtr->m_makeTargetRebirthPoint		= dbConnection->GetIntData("MAKETARGETREBIRTHPOINT",questReader);
			qPtr->m_raceSpecificAccess			= dbConnection->GetIntData("RACESPECIFICACCESS",questReader);
			qPtr->m_optionalCommand				= SanitizeFormatOut(dbConnection->GetStringData("OPTIONALCOMMAND",questReader));
			qPtr->m_miscAction					= dbConnection->GetIntData("MISCACTION",questReader);
			qPtr->m_progessBasedByQuestJournal	= dbConnection->GetIntData("PROGRESSIONBASEDBYQUESTJOURNAL",questReader);
			qPtr->m_cashReward					= dbConnection->GetIntData("CASHREWARD",questReader);
			qPtr->m_requiredSkillType			= dbConnection->GetIntData("REQUIREDSKILLTYPE",questReader);
			qPtr->m_requiredSkillLevel			= dbConnection->GetIntData("REQUIREDSKILLLEVEL",questReader);
			qPtr->m_mobToHuntID					= dbConnection->GetIntData("MOBTOHUNTID",questReader);
			qPtr->m_maxDropCount				= dbConnection->GetIntData("MAXDROPCOUNT",questReader);
			qPtr->m_dropGlid					= dbConnection->GetIntData("DROPGLID",questReader);
			qPtr->m_dropPercentage				= dbConnection->GetIntData("DROPPERCENTAGE",questReader);
			qPtr->m_autocomplete				= dbConnection->GetIntData("AUTOCOMPLETE",questReader);
			qPtr->m_openDialogWindow			= dbConnection->GetIntData("OPENDIALOGWINDOW",questReader);
			qPtr->m_spawnOnFail					= dbConnection->GetIntData("SPAWNONFAIL",questReader);
			qPtr->m_optionalIntruction			= SanitizeFormatOut(dbConnection->GetStringData("OPTIONALINSTRUCTION",questReader));
			qPtr->m_listProgressionOptions		= dbConnection->GetIntData("LISTPROGRESSIONOPTIONS",questReader);
            qPtr->m_undeletable					= dbConnection->GetIntData("UNDELETABLE",questReader);
            qPtr->m_requireOwnershipOfKingdomIndex = dbConnection->GetIntData("MUSTOWNKINGDOMID",questReader);
			qPtr->m_expansionIDReq				= dbConnection->GetIntData("EXPIDREQUIRED",questReader);
			qPtr->m_portalLimitMsg				= dbConnection->GetStringData("PORTALVSMESSAGE",questReader);
			qPtr->m_clientScript				= dbConnection->GetStringData("CLIENTSCRIPT",questReader);
			qPtr->m_requiredScript				= dbConnection->GetStringData("REQUIREDSCRIPT",questReader);
			qPtr->m_rewardScript				= dbConnection->GetStringData("REWARDSCRIPT",questReader);
			qPtr->m_miscString                  = dbConnection->GetStringData("EXPFAILMESSAGE",questReader);

			qPtr->m_clearInventory				= dbConnection->GetIntData("clearInventory",questReader);
			qPtr->m_deleteAllByRequiredGLID		= dbConnection->GetIntData("deleteAllByRequiredGLID",questReader);
			qPtr->m_deteteAllByRewardGLID	    = dbConnection->GetIntData("deleteAllByRewardGLID",questReader);
			qPtr->m_deleteByLocationID			= dbConnection->GetIntData("deleteByLocationID",questReader);

			qPtr->m_despawnOnReward				= dbConnection->GetIntData("despawnFlag",questReader);
			qPtr->m_flagTimer					= dbConnection->GetIntData("timeStampFlag",questReader);
			qPtr->m_requiredTimeStamp           = dbConnection->GetIntData("requiredTimeStamp",questReader);

			qPtr->m_closeJournalEntryByName     = SanitizeFormatOut(dbConnection->GetStringData("CLOSEJOURNALENTRYBYNAME",questReader));
			qPtr->m_maxDmgMod                   = dbConnection->GetIntData("maxDmgMod",questReader);
			qPtr->m_minDmgMod                   = dbConnection->GetIntData("minDmgMod",questReader);
			qPtr->m_flagHostileOnSuccess        = dbConnection->GetIntData("FLAGHOSTILEONSUCCESS",questReader);
			qPtr->m_spawnToWorldName			= dbConnection->GetStringData("spawnWorldName",questReader);

			qPtr->m_instanceGenerateType		= dbConnection->GetIntData("instanceGenerateType",questReader); 
			qPtr->m_instanceGenerateParam		= (DWORD)dbConnection->GetIntData("instanceGenerateParam",questReader);
			qPtr->m_instanceGenerateParam2		= (DWORD)dbConnection->GetIntData("instanceGenerateParam2",questReader);
			qPtr->m_instanceGenerateParam3		= (DWORD)dbConnection->GetIntData("instanceGenerateParam3",questReader);
			qPtr->m_instanceGenerateParam4		= (DWORD)dbConnection->GetIntData("instanceGenerateParam4",questReader);

			//quest inventory reward/required
			qPtr->m_requiredItems	= new CInventoryObjList();
			qPtr->m_reward			= new CInventoryObjList();
	
			CString finalQuerry = getInventoryByQuest;
		    finalQuerry.Replace("$SPAWNID", cabLib.ConvToString(spnGenPtr->m_serverItem->m_spawnID));
			//finalQuerry.Replace("$QUESTINDEX", cabLib.ConvToString(questIndex));
			finalQuerry.Replace("$QUESTINDEX", cabLib.ConvToString(qPtr->m_index));
		    _RecordsetPtr questInvReader = dbConnection->Query(finalQuerry);
	        int questInvCount = dbConnection->GetCount(questInvReader);

		    for(int qInvLoop=0;qInvLoop<questInvCount;qInvLoop++)
			{
				int type = dbConnection->GetIntData("TYPE",questInvReader);
				gIdTemp = dbConnection->GetIntData("GLID",questInvReader);
                CItemObj * invGLPtr = iManager->GetItemByID_LOS(gIdTemp);
				if(invGLPtr)	
				{
				   if(type == 1)
				      qPtr->m_requiredItems->AddInventoryItem(1,dbConnection->GetIntData("ARMED",questInvReader),invGLPtr);
				   else if(type == 2)
                      qPtr->m_reward->AddInventoryItem(1,dbConnection->GetIntData("ARMED",questInvReader),invGLPtr);

				   iManager->Unlock();
				}
				else
				{//a required item is specified but no asset of this type exists
					eventLog->AddEvent("GetIGNEntityFromReader ENTITY referenced required/reward items ids that do not exist in APX Name:"+spnGenPtr->m_genName);
				}
                dbConnection->NextRecord(questInvReader);
			}
			dbConnection->Close(questInvReader);
			//get dynamic rules
			CString dynRules = getDynRules;
			dynRules.Replace("$SPAWNID", cabLib.ConvToString(spnGenPtr->m_serverItem->m_spawnID));
			//dynRules.Replace("$QUESTINDEX", cabLib.ConvToString(questIndex));
			dynRules.Replace("$QUESTINDEX", cabLib.ConvToString(qPtr->m_index));
            _RecordsetPtr questDynRuleReader = dbConnection->Query(dynRules);
	        qPtr->m_ruleCount = dbConnection->GetCount(questDynRuleReader);

			if(qPtr->m_ruleCount > 0)
			   qPtr->m_rules = new DYNRULES[qPtr->m_ruleCount];
		    for(int dynRLoop=0;dynRLoop<qPtr->m_ruleCount;dynRLoop++)
			{
				qPtr->m_rules[dynRLoop].m_rule = dbConnection->GetIntData("RULEID",questDynRuleReader);
				qPtr->m_rules[dynRLoop].m_param = dbConnection->GetIntData("PARAM1",questDynRuleReader);
                dbConnection->NextRecord(questDynRuleReader);
			}
			dbConnection->Close(questDynRuleReader);

			//add quest object
			spnGenPtr->m_serverItem->m_collectionQuestDB->AddTail(qPtr);
			
            dbConnection->NextRecord(questReader);
		}
        dbConnection->Close(questReader);


     return spnGenPtr;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----converts a long into string-----------------------------//
CString CServerItemGenObjList::ConvToString(int number)
{    
              CString temp;
			  temp.Format("%i",number);
		
			  return temp;
}

CString CServerItemGenObjList::SanitizeFormatOut(CString text)
{
    char quoteX[2];
    quoteX[0] = '"';
    quoteX[1] = 0;
	text.Replace(quoteX,"'");

    return text;
}


CString CServerItemGenObjList::SanitizeFormatIn(CString text)
{
    char quoteX[2];
    quoteX[0] = '"';
    quoteX[1] = 0;
	text.Replace("'",quoteX);

    return text;
}




