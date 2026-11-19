#include "CCompressionClass.h"

#ifndef CFILEPATCHCLASS_H
#define CFILEPATCHCLASS_H

typedef struct {
	long m_srcStart;
	long m_destStart;
	long m_length;
    BOOL m_fromExistingData;
} PATCHBLOCK, *LPPATCHBLOCK;

class  CFilePatchObject : public CObject
{
	
     
	DECLARE_SERIAL( CFilePatchObject );
	
    
public:
    CFilePatchObject();

	//CABFunctionLib libLink;
    
	unsigned char    m_patchType;//0=new file compressed   1=patch compressed built from existing data
	CString          m_fileTarget;
	long             m_newFileSize;
	BYTE         *   m_patchData;
	long             m_patchDataSize;
	PATCHBLOCK   *   m_patchProgression;
	long             m_progresssionCount;
	CDialog      *   m_progressInternal;
	int              m_versionNumber;
	long             m_targetFileSize;
	CCompressedObj * m_compressedDataObject;

	//functions
	BOOL WriteVersionFile();
	void SafeDelete();
	long GetCurrentProgressionSize();
	BOOL ApplyPatchToDirectory();
	BOOL ConstructPatch(CString oldFileName,CString newFileName,long maxSearch);
	BOOL UpdateFile(CString fileTarget);
	BOOL AddProgression(long srcStart,
	                                  long destStart,
	                                  long length,
                                      BOOL fromExistingData,
									  PATCHBLOCK ** progressionArray,
									  long & progresionArrayCount);
	BOOL GetDuplicateData(DWORD  oldFileLengthInBytes,
										BYTE * oldFileRawData,
										DWORD newFileLengthInBytes,
										BYTE * newFileRawData,
										long maxSearch);
	void RestorePatchFromCompression();

	BOOL ConstructPatch(CString newFileName);
	
	
	
	void Serialize( CArchive& ar );	
};

class  CFilePatchObjectList : public CObList
{
public:
	CFilePatchObjectList(){};
   void SafeDelete();
    
     
	DECLARE_SERIAL( CFilePatchObjectList );
	
    

	BOOL AddPatch(CString directory,CString newerFileName,CString pathToOldFile,long maxSearch);
	long GetNextVersionNumber();
	long GetCurrentVersionNumber();
	void ApplyAllPatches();
	BOOL WriteVersionFile();
	long GetVersionFromFile();
	CFilePatchObject * GetObjectByVersion(int version);
	BOOL WriteIPDataToDisk(CString ipAdress);
	CString GetIPDataFromDisk();
};

#endif