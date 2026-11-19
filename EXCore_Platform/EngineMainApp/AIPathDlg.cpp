// AIPathDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "AIPathDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAIPathDlg dialog


CAIPathDlg::CAIPathDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAIPathDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAIPathDlg)
	m_locationIndex = 0;
	m_nodeCount = 0;
	m_webName = _T("");
	m_duration = 0;
	m_locationName = _T("");
	m_xPos = 0.0f;
	m_yPos = 0.0f;
	m_zPos = 0.0f;
	//}}AFX_DATA_INIT
}


void CAIPathDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAIPathDlg)
	DDX_Control(pDX, IDC_LIST_pathList, m_pathList);
	DDX_Control(pDX, IDC_LIST_LocationList, m_locationList);
	DDX_Text(pDX, IDC_EDIT_LocationIndex, m_locationIndex);
	DDX_Text(pDX, IDC_EDIT_NodeCount, m_nodeCount);
	DDX_Text(pDX, IDC_EDIT_webName, m_webName);
	DDX_Text(pDX, IDC_EDIT_Duration, m_duration);
	DDX_Text(pDX, IDC_EDIT_locationName, m_locationName);
	DDX_Text(pDX, IDC_EDIT_Xpos, m_xPos);
	DDX_Text(pDX, IDC_EDIT_Ypos, m_yPos);
	DDX_Text(pDX, IDC_EDIT_Zpos, m_zPos);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAIPathDlg, CDialog)
	//{{AFX_MSG_MAP(CAIPathDlg)
	ON_BN_CLICKED(IDC_BTN_QuerryPath, OnBTNQuerryPath)
	ON_BN_CLICKED(IDC_BUTTON_RebuildPath, OnBUTTONRebuildPath)
	ON_BN_CLICKED(IDC_BUTTON_CompileAll, OnBUTTONCompileAll)
	ON_BN_CLICKED(IDC_BUTTON_SetLocationName, OnBUTTONSetLocationName)
	ON_LBN_SELCHANGE(IDC_LIST_LocationList, OnSelchangeLISTLocationList)
	ON_LBN_DBLCLK(IDC_LIST_LocationList, OnDblclkLISTLocationList)
	ON_BN_CLICKED(IDC_BUTTON_IntegrityCheck, OnBUTTONIntegrityCheck)
	ON_BN_CLICKED(IDC_BUTTON_addNodeByPosition, OnBUTTONaddNodeByPosition)
	ON_BN_CLICKED(IDC_BUTTON_DeleteNode, OnBUTTONDeleteNode)
	ON_BN_CLICKED(IDC_BTN_FinalizeManualTreeBuild, OnBTNFinalizeManualTreeBuild)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAIPathDlg message handlers

BOOL CAIPathDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	
    UpdateData(TRUE);
    m_duration = 600;
	m_locationList.ResetContent();
	m_webName = m_aiObject->m_name;
	m_nodeCount = m_aiObject->m_web->GetCount();
	

    UpdateData(FALSE);
	InListLocations(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAIPathDlg::InListLocations(int sel)
{
UpdateData(TRUE);
  m_locationList.ResetContent();
   int trace = 0;
	for(POSITION posLoc = m_aiObject->m_web->GetHeadPosition();posLoc != NULL;)
	{
        CAIWebNode * wNode = (CAIWebNode *)m_aiObject->m_web->GetNext(posLoc);
		CString name = wNode->m_locationName;
		name = operator+(":",name);
		name = operator+(m_cabLib.ConvToString(trace),name);
		
		m_locationList.AddString(name);
        trace++;
	}
	if(sel > -1)
	m_locationList.SetCurSel(sel);
UpdateData(FALSE);
}

void CAIPathDlg::OnBTNQuerryPath() 
{
  int locationIndex = m_locationList.GetCurSel();
  if(locationIndex == -1)
  {
     MessageBox("Must Select a location");
     return;
  }

  InListRoute(locationIndex,m_locationIndex);
}

void CAIPathDlg::InListRoute(int startLocation,int endLocation)
{
  UpdateData(TRUE);
  m_pathList.ResetContent();
	POSITION posLoc = m_aiObject->m_web->FindIndex(startLocation);
	if(posLoc)
	{
	   CAIWebNode * wNode = (CAIWebNode *)m_aiObject->m_web->GetAt(posLoc);
	   POSITION lPos = wNode->m_routeToAllFromLocationList->FindIndex(endLocation);
	   if(lPos)
	   {
           CAIRouteToObj * aiRoute = (CAIRouteToObj *)wNode->m_routeToAllFromLocationList->GetAt(lPos);
           
           for(int loop=0;loop<aiRoute->m_mapNodeCount;loop++)
		   {
		       CString name = m_cabLib.ConvToString(aiRoute->m_map[loop]);
               
			   m_pathList.AddString(name);
		   }
	   }
	}
  UpdateData(FALSE);
}



void CAIPathDlg::OnBUTTONRebuildPath() 
{
   int locationIndex = m_locationList.GetCurSel();
  if(locationIndex == -1)
  {
     MessageBox("Must Select a location");
     return;
  }

  //m_locationIndex
  UpdateData(TRUE);
  m_pathList.ResetContent();
	POSITION posLoc = m_aiObject->m_web->FindIndex(locationIndex);
	if(posLoc)
	{
	   CAIWebNode * wNode = (CAIWebNode *)m_aiObject->m_web->GetAt(posLoc);

       

	   POSITION lPos = wNode->m_routeToAllFromLocationList->FindIndex(m_locationIndex);
	   if(lPos)
	   {
           CAIRouteToObj * aiRoute = (CAIRouteToObj *)wNode->m_routeToAllFromLocationList->GetAt(lPos);
           
           POSITION targetLoc = m_aiObject->m_web->FindIndex(m_locationIndex);
	       if(targetLoc)
		   {
	          CAIWebNode * destNode = (CAIWebNode *)m_aiObject->m_web->GetAt(targetLoc);

              m_aiObject->m_web->CompileNode(wNode,m_duration,destNode,aiRoute);
			  UpdateData(FALSE);
			  InListRoute(locationIndex,m_locationIndex);
		   }
		   else
			   AfxMessageBox("Error :: Target Does not exist");
	   }
	}
  UpdateData(FALSE);
}

void CAIPathDlg::OnBUTTONCompileAll() 
{
	UpdateData(TRUE);
	m_aiObject->m_web->CompileAll(m_duration);
	UpdateData(FALSE);
}

void CAIPathDlg::OnBUTTONSetLocationName() 
{
	int locationIndex = m_locationList.GetCurSel();
  if(locationIndex == -1)
  {
     MessageBox("Must Select a location");
     return;
  }

  POSITION posLoc = m_aiObject->m_web->FindIndex(locationIndex);
	if(!posLoc)
		return;
	
	   CAIWebNode * wNode = (CAIWebNode *)m_aiObject->m_web->GetAt(posLoc);
	   UpdateData(TRUE);
	   wNode->m_locationName = m_locationName;
	   UpdateData(FALSE);
	   InListLocations(locationIndex);
}

void CAIPathDlg::OnSelchangeLISTLocationList() 
{
  int locationIndex = m_locationList.GetCurSel();
  if(locationIndex == -1)
  {
     return;
  }

  POSITION posLoc = m_aiObject->m_web->FindIndex(locationIndex);
	if(!posLoc)
		return;
	
	   CAIWebNode * wNode = (CAIWebNode *)m_aiObject->m_web->GetAt(posLoc);
	   UpdateData(TRUE);
	   m_locationName = wNode->m_locationName;
	   m_xPos = wNode->m_position.x;
	   m_yPos = wNode->m_position.y;
	   m_zPos = wNode->m_position.z;
	   UpdateData(FALSE);
	
}

void CAIPathDlg::OnDblclkLISTLocationList() 
{
     int locationIndex = m_locationList.GetCurSel();
     if(locationIndex == -1)
	 {
     return;
	 }

  InListRoute(locationIndex,m_locationIndex);
}

void CAIPathDlg::OnBUTTONIntegrityCheck() 
{
	// TODO: Add your control notification handler code here
	
}

void CAIPathDlg::OnBUTTONaddNodeByPosition() 
{
  int index = m_aiObject->m_web->GetCount();
  CAIWebNode * wNode = new CAIWebNode(); 

  wNode->m_position.x = m_charCurrentPosition.x;
  wNode->m_position.y = m_charCurrentPosition.y;
  wNode->m_position.z = m_charCurrentPosition.z;
  //set dependancies
  wNode->m_directLinkCount = 2;
  wNode->m_directLinks = new int[wNode->m_directLinkCount];
  wNode->m_directLinks[0] = index-1;
  wNode->m_directLinks[1] = index+1;
  wNode->m_locationName = "Manual Node";

  m_aiObject->m_web->AddTail(wNode);
  InListLocations(m_aiObject->m_web->GetCount()-1);
}

void CAIPathDlg::OnBUTTONDeleteNode() 
{
	int locationIndex = m_locationList.GetCurSel();
	if(locationIndex == -1)
		return;

	POSITION posLoc = m_aiObject->m_web->FindIndex(locationIndex);
	if(!posLoc)
		return;

	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CAIWebNode * wNode = (CAIWebNode *)m_aiObject->m_web->GetAt(posLoc);
	wNode->SafeDelete();
	delete wNode;
	wNode = 0;
	m_aiObject->m_web->RemoveAt(posLoc);
	
	InListLocations(locationIndex-1);
}

void CAIPathDlg::OnBTNFinalizeManualTreeBuild() 
{
  int locationIndex = m_locationList.GetCurSel();
  if(locationIndex == -1)
  {
     MessageBox("Must Select a location");
     return;
  }
  if(m_aiObject->m_web->GetCount() < 2)
  {
     MessageBox("Not enough nodes");
     return;
  }

  //m_locationIndex
  CAIWebNode * wNodeHead = (CAIWebNode *)m_aiObject->m_web->GetHead();
  wNodeHead->m_directLinks[0] = m_aiObject->m_web->GetCount()-1;
  CAIWebNode * wNodeTail = (CAIWebNode *)m_aiObject->m_web->GetTail();
  wNodeTail->m_directLinks[1] = 0;


  m_aiObject->m_web->CompileAll(100);
	
}

void CAIPathDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CAIPathDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
