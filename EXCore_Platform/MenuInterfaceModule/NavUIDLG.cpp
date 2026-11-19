// NavUIDLG.cpp : implementation file
//

#include "stdafx.h"
#include "MenuInterfaceModule.h"
#include "NavUIDLG.h"


// CNavUIDLG dialog

IMPLEMENT_DYNAMIC(CNavUIDLG, CDialog)
CNavUIDLG::CNavUIDLG(CWnd* pParent /*=NULL*/)
	: CDialog(CNavUIDLG::IDD, pParent)
{
	interfaceRef = NULL;
	m_menuHandle = NULL;
	m_hostMenu   = NULL;
}

CNavUIDLG::~CNavUIDLG()
{
}

void CNavUIDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CNavUIDLG, CDialog)
	ON_COMMAND(ID_GAMPLAYMODULES_KINGDOMSMANAGER, OnGamplaymodulesKingdomsmanager)
	ON_COMMAND(ID_DISPLAY_POSTEFFECTS, OnDisplayPosteffects)
	ON_COMMAND(ID_DISPLAY_LOCKVIEW, OnDisplayLockview)
	ON_UPDATE_COMMAND_UI(ID_DISPLAY_LOCKVIEW, OnUpdateDisplayLockview)
	ON_COMMAND(ID_INITIALIZATION_LOADSCRIPTMANAGER, OnInitializationLoadscriptmanager)
	ON_COMMAND(ID_SOUND_MUSICMANAGER, OnSoundMusicmanager)
	ON_COMMAND(ID_INITIALIZATION_SCENESCRIPTMANAGER, OnInitializationScenescriptmanager)
	ON_COMMAND(ID_ENVIRONMENT_ACTIONZONES, OnEnvironmentActionzones)
	ON_COMMAND(ID_DISPLAY_CAPTUREZONEMAP, OnDisplayCaptureZoneMap)
	ON_COMMAND(ID_DISPLAY_CAPTUREALLZONEMAPS, OnDisplayCaptureAllZoneMaps)
END_MESSAGE_MAP()


// CNavUIDLG message handlers

void CNavUIDLG::OnGamplaymodulesKingdomsmanager()
{
	interfaceRef->KingdomManager();
}

BOOL CNavUIDLG::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetItemFont();



	//CMenu * menuHandle = GetMenu();
	//menuHandle->CheckMenuItem(ID_DISPLAY_LOCKVIEW,1);

	m_menuHandle = GetMenu();
    //CMenu* submenu = menuHandle->GetSubMenu(0);
    //m_menuHandle->EnableMenuItem(ID_DISPLAY_LOCKVIEW, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
	



	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CNavUIDLG::SetItemFont()
{
	/*UpdateData(TRUE);
	      HFONT hDlgFont;
          LOGFONT lFont;

          lFont.lfHeight = 7;
          lFont.lfWidth = 0;
          lFont.lfEscapement = 0;
          lFont.lfOrientation = 0;
          lFont.lfWeight = 200; // non-bold font weight
          lFont.lfItalic = 0;
          lFont.lfUnderline = 0;
          lFont.lfStrikeOut = 0;
          lFont.lfCharSet = ANSI_CHARSET;
          lFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
          lFont.lfClipPrecision = CLIP_DEFAULT_PRECIS;
          lFont.lfQuality = DEFAULT_QUALITY;
          lFont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
          lFont.lfFaceName[0] = NULL;
          hDlgFont = CreateFontIndirect(&lFont);

          SendDlgItemMessage(ID_GAMPLAYMODULES_KINGDOMSMANAGER, WM_SETFONT, (WPARAM)hDlgFont,(DWORD)TRUE);

		  UpdateData(FALSE);*/
return TRUE;

}

void CNavUIDLG::OnSetFont(CFont* pFont)
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnSetFont(pFont);
}

// Display > Capture Zone Map: the render loop draws the loaded zone from above, tile by
// tile, and saves Maps\<zone>.png (+ .txt with its world area) in the game folder.
void CNavUIDLG::OnDisplayCaptureZoneMap()
{
	if(!interfaceRef || !interfaceRef->m_userControlInterface || !*interfaceRef->m_userControlInterface)
		return;
	GlobalStatusVariables * gs = (*interfaceRef->m_userControlInterface)->m_gStatus;
	if(!gs)
		return;
	if(::MessageBoxA(GetSafeHwnd(), "Make a map picture of the loaded zone?\n\nThe screen will flicker for a minute while it draws the zone from above.\nIt saves Maps\\<zone>.png in the game folder.",
	                 "Capture Zone Map", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
		gs->m_mapCaptureRequest = TRUE;
}

void CNavUIDLG::OnDisplayCaptureAllZoneMaps()
{
	if(!interfaceRef || !interfaceRef->m_userControlInterface || !*interfaceRef->m_userControlInterface)
		return;
	GlobalStatusVariables * gs = (*interfaceRef->m_userControlInterface)->m_gStatus;
	if(!gs)
		return;
	if(::MessageBoxA(GetSafeHwnd(), "Make map pictures of EVERY zone in GameFiles?\n\n"
	                 "EngineX opens each zone by itself and captures it (the _AI pathing files are skipped).\n"
	                 "This takes a while - leave EngineX open and do not minimize it.\n"
	                 "Pictures go to Maps\\ and a report to Maps\\capture_log.txt.\n\n"
	                 "Anything not saved in the zone you have open now will be lost.",
	                 "Capture All Zone Maps", MB_OKCANCEL | MB_ICONQUESTION) == IDOK)
		gs->m_mapCaptureAllZonesRequest = TRUE;
}

void CNavUIDLG::OnDisplayPosteffects()
{
	interfaceRef->PostEffectManager();
}

void CNavUIDLG::OnDisplayLockview()
{
	UserController * userC = *interfaceRef->m_userControlInterface;
	if(userC->m_lockControlSys)
	{
       userC->m_lockControlSys = 0;
	   { HMENU h = m_hostMenu ? m_hostMenu : (m_menuHandle ? m_menuHandle->GetSafeHmenu() : NULL); if(h) ::CheckMenuItem(h,ID_DISPLAY_LOCKVIEW,MF_BYCOMMAND|MF_UNCHECKED); }
	}
	else
	{
       userC->m_lockControlSys = 1;
	   { HMENU h = m_hostMenu ? m_hostMenu : (m_menuHandle ? m_menuHandle->GetSafeHmenu() : NULL); if(h) ::CheckMenuItem(h,ID_DISPLAY_LOCKVIEW,MF_BYCOMMAND|MF_CHECKED); }
	}
}

void CNavUIDLG::OnUpdateDisplayLockview(CCmdUI *pCmdUI)
{
	UpdateData(TRUE);

	
	UpdateData(FALSE);
}

void CNavUIDLG::OnInitializationLoadscriptmanager()
{
	interfaceRef->ScriptLoader();
}

void CNavUIDLG::OnSoundMusicmanager()
{
	interfaceRef->MusicManager();
}

void CNavUIDLG::OnInitializationScenescriptmanager()
{
	interfaceRef->SceneScriptManager();
}

void CNavUIDLG::OnEnvironmentActionzones()
{
	interfaceRef->ActionZoneManager();
}
