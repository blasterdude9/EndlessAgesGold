//ActiveXPortal.cpp : Implementation of CActiveXPortalApp and DLL registration.

#include "stdafx.h"
#include "CatHelp.h"
#include "ActiveXPortal.h"

#ifdef _DEBUG
#define new DEBUG_NEW
 #endif


CActiveXPortalApp NEAR theApp;

const GUID CDECL BASED_CODE _tlid =
		{ 0x7FB4ED24, 0x9423, 0x44BA, { 0xA7, 0x3, 0x15, 0x9A, 0xF, 0x71, 0x21, 0xF2 } };
const WORD _wVerMajor = 2;
const WORD _wVerMinor =  4;

//{C45F05B2-4D16-11d2-800E-0080C76670C0}
//static const GUID attrGuid1 = 
//{ 0xc45f05b2, 0x4d16, 0x11d2, { 0x80, 0xe, 0x0, 0x80, 0xc7, 0x66, 0x70, 0xc0 } };

//safe
//{CB0663D5-CFEC-436E-81E2-145507C164B4}
//{ 0xcb0663d5, 0xcfec, 0x436e, { 0x81, 0xe2, 0x14, 0x55, 0x07, 0xc1, 0x64, 0xb4 } };

const CATID CATID_SafeForScripting     =
      {0x7dd95801,0x9882,0x11cf,{0x9f,0xa9,0x00,0xaa,0x00,0x6c,0x42,0xc4}};
const CATID CATID_SafeForInitializing  =
      {0x7dd95802,0x9882,0x11cf,{0x9f,0xa9,0x00,0xaa,0x00,0x6c,0x42,0xc4}};

const GUID CDECL BASED_CODE _ctlid =
      { 0x3c403675, 0xb43c, 0x410b, { 0xbf, 0x56, 0xd4, 0xd1, 0xfb, 0x68, 0x35, 0x6c} };



//CActiveXPortalApp::InitInstance - DLL initialization

 BOOL CActiveXPortalApp::InitInstance()
{
	AfxOleInit();
	BOOL bInit = COleControlModule::InitInstance();
	AfxEnableControlContainer();

	if (bInit)
	{
		//TODO: Add your own module initialization code here.
	}

	return bInit;
}



//CActiveXPortalApp::ExitInstance - DLL termination

int CActiveXPortalApp::ExitInstance()
{
	//TODO: Add your own module termination code here.

	return COleControlModule::ExitInstance();
}



//DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(TRUE))
		return ResultFromScode(SELFREG_E_CLASS);



	//safe
	      if (FAILED( CreateComponentCategory(
                  CATID_SafeForScripting,
                  L"Controls that are safely scriptable") ))
                return ResultFromScode(SELFREG_E_CLASS);

          if (FAILED( CreateComponentCategory(
                  CATID_SafeForInitializing,
                  L"Controls safely initializable from persistent data") ))
                return ResultFromScode(SELFREG_E_CLASS);

          if (FAILED( RegisterCLSIDInCategory(
                  _ctlid, CATID_SafeForScripting) ))
                return ResultFromScode(SELFREG_E_CLASS);

          if (FAILED( RegisterCLSIDInCategory(
                  _ctlid, CATID_SafeForInitializing) ))
                return ResultFromScode(SELFREG_E_CLASS);







	return NOERROR;
}



//DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
	AFX_MANAGE_STATE(_afxModuleAddrThis);

	if (!AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor))
		return ResultFromScode(SELFREG_E_TYPELIB);

	if (!COleObjectFactoryEx::UpdateRegistryAll(FALSE))
		return ResultFromScode(SELFREG_E_CLASS);

	return NOERROR;
}
