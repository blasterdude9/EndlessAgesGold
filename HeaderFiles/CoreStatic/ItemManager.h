#pragma once

#include "citemclass.h"
#include "CArmedInventoryClass.h"
#include "caxlist.h"
#include "CMovementModel.h"
#include "dbconnectionsystem.h"
#include "CElementsClass.h"

class CItemManager : public CAxDB
{
public:
	CItemManager(void);
	~CItemManager(void);
	
	//VARIABLES
    CRITICAL_SECTION		m_addDelSect;//only lock with add and delete but search can still go simutanious
    int						m_count;
	CString                 m_filePath; // last successfully loaded or saved APX path
	CArmedInventoryList    *m_visuals;//contains skeletal-damage paths-fire ques

	//FUNCTIONS
	CItemObj * GetItemByID(int globalID);
	bool Add_safe(CItemObj * itemMem);
	BOOL DeleteByOject(CItemObj * itemMem);
	CItemObj * GetItemByID_LOS(int globalID);
	BOOL Save(char * fileName);
	BOOL Load(char * fileName);
	BOOL ImportEDB(char * fileName);
	BOOL ImportAPD(char * fileName);
	BOOL RemoveByData_safe(int globalID, char * name);
	CItemObj * GetItemByName_LOS(char * name);
	CItemObj * GetItemByName(char * name);
	BOOL VerifyRaceCanArm_safe(int raceBind, int globalID);
	int  GetGLIDByName_safe(char * name);
	BOOL GetIsStackableByName_safe(char * name);
	BOOL Exists_safe(int globalID);
	bool AddItemByDataPacket(byte * data);
	int GetFreeGlobalID_safe();
	BOOL DoesItemExist(int globalID);
	BOOL ArmEntity(int globalID,CMovementObj * entityLive,CMovementObj * refDBEntity,LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL DisarmEntity(int globalID, CMovementObj * entityLive, CMovementObj * refDBEntity, LPDIRECT3DDEVICE9 g_pd3dDevice);
	bool AquireByGlid(CString glid,CElementObjList * elementDB,CStatObjList * statDB,CDBConnectionSystem * dbConnection);
	int ImportAllDynamicItems(	CElementObjList * elementDB,CStatObjList * statDB,CDBConnectionSystem * dbConnection);
	bool GenerateDynItemPacket(byte ** data,int & dataSize, int glid, int optNetID);
	BOOL GetItemDynOvrds_safe(int & globalID,int * damage,int * element,int * fireQue);
};
