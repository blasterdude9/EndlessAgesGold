// GenerateMeshDLG.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "GenerateMeshDLG.h"
#include "direct.h"
#include <d3d9.h>

#include "CStructuresMisl.h"
#include "CEXMeshClass.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// GenerateMeshDLG dialog


GenerateMeshDLG::GenerateMeshDLG(CWnd* pParent /*=NULL*/)
	: CDialog(GenerateMeshDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(GenerateMeshDLG)
	m_meshWidth = 0.0f;
	m_meshHeight = 0.0f;
	//}}AFX_DATA_INIT
}


void GenerateMeshDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(GenerateMeshDLG)
	DDX_Text(pDX, IDC_EDIT_GenerateMesh_Height, m_meshWidth);
	DDX_Text(pDX, IDC_EDIT_GenerateMesh_Width,  m_meshHeight);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(GenerateMeshDLG, CDialog)
	//{{AFX_MSG_MAP(GenerateMeshDLG)
	ON_BN_CLICKED(IDC_BTN_GenerateMesh_SaveAs, OnBTNGenerateMeshSaveAs)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// GenerateMeshDLG message handlers

void GenerateMeshDLG::OnBTNGenerateMeshSaveAs() 
{
	UpdateData(TRUE);
	
	ABVERTEX *arbVerts = NULL;
	arbVerts = new ABVERTEX[4];
	
	ZeroMemory(arbVerts, sizeof(ABVERTEX)*4);

	arbVerts[0].x = -(m_meshWidth/2);
	arbVerts[0].y =  (m_meshHeight/2);

	arbVerts[1].x =  (m_meshWidth/2);
	arbVerts[1].y =  (m_meshHeight/2);

	arbVerts[2].x = -(m_meshWidth/2);
	arbVerts[2].y = -(m_meshHeight/2);

	arbVerts[3].x =  (m_meshWidth/2);
	arbVerts[3].y = -(m_meshHeight/2);

	WORD *Indecies = NULL;
	Indecies = new WORD[4];

	Indecies[0] = 0;
	Indecies[1] = 1;
	Indecies[2] = 2;

	Indecies[3] = 2;
	Indecies[4] = 1;
	Indecies[5] = 3;

	CEXMeshObj *newMesh = NULL;
	newMesh = new CEXMeshObj();
	
	newMesh->SetGeometry
		(
		"",
		arbVerts,
		WORD(4),
		Indecies,
		WORD(6)
		);

	static char BASED_CODE filter[] = "ARB Files|*.ARB|All Files|*.*||";
	
	CFileDialog saveAs
		(
		FALSE, 
		"ARB", 
		"newmesh.arb",	
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		filter,
		this
		);

    CString CurDir;
    CString MainDir;

    char buffer[_MAX_PATH];
	_getcwd( buffer, _MAX_PATH );
	MainDir = CurDir = buffer;
	
	CurDir = operator+(CurDir,"\\MapsModels\\");
	SetCurrentDirectory(CurDir);
    
	if ( saveAs.DoModal() == IDOK )
		{
		
		CFile savedMesh;
		savedMesh.Open( saveAs.GetFileName(), CFile::modeCreate | CFile::modeWrite );
		
		CArchive archive(&savedMesh,CArchive::store);
		archive << newMesh;
		archive.Close();
		
		savedMesh.Close();
		
		//test output
		CEXMeshObj *testMesh = NULL;
		testMesh = new CEXMeshObj();

		CFile openedMesh;
		openedMesh.Open( saveAs.GetFileName(), CFile::modeRead );

		CArchive inArchive( &openedMesh, CArchive::load );
		inArchive >> testMesh;
		inArchive.Close();

		openedMesh.Close();

		}

	SetCurrentDirectory(MainDir);
	
}

