#pragma once
#include "aiserverinitialization.h"
#include "CPacketQueClass.h"
#include "CAxList.h"


class CWorldManager
{
public:
	CWorldManager(CAIServerInitialization * aiserverInit,int aiSvrID);
	~CWorldManager(void);

	//Vars
	CString                             m_worldName;//EXG to rip spawngens and trees from
	int									m_worldChannel;//will be used in future additions to override the channel to create spawnable zones
    DWORD                               m_updateNetworkStamp;
	DWORD                               m_aiCycleDuration;
	float                               m_glAdjustFloat;
	double                              m_lastGLAdjust;
	double                              m_glAdjust;
	int                                 m_botCount;
	int                                 m_spawnTrace;
	BOOL                                m_shutDownChannel;

    MatrixARB                           m_mathLib;
	DWORD                               m_startTimeEmulate;
	LARGE_INTEGER					    StartTime;
	LARGE_INTEGER					    EndTime;
	LONGLONG                            QuadPartRef;

    CAIServerInitialization *			m_aiServerBaseRef;
	DWORD                               m_warmUpStamp;
	DWORD                               m_spawnGenStamp;
	DWORD                               m_spawnGenFreq;

	CWldObjectList			     * m_wldObjList;/////
	CSpawnObjList                * m_spawnGeneratorDB;//
	CCollisionObj                * m_collisionBase;/////
	CWldGroupObjList             * m_worldGroupList;//
	CAIWebObjList                * m_aiWebDatabase;//
	CReferenceObjList            * m_libraryReferenceDB;//
	CWaterZoneObjList            * m_waterZoneDB;/////
	CCollisionObjList            * m_collisionDatabase;
	CMovementObjList             * m_entObjRuntime;
	CMissileObjList              * m_missileRuntime;
	CExplosionObjList            * m_explosionRuntimeList;
	CPacketObjList               * m_inBox;
	//internal thread management
	DWORD                          m_axThreadID;
	HANDLE                         m_axThreadHandle;
    CRITICAL_SECTION               m_criticalSec;
    CRITICAL_SECTION               m_statusSec;

	POSITION                       m_curSpawnGenPos;

	//Functions
	BOOL InitWorldLocalData(CString worldFilePath,CString altCollisionFile);
	int RunWorld();
	void SpawnGeneratorCallback(DWORD currentTime);
	BOOL HandleAIDeadEntities(DWORD CurrentTime);
	void AIControlUnit();
	void ProcessPackets();
	void CallBackTimeOpen();
	void CallBackTimeCap();
	void SendAIStateToServerCallback(DWORD currentTickCount);
};

class CWorldManagerDatabase : public CAxDB
{
public:
   
	CWorldManagerDatabase()
	{
		m_count = 0;
        Init(300,1);//determines memory commit and speed of query
        AddIndexVariable(0,"channel");//channel unique

	};
	CWorldManagerDatabase::~CWorldManagerDatabase(void)
	{
		 //traverse list manually to delete core class objects, the list will clear itself when deconstructed
		  Lock();
		  this->WriteStatus("Cleaned CWorldManagerDatabase.txt");
		  if(m_indexedVarCount > 0)//only do one list because core objects are referenced between different search criteria-so they only have to be deleted from one list
			 for(unsigned int cLoop=0;cLoop<m_cellCount;cLoop++)
			 {
				 for(CAxLinkObj * pos = m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
				 {
					 CWorldManager * delPtr = (CWorldManager *) m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
					 if(delPtr)
					 {
						 if(delPtr->m_axThreadHandle)
						 {
							 WaitForSingleObject(delPtr->m_axThreadHandle, 2000);
						 }
						 delete delPtr;
						 delPtr = 0;
					 }
				 }
			 }
		  //internally the rest of the lsists should auto clean themselfs
		  Unlock();
	}

	//vars
	int m_count;

    bool Add_safe(CWorldManager * wldMem)
	{
         if(!wldMem)//assert on add index-return false here
             return false;

		 Lock();
         if(!AddIndexed((void*)wldMem, wldMem->m_worldChannel, wldMem->m_worldChannel,"channel"))
		 {
			 Unlock();
			 return false;
		 }
		 m_count++;
	     Unlock();

		 return true;
	}

	BOOL RemoveByObject(CWorldManager * wldMem)
	{
         if(!wldMem)
			 return FALSE;//should assert when used

		 EnterCriticalSection( &wldMem->m_aiServerBaseRef->m_interactSection);
		 wldMem->m_shutDownChannel = TRUE;
		 LeaveCriticalSection( &wldMem->m_aiServerBaseRef->m_interactSection);
		 bool res = RemoveIndexed(wldMem->m_worldChannel,wldMem->m_worldChannel, "channel");
		 assert(res);
		 m_count--;

		 return TRUE;
	}

	BOOL RemoveByObject_safe(unsigned int channel)
	{
		 Lock();
         CWorldManager * wldMem = GetByChannel(channel);
         if(wldMem)
		 if(RemoveIndexed(channel,channel, "channel"))
		 {
		     m_count--;
             //que thread to delete memory and close
			 EnterCriticalSection( &wldMem->m_aiServerBaseRef->m_interactSection);
			 wldMem->m_shutDownChannel = TRUE;
			 LeaveCriticalSection( &wldMem->m_aiServerBaseRef->m_interactSection);
		 }
         Unlock();

		 return TRUE;
	}

	unsigned int GetCount_safe()
	{
       int temp = 0;
	   Lock();
       temp = m_count;
	   Unlock();
	   return temp;
	}
	unsigned int GetCount() { return GetCount_safe(); }
  

	CWorldManager * GetByChannel(unsigned int channel)
	{
		Lock();
        CAxLinkObj * obj = GetByUniqueID(channel,channel,"channel");
		CWorldManager * result = obj ? (CWorldManager *)obj->m_thisObject : NULL;
		Unlock();
		return result;
	}

	BOOL AddPacket_safe(CPacketObj * packetPtr,unsigned int channel)
	{
		 if(!packetPtr) return FALSE;

		 Lock();
         CAxList * obj = GetAllOnIndex(channel,"channel");
		 if(obj)
		 {
			 for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
			 {
				 CWorldManager * plObj = (CWorldManager *)obj->GetNext_unsafe(&pos);
				 if(plObj && plObj->m_worldChannel==channel)
				 {
					EnterCriticalSection( &plObj->m_criticalSec);
					if(plObj->m_inBox)
						plObj->m_inBox->AddTail(packetPtr);
					LeaveCriticalSection( &plObj->m_criticalSec);
					
					Unlock();
					return TRUE;
				 }
			 }
		 }
		 Unlock();
		 //gets here delete the packet mem because it didnt get entered
         delete packetPtr;
		 packetPtr = 0;
	     return FALSE;
	}
};
