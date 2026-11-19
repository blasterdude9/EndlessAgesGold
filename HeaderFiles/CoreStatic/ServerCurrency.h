#include "dbconnectionsystem.h"
#include "CAxList.h"
#include "CInventoryClass.h"
#include "ItemManager.h"
#pragma once

class CServerCurrency : public CAxLinkObj
{
	public:
	CServerCurrency(void);
	~CServerCurrency(void){};

	int			m_glid;//this ids the item that will be used for this currency type
	int			m_valueRatio;//this is a value that if set to 2 means that its 2to1 ratio 1=1to1 ratio etc good way of doing gold silver
};

class CServerCurrencySystem : public CAxList
{
	public:
		CServerCurrencySystem(){};
		~CServerCurrencySystem();

	BOOL InitFromSQL(CServerEventList *eventLog);
	int EvalCurrency(CInventoryObjList * inventoryToEval);
	int AddCurrency(CItemManager * itemManager,CInventoryObjList * inventory,int glidSpecific,int amount);
	int RemoveCurrencyIfEnoughExists(CItemManager * itemManager,CInventoryObjList * inventoryToEval,int glidSpecific,int amount);
	BOOL OptimizeCurrencyDenominations(CItemManager * itemManager,CInventoryObjList * inventoryToOptimize);
	BOOL OptimizeCurrencyDenominations(CItemManager * itemManager,CInventoryObjList * inventoryToOptimize, int totalAssetsToDist);

};
