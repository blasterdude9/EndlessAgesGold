// CFilePatchClass.cpp : implementation of CFilePatchObject / CFilePatchObjectList
//
// The original patch-builder implementation (binary diff / client update packager)
// was not present in the recovered source tree. These are minimal, inert stubs that
// satisfy the linker and the MFC CObject serialization plumbing (DECLARE_SERIAL)
// so the editor's Patch System dialog is present but performs no patch building.
// Replace with the real algorithm if/when patch-building is needed again.

#include "stdafx.h"
#include "CFilePatchClass.h"

IMPLEMENT_SERIAL( CFilePatchObject, CObject, 1 )
IMPLEMENT_SERIAL( CFilePatchObjectList, CObList, 1 )

//////////////////////////////////////////////////////////////////////////
// CFilePatchObject

CFilePatchObject::CFilePatchObject()
{
    m_patchType            = 0;
    m_newFileSize          = 0;
    m_patchData            = NULL;
    m_patchDataSize        = 0;
    m_patchProgression     = NULL;
    m_progresssionCount    = 0;
    m_progressInternal     = NULL;
    m_versionNumber        = 0;
    m_targetFileSize       = 0;
    m_compressedDataObject = NULL;
}

BOOL CFilePatchObject::WriteVersionFile()                                   { return FALSE; }
void CFilePatchObject::SafeDelete()                                         {}
long CFilePatchObject::GetCurrentProgressionSize()                          { return 0; }
BOOL CFilePatchObject::ApplyPatchToDirectory()                             { return FALSE; }
BOOL CFilePatchObject::ConstructPatch(CString, CString, long)              { return FALSE; }
BOOL CFilePatchObject::UpdateFile(CString)                                  { return FALSE; }
BOOL CFilePatchObject::AddProgression(long, long, long, BOOL, PATCHBLOCK**, long&) { return FALSE; }
BOOL CFilePatchObject::GetDuplicateData(DWORD, BYTE*, DWORD, BYTE*, long)   { return FALSE; }
void CFilePatchObject::RestorePatchFromCompression()                        {}
BOOL CFilePatchObject::ConstructPatch(CString)                              { return FALSE; }
void CFilePatchObject::Serialize( CArchive& ar )                            { CObject::Serialize( ar ); }

//////////////////////////////////////////////////////////////////////////
// CFilePatchObjectList

void    CFilePatchObjectList::SafeDelete()                                  {}
BOOL    CFilePatchObjectList::AddPatch(CString, CString, CString, long)     { return FALSE; }
long    CFilePatchObjectList::GetNextVersionNumber()                        { return 0; }
long    CFilePatchObjectList::GetCurrentVersionNumber()                     { return 0; }
void    CFilePatchObjectList::ApplyAllPatches()                             {}
BOOL    CFilePatchObjectList::WriteVersionFile()                            { return FALSE; }
long    CFilePatchObjectList::GetVersionFromFile()                          { return 0; }
CFilePatchObject * CFilePatchObjectList::GetObjectByVersion(int)            { return NULL; }
BOOL    CFilePatchObjectList::WriteIPDataToDisk(CString)                    { return FALSE; }
CString CFilePatchObjectList::GetIPDataFromDisk()                           { return CString(); }
