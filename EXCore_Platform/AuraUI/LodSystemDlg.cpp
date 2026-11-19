// LodSystemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include <d3d9.h>
#include <d3dx9.h>
#include "LodSystemDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLodSystemDlg dialog


CLodSystemDlg::CLodSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CLodSystemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLodSystemDlg)
	m_fileName = _T("");
	m_activation = 0.0f;
	m_maxX = 0.0f;
	m_maxY = 0.0f;
	m_maxZ = 0.0f;
	m_minX = 0.0f;
	m_minY = 0.0f;
	m_minZ = 0.0f;
	//}}AFX_DATA_INIT
}


void CLodSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLodSystemDlg)
	DDX_Control(pDX, IDC_RADIO_BoundingBoxBased, m_radioBoundingBoxBased);
	DDX_Control(pDX, IDC_RADIO_DistanceBased, m_radioDistanceBased);
	DDX_Text(pDX, IDC_EDIT_FILENAME, m_fileName);
	DDX_Text(pDX, IDC_EDIT_ActivationDistance, m_activation);
	DDX_Text(pDX, IDC_EDIT_MaxX, m_maxX);
	DDX_Text(pDX, IDC_EDIT_MaxY, m_maxY);
	DDX_Text(pDX, IDC_EDIT_MaxZ, m_maxZ);
	DDX_Text(pDX, IDC_EDIT_MinX, m_minX);
	DDX_Text(pDX, IDC_EDIT_MinY, m_minY);
	DDX_Text(pDX, IDC_EDIT_MinZ, m_minZ);
	//}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_RADIO_AlphaDissolve, m_radioAlphaDissolveBased);

	
}


BEGIN_MESSAGE_MAP(CLodSystemDlg, CDialog)
	//{{AFX_MSG_MAP(CLodSystemDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	ON_BN_CLICKED(IDC_RADIO_BoundingBoxBased, OnRADIOBoundingBoxBased)
	ON_BN_CLICKED(IDC_RADIO_DistanceBased, OnRADIODistanceBased)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LOADFROMARW, OnBnClickedButtonLoadfromarw)
	ON_BN_CLICKED(IDC_RADIO_AlphaDissolve, OnBnClickedRadioAlphadissolve)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLodSystemDlg message handlers

void CLodSystemDlg::OnButtonBrowse() 
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
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   m_fileName = opendialog.GetPathName();
	   UpdateData(FALSE);
	 }
	 SetCurrentDirectory(MainDir);	
}

void CLodSystemDlg::OnRADIOBoundingBoxBased() 
{
	m_lodType = 1;
	
}

void CLodSystemDlg::OnRADIODistanceBased() 
{
	m_lodType = 0;
	
}

BOOL CLodSystemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(TRUE);
	if(m_lodType == 0)
	   m_radioDistanceBased.SetCheck(1);
	else if(m_lodType == 1)
	   m_radioBoundingBoxBased.SetCheck(1);
    else if(m_lodType == 2)
	   m_radioAlphaDissolveBased.SetCheck(1);
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CLodSystemDlg::OnBnClickedButtonLoadfromarw()
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
     if ( opendialog.DoModal() == IDOK )
	 {
	   UpdateData(TRUE);
	   CString fileName = opendialog.GetPathName();
	   UpdateData(FALSE);

       CEXMeshObj * m_mesh = new CEXMeshObj();
	   m_mesh->List_materialObject = new CMaterialObject();
	   float dist = 0.0f;
       if(m_mesh->LoadGeometryFromPlugin(fileName,&dist,g_pd3dDeviceRef,NULL))
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
	 SetCurrentDirectory(MainDir);	
}

void CLodSystemDlg::OnBnClickedRadioAlphadissolve()
{
	m_lodType = 2;
}
