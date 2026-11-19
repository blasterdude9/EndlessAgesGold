#include "StdAfx.h"
#include "direct.h"
#include ".\installbuilder.h"

CInstallBuilder::CInstallBuilder(void)
{
}

CInstallBuilder::~CInstallBuilder(void)
{
}

BOOL CInstallBuilder::BuildInstaller()
{
    int fileCount = 2;
	UINT totalFileSize = 0;
	CString cabArray[256];
	cabArray[0] = "installer.exe";
	cabArray[1] = "data.CAB";
	int entryPointWritten = 0;
	for(int sizeloop=0;sizeloop<fileCount;sizeloop++)
	{
		CFileException exc;
		CFile fl;
		BOOL res = fl.Open(cabArray[sizeloop],CFile::modeRead, &exc);
		if(!res)
		{
			AfxMessageBox("cannot find file:"+cabArray[sizeloop]);
            return FALSE;
		}
		
	    UINT length = fl.GetLength();
	    if(entryPointWritten == 1)
		{
		   totalFileSize += sizeof(SPackageHeader) + cabArray[sizeloop].GetLength();
		}
        totalFileSize += length;
        fl.Close();
		entryPointWritten = 1;
	}
	
    char * writedata = new char[totalFileSize+1];
	ZeroMemory(writedata,totalFileSize+1);
	int tracer = 0;
	entryPointWritten = 0;
    for(int cpyLoop=0;cpyLoop<fileCount;cpyLoop++)
	{
		CFileException exc;
		CFile fl;
		BOOL res = fl.Open(cabArray[cpyLoop],CFile::modeRead, &exc);
		UINT length = fl.GetLength();
        char * thisFileData = new char[length];
        fl.Read(thisFileData,length);
        fl.Close();

		if(entryPointWritten == 1)
		{
			SPackageHeader subHeader;
			subHeader.vDirNameLength = cabArray[cpyLoop].GetLength();
			subHeader.dataBlockLength = length;
			subHeader.vDirName = new char[subHeader.vDirNameLength];
			
			for(int chl=0;chl<subHeader.vDirNameLength;chl++)
				subHeader.vDirName[chl] = cabArray[cpyLoop].GetAt(chl);

			CopyMemory(&writedata[tracer],&subHeader,sizeof(SPackageHeader));
			tracer += sizeof(SPackageHeader);

			CopyMemory(&writedata[tracer],subHeader.vDirName,subHeader.vDirNameLength);
			tracer += subHeader.vDirNameLength;
			delete [] subHeader.vDirName;
		    subHeader.vDirName = 0;
		}

        CopyMemory(&writedata[tracer],thisFileData,length);
        tracer += length;
		delete [] thisFileData;
		thisFileData = 0;
		entryPointWritten = 1;
	}

	//create setup.exe
	CString outputFileName = "Setup.exe";
	CFileException exc;
    CFile fl;
    BOOL res = fl.Open(outputFileName,CFile::modeCreate | CFile::modeWrite, &exc);
	fl.Write(writedata,totalFileSize);
	fl.Close();
	return TRUE;
}