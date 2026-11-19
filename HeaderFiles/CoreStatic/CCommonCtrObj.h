
#include "CSkeletalClass.h"
#include "CEXMeshClass.h"
#include "CRTLightClass.h"
#include "CFrameObj.h"
#include "CSSoundRange.h"

#ifndef CCOMMONCTRLOBJ_H
#define CCOMMONCTRLOBJ_H

class  CCommonCtrlObj : public CObject
{
	
     
	DECLARE_SERIAL( CCommonCtrlObj );
	
    
public:
    CCommonCtrlObj();
    CString                m_controlName;
    CSkeletonObject       *m_idleState;
	CSkeletonObject       *m_hoverState;
    CSkeletonObject       *m_clickState;
	int                    m_type;
	
    void SafeDelete();
	CSkeletonObject * GetByState(int state);
	BOOL Reinit(LPDIRECT3DDEVICE9 g_pd3dDevice);
	void Serialize( CArchive& ar );
	
};

class  CCommonCtrlList : public CObList
{
public:
   CCommonCtrlList(){};
   void SafeDelete();
   void ReinitAll(LPDIRECT3DDEVICE9 g_pd3dDevice);
   CCommonCtrlObj * GetByIndex(int index);
    
     
	DECLARE_SERIAL( CCommonCtrlList );
	
    

};

#endif