#ifndef CAIRAIDCLASS_H
#define CAIRAIDCLASS_H
//-------------------------------//
//---------ai cfg option---------//
//-------------------------------//
class CAICfgOption : public CObject
{
	
     
	DECLARE_SERIAL( CAICfgOption );
	
    
public:
    CAICfgOption();
    
	int m_aiCfg;
	
	void Serialize( CArchive& ar );
	
};

class CAICfgOptionList : public CObList
{
public:
   CAICfgOptionList(){};
   void SafeDelete();
    
     
	DECLARE_SERIAL( CAICfgOptionList );
	
    

};
//-------------------------------//
//---------ai spawn point--------//
//-------------------------------//
class CAISpawnPoint : public CObject
{
	
     
	DECLARE_SERIAL( CAISpawnPoint );
	
    
public:
    CAISpawnPoint();
    

	D3DVECTOR m_position;
	int       m_worldID;
	int       m_spawnCfg;
	CAICfgOptionList * m_cfgList;//will randomize spawn
	BOOL      m_commander;
	
	void SafeDelete();
	void Serialize( CArchive& ar );
	
};

class CAISpawnPointList : public CObList
{
public:
   CAISpawnPointList(){};
   void SafeDelete();
    
     
	DECLARE_SERIAL( CAISpawnPointList );
	
    

};
//-------------------------------//
//---------ai raid object--------//
//-------------------------------//
class CAIRaidObj : public CObject
{
	
     
	DECLARE_SERIAL( CAIRaidObj );
	
    
public:
    CAIRaidObj();
    

	CString    m_raidName;
	long       m_raidDuration;
	long       m_raidManditoryWait;
	long       m_lastRaidTimeStamp;
	float      m_bonusPoints;
	CString    m_raidAlertMessage;
	float      m_raidAlertRadius;
	int        m_raidAlertWldID;
	D3DVECTOR  m_raidAlertPosition;
	int        m_percentSpawn;


	CAISpawnPointList * m_spawnPoints;

	void SafeDelete();
	void Serialize( CArchive& ar );
	
};

class CAIRaidObjList : public CObList
{
public:
   CAIRaidObjList(){};
   void SafeDelete();
    
     
	DECLARE_SERIAL( CAIRaidObjList );
	
    


    CAIRaidObj * CallBack();//returns AI Raid Object to execute

};

#endif