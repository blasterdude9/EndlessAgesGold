#pragma once

//ActiveXPortalCtrl.h : Declaration of the CActiveXPortalCtrl ActiveX Control class.


//CActiveXPortalCtrl : See ActiveXPortalCtrl.cpp for implementation.
#include "HttpFileDownloader.h"
#include "Cabinet.hpp"
#include "clientmodule.h"
#include "AXContentClient.h"
#include "ocx2.h"
#include "wmp.h"
#include "loggingclass.h"
#include "crcgenerator.h"

class CActiveXPortalCtrl : public COleControl
{
	DECLARE_DYNCREATE(CActiveXPortalCtrl)

//Constructor
public:
	CActiveXPortalCtrl();
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

//Overrides
public:
	//void SetUPG(LPSTR username, LPSTR password, LPSTR game, LPSTR downloadUrl, LPSTR gserver);
	void SetUPG(LPSTR username, 
								LPSTR password, 
								LPSTR game, 
								LPSTR downloadUrl, 
								LPSTR gserver,
								int fullScreen,
								int fullscreen_W,
								int fullscreen_H,
								int fullscreen_D);
    bool GetDataSetCount(int set);
	bool ParseUrlGetLine(int & curPos, CString & urlData, CString & lineString);
	bool ValidateFile(CString & fullPath, unsigned long fileCRC, BOOL verifyFileSizeRequired);
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

	virtual void OnDraw(CDC* pdc, const CRect& rcBounds, const CRect& rcInvalid);
	virtual void DoPropExchange(CPropExchange* pPX);
	virtual void OnResetState();
    //WMWrapper
	BOOL Stop();
    BOOL PlayURL(char * url);
    BOOL SetVolume(int volume);
    CString WhatsPlaying();

//Implementation
protected:
	~CActiveXPortalCtrl();

	DECLARE_OLECREATE_EX(CActiveXPortalCtrl)    //Class factory and guid
	DECLARE_OLETYPELIB(CActiveXPortalCtrl)      //GetTypeInfo
	DECLARE_PROPPAGEIDS(CActiveXPortalCtrl)     //Property page IDs
	DECLARE_OLECTLTYPE(CActiveXPortalCtrl)		//Type name and misc status

//Message maps
	DECLARE_MESSAGE_MAP()

//Dispatch maps
	DECLARE_DISPATCH_MAP()

	afx_msg void AboutBox();

//Event maps
	DECLARE_EVENT_MAP()

//Dispatch and event IDs
public:
	enum {
	};
	HWND GetTopmostParent();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnKeyPressEvent(USHORT nChar);
	CString ConvToString(int number);
	afx_msg void OnDestroy();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	int CActiveXPortalCtrl::VersionToNumber(CString stringVer);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);


	BOOL DetectDirectXVersionReq(DWORD minVersion,CString errorMsg);
};

