// EditLensFlareDLG.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "EditLensFlareDLG.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EditLensFlareDLG dialog


EditLensFlareDLG::EditLensFlareDLG( CTextureDBObjList *texList, CWnd* pParent)
	: CDialog(EditLensFlareDLG::IDD, pParent)
{
	lensFlareNum = 0;
	textureList = texList;

	//{{AFX_DATA_INIT(EditLensFlareDLG)
	m_meshName = _T("");
	m_distanceAlongAxis = 0.0f;
	m_focalPointX = 0.0f;
	m_focalPointY = 0.0f;
	m_focalPointZ = 0.0f;
	m_meshScaling = 0.0f;
	//}}AFX_DATA_INIT
}

void EditLensFlareDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EditLensFlareDLG)
	DDX_Control(pDX, IDC_COMBO_textureSelect, m_textureDataBaseDropdown);
	DDX_Control(pDX, IDC_LIST_EditLensFlare_SelectedLens, m_lensSelect);
	DDX_Text(pDX, IDC_EDIT_EditLensFlare_meshName, m_meshName);
	DDX_Text(pDX, IDC_EDIT_EditLensFlare_distanceAlongAxis, m_distanceAlongAxis);
	DDX_Text(pDX, IDC_EDIT_EditLensFlare_focalPointX, m_focalPointX);
	DDX_Text(pDX, IDC_EDIT_EditLensFlare_focalPointY, m_focalPointY);
	DDX_Text(pDX, IDC_EDIT_EditLensFlare_focalPointZ, m_focalPointZ);
	DDX_Text(pDX, IDC_EDIT_EditLensFlare_meshScaling, m_meshScaling);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(EditLensFlareDLG, CDialog)
	//{{AFX_MSG_MAP(EditLensFlareDLG)
	ON_BN_CLICKED(IDC_BTN_EditLensFlare_Browse, OnBTNEditLensFlareBrowse)
	ON_BN_CLICKED(IDC_BTN_EditLensFlare_AddLens, OnBTNEditLensFlareAddLens)
	ON_BN_CLICKED(IDC_BTN_EditLensFlare_DeleteLens, OnBTNEditLensFlareDeleteLens)
	ON_BN_CLICKED(IDC_BTN_EditLensFlare_UpdateLens, OnBTNEditLensFlareUpdateLens)
	ON_LBN_SELCHANGE(IDC_LIST_EditLensFlare_SelectedLens, OnSelchangeLISTEditLensFlareSelectedLens)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EditLensFlareDLG message handlers

void EditLensFlareDLG::OnBTNEditLensFlareBrowse() 
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
	MessageBox("start");
	CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
    MessageBox("end");
	if ( opendialog.DoModal() == IDOK )
		{
		m_meshName = opendialog.GetPathName();
		UpdateData(FALSE);
		}
	
	SetCurrentDirectory(MainDir);
}

BOOL EditLensFlareDLG::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int texCnt = 0;
	texCnt = textureList->GetCount();

	m_textureDataBaseDropdown.ResetContent();
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
		POSITION nextPos = textureList->GetHeadPosition();
		CTextureDBObj *nextTex = NULL;
						
		while( nextPos != NULL )
			{
			nextTex = (CTextureDBObj *)textureList->GetAt( nextPos );
			textureList->GetNext(nextPos);

			textureName = nextTex->List_textureName;

			m_textureDataBaseDropdown.AddString( textureName );
			}

		m_textureDataBaseDropdown.SetCurSel(0);
		}

	UpdateListBox();
	
	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void EditLensFlareDLG::UpdateListBox()
{
	m_lensSelect.ResetContent();
	CString addString = "";

	for(int cnt=0; cnt < lensFlare->lensList.GetCount(); cnt++)
		{
		addString.Format("Lens %i", cnt);
		m_lensSelect.InsertString(-1, addString);
		}

}

void EditLensFlareDLG::OnBTNEditLensFlareAddLens() 
{
	//make sure selection is valid
	CString textureName = "";
	int currPos = m_textureDataBaseDropdown.GetCurSel();
	m_textureDataBaseDropdown.GetLBText(currPos, textureName);
	
	UpdateData(TRUE);
		
	LENS *newLens = NULL;
	newLens = new LENS;

	//find texture
	POSITION texPos = NULL;
	texPos  = textureList->FindIndex( currPos );

	CTextureDBObj *texture;
	texture = (CTextureDBObj*)textureList->GetAt( texPos );

	newLens->distAlongAxis   = m_distanceAlongAxis;
	newLens->textureIndex    = currPos;
	newLens->textureFileName = textureName;
	newLens->texture         = texture;
	newLens->scale           = m_meshScaling;

	lensFlare->lensList.AddTail(newLens);

	UpdateListBox();
	
	m_lensSelect.SetCurSel( m_lensSelect.GetCount()-1 );

	m_textureDataBaseDropdown.SetCurSel(0);
	m_distanceAlongAxis = 0;

	UpdateData(FALSE);	
}

void EditLensFlareDLG::OnBTNEditLensFlareDeleteLens() 
{
	int index = m_lensSelect.GetCurSel();

	if( index < 0 || index >= m_lensSelect.GetCount() )
		return;

	LENS *delLens = NULL;
		
	POSITION delPos = lensFlare->lensList.FindIndex(index);
	delLens = (LENS *)lensFlare->lensList.GetAt(delPos);

	lensFlare->lensList.RemoveAt(delPos);

	delete delLens;
	UpdateListBox();

	int newFocus = index;
	int count = lensFlare->lensList.GetCount();

	if( count == 0 )
		{
		m_distanceAlongAxis = 0;
		m_meshScaling = 0.0f;
		UpdateData(FALSE);
		return;
		}

	if( newFocus >= count )
		newFocus--;

	m_lensSelect.SetCurSel(newFocus);

	LENS *updateInf = NULL;

	POSITION update = lensFlare->lensList.FindIndex(newFocus);
	updateInf = (LENS *)lensFlare->lensList.GetAt(update);

	m_distanceAlongAxis = updateInf->distAlongAxis;
	m_meshScaling       = updateInf->scale;
	m_textureDataBaseDropdown.SetCurSel(updateInf->textureIndex);

	UpdateData(FALSE);

}

void EditLensFlareDLG::OnBTNEditLensFlareUpdateLens() 
{
	int index = m_lensSelect.GetCurSel();
	
	if( index < 0 || index >= m_lensSelect.GetCount() )
		return;

	LENS *editLens = NULL;
		
	POSITION editPos = lensFlare->lensList.FindIndex(index);
	editLens = (LENS *)lensFlare->lensList.GetAt(editPos);

	UpdateData(TRUE);
	editLens->distAlongAxis = m_distanceAlongAxis;
	editLens->scale           = m_meshScaling;

	int currPos = 0;
	currPos = m_textureDataBaseDropdown.GetCurSel();

	CString textureName = "";
	m_textureDataBaseDropdown.GetLBText(currPos, textureName);

	editLens->textureFileName = textureName;
	editLens->textureIndex    = currPos;
		
	CTextureDBObj *textureObj = NULL;
		
	POSITION texPos = NULL;
	texPos = textureList->FindIndex(currPos);
	
	if(texPos)
		textureObj = (CTextureDBObj *)textureList->GetAt(texPos);
	else
		{
		MessageBox("Invalid texture index!");
		return;
		}

	editLens->texture = textureObj;
}

void EditLensFlareDLG::OnSelchangeLISTEditLensFlareSelectedLens() 
{
	int index = m_lensSelect.GetCurSel();
	
	if( index < 0 || index >= m_lensSelect.GetCount() )
		return;

	LENS *viewLens = NULL;
		
	POSITION viewPos = lensFlare->lensList.FindIndex(index);
	viewLens = (LENS *)lensFlare->lensList.GetAt(viewPos);

	m_distanceAlongAxis = viewLens->distAlongAxis;
	m_meshScaling       = viewLens->scale;

	m_textureDataBaseDropdown.SetCurSel(viewLens->textureIndex);
	
	UpdateData(FALSE);
}
