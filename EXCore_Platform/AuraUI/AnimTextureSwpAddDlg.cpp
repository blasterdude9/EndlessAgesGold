// AnimTextureSwpAddDlg.cpp : implementation file
//

#include "stdafx.h"

#include "resource.h"
#include "ColorDlg.h"
#include "AnimTextureSwpAddDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAnimTextureSwpAddDlg dialog


CAnimTextureSwpAddDlg::CAnimTextureSwpAddDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAnimTextureSwpAddDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAnimTextureSwpAddDlg)
	m_delay = 0;
	m_alphaMapSize = 0;
	m_howManyTextures = 0;
	m_textureName = _T("");
	m_alphaChannel = 0;
	//}}AFX_DATA_INIT
}


void CAnimTextureSwpAddDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAnimTextureSwpAddDlg)
	DDX_Text(pDX, IDC_EDIT_addativeDelay, m_delay);
	DDX_Text(pDX, IDC_EDIT_alphaMapSize, m_alphaMapSize);
	DDX_Text(pDX, IDC_EDIT_howManyTextures, m_howManyTextures);
	DDX_Text(pDX, IDC_EDIT_textureName, m_textureName);
	DDX_Text(pDX, IDC_EDIT_alphaChannel, m_alphaChannel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAnimTextureSwpAddDlg, CDialog)
	//{{AFX_MSG_MAP(CAnimTextureSwpAddDlg)
	ON_BN_CLICKED(IDC_CHECK_decalTransparency, OnCHECKdecalTransparency)
	ON_BN_CLICKED(IDC_CHECK_usesAlphaTexture, OnCHECKusesAlphaTexture)
	ON_BN_CLICKED(IDC_BUTTON_decalSrcColor, OnBUTTONdecalSrcColor)
	ON_BN_CLICKED(IDC_BUTTON_BrowseMap, OnBUTTONBrowseMap)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAnimTextureSwpAddDlg message handlers

void CAnimTextureSwpAddDlg::OnCHECKdecalTransparency() 
{
	if(useDecalTransparency == TRUE)
       useDecalTransparency = FALSE;
	else
	   useDecalTransparency  = TRUE;	
}

void CAnimTextureSwpAddDlg::OnCHECKusesAlphaTexture() 
{
	if(useAlphaTexture == TRUE)
       useAlphaTexture = FALSE;
	else
	   useAlphaTexture  = TRUE;
}

void CAnimTextureSwpAddDlg::OnBUTTONdecalSrcColor() 
{
   CColorDlg cdlg;

   cdlg.m_red = redSrc;
   cdlg.m_green = greenSrc;
   cdlg.m_blue = blueSrc;
   if(cdlg.DoModal()==IDOK){
   redSrc = cdlg.m_red;
   greenSrc = cdlg.m_green;
   blueSrc = cdlg.m_blue;
   }
	
}

BOOL CAnimTextureSwpAddDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    useDecalTransparency = FALSE;
	useAlphaTexture = FALSE;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAnimTextureSwpAddDlg::OnBUTTONBrowseMap() 
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
	   m_textureName = fileName;
	   UpdateData(FALSE);
	 }	
	SetCurrentDirectory(MainDir);
}
