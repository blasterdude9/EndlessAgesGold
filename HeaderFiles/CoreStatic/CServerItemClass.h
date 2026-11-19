#pragma once

#include "matrixarb.h"
#include "CInventoryClass.h"
#include "CAIICollectionQuest.h"

enum ServerItemTypes : unsigned char
{
	LOOTABLE_CHEST = 0,
	STATIC_RANDOM_SPEECH_AI,
	STATIC_SEQUENCED_SPEECH_AI,
	SELECTION_AI,
	ARENA_BOOKIE,
	BOUNTY_NPC
};

class CServerItemObj
{  	
public:
    CServerItemObj();
	~CServerItemObj();
    
	int            m_world;    //server catagorie
	int            m_dbRef;    //EDB reference
    D3DVECTOR      m_position; //cur position in world space
	int            m_curAnim;  //current animation of entity 
	byte           m_animDirection;//0=foward 1=backwards
	int            m_glid;     //global id for inventory modifications
    int            m_spawnID;  //spawn Gen index for notification
	int			   m_refSpawnId;
	int            m_networkAssignedID;//network managed ID
	int            m_requiredKey;
	int            m_rotationAdjust;//simple rotation transform option
	float          m_interactRadius;
	BOOL           m_autoTrigger;

	//optional exact orientation
	bool           m_useExactOrientation;
	D3DVECTOR      m_upVector;
	D3DVECTOR      m_dirVector;
	//deformable cfg
	int           *m_spawnCfg;
	int            m_spawnCfgCount;

	//armed inventory
	CString        m_displayedName;//visible name
	//int            m_staticItemType;//0 lootable chest container 1=AI STATIC Random speech 2=AI STATIC sequenced speech 3=selection ai
	ServerItemTypes m_staticItemType;
    
	//sequenced Talk system
	CString       *m_chatStrings;//
	int            m_chatStringCount;//
	int            m_chatStringCurIndex;

    //extended options
	int            m_randomAnimation;//-1 = none
	int            m_randomExpEffect;//-1 = none
	long           m_randomInterval;//in milliseconds 
	int            m_mountOption;//-1 = none

	long           m_ranTimeStamp;


	int            m_organizeGroupID;

	CString        m_noResponseMsg;

	float          m_sightRange;
    
	CString        m_botBrain;//blank is none


	//collection quest object
	CAICollectionQuestList * m_collectionQuestDB;
    CInventoryObjList * m_contents;

	void Clone(CServerItemObj ** clone);
};


class CServerItemList : public CAxDB
{
public:
   CServerItemList()
   {
      Init(2000,4);//determines memory commit and speed of query
      AddIndexVariable(0,"net_id");//m_networkAssignedID unique
      AddIndexVariable(1,"channel");//m_world
	  AddIndexVariable(2,"spawn_id");//m_spawnID
	  AddIndexVariable(3,"refspawn_id");//m_spawnID
   };

   ~CServerItemList()
   {
      //traverse list manually to delete core class objects, the list will clear itself when deconstructed
	  Lock();
	  this->WriteStatus("cleaned_CServerItemList.txt");
	  if(m_indexedVarCount > 0)//only do one list because core objects are referenced between different search criteria-so they only have to be deleted from one list
	     for(unsigned int cLoop=0;cLoop<m_cellCount;cLoop++)
	     {
             for(CAxLinkObj * pos = m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
			 {
		         CServerItemObj * delPtr = (CServerItemObj *) m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
				 delete delPtr;
				 delPtr = 0;
			 }
		 }
	  //internally the rest of the lsists should auto clean themselfs
	  Unlock();
   };

   //vars
   MatrixARB        GL_matrixFunctions;

   //Functions
   BOOL Add_safe(CServerItemObj * svrItemMem)
   {
         if(!svrItemMem)//assert on add index-return false here
             return FALSE;

		 Lock();
 
		 if(svrItemMem->m_refSpawnId > -1)
			AddIndexed((void*)svrItemMem, svrItemMem->m_refSpawnId, svrItemMem->m_networkAssignedID,"refspawn_id");//partial

         if(AddIndexed((void*)svrItemMem, svrItemMem->m_networkAssignedID, svrItemMem->m_networkAssignedID,"net_id"))
           if(AddIndexed((void*)svrItemMem, svrItemMem->m_world, svrItemMem->m_networkAssignedID,"channel"))//trace
		     if(AddIndexed((void*)svrItemMem, svrItemMem->m_spawnID, svrItemMem->m_networkAssignedID,"spawn_id"))//trace
			   {
				   Unlock();
				   return TRUE;
			   }
	     Unlock();
		 return FALSE;
   }

   BOOL DeleteByOject(CServerItemObj * svrItemMem)
   {
         if(!svrItemMem)
			 return FALSE;//should assert when used

		 BOOL res = FALSE;
		 if(RemoveIndexed(svrItemMem->m_networkAssignedID,svrItemMem->m_networkAssignedID, "net_id"))
	      if(RemoveIndexed(svrItemMem->m_world,svrItemMem->m_networkAssignedID, "channel"))
	       if(RemoveIndexed(svrItemMem->m_spawnID,svrItemMem->m_networkAssignedID,"spawn_id"))
		      res = TRUE;
		 
		 if(svrItemMem->m_refSpawnId > -1)
		    if(!RemoveIndexed(svrItemMem->m_refSpawnId,svrItemMem->m_networkAssignedID,"refspawn_id"))//partial
			    res = FALSE;
			    

		 delete svrItemMem;
		 svrItemMem = 0;
		
		 return res;
   }

   BOOL DeleteByID_safe(int netID)
   {
	     Lock();
         CServerItemObj * svrItemLocked = GetObjectByID(netID);
		 if(!svrItemLocked)
		 {
             Unlock();
			 return FALSE;
		 }
		 BOOL res = FALSE;
			 
		 if(RemoveIndexed(svrItemLocked->m_networkAssignedID,svrItemLocked->m_networkAssignedID, "net_id"))
	      if(RemoveIndexed(svrItemLocked->m_world,svrItemLocked->m_networkAssignedID, "channel"))
	       if(RemoveIndexed(svrItemLocked->m_spawnID,svrItemLocked->m_networkAssignedID,"spawn_id"))
			  res = TRUE;

		 if(svrItemLocked->m_refSpawnId > -1)
		    if(!RemoveIndexed(svrItemLocked->m_refSpawnId,svrItemLocked->m_networkAssignedID,"refspawn_id"))//partial
			    res = FALSE;

		 Unlock();

		 delete svrItemLocked;
		 svrItemLocked = 0;

		 return res;
   }

   BOOL DeleteByID(int netID)
   {
         CServerItemObj * svrItemLocked = GetObjectByID(netID);
		 if(!svrItemLocked)
		 {
             Unlock();
			 return FALSE;
		 }
		 BOOL res = FALSE;
			 
		 if(RemoveIndexed(svrItemLocked->m_networkAssignedID,svrItemLocked->m_networkAssignedID, "net_id"))
	      if(RemoveIndexed(svrItemLocked->m_world,svrItemLocked->m_networkAssignedID, "channel"))
	       if(RemoveIndexed(svrItemLocked->m_spawnID,svrItemLocked->m_networkAssignedID,"spawn_id"))
			  res = TRUE;

		 if(svrItemLocked->m_refSpawnId > -1)
		    if(!RemoveIndexed(svrItemLocked->m_refSpawnId,svrItemLocked->m_networkAssignedID,"refspawn_id"))//partial
			    res = FALSE;

		 delete svrItemLocked;
		 svrItemLocked = 0;

		 return res;
   }

   BOOL DeleteAllOnChannel_dontNotify_safe(int channel,int * removedCount)
   {
         if(channel < 0)
			 return FALSE;//should assert when used

		 CAxList * obj = GetAllOnIndex(channel,"channel");
		
		 BOOL res = TRUE;
		 int removedTrack = 0;
		 Lock();

         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CServerItemObj * itemMem = (CServerItemObj *)obj->GetNext_unsafe(&pos);
			 if(itemMem->m_world == channel)
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

   

   CServerItemObj * GetObjectByID_LOS(int netID)
   {
		 CAxList * obj = GetAllOnIndex(netID,"net_id");

		 Lock();
         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CServerItemObj * plObj = (CServerItemObj *)obj->GetNext_unsafe(&pos);
             if(plObj->m_networkAssignedID==netID)
			    return plObj;
		 }
		 Unlock();
	     return NULL;
   }

   CServerItemObj * GetObjectBySpawnID_LOS(int spawnID)
   {
		 CAxList * obj = GetAllOnIndex(spawnID,"spawn_id");

		 Lock();
         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CServerItemObj * plObj = (CServerItemObj *)obj->GetNext_unsafe(&pos);
             if(plObj->m_spawnID==spawnID)
			    return plObj;
		 }
		 Unlock();
	     return NULL;
   }

   CServerItemObj * GetObjectByRefSpawnID_LOS(int refSpawnID)
   {
		 CAxList * obj = GetAllOnIndex(refSpawnID,"refspawn_id");

		 Lock();
         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CServerItemObj * plObj = (CServerItemObj *)obj->GetNext_unsafe(&pos);
             if(plObj->m_refSpawnId==refSpawnID)
			    return plObj;
		 }
		 Unlock();
	     return NULL;
   }

   CServerItemObj * GetObjectByID(int netID)
   {
		 CAxList * obj = GetAllOnIndex(netID,"net_id");
		 if(!obj)
		    return NULL;

         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CServerItemObj * plObj = (CServerItemObj *)obj->GetNext_unsafe(&pos);
             if(plObj->m_networkAssignedID==netID)
			    return plObj;
		 }
	     return NULL;
   }

   BOOL VerifyValidByNetID_safe(int netID)
   {
		 CAxList * obj = GetAllOnIndex(netID,"net_id");

		 Lock();
         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CServerItemObj * plObj = (CServerItemObj *)obj->GetNext_unsafe(&pos);
             if(plObj->m_networkAssignedID==netID)
			 {
			    Unlock();
			    return TRUE;
			 }
		 }
		 Unlock();
	     return FALSE;
   }

   CServerItemObj * GetObjectBySpawnID(int spawnID)
   {
		 CAxList * obj = GetAllOnIndex(spawnID,"spawn_id");
		 if(!obj)
		    return NULL;

         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CServerItemObj * plObj = (CServerItemObj *)obj->GetNext_unsafe(&pos);
             if(plObj->m_spawnID==spawnID)
			    return plObj;
		 }
	     return NULL;
   }

  

   BOOL DeleteByRefSpawnID_safe(int refSpawnID)
   {
         CServerItemObj * svrItemLocked = GetObjectByRefSpawnID_LOS(refSpawnID);
		 if(!svrItemLocked)
			 return FALSE;
		 BOOL res = FALSE;
			 
		 if(RemoveIndexed(svrItemLocked->m_networkAssignedID,svrItemLocked->m_networkAssignedID, "net_id"))
	      if(RemoveIndexed(svrItemLocked->m_world,svrItemLocked->m_networkAssignedID, "channel"))
	       if(RemoveIndexed_unsafe(svrItemLocked->m_spawnID,svrItemLocked->m_networkAssignedID,"spawn_id"))//already locked from call above
			  res = TRUE;

		 if(svrItemLocked->m_refSpawnId > -1)
		    if(!RemoveIndexed(svrItemLocked->m_refSpawnId,svrItemLocked->m_networkAssignedID,"refspawn_id"))//partial
			    res = FALSE;

		 Unlock();

		 delete svrItemLocked;
		 svrItemLocked = 0;

		 return res;
   }

   int DeleteAllByRefSpawnID(int refSpawnID)
   {
       int removedCount = 0;
       while(DeleteByRefSpawnID_safe(refSpawnID))
	   { 
		   removedCount++;
	   } 
	   return removedCount;
   }

 
   CServerItemObj * GetItemInRadius_LOSchannel(D3DVECTOR entPos,float radius, int worldChannel);
   

};
