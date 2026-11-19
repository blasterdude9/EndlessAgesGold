// BlockIpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "BlockIpDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBlockIpDlg dialog


CBlockIpDlg::CBlockIpDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CBlockIpDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBlockIpDlg)
	m_ipAdress = _T("");
	//}}AFX_DATA_INIT
}


void CBlockIpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBlockIpDlg)
	DDX_Control(pDX, IDC_LIST_IPBLOCKLIST, m_IPList);
	DDX_Text(pDX, IDC_EDIT_IpAdress, m_ipAdress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBlockIpDlg, CDialog)
	//{{AFX_MSG_MAP(CBlockIpDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBlockIpDlg message handlers

void CBlockIpDlg::OnButtonAdd() 
{
	UpdateData(TRUE);
	  m_IPBlockListRef->AddIpToBlockList(m_ipAdress);
	  
	UpdateData(FALSE);
	InListIps(m_IPBlockListRef->GetCount()-1);
}

void CBlockIpDlg::OnButtonDelete() 
{
	int sel = m_IPList.GetCurSel();
	if(sel == -1)
	{
       AfxMessageBox("Must select a ip");
       return;
	}

    POSITION posLoc = m_IPBlockListRef->FindIndex(sel);
	if(posLoc)
	{
       CBlockIpDlg * ipPtr = (CBlockIpDlg *)m_IPBlockListRef->GetAt(posLoc);
	   delete ipPtr;
	   ipPtr = 0;

       m_IPBlockListRef->RemoveAt(posLoc);
	}
	InListIps(sel-1);
}



BOOL CBlockIpDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    

	InListIps(0);
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}





BOOL CBlockIpDlg::InListIps(int sel)
{
	UpdateData(TRUE);
	 m_IPList.ResetContent();
     for(POSITION posLoc = m_IPBlockListRef->GetHeadPosition();posLoc != NULL;)
	 {
         CBlockIPObj * ipPtr = (CBlockIPObj *)m_IPBlockListRef->GetNext(posLoc);
	     m_IPList.AddString(ipPtr->m_ipAdress);	 
	 }
	 if(sel > -1)
		m_IPList.SetCurSel(sel);

	 UpdateData(FALSE);
	 return TRUE;
}
