#include "stdafx.h"

#define D3D_OVERLOADS
#include <afxtempl.h>
#include "d3d9.h"
#include "CSphereCollisionClass.h"
 
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSphereCollisionObj::CSphereCollisionObj()
{
    m_boneIndex          = 0;
    m_sphereRadius       = 0.0f;
    m_damageMultiplier   = 1.0f;
}

void CSphereCollisionObj::Clone(CSphereCollisionObj ** copy)
{
     CSphereCollisionObj * copyLocal = new CSphereCollisionObj();
     copyLocal->m_boneIndex            = m_boneIndex;
	 copyLocal->m_damageMultiplier     = m_damageMultiplier;
	 copyLocal->m_sphereRadius         = m_sphereRadius;
	 copyLocal->m_comment              = m_comment;

     *copy = copyLocal;
}

void CSphereCollisionObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	    ar << m_boneIndex
		   << m_sphereRadius
		   << m_damageMultiplier
		   << m_comment;
	}
	else
	{
        ar >> m_boneIndex
		   >> m_sphereRadius
		   >> m_damageMultiplier
		   >> m_comment;
	}
}

void CSphereCollisionObjList::SafeDelete()
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CSphereCollisionObj * csObj = (CSphereCollisionObj *)GetNext(posLoc);
        delete csObj;
		csObj=0;
	}
	RemoveAll();
}

void CSphereCollisionObjList::Clone(CSphereCollisionObjList ** copy)
{
	CSphereCollisionObjList * copyLocal = new CSphereCollisionObjList;
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CSphereCollisionObj * csObj = (CSphereCollisionObj *)GetNext(posLoc);
        CSphereCollisionObj * clone = NULL;
		csObj->Clone(&clone);
        copyLocal->AddTail(clone);
	}
    *copy = copyLocal;
}


IMPLEMENT_SERIAL(CSphereCollisionObj, CObject, 0)
IMPLEMENT_SERIAL(CSphereCollisionObjList, CObList, 0)
