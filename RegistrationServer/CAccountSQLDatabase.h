#ifndef CACCOUNTSQLCLASS_H
#define CACCOUNTSQLCLASS_H

class CAccountSQLObj : public CObject
{
	DECLARE_SERIAL( CAccountSQLObj );
	#undef AFX_API
    #define AFX_API
public:
	CAccountSQLObj(){};
    CAccountSQLObj(CString username,
							   CString password,
							   CString email);
    
	CString m_username;
	CString m_password;
	CString m_newUsername;
	CString m_newPassword;
	CString m_email;

  
	void Serialize( CArchive& ar );
	
};

class CAccountSQLDatabase : public CObList
{
public:
   CAccountSQLDatabase(){};
   void SafeDelete();
 
   DECLARE_SERIAL( CAccountSQLDatabase );


};

#endif