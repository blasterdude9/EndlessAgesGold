// CursorLimits.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "CursorLimits.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCursorLimits dialog


CCursorLimits::CCursorLimits(CWnd* pParent /*=NULL*/)
	: CDialog(CCursorLimits::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCursorLimits)
	m_xLimit = 0.0f;
	m_yLimit = 0.0f;
	//}}AFX_DATA_INIT
}


void CCursorLimits::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCursorLimits)
	DDX_Text(pDX, IDC_EDIT_XLimit, m_xLimit);
	DDX_Text(pDX, IDC_EDIT_YLimit, m_yLimit);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCursorLimits, CDialog)
	//{{AFX_MSG_MAP(CCursorLimits)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCursorLimits message handlers
