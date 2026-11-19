#ifndef CWORLDAVAILOBJ_H
#define CWORLDAVAILOBJ_H
#include "dbconnectionsystem.h"
#include "CAxList.h"

class CWorldAvailableObj
{
 
public:
	
	CWorldAvailableObj();
	~CWorldAvailableObj()
	{
		
		
	};
	
    CString   m_worldName;//UNIQUE SINCE 090706
	CString   m_aiWorldName;
	int       m_id;//channel //UNIQUE SINCE 090706
	int       m_instanceType;
	int       m_connections;
	int       m_lastUpdatedConnectionCount;
	int       m_channelRef;//
	DWORD     m_creationTimeStamp;
	DWORD     m_lastProcessTimeStamp;
	BOOL	  m_arenaEndSent;
	int		  m_numArenaBettors;
	int		  m_arenaBetPool;
};

//--------------------------------------
//uniques
//node id + channel id is gloabally unique , channel is unique per server
class   CWorldAvailableObjList : public CAxDB
{
public:
    CWorldAvailableObjList()
	{
		m_dbConnection=NULL;
		m_nodeID="-1";
		m_channelAquireCreep = 500;

		Init(100,4);//determines memory commit and speed of query
		AddIndexVariable(0,"channel");//unique
		AddIndexVariable(1,"instanceType");//
		AddIndexVariable(2,"worldname");//worldname-unique but hash id non-unique and can cause stacking somtimes
		AddIndexVariable(3,"channel_ref");
	}
    ~CWorldAvailableObjList()
    { 
      //traverse list manually to delete core class objects, the list will clear itself when deconstructed
	  Lock();
	  this->WriteStatus("cleaned_CWorldAvailableObjList.txt");
	  if(m_indexedVarCount > 0)//only do one list because core objects are referenced between different search criteria-so they only have to be deleted from one list
		 for(unsigned int cLoop=0;cLoop<m_cellCount;cLoop++)
		 {
			 for(CAxLinkObj * pos = m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
			 {
				 CWorldAvailableObj * delPtr = (CWorldAvailableObj *) m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
				 delete delPtr;
				 delPtr = 0;
			 }
		 }
	  //internally the rest of the lsists should auto clean themselfs
	  Unlock();
	  if(m_dbConnection)
	  {
			delete m_dbConnection;
			m_dbConnection = 0;
	  }
    }

    //list class will sync thread safe with DB
	CDBConnectionSystem                *m_dbConnection;
	CString								m_nodeID;//if the db connection is initialize this better be also
	int									m_channelAquireCreep;//loops between 500-60000 allocated dynamic zone channels

    //functions
	//Functions
    BOOL Add_safe(CWorldAvailableObj * wldMem)
    {
         if(!wldMem)//assert on add index-return false here
             return FALSE;

         if(DoesChannelExist_safe(wldMem->m_id))//no duplicate channels on same node
			 return FALSE;

		 Lock();
         if(AddIndexed((void*)wldMem, wldMem->m_id, wldMem->m_id,"channel"))//int
			if(AddIndexed((void*)wldMem, wldMem->m_instanceType, wldMem->m_id,"instanceType"))//dpnid dword/unsigned int
				if(AddIndexed((void*)wldMem, GetSimpleHash(wldMem->m_worldName.GetBuffer()), wldMem->m_id,"worldname"))//username text
					if(AddIndexed((void*)wldMem, wldMem->m_channelRef, wldMem->m_id,"channel_ref"))//username text
					{
						Unlock();
						return TRUE;
					}
	     Unlock();
		 return FALSE;
    }

	BOOL DeleteByOject_safe(CWorldAvailableObj * wldMem)
    {
         if(!wldMem)
			 return FALSE;//should assert when used

		 BOOL res = FALSE;
		 Lock();
		 if(RemoveIndexed(wldMem->m_id,wldMem->m_id, "channel"))
		    if(RemoveIndexed(wldMem->m_instanceType,wldMem->m_id, "instanceType"))
               if(RemoveIndexed(GetSimpleHash(wldMem->m_worldName.GetBuffer()),wldMem->m_id,"worldname"))
			      if(RemoveIndexed(wldMem->m_channelRef,wldMem->m_id,"channel_ref"))
			         res = TRUE;
         Unlock();
	
		 delete wldMem;
		 wldMem = 0;

		 return res;
    }

	BOOL DeleteByOject(CWorldAvailableObj * wldMem)
    {
         if(!wldMem)
			 return FALSE;//should assert when used

		 BOOL res = FALSE;
		 if(RemoveIndexed(wldMem->m_id,wldMem->m_id, "channel"))
			if(RemoveIndexed(wldMem->m_instanceType,wldMem->m_id, "instanceType"))
			   if(RemoveIndexed(GetSimpleHash(wldMem->m_worldName.GetBuffer()),wldMem->m_id,"worldname"))
			      if(RemoveIndexed(wldMem->m_channelRef,wldMem->m_id,"channel_ref"))
			         res = TRUE;

		 delete wldMem;
		 wldMem = 0;

		 return res;
    }

	BOOL DoesWorldExistByInstanceTypeAndName(int instanceType, CString worldName)
    {
		   CAxList * obj = GetAllOnIndex(instanceType,"instanceType");
		   Lock();
		   for(CAxLinkObj * posLoc = obj->GetHeadPosition_unsafe();posLoc != NULL;)
		   {
			   CWorldAvailableObj * wPtr = (CWorldAvailableObj *)obj->GetNext_unsafe(&posLoc);
			   if(wPtr->m_instanceType == instanceType && wPtr->m_worldName.CompareNoCase(worldName)==0)
			   {
				  Unlock();
				  return TRUE;
			   }
		   }
		   Unlock();
           return FALSE;
    }

	CWorldAvailableObj * GetWorldByChannel(int channel)
    {
		 CAxLinkObj * obj = GetByUniqueID(channel,channel,"channel");
		 if(obj)
		    return (CWorldAvailableObj *)obj->m_thisObject;
	     return NULL;
    }

	int CWorldAvailableObjList::GetStencilChannelByLiveChannel_safe(int channel)
	{
	   CAxList * obj = GetAllOnIndex(channel,"channel");
	   if(!obj)
		   return -1;

	   int stencilChannel = -1;

	   Lock();
	   for(CAxLinkObj * posLoc = obj->GetHeadPosition_unsafe();posLoc != NULL;)
	   {
		   CWorldAvailableObj * wPtr = (CWorldAvailableObj *)obj->GetNext_unsafe(&posLoc);
		   if(wPtr->m_id == channel)
		   {
			  stencilChannel = wPtr->m_channelRef;
			  break;
		   }
	   }
	   Unlock();
	   return stencilChannel;
	}

	BOOL CWorldAvailableObjList::DoesChannelExist_safe(int channel)
	{
	   CAxList * obj = GetAllOnIndex(channel,"channel");
	  
	   Lock();
	   for(CAxLinkObj * posLoc = obj->GetHeadPosition_unsafe();posLoc != NULL;)
	   {
		   CWorldAvailableObj * wPtr = (CWorldAvailableObj *)obj->GetNext_unsafe(&posLoc);
		   if(wPtr->m_id == channel)
		   {
			  Unlock();
			  return TRUE;
		   }
	   }
	   Unlock();
	   return FALSE;
	}

    BOOL InitConnection(ConnectionStructureClass & con, CString serverIdstr, void * eventLog);
	int GetChannelByName_safe(CString wldName);
	BOOL GetZoneDataByChannel_safe(int channelFind, CString * worldName, int * instanceType, int * connections);
	BOOL GetZoneDataByName_safe(CString worldNameInput, int * channel, int * instanceType, int * connections);
	BOOL AddZone_safe(CString & worldName, int & channel, int & instanceType, CString & aiworldName, CString maxConnections,int & channelRef);
	BOOL AddPendingZone_safe( CString & worldName,int instanceType,byte * buffer,int bufLength);
	BOOL MakePendingZoneLive_safe( CString & worldName, 
							       int & channel, 
							       int & instanceType, 
							       CString & aiworldName, 
							       CString maxConnections,
							       int & channelRef);
	BOOL DecrementZoneConnections_safe(int & channel);
	BOOL IncrementZoneConnections_safe(int & channel);
	BOOL GetDirtyDataAndCleanDormant_safe(int & startindex,int cellSearchCount, CString * worldName, int * channel, int * instanceType, int * connections,int * channelWasDeleted,void * playerObjList);
	int GetOpenChannel();
	void ClearDB();
};
//------------------------------------------------//
//------------------------------------------------//
class   CSpawnCfgObj : public CObject
{
	
     
    DECLARE_SERIAL( CSpawnCfgObj )
    
    
public:
	
	CSpawnCfgObj();

	D3DVECTOR   m_position;
    int         m_worldReferenceIndex;
	CString     m_spawnPointName;
    
	void        Serialize( CArchive& ar );
};
//------------------------------------------------//
//------------------------------------------------//
class   CSpawnCfgObjList : public CObList
{
public:
   CSpawnCfgObjList(){};
    
     
    DECLARE_SERIAL( CSpawnCfgObjList )
    
    

    void SafeDelete();
};

#endif