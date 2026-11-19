#pragma once
#include <afxinet.h>
class CUrlReader
{
public:
	CUrlReader(void);
	~CUrlReader(void);



	//functions
	int ReadUrl(LPSTR url, CString & returnString);
};
