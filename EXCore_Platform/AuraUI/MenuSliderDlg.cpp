// MenuSliderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "MenuSliderDlg.h"
#include "direct.h"
#include "SkeletalAnimationDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenuSliderDlg dialog


CMenuSliderDlg::CMenuSliderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMenuSliderDlg::IDD, pParent)
	, m_fontRed(0)
	, m_fontGreen(0)
	, m_fontBlue(0)
{
	//{{AFX_DATA_INIT(CMenuSliderDlg)
	m_boxClickSize = 0.0f;
	m_clickWidth = 0.0f;
	m_fontSizeX = 0.0f;
	m_fontSizeY = 0.0f;
	m_offsetX = 0.0f;
	m_positionBottomX = 0.0f;
	m_positionBottomY = 0.0f;
	m_positionMidX = 0.0f;
	m_positionMidY = 0.0f;
	m_positionTopX = 0.0f;
	m_positionTopY = 0.0f;
	m_visibleItems = 0;
	m_visSpacing = 0.0f;
	m_sliderControlID = 0;
	//}}AFX_DATA_INIT
}


void CMenuSliderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMenuSliderDlg)
	DDX_Text(pDX, IDC_EDIT_BoxClickSize, m_boxClickSize);
	DDX_Text(pDX, IDC_EDIT_ClickWidth, m_clickWidth);
	DDX_Text(pDX, IDC_EDIT_FontSizeX, m_fontSizeX);
	DDX_Text(pDX, IDC_EDIT_FontSizeY, m_fontSizeY);
	DDX_Text(pDX, IDC_EDIT_OFFSETX, m_offsetX);
	DDX_Text(pDX, IDC_EDIT_positionBottomX, m_positionBottomX);
	DDX_Text(pDX, IDC_EDIT_positionBottomY, m_positionBottomY);
	DDX_Text(pDX, IDC_EDIT_positionMidX, m_positionMidX);
	DDX_Text(pDX, IDC_EDIT_positionMidY, m_positionMidY);
	DDX_Text(pDX, IDC_EDIT_positionTopX, m_positionTopX);
	DDX_Text(pDX, IDC_EDIT_positionTopY, m_positionTopY);
	DDX_Text(pDX, IDC_EDIT_VisibleItems, m_visibleItems);
	DDX_Text(pDX, IDC_EDIT_VisualSpacing, m_visSpacing);
	DDX_Text(pDX, IDC_EDIT_SliderControlID, m_sliderControlID);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_CHECK_SelectionAbilityDisabled, m_selectionAbilityDisabled);
	DDX_Text(pDX, IDC_EDIT_FontRedsl, m_fontRed);
	DDX_Text(pDX, IDC_EDIT_FontGreensl, m_fontGreen);
	DDX_Text(pDX, IDC_EDIT_FontBlueSl, m_fontBlue);
}


BEGIN_MESSAGE_MAP(CMenuSliderDlg, CDialog)
	//{{AFX_MSG_MAP(CMenuSliderDlg)
	ON_BN_CLICKED(IDC_BUTTON_CreateTopSystem, OnBUTTONCreateTopSystem)
	ON_BN_CLICKED(IDC_BUTTON_CreateBottomSystem, OnBUTTONCreateBottomSystem)
	ON_BN_CLICKED(IDC_BUTTON_CreateMidSystem, OnBUTTONCreateMidSystem)
	ON_BN_CLICKED(IDC_BUTTON_EditSystemBottom, OnBUTTONEditSystemBottom)
	ON_BN_CLICKED(IDC_BUTTON_EditSystemMid, OnBUTTONEditSystemMid)
	ON_BN_CLICKED(IDC_BUTTON_EditSystemTop, OnBUTTONEditSystemTop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenuSliderDlg message handlers

BOOL CMenuSliderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(TRUE);

    m_boxClickSize               = m_sliderBarRef->m_boxClickSize;
	m_clickWidth                 = m_sliderBarRef->m_clickWidth;
	m_fontSizeX                  = m_sliderBarRef->m_fontSizeX;
	m_fontSizeY                  = m_sliderBarRef->m_fontSizeY;
	m_offsetX                    = m_sliderBarRef->m_visOffsetX;
	m_positionBottomX            = m_sliderBarRef->m_locEnd.x;
	m_positionBottomY            = m_sliderBarRef->m_locEnd.y;
	m_positionMidX               = m_sliderBarRef->m_center.x;
	m_positionMidY               = m_sliderBarRef->m_center.y;
	m_positionTopX               = m_sliderBarRef->m_locStart.x;
	m_positionTopY               = m_sliderBarRef->m_locStart.y;
	m_visibleItems               = m_sliderBarRef->m_itemsThatAreVisible;
	m_visSpacing                 = m_sliderBarRef->m_visSpacing;
    m_sliderControlID            = m_sliderBarRef->m_controlID;
	m_fontRed				     = m_sliderBarRef->m_red;
	m_fontGreen			         = m_sliderBarRef->m_green;
	m_fontBlue			         = m_sliderBarRef->m_blue;


	m_selectionAbilityDisabled.SetCheck(m_sliderBarRef->m_selectionDisabled);

    

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMenuSliderDlg::OnBUTTONCreateTopSystem() 
{
	if(m_sliderBarRef->m_visStart){
       m_sliderBarRef->m_visStart->SafeDelete();
	   delete m_sliderBarRef->m_visStart;
	   m_sliderBarRef->m_visStart=0;
	}
	//use mapsmodels folder for organization
           CString CurDir;
           CString MainDir;
           char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
    //end use mapsmodels folder for organization
          static char BASED_CODE filter[] = "ARB Files|*.ARB|All Files (*.*)|*.*||";
	      CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
          if ( opendialog.DoModal() == IDOK )
		  {
	         CString fileName = opendialog.GetPathName();
             SetCurrentDirectory(MainDir);	
			 m_sliderBarRef->m_visStart = new CSkeletonObject();
			 if(!m_sliderBarRef->m_visStart->InitBoneSystem(fileName,g_pd3dDeviceRef)){
				 m_sliderBarRef->m_visStart->SafeDelete();
                 delete m_sliderBarRef->m_visStart;
				 m_sliderBarRef->m_visStart=0;
			 }
		  }  
		  SetCurrentDirectory(MainDir);
}

void CMenuSliderDlg::OnBUTTONCreateBottomSystem() 
{
	if(m_sliderBarRef->m_visEnd){
       m_sliderBarRef->m_visEnd->SafeDelete();
	   delete m_sliderBarRef->m_visEnd;
	   m_sliderBarRef->m_visEnd=0;
	}
	//use mapsmodels folder for organization
           CString CurDir;
           CString MainDir;
           char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
    //end use mapsmodels folder for organization
          static char BASED_CODE filter[] = "ARB Files|*.ARB|All Files (*.*)|*.*||";
	      CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
          if ( opendialog.DoModal() == IDOK )
		  {
	         CString fileName = opendialog.GetPathName();
             SetCurrentDirectory(MainDir);	
			 m_sliderBarRef->m_visEnd = new CSkeletonObject();
			 if(!m_sliderBarRef->m_visEnd->InitBoneSystem(fileName,g_pd3dDeviceRef)){
				 m_sliderBarRef->m_visEnd->SafeDelete();
                 delete m_sliderBarRef->m_visEnd;
				 m_sliderBarRef->m_visEnd=0;
			 }
		  }  
		  SetCurrentDirectory(MainDir);
}

void CMenuSliderDlg::OnBUTTONCreateMidSystem() 
{
	if(m_sliderBarRef->m_visMov){
       m_sliderBarRef->m_visMov->SafeDelete();
	   delete m_sliderBarRef->m_visMov;
	   m_sliderBarRef->m_visMov=0;
	}
	//use mapsmodels folder for organization
           CString CurDir;
           CString MainDir;
           char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
    //end use mapsmodels folder for organization
          static char BASED_CODE filter[] = "ARB Files|*.ARB|All Files (*.*)|*.*||";
	      CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
          if ( opendialog.DoModal() == IDOK )
		  {
	         CString fileName = opendialog.GetPathName();
             SetCurrentDirectory(MainDir);	
			 m_sliderBarRef->m_visMov = new CSkeletonObject();
			 if(!m_sliderBarRef->m_visMov->InitBoneSystem(fileName,g_pd3dDeviceRef)){
				 m_sliderBarRef->m_visMov->SafeDelete();
                 delete m_sliderBarRef->m_visMov;
				 m_sliderBarRef->m_visMov=0;
			 }
		  }  
		  SetCurrentDirectory(MainDir);
}

void CMenuSliderDlg::OnBUTTONEditSystemBottom() 
{
	if(!m_sliderBarRef->m_visEnd){
        AfxMessageBox("Start Visual Not Created yet..");
		return;
	}
	CSkeletalAnimationDlg dlg;
	dlg.m_globalShaderRef = NULL;
	dlg.m_elementDBREF = NULL;
	dlg.m_skillDatabaseREF = NULL;
	dlg.m_skeletonReferencePtr = m_sliderBarRef->m_visEnd;
	dlg.GL_textureDataBase = GL_textureDataBase;
    dlg.directSoundRef = directSoundRef;
	dlg.DoModal();
}

void CMenuSliderDlg::OnBUTTONEditSystemMid() 
{
	if(!m_sliderBarRef->m_visMov){
        AfxMessageBox("Start Visual Not Created yet..");
		return;
	}
	CSkeletalAnimationDlg dlg;
	dlg.m_globalShaderRef = NULL;
	dlg.m_elementDBREF = NULL;
	dlg.m_skillDatabaseREF = NULL;
	dlg.m_skeletonReferencePtr = m_sliderBarRef->m_visMov;
	dlg.GL_textureDataBase = GL_textureDataBase;
    dlg.directSoundRef = directSoundRef;
	dlg.DoModal();
}

void CMenuSliderDlg::OnBUTTONEditSystemTop() 
{
	if(!m_sliderBarRef->m_visStart){
        AfxMessageBox("Start Visual Not Created yet..");
		return;
	}
	CSkeletalAnimationDlg dlg;
	dlg.m_globalShaderRef = NULL;
	dlg.m_elementDBREF = NULL;
	dlg.m_skillDatabaseREF = NULL;
	dlg.m_skeletonReferencePtr = m_sliderBarRef->m_visStart;
	dlg.GL_textureDataBase = GL_textureDataBase;
    dlg.directSoundRef = directSoundRef;
	dlg.DoModal();
}

void CMenuSliderDlg::OnOK() 
{
	UpdateData(TRUE);

    m_sliderBarRef->m_boxClickSize         = m_boxClickSize;
	m_sliderBarRef->m_clickWidth           = m_clickWidth;
	m_sliderBarRef->m_fontSizeX            = m_fontSizeX;
	m_sliderBarRef->m_fontSizeY            = m_fontSizeY;
	m_sliderBarRef->m_visOffsetX           = m_offsetX;
	m_sliderBarRef->m_locEnd.x             = m_positionBottomX;
	m_sliderBarRef->m_locEnd.y             = m_positionBottomY;
	m_sliderBarRef->m_center.x             = m_positionMidX;
	m_sliderBarRef->m_center.y             = m_positionMidY;
	m_sliderBarRef->m_locStart.x           = m_positionTopX;
	m_sliderBarRef->m_locStart.y           = m_positionTopY;
	m_sliderBarRef->m_itemsThatAreVisible  = m_visibleItems;
	m_sliderBarRef->m_visSpacing           = m_visSpacing;
	m_sliderBarRef->m_controlID            = m_sliderControlID;

	m_sliderBarRef->m_red					= m_fontRed;
	m_sliderBarRef->m_green					= m_fontGreen;
	m_sliderBarRef->m_blue					= m_fontBlue;

	m_sliderBarRef->m_selectionDisabled = m_selectionAbilityDisabled.GetCheck();

	UpdateData(FALSE);
	
	CDialog::OnOK();
}
