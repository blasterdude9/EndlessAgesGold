#include "StdAfx.h"
#include "SelectionEffect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSelectionEffect::CSelectionEffect(void)
{
	m_skeletalPtr = NULL;
	m_usedThisRound = FALSE;
}

CSelectionEffect::~CSelectionEffect(void)
{
	if(m_skeletalPtr)
	{
       m_skeletalPtr->SafeDelete();
	   delete m_skeletalPtr;
	   m_skeletalPtr = 0;
	}
}

//------------------------------------------------------------------//
//------------------------------------------------------------------//
//--------------------Serialize-------------------------------------//
void CSelectionEffect::Serialize( CArchive& ar )
{
BOOL reserved = 0;

CObject::Serialize( ar );
	if(ar.IsStoring() ){

	   ar << m_skeletalPtr
		  << m_usedThisRound
		  << m_effectName;
	  
	}
	else
	{

       ar >> m_skeletalPtr
		  >> m_usedThisRound
		  >> m_effectName;
	}
}




IMPLEMENT_SERIAL(CSelectionEffect, CObject, 0)
IMPLEMENT_SERIAL(CSelectionEffectList, CObList, 0)