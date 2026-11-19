#pragma once

class  CAXTextFormatter
{
public:
	CAXTextFormatter(void);
	~CAXTextFormatter(void);




	//Functions
	BOOL ReplaceNoCaseWordsE(CString & current,CString find, CString replace);
	void ReplaceNoCaseWordsALL(CString & current,CString find, CString replace);
	BOOL ValidateBreakBasis(char & chr);
};
