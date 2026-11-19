#ifndef CCOMMGROUPCLASS_H
#define CCOMMGROUPCLASS_H

class  CCommGroupObj : public CObject
{
	
     
	DECLARE_SERIAL( CCommGroupObj );
	
    
public:
    CCommGroupObj();
    
	CString  m_name;
	int      m_handle;


	void Clone(CCommGroupObj ** clone);
	void Serialize( CArchive& ar );
	void SerializeAlloc( CArchive& ar );
};

class  CCommGroupList : public CObList
{
public:
   CCommGroupList(){};
   
    
     
	DECLARE_SERIAL( CCommGroupList );
	
    

	void SafeDelete();
	void RemoveByHandle(int handle);
	BOOL AddIndividualToGroup(int handle,CString name);
	void Clone(CCommGroupList ** clone);
	void SerializeAlloc(CArchive& ar,int loadCount);

};

#endif