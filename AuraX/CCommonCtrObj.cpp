#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CCommonCtrObj.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CCommonCtrlObj::CCommonCtrlObj()
{
    m_idleState   = NULL;
	m_hoverState  = NULL;
    m_clickState  = NULL;
    m_type        = 0;
}

CSkeletonObject * CCommonCtrlObj::GetByState(int state)
{
	if(state == 0)
       return m_idleState;
	if(state == 1)
	{
	  if(m_hoverState)
       return m_hoverState;
	  else
	   return m_idleState;
	}
    if(state == 2)
	{
       if(m_clickState)
          return m_clickState;
	   else
	      return m_idleState;

	}

	return NULL;
}

BOOL CCommonCtrlObj::Reinit(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	if(m_idleState)
       m_idleState->Reinit(g_pd3dDevice);
	if(m_hoverState)
       m_hoverState->Reinit(g_pd3dDevice);
    if(m_clickState)
       m_clickState->Reinit(g_pd3dDevice);

	return TRUE;
}

void CCommonCtrlObj::SafeDelete()
{
    if(m_idleState)
	{
      m_idleState->SafeDelete();
	  delete m_idleState;
	  m_idleState = 0;
	}

	if(m_hoverState)
	{
      m_hoverState->SafeDelete();
	  delete m_hoverState;
	  m_hoverState = 0;
	}

	if(m_clickState)
	{
      m_clickState->SafeDelete();
	  delete m_clickState;
	  m_clickState = 0;
	}

}

void CCommonCtrlList::ReinitAll(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CCommonCtrlObj * spnPtr = (CCommonCtrlObj *)GetNext(posLoc);
		spnPtr->Reinit(g_pd3dDevice);
	}
}

void CCommonCtrlList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CCommonCtrlObj * spnPtr = (CCommonCtrlObj *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

CCommonCtrlObj * CCommonCtrlList::GetByIndex(int index)
{
    if(index < 0)
		return NULL;

	POSITION posLoc = FindIndex(index);
	if(!posLoc)
		return NULL;
    CCommonCtrlObj * spnPtr = (CCommonCtrlObj *)GetAt(posLoc);
	return spnPtr;
}


void CCommonCtrlObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_idleState
		  << m_hoverState
		  << m_clickState
		  << m_type
		  << m_controlName;
	}
	else
	{
       ar >> m_idleState
		  >> m_hoverState
		  >> m_clickState
		  >> m_type
		  >> m_controlName;
	}
}     

IMPLEMENT_SERIAL(CCommonCtrlObj, CObject, 0)
IMPLEMENT_SERIAL(CCommonCtrlList, CObList, 0)

