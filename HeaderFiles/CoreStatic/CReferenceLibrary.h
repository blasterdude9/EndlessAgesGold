
#include "CEXMeshClass.h"
#include "CMeshLodClass.h"
#include "CVertexAnimationModule.h"
#include "CSkeletalClass.h"


#pragma once

struct Ext_lockpoints
{
    float x, y, z;
	D3DCOLOR diffuse;
};

class CReferenceObj : public CObject
{
	
     
	DECLARE_SERIAL( CReferenceObj );
	
    
public:
    CReferenceObj();
    
    CString                m_referenceName;
	CEXMeshObj           * m_meshObject;
	CMeshLODObjectList   * m_lodVisualList;
	CEXMeshObj           * m_collisionModel;//NULL is none
	BNDBOX                 m_popoutBox;
	CVertexAnimationModule *m_animModule;
	DWORD                   m_animationUpdatedThisRound;
	BOOL                   m_faceCamera;
	int                    m_seqLoad;
	CSkeletonObject       *m_skeletalOption;//if this is valid it will override general mesh parse-uses default stand animation
	CSkeletonObject       *m_skeletalOptionLive;//initializes at runtime

	Ext_lockpoints       * m_extLockPoints;//if valid creates a batch parse
	int                    m_lockPointCount;
	float                  m_lodRange;
	int                    m_lodLevels;
	float                  m_billboardSz;



	void Serialize( CArchive& ar );
	void InitFromPath(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void InitCollisionObjectFromPath(CString fileName,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SafeDelete();
	 BOOL UpdateLods();
	 BOOL UpdateLods(BOOL filterTextureCarry);
	 void GenerateLockPointMesh(D3DMATRIX & matrix,D3DMATRIX objMatrix,IDirect3DDevice9 * pd3dDevice,float distToCam);
	 void fastMatrixMultiply(D3DMATRIX & R,D3DMATRIX A, D3DMATRIX B);
	 void D3DMath_MatrixInvert( D3DMATRIX& q, D3DMATRIX& a );
	 float InterpolateFloatsHiRes(
						            float StartVec, 
						            float EndVec,
					                float NumOfStages, 
					                float ThisStage);
	 void GenerateLockPointMeshGPU(D3DMATRIX & matrix,
										     D3DMATRIX objMatrix,
										     IDirect3DDevice9 * pd3dDevice,
										     float distToCam,
											 D3DVECTOR & constOne,
											 D3DVECTOR & constTwo,
											 D3DVECTOR & constThree,
											 D3DVECTOR & constFour,
											 int ovrdLOD);
	 void GenerateGPUContants(D3DMATRIX & matrix,
							  IDirect3DDevice9 * pd3dDevice,
							  float & distToCam,
							  D3DVECTOR & constOne,
							  D3DVECTOR & constTwo,
							  D3DVECTOR & constThree,
							  D3DVECTOR & constFour,
							  D3DVECTOR & constNorm,
							  float & maxX,
							  float & maxY,
							  WORD & vCount);
	 void CopyValues(CReferenceObj * stencil,LPDIRECT3DDEVICE9 d3dDevice);
	 
	
};

class CReferenceObjList : public CObList
{
public:
	CReferenceObjList(){};
   void SafeDelete();
    
     
	DECLARE_SERIAL( CReferenceObjList );
	
    

	 void ReinitAll(LPDIRECT3DDEVICE9 g_pd3dDevice);
	 CReferenceObj * GetByIndex(int & index);
	 int GetReferenceIndexByName(CString name);
	 CReferenceObj * GetReferenceObjByName(CString name);
	void FlagAllForAnimUpdate();
	void ClearInstances();


};
