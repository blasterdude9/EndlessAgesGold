#include "stdafx.h"
#include "DmeUnifiedCommerce.h"
#include "servermain.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CString Sanitize(CString text)
{
	text.Replace("(","[OpenParan]");
	text.Replace(")","[CloseParan]");

    return text;
}

DWORD WINAPI UnifiedCommerceWorker(LPVOID lpParameter)
{	   
		CServerMain * classObj = (CServerMain *)lpParameter; 
		if(FAILED(CoInitialize(NULL)))
           return 0;

		CSharedNet * sNet = classObj->m_sharedInterface;
		CDBConnectionSystem * dbConnection = new CDBConnectionSystem(classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerIP,
			                                                         classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerCatalog,
																	 classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerUsername,
																	 classObj->m_sharedInterface->m_baseAccountDBConnection.m_sqlServerPassword,
																	 classObj->m_sharedInterface->m_eventLog);
		//verify connection established
        if(dbConnection->m_connectionEstablished == TRUE)
		   classObj->m_sharedInterface->m_eventLog->AddEvent("UCW Thread connected and running");
		else
		{
		   delete dbConnection;
		   dbConnection = 0;
		   classObj->m_sharedInterface->m_eventLog->AddEvent("UCW Thread Failed to connect");
		   EnterCriticalSection( &classObj->m_interactSection);
		   classObj->m_shutDownThreadCount++;
		   LeaveCriticalSection( &classObj->m_interactSection);
		   return 1;//leave because we have no basis to use the database
		}

		CUSCommand			*	cmd			= NULL;
		CString					queryTemp;
		CString					respBuild;
		CUnifiedCommerce	*	ucPtr		= &classObj->m_sharedInterface->m_unifiedCommSys;//better access to interface
        while(1)
		{
			EnterCriticalSection( &classObj->m_interactSection);
			if(classObj->m_systemShutDown)
			{
			   classObj->m_shutDownThreadCount++;
			   LeaveCriticalSection( &classObj->m_interactSection);
			   break;
			}
			LeaveCriticalSection( &classObj->m_interactSection);
			
			cmd = ucPtr->GetNextCommand_safe();
	        if(!cmd)
			{
				Sleep(100);
				continue;
			}

			switch(cmd->m_requestType)
			{
				case UC_LISTRECENT:
				{
					respBuild	=	"FillAH(2753,2757,";
					queryTemp	=	ucPtr->m_byMostRecentQry;
					
					//build condition

					if(cmd->m_param1.GetLength() > 2)
					{
						CString condBuild = "WHERE POSTDATE < '"+sNet->SanitizeFormatIn(cmd->m_param1)+"' ";
						queryTemp.Replace("PARAM1",condBuild);
					}
					else
						queryTemp.Replace("PARAM1","");

					if(ucPtr->m_debug)
						classObj->m_sharedInterface->m_eventLog->AddEvent("UCW processing UC_LISTRECENT:"+queryTemp);
					
					_RecordsetPtr rcdPtr = dbConnection->Query(queryTemp);
					int recordCount = dbConnection->GetCount(rcdPtr);

					for(int i=0;i<recordCount;i++)
					{
						respBuild += Sanitize(dbConnection->GetStringData("ITEMNAME",rcdPtr));
						respBuild += "&";
						respBuild += dbConnection->GetStringData("QUANTITY",rcdPtr);
						respBuild += "&";
						respBuild += dbConnection->GetStringData("PRICE",rcdPtr);
						respBuild += "&";
						respBuild += dbConnection->GetStringData("POSTEDBY",rcdPtr);
						respBuild += "&";
						respBuild += dbConnection->GetStringData("POSTDATE",rcdPtr);//this can be used as offset in this case
						if(i+1 < recordCount)
							respBuild += "&";
						else
							respBuild += ")";
						dbConnection->NextRecord(rcdPtr);
					}
					dbConnection->Close(rcdPtr);
					if(recordCount == 0)
						respBuild += ")";
					//send package
					sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
					CPlayerObject * retPtr = sNet->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandleAlive(cmd->m_fromhandle);
					if(retPtr)
					{
						if(ucPtr->m_debug)
							sNet->SendTextMsg(retPtr->m_currentID,respBuild,retPtr);
						sNet->SendClientScript(retPtr,respBuild,"");//menu id, visual id, batch text to parse by &
					}
					sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
					
					break;
				}
				case UC_LISTBYITEM:
				{
					respBuild	=	"FillAH(2753,2757,";
					queryTemp	=	ucPtr->m_byItemQry;
					queryTemp.Replace("PARAM1",sNet->SanitizeFormatIn(cmd->m_param1));
					queryTemp.Replace("PARAM2",sNet->SanitizeFormatIn(cmd->m_param2));
					queryTemp.Replace("PARAM3",sNet->SanitizeFormatIn(cmd->m_param3));
					queryTemp.Replace("PARAM4",sNet->SanitizeFormatIn(cmd->m_param4));

					if(ucPtr->m_debug)
						classObj->m_sharedInterface->m_eventLog->AddEvent("UCW processing UC_LISTBYITEM:"+queryTemp);
					
					_RecordsetPtr rcdPtr = dbConnection->Query(queryTemp);
					int recordCount = dbConnection->GetCount(rcdPtr);

					for(int i=0;i<recordCount;i++)
					{
						respBuild += Sanitize(dbConnection->GetStringData("ITEMNAME",rcdPtr));
						respBuild += "&";
						respBuild += dbConnection->GetStringData("QUANTITY",rcdPtr);
						respBuild += "&";
						respBuild += dbConnection->GetStringData("PRICE",rcdPtr);//this can be used as offset in this case
						respBuild += "&";
						respBuild += dbConnection->GetStringData("POSTEDBY",rcdPtr);
						respBuild += "&";
						respBuild += dbConnection->GetStringData("POSTDATE",rcdPtr);
						if(i+1 < recordCount)
							respBuild += "&";
						else
							respBuild += ")";
						dbConnection->NextRecord(rcdPtr);
					}
					dbConnection->Close(rcdPtr);

					if(recordCount == 0)
						respBuild += ")";
					//send package
					sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
					CPlayerObject * retPtr = sNet->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandleAlive(cmd->m_fromhandle);
					if(retPtr)
					{
						if(ucPtr->m_debug)
							sNet->SendTextMsg(retPtr->m_currentID,respBuild,retPtr);
						sNet->SendClientScript(retPtr,respBuild,"");//menu id, visual id, batch text to parse by &
					}
					sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
					
					break;
				}
				case UC_SELLITEM://[INTERNAL CHARNAME] [ITEMNAME] [QUANTITY] [PRICE]
				{
					//verify this character can place items based on how many items he or she has already placed
					queryTemp = ucPtr->m_checkItemsAlreadyOnSaleQry;
					queryTemp.Replace("PARAM1",sNet->SanitizeFormatIn(cmd->m_param1));

					if(ucPtr->m_debug)
						classObj->m_sharedInterface->m_eventLog->AddEvent("UCW processing UC_SELLITEM:"+queryTemp);

					_RecordsetPtr rcdPtr = dbConnection->Query(queryTemp);
					int postedCount = dbConnection->GetCount(rcdPtr);
					dbConnection->Close(rcdPtr);
					if(postedCount >= UC_MAXPOSTCOUNT)
					{//too many items already posted
						sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
						CPlayerObject * retPtr = sNet->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandleAlive(cmd->m_fromhandle);
						if(retPtr)
							sNet->SendTextMsg(retPtr->m_currentID,"You have too many items on sale already..",retPtr);
						sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
						break;
					}//end too many items already posted

					int itemGlid = sNet->m_itemManager->GetGLIDByName_safe(cmd->m_param2.GetBuffer());

					int price		= atoi(cmd->m_param4);
					int quantity	= atoi(cmd->m_param3);
					if(price < 0)
					{
						price = 1;
						cmd->m_param4 = "1";
					}
					if(price > 9000000)
					{
						cmd->m_param4 = "9000000";
						price = 9000000;
					}

					if(quantity < 0)
					{
						quantity = 1;
						cmd->m_param3 = "1";
					}
					if(quantity > 30000)
					{
						quantity = 30000;
						cmd->m_param3 = "30000";
					}

					//verify you have the item and quantity in inventory and remove it
					bool itemWasRemovedFromPlayer = false;

					if(itemGlid < 0)
					{//invalid item name
						sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
						CPlayerObject * retPtr = sNet->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandleAlive(cmd->m_fromhandle);
						if(retPtr)
							sNet->SendTextMsg(retPtr->m_currentID,"Invalid item name.."+cmd->m_param2,retPtr);
						sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
						break;
					}//end invalid item name
					
					sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
					CPlayerObject * retPtr = sNet->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandleAlive(cmd->m_fromhandle);
					if(retPtr)
					{
						if(retPtr->m_currentInventory->VerifyInventoryTransferable(itemGlid, quantity))
						{//remove the items
							if(!retPtr->m_currentInventory->DeleteByGLIDUnarmedOnlyWQuanity(itemGlid, quantity))
								sNet->m_eventLog->AddEvent("DeleteByGLIDUnarmedOnlyWQuanity Failed in unified commerce sys");

							sNet->SendTextMsg(retPtr->m_currentID,"you have placed an item for sale..",retPtr);
							itemWasRemovedFromPlayer = true;
						}
						else//user doesnt have these items
							sNet->SendTextMsg(retPtr->m_currentID,"you dont have these items..",retPtr);
					}
					sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();

					//add to items for sale to database
					if(itemWasRemovedFromPlayer)
					{//insert into db
						//"INSERT INTO UC_ITEMS (ITEMNAME, PRICE, QUANTITY, POSTEDBY) Values('$PARAM1','$PARAM2','$PARAM3','$PARAM4')";
						queryTemp	=	ucPtr->m_addItemQry;
						queryTemp.Replace("PARAM1",sNet->SanitizeFormatIn(cmd->m_param2));
						queryTemp.Replace("PARAM2",sNet->SanitizeFormatIn(cmd->m_param4));
						queryTemp.Replace("PARAM3",sNet->m_cabLib.ConvToString(quantity));
						queryTemp.Replace("PARAM4",sNet->SanitizeFormatIn(cmd->m_param1));

						if(ucPtr->m_debug)
							classObj->m_sharedInterface->m_eventLog->AddEvent("UCW processing UC_SELLITEM 2:"+queryTemp);

						dbConnection->ExecQuery(queryTemp);
					}
					
					break;
				}
				case UC_BUYITEM://[INTERNAL CHARNAME] [POSTEDBY] [ITEMNAME] [PRICE] [QUANTITY]
				{
					if(ucPtr->m_debug)
						classObj->m_sharedInterface->m_eventLog->AddEvent("UCW processing UC_BUYITEM");
					//get glid by item name
					int itemGlid = sNet->m_itemManager->GetGLIDByName_safe(cmd->m_param3.GetBuffer());
					BOOL stackable = sNet->m_itemManager->GetIsStackableByName_safe(cmd->m_param3.GetBuffer());
					if(itemGlid < 0)
					{//invalid item name
						sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
						CPlayerObject * retPtr = sNet->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandleAlive(cmd->m_fromhandle);
						if(retPtr)
							sNet->SendTextMsg(retPtr->m_currentID,"Invalid item name.."+cmd->m_param3,retPtr);
						sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
						break;
					}//end invalid item name

					//check for overloaded
					sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
					CPlayerObject * playerOvrloadCheck = sNet->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandleAlive(cmd->m_fromhandle);
					if(playerOvrloadCheck)
					{
						if(stackable)
						if(!playerOvrloadCheck->m_currentInventory->CanTakeObjects(sNet->m_netComObject->m_maxItemLimit_onChar,1))
						{//overloaded ?
							sNet->SendTextMsg(playerOvrloadCheck->m_currentID,"Must make room in your inventory before doing this..",playerOvrloadCheck);
							sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
							break;
						}//end overloaded ?

						if(!stackable)
						if(!playerOvrloadCheck->m_currentInventory->CanTakeObjects(sNet->m_netComObject->m_maxItemLimit_onChar,atoi(cmd->m_param5)))
						{//overloaded ?
							sNet->SendTextMsg(playerOvrloadCheck->m_currentID,"Must make room in your inventory before doing this..",playerOvrloadCheck);
							sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
							break;
						}//end overloaded ?
					}
					sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();

					//get existing count of this type
					queryTemp = ucPtr->m_getItemQry;//SELECT * FROM UC_ITEMS WHERE ITEMNAME = 'PARAM1' AND PRICE = 'PARAM2' AND POSTEDBY = 'PARAM3' AND QUANTITY = 'PARAM4'
					queryTemp.Replace("PARAM1",sNet->SanitizeFormatIn(cmd->m_param3));
					queryTemp.Replace("PARAM2",sNet->SanitizeFormatIn(cmd->m_param4));
					queryTemp.Replace("PARAM3",sNet->SanitizeFormatIn(cmd->m_param2));
					queryTemp.Replace("PARAM4",sNet->SanitizeFormatIn(cmd->m_param5));

					if(ucPtr->m_debug)
						classObj->m_sharedInterface->m_eventLog->AddEvent("UCW processing UC_BUYITEM 2:"+queryTemp);

					_RecordsetPtr rcdPtr = dbConnection->Query(queryTemp);
					int exitingItemCount = dbConnection->GetCount(rcdPtr);
					dbConnection->Close(rcdPtr);
					if(exitingItemCount <= 0)
					{//item does not exist
						sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
						CPlayerObject * retPtr = sNet->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandleAlive(cmd->m_fromhandle);
						if(retPtr)
							sNet->SendTextMsg(retPtr->m_currentID,"Cannot find this item in the unified store..",retPtr);
						sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
						break;
					}
					
					if(cmd->m_param1 == cmd->m_param2)
					{//see if this is your posted item if so just get it for free/remove it and place back into inventory
						//remove it from db
						queryTemp = ucPtr->m_removeItemQry;//DELETE TOP(1) FROM UC_ITEMS WHERE ITEMNAME = 'PARAM1' AND PRICE = 'PARAM2' AND POSTEDBY = 'PARAM3' AND QUANTITY = 'PARAM4'";
						queryTemp.Replace("PARAM1",sNet->SanitizeFormatIn(cmd->m_param3));
						queryTemp.Replace("PARAM2",sNet->SanitizeFormatIn(cmd->m_param4));
						queryTemp.Replace("PARAM3",sNet->SanitizeFormatIn(cmd->m_param2));
						queryTemp.Replace("PARAM4",sNet->SanitizeFormatIn(cmd->m_param5));
						dbConnection->ExecQuery(queryTemp);

						if(ucPtr->m_debug)
							classObj->m_sharedInterface->m_eventLog->AddEvent("UCW processing UC_BUYITEM 3:"+queryTemp);
						//add it to players inventory
						sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
						CPlayerObject * retPtr = sNet->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandleAlive(cmd->m_fromhandle);
						if(retPtr)
						{
							CItemObj * invObj = sNet->m_itemManager->GetItemByID_LOS(itemGlid);
							if(invObj)
							{
								retPtr->m_currentInventory->AddInventoryItem(atoi(cmd->m_param5),FALSE,invObj);
								sNet->m_itemManager->Unlock();
							}
							sNet->SendTextMsg(retPtr->m_currentID,"You have removed this item from the store..",retPtr);
						}
						sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
						break;
					}
					//verify player has the money and remove it then add item
					sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
					CPlayerObject * retPtr = sNet->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandleAlive(cmd->m_fromhandle);
					if(retPtr)
					{
						if(!sNet->m_currencySystem->RemoveCurrencyIfEnoughExists(sNet->m_itemManager,retPtr->m_currentInventory,-1,atoi(cmd->m_param4)))
						{//cant afford this
							sNet->SendTextMsg(retPtr->m_currentID,"You cannot afford this item..",retPtr);
							sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
							break;
						}
						else
						{//bought it
							CItemObj * invObj = sNet->m_itemManager->GetItemByID_LOS(itemGlid);
							if(invObj)
							{
								retPtr->m_currentInventory->AddInventoryItem(atoi(cmd->m_param5),FALSE,invObj);
								sNet->m_itemManager->Unlock();
							}
							sNet->SendTextMsg(retPtr->m_currentID,"You have purchased :"+cmd->m_param3,retPtr);
						}
					}
					sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
					//delete from database
					queryTemp = ucPtr->m_removeItemQry;//DELETE TOP(1) FROM UC_ITEMS WHERE ITEMNAME = 'PARAM1' AND PRICE = 'PARAM2' AND POSTEDBY = 'PARAM3' AND QUANTITY = 'PARAM4'";
					queryTemp.Replace("PARAM1",sNet->SanitizeFormatIn(cmd->m_param3));
					queryTemp.Replace("PARAM2",sNet->SanitizeFormatIn(cmd->m_param4));
					queryTemp.Replace("PARAM3",sNet->SanitizeFormatIn(cmd->m_param2));
					queryTemp.Replace("PARAM4",sNet->SanitizeFormatIn(cmd->m_param5));
					dbConnection->ExecQuery(queryTemp);

					if(ucPtr->m_debug)
							classObj->m_sharedInterface->m_eventLog->AddEvent("UCW processing UC_BUYITEM 4:"+queryTemp);

					//add to money owed to seller
					queryTemp = ucPtr->m_oweMoneyQry;
					queryTemp.Replace("PARAM1",sNet->SanitizeFormatIn(cmd->m_param2));
					queryTemp.Replace("PARAM2",sNet->SanitizeFormatIn(cmd->m_param4));
					dbConnection->ExecQuery(queryTemp);

					//add to sales statistics
					queryTemp = ucPtr->m_addSalesStatsQry;//"INSERT INTO UC_SALESSTATS (ITEMNAME, SOLDFOR,SOLDBY) Values('PARAM1','PARAM2','PARAM3')"
					queryTemp.Replace("PARAM1",sNet->SanitizeFormatIn(cmd->m_param3));
					queryTemp.Replace("PARAM2",sNet->SanitizeFormatIn(cmd->m_param4));
					queryTemp.Replace("PARAM3",sNet->SanitizeFormatIn(cmd->m_param2));
					dbConnection->ExecQuery(queryTemp);

					if(ucPtr->m_debug)
							classObj->m_sharedInterface->m_eventLog->AddEvent("UCW processing UC_BUYITEM 5:"+queryTemp);
					
					break;
				}
				case UC_SALESSTATS://[ITEMNAME]
				{
					
					respBuild	=	"FillList(2753,2759,";
					queryTemp	=	ucPtr->m_salesStatsQry;
					queryTemp.Replace("PARAM1",sNet->SanitizeFormatIn(cmd->m_param1));

					if(ucPtr->m_debug)
						classObj->m_sharedInterface->m_eventLog->AddEvent("UCW processing UC_SALESSTATS:"+queryTemp);

					_RecordsetPtr rcdPtr = dbConnection->Query(queryTemp);
					int recordCount = dbConnection->GetCount(rcdPtr);

					for(int i=0;i<recordCount;i++)
					{
						respBuild += dbConnection->GetStringData("SOLDFOR",rcdPtr);
						if(i+1 < recordCount)
							respBuild += "&";
						else
							respBuild += ")";
						dbConnection->NextRecord(rcdPtr);
					}
					dbConnection->Close(rcdPtr);

					//send package
					sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
					CPlayerObject * retPtr = sNet->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandleAlive(cmd->m_fromhandle);
					if(retPtr)
					{
						if(ucPtr->m_debug)
							sNet->SendTextMsg(retPtr->m_currentID,respBuild,retPtr);
						sNet->SendClientScript(retPtr,respBuild,"");//menu id, visual id, batch text to parse by &
					}
					sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();

					break;
				}
				case UC_CASHOUT:
				{
					int credits =	0;
					respBuild	=	"";
					queryTemp	=	ucPtr->m_getMoneyOwedQry;
					queryTemp.Replace("PARAM1",sNet->SanitizeFormatIn(cmd->m_param1));

					if(ucPtr->m_debug)
						classObj->m_sharedInterface->m_eventLog->AddEvent("UCW processing UC_CASHOUT:"+queryTemp);
					
					_RecordsetPtr rcdPtr = dbConnection->Query(queryTemp);
					int recordCount = dbConnection->GetCount(rcdPtr);

					for(int i=0;i<recordCount;i++)
					{
						credits += dbConnection->GetIntData("MONEYOWED",rcdPtr);
						dbConnection->NextRecord(rcdPtr);
					}
					dbConnection->Close(rcdPtr);

					if(credits > 0)
					{//clear DB before locking and applying credits
						queryTemp	=	ucPtr->m_clearMoneyOwedQry;
						queryTemp.Replace("PARAM1",sNet->SanitizeFormatIn(cmd->m_param1));

						if(ucPtr->m_debug)
						classObj->m_sharedInterface->m_eventLog->AddEvent("UCW processing UC_CASHOUT 2:"+queryTemp);

						dbConnection->ExecQuery(queryTemp);
						//apply to character and notify of funds recieved
						sNet->m_netComObject->m_runtimePlayersOnServer->Lock();
						CPlayerObject * retPtr = sNet->m_netComObject->m_runtimePlayersOnServer->GetObjectByHandleAlive(cmd->m_fromhandle);
						if(retPtr)
						{
							sNet->m_currencySystem->AddCurrency(sNet->m_itemManager,retPtr->m_currentInventory,-1,credits);
							respBuild.Format("You have sold items, %u credits have been placed into your inventory!!",credits);
							sNet->SendTextMsg(retPtr->m_currentID,respBuild,retPtr);
						}
						sNet->m_netComObject->m_runtimePlayersOnServer->Unlock();
					}

					break;
				}
				default:
				{
					classObj->m_sharedInterface->m_eventLog->AddEvent("UCW Unknown command");
					break;
				}
			}

			SafeDelete(cmd);
			Sleep(5);
		}

		delete dbConnection;
		dbConnection = 0;

		CoUninitialize();
return 1;
}


