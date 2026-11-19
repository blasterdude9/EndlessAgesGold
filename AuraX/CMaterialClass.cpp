#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "CMaterialClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CMaterialObject::CMaterialObject()
{
m_forceCameraTransform		= 0;
m_optionalShaderIndex		= 0;
m_mirrorProcessType			= 0;
m_baseMaterial.Diffuse.r	= 1.0f;
m_baseMaterial.Diffuse.g	= 1.0f;
m_baseMaterial.Diffuse.b	= 1.0f;
m_baseMaterial.Diffuse.a	= 1.0f;
m_baseMaterial.Ambient.r	= 1.0f;
m_baseMaterial.Ambient.g	= 1.0f;
m_baseMaterial.Ambient.b	= 1.0f;
m_baseMaterial.Ambient.a	= 1.0f;
m_baseMaterial.Specular.r	= 0.0f;
m_baseMaterial.Specular.g	= 0.0f;
m_baseMaterial.Specular.b	= 0.0f;
m_baseMaterial.Specular.a	= 0.0f;
m_baseMaterial.Emissive.r	= 0.0f;
m_baseMaterial.Emissive.g	= 0.0f;
m_baseMaterial.Emissive.b	= 0.0f;
m_baseMaterial.Emissive.a	= 0.0f;
m_baseMaterial.Power		= 0.0f;
CopyMaterial(m_baseMaterial,&m_useMaterial);
CopyMaterial(m_baseMaterial,&m_targetModMaterial);
m_cycleTime					= 500;//milliseconds
m_switch					= FALSE;
m_timeStamp					= -1;
m_transition				= 0;
m_textureTileType			= 0;
m_cullOn					= TRUE;
m_forceWireFrame			= FALSE;
m_blendMode					= -1;
m_baseBlendMode				= m_blendMode;
m_mirrorEnabled				= FALSE;
m_curDistFromCam			= 0.0f;
m_fadeMaxDist				= 2700;
m_fadeVisDist				= 2400;
m_fadeByDistance			= FALSE;


m_enableTextureMovement		= FALSE;
m_enableSet0				= FALSE;
m_enableSet1				= FALSE;
m_enableSet2				= FALSE;
m_enableSet3				= FALSE;
m_enableSet4				= FALSE;
m_enableSet5				= FALSE;
m_enableSet6				= FALSE;
m_enableSet7				= FALSE;

m_diffuseEffect				= 0;

m_fogFilter					= FALSE;
m_zBias						= 0;
m_fogOverride				= FALSE;

m_transitionBlend			= -2;
for(int loop=0;loop<8;loop++){
m_textureMovement[loop].tu	= 0.0f;          
m_textureMovement[loop].tv	= 0.0f;  
}

m_colorArg1[0]				= 2;
m_colorArg1[1]				= 2;
m_colorArg1[2]				= 2;
m_colorArg1[3]				= 2;
m_colorArg1[4]				= 2;
m_colorArg1[5]				= 2;
m_colorArg1[6]				= 2;
m_colorArg1[7]				= 2;
m_colorArg1[8]				= 2;


m_colorArg2[0]				= 0;
m_colorArg2[1]				= 1;
m_colorArg2[2]				= 1;
m_colorArg2[3]				= 1;
m_colorArg2[4]				= 1;
m_colorArg2[5]				= 1;
m_colorArg2[6]				= 1;
m_colorArg2[7]				= 1;
m_colorArg2[8]				= 1;

ZeroMemory(m_texcoordLevels,10);

m_pixelShader				= -1;
m_vertexShader				= -1;


m_shadowObject				= 0;
m_doAlphaDepthTest			= FALSE;

m_peramBlock				= NULL;
m_peramCount				= 0;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CMaterialObject::~CMaterialObject()
{
      DeleteParamMemoryAndZeroIt();//Param Memory
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CMaterialObject::DeleteParamMemoryAndZeroIt()
{
   if(m_peramBlock)
   {
      for(int loop=0;loop<m_peramCount;loop++)
	  {
          

          if(m_peramBlock[loop].dataSize > 0)
		  if(m_peramBlock[loop].data)
		  {
             delete [] m_peramBlock[loop].data;
			 m_peramBlock[loop].data = 0;
		  }
	  }
	  delete [] m_peramBlock;
	  m_peramBlock = 0;
   }
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
CMaterialObject::CMaterialObject(D3DMATERIAL9 materialInit)
{
m_baseMaterial.Diffuse.r = materialInit.Diffuse.r;
m_baseMaterial.Diffuse.g = materialInit.Diffuse.g;
m_baseMaterial.Diffuse.b = materialInit.Diffuse.b;
m_baseMaterial.Diffuse.a = materialInit.Diffuse.a;
m_baseMaterial.Ambient.r = materialInit.Ambient.r;
m_baseMaterial.Ambient.g = materialInit.Ambient.g;
m_baseMaterial.Ambient.b = materialInit.Ambient.b;
m_baseMaterial.Ambient.a = materialInit.Ambient.a;
m_baseMaterial.Specular.r = materialInit.Specular.r;
m_baseMaterial.Specular.g = materialInit.Specular.g;
m_baseMaterial.Specular.b = materialInit.Specular.b;
m_baseMaterial.Specular.a = materialInit.Specular.a;
m_baseMaterial.Emissive.r = materialInit.Emissive.r;
m_baseMaterial.Emissive.g = materialInit.Emissive.g;
m_baseMaterial.Emissive.b = materialInit.Emissive.b;
m_baseMaterial.Emissive.a = materialInit.Emissive.a;
m_baseMaterial.Power = materialInit.Power;
CopyMaterial(m_baseMaterial,&m_useMaterial);
m_cycleTime = 500;//milliseconds
m_switch = FALSE;
m_timeStamp = -1;
m_transition = 0;
m_peramBlock = NULL;
m_peramCount = 0;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
void CMaterialObject::Clone(CMaterialObject ** clone)
{
CMaterialObject * testDel = *clone;
if(testDel)
   delete testDel;
   testDel = 0;
CMaterialObject * interm = new CMaterialObject(m_baseMaterial);

interm->m_forceCameraTransform	= m_forceCameraTransform;
interm->m_cycleTime				= m_cycleTime;
interm->m_timeStamp				= m_timeStamp;
interm->m_switch				= m_switch;
interm->m_transition			= m_transition;
interm->m_timeStamp				= -1;
interm->m_textureTileType		= m_textureTileType;
interm->m_blendMode				= m_blendMode;
interm->m_baseBlendMode			= m_blendMode;
interm->m_mirrorEnabled			= m_mirrorEnabled;
interm->m_fogFilter				= m_fogFilter;
interm->m_zBias					= m_zBias;
interm->m_fogOverride			= m_fogOverride;
interm->m_diffuseEffect			= m_diffuseEffect;
interm->m_optionalShaderIndex	= m_optionalShaderIndex;
interm->m_shadowObject			= m_shadowObject;

interm->m_enableTextureMovement = m_enableTextureMovement;
interm->m_enableSet0 = m_enableSet0;
interm->m_enableSet1 = m_enableSet1;
interm->m_enableSet2 = m_enableSet2;
interm->m_enableSet3 = m_enableSet3;
interm->m_enableSet4 = m_enableSet4;
interm->m_enableSet5 = m_enableSet5;
interm->m_enableSet6 = m_enableSet6;
interm->m_enableSet7 = m_enableSet7;
interm->m_textureMovement[0].tu = m_textureMovement[0].tu;          
interm->m_textureMovement[0].tv = m_textureMovement[0].tv;  
interm->m_textureMovement[1].tu = m_textureMovement[1].tu;          
interm->m_textureMovement[1].tv = m_textureMovement[1].tv;  
interm->m_textureMovement[2].tu = m_textureMovement[2].tu;          
interm->m_textureMovement[2].tv = m_textureMovement[2].tv;  
interm->m_textureMovement[3].tu = m_textureMovement[3].tu;          
interm->m_textureMovement[3].tv = m_textureMovement[3].tv;  
interm->m_textureMovement[4].tu = m_textureMovement[4].tu;          
interm->m_textureMovement[4].tv = m_textureMovement[4].tv;  
interm->m_textureMovement[5].tu = m_textureMovement[5].tu;          
interm->m_textureMovement[5].tv = m_textureMovement[5].tv;  
interm->m_textureMovement[6].tu = m_textureMovement[6].tu;          
interm->m_textureMovement[6].tv = m_textureMovement[6].tv;  
interm->m_textureMovement[7].tu = m_textureMovement[7].tu;          
interm->m_textureMovement[7].tv = m_textureMovement[7].tv; 
interm->m_forceWireFrame        = m_forceWireFrame; 
interm->m_fadeMaxDist           = m_fadeMaxDist;
interm->m_fadeVisDist           = m_fadeVisDist;
interm->m_fadeByDistance        = m_fadeByDistance;
interm->m_pixelShader           = m_pixelShader;
interm->m_vertexShader          = m_vertexShader;
interm->m_doAlphaDepthTest      = m_doAlphaDepthTest;
interm->m_fxBindString          = m_fxBindString;
interm->m_mirrorProcessType		= m_mirrorProcessType;

CopyMaterial(m_targetModMaterial,&interm->m_targetModMaterial);

CopyMemory(interm->m_colorArg2,m_colorArg2,sizeof(char)*10);
CopyMemory(interm->m_colorArg1,m_colorArg1,sizeof(char)*10);
CopyMemory(interm->m_texcoordLevels,m_texcoordLevels,sizeof(char)*10);


//peram block
interm->m_peramCount            = m_peramCount;
if(m_peramCount > 0)
{//peram block clone
   interm->m_peramBlock = new SLARBperameterBlock[m_peramCount];  

   ZeroMemory(interm->m_peramBlock, sizeof(SLARBperameterBlock)*m_peramCount);
   for(int pmL = 0;pmL < m_peramCount;pmL++)
   {
	   CopyMemory(&interm->m_peramBlock[pmL],&m_peramBlock[pmL],sizeof(SLARBperameterBlock));
	  
	   if(interm->m_peramBlock[pmL].dataSize > 0)
	   if(m_peramBlock[pmL].data)
	   {
          interm->m_peramBlock[pmL].data = new byte[interm->m_peramBlock[pmL].dataSize];
          CopyMemory(interm->m_peramBlock[pmL].data,m_peramBlock[pmL].data,interm->m_peramBlock[pmL].dataSize);
	   }
   }
}//end peram block clone



interm->m_cullOn = m_cullOn;
*clone = interm;
}
//------------------------------------------------------//
//------------------------------------------------------//
//------------------------------------------------------//
int CMaterialObject::GetParameterCount(LPD3DXEFFECT effect)
{
    int trace = 0;
	while(1)
	{
	    D3DXHANDLE floatingHandle = effect->GetParameter(NULL,trace);
		if(floatingHandle)
		{
		   D3DXPARAMETER_DESC localDescStruct;
           if(effect->GetParameterDesc(floatingHandle,&localDescStruct)==S_OK)
		   {
              trace++;
		   }
		}
		else 
		  break;
	}
	return trace;
}
//------------------------------------------------------//
//----inits/reinits IEffect perameterBlock To manage FX Shaders
//------------------------------------------------------//
BOOL CMaterialObject::InitPeramBlockFXShadersHSL(LPD3DXEFFECT effect)
{
	m_peramCount = GetParameterCount(effect);
	
	if(m_peramBlock)
	   DeleteParamMemoryAndZeroIt();

	if(m_peramCount < 1)
	   return FALSE;

	m_peramBlock = new SLARBperameterBlock[m_peramCount];
	ZeroMemory(m_peramBlock,sizeof(SLARBperameterBlock)*m_peramCount);
   
	CString tempString = "";
	int trace = 0;
	while(1)
	{
	    D3DXHANDLE floatingHandle = effect->GetParameter(NULL,trace);
		if(floatingHandle)
		{
		   D3DXPARAMETER_DESC localDescStruct;
           if(effect->GetParameterDesc(floatingHandle,&localDescStruct)==S_OK)
		   {
              //m_peramBlock[trace].type = ARBP_UKNOWN;default set when zeroed
              /*
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
              #define ARBP_DirLight 13*/
			  
              if(localDescStruct.Type == D3DXPT_TEXTURE)
			  {//texture
			     m_peramBlock[trace].type = ARBP_Texture;

				 if(localDescStruct.Annotations > 0)
				 {
				    //D3DXHANDLE hAnnot;
				    D3DXPARAMETER_DESC AnnotDesc;
				    int Width = 0;
					int Height = 0;
					int Depth = 0;
				    LPCSTR textureType,pstrFunction,pstrTarget,pstrName;

					textureType = pstrFunction = pstrTarget = pstrName = NULL;
				    for( UINT iAnnot = 0; iAnnot < localDescStruct.Annotations; iAnnot++ )
                    {
                      D3DXHANDLE hAnnot = effect->GetAnnotation ( floatingHandle, iAnnot );
                      if(effect->GetParameterDesc( hAnnot, &AnnotDesc )==S_OK)
					  {
                       if( _strcmpi( AnnotDesc.Name, "name" ) == 0 )
                           effect->GetString( hAnnot, &pstrName );
                       else if ( _strcmpi( AnnotDesc.Name, "function" ) == 0 )
                           effect->GetString( hAnnot, &pstrFunction );
                       else if ( _strcmpi( AnnotDesc.Name, "target" ) == 0 )
                           effect->GetString( hAnnot, &pstrTarget );
                       else if ( _strcmpi( AnnotDesc.Name, "width" ) == 0 )
                           effect->GetInt( hAnnot, &Width );
                       else if ( _strcmpi( AnnotDesc.Name, "height" ) == 0 )
                           effect->GetInt( hAnnot, &Height );
                       else if ( _strcmpi( AnnotDesc.Name, "depth" ) == 0 )
                           effect->GetInt( hAnnot, &Depth );
                       else if( _strcmpi( AnnotDesc.Name, "type" ) == 0 )
                           effect->GetString( hAnnot, &textureType );
					  }
					  else
					  {//capture error

					  }//end capture error
                    }

					if( Depth > 0 )
					{//3D texture detected
						if(pstrName == NULL && pstrFunction != NULL)
                           m_peramBlock[trace].type = ARBP_VolumeTexturePrcdrl;//dynamic prceedural texture
					}//end 3D texture detected
				 }
			  }//end texture
			  else
			  {
			     tempString = localDescStruct.Name;
				 tempString.MakeUpper();
			     
                 if(tempString == "TIME" || tempString == "TICKS" || tempString == "TIMER")
                    m_peramBlock[trace].type = ARBP_Time;
				 else if(tempString == "WORLDVIEW" || tempString == "WORLDVIEWMATRIX")
                    m_peramBlock[trace].type = ARBP_WorldView;
				 else if(tempString == "WORLDINVERSETRANSPOSE" || tempString == "WORLDIT")
                    m_peramBlock[trace].type = ARBP_WorldInverseTranspose;
				 else if(tempString == "WORLDVIEWPROJECTION" || tempString == "WVP" || tempString == "WORLDVIEWPROJ")
                    m_peramBlock[trace].type = ARBP_WorldViewProjection;
				 else if(tempString == "WORLD" || tempString == "WORLDMATRIX")
                    m_peramBlock[trace].type = ARBP_World;
				 else if(tempString == "VIEWINVTRANS" || tempString == "VIEWINVERSE" || tempString == "VIEWI" || tempString == "VIEWINV" || tempString == "VIEWINVERSEMATRIX")
                    m_peramBlock[trace].type = ARBP_ViewInverse;
				 else if(tempString == "PROJECTION")
                    m_peramBlock[trace].type = ARBP_Projection;
				 else if(tempString == "DIFFUSE")
                    m_peramBlock[trace].type = ARBP_Diffuse;
				 else if(tempString == "SPECULAR" || tempString == "SPECCOLOR")
                    m_peramBlock[trace].type = ARBP_Specular;
				 else if(tempString == "SPECPOWER")
					m_peramBlock[trace].type = ARBP_SpecPower;
				 else if(tempString == "WORLDI")
					m_peramBlock[trace].type = ARBP_WorldInverse;
				 else
				 {//not found
                    m_peramBlock[trace].svData = 0;

				 }//end not found
   

				 m_peramBlock[trace].svData = 0;
                 m_peramBlock[trace].dataSize = localDescStruct.Bytes;
			  }//end shift date for general use
			  m_peramBlock[trace].data = NULL;
              trace++;
		   }
		}
		else 
		  break;
	}

	return TRUE;
}
//------------------------------------------------------//
//----callback of a material
//------------------------------------------------------//
void CMaterialObject::SetFadeInAlpha(long duration)
{
	if(m_transition == 8)
	   return;	
     m_transitionBlend = m_blendMode;
	 m_blendMode = 1;
     m_transition = 8;//fade out
     m_cycleTime = duration;
     m_timeStamp = GetTickCount();
}
//------------------------------------------------------//
//----callback of a material
//------------------------------------------------------//
void CMaterialObject::SetFadeOut(long duration)
{
	if(m_transition == 4)
	   return;
     m_blendMode = 2;
     m_transition = 4;//fade out
     m_cycleTime = duration;
     m_timeStamp = GetTickCount();
}
//------------------------------------------------------//
//----callback of a material
//------------------------------------------------------//
void CMaterialObject::SetFadeIn(long duration,BOOL reset)
{
   if(m_transition == 5)
	   return;
	
     m_blendMode = 2;
     m_transition = 5;
     m_cycleTime = duration;
     m_timeStamp = GetTickCount();

   if(reset)
   {
	m_useMaterial.Emissive.r = 0.0f;
	m_useMaterial.Emissive.g = 0.0f;
	m_useMaterial.Emissive.b = 0.0f;
	m_useMaterial.Diffuse.r = 0.0f;
	m_useMaterial.Diffuse.g = 0.0f;
	m_useMaterial.Diffuse.b = 0.0f;
	m_useMaterial.Ambient.r = 0.0f;
	m_useMaterial.Ambient.g = 0.0f;
	m_useMaterial.Ambient.b = 0.0f;
   }
}
//------------------------------------------------------//
//----callback of a material
//------------------------------------------------------//
void CMaterialObject::SetFadeOutAlpha(long duration)
{
   if(m_transition == 9)
	   return;
	
     m_blendMode = 1;
     m_transition = 9;
     m_cycleTime = duration;
     m_timeStamp = GetTickCount();
}
//------------------------------------------------------//
//----callback of a material
//------------------------------------------------------//
void CMaterialObject::SetOverrideTrans(float percentage)
{	
	 m_transition = 0;
     m_blendMode = 2;//use base blend mode to undo override
	 m_useMaterial.Emissive.r = m_baseMaterial.Emissive.r*percentage;
	 m_useMaterial.Emissive.g = m_baseMaterial.Emissive.g*percentage;
	 m_useMaterial.Emissive.b = m_baseMaterial.Emissive.b*percentage;
	 m_useMaterial.Emissive.a = m_baseMaterial.Emissive.a*percentage;
	 m_useMaterial.Diffuse.r = m_baseMaterial.Diffuse.r*percentage;
	 m_useMaterial.Diffuse.g = m_baseMaterial.Diffuse.g*percentage;
	 m_useMaterial.Diffuse.b = m_baseMaterial.Diffuse.b*percentage;
	 m_useMaterial.Diffuse.a = m_baseMaterial.Diffuse.a*percentage;
	 m_useMaterial.Ambient.r = m_baseMaterial.Ambient.r*percentage;
	 m_useMaterial.Ambient.g = m_baseMaterial.Ambient.g*percentage;
	 m_useMaterial.Ambient.b = m_baseMaterial.Ambient.b*percentage;
	 m_useMaterial.Ambient.a = m_baseMaterial.Ambient.a*percentage;
	 //m_blendMode = 1;//use base blend mode to undo override
	 //m_useMaterial.Emissive.a = m_baseMaterial.Emissive.a*percentage;
	 //m_useMaterial.Diffuse.a = m_baseMaterial.Diffuse.a*percentage;
	 //m_useMaterial.Ambient.a = m_baseMaterial.Ambient.a*percentage;
}
//------------------------------------------------------//
//----callback of a material
//------------------------------------------------------//
void CMaterialObject::Callback()
{
 if(m_fadeByDistance){//fade by distance
	m_blendMode = 1;
    m_useMaterial.Diffuse.a = m_baseMaterial.Diffuse.a;
	  if(m_fadeMaxDist >= 0)
	  {//normal distance fade
	    if(m_curDistFromCam >= m_fadeMaxDist)
	       m_useMaterial.Diffuse.a = 0.0f;
	    else if(m_curDistFromCam <= m_fadeVisDist){
           m_useMaterial.Diffuse.a = m_baseMaterial.Diffuse.a;
	       m_blendMode = m_baseBlendMode;
	    }
	    else{
	       long range = m_fadeMaxDist - m_fadeVisDist;
           m_useMaterial.Diffuse.a =  InterpolateFloats(m_baseMaterial.Diffuse.a,0.0f,range,(int)(m_curDistFromCam-(float)m_fadeVisDist));
		}
	  }//end normal distance fade
	  else{//inverse fade
        if(m_curDistFromCam >= abs(m_fadeMaxDist))
	       m_useMaterial.Diffuse.a = m_baseMaterial.Diffuse.a;
	    else if(m_curDistFromCam <= abs(m_fadeVisDist)){
           m_useMaterial.Diffuse.a = 0.0f;
	       m_blendMode = m_baseBlendMode;
	    }
	    else{
	       long range = abs(m_fadeMaxDist) - abs(m_fadeVisDist);
            m_useMaterial.Diffuse.a =  InterpolateFloats(0.0f,m_baseMaterial.Diffuse.a,range,abs((long)m_curDistFromCam-abs(m_fadeVisDist)) );
		}
	  }//end inverse fade
 }//end fade by distance

if(!m_transition)//material reconfig
    return;

if(m_timeStamp == -1)
   m_timeStamp = GetTickCount();

if(m_transition == 1){//occilate emmissive
    int passedTime = GetTickCount() - m_timeStamp;
	if(passedTime > m_cycleTime){
       if(m_switch==TRUE)
          m_switch = FALSE;
	   else
		  m_switch = TRUE;
	   m_timeStamp = GetTickCount();
	   passedTime=0;
	}	
	if(m_switch == TRUE){
	m_useMaterial.Emissive.r = InterpolateFloats(0.0f,1.0f,m_cycleTime,passedTime);
	m_useMaterial.Emissive.g = InterpolateFloats(0.0f,1.0f,m_cycleTime,passedTime);
	m_useMaterial.Emissive.b = InterpolateFloats(0.0f,1.0f,m_cycleTime,passedTime);
	}
    else{
    m_useMaterial.Emissive.r = InterpolateFloats(1.0f,0.0f,m_cycleTime,passedTime);
	m_useMaterial.Emissive.g = InterpolateFloats(1.0f,0.0f,m_cycleTime,passedTime);
	m_useMaterial.Emissive.b = InterpolateFloats(1.0f,0.0f,m_cycleTime,passedTime);
	}
return;
}//end occilate emmissive

if(m_transition == 2){//occilate all color
    int passedTime = GetTickCount() - m_timeStamp;
	if(passedTime > m_cycleTime){
       if(m_switch==TRUE)
          m_switch = FALSE;
	   else
		  m_switch = TRUE;
	   m_timeStamp = GetTickCount();
	   passedTime=0;
	}	
	if(m_switch == TRUE){
	m_useMaterial.Emissive.r = InterpolateFloats(0.0f,1.0f,m_cycleTime,passedTime);
	m_useMaterial.Emissive.g = InterpolateFloats(0.0f,1.0f,m_cycleTime,passedTime);
	m_useMaterial.Emissive.b = InterpolateFloats(0.0f,1.0f,m_cycleTime,passedTime);
	m_useMaterial.Diffuse.r = InterpolateFloats(0.0f,1.0f,m_cycleTime,passedTime);
	m_useMaterial.Diffuse.g = InterpolateFloats(0.0f,1.0f,m_cycleTime,passedTime);
	m_useMaterial.Diffuse.b = InterpolateFloats(0.0f,1.0f,m_cycleTime,passedTime);
	m_useMaterial.Ambient.r = InterpolateFloats(0.0f,1.0f,m_cycleTime,passedTime);
	m_useMaterial.Ambient.g = InterpolateFloats(0.0f,1.0f,m_cycleTime,passedTime);
	m_useMaterial.Ambient.b = InterpolateFloats(0.0f,1.0f,m_cycleTime,passedTime);
	}
    else{
    m_useMaterial.Emissive.r = InterpolateFloats(1.0f,0.0f,m_cycleTime,passedTime);
	m_useMaterial.Emissive.g = InterpolateFloats(1.0f,0.0f,m_cycleTime,passedTime);
	m_useMaterial.Emissive.b = InterpolateFloats(1.0f,0.0f,m_cycleTime,passedTime);
	m_useMaterial.Diffuse.r = InterpolateFloats(1.0f,0.0f,m_cycleTime,passedTime);
	m_useMaterial.Diffuse.g = InterpolateFloats(1.0f,0.0f,m_cycleTime,passedTime);
	m_useMaterial.Diffuse.b = InterpolateFloats(1.0f,0.0f,m_cycleTime,passedTime);
	m_useMaterial.Ambient.r = InterpolateFloats(1.0f,0.0f,m_cycleTime,passedTime);
	m_useMaterial.Ambient.g = InterpolateFloats(1.0f,0.0f,m_cycleTime,passedTime);
	m_useMaterial.Ambient.b = InterpolateFloats(1.0f,0.0f,m_cycleTime,passedTime);
	}
return;
}//end occilate all color

if(m_transition == 3){//custom	
    int passedTime = GetTickCount() - m_timeStamp;
	if(passedTime > m_cycleTime){
       if(m_switch==TRUE)
          m_switch = FALSE;
	   else
		  m_switch = TRUE;
	   m_timeStamp = GetTickCount();
	   passedTime=0;
	}	
	if(m_switch == TRUE){
	m_useMaterial.Emissive.r = InterpolateFloats(m_baseMaterial.Emissive.r,m_targetModMaterial.Emissive.r,m_cycleTime,passedTime);
	m_useMaterial.Emissive.g = InterpolateFloats(m_baseMaterial.Emissive.g,m_targetModMaterial.Emissive.g,m_cycleTime,passedTime);
	m_useMaterial.Emissive.b = InterpolateFloats(m_baseMaterial.Emissive.b,m_targetModMaterial.Emissive.b,m_cycleTime,passedTime);
	m_useMaterial.Diffuse.r =  InterpolateFloats(m_baseMaterial.Diffuse.r,m_targetModMaterial.Diffuse.r,m_cycleTime,passedTime);
	m_useMaterial.Diffuse.g =  InterpolateFloats(m_baseMaterial.Diffuse.g,m_targetModMaterial.Diffuse.g,m_cycleTime,passedTime);
	m_useMaterial.Diffuse.b =  InterpolateFloats(m_baseMaterial.Diffuse.b,m_targetModMaterial.Diffuse.b,m_cycleTime,passedTime);
	m_useMaterial.Ambient.r =  InterpolateFloats(m_baseMaterial.Ambient.r,m_targetModMaterial.Ambient.r,m_cycleTime,passedTime);
	m_useMaterial.Ambient.g =  InterpolateFloats(m_baseMaterial.Ambient.g,m_targetModMaterial.Ambient.g,m_cycleTime,passedTime);
	m_useMaterial.Ambient.b =  InterpolateFloats(m_baseMaterial.Ambient.b,m_targetModMaterial.Ambient.b,m_cycleTime,passedTime);
	}
    else{
    m_useMaterial.Emissive.r = InterpolateFloats(m_targetModMaterial.Emissive.r,m_baseMaterial.Emissive.r,m_cycleTime,passedTime);
	m_useMaterial.Emissive.g = InterpolateFloats(m_targetModMaterial.Emissive.g,m_baseMaterial.Emissive.g,m_cycleTime,passedTime);
	m_useMaterial.Emissive.b = InterpolateFloats(m_targetModMaterial.Emissive.b,m_baseMaterial.Emissive.b,m_cycleTime,passedTime);
	m_useMaterial.Diffuse.r =  InterpolateFloats(m_targetModMaterial.Diffuse.r,m_baseMaterial.Diffuse.r,m_cycleTime,passedTime);
	m_useMaterial.Diffuse.g =  InterpolateFloats(m_targetModMaterial.Diffuse.g,m_baseMaterial.Diffuse.g,m_cycleTime,passedTime);
	m_useMaterial.Diffuse.b =  InterpolateFloats(m_targetModMaterial.Diffuse.b,m_baseMaterial.Diffuse.b,m_cycleTime,passedTime);
	m_useMaterial.Ambient.r =  InterpolateFloats(m_targetModMaterial.Ambient.r,m_baseMaterial.Ambient.r,m_cycleTime,passedTime);
	m_useMaterial.Ambient.g =  InterpolateFloats(m_targetModMaterial.Ambient.g,m_baseMaterial.Ambient.g,m_cycleTime,passedTime);
	m_useMaterial.Ambient.b =  InterpolateFloats(m_targetModMaterial.Ambient.b,m_baseMaterial.Ambient.b,m_cycleTime,passedTime);
	}

return;
}//end custom

if(m_transition == 4){//fade out
	
    int passedTime = GetTickCount() - m_timeStamp;
	if(passedTime > m_cycleTime)
	   passedTime = m_cycleTime;
	m_useMaterial.Emissive.r = InterpolateFloats(m_baseMaterial.Emissive.r,0.0f,m_cycleTime,passedTime);
	m_useMaterial.Emissive.g = InterpolateFloats(m_baseMaterial.Emissive.g,0.0f,m_cycleTime,passedTime);
	m_useMaterial.Emissive.b = InterpolateFloats(m_baseMaterial.Emissive.b,0.0f,m_cycleTime,passedTime);
	m_useMaterial.Diffuse.r =  InterpolateFloats(m_baseMaterial.Diffuse.r,0.0f,m_cycleTime,passedTime);
	m_useMaterial.Diffuse.g =  InterpolateFloats(m_baseMaterial.Diffuse.g,0.0f,m_cycleTime,passedTime);
	m_useMaterial.Diffuse.b =  InterpolateFloats(m_baseMaterial.Diffuse.b,0.0f,m_cycleTime,passedTime);
	m_useMaterial.Diffuse.a =  InterpolateFloats(m_baseMaterial.Diffuse.a,0.0f,m_cycleTime,passedTime);
	m_useMaterial.Ambient.r =  InterpolateFloats(m_baseMaterial.Ambient.r,0.0f,m_cycleTime,passedTime);
	m_useMaterial.Ambient.g =  InterpolateFloats(m_baseMaterial.Ambient.g,0.0f,m_cycleTime,passedTime);
	m_useMaterial.Ambient.b =  InterpolateFloats(m_baseMaterial.Ambient.b,0.0f,m_cycleTime,passedTime);    
return;
}//end fade out

if(m_transition == 5){//fade in
    int passedTime = GetTickCount() - m_timeStamp;
	if(passedTime > m_cycleTime)
	   passedTime = m_cycleTime;
	m_useMaterial.Emissive.r = InterpolateFloats(m_useMaterial.Emissive.r,m_baseMaterial.Emissive.r,m_cycleTime,passedTime);
	m_useMaterial.Emissive.g = InterpolateFloats(m_useMaterial.Emissive.g,m_baseMaterial.Emissive.g,m_cycleTime,passedTime);
	m_useMaterial.Emissive.b = InterpolateFloats(m_useMaterial.Emissive.b,m_baseMaterial.Emissive.b,m_cycleTime,passedTime);
	m_useMaterial.Diffuse.r =  InterpolateFloats(m_useMaterial.Diffuse.r,m_baseMaterial.Diffuse.r,m_cycleTime,passedTime);
	m_useMaterial.Diffuse.g =  InterpolateFloats(m_useMaterial.Diffuse.g,m_baseMaterial.Diffuse.g,m_cycleTime,passedTime);
	m_useMaterial.Diffuse.b =  InterpolateFloats(m_useMaterial.Diffuse.b,m_baseMaterial.Diffuse.b,m_cycleTime,passedTime);
	m_useMaterial.Ambient.r =  InterpolateFloats(m_useMaterial.Ambient.r,m_baseMaterial.Ambient.r,m_cycleTime,passedTime);
	m_useMaterial.Ambient.g =  InterpolateFloats(m_useMaterial.Ambient.g,m_baseMaterial.Ambient.g,m_cycleTime,passedTime);
	m_useMaterial.Ambient.b =  InterpolateFloats(m_useMaterial.Ambient.b,m_baseMaterial.Ambient.b,m_cycleTime,passedTime);    
    if(passedTime >= m_cycleTime){ 
	m_blendMode = -1;
    m_transition = 0;
	Reset();
	}
	return;
}//end fade in

if(m_transition == 8){//fade alpha in
    int passedTime = GetTickCount() - m_timeStamp;
	if(passedTime > m_cycleTime)
	   passedTime = m_cycleTime;
	
	m_useMaterial.Emissive.a = 0.0f;
	m_useMaterial.Diffuse.a =  InterpolateFloats(0.0f,1.0f,m_cycleTime,passedTime);
	m_useMaterial.Ambient.a =  0.0f; 
    if(passedTime >= m_cycleTime){ 
	m_blendMode = m_transitionBlend;
    m_transition = 0;
	Reset();
	}
	return;
}//end fade alpha in

if(m_transition == 9){//fade alpha out
    int passedTime = GetTickCount() - m_timeStamp;
	if(passedTime > m_cycleTime)
	   passedTime = m_cycleTime;
	
	m_useMaterial.Emissive.a = 1.0f;
	m_useMaterial.Diffuse.a =  InterpolateFloats(1.0f,0.0f,m_cycleTime,passedTime);
	m_useMaterial.Ambient.a =  1.0f; 
    if(passedTime >= m_cycleTime){ 
	m_blendMode = m_transitionBlend;
    m_transition = 0;
	Reset();
	}
	return;
}//end fade alpha in


}
//------------------------------------------------------//
//----callback of a material
//------------------------------------------------------//
void CMaterialObject::SetCallbackMethod(int method)
{
	if(m_transition != method){
       m_transition = 3;
       m_timeStamp = GetTickCount();
	}
}
//------------------------------------------------------//
//----copy a material
//------------------------------------------------------//
void CMaterialObject::CopyMaterial(D3DMATERIAL9 source,D3DMATERIAL9 * dest)
{
dest->Diffuse.r    = source.Diffuse.r;
dest->Diffuse.g    = source.Diffuse.g;
dest->Diffuse.b    = source.Diffuse.b;
dest->Diffuse.a    = source.Diffuse.a;
dest->Ambient.r    = source.Ambient.r;
dest->Ambient.g    = source.Ambient.g;
dest->Ambient.b    = source.Ambient.b;
dest->Ambient.a    = source.Ambient.a;
dest->Specular.r   = source.Specular.r;
dest->Specular.g   = source.Specular.g;
dest->Specular.b   = source.Specular.b;
dest->Specular.a   = source.Specular.a;
dest->Emissive.r   = source.Emissive.r;
dest->Emissive.g   = source.Emissive.g;
dest->Emissive.b   = source.Emissive.b;
dest->Emissive.a   = source.Emissive.a;
dest->Power         = source.Power;
}
//------------------------------------------------------//
//----copy a material
//------------------------------------------------------//
void CMaterialObject::Reset()
{
CopyMaterial(m_baseMaterial,&m_useMaterial);
m_blendMode = m_baseBlendMode;
m_timeStamp = GetTickCount();
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------Interpolation between animations BABY!! Imediate mode style-----------//
float CMaterialObject::InterpolateFloats(
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



void CMaterialObject::Serialize( CArchive& ar )
{
float reservedFloat = 0.0f;
int   resevedInt = 0;
BOOL  reservedBool = FALSE;



CObject::Serialize( ar );
	if(ar.IsStoring() ){

    ar.SetObjectSchema(5);
    ASSERT(ar.GetObjectSchema() == 5);

	ar << m_baseMaterial.Diffuse.r
       << m_baseMaterial.Diffuse.g
       << m_baseMaterial.Diffuse.b
       << m_baseMaterial.Diffuse.a
       << m_baseMaterial.Ambient.r
       << m_baseMaterial.Ambient.g
       << m_baseMaterial.Ambient.b
       << m_baseMaterial.Ambient.a
       << m_baseMaterial.Specular.r
       << m_baseMaterial.Specular.g
       << m_baseMaterial.Specular.b
       << m_baseMaterial.Specular.a
       << m_baseMaterial.Emissive.r
       << m_baseMaterial.Emissive.g
       << m_baseMaterial.Emissive.b
       << m_baseMaterial.Emissive.a
       << m_baseMaterial.Power
	   << m_transition
	   << m_cycleTime
	   << m_textureTileType
	   << m_cullOn
	   << m_forceWireFrame
	   << m_mirrorEnabled
	   << m_fogOverride
	   << m_enableTextureMovement
	   << m_enableSet0
	   << m_enableSet1
	   << m_enableSet2
	   << m_enableSet3
	   << m_enableSet4
	   << m_enableSet5
	   << m_optionalShaderIndex
	   << m_diffuseEffect
	   << m_targetModMaterial.Diffuse.r
       << m_targetModMaterial.Diffuse.g
       << m_targetModMaterial.Diffuse.b
       << m_targetModMaterial.Diffuse.a
       << m_targetModMaterial.Ambient.r
       << m_targetModMaterial.Ambient.g
       << m_targetModMaterial.Ambient.b
       << m_targetModMaterial.Ambient.a
       << m_targetModMaterial.Specular.r
       << m_targetModMaterial.Specular.g
       << m_targetModMaterial.Specular.b
       << m_targetModMaterial.Specular.a
       << m_targetModMaterial.Emissive.r
       << m_targetModMaterial.Emissive.g
       << m_targetModMaterial.Emissive.b
       << m_targetModMaterial.Emissive.a
	   << m_blendMode
       << m_fadeMaxDist
       << m_fadeVisDist
       << m_fadeByDistance
	   << m_fogFilter
	   << m_zBias;

	   int loop=0;
       for(loop=0;loop<8;loop++){
           ar << m_textureMovement[loop].tu   
              << m_textureMovement[loop].tv;  
	   }  


	   for(loop=0;loop<10;loop++){
		   ar << m_colorArg1[loop];
           ar << m_colorArg2[loop];
	   }  

	   ar << m_doAlphaDepthTest
	      << m_peramCount
		  << m_mirrorProcessType
		  << m_forceCameraTransform
		  << m_pixelShader
		  << m_vertexShader;

       //perameter extention
	   if(m_peramBlock)
       {
          for(int loop=0;loop<m_peramCount;loop++)
	      {
              ar << m_peramBlock[loop].type
				 << m_peramBlock[loop].dataSize
				 << m_peramBlock[loop].svData;
			  
			  if(m_peramBlock[loop].svData == 1)
			  if(m_peramBlock[loop].dataSize > 0)
		      if(m_peramBlock[loop].data)
		      {
				 for(int bL=0;bL<m_peramBlock[loop].dataSize;bL++)
                     ar << m_peramBlock[loop].data[bL];
		      }
	      }
       }
	   //+VS3
	   ar << m_fxBindString;

	   for(loop=0;loop<10;loop++)
		   ar << m_texcoordLevels[loop];
	   
	   


	}
	else
	{
    int version = ar.GetObjectSchema();
 

    ar >> m_baseMaterial.Diffuse.r
       >> m_baseMaterial.Diffuse.g
       >> m_baseMaterial.Diffuse.b
       >> m_baseMaterial.Diffuse.a
       >> m_baseMaterial.Ambient.r
       >> m_baseMaterial.Ambient.g
       >> m_baseMaterial.Ambient.b
       >> m_baseMaterial.Ambient.a
       >> m_baseMaterial.Specular.r
       >> m_baseMaterial.Specular.g
       >> m_baseMaterial.Specular.b
       >> m_baseMaterial.Specular.a
       >> m_baseMaterial.Emissive.r
       >> m_baseMaterial.Emissive.g
       >> m_baseMaterial.Emissive.b
       >> m_baseMaterial.Emissive.a
       >> m_baseMaterial.Power
	   >> m_transition
	   >> m_cycleTime
	   >> m_textureTileType
	   >> m_cullOn
	   >> m_forceWireFrame
	   >> m_mirrorEnabled
	   >> m_fogOverride
	   >> m_enableTextureMovement
	   >> m_enableSet0
	   >> m_enableSet1
	   >> m_enableSet2
	   >> m_enableSet3
	   >> m_enableSet4
	   >> m_enableSet5
	   >> m_optionalShaderIndex
	   >> m_diffuseEffect
	   >> m_targetModMaterial.Diffuse.r
       >> m_targetModMaterial.Diffuse.g
       >> m_targetModMaterial.Diffuse.b
       >> m_targetModMaterial.Diffuse.a
       >> m_targetModMaterial.Ambient.r
       >> m_targetModMaterial.Ambient.g
       >> m_targetModMaterial.Ambient.b
       >> m_targetModMaterial.Ambient.a
       >> m_targetModMaterial.Specular.r
       >> m_targetModMaterial.Specular.g
       >> m_targetModMaterial.Specular.b
       >> m_targetModMaterial.Specular.a
       >> m_targetModMaterial.Emissive.r
       >> m_targetModMaterial.Emissive.g
       >> m_targetModMaterial.Emissive.b
       >> m_targetModMaterial.Emissive.a
	   >> m_blendMode
	   >> m_fadeMaxDist
       >> m_fadeVisDist
       >> m_fadeByDistance
	   >> m_fogFilter
	   >> m_zBias;

	   int loop=0;
       for(loop=0;loop<8;loop++){
           ar >> m_textureMovement[loop].tu        
              >> m_textureMovement[loop].tv;  
	   } 

	   m_colorArg1[0] = 2;
       m_colorArg1[1] = 2;
       m_colorArg1[2] = 2;
       m_colorArg1[3] = 2;
       m_colorArg1[4] = 2;
       m_colorArg1[5] = 2;
       m_colorArg1[6] = 2;
       m_colorArg1[7] = 2;
       m_colorArg1[8] = 2;
	   m_colorArg1[9] = 2;

	   m_colorArg2[0] = 0;
       m_colorArg2[1] = 1;
       m_colorArg2[2] = 1;
       m_colorArg2[3] = 1;
       m_colorArg2[4] = 1;
       m_colorArg2[5] = 1;
       m_colorArg2[6] = 1;
       m_colorArg2[7] = 1;
       m_colorArg2[8] = 1;
	   m_colorArg2[9] = 1;

	   m_pixelShader = -1;
	   m_vertexShader = -1;
	   m_peramCount = 0;
	   m_peramBlock = NULL;

       if(version > 1)
	   {
          for(loop=0;loop<10;loop++){
			  ar >> m_colorArg1[loop];
              ar >> m_colorArg2[loop];
	      }  
	   }

	   if(version > 2)
	   {
          ar >> m_doAlphaDepthTest
	         >> m_peramCount
		     >> m_mirrorProcessType
		     >> m_forceCameraTransform
		     >> m_pixelShader
			 >> m_vertexShader; 

          
	   }
	   if(version > 3)
	   {
          if(m_peramCount > 0)
		  {
             m_peramBlock = new SLARBperameterBlock[m_peramCount];
			 ZeroMemory(m_peramBlock,sizeof(SLARBperameterBlock)*m_peramCount);
 
			 for(int loop=0;loop<m_peramCount;loop++)
	         {
                 ar >> m_peramBlock[loop].type
				    >> m_peramBlock[loop].dataSize
					>> m_peramBlock[loop].svData;
			  
				 if(m_peramBlock[loop].svData == 1)
			     if(m_peramBlock[loop].dataSize > 0)
		         {
				    m_peramBlock[loop].data = new byte[m_peramBlock[loop].dataSize];
				    for(int bL=0;bL<m_peramBlock[loop].dataSize;bL++)
                        ar >> m_peramBlock[loop].data[bL];
		         }
	         }
		  }


          ar >> m_fxBindString;
	   }

	   if(version > 4)
	   {
          for(loop=0;loop<10;loop++)
			  ar >> m_texcoordLevels[loop];
	   }

	   
	   CopyMaterial(m_baseMaterial,&m_useMaterial);
	   m_switch = FALSE;
	   m_timeStamp = -1;
	   m_transitionBlend = -2;
	   m_baseBlendMode   = m_blendMode;

	   m_curDistFromCam = 0.0f;
	   m_enableSet6 = FALSE;
	   m_enableSet7 = FALSE;

	   m_shadowObject = 0;
     
	}
}


IMPLEMENT_SERIAL(CMaterialObject, CObject, VERSIONABLE_SCHEMA|5)


