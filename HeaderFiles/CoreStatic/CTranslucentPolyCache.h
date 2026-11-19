#include "matrixarb.h"
#include "CStructuresMisl.h"
#include "CMaterialClass.h"
#include "CEXMeshClass.h"
#include "CStateManagementClass.h"
#include "CReferenceLibrary.h"
#include "EnvParticleSystem.h"
#include "CRTLightClass.h"
#ifndef CTRANSLUCENTPOLYCACHE_H
#define CTRANSLUCENTPOLYCACHE_H

#define MAXRENDEROBJECTS 7000


class   CTranslucentPolyObj : public CObject
{
	
public:
    CTranslucentPolyObj(){};
   
	//ABVERTEX           m_vertex[3];
	//CMaterialObject    m_material;
	//int                m_textureSelect[10];
	float              m_zValue;
	D3DXMATRIX          m_location;
	CEXMeshObj        *m_meshReference;
	//MatrixARB          GL_matrixFunctions;
	D3DMATERIAL9       m_materialRef;
	int                m_blendRef;
	EnvParticleSystem *m_particleSysOption;
	CReferenceObj     *m_referenceOption;


	 void Render(   LPDIRECT3DDEVICE9   g_pd3dDevice,
						  BOOL                &mirrorEnable,
						  D3DMATRIX           &mirrorMatrix,
						  D3DVECTOR           &locationVect,
						  CStateManagementObj * stateManager,
						  CTextureDBObjList   * textureDataBase,
					      float               & gl_adjust,
						  int                 & totalIndecieCount,
						  int                 & objectsRendered,
						  D3DVECTOR           & camLocation,
						  D3DMATRIX           & matrixLoc, 
						  int                 & particleCalculationCount,
						  CEXMeshObj          ** lastMeshREendered,
						  BOOL                & alphaMode,
						  CRTLightObjList     * m_lightDatabase);
	 float MagnitudeSquared(float &Mx1,float &My1,
								         float &Mz1,float &Mx2,
								         float &My2,float &Mz2);
  	
};

struct polyObjDBindexed
{
    CObList m_list;
};

class   CTranslucentPolyList : public CObList
{
public:
   CTranslucentPolyList();

   CEXMeshObj       *		m_lastMeshREendered;
   BOOL						m_alphaMode;

   int						m_indexDensity;
   int						m_validRange;
   int						m_validCount;
   int						m_currentIndexTemp;
   polyObjDBindexed			m_indexedDBAccess[2500];
   CTranslucentPolyObj		m_preAllocObjs[MAXRENDEROBJECTS];
   int						m_allocTracer;
   
    
   //sort options
   int m_currentObjectCount;
   POSITION m_lastPosition;
   float m_lastValue;

   //functions
   void SafeDelete();
    void CacheMesh(CEXMeshObj       * mesh,
	                     EnvParticleSystem *particleSysOptionRef,
				         D3DMATRIX        & viewMatrix,
				         D3DMATRIX        & wldMatrix,
						 D3DVECTOR        & meshCenter,
						 CReferenceObj * refOpt);
	void CacheMeshNoSort(CEXMeshObj * mesh,
											EnvParticleSystem *particleSysOptionRef,
									        D3DMATRIX  & viewMatrix,
									        D3DMATRIX  & wldMatrix,
											D3DVECTOR  & meshCenter,
											CReferenceObj * refOpt);
	void InsertPolygonOrMeshRefNonTrans(ABVERTEX         *vertex,
					                                     D3DMATRIX        &viewMatrix,
										                 D3DMATRIX        &wldMatrix,
												         CEXMeshObj       *meshRef,
														 D3DVECTOR        &meshCenter,
														 EnvParticleSystem *m_particleSysOptionRef,
														 CReferenceObj * refOpt
												         );
    void InsertPolygonOrMeshRef(ABVERTEX         *vertex,
					                  D3DMATRIX        &viewMatrix,
							          D3DMATRIX        &wldMatrix,
							          CEXMeshObj       *meshRef,
									  D3DVECTOR        &meshCenter,
									  EnvParticleSystem *m_particleSysOptionRef,
									  CReferenceObj * refOpt
							          );
    void Render(   LPDIRECT3DDEVICE9    g_pd3dDevice,
						 BOOL                &mirrorEnable,
						 D3DMATRIX           &mirrorMatrix,
						 D3DVECTOR           &locationVect,
						 CStateManagementObj * stateManager,
						 CTextureDBObjList   * textureDataBase,
						 double               & gl_adjust,
						 int                 & totalIndecieCount,
						 int                 & objectsRendered,
						 BOOL                  frontToBack,
						 D3DVECTOR           & camLocation,
						 D3DMATRIX           & matrixLoc, 
						 int                 & particleCalculationCount,
						 CRTLightObjList     * m_lightDatabase);
   void Flush();
   int GetWorseList();

   MatrixARB                  GL_matrixFunctions;
};

#endif