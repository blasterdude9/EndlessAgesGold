// AXmediaFrame.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "AXmediaFrame.h"
#include ".\axmediaframe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CAXmediaFrame dialog

IMPLEMENT_DYNAMIC(CAXmediaFrame, CDialog)
CAXmediaFrame::CAXmediaFrame(CWnd* pParent /*=NULL*/)
	: CDialog(CAXmediaFrame::IDD, pParent)
{
}

CAXmediaFrame::~CAXmediaFrame()
{
}

void CAXmediaFrame::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_OCX2, m_mPlayer);
}


BEGIN_MESSAGE_MAP(CAXmediaFrame, CDialog)
END_MESSAGE_MAP()


// CAXmediaFrame message handlers

//BOOL CAXmediaFrame::Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd)
//{
//	
//
//	return CDialog::Create(lpszTemplateName, pParentWnd);
//}

BOOL CAXmediaFrame::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_mPlayer.put_uiMode("invisible");

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CAXmediaFrame::Stop()
{
	IWMPControls * controls = (IWMPControls *)m_mPlayer.get_controls();
	controls->stop();
	return TRUE;
}

BOOL CAXmediaFrame::PlayURL(char * url)
{
	Stop();
	LPDISPATCH dispatch = m_mPlayer.newMedia(url);
	m_mPlayer.put_currentMedia(dispatch);
	IWMPControls * controls = (IWMPControls *)m_mPlayer.get_controls();
	controls->play();
	return TRUE;
}

BOOL CAXmediaFrame::SetVolume(int volume)
{
	
	//IWMPControls * controls = (IWMPControls *)m_mPlayer.get_controls();
	IWMPSettings * settings = (IWMPSettings *)m_mPlayer.get_settings();
	HRESULT errorRes = settings->put_volume(volume);
	//m_mPlayer.put_volume(0);
	//controls->play();
	return TRUE;
}

CString CAXmediaFrame::WhatsPlaying()
{
    IWMPMedia * media = (IWMPMedia *)m_mPlayer.get_currentMedia();
	BSTR str;
	media->get_name(&str);

	BSTR urlStr;
	media->get_sourceURL(&urlStr);

	CString final = (LPSTR)str;
    return "";
}