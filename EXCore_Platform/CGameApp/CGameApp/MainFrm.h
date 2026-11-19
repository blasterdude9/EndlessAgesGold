//MainFrm.h : interface of the CMainFrame class
//


#pragma once

#include "HttpFileDownloader.h"
#include "Cabinet.hpp"
#include "clientmodule.h"
#include "AXContentClient.h"
#include "ocx2.h"
#include "wmp.h"
#include "loggingclass.h"
#include "crcgenerator.h"

//#include "ChildView.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame();
protected: 
	DECLARE_DYNAMIC(CMainFrame)

//Attributes
public:

	CClientModule * m_engineClient;
	BOOL            m_upgradeThreadWorking;
	//CAXmediaFrame * m_axmedia;
	CAXContentClient * m_contentDownloadManager;
	DWORD           m_globalRenderLoopThreadID;
	HANDLE          m_globalRenderLoopHandle;
	BOOL            m_running;
	HWND            windowHandle;
	BOOL            m_rSp;
	BOOL            m_test;
	BOOL            m_encryptedMode;
	//externally set
	CString         m_username;
	CString         m_password;
	CString         m_game;
	CString         m_gserverIP;

	BOOL            m_closeDown;
	CLoggingClass   m_axLogger;

	BOOL            m_disableMouseCmd;

	BOOL            m_startInFullscreen;
	int             m_width;
	int             m_height;
	int             m_depth;
	int             m_maxTextureSize;
	int             m_shadowCount;
	CDirectoryRegistryWrapper abTool;

	//windows media player
	COcx2           * m_mPlayer;
	BOOL            m_PlayerEnabled;
	VARIANT_BOOL	m_pendingUrlShouldLoop;
	CString         m_pendingUrl;
	int             m_pendingVolume;

	BOOL            m_devMode;
	CCRCgenerator   m_crcGenerator;
	
	//BOOL            m_showMouse;

	CRITICAL_SECTION m_updateCriticalSection;
	int m_percentagePub;
	DWORD m_bytesDownloaded;
	BOOL m_closeNow;
	CString m_statusPub;
	HANDLE m_lgThreadHandle;
	BOOL m_ready;

	DWORD m_rnThreadID;
	HANDLE m_rnThreadHandle;

    //directory maintnence 
	CString m_installPath;

	//dowload specific globals
	CString m_downloadURL;
	BOOL    m_contentUpdatesInClient;  // FALSE: launcher keeps the install current, no in-client downloads
	CString m_saveLocationWFile;
	BOOL m_downloading;
	BOOL m_openProgress;
	//CProgressDlg * m_prgDlg;
	RECT m_windowSz;
	BOOL m_toggleTest;
	CString m_cacheDirectory;
	BOOL m_renderSpinner;
	CAXEncryptionLib * m_xEncrypt;

	BOOL tested;
//Operations
public:

	BOOL DetectDirectXVersionReq(DWORD minVersion,CString errorMsg);
	int VersionToNumber(CString stringVer);
	BOOL Stop();
    BOOL _PlayURL(char * url);
	BOOL PlayURL(char *url, VARIANT_BOOL loop = VARIANT_FALSE);
    BOOL SetVolume(int volume);
	bool ReadUrl(CString & url, CString & retString);
	bool ParseUrlStringToData(int set, 
							  CString urlParse, 
							  CString & fileName, 
							  long * fileSize, 
							  CString & cabName, 
							  CString & offsetDir, 
							  unsigned long * CRC,
							  unsigned long * cab_CRC,
							  BOOL * checkCrc);
	bool ParseUrlGetLine(int & curPos, CString & urlData, CString & lineString);
	bool ValidateFile(CString & fullPath, unsigned long fileCRC, BOOL checkCrc, unsigned long * dynGenCrc);
	CString ParseLine(ULONGLONG & pos,char * Buf, ULONGLONG length, int sqCount);
	BOOL GetInfoFromCoinitFile( CString & username,
								CString & password,
								CString & gameName,
								CString & contentServerIP,
								CString & gameServerIP,
								BOOL & fullScreen,
								BOOL & saveLogonData,
								int & width,
								int & height,
								int & depth
								);
	BOOL WriteCoinitFile(CString username,
									  CString password,
									  CString game,
									  CString downloadUrl,
									  CString gameserverIP,
									  BOOL fullscreen,
									  BOOL saveLogonData,
									  CString wResolution,
									  CString hResolution,
									  CString depth);
//Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

//Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	//CChildView    m_wndView;

//Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnDestroy();
};


