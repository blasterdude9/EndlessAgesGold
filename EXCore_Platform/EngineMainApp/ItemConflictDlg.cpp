// ItemConflictDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "ItemConflictDlg.h"
#include "globalstatusvariables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CItemConflictDlg dialog


CItemConflictDlg::CItemConflictDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CItemConflictDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CItemConflictDlg)
	m_currentName = _T("");
	m_reference = _T("");
	//}}AFX_DATA_INIT
}


void CItemConflictDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CItemConflictDlg)
	DDX_Control(pDX, IDC_COMBO_GlobalInventory, m_globalInvDB);
	DDX_Text(pDX, IDC_EDIT_currentItemName, m_currentName);
	DDX_Text(pDX, IDC_EDIT_Reference, m_reference);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CItemConflictDlg, CDialog)
	//{{AFX_MSG_MAP(CItemConflictDlg)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_replaceAllSimilar, OnBUTTONreplaceAllSimilar)
	ON_BN_CLICKED(IDC_BUTTON_Replace, OnBUTTONReplace)
	ON_BN_CLICKED(IDC_BUTTON_KillSessionLoop, OnBUTTONKillSessionLoop)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CItemConflictDlg message handlers

BOOL CItemConflictDlg::OnInitDialog() 
{
	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;
	CDialog::OnInitDialog();
	UpdateData(TRUE);

    CString tempCnv = m_currentName;
	tempCnv.MakeUpper();

	int tracer = 0;
	int selection = m_lastSelected;

    for(unsigned int cLoop=0;cLoop<gStatus->m_itemManager->m_cellCount;cLoop++)
	{
		 for(CAxLinkObj * pos = gStatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
		 {
			CItemObj * glidPtr = (CItemObj *) gStatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
			m_globalInvDB.AddString(glidPtr->m_itemName);
			if(glidPtr->m_itemName.CompareNoCase(tempCnv)==0)
			   selection = tracer;
			tracer++;
		 }
	}	
	m_globalInvDB.SetCurSel(selection);

    m_returnCommand = 0;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CItemConflictDlg::OnButtonDelete() 
{
	m_returnCommand = 0;
	CDialog::OnOK();
}

void CItemConflictDlg::OnBUTTONreplaceAllSimilar() 
{
	int selection = m_globalInvDB.GetCurSel();
	if(selection == -1)
	{
	   AfxMessageBox("Must select a global inv object");
       return;
	}

	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;

	m_lastSelected = selection;

	CString itemName;
    m_globalInvDB.GetWindowTextA(itemName);

    CItemObj * glidPtr = gStatus->m_itemManager->GetItemByName(itemName.GetBuffer());
	if(glidPtr)
	for(POSITION posLoc = m_currentInventory->GetHeadPosition();posLoc != NULL;)
	{
        CInventoryObj * invPtr = (CInventoryObj *)m_currentInventory->GetNext(posLoc);
        if(invPtr->m_itemData->m_globalID == m_currentItemGlid)
		{
           invPtr->m_itemData->SafeDelete();
		   delete invPtr->m_itemData;
		   invPtr->m_itemData = 0;
           glidPtr->Clone(&invPtr->m_itemData);
		}
	}
	m_returnCommand = 10;
	CDialog::OnOK();
}

void CItemConflictDlg::OnBUTTONReplace() 
{
	int selection = m_globalInvDB.GetCurSel();
	if(selection == -1)
	{
	   AfxMessageBox("Must select a global inv object");
       return;
	}
	m_lastSelected = selection;

	GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gStatusMem;

	CString itemName;
    m_globalInvDB.GetWindowTextA(itemName);

	CItemObj * glidPtr = gStatus->m_itemManager->GetItemByName(itemName.GetBuffer());
	if(!glidPtr)
		return;
	
	
        m_currentInvItem->m_itemData->SafeDelete();
		delete m_currentInvItem->m_itemData;
        m_currentInvItem->m_itemData = 0;
		glidPtr->Clone(&m_currentInvItem->m_itemData);


	CDialog::OnOK();
}

void CItemConflictDlg::OnOK() 
{
	m_returnCommand = 1;
	CDialog::OnOK();
}

void CItemConflictDlg::OnBUTTONKillSessionLoop() 
{
	m_returnCommand = 5;
	CDialog::OnOK();
}
