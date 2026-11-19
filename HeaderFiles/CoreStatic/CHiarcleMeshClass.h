#ifndef CHIARCHMESH_H
#define CHIARCHMESH_H

#include "CEXMeshClass.h"

class CHiarcleVisObj : public CObject
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

class CHiarcleVisList : public CObList
{
public:
   CHiarcleVisList(){};
   
    
     
	DECLARE_SERIAL( CHiarcleVisList );
	
    
    //CHiarcleVisObj * GetByIndex(int index);
	void Clone(CHiarcleVisList ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void Optimize();
	void Invalidate();
	void SafeDelete();

    void DeleteByIndex(int index)
	{
		int trace = 0;
		POSITION posLast = NULL;
         for(POSITION posLoc = GetHeadPosition();(posLast=posLoc) != NULL;)
		 {
             CHiarcleVisObj * vObj = (CHiarcleVisObj *)GetNext(posLoc);
             if(trace == index)
			 {
                vObj->SafeDelete();
				delete vObj;
				vObj = 0;
                RemoveAt(posLast);
			    return;
			 }
			 trace++;
		 }
	}

	CHiarcleVisObj * GetByIndex(int index)
	{
         if(index < 0)
		    return NULL;

         POSITION posLoc = FindIndex(index);
		 if(!posLoc)
		    return NULL;
        
		 CHiarcleVisObj * vObj = (CHiarcleVisObj *)GetAt(posLoc);

		 return vObj;
	}
};

class CHiarchyMesh : public CObject
{
	
     
	DECLARE_SERIAL( CHiarchyMesh );
	
    
public:
    CHiarchyMesh();
    
	CHiarcleVisList * m_lodChain;
    bool              m_collisionFiltered;

  
	CHiarcleVisObj * GetByIndex(int index);
	CHiarcleVisObj * GetLOD(int & lod, int & overrideLod);
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