#pragma once

#define INFO_BUFFER_SIZE MAX_PATH+4

struct SPackageHeader
{
    char * vDirName;
    long vDirNameLength;
	long dataBlockLength; 
};

class CInstallBuilder
{
public:
	CInstallBuilder(void);
	~CInstallBuilder(void);



    //FUNCTIONS
	BOOL BuildInstaller();
 

};
