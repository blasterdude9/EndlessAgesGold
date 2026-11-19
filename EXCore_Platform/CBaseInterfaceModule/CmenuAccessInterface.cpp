#include "Stdafx.h"
#include "d3d9.h"
#include <afxtempl.h>
#include "CMenuAccessInterface.h"
#include "PostEffectDlg.h"
#include "LoadScriptsDlg.h"
#include "MusicDlg.h"
#include "actionzonesdlg.h"



void CMenuAccessInterface::TestFunction()
{
     
}

void CMenuAccessInterface::WaterZoneDlg(CWaterZoneObjList ** appWaterZoneDB, 
										LPDIRECT3DDEVICE9 appDevice)
{
	 //AFX_MANAGE_STATE(AfxGetStaticModuleState());
     /*CWaterZoneDBDLG dlg;
	 dlg.m_waterZnDBREF    = *appWaterZoneDB;
	 dlg.g_pd3dDeviceRef   = appDevice;
	 dlg.DoModal();
	 *appWaterZoneDB = dlg.m_waterZnDBREF;*/
	/* CWaterZoneDBDLG * testInstance = new CWaterZoneDBDLG();	
	 testInstance->m_waterZnDBREF    = *appWaterZoneDB;
	 testInstance->m_waterZnDBREF_memloc = appWaterZoneDB;
	 testInstance->g_pd3dDeviceRef   = appDevice;
	 testInstance->Create(IDD_DIALOG_WaterZoneDB,NULL);
     testInstance->ShowWindow(SW_SHOW);*/
}

void CMenuAccessInterface::LoadActionZonesDlg()
{
     AFX_MANAGE_STATE(AfxGetStaticModuleState());
     CActionZonesDlg * zoneInstance = new CActionZonesDlg();
     zoneInstance->Create(IDD_DIALOG_ActionZonesDlg,NULL);
     zoneInstance->ShowWindow(SW_SHOW);
}

void CMenuAccessInterface::PostEffectManager(CTextureDBObjList * textureDataBase,
											 CPostProcessModuleManager ** processModule,
											 CShaderObjList * pxShaderSystem)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CPostEffectDlg * testInstance					= new CPostEffectDlg();	
	testInstance->m_pxShaderSystemRef				= pxShaderSystem;
	testInstance->m_processModuleMemoryRef          = processModule;
	testInstance->m_processModuleRef                = *processModule;
	testInstance->m_textureDataBaseRef              = textureDataBase;
	testInstance->Create(IDD_DIALOG_PostEffectDialog,NULL);
	testInstance->ShowWindow(SW_SHOW);
}


void CMenuAccessInterface::LoadScriptsManager()
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CLoadScriptsDlg * testInstance					= new CLoadScriptsDlg();	
	testInstance->m_fromFile = TRUE;
	testInstance->Create(IDD_DIALOG_LoadScripts,NULL);
	testInstance->ShowWindow(SW_SHOW);
}

void CMenuAccessInterface::LoadSceneScriptsManager(CEXScriptObjList ** sScript)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CLoadScriptsDlg * testInstance					= new CLoadScriptsDlg();	
	testInstance->m_fromFile = FALSE;
	testInstance->m_scriptListRef = *sScript;
	testInstance->m_scriptListMemoryRef = sScript;
	//m_sceneScriptExec
	testInstance->Create(IDD_DIALOG_LoadScripts,NULL);
	testInstance->ShowWindow(SW_SHOW);
}

void CMenuAccessInterface::LoadMusicManager(CSoundTrackObjList ** m_soundTrackModule,LPDIRECTSOUND dsound,HWND hwndMain,CString soundPath)
{
    AFX_MANAGE_STATE(AfxGetStaticModuleState());
    CMusicDlg * testInstance						= new CMusicDlg();	
	testInstance->m_soundTrackMemoryRef				= m_soundTrackModule;
	testInstance->m_soundTrackModuleRef             = *m_soundTrackModule;
	testInstance->m_soundFilePath					= soundPath;
	
	testInstance->Create(IDD_DIALOG_MusicDlg,NULL);
	testInstance->ShowWindow(SW_SHOW);
}