#include "StdAfx.h"

#include <afxtempl.h>
#include "dbconlimitedsystem.h"


CDBConSystem::CDBConSystem(CString sqlSvrIP,
						   CString catalog,
					       CString slqUsername,
					       CString sqlServerPassword)
{

	m_dbReconnectDuration           = 600000;
	m_dbReconnectTimeStamp          = GetTickCount();
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

	EstablishConnection();  
}

void CDBConSystem::EstablishConnection()
{
    _bstr_t strCnn(m_commandBuild);
    try
	{
		m_pConnection.CreateInstance(__uuidof(Connection));
		m_pConnection->Open (strCnn, "", "", adConnectUnspecified);
		m_currentError = "ADO Connection opened ok";
		Log(m_currentError);
	}
	catch(_com_error &e)
    {
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			m_currentError = (LPSTR)bstrDescription;
			Log(m_currentError);
	}
}

CDBConSystem::~CDBConSystem(void)
{
	try
	{
		m_pConnection->Close();
		m_currentError = "ADO Connection Closed ok";
		Log(m_currentError);
	}
	catch(_com_error &e)
    {
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			m_currentError = (LPSTR)bstrDescription;
			Log(m_currentError);
	}
}

void CDBConSystem::Reconnect(DWORD & currentTickCount)
{
    if((currentTickCount-m_dbReconnectTimeStamp) < m_dbReconnectDuration)
	    return;

	m_dbReconnectTimeStamp = currentTickCount;

	_bstr_t strCnn(m_commandBuild);
	try
	{
		m_pConnection->Close();
		m_currentError = "Close before Reconnect";
		Log(m_currentError);
	}
	catch(_com_error &e)
    {
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			m_currentError = (LPSTR)bstrDescription;
			Log(m_currentError+"Reconnect Close Error");
	}

	try
	{
		m_pConnection->Open (strCnn, "", "", adConnectUnspecified);
		m_currentError = "Reconnected";
		Log(m_currentError);
	}
	catch(_com_error &e)
    {
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			m_currentError = (LPSTR)bstrDescription;
			Log(m_currentError+"Reconnect Open Error");
	}
}

_RecordsetPtr CDBConSystem::Query(CString query)
{
	_bstr_t rqStringBSTR = query;
    _RecordsetPtr innerQuerry;
    innerQuerry.CreateInstance(__uuidof(Recordset));
    
	try
	{
	        innerQuerry->Open (rqStringBSTR,_variant_t((IDispatch *) m_pConnection, true),adOpenStatic, adLockReadOnly, adCmdText);
	}
	catch(_com_error &e)
	{
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			m_currentError = (LPSTR)bstrDescription;
			Log(m_currentError + " query:"+ query);
			return NULL;
	}
	
	return innerQuerry;
}

void CDBConSystem::ExecQuery(CString query)
{
	_bstr_t rqStringBSTR = query;
    _RecordsetPtr innerQuerry;
    innerQuerry.CreateInstance(__uuidof(Recordset));
    
	try
	{
	        innerQuerry->Open (rqStringBSTR,_variant_t((IDispatch *) m_pConnection, true),adOpenStatic, adLockOptimistic, adCmdText);
			//innerQuerry->Close();
	}
	catch(_com_error &e)
	{
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			m_currentError = (LPSTR)bstrDescription;
			Log(m_currentError + " query:"+ query);
			return;
	}
}

CString CDBConSystem::GetStringData(_bstr_t varName,_RecordsetPtr & rcdSet)
{
	try
	{
     _bstr_t bufferSql = rcdSet->Fields->GetItem(varName)->Value;
     return (LPSTR)bufferSql;
	}
	catch(_com_error &e)
	{
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			m_currentError = (LPSTR)bstrDescription;
			Log(m_currentError);
	}
	return "";
}

int CDBConSystem::GetIntData(_bstr_t varName,_RecordsetPtr & rcdSet)
{
   CString temp = GetStringData(varName,rcdSet);
   if(temp.GetLength()>0)
      return atoi(temp);
   
   return 0;
}

float CDBConSystem::GetfloatData(_bstr_t varName,_RecordsetPtr & rcdSet)
{
   CString temp = GetStringData(varName,rcdSet);
   if(temp.GetLength()>0)
      return atof(temp);
   
   return 0.0f;
}

void CDBConSystem::NextRecord(_RecordsetPtr & rcdSet)
{
    try
	{
			rcdSet->MoveNext();
	}
	catch(_com_error &e)
	{
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			m_currentError = (LPSTR)bstrDescription;
			Log(m_currentError);
	}
}

int CDBConSystem::GetCount(_RecordsetPtr & rcdSet)
{
    try
	{
			return rcdSet->GetRecordCount();
	}
	catch(_com_error &e)
	{
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			m_currentError = (LPSTR)bstrDescription;
			Log(m_currentError);
	}
	return 0;
}

void CDBConSystem::Close(_RecordsetPtr & rcdSet)
{
    try
	{
			rcdSet->Close();
	}
	catch(_com_error &e)
	{
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
			m_currentError = (LPSTR)bstrDescription;
			Log(m_currentError);
	}
}

void CDBConSystem::Log(CString msg)
{

	CFileException exc;
    CFile fl;
	CArchive the_out_Archive(&fl,CArchive::store);
    BOOL res = fl.Open("log.txt",CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite, &exc);
	if(res)
	{
	fl.SeekToEnd( );
	the_out_Archive.WriteString(msg);  
    the_out_Archive.WriteString( "\n");   
    the_out_Archive.Close();
    fl.Close(); 
	}
   
}



