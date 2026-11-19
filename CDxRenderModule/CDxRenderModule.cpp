// CDxRenderModule.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "CDxRenderModule.h"
// When built against MFC (_AFXDLL), MFC supplies DllMain and this no-op stub
// would collide with it (LNK2005). It did nothing but return TRUE, so exclude
// it under MFC and let MFC's DllMain run. Kept for a future non-MFC build.
#ifndef _AFXDLL
BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}
#endif

// This is an example of an exported variable
CDXRENDERMODULE_API int nCDxRenderModule=0;

// This is an example of an exported function.
CDXRENDERMODULE_API int fnCDxRenderModule(void)
{
	return 42;
}

// This is the constructor of a class that has been exported.
// see CDxRenderModule.h for the class definition
CCDxRenderModule::CCDxRenderModule()
{ 
	return; 
}
