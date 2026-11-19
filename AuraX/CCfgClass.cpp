#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CCfgClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCfgObj::CCfgObj()
{
       m_globalCFG = -1;
	   m_armed     = FALSE;
	   m_quanity   = 1;

}

void CCfgObj::Clone(CCfgObj ** clone)
{
     CCfgObj * copyLocal = new CCfgObj();

     copyLocal->m_globalCFG   = m_globalCFG;
     copyLocal->m_armed       = m_armed;
	 copyLocal->m_quanity     = m_quanity;
	 
	 *clone = copyLocal;
}


void CCfgObjList::Clone(CCfgObjList ** clone)
{
     CCfgObjList * copyLocal = new CCfgObjList();
     for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CCfgObj * bnPtr = (CCfgObj *)GetNext(posLoc);
        CCfgObj * newObject = NULL; 
		bnPtr->Clone(&newObject);
		copyLocal->AddTail(newObject);
	 }
     *clone = copyLocal;
}


void CCfgObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CCfgObj * spnPtr = (CCfgObj *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}


void CCfgObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_globalCFG
		  << m_armed
		  << m_quanity;
	 
	}
	else
	{
       ar >> m_globalCFG
		  >> m_armed
		  >> m_quanity;
	   
	}
}     


IMPLEMENT_SERIAL(CCfgObj, CObject, 0)
IMPLEMENT_SERIAL(CCfgObjList, CObList, 0)

