// ShaderLibDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include <d3dX9.h>
#include "ShaderLibDlg.h"
#include "direct.h"
#include "globalstatusvariables.h"


// CShaderLibDlg dialog

IMPLEMENT_DYNAMIC(CShaderLibDlg, CDialog)
CShaderLibDlg::CShaderLibDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CShaderLibDlg::IDD, pParent)
	, m_shaderCode(_T(""))
	, m_shaderName(_T(""))
{
}

CShaderLibDlg::~CShaderLibDlg()
{
}

void CShaderLibDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ShaderCode, m_shaderCode);
	DDX_Text(pDX, IDC_SHADERNAMEEDIT, m_shaderName);
	DDX_Control(pDX, IDC_LIST_shaderList, m_shaderList);
	DDX_Control(pDX, IDC_LIST_OutputList, m_outputList);
	DDX_Text(pDX, IDC_STATIC_ModeLabel, m_shaderModeLabel);
	DDX_Control(pDX, IDC_CHECK_FXcheck, m_fxCode);

	DDX_Control(pDX, IDC_SHADERNAMEEDIT, m_shaderNameEditCtrl);
	DDX_Control(pDX, IDC_EDIT_ShaderCode, m_shaderCodeCtrl);
	DDX_Control(pDX, IDC_EDIT_DebugOut, m_debugOutput);
}


BEGIN_MESSAGE_MAP(CShaderLibDlg, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_Update, OnBnClickedButtonUpdate)
	ON_BN_CLICKED(IDC_BUTTON_DELETE, OnBnClickedButtonDelete)
	ON_BN_CLICKED(IDC_BUTTON_SaveShaderLib, OnBnClickedButtonSaveshaderlib)
	ON_BN_CLICKED(IDC_BTN_TestCompile, OnBnClickedBtnTestcompile)
	ON_LBN_SELCHANGE(IDC_LIST_shaderList, OnLbnSelchangeListshaderlist)
	ON_BN_CLICKED(IDC_BUTTON_loadShaderLib, OnBnClickedButtonloadshaderlib)
	ON_BN_CLICKED(IDC_CHECK_FXcheck, OnBnClickedCheckFxcheck)
//	ON_EN_VSCROLL(IDC_EDIT_ShaderCode, &CShaderLibDlg::OnEnVscrollEditShadercode)
ON_EN_CHANGE(IDC_EDIT_ShaderCode, &CShaderLibDlg::OnEnChangeEditShadercode)
ON_BN_CLICKED(IDC_BUTTON_AddShader, &CShaderLibDlg::OnBnClickedButtonAddshader)
END_MESSAGE_MAP()


// CShaderLibDlg message handlers

void CShaderLibDlg::OnBnClickedButtonAdd()
{
	
}

void CShaderLibDlg::OnBnClickedButtonUpdate()
{
	UpdateData(TRUE);
	int sel = m_shaderList.GetCurSel();
	if(sel > -1)
	{
	 CShaderObj* shaderPtr = m_shaderLibRef->GetByIndex(sel);
     if(shaderPtr)
	 {
        shaderPtr->m_shaderCode = m_shaderCode;
        shaderPtr->m_shaderName = m_shaderName;
	 }
	}
    UpdateData(FALSE);
	//InList(sel);
}

void CShaderLibDlg::OnBnClickedButtonDelete()
{
	UpdateData(TRUE);
	int sel = m_shaderList.GetCurSel();
	if(sel > -1)
	   m_shaderLibRef->DeleteByIndex(sel);
    UpdateData(FALSE);
	InList(sel-1);
}

void CShaderLibDlg::OnBnClickedButtonSaveshaderlib()
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
	 LPCTSTR filter = "Vertex Shader Lib|*.SLB|All Files (*.*)|*.*||";
	 CString fltX = "SLB";
	 if(m_shaderTypeMode == 0)
	 {
	  
	 }
	 else{
        filter = "Pixel Shader Lib|*.PSL|All Files (*.*)|*.*||";
		fltX = "PSL";
	 }

	 CFileDialog opendialog( FALSE, fltX, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
	 
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
       the_out_Archive << m_shaderLibRef;
       the_out_Archive.Close();
       fl.Close();
	 }
	 SetCurrentDirectory(MainDir);	
}

void CShaderLibDlg::InList(int sel)
{
	m_shaderList.ResetContent();
     for(POSITION posLoc = m_shaderLibRef->GetHeadPosition();posLoc != NULL;){
         CShaderObj * spnPtr = (CShaderObj *)m_shaderLibRef->GetNext(posLoc);
		 m_shaderList.AddString(spnPtr->m_shaderName);
	}
	 if(sel > -1)
	    m_shaderList.SetCurSel(sel);
}

void CShaderLibDlg::OnBnClickedBtnTestcompile()
{
	/*UpdateData(TRUE);
	HRESULT hr;
	LPDIRECT3DVERTEXSHADER9 m_shader=NULL;
 
	int bufSize = m_shaderCode.GetLength();
    hr = D3DXAssembleShader((LPCTSTR) m_shaderCode,
                                      bufSize,//UINT SrcDataLen,
                                      NULL,//CONST D3DXMACRO* pDefines,
                                      NULL,//LPD3DXINCLUDE pInclude,
                                      0,//DWORD Flags,
                                      (LPD3DXBUFFER*)&m_shader,//LPD3DXBUFFER* ppShader,
                                      NULL);


    UpdateData(FALSE);*/
	m_outputList.ResetContent();
    HRESULT hr;
	int err = 0;
	
	//UpdateData(TRUE);
	//int vScrollPos = m_shaderList.GetScrollPos(SB_VERT);
	//int hScrollPos = m_shaderList.GetScrollPos(SB_HORZ);
	int sel = m_shaderList.GetCurSel();
	CString error;
	if(sel > -1)
	{
	 CShaderObj* shaderPtr = m_shaderLibRef->GetByIndex(sel);
     if(shaderPtr)
	 {
		//UpdateData(TRUE);
		m_shaderCodeCtrl.GetWindowTextA(shaderPtr->m_shaderCode);
        m_shaderNameEditCtrl.GetWindowTextA(shaderPtr->m_shaderName);
		//UpdateData(FALSE);
	    shaderPtr->m_shaderUsedOnce = 0;
		shaderPtr->SafeDelete(); 
        err = shaderPtr->Compile(m_pd3dDevice,&error);
	 }
	}
    //UpdateData(FALSE);
	

    if( err == 0 )
	{
        //m_outputList.AddString("Compile Failed");
		m_debugOutput.SetWindowTextA(error);
	}
	else{
		//m_outputList.AddString("Compiled OK");
		m_debugOutput.SetWindowTextA("Compiled OK : "+error);
	}

	//m_shaderList.SetScrollPos(SB_VERT,vScrollPos,1);
	//m_shaderList.SetScrollPos(SB_HORZ,hScrollPos,1);
}

void CShaderLibDlg::OnLbnSelchangeListshaderlist()
{
	UpdateData(TRUE);
	int sel = m_shaderList.GetCurSel();
	if(sel > -1)
	{
	 CShaderObj* shaderPtr = m_shaderLibRef->GetByIndex(sel);
     if(shaderPtr)
	 {
        m_shaderCode = shaderPtr->m_shaderCode;
        m_shaderName = shaderPtr->m_shaderName;
		m_fxCode.SetCheck(shaderPtr->m_compiler);
	 }
	}
    UpdateData(FALSE);
}

void CShaderLibDlg::OnBnClickedButtonloadshaderlib()
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
    LPCTSTR filter = "Vertex Shader Lib|*.SLB|All Files (*.*)|*.*||";
	 CString fltX = "SLB";
	 if(m_shaderTypeMode == 0)
	 {
	  
	 }
	 else{
        filter = "Pixel Shader Lib|*.PSL|All Files (*.*)|*.*||";
		fltX = "PSL";
	 }

   //static char BASED_CODE filter[] = filter;
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
	   m_shaderLibRef->SafeDelete();
	   delete m_shaderLibRef;
	   m_shaderLibRef = 0;
	   the_in_Archive >> m_shaderLibRef;
	   the_in_Archive.Close();
       fl.Close();

	   GlobalStatusVariables * gStatus = (GlobalStatusVariables *)m_gstatusMem;
       if(m_shaderTypeMode == 1)
	   {//pixel
		   gStatus->m_pxShaderSystem = m_shaderLibRef;
		   gStatus->m_pxShaderSystem->Compile(gStatus->m_dxRenderer->m_pd3dDevice);
		   gStatus->m_renderStateObject->m_pxShderSystemRef = gStatus->m_pxShaderSystem;
	   }//end pixel
	   else
	   {//vertex
           gStatus->m_shaderSystem = m_shaderLibRef;
           gStatus->m_shaderSystem->Compile(gStatus->m_dxRenderer->m_pd3dDevice);
	       gStatus->m_renderStateObject->m_shderSystemRef = gStatus->m_shaderSystem;
	   }//end vertex
	 }

     

     SetCurrentDirectory(MainDir);
	 InList(m_shaderLibRef->GetCount()-1);
}

BOOL CShaderLibDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	InList(m_shaderLibRef->GetCount()-1);
    UpdateData(TRUE);


    if(m_shaderTypeMode == 0)
	   m_shaderModeLabel = "Vertex Shader Mode";
	if(m_shaderTypeMode == 1)
	   m_shaderModeLabel = "Pixel Shader Mode";

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CShaderLibDlg::OnBnClickedCheckFxcheck()
{
	UpdateData(TRUE);
	int sel = m_shaderList.GetCurSel();
	if(sel > -1)
	{
	 CShaderObj* shaderPtr = m_shaderLibRef->GetByIndex(sel);
     if(shaderPtr)
	 {
        shaderPtr->m_compiler = m_fxCode.GetCheck();
	 }
	}
    UpdateData(FALSE);
}

//void CShaderLibDlg::OnEnVscrollEditShadercode()
//{
//	// TODO: Add your control notification handler code here
//}

void CShaderLibDlg::OnEnChangeEditShadercode()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	
}

void CShaderLibDlg::OnBnClickedButtonAddshader()
{
	UpdateData(TRUE);
	m_shaderLibRef->AddShader(m_shaderName, m_shaderCode, m_shaderTypeMode);
	UpdateData(FALSE);
	InList(m_shaderLibRef->GetCount()-1);
}
