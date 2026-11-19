#include "stdafx.h"
#include <direct.h>
#include "aiserverinitialization.h"

CAIServerInitialization::CAIServerInitialization(void)
{
	 CoInitialize(NULL);
     ZeroMemory(&m_aiServerStatus,sizeof(AB_aiServerStatusLive));
     CString CurDir;
     char buffer[_MAX_PATH];
     _getcwd( buffer, _MAX_PATH );
     m_dataPath = buffer;
     m_dataPath = m_dataPath+"\\GameFiles\\";

	 m_eventLog     = new CServerEventList();     
     m_clientModule = new CClientModule(NULL,0,0,0,0,0,0,1,FALSE,FALSE,FALSE,FALSE,2048,2,TRUE);
     if(m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_eventLog)
	 {
        m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_eventLog->SafeDelete();
		delete m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_eventLog;
		m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_eventLog = 0;
	 }

     m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_eventLog = m_eventLog;
     m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_AIServer = TRUE;

	 m_systemShutDown = FALSE;
	 InitializeCriticalSection(  &m_interactSection );
	 InitializeCriticalSection(  &m_statusQueryLock );
	

	 
	 //connect to server
	   //Establish base settings
       m_adoConnection = new CDBConnectionSystem(m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_sqlServerIP,
												 m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_sqlServerCatalog,
												 m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_sqlServerUsername,
												 m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_sqlServerPassword,
												 m_eventLog,
												 15);   
	   if(!m_adoConnection->m_connectionEstablished)
		  m_eventLog->AddEvent("Connection to sql could not be established");
}

int CAIServerInitialization::ConnectToServer(int waitTime)
{
    LoadSharedInterfaces("tblDataLoadConfig","all");//ENABLE
	LoadSharedInterfaces("tblDataLoadConfig","aiserver");//ENABLE

	_RecordsetPtr rdPtr = m_adoConnection->Query("select * from tblMainConfiguration");
	if(rdPtr==NULL)
	{
         m_eventLog->AddEvent("No datalists to load");
		 return 0;
	}
    m_eventLog->AddEvent("Main Configuration aquired");
    int version = m_adoConnection->GetIntData("Version",rdPtr);
    CString serverIP = m_adoConnection->GetStringData("MainServerIP",rdPtr);
    CString username = m_adoConnection->GetStringData("AIserverUsername",rdPtr);
    CString password = m_adoConnection->GetStringData("AIserverpassword",rdPtr);
	m_aiResetTime = (DWORD)m_adoConnection->GetIntData("AIResetDuration",rdPtr);
	if(m_aiResetTime < 30000)
	   m_aiResetTime = 10800000;
    //set username password
	m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_userName = username;
    m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_passWord = password;
	m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->EnableClient(serverIP,version);
    m_adoConnection->Close(rdPtr);

	int tracer = 0;
	while(1)
	{
	    m_eventLog->AddEvent("Attempting connection...");
        Sleep(3000);//wait for enumerated host to establish connection

		if(m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->GetEnabledClient_safe()==TRUE)
		{//connected ok
           m_eventLog->AddEvent("Connection Established...");
           return 1;
		}//end connected ok

        tracer++;
		if(tracer > waitTime)
		{
		    m_eventLog->AddEvent("Connection to server has timed out...");
		    return 2;//Timeout reached
		}
	}

	return 0;
}

BOOL CAIServerInitialization::LoadSharedInterfaces(CString tableName, CString filter)
{
     _RecordsetPtr rdPtr = m_adoConnection->Query("select * from "+tableName+" where usedBy = '"+filter+"'");
	 if(rdPtr==NULL)
	 {
         m_eventLog->AddEvent("No datalists to load");
		 return FALSE;
	 }
	 CString temp;
	 int recordCount = m_adoConnection->GetCount(rdPtr);
	 for(int i=0;i<recordCount;i++)
	 {
         temp = m_adoConnection->GetStringData("filename",rdPtr);
         if(!LoadDataByType(temp))m_eventLog->AddEvent(temp+" failed to load");
		 else m_eventLog->AddEvent(temp+" loaded");

		 m_adoConnection->NextRecord(rdPtr);
	 }
	 m_adoConnection->Close(rdPtr);

	return TRUE;
}

CAIServerInitialization::~CAIServerInitialization(void)
{
    //m_eventLog->OutPutTextFile("Log.txt");

	if(m_adoConnection)
	{
	   delete m_adoConnection;
	   m_adoConnection = 0;
    }
	if(m_clientModule)
	{
	   delete m_clientModule;
	   m_clientModule = 0;
	}
	
	
	DeleteCriticalSection(&m_interactSection);
	DeleteCriticalSection(&m_statusQueryLock);
	CoUninitialize();
}

//--------------------------load lists/databases----------------------------/
BOOL CAIServerInitialization::LoadDataByType(CString filename)
{
    CString extension = m_clientModule->m_initObject->m_gStatus->m_cabLib.GetFileExt(filename);
	CFileException exc;
    CFile fl;

	

	filename = m_dataPath + filename;
	if(extension.CompareNoCase("mis")==0)
	{//Missile Database
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_clientModule->m_initObject->m_gStatus->m_missileDB)
	 {
		m_clientModule->m_initObject->m_gStatus->m_missileDB->SafeDelete();
	    delete m_clientModule->m_initObject->m_gStatus->m_missileDB;
	    m_clientModule->m_initObject->m_gStatus->m_missileDB = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_clientModule->m_initObject->m_gStatus->m_missileDB;
	 the_in_Archive.Close();
     fl.Close();
	 m_clientModule->m_initObject->m_gStatus->m_missileDB->CompileDirectAccess();
	 return TRUE;
	}//End Missile Database
	if(extension.CompareNoCase("apx")==0)
	{//item manager
     if(m_clientModule->m_initObject->m_gStatus->m_itemManager)
          delete m_clientModule->m_initObject->m_gStatus->m_itemManager;
	   m_clientModule->m_initObject->m_gStatus->m_itemManager = 0;

	   m_clientModule->m_initObject->m_gStatus->m_itemManager = new CItemManager();

	   return m_clientModule->m_initObject->m_gStatus->m_itemManager->Load(filename.GetBuffer());
	}//End Missile Database
	else if(extension.CompareNoCase("exp")==0)
	{//Explosion Database
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_clientModule->m_initObject->m_gStatus->m_explosionDBList)
	 {
		m_clientModule->m_initObject->m_gStatus->m_explosionDBList->SafeDelete();
	    delete m_clientModule->m_initObject->m_gStatus->m_explosionDBList;
	    m_clientModule->m_initObject->m_gStatus->m_explosionDBList = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_clientModule->m_initObject->m_gStatus->m_explosionDBList;
	 the_in_Archive.Close();
     fl.Close();
	 m_clientModule->m_initObject->m_gStatus->m_explosionDBList->CompileDirectAccess();
	 return TRUE;
	}//End Explosion Database
	else if(extension.CompareNoCase("air")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_clientModule->m_initObject->m_gStatus->m_aiRaidDatabase)
	 {
		m_clientModule->m_initObject->m_gStatus->m_aiRaidDatabase->SafeDelete();
	    delete m_clientModule->m_initObject->m_gStatus->m_aiRaidDatabase;
	    m_clientModule->m_initObject->m_gStatus->m_aiRaidDatabase = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_clientModule->m_initObject->m_gStatus->m_aiRaidDatabase;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	else if(extension.CompareNoCase("hsf")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_clientModule->m_initObject->m_gStatus->m_housingDB)
	 {
		m_clientModule->m_initObject->m_gStatus->m_housingDB->SafeDelete();
	    delete m_clientModule->m_initObject->m_gStatus->m_housingDB;
	    m_clientModule->m_initObject->m_gStatus->m_housingDB = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_clientModule->m_initObject->m_gStatus->m_housingDB;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	else if(extension.CompareNoCase("pzn")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_clientModule->m_initObject->m_gStatus->m_portalDatabase)
	 {
		m_clientModule->m_initObject->m_gStatus->m_portalDatabase->SafeDelete();
	    delete m_clientModule->m_initObject->m_gStatus->m_portalDatabase;
	    m_clientModule->m_initObject->m_gStatus->m_portalDatabase = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_clientModule->m_initObject->m_gStatus->m_portalDatabase;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	else if(extension.CompareNoCase("cdb")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_clientModule->m_initObject->m_gStatus->m_commerceDB)
	 {
		m_clientModule->m_initObject->m_gStatus->m_commerceDB->SafeDelete();
	    delete m_clientModule->m_initObject->m_gStatus->m_commerceDB;
	    m_clientModule->m_initObject->m_gStatus->m_commerceDB = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_clientModule->m_initObject->m_gStatus->m_commerceDB;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	else if(extension.CompareNoCase("eld")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_clientModule->m_initObject->m_gStatus->m_elementDB)
	 {
		m_clientModule->m_initObject->m_gStatus->m_elementDB->SafeDelete();
	    delete m_clientModule->m_initObject->m_gStatus->m_elementDB;
	    m_clientModule->m_initObject->m_gStatus->m_elementDB = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_clientModule->m_initObject->m_gStatus->m_elementDB;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	else if(extension.CompareNoCase("skl")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_clientModule->m_initObject->m_gStatus->m_skillDatabase)
	 {
		m_clientModule->m_initObject->m_gStatus->m_skillDatabase->SafeDelete();
	    delete m_clientModule->m_initObject->m_gStatus->m_skillDatabase;
	    m_clientModule->m_initObject->m_gStatus->m_skillDatabase = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_clientModule->m_initObject->m_gStatus->m_skillDatabase;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	else if(extension.CompareNoCase("wrf")==0)
	{
     //obsolete
	 return TRUE;
	}
	else if(extension.CompareNoCase("aii")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_clientModule->m_initObject->m_gStatus->m_aiList)
	 {
		m_clientModule->m_initObject->m_gStatus->m_aiList->SafeDelete();
	    delete m_clientModule->m_initObject->m_gStatus->m_aiList;
	    m_clientModule->m_initObject->m_gStatus->m_aiList = 0;
	 }

	 if(m_clientModule->m_initObject->m_gStatus->m_scrList)
	 {
		m_clientModule->m_initObject->m_gStatus->m_scrList->SafeDelete();
	    delete m_clientModule->m_initObject->m_gStatus->m_scrList;
	    m_clientModule->m_initObject->m_gStatus->m_scrList = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_clientModule->m_initObject->m_gStatus->m_aiList >> m_clientModule->m_initObject->m_gStatus->m_scrList;
	 the_in_Archive.Close();
     fl.Close();
	 m_clientModule->m_initObject->m_gStatus->m_aiList->ResetIndexReferences();
	 return TRUE;
	}
	else if(extension.CompareNoCase("edb")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_clientModule->m_initObject->m_gStatus->m_entObjList)
	 {
		m_clientModule->m_initObject->m_gStatus->m_entObjList->SafeDelete();
	    delete m_clientModule->m_initObject->m_gStatus->m_entObjList;
	    m_clientModule->m_initObject->m_gStatus->m_entObjList = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_clientModule->m_initObject->m_gStatus->m_entObjList;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	else if(extension.CompareNoCase("szn")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_clientModule->m_initObject->m_gStatus->m_safeZoneDB)
	 {
		m_clientModule->m_initObject->m_gStatus->m_safeZoneDB->SafeDelete();
	    delete m_clientModule->m_initObject->m_gStatus->m_safeZoneDB;
	    m_clientModule->m_initObject->m_gStatus->m_safeZoneDB = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_clientModule->m_initObject->m_gStatus->m_safeZoneDB;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	else if(extension.CompareNoCase("crd")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_clientModule->m_initObject->m_gStatus->m_collisionDatabase)
	 {
		m_clientModule->m_initObject->m_gStatus->m_collisionDatabase->SafeDelete();
	    delete m_clientModule->m_initObject->m_gStatus->m_collisionDatabase;
	    m_clientModule->m_initObject->m_gStatus->m_collisionDatabase = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_clientModule->m_initObject->m_gStatus->m_collisionDatabase;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	else if(extension.CompareNoCase("hpb")==0)
	{
     if(!fl.Open(filename,CFile::modeRead, &exc))
		return FALSE;
     
	 if(m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_houseingDB)
	 {
		m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_houseingDB->SafeDelete();
	    delete m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_houseingDB;
	    m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_houseingDB = 0;
	 }
	   
	 CArchive the_in_Archive(&fl,CArchive::load);
	 the_in_Archive >> m_clientModule->m_initObject->m_gStatus->m_multiplayerObject->m_houseingDB;
	 the_in_Archive.Close();
     fl.Close();
	 return TRUE;
	}
	
	return FALSE;
}
