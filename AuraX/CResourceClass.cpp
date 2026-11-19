#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>


#include "CResourceClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CResourceObject::CResourceObject()
{
    m_type = 0;
	m_amount = 0;
	m_resourceName = "NAME";
    m_enableMaxAmountVar = FALSE;
    m_maxAmount = 0;
    m_regenRatePerSecond = 10;
    m_regenEnable = FALSE;
    m_lastUpdatedTime = 0;
}

void CResourceObject::Clone(CResourceObject ** copy)
{
     CResourceObject * copyLocal = new CResourceObject();
     copyLocal->m_type = m_type;
     copyLocal->m_amount = m_amount;
	 copyLocal->m_resourceName = m_resourceName;
	 *copy = copyLocal;
}


void CResourceObject::Serialize( CArchive& ar )
{

CObject::Serialize( ar );
	if(ar.IsStoring() ){
	ar << m_type
       << m_amount
       << m_resourceName
	   << m_enableMaxAmountVar
       << m_maxAmount
       << m_regenRatePerSecond
       << m_regenEnable;
	}
	else
	{
    ar >> m_type
       >> m_amount
       >> m_resourceName
	   >> m_enableMaxAmountVar
       >> m_maxAmount
       >> m_regenRatePerSecond
       >> m_regenEnable;
	
	m_lastUpdatedTime = 0;
	}
}

void CResourceObjectList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CResourceObject * resPtr = (CResourceObject *)GetNext(posLoc);
        delete resPtr;
		resPtr=0;
	}
    RemoveAll();
}


void CResourceObjectList::Clone(CResourceObjectList ** copy)
{
    CResourceObjectList * copyLocal = new CResourceObjectList;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CResourceObject * resPtr = (CResourceObject *)GetNext(posLoc);
        CResourceObject * clonePtr = NULL;
		resPtr->Clone(&clonePtr);
		copyLocal->AddTail(clonePtr);
	}
    *copy = copyLocal;
}
//use functions

void CResourceObjectList::RegenerativeCallback(int currentTickCount)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CResourceObject * resPtr = (CResourceObject *)GetNext(posLoc);
        if(resPtr->m_regenEnable == TRUE){//regeneratable
			if((currentTickCount - resPtr->m_lastUpdatedTime) >= 1000){
                resPtr->m_amount += resPtr->m_regenRatePerSecond;
				if(resPtr->m_amount > resPtr->m_maxAmount)
				   resPtr->m_amount = resPtr->m_maxAmount; 

		           resPtr->m_lastUpdatedTime = currentTickCount;
			}
		}//end regeneratable
	}

}

CResourceObject* CResourceObjectList::GetByType(int type)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CResourceObject * resPtr = (CResourceObject *)GetNext(posLoc);
        if(resPtr->m_type == type){//type match
		   return resPtr;
		}//end match
	}
	return NULL;
}



int CResourceObjectList::AddResource(CString resName, 
									  int type, 
									  int amount,
									  BOOL enableMaxVar,
									  int maxAmount,
									  BOOL regenEnable,
									  int regenRatePersecond
									  )
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CResourceObject * resPtr = (CResourceObject *)GetNext(posLoc);
        if(resPtr->m_type == type){
			if(resPtr->m_enableMaxAmountVar)
			if((resPtr->m_amount + amount) > maxAmount)
            return 2;//no room
			 
		   resPtr->m_amount += amount;
		   return 1;//success
		}
	}
    //if we made it this far it doesnt exist
    CResourceObject * resPtr     = new CResourceObject();
	resPtr->m_amount             = amount;
	resPtr->m_resourceName       = resName;
    resPtr->m_type               = type;
	resPtr->m_enableMaxAmountVar = enableMaxVar;
	resPtr->m_maxAmount          = maxAmount;
    resPtr->m_regenEnable        = regenEnable;
    resPtr->m_regenRatePerSecond = regenRatePersecond;
    AddTail(resPtr);
	return 1;
}

BOOL CResourceObjectList::UseResource(int type, int amount)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CResourceObject * resPtr = (CResourceObject *)GetNext(posLoc);
        if(resPtr->m_type == type && amount <= resPtr->m_amount){
           resPtr->m_amount -= amount;
		   return TRUE;
		}
	}
	return FALSE;
}


BOOL CResourceObjectList::VerifyResource(int type, int amount)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CResourceObject * resPtr = (CResourceObject *)GetNext(posLoc);
        if(resPtr->m_type == type && amount <= resPtr->m_amount){
		   return TRUE;
		}
	}
	return FALSE;
}





IMPLEMENT_SERIAL(CResourceObject,          CObject, 0)
IMPLEMENT_SERIAL(CResourceObjectList,      CObList, 0)