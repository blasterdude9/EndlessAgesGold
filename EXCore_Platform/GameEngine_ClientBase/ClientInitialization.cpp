#include "StdAfx.h"
#include "getdxver.h"
#include  "clientinitialization.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
//use only for one time alloc that we need to access from menu
CRenderingManagement * rendermanager;


CClientInitialization::CClientInitialization(HWND & hwnd,
											 int startWidth,
											 int startHieght,
											 int widthLoc,
											 int hieghtLoc,
											 int depth, 
											 BOOL fullScreen, 
											 BOOL editOn, 
											 BOOL overrideRenderDev,
											 BOOL renderingEnabled,
											 BOOL processScripts,
											 BOOL initDirectInput,
											 int maxTextureSize,
											 int shadowCount,
											 BOOL sim)
{
	//zero for debug
	m_gStatus					= NULL;
	m_userControls				= NULL;
	m_clientNet			        = NULL;
	m_sceneAssembler			= NULL;
	m_menuManagement            = NULL;
	m_renderManager             = NULL;
	m_misExpManager             = NULL;
	m_aiManager                 = NULL;
	m_entityManager             = NULL;

	DWORD curVersion = 0x00090003;
	if(!sim)
    if(!DetectDirectXVersionReq(curVersion,"DirectX9c or better must be installed. Download DirectX from www.microsoft.com/directX"))
	    return;

    //Initialize Interfaces
	m_gStatus					= new GlobalStatusVariables(hwnd,editOn,sim);
    m_gStatus->ProssesCfgFile("startcfg.aut");
	// names above heads need the 3D font set (EndlessAges.ETF). Only the Phylon start-up
	// script loaded it, so in EARevolution every name had no font and drew nothing.
	m_gStatus->LoadTextFontInterfaceFromFile("EndlessAges.ETF");
	if(m_gStatus->m_dxRenderer)
	   m_gStatus->m_dxRenderer->m_maxTextureSize = maxTextureSize;
	m_gStatus->m_maxShadowCount = shadowCount;
	m_menuManagement            = new CMenuManagement(m_gStatus, m_sceneAssembler, m_clientNet);//MEM LEAK TEST
	m_userControls				= new UserController(hwnd, m_gStatus, m_menuManagement, initDirectInput);
	m_clientNet			        = new CClientNetworkingLogic(m_gStatus);
	m_sceneAssembler			= new SceneAssembly(m_gStatus);
	m_renderManager             = new CRenderingManagement(m_gStatus,m_sceneAssembler,m_clientNet,m_userControls,m_menuManagement);
	m_misExpManager             = new CMissileExplosionManager(m_gStatus,m_clientNet);
	m_aiManager                 = new CAiManager(m_gStatus,m_userControls);
	m_entityManager             = new CEntityManagement(m_gStatus,m_userControls,m_aiManager,m_clientNet);

	//external access for interface purposes-be carful and only use when absolutly necessary and never on server code
	rendermanager = m_renderManager;

    //Set up rendering surfaces
    if(overrideRenderDev)
	{
	   m_gStatus->m_dxRenderer->m_currentStartHeight	= startHieght;
	   m_gStatus->m_dxRenderer->m_currentStartWidth		= startWidth;
	   m_gStatus->m_dxRenderer->m_currentWidth			= widthLoc;
	   m_gStatus->m_dxRenderer->m_currentHeight			= hieghtLoc;
       m_gStatus->m_dxRenderer->m_defaultDepth			= depth;
	}
	if(renderingEnabled)
	{
		if(m_gStatus->m_dxRenderer->CreateRenderingDevices(!fullScreen,m_gStatus->m_dxRenderer->m_currentWidth,m_gStatus->m_dxRenderer->m_currentHeight,m_gStatus->m_dxRenderer->m_defaultDepth,hwnd)==1)
		{//success
		   m_gStatus->m_imediateRenderRECT = m_gStatus->m_dxRenderer->GetRenderRect();
		}//end success
		else
		{
		   renderingEnabled = FALSE;
		   AfxMessageBox("Device create failed");
		}
	}

	//aquire IP address
	m_gStatus->m_multiplayerObject->m_IPaddress = m_gStatus->m_multiplayerObject->GetIPDataFromDisk();
    //create event log
	if(!m_gStatus->m_multiplayerObject->m_eventLog)
        m_gStatus->m_multiplayerObject->m_eventLog = new CServerEventList();
	//debug startup
	CFileException e;
     if(!m_gStatus->m_debugfl.Open( "ErrorLive.txt", CFile::modeCreate | CFile::modeWrite, &e ))
		 m_gStatus->m_debuggingOn = FALSE;
	   
	 if(renderingEnabled)
	 {
      m_gStatus->SetAllRenderStatesIM();
      m_gStatus->m_3dDevice->SetTransform( D3DTS_WORLD, &m_gStatus->m_matrixInit2 );
	  
	  
	  CXMLFileIO getMeterCfg;
	  if(getMeterCfg.Open("meter.cfg"))
	  {
         float version = getMeterCfg.ReadAttribute();
         m_gStatus->SetupGlobalMeter(
									getMeterCfg.ReadAttribute(),//float length, 
									getMeterCfg.ReadAttribute(),//float width, 
									getMeterCfg.ReadAttribute(),//float xPos, 
									getMeterCfg.ReadAttribute(),//float yPos,
									getMeterCfg.ReadAttribute(),//float backRed,
									getMeterCfg.ReadAttribute(),//float backGreen,
									getMeterCfg.ReadAttribute(),//float backBlue,
									getMeterCfg.ReadAttribute(),//float backAlpha,
									getMeterCfg.ReadAttribute(),//float frontRed,
									getMeterCfg.ReadAttribute(),//float frontGreen,
									getMeterCfg.ReadAttribute(),//float frontBlue,
									getMeterCfg.ReadAttribute(),//float frontAlpha
									getMeterCfg.ReadAttribute(),//font size X
									getMeterCfg.ReadAttribute());//font size Y

	  }
	  else
	  m_gStatus->SetupGlobalMeter(   .375f,//float length, 
									.0325f,//float width, 
									-.1875f,//float xPos, 
									-.234f,//float yPos,
									0.1f,//float backRed,
									0.1f,//float backGreen,
									0.1f,//float backBlue,
									0.0f,//float backAlpha,
									0.0f,//float frontRed,
									0.4f,//float frontGreen,
									0.8f,//float frontBlue,
									1.0f,//float frontAlpha
									.006f,//font size X
									.007f);//font size Y
	  m_gStatus->LoadSceneInfo();//loading image
	 }
 	 if(processScripts)
	 {
	    if(!m_gStatus->RunScript("autoexec.aut"))//try running default script
	    {
	     if(!editOn)
	     {//client: nothing to show without the startup script - say so instead of a black screen
	        char cwd[MAX_PATH] = {0}; GetCurrentDirectoryA(MAX_PATH, cwd);
	        CString msg; msg.Format("autoexec.aut was not found in:\n%s\n\nThe client needs autoexec.aut (the startup script) next to EndlessAges.exe.", cwd);
	        m_gStatus->m_axLogger.WriteDebugFile("CLIENT: autoexec.aut missing - nothing loaded",0,1);
	        AfxMessageBox(msg, MB_ICONERROR);
	     }
	     if(editOn)//editor mode on
		 {//let user choose what autoexec to run
			 static char BASED_CODE filter[] = "Autoexec Files|*.aut|All Files (*.*)|*.*||";
			 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, NULL );
			 if ( opendialog.DoModal() == IDOK )
			 {
			    CString fileName = opendialog.GetPathName();
				m_gStatus->RunScript(fileName);
			 }
			 SetCurrentDirectory(m_gStatus->GL_workingDirectory);
		 }//end let user choose what autoexec to run
	    }//end autoexec.aut not found
	 }

    //close debug startup
    if(m_gStatus->m_debuggingOn)
	{
	   m_gStatus->m_debugfl.Close();
	   m_gStatus->m_debuggingOn = FALSE;
	}
	//init texture databases
	if(renderingEnabled)
	if(m_gStatus->m_textureDataBase)
	   m_gStatus->m_textureDataBase->InitReferences(m_gStatus->GL_pathMapModels,
													m_gStatus->m_3dDevice,
													m_gStatus->m_dxRenderer->m_maxTextureSizeSquared,
													m_gStatus->m_editorModeOn,
													m_gStatus->m_textureSearchPath,
													m_gStatus->m_searchPathCount,
													m_gStatus->m_compressTextures,
													m_gStatus->m_advUrlBasic,
										            m_gStatus->m_advUrlVending);
	if(renderingEnabled)
	if(m_gStatus->m_staticTextureDataBase)
	   m_gStatus->m_staticTextureDataBase->InitReferences(  m_gStatus->GL_pathMapModels,
															m_gStatus->m_3dDevice,
															m_gStatus->m_dxRenderer->m_maxTextureSizeSquared,
															m_gStatus->m_editorModeOn,
															m_gStatus->m_textureSearchPath,
															m_gStatus->m_searchPathCount,
															m_gStatus->m_compressTextures,
															m_gStatus->m_advUrlBasic,
										                    m_gStatus->m_advUrlVending);
	if(renderingEnabled)
	if(m_gStatus->m_dynamicTextureDataBase)
	   m_gStatus->m_dynamicTextureDataBase->InitReferences(m_gStatus->GL_pathMapModels,
															m_gStatus->m_3dDevice,
															m_gStatus->m_dxRenderer->m_maxTextureSizeSquared,
															m_gStatus->m_editorModeOn,
															m_gStatus->m_textureSearchPath,
															m_gStatus->m_searchPathCount,
															m_gStatus->m_compressTextures,
															m_gStatus->m_advUrlBasic,
										                    m_gStatus->m_advUrlVending);

	if(renderingEnabled)
	{
	m_gStatus->InitializeEnvironment(TRUE);
	//m_userControls->AcquireInputDevices();
	}

}

CClientInitialization::~CClientInitialization(void)
{
    if(m_gStatus)
	{
       delete m_gStatus;
	   m_gStatus = 0;
	}
    if(m_userControls)
	{
       delete m_userControls;
	   m_userControls = 0;
	}
    if(m_clientNet)
	{
       delete m_clientNet;
	   m_clientNet = 0;
	}
	if(m_sceneAssembler)
	{
       delete m_sceneAssembler;
	   m_sceneAssembler = 0;
	}
	if(m_menuManagement)
	{
       delete m_menuManagement;
	   m_menuManagement = 0;
	}
	if(m_renderManager)
	{
       delete m_renderManager;
	   m_renderManager = 0;
	}
	if(m_misExpManager)
	{
       delete m_misExpManager;
	   m_misExpManager = 0;
	}
	if(m_aiManager)
	{
       delete m_aiManager;
	   m_aiManager = 0;
	}
	if(m_entityManager)
	{
       delete m_entityManager;
	   m_entityManager = 0;
	}
}
//------------------Direct X Version detection---------------------//
BOOL CClientInitialization::DetectDirectXVersionReq(DWORD minVersion,
													CString errorMsg)
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
