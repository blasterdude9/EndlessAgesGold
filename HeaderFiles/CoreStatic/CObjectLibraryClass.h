#include "CWldObject.h"

#ifndef COBJECTLIBRARYCLASS_H
#define COBJECTLIBRARYCLASS_H

class   CObjectLibraryObj : public CObject
{
	
     
	DECLARE_SERIAL( CObjectLibraryObj );
	
    
public:
    CObjectLibraryObj();
	void SafeDelete();

	CString      m_description;
    CWldObject * m_wldObject;

	BOOL InitFromFile(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void Serialize( CArchive& ar );
};

class   CObjectLibraryObjList : public CObList
{
public: 
    
     
	DECLARE_SERIAL( CObjectLibraryObjList );
	
    

	CObjectLibraryObjList();
	CWldObject * m_altCollisionModel;
	CString      m_description;

	BOOL AddCollisionObject(CString filename,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void RemoveCollisionObject();
	BOOL AddObject(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice);
    void SafeDelete();
	void Serialize( CArchive& ar );
};

class   CObjectCollection : public CObject
{
	
public: 
    
     
	DECLARE_SERIAL( CObjectCollection );
	
    

	CObjectLibraryObjList * m_collection;
	
    CObjectCollection();
    void SafeDelete();
	void Serialize( CArchive& ar );
};

class   CObjectCollectionList : public CObList
{
	
public: 
    
     
	DECLARE_SERIAL( CObjectCollectionList );
	
    
	
    CObjectCollectionList(){};
    void SafeDelete();
};




#endif