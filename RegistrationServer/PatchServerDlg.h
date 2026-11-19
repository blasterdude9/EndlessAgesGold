// PatchServerDlg.h : header file
//

#if !defined(AFX_PATCHSERVERDLG_H__C4800753_F609_4D49_8F2B_51B6E194FAF2__INCLUDED_)
#define AFX_PATCHSERVERDLG_H__C4800753_F609_4D49_8F2B_51B6E194FAF2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "dplay8.h"
#include "dplobby.h"
//#include "CFilePatchClass.h"
#include "CPacketQueClass.h"
#include "CServerList.h"
#include "CClientObject.h"
#include "CAccountSQLDatabase.h"

//helper structures
//LOGON context packet 
typedef struct {
	signed char m_catagorie;
	char  username[25];
	char  password[25];
	short versionInfo;
} CONTEXT_VALIDATION, *LPCONTEXT_VALIDATION;

typedef struct {
	signed char m_catagorie;
	signed char m_type;
	char  username[30];
	char  password[30];
	char  newUsername[30];
	char  newPassword[30];
} ADD_ACCOUNT, *LPADD_ACCOUNT;

typedef struct { // bytes alot 51
	signed char m_catagorie;
	char logInName[25];
	char logInPass[25]; 
} LOG_STRUCT, *LPLOG_STRUCT;

typedef struct {
	signed char m_catagorie;
	int         m_version;
} VERSION_INFO, *LPVERSION_INFO;

typedef struct {
	signed char m_catagorie;
} PATCH_HEADER, *LPPATCH_HEADER;

typedef struct {
	signed char m_catagorie;
	short       m_serverCount;

} SERVERLIST_HEADER, *LPSERVERLIST_HEADER;
//Cast 
typedef struct {
	signed char m_catagorie;
} EXMSG_CATAGORIE, *LPEXMSG_CATAGORIE;

typedef struct {
	signed char m_catagorie;
	int         m_statusType;
	int         m_miscInt;
	long        m_patchSize;
} EXMSG_STATUS, *LPEXMSG_STATUS;

typedef struct {
	signed char m_catagorie;
	int         m_version;
	long        m_currentPosition;
} EXMSG_PATCHSECTIONREQ, *LPEXMSG_PATCHSECTIONREQ;

typedef struct {
	signed char m_catagorie;
} EXMSG_PATCHSECTION, *LPEXMSG_PATCHSECTION;

typedef struct {
	signed char m_catagorie;
	signed char m_patchType;//describes alot abount compression  creation
	long        m_fileTargetLength;
	long        m_expandedPatchSize;
    long        m_newFileSize;
    long        m_patchDataSize;
    long        m_progressionCount;
	int         m_versionNumber;
	long        m_targetFileSize;
} EXMSG_PATCHHEADER, *LPEXMSG_PATCHHEADER;


/////////////////////////////////////////////////////////////////////////////
// CPatchServerDlg dialog

class CPatchServerDlg : public CDialog
{
// Construction
public:
	CPatchServerDlg(CWnd* pParent = NULL);	// standard constructor
 

	CString m_sqlIp;
	CString m_sqlUserid;
	CString m_sqlPass;
	CString m_sqlDB;
	static IDirectPlay8Client *   m_clientInterface;
	static IDirectPlay8Server *   m_serverInterface;
    DWORD                         m_port;
	static CPacketObjList        *m_packetQue;
	BOOL                          m_passwordRequired;
	static CString                m_serverKey;
	static DPNHANDLE              m_dpAsncEnumHostHandle;
	static long                   m_lastCommunicationTime;
	int                           m_maxPlayers;
	static CString                m_userName;
	static CString                m_passWord;
	static CString                m_IPaddress;
	BOOL                          m_serverStarted;
	long                          m_serverStartTime;
	static CRITICAL_SECTION       m_recieveCriticalSection;
	static BOOL                   m_primaryPlayerCreated;
	static BOOL                   m_enabledClient;
	static int                    m_currentVersion;
	//CFilePatchObjectList         *m_filePatchDB;
	BOOL                          m_clientSentVersionInfo;
	BOOL                          m_inProcess;
	CServerListObjectDB          *m_targetServersList;
	BOOL                          m_connectionRequested;
	static CClientObjList        *m_CurrentClientDB;

    long                          m_timeStamp;
	static int                    m_usersCount;
	long                          m_cycleTimeStamp;

	static BOOL                   m_updateServersAccountsInProgress;  
	static int                    m_updateServersCurrentIndex;
    static BOOL                   m_updateServerCurrentlyConnected;  
	static BOOL                   m_updateServerAttemptingConnection;  
	static long                   m_updateServerTimeoutOnConnection; 
	static long                   m_updateServerTimeoutStamp; 
	POSITION                      m_updateServerPOSITION;
    BOOL                          m_updateServerUpdateingPerfect; 
	BOOL                          m_updateServerSentData; 
	
    //Client side proprietary
    BOOL                          m_currentlyDldingPatch;
	long                          m_currentPatchSize;
	long                          m_currentPatchDldPosition;
	int                           m_currentDldVersion;
	//CFilePatchObject             *m_currentFilePatchObject;
	long                          m_patchMaxBlockSz;

	CRITICAL_SECTION              m_criticalSection;


	CAccountSQLDatabase          *m_accountsToBeEntered;
	CAccountSQLDatabase          *m_expChangesToBeEntered;
	CAccountSQLDatabase          *m_accountsInfoChanges;
	CAccountSQLDatabase          *m_accountsToPlaceOnHold;
	CAccountSQLDatabase          *m_accountsToTakeOffHold;
	CAccountSQLDatabase          *m_accountadditionHistory;
	CAccountSQLDatabase          *m_accountsToBeDeleted;



// Dialog Data
	//{{AFX_DATA(CPatchServerDlg)
	enum { IDD = IDD_PATCHSERVER_DIALOG };
	CListBox	m_history;
	CButton	m_enableSystem;
	CListBox	m_targetServerDB;
	CProgressCtrl	m_dldProgressMeter;
	CString	m_serverStatus;
	CString	m_ipadress;
	CString	m_servername;
	long	m_cycleDuration;
	CString	m_registrationURL;
	CString	m_getSignupsURL;
	int		m_packetSendSpeedMS;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPatchServerDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;




    BOOL   EnumHosts(CString TCPIP);
	BOOL   StopEnumerating();
	static HRESULT WINAPI DirectPlayMessageHandler( PVOID pvUserContext, 
                                                    DWORD dwMessageId, 
                                                    PVOID pMsgBuffer );

	static CString WINAPI ConvertWideStringToCString(const WCHAR* wstrSource);
	static CString WINAPI GetIPAddress(IDirectPlay8Address * pAddress);
	static CString WINAPI GetIPAddress(DPNID id);
	static void WINAPI ConvertWideStringToAnsi( CHAR* strDestination, const WCHAR* wstrSource, int cchDestChar );
    void   SendData(BYTE * data,DWORD size,DWORD flags,DPNID sendTo);
	inline BOOL GetNextPacket(int * size,BYTE ** data,DPNID * id);
	void   HandleMessagesClientSide(LPVOID lpMessage,DPNID idFromLoc,int msgSize);
	void   HandleMessagesServerSide(LPVOID lpMessage,DPNID idFromLoc,int msgSize);
	CString ConvToString(int number);
	void   SendVersion(int version);
	void UpdateVersion();
	void SendServerList(DPNID sendTo);
	void HandleServerListClientSide(LPVOID lpMessage,DPNID idFromLoc,int msgSize);
	void InListAvailableServers(int sel);
	BOOL SendStatus(DPNID sendTo,
				    int miscInt,
				    int statusType,
					long patchSize);
	BOOL SendPatchSectionReq(DPNID sendTo,int version,long currentPosition);
	void SendAccountData(CString username,
					     CString password);
	void PlaceAccountOnHold(CString username,
						    CString password);
	void TakeAccountOffHold(CString username,
							CString password);
	void ExpansionEnable(CString username,
						 CString expLevel);
	void GetNewAccountsFromWeb(CString url,CString registerUrl);
	void RegisterAccountsFromWeb(CString url);
	BOOL ConnectToServer(CString ip);
	void InList(int sel);
	void SendCompletedDataTail();
	void Cycle();
	void SaveHistoryDB(CString fileName);
	void InListHistory(int sel);
	BOOL SendLogFast(CString userName,CString password);
	void ChangeAccountInfo(CString username,
						   CString password,
						   CString newUsername,
						   CString newPassword);
    BOOL GetDataFromFile(CString fileName);
	BOOL ArchiveDataFile(CString fileName);
	CString GetFirstFileNameInDirectory(CString dir);
	BOOL GetDataFromSQL();
	BOOL RefreshAllAccountStatus();
	void DeleteAccountData(CString username,
								    CString password);
	void WriteDebugFile(CString logMsg,BOOL clearIt);
	long GetVersionFromFile();//ussually client side

	// Generated message map functions
	//{{AFX_MSG(CPatchServerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();
	virtual void OnOK();
	afx_msg void OnBUTTONtestcode();
	afx_msg void OnBUTTONAdd();
	afx_msg void OnBUTTONDelete();
	afx_msg void OnBUTTONLoad();
	afx_msg void OnBUTTONSave();
	afx_msg void OnBUTTONsaveHistory();
	afx_msg void OnBUTTONloadHistory();
	afx_msg void OnBUTTONRebuildAccounts();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PATCHSERVERDLG_H__C4800753_F609_4D49_8F2B_51B6E194FAF2__INCLUDED_)
