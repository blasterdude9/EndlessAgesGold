#include "stdafx.h"
#include "CDLLExportAB.h"


void CDLLExportAB::Init(HINSTANCE hInstance)
{ 
     if (!AfxInitExtensionModule(AuraXDynamicDLL, hInstance))
	 {

	 }

     new CDynLinkLibrary(AuraXDynamicDLL);
}