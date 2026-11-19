// AdvancedNetworkDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "AdvancedNetworkDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdvancedNetworkDlg dialog


CAdvancedNetworkDlg::CAdvancedNetworkDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAdvancedNetworkDlg::IDD, pParent)
	, m_murderWorkOffRate(0)
	, m_levelRestriction(0)
	, m_maxPacketIntake(0)
	, m_maxPacketOutput(0)
	, m_houseVisRangeValue(0)
{
	//{{AFX_DATA_INIT(CAdvancedNetworkDlg)
	m_calcEntityRadius = 0.0f;
	m_calcItemRadius = 0.0f;
	m_maxLoggedOnPlayers = 0;
	m_availableBandwidth = 0;
	m_frequencyAdjustBuffer = 0;
	m_frequencyIncrement = 0;
	m_frequencyLimit = 0;
	m_serverKey = _T("");
	m_serverName = _T("");
	m_deadEntityDissolveTime = 0;
	m_aiServerCycle = 0;
	m_deadEntityNPCDissolveTime = 0;
	m_autoBackupFile = _T("");
	m_durationBetweenBackups = 0;
	m_versionNumber = 0;
	m_murdererLevel = 0;
	m_sqlIP = _T("");
	m_sqlPassword = _T("");
	m_sqlUsername = _T("");
	m_sqlTableName = _T("");
	m_sqlCatalogDB = _T("");
	//}}AFX_DATA_INIT
}


void CAdvancedNetworkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdvancedNetworkDlg)
	DDX_Control(pDX, IDC_CHECK_enableAutobackup, m_enableAutoBackup);
	DDX_Control(pDX, IDC_CHECK_AISERVER, m_aiSever);
	DDX_Control(pDX, IDC_CHECK_RequirePassword, m_checkRequirePassword);
	DDX_Control(pDX, IDC_CHECK_PersistentServer, m_persistentServer);
	DDX_Control(pDX, IDC_CHECK_NoRenderDedicated, m_noRenderDedicated);
	DDX_Control(pDX, IDC_CHECK_ClientSideFireing, m_clientSideFireing);
	DDX_Control(pDX, IDC_CHECK_RecordNetworkdata, m_recordNetworkData);
	DDX_Text(pDX, IDC_EDIT_CalculateEntityRadius, m_calcEntityRadius);
	DDX_Text(pDX, IDC_EDIT_CalculateItemRadius, m_calcItemRadius);
	DDX_Text(pDX, IDC_EDIT_MaxLoggedOnPlayers, m_maxLoggedOnPlayers);
	DDX_Text(pDX, IDC_EDIT_AvailableBandwidth, m_availableBandwidth);
	DDX_Text(pDX, IDC_EDIT_FrequencyAdjustBuffer, m_frequencyAdjustBuffer);
	DDX_Text(pDX, IDC_EDIT_FrequencyIncrement, m_frequencyIncrement);
	DDX_Text(pDX, IDC_EDIT_FrequencyLimit, m_frequencyLimit);
	DDX_Text(pDX, IDC_EDIT_SERVERKEY, m_serverKey);
	DDX_Text(pDX, IDC_EDIT_Net_ServerName, m_serverName);
	DDX_Text(pDX, IDC_EDIT_DeadEntityDissolveTime, m_deadEntityDissolveTime);
	DDX_Text(pDX, IDC_EDIT_AIserverCycle, m_aiServerCycle);
	DDX_Text(pDX, IDC_EDIT_DeadEntityNPCDissolveTime, m_deadEntityNPCDissolveTime);
	DDX_Text(pDX, IDC_EDIT_AutoBackupFile, m_autoBackupFile);
	DDX_Text(pDX, IDC_EDIT_durationBetweenBackups, m_durationBetweenBackups);
	DDX_Text(pDX, IDC_EDIT_VersionNumber, m_versionNumber);
	DDX_Text(pDX, IDC_EDIT_murderLevel, m_murdererLevel);
	DDX_Text(pDX, IDC_EDIT_sqlIP, m_sqlIP);
	DDX_Text(pDX, IDC_EDIT_sqlPassword, m_sqlPassword);
	DDX_Text(pDX, IDC_EDIT_sqlUsername, m_sqlUsername);
	DDX_Text(pDX, IDC_EDIT_tableName, m_sqlTableName);
	DDX_Text(pDX, IDC_EDIT_CatalogDatabase, m_sqlCatalogDB);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_murderWorkOffRate, m_murderWorkOffRate);
	DDX_Text(pDX, IDC_EDIT_levelRestriction, m_levelRestriction);
	DDX_Text(pDX, IDC_EDIT_MaxPacketIntake, m_maxPacketIntake);
	DDX_Text(pDX, IDC_EDIT_MaxPacketOutput, m_maxPacketOutput);
	DDX_Control(pDX, IDC_EDIT_houseVisRange, m_houseVisRange);
	DDX_Text(pDX, IDC_EDIT_houseVisRange, m_houseVisRangeValue);
}


BEGIN_MESSAGE_MAP(CAdvancedNetworkDlg, CDialog)
	//{{AFX_MSG_MAP(CAdvancedNetworkDlg)
	ON_BN_CLICKED(IDC_CHECK_RecordNetworkdata, OnCHECKRecordNetworkdata)
	ON_BN_CLICKED(IDC_CHECK_ClientSideFireing, OnCHECKClientSideFireing)
	ON_BN_CLICKED(IDC_CHECK_NoRenderDedicated, OnCHECKNoRenderDedicated)
	ON_BN_CLICKED(IDC_CHECK_PersistentServer, OnCHECKPersistentServer)
	ON_BN_CLICKED(IDC_CHECK_RequirePassword, OnCHECKRequirePassword)
	ON_BN_CLICKED(IDC_CHECK_AISERVER, OnCheckAiserver)
	ON_BN_CLICKED(IDC_BUTTON_BrowseAutoBackupFile, OnBUTTONBrowseAutoBackupFile)
	ON_BN_CLICKED(IDC_CHECK_enableAutobackup, OnCHECKenableAutobackup)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAdvancedNetworkDlg message handlers

BOOL CAdvancedNetworkDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	if(RecordNetworkData == TRUE)
       m_recordNetworkData.SetCheck(1);
	else
	   m_recordNetworkData.SetCheck(0);

	if(ClientSideFireing == TRUE)
       m_clientSideFireing.SetCheck(1);
	else
	   m_clientSideFireing.SetCheck(0);
 
    if(NoRenderDedicated == FALSE)
       m_noRenderDedicated.SetCheck(1);
	else
	   m_noRenderDedicated.SetCheck(0);

	if(persistentServerType == TRUE)
       m_persistentServer.SetCheck(1);

	if(aiServer)
	   m_aiSever.SetCheck(1);

	if(enableAutobackup)
	   m_enableAutoBackup.SetCheck(1);

	if(requiresPassword == TRUE)
       m_checkRequirePassword.SetCheck(1);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAdvancedNetworkDlg::OnCHECKNoRenderDedicated() 
{
    if(NoRenderDedicated == TRUE)
       NoRenderDedicated = FALSE;
	else
	   NoRenderDedicated = TRUE;
}

void CAdvancedNetworkDlg::OnCHECKRecordNetworkdata() 
{
	if(RecordNetworkData == TRUE)
       RecordNetworkData = FALSE;
	else
	   RecordNetworkData = TRUE;
}

void CAdvancedNetworkDlg::OnCHECKClientSideFireing() 
{
	if(ClientSideFireing == TRUE)
       ClientSideFireing = FALSE;
	else
	   ClientSideFireing = TRUE;
}



void CAdvancedNetworkDlg::OnCHECKPersistentServer() 
{
    if(persistentServerType == TRUE)
       persistentServerType = FALSE;
	else
	   persistentServerType = TRUE;
}

void CAdvancedNetworkDlg::OnCHECKRequirePassword() 
{
	if(requiresPassword == TRUE)
       requiresPassword = FALSE;
	else
	   requiresPassword = TRUE;
}

void CAdvancedNetworkDlg::OnCheckAiserver() 
{
	if(aiServer)
	   aiServer = FALSE;
	else
	   aiServer = TRUE;
	
}

void CAdvancedNetworkDlg::OnBUTTONBrowseAutoBackupFile() 
{
	//use mapsmodels folder for organization
           CString CurDir;
           CString MainDir;
           char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\GameFiles\\");
			 SetCurrentDirectory(CurDir);
          //end use mapsmodels folder for organization
          static char BASED_CODE filter[] = "SVR Files|*.SVR|All Files (*.*)|*.*||";
	      CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
          if ( opendialog.DoModal() == IDOK )
		  {
			 UpdateData(TRUE);
	         m_autoBackupFile = opendialog.GetFileName();
			 UpdateData(FALSE);
             SetCurrentDirectory(MainDir);	
		  }  
		  SetCurrentDirectory(MainDir);
}

void CAdvancedNetworkDlg::OnCHECKenableAutobackup() 
{
	
    if(enableAutobackup)
	   enableAutobackup = FALSE;
	else
	   enableAutobackup = TRUE;	
}

void CAdvancedNetworkDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
