#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CTriggerClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-----------------------------------------------------------//
//-----class is for persisting characters over a network-----//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CTriggerObject::CTriggerObject(int action,
		                       int miscInt,
			                   float radius,
							   CString miscTxt)
{
m_miscTxt      = miscTxt;
m_action       = action;
m_indexMisc    = miscInt;
m_radius       = radius;
m_miscVector.x = 0.0f;
m_miscVector.y = 0.0f;
m_miscVector.z = 0.0f;
m_oneTimeOnly  = FALSE;
m_everActuated = FALSE;
}

//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CTriggerObject::Serialize( CArchive& ar )
{
int reservedINT = 0;
BOOL reservedBOOL = FALSE;
float reservedFloat = 0.0f;
CString reserved;




CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar << m_action 
		  << m_indexMisc 
		  << m_radius 
		  << m_miscVector.x 
		  << m_miscVector.y 
		  << m_miscVector.z
		  << m_oneTimeOnly 
		  << reservedBOOL 
		  << reservedFloat
		  << m_miscTxt;
	}
	else
	{
       ar >> m_action 
		  >> m_indexMisc 
		  >> m_radius 
		  >> m_miscVector.x  
		  >> m_miscVector.y 
		  >> m_miscVector.z
		  >> m_oneTimeOnly 
		  >> reservedBOOL 
		  >> reservedFloat
		  >> m_miscTxt;

	   m_everActuated = FALSE;
	}
}

void CTriggerObjectList::SafeDelete()
{
   for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
   {
       CTriggerObject * triggerObj = (CTriggerObject *)GetNext(posLoc);
       delete triggerObj;
	   triggerObj=0;
   }
   RemoveAll();
}



IMPLEMENT_SERIAL(CTriggerObject, CObject, 0)
IMPLEMENT_SERIAL(CTriggerObjectList, CObList, 0)

