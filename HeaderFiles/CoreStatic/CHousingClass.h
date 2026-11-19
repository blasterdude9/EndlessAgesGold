#include "CCollisionClass.h"
#include "CEXMeshClass.h"
#include "CRTLightClass.h"

#ifndef CHOUSINGCLASS_H
#define CHOUSINGCLASS_H

class  CHousingObj : public CObject
{
	
     
	DECLARE_SERIAL( CHousingObj );
	
    
public:
    CHousingObj();

    CString                      m_houseName;
    CCollisionObj               *m_closedState; 
    CCollisionObj               *m_openState; 
	CEXMeshObjList              *m_externalVisuals;
    CEXMeshObjList              *m_internalVisuals;
	CEXMeshObjList              *m_lod2Set;
	CEXMeshObjList              *m_lod3Set;
	CEXMeshObj                  *m_doorVisual;
	ABBOX                        m_extrnalRangeLod1;
    ABBOX                        m_extrnalRangeLod2;
	ABBOX                        m_extrnalRangeLod3;
	ABBOX                        m_internalArea;
    ABBOX                        m_clipBox;
	CRTLightObj                 *m_lightObj;
	D3DVECTOR                    m_lightPosition;


    void SafeDelete();
	void Serialize( CArchive& ar );
	CEXMeshObjList * GenerateRenderCache(D3DVECTOR & point,BOOL open);
	BOOL FlushCacheObject(CEXMeshObjList * cache);
	 BOOL BoundBoxPointCheck(D3DVECTOR & point,ABBOX & box);

	
};

class  CHousingObjList : public CObList
{
public:
	CHousingObjList(){};
    void SafeDelete();
	void ReinitAll(LPDIRECT3DDEVICE9 g_pd3dDevice);
	CHousingObj * GetByIndex(int index);
    
     
	DECLARE_SERIAL( CHousingObjList );
	
    

};

//------------used for server side tracking--------------//
class  CHousingPlacementObj : public CObject
{
	
     
	DECLARE_SERIAL( CHousingPlacementObj );
	
    
public:
    CHousingPlacementObj();

    int              m_dbIndex;
	int              m_worldID;
	D3DVECTOR        m_position;
    BOOL             m_open;


    void SafeDelete();
	void Serialize( CArchive& ar );
	
};

class  CHousingPlacementList : public CObList
{
public:
	CHousingPlacementList(){};
    void SafeDelete();
    
     
	DECLARE_SERIAL( CHousingPlacementList );
	
    

};
//------------used for client side use/ref--------------//
class  CHousingCacheObj : public CObject
{
    
public:
    CHousingCacheObj();
  
    int              m_dbIndex;
	D3DVECTOR        m_position;
	D3DMATRIX        m_location;
	D3DMATRIX        m_invLocation;
    BOOL             m_open;
	BOOL             m_validThisRound;
	int              m_acId;

    void SafeDelete();	
};

class  CHousingCacheObjList : public CObList
{
public:
	CHousingCacheObjList(){};
    void SafeDelete();
	void PlaceHouse(D3DVECTOR position, int dbIndex, int acID);
	void FlagAllInvalid();
	void RemoveAllInvalid();
	BOOL FlagValidByAcID(int & id,BOOL open);
	CHousingCacheObj * GetByAcID(int acID);
};




#endif