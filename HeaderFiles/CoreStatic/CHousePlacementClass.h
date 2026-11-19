#include "CInventoryClass.h"
#include "CStructuresMisl.h"
//

#ifndef CHOUSEPLACEMENTCLASS_H
#define CHOUSEPLACEMENTCLASS_H

class  CHPlacementObj : public CObject
{
	
     
	DECLARE_SERIAL( CHPlacementObj );
	
    
public:
    CHPlacementObj();
    
    int                 m_type;//DB reference
    int                 m_accountBind;//one placement per accout allowed
    D3DVECTOR           m_wldPosition;
	int                 m_id;
	BOOL                m_open;
	BOOL                m_vaultInstalled;
	int                 m_vaultItemCapacity;
	int                 m_vaultCurrentCash;
	CInventoryObjList * m_vaultInventory;
	int                 m_storeItemCapacity;
	int                 m_storeCurrentCash;
	CInventoryObjList * m_storeInventory;

	void SafeDelete();
	void Serialize( CArchive& ar );
	void Clone(CHPlacementObj ** clone);
};

class  CHPlacementObjList : public CObList
{
public:
	CHPlacementObjList(){};
	CHPlacementObj * GetByAccountBind(int accountBind);
	BOOL AddPlacement(int type,int accountBind,D3DVECTOR position,int id);
	BOOL RemoveByAccountBind(int accountBind);
	CHPlacementObj* GetHouseByBox(D3DVECTOR position);
	 BOOL BoundBoxPointCheck(D3DVECTOR & point,D3DVECTOR & min,D3DVECTOR & max);
	void Clone(CHPlacementObjList ** clone);

    void SafeDelete();
    
     
	DECLARE_SERIAL( CHPlacementObjList );
	
    

};
//---------
struct Placement
{
    CHPlacementObjList * m_placementList;
};

class  CHousingZone : public CObject
{
	
     
	DECLARE_SERIAL( CHousingZone );
	
    
public:
    CHousingZone();
	void SafeDelete();
    
	CString       m_zoneLabel;
	BNDBOX        m_visibleBox;
	BNDBOX        m_zoneBox;
	float              m_subDivisionX;
    float              m_subDivisionY;
	float              m_subDivisionZ;
	int                 m_wldID;
	float             m_densityAllowance;

	BOOL m_optimizedSave;

	Placement *** m_hsTable;

	CHPlacementObjList * m_houseDatabase;

	void Serialize( CArchive& ar );	
	void Clone(CHousingZone ** clone);
	void CreateNewHouseingZone(CString zoneName,
										 float subX,
										 float subY,
										 float subZ,
										 int wldID,
										 BNDBOX box,
										 BNDBOX visibleBox);
	 BOOL BoundBoxPointCheck(D3DVECTOR & point,D3DVECTOR & min,D3DVECTOR & max);
	int AddPlacementObjToTable(int type,
                                int m_accountBind,
                                BNDBOX m_clipBoxOfObjectPlaced,
								D3DVECTOR wldPosition,
								int id);
	CHPlacementObjList * GetContainerByWldPosition(D3DVECTOR position);
	BOOL RemoveByAccountBind(int accountBind);
	CHPlacementObjList * GetVisibleList(D3DVECTOR wldPosition,float range);
	float MagnitudeSquared(float &Mx1,float &My1,
								         float &Mz1,float &Mx2,
								         float &My2,float &Mz2);
	BOOL AnyHousesWithinArea(D3DVECTOR position,float radius);
	void SerializeAlloc(CArchive& ar , CRITICAL_SECTION * cSection);
};

class  CHousingZoneDB : public CObList
{
    
public:
	CHousingZoneDB();
    void SafeDelete();
    
     
	DECLARE_SERIAL( CHousingZoneDB );
	
    

	int m_currentIDCount;
	CRITICAL_SECTION m_cSection;
	BOOL m_lock;
	

	int AddPlacementObjToTable(int type,
                                int m_accountBind,
                                BNDBOX m_clipBoxOfObjectPlaced,
							    D3DVECTOR wldPosition,
								int * idReturn,
								int wldID);
	CHPlacementObjList * GetVisibleList(D3DVECTOR wldPosition,float range,int wldID);
	 BOOL BoundBoxPointCheck(D3DVECTOR & point,D3DVECTOR & min,D3DVECTOR & max);
	CHPlacementObjList * GetContainer(D3DVECTOR wldPosition);
	CHPlacementObjList * GetContainerAndZone(D3DVECTOR wldPosition,CHousingZone ** zone);
	CHPlacementObj * GetHousePlacementByPosAcSpecial(D3DVECTOR wldPosition,int acID,int wldID);
	CHPlacementObj * GetHousePlacementByPosAc(D3DVECTOR wldPosition,int acID);
	void RemoveHouseByAccountBind(int acID);
	int RemoveHouseByPosition(D3DVECTOR wldPosition,int wldID);
	CHPlacementObj * GetHousePlacementByPos_area(D3DVECTOR wldPosition);
	void Clone(CHousingZoneDB ** clone);
	CHPlacementObj * GetHouseByAccountBind(int acID);
	CHPlacementObj *  GetHouseByPositionAndWldID(D3DVECTOR wldPosition,int wldID);
	CHPlacementObj * GetHousePlacementByPosAc(D3DVECTOR wldPosition,int acID,int * retWldID);
	void SerializeAlloc(CArchive& ar , CRITICAL_SECTION * cSection);
	BOOL ExportToSQL(void *eventLogMem);
	BOOL ImportFromSql(void *eventLogMem,void *houseDBMem,void * globalInvenMem);
	void ClearAllPlacements();
	

};

#endif