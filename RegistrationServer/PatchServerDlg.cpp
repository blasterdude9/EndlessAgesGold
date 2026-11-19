// PatchServerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PatchServer.h"
#include <afxinet.h>
#include "PatchServerDlg.h"
//#include "ServerSettingsDlg.h"
#include "math.h"
#include <process.h>
#include "direct.h"
#include "STDIO.H"
#include <fstream>
#include <iostream>
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#import "msado15.dll" \
         no_namespace rename("EOF", "EndOfFile")//server only pls


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

GUID g_guidApp = { 0xede9493e, 0x6ac8, 0x4f15, { 0x8d, 0x1, 0x8b, 0x16, 0x32, 0x0, 0xb9, 0x66 } };


//static declarations
IDirectPlay8Client *   CPatchServerDlg::m_clientInterface;
IDirectPlay8Server *   CPatchServerDlg::m_serverInterface;
CPacketObjList     *   CPatchServerDlg::m_packetQue;
CString                CPatchServerDlg::m_serverKey;
DPNHANDLE              CPatchServerDlg::m_dpAsncEnumHostHandle;
long                   CPatchServerDlg::m_lastCommunicationTime;
CString                CPatchServerDlg::m_userName;
CString                CPatchServerDlg::m_passWord;
CString                CPatchServerDlg::m_IPaddress;
CRITICAL_SECTION       CPatchServerDlg::m_recieveCriticalSection;
BOOL                   CPatchServerDlg::m_primaryPlayerCreated;
BOOL                   CPatchServerDlg::m_enabledClient;
int                    CPatchServerDlg::m_currentVersion;
int                    CPatchServerDlg::m_usersCount;
CClientObjList     *   CPatchServerDlg::m_CurrentClientDB;
BOOL                   CPatchServerDlg::m_updateServersAccountsInProgress;  
int                    CPatchServerDlg::m_updateServersCurrentIndex;
BOOL                   CPatchServerDlg::m_updateServerCurrentlyConnected;  
BOOL                   CPatchServerDlg::m_updateServerAttemptingConnection; 
long                   CPatchServerDlg::m_updateServerTimeoutOnConnection; 
long                   CPatchServerDlg::m_updateServerTimeoutStamp;  

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatchServerDlg dialog

CPatchServerDlg::CPatchServerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPatchServerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPatchServerDlg)
	m_serverStatus = _T("");
	m_ipadress = _T("");
	m_servername = _T("");
	m_cycleDuration = 0;
	m_registrationURL = _T("");
	m_getSignupsURL = _T("");
	m_packetSendSpeedMS = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CPatchServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPatchServerDlg)
	DDX_Control(pDX, IDC_LIST_history, m_history);
	DDX_Control(pDX, IDC_CHECK_EnableSystem, m_enableSystem);
	DDX_Control(pDX, IDC_LIST_targetServers, m_targetServerDB);
	DDX_Control(pDX, IDC_PROGRESS_DLDProgress, m_dldProgressMeter);
	DDX_Text(pDX, IDC_STATIC_Status, m_serverStatus);
	DDX_Text(pDX, IDC_EDIT_IPAdress, m_ipadress);
	DDX_Text(pDX, IDC_EDIT_ServerName, m_servername);
	DDX_Text(pDX, IDC_EDIT_cycleDuration, m_cycleDuration);
	DDX_Text(pDX, IDC_EDIT_registrationURL, m_registrationURL);
	DDX_Text(pDX, IDC_EDIT_getSignupsURL, m_getSignupsURL);
	DDX_Text(pDX, IDC_EDIT_packetSendSpeedMS, m_packetSendSpeedMS);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPatchServerDlg, CDialog)
	//{{AFX_MSG_MAP(CPatchServerDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_testcode, OnBUTTONtestcode)
	ON_BN_CLICKED(IDC_BUTTON_Add, OnBUTTONAdd)
	ON_BN_CLICKED(IDC_BUTTON_Delete, OnBUTTONDelete)
	ON_BN_CLICKED(IDC_BUTTON_Load, OnBUTTONLoad)
	ON_BN_CLICKED(IDC_BUTTON_Save, OnBUTTONSave)
	ON_BN_CLICKED(IDC_BUTTON_saveHistory, OnBUTTONsaveHistory)
	ON_BN_CLICKED(IDC_BUTTON_loadHistory, OnBUTTONloadHistory)
	ON_BN_CLICKED(IDC_BUTTON_RebuildAccounts, OnBUTTONRebuildAccounts)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPatchServerDlg message handlers

BOOL CPatchServerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();



	CoInitialize(NULL);

	WriteDebugFile("Started registration server",TRUE);
	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}


	//read cfg
	CFileException exc2;
    CFile fl2;
	 CArchive the_Archive2(&fl2,CArchive::load);
     BOOL res = fl2.Open("serverstartup.cfg",CFile::modeRead, &exc2);
	 if(res)
	 {
        the_Archive2.ReadString(m_sqlIp);
		the_Archive2.ReadString(m_sqlUserid);
		the_Archive2.ReadString(m_sqlPass);
        the_Archive2.ReadString(m_sqlDB);
	
     the_Archive2.Close();
	 fl2.Close();
	 }

    


	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	m_usersCount                        = 0;
	m_port                              = 25857;
	m_passwordRequired                  = TRUE;
	m_serverKey                         = "xpass01101aa2";
	m_dpAsncEnumHostHandle              = NULL;
	m_lastCommunicationTime             = 0;
	m_maxPlayers                        = 200;
	m_IPaddress                         = "www.endlessages.net";//"65.33.190.193";
    m_userName                          = "adminarb";
    m_passWord                          = "";
	m_serverStartTime                   = 0;
	m_primaryPlayerCreated              = FALSE;
	m_enabledClient                     = FALSE;
	m_currentVersion                    = 0;
	m_clientInterface                   = NULL;  
    m_serverInterface                   = NULL;
	m_clientSentVersionInfo             = FALSE;
	m_inProcess                         = FALSE;
	m_connectionRequested               = FALSE;
	m_timeStamp                         = 0;
//	m_filePatchDB                       = new CFilePatchObjectList();
	m_targetServersList                 = new CServerListObjectDB();
	m_currentlyDldingPatch              = FALSE;
	m_currentPatchSize                  = 0;
	m_currentPatchDldPosition           = 0;
	m_CurrentClientDB                   = new CClientObjList();
	m_currentDldVersion                 = 0;
	m_patchMaxBlockSz                   = 5000;
	//m_currentFilePatchObject            = NULL;
	m_updateServersAccountsInProgress   = FALSE;  
    m_updateServersCurrentIndex         = 0;
    m_updateServerCurrentlyConnected    = FALSE;  
	m_updateServerAttemptingConnection  = FALSE; 
	m_updateServerTimeoutOnConnection   = 10000;
	m_updateServerTimeoutStamp          = GetTickCount(); 
	m_updateServerPOSITION              = 0;
	m_updateServerUpdateingPerfect      = TRUE;
	m_updateServerSentData              = FALSE;
	m_cycleTimeStamp                    = GetTickCount();
	

	InitializeCriticalSection(  &m_criticalSection );
	
	UpdateData(TRUE);
	m_cycleDuration                     = 10000;
	m_registrationURL = "http://www.endlessages.com/example/registerall.php";
	m_getSignupsURL = "http://www.endlessages.com/example/getnewsignups.php";
	m_packetSendSpeedMS                 = 50;
	UpdateData(FALSE);

	m_accountsInfoChanges               = new CAccountSQLDatabase();
	m_accountsToPlaceOnHold             = new CAccountSQLDatabase();
	m_accountsToTakeOffHold             = new CAccountSQLDatabase();
	m_accountsToBeEntered               = new CAccountSQLDatabase();
	m_accountadditionHistory            = new CAccountSQLDatabase();
	m_accountsToBeDeleted               = new CAccountSQLDatabase();
	m_expChangesToBeEntered             = new CAccountSQLDatabase();
	

	m_dldProgressMeter.SetRange(0,100);
	m_dldProgressMeter.SetPos(0);

    SetTimer( 0, 10, 0 ); 

	InitializeCriticalSection(  &m_recieveCriticalSection );

	
	return TRUE;  // return TRUE  unless you set the focus to a control
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
HCURSOR CPatchServerDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
BOOL CPatchServerDlg::ConnectToServer(CString ip) 
{
m_IPaddress = ip;

if(m_connectionRequested)
	   return FALSE;//dont enum twice

	   m_connectionRequested = TRUE;
    
	   UpdateData(TRUE);	
		m_serverStatus = "Connecting....";
	   UpdateData(FALSE);


	if(m_packetQue){
	   m_packetQue->SafeDelete();
       delete m_packetQue;
	   m_packetQue = 0;
	}
	//retrieve version from file   
    m_currentVersion = GetVersionFromFile();//client side only 
	//UpdateData(TRUE);
	//m_versionUIDisplay = ConvToString(m_currentVersion);
	//UpdateData(FALSE);

	m_packetQue          = new CPacketObjList();

    if(m_clientInterface)
	{
	   m_clientInterface->Close(0);
       m_clientInterface->Release();
       m_clientInterface = 0;
	}


	HRESULT hr;
     if( FAILED( hr = CoCreateInstance( CLSID_DirectPlay8Client, NULL, 
                                        CLSCTX_INPROC_SERVER,
                                        IID_IDirectPlay8Client, 
                                        (LPVOID*) &m_clientInterface ) ) )
	 {
		AfxMessageBox("Error initializing Com Svr interface");
        return FALSE;
	 }

 
	 UpdateData(TRUE);
	// Init IDirectPlay8Client
    if( FAILED( hr = m_clientInterface->Initialize( NULL, DirectPlayMessageHandler, 0 ) ) )
	{
		AfxMessageBox("Error initializing Com Svr interface");
        return FALSE;
	}


	UpdateData(FALSE);
  

	EnumHosts(m_IPaddress);
	m_timeStamp = GetTickCount();
	return TRUE;
}
//-------------END FUNCTION--------------------------------------//
//---------------------------------------------------------------//
//------------enum hosts-----------------------------------------//
BOOL CPatchServerDlg::EnumHosts(CString TCPIP)
{
    if(!m_clientInterface)
		return FALSE;

    StopEnumerating();//cancel last enumeration

	HRESULT hr;
	DPN_APPLICATION_DESC   dpnAppDesc;
    IDirectPlay8Address*   pDP8AddressHost  = NULL;
    IDirectPlay8Address*   pDP8AddressLocal = NULL;

    // Create the local device address object
    if( FAILED( hr = CoCreateInstance( CLSID_DirectPlay8Address, NULL, 
                                       CLSCTX_ALL, IID_IDirectPlay8Address,
                                       (LPVOID*) &pDP8AddressLocal ) ) )
    return FALSE;

    // Set IP service provider
    if( FAILED( hr = pDP8AddressLocal->SetSP( &CLSID_DP8SP_TCPIP ) ) )
    return FALSE;


    // Create the remote host address object
    if( FAILED( hr = CoCreateInstance( CLSID_DirectPlay8Address, NULL, 
                                       CLSCTX_ALL, IID_IDirectPlay8Address,
                                       (LPVOID*) &pDP8AddressHost ) ) )
    return FALSE;
	// Set IP service provider
    if( FAILED( hr = pDP8AddressHost->SetSP( &CLSID_DP8SP_TCPIP ) ) )
    return FALSE;

	TCHAR strIPAddress[MAX_PATH];
	lstrcpy(strIPAddress,TCPIP);


    ZeroMemory( &dpnAppDesc, sizeof( DPN_APPLICATION_DESC ) );
    dpnAppDesc.dwSize = sizeof( DPN_APPLICATION_DESC );
    dpnAppDesc.guidApplication = g_guidApp;


	if(TCPIP.GetLength() != 0){//specific IP
      WCHAR       wszHostName[MAX_PATH];
      MultiByteToWideChar(CP_ACP, 0,TCPIP, -1, wszHostName, sizeof(wszHostName)/sizeof(wszHostName[0]));

      hr = pDP8AddressHost->AddComponent( DPNA_KEY_HOSTNAME, wszHostName, 
                                         (wcslen(wszHostName)+1)*sizeof(WCHAR), 
                                          DPNA_DATATYPE_STRING );
	  if( FAILED(hr))
		  return FALSE;
	}//end specific IP
    if(m_port != 0){//specific port
	     hr = pDP8AddressHost->AddComponent( DPNA_KEY_PORT, 
                                             &m_port, sizeof(m_port),
                                             DPNA_DATATYPE_DWORD );
         if( FAILED(hr))
		     return FALSE;
	}//end specific port

	//get local device

	hr = m_clientInterface->EnumHosts( &dpnAppDesc, pDP8AddressHost, 
                                        pDP8AddressLocal, NULL, 
                                        0, INFINITE, 0, INFINITE, NULL, 
                                        &m_dpAsncEnumHostHandle,0);

    if(pDP8AddressLocal)
       pDP8AddressLocal->Release();
	   pDP8AddressLocal = 0;
	if(pDP8AddressHost)
	   pDP8AddressHost->Release();
	   pDP8AddressHost = 0;


	//set service provider caps
       /* DPN_SP_CAPS spCaps;
		ZeroMemory(&spCaps,sizeof(DPN_SP_CAPS));
		spCaps.dwSize = sizeof(DPN_SP_CAPS);
		m_clientInterface->GetSPCaps(&CLSID_DP8SP_TCPIP,
			                         &spCaps,
                                     0);

		//spCaps.dwBuffersPerThread = spCaps.dwBuffersPerThread + 4;

		spCaps.dwSystemBufferSize = spCaps.dwSystemBufferSize * 50;

		m_clientInterface->SetSPCaps(&CLSID_DP8SP_TCPIP,
			                         &spCaps,
									 0);
		//end set service provider caps*/


	return TRUE;
}
//-------------END FUNCTION--------------------------------------//
//---------------------------------------------------------------//
//------------enum hosts-----------------------------------------//
BOOL CPatchServerDlg::StopEnumerating()
{
 if(!m_clientInterface)
	 return FALSE;

 if(!m_dpAsncEnumHostHandle )
     return FALSE;

     m_clientInterface->CancelAsyncOperation( m_dpAsncEnumHostHandle, 0 );

	 m_dpAsncEnumHostHandle = 0;
     return TRUE;
}
//-----------------------------------------------------------//
//-----------------------------------------------------------//
//-----------------------------------------------------------//
HRESULT WINAPI CPatchServerDlg::DirectPlayMessageHandler( PVOID pvUserContext, 
                                                          DWORD dwMessageId, 
                                                          PVOID pMsgBuffer )
{

    // Try not to stay in this message handler for too long, otherwise
    // there will be a backlog of data.  The best solution is to 
    // queue data as it comes in, and then handle it on other threads.
    
    // This function is called by the DirectPlay message handler pool of 
    // threads, so be careful of thread synchronization problems with shared memory
    //dwMessageId

    switch( dwMessageId )
    {
        case DPN_MSGID_CREATE_PLAYER:
        {
			PDPNMSG_CREATE_PLAYER createPlayerMsg;
            createPlayerMsg = (PDPNMSG_CREATE_PLAYER)pMsgBuffer;

			EnterCriticalSection(  &m_recieveCriticalSection  );

			CClientObj * newClientObj = new CClientObj();
			newClientObj->m_dpnidCom = createPlayerMsg->dpnidPlayer;
            m_CurrentClientDB->AddTail(newClientObj);

			if(!m_primaryPlayerCreated){
		        //m_eventLog->AddEvent("Primary Server Player Created");
                m_primaryPlayerCreated = TRUE;
			}
			else{//handle client player creation
				 CString ipAdress = GetIPAddress(createPlayerMsg->dpnidPlayer);
                 m_usersCount++;
                 
			}//end handle client player creation
            LeaveCriticalSection( &m_recieveCriticalSection);
			break;
        }

        case DPN_MSGID_DESTROY_PLAYER:
        {
			PDPNMSG_DESTROY_PLAYER disconnectDataMsg;
			disconnectDataMsg = (PDPNMSG_DESTROY_PLAYER)pMsgBuffer;

            EnterCriticalSection(  &m_recieveCriticalSection  );
			m_usersCount--;
            m_CurrentClientDB->LogOut(disconnectDataMsg->dpnidPlayer);
			LeaveCriticalSection( &m_recieveCriticalSection);

            break;
        }

        case DPN_MSGID_INDICATE_CONNECT:
		{
			//Validate Connection
            PDPNMSG_INDICATE_CONNECT connectDataMsg;

			connectDataMsg = (PDPNMSG_INDICATE_CONNECT)pMsgBuffer;
			

			if(connectDataMsg->dwUserConnectDataSize == sizeof(CONTEXT_VALIDATION)){
               LPCONTEXT_VALIDATION cntxValidation = (LPCONTEXT_VALIDATION)connectDataMsg->pvUserConnectData;
               CString userName = cntxValidation->username;
               CString password = cntxValidation->password;
			   


               if(!connectDataMsg->pAddressPlayer){//cannot log IP
				   //m_eventLog->AddEvent("Cannot log IP.. rejecting connection");
				   return 2;
			   }//end cannot log IP

                  CString ipAdress = GetIPAddress(connectDataMsg->pAddressPlayer);
				  
				  if(ipAdress != "Undefined"){//valid IP
					 return S_OK;//allow log in
				  }
			}
			return 2;//just reject if it gets here
		}

		case DPN_MSGID_CONNECT_COMPLETE:
		{
            PDPNMSG_CONNECT_COMPLETE connectCompleteMsg;
            connectCompleteMsg = (PDPNMSG_CONNECT_COMPLETE)pMsgBuffer;
            
			if(DPNERR_INVALIDPASSWORD == connectCompleteMsg->hResultCode){
			   //m_eventLog->AddEvent("Invalid password");
			   break;
			}
       	       
			if(DPNERR_HOSTREJECTEDCONNECTION == connectCompleteMsg->hResultCode){
			   //m_eventLog->AddEvent("Server Rejected Connection");
			   break;
			} 

			if(DPNERR_SESSIONFULL == connectCompleteMsg->hResultCode){
               //m_eventLog->AddEvent("Server Full");
			   break;
			}

			if(S_OK == connectCompleteMsg->hResultCode){
               m_enabledClient = TRUE;
               m_updateServerCurrentlyConnected = TRUE;
			   //m_serverStatus = "Connected....";
			   break;
			}
			
			//m_eventLog->AddEvent("Connection failed...");
            break;
		}

        case DPN_MSGID_TERMINATE_SESSION:
        {    
			//m_eventLog->AddEvent("Lost Connection");
            /*PDPNMSG_TERMINATE_SESSION pTerminateSessionMsg;
            pTerminateSessionMsg = (PDPNMSG_TERMINATE_SESSION)pMsgBuffer;*/

            break;
        }

		case DPN_MSGID_ENUM_HOSTS_RESPONSE:
        {
            PDPNMSG_ENUM_HOSTS_RESPONSE pEnumHostsResponseMsg;
            pEnumHostsResponseMsg = (PDPNMSG_ENUM_HOSTS_RESPONSE)pMsgBuffer;
            DPNHANDLE                asyncLocalHandle;
			//specify password
            WCHAR  wCharPassCnv[MAX_PATH];
            MultiByteToWideChar(CP_ACP, 0, m_serverKey, -1, wCharPassCnv, sizeof(wCharPassCnv)/sizeof(wCharPassCnv[0]));
            DPN_APPLICATION_DESC dpnAppDesc = *pEnumHostsResponseMsg->pApplicationDescription;
			dpnAppDesc.pwszPassword = wCharPassCnv;
			//m_eventLog->AddEvent("Attemting connection to",ConvertWideStringToCString(dpnAppDesc.pwszSessionName));
			
			LPCONTEXT_VALIDATION cntxValidation = NULL;
	        DWORD messageSize = sizeof(CONTEXT_VALIDATION);
			cntxValidation = (LPCONTEXT_VALIDATION) new BYTE[messageSize];
			lstrcpy(cntxValidation->password,m_passWord);
			lstrcpy(cntxValidation->username,m_userName);

			cntxValidation->versionInfo = m_currentVersion;
			
			HRESULT hr = m_clientInterface->Connect(&dpnAppDesc,                          // the application desc
                                                    pEnumHostsResponseMsg->pAddressSender,                              // address of the host of the session
                                                    pEnumHostsResponseMsg->pAddressDevice,    // address of the local device the enum responses were received on
                                                    NULL, 
													NULL,                          // DPN_SECURITY_DESC, DPN_SECURITY_CREDENTIALS
                                                    (BYTE*)cntxValidation,         // user data
													messageSize,                   // user data size
                                                    NULL, 
													&asyncLocalHandle,                          // async context, async handle,
                                                    0); // flags

            if(m_dpAsncEnumHostHandle)
            m_clientInterface->CancelAsyncOperation( m_dpAsncEnumHostHandle, 0 );
	  
            break;
        }

        case DPN_MSGID_RECEIVE:
        {

			EnterCriticalSection(  &m_recieveCriticalSection  );

			m_lastCommunicationTime = GetTickCount();
            PDPNMSG_RECEIVE pReceiveMsg;
            pReceiveMsg = (PDPNMSG_RECEIVE)pMsgBuffer;
			
			if(pReceiveMsg->dwReceiveDataSize <= 0){//invalid data
               //m_eventLog->AddEvent("Invalid packet Size ",pReceiveMsg->dwReceiveDataSize);
			}//end invalid data
			else{//add packet to que
			m_packetQue->AddPacket(pReceiveMsg->dwReceiveDataSize,
				                   pReceiveMsg->pReceiveData,
								   pReceiveMsg->dpnidSender); 
			}//end add packet to que
            
			LeaveCriticalSection( &m_recieveCriticalSection);
			
			break;
        }
    }

    return S_OK;
}

//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
CString WINAPI CPatchServerDlg::ConvertWideStringToCString(const WCHAR* wstrSource)
{
    char temp[256];
	ConvertWideStringToAnsi(temp,wstrSource,-1);
    CString ret=temp;
    return ret;
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
CString WINAPI CPatchServerDlg::GetIPAddress(IDirectPlay8Address * pAddress)
{
               CString returnString;
	           DWORD bufferSize=0;
			   DWORD type = DPNA_DATATYPE_STRING;
			   pAddress->GetComponentByName(DPNA_KEY_HOSTNAME,
			                                                      NULL,
                                                                  &bufferSize,
								                                  &type);

			   WCHAR  ipAdress[250];
			   if(bufferSize < 240 && bufferSize > 0 )//reasonable range
			   if(pAddress->GetComponentByName(DPNA_KEY_HOSTNAME,
			                                                         &ipAdress,
                                                                     &bufferSize,
																	 &type)==S_OK){
				  
				  //GetAddress(WCHAR* wszAddress,PDWORD pdwAddressLength,USHORT* psPort );

				  returnString = ConvertWideStringToCString(ipAdress);
				  return returnString;
			   }
			   return "Undefined";
}
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
//-----------------------------------------------------------------------------//
CString WINAPI CPatchServerDlg::GetIPAddress(DPNID id)
{
   if(!m_serverInterface)
	   return "Undefined";

   IDirectPlay8Address * pAddress = NULL;
   if(m_serverInterface->GetClientAddress(id,&pAddress,0)==S_OK)
   {
               CString returnString;
	           DWORD bufferSize=0;
			   DWORD type = DPNA_DATATYPE_STRING;
			   pAddress->GetComponentByName(DPNA_KEY_HOSTNAME,
			                                                      NULL,
                                                                  &bufferSize,
								                                  &type);

			   WCHAR  ipAdress[250];
			   if(bufferSize < 240 && bufferSize > 0 )//reasonable range
			   if(pAddress->GetComponentByName(DPNA_KEY_HOSTNAME,
			                                                         &ipAdress,
                                                                     &bufferSize,
																	 &type)==S_OK){
				  
				   
				  returnString = ConvertWideStringToCString(ipAdress);
				  if(pAddress){
                     pAddress->Release();
					 pAddress=0;
				  }
				  return returnString;
			   }
			   return "Undefined";

   }
   return "Undefined";
}
//-----------------------------------------------------------------------------
// Name: DXUtil_ConvertWideStringToAnsi()
// Desc: This is a UNICODE conversion utility to convert a WCHAR string into a
//       CHAR string. cchDestChar defaults -1 which means it 
//       assumes strDest is large enough to store strSource
//-----------------------------------------------------------------------------
void WINAPI CPatchServerDlg::ConvertWideStringToAnsi( CHAR* strDestination, const WCHAR* wstrSource, 
                                     int cchDestChar )
{
    if( strDestination==NULL || wstrSource==NULL )
        return;

    if( cchDestChar == -1 )
        cchDestChar = wcslen(wstrSource)+1;

    WideCharToMultiByte( CP_ACP, 0, wstrSource, -1, strDestination, 
                         cchDestChar-1, NULL, NULL );

    strDestination[cchDestChar-1] = 0;
}

//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::OnTimer(UINT nIDEvent) 
{
	EnterCriticalSection(  &m_criticalSection  );
	
	
	m_inProcess = TRUE;
	
	CDialog::OnTimer(nIDEvent);
    UpdateData(TRUE);
	
		  //check message que
          LPVOID lpMessage = NULL;
          DPNID messageFrom = 0;
          long currentTickCount = GetTickCount();
          int sizeTracker = 0;

          if(m_enableSystem.GetCheck())
          if((GetTickCount() - m_cycleTimeStamp) > m_cycleDuration)
	      {
			  BOOL sqlInited = TRUE;
              _RecordsetPtr  pRst;
	          _bstr_t strCnn("Provider=sqloledb;Data Source="+m_sqlIp+";Initial Catalog="+m_sqlDB+";User Id="+m_sqlUserid+";Password="+m_sqlPass+";");
	          pRst.CreateInstance(__uuidof(Recordset));
              BOOL enabled = TRUE;
              try
			  {
	           //execute connection method
               pRst->Open(
               "select * from ServerStatus where Status <> 'Online' OR DATEDIFF(minute,LastUpdated,GETDATE()) > 3",//table
               strCnn,
               adOpenStatic,
               adLockOptimistic,
               adCmdText);
			  }
	          catch(_com_error &e)
			  {
	           sqlInited = FALSE;
               _bstr_t bstrSource(e.Source());
               _bstr_t bstrDescription(e.Description());
			  }
			  if(sqlInited)
			  {
                 int offlineServerCount = pRst->RecordCount;
				 if(offlineServerCount > 0)
				 {//stop if all server arent online
                    m_inProcess = FALSE;
					m_serverStatus = "PAUSED::All servers not online....";
	                UpdateData(FALSE);
	                LeaveCriticalSection( &m_criticalSection);
					return;
				 }//end stop if all server arent online

                 Cycle();
                 m_cycleTimeStamp = GetTickCount();
				 pRst->Close();
			  }
			  else{//stop if sql not aquired
                 m_inProcess = FALSE;
				 m_serverStatus = "PAUSED::SQL not online...";
	             UpdateData(FALSE);
	             LeaveCriticalSection( &m_criticalSection);
				 return;
			  }//end stop if sql not aquired
		  }

		  //m_updateServerAttemptingConnection  = FALSE; 
	      //m_updateServerTimeoutOnConnection   = 10000;
	      //m_updateServerTimeoutStamp          = 0; 
		 if(!m_updateServerCurrentlyConnected)
		 {//not connected yet
          if(m_updateServersAccountsInProgress)
		  {//update callback
             if(!m_updateServerAttemptingConnection)
			 {
                 
                 if(!m_updateServerPOSITION)
                     m_updateServerPOSITION = m_targetServersList->GetHeadPosition();
				 else
                     m_targetServersList->GetNext(m_updateServerPOSITION);

				 if(m_updateServerPOSITION)
				 {
                    CServerListObject * svrPtr = (CServerListObject *)m_targetServersList->GetAt(m_updateServerPOSITION);
                    ConnectToServer(svrPtr->m_IPadress);
                    m_updateServerAttemptingConnection = TRUE;
					m_updateServerTimeoutStamp = GetTickCount(); 
				 }
				 else{//done for this cycle-turn off cycle
                    
					m_updateServersAccountsInProgress  = FALSE;
	                m_updateServerAttemptingConnection = FALSE;
                    m_updateServerCurrentlyConnected   = FALSE;
		            m_connectionRequested              = FALSE;
		            m_enabledClient                    = FALSE;
	         	    m_updateServerSentData             = FALSE;

					if(m_clientInterface)
					{
					   m_clientInterface->Close(0);
                       m_clientInterface->Release();
                       m_clientInterface = 0;
					}
                    if(m_updateServerUpdateingPerfect)
					{//cycle is perfect cache it
					   if(m_targetServersList)
					   {
                        /*m_accountsToBeEntered->GetCount() > 0 ||
		                m_accountsToPlaceOnHold->GetCount() > 0 ||
		                 m_accountsToTakeOffHold->GetCount() > 0 ||
		                m_accountsInfoChanges->GetCount() > 0 || 
		                m_accountsToBeDeleted->GetCount() > 0 || 
		                m_expChangesToBeEntered->GetCount() > 0)*/
						   
						   
						for(POSITION tPos = m_accountsToBeEntered->GetHeadPosition();tPos != NULL;)
						{
						   CAccountSQLObj * tPtr = (CAccountSQLObj *)m_accountsToBeEntered->GetNext(tPos);
						   WriteDebugFile("Account Entered:"+tPtr->m_username,FALSE);
						   m_accountadditionHistory->AddTail(tPtr);
						}
					    m_accountsToBeEntered->RemoveAll();

                        if(m_accountsToPlaceOnHold)
						{
							for(POSITION tPos = m_accountsToPlaceOnHold->GetHeadPosition();tPos != NULL;)
							{
							    CAccountSQLObj * tPtr = (CAccountSQLObj *)m_accountsToPlaceOnHold->GetNext(tPos);
								WriteDebugFile("Account Placed on hold:"+tPtr->m_username,FALSE);
							}
							m_accountsToPlaceOnHold->SafeDelete();
						}

						if(m_accountsToTakeOffHold)
						{
							for(POSITION tPos = m_accountsToTakeOffHold->GetHeadPosition();tPos != NULL;)
							{
							    CAccountSQLObj * tPtr = (CAccountSQLObj *)m_accountsToTakeOffHold->GetNext(tPos);
								WriteDebugFile("Account took off hold:"+tPtr->m_username,FALSE);
							}
							m_accountsToTakeOffHold->SafeDelete();
						}

						if(m_accountsInfoChanges)
						{
							for(POSITION tPos = m_accountsInfoChanges->GetHeadPosition();tPos != NULL;)
							{
							    CAccountSQLObj * tPtr = (CAccountSQLObj *)m_accountsInfoChanges->GetNext(tPos);
								WriteDebugFile("Account info changes"+tPtr->m_username,FALSE);
							}
							m_accountsInfoChanges->SafeDelete();
						}

						if(m_accountsToBeDeleted)
						{
							for(POSITION tPos = m_accountsToBeDeleted->GetHeadPosition();tPos != NULL;)
							{
							    CAccountSQLObj * tPtr = (CAccountSQLObj *)m_accountsToBeDeleted->GetNext(tPos);
								WriteDebugFile("account to be deleted"+tPtr->m_username,FALSE);
							}
							m_accountsToBeDeleted->SafeDelete();
						}

					
						if(m_expChangesToBeEntered)
						{
							for(POSITION tPos = m_expChangesToBeEntered->GetHeadPosition();tPos != NULL;)
							{
							    CAccountSQLObj * tPtr = (CAccountSQLObj *)m_expChangesToBeEntered->GetNext(tPos);
								WriteDebugFile("Expansion upgraded:"+tPtr->m_username,FALSE);
							}
							m_expChangesToBeEntered->SafeDelete();
						}

						//SaveHistoryDB("dynAccHistory.txt");
						InListHistory(m_accountadditionHistory->GetCount()-1);
					   }
					}//end cycle is perfect
				 }//end done for this cycle-turn off cycle
			 }
			 else if((GetTickCount()-m_updateServerTimeoutStamp) > 10000 && m_updateServerCurrentlyConnected == FALSE){
                m_updateServerUpdateingPerfect     = FALSE;//we have failed to connect this time around
				m_updateServerAttemptingConnection = FALSE;
	            m_updateServerCurrentlyConnected   = FALSE;
		        m_connectionRequested              = FALSE;
		        m_enabledClient                    = FALSE;
		        m_updateServerSentData             = FALSE;
	            if(m_clientInterface)
				{
		           m_clientInterface->Close(0);
                   m_clientInterface->Release();
                   m_clientInterface = 0;
				}
			 }
		  }//end update callback
		 }//end not connected yet
		 else{//connected
            if(!m_updateServerSentData)
			{
				SendLogFast(m_userName,m_passWord);//establish link comm
				if(m_accountsToBeEntered)
                for(POSITION posLoc = m_accountsToBeEntered->GetHeadPosition();posLoc != NULL;)
				{
                    CAccountSQLObj * acPtr = (CAccountSQLObj *)m_accountsToBeEntered->GetNext(posLoc);
	  	            SendAccountData(acPtr->m_username,acPtr->m_password);
					Sleep(m_packetSendSpeedMS);
				}
				if(m_accountsInfoChanges)
				for(POSITION posLoc = m_accountsInfoChanges->GetHeadPosition();posLoc != NULL;)
				{
				    CAccountSQLObj * acPtr = (CAccountSQLObj *)m_accountsInfoChanges->GetNext(posLoc);
					ChangeAccountInfo(acPtr->m_username,
								      acPtr->m_password,
									  acPtr->m_newUsername,
								      acPtr->m_newPassword);
					Sleep(m_packetSendSpeedMS);
				}
				if(m_accountsToPlaceOnHold)
			    for(POSITION posLoc = m_accountsToPlaceOnHold->GetHeadPosition();posLoc != NULL;)
				{
				    CAccountSQLObj * acPtr = (CAccountSQLObj *)m_accountsToPlaceOnHold->GetNext(posLoc);
					PlaceAccountOnHold(acPtr->m_username,acPtr->m_password);
					Sleep(m_packetSendSpeedMS);
				}
				if(m_accountsToTakeOffHold)
			    for(POSITION posLoc = m_accountsToTakeOffHold->GetHeadPosition();posLoc != NULL;)
				{
				    CAccountSQLObj * acPtr = (CAccountSQLObj *)m_accountsToTakeOffHold->GetNext(posLoc);
					TakeAccountOffHold(acPtr->m_username,acPtr->m_password);
					Sleep(m_packetSendSpeedMS);
				}
				if(m_accountsToBeDeleted)
			    for(POSITION posLoc = m_accountsToBeDeleted->GetHeadPosition();posLoc != NULL;)
				{
				    CAccountSQLObj * acPtr = (CAccountSQLObj *)m_accountsToBeDeleted->GetNext(posLoc);
					DeleteAccountData(acPtr->m_username,acPtr->m_password);
					Sleep(m_packetSendSpeedMS);
				}
				if(m_expChangesToBeEntered)
				for(POSITION posLoc = m_expChangesToBeEntered->GetHeadPosition();posLoc != NULL;)
				{
				    CAccountSQLObj * acPtr = (CAccountSQLObj *)m_expChangesToBeEntered->GetNext(posLoc);
					ExpansionEnable(acPtr->m_username,acPtr->m_password);
					Sleep(m_packetSendSpeedMS);
				}
				
                m_updateServerSentData = TRUE;
				SendCompletedDataTail();
			}
		 }//end connected


    if(m_clientInterface)
	{//handle client callbacks
       if(m_enabledClient)
	   {//client interfaced
          while(GetNextPacket(&sizeTracker,(BYTE**)&lpMessage,&messageFrom)==TRUE)
		  {//if theres a message handle it
	         //GL_mutiplayerObject->m_totalBandwidthIncoming += sizeTracker;
             //HandleEngineMessagesClientSide(lpMessage,messageFrom,sizeTracker);
			 HandleMessagesClientSide(lpMessage,messageFrom,sizeTracker);
		  }//end if theres a message handle it
          //end check message que
	   }//end client interfaced
	}//end handle client callbacks

	m_inProcess = FALSE;
	UpdateData(FALSE);
	LeaveCriticalSection( &m_criticalSection);
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::SendCompletedDataTail()
{
//clinet side function
 EXMSG_CATAGORIE dataPacket;
 int messageSize = sizeof(EXMSG_CATAGORIE);
    dataPacket.m_catagorie   = 22;

    SendData((BYTE *)&dataPacket,
		     messageSize,
		     DPNSEND_GUARANTEED,
		     NULL);
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::SendVersion(int version)
{
//clinet side function
 VERSION_INFO dataPacket;
 int messageSize = sizeof(VERSION_INFO);
    dataPacket.m_catagorie   = 1;
	dataPacket.m_version     = version;

    SendData((BYTE *)&dataPacket,
		     messageSize,
		     DPNSEND_GUARANTEED,
		     NULL);
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::DeleteAccountData(CString username,
								    CString password)
{
//clinet side function
 ADD_ACCOUNT dataPacket;
 int messageSize = sizeof(ADD_ACCOUNT);
 ZeroMemory(&dataPacket,messageSize);
   

 dataPacket.m_catagorie = 21;
 dataPacket.m_type      = 4;
 lstrcpy(dataPacket.username,username);
 lstrcpy(dataPacket.password,password);

    SendData((BYTE *)&dataPacket,
		     messageSize,
		     DPNSEND_GUARANTEED,
		     NULL);
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::ExpansionEnable(CString username,
								      CString expLevel)
{
//clinet side function
 ADD_ACCOUNT dataPacket;
 int messageSize = sizeof(ADD_ACCOUNT);
 ZeroMemory(&dataPacket,messageSize);
   

 dataPacket.m_catagorie = 21;
 dataPacket.m_type      = 8;
 lstrcpy(dataPacket.username,username);
 lstrcpy(dataPacket.password,expLevel);

    SendData((BYTE *)&dataPacket,
		     messageSize,
		     DPNSEND_GUARANTEED,
		     NULL);
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::SendAccountData(CString username,
								      CString password)
{
//clinet side function
 ADD_ACCOUNT dataPacket;
 int messageSize = sizeof(ADD_ACCOUNT);
 ZeroMemory(&dataPacket,messageSize);
   

 dataPacket.m_catagorie = 21;
 dataPacket.m_type      = 0;
 lstrcpy(dataPacket.username,username);
 lstrcpy(dataPacket.password,password);

    SendData((BYTE *)&dataPacket,
		     messageSize,
		     DPNSEND_GUARANTEED,
		     NULL);
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::PlaceAccountOnHold(CString username,
								         CString password)
{
//clinet side function
 ADD_ACCOUNT dataPacket;
 int messageSize = sizeof(ADD_ACCOUNT);
 ZeroMemory(&dataPacket,messageSize);
   

 dataPacket.m_catagorie = 21;
 dataPacket.m_type      = 1;
 lstrcpy(dataPacket.username,username);
 lstrcpy(dataPacket.password,password);

    SendData((BYTE *)&dataPacket,
		     messageSize,
		     DPNSEND_GUARANTEED,
		     NULL);
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::TakeAccountOffHold(CString username,
								         CString password)
{
//clinet side function
 ADD_ACCOUNT dataPacket;
 int messageSize = sizeof(ADD_ACCOUNT);
 ZeroMemory(&dataPacket,messageSize);
   

 dataPacket.m_catagorie = 21;
 dataPacket.m_type      = 2;
 lstrcpy(dataPacket.username,username);
 lstrcpy(dataPacket.password,password);

    SendData((BYTE *)&dataPacket,
		     messageSize,
		     DPNSEND_GUARANTEED,
		     NULL);
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::ChangeAccountInfo(CString username,
								        CString password,
										CString newUsername,
								        CString newPassword)
{
//clinet side function
 ADD_ACCOUNT dataPacket;
 int messageSize = sizeof(ADD_ACCOUNT);
 ZeroMemory(&dataPacket,messageSize);
   

 dataPacket.m_catagorie = 21;
 dataPacket.m_type      = 3;
 lstrcpy(dataPacket.username,username);
 lstrcpy(dataPacket.password,password);
 lstrcpy(dataPacket.newUsername,newUsername);
 lstrcpy(dataPacket.newPassword,newPassword);

    SendData((BYTE *)&dataPacket,
		     messageSize,
		     DPNSEND_GUARANTEED,
		     NULL);
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
    if(m_serverInterface){
       m_serverInterface->Close(0);
       m_serverInterface->Release();
       m_serverInterface = 0;
	}
    if(m_clientInterface){
       m_clientInterface->Close(0);
       m_clientInterface->Release();
       m_clientInterface = 0;
	}

    /*if(m_filePatchDB)
	{
       m_filePatchDB->SafeDelete();
	   delete m_filePatchDB;
	   m_filePatchDB = 0;
	}*/

	if(m_packetQue)
	{
       m_packetQue->SafeDelete();
	   delete m_packetQue;
	   m_packetQue = 0;
	}

    if(m_targetServersList)
	{
       m_targetServersList->SafeDelete();
       delete m_targetServersList;
       m_targetServersList = 0;
	}

	if(m_CurrentClientDB)
	{
       m_CurrentClientDB->SafeDelete();
	   delete m_CurrentClientDB;
	   m_CurrentClientDB = 0;
	}

    if(m_accountsInfoChanges)
	{
       m_accountsInfoChanges->SafeDelete();
	   delete m_accountsInfoChanges;
	   m_accountsInfoChanges = 0;
	}

	if(m_accountsToPlaceOnHold)
	{
       m_accountsToPlaceOnHold->SafeDelete();
	   delete m_accountsToPlaceOnHold;
	   m_accountsToPlaceOnHold = 0;
	}

	if(m_accountsToTakeOffHold)
	{
       m_accountsToTakeOffHold->SafeDelete();
	   delete m_accountsToTakeOffHold;
	   m_accountsToTakeOffHold = 0;
	}

	if(m_accountsToBeEntered)
	{
       m_accountsToBeEntered->SafeDelete();
	   delete m_accountsToBeEntered;
	   m_accountsToBeEntered = 0;
	}

    if(m_accountadditionHistory)
	{
       m_accountadditionHistory->SafeDelete();
	   delete m_accountadditionHistory;
	   m_accountadditionHistory = 0;
	}


	KillTimer(0); 
	CoUninitialize( );
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
void CPatchServerDlg::SendData(BYTE * data,
							   DWORD size,
							   DWORD flags,
							   DPNID sendTo)
{
	//Universal data send for both client or server
	DPNHANDLE hAsync;
    DPN_BUFFER_DESC bufferDesc;
	bufferDesc.dwBufferSize = size;    
	bufferDesc.pBufferData = (BYTE*)data;
 
	HRESULT result = 0;

    if(m_clientInterface){//client side
       result = m_clientInterface->Send(&bufferDesc,//const DPN_BUFFER_DESC *const pBufferDesc,
	                                            1,//const DWORD cBufferDesc,
                                                0,//const DWORD dwTimeOut,
							                    NULL,//void *const pvAsyncContext,
							                    &hAsync,//DPNHANDLE *const phAsyncHandle,
							                    flags);//FIX ME

	   //check error codes
	   if(result == DPNSUCCESS_PENDING)
		  return;
       if(result == S_OK)
		  return;

	}//end client side

	if(m_serverInterface){//server side
       result = m_serverInterface->SendTo( sendTo,//ID of player on server
	                                               &bufferDesc,//const DPN_BUFFER_DESC *const pBufferDesc,
	                                               1,//const DWORD cBufferDesc,
                                                   0,//const DWORD dwTimeOut,
							                       NULL,//void *const pvAsyncContext,
							                       &hAsync,//DPNHANDLE *const phAsyncHandle,
							                       flags);//FIX ME

       //check error codes
	   if(result == DPNSUCCESS_PENDING)
		  return;
       if(result == S_OK)
		  return;
       if(result == DPNERR_CONNECTIONLOST){
		  return;
	   }
	   if(result == DPNERR_INVALIDFLAGS){
		  return;
	   }
       if(result == DPNERR_INVALIDPLAYER){
 
		  return;
	   }
	   if(result == DPNERR_TIMEDOUT){
         
		  return;
	   }
	   }//end server side
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//------------------------------------------------------------------------------------//
inline BOOL CPatchServerDlg::GetNextPacket(int * size,BYTE ** data,DPNID * id)
{
	EnterCriticalSection(  &m_recieveCriticalSection  );

 	POSITION posLoc = m_packetQue->FindIndex(0);
    if(!posLoc){
		LeaveCriticalSection( &m_recieveCriticalSection);
	    return FALSE;
	}
	    CPacketObj * packet = (CPacketObj *)m_packetQue->GetAt(posLoc);
        *size = packet->m_size;
		*data = new BYTE[packet->m_size];
		*id   = packet->m_communicationID;
        CopyMemory(*data,packet->m_data,packet->m_size);
		packet->SafeDelete();
		delete packet;
		packet=0;
        m_packetQue->RemoveAt(posLoc);

	LeaveCriticalSection( &m_recieveCriticalSection);

	return TRUE;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----converts a long into string-----------------------------//
CString CPatchServerDlg::ConvToString(int number)
{    
              if(number==0)
				 return "0";
	 
	          int decimal,sign;
              char *buffer;
              CString temp;	 
	          buffer = _fcvt(number,0,&decimal,&sign);
    	      temp = buffer;
			  if(sign != 0)
                 temp = operator+("-",temp);
		
			  return temp;
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
void CPatchServerDlg::UpdateVersion()
{
  
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::HandleServerListClientSide(LPVOID lpMessage,DPNID idFromLoc,int msgSize)
{
    
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::SendServerList(DPNID sendTo)
{
     
}

//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::InListAvailableServers(int sel)
{
  
}
//--------------------------------------------------------------//
//--------------------------------------------------------------//
//--------------------------------------------------------------//
void CPatchServerDlg::HandleMessagesClientSide(LPVOID lpMessage,DPNID idFromLoc,int msgSize)
{
     LPEXMSG_CATAGORIE identityMsg = (LPEXMSG_CATAGORIE)lpMessage;

     if(identityMsg->m_catagorie == 1)
	 {//disconnect from current server and get the next one
	    m_updateServerAttemptingConnection = FALSE;
	    m_updateServerCurrentlyConnected   = FALSE;
		m_connectionRequested              = FALSE;
		m_enabledClient                    = FALSE;
		m_updateServerSentData             = FALSE;
	    if(m_clientInterface)
		{
		   m_clientInterface->Close(0);
           m_clientInterface->Release();
           m_clientInterface = 0;
		}
	
		m_serverStatus = "Disconnecting....";


		return;
	 }//disconnect from current server and get the next one
	 
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::HandleMessagesServerSide(LPVOID lpMessage,DPNID idFromLoc,int msgSize)
{
    
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
BOOL CPatchServerDlg::SendStatus(DPNID sendTo,
								 int miscInt,
								 int statusType,
								 long patchSize)
{
    EXMSG_STATUS status;
    int messageSize = sizeof(EXMSG_STATUS);
	status.m_catagorie  = 8;
	status.m_statusType = statusType;
	status.m_miscInt    = miscInt;
	status.m_patchSize  = patchSize;

	SendData((BYTE *)&status,
		             messageSize,
		             DPNSEND_GUARANTEED,
		             sendTo);  


	return TRUE;
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
BOOL CPatchServerDlg::SendPatchSectionReq(DPNID sendTo,
								          int version,
								          long currentPosition)
{
    EXMSG_PATCHSECTIONREQ patchSectionReq;
    int messageSize = sizeof(EXMSG_PATCHSECTIONREQ);
	patchSectionReq.m_catagorie         = 2;
	patchSectionReq.m_version           = version;
	patchSectionReq.m_currentPosition   = currentPosition;

	SendData((BYTE *)&patchSectionReq,
		             messageSize,
		             DPNSEND_GUARANTEED,
		             sendTo);  

	return TRUE;
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::Cycle()
{

    //GetNewAccountsFromWeb(m_getSignupsURL,m_registrationURL);
    /*system( "eacrypt.exe" );

	CString curFile = GetFirstFileNameInDirectory("c:\\unsecure\\");
    if(curFile != "EMPTY")
	{
   	   GetDataFromFile(curFile);
	   ArchiveDataFile(curFile);//store for backup purposes
	}*/



    GetDataFromSQL();

	if(!m_updateServersAccountsInProgress)
	if(m_accountsToBeEntered){
	   if(m_accountsToBeEntered->GetCount() > 0 ||
		  m_accountsToPlaceOnHold->GetCount() > 0 ||
		  m_accountsToTakeOffHold->GetCount() > 0 ||
		  m_accountsInfoChanges->GetCount() > 0 || 
		  m_accountsToBeDeleted->GetCount() > 0 || 
		  m_expChangesToBeEntered->GetCount() > 0)
	   {
		  //ConnectToServer("LAN"); 
		  m_updateServersAccountsInProgress   = TRUE;  
          m_updateServersCurrentIndex         = 0;
          m_updateServerCurrentlyConnected    = FALSE; 
		  m_updateServerAttemptingConnection  = FALSE; 
		  m_updateServerUpdateingPerfect      = TRUE;
	   }
	}

}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::OnBUTTONtestcode() 
{
    Cycle();	
	
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::GetNewAccountsFromWeb(CString url,CString registerUrl)
{
	 UpdateData(TRUE);
     m_serverStatus = "HTTP request....";
	 Sleep(200);
	 UpdateData(FALSE);
	 
CString strServerName;
     INTERNET_PORT nPort;
	 CString strObject;
     DWORD dwServiceType;
	 LPCTSTR pszURL = NULL;
	 const TCHAR szHeaders[] = _T("Accept: text/*\r\nUser-Agent: MFC\r\n");
	 DWORD dwHttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT | INTERNET_FLAG_RELOAD;

	 CString buildUrl = url;
	 pszURL = (LPCTSTR)buildUrl;
	 
	 CInternetSession * iSession = new CInternetSession();

     if (!AfxParseURL(pszURL, dwServiceType, strServerName, strObject, nPort) ||
		  dwServiceType != INTERNET_SERVICE_HTTP)
	 {
		  MessageBox("Cant open this URL");
	 }

     CHttpConnection* pServer = NULL;
	 CHttpFile* pFile = NULL;

	 BOOL successful = FALSE;
	 UpdateData(TRUE);
     m_serverStatus = "attempting server connection....";
	 Sleep(200);
	 UpdateData(FALSE);
     pServer = iSession->GetHttpConnection(strServerName, nPort);
     if(pServer)
	 {//valid http connection
		UpdateData(TRUE);
		m_serverStatus = "open request....";
		Sleep(200);
		UpdateData(FALSE);
	    pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET, strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
	    if(pFile)
		{//valid file
           
	       pFile->AddRequestHeaders(szHeaders);
		   UpdateData(TRUE);
		   m_serverStatus = "send request....";
		   Sleep(200);
		   UpdateData(FALSE);
	       if(pFile->SendRequest())
		   {//request
		    UpdateData(TRUE);
		    m_serverStatus = "send request suceeded....";
			Sleep(200);
			UpdateData(FALSE);
	       DWORD dwRet;
		   pFile->QueryInfoStatusCode(dwRet);

			if (dwRet != HTTP_STATUS_OK)
			{
				UpdateData(TRUE);
				m_serverStatus = "status failed....";
				Sleep(200);
				UpdateData(FALSE);
				//AfxMessageBox("Url Failed");
				iSession->Close();
	            delete iSession;
	            iSession=0;
				
				return;
			}

	       TCHAR sz[1024];
		   ZeroMemory(&sz,1024);
		   CString build;

           CString username;
		   CString password;
		   CString email;
		   int index = -2;

		   while (pFile->ReadString(sz, 1023))
		   {

			CString test = sz;
			test.TrimRight();
			if(test.GetLength() == 0)
			{
               index++;
               continue;
			}
			
			
			  if(index == 0){
				 username = test;
				 index++;
				 continue;
			  }

			  if(index == 1){
		
				 password = test;
				 index++;
				 continue;
			  }

			  if(index == 2)
			  {
				 
				 email = test;
                 
				 CAccountSQLObj * acPtr = new CAccountSQLObj();

                 acPtr->m_username = username;
				 acPtr->m_password = password;
				 acPtr->m_email = email;
					 
			     m_accountsToBeEntered->AddTail(acPtr);
				 index = 0;
			  }
		      
			  ZeroMemory(&sz,1024);
			  successful = TRUE;
		   }
		   }//end request
	    pFile->Close();
		}//end valid file
        pServer->Close();
	 }//end valid http connection


     iSession->Close();
	 delete iSession;
	 iSession=0;
	 
     if(successful)
	 {//successful log data
	   UpdateData(TRUE);
	   m_serverStatus = "HTTP registration....";
	   UpdateData(FALSE);
	   RegisterAccountsFromWeb(registerUrl);
	   UpdateData(TRUE);
	   m_serverStatus = "HTTP registration completed....";
	   UpdateData(FALSE);
	 }//end successful log data
	 UpdateData(TRUE);
	 m_serverStatus = "cycle completed....";
	 UpdateData(FALSE);
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::RegisterAccountsFromWeb(CString url)
{


CString strServerName;
     INTERNET_PORT nPort;
	 CString strObject;
     DWORD dwServiceType;
	 LPCTSTR pszURL = NULL;
	 const TCHAR szHeaders[] = _T("Accept: text/*\r\nUser-Agent: MFC\r\n");
	 DWORD dwHttpRequestFlags = INTERNET_FLAG_EXISTING_CONNECT | INTERNET_FLAG_NO_AUTO_REDIRECT | INTERNET_FLAG_RELOAD;

	 CString buildUrl = url;
	 pszURL = (LPCTSTR)buildUrl;
	 
	 CInternetSession * iSession = new CInternetSession();

     if (!AfxParseURL(pszURL, dwServiceType, strServerName, strObject, nPort) ||
		  dwServiceType != INTERNET_SERVICE_HTTP)
	 {
		  //MessageBox("Cant open this URL");
	 }

     CHttpConnection* pServer = NULL;
	 CHttpFile* pFile = NULL;

     pServer = iSession->GetHttpConnection(strServerName, nPort);
     if(pServer)
	 {//valid server
	    pFile = pServer->OpenRequest(CHttpConnection::HTTP_VERB_GET,
			strObject, NULL, 1, NULL, NULL, dwHttpRequestFlags);
	    if(pFile)
		{//valid file
	       pFile->AddRequestHeaders(szHeaders);
	       if(pFile->SendRequest())
		   {//request

	        DWORD dwRet;
		    pFile->QueryInfoStatusCode(dwRet);

			if (dwRet != HTTP_STATUS_OK)
			{
				//AfxMessageBox("Url Failed");
			}
		   }//end request
           pFile->Close();
        }//end valid file
		pServer->Close();
	 }//valid server
     iSession->Close();
	 delete iSession;
	 iSession=0;
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::OnBUTTONAdd() 
{
   UpdateData(TRUE);

     CServerListObject * svrPtr = new CServerListObject();
     svrPtr->m_IPadress = m_ipadress;
     svrPtr->m_serverName = m_servername;
     m_targetServersList->AddTail(svrPtr);

   UpdateData(FALSE);
   InList(m_targetServersList->GetCount()-1);
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::InList(int sel)
{
   m_targetServerDB.ResetContent();
   if(!m_targetServersList)
	   return;

      for(POSITION posLoc = m_targetServersList->GetHeadPosition();posLoc != NULL;)
	  {
          CServerListObject * svrPtr = (CServerListObject *)m_targetServersList->GetNext(posLoc);
           
          CString build = operator+(svrPtr->m_serverName,":");
		  build = operator+(build,svrPtr->m_IPadress);

	      m_targetServerDB.AddString(build);
	  }
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::OnBUTTONDelete() 
{
	int select = m_targetServerDB.GetCurSel();
	if(select == -1)
	{
       AfxMessageBox("must select a server");
       return;
	}
 
	POSITION posLoc = m_targetServersList->FindIndex(select);
	if(!posLoc)
        return;

	    CServerListObject * svrPtr = (CServerListObject *)m_targetServersList->GetAt(posLoc);
        delete svrPtr;
		svrPtr = 0;
        m_targetServersList->RemoveAt(posLoc);

        InList(select-1);

}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::OnBUTTONLoad() 
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization

   static char BASED_CODE filter[] = "Server List|*.TSV|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
	   SetCurrentDirectory(MainDir);
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	   if(res == FALSE){
		  SetCurrentDirectory(MainDir);
		  return;
	   }
	   CArchive the_in_Archive(&fl,CArchive::load);
	   m_targetServersList->SafeDelete();
	   delete m_targetServersList;
	   m_targetServersList = 0;
	   UpdateData(TRUE);
	   the_in_Archive >> m_targetServersList >> m_cycleDuration >> m_registrationURL >> m_getSignupsURL;
	   UpdateData(FALSE);
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InList(0);
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::OnBUTTONSave() 
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
     static char BASED_CODE filter[] = "Server List|*.TSV|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "TSV", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetFileName();
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
       if(res == FALSE){
		  SetCurrentDirectory(MainDir);
	      return;
	   }
       CArchive the_out_Archive(&fl,CArchive::store);
       UpdateData(TRUE);
	   the_out_Archive << m_targetServersList << m_cycleDuration << m_registrationURL << m_getSignupsURL;
	   UpdateData(FALSE);
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

void CPatchServerDlg::OnBUTTONsaveHistory() 
{
	// TODO: Add your control notification handler code here
	
}

//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::SaveHistoryDB(CString fileName) 
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
     //end use gamefiles folder for organization
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
       if(res == FALSE){
		  SetCurrentDirectory(MainDir);
	      return;
	   }
       CArchive the_out_Archive(&fl,CArchive::store);
       the_out_Archive << m_accountadditionHistory;
       the_out_Archive.Close();
       fl.Close();
	 SetCurrentDirectory(MainDir);	
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::OnBUTTONloadHistory() 
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization

   static char BASED_CODE filter[] = "History db|*.HST|All Files (*.*)|*.*||";
	 CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
	   SetCurrentDirectory(MainDir);
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	   if(res == FALSE){
		  SetCurrentDirectory(MainDir);
		  return;
	   }
	   CArchive the_in_Archive(&fl,CArchive::load);
	   m_accountadditionHistory->SafeDelete();
	   delete m_accountadditionHistory;
	   m_accountadditionHistory = 0;
	   the_in_Archive >> m_accountadditionHistory;
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InListHistory(0);
}
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::InListHistory(int sel)
{
   m_history.ResetContent();
   if(!m_targetServersList)
	   return;

      for(POSITION posLoc = m_accountadditionHistory->GetHeadPosition();posLoc != NULL;)
	  {
          CAccountSQLObj * acPtr = (CAccountSQLObj *)m_accountadditionHistory->GetNext(posLoc);
	      m_history.AddString(acPtr->m_username);
	  }

     if(sel > -1)
		m_history.SetCurSel(sel);

}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
BOOL CPatchServerDlg::SendLogFast(CString userName,
								  CString password) 
{
    LPLOG_STRUCT lpChatMessage = NULL;
	DWORD				messageSize;
	messageSize = sizeof(LOG_STRUCT);
    lpChatMessage = (LPLOG_STRUCT) new BYTE[messageSize];
	if (lpChatMessage == NULL)
	{
		return FALSE;
	}
	lpChatMessage->m_catagorie = 1;
	lstrcpy(lpChatMessage->logInName, userName);
	lstrcpy(lpChatMessage->logInPass, password);

	SendData((BYTE *)lpChatMessage,
				     messageSize,
				     DPNSEND_GUARANTEED,
					 NULL);

	if (lpChatMessage)
		delete lpChatMessage;
	    lpChatMessage=0;

	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
BOOL CPatchServerDlg::GetDataFromFile(CString fileName) 
{
	//use gamefiles folder for organization
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\Unsecure\\");
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization 	 
    CFileException exc;
     CFile fl;
     BOOL res = fl.Open(fileName,CFile::modeRead, &exc);
	 if(res == FALSE){
		SetCurrentDirectory(MainDir);
		return FALSE;
	 }
	 CArchive the_in_Archive(&fl,CArchive::load);

	 //m_accountsInfoChanges               = new CAccountSQLDatabase();
 	 //m_accountsToPlaceOnHold             = new CAccountSQLDatabase();
	 //m_accountsToTakeOffHold             = new CAccountSQLDatabase();
	 //m_accountsToBeEntered               = new CAccountSQLDatabase();
	 
	 CString bufferTemp;
	 CString username,password,newUsername,newPassword;
     while(the_in_Archive.ReadString(bufferTemp))
	 {//file data loop
		 if(bufferTemp == "NW")
		 {//NEW ACCOUNT
            the_in_Archive.ReadString(username);
            the_in_Archive.ReadString(password);
            CAccountSQLObj * acPtr = new CAccountSQLObj();
            acPtr->m_username = username;
		    acPtr->m_password = password;	 
		    m_accountsToBeEntered->AddTail(acPtr);
		 }//END NEW ACCOUNT
         if(bufferTemp == "EN")
		 {//take off hold
            the_in_Archive.ReadString(username);
            the_in_Archive.ReadString(password);
            CAccountSQLObj * acPtr = new CAccountSQLObj();
            acPtr->m_username = username;
		    acPtr->m_password = password;	 
		    m_accountsToTakeOffHold->AddTail(acPtr);
		 }//END take off hold
		 if(bufferTemp == "DS")
		 {//place on hold
            the_in_Archive.ReadString(username);
            the_in_Archive.ReadString(password);
            CAccountSQLObj * acPtr = new CAccountSQLObj();
            acPtr->m_username = username;
		    acPtr->m_password = password;	 
		    m_accountsToPlaceOnHold->AddTail(acPtr);
		 }//END place on hold
         if(bufferTemp == "CA")
		 {//account pass change
            the_in_Archive.ReadString(username);
            the_in_Archive.ReadString(password);
			the_in_Archive.ReadString(newUsername);
            the_in_Archive.ReadString(newPassword);
            CAccountSQLObj * acPtr = new CAccountSQLObj();
            acPtr->m_username = username;
		    acPtr->m_password = password;	
			acPtr->m_newUsername = newUsername;
			acPtr->m_newPassword = newPassword;
		    m_accountsInfoChanges->AddTail(acPtr);
		 }//END account pass change
		 if(bufferTemp == "EX")
		 {//EXPANSION
            the_in_Archive.ReadString(username);
            the_in_Archive.ReadString(password);
            CAccountSQLObj * acPtr = new CAccountSQLObj();
            acPtr->m_username = username;
		    acPtr->m_password = password;	 
		    m_expChangesToBeEntered->AddTail(acPtr);
		 }//END EXPANSION
     }//end file data loop end
	 the_in_Archive.Close();
     fl.Close();

     SetCurrentDirectory(MainDir);

	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
/*CString CPatchServerDlg::ExtractNextFromSQL()
{



}*/
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
BOOL CPatchServerDlg::RefreshAllAccountStatus() 
{
   UpdateData(TRUE);	
		      m_serverStatus = "Checking database....";
   UpdateData(FALSE);
	 _RecordsetPtr  pRst;
	 _bstr_t strCnn("Provider=sqloledb;Data Source="+m_sqlIp+";Initial Catalog="+m_sqlDB+";User Id="+m_sqlUserid+";Password="+m_sqlPass+";");
     pRst.CreateInstance(__uuidof(Recordset));

	 _RecordsetPtr  pRstUsers;
     pRstUsers.CreateInstance(__uuidof(Recordset));


     BOOL enabled = TRUE;
     try
	 {
	    //execute connection method
        pRst->Open(
            "tblUsers",//table
             strCnn,
             adOpenStatic,
             adLockOptimistic,
             adCmdTable);
	 }
     catch(_com_error &e)
	 {
	        enabled = FALSE;
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
            //AfxMessageBox(bstrDescription);
			UpdateData(TRUE);	
		      m_serverStatus = (LPSTR)bstrDescription;
	        UpdateData(FALSE);
	 }
	 if(enabled)
	 {
	  UpdateData(TRUE);	
		      m_serverStatus = "Sql connected....";
	  UpdateData(FALSE);
	  _bstr_t bufferSql;
	  CString username,password,active;
      int bstrRecordCount = pRst->RecordCount; 
	  int i = 0;
	  if(bstrRecordCount <= 0)
		 return FALSE;

	   for(int count=0;count<bstrRecordCount;count++)
	   {//file data loop

		 bufferSql = pRst->Fields->GetItem("UserName")->Value;
         username = (LPSTR)bufferSql;
		 
         bufferSql = pRst->Fields->GetItem("Password")->Value;
         password = (LPSTR)bufferSql;

         bufferSql = pRst->Fields->GetItem("Active")->Value;
         active = (LPSTR)bufferSql;

		 if(active == "0")//Disable account
		 {
            CAccountSQLObj * acPtr = new CAccountSQLObj();
            acPtr->m_username = username;
		    acPtr->m_password = password;	 
		    m_accountsToPlaceOnHold->AddTail(acPtr);
		 }//END Disable account
		 else{//NEW account
			CAccountSQLObj * acPtr = new CAccountSQLObj();
            acPtr->m_username = username;
		    acPtr->m_password = password;	 
		    m_accountsToBeEntered->AddTail(acPtr);
		 }//end new account
 
		 if(count<bstrRecordCount)
         pRst->MoveNext();
	   }//end file data loop end
	   pRst->Close();
	   UpdateData(TRUE);	
		      m_serverStatus = "Database querry completed....";
       UpdateData(FALSE);
	   return TRUE;
	 }
	 
	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
BOOL CPatchServerDlg::GetDataFromSQL() 
{
   UpdateData(TRUE);	
		      m_serverStatus = "Checkin database....";
   UpdateData(FALSE);
	 _RecordsetPtr  pRst;
	 _bstr_t strCnn("Provider=sqloledb;Data Source="+m_sqlIp+";Initial Catalog="+m_sqlDB+";User Id="+m_sqlUserid+";Password="+m_sqlPass+";");
     pRst.CreateInstance(__uuidof(Recordset));
     BOOL enabled = TRUE;
     try
	 {
	    //execute connection method
        pRst->Open(
            "RegCommands",//table
             strCnn,
             adOpenStatic,
             adLockOptimistic,
             adCmdTable);
	 }
     catch(_com_error &e)
	 {
	        enabled = FALSE;
            _bstr_t bstrSource(e.Source());
            _bstr_t bstrDescription(e.Description());
            //AfxMessageBox(bstrDescription);
			UpdateData(TRUE);	
		      m_serverStatus = (LPSTR)bstrDescription;
	        UpdateData(FALSE);
	 }
	 if(enabled)
	 {
	  UpdateData(TRUE);	
		      m_serverStatus = "Sql connected....";
	  UpdateData(FALSE);
	  CString command;
	  _bstr_t bufferSql;
	  CString username,password,newUsername,newPassword;
      int bstrRecordCount = pRst->RecordCount; 
	  int i = 0;
	  if(bstrRecordCount <= 0)
		 return FALSE;

	   while(1)
	   {//file data loop
		   
         BOOL exitWhileLoop = FALSE;

		 bufferSql = pRst->Fields->GetItem("Command")->Value;
         command = (LPSTR)bufferSql;
         if(pRst->Fields->GetItem("Perameter1")->ActualSize > 0)
         bufferSql = pRst->Fields->GetItem("Perameter1")->Value;
		 username = (LPSTR)bufferSql;
         if(pRst->Fields->GetItem("Perameter2")->ActualSize > 0)
		 bufferSql = pRst->Fields->GetItem("Perameter2")->Value;
		 password = (LPSTR)bufferSql;
         if(pRst->Fields->GetItem("Perameter3")->ActualSize > 0)
		 bufferSql = pRst->Fields->GetItem("Perameter3")->Value;
		 newUsername = (LPSTR)bufferSql;
         if(pRst->Fields->GetItem("Perameter4")->ActualSize > 0)
		 bufferSql = pRst->Fields->GetItem("Perameter4")->Value;
		 newPassword = (LPSTR)bufferSql;
		 pRst->Delete(adAffectCurrent);
		 i++;
		 if(i<(bstrRecordCount))
		    pRst->MoveNext();
		 else
			exitWhileLoop = TRUE;

		 
		 if(command == "NW")
		 {//NEW ACCOUNT
            CAccountSQLObj * acPtr = new CAccountSQLObj();
            acPtr->m_username = username;
		    acPtr->m_password = password;	 
		    m_accountsToBeEntered->AddTail(acPtr);
		 }//END NEW ACCOUNT
         if(command == "EN")
		 {//take off hold
            CAccountSQLObj * acPtr = new CAccountSQLObj();
            acPtr->m_username = username;
		    acPtr->m_password = password;	 
		    m_accountsToTakeOffHold->AddTail(acPtr);
		 }//END take off hold
		 if(command == "DS")
		 {//place on hold
            CAccountSQLObj * acPtr = new CAccountSQLObj();
            acPtr->m_username = username;
		    acPtr->m_password = password;	 
		    m_accountsToPlaceOnHold->AddTail(acPtr);
		 }//END place on hold
         if(command == "CA")
		 {//account pass change
            CAccountSQLObj * acPtr = new CAccountSQLObj();
            acPtr->m_username = username;
		    acPtr->m_password = password;	
			acPtr->m_newUsername = newUsername;
			acPtr->m_newPassword = newPassword;
		    m_accountsInfoChanges->AddTail(acPtr);
		 }//END account pass change
         /*if(command == "XD")
		 {//account delete
            CAccountSQLObj * acPtr = new CAccountSQLObj();
            acPtr->m_username = username;
		    acPtr->m_password = password;	
		    m_accountsToBeDeleted->AddTail(acPtr);
		 }//END account delete*/
         if(command == "EX")
		 {//account delete
            CAccountSQLObj * acPtr = new CAccountSQLObj();
            acPtr->m_username = username;
		    acPtr->m_password = password;	
		    m_expChangesToBeEntered->AddTail(acPtr);
		 }//END account delete


	     if(exitWhileLoop == TRUE || i > 500)
			break;
        
        }//end file data loop end
	   pRst->Close();
	   UpdateData(TRUE);	
		      m_serverStatus = "Database querry completed....";
       UpdateData(FALSE);
	   return TRUE;
	 }
	 
	return TRUE;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
BOOL CPatchServerDlg::ArchiveDataFile(CString fileName) 
{
	CString CurDir,DestDir;
	char buffer[_MAX_PATH];
	_getcwd( buffer, _MAX_PATH );
    CurDir = buffer;
    CurDir = operator+(CurDir,"\\Unsecure\\");
    DestDir = buffer;
    DestDir = operator+(DestDir,"\\ArchiveHistory\\");
	CString currentFile = operator+(CurDir,fileName);
	CTime t = CTime::GetCurrentTime();
	CString destinationFile = operator+(DestDir,fileName);
	CString s = t.Format( "%A, %H %M, %S" );
    destinationFile = operator+(destinationFile,s);
    BOOL res = MoveFileEx(currentFile,  // file name
                          destinationFile,       // new file name
                          MOVEFILE_COPY_ALLOWED);



return res;
}
//-------------------------End Function-----------------------------------------------//
//------------------------------------------------------------------------------------//
//--------------send message----------------------------------------------------------//
CString CPatchServerDlg::GetFirstFileNameInDirectory(CString dir) 
{
    CString CurDir;
    CString MainDir;
      char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"Unsecure\\");
			 SetCurrentDirectory(CurDir);
    //end use gamefiles folder for organization 	 

   WIN32_FIND_DATA FindFileData;
   HANDLE hFind;


   CString filename = "EMPTY";

    hFind = FindFirstFile("sectrans*.*", &FindFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
       
    } 
    else {
      filename = FindFileData.cFileName;
      FindClose(hFind);
	}
   

  SetCurrentDirectory(MainDir);
return filename;
}

void CPatchServerDlg::OnBUTTONRebuildAccounts() 
{
    if(AfxMessageBox("Are you sure-This will take time?",MB_OKCANCEL,0)!=IDOK)
       return;
	RefreshAllAccountStatus(); 
}

long CPatchServerDlg::GetVersionFromFile()//ussually client side
{
    CFileException exc;
    CFile fl;
    BOOL res = fl.Open("VERSIONINF.VER",CFile::modeRead, &exc);
	 if(res == FALSE){
	    return 1;
	 }
	 else
	 fl.Close();

    //end verify files existance	
	CFile * file = new CFile( "VERSIONINF.VER", CFile::modeReadWrite );
    BYTE * fileRawData = NULL;
    DWORD fileLengthInBytes = 0;
	fileLengthInBytes = file->GetLength();
	if(fileLengthInBytes != 0)
	{//BUILD old file byte array
       
	   fileRawData = new BYTE[fileLengthInBytes];
	   file->Read(fileRawData,fileLengthInBytes);
	   file->Close();

	   long version = 0;
       CopyMemory(&version,fileRawData,sizeof(long));
	   return version;
	}//END BUILD old file byte array
    
	
    return 1;
}

//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
//-----------------------------------------------------------------//
void CPatchServerDlg::WriteDebugFile(CString logMsg,BOOL clearIt)
{

	LPSTR debugString = logMsg.GetBuffer();
    int oldFile = 0;
	if(clearIt)	
	   oldFile = open( "DeviceLog.txt", _O_WRONLY | _O_TRUNC  | _O_CREAT  |_O_BINARY , _S_IWRITE);  
	else
	{
	   oldFile = open( "DeviceLog.txt", _O_WRONLY  | _O_CREAT  |_O_BINARY , _S_IWRITE);  
	   _lseek( oldFile, 0L, SEEK_END );


	}
	
	int trace = 0;
    while(debugString[trace++]!= 0)
	{
        //trace++;
	}
	long bytesRead = _write( oldFile, debugString, trace );
	LPSTR returnCase = "\r\n";
    _write( oldFile, returnCase, 2 );
 
	_close( oldFile );
}
