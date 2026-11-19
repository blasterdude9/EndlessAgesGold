#ifndef DIAMOND_TYPES_INCLUDED
#define DIAMOND_TYPES_INCLUDED

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>

#ifdef  __cplusplus
extern "C" {
#endif

// ------------------- standard file I/O defines ------------------------------

#if defined __BORLANDC__
  #define __MFC_COMPAT__
  #define __MSC
#endif

#include <dos.h>
#include <fcntl.h>
#include <sys/stat.h>


#if !defined (__DOS_H) && !defined (_INC_DOS)
  #define _A_NORMAL     0x00
  #define _A_RDONLY     0x01
  #define _A_HIDDEN     0x02
  #define _A_SYSTEM     0x04
  #define _A_ARCH       0x20
#endif  // DOS.H || IO.H

#if !defined (__FCNTL_H)  && !defined (_INC_FCNTL)
  #define _O_RDONLY      0
  #define _O_WRONLY      1
  #define _O_CREAT       0x0100
  #define _O_BINARY      0x8000
#endif  // FCNTL.H

#if !defined (__STAT_H) && !defined (_INC_STAT)
  #define _S_IREAD       0x0100
  #define _S_IWRITE      0x0080
#endif  // STAT.H

#define fatrNormal      _A_NORMAL
#define fatrReadOnly    _A_RDONLY
#define fatrHidden      _A_HIDDEN
#define fatrSystem      _A_SYSTEM
#define fatrDirectory   _A_SUBDIR
#define fatrArchive     _A_ARCH
#define fatrExecute     0x40
#define fatrNameIsUTF   0x80

inline USHORT MaskedAttributes (const DWORD a) {
if (a == 0xFFFFFFFF)
   return fatrNormal;
else return LOWORD (a & (fatrReadOnly|fatrHidden|fatrSystem|fatrArchive));
}

#define  SZ2SHORT(v)   (USHORT) (*((USHORT*)v))

// ------------------- TExpand & TCompress types ------------------------------

#define DIAMONDAPI __cdecl
#pragma pack(4)

#define CAB_MAXCHUNK          32768U
#define CAB_MAXDISK           0x80000000L

#define CAB_MAXCABPATH        256
#define CAB_MAXFILENAME       256
#define CAB_MAXCABNAME        256
#define CAB_MAXDISKNAME       256

typedef CHAR              tPath [MAX_PATH];
typedef const tPath       tConstPath;

typedef CHAR              tCabName [CAB_MAXCABNAME];
typedef CHAR              tCabPath [CAB_MAXCABPATH];
typedef CHAR              tCabFile [CAB_MAXFILENAME];
typedef CHAR              tCabDisk [CAB_MAXDISKNAME];

typedef INT               tFileHandle;

enum  eCreate {
      errSuccess,
      errCreateObject,
      errLoadCabinetDll,
      errInvalidDirectory,
      errInvalidFileName,
      errInavlidCabinet,
      errCantStartHere
      };

typedef enum eCreate      tCreateError;


char szCreateError[][20] =   {{"Success"},
                              {"CreateObject"},
                              {"LoadCabinetDll"},
                              {"InvalidDirectory"},
                              {"InvalidFileName"},
                              {"InvalidCabinet"},
                              {"Can'tStartHere"}};
enum eStoredPath {
      spFileOnly,
      spRelative,
      spFullPath
      };

typedef enum eStoredPath  tStoredPath;

#define DEFSTOREPATH      spFileOnly

// Memory alloc && free for TExpand && TCompress
#define memory_dbg // printf // remove first comment for debug
typedef PVOID  (DIAMONDAPI *pfAlloc)    (ULONG);
typedef VOID   (DIAMONDAPI *pfFree)     (PVOID);

PVOID DIAMONDAPI fAlloc (ULONG u) {
PVOID p = malloc(u);
memory_dbg ("\nalloc(%8.x) [%9.d]",p,u);
return p;
}

VOID  DIAMONDAPI fFree  (PVOID p) {
memory_dbg ("\nfree (%8.x)",p);
free (p);
}

// CabFileHeader
#define CabSignature            (unsigned long) 0x4643534DL

#define cabHasPrev              0x0001
#define cabHasNext              0x0002
#define cabReserve              0x0004

#define fileFromPrev            0xFFFD
#define fileContinued           0xFFFE
#define filePrevAndCont         0xFFFF

typedef struct {
      unsigned long  Signature;  // cabinet file signature
      unsigned long  reserved1;  // reserved
      unsigned long  CabSize;    // size of this cabinet file in bytes
      unsigned long  reserved2;  // reserved
      unsigned long  iFiles;     // offset of the first CFFILE entry
      unsigned long  reserved3;  // reserved
      unsigned char  MinVersion; // cabinet file format version, minor
      unsigned char  Version;    // cabinet file format version, major
      unsigned short Folders;    // number of CFFOLDER entries in this cabinet
      unsigned short nFiles;     // number of CFFILE entries in this cabinet
      unsigned short Flags;      // cabinet file option indicators
      unsigned short SetID;      // must be the same for all cabinets in a set
      unsigned short nCabinet;   // number of this cabinet file in a set
      } CABHEADER;

typedef struct {
      unsigned long  iCabStart;   // offset of the first CFDATA block in this folder
      unsigned short nCFData;     // number of CFDATA blocks in this folder
      unsigned short CompressType;// compression type indicator
      } CABFOLDER;

typedef struct {
      unsigned long  UnComp;      // uncompressed size of this file in bytes
      unsigned long  iFileStart;  // uncompressed offset of this file in the folder
      unsigned short iFolder;     // index into the CFFOLDER area
      unsigned short Date;        // date stamp for this file
      unsigned short Time;        // time stamp for this file
      unsigned short Attrib;      // attribute flags for this file
      } CABFILE;

// -------------------  Compress types ----------------------------------------

class TCompress;
typedef PVOID         thCompress;

enum eCompressError {
      ecNone,
      ecOpenSource,
      ecReadSource,
      ecAllocFail,
      ecTempFail,
      ecBadCompression,
      ecCabFile,
      ecUserAbort,
      ecCompressFail
      };

typedef enum eCompressError tcError;

enum eCBState {
      cbsFile,
      cbsFolder,
      cbsCabinet
      };

typedef enum eCBState       tcCBState;

enum eCompression {
      cmpNone,
      cmpMSZip,
      cmpLZX,
      cmpQuantum,
      cmpBad = 0xF
      };

typedef enum eCompression   tcCompression;

typedef struct {
      ULONG       cb;               // size available for cabinet on this media
      ULONG       cbFolderThresh;   // Thresshold for forcing a new Folder
      UINT        cbReserveCFHeader;// Space to reserve in CFHEADER
      UINT        cbReserveCFFolder;// Space to reserve in CFFOLDER
      UINT        cbReserveCFData;  // Space to reserve in CFDATA
      int         iCab;             // sequential numbers for cabinets
      int         iDisk;            // Disk number
      #ifndef REMOVE_CHICAGO_M6_HACK
      int    fFailOnIncompressible; // TRUE => if a block is incompressible
      #endif
      USHORT setID;                 // Cabinet set ID
      tCabDisk    szDisk;           // current disk name
      tCabName    szCab;            // current cabinet name
      tCabPath    szCabPath;        // path for creating cabinet
      } CCAB;

typedef struct {
      tcError     Error;            // tcError
      INT         Type;             // C-Runtime error (value of errno)
      BOOL        IsError;          // error occured ?
      } COMPRESSERROR;


typedef tFileHandle (DIAMONDAPI *tcpfOpen)(char*,int,int,int*,TCompress*);
typedef UINT (DIAMONDAPI *tcpfRead)       (tFileHandle,void*,UINT,int*,TCompress*);
typedef UINT (DIAMONDAPI *tcpfWrite)      (tFileHandle,void*,UINT,int*,TCompress*);
typedef int  (DIAMONDAPI *tcpfClose)      (tFileHandle,int*,TCompress*);
typedef long (DIAMONDAPI *tcpfSeek)       (tFileHandle,long,int,int*,TCompress*);
typedef int  (DIAMONDAPI *tcpfRemove)     (char *,int*,TCompress*);

typedef int  (DIAMONDAPI *tccbReplace)    (CCAB*,char*,long,BOOL,TCompress*);
typedef long (DIAMONDAPI *tccbStatus)     (tcCBState,ULONG,ULONG,TCompress*);
typedef BOOL (DIAMONDAPI *tccbTempName)   (char*,int,TCompress*);
typedef BOOL (DIAMONDAPI *tccbNextCab)    (CCAB*,ULONG,TCompress*);
typedef int  (DIAMONDAPI *tccbOpenInfo)   (char*,USHORT*,USHORT*,USHORT*,int*,TCompress*);

// ------------------- Expand types -------------------------------------------

class TExpand;
typedef PVOID         thExpand;
typedef INT           eUnusedFlag;

enum eCPUType   {CPU286 = 0,CPU386 = 1,CPUUnknown = -1};
typedef enum eCPUType       teCpuType;

enum eExpandError {
      eeNone,
      eeCabNotFound,
      eeNotACabinet,
      eeUnknownCabVersion,
      eeCorruptCabinet,
      eeAllocFail,
      eeBadCompression,
      eeMDIFail,
      eeTargetFile,
      eeReserveMismatch,
      eeWrongCabinet,
      eeUserAbort
      };

typedef enum eExpandError  teError;

enum eDecryptType {
      dtNewCabinet,     // New cabinet
      dtNewFolder,      // New folder
      dtDecrypt         // Decrypt a data block
      };

typedef enum eDecryptType   teDecryptType;

enum eNotifyType {
      ntCabinetInfo,    // General information about cabinet
      ntPartial,        // First file in cabinet is continuation
      ntCopy,           // File to be copied
      ntCloseInfo,      // close the file, set relevant info
      ntNextCabinet,    // File continued to next cabinet
      ntEnumerate,      // Enumeration status
      };

typedef enum eNotifyType   teNotifyType;

typedef struct {
      teError   Error;
      int       Type;   // C-Runtime error (value of errno)
      BOOL      IsError;// error occured ?
      } EXPANDERROR;

typedef struct {
      teDecryptType  dt;               // Command type (selects union below)
      TExpand*       pThis;            // Decryption context

      union {
         struct {                      // dtNewCabinet
            void     *pHeaderReserve;  // RESERVE section from CFHEADER
            USHORT   cbHeaderReserve;  // Size of pHeaderReserve
            USHORT   setID;            // Cabinet set ID
            int      iCabinet;         // Cabinet number in set (0 based)
            } cabinet;

         struct {                       // dtNewFolder
            void     *pFolderReserve;  // RESERVE section from CFFOLDER
            USHORT   cbFolderReserve;  // Size of pFolderReserve
            USHORT   iFolder;          // Folder number in cabinet (0 based)
            } folder;

         struct {                       // dtDecrypt
            void     *pDataReserve;    // RESERVE section from CFDATA
            USHORT   cbDataReserve;    // Size of pDataReserve
            void     *pbData;          // Data buffer
            USHORT   cbData;           // Size of data buffer
            BOOL     fSplit;           // TRUE if this is a split data block
            USHORT   cbPartial;        // 0 if this is not a split block, or
                                       //  the first piece of a split block;
                                       // Greater than 0 if this is the
                                       //  second piece of a split block.
            } decrypt;
         };
      } DECRYPT;

typedef struct {
      long        cb;
      char        *c1;
      char        *c2;
      char        *c3;                // Points to a 256 character buffer
      TExpand*    pv;                 // Value for client

      tFileHandle hf;

      USHORT      date;
      USHORT      time;
      USHORT      attribs;

      USHORT      SetID;              // Cabinet set ID
      USHORT      iCabinet;           // Cabinet number (0-based)
      USHORT      iFolder;            // Folder number (0-based)
      teError     error;
      } NOTIFICATION;

typedef tFileHandle (DIAMONDAPI *tepfOpen)    (char *file,int access,int mode);
typedef UINT        (DIAMONDAPI *tepfRead)    (tFileHandle,void *pv,UINT cb);
typedef UINT        (DIAMONDAPI *tepfWrite)   (tFileHandle,void *pv,UINT cb);
typedef long        (DIAMONDAPI *tepfSeek)    (tFileHandle,long dist,int from);
typedef int         (DIAMONDAPI *tepfClose)   (tFileHandle);

typedef int         (DIAMONDAPI *tecbDecrypt) (DECRYPT*);
typedef int         (DIAMONDAPI *tecbNotify)  (teNotifyType,NOTIFICATION*);

// -------------------------- cabinet.dll -------------------------------------
// prototypes for exported functions

typedef thCompress (DIAMONDAPI *tclfCreate) (COMPRESSERROR*,
                                             tccbReplace,
                                             pfAlloc,pfFree,
                                             tcpfOpen,tcpfRead,tcpfWrite,
                                             tcpfClose,tcpfSeek,tcpfRemove,
                                             tccbTempName,CCAB*,TCompress*);

typedef BOOL (DIAMONDAPI       *tclfAdd)    (thCompress,char*,char*,BOOL,
                                             tccbNextCab,
                                             tccbStatus,
                                             tccbOpenInfo,
                                             tcCompression);

typedef BOOL (DIAMONDAPI       *tclfFlush)  (thCompress,tccbNextCab,tccbStatus);

typedef BOOL (DIAMONDAPI       *tclfStore)  (thCompress,
                                             BOOL,tccbNextCab,tccbStatus);

typedef BOOL (DIAMONDAPI       *tclfDestroy)(thCompress);

         // --------------------------------------

typedef thExpand (DIAMONDAPI   *telfCreate) (pfAlloc,pfFree,
                                             tepfOpen,tepfRead,tepfWrite,
                                             tepfClose,tepfSeek,
                                             teCpuType,EXPANDERROR*);

typedef BOOL (DIAMONDAPI       *telfCopy)   (thExpand,char*,char*,eUnusedFlag,
                                             tecbNotify,tecbDecrypt,TExpand*);

typedef BOOL (DIAMONDAPI       *telfDestroy)(thExpand);

#pragma pack()

#ifdef  __cplusplus
}
#endif  // extern "c"
#endif  // DIAMOND_TYPES_INCLUDED
