// CurrentWorldsDlg.cpp : implementation file
//

#include "resource.h"
#include "stdafx.h"
#include "d3d9.h"
#include "CurrentWorldsDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCurrentWorldsDlg dialog


CCurrentWorldsDlg::CCurrentWorldsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCurrentWorldsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCurrentWorldsDlg)
	m_worldName = _T("");
	m_positionX = 0.0f;
	m_positionY = 0.0f;
	m_positionZ = 0.0f;
	m_spawnPointCfgName = _T("");
	m_wldId = 0;
	//}}AFX_DATA_INIT
}


void CCurrentWorldsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCurrentWorldsDlg)
	DDX_Control(pDX, IDC_LIST_SpawnPointCfgList, m_spawnPointCfgList);
	DDX_Control(pDX, IDC_LIST_WorldList, m_worldList);
	DDX_Text(pDX, IDC_EDIT_worldName, m_worldName);
	DDX_Text(pDX, IDC_EDIT_PositionX, m_positionX);
	DDX_Text(pDX, IDC_EDIT_PositionY, m_positionY);
	DDX_Text(pDX, IDC_EDIT_PositionZ, m_positionZ);
	DDX_Text(pDX, IDC_EDIT_spawnPointCfgname, m_spawnPointCfgName);
	DDX_Text(pDX, IDC_EDIT_ID, m_wldId);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCurrentWorldsDlg, CDialog)
	//{{AFX_MSG_MAP(CCurrentWorldsDlg)
	ON_BN_CLICKED(IDC_BUTTON_BrowseFromFile, OnBUTTONBrowseFromFile)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_Replace, OnBUTTONReplace)
	ON_BN_CLICKED(IDC_BUTTON_Add, OnBUTTONAdd)
	ON_BN_CLICKED(IDC_BUTTON_AddSpawnPointCfg, OnBUTTONAddSpawnPointCfg)
	ON_BN_CLICKED(IDC_BUTTON_DeleteSpawnPointCfg, OnBUTTONDeleteSpawnPointCfg)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceSpawnPointCfg, OnBUTTONReplaceSpawnPointCfg)
	ON_LBN_SELCHANGE(IDC_LIST_SpawnPointCfgList, OnSelchangeLISTSpawnPointCfgList)
	ON_LBN_SELCHANGE(IDC_LIST_WorldList, OnSelchangeLISTWorldList)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BTN_AddworldC, OnBnClickedBtnAddworldc)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCurrentWorldsDlg message handlers

BOOL CCurrentWorldsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    if(!m_multiplayerObject->m_currentWorldsUsed)
		m_multiplayerObject->m_currentWorldsUsed = new CWorldAvailableObjList();

    if(!m_multiplayerObject->m_spawnPointCfgs)
		m_multiplayerObject->m_spawnPointCfgs = new CSpawnCfgObjList();

	InListCurrentWorlds(0);
	InListCurrentSpawnCfg(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCurrentWorldsDlg::InListCurrentWorlds(int sel)
{
 
}

void CCurrentWorldsDlg::OnBUTTONReplace() 
{
   
	
}

void CCurrentWorldsDlg::OnButtonDelete() 
{
  
	
}

void CCurrentWorldsDlg::OnBUTTONBrowseFromFile() 
{
   //use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = CurDir+"\\GameFiles\\";
			 SetCurrentDirectory(CurDir);
 //end use mapsmodels folder for organization
 static char BASED_CODE filter[] = "EXG Files|*.EXG|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   m_worldName = opendialog.GetFileName();
	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);
}

void CCurrentWorldsDlg::OnBUTTONAdd() 
{
   
}

void CCurrentWorldsDlg::OnBUTTONAddSpawnPointCfg() 
{
    int worldSelect = m_worldList.GetCurSel();
	if(worldSelect == -1)
	{
       MessageBox("Must select a world");
       return;
	}

	UpdateData(TRUE);
	CSpawnCfgObj * spawnPtr           = new CSpawnCfgObj();
	spawnPtr->m_spawnPointName        = m_spawnPointCfgName;
	spawnPtr->m_position.x            = m_positionX;
	spawnPtr->m_position.y            = m_positionY;
	spawnPtr->m_position.z            = m_positionZ;
	spawnPtr->m_worldReferenceIndex   = worldSelect;
	m_multiplayerObject->m_spawnPointCfgs->AddTail(spawnPtr);
	UpdateData(FALSE);
    InListCurrentSpawnCfg(m_multiplayerObject->m_spawnPointCfgs->GetCount()-1);
	
}

void CCurrentWorldsDlg::OnBUTTONDeleteSpawnPointCfg() 
{
	int spawnSelect = m_spawnPointCfgList.GetCurSel();
	if(spawnSelect == -1)
	{
		MessageBox("Must select a spawn Cfg");
		return;
	}

	POSITION spawnPos = m_multiplayerObject->m_spawnPointCfgs->FindIndex(spawnSelect);
	if(spawnPos)
	{
		if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
			return;
		
		CSpawnCfgObj * spawnPtr = (CSpawnCfgObj *)m_multiplayerObject->m_spawnPointCfgs->GetAt(spawnPos);
		delete spawnPtr;
		spawnPtr=0;
		m_multiplayerObject->m_spawnPointCfgs->RemoveAt(spawnPos);
	}
	InListCurrentSpawnCfg(spawnSelect-1);
}

void CCurrentWorldsDlg::OnBUTTONReplaceSpawnPointCfg() 
{
	int spawnSelect = m_spawnPointCfgList.GetCurSel();
	if(spawnSelect == -1)
	{
       MessageBox("Must select a spawn Cfg");
       return;
	}

	int worldSelect = m_worldList.GetCurSel();
	if(worldSelect == -1)
	{
       MessageBox("Must select a world");
       return;
	}
 

	POSITION spawnPos = m_multiplayerObject->m_spawnPointCfgs->FindIndex(spawnSelect);
	if(spawnPos){
       CSpawnCfgObj * spawnPtr = (CSpawnCfgObj *)m_multiplayerObject->m_spawnPointCfgs->GetAt(spawnPos);
       UpdateData(TRUE);
	   spawnPtr->m_spawnPointName        = m_spawnPointCfgName;
	   spawnPtr->m_position.x            = m_positionX;
	   spawnPtr->m_position.y            = m_positionY;
	   spawnPtr->m_position.z            = m_positionZ;
	   spawnPtr->m_worldReferenceIndex   = worldSelect;
	   UpdateData(FALSE);
	}
	InListCurrentSpawnCfg(spawnSelect);
}

void CCurrentWorldsDlg::InListCurrentSpawnCfg(int sel)
{
  UpdateData(TRUE);
  int i = 0;

  m_spawnPointCfgList.ResetContent();
	for(POSITION posLoc = m_multiplayerObject->m_spawnPointCfgs->GetHeadPosition();posLoc != NULL;)
    {
       CSpawnCfgObj * spawnPtr = (CSpawnCfgObj *)m_multiplayerObject->m_spawnPointCfgs->GetNext(posLoc);
	   m_spawnPointCfgList.AddString(operator+(spawnPtr->m_spawnPointName,ConvToString(i)));
	   i++;
	}
	if(sel >= 0)
       m_spawnPointCfgList.SetCurSel(sel);
  UpdateData(FALSE);
}



void CCurrentWorldsDlg::OnSelchangeLISTSpawnPointCfgList() 
{
	int spawnSelect = m_spawnPointCfgList.GetCurSel();
	if(spawnSelect == -1)
	{
       return;
	}

	POSITION spawnPos = m_multiplayerObject->m_spawnPointCfgs->FindIndex(spawnSelect);
	if(spawnPos){
       CSpawnCfgObj * spawnPtr = (CSpawnCfgObj *)m_multiplayerObject->m_spawnPointCfgs->GetAt(spawnPos);
       UpdateData(TRUE);
	   m_spawnPointCfgName   = spawnPtr->m_spawnPointName;
	   m_positionX           = spawnPtr->m_position.x;
	   m_positionY           = spawnPtr->m_position.y;
	   m_positionZ           = spawnPtr->m_position.z;
	   m_worldList.SetCurSel(spawnPtr->m_worldReferenceIndex);
	   UpdateData(FALSE);
	}
}

void CCurrentWorldsDlg::OnSelchangeLISTWorldList() 
{

}

//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----converts a long into string-----------------------------//
CString CCurrentWorldsDlg::ConvToString(int number)
{    
              if(number==0)
				 return "0";
	 
	          int decimal,sign;
              char *buffer;
              CString temp;	 
	          buffer = _fcvt(number,0,&decimal,&sign);
    	      temp = buffer;
			  if(sign != 0)
                 temp = operator+("-",temp);
		
			  return temp;
}

void CCurrentWorldsDlg::OnBnClickedBtnAddworldc()
{
	
}
