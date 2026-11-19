#include "CAxList.h"
#include "dbconnectionsystem.h"
#pragma once

enum WorldRules
{
	WORLDRULE_NOHEAL = 0,
	WORLDRULE_NOHIDE,
	WORLDRULE_NODROP,
	WORLDRULE_NOMURDERCOUNT,
	WORLDRULE_NOLEVELRESTRICTIONS,
	WORLDRULE_ARENA_ADDPOINTSPERKILL,
    WORLDRULE_HOSTILEFLAGREQUIRED,
	WORLDRULE_TIMELIMIT,//uses param 1 for duration
	WORLDRULE_ELIMINATION,
	WORLDRULE_TEAM_ELIMINATION,
    WORLDRULE_RUNSCRIPTONCLOSE,   //references param 1 for what server script to exec
	WORLDRULE_RUNSCRIPTONOPEN,
	WORLDRULE_ELIMINATEMOBS_ARENA,
	WORLDRULE_FILTERTEAMFIRE,
	WORLDRULE_DEATHMATCH,
	WORLDRULE_TEAM_DEATHMATCH,
	WORLDRULE_DOMINATION,
	WORLDRULE_TEAM_DOMINATION,
	WORLDRULE_SCORE_LIMIT,
	WORLDRULE_NODISARM,
	WORLDRULE_FILTERPCDMG,
	WORLDRULE_FILTERALLDMG,
	WORLDRULE_INSTANTKILL,
	WORLDRULE_SPAWN_INVUNERABLE,
	WORLDRULE_HIDE_INVENTORY_ON_SPAWN,
	WORLDRULE_THREE_HIT_KILL,
	WORLDRULE_NOHEALTHREGEN
};

class CWordlChannelRuleObj
{  
  public:
    CWordlChannelRuleObj();
	~CWordlChannelRuleObj(){};
	
	CString                    m_ruleName;
    int                        m_rule;
	int                        m_wldID;
	int						   m_primaryKey;
	DWORD                      m_param1;
};
 
class CWorldChannelRuleList : public CAxDB
{
  public:
    CWorldChannelRuleList()
	{
	   openKeyCreep = 1;
       Init(2000,3);//determines memory commit and speed of query
       AddIndexVariable(0,"primaryKey");//m_primaryKey unique
       AddIndexVariable(1,"rule");//m_rule
       AddIndexVariable(2,"wldID");//m_wldID
	};
	~CWorldChannelRuleList()
    {
      //traverse list manually to delete core class objects, the list will clear itself when deconstructed
	  Lock();
	  if(m_indexedVarCount > 0)//only do one list because core objects are referenced between different search criteria-so they only have to be deleted from one list
	     for(unsigned int cLoop=0;cLoop<m_cellCount;cLoop++)
	     {
             for(CAxLinkObj * pos = m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
			 {
		         CWordlChannelRuleObj * delPtr = (CWordlChannelRuleObj *) m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
				 delete delPtr;
				 delPtr = 0;
			 }
		 }
	  //internally the rest of the lsists should auto clean themselfs
	  Unlock();
    };

	int openKeyCreep;

	bool Add_safe(CWordlChannelRuleObj * ruleMem)
	{
         if(!ruleMem)//assert on add index-return false here
             return false;

		 Lock();
         if(AddIndexed((void*)ruleMem, ruleMem->m_primaryKey, ruleMem->m_primaryKey,"primaryKey"))
			if(AddIndexed((void*)ruleMem, ruleMem->m_rule, ruleMem->m_primaryKey,"rule"))
		        if(AddIndexed((void*)ruleMem, ruleMem->m_wldID, ruleMem->m_primaryKey,"wldID"))
				{
					Unlock();
					return true;
				}
	     Unlock();
		 return false;
	}

	BOOL DeleteByOject(CWordlChannelRuleObj * ruleMem)
    {
         if(!ruleMem)
			 return FALSE;//should assert when used

		 BOOL res = FALSE;
		 if(RemoveIndexed(ruleMem->m_primaryKey,ruleMem->m_primaryKey, "primaryKey"))
			if(RemoveIndexed(ruleMem->m_rule,ruleMem->m_primaryKey, "rule"))
			   if(RemoveIndexed(ruleMem->m_wldID,ruleMem->m_primaryKey,"wldID"))
			         res = TRUE;


		 delete ruleMem;
		 ruleMem = 0;

		 return res;
	}

	BOOL DoesKeyExist(int & key)
	{
         if(this->GetByUniqueID(key,key,"primaryKey"))
		    return TRUE;

		 return FALSE;
	}

	int GetUniqueKey()
	{
		while(DoesKeyExist(openKeyCreep))
		{
			if(openKeyCreep > 9000000)
			   openKeyCreep = 0;

			openKeyCreep++;
		}

		return openKeyCreep;
	}

	BOOL DeleteAllOnChannel_safe(int channel,int * removedCount)
	{
         if(channel < 0)
			 return FALSE;//should assert when used

		 CAxList * obj = GetAllOnIndex(channel,"wldID");
		 if(!obj)
		    return NULL;

		 BOOL res = TRUE;
		 int removedTrack = 0;
		 Lock();
         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CWordlChannelRuleObj * itemMem = (CWordlChannelRuleObj *)obj->GetNext_unsafe(&pos);
			 if(itemMem->m_wldID == channel)
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

    //vars
	BOOL DoesThisRuleExistInThisChannel_safe(int & stencilChannel,int & dynamicChannel, int searchRule, DWORD * param1);
	BOOL DoesThisRuleExistInThisChannel_safe(int & stencilChannel,int & dynamicChannel, int searchRule);
	BOOL GetFromSQL(CServerEventList *eventLog);
};
