//---------------------------------------------------------------------------
// Copyright (C) 1998, Interscope Ltd. All rights reserved.
// Reproduction or distribution of this program, or any portion of it, 
// is permitted only if this header is kept as it is.
// For more information, contact:
//
// Interscope Ltd., 5 Culturii St., 5th Floor, 4800 Baia Mare, RO
//    Phone/Fax: +40-62-215023
//    E-mail: office@interscope.ro
//
//   $Author: Levente Farkas $
//     $Date: 5/12/98 11:50p $
//  $Modtime: 4/30/98 8:22a $
// $Revision: 23 $
//  $Archive: /Interscope/Thebe/InstallMaster/OSVersion.cpp $
// $Workfile: OSVersion.cpp $
//---------------------------------------------------------------

// 4 more details about API calls in this module, see
// Knowledge Base Atricle Q113998 and
// Knowledge Base Atricle Q114470 on
// Microsoft Development Library (October 1995)


#include "StdAfx.H"


#include "Portable.H"
#include "AssertX.H"
#include "OSVersion.Hpp"


//--- Debugee --------------------------------------------------------------

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#ifdef __MFC__
#define new DEBUG_NEW
#endif // __MFC__
#endif // _DEBUG


//--- Some types and consts -----------------------------------

#define WNNC_NET_MULTINET        0x8000
#define WNNC_SUBNET_WINWORKGROUP 0x0004
#define WNNC_NET_TYPE            0x0002

typedef WORD (WINAPI *NETCAPFUNC)(int);


//-------------------------------------------------------------
// Pre     :
// Post    :
// Globals :
// I/O     :
// Task    : Constructs and OS version class and extracts all the
//           version information necessary
//-------------------------------------------------------------
COSVersion::COSVersion():
            m_nOSType(OS_UNKNOWN),
            m_nWinType(WIN_UNKNOWN),
            m_dwVersion(GetVersion())
{
#if defined(__WIN32__) || defined(_WIN32) // 32-bit platform


	osvi.dwOSVersionInfoSize =sizeof(osvi);
    GetVersionEx(&osvi);

    // Test 4 NT/NTS/NTAS by the highest bit of the version
    if(m_dwVersion & 0x80000000)
    {
        // This is not NT
        // Check major version number 2 distinguish between
        // WIN32s (Win 3.X) and WIN32c (Win 95)
        if(LOBYTE(LOWORD(m_dwVersion)) <= 3)
        {
            m_nWinType =WIN_32S;

            // Determine if Win 3.X or WFW
            m_nOSType =OS_WIN3X;
        }
        else
        {
            m_nWinType =WIN_32C;

            // Determine if Win95 or Win98
            // Note: We consider Win98 from beta 2 up (4.10.1650)
            ASSERTX(osvi.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS);
            if(osvi.dwMinorVersion == 0)
                m_nOSType =OS_WIN95;
            else
                m_nOSType =OS_WIN98;
        }
    }
    else
    {
        // Must be NTWS, NTS or NTAS
        // Check the registry 2 distinguish between them
        HKEY  hKey;
        BYTE  szValue[128];
        DWORD dwSize =sizeof(szValue);
        RegOpenKeyEx(HKEY_LOCAL_MACHINE,_T("SYSTEM\\CurrentControlSet\\Control\\ProductOptions"),
                     0,KEY_READ,&hKey);
        RegQueryValueEx(hKey,_T("ProductType"),0,NULL,szValue,&dwSize);
        RegCloseKey(hKey);
        if(!STRICMP((TCHAR *)&szValue[0],_T("WinNT"))) 
            // WinNT (Windows NT Workstation)
            m_nOSType =OS_WINNTWS;
        else if(!STRICMP((TCHAR *)&szValue[0],_T("ServerNT"))) 
            // ServerNT (Windows NT Server)
            m_nOSType =OS_WINNTS;
        else 
            // LanmanNT (Windows NT Advanced Server)
            m_nOSType =OS_WINNTAS;

        m_nWinType =WIN_32;
    }


#else // 16-bit platform


    // For 16-bit code, use GetWinFlags 2 find out if running on the
    // Windows on Windows layer of NT/NTAS
    if(GetWinFlags() & 0x4000) 
        m_nOSType =OS_WINNTWS;
    else
    {
        // Check version number 2 distinguish between Win 3.X and Win 95
        if((LOBYTE(LOWORD(dwVersion)) > 3) ||
           (HIBYTE(LOWORD(dwVersion)) > 50)) 
            // Windows 95
            m_nOSType =OS_WIN95;
        else
        {
           // Determine if Win 3.X or WFW
         
               m_nOSType =OS_WIN3X;
        }
    }
    m_nWinType =WIN_16;

#endif
}

//-------------------------------------------------------------
// Pre     : Check method
// Post    :
// Globals :
// I/O     :
// Task    : Checks if running on Windows 4 Workgroups
//-------------------------------------------------------------


//-------------------------------------------------------------
// Pre     :
// Post    :
// Globals :
// I/O     :
// Task    : Get OS major version number
//-------------------------------------------------------------
DWORD COSVersion::GetMajorVersion() const
{
#if defined(__WIN32__) || defined(_WIN32)
    return osvi.dwMajorVersion;
#else
    return (DWORD)(LOBYTE(LOWORD(dwVersion)));
#endif
}

//-------------------------------------------------------------
// Pre     :
// Post    :
// Globals :
// I/O     :
// Task    : Get OS minor version number
//-------------------------------------------------------------
DWORD COSVersion::GetMinorVersion() const
{
#if defined(__WIN32__) || defined(_WIN32)
    return osvi.dwMinorVersion;
#else
    return (DWORD)(HIBYTE(LOWORD(dwVersion)));        
#endif
}

#if defined(__WIN32__) || defined(_WIN32)
//-------------------------------------------------------------
// Pre     :
// Post    :
// Globals :
// I/O     :
// Task    : Get the build number of the OS
//-------------------------------------------------------------
DWORD COSVersion::GetBuildNumber() const
{
    return (m_nOSType & OS_WINNT) ? osvi.dwBuildNumber : LOWORD(osvi.dwBuildNumber);
}
#endif

#if defined(__WIN32__) || defined(_WIN32)
//-------------------------------------------------------------
// Pre     :
// Post    :
// Globals :
// I/O     :
// Task    : Get the special version data (string got from GetVersionEX)
//-------------------------------------------------------------
LPCTSTR COSVersion::GetSpecialVersion() const
{
	return osvi.szCSDVersion;
}
#endif

