// SpawnGenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "ItemConflictDlg.h"
#include "direct.h"
#include "spawngendlg.h"
#include "CReferenceImporterExporter.h"
#include "globalstatusvariables.h"
#include "SqlConnectionDlg.h"
#include "dbconnectionsystem.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpawnGenDlg dialog


CSpawnGenDlg::CSpawnGenDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSpawnGenDlg::IDD, pParent)
	, m_randomAnimation(0)
	, m_mountOption(0)
	, m_randomGlobalPerc(0)
	, m_randomExpEffect(0)
	, m_organizedGroupID(0)
	, m_sightRangeOverride(0)
	, m_interactRadius(0)
	, m_autoTriggerType(0)
	, m_worldChannel(0)
{
	//{{AFX_DATA_INIT(CSpawnGenDlg)
	m_posX = 0.0f;
	m_posY = 0.0f;
	m_posZ = 0.0f;
	m_respawnDuration = 0;
	m_worldID = 0;
	m_curAnimation = 0;
	m_generatorName = _T("");
	m_spawnGenID = 0;
	m_keyID = 0;
	m_curCfgNum = 0;
	m_displayedName = _T("");
	m_rotationAdjust = 0;
	//}}AFX_DATA_INIT
	m_noResponsMsg = _T("");
}


void CSpawnGenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpawnGenDlg)
	DDX_Control(pDX, IDC_CHECK_armed, m_checkArmed);
	DDX_Control(pDX, IDC_COMBO_ItemType, m_itemTypeCombo);
	DDX_Control(pDX, IDC_COMBO_DimCfgList, m_comboDimCfgList);
	DDX_Control(pDX, IDC_LIST_ContentsList, m_contentsList);
	DDX_Control(pDX, IDC_COMBO_WorldSelection, m_worldSelection);
	DDX_Control(pDX, IDC_COMBO_DropValueGlid, m_dropValueGlids);
	DDX_Control(pDX, IDC_COMBO_DBRef, m_dbRef);
	DDX_Control(pDX, IDC_LIST_GeneratorList, m_generators);
	DDX_Text(pDX, IDC_EDIT_POSX, m_posX);
	DDX_Text(pDX, IDC_EDIT_POSY, m_posY);
	DDX_Text(pDX, IDC_EDIT_POSZ, m_posZ);
	DDX_Text(pDX, IDC_EDIT_RespawnDuration, m_respawnDuration);
	DDX_Text(pDX, IDC_EDIT_WorldID, m_worldID);
	DDX_Text(pDX, IDC_EDIT_CurAnimation, m_curAnimation);
	DDX_Text(pDX, IDC_EDIT_GeneratorName, m_generatorName);
	DDX_Text(pDX, IDC_EDIT_SpawnGenID, m_spawnGenID);
	DDX_Text(pDX, IDC_EDIT_keyID, m_keyID);
	DDX_Text(pDX, IDC_EDIT_CURCFGNUM, m_curCfgNum);
	DDX_Text(pDX, IDC_EDIT_DisplayedName, m_displayedName);
	DDX_Text(pDX, IDC_EDIT_rotationAdjust, m_rotationAdjust);
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_EDIT_RandomAnimation, m_randomAnimation);
	DDX_Text(pDX, IDC_EDIT_MountOption, m_mountOption);
	DDX_Text(pDX, IDC_EDIT_RandomGlobalPerc, m_randomGlobalPerc);
	DDX_Text(pDX, IDC_EDIT_RandomExpEffect, m_randomExpEffect);
	DDX_Control(pDX, IDC_TREE_generatorsList, m_generatorTree);
	DDX_Text(pDX, IDC_EDIT_OrganizeGroupID, m_organizedGroupID);
	DDX_Text(pDX, IDC_EDIT_NoResponseMSG, m_noResponsMsg);
	DDX_Text(pDX, IDC_EDIT_sightRangeOverride, m_sightRangeOverride);


	DDX_Text(pDX, IDC_EDIT_InteractRadius, m_interactRadius);
	DDX_Text(pDX, IDC_EDIT_autoTriggerType, m_autoTriggerType);
	DDX_Text(pDX, IDC_EDIT_WorldChannel, m_worldChannel);
    DDX_Text(pDX, IDC_EDIT_entityMappingOverride, m_entityMappingOverride);
}


BEGIN_MESSAGE_MAP(CSpawnGenDlg, CDialog)
	//{{AFX_MSG_MAP(CSpawnGenDlg)
	ON_BN_CLICKED(IDC_BUTTON_AddNew, OnBUTTONAddNew)
	ON_BN_CLICKED(IDC_BUTTON_Edit, OnBUTTONEdit)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_GetCurData, OnBUTTONGetCurData)
	ON_LBN_SELCHANGE(IDC_LIST_GeneratorList, OnSelchangeLISTGeneratorList)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_AddItemToContents, OnBUTTONAddItemToContents)
	ON_BN_CLICKED(IDC_BUTTON_DeleteItemFromContents, OnBUTTONDeleteItemFromContents)
	ON_LBN_SELCHANGE(IDC_LIST_ContentsList, OnSelchangeLISTContentsList)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_DimAdjust, OnDeltaposSPINDimAdjust)
	ON_CBN_SELCHANGE(IDC_COMBO_DimCfgList, OnSelchangeCOMBODimCfgList)
	ON_BN_CLICKED(IDC_CHECK_armed, OnCHECKarmed)
	ON_BN_CLICKED(IDC_BUTTON_TypeCfg, OnBUTTONTypeCfg)
	ON_BN_CLICKED(IDC_BUTTON_SaveUnit, OnBUTTONSaveUnit)
	ON_BN_CLICKED(IDC_BUTTON_LoadUnit, OnBUTTONLoadUnit)
	ON_BN_CLICKED(IDC_BUTTON_RebuildDim, OnBUTTONRebuildDim)
	ON_BN_CLICKED(IDC_BUTTON_IntegrityCheck, OnBUTTONIntegrityCheck)
	ON_BN_CLICKED(IDC_BUTTON_rebuild, OnBUTTONrebuild)
	ON_BN_CLICKED(IDC_BUTTON_search, OnBUTTONsearch)
	//}}AFX_MSG_MAP
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_generatorsList, OnTvnSelchangedTreegeneratorslist)
	ON_CBN_SELCHANGE(IDC_COMBO_WorldSelection, OnCbnSelchangeComboWorldselection)
	ON_BN_CLICKED(IDC_BUTTON_ExportToSql, OnBnClickedButtonExporttosql)
	ON_BN_CLICKED(IDC_BUTTON_ImportFromSql, OnBnClickedButtonImportfromsql)
	ON_BN_CLICKED(IDC_BUTTON_LoadFromRNE, &CSpawnGenDlg::OnBnClickedButtonLoadfromrne)
	ON_BN_CLICKED(IDC_BUTTON_LoadUploadSQLToFolder, &CSpawnGenDlg::OnBnClickedButtonLoaduploadsqltofolder)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpawnGenDlg message handlers

void CSpawnGenDlg::OnBUTTONAddNew() 
{
 
	
}

void CSpawnGenDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

HTREEITEM CSpawnGenDlg::InsertRoot(CString name,int idIndex,BOOL selected)
{
     const char * nameLoc = (LPCTSTR)name;
		TVINSERTSTRUCT tvInsert;
        tvInsert.hParent = TVI_ROOT;
        tvInsert.hInsertAfter = TVI_LAST;
        tvInsert.item.mask = TVIF_TEXT | TVIF_PARAM;
        tvInsert.item.pszText = (char*)nameLoc;//_T("United States");
		tvInsert.item.lParam = idIndex;
		HTREEITEM temp = m_generatorTree.InsertItem(&tvInsert);
		if(selected)
		{
        m_generatorTree.SelectItem(temp);
		m_generatorTree.SetFocus();
		}
		return temp;
}

HTREEITEM CSpawnGenDlg::AddTreeItem(HTREEITEM root,CString name,int idIndex,BOOL selected)
{
        const char * nameLoc = (LPCTSTR)name;
		TVINSERTSTRUCT tvInsert;
        tvInsert.hParent = root;
        tvInsert.hInsertAfter = TVI_LAST;
        tvInsert.item.mask = TVIF_TEXT | TVIF_PARAM;
        tvInsert.item.pszText = (char*)nameLoc;//_T("United States");
		tvInsert.item.lParam = idIndex;
		HTREEITEM temp =  m_generatorTree.InsertItem(&tvInsert);
		if(selected)
		{
		m_generatorTree.SelectItem(temp);
		m_generatorTree.SetFocus();
		}
		return temp;
}



void CSpawnGenDlg::InListTreeGenerators(int sel)
{

   
}





void CSpawnGenDlg::InListGenerators(int sel)
{
	
}

BOOL CSpawnGenDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSpawnGenDlg::OnBUTTONEdit() 
{
   
}

void CSpawnGenDlg::OnButtonDelete() 
{
   
}

void CSpawnGenDlg::OnBUTTONGetCurData() 
{
	if(!m_focusEntityExists)
	{
        MessageBox("Focus entity was not spawned");
        return;
	}
	D3DVECTOR posWldSpc;
    m_focusEntity->List_baseFrame->GetPosition(NULL,&posWldSpc);

	UpdateData(TRUE);
	m_posX                                          = posWldSpc.x;
    m_posY                                          = posWldSpc.y;
    m_posZ                                          = posWldSpc.z;

	/*m_posX                                          = m_focusEntity->List_lastPosition.x;
    m_posY                                          = m_focusEntity->List_lastPosition.y;
    m_posZ                                          = m_focusEntity->List_lastPosition.z;*/
	UpdateData(FALSE);
}

void CSpawnGenDlg::OnSelchangeLISTGeneratorList() 
{
	
}

CString CSpawnGenDlg::ConvToString(int number)
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



void CSpawnGenDlg::OnButtonLoad() 
{

}

void CSpawnGenDlg::OnButtonSave() 
{
	
}

void CSpawnGenDlg::OnBUTTONAddItemToContents() 
{
    
}

void CSpawnGenDlg::InListItemContents(int sel)
{
   
}

void CSpawnGenDlg::OnBUTTONDeleteItemFromContents() 
{
	
}

void CSpawnGenDlg::OnSelchangeLISTContentsList() 
{

}

void CSpawnGenDlg::OnDeltaposSPINDimAdjust(NMHDR* pNMHDR, LRESULT* pResult) 
{
	
}

void CSpawnGenDlg::OnSelchangeCOMBODimCfgList() 
{

}

void CSpawnGenDlg::InListConfigurations(CServerItemGenObj * spnGenPtr,int sel)
{
   m_comboDimCfgList.ResetContent();
   if(!spnGenPtr->m_serverItem->m_spawnCfg)
	   return;
 
       for(int i=0;i<spnGenPtr->m_serverItem->m_spawnCfgCount;i++)
           m_comboDimCfgList.AddString(operator+(ConvToString(i),":CFG"));

	   if(sel > -1)
		  m_comboDimCfgList.SetCurSel(sel);
}

void CSpawnGenDlg::OnCHECKarmed() 
{

}

void CSpawnGenDlg::OnBUTTONTypeCfg() 
{
	
}

void CSpawnGenDlg::OnBUTTONSaveUnit() 
{
	
}

void CSpawnGenDlg::OnBUTTONLoadUnit() 
{

}

void CSpawnGenDlg::OnBUTTONRebuildDim() 
{

}

void CSpawnGenDlg::OnBUTTONIntegrityCheck() 
{
	
}

void CSpawnGenDlg::OnBUTTONrebuild() 
{

	
}


void CSpawnGenDlg::SetAllItemData(CItemObj * itemData,int glid) 
{

}


void CSpawnGenDlg::OnBUTTONsearch() 
{
	
}

void CSpawnGenDlg::OnTvnSelchangedTreegeneratorslist(NMHDR *pNMHDR, LRESULT *pResult)
{
	
}

void CSpawnGenDlg::OnCbnSelchangeComboWorldselection()
{
	// TODO: Add your control notification handler code here
}

void CSpawnGenDlg::OnBnClickedButtonExporttosql()
{
}

void CSpawnGenDlg::OnBnClickedButtonImportfromsql()
{
}

void CSpawnGenDlg::OnBnClickedButtonLoadfromrne()
{
}

void CSpawnGenDlg::OnBnClickedButtonLoaduploadsqltofolder()
{

}
