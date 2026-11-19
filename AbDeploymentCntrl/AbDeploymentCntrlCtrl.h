#pragma once

// AbDeploymentCntrlCtrl.h : Declaration of the CAbDeploymentCntrlCtrl ActiveX Control class.

//render headers
#include "CCabinetControl.h"
#include "clientmodule.h"

// CAbDeploymentCntrlCtrl : See AbDeploymentCntrlCtrl.cpp for implementation.

class CAbDeploymentCntrlCtrl : public COleControl
{
	DECLARE_DYNCREATE(CAbDeploymentCntrlCtrl)
	CClientModule * m_engineClient;
	DWORD           m_globalRenderLoopThreadID;
	HANDLE          m_globalRenderLoopHandle;
	BOOL            m_running;
	HWND            windowHandle;
	BOOL            m_rSp;
	BOOL            m_test;

// Constructor
public:
	CAbDeploymentCntrlCtrl();

	
//Functions
	long DownloadFileToDisk(LPSTR url,LPSTR localFilePathName,BOOL showProgressMeter);
	void LaunchExecutable(LPSTR path, LPSTR fileNameX);
	int ExtractCabFile(LPSTR destPath,LPSTR CabinetFile,BOOL showProgress);
	void SetSoftwareRegistry(LPSTR headerX,
							 LPSTR valueName,
							 LPSTR valueDataX);
	LPSTR GetSoftwareRegkeyValue(LPSTR headerX,LPSTR valueRequest);
	void MakeDirectory(LPSTR path);
	long GetFreeSpaceMB(LPSTR path);
	long ValidateVersion(LPSTR localPath);
	void WriteVersion(LPSTR localPath, long version);
	CString ConvToString(int number);
	void StartGame();

// Overrides
public:
	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();

// Implementation
protected:
	~CAbDeploymentCntrlCtrl();

	DECLARE_OLECREATE_EX(CAbDeploymentCntrlCtrl)    // Class factory and guid
	DECLARE_OLETYPELIB(CAbDeploymentCntrlCtrl)      // GetTypeInfo
	DECLARE_PROPPAGEIDS(CAbDeploymentCntrlCtrl)     // Property page IDs
	DECLARE_OLECTLTYPE(CAbDeploymentCntrlCtrl)		// Type name and misc status

// Message maps
	DECLARE_MESSAGE_MAP()

// Dispatch maps
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

// Event maps
	DECLARE_EVENT_MAP()

// Dispatch and event IDs
public:
	enum {
	};
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
};

