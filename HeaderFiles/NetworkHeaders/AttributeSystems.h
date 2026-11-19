#pragma once
#include "sharednet.h"
#include "arenasystems.h"
#include "questsystems.h"



class CAttributeSystems
{
public:
	CAttributeSystems(CSharedNet * sharedInterface,CArenaSystems * arenaSys,CQuestSystems * questSys);
	~CAttributeSystems(void);

    //Vars
    CSharedNet * m_sharedInterfaceRef;
    CArenaSystems * m_arenaInterfaceRef;
	CQuestSystems * m_questSystemsRef;
 
	//Functions
	BOOL AtribRequestBadgeInfo(LPATTRIB_STRUCT formatedMessage, DPNID idFromLoc);
	BOOL AtribSkillAbilityUse_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
    BOOL AllowExtremeMovement(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL RequestAiPendingInfo_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
    BOOL HandleAttribMessageServerSide_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribRqstAttachmentStatesPlayerServerSide_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribRemovePlayerByNetworkID_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribRequestItemInfo_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribClientRequestPlayer_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribDeleteCharacterFromAccount_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribPlayerIsLooting(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribRequestCharList_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribSetInvenItemSwap_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribSetDeformableItem_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribSetSkillTitle_safe(LPATTRIB_STRUCT formatedMessage, DPNID idFromLoc);
	BOOL AtribRequestCharInventory_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribRequestCharArmedInventory_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribRequestCharUnarmedInventory_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribRequestBuyItemFromStore_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribRequestSellItemToStore_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribRequestRepairItem_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribRequestToLootItem_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribRequestToLootAllItems_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribRequestToBankItem_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribTakeItemFromBank_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribRequestPortal_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribUseItem_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribCancelTransaction_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribAcceptTradeTerms_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribDestroyItemByGlid_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribQuestHandling_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribRequestHouseCfg_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribExpandItemDataRequest_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribBuyItemFromHouseStore_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribAddItemToHouseStore_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribRemoveItemFromHouseStore_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribRemoveCashFromHouseStore_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribRequestToOrganize2BankItems_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribRequestToOrganize2InventoryItems_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL HandleQuestJournalRequest_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL ReportMurder_safe(int netAssignedID, int handle, int creditAmount);
	BOOL AtribAIServerRequestHousingState_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	void SendForceJetValuesIfAvail(CInventoryObjList * inventoryList,CPlayerObject * playerToReceive);
	BOOL AtribSkillAbilityUse(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc,CPlayerObject * playerObjectData,CAccountObject * accountPtr);
	BOOL RequestDynamicItem(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
	BOOL AtribRequestPlayerCash_safe(LPATTRIB_STRUCT formatedMessage,DPNID idFromLoc);
};
