#include "stdafx.h"
#define D3D_OVERLOADS

#include <D3DX9.h>
#include <d3d9.h>
#include <afxtempl.h>
#include <direct.h>
#include <math.h>
#include "CStateManagementClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CStateManagementObj::CStateManagementObj()
{
	ZeroMemory(m_zero4Float,sizeof(float)*4);
	m_forceShadeMode           = FALSE;
	m_forceWireframe           = FALSE;
    m_globalBlend              = -1;
	m_globalZBias              = 0;
	m_globalFilterFog          = FALSE;
	m_globalEnvFogOn           = FALSE;
	m_multiTextureSupported    = 0;
	m_stateChangesPerPass      = 0;
	GL_cullOn                  = TRUE;
	GL_wireFillState           = FALSE;
	GL_lastTextureWrapMode     = 0;
	m_fogOverride              = FALSE;
	m_oldFogColor              = D3DCOLOR_XRGB(0,0,0);
	m_currentVertexType        = -1;
	m_dotProductEnabled        = FALSE;
	m_shaderMode               = 0;
    m_mirrorModeOn             = FALSE;
	m_anisoMode                = 4;//default: 16x anisotropic filtering when the adapter supports it
	m_locationOffset.x         = 0.0f;
	m_locationOffset.y         = 0.0f;
	m_locationOffset.z         = 0.0f;

	m_specularEnable			= FALSE;
	m_shderSystemRef			= NULL;//reference
	m_currentShader				= -99;
	m_currentPxShader			= -99;
    m_hightColorRangeStart		= 0;
    m_hightColorRangeEnd		= 200;
	m_currentDiffuseSource		= 0;
	m_supportBUMPENVMAP			= FALSE;
	m_stencilMode				= FALSE;
	m_stencilBufferAble			= FALSE;
	m_lightingMode				= TRUE;
	m_shadeMode					= 0;
    m_tangentExists				= 0;
    m_glAdjust					= 1.0f;
	m_currentHSLeffect			= NULL;
	m_intNegOne					= -1;
    m_intZero					= 0;
	m_int1						= 1;
	m_int2						= 2;
	m_int3						= 3;
	m_int4						= 4;
	m_int5						= 5;
	m_int6						= 6;
	m_int7						= 7;
	m_int8						= 8;
	m_int9						= 9;
	m_int10						= 10;

	m_maxPixShaderVs           = 0.0f;
	m_maxVSShaderVs            = 0.0f;

	m_cirrentAlphaPixelTestState = FALSE;
	m_currentZWritableState = TRUE;
	m_cameraSpaceEnabled = FALSE;


	ZeroMemory(&m_matrixInit,  sizeof(D3DMATRIX));
	m_matrixInit._11    = m_matrixInit._22  = m_matrixInit._33  = m_matrixInit._44  = 1.0f;

	m_invViewMatrix = m_matrixInit;

	ZeroMemory(&m_lastMaterial,sizeof(D3DMATERIAL9));

	m_pVertexDeclaration1UVT = NULL;
	m_pVertexDeclaration2UVT = NULL;
	m_pVertexDeclaration3UVT = NULL;
	m_pVertexDeclaration4UVT = NULL;
	m_pVertexDeclaration5UVT = NULL;
	m_pVertexDeclaration6UVT = NULL;
	m_pVertexLeafUVx         = NULL;
	m_pVertexSimple          = NULL;
	
    m_lastDirLight.x = 0.0f;
	m_lastDirLight.y = 0.0f;
	m_lastDirLight.z = 0.0f;
	

	for(int loop=0;loop<10;loop++){
        GlobalTextureIndex[loop] = -1;
		GlobalBlendLevels[loop] = -1;
		GlobalColorArg1Levels[loop] = 50;
		GlobalColorArg2Levels[loop] = 50;
	}
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CStateManagementObj::InitCaps(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
   g_pd3dDevice->GetDeviceCaps(&m_pCaps);

   //m_pCaps.TextureOpCaps = 0;

   if(m_pCaps.TextureOpCaps & D3DTEXOPCAPS_DOTPRODUCT3)
      m_dotProductEnabled = TRUE;

   if ( m_pCaps.TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAP)
        m_supportBUMPENVMAP = TRUE;

   //reset
   for(int loop=0;loop<10;loop++){
        GlobalTextureIndex[loop] = -1;
		GlobalBlendLevels[loop] = -1;
		GlobalColorArg1Levels[loop] = 50;
		GlobalColorArg2Levels[loop] = 50;
   }
   //init shader constants
   D3DVECTOR initVect;
   ZeroMemory(&initVect,sizeof(D3DVECTOR));
   SetWindGlobal(initVect,g_pd3dDevice);

   //allocate custom vertex formats


   if(!m_pVertexDeclaration1UVT)
   { 
    D3DVERTEXELEMENT9 declU0_c[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        D3DDECL_END()
    };
    HRESULT error = g_pd3dDevice->CreateVertexDeclaration( declU0_c, &m_pVertexSimple );

    D3DVERTEXELEMENT9 declU0[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   //v3
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration( declU0, &m_pVertexDeclaration0UV );

    D3DVERTEXELEMENT9 declU1[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   //v3
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //v7
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration( declU1, &m_pVertexDeclaration1UV );
    D3DVERTEXELEMENT9 declU2[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   //v3
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //v7
		{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration( declU2, &m_pVertexDeclaration2UV );

	D3DVERTEXELEMENT9 declU3[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   //v3
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //v7
		{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration( declU3, &m_pVertexDeclaration3UV );

	D3DVERTEXELEMENT9 declU4[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   //v3
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //v7
		{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration( declU4, &m_pVertexDeclaration4UV );

	D3DVERTEXELEMENT9 declU5[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   //v3
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //v7
		{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
		{ 0, 56, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration( declU5, &m_pVertexDeclaration5UV );

	D3DVERTEXELEMENT9 declU6[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   //v3
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //v7
		{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
		{ 0, 56, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
		{ 0, 64, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration( declU6, &m_pVertexDeclaration6UV );
	
	D3DVERTEXELEMENT9 declU7[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   //v3
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //v7
		{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
		{ 0, 56, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
		{ 0, 64, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },
		{ 0, 72, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 6 },
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration( declU7, &m_pVertexDeclaration7UV );

    D3DVERTEXELEMENT9 declU8[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   //v3
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //v7
		{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
		{ 0, 56, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
		{ 0, 64, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },
		{ 0, 72, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 6 },
		{ 0, 80, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 7 },
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration( declU8, &m_pVertexDeclaration8UV );
  //-------------
	D3DVERTEXELEMENT9 declU0D[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   //v3
		{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },   //v3
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration( declU0D, &m_pVertexDeclaration0UVD );

    D3DVERTEXELEMENT9 declU1D[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   //v3
		{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },   //v3
        { 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //v7
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration( declU1D, &m_pVertexDeclaration1UVD );
    D3DVERTEXELEMENT9 declU2D[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   //v3
		{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },   //v3
        { 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //v7
		{ 0, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration( declU2D, &m_pVertexDeclaration2UVD );

   
   D3DVERTEXELEMENT9 decl0[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   //v3
        { 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },  //v8
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration( decl0, &m_pVertexDeclaration0UVT );
   D3DVERTEXELEMENT9 decl1[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   //v3
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //v7
        { 0, 32, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },  //v8
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration( decl1, &m_pVertexDeclaration1UVT );
   D3DVERTEXELEMENT9 decl2[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   //v3
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //v7
		{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
        { 0, 40, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },  //v8
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration( decl2, &m_pVertexDeclaration2UVT );
	D3DVERTEXELEMENT9 decl3[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   //v3
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //v7
		{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
        { 0, 48, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },  //v8
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration( decl3, &m_pVertexDeclaration3UVT );
	D3DVERTEXELEMENT9 decl4[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   //v3
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //v7
		{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
        { 0, 56, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },  //v8
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration( decl4, &m_pVertexDeclaration4UVT );
	D3DVERTEXELEMENT9 decl5[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   //v3
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //v7
		{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
		{ 0, 56, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
        { 0, 64, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },  //v8
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration( decl5, &m_pVertexDeclaration5UVT );
	D3DVERTEXELEMENT9 decl6[] =
    {
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   //v3
        { 0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //v7
		{ 0, 32, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
		{ 0, 40, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2 },
		{ 0, 48, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 3 },
		{ 0, 56, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 4 },
		{ 0, 64, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 5 },
        { 0, 72, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TANGENT, 0 },  //v8
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration( decl6, &m_pVertexDeclaration6UVT );
    D3DVERTEXELEMENT9 declLeafDecl[] =
    {
		/*{ 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
        { 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },   //v3
		{ 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 },   //v3
        { 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 }, //v7
		{ 0, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
        D3DDECL_END()*/
        { 0, 0, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },  //v0
		{ 0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0 },
        { 0, 24, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0 }, //v7
		{ 0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0 },
		{ 0, 36, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1 },
        D3DDECL_END()
    };
    g_pd3dDevice->CreateVertexDeclaration( declLeafDecl, &m_pVertexLeafUVx );


   }

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------------sets a texture---------------------------------------------------//
void CStateManagementObj::SafeDelete()
{
    if(m_pVertexDeclaration1UVT)
	{
       m_pVertexDeclaration1UVT->Release();
	   m_pVertexDeclaration1UVT=0;
	}

	if(m_pVertexDeclaration2UVT)
	{
       m_pVertexDeclaration2UVT->Release();
	   m_pVertexDeclaration2UVT=0;
	}

	if(m_pVertexDeclaration3UVT)
	{
       m_pVertexDeclaration3UVT->Release();
	   m_pVertexDeclaration3UVT=0;
	}
	if(m_pVertexDeclaration4UVT)
	{
       m_pVertexDeclaration4UVT->Release();
	   m_pVertexDeclaration4UVT=0;
	}
	if(m_pVertexDeclaration5UVT)
	{
       m_pVertexDeclaration5UVT->Release();
	   m_pVertexDeclaration5UVT=0;
	}
	if(m_pVertexDeclaration6UVT)
	{
       m_pVertexDeclaration6UVT->Release();
	   m_pVertexDeclaration6UVT=0;
	}
	if(m_pVertexLeafUVx)
	{
       m_pVertexLeafUVx->Release();
	   m_pVertexLeafUVx=0;
	}

	if(m_pVertexSimple)
	{
       m_pVertexSimple->Release();
	   m_pVertexSimple = 0;
	}
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CStateManagementObj::SetLightingMode(BOOL state,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	if(m_lightingMode == state)
	   return;

	if(state == FALSE)
	 {//disable
        g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
        m_lightingMode = FALSE;
	 }//end disable
	 else{//enable
        g_pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
        m_lightingMode = TRUE;
	 }//end enable
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CStateManagementObj::SetShadeMode(int state,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	if(m_shadeMode == state)
	   return;

	if(state == 0)
	{//disable
        g_pd3dDevice->SetRenderState( D3DRS_SHADEMODE,D3DSHADE_GOURAUD );
        m_shadeMode = 0;
	}//end disable
	else if(state == 1){//enable
        g_pd3dDevice->SetRenderState( D3DRS_SHADEMODE,D3DSHADE_FLAT);
        m_shadeMode = 1;
	}//end enable
	else if(state == 2){//enable
        g_pd3dDevice->SetRenderState( D3DRS_SHADEMODE,D3DSHADE_PHONG);
        m_shadeMode = 2;
	}//end enable
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------------sets a texture---------------------------------------------------//
void CStateManagementObj::SetStencilMode(BOOL state,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     if(!m_stencilBufferAble)
	     return;

     if(state == m_stencilMode)
	    return;

     if(state == FALSE)
	 {//disable
        g_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    FALSE );
        m_stencilMode = FALSE;
	 }//end disable
	 else{//enable
        g_pd3dDevice->SetRenderState( D3DRS_STENCILENABLE,    TRUE );
        m_stencilMode = TRUE;
	 }//end enable

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------------sets a texture---------------------------------------------------//
void CStateManagementObj::SetTextureState(int & textureIndex,
										  int & texturePass,
										  LPDIRECT3DDEVICE9 g_pd3dDevice,
										  CTextureDBObjList * textureDataBase)
{
      if(GlobalTextureIndex[texturePass] == textureIndex)
         return;

          if(textureIndex == -1 || m_forceShadeMode == TRUE){
             g_pd3dDevice->SetTexture( texturePass,0);//no texture
             GlobalTextureIndex[texturePass] = textureIndex;
             m_stateChangesPerPass++;
             return;
		  }
          //POSITION texPositionLoc = textureDataBase->FindIndex(textureIndex);
          if(textureIndex < textureDataBase->directAccessObjectCount){//dont render first round with textures
             //CTextureDBObj * txtPtrLoc = (CTextureDBObj *)textureDataBase->GetAt(texPositionLoc);
			  //float peramTest = .001f;
			  textureDataBase->directAccess[textureIndex].object->m_textureObject->Callback(g_pd3dDevice,texturePass,textureDataBase->directAccess[textureIndex].object->m_bumpType,textureDataBase->directAccess[textureIndex].object->m_peramameter1);
             g_pd3dDevice->SetTexture( texturePass,textureDataBase->directAccess[textureIndex].object->m_textureObject->GetSurface());//select texture
             GlobalTextureIndex[texturePass] = textureIndex;
             if(texturePass==0)
                m_stateChangesPerPass++;
                return;
		  }
		  else{
             g_pd3dDevice->SetTexture( texturePass,0);//no texture
             GlobalTextureIndex[texturePass] = -1;
             m_stateChangesPerPass++;
		  }
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CStateManagementObj::SetBlendLevel(int & level, int & type, LPDIRECT3DDEVICE9 g_pd3dDevice)
{

  if(GlobalBlendLevels[level] == type)
     return;

     GlobalBlendLevels[level] = type;

  switch(type){
	case -1:
	  g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLOROP, D3DTOP_DISABLE );
	  break;
    case 0:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLOROP, D3DTOP_MODULATE);
	  break;
	case 1:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLOROP, D3DTOP_ADD);
	  break;
	case 2:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLOROP, D3DTOP_SUBTRACT);
	  break;
	case 3:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLOROP, D3DTOP_ADDSMOOTH);	
	  break;
	case 4:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLOROP, D3DTOP_ADDSIGNED);	
	  break;
	case 5:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLOROP, D3DTOP_MODULATE2X);
	  break;
    case 6:
	  if(m_dotProductEnabled)
         g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLOROP, D3DTOP_DOTPRODUCT3);
	  else
	     g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLOROP, D3DTOP_MODULATE);
	  break;
	case 7:
		if(m_supportBUMPENVMAP)
         g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLOROP, D3DTOP_BUMPENVMAP);
		else
	     g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLOROP, D3DTOP_MODULATE);
	  break;
	case 8:
         g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	  break;
    case 9:
         g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLOROP, D3DTOP_SELECTARG2);
	  break;
    case 10:
         g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLOROP, D3DTOP_MODULATE4X);
	  break;
    case 11:
         g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLOROP, D3DTOP_ADDSIGNED2X);
	  break;
    case 12:
         g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLOROP, D3DTOP_PREMODULATE);
	  break;
    case 13:
         g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLOROP, D3DTOP_MULTIPLYADD);
	  break;
      
  }
  return;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CStateManagementObj::SetColorArg2Level(int & level,char & type,LPDIRECT3DDEVICE9 g_pd3dDevice)
{

  if(GlobalColorArg2Levels[level] == type)
     return;

     GlobalColorArg2Levels[level] = type;

  switch(type){
    case 0:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
	  break;
	case 1:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG2, D3DTA_CURRENT);
	  break;
	case 2:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG2, D3DTA_TEXTURE);
	  break;
	case 3:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG2, D3DTA_TFACTOR);	
	  break;
	case 4:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG2, D3DTA_SPECULAR);	
	  break;
	case 5:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG2, D3DTA_SELECTMASK);
	  break;
	case 6:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG2, D3DTA_DIFFUSE|D3DTA_COMPLEMENT);//inverse diffuse
	  break;
    case 7:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG2, D3DTA_CURRENT|D3DTA_COMPLEMENT);//inverse current
	  break;
    case 8:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG2, D3DTA_TEMP|D3DTA_COMPLEMENT);//inverse temp
	  break;
    case 9:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG2, D3DTA_TEXTURE|D3DTA_COMPLEMENT);//inverse texture
	  break;
    case 10:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG2, D3DTA_TEXTURE|D3DTA_ALPHAREPLICATE);//texture with alpha
	  break;
      
  }
  return;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CStateManagementObj::SetTexcoordIndexLevel(int & level,char & type,LPDIRECT3DDEVICE9 g_pd3dDevice)
{

  if(GlobalTexcoordLevels[level] == type)
     return;

     GlobalTexcoordLevels[level] = type;

  switch(type){
    case 0:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_TEXCOORDINDEX, level);
	  break;
	case 1:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACENORMAL);
	  break;
	case 2:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEPOSITION);
	  break;
	case 3:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_CAMERASPACEREFLECTIONVECTOR);	
	  break;
	case 4:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_TEXCOORDINDEX, D3DTSS_TCI_SPHEREMAP);	
	  break;
  }
  return;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CStateManagementObj::SetColorArg1Level(int & level,char & type,LPDIRECT3DDEVICE9 g_pd3dDevice)
{

  if(GlobalColorArg1Levels[level] == type)
     return;

     GlobalColorArg1Levels[level] = type;

  switch(type){
    case 0:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG1, D3DTA_DIFFUSE);
	  break;
	case 1:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG1, D3DTA_CURRENT);
	  break;
	case 2:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	  break;
	case 3:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG1, D3DTA_TFACTOR);	
	  break;
	case 4:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG1, D3DTA_SPECULAR);	
	  break;
	case 5:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG1, D3DTA_SELECTMASK);
	  break;
	case 6:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG1, D3DTA_DIFFUSE|D3DTA_COMPLEMENT);//inverse diffuse
	  break;
    case 7:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG1, D3DTA_CURRENT|D3DTA_COMPLEMENT);//inverse current
	  break;
    case 8:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG1, D3DTA_TEMP|D3DTA_COMPLEMENT);//inverse temp
	  break;
    case 9:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG1, D3DTA_TEXTURE|D3DTA_COMPLEMENT);//inverse texture
	  break;
    case 10:
      g_pd3dDevice->SetTextureStageState(level, D3DTSS_COLORARG1, D3DTA_TEXTURE|D3DTA_ALPHAREPLICATE);//texture with alpha
	  break;


      
  }
  return;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 void CStateManagementObj::SetDiffuseSource(int type,LPDIRECT3DDEVICE9 g_pd3dDevice)
{

	if(m_currentDiffuseSource == type)
	   return;

	m_currentDiffuseSource = type;
   
	
	switch(m_currentDiffuseSource){
	case 0:
	  g_pd3dDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	  break;
    case 1:
      g_pd3dDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	  break;
	}


}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 void CStateManagementObj::SetVertexType(int uvCount,LPDIRECT3DDEVICE9 g_pd3dDevice)
{

  //if(m_currentVertexType == uvCount)
  //  return;
     int tempType = uvCount;

	 if(m_currentDiffuseSource == 1)
	    tempType = tempType + 9;

	

  if(m_currentVertexType == tempType)
     return;

  m_currentVertexType = tempType;

  switch(m_currentVertexType){
    case 0:
      g_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration0UV );
	  break;
	case 1:
      g_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration1UV );
	  break;
	case 2:
      g_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration2UV );
	  break;
	case 3:
      g_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration3UV );
	  break;
	case 4:
      g_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration4UV );
	  break;
	case 5:
      g_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration5UV );
	  break;
    case 6:
      g_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration6UV );
	  break;
    case 7:
      g_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration7UV );
	  break;
    case 8:
      g_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration8UV );
	  break;
    case 9:
	  g_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration0UVD );   
	  break;
    case 10:
	  g_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration1UVD );
	  break;
    case 11:
	  g_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration2UVD ); 
	  break;
    case 12:
      g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX3) );
	  break;
    case 13:
      g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX4) );
	  break;
    case 14:
      g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX5) );
	  break;
    case 15:
      g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX6) );
	  break;
    case 16:
      g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX7) );
	  break;
    case 17:
      g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX8) );
	  break;
    case 18:
      g_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration0UVT );
	  break;
    case 19:
      g_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration1UVT );
	  break;
    case 20:
      g_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration2UVT );
	  break;
    case 21:
      g_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration3UVT );
	  break;
    case 22:
      g_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration4UVT );
	  break;
    case 23:
      g_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration5UVT );
	  break;
    case 24:
      g_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration6UVT );
	  break;
    case 50:
      g_pd3dDevice->SetVertexDeclaration( m_pVertexDeclaration3UVT );
	  break;
    case 70:
      g_pd3dDevice->SetVertexDeclaration( m_pVertexLeafUVx );
	  break;
    case 71:
      g_pd3dDevice->SetVertexDeclaration( m_pVertexSimple );
	  break;
      
  }
  return;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CStateManagementObj::SetWindGlobal(D3DVECTOR curPush,LPDIRECT3DDEVICE9 g_pd3dDevice)
{

g_pd3dDevice->SetVertexShaderConstantF( VAL_WIND, (float*)&curPush, 4 );
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CStateManagementObj::SetZWritableState(BOOL state,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
    if(m_currentZWritableState == state)
	   return;
	
	m_currentZWritableState = state;
	
	g_pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, state );
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CStateManagementObj::SetAlphaPixelTest(BOOL state,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
    if(m_cirrentAlphaPixelTestState == state)
	   return;
	
	m_cirrentAlphaPixelTestState = state;
	
	g_pd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, state);
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 void CStateManagementObj::SetBlendLevelMultipass(int type,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
  switch(type){
	case -1:
	  //g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND ,           D3DBLEND_DESTCOLOR);
      //g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,           D3DBLEND_ZERO); 
	  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	  m_globalBlend = -1;
	  break;
    case 0:
	  g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND ,           D3DBLEND_DESTCOLOR);
      g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,           D3DBLEND_ZERO); 
      g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,   TRUE );
      
	  m_globalBlend = 0;
	  break;
	case 1:
      g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
      g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,           D3DBLEND_ONE );
      g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,          D3DBLEND_ONE );
	  m_globalBlend = 1;
	  break;
	case 2:
      g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
      g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,           D3DBLEND_ZERO );
      g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,          D3DBLEND_INVSRCCOLOR );
	  m_globalBlend = 2;
	  break;
	case 3:
	  g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
      g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,           D3DBLEND_ONE);
      g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,          D3DBLEND_INVSRCCOLOR);
	  m_globalBlend = 3;
	  break;
	case 4:
      g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
      g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,           D3DBLEND_ONE );
      g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,          D3DBLEND_ONE );
	  m_globalBlend = 4;
	  break;
	case 5:
	  g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
      g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND ,           D3DBLEND_DESTCOLOR);
      g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,           D3DBLEND_SRCCOLOR); 
	  m_globalBlend = 5;
 	  break;
    
    
  }
  return;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------change bias--------------------------------------------------------------//
 BOOL CStateManagementObj::SetZBias(LPDIRECT3DDEVICE9 g_pd3dDevice,
										  int bias)
{
   if(m_globalZBias == bias)
      return FALSE;

   //g_pd3dDevice->SetRenderState( D3DRS_ZBIAS, bias );
   m_globalZBias = bias;
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------change blend type--------------------------------------------------------//
 BOOL CStateManagementObj::SetBlendType(LPDIRECT3DDEVICE9 g_pd3dDevice,
									          int type)
{
//-1 = none 1 = even opacity 2 = opacity map darker is more transparent
      if(m_globalBlend == type)//same
         return FALSE;

         switch( type ) 
		 {
               case -1:
                g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );
				//g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND ,           D3DBLEND_ONE);
                //g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,           D3DBLEND_ONE); 
                m_globalBlend = type;
               break;
               case 0 ://modulate
                g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,    TRUE );
                g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND ,           D3DBLEND_DESTCOLOR);
                g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,           D3DBLEND_ONE); 
                m_globalBlend = type;
               break;
               case 1 ://alpha
                g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
                g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
                g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
                m_globalBlend = type;
               break;
			   case 2 ://amplified
                g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
                g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_ONE );
                g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_ONE );

				
                m_globalBlend = type;
               break;
			   case 3 ://wash
                g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
                g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,    D3DBLEND_SRCCOLOR );
                g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,   D3DBLEND_DESTCOLOR  );
                m_globalBlend = type;
               break;
			   case 4 ://inversion amplification
                g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
                g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_ZERO );
                g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCCOLOR );

           
                m_globalBlend = type;
               break;
			   case 5 :
                g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
                g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,    D3DBLEND_ZERO  );
                g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,   D3DBLEND_INVSRCCOLOR   );
                m_globalBlend = type;
               break;
			   case 6 :
                g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
                g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,    D3DBLEND_ZERO  );
                g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,   D3DBLEND_SRCCOLOR   );
                m_globalBlend = type;
               break;
			   case 7 :
                g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
                g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
                g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
                m_globalBlend = type;
               break;
			   case 8 :
                g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
                g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_ZERO );
                g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_ONE );
                m_globalBlend = type;
               break;

		 }

return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 BOOL CStateManagementObj::SetFogState(LPDIRECT3DDEVICE9 g_pd3dDevice,
											 BOOL status)
{

   if(m_globalEnvFogOn == status)
	  return FALSE;


   if(!m_globalFilterFog){
       g_pd3dDevice->SetRenderState( D3DRS_FOGENABLE, status );
  
   }

   m_globalEnvFogOn = status;
   return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 BOOL CStateManagementObj::SetFogFilter(LPDIRECT3DDEVICE9 g_pd3dDevice,
											  BOOL              filterState)
{
   if(!m_globalEnvFogOn)
      return FALSE;

   if(m_globalFilterFog == filterState)
	  return FALSE;

      if(!filterState)
        g_pd3dDevice->SetRenderState( D3DRS_FOGENABLE, TRUE );
	  else
		g_pd3dDevice->SetRenderState( D3DRS_FOGENABLE, FALSE );

      m_globalFilterFog = filterState;
   
   return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-----------add texture--------------------------------------------------------------//
/*int CStateManagementObj::TextureReference(CString textureFileName,
										  CTextureDBObjList * textureDataBase,
										  LPDIRECT3DDEVICE9 g_pd3dDevice,
										  BOOL bitmapLoadOvverride,
										  int & maxTextureSize,
										  int & compressIt)
{
 char charbuf[256];
 CString temp = textureFileName;
 int chCount = temp.GetLength();
 if(chCount == 0)
    return -1;

    lstrcpy(charbuf, textureFileName);
    //make sure it doesnt already exist
    int indexTraceLoc = 0;
    BOOL continueIt = FALSE;
    for(POSITION posLoc = textureDataBase->GetHeadPosition();posLoc != NULL;)
	{
    CTextureDBObj * ptrLoc = ( CTextureDBObj* ) textureDataBase->GetNext(posLoc);
    int result = strncmp(charbuf,ptrLoc->List_textureName,256);
      if(result==0){//exists
	     return indexTraceLoc;
	     continueIt = TRUE;
	     break;//dont add
	  }
     indexTraceLoc++;
	}

    //end make sure it doesnt already exist
    CString cmp = charbuf;
       if(cmp == "NONE" || cmp == "none" )
		  return -1;
          CTextureDBObj * newPtr = new CTextureDBObj(charbuf);
          newPtr->m_textureObject = new CTextureEX();
	      //use mapsmodels folder for organization
          CString CurDir;
          CString MainDir;
          char buffer[_MAX_PATH];
		  _getcwd( buffer, _MAX_PATH );
		  MainDir = CurDir = buffer;
		  CurDir = operator+(CurDir,"\\MapsModels\\");
          //end use mapsmodels folder for organization   
          CString convString = charbuf;
	      CString final = operator+(CurDir,convString);
	      if(newPtr->m_textureObject->LoadIt(final,
			                                 10,
											 g_pd3dDevice,
											 FALSE)==TRUE){//success

		    if(newPtr->m_textureObject->Restore(g_pd3dDevice,
												newPtr->List_BltOpOn,
												newPtr->m_bump,
												newPtr->m_bumpType,
												maxTextureSize,
												m_pCaps,
												compressIt)==S_OK){//2
	         textureDataBase->AddTail(newPtr);
	         return indexTraceLoc;
			}//end 2
		    else{
		     newPtr->m_textureObject->SafeDelete();
		     delete newPtr->m_textureObject;
		     newPtr->m_textureObject=0;
		     delete newPtr;
		     newPtr=0;
			}
		  }//end success
	      else{
             newPtr->m_textureObject->SafeDelete();
		     delete newPtr->m_textureObject;
		     newPtr->m_textureObject=0;
		     delete newPtr;
		     newPtr=0;
		  }
return -1;
}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CStateManagementObj::SetCullState(int onType,
									   LPDIRECT3DDEVICE9 g_pd3dDevice)
{

if(m_mirrorModeOn)
{
   if(onType == 1)
      onType = 2;
   else if(onType == 2)
      onType = 1;
}


if(onType == GL_cullOn)
return FALSE;

if(onType == 1)
g_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
else if(onType == 0)
g_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);
else if(onType == 2)
g_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CW);

GL_cullOn = onType;
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CStateManagementObj::SetFogOverrideState(BOOL on,
									          LPDIRECT3DDEVICE9 g_pd3dDevice)
{
if(on == m_fogOverride)
return FALSE;




if(on == TRUE)
{
   D3DCOLOR colorLoc = D3DCOLOR_XRGB(0,0,0);
   
   g_pd3dDevice->GetRenderState( D3DRS_FOGCOLOR, &m_oldFogColor);
   g_pd3dDevice->SetRenderState( D3DRS_FOGCOLOR, colorLoc);
}
if(on == FALSE)
{
   g_pd3dDevice->SetRenderState( D3DRS_FOGCOLOR, m_oldFogColor);
}

m_fogOverride = on;
return TRUE;
}



//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 BOOL CStateManagementObj::SetWireFillState(BOOL on,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
if(on == GL_wireFillState)
   return FALSE;

if(m_forceWireframe)  
   return FALSE;
      
if(on == TRUE)
   g_pd3dDevice->SetRenderState( D3DRS_FILLMODE,  D3DFILL_WIREFRAME );
if(on == FALSE)
   g_pd3dDevice->SetRenderState( D3DRS_FILLMODE,  D3DFILL_SOLID );

GL_wireFillState = on;
return TRUE;
}
 //-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CStateManagementObj::SetGlobalWireFrameMode(BOOL enable,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     m_forceWireframe = enable;
	 if(enable)
	 {
        g_pd3dDevice->SetRenderState( D3DRS_FILLMODE,  D3DFILL_WIREFRAME );
		GL_wireFillState = 1;
	 }

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CStateManagementObj::SetCameraSpace(BOOL enabled,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
if(enabled == m_cameraSpaceEnabled)
   return;
      
if(enabled == TRUE)
   SetViewMatrix((D3DXMATRIX*)&m_matrixInit,g_pd3dDevice);
else
   SetViewMatrix((D3DXMATRIX*)&m_viewMatrix,g_pd3dDevice);

   m_cameraSpaceEnabled = enabled;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CStateManagementObj::SetTextureTileMode(int mode,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
if(GL_lastTextureWrapMode == mode)
return FALSE;

if(mode == 0){
g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
}
if(mode == 1){
g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_MIRROR);
g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_MIRROR);
}
if(mode == 2){
g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_CLAMP);
g_pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_CLAMP);
}
GL_lastTextureWrapMode = mode;
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CStateManagementObj::InitShaderGlobals(LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	D3DXVECTOR4 vZero( 0.0f, 0.0f, 0.0f, 0.0f );
    D3DXVECTOR4 vOne( 1.0f, 1.0f, 1.0f, 1.0f );
	D3DXVECTOR4 half( 0.5f, 0.5f, 0.5f, 0.5f );
	D3DXVECTOR4 maxPower( 128.0f, 128.0f, 128.0f, 128.0f );
	g_pd3dDevice->SetVertexShaderConstantF( VAL_ONE, (float*)&vOne, 1 );
	g_pd3dDevice->SetVertexShaderConstantF( VAL_ZERO, (float*)&vZero, 1 );
	g_pd3dDevice->SetVertexShaderConstantF( VAL_HALF, (float*)&half, 1 );
	g_pd3dDevice->SetVertexShaderConstantF( MAX_POWER, (float*)&maxPower, 1 );
	
	D3DMATRIX projMMat;
	ZeroMemory(&projMMat,sizeof(D3DMATRIX));
    projMMat._11 = 0.5f;
	projMMat._22 = -0.5f;
	projMMat._41 = 0.5f;
	projMMat._42 = 0.5f;
	projMMat._43 = 1.0f;


	D3DXMatrixTranspose( (D3DXMATRIX*)&projMMat, (D3DXMATRIX*)&projMMat );

	g_pd3dDevice->SetVertexShaderConstantF( PROJ_M, (float*)&projMMat, 4 );
	//PROJ_M


	return TRUE;
}
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
//-------------------------------------------------------------------//
 void CStateManagementObj::UpdateTFACTORVIEW(D3DVECTOR position,
                                                   D3DVECTOR objectPosition,
											       LPDIRECT3DDEVICE9 g_pd3dDevice
											       )
{
        position.x = objectPosition.x - position.x;
		position.y = objectPosition.y - position.y;
		position.z = objectPosition.z - position.z;
	    float dist = m_matrixAB.Magnitude(position);
        if(dist == 0.0f)
		   return;

        position.x = position.x / dist;
        position.y = position.y / dist;
        position.z = position.z / dist;
        DWORD dwFactor = VectortoRGBA( position, 10.0f );
        g_pd3dDevice->SetRenderState( D3DRS_TEXTUREFACTOR, dwFactor );
}
//-----------------------------------------------------------------------------
//Name: VectortoRGBA()
//Desc: Turns a normalized vector into RGBA form. Used to encode vectors into
//      a height map. 
//-----------------------------------------------------------------------------
DWORD CStateManagementObj::VectortoRGBA( D3DVECTOR v, FLOAT fHeight )
{
    DWORD r = (DWORD)( 127.0f * v.x + 128.0f );
    DWORD g = (DWORD)( 127.0f * v.y + 128.0f );
    DWORD b = (DWORD)( 127.0f * v.z + 128.0f );
    DWORD a = (DWORD)( 255.0f * fHeight );
    
    return( (a<<24L) + (r<<16L) + (g<<8L) + (b<<0L) );
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 void CStateManagementObj::SetProjUVGenMatrix(D3DXMATRIX projection,LPDIRECT3DDEVICE9 g_pd3dDevice)
 {
         projection._11 *= .5f;
		 projection._22 *= -.5f;
		 projection._31 += .5f;
		 projection._32 += .5f;
		 D3DXMatrixTranspose( (D3DXMATRIX*)&projection, (D3DXMATRIX*)&projection );
		 g_pd3dDevice->SetVertexShaderConstantF( PROJ_M, (float*)&projection, 4 );
 }
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 void CStateManagementObj::SetViewProjMatrix(D3DXMATRIX *pView, 
	                                         D3DXMATRIX *pProj, 
											 LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	 g_pd3dDevice->SetTransform( D3DTS_PROJECTION, pProj );
	 g_pd3dDevice->SetTransform( D3DTS_VIEW , pView );
     m_viewMatrix = *pView;
	 m_projMatrix = *pProj;
	 m_viewMatrixUTP = *pView;//untransposed
	 m_projMatrixUTP = *pProj;//untransposed
	 D3DXMatrixInverse(&m_invViewMatrix,NULL,&m_viewMatrixUTP);

	 D3DXMatrixTranspose( &m_viewMatrix, &m_viewMatrix );
	 D3DXMatrixTranspose( &m_projMatrix, &m_projMatrix );

	 g_pd3dDevice->SetVertexShaderConstantF( VIEW_MATRIX, (float*)&m_viewMatrix, 4 );
	 g_pd3dDevice->SetVertexShaderConstantF( CLIP_MATRIX, (float*)&m_projMatrix, 4 );
	 g_pd3dDevice->SetVertexShaderConstantF( EYE_VECTOR, (float*)&m_locationOffset, 1 );

	 

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CStateManagementObj::SetCamMatrix(D3DXMATRIX *cMat, LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	 D3DXMATRIX m_cMatTransposed;
	 D3DXMatrixTranspose( &m_cMatTransposed, cMat );
     g_pd3dDevice->SetVertexShaderConstantF( VAL_CAMROTMATRIX, (float*)&m_cMatTransposed, 4 );
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 void CStateManagementObj::SetViewProjMatrixOnly(D3DXMATRIX *pProj, LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	 g_pd3dDevice->SetTransform( D3DTS_PROJECTION, pProj );
	 m_projMatrix = *pProj;
	 m_projMatrixUTP = *pProj;//untransposed
	 D3DXMatrixInverse(&m_invViewMatrix,NULL,&m_viewMatrixUTP);
	 D3DXMatrixTranspose( &m_projMatrix, &m_projMatrix );
	 g_pd3dDevice->SetVertexShaderConstantF( CLIP_MATRIX, (float*)&m_projMatrix, 4 );
	 g_pd3dDevice->SetVertexShaderConstantF( EYE_VECTOR, (float*)&m_locationOffset, 1 );

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 void CStateManagementObj::SetViewMatrix(D3DXMATRIX *pView, LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	 g_pd3dDevice->SetTransform( D3DTS_VIEW , pView );
     m_viewMatrix = *pView;
	 m_viewMatrixUTP = *pView;
	 D3DXMatrixTranspose( &m_viewMatrix, &m_viewMatrix );
	 g_pd3dDevice->SetVertexShaderConstantF( VIEW_MATRIX, (float*)&m_viewMatrix, 4 );
	 g_pd3dDevice->SetVertexShaderConstantF( EYE_VECTOR, (float*)&m_locationOffset, 1 );
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 void CStateManagementObj::SetMaterial(LPDIRECT3DDEVICE9 g_pd3dDevice, D3DMATERIAL9 * material)
{
        

  if(memcmp(&m_lastMaterial,material,sizeof(D3DMATERIAL9))==0 && m_currentShader == -1)
     return;

  CopyMemory(&m_lastMaterial,material,sizeof(D3DMATERIAL9));

      if(material)
	  {
       if(material->Specular.a == 0.0f)
	   {
	      SetSpecular(g_pd3dDevice, FALSE);
		  material->Power = 0.0f;
	   }
	   else
	   {
          SetSpecular(g_pd3dDevice, TRUE);
		  material->Power = 20.0f;
	   }
	   g_pd3dDevice->SetMaterial(material);
	  }
	  else{
	    SetSpecular(g_pd3dDevice, FALSE);

        //g_pd3dDevice->SetMaterial(NULL);
		return;
	  }

	

      if(m_currentShader == -1)
	    return;

	  g_pd3dDevice->SetVertexShaderConstantF( DIFFUSE_COLOR, (float*)&material->Diffuse, 1 );
	  g_pd3dDevice->SetVertexShaderConstantF( AMBIENT_COLOR, (float*)&material->Ambient, 1 );
	  g_pd3dDevice->SetVertexShaderConstantF( EMMISIVE_COLOR, (float*)&material->Emissive, 1 );
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 void CStateManagementObj::SetAmbientLight(LPDIRECT3DDEVICE9 g_pd3dDevice, float r, float g, float b)
{
    D3DXVECTOR4 ambientColor( r, g, b, 0.0f );
	g_pd3dDevice->SetVertexShaderConstantF( AMBIENT_LIGHT, (float*)&ambientColor, 1 );
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 void CStateManagementObj::SetDirLight(LPDIRECT3DDEVICE9 g_pd3dDevice, D3DVECTOR dirVect, float r, float g, float b)
{
    D3DXVECTOR4 dirColor( r, g, b, 0.0f );

	m_lastDirLight = dirVect;

	g_pd3dDevice->SetVertexShaderConstantF( DIR_LIGHTVECT, (float*)&dirVect, 1 );
	g_pd3dDevice->SetVertexShaderConstantF( DIR_LIGHTCOLOR, (float*)&dirColor, 1 );
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CStateManagementObj::SetSTLight(int & index,LPDIRECT3DDEVICE9 g_pd3dDevice, D3DVECTOR & position, float & r, float & g, float & b, float & rg)
{
	CopyMemory(m_tempLightDir,&position,sizeof(D3DVECTOR));
	m_tempLightDir[3] = rg;
	m_tempLightColor[0] = r;
	m_tempLightColor[1] = g;
	m_tempLightColor[2] = b;
	m_tempLightColor[3] = 0.0f;
   

	     switch( index ) 
		 {
               case 1:
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT1, (float*)&m_tempLightDir, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT1_COLOR, (float*)&m_tempLightColor, 1 );
               break;
               case 2 :
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT2, (float*)&m_tempLightDir, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT2_COLOR, (float*)&m_tempLightColor, 1 );
               break;
               case 3 :
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT3, (float*)&m_tempLightDir, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT3_COLOR, (float*)&m_tempLightColor, 1 );
               break;
			   case 4 :
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT4, (float*)&m_tempLightDir, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT4_COLOR, (float*)&m_tempLightColor, 1 );
               break;
			   case 5 :
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT5, (float*)&m_tempLightDir, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT5_COLOR, (float*)&m_tempLightColor, 1 );
               break;
			   case 6 ://inversion amplification
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT6, (float*)&m_tempLightDir, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT6_COLOR, (float*)&m_tempLightColor, 1 );
               break;
			   case 7 :
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT7, (float*)&m_tempLightDir, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT7_COLOR, (float*)&m_tempLightColor, 1 );
               break;
		 }
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CStateManagementObj::SetSTLightCustom(int & constantIndex,LPDIRECT3DDEVICE9 g_pd3dDevice, D3DVECTOR & position, float & r, float & g, float & b, float & rg)
{
	CopyMemory(m_tempLightDir,&position,sizeof(D3DVECTOR));
	m_tempLightDir[3] = rg;
	m_tempLightColor[0] = r;
	m_tempLightColor[1] = g;
	m_tempLightColor[2] = b;
	m_tempLightColor[3] = 0.0f;

	g_pd3dDevice->SetVertexShaderConstantF( constantIndex, (float*)&m_tempLightDir, 1 );//pos range
    g_pd3dDevice->SetVertexShaderConstantF( (constantIndex+1), (float*)&m_tempLightColor, 1 );
  
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CStateManagementObj::StopSTLight(int & index,LPDIRECT3DDEVICE9 g_pd3dDevice)
{

	     switch( index ) 
		 {
               case 1:
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT1, (float*)&m_zero4Float, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT1_COLOR, (float*)&m_zero4Float, 1 );
               break;
               case 2 :
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT2, (float*)&m_zero4Float, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT2_COLOR, (float*)&m_zero4Float, 1 );
               break;
               case 3 :
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT3, (float*)&m_zero4Float, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT3_COLOR, (float*)&m_zero4Float, 1 );
               break;
			   case 4 :
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT4, (float*)&m_zero4Float, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT4_COLOR, (float*)&m_zero4Float, 1 );
               break;
			   case 5 :
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT5, (float*)&m_zero4Float, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT5_COLOR, (float*)&m_zero4Float, 1 );
               break;
			   case 6 ://inversion amplification
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT6, (float*)&m_zero4Float, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT6_COLOR, (float*)&m_zero4Float, 1 );
               break;
			   case 7 :
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT7, (float*)&m_zero4Float, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT7_COLOR, (float*)&m_zero4Float, 1 );
               break;
		 }
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 void CStateManagementObj::SetSpecular(LPDIRECT3DDEVICE9 g_pd3dDevice, BOOL state)
{
      if(m_specularEnable == state)
		 return;
      m_specularEnable = state;

	  g_pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, m_specularEnable );
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 void CStateManagementObj::SetFogConstant(LPDIRECT3DDEVICE9 g_pd3dDevice, float startPoint, float endPoint)
{

	  D3DXVECTOR4 fogVal(startPoint,endPoint,1.0f/(endPoint-startPoint),0.0f);
	  g_pd3dDevice->SetVertexShaderConstantF( FOG_RANGE, (float*)&fogVal, 1 );	  
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 void CStateManagementObj::SetShader(LPDIRECT3DDEVICE9 g_pd3dDevice,CShaderObjList * shaderSystem, int ShaderIndexID)
{
       if(m_currentShader == ShaderIndexID && m_currentHSLeffect == NULL)
	      return;

	   if(m_forceShadeMode)
	      ShaderIndexID = -1;
	   
	   m_currentShader = ShaderIndexID;
 
	   if(shaderSystem)
          m_shderSystemRef = shaderSystem;
	   if(m_shderSystemRef)
	      m_shderSystemRef->SetShaderByIndex(g_pd3dDevice,ShaderIndexID,0,&m_currentHSLeffect,m_currrentEffectPeramCountCRC,&currentHSLcode,NULL);//-1 fixed pipeline
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CStateManagementObj::SetPxShader(LPDIRECT3DDEVICE9 g_pd3dDevice,CShaderObjList * shaderSystem, int ShaderIndexID)
{
       if(m_currentPxShader == ShaderIndexID && m_currentHSLeffect == NULL)
	      return;

	   if(m_forceShadeMode)
	      ShaderIndexID = -1;
	   
	   m_currentPxShader = ShaderIndexID;
	   m_currentHSLeffect = NULL;
	   m_currentPxFxShader = NULL;
 
	   if(shaderSystem)
          m_pxShderSystemRef = shaderSystem;
	   if(m_pxShderSystemRef)
	      m_pxShderSystemRef->SetShaderByIndex(g_pd3dDevice,ShaderIndexID,1,&m_currentHSLeffect,m_currrentEffectPeramCountCRC,&currentHSLcode,&m_currentPxFxShader);//-1 fixed pipeline
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 void CStateManagementObj::SetWorldMatrix(D3DXMATRIX *pObject, LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     g_pd3dDevice->SetTransform( D3DTS_WORLD, pObject );

	 if(m_currentShader == -1)
	    return;
	 
	 //D3DXMATRIX mat, wldTranspose, viewMatrix;
	 D3DXMatrixTranspose( &m_tempMatrix, pObject );
	 g_pd3dDevice->SetVertexShaderConstantF( WORLD_MATRIX, (float*)&m_tempMatrix, 4 );
	 
	 /*D3DXMatrixTranspose( &wldTranspose, pObject );
	 D3DXMatrixMultiply( &mat, pObject,&m_viewMatrix );
     D3DXMatrixMultiply( &mat, &mat, &m_projMatrix );
     D3DXMatrixTranspose( &mat, &mat );
	 D3DXMatrixTranspose( &viewMatrix, &m_viewMatrix );
	 
     g_pd3dDevice->SetVertexShaderConstantF( VIEW_MATRIX, (float*)&viewMatrix, 4 );
     g_pd3dDevice->SetVertexShaderConstantF( CLIP_MATRIX, (float*)&mat, 4 );
     g_pd3dDevice->SetVertexShaderConstantF( EYE_VECTOR, (float*)&m_locationOffset, 1 );
     g_pd3dDevice->SetVertexShaderConstantF( WORLD_MATRIX, (float*)&wldTranspose, 4 );
	 D3DXVECTOR4 fogVal2(m_hightColorRangeStart,m_hightColorRangeEnd,1.0f/(m_hightColorRangeEnd-m_hightColorRangeStart),(pObject->_42 + m_locationOffset.y));
	 g_pd3dDevice->SetVertexShaderConstantF( DEPTH_DIMINISH, (float*)&fogVal2, 1 );*/
	 
	 //D3DXMatrixMultiply( &mat, pObject,&m_viewMatrix );
     //D3DXMatrixMultiply( &mat, &mat, &m_projMatrix );
     //D3DXMatrixTranspose( &mat, &mat );
	 //D3DXMatrixTranspose( &viewMatrix, &m_viewMatrix );
	 
     
     
    
     
	 //D3DXVECTOR4 fogVal2(m_hightColorRangeStart,m_hightColorRangeEnd,1.0f/(m_hightColorRangeEnd-m_hightColorRangeStart),(pObject->_42 + m_locationOffset.y));
	 //g_pd3dDevice->SetVertexShaderConstantF( DEPTH_DIMINISH, (float*)&fogVal2, 1 );

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CStateManagementObj::InitializeAllStates(LPDIRECT3DDEVICE9 pd3dDevice)
{
	//SetPxShader(pd3dDevice,m_pxShaderSystem,-1);
    D3DCAPS9 pCaps;
    pd3dDevice->GetDeviceCaps(&pCaps);

    if(pCaps.PixelShaderVersion >= D3DPS_VERSION(1,1))
	   m_maxPixShaderVs = 1.1f;
	if(pCaps.PixelShaderVersion >= D3DPS_VERSION(1,4))
	   m_maxPixShaderVs = 1.4f;
	if(pCaps.PixelShaderVersion >= D3DPS_VERSION(2,0))
	   m_maxPixShaderVs = 2.0f;
	if(pCaps.PixelShaderVersion >= D3DPS_VERSION(2,1))
	   m_maxPixShaderVs = 2.1f;
	if(pCaps.PixelShaderVersion >= D3DPS_VERSION(3,0))
	   m_maxPixShaderVs = 3.0f;

	if(pCaps.VertexShaderVersion >= D3DVS_VERSION(1,0))
	   m_maxVSShaderVs = 1.0f;
	if(pCaps.VertexShaderVersion >= D3DVS_VERSION(1,1))
	   m_maxVSShaderVs = 1.1f;
	if(pCaps.VertexShaderVersion >= D3DVS_VERSION(1,4))
	   m_maxVSShaderVs = 1.4f;
	if(pCaps.VertexShaderVersion >= D3DVS_VERSION(2,0))
	   m_maxVSShaderVs = 2.0f;
	if(pCaps.VertexShaderVersion >= D3DVS_VERSION(2,1))
	   m_maxVSShaderVs = 2.1f;
	if(pCaps.VertexShaderVersion >= D3DVS_VERSION(3,0))
	   m_maxVSShaderVs = 3.0f;


	
	//m_maxVSShaderVs            = pCaps.VertexShaderVersion;

    pd3dDevice->SetRenderState(D3DRS_ALPHAREF, DWORD(84.0f));
    pd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

    BOOL supportsBumpLumen = TRUE;
    m_multiTextureSupported = pCaps.MaxSimultaneousTextures;
    m_maxActiveLights = pCaps.MaxActiveLights-1;
    
    InitCaps(pd3dDevice);
    m_stencilBufferAble = m_supportedStencilBuffer;
    SetBlendLevel(m_intZero,m_intZero,pd3dDevice);

	//--------------------------------------------------------------------
	// Anisotropic filtering (m_anisoMode, pushed from startcfg.aut by the
	// client): 0=off, 1=2x, 2=4x, 3=8x, 4=16x. Requires the MIN anisotropic
	// filter cap; the level is clamped to what the adapter reports in
	// MaxAnisotropy. MIPFILTER stays trilinear LINEAR - D3D9 has no anisotropic
	// mip filter - and MAGFILTER stays LINEAR (magnification anisotropy is
	// essentially never supported and gains nothing).
	//--------------------------------------------------------------------
	if(m_anisoMode < 0 || m_anisoMode > 4)
	   m_anisoMode = 4;//clamp garbage to the safe default
	int anisoLevel = 0;
	if(m_anisoMode > 0 &&
	   (pCaps.TextureFilterCaps & D3DPTFILTERCAPS_MINFANISOTROPIC))
	{
	   anisoLevel = 1 << m_anisoMode;//2,4,8,16
	   if((DWORD)anisoLevel > pCaps.MaxAnisotropy)
	      anisoLevel = (int)pCaps.MaxAnisotropy;
	   if(anisoLevel < 2)
	      anisoLevel = 0;//adapter reports aniso caps below 2x - treat as off
	}
	DWORD minFilterState = (anisoLevel > 0) ? D3DTEXF_ANISOTROPIC : D3DTEXF_LINEAR;

	pd3dDevice->SetSamplerState( 0, D3DSAMP_MINFILTER, minFilterState );
    pd3dDevice->SetSamplerState( 0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 0, D3DTSS_TEXCOORDINDEX, 0 );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_DIFFUSE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );
	pd3dDevice->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
	    

    pd3dDevice->SetTextureStageState( 1, D3DTSS_TEXCOORDINDEX, 1 );
	pd3dDevice->SetSamplerState( 1, D3DSAMP_MINFILTER, minFilterState );
    pd3dDevice->SetSamplerState( 1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    pd3dDevice->SetTextureStageState( 1, D3DTSS_COLORARG2, D3DTA_CURRENT ); 
	pd3dDevice->SetTextureStageState( 1, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	pd3dDevice->SetTextureStageState( 1, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	pd3dDevice->SetTextureStageState( 2, D3DTSS_TEXCOORDINDEX, 2 );
	pd3dDevice->SetSamplerState( 2, D3DSAMP_MINFILTER, minFilterState );
    pd3dDevice->SetSamplerState( 2, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 2, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    pd3dDevice->SetTextureStageState( 2, D3DTSS_COLORARG2, D3DTA_CURRENT ); 
	pd3dDevice->SetTextureStageState( 2, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	pd3dDevice->SetTextureStageState( 2, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	pd3dDevice->SetTextureStageState( 3, D3DTSS_TEXCOORDINDEX, 3 );
	pd3dDevice->SetSamplerState( 3, D3DSAMP_MINFILTER, minFilterState );
    pd3dDevice->SetSamplerState( 3, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 3, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    pd3dDevice->SetTextureStageState( 3, D3DTSS_COLORARG2, D3DTA_CURRENT ); 
	pd3dDevice->SetTextureStageState( 3, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	pd3dDevice->SetTextureStageState( 3, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

    pd3dDevice->SetTextureStageState( 4, D3DTSS_TEXCOORDINDEX, 4 );
	pd3dDevice->SetSamplerState( 4, D3DSAMP_MINFILTER, minFilterState );
    pd3dDevice->SetSamplerState( 4, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 4, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    pd3dDevice->SetTextureStageState( 4, D3DTSS_COLORARG2, D3DTA_CURRENT ); 
	pd3dDevice->SetTextureStageState( 4, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	pd3dDevice->SetTextureStageState( 4, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	pd3dDevice->SetTextureStageState( 5, D3DTSS_TEXCOORDINDEX, 5 );
	pd3dDevice->SetSamplerState( 5, D3DSAMP_MINFILTER, minFilterState );
    pd3dDevice->SetSamplerState( 5, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 5, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    pd3dDevice->SetTextureStageState( 5, D3DTSS_COLORARG2, D3DTA_CURRENT ); 
	pd3dDevice->SetTextureStageState( 5, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	pd3dDevice->SetTextureStageState( 5, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	pd3dDevice->SetTextureStageState( 6, D3DTSS_TEXCOORDINDEX, 6 );
	pd3dDevice->SetSamplerState( 6, D3DSAMP_MINFILTER, minFilterState );
    pd3dDevice->SetSamplerState( 6, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 6, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    pd3dDevice->SetTextureStageState( 6, D3DTSS_COLORARG2, D3DTA_CURRENT ); 
	pd3dDevice->SetTextureStageState( 6, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	pd3dDevice->SetTextureStageState( 6, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	pd3dDevice->SetTextureStageState( 7, D3DTSS_TEXCOORDINDEX, 7 );
	pd3dDevice->SetSamplerState( 7, D3DSAMP_MINFILTER, minFilterState );
    pd3dDevice->SetSamplerState( 7, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
    pd3dDevice->SetTextureStageState( 7, D3DTSS_COLORARG1, D3DTA_TEXTURE );
    pd3dDevice->SetTextureStageState( 7, D3DTSS_COLORARG2, D3DTA_CURRENT ); 
	pd3dDevice->SetTextureStageState( 7, D3DTSS_COLOROP,   D3DTOP_DISABLE );
	pd3dDevice->SetTextureStageState( 7, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );

	pd3dDevice->SetRenderState( D3DRS_DITHERENABLE,   TRUE );
    pd3dDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
	pd3dDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
	
	pd3dDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
    pd3dDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	   
       
	//pd3dDevice->SetRenderState( D3DRENDERSTATE_COLORKEYENABLE,TRUE);
	//texture state
	pd3dDevice->SetSamplerState( 0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState( 1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState( 2, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState( 3, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState( 4, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState( 5, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState( 6, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
	pd3dDevice->SetSamplerState( 7, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR );
	//Anisotropy degree for every sampler when active
	if(anisoLevel > 0)
	{
	   for(DWORD samplerLoc = 0; samplerLoc < 8; samplerLoc++)
	      pd3dDevice->SetSamplerState(samplerLoc,D3DSAMP_MAXANISOTROPY,(DWORD)anisoLevel);
	}
	//software settings
		

	pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
	pd3dDevice->SetSamplerState(0,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
	pd3dDevice->SetSamplerState(1,D3DSAMP_ADDRESSU,D3DTADDRESS_WRAP);
	pd3dDevice->SetSamplerState(1,D3DSAMP_ADDRESSV,D3DTADDRESS_WRAP);
	pd3dDevice->SetRenderState( D3DRS_ZFUNC,D3DCMP_LESSEQUAL);
	pd3dDevice->SetRenderState( D3DRS_LIGHTING, TRUE );
	pd3dDevice->SetRenderState( D3DRS_CULLMODE,  D3DCULL_CCW );
	pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, FALSE );

	InitShaderGlobals(pd3dDevice);
}






