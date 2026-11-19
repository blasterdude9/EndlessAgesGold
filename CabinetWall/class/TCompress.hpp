#ifndef COMPRESS_HEADER_INCLUDED
#define COMPRESS_HEADER_INCLUDED
#include "DiamondTypes.h"
#include <io.h>            // include Standard C FileI/O

#if defined __BORLANDC__
  #pragma option -w-inl  // turn off "cannot expand inline..."
#endif  

class TCompress {
public:
    TCompress () {
      bTerminate = false;

      hfci        = NULL;
      hLibrary    = NULL;
      plfCreate   = NULL;
      plfAdd      = NULL;
      plfStore    = NULL;
      plfDestroy  = NULL;
      ZeroMemory (&Error,sizeof(Error));

      ZeroMemory (m_BaseName,sizeof(m_BaseName));
      m_Compression = cmpMSZip;
      m_StorePath   = DEFSTOREPATH;
      }

   ~TCompress () {
      Destroy();
      if (hLibrary != NULL)
         FreeLibrary (hLibrary);
      }

   // terminate any action
   void                Terminate  ()                        {bTerminate = true;}
   // true at end or expand is broken
   bool                Terminated ()                        {return bTerminate;}
   // true on any error
   BOOL                IsError    ()                     {return Error.IsError;}
   // get  FCIError (errno)
   tcError             FciError   ()                       {return Error.Error;}
   // get  Systemerror (errno)
   int                 SysError   ()                        {return Error.Type;}

protected:

   typedef ULONG     USELESS;           // useless Params in CBFolder
   typedef INT       tcFolderStatus;    // three CBFolder() States
   typedef struct tagCurrentFileStatus {
      tCabFile Name;        // current file
      ULONG    Size;        // uncompressed size
      ULONG    Position;    // current position in file
      ULONG    Compressed;  // compressed size for position
      } tcFileStatus;

   // ----------------------------


   /* **************   FCI - Status Callbacks ******************************/
   // this callbacks would be called on misc points on compression (see simple)
   inline virtual void CBReplaced (const PCHAR File,BOOL IsContinued)        {;}
   inline virtual void CBFile     (const tcFileStatus &Status)               {;}
   inline virtual void CBFolder   (tcFolderStatus s,USELESS ul1,USELESS ul2) {;}
   inline virtual void CBCabinet  (ULONG CabinetSizeWritten)                 {;}

   /*****   Add() Requests (needs information about files or folder) ******/
   // set Filetime && Attributes should be stored in Cabinet
   inline virtual void CBOpenInfo (const PCHAR File,FILETIME *t,USHORT &Attr){;}
   // get the name of a temporary directory
   inline virtual void CBTempName (PCHAR szTemp,int nTemp)    {tmpnam (szTemp);}
   // called on splitted Cabinets when the name for the next CabinetFile is needed
   virtual  void       CBCabName  (tCabName NextCabName,INT Count);
   // called on splitted Cabinets when the name for the next Disk is needed
   virtual  void       CBDiskName (tCabDisk NextDiskName,INT Count);

   /* **************   FCI - Library ****************************************/
   // Initialize a new FCI Context
   tCreateError        Create     (PCHAR Cab,PCHAR Path,ULONG Size = CAB_MAXDISK);
   // add a file to Cabinet set bExec when the file should ecexuted at finish
   BOOL                Add        (const PCHAR File,BOOL bExec = false);
   // flush a folder within a CabinetFile
   BOOL                Flush      ();
   // store the CabinetFile to Disk
   BOOL                Store      (BOOL bGetNextCabinet = false);
   // destroy the FCI Context
   void                Destroy    ();

   // set/unset Compression (other tcCompression-Types are not used in FCI)
   inline void SetCompression (bool b) {m_Compression = b ? cmpMSZip : cmpNone;}
   // set Information stored in Archiv (w/o path, relative path or full path)
   inline void SetStoredPath  (tStoredPath sp)               {m_StorePath = sp;}
   // File I/O - Callbacks

private:

   thCompress     hfci;             // FCI Contexthandle
   COMPRESSERROR  Error;            // FCI Error Struct
   bool           bTerminate;       // terminate immediately current action

   HMODULE        hLibrary;         // cabinet.dll
   tclfCreate     plfCreate;        // FCICreate
   tclfAdd        plfAdd;           // FCIAddFile
   tclfFlush      plfFlush;         // FCIFlushFolder
   tclfStore      plfStore;         // FCIFlushCabinet
   tclfDestroy    plfDestroy;       // FCIDestroy

   CHAR           m_BaseName[64];   // Basename used in GetNextCabinet()
   tStoredPath    m_StorePath;      // store Path for Filename in Archiv or not
   tcCompression  m_Compression;    // current Compression for Add()
   tcFileStatus   m_FileStatus;     // current FileStatus
   tcFolderStatus m_FolderStatus;   // used to switch Folder_x() Procs

   inline bool    mf_loadlib();     // load Lib & ProcPtrs, used in Create()
   bool mf_direxists (PCHAR dir);   // ensure Directory exists

   /* **************   FCI Callback - Adapter  ******************************/
   static int  DIAMONDAPI acbReplace (CCAB *pccab,char *f,long n,BOOL c,TCompress* o) {
      o->CBReplaced (f,c);
      o->m_FolderStatus = 2;
      return  o->bTerminate ? -1 : 0;
      }

   static long DIAMONDAPI acbStatus  (tcCBState s,ULONG cb1,ULONG cb2,TCompress* o) {
      if (s == cbsFile) {
         o->m_FolderStatus = 1;
         o->m_FileStatus.Compressed += cb1;
         o->m_FileStatus.Position   += cb2;
         o->CBFile (o->m_FileStatus);
         }
      else if (s == cbsFolder) {
         o->CBFolder (o->m_FolderStatus,cb1,cb2);
         }
      else if (s == cbsCabinet) {
         o->m_FolderStatus = 3;
         o->CBCabinet (cb2);
         }
      return  o->bTerminate ? -1 : (cbsCabinet ? cb2 : 0);
      }

   static BOOL DIAMONDAPI acbTempName(char* t,int n,TCompress* o) {
      o->CBTempName (t,n);
      return  o->bTerminate ? false : true;
      }

   static BOOL DIAMONDAPI acbNextCab (CCAB *pccab,ULONG p,TCompress* o) {
      o->CBDiskName (pccab->szDisk,pccab->iDisk +1);
      o->CBCabName (pccab->szCab,pccab->iCab);
      return  o->bTerminate ? false : true;
      }

   static int  DIAMONDAPI acbOpenInfo(char *f,USHORT *d,USHORT *t,USHORT *a,int *err,TCompress*o) {
      FILETIME ft;
      DosDateTimeToFileTime (*d,*t,&ft);
      o->CBOpenInfo (f,&ft,*a);
      FileTimeToDosDateTime (&ft,d,t);
      return acbOpen (f,_O_RDONLY|_O_BINARY,_S_IREAD,err,o);
      }

   /* **************   File I/O Callback Adapter ****************************/

   static tFileHandle DIAMONDAPI acbOpen  (char *f,int a,int m,int *e,TCompress* o) {
      tFileHandle h = open (f,a,m);
      *e = errno;
      return h;
      }

   static UINT        DIAMONDAPI acbRead  (tFileHandle h,void *m,UINT n,int *e,TCompress* o) {
      UINT r = read (h,m,n);
      *e = errno;
      return r;
      }

   static UINT        DIAMONDAPI acbWrite (tFileHandle h,void *m,UINT n,int *e,TCompress* o) {
      UINT r  = write (h,m,n);
      *e = errno;
      return r;
      }

   static int         DIAMONDAPI acbClose (tFileHandle h,int *e,TCompress* o) {
      int r = close (h);
      *e = errno;
      return r;
      }

   static long        DIAMONDAPI acbSeek  (tFileHandle h,long d,int t,int *e,TCompress* o) {
      long r = lseek (h,d,t);
      *e = errno;
      return r;
      }

   static int         DIAMONDAPI acbRemove(char* f,int *e,TCompress* o) {
      chmod (f,_S_IREAD|_S_IWRITE);
      int r = unlink (f);
      *e = errno;
      return r;
      }
};

#if defined __BORLANDC__
  #pragma option -w.inl
#endif

#include "TCompress.cpp"
#endif

