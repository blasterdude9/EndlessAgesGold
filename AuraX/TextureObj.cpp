//-----------------------------------------------------------------------------
#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <D3DX9.h>
#include <afxtempl.h>
#include "textureObj.h"
#include "HttpFileDownloader.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//Externals shared between classes
CString GetFileExt(CString currentName);
void WriteDebugFile(CString filename,CString debugString);
//-----------------------------------------------------------------------------
CTextureEX::CTextureEX()
{
	m_fileType				= 0;//0=bmp 1=tga type
    m_pddsSurface			= NULL;
	m_mapFileList			= NULL;
	m_tempSurface			= FALSE;
	m_restoreType			= 0;
	m_format				= D3DFMT_UNKNOWN;
	m_rawMapData			= NULL;
	m_rawMapDataLength		= NULL;
	m_compressedOvrd		= 0;
	m_usage					= 0;
}
//-----------------------------------------------------------------------------
BOOL CTextureEX::SafeDelete()
{
   
   if(m_mapFileList)
   {
      for(POSITION posLoc = m_mapFileList->GetHeadPosition();posLoc != NULL;)
	  {//map loop
        CMapObj * mapPtr = (CMapObj *)m_mapFileList->GetNext(posLoc);
        
		   if(mapPtr->m_rgbData.m_rgbaData16)
           delete [] mapPtr->m_rgbData.m_rgbaData16;
           mapPtr->m_rgbData.m_rgbaData16=0;

		   if(mapPtr->m_rgbData.m_rgbaData32)
           delete [] mapPtr->m_rgbData.m_rgbaData32;
           mapPtr->m_rgbData.m_rgbaData32=0;
		
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

   if(m_rawMapData)
   {
      delete [] m_rawMapData;
      m_rawMapData = 0;
   }

   return TRUE;
}
//-----------------------------------------------------------------------------
void CTextureEX::Clone(CTextureEX **copy)
{
    CTextureEX * clone        = new CTextureEX();
	clone->m_fileType         = m_fileType;//legacy format use only
	clone->m_widthBasis       = m_widthBasis;
    clone->m_hieghtBasis      = m_hieghtBasis;
	clone->m_depth            = m_depth;
	clone->m_dwMipMapCount    = m_dwMipMapCount;
	clone->m_fileName         = m_fileName;
	clone->m_format           = m_format;
	clone->m_rawMapDataLength = m_rawMapDataLength;
	clone->m_restoreType      = m_restoreType;
	clone->m_compressedOvrd   = m_compressedOvrd;
	clone->m_usage			  = m_usage;

	if(m_restoreType == 0)
	{//handle legacy data
      clone->m_pddsSurface = this->m_pddsSurface;//borrow surface memory
      clone->CopyDataLiveDataToBaseData(0);//generate new data
      clone->m_pddsSurface = NULL;
	  clone->m_restoreType = 1;
	}//end handle legacy data
	
	if(m_rawMapDataLength>0)
	{
	   clone->m_rawMapData = new byte[m_rawMapDataLength];
       CopyMemory(clone->m_rawMapData,m_rawMapData,m_rawMapDataLength);
	}

	*copy = clone;
}
//-----------------------------------------------------------------------------
void CTextureEX::CacheRawData()
{
   if(m_rawMapData)
   {
      delete [] m_rawMapData;
	  m_rawMapData = 0;
   }


   if(m_mapFileList)
   {
      


      for(POSITION posLoc = m_mapFileList->GetHeadPosition();posLoc != NULL;)
	  {//map loop
        CMapObj * mapPtr = (CMapObj *)m_mapFileList->GetNext(posLoc);
 
		   if(mapPtr->m_rgbData.m_rgbaData16)
              delete [] mapPtr->m_rgbData.m_rgbaData16;
              mapPtr->m_rgbData.m_rgbaData16=0;

		   if(mapPtr->m_rgbData.m_rgbaData32)
              delete [] mapPtr->m_rgbData.m_rgbaData32;
              mapPtr->m_rgbData.m_rgbaData32=0;
		
		delete mapPtr;
		mapPtr=0;
	  }
      m_mapFileList->RemoveAll();
      delete m_mapFileList;
      m_mapFileList=0;
   }

   return;
}
//-----------------------------------------------------------------------------
 void CTextureEX::Callback(LPDIRECT3DDEVICE9 pd3dDevice,int stage,int & type,float & perameter)
{
   if(type == 1)
   {
      float wavesize = 0.09f;
	  float waveSpeed = 1.0f;
	  float waveSpeed2 = 12.0f;
	  float waveSpeed3 = 5.0f;
	  float waveSpeed4 = 12.0f;
	
	  float curTime = (float)GetTickCount()*perameter;//.001;//true wave speed control
	  D3DXMATRIXA16 m_localMatrix;
      m_localMatrix._11 =  wavesize * cosf( curTime * waveSpeed );
      m_localMatrix._12 = -wavesize * sinf( curTime * waveSpeed2 );
      m_localMatrix._21 =  wavesize * sinf( curTime * waveSpeed3 );
      m_localMatrix._22 =  wavesize * cosf( curTime * waveSpeed4 );
      //Set up texture stage 0's states for the bumpmap
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

/*HRESULT WINAPI D3DXCreateTextureFromFileInMemory(          LPDIRECT3DDEVICE9 pDevice,
    LPCVOID pSrcData,
    UINT SrcDataSize,
    LPDIRECT3DTEXTURE9 *ppTexture
);*/
/*
HRESULT WINAPI D3DXCreateTextureFromFileInMemoryEx(          LPDIRECT3DDEVICE9 pDevice,
    LPCVOID pSrcData,
    UINT SrcDataSize,
    UINT Width,
    UINT Height,
    UINT MipLevels,
    DWORD Usage,
    D3DFORMAT Format,
    D3DPOOL Pool,
    DWORD Filter,
    DWORD MipFilter,
    D3DCOLOR ColorKey,
    D3DXIMAGE_INFO *pSrcInfo,
    PALETTEENTRY *pPalette,
    LPDIRECT3DTEXTURE9 *ppTexture
);*/

//-----------------------------------------------------------------------------
BOOL CTextureEX::LoadItFromURL(CString url,
						       int mipLevels,
						       LPDIRECT3DDEVICE9 pd3dDevice,
						       int tempSurface,
							   bool createData,
							   CString & advUrlBasic,
							   CString & advUrlVending)
{
	      // nothing to fetch without a content server (client builds): never touch WinINet
	      if(url.Left(4).CompareNoCase("http") != 0)
	          return FALSE;

	      this->m_restoreType = 2;
	      m_dwMipMapCount = mipLevels;
           /*CString urlBase = "http://72.29.77.21/xthreads/chatbot.aspx?botname=$bName&question=$text";     
                    urlBase.Replace("$bName","Droid");
					urlBase.Replace("$text","hello");
					//CString response = "";
                    CHttpFileDownloader * urlHanlder = new CHttpFileDownloader();
					DWORD bufLen = 0;
					BYTE * bufferX = NULL;
					if(urlHanlder->GetUrlData("botRequest",urlBase.GetBuffer(),&bufferX,&bufLen))
					{


					}
                    delete urlHanlder;
					urlHanlder = 0;*/
           this->m_fileName = url;
		   if(url.GetLength() > 15)
		   {
              CString temp = url;
			  temp.Delete(15,url.GetLength()-15);
		      if(temp.CompareNoCase("www.advertising")==0)
                 url = advUrlBasic;
			  else if(temp.CompareNoCase("www.advertivend")==0)
				 url = advUrlVending;
		   }



           CHttpFileDownloader * urlFileDownloader = new CHttpFileDownloader();
		   BYTE * buffer = NULL;
		   DWORD bufLength = 0;
		   if(urlFileDownloader->GetUrlData("iexplorer data",
													   url.GetBuffer(),
													   &buffer,
													   &bufLength))
		   {
			   /*int result = urlFileDownloader->DownloadChunk(buffer);
					while(result==1)
					{
                         result = urlFileDownloader->DownloadChunk(buffer);
					}*/
			   delete urlFileDownloader;
		       urlFileDownloader = 0;
			   /*if(result != 2)
                  return FALSE;*/
		   }
		   else
		   {
               delete urlFileDownloader;
		       urlFileDownloader = 0;
			   return FALSE;
		   }

	       m_fileType = 1;
		   
           IDirect3DTexture9 * highlevelGenSurface = NULL;
		   D3DXIMAGE_INFO imageInfo;

           if( FAILED(D3DXCreateTextureFromFileInMemoryEx( pd3dDevice, buffer, bufLength,
                      D3DX_DEFAULT, D3DX_DEFAULT, m_dwMipMapCount, 0, D3DFMT_UNKNOWN,//D3DFMT_A8R8G8B8, 
                      D3DPOOL_MANAGED,
					  D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
                      D3DX_FILTER_NONE/*D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR*/,
					  0, &imageInfo, NULL, &highlevelGenSurface )))
		   {
                if(buffer)
				   delete [] buffer;
				buffer = 0;
  

				return FALSE;
		   }

		   if(buffer)
	          delete [] buffer;
		   buffer = 0;

		   m_format           = imageInfo.Format;
           m_restoreType      = 2;
           m_widthBasis       = imageInfo.Width;
           m_hieghtBasis      = imageInfo.Height;
		   m_depth            = imageInfo.Depth;
	       m_dwMipMapCount    = mipLevels;
		   //m_fileName         = url;

		   CString temp = GetFormatName();
		   if(temp.CompareNoCase("Unknown")==0)
		   {
			   AfxMessageBox("Format not supported on file:"+url);
              return FALSE;
		   }

	       
		   if(tempSurface > 0 || createData == false)
		   {
		      this->m_pddsSurface = highlevelGenSurface;
			  m_tempSurface = tempSurface;
              if(m_dwMipMapCount > 1)
                 GenerateMipMaps(m_pddsSurface,m_dwMipMapCount);//generate mip maps
              return TRUE;
		   }

		   if(m_rawMapData)
		      delete [] m_rawMapData;
		      m_rawMapData = 0;
	       m_rawMapDataLength = 0;

		   DWORD retMipMaps = 1;
		   BOOL res =  GetBaseDataFromSurface(&m_rawMapDataLength,//DWORD * length, 
						                      &m_rawMapData,//byte ** surfaceBytes,
						                      &m_widthBasis,//DWORD * width,
						                      &m_hieghtBasis,//DWORD * hieght,
						                      &retMipMaps,//DWORD * mipCount,
						                      &m_format,//D3DFORMAT * d3dFormat,
						                      highlevelGenSurface,//IDirect3DTexture9 * texture
											  TRUE);

		   if(highlevelGenSurface)
		   {
              highlevelGenSurface->Release();
			  highlevelGenSurface = 0;
		   }
return res;
}
//-----------------------------------------------------------------------------
BOOL CTextureEX::LoadIt(CString fileName,
						int mipLevels,
						LPDIRECT3DDEVICE9 pd3dDevice,
						int tempSurface)
{
	       m_fileType = 1;
		   
           IDirect3DTexture9 * highlevelGenSurface = NULL;
		   D3DXIMAGE_INFO imageInfo;

           if( FAILED(D3DXCreateTextureFromFileEx( pd3dDevice, fileName, 
                      D3DX_DEFAULT_NONPOW2, D3DX_DEFAULT_NONPOW2, 1/*D3DX_DEFAULT*/, 0, D3DFMT_UNKNOWN,//D3DFMT_A8R8G8B8, 
                      D3DPOOL_MANAGED,
					  D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR, 
                      D3DX_FILTER_NONE/*D3DX_FILTER_TRIANGLE|D3DX_FILTER_MIRROR*/,
					  0, &imageInfo, NULL, &highlevelGenSurface )))
		   {
				return FALSE;
		   }

		   m_format           = imageInfo.Format;
           m_restoreType      = 1;
           m_widthBasis       = imageInfo.Width;
           m_hieghtBasis      = imageInfo.Height;
		   m_depth            = imageInfo.Depth;
	       m_dwMipMapCount    = mipLevels;
		   m_fileName         = fileName;


		   D3DSURFACE_DESC testDesc;
		   highlevelGenSurface->GetLevelDesc(0,&testDesc);

		   CString temp = GetFormatName();
		   if(temp.CompareNoCase("Unknown")==0)
		   {
			   m_format = testDesc.Format;
			   temp = GetFormatName();
		   }
		   if(temp.CompareNoCase("Unknown")==0)
		   {
			   AfxMessageBox("Format not supported on file:"+fileName);
              return FALSE;
		   }

	       
		   if(tempSurface > 0)
		   {
		      this->m_pddsSurface = highlevelGenSurface;
			      m_tempSurface = tempSurface;
                  return TRUE;
		   }

		   if(m_rawMapData)
		      delete [] m_rawMapData;
		      m_rawMapData = 0;
	       m_rawMapDataLength = 0;

		   DWORD retMipMaps = 1;
		   BOOL res =  GetBaseDataFromSurface(&m_rawMapDataLength,//DWORD * length, 
						                      &m_rawMapData,//byte ** surfaceBytes,
						                      &m_widthBasis,//DWORD * width,
						                      &m_hieghtBasis,//DWORD * hieght,
						                      &retMipMaps,//DWORD * mipCount,
						                      &m_format,//D3DFORMAT * d3dFormat,
						                      highlevelGenSurface,//IDirect3DTexture9 * texture
											  TRUE);

		   //this->m_pddsSurface = highlevelGenSurface;
		   if(highlevelGenSurface != NULL)
		   {
              highlevelGenSurface->Release();
			  highlevelGenSurface = 0;
		   }
return res;
}
//-----------------------------------------------------------------------------
BOOL CTextureEX::CopyDataLiveDataToBaseData(int optimizedType)
{
    if(!m_pddsSurface)
		return FALSE;

    if(m_rawMapData)
	   delete [] m_rawMapData;
	   m_rawMapData = 0;

	DWORD retMipMaps = 1;
		   BOOL res =  GetBaseDataFromSurface(&m_rawMapDataLength,//DWORD * length, 
						                      &m_rawMapData,//byte ** surfaceBytes,
						                      &m_widthBasis,//DWORD * width,
						                      &m_hieghtBasis,//DWORD * hieght,
						                      &retMipMaps,//DWORD * mipCount,
						                      &m_format,//D3DFORMAT * d3dFormat,
						                      m_pddsSurface,//IDirect3DTexture9 * texture
											  optimizedType
											  );
	   return res;
}
//-----------------------------------------------------------------------------
BOOL CTextureEX::GetBaseDataFromSurface(DWORD * length, 
										byte ** surfaceBytes,
										DWORD * width,
										DWORD * hieght,
										DWORD * mipCount,
										D3DFORMAT * d3dFormat,
										IDirect3DSurface9 * currentMipSurface)
{
	D3DXIMAGE_FILEFORMAT selectedFormat = D3DXIFF_DDS;//D3DXIFF_JPG
	
	LPD3DXBUFFER dxBuffer               = NULL;
    HRESULT err =					    D3DXSaveSurfaceToFileInMemory(&dxBuffer,
                                                                      selectedFormat,
                                                                      currentMipSurface,//LPDIRECT3DSURFACE9 pSrcSurface,
                                                                      NULL,//const PALETTEENTRY *pSrcPalette,
																      NULL);//const RECT *pSrcRect
    if(err != 0)
	   return FALSE;

	D3DSURFACE_DESC sufDesc;
    currentMipSurface->GetDesc(&sufDesc);
	*width = sufDesc.Width;
	*hieght = sufDesc.Height;
	*d3dFormat = sufDesc.Format;					


	*length = dxBuffer->GetBufferSize();
	if(*length > 0)
	{
       byte * bufferSz = new byte[*length];
       byte * refMem = (byte*)dxBuffer->GetBufferPointer();
	   CopyMemory(bufferSz,refMem,*length);
	   *surfaceBytes = bufferSz;
	   if(dxBuffer)
	      dxBuffer->Release();
	      dxBuffer = 0;
	}

	return TRUE;
}
//-----------------------------------------------------------------------------
BOOL CTextureEX::GetBaseDataFromSurface(DWORD * length, 
										byte ** surfaceBytes,
										DWORD * width,
										DWORD * hieght,
										DWORD * mipCount,
										D3DFORMAT * d3dFormat,
										IDirect3DSurface9 * currentMipSurface,
										BOOL & optimized)
{
	D3DXIMAGE_FILEFORMAT selectedFormat = D3DXIFF_DDS;//D3DXIFF_JPG
	if(optimized)
	{
	   selectedFormat = D3DXIFF_DDS;//D3DXIFF_JPG
       switch( *d3dFormat )
       {//sw
		   case D3DFMT_DXT5:
                selectedFormat = D3DXIFF_DDS;//Native     
           break;
		   case D3DFMT_V8U8:
                selectedFormat = D3DXIFF_DDS;//Native
           break;
		   case D3DFMT_A1R5G5B5:
                selectedFormat = D3DXIFF_DDS;//Native
           break;
		   case D3DFMT_A8R8G8B8:
                selectedFormat = D3DXIFF_DDS;//Native
           break;
	   }
	}
	
	LPD3DXBUFFER dxBuffer               = NULL;
    HRESULT err =					    D3DXSaveSurfaceToFileInMemory(&dxBuffer,
                                                                      selectedFormat,
                                                                      currentMipSurface,//LPDIRECT3DSURFACE9 pSrcSurface,
                                                                      NULL,//const PALETTEENTRY *pSrcPalette,
																      NULL);//const RECT *pSrcRect
    if(err != 0)
	   return FALSE;

	D3DSURFACE_DESC sufDesc;
    currentMipSurface->GetDesc(&sufDesc);
	*width = sufDesc.Width;
	*hieght = sufDesc.Height;
	*d3dFormat = sufDesc.Format;					


	*length = dxBuffer->GetBufferSize();
	if(*length > 0)
	{
       byte * bufferSz = new byte[*length];
       byte * refMem = (byte*)dxBuffer->GetBufferPointer();
	   CopyMemory(bufferSz,refMem,*length);
	   *surfaceBytes = bufferSz;
	   if(dxBuffer)
	      dxBuffer->Release();
	      dxBuffer = 0;
	}

	return TRUE;
}
//-----------------------------------------------------------------------------
BOOL CTextureEX::GetBaseDataFromSurface(DWORD * length, 
						                byte ** surfaceBytes,
						                DWORD * width,
						                DWORD * hieght,
						                DWORD * mipCount,
						                D3DFORMAT * d3dFormat,
						                IDirect3DTexture9 * texture)
{
    IDirect3DSurface9 * currentMipSurface = NULL;
    texture->GetSurfaceLevel(0,&currentMipSurface);
	*mipCount = texture->GetLevelCount();

	BOOL result =  GetBaseDataFromSurface(length, 
								          surfaceBytes,
                                          width,
										  hieght,
										  mipCount,
										  d3dFormat,
								          currentMipSurface);

	if(currentMipSurface)
	   currentMipSurface->Release();
	   currentMipSurface = 0;

    return result;
}
//-----------------------------------------------------------------------------
BOOL CTextureEX::GetBaseDataFromSurface(DWORD * length, 
						                byte ** surfaceBytes,
						                DWORD * width,
						                DWORD * hieght,
						                DWORD * mipCount,
						                D3DFORMAT * d3dFormat,
						                IDirect3DTexture9 * texture,
										BOOL optimized)
{
    IDirect3DSurface9 * currentMipSurface = NULL;
    texture->GetSurfaceLevel(0,&currentMipSurface);
	*mipCount = texture->GetLevelCount();

	BOOL result =  GetBaseDataFromSurface(length, 
								          surfaceBytes,
                                          width,
										  hieght,
										  mipCount,
										  d3dFormat,
								          currentMipSurface,
										  optimized);

	if(currentMipSurface)
	   currentMipSurface->Release();
	   currentMipSurface = 0;

    return result;
}
//-----------------------------------------------------------------------------
BOOL CTextureEX::GenerateSurfaceFromBaseData(DWORD & length, 
										     byte * surfaceBytes,
											 IDirect3DSurface9 ** surface)
{

	IDirect3DSurface9 * currentMipSurface = NULL;

    HRESULT err = D3DXLoadSurfaceFromFileInMemory( currentMipSurface,//LPDIRECT3DSURFACE9 pDestSurface,
                                                   NULL,//CONST PALETTEENTRY *pDestPalette,
                                                   NULL,//CONST RECT *pDestRect,
                                                   surfaceBytes,//LPCVOID pSrcData,
                                                   length,//UINT SrcData,
                                                   NULL,//CONST RECT *pSrcRect,
                                                   D3DX_DEFAULT,//DWORD Filter,
                                                   0,//D3DCOLOR ColorKey,
                                                   NULL//D3DXIMAGE_INFO *pSrcInfo
												   );



	if(err != 0)
	   return FALSE;
	   
    *surface = currentMipSurface;
    return TRUE;
}

BOOL CTextureEX::IsTextureFormatOk( D3DFORMAT TextureFormat, 
								    D3DFORMAT AdapterFormat,
									IDirect3D9 * d3d9Ptr,
									D3DFORMAT checkFormat) 
{
    HRESULT hr = d3d9Ptr->CheckDeviceFormat( D3DADAPTER_DEFAULT,
                                             D3DDEVTYPE_HAL,
                                             AdapterFormat,
                                             0,
                                             D3DRTYPE_TEXTURE,
                                             checkFormat);

    return SUCCEEDED( hr );
}


//-----------------------------------------------------------------------------
BOOL CTextureEX::GenerateSurfaceFromBaseData(IDirect3DDevice9 * pd3dDevice,
	                                         DWORD & length, 
										     byte * surfaceBytes,
											 DWORD & width,
											 DWORD & hieght,
											 DWORD & mipCount,
											 D3DFORMAT & format,
											 IDirect3DTexture9 ** surface,
											 D3DCAPS9 & pCaps)
{

	IDirect3DSurface9 * currentMipSurface = NULL;
	IDirect3DTexture9 * currentTexture = NULL;
	HRESULT err = 0;
		
	 if(m_usage == 0)
	 {//defaul usage
	 err =        pd3dDevice->CreateTexture(width,//pDesc.Width,
		                                    hieght,//pDesc.Height,
                                            mipCount,//m_dwMipMapCount,
                                            0,//DWORD Usage
                                            format,//D3DFORMAT Format,
                                            D3DPOOL_MANAGED,//D3DPOOL_DEFAULT,//D3DPOOL Pool,
                                            &currentTexture,
											NULL);
	 if(err != 0)
	    err = pd3dDevice->CreateTexture(width,//pDesc.Width,
										hieght,//pDesc.Height,
										mipCount,//m_dwMipMapCount,
										0,//DWORD Usage,
										D3DFMT_A1R5G5B5,//D3DFORMAT Format,
										D3DPOOL_MANAGED,//D3DPOOL_DEFAULT,//D3DPOOL Pool,
										&currentTexture,
										NULL);
	 }//end defaul usage
	 else
	 {//special usage
     err =        pd3dDevice->CreateTexture(width,//pDesc.Width,
		                                    hieght,//pDesc.Height,
                                            mipCount,//m_dwMipMapCount,
                                            D3DUSAGE_RENDERTARGET,//DWORD Usage
                                            format,//D3DFORMAT Format,
                                            D3DPOOL_DEFAULT,//D3DPOOL_DEFAULT,//D3DPOOL Pool,
                                            &currentTexture,
											NULL);


	 }//end special usage

	//D3DFMT_A1R5G5B5

	if(err != 0)
	   return FALSE;

	err = currentTexture->GetSurfaceLevel(0,&currentMipSurface);

	if(err != 0)
	   return FALSE;

	if(m_usage == 0)
    err = D3DXLoadSurfaceFromFileInMemory(  currentMipSurface,//LPDIRECT3DSURFACE9 pDestSurface,
                                            NULL,//CONST PALETTEENTRY *pDestPalette,
                                            NULL,//CONST RECT *pDestRect,
                                            surfaceBytes,//LPCVOID pSrcData,
                                            length,//UINT SrcData,
                                            NULL,//CONST RECT *pSrcRect,
                                            D3DX_DEFAULT,//DWORD Filter,
                                            0,//D3DCOLOR ColorKey,
                                            NULL//D3DXIMAGE_INFO *pSrcInfo
											);
	if(err != 0)
	{
	   if(currentMipSurface)
	      currentMipSurface->Release();
	      currentMipSurface = 0;
	   return FALSE;
	}

	if(currentMipSurface)
	   currentMipSurface->Release();
	   currentMipSurface = 0;

    *surface = currentTexture;
    return TRUE;
}
//-----------------------------------------------------------------------------
//Name: MipMap_Restore()
//Desc: Creates the device-dependant surface and texture for the mipmap
//-----------------------------------------------------------------------------
HRESULT CTextureEX::Restore( IDirect3DDevice9 * pd3dDevice,
							 BOOL blitOn,
							 int bump,
							 int bumpType,
							 int & maxTextureSize,
							 D3DCAPS9 & pCaps,
							 int & compressIt,
							 BOOL & processImage,
							 CString & advUrl,
							 CString & advUrlVending)
{
	if(m_restoreType == 0)
	   RestoreLegacy(pd3dDevice,blitOn,bump,bumpType,maxTextureSize,pCaps,compressIt);
	else if(m_restoreType == 1)
	{//native restore system
	   Invalidate();
	   D3DFORMAT curFormat = m_format;
	   if(compressIt == TRUE && this->m_compressedOvrd != 1)
	   {
	      curFormat = TransformToCompressedFormat(curFormat);
		  this->m_format = curFormat;
	   }
       if(m_usage == 1)
	   {
	      this->m_format = D3DFMT_A8R8G8B8;
          curFormat = D3DFMT_A8R8G8B8;
	   }
	   if(bumpType == 1)
	   {
		  this->m_format = D3DFMT_V8U8;
          curFormat = D3DFMT_V8U8;
		  processImage = TRUE;
	   }

	   DWORD curWidth = m_widthBasis;
	   DWORD curHieght = m_hieghtBasis;
	   DWORD curMipCount = m_dwMipMapCount;
	   if((int)m_widthBasis > maxTextureSize)
	      curWidth = maxTextureSize;
	   if((int)m_hieghtBasis > maxTextureSize)
	      curHieght = maxTextureSize;

	   curMipCount = CheckMaxMipCount(curWidth,curHieght,m_dwMipMapCount);
	   if(!GenerateSurfaceFromBaseData(pd3dDevice,m_rawMapDataLength,m_rawMapData,curWidth,curHieght,curMipCount,curFormat,&m_pddsSurface,pCaps))
          return 2;
	   
	   //dynamic image processing
	   if(processImage)
	    if(bump > 0)
	    {//enable bump mapping
	      D3DSURFACE_DESC    desc;
	      m_pddsSurface->GetLevelDesc(0,&desc);
	      if(bumpType == 0)
		  {//dot product bump map
	         GenerateNormalMap(pd3dDevice,bump);
		  }//end dot product bump map
		  else if(bumpType == 1)
		  {//enviorment
             ProcessToEnvWaveMap(pd3dDevice,bump);
		  }//end enviorment
	      else if(bumpType == 2)
	      {//Existing bump 2
             ProcessToEnvMap(pd3dDevice); 
	      }//end Existing bump 2
        }//end enable bump mapping
	    //end dynamic image processing

	   GenerateMipMaps(m_pddsSurface,curMipCount);//generate mip maps
	
	}//end native restore system
	else if(m_restoreType == 2)
	{
          LoadItFromURL(this->m_fileName,m_dwMipMapCount,pd3dDevice,FALSE,false,advUrl,advUrlVending);
	}

    return S_OK;
}
//----------------compatibility overload---------------------------------------
HRESULT CTextureEX::Restore( IDirect3DDevice9 * pd3dDevice,
							 BOOL blitOn,
							 int bump,
							 int bumpType,
							 int & maxTextureSize,
							 D3DCAPS9 & pCaps,
							 int & compressIt,
							 CString & advUrl,
							 CString & advUrlVending)
{
	BOOL temp=FALSE;
	return Restore(pd3dDevice,
				   blitOn,
				   bump,
				   bumpType,
				   maxTextureSize,
				   pCaps,
				   compressIt,
				   temp,
				   advUrl,
				   advUrlVending);
}
//-----------------------------------------------------------------------------
DWORD CTextureEX::CheckMaxMipCount(DWORD width, DWORD height, DWORD & currentMipCount)
{ 
	DWORD maxMipCount = 0;
    if(width > height)
	{
       while(width >= 2)
	   {
             width = width / 2;
			 maxMipCount++;
	   }
	   if(currentMipCount > maxMipCount)
	      return maxMipCount;
	}
	else
	{
       while(height >= 2)
	   {
             height = height / 2;
			 maxMipCount++;
	   }
	   if(currentMipCount > maxMipCount)
	      return maxMipCount;
	}
	return currentMipCount;
}
//Figures out best compression based on current format
D3DFORMAT CTextureEX::TransformToCompressedFormat(D3DFORMAT & format)
{
      switch( format )
      {//sw
           case D3DFMT_X8R8G8B8:
                return D3DFMT_DXT1;     
           break;
		   case D3DFMT_DXT1:
                return D3DFMT_DXT1;//Native     
           break;
		   case D3DFMT_V8U8:
                return D3DFMT_V8U8;//cannot work at DXT5 so far    
           break;
	  }
	  return D3DFMT_DXT5;
}
//-----------------------------------------------------------------------------
VOID CTextureEX::Invalidate()
{
	if(m_pddsSurface)
	   m_pddsSurface->Release();
	   m_pddsSurface=0;
}
//-----------------------------------------------------------------------------
HRESULT CTextureEX::ProcessToEnvWaveMap(LPDIRECT3DDEVICE9 pd3dDevice, int bump)
{
	D3DSURFACE_DESC    desc;
    D3DCAPS9           pCaps;
    m_pddsSurface->GetLevelDesc(0,&desc);
    pd3dDevice->GetDeviceCaps(&pCaps);
			 IDirect3DTexture9 * pHeightMap;
			 if(pCaps.TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAP)
             if(pd3dDevice->CreateTexture( desc.Width, desc.Height, m_dwMipMapCount, 0 ,D3DFMT_V8U8, D3DPOOL_MANAGED,&pHeightMap, NULL )==S_OK)
			 {//Create Texture
			    this->m_format = D3DFMT_V8U8;
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
		 return 0;
}//end enviorment
//-----------------------------------------------------------------------------
HRESULT CTextureEX::GenerateNormalMap(LPDIRECT3DDEVICE9 pd3dDevice,int bump)
{
	   D3DSURFACE_DESC    desc;
       D3DCAPS9           pCaps;
       m_pddsSurface->GetLevelDesc(0,&desc);
       pd3dDevice->GetDeviceCaps(&pCaps);
       IDirect3DTexture9 * pHeightMap;
	   if(pCaps.TextureOpCaps & D3DTEXOPCAPS_DOTPRODUCT3)
	   if(D3DXCreateTexture(pd3dDevice,desc.Width,desc.Height,m_dwMipMapCount,0,D3DFMT_A8R8G8B8,D3DPOOL_MANAGED, &pHeightMap)==S_OK)
	   {			 
			 D3DXComputeNormalMap(pHeightMap,m_pddsSurface,NULL,0,D3DX_CHANNEL_RED,(float)bump);
			 this->m_format = D3DFMT_A8R8G8B8;
	         m_pddsSurface->Release();
	         m_pddsSurface = pHeightMap;
	   }
	   return 0;
}
//-----------------------------------------------------------------------------
HRESULT CTextureEX::ProcessToEnvMap(LPDIRECT3DDEVICE9 pd3dDevice)
{//Existing bump 2
   D3DSURFACE_DESC    desc;
   D3DCAPS9           pCaps;
   m_pddsSurface->GetLevelDesc(0,&desc);
   pd3dDevice->GetDeviceCaps(&pCaps);


   if (pCaps.TextureOpCaps & D3DTEXOPCAPS_BUMPENVMAP)
   {
   }
   else
      return -1;//no env support
           
   LPDIRECT3DTEXTURE9 psBumpSrc = NULL;//m_pddsSurface;
   D3DLOCKED_RECT     d3dlr;
   IDirect3DTexture9 * m_psBumpMap;
   //Create the bumpmap's surface and texture objects
   if(pd3dDevice->CreateTexture( desc.Width, desc.Height, m_dwMipMapCount, 0, D3DFMT_V8U8, D3DPOOL_MANAGED, &m_psBumpMap, NULL )==S_OK )
   {//Create texture 
	  if( FAILED( pd3dDevice->CreateTexture( desc.Width, desc.Height, 1, 0,D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &psBumpSrc, NULL ) ) )
      {
          return -1;    
      }
	  this->m_format = D3DFMT_V8U8;
      IDirect3DSurface9 * currentMipSurface = NULL;
      psBumpSrc->GetSurfaceLevel(0,&currentMipSurface);
	  IDirect3DSurface9 * topSurface = NULL;
      m_pddsSurface->GetSurfaceLevel(0,&topSurface);
      D3DXLoadSurfaceFromSurface(currentMipSurface, NULL, NULL, topSurface, NULL, NULL, D3DX_FILTER_POINT , 0);

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
  
      //Fill the bits of the new texture surface with bits from
      //a private format.
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
         BYTE* pSrcB0; //addr of current pixel
         BYTE* pSrcB1; //addr of pixel below current pixel, wrapping to top if necessary
         BYTE* pSrcB2; //addr of pixel above current pixel, wrapping to bottom if necessary
         BYTE* pDstT;  //addr of dest pixel;
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
                     LONG v00; //Current pixel
                     LONG v01; //Pixel to the right of current pixel, wrapping to left edge if necessary
                     LONG vM1; //Pixel to the left of current pixel, wrapping to right edge if necessary
                     LONG v10; //Pixel one line below.
                     LONG v1M; //Pixel one line above.

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

                     LONG iDu = (vM1-v01); //The delta-u bump value
                     LONG iDv = (v1M-v10); //The delta-v bump value

                     //The luminance bump value (land masses are less shiny)
                     //WORD uL = ( v00>1 ) ? 63 : 127;
					 WORD uL = ( v00>1 ) ? 30 : 100;

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
                     //Move one pixel to the right (src is 32-bpp)
                     pSrcB0+=4;
                     pSrcB1+=4;
                     pSrcB2+=4;	
			 }//end primary loop
              //Move to the next line
              pSrcCurRow += dwSrcPitch;
              pDstCurRow += dwDstPitch;		 
	  }//end hight loop
      m_psBumpMap->UnlockRect(0);
      psBumpSrc->UnlockRect(0);
      psBumpSrc->Release();
	  psBumpSrc = 0;
	  m_pddsSurface->Release();
	  m_pddsSurface = m_psBumpMap;		   
    }
	return 0;
}
//-----------------------------------------------------------------------------
void CTextureEX::GenerateMipMaps(IDirect3DTexture9 * texture, DWORD & mipCount)
{
	   IDirect3DSurface9 * topSurface = NULL;
	   //IDirect3DSurface9 * lastSurface = NULL;
	   IDirect3DSurface9 * currentMipSurface = NULL;
	   //D3DXLoadSurfaceFromSurface(currentMipSurface, NULL, NULL, topSurface, NULL, NULL, D3DX_FILTER_POINT , 0);
	   texture->GetSurfaceLevel(0,&topSurface);
       DWORD loop=0;
	   
	   while(loop<mipCount)
	   {
           texture->GetSurfaceLevel(loop,&currentMipSurface);
           D3DXLoadSurfaceFromSurface(currentMipSurface, NULL, NULL, topSurface, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);
		   if(topSurface)topSurface->Release();topSurface=0;
	       topSurface = currentMipSurface;
		   loop++;
	   }
		  
	   if(topSurface)
	   {
		  topSurface->Release();
          topSurface = 0;
	   }
}
//-----------------------------------------------------------------------------
//Name: CopyRGBADataToSurface()
//Desc: Invalidates the current texture objects and rebuilds new ones
//      using the new device.
//-----------------------------------------------------------------------------
HRESULT CTextureEX::CopyRGBADataToSurface(CMapObj * mapData,
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
          
		 if(mapData->m_rgbData.m_tgaBBP == 16)
		 {//16 bit
            for(int y=0; y<m_baseHieght; y++ )
			{
                 WORD*  pDstData16 = (WORD*)pBytes;

                 for(int x=0; x<m_baseWidth; x++ )
				 {
                    DWORD dwPixel = mapData->m_rgbData.m_rgbaData16[y*m_baseWidth+x];

                        pDstData16[x] = (WORD)dwPixel;
				 }
                 pBytes += lPitch;
			}	
		 }//end 16 bit
         if(mapData->m_rgbData.m_tgaBBP == 32)
		 {//32 bit
            for( int y=0; y<m_baseHieght; y++ )
			{
                 DWORD* pDstData32 = (DWORD*)pBytes;

                 for( int x=0; x<m_baseWidth; x++ )
				 {
                    DWORD dwPixel = mapData->m_rgbData.m_rgbaData32[y*m_baseWidth+x];

						pDstData32[x] = (DWORD)dwPixel;
				 }
                 pBytes += lPitch;
			}	
		 }//end 32 bit
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
            
		 if(mapData->m_rgbData.m_tgaBBP == 16)
		 {//16 bit
            for( int y=0; y<createHeight; y++ )
			{
                 WORD*  pDstData16 = (WORD*)pBytes;

                 for( int x=0; x<createWidth; x++ )
				 {
                    DWORD dwPixel = mapData->m_rgbData.m_rgbaData16[((y*multiplier)*m_baseWidth+(x*multiplier))];
                 
					pDstData16[x] = (WORD)dwPixel;
				 }
                 pBytes += lPitch;
			}	
		 }//end 16 bit
		 if(mapData->m_rgbData.m_tgaBBP == 32)
		 {//32 bit
            for( int y=0; y<createHeight; y++ )
			{
                 DWORD* pDstData32 = (DWORD*)pBytes;

                 for( int x=0; x<createWidth; x++ )
				 {
                    DWORD dwPixel = mapData->m_rgbData.m_rgbaData32[((y*multiplier)*m_baseWidth+(x*multiplier))];

					pDstData32[x] = (DWORD)dwPixel;
				 }
                 pBytes += lPitch;
			}	
		 }//end 32 bit
	     surface->UnlockRect(0);
	  }
	 }//end must truncate this
	}//end primary map incode this
   
    return S_OK;
}

HRESULT CTextureEX::RestoreLegacy( IDirect3DDevice9 * pd3dDevice,
							       BOOL & blitOn,
							       int & bump,
							       int & bumpType,
							       int & maxTextureSize,
							       D3DCAPS9 & pCaps,
							       int & compressIt)
{
	//if(m_restoreType == 0)
	//Release any previously created objects
    Invalidate();
    DWORD textureCRWidth = m_widthBasis;
	DWORD textureCRHeight = m_hieghtBasis;

	BOOL problem = FALSE;

	if(textureCRWidth == 8 || textureCRWidth == 16 ||
	   textureCRWidth == 32 || textureCRWidth == 64 || 
	   textureCRWidth == 128 || textureCRWidth == 256 || 
	   textureCRWidth == 512 || textureCRWidth == 1024 || textureCRWidth == 2048)
	{

	}
	else
	   problem = TRUE;

	if(problem == TRUE)
	{
		return 1;
	}

	if((int)m_widthBasis > maxTextureSize)
	{
	   textureCRWidth = maxTextureSize;
	   if(maxTextureSize == 256)
	      m_dwMipMapCount = 8;
	   else if(maxTextureSize == 512)
	      m_dwMipMapCount = 9;
	   else
	      m_dwMipMapCount = 8;
	}
	if((int)m_hieghtBasis > maxTextureSize)
	   textureCRHeight = maxTextureSize;
    

	int trace = 1;

    POSITION posLoc = m_mapFileList->FindIndex(0);
	if(posLoc)
	{
       CMapObj * mapPtr = (CMapObj *)m_mapFileList->GetAt(posLoc);


	   HRESULT err = 0;

       if(mapPtr->m_rgbData.m_tgaBBP == 16)
		    err = pd3dDevice->CreateTexture(textureCRWidth,
		                                    textureCRHeight,
                                            m_dwMipMapCount,
                                            0,//DWORD Usage,
                                            D3DFMT_A1R5G5B5,//D3DFORMAT Format,
                                            D3DPOOL_MANAGED,//D3DPOOL_MANAGED,//D3DPOOL Pool,
                                            &m_pddsSurface,
											NULL);

	   if(mapPtr->m_rgbData.m_tgaBBP == 32)
		    err = pd3dDevice->CreateTexture(textureCRWidth,
		                                    textureCRHeight,
                                            m_dwMipMapCount,
                                            0,//DWORD Usage,
                                            D3DFMT_A8R8G8B8,//D3DFORMAT Format,
                                            D3DPOOL_MANAGED,//D3DPOOL_MANAGED,//D3DPOOL Pool,
                                            &m_pddsSurface,
											NULL);
	   if(err != 0)
	      return err;

	   IDirect3DTexture9 * pddsDest   = m_pddsSurface;
	   IDirect3DTexture9 * persistMap = NULL;

	   if(CopyRGBADataToSurface(mapPtr,
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
	         GenerateNormalMap(pd3dDevice,bump);
		  }//end dot product bump map
		  else if(bumpType == 1)
		  {//enviorment
             ProcessToEnvWaveMap(pd3dDevice,bump);
		  }//end enviorment
	      else if(bumpType == 2)
	      {//Existing bump 2
             ProcessToEnvMap(pd3dDevice); 
	      }//end Existing bump 2
       }//end enable bump mapping

       
	   IDirect3DSurface9 * topSurface = NULL;
	   //compress textures if specified
       if(compressIt)
	   if(bump < 1)
	   {//compress this
	      
		  IDirect3DTexture9 * compressedTexture;
          if(pd3dDevice->CreateTexture(textureCRWidth,
		                               textureCRHeight,
                                       m_dwMipMapCount,
                                       0,//DWORD Usage,
                                       D3DFMT_DXT5,//D3DFORMAT Format,
                                       D3DPOOL_MANAGED,//D3DPOOL_MANAGED,//D3DPOOL Pool,
                                       &compressedTexture,
									   NULL)==0)
		  {//succeeded compression
             IDirect3DSurface9 * compressedSurface = NULL;
             compressedTexture->GetSurfaceLevel(0,&compressedSurface);
	         m_pddsSurface->GetSurfaceLevel(0,&topSurface);

			 D3DXLoadSurfaceFromSurface(compressedSurface, NULL, NULL, topSurface, NULL, NULL, D3DX_FILTER_TRIANGLE, 0);
             if(topSurface)
			 {
			    topSurface->Release();
			    topSurface = 0;
			 }
			 if(compressedSurface)
			 {
			    compressedSurface->Release();
			    compressedSurface = 0;
			 }
			 m_pddsSurface->Release();
			 m_pddsSurface = 0;
			 m_pddsSurface = compressedTexture;
		  }//end succeeded compression
	   }//end compress this



       if(m_dwMipMapCount<2)
          return S_OK;

	   GenerateMipMaps(m_pddsSurface,m_dwMipMapCount);   
	}

    return S_OK;
}
//---------------------------------------//
short CTextureEX::EncodeFormat(D3DFORMAT & format)
{
      switch( format )
      {//sw
           case D3DFMT_X8R8G8B8:
                return 0;     
           break;
		   case D3DFMT_DXT1:
                return 1;//Native     
           break;
		   case D3DFMT_DXT2:
                return 2;//Native     
           break;
		   case D3DFMT_DXT3:
                return 3;//Native     
           break;
		   case D3DFMT_DXT4:
                return 4;//Native     
           break;
		   case D3DFMT_DXT5:
                return 5;//native    
           break;
		   case D3DFMT_X1R5G5B5:
                return 6;     
           break;
		   case D3DFMT_R5G6B5:
                return 7;     
           break;
		   case D3DFMT_V8U8:
                return 8;     
           break;
		   case D3DFMT_A1R5G5B5:
                return 9;     
           break;
		   case D3DFMT_A8B8G8R8:
                return 10;     
           break;
		   case D3DFMT_X8B8G8R8:
                return 11;     
           break;
		   case D3DFMT_A8R8G8B8:
			    return 12;
           break;
		   case D3DFMT_R8G8B8:
			    return 14;
           break;
		   case D3DFMT_L8:
			    return 15;
           break;
	  }
	  return 5;
}
//---------------------------------------//
CString CTextureEX::GetFormatName()
{
	  
      switch( this->m_format )
      {//sw
           case D3DFMT_X8R8G8B8:
                return "D3DFMT_X8R8G8B8";     
           break;
		   case D3DFMT_DXT1:
                return "D3DFMT_DXT1";//Native     
           break;
		   case D3DFMT_DXT2:
                return "D3DFMT_DXT2";//Native     
           break;
		   case D3DFMT_DXT3:
                return "D3DFMT_DXT3";//Native     
           break;
		   case D3DFMT_DXT4:
                return "D3DFMT_DXT4";//Native     
           break;
		   case D3DFMT_DXT5:
                return "D3DFMT_DXT5";//native    
           break;
		   case D3DFMT_X1R5G5B5:
                return "D3DFMT_X1R5G5B5";     
           break;
		   case D3DFMT_R5G6B5:
                return "D3DFMT_R5G6B5";     
           break;
		   case D3DFMT_V8U8:
                return "D3DFMT_V8U8";     
           break;
		   case D3DFMT_A1R5G5B5:
                return "D3DFMT_A1R5G5B5";     
           break;
		   case D3DFMT_A8B8G8R8:
                return "D3DFMT_A8B8G8R8";     
           break;
		   case D3DFMT_X8B8G8R8:
                return "D3DFMT_X8B8G8R8";     
           break;
		   case D3DFMT_A8R8G8B8:
			    return "D3DFMT_A8R8G8B8";
           break;
		   case D3DFMT_R8G8B8:
			    return "D3DFMT_R8G8B8";
           break;
		   case D3DFMT_L8:
			    return "D3DFMT_L8";
           break;
	  }
	  return "Unknown";
}
//---------------------------------------//
D3DFORMAT CTextureEX::DecodeFormat(short & id)
{
      switch( id )
      {//sw
           case 0:
                return D3DFMT_X8R8G8B8;     
           break;
		   case 1:
                return D3DFMT_DXT1;//Native     
           break;
		   case 2:
                return D3DFMT_DXT2;//Native     
           break;
		   case 3:
                return D3DFMT_DXT3;//Native     
           break;
		   case 4:
                return D3DFMT_DXT4;//Native     
           break;
		   case 5:
                return D3DFMT_DXT5;//native    
           break;
		   case 6:
                return D3DFMT_X1R5G5B5;     
           break;
		   case 7:
                return D3DFMT_R5G6B5;     
           break;
		   case 8:
                return D3DFMT_V8U8;     
           break;
		   case 9:
                return D3DFMT_A1R5G5B5;     
           break;
		   case 10:
                return D3DFMT_A8B8G8R8;     
           break;
		   case 11:
                return D3DFMT_X8B8G8R8;     
           break;
		   case 12:
			    return D3DFMT_A8R8G8B8;
           break;
		   case 14:
			    return D3DFMT_R8G8B8;
           break;
		   case 15:
			    return D3DFMT_L8;
           break;
	  }
	  return D3DFMT_DXT5;
}
//---------------------------------------//
void CTextureEX::Serialize( CArchive& ar )
{
DWORD reserved = 0;
CObject::Serialize( ar );
	if(ar.IsStoring() ){

       ar.SetObjectSchema(4);
       ASSERT(ar.GetObjectSchema() == 4);

	   //AfxMessageBox(m_fileName);
	   WriteDebugFile("lastTexture.txt",m_fileName);
       //m_restoreType = 1;

	   ar << m_widthBasis
		  << m_hieghtBasis
		  << m_fileType
		  << m_dwMipMapCount;

	   ar << m_depth
		  << m_fileName
		  << EncodeFormat(m_format)
		  << m_rawMapDataLength
		  << m_restoreType
		  << m_compressedOvrd;

	   ar << m_usage
		  << reserved
		  << reserved;

	   if(m_restoreType == 0 || m_restoreType == 1)
       if(m_rawMapDataLength)
	      for(DWORD loop=0;loop<m_rawMapDataLength;loop++)
              ar << m_rawMapData[loop];
	}
	else
	{
       int version = ar.GetObjectSchema();
	   m_depth              = 32;
	   m_format             = D3DFMT_UNKNOWN;
	   m_rawMapDataLength   = 0;
	   m_restoreType        = 0;
	   m_rawMapData         = NULL;
	   short d3dFormatID    = 0;
	   m_compressedOvrd     = 0;
	   m_usage              = 0;
    
       //initialize
         m_pddsSurface = NULL;
		 m_tempSurface = FALSE;

	   if(version < 2)
	   {
          ar >> m_widthBasis
		     >> m_hieghtBasis
		     >> m_fileType
		     >> m_dwMipMapCount
		     >> m_mapFileList; 
	   }
	   else if(version == 2)
	   {
          ar >> m_widthBasis
		     >> m_hieghtBasis
		     >> m_fileType
		     >> m_dwMipMapCount; 

		  ar >> m_depth
		     >> m_fileName
		     >> d3dFormatID
		     >> m_rawMapDataLength
		     >> m_restoreType;

		  m_format = DecodeFormat(d3dFormatID);

	   if(m_restoreType == 0 || m_restoreType == 1)
       if(m_rawMapDataLength)
	      m_rawMapData = new byte[m_rawMapDataLength];
	      for(DWORD loop=0;loop<m_rawMapDataLength;loop++)
              ar >> m_rawMapData[loop];

	   }
	   else if(version == 3)
	   {
          ar >> m_widthBasis
		     >> m_hieghtBasis
		     >> m_fileType
		     >> m_dwMipMapCount; 

		  ar >> m_depth
		     >> m_fileName
		     >> d3dFormatID
		     >> m_rawMapDataLength
		     >> m_restoreType
			 >> m_compressedOvrd;

		  m_format = DecodeFormat(d3dFormatID);

	   if(m_restoreType == 0 || m_restoreType == 1)
       if(m_rawMapDataLength)
	      m_rawMapData = new byte[m_rawMapDataLength];
	      for(DWORD loop=0;loop<m_rawMapDataLength;loop++)
              ar >> m_rawMapData[loop];

	   }
       else if(version == 4)
	   {
          ar >> m_widthBasis
		     >> m_hieghtBasis
		     >> m_fileType
		     >> m_dwMipMapCount; 

		  ar >> m_depth
		     >> m_fileName
		     >> d3dFormatID
		     >> m_rawMapDataLength
		     >> m_restoreType
			 >> m_compressedOvrd;

		  ar >> m_usage
			 >> reserved
			 >> reserved;

		  m_format = DecodeFormat(d3dFormatID);

	   if(m_restoreType == 0 || m_restoreType == 1)
       if(m_rawMapDataLength)
	      m_rawMapData = new byte[m_rawMapDataLength];
	      for(DWORD loop=0;loop<m_rawMapDataLength;loop++)
              ar >> m_rawMapData[loop];

	   }

	   
  
	}
}

//---------map lists(legacy)----//
CMapObj::CMapObj(CString filename)
{
    m_fileName = filename;
	m_rgbData.m_rgbaData16 = NULL;
	m_rgbData.m_rgbaData32 = NULL;

	m_rgbData.m_tgaBBP = 0; 
	m_rgbData.m_tgaHieght = 0;
	m_rgbData.m_tgaWidth = 0;
}
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
             
		     if(m_rgbData.m_tgaBBP == 16)
		     for(int count=0;count < (m_rgbData.m_tgaWidth*m_rgbData.m_tgaHieght);count++)
		         ar << m_rgbData.m_rgbaData16[count];

			 if(m_rgbData.m_tgaBBP == 32)
		     for(int count=0;count < (m_rgbData.m_tgaWidth*m_rgbData.m_tgaHieght);count++)
		         ar << m_rgbData.m_rgbaData32[count];
	   }//end tga type
	}
	else
	{
       m_rgbData.m_rgbaData16 = NULL;
	   m_rgbData.m_rgbaData32 = NULL;
  
       ar >> m_fileName
          >> m_mapFileType;
       if(m_mapFileType == 1){//tga type
       ar >> m_rgbData.m_tgaWidth
          >> m_rgbData.m_tgaHieght
	      >> m_rgbData.m_tgaBBP;
	    
	      long countObj = m_rgbData.m_tgaWidth*m_rgbData.m_tgaHieght;
	      
		  if(countObj > 0)
		  {
              if(m_rgbData.m_tgaBBP == 32)
			  {
			     m_rgbData.m_tgaBBP = 32;
			     m_rgbData.m_rgbaData32 = new DWORD[(m_rgbData.m_tgaWidth*m_rgbData.m_tgaHieght)];
			  }
			  else
			  {
			     m_rgbData.m_tgaBBP = 16;
			     m_rgbData.m_rgbaData16 = new WORD[(m_rgbData.m_tgaWidth*m_rgbData.m_tgaHieght)]; 
			  }
		  }
		 
          if(m_rgbData.m_rgbaData16)
          for(int count=0;count < countObj;count++)
		      ar >> m_rgbData.m_rgbaData16[count];

		  if(m_rgbData.m_rgbaData32)
          for(int count=0;count < countObj;count++)
		      ar >> m_rgbData.m_rgbaData32[count];
       
	   
	   }//end tga type	
	}
}
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
		
        FILE* file = NULL;
		fopen_s(&file, mpPtr->m_fileName, "rb" );
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
//-----------------------------------------------------------------//
void WriteDebugFile(CString filename,CString debugString)
{
	CFileException exc;
    CFile fl;
	CArchive the_out_Archive(&fl,CArchive::store);
    BOOL res = fl.Open(filename,CFile::modeCreate | CFile::modeWrite, &exc);
       the_out_Archive.WriteString( debugString );  
	   the_out_Archive.WriteString( "\n");  
	
	 the_out_Archive.Close();
     fl.Close(); 
}
//Serial Macros
IMPLEMENT_SERIAL(CTextureEX, CObject, VERSIONABLE_SCHEMA|4)
IMPLEMENT_SERIAL(CMapObj, CObject, 0)
IMPLEMENT_SERIAL(CMapObjList, CObList, 0)
