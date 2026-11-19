// EntityLODSystemsDlg.cpp : implementation file
//
#include "stdafx.h"
#define D3D_OVERLOADS
#include "resource.h"
#include <d3d9.h>
#include <afxtempl.h>
#include "direct.h"
#include "EntityLODSystemsDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CEntityLODSystemsDlg dialog


CEntityLODSystemsDlg::CEntityLODSystemsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CEntityLODSystemsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CEntityLODSystemsDlg)
	m_distance = 0.0f;
	//}}AFX_DATA_INIT
}


void CEntityLODSystemsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CEntityLODSystemsDlg)
	DDX_Control(pDX, IDC_LIST_LODSYSTEMS, m_listLODSystems);
	DDX_Text(pDX, IDC_EDIT_distance, m_distance);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CEntityLODSystemsDlg, CDialog)
	//{{AFX_MSG_MAP(CEntityLODSystemsDlg)
	ON_BN_CLICKED(IDC_BUTTON_ADD_LODSYS, OnButtonAddLodsys)
	ON_BN_CLICKED(IDC_BUTTON_DeleteSystem, OnBUTTONDeleteSystem)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceValues, OnBUTTONReplaceValues)
	ON_LBN_SELCHANGE(IDC_LIST_LODSYSTEMS, OnSelchangeListLodsystems)
	ON_BN_CLICKED(IDC_BUTTON_EDIT_LODSYS, OnButtonEditLodsys)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEntityLODSystemsDlg message handlers

void CEntityLODSystemsDlg::OnButtonAddLodsys() 
{
	UpdateData(TRUE);
	if(!m_lodRangeRefList)
		m_lodRangeRefList = new CSkeletalLODObjectList();
	CSkeletalLODObject * newLODEntity = new CSkeletalLODObject();
	newLODEntity->m_distance = m_distance;
    m_lodRangeRefList->AddTail(newLODEntity);
	UpdateData(FALSE);
	InListLODs(-1);
}


void CEntityLODSystemsDlg::InListLODs(int sel)
{
	UpdateData(TRUE);
	int trace = 0;
    m_listLODSystems.ResetContent();
	if(!m_lodRangeRefList)
		return;

	for(POSITION posLoc = m_lodRangeRefList->GetHeadPosition();posLoc != NULL;)
	{
		CSkeletalLODObject * lodPtr = (CSkeletalLODObject *)m_lodRangeRefList->GetNext(posLoc);
        m_listLODSystems.AddString("LOD");
	}
	if(sel != -1)
    m_listLODSystems.SetCurSel(sel);

	UpdateData(FALSE);
}

void CEntityLODSystemsDlg::OnBUTTONDeleteSystem() 
{
	int listIndex = m_listLODSystems.GetCurSel();
	if(listIndex ==-1){
       MessageBox("must select a LOD");
       return;
	}

	POSITION posLoc = m_lodRangeRefList->FindIndex(listIndex);
	if(!posLoc)
		return;
	CSkeletalLODObject * lodPtr = (CSkeletalLODObject *) m_lodRangeRefList->GetAt(posLoc);
	lodPtr->SafeDelete();
	delete lodPtr;
	m_lodRangeRefList->RemoveAt(posLoc);
	InListLODs(listIndex-1);
}

void CEntityLODSystemsDlg::OnBUTTONReplaceValues() 
{
	int listIndex = m_listLODSystems.GetCurSel();
	if(listIndex ==-1){
       MessageBox("must select a LOD");
       return;
	}

	POSITION posLoc = m_lodRangeRefList->FindIndex(listIndex);
	if(!posLoc)
		return;
	CSkeletalLODObject * lodPtr = (CSkeletalLODObject *) m_lodRangeRefList->GetAt(posLoc);
	UpdateData(TRUE);
    lodPtr->m_distance = m_distance; 
	UpdateData(FALSE);
}

BOOL CEntityLODSystemsDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	InListLODs(-1);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CEntityLODSystemsDlg::OnSelchangeListLodsystems() 
{
	int listIndex = m_listLODSystems.GetCurSel();
	if(listIndex ==-1){
       return;
	}

	POSITION posLoc = m_lodRangeRefList->FindIndex(listIndex);
	if(!posLoc)
		return;
	CSkeletalLODObject * lodPtr = (CSkeletalLODObject *) m_lodRangeRefList->GetAt(posLoc);
	UpdateData(TRUE);
    m_distance = lodPtr->m_distance; 
	UpdateData(FALSE);
	
}

void CEntityLODSystemsDlg::OnButtonEditLodsys() 
{
	UpdateData(TRUE);

    int listIndex = m_listLODSystems.GetCurSel();
	if(listIndex ==-1){
       return;
	}

	POSITION posLoc = m_lodRangeRefList->FindIndex(listIndex);
	if(!posLoc)
		return;
	CSkeletalLODObject * lodPtr = (CSkeletalLODObject *) m_lodRangeRefList->GetAt(posLoc);    

	UpdateData(FALSE);
	InListLODs(listIndex);
}
