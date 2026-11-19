#ifndef CPLUGINTRANSFER_H
#define CPLUGINTRANSFER_H


class   CPluginTransfer 
	{
	public:

	CPluginTransfer();
	~CPluginTransfer() {};

	BYTE * m_miscBlock;
	DWORD m_miscSize;
	UINT m_linkMSG;
	//FUNCTIONS
	
	BOOL ReceiveMeshExport();
	BOOL BroadCastData(byte * memBlock, DWORD sizeMemBlock);
	BOOL ReceiveMemory(byte ** dataReturn, DWORD * memSize);
	BOOL ClearBlock();
	BOOL AppendToBlock(BYTE * newData,DWORD size);
	BOOL AppendStringSegToBlock(CString stringLoc,short extData);
	BOOL BroadCastData();

	void GetMesh
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
		);

	BOOL MeshInQueue();

	//VARIABLES

	//UINT m_linkMSG;

	DWORD m_meshesLeft;
	DWORD m_memBlockSize;

	BYTE *m_dataPtr;
	DWORD m_dataOffset;

	};

#endif