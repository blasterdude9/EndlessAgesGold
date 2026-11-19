#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d9.h>
#include <afxtempl.h>
#include "CNoterietyClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CNoterietyObj::CNoterietyObj()
{
    m_currentMurders					= 0;
	m_currentMinuteCountSinceLastMurder = 0;
	m_murderUpdateTick					= GetTickCount();
    m_currentAgressors					= NULL;
	
	
}



void CNoterietyObj::Clone(CNoterietyObj ** clone)
{
     CNoterietyObj * copyLocal = new CNoterietyObj();
     copyLocal->m_currentMurders = m_currentMurders;
	 copyLocal->m_currentMinuteCountSinceLastMurder = m_currentMinuteCountSinceLastMurder;
   
	 if(m_currentAgressors)
	    m_currentAgressors->Clone(&copyLocal->m_currentAgressors);

     *clone = copyLocal;
}

void CNoterietyObj::SerializeAlloc( CArchive& ar )
{
	if(ar.IsStoring() ){	
	   ar << m_currentMurders
		  << m_currentMinuteCountSinceLastMurder;
	}
	else
	{
       ar >> m_currentMurders
		  >> m_currentMinuteCountSinceLastMurder;
	}
}    

void CNoterietyObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_currentMurders
		  << m_currentMinuteCountSinceLastMurder;
	}
	else
	{
       ar >> m_currentMurders
		  >> m_currentMinuteCountSinceLastMurder;
	}
}    

BOOL CAggressorObjList::LootDamageBasedAccessCheck(int looterHandleOrGroup)
{//group will be negative handles will be positive

	int handleThatDidMostDamage = 0;
	int mostDamageDone = -1;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CAggressorObj * aPtr = (CAggressorObj *)GetNext(posLoc);
		if(aPtr->m_curDamageDone > mostDamageDone)
		{
		   mostDamageDone				= aPtr->m_curDamageDone;
		   handleThatDidMostDamage		= aPtr->m_handle;
		}
	}
    if(handleThatDidMostDamage == looterHandleOrGroup)
	   return TRUE;
    return FALSE;
}

BOOL CAggressorObjList::UpdateAgressorState(int handle, int damageAmount, int & channel)
{//group will be negative handles will be positive
	DWORD currentTime = GetTickCount();
	POSITION posLast;
	int count = 0;
     for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CAggressorObj * aPtr = (CAggressorObj *)GetNext(posLoc);
		if(aPtr->m_handle == handle)
		{
		   aPtr->m_curDamageDone	+= damageAmount;
		   aPtr->m_timeStamp		= currentTime;

           return TRUE;
		}
		else if((currentTime - aPtr->m_timeStamp) > m_agressorTimeout)
		{//remove agressor
		   delete aPtr;
		   aPtr = 0;
		   RemoveAt(posLast);
		}//end removeagressor
		count++;
	}
    if(count > m_maxAgressors)
	   return FALSE;

    CAggressorObj * newAgressor = new CAggressorObj();
    newAgressor->m_handle			    = handle;
    newAgressor->m_timeStamp			= GetTickCount();
    newAgressor->m_wldID				= channel;
	newAgressor->m_curDamageDone		= damageAmount;
    AddTail(newAgressor);
    return TRUE;
}

BOOL CAggressorObjList::DoesHandleExist(int & handle,long timeout)
{
   long currentTime = GetTickCount();
   POSITION posLast;
	for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CAggressorObj * aPtr = (CAggressorObj *)GetNext(posLoc);	
		if((currentTime - aPtr->m_timeStamp) > timeout)
		{//remove agressor
		   delete aPtr;
		   aPtr = 0;
		   RemoveAt(posLast);
		}//end removeagressor
		else if(aPtr->m_handle == handle)
		{
           return TRUE;
		}
	}
	return FALSE;
} 

void CAggressorObjList::CacheTimedOutAgressors(long timeout)
{
   long currentTime = GetTickCount();

   POSITION posLast;
	for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CAggressorObj * aPtr = (CAggressorObj *)GetNext(posLoc);
		if((currentTime - aPtr->m_timeStamp) > timeout)
		{//remove agressor
		   delete aPtr;
		   aPtr = 0;
		   RemoveAt(posLast);
		}//end removeagressor
	}
} 

void CAggressorObjList::FilterByWldID(int id)
{
   POSITION posLast;
	for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CAggressorObj * aPtr = (CAggressorObj *)GetNext(posLoc);
		if(aPtr->m_wldID == id)
		{//remove agressor
		   delete aPtr;
		   aPtr = 0;
		   RemoveAt(posLast);
		}//end removeagressor
	}
} 

CAggressorObj::CAggressorObj()
{
     m_handle				= 0;
     m_timeStamp			= 0;
	 m_wldID				= 0;
	 m_curDamageDone		= 0;
}

void CAggressorObj::Clone(CAggressorObj ** clone)
{
    CAggressorObj * copyLocal = new CAggressorObj();
    copyLocal->m_handle = m_handle;
	copyLocal->m_timeStamp = m_timeStamp;
	copyLocal->m_wldID = m_wldID;
   
    *clone = copyLocal;
}

void CAggressorObjList::Clone(CAggressorObjList ** clone)
{
   CAggressorObjList * newList = new CAggressorObjList();
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CAggressorObj * bnPtr = (CAggressorObj *)GetNext(posLoc);
        CAggressorObj * newObject = NULL;  
		bnPtr->Clone(&newObject);
		newList->AddTail(newObject);
	}
    *clone = newList;
} 


IMPLEMENT_SERIAL(CNoterietyObj, CObject, 0)

