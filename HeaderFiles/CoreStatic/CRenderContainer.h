#include "matrixarb.h"
#include "CEXMeshClass.h"
#include "CStateManagementClass.h"
#ifndef CRENDERCONTAINER_H
#define CRENDERCONTAINER_H

class   CRenderContainerObj : public CObject
{
    
public:
	CRenderContainerObj(){};
	CRenderContainerObj(D3DMATRIX          & positionalMatrix,
					    CMaterialObject    * materialRef,
						CEXMeshObj         * mesh);
   
	MatrixARB             GL_matrixFunctions;
	D3DMATRIX             m_positionalMatrix;
	CMaterialObject       *m_materialReference;
	CEXMeshObj            *m_mesh;
	D3DMATERIAL9           m_materialRef;
	int                    m_blendRef;

	 void SafeDelete();
	 void Render(LPDIRECT3DDEVICE9     g_pd3dDevice,
					   BOOL                & mirrorEnable,
					   D3DMATRIX           & mirrorMatrix,
					   D3DVECTOR           & locationVect,
					   CStateManagementObj * stateManager,
					   CTextureDBObjList   * textureDataBase,
					   float               & gl_adjust,
					   int                 & totalIndecieCount,
					   int                 & objectsRendered);
};

class   CRenderContainerObjList : public CObList
{
public:
    
	CRenderContainerObjList(){};
    void SafeDelete();
    void AddEXMesh(CEXMeshObj * meshRef);
    void Render(LPDIRECT3DDEVICE9    g_pd3dDevice,
					  BOOL                &mirrorEnable,
					  D3DMATRIX           &mirrorMatrix,
					  D3DVECTOR           &locationVect,
					  CStateManagementObj * stateManager,
					  CTextureDBObjList   * textureDataBase,
					  float               & gl_adjust,
					  int                 & totalIndecieCount,
					  int                 & objectsRendered);
};

#endif