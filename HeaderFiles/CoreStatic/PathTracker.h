#pragma once
#include "d3d9.h"
#include "matrixarb.h"

class CPathTrackerNode : public CObject
{
public:
	CPathTrackerNode(void){ZeroMemory(&m_position,sizeof(D3DVECTOR));};
	~CPathTrackerNode(void){};

	D3DVECTOR m_position;
};

class CPathTrackerNodeList : public CObList
{
public:
	CPathTrackerNodeList(void){m_currentPathCount=0;};
	~CPathTrackerNodeList(void){Clear();};

	int m_currentPathCount;//maintain manually for getcount speed
    

	void Clear()
	{
        for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
		{
            CPathTrackerNode * pNode = (CPathTrackerNode *)GetNext(posLoc);

			delete pNode;
			pNode = 0;
		}
		RemoveAll();
		m_currentPathCount = 0;
	}

	void AddPosition(D3DVECTOR & position)
	{
		CPathTrackerNode * node = new CPathTrackerNode();
        node->m_position = position;
        AddTail(node);
		m_currentPathCount++;
	}

	CPathTrackerNode * GetNodeByIndex(int index)
	{
		POSITION posLoc = FindIndex(index);
		 if(!posLoc)
			 return NULL;

		 CPathTrackerNode * pNode = (CPathTrackerNode *)GetAt(posLoc);
		 return pNode;
	}

	BOOL RemoveNext()
	{
         POSITION posLoc = FindIndex(0);
		 if(!posLoc)
			 return FALSE;

		 CPathTrackerNode * pNode = (CPathTrackerNode *)GetAt(posLoc);
         delete pNode;
		 pNode = 0;
         RemoveAt(posLoc);
		 m_currentPathCount--;
		 return TRUE;
	}
    
};

class CPathTracker
{
public:
	CPathTracker(void)
	{
	    m_targetID							= -1;//not assigned but > would be the trace of the target
		m_travelCloseEnoughDist             = 5.0f;//
	    m_maxPathCount						= 0;
	    m_distPerRecord						= 15.0f;
	    ZeroMemory(&m_curTravelToPosition,sizeof(D3DVECTOR));
		ZeroMemory(&m_currentTargetPosition,sizeof(D3DVECTOR));
		ZeroMemory(&m_curPosMe,sizeof(D3DVECTOR));
		ZeroMemory(&m_lastTargetRecordedPosition,sizeof(D3DVECTOR));
		m_liveRoute							= NULL;
		m_initialize                        = FALSE;
		m_maxed                             = FALSE;
		m_endOfPath                         = FALSE;
		m_calculationDuration               = 200;//init at 5 times a second
	    m_calulationStamp					= GetTickCount();
	};
	~CPathTracker(void)
	{
	   if(m_liveRoute)
	      delete m_liveRoute;
	   m_liveRoute = 0;
	
	};


	void SetParams(float travelCloseEnoughDist, float distPerRecord, int maxPathCount)
	{ 
         m_travelCloseEnoughDist = travelCloseEnoughDist;
		 m_distPerRecord = distPerRecord;
		 m_maxPathCount = maxPathCount;
	}


	int							m_targetID;
	int							m_maxPathCount;
	float						m_distPerRecord;
	BOOL                        m_maxed;//means must finish route and look around for target
	D3DVECTOR                   m_curPosMe;
	D3DVECTOR					m_curTravelToPosition;
	D3DVECTOR					m_lastTargetRecordedPosition;
	D3DVECTOR					m_currentTargetPosition;//chenges when updates, used for effecient search
	BOOL						m_initialize;
	BOOL                        m_endOfPath;
	MatrixARB                   m_matLib;
	float                       m_travelCloseEnoughDist;
	DWORD                       m_calculationDuration;
	DWORD                       m_calulationStamp;

	CPathTrackerNodeList	  * m_liveRoute;

	BOOL Consolidate(D3DVECTOR myCurPosition, D3DVECTOR targetPosition, int & targetID, DWORD & currentTime)
	{
        if(!m_liveRoute)
		    m_liveRoute = new CPathTrackerNodeList();

        m_liveRoute->Clear();
        CopyMemory(&m_curPosMe,&myCurPosition,sizeof(D3DVECTOR));
	    CopyMemory(&m_currentTargetPosition,&targetPosition,sizeof(D3DVECTOR));
	    CopyMemory(&m_curTravelToPosition,&targetPosition,sizeof(D3DVECTOR));
        CopyMemory(&m_lastTargetRecordedPosition,&targetPosition,sizeof(D3DVECTOR));
		m_liveRoute->AddPosition(targetPosition);
		m_lastTargetRecordedPosition = targetPosition;

		return TRUE;
	}

    BOOL Calculate(D3DVECTOR myCurPosition, D3DVECTOR targetPosition, int & targetID, DWORD & currentTime)
	{
	    if(currentTime - m_calulationStamp <= m_calculationDuration)
           return TRUE;

		m_calulationStamp = currentTime;
		
		m_endOfPath = FALSE;

        if(!m_liveRoute)
		    m_liveRoute = new CPathTrackerNodeList();

		m_initialize = FALSE;
        if(targetID != m_targetID)
		{//clear route
           if(m_liveRoute)
			  m_liveRoute->Clear();
  
		   m_targetID = targetID;
		   m_initialize = TRUE;
		}//end clear route

		if(m_initialize)
		{//init target and your pos
           CopyMemory(&m_curPosMe,&myCurPosition,sizeof(D3DVECTOR));
		   CopyMemory(&m_currentTargetPosition,&targetPosition,sizeof(D3DVECTOR));
		   CopyMemory(&m_curTravelToPosition,&targetPosition,sizeof(D3DVECTOR));
           CopyMemory(&m_lastTargetRecordedPosition,&targetPosition,sizeof(D3DVECTOR));
		}//end init target and your pos
		else
		{//calculate new positions
		   CopyMemory(&m_currentTargetPosition,&targetPosition,sizeof(D3DVECTOR));
		   CopyMemory(&m_curPosMe,&myCurPosition,sizeof(D3DVECTOR));
           
		   //Manage new traveling to position
		   if(m_matLib.MagnitudeSquared(myCurPosition,m_curTravelToPosition)<=m_travelCloseEnoughDist)
		   {//time to record a new travel to position
              
			  if(m_liveRoute->m_currentPathCount > 0)
			  {//can go to the next position
                 m_liveRoute->RemoveNext();
                 CPathTrackerNode * node = m_liveRoute->GetNodeByIndex(0);
				 if(node)
				 {
                    m_curTravelToPosition = node->m_position;
					
				 }
			  }//end can go to the next position
			  else
			  {//*end of the line
                  m_endOfPath = TRUE;

			  }//end*end of the line
		   }//end time to record a new travel to position
		   
		   //Manage new positions and route creation management
		   if(m_liveRoute->m_currentPathCount >= m_maxPathCount)
		      m_maxed = TRUE;//now must finish route then search or find a direct route
			  
		   if(!m_maxed)
		   if(m_matLib.MagnitudeSquared(targetPosition,m_lastTargetRecordedPosition)>m_distPerRecord)
		   {//time to record a new position
              m_liveRoute->AddPosition(targetPosition);
			  m_lastTargetRecordedPosition = targetPosition;
		   }//end time to record a new position
		}//end calculate new positions

		return TRUE;
	}
};
