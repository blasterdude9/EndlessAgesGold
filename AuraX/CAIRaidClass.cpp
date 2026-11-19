#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>

#include "CAIRaidClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//---------------------------------
CAICfgOption::CAICfgOption()
{
     m_aiCfg = 0;
}


void CAICfgOptionList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CAICfgOption * spnPtr = (CAICfgOption *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}


void CAICfgOption::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_aiCfg;

	}
	else
	{
       ar >> m_aiCfg;
	}
}     
//-------------------------------
CAISpawnPoint::CAISpawnPoint()
{
    ZeroMemory(&m_position,sizeof(D3DVECTOR));
	m_worldID    = 0;
	m_spawnCfg    = 0;
    m_cfgList    = NULL;//will randomize spawn
    m_commander  = FALSE;
}

void CAISpawnPoint::SafeDelete()
{
    if(m_cfgList)
	{
       m_cfgList->SafeDelete();
	   delete m_cfgList;
	   m_cfgList = 0;
	}
}

void CAISpawnPointList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CAISpawnPoint * spnPtr = (CAISpawnPoint *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}


void CAISpawnPoint::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_worldID
		  << m_spawnCfg
		  << m_cfgList
		  << m_commander
		  << m_position.x
		  << m_position.y
		  << m_position.z;

	}
	else
	{
       ar >> m_worldID
		  >> m_spawnCfg
		  >> m_cfgList
		  >> m_commander
		  >> m_position.x
		  >> m_position.y
		  >> m_position.z;
	}
}   
//-------------------------------  
CAIRaidObj::CAIRaidObj()
{
    m_raidDuration        = 600000;
    m_raidManditoryWait   = 1200000;
    m_bonusPoints         = 1.0f;
	m_spawnPoints         = NULL;
    m_raidAlertRadius     = 4000.0f;
    m_raidAlertWldID      = 0;
	m_percentSpawn        = 100;//100 max
    ZeroMemory(&m_raidAlertPosition,sizeof(D3DVECTOR));
	m_lastRaidTimeStamp = GetTickCount();
}

void CAIRaidObj::SafeDelete()
{
    if(m_spawnPoints)
	{
       m_spawnPoints->SafeDelete();
	   delete m_spawnPoints;
	   m_spawnPoints = 0;
	}
}

void CAIRaidObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CAIRaidObj * spnPtr = (CAIRaidObj *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

CAIRaidObj * CAIRaidObjList::CallBack()
{

	long currentTime = GetTickCount();
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CAIRaidObj * rdPtr = (CAIRaidObj *)GetNext(posLoc);
		if((currentTime - rdPtr->m_lastRaidTimeStamp) > rdPtr->m_raidManditoryWait)
		{//ready to spawn again
           //add random code here
		   if((rand()%100)<rdPtr->m_percentSpawn)
		   {//lets do this
		     rdPtr->m_lastRaidTimeStamp = currentTime;
             return rdPtr;
		   }//end lets do this
		}//end ready to spawn again
	}
	return NULL;
}


void CAIRaidObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_raidName
		  << m_raidDuration
		  << m_raidManditoryWait
		  << m_bonusPoints
		  << m_spawnPoints
		  << m_raidAlertMessage
		  << m_raidAlertRadius
		  << m_raidAlertWldID
		  << m_raidAlertPosition.x
		  << m_raidAlertPosition.y
		  << m_raidAlertPosition.z
		  << m_percentSpawn;
		  
	}
	else
	{
       ar >> m_raidName
		  >> m_raidDuration
		  >> m_raidManditoryWait
		  >> m_bonusPoints
		  >> m_spawnPoints
		  >> m_raidAlertMessage
		  >> m_raidAlertRadius
		  >> m_raidAlertWldID
		  >> m_raidAlertPosition.x
		  >> m_raidAlertPosition.y
		  >> m_raidAlertPosition.z
		  >> m_percentSpawn;

       //m_percentSpawn = 100;
	   m_lastRaidTimeStamp = GetTickCount();
	}
}   


IMPLEMENT_SERIAL(CAICfgOption, CObject, 0)
IMPLEMENT_SERIAL(CAICfgOptionList, CObList, 0)
IMPLEMENT_SERIAL(CAISpawnPoint, CObject, 0)
IMPLEMENT_SERIAL(CAISpawnPointList, CObList, 0)
IMPLEMENT_SERIAL(CAIRaidObj, CObject, 0)
IMPLEMENT_SERIAL(CAIRaidObjList, CObList, 0)

