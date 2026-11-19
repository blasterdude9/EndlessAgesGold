// ScriptEventAdd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ScriptEventAdd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScriptEventAdd dialog


CScriptEventAdd::CScriptEventAdd(CWnd* pParent /*=NULL*/)
	: CDialog(CScriptEventAdd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScriptEventAdd)
	m_miscInt = 0;
	m_changeableText = _T("");
	//}}AFX_DATA_INIT
}


void CScriptEventAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScriptEventAdd)
	DDX_Control(pDX, IDC_COMBO_EVENT, m_event);
	DDX_Text(pDX, IDC_EDIT_MISCINT, m_miscInt);
	DDX_Text(pDX, IDC_STATIC_ChangeableText, m_changeableText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScriptEventAdd, CDialog)
	//{{AFX_MSG_MAP(CScriptEventAdd)
	ON_CBN_SELCHANGE(IDC_COMBO_EVENT, OnSelchangeComboEvent)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScriptEventAdd message handlers

BOOL CScriptEventAdd::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(TRUE);
	m_event.SetCurSel(m_finalEventInt);

	UpdateData(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CScriptEventAdd::OnSelchangeComboEvent() 
{	
	int index = m_event.GetCurSel();
	if(index == -1){
       return;
	}
	UpdateData(TRUE);
    if(index == 0)
       m_changeableText = "Location index";
	if(index == 1)
       m_changeableText = "Milliseconds";
	if(index == 2)
       m_changeableText = "Version";
	if(index == 3)
       m_changeableText = "Not Used";
	if(index == 4)
       m_changeableText = "Not Used";

	m_finalEventInt = index;
	UpdateData(FALSE);
}
