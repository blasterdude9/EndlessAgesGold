// EditStartInventoryDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\EngineMainApp\resource.h"
#include "d3d9.h"
#include "EditStartInventoryDlg.h"
#include "globalstatusvariables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEditStartInventoryDlg dialog


CEditStartInventoryDlg::CEditStartInventoryDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEditStartInventoryDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEditStartInventoryDlg)
	m_itemCountLimit = 0;
	m_maxWieghtLimit = 0;
	m_quanity = 0;
	//}}AFX_DATA_INIT
}


void CEditStartInventoryDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEditStartInventoryDlg)
	DDX_Control(pDX, IDC_CHECK_armed, m_radioArmed);
	DDX_Control(pDX, IDC_COMBO_GlobalInventory, m_globalInventory);
	DDX_Control(pDX, IDC_LIST_Inventory, m_listInventory);
	DDX_Text(pDX, IDC_EDIT_ItemCountLimit, m_itemCountLimit);
	DDX_Text(pDX, IDC_EDIT_MaxWieghtLimit, m_maxWieghtLimit);
	DDX_Text(pDX, IDC_EDIT_Quanity, m_quanity);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEditStartInventoryDlg, CDialog)
	//{{AFX_MSG_MAP(CEditStartInventoryDlg)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_LBN_SELCHANGE(IDC_LIST_Inventory, OnSelchangeLISTInventory)
	ON_EN_CHANGE(IDC_EDIT_ItemCountLimit, OnChangeEDITItemCountLimit)
	ON_EN_CHANGE(IDC_EDIT_MaxWieghtLimit, OnChangeEDITMaxWieghtLimit)
	ON_BN_CLICKED(IDC_CHECK_armed, OnCHECKarmed)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditStartInventoryDlg message handlers

BOOL CEditStartInventoryDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
    UpdateData(TRUE);
	
	if(!m_autoCfgRef){
		m_autoCfgRef = new CCfgObjList();
	}
	
	InListInventory(0);
	InListGlobalItems(0); 
	m_quanity = 1;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEditStartInventoryDlg::OnButtonAdd() 
{
	    GlobalStatusVariables * gstatus = (GlobalStatusVariables *)m_gstatusMem;
	    UpdateData(TRUE);
        int type = m_globalInventory.GetCurSel();
        if(type == -1){
           MessageBox("must select a global Inventory Item...");
		   return;
		}

		CString selectedText;
		m_globalInventory.GetWindowTextA(selectedText);

		if(selectedText.GetLength() < 1)
		   return;
		
        CItemObj* itemPtr = gstatus->m_itemManager->GetItemByName(selectedText.GetBuffer());
		if(!itemPtr)
		    return;

        CCfgObj* cfgPtr = new CCfgObj();
		cfgPtr->m_globalCFG = itemPtr->m_globalID;
		if(m_radioArmed.GetCheck())
		   cfgPtr->m_armed     = TRUE;
		else
		   cfgPtr->m_armed     = FALSE;
		
		cfgPtr->m_quanity   = m_quanity;

		m_autoCfgRef->AddTail(cfgPtr);
		UpdateData(FALSE);
		InListInventory(m_autoCfgRef->GetCount());
}

void CEditStartInventoryDlg::OnButtonDelete() 
{
	UpdateData(TRUE);
	int type = m_listInventory.GetCurSel();
	if(type == -1)
	{
		MessageBox("must select a local Inventory Item...");
		return;
	}

	POSITION armPos = m_autoCfgRef->FindIndex(type);
	if(!armPos)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CCfgObj* cfgPtr = (CCfgObj*)m_autoCfgRef->GetAt(armPos);
	delete cfgPtr;
	cfgPtr=0;
	m_autoCfgRef->RemoveAt(armPos);

	UpdateData(FALSE);
	InListInventory(m_autoCfgRef->GetCount());
}

void CEditStartInventoryDlg::OnSelchangeLISTInventory() 
{
	UpdateData(TRUE);
        int type = m_listInventory.GetCurSel();
        if(type == -1){
           MessageBox("must select a local Inventory Item...");
		   return;
		}
		
	    POSITION armPos = m_autoCfgRef->FindIndex(type);
		if(!armPos)
			return;
            CCfgObj* cfgPtr = (CCfgObj*)m_autoCfgRef->GetAt(armPos);
		    m_quanity = cfgPtr->m_quanity;
			if(cfgPtr->m_armed)
			   m_radioArmed.SetCheck(1);
			else
			   m_radioArmed.SetCheck(0);
		    

		UpdateData(FALSE);
}

void CEditStartInventoryDlg::InListInventory(int sel) 
{
	GlobalStatusVariables * gstatus = (GlobalStatusVariables *)m_gstatusMem;
	UpdateData(TRUE);
	m_listInventory.ResetContent();
	    
	for(POSITION posLoc = m_autoCfgRef->GetHeadPosition();posLoc != NULL;){
        CCfgObj* cfgPtr = (CCfgObj*)m_autoCfgRef->GetNext(posLoc);
		if(m_itemDBRef)
		{
			CItemObj * iPtr = gstatus->m_itemManager->GetItemByID_LOS(cfgPtr->m_globalCFG);
            if(iPtr)
			{
				m_listInventory.AddString(iPtr->m_itemName);
				gstatus->m_itemManager->Unlock();
			}
			else
				m_listInventory.AddString("Unidentified");
		}
	}
	if(sel != -1)
	   m_listInventory.SetCurSel(sel);
	UpdateData(FALSE);
}

void CEditStartInventoryDlg::InListGlobalItems(int sel) 
{
	 GlobalStatusVariables * gstatus = (GlobalStatusVariables *)m_gstatusMem;
	 UpdateData(TRUE);
	 m_globalInventory.ResetContent();

	 for(unsigned int cLoop=0;cLoop<gstatus->m_itemManager->m_cellCount;cLoop++)
	 {
		 for(CAxLinkObj * pos = gstatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetHeadPosition_unsafe();pos != NULL;)
		 {
			 CItemObj * iPtr = (CItemObj *) gstatus->m_itemManager->m_listArray[0].m_axList[cLoop].GetNext_unsafe(&pos);
			 m_globalInventory.AddString(iPtr->m_itemName);
		 }
	 }	
	 UpdateData(FALSE);
	
	if(sel != -1)
	   m_globalInventory.SetCurSel(sel);
	
}



void CEditStartInventoryDlg::OnChangeEDITItemCountLimit() 
{
	UpdateData(TRUE);
	//m_modelRef->m_inventory->m_itemCountLimit = m_itemCountLimit;	    
    UpdateData(FALSE);
}

void CEditStartInventoryDlg::OnChangeEDITMaxWieghtLimit() 
{
	UpdateData(TRUE);
	//m_modelRef->m_inventory->m_wieghtLimit    = m_maxWieghtLimit;
	UpdateData(FALSE);
}

void CEditStartInventoryDlg::OnCHECKarmed() 
{
	UpdateData(TRUE);
        int type = m_listInventory.GetCurSel();
        if(type == -1){
           MessageBox("must select a local Inventory Item...");
		   return;
		}
		
	    POSITION armPos = m_autoCfgRef->FindIndex(type);
		if(!armPos)
			return;
            CCfgObj* cfgPtr = (CCfgObj*)m_autoCfgRef->GetAt(armPos);
			if(cfgPtr->m_armed){
			   m_radioArmed.SetCheck(0);
			   cfgPtr->m_armed = FALSE;
			}
			else{
			   m_radioArmed.SetCheck(1);
			   cfgPtr->m_armed = TRUE;
			}

		UpdateData(FALSE);
	
}
