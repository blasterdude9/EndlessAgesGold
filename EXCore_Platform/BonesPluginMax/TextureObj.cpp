//-----------------------------------------------------------------------------

#include "afxwin.h"
#include "afxext.h"
//#include "stdafx.h"
#define D3D_OVERLOADS
#include "resource.h"
#include <d3d9.h>
#include <D3DX9.h>
//#include <afxtempl.h>
#include "textureObj.h"

//Externals shared between classes
CString GetFileExt(CString currentName);
//-----------------------------------------------------------------------------
// Name: CD3DMipMap()
// Desc: Constructs a mipmap object
//-----------------------------------------------------------------------------
CTextureEX::CTextureEX()
{
	m_fileType = 0;//0=bmp 1=tga type
    m_pddsSurface = NULL;
	m_mapFileList = NULL;
}
//-----------------------------------------------------------------------------
// Name: Safe Delete()
// Desc: Constructs a mipmap object
//-----------------------------------------------------------------------------
BOOL CTextureEX::SafeDelete()
{
   
   if(m_mapFileList)
   {
      for(POSITION posLoc = m_mapFileList->GetHeadPosition();posLoc != NULL;)
	  {//map loop
        CMapObj * mapPtr = (CMapObj *)m_mapFileList->GetNext(posLoc);
        if(mapPtr->m_mapFileType == 1){
		   if(mapPtr->m_rgbData.m_rgbaData)
           delete [] mapPtr->m_rgbData.m_rgbaData;
           mapPtr->m_rgbData.m_rgbaData=0;
		}
		delete mapPtr;
		mapPtr=0;
	  }
      m_mapFileList->RemoveAll();
      delete m_mapFileList;
      m_mapFileList=0;
   }

   if(m_pddsSurface)
   {
      m_pddsSurface->Release();
	  m_pddsSurface = 0;
   }


   return TRUE;
}
//-----------------------------------------------------------------------------
// Name: Safe Delete()
// Desc: cach raw data thats not needed
//-----------------------------------------------------------------------------
void CTextureEX::CacheRawData()
{
   
   if(m_mapFileList)
   {
      for(POSITION posLoc = m_mapFileList->GetHeadPosition();posLoc != NULL;)
	  {//map loop
        CMapObj * mapPtr = (CMapObj *)m_mapFileList->GetNext(posLoc);
        if(mapPtr->m_mapFileType == 1){
		   if(mapPtr->m_rgbData.m_rgbaData)
           delete [] mapPtr->m_rgbData.m_rgbaData;
           mapPtr->m_rgbData.m_rgbaData=0;
		}
		delete mapPtr;
		mapPtr=0;
	  }
      m_mapFileList->RemoveAll();
      delete m_mapFileList;
      m_mapFileList=0;
   }

   return;
}

 void CTextureEX::Callback(LPDIRECT3DDEVICE9 pd3dDevice,int stage,int & type,float & perameter)
{
   if(type == 1)
   {
      float wavesize = 0.09f;
	  float waveSpeed = 1.0f;
	  float waveSpeed2 = 12.0f;
	  float waveSpeed3 = 5.0f;
	  float waveSpeed4 = 12.0f;
	
	  double curTime = (double)GetTickCount()*perameter;//.001;//true wave speed control
	  D3DXMATRIXA16 m_localMatrix;
      m_localMatrix._11 =  wavesize * cosf( curTime * waveSpeed );
      m_localMatrix._12 = -wavesize * sinf( curTime * waveSpeed2 );
      m_localMatrix._21 =  wavesize * sinf( curTime * waveSpeed3 );
      m_localMatrix._22 =  wavesize * cosf( curTime * waveSpeed4 );
      // Set up texture stage 0's states for the bumpmap
      pd3dDevice->SetTextureStageState( stage, D3DTSS_BUMPENVMAT00,   F2DW( m_localMatrix._11 ) );
      pd3dDevice->SetTextureStageState( stage, D3DTSS_BUMPENVMAT01,   F2DW( m_localMatrix._12 ) );
      pd3dDevice->SetTextureStageState( stage, D3DTSS_BUMPENVMAT10,   F2DW( m_localMatrix._21 ) );
      pd3dDevice->SetTextureStageState( stage, D3DTSS_BUMPENVMAT11,   F2DW( m_localMatrix._22 ) );
	  pd3dDevice->SetTextureStageState( stage, D3DTSS_BUMPENVLSCALE,  F2DW(0.8f) );
      pd3dDevice->SetTextureStageState( stage, D3DTSS_BUMPENVLOFFSET, F2DW(0.0f) );
   }
   if(type == 2)
   {
      pd3dDevice->SetTextureStageState( stage, D3DTSS_BUMPENVMAT00, F2DW(0.5f) );
      pd3dDevice->SetTextureStageState( stage, D3DTSS_BUMPENVMAT01, F2DW(0.0f) );
      pd3dDevice->SetTextureStageState( stage, D3DTSS_BUMPENVMAT10, F2DW(0.0f) );
      pd3dDevice->SetTextureStageState( stage, D3DTSS_BUMPENVMAT11, F2DW(0.5f) );
      pd3dDevice->SetTextureStageState( stage, D3DTSS_BUMPENVLSCALE, F2DW(perameter) );
      pd3dDevice->SetTextureStageState( stage, D3DTSS_BUMPENVLOFFSET, F2DW(0.0f) );
   }
}
//-----------------------------------------------------------------------------
// Name: LoadIt()
// Desc: builds
//-----------------------------------------------------------------------------
BOOL CTextureEX::LoadIt(CString fileName,
						int mipLevels,
						LPDIRECT3DDEVICE9 pd3dDevice)
{
	m_mapFileList = new CMapObjList;
	
	CString extension = GetFileExt(fileName);
	extension.MakeLower();
	    if(extension == "tga")
           m_fileType = 1;
		else
		   return FALSE;
		   

           CMapObj * mapPtr = new CMapObj(fileName);
           mapPtr->m_mapFileType = m_fileType;

           IDirect3DTexture9 * highlevelGenSurface = NULL;

           /*if( FAILED(D3DXCreateTextureFromFileEx( pd3dDevice, fileName, 
                      D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_A1R5G5B5, 
                      D3DPOOL_MANAGED,
					  D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
                      D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 0, NULL, NULL, &highlevelGenSurface )))
				return FALSE;*/

           //get dimensions
           IDirect3DSurface9 * topSurface = NULL;
	       highlevelGenSurface->GetSurfaceLevel(0,&topSurface);

		   D3DSURFACE_DESC surfaceDescription;
           topSurface->GetDesc(&surfaceDescription);

		   mapPtr->m_rgbData.m_tgaWidth   = surfaceDescription.Width;
	       mapPtr->m_rgbData.m_tgaHieght  = surfaceDescription.Height;

           /*if(mapPtr->m_rgbData.m_tgaWidth > 256 || mapPtr->m_rgbData.m_tgaHieght > 256)
		   {
              AfxMessageBox(operator+("out of size texture (over 256) :",fileName));
              return FALSE;
		   }*/

		   m_widthBasis    = surfaceDescription.Width;
		   m_hieghtBasis   = surfaceDescription.Height;


		   m_dwMipMapCount = mipLevels;

		   int szSquared  = m_widthBasis;
		   int maxMipMaps = 0;
           //get max mipmaps
		   while(szSquared > 1)
		   {
                 szSquared = szSquared/2;
				 maxMipMaps++;
		   }

		   if(mipLevels > maxMipMaps)
              m_dwMipMapCount = maxMipMaps;



	       mapPtr->m_rgbData.m_rgbaData   = new WORD[(surfaceDescription.Width*surfaceDescription.Height)];
		  
           //get bit data
		   D3DLOCKED_RECT surfaceBits;
	       if(highlevelGenSurface->LockRect(0,&surfaceBits,NULL,0)==0)
		   {
              INT lPitch = surfaceBits.Pitch;
              BYTE* pBytes = (BYTE*)surfaceBits.pBits;
            
              for( DWORD y=0; y<surfaceDescription.Height; y++ )
			  {
                 DWORD* pDstData32 = (DWORD*)pBytes;
                 WORD*  pDstData16 = (WORD*)pBytes;

                 for( DWORD x=0; x<surfaceDescription.Width; x++ )
				 {
                    //DWORD dwPixel = tgaData[y*surfaceDescription.Width+x];
                    mapPtr->m_rgbData.m_rgbaData[y*surfaceDescription.Width+x] = pDstData16[x];
                        //pDstData16[x] = (WORD)dwPixel;
					
				 }
    
                 pBytes += lPitch;
			  }
	          highlevelGenSurface->UnlockRect(0);
		   }
           
		   if(highlevelGenSurface)
			  highlevelGenSurface->Release();
		      highlevelGenSurface = 0;

		   if(topSurface)
		   {
              topSurface->Release();
			  topSurface = 0;
		   }
		   
		   m_mapFileList->AddTail(mapPtr);
return TRUE;
}
//-----------------------------------------------------------------------------
// Name: MipMap_Restore()
// Desc: Creates the device-dependant surface and texture for the mipmap
//-----------------------------------------------------------------------------
HRESULT CTextureEX::Restore( IDirect3DDevice9 * pd3dDevice,
							 BOOL blitOn,
							 int bump,
							 int bumpType,
							 int & maxTextureSize,
							 D3DCAPS9 & pCaps)
{
    // Release any previously created objects
    Invalidate();

	//maxTextureSize = 256;

	//if(maxTextureSize < m_widthBasis || maxTextureSize < m_hieghtBasis)
	//   return -1;//ERROR loading textures
    DWORD textureCRWidth = m_widthBasis;
	DWORD textureCRHeight = m_hieghtBasis;
	if(m_widthBasis > maxTextureSize)
	{
	   textureCRWidth = maxTextureSize;
	   if(maxTextureSize == 256)
	      m_dwMipMapCount = 8;
	   else if(maxTextureSize == 512)
	      m_dwMipMapCount = 9;
	   else
	      m_dwMipMapCount = 8;
	}
	if(m_hieghtBasis > maxTextureSize)
	   textureCRHeight = maxTextureSize;
    
    


    //HRESULT err = D3DXCreateTexture(pd3dDevice,m_widthBasis,m_hieghtBasis,0,0,D3DFMT_A1R5G5B5,D3DPOOL_MANAGED, &m_pddsSurface);
    HRESULT err = pd3dDevice->CreateTexture(textureCRWidth,
		                                    textureCRHeight,
                                            m_dwMipMapCount,
                                            0,//DWORD Usage,
                                            D3DFMT_A1R5G5B5,//D3DFORMAT Format,
                                            D3DPOOL_MANAGED,//D3DPOOL Pool,
                                            &m_pddsSurface,
											NULL);
	if(err != 0)
	   return err;
    // Loop through each surface in the mipmap, copying the bitmap to the temp
    // surface, and then blitting the temp surface to the real one.
    IDirect3DTexture9 * pddsDest   = m_pddsSurface;
	IDirect3DTexture9 * persistMap = NULL;

	int trace = 1;

    POSITION posLoc = m_mapFileList->FindIndex(0);
	if(posLoc)
	{
       CMapObj * mapPtr = (CMapObj *)m_mapFileList->GetAt(posLoc);
       if(CopyRGBADataToSurface(mapPtr->m_rgbData.m_rgbaData,
					            pddsDest,
							    persistMap,
							    m_widthBasis,
							    m_hieghtBasis,
								maxTextureSize,
								maxTextureSize,
								textureCRWidth,
                                textureCRHeight
								    )!=0){

           
           //failed
	   }
       //Bump Mapping
	   

	   if(bump > 0)
	   {//enable bump mapping
	      D3DSURFACE_DESC    desc;
	      m_pddsSurface->GetLevelDesc(0,&desc);
	      if(bumpType == 0)
		  {//dot product bump map
		    
	        IDirect3DTexture9 * pHeightMap;
			/*if(pCaps.TextureOpCaps & D3DTEXOPCAPS_DOTPRODUCT3)
	        if(D3DXCreateTexture(pd3dDevice,desc.Width,desc.Height,m_dwMipMapCount,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED, &pHeightMap)==S_OK)
		    {			 
			 D3DXComputeNormalMap(pHeightMap,m_pddsSurface,NULL,0,D3DX_CHANNEL_RED,bump);
	         m_pddsSurface->Release();
	         m_pddsSurface = pHeightMap;
		    }*/
		  }//end dot product bump map
		  else if(bumpType == 1)
		  {//enviorment
             // Create the bump map texture
		     
			 IDirect3DTexture9 * pHeightMap;
			 if(pCaps.TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAP)
             if(pd3dDevice->CreateTexture( desc.Width, desc.Height, m_dwMipMapCount, 0 ,D3DFMT_V8U8, D3DPOOL_MANAGED,&pHeightMap, NULL )==S_OK)
			 {//Create Texture
			    m_pddsSurface->Release();
			    m_pddsSurface = pHeightMap;

	            D3DLOCKED_RECT d3dlr;
                m_pddsSurface->LockRect( 0, &d3dlr, 0, 0 );
                CHAR* pDst = (CHAR*)d3dlr.pBits;
                CHAR  iDu, iDv;
				float waveDensity = (float)bump;//larger is more waves
	
                for( DWORD y=0; y<desc.Height; y++ )
                {
                     CHAR* pPixel = pDst;
                     for( DWORD x=0; x<desc.Width; x++ )
                     {
			              FLOAT fx = x/(FLOAT)desc.Width - 0.5f;
                          FLOAT fy = y/(FLOAT)desc.Height - 0.2f;
                          FLOAT r = sqrtf( fx*fx + fy*fy );

                          iDu = (CHAR)( 32 * cosf( waveDensity * ( fx + fy ) ) );
                          iDu += (CHAR)( 16 * cosf( waveDensity * ( fx * 0.85f - fy ) ) );

                          iDv = (CHAR)( 32 * sinf( waveDensity * ( fx + fy ) ) );
                          iDv += (CHAR)( 16 * sinf( waveDensity * ( fx * 0.85f - fy ) ) );

                          *pPixel++ = iDu;
                          *pPixel++ = iDv;
					 }
                   pDst += d3dlr.Pitch;
                }
                m_pddsSurface->UnlockRect(0);//*/
			 }//end Create Texture
		  }//end enviorment
	      else if(bumpType == 2)
	      {//Existing bump 2
           LPDIRECT3DTEXTURE9 psBumpSrc = NULL;//m_pddsSurface;
           D3DLOCKED_RECT     d3dlr;
           IDirect3DTexture9 * m_psBumpMap;

           // Create the bumpmap's surface and texture objects
		   if (pCaps.TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAP)
           if(pd3dDevice->CreateTexture( desc.Width, desc.Height, m_dwMipMapCount, 0, D3DFMT_V8U8, D3DPOOL_MANAGED, &m_psBumpMap, NULL )==S_OK )
           {//Create texture 
			  if( FAILED( pd3dDevice->CreateTexture( desc.Width, desc.Height, 1, 0,
                  D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &psBumpSrc, NULL ) ) )
              {
                  
              }

			  IDirect3DSurface9 * currentMipSurface = NULL;
              psBumpSrc->GetSurfaceLevel(0,&currentMipSurface);
	          IDirect3DSurface9 * topSurface = NULL;
              m_pddsSurface->GetSurfaceLevel(0,&topSurface);
              //D3DXLoadSurfaceFromSurface(currentMipSurface, NULL, NULL, topSurface, NULL, NULL, D3DX_FILTER_POINT , 0);

	          if(currentMipSurface)
	          {
                 currentMipSurface->Release();
                 currentMipSurface = 0;
	          }
			  if(topSurface)
	          {
                 topSurface->Release();
                 topSurface = 0;
	          }
  
           // Fill the bits of the new texture surface with bits from
           // a private format.
           psBumpSrc->LockRect( 0, &d3dlr, 0, 0 );
           DWORD dwSrcPitch = (DWORD)d3dlr.Pitch;
           BYTE* pSrcTopRow = (BYTE*)d3dlr.pBits;
           BYTE* pSrcCurRow = pSrcTopRow;
           BYTE* pSrcBotRow = pSrcTopRow + (dwSrcPitch * (desc.Height - 1) );

           m_psBumpMap->LockRect( 0, &d3dlr, 0, 0 );
           DWORD dwDstPitch = (DWORD)d3dlr.Pitch;
           BYTE* pDstTopRow = (BYTE*)d3dlr.pBits;
           BYTE* pDstCurRow = pDstTopRow;

             for( DWORD y=0; y<desc.Height; y++ )
             {//hight loop
                BYTE* pSrcB0; // addr of current pixel
                BYTE* pSrcB1; // addr of pixel below current pixel, wrapping to top if necessary
                BYTE* pSrcB2; // addr of pixel above current pixel, wrapping to bottom if necessary
                BYTE* pDstT;  // addr of dest pixel;
                pSrcB0 = pSrcCurRow;
                if( y == desc.Height - 1)
                    pSrcB1 = pSrcTopRow;
                else
                    pSrcB1 = pSrcCurRow + dwSrcPitch;
                
				if( y == 0 )
                    pSrcB2 = pSrcBotRow;
                else
                    pSrcB2 = pSrcCurRow - dwSrcPitch;

                pDstT = pDstCurRow;

                for( DWORD x=0; x<desc.Width; x++ )
                {//primary loop
                     LONG v00; // Current pixel
                     LONG v01; // Pixel to the right of current pixel, wrapping to left edge if necessary
                     LONG vM1; // Pixel to the left of current pixel, wrapping to right edge if necessary
                     LONG v10; // Pixel one line below.
                     LONG v1M; // Pixel one line above.

                     v00 = *(pSrcB0+0);

                     if( x == desc.Width - 1 )
                         v01 = *(pSrcCurRow);
                     else
                         v01 = *(pSrcB0+4);

                     if( x == 0 )
                         vM1 = *(pSrcCurRow + (4 * (desc.Width - 1)));
                     else
                         vM1 = *(pSrcB0-4);
                     v10 = *(pSrcB1+0);
                     v1M = *(pSrcB2+0);

                     LONG iDu = (vM1-v01); // The delta-u bump value
                     LONG iDv = (v1M-v10); // The delta-v bump value

                     // The luminance bump value (land masses are less shiny)
                     WORD uL = ( v00>1 ) ? 63 : 127;
					 //WORD uL = ( v00>1 ) ? 30 : 100;

                     switch( D3DFMT_V8U8 )
                     {//sw
                       case D3DFMT_V8U8:
                            *pDstT++ = (BYTE)iDu;
                            *pDstT++ = (BYTE)iDv;
                       break;

                       case D3DFMT_L6V5U5:
                            *(WORD*)pDstT  = (WORD)( ( (iDu>>3) & 0x1f ) <<  0 );
                            *(WORD*)pDstT |= (WORD)( ( (iDv>>3) & 0x1f ) <<  5 );
                            *(WORD*)pDstT |= (WORD)( ( ( uL>>2) & 0x3f ) << 10 );
                             pDstT += 2;
                       break;

                       case D3DFMT_X8L8V8U8:
                            *pDstT++ = (BYTE)iDu;
                            *pDstT++ = (BYTE)iDv;
                            *pDstT++ = (BYTE)uL;
                            *pDstT++ = (BYTE)0L;
                       break;
					 }//end sw
                     // Move one pixel to the right (src is 32-bpp)
                     pSrcB0+=4;
                     pSrcB1+=4;
                     pSrcB2+=4;
				}//end primary loop
                // Move to the next line
                pSrcCurRow += dwSrcPitch;
                pDstCurRow += dwDstPitch;
			 }//end hight loop
             m_psBumpMap->UnlockRect(0);
             psBumpSrc->UnlockRect(0);
             psBumpSrc->Release();
			 psBumpSrc = 0;
			 m_pddsSurface->Release();
			 m_pddsSurface = m_psBumpMap;
		   }//end create texture
	     }//end Existing bump 2
}//end enable bump mapping

      //m_dwMipMapCount--;
	  //m_dwMipMapCount--;

       if(m_dwMipMapCount<2)
          return S_OK;

     

       //generate mip maps
	   IDirect3DSurface9 * topSurface = NULL;
	   m_pddsSurface->GetSurfaceLevel(0,&topSurface);
       for(int loop=1;loop<m_dwMipMapCount;loop++)
	   {
		   IDirect3DSurface9 * currentMipSurface = NULL;
		   //IDirect3DSurface9 * aboveMipLevel = NULL;
           m_pddsSurface->GetSurfaceLevel(loop,&currentMipSurface);
		   //m_pddsSurface->GetSurfaceLevel(loop-1,&aboveMipLevel);

		   /*if(!blitOn)
		   D3DXLoadSurfaceFromSurface(currentMipSurface, NULL, NULL, topSurface, NULL, NULL, D3DX_FILTER_POINT , 0);
		   else
	       D3DXLoadSurfaceFromSurface(currentMipSurface, NULL, NULL, topSurface, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);*/

	       if(currentMipSurface)
		   {
              currentMipSurface->Release();
              currentMipSurface = 0;
		   }
		   /*if(aboveMipLevel)
		   {
              aboveMipLevel->Release();
              aboveMipLevel = 0;
		   }*/
	   }
       if(topSurface)
	   {
          topSurface->Release();
          topSurface = 0;
	   }

	}

    return S_OK;
}




//-----------------------------------------------------------------------------
// Name: MipMap_Invalidate()
// Desc: Frees device dependant objects for the mipmap
//-----------------------------------------------------------------------------
VOID CTextureEX::Invalidate()
{
	if(m_pddsSurface)
	   m_pddsSurface->Release();
	   m_pddsSurface=0;
}
//-----------------------------------------------------------------------------
// Name: CopyRGBADataToSurface()
// Desc: Invalidates the current texture objects and rebuilds new ones
//       using the new device.
//-----------------------------------------------------------------------------
HRESULT CTextureEX::CopyRGBADataToSurface(WORD * tgaData,
										  IDirect3DTexture9 * surface,
										  IDirect3DTexture9 * persistMap,
										  int m_baseWidth,
										  int m_baseHieght,
										  int maxTextureWidth,
										  int maxTextureHeight,
										  int createWidth,
										  int createHeight)
{

	if(!persistMap)
	{//primary map incode this
     D3DLOCKED_RECT surfaceBits;
	 if(m_baseWidth <= maxTextureWidth)
	 {//size is supported
	  if(surface->LockRect(0,&surfaceBits,NULL,0)==0)
	  {
         INT lPitch = surfaceBits.Pitch;
         BYTE* pBytes = (BYTE*)surfaceBits.pBits;
            
            for( DWORD y=0; y<m_baseHieght; y++ )
			{
                 DWORD* pDstData32 = (DWORD*)pBytes;
                 WORD*  pDstData16 = (WORD*)pBytes;

                 for( DWORD x=0; x<m_baseWidth; x++ )
				 {
                    DWORD dwPixel = tgaData[y*m_baseWidth+x];

                        pDstData16[x] = (WORD)dwPixel;
					
				 }
    
                 pBytes += lPitch;
			}
	     surface->UnlockRect(0);
	  }
	 }//end size is supported
	 else
	 {//must truncate this
	  int multiplier = m_baseWidth / createWidth;
      if(surface->LockRect(0,&surfaceBits,NULL,0)==0)
	  {
         INT lPitch = surfaceBits.Pitch;
         BYTE* pBytes = (BYTE*)surfaceBits.pBits;
            
            for( DWORD y=0; y<createHeight; y++ )
			{
                 DWORD* pDstData32 = (DWORD*)pBytes;
                 WORD*  pDstData16 = (WORD*)pBytes;

                 for( DWORD x=0; x<createWidth; x++ )
				 {
                    DWORD dwPixel = tgaData[((y*multiplier)*m_baseWidth+(x*multiplier))];
                    pDstData16[x] = (WORD)dwPixel;
				 }
                 pBytes += lPitch;
			}
	     surface->UnlockRect(0);
	  }
	 }//end must truncate this
	}//end primary map incode this
   
    return S_OK;
}
//---------------------------------------//
//---------------------------------------//

void CTextureEX::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){

	   ar << m_widthBasis
		  << m_hieghtBasis
		  << m_fileType
		  << m_dwMipMapCount
		  << m_mapFileList;
	}
	else
	{
       ar >> m_widthBasis
		  >> m_hieghtBasis
		  >> m_fileType
		  >> m_dwMipMapCount
		  >> m_mapFileList;  

	     m_pddsSurface = NULL;
	}
}

//------------------------------//
//---------map lists------------//
//------------------------------//
CMapObj::CMapObj(CString filename)
{
    m_fileName = filename;
	m_rgbData.m_rgbaData = NULL;
	m_rgbData.m_tgaBBP = 0; 
	m_rgbData.m_tgaHieght = 0;
	m_rgbData.m_tgaWidth = 0;
}
//------------------------------//
//------------------------------//
//------------------------------//
void CMapObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){
	   ar << m_fileName
		  << m_mapFileType;
          
	   if(m_mapFileType == 1){//tga type
	      ar << m_rgbData.m_tgaWidth
             << m_rgbData.m_tgaHieght
	         << m_rgbData.m_tgaBBP;
             for(int count=0;count < (m_rgbData.m_tgaWidth*m_rgbData.m_tgaHieght);count++)
		         ar << m_rgbData.m_rgbaData[count];
	   }//end tga type
	}
	else
	{


       ar >> m_fileName
          >> m_mapFileType;
       if(m_mapFileType == 1){//tga type
       ar >> m_rgbData.m_tgaWidth
          >> m_rgbData.m_tgaHieght
	      >> m_rgbData.m_tgaBBP;
	    
	      long countObj = m_rgbData.m_tgaWidth*m_rgbData.m_tgaHieght;
	      if(countObj > 0)
	      m_rgbData.m_rgbaData = new WORD[(m_rgbData.m_tgaWidth*m_rgbData.m_tgaHieght)];   
		  else
		  m_rgbData.m_rgbaData = NULL;

          for(int count=0;count < countObj;count++)
		      ar >> m_rgbData.m_rgbaData[count];
       
	   
	   }//end tga type	
	}
}
//------------------------------//
//------------------------------//
//------------------------------//
int CMapObjList::AddTextureSet(CString fileName,int mipLevels)
{
CString fileExt = GetFileExt(fileName);
fileExt.MakeLower();
RemoveFileExt(&fileName);
CString original = fileName;

char stage = 'a';
int loop=0;
	for(loop=0;loop<mipLevels;loop++){//loop
        CString conv = (CString)stage;
		CString final;
		if(loop != 0){
		final = operator+(original,stage);
        stage++;
		}
		else
        final = original;
		
		CMapObj * mpPtr = new CMapObj(operator+(operator+(final,"."),fileExt));
		

		FILE* file = fopen( mpPtr->m_fileName, "rb" );
        if( NULL == file ){
			delete mpPtr;
			mpPtr=0;
            return loop;
		}
		fclose(file);
		AddTail(mpPtr);
		
	}//end loop
	return loop;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----remove a new file ext-----------------------------------//
void CMapObjList::RemoveFileExt(CString *currentName)
{
CString tempName = *currentName;
CString builtName;
int length = tempName.GetLength();
BOOL done = FALSE;
 for(int loop=0;loop<length;loop++){     
  CString chr = (CString)tempName.GetAt(loop);
  if(chr == ".")
	 break;
  builtName = operator+( builtName, chr);  
 }
*currentName = builtName;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----Get File Ext--------------------------------------------//
CString GetFileExt(CString currentName)
{
CString ext;
int length = currentName.GetLength();
BOOL done = FALSE;
 while(done == FALSE){
  CString chr = (CString)currentName.GetAt(length-1);
  
  if(chr == ".")
	 break;

  ext = operator+(chr,ext);
  length = length -1;
 }
return ext;
}


IMPLEMENT_SERIAL(CTextureEX, CObject, 0)
IMPLEMENT_SERIAL(CMapObj, CObject, 0)
IMPLEMENT_SERIAL(CMapObjList, CObList, 0)
