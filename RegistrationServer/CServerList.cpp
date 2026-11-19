#include "stdafx.h"
#include "resource.h"
#include <afxtempl.h>
#include "CServerList.h"


CServerListObject::CServerListObject()
{
    
}

void CServerListObject::SafeDelete()
{
  
}

void CServerListObject::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	       ar << m_serverName
		      << m_IPadress;
	}
	else
	{
           ar >> m_serverName
		      >> m_IPadress;

	}
}     

void CServerListObjectDB::SafeDelete()
{
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	 {
         CServerListObject * packetPtr = (CServerListObject *)GetNext(posLoc);
         packetPtr->SafeDelete();
		 delete packetPtr;
		 packetPtr = 0;
	 }
	 RemoveAll();
}


IMPLEMENT_SERIAL(CServerListObject, CObject, 0)
IMPLEMENT_SERIAL(CServerListObjectDB, CObList, 0)

