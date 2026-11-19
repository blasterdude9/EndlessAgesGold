// CharonaDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "CharonaDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCharonaDlg dialog


CCharonaDlg::CCharonaDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCharonaDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCharonaDlg)
	m_endDistance = 0.0f;
	m_fadeTime = 0;
	m_size = 0.0f;
	m_startDistance = 0.0f;
	m_visibleDistance = 0.0f;
	//}}AFX_DATA_INIT
}


void CCharonaDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCharonaDlg)
	DDX_Control(pDX, IDC_CHECK_Enable, m_enable);
	DDX_Control(pDX, IDC_COMBO_TextureSelection, m_textureList);
	DDX_Text(pDX, IDC_EDIT_endDistance, m_endDistance);
	DDX_Text(pDX, IDC_EDIT_fadeTime, m_fadeTime);
	DDX_Text(pDX, IDC_EDIT_size, m_size);
	DDX_Text(pDX, IDC_EDIT_startDistance, m_startDistance);
	DDX_Text(pDX, IDC_EDIT_visibleDistance, m_visibleDistance);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCharonaDlg, CDialog)
	//{{AFX_MSG_MAP(CCharonaDlg)
	ON_BN_CLICKED(IDC_CHECK_Enable, OnCHECKEnable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCharonaDlg message handlers

void CCharonaDlg::OnOK() 
{
	UpdateData(TRUE);
	if(m_charonaRef){
	   m_charonaRef->m_textureIndex    = m_textureList.GetCurSel()-1;
       if(m_charonaRef->m_textureIndex < 0){
		  m_charonaRef->m_textureIndex = 0;
          AfxMessageBox("Invalid texture select");

	   }

	   POSITION tPos = m_textureDataBase->FindIndex(m_charonaRef->m_textureIndex);
       if(tPos){
          CTextureDBObj * texture = (CTextureDBObj *)m_textureDataBase->GetAt(tPos);
          m_charonaRef->m_textureName = texture->List_textureName;
	   }

       m_charonaRef->m_endDistance     = m_endDistance;
   	   m_charonaRef->m_fadeTime        = m_fadeTime;
	   m_charonaRef->m_size            = m_size;
	   m_charonaRef->m_startDistance   = m_startDistance;
	   m_charonaRef->m_visibleDistance = m_visibleDistance;
	   m_charonaRef->InitializeVis();
	}
    UpdateData(FALSE);

	CDialog::OnOK();
}

BOOL CCharonaDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);
	int trace=0;
    m_textureList.AddString("NONE");
	for(POSITION posLoc = m_textureDataBase->GetHeadPosition();posLoc!=NULL;)
	{
			CTextureDBObj * texture = (CTextureDBObj *)m_textureDataBase->GetNext(posLoc);
            CString num = operator+(cablib.ConvToString(trace)," ");
			num = operator+(num,texture->List_textureName);
			m_textureList.AddString(num);
			trace++;
	}
	if(m_charonaRef){//enabled charona
       m_textureList.SetCurSel(m_charonaRef->m_textureIndex+1);
       m_endDistance        = m_charonaRef->m_endDistance;
   	   m_fadeTime           = m_charonaRef->m_fadeTime;
	   m_size               = m_charonaRef->m_size;
	   m_startDistance      = m_charonaRef->m_startDistance;
	   m_visibleDistance    = m_charonaRef->m_visibleDistance;
	   m_enable.SetCheck(1);
	}//end enabled charona
    else{//need default values
       m_textureList.SetCurSel(0);
       m_endDistance        = 2000;
   	   m_fadeTime           = 1000;
	   m_size               = 200;
	   m_startDistance      = 100;
	   m_visibleDistance    = 3000;
	}//end need default values

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCharonaDlg::OnCHECKEnable() 
{
	UpdateData(TRUE);
	if(m_charonaRef){
       m_charonaRef->SafeDelete();
	   delete m_charonaRef;
	   m_charonaRef=0;
	}
	else{
       m_charonaRef = new CCharonaObj();
	   m_charonaRef->m_textureIndex    = m_textureList.GetCurSel()-1;
       POSITION posLoc = m_textureDataBase->FindIndex(m_charonaRef->m_textureIndex);
	   if(posLoc){
	      CTextureDBObj * texture = (CTextureDBObj *)m_textureDataBase->GetAt(posLoc);
	      m_charonaRef->m_textureName = texture->List_textureName;
	   }

       m_charonaRef->m_endDistance     = m_endDistance;
   	   m_charonaRef->m_fadeTime        = m_fadeTime;
	   m_charonaRef->m_size            = m_size;
	   m_charonaRef->m_startDistance   = m_startDistance;
	   m_charonaRef->m_visibleDistance = m_visibleDistance;
	   m_charonaRef->InitializeVis();
	}
	UpdateData(FALSE);
}
