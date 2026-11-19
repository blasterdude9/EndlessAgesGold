#ifndef CMATERIALCLASS_H
#define CMATERIALCLASS_H

#include "Common.h"
#include "CStructuresMisl.h"

//define know constants
#define ARBP_UKNOWN 0
#define ARBP_USEDEFAULT 1
#define ARBP_WorldInverseTranspose 2
#define ARBP_WorldViewProjection 3
#define ARBP_World 4
#define ARBP_ViewInverse 5
#define ARBP_Time 6
#define ARBP_WorldView 7
#define ARBP_Projection 8
#define ARBP_Texture 9
#define ARBP_Material 12
#define ARBP_DirLight 13
#define ARBP_Diffuse 14
#define ARBP_Specular 15
#define ARBP_Emmisive 16
#define ARBP_Ambient 17
#define ARBP_SpecPower 18
#define ARBP_WorldInverse 20
#define ARBP_VolumeTexturePrcdrl 21

class SLARBperameterBlock
{
public:
	SLARBperameterBlock()
	{
		this->data = NULL;
	}

	~SLARBperameterBlock()
	{
		SafeDeleteArray(this->data);
	}

    short type;//determins what dataset is initialized IEFFECT CLONE TYPES
	short dataSize;
	byte * data;
	byte svData;//1=sv data 0=skip save data
};




class CMaterialObject : public CObject
{

	
     
	DECLARE_SERIAL( CMaterialObject );
	
    

public:
	CMaterialObject();
	~CMaterialObject();
	CMaterialObject(D3DMATERIAL9 materialInit);
	D3DMATERIAL9    m_useMaterial;//main material used to render
	D3DMATERIAL9    m_baseMaterial;//original colors set before any mod functions
	int             m_transition;//0=none
	int             m_cycleTime;
    int             m_textureTileType;
	BOOL            m_switch;
	DWORD           m_timeStamp;
	BOOL            m_cullOn;
	BOOL            m_forceWireFrame;
	int             m_blendMode;//0=modulated transparency 2=amplified transparency
	int             m_optionalShaderIndex;

	BOOL            m_enableTextureMovement;
	BOOL            m_enableSet0,m_enableSet1,m_enableSet2,
		            m_enableSet3,m_enableSet4,m_enableSet5,
					m_enableSet6,m_enableSet7;
	TXTUV           m_textureMovement[8];  
	D3DMATERIAL9    m_targetModMaterial;//for transition
	

	int             m_mirrorEnabled;
	int             m_baseBlendMode;

	float           m_curDistFromCam;
    long            m_fadeMaxDist;
    long            m_fadeVisDist;
    BOOL            m_fadeByDistance;

	int             m_transitionBlend;

	BOOL            m_fogFilter;
	int             m_zBias;
	BOOL            m_fogOverride;
	char            m_shadowObject;

	int             m_diffuseEffect;
	char            m_colorArg1[10];
	char            m_colorArg2[10];
	char            m_texcoordLevels[10];
	short           m_pixelShader;
	short           m_vertexShader;
	BOOL            m_doAlphaDepthTest;
	int			    m_mirrorProcessType;
	BOOL			m_forceCameraTransform;
 
	//IEFFECT extenstions temp adaptation
	SLARBperameterBlock * m_peramBlock;
	int                   m_peramCount;
    CString               m_fxBindString;
	//override serialized

 


    void        SetFadeOutAlpha(long duration);
	void        SetFadeInAlpha(long duration);
    void        SetFadeOut(long duration);
    void        SetFadeIn(long duration,BOOL reset);
	void        CopyMaterial(D3DMATERIAL9 source,D3DMATERIAL9 * dest);
    void		Callback();
	void        Clone(CMaterialObject ** clone);
	float       InterpolateFloats(
						          float StartVec, 
						          float EndVec,
					              int NumOfStages, 
					              int ThisStage);
	void        Reset();
	void        SetCallbackMethod(int method);
	int         GetParameterCount(LPD3DXEFFECT effect);
	void        DeleteParamMemoryAndZeroIt();
	BOOL        InitPeramBlockFXShadersHSL(LPD3DXEFFECT effect);
	void		SetOverrideTrans(float percentage);
	void        Serialize( CArchive& ar );

};

#endif