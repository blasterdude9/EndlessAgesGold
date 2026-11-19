

#ifndef CCOMMERCECLASS_H
#define CCOMMERCECLASS_H
//---------------------------------------------------//
//---------------------------------------------------//
//---------------------------------------------------//
class  CStoreInventoryObj : public CObject
{
	
     
	DECLARE_SERIAL( CStoreInventoryObj );
	
    
public:
    CStoreInventoryObj();

    int         m_globalInventoryID;
	int         m_raceID;
    
	void SafeDelete();
	void Serialize( CArchive& ar );
};
//---------------------------------------------------//
//---------------------------------------------------//
//---------------------------------------------------//
class  CStoreInventoryObjList : public CObList
{
public:
	CStoreInventoryObjList(){};
    
    
     
	DECLARE_SERIAL( CStoreInventoryObjList );
	
    

	BOOL VerifyInventoryGLID(int GLID);
	void SafeDelete();
};
//---------------------------------------------------//
//---------------------------------------------------//
//---------------------------------------------------//
class  CCommerceObj : public CObject
{
	
     
	DECLARE_SERIAL( CCommerceObj );
	
    
public:
    CCommerceObj();
    
    CString                  m_establishmentName;
    int                      m_uniqueID;
    CStoreInventoryObjList * m_storeInventory;
	int                      m_worldID;
	D3DVECTOR                m_boundingBoxMax;
	D3DVECTOR                m_boundingBoxMin;
	int                      m_menuBind;
	BOOL                     m_willDealWithCriminals;
	int                      m_raceID;

	void SafeDelete();
	void Serialize( CArchive& ar );
	
};
//-----------------------------------------------------//
//-----------------------------------------------------//
//-----------------------------------------------------//
class  CCommerceObjList : public CObList
{
public:
   CCommerceObjList(){};
   
    
     
	DECLARE_SERIAL( CCommerceObjList );
	
    

	CCommerceObj * GetEstablishmentByID(int ID);
	 BOOL BoundBoxPointCheck(D3DVECTOR & point,D3DVECTOR & min,D3DVECTOR & max);
	CCommerceObj * GetEstablishmentByPosition(D3DVECTOR position,
														  int worldID);
	void SafeDelete();
};

#endif