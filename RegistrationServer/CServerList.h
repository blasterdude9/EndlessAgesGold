#ifndef CSERVERLIST_H
#define CSERVERLIST_H

class CServerListObject : public CObject
{
	DECLARE_SERIAL( CServerListObject );

public:
    CServerListObject();

    CString m_serverName;
	CString m_IPadress;

	//functions
	void SafeDelete();

	void Serialize( CArchive& ar );	
};

class CServerListObjectDB : public CObList
{
public:
	CServerListObjectDB(){};
    void SafeDelete();
  
	DECLARE_SERIAL( CServerListObjectDB );


};

#endif