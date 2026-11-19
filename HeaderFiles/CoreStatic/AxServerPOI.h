#include "CAxList.h"
#include "d3d9.h"

#pragma once

class CAxServerPOI
{
public:
	CAxServerPOI(void);
	~CAxServerPOI(void);

	//variables
	int			m_uniqueID;
	int			m_channel;
	float		m_radius;
	D3DVECTOR	m_position;
};

class CAxServerPOISystem : public CAxDB
{
public:
	CAxServerPOISystem(void)
	{
		seed = 565;
		m_newIDCreep = 1;
		Init(1000,2);//determines memory commit and speed of query
		AddIndexVariable(0,"id");//m_currentWorldID-non unique
		AddIndexVariable(1,"channel");//m_currentWorldID-non unique
	};
	~CAxServerPOISystem(void)
	{
	  Lock();
	  this->WriteStatus("cleaned_CAxServerPOISystem.txt");
	  if(m_indexedVarCount > 0)//only do one list because core objects are referenced between different search criteria-so they only have to be deleted from one list
	     for(unsigned int cLoop=0;cLoop<m_cellCount;cLoop++)
	     {
             for(CAxLinkObj * pos = m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
			 {
		         CAxServerPOI * delPtr = (CAxServerPOI *) m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
				 delete delPtr;
				 delPtr = 0;
			 }
		 }
	  //internally the rest of the lsists should auto clean themselfs
	  Unlock();
	};

	//variables
	int m_newIDCreep;
	DWORD seed;

	//Functions
    BOOL Add_safe(CAxServerPOI * poiMem)
	{
		if(!poiMem)//assert on add index-return false here
			return FALSE;

		Lock();
		if(AddIndexed((void*)poiMem, poiMem->m_uniqueID, poiMem->m_uniqueID,"id"))
			if(AddIndexed((void*)poiMem, poiMem->m_channel, poiMem->m_uniqueID,"channel"))
			{
				Unlock();
				return TRUE;
			}
		Unlock();
		return FALSE;
	}

	//Functions
    BOOL Add(CAxServerPOI * poiMem)
	{
		if(!poiMem)//assert on add index-return false here
			return FALSE;

		if(AddIndexed((void*)poiMem, poiMem->m_uniqueID, poiMem->m_uniqueID,"id"))
			if(AddIndexed((void*)poiMem, poiMem->m_channel, poiMem->m_uniqueID,"channel"))
			{
				return TRUE;
			}
		return FALSE;
	}

	BOOL Add_safe(int channel, float radius, D3DVECTOR position)
	{
		CAxServerPOI * poiMem = new CAxServerPOI();
		poiMem->m_channel = channel;
		poiMem->m_radius = radius;
		CopyMemory(&poiMem->m_position,&position,sizeof(D3DVECTOR));
	    Lock();
			poiMem->m_uniqueID = GetNewUniqueID();
			Add(poiMem);
		Unlock();
		return FALSE;
	}

	BOOL Get3DPositionInOrderInChannel_safe(int channel,int index,D3DVECTOR * retPosition)
	{
		CAxList * obj = GetAllOnIndex(channel,"channel");
		int trace = 0;
		Lock();
		for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		{
             CAxServerPOI * plObj = (CAxServerPOI *)obj->GetNext_unsafe(&pos);
             if(channel == plObj->m_channel)
			 {
			    if(trace == index)
				{
                    *retPosition = plObj->m_position;
					Unlock();
					return TRUE;
				}
			    trace++;
			 }
		}
		Unlock();
		return FALSE;
	}

	BOOL Get3DPositionRandomInChannel_safe(int channel,D3DVECTOR * retPosition)
	{
		CAxList * obj = GetAllOnIndex(channel,"channel");
		
		srand(GetTickCount()+(seed+=10));
		if(seed > 9000000)
		   seed = 565;
		Lock();
		int poiCount = 0;//manually loop so you can check the channel, get count could have stacked ptrs that are indexed with other indexes due to lack of count
		int index = 0;
		for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		{
             CAxServerPOI * plObj = (CAxServerPOI *)obj->GetNext_unsafe(&pos);
             if(channel == plObj->m_channel)//verify hash cell
				poiCount++;
		}

		if(poiCount == 0)
		{
			Unlock();
		    return FALSE;
		}

		if(poiCount > 1)
			index = rand()%(obj->GetCount()-1);

		int trace = 0;
		for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		{
             CAxServerPOI * plObj = (CAxServerPOI *)obj->GetNext_unsafe(&pos);
             if(channel == plObj->m_channel)
			 {
			    if(trace == index)
				{
                    *retPosition = plObj->m_position;
					Unlock();
					return TRUE;
				}
			    trace++;
			 }
		}
		Unlock();
		return FALSE;
	}



	BOOL DeleteByOject(CAxServerPOI * poiMem)
    {
         if(!poiMem)
			 return FALSE;//should assert when used

		 BOOL res = FALSE;
		 if(RemoveIndexed(poiMem->m_uniqueID,poiMem->m_uniqueID, "id"))
			if(RemoveIndexed(poiMem->m_channel,poiMem->m_uniqueID, "channel"))
			   res = TRUE;


		 delete poiMem;
		 poiMem = 0;

		 return res;
	}

	BOOL VerifyIdExists(int & id)
	{
        CAxList * obj = GetAllOnIndex(id,"id");
        if(obj->GetCount() > 0)
		   return TRUE;
		return FALSE;
	}

	int GetNewUniqueID()
	{
		while(VerifyIdExists(m_newIDCreep))
		{
              m_newIDCreep++;
			  if(m_newIDCreep > 9999999)
			     m_newIDCreep = 1;
		}
		return m_newIDCreep;
	}

	BOOL DeleteAllOnChannel_safe(int channel,int * removedCount)
	{
         if(channel < 0)
			 return FALSE;//should assert when used

		 CAxList * obj = GetAllOnIndex(channel,"channel");

		 BOOL res = TRUE;
		 int removedTrack = 0;
		 Lock();
         for(CAxLinkObj * pos = obj->GetHeadPosition_unsafe();pos != NULL;)
		 {
             CAxServerPOI * itemMem = (CAxServerPOI *)obj->GetNext_unsafe(&pos);
			 if(itemMem->m_channel == channel)
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
	
};
