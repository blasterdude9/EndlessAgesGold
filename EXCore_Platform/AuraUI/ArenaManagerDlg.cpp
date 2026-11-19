// ArenaManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "ArenaManagerDlg.h"
#include "Direct.h"

// CArenaManagerDlg dialog

IMPLEMENT_DYNAMIC(CArenaManagerDlg, CDialog)
CArenaManagerDlg::CArenaManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CArenaManagerDlg::IDD, pParent)
	, m_arenaName(_T(""))
	, m_battleMinimum(0)
	, m_battleMaximum(0)
	, m_battleDuration(0)
	, m_levelRangeMax(0)
	, m_levelRangeMin(0)
	, m_visualWorldIndex(0)
	, m_worldChannel(0)
	, m_spawnPointX(0)
	, m_spawnPointY(0)
	, m_spawnPointZ(0)
{
}

CArenaManagerDlg::~CArenaManagerDlg()
{
}

void CArenaManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BattleList, m_battlesList);
	DDX_Text(pDX, IDC_EDIT_ArenaName, m_arenaName);
	DDX_Control(pDX, IDC_COMBO_ArenaType, m_arenaType);
	DDX_Text(pDX, IDC_EDIT_BattleMinimum, m_battleMinimum);
	DDX_Text(pDX, IDC_EDIT_BattleMaximum, m_battleMaximum);
	DDX_Text(pDX, IDC_EDIT_BattleDuration, m_battleDuration);
	DDX_Text(pDX, IDC_EDIT_LevelRangeMax, m_levelRangeMax);
	DDX_Text(pDX, IDC_EDIT_LevelRangeMin, m_levelRangeMin);
	DDX_Text(pDX, IDC_EDIT_VisualWldIndex, m_visualWorldIndex);
	DDX_Text(pDX, IDC_EDIT_WorldChannel, m_worldChannel);
	DDX_Control(pDX, IDC_LIST_SpawnPoints, m_spawnPoints);
	DDX_Text(pDX, IDC_EDIT_SpawnPointX, m_spawnPointX);
	DDX_Text(pDX, IDC_EDIT_SpawnPointY, m_spawnPointY);
	DDX_Text(pDX, IDC_EDIT_SpawnPointZ, m_spawnPointZ);
}


BEGIN_MESSAGE_MAP(CArenaManagerDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_Replace, OnBnClickedButtonReplace)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
	ON_LBN_SELCHANGE(IDC_LIST_BattleList, OnLbnSelchangeListBattlelist)
	ON_BN_CLICKED(IDC_BUTTON_AddSpawnPoint, OnBnClickedButtonAddspawnpoint)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceSpawnPoint, OnBnClickedButtonReplacespawnpoint)
	ON_BN_CLICKED(IDC_BUTTON_DeleteSpawnPoint, OnBnClickedButtonDeletespawnpoint)
	ON_LBN_SELCHANGE(IDC_LIST_SpawnPoints, OnLbnSelchangeListSpawnpoints)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CArenaManagerDlg message handlers
BOOL CArenaManagerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_arenaType.AddString("Last Man Standing");
	m_arenaType.AddString("Team Arena");
	m_arenaType.SetCurSel(0);
	Inlist(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CArenaManagerDlg::Inlist(int sel)
{
	m_battlesList.ResetContent();
     for(POSITION posLoc = m_battleSysListRef->GetHeadPosition();posLoc != NULL;)
	 {
         CBattleSchedulerObj * battleObj = (CBattleSchedulerObj *) m_battleSysListRef->GetNext(posLoc);
         m_battlesList.AddString(battleObj->m_name);
	 }
	 if(sel > 1)
	    m_battlesList.SetCurSel(sel);

	 
}

void CArenaManagerDlg::OnBnClickedButtonAdd()
{
	UpdateData(TRUE);
	CBattleSchedulerObj * battleObj = new CBattleSchedulerObj();
    battleObj->m_name = m_arenaName;
    battleObj->m_type = m_arenaType.GetCurSel();
	if(battleObj->m_type < 0)
	   battleObj->m_type = 0;

	battleObj->m_batlleMinimum = m_battleMinimum;
	battleObj->m_battleMaximum = m_battleMaximum;
	battleObj->m_battleDuration = m_battleDuration;
	battleObj->m_levelRangeMax = m_levelRangeMax;
	battleObj->m_levelRangeMin = m_levelRangeMin;
	battleObj->m_visualWldIndex = m_visualWorldIndex;
	battleObj->m_wldChannel = m_worldChannel;
	
    m_battleSysListRef->AddTail(battleObj);

    UpdateData(FALSE);

	Inlist(m_battleSysListRef->GetCount()-1);
}

void CArenaManagerDlg::OnBnClickedButtonReplace()
{
	int selection = m_battlesList.GetCurSel();
	if(selection == -1)
	{
        MessageBox("You Must select a battle");
		return;
	}
	POSITION posLoc = m_battleSysListRef->FindIndex(selection);
	if(!posLoc)
		return;
    CBattleSchedulerObj * battleObj = (CBattleSchedulerObj *) m_battleSysListRef->GetAt(posLoc);

	UpdateData(TRUE);
	battleObj->m_name = m_arenaName;
    battleObj->m_type = m_arenaType.GetCurSel();
	if(battleObj->m_type < 0)
	   battleObj->m_type = 0;

	battleObj->m_batlleMinimum     = m_battleMinimum;
	battleObj->m_battleMaximum     = m_battleMaximum;
	battleObj->m_battleDuration    = m_battleDuration;
	battleObj->m_levelRangeMax     = m_levelRangeMax;
	battleObj->m_levelRangeMin     = m_levelRangeMin;
	battleObj->m_visualWldIndex    = m_visualWorldIndex;
	battleObj->m_wldChannel        = m_worldChannel;
    UpdateData(FALSE);
	Inlist(selection);

}

void CArenaManagerDlg::OnBnClickedButtonDelete()
{
	int selection = m_battlesList.GetCurSel();
	if(selection == -1)
	{
        MessageBox("You Must select a battle");
		return;
	}
	POSITION posLoc = m_battleSysListRef->FindIndex(selection);
	if(!posLoc)
		return;
    CBattleSchedulerObj * battleObj = (CBattleSchedulerObj *) m_battleSysListRef->GetAt(posLoc);
    battleObj->SafeDelete();
	delete battleObj;
	battleObj = 0;
	m_battleSysListRef->RemoveAt(posLoc);
	
	Inlist(selection-1);
}

void CArenaManagerDlg::OnLbnSelchangeListBattlelist()
{
	int selection = m_battlesList.GetCurSel();
	if(selection == -1)
	{
        MessageBox("You Must select a battle");
		return;
	}
	POSITION posLoc = m_battleSysListRef->FindIndex(selection);
	if(!posLoc)
		return;
    CBattleSchedulerObj * battleObj = (CBattleSchedulerObj *) m_battleSysListRef->GetAt(posLoc);

	UpdateData(TRUE);
	m_arenaName             = battleObj->m_name;
    m_arenaType.SetCurSel(battleObj->m_type);
	m_battleMinimum         = battleObj->m_batlleMinimum;
	m_battleMaximum         = battleObj->m_battleMaximum;
	m_battleDuration        = battleObj->m_battleDuration;
	m_levelRangeMax         = battleObj->m_levelRangeMax;
	m_levelRangeMin         = battleObj->m_levelRangeMin;
	m_visualWorldIndex      = battleObj->m_visualWldIndex;
	m_worldChannel          = battleObj->m_wldChannel;
    UpdateData(FALSE);
	InListSpawnPoints(0);
}

void CArenaManagerDlg::OnBnClickedButtonAddspawnpoint()
{
	int selection = m_battlesList.GetCurSel();
	if(selection == -1)
	{
        MessageBox("You Must select a battle");
		return;
	}
	POSITION posLoc = m_battleSysListRef->FindIndex(selection);
	if(!posLoc)
		return;
    CBattleSchedulerObj * battleObj = (CBattleSchedulerObj *) m_battleSysListRef->GetAt(posLoc);
	D3DVECTOR spawnPoint;
	UpdateData(TRUE);
	spawnPoint.x = m_spawnPointX;
	spawnPoint.y = m_spawnPointY;
	spawnPoint.z = m_spawnPointZ;
	UpdateData(FALSE);
	battleObj->AddSpawnPoint(spawnPoint);
	InListSpawnPoints(battleObj->m_spawnPointCount-1);
}

void CArenaManagerDlg::OnBnClickedButtonReplacespawnpoint()
{
	int selection = m_battlesList.GetCurSel();
	if(selection == -1)
	{
        MessageBox("You Must select a battle");
		return;
	}
	POSITION posLoc = m_battleSysListRef->FindIndex(selection);
	if(!posLoc)
		return;
  
	int spawnSel = m_spawnPoints.GetCurSel();
    if(spawnSel < 0)
	{
       MessageBox("Must select a spawn point");
	   return;
	}

    CBattleSchedulerObj * battleObj = (CBattleSchedulerObj *) m_battleSysListRef->GetAt(posLoc);
	D3DVECTOR spawnPoint;
	UpdateData(TRUE);
	spawnPoint.x = m_spawnPointX;
	spawnPoint.y = m_spawnPointY;
	spawnPoint.z = m_spawnPointZ;
	UpdateData(FALSE);
	battleObj->ReplaceSpawnPoint(spawnSel,spawnPoint);
	InListSpawnPoints(spawnSel);
}

void CArenaManagerDlg::OnBnClickedButtonDeletespawnpoint()
{
	int selection = m_battlesList.GetCurSel();
	if(selection == -1)
	{
        MessageBox("You Must select a battle");
		return;
	}
	POSITION posLoc = m_battleSysListRef->FindIndex(selection);
	if(!posLoc)
		return;
  
	int spawnSel = m_spawnPoints.GetCurSel();
    if(spawnSel < 0)
	{
       MessageBox("Must select a spawn point");
	   return;
	}

    CBattleSchedulerObj * battleObj = (CBattleSchedulerObj *) m_battleSysListRef->GetAt(posLoc);
	
	battleObj->DeleteSpawnPoint(spawnSel);
	InListSpawnPoints(spawnSel-1);
}

void CArenaManagerDlg::InListSpawnPoints(int sel)
{
	m_spawnPoints.ResetContent();
    int selection = m_battlesList.GetCurSel();
	if(selection == -1)
	{
		return;
	}
	POSITION posLoc = m_battleSysListRef->FindIndex(selection);
	if(!posLoc)
		return;
 
    
    CBattleSchedulerObj * battleObj = (CBattleSchedulerObj *) m_battleSysListRef->GetAt(posLoc);
	for(int loop=0;loop<battleObj->m_spawnPointCount;loop++)
	{
        m_spawnPoints.AddString("SPAWNPOINT");
	}
	if(sel > -1)
	m_spawnPoints.SetCurSel(sel);
}

void CArenaManagerDlg::OnLbnSelchangeListSpawnpoints()
{
	int selection = m_battlesList.GetCurSel();
	if(selection == -1)
	{
        MessageBox("You Must select a battle");
		return;
	}
	POSITION posLoc = m_battleSysListRef->FindIndex(selection);
	if(!posLoc)
		return;
  
	int spawnSel = m_spawnPoints.GetCurSel();
    if(spawnSel < 0)
	{
       MessageBox("Must select a spawn point");
	   return;
	}

    CBattleSchedulerObj * battleObj = (CBattleSchedulerObj *) m_battleSysListRef->GetAt(posLoc);

	UpdateData(TRUE);
	m_spawnPointX = battleObj->m_spawnPositions[spawnSel].x;
	m_spawnPointY = battleObj->m_spawnPositions[spawnSel].y;
	m_spawnPointZ = battleObj->m_spawnPositions[spawnSel].z;
	UpdateData(FALSE);
	
}



void CArenaManagerDlg::OnBnClickedButtonLoad()
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

   static char BASED_CODE filter[] = "Arena Manager Files|*.AMF|All Files (*.*)|*.*||";
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
	   m_battleSysListRef->SafeDelete();
	   delete m_battleSysListRef;
	   m_battleSysListRef = 0;
	   the_in_Archive >> m_battleSysListRef;
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 Inlist(0);
}

void CArenaManagerDlg::OnBnClickedButtonSave()
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
     static char BASED_CODE filter[] = "Arena Manager Files|*.AMF|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "AMF", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
       if(res == FALSE)
	   {
		  SetCurrentDirectory(MainDir);
	      return;
	   }
       CArchive the_out_Archive(&fl,CArchive::store);
       the_out_Archive << m_battleSysListRef;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}
