#ifndef PERSISTCLASS_H
#define PERSISTCLASS_H


class CPersistObj : public CObject
{
	CPersistObj(){}
	
     
	DECLARE_SERIAL( CPersistObj );
	
    
public:
	
	CPersistObj(int             L_entDBIndex,
		        D3DMATRIX       L_entMatrix,
				int             L_entControlType
			   );

	int             List_entControlType;
	int             List_entDBIndex;
	D3DMATRIX       List_entMatrix;            
    
	void Serialize( CArchive& ar );

};

class CPersistObjList : public CObList
{
public:
   CPersistObjList(){};
   void SafeDelete();
    
     
	DECLARE_SERIAL( CPersistObjList );
	
    
};

#endif



