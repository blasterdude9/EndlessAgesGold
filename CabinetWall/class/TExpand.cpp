#include "TExpand.hpp"
/******************************************************************************/
//
/******************************************************************************/

bool         TExpand::mf_storedpath (PCHAR Stored) {
ZeroMemory (m_CurrentFile,sizeof(m_CurrentFile));

switch  (m_ExpandStoredPath) {

   case spFileOnly: {
      PCHAR p = strrchr (Stored,'\\');
      wsprintf(m_CurrentFile,"%s%s",m_BaseDir,p != NULL && *(++p) ? p : Stored);
      break;
      }

   case spFullPath: {
      if (SZ2SHORT(&Stored[1]) == SZ2SHORT(":\\") ||
              SZ2SHORT(Stored) == SZ2SHORT("\\\\")) {
         strcpy (m_CurrentFile,Stored);
         }
      else wsprintf (m_CurrentFile,"%s%s",m_BaseDir,Stored);
      break;
      }

   case spRelative: {
      PCHAR p = Stored;
      if (SZ2SHORT(&Stored[1]) == SZ2SHORT(":\\")) {
         p = &Stored[3];
         }
      else if (SZ2SHORT(Stored) == SZ2SHORT("\\\\")) {
         if ((p = strchr (&Stored[2],'\\')) != NULL && *(++p)) {
            PCHAR s;
            if ((s = strchr (p,'\\')) != NULL && *(++s))
               p = s;
            }
         else p = &Stored[2];
         }
      wsprintf (m_CurrentFile,"%s%s",m_BaseDir,p);
      break;
      }
   }
return m_CurrentFile[0] != '\0';
} // --------------------------------------------- /

tFileHandle  TExpand::mf_sureopen (void) {
tFileHandle hf = -1;

if (strlen (m_CurrentFile) <= MAX_PATH) {
   PCHAR pf;

   if ((pf = strrchr (m_CurrentFile,'\\')) != NULL) {
      *pf = '\0';
      if (CreatePath (m_CurrentFile)) {
         *pf = '\\';
         hf = acbOpen (m_CurrentFile,_O_BINARY|_O_CREAT|_O_WRONLY,
                                                           _S_IREAD|_S_IWRITE);
         }
      *pf = '\\';
      }
   }
return hf;
} // --------------------------------------------- /

bool         TExpand::GetFirstCab (const PCHAR cabinet,tPath First) {
bool        success = false;
PCHAR       pf;
tPath       path;
tPath       file;
tFileHandle hf;

if (GetFullPathName (cabinet,sizeof(path),path,&pf)) {
   strcpy (file,pf);
   *pf = '\0';

   teError Error = eeNone;

   do {
      wsprintf (First,"%s%s",path,file);

      if ((hf = acbOpen (First,_O_BINARY|_O_RDONLY,_S_IREAD)) != -1) {
         CABHEADER    hd;

         if (acbRead(hf,(void*) &hd,sizeof(CABHEADER)) == sizeof(CABHEADER) &&
                                                CabSignature == hd.Signature) {
            if (hd.Flags & cabHasPrev) {

               if (hd.Flags & cabReserve)
                  acbSeek (hf,5,SEEK_CUR);
                  
               char buffer[sizeof(tCabName)];
               if (acbRead (hf,buffer,sizeof(buffer))) {
                  strcpy (file,buffer);
                  }
               else Error = eeNotACabinet;
               }
            else success = true;
            }
         else Error = eeNotACabinet;
         acbClose (hf);
         }
      else {
         Error = eeCabNotFound;
         CBNextCabinet (file,"",path,Error);
         }
      } while (Error == eeNone && ! success);
   }
return success;
} // --------------------------------------------- /

bool         TExpand::CreatePath (const tPath PSet) {
tPath Path;
memcpy (Path,PSet,sizeof(Path));
PCHAR p = Path;

if (SZ2SHORT(&PSet[1])  == SZ2SHORT(":\\")) {
   p += 2;
   }
else if (SZ2SHORT(PSet) == SZ2SHORT("\\\\")) {
   if ((p = strrchr (&p[2],'\\')) != NULL) {
      p = strrchr (&p[1],'\\');
      }
   }

if (p != NULL && *p) {
   while ((p = strchr (&p[1],'\\')) != NULL) {
      *p = '\0';
      CreateDirectory (Path,NULL);
      *p= '\\';
      }
   CreateDirectory (Path,NULL);
   }

return DirectoryExists (Path);
} // --------------------------------------------- /

bool         TExpand::FileAvaliable (const tPath file) {
bool        success = false;
tFileHandle hf;

if ((hf = acbOpen ((char*) file,_O_BINARY|_O_RDONLY,_S_IREAD)) > 0) {
   success = true;
   acbClose (hf);
   }
return success;
} // --------------------------------------------- /

bool         TExpand::DirectoryExists (const tPath path) {
tPath p;
strcpy (p,path);

int n = strlen (p) -1;
if (p[n] == '\\') p[n] = '\0';

WIN32_FIND_DATA   ff;
HANDLE            hf;
bool              success = false;

if ((hf = FindFirstFile(p,&ff)) != INVALID_HANDLE_VALUE) {
   FindClose (hf);
   success =  (ff.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) != 0;
   }
return success;
} // --------------------------------------------- /
bool         TExpand::GetCurrentFile (PCHAR File,UINT nFile) {
bool success = false;

if (nFile > (strlen (m_CurrentFile) +1)) {
   strcpy (File,m_CurrentFile);
   success = true;
   }
return success;
} // --------------------------------------------- /
bool         TExpand::HasIncomplete (PCHAR File,UINT nFile) {
if (m_Incomplete)
   GetCurrentFile (File,nFile);
else ZeroMemory (File,nFile);
return m_Incomplete;
} // --------------------------------------------- /
/******************************************************************************/
//
/******************************************************************************/
tCreateError TExpand::Create (PCHAR File,PCHAR Target) {
tCreateError success;

Destroy ();

if (hLibrary != NULL || mf_loadlib()) {
   ZeroMemory (m_CabFile,sizeof(m_CabFile));
   ZeroMemory (m_CabPath,sizeof(m_CabPath));
   ZeroMemory (m_BaseDir,sizeof(m_BaseDir));

   if (Target != NULL)
      strncpy (m_BaseDir,Target,sizeof(m_BaseDir) -2);
   else BaseExpandDir (m_BaseDir);

   INT ntgt = strlen (m_BaseDir) -1;

   if (m_BaseDir[ntgt] != '\\') {
      m_BaseDir[++ntgt] = '\\';
      m_BaseDir[++ntgt] = '\0';
      }

   if (m_ExpandStoredPath == spFullPath || CreatePath (m_BaseDir)) {
      PCHAR pf;

      if (GetFullPathName (File,sizeof(m_CabPath),m_CabPath,&pf)) {
         BOOL      bStart;

         if (mf_isvalid (m_CabPath,&bStart,&m_SetID)) {
            if (bStart) {
               strcpy (m_CabFile,pf);
               *pf = '\0';

               if ((hfdi = plfCreate (fAlloc,fFree,acbOpen,acbRead,acbWrite,
                                    acbClose,acbSeek,CPU386,&Error)) != NULL) {
                  success = errSuccess;
                  }
               else success = errCreateObject;
               }
            else success = errCantStartHere;
            }
         else success = errInavlidCabinet;
         }
      else success = errInvalidFileName;
      }
   else success = errInvalidDirectory;
   }
else success = errLoadCabinetDll;
return success;
} // --------------------------------------------- /
inline bool  TExpand::mf_loadlib () {
if (hLibrary != NULL || (hLibrary = ::LoadLibrary ("cabinet")) != NULL) {
   plfCreate    = (telfCreate)    GetProcAddress(hLibrary,"FDICreate");
   plfCopy      = (telfCopy)      GetProcAddress(hLibrary,"FDICopy");
   plfDestroy   = (telfDestroy)   GetProcAddress(hLibrary,"FDIDestroy");
   }
return hLibrary != NULL &&
                     plfCreate != NULL && plfCopy != NULL && plfDestroy != NULL;
} // --------------------------------------------- /
inline bool  TExpand::mf_isvalid (PCHAR File,BOOL *CanStart,USHORT *SetID) {
bool        success = false;
CABHEADER   hd;
tFileHandle hf;
if ((hf = acbOpen (File,_O_BINARY|_O_RDONLY,_S_IREAD)) != -1) {
   if (acbRead(hf,(void*) &hd,sizeof (CABHEADER)) == sizeof(CABHEADER) &&
                                            CabSignature == hd.Signature) {
      success = true;

      if (SetID != NULL)
         *SetID  = hd.SetID;

      if (CanStart != NULL) {
         *CanStart = false;

         if (hd.nFiles == 1) {
            if (acbSeek (hf,hd.iFiles,SEEK_SET)) {
               CABFILE cf;

               if (acbRead (hf,&cf,sizeof(cf))) {
                  *CanStart = (cf.iFolder != fileFromPrev &&
                                  cf.iFolder != filePrevAndCont);
                  }
               }
            }
         else *CanStart = true;
         }
      }
   acbClose (hf);
   }
return success;
} // --------------------------------------------- /
BOOL         TExpand::Copy (eUnusedFlag f) {
BOOL  success = false;
if (hfdi != NULL) {
   do {
      success = plfCopy (hfdi,m_CabFile,m_CabPath,f,acbNotify,acbDecrypt,this);
      } while (success && mf_getnext() && ! bTerminate);
   }
return success;
} // --------------------------------------------- /
bool         TExpand::mf_getnext () {
bool         success = false;

if (m_SubSequent) {
   tFileHandle hf;
   CHAR        File[sizeof(tPath) *2];

   wsprintf (File,"%s%s",m_CabPath,m_CabFile);
   if ((hf = acbOpen (File,_O_BINARY|_O_RDONLY,_S_IREAD)) != -1) {
      CABHEADER    hd;

      if (acbRead(hf,(void*) &hd,sizeof (CABHEADER)) == sizeof(CABHEADER) &&
                        m_SetID == hd.SetID && CabSignature == hd.Signature) {
         if (hd.Flags & cabHasNext) {
            if (hd.Flags & cabReserve)
               acbSeek (hf,5,SEEK_CUR);

            char buffer[1024];
            if (acbRead (hf,buffer,sizeof(buffer))) {
               PCHAR file = buffer;

               if (hd.Flags & cabHasPrev) {
                  file += strlen (file) +1;
                  file += strlen (file) +1;
                  }

               PCHAR    disk  = file +strlen(file) +1;
               strcpy (m_CabFile,file);
               teError  err   = eeNone;

               do {
                  CBNextCabinet (m_CabFile,disk,m_CabPath,err);

                  if (err == eeNone)
                     success = true;
                  else bTerminate = true;

                  } while (!success && !bTerminate);
               }
            }
         }
      acbClose (hf);
      }
   }
return success;
} // --------------------------------------------- /
void         TExpand::Destroy (void) {
if (hfdi != NULL)
   plfDestroy (hfdi);
hfdi = NULL;
}

// EOF

