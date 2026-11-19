// ActionZonesDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CBaseInterfaceModule.h"
#include "ActionZonesDlg.h"
#include ".\actionzonesdlg.h"
#include "direct.h"
#include "CEXMeshClass.h"


// CActionZonesDlg dialog

IMPLEMENT_DYNAMIC(CActionZonesDlg, CDialog)
CActionZonesDlg::CActionZonesDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CActionZonesDlg::IDD, pParent)
	, m_maxX(0)
	, m_maxY(0)
	, m_maxZ(0)
	, m_minX(0)
	, m_minY(0)
	, m_minZ(0)
	, m_actionValue(0)
	, m_zoneName(_T(""))
	, m_attributeValue(0)
{
}

CActionZonesDlg::~CActionZonesDlg()
{
}

void CActionZonesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_Channels, CWorldChannelList);
	DDX_Control(pDX, IDC_LIST_ZoneList, CActionZoneList);
	DDX_Text(pDX, IDC_EDIT_MaxX, m_maxX);
	DDX_Text(pDX, IDC_EDIT_MaxY, m_maxY);
	DDX_Text(pDX, IDC_EDIT_MaxZ, m_maxZ);
	DDX_Text(pDX, IDC_EDIT_MinX, m_minX);
	DDX_Text(pDX, IDC_EDIT_MinY, m_minY);
	DDX_Text(pDX, IDC_EDIT_MinZ, m_minZ);
	DDX_Text(pDX, IDC_EDIT_actionValue, m_actionValue);
	DDX_Control(pDX, IDC_COMBO_ActionType, m_actionType);
	DDX_Text(pDX, IDC_EDIT_zoneLabel, m_zoneName);
	DDX_Text(pDX, IDC_EDIT_attributeValue, m_attributeValue);
}


BEGIN_MESSAGE_MAP(CActionZonesDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ACAddChannel, OnBnClickedButtonAcaddchannel)
	ON_BN_CLICKED(IDC_BUTTON_ACDeleteChannel, OnBnClickedButtonAcdeletechannel)
	ON_BN_CLICKED(IDC_BUTTON_ACAdd, OnBnClickedButtonAcadd)
	ON_BN_CLICKED(IDC_BUTTON_ACReplace, OnBnClickedButtonAcreplace)
	ON_BN_CLICKED(IDC_BUTTON_ACDelete, OnBnClickedButtonAcdelete)
	ON_BN_CLICKED(IDC_BUTTON_LoadARW, OnBnClickedButtonLoadarw)
	ON_LBN_SELCHANGE(IDC_LIST_Channels, OnLbnSelchangeListChannels)
	ON_LBN_SELCHANGE(IDC_LIST_ZoneList, OnLbnSelchangeListZonelist)
	ON_BN_CLICKED(IDC_BUTTON_LoadDB, OnBnClickedButtonLoaddb)
	ON_BN_CLICKED(IDC_BUTTON_SaveDB, OnBnClickedButtonSavedb)
END_MESSAGE_MAP()


// CActionZonesDlg message handlers

void CActionZonesDlg::OnBnClickedButtonAcaddchannel()
{
	m_actionZoneDB->AddZone();
	InListChannels(m_actionZoneDB->GetCount()-1);
}

void CActionZonesDlg::OnBnClickedButtonAcdeletechannel()
{
    int channelSelect = CWorldChannelList.GetCurSel();
	if(channelSelect < 0)
	{
	   AfxMessageBox("Must select a channel");
       return;
	}

	m_actionZoneDB->DeleteZone(channelSelect);
	InListChannels(channelSelect-1);
}

void CActionZonesDlg::OnBnClickedButtonAcadd()
{
	int channelSelect = CWorldChannelList.GetCurSel();
	if(channelSelect < 0)
	{
	   AfxMessageBox("Must select a channel");
       return;
	}

    CActionZone * acZone = m_actionZoneDB->GetByIndex(channelSelect);
	if(!acZone)
	    return;
    UpdateData(TRUE);
	D3DVECTOR min,max;
	min.x = m_minX;
	min.y = m_minY;
	min.z = m_minZ;
	max.x = m_maxX;
	max.y = m_maxY;
	max.z = m_maxZ;
	acZone->AddBox(min,max,m_actionType.GetCurSel(),m_actionValue,m_attributeValue,m_zoneName);
    UpdateData(FALSE);
	InListZones(channelSelect);

}

void CActionZonesDlg::OnBnClickedButtonAcreplace()
{
	int channelSelect = CWorldChannelList.GetCurSel();
	if(channelSelect < 0)
	{
	   AfxMessageBox("Must select a channel");
       return;
	}

	int zoneSelect = CActionZoneList.GetCurSel();
	if(zoneSelect < 0)
	{
	   AfxMessageBox("Must select a zone");
       return;
	}

    CActionZone * acZone = m_actionZoneDB->GetByIndex(channelSelect);
	if(!acZone)
	    return;
    UpdateData(TRUE);
	D3DVECTOR min,max;
	min.x = m_minX;
	min.y = m_minY;
	min.z = m_minZ;
	max.x = m_maxX;
	max.y = m_maxY;
	max.z = m_maxZ;
	acZone->EditBox(zoneSelect,min,max,m_actionType.GetCurSel(),m_actionValue,m_attributeValue,m_zoneName);
    UpdateData(FALSE);
	InListZones(zoneSelect);
}

void CActionZonesDlg::OnBnClickedButtonAcdelete()
{
	int channelSelect = CWorldChannelList.GetCurSel();
	if(channelSelect < 0)
	{
	   AfxMessageBox("Must select a channel");
       return;
	}

    CActionZone * acZone = m_actionZoneDB->GetByIndex(channelSelect);
	if(!acZone)
	    return;
    UpdateData(TRUE);
	acZone->DeleteBox(channelSelect);
    UpdateData(FALSE);
	InListZones(channelSelect);
}

void CActionZonesDlg::OnBnClickedButtonLoadarw()
{
    static char BASED_CODE filter[] = "Engine Models|*.arw;*.x;*.3ds;*.fbx;*.obj;*.gltf;*.glb;*.dae;*.stl;*.ply;*.blend;*.ase;*.lwo;*.dxf|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   CString fileName = opendialog.GetPathName();
	   UpdateData(FALSE);

       CEXMeshObj * m_mesh = new CEXMeshObj();
	   m_mesh->List_materialObject = new CMaterialObject();
	   float dist = 0.0f;
       if(m_mesh->LoadGeometryFromPlugin(fileName,&dist,NULL,NULL))
	   {//1
          UpdateData(TRUE);
          m_maxX = m_mesh->List_boundingBox.maxX;
          m_minX = m_mesh->List_boundingBox.minX;
          m_maxY = m_mesh->List_boundingBox.maxY;
          m_minY = m_mesh->List_boundingBox.minY;
          m_maxZ = m_mesh->List_boundingBox.maxZ;
          m_minZ = m_mesh->List_boundingBox.minZ;
          UpdateData(FALSE);
          m_mesh->SafeDelete();
		  delete m_mesh;
		  m_mesh = 0;
	   }//end 1
	 }
}

void CActionZonesDlg::InListChannels(int sel)
{
    CWorldChannelList.ResetContent();
    
	int trace = 0;
	for(POSITION posLoc = m_actionZoneDB->GetHeadPosition();posLoc != NULL;)
	{
        CActionZone * actionZonePtr = (CActionZone *) m_actionZoneDB->GetNext(posLoc);

        CWorldChannelList.AddString("Channel"+m_cabLib.ConvToString(trace));
		trace++;
	}
	if(sel > -1)
       CWorldChannelList.SetCurSel(sel);
}

void CActionZonesDlg::InListZones(int sel)
{
    CActionZoneList.ResetContent();
    
	int channelSel = CWorldChannelList.GetCurSel();
	if(channelSel < 0)
       return;

	POSITION posLoc = m_actionZoneDB->FindIndex(channelSel);
	if(!posLoc)
		return;

	    CActionZone * actionZonePtr = (CActionZone *)m_actionZoneDB->GetAt(posLoc);
        int trace = 0;
		for(POSITION posLoc = actionZonePtr->m_boxList.GetHeadPosition();posLoc != NULL;)
		{
			CActionBox * abox = (CActionBox *) actionZonePtr->m_boxList.GetNext(posLoc);

			CActionZoneList.AddString(m_cabLib.ConvToString(trace) + ")" + abox->m_zoneLabel);
			trace++;
		}
		if(sel > -1)
		   CActionZoneList.SetCurSel(sel);

}



BOOL CActionZonesDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_actionType.AddString("NONE");
	m_actionType.AddString("Damage zone");
	m_actionType.AddString("Safe zone");
	m_actionType.AddString("Safe zone/Rebirth");
	m_actionType.SetCurSel(0);

	m_actionZoneDB = new CActionZoneDB();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CActionZonesDlg::OnLbnSelchangeListChannels()
{
	/*int channelSelect = CWorldChannelList.GetCurSel();
	if(channelSelect < 0)
	{
       return;
	}*/
	//InListChannels(CWorldChannelList.GetCurSel());
	InListZones(CActionZoneList.GetCurSel());
}

void CActionZonesDlg::OnLbnSelchangeListZonelist()
{
	int zoneSelect = CActionZoneList.GetCurSel();
	if(zoneSelect < 0)
	{
       return;
	}
	int channelSelect = CWorldChannelList.GetCurSel();
	if(channelSelect < 0)
	{
       return;
	}

    CActionZone * acZone = m_actionZoneDB->GetByIndex(channelSelect);
	if(!acZone)
	    return;
    
	POSITION posLoc = acZone->m_boxList.FindIndex(zoneSelect);
	if(!posLoc)
		return;
	
	CActionBox * acBox = (CActionBox *)acZone->m_boxList.GetAt(posLoc);

	UpdateData(TRUE);
	m_minX = acBox->boxMin.x;
	m_minY = acBox->boxMin.y;
	m_minZ = acBox->boxMin.z;
	m_maxX = acBox->boxMax.x;
	m_maxY = acBox->boxMax.y;
	m_maxZ = acBox->boxMax.z;
	m_zoneName = acBox->m_zoneLabel;
	m_actionValue = acBox->m_actionValue;
	m_attributeValue = acBox->m_actionAttrib;
	m_actionType.SetCurSel(acBox->m_action);
    UpdateData(FALSE);

	
	
}

void CActionZonesDlg::OnBnClickedButtonLoaddb()
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

   static char BASED_CODE filter[] = "Action Zone Files|*.ACZ|All Files (*.*)|*.*||";
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
       if(m_actionZoneDB)
	   delete m_actionZoneDB;
	   m_actionZoneDB = 0;
	   the_in_Archive >> m_actionZoneDB;
	   the_in_Archive.Close();
       fl.Close();
	   m_actionZoneDB->CompileDirectAccess();
	 }
     SetCurrentDirectory(MainDir);
	 InListChannels(0);
}

void CActionZonesDlg::OnBnClickedButtonSavedb()
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
     static char BASED_CODE filter[] = "Action Zone Files|*.ACZ|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "ACZ", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_actionZoneDB;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}
