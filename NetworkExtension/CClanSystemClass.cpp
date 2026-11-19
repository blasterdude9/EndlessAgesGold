#include "stdafx.h"
#define D3D_OVERLOADS

#include <d3d9.h>
#include <afxtempl.h>
#include "direct.h"

#include "CClanSystemClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CClanMemberObj::CClanMemberObj()
{
    m_memberHandle = 0;
}

void CClanMemberObj::Clone(CClanMemberObj ** clone)
{
     CClanMemberObj * copyLocal = new CClanMemberObj();
     copyLocal->m_memberHandle = m_memberHandle;
     copyLocal->m_memberName = m_memberName;

	 *clone = copyLocal;
}


void CClanMemberList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CClanMemberObj * spnPtr = (CClanMemberObj *)GetNext(posLoc);
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

void CClanMemberList::Clone(CClanMemberList ** clone)
{
	CClanMemberList * copyLocal = new CClanMemberList();

	 for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CClanMemberObj * skPtr = (CClanMemberObj *)GetNext(posLoc);
		 CClanMemberObj * clonePtr = NULL;
         skPtr->Clone(&clonePtr);
         copyLocal->AddTail(clonePtr);
	 }

	 *clone = copyLocal;
}

CClanMemberObj * CClanMemberList::GetByHandle(int handle)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CClanMemberObj * spnPtr = (CClanMemberObj *)GetNext(posLoc);
		if(spnPtr->m_memberHandle == handle)
           return spnPtr;
	}
	return NULL;
}

CClanMemberObj * CClanMemberList::GetByName(CString & name)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CClanMemberObj * spnPtr = (CClanMemberObj *)GetNext(posLoc);
		if(spnPtr->m_memberName == name)
           return spnPtr;
	}
	return NULL;
}

int CClanMemberList::RemoveMemberByName(CString membername)
{
	POSITION posLast;
    for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CClanMemberObj * spnPtr = (CClanMemberObj *)GetNext(posLoc);
		if(spnPtr->m_memberName.CompareNoCase(membername)==0)
		{ 
		   int memH = spnPtr->m_memberHandle;
		   delete spnPtr;
		   spnPtr = 0;
           RemoveAt(posLast);
           return memH;
		}
	}
	return 0;
}

void CClanMemberObj::Serialize( CArchive& ar )
{
CObject::Serialize( ar );
	if(ar.IsStoring() ){	
	   ar << m_memberHandle
		  << m_memberName;
	}
	else
	{
       ar >> m_memberHandle
		  >> m_memberName;

	}
}     

CClanSystemObj::CClanSystemObj()
{
    m_clanName;
    m_abbrev;
    m_guildMasterHandle = 0;
    m_memberList = NULL;
	m_houseLink  = -1;
	m_warsWon = 0;
	m_clanWarCount = 0;
	m_clanWarList = NULL;
	m_clanArenaPoints = 0;
}

void CClanSystemObj::Clone(CClanSystemObj ** clone)
{
    CClanSystemObj * copyLocal = new CClanSystemObj();
	copyLocal->m_clanName          = m_clanName;
    copyLocal->m_abbrev            = m_abbrev;
    copyLocal->m_guildMasterHandle = m_guildMasterHandle;
	copyLocal->m_houseLink         = m_houseLink;
	copyLocal->m_warsWon           = m_warsWon;
	copyLocal->m_clanWarCount      = m_clanWarCount;
	copyLocal->m_clanArenaPoints   = m_clanArenaPoints;

	if(m_clanWarCount > 0)
	if(copyLocal->m_clanWarList)
	{
       copyLocal->m_clanWarList = new int[m_clanWarCount];
	   CopyMemory(copyLocal->m_clanWarList,m_clanWarList,sizeof(int)*m_clanWarCount);

	}

    if(m_memberList)
	   m_memberList->Clone(&copyLocal->m_memberList);

	*clone = copyLocal;
}

void CClanSystemObj::SerializeAlloc( CArchive& ar, CRITICAL_SECTION * csection)
{





}


BOOL CClanSystemObj::VerifyClanOnWarList(int clanHandle)
{
  if(!m_clanWarList)
	  return FALSE;

    for(int i=0;i<m_clanWarCount;i++)
	    if(m_clanWarList[i] == clanHandle)
		   return TRUE;

	return FALSE;
}


BOOL CClanSystemObj::AddClanToWarList(int clanHandle)
{

  if(VerifyClanOnWarList(clanHandle))
	  return FALSE;

  m_clanWarCount++;
  int * newList = new int[m_clanWarCount];
  
  if(m_clanWarList){
	 if(m_clanWarCount > 1)
     CopyMemory(newList,m_clanWarList,(sizeof(int)*(m_clanWarCount-1)));
     delete [] m_clanWarList;
	 m_clanWarList = 0;
  }

  newList[(m_clanWarCount-1)] = clanHandle;
  m_clanWarList = newList;

  return TRUE;
}

BOOL CClanSystemObj::RemoveClanFromWarList(int clanHandle)
{

  if(!VerifyClanOnWarList(clanHandle))//not there
	  return FALSE;

  if(!m_clanWarList)
  {
      m_clanWarCount = 0;
      return FALSE;
  }
  
  int oldCount = m_clanWarCount;
  if(m_clanWarCount > 0){
	 m_clanWarCount--;
     if(m_clanWarCount == 0){
		delete [] m_clanWarList;
		m_clanWarList = 0;
		return TRUE;
	 }

	 int * newList = new int[m_clanWarCount];
	 int trace = 0;
	 for(int i=0;i<oldCount;i++)
	 {
         if(m_clanWarList[i] != clanHandle)
		 {
		    newList[trace] = m_clanWarList[i];
	        trace++;
		 }
	 }
	 delete [] m_clanWarList;
     m_clanWarList = 0;
	 m_clanWarList = newList;
	 return TRUE;//removed (used to fall through and report failure)
  }

  return FALSE;
}

void CClanSystemObj::SafeDelete()
{
    if(m_memberList)
	{
       m_memberList->SafeDelete();
	   delete m_memberList;
	   m_memberList = 0;
	}
	if(m_clanWarList)
	{
       delete [] m_clanWarList;
	   m_clanWarList = 0;
	}
}

void CClanSystemObj::AddMemberByHandle(int handle,CString memberName)
{
   if(!m_memberList)
	   m_memberList = new CClanMemberList();

   
   if(!m_memberList->GetByHandle(handle))
   {//doesnt already exist
      CClanMemberObj * newMember = new CClanMemberObj();
      newMember->m_memberHandle = handle;
	  newMember->m_memberName = memberName;
      m_memberList->AddTail(newMember);
   }//end doesnt already exist
}

int CClanSystemObj::RemoveMemberByHandle(int handle)
{
   if(!m_memberList)
	   return -1;

   POSITION posLast = NULL;
   for(POSITION posLoc = m_memberList->GetHeadPosition();(posLast = posLoc) != NULL;)
   {
        CClanMemberObj * spnPtr = (CClanMemberObj *)m_memberList->GetNext(posLoc);
		if(spnPtr->m_memberHandle == handle)
		{
           delete spnPtr;
		   spnPtr = 0;
           m_memberList->RemoveAt(posLast);
		   return handle;
		}
   }
   return -1;
}

void CClanSystemObjList::SafeDelete()
{
	for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CClanSystemObj * spnPtr = (CClanSystemObj *)GetNext(posLoc);
		spnPtr->SafeDelete();
		delete spnPtr;
		spnPtr=0;
	}
	RemoveAll();
}

int CClanSystemObjList::CreateClan(CString m_clanFullName,
								   CString m_clanAbbrev,
								   int m_guildMasterHandle)
{
	if(m_lock)
	   return FALSE;
    //lengths
	if(m_clanAbbrev.GetLength() > 15)
       return 3;//too long

	if(m_clanAbbrev.GetLength() <= 0)
       return 3;//too long
    //verify clan with this handle doesnt exists
    if(GetClanByHandle(m_guildMasterHandle))
       return 0;//clan already exists
    //verify name differences
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CClanSystemObj * spnPtr = (CClanSystemObj *)GetNext(posLoc);
		if(spnPtr->m_abbrev == m_clanAbbrev)
           return 2;//already exists
	}


	//create it
	CClanSystemObj * clanObject     = new CClanSystemObj();
    clanObject->m_abbrev            = m_clanAbbrev;
    clanObject->m_clanName          = m_clanFullName;
	clanObject->m_guildMasterHandle = m_guildMasterHandle;
    clanObject->m_memberList        = new CClanMemberList();
  
	AddTail(clanObject);
	return 1;//created successfully
}

CClanSystemObj * CClanSystemObjList::GetClanByHandle(int m_guildMasterHandle)
{
    if(m_guildMasterHandle == -1)
	   return NULL;


    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CClanSystemObj * spnPtr = (CClanSystemObj *)GetNext(posLoc);
		if(spnPtr->m_guildMasterHandle == m_guildMasterHandle)
           return spnPtr;
	}
	return NULL;
}

CClanSystemObj * CClanSystemObjList::GetClanByPreFix(CString & prefix)
{
    for(POSITION posLoc = GetHeadPosition();posLoc != NULL;){
        CClanSystemObj * spnPtr = (CClanSystemObj *)GetNext(posLoc);
		if(spnPtr->m_abbrev.CompareNoCase(prefix)==0)
           return spnPtr;
	}
	return NULL;
}



BOOL CClanSystemObjList::DeleteClanByHandle(int m_guildMasterHandle)
{
    if(m_lock)
	   return FALSE;

	POSITION posLast;
    for(POSITION posLoc = GetHeadPosition();(posLast = posLoc) != NULL;){
        CClanSystemObj * spnPtr = (CClanSystemObj *)GetNext(posLoc);
		if(spnPtr->m_guildMasterHandle == m_guildMasterHandle)
		{
           spnPtr->SafeDelete();
		   delete spnPtr;
		   spnPtr = 0;
           RemoveAt(posLast);
           return TRUE;
		}
	}
	return FALSE;
}

//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----converts a long into string-----------------------------//
CString CClanSystemObjList::ConvToString(int number)
{    
	          CString temp;
			  temp.Format("%i",number);
			  return temp;
}


//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
BOOL CClanSystemObjList::AreTheseClansAtWar(int clanHandle_1,int clanHandle_2)
{    
     for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CClanSystemObj * skPtr = (CClanSystemObj *)GetNext(posLoc);
		 if(skPtr->m_guildMasterHandle == clanHandle_1)
		 {

            if(skPtr->VerifyClanOnWarList(clanHandle_2))
			   return TRUE;
			else
			   return FALSE;
		 }
	 }  
	 return FALSE;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
BOOL CClanSystemObjList::TheseClansHaveAgreedToWar(int clanHandle_1,int clanHandle_2)
{    
	 int added = 0;
     for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CClanSystemObj * skPtr = (CClanSystemObj *)GetNext(posLoc);
		 if(skPtr->m_guildMasterHandle == clanHandle_1)
		 {
            if(skPtr->AddClanToWarList(clanHandle_2))
			   added++;
		 }
		 if(skPtr->m_guildMasterHandle == clanHandle_2)
		 {
            if(skPtr->AddClanToWarList(clanHandle_1))
		       added++;
		 }
	 }  
     if(added == 2)
	    return TRUE;

	 return FALSE;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
BOOL CClanSystemObjList::TheseClansHaveStoppedTheWar(int clanHandle_1,int clanHandle_2)
{    
	int added = 0;
     for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CClanSystemObj * skPtr = (CClanSystemObj *)GetNext(posLoc);
		 if(skPtr->m_guildMasterHandle == clanHandle_1)
		 {
            skPtr->RemoveClanFromWarList(clanHandle_2);
			added++;
		 }
		 if(skPtr->m_guildMasterHandle == clanHandle_2)
		 {
            skPtr->RemoveClanFromWarList(clanHandle_1);
			added++;
		 }
	 }  

	 if(added == 2)
	    return TRUE;

	 return FALSE;
}

void CClanSystemObjList::Clone(CClanSystemObjList ** clone)
{
    CClanSystemObjList * copyLocal = new CClanSystemObjList();

	 for(POSITION posLoc = GetHeadPosition();posLoc!=NULL;){//account list
         CClanSystemObj * skPtr = (CClanSystemObj *)GetNext(posLoc);
		 CClanSystemObj * clonePtr = NULL;
         skPtr->Clone(&clonePtr);
         copyLocal->AddTail(clonePtr);
	 }

	 *clone = copyLocal;
}

void CClanSystemObjList::SerializeAlloc( CArchive& ar, CRITICAL_SECTION * csection)
{
	EnterCriticalSection(csection);
	m_lock = TRUE;
	LeaveCriticalSection(csection);
	int version = 0;
    if(ar.IsStoring() ){	

	   ar << version;
	   int count = (int)GetCount();
	   ar << count;
	   

	   int trace = 0;
	   //EnterCriticalSection(csection);
       for(POSITION posLoc = GetHeadPosition(); posLoc != NULL;)
	   {
		   //LeaveCriticalSection(csection);
		   //Sleep(0);
		   //EnterCriticalSection(csection);
           CClanSystemObj * cObj = (CClanSystemObj *)GetNext(posLoc);
		   ar << cObj;
           if(trace == count)
		      break;
		   trace++;
	   }
	   //LeaveCriticalSection(csection);
	}
	else
	{
	   EnterCriticalSection(csection);
	   ar >> version;
	   int count = 0;
	   ar >> count;

       for(int loop=0;loop<count;loop++)
	   {
           CClanSystemObj * cObj = NULL;
		   
           ar >> cObj;
		   AddTail(cObj);
		   
	   }
       LeaveCriticalSection(csection);
	}
	EnterCriticalSection(csection);
	m_lock = FALSE;
	LeaveCriticalSection(csection);
}

void CClanSystemObj::Serialize( CArchive& ar )
{
int reserved = 0;
CObject::Serialize( ar );
	if(ar.IsStoring() ){	

       ar.SetObjectSchema(2);
       ASSERT(ar.GetObjectSchema() == 2);


	   ar << m_memberList
		  << m_clanName
		  << m_abbrev
		  << m_guildMasterHandle
		  << m_houseLink;

	   ar << m_clanArenaPoints
		  << reserved
		  << reserved;
	}
	else
	{
  
       int version = ar.GetObjectSchema();

       ar >> m_memberList
		  >> m_clanName
		  >> m_abbrev
		  >> m_guildMasterHandle
		  >> m_houseLink;

	   if(version > 1)
	   {
          ar >> m_clanArenaPoints
		     >> reserved
		     >> reserved;
	   }

	   //m_houseLink = -1;
	   m_warsWon = 0;
	   m_clanWarList = NULL;
	   m_clanWarCount = 0;
	   //m_clanArenaPoints = 0;
	}
}     

IMPLEMENT_SERIAL(CClanMemberObj, CObject, 0)
IMPLEMENT_SERIAL(CClanMemberList, CObList, 0)
IMPLEMENT_SERIAL(CClanSystemObj, CObject, VERSIONABLE_SCHEMA|2)
IMPLEMENT_SERIAL(CClanSystemObjList, CObList, 0)
