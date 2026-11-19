// CursorScaleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "..\EngineMainApp\resource.h"
#include "CursorScaleDlg.h"
#include "direct.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCursorScaleDlg dialog


CCursorScaleDlg::CCursorScaleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCursorScaleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCursorScaleDlg)
	m_scale = 0.0f;
	//}}AFX_DATA_INIT
}


void CCursorScaleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCursorScaleDlg)
	DDX_Text(pDX, IDC_EDIT_Scale, m_scale);
	DDX_Control(pDX, IDC_COMBO_cursorType, m_cursorTypeDList);
    DDX_Control(pDX, IDC_COMBO_cursorClickEffect, m_cursorClickEffectDList);
    DDX_Control(pDX, IDC_COMBO_CursorHoverID, m_cursorHoverIDList);
    DDX_Control(pDX, IDC_LIST_cursorChain, m_additionalCursorList);
	DDX_Control(pDX, IDC_CHECK_cUseexisting, m_checkUseExisting);
    DDX_Text(pDX, IDC_EDIT_limitX, m_limitX);
    DDX_Text(pDX, IDC_EDIT_limitY, m_limitY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCursorScaleDlg, CDialog)
	//{{AFX_MSG_MAP(CCursorScaleDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CCursorScaleDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_mnAddCursor, &CCursorScaleDlg::OnBnClickedButtonmnaddcursor)
	ON_BN_CLICKED(IDC_BUTTON_mnDeleteCursor, &CCursorScaleDlg::OnBnClickedButtonmndeletecursor)
	ON_BN_CLICKED(IDC_BUTTON_mnReplaceValues, &CCursorScaleDlg::OnBnClickedButtonmnreplacevalues)
	ON_BN_CLICKED(IDC_BUTTON_mnEditSkeletal, &CCursorScaleDlg::OnBnClickedButtonmneditskeletal)
	ON_LBN_SELCHANGE(IDC_LIST_cursorChain, &CCursorScaleDlg::OnLbnSelchangeListcursorchain)
	ON_BN_CLICKED(IDC_BUTTON_EditMainCursorSk, &CCursorScaleDlg::OnBnClickedButtonEditmaincursorsk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCursorScaleDlg message handlers




BOOL CCursorScaleDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_cursorHoverIDList.AddString("DISABLED");
	m_cursorHoverIDList.AddString("ENEMY");
    m_cursorHoverIDList.AddString("NPC");
	
	m_cursorHoverIDList.SetCurSel(0);

	m_cursorTypeDList.AddString("GENERAL");
    m_cursorTypeDList.AddString("GAMEPLAY");
	m_cursorTypeDList.AddString("GAMEPLAY-DEBUG");
	m_cursorTypeDList.SetCurSel(m_cursorRef->m_cursorType);

	m_cursorClickEffectDList.AddString("NONE");
	for(POSITION posLoc = m_explosionList->GetHeadPosition();posLoc != NULL;)
	{
        CExplosionObj * ePtr = (CExplosionObj *)m_explosionList->GetNext(posLoc);
        m_cursorClickEffectDList.AddString(ePtr->m_explosionName);
	}
	m_cursorClickEffectDList.SetCurSel(m_cursorRef->m_expEffectOnClick+1);

	//
	if(m_cursorRef)	
	{
	   m_gStatus->SkeletalTextureReassignment(m_cursorRef->m_skeletonObject);
       UpdateData(TRUE);
	    m_scale = m_cursorRef->m_scaleFactor;
        m_limitX = m_cursorRef->m_limitX;
        m_limitY = m_cursorRef->m_limitY;
	   UpdateData(FALSE);


	}

	 if(m_menuRef->m_additionalCursors)
	 {//chain of cursors
        for(POSITION acPos = m_menuRef->m_additionalCursors->GetHeadPosition();acPos != NULL;)
		{
            CBoneCursorObj * lCursorPtr = (CBoneCursorObj *)m_menuRef->m_additionalCursors->GetNext(acPos);
			m_gStatus->SkeletalTextureReassignment(lCursorPtr->m_skeletonObject);
		}
	 }//end chain of cursors


	InListAdditionalCursors(0);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CCursorScaleDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	m_cursorRef->m_scaleFactor = m_scale;
    m_cursorRef->m_limitX = m_limitX;
    m_cursorRef->m_limitY = m_limitY;
	UpdateData(FALSE);
	m_cursorRef->m_cursorType			= m_cursorTypeDList.GetCurSel();
	m_cursorRef->m_expEffectOnClick		= m_cursorClickEffectDList.GetCurSel()-1;
	OnOK();
}

void CCursorScaleDlg::InListAdditionalCursors(int sel)
{

    m_additionalCursorList.ResetContent();
	if(m_menuRef->m_additionalCursors)
	   for(POSITION posLoc = m_menuRef->m_additionalCursors->GetHeadPosition();posLoc != NULL;)
	   {
           CBoneCursorObj * bCursor = (CBoneCursorObj *) m_menuRef->m_additionalCursors->GetNext(posLoc);
           switch(bCursor->m_useTypeID)
		   {
			   case 0:m_additionalCursorList.AddString("DISABLED");break;
			   case 2:m_additionalCursorList.AddString("NPC");break;
			   case 1:m_additionalCursorList.AddString("ENEMY");break;
			   case -1:m_additionalCursorList.AddString("ERROR");break;
		   }
	   }

	   if(sel > -1)
	      m_additionalCursorList.SetCurSel(sel);

}

void CCursorScaleDlg::OnBnClickedButtonmnaddcursor()
{
	if(m_checkUseExisting.GetCheck() == 1)
	{
       CBoneCursorObj * cursorPtr = NULL;
	   //cursorPtr->m_skeletonObject = NULL;
	 
	   m_cursorRef->Clone(&cursorPtr,g_pd3dDeviceRef);
	   cursorPtr->m_useTypeID = m_cursorHoverIDList.GetCurSel();
	   if(!m_menuRef->m_additionalCursors)
		   m_menuRef->m_additionalCursors = new CBoneCursorObjList();
	   m_menuRef->m_additionalCursors->AddTail(cursorPtr);
	   m_gStatus->SkeletalTextureReassignment(cursorPtr->m_skeletonObject);

	   InListAdditionalCursors(m_menuRef->m_additionalCursors->GetCount()-1);

       return;
	}
	
	
	CBoneCursorObj * cursorPtr = new CBoneCursorObj();
	cursorPtr->m_skeletonObject = new CSkeletonObject();
	cursorPtr->m_useTypeID = m_cursorHoverIDList.GetCurSel();
    


	//use mapsmodels folder for organization
    CString CurDir;
    CString MainDir;
    char buffer[_MAX_PATH];
	 _getcwd( buffer, _MAX_PATH );
	 MainDir = CurDir = buffer;
    //end use mapsmodels folder for organization
    static char BASED_CODE filter[] = "ARB Files|*.ARB|ARB Files|*.ARB|All Files (*.*)|*.*||";
    CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
    if ( opendialog.DoModal() == IDOK )
	{ 
         CString filenamePathandAll = opendialog.GetPathName();
         cursorPtr->m_skeletonObject->InitBoneSystem(filenamePathandAll,g_pd3dDeviceRef);
	}
    SetCurrentDirectory(MainDir);
	//
    if(!m_menuRef->m_additionalCursors)
		m_menuRef->m_additionalCursors = new CBoneCursorObjList();

	UpdateData(TRUE);
	cursorPtr->m_scaleFactor = m_scale;
    cursorPtr->m_limitX = m_limitX;
    cursorPtr->m_limitY = m_limitY;
	UpdateData(FALSE);

	m_menuRef->m_additionalCursors->AddTail(cursorPtr);
	m_gStatus->SkeletalTextureReassignment(cursorPtr->m_skeletonObject);
	InListAdditionalCursors(m_menuRef->m_additionalCursors->GetCount()-1);
}

void CCursorScaleDlg::OnBnClickedButtonmndeletecursor()
{
	int selection = m_additionalCursorList.GetCurSel();
	if(selection < 0)
	{
       AfxMessageBox("Must select a cursor");
	   return;
	}

    POSITION posLoc = m_menuRef->m_additionalCursors->FindIndex(selection);
    if(!posLoc)
		return;

	CBoneCursorObj * cursor = (CBoneCursorObj *)m_menuRef->m_additionalCursors->GetAt(posLoc);
    if(!cursor)
	    return;

	cursor->SafeDelete();
	delete cursor;
	cursor = 0;

	m_menuRef->m_additionalCursors->RemoveAt(posLoc);

	if(m_menuRef->m_additionalCursors->GetCount() == 0) 
	{
       delete m_menuRef->m_additionalCursors;
	   m_menuRef->m_additionalCursors = 0;
	}
	InListAdditionalCursors(selection-1);
}

void CCursorScaleDlg::OnBnClickedButtonmnreplacevalues()
{
	int selection = m_additionalCursorList.GetCurSel();
	if(selection < 0)
	{
       AfxMessageBox("Must select a cursor");
	   return;
	}

    POSITION posLoc = m_menuRef->m_additionalCursors->FindIndex(selection);
    if(!posLoc)
		return;

	CBoneCursorObj * cursor = (CBoneCursorObj *)m_menuRef->m_additionalCursors->GetAt(posLoc);
    if(!cursor)
	    return;

	cursor->m_useTypeID = m_cursorHoverIDList.GetCurSel();
	UpdateData(TRUE);
	cursor->m_scaleFactor = m_scale;
    cursor->m_limitX = m_limitX;
    cursor->m_limitY = m_limitY;
	UpdateData(FALSE);
	
	InListAdditionalCursors(selection);
	
}

void CCursorScaleDlg::OnBnClickedButtonmneditskeletal()
{
    int selection = m_additionalCursorList.GetCurSel();
	if(selection < 0)
	{
       AfxMessageBox("Must select a cursor");
	   return;
	}

    POSITION posLoc = m_menuRef->m_additionalCursors->FindIndex(selection);
    if(!posLoc)
		return;

	CBoneCursorObj * cursor = (CBoneCursorObj *)m_menuRef->m_additionalCursors->GetAt(posLoc);
    if(!cursor)
	    return;



	CSkeletalAnimationDlg dlg;
	dlg.m_renderStateObjectRef	= m_renderStateObjectRef;
	dlg.m_globalShaderRef		= m_cgGlobalShaderDB;
	dlg.m_skillDatabaseREF		= m_skillDatabaseREF;
	dlg.m_elementDBREF			= m_elementDBREF;
	dlg.GL_textureDataBase		= m_textureDataBase;
	dlg.m_skeletonReferencePtr	= cursor->m_skeletonObject;
	dlg.directSoundRef			= m_dirSndRef;
	dlg.DoModal();
}

void CCursorScaleDlg::OnLbnSelchangeListcursorchain()
{
	int selection = m_additionalCursorList.GetCurSel();
	if(selection < 0)
	{
	   return;
	}

    POSITION posLoc = m_menuRef->m_additionalCursors->FindIndex(selection);
    if(!posLoc)
		return;

	CBoneCursorObj * cursor = (CBoneCursorObj *)m_menuRef->m_additionalCursors->GetAt(posLoc);
    if(!cursor)
	    return;

	m_cursorHoverIDList.SetCurSel(cursor->m_useTypeID);
}

void CCursorScaleDlg::OnBnClickedButtonEditmaincursorsk()
{
	CSkeletalAnimationDlg dlg;
	dlg.m_renderStateObjectRef	= m_renderStateObjectRef;
	dlg.m_globalShaderRef		= m_cgGlobalShaderDB;
	dlg.m_skillDatabaseREF		= m_skillDatabaseREF;
	dlg.m_elementDBREF			= m_elementDBREF;
	dlg.GL_textureDataBase		= m_textureDataBase;
	dlg.m_skeletonReferencePtr	= m_cursorRef->m_skeletonObject;
	dlg.directSoundRef			= m_dirSndRef;
	dlg.DoModal();
}
