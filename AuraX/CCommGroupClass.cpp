#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d9.h>
#include <afxtempl.h>
#include "CCommGroupClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCommGroupObj::CCommGroupObj()
{
   m_handle = 0;
}

void CCommGroupObj::Clone(CCommGroupObj ** clone)
{
     CCommGroupObj * copyLocal = new CCommGroupObj();

	 copyLocal->m_handle = m_handle;
	 copyLocal->m_name   = m_name;

     *clone = copyLocal;
}

void CCommGroupList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CCommGroupObj * spnPtr = (CCommGroupObj *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

BOOL CCommGroupList::AddIndividualToGroup(int handle,CString name)
{
    
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{//verify he is not in group already
        CCommGroupObj * spnPtr = (CCommGroupObj *)GetNext(posLoc);
	    if(spnPtr->m_handle == handle)
		   return FALSE;

	}//end verify he is not in group already
	
	
	
	CCommGroupObj * spnPtr = new CCommGroupObj();
    spnPtr->m_handle = handle;
    spnPtr->m_name   = name;

	AddTail(spnPtr);
	return TRUE;
}

void CCommGroupList::Clone(CCommGroupList ** clone)
{
	CCommGroupList * copyLocal = new CCommGroupList();
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CCommGroupObj * groupPtr = (CCommGroupObj *)GetNext(posLoc);
        CCommGroupObj * copyPtr = NULL;
		groupPtr->Clone(&copyPtr);
	    copyLocal->AddTail(copyPtr);
	}
	*clone = copyLocal;
}

void CCommGroupList::RemoveByHandle(int handle)
{
    POSITION posLast;
	for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CCommGroupObj * spnPtr = (CCommGroupObj *)GetNext(posLoc);
		if(spnPtr->m_handle == handle)
		{
		   delete spnPtr;
		   spnPtr=0;
		   RemoveAt(posLast);
		   return;
		}
	}
}

void CCommGroupObj::SerializeAlloc( CArchive& ar )
{
	if(ar.IsStoring() ){	
	   ar << m_name << m_handle;
	}
	else
	{
       ar >> m_name >> m_handle;
	}
}     

void CCommGroupObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_name << m_handle;
	}
	else
	{
       ar >> m_name >> m_handle;
	}
}  

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//------------------------------------------------------------------//
void CCommGroupList::SerializeAlloc(CArchive& ar,int loadCount)
{
	if(ar.IsStoring() ){	
	   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
           CCommGroupObj * elementPtr = (CCommGroupObj *)GetNext(posLoc);
		   elementPtr->SerializeAlloc(ar);
	   }
	}
	else
	{
       for(int i=0;i<loadCount;i++)
	   {
		   CCommGroupObj * elementPtr = new CCommGroupObj();
           elementPtr->SerializeAlloc(ar);
		   AddTail(elementPtr);
	   }
	}
}

IMPLEMENT_SERIAL(CCommGroupObj, CObject, 0)
IMPLEMENT_SERIAL(CCommGroupList, CObList, 0)

