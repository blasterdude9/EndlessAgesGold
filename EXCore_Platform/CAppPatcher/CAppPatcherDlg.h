// CAppPatcherDlg.h : header file
//

#pragma once
#include "UIImageExt.h"
#include "HttpFileDownloader.h"
#include "getdxver.h"
#include "loggingclass.h"
#include "afxwin.h"
#include "UIButtonExt.h"
#include "httpfiledownloader.h"
#include "Cabinet.hpp"
#include "afxcmn.h"
#include "crcgenerator.h"
#include "AXEncryptionLib.h"
#include "explorerWrapper.h"

#define INFO_BUFFER_SIZE MAX_PATH+4

struct ServerList
{
    CString serverName;
	CString serverIP;
	CString gameName;
};



// CCAppPatcherDlg dialog
class CCAppPatcherDlg : public CDialog
{
// Construction
public:
	CCAppPatcherDlg(CWnd* pParent = NULL);	// standard constructor

	CUIImageExt						m_imageAx;
	CDirectoryRegistryWrapper		m_abTool;
	CString							m_installPath;
	int								m_devMode;
	BOOL                            m_mustInstallDirectX;
	CLoggingClass					m_axLogger;
	CLoggingClass					m_uninstallData;
	CString                         m_urlOfCurrentDownload;
	CString                         m_cabNameCurrentDld;
	int                             m_mode;
	BOOL                            m_modeEngaged;
	int                             m_currentResource;
	CUIButtonExtList                m_buttonList;
	CString                         m_decoratedGameName;
	int                             m_timeWaitCount;

	CRITICAL_SECTION                m_section;
	int                             m_currentProgress;
	DWORD							m_dlThreadID;
	HANDLE                          m_dlHandle;
	CString                         m_contentServerUrl;
	CString                         m_contentServerIP;
	CString                         m_gameName;
	CCRCgenerator                   m_crcGenerator;
	CString                         m_clientCAB;
	CString                         m_clientEXE;
	CString                         m_loadPointDirectory;//used as basis if first time launch
	ServerList                      m_serverListBuf[256];
	int                             m_serverCount;
	int								m_clientDownloadAttempts;
	

	//HBITMAP LoadAnImage(char* FileName, int resHandle);
	void FunctionProcessor(int function);
	BOOL ResolutionPanelVisibility(BOOL visible);
	bool ParseUrlGetLine(int & curPos, CString & urlData, CString & lineString);
	bool ParseUrlStringToData(int set, 
											  CString urlParse, 
											  CString & fileName, 
											  long * fileSize, 
											  CString & cabName, 
											  CString & offsetDir, 
											  unsigned long * CRC,
											  unsigned long * cab_CRC,
											  BOOL * fileSizeVerificationRequired);
	void CreateInstallRegistries(CString installPath);
	void DownloadCabinetAndExtract(CString url,CString cabName);
	CBitmap m_installerBackgroundImage;
	bool ValidateFile(CString & fullPath, unsigned long fileCRC, BOOL verifyCRC);
	BOOL ExecApplication(CString appExe);
// Dialog Data
	enum { IDD = IDD_CAPPPATCHER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support



// Implementation
protected:
	HICON m_hIcon;
	BOOL CreateShortcuts(CString applicationWPath, CString shortcutName,CString installPath);
	HRESULT CreateLink(CString lpszPathObj, CString lpszPathLink,CString workingDirectoy,CString lpszDesc);
    CString GetSystemDrive();

	BOOL GetInfoFromCoinitFile( CString & username,
								CString & password,
								CString & gameName,
								CString & contentServerIP,
								CString & gameServerIP,
								BOOL & fullScreen,
								BOOL & saveLogonData,
								int & width,
								int & height,
								int & depth);
	BOOL DetectDirectXVersionReq(DWORD minVersion,CString errorMsg);
	CString ParseLine(ULONGLONG & pos,char * Buf, ULONGLONG length, int sqCount);
	CAXEncryptionLib			*   m_xEncrypt;
	HRESULT CreateUnistallLinks(CString decoratedName, CString installPath, CString gameName);
	BOOL WriteCoinitFile( CString username,
						  CString password,
						  CString game,
						  CString downloadUrl,
						  CString gameserverIP,
						  BOOL fullscreen,
						  BOOL saveLogonData,
						  CString wResolution,
						  CString hResolution,
						  CString depth);
	void ChangeBackground(int resourceID);

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
public:
	afx_msg void OnDestroy();
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	CString m_currentDirectory;
public:
	afx_msg void OnBnClickedButtonNext();
public:
	CEdit m_currentDirEdit;
public:
	CButton m_nextButtonControl;
public:
	CStatic m_imageBtn;
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
public:
	CProgressCtrl m_progressMeter;
public:
	CEdit m_username;
public:
	CEdit m_password;
public:
	CComboBox m_resolutions;
public:
	CButton m_fullscreen;
public:
	CButton m_saveLogonData;
public:
	CComboBox m_bitDepth;
public:
	CComboBox m_serversList;
	CExplorer1 m_explorerCtrl;
};
