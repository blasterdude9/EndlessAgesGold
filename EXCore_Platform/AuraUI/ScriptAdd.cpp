// ScriptAdd.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "ScriptAdd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CScriptAdd dialog


CScriptAdd::CScriptAdd(CWnd* pParent /*=NULL*/)
	: CDialog(CScriptAdd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CScriptAdd)
	m_scriptDescription = _T("");
	m_scriptName = _T("");
	//}}AFX_DATA_INIT
}


void CScriptAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CScriptAdd)
	DDX_Text(pDX, IDC_EDIT_ScriptDescription, m_scriptDescription);
	DDX_Text(pDX, IDC_EDIT_ScriptName, m_scriptName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CScriptAdd, CDialog)
	//{{AFX_MSG_MAP(CScriptAdd)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CScriptAdd message handlers
