// MaterialShaderLibDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "MaterialShaderLibDlg.h"
#include "direct.h"
#include "ColorDlg.h"
#include ".\materialshaderlibdlg.h"


// CMaterialShaderLibDlg dialog

IMPLEMENT_DYNAMIC(CMaterialShaderLibDlg, CDialog)
CMaterialShaderLibDlg::CMaterialShaderLibDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMaterialShaderLibDlg::IDD, pParent)
	, m_envUvSet(0)
	, m_shaderName(_T(""))
	, m_materialNotes(_T(""))
{
}

CMaterialShaderLibDlg::~CMaterialShaderLibDlg()
{
}



void CMaterialShaderLibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_Level1_TextureSelect, m_level1TextureSelection);
	DDX_Control(pDX, IDC_COMBO_Level2_TextureSelect, m_level2TextureSelection);
	DDX_Control(pDX, IDC_COMBO_Level3_TextureSelect, m_level3TextureSelection);
	DDX_Control(pDX, IDC_COMBO_Level4_TextureSelect, m_level4TextureSelection);
	DDX_Control(pDX, IDC_COMBO_Level5_TextureSelect, m_level5TextureSelection);
	DDX_Control(pDX, IDC_LIST_shaderList, m_shaderList);
	DDX_Control(pDX, IDC_COMBO_BlendLevel1, m_level1Blend);
	DDX_Control(pDX, IDC_COMBO_BlendLevel2, m_level2Blend);
	DDX_Control(pDX, IDC_COMBO_BlendLevel3, m_level3Blend);
	DDX_Control(pDX, IDC_COMBO_BlendLevel4, m_level4Blend);
	DDX_Control(pDX, IDC_COMBO_BlendLevel5, m_level5Blend);
	DDX_Text(pDX, IDC_EDIT_UVSetIndex, m_envUvSet);
	DDX_Control(pDX, IDC_CHECK_EnableMaterialOvverride, m_enableMaterialOvverride);
	DDX_Control(pDX, IDC_CHECK_EnableEnvMapping, m_enableEnvMapping);
	DDX_Text(pDX, IDC_EDIT_shadername, m_shaderName);
	DDX_Control(pDX, IDC_COMBO_pixelShaderList, m_pixelShaderDList);
	DDX_Control(pDX, IDC_COMBO_VertexShaderList, m_vertexShaderDList);
	DDX_Control(pDX, IDC_COMBO_VSSHD_RQ, m_vsReqDList);
	DDX_Control(pDX, IDC_COMBO_PIXSHD_RQ, m_psReqDList);
	DDX_Control(pDX, IDC_LIST_shaderChainList, m_shaderChainList);
    DDX_Text(pDX, IDC_EDIT_ShaderNotes, m_materialNotes);
	
}


BEGIN_MESSAGE_MAP(CMaterialShaderLibDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_AddNewShader, OnBnClickedButtonAddnewshader)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceShader, OnBnClickedButtonReplaceshader)
	ON_BN_CLICKED(IDC_BUTTON_DeleteShader, OnBnClickedButtonDeleteshader)
	ON_BN_CLICKED(IDC_CHECK_EnableMaterialOvverride, OnBnClickedCheckEnablematerialovverride)
	ON_BN_CLICKED(IDC_CHECK_EnableEnvMapping, OnBnClickedCheckEnableenvmapping)
	ON_BN_CLICKED(IDC_BUTTON_AmbientAccess, OnBnClickedButtonAmbientaccess)
	ON_LBN_SELCHANGE(IDC_LIST_shaderList, OnLbnSelchangeListshaderlist)
	ON_BN_CLICKED(IDC_BUTTON_DiffuseAccess, OnBnClickedButtonDiffuseaccess)
	ON_BN_CLICKED(IDC_BUTTON_EmmisiveAccess, OnBnClickedButtonEmmisiveaccess)
	ON_BN_CLICKED(IDC_BUTTON_SpecularAccess, OnBnClickedButtonSpecularaccess)
	ON_BN_CLICKED(IDC_BUTTON_LoadShader, OnBnClickedButtonLoadshader)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_AddShaderToChain, &CMaterialShaderLibDlg::OnBnClickedButtonAddshadertochain)
	ON_BN_CLICKED(IDC_BUTTON_DelShaderFromChain, &CMaterialShaderLibDlg::OnBnClickedButtonDelshaderfromchain)
	ON_BN_CLICKED(IDC_BUTTON_ReplaceShaderOnChain, &CMaterialShaderLibDlg::OnBnClickedButtonReplaceshaderonchain)
	ON_LBN_SELCHANGE(IDC_LIST_shaderChainList, &CMaterialShaderLibDlg::OnLbnSelchangeListshaderchainlist)
END_MESSAGE_MAP()


// CMaterialShaderLibDlg message handlers

BOOL CMaterialShaderLibDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	//fill pixel shaders
	m_pixelShaderDList.AddString("NONE");
    if(m_pixelShaderLibRef)
	for(POSITION pxLoc = m_pixelShaderLibRef->GetHeadPosition();pxLoc != NULL;)
	{
        CShaderObj * shdPtr = (CShaderObj *)m_pixelShaderLibRef->GetNext(pxLoc);
        m_pixelShaderDList.AddString(shdPtr->m_shaderName);
	}
	m_pixelShaderDList.SetCurSel(0);

	m_vertexShaderDList.AddString("NONE");
	if(m_vertexShaderLibRef)
	for(POSITION pxLoc = m_vertexShaderLibRef->GetHeadPosition();pxLoc != NULL;)
	{
        CShaderObj * shdPtr = (CShaderObj *)m_vertexShaderLibRef->GetNext(pxLoc);
        m_vertexShaderDList.AddString(shdPtr->m_shaderName);
	}
	m_vertexShaderDList.SetCurSel(0);

	m_vsReqDList.AddString("1.0");//0
	m_vsReqDList.AddString("1.1");//1
	m_vsReqDList.AddString("1.4");//2
	m_vsReqDList.AddString("2.0");//3
	m_vsReqDList.AddString("2.1");//4
	m_vsReqDList.AddString("3.0");//5
	m_vsReqDList.SetCurSel(0);

	m_psReqDList.AddString("1.0");
	m_psReqDList.AddString("1.1");
	m_psReqDList.AddString("1.4");
	m_psReqDList.AddString("2.0");
	m_psReqDList.AddString("2.1");
	m_psReqDList.AddString("3.0");
	m_psReqDList.SetCurSel(0);


	if(GL_textureDataBase)
	{//texture fill
		int trace=0;
		m_level1TextureSelection.AddString("USE EXISTING");
	    m_level2TextureSelection.AddString("USE EXISTING");
		m_level3TextureSelection.AddString("USE EXISTING");
	    m_level4TextureSelection.AddString("USE EXISTING");
		m_level5TextureSelection.AddString("USE EXISTING");
        for(POSITION posLoc = GL_textureDataBase->GetHeadPosition();posLoc!=NULL;)
		{
			CTextureDBObj * texture = (CTextureDBObj *)GL_textureDataBase->GetNext(posLoc);
            CString num = operator+(cablib->ConvToString(trace)," ");
			num = operator+(num,texture->List_textureName);
			m_level1TextureSelection.AddString(num);
	        m_level2TextureSelection.AddString(num);
		    m_level3TextureSelection.AddString(num);
	        m_level4TextureSelection.AddString(num);
		    m_level5TextureSelection.AddString(num);
			trace++;
		}
	 }//end texture fill

	        m_level1TextureSelection.SetCurSel(0);
	        m_level2TextureSelection.SetCurSel(0);
		    m_level3TextureSelection.SetCurSel(0);
	        m_level4TextureSelection.SetCurSel(0);
		    m_level5TextureSelection.SetCurSel(0);

			m_level1Blend.SetCurSel(0);
	        m_level2Blend.SetCurSel(0);
		    m_level3Blend.SetCurSel(0);
	        m_level4Blend.SetCurSel(0);
		    m_level5Blend.SetCurSel(0);


	InListShaders(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CMaterialShaderLibDlg::InListShaders(int sel)
{
	m_shaderList.ResetContent();
     for(POSITION posLoc = m_globalShaderRef->GetHeadPosition();posLoc != NULL;)
	 {
         CGlobalMaterialShader * shdObj = (CGlobalMaterialShader*) m_globalShaderRef->GetNext(posLoc);
         m_shaderList.AddString(shdObj->m_shaderName);
	 }
    if(sel > -1)
	   m_shaderList.SetCurSel(sel);
}

void CMaterialShaderLibDlg::InListShaderChain(int sel)
{
	m_shaderChainList.ResetContent();
    int selIndex = m_shaderList.GetCurSel();
    if(selIndex == -1)
	{
       return;
	}

	POSITION posLoc = m_globalShaderRef->FindIndex(selIndex);
	if(!posLoc)
		return;
    CGlobalMaterialShader * shdObj = (CGlobalMaterialShader*) m_globalShaderRef->GetAt(posLoc);

	int trace = 0;
	if(shdObj->m_shaderChain)
     for(POSITION posLoc = shdObj->m_shaderChain->GetHeadPosition();posLoc != NULL;)
	 {
         CShaderDefObj * shdDPtr = (CShaderDefObj*) shdObj->m_shaderChain->GetNext(posLoc);
		 
		 m_shaderChainList.AddString("Shader Set:"+cablib->ConvToString(trace));
		 trace++;
	 }
    
	 if(sel > -1)
	   m_shaderChainList.SetCurSel(sel);
}

void CMaterialShaderLibDlg::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialog::OnOK();
}

void CMaterialShaderLibDlg::OnBnClickedButtonAddnewshader()
{
	CGlobalMaterialShader * shdObj = new CGlobalMaterialShader();
    UpdateData(TRUE);

	shdObj->m_shaderName = m_shaderName;
	shdObj->m_enableChroming        = m_enableEnvMapping.GetCheck();
    shdObj->m_useOverrideMaterial   = m_enableMaterialOvverride.GetCheck();
    shdObj->m_chromeUvSet           = m_envUvSet;
	shdObj->m_shaderNotes           = m_materialNotes;
	shdObj->m_textureIndexArray[0]  = m_level1TextureSelection.GetCurSel()-1;
	shdObj->m_textureIndexArray[1]  = m_level2TextureSelection.GetCurSel()-1;
	shdObj->m_textureIndexArray[2]  = m_level3TextureSelection.GetCurSel()-1;
	shdObj->m_textureIndexArray[3]  = m_level4TextureSelection.GetCurSel()-1;
	shdObj->m_textureIndexArray[4]  = m_level5TextureSelection.GetCurSel()-1;

	shdObj->m_textureStringArray[0] = GL_textureDataBase->GetNameByIndex(m_level1TextureSelection.GetCurSel()-1);
	shdObj->m_textureStringArray[1] = GL_textureDataBase->GetNameByIndex(m_level2TextureSelection.GetCurSel()-1);
	shdObj->m_textureStringArray[2] = GL_textureDataBase->GetNameByIndex(m_level3TextureSelection.GetCurSel()-1);
	shdObj->m_textureStringArray[3] = GL_textureDataBase->GetNameByIndex(m_level4TextureSelection.GetCurSel()-1);
	shdObj->m_textureStringArray[4] = GL_textureDataBase->GetNameByIndex(m_level5TextureSelection.GetCurSel()-1);

	shdObj->m_blendArray[0] = m_level1Blend.GetCurSel()-1;
	shdObj->m_blendArray[1] = m_level2Blend.GetCurSel()-1;
	shdObj->m_blendArray[2] = m_level3Blend.GetCurSel()-1;
	shdObj->m_blendArray[3] = m_level4Blend.GetCurSel()-1;
	shdObj->m_blendArray[4] = m_level5Blend.GetCurSel()-1;


	for(int j=0;j<10;j++)
	{
        if(shdObj->m_textureIndexArray[j] != -1)
           shdObj->m_requiredUvCount = j+1;
	}

	m_globalShaderRef->AddTail(shdObj);
    UpdateData(FALSE);
	InListShaders(m_globalShaderRef->GetCount()-1);
}

void CMaterialShaderLibDlg::OnBnClickedButtonReplaceshader()
{
	int selIndex = m_shaderList.GetCurSel();
    if(selIndex == -1)
	{
       AfxMessageBox("Must select a shader");
       return;
	}

	POSITION posLoc = m_globalShaderRef->FindIndex(selIndex);
	if(!posLoc)
		return;
    CGlobalMaterialShader * shdObj = (CGlobalMaterialShader*) m_globalShaderRef->GetAt(posLoc);
    UpdateData(TRUE);
	shdObj->m_shaderName            = m_shaderName;
	shdObj->m_enableChroming        = m_enableEnvMapping.GetCheck();
    shdObj->m_useOverrideMaterial   = m_enableMaterialOvverride.GetCheck();
    shdObj->m_chromeUvSet           = m_envUvSet;
	shdObj->m_shaderNotes           = m_materialNotes;
	shdObj->m_textureIndexArray[0]  = m_level1TextureSelection.GetCurSel()-1;
	shdObj->m_textureIndexArray[1]  = m_level2TextureSelection.GetCurSel()-1;
	shdObj->m_textureIndexArray[2]  = m_level3TextureSelection.GetCurSel()-1;
	shdObj->m_textureIndexArray[3]  = m_level4TextureSelection.GetCurSel()-1;
	shdObj->m_textureIndexArray[4]  = m_level5TextureSelection.GetCurSel()-1;

	shdObj->m_textureStringArray[0] = GL_textureDataBase->GetNameByIndex(m_level1TextureSelection.GetCurSel()-1);
	shdObj->m_textureStringArray[1] = GL_textureDataBase->GetNameByIndex(m_level2TextureSelection.GetCurSel()-1);
	shdObj->m_textureStringArray[2] = GL_textureDataBase->GetNameByIndex(m_level3TextureSelection.GetCurSel()-1);
	shdObj->m_textureStringArray[3] = GL_textureDataBase->GetNameByIndex(m_level4TextureSelection.GetCurSel()-1);
	shdObj->m_textureStringArray[4] = GL_textureDataBase->GetNameByIndex(m_level5TextureSelection.GetCurSel()-1);

	shdObj->m_blendArray[0] = m_level1Blend.GetCurSel()-1;
	shdObj->m_blendArray[1] = m_level2Blend.GetCurSel()-1;
	shdObj->m_blendArray[2] = m_level3Blend.GetCurSel()-1;
	shdObj->m_blendArray[3] = m_level4Blend.GetCurSel()-1;
	shdObj->m_blendArray[4] = m_level5Blend.GetCurSel()-1;

	for(int j=0;j<10;j++)
	{
        if(shdObj->m_textureIndexArray[j] != -1)
           shdObj->m_requiredUvCount = j+1;
	}


    UpdateData(FALSE);
	InListShaders(selIndex);
}

void CMaterialShaderLibDlg::OnBnClickedButtonDeleteshader()
{
	int selIndex = m_shaderList.GetCurSel();
    if(selIndex == -1)
	{
       AfxMessageBox("Must select a shader");
       return;
	}

	POSITION posLoc = m_globalShaderRef->FindIndex(selIndex);
	if(!posLoc)
		return;
    CGlobalMaterialShader * shdObj = (CGlobalMaterialShader*) m_globalShaderRef->GetAt(posLoc);
	delete shdObj;
	shdObj = 0;
    m_globalShaderRef->RemoveAt(posLoc);
	InListShaders(selIndex-1);
}

void CMaterialShaderLibDlg::OnBnClickedCheckEnablematerialovverride()
{
	// TODO: Add your control notification handler code here
}

void CMaterialShaderLibDlg::OnBnClickedCheckEnableenvmapping()
{
	// TODO: Add your control notification handler code here
}

void CMaterialShaderLibDlg::OnBnClickedButtonAmbientaccess()
{
int selIndex = m_shaderList.GetCurSel();
    if(selIndex == -1)
	{
       AfxMessageBox("Must select a shader");
       return;
	}

	POSITION posLoc = m_globalShaderRef->FindIndex(selIndex);
	if(!posLoc)
		return;
    CGlobalMaterialShader * shdObj = (CGlobalMaterialShader*) m_globalShaderRef->GetAt(posLoc);


	UpdateData(TRUE);
CColorDlg dlg;
dlg.m_red   = shdObj->m_overrideMaterial.Ambient.r;
dlg.m_green = shdObj->m_overrideMaterial.Ambient.g; 
dlg.m_blue  = shdObj->m_overrideMaterial.Ambient.b;
dlg.m_alpha = shdObj->m_overrideMaterial.Ambient.a;
dlg.DoModal();
shdObj->m_overrideMaterial.Ambient.r = dlg.m_red;
shdObj->m_overrideMaterial.Ambient.g = dlg.m_green; 
shdObj->m_overrideMaterial.Ambient.b = dlg.m_blue;
shdObj->m_overrideMaterial.Ambient.a = dlg.m_alpha;
UpdateData(FALSE);		
}

void CMaterialShaderLibDlg::OnLbnSelchangeListshaderlist()
{
	int selIndex = m_shaderList.GetCurSel();
    if(selIndex == -1)
	   return;

	POSITION posLoc = m_globalShaderRef->FindIndex(selIndex);
	if(!posLoc)
		return;
    CGlobalMaterialShader * shdObj = (CGlobalMaterialShader*) m_globalShaderRef->GetAt(posLoc);
    UpdateData(TRUE);
    m_enableEnvMapping.SetCheck(shdObj->m_enableChroming);
    m_enableMaterialOvverride.SetCheck(shdObj->m_useOverrideMaterial);
    m_envUvSet = shdObj->m_chromeUvSet;
	m_shaderName =  shdObj->m_shaderName;
	m_materialNotes = shdObj->m_shaderNotes;


    m_level1TextureSelection.SetCurSel(shdObj->m_textureIndexArray[0]+1);
	m_level2TextureSelection.SetCurSel(shdObj->m_textureIndexArray[1]+1);
	m_level3TextureSelection.SetCurSel(shdObj->m_textureIndexArray[2]+1);
	m_level4TextureSelection.SetCurSel(shdObj->m_textureIndexArray[3]+1);
	m_level5TextureSelection.SetCurSel(shdObj->m_textureIndexArray[4]+1);

	m_level1Blend.SetCurSel(shdObj->m_blendArray[0]+1);
	m_level2Blend.SetCurSel(shdObj->m_blendArray[1]+1);
	m_level3Blend.SetCurSel(shdObj->m_blendArray[2]+1);
	m_level4Blend.SetCurSel(shdObj->m_blendArray[3]+1);
	m_level5Blend.SetCurSel(shdObj->m_blendArray[4]+1);
	UpdateData(FALSE);
	InListShaderChain(0);

}

void CMaterialShaderLibDlg::OnBnClickedButtonDiffuseaccess()
{
	int selIndex = m_shaderList.GetCurSel();
    if(selIndex == -1)
	{
       AfxMessageBox("Must select a shader");
       return;
	}

	POSITION posLoc = m_globalShaderRef->FindIndex(selIndex);
	if(!posLoc)
		return;
    CGlobalMaterialShader * shdObj = (CGlobalMaterialShader*) m_globalShaderRef->GetAt(posLoc);


	UpdateData(TRUE);
CColorDlg dlg;
dlg.m_red   = shdObj->m_overrideMaterial.Diffuse.r;
dlg.m_green = shdObj->m_overrideMaterial.Diffuse.g; 
dlg.m_blue  = shdObj->m_overrideMaterial.Diffuse.b;
dlg.m_alpha = shdObj->m_overrideMaterial.Diffuse.a;
dlg.DoModal();
shdObj->m_overrideMaterial.Diffuse.r = dlg.m_red;
shdObj->m_overrideMaterial.Diffuse.g = dlg.m_green; 
shdObj->m_overrideMaterial.Diffuse.b = dlg.m_blue;
shdObj->m_overrideMaterial.Diffuse.a = dlg.m_alpha;
UpdateData(FALSE);		
}

void CMaterialShaderLibDlg::OnBnClickedButtonEmmisiveaccess()
{
int selIndex = m_shaderList.GetCurSel();
    if(selIndex == -1)
	{
       AfxMessageBox("Must select a shader");
       return;
	}

	POSITION posLoc = m_globalShaderRef->FindIndex(selIndex);
	if(!posLoc)
		return;
    CGlobalMaterialShader * shdObj = (CGlobalMaterialShader*) m_globalShaderRef->GetAt(posLoc);


	UpdateData(TRUE);
CColorDlg dlg;
dlg.m_red   = shdObj->m_overrideMaterial.Emissive.r;
dlg.m_green = shdObj->m_overrideMaterial.Emissive.g; 
dlg.m_blue  = shdObj->m_overrideMaterial.Emissive.b;
dlg.m_alpha = shdObj->m_overrideMaterial.Emissive.a;
dlg.DoModal();
shdObj->m_overrideMaterial.Emissive.r = dlg.m_red;
shdObj->m_overrideMaterial.Emissive.g = dlg.m_green; 
shdObj->m_overrideMaterial.Emissive.b = dlg.m_blue;
shdObj->m_overrideMaterial.Emissive.a = dlg.m_alpha;
UpdateData(FALSE);		
}

void CMaterialShaderLibDlg::OnBnClickedButtonSpecularaccess()
{
int selIndex = m_shaderList.GetCurSel();
    if(selIndex == -1)
	{
       AfxMessageBox("Must select a shader");
       return;
	}

	POSITION posLoc = m_globalShaderRef->FindIndex(selIndex);
	if(!posLoc)
		return;
    CGlobalMaterialShader * shdObj = (CGlobalMaterialShader*) m_globalShaderRef->GetAt(posLoc);


	UpdateData(TRUE);
CColorDlg dlg;
dlg.m_red   = shdObj->m_overrideMaterial.Specular.r;
dlg.m_green = shdObj->m_overrideMaterial.Specular.g; 
dlg.m_blue  = shdObj->m_overrideMaterial.Specular.b;
dlg.m_alpha = shdObj->m_overrideMaterial.Specular.a;
dlg.DoModal();
shdObj->m_overrideMaterial.Specular.r = dlg.m_red;
shdObj->m_overrideMaterial.Specular.g = dlg.m_green; 
shdObj->m_overrideMaterial.Specular.b = dlg.m_blue;
shdObj->m_overrideMaterial.Specular.a = dlg.m_alpha;
UpdateData(FALSE);		
}

void CMaterialShaderLibDlg::OnBnClickedButtonLoadshader()
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

   static char BASED_CODE filter[] = "Material Shader Files|*.MSL|All Files (*.*)|*.*||";
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
	   m_globalShaderRef->SafeDelete();
	   delete m_globalShaderRef;
	   m_globalShaderRef = 0;
	   the_in_Archive >> m_globalShaderRef;
	   the_in_Archive.Close();
       fl.Close();

	   //m_globalShaderRef->ScaleForRuntimeCompatibility();
	 }
     SetCurrentDirectory(MainDir);
	 InListShaders(0);
}

void CMaterialShaderLibDlg::OnBnClickedButtonSave()
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
     static char BASED_CODE filter[] = "Material Shader Files|*.MSL|All Files (*.*)|*.*||";
     CFileDialog opendialog( FALSE, "MSL", 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
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
       the_out_Archive << m_globalShaderRef;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

void CMaterialShaderLibDlg::OnBnClickedButtonAddshadertochain()
{
/*DDX_Control(pDX, IDC_COMBO_pixelShaderList, m_pixelShaderDList);
	DDX_Control(pDX, IDC_COMBO_VertexShaderList, m_vertexShaderDList);
	DDX_Control(pDX, IDC_COMBO_VSSHD_RQ, m_vsReqDList);
	DDX_Control(pDX, IDC_COMBO_PIXSHD_RQ, m_psReqDList);
	DDX_Control(pDX, IDC_LIST_shaderChainList, m_shaderChainList);*/


    int selIndex = m_shaderList.GetCurSel();
    if(selIndex == -1)
	{
	   AfxMessageBox("Must select a material shader");
       return;
	}

	POSITION posLoc = m_globalShaderRef->FindIndex(selIndex);
	if(!posLoc)
		return;
    CGlobalMaterialShader * shdObj = (CGlobalMaterialShader*) m_globalShaderRef->GetAt(posLoc);

	UpdateData(TRUE);
	CShaderDefObj * shdDPtr = new CShaderDefObj();
	shdDPtr->m_pixelShader = m_pixelShaderDList.GetCurSel()-1;
	shdDPtr->m_vertexShader = m_vertexShaderDList.GetCurSel()-1;

	CString temp = "";
	m_psReqDList.GetLBText(m_psReqDList.GetCurSel(),temp);
	shdDPtr->m_minimumPixSupport = atof(temp);

	m_vsReqDList.GetLBText(m_vsReqDList.GetCurSel(),temp);
	shdDPtr->m_minimumVSSupport = atof(temp);
	
	if(!shdObj->m_shaderChain)
	    shdObj->m_shaderChain = new CShaderChain();

	shdObj->m_shaderChain->AddTail(shdDPtr);
	UpdateData(FALSE);
	InListShaderChain(shdObj->m_shaderChain->GetCount()-1);
    
}

void CMaterialShaderLibDlg::OnBnClickedButtonDelshaderfromchain()
{
	int selIndex = m_shaderList.GetCurSel();
    if(selIndex == -1)
	{
	   AfxMessageBox("Must select a material");
       return;
	}

	int chainIndex = m_shaderChainList.GetCurSel();
	if(chainIndex == -1)
	{
	   AfxMessageBox("Must select a shader chain object");
       return;
	}

	POSITION posLoc = m_globalShaderRef->FindIndex(selIndex);
	if(!posLoc)
		return;
    
	CGlobalMaterialShader * shdObj = (CGlobalMaterialShader*) m_globalShaderRef->GetAt(posLoc);

	if(!shdObj->m_shaderChain)
		return;

    posLoc = shdObj->m_shaderChain->FindIndex(chainIndex);
	if(!posLoc)
		return;

	CShaderDefObj * shdDPtr = (CShaderDefObj *)shdObj->m_shaderChain->GetAt(posLoc);

	delete shdDPtr;
	shdDPtr = 0;

    shdObj->m_shaderChain->RemoveAt(posLoc);
	if(shdObj->m_shaderChain->GetCount() == 0)
	{
       delete shdObj->m_shaderChain;
       shdObj->m_shaderChain = 0;
	}

	InListShaderChain(chainIndex-1);
}

void CMaterialShaderLibDlg::OnBnClickedButtonReplaceshaderonchain()
{
	int selIndex = m_shaderList.GetCurSel();
    if(selIndex == -1)
	{
	   AfxMessageBox("Must select a material");
       return;
	}

	int chainIndex = m_shaderChainList.GetCurSel();
	if(chainIndex == -1)
	{
	   AfxMessageBox("Must select a shader chain object");
       return;
	}

	

	POSITION posLoc = m_globalShaderRef->FindIndex(selIndex);
	if(!posLoc)
		return;
    
	CGlobalMaterialShader * shdObj = (CGlobalMaterialShader*) m_globalShaderRef->GetAt(posLoc);

    if(!shdObj->m_shaderChain)
	   return;

    posLoc = shdObj->m_shaderChain->FindIndex(chainIndex);
	if(!posLoc)
		return;

	CShaderDefObj * shdDPtr = (CShaderDefObj *)shdObj->m_shaderChain->GetAt(posLoc);

	UpdateData(TRUE);
	shdDPtr->m_pixelShader = m_pixelShaderDList.GetCurSel()-1;
	shdDPtr->m_vertexShader = m_vertexShaderDList.GetCurSel()-1;

	CString temp = "";
	m_psReqDList.GetLBText(m_psReqDList.GetCurSel(),temp);
	shdDPtr->m_minimumPixSupport = atof(temp);

	m_vsReqDList.GetLBText(m_vsReqDList.GetCurSel(),temp);
	shdDPtr->m_minimumVSSupport = atof(temp);
    UpdateData(FALSE);
}


int CMaterialShaderLibDlg::ConvertStVStoIndex(float vsfl)
{
   
	  if(vsfl == 1.0f)return 0;
      if(vsfl == 1.1f)return 1;
	  if(vsfl == 1.4f)return 2;
	  if(vsfl == 2.0f)return 3;
      if(vsfl == 2.1f)return 4;
	  if(vsfl == 3.0f)return 5;
	
	  return 0;
}


void CMaterialShaderLibDlg::OnLbnSelchangeListshaderchainlist()
{
	int selIndex = m_shaderList.GetCurSel();
    if(selIndex == -1)
	{
       return;
	}

	int chainIndex = m_shaderChainList.GetCurSel();
	if(chainIndex == -1)
	{
       return;
	}

	POSITION posLoc = m_globalShaderRef->FindIndex(selIndex);
	if(!posLoc)
		return;
    
	CGlobalMaterialShader * shdObj = (CGlobalMaterialShader*) m_globalShaderRef->GetAt(posLoc);

    if(!shdObj->m_shaderChain)
	   return;

    posLoc = shdObj->m_shaderChain->FindIndex(chainIndex);
	if(!posLoc)
		return;

	CShaderDefObj * shdDPtr = (CShaderDefObj *)shdObj->m_shaderChain->GetAt(posLoc);

	UpdateData(TRUE);
	m_pixelShaderDList.SetCurSel(shdDPtr->m_pixelShader+1);
	m_vertexShaderDList.SetCurSel(shdDPtr->m_vertexShader+1);
    m_vsReqDList.SetCurSel(ConvertStVStoIndex(shdDPtr->m_minimumVSSupport));
    m_psReqDList.SetCurSel(ConvertStVStoIndex(shdDPtr->m_minimumPixSupport));
    UpdateData(FALSE);
}
