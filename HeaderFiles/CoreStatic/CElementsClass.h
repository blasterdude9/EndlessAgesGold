#ifndef CELEMENTSCLASS_H
#define CELEMENTSCLASS_H

class  CElementObj : public CObject
{
	
     
	DECLARE_SERIAL( CElementObj );
	
    
public:
    CElementObj();

	CString             m_elementName;

	void SafeDelete();
	void Clone(CElementObj ** clone);

	void Serialize( CArchive& ar );
};

class  CElementObjList : public CObList
{
public:
   CElementObjList(){};
   void SafeDelete();
    
     
	DECLARE_SERIAL( CElementObjList );
	
    
    CElementObj * GetByIndex(int index);
	void Clone(CElementObjList ** clone);
	CElementObj * GetByName(CString name);
	int GetIndexByName(CString name);
	BOOL ExportToSQL(CString ip, CString database, CString username, CString password);
};

#endif