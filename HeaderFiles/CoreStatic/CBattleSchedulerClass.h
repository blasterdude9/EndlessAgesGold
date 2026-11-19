#ifndef CBATTLESCHEDULERCLASS_H
#define CBATTLESCHEDULERCLASS_H

class  CBattleSchedulerObj : public CObject
{
	
     
	DECLARE_SERIAL( CBattleSchedulerObj );
	
    
public:
    CBattleSchedulerObj();
	//general vars
	CString                    m_name;
    int                        m_type;//0=last man standing
	int                        m_batlleMinimum;//will cancel match is not enough play (notify players)
    int                        m_battleMaximum;
	DWORD                      m_battleDuration;
	//restriction
	int                        m_levelRangeMax;
	int                        m_levelRangeMin;
    //signupList
	int                        m_currentSignupCount;
    int                       *m_signups;
    //portal vars
	int                        m_visualWldIndex;//file to load
	int                        m_wldChannel;//file to load
	int                        m_spawnPointCount;
    D3DVECTOR                 *m_spawnPositions;

	void AddSpawnPoint(D3DVECTOR position);
	void ReplaceSpawnPoint(int index,D3DVECTOR position);
	void DeleteSpawnPoint(int index);
	D3DVECTOR GetSpawnPointData(int index);
	D3DVECTOR GetRandomSpawnPoint();
	BOOL AddSignup(int playerHandle);
	void ResetBattle();
	void Serialize( CArchive& ar );
	void SafeDelete();
	
};
 
class  CBattleSchedulerList : public CObList
{
public:
   CBattleSchedulerList();
   

    
     
	DECLARE_SERIAL( CBattleSchedulerList );
	
    

    
	int           m_nextArena;//arena that will be played next
    DWORD         m_nextScheduledCalling;//dynamic time of naxt battle beginning
	BOOL          m_signupsAreClosed;

	void SafeDelete();
    CBattleSchedulerObj * GetByIndex(int index);
	void SetNextArenaUp();
	void Initialize();

};

#endif