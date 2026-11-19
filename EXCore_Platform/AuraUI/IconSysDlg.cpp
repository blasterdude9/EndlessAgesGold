// IconSysDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include <d3d9.h>
#include "direct.h"
#include "IconSysDlg.h"
#include "CEXMeshClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIconSysDlg dialog


CIconSysDlg::CIconSysDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CIconSysDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CIconSysDlg)
	m_gridDensity = 0.0f;
	m_iconName = _T("");
	m_iconSize = 0.0f;
	m_xIndex = 0.0f;
	m_yIndex = 0.0f;
	//}}AFX_DATA_INIT
}


void CIconSysDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIconSysDlg)
	DDX_Control(pDX, IDC_COMBO_TextureDatabase, m_textureDatabaseCombo);
	DDX_Control(pDX, IDC_LIST_IconDatabase, m_iconList);
	DDX_Text(pDX, IDC_EDIT_GridDensity, m_gridDensity);
	DDX_Text(pDX, IDC_EDIT_IconName, m_iconName);
	DDX_Text(pDX, IDC_EDIT_IconSize, m_iconSize);
	DDX_Text(pDX, IDC_EDIT_Xindex, m_xIndex);
	DDX_Text(pDX, IDC_EDIT_Yindex, m_yIndex);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIconSysDlg, CDialog)
	//{{AFX_MSG_MAP(CIconSysDlg)
	ON_BN_CLICKED(IDC_BUTTON_AccessMaterial, OnBUTTONAccessMaterial)
	ON_BN_CLICKED(IDC_BUTTON_AddIcon, OnBUTTONAddIcon)
	ON_BN_CLICKED(IDC_BUTTON_DeleteIcon, OnBUTTONDeleteIcon)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceIcon, OnBUTTONReplaceIcon)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, OnButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnButtonSave)
	ON_LBN_SELCHANGE(IDC_LIST_IconDatabase, OnSelchangeLISTIconDatabase)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIconSysDlg message handlers

BOOL CIconSysDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InListTexturedatabase(0);
	InListIcons(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
//-------------------------------------------------------//
//-------------------------------------------------------//
//-------------------------------------------------------//
void CIconSysDlg::InListIcons(int sel)
{
	 m_iconList.ResetContent();
	 int tracer = 0;
     for(POSITION posLoc = m_iconDatabaseREF->GetHeadPosition();posLoc != NULL;)
	 {
         CIconSysObj * iconPtr = (CIconSysObj *) m_iconDatabaseREF->GetNext(posLoc);
         m_iconList.AddString((ConvToString(tracer) + " " + iconPtr->m_iconName));
		 tracer++;
	 }
	 if(sel > -1)
		m_iconList.SetCurSel(sel);
}
//-----------END FUNCTION--------------------------------------//
//-------------------------------------------------------------//
//-----converts a long into string-----------------------------//
CString CIconSysDlg::ConvToString(int number)
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
//-------------------------------------------------------//
//-------------------------------------------------------//
//-------------------------------------------------------//
void CIconSysDlg::InListTexturedatabase(int sel)
{
     m_textureDatabaseCombo.ResetContent();
	 for(POSITION posLoc = m_texturedatabaseREF->GetHeadPosition();posLoc != NULL;)
	 {
         CTextureDBObj * texture = (CTextureDBObj *) m_texturedatabaseREF->GetNext(posLoc);
         m_textureDatabaseCombo.AddString(texture->List_textureName);
	 }
	 if(sel > -1)
		m_textureDatabaseCombo.SetCurSel(sel);
}
//-------------------------------------------------------//
//-------------------------------------------------------//
//-------------------------------------------------------//
void CIconSysDlg::OnBUTTONAccessMaterial() 
{
	
	int iconSelection = m_iconList.GetCurSel();
	if(iconSelection == -1)
	{
       AfxMessageBox("Must select a icon");
	   return;
	}

	POSITION posLoc = m_iconDatabaseREF->FindIndex(iconSelection);
	if(!posLoc)
		return;

	    CIconSysObj * iconPtr = (CIconSysObj *)m_iconDatabaseREF->GetAt(posLoc);
	
		CMaterialDlg dlg;
		dlg.m_pxShaderSystemRef = NULL;
		dlg.m_vxShaderSystemRef = NULL;
		dlg.m_globalShaderRef = NULL;
        dlg.GL_textureDataBase = m_texturedatabaseREF;
        CEXMeshObj * refMesh = new CEXMeshObj();
	    
		refMesh->List_textureSelect[0] = iconPtr->m_textureIndex;
		if(refMesh->List_materialObject){
		   delete refMesh->List_materialObject;
           refMesh->List_materialObject = 0;
		}

		if(!iconPtr->m_materialObj)
			iconPtr->m_materialObj = new CMaterialObject();

		iconPtr->m_materialObj->Clone(&refMesh->List_materialObject);
		
		dlg.meshRef = refMesh;
	    dlg.DoModal();

        iconPtr->m_textureIndex = refMesh->List_textureSelect[0];
		if(iconPtr->m_textureIndex > -1)
		{
		   POSITION txtPos = m_texturedatabaseREF->FindIndex(iconPtr->m_textureIndex);
           if(txtPos)
		   {
               CTextureDBObj * txtPtr = (CTextureDBObj *) m_texturedatabaseREF->GetAt(txtPos);
               iconPtr->m_textureName = txtPtr->List_textureName;
		   }
		}
        if(iconPtr->m_materialObj){
		   delete iconPtr->m_materialObj;
		   iconPtr->m_materialObj = 0;
		}

		refMesh->List_materialObject->Clone(&iconPtr->m_materialObj);
	
		refMesh->SafeDelete();
		delete refMesh;
		refMesh = 0;
}
//-------------------------------------------------------//
//-------------------------------------------------------//
//-------------------------------------------------------//
void CIconSysDlg::OnBUTTONAddIcon() 
{
	UpdateData(TRUE);
	CIconSysObj * icnPtr    = new CIconSysObj();
	
	icnPtr->m_gridDensity   = m_gridDensity;
	icnPtr->m_iconName      = m_iconName;
	icnPtr->m_size          = m_iconSize;
	icnPtr->m_xIndex        = m_xIndex;
	icnPtr->m_yIndex        = m_yIndex;

	m_iconDatabaseREF->AddTail(icnPtr);

    UpdateData(FALSE);
	InListIcons(m_iconDatabaseREF->GetCount()-1);
}
//-------------------------------------------------------//
//-------------------------------------------------------//
//-------------------------------------------------------//
void CIconSysDlg::OnBUTTONDeleteIcon() 
{
	int iconSelection = m_iconList.GetCurSel();
	if(iconSelection == -1)
	{
		AfxMessageBox("Must select an icon");
		return;
	}

	POSITION posLoc = m_iconDatabaseREF->FindIndex(iconSelection);
	if(!posLoc)
		return;

	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;
	
	CIconSysObj * iconPtr = (CIconSysObj *)m_iconDatabaseREF->GetAt(posLoc);
	iconPtr->SafeDelete();
	delete iconPtr;
	iconPtr = 0;
	m_iconDatabaseREF->RemoveAt(posLoc);

	InListIcons(iconSelection-1);
}
//-------------------------------------------------------//
//-------------------------------------------------------//
//-------------------------------------------------------//
void CIconSysDlg::OnBUTTONReplaceIcon() 
{
	int iconSelection = m_iconList.GetCurSel();
	if(iconSelection == -1)
	{
       AfxMessageBox("Must select a icon");
	   return;
	}

	POSITION posLoc = m_iconDatabaseREF->FindIndex(iconSelection);
	if(!posLoc)
		return;

	UpdateData(TRUE);
	    CIconSysObj * iconPtr = (CIconSysObj *)m_iconDatabaseREF->GetAt(posLoc);
		iconPtr->m_gridDensity   = m_gridDensity;
	    iconPtr->m_iconName      = m_iconName;
	    iconPtr->m_size          = m_iconSize;
	    iconPtr->m_xIndex        = m_xIndex;
	    iconPtr->m_yIndex        = m_yIndex;
    UpdateData(FALSE);

	InListIcons(iconSelection);
}
//-------------------------------------------------------//
//-------------------------------------------------------//
//-------------------------------------------------------//
void CIconSysDlg::OnButtonLoad() 
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

   static char BASED_CODE filter[] = "Icon DB Files|*.IDB|All Files (*.*)|*.*||";
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
	   m_iconDatabaseREF->SafeDelete();
	   delete m_iconDatabaseREF;
	   m_iconDatabaseREF = 0;
	   the_in_Archive >> m_iconDatabaseREF;
	   the_in_Archive.Close();
       fl.Close();
	 }
     SetCurrentDirectory(MainDir);
	 InListIcons(0);
}
//-------------------------------------------------------//
//-------------------------------------------------------//
//-------------------------------------------------------//
void CIconSysDlg::OnButtonSave() 
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
     static char BASED_CODE filter[] = "Icon DB Files|*.IDB|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "IDB", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_iconDatabaseREF;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}
//-------------------------------------------------------//
//-------------------------------------------------------//
//-------------------------------------------------------//
void CIconSysDlg::OnSelchangeLISTIconDatabase() 
{
	int iconSelection = m_iconList.GetCurSel();
	if(iconSelection == -1)
	{
       AfxMessageBox("Must select a icon");
	   return;
	}

	POSITION posLoc = m_iconDatabaseREF->FindIndex(iconSelection);
	if(!posLoc)
		return;

	UpdateData(TRUE);
	    CIconSysObj * iconPtr = (CIconSysObj *)m_iconDatabaseREF->GetAt(posLoc);
		m_gridDensity = iconPtr->m_gridDensity;
	    m_iconName    = iconPtr->m_iconName;
	    m_iconSize    = iconPtr->m_size;
	    m_xIndex      = iconPtr->m_xIndex;
	    m_yIndex      = iconPtr->m_yIndex;
    UpdateData(FALSE);
}
