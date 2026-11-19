#include "stdafx.h"

#include <afxtempl.h>
#include "CEXScriptClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CEXScriptObj::CEXScriptObj(int         L_actionAttribute,
		                   CString     L_miscString,
		                   int         L_miscInt
		  	               )
{
List_actionAttribute = L_actionAttribute;
List_miscString = L_miscString;
List_miscInt = L_miscInt;
}

void CEXScriptObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
		ar << List_actionAttribute 
           << List_miscString
           << List_miscInt;
	}
	else
	{
        ar >> List_actionAttribute 
           >> List_miscString
           >> List_miscInt;
	}
}

//-------------------Deconstructor----------------------------------//
void CEXScriptObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CEXScriptObj * ptRang = (CEXScriptObj *)GetNext(posLoc);
		delete ptRang;
		ptRang = 0;
	}
	RemoveAll();
}

IMPLEMENT_SERIAL(CEXScriptObj, CObject, 0)
IMPLEMENT_SERIAL(CEXScriptObjList, CObList, 0)