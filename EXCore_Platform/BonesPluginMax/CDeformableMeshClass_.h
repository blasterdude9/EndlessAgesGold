#ifndef CDEFORMABLEMESHCLASS_H
#define CDEFORMABLEMESHCLASS_H

#include "CEXMeshClass.h"

struct VertexAssignment
{
    int   boneIndex;
	int   vertexIndex;
	int * vertexesCarried;//optimizing vertex movement
	int   carriedCount;
};


class CDeformableVisObj : public CObject
{

	DECLARE_SERIAL( CDeformableVisObj );

public:
    CDeformableVisObj();
    

	CEXMeshObj       * m_mesh;
	ABVERTEX         * m_originalArray;
	VertexAssignment * m_vertexAssignments;
	int                m_assignmentCount;


	void Optimize();
	void OptimizeVertexAssignments();
	BOOL VerticiePartialCompare(ABVERTEX v1,ABVERTEX v2);
	void AddAssignmentSub(VertexAssignment * assignment,
										 int appendVertexIndex);
	void AddVertexAssignment(int boneIndex,int vertexIndex);
	void AddVertexAssignmentIndi(                             int boneIndex,
											                  int vertexIndex,
															  VertexAssignment ** vReturn,
															  int *vCountReturn);
	void Clone(CDeformableVisObj ** clone);
	void SafeDelete();
	void Serialize( CArchive& ar );
	
};

class CDeformableVisList : public CObList
{
public:
   CDeformableVisList(){};
   
 
	DECLARE_SERIAL( CDeformableVisList );

    CDeformableVisObj * GetByIndex(int index);
	void Clone(CDeformableVisList ** clone);
	void Optimize();
	void OptimizeVertexAssignments();
	void Normalize();
	void SafeDelete();
};

class CDeformableMesh : public CObject
{

	DECLARE_SERIAL( CDeformableMesh );

public:
    CDeformableMesh();
    
	CDeformableVisList * m_lodChain;
  
	void Load(CString filename);
    void Save(CString filename);
	BOOL SetLevel(int index,CString filename);
	BOOL SaveLevel(int index,CString filename);
	void SafeDelete();
	void Clone(CDeformableMesh ** clone);	
	void Serialize( CArchive& ar );
	
};

class CDeformableMeshList : public CObList
{
public:
   CDeformableMeshList(){};
   
	DECLARE_SERIAL( CDeformableMeshList );

    
	void SafeDelete();
};

#endif