#include "stdafx.h"
#define D3D_OVERLOADS
#include <d3d9.h>
#include <afxtempl.h>
#include "dbconnectionsystem.h"
#include "CElementsClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CElementObj::CElementObj()
{

}

void CElementObj::SafeDelete()
{

}

void CElementObj::Clone(CElementObj ** clone)
{


}

void CElementObjList::Clone(CElementObjList ** clone)
{


}

void CElementObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CElementObj * elementPtr = (CElementObj *)GetNext(posLoc);
		elementPtr->SafeDelete();
		delete elementPtr;
		elementPtr=0;
	}
	RemoveAll();
}

CElementObj * CElementObjList::GetByIndex(int index)
{
	if(index < 0)
	   return NULL;
	POSITION posLoc = FindIndex(index);
	if(!posLoc)
	   return NULL;
	  
       CElementObj * elementPtr = (CElementObj *)GetAt(posLoc);
	   return elementPtr; 
}

CElementObj * CElementObjList::GetByName(CString name)
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CElementObj * elementPtr = (CElementObj *)GetNext(posLoc);
		if(name.CompareNoCase(elementPtr->m_elementName)==0)
		{
			return elementPtr;
		}
	}
	return NULL;
}

int CElementObjList::GetIndexByName(CString name)
{
	int ret = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CElementObj * elementPtr = (CElementObj *)GetNext(posLoc);
		if(name.CompareNoCase(elementPtr->m_elementName)==0)
			return ret;
		ret++;
	}
	return -1;
}

//-------------------------------------------------------------------//
BOOL CElementObjList::ExportToSQL(CString ip, CString database, CString username, CString password)
{	
    CDBConnectionSystem * dbConnection = new CDBConnectionSystem(ip,
			                                                     database,
																 username,
																 password,
																 NULL);


	if(!dbConnection->m_connectionEstablished)
	{
        return FALSE;
	}

    CString clear				= "DELETE FROM GM_ELEMENTS";
    CString addObject           = "INSERT INTO GM_ELEMENTS (id,NAME) Values('$ID','$NAME')";

	CABFunctionLib cablib;
	CString temp;
	dbConnection->ExecQuery(clear);
	
	int trace = 0;
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CElementObj * objPtr  = (CElementObj *)GetNext(posLoc);

		CString id;
	    id.Format("%i",trace);
		
        temp = addObject;
		temp.Replace("$ID",id);
		temp.Replace("$NAME",objPtr->m_elementName);
		dbConnection->ExecQuery(temp);
		trace++;
	}

	delete dbConnection;
	dbConnection=0;
	return TRUE;
}




void CElementObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_elementName;
	}
	else
	{
       ar >> m_elementName;
	}
}     


IMPLEMENT_SERIAL(CElementObj, CObject, 0)
IMPLEMENT_SERIAL(CElementObjList, CObList, 0)

