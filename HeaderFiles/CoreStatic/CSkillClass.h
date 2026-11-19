#include "CInventoryClass.h"
#include "CStatClass.h"

#ifndef CSKILLCLASS_H
#define CSKILLCLASS_H
//-------------------------------//
//--------Racial Adv Object------//
//-------------------------------//
class   CRacialAdvantageObject : public CObject
{
	
     
	DECLARE_SERIAL( CRacialAdvantageObject );
	
    
public:
	
	CRacialAdvantageObject();

	int         m_edbRef;
    float       m_adjustment;
	
	void        Clone(CRacialAdvantageObject ** clone);
	void        Serialize( CArchive& ar );
};

class   CRacialAdvantageList : public CObList
{
public:
   CRacialAdvantageList(){};
   void SafeDelete();
    
     
	DECLARE_SERIAL( CRacialAdvantageList );
	
    

	float GetAdjustmentByEdbIndex(int edbIndex);
	void Clone(CRacialAdvantageList ** clone);
};
//-------------------------------//
//------Skill Ability object-----//
//-------------------------------//
class   CSkillAbilityObject : public CObject
{
	
     
	DECLARE_SERIAL( CSkillAbilityObject );
	
    
public:
	
	CSkillAbilityObject();

	CString                m_abilityName;
	int                    m_menuID;//less than zero means no menu
	long                   m_minimumLevelToUse;
	float                  m_masterLevel;
	int                    m_functionType;//0=none 1=create item 2=shapeshift 3=hiding 
	int                    m_miscInt;//can be a glid, shapeshift db index, or duration of hiding
	int                    m_miscInt2;
	int                    m_miscInt3;
	int                    m_effectBind;
	int                    m_miscAmount;
	CInventoryObjList     *m_requiredItems;//if null nothing is required
	CInventoryObjList     *m_resultItems;//if successful this is what you will recieve
	CStatObjList          *m_requiredStatistics;//minimum statistics to attempt
	
	void        Clone(CSkillAbilityObject ** clone);
	void        Serialize( CArchive& ar );
	void        SafeDelete();
};

class   CSkillAbilityList : public CObList
{
public:
   CSkillAbilityList(){};
   
    
     
	DECLARE_SERIAL( CSkillAbilityList );
	
    

    CSkillAbilityObject * GetObjByIndex(int index);
	void Clone(CSkillAbilityList ** clone);
	void SafeDelete();
};
//-------------------------------//
//------Level Range Database-----//
//-------------------------------//
class   CLevelRangeObject : public CObject
{
	
     
	DECLARE_SERIAL( CLevelRangeObject );
	
    
public:
	
	CLevelRangeObject();
	long                m_rangeStart;
	long                m_rangeEnd;
	CString             m_levelTitle;
	CSkillAbilityList * m_abilityDB;
	int                 m_menuID;//-1 = no menu
	int                 m_profficiencyBonus;//
	int                 m_perfectiveBonus;//

	void        Clone(CLevelRangeObject ** clone);
	void        Serialize( CArchive& ar );
	void        SafeDelete();
};
//database
class   CLevelRangeList : public CObList
{
public:
   CLevelRangeList(){};
   
    
     
	DECLARE_SERIAL( CLevelRangeList );
	
    

	void SafeDelete();
    void Clone(CLevelRangeList ** clone);
	BOOL GetExperienceValueByLevel(int level,long & experiencePoints);
    CLevelRangeObject * GetLevelByExp(long exp);
	CLevelRangeObject * GetObjByIndex(int index);
};
//-------------------------------//
//----Skill object definitions---//
//-------------------------------//
class   CSkillObject : public CObject
{
	
     
	DECLARE_SERIAL( CSkillObject );
	
    
public:
	
	CSkillObject();
	CString                 m_skillName;
	float                   m_currentExperience;//depends
	int                     m_currentLevel;//depends
	int                     m_skillType;//individual number like an id
	float                   m_gainIncrement;//basis for gain (how fast a skill can be built)
	BOOL                    m_basicAbilitySkill;//means that it only has one function like hiding
	float                   m_basicAbilityExpMastered;//component of ability basic skill option
	int                     m_basicAbilityFunction;//component of ability basic-function resulting from successful throw
    float                   m_skillGainsWhenFailed;//0.0 is no gain when failed if not 0.0 then it will gain till that exp point then not gain from trying anymore
    float                   m_maxMultiplier;//
	BOOL                    m_minimalSave;
	long                    m_lastUseTime;
	BOOL                    m_useInternalTimer;
	long                    m_currentSkillDelay;
	BOOL                    m_filterEvalLevel;


	CLevelRangeList       * m_levelRangeDB;//defines what level you are and yout title in that skill
	CRacialAdvantageList  * m_raceAdvantageDB;//defines who will gain faster is specific skills

	void        Clone(CSkillObject ** clone);
	void        SafeDelete();
	BOOL        EvaluateLevelAndTitle(CString & title,int & level,CSkillObject * skillRef);
	int         UseSkill(long expOfTarget,
		                 int levelToUse,
		                 int ability,
		                 int curDBCfg,
		                 CInventoryObjList * currentInv,
						 int * specialFunction,
						 CStatObjList * currentStats,
						 BOOL * updateStatsToClient,
						 CLevelRangeObject ** levelRaisedPtr,
						 int * returnMiscInt,
						 float totalCurrentExp,
						 int maxItemsInInventory,
						 CSkillObject * dbObjectRef,
						 int * effectBind,
						 long & curGlobalSkillDelay,
					     long & curPlayerSkillTimeStamp);
	int         GetPercentageToNextLevel(CSkillObject * skillPtrRef);
	BOOL        EvaluateLevel(int & level,CSkillObject * skillRef);
	void        CloneMinimum(CSkillObject ** clone);
	void        Serialize( CArchive& ar );
	void        SerializeAlloc(CArchive& ar);
};

class   CSkillObjectList : public CObList
{
public:
	CSkillObjectList();
	void SafeDelete();
     
	DECLARE_SERIAL( CSkillObjectList );

	int   m_skillToDisplay;
	float m_totalExperience;
	float UpdateTotalExperienceSum();
	float m_speedGainCap;
  
	void Clone(CSkillObjectList ** clone);
	long GetTotalExp();
	void ExportToSQL();
	BOOL ExportToSQL(CString ip, CString database, CString usename, CString password);
	CSkillObject* GetBestSkill(CSkillObjectList * skillDB);
	CSkillObject* GetBestSkillWEval(CSkillObjectList * skillDB);
	long GetHighestSkillExp();
	int UseSkillByType(int type,
					   CSkillObjectList * dbList,
					   long expOfTarget,
					   int levelToUse,
					   int ability,
					   int curDBCfg,
					   CInventoryObjList * currentInv,
					   int * specialFunction,
					   CSkillObject ** newAddedSkill,
					   CStatObjList  *currentStats,
					   BOOL * updateStatsToClient,
					   CLevelRangeObject ** levelRaisedPtr,
					   CSkillObject ** skillPtr,
					   int * returnMiscInt,
					   int maxItemsInInventory,
					   int * effectBind,
					   long & curGlobalSkillDelay,
					   long & curPlayerSkillTimeStamp);
	CSkillObject * GetSkillByType(int type);
	CSkillObject * GetSkillByIndex(int index);
	CSkillObject * GetSkillByName(CString skillName);
	int GetSkillIndexByType(int type);
	CString GetTitleByLevelAndType(int type,int level);
	BOOL EvaluateRequiredSkillLevel(int type,int requiredLevel,CSkillObjectList * skillDBRef);
	int GetEffeciencyByTypeAndLevel(int type,CSkillObjectList * charSkillList,int * proficiency,int * perfective);
	int GetFreeID();
	void CloneMinimum(CSkillObjectList ** clone);
	void SerializeAlloc(CArchive& ar,int loadCount);
};

#endif