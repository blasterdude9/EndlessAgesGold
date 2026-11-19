//#include "SpeedTreeRT.h"
#include "matrixarb.h"
#include "CReferenceLibrary.h"

#ifndef CTREEIMPORTER_H
#define CTREEIMPORTER_H

class  AFX_EXT_CLASS CTreeImporterObj : public CObject
{

public:
    CTreeImporterObj();

	D3DMATRIX MatrixInit2;
    MatrixARB GL_matrixFunctions;

	CEXMeshObj * GetLeafBuffersEXMesh(CSpeedTreeRT* m_pSpeedTree, 
									  Ext_lockpoints ** lockPointsReturn, 
								      int & lockCount,
								      float & billboardSize,
									  CSpeedTreeRT::SGeometry* m_pGeometryCache,
									  int lodOffset,
									  float lodLevel);

	CEXMeshObj * GetFrondsEXMeshObjFromSpeedTree(float lodVal,CSpeedTreeRT* m_pSpeedTree,CSpeedTreeRT::SGeometry* m_pGeometryCache);
	CEXMeshObj * GetEXMeshObjFromSpeedTree(float lodVal,CSpeedTreeRT* m_pSpeedTree,CSpeedTreeRT::SGeometry* m_pGeometryCache);
	BOOL GetLibraryObjFromSPTFile(CReferenceObjList * refDB,
								  CString * textureSearchPaths,
								  int & searchPathCount,
								  LPDIRECT3DDEVICE9 pd3dDevice);
	CEXMeshObj * GetBillboardBuffersEXMesh(CSpeedTreeRT* m_pSpeedTree, 
											  Ext_lockpoints ** lockPointsReturn, 
											  int & lockCount,
											  float & billboardSize,
											  CSpeedTreeRT::SGeometry* m_pGeometryCache,
											  int lodOffset,
											  CString title);

	
};


#endif