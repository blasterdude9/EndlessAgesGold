#include "stdafx.h"
#include <d3d9.h>

#include <afxtempl.h>
#include "CWorldAvailableClass.h"
#include "CAIICollectionQuest.h"
#include "CDplayPlayerClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CWorldAvailableObj::CWorldAvailableObj()
{
     m_worldName					= "None";
	 m_id							= 0;
	 m_instanceType					= FALSE;
	 m_connections					= 0;
	 m_lastUpdatedConnectionCount	= 0;
	 m_channelRef                   = -1;
	 m_creationTimeStamp			= GetTickCount();
	 m_lastProcessTimeStamp			= GetTickCount();
	 m_arenaEndSent					= FALSE;
	 m_numArenaBettors				= 0;
	m_arenaBetPool					= 0;

}

int CWorldAvailableObjList::GetChannelByName_safe(CString wldName)
{
   CAxList * obj = GetAllOnIndex(GetSimpleHash(wldName.GetBuffer()),"worldname");
   if(!obj)
	   return 0;

   int channel = -1;

   Lock();
   for(CAxLinkObj * posLoc = obj->GetHeadPosition_unsafe();posLoc != NULL;)
   {
       CWorldAvailableObj * wPtr = (CWorldAvailableObj *)obj->GetNext_unsafe(&posLoc);
       if(wPtr->m_worldName.CompareNoCase(wldName)==0)
	   {
          channel = wPtr->m_id;
		  break;
	   }
   }
   Unlock();
   return channel;
}

BOOL CWorldAvailableObjList::GetDirtyDataAndCleanDormant_safe(int & startindex, 
															  int cellSearchCount, 
															  CString * worldName, 
															  int * channel, 
															  int * instanceType, 
															  int * connections,
															  int * channelWasDeleted,
															  void * playerObjList)
{
	     *channelWasDeleted = -1;
	     if(startindex >= m_cellCount)
		    startindex = 0;//reset loop
         
	     int startPointStore = startindex;
	     int cellCountMod = startindex + cellSearchCount;
		 if(cellCountMod > m_cellCount)
		    cellCountMod = m_cellCount;

		 if(m_indexedVarCount <= 0)//only do one list because core objects are referenced between different search criteria-so they only have to be deleted from one list
		    return FALSE;

         DWORD currentTickCount = GetTickCount();

		 
		 ((CPlayerObjectList*)playerObjList)->Lock();
		 Lock();
	     unsigned int cLoop=startPointStore;
		 for(cLoop;cLoop<cellCountMod;cLoop++)
		 {
			 CAxLinkObj * posLast = NULL;
			 for(CAxLinkObj * pos = m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();(posLast = pos) != NULL;)
			 {//loop through
				 CWorldAvailableObj * wPtr = (CWorldAvailableObj *) m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
				 if(wPtr->m_lastUpdatedConnectionCount != wPtr->m_connections)
	             {
				   if(worldName)
					  *worldName	= wPtr->m_worldName;

				   if(channel)
					  *channel		= wPtr->m_id;//channel

				   if(instanceType)
					  *instanceType = wPtr->m_instanceType;

				   if(connections)
					  *connections  = wPtr->m_connections;

				   startindex = cLoop;//transfer externally

				   wPtr->m_lastUpdatedConnectionCount = wPtr->m_connections;

				   ((CPlayerObjectList*)playerObjList)->Unlock();
				   Unlock();
				   return TRUE;
				 }
				 else if(wPtr->m_connections <= 0 && (currentTickCount-wPtr->m_lastProcessTimeStamp) > 60000)
				 {//no body in zone for 5 minutes zone should be removed
					startindex = cLoop;//transfer externally

					int deadCount = 0;
					CAxList * rPlList = ((CPlayerObjectList*)playerObjList)->GetListByChannel(wPtr->m_id);
					for(CAxLinkObj * pos = rPlList->GetHeadPosition_unsafe();pos != NULL;)
					{
						CPlayerObject * plObj = (CPlayerObject *)rPlList->GetNext_unsafe(&pos);
						if(plObj->m_aiControlled)
							continue;
						deadCount++;
					}
					
					if(deadCount <= 0)
					{
						*channelWasDeleted = wPtr->m_id;
						DeleteByOject(wPtr);
						((CPlayerObjectList*)playerObjList)->Unlock();
						Unlock();
						if(m_dbConnection)
						{
							CString temp;
							temp.Format("%i",*channelWasDeleted);
							m_dbConnection->ExecQuery("DELETE FROM SYSLIVEZONEDB WHERE CHANNEL = '"+temp+"' AND NODEID = '"+m_nodeID+"'");
						}
						return TRUE;
					}
				 }//end no body in zone for 5 minutes zone should be removed
			 }//end loop through
			 ((CPlayerObjectList*)playerObjList)->Unlock();
			 Unlock();
			 Sleep(0);
			 ((CPlayerObjectList*)playerObjList)->Lock();
			 Lock();
		 }
		 startindex = cLoop;//transfer externally

		 ((CPlayerObjectList*)playerObjList)->Unlock();
		 Unlock();

		 return FALSE;
}

/*BOOL CWorldAvailableObjList::GetDirtyDataByIndex_safe(int startindex, CString * worldName, int * channel, int * instanceType, int * connections)
{
   if(startindex < 0)
      return FALSE;

   EnterCriticalSection(&m_sect);

   POSITION posLoc = FindIndex(startindex);
   if(!posLoc)
   {
	   LeaveCriticalSection(&m_sect);
	   return FALSE;
   }

   for(posLoc;posLoc != NULL;)
   {
	   CWorldAvailableObj * eventPtr = (CWorldAvailableObj *)GetNext(posLoc);
			
       if(eventPtr->m_lastUpdatedConnectionCount != eventPtr->m_connections)
	   {
		   if(worldName)
			  *worldName	= eventPtr->m_worldName;

		   if(channel)
			  *channel		= eventPtr->m_id;//channel

		   if(instanceType)
			  *instanceType = eventPtr->m_instanceType;

		   if(connections)
			  *connections  = eventPtr->m_connections;

           eventPtr->m_lastUpdatedConnectionCount = eventPtr->m_connections;
       LeaveCriticalSection(&m_sect);
	   return TRUE;
	   }

   }
    
   LeaveCriticalSection(&m_sect);
   return FALSE;
}*/

BOOL CWorldAvailableObjList::GetZoneDataByChannel_safe(int channelFind, CString * worldName, int * instanceType, int * connections)
{
   if(channelFind < 0)
      return FALSE;

   Lock();
   CWorldAvailableObj * wldPtr = GetWorldByChannel(channelFind);
   if(wldPtr)
   {	
	   if(worldName)
		  *worldName	= wldPtr->m_worldName;

	   if(instanceType)
		  *instanceType = wldPtr->m_instanceType;

	   if(connections)
		  *connections  = wldPtr->m_connections;

	   Unlock();
       return TRUE;
   }
   Unlock();
   return FALSE;
}

BOOL CWorldAvailableObjList::GetZoneDataByName_safe(CString worldNameInput, int * channel, int * instanceType, int * connections)
{
   CAxList * obj = GetAllOnIndex(GetSimpleHash(worldNameInput.GetBuffer()),"worldname");

   Lock();
   for(CAxLinkObj * posLoc = obj->GetHeadPosition_unsafe();posLoc != NULL;)
   {
       CWorldAvailableObj * wPtr = (CWorldAvailableObj *)obj->GetNext_unsafe(&posLoc);
       if(wPtr->m_worldName.CompareNoCase(worldNameInput)==0)
	   {

		   if(instanceType)
			  *instanceType		= wPtr->m_instanceType;

		   if(channel)
			  *channel			= wPtr->m_id;

		   if(connections)
			  *connections		= wPtr->m_connections;

		   Unlock();
           return TRUE;
	   }
   }
   Unlock();
   return FALSE;
}

BOOL CWorldAvailableObjList::IncrementZoneConnections_safe(int & channel)
{
   Lock();
   CWorldAvailableObj * wldPtr = GetWorldByChannel(channel);
   if(wldPtr)
   {
      wldPtr->m_connections++;
	  Unlock();
	  return TRUE;
   }
   Unlock();
   return FALSE;
}

BOOL CWorldAvailableObjList::DecrementZoneConnections_safe(int & channel)
{
	Lock();
	CWorldAvailableObj * wldPtr = GetWorldByChannel(channel);
	if(wldPtr)
	{
		wldPtr->m_connections--;
		wldPtr->m_lastProcessTimeStamp = GetTickCount();
		Unlock();
		return TRUE;
	}
	Unlock();
	return FALSE;
}


int CWorldAvailableObjList::GetOpenChannel()
{//todo index class and make proper id search
   Lock();
   int channel = m_channelAquireCreep++;
   while(GetWorldByChannel(channel))
   {
        channel++;
		Unlock();
		Sleep(0);
		Lock();
   }
   Unlock();
   return channel;
}

BOOL CWorldAvailableObjList::AddZone_safe(CString & worldName, 
										  int & channel, 
										  int & instanceType, 
										  CString & aiworldName, 
										  CString maxConnections,
										  int & channelRef)
{
   CWorldAvailableObj * eventPtr = new CWorldAvailableObj();
   eventPtr->m_id			= channel;
   eventPtr->m_worldName	= worldName;
   eventPtr->m_instanceType = instanceType;
   eventPtr->m_aiWorldName  = aiworldName;
   eventPtr->m_channelRef   = channelRef;


   if(m_dbConnection)
   {//non blocking db update
	  CString temp,type,stencilChannel;
	  temp.Format("%i",channel);
	  type.Format("%i",instanceType);
	  stencilChannel.Format("%i",channelRef);
      m_dbConnection->ExecQuery("INSERT INTO SYSLIVEZONEDB (ZONENAME, CHANNEL, NODEID, INSTANCETYPE, MAX_CONNECTIONS, AIZONENAME, STENCILCHANNEL) Values('"+worldName+"','"+temp+"','"+m_nodeID+"','"+type+"','"+maxConnections+"','"+aiworldName+"','"+stencilChannel+"')");
   }
   else
	   return FALSE;

   return Add_safe(eventPtr);
}

BOOL CWorldAvailableObjList::AddPendingZone_safe( CString & worldName, 
												  int  instanceType,
												  byte * buffer,
												  int bufLength)
{//function just adds to database and is created later-genarally private-arena zones
	if(!m_dbConnection)
		return FALSE;
   
	
	
	CString requestNodes = "SELECT * FROM SYSLIVESERVERNODES WHERE DATEADD(SECOND,25,CHECKINTIME) > GETDATE() ORDER BY CONNECTIONCOUNT";
	_RecordsetPtr availNodes = m_dbConnection->Query(requestNodes);
	int nodeCount = m_dbConnection->GetCount(availNodes);
	CString nodeID = "-1";
	for(int nodeSelLoop=0;nodeSelLoop < nodeCount;nodeSelLoop++)
	{//switch node logic
		nodeID					= m_dbConnection->GetStringData("NODEID",availNodes);
	}//end switch node logic
	m_dbConnection->Close(availNodes);
	if(nodeID == "-1")
	{
		if(	m_dbConnection->m_eventLogRef)
			m_dbConnection->m_eventLogRef->LogByPriority("AddPendingZone_safe : FAILED no node found:"+worldName,(DWORD)instanceType,2);
		return FALSE;
	}

	//check to see any similar zones that can conflict
	CString checkForDup = "SELECT * FROM SYSLIVEZONEDB WHERE ZONENAME = '"+worldName+"' AND INSTANCETYPE = '"+CSToString(instanceType)+"'";
	_RecordsetPtr duplicates = m_dbConnection->Query(checkForDup);
	int dupCount = m_dbConnection->GetCount(duplicates);
	m_dbConnection->Close(duplicates);
	if(dupCount > 0)
	{
		if(	m_dbConnection->m_eventLogRef)
			m_dbConnection->m_eventLogRef->LogByPriority("AddPendingZone_safe : FAILED duplicate pending zone in use:"+worldName,(DWORD)instanceType,2);
		return FALSE;
	}

   
	CString type;
	type.Format("%i",instanceType);
	//parse dynamic rules to string
	CString dynamicRules;
	if(bufLength > 0)
	{
		CString typeCnv;
		DYNRULES * castArray = (DYNRULES *)buffer;
		int ruleCount = bufLength/sizeof(DYNRULES);
		for(int drl=0;drl<ruleCount;drl++)
		{ 
			typeCnv.Format("%i,%i,",castArray[drl].m_rule,castArray[drl].m_param);
			dynamicRules += typeCnv;
		}
	}

	if(m_dbConnection->m_eventLogRef)
		m_dbConnection->m_eventLogRef->LogByPriority("AddPendingZone_safe : creating:"+worldName,(DWORD)instanceType,2);

	m_dbConnection->ExecQuery("INSERT INTO SYSLIVEZONEDB (ZONENAME, INSTANCETYPE, PENDING, NODEID, MAX_CONNECTIONS, DYNRULES) Values('"+worldName+"','"+type+"','1','"+nodeID+"','100','"+dynamicRules+"')");//all other data must be aquired at zone creation-needs default max connection to get started
	return TRUE;
}

BOOL CWorldAvailableObjList::MakePendingZoneLive_safe( CString & worldName, 
												       int & channel, 
												       int & instanceType, 
												       CString & aiworldName, 
												       CString maxConnections,
												       int & channelRef)
{//function just adds to database and is created later-genarally private-arena zones
   CWorldAvailableObj * eventPtr = new CWorldAvailableObj();
   eventPtr->m_id			= channel;
   eventPtr->m_worldName	= worldName;
   eventPtr->m_instanceType = instanceType;
   eventPtr->m_aiWorldName  = aiworldName;
   eventPtr->m_channelRef   = channelRef;
   
	
   if(m_dbConnection)
   {//non blocking db update
	  CString temp,type,stencilChannel;
	  temp.Format("%i",channel);
	  type.Format("%i",instanceType);
	  stencilChannel.Format("%i",channelRef);
	  m_dbConnection->ExecQuery("UPDATE SYSLIVEZONEDB SET STENCILCHANNEL = '"+stencilChannel+"',PENDING = '0',MAX_CONNECTIONS = '"+maxConnections+"',AIZONENAME = '"+aiworldName+"',CHANNEL = '"+temp+"' WHERE ZONENAME = '"+worldName+"' AND NODEID = '"+m_nodeID+"' AND PENDING = '1'");
   } 

   return Add_safe(eventPtr);
}

void CWorldAvailableObjList::ClearDB()
{
	if(m_dbConnection)
       m_dbConnection->ExecQuery("DELETE FROM SYSLIVEZONEDB WHERE NODEID = '"+m_nodeID+"'");
}

CSpawnCfgObj::CSpawnCfgObj()
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;
    m_worldReferenceIndex = 0;
	m_spawnPointName = "NONE";
}

void CSpawnCfgObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar << m_worldReferenceIndex
		  << m_position.x
		  << m_position.y
		  << m_position.z
		  << m_spawnPointName;
	}
	else
	{
       ar >> m_worldReferenceIndex
		  >> m_position.x
		  >> m_position.y
		  >> m_position.z
		  >> m_spawnPointName;
	}
}

void CSpawnCfgObjList::SafeDelete()
{
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {
       CSpawnCfgObj * eventPtr = (CSpawnCfgObj *)GetNext(posLoc);
       delete eventPtr;
	   eventPtr=0;
   }
   RemoveAll();
}

BOOL CWorldAvailableObjList::InitConnection(ConnectionStructureClass & con, CString serverIdstr, void * eventLog)
{
     if(m_dbConnection)
	    delete m_dbConnection;
	 m_dbConnection = 0;

	 m_nodeID = serverIdstr;
 
	 m_dbConnection = new CDBConnectionSystem(con.m_sqlServerIP,
			                                  con.m_sqlServerCatalog,
											  con.m_sqlServerUsername,
											  con.m_sqlServerPassword,
											  (CServerEventList*)eventLog);
    if(m_dbConnection->m_connectionEstablished == TRUE)
	   return TRUE;

    return FALSE;
}


IMPLEMENT_SERIAL(CSpawnCfgObj, CObject, 0)
IMPLEMENT_SERIAL(CSpawnCfgObjList, CObList, 0)


