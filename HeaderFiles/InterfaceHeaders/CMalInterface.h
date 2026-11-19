

#ifndef CMalInterfaceCLASS_H
#define CMalInterfaceCLASS_H
//main application headers so define references memory

#include "d3d9.h"
#include "d3dx9.h"



#include <mmsystem.h>
#include "dsound.h"
#include "CMovementModel.h"
#include "CFrameObj.h"
#include "CWaterZoneClass.h"
#include "AIclass.h"
#include "postprocessmodule.h"
#include "usercontroller.h"
#include "globalstatusvariables.h"



class AFX_EXT_CLASS CMalInterface
{

    public:
		
	CMalInterface();
	
	~CMalInterface();
	
	//variables
	
    CRITICAL_SECTION m_applicationAccess; //this will unlock breifly for safe access to application data, 
	                                             //must include this lock point to interactive dlls that access the primary application
    //functions
	void CMalInterface::InitializeInterfaces(   CWaterZoneObjList				** appWaterZoneDB, 
												LPDIRECT3DDEVICE9				   appDevice,
												CAIObjList						** aiDBAddress,
												CTextureDBObjList				** textureDataBase,
												CPostProcessModuleManager		** postProcessModule,
												CShaderObjList					** pxShaderSystem,
												UserController                  ** userControlInterface,
												CSoundTrackObjList              ** soundTrackDB,
												GlobalStatusVariables			** gstatus,
												LPDIRECTSOUND                      dsound,
												HWND                               hwndMain);
	void		    Callback()
	{
		LeaveCriticalSection( &m_applicationAccess );
	    EnterCriticalSection( &m_applicationAccess );
	}//place in safe synchronous loop

	void TestFunction();
	void KingdomManager();
	void OpenMenuBar();
	// Nav UI menus (World/Characters/Environment/...) merged into a frame's own menu bar
	// instead of the floating Nav UI window. Returns FALSE if the frame has no menu.
	BOOL AttachNavMenuToFrame(CWnd* frame);
	// Route a frame WM_COMMAND / CN_UPDATE_COMMAND_UI to the Nav UI handlers.
	BOOL RouteNavCommand(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);
	void ShowNavWindow();
	void PostEffectManager();
	void ScriptLoader();
	void SceneScriptManager();
	void MusicManager();
	void ActionZoneManager();

	//Data references
	GlobalStatusVariables			*  m_gStatus;
    CMovementObjList				*  m_baseEntityDatabase;
    CMovementObjList				*  m_runtimeEntityDatabase;
	CWaterZoneObjList				** m_waterZoneDBAddress;
	CAIObjList						** m_aiDBAddress;
	CTextureDBObjList				** m_textureDataBase;
	CPostProcessModuleManager		** m_postProcessModule;
	CShaderObjList                  ** m_pxShaderSystem;
	UserController                  ** m_userControlInterface;
	CSoundTrackObjList              ** m_soundTrackDB;
	LPDIRECT3DDEVICE9				   m_currentD3DDevice;
	LPDIRECTSOUND                      m_directSound;
	HWND                               m_hwndMain;
	

};


#endif