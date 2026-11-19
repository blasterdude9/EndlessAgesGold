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

struct VertexAssignmentBldTgt
{
	byte    boneIndexesCount;
	ABVERTEX0Lb * offsetVects;
    byte   *boneIndexes;
	float  *weightsToBone;
	short   vertexIndex;
	short * vertexesCarried;//optimizing vertex movement
	byte   carriedCount;
};


class CDeformableVisObj : public CObject
{
	
     
	DECLARE_SERIAL( CDeformableVisObj );
	
    
public:
    CDeformableVisObj();
    

	CEXMeshObj              * m_mesh;
	ABVERTEX0L              * m_originalArrayVector;
	VertexAssignment        * m_vertexAssignments;
	VertexAssignmentBlended * m_blendedVertexAssignments;
	VertexAssignmentBldTgt  * m_blendedVertexAssignmentsWt;
	int                       m_assignmentCount;
	BOOL                      m_referencedMemory;
	int                       m_blendedType;
	


	void Invalidate();
	void Optimize();
	void OptimizeVertexAssignments();
	BOOL VerticiePartialCompare(ABVERTEX v1,ABVERTEX v2);
	void AddAssignmentSub(VertexAssignment * assignment,int appendVertexIndex);
	void AddVertexAssignment(int boneIndex,int vertexIndex);
	void AddVertexAssignmentBlended(byte * boneIndexes,float * wieghtsToBone,int boneIndexesCount,int vertexIndex);
	void AddVertexAssignmentIndi(                             int boneIndex,
											                  int vertexIndex,
															  VertexAssignment ** vReturn,
															  int *vCountReturn);
	void Clone(CDeformableVisObj ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void CloneAllRaw(CDeformableVisObj ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void SafeDelete();
	void Serialize( CArchive& ar );
	
};

class CDeformableVisList : public CObList
{
public:
   CDeformableVisList(){};
   
    
     
	DECLARE_SERIAL( CDeformableVisList );
	
    
    CDeformableVisObj * GetByIndex(int index);
	void Clone(CDeformableVisList ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void CloneAllRaw(CDeformableVisList ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void Optimize();
	void Invalidate();
	void OptimizeVertexAssignments();
	void Normalize();
	void SafeDelete();

	void DeleteByIndex(int index)
	{
		int trace = 0;
		POSITION posLast = NULL;
         for(POSITION posLoc = GetHeadPosition();(posLast=posLoc) != NULL;)
		 {
             CDeformableVisObj * vObj = (CDeformableVisObj *)GetNext(posLoc);
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
};

CString PadAltIds(CString* strID);

class CDeformableMesh : public CObject
{
	friend CString PadAltIds(CString* strID);   
	DECLARE_SERIAL( CDeformableMesh );
   
public:
    CDeformableMesh();
    BOOL  m_publicDim;
	CDeformableVisList * m_lodChain;
	BOOL                 m_collisionFiltered;
	void Invalidate();

	inline CString DimName() 
	{ 
		if (this->m_dimName != "") 
			return PadAltIds(&this->m_dimName); 
		else
			return this->m_dimName; 
	}
  
	CDeformableVisObj * GetLOD(int & lod, int & overrideLod);
	void Load(CString filename);
    void Save(CString filename);
	BOOL SetLevel(int index,CString filename);
	BOOL SaveLevel(int index,CString filename);
	void SafeDelete();
	void Clone(CDeformableMesh ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice);	
	void CloneAllRaw(CDeformableMesh ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void Serialize( CArchive& ar );
	
public: // was private; editor dialog reads raw m_dimName
	CString m_dimName;
};

class CDeformableMeshList : public CObList
{
public:
   CDeformableMeshList(){};
   
    
     
	DECLARE_SERIAL( CDeformableMeshList );
	
    
    
	void SafeDelete();
	CDeformableMesh * GetByName(CString altName);
};

#endif