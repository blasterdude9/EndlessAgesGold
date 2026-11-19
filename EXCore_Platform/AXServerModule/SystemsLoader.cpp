#include "StdAfx.h"
#include "direct.h"
#include "systemsloader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSystemsLoader::CSystemsLoader(CSharedNet * sharedInterface)
{
   CString CurDir;
   char buffer[_MAX_PATH];
   _getcwd( buffer, _MAX_PATH );
   m_dataPath = buffer;
   m_dataPath = m_dataPath+"\\GameFiles\\";

   m_sharedInterfaceRef = sharedInterface;

   
}

CSystemsLoader::~CSystemsLoader(void)
{
  

}
//--------------------------------------------------------------------------/
void CSystemsLoader::ProcessMainCfg(CString tableName)
{
     _RecordsetPtr rdPtr = m_sharedInterfaceRef->m_dbConnection->Query("select * from "+tableName);
	 if(rdPtr==NULL)
	 {
         m_sharedInterfaceRef->m_eventLog->AddEvent("No datalists to load");
		 return;
	 }
	 int recordCount = m_sharedInterfaceRef->m_dbConnection->GetCount(rdPtr);
	 if(recordCount == 0)
	 {
        m_sharedInterfaceRef->m_eventLog->AddEvent("No Main Configuration aquired");
	    return;
	 }
	    m_sharedInterfaceRef->m_eventLog->AddEvent("Main Configuration aquired");
		
        m_sharedInterfaceRef->m_netComObject->m_serverID = m_sharedInterfaceRef->m_dbConnection->GetIntData("ServerID",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("Server ID set: ",(DWORD)m_sharedInterfaceRef->m_netComObject->m_serverID);
		if(m_sharedInterfaceRef->m_netComObject->m_serverID < 1)
		{//correct for backwards compatibility
           m_sharedInterfaceRef->m_netComObject->m_serverID = 1;
           m_sharedInterfaceRef->m_eventLog->AddEvent("Server ID corrected due to invalid serverID or no var in DB: ",(DWORD)m_sharedInterfaceRef->m_netComObject->m_serverID);
		}//end correct for backward compatibility

        m_sharedInterfaceRef->m_netComObject->m_currentVersion = m_sharedInterfaceRef->m_dbConnection->GetIntData("Version",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("Version set: ",(DWORD)m_sharedInterfaceRef->m_netComObject->m_currentVersion);

		m_sharedInterfaceRef->m_netComObject->m_maxPlayers = m_sharedInterfaceRef->m_dbConnection->GetIntData("Max_Players",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("Max Players set: ",(DWORD)m_sharedInterfaceRef->m_netComObject->m_maxPlayers);

		m_sharedInterfaceRef->m_netComObject->m_frequencyLimit = m_sharedInterfaceRef->m_dbConnection->GetIntData("Frequency",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("Frequency set: ",(DWORD)m_sharedInterfaceRef->m_netComObject->m_frequencyLimit);

		m_sharedInterfaceRef->m_netComObject->m_frequencyBuffer = m_sharedInterfaceRef->m_dbConnection->GetIntData("Freq_Adjust",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("Frequency Adjust set: ",(DWORD)m_sharedInterfaceRef->m_netComObject->m_frequencyBuffer);

		m_sharedInterfaceRef->m_netComObject->m_accountDBMode = m_sharedInterfaceRef->m_dbConnection->GetIntData("ACCOUNTDB_MODE",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("Account DB Mode set: ",(DWORD)m_sharedInterfaceRef->m_netComObject->m_accountDBMode);

		m_sharedInterfaceRef->m_netComObject->m_frequencyIncrement = m_sharedInterfaceRef->m_dbConnection->GetIntData("Freq_Incr",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("Frequency Inrement set: ",(DWORD)m_sharedInterfaceRef->m_netComObject->m_frequencyIncrement);

		m_sharedInterfaceRef->m_netComObject->m_serverCalculateRadius = m_sharedInterfaceRef->m_dbConnection->GetfloatData("Entity_Vis_Radius",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("Entity Visual Radius set: ",(DWORD)m_sharedInterfaceRef->m_netComObject->m_serverCalculateRadius);

		m_sharedInterfaceRef->m_netComObject->m_serverCalculateRadiusItems = m_sharedInterfaceRef->m_dbConnection->GetfloatData("Item_Vis_Radius",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("Item Visual Radius set: ",(DWORD)m_sharedInterfaceRef->m_netComObject->m_serverCalculateRadiusItems);

		m_sharedInterfaceRef->m_netComObject->m_deadEntityDissolveTime = m_sharedInterfaceRef->m_dbConnection->GetIntData("Dead_PCEntity_dissolve_Time",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("Dead PC Dissolve time set: ",(DWORD)m_sharedInterfaceRef->m_netComObject->m_deadEntityDissolveTime);

		m_sharedInterfaceRef->m_netComObject->m_deadEntityNPCDissolveTime = m_sharedInterfaceRef->m_dbConnection->GetIntData("Dead_NPCEntity_dissolve_Time",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("Dead NPC Dissolve time set: ",(DWORD)m_sharedInterfaceRef->m_netComObject->m_deadEntityNPCDissolveTime);

		m_sharedInterfaceRef->m_netComObject->m_aiserverCycleDuration = m_sharedInterfaceRef->m_dbConnection->GetIntData("AIServer_Cycle",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("Aiserver cycle set: ",(DWORD)m_sharedInterfaceRef->m_netComObject->m_aiserverCycleDuration);

		m_sharedInterfaceRef->m_netComObject->m_murderCountMax = m_sharedInterfaceRef->m_dbConnection->GetIntData("Murder_Level_Count",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("Murder Count set: ",(DWORD)m_sharedInterfaceRef->m_netComObject->m_murderCountMax);

		m_sharedInterfaceRef->m_netComObject->m_goodBehaviorDuration = m_sharedInterfaceRef->m_dbConnection->GetIntData("Workoff_murder_rate",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("Murder work off rate set: ",(DWORD)m_sharedInterfaceRef->m_netComObject->m_goodBehaviorDuration);

		m_sharedInterfaceRef->m_netComObject->m_autoBackupFile = m_sharedInterfaceRef->m_dbConnection->GetStringData("Backup_file_name",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("Backup file name set: ",m_sharedInterfaceRef->m_netComObject->m_autoBackupFile);

		m_sharedInterfaceRef->m_netComObject->m_autoBackupFrequency = m_sharedInterfaceRef->m_dbConnection->GetIntData("Duration_between_backups",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("Backup duration set: ",(DWORD)m_sharedInterfaceRef->m_netComObject->m_autoBackupFrequency);

		m_sharedInterfaceRef->m_netComObject->m_bandwidthAvailable = m_sharedInterfaceRef->m_dbConnection->GetIntData("Available_Bandwidth",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("Available Bandwidth set: ",(DWORD)m_sharedInterfaceRef->m_netComObject->m_bandwidthAvailable);

		m_sharedInterfaceRef->m_netComObject->m_enableAutoBackup = m_sharedInterfaceRef->m_dbConnection->GetIntData("enableBackupSystem",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("Enable Backup: ",(DWORD)m_sharedInterfaceRef->m_netComObject->m_enableAutoBackup);

		m_sharedInterfaceRef->m_netComObject->m_sqlStatusServerUsername = m_sharedInterfaceRef->m_dbConnection->GetStringData("sqlStatusServerUsername",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("SQL_STATUSUsername: ",m_sharedInterfaceRef->m_netComObject->m_sqlStatusServerUsername);

		m_sharedInterfaceRef->m_netComObject->m_sqlStatusServerCatalog = m_sharedInterfaceRef->m_dbConnection->GetStringData("sqlStatusServerCatalog",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("sqlStatusServerCatalog: ",m_sharedInterfaceRef->m_netComObject->m_sqlStatusServerCatalog);

		m_sharedInterfaceRef->m_dbIgnMode = m_sharedInterfaceRef->m_dbConnection->GetIntData("ignDbMode",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("ignDbMode set: ",(DWORD)m_sharedInterfaceRef->m_dbIgnMode);
		
		m_sharedInterfaceRef->m_netComObject->m_sqlStatusServerIP = m_sharedInterfaceRef->m_dbConnection->GetStringData("sqlStatusServerIP",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("sqlStatusServerIP: ",m_sharedInterfaceRef->m_netComObject->m_sqlStatusServerIP);

		m_sharedInterfaceRef->m_netComObject->m_sqlStatusServerPassword = m_sharedInterfaceRef->m_dbConnection->GetStringData("sqlStatusServerPassword",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("sqlStatusServerPassword: ","*****");

		m_sharedInterfaceRef->m_globalRefURL = m_sharedInterfaceRef->m_dbConnection->GetStringData("globalUrlRef",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("global reference url loaded: ",m_sharedInterfaceRef->m_globalRefURL);

		m_sharedInterfaceRef->m_globalBotUrl = m_sharedInterfaceRef->m_dbConnection->GetStringData("globalBotUrl",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("global bot url loaded: ",m_sharedInterfaceRef->m_globalBotUrl);
		
	    m_sharedInterfaceRef->m_netComObject->m_sqlStatusServerTable = m_sharedInterfaceRef->m_dbConnection->GetStringData("sqlStatusServerTable",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("sqlStatusServerTable: ",m_sharedInterfaceRef->m_netComObject->m_sqlStatusServerTable);

		m_sharedInterfaceRef->m_eventLog->m_logPriority = m_sharedInterfaceRef->m_dbConnection->GetIntData("LogPriorityLevel",rdPtr);
		m_sharedInterfaceRef->m_eventLog->AddEvent("Log Priority Set: ",(DWORD)m_sharedInterfaceRef->m_eventLog->m_logPriority);

		int temp = m_sharedInterfaceRef->m_dbConnection->GetIntData("RegenTurnSpeed",rdPtr);
		if(temp > 0)m_sharedInterfaceRef->m_regenerationTurnSpeed = temp;
        
		temp = m_sharedInterfaceRef->m_dbConnection->GetIntData("DefaultHealthRegenAmount",rdPtr);
		if(temp > 0)m_sharedInterfaceRef->m_defaultHealthRegenAmount = temp;

		temp = m_sharedInterfaceRef->m_dbConnection->GetIntData("LootMode",rdPtr);
		if(temp > 0)m_sharedInterfaceRef->m_lootMode = temp;//0=simple 1=damage based

		temp = m_sharedInterfaceRef->m_dbConnection->GetIntData("AgressorMaxTrack",rdPtr);
		if(temp > 0)m_sharedInterfaceRef->m_aggressorMaxTrack = temp;

		temp = m_sharedInterfaceRef->m_dbConnection->GetIntData("MaxDotTrack", rdPtr);
		if(temp > 0)m_sharedInterfaceRef->m_maxDotTrack = temp;

		temp = m_sharedInterfaceRef->m_dbConnection->GetIntData("DamageMode", rdPtr);
		if(temp > 0)m_sharedInterfaceRef->m_damageMode = temp;

		temp = m_sharedInterfaceRef->m_dbConnection->GetIntData("RespawnHealthPercentage", rdPtr);
		if(temp > 0)m_sharedInterfaceRef->m_respawnHealthPercentage = (float)temp;

		temp = m_sharedInterfaceRef->m_dbConnection->GetIntData("teamGlidNotifier", rdPtr);
		if(temp > 0)m_sharedInterfaceRef->m_teamGlidNotifier = temp;

		temp = m_sharedInterfaceRef->m_dbConnection->GetIntData("allowOnlyPaidAccountsToConnect", rdPtr);
		if(temp > 0)m_sharedInterfaceRef->m_netComObject->m_allowOnlyPaidAccountsToConnect = temp;	

		temp = m_sharedInterfaceRef->m_dbConnection->GetIntData("primaryProcessingThreadCount", rdPtr);
		if(temp > 0)m_sharedInterfaceRef->m_primaryProcessingThreadCount = temp;	

		temp = m_sharedInterfaceRef->m_dbConnection->GetIntData("maxCharacterCreate", rdPtr);
		if(temp > 0)m_sharedInterfaceRef->m_maxCharacterCreate = temp;

		temp = m_sharedInterfaceRef->m_dbConnection->GetIntData("pkLevelRestriction", rdPtr);
		if(temp > 0)m_sharedInterfaceRef->m_netComObject->m_pkLevelRestriction = temp;

		temp = m_sharedInterfaceRef->m_dbConnection->GetIntData("factionArenaBonus", rdPtr);
		if(temp > -1)m_sharedInterfaceRef->m_arenaFactionBonus = temp;

		temp = m_sharedInterfaceRef->m_dbConnection->GetIntData("factionPVPBonus", rdPtr);
		if(temp > -1)m_sharedInterfaceRef->m_pvpFactionBonus = temp;

		temp = m_sharedInterfaceRef->m_dbConnection->GetIntData("factionGuildBonus", rdPtr);
		if(temp > -1)m_sharedInterfaceRef->m_clanFactionBonus = temp;

		temp = m_sharedInterfaceRef->m_dbConnection->GetIntData("groupGlidNotifier", rdPtr);
		if(temp > 0)m_sharedInterfaceRef->m_groupGlidNotifier = temp;

}
//--------------------------------------------------------------------------/
void CSystemsLoader::ProcessLoadDataTable(CString tableName)
{
     _RecordsetPtr rdPtr = m_sharedInterfaceRef->m_dbConnection->Query("select * from "+tableName);
	 if(rdPtr==NULL)
	 {
         m_sharedInterfaceRef->m_eventLog->AddEvent("No datalists to load");
		 return;
	 }
	 CString temp;
	 int recordCount = m_sharedInterfaceRef->m_dbConnection->GetCount(rdPtr);
	 for(int i=0;i<recordCount;i++)
	 {
         temp = m_sharedInterfaceRef->m_dbConnection->GetStringData("filename",rdPtr);
		 m_sharedInterfaceRef->m_realtimeLogger.WriteDebugFile("Loading:",0,0);
		 m_sharedInterfaceRef->m_realtimeLogger.WriteDebugFile(temp.GetBuffer(),0,1);
         if(!LoadDataByType(temp))m_sharedInterfaceRef->m_eventLog->AddEvent(temp+" failed to load");
		 else m_sharedInterfaceRef->m_eventLog->AddEvent(temp+" loaded");

		 m_sharedInterfaceRef->m_dbConnection->NextRecord(rdPtr);
	 }
	 m_sharedInterfaceRef->m_dbConnection->Close(rdPtr);
}
//--------------------------load supportedWorlds----------------------------/
BOOL CSystemsLoader::LoadSupportedWorldsFromDB()
{
	 /*_RecordsetPtr rdPtr = m_sharedInterfaceRef->m_dbConnection->Query("select id, worldName, worldChannel,ai_worldname from tblSupportedWorlds order by id");
	 if(rdPtr==NULL)
	 {
         m_sharedInterfaceRef->m_eventLog->AddEvent("LoadSupportedWorldsFromDB() cannot aquire db data");
		 return FALSE;
	 }
     int recordCount = m_sharedInterfaceRef->m_dbConnection->GetCount(rdPtr);
	 if(recordCount == 0)
	 {
		m_sharedInterfaceRef->m_dbConnection->Close(rdPtr);
        m_sharedInterfaceRef->m_eventLog->AddEvent("LoadSupportedWorldsFromDB() no data available");
	    return FALSE;
	 }
	 m_sharedInterfaceRef->m_eventLog->AddEvent("LoadSupportedWorldsFromDB() aquired:",(DWORD)recordCount);*/


     if(m_sharedInterfaceRef->m_netComObject->m_currentWorldsUsed)
	 {//clear 
		delete m_sharedInterfaceRef->m_netComObject->m_currentWorldsUsed;
		m_sharedInterfaceRef->m_netComObject->m_currentWorldsUsed = 0;
	 }//end clear
     
	 m_sharedInterfaceRef->m_netComObject->m_currentWorldsUsed = new CWorldAvailableObjList();
	 m_sharedInterfaceRef->m_netComObject->m_currentWorldsUsed->InitConnection(m_sharedInterfaceRef->m_baseAccountDBConnection,m_sharedInterfaceRef->m_cabLib.ConvToString(m_sharedInterfaceRef->m_netComObject->m_serverID),m_sharedInterfaceRef->m_eventLog);
     m_sharedInterfaceRef->m_netComObject->m_currentWorldsUsed->ClearDB();
	 /*for(int loop=0;loop<recordCount;loop++)
	 {
		int id					= m_sharedInterfaceRef->m_dbConnection->GetIntData("id",rdPtr);
		CString worldName		= m_sharedInterfaceRef->m_dbConnection->GetStringData("worldName",rdPtr);
		int worldChannel		= m_sharedInterfaceRef->m_dbConnection->GetIntData("worldChannel",rdPtr);
		CString ai_worldName	= m_sharedInterfaceRef->m_dbConnection->GetStringData("ai_worldname",rdPtr);
		int instanceType = 0;

		m_sharedInterfaceRef->m_eventLog->AddEvent(worldName + " Channel:",(DWORD)worldChannel);
		m_sharedInterfaceRef->m_netComObject->m_currentWorldsUsed->AddZone_safe(worldName,worldChannel,instanceType,ai_worldName);
		m_sharedInterfaceRef->m_dbConnection->NextRecord(rdPtr);
	 }*/

	 //m_sharedInterfaceRef->m_dbConnection->Close(rdPtr);
	 return TRUE;
}
//--------------------------load supportedWorlds----------------------------/
BOOL CSystemsLoader::LoadSupportedArenaItems()
{
	_RecordsetPtr rdPtr = m_sharedInterfaceRef->m_dbConnection->Query("select id, globalID, itemName from tblArenaItems order by id");
	if(rdPtr==NULL)
	{
		m_sharedInterfaceRef->m_eventLog->AddEvent("LoadSupportedArenaItems() cannot aquire db data");
		return FALSE;
	}
	int recordCount = m_sharedInterfaceRef->m_dbConnection->GetCount(rdPtr);
	
	if(recordCount == 0)
	{
		m_sharedInterfaceRef->m_eventLog->AddEvent("LoadSupportedArenaItems() no data available");
		return FALSE;
	}
	m_sharedInterfaceRef->m_eventLog->AddEvent("LoadSupportedArenaItems() aquired: ",(DWORD)recordCount);


	if(m_sharedInterfaceRef->m_arenaItems)
	{//clear 
		delete [] m_sharedInterfaceRef->m_arenaItems;
		m_sharedInterfaceRef->m_arenaItems = 0;
	}//end clear

	m_sharedInterfaceRef->m_arenaItems = new int[recordCount];
	m_sharedInterfaceRef->m_arenaItemCount = recordCount;

	for(int loop = 0; loop < m_sharedInterfaceRef->m_arenaItemCount; loop++)
	{
		int globalID		= m_sharedInterfaceRef->m_dbConnection->GetIntData("globalID",rdPtr);
		CString itemName	= m_sharedInterfaceRef->m_dbConnection->GetStringData("itemName",rdPtr);

		m_sharedInterfaceRef->m_arenaItems[loop] = globalID;

		m_sharedInterfaceRef->m_eventLog->AddEvent(itemName + " Loaded.");
		m_sharedInterfaceRef->m_dbConnection->NextRecord(rdPtr);
	}

	m_sharedInterfaceRef->m_dbConnection->Close(rdPtr);
	return TRUE;
}
//--------------------------load supportedWorlds----------------------------/
BOOL CSystemsLoader::LoadSpawnPointsFromDB()
{
	 _RecordsetPtr rdPtr = m_sharedInterfaceRef->m_dbConnection->Query("select SpawnPointName, posX, posY, posZ, worldID from tblSpawnPoints where spawnpointname = 'NewbZone'");
	 if(rdPtr==NULL)
	 {
         m_sharedInterfaceRef->m_eventLog->AddEvent("LoadSpawnPointsFromDB() cannot aquire db data");
		 return FALSE;
	 }
     int recordCount = m_sharedInterfaceRef->m_dbConnection->GetCount(rdPtr);
	 if(recordCount == 0)
	 {
        m_sharedInterfaceRef->m_eventLog->AddEvent("LoadSpawnPointsFromDB() no data available");
		m_sharedInterfaceRef->m_dbConnection->Close(rdPtr);
	    return FALSE;
	 }
	 m_sharedInterfaceRef->m_eventLog->AddEvent("LoadSpawnPointsFromDB() aquired:",(DWORD)recordCount);


     if(m_sharedInterfaceRef->m_netComObject->m_spawnPointCfgs)
	 {//clear 
        m_sharedInterfaceRef->m_netComObject->m_spawnPointCfgs->SafeDelete();
		delete m_sharedInterfaceRef->m_netComObject->m_spawnPointCfgs;
		m_sharedInterfaceRef->m_netComObject->m_spawnPointCfgs = 0;
	 }//end clear
     
        
	 m_sharedInterfaceRef->m_netComObject->m_spawnPointCfgs = new CSpawnCfgObjList();

	 for(int loop=0;loop<recordCount;loop++)
	 {

		float positionX = m_sharedInterfaceRef->m_dbConnection->GetfloatData("posX",rdPtr);
		float positionY = m_sharedInterfaceRef->m_dbConnection->GetfloatData("posY",rdPtr);
		float positionZ = m_sharedInterfaceRef->m_dbConnection->GetfloatData("posZ",rdPtr);
		CString spawnName	= m_sharedInterfaceRef->m_dbConnection->GetStringData("SpawnPointName",rdPtr);
		int worldIndex	= m_sharedInterfaceRef->m_dbConnection->GetIntData("worldID",rdPtr);

		 
		 
		CSpawnCfgObj * spnCfg = new CSpawnCfgObj();
		spnCfg->m_position.x = positionX;
		spnCfg->m_position.y = positionY;
		spnCfg->m_position.z = positionZ;
		spnCfg->m_spawnPointName = spawnName;
		spnCfg->m_worldReferenceIndex = worldIndex;
		m_sharedInterfaceRef->m_netComObject->m_spawnPointCfgs->AddTail(spnCfg);

		m_sharedInterfaceRef->m_eventLog->AddEvent(spawnName + " worldID:",(DWORD)worldIndex);
		m_sharedInterfaceRef->m_eventLog->AddEvent("SpawnPoint:",spnCfg->m_position);

		m_sharedInterfaceRef->m_dbConnection->NextRecord(rdPtr);
	 }
	 m_sharedInterfaceRef->m_dbConnection->Close(rdPtr);
	 return TRUE;
}
//--------------------------load lists/databases----------------------------/
BOOL CSystemsLoader::LoadDataByType(CString filename)
{


    CString extension = m_sharedInterfaceRef->m_cabLib.GetFileExt(filename);
	CFileException exc;
    CFile fl;

	filename = m_dataPath + filename;
	if(extension.CompareNoCase("mis")==0)
	{//Missile Database
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_sharedInterfaceRef->m_missileDB)
	 {
		m_sharedInterfaceRef->m_missileDB->SafeDelete();
	    delete m_sharedInterfaceRef->m_missileDB;
	    m_sharedInterfaceRef->m_missileDB = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_sharedInterfaceRef->m_missileDB;
	 the_in_Archive.Close();
     fl.Close();
	 m_sharedInterfaceRef->m_missileDB->CompileDirectAccess();
	 return TRUE;
	}//End Missile Database
	if(extension.CompareNoCase("apx")==0)
	{//item manager
	   if(m_sharedInterfaceRef->m_itemManager)
          delete m_sharedInterfaceRef->m_itemManager;
	   m_sharedInterfaceRef->m_itemManager = 0;

	   m_sharedInterfaceRef->m_itemManager = new CItemManager();

	   return m_sharedInterfaceRef->m_itemManager->Load(filename.GetBuffer());
	}//End Missile Database
	else if(extension.CompareNoCase("exp")==0)
	{//Explosion Database
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_sharedInterfaceRef->m_explosionDBList)
	 {
		m_sharedInterfaceRef->m_explosionDBList->SafeDelete();
	    delete m_sharedInterfaceRef->m_explosionDBList;
	    m_sharedInterfaceRef->m_explosionDBList = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_sharedInterfaceRef->m_explosionDBList;
	 the_in_Archive.Close();
     fl.Close();
	 m_sharedInterfaceRef->m_explosionDBList->CompileDirectAccess();
	 return TRUE;
	}//End Explosion Database
	else if(extension.CompareNoCase("air")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_sharedInterfaceRef->m_aiRaidDatabase)
	 {
		m_sharedInterfaceRef->m_aiRaidDatabase->SafeDelete();
	    delete m_sharedInterfaceRef->m_aiRaidDatabase;
	    m_sharedInterfaceRef->m_aiRaidDatabase = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_sharedInterfaceRef->m_aiRaidDatabase;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	else if(extension.CompareNoCase("hsf")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_sharedInterfaceRef->m_housingDB)
	 {
		m_sharedInterfaceRef->m_housingDB->SafeDelete();
	    delete m_sharedInterfaceRef->m_housingDB;
	    m_sharedInterfaceRef->m_housingDB = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_sharedInterfaceRef->m_housingDB;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	else if(extension.CompareNoCase("acz")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_sharedInterfaceRef->m_actionZoneDB)
	 {
	    delete m_sharedInterfaceRef->m_actionZoneDB;
	    m_sharedInterfaceRef->m_actionZoneDB = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_sharedInterfaceRef->m_actionZoneDB;
	 the_in_Archive.Close();
     fl.Close();

	 m_sharedInterfaceRef->m_actionZoneDB->CompileDirectAccess();
	 return TRUE;
	}
	else if(extension.CompareNoCase("pzn")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_sharedInterfaceRef->m_portalDatabase)
	 {
		m_sharedInterfaceRef->m_portalDatabase->SafeDelete();
	    delete m_sharedInterfaceRef->m_portalDatabase;
	    m_sharedInterfaceRef->m_portalDatabase = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_sharedInterfaceRef->m_portalDatabase;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	else if(extension.CompareNoCase("cdb")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_sharedInterfaceRef->m_commerceDB)
	 {
		m_sharedInterfaceRef->m_commerceDB->SafeDelete();
	    delete m_sharedInterfaceRef->m_commerceDB;
	    m_sharedInterfaceRef->m_commerceDB = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_sharedInterfaceRef->m_commerceDB;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	else if(extension.CompareNoCase("hpb")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_sharedInterfaceRef->m_netComObject->m_houseingDB)
	 {
		m_sharedInterfaceRef->m_netComObject->m_houseingDB->SafeDelete();
	    delete m_sharedInterfaceRef->m_netComObject->m_houseingDB;
	    m_sharedInterfaceRef->m_netComObject->m_houseingDB = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_sharedInterfaceRef->m_netComObject->m_houseingDB;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	else if(extension.CompareNoCase("bzn")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_sharedInterfaceRef->m_bankZoneList)
	 {
		m_sharedInterfaceRef->m_bankZoneList->SafeDelete();
	    delete m_sharedInterfaceRef->m_bankZoneList;
	    m_sharedInterfaceRef->m_bankZoneList = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_sharedInterfaceRef->m_bankZoneList;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	else if(extension.CompareNoCase("eld")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_sharedInterfaceRef->m_elementDB)
	 {
		m_sharedInterfaceRef->m_elementDB->SafeDelete();
	    delete m_sharedInterfaceRef->m_elementDB;
	    m_sharedInterfaceRef->m_elementDB = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_sharedInterfaceRef->m_elementDB;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	else if(extension.CompareNoCase("dbd")==0)
	{
		if(!fl.Open(filename,CFile::modeRead, &exc))
			return FALSE;

		if(m_sharedInterfaceRef->m_dotDB)
		{
			m_sharedInterfaceRef->m_dotDB->SafeDelete();
			delete m_sharedInterfaceRef->m_dotDB;
			m_sharedInterfaceRef->m_dotDB = 0;
		}

		CArchive the_in_Archive(&fl, CArchive::load);
		the_in_Archive >> m_sharedInterfaceRef->m_dotDB;
		the_in_Archive.Close();
		fl.Close();
		return TRUE;
	}
	else if(extension.CompareNoCase("skl")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_sharedInterfaceRef->m_skillDatabase)
	 {
		m_sharedInterfaceRef->m_skillDatabase->SafeDelete();
	    delete m_sharedInterfaceRef->m_skillDatabase;
	    m_sharedInterfaceRef->m_skillDatabase = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_sharedInterfaceRef->m_skillDatabase;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	else if(extension.CompareNoCase("aii")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_sharedInterfaceRef->m_aiList)
	 {
		m_sharedInterfaceRef->m_aiList->SafeDelete();
	    delete m_sharedInterfaceRef->m_aiList;
	    m_sharedInterfaceRef->m_aiList = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_sharedInterfaceRef->m_aiList;
	 the_in_Archive.Close();
     fl.Close();
	 m_sharedInterfaceRef->m_aiList->ResetIndexReferences();
	 return TRUE;
	}
	else if(extension.CompareNoCase("edb")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_sharedInterfaceRef->m_entObjList)
	 {
		m_sharedInterfaceRef->m_entObjList->SafeDelete();
	    delete m_sharedInterfaceRef->m_entObjList;
	    m_sharedInterfaceRef->m_entObjList = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 //the_in_Archive.miscInt = 1;
	 the_in_Archive >> m_sharedInterfaceRef->m_entObjList;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	else if(extension.CompareNoCase("szn")==0)
	{
     /*if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_sharedInterfaceRef->m_safeZoneDB)
	 {
		m_sharedInterfaceRef->m_safeZoneDB->SafeDelete();
	    delete m_sharedInterfaceRef->m_safeZoneDB;
	    m_sharedInterfaceRef->m_safeZoneDB = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_sharedInterfaceRef->m_safeZoneDB;
	 the_in_Archive.Close();
     fl.Close();*/
	 return TRUE;
	}
	else if(extension.CompareNoCase("crd")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_sharedInterfaceRef->m_collisionDatabase)
	 {
		m_sharedInterfaceRef->m_collisionDatabase->SafeDelete();
	    delete m_sharedInterfaceRef->m_collisionDatabase;
	    m_sharedInterfaceRef->m_collisionDatabase = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_sharedInterfaceRef->m_collisionDatabase;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	else if(extension.CompareNoCase("amf")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_sharedInterfaceRef->m_battleSchedulerSys)
	 {
		m_sharedInterfaceRef->m_battleSchedulerSys->SafeDelete();
	    delete m_sharedInterfaceRef->m_battleSchedulerSys;
	    m_sharedInterfaceRef->m_battleSchedulerSys = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_sharedInterfaceRef->m_battleSchedulerSys;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	else if(extension.CompareNoCase("kdb")==0)
	{
	 return TRUE;
	}
	return FALSE;
}
