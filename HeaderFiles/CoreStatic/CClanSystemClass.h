#include "CAccountClass.h"

#ifndef CCLANSYSTEMCLASS_H
#define CCLANSYSTEMCLASS_H

class   CClanMemberObj : public CObject
{
	
     
	DECLARE_SERIAL( CClanMemberObj );
	
    
public:
    CClanMemberObj();
   
    int               m_memberHandle;
	CString           m_memberName;
   
	void Serialize( CArchive& ar );
	void Clone(CClanMemberObj ** clone);
};

class   CClanMemberList : public CObList
{
public:
	CClanMemberList(){};
	CClanMemberObj * GetByHandle(int handle);
	int RemoveMemberByName(CString membername);
	CClanMemberObj * GetByName(CString & name);
	void Clone(CClanMemberList ** clone);
   void SafeDelete();
    
     
	DECLARE_SERIAL( CClanMemberList );
	
    

};

class   CClanSystemObj : public CObject
{
	
     
	DECLARE_SERIAL( CClanSystemObj );
	
    
public:
    CClanSystemObj();
    
    CString           m_clanName;
 	CString           m_abbrev;
    int               m_guildMasterHandle;
	CClanMemberList  *m_memberList;//member database
	int               m_houseLink;
	int               m_warsWon;//keeps track of wars that are won
	int              *m_clanWarList;
	int               m_clanWarCount;
	int               m_clanArenaPoints;
    
	void SerializeAlloc( CArchive& ar, CRITICAL_SECTION * csection);
	void AddMemberByHandle(int handle,CString memberName);
	void Clone(CClanSystemObj ** clone);
	int RemoveMemberByHandle(int handle);
    void SafeDelete();
	BOOL VerifyClanOnWarList(int clanHandle);
	BOOL AddClanToWarList(int clanHandle);
	BOOL RemoveClanFromWarList(int clanHandle);
	void Serialize( CArchive& ar );	
};

class   CClanSystemObjList : public CObList
{
public:
   CClanSystemObjList(){m_lock = 0;};
   BOOL m_lock;//while locked you cannot delete a clan
   void SafeDelete();
   void Clone(CClanSystemObjList ** clone);
   int CreateClan(CString m_clanFullName,
				  CString m_clanAbbrev,
				  int m_guildMasterHandle);
   CClanSystemObj * GetClanByHandle(int m_guildMasterHandle);
   CClanSystemObj * GetClanByPreFix(CString & prefix);
   BOOL DeleteClanByHandle(int m_guildMasterHandle);
   CString ConvToString(int number);
   BOOL AreTheseClansAtWar(int clanHandle_1,int clanHandle_2);
   BOOL TheseClansHaveAgreedToWar(int clanHandle_1,int clanHandle_2);
   BOOL TheseClansHaveStoppedTheWar(int clanHandle_1,int clanHandle_2);
   void SerializeAlloc( CArchive& ar, CRITICAL_SECTION * csection);

    
     
	DECLARE_SERIAL( CClanSystemObjList );
	
    

};

#endif