#ifndef EXPAND_HEADER_INCLUDED
#define EXPAND_HEADER_INCLUDED

#include "DiamondTypes.h"
#include <io.h>           // include Standard C FileIO

#if defined __BORLANDC__
  #pragma option -w-inl   // turn off "cannot expand inline..." warnings
#endif

class TExpand {
  public:

   TExpand  ()  {
      bTerminate = false;

      hfdi         = NULL;
      hLibrary     = NULL;
      plfCreate    = NULL;
      plfCopy      = NULL;
      plfDestroy   = NULL;

      ZeroMemory (&Error,sizeof(Error));
      ZeroMemory (m_BaseDir,sizeof(m_BaseDir));
      ZeroMemory (m_CabFile,sizeof(m_CabFile));
      ZeroMemory (m_CabPath,sizeof(m_CabPath));
      ZeroMemory (m_CurrentFile,sizeof(m_CurrentFile));

      m_SetID              = 0xFFFF;
      m_ExpandStoredPath   = DEFSTOREPATH;
      m_Incomplete         = false;
      m_SubSequent         = true;
      }

   ~TExpand () {
      Destroy();
      if (hLibrary != NULL)
         FreeLibrary (hLibrary);
      }

   // terminate any action
   void           Terminate          ()                     {bTerminate = true;}
   // true at end or expand is broken
   bool           Terminated         ()                     {return bTerminate;}
   // true on any error
   BOOL           IsError            ()                  {return Error.IsError;}
   // get FDI error
   teError        FdiError           ()                    {return Error.Error;}
   // get  Systemerror (errno)
   int            SysError           ()                     {return Error.Type;}
   // set how stored path should be restored
   void           SetExpandStoredPath(tStoredPath s)   {m_ExpandStoredPath = s;}
   // expand subsequent Cabinets from a set of CabinetFiles
   void           SetSubsequent      (bool e)                {m_SubSequent = e;}
   // get the first cab in a set , start on any file from within a set
   bool           GetFirstCab        (PCHAR AnyCabinet,tPath FirstCabinet);
   // create the full path given by path
   bool           CreatePath         (tConstPath Path);
   // try to open a file readonly given by path
   bool           FileAvaliable      (tConstPath Path);
   // check if a directory exists
   bool           DirectoryExists    (tConstPath path);
   // gets the name of the currently expanded file
   bool           GetCurrentFile     (PCHAR File,UINT nFile);
   // is the last file incomplete  (is continued in a not avaliable cabinet)
   bool           HasIncomplete      (PCHAR File,UINT nFile);


protected:

   // set the base dir if BasePath on create is == NULL
   inline virtual void BaseExpandDir (tPath path) {
      GetTempPath (sizeof(tPath),path);
      }

   /* **************   FDI - Status Callbacks ********************************/
   // set a step in a progress bar
   inline virtual void CBProgress    ()                                      {;}

   // next file is needed set CabinetPath to the location of CabFile_Needed
   inline virtual void CBNextCabinet (const PCHAR  CabFile_Needed,
                                      const PCHAR  Disk_Needed,
                                      tCabPath     CabinetPath,
                                      teError     &Error)                    {;}

   // set FileTime and Attributes for the expanded file
   inline virtual void CBCloseInfo   (const PCHAR  FileName_OnDisk,
                                      USHORT       FileTime_ToSet,
                                      USHORT       FileDate_ToSet,
                                      USHORT       FileAttribute_ToSet)      {;}

   // return false to skip the file, set error != eeNone to terminate
   inline virtual bool CBCopyFile    (tPath        FileName,
                                      LONG         UncompressedSize,
                                      SYSTEMTIME   SysTime_InCab,
                                      USHORT       Attribs_InCab,
                                      bool        &Error)         {return true;}

   /* **************   FDI - Library *****************************************/
   // Create a new FDI-Context for CabFile
   tCreateError   Create             (PCHAR CabFile,PCHAR BasePath = NULL);
   // start Expand
   BOOL           Copy               (eUnusedFlag f = 0);
   // Destroy the Context
   void           Destroy            ();

private:

   thExpand       hfdi;                           // FDI Context Handle
   EXPANDERROR    Error;                          // FDI ErrorStruct
   bool           bTerminate;                     // terminate immediately

   HMODULE        hLibrary;                       // cabinet.dll
   telfCreate     plfCreate;                      // FDICreate
   telfCopy       plfCopy;                        // FDICopy
   telfDestroy    plfDestroy;                     // FDIDestroy

   USHORT         m_SetID;                        // current SetId
   bool           m_Incomplete;                   // current file is incomplete
   tStoredPath    m_ExpandStoredPath;             // expand File w or w/o Path
   bool           m_SubSequent;                   // expand subsequent Archives
   tPath          m_BaseDir;                      // Basic Target Directory
   tCabFile       m_CabFile;                      // Cabinet FileName
   tCabPath       m_CabPath;                      // Cabinet Path ending
   CHAR           m_CurrentFile[sizeof(tPath)*2]; // current extracted file

   //  intern used memberprocs
   bool           mf_storedpath      (PCHAR path);
   bool           mf_getnext         ();
   tFileHandle    mf_sureopen        ();
   inline bool    mf_loadlib         ();
   inline bool    mf_isvalid         (PCHAR File,BOOL *CanStart,USHORT *SetID);

   /* **************   Callback - Adapter  **********************************/
   // FDI Callbacks
   #define                      acbDecrypt NULL   // currently not used by FDI
   static int        DIAMONDAPI acbNotify (teNotifyType type,NOTIFICATION *p) {
      INT     r = 0;
      TExpand *o = (TExpand*) p->pv;

      switch (type) {

         case ntNextCabinet: {
            o->CBNextCabinet (p->c1,p->c2,p->c3,p->error);

            if (p->error != eeNone)    // don't infinite loop here
               o->bTerminate = true;   // error must be reset in CBNextCabinet()
            break;
            }

         case ntCopy: {
            o->mf_storedpath (p->c1);  // write dest.path in m_CurrentFile

            SYSTEMTIME   st;
            FILETIME     ft;
            DosDateTimeToFileTime (p->date,p->time,&ft);
            FileTimeToSystemTime (&ft,&st);

            tFileHandle  hf = -1;
            bool         bError = false;

            while (hf == -1) {
               if (o->CBCopyFile(o->m_CurrentFile,p->cb,st,p->attribs,bError)) {

                  if (o->bTerminate || bError)
                     break;            // if bError not reset Terminate Copy()

                  if ((hf = o->mf_sureopen ()) != -1) {
                     r = hf;
                     o->m_Incomplete = true;
                     }
                  else bError = true;
                  }
               else r = 0;             // don't copy, skip file
               }
            break;
            }

         case ntCloseInfo: {
            acbClose (p->hf);
            o->CBCloseInfo(p->c1,p->date,p->time,MaskedAttributes(p->attribs));
            o->m_Incomplete = false;
            r = TRUE;                  // must return true for success
            break;
            }

         default: {
            o->CBProgress();           // ntEnumerate ,ntPartial, ntCabinetInfo
            break;                     // and other unknown notifications
            }
         }
      return o->bTerminate ? -1 : r;
      }

   /* **************   File I/O Callbacks **********************************/
   //

   static tFileHandle DIAMONDAPI acbOpen   (char *f,int a,int m) {
      return open (f,a,m);
      }

   static UINT        DIAMONDAPI acbRead   (tFileHandle h,void *m,UINT n) {
      return read (h,m,n);;
      }

   static UINT        DIAMONDAPI acbWrite  (tFileHandle h,void *m,UINT n) {
      return write (h,m,n);
      }

   static int         DIAMONDAPI acbClose  (tFileHandle h) {
      return close(h);
      }

   static long        DIAMONDAPI acbSeek   (tFileHandle h,long d,int t) {
      return lseek  (h,d,t);
      }

   static int         DIAMONDAPI acbRemove (char* f) {
      return unlink (f);
      }
};

#if defined __BORLANDC__
  #pragma option -w.inl
#endif
  
#include "TExpand.cpp"
#endif
