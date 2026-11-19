#ifndef CDEFORMABLEMESHCLASS_H
#define CDEFORMABLEMESHCLASS_H

#include "CEXMeshClass.h"

struct VertexAssignment
{
    byte   boneIndex;
	short   vertexIndex;
	short * vertexesCarried;//optimizing vertex movement
	byte   carriedCount;
};

struct VertexAssignmentBlended
{
	byte    boneIndexesCount;
	ABVERTEX0L * offsetVects;
    byte   *boneIndexes;
	float  *weightsToBone;
	short   vertexIndex;
	short * vertexesCarried;//optimizing vertex movement
	byte   carriedCount;
};


class  CDeformableVisObj : public CObject
{
	
     
	DECLARE_SERIAL( CDeformableVisObj );
	
    
public:
    CDeformableVisObj();
    
    CString                   m_nameTemp;
	CEXMeshObj              * m_mesh;
	D3DVECTOR               * m_originalArrayVector;
	VertexAssignment        * m_vertexAssignments;
	VertexAssignmentBlended * m_blendedVertexAssignments;
	int                       m_assignmentCount;
	BOOL                      m_referencedMemory;
	BOOL                      m_blendedType;


	void Invalidate();
	void Optimize();
	void OptimizeBlended(float angTol);
	void OptimizeVertexAssignments();
	void OptimizeVertexAssignmentsBlended(float angTol);
	BOOL VerticiePartialCompare(ABVERTEX v1,ABVERTEX v2);
	BOOL VerticiePartialCompare(ABVERTEX v1,ABVERTEX v2,float tolAng);
	void AddAssignmentSub(VertexAssignment * assignment,int appendVertexIndex);
	void AddAssignmentSubBlended(VertexAssignmentBlended * assignment,int appendVertexIndex);
	void AddVertexAssignment(int boneIndex,int vertexIndex);
	void AddVertexAssignmentBlended(byte * boneIndexes,float * wieghtsToBone,int boneIndexesCount,int vertexIndex);
	void AddVertexAssignmentIndi(                             int boneIndex,
											                  int vertexIndex,
															  VertexAssignment ** vReturn,
															  int *vCountReturn);
	void AddVertexAssignmentIndiBlended(       byte * boneIndexes,
													          float * wieghtsToBone,
															  ABVERTEX0L * offsetVect,
															  int boneIndexesCount,
											                  int vertexIndex,
															  VertexAssignmentBlended ** vReturn,
															  int *vCountReturn);
	void Clone(CDeformableVisObj ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void SafeDelete();
	void Serialize( CArchive& ar );
	
};

class  CDeformableVisList : public CObList
{
public:
   CDeformableVisList(){};
   
    
     
	DECLARE_SERIAL( CDeformableVisList );
	
    
    CDeformableVisObj * GetByIndex(int index);
	void Clone(CDeformableVisList ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void Optimize(float angTol);
	void Invalidate();
	void NormalizeAllSmoothUnion();
	void AvgNormals(float angTol);
	void OptimizeVertexAssignments();
	void UnifyNormals(float tol);
	void NormilizeOnly();
	
	void Normalize();
	void SafeDelete();
};

class  CDeformableMesh : public CObject
{
	
     
	DECLARE_SERIAL( CDeformableMesh );
	
    
public:
    CDeformableMesh();
    BOOL  m_publicDim;
	CString m_dimName;
	CDeformableVisList * m_lodChain;

	void Invalidate();
  
	void Load(CString filename);
    void Save(CString filename);
	BOOL SetLevel(int index,CString filename);
	BOOL SaveLevel(int index,CString filename);
	void SafeDelete();
	void Clone(CDeformableMesh ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice);	
	void Serialize( CArchive& ar );
	
};

class  CDeformableMeshList : public CObList
{
public:
   CDeformableMeshList(){};
   
    
     
	DECLARE_SERIAL( CDeformableMeshList );
	
    
    
	void SafeDelete();
};

#endif