// ParticleSystem_Both.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "direct.h"
#include "d3d9.h"
#include "CTextureDBClass.h"
#include "ParticleSystem_Both.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ParticleSystem_Both property page

IMPLEMENT_DYNCREATE(ParticleSystem_Both, CPropertyPage)

ParticleSystem_Both::ParticleSystem_Both(CTextureDBObjList *texList)
	: CPropertyPage(ParticleSystem_Both::IDD)
{
	m_textureList  = texList;
	m_textureIndex = 0;
	m_textureName  = "";

	//{{AFX_DATA_INIT(ParticleSystem_Both)
	m_blue  = 0.0f;
	m_green = 0.0f;
	m_red   = 0.0f;
	m_endSpawn = FALSE;
	m_fadeTime = FALSE;
	m_respawn = FALSE;
	m_sort = FALSE;
	m_sphereType = FALSE;
	m_varyLifetime = FALSE;
	m_fileName = _T("");
	m_particleCount = 0;
	m_particleLifetime = 0;
	m_meshScale = 0.0f;
	//}}AFX_DATA_INIT
}

ParticleSystem_Both::~ParticleSystem_Both()
{
}

void ParticleSystem_Both::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ParticleSystem_Both)
	DDX_Control(pDX, IDC_COMBO_textureSelect, m_textureSelect);
	DDX_Control(pDX, IDC_RADIO_ParticleSystemBoth_IsNormal, m_isNormal);
	DDX_Control(pDX, IDC_RADIO_ParticleSystemBoth_IsDirectional, m_isDirectional);
	DDX_Control(pDX, IDC_VISUAL, m_visual);
	DDX_Control(pDX, IDC_SLIDER_RED, m_redSlider);
	DDX_Control(pDX, IDC_SLIDER_GREEN, m_greenSlider);
	DDX_Control(pDX, IDC_SLIDER_BLUE, m_blueSlider);
	DDX_Text(pDX, IDC_EDIT_BLUE, m_blue);
	DDX_Text(pDX, IDC_EDIT_GREEN, m_green);
	DDX_Text(pDX, IDC_EDIT_RED, m_red);
	DDX_Check(pDX, IDC_CHECK_EndSpwn, m_endSpawn);
	DDX_Check(pDX, IDC_CHECK_FadeTime, m_fadeTime);
	DDX_Check(pDX, IDC_CHECK_Regenerate, m_respawn);
	DDX_Check(pDX, IDC_CHECK_SortOn, m_sort);
	DDX_Check(pDX, IDC_CHECK_SphereType, m_sphereType);
	DDX_Check(pDX, IDC_CHECK_VaryLifetime, m_varyLifetime);
	DDX_Text(pDX, IDC_EDIT_VisFileName, m_fileName);
	DDX_Text(pDX, IDC_EDIT_ParticleCount, m_particleCount);
	DDX_Text(pDX, IDC_EDIT_ParticleLifeTime, m_particleLifetime);
	DDX_Text(pDX, IDC_EDIT_meshScale, m_meshScale);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_BlendOptions, m_blendMethod);
	DDX_Text(pDX, IDC_EDIT_systemName, m_systemName);
}


BEGIN_MESSAGE_MAP(ParticleSystem_Both, CPropertyPage)
	//{{AFX_MSG_MAP(ParticleSystem_Both)
	ON_BN_CLICKED(IDC_BUTTON_ParticleSystemBoth_Browse, OnBUTTONParticleSystemBothBrowse)
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_textureSelect, OnSelchangeCOMBOtextureSelect)
	ON_BN_CLICKED(IDC_RADIO_ParticleSystemBoth_IsDirectional, OnRADIOParticleSystemBothIsDirectional)
	ON_BN_CLICKED(IDC_RADIO_ParticleSystemBoth_IsNormal, OnRADIOParticleSystemBothIsNormal)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void ParticleSystem_Both::OnBUTTONParticleSystemBothBrowse() 
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
    
	if( opendialog.DoModal() == IDOK )
		{
		UpdateData(TRUE);
		m_fileName = opendialog.GetPathName();
		UpdateData(FALSE);
		}
	
	SetCurrentDirectory(MainDir);
}

BOOL ParticleSystem_Both::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	m_blueSlider.SetRange (0,100,TRUE);
	m_redSlider.SetRange  (0,100,TRUE);
	m_greenSlider.SetRange(0,100,TRUE);
		
	m_redSlider.SetPos  ( int(m_red   * 100.0f) );
	m_greenSlider.SetPos( int(m_green * 100.0f) );
	m_blueSlider.SetPos ( int(m_blue  * 100.0f) );
	    
	//get rectangle of visual frame
    m_visual.GetClientRect( &dimensions ); 
    m_visual.ClientToScreen( &dimensions );
    ScreenToClient( &dimensions );


	m_blendMethod.SetCurSel(m_blendMethodLoc+1);
    
	m_isDirectional.SetCheck( m_isDirectionalChecked  );
	m_isNormal.SetCheck( !m_isDirectionalChecked );
	
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

	Invalidate(FALSE);

	return TRUE;  

}

void ParticleSystem_Both::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	
	UpdateData(TRUE);

	m_red   = m_redSlider.GetPos()   * 0.01f;
	m_green = m_greenSlider.GetPos() * 0.01f;
	m_blue  = m_blueSlider.GetPos()  * 0.01f;

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
	
	UpdateData(FALSE);
	Invalidate(FALSE);

}

void ParticleSystem_Both::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
    CBrush* tbrush = new CBrush;
    //tbrush->CreateSolidBrush(RGB(m_red,m_green,m_blue));
	tbrush->CreateSolidBrush(RGB(m_red * 255.0f,m_green * 255.0f,m_blue * 255.0f));

    dc.SelectObject(tbrush);
	dc.Rectangle
		(
		dimensions.left,
		dimensions.top,
		dimensions.right,
		dimensions.bottom
		);	
	
	DeleteObject(tbrush);
	delete tbrush;
	
	// Do not call CPropertyPage::OnPaint() for painting messages
}

void ParticleSystem_Both::OnOK() 
{
	CFileException exception;
    CFile file;
    /*BOOL res = file.Open(m_fileName,CFile::modeRead, &exception);
	
	if(res == FALSE)
		{
		MessageBox("Please select a valid file");
		OnBUTTONParticleSystemBothBrowse();
		}
	else
		{
		file.Close();
		m_isDirectionalChecked = m_isDirectional.GetCheck();
		CPropertyPage::OnOK();
		}*/
        m_blendMethodLoc = m_blendMethod.GetCurSel()-1;


	    m_isDirectionalChecked = m_isDirectional.GetCheck();
		CPropertyPage::OnOK();
}


void ParticleSystem_Both::OnSelchangeCOMBOtextureSelect() 
{
	int selection = 0;
	selection = m_textureSelect.GetCurSel();

	CString textureName = "";
	m_textureSelect.GetLBText(selection, textureName);

	m_textureName  = textureName;
	m_textureIndex = selection;	
}

void ParticleSystem_Both::OnRADIOParticleSystemBothIsDirectional() 
{
	if( m_isNormal.GetCheck() )
		{
		m_isNormal.SetCheck(0);
		m_isDirectional.SetCheck(1);
		}
}

void ParticleSystem_Both::OnRADIOParticleSystemBothIsNormal() 
{
	if( m_isDirectional.GetCheck() )
		{
		m_isDirectional.SetCheck(0);
		m_isNormal.SetCheck(1);
		}
}
