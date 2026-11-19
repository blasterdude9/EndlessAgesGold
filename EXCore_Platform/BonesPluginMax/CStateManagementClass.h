


#ifndef CSTATEMANAGEMENTCLASS_H
#define CSTATEMANAGEMENTCLASS_H

#include "CShaderLibClass.h"
#include "CTextureDBClass.h"
#include "matrixarb.h"

#define CLIP_MATRIX 0
#define WORLD_MATRIX 4
#define VIEW_MATRIX 8
#define EYE_VECTOR 12
#define DIFFUSE_COLOR 15
#define AMBIENT_COLOR 16
#define EMMISIVE_COLOR 17
#define MAX_POWER 18
#define POINT_LIGHT1 19
#define POINTLIGHT1_COLOR 20
#define POINT_LIGHT2 21
#define POINTLIGHT2_COLOR 22
#define POINT_LIGHT3 23
#define POINTLIGHT3_COLOR 24
#define POINT_LIGHT4 25
#define POINTLIGHT4_COLOR 26
#define POINT_LIGHT5 27
#define POINTLIGHT5_COLOR 28
#define POINT_LIGHT6 29
#define POINTLIGHT6_COLOR 30
#define POINT_LIGHT7 31
#define POINTLIGHT7_COLOR 32
#define DIR_LIGHTVECT 33
#define DIR_LIGHTCOLOR 34
#define AMBIENT_LIGHT 35
#define FOG_RANGE 36
#define DEPTH_DIMINISH 37
#define VAL_ONE 38
#define VAL_ZERO 39



class  CStateManagementObj : public CObject
{
public:
    CStateManagementObj();
    
    int m_currentDiffuseSource;
	MatrixARB m_matrixAB;
	int  m_currentVertexType;
    int  m_globalBlend;
    int  GlobalTextureIndex[10];
	int  GlobalBlendLevels[10];
	int  m_globalZBias;
	BOOL m_globalFilterFog;
	BOOL m_globalEnvFogOn;
    int  m_multiTextureSupported;
	int  m_stateChangesPerPass;
	BOOL GL_cullOn;
	BOOL GL_wireFillState;
	int GL_lastTextureWrapMode;
	BOOL m_fogOverride;
	D3DCOLOR m_oldFogColor;
	BOOL m_dotProductEnabled;
    int  m_shaderMode;
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_projMatrix;
	D3DXVECTOR3 m_locationOffset;
	BOOL m_supportBUMPENVMAP;

	D3DCAPS9 m_pCaps;



	BOOL m_specularEnable;
	CShaderObjList * m_shderSystemRef;
	int m_currentShader;
	float m_hightColorRangeStart;
    float m_hightColorRangeEnd;

	LPDIRECT3DVERTEXDECLARATION9   m_pVertexDeclaration;
	

    void SafeDelete();
	 void SetDiffuseSource(int type,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SetVertexType(int uvCount,LPDIRECT3DDEVICE9 g_pd3dDevice);
	int TextureReference(CString textureFileName,
		                 CTextureDBObjList * textureDataBase,
						 LPDIRECT3DDEVICE9 g_pd3dDevice,
						 BOOL bitmapLoadOvverride,
						 int & maxTextureSize);
	 BOOL SetBlendType(LPDIRECT3DDEVICE9 g_pd3dDevice,int type);
	 BOOL SetZBias(LPDIRECT3DDEVICE9 g_pd3dDevice,int bias);
	void SetTextureState(int textureIndex,
		                 int texturePass,
						 LPDIRECT3DDEVICE9 g_pd3dDevice,
						 CTextureDBObjList * textureDataBase);
	 BOOL SetFogState(LPDIRECT3DDEVICE9 g_pd3dDevice,BOOL status);
	 BOOL SetFogFilter(LPDIRECT3DDEVICE9 g_pd3dDevice,BOOL filterState);
	 void SetBlendLevel(int level,int type,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 BOOL SetCullState(BOOL on,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 BOOL SetWireFillState(BOOL on,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 BOOL SetTextureTileMode(int mode,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SetBlendLevelMultipass(int type,LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL SetFogOverrideState(BOOL on,LPDIRECT3DDEVICE9 g_pd3dDevice);
	BOOL InitShaderGlobals(LPDIRECT3DDEVICE9 g_pd3dDevice);
	DWORD VectortoRGBA( D3DVECTOR v, FLOAT fHeight );
	void InitCaps(LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void UpdateTFACTORVIEW(D3DVECTOR position,D3DVECTOR objectPosition,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SetWorldMatrix(D3DXMATRIX *pObject, LPDIRECT3DDEVICE9 g_pd3dDevice);
     void SetViewProjMatrix(D3DXMATRIX *pView, D3DXMATRIX *pProj, LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SetViewMatrix(D3DXMATRIX *pView, LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SetMaterial(LPDIRECT3DDEVICE9 g_pd3dDevice, D3DMATERIAL9 * material);
	 void SetSpecular(LPDIRECT3DDEVICE9 g_pd3dDevice, BOOL state);
	 void SetAmbientLight(LPDIRECT3DDEVICE9 g_pd3dDevice, float r, float g, float b);
	 void SetDirLight(LPDIRECT3DDEVICE9 g_pd3dDevice, D3DVECTOR dirVect, float r, float g, float b);
	 void SetSTLight(int index,LPDIRECT3DDEVICE9 g_pd3dDevice, D3DVECTOR position, float r, float g, float b, float rg);
	 void SetFogConstant(LPDIRECT3DDEVICE9 g_pd3dDevice, float startPoint, float endPoint);
	 void SetShader(LPDIRECT3DDEVICE9 g_pd3dDevice,CShaderObjList * shaderSystem, int ShaderIndexID);
	 void SetShaderOpt(LPDIRECT3DDEVICE9 g_pd3dDevice,int ShaderIndexID);
};

#endif