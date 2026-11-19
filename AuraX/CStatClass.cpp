#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d9.h>
#include <afxtempl.h>
#include "CStatClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CStatObj::CStatObj()
{
    m_statName             = "UNKNOWN";
	m_capValue             = 250.0f;
	m_currentValue         = 50.0f;
	m_id                   = 0;
	m_gainBasis            = .1f;
	m_benefitType          = 0;
}

void CStatObj::Clone(CStatObj ** clone)
{
     CStatObj * copyLocal = new CStatObj();
     copyLocal->m_statName       = m_statName;
	 copyLocal->m_capValue       = m_capValue;
	 copyLocal->m_currentValue   = m_currentValue;
	 copyLocal->m_gainBasis      = m_gainBasis;
     copyLocal->m_id             = m_id;
	 copyLocal->m_benefitType    = m_benefitType;

	 *clone = copyLocal;
}

float CStatObj::GetValueIncludingBonusItems(CInventoryObjList * inventory)
{   
	 float returnValue = m_currentValue;
	 
	 if(inventory)
     for(POSITION posLoc = inventory->GetHeadPosition();posLoc != NULL;)
	 {
         CInventoryObj * invenPtr = (CInventoryObj *) inventory->GetNext(posLoc);
         if(invenPtr->m_armed)
		 if(invenPtr->m_itemData)//valid item data
		 if(invenPtr->m_itemData->m_statBonuses)
		 {
            returnValue += ((float)invenPtr->m_itemData->m_statBonuses->GetBonusByID(m_id));
		 }
	 }

	 return returnValue;
}

 BOOL CStatObj::UseStat(float useLevel,CInventoryObjList * inventory)
{
     
	float virtualCurrentValue = GetValueIncludingBonusItems(inventory);
	if(virtualCurrentValue > m_capValue)
	   return FALSE;


	if(virtualCurrentValue < useLevel)
	   return FALSE;
	//if(m_currentValue < useLevel)
    //   return FALSE;

     int floor = (int)m_currentValue;

	 if(m_currentValue == 0.0f)
		m_currentValue = 1.0f;//increment zeroed stats

	 if(m_currentValue > m_capValue)
		return FALSE;


	 float degrationOfMax = 1.0f - (virtualCurrentValue/m_capValue);

     m_currentValue += (degrationOfMax*(useLevel/(virtualCurrentValue*6) * m_gainBasis));
	 

     int floorCmp = (int)m_currentValue;

     if(floorCmp != floor)//indicate change
	 return TRUE;
	 else
	 return FALSE;
}

//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CStatObjList::Clone(CStatObjList ** clone)
{
     CStatObjList * copyLocal = new CStatObjList();

	 for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CStatObj * skPtr = (CStatObj *)GetNext(posLoc);
		 CStatObj * clonePtr = NULL;
         skPtr->Clone(&clonePtr);
         copyLocal->AddTail(clonePtr);
	 }

	 *clone = copyLocal;
}


void CStatObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CStatObj * spnPtr = (CStatObj *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

float CStatObjList::GetStatValueByID(int id,CInventoryObjList * inventory)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CStatObj * spnPtr = (CStatObj *)GetNext(posLoc);
		if(spnPtr->m_id == id)
		{
	       return spnPtr->GetValueIncludingBonusItems(inventory);
		}
	}
	return -1;
}

int CStatObjList::GetBenefitBonus(int benefit,CInventoryObjList * inventory)
{
	int tally = 0;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CStatObj * spnPtr = (CStatObj *)GetNext(posLoc);
		if(spnPtr->m_benefitType == benefit)
		{
	       tally += (int)spnPtr->GetValueIncludingBonusItems(inventory);
		}
	}
	return tally;
}

BOOL CStatObjList::UseStatByID(float useLevel,int id,CInventoryObjList * inventory)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CStatObj * spnPtr = (CStatObj *)GetNext(posLoc);
		if(spnPtr->m_id == id)
		{
	       return spnPtr->UseStat(useLevel,inventory);
		}
	}
	return FALSE;
}

CStatObj * CStatObjList::GetStatByID(int id)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CStatObj * spnPtr = (CStatObj *)GetNext(posLoc);
		if(spnPtr->m_id == id)
		{
	       return spnPtr;
		}
	}
	return NULL;
}

int CStatObjList::GetStatIdByName(CString name)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CStatObj * spnPtr = (CStatObj *)GetNext(posLoc);
		if(name.CompareNoCase(spnPtr->m_statName)==0)
		{
	       return spnPtr->m_id;
		}
	}
	return -1;
}

BOOL CStatObjList::VerifyStatRequirement(float minimumValue,int id,CInventoryObjList * inventory)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CStatObj * spnPtr = (CStatObj *)GetNext(posLoc);
		if(spnPtr->m_id == id)
		{
		   if(spnPtr->GetValueIncludingBonusItems(inventory) >= minimumValue)
	       return TRUE;
		   else
		   return FALSE;
		}
	}
	return TRUE;
}

void CStatObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_statName 
		  << m_capValue 
		  << m_currentValue 
		  << m_id
		  << m_gainBasis
		  << m_benefitType;
	}
	else
	{
       ar >> m_statName 
		  >> m_capValue 
		  >> m_currentValue 
		  >> m_id
		  >> m_gainBasis
		  >> m_benefitType;

	}
}     

void CStatObj::SerializeAlloc( CArchive& ar )
{
	if(ar.IsStoring() ){	
	   ar << m_statName 
		  << m_capValue 
		  << m_currentValue 
		  << m_id
		  << m_gainBasis
		  << m_benefitType;
	}
	else
	{
       ar >> m_statName 
		  >> m_capValue 
		  >> m_currentValue 
		  >> m_id
		  >> m_gainBasis
		  >> m_benefitType;

	}
}     

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CStatObjList::SerializeAlloc(CArchive& ar,int loadCount)
{
	if(ar.IsStoring() ){	
	   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
           CStatObj * elementPtr = (CStatObj *)GetNext(posLoc);
		   elementPtr->SerializeAlloc(ar);
	   }
	}
	else
	{
       for(int i=0;i<loadCount;i++)
	   {
		   CStatObj * elementPtr = new CStatObj();
           elementPtr->SerializeAlloc(ar);
		   AddTail(elementPtr);
	   }
	}
}


IMPLEMENT_SERIAL(CStatObj, CObject, 0)
IMPLEMENT_SERIAL(CStatObjList, CObList, 0)

