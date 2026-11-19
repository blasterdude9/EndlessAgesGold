#include "Stdafx.h"
#include <afxtempl.h>
#include "CMalInterface.h"
#include "CMenuAccessInterface.h"
#include "NavUIDLG.h"
#include "CrashReporter.h"

static CNavUIDLG m_toolbarInstance;

CMalInterface::CMalInterface()
{
		InitializeCriticalSection(  &m_applicationAccess );
		EnterCriticalSection(  &m_applicationAccess );
}
CMalInterface::~CMalInterface()
{
        DeleteCriticalSection( &m_applicationAccess);
}

void CMalInterface::TestFunction()
{ 
	  
	  CMenuAccessInterface m_baseMenus_AUI;
	  m_baseMenus_AUI.WaterZoneDlg(m_waterZoneDBAddress,m_currentD3DDevice);
	  
}

void CMalInterface::KingdomManager()
{
	
}

void CMalInterface::PostEffectManager()
{
      CMenuAccessInterface m_baseMenus_AUI;
	  m_baseMenus_AUI.PostEffectManager(*m_textureDataBase,m_postProcessModule,*m_pxShaderSystem);
}

void CMalInterface::ScriptLoader()
{
      CMenuAccessInterface m_baseMenus_AUI;
	  m_baseMenus_AUI.LoadScriptsManager();
}


void CMalInterface::MusicManager()
{
      CMenuAccessInterface m_baseMenus_AUI;

	  m_baseMenus_AUI.LoadMusicManager(m_soundTrackDB,m_directSound,m_hwndMain,this->m_gStatus->GL_pathGameSounds);
}

void CMalInterface::ActionZoneManager()
{
      CMenuAccessInterface m_baseMenus_AUI;
	  m_baseMenus_AUI.LoadActionZonesDlg();

}

void CMalInterface::OpenMenuBar()
{
	  AFX_MANAGE_STATE(AfxGetStaticModuleState());


	  m_toolbarInstance.interfaceRef    = this;
	  if(m_toolbarInstance.GetSafeHwnd() == NULL)
	  {
	     if(!m_toolbarInstance.Create(IDD_DIALOG_NAVUI,NULL))
	     {
	        char line[256];
	        sprintf_s(line, "Nav UI: CNavUIDLG::Create(IDD_DIALOG_NAVUI=%u) failed - dialog template not found in exe resources", (unsigned)IDD_DIALOG_NAVUI);
	        CrashLogLine(line);
	        AfxMessageBox(line, MB_ICONERROR);
	        return;
	     }
	  }
	  // created hidden: EngineXWin either merges the menu into its own menu bar
	  // (AttachNavMenuToFrame) or shows the floating window (ShowNavWindow).
}

void CMalInterface::ShowNavWindow()
{
	  if(m_toolbarInstance.GetSafeHwnd())
	     m_toolbarInstance.ShowWindow(SW_SHOW);
}

BOOL CMalInterface::AttachNavMenuToFrame(CWnd* frame)
{
	  if(!frame || !frame->GetSafeHwnd())
	     return FALSE;
	  CMenu* frameMenu = frame->GetMenu();
	  if(!frameMenu || !frameMenu->GetSafeHmenu())
	     return FALSE;
	  CMenu nav;
	  if(!nav.LoadMenu(IDR_MENU_NAV))
	     return FALSE;

	  // insert in front of the frame's own "Help" so Help stays last
	  int insertAt = frameMenu->GetMenuItemCount();
	  for(int i = 0; i < frameMenu->GetMenuItemCount(); i++)
	  {
	     CString t; frameMenu->GetMenuString(i, t, MF_BYPOSITION); t.Remove('&');
	     if(t.CompareNoCase("Help") == 0) { insertAt = i; break; }
	  }

	  while(nav.GetMenuItemCount() > 0)
	  {
	     CString text; nav.GetMenuString(0, text, MF_BYPOSITION);
	     CString plain = text; plain.Remove('&');
	     HMENU hSub = ::GetSubMenu(nav.GetSafeHmenu(), 0);
	     UINT  id   = nav.GetMenuItemID(0);
	     // RemoveMenu detaches without destroying, so a popup can be re-parented
	     ::RemoveMenu(nav.GetSafeHmenu(), 0, MF_BYPOSITION);
	     if(plain.CompareNoCase("Help") == 0)
	     {
	        if(hSub) ::DestroyMenu(hSub);
	        continue;   // the frame already has Help
	     }
	     if(hSub)
	        frameMenu->InsertMenu(insertAt++, MF_BYPOSITION | MF_POPUP | MF_STRING, (UINT_PTR)hSub, text);
	     else
	        frameMenu->InsertMenu(insertAt++, MF_BYPOSITION | MF_STRING, id, text);
	  }
	  nav.DestroyMenu();
	  frame->DrawMenuBar();

	  // the Nav UI handlers check/uncheck items on "their" menu - point them at the frame's
	  m_toolbarInstance.m_hostMenu = frameMenu->GetSafeHmenu();
	  if(m_toolbarInstance.GetSafeHwnd())
	     m_toolbarInstance.ShowWindow(SW_HIDE);
	  return TRUE;
}

BOOL CMalInterface::RouteNavCommand(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	  if(!m_toolbarInstance.GetSafeHwnd())
	     return FALSE;
	  return m_toolbarInstance.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



void CMalInterface::SceneScriptManager()
{
	UserController * tempCastUserC = *m_userControlInterface;
    CMenuAccessInterface m_baseMenus_AUI;
	m_baseMenus_AUI.LoadSceneScriptsManager(&tempCastUserC->m_gStatus->m_sceneScriptExec);
}

void CMalInterface::InitializeInterfaces(      CWaterZoneObjList				** appWaterZoneDB, 
												LPDIRECT3DDEVICE9				   appDevice,
												CAIObjList						** aiDBAddress,
												CTextureDBObjList				** textureDataBase,
												CPostProcessModuleManager		** postProcessModule,
												CShaderObjList					** pxShaderSystem,
												UserController                  ** userControlInterface,
												CSoundTrackObjList              ** soundTrackDB,
												GlobalStatusVariables			** gstatus,
												LPDIRECTSOUND                      dsound,
												HWND                               hwndMain)
{
	m_waterZoneDBAddress     = appWaterZoneDB;
	m_currentD3DDevice       = appDevice;
	m_aiDBAddress            = aiDBAddress;
	m_textureDataBase        = textureDataBase;
    m_postProcessModule      = postProcessModule;
	m_pxShaderSystem         = pxShaderSystem;
	m_userControlInterface   = userControlInterface;
	m_soundTrackDB           = soundTrackDB;
	m_directSound			 = dsound;
	m_hwndMain				 = hwndMain;
	m_gStatus				 = *gstatus;
}