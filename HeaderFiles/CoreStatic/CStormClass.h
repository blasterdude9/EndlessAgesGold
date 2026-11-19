
#include "EnvParticleSystem.h"
#include "matrixarb.h"
#include "CLightningEffectClass.h"
#include "CSSoundRange.h"

#ifndef CSTORMCLASS_H
#define CSTORMCLASS_H

struct StrikeVariation
{
   char textureName[256];
   int  textureByIndex;
   int   m_sndID;
   float m_soundFactor;
};



class   CStormObj : public CObject
{
	
     
	DECLARE_SERIAL( CStormObj );
	
    
public:
    CStormObj();
	void Clone(CStormObj ** clone);
    
	CString               m_stormName;
    EnvParticleSystem *   m_particleLink;
	int                   m_particleSystemRef;
	long                  m_duration;
    long                  m_stormTransition;
	long                  m_frequency;//percentage possibility Of storming out of 100%
    
	
	float                 m_fogRedMod;
	float                 m_fogGreenMod;
	float                 m_fogBlueMod;

	float                 m_sunColorRedMod;
	float                 m_sunColorGreenMod;
	float                 m_sunColorBlueMod;

	MatrixARB             m_abMath;

	StrikeVariation      *m_strikeVariationArray;
	int                   m_VariationLightningCount;
	int                   m_totalStrikeCountPerMinute;
	long                  m_lightningStamp;
    long                  m_strikeDuration;

	float                 m_strikePanelWidth;
	float                 m_strikeSpawnHieght;
	float                 m_strikeDepth;
	float                 m_stormRadius;


	void SafeDelete();
	BOOL RemoveLightningStrike(int index);
	void AddLightningStrike(char textureName[256],
								   int sndID,
								   float soundFactor);
	void Serialize( CArchive& ar );
};

class   CStormObjList : public CObList
{
public:
   CStormObjList(){};

   void SafeDelete();
    
     
	DECLARE_SERIAL( CStormObjList );
	
    
};

#endif