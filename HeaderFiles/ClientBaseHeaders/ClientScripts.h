#pragma once

class CClientScripts
{
public:
	CClientScripts(void);
	~CClientScripts(void);

	CString m_params[100];

	//Functions
	bool ScriptProcessor(CString script, void * gStatusMem, CString & error, CString miscText);
	bool GetNextFunctionData(int & currentPosition,int & scriptLength, char * cArray, CString & cmd, CString & paramsBlock);
	bool DoFunction(CString & command, CString & params,void * memoryObj,CString & miscText);
	bool FillParams(CString & paramBlock);
};

