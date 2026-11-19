// AIRaidDbClass.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "direct.h"
#include "AIRaidDbClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAIRaidDbClass dialog


CAIRaidDbClass::CAIRaidDbClass(CWnd* pParent /*=NULL*/)
	: CDialog(CAIRaidDbClass::IDD, pParent)
	, m_raidPercent(0)
{
	//{{AFX_DATA_INIT(CAIRaidDbClass)
	m_aiCfg = 0;
	m_bonusPoints = 0.0f;
	m_manditoryWait = 0;
	m_notificationPosX = 0.0f;
	m_notificationPosY = 0.0f;
	m_notificationPosZ = 0.0f;
	m_notificationRadius = 0.0f;
	m_notificationWldID = 0;
	m_posX = 0.0f;
	m_posY = 0.0f;
	m_posZ = 0.0f;
	m_raidDuration = 0;
	m_raidName = _T("");
	m_raidWarningString = _T("");
	m_worldID = 0;
	//}}AFX_DATA_INIT
}


void CAIRaidDbClass::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAIRaidDbClass)
	DDX_Control(pDX, IDC_LIST_RaidDB, m_raidList);
	DDX_Control(pDX, IDC_LIST_SpawnPointDB, m_spawnPointList);
	DDX_Control(pDX, IDC_LIST_AIcfgDatabase, m_aiCfgList);
	DDX_Text(pDX, IDC_EDIT_AiCfg, m_aiCfg);
	DDX_Text(pDX, IDC_EDIT_bonusPoints, m_bonusPoints);
	DDX_Text(pDX, IDC_EDIT_manditoryWait, m_manditoryWait);
	DDX_Text(pDX, IDC_EDIT_notificationPosX, m_notificationPosX);
	DDX_Text(pDX, IDC_EDIT_notificationPosY, m_notificationPosY);
	DDX_Text(pDX, IDC_EDIT_notificationPosZ, m_notificationPosZ);
	DDX_Text(pDX, IDC_EDIT_notificationRadius, m_notificationRadius);
	DDX_Text(pDX, IDC_EDIT_notificationWldID, m_notificationWldID);
	DDX_Text(pDX, IDC_EDIT_POSX, m_posX);
	DDX_Text(pDX, IDC_EDIT_POSY, m_posY);
	DDX_Text(pDX, IDC_EDIT_POSZ, m_posZ);
	DDX_Text(pDX, IDC_EDIT_RaidDuration, m_raidDuration);
	DDX_Text(pDX, IDC_EDIT_RaidName, m_raidName);
	DDX_Text(pDX, IDC_EDIT_RaidWarningString, m_raidWarningString);
	DDX_Text(pDX, IDC_EDIT_WorldID, m_worldID);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_PercentSpawn, m_raidPercent);
}


BEGIN_MESSAGE_MAP(CAIRaidDbClass, CDialog)
	//{{AFX_MSG_MAP(CAIRaidDbClass)
	ON_BN_CLICKED(IDC_BUTTON_AddRaid, OnBUTTONAddRaid)
	ON_BN_CLICKED(IDC_BUTTON_AddSpawnPoint, OnBUTTONAddSpawnPoint)
	ON_BN_CLICKED(IDC_BUTTON_DeleteRaid, OnBUTTONDeleteRaid)
	ON_BN_CLICKED(IDC_BTN_Save, OnBTNSave)
	ON_BN_CLICKED(IDC_BTN_LoadDB, OnBTNLoadDB)
	ON_BN_CLICKED(IDC_BUTTON_AddCfgOption, OnBUTTONAddCfgOption)
	ON_LBN_SELCHANGE(IDC_LIST_RaidDB, OnSelchangeLISTRaidDB)
	ON_LBN_SELCHANGE(IDC_LIST_SpawnPointDB, OnSelchangeLISTSpawnPointDB)
	ON_LBN_SELCHANGE(IDC_LIST_AIcfgDatabase, OnSelchangeLISTAIcfgDatabase)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceRaid, OnBUTTONReplaceRaid)
	ON_BN_CLICKED(IDC_BUTTON_DeleteSpawnPoint, OnBUTTONDeleteSpawnPoint)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceSpawnPoint, OnBUTTONReplaceSpawnPoint)
	ON_BN_CLICKED(IDC_BUTTON_DelCfgOption, OnBUTTONDelCfgOption)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LoadUnitAI, OnBnClickedButtonLoadunitai)
	ON_BN_CLICKED(IDC_BUTTON_SaveUnitAI, OnBnClickedButtonSaveunitai)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAIRaidDbClass message handlers

BOOL CAIRaidDbClass::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InListRaids(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAIRaidDbClass::InListRaids(int sel)
{
    m_raidList.ResetContent();
	for(POSITION posLoc = m_aiRaidRefList->GetHeadPosition();posLoc != NULL;)
	{
        CAIRaidObj * aiRaidPtr = (CAIRaidObj *)m_aiRaidRefList->GetNext(posLoc);
		m_raidList.AddString(aiRaidPtr->m_raidName);


	}

	if(sel > -1)
	   m_raidList.SetCurSel(sel);
}

void CAIRaidDbClass::InListSpawnPoints(int sel)
{
	m_spawnPointList.ResetContent();
    int raidSelection = m_raidList.GetCurSel();
	if(raidSelection == -1)
	   return;

	POSITION posLoc = m_aiRaidRefList->FindIndex(raidSelection);
	if(!posLoc)
		return;

	    CAIRaidObj * aiRaidPtr = (CAIRaidObj *)m_aiRaidRefList->GetAt(posLoc);
		if(aiRaidPtr->m_spawnPoints)
	       for(POSITION posLoc = aiRaidPtr->m_spawnPoints->GetHeadPosition();posLoc != NULL;)
		   {
               CAISpawnPoint * spawnPtr = (CAISpawnPoint *)aiRaidPtr->m_spawnPoints->GetNext(posLoc);
		       m_spawnPointList.AddString("SpawnPoint");
		   }

    if(sel > -1)
	   m_spawnPointList.SetCurSel(sel);

}

void CAIRaidDbClass::InListAiCfOptions(int sel)
{
     m_aiCfgList.ResetContent();
	 int raidSelection = m_raidList.GetCurSel();
	 if(raidSelection == -1)
	    return;

	 int spawnPointSelection = m_spawnPointList.GetCurSel();
	 if(spawnPointSelection == -1)
	    return;

	POSITION posLoc = m_aiRaidRefList->FindIndex(raidSelection);
	if(!posLoc)
		return;

	    CAIRaidObj * aiRaidPtr = (CAIRaidObj *)m_aiRaidRefList->GetAt(posLoc);
		if(!aiRaidPtr->m_spawnPoints)
			return;

		posLoc = aiRaidPtr->m_spawnPoints->FindIndex(spawnPointSelection);
	    if(!posLoc)
		    return;

	        CAISpawnPoint * spawnPoint = (CAISpawnPoint *)aiRaidPtr->m_spawnPoints->GetAt(posLoc);

			if(spawnPoint->m_cfgList)
			for(POSITION posLoc = spawnPoint->m_cfgList->GetHeadPosition();posLoc != NULL;)
			{
               CAICfgOption * cfgPtr = (CAICfgOption *)spawnPoint->m_cfgList->GetNext(posLoc);
		       m_aiCfgList.AddString("Cfg");
			}

    if(sel > -1)
	   m_aiCfgList.SetCurSel(sel);

}

void CAIRaidDbClass::OnBUTTONAddRaid() 
{
	CAIRaidObj * aiRaidPtr = new CAIRaidObj();

	UpdateData(TRUE);
	aiRaidPtr->m_raidName            = m_raidName;
	aiRaidPtr->m_raidDuration        = m_raidDuration;
	aiRaidPtr->m_bonusPoints         = m_bonusPoints;
	aiRaidPtr->m_raidAlertWldID      = m_notificationWldID;
	aiRaidPtr->m_raidAlertPosition.x = m_notificationPosX;
	aiRaidPtr->m_raidAlertPosition.y = m_notificationPosY;
	aiRaidPtr->m_raidAlertPosition.z = m_notificationPosZ;
	aiRaidPtr->m_raidAlertRadius     = m_notificationRadius;
	aiRaidPtr->m_raidAlertMessage    = m_raidWarningString;
	aiRaidPtr->m_raidManditoryWait   = m_manditoryWait;
	aiRaidPtr->m_percentSpawn        = m_raidPercent;

	UpdateData(FALSE);

	m_aiRaidRefList->AddTail(aiRaidPtr);
	InListRaids(m_aiRaidRefList->GetCount()-1);
}

void CAIRaidDbClass::OnBUTTONAddSpawnPoint() 
{
    int raidSelection = m_raidList.GetCurSel();
	if(raidSelection == -1){
	    MessageBox("Must select a raid object");
		return;
	}

	POSITION posLoc = m_aiRaidRefList->FindIndex(raidSelection);
	if(!posLoc)
		return;

	    CAIRaidObj * aiRaidPtr = (CAIRaidObj *)m_aiRaidRefList->GetAt(posLoc);
		if(!aiRaidPtr->m_spawnPoints)
			aiRaidPtr->m_spawnPoints = new CAISpawnPointList();


		    UpdateData(TRUE);

            CAISpawnPoint * newSpawnPoint = new CAISpawnPoint();
            newSpawnPoint->m_position.x = m_posX; 
			newSpawnPoint->m_position.y = m_posY; 
			newSpawnPoint->m_position.z = m_posZ; 
			newSpawnPoint->m_worldID    = m_worldID;

			UpdateData(FALSE);

			aiRaidPtr->m_spawnPoints->AddTail(newSpawnPoint);
			InListSpawnPoints(aiRaidPtr->m_spawnPoints->GetCount()-1);
	
}

void CAIRaidDbClass::OnBUTTONDeleteRaid() 
{
	int raidSelection = m_raidList.GetCurSel();
	if(raidSelection == -1)
	{
		MessageBox("Must select a raid object");
		return;
	}

	POSITION posLoc = m_aiRaidRefList->FindIndex(raidSelection);
	if(!posLoc)
		return;

	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CAIRaidObj * aiRaidPtr = (CAIRaidObj *)m_aiRaidRefList->GetAt(posLoc);
	aiRaidPtr->SafeDelete();
	delete aiRaidPtr;
	aiRaidPtr = 0;
	m_aiRaidRefList->RemoveAt(posLoc);

	InListRaids(raidSelection-1);
}

void CAIRaidDbClass::OnBTNSave() 
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "AI Raid Files|*.AIR|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "AIR", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
       if(res == FALSE){
		  SetCurrentDirectory(MainDir);
	      return;
	   }
       CArchive the_out_Archive(&fl,CArchive::store);
       the_out_Archive << m_aiRaidRefList;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

void CAIRaidDbClass::OnBTNLoadDB() 
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization

   static char BASED_CODE filter[] = "AI Raid Files|*.AIR|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
	   SetCurrentDirectory(MainDir);
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	   if(res == FALSE){
		  SetCurrentDirectory(MainDir);
		  return;
	   }
	   CArchive the_in_Archive(&fl,CArchive::load);
	   m_aiRaidRefList->SafeDelete();
	   delete m_aiRaidRefList;
	   m_aiRaidRefList = 0;
	   the_in_Archive >> m_aiRaidRefList;
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InListRaids(0);
}

void CAIRaidDbClass::OnBUTTONAddCfgOption() 
{
	m_aiCfgList.ResetContent();
	 int raidSelection = m_raidList.GetCurSel();
	 if(raidSelection == -1)
	 {
		MessageBox("Must select a raid");
	    return;
	 }

	 int spawnPointSelection = m_spawnPointList.GetCurSel();
	 if(spawnPointSelection == -1)
	 {
		MessageBox("Must select a spawn point selection");
	    return;
	 }

	POSITION posLoc = m_aiRaidRefList->FindIndex(raidSelection);
	if(!posLoc)
		return;

	    CAIRaidObj * aiRaidPtr = (CAIRaidObj *)m_aiRaidRefList->GetAt(posLoc);
		if(!aiRaidPtr->m_spawnPoints)
			return;

		posLoc = aiRaidPtr->m_spawnPoints->FindIndex(spawnPointSelection);
	    if(!posLoc)
		    return;

	        CAISpawnPoint * spawnPoint = (CAISpawnPoint *)aiRaidPtr->m_spawnPoints->GetAt(posLoc);

			if(!spawnPoint->m_cfgList)
				spawnPoint->m_cfgList = new CAICfgOptionList();
			
                CAICfgOption * cfgPtr = new CAICfgOption();
			    UpdateData(TRUE);
				cfgPtr->m_aiCfg = m_aiCfg;
				UpdateData(FALSE);

			spawnPoint->m_cfgList->AddTail(cfgPtr);
		      

        InListAiCfOptions(spawnPoint->m_cfgList->GetCount()-1);
}

void CAIRaidDbClass::OnSelchangeLISTRaidDB() 
{
	int raidSelection = m_raidList.GetCurSel();
	 if(raidSelection == -1)
	 {
		
	    return;
	 }

	 POSITION posLoc = m_aiRaidRefList->FindIndex(raidSelection);
	 if(!posLoc)
		 return;

	     CAIRaidObj * aiRaidPtr = (CAIRaidObj *)m_aiRaidRefList->GetAt(posLoc);
         
		 UpdateData(TRUE);
	     m_raidName                   = aiRaidPtr->m_raidName;
   	     m_raidDuration               = aiRaidPtr->m_raidDuration;
	     m_bonusPoints                = aiRaidPtr->m_bonusPoints;
	     m_notificationWldID          = aiRaidPtr->m_raidAlertWldID;
	     m_notificationPosX           = aiRaidPtr->m_raidAlertPosition.x;
	     m_notificationPosY           = aiRaidPtr->m_raidAlertPosition.y;
	     m_notificationPosZ           = aiRaidPtr->m_raidAlertPosition.z;
	     m_notificationRadius         = aiRaidPtr->m_raidAlertRadius;
	     m_raidWarningString          = aiRaidPtr->m_raidAlertMessage;
	     m_manditoryWait              = aiRaidPtr->m_raidManditoryWait;
		 m_raidPercent                = aiRaidPtr->m_percentSpawn;
	     UpdateData(FALSE);
		 InListSpawnPoints(0);
}

void CAIRaidDbClass::OnSelchangeLISTSpawnPointDB() 
{
	int raidSelection = m_raidList.GetCurSel();
	 if(raidSelection == -1)
	 {
	    return;
	 }

	 int spawnPointSelection = m_spawnPointList.GetCurSel();
	 if(spawnPointSelection == -1)
	 {
	    return;
	 }

	POSITION posLoc = m_aiRaidRefList->FindIndex(raidSelection);
	if(!posLoc)
		return;

	    CAIRaidObj * aiRaidPtr = (CAIRaidObj *)m_aiRaidRefList->GetAt(posLoc);
		if(!aiRaidPtr->m_spawnPoints)
			return;

		posLoc = aiRaidPtr->m_spawnPoints->FindIndex(spawnPointSelection);
	    if(!posLoc)
		    return;

	        CAISpawnPoint * spawnPoint = (CAISpawnPoint *)aiRaidPtr->m_spawnPoints->GetAt(posLoc);

			UpdateData(TRUE);
			m_posX    = spawnPoint->m_position.x; 
			m_posY    = spawnPoint->m_position.y; 
			m_posZ    = spawnPoint->m_position.z; 
			m_worldID = spawnPoint->m_worldID;
			UpdateData(FALSE);
			
	        InListAiCfOptions(0);
}

void CAIRaidDbClass::OnSelchangeLISTAIcfgDatabase() 
{
	int raidSelection = m_raidList.GetCurSel();
	 if(raidSelection == -1)
	 {
	    return;
	 }

	 int spawnPointSelection = m_spawnPointList.GetCurSel();
	 if(spawnPointSelection == -1)
	 {
	    return;
	 }

	 int aiCfgSelection = m_aiCfgList.GetCurSel();
	 if(aiCfgSelection == -1)
	 {
	    return;
	 }

	POSITION posLoc = m_aiRaidRefList->FindIndex(raidSelection);
	if(!posLoc)
		return;

	    CAIRaidObj * aiRaidPtr = (CAIRaidObj *)m_aiRaidRefList->GetAt(posLoc);
		if(!aiRaidPtr->m_spawnPoints)
			return;

		posLoc = aiRaidPtr->m_spawnPoints->FindIndex(spawnPointSelection);
	    if(!posLoc)
		    return;

	        CAISpawnPoint * spawnPoint = (CAISpawnPoint *)aiRaidPtr->m_spawnPoints->GetAt(posLoc);

			if(!spawnPoint->m_cfgList)
				return;
		
            posLoc = spawnPoint->m_cfgList->FindIndex(aiCfgSelection);
	        if(!posLoc)
		        return;

	            CAICfgOption * cfgPtr = (CAICfgOption *)spawnPoint->m_cfgList->GetAt(posLoc);

				UpdateData(TRUE);
                m_aiCfg = cfgPtr->m_aiCfg;
				UpdateData(FALSE);

			
}

void CAIRaidDbClass::OnBUTTONReplaceRaid() 
{
	int raidSelection = m_raidList.GetCurSel();
	 if(raidSelection == -1)
	 {
		MessageBox("Must select a monster raid object");
	    return;
	 }

	 POSITION posLoc = m_aiRaidRefList->FindIndex(raidSelection);
	 if(!posLoc)
		 return;

	     CAIRaidObj * aiRaidPtr = (CAIRaidObj *)m_aiRaidRefList->GetAt(posLoc);
         
		 UpdateData(TRUE);
	     aiRaidPtr->m_raidName            = m_raidName;
     	 aiRaidPtr->m_raidDuration        = m_raidDuration;
	     aiRaidPtr->m_bonusPoints         = m_bonusPoints;
 	     aiRaidPtr->m_raidAlertWldID      = m_notificationWldID;
	     aiRaidPtr->m_raidAlertPosition.x = m_notificationPosX;
	     aiRaidPtr->m_raidAlertPosition.y = m_notificationPosY;
	     aiRaidPtr->m_raidAlertPosition.z = m_notificationPosZ;
	     aiRaidPtr->m_raidAlertRadius     = m_notificationRadius;
	     aiRaidPtr->m_raidAlertMessage    = m_raidWarningString;
	     aiRaidPtr->m_raidManditoryWait   = m_manditoryWait;
		 aiRaidPtr->m_percentSpawn        = m_raidPercent;
	     UpdateData(FALSE);
		 InListRaids(raidSelection);
}

void CAIRaidDbClass::OnBUTTONDeleteSpawnPoint() 
{
	m_aiCfgList.ResetContent();
	int raidSelection = m_raidList.GetCurSel();
	if(raidSelection == -1)
	{
		MessageBox("Must select a raid");
		return;
	}

	int spawnPointSelection = m_spawnPointList.GetCurSel();
	if(spawnPointSelection == -1)
	{
		MessageBox("Must select a spawn point selection");
		return;
	}

	POSITION posLoc = m_aiRaidRefList->FindIndex(raidSelection);
	if(!posLoc)
		return;

	CAIRaidObj * aiRaidPtr = (CAIRaidObj *)m_aiRaidRefList->GetAt(posLoc);
	if(!aiRaidPtr->m_spawnPoints)
		return;

	posLoc = aiRaidPtr->m_spawnPoints->FindIndex(spawnPointSelection);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CAISpawnPoint * spawnPoint = (CAISpawnPoint *)aiRaidPtr->m_spawnPoints->GetAt(posLoc);
	spawnPoint->SafeDelete();
	delete spawnPoint;
	spawnPoint = 0;
	aiRaidPtr->m_spawnPoints->RemoveAt(posLoc);

	InListSpawnPoints(spawnPointSelection-1);
}

void CAIRaidDbClass::OnBUTTONReplaceSpawnPoint() 
{
	m_aiCfgList.ResetContent();
	 int raidSelection = m_raidList.GetCurSel();
	 if(raidSelection == -1)
	 {
		MessageBox("Must select a raid");
	    return;
	 }

	 int spawnPointSelection = m_spawnPointList.GetCurSel();
	 if(spawnPointSelection == -1)
	 {
		MessageBox("Must select a spawn point selection");
	    return;
	 }

	POSITION posLoc = m_aiRaidRefList->FindIndex(raidSelection);
	if(!posLoc)
		return;

	    CAIRaidObj * aiRaidPtr = (CAIRaidObj *)m_aiRaidRefList->GetAt(posLoc);
		if(!aiRaidPtr->m_spawnPoints)
			return;

		posLoc = aiRaidPtr->m_spawnPoints->FindIndex(spawnPointSelection);
	    if(!posLoc)
		    return;

	        CAISpawnPoint * spawnPoint = (CAISpawnPoint *)aiRaidPtr->m_spawnPoints->GetAt(posLoc);
			UpdateData(TRUE);
            spawnPoint->m_position.x = m_posX; 
			spawnPoint->m_position.y = m_posY; 
			spawnPoint->m_position.z = m_posZ; 
			spawnPoint->m_worldID    = m_worldID;
			UpdateData(FALSE);

			InListSpawnPoints(spawnPointSelection);
	
}

void CAIRaidDbClass::OnBUTTONDelCfgOption() 
{
	int raidSelection = m_raidList.GetCurSel();
	 if(raidSelection == -1)
	 {
		AfxMessageBox("Must select a raid object");
	    return;
	 }

	 int spawnPointSelection = m_spawnPointList.GetCurSel();
	 if(spawnPointSelection == -1)
	 {
		AfxMessageBox("Must select spawnpoint selection");
	    return;
	 }

	 int aiCfgSelection = m_aiCfgList.GetCurSel();
	 if(aiCfgSelection == -1)
	 {
		AfxMessageBox("Must select a cfg");
	    return;
	 }

	POSITION posLoc = m_aiRaidRefList->FindIndex(raidSelection);
	if(!posLoc)
		return;

	    CAIRaidObj * aiRaidPtr = (CAIRaidObj *)m_aiRaidRefList->GetAt(posLoc);
		if(!aiRaidPtr->m_spawnPoints)
			return;

		posLoc = aiRaidPtr->m_spawnPoints->FindIndex(spawnPointSelection);
	    if(!posLoc)
		    return;

	        CAISpawnPoint * spawnPoint = (CAISpawnPoint *)aiRaidPtr->m_spawnPoints->GetAt(posLoc);

			if(!spawnPoint->m_cfgList)
				return;
		
            posLoc = spawnPoint->m_cfgList->FindIndex(aiCfgSelection);
	        if(!posLoc)
		        return;

	            CAICfgOption * cfgPtr = (CAICfgOption *)spawnPoint->m_cfgList->GetAt(posLoc);

				delete cfgPtr;
				cfgPtr = 0;
				spawnPoint->m_cfgList->RemoveAt(posLoc);

				InListAiCfOptions(aiCfgSelection-1);

				
}

void CAIRaidDbClass::OnBnClickedButtonLoadunitai()
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization

   static char BASED_CODE filter[] = "AI Raid Unit Files|*.AIU|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
	   SetCurrentDirectory(MainDir);
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	   if(res == FALSE){
		  SetCurrentDirectory(MainDir);
		  return;
	   }
	   CArchive the_in_Archive(&fl,CArchive::load);
	   CAIRaidObj * aiRaidPtr = NULL;
	   the_in_Archive >> aiRaidPtr;
	   m_aiRaidRefList->AddTail(aiRaidPtr);
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InListRaids(m_aiRaidRefList->GetCount()-1);
}

void CAIRaidDbClass::OnBnClickedButtonSaveunitai()
{
	int raidSelection = m_raidList.GetCurSel();
	 if(raidSelection == -1)
	 {
		MessageBox("Must select a monster raid object");
	    return;
	 }

	 POSITION posLoc = m_aiRaidRefList->FindIndex(raidSelection);
	 if(!posLoc)
		 return;

	     CAIRaidObj * aiRaidPtr = (CAIRaidObj *)m_aiRaidRefList->GetAt(posLoc);
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "AI Raid Unit Files|*.AIU|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "AIU", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
       if(res == FALSE){
		  SetCurrentDirectory(MainDir);
	      return;
	   }
       CArchive the_out_Archive(&fl,CArchive::store);
       the_out_Archive << aiRaidPtr;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}
