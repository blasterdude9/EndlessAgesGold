// AddLightningStrikeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "AddLightningStrikeDlg.h"
#include "direct.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
 #endif

/////////////////////////////////////////////////////////////////////////////
// CAddLightningStrikeDlg dialog


CAddLightningStrikeDlg::CAddLightningStrikeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddLightningStrikeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddLightningStrikeDlg)
	m_soundFile = _T("");
	m_soundFactor = 0.0f;
	m_sndID = 0;
	//}}AFX_DATA_INIT
}


void CAddLightningStrikeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddLightningStrikeDlg)
	DDX_Control(pDX, IDC_COMBO_TextureDatabase, m_textureDatabase);
	DDX_Text(pDX, IDC_EDIT_soundFile, m_soundFile);
	DDX_Text(pDX, IDC_EDIT_SoundFactorX, m_soundFactor);
	DDX_Text(pDX, IDC_EDIT_SoundID, m_sndID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddLightningStrikeDlg, CDialog)
	//{{AFX_MSG_MAP(CAddLightningStrikeDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddLightningStrikeDlg message handlers

BOOL CAddLightningStrikeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);
	for(POSITION posLoc = m_textureDataBaseRef->GetHeadPosition();posLoc != NULL;)
	{
        CTextureDBObj * txbPtr = (CTextureDBObj *)m_textureDataBaseRef->GetNext(posLoc);
        m_textureDatabase.AddString(txbPtr->List_textureName);
	}

    m_textureDatabase.SetCurSel(0);
	m_textureIndexSelection = 0;

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CAddLightningStrikeDlg::OnButtonBrowse() 
{
    //use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\Sounds\\");
			 SetCurrentDirectory(CurDir);
    //end use mapsmodels folder for organization
    static char BASED_CODE filter[] = "*.Wav Files|*.Wav|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   m_soundFile = opendialog.GetFileName();
	   UpdateData(FALSE);
	 }	
	SetCurrentDirectory(MainDir);	
}

void CAddLightningStrikeDlg::OnOK() 
{
	m_textureIndexSelection = m_textureDatabase.GetCurSel();
	
	CDialog::OnOK();
}
