#pragma once
//#include "afxdb.h"
#import "C:\Program Files\Common Files\System\ADO\msado15.dll" no_namespace rename("EOF", "EndOfFile")



class CDBConSystem
{
public:
	CDBConSystem(CString sqlSvrIP,
						CString catalog,
						CString slqUsername,
						CString sqlServerPassword);
	~CDBConSystem(void);


    //Vars
	_ConnectionPtr					m_pConnection;
    BOOL                            m_connectionEstablished;
	CString                         m_currentError;
	CString							m_commandBuild;
	DWORD                           m_dbReconnectDuration;
	DWORD                           m_dbReconnectTimeStamp;


	//Functions
	void EstablishConnection();
	void Reconnect(DWORD & currentTickCount);
	CString GetStringData(_bstr_t varName,_RecordsetPtr & rcdSet);
	int GetIntData(_bstr_t varName,_RecordsetPtr & rcdSet);
	float GetfloatData(_bstr_t varName,_RecordsetPtr & rcdSet);
	_RecordsetPtr Query(CString query);
	void NextRecord(_RecordsetPtr & rcdSet);
	int GetCount(_RecordsetPtr & rcdSet);
	void Log(CString msg);
	void Close(_RecordsetPtr & rcdSet);
	void ExecQuery(CString query);

};
