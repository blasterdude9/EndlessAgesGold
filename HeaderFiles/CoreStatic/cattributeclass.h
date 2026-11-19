#ifndef CATRIBUTECLASS_H
#define CATRIBUTECLASS_H


//object attributes for animation managment
class CAttributeObj : public CObject
{
	CAttributeObj(){}
	
     
    DECLARE_SERIAL( CAttributeObj )
    
    
public:
	
	CAttributeObj(
		         int L_selfAttributeType,
				 int L_selfAttributeValue,
				 int L_otherAttributeType,
				 int L_otherAttributeValue,
				 int L_otherObjectSelect,
				 BOOL L_vicinityBased,
				 BOOL L_autoRetract,
				 BOOL L_inUse,
				 int L_retractDelay,
				 int L_retractDelayVarial
		  	     );


	             int List_retractDelay;
				 int List_retractDelayVarial;
	             BOOL List_autoRetract;
				 BOOL List_inUse;
	             BOOL List_vicinityBased;
                 int List_selfAttributeType;
				 int List_selfAttributeValue;
				 int List_otherAttributeType;
				 int List_otherAttributeValue;
				 int List_otherObjectSelect;
    
	void Serialize( CArchive& ar );

};

class CAttributeObjList : public CObList
{
public:
   CAttributeObjList(){};
    
     
    DECLARE_SERIAL( CAttributeObjList )
    
    
};

#endif