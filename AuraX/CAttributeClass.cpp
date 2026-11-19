//object attribute class for animation managment
#include "stdafx.h"

#include <afxtempl.h>
#include "CAttributeClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAttributeObj::CAttributeObj(
		         int L_selfAttributeType,
				 int L_selfAttributeValue,
				 int L_otherAttributeType,
				 int L_otherAttributeValue,
				 int L_otherObjectSelect,
				 BOOL L_vicinityBased,
				 BOOL L_autoRetract,
				 BOOL L_inUse,
				 int L_retractDelay,
				 int L_retractDelayVarial
		  	     )
{
List_retractDelay =  L_retractDelay;
List_retractDelayVarial = L_retractDelayVarial;
List_autoRetract = L_autoRetract;
List_inUse = L_inUse;
List_vicinityBased = L_vicinityBased;
List_selfAttributeType = L_selfAttributeType;
List_selfAttributeValue = L_selfAttributeValue;
List_otherAttributeType = L_otherAttributeType;
List_otherAttributeValue = L_otherAttributeValue;
List_otherObjectSelect = L_otherObjectSelect;
}

void CAttributeObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
       
		ar << List_selfAttributeType 
           << List_selfAttributeValue
           << List_otherAttributeType
           << List_otherAttributeValue
           << List_otherObjectSelect
		   << List_vicinityBased
		   << List_autoRetract
		   << List_retractDelay;
	}
	else
	{
        ar >> List_selfAttributeType 
           >> List_selfAttributeValue
           >> List_otherAttributeType
           >> List_otherAttributeValue
           >> List_otherObjectSelect
		   >> List_vicinityBased
		   >> List_autoRetract
		   >> List_retractDelay;
	
		List_retractDelayVarial = 0;
		List_inUse=FALSE;
	}
}

IMPLEMENT_SERIAL(CAttributeObj, CObject, 0)
IMPLEMENT_SERIAL(CAttributeObjList, CObList, 0)