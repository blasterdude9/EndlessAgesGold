#include "matrixarb.h"
#include "CMaterialClass.h"
#include "CStateManagementClass.h"
#ifndef CRTLIGHTCLASS_H
#define CRTLIGHTCLASS_H

class CRTLightObj : public CObject
{
    
	DECLARE_SERIAL ( CRTLightObj );

public:
    CRTLightObj();
	~CRTLightObj();

    MatrixARB                  GL_matrixFunctions;
    	
    CMaterialObject           *m_material;
	float                      m_range;
	float                      m_attenuation0;
	float                      m_attenuation1;
	float                      m_attenuation2;
	D3DVECTOR                  m_wldspacePos;
	
	//DPD l
	//light type {1 = point, 2 = spot, 3 = directional}
	//D3DLIGHTTYPE			   m_type;
	int						   m_type;
	D3DVECTOR				   m_direction;
	float					   m_theta;
	float					   m_phi;
	float					   m_falloff;

	//DPD4
	BOOL					   m_blinkOn;
	long					   m_blinkDur;
	BOOL					   m_randomOn;
	int						   m_intMin;
	int						   m_intMax;

	//light off-used for utilizing lights for nodes and not giving light
	BOOL                       m_lightOn;

	void Serialize( CArchive& ar );
	void Clone(CRTLightObj ** clone);
};

class CRTLightObjList : public CObList
{
public:
   CRTLightObjList()
   {
	   m_fixedMaxLightCount = 7;
	   m_progMaxLightCount=3;
	   ZeroMemory(&m_lastQuerryPos,sizeof(D3DVECTOR));
	   m_lastQuerryPos.z = -10000.1f;
	   m_lastShaderMode = -10;
	   ZeroMemory(&m_zeroVect,sizeof(D3DVECTOR));
   };
   ~CRTLightObjList();

   int m_fixedMaxLightCount;
   int m_progMaxLightCount;
   D3DLIGHT9 m_lightImStructTemp;
   int m_maxLights;
   int m_indexTemp;	
   int m_maxLightTrace;
   int m_lastShaderMode;
   D3DVECTOR m_lastQuerryPos;

   D3DVECTOR m_zeroVect;
   MatrixARB m_matLib;
   
	DECLARE_SERIAL( CRTLightObjList );
	
  	void FlushSys(LPDIRECT3DDEVICE9 g_pd3dDevice,CStateManagementObj * stateManager);
	void AddLight(CRTLightObj * light, D3DVECTOR & position);
	void AddLight(CRTLightObj * light, D3DVECTOR & position, D3DVECTOR & charPosition);//DPD2
	void EnableLightingPerObject(LPDIRECT3DDEVICE9 g_pd3dDevice,
									          D3DVECTOR & offset,
									          CStateManagementObj * stateManager,
											  D3DVECTOR & objectCenter,
											  float & objectRadius,
											  int & shaderMode);
	void SetLightingConstants(LPDIRECT3DDEVICE9 g_pd3dDevice,CStateManagementObj * stateManager,D3DVECTOR & offset);
	void SafeDelete();
};

#endif