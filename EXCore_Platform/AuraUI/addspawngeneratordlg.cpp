// AddSpawnGeneratorDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "AddSpawnGeneratorDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddSpawnGeneratorDlg dialog


CAddSpawnGeneratorDlg::CAddSpawnGeneratorDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAddSpawnGeneratorDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddSpawnGeneratorDlg)
	m_generatorName = _T("");
	m_maxOutputCapacity = 0;
	m_possibilityOfSpawn = 0;
	m_posX = 0.0f;
	m_posY = 0.0f;
	m_posZ = 0.0f;
	m_spawnIndex = 0;
	m_spawnRadius = 0.0f;
	m_spawnType = 0;
	m_aiCfg = 0;
	//}}AFX_DATA_INIT
}


void CAddSpawnGeneratorDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddSpawnGeneratorDlg)
	DDX_Control(pDX, IDC_COMBO_AiCfgDB, m_aiCfgDB);
	DDX_Control(pDX, IDC_COMBO_Trees, m_trees);
	DDX_Control(pDX, IDC_COMBO_TreeLocations, m_treeLocations);
	DDX_Control(pDX, IDC_CHECK_BasedOnRuntime, m_baseOnRuntimeEntity);
	DDX_Text(pDX, IDC_EDIT_GeneratorName, m_generatorName);
	DDX_Text(pDX, IDC_EDIT_MaxOutputCapacity, m_maxOutputCapacity);
	DDX_Text(pDX, IDC_EDIT_PossibilityOfSpawn, m_possibilityOfSpawn);
	DDV_MinMaxInt(pDX, m_possibilityOfSpawn, 0, 1000);
	DDX_Text(pDX, IDC_EDIT_POSX, m_posX);
	DDX_Text(pDX, IDC_EDIT_POSY, m_posY);
	DDX_Text(pDX, IDC_EDIT_POSZ, m_posZ);
	DDX_Text(pDX, IDC_EDIT_SpawnIndex, m_spawnIndex);
	DDX_Text(pDX, IDC_EDIT_SpawnRadius, m_spawnRadius);
	DDX_Text(pDX, IDC_EDIT_SpawnType, m_spawnType);
	DDX_Text(pDX, IDC_EDIT_AiCfg, m_aiCfg);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddSpawnGeneratorDlg, CDialog)
	//{{AFX_MSG_MAP(CAddSpawnGeneratorDlg)
	ON_BN_CLICKED(IDC_CHECK_BasedOnRuntime, OnCHECKBasedOnRuntime)
	ON_CBN_SELCHANGE(IDC_COMBO_Trees, OnSelchangeCOMBOTrees)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddSpawnGeneratorDlg message handlers

void CAddSpawnGeneratorDlg::OnCHECKBasedOnRuntime() 
{
	if(useRuntimeEntityList == TRUE)
	{
	useRuntimeEntityList = FALSE;
	m_baseOnRuntimeEntity.SetCheck(0);
	}
	else{
    useRuntimeEntityList = TRUE;
	m_baseOnRuntimeEntity.SetCheck(1);
	}
}

BOOL CAddSpawnGeneratorDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(TRUE);
	//spawnObj->
	if(useRuntimeEntityList == TRUE)
	   m_baseOnRuntimeEntity.SetCheck(1);


    if(treeList->GetCount()<=0){
        m_trees.AddString("Not Available");
        m_treeLocations.AddString("Not Available");
		m_trees.SetCurSel(0);
		m_treeLocations.SetCurSel(0);
	}
	else{
        m_trees.AddString("Disabled");
		for(POSITION posLoc = treeList->GetHeadPosition();posLoc != NULL;)
		{
            CAIWebObj * treeObj = (CAIWebObj *)treeList->GetNext(posLoc);
			m_trees.AddString(treeObj->m_name);
		}
		m_trees.SetCurSel(spawnObj->m_treeUse+1);
		InListTreeLocations(spawnObj->m_treePosition+1);
	}

	int tracerTemp = 0;
    for(POSITION posLoc = m_aiCfgDatabaseRef->GetHeadPosition();posLoc != NULL;)
	{
        CAIObj * aiPtr = (CAIObj *)m_aiCfgDatabaseRef->GetNext(posLoc);
        m_aiCfgDB.AddString(ConvertLongToString(tracerTemp) + " " + aiPtr->m_aiCfgName);
		tracerTemp++;
	}
	if(spawnObj->m_aiCfg > -1)
	m_aiCfgDB.SetCurSel(spawnObj->m_aiCfg);
	else
	m_aiCfgDB.SetCurSel(0);

    m_aiCfg = spawnObj->m_aiCfg;
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CAddSpawnGeneratorDlg::InListTreeLocations(int sel)
{
   UpdateData(TRUE);
   int treeIndex = m_trees.GetCurSel();
   m_treeLocations.ResetContent();
   treeIndex--;
   if(treeIndex == -1)
	  return;
   
   POSITION posLoc = treeList->FindIndex(treeIndex);
   if(!posLoc)
	   return;
       CAIWebObj * treeObj = (CAIWebObj *)treeList->GetAt(posLoc);
       m_treeLocations.AddString("Random");
	   
	   int tracerTemp = 0;
	   for(POSITION webPos = treeObj->m_web->GetHeadPosition();webPos != NULL;)
	   {
           CAIWebNode * wNode = (CAIWebNode *)treeObj->m_web->GetNext(webPos);
           m_treeLocations.AddString(ConvertLongToString(tracerTemp) + " " + wNode->m_locationName);
		   tracerTemp++;
	   }

	   if(sel != -1)
		  m_treeLocations.SetCurSel(sel);
	UpdateData(FALSE);
}
void CAddSpawnGeneratorDlg::OnSelchangeCOMBOTrees() 
{
	InListTreeLocations(0);
}

CString CAddSpawnGeneratorDlg::ConvertLongToString(int number)
{    
	          int decimal,sign;
              char *buffer;
              CString temp;	 
	          buffer = _fcvt(number,0,&decimal,&sign);
    	      temp = buffer;
			  if(sign != 0)
                 temp = operator+("-",temp);
			  return temp;
}

void CAddSpawnGeneratorDlg::OnOK() 
{
	UpdateData(TRUE);
	int treeIndex = m_trees.GetCurSel();
	if(treeIndex != -1)
	   treeIndex--;

    int location = m_treeLocations.GetCurSel();
	if(location != -1)
	   location--;

	   spawnObj->m_treeUse               = treeIndex;//-1 = none
       spawnObj->m_treePosition          = location;//-1 = rendom
       spawnObj->m_aiCfg                 = m_aiCfgDB.GetCurSel();
	
	UpdateData(FALSE);
	CDialog::OnOK();
}
