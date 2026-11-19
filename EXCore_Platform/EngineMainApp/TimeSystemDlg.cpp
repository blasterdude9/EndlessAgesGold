// TimeSystemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "TimeSystemDlg.h"
#include "StormDlg.h"
#include "DayStateDlg.h"
#include "CharonaDlg.h"
#include "Direct.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTimeSystemDlg dialog


CTimeSystemDlg::CTimeSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeSystemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTimeSystemDlg)
	m_daysPerMonth = 0;
	m_monthsPerYear = 0;
	//}}AFX_DATA_INIT
}


void CTimeSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTimeSystemDlg)
	DDX_Control(pDX, IDC_LIST_stormSystemsList, m_stormSystemsList);
	DDX_Control(pDX, IDC_LIST_dayStates, m_dayStatesList);
	DDX_Text(pDX, IDC_EDIT_DaysPerMonth, m_daysPerMonth);
	DDX_Text(pDX, IDC_EDIT_MonthsPerYear, m_monthsPerYear);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTimeSystemDlg, CDialog)
	//{{AFX_MSG_MAP(CTimeSystemDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADDDAYSTATE, OnButtonAdddaystate)
	ON_BN_CLICKED(IDC_BUTTON_AddStorm, OnBUTTONAddStorm)
	ON_BN_CLICKED(IDC_BUTTON_EditStorm, OnBUTTONEditStorm)
	ON_BN_CLICKED(IDC_BUTTON_DeleteStorm, OnBUTTONDeleteStorm)
	ON_BN_CLICKED(IDC_BUTTON_EDITDAYSTATE, OnButtonEditdaystate)
	ON_BN_CLICKED(IDC_BUTTON_DELETEDAYSTATE, OnButtonDeletedaystate)
	ON_BN_CLICKED(IDC_BUTTON_AddVisual, OnBUTTONAddVisual)
	ON_BN_CLICKED(IDC_BUTTON_AccessCharona, OnBUTTONAccessCharona)
	ON_BN_CLICKED(IDC_BUTTON_RemoveVisual, OnBUTTONRemoveVisual)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTimeSystemDlg message handlers
BOOL CTimeSystemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    UpdateData(TRUE);

    m_daysPerMonth  = m_enviormentObjRef->m_enviormentTimeSystem->m_daysPerMonth;
    m_monthsPerYear = m_enviormentObjRef->m_enviormentTimeSystem->m_monthsPerYear;

	UpdateData(FALSE);

	InListStorms(0);
	InListDayStates(0);
	return TRUE;  
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CTimeSystemDlg::InListDayStates(int sel)
{
	UpdateData(TRUE);
	 m_dayStatesList.ResetContent();

     if(!m_enviormentObjRef->m_enviormentTimeSystem)
		 return;

	 if(!m_enviormentObjRef->m_enviormentTimeSystem->m_dayStateList)
		 return;
 
	 for(POSITION posLoc = m_enviormentObjRef->m_enviormentTimeSystem->m_dayStateList->GetHeadPosition();posLoc != NULL;)
	 {
         CTimeSystemObj * timeSysObj = (CTimeSystemObj *) m_enviormentObjRef->m_enviormentTimeSystem->m_dayStateList->GetNext(posLoc);
         m_dayStatesList.AddString("State");
	 }
	if(sel > -1)
	m_dayStatesList.SetCurSel(sel);
	UpdateData(FALSE);
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CTimeSystemDlg::InListStorms(int sel)
{
    UpdateData(TRUE);

	if(!m_enviormentObjRef->m_enviormentTimeSystem)
		return;

	if(!m_enviormentObjRef->m_enviormentTimeSystem->m_stormDB)
		return;

    m_stormSystemsList.ResetContent();
    for(POSITION posLoc = m_enviormentObjRef->m_enviormentTimeSystem->m_stormDB->GetHeadPosition();posLoc != NULL;)
	{
         CStormObj * stormObj = (CStormObj *) m_enviormentObjRef->m_enviormentTimeSystem->m_stormDB->GetNext(posLoc);
         m_stormSystemsList.AddString(stormObj->m_stormName);
	}
    if(sel > -1)
	m_stormSystemsList.SetCurSel(sel);

    UpdateData(FALSE);
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CTimeSystemDlg::OnButtonAdddaystate() 
{
    if(!m_enviormentObjRef->m_enviormentTimeSystem->m_dayStateList)
        m_enviormentObjRef->m_enviormentTimeSystem->m_dayStateList = new CDayStateObjList();

 
	CDayStateObj * dayState = new CDayStateObj();
	
	CDayStateDlg dlg;
	dlg.m_dayStateRef = dayState;

	if(dlg.DoModal()==IDOK)
	{
       m_enviormentObjRef->m_enviormentTimeSystem->m_dayStateList->AddTail(dayState);
	}
	else{
       dayState->SafeDelete();
	   delete dayState;
	   dayState=0;
	}

	InListDayStates(m_enviormentObjRef->m_enviormentTimeSystem->m_dayStateList->GetCount()-1);
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CTimeSystemDlg::OnBUTTONAddStorm() 
{
    if(!m_enviormentObjRef->m_enviormentTimeSystem->m_stormDB)
        m_enviormentObjRef->m_enviormentTimeSystem->m_stormDB = new CStormObjList();

	CStormObj * stormPtr = new CStormObj();
	CStormDlg dlg;
	
	dlg.m_textureDataBaseRef = m_textureDataBaseRef;
	dlg.m_stormRef           = stormPtr;

	if(dlg.DoModal()==IDOK)
	{
       m_enviormentObjRef->m_enviormentTimeSystem->m_stormDB->AddTail(stormPtr);
	}
	else
	{
       stormPtr->SafeDelete();
	   delete stormPtr;
	   stormPtr=0;
	}

	InListStorms((m_enviormentObjRef->m_enviormentTimeSystem->m_stormDB->GetCount()-1));


}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CTimeSystemDlg::OnBUTTONEditStorm() 
{
    int stormSelect = m_stormSystemsList.GetCurSel();
	if(stormSelect == -1)
	{
       MessageBox("Must select a storm");
       return;
	}

	   POSITION stormPos = m_enviormentObjRef->m_enviormentTimeSystem->m_stormDB->FindIndex(stormSelect);
       if(!stormPos)
		   return;
           CStormObj * stormPtr = ( CStormObj * )m_enviormentObjRef->m_enviormentTimeSystem->m_stormDB->GetAt(stormPos);

           CStormDlg dlg;
           dlg.m_textureDataBaseRef = m_textureDataBaseRef;
           dlg.m_stormRef           = stormPtr;

	       if(dlg.DoModal()==IDOK)
		   {
              InListStorms(stormSelect);
		   }	
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CTimeSystemDlg::OnOK() 
{
	UpdateData(TRUE);

    m_enviormentObjRef->m_enviormentTimeSystem->m_daysPerMonth    = m_daysPerMonth;
    m_enviormentObjRef->m_enviormentTimeSystem->m_monthsPerYear   = m_monthsPerYear;

	UpdateData(FALSE);
	
	CDialog::OnOK();
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CTimeSystemDlg::OnBUTTONDeleteStorm() 
{
	int stormSelect = m_stormSystemsList.GetCurSel();
	if(stormSelect == -1)
	{
		MessageBox("Must select a storm");
		return;
	}

	POSITION stormPos = m_enviormentObjRef->m_enviormentTimeSystem->m_stormDB->FindIndex(stormSelect);
	if(!stormPos)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CStormObj * stormPtr = ( CStormObj * )m_enviormentObjRef->m_enviormentTimeSystem->m_stormDB->GetAt(stormPos);
	stormPtr->SafeDelete();
	delete stormPtr;
	stormPtr = 0;
	m_enviormentObjRef->m_enviormentTimeSystem->m_stormDB->RemoveAt(stormPos);

	InListStorms(stormSelect-1);
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CTimeSystemDlg::OnButtonEditdaystate() 
{
	if(!m_enviormentObjRef->m_enviormentTimeSystem->m_dayStateList)
		return;
	
	int dayStateSelection = m_dayStatesList.GetCurSel();
	if(dayStateSelection == -1)
	{
       MessageBox("must select a day state");
	   return;
	}

	POSITION statePos = m_enviormentObjRef->m_enviormentTimeSystem->m_dayStateList->FindIndex(dayStateSelection);
    if(!statePos)
		return;
	    CDayStateObj * dayState = (CDayStateObj *)m_enviormentObjRef->m_enviormentTimeSystem->m_dayStateList->GetAt(statePos);
	    CDayStateDlg dlg;
		dlg.m_dayStateRef = dayState;

	    if(dlg.DoModal()==IDOK)
		{
           InListDayStates(dayStateSelection);
		}
	
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CTimeSystemDlg::OnButtonDeletedaystate() 
{
	if(!m_enviormentObjRef->m_enviormentTimeSystem->m_dayStateList)
		return;

	int dayStateSelection = m_dayStatesList.GetCurSel();
	if(dayStateSelection == -1)
	{
		MessageBox("must select a day state");
		return;
	}

	POSITION statePos = m_enviormentObjRef->m_enviormentTimeSystem->m_dayStateList->FindIndex(dayStateSelection);
	if(!statePos)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CDayStateObj * dayState = (CDayStateObj *)m_enviormentObjRef->m_enviormentTimeSystem->m_dayStateList->GetAt(statePos);
	dayState->SafeDelete();
	delete dayState;
	dayState=0;
	m_enviormentObjRef->m_enviormentTimeSystem->m_dayStateList->RemoveAt(statePos);

	InListDayStates(m_enviormentObjRef->m_enviormentTimeSystem->m_dayStateList->GetCount()-1);
}

void CTimeSystemDlg::OnBUTTONAddVisual() 
{
	 
     //use mapsmodels folder for organization
     CString CurDir;
     CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
     //end use mapsmodels folder for organization
     static char BASED_CODE filter[] = "Engine Models|*.arw;*.x;*.3ds;*.fbx;*.obj;*.gltf;*.glb;*.dae;*.stl;*.ply;*.blend;*.ase;*.lwo;*.dxf|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() != IDOK )
		  return;
     CString fileName = opendialog.GetPathName();
	 SetCurrentDirectory(MainDir);

	 if(m_enviormentObjRef->m_enviormentTimeSystem->m_visual)
	 {
        m_enviormentObjRef->m_enviormentTimeSystem->m_visual->SafeDelete();
		delete m_enviormentObjRef->m_enviormentTimeSystem->m_visual;
		m_enviormentObjRef->m_enviormentTimeSystem->m_visual = 0;
	 }

	 

	 m_enviormentObjRef->m_enviormentTimeSystem->m_visual = new CEXMeshObj();
	 m_enviormentObjRef->m_enviormentTimeSystem->m_visual->List_materialObject = new CMaterialObject();
	 float range = 0.0f;
     m_enviormentObjRef->m_enviormentTimeSystem->m_visual->LoadGeometryFromPlugin(fileName,&range,g_pd3dDeviceRef,NULL);
	 
	 
	 m_enviormentObjRef->m_enviormentTimeSystem->m_visual->List_materialObject->m_baseMaterial.Emissive.r = 1.0f;
     m_enviormentObjRef->m_enviormentTimeSystem->m_visual->List_materialObject->m_baseMaterial.Emissive.g = 1.0f;
     m_enviormentObjRef->m_enviormentTimeSystem->m_visual->List_materialObject->m_baseMaterial.Emissive.b = 1.0f;
     m_enviormentObjRef->m_enviormentTimeSystem->m_visual->List_materialObject->m_baseMaterial.Emissive.a = 1.0f;
     m_enviormentObjRef->m_enviormentTimeSystem->m_visual->List_materialObject->Reset();
	
}

void CTimeSystemDlg::OnBUTTONAccessCharona() 
{
	if(!m_enviormentObjRef->m_enviormentTimeSystem->m_visual)
	{
        MessageBox("Must create a sun visual");
        return;
	}

    CCharonaDlg dlg;
	dlg.m_charonaRef     = m_enviormentObjRef->m_enviormentTimeSystem->m_visual->m_charona;
	dlg.m_textureDataBase = m_textureDataBaseRef;
    dlg.DoModal();
	m_enviormentObjRef->m_enviormentTimeSystem->m_visual->m_charona = dlg.m_charonaRef;
	if(m_enviormentObjRef->m_enviormentTimeSystem->m_visual->m_charona){
	   m_enviormentObjRef->m_enviormentTimeSystem->m_visual->m_charona->InitializeVis();
	   m_enviormentObjRef->m_enviormentTimeSystem->m_visual->m_charona->m_visual->InitBuffer(g_pd3dDeviceRef,m_enviormentObjRef->m_enviormentTimeSystem->m_visual->m_charona->m_visual->m_uvCount);
	}
}

void CTimeSystemDlg::OnBUTTONRemoveVisual() 
{
	if(m_enviormentObjRef->m_enviormentTimeSystem->m_visual)
	{
        m_enviormentObjRef->m_enviormentTimeSystem->m_visual->SafeDelete();
		delete m_enviormentObjRef->m_enviormentTimeSystem->m_visual;
		m_enviormentObjRef->m_enviormentTimeSystem->m_visual = 0;
		AfxMessageBox("Removed");
	}
	else{
        AfxMessageBox("no valid sun visual");
	}
	 
	
}
