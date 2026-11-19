#ifndef CHIARCHMESH_H
#define CHIARCHMESH_H

#include "CEXMeshClass.h"

class  CHiarcleVisObj : public CObject
{
	
     
	DECLARE_SERIAL( CHiarcleVisObj );
	
    
public:
    CHiarcleVisObj();
    
	CEXMeshObj       * m_mesh;

	void Invalidate();
	void Optimize();
	void Clone(CHiarcleVisObj ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void SafeDelete();
	void Serialize( CArchive& ar );
};

class  CHiarcleVisList : public CObList
{
public:
   CHiarcleVisList(){};
   
    
     
	DECLARE_SERIAL( CHiarcleVisList );
	
    
    CHiarcleVisObj * GetByIndex(int index);
	void Clone(CHiarcleVisList ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void Optimize();
	void Invalidate();
	void SafeDelete();
};

class  CHiarchyMesh : public CObject
{
	
     
	DECLARE_SERIAL( CHiarchyMesh );
	
    
public:
    CHiarchyMesh();
    
	CHiarcleVisList * m_lodChain;
  
	CHiarcleVisObj * GetByIndex(int index);
	void Load(CString filename);
    void Save(CString filename);
	BOOL SetLevel(int index,CString filename);
	BOOL SaveLevel(int index,CString filename);
	void SafeDelete();
	void Invalidate();
	void Clone(CHiarchyMesh ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void SetFadeIn(long duration);
	void SetFadeOut(long duration);
	void Serialize( CArchive& ar );
};

#endif