#include "StdAfx.h"
#include "pendinglogonslist.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CPendingLogonObj::CPendingLogonObj()
{
    m_timeAdded = GetTickCount();
	m_dpnid = 0;
    m_processing = FALSE;
}

CPendingLogonObj::~CPendingLogonObj()
{

}

CPendingLogonsList::CPendingLogonsList(void)
{
    InitializeCriticalSection(&m_cSection);
	m_timeoutDuration		= 120000;
	m_maxSimuIPs			= 5;
	m_connectionStructTemp.AquireFromCfgFile("authAccount.cfg");
	//m_dbConnection = NULL;
	m_authenticateUserPass		  = "SELECT PASSWORD,muteDeaf,InGameName,MinutesPlayed,LOGGEDON,ID, DATEDIFF([day], GETDATE(), recurringDate) AS paidAc FROM GMACCOUNTBASE WHERE USERNAME = '$USERNAME'";
    m_ipUsernameBannedCheck		  = "SELECT USERNAME FROM GMBANNEDINFO WHERE USERNAME = '$USER' OR IP = '$IP'";
    m_checkLoggedOnDBStatusChange = "UPDATE GMACCOUNTBASE SET LOGGEDON = '$LOGGEDON', LASTLOGGEDONDATE = GETDATE() WHERE USERNAME = '$USER'";
	m_permissions                 = "SELECT * FROM TBLUSERPERMS WHERE USERNAME = '$USER'";
	m_getCharacterCount			  = "SELECT * FROM GMCHARACTERS WHERE USERNAME = '$USER' AND INUSE = '1' AND GAMENAME = '$GAMNAME'";
	m_getHouseBind			      = "SELECT * FROM GM_SERVERPLACEMENTS WHERE ACCOUNTBIND = '$ACCOUNTNUMBER'";
	m_isPaidAccount               = "SELECT * FROM GMACCOUNTBASE WHERE USERNAME = '$USERNAME' AND RECURRINGDATE > GETDATE()";
	m_encryptionManager			  = new CAXEncryptionLib("lucidcatscanarbcowlordarronnicol");
}

CPendingLogonsList::CPendingLogonsList(CServerEventList * eventLog)
{
    InitializeCriticalSection(&m_cSection);
	m_timeoutDuration		= 120000;
	m_maxSimuIPs			= 5;
	m_connectionStructTemp.AquireFromCfgFile("authAccount.cfg");

    /*m_dbConnection                     = new CDBConnectionSystem(m_connectionStructTemp.m_sqlServerIP,
			                                                     m_connectionStructTemp.m_sqlServerCatalog,
																 m_connectionStructTemp.m_sqlServerUsername,
																 m_connectionStructTemp.m_sqlServerPassword,
																 eventLog,
																 1);
    if(m_dbConnection->m_connectionEstablished == TRUE)
	{//verify connection established
		eventLog->AddEvent("Pending logon sys connected");
	}
	else
	{
		delete m_dbConnection;
		m_dbConnection=0;
		eventLog->AddEvent("Pending logon sys Failed to connect");
	}*/

	m_authenticateUserPass		  = "SELECT PASSWORD,muteDeaf,InGameName,MinutesPlayed,LOGGEDON,ID, DATEDIFF([day], GETDATE(), recurringDate) AS paidAc FROM GMACCOUNTBASE WHERE USERNAME = '$USERNAME'";
    m_ipUsernameBannedCheck		  = "SELECT USERNAME FROM GMBANNEDINFO WHERE USERNAME = '$USER' OR IP = '$IP'";
    m_checkLoggedOnDBStatusChange = "UPDATE GMACCOUNTBASE SET LOGGEDON = '$LOGGEDON', LASTLOGGEDONDATE = GETDATE() WHERE USERNAME = '$USER'";
	m_permissions                 = "SELECT * FROM TBLUSERPERMS WHERE USERNAME = '$USER'";
	m_getCharacterCount			  = "SELECT * FROM GMCHARACTERS WHERE USERNAME = '$USER' AND INUSE = '1' AND GAMENAME = '$GAMNAME'";
	m_getHouseBind			      = "SELECT * FROM GM_SERVERPLACEMENTS WHERE ACCOUNTBIND = '$ACCOUNTNUMBER'";
	m_isPaidAccount               = "SELECT * FROM GMACCOUNTBASE WHERE USERNAME = '$USERNAME' AND RECURRINGDATE > GETDATE()";
	m_encryptionManager			  = new CAXEncryptionLib("lucidcatscanarbcowlordarronnicol");

}

CPendingLogonsList::~CPendingLogonsList(void)
{
	/*if(m_dbConnection)
	{
       delete m_dbConnection;
	   m_dbConnection = 0;
	}*/

	if(m_encryptionManager)
	{ 
       delete m_encryptionManager;
	   m_encryptionManager = 0;
	}

    DeleteCriticalSection(&m_cSection);
}
//------------------------------------------------------------------//
//-------------first step to begine authentication------------------//
//------------------------------------------------------------------//
int CPendingLogonsList::Logon(CString username,CString password,CString ip)
{
    DWORD currentIPCount = 0;
	int usernameCount = 0;
	DWORD currentTime = GetTickCount();
	POSITION posLast = NULL;

	EnterCriticalSection(&m_cSection);
    for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	{
        CPendingLogonObj * pObj = (CPendingLogonObj *) GetNext(posLoc);
			
		if(pObj->m_username.CompareNoCase(username)==0)
		{
           usernameCount++;
		}

		if(pObj->m_ip.CompareNoCase(ip)==0)
		{
           currentIPCount++;
		}
	}

    if(currentIPCount > m_maxSimuIPs)
	{
	   LeaveCriticalSection(&m_cSection);
	   return 3;
	}

	if(usernameCount > 0)
	{
	   LeaveCriticalSection(&m_cSection);
	   return 2;
	}

	CPendingLogonObj * addObj = new CPendingLogonObj();
	addObj->m_ip			= ip;
	addObj->m_username		= username;
	addObj->m_password      = password;
	addObj->m_logOn         = TRUE;
	
    AddTail(addObj);

	LeaveCriticalSection(&m_cSection);

	return 1;
}

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
int CPendingLogonsList::LogonComplete(CString username)
{
	POSITION posLast = NULL;
    EnterCriticalSection(&m_cSection);
    for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	{
        CPendingLogonObj * pObj = (CPendingLogonObj *) GetNext(posLoc);
	    if(pObj->m_username.CompareNoCase(username)==0)
		{
           delete pObj;
		   pObj = 0;
           RemoveAt(posLast);
		   LeaveCriticalSection(&m_cSection);
		   return 1;
		}
	}
	LeaveCriticalSection(&m_cSection);
    return 0;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CPendingLogonsList::GetDataByUsername(CString & username,DPNID * dpnid, CString * ipStr)
{
	POSITION posLast = NULL;
    EnterCriticalSection(&m_cSection);
    for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	{
        CPendingLogonObj * pObj = (CPendingLogonObj *) GetNext(posLoc);
	    if(pObj->m_username.CompareNoCase(username)==0)
		{
		   *ipStr = pObj->m_ip;
		   *dpnid = pObj->m_dpnid;
		   LeaveCriticalSection(&m_cSection);
		   return TRUE;
		}
	}
	LeaveCriticalSection(&m_cSection);
    return FALSE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
int CPendingLogonsList::GetCountSafe()
{
    EnterCriticalSection(&m_cSection);
    int count = (int)GetCount();
    LeaveCriticalSection(&m_cSection);
    return count;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CPendingLogonsList::GetNextLogon(CPendingLogonObj ** pObjRet)
{
	 EnterCriticalSection(&m_cSection);
     POSITION posLoc = GetHeadPosition();
	 if(posLoc == NULL)
	 {
	    LeaveCriticalSection(&m_cSection);
	    return FALSE;
	 }

	 CPendingLogonObj * pObj = (CPendingLogonObj *) GetAt(posLoc);
	 *pObjRet = pObj;
	 RemoveAt(posLoc);
	 LeaveCriticalSection(&m_cSection);
     return TRUE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CPendingLogonsList::SetDPNID(DPNID dpnid, CString username)
{
	EnterCriticalSection(&m_cSection);
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CPendingLogonObj * pObj = (CPendingLogonObj *) GetNext(posLoc);
		if(pObj->m_username.CompareNoCase(username)==0)
		{ 
           pObj->m_dpnid = dpnid;
		   LeaveCriticalSection(&m_cSection);
		return TRUE;
		}
	}
	LeaveCriticalSection(&m_cSection);
	return FALSE;
}
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
/*BOOL CPendingLogonsList::ExportContext(CONTEXT_VALIDATION ** newContextPointer, CString & username)
{
    EnterCriticalSection(&m_cSection);
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CPendingLogonObj * pObj = (CPendingLogonObj *) GetNext(posLoc);
		if(pObj->m_username.CompareNoCase(username)==0)
		{ 
		   *newContextPointer = pObj->m_context;
		   LeaveCriticalSection(&m_cSection);
		return TRUE;
		}
	}
	LeaveCriticalSection(&m_cSection);
	return FALSE;
}*/
//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
BOOL CPendingLogonsList::GetPtrThatsReadyForFinalize(CPendingLogonObj ** pObjRet,int * currentInQue)
{
    EnterCriticalSection(&m_cSection);
	*currentInQue = (int)GetCount();
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CPendingLogonObj * pObj = (CPendingLogonObj *) GetNext(posLoc);
		if(pObj->m_processing)
		   continue;

		if(pObj->m_dpnid == 0)
		   continue;

		pObj->m_processing = TRUE;
	    *pObjRet = pObj;
		LeaveCriticalSection(&m_cSection);
		return TRUE;
	}
	LeaveCriticalSection(&m_cSection);
	return FALSE;
}

//------------------------------------------------------------------//
BOOL CPendingLogonsList::GetPtrThatsReadyForFinalize(CString * ip,CString * password,CString * username,DPNID * dpnidLoc,int * currentInQue)
{
    EnterCriticalSection(&m_cSection);
	*currentInQue = (int)GetCount();
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CPendingLogonObj * pObj = (CPendingLogonObj *) GetNext(posLoc);
		if(pObj->m_processing)
		   continue;

		if(pObj->m_dpnid == 0)
		   continue;

		pObj->m_processing = TRUE;
	    *username = pObj->m_username;
		*password = pObj->m_password;
		*dpnidLoc = pObj->m_dpnid;
		*ip = pObj->m_ip;
		LeaveCriticalSection(&m_cSection);
		return TRUE;
	}
	LeaveCriticalSection(&m_cSection);
	return FALSE;
}
