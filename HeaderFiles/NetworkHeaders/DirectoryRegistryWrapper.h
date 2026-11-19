#pragma once

class CDirectoryRegistryWrapper
{
public:
	CDirectoryRegistryWrapper(void);
	~CDirectoryRegistryWrapper(void);



	//Functions
	void MakeDirectory(LPSTR path);
	DWORD64 GetFreeSpaceMB(LPSTR path);
	void SetSoftwareReg(LPSTR headerX,LPSTR valueName,LPSTR valueDataX);
	CString GetSoftwareRegkeyVal(LPSTR headerX,LPSTR valueRequest);
	CString GetSoftwareRegkeyValDef(LPSTR headerX,LPSTR valueRequest,LPSTR root);
	BOOL ValidateRegistryEntry(LPSTR headerX);


};
