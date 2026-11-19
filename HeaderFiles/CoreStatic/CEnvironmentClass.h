#include "CWldObject.h"
#include "cabfunctionlib.h"
#include "EnvParticleSystem.h"
#include "CFrameObj.h"
#include "CTimeSystem.h"
#include "matrixarb.h"
#include "CLightningEffectClass.h"
#include "CStateManagementClass.h"

#ifndef ENVIRONMENTCLASS_H
#define ENVIRONMENTCLASS_H

class  CEnvironmentObj : public CObject
{
	
	
     
    DECLARE_SERIAL( CEnvironmentObj )
    
    
public:
    MatrixARB GL_matrixFunctions;
    CABFunctionLib CABLIBREF; 
	CEnvironmentObj();
          
            
	       CTimeSystemObj   *m_enviormentTimeSystem;
		   MatrixARB         m_abMathLib;
           BOOL              m_fogOn;
	       float             m_fogDensity;
		   float             m_fogRange;
		   float             m_fogStart;
		   float             m_fogRed;
		   float             m_fogGreen;
		   float             m_fogBlue;
		   float             m_backRed;
		   float             m_backGreen;
		   float             m_backBlue;	
		   float             m_emisiveRed;
		   float             m_emisiveGreen;
		   float             m_emisiveBlue;
		   D3DVECTOR         m_sunPosition;
		   BOOL              m_sunLightOn;
		   float             m_sunRed;
		   float             m_sunGreen;
		   float             m_sunBlue;
		   int               m_currentTimeVarial;
           BOOL              m_enableTimeSystem;
		   float             m_ambientLightRedDay;
           float             m_ambientLightGreenDay;
		   float             m_ambientLightBlueDay;
           BOOL              m_ambientLightOn;
		   float             m_windDirectionX;
           float             m_windDirectionY;
           float             m_windDirectionZ;
           BOOL              m_enableWindRandomizer;
		   float             m_maxWindStrength;
		   int               m_windChangeFrequency;
           int               m_lastWindUpdate;
		   float             m_gravity;
	       BOOL              m_atmosphereDensity;
		   float             m_seaLevel;
		   float             m_seaViscusDrag;
		   bool              m_lockBackColor;


		   D3DVECTOR         m_currentWindPushVect;
		   float             m_maxWindPush;
		   float             m_minWindPush;
		   float             m_curWindPush;
		   float             m_curWindAdd;
		   float             m_curWindMinus;
		   BOOL              m_curBurstDir;
		   long              m_windCycleDuration;
		   long              m_windCycleLastUpdate;
	
    //Functions
	BOOL SetWorldCurrentTime(long currentSystemDay,
										  long currentSystemYear,
										  long currentSystemMonth,
										  long currentDayTime);


    BOOL ReinitAllTextures( CTextureDBObjList * textureDataBase,
		                    CTextureDBObjList * dynTextureDataBase,
						    LPDIRECT3DDEVICE9 g_pd3dDevice,
							CStateManagementObj * stateObj,
							BOOL bitmapLoadOverride,
							int & maxTextureSize);
    
	BOOL GetRandomWindDirection();
	float InterpolateFloats(
						 float StartVec, 
						 float EndVec,
					     int NumOfStages, 
					     int ThisStage);
	VOID D3DUtil_InitLight( D3DLIGHT9& light, D3DLIGHTTYPE ltType,FLOAT x, FLOAT y, FLOAT z );
	 DWORD FtoDW( FLOAT f ) { return *((DWORD*)&f); }
	BOOL CallBack(int CurrentTime,
		          LPDIRECT3DDEVICE9 device,
				  CFrameObj* scene,
				  D3DCOLOR * BackgroundColorGlobal,
				  EnvParticleSystemList * envpParticleList,
				  CLightningObjList * lightningEffectSystem,
				  IDirectSound8 * directSoundPtr,
				  D3DMATRIX soundCenter,
				  CSoundBankList * soundManager,
				  BOOL & renderEnviorment,
				  CStateManagementObj * stateManager,
				  CEXMeshObjList * charonaList);
	void SafeDelete();
	CString ConvToString(int number);
    BOOL InitAll(LPDIRECT3DDEVICE9 device,
		         CFrameObj* scene,
				 D3DCOLOR * BackgroundColorGlobal,
				 IDirectSound8 * directSoundPtr,
				 CStateManagementObj * stateManager,
				 long currentTimePassed);
	
	void Serialize( CArchive& ar );
};

#endif