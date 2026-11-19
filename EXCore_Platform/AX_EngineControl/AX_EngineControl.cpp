// AX_EngineControl.cpp : Implementation of CAX_EngineControlApp and DLL registration.

#include "stdafx.h"
#include "AX_EngineControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


CAX_EngineControlApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x1CFD2496, 0x48DE, 0x462F, { 0xAD, 0x3D, 0x75, 0x58, 0xB8, 0xEB, 0x29, 0xFC } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;



// CAX_EngineControlApp::InitInstance - DLL initialization

BOOL CAX_EngineControlApp::InitInstance()
{
	BOOL bInit = COleControlModule::InitInstance();

	if (bInit)
	{
		// TODO: Add your own module initialization code here.
	}

	return bInit;
}



// CAX_EngineControlApp::ExitInstance - DLL termination

int CAX_EngineControlApp::ExitInstance()
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
