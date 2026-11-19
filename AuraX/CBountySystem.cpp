#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CBountySystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBountyObj::CBountyObj()
{
    m_bountyAmount = 0;
    m_playerHandle = 0;
}


void CBountyObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CBountyObj * spnPtr = (CBountyObj *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}


void CBountyObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_playerName
		  << m_bountyAmount
		  << m_playerHandle;
	}
	else
	{
       ar >> m_playerName
		  >> m_bountyAmount
		  >> m_playerHandle;
	}
}  

int CBountyObjList::AddBounty(CString & playerName, 
							   int & bountyAmount,
							   int & playerHandle,
							   int maxBountyCount)
{
     POSITION posLast,posLoc;
     for(posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	 {
         CBountyObj * bPtr = (CBountyObj *) GetNext(posLoc);

		 if(playerHandle == bPtr->m_playerHandle)
		 {
             bPtr->m_bountyAmount = bountyAmount;
			 return 2;//dont add twice just update him or her
		 }
	 }


	 BOOL added = FALSE;
	 for(posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	 {
         CBountyObj * bPtr = (CBountyObj *) GetNext(posLoc);

		 if(bountyAmount > bPtr->m_bountyAmount)
		 {
            CBountyObj * bountyPtr = new CBountyObj();
	        bountyPtr->m_playerName = playerName;
	        bountyPtr->m_bountyAmount = bountyAmount;
	        bountyPtr->m_playerHandle = playerHandle;
            InsertBefore(posLast,bountyPtr);
			added = TRUE;
			break;
		 }
	 }

	 if(!added)
	 {
		 CBountyObj * bountyPtr = new CBountyObj();
	     bountyPtr->m_playerName = playerName;
	     bountyPtr->m_bountyAmount = bountyAmount;
	     bountyPtr->m_playerHandle = playerHandle;
		 AddTail(bountyPtr);
	 }
	
	 int trace = 0;
	 for(posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	 {
         CBountyObj * bPtr = (CBountyObj *) GetNext(posLoc);
         if(trace > maxBountyCount)
		 {
            delete bPtr;
			bPtr = 0;
            RemoveAt(posLast);
		 }
		
		 trace++;
	 }

	 return 1;
}

void CBountyObjList::RemoveBountyByHandle(int mHandle)
{
	POSITION posLast;
    for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	 {
         CBountyObj * bPtr = (CBountyObj *) GetNext(posLoc);
         if(mHandle == bPtr->m_playerHandle)
		 {
            delete bPtr;
			bPtr = 0;
            RemoveAt(posLast);
			return;
		 }
	 }
}

IMPLEMENT_SERIAL(CBountyObj, CObject, 0)
IMPLEMENT_SERIAL(CBountyObjList, CObList, 0)

