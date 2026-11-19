// MenuClickZoneDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "MenuClickZoneDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuClickZoneDlg dialog


CMenuClickZoneDlg::CMenuClickZoneDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMenuClickZoneDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMenuClickZoneDlg)
	m_centerX = 0.0f;
	m_centerY = 0.0f;
	m_centerZ = 0.0f;
	m_maxX = 0.0f;
	m_maxY = 0.0f;
	m_minX = 0.0f;
	m_minY = 0.0f;
	m_miscInt = 0;
	m_miscInt2 = 0;
	m_radius = 0.0f;
	m_miscString = _T("");
	m_miscInt3 = 0;
	m_staticMiscInt = _T("");
	m_staticMiscInt2 = _T("");
	m_staticMiscString = _T("");
	m_staticMiscInt3 = _T("");
	//}}AFX_DATA_INIT
}


void CMenuClickZoneDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMenuClickZoneDlg)
	DDX_Control(pDX, IDC_CHECK_ExecOnClosing, m_execOnClosing);
	DDX_Control(pDX, IDC_CHECK_RadiusType, m_radioRadiusType);
	DDX_Control(pDX, IDC_COMBO_Actions, m_actions);
	DDX_Control(pDX, IDC_COMBO_Visuals, m_visualsCombo);
	DDX_Text(pDX, IDC_EDIT_CenterX, m_centerX);
	DDX_Text(pDX, IDC_EDIT_CenterY, m_centerY);
	DDX_Text(pDX, IDC_EDIT_CenterZ, m_centerZ);
	DDX_Text(pDX, IDC_EDIT_menu_MaxX, m_maxX);
	DDX_Text(pDX, IDC_EDIT_menu_MaxY, m_maxY);
	DDX_Text(pDX, IDC_EDIT_menu_MinX, m_minX);
	DDX_Text(pDX, IDC_EDIT_menu_MinY, m_minY);
	DDX_Text(pDX, IDC_EDIT_MenuMiscInt, m_miscInt);
	DDX_Text(pDX, IDC_EDIT_menuMiscInt2, m_miscInt2);
	DDX_Text(pDX, IDC_EDIT_Radius, m_radius);
	DDX_Text(pDX, IDC_EDIT_miscString, m_miscString);
	DDX_Text(pDX, IDC_EDIT_menuMiscInt3, m_miscInt3);
	DDX_Text(pDX, IDC_STATIC_miscInt, m_staticMiscInt);
	DDX_Text(pDX, IDC_STATIC_miscInt2, m_staticMiscInt2);
	DDX_Text(pDX, IDC_STATIC_MiscString, m_staticMiscString);
	DDX_Text(pDX, IDC_STATIC_miscInt3, m_staticMiscInt3);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_DropZone, m_dropZoneCheck);
	DDX_Control(pDX, IDC_CHECK_DefaultZone, m_defaultZone);
}


BEGIN_MESSAGE_MAP(CMenuClickZoneDlg, CDialog)
	//{{AFX_MSG_MAP(CMenuClickZoneDlg)
	ON_BN_CLICKED(IDC_CHECK_RadiusType, OnCHECKRadiusType)
	ON_CBN_SELCHANGE(IDC_COMBO_Actions, OnSelchangeCOMBOActions)
	ON_BN_CLICKED(IDC_CHECK_ExecOnClosing, OnCHECKExecOnClosing)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_DropZone, OnBnClickedCheckDropzone)
	ON_EN_CHANGE(IDC_EDIT_MenuMiscInt, &CMenuClickZoneDlg::OnEnChangeEditMenumiscint)
	ON_BN_CLICKED(IDC_CHECK_DefaultZone, &CMenuClickZoneDlg::OnBnClickedCheckDefaultzone)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuClickZoneDlg message handlers

BOOL CMenuClickZoneDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);
	  m_visualsCombo.AddString("NONE");
	  for(POSITION posLoc = menu3DObjRef->m_visualItems->GetHeadPosition();posLoc != NULL;)
	  {
          CMenuVisObj* mObj = (CMenuVisObj*) menu3DObjRef->m_visualItems->GetNext(posLoc);
          m_visualsCombo.AddString(mObj->m_visualName);
	  }
	m_visualsCombo.SetCurSel(clickZoneRef->m_visualIndex+1);
	m_actions.SetCurSel(clickZoneRef->m_action);
	
	if(clickZoneRef->m_radiusType == TRUE)
	   m_radioRadiusType.SetCheck(1);

	if(clickZoneRef->m_dropZone == TRUE)
	   m_dropZoneCheck.SetCheck(1);

	m_radius     = clickZoneRef->m_radius;
	m_centerX    = clickZoneRef->m_center.x;
	m_centerY    = clickZoneRef->m_center.y;
	m_centerZ    = clickZoneRef->m_center.z;
	m_maxX       = clickZoneRef->m_rectMaxX;
	m_maxY       = clickZoneRef->m_rectMaxY;
	m_minX       = clickZoneRef->m_rectMinX;
    m_minY       = clickZoneRef->m_rectMinY;
	m_miscInt    = clickZoneRef->m_miscInt;
	m_miscInt2   = clickZoneRef->m_miscInt2;
	m_miscInt3   = clickZoneRef->m_miscInt3;
	m_miscString = clickZoneRef->m_miscString;


	if(clickZoneRef->m_execOnCloseing)
	   m_execOnClosing.SetCheck(1);

    if(clickZoneRef->m_default)
	   m_defaultZone.SetCheck(1);

	UpdateData(FALSE);
	TextInterperate(clickZoneRef->m_action);
	   return TRUE;  // return TRUE unless you set the focus to a control
	                 // EXCEPTION: OCX Property Pages should return FALSE
}

void CMenuClickZoneDlg::OnCHECKRadiusType() 
{
	if(clickZoneRef->m_radiusType)
       clickZoneRef->m_radiusType = FALSE;
	else
       clickZoneRef->m_radiusType = TRUE;
}

void CMenuClickZoneDlg::OnOK() 
{
	UpdateData(TRUE);
    clickZoneRef->m_visualIndex   = m_visualsCombo.GetCurSel()-1;
	clickZoneRef->m_action        = m_actions.GetCurSel();
	clickZoneRef->m_radius        = m_radius;
	clickZoneRef->m_center.x      = m_centerX;
	clickZoneRef->m_center.y      = m_centerY;
	clickZoneRef->m_center.z      = m_centerZ;
	clickZoneRef->m_rectMaxX      = m_maxX;
	clickZoneRef->m_rectMaxY      = m_maxY;
	clickZoneRef->m_rectMinX      = m_minX;
    clickZoneRef->m_rectMinY      = m_minY;
	clickZoneRef->m_miscInt       = m_miscInt;
	clickZoneRef->m_miscInt2      = m_miscInt2;
	clickZoneRef->m_miscInt3      = m_miscInt3;
	clickZoneRef->m_miscString    = m_miscString;
	clickZoneRef->m_default       = m_defaultZone.GetCheck();
	UpdateData(FALSE);
	
	CDialog::OnOK();
}

void CMenuClickZoneDlg::OnSelchangeCOMBOActions() 
{
	int sel = m_actions.GetCurSel();
	if(sel < 0)
	   return;

	TextInterperate(sel);
	
}


void CMenuClickZoneDlg::TextInterperate(int sel)
{
    UpdateData(TRUE);
	m_staticMiscInt      = "Not Used";
	m_staticMiscInt2     = "Not Used";
	m_staticMiscString   = "Not Used";
	m_staticMiscInt3     = "Not Used";

    if(sel == 5){
       m_staticMiscInt      = "Spawn Position X";
	   m_staticMiscInt2     = "Spawn Position Y";
	   m_staticMiscInt3     = "Spawn Position Z";
	   m_staticMiscString   = "World File";	   
	}
	if(sel == 11){
       m_staticMiscInt      = "Entity Visual ID";
	   m_staticMiscInt2     = "Item Location ID";
	   
	}
    if(sel == 12){
       m_staticMiscInt      = "Name Visual ID";
	   m_staticMiscInt2     = "Entity Visual ID";
	   m_staticMiscInt3     = "Open Menu";
	   m_staticMiscString   = "Spawn Index";
	}
	if(sel == 13){
       m_staticMiscInt      = "Name Visual ID";
	   m_staticMiscInt2     = "Dim Visual Index";
	}
	if(sel == 15){
       m_staticMiscInt      = "Slider Visual ID";
	}
	if(sel == 16){
       m_staticMiscInt      = "Slider Visual ID";
	}
	if(sel == 17){
       m_staticMiscInt      = "Store Inventory Visual ID";
	   m_staticMiscInt2     = "Your Inventory Visual ID";
	}
	if(sel == 18){
       m_staticMiscInt      = "your Inventory Visual ID";
	}
	if(sel == 19){
       m_staticMiscInt      = "Victims Inventory Visual ID";
	}
	if(sel == 20){
       m_staticMiscInt       = "Players Inventory Visual ID";
	   m_staticMiscInt2      = "Bank Inventory";
	}
	if(sel == 21){
       m_staticMiscInt       = "Bank Inventory";
	   m_staticMiscInt2      = "Players Inventory Visual ID";
	}
	if(sel == 22){
       m_staticMiscInt       = "Players Inventory";
	}

	if(sel == 24){
       m_staticMiscInt       = "Slider System ID";
	}

	if(sel == 25){
       m_staticMiscInt       = "Slider System ID";
	}

	if(sel == 26){
       m_staticMiscInt        = "Offer List";
	   m_staticMiscInt2       = "Cash Offer";
	}
	if(sel == 27){
       m_staticMiscInt        = "List Source";
	   m_staticMiscInt2       = "List Destination";
	}
    if(sel == 30){
       m_staticMiscInt        = "Visual ID";
	}
	if(sel == 31){
       m_staticMiscInt        = "Visual ID";
	}
    if(sel == 32){
       m_staticMiscInt      = "Action List ID";
	   m_staticMiscInt2     = "Input List ID";
	   m_staticMiscInt3     = "Inventory Glid List";
	}
	if(sel == 35){
       m_staticMiscInt      = "Action List ID";
	   m_staticMiscInt2     = "Input List ID";
	   m_staticMiscInt3     = "Inventory Glid List";
	}
	if(sel == 36){
       m_staticMiscString   = "Server Command";
	}
	if(sel == 39){
       m_staticMiscInt      = "Group List";
	}
    if(sel == 40){
       m_staticMiscInt      = "List ID of area players";
	}
    if(sel == 45){
	   m_staticMiscInt      = "Ability Selection";
	   m_staticMiscInt2     = "Skill Level";
	   m_staticMiscInt3     = "Skill Type";
	}
	if(sel == 50){
	   m_staticMiscInt        = "Inventory List ID";
	   m_staticMiscInt2     = "Store List ID";
	   m_staticMiscInt3     = "Quanity ID";
	   m_staticMiscString = "Set Cost ID";
	}
    if(sel == 51){
	   m_staticMiscInt        = "Inventory List ID";
	   m_staticMiscInt2     = "Store List ID";
	   m_staticMiscInt3     = "Quanity ID";
	}
    if(sel == 52){
	   m_staticMiscInt        = "Current Cash Link ID";

	}
	if(sel == 53){
	   m_staticMiscInt      = "Inventory List ID";
	   m_staticMiscInt2     = "Store List ID";
	   m_staticMiscInt3     = "Quanity ID";
	}
	if(sel == 54){
	   m_staticMiscInt      = "Action List ID";
	   m_staticMiscInt2     = "Inventory List ID";
	   m_staticMiscInt3     = "status output Txt ID";
	}


	UpdateData(FALSE);
}


void CMenuClickZoneDlg::OnCHECKExecOnClosing() 
{
	if(clickZoneRef->m_execOnCloseing)
	   clickZoneRef->m_execOnCloseing = FALSE;
	else
	   clickZoneRef->m_execOnCloseing = TRUE;
	
}

void CMenuClickZoneDlg::OnBnClickedCheckDropzone()
{
	if(clickZoneRef->m_dropZone)
	   clickZoneRef->m_dropZone = FALSE;
	else
	   clickZoneRef->m_dropZone = TRUE;
}

void CMenuClickZoneDlg::OnEnChangeEditMenumiscint()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
}

void CMenuClickZoneDlg::OnBnClickedCheckDefaultzone()
{
	// TODO: Add your control notification handler code here
}
