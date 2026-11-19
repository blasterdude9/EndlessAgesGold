#include "stdafx.h"
#define D3D_OVERLOADS
#include "resource.h"
#include <d3d8.h>
#include <afxtempl.h>
#include "CClientObject.h"

CClientObj::CClientObj()
{
    m_dpnidCom                 = 0;
    m_packetBuild              = NULL;
    m_finalPacketSize          = 0;
    m_currentAmountDownloaded  = 0;
}


void CClientObj::SafeDelete()
{
    if(m_packetBuild){
       delete [] m_packetBuild;
	   m_packetBuild=0;
	}
}

void CClientObjList::LogOut(DPNID dpnid)
{
	POSITION posLast;
     for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	 {
         CClientObj * cPtr = (CClientObj *)GetNext(posLoc);
         if(cPtr->m_dpnidCom == dpnid)
		 {	 
            cPtr->SafeDelete();
			delete cPtr;
			cPtr = 0;
            RemoveAt(posLast);
			return;
		 }
	 }
}

CClientObj * CClientObjList::GetObjByDPNID(DPNID dpnid)
{
     POSITION posLast;
     for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;)
	 {
         CClientObj * cPtr = (CClientObj *)GetNext(posLoc);
         if(cPtr->m_dpnidCom == dpnid)
		 {	 
			return cPtr;
		 }
	 }
	 return NULL;
}



void CClientObjList::SafeDelete()
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
         CClientObj * cPtr = (CClientObj *)GetNext(posLoc);
         cPtr->SafeDelete();
		 delete cPtr;
		 cPtr = 0;
	 }
	 RemoveAll();
}

