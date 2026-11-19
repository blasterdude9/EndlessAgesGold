#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CDynamicOverlay.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CDynamicOverlayObj::CDynamicOverlayObj()
{
   m_screenPos.x = 0.0f;
   m_screenPos.y = 0.0f;
   m_screenPos.z = 0.0f;
   m_multiplier = 1.0f;
}


void CDynamicOverlayList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CDynamicOverlayObj * spnPtr = (CDynamicOverlayObj *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}
