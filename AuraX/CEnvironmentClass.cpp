//#include "matrixarb.h"
#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "math.h"

#include "CEnvironmentClass.h"
#include "matrixArb.h"

// Characters, NPCs and items are lit by the zone light (sun + ambient). Indoor zones often have
// no sun and no ambient, which drew them pitch black. When the zone would light them with
// (almost) nothing, keep a soft minimum ambient. Zones that already light them are unchanged.
static const float AURA_MIN_AMBIENT = 0.30f;
static void AuraAmbientFloor(float & r, float & g, float & b, BOOL sunOn, float sr, float sg, float sb)
{
	float amb = (r > g) ? ((r > b) ? r : b) : ((g > b) ? g : b);
	float sun = sunOn ? ((sr > sg) ? ((sr > sb) ? sr : sb) : ((sg > sb) ? sg : sb)) : 0.0f;
	if (amb >= AURA_MIN_AMBIENT || sun >= 0.15f)
		return;
	if (r < AURA_MIN_AMBIENT) r = AURA_MIN_AMBIENT;
	if (g < AURA_MIN_AMBIENT) g = AURA_MIN_AMBIENT;
	if (b < AURA_MIN_AMBIENT) b = AURA_MIN_AMBIENT;
}


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CEnvironmentObj::CEnvironmentObj()
{
    m_fogOn                                   = FALSE; 
    m_fogDensity                              = 1.0f;
    m_fogRange                                = 3000.0f;
    m_fogStart                                = 300.0f;
    m_fogRed                                  = 0.0f;
    m_fogGreen                                = 0.0f;
	m_fogBlue                                 = 0.0f;
	m_backRed                                 = 0.0f;
	m_backGreen                               = 0.0f;
	m_backBlue                                = 0.0f;
	m_emisiveRed                              = 0.0f;
    m_emisiveGreen                            = 0.0f;
    m_emisiveBlue                             = 0.0f;
	m_sunPosition.x                           = 0.0f;
	m_sunPosition.y                           = 1000.0f;
	m_sunPosition.z                           = 1000.0f;
	m_sunLightOn                              = TRUE;
	m_sunRed                                  = 1.0f;
    m_sunGreen                                = 1.0f;
    m_sunBlue                                 = 1.0f;
    m_currentTimeVarial                       = 0;
    m_enableTimeSystem                        = FALSE;
	m_ambientLightRedDay                      = .2f;
    m_ambientLightGreenDay                    = .2f;
    m_ambientLightBlueDay                     = .2f;
    m_ambientLightOn                          = TRUE;
    m_windDirectionX                          = 0.0f;
    m_windDirectionY                          = 0.0f;
    m_windDirectionZ                          = 0.0f;
    m_enableWindRandomizer                    = FALSE;
    m_maxWindStrength                         = 0.0f;
    m_windChangeFrequency                     = 0;
    m_lastWindUpdate                          = 0;
	m_gravity                                 = -.1f;
    m_atmosphereDensity                       = 0;	   
    m_seaLevel                                = 0.0f;
    m_seaViscusDrag                           = .97f;
    m_enviormentTimeSystem                    = NULL;
	m_lockBackColor                           = false;

	m_currentWindPushVect.x                   = 1.0f;
	m_currentWindPushVect.y                   = 0.0f;
	m_currentWindPushVect.z                   = 1.0f;
	m_maxWindPush                             = 2.0f;
	m_minWindPush                             = .01f;
	m_curWindPush                             = 0.0f;
	m_windCycleDuration                       = 2000;
	m_curWindAdd                              = 0.0f;
	m_curWindMinus                            = 0.0f;
	m_curBurstDir                             = TRUE;
	m_windCycleLastUpdate                     = GetTickCount();
}

void CEnvironmentObj::SafeDelete()
{
//----------Destroy environment objects-------------//
   if(m_enviormentTimeSystem)
   {
      m_enviormentTimeSystem->SafeDelete();
	  delete m_enviormentTimeSystem;
	  m_enviormentTimeSystem=0;
   }
//--------end destroy environment objects-----------//
}
BOOL CEnvironmentObj::SetWorldCurrentTime(long currentSystemDay,
										  long currentSystemYear,
										  long currentSystemMonth,
										  long currentDayTime)
{
   if(!m_enviormentTimeSystem)
       return FALSE;

   
    m_enviormentTimeSystem->m_timeStamp             = GetTickCount();
	m_enviormentTimeSystem->m_currentSystemDay      = currentSystemDay;
	m_enviormentTimeSystem->m_currentSystemMonth    = currentSystemMonth;
	m_enviormentTimeSystem->m_currentSystemYear     = currentSystemYear;
	m_enviormentTimeSystem->m_currentDayTime        = currentDayTime;

return TRUE;
}
//Callback -- 
BOOL CEnvironmentObj::CallBack(int CurrentTime,
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
							   CEXMeshObjList * charonaList
							   )
{
  //calc wind cur factor push
  m_currentWindPushVect.x                   = 1.0f; 
  m_currentWindPushVect.y                   = 0.0f;
  m_currentWindPushVect.z                   = 1.0f;
  m_currentWindPushVect = m_abMathLib.Normalize(m_currentWindPushVect);
 

  m_maxWindPush = m_maxWindStrength;//0.8f;
  float maxVelVect = .02f;
  float maxMinVelVect = -.02f;

      if(m_curBurstDir == TRUE)
	  {
	     m_curWindAdd = m_curWindAdd + (.0004f*stateManager->m_glAdjust);
		 if(m_curWindAdd > maxVelVect)
		    m_curWindAdd = maxVelVect;

	     m_curWindPush += m_curWindAdd;
		 if(m_curWindPush > m_maxWindPush)
		 {
		    m_curBurstDir = FALSE;
			m_curWindMinus = m_curWindAdd;
			m_curWindAdd = 0.0f;
		 }
	  }
	  else if(!m_curBurstDir)
	  {
	     m_curWindMinus = m_curWindMinus - (.0002f*stateManager->m_glAdjust);
		 if(m_curWindMinus < maxMinVelVect)
		    m_curWindAdd = maxMinVelVect;
         
		 m_curWindPush += m_curWindMinus;
		 if(m_curWindPush < .2)
		 {
		    m_curBurstDir = TRUE;
			m_curWindAdd = m_curWindMinus;
			m_curWindMinus = 0.0f;
	     }
	  }

  m_currentWindPushVect.x                   = m_currentWindPushVect.x * m_curWindPush;
  m_currentWindPushVect.y                   = m_currentWindPushVect.y * m_curWindPush;
  m_currentWindPushVect.z                   = m_currentWindPushVect.z * m_curWindPush;
  stateManager->SetWindGlobal(m_currentWindPushVect,device);



  //end calc wind cur factor push
  if(m_enableTimeSystem)
  if(m_enviormentTimeSystem){//valid system
     m_enviormentTimeSystem->UpdateSystem(GetTickCount(),
		                                  lightningEffectSystem,
										  soundCenter,
										  soundManager);

	 

	 if(m_enviormentTimeSystem->m_currentState)
	 {//valid system
        if(renderEnviorment)
		{//set Enviorment
		//fog
        stateManager->SetFogOverrideState(FALSE,device);
		//*BackgroundColorGlobal = D3DRGB(m_backRed,m_backGreen,m_backBlue);
		D3DCOLOR currentfogColor = D3DCOLOR_XRGB((int)(m_enviormentTimeSystem->m_currentState->m_fogColorRed*255.0f),
			                                     (int)(m_enviormentTimeSystem->m_currentState->m_fogColorGreen*255.0f),
									             (int)(m_enviormentTimeSystem->m_currentState->m_fogColorBlue*255.0f));
     
		//D3DRS_FOGTABLEMODE
		device->SetRenderState( D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
		//device->SetRenderState( D3DRS_FOGTABLEMODE, D3DFOG_LINEAR);

		//device->SetRenderState( D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
		device->SetRenderState( D3DRS_FOGTABLEMODE, D3DFOG_NONE);

		device->SetRenderState( D3DRS_FOGCOLOR, currentfogColor);
		device->SetRenderState( D3DRS_FOGSTART, *(DWORD *)(&m_enviormentTimeSystem->m_currentState->m_fogStartRange));
         device->SetRenderState( D3DRS_FOGEND, *(DWORD *)(&m_enviormentTimeSystem->m_currentState->m_fogEndRange) );
		stateManager->SetFogConstant(device,m_enviormentTimeSystem->m_currentState->m_fogStartRange,m_enviormentTimeSystem->m_currentState->m_fogEndRange);
		
		if(!m_lockBackColor)
		   *BackgroundColorGlobal = currentfogColor;
        //set sun 


		D3DLIGHT9 light;
		     D3DUtil_InitLight( light, D3DLIGHT_DIRECTIONAL, m_enviormentTimeSystem->m_currentState->m_sunColorRed,
				                                             m_enviormentTimeSystem->m_currentState->m_sunColorGreen,
															 m_enviormentTimeSystem->m_currentState->m_sunColorBlue);
             //build directional vector
			 D3DVECTOR directionalBuild;
			 directionalBuild.x = 0.0f;
			 directionalBuild.y = 0.0f;
			 directionalBuild.z = 1.0f;
			    
			 D3DMATRIX matMod;

             m_abMathLib.Rotate(m_enviormentTimeSystem->m_currentState->m_sunRotationX,0.0f,0.0f,matMod.m);
			 directionalBuild = m_abMathLib.UpdatePolygonVertex(matMod.m,directionalBuild);

			 m_abMathLib.Rotate(0.0f,m_enviormentTimeSystem->m_currentState->m_sunRotationY,0.0f,matMod.m);
             directionalBuild = m_abMathLib.UpdatePolygonVertex(matMod.m,directionalBuild);

			 directionalBuild = m_abMathLib.Normalize(directionalBuild);
			 
			 light.Direction.x = directionalBuild.x;   
             light.Direction.y = directionalBuild.y;  
             light.Direction.z = directionalBuild.z; 

			 stateManager->SetDirLight(device,directionalBuild,m_enviormentTimeSystem->m_currentState->m_sunColorRed,m_enviormentTimeSystem->m_currentState->m_sunColorGreen,m_enviormentTimeSystem->m_currentState->m_sunColorBlue);

             light.Diffuse.r = m_enviormentTimeSystem->m_currentState->m_sunColorRed;
             light.Diffuse.g = m_enviormentTimeSystem->m_currentState->m_sunColorGreen;
             light.Diffuse.b = m_enviormentTimeSystem->m_currentState->m_sunColorBlue;  
			 light.Specular.r = m_enviormentTimeSystem->m_currentState->m_sunColorRed;
			 light.Specular.g = m_enviormentTimeSystem->m_currentState->m_sunColorGreen;
			 light.Specular.b = m_enviormentTimeSystem->m_currentState->m_sunColorBlue;
			 light.Specular.a = 1.0f;
             device->SetLight( 0, &light );
			 device->LightEnable( 0, TRUE );
             //calculate sun position model
			 m_enviormentTimeSystem->m_currentSunPosition.x = -directionalBuild.x;
             m_enviormentTimeSystem->m_currentSunPosition.y = -directionalBuild.y;
			 m_enviormentTimeSystem->m_currentSunPosition.z = -directionalBuild.z;
			 m_enviormentTimeSystem->m_currentSunPosition = m_abMathLib.Normalize(m_enviormentTimeSystem->m_currentSunPosition);
			 m_enviormentTimeSystem->m_currentSunPosition.x = m_enviormentTimeSystem->m_currentSunPosition.x * m_enviormentTimeSystem->m_currentState->m_sunDistance;
             m_enviormentTimeSystem->m_currentSunPosition.y = m_enviormentTimeSystem->m_currentSunPosition.y * m_enviormentTimeSystem->m_currentState->m_sunDistance;
			 m_enviormentTimeSystem->m_currentSunPosition.z = m_enviormentTimeSystem->m_currentSunPosition.z * m_enviormentTimeSystem->m_currentState->m_sunDistance;
	    //set ambient light (with a soft minimum so characters are never pitch black)
			 {
			 float ambR = m_enviormentTimeSystem->m_currentState->m_ambientColorRed;
			 float ambG = m_enviormentTimeSystem->m_currentState->m_ambientColorGreen;
			 float ambB = m_enviormentTimeSystem->m_currentState->m_ambientColorBlue;
			 AuraAmbientFloor(ambR, ambG, ambB, TRUE,
							  m_enviormentTimeSystem->m_currentState->m_sunColorRed,
							  m_enviormentTimeSystem->m_currentState->m_sunColorGreen,
							  m_enviormentTimeSystem->m_currentState->m_sunColorBlue);
			 device->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_XRGB((int)(ambR*255.0f),(int)(ambG*255.0f),(int)(ambB*255.0f)));
			 stateManager->SetAmbientLight(device,ambR,ambG,ambB);
			 }
			 stateManager->SetFogState(device,m_fogOn);
		     
		}//end set enviorment
		else{//disable enviorment
			  device->LightEnable( 0, FALSE );
			  // no zone light at all: soft minimum so characters are not pitch black
			  device->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_XRGB((int)(AURA_MIN_AMBIENT*255.0f),(int)(AURA_MIN_AMBIENT*255.0f),(int)(AURA_MIN_AMBIENT*255.0f)));
			  stateManager->SetAmbientLight(device,AURA_MIN_AMBIENT,AURA_MIN_AMBIENT,AURA_MIN_AMBIENT);

              if(m_fogOn)
			  stateManager->SetFogState(device,FALSE);

			  *BackgroundColorGlobal = D3DCOLOR_XRGB(0,0,0);
		}//end disable enviorment
	 }//end valid system
  }//end valid system

return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CEnvironmentObj::ReinitAllTextures(CTextureDBObjList * textureDataBase,
										CTextureDBObjList * dynTextureDataBase,
						                LPDIRECT3DDEVICE9 g_pd3dDevice,
										CStateManagementObj * stateObj,
										BOOL bitmapLoadOverride,
									    int & maxTextureSize)
{
   if(m_enviormentTimeSystem)
   {//valid time system
      if(m_enviormentTimeSystem->m_stormDB)
	  {
         for(POSITION posLoc = m_enviormentTimeSystem->m_stormDB->GetHeadPosition();posLoc != NULL;)
		 {
             CStormObj * strPtr = (CStormObj *)m_enviormentTimeSystem->m_stormDB->GetNext(posLoc);
             for(int loop=0;loop<strPtr->m_VariationLightningCount;loop++)
			 {
                 textureDataBase->TextureSearch(strPtr->m_strikeVariationArray[loop].textureName,
				                                strPtr->m_strikeVariationArray[loop].textureByIndex,
												12,
                                                dynTextureDataBase,
												0);

                 /*strPtr->m_strikeVariationArray[loop].textureByIndex = stateObj->TextureReference(
					                                                            strPtr->m_strikeVariationArray[loop].textureName,
                                                                                textureDataBase,
																				g_pd3dDevice,
																				bitmapLoadOverride,
																				maxTextureSize,
																				textureDataBase->m_compressRef);*/
			 }
		 }
	  }
   }//end valid time system

return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------------get new wind direction------------------------------------------//
BOOL CEnvironmentObj::GetRandomWindDirection()
{

D3DVECTOR direction;
direction.x = 0.0f;
direction.y = 0.0f;
direction.z = 0.0f;
direction = CABLIBREF.GetRandomPointInRadius(m_maxWindStrength,direction,0.0f);
m_windDirectionX = direction.x;
m_windDirectionY = direction.y;
m_windDirectionZ = direction.z;
return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Interpolation between animations BABY!! Imediate mode style-----------//
float CEnvironmentObj::InterpolateFloats(
						 float StartVec, 
						 float EndVec,
					     int NumOfStages, 
					     int ThisStage)
{
 float propfactor;
 if (NumOfStages <= 0)
  NumOfStages = 1;
  propfactor = (float)ThisStage/NumOfStages;
  float StageVec = StartVec + propfactor * (EndVec - StartVec);

 return StageVec;
}

//Initialize Environment
BOOL CEnvironmentObj::InitAll(LPDIRECT3DDEVICE9 device,
							  CFrameObj* scene,
							  D3DCOLOR * BackgroundColorGlobal,
							  IDirectSound8 * directSoundPtr,
							  CStateManagementObj * stateManager,
							  long currentTimePassed)
{	
  if(!device)
	  return FALSE;


  if(m_enviormentTimeSystem){
  m_enviormentTimeSystem->StartSystem(GetTickCount(),currentTimePassed);
   if(m_enviormentTimeSystem->m_visual){
    m_enviormentTimeSystem->m_visual->InitBuffer(device,NULL);
	if(m_enviormentTimeSystem->m_visual->m_charona)
	{
     m_enviormentTimeSystem->m_visual->m_charona->InitializeVis();
     m_enviormentTimeSystem->m_visual->m_charona->m_visual->InitBuffer(device,m_enviormentTimeSystem->m_visual->m_charona->m_visual->m_uvCount);
	}
   }
  }

  m_lastWindUpdate = 0;
  m_currentTimeVarial = GetTickCount();
//------------------//
   D3DCOLOR colorLoc = D3DCOLOR_XRGB((int)(m_fogRed*255.0f),
	                                 (int)(m_fogGreen*255.0f),
									 (int)(m_fogBlue*255.0f));
   *BackgroundColorGlobal = D3DCOLOR_XRGB((int)(m_backRed*255.0f),
	                                      (int)(m_backGreen*255.0f),
										  (int)(m_backBlue*255.0f));
	     //set fog shiap if required
         FLOAT fFogEnd = m_fogRange;
         FLOAT fFogStart = m_fogStart;
         FLOAT fFogDensity = m_fogDensity;
		 stateManager->SetFogState(device,m_fogOn);

         //device->SetRenderState( D3DRS_FOGVERTEXMODE, D3DFOG_LINEAR);
		 device->SetRenderState( D3DRS_FOGTABLEMODE, D3DFOG_NONE);

		 //device->SetRenderState( D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
	 	//device->SetRenderState( D3DRS_FOGTABLEMODE, D3DFOG_NONE);

         device->SetRenderState( D3DRS_FOGCOLOR, colorLoc);
         device->SetRenderState( D3DRS_FOGSTART, *(DWORD *)(&fFogStart));
         device->SetRenderState( D3DRS_FOGEND, *(DWORD *)(&fFogEnd) );
         stateManager->SetFogConstant(device,fFogStart,fFogEnd);
         //device->SetRenderState( D3DRS_FOGDENSITY, FtoDW(0.0f) );
//------------------//
//---sun setup------//
//------------------//
   if(m_sunLightOn == TRUE){//sun should be on
      //init variables
      m_currentTimeVarial = GetTickCount();
             D3DLIGHT9 light;
             D3DUtil_InitLight( light, D3DLIGHT_DIRECTIONAL, m_sunRed,m_sunGreen,m_sunBlue);
             
			 D3DVECTOR temp;
			 temp.x = m_sunPosition.x;
			 temp.y = m_sunPosition.y;
			 temp.z = m_sunPosition.z;
			 m_abMathLib.Normalize(temp);
			 light.Range = 0.0f;
			 light.Direction.x = -temp.x;   
             light.Direction.y = -temp.y;  
             light.Direction.z = -temp.z; 
             light.Diffuse.r = m_sunRed;
             light.Diffuse.g = m_sunGreen;
             light.Diffuse.b = m_sunBlue; 
			 light.Specular.r = m_sunRed;
			 light.Specular.g = m_sunGreen;
			 light.Specular.b = m_sunBlue;
			 light.Specular.a = 1.0f;
             device->SetLight( 0, &light );
             device->LightEnable( 0, TRUE );
   }//end sun should be on
   else
   device->LightEnable( 0, FALSE );
//------------------//
//---ambientLight---//
//------------------//
   {//ambient light (with a soft minimum so characters are never pitch black)
	  float ambR = 0.0f, ambG = 0.0f, ambB = 0.0f;
	  if(m_ambientLightOn == TRUE)
	  { ambR = m_ambientLightRedDay; ambG = m_ambientLightGreenDay; ambB = m_ambientLightBlueDay; }
	  AuraAmbientFloor(ambR, ambG, ambB, m_sunLightOn, m_sunRed, m_sunGreen, m_sunBlue);
	  device->SetRenderState(D3DRS_AMBIENT,D3DCOLOR_XRGB((int)(ambR*255.0f),(int)(ambG*255.0f),(int)(ambB*255.0f)));
	  stateManager->SetAmbientLight(device,ambR,ambG,ambB);
   }
//------------------//
//-storms sounds-//
//------------------//
//reset stuff
   if(m_enviormentTimeSystem){
       if(m_enviormentTimeSystem->m_currentState)
	   {
          m_enviormentTimeSystem->m_currentState->SafeDelete();
          delete m_enviormentTimeSystem->m_currentState;
          m_enviormentTimeSystem->m_currentState = 0;
	   }

	   if(m_enviormentTimeSystem->m_stormPtr)
	   {
          m_enviormentTimeSystem->m_stormPtr->SafeDelete();
          delete m_enviormentTimeSystem->m_stormPtr;
          m_enviormentTimeSystem->m_stormPtr = 0;
	   }
   }


   return TRUE;
}		   



void CEnvironmentObj::Serialize( CArchive& ar )
{
BOOL reservedBOOL = FALSE;

CObject::Serialize( ar );
	if(ar.IsStoring() ){
 
       ar.SetObjectSchema(2);
       ASSERT(ar.GetObjectSchema() == 2);

	   ar <<   m_fogOn
          <<   m_fogDensity
          <<   m_fogRange 
          <<   m_fogStart   
          <<   m_fogRed                 
          <<   m_fogGreen                     
	      <<   m_fogBlue                           
	      <<   m_backRed                               
	      <<   m_backGreen                             
	      <<   m_backBlue                                
	      <<   m_emisiveRed                              
          <<   m_emisiveGreen                           
          <<   m_emisiveBlue                           
	      <<   m_sunPosition.x                         
	      <<   m_sunPosition.y                        
	      <<   m_sunPosition.z                         
	      <<   m_sunLightOn                             
	      <<   m_sunRed                                
          <<   m_sunGreen                              
          <<   m_sunBlue                                                
          <<   m_enableTimeSystem                      
	      <<   m_ambientLightRedDay                   
          <<   m_ambientLightGreenDay                 
          <<   m_ambientLightBlueDay                  
          <<   m_ambientLightOn                       
          <<   m_windDirectionX                       
          <<   m_windDirectionY                       
          <<   m_windDirectionZ                        
          <<   m_enableWindRandomizer                   
          <<   m_maxWindStrength                        
          <<   m_windChangeFrequency                                       
	      <<   m_gravity                                
          <<   m_atmosphereDensity                      
          <<   m_seaLevel                               
          <<   m_seaViscusDrag                          
          <<   m_enviormentTimeSystem;
		
	   ar <<   m_lockBackColor
		  <<   reservedBOOL
		  <<   reservedBOOL
		  <<   reservedBOOL;                 

	}
	else
	{

	   int version = ar.GetObjectSchema();

       ar >>   m_fogOn
          >>   m_fogDensity
          >>   m_fogRange 
          >>   m_fogStart   
          >>   m_fogRed                 
          >>   m_fogGreen                     
	      >>   m_fogBlue                           
	      >>   m_backRed                               
	      >>   m_backGreen                             
	      >>   m_backBlue                                
	      >>   m_emisiveRed                              
          >>   m_emisiveGreen                           
          >>   m_emisiveBlue                           
	      >>   m_sunPosition.x                         
	      >>   m_sunPosition.y                        
	      >>   m_sunPosition.z                         
	      >>   m_sunLightOn                             
	      >>   m_sunRed                                
          >>   m_sunGreen                              
          >>   m_sunBlue                                                
          >>   m_enableTimeSystem                      
	      >>   m_ambientLightRedDay                   
          >>   m_ambientLightGreenDay                 
          >>   m_ambientLightBlueDay                  
          >>   m_ambientLightOn                       
          >>   m_windDirectionX                       
          >>   m_windDirectionY                       
          >>   m_windDirectionZ                        
          >>   m_enableWindRandomizer                   
          >>   m_maxWindStrength                        
          >>   m_windChangeFrequency                                       
	      >>   m_gravity                                
          >>   m_atmosphereDensity                      
          >>   m_seaLevel                               
          >>   m_seaViscusDrag                          
          >>   m_enviormentTimeSystem;  

	   m_lockBackColor = false;

	   if(version > 1)
	   {
	   ar >>   m_lockBackColor
		  >>   reservedBOOL
		  >>   reservedBOOL
		  >>   reservedBOOL;  
	   }

	   m_currentTimeVarial = 0;
	   m_lastWindUpdate    = 0;
	}
	   
}


//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----converts a long into string-----------------------------//
CString CEnvironmentObj::ConvToString(int number)
{    
	          CString temp;
			  temp.Format("%i",number);
              /*if(number==0)
				 return "0";
	 
	          int decimal,sign;
              char *buffer;
              CString temp;	 
	          buffer = _fcvt(number,0,&decimal,&sign);
    	      temp = buffer;
			  if(sign != 0)
                 temp = operator+("-",temp);*/
		
			  return temp;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----converts a long into string-----------------------------//
VOID CEnvironmentObj::D3DUtil_InitLight( D3DLIGHT9& light, D3DLIGHTTYPE ltType,
                        FLOAT x, FLOAT y, FLOAT z )
{
    ZeroMemory( &light, sizeof(D3DLIGHT9) );
    light.Type        = ltType;
    light.Diffuse.r   = 1.0f;
    light.Diffuse.g   = 1.0f;
    light.Diffuse.b   = 1.0f;
    light.Specular    = light.Diffuse;
    light.Position.x   = light.Direction.x = x;
    light.Position.y   = light.Direction.y = y;
    light.Position.z   = light.Direction.z = z;
    light.Attenuation0 = 1.0f;
    light.Range        = 0.0f;
}


IMPLEMENT_SERIAL(CEnvironmentObj, CObject, VERSIONABLE_SCHEMA|2)
