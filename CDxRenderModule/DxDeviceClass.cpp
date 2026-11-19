#include "StdAfx.h"
#include "atlbase.h"
#include "atlwin.h" 
#include "STDIO.H"
#include <fstream>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <mmsystem.h>//timeBeginPeriod/timeEndPeriod (winmm) for frame pacing

#include ".\dxdeviceclass.h"

//----------------------------------------------------------------------------
// D3D9On12: run this DirectX 9 engine on DirectX 12 through Windows' built-in
// translation layer (d3d9.dll on Windows 10/11). Same rendering, modern driver
// path. Looked up at run time so the build needs no newer SDK, and it falls
// back to plain Direct3D 9 if the layer is missing or fails.
// To force plain DX9: put an empty file named "dx9.only" next to the exe.
//----------------------------------------------------------------------------
typedef struct _EA_D3D9ON12_ARGS
{
	BOOL       Enable9On12;
	IUnknown * pD3D12Device;
	IUnknown * ppD3D12Queues[2];
	UINT       NumQueues;
	UINT       NodeMask;
} EA_D3D9ON12_ARGS;
typedef IDirect3D9 * (WINAPI * PFN_Direct3DCreate9On12)(UINT, EA_D3D9ON12_ARGS *, UINT);

static IDirect3D9 * CreateD3D9PreferOn12(const char ** which)
{
	char exePath[MAX_PATH] = {0};
	GetModuleFileNameA(NULL, exePath, MAX_PATH);
	char * slash = strrchr(exePath, '\\');
	if(slash) { strcpy_s(slash + 1, MAX_PATH - (slash + 1 - exePath), "dx9.only"); }
	BOOL forceDx9 = (GetFileAttributesA(exePath) != INVALID_FILE_ATTRIBUTES);

	if(!forceDx9)
	{
		HMODULE d3d9 = GetModuleHandleA("d3d9.dll");
		if(!d3d9) d3d9 = LoadLibraryA("d3d9.dll");
		PFN_Direct3DCreate9On12 create9On12 = d3d9 ? (PFN_Direct3DCreate9On12)GetProcAddress(d3d9, "Direct3DCreate9On12") : NULL;
		if(create9On12)
		{
			EA_D3D9ON12_ARGS args;
			ZeroMemory(&args, sizeof(args));
			args.Enable9On12 = TRUE;
			IDirect3D9 * d3d = create9On12(D3D_SDK_VERSION, &args, 1);
			if(d3d)
			{
				*which = "Renderer: Direct3D 9 on DirectX 12 (D3D9On12)";
				return d3d;
			}
		}
		*which = "Renderer: D3D9On12 not available - using plain Direct3D 9";
	}
	else
		*which = "Renderer: dx9.only found - using plain Direct3D 9";

	return Direct3DCreate9(D3D_SDK_VERSION);
}

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
/*struct SHADOWVERTEX
{
    D3DXVECTOR4 p;
    D3DCOLOR    color;
};*/

//-----------------------------------------------------------------------
DxDeviceClass::DxDeviceClass(int maxTextureSize,
							 int defaultDispWidth,
							 int defaultDispHeight,
							 int defaultDispDepth,
							 void ** devicePublish)
{
	m_shaderSupport = FALSE;
	m_deviceBehaviorFlags       = 0;
	m_vsyncMode                 = 1;//default: vsync at monitor rate (no uncapped 2000+ FPS)
	m_frameLimitHz              = 0;
	m_aaMode                    = 2;//default: 4x MSAA when the adapter supports it (auto step-down otherwise)
	m_anisoMode                 = 4;//default: 16x anisotropic filtering when supported (pushed to the state manager)
	m_limiterLast.QuadPart      = 0;
	m_limiterFreq.QuadPart      = 1000;
	QueryPerformanceFrequency(&m_limiterFreq);
	timeBeginPeriod(1);//1 ms Sleep granularity for the frame pacer (winmm)
	m_publishDevice = (LPDIRECT3DDEVICE9*)devicePublish;
	InitializeCriticalSection(&m_mouseManagerSection);
	WriteDebugFile("Open Device",TRUE);
    m_supportedStencilBuffer    = TRUE;
	m_currentWidth				= -1;
	m_currentHeight             = -1;
	m_currentDepth              = -1;
    m_pD3D                      = NULL;
    m_pd3dDevice                = NULL;
	m_availTextureMem           = 128;
	m_maxTextureSize            = maxTextureSize;
	m_shadowSquareVB            = NULL;
	m_renderTargetSquareVB      = NULL;
	m_defaultWidth              = defaultDispWidth;
	m_defaultHeight             = defaultDispHeight;
	m_defaultDepth              = defaultDispDepth;

    m_initialCreateRect.left    = 0;
	m_initialCreateRect.top     = 0;
	m_initialCreateRect.right   = defaultDispWidth;
	m_initialCreateRect.bottom  = defaultDispHeight;

	m_loggedDisplayModes        = 0;
	m_renderingPaused           = FALSE;
	m_fullScreenMode            = FALSE;
	m_aspectBasis               = 950;
	m_aspectScalarMax           = .3f;
	m_hwnd                      = NULL;
	m_restoreStatesPls          = FALSE;
	m_currentStartWidth         = 0;
	m_currentStartHeight        = 0;
	m_renderTargetAvailable     = 0;
	m_showMouse                 = 1;
	m_windowHandleOrig          = NULL;
	m_windowHandleRef           = NULL;
	m_topWindowHandle           = NULL;
	m_mouseManagerEnabled       = FALSE;
	m_supportsGammaControl      = TRUE;
	m_mousePendingCommand       = 0;
    m_toggleTimer               = GetTickCount();
	ZeroMemory(&m_viewPortTemp,sizeof(D3DVIEWPORT9));
	m_viewPortTemp.MaxZ			= 1.0f;

	

	for(int i=0;i<RT_COUNT;i++)
	{
	    m_renderTargets[i] = NULL;
        m_renderTargetsSurfaces[i] = NULL;
	}

	ZeroMemory( &m_d3dpp, sizeof(D3DPRESENT_PARAMETERS) );
}
//-----------------------------------------------------------------------
DxDeviceClass::~DxDeviceClass(void)
{
    if(m_pD3D)
	   m_pD3D->Release();
	   m_pD3D = 0;
    if(m_pd3dDevice)
	   m_pd3dDevice->Release();
	   m_pd3dDevice = 0;
    if(m_shadowSquareVB)
	   m_shadowSquareVB->Release();
	   m_shadowSquareVB = 0;
    if(m_renderTargetSquareVB)
	{
       m_renderTargetSquareVB->Release();
	   m_renderTargetSquareVB = 0;
	}
	for(int dLoop=0;dLoop<RT_COUNT;dLoop++)
	{
	      if(m_renderTargets[dLoop])
		     m_renderTargets[dLoop]->Release();
		     m_renderTargets[dLoop] = 0;

		  if(m_renderTargetsSurfaces[dLoop])
		     m_renderTargetsSurfaces[dLoop]->Release();
		     m_renderTargetsSurfaces[dLoop] = 0;
	}

	timeEndPeriod(1);//match timeBeginPeriod(1) from the constructor
	DeleteCriticalSection(&m_mouseManagerSection);
}

void DxDeviceClass::SetGamma(WORD r, WORD g, WORD b)
{
    if(m_pd3dDevice)
	{
       D3DGAMMARAMP rampX;
	   /*rampX.red		= r;
	   rampX.green		= g;
	   rampX.blue		= b;*/

	   m_pd3dDevice->SetGammaRamp(0,D3DSGR_NO_CALIBRATION,&rampX);
	}
}

void DxDeviceClass::EnableManagerCallback()
{
     m_mouseManagerEnabled = TRUE;
}

void DxDeviceClass::DisableManagerCallback()
{
	EnterCriticalSection(&m_mouseManagerSection);
     m_mouseManagerEnabled = FALSE;
	LeaveCriticalSection(&m_mouseManagerSection);
}

void DxDeviceClass::DisableMouse()
{
	EnterCriticalSection(&m_mouseManagerSection);
     if(!m_mouseManagerEnabled)
	 {
	    LeaveCriticalSection(&m_mouseManagerSection);
	    return;
	 }

	 m_mousePendingCommand = 2;

	 /*if(m_fullScreenMode == FALSE)
     if(m_showMouse == TRUE)
	 {
	    //::ShowCursor(FALSE);
        //m_showMouse = FALSE;
		 m_mousePendingCommand = 2;
     }*/
    LeaveCriticalSection(&m_mouseManagerSection);
}

void DxDeviceClass::ToggleMouse()
{
	 if(!m_mouseManagerEnabled)
	    return;

     if(m_fullScreenMode == TRUE)
	    return;

	 if(( GetTickCount()-m_toggleTimer ) < 1000)
          return;

     if(m_showMouse == TRUE)
	 {
		m_mousePendingCommand = 2;
     }
	 else
	 {
		m_mousePendingCommand = 1;
	 }
	 m_toggleTimer = GetTickCount();
}

void DxDeviceClass::EnableMouse()
{
	 EnterCriticalSection(&m_mouseManagerSection);
     if(!m_mouseManagerEnabled)
	 {
	    LeaveCriticalSection(&m_mouseManagerSection);
	    return;
	 }

	 m_mousePendingCommand = 1;

	 /*if(m_fullScreenMode == FALSE)
     if(m_showMouse == FALSE)
	 {
	    //::ShowCursor(TRUE);
        //m_showMouse = TRUE;
		m_mousePendingCommand = 1;
     }*/
	 LeaveCriticalSection(&m_mouseManagerSection);
}

void DxDeviceClass::HideMouseByRectClick()
{
    EnterCriticalSection(&m_mouseManagerSection);

    if(!m_mouseManagerEnabled)
	{
		LeaveCriticalSection(&m_mouseManagerSection);
	    return;
	}
	RECT rectX;
	POINT pt;
	::GetCursorPos(&pt);
	::GetWindowRect(m_topWindowHandle,&rectX);
    if(pt.x >= rectX.left &&  pt.x <= rectX.right)
	   if(pt.y >= rectX.top && pt.y <= rectX.bottom)
	   {
              m_mousePendingCommand = 2;
			  LeaveCriticalSection(&m_mouseManagerSection);
		      return;
	   }

    LeaveCriticalSection(&m_mouseManagerSection);
}

void DxDeviceClass::MouseManagerCallback()
{
	return;
	EnterCriticalSection(&m_mouseManagerSection);
	if(m_fullScreenMode == TRUE)
	{
		if(m_showMouse == TRUE)
		{
		   #ifndef _FORCEMOUSE
		   ::ShowCursor(FALSE);
		   #endif
           m_showMouse = FALSE;
		   m_mousePendingCommand = 0;
		}
		LeaveCriticalSection(&m_mouseManagerSection);
		return;
	}

	/*if(m_fullScreenMode == TRUE)
	{
		if(m_showMouse == FALSE)
		{
		   ::ShowCursor(TRUE);
           m_showMouse = TRUE;
		   m_mousePendingCommand = 0;
		}
		
	}*/

	if(!m_mouseManagerEnabled)
	{
		LeaveCriticalSection(&m_mouseManagerSection);
	    return;
	}

    if(m_mousePendingCommand != 0)
	{
       if(m_mousePendingCommand == 2)
	   {//disable if need be
         if(m_showMouse == TRUE)
		 {
		    #ifndef _FORCEMOUSE
            ::ShowCursor(FALSE);
			#endif
		    m_showMouse = FALSE;
		 }
	   }
	   else if(m_mousePendingCommand == 1)
	   {
         if(m_showMouse == FALSE)
		 {
		    #ifndef _FORCEMOUSE
            ::ShowCursor(TRUE);
			#endif
		    m_showMouse = TRUE;
		 }
	   }
	   m_mousePendingCommand = 0;
	}


    if(m_showMouse == FALSE)
	{//handle unfocus if not then lock curson to center of window handle
		if(::GetForegroundWindow()!=m_topWindowHandle && m_fullScreenMode == FALSE)
		{
		   #ifndef _FORCEMOUSE
           ::ShowCursor(TRUE);
           #endif
		   m_showMouse = TRUE;
		   LeaveCriticalSection(&m_mouseManagerSection);
		   return;
		}
		else
		{
			RECT rectX;
			::GetWindowRect(m_windowHandleRef,&rectX);
			SetCursorPos(rectX.left+((rectX.right-rectX.left)/2),rectX.top+((rectX.bottom-rectX.top)/2));
			LeaveCriticalSection(&m_mouseManagerSection);
			//::SetCursorPos(0,0);
		   return;
		}
	}//end handle unfocus if not then lock curson to center of window handle
	LeaveCriticalSection(&m_mouseManagerSection);
}

HWND DxDeviceClass::GetTopParent(HWND & hwnd)
{
    HWND current = hwnd;
	HWND topMost = ::GetParent(current);
	if(topMost == NULL)
	   return current;
 
	while(topMost != NULL)
	{
       current = topMost;
       topMost = ::GetParent(current);
	}
    return current;
}
//-----------------------------------------------------------------------
//Initialized Object or reinitializes to new settings
//call function is application GetDevices(...,...) to aquire
//-----------------------------------------------------------------------
int DxDeviceClass::CreateRenderingDevices(BOOL windowed,
								           int widthLoc,
								           int hieghtLoc,
								           int depth,
										   HWND windowHandle)
{
    /*if(widthLoc == -2)
	{//force original window
	   windowed = TRUE;
       widthLoc = m_initialCreateRect.right;
       hieghtLoc = m_initialCreateRect.bottom;
	}//end force original window*/
 


	if(windowHandle != NULL)
       m_windowHandleOrig = windowHandle;

    m_windowHandleRef = m_windowHandleOrig;
	//m_topWindowHandle = m_windowHandleRef;
	
	if(windowed == FALSE)
	{
	   m_windowHandleRef = GetTopParent(m_windowHandleRef);
	   m_topWindowHandle = m_windowHandleRef;
	}
	else
	   m_topWindowHandle = GetTopParent(m_windowHandleRef);

	WriteDebugFile("CreateRenderingDevices()",FALSE);
	::ShowWindow(m_windowHandleRef,SW_SHOW);
	
	
	
    if(m_currentWidth == widthLoc && m_currentHeight == hieghtLoc && m_currentDepth == depth && m_fullScreenMode != windowed)
	   return FALSE;//already set at this mode

	m_restoreStatesPls = TRUE;
    m_hwnd = m_windowHandleRef;
	m_fullScreenMode = !windowed;
	
	//Aquire D3D object
	if(!m_pD3D)
	{
		const char * which = "Renderer: plain Direct3D 9";
		m_pD3D = CreateD3D9PreferOn12(&which);
		WriteDebugFile((LPSTR)which,FALSE);
	}
	if(!m_pD3D)
		return 0;

	WriteDebugFile("D3D Create()",FALSE);
	//Aquire Current Display Mode
	D3DDISPLAYMODE d3ddm;
    if( FAILED( m_pD3D->GetAdapterDisplayMode( D3DADAPTER_DEFAULT, &d3ddm ) ) )
        return 3;

	if(!EnumerateAllDisplayModes())
		return 4;
	// "-1 = Auto Detect" in startcfg.aut (or no startcfg.aut at all) reaches
	// here as -1 x -1. That used to go straight into CreateDevice, fail three
	// times, land on the 640x480 fallback while m_currentWidth/Height stayed
	// -1, and every render target / viewport built from them afterwards was
	// garbage. Resolve it here: windowed -> the window's client area, else the
	// desktop mode.
	if(widthLoc <= 0 || hieghtLoc <= 0)
	{
	   RECT rcClient = {0,0,0,0};
	   if(windowed && ::GetClientRect(m_windowHandleRef,&rcClient) && rcClient.right > 0 && rcClient.bottom > 0)
	   {
	      widthLoc  = rcClient.right;
	      hieghtLoc = rcClient.bottom;
	   }
	   else
	   {
	      widthLoc  = d3ddm.Width;
	      hieghtLoc = d3ddm.Height;
	   }
	   WriteDebugFile("Auto-detect resolution ->",FALSE);
	   WriteDebugFile(widthLoc,FALSE);
	   WriteDebugFile(hieghtLoc,FALSE);
	}
	if(depth <= 0)
	   depth = (d3ddm.Format == D3DFMT_R5G6B5 || d3ddm.Format == D3DFMT_X1R5G5B5) ? 16 : 32;

	//validate resolution
	if(!windowed)
	if(!ValidateResolution(widthLoc,hieghtLoc,depth))
	{//requested full-screen mode is not available on this adapter
	   // 16-bit full-screen modes no longer exist on modern GPUs/drivers: keep the
	   // requested size at 32-bit if that mode exists, otherwise use the desktop mode.
	   if(ValidateResolution(widthLoc,hieghtLoc,32))
	      depth = 32;
	   else
	   {
	      widthLoc  = d3ddm.Width;
	      hieghtLoc = d3ddm.Height;
	      depth     = (d3ddm.Format == D3DFMT_R5G6B5 || d3ddm.Format == D3DFMT_X1R5G5B5) ? 16 : 32;
	   }
	   WriteDebugFile("Requested full-screen mode unavailable; using",FALSE);
	   WriteDebugFile(widthLoc,FALSE); WriteDebugFile(hieghtLoc,FALSE); WriteDebugFile(depth,FALSE);
	}//end requested full-screen mode not available

	m_currentWidth				= widthLoc - m_currentStartWidth;
	m_currentHeight             = hieghtLoc - m_currentStartHeight;
	m_currentDepth              = depth;

	if(!windowed)
	   ::MoveWindow(m_windowHandleRef,0,0,m_currentWidth,m_currentHeight,FALSE);

    

    //if device is already initialized just update it
	if(m_pd3dDevice)
	{//if already here just reset the device to perams
		WriteDebugFile("Reinit d3ddevice()",FALSE);
	   m_d3dpp.hDeviceWindow    = m_windowHandleRef;
	   m_d3dpp.Windowed         = windowed;	
	   m_d3dpp.BackBufferWidth  = m_currentWidth;
	   m_d3dpp.BackBufferHeight = m_currentHeight;

	   if(windowed == TRUE)
	   {
	      m_d3dpp.BackBufferFormat = d3ddm.Format;//Match Current display so we can create a compatible surface
	   }
	   else
	   {
	      //m_currentDepth = 16;
	      m_d3dpp.BackBufferFormat = GetDisplayFormat(m_currentDepth);
	   }

	   ApplyPresentationSettings(windowed);//re-apply vsync/pacing for the new mode
	   //MSAA must be re-validated against the new backbuffer/depth formats before Reset,
	   //an unsupported MultiSampleType fails the whole Reset call.
	   ApplyMultisampleSettings(windowed);
	   
	   if(m_pd3dDevice->Reset(&m_d3dpp)!=S_OK)
	   {
	      return 7;
	   }
	   GenerateStencilAndRenderTargetPlanes();//create stecil pass object 
	   CreateRenderTargets();
	   
	   
       return TRUE;
	}//end if already here just reset the device to perams
    

    //Aquire Depth buffer
    if(!GetBestDepthBuffer(FALSE))
	    return 2;//depth buffer aquired
    
	
	
	if(windowed == TRUE)
	   m_d3dpp.BackBufferFormat = d3ddm.Format;//Match Current display so we can create a compatible surface
	else
	   m_d3dpp.BackBufferFormat = GetDisplayFormat(m_currentDepth);
	
	m_d3dpp.SwapEffect             = D3DSWAPEFFECT_DISCARD;
	m_d3dpp.Windowed               = windowed;
	m_d3dpp.AutoDepthStencilFormat = m_depthBufferFormat;//Set Depth Buffer to present structure
	m_d3dpp.hDeviceWindow          = m_windowHandleRef;
	m_d3dpp.EnableAutoDepthStencil = TRUE;
	m_d3dpp.BackBufferCount        = 1;
	m_d3dpp.BackBufferWidth        = widthLoc;
	m_d3dpp.BackBufferHeight       = hieghtLoc;
	//VSync & presentation pacing: interval + full-screen refresh come from
	//m_vsyncMode and the adapter caps instead of hardcoded IMMEDIATE.
	ApplyPresentationSettings(windowed);
		//MSAA: validate the configured sample count against the adapter for the exact
		//backbuffer/depth pair; silently steps down (8->4->2->off) when unsupported.
		ApplyMultisampleSettings(windowed);

	char buftemp[250];
	ZeroMemory(buftemp,250);
	WriteDebugFile("BackBufferformat:",FALSE);
	char * tempDist = GetBufferDesc(m_d3dpp.BackBufferFormat,buftemp);
	WriteDebugFile(tempDist,FALSE);

	ZeroMemory(buftemp,250);
	WriteDebugFile("AutoDepthStencilFormat:",FALSE);
	tempDist = GetBufferDesc(m_d3dpp.AutoDepthStencilFormat,buftemp);
	WriteDebugFile(tempDist,FALSE);




	WriteDebugFile("::Current Device Resolution::",FALSE);
	WriteDebugFile(widthLoc,FALSE);
	WriteDebugFile(hieghtLoc,FALSE);
	WriteDebugFile(m_currentDepth,FALSE);
	//m_d3dpp.Flags


	//CreateDevice
	HRESULT err = 0;

	//--------------------------------------------------------------------
	// Pick the vertex-processing path from the adapter's real caps. The
	// 2002 code passed D3DCREATE_MIXED_VERTEXPROCESSING blindly and
	// degraded to D3DCREATE_SOFTWARE_VERTEXPROCESSING (CPU) from there.
	// On modern GPUs - and especially on the Windows 10/11 D3D9On12
	// mapping layer, which only implements hardware vertex processing -
	// the MIXED/SOFTWARE paths fail outright or throw the GPU's vertex
	// pipeline away. So ask the adapter first: anything reporting
	// D3DDEVCAPS_HWTRANSFORMANDLIGHT is created with
	// D3DCREATE_HARDWARE_VERTEXPROCESSING, so every vertex transform and
	// light runs on the GPU. Legacy adapters without hardware T&L keep
	// the original MIXED -> SOFTWARE degrade cascade below.
	// D3DCREATE_PUREDEVICE is deliberately NOT added: pure devices
	// refuse device-state Get calls and the engine uses them
	// (CStateManagementObj::SetFogOverrideState reads D3DRS_FOGCOLOR).
	//--------------------------------------------------------------------
	D3DCAPS9 adapterCaps;
	ZeroMemory(&adapterCaps,sizeof(adapterCaps));
	DWORD behaviorFlags = D3DCREATE_MIXED_VERTEXPROCESSING;//legacy default if caps can't be read
	if(SUCCEEDED(m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&adapterCaps)))
	{
	   if(adapterCaps.DevCaps & D3DDEVCAPS_HWTRANSFORMANDLIGHT)
	   {
	      behaviorFlags = D3DCREATE_HARDWARE_VERTEXPROCESSING;//GPU vertex processing: modern GPUs / D3D9On12
	      WriteDebugFile("Adapter caps: hardware T&L -> D3DCREATE_HARDWARE_VERTEXPROCESSING",FALSE);
	   }
	   else
	      WriteDebugFile("Adapter caps: no hardware T&L -> legacy D3DCREATE_MIXED_VERTEXPROCESSING",FALSE);
	}
	else
	   WriteDebugFile("Adapter caps: GetDeviceCaps failed -> legacy D3DCREATE_MIXED_VERTEXPROCESSING",FALSE);

	err = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
                                       D3DDEVTYPE_HAL,//D3DDEVTYPE DeviceType,
                                       m_windowHandleRef,//HWND  hFocusWindow,
                                       behaviorFlags,//DWORD BehaviorFlags,
                                       &m_d3dpp,
                                       &m_pd3dDevice);
	m_deviceBehaviorFlags = behaviorFlags;

	
	//if failed slam into redundancy mode
    if(!m_pd3dDevice)
	{
		//degrade vertex processing one tier: HARDWARE -> MIXED -> SOFTWARE
		if(behaviorFlags == D3DCREATE_HARDWARE_VERTEXPROCESSING)
		   behaviorFlags = D3DCREATE_MIXED_VERTEXPROCESSING;
		else
		   behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;
        err = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
                                   D3DDEVTYPE_HAL,//D3DDEVTYPE DeviceType,
                                   m_windowHandleRef,//HWND  hFocusWindow,
                                   behaviorFlags,//DWORD BehaviorFlags,
                                   &m_d3dpp,
                                   &m_pd3dDevice);
		m_deviceBehaviorFlags = behaviorFlags;
		WriteDebugFile("Failed try 1",FALSE);
    }

	if(!m_pd3dDevice)
	{
		behaviorFlags = D3DCREATE_SOFTWARE_VERTEXPROCESSING;//lowest vertex tier from here on
		m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
			ApplyMultisampleSettings(windowed);//re-check MSAA against the D16 depth buffer
		//m_d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
        err = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
                                   D3DDEVTYPE_HAL,//D3DDEVTYPE DeviceType,
                                   m_windowHandleRef,//HWND  hFocusWindow,
                                   behaviorFlags,//DWORD BehaviorFlags,
                                   &m_d3dpp,
                                   &m_pd3dDevice);
		m_deviceBehaviorFlags = behaviorFlags;
		WriteDebugFile("Failed try 2",FALSE);
    }

	if(!m_pd3dDevice)
	{
		m_d3dpp.BackBufferWidth  = 640;
	    m_d3dpp.BackBufferHeight = 480;
		m_d3dpp.BackBufferFormat = D3DFMT_R5G6B5;
		m_d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;//last resort: no vsync constraints
		m_d3dpp.FullScreen_RefreshRateInHz = 0;
		m_d3dpp.MultiSampleType    = D3DMULTISAMPLE_NONE;//last resort: no MSAA
		m_d3dpp.MultiSampleQuality = 0;
		m_frameLimitHz = 0;
        err = m_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
                                   D3DDEVTYPE_HAL,//D3DDEVTYPE DeviceType,
                                   m_windowHandleRef,//HWND  hFocusWindow,
                                   behaviorFlags,//DWORD BehaviorFlags,
                                   &m_d3dpp,
                                   &m_pd3dDevice);
		m_deviceBehaviorFlags = behaviorFlags;
		WriteDebugFile("Failed try 3",FALSE);
		if(m_pd3dDevice)
		{//the fallback changed the back buffer size; keep our bookkeeping in step
		   m_currentWidth  = 640;
		   m_currentHeight = 480;
		   m_currentDepth  = 16;
		}
    }


	if(m_publishDevice)
	  *m_publishDevice = m_pd3dDevice;

	if(!m_pd3dDevice)
	{
	   WriteDebugFile("Could not create device",FALSE);
	   if(err == D3DERR_DEVICELOST)
		   WriteDebugFile("D3DERR_DEVICELOST",FALSE);
        if(err == D3DERR_INVALIDCALL)
		   WriteDebugFile("D3DERR_INVALIDCALL",FALSE);
        if(err == D3DERR_NOTAVAILABLE)
		   WriteDebugFile("D3DERR_NOTAVAILABLE",FALSE);
        if(err == D3DERR_OUTOFVIDEOMEMORY)
			WriteDebugFile("D3DERR_OUTOFVIDEOMEMORY",FALSE);
       return 5;
	}

    



	WriteDebugFile("Device vertex-processing behavior flags:",FALSE);
	WriteDebugFile((int)m_deviceBehaviorFlags,FALSE);//64=HARDWARE, 128=MIXED, 32=SOFTWARE
	InitCapabilities();WriteDebugFile("InitCapabilities()",FALSE);//publish what this device can do
	CreateRenderTargets();WriteDebugFile("CreateRenderTargets()",FALSE);
	GenerateStencilAndRenderTargetPlanes();WriteDebugFile("GenerateStencilAndRenderTargetPlanes()",FALSE);//create stecil pass object 
	GetRenderRect();WriteDebugFile("GetRenderRect()",FALSE);

	return 1;
}
//-----------------------------------------------------------------------
//Multisample anti-aliasing (m_aaMode from startcfg.aut):
//0 = off (legacy 2002 behavior)
//1/2/3 = 2x/4x/8x MSAA. D3D9 MSAA lives in the swap-chain backbuffer and
//    the auto depth/stencil surface, so BOTH formats must pass
//    CheckDeviceMultiSampleType for the current windowed/full-screen mode.
//    Unsupported levels step down (8->4->2->off) instead of failing device
//    creation or Reset. The engine never reads the backbuffer back (no
//    StretchRect/GetRenderTargetData paths), so a multisampled backbuffer
//    needs no resolve handling anywhere else. On Windows 10/11 D3D9On12
//    modern GPUs report support up to 8x.
//-----------------------------------------------------------------------
void DxDeviceClass::ApplyMultisampleSettings(BOOL windowed)
{
	if(m_aaMode < 0 || m_aaMode > 3)
	   m_aaMode = 2;//clamp garbage to the safe default

	m_d3dpp.MultiSampleType    = D3DMULTISAMPLE_NONE;
	m_d3dpp.MultiSampleQuality = 0;
	if(m_aaMode == 0)
	   return;

	static const D3DMULTISAMPLE_TYPE msaaLevels[3] = { D3DMULTISAMPLE_2_SAMPLES,
		                                                 D3DMULTISAMPLE_4_SAMPLES,
		                                                 D3DMULTISAMPLE_8_SAMPLES };
	for(int tryLevel = m_aaMode; tryLevel >= 1; tryLevel--)
	{
	   D3DMULTISAMPLE_TYPE msType = msaaLevels[tryLevel-1];
	   DWORD qualityLevels = 0;
	   BOOL rtOK = SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
	                                                            D3DDEVTYPE_HAL,
	                                                            m_d3dpp.BackBufferFormat,
	                                                            windowed,
	                                                            msType,
	                                                            &qualityLevels));
	   BOOL dsOK = SUCCEEDED(m_pD3D->CheckDeviceMultiSampleType(D3DADAPTER_DEFAULT,
	                                                            D3DDEVTYPE_HAL,
	                                                            m_d3dpp.AutoDepthStencilFormat,
	                                                            windowed,
	                                                            msType,
	                                                            NULL));
	   if(rtOK && dsOK)
	   {
	      m_d3dpp.MultiSampleType    = msType;
	      m_d3dpp.MultiSampleQuality = (qualityLevels > 0) ? (qualityLevels - 1) : 0;
	      WriteDebugFile("MSAA sample mode applied:",FALSE);
	      WriteDebugFile(tryLevel,FALSE);
	      if(tryLevel < m_aaMode)
	         WriteDebugFile("MSAA: requested level unsupported - stepped down",FALSE);
	      return;
	   }
	}
	WriteDebugFile("MSAA: no supported sample level for this mode - disabled",FALSE);
}

//-----------------------------------------------------------------------
//VSync & presentation interval control (m_vsyncMode from startcfg.aut):
//0 = off (uncapped, legacy 2002 behavior)
//1 = vsync at monitor rate (D3DPRESENT_INTERVAL_ONE) - the default, so
//    GPUs no longer run uncapped at 2000+ FPS
//2/3/4/5 = cap at 60/120/144/240 Hz: vsync plus, in full-screen, the
//    closest adapter refresh <= target so the panel actually scans out
//    at that rate, plus a QPC frame pacer (PaceFrameLimit) that holds
//    the loop to the target when the monitor refreshes faster - or when
//    windowed, where the refresh rate belongs to the desktop.
//-----------------------------------------------------------------------
void DxDeviceClass::ApplyPresentationSettings(BOOL windowed)
{
	m_d3dpp.FullScreen_RefreshRateInHz = 0;//default unless a target Hz applies
	m_frameLimitHz = 0;

	if(m_vsyncMode < 0 || m_vsyncMode > 5)
	   m_vsyncMode = 1;//clamp garbage to the safe default

	//What presentation intervals does the adapter actually support?
	DWORD supportedIntervals = D3DPRESENT_INTERVAL_ONE;//safe baseline
	D3DCAPS9 capsLoc;
	ZeroMemory(&capsLoc,sizeof(capsLoc));
	if(m_pD3D && SUCCEEDED(m_pD3D->GetDeviceCaps(D3DADAPTER_DEFAULT,D3DDEVTYPE_HAL,&capsLoc)))
	   supportedIntervals = capsLoc.PresentationIntervals;

	if(m_vsyncMode == 0)
	   m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;//uncapped
	else
	{
	   if(supportedIntervals & D3DPRESENT_INTERVAL_ONE)
	      m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_ONE;
	   else
	      m_d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;//let the driver pick

	   switch(m_vsyncMode)
	   {
	      case 2: m_frameLimitHz = 60;  break;
	      case 3: m_frameLimitHz = 120; break;
	      case 4: m_frameLimitHz = 144; break;
	      case 5: m_frameLimitHz = 240; break;
	   }
	   //Full-screen: scan out at the target rate when the adapter has such a mode
	   if(!windowed && m_frameLimitHz > 0 && m_pD3D)
	      m_d3dpp.FullScreen_RefreshRateInHz = FindFullscreenRefreshRate(m_d3dpp.BackBufferWidth,
	                                                                     m_d3dpp.BackBufferHeight,
	                                                                     m_d3dpp.BackBufferFormat,
	                                                                     m_frameLimitHz);
	}

	WriteDebugFile("VSync mode (0=off 1=monitor 2=60Hz 3=120Hz 4=144Hz 5=240Hz):",FALSE);
	WriteDebugFile(m_vsyncMode,FALSE);
	WriteDebugFile("PresentationInterval (1=ONE 0=DEFAULT -2147483648=IMMEDIATE):",FALSE);
	WriteDebugFile((int)m_d3dpp.PresentationInterval,FALSE);
	WriteDebugFile("Fullscreen refresh Hz (0=default):",FALSE);
	WriteDebugFile((int)m_d3dpp.FullScreen_RefreshRateInHz,FALSE);
	WriteDebugFile("CPU frame cap Hz (0=off):",FALSE);
	WriteDebugFile(m_frameLimitHz,FALSE);
}
//-----------------------------------------------------------------------
//Closest adapter refresh rate <= targetHz for width x height in the given
//back-buffer format (exact match wins). 0 = none found; D3D then picks the
//mode's default refresh.
//-----------------------------------------------------------------------
int DxDeviceClass::FindFullscreenRefreshRate(int width,int height,D3DFORMAT format,int targetHz)
{
	if(!m_pD3D || targetHz <= 0)
	   return 0;

	int bestHz = 0;
	int modeCount = (int)m_pD3D->GetAdapterModeCount(D3DADAPTER_DEFAULT,format);
	for(int i=0;i<modeCount;i++)
	{
	   D3DDISPLAYMODE modeLoc;
	   if(FAILED(m_pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT,format,i,&modeLoc)))
	      continue;
	   if((int)modeLoc.Width != width || (int)modeLoc.Height != height)
	      continue;
	   int hz = (int)modeLoc.RefreshRate;
	   if(hz == targetHz)
	      return hz;//exact match - use it
	   if(hz < targetHz && hz > bestHz)
	      bestHz = hz;//closest below the target
	}
	return bestHz;
}
//-----------------------------------------------------------------------
//Frame pacer for the 60/120/144/240 Hz caps. Called right after a
//successful Present: sleep all but the last ~1.5 ms of the frame (1 ms
//Sleep granularity via timeBeginPeriod in the constructor), then spin
//onto the deadline with QPC. No-op when no cap is selected or vsync is
//already slower than the target.
//-----------------------------------------------------------------------
void DxDeviceClass::PaceFrameLimit()
{
	if(m_frameLimitHz <= 0)
	   return;

	LARGE_INTEGER nowLoc;
	QueryPerformanceCounter(&nowLoc);
	if(m_limiterLast.QuadPart == 0)
	{//first paced frame: seed the clock and go
	   m_limiterLast = nowLoc;
	   return;
	}

	LONGLONG frameTicks = (LONGLONG)(((double)m_limiterFreq.QuadPart) / ((double)m_frameLimitHz));
	LONGLONG deadline   = m_limiterLast.QuadPart + frameTicks;
	LONGLONG remaining  = deadline - nowLoc.QuadPart;
	if(remaining > 0)
	{
	   LONGLONG sleepTicks = remaining - ((m_limiterFreq.QuadPart * 3) / 2000);//keep last 1.5 ms for the spin
	   if(sleepTicks > 0)
	      Sleep((DWORD)((sleepTicks * 1000) / m_limiterFreq.QuadPart));
	   int spinGuard = 0;
	   while(++spinGuard < 500000)
	   {
	      QueryPerformanceCounter(&nowLoc);
	      if(nowLoc.QuadPart >= deadline)
	         break;
	   }
	}
	m_limiterLast = nowLoc;
}
//-----------------------------------------------------------------------
//Enumareate Compatible display modes
//-----------------------------------------------------------------------
BOOL DxDeviceClass::EnumerateAllDisplayModes()
{
  if(!m_pD3D)
	  return FALSE;
  //log possible modes
	m_loggedDisplayModes = 1;
    //fill in windowed mode
    m_vmodeWidth[0]              = -2;
    m_vmodeHeight[0]             = -2;
    m_vmodeDepth[0]              = -2;
    //end fill in windowed mode

	// Enumerate 32-bit modes first and deduplicate across all already-logged
	// resolutions so high-refresh-rate monitors (60/120/144/240Hz) do not
	// fill the 128-slot table with duplicate low resolutions before reaching
	// 1920x1080, 2560x1440, or 3840x2160.
	int totalDisplayModes = m_pD3D->GetAdapterModeCount( D3DADAPTER_DEFAULT,D3DFMT_X8R8G8B8 );
	int i=0;
	for(i=0;i<totalDisplayModes && m_loggedDisplayModes < 128;i++)
	{//find good 32-bit display modes
	   D3DDISPLAYMODE displayMode;
       m_pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT,D3DFMT_X8R8G8B8,i,&displayMode);

       if(displayMode.Format == D3DFMT_X8R8G8B8 && displayMode.Width >= 600)
	   {
	      BOOL alreadyLogged = FALSE;
	      for(int k=1;k<m_loggedDisplayModes;k++)
	      {
	         if(m_vmodeWidth[k] == (int)displayMode.Width &&
	            m_vmodeHeight[k] == (int)displayMode.Height &&
	            m_vmodeDepth[k] == 32)
	         {
	            alreadyLogged = TRUE;
	            break;
	         }
	      }
	      if(!alreadyLogged)
	      {
	         m_vmodeWidth[m_loggedDisplayModes]  = displayMode.Width;
	         m_vmodeHeight[m_loggedDisplayModes] = displayMode.Height;
	         m_vmodeDepth[m_loggedDisplayModes]  = 32;
	         m_loggedDisplayModes++;
	      }
	   }
	}//end find good 32-bit display modes

	// Guarantee the current desktop resolution is in the mode table.
	D3DDISPLAYMODE desktopMode;
	if(SUCCEEDED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT,&desktopMode)) &&
	   desktopMode.Width >= 600)
	{
	   BOOL desktopLogged = FALSE;
	   for(int k=1;k<m_loggedDisplayModes;k++)
	   {
	      if(m_vmodeWidth[k] == (int)desktopMode.Width &&
	         m_vmodeHeight[k] == (int)desktopMode.Height &&
	         m_vmodeDepth[k] == 32)
	      {
	         desktopLogged = TRUE;
	         break;
	      }
	   }
	   if(!desktopLogged)
	   {
	      int slot = (m_loggedDisplayModes < 128) ? m_loggedDisplayModes++ : 127;
	      m_vmodeWidth[slot]  = desktopMode.Width;
	      m_vmodeHeight[slot] = desktopMode.Height;
	      m_vmodeDepth[slot]  = 32;
	   }
	}

	// Fill any remaining slots with 16-bit modes on legacy adapters.
	totalDisplayModes = m_pD3D->GetAdapterModeCount( D3DADAPTER_DEFAULT,D3DFMT_R5G6B5 );
	for(i=0;i<totalDisplayModes && m_loggedDisplayModes < 128;i++)
	{//find good 16-bit display modes
	   D3DDISPLAYMODE displayMode;
       m_pD3D->EnumAdapterModes(D3DADAPTER_DEFAULT,D3DFMT_R5G6B5,i,&displayMode);

       if(displayMode.Format == D3DFMT_R5G6B5 && displayMode.Width >= 600)
	   {
	      BOOL alreadyLogged = FALSE;
	      for(int k=1;k<m_loggedDisplayModes;k++)
	      {
	         if(m_vmodeWidth[k] == (int)displayMode.Width &&
	            m_vmodeHeight[k] == (int)displayMode.Height &&
	            m_vmodeDepth[k] == 16)
	         {
	            alreadyLogged = TRUE;
	            break;
	         }
	      }
	      if(!alreadyLogged)
	      {
	         m_vmodeWidth[m_loggedDisplayModes]  = displayMode.Width;
	         m_vmodeHeight[m_loggedDisplayModes] = displayMode.Height;
	         m_vmodeDepth[m_loggedDisplayModes]  = 16;
	         m_loggedDisplayModes++;
	      }
	   }
	}//end find good 16-bit display modes
  return TRUE;
}
//-----------------------------------------------------------------------
//Get best Depth buffer
//-----------------------------------------------------------------------
BOOL DxDeviceClass::GetBestDepthBuffer(BOOL force16BitDepthForSpeed)
{
    if(!m_pD3D)
		return FALSE;

    int formatCount = 6;
	D3DFORMAT depthStencilFmts[50];
	BOOL      stencilSupported[50];
	LPSTR     depthStencilFmtsNames[50];
	if(!force16BitDepthForSpeed)
	{
       depthStencilFmts[0] = D3DFMT_D24X8;
	   depthStencilFmtsNames[0] = "D3DFMT_D24X8";

	}
	else
	{
	   depthStencilFmts[0] = D3DFMT_D16;
	   depthStencilFmtsNames[0] = "D3DFMT_D16";
	}


	depthStencilFmts[1] = D3DFMT_D24S8;
	depthStencilFmts[2] = D3DFMT_D24X8;
	depthStencilFmts[3] = D3DFMT_D15S1;
	depthStencilFmts[4] = D3DFMT_D32;
	depthStencilFmts[5] = D3DFMT_D16;

	
	depthStencilFmtsNames[1] = "D3DFMT_D24S8";
	depthStencilFmtsNames[2] = "D3DFMT_D24X8";
	depthStencilFmtsNames[3] = "D3DFMT_D15S1";
	depthStencilFmtsNames[4] = "D3DFMT_D32";
	depthStencilFmtsNames[5] = "D3DFMT_D16";

	stencilSupported[0] = TRUE;
	stencilSupported[1] = TRUE;
	stencilSupported[2] = TRUE;
	stencilSupported[3] = TRUE;
	stencilSupported[4] = FALSE;
	stencilSupported[5] = FALSE;
    

    for(int dbLoop = 0;dbLoop<formatCount;dbLoop++)
	{//get best format
       if(m_pD3D->CheckDeviceFormat( D3DADAPTER_DEFAULT, 
                                     D3DDEVTYPE_HAL, 
                                     GetDisplayFormat(m_currentDepth),//AdapterFormat, 
                                     D3DUSAGE_DEPTHSTENCIL, 
                                     D3DRTYPE_SURFACE,
                                     depthStencilFmts[dbLoop] ) == 0)
	   {

          m_depthBufferFormat          = depthStencilFmts[dbLoop];//d3ddm.Format;
		  m_supportedStencilBuffer     = stencilSupported[dbLoop];

		  WriteDebugFile("Aquired depth buffer",FALSE);
		  WriteDebugFile(depthStencilFmtsNames[dbLoop],FALSE);
	      return TRUE;
	   }
	}//end get best format
	WriteDebugFile("Failed to get Depth buffer",FALSE);
    return FALSE;
}
//-----------------------------------------------------------------------
//Get best Display Format
//-----------------------------------------------------------------------
D3DFORMAT DxDeviceClass::GetDisplayFormat(int depth)
{
    D3DFORMAT displayFormat = D3DFMT_R5G6B5;
    
	if(depth == 32)
       displayFormat = D3DFMT_X8R8G8B8;
    //log possible modes
	if(!m_pD3D)
	   return displayFormat;
	if(m_pD3D->GetAdapterModeCount( D3DADAPTER_DEFAULT,displayFormat ) > 0)
	   return displayFormat;
	else
       return D3DFMT_R5G6B5;//more compatible in bad case
}
//-----------------------------------------------------------------------
//Aquire device caps
//-----------------------------------------------------------------------
BOOL DxDeviceClass::InitCapabilities()
{
        if(!m_pd3dDevice)
		   return FALSE;
	
        m_pd3dDevice->GetDeviceCaps(&m_pCaps);
		m_shaderSupport = FALSE;
	    if(m_pCaps.VertexShaderVersion >= D3DVS_VERSION(1,1))
	       m_shaderSupport = TRUE;
	    

	    if(m_pCaps.MaxTextureWidth < m_pCaps.MaxTextureHeight)
           m_maxTextureSizeSquared = m_pCaps.MaxTextureWidth;
		else
           m_maxTextureSizeSquared = m_pCaps.MaxTextureHeight;

	    m_availTextureMem = GetAvailVideoMem();
		if(m_availTextureMem < 32)
           m_maxTextureSizeSquared = 256;
		else if(m_availTextureMem < 64)
           m_maxTextureSizeSquared = 512;

		if(m_maxTextureSizeSquared > m_maxTextureSize)
           m_maxTextureSizeSquared = m_maxTextureSize;

		return TRUE;
}
//-----------------------------------------------------------------------
//Aquire device caps
//-----------------------------------------------------------------------
int DxDeviceClass::GetAvailVideoMem()
{
    m_availTextureMem = m_pd3dDevice->GetAvailableTextureMem()/1000000;
    return m_availTextureMem;
}
//-----------------------------------------------------------------------
//Create stencil render plane
//-----------------------------------------------------------------------
BOOL DxDeviceClass::GenerateStencilAndRenderTargetPlanes()
{
	if(!m_pd3dDevice)
		return FALSE;
      
	if(m_shadowSquareVB)
	{
       m_shadowSquareVB->Release();
       m_shadowSquareVB = 0;
    }
	if(m_renderTargetSquareVB)
	{
       m_renderTargetSquareVB->Release();
       m_renderTargetSquareVB = 0;
	}
	//create shadow plane
	//Create a big square for rendering the stencilbuffer contents
    if(m_pd3dDevice->CreateVertexBuffer( 4*sizeof(SHADOWVERTEX),
                                         D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_DIFFUSE,
                                         D3DPOOL_MANAGED, &m_shadowSquareVB, NULL )==0 )
	{
          SHADOWVERTEX* v;
          float sx = (float)m_d3dpp.BackBufferWidth;
          float sy = (float)m_d3dpp.BackBufferHeight;
          m_shadowSquareVB->Lock( 0, 0, (void**)&v, 0 );
          v[0].p = D3DXVECTOR4(  0, sy, 0.0f, 1.0f );
          v[1].p = D3DXVECTOR4(  0,  0, 0.0f, 1.0f );
          v[2].p = D3DXVECTOR4( sx, sy, 0.0f, 1.0f );
          v[3].p = D3DXVECTOR4( sx,  0, 0.0f, 1.0f );
          v[0].color = 0x7f000000;
          v[1].color = 0x7f000000;
          v[2].color = 0x7f000000;
          v[3].color = 0x7f000000;
          m_shadowSquareVB->Unlock();
	}
	//Create a big square for rendering texture render targets to contents
	if(m_pd3dDevice->CreateVertexBuffer( 4*sizeof(RENDERTARGETABVX),
                                         D3DUSAGE_WRITEONLY, D3DFVF_XYZRHW | D3DFVF_TEX1 | D3DFVF_TEX2 | D3DFVF_TEX3 ,
                                         D3DPOOL_MANAGED, &m_renderTargetSquareVB, NULL )==0 )
	{
          RENDERTARGETABVX* v;
          float sx = (float)m_d3dpp.BackBufferWidth;
          float sy = (float)m_d3dpp.BackBufferHeight;
          m_renderTargetSquareVB->Lock( 0, 0, (void**)&v, 0 );
          v[0].p = D3DXVECTOR4(  0, sy, 0.0f,1.0f);//upper left
          v[1].p = D3DXVECTOR4(  0,  0, 0.0f,1.0f);//bottom left
          v[2].p = D3DXVECTOR4( sx, sy, 0.0f,1.0f);//upper right
          v[3].p = D3DXVECTOR4( sx,  0, 0.0f,1.0f);//bottom right
          v[0].u1 = 0.0f;//upper left
		  v[0].v1 = 1.0f;//upper left
          v[1].u1 = 0.0f;//bottom left
		  v[1].v1 = 0.0f;//bottom left
		  v[2].u1 = 1.0f;//upper right
		  v[2].v1 = 1.0f;//upper right
		  v[3].u1 = 1.0f;//bottom right
		  v[3].v1 = 0.0f;//bottom right
          //copy to second uv
		  v[0].u2 = v[0].u1-0.0015f;//copy
		  v[0].v2 = v[0].v1+0.0015f;//copy
		  v[1].u2 = v[1].u1-0.0015f;//copy
		  v[1].v2 = v[1].v1+0.0015f;//copy
		  v[2].u2 = v[2].u1-0.0015f;//copy
		  v[2].v2 = v[2].v1+0.0015f;//copy
		  v[3].u2 = v[3].u1-0.0015f;//copy
		  v[3].v2 = v[3].v1+0.0015f;//copy
		  //copy to third uv
		  v[0].u3 = v[0].u1-0.0015f;//copy
		  v[0].v3 = v[0].v1-0.0015f;//copy
		  v[1].u3 = v[1].u1-0.0015f;//copy
		  v[1].v3 = v[1].v1-0.0015f;//copy
		  v[2].u3 = v[2].u1-0.0015f;//copy
		  v[2].v3 = v[2].v1-0.0015f;//copy
		  v[3].u3 = v[3].u1-0.0015f;//copy
		  v[3].v3 = v[3].v1-0.0015f;//copy
          m_renderTargetSquareVB->Unlock();
		  return TRUE;
	}
	return FALSE;
}
//-----------------------------------------------------------------------
BOOL DxDeviceClass::ValidateResolution(int width, int height, int depth)
{
	 for(int dLoop=0;dLoop < m_loggedDisplayModes;dLoop++)
	 {
	    if(width == m_vmodeWidth[dLoop])
		   if(height == m_vmodeHeight[dLoop])
	          if(depth == m_vmodeDepth[dLoop])
			  {//found supported resolutuion
                 return TRUE;
			  }
	 }
  return FALSE;
}

//-----------------------------------------------------------------------
LPDIRECT3DVERTEXBUFFER9 DxDeviceClass::GetStencilRenderObjectRef()
{
   return m_shadowSquareVB;
}
//-----------------------------------------------------------------------
LPDIRECT3DDEVICE9 DxDeviceClass::GetD3DDeviceRef()
{
   return m_pd3dDevice;
}
//-----------------------------------------------------------------------
LPDIRECT3D9 DxDeviceClass::GetD3DRef()
{
   return m_pD3D;
}
//-----------------------------------------------------------------------
void DxDeviceClass::GetVideoModes(int * width, int * height, int * depth)
{
     CopyMemory(width,m_vmodeWidth,sizeof(int)*128);
	 CopyMemory(height,m_vmodeHeight,sizeof(int)*128);
	 CopyMemory(depth,m_vmodeDepth,sizeof(int)*128);
}
//-----------------------------------------------------------------------
RECT DxDeviceClass::GetRenderRect()
{
	
	 m_renderRect.left = m_currentStartWidth;
     m_renderRect.right = m_currentWidth;
	 m_renderRect.top = m_currentStartHeight;
	 m_renderRect.bottom = m_currentHeight;
	 return m_renderRect;
}
//-----------------------------------------------------------------------
int DxDeviceClass::GetDeviceMaxTextureSize()
{
	 return m_maxTextureSizeSquared;
}
//-----------------------------------------------------------------------
float DxDeviceClass::GetHeightAspectMultiplier()
{
	 return ((float)m_aspectBasis)/((float)m_currentHeight);
}
//-----------------------------------------------------------------------
float DxDeviceClass::GetWidthAspectMultiplier()
{
    if(m_aspectBasis > m_currentWidth)
	   return  1.0f;

    float scalar = ((float)m_aspectBasis)/((float)m_currentWidth);
	if(scalar < m_aspectScalarMax)
	   scalar = m_aspectScalarMax;

	return scalar;
}
//-----------------------------------------------------------------------
BOOL DxDeviceClass::UpdateDisplaySurfaces(int & szXMonitor,int & szYMonitor)
{
//BRING INTO VIEW
  if(m_fullScreenMode == TRUE){
   if(m_pd3dDevice->Present(NULL,NULL,NULL,NULL)==D3DERR_DEVICELOST)
   {
	   if(!m_renderingPaused){
		  szXMonitor = 0;
		  szYMonitor = 0;
	      Sleep(2000);
	   }
	   m_renderingPaused = TRUE;
	
	   Sleep(2000);
	   HWND tempHWnd = ::GetForegroundWindow();
	   if(tempHWnd==m_windowHandleRef && szXMonitor > 0 && szYMonitor > 0){
		  ::ShowWindow(m_windowHandleRef,SW_SHOW);
		  if(CreateRenderingDevices(!m_fullScreenMode,m_currentWidth,m_currentHeight,m_currentDepth,m_windowHandleRef))
		  {//reset completed ok
		    m_renderingPaused = FALSE;
		  }//end reset completed ok
		  else
		  {
	        //::ShowWindow(m_windowHandleRef,SW_MINIMIZE);
            Sleep(2000);
			return FALSE;
		  }
	   }
   }
   else
      PaceFrameLimit();//hold the 60/120/144/240 Hz caps
  }
  else
  {
   if(m_pd3dDevice->Present(NULL,&m_renderRect,NULL,NULL) != 0)
   {
      return FALSE;
   }
   PaceFrameLimit();//hold the 60/120/144/240 Hz caps
  }
  return TRUE;
}
//-----------------------------------------------------------------------
int DxDeviceClass::UpdateDisplaySurfaces()
{
//BRING INTO VIEW
  if(m_fullScreenMode == TRUE){
   if(m_pd3dDevice->Present(NULL,NULL,NULL,NULL)!=0)
   {
        HWND tempHWnd = ::GetForegroundWindow();
	    Sleep(2000);
	    if(tempHWnd==m_windowHandleRef)
		{
		   CreateRenderTargets();
           if(m_pd3dDevice->Reset(&m_d3dpp)!=S_OK)
		   {
              Sleep(2000);
			  return 3;
		   }
		   else
		   {
		      CreateRenderTargets();
	          GenerateStencilAndRenderTargetPlanes();//create stecil pass object 
		      m_restoreStatesPls = TRUE;
			  return 2;
		   }
		}
	  return 0;
   }
   PaceFrameLimit();//hold the 60/120/144/240 Hz caps
   return 1;
  }
  else
  {
   if(m_pd3dDevice->Present(NULL,&m_renderRect,NULL,NULL) != 0)
   {
     //HWND tempHWnd = ::GetForegroundWindow();
	    Sleep(2000);
	   //if(tempHWnd==m_windowHandleRef)
		//{
		   TearDownDeviceStuff();
           if(m_pd3dDevice->Reset(&m_d3dpp)!=S_OK)
		   {
              Sleep(1000);
			  return 3;
		   }
		   else
		   {
			  Sleep(1000);
		      CreateRenderTargets();
	          //GenerateStencilAndRenderTargetPlanes();//create stecil pass object 
			  GenerateStencilAndRenderTargetPlanes();//create stecil pass object 
			  m_restoreStatesPls = TRUE;
			  return 2;
		   }
		//}
      return 0;
   }
   PaceFrameLimit();//hold the 60/120/144/240 Hz caps
   return 1;
  }
  return 1;
}
//-----------------------------------------------------------------------
void DxDeviceClass::TearDownDeviceStuff()
{
     for(int dLoop=0;dLoop<RT_COUNT;dLoop++)
	 {
	      if(m_renderTargets[dLoop])
		     m_renderTargets[dLoop]->Release();
		     m_renderTargets[dLoop] = 0;

		  if(m_renderTargetsSurfaces[dLoop])
		     m_renderTargetsSurfaces[dLoop]->Release();
		     m_renderTargetsSurfaces[dLoop] = 0;
	 }

	 if(m_shadowSquareVB)
	 {
       m_shadowSquareVB->Release();
       m_shadowSquareVB = 0;
     }
	 if(m_renderTargetSquareVB)
	 {
       m_renderTargetSquareVB->Release();
       m_renderTargetSquareVB = 0;
	 }

}
//-----------------------------------------------------------------------
BOOL DxDeviceClass::CreateRenderTargets()
{
     //deallocate
     for(int dLoop=0;dLoop<RT_COUNT;dLoop++)
	 {
	      if(m_renderTargets[dLoop])
		     m_renderTargets[dLoop]->Release();
		     m_renderTargets[dLoop] = 0;

		  if(m_renderTargetsSurfaces[dLoop])
		     m_renderTargetsSurfaces[dLoop]->Release();
		     m_renderTargetsSurfaces[dLoop] = 0;
	 }
	 //detect
	 if( FAILED( m_pD3D->CheckDeviceFormat(   m_pCaps.AdapterOrdinal, m_pCaps.DeviceType,
										      m_d3dpp.BackBufferFormat, D3DUSAGE_RENDERTARGET, 
										      D3DRTYPE_TEXTURE, D3DFMT_A16B16G16R16F ) ) )
	 {
        HRESULT err1 = m_pD3D->CheckDeviceFormat(   m_pCaps.AdapterOrdinal, m_pCaps.DeviceType,
										            m_d3dpp.BackBufferFormat, D3DUSAGE_RENDERTARGET, 
										            D3DRTYPE_TEXTURE, D3DFMT_A8R8G8B8 );

        if(err1 != 0)
		   return FALSE;

        m_renderTargetTextFormat = D3DFMT_A8R8G8B8;
	 }
     else
        m_renderTargetTextFormat = D3DFMT_A16B16G16R16F;
	 //create
	 for(int rLoop=0;rLoop<RT_COUNT;rLoop++)
	 {
		HRESULT err = m_pd3dDevice->CreateTexture( m_d3dpp.BackBufferWidth,
													m_d3dpp.BackBufferHeight,
													1,
													D3DUSAGE_RENDERTARGET,
													m_renderTargetTextFormat,
													D3DPOOL_DEFAULT,
													&m_renderTargets[rLoop],
													NULL );
	
		switch(err)
		{
		case D3DERR_INVALIDCALL:
           m_renderTargetAvailable = FALSE;

        break;
		case D3DERR_OUTOFVIDEOMEMORY:
           m_renderTargetAvailable = FALSE;

        break;
		case E_OUTOFMEMORY:
           m_renderTargetAvailable = FALSE;

        break;
		case S_OK:
           
		   if(m_renderTargets[rLoop]->GetSurfaceLevel(0,&m_renderTargetsSurfaces[rLoop])==S_OK)
		      m_renderTargetAvailable = TRUE;
		   
        break;
		}
	 }
     return TRUE;
}


//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//

void DxDeviceClass::WriteDebugFile(int debugString,BOOL clearIt)
{
	if(debugString == 0)
	{
      WriteDebugFile("0",clearIt);
	}
	else
	{
	char buffer[67];
    _ltoa_s(debugString,buffer,10);
    WriteDebugFile(buffer,clearIt);
	}
}
//-----------------------------------------------------------------//
void DxDeviceClass::WriteDebugFile(LPSTR debugString,BOOL clearIt)
{

    int oldFile = 0;
	if(clearIt)	
	   _sopen_s(&oldFile, "DeviceLog.txt", _O_WRONLY | _O_TRUNC  | _O_CREAT  |_O_BINARY , _SH_DENYNO, _S_IWRITE);  
	else
	{
	   _sopen_s(&oldFile, "DeviceLog.txt", _O_WRONLY  | _O_CREAT  |_O_BINARY ,  _SH_DENYNO, _S_IWRITE);  
	   if(oldFile == -1)
	      return;
	   _lseek( oldFile, 0L, SEEK_END );


	}
	if(oldFile == -1)
	      return;
	
	int trace = 0;
    while(debugString[trace++]!= 0)
	{
        //trace++;
	}
	long bytesRead = _write( oldFile, debugString, trace );
	LPSTR returnCase = "\n";
    _write( oldFile, returnCase, 2 );
 
	_close( oldFile );

    /*HANDLE hFile = NULL;
	
	if(clearIt)
	{
	       hFile = CreateFile("DeviceLog.txt",
								GENERIC_WRITE, FILE_SHARE_READ,
								NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}
	else
	{
           hFile = CreateFile( "DeviceLog.txt",
								GENERIC_WRITE, FILE_SHARE_READ,
								NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	}

	if (hFile == INVALID_HANDLE_VALUE)
	    return;

    int trace = 0;
    while(debugString[trace++]!= 0)
	{

	}

	OVERLAPPED temp;
	DWORD temp2 = 0;

	WriteFile(
					hFile,//HANDLE hFile,
					debugString,//LPCVOID lpBuffer,
					trace,//DWORD nNumberOfBytesToWrite,
					&temp2,//LPDWORD lpNumberOfBytesWritten,
					&temp//LPOVERLAPPED lpOverlapped
					);

	CloseHandle(hFile);*/


	/*CFileException exc;
    CFile fl;
	CArchive the_out_Archive(&fl,CArchive::store);
    BOOL res = 0;
	if(clearIt)
	   res = fl.Open(filename,CFile::modeCreate | CFile::modeWrite, &exc);
	else
	   res = fl.Open(filename,CFile::modeNoTruncate | CFile::modeWrite, &exc);
	
	
	if(res)
	{
		the_out_Archive.WriteString( debugString );  
		the_out_Archive.WriteString( "\n");  
		the_out_Archive.Close();
		fl.Close(); 
	}*/
}

void DxDeviceClass::SetViewPort(DWORD & width, DWORD & height)
{
     m_viewPortTemp.Width = width;
     m_viewPortTemp.Height = height;
	 m_pd3dDevice->SetViewport(&m_viewPortTemp);
}

void DxDeviceClass::GetViewPort(DWORD & width, DWORD & height)
{
     width = m_viewPortTemp.Width;
     height = m_viewPortTemp.Height;
}

//-----------------------------------------------------------------//
char * DxDeviceClass::GetBufferDesc(D3DFORMAT format,char * returnText)
{

     switch(format)
	 {
	    case D3DFMT_R8G8B8:returnText="D3DFMT_R8G8B8";break;
	    case D3DFMT_A8R8G8B8:returnText="D3DFMT_A8R8G8B8";break;
	    case D3DFMT_X8R8G8B8:returnText="D3DFMT_X8R8G8B8";break;
        case D3DFMT_R5G6B5:returnText="D3DFMT_R5G6B5";break;
	    case D3DFMT_X1R5G5B5:returnText="D3DFMT_X1R5G5B5";break;
	    case D3DFMT_A1R5G5B5:returnText="D3DFMT_A1R5G5B5";break;
		case D3DFMT_A4R4G4B4:returnText="D3DFMT_A4R4G4B4";break;

		case D3DFMT_A8:returnText="D3DFMT_A8";break;
		case D3DFMT_R3G3B2:returnText="D3DFMT_R3G3B2";break;
		case D3DFMT_A8R3G3B2:returnText="D3DFMT_A8R3G3B2";break;
		case D3DFMT_X4R4G4B4:returnText="D3DFMT_X4R4G4B4";break;
		case D3DFMT_A2B10G10R10:returnText="D3DFMT_A2B10G10R10";break;

		case D3DFMT_G16R16:returnText="D3DFMT_G16R16";break;
		case D3DFMT_A8P8:returnText="D3DFMT_A8P8";break;
		case D3DFMT_P8:returnText="D3DFMT_P8";break;
		case D3DFMT_L8:returnText="D3DFMT_L8";break;
		case D3DFMT_A8L8:returnText="D3DFMT_A8L8";break;
		case D3DFMT_A4L4:returnText="D3DFMT_A4L4";break;

		case D3DFMT_V8U8:returnText="D3DFMT_V8U8";break;
		case D3DFMT_Q8W8V8U8:returnText="D3DFMT_Q8W8V8U8";break;
		case D3DFMT_V16U16:returnText="D3DFMT_V16U16";break;
		case D3DFMT_L6V5U5:returnText="D3DFMT_L6V5U5";break;

		case D3DFMT_X8L8V8U8:returnText="D3DFMT_X8L8V8U8";break;
		case D3DFMT_A2W10V10U10:returnText="D3DFMT_A2W10V10U10";break;
		case D3DFMT_UYVY:returnText="D3DFMT_UYVY";break;
		case D3DFMT_YUY2:returnText="D3DFMT_YUY2";break;
		case D3DFMT_DXT1:returnText="D3DFMT_DXT1";break;
		case D3DFMT_DXT2:returnText="D3DFMT_DXT2";break;
		case D3DFMT_DXT3:returnText="D3DFMT_DXT3";break;
		case D3DFMT_DXT4:returnText="D3DFMT_DXT4";break;
		case D3DFMT_DXT5:returnText="D3DFMT_DXT5";break;


		case D3DFMT_VERTEXDATA:returnText="D3DFMT_VERTEXDATA";break;
		case D3DFMT_INDEX16:returnText="D3DFMT_INDEX16";break;
		case D3DFMT_INDEX32:returnText="D3DFMT_INDEX32";break;
		case D3DFMT_D16_LOCKABLE:returnText="D3DFMT_D16_LOCKABLE";break;
		case D3DFMT_D32:returnText="D3DFMT_D32";break;
		case D3DFMT_D15S1:returnText="D3DFMT_D15S1";break;
		case D3DFMT_D24S8:returnText="D3DFMT_D24S8";break;
		case D3DFMT_D16:returnText="D3DFMT_D16";break;
		case D3DFMT_D24X8:returnText="D3DFMT_D24X8";break;
	    case D3DFMT_D24X4S4:returnText="D3DFMT_D24X4S4";break;
        case D3DFMT_UNKNOWN:returnText="D3DFMT_UNKNOWN";break;
	    default:returnText="cannot find";break;
	 }

	 return returnText;
}

