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
//     $Date: 5/12/98 11:49p $
//  $Modtime: 4/27/98 6:50a $
// $Revision: 68 $
//  $Archive: /Interscope/Thebe/InstallMaster/Cabinet.cpp $
// $Workfile: Cabinet.cpp $
//-----------------------------------------------------------------------

#include "StdAfx.H"

#include <Sys/Stat.H>
#include <IO.H>
#include <FCntl.H>
#include <DOS.H>
#include <TypeInfo.H>

#ifndef __MFC__
#include "Trace.H"
#endif

#include "AssertX.H"
#include "Reminder.Hpp"
#include "Cabinet.Hpp"


//--- Debugee --------------------------------------------------------------

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#ifdef __MFC__
#define new DEBUG_NEW
#endif // __MFC__
#endif // _DEBUG


//-------------------------------------------------------------------------
// Pre     :
// Post    :
// Globals :
// I/O     :
// Task    : Allocate memory
//-------------------------------------------------------------------------
void HUGE * FAR DIAMONDAPI CCabinetBase::MemAlloc(ULONG cbSize)
{
#ifdef __TRACE_CAB_MEMORY__
    TRACEFN(_T("CCabinetBase::MemAlloc(%lu)"),cbSize);
#endif

    return malloc(cbSize);
}

//-------------------------------------------------------------------------
// Pre     :
// Post    :
// Globals :
// I/O     :
// Task    : Free allocated memory
//-------------------------------------------------------------------------
void FAR DIAMONDAPI CCabinetBase::MemFree(void HUGE *ptrMemory)
{
#ifdef __TRACE_CAB_MEMORY__
    TRACEFN(_T("CCabinetBase::MemFree(0x%lx)\n"),ptrMemory);
#endif

    free(ptrMemory);
}

//-------------------------------------------------------------------------
// Pre     : If not NULL, the wild parameter pv is a pointer 2 an instance 
//           of a class CCabinetBase
// Post    :
// Globals :
// I/O     :
// Task    : Open a file
//-------------------------------------------------------------------------
int FAR DIAMONDAPI CCabinetBase::FileOpen(char FAR *pszFile, int oFlag, int pMode, int FAR *ptrErr, void FAR *pv)
{
#if defined(__TRACE_CAB_COMPRESSION__) || defined(__TRACE_CAB_EXTRACTION__)
    TRACEFN(_T("CCabinetBase::FileOpen(%s,0x%04x,0x%04x)\n"),pszFile,oFlag,pMode);
#endif

    // No error yet
    ASSERTX(ptrErr);
    *ptrErr =NOERROR;

    // Text-mode files are not supported, everything is
    // considered 2 be raw binary
    if((oFlag & _O_TEXT) == _O_TEXT)
    {
        ASSERTX(FALSE);
        *ptrErr =EINVAL;
        return -1;
    }

    // Extract access mode from oFlag
    DWORD dwDesiredAccess =GENERIC_READ;
    if((oFlag & _O_WRONLY) == _O_WRONLY)
        dwDesiredAccess =GENERIC_WRITE;
    else if((oFlag & _O_RDWR) == _O_RDWR)
        dwDesiredAccess |=GENERIC_WRITE;

    if(dwDesiredAccess == GENERIC_READ)
        // When we only want 2 read from a file, make sure
        // we open it with share-read, otherwise we won't be 
        // able 2 read from files which are in use by Windows
        pMode |=_S_IREAD;

    // Extract creation mode from oFlag
    DWORD dwCreateMode;
    if((oFlag & _O_CREAT) == _O_CREAT)
    {
        // Creating a file
        if((oFlag & _O_EXCL) == _O_EXCL)
            dwCreateMode =CREATE_NEW;
        else if((oFlag & _O_TRUNC) == _O_TRUNC)
            dwCreateMode =CREATE_ALWAYS;
        else
            dwCreateMode =OPEN_ALWAYS;
    }
    else
    {
        // Opening a file
        if((oFlag & _O_TRUNC) == _O_TRUNC)
            dwCreateMode =TRUNCATE_EXISTING;
        else
            dwCreateMode =OPEN_EXISTING;
    }

    // Extract other flags from oFlag
    DWORD dwFlagsAttrs =FILE_ATTRIBUTE_NORMAL;
    if((oFlag & _O_RANDOM) == _O_RANDOM)
        dwFlagsAttrs |=FILE_FLAG_RANDOM_ACCESS;
    if((oFlag & _O_SEQUENTIAL) == _O_SEQUENTIAL)
        dwFlagsAttrs |=FILE_FLAG_SEQUENTIAL_SCAN;
    if((oFlag & _O_CREAT) == _O_CREAT)
    {
        // Thease are valid only when creating a new file
        if((oFlag & _O_SHORT_LIVED) == _O_SHORT_LIVED)
            dwFlagsAttrs |=FILE_ATTRIBUTE_TEMPORARY;
        if((oFlag & _O_TEMPORARY) == _O_TEMPORARY)
            dwFlagsAttrs |=FILE_FLAG_DELETE_ON_CLOSE;
    }

    // Converting share mode
    DWORD dwShareMode =0;
    if((pMode & _S_IREAD) == _S_IREAD)
        dwShareMode |=FILE_SHARE_READ;
    if((pMode & _S_IWRITE) == _S_IWRITE)
        dwShareMode |=FILE_SHARE_WRITE;

    HANDLE hFile   =CreateFile(pszFile,dwDesiredAccess,dwShareMode,NULL,dwCreateMode,dwFlagsAttrs,NULL);
    BOOL   bResult =(hFile != INVALID_HANDLE_VALUE);

    if(bResult)
    {
        if((oFlag & _O_APPEND) == _O_APPEND)
            // Move file pointer 2 the end of the file first
            SetFilePointer(hFile,0,NULL,FILE_END);
    }
    else
    {
        // Store the error code
        DWORD dwError =GetLastError();
        switch(dwError)
        {
            case ERROR_ACCESS_DENIED:
            case ERROR_LOCK_VIOLATION:
            case ERROR_SHARING_VIOLATION:
                *ptrErr =EACCES;
                break;

            case ERROR_INVALID_PARAMETER:
                *ptrErr =EINVAL;
                break;

            case ERROR_FILE_EXISTS:
            case ERROR_ALREADY_EXISTS:
                *ptrErr =EEXIST;
                break;

            case ERROR_TOO_MANY_OPEN_FILES:
                *ptrErr =EMFILE;
                break;

            case ERROR_FILE_NOT_FOUND:
            case ERROR_PATH_NOT_FOUND:
                *ptrErr =ENOENT;
                break;

            default:
                *ptrErr =(int)dwError;
        }
    }

    return bResult ? (int)hFile : -1;
}

//-------------------------------------------------------------------------
// Pre     : If not NULL, the wild parameter pv is a pointer 2 an instance 
//           of a class CCabinetBase
// Post    :
// Globals :
// I/O     :
// Task    : Read cbRead bytes from file specified by handle into buffer ptrBuffer
//-------------------------------------------------------------------------
UINT FAR DIAMONDAPI CCabinetBase::FileRead(int hFile, void FAR *ptrBuffer, UINT cbRead, int FAR *ptrErr, void FAR *pv)
{
#if defined(__TRACE_CAB_COMPRESSION__) || defined(__TRACE_CAB_EXTRACTION__)
    TRACEFN(_T("CCabinetBase::FileRead(%d,0x%p,%u)\n"),hFile,ptrBuffer,cbRead);
#endif

    // No error yet
    ASSERTX(ptrErr);
    *ptrErr =NOERROR;

    DWORD dwBytesRead;
    BOOL  bResult =ReadFile((HANDLE)hFile,ptrBuffer,cbRead,&dwBytesRead,NULL);

    // Check 4 end-of-file condition
    if(bResult && (dwBytesRead == 0))
        // EOF
        return 0;

    if(!bResult)
        // Store the error code
        *ptrErr =EBADF;

    return bResult ? (UINT)dwBytesRead : (UINT)-1;
}

//-------------------------------------------------------------------------
// Pre     : If not NULL, the wild parameter pv is a pointer 2 an instance 
//           of a class CCabinetBase
// Post    :
// Globals :
// I/O     :
// Task    : Write cbWrite bytes from buffer ptrBuffer int file specified by handle
//-------------------------------------------------------------------------
UINT FAR DIAMONDAPI CCabinetBase::FileWrite(int hFile, void FAR *ptrBuffer, UINT cbWrite, int FAR *ptrErr, void FAR *pv)
{
#if defined(__TRACE_CAB_COMPRESSION__) || defined(__TRACE_CAB_EXTRACTION__)
    TRACEFN(_T("CCabinetBase::FileWrite(%d,0x%p,%u)\n"),hFile,ptrBuffer,cbWrite);
#endif

    // No error yet
    ASSERTX(ptrErr);
    *ptrErr =NOERROR;

    DWORD dwBytesWritten;
    BOOL  bResult =WriteFile((HANDLE)hFile,ptrBuffer,cbWrite,&dwBytesWritten,NULL);

    if(!bResult)
    {
        // Store the error code
        DWORD dwError =GetLastError();
        switch(dwError)
        {
            case ERROR_HANDLE_DISK_FULL:
            case ERROR_DISK_FULL:
                *ptrErr =ENOSPC;
                break;

            default:
                *ptrErr =EBADF;
        }
    }

    return bResult ? (UINT)dwBytesWritten : (UINT)-1;
}

//-------------------------------------------------------------------------
// Pre     : If not NULL, the wild parameter pv is a pointer 2 an instance 
//           of a class CCabinetBase
// Post    :
// Globals :
// I/O     :
// Task    : Seek the file pointer
//-------------------------------------------------------------------------
long FAR DIAMONDAPI CCabinetBase::FileSeek(int hFile, long offset, int seekType, int FAR *ptrErr, void FAR *pv)
{
#if defined(__TRACE_CAB_COMPRESSION__) || defined(__TRACE_CAB_EXTRACTION__)
    TRACEFN(_T("CCabinetBase::FileSeek(%d,%ld,%d)\n"),hFile,offset,seekType);
#endif

    // No error yet
    ASSERTX(ptrErr);
    *ptrErr =NOERROR;

    // Convert seek type
    DWORD dwMoveMethod;
    switch(seekType)
    {
        case SEEK_SET:
            dwMoveMethod =FILE_BEGIN;
            break;

        case SEEK_CUR:
            dwMoveMethod =FILE_CURRENT;
            break;

        case SEEK_END:
            dwMoveMethod =FILE_END; 
            break;

        default:
            // This should never happen
            ASSERTX(FALSE);
            *ptrErr =EINVAL;
            return -1;
    }

    DWORD dwNewOffset =SetFilePointer((HANDLE)hFile,offset,NULL,dwMoveMethod);
    BOOL  bResult =(dwNewOffset != (DWORD)-1);

    if(!bResult)
    {
        // Store the error code
        DWORD dwError =GetLastError();
        switch(dwError)
        {
            case ERROR_INVALID_PARAMETER:
                *ptrErr =EINVAL;
                break;

            default:
                *ptrErr =EBADF;
        }
    }

    return bResult ? (long)dwNewOffset : -1;
}

//-------------------------------------------------------------------------
// Pre     : If not NULL, the wild parameter pv is a pointer 2 an instance 
//           of a class CCabinetBase
// Post    :
// Globals :
// I/O     :
// Task    : Close this file
//-------------------------------------------------------------------------
int FAR DIAMONDAPI CCabinetBase::FileClose(int hFile, int FAR *ptrErr, void FAR *pv)
{
#if defined(__TRACE_CAB_COMPRESSION__) || defined(__TRACE_CAB_EXTRACTION__)
    TRACEFN(_T("CCabinetBase::FileClose(%d)\n"),hFile);
#endif

    // No error yet
    ASSERTX(ptrErr);
    *ptrErr =NOERROR;

    BOOL bResult =CloseHandle((HANDLE)hFile);

    if(!bResult)
        // Store the error code
        *ptrErr =EBADF;
    
    return bResult ? 0 : -1;
}


#ifdef __CAB_BUILD__
//-------------------------------------------------------------------------
// Pre     :
// Post    :
// Globals :
// I/O     :
// Task    : Create a cabinet builder object
//-------------------------------------------------------------------------
CCabinetBuilder::CCabinetBuilder(USHORT nID, ULONG cbFirstCabSize, ULONG cbCabSize, ULONG cbTreshold):
                 m_cbFirstCabSize(cbFirstCabSize),
                 m_cbCabSize(cbCabSize),
                 m_cbTreshold(cbTreshold),
                 m_nID(nID),
                 m_cbReserveCFHeader(0),
                 m_cbReserveCFFolder(0),
                 m_cbReserveCFData(0),
                 m_hContext(NULL)
{
#ifdef __TRACE_CAB_COMPRESSION__
    TRACEFN(_T("CCabinetBuilder::CCabinetBuilder\n"));
#endif

    // Inited, but before we can do actual compression client must call InitCabinet
}
#endif



#ifdef __CAB_BUILD__
//-------------------------------------------------------------------------
// Pre     : The wild parameter pv is a pointer 2 an instance of class CCabinetBuilder 
// Post    :
// Globals :
// I/O     :
// Task    : Compression interface wants 2 delete this file
//-------------------------------------------------------------------------
int FAR DIAMONDAPI CCabinetBuilder::FileDelete(char FAR *pszFile, int FAR *ptrErr, void FAR *pv)
{
#ifdef __TRACE_CAB_COMPRESSION__
    TRACEFN(_T("CCabinetBuilder::FileDelete(%s)\n"),pszFile);
#endif

    // No error yet
    ASSERTX(ptrErr);
    *ptrErr =NOERROR;

    BOOL bResult =DeleteFile(pszFile);

    if(!bResult)
    {
        // Store the error code
        DWORD dwError =GetLastError();
        switch(dwError)
        {
            case ERROR_ACCESS_DENIED:
            case ERROR_LOCK_VIOLATION:
            case ERROR_SHARING_VIOLATION:
                *ptrErr =EACCES;
                break;

            case ERROR_FILE_NOT_FOUND:
            case ERROR_PATH_NOT_FOUND:
                *ptrErr =ENOENT;
                break;

            default:
                *ptrErr =(int)dwError;
        }
    }

    return bResult;
}
#endif

#ifdef __CAB_BUILD__
//-------------------------------------------------------------------------
// Pre     : pszTempName points 2 a buffer to receive complete tempfile name
//           cbTempName  is size of pszTempName buffer
//           The wild parameter pv is a pointer 2 an instance of class CCabinetBuilder 
// Post    : Return TRUE on success
// Globals :
// I/O     :
// Task    : Compression interface requests a temporary filename
//-------------------------------------------------------------------------
BOOL FAR DIAMONDAPI CCabinetBuilder::GetTempFile(char *pszTempName, int cbTempName, void FAR *pv)
{
#ifdef __TRACE_CAB_COMPRESSION__
    TRACEFN(_T("CCabinetBuilder::GetTempFile... "));
#endif

    // Manufacture temporary path
    CPath temp_path;
    temp_path.TempDirectory();
    if(temp_path.CreateTempName(_T("int")))
    {
        // Got a temp filename
        string strTempFileName((LPCTSTR)temp_path);
        if(cbTempName < strTempFileName.length())
            // Buffer too small
            return FALSE;

        STRCPY(pszTempName,strTempFileName.c_str());

        #ifdef __TRACE_CAB_COMPRESSION__
        TRACEX(_T("%s\n"),pszTempName);
        #endif

        return TRUE;
    }

#ifdef __TRACE_CAB_COMPRESSION__
    TRACEX(_T("Failed\n"));
#endif

    // Could not get a temp filename
    return FALSE;
}
#endif

#ifdef __CAB_BUILD__
//-------------------------------------------------------------------------
// Pre     : The wild parameter pv is a pointer 2 an instance of class CCabinetBuilder 
// Post    : 
// Globals :
// I/O     :
// Task    : Opens a file and returns its datestamp, timestamp, and attributes at
//           compression interface request
//-------------------------------------------------------------------------
int FAR DIAMONDAPI CCabinetBuilder::GetFileInfo(char *pszName, USHORT *pdate, USHORT *ptime, USHORT *pattribs, int FAR *ptrErr, void FAR *pv)
{
#ifdef __TRACE_CAB_COMPRESSION__
    TRACEFN(_T("CCabinetBuilder::GetFileInfo\n"));
#endif

    // No error yet
    ASSERTX(ptrErr);
    *ptrErr =NOERROR;

    BY_HANDLE_FILE_INFORMATION	finfo;
	FILETIME					filetime;
	HANDLE						handle;
    DWORD                       attrs;

    // We need a Win32 type handle to get file date/time
	handle =CreateFile(pszName,
		               GENERIC_READ,
		               FILE_SHARE_READ,
		               NULL,
		               OPEN_EXISTING,
		               FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN,
		               NULL);
	if(handle == INVALID_HANDLE_VALUE)
    {
        // Could not open file
        goto LABEL_InfoError;
    }

	if(!GetFileInformationByHandle(handle,&finfo))
	{
        // Could not retrieve file info
		CloseHandle(handle);
		goto LABEL_InfoError;
	}
   
	FileTimeToLocalFileTime(&finfo.ftLastWriteTime,&filetime);
	FileTimeToDosDateTime(&filetime,pdate,ptime);

    if((attrs =GetFileAttributes(pszName)) == 0xFFFFFFFF)
    {
        // Failure
        *pattribs =0;
    }
    else
    {
        // Mask out all other bits except these, since other
        // bits are used by the cabinet format 2 indicate a
        // special meaning
        *pattribs =(int)(attrs & (_A_RDONLY | _A_SYSTEM | _A_HIDDEN | _A_ARCH));
    }

    CloseHandle(handle);

    // We've got all the info we needed,
    // now open the file so that the class' file utility
    // functions can work with it
	return FileOpen(pszName,_O_RDONLY | _O_BINARY,0,ptrErr,pv);

LABEL_InfoError:
    // Store the error code
    DWORD dwError =GetLastError();
    switch(dwError)
    {
        case ERROR_ACCESS_DENIED:
        case ERROR_LOCK_VIOLATION:
        case ERROR_SHARING_VIOLATION:
            *ptrErr =EACCES;
            break;

        case ERROR_INVALID_PARAMETER:
            *ptrErr =EINVAL;
            break;

        case ERROR_TOO_MANY_OPEN_FILES:
            *ptrErr =EMFILE;
            break;

        case ERROR_FILE_NOT_FOUND:
        case ERROR_PATH_NOT_FOUND:
            *ptrErr =ENOENT;
            break;

        default:
            *ptrErr =(int)dwError;
    }

	return -1;
}
#endif

#ifdef __CAB_BUILD__
//-------------------------------------------------------------------------
// Pre     : The wild parameter pv is a pointer 2 an instance of class CCabinetBuilder 
// Post    : Return FALSE 2 abort cabinet creation
// Globals :
// I/O     :
// Task    : The compression interface wishes to create a new cabinet, which 
//           will happen whenever the size of the cabinet is about to exceed 
//           the media size specified
//           The cabinet name MUST be modified. Also, the disk name, media 
//           size, folder threshold, etc. parameters may also be modified
//-------------------------------------------------------------------------
BOOL FAR DIAMONDAPI CCabinetBuilder::GetNextCabinet(PCCAB pccab, ULONG cbPrevCab, void FAR *pv)
{
#ifdef __TRACE_CAB_COMPRESSION__
    TRACEFN(_T("CCabinetBuilder::GetNextCabinet(%u)\n"),cbPrevCab);
#endif

    if(pccab)
    {
        // Cabinet counters already incremented by FCI, updating the names
        CCabinetBuilder *pcb =(CCabinetBuilder *)pv;
        ASSERTX(pcb);

        pccab->iDisk++;

        pcb->ManufactureDiskName(pccab);
        pcb->ManufactureCabinetName(pccab);

        // Update the media size after the first cabinet
        pccab->cb =pcb->m_cbCabSize;
    }

    return TRUE;
}
#endif

#ifdef __CAB_BUILD__
//---------------------------------------------------------------------------
// Pre     : pszFile is name of file
//           cbFile is 
// Post    : Return FALSE 2 abort
// Globals : 
// I/O     : 
// Task    : Simple notification that a file was committed 2 a cabinet
//---------------------------------------------------------------------------
BOOL CCabinetBuilder::NotifyPlaced(char *pszFile, long cbFile, BOOL bContinuation, int nDisk, char *pszCabinet)
{
#ifdef __TRACE_CAB_COMPRESSION__
    TRACEFN(_T("CCabinetBuilder::NotifyPlaced(%s,%ld,%u,%d,%s)\n"),pszFile,cbFile,bContinuation,nDisk,pszCabinet);
#endif

    return TRUE;
}
#endif

#ifdef __CAB_BUILD__
//-------------------------------------------------------------------------
// Pre     : The wild parameter pv is a pointer 2 an instance of class CCabinetBuilder 
// Post    : Return -1 to abort
// Globals :
// I/O     :
// Task    : Notification from compression interface that a file was 
//           committed 2 a cabinet
//-------------------------------------------------------------------------
int FAR DIAMONDAPI CCabinetBuilder::NotifyFilePlaced(PCCAB pccab, char *pszFile, long cbFile, BOOL bContinuation, void FAR *pv)
{
#ifdef __TRACE_CAB_COMPRESSION__
    TRACEFN(_T("CCabinetBuilder::NotifyFilePlaced(%s,%ld,%u,%d,%d)\n"),pszFile,cbFile,bContinuation,pccab->iDisk,pccab->iCab);
#endif

    // Call the client's notification proc too
    ASSERTX(pccab != NULL);
    CCabinetBuilder *pcb =(CCabinetBuilder *)pv;
    ASSERTX(pcb != NULL);

    BOOL ok_2_continue =pcb->NotifyPlaced(pszFile,cbFile,bContinuation,pccab->iDisk,pccab->szCab);

    return ok_2_continue ? 0 : -1;
}
#endif

#ifdef __CAB_BUILD__
//---------------------------------------------------------------------------
// Pre     : cbOriginal is the size of uncompressed block
//           cbCompressed is the size of the block after compression
// Post    : Return FALSE 2 abort
// Globals : 
// I/O     : 
// Task    : Simple notification that FCI is compressing a block in2 a folder
//           Useful 2 keep track of compression statistics
//---------------------------------------------------------------------------
BOOL CCabinetBuilder::NotifyCompress(ULONG cbOriginal, ULONG cbCompressed)
{
#ifdef __TRACE_CAB_COMPRESSION__
    TRACEFN(_T("CCabinetBuilder::NotifyCompress(%lu,%lu)\n"),cbOriginal,cbCompressed);
#endif

    return TRUE;
}
#endif

#ifdef __CAB_BUILD__
//---------------------------------------------------------------------------
// Pre     : cbSize is the total size of folder
//           cbWritten is the amount of folder copied to cabinet so far
// Post    : Return FALSE 2 abort
// Globals : 
// I/O     : 
// Task    : Simple notification that FCI is adding a folder 2 a cabinet
//---------------------------------------------------------------------------
BOOL CCabinetBuilder::NotifyFolderFlush(ULONG cbSize, ULONG cbWritten)
{
#ifdef __TRACE_CAB_COMPRESSION__
    TRACEFN(_T("CCabinetBuilder::NotifyFolderFlush(%lu,%lu)\n"),cbSize,cbWritten);
#endif

    return TRUE;
}
#endif

#ifdef __CAB_BUILD__
//---------------------------------------------------------------------------
// Pre     : cbEstimatedSize is the estimated cabinet size
//           cbActualSize is the actual cabinet size
// Post    : Return the desired cabinet size
// Globals : 
// I/O     : 
// Task    : Simple notification that FCI is writing out a cabinet
//---------------------------------------------------------------------------
ULONG CCabinetBuilder::NotifyCabinetFlush(ULONG cbEstimatedSize, ULONG cbActualSize)
{
#ifdef __TRACE_CAB_COMPRESSION__
    TRACEFN(_T("CCabinetBuilder::NotifyCabinetFlush(%lu,%lu)\n"),cbEstimatedSize,cbActualSize);
#endif

    return cbActualSize;
}
#endif

#ifdef __CAB_BUILD__
//-------------------------------------------------------------------------
// Pre     : The wild parameter pv is a pointer 2 an instance of class CCabinetBuilder 
//           If typeStatus == statusFile CFI is compressing a block into a folder
//                  cb1 = Size of compressed block
//                  cb2 = Size of uncompressed block
//
//           If typeStatus == statusFolder CFI is adding a folder to a cabinet
//                  cb1 = Amount of folder copied to cabinet so far
//                  cb2 = Total size of folder
//
//           If typeStatus == statusCabinet CFI is writing out a complete cabinet
//                  cb1 = Estimated cabinet size that was previously passed to GetNextCabinet()
//                  cb2 = Actual cabinet size
// Post    : Return -1 to signal that FCI should abort except if typeStatus == statusCabinet, when
//           the return value is the desired client size for cabinet file. FCI updates the maximum
//           cabinet size remaining using this value. This allows a client to generate multiple 
//           cabinets per disk, and have FCI limit the size correctly (the client can do cluster 
//           size rounding on the cabinet size!)
//           The client should either return cb2, or round cb2 up to some larger value and return that
// Globals :
// I/O     :
// Task    : Notification from compression interface that some action is in progress
//-------------------------------------------------------------------------
long FAR DIAMONDAPI CCabinetBuilder::NotifyProgress(UINT typeStatus, ULONG cb1, ULONG cb2, void FAR *pv)
{
#ifdef __TRACE_CAB_COMPRESSION__
    TRACEFN(_T("CCabinetBuilder::NotifyProgress(%u,%lu,%lu)\n"),typeStatus,cb1,cb2);
#endif

    // Handle notification
    long result =0;
    BOOL ok_2_continue;

    CCabinetBuilder *pcb =(CCabinetBuilder *)pv;
    ASSERTX(pcb);

    switch(typeStatus)
    {
        case statusFile:
            // A block is being compressed
            ok_2_continue =pcb->NotifyCompress(cb2,cb1);
            result        =ok_2_continue ? 0 : -1;
            break;

        case statusFolder:
            // Flushing a folder
            ok_2_continue =pcb->NotifyFolderFlush(cb2,cb1);
            result        =ok_2_continue ? 0 : -1;
            break;

        case statusCabinet:
            // Writing out a cabinet
            result =pcb->NotifyCabinetFlush(cb1,cb2);
            break;
    }

    return result;
}
#endif

#ifdef __CAB_BUILD__
//-------------------------------------------------------------------------
// Pre     : 
// Post    :
// Globals :
// I/O     :
// Task    : Build a disk name and store it in the passed CCAB struct
//-------------------------------------------------------------------------
void CCabinetBuilder::ManufactureDiskName(PCCAB cabinfo)
{
    if(cabinfo)
    {
        // Manufacture name
        TCHAR disk_name[CB_MAX_DISK_NAME];

        SPRINTF(disk_name,_T("%s%d"),m_strDiskName.c_str(),cabinfo->iDisk);
        STRNCPY(cabinfo->szDisk,disk_name,CB_MAX_DISK_NAME);
    }
}
#endif

#ifdef __CAB_BUILD__
//-------------------------------------------------------------------------
// Pre     : 
// Post    :
// Globals :
// I/O     :
// Task    : Build a cabinet name and store it in the passed CCAB struct
//-------------------------------------------------------------------------
void CCabinetBuilder::ManufactureCabinetName(PCCAB cabinfo)
{
    if(cabinfo)
    {
        // Manufacture name
        TCHAR cabinet_name[CB_MAX_CABINET_NAME];

        SPRINTF(cabinet_name,_T("%s%d.cab"),m_strCabinetName.c_str(),cabinfo->iCab);
        STRNCPY(cabinfo->szCab,cabinet_name,CB_MAX_CABINET_NAME);
    }
}
#endif

#ifdef __CAB_BUILD__
//-------------------------------------------------------------------------
// Pre     : If bCreatePath is TRUE, the .CAB file's path is created if not exists
// Post    : Return TRUE on success
// Globals :
// I/O     :
// Task    : Initialize the cabinet
//-------------------------------------------------------------------------
BOOL CCabinetBuilder::InitCabinet(LPCTSTR lpcszCabPath, LPCTSTR lpcszCabName, LPCTSTR lpcszDiskName, BOOL bCreatePath)
{
    // Init the name of the first cabinet and the number of the first disk
    TCHAR cabinet_name[CB_MAX_CABINET_NAME];
    SPRINTF(cabinet_name,_T("%s%d.cab"),lpcszCabName,1);
    m_nLastStartDisk =1;
    m_strLastCabinet =cabinet_name;

    // Check path (and create it if not exists and so specified)
    string cab_str(lpcszCabPath);
    EnsureTrailingBackslash(cab_str);

    m_CabinetPath =cab_str.c_str();
    if(!m_CabinetPath.DirectoryExists())
    {
        // Cabinet path does not exist
        if(!bCreatePath)
            return FALSE;

        m_CabinetPath.CreateDirectory();
    }

    // Prepare compression parameters
    m_strCabinetName =lpcszCabName;
    m_strDiskName    =lpcszDiskName;
    m_strDiskName   +=_T(" "); // So that the name of the disks will be e.g. "Disk 1" and we can use
                               // this name to prompt user for new disks when necessary

    ZeroMemory(&m_CabinetInfo,sizeof(m_CabinetInfo));

    m_CabinetInfo.setID             =m_nID;
    m_CabinetInfo.cb                =999999999;//MAX CAB SIZE such a descritive structure var :O//m_cbFirstCabSize;
    m_CabinetInfo.cbFolderThresh    =m_cbTreshold;
    m_CabinetInfo.cbReserveCFHeader =m_cbReserveCFHeader;
    m_CabinetInfo.cbReserveCFFolder =m_cbReserveCFFolder;
    m_CabinetInfo.cbReserveCFData   =m_cbReserveCFData;
    m_CabinetInfo.iCab              =1;
    m_CabinetInfo.iDisk             =1;
    STRNCPY(m_CabinetInfo.szCabPath,cab_str.c_str(),CB_MAX_CAB_PATH);

    ManufactureDiskName(&m_CabinetInfo);
    ManufactureCabinetName(&m_CabinetInfo);

    // Create a compression context
    m_hContext =FCICreate(&m_CabinetError,
                          NotifyFilePlacedProc(),
                          MemAllocProc(),
                          MemFreeProc(),
                          FileOpenProc(),
                          FileReadProc(),
                          FileWriteProc(),
                          FileCloseProc(),
                          FileSeekProc(),
                          FileDeleteProc(),
                          GetTempFileProc(),
                          &m_CabinetInfo,
                          this); // Pass an instance 2 ourselves, so that we are accessible from the statix

    if(m_hContext == NULL)
        // Could not create compression context
        // Client can call GetErrorCode() 2 get a code of what happened
        return FALSE;

    return (m_bStatus =TRUE); // Success
}
#endif

#ifdef __CAB_BUILD__
//-------------------------------------------------------------------------
// Pre     : lpcszFileName is the fully qualified name of file 2 add
//           lpcszStoreAs  is the name without path info under which 2 store the file in the cabinet
// Post    : Return TRUE on success
// Globals :
// I/O     :
// Task    : Add a file in2 the cabinet
//-------------------------------------------------------------------------
BOOL CCabinetBuilder::AddFile(LPTSTR lpszFileName, LPTSTR lpszStoreAs, BOOL bCompress, BOOL bExecuteOnExtract)
{
    // Check our internal status
    CPath file_path(lpszFileName);
    if(!file_path.Exists())
        return FALSE;

    // Strip path info from filename 2 add, in case we are not specified 
    // any name 2 store file being added as
    string strStrippedName;
    TCHAR  szStrippedName[MAX_PATH];

    file_path.GetNameExtension(strStrippedName);
    STRNCPY(szStrippedName,strStrippedName.c_str(),sizeof(szStrippedName)/sizeof(TCHAR));

    // Adding file...
    BOOL added_OK =FCIAddFile(m_hContext,
                              lpszFileName,
                              lpszStoreAs ? lpszStoreAs : szStrippedName,
                              bExecuteOnExtract,
                              GetNextCabinetProc(),
                              NotifyProgressProc(),
                              GetFileInfoProc(),
                              bCompress ? tcompTYPE_MSZIP : tcompTYPE_NONE);
    if(!added_OK)
    {
		return FALSE;
        // Could not add the file 2 cabinet, cleanup
        FCIDestroy(m_hContext);

        m_hContext =NULL;
        m_bStatus  =FALSE;
		//AfxMessageBox(lpszFileName);
		//AfxMessageBox(szStrippedName);
		
    }

    return added_OK;
}
#endif

#ifdef __CAB_BUILD__
//-------------------------------------------------------------------------
// Pre     :
// Post    : Return TRUE on success
// Globals :
// I/O     :
// Task    : Force the current feolder under construction 2 be completed 
//           immediately and written 2 disk
//-------------------------------------------------------------------------
BOOL CCabinetBuilder::FlushFolder()
{
    // Check our internal status
    if(!m_bStatus)
        return FALSE;

    return FCIFlushFolder(m_hContext,GetNextCabinetProc(),NotifyProgressProc());
}
#endif

#ifdef __CAB_BUILD__
//-------------------------------------------------------------------------
// Pre     :
// Post    : Return TRUE on success
// Globals :
// I/O     :
// Task    : Force the current cabinet under construction 2 be completed 
//           immediately and written 2 disk
//-------------------------------------------------------------------------
BOOL CCabinetBuilder::FlushCabinet()
{
    // Check our internal status
    if(!m_bStatus)
        return FALSE;

    return FCIFlushCabinet(m_hContext,FALSE,GetNextCabinetProc(),NotifyProgressProc());
}
#endif

#ifdef __CAB_BUILD__
//-------------------------------------------------------------------------
// Pre     :
// Post    :
// Globals :
// I/O     :
// Task    : Cleanup and delete a cabinet builder object
//-------------------------------------------------------------------------
CCabinetBuilder::~CCabinetBuilder()
{
#ifdef __TRACE_CAB_COMPRESSION__
    TRACEFN(_T("CCabinetBuilder::~CCabinetBuilder\n"));
#endif

    // Free the compression context (if any)
    if(m_bStatus && (m_hContext != NULL))
    {
        FCIDestroy(m_hContext);
    }
}
#endif


#ifdef __CAB_EXTRACT__
//-------------------------------------------------------------------------
// Pre     :
// Post    :
// Globals :
// I/O     :
// Task    : Create a cabinet extractor object
//-------------------------------------------------------------------------
CCabinetExtractor::CCabinetExtractor()
{
    // Allocate a decompression context
    m_hContext =FDICreate(MemAllocProc(),
                          MemFreeProc(),
                          FileOpenProc(),
                          FileReadProc(),
                          FileWriteProc(),
                          FileCloseProc(),
                          FileSeekProc(),
                          cpu80386,
                          &m_CabinetError);

    // Check if context allocated OK and update our internal status
    m_bStatus =(m_hContext != NULL);

    // Set the default extraction path in2 a temp folder
    m_DefaultExtractPath.TempDirectory();
}
#endif

#ifdef __CAB_EXTRACT__
//-------------------------------------------------------------------------
// Pre     : 
// Post    :
// Globals :
// I/O     :
// Task    : Open a file 4 decompression interface
//-------------------------------------------------------------------------
int FAR DIAMONDAPI CCabinetExtractor::FileOpen(char FAR *pszFile, int oFlag, int pMode)
{
    return CCabinetBase::FileOpen(pszFile,oFlag,pMode,&errno,NULL);
}
#endif

#ifdef __CAB_EXTRACT__
//-------------------------------------------------------------------------
// Pre     : 
// Post    :
// Globals :
// I/O     :
// Task    : Read cbRead bytes from file specified by handle into buffer ptrBuffer at
//           decompression interface request
//-------------------------------------------------------------------------
UINT FAR DIAMONDAPI CCabinetExtractor::FileRead(int hFile, void FAR *ptrBuffer, UINT cbRead)
{
    return CCabinetBase::FileRead(hFile,ptrBuffer,cbRead,&errno,NULL);
}
#endif

#ifdef __CAB_EXTRACT__
//-------------------------------------------------------------------------
// Pre     : 
// Post    :
// Globals :
// I/O     :
// Task    : Write cbWrite bytes from buffer ptrBuffer int file specified by handle at
//           decompression interface request
//-------------------------------------------------------------------------
UINT FAR DIAMONDAPI CCabinetExtractor::FileWrite(int hFile, void FAR *ptrBuffer, UINT cbWrite)
{
    return CCabinetBase::FileWrite(hFile,ptrBuffer,cbWrite,&errno,NULL);
}
#endif

#ifdef __CAB_EXTRACT__
//-------------------------------------------------------------------------
// Pre     : 
// Post    :
// Globals :
// I/O     :
// Task    : Compression interface wants 2 seek the file pointer
//-------------------------------------------------------------------------
long FAR DIAMONDAPI CCabinetExtractor::FileSeek(int hFile, long offset, int seekType)
{
    return CCabinetBase::FileSeek(hFile,offset,seekType,&errno,NULL);
}
#endif

#ifdef __CAB_EXTRACT__
//-------------------------------------------------------------------------
// Pre     : 
// Post    :
// Globals :
// I/O     :
// Task    : Decompression interface wants 2 close this file
//-------------------------------------------------------------------------
int FAR DIAMONDAPI CCabinetExtractor::FileClose(int hFile)
{
    return CCabinetBase::FileClose(hFile,&errno,NULL);
}
#endif

#ifdef __CAB_EXTRACT__
//-------------------------------------------------------------------------
// Pre     : If lpInfo is NULL, then no information is returned on the cabinet
//           only checking 2 see if it's a valid cabinet file
// Post    : Return TRUE if info extracted OK, FALSE if not a cabinet file or 
//           there was some error
// Globals :
// I/O     :
// Task    : Return info about a cabinet file
//-------------------------------------------------------------------------
BOOL CCabinetExtractor::GetCabinetInfo(LPCTSTR lpcszCabFile, CABINETINFO *lpInfo)
{
    // Check our internal status
    if(!m_bStatus)
        return FALSE;

    // Open file because the decompression interface 
    // wants an open file 2 get info about
    TCHAR szCabFile[MAX_PATH];
    STRCPY(szCabFile,lpcszCabFile);

    int hFile =FileOpen(szCabFile,_O_RDONLY | _O_SEQUENTIAL,0);
    if(hFile == -1)
        // Could not open file
        return FALSE;

    // Get info...
    FDICABINETINFO info;
    BOOL is_cabinet =FDIIsCabinet(m_hContext,hFile,&info);
    if(is_cabinet && lpInfo)
    {
        // Store info about the cabinet
        lpInfo->m_cbSize             =info.cbCabinet;
        lpInfo->m_nFolders           =info.cFolders;
        lpInfo->m_nFiles             =info.cFiles;
        lpInfo->m_nID                =info.setID;
        lpInfo->m_nIndex             =info.iCabinet;
        lpInfo->m_bHasReservedSpace  =info.fReserve;
        lpInfo->m_bChainedPrev       =info.hasprev;
        lpInfo->m_bChainedNext       =info.hasnext;
    }

    // Close the file we've got info from
    FileClose(hFile);

    return is_cabinet;
}
#endif

#ifdef __CAB_EXTRACT__
//-------------------------------------------------------------------------
// Pre     : 
// Post    : 
// Globals :
// I/O     :
// Task    : Store a default extraction folder
//-------------------------------------------------------------------------
void CCabinetExtractor::SetDefaultExtractPath(LPCTSTR lpcszDefExtractPath)
{
    if(lpcszDefExtractPath)
    {
        string str(lpcszDefExtractPath);
        EnsureTrailingBackslash(str);

        m_DefaultExtractPath =str.c_str();
    }
}
#endif

#ifdef __CAB_EXTRACT__
//-------------------------------------------------------------------------
// Pre     : lpcszCabFile is the full path 2 the first .cab file in set
//           bContinuous specifies whether 2 attempt processing possible following
//           cabinets in set (if any), or only extract files from the specified .cab file
// Post    : Return TRUE if file(s) extracted OK, FALSE if not a cabinet file or 
//           there was some error
// Globals :
// I/O     :
// Task    : Extract one or more files from a cabinet. Clients must implement 
//           notification members, so that they can supply the extractor object
//           with info on each file 2 extract (e.g. if extract, where 2 extract etc.)
//-------------------------------------------------------------------------
BOOL CCabinetExtractor::ExtractFiles(LPCTSTR lpcszCabFile, BOOL bContinuous)
{
    // Check our internal status
    if(!m_bStatus)
        return FALSE;

    // Make sure we've got a valid cabinet file
    CABINETINFO cabinfo;
	if(!GetCabinetInfo(lpcszCabFile,&cabinfo))
		return FALSE;

    // Store the path and name of cabinet being extracted
    m_bNextCabinetReady =TRUE;
    m_CabinetPath       =lpcszCabFile;
    m_CabinetPath.GetNameExtension(m_strCabinetName);
    m_CabinetPath.SetNameExtension(_T(""));

    BOOL bResult =TRUE;

    while(bResult)
    {
        // Start extraction...
        // This will extract all files in this .cab file, including the
        // ones which are continued in successive cabinets
        TCHAR sz_cabinet_path[MAX_PATH];
        TCHAR sz_cabinet_name[MAX_PATH];
        STRCPY(sz_cabinet_path,(LPCTSTR)m_CabinetPath);
        STRCPY(sz_cabinet_name,m_strCabinetName.c_str());

        // Make sure the required cabinet is available
        // Only do this 4 second or subsequent passes
		string strNextCabinetPath((LPCTSTR)m_CabinetPath);
        m_CabinetError.erfOper =FDIERROR_NONE;
        while(!m_bNextCabinetReady)
        {
			// Got a cabinet, see if cabinet is already available at the expected location
			// and if not, ask our client 2 provide us with the path 2 the next cabinet
            CPath CabinetValidator;
            CabinetValidator.SetDriveDirectory(strNextCabinetPath.c_str());
            CabinetValidator.SetNameExtension(m_strCabinetName.c_str());
			if(CabinetValidator.Exists())
			{
				// Check if file is a real cabinet
				if(GetCabinetInfo(CabinetValidator,&cabinfo))
				{
					// Check if the cabinet we got is the one we want
					if(cabinfo.m_nIndex == m_nContinueIndex)
					{

#pragma message(Reminder(_T("Also check set ID")))

						// Yeah, we have the cabinet we need
						m_bNextCabinetReady =TRUE;
					}
					else
					{
						// Cabinet is the wrong one, again
						m_CabinetError.erfOper =FDIERROR_WRONG_CABINET;
					}
				}
				else
				{
					// File is not a cabinet, again
					m_CabinetError.erfOper =FDIERROR_NOT_A_CABINET;
				}
			}
			else
			{
				// No such file, again
				m_CabinetError.erfOper =FDIERROR_CABINET_NOT_FOUND;
			}

			if(m_bNextCabinetReady)
			{
				// Yeah, if we got here, then we have the cabinet we need
				m_CabinetError.erfOper =FDIERROR_NONE;

				m_CabinetPath.SetDriveDirectory(strNextCabinetPath.c_str());
				STRCPY(sz_cabinet_path,(LPCTSTR)m_CabinetPath);
			}
			else
			{
				// We'll call NotifyNextCabinet until we get it right
				// or client aborts
				if(!NotifyNextCabinet(m_strCabinetName.c_str(),m_strContinueDisk.c_str(),strNextCabinetPath,m_CabinetError.erfOper))
				{
					// Aborted
					m_CabinetError.erfOper =FDIERROR_USER_ABORT;
					m_CabinetError.fError  =TRUE;
					return FALSE;
				}
			}
        }

        // Make sure that cabinet is available at the expected location
        // find out the index of the cabinet which is first 2 be extracted from
        m_strNextDisk.erase();
        m_NextCabinet =m_CabinetPath;
        m_NextCabinet.SetNameExtension(m_strCabinetName.c_str());
		ASSERTX(m_NextCabinet.Exists());
        ASSERTX(GetCabinetInfo(m_NextCabinet,&cabinfo));
        m_nNextIndex     =cabinfo.m_nIndex;
        m_nContinueIndex =-1;

        // Some internal initializations
        m_nLastFileStartCabinetIndex =-1;
        m_nLastFileEndCabinetIndex   =-1;
        m_bLastFileSkipped           =FALSE;
        m_bCabinetSetFinished        =FALSE;
        m_bNextCabinetReady          =FALSE;

        // Extract files in this cabinet and any file that starts 
        // in this cabinet but spans across cabinet boundaries
        // This might result in more than one cabinet file 2 be processed
        bResult =FDICopy(m_hContext,
                         sz_cabinet_name,
                         sz_cabinet_path,
                         0,
                         NotifyProgressProc(),
                         NULL,
                         this);

        if(!bContinuous)
            // Extraction ends after one cabinet (and all cabinets 
            // linked 2 this one by files spanning across cabinet boundaries)
            break;

        if(bResult)
        {
            // Check if we already processed all cabinets in this set
            // Also check that there was no file extracted from the last cabinet
            // Otherwise, we might get here when we just finished extracting the last 
            // chunk of a file spanning across cabinet boundaries, having its
            // last chunk in the last cabinet, but the last cabinet also contains other files
            // and these won't get extracted if we stop now
            if(m_bCabinetSetFinished && (m_nLastFileStartCabinetIndex == m_nLastFileEndCabinetIndex))
                // Yeah, we processed them all
                break;

            // Advancing 2 the next one
            // Here we should figure out which is the cabinet we should continue with
            if(m_bLastFileSkipped)
            {
                // The last file in this cabinet was skipped
                // We should continue with the next cabinet
                ASSERTX(!m_bCabinetSetFinished);
                m_ContinueCabinet =m_NextCabinet;
                m_strContinueDisk =m_strNextDisk;
                m_nContinueIndex  =m_nNextIndex;
            }

            // Check if any file extracted from this cabinet
            if((m_nLastFileStartCabinetIndex == -1) && (m_nLastFileEndCabinetIndex == -1))
            {
                // No files extracted, probably because this cabinet only 
                // contains a chunk of a file spanning multiple cabinets
                // which is being skipped
                m_ContinueCabinet =m_NextCabinet;
                m_strContinueDisk =m_strNextDisk;
                m_nContinueIndex  =m_nNextIndex;
            }

            m_CabinetPath =m_ContinueCabinet;
            m_CabinetPath.GetNameExtension(m_strCabinetName);
            m_CabinetPath.SetNameExtension(_T(""));

		    m_bNextCabinetReady =FALSE;
        }
    }

    return bResult;
}
#endif

#ifdef __CAB_EXTRACT__
//---------------------------------------------------------------------------
// Pre     : nID is the ID of cabinet set
//           lpcszCabPath is cabinet path
//           lpcszCabName is next cabinet's name (might be empty)
//           lpcszDiskName is next disk's name   (might be empty)
//           nCabIndex is 0-based index of cabinet being processed
// Post    : Return FALSE 2 abort
// Globals : 
// I/O     : 
// Task    : Simple notification that the decompression interface wants info 
//           about the next cabinet
//           If overridden by clients, they should also call this
//---------------------------------------------------------------------------
BOOL CCabinetExtractor::NotifyCabinetInfo(USHORT nID, LPCTSTR lpcszCabPath, LPCTSTR lpcszCabName, LPCTSTR lpcszDiskName, USHORT nCabIndex)
{
#ifdef __TRACE_CAB_EXTRACTION__
    TRACEFN(_T("CCabinetExtractor::NotifyCabinetInfo(%u,%s,%s,%s,%u)\n"),nID,lpcszCabPath,lpcszCabName,lpcszDiskName,nCabIndex);
#endif

    // Some additional check, this should be an empty
    // string if this is the last cabinet in the set
    if(!STRLEN(lpcszDiskName))
        m_bCabinetSetFinished =TRUE;

    m_ContinueCabinet =m_NextCabinet;
    m_strContinueDisk =m_strNextDisk;
    m_nContinueIndex  =m_nNextIndex;

    if(!m_bCabinetSetFinished)
    {
        m_nNextIndex++;
	    m_NextCabinet.SetDriveDirectory(lpcszCabPath);
        m_NextCabinet.SetNameExtension(lpcszCabName);
        m_strNextDisk.erase();
        m_strNextDisk =lpcszDiskName;
    }

    return TRUE;
}
#endif

#ifdef __CAB_EXTRACT__
//---------------------------------------------------------------------------
// Pre     : lpcszFileName is name of file continued from a previous cabinet
//           lpcszFirstCab is name of cabinet where the file starts
//           lpcszFirstDisk is name of disk where the file starts
// Post    : Return FALSE 2 abort
// Globals : 
// I/O     : 
// Task    : Simple notification that first file in the cabinet is continuation
//           from some previous cabinet
//---------------------------------------------------------------------------
BOOL CCabinetExtractor::NotifyPartialFile(LPCTSTR lpcszFileName, LPCTSTR lpcszFirstCab, LPCTSTR lpcszFirstDisk)
{
#ifdef __TRACE_CAB_EXTRACTION__
    TRACEFN(_T("CCabinetExtractor::NotifyPartialFile(%s,%s,%s)\n"),lpcszFileName,lpcszFirstCab,lpcszFirstDisk);
#endif

    return TRUE;
}
#endif

#ifdef __CAB_EXTRACT__
//---------------------------------------------------------------------------
// Pre     : lpcszFileName is name of file being extracted
//           cbSize is real (uncompressed) size of file
// Post    : Return FALSE 2 abort
// Globals : 
// I/O     : 
// Task    : Simple notification that a file is about 2 be extracted from cabinet
//           we can specify where 2 extract it by filling strExtractTo with an 
//           extract path. Before returning, make sure that the path you specified exists
//           If you want 2 skip this file, and do not extract it, set bSkipFile 2 TRUE
//---------------------------------------------------------------------------
BOOL CCabinetExtractor::NotifyFileCopy(LPCTSTR lpcszFileName, ULONG cbSize, string &strExtractTo, BOOL &bSkipFile)
{
#ifdef __TRACE_CAB_EXTRACTION__
    TRACEFN(_T("CCabinetExtractor::NotifyFileCopy(%s,%lu,%s,%s)\n"),lpcszFileName,cbSize,strExtractTo.c_str(),bSkipFile ? _T("TRUE") : _T("FALSE"));
#endif

    // Extracting files 2 default extraction path and
    // using the same name as stored in the cabinet file
    m_DefaultExtractPath.GetDriveDirectory(strExtractTo);
    EnsureTrailingBackslash(strExtractTo);
    strExtractTo +=lpcszFileName;

    return TRUE;
}
#endif

#ifdef __CAB_EXTRACT__
//---------------------------------------------------------------------------
// Pre     : lpcszFileName is the name of the extracted file
// Post    : Return FALSE 2 abort
// Globals : 
// I/O     : 
// Task    : Simple notification that a file was extracted OK, now it's time 2 
//           set it's attributes and date/time. Clients can specify the date/time 
//           and attributes they want the extracted file 2 have
//           Note that at the time when this notification is called, the file is
//           still open. If you want 2 manipulate the file after it's extracted, 
//           you can do so from NotifyFileCopiedAndClosed()
//---------------------------------------------------------------------------
BOOL CCabinetExtractor::NotifyFileCopied(LPCTSTR lpcszFileName, USHORT &nDate, USHORT &nTime, USHORT &nAttribs)
{
#ifdef __TRACE_CAB_EXTRACTION__
    TRACEFN(_T("CCabinetExtractor::NotifyFileCopied(%s,%u,%u,%u)\n"),lpcszFileName,nDate,nTime,nAttribs);
#endif

    return TRUE;
}
#endif

#ifdef __CAB_EXTRACT__
//---------------------------------------------------------------------------
// Pre     : lpcszFileName is the name of the extracted file
// Post    : Return FALSE 2 abort
// Globals : 
// I/O     : 
// Task    : Simple notification that a file was extracted OK
//           At this time the decompression interface got it's hands off the 
//           extracted file, clients can do whatever they want with it
//           Here clients can also use the members:
//                  m_nLastFileStartCabinetIndex
//                  m_nLastFileEndCabinetIndex
//---------------------------------------------------------------------------
BOOL CCabinetExtractor::NotifyFileCopiedAndClosed(LPCTSTR lpcszFileName)
{
#ifdef __TRACE_CAB_EXTRACTION__
    TRACEFN(_T("CCabinetExtractor::NotifyFileCopiedAndClosed(%s)\n"),lpcszFileName);
#endif

    return TRUE;
}
#endif

#ifdef __CAB_EXTRACT__
//---------------------------------------------------------------------------
// Pre     : 
// Post    : Return FALSE 2 abort
// Globals : 
// I/O     : 
// Task    : Simple notification of file enumeration
//---------------------------------------------------------------------------
BOOL CCabinetExtractor::NotifyEnumerate(USHORT nID, long &nCurrentPosition, USHORT &nFilesRemaining)
{
#ifdef __TRACE_CAB_EXTRACTION__
    TRACEFN(_T("CCabinetExtractor::NotifyEnumerate(%u,%ld,%u)\n"),nID,nCurrentPosition,nFilesRemaining);
#endif

    return TRUE;
}
#endif

#ifdef __CAB_EXTRACT__
//---------------------------------------------------------------------------
// Pre     : lpcszNextCab is the name of the next cabinet
//           lpcszNextDisk is the name of the next disk
//           nErrorCode is the last error code (The decompression interface will keep calling this,
//           until the correct cabinet is returned or the extraction is aborted)
// Post    : Return FALSE 2 abort
// Globals : 
// I/O     : 
// Task    : Simple notification that another cabinet is needed
//           Clients should here locate the next cabinet (eventually asking 
//           users 4 the next disk or allowing browse 4 the next cabinet's location) 
//           and return it 2 the decompression engine
//           Before returning a path, make sure that there is a cabinet with the proper name
//           (eventually also checking that is from the correct set) and is readable
//---------------------------------------------------------------------------
BOOL CCabinetExtractor::NotifyNextCabinet(LPCTSTR lpcszNextCab, LPCTSTR lpcszNextDisk, string &strNextCabPath, int nErrorCode)
{
#ifdef __TRACE_CAB_EXTRACTION__
    TRACEFN(_T("CCabinetExtractor::NotifyNextCabinet(%s,%s,%s,%d)\n"),lpcszNextCab,lpcszNextDisk,strNextCabPath.c_str(),nErrorCode);
#endif

    // Assuming that next cabinet file is in the same folder
    return TRUE;
}
#endif

#ifdef __CAB_EXTRACT__
//-------------------------------------------------------------------------
// Pre     : The wild pointer notifyInfo->pv field points 2 a CCabinetExtractor
//           If notifyType == fdintCABINET_INFO:
//              Called exactly once for each cabinet opened by FDICopy(), including
//              continuation cabinets opened due to file(s) spanning cabinet
//              boundaries. Primarily intended to permit 2 automatically select 
//              the next cabinet in a cabinet sequence even if not copying files 
//              that span cabinet boundaries
//                  notifyInfo->psz1     is name of next cabinet (no path)
//                  notifyInfo->psz2     is name of next disk
//                  notifyInfo->psz3     is cabinet path name
//                  notifyInfo->setID    is cabinet set ID
//                  notifyInfo->iCabinet is Cabinet number within cabinet set of the cabinet being processed now (0-based)
//              This call is made *every* time a new cabinet is examined by
//              FDICopy(). So if "foo2.cab" is examined because a file is
//              continued from "foo1.cab", and then you call FDICopy() again
//              on "foo2.cab", you will get *two* fdintCABINET_INFO calls
//
//           If notifyType == fdintCOPY_FILE:
//              Called for each file that *starts* in the current cabinet, giving
//              the client the opportunity to request that the file be copied or skipped
//                  notifyInfo->psz1    is file name in cabinet
//                  notifyInfo->cb      is uncompressed size of file
//                  notifyInfo->date    is file date
//                  notifyInfo->time    is file time
//                  notifyInfo->attribs is file attributes
//                  notifyInfo->iFolder is file's folder index
//
//           If notifyType == fdintCLOSE_FILE_INFO:
//              Called after all of the data has been written to a target file.
//              This function must close the file and set the file date, time and attributes
//                  notifyInfo->psz1    is file name in cabinet
//                  notifyInfo->hf      is file handle
//                  notifyInfo->date    is file date
//                  notifyInfo->time    is file time
//                  notifyInfo->attribs is file attributes
//                  notifyInfo->iFolder is file's folder index
//                  notifyInfo->cb      is Run After Extract flag (0 don't run, 1 run)
//              FDI assumes that the target file was closed, even if this
//              callback returns failure.  FDI will NOT attempt to use
//              the PFNCLOSE function supplied on FDICreate() to close the file
//
//           If notifyType == fdintPARTIAL_FILE:
//              Called for files at the front of the cabinet that are CONTINUED
//              from a previous cabinet.  This callback occurs only when FDICopy is
//              started on second or subsequent cabinet in a series that has files
//              continued from a previous cabinet
//                  notifyInfo->psz1 is file name of file CONTINUED from a PREVIOUS cabinet
//                  notifyInfo->psz2 is name of cabinet where file starts
//                  notifyInfo->psz3 is name of disk where file starts
//
//           If notifyType == fdintENUMERATE:
//              Called once after a call to FDICopy() starts scanning a CAB's
//              CFFILE entries, and again when there are no more CFFILE entries.
//              If CAB spanning occurs, an additional call will occur after the
//              first spanned file is completed.  If the notifyInfo->iFolder value is
//              changed from zero, additional calls will occur next time it reaches
//              zero.  If iFolder is changed to zero, FDICopy will terminate, as if
//              there were no more CFFILE entries.  Primarily intended to allow an
//              application with it's own file list to help FDI advance quickly to
//              a CFFILE entry of interest.  Can also be used to allow an
//              application to determine the cb values for each file in the CAB
//                  notifyInfo->cb        is current CFFILE position
//                  notifyInfo->iFolder   is number of files remaining
//                  notifyInfo->setID     is current CAB's setID value
//              This call can be ignored by applications which want normal file
//              searching.  The application can adjust the supplied values to
//              force FDICopy() to continue it's search at another location, or
//              to force FDICopy() to terminate the search, by setting iFolder to 0.
//              (FDICopy() will report no error when terminated this way)
//              FDI has no means to verify the supplied cb or iFolder values
//              Arbitrary values are likely to cause undesirable results. An
//              application should cross-check notifyInfo->setID to be certain the
//              external database is in sync with the CAB.  Reverse-skips are OK
//              (but may be inefficient) unless fdintNEXT_CABINET has been called
//
//           If notifyType == fdintNEXT_CABINET:
//              This function is *only* called when fdintCOPY_FILE was told to copy
//              a file in the current cabinet that is continued to a subsequent
//              cabinet file. It is important that the cabinet path name (psz3)
//              be validated before returning! This function should ensure that
//              the cabinet exists and is readable before returning.  So, this
//              is the function that should, for example, issue a disk change
//              prompt and make sure the cabinet file exists
//              When this function returns to FDI, FDI will check that the setID
//              and iCabinet match the expected values for the next cabinet.
//              If not, FDI will continue to call this function until the correct
//              cabinet file is specified, or until this function returns -1 to
//              abort the FDICopy() function. notifyInfo->fdie is set to
//              FDIERROR_WRONG_CABINET to indicate this case
//                  notifyInfo->psz1 is name of next cabinet where current file is continued
//                  notifyInfo->psz2 is name of next disk where current file is continued
//                  notifyInfo->psz3 is cabinet path name; FDI concatenates psz3 with psz1
//                                      to produce the fully-qualified path for the cabinet
//                                      file. The 256-byte buffer pointed at by psz3 may
//                                      be modified, but psz1 may not!
//                  notifyInfo->fdie is FDIERROR_WRONG_CABINET if the previous call to
//                                      fdintNEXT_CABINET specified a cabinet file that
//                                      did not match the setID/iCabinet that was expected
//              If you *haven't* ensured that the cabinet file is present and
//              readable, or the cabinet file has been damaged, notifyInfo->fdie will
//              receive other appropriate error codes
//                  FDIERROR_CABINET_NOT_FOUND
//                  FDIERROR_NOT_A_CABINET
//                  FDIERROR_UNKNOWN_CABINET_VERSION
//                  FDIERROR_CORRUPT_CABINET
//                  FDIERROR_BAD_COMPR_TYPE
//                  FDIERROR_RESERVE_MISMATCH
//                  FDIERROR_WRONG_CABINET
//              This call is almost always made when a target file is open and
//              being written to, and the next cabinet is needed to get more
//              data for the file
// Post    : If notifyType == fdintCABINET_INFO:
//           If notifyType == fdintPARTIAL_FILE:
//           If notifyType == fdintNEXT_CABINET:
//              Return -1 to abort file copying
//
//           If notifyType == fdintCLOSE_FILE_INFO:
//              Return 0 or -1 to abort copying
//
//           If notifyType == fdintCOPY_FILE:
//              Return non-zero file handle for destination file; FDI writes
//              data to this file use the PFNWRITE function supplied to FDICreate,
//              and then calls fdintCLOSE_FILE_INFO to close the file and set
//              the date, time, and attributes.  NOTE: This file handle returned
//              must also be closeable by the PFNCLOSE function supplied to
//              FDICreate, since if an error occurs while writing to this handle,
//              FDI will use the PFNCLOSE function to close the file so that the
//              client may delete it
//              Return  0 to skip file, do not copy
//              Return -1 to abort file copying
//
//           If notifyType == fdintENUMERATE:
//              Return anything but -1 if we don't care
//              If we wish to force a skip, return anything but -1 and
//                  notifyInfo->cb        set to desired CFFILE position
//                  notifyInfo->iFolder   set desired # of files remaining
//              If we wish to stop enumeration, Return anything but -1 and
//                  notifyInfo->iFolder   set to 0
//              Return -1 to abort file copying ("user aborted")
// Globals :
// I/O     :
// Task    : This notification proc is called by the decompression interface 2
//           inform us what it is doing with our cabinet file(s)
//-------------------------------------------------------------------------
int FAR DIAMONDAPI CCabinetExtractor::NotifyProgress(FDINOTIFICATIONTYPE notifyType, PFDINOTIFICATION notifyInfo)
{
#ifdef __TRACE_CAB_EXTRACTION__
    TRACEFN(_T("CCabinetExtractor::NotifyProgress("));
#endif

    int  result =0;
    BOOL ok_2_continue;

    CCabinetExtractor *pce =(CCabinetExtractor *)notifyInfo->pv;
    ASSERTX(pce);

    // Handle notifications differently...
    switch(notifyType)
	{
		case fdintCABINET_INFO: 
            // Get general information about the cabinet
            #ifdef __TRACE_CAB_EXTRACTION__
            TRACEX(_T("CABINET_INFO,"));
            TRACEX(_T("%u,%s,%s,%s,%u)\n"),notifyInfo->setID,notifyInfo->psz3,notifyInfo->psz1,notifyInfo->psz2,notifyInfo->iCabinet);
            #endif
            {
                // Notifying client...
                ok_2_continue =pce->NotifyCabinetInfo(notifyInfo->setID,     // ID of cabinet set
                                                      notifyInfo->psz3,      // Cabinet path
                                                      notifyInfo->psz1,      // Cabinet name (might be empty)
                                                      notifyInfo->psz2,      // Disk name    (might be empty)
                                                      notifyInfo->iCabinet); // Index of cabinet being processed (the one used in ExtractFiles) (0-based)
                result =ok_2_continue ? 0 : -1;
            }
			break;

		case fdintCOPY_FILE:
            // File to be copied
            #ifdef __TRACE_CAB_EXTRACTION__
            TRACEX(_T("COPY_FILE,"));
            TRACEX(_T("%s,%lu,0x%04x,0x%04x,0x%04x)...\n"),notifyInfo->psz1,notifyInfo->cb,notifyInfo->date,notifyInfo->time,notifyInfo->attribs);
            #endif
            {
                pce->m_bLastFileSkipped =FALSE;

                // Notifying client...
                string strExtractTo;
                BOOL   bSkipFile =FALSE;
                ok_2_continue =pce->NotifyFileCopy(notifyInfo->psz1, // File name
                                                   notifyInfo->cb,   // File size uncompressed
                                                   strExtractTo,     // Extract name (client fills this in)
                                                   bSkipFile);       // Whether 2 extract or skip (client fills this in)
                result =ok_2_continue ? (bSkipFile ? 0 : 1) : -1;

                // Check if client wants 2 copy this file somewhere or 
                // requested abort copying
                if(ok_2_continue)
                {
                    // Check if client returned a path where 2 extract this file
                    // If yes, we'll extract there otherwise we will skip this file
                    if(!bSkipFile && !strExtractTo.empty())
                    {
                        // Create target file
                        TCHAR temp_path[MAX_PATH];
                        STRCPY(temp_path,strExtractTo.c_str());

                        #ifdef __TRACE_CAB_EXTRACTION__
                        TRACEX(_T("Extracting as %s\n"),strExtractTo.c_str());
                        #endif

                        SetFileAttributes(temp_path,FILE_ATTRIBUTE_NORMAL);
                        result =FileOpen(temp_path,_O_BINARY | _O_CREAT | _O_WRONLY | _O_SEQUENTIAL,_S_IREAD | _S_IWRITE);
                        pce->m_LastExtracted =strExtractTo.c_str();

                        // Remeber the cabinet index where this file starts
                        // We always remember the start/end cabinet index of the last
                        // file being extracted, so that we can determine if the
                        // file spans across cabinet boundaries
                        pce->m_nLastFileStartCabinetIndex =pce->m_nNextIndex;
                        if(!pce->m_bCabinetSetFinished)
                            (pce->m_nLastFileStartCabinetIndex)--;
                    }
                    else
                    {
                        // Force skipping this file because no extract path 
                        // was specified
                        result =0;
                        pce->m_bLastFileSkipped =TRUE;

                        #ifdef __TRACE_CAB_EXTRACTION__
                        TRACEX(_T("Skipped\n"));
                        #endif
                    }
                }
                else
                {
                    #ifdef __TRACE_CAB_EXTRACTION__
                    TRACEX(_T("Aborted\n"));
                    #endif
                }
            }
            break;

		case fdintCLOSE_FILE_INFO:
            // Close the file, set relevant info
            #ifdef __TRACE_CAB_EXTRACTION__
            TRACEX(_T("CLOSE_FILE_INFO,"));
            TRACEX(_T("%s,0x%04x,0x%04x,0x%04x)\n"),notifyInfo->psz1,notifyInfo->date,notifyInfo->time,notifyInfo->attribs);
            #endif
            {
                // Notifying client...
                ok_2_continue =pce->NotifyFileCopied(notifyInfo->psz1,     // File name
                                                     notifyInfo->date,     // File date
                                                     notifyInfo->time,     // File time
                                                     notifyInfo->attribs); // File attributes
                result =ok_2_continue ? 1 : -1;

                // Check if client aborted copying
                if(ok_2_continue)
                {
                    // Close file
                    FileClose(notifyInfo->hf);

                    // Set file date/time
                    string strFullExractName =(LPCTSTR)pce->m_LastExtracted;
                    HANDLE handle =CreateFile(strFullExractName.c_str(),
                                              GENERIC_READ | GENERIC_WRITE,
                                              FILE_SHARE_READ,
                                              NULL,
                                              OPEN_EXISTING,
                                              FILE_ATTRIBUTE_NORMAL,
                                              NULL);
                    if(handle != INVALID_HANDLE_VALUE)
                    {
                        FILETIME datetime;
                        FILETIME datetime_local;

                        DosDateTimeToFileTime(notifyInfo->date,notifyInfo->time,&datetime);
                        LocalFileTimeToFileTime(&datetime,&datetime_local);
                        SetFileTime(handle,&datetime_local,NULL,&datetime_local);

                        CloseHandle(handle);
                    }

                    // Set file attributes
                    // Mask out attribute bits other than read-only,
                    // hidden, system, and archive, since the other
                    // attribute bits are reserved for use by the cabinet format
                    DWORD attributes =notifyInfo->attribs & (_A_RDONLY | _A_HIDDEN | _A_SYSTEM | _A_ARCH);
                    SetFileAttributes(strFullExractName.c_str(),attributes);

                    // Remeber the cabinet index where this file ends
                    // We always remember the start/end cabinet index of the last
                    // file being extracted, so that we can determine if the
                    // file spans across cabinet boundaries
                    pce->m_nLastFileEndCabinetIndex =pce->m_nNextIndex;
                    if(!pce->m_bCabinetSetFinished)
                        (pce->m_nLastFileEndCabinetIndex)--;

                    // Now also notify clients that file is ready 4 manipulation
                    ok_2_continue =pce->NotifyFileCopiedAndClosed(notifyInfo->psz1); // File name
                    if(!ok_2_continue)
                        result =-1;
                }
            }
            break;

        case fdintPARTIAL_FILE: 
            // First file in cabinet is continuation
            #ifdef __TRACE_CAB_EXTRACTION__
            TRACEX(_T("PARTIAL_FILE,"));
            TRACEX(_T("%s,%s,%s)\n"),notifyInfo->psz1,notifyInfo->psz2,notifyInfo->psz3);
            #endif
            {
                // Notifying client...
                ok_2_continue =pce->NotifyPartialFile(notifyInfo->psz1,  // File name
                                                      notifyInfo->psz2,  // Name of cabinet where file starts
                                                      notifyInfo->psz3); // Name of disk where file starts
                result =ok_2_continue ? 0 : -1;
            }
			break;

		case fdintNEXT_CABINET:	
            // File continued to next cabinet
            #ifdef __TRACE_CAB_EXTRACTION__
            TRACEX(_T("NEXT_CABINET,"));
            TRACEX(_T("%s,%s,%s,%d)\n"),notifyInfo->psz1,notifyInfo->psz2,notifyInfo->psz3,notifyInfo->fdie);
            #endif
            {
                // Notifying client...
                string strPathToNextCabinet(notifyInfo->psz3);
                ok_2_continue =pce->NotifyNextCabinet(notifyInfo->psz1,     // Name of next cabinet
                                                      notifyInfo->psz2,     // Name of next disk
                                                      strPathToNextCabinet, // Path 2 next cabinet (client fills this in)
                                                      notifyInfo->fdie);    // Last error code
                STRNCPY(notifyInfo->psz3,strPathToNextCabinet.c_str(),MAX_PATH);
                result =ok_2_continue ? 0 : -1;
            }
            break;

        case fdintENUMERATE:
            // Enumerating files
            #ifdef __TRACE_CAB_EXTRACTION__
            TRACEX(_T("ENUMERATE,"));
            TRACEX(_T("%u,%lu,%u)\n"),notifyInfo->setID,notifyInfo->cb,notifyInfo->iFolder);
            #endif
            {
                ok_2_continue =pce->NotifyEnumerate(notifyInfo->setID,    // Cabinet set ID
                                                    notifyInfo->cb,       // Current CFFILE position
                                                    notifyInfo->iFolder); // Number of files remaining
                result =ok_2_continue ? 0 : -1;
            }
            break;
	}

	return result;
}
#endif

#ifdef __CAB_EXTRACT__
//-------------------------------------------------------------------------
// Pre     :
// Post    :
// Globals :
// I/O     :
// Task    : Delete and cleanup a cabinet extractor object
//-------------------------------------------------------------------------
CCabinetExtractor::~CCabinetExtractor()
{
#ifdef __TRACE_CAB_EXTRACTION__
    TRACEFN(_T("CCabinetExtractor::~CCabinetExtractor\n"));
#endif

    // Free the decompression context (if any)
    if(m_bStatus && (m_hContext != NULL))
        FDIDestroy(m_hContext);
}
#endif

