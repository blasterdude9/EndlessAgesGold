#include "StdAfx.h"
#include "ServerCurrency.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CServerCurrency::CServerCurrency(void)
{
	m_glid			= 0;
	m_valueRatio	= 0;
	this->m_thisObject = this;
}

CServerCurrencySystem::~CServerCurrencySystem()
{
	RemoveAll();
}

BOOL CServerCurrencySystem::InitFromSQL(CServerEventList *eventLog)
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
	       eventLog->AddEvent("CServerCurrencySystem::InitFromSQL() Aborted due to connection not being established");
        return FALSE;//this would be a sever error the srever should halt
	}

    CString qry = "SELECT GLID,VALUERATIO FROM GM_CURRENCY ORDER BY VALUERATIO DESC";
	_RecordsetPtr cReader = dbConnection->Query(qry);
	int cCount = dbConnection->GetCount(cReader);
	if(eventLog)
	   eventLog->AddEvent("CServerCurrencySystem::InitFromSQL currencies loaded:",(DWORD)cCount);
    for(unsigned int loop=0;loop<cCount;loop++)
	{
	    CServerCurrency * newC = new CServerCurrency();
		
		newC->m_glid								= dbConnection->GetIntData("GLID",cReader);
        newC->m_valueRatio							= dbConnection->GetIntData("VALUERATIO",cReader);
		newC->m_uniqueID							= newC->m_glid;

		if(newC->m_valueRatio < 1)
		   newC->m_valueRatio = 1;

		AddTail(newC);

		dbConnection->NextRecord(cReader);
	}
	dbConnection->Close(cReader);

	delete dbConnection;
	dbConnection = 0;

	return TRUE;
}

int CServerCurrencySystem::EvalCurrency(CInventoryObjList * inventoryToEval)
{
	int totalAssets = 0;
	if (!inventoryToEval) 
		return totalAssets;
	for(POSITION posLoc = inventoryToEval->GetHeadPosition();posLoc != NULL;)
	{
		CInventoryObj * iPtr = (CInventoryObj *)inventoryToEval->GetNext(posLoc);
		CServerCurrency * cPtr = (CServerCurrency *)GetByUniqueID(iPtr->m_itemData->m_globalID);
        if(cPtr)
		{//this is a currency object
           totalAssets += iPtr->m_quanity * cPtr->m_valueRatio;
		}//end this is a currency object
	}

	return totalAssets;
}

int CServerCurrencySystem::AddCurrency(CItemManager * itemManager,
									   CInventoryObjList * inventory,
									   int glidSpecific,
									   int amount)
{
    int totalAssets = EvalCurrency(inventory);

    CServerCurrency * cPtr = NULL;
	if(glidSpecific < 0)
	{
        cPtr = (CServerCurrency *)FindIndex(GetCount()-1);//get lowest denominator
	}
	else
		cPtr = (CServerCurrency *)GetByUniqueID(glidSpecific);

	if(!cPtr)
        return 2;//glid for currency not found

	int totalChange = cPtr->m_valueRatio * amount;

	//simply take total assets minus cost then redistribute denominations 
    int remainingAssets = totalAssets + totalChange;
	if(remainingAssets < 0)
	   remainingAssets = 0;

	return OptimizeCurrencyDenominations(itemManager,inventory,remainingAssets);
}
	
int CServerCurrencySystem::RemoveCurrencyIfEnoughExists(CItemManager * itemManager,
														CInventoryObjList * inventoryToEval,
														int glidSpecific,
														int amount)
{
    int totalAssets = EvalCurrency(inventoryToEval);

    CServerCurrency * cPtr = NULL;
	if(glidSpecific < 0)
	{
        cPtr = (CServerCurrency *)FindIndex(GetCount()-1);//get lowest denominator
	}
	else
		cPtr = (CServerCurrency *)GetByUniqueID(glidSpecific);

	if(!cPtr)
        return 2;//glid for currency not found

	int totalCost = cPtr->m_valueRatio * amount;

	if(totalAssets >= totalCost)
	{//can afford this
		//simply take total assets minus cost then redistribute denominations 
        int remainingAssets = totalAssets - totalCost;

		return OptimizeCurrencyDenominations(itemManager,inventoryToEval,remainingAssets);
	}//end can afford this

	return 0;//cant afford
}
//-------------------------------------------------------
//This can change lower denominations to more valuable but lesser quantity of items
//-------------------------------------------------------
BOOL CServerCurrencySystem::OptimizeCurrencyDenominations(CItemManager * itemManager,
														  CInventoryObjList * inventoryToOptimize, 
														  int totalAssetsToDist)
{
    for(CAxLinkObj * pos = GetHeadPosition_unsafe();pos != NULL;)
	{//ordered from greatest to least
        CServerCurrency * pData = (CServerCurrency *)GetNext_unsafe(&pos);
		
        
		int totalNeeded = totalAssetsToDist/pData->m_valueRatio;
		if(totalNeeded == 0)
		{//if present remove from inventory and continue to next denomination
		   inventoryToOptimize->DeleteByGLIDAll(pData->m_glid);
		   continue;
		}

		if(totalAssetsToDist == 0)
		   continue;

		totalAssetsToDist = totalAssetsToDist-totalNeeded*pData->m_valueRatio;//this will leave a remainder is there is one to distribute in smaller denominations
		//update or add glid credit
		CInventoryObj * invPtr = inventoryToOptimize->GetByGLID(pData->m_glid);
		if(invPtr)
		   invPtr->m_quanity = totalNeeded;
		else
		{//add currency glid
           CItemObj * invObj = itemManager->GetItemByID(pData->m_glid); 
		   if(invObj)
              inventoryToOptimize->AddInventoryItem(totalNeeded,FALSE,invObj);
		   else
		      return FALSE;
		}//end add currency glid
	}


	return TRUE;
}
//-------------------------------------------------------
BOOL CServerCurrencySystem::OptimizeCurrencyDenominations(CItemManager * itemManager,CInventoryObjList * inventoryToOptimize)
{
    int totalAssets = EvalCurrency(inventoryToOptimize);
	if(totalAssets == 0)
	   return TRUE;

	return OptimizeCurrencyDenominations(itemManager,inventoryToOptimize,totalAssets);
}