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
//     $Date: 5/13/98 12:03a $
//  $Modtime: 4/27/98 6:50a $
// $Revision: 20 $
//  $Archive: /Interscope/Thebe/InstallMaster/Portable.H $
// $Workfile: Portable.H $
//-----------------------------------------------------------------------

#ifndef __Portability_H__
#define __Portability_H__

#if !defined(__WIN32__) && !defined(_WIN32)
#ifndef STRICT
#define STRICT
#endif
#endif // __WIN32__

#include <Windows.H>
#include <TChar.H>


//--- UNICODE helpers --------------------------------------------------

#ifdef _UNICODE
    #ifndef UNICODE
    #define UNICODE
    #endif

#include "Winnls.H"

#endif


//--- UNICODE string macros --------------------------------------------

// Please note that any LPCSTRs in the the code
// could be intentially left for ANSI interface to system calls

#ifdef _UNICODE
	#define STRLEN      wcslen
	#define STRCPY      wcscpy
	#define STRCAT		wcscat
	#define STRNCPY     wcsncpy
    #define STRNCAT     wcsncat
	#define STRCMP      wcscmp
	#define STRICMP     _wcsicmp
	#define STRNCMP     wcsncmp
	#define STRNICMP    _wcsnicmp		
	#define ITOA        _itow
	#define LTOA        _ltow
	#define ATOI        _wtoi
	#define ATOL        _wtol
    #define SSCANF      swscanf
	#define SPRINTF     swprintf
	#define	VSPRINTF    vswprintf
	#define STRDUP      _wcsdup
	#define STRTOK      wcstok
	#define STRCHR      wcschr
	#define STRRCHR		wcsrchr
	#define STRSTR		wcsstr
	#define STRUPR		_wcsupr
	#define	STRLWR		_wcslwr
    #define TOUPPER     towupper
    #define TOLOWER     towlower
    #define ISALPHA     iswalpha
    #define ISALNUM     iswalnum
    #define ISDIGIT     iswdigit    
    #define ISXDIGIT    iswxdigit
    #define ISSPACE     iswspace
    #define ISPUNCT     iswpunct
    #define ISLOWER     iswlower
    #define ISUPPER     iswupper
    #define ISCNTRL     iswcntrl
#else
	#define STRLEN      lstrlen
	#define STRCPY      lstrcpy
	#define STRCAT      lstrcat
	#define STRNCPY     lstrcpyn
    #define STRNCAT     strncat
	#define STRCMP      lstrcmp
	#define STRICMP     lstrcmpi
	#define STRNCMP     strncmp
	#define STRNICMP    strnicmp	
	#define ITOA        _itoa
	#define LTOA        _ltoa
	#define ATOI        atoi
	#define ATOL        atol
    #define SSCANF      sscanf
	#define SPRINTF     wsprintf
	#define VSPRINTF    wvsprintf	
	#define STRDUP      strdup
	#define STRTOK      strtok
	#define STRCHR      strchr
	#define STRRCHR		strrchr
	#define STRSTR		strstr
	#define STRUPR		_strupr
	#define	STRLWR		_strlwr
    #define TOUPPER     toupper
    #define TOLOWER     tolower
    #define ISALPHA     isalpha
    #define ISALNUM     isalnum
    #define ISDIGIT     isdigit    
    #define ISXDIGIT    isxdigit
    #define ISSPACE     isspace
    #define ISPUNCT     ispunct
    #define ISLOWER     islower
    #define ISUPPER     isupper
    #define ISCNTRL     iscntrl
#endif


//--- UNICODE file macros ----------------------------------------------

#ifdef _UNICODE
	#define SPLITPATH   _wsplitpath
	#define FULLPATH    _wfullpath
	#define MAKEPATH    _wmakepath
	#define GETCWD      _wgetcwd
	#define REMOVE      _wremove
	#define ACCESS      _waccess
	#define CHMOD       _wchmod
	#define RENAME      _wrename
	#define STAT        _wstat
	#define MKDIR       _wmkdir
	#define CHDIR       _wchdir
	#define RMDIR       _wrmdir
    #define OPEN        _lopen
    #define CREATE      _lcreat
    #define CLOSE       _lclose
    #define READ        _hread
    #define WRITE       _hwrite
    #define SEEK        _llseek
#else
	#define SPLITPATH   _splitpath
	#define FULLPATH    _fullpath
	#define MAKEPATH    _makepath
	#define GETCWD      _getcwd
	#define REMOVE      remove
	#define ACCESS      _access
	#define CHMOD       _chmod
	#define RENAME      rename
	#define STAT        _stat
	#define MKDIR       _mkdir	
	#define CHDIR       _chdir
	#define RMDIR       _rmdir
    #define OPEN        _lopen
    #define CREATE      _lcreat
    #define CLOSE       _lclose
    #define READ        _hread
    #define WRITE       _hwrite
    #define SEEK        _llseek
#endif


//--- Portable function mappers ----------------------------------------

// These are getting obsolete, do not use them anymore if possible

// This file contains some macros designed to make it easier to keep
// single source for both 16 and 32 bit C++ applications.
// In 16 bit Windows, the small memory model is often used, with considerable
// use of the _far keyword to enable far data. This requires the use of 
// far versions of the runtime, such as _fstrcpy. These "model-independent"
// versions are not available in Win32, so we use these macros instead.

#ifndef LPDWORD
typedef DWORD FAR *LPDWORD;
#endif
#ifndef LPBYTE
typedef BYTE FAR *LPBYTE;
#endif

#ifdef _WIN32
    // 32-bit platforms (Windows95 and WindowsNT)
    #define FSTRCPY     strcpy
    #define FSTRSTR		strstr
    #define FSTRTOK		strtok
    #define FSTRICMP	stricmp
    #define FSTRLEN		strlen
    #define FSTRCAT		strcat
    #define FSTRRCHR	strrchr
#else
    // 16-bit platforms (Windows3.x)
    #define FSTRCPY		_fstrcpy
    #define FSTRSTR		_fstrstr
    #define FSTRTOK		_fstrtok
    #define FSTRICMP	_fstricmp
    #define FSTRLEN		_fstrlen
    #define FSTRCAT		_fstrcat
    #define FSTRRCHR	_fstrrchr

    #define LPCTSTR		LPCSTR
    #define LPTSTR      LPSTR
    #define TCHAR       char

    #define _T(x)       x
    #define _TEXT(x)    x
#endif


#endif

