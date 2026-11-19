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
//     $Date: 9/14/98 4:04a $
//  $Modtime: 9/14/98 2:52a $
// $Revision: 65 $
//  $Archive: /Interscope/Callisto/Path.Hpp $
// $Workfile: Path.Hpp $
//-----------------------------------------------------------------------

#ifndef __Path_Hpp__
#define __Path_Hpp__

// This class is usable both with and without MFC
// When used with MFC, it also has support 4 saving and loading from CArchive
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

#include <Sys\Types.H>
#include <Dos.H>
#include <String>
#include <Windows.H>

#include "Portable.H"
#include "DrvType.H"


using namespace std;


//--- Some special path types --------------------------------------------

// If you have static CPaths in your program, you cannot use these
// predefined paths because Windows support may not be enabled yet

enum SpecialDirectoryType
{
    CURRENT_DIRECTORY =0,
	WINDOWS_DIRECTORY,
	SYSTEM_DIRECTORY,
    SYSTEM_DRIVE_ROOT_DIRECTORY,
	MODULE_DIRECTORY,   // Only available in MFC projects
	TEMP_DIRECTORY,
    PROGRAM_FILES_DIRECTORY,
    COMMON_FILES_DIRECTORY,
    ACCESSORIES_DIRECTORY,
    MEDIA_DIRECTORY,
    DEVICE_DIRECTORY,
    USER_DESKTOP_DIRECTORY,
    USER_FAVORITES_DIRECTORY,
    USER_FONTS_DIRECTORY,
    USER_NETHOOD_DIRECTORY,
    USER_DOCUMENTS_DIRECTORY,
    USER_RECENT_DIRECTORY,
    USER_SENDTO_DIRECTORY,
    USER_TEMPLATES_DIRECTORY,
    USER_RECYCLE_DIRECTORY,
    USER_APPLICATION_DATA_DIRECTORY,
    USER_STARTMENU_DIRECTORY,
    USER_STARTMENU_STARTUP_DIRECTORY,
    USER_STARTMENU_PROGRAMS_DIRECTORY,
    COMMON_DESKTOP_DIRECTORY,
    COMMON_STARTMENU_DIRECTORY,
    COMMON_STARTMENU_STARTUP_DIRECTORY,
    COMMON_STARTMENU_PROGRAMS_DIRECTORY,
    LAST_SPECIAL
};


//--- Constants and defines ---------------------------------------------

LPCTSTR const DLL_EXTENSION = _T("dll");
LPCTSTR const INI_EXTENSION = _T("ini");
LPCTSTR const EXE_EXTENSION = _T("exe");
LPCTSTR const WILD_NAME_EXTENSION = _T("*.*");


//--- Forwards -----------------------------------------------------------

#ifdef __MFC__
class CPath;
CArchive& operator <<(CArchive& rArchive, const CPath& rPath);
CArchive& operator >>(CArchive& rArchive, CPath& rPath);
#endif


//--- Exception class 4 path(s) ------------------------------------------

class CPathException
{
public:
    DWORD m_dwErrorCode;

public:
    CPathException(DWORD code =0): m_dwErrorCode(code) {}
};


//--- Path class --------------------------------------------------------

// This class is used to represent pathnames, that is the name and 
// location of a file. CPaths are used when you want to refer to a file
// as a whole, or to the location of a file, as opposed to CFile, which is 
// used when you want to change the contents of the file
//
// The data representation for CPath consists of two data members. The 
// first, m_strPath, is a CString. This allows for easy operators to and 
// from strings. Using a string to represent the path allows for fully 
// qualified as well as relative paths. The class is fundamentally built 
// on the C runtime functions _splitpath and _makepath.
// 
// This class should suppport long filenames when compiled with a version
// of C++ runtime designed for them (e.g. Windows 95). It does not support
// UNC's as of yet (UNCs are of the form \\<server>\dir\dir\name.ext)

class CPath
{
// Data members
protected:
    string  m_strPath;
	DWORD   m_dwFindFileAttributes;
	HANDLE	m_hFindFile;

// Construction and destruction
public:    
    CPath();
    CPath(const CPath& rPath);
    CPath(LPCTSTR lpszPath);
    CPath(const string& strPath);
    CPath(SpecialDirectoryType special_dir);
    virtual ~CPath();

// Setup and cleanup
    inline void Init();
	inline void Exit();

// Operators
    CPath& operator  =(const CPath& rPath);
    CPath& operator  =(LPCTSTR lpszPath);
    CPath& operator  =(const string& strPath);
    BOOL   operator ==(const CPath& rPath) const;
    BOOL   operator !=(const CPath& rPath) const;
    operator LPCTSTR() const;
	operator string&() { return m_strPath; }

// Get path components
    void GetDrive(string& rDrive) const;
    void GetDriveDirectory(string& rDriveDirectory) const;
    void GetDirectory(string& rDirectory) const;
    void GetName(string& rName) const;
    void GetNameExtension(string& rNameExtension) const;
    void GetExtension(string& rExtension) const;
    void GetFullyQualified(string& rFullyQualified) const;
    void GetFullyQualifiedShort(string& rFullyQualifiedShort) const;
	void GetComponents(string* pDrive     =NULL, 
                       string* pDirectory =NULL, 
                       string* pName      =NULL, 
                       string* pExtension =NULL) const;

// Get other state
    BOOL IsEmpty() const { return m_strPath.empty(); }
    BOOL IsRelative() const;
    BOOL IsWild() const;
    BOOL IsValid() const; // Checks lexical correctness only

// Set path components
    void SetDrive(TCHAR chDrive);
    void SetDriveDirectory(LPCTSTR lpszDriveDirectory);
    void SetDirectory(LPCTSTR lpszDirectory, BOOL bEnsureAbsolute =FALSE);
    void SetName(LPCTSTR lpszName);
    void SetNameExtension(LPCTSTR lpszNameExtension);
    void SetExtension(LPCTSTR lpszExtension);

	void AppendDirectory(LPCTSTR lpszSubDirectory);
	void UpDirectory(string* pLastDirectory =NULL);

	void SetComponents(LPCTSTR lpszDrive, 
                       LPCTSTR lpszDirectory,
					   LPCTSTR lpszName, 
                       LPCTSTR lpszExtension);

// Set whole path
    void Empty() { m_strPath.erase(); }
    void MakeRoot();
    
    void CurrentDirectory();
    void WindowsDirectory();
    void SystemDirectory();
    void SystemDriveRootDirectory();
    void TempDirectory();
    void Module(HINSTANCE hInstance);
    void ModuleDirectory(HINSTANCE hInstance);

#ifdef __MFC__
    void Module();          // Uses AfxGetInstanceHandle 2 set itself 2 filename of current module
    void ModuleDirectory(); // Uses AfxGetInstanceHandle 2 set itself 2 location of current module
#endif

    void ProgramFilesDirectory();   // C:\Program Files
    void CommonFilesDirectory();    // C:\Program Files\Common Files
    void AccessoriesDirectory();    // C:\Program Files\Accessories
    void MediaDirectory();          // C:\Windows\Media
    void DeviceDirectory();         // C:\Windows\Inf

    void UserDesktopDirectory();
    void UserFavoritesDirectory();
    void UserFontsDirectory();
    void UserNetworkNeighbourhoodDirectory();
    void UserDocumentsDirectory();
    void UserRecentDirectory();
    void UserSendToDirectory();
    void UserTemplatesDirectory();
    void UserRecycleBinDirectory();
    void UserApplicationDataDirectory();
    void UserStartMenuDirectory();
    void UserStartMenuStartupDirectory();
    void UserStartMenuProgramsDirectory();

    void CommonDesktopDirectory();
    void CommonStartMenuDirectory();
    void CommonStartMenuStartupDirectory();
    void CommonStartMenuProgramsDirectory();

#ifdef __MFC__
    void PrivateProfile(); // .INI file in Windows directory
    void LocalProfile(LPCTSTR lpszName, LPCTSTR lpszExtension =NULL);
    void LocalProfile(UINT nNameResourceID, UINT nExtensionResourceID =0);
#endif

    void WindowsProfile(); // Win.INI
    void SystemProfile();  // System.INI

	BOOL CreateTempName(LPCTSTR lpcszPrefix);
    BOOL CreateTempDir(LPCTSTR lpcszPrefix, UINT nRetries =100);
    BOOL CreateRandomName(BOOL bMustNotExist =TRUE, UINT nRetries =1000);
	BOOL CreateSimilarName(BOOL bMustNotExist =TRUE, UINT nRetries =1000);

// Drive Information 
    UINT GetDriveType() const;
    BOOL IsRemovableDrive() const { return GetDriveType()==EX_DRIVE_REMOVABLE; }
    BOOL IsCDRomDrive() const     { return GetDriveType()==EX_DRIVE_CDROM; }
    BOOL IsNetworkDrive() const   { return GetDriveType()==EX_DRIVE_REMOTE; }
    BOOL IsRAMDrive() const       { return GetDriveType()==EX_DRIVE_RAMDISK; }

    DWORD DriveTotalSpaceBytes() const;
    DWORD DriveFreeSpaceBytes() const;
    DWORD GetDriveClusterSize() const;
    BOOL  GetDiskInfo(LPDWORD lpSectorsPerCluster,
                      LPDWORD lpBytesPerSector,
                      LPDWORD lpFreeClusters,
                      LPDWORD lpClusters) const;

// Directory information
    BOOL IsDirectory() const;
    BOOL DirectoryExists() const;
    BOOL IsDirectoryEmpty() const;

// File Information
    BOOL     IsFile() const { return !IsDirectory(); }
    BOOL     Exists() const;
    DWORD    GetSize() const; 
    DWORD    GetAttributes() const;
    BOOL     GetTime(FILETIME *lpCreated, FILETIME *lpAccessed, FILETIME *lpModified) const;
    FILETIME GetTimeCreated() const;
    FILETIME GetTimeLastModified() const;
    FILETIME GetTimeLastAccessed() const;

// Directory operations
    BOOL CreateDirectory(BOOL bCreateIntermediates =TRUE);
    void CreateDirectoryEx(BOOL bCreateIntermediates =TRUE); // Throws CPathException
	BOOL RemoveDirectory(BOOL bEvenIfNotEmpty =FALSE);
    BOOL RemoveDirectoryContent();
    BOOL ChangeDirectory();
	    
// File operations
	BOOL Delete(BOOL bEvenIfReadOnly =TRUE);
	BOOL Rename(LPCTSTR lpszNewPath);	
    BOOL CopyTo(LPCTSTR lpcszTargetFile, BOOL bOverwrite =TRUE);
    BOOL MoveTo(LPCTSTR lpcszTargetFile, BOOL bOverwrite =TRUE);
    BOOL SetAttributes(DWORD dwAttributes);
    BOOL SetTime(const FILETIME *lpCreated, const FILETIME *lpAccessed, const FILETIME *lpModified);
    BOOL SetTimeCreated(const FILETIME *lpCreated);
    BOOL SetTimeLastModified(const FILETIME *lpModified);
    BOOL SetTimeLastAccessed(const FILETIME *lpAccessed);

// Finders
    BOOL FindFirst(DWORD dwAttributes =_A_NORMAL);
    BOOL FindNext();

// Helpers
private:
    BOOL AttributesMatch(DWORD dwTargetAttributes, DWORD dwFileAttributes);

    BOOL ShellDirectory(int nShellFolderID);
    BOOL ShellDirectory2(int nShellFolderID);
    BOOL GetRegistryPath(HKEY hRootKey, LPCTSTR lpcszKeyName, LPCTSTR lpcszValueName, string &strPath);

// Friends
#ifdef __MFC__
    friend CArchive& operator <<(CArchive& rArchive, const CPath& rPath);
    friend CArchive& operator >>(CArchive& rArchive, CPath& rPath);
#endif
};


//--- Archive operator inlines ----------------------------------

#ifdef __MFC__
inline CArchive& operator <<(CArchive& rArchive, const CPath& rPath)
{
    CString temp(rPath.m_strPath.c_str());
    rArchive << temp;
    return rArchive;
}
#endif

#ifdef __MFC__
inline CArchive& operator >>(CArchive& rArchive, CPath& rPath)
{
    CString temp;
    rArchive >> temp;
    rPath.m_strPath =(LPCTSTR)temp;
    return rArchive;
}
#endif


#endif 
