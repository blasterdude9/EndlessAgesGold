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
//     $Date: 5/13/98 12:04a $
//  $Modtime: 4/27/98 6:51a $
// $Revision: 11 $
//  $Archive: /Interscope/Thebe/InstallMaster/Trace.H $
// $Workfile: Trace.H $
//-----------------------------------------------------------------------

#ifndef __Trace_H__
#define __Trace_H__

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

#if defined(_DEBUG) || defined(__DEBUG__)
#include <CrtDbg.H>
#endif


//--- Trace function(s) -------------------------------------------------

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_DEBUG) || defined(__DEBUG__)
void _cdecl InterscopeTrace(LPCTSTR lpszFormat, ...);
#else
inline void _cdecl InterscopeNoTrace(LPCTSTR, ...) { }
#endif

#ifdef __cplusplus
}
#endif


//--- TRACE macro(s) ----------------------------------------------------

#if defined(_DEBUG) || defined(__DEBUG__)
// Debug mode 
#ifndef TRACE
#define TRACE   InterscopeTrace
#endif
#define TRACEX  InterscopeTrace
// Release mode
#else
#ifndef TRACE
#define TRACE   1 ? (void)0 : InterscopeNoTrace
#endif
#define TRACEX  1 ? (void)0 : InterscopeNoTrace
#endif


//--- Function trace with indenting -------------------------------------

#ifdef __cplusplus // This stuff only available in C++

#if defined(_DEBUG) || defined(__DEBUG__)

class CTraceIndentLevel
{
// Data members
private:
    static int m_nIndent;

// Construction and destruction
public:
    CTraceIndentLevel()  { m_nIndent++; }
    ~CTraceIndentLevel() { m_nIndent--; }

friend void _cdecl InterscopeTrace(LPCTSTR lpszFormat, ...);
};

#endif

#if defined(_DEBUG) || defined(__DEBUG__)
#define TRACEFN CTraceIndentLevel __indent_level_tracker; TRACEX
#else
#define TRACEFN TRACEX
#endif

#endif // __cplusplus


//--- Goodies to get names of things -------------------------------------

#if defined(__MFC__) && (defined(_DEBUG) || defined(__DEBUG__))

CString DebugName(CWnd *pWnd); // Get name of window
CString DebugName(UINT  uMsg); // Get name of WM_ message

#endif // __MFC__


#endif
