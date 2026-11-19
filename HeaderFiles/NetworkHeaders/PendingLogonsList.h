#pragma once
#include "dbconnectionsystem.h"
#include "axencryptionlib.h"
#include "CStructuresMisl.h"
#include "dplay8.h"

class CPendingLogonObj : public CObject
{
public:
    CPendingLogonObj(void);
	~CPendingLogonObj(void);

	CString				m_ip;
	CString				m_username;
	CString             m_password;
	DWORD				m_timeAdded;
	BOOL                m_logOn;
	DPNID               m_dpnid;
    BOOL                m_processing;

};


class CPendingLogonsList : public CObList
{
public:
	CPendingLogonsList(void);
	~CPendingLogonsList(void);
	CPendingLogonsList(CServerEventList * eventLog);

    //vars
	CRITICAL_SECTION			m_cSection;
	DWORD						m_timeoutDuration;
	DWORD						m_maxSimuIPs;
	//CDBConnectionSystem *		m_dbConnection;
	ConnectionStructureClass    m_connectionStructTemp;
	CString						m_authenticateUserPass;
    CString						m_ipUsernameBannedCheck;
    CString						m_checkLoggedOnDBStatusChange;
	CString						m_permissions;
	CString						m_getCharacterCount;
	CString						m_getHouseBind;
	CString						m_isPaidAccount;
	CAXEncryptionLib           *m_encryptionManager;
	


	//functions
	BOOL SetDPNID(DPNID dpnid, CString username);
	BOOL GetPtrThatsReadyForFinalize(CString * ip,CString * password,CString * username,DPNID * dpnidLoc,int * currentInQue);
	BOOL GetPtrThatsReadyForFinalize(CPendingLogonObj ** pObjRet,int * currentInQue);
	int Logon(CString username,CString password,CString ip);//will timout other pending logons if necessary
	int LogonComplete(CString username);//removes username entries after authentication
	BOOL GetDataByUsername(CString & username,DPNID * dpnid, CString * ipStr);
	//BOOL ExportContext(CONTEXT_VALIDATION ** newContextPointer, CString & username);
    int GetCountSafe();
	BOOL GetNextLogon(CPendingLogonObj ** pObjRet);
	
};
