#pragma once
#include "CServerEventLogClass.h"
#include "DBConnectionSystem.h"
#include "CAXCabControl.h"
#include <tlhelp32.h>
#include <stdio.h>
#include "urlreader.h"



class CMainFunctions
{
public:
	CMainFunctions(void);
	~CMainFunctions(void);
	CMainFunctions(CString runtimepath);

	//Variables
	CRITICAL_SECTION m_secureSection;
	CDBConnectionSystem * m_dmConnection;
	CString m_sqlServerIP;
	CString m_sqlServerUsername;
	CString m_sqlServerPassword;
	CString m_sqlServerCatalog;
	CString m_sqlServerTable;
	CString m_boxID;
	BOOL    m_systemShutDown;
	CServerEventList * m_eventLog;
	CString m_installPath;
	DWORD m_restartDuration;
	DWORD m_restartStamp;


	//Functions

    BOOL ExtractCabinetToDir(CString cabinetPath, CString extractPath);
	BOOL SendCmdLine(CString syscmd);
	BOOL AquireConnectionString();
	BOOL RebootServer();
	BOOL RenameFile(CString filePath, CString newNameAndPath);
	BOOL ReplaceTextInFile(CString filePath, CString searchText, CString replacementText);
	BOOL DeleteDirectory(CString dirPath);
	BOOL GetProcessList();
	BOOL GetProcessModule(DWORD dwPID, DWORD dwModuleID, LPMODULEENTRY32 lpMe32, DWORD cbMe32);
	BOOL CloseProcessByExeName(CString exeName);
	void AddSvrCommand(CString ipRouting, CString catagoriePeram1, CString peram2, CString peram3, CString peram4, CString peram5);
    void MakeDirectory(LPSTR path);
	BOOL ProcessCommands(int param1,CString param2,CString param3,CString param4,CString param5);
	BOOL StartUpAllServersThatareValid();

};
