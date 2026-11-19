#include "stdafx.h"

#include <afxtempl.h>

#include "CBlockIpListClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CBlockIPObj::CBlockIPObj()
{

}


void CBlockIPList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CBlockIPObj * spnPtr = (CBlockIPObj *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

BOOL CBlockIPList::IsThisIPBlocked(CString ip)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CBlockIPObj * spnPtr = (CBlockIPObj *)GetNext(posLoc);
		if(spnPtr->m_ipAdress == ip)
		{
 

           return TRUE;//is blocked
		}
	}
	return FALSE;//is not blocked
}

void CBlockIPList::AddIpToBlockList(CString ip)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CBlockIPObj * spnPtr = (CBlockIPObj *)GetNext(posLoc);
		if(spnPtr->m_ipAdress == ip)
		{
 

           return;//already blocked
		}
	}

	CBlockIPObj * spnPtr = new CBlockIPObj();
	spnPtr->m_ipAdress = ip;
    AddTail(spnPtr);

}

void CBlockIPList::UnblockIP(CString ip)
{
	POSITION posLast;
	for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CBlockIPObj * spnPtr = (CBlockIPObj *)GetNext(posLoc);
		if(spnPtr->m_ipAdress == ip)
		{
           delete spnPtr;
		   spnPtr = 0;
           RemoveAt(posLast);
           return;//already blocked
		}
	}
}


void CBlockIPObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_ipAdress;
	}
	else
	{
       ar >> m_ipAdress;
	}
}     

IMPLEMENT_SERIAL(CBlockIPObj, CObject, 0)
IMPLEMENT_SERIAL(CBlockIPList, CObList, 0)

