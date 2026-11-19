#ifndef CABMsgObjCLASS_H
#define CABMsgObjCLASS_H


class AFX_EXT_CLASS CABMsgObj : public CObject
{

public:
	CABMsgObj(){};
    
	CString m_miscMsg;
 
	void TestFunction(CWnd * winPtr);

	int test;
};

#endif