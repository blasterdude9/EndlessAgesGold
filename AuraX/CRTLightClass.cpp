#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CRTLightClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CRTLightObj::CRTLightObj()
{
   m_material        = NULL;
   m_range           = 0.0f;
   m_attenuation0    = 1.0f;
   m_attenuation1    = 0.0f;
   m_attenuation2    = 0.0f;
   m_type			 = D3DLIGHT_POINT;//DPD default type to point, 0.0 for spot properties, 0 0 1 for dir
   m_direction.x	 = 0;
   m_direction.y	 = 0;
   m_direction.z	 = 1;
   m_theta		     = 0.0f;
   m_phi			 = 0.0f;
   m_falloff		 = 0.0f;
   m_blinkOn		 = FALSE;
   m_blinkDur		 = 0;
   m_randomOn		 = FALSE;

   ZeroMemory(&m_wldspacePos,sizeof(D3DVECTOR));
}

CRTLightObj::~CRTLightObj()
{
   if(m_material)
   {
      delete m_material;
	  m_material = 0;
   }
}

void CRTLightObj::Clone(CRTLightObj ** clone)
{
   CRTLightObj * copyLocal = new CRTLightObj();
	
   if(m_material)
	  m_material->Clone(&copyLocal->m_material);
   
   copyLocal->m_range           = m_range;
   copyLocal->m_attenuation0    = m_attenuation0;
   copyLocal->m_attenuation1    = m_attenuation1;
   copyLocal->m_attenuation2    = m_attenuation2;
   
   //DPD
   copyLocal->m_type	= m_type;
   memcpy(&copyLocal->m_direction, &m_direction, sizeof(D3DVECTOR));
   copyLocal->m_theta	= m_theta;
   copyLocal->m_phi		= m_phi;
   copyLocal->m_falloff	= m_falloff;

   //DPD4
   copyLocal->m_blinkOn  = m_blinkOn;
   copyLocal->m_blinkDur = m_blinkDur;
   copyLocal->m_randomOn = m_randomOn;

   copyLocal->m_lightOn  = m_lightOn;

   *clone = copyLocal;
}

CRTLightObjList::~CRTLightObjList()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CRTLightObj * spnPtr = (CRTLightObj *)GetNext(posLoc);	
		delete spnPtr;
		spnPtr = 0;
	}
	RemoveAll();
}

 void CRTLightObjList::FlushSys(LPDIRECT3DDEVICE9 g_pd3dDevice,CStateManagementObj * stateManager)
{
	int index = 1;
	//for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
      //  CRTLightObj * spnPtr = (CRTLightObj *)GetNext(posLoc);	//do not use this pointer at this point it can be invalidated simple zero tech
		//g_pd3dDevice->LightEnable( index, FALSE );
		//stateManager->SetSTLight( index,g_pd3dDevice, m_zeroVect, 0.0f, 0.0f, 0.0f, 0.0f);
		//index++;
		//if(index > 8)
		   //break;
	//}

   

	RemoveAll();
	m_lastQuerryPos.x=-5000123.0f;
	m_lastShaderMode = -10;
}

 void CRTLightObjList::AddLight(CRTLightObj * light,
									  D3DVECTOR & position)
{		
	if(!light->m_material)
		return;

	//DPD4 Blinking
	if(light->m_blinkOn)
	{
		long blinkd;

		if(light->m_randomOn)
		{
			blinkd = ( rand() ) / ( RAND_MAX + 1 ) * light->m_blinkDur  ;
		}
		blinkd = light->m_blinkDur;
		light->m_material->SetFadeIn(blinkd, TRUE);
	}

	light->m_material->Callback();

	//set position
	light->m_wldspacePos.x = position.x;      
	light->m_wldspacePos.y = position.y;       
	light->m_wldspacePos.z = position.z;

	AddTail(light);
}

 void CRTLightObjList::AddLight(CRTLightObj * light,
									  D3DVECTOR & position,
									  D3DVECTOR & charPosition)
{		
	if(!light->m_material)
		return;

	//DPD4 Blinking
	if(light->m_blinkOn)
	{
		long blinkd;

		if(light->m_randomOn)
		{
			blinkd =  ( rand() ) / ( RAND_MAX + 1 ) * light->m_blinkDur  ;
		}
		else
		{
            blinkd = light->m_blinkDur;
		}

		light->m_material->SetFadeIn(blinkd, TRUE);
	}

	light->m_material->Callback();
		 
	//set position
	light->m_wldspacePos.x = position.x;      
	light->m_wldspacePos.y = position.y;       
	light->m_wldspacePos.z = position.z;


	AddTail(light);
}


void CRTLightObjList::SetLightingConstants(LPDIRECT3DDEVICE9 g_pd3dDevice,CStateManagementObj * stateManager,D3DVECTOR & offset)
{
	    int lightCount = GetCount();    
   
		if(lightCount < 1)
		{
		   //loop constants
			int loopReg[4];
			loopReg[0] = lightCount;//key frame count
			loopReg[1] = 0;//initial value of AL internal vertex register
			loopReg[2] = 1;//loop stride
			loopReg[3] = 0;//reserved
			g_pd3dDevice->SetVertexShaderConstantI(2,loopReg,1);
		   return;
		}

		if(lightCount > 30)
		   lightCount = 30;
		//loop constants
		int loopReg[4];
		loopReg[0] = lightCount;//key frame count
        loopReg[1] = 150;//initial value of AL internal vertex register
		loopReg[2] = 2;//loop stride
		loopReg[3] = 0;//reserved
		g_pd3dDevice->SetVertexShaderConstantI(2,loopReg,1);

		int constantStartPoint = 150;
		//key frames
		
        for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
		{
			CRTLightObj * light = (CRTLightObj *)GetNext(posLoc);	
			
			m_lightImStructTemp.Position.x = light->m_wldspacePos.x - offset.x;      
			m_lightImStructTemp.Position.y = light->m_wldspacePos.y - offset.y;  
			m_lightImStructTemp.Position.z = light->m_wldspacePos.z - offset.z;
			stateManager->SetSTLightCustom(constantStartPoint,g_pd3dDevice, m_lightImStructTemp.Position, light->m_material->m_useMaterial.Diffuse.r, light->m_material->m_useMaterial.Diffuse.g, light->m_material->m_useMaterial.Diffuse.b, light->m_range);
			constantStartPoint += 2;
			if(constantStartPoint > 210)
			   break;
		}
		return;
}


void CRTLightObjList::EnableLightingPerObject(LPDIRECT3DDEVICE9 g_pd3dDevice,
									          D3DVECTOR & offset,
									          CStateManagementObj * stateManager,
											  D3DVECTOR & objectCenter,
											  float & objectRadius,
											  int & shaderMode)
{		

    //SetLightingConstants(g_pd3dDevice,stateManager,offset);//remove

	//tolerance rese t
	if(m_lastShaderMode == shaderMode)
      if(m_lastQuerryPos.x - objectCenter.x < 2.0)
        if(m_lastQuerryPos.x - objectCenter.x > -2.0)
		  if(m_lastQuerryPos.y - objectCenter.y < 2.0)
            if(m_lastQuerryPos.y - objectCenter.y > -2.0)
			  if(m_lastQuerryPos.z - objectCenter.z < 2.0)
                if(m_lastQuerryPos.z - objectCenter.z > -2.0)
				  return;

	CopyMemory(&m_lastQuerryPos,&objectCenter,sizeof(D3DVECTOR));

    m_lastShaderMode = shaderMode;

	m_maxLights = m_fixedMaxLightCount;
	if(shaderMode)
	    m_maxLights = m_progMaxLightCount;
    m_indexTemp = 1;
	
	m_maxLightTrace = 0;
   
	//build collection box

    float curRange = 0.0f;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;)
	{
        CRTLightObj * light = (CRTLightObj *)GetNext(posLoc);	
	    curRange = objectRadius + light->m_range;
        if(light->m_wldspacePos.x - objectCenter.x > curRange)
		   continue;  

		if(light->m_wldspacePos.x - objectCenter.x < -curRange)
		   continue;  

		if(light->m_wldspacePos.y - objectCenter.y > curRange)
		   continue;  

		if(light->m_wldspacePos.y - objectCenter.y < -curRange)
		   continue;  

		if(light->m_wldspacePos.z - objectCenter.z > curRange)
		   continue;  

		if(light->m_wldspacePos.z - objectCenter.z < -curRange)
		   continue;  
		
            
 
		if(!shaderMode)
		{
			ZeroMemory(&m_lightImStructTemp,sizeof(D3DLIGHT9));
			switch(light->m_type)
			{
			case D3DLIGHT_POINT:
				m_lightImStructTemp.Type = D3DLIGHT_POINT;
				break;
			case D3DLIGHT_SPOT: 
				m_lightImStructTemp.Type	    = D3DLIGHT_SPOT;
				m_lightImStructTemp.Theta		= light->m_theta;
				m_lightImStructTemp.Phi		= light->m_phi;
				m_lightImStructTemp.Falloff	= light->m_falloff;
				memcpy(&m_lightImStructTemp.Direction, &light->m_direction, sizeof(D3DVECTOR));
				break;
			}
			
		    CopyMemory(&m_lightImStructTemp.Diffuse,&light->m_material->m_useMaterial.Diffuse,sizeof(D3DCOLORVALUE));
				
			if(light->m_randomOn)//random intensity
			{
				float blinkd;
				int range;
				range = light->m_intMax - light->m_intMin;

				if(range)
					blinkd = (float)(light->m_intMin + (rand()%range))/100.0f;
				else
					blinkd = 1.0f;

				m_lightImStructTemp.Diffuse.r  *= blinkd;   
				m_lightImStructTemp.Diffuse.g  *= blinkd;   
				m_lightImStructTemp.Diffuse.b  *= blinkd;  
				m_lightImStructTemp.Diffuse.a  *= blinkd;
			}

			//set ambient / specular
			CopyMemory(&m_lightImStructTemp.Ambient,&light->m_material->m_useMaterial.Ambient,sizeof(D3DCOLORVALUE));
			CopyMemory(&m_lightImStructTemp.Specular,&light->m_material->m_useMaterial.Specular,sizeof(D3DCOLORVALUE));
			//set range
			m_lightImStructTemp.Range = light->m_range;
	    
			//set position
			m_lightImStructTemp.Position.x = light->m_wldspacePos.x - offset.x;      
			m_lightImStructTemp.Position.y = light->m_wldspacePos.y - offset.y;  
			m_lightImStructTemp.Position.z = light->m_wldspacePos.z - offset.z;     
	                      
			//set attenuation 
			m_lightImStructTemp.Attenuation0 = light->m_attenuation0;     
			m_lightImStructTemp.Attenuation1 = light->m_attenuation1;     
			m_lightImStructTemp.Attenuation2 = light->m_attenuation2; 
			g_pd3dDevice->SetLight(    m_indexTemp , &m_lightImStructTemp);
			g_pd3dDevice->LightEnable( m_indexTemp , TRUE );
			
		}
		else
		{
			m_lightImStructTemp.Position.x = light->m_wldspacePos.x - offset.x;      
			m_lightImStructTemp.Position.y = light->m_wldspacePos.y - offset.y;  
			m_lightImStructTemp.Position.z = light->m_wldspacePos.z - offset.z;     
			stateManager->SetSTLight(m_indexTemp,g_pd3dDevice, m_lightImStructTemp.Position, light->m_material->m_useMaterial.Diffuse.r, light->m_material->m_useMaterial.Diffuse.g, light->m_material->m_useMaterial.Diffuse.b, light->m_range);
		}
		m_indexTemp++;
		m_maxLightTrace++;
		if(m_maxLights == m_maxLightTrace && m_maxLights != 0)
		   return;//get out
	}
    //fixed pipline zero extra lights
    for(int loop=m_indexTemp;loop<(m_maxLights+1);loop++)
    {
		if(!shaderMode)
		   g_pd3dDevice->LightEnable( loop , FALSE );
		else
		   stateManager->StopSTLight(m_indexTemp,g_pd3dDevice);
	}

	
}

void CRTLightObj::Serialize( CArchive& ar )
{
BOOL reserved = 0;

CObject::Serialize( ar );
	if(ar.IsStoring() ){	

       ar.SetObjectSchema(5); //DPD set to 5
       ASSERT(ar.GetObjectSchema() == 5); //DPD set to 5

	   ar << m_material
		  << m_range
		  << m_attenuation0
		  << m_attenuation1 
		  << m_attenuation2;

	   ar  << m_wldspacePos.x
		   << m_wldspacePos.y
		   << m_wldspacePos.z;

	   //DPD for spotlight
	   ar  << m_type
		   << m_direction.x
		   << m_direction.y
		   << m_direction.z
		   << m_theta
		   << m_phi
		   << m_falloff;

	   //DPD2 Fading
	   ar  << reserved;

	   //DPD4 Blinking
	   ar  << m_blinkOn
		   << m_blinkDur
		   << m_randomOn;

	   //ARB5
	   ar  << m_lightOn;
	}
	else
	{
       int version = ar.GetObjectSchema();

       

	   ZeroMemory(&m_wldspacePos,sizeof(D3DVECTOR));
	   m_lightOn = TRUE;
   
       switch(version)
	   {
	      default:
			  ar  >> m_material
				  >> m_range
				  >> m_attenuation0
				  >> m_attenuation1 
				  >> m_attenuation2;
		  break;
		  case 2:
			  ar  >> m_material
				  >> m_range
				  >> m_attenuation0
				  >> m_attenuation1 
				  >> m_attenuation2;
			  ar  >> m_wldspacePos.x
				  >> m_wldspacePos.y
				  >> m_wldspacePos.z;
		  break;
		  case 3:
			  ar  >> m_material
				  >> m_range
				  >> m_attenuation0
				  >> m_attenuation1 
				  >> m_attenuation2;
			  ar  >> m_wldspacePos.x
				  >> m_wldspacePos.y
				  >> m_wldspacePos.z;
			  ar  >> m_type
				  >> m_direction.x
				  >> m_direction.y
				  >> m_direction.z
				  >> m_theta
				  >> m_phi
				  >> m_falloff;
		  break;
		  case 4:
			  ar  >> m_material
				  >> m_range
				  >> m_attenuation0
				  >> m_attenuation1 
				  >> m_attenuation2;
			  ar  >> m_wldspacePos.x
				  >> m_wldspacePos.y
				  >> m_wldspacePos.z;
			  ar  >> m_type
				  >> m_direction.x
				  >> m_direction.y
				  >> m_direction.z
				  >> m_theta
				  >> m_phi
				  >> m_falloff;
			  ar  >> reserved;
		  break;
		  case 5:
			  ar  >> m_material
				  >> m_range
				  >> m_attenuation0
				  >> m_attenuation1 
				  >> m_attenuation2;
			  ar  >> m_wldspacePos.x
				  >> m_wldspacePos.y
				  >> m_wldspacePos.z;
			  ar  >> m_type
				  >> m_direction.x
				  >> m_direction.y
				  >> m_direction.z
				  >> m_theta
				  >> m_phi
				  >> m_falloff;
			  ar  >> reserved;
			  ar  >> m_blinkOn
				  >> m_blinkDur
			      >> m_randomOn;
		  break;
		  case 6:
			  ar  >> m_material
				  >> m_range
				  >> m_attenuation0
				  >> m_attenuation1 
				  >> m_attenuation2;
			  ar  >> m_wldspacePos.x
				  >> m_wldspacePos.y
				  >> m_wldspacePos.z;
			  ar  >> m_type
				  >> m_direction.x
				  >> m_direction.y
				  >> m_direction.z
				  >> m_theta
				  >> m_phi
				  >> m_falloff;
			  ar  >> reserved;
			  ar  >> m_blinkOn
				  >> m_blinkDur
			      >> m_randomOn;
			  ar  >> m_lightOn;
		  break;
	   }

	}
}     

IMPLEMENT_SERIAL(CRTLightObj, CObject, VERSIONABLE_SCHEMA|6)//DPD set to 5
IMPLEMENT_SERIAL(CRTLightObjList, CObList, 0)

