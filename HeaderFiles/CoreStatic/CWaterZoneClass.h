#include "CStructuresMisl.h"
#include "CEXMeshClass.h"
#ifndef CWATERZONECLASS_H
#define CWATERZONECLASS_H


//#define DECLARE_SERIAL(CWaterZoneObj) _DECLARE_DYNCREATE(CWaterZoneObj) AFX_EXT_API friend CArchive& AFXAPI operator>>(CArchive& ar, CWaterZoneObj* &pOb);

class  CWaterZoneObj : public CObject
{
    #undef AFX_DATA
    #define AFX_DATA AFX_EXT_DATA
    DECLARE_SERIAL( CWaterZoneObj );
    #undef AFX_DATA
    #define AFX_DATA

public:

	

    CWaterZoneObj();
    
	CString m_waterZoneName;
    ABBOX m_waterArea;
	BOOL  m_isExclusionZone;//if false is a water zone
  

    BOOL InitFromARW(CString filename,LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL BoundBoxPointCheck(D3DVECTOR & point,ABBOX box);
	BOOL IsPositionInZone(D3DVECTOR position);
	void Serialize( CArchive& ar );
	
};




class  CWaterZoneObjList : public CObList
{
   
	DECLARE_SERIAL( CWaterZoneObjList );

public:
	CWaterZoneObjList(){};
	
    //void Serialize( CArchive& ar );

	void SafeDelete();
	BOOL IsPositionInWater(D3DVECTOR position,float * submergedValue);
};



#endif