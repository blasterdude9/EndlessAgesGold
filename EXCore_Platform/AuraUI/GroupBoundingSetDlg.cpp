// GroupBoundingSetDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "GroupBoundingSetDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGroupBoundingSetDlg dialog


CGroupBoundingSetDlg::CGroupBoundingSetDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGroupBoundingSetDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CGroupBoundingSetDlg)
	m_group = 0;
	m_XnegBump = 0.0f;
	m_XposBump = 0.0f;
	m_YnegBump = 0.0f;
	m_YposBump = 0.0f;
	m_ZnegBump = 0.0f;
	m_ZposBump = 0.0f;
	//}}AFX_DATA_INIT
}


void CGroupBoundingSetDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGroupBoundingSetDlg)
	DDX_Text(pDX, IDC_EDIT_group, m_group);
	DDX_Text(pDX, IDC_EDIT_XnegBump, m_XnegBump);
	DDX_Text(pDX, IDC_EDIT_XposBump, m_XposBump);
	DDX_Text(pDX, IDC_EDIT_YnegBump, m_YnegBump);
	DDX_Text(pDX, IDC_EDIT_YposBump, m_YposBump);
	DDX_Text(pDX, IDC_EDIT_ZnegBump, m_ZnegBump);
	DDX_Text(pDX, IDC_EDIT_ZposBump, m_ZposBump);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGroupBoundingSetDlg, CDialog)
	//{{AFX_MSG_MAP(CGroupBoundingSetDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGroupBoundingSetDlg message handlers
