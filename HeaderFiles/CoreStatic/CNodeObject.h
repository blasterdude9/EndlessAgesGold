
#ifndef CNODEOBJECT_H
#define CNODEOBJECT_H

class   CNodeObj : public CObject
{
	
     
	DECLARE_SERIAL( CNodeObj );
	
    
public:
    CNodeObj();

    D3DMATRIX     m_position;//orientation position worldspace
    int           m_libraryReference;//index into a visual library
	ABBOX         m_clipBox;
	void        * m_refPtr;//if null must be aquired using m_libraryReference
	

	void SafeDelete();
	void Serialize( CArchive& ar );
};

class   CNodeObjList : public CObList
{
public:
   CNodeObjList(){};
   void SafeDelete();
    
     
	DECLARE_SERIAL( CNodeObjList );
	
    
};

#endif