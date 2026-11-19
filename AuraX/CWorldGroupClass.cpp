#include "stdafx.h"
#include "CWorldGroupClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//------------Main object list------------------//
CWldGroupObj::CWldGroupObj(CString groupName,int groupInt)
{
List_groupInt = groupInt;
List_groupName = groupName;
}

void CWldGroupObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar << List_groupInt << List_groupName;
	}
	else
	{
	   ar >> List_groupInt >> List_groupName;
	}
}

//-------------------Deconstructor----------------------------------//
void CWldGroupObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CWldGroupObj * ptRang = (CWldGroupObj *)GetNext(posLoc);
		delete ptRang;
		ptRang = 0;
	}
	RemoveAll();
}

IMPLEMENT_SERIAL(CWldGroupObj, CObject, 0)
IMPLEMENT_SERIAL(CWldGroupObjList, CObList, 0)
