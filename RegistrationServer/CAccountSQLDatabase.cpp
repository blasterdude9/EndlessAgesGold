#include "stdafx.h"
#define D3D_OVERLOADS
#include "resource.h"
#include <afxtempl.h>

#include "CAccountSQLDatabase.h"

CAccountSQLObj::CAccountSQLObj(CString username,
							   CString password,
							   CString email)
{
    m_username = username;
    m_password = password;
    m_email    = email;
    m_newUsername;
    m_newPassword;
}

void CAccountSQLObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_username << m_password << m_email << m_newUsername << m_newPassword;
	}
	else
	{
       ar >> m_username >> m_password >> m_email >> m_newUsername >> m_newPassword;
	}
}     

void CAccountSQLDatabase::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CAccountSQLObj * spnPtr = (CAccountSQLObj *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}


IMPLEMENT_SERIAL(CAccountSQLObj, CObject, 0)
IMPLEMENT_SERIAL(CAccountSQLDatabase, CObList, 0)

