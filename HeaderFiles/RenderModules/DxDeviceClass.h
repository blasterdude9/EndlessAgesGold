

#pragma once
#include "CStructuresMisl.h"

#define RT_COUNT 1

struct RENDERTARGETABVX
{
	D3DXVECTOR4 p;
	float u1,v1;
	float u2,v2;
	float u3,v3;
};

class DxDeviceClass
{
public:
	DxDeviceClass(int maxTextureSize,
		          int defaultDispWidth,
				  int defaultDispHeight,
				  int defaultDispDepth,
				  void ** devicePublish);
	~DxDeviceClass(void);


	//Device Variables
	LPDIRECT3D9             m_pD3D;
    LPDIRECT3DDEVICE9       m_pd3dDevice;
	LPDIRECT3DDEVICE9     * m_publishDevice;//device to update for when it recreates
	D3DFORMAT               m_depthBufferFormat;
	D3DPRESENT_PARAMETERS   m_d3dpp;
	D3DCAPS9                m_pCaps;
	DWORD                   m_deviceBehaviorFlags;//CreateDevice behavior flags in use (HW/MIXED/SW vertex processing)
	int                   m_vsyncMode;//0=off 1=vsync at monitor rate 2=60Hz 3=120Hz 4=144Hz 5=240Hz cap
	int                   m_frameLimitHz;//CPU frame-pacing target derived from m_vsyncMode (0=off)
	int                   m_aaMode;//MSAA: 0=off 1=2x 2=4x 3=8x (caps-checked, auto step-down)
	int                   m_anisoMode;//anisotropic filtering: 0=off 1=2x 2=4x 3=8x 4=16x (caps-checked)
	LARGE_INTEGER         m_limiterFreq;//QueryPerformanceFrequency for the frame pacer
	LARGE_INTEGER         m_limiterLast;//QPC timestamp of the last paced frame
	int                     m_vmodeWidth[128];
	int                     m_vmodeHeight[128];
	int                     m_vmodeDepth[128];
	LPDIRECT3DVERTEXBUFFER9 m_shadowSquareVB;
	LPDIRECT3DVERTEXBUFFER9 m_renderTargetSquareVB;
	BOOL                    m_restoreStatesPls;
	RECT                    m_initialCreateRect;
	//Status variables
	BOOL                  m_supportedStencilBuffer;
	int                   m_currentWidth;
	int                   m_currentStartWidth;
	int                   m_currentHeight;
	int                   m_currentStartHeight;
	int                   m_currentDepth;
	int                   m_defaultWidth;
	int                   m_defaultHeight;
	int                   m_defaultDepth;
	int                   m_maxTextureSizeSquared;
	BOOL                  m_shaderSupport;
	int                   m_availTextureMem;
	int                   m_loggedDisplayModes;
	RECT                  m_renderRect;
	HWND                  m_windowHandleRef;
	HWND                  m_windowHandleOrig;
	BOOL                  m_renderingPaused;
	BOOL                  m_fullScreenMode;
	int                   m_aspectBasis;
	HWND                  m_hwnd;
	HWND                  m_topWindowHandle;
	float                 m_aspectScalarMax;
	BOOL                  m_showMouse;
	BOOL                  m_mouseManagerEnabled;
	DWORD                 m_toggleTimer;
	CRITICAL_SECTION      m_mouseManagerSection;
	int                   m_mousePendingCommand;
	BOOL                  m_supportsGammaControl;
	D3DVIEWPORT9		  m_viewPortTemp;
	D3DFORMAT			  m_renderTargetTextFormat;
	

	//render targets
	IDirect3DTexture9*    m_renderTargets[RT_COUNT];
	IDirect3DSurface9*    m_renderTargetsSurfaces[RT_COUNT];
	BOOL                  m_renderTargetAvailable;

	//Manual Overrides
	int                   m_maxTextureSize;


	//Functions
    int  CreateRenderingDevices(BOOL windowed,
								int widthLoc,
								int hieghtLoc,
								int depth,
								HWND windowHandle);
	BOOL EnumerateAllDisplayModes();
	BOOL GetBestDepthBuffer(BOOL force16BitDepthForSpeed);
	D3DFORMAT GetDisplayFormat(int depth);
	BOOL InitCapabilities();
	int GetAvailVideoMem();
	BOOL GenerateStencilAndRenderTargetPlanes();
	LPDIRECT3DVERTEXBUFFER9 GetStencilRenderObjectRef();
	LPDIRECT3DDEVICE9 GetD3DDeviceRef();
	LPDIRECT3D9 GetD3DRef();
    void GetVideoModes(int * width, int * height, int * depth);
	BOOL ValidateResolution(int width, int height, int depth);
	RECT GetRenderRect();
	int UpdateDisplaySurfaces(int & szXMonitor,int & szYMonitor);
	int GetDeviceMaxTextureSize();
	void ApplyPresentationSettings(BOOL windowed);
	void ApplyMultisampleSettings(BOOL windowed);
	int FindFullscreenRefreshRate(int width,int height,D3DFORMAT format,int targetHz);
	void PaceFrameLimit();
	float GetWidthAspectMultiplier();
	float GetHeightAspectMultiplier();
	BOOL UpdateDisplaySurfaces();
	BOOL CreateRenderTargets();
	void WriteDebugFile(LPSTR debugString,BOOL clearIt);
	HWND GetTopParent(HWND & hwnd);
	void MouseManagerCallback();
	void EnableManagerCallback();
    void DisableManagerCallback();
	void DisableMouse();
	void EnableMouse();
	void HideMouseByRectClick();
	void ToggleMouse();
	void TearDownDeviceStuff();
	void SetGamma(WORD r, WORD g, WORD b);
	void WriteDebugFile(int debugString,BOOL clearIt);
	char * GetBufferDesc(D3DFORMAT format,char * returnText);
	void SetViewPort(DWORD & width, DWORD & height);
	void GetViewPort(DWORD & width, DWORD & height);

};
