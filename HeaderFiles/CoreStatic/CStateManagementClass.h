#pragma once

#include <D3DX9.h>
#include <d3d9.h>
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
#define VAL_HALF 40
#define VAL_CAM 41
#define VAL_MISC1 50
#define VAL_MISC2 51
#define VAL_MISC3 52
#define VAL_MISC4 53
#define VAL_MISC5 54
#define VAL_WIND 55
#define VAL_CAMROTMATRIX 64
#define PROJ_M 68

class   CStateManagementObj : public CObject
{
public:
    CStateManagementObj();
    
    int m_currentDiffuseSource;
	char m_tangentExists;
	MatrixARB m_matrixAB;
	int  m_currentVertexType;
    int  m_globalBlend;
    int  GlobalTextureIndex[10];
	int  GlobalBlendLevels[10];
	char  GlobalColorArg1Levels[10];
	char  GlobalColorArg2Levels[10];
	char  GlobalTexcoordLevels[10];
	int  m_globalZBias;
	BOOL m_globalFilterFog;
	BOOL m_globalEnvFogOn;
    int  m_multiTextureSupported;
	int  m_stateChangesPerPass;
	int GL_cullOn;
	BOOL GL_wireFillState;
	int GL_lastTextureWrapMode;
	BOOL m_fogOverride;
	D3DCOLOR m_oldFogColor;
	BOOL m_dotProductEnabled;
    int  m_shaderMode;
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_projMatrix;
	D3DXMATRIX m_viewMatrixUTP;//untransposed
	D3DXMATRIX m_projMatrixUTP;//untransposed
	D3DXMATRIX m_invViewMatrix;
	D3DXMATRIX m_worldViewMatrixRegister;
	D3DXMATRIX m_worldViewProjectionMatrixRegister;
	D3DXMATRIX m_worldInverseTransposeRegister;
	D3DXVECTOR3 m_locationOffset;
	BOOL m_supportBUMPENVMAP;
	BOOL m_stencilMode;
	BOOL m_lightingMode;
	int  m_shadeMode;
	BOOL m_currentZWritableState;
	BOOL m_cirrentAlphaPixelTestState;
	BOOL m_forceWireframe;
	BOOL m_forceShadeMode;
	BOOL m_mirrorModeOn;
	float m_tempLightDir[4];
	float m_tempLightColor[4];
	float m_zero4Float[4];

	float m_maxPixShaderVs;
	float m_maxVSShaderVs;

	BOOL m_cameraSpaceEnabled;

	D3DMATERIAL9 m_lastMaterial;

	D3DCAPS9 m_pCaps;
	BOOL m_stencilBufferAble;

	float m_glAdjust;

	D3DXMATRIX m_tempMatrix;
	D3DXMATRIX m_matrixInit;

	LPD3DXEFFECT m_currentHSLeffect;
	int          m_currrentEffectPeramCountCRC;

	//maintained light states
	D3DXVECTOR3 m_lastDirLight;
	CString currentHSLcode;
	CShaderObj * m_currentPxFxShader;

	//more caps
	BOOL                   m_maxActiveLights;
	BOOL                   m_supportedStencilBuffer;

	//preset
	int                    m_intNegOne;
    int                    m_intZero;
	int                    m_int1;
	int                    m_int2;
	int                    m_int3;
	int                    m_int4;
	int                    m_int5;
	int                    m_int6;
	int                    m_int7;
	int                    m_int8;
	int                    m_int9;
	int                    m_int10;



	
	BOOL					m_specularEnable;
	CShaderObjList *		m_shderSystemRef;
	CShaderObjList *		m_pxShderSystemRef;
	int						m_currentShader;
	int						m_currentPxShader;
	float					m_hightColorRangeStart;
    float					m_hightColorRangeEnd;
	D3DVECTOR               m_miscConst1,m_miscConst2,m_miscConst3,m_miscConst4,m_miscConst5;


	LPDIRECT3DVERTEXDECLARATION9   m_pVertexSimple;
    LPDIRECT3DVERTEXDECLARATION9   m_pVertexDeclaration0UV;
	LPDIRECT3DVERTEXDECLARATION9   m_pVertexDeclaration1UV;
	LPDIRECT3DVERTEXDECLARATION9   m_pVertexDeclaration2UV;
	LPDIRECT3DVERTEXDECLARATION9   m_pVertexDeclaration3UV;
	LPDIRECT3DVERTEXDECLARATION9   m_pVertexDeclaration4UV;
	LPDIRECT3DVERTEXDECLARATION9   m_pVertexDeclaration5UV;
	LPDIRECT3DVERTEXDECLARATION9   m_pVertexDeclaration6UV;
	LPDIRECT3DVERTEXDECLARATION9   m_pVertexDeclaration7UV;
	LPDIRECT3DVERTEXDECLARATION9   m_pVertexDeclaration8UV;

	LPDIRECT3DVERTEXDECLARATION9   m_pVertexDeclaration0UVD;
	LPDIRECT3DVERTEXDECLARATION9   m_pVertexDeclaration1UVD;
	LPDIRECT3DVERTEXDECLARATION9   m_pVertexDeclaration2UVD;

	LPDIRECT3DVERTEXDECLARATION9   m_pVertexDeclaration0UVT;
	LPDIRECT3DVERTEXDECLARATION9   m_pVertexDeclaration1UVT;
	LPDIRECT3DVERTEXDECLARATION9   m_pVertexDeclaration2UVT;
	LPDIRECT3DVERTEXDECLARATION9   m_pVertexDeclaration3UVT;
	LPDIRECT3DVERTEXDECLARATION9   m_pVertexDeclaration4UVT;
	LPDIRECT3DVERTEXDECLARATION9   m_pVertexDeclaration5UVT;
	LPDIRECT3DVERTEXDECLARATION9   m_pVertexDeclaration6UVT;
	

	LPDIRECT3DVERTEXDECLARATION9   m_pVertexLeafUVx;

     void SafeDelete();
	 void SetDiffuseSource(int type,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SetVertexType(int uvCount,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 BOOL SetBlendType(LPDIRECT3DDEVICE9 g_pd3dDevice,int type);
	 BOOL SetZBias(LPDIRECT3DDEVICE9 g_pd3dDevice,int bias);
	 void SetTextureState(int & textureIndex,
		                  int & texturePass,
						  LPDIRECT3DDEVICE9 g_pd3dDevice,
						  CTextureDBObjList * textureDataBase);
	 BOOL SetFogState(LPDIRECT3DDEVICE9 g_pd3dDevice,BOOL status);
	 BOOL SetFogFilter(LPDIRECT3DDEVICE9 g_pd3dDevice,BOOL filterState);
	 void SetBlendLevel(int & level,int & type,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 BOOL SetCullState(int onType,LPDIRECT3DDEVICE9 g_pd3dDevice);
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
	 void SetProjUVGenMatrix(D3DXMATRIX projection,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SetViewMatrix(D3DXMATRIX *pView, LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SetMaterial(LPDIRECT3DDEVICE9 g_pd3dDevice, D3DMATERIAL9 * material);
	 void SetSpecular(LPDIRECT3DDEVICE9 g_pd3dDevice, BOOL state);
	 void SetAmbientLight(LPDIRECT3DDEVICE9 g_pd3dDevice, float r, float g, float b);
	 void SetDirLight(LPDIRECT3DDEVICE9 g_pd3dDevice, D3DVECTOR dirVect, float r, float g, float b);
	 void SetSTLight(int & index,LPDIRECT3DDEVICE9 g_pd3dDevice, D3DVECTOR & position, float & r, float & g, float & b, float & rg);
	 void SetFogConstant(LPDIRECT3DDEVICE9 g_pd3dDevice, float startPoint, float endPoint);
	 void SetShader(LPDIRECT3DDEVICE9 g_pd3dDevice,CShaderObjList * shaderSystem, int ShaderIndexID);
	 void SetStencilMode(BOOL state,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SetLightingMode(BOOL state,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SetShadeMode(int state,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SetColorArg2Level(int & level,char & type,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SetColorArg1Level(int & level,char & type,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SetPxShader(LPDIRECT3DDEVICE9 g_pd3dDevice,CShaderObjList * shaderSystem, int ShaderIndexID);
	 void SetViewProjMatrixOnly(D3DXMATRIX *pProj, LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SetZWritableState(BOOL state,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SetAlphaPixelTest(BOOL state,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SetCameraSpace(BOOL enabled,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SetWindGlobal(D3DVECTOR curPush,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SetCamMatrix(D3DXMATRIX *cMat, LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void InitializeAllStates(LPDIRECT3DDEVICE9 pd3dDevice);
	 int m_anisoMode;//anisotropic filtering: 0=off 1=2x 2=4x 3=8x 4=16x (pushed from startcfg.aut by the client)
	 void SetGlobalWireFrameMode(BOOL enable,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SetTexcoordIndexLevel(int & level,char & type,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void StopSTLight(int & index,LPDIRECT3DDEVICE9 g_pd3dDevice);
	 void SetSTLightCustom(int & constantIndex,LPDIRECT3DDEVICE9 g_pd3dDevice, D3DVECTOR & position, float & r, float & g, float & b, float & rg);
};
