// VertexAnimSettingsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "direct.h"
#include "CEXMeshClass.h"
#include "VertexAnimSettingsDlg.h"
#include ".\vertexanimsettingsdlg.h"


// CVertexAnimSettingsDlg dialog

IMPLEMENT_DYNAMIC(CVertexAnimSettingsDlg, CDialog)
CVertexAnimSettingsDlg::CVertexAnimSettingsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVertexAnimSettingsDlg::IDD, pParent)
	, m_timeBetweenFrames(0)
{
}

CVertexAnimSettingsDlg::~CVertexAnimSettingsDlg()
{



}

void CVertexAnimSettingsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_LOOPING, m_enableLooping);
	DDX_Text(pDX, IDC_EDIT_timeBetweenFrames, m_timeBetweenFrames);
	DDX_Control(pDX, IDC_LIST_fileList, m_fileList);
}


BEGIN_MESSAGE_MAP(CVertexAnimSettingsDlg, CDialog)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON_AddFiles, OnBnClickedButtonAddfiles)
	ON_BN_CLICKED(IDC_BUTTON_ClearFiles, OnBnClickedButtonClearfiles)
END_MESSAGE_MAP()


// CVertexAnimSettingsDlg message handlers

BOOL CVertexAnimSettingsDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	UpdateData(TRUE);
	if(!m_animModuleRef)
	    m_animModuleRef = new CVertexAnimationModule();

	m_enableLooping.SetCheck(m_animModuleRef->m_looping);
	m_timeBetweenFrames = m_animModuleRef->m_timeBetweenFrames;

	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CVertexAnimSettingsDlg::OnBnClickedOk()
{
	UpdateData(TRUE);
	m_animModuleRef->m_looping = m_enableLooping.GetCheck();
	m_animModuleRef->m_timeBetweenFrames = m_timeBetweenFrames;
	
	
	if(m_animModuleRef->m_frameCount == 0)
	{
       m_animModuleRef->SafeDelete();
	   delete m_animModuleRef;
	   m_animModuleRef = 0;
	}
	
	
	
	UpdateData(FALSE);
	OnOK();
}

void CVertexAnimSettingsDlg::OnBnClickedButtonAddfiles()
{

         char bigBuff[20000] = "";  // maximum common dialog buffer size
         //use mapsmodels folder for organization
         CString CurDir;
         CString MainDir;
         CString Mdir;
         char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 Mdir = CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
         //end use mapsmodels folder for organization
	     static char BASED_CODE filter[] = "Engine Models|*.arw;*.x;*.3ds;*.fbx;*.obj;*.gltf;*.glb;*.dae;*.stl;*.ply;*.blend;*.ase;*.lwo;*.dxf|All Files (*.*)|*.*||";
	     CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_ALLOWMULTISELECT |OFN_FILEMUSTEXIST, filter, this );
         opendialog.m_ofn.hInstance = AfxGetInstanceHandle();
	     opendialog.m_ofn.lpstrFile = bigBuff;
	     opendialog.m_ofn.nMaxFile = sizeof(bigBuff);
	     if ( opendialog.DoModal() == IDOK )
		 {//file chosen
			 m_animModuleRef->SafeDelete();
	         //solisify directory
		     char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 CurDir = buffer;
		     //-------------build selected files database------------//
		     CString FileNames[2000];
	         int counter = 0;
	         int length = 0;
	         int length2 = 0;
			 int loop=0;
	         for(loop=0;loop<15000;loop++){
		        if(bigBuff[loop] == NULL)
				{
		           counter++;
		           if(bigBuff[loop+1] == NULL)
                      break; 
		              loop++;
				}
		        CString temp = (CString)bigBuff[loop];
		        FileNames[counter] = operator+(FileNames[counter],temp);
			 }
	         if(counter == 1){
                FileNames[1] = opendialog.GetFileName();
	            counter++;
			 }
		     SetCurrentDirectory(CurDir);
	         //---------end-build selected files database------------//
		     for(loop=1;loop<counter;loop++){//begin file load loop
                 
				 CEXMeshObj * tempMesh = new CEXMeshObj();
				 tempMesh->List_materialObject = new CMaterialObject();
				 float range = 50000.0f;
				 BOOL result = tempMesh->LoadGeometryFromPlugin(FileNames[loop],&range,g_pd3dDevice,NULL);
				 if(!result)
					 MessageBox("Failed to load file: " + FileNames[loop]);

				 m_animModuleRef->AddKeyFrame(&tempMesh->m_abVertexArray);

				 m_fileList.AddString(FileNames[loop]);

				 tempMesh->SafeDelete();
				 delete tempMesh;
				 tempMesh = 0;
				 //CObjectLibraryObj * newPtr = new CObjectLibraryObj();
                 /*if(newPtr->InitFromFile(FileNames[loop],g_pd3dDeviceRef)){//success
                    lPtr->m_collection->AddTail(newPtr);
			        newPtr->m_wldObject->m_fileNameRef = FileNames[loop];
				 }//end success
		         else{
                     MessageBox("Failed to load file");
				 }*/
			 }
	 
		 }// end file chosen		  
      SetCurrentDirectory(MainDir);

}

void CVertexAnimSettingsDlg::OnBnClickedButtonClearfiles()
{
	m_animModuleRef->SafeDelete();
}
