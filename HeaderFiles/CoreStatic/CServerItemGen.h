#include "CServerItemClass.h"
#include "dbconnectionsystem.h"
#include "CAxList.h"
#pragma once

class CServerItemGenObj
{
public:
    CServerItemGenObj();
	~CServerItemGenObj()
	{
	   if(m_serverItem)
       {
          delete m_serverItem;
	      m_serverItem = 0;
       }
	}
    
    CServerItemObj * m_serverItem;
    BOOL             m_existsOnServer;
	DWORD            m_respawnDuration;
    DWORD            m_timeStamp;
	int              m_spawnGenID;
	int              m_refSpawnGenID;//in a zone instancing situation this one will show link, also a way to update when base object is changed
	CString          m_genName;
	DWORD            m_lastCallbackUpdate;
    
	void Clone(CServerItemGenObj ** clone)
	{
         CServerItemGenObj * newPtr = new CServerItemGenObj();
		 newPtr->m_existsOnServer				= FALSE;
		 newPtr->m_respawnDuration				= m_respawnDuration;
	     newPtr->m_timeStamp					= GetTickCount();
		 newPtr->m_lastCallbackUpdate			= GetTickCount();
		 newPtr->m_spawnGenID					= m_spawnGenID;
		 newPtr->m_genName						= m_genName;
		 m_serverItem->Clone(&newPtr->m_serverItem);

         *clone = newPtr;
	}
};

class CServerItemGenObjList : public CAxDB
{
public:
    CServerItemGenObjList()
	{
		  Init(2000,3);//determines memory commit and speed of query
		  AddIndexVariable(0,"spawnid");//
		  AddIndexVariable(1,"ref_spawnid");//
		  AddIndexVariable(2,"channel");//
		  m_spawnGenIDSearch    = 30000;//start point for dyn gen id search
		  m_count = 0;
		  m_lastCallbackStamp   = GetTickCount();
	      m_currentUpdateIndex  = 30000;//top 30 k is generated lower is stencil
		  m_dbConnection = NULL;
	};

	~CServerItemGenObjList()
	{
	      //traverse list manually to delete core class objects, the list will clear itself when deconstructed
		  Lock();
		  this->WriteStatus("cleaned_CServerItemGenObjList.txt");
		  if(m_indexedVarCount > 0)//only do one list because core objects are referenced between different search criteria-so they only have to be deleted from one list
			 for(unsigned int cLoop=0;cLoop<m_cellCount;cLoop++)
			 {
				 for(CAxLinkObj * pos = m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
				 {
					 CServerItemGenObj * delPtr = (CServerItemGenObj *) m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
					 delete delPtr;
					 delPtr = 0;
				 }
			 }
		  //internally the rest of the lsists should auto clean themselfs
		  Unlock();
		  if(m_dbConnection)
			delete m_dbConnection;
			m_dbConnection = 0;
	};

	//VARIABLES
    MatrixARB				m_matrixFunctions;
	int                     m_spawnGenIDSearch;
	int                     m_count;
	DWORD                   m_lastCallbackStamp;
	int                     m_currentUpdateIndex;
	CDBConnectionSystem   * m_dbConnection;//for updates



	//Functions
	bool Add_safe(CServerItemGenObj * itemMem)
	{
         if(!itemMem)//assert on add index-return false here
             return false;

		 bool res = true;
		 Lock();
         if(!AddIndexed((void*)itemMem, itemMem->m_spawnGenID, itemMem->m_spawnGenID,"spawnid"))
			 res = false;
         if(itemMem->m_refSpawnGenID > -1)
		   if(!AddIndexed((void*)itemMem, itemMem->m_refSpawnGenID, itemMem->m_spawnGenID,"ref_spawnid"))//trace
			 res = false;
         if(!AddIndexed((void*)itemMem, itemMem->m_serverItem->m_world, itemMem->m_spawnGenID,"channel"))//channel
			 res = false;
		 m_count++;
	     Unlock();

		 return res;
	}

	bool Add_u(CServerItemGenObj * itemMem)
	{
         if(!itemMem)//assert on add index-return false here
             return false;

		 bool res = true;
         if(!AddIndexed((void*)itemMem, itemMem->m_spawnGenID, itemMem->m_spawnGenID,"spawnid"))
			 res = false;
         if(itemMem->m_refSpawnGenID > -1)
		   if(!AddIndexed((void*)itemMem, itemMem->m_refSpawnGenID, itemMem->m_spawnGenID,"ref_spawnid"))//trace
			 res = false;
         if(!AddIndexed((void*)itemMem, itemMem->m_serverItem->m_world, itemMem->m_spawnGenID,"channel"))//channel
			 res = false;
		 m_count++;

		 return res;
	}

	BOOL DeleteByOject(CServerItemGenObj * itemMem)
	{
         if(!itemMem)
			 return FALSE;//should assert when used

		 BOOL res = FALSE;
		 if(RemoveIndexed(itemMem->m_spawnGenID,itemMem->m_spawnGenID, "spawnid"))
	       if(RemoveIndexed(itemMem->m_serverItem->m_world,itemMem->m_spawnGenID,"channel"))
			  res = TRUE;

		 if(itemMem->m_refSpawnGenID > -1)
		   if(!RemoveIndexed(itemMem->m_refSpawnGenID,itemMem->m_spawnGenID, "ref_spawnid"))
			 res = FALSE;
         
		 delete itemMem;
		 itemMem = 0;
		 m_count--;

		 return res;
	}

	int IncrementUpdateIndex()
	{
		Lock();
		int ret = m_currentUpdateIndex++;

		if(m_currentUpdateIndex >= 60000)
		   m_currentUpdateIndex = 30000;//top 30 k is generated lower is stencil

		Unlock();
		return ret;
	}

	BOOL DeleteAllOnChannel_safe(int channel,int * removedCount)
	{
         if(channel < 0)
			 return FALSE;//should assert when used

		 CAxList * obj = GetAllOnIndex(channel,"channel");
		 if(!obj)
		    return NULL;

		 BOOL res = TRUE;
		 int removedTrack = 0;
		 Lock();
         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CServerItemGenObj * itemMem = (CServerItemGenObj *)obj->GetNext_unsafe(&pos);
			 if(itemMem->m_serverItem->m_world == channel)
			 {
			    if(!DeleteByOject(itemMem))
					res = FALSE;
				else 
				    removedTrack++;
		     }
		 }
		 if(removedCount)
		   *removedCount = removedTrack;

		 Unlock();

		 return res;
	}

	int GetCount()
	{
		int trace = 0;
        Lock();
        trace = m_count;
		Unlock();
		return trace;
	}

	CServerItemGenObj * GetBySpawnID_LOS(int spawnID)
	{
         CAxList * obj = GetAllOnIndex(spawnID,"spawnid");
		
		 Lock();
         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CServerItemGenObj * plObj = (CServerItemGenObj *)obj->GetNext_unsafe(&pos);
			 if(plObj->m_spawnGenID == spawnID)
			 {
			    return plObj;
		     }
		 }
		 Unlock();
	     return NULL;
	}

	CServerItemGenObj * GetByChannel_LOS(int channel)
	{
         CAxList * obj = GetAllOnIndex(channel,"channel");

		 Lock();
         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CServerItemGenObj * itemMem = (CServerItemGenObj *)obj->GetNext_unsafe(&pos);
			 if(itemMem->m_serverItem->m_world == channel)
			 {
			    return itemMem;
		     }
		 }
		 Unlock();
	     return NULL;
	}

	BOOL VerifySpawnID_safe(int spawnID)
	{
         CAxList * obj = GetAllOnIndex(spawnID,"spawnid");
		 if(!obj)
		    return FALSE;

		 Lock();
         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CServerItemGenObj * plObj = (CServerItemGenObj *)obj->GetNext_unsafe(&pos);
			 if(plObj->m_spawnGenID == spawnID)
			 {
			    Unlock();
			    return TRUE;
			 }
		 }
		 Unlock();
	     return FALSE;
	}

	BOOL VerifyRefSpawnIDbyChannel_safe(int spawnID, int channel)
	{
         CAxList * obj = GetAllOnIndex(channel,"channel");

		 Lock();
         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CServerItemGenObj * plObj = (CServerItemGenObj *)obj->GetNext_unsafe(&pos);
			 if(plObj->m_refSpawnGenID == spawnID)
			 {
			    Unlock();
			    return TRUE;
			 }
		 }
		 Unlock();
	     return FALSE;
	}

	BOOL UpdateObjectOrAdd_safe(CServerItemGenObj * object, int spawnID)
	{		 
		 CAxList * obj = GetAllOnIndex(spawnID,"spawnid");

		 Lock();
         CAxLinkObj * posLast;
         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();(posLast = pos) != NULL;)
		 {
             CServerItemGenObj * delObj = (CServerItemGenObj *)obj->GetNext_unsafe(&pos);
			 if(delObj->m_spawnGenID == spawnID)
			 {
                DeleteByOject(delObj);
			    break;
			 }
		 }
		 

		 BOOL res = Add_u(object);//add this generator in

		 //find generators that were cloned by this spawn ID then update them
         CAxList * clone_list = GetAllOnIndex(spawnID,"ref_spawnid");
		 CServerItemGenObj * clonePtr = NULL;
         for(CAxLinkObj * pos = clone_list->GetHeadPosition_unsafe();(posLast = pos) != NULL;)
		 {
             CServerItemGenObj * delObj = (CServerItemGenObj *)clone_list->GetNext_unsafe(&pos);
			 if(delObj->m_refSpawnGenID == spawnID)
			 {
			    clonePtr = NULL;
				object->Clone(&clonePtr);
				clonePtr->m_spawnGenID = delObj->m_spawnGenID;
				clonePtr->m_serverItem->m_world = delObj->m_serverItem->m_world;
				clonePtr->m_refSpawnGenID = spawnID;
                
				delete delObj;
				delObj = 0;

				posLast->m_thisObject = clonePtr;

				if(!UpdateMemory_unsafe(clonePtr->m_spawnGenID,clonePtr->m_spawnGenID,"spawnid",(void *)clonePtr))
					res = FALSE;
		        if(!UpdateMemory_unsafe(clonePtr->m_serverItem->m_world,clonePtr->m_spawnGenID,"channel",(void *)clonePtr))
					res = FALSE;
			 }
		 }
		 Unlock();


	     return res;
	}

	

    BOOL CloneChannelToNewChannel_safe(int stencilChannel, int targetChannel);
    CString ConvToString(int number);
	BOOL InformGeneratorThatItCanRespawn(int generatorID);
	int GetUniqID_safe();
	bool GetFromSQL(CServerEventList *eventLog,void * itemGenMem);
	CString SanitizeFormatIn(CString text);
	CString SanitizeFormatOut(CString text);
	CServerItemGenObj * GetIGNEntityFromReader(_RecordsetPtr & entReader,CDBConnectionSystem * dbConnection,CABFunctionLib & cabLib,void * itemGenMem,CServerEventList *eventLog);
	bool GetFromSQL(CServerEventList *eventLog,void * itemGenMem,int spawnID,void * worldAvailMem);
};