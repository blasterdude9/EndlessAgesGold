// EditTextureDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "EditTextureDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditTextureDlg dialog


CEditTextureDlg::CEditTextureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditTextureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditTextureDlg)
	m_filename = _T("");
	m_level = 0;
	//}}AFX_DATA_INIT
}


void CEditTextureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditTextureDlg)
	DDX_Text(pDX, IDC_EDIT_VisualFileName, m_filename);
	DDX_Text(pDX, IDC_EDIT_Level, m_level);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditTextureDlg, CDialog)
	//{{AFX_MSG_MAP(CEditTextureDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Level, OnDeltaposSPINLevel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditTextureDlg message handlers

void CEditTextureDlg::OnButtonBrowse() 
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
static char BASED_CODE filter[] = "Engine Models|*.arw;*.x;*.3ds;*.fbx;*.obj;*.gltf;*.glb;*.dae;*.stl;*.ply;*.blend;*.ase;*.lwo;*.dxf|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   m_filename = opendialog.GetFileName();
	   UpdateData(FALSE);
	 }	
	SetCurrentDirectory(MainDir);
	

}

void CEditTextureDlg::OnDeltaposSPINLevel(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_UPDOWN* pNMUpDown = (NM_UPDOWN*)pNMHDR;
	
    UpdateData(TRUE);
    m_level -= pNMUpDown->iDelta;
    UpdateData(FALSE);	

	*pResult = 0;
}
