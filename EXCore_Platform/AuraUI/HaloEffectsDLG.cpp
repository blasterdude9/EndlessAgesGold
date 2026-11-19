// HaloEffectsDLG.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "HaloEffectsDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// HaloEffectsDLG dialog


HaloEffectsDLG::HaloEffectsDLG(CWnd* pParent /*=NULL*/)
	: CDialog(HaloEffectsDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(HaloEffectsDLG)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void HaloEffectsDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(HaloEffectsDLG)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(HaloEffectsDLG, CDialog)
	//{{AFX_MSG_MAP(HaloEffectsDLG)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// HaloEffectsDLG message handlers

