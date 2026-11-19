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
//     $Date: 5/13/98 12:02a $
//  $Modtime: 4/27/98 6:50a $
// $Revision: 51 $
//  $Archive: /Interscope/Thebe/InstallMaster/Cabinet.hpp $
// $Workfile: Cabinet.hpp $
//-----------------------------------------------------------------------

#ifndef __Cabinet_Hpp__
#define __Cabinet_Hpp__

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

// If you want 2 trace the progress of compression/decompression, define the
// symbols below
// #define __TRACE_CAB_MEMORY__
// #define __TRACE_CAB_COMPRESSION__
// #define __TRACE_CAB_EXTRACTION__

// Define one of the following symbols 2 include only the code you need
// in your project
#define __CAB_BUILD__
// #define __CAB_EXTRACT__


#if !defined(__CAB_BUILD__) && !defined(__CAB_EXTRACT__)
#error You did not specified what cabinet functionality to include
#endif

#include <String>

#include "Portable.H"
#include "StringHelper.Hpp"
#include "Path.Hpp"

#ifdef __CAB_BUILD__
#include "FCI.H"        // File Compression Interface
#endif

#ifdef __CAB_EXTRACT__
#include "FDI.H"        // File Decompression Interface
#endif


using namespace std;


//--- Cabinet handler base -----------------------------------------------

// These classes implement cabinet file (.CAB) file handlig. They require the
// code of the Microsoft Cabinet API and the headers and sources that implement 
// C wrappers for the API. These reside in the files CABINET.DLL (the API code) and
// in the files FCI.H, FDI.H, FCIDLL.C and FDIDLL.C. These files are part of the
// Microsoft Cabinet SDK which can be obtained from Microsft SiteBuilder website
// at http://www.microsoft.com/sbnmember/download/download.asp
//
// Warning: These classes are not thread safe. So, do not mess with the same instance
//          of either CCabinetBuilder or CCabinetExtractor from withing different threads
//          However, since each instance of these classes keeps its own compression/decompression
//          context, you can use multiple instances and you even can instantiate separate instances 
//          in separate threads

class CCabinetBase
{
// Data members
protected:
    ERF     m_CabinetError;
    BOOL    m_bStatus;          // TRUE when everything is OK

public:
    CPath   m_CabinetPath;
    string  m_strCabinetName;

// Implementation
public:
    CCabinetBase(): m_bStatus(FALSE) { ZeroMemory(&m_CabinetError,sizeof(m_CabinetError)); }
    BOOL GetStatus() const      { return m_bStatus && !m_CabinetError.fError; }
    int  GetErrorCode()   const { return m_CabinetError.erfOper; }
    int  GetErrorCodeEx() const { return m_CabinetError.erfType; }

// Callbacks
protected:
    static void HUGE * FAR DIAMONDAPI MemAlloc(ULONG cbSize);
    static void FAR DIAMONDAPI MemFree(void HUGE *ptrMemory);

    static int  FAR DIAMONDAPI FileOpen(char FAR *pszFile, int oFlag, int pMode, int FAR *ptrErr, void FAR *pv);
    static UINT FAR DIAMONDAPI FileRead (int hFile, void FAR *ptrBuffer, UINT cbRead,  int FAR *ptrErr, void FAR *pv);
    static UINT FAR DIAMONDAPI FileWrite(int hFile, void FAR *ptrBuffer, UINT cbWrite, int FAR *ptrErr, void FAR *pv);
    static long FAR DIAMONDAPI FileSeek (int hFile, long offset, int seekType, int FAR *ptrErr, void FAR *pv);
    static int  FAR DIAMONDAPI FileClose(int hFile, int FAR *ptrErr, void FAR *pv);
};


//--- Class 4 cabinet creation -------------------------------------------

#ifdef __CAB_BUILD__
// Warning: File paths are not stored in the .CAB file(s), so that you will
//          have 2 remember or know where 2 extract them later




class CCabinetBuilder: public CCabinetBase
{
// Data members
protected:
    HFCI    m_hContext;
    CCAB    m_CabinetInfo;
    int     m_nLastStartDisk;
    string  m_strLastCabinet;

public:
    string  m_strDiskName;

    ULONG   m_cbFirstCabSize;       // Size 4 the first .CAB file
    ULONG   m_cbCabSize;            // Size 4 the rest of the .CAB files
    ULONG   m_cbTreshold;           // Flush 2 cabinet after each block of this size
    USHORT  m_nID;                  // Unique ID
    UINT    m_cbReserveCFHeader;    // Space to reserve in CFHEADER
    UINT    m_cbReserveCFFolder;    // Space to reserve in CFFOLDER
    UINT    m_cbReserveCFData;      // Space to reserve in CFDATA

// Construction and destruction
public:
    CCabinetBuilder(USHORT nID, ULONG cbFirstCabSize, ULONG cbCabSize, ULONG cbTreshold);
    BOOL InitCabinet(LPCTSTR lpcszCabPath, LPCTSTR lpcszCabName, LPCTSTR lpcszDiskName, BOOL bCreatePath =TRUE);
    void ReserveHeader(UINT reserve)      { m_cbReserveCFHeader =reserve;   }
    void ReserveFolder(UINT reserve)      { m_cbReserveCFFolder =reserve;   }
    void ReserveData(UINT reserve)        { m_cbReserveCFData   =reserve;   }
    BOOL AddFile(LPTSTR lpszFileName, LPTSTR lpszStoreAs =NULL, BOOL bCompress =TRUE, BOOL bExecuteOnExtract =FALSE);
    BOOL FlushFolder();
    BOOL FlushCabinet();
    void WaitUntilDone() { FlushCabinet(); }
    virtual ~CCabinetBuilder();

// Callbacks
protected:
    static int  FAR DIAMONDAPI FileDelete(char FAR *pszFile, int FAR *ptrErr, void FAR *pv);
    static BOOL FAR DIAMONDAPI GetTempFile(char *pszTempName, int cbTempName, void FAR *pv);
    static int  FAR DIAMONDAPI GetFileInfo(char *pszName, USHORT *pdate, USHORT *ptime, USHORT *pattribs, int FAR *ptrErr, void FAR *pv);

    static BOOL FAR DIAMONDAPI GetNextCabinet(PCCAB pccab, ULONG cbPrevCab, void FAR *pv);
    static int  FAR DIAMONDAPI NotifyFilePlaced(PCCAB pccab, char *pszFile, long cbFile, BOOL bContinuation, void FAR *pv);
    static long FAR DIAMONDAPI NotifyProgress(UINT typeStatus, ULONG cb1, ULONG cb2, void FAR *pv);

// Callback helpers
protected:
    virtual PFNFCIALLOC          MemAllocProc()         const { return CCabinetBase::MemAlloc;            }
    virtual PFNFCIFREE           MemFreeProc()          const { return CCabinetBase::MemFree;             }
    virtual PFNFCIOPEN           FileOpenProc()         const { return CCabinetBase::FileOpen;            }
    virtual PFNFCIREAD           FileReadProc()         const { return CCabinetBase::FileRead;            }
    virtual PFNFCIWRITE          FileWriteProc()        const { return CCabinetBase::FileWrite;           }
    virtual PFNFCISEEK           FileSeekProc()         const { return CCabinetBase::FileSeek;            }
    virtual PFNFCICLOSE          FileCloseProc()        const { return CCabinetBase::FileClose;           }
    virtual PFNFCIDELETE         FileDeleteProc()       const { return CCabinetBuilder::FileDelete;       }
    virtual PFNFCIGETTEMPFILE    GetTempFileProc()      const { return CCabinetBuilder::GetTempFile;      }
    virtual PFNFCIGETOPENINFO    GetFileInfoProc()      const { return CCabinetBuilder::GetFileInfo;      }
    virtual PFNFCIGETNEXTCABINET GetNextCabinetProc()   const { return CCabinetBuilder::GetNextCabinet;   }
    virtual PFNFCISTATUS         NotifyProgressProc()   const { return CCabinetBuilder::NotifyProgress;   }
    virtual PFNFCIFILEPLACED     NotifyFilePlacedProc() const { return CCabinetBuilder::NotifyFilePlaced; }

// Miscellaneous helpers
public:
    virtual void ManufactureDiskName(PCCAB cabinfo);
    virtual void ManufactureCabinetName(PCCAB cabinfo);

protected:
    virtual BOOL  NotifyPlaced(char *pszFile, long cbFile, BOOL bContinuation, int nDisk, char *pszCabinet);
    virtual BOOL  NotifyCompress(ULONG cbOriginal, ULONG cbCompressed);
    virtual BOOL  NotifyFolderFlush(ULONG cbSize, ULONG cbWritten);
    virtual ULONG NotifyCabinetFlush(ULONG cbEstimatedSize, ULONG cbActualSize);
};

#endif


//--- Compression errors ---------------------------------------------------

// Clients can retrieve the error code of the last cabinet operation using the
// GetErrorCode and GetErrorCodeEx (additional error info) members
// Possible values returned by GetErrorCode are:
//      FCIERR_NONE             No error
//      FCIERR_OPEN_SRC         Failure opening file to be stored in cabinet
//                                  Use GetErrorCodeEx 2 get C run-time *errno* value
//      FCIERR_READ_SRC         Failure reading file to be stored in cabinet
//                                  Use GetErrorCodeEx 2 get C run-time *errno* value
//      FCIERR_ALLOC_FAIL       Out of memory
//      FCIERR_TEMP_FILE        Could not create a temporary file
//                                  Use GetErrorCodeEx 2 get C run-time *errno* value
//      FCIERR_BAD_COMPR_TYPE   Unknown compression type
//      FCIERR_CAB_FILE         Could not create cabinet file
//                                  Use GetErrorCodeEx 2 get C run-time *errno* value
//      FCIERR_USER_ABORT       Client requested abort
//      FCIERR_MCI_FAIL         Failure compressing data


//--- Structure 2 hold information about a cabinet file --------------------

#ifdef __CAB_EXTRACT__

struct CABINETINFO
{
    long        m_cbSize;            // Total length of cabinet file
    USHORT      m_nFolders;          // Count of folders in cabinet
    USHORT      m_nFiles;            // Count of files in cabinet
    USHORT      m_nID;               // Cabinet set ID
    USHORT      m_nIndex;            // Cabinet number in set (0 based)
    BOOL        m_bHasReservedSpace; // TRUE if there are reserved areas present in cabinet
    BOOL        m_bChainedPrev;      // TRUE if cabinet is chained prev
    BOOL        m_bChainedNext;      // TRUE if cabinet is chained next
};

#endif

#define MAX_CABINETS_IN_SET     50


//--- Class 4 cabinet extraction ------------------------------------------

#ifdef __CAB_EXTRACT__

class CCabinetExtractor: public CCabinetBase
{
// Data members
private:
    CPath   m_NextCabinet;
    string  m_strNextDisk;
    int     m_nNextIndex;

    CPath   m_ContinueCabinet;
    string  m_strContinueDisk;
    int     m_nContinueIndex;

    BOOL    m_bLastFileSkipped;
    BOOL    m_bNextCabinetReady;

protected:
    HFDI    m_hContext;
    int     m_nLastFileStartCabinetIndex;
    int     m_nLastFileEndCabinetIndex;
    BOOL    m_bCabinetSetFinished;

public:
    CPath   m_DefaultExtractPath;
    CPath   m_LastExtracted; // Last extracted file (real path, where file was extracted)

// Construction and destruction
public:
    CCabinetExtractor();
    BOOL GetCabinetInfo(LPCTSTR lpcszCabFile, CABINETINFO *lpInfo =NULL);
    void SetDefaultExtractPath(LPCTSTR lpcszDefExtractPath);
    BOOL ExtractFiles(LPCTSTR lpcszCabFile, BOOL bContinuous =TRUE);
    virtual ~CCabinetExtractor();

// Callbacks
protected:
    static int  FAR DIAMONDAPI FileOpen(char FAR *pszFile, int oFlag, int pMode);
    static UINT FAR DIAMONDAPI FileRead(int hFile, void FAR *ptrBuffer, UINT cbRead);
    static UINT FAR DIAMONDAPI FileWrite(int hFile, void FAR *ptrBuffer, UINT cbWrite);
    static int  FAR DIAMONDAPI FileClose(int hFile);
    static long FAR DIAMONDAPI FileSeek(int hFile, long offset, int seekType);

    static int  FAR DIAMONDAPI NotifyProgress(FDINOTIFICATIONTYPE notifyType, PFDINOTIFICATION notifyInfo);

// Callback helpers
protected:
    virtual PFNALLOC     MemAllocProc()       const { return CCabinetBase::MemAlloc;            }
    virtual PFNFREE      MemFreeProc()        const { return CCabinetBase::MemFree;             }
    virtual PFNOPEN      FileOpenProc()       const { return CCabinetExtractor::FileOpen;       }
    virtual PFNREAD      FileReadProc()       const { return CCabinetExtractor::FileRead;       }
    virtual PFNWRITE     FileWriteProc()      const { return CCabinetExtractor::FileWrite;      }
    virtual PFNSEEK      FileSeekProc()       const { return CCabinetExtractor::FileSeek;       }
    virtual PFNCLOSE     FileCloseProc()      const { return CCabinetExtractor::FileClose;      }
    virtual PFNFDINOTIFY NotifyProgressProc() const { return CCabinetExtractor::NotifyProgress; }

// Miscellaneous helpers
protected:
    virtual BOOL NotifyCabinetInfo(USHORT nID, LPCTSTR lpcszCabPath, LPCTSTR lpcszCabName, LPCTSTR lpcszDiskName, USHORT nCabIndex);
    virtual BOOL NotifyPartialFile(LPCTSTR lpcszFileName, LPCTSTR lpcszFirstCab, LPCTSTR lpcszFirstDisk);
    virtual BOOL NotifyFileCopy(LPCTSTR lpcszFileName, ULONG cbSize, string &strExtractTo, BOOL &bSkipFile);
    virtual BOOL NotifyFileCopied(LPCTSTR lpcszFileName, USHORT &nDate, USHORT &nTime, USHORT &nAttribs);
    virtual BOOL NotifyFileCopiedAndClosed(LPCTSTR lpcszFileName);
    virtual BOOL NotifyEnumerate(USHORT nID, long &nCurrentPosition, USHORT &nFilesRemaining);
    virtual BOOL NotifyNextCabinet(LPCTSTR lpcszNextCab, LPCTSTR lpcszNextDisk, string &strNextCabPath, int nErrorCode);
};

#endif


//--- Decompression errors -------------------------------------------------------

// Clients can retrieve the error code of the last cabinet operation using the
// GetErrorCode and GetErrorCodeEx (additional error info) members
// Possible values returned by GetErrorCode are:
//      FDIERROR_NONE                   No error
//      FDIERROR_CABINET_NOT_FOUND      Cabinet not found
//                                          Bad file name or path passed to FDICopy(), or returned
//                                          to fdintNEXT_CABINET
//      FDIERROR_NOT_A_CABINET          Cabinet file does not have the correct format
//                                          File passed to to FDICopy(), or returned to
//                                          fdintNEXT_CABINET, is too small to be a cabinet file,
//                                          or does not have the cabinet signature in its first 4 bytes
//      FDIERROR_UNKNOWN_CABINET_VERSION Cabinet file has an unknown version number
//                                          File passed to to FDICopy(), or returned to
//                                          fdintNEXT_CABINET, has what looks like a cabinet file
//                                          header, but the version of the cabinet file format
//                                          is not one understood by this version of FDI. The
//                                          erf.erfType field is filled in with the version number
//                                          found in the cabinet file
//      FDIERROR_CORRUPT_CABINET        Cabinet file is corrupt
//                                          FDI returns this error any time it finds a problem
//                                          with the logical format of a cabinet file, and any
//                                          time one of the passed-in file I/O calls fails when
//                                          operating on a cabinet (PFNOPEN, PFNSEEK, PFNREAD,
//                                          or PFNCLOSE). The client can distinguish these two
//                                          cases based upon whether the last file I/O call
//                                          failed or not
//                                          Assuming this is not a real corruption problem in
//                                          a cabinet file, the file I/O functions could attempt
//                                          to do retries on failure (for example, if there is a
//                                          temporary network connection problem).  If this does
//                                          not work, and the file I/O call has to fail, then the
//                                          FDI client will have to clean up and call the
//                                          FDICopy() function again.
//      FDIERROR_ALLOC_FAIL             Could not allocate enough memory
//                                          FDI tried to allocate memory with the PFNALLOC
//                                          function, but it failed
//                                          If possible, PFNALLOC should take whatever steps
//                                          are possible to allocate the memory requested.  If
//                                          memory is not immediately available, it might post a
//                                          dialog asking the user to free memory, for example.
//                                          Note that the bulk of FDI's memory allocations are
//                                          made at FDICreate() time and when the first cabinet
//                                          file is opened during FDICopy()
//      FDIERROR_BAD_COMPR_TYPE         Unknown compression type in a cabinet folder [Should never happen]
//                                          A folder in a cabinet has an unknown compression type.
//                                          This is probably caused by a mismatch between the version of FCI.LIB 
//                                          used to create the cabinet and the FDI.LIB used to read the cabinet
//      FDIERROR_MDI_FAIL               Failure decompressing data from a cabinet file
//                                          The decompressor found an error in the data coming
//                                          from the file cabinet.  The cabinet file was corrupted.
//                                          [When checksuming is turned on, this error should never occur]
//      FDIERROR_TARGET_FILE            Failure writing to target file
//                                          FDI returns this error any time it gets an error back
//                                          from one of the passed-in file I/O calls fails when
//                                          writing to a file being extracted from a cabinet
//                                          To avoid or minimize this error, the file I/O functions
//                                          could attempt to avoid failing.  A common cause might
//                                          be disk full. In this case, the PFNWRITE function
//                                          could have a check for free space, and put up a dialog
//                                          asking the user to free some disk space.
//      FDIERROR_RESERVE_MISMATCH       Cabinets in a set do not have the same RESERVE sizes [Should never happen]
//                                          FDI requires that the sizes of the per-cabinet, per-folder, 
//                                          and per-data block RESERVE sections be consistent across all the 
//                                          cabinets in a set
//      FDIERROR_WRONG_CABINET          Cabinet returned on fdintNEXT_CABINET is incorrect
//                                          Note: This error is NEVER returned by FDICopy()
//                                          Rather, FDICopy() keeps calling the fdintNEXT_CABINET
//                                          callback until either the correct cabinet is specified,
//                                          or you return ABORT.
//                                          When FDICopy() is extracting a file that crosses a
//                                          cabinet boundary, it calls fdintNEXT_CABINET to ask
//                                          for the path to the next cabinet.  Not being very
//                                          trusting, FDI then checks to make sure that the
//                                          correct continuation cabinet was supplied. It does
//                                          this by checking the "setID" and "iCabinet" fields
//                                          in the cabinet. FDI makes sure that the 16-bit
//                                          setID of the continuation cabinet matches the
//                                          cabinet file just processed, then checks that
//                                          the cabinet number (iCabinet) is one more than the
//                                          cabinet number for the cabinet just processed
//      FDIERROR_USER_ABORT             Client requested abort


#endif

