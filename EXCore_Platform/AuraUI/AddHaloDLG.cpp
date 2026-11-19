// AddHaloDLG.cpp : implementation file
//
#include "stdafx.h"
#include "Resource.h"
#include "CEXMeshClass.h"
#include "AddHaloDLG.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
 #endif

/////////////////////////////////////////////////////////////////////////////
// AddHaloDLG dialog


AddHaloDLG::AddHaloDLG(CTextureDBObjList *texList, CWnd* pParent)
	: CDialog(AddHaloDLG::IDD, pParent)
{
	m_textureList = texList;
	m_textureIndex = 0;
	m_textureName  = "";

	//{{AFX_DATA_INIT(AddHaloDLG)
	m_growthRate = 0.005f;
	m_meshFileName = "<none>";
	m_maxSize = 0.5f;
	//}}AFX_DATA_INIT
}

void AddHaloDLG::OnBrowseFileName()
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
		m_meshFileName = opendialog.GetPathName();
		UpdateData(FALSE);
		}
	
	SetCurrentDirectory(MainDir);

}

void AddHaloDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(AddHaloDLG)
	DDX_Control(pDX, IDC_COMBO_AddHalo_textureSelect, m_textureSelect);
	DDX_Text(pDX, IDC_EDIT_AddHalo_GrowthRate, m_growthRate);
	DDX_Text(pDX, IDC_EDIT_MeshFileName, m_meshFileName);
	DDX_Text(pDX, IDC_EDIT_AddHalo_MaxSize, m_maxSize);
	//}}AFX_DATA_MAP
}

void AddHaloDLG::OnOK()
{
	if( m_meshFileName == "<none>" )
		{
		MessageBox("Please select a valid object.");
		OnBrowseFileName();
		}
	else
		CDialog::OnOK();

}

BEGIN_MESSAGE_MAP(AddHaloDLG, CDialog)
	//{{AFX_MSG_MAP(AddHaloDLG)
	ON_BN_CLICKED(IDC_BUTTON_BrowseFileName, OnBrowseFileName)
	ON_CBN_SELCHANGE(IDC_COMBO_AddHalo_textureSelect, OnSelchangeCOMBOAddHalotextureSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// AddHaloDLG message handlers

BOOL AddHaloDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	int texCnt = 0;
	texCnt = m_textureList->GetCount();
	
	m_textureSelect.ResetContent();
	if( texCnt <= 0 )
		{
		
		MessageBox
			(
			"Empty texture database!",
			"No textures!"
			);
		OnCancel();

		}
	else
		{
		CString textureName = "";
		POSITION nextPos = m_textureList->GetHeadPosition();
		CTextureDBObj *nextTex = NULL;
						
		while( nextPos != NULL )
			{
			nextTex = (CTextureDBObj *)m_textureList->GetAt( nextPos );
			m_textureList->GetNext(nextPos);

			textureName = nextTex->List_textureName;

			m_textureSelect.AddString( textureName );
			}

		m_textureSelect.GetLBText(0, m_textureName);
		m_textureIndex = 0;

		m_textureSelect.SetCurSel(0);
		}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void AddHaloDLG::OnSelchangeCOMBOAddHalotextureSelect() 
{
	int selection = 0;
	selection = m_textureSelect.GetCurSel();

	CString textureName = "";
	m_textureSelect.GetLBText(selection, textureName);

	m_textureName  = textureName;
	m_textureIndex = selection;
}
