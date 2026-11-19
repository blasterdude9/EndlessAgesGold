// AccountViewerModule.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include <d3d9.h>
#include "AccountViewerModule.h"
#include "direct.h"
#include ".\accountviewermodule.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAccountViewerModule dialog


CAccountViewerModule::CAccountViewerModule(CWnd* pParent /*=NULL*/)
	: CDialog(CAccountViewerModule::IDD, pParent)
	, m_lastLoggedOn(_T(""))
{
	//{{AFX_DATA_INIT(CAccountViewerModule)
	m_passwordChange = _T("");
	m_passwordChangeVerify = _T("");
	m_murders = 0;
	m_currentHealth = 0;
	m_accountName = _T("");
	m_minutesPlayed = _T("");
	m_onPersonCash = 0;
	m_bankCash = 0;
	m_wldSpcaceX = 0.0f;
	m_wldSpaceY = 0.0f;
	m_wldSpaceZ = 0.0f;
	m_userNameEdit = _T("");
	m_statValueEdit = 0.0f;
	m_suspentionNotes = _T("");
	m_staticHandle = 0;
	m_setWldID = 0;
	m_skillCurExp = 0.0f;
	m_houseBind = 0;
	m_accountNumber = 0;
	//}}AFX_DATA_INIT
}


void CAccountViewerModule::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAccountViewerModule)
	DDX_Control(pDX, IDC_CHECK_CanSpawn, m_canSpawn);
	DDX_Control(pDX, IDC_CHECK_GMType, m_gmType);
	DDX_Control(pDX, IDC_CHECK_adminPrivilidge, m_adminPrivlidge);
	DDX_Control(pDX, IDC_CHECK_suspendAccount, m_suspenAccountCheck);
	DDX_Control(pDX, IDC_LIST_STATS, m_statistics);
	DDX_Control(pDX, IDC_LIST_GlobalInventory, m_globalInventory);
	DDX_Control(pDX, IDC_LIST_skills, m_skillsDB);
	DDX_Control(pDX, IDC_CHECK_enableAISERVER, m_enableAIserver);
	DDX_Control(pDX, IDC_LIST_charInventory, m_charCurrentInventory);
	DDX_Control(pDX, IDC_LIST_charBankInventory, m_characterBankInventory);
	DDX_Control(pDX, IDC_LIST_CharactersOnAccount, m_charactersOnAccount);
	DDX_Text(pDX, IDC_EDIT_passwordChange, m_passwordChange);
	DDX_Text(pDX, IDC_EDIT_passwordChangeVerify, m_passwordChangeVerify);
	DDX_Text(pDX, IDC_EDIT_murders, m_murders);
	DDX_Text(pDX, IDC_EDIT_CurrentHeath, m_currentHealth);
	DDX_Text(pDX, IDC_STATIC_accountName, m_accountName);
	DDX_Text(pDX, IDC_STATIC_minutesPlayed, m_minutesPlayed);
	DDX_Text(pDX, IDC_EDIT_onPersonCash, m_onPersonCash);
	DDX_Text(pDX, IDC_EDIT_bankCash, m_bankCash);
	DDX_Text(pDX, IDC_EDIT_wldSpcX, m_wldSpcaceX);
	DDX_Text(pDX, IDC_EDIT_WldSpaceY, m_wldSpaceY);
	DDX_Text(pDX, IDC_EDIT_wldSpcZ, m_wldSpaceZ);
	DDX_Text(pDX, IDC_EDIT_userNameEdit, m_userNameEdit);
	DDX_Text(pDX, IDC_EDIT_statValueEdit, m_statValueEdit);
	DDX_Text(pDX, IDC_EDIT_SuspentionNotes, m_suspentionNotes);
	DDX_Text(pDX, IDC_EDIT_staticHandle, m_staticHandle);
	DDX_Text(pDX, IDC_EDIT_setWldID, m_setWldID);
	DDX_Text(pDX, IDC_EDIT_SkillCurExp, m_skillCurExp);
	DDX_Text(pDX, IDC_EDIT_HouseBind, m_houseBind);
	DDX_Text(pDX, IDC_EDIT_accountNumber, m_accountNumber);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_LIST_acGroup, m_acGroupList);
	DDX_Text(pDX, IDC_STATIC_lastLoggedOn, m_lastLoggedOn);
}


BEGIN_MESSAGE_MAP(CAccountViewerModule, CDialog)
	//{{AFX_MSG_MAP(CAccountViewerModule)
	ON_LBN_SELCHANGE(IDC_LIST_CharactersOnAccount, OnSelchangeLISTCharactersOnAccount)
	ON_BN_CLICKED(IDC_BTN_DeleteChar, OnBTNDeleteChar)
	ON_BN_CLICKED(IDC_BUTTON_applyPasswordChange, OnBUTTONapplyPasswordChange)
	ON_BN_CLICKED(IDC_CHECK_enableAISERVER, OnCHECKenableAISERVER)
	ON_BN_CLICKED(IDC_BUTTON_setBankCash, OnBUTTONsetBankCash)
	ON_BN_CLICKED(IDC_BUTTON_setOnPlayerCash, OnBUTTONsetOnPlayerCash)
	ON_BN_CLICKED(IDC_BUTTON_AddBankItem, OnBUTTONAddBankItem)
	ON_BN_CLICKED(IDC_BUTTON_AddCharItem, OnBUTTONAddCharItem)
	ON_BN_CLICKED(IDC_BUTTON_DeleteBankItem, OnBUTTONDeleteBankItem)
	ON_BN_CLICKED(IDC_BUTTON_DeleteCharItem, OnBUTTONDeleteCharItem)
	ON_BN_CLICKED(IDC_BUTTON_SetPosition, OnBUTTONSetPosition)
	ON_BN_CLICKED(IDC_BUTTON_ApplyNameChange, OnBUTTONApplyNameChange)
	ON_BN_CLICKED(IDC_BUTTON_ExportChar, OnBUTTONExportChar)
	ON_BN_CLICKED(IDC_BUTTON_ImportChar, OnBUTTONImportChar)
	ON_BN_CLICKED(IDC_BUTTON_SetStatValue, OnBUTTONSetStatValue)
	ON_BN_CLICKED(IDC_BUTTON_setData, OnBUTTONsetData)
	ON_LBN_SELCHANGE(IDC_LIST_skills, OnSelchangeLISTskills)
	ON_BN_CLICKED(IDC_BUTTON_SetSkillValue, OnBUTTONSetSkillValue)
	ON_BN_CLICKED(IDC_CHECK_adminPrivilidge, OnCHECKadminPrivilidge)
	ON_BN_CLICKED(IDC_BUTTON_armInven, OnBUTTONarmInven)
	ON_BN_CLICKED(IDC_BUTTON_disarmInven, OnBUTTONdisarmInven)
	ON_BN_CLICKED(IDC_CHECK_GMType, OnCHECKGMType)
	ON_BN_CLICKED(IDC_CHECK_CanSpawn, OnCHECKCanSpawn)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_CHECK_suspendAccount, OnBnClickedChecksuspendaccount)
	ON_BN_CLICKED(IDC_BUTTON_DeleteAllType, OnBnClickedButtonDeletealltype)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAccountViewerModule message handlers

BOOL CAccountViewerModule::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(TRUE);

    EnterCriticalSection(  &m_multiplayerRef->m_recieveCriticalSection  );

    m_accountName = accountPtr->m_accountName;
	m_minutesPlayed = cabLib.ConvToString(accountPtr->m_timePlayedMinutes);
	m_lastLoggedOn = cabLib.ConvToString(accountPtr->m_monthOfLastLogon)+"/"+cabLib.ConvToString(accountPtr->m_dayOfLastLogon) + "/" +cabLib.ConvToString(accountPtr->m_yearOfLastLogon);
	
    if(accountPtr->m_aiServer)
	m_enableAIserver.SetCheck(1);

	m_passwordChange = accountPtr->m_accountPassword;
	m_houseBind      = accountPtr->m_housingBind;
	m_accountNumber  = accountPtr->m_accountNumber;
	
	m_suspentionNotes = accountPtr->m_suspentionNotes;
	m_suspenAccountCheck.SetCheck(accountPtr->m_accountStatus);
	m_adminPrivlidge.SetCheck(accountPtr->m_administratorAccess);
	m_gmType.SetCheck(accountPtr->m_gm);
	m_canSpawn.SetCheck(accountPtr->m_canSpawn);
	
	UpdateData(FALSE);

	InListCharactersOnAccount(0);
	InListGlobalInventory(0);

	LeaveCriticalSection( &m_multiplayerRef->m_recieveCriticalSection);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAccountViewerModule::InListPlayerInventory(int sel)
{

}

void CAccountViewerModule::InListSkills(int sel)
{

}

void CAccountViewerModule::InListCharactersOnAccount(int sel)
{
	
}

void CAccountViewerModule::InListGlobalInventory(int sel)
{
	
}

void CAccountViewerModule::OnSelchangeLISTCharactersOnAccount() 
{
	
}

void CAccountViewerModule::OnBTNDeleteChar() 
{
	
}

void CAccountViewerModule::OnBUTTONapplyPasswordChange() 
{
	
	
}

void CAccountViewerModule::OnCHECKenableAISERVER() 
{

}

void CAccountViewerModule::OnBUTTONsetBankCash() 
{

}

void CAccountViewerModule::OnBUTTONsetOnPlayerCash() 
{
	
}

void CAccountViewerModule::OnBUTTONAddBankItem() 
{
	
}

void CAccountViewerModule::OnBUTTONAddCharItem() 
{
	
}

void CAccountViewerModule::OnBUTTONDeleteBankItem() 
{
	
}

void CAccountViewerModule::OnBUTTONDeleteCharItem() 
{
	
}

void CAccountViewerModule::OnBUTTONSetPosition() 
{

}

void CAccountViewerModule::OnBUTTONApplyNameChange() 
{
	
}

void CAccountViewerModule::OnBUTTONExportChar() 
{
	
}

void CAccountViewerModule::OnBUTTONImportChar() 
{
	
}

void CAccountViewerModule::OnBUTTONSetStatValue() 
{
	
}

void CAccountViewerModule::OnOK() 
{
	
}

void CAccountViewerModule::OnBUTTONsetData() 
{
	
}

void CAccountViewerModule::OnSelchangeLISTskills() 
{
   
}

void CAccountViewerModule::OnBUTTONSetSkillValue() 
{
	
}

void CAccountViewerModule::OnCHECKadminPrivilidge() 
{
	

}

void CAccountViewerModule::OnBUTTONarmInven() 
{
	
	
}

void CAccountViewerModule::OnBUTTONdisarmInven() 
{
	
}

void CAccountViewerModule::OnCHECKGMType() 
{
	
}

void CAccountViewerModule::OnCHECKCanSpawn() 
{
	
}

void CAccountViewerModule::OnBnClickedChecksuspendaccount()
{
	
}

void CAccountViewerModule::OnBnClickedButtonDeletealltype()
{
	
}
