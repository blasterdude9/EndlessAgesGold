#pragma once
#include "afxcmn.h"


// CProgressDlg dialog

class CProgressDlg : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CProgressDlg)

public:
	CProgressDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CProgressDlg();
// Overrides
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// Dialog Data
	enum { IDD = IDD_DIALOG_PROGRESS, IDH = IDR_HTML_PROGRESSDLG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
public:
	CProgressCtrl m_progressControl;
	CString m_description;
};
