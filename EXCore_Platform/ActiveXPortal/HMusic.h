#pragma once


// CHMusic dialog

class CHMusic : public CDHtmlDialog
{
	DECLARE_DYNCREATE(CHMusic)

public:
	CHMusic(CWnd* pParent = NULL);   // standard constructor
	virtual ~CHMusic();
// Overrides

	virtual void OnFinalRelease();
	HRESULT OnButtonOK(IHTMLElement *pElement);
	HRESULT OnButtonCancel(IHTMLElement *pElement);

// Dialog Data
	enum { IDD = IDD_DIALOG_musicAxc, IDH = IDR_HTML_HMUSIC };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_DHTML_EVENT_MAP()
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};
