#ifndef CRESOURCECLASS_H
#define CRESOURCECLASS_H

class CResourceObject : public CObject
{
	
     
	DECLARE_SERIAL( CResourceObject );
	
    
public:
	
	CResourceObject();
	
	int     m_type;
	int     m_amount;
	CString m_resourceName;
    BOOL    m_enableMaxAmountVar;
	int     m_maxAmount;
	int     m_regenRatePerSecond;
	BOOL    m_regenEnable;
	long    m_lastUpdatedTime;

	void        Clone(CResourceObject ** copy);
	void        Serialize( CArchive& ar );

};

class CResourceObjectList : public CObList
{
public:
   CResourceObjectList(){};
   void Clone(CResourceObjectList ** copy);
   int  AddResource(CString resName, 
			        int type, 
				    int amount,
				    BOOL enableMaxVar,
				    int maxAmount,
				    BOOL regenEnable,
				    int regenRatePersecond
				    );
   CResourceObject* GetByType(int type);
   void RegenerativeCallback(int currentTickCount);
   BOOL UseResource(int type, int amount);
   BOOL VerifyResource(int type, int amount);
   void SafeDelete();
    
     
	DECLARE_SERIAL( CResourceObjectList );
	
    
};

#endif