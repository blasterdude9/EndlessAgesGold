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
//     $Date: 5/13/98 12:02a $
//  $Modtime: 4/27/98 6:50a $
// $Revision: 14 $
//  $Archive: /Interscope/Thebe/InstallMaster/DrvType.H $
// $Workfile: DrvType.H $
//-----------------------------------------------------------------------

#ifndef __DriveType_H__
#define __DriveType_H__

// Define the following symbol if compiling using precompiled headers through 
// header file StdAfx.H
// #define __STDAFX__
//
// Define the following symbol if used in a MFC project
// #define __MFC__

#ifdef __MFC__
#undef __STDAFX__
#define __STDAFX__
#endif

#if !defined(__WIN32__) && !defined(_WIN32)
#ifndef STRICT
#define STRICT
#endif
#endif // __WIN32__

#include <Windows.H>


//--- Drive types -------------------------------------------------------

#define EX_DRIVE_INVALID    0
#define EX_DRIVE_REMOVABLE  1
#define EX_DRIVE_FIXED      2
#define EX_DRIVE_REMOTE     3
#define EX_DRIVE_CDROM      4
#define EX_DRIVE_FLOPPY     5
#define EX_DRIVE_RAMDISK    6


//--- Determines the type of a drive ------------------------------------

UINT GetDriveTypeEx(WORD nDrive); // A=0, B=1, etc


#endif

