// ProgressDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ActiveXPortal.h"
#include "ProgressDlg.h"


// CProgressDlg dialog

 IMPLEMENT_DYNCREATE(CProgressDlg, CDHtmlDialog)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CProgressDlg::IDD, CProgressDlg::IDH, pParent)
	, m_description(_T(""))
{
}

CProgressDlg::~CProgressDlg()
{
}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_CNT, m_progressControl);
	DDX_Text(pDX, IDC_INFO_TEXT, m_description);
}

BOOL CProgressDlg::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CProgressDlg, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CProgressDlg)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()



// CProgressDlg message handlers

HRESULT CProgressDlg::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;  // return TRUE  unless you set the focus to a control
}

HRESULT CProgressDlg::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;  // return TRUE  unless you set the focus to a control
}
