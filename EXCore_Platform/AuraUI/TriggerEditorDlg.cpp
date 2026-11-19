// TriggerEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#define D3D_OVERLOADS
#include "resource.h"
#include <d3d9.h>
#include "direct.h"
#include "TriggerEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTriggerEditorDlg dialog


CTriggerEditorDlg::CTriggerEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTriggerEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTriggerEditorDlg)
	m_dataRadius = 0.0f;
	m_miscInt = 0;
	m_pullDownActions = _T("");
	m_miscTest = _T("");
	m_miscVectX = 0.0f;
	m_miscVectY = 0.0f;
	m_miscVectZ = 0.0f;
	//}}AFX_DATA_INIT
}


void CTriggerEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTriggerEditorDlg)
	DDX_Control(pDX, IDC_LIST_TriggerList, m_triggerDatabase);
	DDX_Text(pDX, IDC_EDIT_Radius, m_dataRadius);
	DDX_Text(pDX, IDC_EDIT_MISCINT, m_miscInt);
	DDX_CBString(pDX, IDC_COMBO_Actions, m_pullDownActions);
	DDX_Text(pDX, IDC_EDIT_MiscTxt, m_miscTest);
	DDX_Text(pDX, IDC_EDIT_MISCVECTX, m_miscVectX);
	DDX_Text(pDX, IDC_EDIT_MISCVECTY, m_miscVectY);
	DDX_Text(pDX, IDC_EDIT_MISCVECTZ, m_miscVectZ);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_oneTimeOnly, m_doOnlyOnce);
}


BEGIN_MESSAGE_MAP(CTriggerEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CTriggerEditorDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADDTRIGGER, OnButtonAddtrigger)
	ON_BN_CLICKED(IDC_BUTTON_DeleteTRIGGER, OnBUTTONDeleteTRIGGER)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceTRIGGER, OnBUTTONReplaceTRIGGER)
	ON_BN_CLICKED(IDC_BUTTON_BROWSEfile, OnBUTTONBROWSEfile)
	ON_LBN_SELCHANGE(IDC_LIST_TriggerList, OnSelchangeLISTTriggerList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTriggerEditorDlg message handlers

BOOL CTriggerEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InListTriggers();
	



	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTriggerEditorDlg::InListTriggers() 
{
    UpdateData(TRUE);
	m_triggerDatabase.ResetContent();
	for(POSITION posLoc = GL_worldObject->List_triggerList->GetHeadPosition();posLoc != NULL;){
            CTriggerObject * trgPtr = (CTriggerObject *)GL_worldObject->List_triggerList->GetNext(posLoc);
            m_triggerDatabase.AddString(operator+("Trigger :: ",ConvertLongToString(trgPtr->m_action)));
	}
	UpdateData(FALSE);
}

void CTriggerEditorDlg::OnButtonAddtrigger() 
{
	UpdateData(TRUE);
    int action=0;
	if(m_pullDownActions == "Activate Animation")
	   action=1;
	if(m_pullDownActions == "Local Portal")
	   action=2;
	if(m_pullDownActions == "External Portal")
	   action=3;
	if(m_pullDownActions == "Play Movie Interlaced")
	   action=4;
	if(m_pullDownActions == "Force Pad")
	   action=5;
	if(m_pullDownActions == "Play SoundTrack")
	   action=6;
	if(m_pullDownActions == "Text Output")
       action=7;

    D3DVECTOR tempVect;
	tempVect.x = m_miscVectX;
	tempVect.y = m_miscVectY;
	tempVect.z = m_miscVectZ;


    GL_worldObject->AddTrigger(action,m_miscInt,m_dataRadius,m_miscTest,tempVect,m_doOnlyOnce.GetCheck());
	UpdateData(FALSE);
	InListTriggers();
}

void CTriggerEditorDlg::OnBUTTONDeleteTRIGGER() 
{
	UpdateData(TRUE);
	int listIndex = m_triggerDatabase.GetCurSel();
	if(listIndex !=-1)
	{
       if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	   
	   GL_worldObject->DeleteTrigger(listIndex);
	   InListTriggers();
	}
	else
		MessageBox("must select a trigger");
	UpdateData(FALSE);
}

void CTriggerEditorDlg::OnBUTTONReplaceTRIGGER() 
{
	UpdateData(TRUE);
	int listIndex = m_triggerDatabase.GetCurSel();
	if(listIndex !=-1){
	   int action=0;
	   if(m_pullDownActions == "Activate Animation")
	   action=1;
	   if(m_pullDownActions == "Local Portal")
	   action=2;
	   if(m_pullDownActions == "External Portal")
	   action=3;
	   if(m_pullDownActions == "Play Movie Interlaced")
	   action=4;
	   if(m_pullDownActions == "Force Pad")
	   action=5;
	   if(m_pullDownActions == "Play SoundTrack")
	   action=6;
	   if(m_pullDownActions == "Text Output")
       action=7;

       D3DVECTOR tempVect;
	   tempVect.x = m_miscVectX;
	   tempVect.y = m_miscVectY;
	   tempVect.z = m_miscVectZ;

       GL_worldObject->EditTrigger(listIndex,action,m_miscInt,m_dataRadius,m_miscTest,tempVect,m_doOnlyOnce.GetCheck());
	   InListTriggers();
	}
	else
	MessageBox("must select a trigger");
	UpdateData(FALSE);
}
//---------------
CString CTriggerEditorDlg::ConvertLongToString(int number)
{    
	          int decimal,sign;
              char *buffer;
              CString temp;	 
	          buffer = _fcvt(number,0,&decimal,&sign);
    	      temp = buffer;
			  if(sign != 0)
                 temp = operator+("-",temp);
			  return temp;
}

void CTriggerEditorDlg::OnBUTTONBROWSEfile() 
{
//use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
//end use mapsmodels folder for organization
static char BASED_CODE filter[] = "*.* Files|*.*|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   CString fileName = opendialog.GetFileName();
	   m_miscTest = fileName;
	   UpdateData(FALSE);
	 }	
	SetCurrentDirectory(MainDir);
}

void CTriggerEditorDlg::OnSelchangeLISTTriggerList() 
{
	UpdateData(TRUE);
	int listIndex = m_triggerDatabase.GetCurSel();
	if(listIndex !=-1){
	   POSITION posLoc	= GL_worldObject->List_triggerList->FindIndex(listIndex);
       CTriggerObject*trgPtr = (CTriggerObject*) GL_worldObject->List_triggerList->GetAt(posLoc);
	

       m_dataRadius = trgPtr->m_radius;
       m_miscInt = trgPtr->m_indexMisc;
       m_miscTest = trgPtr->m_miscTxt;
	   m_miscVectX = trgPtr->m_miscVector.x;
	   m_miscVectY = trgPtr->m_miscVector.y;
	   m_miscVectZ = trgPtr->m_miscVector.z;


	   m_doOnlyOnce.SetCheck(trgPtr->m_oneTimeOnly);
  
	   if(trgPtr->m_action == 1)
	   m_pullDownActions = "Activate Animation";
	   if(trgPtr->m_action == 2)
	   m_pullDownActions = "Local Portal";
	   if(trgPtr->m_action == 3)
	   m_pullDownActions = "External Portal";
       if(trgPtr->m_action == 4)
	   m_pullDownActions = "Play Movie Interlaced";
       if(trgPtr->m_action == 5)
	   m_pullDownActions = "Force Pad";
	   if(trgPtr->m_action == 6)
	   m_pullDownActions = "Play SoundTrack";
	   if(trgPtr->m_action == 7)
       m_pullDownActions = "Text Output";
	}
	UpdateData(FALSE);
}
