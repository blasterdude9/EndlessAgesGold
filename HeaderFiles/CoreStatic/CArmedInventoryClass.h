#ifndef CARMEDINVENTORYCLASS_H
#define CARMEDINVENTORYCLASS_H

#include "matrixarb.h"
#include "CSkeletalClass.h"

class  CArmedInventoryObj : public CObject
{
	CArmedInventoryObj(){}
	
     
    DECLARE_SERIAL( CArmedInventoryObj )
    
    
public:
	
	CArmedInventoryObj(CString name);
 

	CSkeletonObject            * m_firstPersonSystem;
	CSkeletonObject            * m_thirdPersonSystem;
    CString                      m_name;
	int                          m_boneAttached;
	int                          m_functionalityAttribute;
	int                          m_miscInt;
    int                          m_miscInt2;
	int                          m_miscInt3;
	D3DVECTOR                    m_miscPosition;
	int                          m_id;
	int                          m_skillUse;//
	int                          m_abilityTypeUse;//
	int                          m_lastUpdatedTime;
	int                          m_locationOccupancy;//the same cannot occupy at the same time
	int                          m_animAccessVersion;
	int                          m_type;
	int                         *m_obfusionMiscInt;
	int                          m_reloadDelay;
	BOOL                         m_tempItem;
	BOOL						 m_alwaysShowFirstPerson;

	CString                      m_boneMapping;

	//fire que 
	int							 m_fireQueOvrd;


    void Invalidate();
    void SafeDelete();
	void Clone(CArmedInventoryObj **copy,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void Serialize( CArchive& ar );
};


struct DirectAccessStructAIO
{
    CArmedInventoryObj * object;
};

class  CArmedInventoryList : public CObList
{
public:
   CArmedInventoryList(){directAccessObjectCount=0;directAccess=NULL;m_lodScriptHasBeenExecuted=0;m_texturesHaveBeenInitialized=0;};


   char m_texturesHaveBeenInitialized;
   char m_lodScriptHasBeenExecuted;
   BOOL RemoveByID(int id);
   BOOL GetIndexByID(int id);
   CArmedInventoryObj * GetByTypeDirectAccess(int type);
   BOOL RemoveByType(int type,BOOL deleteMem);
   void SafeDelete();
    void Invalidate();
   CArmedInventoryObj * GetByType(int type);
   void Clone(CArmedInventoryList ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice);
   void CompileDirectAccess();
   BOOL MergeAsTempItems(CArmedInventoryList * tempItemsDB);
   void ClearTempItems();
   int GetHighestNumber();
   int GetIndexByType(int type);
   CArmedInventoryObj * GetByName(CString name);
   
   int GetFreeID();
   void ApplyLODProcessing(CString strLodScript);

   DirectAccessStructAIO * directAccess;
   int                     directAccessObjectCount;
  
   DECLARE_SERIAL( CArmedInventoryList )    
};


#endif