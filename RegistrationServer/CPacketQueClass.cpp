#include "stdafx.h"
#define D3D_OVERLOADS
#include "resource.h"
#include <d3d8.h>
//#include "CStructuresMisl.h"
#include <afxtempl.h>



#include "CPacketQueClass.h"

CPacketObj::CPacketObj()
{
       m_data            = NULL;
	   m_size            = 0;
	   m_communicationID = 0;
}


void CPacketObj::SafeDelete()
{
    if(m_data){
       delete [] m_data;
	   m_data=0;
	}
}


void CPacketObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_size
		  << m_communicationID;
	      for(int loop=0;loop<m_size;loop++)
			  ar << m_data[loop];
	}
	else
	{
       ar >> m_size
		  >> m_communicationID;
	          m_data = new BYTE[m_size];
	      for(int loop=0;loop<m_size;loop++)
			  ar >> m_data[loop];
	}
}     

void CPacketObjList::AddPacket(int size,BYTE * data,DPNID id)
{
 	 CPacketObj * packetPtr = new CPacketObj();
     packetPtr->m_size = size;  
	 packetPtr->m_data = new BYTE[size];
	 packetPtr->m_communicationID = id;
     CopyMemory(packetPtr->m_data,data,size);
     AddTail(packetPtr);
}


void CPacketObjList::SafeDelete()
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
         CPacketObj * packetPtr = (CPacketObj *)GetNext(posLoc);
         packetPtr->SafeDelete();
		 delete packetPtr;
		 packetPtr = 0;
	 }
	 RemoveAll();
}


IMPLEMENT_SERIAL(CPacketObj, CObject, 0)
IMPLEMENT_SERIAL(CPacketObjList, CObList, 0)

