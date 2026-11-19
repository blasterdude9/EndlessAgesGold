// AddHud.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "AddHud.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
 static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddHud dialog


CAddHud::CAddHud(CWnd* pParent /*=NULL*/)
	: CDialog(CAddHud::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddHud)
	m_name = _T("");
	//}}AFX_DATA_INIT
}


void CAddHud::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddHud)
	DDX_Text(pDX, IDC_EDIT_NAME, m_name);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddHud, CDialog)
	//{{AFX_MSG_MAP(CAddHud)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddHud message handlers
