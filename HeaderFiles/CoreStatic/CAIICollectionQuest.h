#pragma once
#include "CDplayPlayerClass.h"
#include "CInventoryClass.h"
#include "CSkillClass.h"
#include "ServerCurrency.h"
#include "ItemManager.h"

struct DYNRULES
{
	int m_rule;
	int m_param;
};

class CAICollectionQuestObj : public CObject
{
	DECLARE_SERIAL( CAICollectionQuestObj );
	
public:
	
	CAICollectionQuestObj();

	int					m_index;
	int					m_completedKeyEnabled;//logs journal 0=none 1=store quest description 2=store quest completed 3=store both
	CString				m_title;
	CString				m_questDesription;
	CString				m_questCompletionMessage;
	CString				m_spawnToWorldName;
	int					m_type;//0= collection quest
	CInventoryObjList *	m_requiredItems;
	CInventoryObjList *	m_reward;
	int					m_skillTypeBonus;
	float				m_expBonus;
	float				m_maxExpLimit;
	int					m_spawnAiCfg;//-1 =  none
	D3DVECTOR			m_spawnPosition;
	int					m_worldID;
	long				m_spawnedAiLifeline;
	long				m_spawnStamp;
	int					m_spawnArrayCountOption;
	float				m_spawnArrayOffset;
	BOOL				m_randomRewardType;//
	BOOL				m_spawnToLocationOverride;
	BOOL				m_makeTargetRebirthPoint;
    int					m_raceSpecificAccess;//-1=whatever race
	CString				m_optionalCommand;
	int					m_miscAction;//0=none
	BOOL				m_progessBasedByQuestJournal;
	int					m_cashReward;
	int					m_requiredSkillType;
    int					m_requiredSkillLevel;
	int					m_mobToHuntID;
	int					m_maxDropCount;
	int					m_dropGlid;
	int					m_dropPercentage;
	BOOL				m_autocomplete;
    BOOL				m_openDialogWindow;
	int					m_spawnOnFail;
	CString				m_optionalIntruction;
	BOOL				m_listProgressionOptions;
	BOOL				m_undeletable;
	CString				m_portalLimitMsg;
	int					m_portalLimitID;
	int					m_expansionIDReq;//must equal or greater than to pass variable, used in expansion securities
	int					m_flagHostileOnSuccess;
	int					m_requireOwnershipOfKingdomIndex;//0=off  >1=zerobased conversion to kingdom database
	int					m_instanceGenerateType;//0=none 1=group based 2=arena based
	DWORD				m_instanceGenerateParam;//*sometimes if arena then its time based
	DWORD				m_instanceGenerateParam2;//*sometimes if arena then its a timeStamp
	DWORD				m_instanceGenerateParam3;//*sometimes if arena minimum before start timer begin
	DWORD				m_instanceGenerateParam4;//0=disabled
	DWORD               m_arenaStartTimerStamp;//0=disabled   this var is dynamic, cloning properly is not necessary
	CString				m_closeJournalEntryByName;
	BOOL				m_closeJournalOnSuccess;
	int					m_kingdomID;
	int					m_cashContribution;
	CString				m_miscString;
	CString				m_clientScript;
	CString				m_requiredScript;
	CString				m_rewardScript;
	BOOL				m_deleteByLocationID;
	BOOL				m_deleteAllByRequiredGLID;
	BOOL				m_deteteAllByRewardGLID;
	BOOL				m_clearInventory;
	BOOL				m_despawnOnReward;
	BOOL				m_flagTimer;
	DWORD				m_requiredTimeStamp;//this is the maxmimum time that has passed since the timer has been updated 0 = disabled
	
	int					m_maxDmgMod;
	int					m_minDmgMod;
	int					m_spawnRotationRange;

	int					m_ruleCount;
	DYNRULES		  *	m_rules;

	//temp storage-ussually used by scripts
	long              *m_dwMem;
	int                m_dwMemCount;

	int				   m_arenaBetPool;
	
	
	BOOL         EvaluateQuest(CInventoryObjList * playersInventory,
										  CSkillObjectList  * skillDatabase,
										  CSkillObjectList  * skillMainDatabase,
										  float * expGained,
										  CPlayerObject * currentPlayer,
										  int * optErrorCode,
										  CServerCurrencySystem * currencyDatabase,
										  CItemManager * m_itemManager);
	void		 ClearDWMEM();
	int			 AddToDWMEM(int maxCount,long dwVar);
	bool		 RemoveFromDWMEM(long dwVar);
	BOOL         VerifyInDWMEM(long dwVar);
	int			 DWMEM_getCount();
	void         SafeDelete();
	void         Clone(CAICollectionQuestObj ** clone);
	void         Serialize( CArchive& ar );
};

class   CAICollectionQuestList : public CObList
{
public:
   CAICollectionQuestList(){};
   void SafeDelete();
    
     
	DECLARE_SERIAL( CAICollectionQuestList );
	
    

    CAICollectionQuestObj * GetByAvailIndex(int baseSelection,int raceID,int * retTraceVis);
	void Clone(CAICollectionQuestList ** clone);
	CAICollectionQuestObj * GetByIndex(int selection);
	
};