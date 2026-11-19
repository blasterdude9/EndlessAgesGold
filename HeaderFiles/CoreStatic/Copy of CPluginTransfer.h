#ifndef CPLUGINTRANSFER_H
#define CPLUGINTRANSFER_H


class  CPluginTransfer 
	{
	public:

	CPluginTransfer();
	~CPluginTransfer() {};

	//FUNCTIONS
	
	BOOL ReceiveMeshExport();

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

	UINT m_linkMSG;

	DWORD m_meshesLeft;
	DWORD m_memBlockSize;

	BYTE *m_dataPtr;
	DWORD m_dataOffset;

	};

#endif