// NodeImportDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "NodeImportDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNodeImportDlg dialog


CNodeImportDlg::CNodeImportDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNodeImportDlg::IDD, pParent)
{
	m_selectionArray = NULL;
	m_selectionCount = 0;
	//{{AFX_DATA_INIT(CNodeImportDlg)
	m_popoutBoxMaxX = 0.0f;
	m_popoutBoxMaxY = 0.0f;
	m_popoutBoxMaxZ = 0.0f;
	m_popoutBoxSizeMinX = 0.0f;
	m_popoutBoxSizeMinY = 0.0f;
	m_popoutBoxSizeMinZ = 0.0f;
	m_moveX = 0.0f;
	m_moveY = 0.0f;
	m_moveZ = 0.0f;
	m_scaleX = 1.0f;
	m_scaleY = 1.0f;
	m_scaleZ = 1.0f;
	//}}AFX_DATA_INIT
}


void CNodeImportDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNodeImportDlg)
	DDX_Control(pDX, IDC_LIST_ReferenceLibDB, m_referenceLibDB);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMaxX, m_popoutBoxMaxX);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMaxY, m_popoutBoxMaxY);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMaxZ, m_popoutBoxMaxZ);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMinX, m_popoutBoxSizeMinX);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMinY, m_popoutBoxSizeMinY);
	DDX_Text(pDX, IDC_EDIT_PopoutBoxSizeMinZ, m_popoutBoxSizeMinZ);
	DDX_Text(pDX, IDC_EDIT_MOVEX, m_moveX);
	DDX_Text(pDX, IDC_EDIT_MOVEY, m_moveY);
	DDX_Text(pDX, IDC_EDIT_MOVEZ, m_moveZ);
	DDX_Text(pDX, IDC_EDIT_SCALEX, m_scaleX);
	DDX_Text(pDX, IDC_EDIT_SCALEY, m_scaleY);
	DDX_Text(pDX, IDC_EDIT_SCALEZ, m_scaleZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNodeImportDlg, CDialog)
	//{{AFX_MSG_MAP(CNodeImportDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNodeImportDlg message handlers

void CNodeImportDlg::OnOK() 
{
	UpdateData(TRUE);
    
	m_selectionCount = m_referenceLibDB.GetSelCount();
	if(m_selectionCount < 1)
	{
       AfxMessageBox("Must select a reference object");
       return;
	}
    if(m_selectionArray > 0){
        delete [] m_selectionArray;
		m_selectionArray = 0;
	}

	m_selectionArray = new int[m_selectionCount];
    m_referenceLibDB.GetSelItems(m_selectionCount, m_selectionArray); 

	UpdateData(FALSE);
	
	CDialog::OnOK();
}

void CNodeImportDlg::InListReferenceLibrary(int sel)
{
	m_referenceLibDB.ResetContent();
    for(POSITION posLoc = m_referenceDBREF->GetHeadPosition();posLoc != NULL;)
	{
        CReferenceObj * refPtr = (CReferenceObj *)m_referenceDBREF->GetNext(posLoc);
        m_referenceLibDB.AddString(refPtr->m_referenceName);
	}
	if(sel > -1)
	   m_referenceLibDB.SetCurSel(sel);
}


BOOL CNodeImportDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InListReferenceLibrary(0);

	if(m_selectionCount != 0)
	{
	   for(int loop=0;loop<m_selectionCount;loop++)
	   {
           m_referenceLibDB.SetSel(m_selectionArray[loop],TRUE);
	   }
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNodeImportDlg::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}
