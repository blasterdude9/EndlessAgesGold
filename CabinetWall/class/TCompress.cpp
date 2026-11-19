#include "TCompress.hpp"

/******************************************************************************/
//  default Callbacks
/******************************************************************************/

void         TCompress::CBCabName (tCabName NextCabinet,INT iCab) {
if (*m_BaseName) {
   PCHAR p = strchr (m_BaseName,'%');

   if (p != NULL) {
      tCabName  name;
      CHAR  *cnt = p;
      INT   num = 0,base = (int) (p-m_BaseName);

      while (*cnt++ == '%') num++;

      strncpy (name,m_BaseName,base);
      name [base] = '\0';

      wsprintf (NextCabinet,"%s%.*d.cab",name,num,iCab);
      }
   else if (! iCab) {
      wsprintf (NextCabinet,"%s.cab",m_BaseName);
      }
   else wsprintf (NextCabinet,"%s%.3d.cab",m_BaseName,iCab);
   }
else wsprintf (NextCabinet,"%.4d.cab",iCab);
} // --------------------------------------------- /
void         TCompress::CBDiskName (tCabDisk NextDisk,INT iDisk) {
wsprintf (NextDisk,"Disk%3.d",iDisk);
}

/******************************************************************************/
// FCI library
/******************************************************************************/

inline bool  TCompress::mf_loadlib () {
if (hLibrary != NULL || (hLibrary = ::LoadLibrary ("cabinet")) != NULL) {
   plfCreate  = (tclfCreate)  GetProcAddress(hLibrary,"FCICreate");
   plfAdd     = (tclfAdd)     GetProcAddress(hLibrary,"FCIAddFile");
   plfFlush   = (tclfFlush)   GetProcAddress(hLibrary,"FCIFlushFolder");
   plfStore   = (tclfStore)   GetProcAddress(hLibrary,"FCIFlushCabinet");
   plfDestroy = (tclfDestroy) GetProcAddress(hLibrary,"FCIDestroy");
   }
return  hLibrary != NULL && plfCreate != NULL && plfAdd != NULL &&
                                         plfStore != NULL && plfDestroy != NULL;
} // --------------------------------------------- /
bool         TCompress::mf_direxists (PCHAR dir) {
PCHAR p = dir;

if (SZ2SHORT(&dir[1])  == SZ2SHORT(":\\")) {
   p += 2;
   }
else if (SZ2SHORT(dir) == SZ2SHORT("\\\\")) {
   if ((p = strrchr (&p[2],'\\')) != NULL) {
      p = strrchr (&p[1],'\\');
      }
   }

if (p != NULL && *p) {
   while ((p = strchr (&p[1],'\\')) != NULL) {
      *p = '\0';
      CreateDirectory (dir,NULL);
      *p= '\\';
      }
   CreateDirectory (dir,NULL);
   }
return SetCurrentDirectory (dir) != 0;
} // --------------------------------------------- /
tCreateError TCompress::Create (PCHAR File,PCHAR Path,ULONG MediaSize)
{
Destroy();

tCreateError   success = errSuccess;
CCAB           ccab;

ZeroMemory (&Error,sizeof(Error));
ZeroMemory (m_BaseName,sizeof(m_BaseName));
ZeroMemory (&ccab,sizeof(ccab));

srand (GetTickCount());

ccab.cb              = MediaSize;
ccab.cbFolderThresh  = MediaSize;
ccab.setID           = LOWORD (rand());

if (*Path != NULL && *Path) {
   if (mf_loadlib()) {
      bTerminate = false;
      size_t n = strlen (Path);

      if (n > 2 && n < sizeof(tCabPath)) {
         strncpy (m_BaseName,File,sizeof (m_BaseName));
         m_BaseName[sizeof (m_BaseName) -1] = '\0';

         if (Path[n-1] != '\\') {
            Path[n++] = '\\';
            Path[n] = '\0';
            }

         strncpy (ccab.szCabPath,Path,sizeof(ccab.szCabPath));
         ccab.szCabPath[sizeof(ccab.szCabPath) -1] = '\0';

         acbNextCab (&ccab,0,this);

         if (mf_direxists (ccab.szCabPath)) {
            if ((hfci = plfCreate (&Error,acbReplace,
                                          fAlloc,fFree,
                                          acbOpen,acbRead,acbWrite,acbClose,
                                          acbSeek,acbRemove,
                                          acbTempName,&ccab,this)) == NULL)
               success = errCreateObject;
            }
         else success = errInvalidDirectory;
         }
      else success = errInvalidDirectory;
      }
   else success = errLoadCabinetDll;
   }
else success = errInvalidDirectory;
return (success);
} // --------------------------------------------- /
BOOL         TCompress::Store (BOOL getnext) {
BOOL success = false;

if (hfci != NULL) {
   if (! bTerminate) {
      success = plfStore (hfci,getnext,acbNextCab,acbStatus);
      }
   else success = true;
   }
return success;
} // --------------------------------------------- /
void         TCompress::Destroy (void) {
if (hfci != NULL)
   plfDestroy (hfci);
hfci = NULL;
} // --------------------------------------------- /
BOOL         TCompress::Add (PCHAR pf,BOOL bExecute) {
BOOL success = false;
tPath FilePath;
PCHAR File,NameInCab;

GetFullPathName (pf,sizeof(FilePath),FilePath,&File);

if (hfci != NULL && ! bTerminate) {

   switch (m_StorePath) {
      case spRelative:
         if (SZ2SHORT(FilePath+1) == SZ2SHORT(":\\")) {      // driveletter
            NameInCab = &FilePath[3];
            }
         else if (SZ2SHORT(FilePath) == SZ2SHORT("\\\\")) {  // UNC - path
            PCHAR p;
            if ((p = strchr (&FilePath[2],'\\')) != NULL && *(p+1)) {
               PCHAR s;
               if ((s = strchr (p+1,'\\')) != NULL && *(s+1))
                  NameInCab = s+1;
               else NameInCab = p;
               }
            else NameInCab = &FilePath[2];
            }
         else NameInCab = pf;                                // any unknown stuff
         break;

      case spFullPath:
         NameInCab = FilePath;
         break;

      default:
         NameInCab = File;
         break;
      }

   strcpy (m_FileStatus.Name,File);
   m_FileStatus.Position = m_FileStatus.Compressed = 0;
   m_FileStatus.Size = 0xFFFFFFFF;

   INT         dummy;
   tFileHandle hf;
   if ((hf = acbOpen (FilePath,_O_RDONLY|_O_BINARY,_S_IREAD,&dummy,this))
                                                                      != -1) {
      m_FileStatus.Size = acbSeek (hf,0,SEEK_END,&dummy,this);
      acbClose (hf,&dummy,this);
      success = plfAdd (hfci,FilePath,NameInCab,bExecute,acbNextCab,acbStatus,
                                                     acbOpenInfo,m_Compression);
      }
   }
return bTerminate ? true : success;
}

// EOF

