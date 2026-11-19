// AddAttachmentTypeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "AddAttachmentTypeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddAttachmentTypeDlg dialog


CAddAttachmentTypeDlg::CAddAttachmentTypeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddAttachmentTypeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddAttachmentTypeDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_dimCfgIndex			= -1;
    m_dimCfgChangeTo		= -1;
}


void CAddAttachmentTypeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddAttachmentTypeDlg)
	DDX_Control(pDX, IDC_COMBO_Attachments, m_attachments);
	DDX_Control(pDX, IDC_LIST_currentItems, m_currentItems);
    DDX_Control(pDX, IDC_COMBO_edbID, m_edbList);
	DDX_Text(pDX, IDC_EDIT_DimCfgIndex, m_dimCfgIndex);
    DDX_Text(pDX, IDC_EDIT_dimCfgChangeTo, m_dimCfgChangeTo);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddAttachmentTypeDlg, CDialog)
	//{{AFX_MSG_MAP(CAddAttachmentTypeDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_atUpdateItem, &CAddAttachmentTypeDlg::OnBnClickedButtonatupdateitem)
	ON_LBN_SELCHANGE(IDC_LIST_currentItems, &CAddAttachmentTypeDlg::OnLbnSelchangeListcurrentitems)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddAttachmentTypeDlg message handlers

void CAddAttachmentTypeDlg::OnButtonAdd() 
{
	
	int selection = m_attachments.GetCurSel();
    if(selection == -1){
       MessageBox("must select a attachment");
	   return;
	}

	int edbSelection = m_edbList.GetCurSel();
    if(edbSelection == -1){
       MessageBox("must select a EDB id");
	   return;
	}
	UpdateData(TRUE);

	

	POSITION posLoc = m_armedInventoryListRef->FindIndex(selection);
	if(!posLoc)
		return;
        CArmedInventoryObj * attachment = (CArmedInventoryObj *)m_armedInventoryListRef->GetAt(posLoc);
        CAttachmentRefObj * attachRef = new CAttachmentRefObj();

        CMovementObj * edbEntityRef = m_enitityDB->GetObjByIndex(edbSelection - 1);
		if(edbEntityRef)
		   attachRef->m_raceBind			= edbEntityRef->m_uniqueId;
		else
		   attachRef->m_raceBind			= -1;

		attachRef->m_dimIndex				= m_dimCfgIndex;
		attachRef->m_dimSwitchTo			= m_dimCfgChangeTo;

		attachRef->m_attachmentType = attachment->m_type;
		m_itemRef->m_attachmentRefList->AddTail(attachRef);
        InListCurrentItems(m_itemRef->m_attachmentRefList->GetCount()-1);
	UpdateData(FALSE);
}

void CAddAttachmentTypeDlg::OnButtonDelete() 
{	
	int selection = m_currentItems.GetCurSel();
	if(selection == -1)
	{
		MessageBox("must select a attachment");
		return;
	}
	UpdateData(TRUE);

	POSITION posLoc = m_itemRef->m_attachmentRefList->FindIndex(selection);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;

	CAttachmentRefObj * attachmentRef = (CAttachmentRefObj *)m_itemRef->m_attachmentRefList->GetAt(posLoc);
	delete attachmentRef;
	attachmentRef=0;
	m_itemRef->m_attachmentRefList->RemoveAt(posLoc);
	InListCurrentItems(selection-1);

	UpdateData(FALSE);
}

BOOL CAddAttachmentTypeDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
    
	UpdateData(TRUE);
  
	
	InListCurrentItems(0);

	InListAttachments(-1);

	InListEDB(-1);

	UpdateData(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddAttachmentTypeDlg::InListEDB(int sel)
{
	m_edbList.ResetContent();
	m_edbList.AddString("ALL");
    for(POSITION posLoc = m_enitityDB->GetHeadPosition();posLoc != NULL;)
	{
        CMovementObj * mPtr = (CMovementObj *)m_enitityDB->GetNext(posLoc);
        m_edbList.AddString(mPtr->List_name);
	}
	m_edbList.SetCurSel(sel);
}

void CAddAttachmentTypeDlg::InListAttachments(int sel)
{
	m_attachments.ResetContent();
    for(POSITION posLoc = m_armedInventoryListRef->GetHeadPosition();posLoc != NULL;)
	{
        CArmedInventoryObj * attachment = (CArmedInventoryObj *)m_armedInventoryListRef->GetNext(posLoc);
        m_attachments.AddString(attachment->m_name);
	}
	m_attachments.SetCurSel(sel);
}


void CAddAttachmentTypeDlg::InListCurrentItems(int sel)
{
	m_currentItems.ResetContent();
	if(m_itemRef->m_attachmentRefList)
    for(POSITION posLoc = m_itemRef->m_attachmentRefList->GetHeadPosition();posLoc != NULL;)
	{
        CAttachmentRefObj * ref = (CAttachmentRefObj *)m_itemRef->m_attachmentRefList->GetNext(posLoc);
        CArmedInventoryObj * attachment = m_armedInventoryListRef->GetByType(ref->m_attachmentType);
		if(!attachment)
		  m_currentItems.AddString("Unidentified");
		else{
          CString base = operator+(attachment->m_name,":Type::");
		  m_currentItems.AddString(operator+(base,m_cbLib.ConvToString(ref->m_attachmentType)));
		}
	}
	m_currentItems.SetCurSel(sel);
}
void CAddAttachmentTypeDlg::OnBnClickedButtonatupdateitem()
{
	
    int armedInvenSel = m_attachments.GetCurSel();
    if(armedInvenSel == -1){
       MessageBox("must select a attachment visual");
	   return;
	}

	int selection = m_currentItems.GetCurSel();
    if(selection == -1){
       MessageBox("must select a attachment");
	   return;
	}

	int edbSelection = m_edbList.GetCurSel();
    if(edbSelection == -1){
       MessageBox("must select a EDB id");
	   return;
	}
	UpdateData(TRUE);

    POSITION posLoc = m_armedInventoryListRef->FindIndex(armedInvenSel);
	if(!posLoc)
		return;
    CArmedInventoryObj * attachment = (CArmedInventoryObj *)m_armedInventoryListRef->GetAt(posLoc);

    
    posLoc = m_itemRef->m_attachmentRefList->FindIndex(selection);
	if(!posLoc)
		return;
        CAttachmentRefObj * attachmentRef	= (CAttachmentRefObj *)m_itemRef->m_attachmentRefList->GetAt(posLoc);
        
		CMovementObj * edbEntityRef = m_enitityDB->GetObjByIndex(edbSelection - 1);
		if(edbEntityRef)
		   attachmentRef->m_raceBind			= edbEntityRef->m_uniqueId;
		else
		   attachmentRef->m_raceBind = -1;
		attachmentRef->m_attachmentType		 = attachment->m_type;

		attachmentRef->m_dimIndex		     = m_dimCfgIndex;
		attachmentRef->m_dimSwitchTo		 = m_dimCfgChangeTo;

	InListCurrentItems(selection);
	
	UpdateData(FALSE);
}

void CAddAttachmentTypeDlg::OnLbnSelchangeListcurrentitems()
{
	 int selection = m_currentItems.GetCurSel();
     if(selection == -1)
	    return;

	 POSITION posLoc = m_itemRef->m_attachmentRefList->FindIndex(selection);
	 if(!posLoc)
		 return;
     CAttachmentRefObj * attachmentRef	= (CAttachmentRefObj *)m_itemRef->m_attachmentRefList->GetAt(posLoc);

	 int phyIndex = m_enitityDB->GetPhysicalIndexByUniqueID(attachmentRef->m_raceBind);
     UpdateData(TRUE);
	 m_dimCfgIndex		= attachmentRef->m_dimIndex;
	 m_dimCfgChangeTo	= attachmentRef->m_dimSwitchTo;

	 UpdateData(FALSE);
     m_edbList.SetCurSel(phyIndex + 1);
	 int atSel = m_armedInventoryListRef->GetIndexByType(attachmentRef->m_attachmentType);
	 m_attachments.SetCurSel(atSel);
}
