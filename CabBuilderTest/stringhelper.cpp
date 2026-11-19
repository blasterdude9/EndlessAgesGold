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
//     $Date: 5/12/98 11:50p $
//  $Modtime: 4/27/98 6:51a $
// $Revision: 23 $
//  $Archive: /Interscope/Thebe/InstallMaster/StringHelper.Cpp $
// $Workfile: StringHelper.Cpp $
//-----------------------------------------------------------------------


#include "StdAfx.H"


#include "StringHelper.Hpp"


//--- Debugee --------------------------------------------------------------

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#ifdef __MFC__
#define new DEBUG_NEW
#endif // __MFC__
#endif // _DEBUG


#ifdef __MFC__
//------------------------------------------------------------------------
// Pre     :
// Post    : 
// Globals :
// I/O     :
// Task    : Set the length of Text to nLength. Like putting a '\0' at 
//           the nLength spot
//------------------------------------------------------------------------
void SetStringLength(CString& Text, int nLength)
{
	Text.GetBufferSetLength(nLength);
	Text.ReleaseBuffer();
}
#endif


#ifdef __MFC__
//------------------------------------------------------------------------
// Pre     :
// Post    : 
// Globals :
// I/O     :
// Task    : Remove first character (if any) if it's chLeading
//------------------------------------------------------------------------
void StripLeadingChar(CString& rText, TCHAR chLeading)
{
	int	nLength =rText.GetLength();
	if(nLength == 0)
		return;

	if(rText[0] == chLeading)
		rText =rText.Right(nLength-1);
}
#endif

//------------------------------------------------------------------------
// Pre     :
// Post    : 
// Globals :
// I/O     :
// Task    : Remove first character (if any) if it's chLeading
//------------------------------------------------------------------------
void StripLeadingChar(string& rText, TCHAR chLeading)
{
    string::size_type nLength =rText.length();
	if(nLength == 0)
		return;

	if(rText[0] == chLeading)
		rText =rText.substr(1);
}


#ifdef __MFC__
//------------------------------------------------------------------------
// Pre     :
// Post    : 
// Globals :
// I/O     :
// Task    : Remove first character if \
//------------------------------------------------------------------------
void StripLeadingBackslash(CString& Directory)
{
	int	nLength =Directory.GetLength();

    // If Directory is of the form '\', don't do it
	if(nLength <= 1)
		return;

	if(Directory[0] == DIRECTORY_DELIMITER)
		Directory=Directory.Right(nLength-1);
}
#endif

//------------------------------------------------------------------------
// Pre     :
// Post    : 
// Globals :
// I/O     :
// Task    : Remove first character if \
//------------------------------------------------------------------------
void StripLeadingBackslash(string& Directory)
{
	string::size_type nLength =Directory.length();

    // If Directory is of the form '\', don't do it
	if(nLength <= 1)
		return;

	if(Directory[0] == DIRECTORY_DELIMITER)
		Directory =Directory.substr(1);
}


#ifdef __MFC__
//------------------------------------------------------------------------
// Pre     :
// Post    : 
// Globals :
// I/O     :
// Task    : Remove last character (if any) if it's chTrailing
//------------------------------------------------------------------------
void StripTrailingChar(CString& rText, TCHAR chTrailing)
{
	int	nLength =rText.GetLength();
	if(nLength == 0)
		return;
	
	if(rText[nLength-1] == chTrailing)
		SetStringLength(rText,nLength-1);	
}
#endif

//------------------------------------------------------------------------
// Pre     :
// Post    : 
// Globals :
// I/O     :
// Task    : Remove last character (if any) if it's chTrailing
//------------------------------------------------------------------------
void StripTrailingChar(string& rText, TCHAR chTrailing)
{
	string::size_type nLength =rText.length();
	if(nLength == 0)
		return;
	
	if(rText[nLength-1] == chTrailing)
		rText.resize(nLength-1);
}


#ifdef __MFC__
//------------------------------------------------------------------------
// Pre     :
// Post    : 
// Globals :
// I/O     :
// Task    : Remove last character if \
//------------------------------------------------------------------------
void StripTrailingBackslash(CString& rDirectory)
{
	int	nLength =rDirectory.GetLength();
	
	// If Directory is of the form '\', don't do it
	if(nLength <= 1)
		return;
		
	if(rDirectory[nLength-1] == DIRECTORY_DELIMITER)
		SetStringLength(rDirectory,nLength-1);
}
#endif

//------------------------------------------------------------------------
// Pre     :
// Post    : 
// Globals :
// I/O     :
// Task    : Remove last character if \
//------------------------------------------------------------------------
void StripTrailingBackslash(string& rDirectory)
{
	string::size_type nLength =rDirectory.length();
	
	// If Directory is of the form '\', don't do it
	if(nLength <= 1)
		return;
		
	if(rDirectory[nLength-1] == DIRECTORY_DELIMITER)
		rDirectory.resize(nLength-1);
}


#ifdef __MFC__
//------------------------------------------------------------------------
// Pre     :
// Post    : 
// Globals :
// I/O     :
// Task    : Add a backslash to the end of Directory if there is
//           not already one there
//------------------------------------------------------------------------
void EnsureTrailingBackslash(CString& Directory)
{
	int	nLength =Directory.GetLength();

	if(Directory.IsEmpty() || (Directory[nLength-1] != DIRECTORY_DELIMITER))
		Directory +=DIRECTORY_DELIMITER;
}
#endif

//------------------------------------------------------------------------
// Pre     :
// Post    : 
// Globals :
// I/O     :
// Task    : Add a backslash to the end of Directory if there is
//           not already one there
//------------------------------------------------------------------------
void EnsureTrailingBackslash(string& Directory)
{
	string::size_type nLength =Directory.length();

	if(Directory.empty() || (Directory[nLength-1] != DIRECTORY_DELIMITER))
		Directory +=DIRECTORY_DELIMITER;
}


#ifdef __MFC__
//------------------------------------------------------------------------
// Pre     :
// Post    : 
// Globals :
// I/O     :
// Task    : Add a backslash to the beginning of Directory if there
//           is not already one there
//------------------------------------------------------------------------
void EnsureLeadingBackslash(CString& Directory)
{
	if(Directory.IsEmpty() || (Directory[0] != DIRECTORY_DELIMITER))
		Directory = DIRECTORY_DELIMITER + Directory;
}
#endif

//------------------------------------------------------------------------
// Pre     :
// Post    : 
// Globals :
// I/O     :
// Task    : Add a backslash to the beginning of Directory if there
//           is not already one there
//------------------------------------------------------------------------
void EnsureLeadingBackslash(string& Directory)
{
	if(Directory.empty() || (Directory[0] != DIRECTORY_DELIMITER))
    {
        string temp =Directory;
		Directory   =DIRECTORY_DELIMITER;
        Directory  +=temp;
    }
}

