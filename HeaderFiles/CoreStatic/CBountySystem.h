#ifndef CBOUNTYOBJCLASS_H
#define CBOUNTYOBJCLASS_H

class  CBountyObj : public CObject
{ 
	DECLARE_SERIAL( CBountyObj );
	
public:
    CBountyObj();

    CString m_playerName;
	int m_bountyAmount;
	int m_playerHandle;
    
	void Serialize( CArchive& ar );
};

class  CBountyObjList : public CObList
{
public:
	CBountyObjList(){};
   void SafeDelete();
    
   int AddBounty(CString & playerName, 
							   int & bountyAmount,
							   int & playerHandle,
							   int maxBountyCount);
   void RemoveBountyByHandle(int mHandle);
     
   DECLARE_SERIAL( CBountyObjList );
};

#endif