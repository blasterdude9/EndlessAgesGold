#ifndef CTRIGGERCLASS_H
#define CTRIGGERCLASS_H


class   CTriggerObject : public CObject
{
	CTriggerObject(){}
	
     
    DECLARE_SERIAL( CTriggerObject )
    
    
public:
	
	CTriggerObject(int action,
		           int miscInt,
				   float radius,
				   CString miscTxt);
    int        m_action;//1 = activate animation
	int        m_indexMisc;
	float      m_radius;//activation radius
	CString    m_miscTxt;
	D3DVECTOR  m_miscVector;
	BOOL       m_oneTimeOnly;
	BOOL       m_everActuated;
	

	void Serialize( CArchive& ar );

};

class   CTriggerObjectList : public CObList
{
public:
   CTriggerObjectList(){};

   void SafeDelete();
   
    
   DECLARE_SERIAL( CTriggerObjectList )
   
   
};

#endif