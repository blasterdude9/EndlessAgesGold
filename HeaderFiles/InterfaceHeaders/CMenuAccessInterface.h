

#pragma once
//main application headers so define references memory

#include "d3d9.h"
#include "resource.h"
#include "CFrameObj.h"
#include "CWaterZoneClass.h"
#include "AIclass.h"
#include "postprocessmodule.h"
#include "CSoundTrackClass.h"
#include "CEXScriptClass.h"

class AFX_EXT_CLASS CMenuAccessInterface
{

    public:
		
		CMenuAccessInterface(){};
		~CMenuAccessInterface(){};
	//variables
   
    void TestFunction();
	void WaterZoneDlg(CWaterZoneObjList ** appWaterZoneDB, LPDIRECT3DDEVICE9 appDevice);
	void PostEffectManager(CTextureDBObjList * textureDataBase,
						   CPostProcessModuleManager ** processModule,
						   CShaderObjList * pxShaderSystem);
	void LoadScriptsManager();
	void LoadActionZonesDlg();
	void LoadSceneScriptsManager(CEXScriptObjList ** sScript);
	void LoadMusicManager(CSoundTrackObjList ** m_soundTrackModule,LPDIRECTSOUND dsound,HWND hwndMain,CString soundPath);
};
