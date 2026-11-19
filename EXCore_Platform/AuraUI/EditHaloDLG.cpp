// EditHaloDLG.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "EditHaloDLG.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EditHaloDLG dialog


EditHaloDLG::EditHaloDLG(CTextureDBObjList *texList, CWnd* pParent)
	: CDialog(EditHaloDLG::IDD, pParent)
{
	m_textureList  = texList;
	m_textureIndex = 0;
	m_textureName  = "";

	//{{AFX_DATA_INIT(EditHaloDLG)
	m_growthRate = 0.0f;
	m_maxSize = 0.0f;
	m_meshFileName = "<none>";
	//}}AFX_DATA_INIT
}


void EditHaloDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EditHaloDLG)
	DDX_Control(pDX, IDC_COMBO_EditHalo_textureSelect, m_textureSelect);
	DDX_Text(pDX, IDC_EDIT_EditHalo_GrowthRate, m_growthRate);
	DDX_Text(pDX, IDC_EDIT_EditHalo_MaxSize, m_maxSize);
	DDX_Text(pDX, IDC_EDIT_MeshFileName, m_meshFileName);
	//}}AFX_DATA_MAP
}

void EditHaloDLG::OnBrowseFileName()
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

BEGIN_MESSAGE_MAP(EditHaloDLG, CDialog)
	//{{AFX_MSG_MAP(EditHaloDLG)
	ON_BN_CLICKED(IDC_BUTTON_BrowseFileName, OnBrowseFileName)
	ON_CBN_SELCHANGE(IDC_COMBO_EditHalo_textureSelect, OnSelchangeCOMBOEditHalotextureSelect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EditHaloDLG message handlers

BOOL EditHaloDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();

	int texCnt = 0;
	texCnt = m_textureList->GetCount();
	
	m_textureSelect.ResetContent();

	if( texCnt <= 0 )
		{
		
		MessageBox
			(
			"EditHaloDLG::OnInitDialog()",
			"This should never happen"
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

		m_textureSelect.SetCurSel(m_textureIndex);
		
		}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void EditHaloDLG::OnSelchangeCOMBOEditHalotextureSelect() 
{
	int selection = 0;
	selection = m_textureSelect.GetCurSel();

	CString textureName = "";
	m_textureSelect.GetLBText(selection, textureName);

	m_textureName  = textureName;
	m_textureIndex = selection;	
}
