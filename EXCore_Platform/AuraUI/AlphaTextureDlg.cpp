// AlphaTextureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "AlphaTextureDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAlphaTextureDlg dialog


CAlphaTextureDlg::CAlphaTextureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAlphaTextureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAlphaTextureDlg)
	m_channel = 0;
	m_fileName = _T("");
	//}}AFX_DATA_INIT
}


void CAlphaTextureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAlphaTextureDlg)
	DDX_Control(pDX, IDC_CHECK_ENABLED, m_checkEnabled);
	DDX_Control(pDX, IDC_CHECK_64X64, m_check64X64);
	DDX_Control(pDX, IDC_CHECK_256X256, m_check256X256);
	DDX_Control(pDX, IDC_CHECK_128X128, m_check128X128);
	DDX_Text(pDX, IDC_EDIT_Channel, m_channel);
	DDX_Text(pDX, IDC_EDIT_TextureFileName, m_fileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAlphaTextureDlg, CDialog)
	//{{AFX_MSG_MAP(CAlphaTextureDlg)
	ON_BN_CLICKED(IDC_BUTTON_BrowseFileName, OnBUTTONBrowseFileName)
	ON_BN_CLICKED(IDC_CHECK_128X128, OnCheck128x128)
	ON_BN_CLICKED(IDC_CHECK_256X256, OnCheck256x256)
	ON_BN_CLICKED(IDC_CHECK_64X64, OnCheck64x64)
	ON_BN_CLICKED(IDC_CHECK_ENABLED, OnCheckEnabled)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAlphaTextureDlg message handlers

void CAlphaTextureDlg::OnBUTTONBrowseFileName() 
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
static char BASED_CODE filter[] = "Texture Files|*.tga;*.bmp;*.png;*.dds;*.jpg;*.jpeg|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   CString fileName = opendialog.GetFileName();
	   m_fileName = fileName;
	   UpdateData(FALSE);
	 }	
	SetCurrentDirectory(MainDir);
}

BOOL CAlphaTextureDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);
	//textureSize
	if(textureSize == 64)
	   m_check64X64.SetCheck(1);
	if(textureSize == 128)
	   m_check128X128.SetCheck(1);
    if(textureSize == 256)
	   m_check256X256.SetCheck(1);
	if(enabled==TRUE)
	    m_checkEnabled.SetCheck(1);
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAlphaTextureDlg::OnCheck128x128() 
{
	textureSize = 128;
	m_check64X64.SetCheck(0);
	m_check256X256.SetCheck(0);
	m_check128X128.SetCheck(1);
}

void CAlphaTextureDlg::OnCheck256x256() 
{
	textureSize = 256;
	m_check64X64.SetCheck(0);
	m_check256X256.SetCheck(1);
	m_check128X128.SetCheck(0);
}

void CAlphaTextureDlg::OnCheck64x64() 
{
	textureSize = 64;
	m_check64X64.SetCheck(1);
	m_check256X256.SetCheck(0);
	m_check128X128.SetCheck(0);
	
}

void CAlphaTextureDlg::OnCheckEnabled() 
{
	if(enabled == TRUE){
       enabled = FALSE;
	   m_checkEnabled.SetCheck(0);
	}
	else{
       enabled = TRUE;
	   m_checkEnabled.SetCheck(1);
	}
}
