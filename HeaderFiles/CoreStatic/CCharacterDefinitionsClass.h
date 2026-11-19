#ifndef CCHARACTERDEFINITIONSCLASS_H
#define CCHARACTERDEFINITIONSCLASS_H



//----------------------------------------------//
//statistical class
//this class can define variables like strength dexterity intelligence ect


//----------------------------------------------//
class  CStatisticObject : public CObject
{
	
     
    DECLARE_SERIAL( CStatisticObject )
    
    
public:
	
	CStatisticObject();
	int          m_type;
    CString      m_statName;
	float        m_currentAmount;
	float        m_statCap;

	void        Clone(CStatisticObject ** copy);
	void        Serialize( CArchive& ar );

};

class  CStatisticObjectList : public CObList
{
public:
   CStatisticObjectList(){};
   void Clone(CStatisticObjectList ** copy);
   void SafeDelete();
   CStatisticObject * GetStatObjectByType(int type);
    
     
    DECLARE_SERIAL( CStatisticObjectList )
    
    
};
//----------------------------------------------//
//Affected By Stat Object
//basically this class tells what stats can improve chance of success
//also this class defines how useing a skill can improve your base
//stat values
//----------------------------------------------//
class  CAffectedByStatObject : public CObject
{
	
     
    DECLARE_SERIAL( CAffectedByStatObject )
    
    
public:
	
	CAffectedByStatObject();
	float          m_statisticalImprovement;
    float          m_statisticalImprovementCap;
	int            m_statType;
	float          m_improvementOfSuccessVariable;

	void           Clone(CAffectedByStatObject ** copy);
	void           Serialize( CArchive& ar );

};

class  CAffectedByStatObjectList : public CObList
{
public:
   CAffectedByStatObjectList(){};
   void SafeDelete();
   void Clone(CAffectedByStatObjectList ** copy);
    
     
    DECLARE_SERIAL( CAffectedByStatObjectList )
    
    
};
//----------------------------------------------//
//skill cost class
//this class defines what a skill ability will cost a user
//example : hiding can cloak - cost is enery ect
//engineering : weapon A 200 resource minerals and 5RT metal alloy
//----------------------------------------------//
class  CSkillCostObject : public CObject
{
	
     
    DECLARE_SERIAL( CSkillCostObject )
    
    
public:
	
	CSkillCostObject();
	int            m_type;
    int            m_costAmount;

    void           Clone(CSkillCostObject ** copy);
	void           Serialize( CArchive& ar );

};

class  CSkillCostObjectList : public CObList
{
public:
   CSkillCostObjectList(){};
   void Clone(CSkillCostObjectList ** copy);
   void SafeDelete();
    
     
    DECLARE_SERIAL( CSkillCostObjectList )
    
    
};
//----------------------------------------------//
//ability class
//this class defines what a skill can do or create
//example : hiding can cloak - engineering could create a tool

//----------------------------------------------//
class  CAbilityObject : public CObject
{
	
     
    DECLARE_SERIAL( CAbilityObject )
    
    
public:
	
	CAbilityObject();
	CString                m_name;
	int                    m_type;
    int                    m_miscInt;
	float                  m_minimumRequiredSkill;//must have at least this value in the skill to use ability
	float                  m_masterLimit;//this is when you basically never mess up
    CSkillCostObjectList * m_costOfAbilityList;
	float                  m_amountOfGainOnSuccess;
    //actions implemented by success
	int                    m_actionAttribute;//definable action upon success
	                       //0=none //1=generate resource //2=create item 
	                       //3=use missile database //4=use blend mode on self
	int                    m_actionAttributeInt;//misc count or index or type or mode ect
    CString                m_comment;
	int                    m_abilityDuration;
	BOOL                   m_abilityInUse;



	void           SafeDelete();
	void           Clone(CAbilityObject ** copy);
	void           Serialize( CArchive& ar );

};

class  CAbilityObjectList : public CObList
{
public:
   CAbilityObjectList(){};
   void SafeDelete();
   void Clone(CAbilityObjectList ** copy);
   CAbilityObject* GetAbilityByType(int type);
    
     
    DECLARE_SERIAL( CAbilityObjectList )
    
    
};
//----------------------------------------------//
//base skill class
//

//----------------------------------------------//
class  CBaseSkillObject : public CObject
{
	
     
    DECLARE_SERIAL( CBaseSkillObject )
    
    
public:
	
	CBaseSkillObject();
	CString                    m_skillName;
	int                        m_skillType;
    CAffectedByStatObjectList* m_affectedByStatList;
	CAbilityObjectList*        m_skillAbilities;
    float                      m_currentSkillValue;
    float                      m_skillCap;


	void           SafeDelete();
	void           Clone(CBaseSkillObject ** clone);
	void           Serialize( CArchive& ar );

};

class  CBaseSkillObjectList : public CObList
{
public:
   CBaseSkillObjectList(){};
   void SafeDelete();
   CBaseSkillObject* GetSkillByType(int type);
   void Clone(CBaseSkillObjectList ** copy);
    
     
    DECLARE_SERIAL( CBaseSkillObjectList )
    
    
};
//----------------------------------------------//
//Character definition class
//this is the main class that utilizes everyclass in this file
//include this class in the entity object
//----------------------------------------------//
class  CharacterDefObject : public CObject
{
	
     
    DECLARE_SERIAL( CharacterDefObject )
    
    
public:
	
	CharacterDefObject();
	CStatisticObjectList * m_statistics;
	CBaseSkillObjectList * m_skills;
	int                    m_skillInUse;//-1 = none
	int                    m_abilityInUse;//-1 = none
	int                    m_inUseTimeStamp;
	int                    m_durationInUse;
	int                    m_misInput1;



    int            AddStatistic(int type,
		                        CString name,
								float startAmount,
								float statCap);
	int            UseSkill(int skillType,
		                    int abilityType,
							int *actionRet,
							int *actionIndexMiscRet,
							CString * commentRet);

	int            AddAbility(int skillType,
							  int type,
							  int miscInt,
							  float minimumRequiredSkill,
							  float masterLimit,
							  float gainBasis,
							  CString name,
							  CString comment,
							  int duration);

	int            AddSkill(CString   skillName,
						    int       skillType,
						    float     initialSkillValue,
						    float     skillCap);

	void           Clone(CharacterDefObject ** copy);
    void           SafeDelete();
	void           Serialize( CArchive& ar );

};

class  CharacterDefObjectList : public CObList
{
public:
   CharacterDefObjectList(){};
   void SafeDelete();
   void Clone(CharacterDefObjectList ** copy);
    
     
    DECLARE_SERIAL( CharacterDefObjectList )
    
    
};




#endif