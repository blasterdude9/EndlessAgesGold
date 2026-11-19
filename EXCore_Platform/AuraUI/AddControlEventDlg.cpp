// AddControlEventDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include <d3d9.h>
#include "AddControlEventDlg.h"

#include <afxtempl.h>
#include <math.h>
#include <dinput.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddControlEventDlg dialog


CAddControlEventDlg::CAddControlEventDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddControlEventDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddControlEventDlg)
	m_comboActions = _T("");
	m_comboKeyButton = _T("");
	m_miscFloat1 = 0.0f;
	m_miscFloat3 = 0.0f;
	m_miscFloat2 = 0.0f;
	//}}AFX_DATA_INIT
}


void CAddControlEventDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddControlEventDlg)
	DDX_CBString(pDX, IDC_COMBO_Actions, m_comboActions);
	DDX_CBString(pDX, IDC_COMBO_DefaultKeyButton, m_comboKeyButton);
	DDX_Text(pDX, IDC_EDIT_MiscFloat1, m_miscFloat1);
	DDX_Text(pDX, IDC_EDIT_MiscFloat3, m_miscFloat3);
	DDX_Text(pDX, IDC_EDIT_miscFloat2, m_miscFloat2);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_COMBO_Actions, m_actionDropList);
	DDX_Control(pDX, IDC_COMBO_DefaultKeyButton, m_keyList);
}


BEGIN_MESSAGE_MAP(CAddControlEventDlg, CDialog)
	//{{AFX_MSG_MAP(CAddControlEventDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddControlEventDlg message handlers

BOOL CAddControlEventDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	int tracer = 0;
	while(1)
	{
	  CString action = m_controlListRef->ReturnActionByIndex(tracer);
	  if(action == "")
	     break;
	  m_actionDropList.AddString(action);
	  tracer++;
	}

	tracer = 0;
	while(1)
	{
	  CString desc = m_controlListRef->ReturnKeyStringByIndex(tracer);
	  if(desc == "")
	     break;
	  m_keyList.AddString(desc);
	  tracer++;
	}

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddControlEventDlg::OnOK() 
{
UpdateData(TRUE);

finalKeyNum = m_controlListRef->GetKeyNum(m_comboKeyButton);
finalActionNum = m_controlListRef->GetActionNum(m_comboActions);
	
if(finalActionNum == -1){
MessageBox("Must select a two valid fields");
return;
}

if(finalKeyNum != -1){
eventBasis = 0;
CDialog::OnOK();
return;
}

finalKeyNum = m_controlListRef->GetJSNum(m_comboKeyButton);
if(finalKeyNum != -1){
eventBasis = 2;
CDialog::OnOK();
return;
}

finalKeyNum = m_controlListRef->GetMouseKey(m_comboKeyButton);
if(finalKeyNum != -1){
eventBasis = 1;
CDialog::OnOK();
return;
}

UpdateData(FALSE);	
if(finalKeyNum == -1){
MessageBox("Must select a two valid fields");
return;
}
}

