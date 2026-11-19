// PostEffectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CBaseInterfaceModule.h"
#include "PostEffectDlg.h"
#include "direct.h"
#include ".\posteffectdlg.h"


// CPostEffectDlg dialog

IMPLEMENT_DYNAMIC(CPostEffectDlg, CDialog)
CPostEffectDlg::CPostEffectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPostEffectDlg::IDD, pParent)
	, m_postEffectName(_T(""))
{
}

CPostEffectDlg::~CPostEffectDlg()
{
}

void CPostEffectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_pixelShaderSelection, m_pixelShaderList);
	DDX_Text(pDX, IDC_EDIT_postEffectName, m_postEffectName);
	DDX_Control(pDX, IDC_COMBO_TextureSelectLevel1, m_textureSelection1);
	DDX_Control(pDX, IDC_COMBO_textureSelectLevel2, m_textureSelection2);
	DDX_Control(pDX, IDC_COMBO_textureSelectLevel3, m_textureSelection3);
	DDX_Control(pDX, IDC_COMBO_textureSelectLevel4, m_textureSelection4);
	DDX_Control(pDX, IDC_COMBO_textureSelectLevel5, m_textureSelection5);
	DDX_Control(pDX, IDC_COMBO_textureSelectLevel6, m_textureSelection6);
	DDX_Control(pDX, IDC_COMBO_textureSelectLevel7, m_textureSelection7);
	DDX_Control(pDX, IDC_LIST_postEffectList, m_postEffectList);
}


BEGIN_MESSAGE_MAP(CPostEffectDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_ApplyLive, OnBnClickedButtonApplylive)
	ON_BN_CLICKED(IDC_BUTTON_AddNew, OnBnClickedButtonAddnew)
	ON_BN_CLICKED(IDC_BUTTON_Replace, OnBnClickedButtonReplace)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_DisablePostEffects, OnBnClickedButtonDisableposteffects)
	ON_LBN_SELCHANGE(IDC_LIST_postEffectList, OnLbnSelchangeListposteffectlist)
	ON_BN_CLICKED(IDC_BUTTON_Export, OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_Import, OnBnClickedButtonImport)
END_MESSAGE_MAP()


// CPostEffectDlg message handlers

void CPostEffectDlg::OnBnClickedButtonApplylive()
{
	 int sel = m_postEffectList.GetCurSel();
	 if(sel > -1)
	 {
        m_processModuleRef->SetEffect(sel);
	 }
	 else
        AfxMessageBox("Must select a post effect to do this operation");
}

void CPostEffectDlg::OnBnClickedButtonAddnew()
{
	CPostProcessModule * pEffect = new CPostProcessModule();
	pEffect->m_material = new CMaterialObject();

    UpdateData(TRUE);
	pEffect->m_processName = m_postEffectName;
	pEffect->m_assignIndex[0] = GetSelHelperTextures(&m_textureSelection1);
	pEffect->m_assignIndex[1] = GetSelHelperTextures(&m_textureSelection2);
	pEffect->m_assignIndex[2] = GetSelHelperTextures(&m_textureSelection3);
	pEffect->m_assignIndex[3] = GetSelHelperTextures(&m_textureSelection4);
	pEffect->m_assignIndex[4] = GetSelHelperTextures(&m_textureSelection5);
	pEffect->m_assignIndex[5] = GetSelHelperTextures(&m_textureSelection6);
	pEffect->m_assignIndex[6] = GetSelHelperTextures(&m_textureSelection7);

	pEffect->m_material->m_pixelShader = m_pixelShaderList.GetCurSel()-1;

    UpdateData(FALSE);

	m_processModuleRef->AddTail(pEffect);
	InListPostEffects(m_processModuleRef->GetCount()-1);
}

int CPostEffectDlg::GetSelHelperTextures(CComboBox * comboBox)
{

    int sel = comboBox->GetCurSel();
	if(sel < 0)
	   return -1;

	if(sel == 0)
	   return -5;

	if(sel == 1)
	   return -1;

	return sel - 2;
}

void CPostEffectDlg::SetSelHelperTextures(CComboBox * comboBox, int sel)
{
    
    if(sel == -5)
	   comboBox->SetCurSel(0);
    else if(sel == -1)
	   comboBox->SetCurSel(1);
	else
	   comboBox->SetCurSel(sel+2);
}

void CPostEffectDlg::OnBnClickedButtonReplace()
{
	int effectSel = m_postEffectList.GetCurSel();
    if(effectSel < 0)
	{
		AfxMessageBox("Must select an effect");
		return;
	}

	CPostProcessModule * pEffect = m_processModuleRef->GetByIndex(effectSel);
    if(pEffect)
	{
	   UpdateData(TRUE);
	   pEffect->m_processName	 = m_postEffectName;
       pEffect->m_assignIndex[0] = GetSelHelperTextures(&m_textureSelection1);
	   pEffect->m_assignIndex[1] = GetSelHelperTextures(&m_textureSelection2);
	   pEffect->m_assignIndex[2] = GetSelHelperTextures(&m_textureSelection3);
	   pEffect->m_assignIndex[3] = GetSelHelperTextures(&m_textureSelection4);
	   pEffect->m_assignIndex[4] = GetSelHelperTextures(&m_textureSelection5);
	   pEffect->m_assignIndex[5] = GetSelHelperTextures(&m_textureSelection6);
	   pEffect->m_assignIndex[6] = GetSelHelperTextures(&m_textureSelection7);

	   pEffect->m_material->m_pixelShader = m_pixelShaderList.GetCurSel()-1;
	   UpdateData(FALSE);
       
	}
	InListPostEffects(effectSel);
}

void CPostEffectDlg::OnBnClickedButtonDelete()
{
	int effectSel = m_postEffectList.GetCurSel();
    if(effectSel < 0)
	{
		AfxMessageBox("Must select an effect");
		return;
	}

	POSITION posLoc = m_processModuleRef->FindIndex(effectSel);
	if(!posLoc)
		return;

    CPostProcessModule * pEffect = (CPostProcessModule *)m_processModuleRef->GetAt(posLoc);
    delete pEffect;
	pEffect=0;
	m_processModuleRef->RemoveAt(posLoc);
	InListPostEffects(effectSel-1);

}

void CPostEffectDlg::InListPostEffects(int sel)
{
    UpdateData(TRUE);
    m_postEffectList.ResetContent();
    for(POSITION posLoc = m_processModuleRef->GetHeadPosition();posLoc != NULL;)
	{
        CPostProcessModule * pEffect = (CPostProcessModule *)m_processModuleRef->GetNext(posLoc);

		m_postEffectList.AddString(pEffect->m_processName);
	}

	if(sel > -1)
	{
        m_postEffectList.SetCurSel(sel);
	}
	UpdateData(FALSE);
}

void CPostEffectDlg::InListAvailShaders(int sel)
{
    UpdateData(TRUE);
    m_pixelShaderList.ResetContent();
	m_pixelShaderList.AddString("NONE");
    for(POSITION posLoc = m_pxShaderSystemRef->GetHeadPosition();posLoc != NULL;)
	{
        CShaderObj * shader = (CShaderObj *)m_pxShaderSystemRef->GetNext(posLoc);

		m_pixelShaderList.AddString(shader->m_shaderName);
	}

	if(sel > -1)
	{
        m_pixelShaderList.SetCurSel(sel);
	}
	UpdateData(FALSE);
}

void CPostEffectDlg::InListTextureData(int sel)
{
	UpdateData(TRUE);
     m_textureSelection1.ResetContent();
	 m_textureSelection2.ResetContent();
	 m_textureSelection3.ResetContent();
	 m_textureSelection4.ResetContent();
	 m_textureSelection5.ResetContent();
	 m_textureSelection6.ResetContent();
	 m_textureSelection7.ResetContent();

	     m_textureSelection1.AddString("RENDER TARGET");
         m_textureSelection2.AddString("RENDER TARGET");
		 m_textureSelection3.AddString("RENDER TARGET");
		 m_textureSelection4.AddString("RENDER TARGET");
		 m_textureSelection5.AddString("RENDER TARGET");
		 m_textureSelection6.AddString("RENDER TARGET");
		 m_textureSelection7.AddString("RENDER TARGET");
		 m_textureSelection1.AddString("NONE");
         m_textureSelection2.AddString("NONE");
		 m_textureSelection3.AddString("NONE");
		 m_textureSelection4.AddString("NONE");
		 m_textureSelection5.AddString("NONE");
		 m_textureSelection6.AddString("NONE");
		 m_textureSelection7.AddString("NONE");

     for(POSITION posLoc = m_textureDataBaseRef->GetHeadPosition();posLoc != NULL;)
	 {
         CTextureDBObj * texture = (CTextureDBObj *)m_textureDataBaseRef->GetNext(posLoc);

		 m_textureSelection1.AddString(texture->List_textureName);
         m_textureSelection2.AddString(texture->List_textureName);
		 m_textureSelection3.AddString(texture->List_textureName);
		 m_textureSelection4.AddString(texture->List_textureName);
		 m_textureSelection5.AddString(texture->List_textureName);
		 m_textureSelection6.AddString(texture->List_textureName);
		 m_textureSelection7.AddString(texture->List_textureName);
	 }

	 if(sel > -1)
	 {
         m_textureSelection1.SetCurSel(sel);
         m_textureSelection2.SetCurSel(sel);
		 m_textureSelection3.SetCurSel(sel);
		 m_textureSelection4.SetCurSel(sel);
		 m_textureSelection5.SetCurSel(sel);
		 m_textureSelection6.SetCurSel(sel);
		 m_textureSelection7.SetCurSel(sel);
	 }
    UpdateData(FALSE);
}

BOOL CPostEffectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InListTextureData(0);
	InListAvailShaders(0);
	InListPostEffects(-1);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CPostEffectDlg::OnBnClickedButtonDisableposteffects()
{
	m_processModuleRef->SetEffect(-1);
}

void CPostEffectDlg::OnLbnSelchangeListposteffectlist()
{
	int effectSel = m_postEffectList.GetCurSel();
    if(effectSel < 0)
	{
		AfxMessageBox("Must select an effect");
		return;
	}

	CPostProcessModule * pEffect = m_processModuleRef->GetByIndex(effectSel);
    if(pEffect)
	{
       UpdateData(TRUE);
       m_postEffectName			= pEffect->m_processName;

       SetSelHelperTextures(&m_textureSelection1,pEffect->m_assignIndex[0]);
	   SetSelHelperTextures(&m_textureSelection2,pEffect->m_assignIndex[1]);
	   SetSelHelperTextures(&m_textureSelection3,pEffect->m_assignIndex[2]);
	   SetSelHelperTextures(&m_textureSelection4,pEffect->m_assignIndex[3]);
	   SetSelHelperTextures(&m_textureSelection5,pEffect->m_assignIndex[4]);
	   SetSelHelperTextures(&m_textureSelection6,pEffect->m_assignIndex[5]);
	   SetSelHelperTextures(&m_textureSelection7,pEffect->m_assignIndex[6]);

	   m_pixelShaderList.SetCurSel(pEffect->m_material->m_pixelShader+1);
	   UpdateData(FALSE);
       
	}
}

void CPostEffectDlg::OnBnClickedButtonExport()
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
     static char BASED_CODE filter[] = "Post Process Files|*.PEF|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "PEF", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
     if ( opendialog.DoModal() == IDOK )
	 {
	   CString fileName = opendialog.GetPathName();
	   CFileException exc;
       CFile fl;
       BOOL res = fl.Open(fileName,CFile::modeCreate | CFile::modeWrite, &exc);
       if(res == FALSE){
		  SetCurrentDirectory(MainDir);
	      return;
	   }
       CArchive the_out_Archive(&fl,CArchive::store);
       the_out_Archive << m_processModuleRef;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

void CPostEffectDlg::OnBnClickedButtonImport()
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

   static char BASED_CODE filter[] = "Post Effect Files|*.PEF|All Files (*.*)|*.*||";
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
	   delete m_processModuleRef;
	   m_processModuleRef = 0;
	   the_in_Archive >> m_processModuleRef;
	   the_in_Archive.Close();
       fl.Close();
	   *m_processModuleMemoryRef = m_processModuleRef;
	 }
     SetCurrentDirectory(MainDir);
	 InListPostEffects(-1);
}
