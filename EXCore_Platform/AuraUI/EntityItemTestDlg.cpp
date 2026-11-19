// EntityItemTestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "EntityItemTestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEntityItemTestDlg dialog


CEntityItemTestDlg::CEntityItemTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEntityItemTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEntityItemTestDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CEntityItemTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEntityItemTestDlg)
	DDX_Control(pDX, IDC_LIST_ITEMS, m_itemList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEntityItemTestDlg, CDialog)
	//{{AFX_MSG_MAP(CEntityItemTestDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityItemTestDlg message handlers

void CEntityItemTestDlg::OnOK() 
{
    UpdateData(TRUE);

    m_selection = m_itemList.GetCurSel();

	UpdateData(FALSE);
	CDialog::OnOK();
}

BOOL CEntityItemTestDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();


    

	UpdateData(TRUE);
	
	for(POSITION posLoc = m_itemObjectListRef->GetHeadPosition();posLoc != NULL;)
	{
        CItemObj * itemObj = (CItemObj *)m_itemObjectListRef->GetNext(posLoc);
        m_itemList.AddString(itemObj->m_itemName);
	}

	m_itemList.SetCurSel(m_selection);
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
