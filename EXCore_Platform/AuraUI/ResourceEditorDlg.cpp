// ResourceEditorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ResourceEditorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CResourceEditorDlg dialog


CResourceEditorDlg::CResourceEditorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CResourceEditorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CResourceEditorDlg)
	m_name = _T("");
	m_regenerationVar = 0.0f;
	m_type = 0;
	m_amount = 0;
	m_maxAmount = 0;
	//}}AFX_DATA_INIT
}


void CResourceEditorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CResourceEditorDlg)
	DDX_Control(pDX, IDC_CHECK_RegenEnable, m_enableRegenerate);
	DDX_Control(pDX, IDC_CHECK_maxAmountEnable, m_enableMaxAmount);
	DDX_Control(pDX, IDC_LIST_Resources, m_listResource);
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	DDX_Text(pDX, IDC_EDIT_regenerationVariable, m_regenerationVar);
	DDX_Text(pDX, IDC_EDIT_resourceType, m_type);
	DDX_Text(pDX, IDC_EDIT_amount, m_amount);
	DDX_Text(pDX, IDC_EDIT_maxAmount, m_maxAmount);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CResourceEditorDlg, CDialog)
	//{{AFX_MSG_MAP(CResourceEditorDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnButtonAdd)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceValues, OnBUTTONReplaceValues)
	ON_BN_CLICKED(IDC_CHECK_maxAmountEnable, OnCHECKmaxAmountEnable)
	ON_BN_CLICKED(IDC_CHECK_RegenEnable, OnCHECKRegenEnable)
	ON_LBN_SELCHANGE(IDC_LIST_Resources, OnSelchangeLISTResources)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CResourceEditorDlg message handlers

void CResourceEditorDlg::OnButtonAdd() 
{
	if(!resourceListRef)
		resourceListRef = new CResourceObjectList;
    UpdateData(TRUE);
    
	/*resourceListRef->AddResource(m_name,
	                             m_type,
								 m_amount,
								 m_enableMaxVar,
								 m_maxAmount,
								 m_enableRegenerationVar,
								 m_regenerationVar);*/
	resourceListRef->AddResource(m_name,
	                             m_type,
								 m_amount,
								 m_enableMaxVar,
								 m_maxAmount,
								 m_enableRegenerationVar,
								 (int)m_regenerationVar);
	//WARNING: C4244 --> conversion from float to int, possible loss of data

	UpdateData(FALSE);
	InList(resourceListRef->GetCount());
}

void CResourceEditorDlg::OnButtonDelete() 
{
	int resIndex = m_listResource.GetCurSel();
	if(resIndex == -1)
	{
		MessageBox("Must select a resource");
		return;
	}

	POSITION posLoc = resourceListRef->FindIndex(resIndex);
	if(!posLoc)
		return;

	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CResourceObject * rPtr = (CResourceObject *)resourceListRef->GetAt(posLoc);
	delete rPtr;
	rPtr=0;
	resourceListRef->RemoveAt(posLoc);
	InList(resIndex-1);
}

void CResourceEditorDlg::OnBUTTONReplaceValues() 
{
	int resIndex = m_listResource.GetCurSel();
	if(resIndex == -1){
       MessageBox("Must select a resource");
       return;
	}

	POSITION posLoc = resourceListRef->FindIndex(resIndex);
	if(!posLoc)
		return;

        CResourceObject * rPtr = (CResourceObject *)resourceListRef->GetAt(posLoc);
		UpdateData(TRUE);
    
	    rPtr->m_resourceName           = m_name; 
        rPtr->m_type                   = m_type;
        rPtr->m_amount                 = m_amount;
        rPtr->m_enableMaxAmountVar     = m_enableMaxVar;
	    rPtr->m_maxAmount              = m_maxAmount;
	    rPtr->m_regenEnable            = m_enableRegenerationVar;

		/*rPtr->m_regenRatePerSecond     = m_regenerationVar;*/

		rPtr->m_regenRatePerSecond     = (int)m_regenerationVar;
		//WARNING: C4244 --> conversion from float to int, possible loss of data

	    UpdateData(FALSE);
		InList(resIndex);
}

void CResourceEditorDlg::OnCHECKmaxAmountEnable() 
{
	if(m_enableMaxVar){
       m_enableMaxAmount.SetCheck(FALSE);
       m_enableMaxVar = FALSE;
	}
	else{
       m_enableMaxAmount.SetCheck(TRUE);
       m_enableMaxVar = TRUE;
	}
}

void CResourceEditorDlg::OnCHECKRegenEnable() 
{
    if(m_enableRegenerationVar){
       m_enableRegenerate.SetCheck(FALSE);
       m_enableRegenerationVar = FALSE;
	}
	else{
       m_enableRegenerate.SetCheck(TRUE);
       m_enableRegenerationVar = TRUE;
	}	
}

void CResourceEditorDlg::InList(int sel) 
{
	m_listResource.ResetContent();
	if(!resourceListRef)
		return;
    
	for(POSITION posLoc = resourceListRef->GetHeadPosition();posLoc != NULL;)
	{
        CResourceObject * rPtr = (CResourceObject *)resourceListRef->GetNext(posLoc);
        m_listResource.AddString(rPtr->m_resourceName);
	}
    if(sel >= 0)
	   m_listResource.SetCurSel(sel);

}

void CResourceEditorDlg::OnSelchangeLISTResources() 
{
int resIndex = m_listResource.GetCurSel();
	if(resIndex == -1){
       return;
	}

	POSITION posLoc = resourceListRef->FindIndex(resIndex);
	if(!posLoc)
		return;

        CResourceObject * rPtr = (CResourceObject *)resourceListRef->GetAt(posLoc);
		UpdateData(TRUE);
    
	    m_name                  = rPtr->m_resourceName; 
        m_type                  = rPtr->m_type;
        m_amount                = rPtr->m_amount;
        m_enableMaxVar          = rPtr->m_enableMaxAmountVar;
	    m_maxAmount             = rPtr->m_maxAmount;
	    m_enableRegenerationVar = rPtr->m_regenEnable;
	    /*m_regenerationVar       = rPtr->m_regenRatePerSecond;*/

		m_regenerationVar       = (float)rPtr->m_regenRatePerSecond;
		//WARNING: C4244 --> conversion from int to float, possible loss of data

	    
		if(m_enableMaxVar)
           m_enableMaxAmount.SetCheck(TRUE);
	    else
           m_enableMaxAmount.SetCheck(FALSE);
       
        if(m_enableRegenerationVar)
           m_enableRegenerate.SetCheck(TRUE);
	    else
           m_enableRegenerate.SetCheck(FALSE);	
			
		UpdateData(FALSE);

		InList(resIndex);
}

BOOL CResourceEditorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_enableMaxVar            = FALSE;
	m_enableRegenerationVar   = FALSE;
	InList(-1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
