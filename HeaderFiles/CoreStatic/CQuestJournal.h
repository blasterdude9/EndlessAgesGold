#ifndef CQUESTJOURNAL_H
#define CQUESTJOURNAL_H

#include "CInventoryClass.h"
#include "CABFunctionLib.h"

static int MAXJOURNALITEMS = 20;

class   CQJRequiredItem : public CObject
{
	DECLARE_SERIAL( CQJRequiredItem );
	
    
public:
    CQJRequiredItem();
    
	CString m_itemName;
	int m_glid;
	int m_quanity;
	int m_logType;

    void Clone(CQJRequiredItem ** clone);
	void Serialize( CArchive& ar );
	
};

class   CQJRequiredItemList : public CObList
{
public:
	CQJRequiredItemList(){};
   
	void Clone(CQJRequiredItemList ** clone);
	CQJRequiredItem * GetByID(int id);
    void SafeDelete();  
	DECLARE_SERIAL( CQJRequiredItemList );
};

class   CQuestJournalObj : public CObject
{
	DECLARE_SERIAL( CQuestJournalObj );

public:
    CQuestJournalObj();
	~CQuestJournalObj();

	void Clone(CQuestJournalObj ** clone);
	void Serialize( CArchive& ar );
    
	CString m_questDescription;
	CString m_npcName;
	CString m_keyStatement;
	int m_currentProgression;
	int m_status;
	int m_mobToHuntID;
	int m_maxDropCount;
	int m_dropGlid;
	int m_dropPercentage;
	int m_lastSelectionIndex;
	BOOL m_undeletable;
	BOOL  m_autoComplete;//for hunting quests
	int   m_skillExpType;
    float m_expBonus;
    float m_thisGainCap;
    CQJRequiredItemList * m_requiredItems;

private:
	void SafeDelete();
	
};

class   CQuestJournalList : public CObList
{
	DECLARE_SERIAL( CQuestJournalList );

public:
	enum QJ_INSERT_LOCATION
	{
		QJ_HEAD = 0,
		QJ_TAIL
	};

	CQuestJournalList() {}

	void Clone(CQuestJournalList ** clone);
	void SafeDelete();
	void MakeMostRecentBySeer(CString seerName);
	void RemoveBySeer(CString seerName);
	BOOL DeleteEntryByNpcName(CString name);	
	BOOL BuildRequiredList(CQuestJournalObj * newPtr,
		CInventoryObjList * requiredItems,
		void * itemManagerMem,
		int logOption);
	CQuestJournalObj * GetMostRecentUncompleted();	
	CQuestJournalObj * GetBySeer(CString seerName);
	CQuestJournalObj * GetUncompletedQuestJournalEntryByTarget(int aiCfgId);
	CQuestJournalObj * GetByIndex(int index);
	BOOL LogQuest(CString npcName,
		CString description,
		CString keyStatement);
	BOOL LogQuest(CString description, 
		CInventoryObjList * requiredItems,
		CString m_npcName,
		CString m_keyStatement,
		void * itemManagerMem,
		BOOL succeeded,
		int curVisibleSelection,
		int logOption,
		int mobToHuntID,
		int maxDropCount,
		int dropGlid,
		int dropPercentage,
		BOOL autoComplete,
		int skillExpType,
		float expBonus,
		float thisGainCap,
		BOOL isProgressive,
		BOOL undeletable,
		CString closeJournalEntryByName,
		BOOL closeJournalOnSuccess,
		int & maxProgressions);
	BOOL AddQuestJournalEntry(CQuestJournalObj *pJournalEntry, QJ_INSERT_LOCATION eLocation);

	CABFunctionLib m_cabLib;
};

#endif