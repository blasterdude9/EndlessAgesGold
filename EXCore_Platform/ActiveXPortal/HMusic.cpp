// HMusic.cpp : implementation file
//

#include "stdafx.h"
#include "ActiveXPortal.h"
#include "HMusic.h"


// CHMusic dialog

IMPLEMENT_DYNCREATE(CHMusic, CDHtmlDialog)

CHMusic::CHMusic(CWnd* pParent /*=NULL*/)
	: CDHtmlDialog(CHMusic::IDD, CHMusic::IDH, pParent)
{
	EnableActiveAccessibility();
	EnableAutomation();

}

CHMusic::~CHMusic()
{
}

void CHMusic::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDHtmlDialog::OnFinalRelease();
}

void CHMusic::DoDataExchange(CDataExchange* pDX)
{
	CDHtmlDialog::DoDataExchange(pDX);
}

BOOL CHMusic::OnInitDialog()
{
	CDHtmlDialog::OnInitDialog();
	return TRUE;  // return TRUE  unless you set the focus to a control
}

BEGIN_MESSAGE_MAP(CHMusic, CDHtmlDialog)
END_MESSAGE_MAP()

BEGIN_DHTML_EVENT_MAP(CHMusic)
	DHTML_EVENT_ONCLICK(_T("ButtonOK"), OnButtonOK)
	DHTML_EVENT_ONCLICK(_T("ButtonCancel"), OnButtonCancel)
END_DHTML_EVENT_MAP()


BEGIN_DISPATCH_MAP(CHMusic, CDHtmlDialog)
END_DISPATCH_MAP()

// Note: we add support for IID_IHMusic to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {FAFD559E-EECF-45C3-A230-0B2FE81B6246}
static const IID IID_IHMusic =
{ 0xFAFD559E, 0xEECF, 0x45C3, { 0xA2, 0x30, 0xB, 0x2F, 0xE8, 0x1B, 0x62, 0x46 } };

BEGIN_INTERFACE_MAP(CHMusic, CDHtmlDialog)
	INTERFACE_PART(CHMusic, IID_IHMusic, Dispatch)
END_INTERFACE_MAP()


// CHMusic message handlers

HRESULT CHMusic::OnButtonOK(IHTMLElement* /*pElement*/)
{
	OnOK();
	return S_OK;  // return TRUE  unless you set the focus to a control
}

HRESULT CHMusic::OnButtonCancel(IHTMLElement* /*pElement*/)
{
	OnCancel();
	return S_OK;  // return TRUE  unless you set the focus to a control
}
