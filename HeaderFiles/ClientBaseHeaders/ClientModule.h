#pragma once
#include "GlobalDefines.h"
#include "clientinitialization.h"

class CClientModule
{
public:
	CClientModule(HWND hwnd,
		          int startWidth,
				  int startHeight,
		          int widthLoc,
				  int hieghtLoc,
				  int depth,
				  BOOL fullScreen,
				  BOOL editOn,
				  BOOL ovrdRDsrfc,
				  BOOL renderingEnabled,
				  BOOL processScripts,
				  BOOL initDirectInput,
				  int maxTextureSize,
				  int shadowCount,
				  BOOL sim);
	~CClientModule(void);

	//Variables
    CClientInitialization			* m_initObject;
    LARGE_INTEGER					StartTime;
	LARGE_INTEGER					EndTime;

	//DWORD                           m_startTimeEmulate;
	DWORD                           m_lastMsgProcessed;

    //primary client loop
	void ClientCallback();
	void CallBackTimeOpen();
	void CallBackTimeCap();


    //windows msg processing
	void ProcessAnalogKeyboardMsgs(UINT & chrPass);

	//Interface Aquisition Functions
    GlobalStatusVariables			* GetGlobalVarInterface();
	SceneAssembly					* GetSceneAssemblerInterface();
	CClientNetworkingLogic			* GetNetClientInterface();
	UserController					* GetUserControllerInterface();
	CMenuManagement					* GetMenuManagerInterface();
	CRenderingManagement            * GetRenderManagerInterface();


};
