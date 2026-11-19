#pragma once

#include "clientnetworkinglogic.h"
#include "sceneassembly.h"
#include "globalstatusvariables.h"
#include "menumanagement.h"
#include "usercontroller.h"
#include "renderingmanagement.h"
#include "missileexplosionmanager.h"
#include "aimanager.h"
#include "entitymanagement.h"


class CClientInitialization
{
public:
	CClientInitialization(HWND & hwnd,
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
						  BOOL sim);
	~CClientInitialization(void);

	//Variables
	GlobalStatusVariables        *	m_gStatus;
	SceneAssembly                *	m_sceneAssembler;
	CClientNetworkingLogic       *	m_clientNet;
	UserController               *  m_userControls;
	CMenuManagement              *  m_menuManagement;
	CRenderingManagement         *  m_renderManager;
	CMissileExplosionManager     *  m_misExpManager;
	CAiManager                   *  m_aiManager;
	CEntityManagement            *  m_entityManager;
	

    //Functions
    BOOL DetectDirectXVersionReq(DWORD minVersion,CString errorMsg);



};
