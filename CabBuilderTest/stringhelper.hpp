//---------------------------------------------------------------------------
// Copyright (C) 1998, Interscope Ltd. All rights reserved.
// Reproduction or distribution of this program, or any portion of it, 
// is permitted only if this header is kept as it is.
// For more information, contact:
//
// Interscope Ltd., 5 Culturii St., 5th floor, 4800 Baia Mare, Romania
//    Phone/Fax: +40-62-215023
//    E-mail: office@interscope.ro
//
//   $Author: Levente Farkas $
//     $Date: 5/13/98 12:03a $
//  $Modtime: 4/27/98 6:51a $
// $Revision: 13 $
//  $Archive: /Interscope/Thebe/InstallMaster/StringHelper.Hpp $
// $Workfile: StringHelper.Hpp $
//-----------------------------------------------------------------------

#ifndef __String_Helper_Hpp__
#define __String_Helper_Hpp__

// These string helper functions are available both 4 MFC and standard C++ strings
//
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

#include <TChar.H>
#include <String>

#include "Portable.H"


using namespace std;


//--- Constants and defines ---------------------------------------------

const TCHAR WILD_ONE             = '?';
const TCHAR WILD_ANY             = '*';
LPCTSTR const WILD_SET           = _T("?*");
const TCHAR DRIVE_DELIMITER      = ':';
const TCHAR DIRECTORY_DELIMITER  = '\\';
const TCHAR EXTENSION_DELIMITER  = '.';


//--- Helpers ------------------------------------------------------------

#ifdef __MFC__
void SetStringLength(CString& Text, int nLength);
void StripLeadingChar(CString& rText, TCHAR chLeading);
void StripLeadingBackslash(CString& Directory);
void StripTrailingChar(CString& rText, TCHAR chTrailing);
void StripTrailingBackslash(CString& rDirectory);
void EnsureTrailingBackslash(CString& Directory);
void EnsureLeadingBackslash(CString& Directory);
#endif

void StripLeadingChar(string& rText, TCHAR chLeading);
void StripLeadingBackslash(string& Directory);
void StripTrailingChar(string& rText, TCHAR chTrailing);
void StripTrailingBackslash(string& rDirectory);
void EnsureTrailingBackslash(string& Directory);
void EnsureLeadingBackslash(string& Directory);


#endif

