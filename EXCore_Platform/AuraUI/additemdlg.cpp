// AddItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "AddItemDlg.h"
#include "AddAttachmentTypeDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddItemDlg dialog


CAddItemDlg::CAddItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddItemDlg::IDD, pParent)
	, m_visualRef(0)
	, m_dimCfgIndex(0)
	, m_dimCfgChangeTo(0)
	, m_dimCfgChangeBack(0)
{
	//{{AFX_DATA_INIT(CAddItemDlg)
	m_itemName = _T("");
	m_marketCost = 0;
	m_requiredDex = 0;
	m_requiredKnowledge = 0;
	m_requiredStr = 0;
	m_uniqueLocationID = 0;
	m_glID = 0;
	m_animapreference = 0;
	m_minimumSkillRequired = 0;
	m_masteredSkillLevel = 0;
	m_useValue = 0;
	m_armourRating = 0;
	m_bonusStatID = 0;
	m_statBonusAmount = 0;
	m_currentCharges = 0;
	m_chargeType = 0;
	m_maxCharges = 0;
	m_keyID = 0;
	m_elementDefenseValue = 0;
	m_requiredSkillLevel = 0;
	m_iconDBIndex = 0;
	m_iconMiscInt = 0;
	m_iconMiscInt2 = 0;
	m_iconPosXonscreen = 0.0f;
	m_iconPosYonscreen = 0.0f;
	m_defenseSpecificID = 0;
	m_bonusDuration = 0;
	m_damageMultiplier = 1.0f;
	m_explosionRef = -1;
	//}}AFX_DATA_INIT
}


void CAddItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddItemDlg)
	DDX_Control(pDX, IDC_LIST_IconExpObjList, m_expIconList);
	DDX_Control(pDX, IDC_CHECK_stackable, m_stackable);
	DDX_Control(pDX, IDC_CHECK_destroyWhenUsed, m_destroyWhenUsed);
	DDX_Control(pDX, IDC_CHECK_nonTransferable, m_nonTransferable);
	DDX_Control(pDX, IDC_LIST_ElementDefenseList, m_elementDefensiveList);
	DDX_Control(pDX, IDC_COMBO_Elements, m_elements);
	DDX_Control(pDX, IDC_LIST_StatBonusList, m_statBonusList);
	DDX_Control(pDX, IDC_CHECK_nonDrop, m_nonDrop);
	DDX_Control(pDX, IDC_COMBO_useType, m_useType);
	DDX_Control(pDX, IDC_CHECK_permanent, m_permanent);
	DDX_Control(pDX, IDC_COMBO_SkillsAvailable, m_skillsAvailable);
	DDX_Control(pDX, IDC_CHECK_disarmable, m_checkDisarmable);
	DDX_Control(pDX, IDC_CHECK_unarmedAttack, m_checkUnarmedAttack);
	DDX_Control(pDX, IDC_COMBO_PowerType, m_bonusType);
	DDX_Control(pDX, IDC_COMBO_AbilityType, m_abilityType);
	DDX_Control(pDX, IDC_CHECK_EMBLEM0, m_checkEmblem0);
	DDX_Control(pDX, IDC_CHECK_EMBLEM1, m_checkEmblem1);
	DDX_Control(pDX, IDC_CHECK_EMBLEM2, m_checkEmblem2);
	DDX_Control(pDX, IDC_CHECK_EMBLEM3, m_checkEmblem3);
	DDX_Control(pDX, IDC_CHECK_EMBLEM4, m_checkEmblem4);
	DDX_Text(pDX, IDC_EDIT_ItemName, m_itemName);
	DDX_Text(pDX, IDC_EDIT_MarketCost, m_marketCost);
	DDX_Text(pDX, IDC_EDIT_RequiredDex, m_requiredDex);
	DDX_Text(pDX, IDC_EDIT_RequiredKnowlege, m_requiredKnowledge);
	DDX_Text(pDX, IDC_EDIT_RequiredStr, m_requiredStr);
	DDX_Text(pDX, IDC_EDIT_UniqueLocationID, m_uniqueLocationID);
	DDX_Text(pDX, IDC_EDIT_GL_ID, m_glID);
	DDX_Text(pDX, IDC_EDIT_AccessPrefValue, m_animapreference);
	DDX_Text(pDX, IDC_EDIT_animAccessDefault, m_animaAccessDefault);
	DDX_Text(pDX, IDC_EDIT_minimumSkillRequired, m_minimumSkillRequired);
	DDX_Text(pDX, IDC_EDIT_masteredSkillLevel, m_masteredSkillLevel);
	DDX_Text(pDX, IDC_EDIT_useValue, m_useValue);
	DDX_Text(pDX, IDC_EDIT_armourRating, m_armourRating);
	DDX_Text(pDX, IDC_EDIT_BonusStatID, m_bonusStatID);
	DDX_Text(pDX, IDC_EDIT_StatBonusAmount, m_statBonusAmount);
	DDX_Text(pDX, IDC_EDIT_CurrentCharges, m_currentCharges);
	DDX_Text(pDX, IDC_EDIT_ChargeType, m_chargeType);
	DDX_Text(pDX, IDC_EDIT_MaxCharges, m_maxCharges);
	DDX_Text(pDX, IDC_EDIT_keyID, m_keyID);
	DDX_Text(pDX, IDC_EDIT_ElementDefenseValue, m_elementDefenseValue);
	DDX_Text(pDX, IDC_EDIT_requiredSkillLevel, m_requiredSkillLevel);
	DDX_Text(pDX, IDC_EDIT_iconDBIndex, m_iconDBIndex);
	DDX_Text(pDX, IDC_EDIT_iconMiscInt, m_iconMiscInt);
	DDX_Text(pDX, IDC_EDIT_iconMiscInt2, m_iconMiscInt2);
	DDX_Text(pDX, IDC_EDIT_iconPosXonscreen, m_iconPosXonscreen);
	DDX_Text(pDX, IDC_EDIT_iconPosYonscreen, m_iconPosYonscreen);
	DDX_Text(pDX, IDC_EDIT_defenseSpecificID, m_defenseSpecificID);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_visualReference, m_visualRef);
	DDX_Text(pDX, IDC_EDIT_DimCfgIndex, m_dimCfgIndex);
    DDX_Text(pDX, IDC_EDIT_dimCfgChangeTo, m_dimCfgChangeTo);
	DDX_Text(pDX, IDC_EDIT_dimCfgChangeBack, m_dimCfgChangeBack);
    DDX_Text(pDX, IDC_EDIT_reloadDuration, m_reloadDuration);
	DDX_Text(pDX, IDC_EDIT_BonusDuration, m_bonusDuration);
	DDX_Text(pDX, IDC_EDIT_damageMult, m_damageMultiplier);
	DDX_Text(pDX, IDC_EDIT_MeleeExplosionRef, m_explosionRef);
	DDX_Text(pDX, IDC_EDIT_ShotsPerFire, m_shotsPerFire);
	DDX_Text(pDX, IDC_EDIT_AmmoRegenRate, m_ammoRegenRate);
	DDX_Text(pDX, IDC_EDIT_AmmoSec, m_ammoSecTick);
	DDX_Text(pDX, IDC_EDIT_ItemHP, m_itemHP);
	
}


BEGIN_MESSAGE_MAP(CAddItemDlg, CDialog)
	//{{AFX_MSG_MAP(CAddItemDlg)
	ON_BN_CLICKED(IDC_BUTTON_AddAttachmentType, OnBUTTONAddAttachmentType)
	ON_BN_CLICKED(IDC_CHECK_disarmable, OnCHECKdisarmable)
	ON_BN_CLICKED(IDC_CHECK_EMBLEM0, OnCHECKemblem0)
	ON_BN_CLICKED(IDC_CHECK_EMBLEM1, OnCHECKemblem1)
	ON_BN_CLICKED(IDC_CHECK_EMBLEM2, OnCHECKemblem2)
	ON_BN_CLICKED(IDC_CHECK_EMBLEM3, OnCHECKemblem3)
	ON_BN_CLICKED(IDC_CHECK_EMBLEM4, OnCHECKemblem4)
	ON_BN_CLICKED(IDC_CHECK_permanent, OnCHECKpermanent)
	ON_BN_CLICKED(IDC_BUTTON_DeleteStatBonus, OnBUTTONDeleteStatBonus)
	ON_BN_CLICKED(IDC_BUTTON_AddStatBonus, OnBUTTONAddStatBonus)
	ON_LBN_SELCHANGE(IDC_LIST_StatBonusList, OnSelchangeLISTStatBonusList)
	ON_BN_CLICKED(IDC_BUTTON_AddElementDefense, OnBUTTONAddElementDefense)
	ON_BN_CLICKED(IDC_BUTTON_DeleteElementDefense, OnBUTTONDeleteElementDefense)
	ON_BN_CLICKED(IDC_CHECK_stackable, OnCHECKstackable)
	ON_BN_CLICKED(IDC_BUTTON_AddIconExpObj, OnBUTTONAddIconExpObj)
	ON_BN_CLICKED(IDC_BUTTON_DeleteIconExpObj, OnBUTTONDeleteIconExpObj)
	ON_LBN_SELCHANGE(IDC_LIST_IconExpObjList, OnSelchangeLISTIconExpObjList)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddItemDlg message handlers
BOOL CAddItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);
    
	m_itemName                  = m_itemRef->m_itemName;
	m_marketCost                = m_itemRef->m_marketCost;
	m_requiredDex               = m_itemRef->m_requiredDex;
	m_requiredKnowledge         = m_itemRef->m_requiredKnowledge;
	m_requiredStr               = m_itemRef->m_requiredStr;
	m_uniqueLocationID          = m_itemRef->m_locationID;
	m_glID                      = m_itemRef->m_globalID;
	m_animapreference           = m_itemRef->m_animAccessValue;
	m_animaAccessDefault		= m_itemRef->m_animAccessDefault;
	m_minimumSkillRequired      = m_itemRef->m_minimumLevel;
	m_masteredSkillLevel        = m_itemRef->m_masteredLevel;   
	m_useValue                  = m_itemRef->m_miscUseValue;
	m_armourRating              = m_itemRef->m_armourClass;
	m_currentCharges            = m_itemRef->m_chargeCurrent;
	m_chargeType                = m_itemRef->m_chargeType;
	m_maxCharges                = m_itemRef->m_maxCharges;
	m_keyID                     = m_itemRef->m_keyID;
	m_requiredSkillLevel        = m_itemRef->m_requiredSkillLevel;
	m_defenseSpecificID         = m_itemRef->m_defensesSpecific;
	m_visualRef                 = m_itemRef->m_visualRef;
	m_dimCfgIndex               = m_itemRef->m_dimCfgIndex;
	m_dimCfgChangeTo            = m_itemRef->m_dimCfgChangeTo;
	m_dimCfgChangeBack          = m_itemRef->m_dimCfgChangeBack;
	m_reloadDuration            = m_itemRef->m_reloadTime;
	m_bonusDuration				= m_itemRef->m_bonusDuration;
	m_damageMultiplier			= m_itemRef->m_damageMultiplier;
	m_explosionRef				= m_itemRef->m_explosionRef;
	m_shotsPerFire				= m_itemRef->m_shotsPerFire;
	m_ammoRegenRate				= m_itemRef->m_ammoRegenRate;
	m_itemHP					= m_itemRef->m_maxHP;
	m_ammoSecTick				= m_itemRef->m_ammoSecTick;

    m_useType.SetCurSel(m_itemRef->m_useType);
	m_bonusType.SetCurSel(m_itemRef->m_bonusType);
	m_abilityType.SetCurSel(m_itemRef->m_abilityType);

    if(m_itemRef->m_disarmable)
	   m_checkDisarmable.SetCheck(1);

	if(m_itemRef->m_emblemEffects[0])
		m_checkEmblem0.SetCheck(1);
	
	if(m_itemRef->m_emblemEffects[1])
		m_checkEmblem1.SetCheck(1);
	
	if(m_itemRef->m_emblemEffects[2])
		m_checkEmblem2.SetCheck(1);
	
	if(m_itemRef->m_emblemEffects[3])
		m_checkEmblem3.SetCheck(1);
	
	if(m_itemRef->m_emblemEffects[4])
		m_checkEmblem4.SetCheck(1);

	if(m_itemRef->m_unarmedAttack)
	   m_checkUnarmedAttack.SetCheck(1);

	if(m_itemRef->m_permanent)
	   m_permanent.SetCheck(1);

	if(m_itemRef->m_nonDrop)
	   m_nonDrop.SetCheck(1);

	if(m_itemRef->m_nonTransferable)
	   m_nonTransferable.SetCheck(1);

	if(m_itemRef->m_destroyOnUse)
	   m_destroyWhenUsed.SetCheck(1);

	if(m_itemRef->m_stackable)
	   m_stackable.SetCheck(1);

	if(m_currentElements)
	for(POSITION posLoc = m_currentElements->GetHeadPosition();posLoc != NULL;)
	{
        CElementObj * elementPtr = (CElementObj *) m_currentElements->GetNext(posLoc);
		m_elements.AddString(elementPtr->m_elementName);
	}
	m_elements.SetCurSel(0);


	m_skillsAvailable.AddString("NONE");
    if(m_skillDatabaseRef)
    for(POSITION posLoc = m_skillDatabaseRef->GetHeadPosition();posLoc != NULL;)
	{
        CSkillObject * skillPtr = (CSkillObject *) m_skillDatabaseRef->GetNext(posLoc);
		m_skillsAvailable.AddString(skillPtr->m_skillName);
	}
	m_skillsAvailable.SetCurSel(m_skillDatabaseRef->GetSkillIndexByType(m_itemRef->m_requiredSkillType)+1);
  


	UpdateData(FALSE);

	InListStatBonuses(0);
	InListElementStatus(0);
	InListIconExpandable(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddItemDlg::OnBUTTONAddAttachmentType() 
{
     CAddAttachmentTypeDlg dlg;
	 dlg.m_itemRef               = m_itemRef;
	 dlg.m_armedInventoryListRef = m_armedInventoryListRef;
	 dlg.m_enitityDB             = m_enitityDB;
	 if(dlg.DoModal()==IDOK){
        


	 }
}

void CAddItemDlg::OnOK() 
{
	UpdateData(TRUE);
 
    if(m_itemRef->m_globalID == -1){//must assign a global ID
	   int newID = 0;
       for(POSITION posLoc = m_enitityDB->GetHeadPosition();posLoc != NULL;)
	   {
           CMovementObj * mObj = (CMovementObj *)m_enitityDB->GetNext(posLoc);
           if(mObj->m_itemDB){//valid
			   for(POSITION itPos = mObj->m_itemDB->GetHeadPosition();itPos != NULL;)
			   {
                   CItemObj * m_itemObj = (CItemObj *)mObj->m_itemDB->GetNext(itPos);
				   if(newID <= m_itemObj->m_globalID){
                      newID =  m_itemObj->m_globalID+1;
				   }
			   }
		   }//end valid
	   }
	   m_itemRef->m_globalID = newID;
	}//end must assign a global ID

    m_itemRef->m_itemName            = m_itemName;
	m_itemRef->m_marketCost          = m_marketCost;
	m_itemRef->m_requiredDex         = m_requiredDex;
	m_itemRef->m_requiredKnowledge   = m_requiredKnowledge;
	m_itemRef->m_requiredStr         = m_requiredStr;
	m_itemRef->m_locationID          = m_uniqueLocationID;	
	m_itemRef->m_minimumLevel        = m_minimumSkillRequired;
	m_itemRef->m_masteredLevel       = m_masteredSkillLevel;
	m_itemRef->m_miscUseValue        = m_useValue;
	m_itemRef->m_armourClass         = m_armourRating;
	m_itemRef->m_chargeCurrent       = m_currentCharges;
	m_itemRef->m_chargeType          = m_chargeType;
	m_itemRef->m_maxCharges          = m_maxCharges;
    m_itemRef->m_keyID               = m_keyID;
	m_itemRef->m_requiredSkillLevel  = m_requiredSkillLevel;
	m_itemRef->m_defensesSpecific    = m_defenseSpecificID;
	m_itemRef->m_visualRef           = m_visualRef;

	m_itemRef->m_nonTransferable     = m_nonTransferable.GetCheck();
    m_itemRef->m_destroyOnUse        = m_destroyWhenUsed.GetCheck();
	m_itemRef->m_useType             = m_useType.GetCurSel();
    m_itemRef->m_permanent           = m_permanent.GetCheck();
	m_itemRef->m_disarmable          = m_checkDisarmable.GetCheck();
	m_itemRef->m_unarmedAttack       = m_checkUnarmedAttack.GetCheck();
	m_itemRef->m_emblemEffects[0]	 = m_checkEmblem0.GetCheck();
	m_itemRef->m_emblemEffects[1]	 = m_checkEmblem1.GetCheck();
	m_itemRef->m_emblemEffects[2]	 = m_checkEmblem2.GetCheck();
	m_itemRef->m_emblemEffects[3]	 = m_checkEmblem3.GetCheck();
	m_itemRef->m_emblemEffects[4]	 = m_checkEmblem4.GetCheck();
	m_itemRef->m_nonDrop             = m_nonDrop.GetCheck();
	m_itemRef->m_dimCfgIndex         = m_dimCfgIndex;
	m_itemRef->m_dimCfgChangeTo      = m_dimCfgChangeTo;
	m_itemRef->m_dimCfgChangeBack    = m_dimCfgChangeBack;
	m_itemRef->m_reloadTime          = m_reloadDuration;
	m_itemRef->m_bonusDuration		 = m_bonusDuration;
	m_itemRef->m_damageMultiplier	 = m_damageMultiplier;
	m_itemRef->m_explosionRef		 = m_explosionRef;
	m_itemRef->m_bonusType			 = m_bonusType.GetCurSel();
	m_itemRef->m_abilityType		 = m_abilityType.GetCurSel();
	m_itemRef->m_shotsPerFire		 = m_shotsPerFire;
	m_itemRef->m_ammoRegenRate		 = m_ammoRegenRate;
	m_itemRef->m_hp					 = m_itemHP;
	m_itemRef->m_maxHP				 = m_itemHP;
	m_itemRef->m_ammoSecTick		 = m_ammoSecTick;
    m_itemRef->m_animAccessDefault	 = m_animaAccessDefault;
	m_itemRef->m_animAccessValue     = m_animapreference;


	CSkillObject * skillPtr = m_skillDatabaseRef->GetSkillByIndex(m_skillsAvailable.GetCurSel()-1);
	if(skillPtr)
       m_itemRef->m_requiredSkillType = skillPtr->m_skillType;
	else
	   m_itemRef->m_requiredSkillType = -1;

	UpdateData(FALSE);
	
	CDialog::OnOK();
}

void CAddItemDlg::OnCHECKdisarmable() 
{
	if(m_itemRef->m_disarmable)
	   m_itemRef->m_disarmable = FALSE;
	else
	   m_itemRef->m_disarmable = TRUE;
}

void CAddItemDlg::OnCHECKemblem0()
{
	if(m_itemRef->m_emblemEffects[0])
		m_itemRef->m_emblemEffects[0] = FALSE;
	else
		m_itemRef->m_emblemEffects[0] = TRUE;
}

void CAddItemDlg::OnCHECKemblem1()
{
	if(m_itemRef->m_emblemEffects[1])
		m_itemRef->m_emblemEffects[1] = FALSE;
	else
		m_itemRef->m_emblemEffects[1] = TRUE;
}

void CAddItemDlg::OnCHECKemblem2()
{
	if(m_itemRef->m_emblemEffects[2])
		m_itemRef->m_emblemEffects[2] = FALSE;
	else
		m_itemRef->m_emblemEffects[2] = TRUE;
}

void CAddItemDlg::OnCHECKemblem3()
{
	if(m_itemRef->m_emblemEffects[3])
		m_itemRef->m_emblemEffects[3] = FALSE;
	else
		m_itemRef->m_emblemEffects[3] = TRUE;
}

void CAddItemDlg::OnCHECKemblem4()
{
	if(m_itemRef->m_emblemEffects[4])
		m_itemRef->m_emblemEffects[4] = FALSE;
	else
		m_itemRef->m_emblemEffects[4] = TRUE;
}

void CAddItemDlg::OnCHECKunarmedAttack() 
{
	//m_itemRef->m_unarmedAttack = !m_itemRef->m_unarmedAttack;
	//above isn't needed because IDOK sets the value.
}

void CAddItemDlg::InListStatBonuses(int sel)
{
     m_statBonusList.ResetContent();
     if(!m_itemRef->m_statBonuses)
         return;

	 UpdateData(TRUE);

	 for(POSITION posLoc = m_itemRef->m_statBonuses->GetHeadPosition();posLoc != NULL;)
	 {
         CStatBonusObj * statBonus = (CStatBonusObj *) m_itemRef->m_statBonuses->GetNext(posLoc);
         CString build = "BND ID:";
         build = operator+(build,ConvToString(statBonus->m_statID));
         m_statBonusList.AddString(build);

	 }

	 if(sel > -1)
		m_statBonusList.SetCurSel(sel);

	 UpdateData(TRUE);
}


void CAddItemDlg::OnCHECKpermanent() 
{
	// TODO: Add your control notification handler code here
	
}

void CAddItemDlg::OnBUTTONDeleteStatBonus() 
{
	if(!m_itemRef->m_statBonuses)
		return;

	int index = m_statBonusList.GetCurSel();
	if(index == -1)
	{
		AfxMessageBox("Must select a stat bonus");
		return;
	}

	POSITION posLoc = m_itemRef->m_statBonuses->FindIndex(index);
	if(!posLoc)
		return;

	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CStatBonusObj * statBonus = (CStatBonusObj *)m_itemRef->m_statBonuses->GetAt(posLoc);
	delete statBonus;
	statBonus = 0;

	m_itemRef->m_statBonuses->RemoveAt(posLoc);
	if(m_itemRef->m_statBonuses->GetCount()==0)
	{
		delete m_itemRef->m_statBonuses;
		m_itemRef->m_statBonuses = 0;
	}
	InListStatBonuses(index-1);
}

void CAddItemDlg::OnBUTTONAddStatBonus() 
{
	UpdateData(TRUE);
	if(!m_itemRef->m_statBonuses)
        m_itemRef->m_statBonuses = new CStatBonusList();
	    CStatBonusObj * statBonus = new CStatBonusObj();
	
        statBonus->m_statID    = m_bonusStatID;
	    statBonus->m_statBonus = m_statBonusAmount;
        
		m_itemRef->m_statBonuses->AddTail(statBonus);

	UpdateData(FALSE);
	InListStatBonuses(m_itemRef->m_statBonuses->GetCount());
}


//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----converts a long into string-----------------------------//
CString CAddItemDlg::ConvToString(int number)
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
void CAddItemDlg::OnSelchangeLISTStatBonusList() 
{
	if(!m_itemRef->m_statBonuses)
         return;

	int index = m_statBonusList.GetCurSel();
	if(index == -1)
	{
       return;
	}


    POSITION posLoc = m_itemRef->m_statBonuses->FindIndex(index);
    if(!posLoc)
        return;

	    CStatBonusObj * statBonus = (CStatBonusObj *)m_itemRef->m_statBonuses->GetAt(posLoc);
		UpdateData(TRUE);
		m_bonusStatID           = statBonus->m_statID;
	    m_statBonusAmount       = statBonus->m_statBonus;
        UpdateData(FALSE);
		
	
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
void CAddItemDlg::OnBUTTONAddElementDefense() 
{
     if(!m_itemRef->m_elementDefenses)
         m_itemRef->m_elementDefenses = new CElementStatusList();

	 int elementSelect = m_elements.GetCurSel();
	 if(elementSelect == -1)
	 {
		MessageBox("Must select a alement");
		return;
	 }

	 UpdateData(TRUE);
	 CElementStatusObj * statusPtr = new CElementStatusObj();
	 statusPtr->m_currentValue = m_elementDefenseValue;
	 statusPtr->m_elementIndex = elementSelect;
	 UpdateData(FALSE);

	 m_itemRef->m_elementDefenses->AddTail(statusPtr);	
	 InListElementStatus(m_itemRef->m_elementDefenses->GetCount()-1);
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
void CAddItemDlg::InListElementStatus(int sel)
{
     if(!m_itemRef->m_elementDefenses)
		 return;
    
         m_elementDefensiveList.ResetContent();

	     for(POSITION posLoc = m_itemRef->m_elementDefenses->GetHeadPosition();posLoc != NULL;)
		 {
             CElementStatusObj * statusPtr = (CElementStatusObj *) m_itemRef->m_elementDefenses->GetNext(posLoc);
             CString tempName = "Unknown";
			 POSITION elePos = m_currentElements->FindIndex(statusPtr->m_elementIndex);
			 if(elePos)
			 {
               CElementObj * ptr = (CElementObj *)m_currentElements->GetAt(elePos);
               tempName = ptr->m_elementName;
			   
			 }
			 
			 CString append = operator+(":",ConvToString(statusPtr->m_currentValue));
			 
			 m_elementDefensiveList.AddString(operator+(tempName,append));
		 }

	     if(sel > -1)
		    m_elementDefensiveList.SetCurSel(sel);

}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
void CAddItemDlg::OnBUTTONDeleteElementDefense() 
{
	int elementSelect = m_elementDefensiveList.GetCurSel();
	if(elementSelect == -1)
	{
		MessageBox("Must select a status element");
		return;
	}

	POSITION posLoc = m_itemRef->m_elementDefenses->FindIndex(elementSelect);
	if(!posLoc)
		return;
	
	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CElementStatusObj * statusPtr = (CElementStatusObj *)m_itemRef->m_elementDefenses->GetAt(posLoc);
	statusPtr->SafeDelete();
	delete statusPtr;
	statusPtr = 0;
	m_itemRef->m_elementDefenses->RemoveAt(posLoc);
	
	InListElementStatus(elementSelect-1);	
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
void CAddItemDlg::OnCHECKstackable() 
{
    if(m_itemRef->m_stackable)
	   m_itemRef->m_stackable = FALSE;
	else
       m_itemRef->m_stackable = TRUE;

}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
void CAddItemDlg::InListIconExpandable(int sel)
{
   m_expIconList.ResetContent();

   if(m_itemRef->m_expansiveCollection)
   for(POSITION posLoc = m_itemRef->m_expansiveCollection->GetHeadPosition();posLoc != NULL;)
   {
       CExpansiveObj * expansiveObj = (CExpansiveObj *) m_itemRef->m_expansiveCollection->GetNext(posLoc);
	   m_expIconList.AddString("ICON ENTRY");
   }

   if(sel > -1)
	  m_expIconList.SetCurSel(sel);
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
void CAddItemDlg::OnBUTTONAddIconExpObj() 
{
/*m_iconDBIndex = 0;
	m_iconMiscInt = 0;
	m_iconMiscInt2 = 0;
	m_iconPosXonscreen = 0.0f;
	m_iconPosYonscreen = 0.0f;*/

	if(!m_itemRef->m_expansiveCollection)
        m_itemRef->m_expansiveCollection = new CCExpansiveObjList();


	    CExpansiveObj * newObj = new CExpansiveObj();
		UpdateData(TRUE);
		newObj->m_iconIndex   = m_iconDBIndex;
	    newObj->m_miscInt     = m_iconMiscInt;
		newObj->m_miscInt2    = m_iconMiscInt2;
		newObj->m_xPos        = m_iconPosXonscreen;
		newObj->m_yPos        = m_iconPosYonscreen;
		UpdateData(FALSE);

		m_itemRef->m_expansiveCollection->AddTail(newObj);
		InListIconExpandable(m_itemRef->m_expansiveCollection->GetCount()-1);
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
void CAddItemDlg::OnBUTTONDeleteIconExpObj() 
{
	int select = m_expIconList.GetCurSel();
	if(select == -1)
	{
	AfxMessageBox("Must select a icon object");
	return;
	}

	POSITION posLoc = m_itemRef->m_expansiveCollection->FindIndex(select);
	if(!posLoc)
		return;

	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CExpansiveObj * newObj = (CExpansiveObj *) m_itemRef->m_expansiveCollection->GetAt(posLoc);
	delete newObj;
	newObj = 0;

	m_itemRef->m_expansiveCollection->RemoveAt(posLoc);
	if(m_itemRef->m_expansiveCollection->GetCount()==0)
	{
		delete m_itemRef->m_expansiveCollection;
		m_itemRef->m_expansiveCollection = 0;
	}
	
	InListIconExpandable(select-1);
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-------------------------------------------------------------//
void CAddItemDlg::OnSelchangeLISTIconExpObjList() 
{
	int select = m_expIconList.GetCurSel();
     if(select == -1)
	 {
        AfxMessageBox("Must select a icon object");
        return;
	 }


     POSITION posLoc = m_itemRef->m_expansiveCollection->FindIndex(select);
	 if(!posLoc)
		 return;

         CExpansiveObj * newObj = (CExpansiveObj *) m_itemRef->m_expansiveCollection->GetAt(posLoc);
		 UpdateData(TRUE);

         m_iconDBIndex      = newObj->m_iconIndex;
	     m_iconMiscInt      = newObj->m_miscInt;
		 m_iconMiscInt2     = newObj->m_miscInt2;
		 m_iconPosXonscreen = newObj->m_xPos;
		 m_iconPosYonscreen = newObj->m_yPos;

		 UpdateData(FALSE);
}

void CAddItemDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	OnOK();
}
