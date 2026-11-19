#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>

#include "CElementStatusClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CElementStatusObj::CElementStatusObj()
{
   m_elementIndex = 0;//corilates to index zero based
   m_currentValue = 0;//current value
}

void CElementStatusObj::SafeDelete()
{


}

void CElementStatusObj::Clone(CElementStatusObj ** clone)
{
     CElementStatusObj * elmPtr = new CElementStatusObj();
	 elmPtr->m_currentValue = m_currentValue;
	 elmPtr->m_elementIndex = m_elementIndex;

     *clone = elmPtr;
}

int CElementStatusList::GetDefenseForSpecifiedChannel(int damageChannel)
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CElementStatusObj * invObj = (CElementStatusObj *)GetNext(posLoc);
		if(invObj->m_elementIndex == damageChannel)
		   return invObj->m_currentValue;
	 }
	 return 0;
}


void CElementStatusList::Clone(CElementStatusList ** clone)
{
	CElementStatusList * copyLocal = new CElementStatusList();
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
        CElementStatusObj * invObj = (CElementStatusObj *)GetNext(posLoc);
        CElementStatusObj * cpyPtr = NULL;
		invObj->Clone(&cpyPtr);
		copyLocal->AddTail(cpyPtr);
	 }
	 *clone = copyLocal;
}

void CElementStatusList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CElementStatusObj * elementPtr = (CElementStatusObj *)GetNext(posLoc);
		elementPtr->SafeDelete();
		delete elementPtr;
		elementPtr=0;
	}
	RemoveAll();
}

void CElementStatusObj::SerializeAlloc(CArchive& ar)
{
	if(ar.IsStoring() ){	
	   ar << m_currentValue
		  << m_elementIndex;
	}
	else
	{
       ar >> m_currentValue
		  >> m_elementIndex;
	}
}

void CElementStatusList::SerializeAlloc(CArchive& ar,int loadCount)
{
	if(ar.IsStoring() ){	
	   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
           CElementStatusObj * elementPtr = (CElementStatusObj *)GetNext(posLoc);
		   elementPtr->SerializeAlloc(ar);
	   }
	}
	else
	{
       for(int i=0;i<loadCount;i++)
	   {
		   CElementStatusObj * elementPtr = new CElementStatusObj();
           elementPtr->SerializeAlloc(ar);
	   }
	}
}

void CElementStatusObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_currentValue
		  << m_elementIndex;
	}
	else
	{
       ar >> m_currentValue
		  >> m_elementIndex;
	}
}     


IMPLEMENT_SERIAL(CElementStatusObj, CObject, 0)
IMPLEMENT_SERIAL(CElementStatusList, CObList, 0)

