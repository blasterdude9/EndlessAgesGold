#pragma once

#include "wmp.h"
#include "ocx2.h"

// CAXmediaFrame dialog

class CAXmediaFrame : public CDialog
{
	DECLARE_DYNAMIC(CAXmediaFrame)

public:
	CAXmediaFrame(CWnd* pParent = NULL);   // standard constructor
	virtual ~CAXmediaFrame();

	CString url;

// Dialog Data
	enum { IDD = IDD_DIALOG_axMedia };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
//	virtual BOOL Create(LPCTSTR lpszTemplateName, CWnd* pParentWnd = NULL);
	COcx2 m_mPlayer;
	virtual BOOL OnInitDialog();

	BOOL Stop();
	BOOL SetVolume(int volume);
    BOOL PlayURL(char * url);
	CString WhatsPlaying();
};
