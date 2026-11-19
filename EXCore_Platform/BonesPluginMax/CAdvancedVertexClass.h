#include "CStructuresMisl.h"
#ifndef CADVANCEDVERTEXCLASS_H
#define CADVANCEDVERTEXCLASS_H

class  CAdvancedVertexObj : public CObject
{
	
     
	DECLARE_SERIAL( CAdvancedVertexObj );
	
    

public:
    CAdvancedVertexObj();
    
	int                        m_uvCount;
	DWORD                      m_lockFlags;
	int                        m_advancedFixedMode;
    
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
	 void Render(LPDIRECT3DDEVICE9 g_pd3dDevice,int & vertexType);
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
	 void FillD3DVECTORArrayData(D3DVECTOR * array);
	D3DCOLOR DiffuseGradiant(float & curY,float & startRed, float & startGreen, float & startBlue,
											              float & endRed, float & endGreen, float & endBlue,
														  float & constantEffectStart,float & constantEffectEnd);
	
	
};

class  CAdvancedVertexObjList : public CObList
{
public:
	CAdvancedVertexObjList(){};
   void SafeDelete();
  
    
     
	DECLARE_SERIAL( CAdvancedVertexObjList );
	
    


	//void Render(LPDIRECT3DDEVICE9 g_pd3dDevice);
};

#endif