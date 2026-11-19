// VisualManagerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "direct.h"
#include "VisualManagerDlg.h"
#include "MaterialDlg.h"
#include "CAssimpModelImporter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CVisualManagerDlg dialog


CVisualManagerDlg::CVisualManagerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CVisualManagerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CVisualManagerDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CVisualManagerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CVisualManagerDlg)
	DDX_Control(pDX, IDC_LIST_Visuals, m_visualList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CVisualManagerDlg, CDialog)
	//{{AFX_MSG_MAP(CVisualManagerDlg)
	ON_BN_CLICKED(IDC_BUTTON_AddVisual, OnBUTTONAddVisual)
	ON_BN_CLICKED(IDC_BUTTON_DeleteVisual, OnBUTTONDeleteVisual)
	ON_BN_CLICKED(IDC_BUTTON_EditMaterial, OnBUTTONEditMaterial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CVisualManagerDlg message handlers

void CVisualManagerDlg::OnBUTTONAddVisual() 
{
	//char temp;
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
		  SetCurrentDirectory(MainDir);
	      //---------end-build selected files database------------//
		  for(loop=1;loop<counter;loop++){//begin file load loop
			  SetCurrentDirectory(CurDir);//set to base location
			  if(CAssimpModelImporter::IsSupportedFile(FileNames[loop]))
			  {
			      CAssimpModelImporter::ImportToMeshList(FileNames[loop],g_pd3dDeviceRef,m_exMeshListREF);
			  }
			  else
			  {
			      CEXMeshObj * retBasisLoc = new CEXMeshObj();
			      retBasisLoc->List_materialObject = new CMaterialObject();
			      float sightRange = 0.0f;
			      retBasisLoc->LoadGeometryFromPlugin(FileNames[loop],&sightRange,g_pd3dDeviceRef,NULL);//proprietory file format
			      m_exMeshListREF->AddTail(retBasisLoc);
			  }
			  SetCurrentDirectory(Mdir);//set to maps and models 
		  }
	  }// end file chosen		  
      SetCurrentDirectory(MainDir);
	  InList(m_exMeshListREF->GetCount()-1);
}

void CVisualManagerDlg::OnBUTTONDeleteVisual() 
{
	int selection = m_visualList.GetCurSel();
	if(selection == -1)
	{
		AfxMessageBox("Must select a visual");
		return;
	}

	POSITION posLoc = m_exMeshListREF->FindIndex(selection);
	if(!posLoc)
		return;

	if(AfxMessageBox("Confirm Delete?", MB_OKCANCEL, 0) != IDOK)
		return;

	CEXMeshObj * mPtr = (CEXMeshObj *)m_exMeshListREF->GetAt(posLoc);
	mPtr->SafeDelete();
	delete mPtr;
	mPtr = 0;

	m_exMeshListREF->RemoveAt(posLoc);
	InList(selection-1);
}

BOOL CVisualManagerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	InList(0);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CVisualManagerDlg::InList(int sel)
{
   m_visualList.ResetContent();
   for(POSITION posLoc = m_exMeshListREF->GetHeadPosition();posLoc != NULL;)
   {
       CEXMeshObj * mPtr = (CEXMeshObj *)m_exMeshListREF->GetNext(posLoc);
       m_visualList.AddString("MESH");
   }
   if(sel > -1)
   m_visualList.SetCurSel(sel);
}

void CVisualManagerDlg::OnBUTTONEditMaterial() 
{
	int selection = m_visualList.GetCurSel();
	if(selection == -1)
	{
      AfxMessageBox("Must select a visual");
      return;
	}

    POSITION posLoc = m_exMeshListREF->FindIndex(selection);
    if(!posLoc)
		return;
	
	    CEXMeshObj * mPtr = (CEXMeshObj *)m_exMeshListREF->GetAt(posLoc);
        
		CMaterialDlg dlg;
		dlg.m_pxShaderSystemRef = NULL;
		dlg.m_vxShaderSystemRef = NULL;
		dlg.m_globalShaderRef = NULL;
	    dlg.GL_textureDataBase = GL_textureDataBase;
        dlg.meshRef = mPtr;
	    if(dlg.DoModal()==IDOK){
		   mPtr->List_materialObject->Reset();
		}
    
}
