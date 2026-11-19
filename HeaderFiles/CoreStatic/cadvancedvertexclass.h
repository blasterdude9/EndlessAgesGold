#include "CStructuresMisl.h"
#ifndef CADVANCEDVERTEXCLASS_H
#define CADVANCEDVERTEXCLASS_H


struct edge_struct
{
    WORD v1, v2;
};


class CAdvancedVertexObj : public CObject
{
	
     
	DECLARE_SERIAL( CAdvancedVertexObj );
	
    

public:
    CAdvancedVertexObj();
	~CAdvancedVertexObj() { this->SafeDelete(); }
    
	int                        m_uvCount;
	DWORD                      m_lockFlags;
	int                        m_advancedFixedMode;
	char                       m_tangentExists;
    
	//original data
    //ABVERTEX   *               m_origVertexArray;
	ABVERTEX0L *               m_vertexArray0;
    ABVERTEX1L *               m_vertexArray1;    
    ABVERTEX2L *               m_vertexArray2;
    ABVERTEX3L *               m_vertexArray3;
    ABVERTEX4L *               m_vertexArray4;
	ABVERTEX5L *               m_vertexArray5;
    ABVERTEX6L *               m_vertexArray6;
    ABVERTEX7L *               m_vertexArray7;
    ABVERTEX8L *               m_vertexArray8;

	ABVERTEX0Lb *              m_vertexArray0t;
    ABVERTEX1Lb *              m_vertexArray1t;    
    ABVERTEX2Lb *              m_vertexArray2t;
    ABVERTEX3Lb *              m_vertexArray3t;
    ABVERTEX4Lb *              m_vertexArray4t;
	ABVERTEX5Lb *              m_vertexArray5t;
	ABVERTEX6Lb *              m_vertexArray6t;

	ABVERTEX1Lx *              m_vertexArray1x;

	/*ABVERTEX0La *              m_vertexArray0a;
    ABVERTEX1La *              m_vertexArray1a;    
    ABVERTEX2La *              m_vertexArray2a;
    ABVERTEX3La *              m_vertexArray3a;
    ABVERTEX4La *              m_vertexArray4a;*/

	//buffers
	
	LPDIRECT3DVERTEXBUFFER9    m_vertexBuffer;
	IDirect3DIndexBuffer9     *m_indicieBuffer;
	//fixed data
    WORD                      *m_indecieArray;
    WORD                       m_indecieCount;
	WORD                       m_vertexCount; 
  
	void Serialize( CArchive& ar );
	void SafeDelete();
	void Invalidate();
	 void CacheUnimediateData();
	 int Render(LPDIRECT3DDEVICE9 g_pd3dDevice,int & vertexType);
	 void InitFromData(WORD vCount,
							 WORD indecieCount,
							 ABVERTEX * vArray,
							 WORD * indecieArray,
							 int uvCount,
							 LPDIRECT3DDEVICE9 g_pd3dDevice
							 );
	 void InitBuffer(LPDIRECT3DDEVICE9 g_pd3dDevice,int requiredUVCount);
	 void InitBuffer(LPDIRECT3DDEVICE9 g_pd3dDevice,int effect, int requiredUVCount);
	 void ApplyMoveTexture(float &u,
												 float &v,
												 int   textureSet,
												 float &adjustfactor,
												 int & vertexType);
	 void ApplySphereAlgorithm(D3DMATRIX matWV,int & vertexType,int uvSet);
    void CloneMinimum2(CAdvancedVertexObj * clone2,LPDIRECT3DDEVICE9 g_pd3dDevice);
	void CloneDataOnly(CAdvancedVertexObj * clone2);
	 void CloneMinimum(CAdvancedVertexObj ** clone,LPDIRECT3DDEVICE9 g_pd3dDevice);
	
	 void SetPositionXYZ(int & vIndex,float & x,float & y,float & z);
	 void MoveTextureReset(float &u,
								 float &v,
								 float &adjustfactor,
								 TXTUV &uvStruct,
								 TXTUV &origUvStruct,
								 BOOL  &resetU,
								 BOOL  &resetV);

	 void SphereMapL1(D3DMATRIX matWV,int uvSetIndex);
     void SphereMapL2(D3DMATRIX matWV,int uvSetIndex);
	 void SphereMapL3(D3DMATRIX matWV,int uvSetIndex);
     void SphereMapL4(D3DMATRIX matWV,int uvSetIndex);
	 void SphereMapL5(D3DMATRIX matWV,int uvSetIndex);
     void SphereMapL6(D3DMATRIX matWV,int uvSetIndex);
	 void SphereMapL7(D3DMATRIX matWV,int uvSetIndex);
     void SphereMapL8(D3DMATRIX matWV,int uvSetIndex);
	 void SphereMapL1a(D3DMATRIX matWV,int uvSetIndex);
	 void SphereMapL2a(D3DMATRIX matWV,int uvSetIndex);
	 void SphereMapL3a(D3DMATRIX matWV,int uvSetIndex);

	 void MoveTextureL1(float &u,
							  float &v,
							  int &textureSet,
							  float &adjustfactor);
	 void MoveTextureL2(float &u,
									  float &v,
									  int &textureSet,
									  float &adjustfactor);
	 void MoveTextureL3(float &u,
									  float &v,
									  int &textureSet,
									  float &adjustfactor);
	 void MoveTextureL4(float &u,
									  float &v,
									  int &textureSet,
									  float &adjustfactor);
	 void MoveTextureL5(float &u,
									  float &v,
									  int &textureSet,
									  float &adjustfactor);
	 void MoveTextureL6(float &u,
									  float &v,
									  int &textureSet,
									  float &adjustfactor);
	 void MoveTextureL7(float &u,
									  float &v,
									  int &textureSet,
									  float &adjustfactor);
	 void MoveTextureL8(float &u,
									  float &v,
									  int &textureSet,
									  float &adjustfactor);
	 void MoveTextureL1a(float &u,
									  float &v,
									  int &textureSet,
								      float &adjustfactor);

	 void MoveTextureL2a(float &u,
									           float &v,
									           int &textureSet,
									           float &adjustfactor);
	//void  ReinitializeDxVbuffers(IDirect3DDevice9 * pd3dDevice);
	 BOOL GetVertexTraverseData(byte ** vBufferData, int * XYZstep, int * vertexStep, long * datasize);
	void  ReinitializeDxVbuffers(IDirect3DDevice9 * pd3dDevice,
												D3DPOOL memManage,
												DWORD bufUsage,
												DWORD vUsage,
												int targetUVcount);
	void ReinitializeDxVbuffersAdvanced(IDirect3DDevice9 * pd3dDevice,
									    D3DPOOL memManage,
									    DWORD bufUsage,
									    DWORD vUsage,
										int effect);
	void  ReinitializeDxVbuffersPositionOnly(IDirect3DDevice9 * pd3dDevice);
	void  ReinitializeDxVbuffersPosNormOnly(IDirect3DDevice9 * pd3dDevice);
	 void InitBufferDyn(LPDIRECT3DDEVICE9 g_pd3dDevice, int requiredUVCount);
	 void SetABVERTEXStyle(IDirect3DDevice9 * pd3dDevice,ABVERTEX * vArray,int vertexCount);
	 void GetABVERTEXStyle(ABVERTEX ** vArray,int * vertexCount);
	 void SetPositionXYZNXNYNZ(int & vIndex,
											   float & x,float & y,float & z,
											   float & nx,float & ny,float & nz);
	 void SetPositionXYZNXNYNZMEM(int  vIndex,
											   float  x,float  y,float  z,
											   float  nx,float  ny,float  nz);
	 void Fill0UVArrayData(ABVERTEX0L * array);
	 void AddEdge( WORD* pEdges, DWORD& dwNumEdges, WORD v0, WORD v1 );
	 void AddEdge2( edge_struct* pEdges, DWORD& dwNumEdges, WORD & v0, WORD & v1 );
	 void FillD3DVECTORArrayData(D3DVECTOR * array);
	 void MakeIntoComplexShadowVolume(IDirect3DDevice9 * pd3dDevice,
								      D3DXVECTOR3 light,
									  D3DXVECTOR3 * returnBuffer,
									  int * vertexCount,
									  float maxDepth);
	 void MakeIntoComplexShadowVolume2(IDirect3DDevice9 * pd3dDevice,
								      D3DXVECTOR3 light,
									  D3DXVECTOR3 * returnBuffer,
									  DWORD * vertexCount,
									  float maxDepth);
	 void MakeIntoComplexShadowVolume3(IDirect3DDevice9 * pd3dDevice,
													 D3DXVECTOR3 light,
													 D3DXVECTOR3 * returnBuffer,
													 DWORD * vertexCount,
													 float maxDepth,
													 WORD* pEdges);
	 void MakeIntoSimpleShadowVolume(IDirect3DDevice9 * pd3dDevice,
									 float max,
									 float min);
	D3DCOLOR DiffuseGradiant(float & curY,float & startRed, float & startGreen, float & startBlue,
											              float & endRed, float & endGreen, float & endBlue,
														  float & constantEffectStart,float & constantEffectEnd);
	void GetShadowVolumeMesh(DWORD dwNumFaces,
											 D3DXVECTOR3 light,
											 D3DXVECTOR3 * returnBuffer,
											 D3DXVECTOR3 * faceBuffer,
											 DWORD * vertexCount,
											 WORD * pEdges);
	void GetShadowVolumeMesh2(DWORD dwNumFaces,
											 D3DXVECTOR3 light,
											 D3DXVECTOR3 * returnBuffer,
											 D3DXVECTOR3 * faceBuffer,
											 DWORD * vertexCount,
											 edge_struct * pEdges);
	void GetShadowVolumeMesh3(DWORD dwNumFaces,
											  D3DXVECTOR3 light,
											  D3DXVECTOR3 * returnBuffer,
											  ABSPECFACE * faceBuffer,
											  DWORD * vertexCount,
											  edge_struct * pEdges,
											  WORD * indecieBuffer);
	void GetShadowVolumeMesh4(DWORD dwNumFaces,
											  D3DXVECTOR3 light,
											  D3DXVECTOR3 * returnBuffer,
											  ABSPECFACE * faceBuffer,
											  DWORD * vertexCount,
											  edge_struct * pEdges,
											  WORD * indecieBuffer);
	void GetFaceData(D3DXVECTOR3 * faceBuffer,DWORD * vCount);
	float GetVertexX(int  i);
	float GetVertexY(int  i);
	float GetVertexZ(int  i);
	float GetVertexNX(int  i);
	float GetVertexNY(int  i);
	float GetVertexNZ(int  i);
	void GetFaceDataIndecie(ABSPECFACE * faceBuffer,
									         DWORD * vCount,
											 DWORD * indecieCount,
                                             WORD * indecieBuffer
									         );
	D3DVECTOR CalculateTangentVector(D3DVERTEX & vector2,D3DVERTEX & vector3);
	void GenerateTangents(LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL IntegrityCheck();
	void SetPositionXYZNXNYNZTGT(int & vIndex,
								 float & x,float & y,float & z,
								 float & nx,float & ny,float & nz,
								 float & tx,float & ty,float & tz);
	void AVGTangents(float smoothBasisAngle);
	D3DVECTOR GetTangentVector(int vIndex);
	BOOL GenerateTangentsPerfect(LPDIRECT3DDEVICE9 g_pd3dDevice,BOOL refresh,int basis);
	void AVGNormals(float smoothBasisAngle);
	void UnifyNormals(float angleTolerance);
	void NormalizeOnly();
	int GetPolyCount();
	void GetPolygon(int & indecieStart,D3DVECTOR * poly);
	D3DVECTOR GetNormal(int i);
	int GetVertexSize();
	void * GetVertexBuf();
	void ReinitializeDxVbuffersFS(IDirect3DDevice9 * pd3dDevice);
	
};

class CAdvancedVertexObjList : public CObList
{
public:
	CAdvancedVertexObjList(){};
   void SafeDelete();
  
    
     
	DECLARE_SERIAL( CAdvancedVertexObjList );
	
    


	//void Render(LPDIRECT3DDEVICE9 g_pd3dDevice);
};

#endif