// RuntimeEntityViewerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "RuntimeEntityViewerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRuntimeEntityViewerDlg dialog


CRuntimeEntityViewerDlg::CRuntimeEntityViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRuntimeEntityViewerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRuntimeEntityViewerDlg)
	m_networkDefinedID = 0;
	m_targetNetworkID = 0;
	m_worldPositionX = 0.0f;
	m_worldPositionY = 0.0f;
	m_worldPositionZ = 0.0f;
	m_localTraceNumber = 0;
	m_targetTraceNumber = 0;
	//}}AFX_DATA_INIT
}


void CRuntimeEntityViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRuntimeEntityViewerDlg)
	DDX_Control(pDX, IDC_CHECK_hasTarget, m_hasTarget);
	DDX_Control(pDX, IDC_CHECK_isAI, m_isAI);
	DDX_Text(pDX, IDC_EDIT_NetworkDefinedID, m_networkDefinedID);
	DDX_Text(pDX, IDC_EDIT_TargetID, m_targetNetworkID);
	DDX_Text(pDX, IDC_EDIT_WorldPosX, m_worldPositionX);
	DDX_Text(pDX, IDC_EDIT_WorldPosY, m_worldPositionY);
	DDX_Text(pDX, IDC_EDIT_WorldPosZ, m_worldPositionZ);
	DDX_Text(pDX, IDC_EDIT_LocalTraceNumber, m_localTraceNumber);
	DDX_Text(pDX, IDC_EDIT_TargetTraceNumber, m_targetTraceNumber);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRuntimeEntityViewerDlg, CDialog)
	//{{AFX_MSG_MAP(CRuntimeEntityViewerDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRuntimeEntityViewerDlg message handlers

BOOL CRuntimeEntityViewerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    UpdateData(TRUE);
	D3DVECTOR wldSpcPosition;
	m_runtimeEntityRef->List_baseFrame->GetPosition(NULL,&wldSpcPosition);
    m_networkDefinedID         = m_runtimeEntityRef->List_networkDefinedID;
	
	m_worldPositionX           = wldSpcPosition.x;
	m_worldPositionY           = wldSpcPosition.y;
	m_worldPositionZ           = wldSpcPosition.z;
	m_localTraceNumber         = m_runtimeEntityRef->List_traceNumber;
	

	//ai section
	if(m_runtimeEntityRef->m_aiObj)
	{
       m_isAI.SetCheck(1);
    
	   if(m_runtimeEntityRef->List_targetRef)
	   {
	      m_hasTarget.SetCheck(1);
          m_targetNetworkID          = m_runtimeEntityRef->List_targetRef->List_networkDefinedID;
          m_targetTraceNumber        = m_runtimeEntityRef->List_targetRef->List_traceNumber;
	   }
	}
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
