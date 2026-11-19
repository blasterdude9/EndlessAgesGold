#include "stdafx.h"

#include <d3d9.h>
#include <afxtempl.h>
#include "CBattleSchedulerClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBattleSchedulerObj::CBattleSchedulerObj()
{
    m_type               = 0;//0=last man standing 1=Team Arena
	m_batlleMinimum      = 10;//will cancel match is not enough play (notify players)
    m_battleMaximum      = 30;
	m_battleDuration     = 30000;
	//restriction
	m_levelRangeMax      = 18;
	m_levelRangeMin      = 0;
    //signupList
    m_signups            = NULL;
	m_currentSignupCount = 0;
    //portal vars
	m_visualWldIndex     = 0;//file to load
	m_wldChannel         = 0;//channel
	m_spawnPointCount    = 0;
    m_spawnPositions     = NULL;
	
}

void CBattleSchedulerObj::ResetBattle()
{
     m_currentSignupCount = 0;
}



BOOL CBattleSchedulerObj::AddSignup(int playerHandle)
{
    if(m_currentSignupCount == 0 || m_signups == NULL)
	{//init
       if(m_signups)
	   {
          delete [] m_signups;
		  m_signups = 0;
		  
	   }
	   m_signups = new int[m_battleMaximum];
	   ZeroMemory(m_signups,sizeof(int)*m_battleMaximum);
	}//end init

    if(m_currentSignupCount == m_battleMaximum)
	   return FALSE;//too many

    for(int i=0;i<m_currentSignupCount;i++)
	{
        if(m_signups[i] == playerHandle)
		   return FALSE;
	}

	m_signups[m_currentSignupCount] = playerHandle;
    m_currentSignupCount++;
	return TRUE;
}

void CBattleSchedulerObj::AddSpawnPoint(D3DVECTOR position)
{

    if(m_spawnPointCount == 0 || m_spawnPositions == NULL)
	{//init
       if(m_spawnPositions)
	   {
          delete [] m_spawnPositions;
		  m_spawnPositions = 0;
	   }
	   m_spawnPositions = new D3DVECTOR[m_spawnPointCount+1];
	   m_spawnPositions[m_spawnPointCount].x = position.x;
       m_spawnPositions[m_spawnPointCount].y = position.y;
       m_spawnPositions[m_spawnPointCount].z = position.z;
       m_spawnPointCount++;
	   return;
	}//end init

	D3DVECTOR * spawnArrayTemp = new D3DVECTOR[m_spawnPointCount+1];
    for(int i=0;i<m_spawnPointCount;i++)
	{
       spawnArrayTemp[i].x = m_spawnPositions[i].x;
       spawnArrayTemp[i].y = m_spawnPositions[i].y;
       spawnArrayTemp[i].z = m_spawnPositions[i].z;
	}

	spawnArrayTemp[m_spawnPointCount].x = position.x;
    spawnArrayTemp[m_spawnPointCount].y = position.y;
    spawnArrayTemp[m_spawnPointCount].z = position.z;
    m_spawnPointCount = m_spawnPointCount+1;

	delete [] m_spawnPositions;
	m_spawnPositions = 0;
	m_spawnPositions = spawnArrayTemp;

}

void CBattleSchedulerObj::DeleteSpawnPoint(int index)
{
    if(!m_spawnPositions)
		return;

	int newCount = m_spawnPointCount-1;
	if(newCount < 1)
	{
       delete [] m_spawnPositions;
	   m_spawnPositions = 0;
       m_spawnPointCount=0;
	   return;
	}
	D3DVECTOR * tempArray = new D3DVECTOR[m_spawnPointCount-1];
    int tracer = 0;
	for(int i=0;i<m_spawnPointCount;i++)
	{
		if(i == index)
		   continue;

		tempArray[tracer].x = m_spawnPositions[i].x;
		tempArray[tracer].y = m_spawnPositions[i].y;
		tempArray[tracer].z = m_spawnPositions[i].z;
		tracer++;
	}

	delete [] m_spawnPositions;
	m_spawnPositions = 0;

    m_spawnPositions = tempArray;
	m_spawnPointCount--;
}

void CBattleSchedulerObj::ReplaceSpawnPoint(int index,D3DVECTOR position)
{

    if(m_spawnPointCount == 0 || m_spawnPositions == NULL)
	{//init
       return;
	}//end init


   m_spawnPositions[index].x = position.x;
   m_spawnPositions[index].y = position.y;
   m_spawnPositions[index].z = position.z;

}

D3DVECTOR CBattleSchedulerObj::GetSpawnPointData(int index)
{
    
    if(m_spawnPointCount == 0 || m_spawnPositions == NULL)
	{//init
       D3DVECTOR initPos;
       ZeroMemory(&initPos,sizeof(D3DVECTOR));
       return initPos;
	}//end init

   return m_spawnPositions[index];
}

D3DVECTOR CBattleSchedulerObj::GetRandomSpawnPoint()
{
    
    if(m_spawnPointCount == 0 || m_spawnPositions == NULL)
	{//init
       D3DVECTOR initPos;
       ZeroMemory(&initPos,sizeof(D3DVECTOR));
       return initPos;
	}//end init

    int selectedPoint = rand()%m_spawnPointCount;
	if(selectedPoint >= m_spawnPointCount)
	   selectedPoint = m_spawnPointCount-1;

   return m_spawnPositions[selectedPoint];
}

void CBattleSchedulerObj::SafeDelete()
{
	if(m_signups)
	{ 
       delete [] m_signups;
	   m_signups = 0;
	}

	if(m_spawnPositions)
	{ 
       delete [] m_spawnPositions;
	   m_spawnPositions = 0;
	}

}


CBattleSchedulerList::CBattleSchedulerList()
{
    m_nextArena               = 0;//arena that will be played next
    m_nextScheduledCalling    = GetTickCount()+ 30000;//dynamic time of naxt battle beginning
    m_signupsAreClosed        = FALSE;
}

void CBattleSchedulerList::Initialize()
{
    m_nextArena               = 0;//arena that will be played next
    m_nextScheduledCalling    = GetTickCount()+ 30000;//dynamic time of naxt battle beginning
    m_signupsAreClosed        = FALSE;
}

void CBattleSchedulerList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CBattleSchedulerObj * spnPtr = (CBattleSchedulerObj *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

void CBattleSchedulerList::SetNextArenaUp()
{
     CBattleSchedulerObj * currentArena = GetByIndex(m_nextArena);
	 if(currentArena)
	 {
        currentArena->ResetBattle();
	 }

	 m_nextScheduledCalling = GetTickCount()+120000;//5 min wait between matches

     //next arena setup
	 m_nextArena++;
     if(!GetByIndex(m_nextArena))
		 m_nextArena = 0;

	 m_signupsAreClosed = FALSE;//allow signup
}

CBattleSchedulerObj * CBattleSchedulerList::GetByIndex(int index)
{
    if(index < 0)
	   return NULL;

	POSITION posLoc = FindIndex(index);
	if(!posLoc)
		return NULL;
    CBattleSchedulerObj * tempPtr = (CBattleSchedulerObj *)GetAt(posLoc);
    return tempPtr;
}



void CBattleSchedulerObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	

       ar.SetObjectSchema(2); //DPD set to 5
       ASSERT(ar.GetObjectSchema() == 2); //DPD set to 5


	   ar << m_name
	      << m_type
	      << m_batlleMinimum
	      << m_battleMaximum
	      << m_battleDuration
	      << m_levelRangeMax
	      << m_levelRangeMin
	      << m_visualWldIndex
	      << m_spawnPointCount
		  << m_wldChannel;

	      for(int loop=0;loop<m_spawnPointCount;loop++)
		  {
             ar << m_spawnPositions[loop].x
	            << m_spawnPositions[loop].y
	            << m_spawnPositions[loop].z;
		  }

	}
	else
	{
       int version = ar.GetObjectSchema();
       if(version < 2)
	   {
	   long battleDurationVarChng = 0;
       ar >> m_name
	      >> m_type
	      >> m_batlleMinimum
	      >> m_battleMaximum
	      >> battleDurationVarChng
	      >> m_levelRangeMax
	      >> m_levelRangeMin
	      >> m_visualWldIndex
	      >> m_spawnPointCount
		  >> m_wldChannel;

	      m_battleDuration = battleDurationVarChng;

	      m_spawnPositions = NULL;
          if(m_spawnPointCount > 0)
		     m_spawnPositions = new D3DVECTOR[m_spawnPointCount];

	      for(int loop=0;loop<m_spawnPointCount;loop++)
		  {
             ar >> m_spawnPositions[loop].x
	            >> m_spawnPositions[loop].y
	            >> m_spawnPositions[loop].z;
		  }
	   }
	   else if(version == 2)
	   {
          ar  >> m_name
			  >> m_type
			  >> m_batlleMinimum
			  >> m_battleMaximum
			  >> m_battleDuration
			  >> m_levelRangeMax
			  >> m_levelRangeMin
			  >> m_visualWldIndex
			  >> m_spawnPointCount
			  >> m_wldChannel;

	      m_spawnPositions = NULL;
          if(m_spawnPointCount > 0)
		     m_spawnPositions = new D3DVECTOR[m_spawnPointCount];

	      for(int loop=0;loop<m_spawnPointCount;loop++)
		  {
             ar >> m_spawnPositions[loop].x
	            >> m_spawnPositions[loop].y
	            >> m_spawnPositions[loop].z;
		  }
	   }


	   m_signups = NULL;
	   m_currentSignupCount = 0;
	}
}  


IMPLEMENT_SERIAL(CBattleSchedulerObj, CObject, VERSIONABLE_SCHEMA|2)
IMPLEMENT_SERIAL(CBattleSchedulerList, CObList, 0)

