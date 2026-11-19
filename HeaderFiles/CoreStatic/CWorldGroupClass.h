#ifndef CWORLDGROUPOBJ_H
#define CWORLDGROUPOBJ_H


class   CWldGroupObj : public CObject
{
	//CWldGroupObj(){}
	
     
    DECLARE_SERIAL( CWldGroupObj )
    
    
public:
    CWldGroupObj(){}
	CWldGroupObj(CString groupName,int groupInt);
    int List_groupInt;
	CString List_groupName;
	void Serialize( CArchive& ar );
};

class   CWldGroupObjList : public CObList
{
public:
   CWldGroupObjList(){};
    
   void SafeDelete();
    DECLARE_SERIAL( CWldGroupObjList )
    
    
};

#endif