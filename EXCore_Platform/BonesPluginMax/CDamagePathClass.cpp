//#include "stdafx.h"
#include "afxwin.h"
#include "afxext.h"
#include "resource.h"
#define D3D_OVERLOADS
//#include <afxtempl.h>
#include "d3d9.h"
#include "CDamagePathClass.h"
 
CDamagePathObj::CDamagePathObj()
{
    m_boneIndex                 = 0;
    m_lastPosition.x            = 0.0f;
	m_lastPosition.y            = 0.0f;
	m_lastPosition.z            = 0.0f;
    m_radius                    = 0.0f;
    m_enableOnAnimationIndex    = 0;
    m_damageValue               = 0;
	m_contactMade               = FALSE;
	m_damageDelay               = 200;
    m_damageDelayStamp          = 0;
	m_damageChannel             = 0;
	m_criteriaForSkillUse       = -1;
	m_damageSpecific            = -1;
}

void CDamagePathObj::Clone(CDamagePathObj ** copy)
{
     CDamagePathObj * copyLocal = new CDamagePathObj();

     copyLocal->m_boneIndex                 = m_boneIndex;
     copyLocal->m_lastPosition.x            = m_lastPosition.x;
  	 copyLocal->m_lastPosition.y            = m_lastPosition.y;
	 copyLocal->m_lastPosition.z            = m_lastPosition.z;
     copyLocal->m_radius                    = m_radius;
     copyLocal->m_enableOnAnimationIndex    = m_enableOnAnimationIndex;
     copyLocal->m_damageValue               = m_damageValue;
	 copyLocal->m_contactMade               = m_contactMade;
	 copyLocal->m_damageDelay               = m_damageDelay;
	 copyLocal->m_damageChannel             = m_damageChannel;
	 copyLocal->m_criteriaForSkillUse       = m_criteriaForSkillUse;
	 copyLocal->m_damageSpecific            = m_damageSpecific;

     *copy = copyLocal;
}

void CDamagePathObj::Serialize( CArchive& ar )
{
BOOL reseved = FALSE;

CObject::Serialize( ar );
	if(ar.IsStoring() ){
	    ar << m_boneIndex
           << m_lastPosition.x
  	       << m_lastPosition.y
	       << m_lastPosition.z
           << m_radius
           << m_enableOnAnimationIndex
           << m_damageValue
	       << m_contactMade
		   << m_damageChannel
		   << m_criteriaForSkillUse
		   << m_damageSpecific;//migrated
	}
	else
	{
        ar >> m_boneIndex
           >> m_lastPosition.x
  	       >> m_lastPosition.y
	       >> m_lastPosition.z
           >> m_radius
           >> m_enableOnAnimationIndex
           >> m_damageValue
	       >> m_contactMade
		   >> m_damageChannel
		   >> m_criteriaForSkillUse
		   >> m_damageSpecific;//migrated

		//m_damageSpecific    = -1;
		m_damageDelay       = 200;
		m_damageDelayStamp  = 0;
		//m_criteriaForSkillUse = -1;
	}
}

void CDamagePathObjList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CDamagePathObj * csObj = (CDamagePathObj *)GetNext(posLoc);
        delete csObj;
		csObj=0;
	}
	RemoveAll();
}

void CDamagePathObjList::Clone(CDamagePathObjList ** copy)
{
	CDamagePathObjList * copyLocal = new CDamagePathObjList;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CDamagePathObj * csObj = (CDamagePathObj *)GetNext(posLoc);
        CDamagePathObj * clone = NULL;
		csObj->Clone(&clone);
        copyLocal->AddTail(clone);
	}
    *copy = copyLocal;
}


IMPLEMENT_SERIAL(CDamagePathObj, CObject, 0)
IMPLEMENT_SERIAL(CDamagePathObjList, CObList, 0)
