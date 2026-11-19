// MissileAddBillboardDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "d3d9.h"
#include "MissileAddBillboardDlg.h"
#include "MaterialDlg.h"
#include "math.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMissileAddBillboardDlg dialog


CMissileAddBillboardDlg::CMissileAddBillboardDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMissileAddBillboardDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMissileAddBillboardDlg)
	m_billboardSize = 0.0f;
	//}}AFX_DATA_INIT
}


void CMissileAddBillboardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMissileAddBillboardDlg)
	DDX_Control(pDX, IDC_BUTTON_Material, m_materialButton);
	DDX_Control(pDX, IDC_COMBO_Textures, m_textureList);
	DDX_Text(pDX, IDC_EDIT_BillboardSize, m_billboardSize);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMissileAddBillboardDlg, CDialog)
	//{{AFX_MSG_MAP(CMissileAddBillboardDlg)
	ON_BN_CLICKED(IDC_BUTTON_Material, OnBUTTONMaterial)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMissileAddBillboardDlg message handlers

BOOL CMissileAddBillboardDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateData(TRUE);
	
	
	
	if(!m_missileRef){//disable certain functions
        m_materialButton.EnableWindow(FALSE);
		m_billboardSize = 10.0f;
	    for(POSITION posLoc = textureDBref->GetHeadPosition();posLoc != NULL;)
		{
           CTextureDBObj * texPtr = (CTextureDBObj *)textureDBref->GetNext(posLoc);
           m_textureList.AddString(texPtr->List_textureName);
		}
	}//disable certain functions
	else if(m_missileRef->m_basicMesh){
            if(!m_missileRef->m_basicMesh->m_abVertexArray.m_vertexArray1)
			{
				m_missileRef->m_basicMesh->m_abVertexArray.SafeDelete();

                AfxMessageBox("Rebuilding");
				
                ABVERTEX1L * vertexArray  = new ABVERTEX1L[4];
	            ZeroMemory(vertexArray, sizeof(D3DVERTEX) * 4);

	            WORD *      indecieArray = new WORD[6];
	            ZeroMemory(indecieArray, sizeof(WORD) * 6);

	            WORD        vertexCount  = 4;
	            WORD        indecieCount = 6;
    
	            //vertex 1 upper left
	            vertexArray[0].x  = 5;
	            vertexArray[0].y  = 5;
	            vertexArray[0].z  = 0.0f;
	            vertexArray[0].tx0.tu = 1.0f;
	            vertexArray[0].tx0.tv = 0.0f;
	            vertexArray[0].nx = 0.0f;
	            vertexArray[0].ny = 0.0f;
	            vertexArray[0].nz = 1.0f;
	            //vertex 2 bottom left
	            vertexArray[1].x  = 5;
	            vertexArray[1].y  = -5;
	            vertexArray[1].z  = 0.0f;
	            vertexArray[1].tx0.tu = 0.0f;
	            vertexArray[1].tx0.tv = 0.0f;
	            vertexArray[1].nx = 0.0f;
	            vertexArray[1].ny = 0.0f;
	            vertexArray[1].nz = 1.0f;
	            //vertex 3 bottom right
	            vertexArray[2].x  = -5;
	            vertexArray[2].y  = -5;
	            vertexArray[2].z  = 0.0f;
	            vertexArray[2].tx0.tu = 0.0f;
	            vertexArray[2].tx0.tv = 1.0f;
	            vertexArray[2].nx = 0.0f;
	            vertexArray[2].ny = 0.0f;
	            vertexArray[2].nz = 1.0f;
	            //vertex 4 upper right
	            vertexArray[3].x  = -5;
	            vertexArray[3].y  = 5;
	            vertexArray[3].z  = 0.0f;
	            vertexArray[3].tx0.tu = 1.0f;
	            vertexArray[3].tx0.tv = 1.0f;
	            vertexArray[3].nx = 0.0f;
	            vertexArray[3].ny = 0.0f;
	            vertexArray[3].nz = 1.0f;
	            //indecies
	            //no facing
	            indecieArray[0] = 1;
	            indecieArray[1] = 0;
	            indecieArray[2] = 3;
	            indecieArray[3] = 2;
	            indecieArray[4] = 1;
	            indecieArray[5] = 3;

	            m_missileRef->m_basicMesh->SetGeometry2("NONE",
							                             vertexArray,
							                             vertexCount,
							                             indecieArray,
							                             indecieCount);
                 //newMissilePtr->m_basicMesh->InitBuffer(g_pd3dDevice);





			}
           
            m_billboardSize = fabs(m_missileRef->m_basicMesh->m_abVertexArray.m_vertexArray1[0].x)*2.0f;

			m_textureList.EnableWindow(FALSE);
	}
	
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMissileAddBillboardDlg::OnOK() 
{
	UpdateData(TRUE);
	m_textureIndexRet = m_textureList.GetCurSel();
	

	UpdateData(FALSE);
	CDialog::OnOK();
}

void CMissileAddBillboardDlg::OnBUTTONMaterial() 
{
	CMaterialDlg dlg;
	dlg.m_pxShaderSystemRef = NULL;
	dlg.m_vxShaderSystemRef = NULL;
	dlg.m_globalShaderRef = NULL;
	dlg.GL_textureDataBase = textureDBref;
	if(!m_missileRef->m_basicMesh)
		return;
	dlg.meshRef = m_missileRef->m_basicMesh;
	dlg.DoModal();
	
}
