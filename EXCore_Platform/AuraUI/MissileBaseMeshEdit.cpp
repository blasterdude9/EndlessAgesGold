// MissileBaseMeshEdit.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "direct.h"
#include "d3d9.h"
#include "MissileBaseMeshEdit.h"
#include "MaterialDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMissileBaseMeshEdit dialog


CMissileBaseMeshEdit::CMissileBaseMeshEdit(CWnd* pParent /*=NULL*/)
	: CDialog(CMissileBaseMeshEdit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMissileBaseMeshEdit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMissileBaseMeshEdit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMissileBaseMeshEdit)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMissileBaseMeshEdit, CDialog)
	//{{AFX_MSG_MAP(CMissileBaseMeshEdit)
	ON_BN_CLICKED(IDC_BUTTON_EditMeshData, OnBUTTONEditMeshData)
	ON_BN_CLICKED(IDC_BUTTON_EditMeshMaterial, OnBUTTONEditMeshMaterial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMissileBaseMeshEdit message handlers

void CMissileBaseMeshEdit::OnBUTTONEditMeshData() 
{	
	        //use mapsmodels folder for organization
            CString CurDir;
            CString MainDir;
            char buffer[_MAX_PATH];
			 _getcwd( buffer, _MAX_PATH );
			 MainDir = CurDir = buffer;
			 CurDir = operator+(CurDir,"\\MapsModels\\");
			 SetCurrentDirectory(CurDir);
             //end use mapsmodels folder for organization
             static char BASED_CODE filter[] = "Engine Models|*.arw;*.x;*.3ds;*.fbx;*.obj;*.gltf;*.glb;*.dae;*.stl;*.ply;*.blend;*.ase;*.lwo;*.dxf|All Files (*.*)|*.*||";
	         CFileDialog opendialog( TRUE, 0, 0, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, filter, this );
             if ( opendialog.DoModal() == IDOK )
			 { 
                 if(m_missileRef->m_basicMesh){
					m_missileRef->m_basicMesh->SafeDelete();
					delete m_missileRef->m_basicMesh;
					m_missileRef->m_basicMesh=0;
				 }

	             CString filenamePathandAll = opendialog.GetPathName();
		           //load imediate mode mesh data
                   m_missileRef->m_basicMesh = new CEXMeshObj();
			       m_missileRef->m_basicMesh->List_materialObject = new CMaterialObject();
                   float sightRange = 0.0f;
                   BOOL result = m_missileRef->m_basicMesh->LoadGeometryFromPlugin(filenamePathandAll,&sightRange,g_pd3dDeviceRef,NULL);
			 
			    SetCurrentDirectory(MainDir);
			 }
			 else{
	            SetCurrentDirectory(MainDir);
			    return;
			 }       
	
}

void CMissileBaseMeshEdit::OnBUTTONEditMeshMaterial() 
{
    if(!m_missileRef->m_basicMesh)
		return;


	CMaterialDlg dlg;
	dlg.m_pxShaderSystemRef = NULL;
	dlg.m_vxShaderSystemRef = NULL;
	dlg.m_globalShaderRef = NULL;
	dlg.GL_textureDataBase = GL_textureDataBase;
	dlg.meshRef = m_missileRef->m_basicMesh;
	dlg.DoModal();


}

BOOL CMissileBaseMeshEdit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
