#include "CEXMeshClass.h"
#include "CStructuresMisl.h"
#include "matrixarb.h"

#ifndef CMESHLODCLASS_H
#define CMESHLODCLASS_H

class CMeshLODObject : public CObject
{
	
     
	DECLARE_SERIAL( CMeshLODObject );
	
    
public:
	
	CMeshLODObject();
	

    
	float           m_distance;//greater than activate it
	ABBOX           m_boxOption;
    CEXMeshObj *    m_meshLodLevel;
	
	
	void        Clone(CMeshLODObject ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void        SafeDelete();
	void        Serialize( CArchive& ar );

};

class CMeshLODObjectList : public CObList
{
public:
   CMeshLODObjectList();


   int m_lodType;//0=distance from center 1=box, 2=distance overlap fade
   float m_optionalOverlapDist;
   

   void SafeDelete();
   MatrixARB GL_matrixFunctions;

   void Initialize(LPDIRECT3DDEVICE9 g_pd3dDevice, void * cgMaterialDB);
   void Clone(CMeshLODObjectList ** copy,LPDIRECT3DDEVICE9 g_pd3dDevice);
   BOOL AddLOD(int type,
	           ABBOX * box,
			   float distance,
			   CString filename,
			   CMaterialObject * materialObject,
			   LPDIRECT3DDEVICE9 g_pd3dDevice);
   CString GetFileExt(CString currentName);
   CEXMeshObj * GetLodVisual(D3DVECTOR & viewPoint,D3DVECTOR & objPoint,float & currentFov,int & lodOverride);
   BOOL BoundBoxPointCheck(D3DVECTOR & point,ABBOX & box);
   void RemoveLod(int index);
   CEXMeshObj * GetByIndex(int lod);
   CEXMeshObj * GetLodVisualExt(D3DVECTOR & viewPoint,
											     D3DVECTOR & objPoint,
											     float     & currentFov,
                                                 CEXMeshObj** refObj2,
												 float     * obj1AlpaOpt,
												 float     * obj2AlpaOpt,
												 int & lodOverride);
   
    
   DECLARE_SERIAL( CMeshLODObjectList )
   
   
   void Serialize( CArchive& ar );
};

#endif