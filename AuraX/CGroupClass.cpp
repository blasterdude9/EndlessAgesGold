#include "stdafx.h"

#include <afxtempl.h>
#include <d3d9.h>
#include "CGroupClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//class is basically used for popout
CGroupObj::CGroupObj(
		             float         L_groupRadius,
			         D3DVECTOR     L_groupCenter,
                     BOOL          L_valid
		            )
{ 
List_groupRadius = L_groupRadius;
List_groupCenter = L_groupCenter;
List_valid = L_valid;
}

void CGroupObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar << List_groupRadius
		  << List_groupCenter.x
		  << List_groupCenter.y
		  << List_groupCenter.z
		  << TRUE;//List_valid;
	}
	else
	{
       ar >> List_groupRadius
		  >> List_groupCenter.x
		  >> List_groupCenter.y
		  >> List_groupCenter.z
		  >> List_valid;
	}
}

IMPLEMENT_SERIAL(CGroupObj, CObject, 0)
IMPLEMENT_SERIAL(CGroupList, CObList, 0)