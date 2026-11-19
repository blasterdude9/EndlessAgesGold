//#include "stdafx.h"
#include "afxwin.h"
#include "afxext.h"
#define D3D_OVERLOADS
#include "resource.h"
#include <d3d9.h>
//#include <afxtempl.h>
#include <direct.h>
#include <math.h>
#include "CStateManagementClass.h"



CStateManagementObj::CStateManagementObj()
{
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
	m_pVertexDeclaration       = NULL;
	m_currentVertexType        = -1;
	m_dotProductEnabled        = FALSE;
	m_shaderMode               = 0;

	m_locationOffset.x         = 0.0f;
	m_locationOffset.y         = 0.0f;
	m_locationOffset.z         = 0.0f;

	m_specularEnable           = FALSE;
	m_shderSystemRef           = NULL;//reference
	m_currentShader            = -99;
    m_hightColorRangeStart     = 0;
    m_hightColorRangeEnd       = 200;
	m_currentDiffuseSource     = 0;
	m_supportBUMPENVMAP        = FALSE;
	

	for(int loop=0;loop<10;loop++){
        GlobalTextureIndex[loop] = -1;
		GlobalBlendLevels[loop] = -1;
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

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------------sets a texture---------------------------------------------------//
void CStateManagementObj::SafeDelete()
{
    if(m_pVertexDeclaration)
	{
       m_pVertexDeclaration->Release();
	   m_pVertexDeclaration=0;
	}
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//-------------------sets a texture---------------------------------------------------//
void CStateManagementObj::SetTextureState(int textureIndex,
										         int texturePass,
										         LPDIRECT3DDEVICE9 g_pd3dDevice,
										         CTextureDBObjList * textureDataBase)
{
      if(GlobalTextureIndex[texturePass] == textureIndex)
         return;

          if(textureIndex == -1){
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
 void CStateManagementObj::SetBlendLevel(int level,
									           int type,
									           LPDIRECT3DDEVICE9 g_pd3dDevice)
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
  //   return;
     int tempType = uvCount;

	 if(m_currentDiffuseSource == 1)
	    tempType = tempType + 9;

  if(m_currentVertexType == tempType)
     return;

  m_currentVertexType = tempType;

  switch(m_currentVertexType){
    case 0:
      g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX0) );
	  break;
	case 1:
      g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1) );
	  break;
	case 2:
      g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX2) );
	  break;
	case 3:
      g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX3) );
	  break;
	case 4:
      g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX4) );
	  break;
	case 5:
      g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX5) );
	  break;
    case 6:
      g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX6) );
	  break;
    case 7:
      g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX7) );
	  break;
    case 8:
      g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX8) );
	  break;
    case 9:
      g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX0) );
	  break;
    case 10:
      g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1) );
	  break;
    case 11:
      g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX2) );
	  break;
    case 12:
      g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX3) );
	  break;
    case 13:
      g_pd3dDevice->SetFVF( (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX4) );
	  break;

      
  }
  return;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 void CStateManagementObj::SetBlendLevelMultipass(int type,LPDIRECT3DDEVICE9 g_pd3dDevice)
{
  switch(type){
	case -1:
	  g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,FALSE);
	  m_globalBlend = -1;
	  break;
    case 0:
      g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,   TRUE );
      g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND ,           D3DBLEND_DESTCOLOR);
      g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,           D3DBLEND_ZERO); 
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
                m_globalBlend = type;
               break;
               case 0 :
                g_pd3dDevice->SetRenderState(D3DRS_ALPHABLENDENABLE,    TRUE );
                g_pd3dDevice->SetRenderState(D3DRS_SRCBLEND ,           D3DBLEND_DESTCOLOR);
                g_pd3dDevice->SetRenderState(D3DRS_DESTBLEND,           D3DBLEND_ONE); 
                m_globalBlend = type;
               break;
               case 1 :
                g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,   TRUE );
                g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND, D3DBLEND_SRCALPHA );
                g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );
                m_globalBlend = type;
               break;
			   case 2 :
                g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
                g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_ONE );
                g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_ONE );

				
                m_globalBlend = type;
               break;
			   case 3 :
                g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
                g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,    D3DBLEND_SRCCOLOR );
                g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,   D3DBLEND_DESTCOLOR  );
                m_globalBlend = type;
               break;
			   case 4 ://inversion amplification
                g_pd3dDevice->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
                g_pd3dDevice->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_ZERO );
                g_pd3dDevice->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCCOLOR );

                /*g_pd3dDevice->SetRenderState( D3DRENDERSTATE_ALPHABLENDENABLE, TRUE );
                g_pd3dDevice->SetRenderState( D3DRENDERSTATE_SRCBLEND,    D3DBLEND_DESTCOLOR  );
                g_pd3dDevice->SetRenderState( D3DRENDERSTATE_DESTBLEND,   D3DBLEND_ONE   );*/
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
int CStateManagementObj::TextureReference(CString textureFileName,
										  CTextureDBObjList * textureDataBase,
										  LPDIRECT3DDEVICE9 g_pd3dDevice,
										  BOOL bitmapLoadOvverride,
										  int & maxTextureSize)
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
											 g_pd3dDevice)==TRUE){//success

		    if(newPtr->m_textureObject->Restore(g_pd3dDevice,
												newPtr->List_BltOpOn,
												newPtr->m_bump,
												newPtr->m_bumpType,
												maxTextureSize,
												m_pCaps)==S_OK){//2
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
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
BOOL CStateManagementObj::SetCullState(BOOL on,
									   LPDIRECT3DDEVICE9 g_pd3dDevice)
{
if(on == GL_cullOn)
return FALSE;

if(on == TRUE)
g_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_CCW);
if(on == FALSE)
g_pd3dDevice->SetRenderState(D3DRS_CULLMODE,D3DCULL_NONE);

GL_cullOn = on;
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
	D3DXVECTOR4 maxPower( 128.0f, 128.0f, 128.0f, 128.0f );
	g_pd3dDevice->SetVertexShaderConstantF( VAL_ONE, (float*)&vOne, 1 );
	g_pd3dDevice->SetVertexShaderConstantF( VAL_ZERO, (float*)&vZero, 1 );
	g_pd3dDevice->SetVertexShaderConstantF( MAX_POWER, (float*)&maxPower, 1 );

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
// Name: VectortoRGBA()
// Desc: Turns a normalized vector into RGBA form. Used to encode vectors into
//       a height map. 
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
 void CStateManagementObj::SetViewProjMatrix(D3DXMATRIX *pView, D3DXMATRIX *pProj, LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	g_pd3dDevice->SetTransform( D3DTS_PROJECTION, pProj );
	 g_pd3dDevice->SetTransform( D3DTS_VIEW , pView );
     m_viewMatrix = *pView;
	 m_projMatrix = *pProj;

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 void CStateManagementObj::SetViewMatrix(D3DXMATRIX *pView, LPDIRECT3DDEVICE9 g_pd3dDevice)
{
	 g_pd3dDevice->SetTransform( D3DTS_VIEW , pView );
     m_viewMatrix = *pView;

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 void CStateManagementObj::SetMaterial(LPDIRECT3DDEVICE9 g_pd3dDevice, D3DMATERIAL9 * material)
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

	g_pd3dDevice->SetVertexShaderConstantF( DIR_LIGHTVECT, (float*)&dirVect, 1 );
	g_pd3dDevice->SetVertexShaderConstantF( DIR_LIGHTCOLOR, (float*)&dirColor, 1 );
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 void CStateManagementObj::SetSTLight(int index,LPDIRECT3DDEVICE9 g_pd3dDevice, D3DVECTOR position, float r, float g, float b, float rg)
{
    D3DXVECTOR4 lightDir(position.x,position.y,position.z,rg);
	D3DXVECTOR4 LightColor( r, g, b, 0.0f );

	     switch( index ) 
		 {
               case 1:
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT1, (float*)&lightDir, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT1_COLOR, (float*)&LightColor, 1 );
               break;
               case 2 :
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT2, (float*)&lightDir, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT2_COLOR, (float*)&LightColor, 1 );
               break;
               case 3 :
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT3, (float*)&lightDir, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT3_COLOR, (float*)&LightColor, 1 );
               break;
			   case 4 :
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT4, (float*)&lightDir, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT4_COLOR, (float*)&LightColor, 1 );
               break;
			   case 5 :
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT5, (float*)&lightDir, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT5_COLOR, (float*)&LightColor, 1 );
               break;
			   case 6 ://inversion amplification
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT6, (float*)&lightDir, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT6_COLOR, (float*)&LightColor, 1 );
               break;
			   case 7 :
                g_pd3dDevice->SetVertexShaderConstantF( POINT_LIGHT7, (float*)&lightDir, 1 );
	            g_pd3dDevice->SetVertexShaderConstantF( POINTLIGHT7_COLOR, (float*)&LightColor, 1 );
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
       if(m_currentShader == ShaderIndexID)
	      return;
	   
	   m_currentShader = ShaderIndexID;
 
	   if(shaderSystem)
          m_shderSystemRef = shaderSystem;
	   //if(m_shderSystemRef)
	   //   m_shderSystemRef->SetShaderByIndex(g_pd3dDevice,ShaderIndexID);//-1 fixed pipeline
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 void CStateManagementObj::SetShaderOpt(LPDIRECT3DDEVICE9 g_pd3dDevice,int ShaderIndexID)
{
       if(m_currentShader == ShaderIndexID)
	      return;
	   
	   m_currentShader = ShaderIndexID;

       //if(m_shderSystemRef)
	   //   m_shderSystemRef->SetShaderByIndex(g_pd3dDevice,ShaderIndexID);//-1 fixed pipeline
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
 void CStateManagementObj::SetWorldMatrix(D3DXMATRIX *pObject, LPDIRECT3DDEVICE9 g_pd3dDevice)
{
     g_pd3dDevice->SetTransform( D3DTS_WORLD, pObject );

	 /*if(m_currentShader == -1)
	    return;
	 
	 D3DXMATRIX mat, wldTranspose, viewMatrix;
	 
	 D3DXMatrixTranspose( &wldTranspose, pObject );
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

}




