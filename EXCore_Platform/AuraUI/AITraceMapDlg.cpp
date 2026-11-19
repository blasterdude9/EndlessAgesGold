// AITraceMapDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "AITraceMapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAITraceMapDlg dialog


CAITraceMapDlg::CAITraceMapDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAITraceMapDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAITraceMapDlg)
	m_locationName = _T("");
	m_traceRaius = 0.0f;
	//}}AFX_DATA_INIT
}


void CAITraceMapDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAITraceMapDlg)
	DDX_Text(pDX, IDC_EDIT_TraceLocName, m_locationName);
	DDX_Text(pDX, IDC_EDIT_TraceRadius, m_traceRaius);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAITraceMapDlg, CDialog)
	//{{AFX_MSG_MAP(CAITraceMapDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAITraceMapDlg message handlers
