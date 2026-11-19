#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CStormClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CStormObj::CStormObj()
{
     m_stormName           = "NA";
    m_particleLink        = NULL;
    m_particleSystemRef   = -1;
    m_duration            = 20000;
    m_stormTransition     = 1000;
	m_frequency           = 10;
    m_fogRedMod           = .5f;
    m_fogGreenMod         = .5f;
    m_fogBlueMod          = .5f;
	m_sunColorRedMod      = .5f;
	m_sunColorGreenMod    = .5f;
	m_sunColorBlueMod     = .5f;


	//m_textureNameArray          = NULL;
	//m_textureIndexArray         = NULL;
	m_VariationLightningCount   = 0;
	m_totalStrikeCountPerMinute = 50;
    m_lightningStamp            = 0;
    m_strikeDuration            = 500;
	m_strikeVariationArray      = NULL;
    m_strikePanelWidth          = 300;
    m_strikeSpawnHieght         = 2000;
    m_strikeDepth               = 2500;
	m_stormRadius               = 3000.0;
}

BOOL CStormObj::RemoveLightningStrike(int index)
{

   if(!m_strikeVariationArray)
       return FALSE;

   if(m_VariationLightningCount <= index)
	   return FALSE;

   StrikeVariation * strikeVariationTemp = new StrikeVariation[m_VariationLightningCount-1];
   //transfer array
    int loop=0;
    for(loop=0;loop<m_VariationLightningCount;loop++)
	{
		if(index != loop)
		{
		 strikeVariationTemp[loop].textureByIndex = m_strikeVariationArray[loop].textureByIndex;
		 lstrcpy(strikeVariationTemp[loop].textureName,m_strikeVariationArray[loop].textureName);
		 strikeVariationTemp[loop].m_sndID = m_strikeVariationArray[loop].m_sndID;
         strikeVariationTemp[loop].m_soundFactor = m_strikeVariationArray[loop].m_soundFactor;
		}
	}
    if(m_strikeVariationArray){//delete existing 
          delete [] m_strikeVariationArray;
	      m_strikeVariationArray=0;
	 }//end delete existing 
     m_VariationLightningCount--;
	    if(m_VariationLightningCount > 0)
		m_strikeVariationArray = new StrikeVariation[m_VariationLightningCount];
	    //transfer back
		for(loop=0;loop<m_VariationLightningCount;loop++)
		{
		 m_strikeVariationArray[loop].textureByIndex = strikeVariationTemp[loop].textureByIndex;
		 lstrcpy(m_strikeVariationArray[loop].textureName,strikeVariationTemp[loop].textureName);
		 m_strikeVariationArray[loop].m_sndID       = strikeVariationTemp[loop].m_sndID;
         m_strikeVariationArray[loop].m_soundFactor = strikeVariationTemp[loop].m_soundFactor;
		}
		return TRUE;
}

void CStormObj::AddLightningStrike(char textureName[256],
								   int sndID,
								   float soundFactor)
{
	 StrikeVariation * strikeVariationTemp = new StrikeVariation[m_VariationLightningCount+1];
     //transfer array
	 int loop=0;
     for(loop=0;loop<m_VariationLightningCount;loop++)
	 {
		 strikeVariationTemp[loop].textureByIndex = m_strikeVariationArray[loop].textureByIndex;
		 lstrcpy(strikeVariationTemp[loop].textureName,m_strikeVariationArray[loop].textureName);
		 strikeVariationTemp[loop].m_sndID = m_strikeVariationArray[loop].m_sndID;
         strikeVariationTemp[loop].m_soundFactor = m_strikeVariationArray[loop].m_soundFactor;
	 }
	 lstrcpy(strikeVariationTemp[m_VariationLightningCount].textureName,textureName);
	 strikeVariationTemp[loop].m_sndID = sndID;
     strikeVariationTemp[loop].m_soundFactor = soundFactor;
     strikeVariationTemp[m_VariationLightningCount].textureByIndex = 0;
	 if(m_strikeVariationArray){//delete existing 
          delete [] m_strikeVariationArray;
	      m_strikeVariationArray=0;
	 }//end delete existing 
	 m_VariationLightningCount++;
	    if(m_VariationLightningCount > 0)
		m_strikeVariationArray = new StrikeVariation[m_VariationLightningCount];
	    //transfer back
		for(loop=0;loop<m_VariationLightningCount;loop++)
		{
		 m_strikeVariationArray[loop].textureByIndex = strikeVariationTemp[loop].textureByIndex;
		 lstrcpy(m_strikeVariationArray[loop].textureName,strikeVariationTemp[loop].textureName);
		 m_strikeVariationArray[loop].m_sndID       = strikeVariationTemp[loop].m_sndID;
         m_strikeVariationArray[loop].m_soundFactor = strikeVariationTemp[loop].m_soundFactor;
		}
}

void CStormObj::Clone(CStormObj ** clone)
{
    CStormObj * copyLocal                  = new CStormObj();
    copyLocal->m_stormName                 = m_stormName;
	copyLocal->m_particleSystemRef         = m_particleSystemRef;
	copyLocal->m_duration                  = m_duration;
	copyLocal->m_stormTransition           = m_stormTransition;
	copyLocal->m_frequency                 = m_frequency;
	copyLocal->m_fogRedMod                 = m_fogRedMod;
    copyLocal->m_fogGreenMod               = m_fogGreenMod;
    copyLocal->m_fogBlueMod                = m_fogBlueMod;
	copyLocal->m_sunColorRedMod            = m_sunColorRedMod;
	copyLocal->m_sunColorGreenMod          = m_sunColorGreenMod;
	copyLocal->m_sunColorBlueMod           = m_sunColorBlueMod;
	copyLocal->m_VariationLightningCount   = m_VariationLightningCount;
	copyLocal->m_totalStrikeCountPerMinute = m_totalStrikeCountPerMinute;
	copyLocal->m_lightningStamp            = m_lightningStamp;
    copyLocal->m_strikeDuration            = m_strikeDuration;
	copyLocal->m_strikePanelWidth          = m_strikePanelWidth;
    copyLocal->m_strikeSpawnHieght         = m_strikeSpawnHieght;
    copyLocal->m_strikeDepth               = m_strikeDepth;
	copyLocal->m_stormRadius               = m_stormRadius;
	
    if(m_VariationLightningCount > 0)
	{
	 copyLocal->m_strikeVariationArray = new StrikeVariation[m_VariationLightningCount];
     for(int loop=0;loop<m_VariationLightningCount;loop++)
	 {
		 copyLocal->m_strikeVariationArray[loop].textureByIndex = m_strikeVariationArray[loop].textureByIndex;
		 lstrcpy(copyLocal->m_strikeVariationArray[loop].textureName,m_strikeVariationArray[loop].textureName);
	     copyLocal->m_strikeVariationArray[loop].m_sndID        = m_strikeVariationArray[loop].m_sndID;
         copyLocal->m_strikeVariationArray[loop].m_soundFactor  = m_strikeVariationArray[loop].m_soundFactor;
	 }
	
	}
    *clone = copyLocal;
}

void CStormObj::SafeDelete()
{
  if(m_strikeVariationArray)
  {
     delete [] m_strikeVariationArray;
	 m_strikeVariationArray=0;
  }
  if(m_particleLink)
  {
     m_particleLink->m_Regenerate = FALSE;
	 m_particleLink = NULL;
  }

}

void CStormObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CStormObj * spnPtr = (CStormObj *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

void CStormObj::Serialize( CArchive& ar )
{

CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_stormName 
		  << m_particleSystemRef
		  << m_duration
		  << m_stormTransition
		  << m_frequency
		  << m_fogRedMod
		  << m_fogGreenMod
		  << m_fogBlueMod
		  << m_sunColorRedMod
		  << m_sunColorGreenMod
		  << m_sunColorBlueMod
          << m_VariationLightningCount
	      << m_totalStrikeCountPerMinute
	      << m_lightningStamp
          << m_strikeDuration
	      << m_strikePanelWidth
          << m_strikeSpawnHieght
          << m_strikeDepth
		  << m_stormRadius;


           for(int loop=0;loop<m_VariationLightningCount;loop++)
		   {
			   CString textureName;
			   textureName = m_strikeVariationArray[loop].textureName;
               //CSoundRangeObject * soundObject = NULL;
			   ar << m_strikeVariationArray[loop].textureByIndex
				  << m_strikeVariationArray[loop].m_sndID
				  << m_strikeVariationArray[loop].m_soundFactor
				  << textureName;
		   }
	}
	else
	{
       ar >> m_stormName 
		  >> m_particleSystemRef
		  >> m_duration
		  >> m_stormTransition
		  >> m_frequency
		  >> m_fogRedMod
		  >> m_fogGreenMod
		  >> m_fogBlueMod
		  >> m_sunColorRedMod
		  >> m_sunColorGreenMod
		  >> m_sunColorBlueMod
		  >> m_VariationLightningCount
	      >> m_totalStrikeCountPerMinute
		  >> m_lightningStamp
          >> m_strikeDuration
          >> m_strikePanelWidth        
          >> m_strikeSpawnHieght       
          >> m_strikeDepth
		  >> m_stormRadius;

           if(m_VariationLightningCount > 0)
	       m_strikeVariationArray = new StrikeVariation[m_VariationLightningCount];
	       for(int loop=0;loop<m_VariationLightningCount;loop++)
		   {
			   CString textureName;
               CSoundRangeObject * soundObject = NULL;
			   ar >> m_strikeVariationArray[loop].textureByIndex
				  >> m_strikeVariationArray[loop].m_sndID
				  >> m_strikeVariationArray[loop].m_soundFactor
				  >> textureName;

			   lstrcpy(m_strikeVariationArray[loop].textureName, textureName);
		   }

   
	   m_particleLink        = NULL;
	}
}     




IMPLEMENT_SERIAL(CStormObj, CObject, 0)
IMPLEMENT_SERIAL(CStormObjList, CObList, 0)
