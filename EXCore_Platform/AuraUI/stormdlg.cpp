// StormDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "StormDlg.h"
#include "AddLightningStrikeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStormDlg dialog


CStormDlg::CStormDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CStormDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CStormDlg)
	m_duration = 0;
	m_frequency = 0;
	m_particleLink = 0;
	m_stormRadius = 0.0f;
	m_strikeDepth = 0.0f;
	m_strikeDuration = 0;
	m_strikesPerMinute = 0;
	m_strikeSpawnHieght = 0.0f;
	m_strikeWidth = 0.0f;
	m_transitionTime = 0;
	m_stormName = _T("");
	//}}AFX_DATA_INIT
}


void CStormDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CStormDlg)
	DDX_Control(pDX, IDC_LIST_StrikeVariationList, m_strikeVariationsList);
	DDX_Text(pDX, IDC_EDIT_Duration, m_duration);
	DDX_Text(pDX, IDC_EDIT_frequency, m_frequency);
	DDX_Text(pDX, IDC_EDIT_ParticleLink, m_particleLink);
	DDX_Text(pDX, IDC_EDIT_StormRadius, m_stormRadius);
	DDX_Text(pDX, IDC_EDIT_StrikeDepth, m_strikeDepth);
	DDX_Text(pDX, IDC_EDIT_StrikeDuration, m_strikeDuration);
	DDX_Text(pDX, IDC_EDIT_StrikesPerMinute, m_strikesPerMinute);
	DDX_Text(pDX, IDC_EDIT_StrikesSpawnHieght, m_strikeSpawnHieght);
	DDX_Text(pDX, IDC_EDIT_StrikesWidth, m_strikeWidth);
	DDX_Text(pDX, IDC_EDIT_transitionTime, m_transitionTime);
	DDX_Text(pDX, IDC_EDIT_StormName, m_stormName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CStormDlg, CDialog)
	//{{AFX_MSG_MAP(CStormDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADDStrike, OnBUTTONADDStrike)
	ON_BN_CLICKED(IDC_BUTTON_DeleteStrike, OnBUTTONDeleteStrike)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStormDlg message handlers
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
BOOL CStormDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	UpdateData(TRUE);

	m_duration          = m_stormRef->m_duration;
	m_frequency         = m_stormRef->m_frequency;
	m_particleLink      = m_stormRef->m_particleSystemRef;
	m_stormRadius       = m_stormRef->m_stormRadius;
	m_strikeDepth       = m_stormRef->m_strikeDepth;
	m_strikeDuration    = m_stormRef->m_strikeDuration;
	m_strikesPerMinute  = m_stormRef->m_totalStrikeCountPerMinute;
	m_strikeSpawnHieght = m_stormRef->m_strikeSpawnHieght;
	m_strikeWidth       = m_stormRef->m_strikePanelWidth;
	m_transitionTime    = m_stormRef->m_stormTransition;
	m_stormName         = m_stormRef->m_stormName;
    
	UpdateData(FALSE);
	
    InListStrikeVariations(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CStormDlg::OnBUTTONADDStrike() 
{
   CAddLightningStrikeDlg dlg;
   dlg.m_textureDataBaseRef = m_textureDataBaseRef;
   if(dlg.DoModal() == IDOK){
      char buffer[256];
	  if(dlg.m_textureIndexSelection != -1)
	  {
         POSITION texturePosition = m_textureDataBaseRef->FindIndex(dlg.m_textureIndexSelection);
		 if(texturePosition)
		 {
			 CTextureDBObj * texturePtr = (CTextureDBObj *)m_textureDataBaseRef->GetAt(texturePosition);
             lstrcpy(buffer,texturePtr->List_textureName);
		 }
	  }

	  
         m_stormRef->AddLightningStrike(buffer,dlg.m_sndID,dlg.m_soundFactor);

   }
   InListStrikeVariations(m_stormRef->m_VariationLightningCount-1);
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CStormDlg::InListStrikeVariations(int sel)
{
 UpdateData(TRUE);
  m_strikeVariationsList.ResetContent();
   for(int loop=0;loop<m_stormRef->m_VariationLightningCount;loop++)
   {
       m_strikeVariationsList.AddString(m_stormRef->m_strikeVariationArray[loop].textureName);
   }
   if(sel > -1)
      m_strikeVariationsList.SetCurSel(sel);

 UpdateData(FALSE);
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CStormDlg::OnOK() 
{
	UpdateData(TRUE);

	m_stormRef->m_duration                      = m_duration;
	m_stormRef->m_frequency                     = m_frequency;
	m_stormRef->m_particleSystemRef             = m_particleLink;
	m_stormRef->m_stormRadius                   = m_stormRadius;
	m_stormRef->m_strikeDepth                   = m_strikeDepth;
	m_stormRef->m_strikeDuration                = m_strikeDuration;
	m_stormRef->m_totalStrikeCountPerMinute     = m_strikesPerMinute;
	m_stormRef->m_strikeSpawnHieght             = m_strikeSpawnHieght;
	m_stormRef->m_strikePanelWidth              = m_strikeWidth;
	m_stormRef->m_stormTransition               = m_transitionTime;
	m_stormRef->m_stormName                     = m_stormName;
    
	UpdateData(FALSE);

	CDialog::OnOK();
}
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
//-------------------------------------------------------------------------//
void CStormDlg::OnBUTTONDeleteStrike() 
{
	int strikeSelection = m_strikeVariationsList.GetCurSel();
	if(strikeSelection == -1)
	{
		MessageBox("Must select a strike");
		return;
	}

	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;

	m_stormRef->RemoveLightningStrike(strikeSelection);
	InListStrikeVariations(m_stormRef->m_VariationLightningCount-1);
}
