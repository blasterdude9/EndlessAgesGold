#include "stdafx.h"

#include <afxtempl.h>
#include <d3d9.h>
#include "CAITraceClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//CONSTRUCTION
CAITraceObj::CAITraceObj()
{
         m_NameOfLocation = "Default";
		 m_locationCenter.x = 0.0f; 
		 m_locationCenter.y = 0.0f; 
		 m_locationCenter.z = 0.0f;
		 m_toleranceOfLocation = 20.0f;
}

BOOL CAITraceObj::SetData(CString      locationName,
					      D3DVECTOR    position,
					      float        Tolerance)
{
     m_NameOfLocation        = locationName;
     m_locationCenter.x      = position.x; 
     m_locationCenter.y      = position.y; 
	 m_locationCenter.z      = position.z;
	 m_toleranceOfLocation   = Tolerance;
	return TRUE;
}


void CAITraceObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar << m_NameOfLocation 
		  << m_locationCenter.x 
		  << m_locationCenter.y 
		  << m_locationCenter.z 
		  << m_toleranceOfLocation;
	}
	else
	{
       ar >> m_NameOfLocation 
		  >> m_locationCenter.x 
		  >> m_locationCenter.y 
		  >> m_locationCenter.z 
		  >> m_toleranceOfLocation;
	}
}

IMPLEMENT_SERIAL(CAITraceObj, CObject, 0)
IMPLEMENT_SERIAL(CAITraceObjList, CObList, 0)