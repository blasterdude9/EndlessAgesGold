// AbDeploymentCntrl.cpp : Implementation of CAbDeploymentCntrlApp and DLL registration.

#include "stdafx.h"
#include "AbDeploymentCntrl.h"
#include ".\abdeploymentcntrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CAbDeploymentCntrlApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0xA4521B0A, 0xB524, 0x431B, { 0xAA, 0xDE, 0xA3, 0x89, 0xCB, 0x77, 0x1D, 0x21 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CAbDeploymentCntrlApp::InitInstance - DLL initialization

BOOL CAbDeploymentCntrlApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{

	}

	return bInit;
}



// CAbDeploymentCntrlApp::ExitInstance - DLL termination

int CAbDeploymentCntrlApp::ExitInstance()
{
	// TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}



// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}



// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}

//BOOL CAbDeploymentCntrlApp::OnIdle(LONG lCount)
//{
//	/*if(!m_running)
//	if(!m_engineClient)
//	{
//		m_running = TRUE;
//		::ShowWindow(this->m_pActiveWnd->m_hWnd,SW_SHOW);
//	    m_engineClient = new CClientModule(this->m_pActiveWnd->m_hWnd,256,256,32);
//		//m_globalRenderLoopHandle = CreateThread(NULL,0,RenderLoopThread,m_engineClient,0,&m_globalRenderLoopThreadID);
//	}
//
//	m_engineClient->CallBackTimeOpen();
//	m_engineClient->ClientCallback();
//	m_engineClient->CallBackTimeCap();*/
//
//	return COleControlModule::OnIdle(lCount);
//}
