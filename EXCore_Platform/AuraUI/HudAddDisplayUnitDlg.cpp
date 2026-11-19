// HudAddDisplayUnitDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "HudAddDisplayUnitDlg.h"
#include "direct.h"
#include "ColorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHudAddDisplayUnitDlg dialog


CHudAddDisplayUnitDlg::CHudAddDisplayUnitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CHudAddDisplayUnitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHudAddDisplayUnitDlg)
	m_posX = 0;
	m_posY = 0;
	m_uniFloat = 0.0f;
	m_uniText = _T("");
	m_fontIndex = 0;
	m_type = _T("");
	m_uniInt = 0;
	m_lineThickness = 0;
	//}}AFX_DATA_INIT
}


void CHudAddDisplayUnitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHudAddDisplayUnitDlg)
	DDX_Text(pDX, IDC_EDIT_POSX, m_posX);
	DDX_Text(pDX, IDC_EDIT_POSY, m_posY);
	DDX_Text(pDX, IDC_EDIT_UniFloat, m_uniFloat);
	DDX_Text(pDX, IDC_EDIT_UniText, m_uniText);
	DDX_Text(pDX, IDC_EDITFontIndex, m_fontIndex);
	DDX_CBString(pDX, IDC_COMBO_Type, m_type);
	DDX_Text(pDX, IDC_EDITUniInt, m_uniInt);
	DDX_Text(pDX, IDC_EDIT_LineThickness, m_lineThickness);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHudAddDisplayUnitDlg, CDialog)
	//{{AFX_MSG_MAP(CHudAddDisplayUnitDlg)
	ON_BN_CLICKED(IDC_BUTTON_DXFBrowse, OnBUTTONDXFBrowse)
	ON_BN_CLICKED(IDC_BUTTON_RGBText, OnBUTTONRGBText)
	ON_BN_CLICKED(IDC_BUTTON_RGBVectors, OnBUTTONRGBVectors)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHudAddDisplayUnitDlg message handlers

void CHudAddDisplayUnitDlg::OnBUTTONDXFBrowse() 
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
static char BASED_CODE filter[] = "dxf Files|*.dxf|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   m_uniText = opendialog.GetPathName();
	   UpdateData(FALSE);
	 }	
SetCurrentDirectory(MainDir);	
}

void CHudAddDisplayUnitDlg::OnBUTTONRGBText() 
{
CColorDlg dlg;
 dlg.color255 = TRUE;
  dlg.m_red = (float)redText;
  dlg.m_green = (float)greenText;
  dlg.m_blue = (float)blueText;
  if(dlg.DoModal()==TRUE){
	 redText = (int)dlg.m_red;
     greenText = (int)dlg.m_green;
     blueText = (int)dlg.m_blue;
  }	
}

void CHudAddDisplayUnitDlg::OnBUTTONRGBVectors() 
{
CColorDlg dlg;
 dlg.color255 = TRUE;
  dlg.m_red = (float)redPen;
  dlg.m_green = (float)greenPen;
  dlg.m_blue = (float)bluePen;
  if(dlg.DoModal()==TRUE){
	 redPen = (int)dlg.m_red;
     greenPen = (int)dlg.m_green;
     bluePen = (int)dlg.m_blue;
  }	
}

BOOL CHudAddDisplayUnitDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);
	m_type = GetString(typeReturned);
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


int CHudAddDisplayUnitDlg::GetInt(CString nameString) 
{
	if("Static Image or Text" == nameString)
		return 0;
    if("Text Linked To Energy" == nameString)
		return 1;
    if("Radar System 2D" == nameString)
		return 2;
    if("Graphic X Energy" == nameString)
		return 3;
    if("Graphic Y Energy" == nameString)
		return 4;
    if("Fov Track" == nameString)
		return 6;
    if("Target System A" == nameString)
		return 7;
    if("Chat Tracker" == nameString)
		return 8;
	if("Base Jet Fuel Tracker" == nameString)
		return 30;
	if("Vector X jet fuel" == nameString)
		return 31;
	if("Vector Y jet fuel" == nameString)
		return 32;

return 0;
}

CString CHudAddDisplayUnitDlg::GetString(int indexLocal) 
{
	if(0 == indexLocal)
		return "Static Image or Text";
    if(1 == indexLocal)
		return "Text Linked To Energy";
    if(2 == indexLocal)
		return "Radar System 2D";
    if(3 == indexLocal)
		return "Graphic X Energy";
    if(4 == indexLocal)
		return "Graphic Y Energy";
    if(6 == indexLocal)
		return "Fov Track";
    if(7 == indexLocal)
		return "Target System A";
    if(8 == indexLocal)
		return "Chat Tracker";
    if(30 == indexLocal)
		return "Base Jet Fuel Tracker";
	if(31 == indexLocal)
		return "Vector X jet fuel";
	if(32 == indexLocal)
		return "Vector Y jet fuel";

return "NA";
}

void CHudAddDisplayUnitDlg::OnOK() 
{
	UpdateData(TRUE);
	typeReturned = GetInt(m_type);
	UpdateData(FALSE);

	CDialog::OnOK();
}
