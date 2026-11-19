#pragma once
#include "Common.h"
#include <string>
#include "CAxMap.h"

enum SS_SERVER_SCRIPTS
{
	SS_AddFullJournalEntry = 5,
	SS_SetCharPosition,			
	SS_AddJournalEntry,
	SS_CloseJournalByNpc,
	SS_SetCharRebirthPosition,
	SS_SetCharArenaRebirthPosition,
	SS_ResetArenaStats,
	SS_OptimizeInventory,
	SS_SetNPCAnim,
	SS_ToggleNPCAnimDir,
	SS_SetLargeMoveFlag,
	SS_LocalTeleport,
	SS_SendText,
	SS_SetHP,
	SS_SpawnAI,
	SS_AddPoi,
	SS_AddItem,
	SS_BroadcastText,
	SS_MAX	
};

class CServerScripts : public CObject
{
public:
	CServerScripts(void){m_scriptID = 0;m_scriptBasis=0;m_edbSpecific=-1;};
	~CServerScripts(void){};


    //Vars
	CString         m_scriptName;
	CString         m_GameSpecific;
	CString         m_gameDescription;//notes on game definition
	int             m_scriptID;//unique ID
    int             m_edbSpecific;//-1 none > valid
	int				m_scriptBasis;//0=character script
	CString			m_scriptBody;//src code of script

};





class CServerScriptSystem : public CObList
{
public:

	CServerScriptSystem()
	{
		InitializeCriticalSection(&m_sect);

		//add constants
        m_axMap.AddItem(SS_AddFullJournalEntry,"AddFullJournalEntry");
		m_axMap.AddItem(SS_SetCharPosition,"SetCharPosition");
		m_axMap.AddItem(SS_AddJournalEntry,"AddJournalEntry");
		m_axMap.AddItem(SS_CloseJournalByNpc,"CloseJournalByNpc");
		m_axMap.AddItem(SS_SetCharRebirthPosition,"SetCharRebirthPosition");
		m_axMap.AddItem(SS_SetCharArenaRebirthPosition,"SetCharArenaRebirthPosition");
		m_axMap.AddItem(SS_ResetArenaStats,"ResetArenaStats");
		m_axMap.AddItem(SS_OptimizeInventory,"OptimizeInventory");
		m_axMap.AddItem(SS_SetNPCAnim,"SetNPCAnim");
		m_axMap.AddItem(SS_ToggleNPCAnimDir,"ToggleNPCAnimDir");
		m_axMap.AddItem(SS_SetLargeMoveFlag,"SetLargeMoveFlag");
		m_axMap.AddItem(SS_LocalTeleport,"LocalTeleport");
		m_axMap.AddItem(SS_SendText,"SendText");
		m_axMap.AddItem(SS_SetHP,"SetHP");
		m_axMap.AddItem(SS_SpawnAI,"SpawnAI");
		m_axMap.AddItem(SS_AddPoi,"AddPoi");
		m_axMap.AddItem(SS_AddItem,"AddItem");
		m_axMap.AddItem(SS_BroadcastText,"BroadcastText");	

	};
	~CServerScriptSystem(void)
	{
	   DeleteAllScripts();
	   DeleteCriticalSection(&m_sect);
	};

	CString m_params[100];
	CRITICAL_SECTION m_sect;
	CAXMap m_axMap;

	//functions
	void DeleteAllScripts();
	int GetUIDByName(CString ScriptName);
	bool ScriptProcessor( void * object,
					      int objectSize,
						  int scriptID,
						  CString & error,
						  CString & gameSpecific,
						  int edbIndex,
						  void * sNetMem);
	bool ScriptProcessor( void * object,
						  int objectSize,
						  void * object2, 
						  int object2Size,
						  CString scriptBody,
						  CString & error,
						  void * sNetMem);
	bool AddScript( CString scriptIDName, 
					int scriptID, 
					int scriptBasis, 
					CString scriptBody, 
					CString gameDescription,
					CString gameSpecific,
					int edbSpecific);
	CServerScripts * GetByUID(int uid, CString gameSpecific, int edbSpecific);
	int AquireFromDatabase(void * databaseObject);
    CServerScripts * GetByUID(int uid);
	bool GetNextFunctionData(int & currentPosition,int & scriptLength, char * cArray, CString & cmd, CString & paramsBlock);
	bool DoFunction( CString & command, 
					 CString & params,
					 void * memoryObj,
					 int memoryObjSize,
					 void * memoryObj2,
					 int memoryObj2Size,
					 void * sNetMem);
	DWORD GetDWORDval(CString command);
	bool FillParams(CString & paramBlock);
	bool MacroHandler(CString & script, void * playerObjectRef,void * sharedInterfaceRef);
	bool ScriptProcessor(void * object,int objectSize,int scriptID,CString & error,void * sNetMem);

};