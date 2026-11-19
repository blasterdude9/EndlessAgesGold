#include "StdAfx.h"

#include <afxtempl.h>
#include "dbconnectionsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool ConnectionStructureClass::AquireFromCfgFile(CString path)
{
	for(int attempt = 0;attempt<10;attempt++)
	{
     CFileException exc;
     CFile fl;
	 CArchive the_Archive(&fl,CArchive::load);
     BOOL res = fl.Open(path,CFile::modeRead, &exc);
	 if(res)
	 {
        the_Archive.ReadString(m_sqlServerIP);
		the_Archive.ReadString(m_sqlServerUsername);
		the_Archive.ReadString(m_sqlServerPassword);
		the_Archive.ReadString(m_sqlServerCatalog);

        the_Archive.Close();
	    fl.Close();
	   return true;
	 }
	 Sleep(223);
	}
	return false;
}



CDBConnectionSystem::CDBConnectionSystem(CString sqlSvrIP,
										 CString catalog,
										 CString slqUsername,
										 CString sqlServerPassword,
										 CServerEventList * m_eventLog,
										 int timeout)
{

	InitializeCriticalSection(  &m_cSection  );
	m_dbReconnectDuration           = 600000;
	m_dbReconnectTimeStamp          = GetTickCount();
	m_eventLogRef                   = m_eventLog;
	m_connectionEstablished			= FALSE;
	m_pConnection					= NULL;
	m_commandBuild					= "Provider=sqloledb;Data Source=";
	m_commandBuild					= m_commandBuild+sqlSvrIP;
	m_commandBuild					= m_commandBuild+";Initial Catalog=";
	m_commandBuild					= m_commandBuild+catalog;
	m_commandBuild					= m_commandBuild+";User Id=";
    m_commandBuild					= m_commandBuild+slqUsername;
	m_commandBuild					= m_commandBuild+";Password=";
	m_commandBuild					= m_commandBuild+sqlServerPassword;
	m_commandBuild					= m_commandBuild+";";

	EstablishConnection(timeout);  

}

void CDBConnectionSystem::EstablishConnection(int timeout)
{
	CString currentError;
	EnterCriticalSection(&m_cSection);
    _bstr_t strCnn(ConvertStringToBSTR(m_commandBuild.GetBuffer()));
    
    try
	{
		m_pConnection.CreateInstance(__uuidof(Connection));
		m_pConnection->Open (strCnn, "", "", adConnectUnspecified);
		HRESULT err = m_pConnection->put_CommandTimeout(timeout);
		currentError = "ADO Connection opened ok";
		if(err == S_OK)
           currentError = " Timeout Set OK ";
		m_connectionEstablished = TRUE;
	}
	catch(_com_error &e)
    {
        _bstr_t bstrSource(e.Source());
        _bstr_t bstrDescription(e.Description());
		currentError = ConvertBSTRToString(bstrDescription);	
	}
	LeaveCriticalSection(&m_cSection);


	Log(currentError);
}

CDBConnectionSystem::~CDBConnectionSystem(void)
{
	CString currentError;
	try
	{
		m_pConnection->Close();
		currentError = "ADO Connection Closed ok";
	}
	catch(_com_error &e)
    {
        _bstr_t bstrSource(e.Source());
        _bstr_t bstrDescription(e.Description());
		currentError = ConvertBSTRToString(bstrDescription);	
	}

	Log(currentError);

	DeleteCriticalSection(  &m_cSection );
}

void CDBConnectionSystem::Reconnect(DWORD & currentTickCount)
{
    if((currentTickCount-m_dbReconnectTimeStamp) < m_dbReconnectDuration)
	    return;

	EnterCriticalSection(&m_cSection);

	m_dbReconnectTimeStamp = currentTickCount;
    CString currentError;
	_bstr_t strCnn(ConvertStringToBSTR(m_commandBuild.GetBuffer()));
	try
	{
		m_pConnection->Close();
		currentError = "Close before Reconnect";
	}
	catch(_com_error &e)
    {
        _bstr_t bstrSource(e.Source());
        _bstr_t bstrDescription(e.Description());
		currentError = ConvertBSTRToString(bstrDescription);
	}

	try
	{
		m_pConnection->Open (strCnn, "", "", adConnectUnspecified);
		currentError += "Reconnected";
	}
	catch(_com_error &e)
    {
        _bstr_t bstrSource(e.Source());
        _bstr_t bstrDescription(e.Description());
		currentError = ConvertBSTRToString(bstrDescription);
	}

	LeaveCriticalSection(&m_cSection);

	Log(currentError+" Reconnect Error");
}

_RecordsetPtr CDBConnectionSystem::Query(CString query)
{
	CString currentError;
	_bstr_t rqStringBSTR = ConvertStringToBSTR(query.GetBuffer());
    _RecordsetPtr innerQuerry = NULL;
    innerQuerry.CreateInstance(__uuidof(Recordset));
    
	EnterCriticalSection(&m_cSection);
	try
	{
	        innerQuerry->Open (rqStringBSTR,_variant_t((IDispatch *) m_pConnection, true),adOpenStatic, adLockReadOnly, adCmdText);
	}
	catch(_com_error &e)
	{
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			currentError = ConvertBSTRToString(bstrDescription);
	}
	LeaveCriticalSection(&m_cSection);

	if(currentError.GetLength() > 0)
	{
	   Log(currentError);//two lines incase its too long to log
	   Log("***Querry:"+query);
	   return false;
	}
	
	return innerQuerry;
}

bool CDBConnectionSystem::InlineReconnect(CString & currentError)
{
    _bstr_t strCnn(ConvertStringToBSTR(m_commandBuild.GetBuffer()));
	try{m_pConnection->Close();}
	catch(_com_error &e)
	{
	    _bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());
	}
	
    try
	{
		m_pConnection->Open (strCnn, "", "", adConnectUnspecified);
		currentError = "Reconnected";
	}
	catch(_com_error &e)
	{
		_bstr_t bstrSource(e.Source());
		_bstr_t bstrDescription(e.Description());
		currentError = ConvertBSTRToString(bstrDescription);	

		return false;
	}
	return true;
}

bool CDBConnectionSystem::ExecQuery(CString query)
{
	_bstr_t rqStringBSTR = ConvertStringToBSTR(query.GetBuffer());
	CString currentError;
    _RecordsetPtr innerQuerry = NULL;
    innerQuerry.CreateInstance(__uuidof(Recordset));
    EnterCriticalSection(&m_cSection);
	try
	{
	        innerQuerry->Open (rqStringBSTR,_variant_t((IDispatch *) m_pConnection, true),adOpenStatic, adLockOptimistic, adCmdText);
	}
	catch(_com_error &e)
	{
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			currentError = ConvertBSTRToString(bstrDescription);
	}
    LeaveCriticalSection(&m_cSection);
 
	if(currentError.GetLength() > 0)
	{
	   Log(currentError);//two lines incase its too long to log
	   Log("***Querry:"+query);
	   return false;
	}

	return true;
}

CString CDBConnectionSystem::GetStringData(CString varName,_RecordsetPtr & rcdSet)
{
    EnterCriticalSection(&m_cSection);
	try
	{
	 _variant_t vt = rcdSet->Fields->GetItem(varName.GetBuffer())->Value;
	 
	 CString tempCnv =  (vt.vt == VT_NULL) ? "" : _bstr_t( vt );

	 LeaveCriticalSection(&m_cSection);
     return tempCnv;
	}
	catch(_com_error &e)
	{
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			CString currentError = ConvertBSTRToString(bstrDescription);	
			LeaveCriticalSection(&m_cSection);
			Log(currentError+" GetStringData() VarName:"+varName);
			return "";
	}
	LeaveCriticalSection(&m_cSection);
	return "";
}

int CDBConnectionSystem::GetIntData(CString varName,_RecordsetPtr & rcdSet)
{
   varName = GetStringData(varName,rcdSet);
   if(varName.GetLength()>0)
      return atoi(varName);
   
   return 0;
}

bool CDBConnectionSystem::GetBoolData(CString varName,_RecordsetPtr & rcdSet)
{
   varName = GetStringData(varName,rcdSet);
   if(atoi(varName)==0)
      return false;
   
   return true;
}

float CDBConnectionSystem::GetfloatData(CString varName,_RecordsetPtr & rcdSet)
{
   varName = GetStringData(varName,rcdSet);
   if(varName.GetLength()>0)
      return (float)atof(varName);
   
   return 0.0f;
}

void CDBConnectionSystem::NextRecord(_RecordsetPtr & rcdSet)
{
	EnterCriticalSection(&m_cSection);
	CString currentError;
    try
	{
			rcdSet->MoveNext();
	}
	catch(_com_error &e)
	{
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			currentError = ConvertBSTRToString(bstrDescription);
			LeaveCriticalSection(&m_cSection);
			Log(currentError + " NextRecord()");
			return;
	}
	LeaveCriticalSection(&m_cSection);
}

int CDBConnectionSystem::GetCount(_RecordsetPtr & rcdSet)
{
	EnterCriticalSection(&m_cSection);
	CString currentError;
	int count = 0;
    try
	{       count = rcdSet->GetRecordCount();
	        LeaveCriticalSection(&m_cSection);
			return count;
	}
	catch(_com_error &e)
	{
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			currentError = ConvertBSTRToString(bstrDescription);
			LeaveCriticalSection(&m_cSection);
			Log(currentError+" GetCount()");
			return 0;
	}
	LeaveCriticalSection(&m_cSection);
	return 0;
}

void CDBConnectionSystem::Close(_RecordsetPtr & rcdSet)
{
	CString currentError;
	EnterCriticalSection(&m_cSection);
    try
	{
			rcdSet->Close();
	}
	catch(_com_error &e)
	{
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			currentError = ConvertBSTRToString(bstrDescription);
			LeaveCriticalSection(&m_cSection);
			Log(currentError + " Close()");
			return;
	}
	LeaveCriticalSection(&m_cSection);
}

//------------------------------------------------------------------------------------//
CString CDBConnectionSystem::SanitizeFormatOut(CString text)
{
    char quoteX[2];
    quoteX[0] = '"';
    quoteX[1] = 0;
	text.Replace(quoteX,"'");

    return text;
}

//------------------------------------------------------------------------------------//
CString CDBConnectionSystem::SanitizeFormatIn(CString text)
{
    char quoteX[2];
    quoteX[0] = '"';
    quoteX[1] = 0;
	text.Replace("'",quoteX);

    return text;
}

//------------------------------------------------------------------------------------//
/*CString CDBConnectionSystem::SanitizeFormatInENC(CString text)
{
    char quoteX[2];
    quoteX[0] = '"';
    quoteX[1] = 0;
	text.Replace("'","[hyphen]");

    return text;
}*/
//------------------------------------------------------------------------------------//
CString CDBConnectionSystem::SanitizeFormatInENC(CString text)
{
	text.Replace("'","[hyphen]");

    return text;
}
//------------------------------------------------------------------------------------//
CString CDBConnectionSystem::SanitizeFormatOutENC(CString text)
{
	text.Replace("[hyphen]","'");
    return text;
}
//------------------------------------------------------------------------------------//
void CDBConnectionSystem::Log(CString msg)
{
    if(m_eventLogRef)
       m_eventLogRef->AddEvent("ADO:"+msg);
}
//------------------------------------------------------------------------------------//
inline BSTR CDBConnectionSystem::ConvertStringToBSTR(const char* pSrc)
{
    if(!pSrc) return NULL;

    DWORD cwch;

    BSTR wsOut(NULL);

    if(cwch = ::MultiByteToWideChar(CP_ACP, 0, pSrc, 
         -1, NULL, 0))//get size minus NULL terminator
    {
                cwch--;
            wsOut = ::SysAllocStringLen(NULL, cwch);

        if(wsOut)
        {
            if(!::MultiByteToWideChar(CP_ACP, 
                     0, pSrc, -1, wsOut, cwch))
            {
                if(ERROR_INSUFFICIENT_BUFFER == ::GetLastError())
                    return wsOut;
                ::SysFreeString(wsOut);//must clean up
                wsOut = NULL;
            }
        }

    };

    return wsOut;
};
//------------------------------------------------------------------------------------//
inline char* CDBConnectionSystem::ConvertBSTRToString(BSTR pSrc)
{
    if(!pSrc) return NULL;

    //convert even embeded NULL
    DWORD cb,cwch = ::SysStringLen(pSrc);

    char *szOut = NULL;

    if(cb = ::WideCharToMultiByte(CP_ACP, 0, 
               pSrc, cwch + 1, NULL, 0, 0, 0))
    {
        szOut = new char[cb];
        if(szOut)
        {
            szOut[cb - 1]  = '\0';

            if(!::WideCharToMultiByte(CP_ACP, 0, 
                pSrc, cwch + 1, szOut, cb, 0, 0))
            {
                delete []szOut;//clean up if failed;
                szOut = NULL;
            }
        }
    }

    return szOut;
};


