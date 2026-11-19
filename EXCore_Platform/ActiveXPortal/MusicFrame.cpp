// MusicFrame.cpp : implementation file
//

#include "stdafx.h"
#include "ActiveXPortal.h"
#include "MusicFrame.h"


// CMusicFrame dialog

IMPLEMENT_DYNAMIC(CMusicFrame, CDialog)
CMusicFrame::CMusicFrame(CWnd* pParent /*=NULL*/)
	: CDialog(CMusicFrame::IDD, pParent)
{
}

CMusicFrame::~CMusicFrame()
{
}

void CMusicFrame::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMusicFrame, CDialog)
END_MESSAGE_MAP()


// CMusicFrame message handlers
