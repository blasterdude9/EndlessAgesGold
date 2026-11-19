#include "stdafx.h"
#define D3D_OVERLOADS
#include <afxtempl.h>
#include "d3d9.h"
#include "CDamagePathClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
 
CDamagePathObj::CDamagePathObj()
{
    m_boneIndex                 = 0;
    m_lastPosition.x            = 0.0f;
	m_lastPosition.y            = 0.0f;
	m_lastPosition.z            = 0.0f;
    m_radius                    = 0.0f;
    m_enableOnAnimationIndex    = 0;
    m_damageMinValue            = 0;
	m_damageMaxValue            = 0;
	m_damageDotValue            = 0;
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
     copyLocal->m_damageMinValue            = m_damageMinValue;
	 copyLocal->m_damageMaxValue            = m_damageMaxValue;
	 copyLocal->m_damageDotValue            = m_damageDotValue;
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
	if(ar.IsStoring() )
	{
	    ar.SetObjectSchema(1);
        ASSERT(ar.GetObjectSchema() == 1);
		
		ar << m_boneIndex
           << m_lastPosition.x
  	       << m_lastPosition.y
	       << m_lastPosition.z
           << m_radius
           << m_enableOnAnimationIndex
           << m_damageMinValue
	       << m_contactMade
		   << m_damageChannel
		   << m_criteriaForSkillUse
		   << m_damageSpecific//migrated
		   << m_damageMaxValue
		   << m_damageDotValue;
	}
	else
	{
        int version = ar.GetObjectSchema();
		
		ar >> m_boneIndex
           >> m_lastPosition.x
  	       >> m_lastPosition.y
	       >> m_lastPosition.z
           >> m_radius
           >> m_enableOnAnimationIndex
           >> m_damageMinValue
	       >> m_contactMade
		   >> m_damageChannel
		   >> m_criteriaForSkillUse
		   >> m_damageSpecific;//migrated

		//m_damageSpecific    = -1;
		m_damageDelay       = 200;
		m_damageDelayStamp  = 0;
		
		if(version == 1)
		{
			ar  >> m_damageMaxValue
				>> m_damageDotValue;
		}
		else
		{
			m_damageMaxValue = m_damageMinValue + 5;
			m_damageDotValue = -1;
		}
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


IMPLEMENT_SERIAL(CDamagePathObj, CObject, VERSIONABLE_SCHEMA|1)
IMPLEMENT_SERIAL(CDamagePathObjList, CObList, 0)
