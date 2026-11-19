#include "stdafx.h"
#include <d3d9.h>
#include <afxtempl.h>
#include "CWorldChannelRules.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWordlChannelRuleObj::CWordlChannelRuleObj()
{
	m_rule				= 0;
	m_param1			= 0;
	m_primaryKey		= 0;
	m_wldID				= 0;
}
//-------------------------------------------------------------//
BOOL CWorldChannelRuleList::DoesThisRuleExistInThisChannel_safe(int & stencilChannel,int & dynamicChannel, int searchRule, DWORD * param1)
{
    CAxList * obj = GetAllOnIndex(stencilChannel,"wldID");
	Lock();
    for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
	{
         CWordlChannelRuleObj * plObj = (CWordlChannelRuleObj *)obj->GetNext_unsafe(&pos);
         if(plObj->m_wldID == stencilChannel)//verify for hash overlap
		 if(plObj->m_rule==searchRule)
		 {
            if(param1)
			   *param1 = plObj->m_param1;

		    Unlock();
		    return TRUE;
		 }
	}
	obj = GetAllOnIndex(dynamicChannel,"wldID");
    for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
	{
         CWordlChannelRuleObj * plObj = (CWordlChannelRuleObj *)obj->GetNext_unsafe(&pos);
         if(plObj->m_wldID == dynamicChannel)//verify for hash overlap
		 if(plObj->m_rule==searchRule)
		 {
            if(param1)
			   *param1 = plObj->m_param1;

		    Unlock();
		    return TRUE;
		 }
	}
	Unlock();
	return FALSE;
}

//-------------------------------------------------------------//
BOOL CWorldChannelRuleList::DoesThisRuleExistInThisChannel_safe(int & stencilChannel,int & dynamicChannel, int searchRule)
{
	return DoesThisRuleExistInThisChannel_safe(stencilChannel,dynamicChannel, searchRule, NULL);
}
//-------------------------------------------------------------//
BOOL CWorldChannelRuleList::GetFromSQL(CServerEventList *eventLog)
{
	ConnectionStructureClass connectionStructTemp;
    connectionStructTemp.AquireFromCfgFile("baseAccount.cfg");
    CDBConnectionSystem * dbConnection = new CDBConnectionSystem(connectionStructTemp.m_sqlServerIP,
			                                                     connectionStructTemp.m_sqlServerCatalog,
																 connectionStructTemp.m_sqlServerUsername,
																 connectionStructTemp.m_sqlServerPassword,
																 eventLog);


	if(!dbConnection->m_connectionEstablished)
	{
		if(eventLog)
	       eventLog->AddEvent("CWorldChannelRuleList::GetFromSQL Aborted due to connection not being established");
        return FALSE;
	}

    //set up GET querys
    CString getWorldRules				= "SELECT * FROM SYSWORLDRULES";
	
    //aquire from SQL server
	_RecordsetPtr wldReader = dbConnection->Query(getWorldRules);
	int worldRuleCount = dbConnection->GetCount(wldReader);
	if(eventLog)
	   eventLog->AddEvent("CWorldChannelRuleList::GetFromSQL World Rules to load:",(DWORD)worldRuleCount);
    for(int loop=0;loop<worldRuleCount;loop++)
	{
	    CWordlChannelRuleObj * newRule = new CWordlChannelRuleObj();

		newRule->m_primaryKey						= dbConnection->GetIntData("PKEY",wldReader);
        newRule->m_rule								= dbConnection->GetIntData("RULEID",wldReader);
		newRule->m_ruleName							= dbConnection->GetStringData("RULENAME",wldReader);
		newRule->m_wldID							= dbConnection->GetIntData("WLDID",wldReader);
		newRule->m_param1							= dbConnection->GetIntData("PARAM1",wldReader);

		Add_safe(newRule);
		dbConnection->NextRecord(wldReader);
	}
	dbConnection->Close(wldReader);
	if(eventLog)
       eventLog->AddEvent("CWorldChannelRuleList::GetFromSQL complete..");

	delete dbConnection;
	dbConnection = 0;

	return true;
}

