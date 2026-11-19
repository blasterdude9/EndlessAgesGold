//MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CGameApp.h"

#include "MainFrm.h"
#include "direct.h"
#include "getdxver.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


//CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_TIMER()
	ON_WM_ACTIVATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

DWORD WINAPI ValidateDownloadFiles(LPVOID lpParameter);
DWORD WINAPI RenderSpinner(LPVOID lpParameter);
//CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_mPlayer = NULL;
	m_maxTextureSize = 2048;
    m_shadowCount = 10;
	m_upgradeThreadWorking = FALSE;
	m_lgThreadHandle = NULL;
	m_engineClient = NULL;
	m_percentagePub = 0;
	m_bytesDownloaded = 0;
	m_toggleTest = 0;
	m_closeNow = 0;
	tested = FALSE;
	m_running = FALSE;
	m_rSp = FALSE;
	m_downloading = FALSE;
	m_openProgress = FALSE;
	//m_prgDlg = NULL;
    BOOL m_sup = FALSE;
    m_xEncrypt  =NULL;
    DWORD curVersion = 0x00090003;
    if(!DetectDirectXVersionReq(curVersion,"DirectX9c or better must be installed. Download DirectX from www.microsoft.com/directX."))
		return;

	m_devMode = 0;
    m_ready = FALSE;
	m_axLogger.m_logFileName = "CGameApp.txt";
	m_encryptedMode = TRUE;
	 //m_axmedia           = NULL;
	m_pendingVolume  = 0;
	m_xEncrypt = new CAXEncryptionLib("qwertyuiopoiuytrewqwertyuiopoiuy");
	m_renderSpinner = FALSE;
	m_closeDown       = FALSE;
	m_disableMouseCmd = 0;
    m_rnThreadID = 0;
	m_rnThreadHandle = 0;
	m_downloadURL = "";
	m_contentUpdatesInClient = FALSE;   // the launcher updates files; the client never downloads content
	m_startInFullscreen = FALSE;
	m_width				= 1024;
	m_height			= 768;
	m_depth				= 32;
	//InitializeIIDs(&IID_DActiveXPortal, &IID_DActiveXPortalEvents);

    InitializeCriticalSection(&m_updateCriticalSection);
	
	//EnableSimpleFrame();
	
	HKEY tempKey;
    
	// Install path: the folder the exe lives in wins when it holds the game data
	// (a GameFiles\ subfolder), so a copy of the game in any folder runs from that
	// folder. Only when the exe sits alone do we fall back to the old installer's
	// registry key ENDLESS_AGESR\INSTALL_PATH.
	{
		char exeDir[MAX_PATH] = {0};
		if(GetModuleFileNameA(NULL, exeDir, MAX_PATH))
		{
			char* slash = strrchr(exeDir, '\\');
			if(slash) *(slash + 1) = 0;
			CString probe = CString(exeDir) + "GameFiles";
			DWORD attr = GetFileAttributesA(probe);
			if(attr != INVALID_FILE_ATTRIBUTES && (attr & FILE_ATTRIBUTE_DIRECTORY))
				m_installPath = exeDir;
		}
	}
	if(m_installPath.IsEmpty())
		m_installPath = abTool.GetSoftwareRegkeyVal("ENDLESS_AGESR","INSTALL_PATH");
	if(m_installPath.GetLength() > 0 && m_installPath.Right(1) != "\\")
		m_installPath += "\\";
    if(!SetCurrentDirectory(m_installPath))
		m_installPath = "";

	CString devMode = abTool.GetSoftwareRegkeyVal("ENDLESS_AGESR","DEV_ON");
	if(devMode.GetLength() > 0) 
		m_devMode = 1;

	//#ifdef _DEBUG
	//	m_devMode = 1;
	//#endif

    /*if(m_installPath.GetLength()==0)
	{//make user select path
	   m_axLogger.WriteDebugFile("Creating Content Path",0,1);
       CDirectorySelect dlg;
	   dlg.m_selectedDirectory = "C:\\GameWorlds\\";//set default//
	   dlg.DoModal();
	   m_axLogger.WriteDebugFile("Set Content Path:",0,0);
	   m_axLogger.WriteDebugFile(dlg.m_selectedDirectory.GetBuffer(),0,1);
	   m_installPath = dlg.m_selectedDirectory;
	   //abTool.MakeDirectory(m_installPath.GetBuffer());
	   CString tempDir = m_installPath+"CACHE\\";
	   abTool.MakeDirectory(tempDir.GetBuffer());
	   tempDir = m_installPath+"GameFiles\\";
	   abTool.MakeDirectory(tempDir.GetBuffer());
	   tempDir = m_installPath+"GameFiles\\";
	   abTool.MakeDirectory(tempDir.GetBuffer());
	   tempDir = m_installPath+"MapsModels\\";
	   abTool.MakeDirectory(tempDir.GetBuffer());
	   tempDir = m_installPath+"Sounds\\";
	   abTool.MakeDirectory(tempDir.GetBuffer());
	   abTool.SetSoftwareReg("GAME_WORLDS","INSTALL_PATH",m_installPath.GetBuffer());
	}//end make user select path*/
    m_contentDownloadManager = new CAXContentClient();
	m_axLogger.WriteDebugFile("Starting Up",1,1);
	m_axLogger.WriteDebugFile("Getting content Path",0,1);
	//aquire username and pass then set it
	m_cacheDirectory = m_installPath+"CACHE\\";
	abTool.MakeDirectory(m_cacheDirectory.GetBuffer());
	m_contentDownloadManager->SetCacheDirectory(m_cacheDirectory);
	m_axLogger.WriteDebugFile("Set Cache dir:",0,0);
	m_axLogger.WriteDebugFile(m_cacheDirectory.GetBuffer(),0,1);
	m_ready = TRUE;
	//config block
	BOOL tempBool = 0;
	if(!GetInfoFromCoinitFile(m_username,//CString & username,
						      m_password,//CString & password,
						      m_game,//CString & gameName,
					          m_downloadURL,//CString & contentServerIP,
						      m_gserverIP,//CString & gameServerIP,
						      m_startInFullscreen,//BOOL & fullScreen,
						      tempBool,
							  m_width,
							  m_height,
							  m_depth))//BOOL & saveLogonData))
		m_axLogger.WriteDebugFile("GetInfoFromCoinitFile cannot locate file coinit",0,1);

	// Login is done IN-GAME (EARevolution.MOD login window -> Login() script command).
	// Never start with credentials from coinit.tmp: the start-up script's connect step
	// only shows the login window when the username is empty, so there is no way to
	// skip the login screen. (Game/server address from coinit.tmp are still used.)
	m_username = "";
	m_password = "";
	if(m_game.IsEmpty())      m_game      = "ENDLESS";
	if(m_gserverIP.IsEmpty()) m_gserverIP = "127.0.0.1";

//set window title
	m_downloadURL = "http://" + m_downloadURL;
	m_ready = TRUE;
}

CMainFrame::~CMainFrame()
{
	if(!m_xEncrypt)
		return;

	if(m_engineClient)
		m_engineClient->m_initObject->m_gStatus->SetCloseDownCallState(TRUE);
    EnterCriticalSection(&m_updateCriticalSection);
	m_closeDown = TRUE;
	LeaveCriticalSection(&m_updateCriticalSection);
	Sleep(100);
	EnterCriticalSection(&m_updateCriticalSection);
	if(m_lgThreadHandle != NULL)
	   CloseHandle(m_lgThreadHandle);
		
	if(m_renderSpinner == TRUE)
	{
		while(m_closeDown == TRUE)
		{
			LeaveCriticalSection(&m_updateCriticalSection);
			Sleep(100);
			EnterCriticalSection(&m_updateCriticalSection);
		}
	}
	LeaveCriticalSection(&m_updateCriticalSection);
	
	EnterCriticalSection(&m_updateCriticalSection);
	while(m_upgradeThreadWorking == TRUE)
	{
		LeaveCriticalSection(&m_updateCriticalSection);
		Sleep(100);
		EnterCriticalSection(&m_updateCriticalSection);
	}
	LeaveCriticalSection(&m_updateCriticalSection);
	   

	if(m_contentDownloadManager)
		delete m_contentDownloadManager;
	m_contentDownloadManager = 0;

	if(m_engineClient)
		delete m_engineClient;
	m_engineClient = 0;

    if(m_xEncrypt)
		delete m_xEncrypt;
	m_xEncrypt = 0;

	if(m_mPlayer)
		delete m_mPlayer;
	m_mPlayer = 0;

	DeleteCriticalSection(&m_updateCriticalSection);
}


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if(CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//Init engine
    if(!m_xEncrypt)
	    return 0;
		
	HWND windowHandleTest = NULL;
	windowHandleTest = GetSafeHwnd();
	SetCurrentDirectory(m_installPath.GetBuffer());
	if(windowHandleTest)
	{//valid windowHandleTest
		if(!m_running)
		{//not running
			if(!m_engineClient)
			{//not the engine
				m_axLogger.WriteDebugFile("Init Client Object",0,1);
				windowHandle = GetSafeHwnd();
				HWND fullScreenWindow = windowHandleTest;//GetTopmostParent();
				//::GetWindowRect(windowHandleTest,&m_windowSz);
				::GetClientRect(windowHandleTest,&m_windowSz);
				::SetWindowText(windowHandleTest,m_game);

				m_PlayerEnabled = FALSE;
				if(abTool.ValidateRegistryEntry("Software\\Microsoft\\Active Setup\\Installed Components\\{6BF52A52-394A-11d3-B153-00C04F79FAA6}"))
				{//make sure control exists on machine
					if(!m_mPlayer)
						m_mPlayer = new COcx2();

					//BOOL resCnt = m_mPlayer->Create(NULL, 0, CRect(50,50,100,100), this, 0);
					BOOL resCnt = m_mPlayer->Create(NULL, 0, CRect(50,50,100,100), this, 0);
					if(resCnt)
					{
						if(VersionToNumber(m_mPlayer->get_versionInfo()) > 8)
						{
							m_PlayerEnabled = TRUE;
							IWMPSettings * settings = (IWMPSettings *)m_mPlayer->get_settings();
							settings->put_enableErrorDialogs(0);
						}
					}
					else
					{
						delete m_mPlayer;
						m_mPlayer = 0;
					}
				}//end make sure control exists on machine
				
				if(!m_startInFullscreen)
					m_engineClient = new CClientModule(windowHandle,0,0,(m_windowSz.right-m_windowSz.left),(m_windowSz.bottom-m_windowSz.top),32,FALSE,FALSE,TRUE,TRUE,FALSE/*processScripts: this app runs GameFiles\<game>.aut itself*/,TRUE,m_maxTextureSize,m_shadowCount,FALSE);
				else
					m_engineClient = new CClientModule(windowHandle,0,0,m_width,m_height,m_depth,TRUE,FALSE,TRUE,TRUE,FALSE/*processScripts: this app runs GameFiles\<game>.aut itself*/,TRUE,m_maxTextureSize,m_shadowCount,FALSE);

				//m_engineClient->m_initObject->m_gStatus->m_dxRenderer->EnableManagerCallback();
				if(m_engineClient->m_initObject->m_gStatus)
				{
					m_axLogger.WriteDebugFile("Init Client Complete",0,1);
					m_running = TRUE;
					DWORD threadID = 0;
					m_rSp = TRUE;
					SetTimer(1,100,0);
					m_lgThreadHandle = CreateThread(NULL,0,ValidateDownloadFiles,this,0,&threadID);//call validate/dowloader
				}
			}//end not the engine
		}//end not running
	}//end valid windowHandleTest
	m_axLogger.WriteDebugFile("Init Client Exit",0,1);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if(!CFrameWnd::PreCreateWindow(cs))
		return FALSE;

	if(!m_startInFullscreen)
	{
		//Create a RECT using current height / width
		RECT adjustSize;
		adjustSize.bottom = 0;
		adjustSize.top = m_height;
		adjustSize.left = m_width;
		adjustSize.right = 0;

		//Adjust the RECT for Window Decorations
		AdjustWindowRect(&adjustSize,WS_CAPTION|WS_SYSMENU|WS_THICKFRAME,false);

		//Change the Width and Height globally
		m_width += (m_width - adjustSize.left) + 6;
		m_height += (m_height - adjustSize.top) + 5;
	}

	cs.cx = m_width;
	cs.cy = m_height;

	//m_width				= 1024;
	//m_height			= 768;
	//TODO: Modify the Window class or styles here by modifying
	// the CREATESTRUCT cs

	cs.style = WS_CAPTION | WS_SYSMENU;//| WS_CAPTION | FWS_ADDTOTITLE;

	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}


//CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

//CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	//forward focus to the view window
	//m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	//let the view have first crack at the command
	//if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
	//	return TRUE;

	//otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnTimer(UINT_PTR nIDEvent)
{
	if(!m_engineClient)
		return;

	m_engineClient->m_initObject->m_userControls->MouseManagerCallback();

	if(m_PlayerEnabled)
	if(m_engineClient->m_initObject->m_gStatus->m_soundTrackList)
	{//player enabled
		EnterCriticalSection(&m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_mSection);

		this->m_pendingUrl = m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_pendingURL;
		this->m_pendingUrlShouldLoop = m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_pendingUrlShouldLoop;

		if(m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer)
			m_pendingVolume = m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_currentVolume;

		LeaveCriticalSection(&m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_mSection);

		if(m_pendingUrl.GetLength() > 0)
		{//streaming music time
			if(m_pendingUrl.CompareNoCase("stop")==0)
			{
				Stop();
			}
			else
			{
				CString descBuffer,
						descUrl = m_pendingUrl;
				/*
				if(ReadUrl(descUrl, descBuffer)==true)
				{
					if(m_engineClient->m_initObject->m_gStatus->m_systemReadout)
					m_engineClient->m_initObject->m_gStatus->m_systemReadout->AddLine(descBuffer,MENU_TEXTCOLOR_NORMAL);
				}
				*/
				if(!PlayURL(m_pendingUrl.GetBuffer(), this->m_pendingUrlShouldLoop))
				{
					m_PlayerEnabled = FALSE;
					return;
				}

				// SETUP for initializing music volume settings
				POSITION pos = NULL;
				ControlDBObj *pControl = NULL;
				CMovementObj *pMovement = NULL;
				int iVolume = 0;

				// LOAD the movement object
				pos = m_engineClient->m_initObject->m_gStatus->m_entObjRuntime->FindIndex(0);
				if(pos)
					pMovement = (CMovementObj*)m_engineClient->m_initObject->m_gStatus->m_entObjRuntime->GetAt(pos);

				// CREATE filename for reading settings
				CString strFileName = KEY_CONFIG_FILE;
				strFileName += ".";
				strFileName += m_engineClient->m_initObject->m_gStatus->m_cabLib.ConvToString(pMovement->m_uniqueId);

				// INIT player preferences from disk or default
				bool bResult = m_engineClient->m_initObject->m_gStatus->LoadControlCfgFromDisk(strFileName, NULL);	

				// GET the control position
				pos = m_engineClient->m_initObject->m_gStatus->m_controlsList->FindIndex(0);

				// GET control object and volume level
				if (pos)
				{
					pControl = (ControlDBObj*)m_engineClient->m_initObject->m_gStatus->m_controlsList->GetAt(pos);
					if (pControl)
						iVolume = pControl->m_musicVolume;
				}

				// SET the music volume
				this->SetVolume(iVolume * VOLUME_SCALED_MAX);
				m_engineClient->m_initObject->m_gStatus->m_soundTrackList->SetVolume(iVolume * VOLUME_SCALED_MAX);
				m_pendingVolume = iVolume;
			}

			//handle volume
			EnterCriticalSection(&m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_mSection);
			m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_currentURL =  m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_pendingURL;
			m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_pendingURL = "";
			m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->SetMusicStreamerEnable(TRUE);
			LeaveCriticalSection(&m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_mSection);
		}//end streaming music time


		if(m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer)
		{
			if(m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->GetMusicStreamerStatus())
			{
				if(m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->GetDisabledStatus() != m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->GetCurDisabledStateStatus())
				{
					Stop();
					m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_trackInUse = -1;
					//m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_disabledCurState = m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->m_disabled;
					m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->SetDisabledCurState(m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->GetDisabledStatus());
				}

				if(m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->VolumeUpdateNeeded())
				{
					long volume = m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->GetVolume();
					SetVolume(volume);
					m_engineClient->m_initObject->m_gStatus->m_soundTrackList->m_musicPlayer->UpdateLastSetVolume(volume);
				}
			}
		}
	}//end player enabled


	if(m_engineClient->m_initObject->m_gStatus->GetCloseDownCallState())
	{
		/*
		if(m_mPlayer)
		if(m_PlayerEnabled)
		{
			Stop();
			delete m_mPlayer;
			m_mPlayer = 0;
			m_PlayerEnabled = FALSE;
		}
		*/

		CFrameWnd::PostMessage(WM_CLOSE,0,0);
		KillTimer(1);
		//exit(1);
		// CFrameWnd::PostMessage(CFrameWnd/*m_engineClient->m_initObject->m_gStatus->m_dxRenderer->m_topWindowHandle*/, WM_CLOSE, 0, 0 );
	}

	// CALL base class function
	CFrameWnd::OnTimer(nIDEvent);
}

//------------------Direct X Version detection---------------------//
BOOL CMainFrame::DetectDirectXVersionReq(DWORD minVersion,CString errorMsg)
{
    BOOL requirementMet			= FALSE;
	HRESULT						hr;
    //TCHAR						trResult[128];
    DWORD						dwDirectXVersion = 0;
    TCHAR						strDirectXVersion[10];
	GETDXverObj					verObj;
    hr = verObj.GetDXVersionPub( &dwDirectXVersion, strDirectXVersion, 10 );
    if( SUCCEEDED(hr) )
    {
        //if( dwDirectXVersion >= 0x00090003)
        //   requirementMet = TRUE;
		if( dwDirectXVersion >= minVersion)
            return TRUE;
    }

    if(!requirementMet)
	{
		if(errorMsg.GetLength() > 0)
           AfxMessageBox(errorMsg);
	}
	return FALSE;
}
void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);

	if(m_running)
	{
		if (WA_INACTIVE == nState)
		{
	        m_engineClient->m_initObject->m_userControls->UnacquireInputDevices();
		}
		/*else
		{
	        m_engineClient->m_initObject->m_userControls->AcquireInputDevices();
		}*/
	}
}

BOOL CMainFrame::Stop()
{
	if(!m_mPlayer)
	    return FALSE;
	
	IWMPControls * controls = (IWMPControls *)m_mPlayer->get_controls();
	if(controls)
	   controls->stop();
	return TRUE;
}

BOOL 
CMainFrame::PlayURL(char *url, VARIANT_BOOL loop)
{
	if(!m_mPlayer)
	    return FALSE;

	HRESULT errorRes;
	BOOL result = FALSE;
	BSTR strLoop = ::SysAllocString(L"loop");
	BSTR strAutoRewind = ::SysAllocString(L"autoRewind");

	IWMPSettings *settings = (IWMPSettings*)m_mPlayer->get_settings();
	if(settings)
	{
		//if (loop == VARIANT_TRUE)
			errorRes = settings->setMode(strLoop, loop);

		if (SUCCEEDED(errorRes) 
		&& this->_PlayURL(url))
			result = TRUE;
	}

	::SysFreeString(strLoop);
	::SysFreeString(strAutoRewind);
	return result;
}

BOOL CMainFrame::_PlayURL(char * url)
{
	if(!m_mPlayer)
	    return FALSE;

	Stop();
	LPDISPATCH dispatch = m_mPlayer->newMedia(url);
	if(dispatch)
	{
	   m_mPlayer->put_currentMedia(dispatch);
	   IWMPControls * controls = (IWMPControls *)m_mPlayer->get_controls();
	   if(controls)
	      controls->play();
	   return TRUE;
	}
	return FALSE;
}

BOOL CMainFrame::SetVolume(int volume)
{
	if(!m_mPlayer)
	    return FALSE;
	
	//IWMPControls * controls = (IWMPControls *)m_mPlayer.get_controls();
	IWMPSettings * settings = (IWMPSettings *)m_mPlayer->get_settings();
	if(settings)
	{
	   HRESULT errorRes = settings->put_volume(volume);
	   return TRUE;
	}
	return FALSE;
}

int CMainFrame::VersionToNumber(CString stringVer)
{
    int length = stringVer.GetLength();
    if(length == 0)
	   return 0;

    CString build = "";
	for(int loop=0;loop<length;loop++)
	{
        if(stringVer.GetAt(loop) == '.')
		   break;

		build += stringVer.GetAt(loop);
	}

    return atoi(build);
}

//-----------------------------------------------------------//
DWORD WINAPI ValidateDownloadFiles(LPVOID lpParameter)
{	   	    
	CMainFrame * classObj = (CMainFrame *)lpParameter; 
	if(FAILED(CoInitialize(NULL)))
		return 0;

	classObj->m_axLogger.WriteDebugFile("Check VS",0,1);

	EnterCriticalSection(&classObj->m_updateCriticalSection);
	classObj->m_upgradeThreadWorking = TRUE;
	while(classObj->m_ready == FALSE)
	{
		LeaveCriticalSection(&classObj->m_updateCriticalSection);
		Sleep(1000);
		classObj->m_axLogger.WriteDebugFile("Waiting for Download URL:",0,1);
		//ABORT
		EnterCriticalSection(&classObj->m_updateCriticalSection);
	}
	LeaveCriticalSection(&classObj->m_updateCriticalSection);

	classObj->m_axLogger.WriteDebugFile("SET AUTH",0,1);
		
	//set authentication data
	classObj->m_contentDownloadManager->SetUsernamePassword(classObj->m_username,classObj->m_password);
	classObj->m_engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_userName = classObj->m_username;
	classObj->m_engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_passWord = classObj->m_password;
	classObj->m_engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_IPaddress = classObj->m_gserverIP;
	// No content server in this client: the loading image, font texture, font palette and
	// "adv" images all came from contentmanager.aspx in 2008. Every one of those is an HTTP
	// fetch that blocks the start-up thread when the host answers slowly (or not at all).
	// Everything loads from GameFiles; the launcher is the only thing that downloads.
	classObj->m_engineClient->m_initObject->m_gStatus->m_contentUrl = "";

    classObj->m_axLogger.WriteDebugFile("Set download url:",0,0);
	classObj->m_axLogger.WriteDebugFile(classObj->m_downloadURL.GetBuffer(),0,1);

	classObj->m_engineClient->m_initObject->m_gStatus->m_advUrl = "";
	classObj->m_engineClient->m_initObject->m_gStatus->m_advUrlBasic = "";
	classObj->m_engineClient->m_initObject->m_gStatus->m_advUrlVending = "";
	classObj->m_engineClient->m_initObject->m_gStatus->m_installPath = classObj->m_installPath;
	classObj->m_engineClient->m_initObject->m_gStatus->m_cacheDirectory = classObj->m_cacheDirectory;
	// Content is NOT fetched by the client any more: the launcher/updater keeps the
	// install current before the game starts, and missing zones are reported, not
	// downloaded mid-session (that behaviour is what players disliked about the
	// later client).
	classObj->m_engineClient->m_initObject->m_gStatus->m_autoDownloadMissingAssets = FALSE;
	classObj->m_engineClient->m_initObject->m_gStatus->m_contentDownloadManager->SetUsernamePassword(classObj->m_username,classObj->m_password);
	classObj->m_engineClient->m_initObject->m_gStatus->m_contentDownloadManager->SetCacheDirectory(classObj->m_cacheDirectory);
    classObj->m_engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_ingameNameRef = classObj->m_game;
	classObj->m_axLogger.WriteDebugFile("Set Game Name Ref:",0,0);
	classObj->m_axLogger.WriteDebugFile(classObj->m_engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_ingameNameRef.GetBuffer(),0,1);

	classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeterAdv(75.0f,"Initializing...");
	//set Game specific data
    SetCurrentDirectory(classObj->m_installPath);
	classObj->m_axLogger.WriteDebugFile(classObj->m_installPath.GetBuffer(),0,1);
	if(!classObj->m_contentUpdatesInClient)
		classObj->m_axLogger.WriteDebugFile("Content updates are handled by the launcher - skipping in-client download pass",0,1);
	if(classObj->m_contentUpdatesInClient)
	{//---- in-client content validate/download pass (launcher does this now) ----
	CString contentUrl = classObj->m_downloadURL + "/contentmanager.aspx?contenttype=init&gametype=" + classObj->m_game;
	CString ipTcpServer = "";
	CString tcpIPUrl   = classObj->m_downloadURL + "/contentmanager.aspx?tcpserver=1";
	CString tcpPortUrl = classObj->m_downloadURL + "/contentmanager.aspx?tcpserver=2";
	long tcpPort = 0;
		
	CString temp = "";
	CString portString;
	if(classObj->ReadUrl(tcpIPUrl, ipTcpServer)==false)
	{//error
		classObj->m_axLogger.WriteDebugFile(tcpIPUrl.GetBuffer(),0,0);
		classObj->m_axLogger.WriteDebugFile(":FAILED ReadUrl() tcpIPUrl trying default",0,1);
		classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeterAdv(100.0f,"tcpIPUrl READ FAILED:"+classObj->m_downloadURL);
		ipTcpServer = "12.11.1.188";
		//Sleep(1000);
	}//end error
	classObj->m_axLogger.WriteDebugFile(tcpIPUrl.GetBuffer(),0,0);
	classObj->m_axLogger.WriteDebugFile(" tcpIPUrl patcher:",0,0);
	classObj->m_axLogger.WriteDebugFile(ipTcpServer.GetBuffer(),0,1);
	if(classObj->ReadUrl(tcpPortUrl, temp)==false)
	{//error
		classObj->m_axLogger.WriteDebugFile(tcpPortUrl.GetBuffer(),0,0);
		classObj->m_axLogger.WriteDebugFile(":FAILED ReadUrl() tcpPortUrl",0,1);
		classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeterAdv(100.0f,"tcpPortUrl READ FAILED:"+classObj->m_downloadURL);
		temp = "1050";
		//Sleep(1000);
	}//end error
	classObj->m_axLogger.WriteDebugFile("tcpPortUrl patcher port:",0,0);
	classObj->m_axLogger.WriteDebugFile(temp.GetBuffer(),0,1);

	portString = temp;
	tcpPort = atoi(temp);

	DWORD returnLength = 0;
	char * buffer = NULL;
	CHttpFileDownloader * urlReader = new CHttpFileDownloader();
	urlReader->m_asciMode = TRUE;

	if(classObj->m_devMode==0)
	{
		if(urlReader->GetUrlData("iexplorer data",contentUrl.GetBuffer(),(BYTE**)&buffer,&returnLength))
		{//succeeded
			classObj->m_axLogger.WriteDebugFile("GetUrlData() Success",0,1);
			CString startupFiles = buffer;
			if(buffer)
				delete [] buffer;
			buffer = 0;
		       
			int	retryMax				= 0;//zero is disabled
			CString	fileNameTemp		= ""; 
			long fileSizeTemp			= 0;
			CString	cabNameTemp			= "";
			CString	offsetDirTemp		= ""; 
			unsigned long CRCTemp		= 0;
			unsigned long CAB_CRCTemp	= 0;
			int currentSet				= 0;
			BOOL checkCrc				= TRUE;
			BOOL abort					= FALSE;
			unsigned long crcCheckRef	= 0;
			classObj->m_engineClient->m_initObject->m_gStatus->m_devMode = classObj->m_devMode;
			
			while(classObj->ParseUrlStringToData(currentSet, 
												 startupFiles,//CString urlParse, 
												 fileNameTemp, 
												 &fileSizeTemp, 
												 cabNameTemp, 
												 offsetDirTemp, 
												 &CRCTemp,
												 &CAB_CRCTemp,
												 &checkCrc))
			{//validate or download
				CString fullFilePath = classObj->m_installPath + offsetDirTemp + fileNameTemp;
				classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeterAdv(75.0f,"Verifying:"+cabNameTemp);
				int retryCurrent = 0;
				crcCheckRef = 0;
				if(classObj->ValidateFile(fullFilePath, CRCTemp, checkCrc, &crcCheckRef)==false && classObj->m_devMode == 0)
				{//file needs to be redownloaded
					CString verifyMsg;
					verifyMsg.Format("File needs update %s svr:%u file:%u",fileNameTemp,CRCTemp,crcCheckRef);
					classObj->m_contentDownloadManager->m_loggingSystem.WriteDebugFile(verifyMsg.GetBuffer(),0,1);
						
					if(classObj->m_contentDownloadManager->DownloadFileRequest(cabNameTemp,ipTcpServer,tcpPort)==1)
					{//success to start thread for downloading
						DWORD timoutMsgStamp = GetTickCount();
						while(classObj->m_contentDownloadManager->GetPercentageOfDownload() < 100)
						{						
							EnterCriticalSection(&classObj->m_updateCriticalSection);
							if(classObj->m_contentDownloadManager->m_downloadInProgress == false)
							{
								if(GetTickCount()-timoutMsgStamp > 10000)
								{
									classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeterAdv(100.0,classObj->m_contentDownloadManager->m_errorMessage);
								    
									abort = classObj->m_closeDown;
									if(abort)
									{
										classObj->m_upgradeThreadWorking = FALSE;
										LeaveCriticalSection(&classObj->m_updateCriticalSection);
										return 0;
									}
								}
							}
							else
							{
								classObj->m_percentagePub = classObj->m_contentDownloadManager->GetPercentageOfDownload();
								if(classObj->m_contentDownloadManager->m_errorMessage.GetLength()<2)
								{
									CString kb_sec = classObj->m_engineClient->m_initObject->m_gStatus->m_cabLib.ConvToString(classObj->m_contentDownloadManager->GetKBSec()*8);
									classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeterAdv((float)classObj->m_percentagePub,"kb: "+kb_sec+" Downloading:"+cabNameTemp);
								}
								else
									classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeterAdv((float)classObj->m_percentagePub,classObj->m_contentDownloadManager->m_errorMessage+cabNameTemp);								
							}
							
							classObj->m_engineClient->m_initObject->m_gStatus->m_dxRenderer->EnableMouse();
							abort = classObj->m_closeDown;
							LeaveCriticalSection(&classObj->m_updateCriticalSection);

							if(abort == TRUE)
							{
							   EnterCriticalSection(&classObj->m_updateCriticalSection);
							   classObj->m_contentDownloadManager->Abort();
							   classObj->m_contentDownloadManager->WaitForThreadToClose();
							   classObj->m_upgradeThreadWorking = FALSE;
							   LeaveCriticalSection(&classObj->m_updateCriticalSection);
							   return 0;
							}
							Sleep(10);
						}
						//Validate Download

						EnterCriticalSection(&classObj->m_updateCriticalSection);
						classObj->m_percentagePub = classObj->m_contentDownloadManager->GetPercentageOfDownload();
						classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeter((float)classObj->m_percentagePub);
						classObj->m_engineClient->m_initObject->m_gStatus->LoadSceneInfo();
						classObj->m_contentDownloadManager->Reset();
						int tryCount = 0;
						LeaveCriticalSection(&classObj->m_updateCriticalSection);
						BOOL resultFromDelete = 0;//DeleteFile(classObj->m_saveLocationWFile);//delete temp file
						//validate cabinet
						crcCheckRef = 0;
						classObj->m_saveLocationWFile = classObj->m_cacheDirectory + cabNameTemp;
						if(classObj->ValidateFile(classObj->m_saveLocationWFile, CAB_CRCTemp, TRUE, &crcCheckRef)==false)
						{//Cabinet failed
							CString errorCabCrc;
							errorCabCrc.Format("CAB CRC FAILED %s svr:%u file:%u",fileNameTemp,CRCTemp,crcCheckRef);
							classObj->m_contentDownloadManager->m_loggingSystem.WriteDebugFile(errorCabCrc.GetBuffer(),0,1);
							if(retryCurrent < retryMax)
							{
								resultFromDelete = DeleteFile(classObj->m_saveLocationWFile);//delete temp file
								while(!resultFromDelete)
								{//delete failedfullFilePath
									classObj->m_contentDownloadManager->m_loggingSystem.WriteDebugFile(classObj->m_saveLocationWFile.GetBuffer(),0,0);
									classObj->m_contentDownloadManager->m_loggingSystem.WriteDebugFile("Delete Failed (CRC SECTION)!!",0,1);
									resultFromDelete = DeleteFile(classObj->m_saveLocationWFile);//delete temp file
									Sleep(50);
									tryCount++;
									if(tryCount > 10)
										break;
								}//end delete failed
								currentSet--;
								retryCurrent++;
								continue;
							}
						}//end cabinet failed
						//extract cabinet to target directory

						//CCabinetObj newCabObj;
						CResourceCabinet newCabObj;
						newCabObj.Init();
						newCabObj.CreateFDIContext();
						newCabObj.SetProgressCountRef(&classObj->m_percentagePub);
						newCabObj.SetCriticalSection(&classObj->m_updateCriticalSection);
						
						newCabObj.Copy(classObj->m_saveLocationWFile.GetBuffer(),classObj->m_installPath,NULL);
						Sleep(500);//let it digest
						tryCount = 0;
						//handle bad download
						crcCheckRef = 0;
						classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeterAdv(97,"Verifying Download");
						if(classObj->ValidateFile(fullFilePath, CRCTemp, checkCrc, &crcCheckRef)==false && classObj->m_devMode == 0)
						{//file extracted badly
							CString errorCabCrc;
							errorCabCrc.Format("FILE CRC FAILED %s svr:%u file:%u",fileNameTemp,CRCTemp,crcCheckRef);
							classObj->m_contentDownloadManager->m_loggingSystem.WriteDebugFile(errorCabCrc.GetBuffer(),0,1);
							if(retryCurrent < retryMax)
							{
								currentSet--;
								classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeterAdv(100.0f,"File Corruption Error:"+fullFilePath);
								resultFromDelete = DeleteFile(fullFilePath);
								if(!resultFromDelete)
								{
									classObj->m_contentDownloadManager->m_loggingSystem.WriteDebugFile(fullFilePath.GetBuffer(),0,0);
									classObj->m_contentDownloadManager->m_loggingSystem.WriteDebugFile("Delete Failed!!",0,1);
								}
							}
						}//end file extracted badly
						//clean up cabinet
						resultFromDelete = DeleteFile(classObj->m_saveLocationWFile);//delete temp file
						while(!resultFromDelete)
						{//delete failedfullFilePath
							classObj->m_contentDownloadManager->m_loggingSystem.WriteDebugFile(classObj->m_saveLocationWFile.GetBuffer(),0,0);
							classObj->m_contentDownloadManager->m_loggingSystem.WriteDebugFile("Delete Failed!!",0,1);
							resultFromDelete = DeleteFile(classObj->m_saveLocationWFile);//delete temp file
							Sleep(50);
							tryCount++;
							if(tryCount > 10)
								break;
						}//end delete failed					
					}//end success to start thread for downloading
				}//end file needs to be redownloaded
				currentSet++;
			}//end validate or download
		}//end succeded
		else
		{
			classObj->m_axLogger.WriteDebugFile("Cannot connect to content Server",0,1);
			classObj->m_engineClient->m_initObject->m_gStatus->UpdateMeterAdv(100.0f,"Cannot connect to content Server  GAME:"+classObj->m_game);
			Sleep(1000);
		}
	}
	    
	delete urlReader;
	urlReader = 0;
	//load game specific data

	}//---- end in-client content validate/download pass ----
	//reload language filter
	if(classObj->m_engineClient->m_initObject->m_gStatus->m_langFilter)
		delete classObj->m_engineClient->m_initObject->m_gStatus->m_langFilter;
	classObj->m_engineClient->m_initObject->m_gStatus->m_langFilter  = 0;

	classObj->m_engineClient->m_initObject->m_gStatus->m_langFilter = new CAXLanguageFilter();

	classObj->m_axLogger.WriteDebugFile("Pre Run Script()",0,1);
	classObj->m_engineClient->m_initObject->m_gStatus->m_multiplayerObject->m_currentVersion = 252;
	classObj->m_engineClient->m_initObject->m_gStatus->RunScript(classObj->m_installPath + "GameFiles\\" + classObj->m_game + ".aut");//load from scripts
	//end load game specific data
	EnterCriticalSection(&classObj->m_updateCriticalSection);
	classObj->m_rSp = FALSE;
	LeaveCriticalSection(&classObj->m_updateCriticalSection);

	char bufferXY[_MAX_PATH];
	_getcwd( bufferXY, _MAX_PATH );
	classObj->m_axLogger.WriteDebugFile(bufferXY,0,1);

	classObj->m_axLogger.WriteDebugFile("Trying to connect....",0,1);
	//create render thread

	classObj->m_rnThreadHandle = CreateThread(NULL,0,RenderSpinner,classObj,0,&classObj->m_rnThreadID);//call validate/dowloader

	EnterCriticalSection(&classObj->m_updateCriticalSection);
	classObj->m_upgradeThreadWorking = FALSE;
	LeaveCriticalSection(&classObj->m_updateCriticalSection);

	return 1;
}

//-----------------------------------------------------------//
DWORD WINAPI RenderSpinner(LPVOID lpParameter)
{	   
	CMainFrame * classObj = (CMainFrame *)lpParameter; 
	if(FAILED(CoInitialize(NULL)))
		return 0;

	classObj->m_engineClient->m_initObject->m_gStatus->m_dxRenderer->EnableManagerCallback();
	classObj->m_engineClient->m_initObject->m_gStatus->m_dxRenderer->DisableMouse();

	classObj->m_renderSpinner = TRUE;
	while(1)
	{
		EnterCriticalSection(&classObj->m_updateCriticalSection);
		if(classObj->m_closeDown == TRUE)
		{
			classObj->m_closeDown = FALSE;
			LeaveCriticalSection(&classObj->m_updateCriticalSection);
			break;   
		}
		LeaveCriticalSection(&classObj->m_updateCriticalSection);

		classObj->m_engineClient->CallBackTimeOpen();
		//classObj->m_engineClient->m_initObject->m_gStatus->m_dxRenderer->MouseManagerCallback();
		classObj->m_engineClient->ClientCallback();
		classObj->m_engineClient->CallBackTimeCap();
	}

	return 0;
}

//-----------------------------------------------------------//
bool CMainFrame::ReadUrl(CString & url, CString & retString)
{
	if(url.GetLength() > 0)
		if(url.GetAt(0) != 'h' && url.GetAt(0) != 'H')
			return true;

	DWORD returnLength = 0;
	char * buffer = NULL;
	CHttpFileDownloader * urlReaderTCPPort = new CHttpFileDownloader();
	urlReaderTCPPort->m_asciMode = TRUE;
	if(urlReaderTCPPort->GetUrlData("iexplorer data",url.GetBuffer(),(BYTE**)&buffer,&returnLength))
	{
		retString = buffer;
		if(buffer)
			delete [] buffer;
		buffer = 0;
		
		delete urlReaderTCPPort;
		urlReaderTCPPort = 0;
		
		return true;
	}
	
	delete urlReaderTCPPort;
	urlReaderTCPPort = 0;
	
	return false;
}
//-----------------------------------------------------------//
bool CMainFrame::ParseUrlStringToData(int set, 
									  CString urlParse, 
									  CString & fileName, 
									  long * fileSize, 
									  CString & cabName, 
									  CString & offsetDir, 
									  unsigned long * CRC,
									  unsigned long * cab_CRC,
									  BOOL * checkCrc)
{
	int lineSkip = 7*set;
	int currentPos = 0;
	int currentLine = 0;
	CString currentLineData = "";
	int lengthTotal = urlParse.GetLength();
	int varIndex = 0;
    /*while(ParseUrlGetLine(currentPos, urlParse, currentLineData))
	{
        if(currentLine == lineSkip)
		   break;

		currentLine++;
	}*/

    while(ParseUrlGetLine(currentPos, urlParse, currentLineData))
	{
        if(currentLine < lineSkip)
		{
		   currentLine++;
		   continue;
		}

		if(varIndex == 0)
           fileName	 = currentLineData;
		else if(varIndex == 1)
           *fileSize  = (long)atol(currentLineData.GetBuffer());
		else if(varIndex == 2)
           cabName	 = currentLineData;
		else if(varIndex == 3)
		   offsetDir = currentLineData;
		else if(varIndex == 4)
		{
		   *CRC		 = (unsigned long)_atoi64(currentLineData.GetBuffer());
		}
		else if(varIndex == 5)
		{
		   *checkCrc		 = (BOOL)atoi(currentLineData.GetBuffer());//file CRC
		}
		else if(varIndex == 6)
		{
		   *cab_CRC	 = (unsigned long)_atoi64(currentLineData.GetBuffer());//cab crc
		   return true;
		}
		currentLine++;
		varIndex++;
	}
	return false;
}
//-----------------------------------------------------------//
bool CMainFrame::ValidateFile(CString & fullPath, unsigned long fileCRC, BOOL checkCrc, unsigned long * dynGenCrc)
{
	CFileException exc;
	CFile fl;
	BOOL res = fl.Open(fullPath,CFile::modeRead, &exc);
	if(res == FALSE)
	{
		if(exc.m_cause == CFileException::fileNotFound)
			return false;
		
		Sleep(50);
		res = fl.Open(fullPath,CFile::modeRead, &exc);
		
		if(!res)
			return false;
	}
	//validate file size
	if(!checkCrc)
		return true;

	unsigned long intermCrc = m_crcGenerator.getcrc(&fl);
	fl.Close();

	if(dynGenCrc)
		*dynGenCrc = intermCrc;

	if(intermCrc==fileCRC)
		return true;

	return false;
}

//-----------------------------------------------------------//
bool CMainFrame::ParseUrlGetLine(int & curPos, CString & urlData, CString & lineString)
{
	int length = urlData.GetLength();
	lineString = "";
	while(curPos < length)
	{
		if(urlData.GetAt(curPos) == '\r')
		{
			curPos++;
			curPos++;
			return true;
		}
		lineString += urlData.GetAt(curPos);
		curPos++;
	} 
	return false;
}
void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(m_engineClient)
	   m_engineClient->m_initObject->m_gStatus->m_dxRenderer->HideMouseByRectClick();

	return;

	CFrameWnd::OnLButtonDown(nFlags, point);
}
//-----------------------------------------------------------//
BOOL CMainFrame::GetInfoFromCoinitFile(CString & username,
									   CString & password,
									   CString & gameName,
									   CString & contentServerIP,
									   CString & gameServerIP,
									   BOOL & fullScreen,
									   BOOL & saveLogonData,
									   int & width,
									   int & height,
									   int & depth)
{
	CFileException exc;
	CFile fl;
	BOOL res = fl.Open("coinit.tmp",CFile::modeRead, &exc);
	if(res)
	{
		ULONGLONG length = fl.GetLength();
		char * buf = new char[length+1];
		ZeroMemory(buf,length+1);
		fl.Read(buf,length);
		fl.Close();
		ULONGLONG pos = 0;

		username = m_xEncrypt->DecryptString64(ParseLine(pos,buf,length,1).GetBuffer());
		password = m_xEncrypt->DecryptString64(ParseLine(pos,buf,length,1).GetBuffer());
		gameName = m_xEncrypt->DecryptString64(ParseLine(pos,buf,length,1).GetBuffer());
		contentServerIP = m_xEncrypt->DecryptString64(ParseLine(pos,buf,length,1).GetBuffer());
		gameServerIP = m_xEncrypt->DecryptString64(ParseLine(pos,buf,length,1).GetBuffer());
        fullScreen = atoi(ParseLine(pos,buf,length,0));
		saveLogonData = atoi(ParseLine(pos,buf,length,0));
		CString widthStr = ParseLine(pos,buf,length,0);
		width = atoi(widthStr);
		CString heightStr = ParseLine(pos,buf,length,0);
		height = atoi(heightStr);
		CString depthStr = ParseLine(pos,buf,length,0);
		depth = atoi(depthStr);

		delete [] buf;
		buf = 0;

		if(saveLogonData == 0)
		{//clear file
           if(!WriteCoinitFile(username,
							   "",
							   gameName,
							   contentServerIP,
							   gameServerIP,
							   fullScreen,
							   saveLogonData,
							   widthStr,
							   heightStr,
							   depthStr))
							   m_axLogger.WriteDebugFile("ERROR 76::Could Not Clear Coinit File",0,1);

		}//end clear file
		return TRUE;
	}
	 
	return FALSE;	 
}
//-----------------------------------------------------------//
CString CMainFrame::ParseLine(ULONGLONG & pos,char * Buf, ULONGLONG length, int sqCount)
{
	CString build;
	int curLineC = 0;
	while(pos < length-1)
	{
		build += Buf[pos];
		pos++;

		if(Buf[pos] == '\r')
		{
			if(Buf[pos+1] == '\n')
			{
				curLineC++;
				if(curLineC > sqCount)
					break;
			}
		}
	}
	pos++;
	pos++;
	return build;
}
//-----------------------------------------------------------//
BOOL CMainFrame::WriteCoinitFile(CString username,
								 CString password,
								 CString game,
								 CString downloadUrl,
								 CString gameserverIP,
								 BOOL fullscreen,
								 BOOL saveLogonData,
								 CString wResolution,
								 CString hResolution,
								 CString depth)
{
	CFileException exc;
	CFile fl;
	BOOL res = fl.Open("coinit.tmp",CFile::modeCreate | CFile::modeWrite, &exc);
	if(!res)
	{
		Sleep(2000);
		res = fl.Open("coinit.tmp",CFile::modeCreate | CFile::modeWrite, &exc);
	} 
	if(!res)
	{//failed to create temp file
		m_axLogger.WriteDebugFile("ERROR 21::could not create coinit.tmp file",0,1);
		return FALSE;
	}//end failed to create temp file
	else
	{
		//username
		CString tempStr,newLine;
		newLine = "\r\n";
		CString tempWrite = m_xEncrypt->EncryptString64(username.GetBuffer());
		fl.Write(tempWrite.GetBuffer(),tempWrite.GetLength());
		fl.Write(newLine.GetBuffer(),newLine.GetLength());
		//password
		tempWrite = m_xEncrypt->EncryptString64(password.GetBuffer());
		fl.Write(tempWrite.GetBuffer(),tempWrite.GetLength());
		fl.Write(newLine.GetBuffer(),newLine.GetLength());
		//game
		tempWrite = m_xEncrypt->EncryptString64(game.GetBuffer());
		fl.Write(tempWrite.GetBuffer(),tempWrite.GetLength());
		fl.Write(newLine.GetBuffer(),newLine.GetLength());
		//download URL
		tempWrite = m_xEncrypt->EncryptString64(downloadUrl.GetBuffer());
		fl.Write(tempWrite.GetBuffer(),tempWrite.GetLength());
		fl.Write(newLine.GetBuffer(),newLine.GetLength());
		//Game Server IP
		tempWrite = m_xEncrypt->EncryptString64(gameserverIP.GetBuffer());
		fl.Write(tempWrite.GetBuffer(),tempWrite.GetLength());
		fl.Write(newLine.GetBuffer(),newLine.GetLength());
		//fullscreen
		tempWrite = "0";
		if(fullscreen == 1)
			tempWrite = "1";
		fl.Write(tempWrite.GetBuffer(),tempWrite.GetLength());
		fl.Write(newLine.GetBuffer(),newLine.GetLength());
		//save user logon data
		tempWrite = "0";
		if(saveLogonData==1)
			tempWrite = "1";
		fl.Write(tempWrite.GetBuffer(),tempWrite.GetLength());
		fl.Write(newLine.GetBuffer(),newLine.GetLength());
		//resolution parse
		//w resolution
		fl.Write(wResolution.GetBuffer(),wResolution.GetLength());
		fl.Write(newLine.GetBuffer(),newLine.GetLength());
		//H resolution
		fl.Write(hResolution.GetBuffer(),hResolution.GetLength());
		fl.Write(newLine.GetBuffer(),newLine.GetLength());
		//Bit Depth
		fl.Write(depth.GetBuffer(),depth.GetLength());
		fl.Write(newLine.GetBuffer(),newLine.GetLength());

		fl.Close();
		return TRUE;
	}
	return FALSE;
}
void CMainFrame::OnDestroy()
{
	CFrameWnd::OnDestroy();	
}
