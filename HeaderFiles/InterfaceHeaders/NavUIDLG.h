
#include "CMalInterface.h"
#pragma once


// CNavUIDLG dialog

class CNavUIDLG : public CDialog
{
	DECLARE_DYNAMIC(CNavUIDLG)
public:

	CMalInterface	* interfaceRef;
	CMenu			* m_menuHandle;
	HMENU			  m_hostMenu;   // set when the Nav menu is merged into a frame's menu bar

	BOOL SetItemFont();

public:
	CNavUIDLG(CWnd* pParent = NULL);   // standard constructor
	virtual ~CNavUIDLG();

// Dialog Data
	enum { IDD = IDD_DIALOG_NAVUI };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGamplaymodulesKingdomsmanager();
	virtual BOOL OnInitDialog();
	virtual void OnSetFont(CFont* pFont);
	afx_msg void OnDisplayPosteffects();
	afx_msg void OnDisplayCaptureZoneMap();
	afx_msg void OnDisplayCaptureAllZoneMaps();
	afx_msg void OnDisplayLockview();
	afx_msg void OnUpdateDisplayLockview(CCmdUI *pCmdUI);
	afx_msg void OnInitializationLoadscriptmanager();
	afx_msg void OnSoundMusicmanager();
	afx_msg void OnInitializationScenescriptmanager();
	afx_msg void OnEnvironmentActionzones();
};
