#include "stdafx.h"
#include "CViewToSurface.h"
#include "CTextureDBClass.h"
#include "globalstatusvariables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

IDirect3DSurface9 * CViewToSurface::GetRenderTargetSurfaceX(void * textureDBMem, 
															void * gStatusMem)
{
     CTextureDBObjList * textureDB = (CTextureDBObjList *)textureDBMem;
	 GlobalStatusVariables * gStatus = (GlobalStatusVariables *)gStatusMem;

     if(m_textureIndex < textureDB->directAccessObjectCount && m_textureIndex > -1)
	 {//bounds
	    if(m_curSurface)
		   m_curSurface->Release();
		m_curSurface = 0;
        //verify render target
		if(textureDB->directAccess[m_textureIndex].object->m_textureObject->m_usage == 0)
		{//must convert
		   textureDB->directAccess[m_textureIndex].object->m_textureObject->m_usage = 1;
		   textureDB->directAccess[m_textureIndex].object->m_textureObject->m_dwMipMapCount = 1;
		   BOOL processImage = TRUE;
           int finalMaxSize = gStatus->m_dxRenderer->m_currentHeight;
		   if(gStatus->m_dxRenderer->m_currentWidth < finalMaxSize)//why because render targets can be bigger than the depth buffer :|
		      finalMaxSize = gStatus->m_dxRenderer->m_currentWidth;
		   
		   
		   textureDB->directAccess[m_textureIndex].object->m_textureObject->Restore(	gStatus->m_dxRenderer->m_pd3dDevice,
																						0,
																						textureDB->directAccess[m_textureIndex].object->m_bump,
																						textureDB->directAccess[m_textureIndex].object->m_bumpType,
																						finalMaxSize,
																						gStatus->m_renderStateObject->m_pCaps,
																						gStatus->m_compressTextures,
																						processImage,
																						gStatus->m_advUrlBasic,
																						gStatus->m_advUrlVending);

		}//end must convert
        m_tempTargetWidthPx = textureDB->directAccess[m_textureIndex].object->m_textureObject->m_widthBasis;
        m_tempTargetHeightPx = textureDB->directAccess[m_textureIndex].object->m_textureObject->m_hieghtBasis;

		if(textureDB->directAccess[m_textureIndex].object->m_textureObject->m_pddsSurface->GetSurfaceLevel(0,&m_curSurface)!=S_OK)
		   return NULL;
		//deal with mip maps here, or utilize the render target indexes and channels
        return m_curSurface;
	 }//end bounds
	 return NULL;
}