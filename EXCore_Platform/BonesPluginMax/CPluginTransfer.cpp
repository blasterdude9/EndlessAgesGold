#include "afxwin.h"
#include "afxext.h"
#include <d3d9.h>
#include "direct.h"
#include "CStructuresMisl.h"
#include "CPluginTransfer.h"

CPluginTransfer::CPluginTransfer()
{
	//m_linkMSG = RegisterWindowMessage("--landmesh export from 3D Studio Max to Aura--");
	m_linkMSG = RegisterWindowMessage("-Aura_FileTarget_System-");
	m_meshesLeft = 0;
	m_dataPtr = NULL;
	m_dataOffset = 0;
	m_memBlockSize = 0;
	m_miscBlock = NULL;
	m_miscSize = 0;
}

BOOL CPluginTransfer::AppendStringSegToBlock(CString stringLoc,short extData)
{
    if(stringLoc.GetLength() < 1)
	   return FALSE;

	DWORD strlen1 = stringLoc.GetLength()+sizeof(DWORD)+sizeof(short);
    BYTE * newData = new BYTE[strlen1];
	CopyMemory(&newData[0],&strlen1,sizeof(DWORD));
	for(int i=0;i<(strlen1-(sizeof(DWORD)+sizeof(short)));i++)
        newData[i+sizeof(DWORD)] = stringLoc.GetAt(i);

	CopyMemory(&newData[strlen1-sizeof(short)],&extData,sizeof(short));

	BOOL res = AppendToBlock(newData,strlen1);
	delete [] newData;
	newData = 0;
	return res;
}

BOOL CPluginTransfer::AppendToBlock(BYTE * newData,DWORD size)
{
    if(!m_miscBlock)
	{
		m_miscBlock = new BYTE[size];
		m_miscSize = size;
		CopyMemory(&m_miscBlock[0],newData,size);
	}
	else{
        BYTE * tempBuffer = new BYTE[size+m_miscSize];
        CopyMemory(&tempBuffer[0],m_miscBlock,m_miscSize);
		CopyMemory(&tempBuffer[m_miscSize],newData,size);
		m_miscSize = m_miscSize+size;
		delete [] m_miscBlock;
		m_miscBlock=0;
		m_miscBlock = tempBuffer;
	}
	return TRUE;
}

BOOL CPluginTransfer::ClearBlock()
{
	if(m_miscBlock)
	   delete [] m_miscBlock;
	   m_miscBlock = 0;

    m_miscBlock = NULL;
	m_miscSize = 0;
	return TRUE;
}

BOOL CPluginTransfer::ReceiveMemory(byte ** dataReturn, DWORD * memSize)
{
	
	HANDLE hData = NULL;
	hData = OpenFileMapping(FILE_MAP_READ, FALSE, "3DStudioMaxToAura-Transfer");

	if(hData==NULL) 
		return FALSE;

	LPVOID data = NULL;
	data = MapViewOfFile(hData,FILE_MAP_READ,0,0,0);
		
	if(data == NULL) 
		return FALSE;

	BYTE *dataByte = (BYTE*)data;
	

	CopyMemory(memSize,dataByte,sizeof(DWORD));
	
	BYTE * newDataCopy = new BYTE[*memSize];
	
	CopyMemory(&newDataCopy[0],&dataByte[sizeof(DWORD)],*memSize);

	*dataReturn = newDataCopy;

	UnmapViewOfFile(data);
	CloseHandle(hData);

	HANDLE auraDone = NULL;
	auraDone = CreateSemaphore( NULL, 0, 1, "-AuraTransferDone-");
	ReleaseSemaphore(auraDone,1,NULL);
	CloseHandle(auraDone);

	return TRUE;
}

BOOL CPluginTransfer::BroadCastData()
{
    return BroadCastData(m_miscBlock, m_miscSize);
}


BOOL CPluginTransfer::BroadCastData(byte * memBlock, DWORD sizeMemBlock)
{
 if(sizeMemBlock < 1)
    return FALSE;


UINT linkMsg = RegisterWindowMessage("-Aura_FileTarget_System-");
	
	HANDLE hMapFile = NULL;

	hMapFile = CreateFileMapping
		(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0,
		sizeMemBlock+100,
		"3DStudioMaxToAura-Transfer"
		);

	if(hMapFile==NULL)
		{
		MessageBox(GetForegroundWindow(),"CreateFileMapping() failed!", "::BroadCastData()",MB_OK);
		return FALSE;
		}
	
	LPVOID data = NULL;
	data = MapViewOfFile(hMapFile,FILE_MAP_WRITE,0,0,0);

	if(data==NULL) 	
		{
		MessageBox(GetForegroundWindow(),"MapViewOfFile() failed!", "::BroadCastData()",MB_OK);
		return FALSE;
		}

	BYTE *dataByte = (BYTE*)data;

	CopyMemory(&dataByte[0], &sizeMemBlock, sizeof(DWORD));
	CopyMemory(&dataByte[sizeof(DWORD)], memBlock, sizeMemBlock);
	
	DWORD wParam=0, lParam=0;//wParam indicates transfer type
	::PostMessage( HWND_BROADCAST, linkMsg, wParam, lParam );

	HANDLE auraDone = NULL;
	auraDone = CreateSemaphore( NULL, 0, 1, "-AuraTransferDone-");
	
	if(WaitForSingleObject(auraDone,3000)==WAIT_TIMEOUT)
	{
		//MessageBox(GetForegroundWindow(),"ERROR: transfer timed out","::memSend()",MB_OK);
	}
	CloseHandle(auraDone);


	UnmapViewOfFile(data);
	CloseHandle(hMapFile);
	return TRUE;
}


BOOL CPluginTransfer::MeshInQueue()
{
	if(m_meshesLeft == 0) 
		return FALSE;
	else
		return TRUE;	
}

BOOL CPluginTransfer::ReceiveMeshExport()
{
	
	HANDLE hData = NULL;
	hData = OpenFileMapping(FILE_MAP_READ, FALSE, "3DStudioMaxToAura-Transfer");

	if(hData==NULL) 
		return FALSE;

	LPVOID data = NULL;
	data = MapViewOfFile(hData,FILE_MAP_READ,0,0,0);
		
	if(data == NULL) 
		return FALSE;

	BYTE *dataByte = (BYTE*)data;

	CopyMemory(&m_meshesLeft, &dataByte[0], sizeof(DWORD));
	CopyMemory(&m_memBlockSize, &dataByte[sizeof(DWORD)], sizeof(DWORD));
	
	if(m_dataPtr)
		{
		delete [] m_dataPtr;
		m_dataPtr = NULL;
		}

	m_dataPtr = new BYTE[m_memBlockSize];
	
	CopyMemory(m_dataPtr, &dataByte[sizeof(DWORD)*2], m_memBlockSize);
	m_dataOffset = 0;

	UnmapViewOfFile(data);
	CloseHandle(hData);

	HANDLE auraDone = NULL;
	auraDone = CreateSemaphore( NULL, 0, 1, "-AuraTransferDone-");
	ReleaseSemaphore(auraDone,1,NULL);
	CloseHandle(auraDone);

	return TRUE;
}

void CPluginTransfer::GetMesh
		(
		BOOL *smoothShading,
		float *sightRange,
		CString &fileName,
		ABVERTEX **vArray,
		WORD **iArray,
		WORD *vCount,
		WORD *iCount,
		CString &texture1,
		CString &texture2,
		CString &texture3,
		CString &texture4,
		CString &texture5,
		CString &texture6,
		CString &texture7,
		CString &texture8
		)
{
	//initialize input variables
	*vArray = NULL;
	*iArray = NULL;
	*vCount = 0;
	*iCount = 0;
	
	CopyMemory(vCount, &m_dataPtr[m_dataOffset], sizeof(WORD));
	m_dataOffset += sizeof(WORD);

	CopyMemory(iCount, &m_dataPtr[m_dataOffset], sizeof(WORD));
	m_dataOffset += sizeof(WORD);

	CopyMemory(smoothShading, &m_dataPtr[m_dataOffset], sizeof(BOOL));
	m_dataOffset += sizeof(BOOL);

	CopyMemory(sightRange, &m_dataPtr[m_dataOffset], sizeof(float));
	m_dataOffset += sizeof(float);

	char fName[256];
	CopyMemory( fName, &m_dataPtr[m_dataOffset], sizeof(char)*256);
	m_dataOffset += sizeof(char)*256;
	fileName = fName;	

	//allocate memory for vArray and iArray
	*vArray = new ABVERTEX[*vCount];
	*iArray = new WORD[*iCount];

	ABVERTEX *newvArray = *vArray;

	int loop = 0;
	for(loop=0; loop < *vCount; loop++)
		{
		CopyMemory(&newvArray[loop].x, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);
	    CopyMemory(&newvArray[loop].y, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);+
		CopyMemory(&newvArray[loop].z, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);
		CopyMemory(&newvArray[loop].nx, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);
		CopyMemory(&newvArray[loop].ny, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);
		CopyMemory(&newvArray[loop].nz, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);
		CopyMemory(&newvArray[loop].tx0.tu, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);
		CopyMemory(&newvArray[loop].tx0.tv, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);
		CopyMemory(&newvArray[loop].tx1.tu, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);
		CopyMemory(&newvArray[loop].tx1.tv, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);
		CopyMemory(&newvArray[loop].tx2.tu, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);
		CopyMemory(&newvArray[loop].tx2.tv, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);
		CopyMemory(&newvArray[loop].tx3.tu, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);
		CopyMemory(&newvArray[loop].tx3.tv, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);
		CopyMemory(&newvArray[loop].tx4.tu, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);
		CopyMemory(&newvArray[loop].tx4.tv, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);
		CopyMemory(&newvArray[loop].tx5.tu, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);
		CopyMemory(&newvArray[loop].tx5.tv, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);
		CopyMemory(&newvArray[loop].tx6.tu, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);
		CopyMemory(&newvArray[loop].tx6.tv, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);
		CopyMemory(&newvArray[loop].tx7.tu, &m_dataPtr[m_dataOffset], sizeof(float));
		m_dataOffset += sizeof(float);
		CopyMemory(&newvArray[loop].tx7.tv, &m_dataPtr[m_dataOffset], sizeof(float));		
		m_dataOffset += sizeof(float);
		}

	WORD *newiArray = *iArray;
	for(loop = 0; loop < *iCount; loop++)
		{
		CopyMemory(&newiArray[loop], &m_dataPtr[m_dataOffset], sizeof(WORD));
		m_dataOffset += sizeof(WORD);
		}

	CString nullTexture = "";
	
	char tmpChar[256];
	int strSize = sizeof(char)*256;
	CopyMemory( tmpChar, &m_dataPtr[m_dataOffset], strSize);
	m_dataOffset += strSize;
	texture1 = tmpChar;
	CopyMemory( tmpChar, &m_dataPtr[m_dataOffset], strSize);
	m_dataOffset += strSize;
	texture2 = tmpChar;
	CopyMemory( tmpChar, &m_dataPtr[m_dataOffset], strSize);
	m_dataOffset += strSize;
	texture3 = tmpChar;
	CopyMemory( tmpChar, &m_dataPtr[m_dataOffset], strSize);
	m_dataOffset += strSize;
	texture4 = tmpChar;
	CopyMemory( tmpChar, &m_dataPtr[m_dataOffset], strSize);
	m_dataOffset += strSize;
	texture5 = tmpChar;
	CopyMemory( tmpChar, &m_dataPtr[m_dataOffset], strSize);
	m_dataOffset += strSize;
	texture6 = tmpChar;
	CopyMemory( tmpChar, &m_dataPtr[m_dataOffset], strSize);
	m_dataOffset += strSize;
	texture7 = tmpChar;
	CopyMemory( tmpChar, &m_dataPtr[m_dataOffset], strSize);
	m_dataOffset += strSize;
	texture8 = tmpChar;

	m_meshesLeft--;

	if(m_meshesLeft <= 0)
		{
		delete [] m_dataPtr;
		m_dataPtr = NULL;
		m_dataOffset = 0;
		}

}
