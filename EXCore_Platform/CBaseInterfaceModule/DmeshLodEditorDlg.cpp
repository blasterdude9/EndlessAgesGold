// DmeshLodEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "DmeshLodEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDmeshLodEditorDlg dialog


CDmeshLodEditorDlg::CDmeshLodEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDmeshLodEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDmeshLodEditorDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDmeshLodEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDmeshLodEditorDlg)
	DDX_Control(pDX, IDC_LIST_selectionList, m_selectionList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDmeshLodEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CDmeshLodEditorDlg)
	ON_BN_CLICKED(IDC_BTN_Delete, OnBTNDelete)
	ON_BN_CLICKED(IDC_BTN_Replace, OnBTNReplace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDmeshLodEditorDlg message handlers

void CDmeshLodEditorDlg::OnBTNDelete() 
{
	UpdateData(TRUE);
	m_listSelection = m_selectionList.GetCurSel();
	UpdateData(FALSE);
	m_selection = 3;
	CDialog::OnOK();
}

void CDmeshLodEditorDlg::OnBTNReplace() 
{
    UpdateData(TRUE);
	m_listSelection = m_selectionList.GetCurSel();
	m_selection = 2;
	UpdateData(FALSE);
	CDialog::OnOK();
}

void CDmeshLodEditorDlg::OnOK() 
{
	UpdateData(TRUE);
	m_listSelection = 1000;
	m_selection = 1;
	UpdateData(FALSE);
	CDialog::OnOK();
}

BOOL CDmeshLodEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);
	m_listSelection = 0;
	m_selectionList.SetCurSel(0);
	int trace = 0;
	
	if(m_lodChainRef)
	for(POSITION posLoc = m_lodChainRef->GetHeadPosition();posLoc != NULL;)
	{
        CDeformableVisObj * dVis = (CDeformableVisObj *)m_lodChainRef->GetNext(posLoc);
        m_selectionList.AddString(operator+("Level : ",m_cablib.ConvToString(trace)));
	trace++;
	}

	if(m_lodChainRefHiarchy)
	for(POSITION posLoc = m_lodChainRefHiarchy->GetHeadPosition();posLoc != NULL;)
	{
        CHiarcleVisObj * dVis = (CHiarcleVisObj *)m_lodChainRefHiarchy->GetNext(posLoc);
        m_selectionList.AddString(operator+("Level : ",m_cablib.ConvToString(trace)));
	trace++;
	}
	UpdateData(FALSE);
	
    m_selection = -1;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
