// IIAMapView.cpp : implementation file
//

#include "stdafx.h"
#include "PatchServer.h"
#include "IIAMapView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIIAMapView dialog


CIIAMapView::CIIAMapView(CWnd* pParent /*=NULL*/)
	: CDialog(CIIAMapView::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIIAMapView)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CIIAMapView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIIAMapView)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIIAMapView, CDialog)
	//{{AFX_MSG_MAP(CIIAMapView)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIIAMapView message handlers
