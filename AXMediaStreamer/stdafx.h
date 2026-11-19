// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers

#include <atlbase.h>
//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
class CExeModule : public CComModule
{
public:
    LONG Unlock();
    DWORD dwThreadID;
    HANDLE hEventShutdown;
    void MonitorShutdown();
    bool StartMonitor();
    bool bActivity;
};
extern CExeModule _Module;
#include <atlcom.h>
#include <atlwin.h>

// Added for common controls
#include <commctrl.h>
#include <stdio.h>

// Max length of string 
#define MAX_BSTR_LONG       400
#define MAX_BSTR_VERY_LONG  50000

enum MEDIATYPE
{
    INVALID = -1,
    MUSIC,
    VIDEO,
    OTHER,
    PLAYLIST
};

MEDIATYPE TellMediaType(TCHAR *szMediaType);
