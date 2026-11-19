
//------------------------------------------------------//
//Interactive database driven world wide in game store
//
//------------------------------------------------------//
#pragma once
#include "Common.h"
#include "CAxList.h"
#include "DBConnectionSystem.h"

#define UC_MAXRETURNCOUNT			30			//Maximum return count
#define UC_MAXPOSTCOUNT				3			//Maximum item placement per character

enum UC_REQUESTTYPES
{
	UC_LISTRECENT = 1,					//return most recently posted items				example: /US_LISTRECENT [OFFSET]
	UC_LISTBYITEM,						//return by item type collection				example: /US_LISTBYITEM [ITEMNAME] [OFFSET]
	UC_SELLITEM,						//requests to place a item for sale				example: /US_SELLITEM [INTERNAL CHARNAME] [ITEMNAME] [QUANTITY] [PRICE]
	UC_BUYITEM,							//requests to buy a listed item					example: /US_BUYITEM [INTERNAL CHARNAME] [POSTEDBY] [ITEMNAME] [PRICE] [QUANTITY]
	UC_CASHOUT,							//returns money thats owed to you from sales	example: /UC_CASHOUT [CHARNAME]
	UC_SALESSTATS,						//returns the avg of the last 5 of this itemID  example: /UC_SALESSTATS [ITEMNAME]
	UC_MAX
};

//------------------------------------------------------//
//client use examples from in game, is case sensitive 
//in some cases
//------------------------------------------------------//
// /UC_LISTRECENT [OFFSET]
// /UC_LISTBYITEM [ITEMNAME] [OFFSET]
// /UC_SELLITEM [ITEMNAME] [QUANTITY] [PRICE]
// /UC_BUYITEM [POSTEDBY] [ITEMNAME] [PRICE] [QUANTITY]
// /UC_CASHOUT
// /UC_SALESSTATS [ITEMNAME]
//------------------------------------------------------//
//class CUSCommand
//
//------------------------------------------------------//
class CUSCommand
{
	public:
		CUSCommand(	int & command, 
					int & requestHandle, 
					CString  param1 = "", 
					CString  param2 = "", 
					CString  param3 = "", 
					CString  param4 = "",
					CString  param5 = "")
		{
			m_requestType		= command;
			m_fromhandle		= requestHandle;
			m_param1			= param1;
			m_param2			= param2;
			m_param3			= param3;
			m_param4			= param4;
			m_param5			= param5;
		}
		~CUSCommand(){}

	int				m_requestType;
	int				m_fromhandle;
	CString			m_param1;
	CString			m_param2;
	CString			m_param3;
	CString			m_param4;
	CString			m_param5;
};

//------------------------------------------------------//
//class CUnifiedCommerce
//
//item table name:			:	UC_ITEMS
//expected database columns	:	ITEMNAME, PRICE, QUANTITY, POSTEDBY, POSTDATE
//
//owed money table name		:	UC_MONEYOWED
//expected database columns :	CHARNAME, MONEYOWED
//
//sales history table name  :	UC_SALESSTATS
//expected database columns :	ITEMNAME, SOLDFOR, POSTDATE, SOLDBY
//------------------------------------------------------//
class CUnifiedCommerce : public CAxList
{
public:
	CUnifiedCommerce()
	{
		InitializeCriticalSection(&m_sect);
		m_debug			= false;

		//stencil query definitions
		m_byMostRecentQry.Format(	"SELECT TOP(%u) ITEMNAME, PRICE, QUANTITY, POSTEDBY,POSTDATE FROM UC_ITEMS PARAM1 ORDER BY POSTDATE DESC",UC_MAXRETURNCOUNT);
		m_byItemQry.Format(	"SELECT TOP(%u) ITEMNAME, PRICE, QUANTITY, POSTEDBY,POSTDATE FROM UC_ITEMS WHERE ITEMNAME LIKE 'PARAM1' ORDER BY PRICE",UC_MAXRETURNCOUNT);
		m_byItemQry.Replace("PARAM1","%PARAM1%");
		m_removeItemQry					=	"DELETE TOP(1) FROM UC_ITEMS WHERE ITEMNAME = 'PARAM1' AND PRICE = 'PARAM2' AND POSTEDBY = 'PARAM3' AND QUANTITY = 'PARAM4'";
		m_addItemQry					=	"INSERT INTO UC_ITEMS (ITEMNAME, PRICE, QUANTITY, POSTEDBY) Values('PARAM1','PARAM2','PARAM3','PARAM4')";
		m_getMoneyOwedQry				=	"SELECT MONEYOWED FROM UC_MONEYOWED WHERE CHARNAME = 'PARAM1'";
		m_clearMoneyOwedQry				=	"DELETE FROM UC_MONEYOWED WHERE CHARNAME = 'PARAM1'";
		m_salesStatsQry					=	"SELECT TOP(5) SOLDFOR FROM UC_SALESSTATS WHERE ITEMNAME = 'PARAM1' ORDER BY POSTDATE DESC";
		m_checkItemsAlreadyOnSaleQry	=	"SELECT * FROM UC_ITEMS WHERE POSTEDBY = 'PARAM1'";
		m_getItemQry					=   "SELECT * FROM UC_ITEMS WHERE ITEMNAME = 'PARAM1' AND PRICE = 'PARAM2' AND POSTEDBY = 'PARAM3' AND QUANTITY = 'PARAM4'";
		m_addSalesStatsQry				=	"INSERT INTO UC_SALESSTATS (ITEMNAME, SOLDFOR, SOLDBY) Values('PARAM1','PARAM2','PARAM3')";
		m_oweMoneyQry					=	"INSERT INTO UC_MONEYOWED (CHARNAME, MONEYOWED) Values('PARAM1','PARAM2')";
	
	}

	~CUnifiedCommerce()
	{
		for(CAxLinkObj * pos = GetHeadPosition_unsafe();pos != NULL;)
		{//in this case we do have to run this loop the CAxClanCommand isnt derive from CAxLinkObj
			CUSCommand * pData = (CUSCommand *)GetNext_unsafe(&pos);
			SafeDelete(pData);
		}
		RemoveAll();
		DeleteCriticalSection(&m_sect);
	}

	//variables
	CRITICAL_SECTION	m_sect;
	bool				m_debug;//enabled output to chat box
	CString				m_byMostRecentQry;
	CString				m_byItemQry;
	CString				m_salesStatsQry;
	CString				m_removeItemQry;
	CString				m_addItemQry;
	CString				m_getMoneyOwedQry;
	CString				m_clearMoneyOwedQry;
	CString				m_checkItemsAlreadyOnSaleQry;
	CString				m_getItemQry;
	CString				m_addSalesStatsQry;
	CString				m_oweMoneyQry;

	//functions
	BOOL IsThisHandleAlreadyRequesting_safe(int & requestHandle)
	{
		EnterCriticalSection(&m_sect);
		for(CAxLinkObj * pos = GetHeadPosition_unsafe();pos != NULL;)
		{//ordered from greatest to least
			CUSCommand * pData = (CUSCommand *)GetNext_unsafe(&pos);
			if(pData->m_fromhandle == requestHandle)
			{
				LeaveCriticalSection(&m_sect);
				return TRUE;
			}
		}
		LeaveCriticalSection(&m_sect);
		return FALSE;
	}

	BOOL AddCommand_safe(	int command, 
							int requestHandle, 
							CString param1 = "", 
							CString param2 = "", 
							CString param3 = "", 
							CString param4 = "",
							CString param5 = "")
	{
		if(IsThisHandleAlreadyRequesting_safe(requestHandle))
		   return FALSE;

		//add it to the que
		EnterCriticalSection(&m_sect);
		CUSCommand * cmd = new CUSCommand(command,requestHandle,param1,param2,param3,param4,param5);
		CAxLinkObj * newPtr		= new CAxLinkObj();
		newPtr->m_uniqueID		= requestHandle;
		newPtr->m_thisObject	= cmd;
		AddTail(newPtr);
		LeaveCriticalSection(&m_sect);

		return TRUE;
	}

	CUSCommand * GetNextCommand_safe()
	{
		EnterCriticalSection(&m_sect);
		CUSCommand * cmd = NULL;
		CAxLinkObj * pos = this->GetHeadPosition_unsafe();
		if(pos)
		{  
		   cmd = (CUSCommand *)pos->m_thisObject;
		   this->RemoveAt(cmd->m_fromhandle);
		   LeaveCriticalSection(&m_sect);
		   return cmd;//the reciever should delete memory of command
		}
		LeaveCriticalSection(&m_sect);
		return NULL;
	}
};

//thread callback
DWORD WINAPI UnifiedCommerceWorker(LPVOID lpParameter);





