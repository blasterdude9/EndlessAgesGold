#pragma once
//#include "afxdb.h"


#import "C:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "EndOfFile")

#include "CServerEventLogClass.h"


class  ConnectionStructureClass
{
   public:
   ConnectionStructureClass(){};
   ~ConnectionStructureClass(){};
   CString   m_sqlServerIP;
   CString   m_sqlServerCatalog;
   CString   m_sqlServerUsername;
   CString   m_sqlServerPassword;

   //functions
   bool AquireFromCfgFile(CString path);
};

class  CDBConnectionSystem
{
public:
	CDBConnectionSystem(CString sqlSvrIP,
						CString catalog,
						CString slqUsername,
						CString sqlServerPassword,
						CServerEventList * m_eventLog,
						int timeout = 15);
	~CDBConnectionSystem(void);


    //Vars
	_ConnectionPtr					m_pConnection;
    BOOL                            m_connectionEstablished;
	
	CString							m_commandBuild;
	CServerEventList              * m_eventLogRef;
	DWORD                           m_dbReconnectDuration;
	DWORD                           m_dbReconnectTimeStamp;
	CRITICAL_SECTION                m_cSection;


	//Functions
	void EstablishConnection(int timeout = 15);
	void Reconnect(DWORD & currentTickCount);
	CString GetStringData(CString varName,_RecordsetPtr & rcdSet);
	bool GetBoolData(CString varName,_RecordsetPtr & rcdSet);
	int GetIntData(CString varName,_RecordsetPtr & rcdSet);
	float GetfloatData(CString varName,_RecordsetPtr & rcdSet);
	_RecordsetPtr Query(CString query);
	void NextRecord(_RecordsetPtr & rcdSet);
	int GetCount(_RecordsetPtr & rcdSet);
	void Log(CString msg);
	void Close(_RecordsetPtr & rcdSet);
	bool ExecQuery(CString query);
	bool InlineReconnect(CString & currentError);
	CString SanitizeFormatOut(CString text);
    CString SanitizeFormatIn(CString text);
	CString SanitizeFormatInENC(CString text);
    CString SanitizeFormatOutENC(CString text);
	inline char* ConvertBSTRToString(BSTR pSrc);
	inline BSTR ConvertStringToBSTR(const char* pSrc);

};
