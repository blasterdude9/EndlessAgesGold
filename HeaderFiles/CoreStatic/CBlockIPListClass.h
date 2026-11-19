#ifndef CBLOCKIPCLASS_H
#define CBLOCKIPCLASS_H

class  CBlockIPObj : public CObject
{
	
     
	DECLARE_SERIAL( CBlockIPObj );
	
    
public:
    CBlockIPObj();
    
    CString m_ipAdress;

  
	void Serialize( CArchive& ar );
	
};

class  CBlockIPList : public CObList
{
public:
	CBlockIPList(){};
   
    
     
	DECLARE_SERIAL( CBlockIPList );
	
    

	void SafeDelete();
	BOOL IsThisIPBlocked(CString ip);
	void AddIpToBlockList(CString ip);
	void UnblockIP(CString ip);
};

#endif