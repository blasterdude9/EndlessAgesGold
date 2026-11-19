// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

//#include <windows.h>
//#include <stdio.h>
//#include <iostream>
//#include <tchar.h>

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes

// TODO: reference additional headers your program requires here
#ifndef DECLARE_SERIAL_EXTDLL
#define DECLARE_SERIAL_EXTDLL(class_name) _DECLARE_DYNCREATE(class_name) AFX_EXT_API friend CArchive& AFXAPI operator>>(CArchive& ar, class_name* &pOb);
#endif